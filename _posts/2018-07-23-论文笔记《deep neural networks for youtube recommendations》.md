---
layout: post
title:  "《deep&wide》论文!"
categories: 推荐系统
tags:  deep&wide 
author: canisn
---

* content
{:toc}







# 1. 介绍

- **规模大**：用户和视频的数量都很大，只能适应小规模数据集的算法就不考虑了。
- **更新快**：youtube视频更新频率很高，每秒有小时级别的视频上传，需要在新发布视频和已有存量视频间进行balance。更新快（实时性）的另一方面的体现是用户实时行为切换很快，模型需要很好的追踪用户的实时行为。
- **噪音**：噪音主要体现在用户的历史行为往往是稀疏的并且是不完整的，并且没有一个明确的ground truth的满意度signal，我们面对的都是noisy implicit feedback signals。噪音另一个方面就是视频本身很多数据都是非结构化的。这两点对算法的鲁棒性提出了很高的挑战。

# 2. matching

## 2.1 问题建模

​	把推荐问题建模成一个“超大规模多分类”问题。即在时刻![t](https://www.zhihu.com/equation?tex=t)，为用户![U](https://www.zhihu.com/equation?tex=U)（上下文信息![C](https://www.zhihu.com/equation?tex=C)）在视频库![V](https://www.zhihu.com/equation?tex=V)中精准的预测出视频![i](https://www.zhihu.com/equation?tex=i)的类别（每个具体的视频视为一个类别，![i](https://www.zhihu.com/equation?tex=i)即为一个类别），用数学公式表达如下：

![img](https://pic4.zhimg.com/80/v2-adde74b978b971e588c002038c390e0b_hd.jpg)

上式为一个softmax多分类器的形式。向量![u\in R^N](https://www.zhihu.com/equation?tex=u%5Cin+R%5EN)是<user, context>信息的高纬“embedding”，而向量![v_{j}\in R^N](https://www.zhihu.com/equation?tex=v_%7Bj%7D%5Cin+R%5EN)则是视频 j 的embedding向量。所以DNN的目标就是在用户信息和上下文信息为输入条件下学习用户的embedding向量![u](https://www.zhihu.com/equation?tex=u)。用公式表达DNN就是在拟合函数![u = f_{DNN}(user_info, context_info)](https://www.zhihu.com/equation?tex=u+%3D+f_%7BDNN%7D%28user_info%2C+context_info%29)。

## 2.2 模型架构

![img](https://pic1.zhimg.com/80/v2-7f97ddd40285e08b64546e3a54a5d64a_hd.jpg)

​	整个模型架构是包含三个隐层的DNN结构。输入是用户浏览历史、搜索历史、人口统计学信息和其余上下文信息concat成的输入向量；输出分线上和离线训练两个部分。

​	离线训练阶段输出层为softmax层，输出2.1公式表达的概率。

​	线上则直接利用user向量查询相关商品，最重要问题是在性能。我们利用类似局部敏感哈希的算法为用户提供最相关的N个视频。

## 2.3 特征工程

- **历史搜索query**：把历史搜索的query分词后的token的embedding向量进行加权平均，能够反映用户的整体搜索历史状态
- **人口统计学信息**：性别、年龄、地域等
- **其他上下文信息**：设备、登录状态等
- example age:推荐系统产生的视频集合在视频的分布，基本上反映的是训练所取时间段的平均的观看喜好的视频。因此我们我们把样本的 “age” 作为一个feature加入模型训练中。从下图可以很清楚的看出，加入“example age” feature后和经验分布更为match。

![img](https://pic2.zhimg.com/80/v2-83f523875baab074b340b6ccea2eba02_hd.jpg)

## 2.4 label

- **使用更广的数据源**：不仅仅使用推荐场景的数据进行训练，其他场景比如搜索等的数据也要用到，这样也能为推荐场景提供一些explore。
- **为每个用户生成固定数量训练样本**：我们在实际中发现的一个practical lessons，如果为每个用户固定样本数量上限，平等的对待每个用户，避免loss被少数active用户domanate，能明显提升线上效果。
- **抛弃序列信息**：我们在实现时尝试的是去掉序列信息，对过去观看视频/历史搜索query的embedding向量进行加权平均。这点其实违反直觉，可能原因是模型对负反馈没有很好的建模。
- **不对称的共同浏览（asymmetric co-watch）问题**：所谓asymmetric co-watch值的是用户在浏览视频时候，往往都是序列式的，开始看一些比较流行的，逐渐找到细分的视频。下图所示图(a)是hled-out方式，利用**上下文信息**预估中间的一个视频；图(b)是predicting next watch的方式，则是利用**上文信息**，预估下一次浏览的视频。我们发现图(b)的方式在线上A/B test中表现更佳。*而实际上，传统的协同过滤类的算法，都是隐含的采用图(a)的held-out方式，忽略了不对称的浏览模式。*

![img](https://pic4.zhimg.com/80/v2-4c34494e753fa7ad6525f3533db31147_hd.jpg)



# 3. Ranking

​	Ranking阶段的最重要任务就是精准的预估用户对视频的喜好程度。不同于Matching阶段面临的是百万级的候选视频集，Ranking阶段面对的只是百级别的商品集，因此我们可以*使用更多更精细的feature*来刻画视频（item）以及用户与视频（user-item）的关系。

## 3.1 模型架构

Ranking阶段的模型和Matching基本相似，不同的是training最后一层是一个weighted LR层，而serving阶段激励函数用的是![e^{x} ](https://www.zhihu.com/equation?tex=e%5E%7Bx%7D+)

![img](https://pic4.zhimg.com/80/v2-33f93002d2d7f42f50e617e03ef659bd_hd.jpg)

## 3.2 特征工程

### 3.2.1 特征选取

**最重要的Signal是描述用户与商品本身或相似商品之间交互的Signal**

- **数量特征**：浏览该频道的次数？
- **时间特征**：比如最近一次浏览该频道距离现在的时间？

这两个连续特征的最大好处是具备非常强的泛化能力。另外除了这两个偏正向的特征，用户对于视频所在频道的一些PV但不点击的行为，即**负反馈Signal同样非常重要**。

### 3.2.2 特征组合

​	NN更适合处理连续特征，因此稀疏的特别是高基数空间的离散特征需要embedding到稠密的向量中。每个维度（比如query/user_id）都有独立的embedding空间，一般来说空间的维度基本与log(去重后值得数量)相当。实际并非为所有的id进行embedding，比如视频id，只需要按照点击排序，选择top N视频进行embedding，其余置为0向量。而对于像“过去点击的视频”这种multivalent特征，与Matching阶段的处理相同，进行加权平均即可。

​	另外一个值得注意的是，同维度不同feature采用的相同ID的embedding是共享的（比如“过去浏览的视频id” “seed视频id”），这样可以大大加速训练，但显然输入层仍要分别填充。

### 3.2.3 特征归一化

​	众所周知，NN对输入特征的尺度和分布都是非常敏感的，实际上基本上除了Tree-Based的模型（比如GBDT/RF），机器学习的大多算法都如此。我们发现归一化方法对收敛很关键，推荐一种排序分位归一到[0,1]区间的方法，即![\bar{x}=\int_{-\infty }^{x}df  ](https://www.zhihu.com/equation?tex=%5Cbar%7Bx%7D%3D%5Cint_%7B-%5Cinfty+%7D%5E%7Bx%7Ddf++)，累计分位点。

​	除此之外，我们还把归一化后的![\bar{x} ](https://www.zhihu.com/equation?tex=%5Cbar%7Bx%7D+)的根号![\sqrt{x} ](https://www.zhihu.com/equation?tex=%5Csqrt%7Bx%7D+)和平方![x^{2} ](https://www.zhihu.com/equation?tex=x%5E%7B2%7D+)作为网络输入，以期能使网络能够更容易得到特征的次线性（sub-linear）和（super-linear）超线性函数。

### 3.3 预测，希望观看时长

目标是预测**期望观看时长**。有点击的为正样本，有PV无点击的为负样本，正样本需要根据观看时长进行加权。因此，我们训练阶段网络最后一层用的是 weighted logistic regression。

正样本的权重为观看时长 ![T_{i}](https://www.zhihu.com/equation?tex=T_%7Bi%7D)，负样本权重为1。这样的话，LR学到的odds为：

![img](https://pic1.zhimg.com/80/v2-4b76c3502d11f87785fb32712ec4b623_hd.jpg)

其中N是样本总数，k是正样本数。$t_i是正样本的观看时长。一般来说，k相对N比较小，在线上serving的inference阶段，我们采用![e^{x} ](https://www.zhihu.com/equation?tex=e%5E%7Bx%7D+)作为激励函数，就是近似的估计期望的观看时长。

