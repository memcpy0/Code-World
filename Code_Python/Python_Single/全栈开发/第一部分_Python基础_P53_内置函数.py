# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-09-14 20:49
program     : 内置函数68个
"""
"""
基础数据类型相关 38  作用域相关 2
反射相关 4  面向对象相关 9  迭代器/生成器相关 3  其他 12
--------------------------------------------------------------------
*作用域相关(2)  globals() 全局作用域的名字 字典
               locals()  当前作用域的名字 字典

*迭代器/生成器相关(3)  range() iter() next()

*其他(12)
  输入输出(2)    print(*args, sep=' ', end='\n', file=sys.stdout, flush=False)
                input()
  内存相关(2)    id(object)
                hash(object) 目的是唯一性；dict查找效率非常高；hash表用空间换时间
                    print(hash('123'))  # -3343635203645398949
  文件操作相关(1) open(文件名, 'r', encoding='')
  模块相关(1)     __import__()
  帮助相关(1)    help()
  查看内置属性(1) dir()
  调用相关(1)    callable(object) 判断是否可以被调用
                    print(callable(10))  # False
  字符串形式代码执行(3)
                 eval(s)  print(eval('1+3')) 可以动态地执行代码，要求代码要有返回值
                 exec(s)  执行代码没有返回值
                    exec('''
                    def func():
                       print("我是zz")
                    func()
                    ''')
                 compile(source, filename, mode)
                    mode: eval; exec; single(有交互)

*基础数据类型(38)
  数据类型(4)  bool(x) int(x, base=10)  float(x) complex(real[, imag])
  进制转换(3)  bin(number) oct(i) hex(i)
  数值运算(7)  abs() 绝对值
              divmod() 求商和余数
              round() 四舍五入
              pow() 求平方；给了第三个参数，表示最后取余
              sum() 求和
              min() 最小值 
              max() 最大值
  数据结构相关(24)  
    序列(13) 
        列表元祖(2) list tuple
        相关内置函数(2) reversed(iterable) -> iterator 代码通用性更强
                      slice(start, end, stride) -> iterator slice对象，切片用的
        字符串(9)   repr      原样输出，字符串中的转义\n \t \r不起作用，不管%s占位符
                   format    与具体数据相关，与字符串格式化语法有关
                   str(str(bytes_or_buffer[, encoding[, errors]])       
                   bytes(s, encoding='utf-8') -> bytes类型 等同于s.encode('utf-8')
                   bytearray(s, encoding='utf-8') -> bytearray 数组元素可变，每个值范围在0-255
                   ord('c')  返回字符在编码表中的位置(字母在ASCLL，中文在中文编码表)
                   chr(num)  已知码位，返回字符
                   ascii('c') 判断字符是否在ASCII码里，是的话返回其本身     
    数据集合(3) 
        字典(1)     dict()
        集合(2)     set() frozenset() 
    操作内置函数(8)  enumerate(iterable[, start]) 把索引和元素一起获取
                   all() 可迭代对象中所有元素均为True，返回True
                   any() 可迭代对象中所有元素一个为True，返回True
                   zip() 将各个可迭代对象组合成迭代器，其中每个元素为元组   
                   lambda sorted filter map               

*面向对象相关(9) 
    实例与类关系判断(判断对象所属类型):
        isinstance(object, type) 判断实例对象是否是某个类的实例或某个父类的子类的实例，包含继承关系，不管隔了多少层
        type: 判断实例对象是否是某个类的实例, 不包含继承关系，只管一层
            - type('s') is str
    类与类关系判断:
        issubclass(subclass, class) 判断类与类的继承关系，包含多层
"""
# import dis
# dis.dis(com)
# s = 'for i in range(10):print(i)'
# com = compile(s, '', 'exec')
# print(com)
# exec(com)
#
# s2 = '2+3'
# com2 = compile(s2, '', 'eval')
# print(eval(com2))
#
# s3 = "name=input('输入你的名字')"
# com3 = compile(s3, '', 'single')
# exec(s3)
# print(name)
#-------------------------------------------------------------------------------
# print("123", end=' ')
# print("\b\bss")
#-------------------------------------------------------------------------------
# print(ord('a'), ord('中')) # 97 20013
# print(chr(97), chr(20013)) # a 中
# for i in range(33333):
#     print(chr(i), end=' ')
# print(ascii('z'))  # 判断是否在ASCII码里，是的话返回其本身
#-------------------------------------------------------------------------------
# ret = bytearray('alex', encoding='utf-8')
# print(ret[0])    # 97
# ret[0] = 65
# print(str(ret))  # bytearray(b'Alex')
# s = memoryview("刚刚".encode('utf-8'))
# print(s)
#--------------------------------------------------------------------------------
# print(format(2221333, 'e'))
#--------------------------------------------------------------------------------

class A:
    pass

class B(A):
    pass

class C(B):
    pass

a = A()
b = B()
c = C()

print(isinstance(a, A))  # True
print(isinstance(b, B))  # True
print(isinstance(b, A))  # True

print(isinstance(c, C))  # True
print(isinstance(c, B))  # True
print(isinstance(c, A))  # True

print(issubclass(C, B))  # True
print(issubclass(C, A))  # True


