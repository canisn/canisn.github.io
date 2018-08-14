import re

# 取出文本和tid
write_file = open("./content.txt", 'w')
pattern = re.compile(r'^(\d+),.*,\d+,(.*)\n$')
with open("test.txt", encoding='utf-8') as f:
    for line in f.readlines():
        r = pattern.match(line)
        if r:
#             print(r.group(1)+ "dasda" + r.group(2))
            write_file.write(r.group(1)+ "\t" + r.group(2) + '\n')
write_file.close()


# 正则剔除文本的非法字符
CLEAN_TAB = re.compile(u'[\t\r\n]')
CLEAN_OUTRANGE = re.compile(r'[\U00010000-\U0010ffff]')
CLEAN_CHAR = re.compile(u'[：，。、！…|!“”?？\',+【】~/\s]')
CLEAN_HAHA = re.compile(u'[\u54c8]{2,}')

# jieba load 字典，停用词
jieba.load_userdict("jieba_dict_v1.txt")  # 加载自定义词典
stoplist = {}.fromkeys([line.strip() for line in codecs.open("stopwords_avg_half_v1.txt", encoding='utf-8')])


def text_parse(sentence):
    """
    清理文本，分词，去停止词
    """
    sentence = sentence.replace('\t', ' ').replace('\r', ' ')
    sentence = re.sub(CLEAN_OUTRANGE, ' ',sentence)
    sentence = re.sub(CLEAN_CHAR,'',sentence)
    sentence = re.sub(CLEAN_HAHA, '', sentence)
    cut_list = jieba.lcut(sentence)
    cut_lists = ' '.join([k for k in cut_list if k and k != u'\u200b' and k != "\t" and k != ' '])
    seg_list = [word for word in cut_list if word not in stoplist]
    seg_list = ' '.join([k for k in seg_list if k and k != u'\u200b' and k != "\t" and k != ' '])
    return seg_list



def get_seglist(post_dict):
    """
    将分词结果集合
    """
    print '3'
    seg_dict = {}
    for pid in post_dict:
        seg_dict[pid] = text_parse(' '.join(post_dict[pid]['content']))
        if not seg_dict[pid]:
            continue
    return seg_dict


