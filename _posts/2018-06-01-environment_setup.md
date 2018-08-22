---
layout: post
title:  "开发环境配置"
categories: linux
tags: virtual python
author: canisn
---

* content
{:toc}




# 1. 基础环境安装

安装Python3：

```shell
apt-get install python36
# centos
yum install python36
```

安装pip3:

```shell
curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py | python3
```

安装virtualenv:

```shell
# 注意这里的pip一定是pip3的
(py3venv) [work@sh-rec-offline03 train_model]$ pip -V
pip 18.0 from /data/canisn/py3venv/lib/python3.6/site-packages/pip (python 3.6)
# 这样是为了保证安装的virtualenv是属于python3的
pip install virtualenv
```

安装基于Python3的虚拟环境：

```shell
virtualenv -p python3 py3vnev
```

# 2. 安装依赖包

可以手动pip安装，或者一次性自动安装。

## 手动安装

```shell
pip install ***
```

## 一次性安装

依赖包文件可以是手动添加的

tensorflow==1.8.0

pandas==0.23.4

sklearn==0.19.2

scipy==1.1.0

numba=0.39.0

influxdb==5.2.0

还可以在用来的虚拟环境下直接导出全部第三方包：

```shell
pip freeze > requirements.txt
```

如果原来的环境是跟别的项目混用，那就会导出一些不需要的包，高端方法是使用工具扫描工程文件，导出依赖包。

```shell
pip install pipreqs
cd /path/to/proj/root
# 自动生成requirements.txt文件
pipreqs ./
# 这种方法可能会有偏差，实际调整就好
```

自动导出：

```shell
# 把全部依赖包写入requirements文件
pip install -r requirements
```

## 3. 在服务器上安装**Jupyter**

```shell
pip install jupyter
# 生成配置文件
jupyter notebook --generate-config
```

```shell
# 在python环境下生成密码
(py3venv) [work@sh-rec-offline03 canisn]$ python
Python 3.6.2 (default, Mar 28 2018, 22:18:10)
[GCC 4.8.5 20150623 (Red Hat 4.8.5-16)] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> from notebook.auth import passwd
>>> passwd()
Enter password:
Verify password:
'sha1:d00ba59840c6:f42d2bbaca83f2045b4c8b0b082574cd77413f3a'
```

```shell
# 打开配置文件
vi /home/work/.jupyter/jupyter_notebook_config.py
# 编辑以下内容
c.NotebookApp.port = 8888
c.NotebookApp.notebook_dir = "/data/canisn"
# 允许登陆的IP，可以是IP段
c.NotebookApp.ip='*'
c.NotebookApp.open_browser = False
# 刚才生成的密文
c.NotebookApp.password = u'sha1:d00ba59840c6:f42d2bbaca83f2045b4c8b0b082574cd77413f3a'
```

之后就可以在浏览器中打开了，注意密码是明文，就是刚才生成密文时输入的。