# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-24 18:02
program     : 学习正则表达式的语法
"""
"""
正则表达式语法：
1. 元字符
   * 字符组：
      - []
         # [0-9] [A-Z] [a-z] 匹配字符组中的一个字符
         # 约束一个字符的位置上出现的内容, 必须是ASCLL字符，有范围的话必须从小到大
         # [0-9A-Za-z] 匹配一个(Ascll)字符
         # [0-9][A-Z][a-z] 匹配三个字符
      
      - [^]   匹配除了字符组中字符之外的任何一个字符(字符组中的任何一个字符都不能出现)
      - ()
             
   * 元字符
      - \d  == [0-9]         也表示匹配一个字符，匹配的是一个数字
      - \w  == [0-9A-Za-z_]  也表示匹配一个字符，匹配的是一个字母或数字或下划线
      - \s  == [ \n\t]       表示匹配一个泛空白符(空格 制表符 回车换行)
         #  \n               匹配回车
         #  \t               匹配空格
         
      - \D                   匹配非数字
      - \W                   匹配非字母或数字或下划线
      - \S                   匹配非泛空白符
      
      - [\d\D] [\w\W] [\s\S] 匹配任意一个字符(包括字母数字下划线泛空白符和其他字符)
      - \b                   匹配一个单词的前后边界
      
      - ^                    从一个字符串的开始向后匹配起(匹配一个字符串的开头), 只能放在开头
      - $                    从一个字符串的结尾向前匹配起(匹配一个字符串的结尾), 只能放在结尾
        如^hello$, 只能匹配hello
    
      - |                   或, 匹配两者之一, 匹配到第一个就短路, 因此该把长的字符串放在前面
      
      - .                   匹配除了换行符之外的任意一个字符

2. 量词  **默认贪婪匹配, 能匹配多的就不匹配少的, 使用回溯算法
      - {}                  约束前面的一个字符出现的次数
        如 \d{3}   匹配数字字符3次
           \d{3,5} 匹配数字字符3-5次  表示范围 (能匹配5次就不匹配4次3次)
           \d{3,}  匹配数字字符3-无限次       (尽可能多的匹配)    
      - ?                   匹配0-1次(可有可无)(尽可能匹配一个)
      - +                   匹配1-任意次       (尽可能匹配多个)
      - *                   匹配0-任意次       (尽可能匹配多个)      

3. 转义符
   * Python中的转义符 
   * 正则中的字符:  \(  \)  \.  \|  \[  
              \]  \+  \*  \?  \/  \$  \-
            或者:  [()+*?/$.|]   
     减号如果出现在字符组的首位[-] 表示减号
            出现在字符组中间则表示范围  [0-9]
            如在中间表示减号，则需要   [1\-9] 

4. 特殊的用法和现象            
      - ?的使用            
        # 在量词的后面使用, 意为非贪婪匹配模式(惰性匹配)
        # 最常用 .*?x  匹配任意字符直到找到x
        
5. 总结
      - 元字符 
      - 元字符量词   默认贪婪匹配
      - 元字符量词?  表示惰性匹配 

# 1. 匹配任意长度的正整数 [1-9]\d+  
# 2. 匹配小数          -?\d+\.\d+?
# 4. 匹配负数          -\d+(\.\d+)?   
# 5. 匹配QQ号          [1-9]\d{4, 11}
# 6. 匹配长度为11为的电话号码  
# 7. 长度为8-10位的用户密码(包含数字字母下划线和?@) [\w?@]{8,10}
# 8. 匹配验证码，4位数字字母组成的  [\da-zA-Z]{4}

"""

import re
# re模块
"""
re 
# 字符串 
# 匹配   match
  findall  *****
- 参数, 返回值类型(列表), 返回值个数:1, 返回值内容:所有匹配上的项
  
  search   *****
- 返回匹配到的第一个结果, 为一个match对象, 使用group()方法取出它匹配到的结果
  如没有匹配的结果, 返回None
 
  match    ** 相当于对search的匹配pattern字符串前面加了个^
- 从字符串的头部匹配起，返回匹配到的第一个结果, 为一个match对象, 使用group()方法取出它匹配到的结果
  如没有匹配的结果, 返回None
  
"""
ret = re.findall('\d', '19039320alpha0239')
print(ret)

ret2 = re.search('\d+', '19039320alpha0239')
print(ret2)          # <_sre.SRE_Match object; span=(0, 8), match='19039320'>
print(ret2.group())  # 19039320

ret3 = re.search('\s+', '19039320alpha0239')
print(ret3)          # 返回值为None 没有匹配上就是None

ret4 = re.match('\d+', '#@19039320alpha0239')
print(ret4)          # None

"""
# 字符串
# 替换  replace
  sub  ***
"""
ret5 = re.sub('\d+', 'H', 'replace19039320alpha0239')
print(ret5)     # replaceHalphaH

ret6 = re.subn('\d+', 'H', 'replace19039320alpha0239')
print(ret6)     # ('replaceHalphaH', 2)

"""
# 字符串
# 切割  
  split  ***
"""
ret7 = re.split('\d+', 'replace19039320alpha0239')
print(ret7)     # ['replace', 'alpha', '']

# 进阶方法 - 爬虫、自动化开发
"""
  compile *****
# 编译 提高时间效率
"""
string = re.compile('-0\.\d+|-[1-9]+(\.\d+)?')
print(string)
ret8 = re.search(string, 'akfal203901lkaf-10!@43901')
print(ret8.group())   # -1

"""
  finditer *****
# 提高空间效率
"""
ret9 = re.finditer('\d+', '19039320alpha0239')
# print(ret9)   # 可调用的迭代器
for r in ret9:
    print(r.group())

"""
Python表达式中的分组
- 使用findall的话, 会优先显示分组中的内容, 但它不是不匹配; 要取消的话, 使用(?:正则表达式)
  分组优先: 假设想要匹配到的内容与不想要匹配的内容混合, 就可以用()来优先显示想要的内容, 不显示就得了
- split的特异分组显示: 遇到分组的话, 会保留split中被分开的内容

- 使用search和match的话不会发生这个情况, 如果search中有分组的话, 通过group(n)就能拿到对应的组
"""
rt1 = re.findall('www\.baidu\.com|www\.oldboy\.com', 'www.oldboy.com')
print(rt1)

rt2 = re.findall('www\.(?:baidu|oldboy)\.com', 'www.oldboy.com')
print(rt2)   # ['oldboy'] -> ['www.oldboy.com']

rt3 = re.split('(\d+)', 'alex99alpha89titan')
print(rt3)   # ['alex', 'alpha', 'titan'] -> ['alex', '99', 'alpha', '89', 'titan']

# 正则遇上分组
rt4 = re.search('\d+(.\d+)(.\d+)(.\d+)?', '1.2.3.4-2*(67+(-40.35/5)-(-4*3))')
print(rt4.group())   # 等同于 group(0)
print(rt4.group(1))
print(rt4.group(2))
print(rt4.group(3))
print(rt4.groups())  # ('.2', '.3', '.4')

# 分组命名的语法 (?P<name>正则表达式) 表示给分组取组名
# 使用这个分组   (?P=name), 这里匹配到的内容应该和分组中的内容完全相同
# 需要一个地方的内容和另外的内容一模一样的话可以使用
# 通过索引使用分组
# \1 表示使用第一组, 匹配到的内容必须和第一个组中的内容完全相同
ret = re.search('<(?P<name>\w+)>(?P<content>\w+)</(?P=name)>', '<h1>wang</h1>')
print(ret.group())           # <h1>wang</h1>
print(ret.group('name'))     # h1
print(ret.group('content'))  # wang