---
layout: post
title:  "关于TensorFlow 高阶API的爬坑指南!"
categories: tensorflow 
tags:  tensorflow estimator 分布式 
author: canisn
---

* content
{:toc}

关于TensorFlow 高阶API的爬坑指南

​	本文使用TensorFlow的高阶API进行试验。

​	第一次试验采用dataset+pre_made estimator+.train()+.evaluation()过程，完成了模型的试验，导出和线上部署测试，具体方法可见（这是一个伪链接）。此过程尝试分布式部署失败，转战方法二。

​	第二次试验采用dataset+pre_made estimator+train_and_evaluate，一下是二次爬坑指东西南。





## 1. 分布式部署时环境

​	分布式部署基础环境要求：有一个可以给集群共享的文件夹，用于存取共同的check_point，要求每个节点访问，首选NFS,实现简单好用不费劲，但是不符合我司风格吧，投入现成HDFS研究，首先就是配置Python对HDFS系统的访问。

​	在使用HDFS作为存储方案时，包括训练数据的读取，模型保存等，都可以在HDFS上直接操作。配置上要求紧跟官方配置要求，一部都不能少。https://www.tensorflow.org/deploy/distributed 。

```shell
16 export JAVA_HOME=/usr/lib/jvm/jre-1.8.0-openjdk-1.8.0.91-0.b14.el7_2.x86_64
 17 export HADOOP_HOME=/data/hadoop-2.6.0
 18 export HADOOP_HDFS_HOME=/data/hadoop-2.6.0
 19 export CLASSPATH=$(${HADOOP_HDFS_HOME}/bin/hadoop classpath --glob)
 20 export LD_LIBRARY_PATH=$HADOOP_HOME/lib/native:${JAVA_HOME}/lib/amd64/server
```

一步都不能少，比如在LD_LIBRARY_PATH要加入JAVA中的libjvm.io的路径。

在Hadoop版本低于（2.6.0）时CLASSPATH是 必要的，写上准没错。

完成环境配置后直接在相应的路径上写HDFS远程路径即可，用法与本地相当。

```shell
hdfs://namenode:8020/path/to/file1.csv
```



## 2. 实验过程记录

1. 单机模式下运行，读取训练数据在HDFS上，当配置check_point路径在HDFS上，训练完成后验证时无法恢复从远程上恢复训练参数，报错

   ```shell
   Restoring parameters from hdfs://emr-header-1:9000/user/canisn/check_point/model.ckpt-1000
   Traceback (most recent call last):
     File "/data/canisn/py3venv/lib/python3.6/site-packages/tensorflow/python/client/session.py", line 1322, in _do_call
       return fn(*args)
     File "/data/canisn/py3venv/lib/python3.6/site-packages/tensorflow/python/client/session.py", line 1307, in _run_fn
       options, feed_dict, fetch_list, target_list, run_metadata)
     File "/data/canisn/py3venv/lib/python3.6/site-packages/tensorflow/python/client/session.py", line 1409, in _call_tf_sessionrun
       run_metadata)
   tensorflow.python.framework.errors_impl.InvalidArgumentError: hdfs://emr-header-1:9000/user/canisn/check_point/model.ckpt-1000.data-00000-of-00001; Invalid argument
   	 [[Node: save/RestoreV2 = RestoreV2[dtypes=[DT_FLOAT, DT_FLOAT, DT_FLOAT, DT_FLOAT, DT_FLOAT, ..., DT_FLOAT, DT_FLOAT, DT_FLOAT, DT_FLOAT, DT_FLOAT], _device="/job:localhost/replica:0/task:0/device:CPU:0"](_arg_save/Const_0_0, save/RestoreV2/tensor_names, save/RestoreV2/shape_and_slices)]]
   ```

   但是当配置check_point路径在本机地址是，没有问题。

   bug在此，https://github.com/tensorflow/tensorflow/issues/18769，模型太大了,验证了一下wide模型，没有问题。

   ![image-20180715015622691](/var/folders/zf/mxwhhrl95cl3pkncv9xykqqc0000gn/T/abnerworks.Typora/image-20180715015622691.png)

   看样子需要给模型分块了，这不坑人么！垃圾tf!

   实验三：发现模型巨大的原因是embedding部分，将10M长度的向量embendding到64，参数为float类型，8B，总大小为5.12G，也就是说10M向量embedding后长度每增加一维，模型增大80M。于是，我把现有的模型修改成20M->16, 10M->8,最后模型总大小3.6+G，略小于允许的单个最大4G，在作死的边缘试探。(这个问题暂时结束)

2. 单机模式问题二：

   已经配置了num_epoch为1，不设置max_step，预期是训练完成一轮数据后程序应该保存检查点，验证，导出模型。

   但是现在是保存检查点后恢复模型验证，然后又开始载入模型重新训练，一直循环。那啥时候导出模型？

   1）实验测试一：设置max_step，完成最大训练步数后可以自动导出模型。不能采用原有的轮一遍数据后自动结束？这是一个bug！（还需要深究）

