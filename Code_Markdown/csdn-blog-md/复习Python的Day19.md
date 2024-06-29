# 复习Python的第十九天

## 习题 20: 函数和文件 
先建立一个文件，名为“save.txt”。内容即为作者在代码中给出的那样：
```python2
To all the people out there. 
I say I don't like my hair. 
I need to shave it off. 
```
> The Codes
```python
from sys import argv 
# 命令行参数解包 
script, input_file = argv 
 
def print_all(f): 
    print f.read() 
 
def rewind(f): 
    f.seek(0) # 将文件指针移至文件开始处
 
def print_a_line(line_count, f): 
    print line_count, f.readline() 
# 打开目标文件 
current_file = open(input_file) 
 
print "First let's print the whole file:\n" 
# 打印全部文件内容 
print_all(current_file) 

# 让我们倒回文件开始，像打字机一样从头开始
print "Now let's rewind, kind of like a tape." 
rewind(current_file) 
 
print "Let's print three lines:" 
# 打印第一行 
current_line = 1 
print_a_line(current_line, current_file) 

# 打印第二行 
current_line = current_line + 1 
print_a_line(current_line, current_file) 

# 打印第三行
current_line = current_line + 1 
print_a_line(current_line, current_file) 
```

## 本节知识
### 各种文件相关的命令（方法/函数）
- `close` – 关闭文件
- `read` – 读取文件内容。你可以把结果赋给一个变量
- `readline` – 读取文本文件中的一行
- `truncate` – 清空文件内容
- `write(stuff)` – 将stuff写入文件
- `seek()`  – 移动文件指针至某一字节处

让我们试试最后一个方法，在交互式模式下运行：
```python2
>>> file = open('save.txt')
>>> file
<_io.TextIOWrapper name='save.txt' mode='r' encoding='cp936'>
>>> print file.readline()  # 文件指针移至第一行末尾
To all the people out there.

>>> print file.readline() # 文件指针移至第二行末尾
I say I don't like my hair.

>>> print file.readline()  # 文件指针移至文件末尾
I need to shave it off.

>>> print file.readline() # 返回空字符串
''
>>> file.seek(0) # 将文件指针移至文件开始处
0
>>> print(file.readline()) # 又打印出第一行
To all the people out there.
```

让我们看一看关于返回的文件对象的文档：
```python
 |  seek(self, cookie, whence=0, /)
 |      Change stream position.
 |
 |      Change the stream position to the given byte offset. The offset is
 |      interpreted relative to the position indicated by whence.  Values
 |      for whence are:
 |
 |      * 0 -- start of stream (the default); offset should be zero or positive
 |      * 1 -- current stream position; offset may be negative
 |      * 2 -- end of stream; offset is usually negative
 |
 |      Return the new absolute position.
```
`whence`：可选，默认值为 0。给offset参数一个定义，表示要从哪个位置开始偏移；0 代表从文件开头开始算起，1 代表从当前位置开始算起，2 代表从文件末尾算起。

该函数会**返回新的绝对位置**。

本节内容就到这里。