---
layout: post
title:  "tensorflow之部分API"
categories: tensorflow
tags:  API 
author: canisn
---

* content
{:toc}





### 1. 变量
tf.get_variable()

使用get_variable()的好处在于变量共享，因此在一个命名空间里一个名字只能申请一次，重新赋值给其他变量名需要显式声明变量重用。不申明会报错。

```python
with tf.variable_scope('layer1', reuse = tf.AUTO_REUSE):
    w = tf.get_variable("w", shape=[2,3], initializer=tf.truncated_normal_initializer(stddev=0.01))
    w1 = tf.get_variable("w")

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
-----
[[ 0.01414223  0.00312555 -0.00456955]
 [-0.00422565  0.00015657  0.00511333]]
[[ 0.01414223  0.00312555 -0.00456955]
 [-0.00422565  0.00015657  0.00511333]]
```
在建图之前还可以添加tf.reset_default_graph()来确保内存中没有存在其他节点。