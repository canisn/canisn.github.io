import re
import jieba
import codecs
from gensim.models.word2vec import Word2Vec
import pandas as pd


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


model_name = 'sk_300features_10window_avg_half'
model = Word2Vec.load(model_name)

def text_to_words(text):
    words = list(text_parse(text).split(' '))
    return words

def words_to_array(words):
    array = np.array([model[w] for w in words if w in model])
    return array

def embedding_to_avg(array):
    avg = array.mean(axis=0)
    return avg


# 抽取出文本与tid
write_file = open("./contents.txt", 'w')
pattern = re.compile(r'^(\d+),.*,\d+,(.*)\n$')
with open("postmd.txt", encoding='utf-8') as f:
    for line in f.readlines():
        r = pattern.match(line)
        if r:
#             print(r.group(1)+ "dasda" + r.group(2))
            write_file.write(r.group(1)+ "\t" + r.group(2) + '\n')
write_file.close()
    

train = pd.read_table('content.txt', sep='\t', names=('tid', 'text'))
# 切分并清理无效词
words = train['text'].map(text_to_words)
# 词编码
embeddings = words.map(words_to_array)
# 取编码均值
train['text'] = embeddings.map(embedding_to_avg)
# 写入文件
train.to_csv('avg_result.csv', sep='\t', index=False)
# # 切分并清理无效词
# train['words'] = train['text'].map(text_to_words)
# # 词编码
# train['embeding'] = train['words'].map(words_to_array)
# # # 取编码均值
# # train['avg'] = train['embeding'].map(embedding_to_avg)
# # 写入文件
# train.to_csv('full_result.csv', sep='\t', index=False)



