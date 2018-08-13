---
layout: post
title:  "TFRecord读写"
categories: tensorflow 
tags:  tensorflow dataset
author: canisn
---

* content
{:toc}

TFRecord是TensorFlow官方推荐的数据格式，文件采用二进制定长存储，理论上读写速度可以提高，具体待我测试一波。目前已知的是从CSV格式到TFRecord格式，文件的大小至少增加5倍。且文件转换时数据逐行操作，速度非常慢。好处是由于它是类似json格式，可以存储非结构化数据，这个在处理变长特征时好用。





虽然我觉得不好用，但是还是先测试一下。

首先是文件的写入，即TFRecord文件制作。本文采用从一个原始日志文件中读取数据，挑选需要的特征列并做一些调整后再转换成TFRecord格式。

```python
def clean_data(raw_data_path, clean_data_path, file_name, use_cols, column_names, na_values):
    df = pd.read_csv(os.path.join(raw_data_path, file_name),
                     usecols=use_cols,
                     names=column_names,
                     delimiter='\t',
                     header=0,
                     skiprows=False)

    df = df.fillna(value=na_values)
    df['uid'] = df['uid'].apply(lambda x: abs(x) % 20000000).astype(int)
    df['iid'] = df['iid'].apply(lambda x: abs(x) % 10000000).astype(int)
    df['score'] = df['score'].apply(lambda x: abs(x) > 0.000001).astype(int)
    df['net_env'] = df['net_env'].apply(lambda x: abs(x) % 100).astype(int)
    df['position'] = df['position'].apply(lambda x: x % 100).astype(int)
    df['time'] = df['time'].apply(lambda x: x % 86400 // 60).astype(int)
    df['post_tid'] = df['post_tid'].apply(lambda x: x % 1000000).astype(int)
    df['post_type'] = df['post_type'].apply(lambda x: x % 10).astype(int)
    df['user_gender'] = df['user_gender'].apply(lambda x: x % 10).astype(int)

    writer = tf.python_io.TFRecordWriter(os.path.join(clean_data_path, file_name))
    for row in df.iterrows():
        example = tf.train.Example(features=tf.train.Features(feature={
            'uid': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['uid']])),
            'iid': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['iid']])),
            'score': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['score']])),
            'net_env': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['net_env']])),
            'position': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['position']])),
            'time': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['time']])),
            'tab': tf.train.Feature(bytes_list=tf.train.BytesList(value=[row[1]['tab'].encode('utf-8')])),
            'post_tid': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['post_tid']])),
            'post_type': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['post_type']])),
            'user_gender': tf.train.Feature(int64_list=tf.train.Int64List(value=[row[1]['user_gender']])),
            'device_channel': tf.train.Feature(
                bytes_list=tf.train.BytesList(value=[row[1]['device_channel'].encode('utf-8')]))
        }))
        writer.write(example.SerializeToString())

    writer.close()
    print("success clean file %s!" % file_name)
```
处理过程非常缓慢，建议多进程处理。

```python
def main():
    # 进程数量
    proc_poll = multiprocessing.Pool(10) 
    num = 0
    for f_name in os.listdir(raw_data_path)[:10]:
        num = num + 1
        proc_poll.apply_async(clean_data, args=(raw_data_path, clean_data_path, f_name,
                                                use_cols, col_names, na_values))

    proc_poll.close()
    proc_poll.join()
```
实测单进程处理速度不超过3K/s，基本无法忍受

下面是使用高阶API的dataset读取数据，dataset自动使用pipeline模式，实测应该是单线程读取数据，然后可以多线程处理数据，对比原先的多线程填充队列模式也没有明显的提升。

```python
# coding=UTF-8

import tensorflow as tf

USECOLS = [0, 1, 2, 6, 11, 12, 13, 14, 16, 18, 20]
COLUMN_NAMES = ["uid", "iid", "score", "net_env", "position", "time", "tab", "post_tid", "post_type", "user_gender",
                "device_channel"]
_RECODE_DEFAULT = [[0], [0], [0], [0], [0], [0], ["null"], [0], [0], [0], ["null"]]
LABEL_COLUMN = "score"


def parse_tf(example_proto):
    # 定义解析的字典
    dics = {}
    dics['uid'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['iid'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['score'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['net_env'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['position'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['time'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['tab'] = tf.FixedLenFeature(shape=[], dtype=tf.string)
    dics['post_tid'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['post_type'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['user_gender'] = tf.FixedLenFeature(shape=[], dtype=tf.int64)
    dics['device_channel'] = tf.FixedLenFeature(shape=[], dtype=tf.string)
    # 调用接口解析一行样本
    parsed_example = tf.parse_single_example(serialized=example_proto, features=dics)
    label = parsed_example.pop('score')
    return parsed_example, label


def input_fn(data_file_names, num_epochs, shuffle, batch_size):
    # Extract lines from input files using the Dataset API.
    dataset = tf.data.TFRecordDataset(data_file_names)
    # 批量解析数据，速度有质的提升，400%吧
    dataset = dataset.apply(tf.contrib.data.map_and_batch(map_func=parse_tf,
                                                          batch_size=batch_size,
                                                          num_parallel_batches=32))

    # if shuffle:
    #     dataset = dataset.shuffle(buffer_size=batch_size * 10)
    #
    # dataset = dataset.map(parse_csv, num_parallel_calls=20)
    # dataset = dataset.apply(tf.contrib.data.map_and_batch(parse_csv, batch_size))

    # We call repeat after shuffling, rather than before, to prevent separate
    # epochs from blending together.
    # dataset = dataset.repeat(num_epochs)
    # dataset = dataset.batch(batch_size)
    dataset = dataset.prefetch(30)

    iterator = dataset.make_one_shot_iterator()
    features, labels = iterator.get_next()

    return features, labels
```