3. 分布式集群问题：

   在单机模式问题二的基础上，配置系统的【TF_CONFIG】环境变量，组建一个三机分布式集群，配置为：

   ```shell
       # set node type
       os.environ['TF_CONFIG'] = json.dumps({
           "cluster": {
               "ps": ["172.16.0.167:2220"],
               "chief": ["172.16.1.5:2220"],
               "worker": ["172.16.1.22:2220"],
           },
           "task": {
               "index": 0,   
               "type": hparams.type 
           }
       })
   ```

   计算节点在完成各自的计算任务后就正常结束退出了，其中计算主节点chief在退出前会保存一下check_point。于是就有问题了，在chief较快完成自己的任务后退出了，其他worker还在计算，那么它们算完的数据没人保存，丢弃了？

   工作节点后正常退出了，验证环节怎么没了？同样的，啥时候导出模型？

   1） 实验二，在集群模式下设置了最大迭代步数，结果是完成迭代步数后依然在保存检查点之后就臭不要脸的退出了！不eval不export算什么本事！

4. 分布式模式下，数据的分发问题，如何使每个节点可以**按需**取到数据集的不重复子集，一个是数据不需要重复轮，一个是保证每个节点的工作时间应该一致，而不是工作量一致。

   ​	找到	tf原生的分布式数据分发解决方案，每个节点在生成pipeline的时候先根据worker数量和自己的index平均切分数据并领取自己的一份。平均！！！数据平均分！！！什么傻逼玩意儿！

   ```python
   dataset = tf.data.TextLineDataset(data_file_names)
   dataset = dataset.shard(num_workers, worker_index)
   ```

   实测一下，速度无法直视，不管是不是我的打开方式不对，都。。。。去吧，皮卡丘~~~

5. 实验中发现新的问题，训练速度会在100多次迭代后突然下降到一个很低的水平并保持。查资料说可能是因为在运行过程中图节点不断增加导致，但是这个与突然下降不符合，且我的数据采用pipeline导出，不应该会重新创建节点，又是一个谜。

   一个猜测，可能是在建图的时候数据缓存发力了，一段时间后存货用光了，就有卡IO了。

6. 经过几天的努力爬坑，终于还是没有爬出来，再见。滚回去单机了。

## 3. 出现的一系列其他问题：

1. 程序被killed，原因是机器内存不足，导致python进程直接被杀死。

2. 工作节点报错：

   ```shell
   E tensorflow/core/distributed_runtime/master.cc:269] Master init: Unavailable: OS Error
   INFO:tensorflow:An error was raised while a session was being created. This may be due to a preemption of a connected worker or parameter server. A new session will be created. Error: OS Error
   INFO:tensorflow:Graph was finalized.
   ```

   原因是在集群注册的时候已经注册了有多少个节点，此时启动节点进程要求每个节点直接进行通信，如果有进程没有启动，则会出现连接失败（猜测原因，需要进一步确认）。另外要注意进程的端口是否可用，是否被占用。

3. 在训练，验证等环节模型需要一直重建在导入参数，影响性能，G家开发者坚定认为这个对性能影响不大，他们主要是考虑分布式情况下，节点在工作状态转变的时候不应该影响到其他节点啥的，至于只有单机的孩子，滚回去tf.keral.model吧。https://github.com/tensorflow/tensorflow/issues/13895

   ![image-20180715014129365](/var/folders/zf/mxwhhrl95cl3pkncv9xykqqc0000gn/T/abnerworks.Typora/image-20180715014129365.png)

4. 关于模型服务时的输入数据类型，可以自行定义，csv,TFRecord，以及example类型都是可行的方案。

   ```python
   def csv_serving_input_fn():
       """Build the serving inputs."""
       csv_row = tf.placeholder(
           shape=[None],
           dtype=tf.string
       )
       features, _ = parse_csv(csv_row)
       # features
       return tf.estimator.export.ServingInputReceiver(features, {'csv_row': csv_row})
   def parse_csv(value):
       columns = tf.decode_csv(value, record_defaults=_RECODE_DEFAULT)
       feature = dict(zip(COLUMN_NAMES, columns))
       label = feature.pop(LABEL_COLUMN)
       return feature, label
   
   def example_serving_input_fn():
       """Build the serving inputs."""
       example_bytestring = tf.placeholder(
           shape=[None],
           dtype=tf.string,
       )
       feature_scalars = tf.parse_example(
           example_bytestring,
           tf.feature_column.make_parse_example_spec(INPUT_COLUMNS)
       )
       return tf.estimator.export.ServingInputReceiver(
           feature_scalars,
           {'example_proto': example_bytestring}
       )
   
   def json_serving_input_fn():
       """Build the serving inputs."""
       inputs = {}
       for feat in INPUT_COLUMNS:
           inputs[feat.name] = tf.placeholder(shape=[None], dtype=feat.dtype)
       return tf.estimator.export.ServingInputReceiver(inputs, inputs)1.4版本推出了高阶分布式版本，于是我换个版本测试一下吧：
   ```

   . TensorFlow==1.4.0	

   

   ```shell
   # 训练速度很慢。
   RuntimeWarning: compiletime version 3.5 of module 'tensorflow.python.framework.fast_tensor_util' does not match runtime version 3.6
   ```

   ​	多机时num_epoch有效，正常退出，无eval无export

   ​	单机num_epoch无效，达到max_step正常退出，有eval有export

   TensorFlow==1.7.0

   ​	多机时num_epoch有效，正常退出，无eval无export

   ​	单机num_epoch无效，达到max_step正常退出，有eval有export

   TensorFlow==1.8.0

   ​	多机时num_epoch有效，正常退出，无eval无export

   ​	单机num_epoch无效，达到max_step正常退出，有eval有export

   结论：跟版本没关系






