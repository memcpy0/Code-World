# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-19 23:53
program     :
"""
# import pyperclip
# import re, string
# import time, datetime
# import json, csv
# import requests, urllib.request
# from lxml import etree
# from bs4 import BeautifulSoup
# import os, glob, sys, subprocess, threading
# from multiprocessing.dummy import Pool
# from PIL import Image
# import PyPDF2, reportlab, csv, json
# import smtplip, imapclient, pyzmail
# from collections import *
"""
Python Scikit-learn库
依赖于Numpy和Scipy、Matplotlib库

sklearn库功能主要分为六类：
1 分类
* 最近邻算法 neighbors.NearestNeighbors
* 支持向量机 svm.SVC
* 朴素贝叶斯 naive_bayes.GaussianNB
* 决策树     tree.DecisionTreeClassifier
* 集成方法   ensemble.BaggingClassifier
* 神经网络   neural_network.MLPClassifier

2 回归    
* 岭回归     linear_model.Ridge
* Lasso回归  linear_model.Lasso
* 弹性网络    linear_model.ElasticNet
* 最小角回归  linear_model.Lars
* 贝叶斯回归  linear_model.BayesianRidge
* 逻辑回归    linear_model.LogisticRegression
* 多项式回归  linear_model.PolynomialFeatures

3 聚类
* K-means    cluster.KMeans
* AP聚类      cluster.AffinityPropagation
* 均值漂移    cluster.MeanShift
* 层次聚类    cluster.AgglomerativeClustering
* DBSCAN     cluster.DBSCAN
* BIRCH      cluster.Birch
* 谱聚类      cluster.SpectralClustering

4 降维
* 主成分分析    decomposition.PCA
* 截断SVD和LSA  decomposition.TruncatedSVD
* 字典学习      decomposition.SparseCoder
* 因子分析      decomposition.FactorAnalysis
* 独立成分分析   decomposition.FastICA
* 非负矩阵分解   decomposition.NMF
* LDA          decomposition.LatentDirichletAllocation

5 模型选择
6 数据预处理
"""
from sklearn import datasets

# boston 回归任务 房价 NOX 二氧化氮
boston = datasets.load_boston()  # Bunch Dictionary-like object返回全部信息
# print(boston.DESCR)         # (506, 13)

# data, target = datasets.load_boston(return_X_y=True)  # 只返回data和target的数据(data, target)
# print(data.shape)   # (506, 13)
# print(target.shape) # (506,)


# 鸢尾花数据集 鸢尾花的测量数据 测试多分类器
# iris = datasets.load_iris()
# print(iris.data.shape)    # (150, 4)
# print(iris.target.shape)  # (150,)
# print(list(iris.target_names))  # ['setosa', 'versicolor', 'virginica']
# print(iris.DESCR)               # Iris Plants Database

# 手写数字数据集
digits = datasets.load_digits()
# print(digits.DESCR)

import matplotlib.pyplot as plt
plt.matshow(digits.images[0])
plt.show()
