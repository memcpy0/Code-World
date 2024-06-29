# 复习Python的第二十五天
[TOC]
## 习题27：记住逻辑关系
先记着吧。其实在C语言中已经学过了。
作者之劝慰：
>一些重要的概念是必须记住的，一旦你明白了这些概念，你会获得相当的成就感，但是一开始你会觉得它们很难掌握，就跟和乌贼摔跤一样，而等到某一天，你会刷的一下豁然开 朗。你会从这些基础的记忆学习中得到丰厚的回报。

### 逻辑术语
|字符             |  含义|
|---------------------|-----------|
|and             |    与|
|or              |   或|
|not              |  非|
|!= (not equal)       | 不等于|
|== (equal)          | 等于|
|$>=(greater-than-equal) | 大于等于| 
|<= (less-than-equal)   |  小于等于|
|True              |  真|
|False             | 假|

### 真值表
|NOT  	|  True?   	  | OR 	 		 |   True?    	 |AND  |  True?        
|:---------|:------  |:---------		|:------    |:---------|:----------  
|not False |True    |True or False |  True   |True and False| False  
|not True | False   |True or True |  True    |True and True | True    
|       |        | False or True | True   |False and True |False
|       |        |False or False | False  |False and False |False    

|NOT OR |  True?      |NOT AND  |  True?
|:---------|:--------------   |:---------|:------------ 
|not (True or False) |  False  |not (True and False)| True 
|not (True or True)|  False  |not (True and True)  |False 
|not(False or True)  |  False |not (False and True) | True 
|not (False or False)| True  |not (False and False)| True 

|!=     |  True?     |==    |  True?   
|:---------|:------     |:---------|:------
|1 != 0   |  True      |1 == 0 |  False
|1 != 1  |  False       |1 == 1 |  True
|0 != 1  |  True        |0 == 1 |  False
|0 != 0  |  False       |0 == 0  |  True