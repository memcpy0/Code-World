学习这本书，直接从C++11开始，直接养成好的习惯。
@[toc]
## 第一章 编写一个简单的C++程序
这一章太简单了。但也有一些可以注意的地方：
### 1.1 编写一个简单的C++程序
不从helloworld开始，没有任何花里胡哨的东西：
```cpp
int main() {
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110200153656.png)
唯一的一条语句return，**结束函数的运行，同时会向调用者返回一个用于指示程序状态的值**，0表示成功。非零值由系统定义，用来指出错误类型。试试返回-1：
```cpp
int main() {
    return -1;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110200209232.png)4294967295是什么意思？它是计算机程序设计里面的一个值,表示无符号整数的十进制最大值。下面返回22：

```cpp
int main() {
    return 22;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110200322372.png)
```cpp
int main() {
    return -2;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110200633434.png)
看来调用者将返回的有符号整数转换成了无符号整数。

### 1.2 初始输入输出
下面是头文件的时间，iostream就是为我们准备的基础输入输出设施，它包括istream和ostream，表示输入流和输出流，从IO设备(通常是控制台)中顺序读出字节，或将字节顺序写入IO设备。４个IO对象，cin/cout/cerr/clog。**cerr(标准错误)输出警告和错误消息，clog输出程序运行时的一般性消息**：
```cpp
//test1-2
#include <iostream>
int main() {
	std::cout << "Enter two numbers:" << std::endl;
	int v1 = 0, v2 = 0;
	std::cin >> v1 >> v2;
	std::cout << "The sum of " << v1 << " and " << v2 << " is "
	          << v1 + v2 << std::endl;
	std::cerr << "你在错误的路上行走！" << std::endl;
	std::clog << "??一般性消息是什么?" << std::endl;
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111020193362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
① cout和cerr、clog看起来都可以输出，但联系和区别在哪里？
联系：C++Primer5说`当我们向cout/cerr/clog写入数据时，将会写到同一个窗口`。
区别：cout经过缓冲后输出，是标准输出，并且可以重新定向；cerr一般**用于迅速输出出错信息**，是标准错误，默认情况下被关联到标准输出流，**不经过缓冲而直接输出**，即错误消息可以直接发送到显示器，而无需等到缓冲区或者新的换行符时才被显示，**一般情况下无法被重定向**；clog流也是标准错误流，作用和cerr一样，区别在于cerr不经过缓冲区，直接向显示器输出信息，而**clog中的信息存放在缓冲区，缓冲区满或者遇到endl时才输出**。

是真是假试一下就知道了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110203153299.png)
然后在控制台输入`test1-2 < test.txt > testout.txt`，发现下面的情况，标准输出都重定向到了文件中，而cerr和clog都没有重定向。另外，缓冲不缓冲的这里测不出来。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110203353917.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191110203344759.png)
那么作用？为什么有cerr和clog？比如，程序遇到调用栈溢出的威胁（无限递归），这时到什么地方借内存存放你的错误信息？所以才有了cerr，其目的就是让你**在最需要它的紧急情况下还能得到输出功能的支持**。

② cin/cout等的运算顺序？或者说是<<和>>的运算顺序。这一点看似不重要，但是当你写运算符重载的时候就知道了。它们也是运算符，和+-*/=一样。**<<接受两个运算对象**，将右侧的运输对象的值写到左侧**给定**的ostream对象中，**返回结果就是其左侧被写入值的ostream对象**。因此第一个<<的结果可以成为第二个<<的左侧运算对象，我们才可以串起来写多个<<。输入>>和<<类似，也是返回其左侧运算对象(**istream对象**)作为结果。

③ std::endl，本以为只是换行符，但C++Primer里面还介绍说，`写入endl的效果是结束当前行，并将于设备关联的缓冲区中的内容刷到设备中。缓冲刷新操作可以保证到目前为止程序产生的所有输出都真正写入输出流中，而不是仅仅停留在内存中等待写入流`。调试时添加打印语句必须要保证流的持续刷新...差不多等效于cerr。

④ `::`作用域运算符，用于指定使用命名空间中的名字。

练习1.3：
```cpp
#include <iostream>
int main() {
	std::cout << "Hello, World" << std::endl;
	return 0;
}
```

### 1.3 注释简介
```cpp
#include <iostream>
/*
 * 简单主函数
 * 读取两个数，求他们的和 
 */
int main() {
	// 提示用户输入两个数 
	std::cout << "Enter two numbers:"<< std::endl;
	int v1 = 0, v2 = 0;
	std::cin>> v1 >> v2;
	std::cout << "The sum of " << v1 << " and " << v2 << " is " 
	          << v1 + v2 << std::endl;
	return 0;
}
```
没多少好说的：
(面对多行时)/**/注释符：内部每行都以一个星号开头，从而指出整个范围都是多行注释的一部分。这是良好的代码风格。

- Exercise 1.7
Compile a program that has incorrectly nested comments.
```cpp
#include <iostream>
/* comment pairs /* */ cannot nest.
 * "cannot nest" is considered source code,  
 * as is the rest of the program.
 */
int main() {
	std::cout << "incorrected nested comments" << std::endl;
	return 0;
}
```
- Exercise 1.8
```cpp
#include <iostream>
int main() {
	std::cout << "/*";
	std::cout << "*/";
	std::cout << /* "*/" */;
	std::cout << /* "*/" /* "/*" */;
	return 0;
}
```
一二句没有错，三四句都有错。
### 1.4 控制流
读取数量不定的输入数据：
```cpp
#include <iostream>
int main() {
	int sum = 0, value = 0;
	// 读取数据直到文件尾，计算所有输入的值的和
	while (std::cin >> value) 
		sum += value;
	std::cout << "Sum is: " << sum << std::endl;
	return 0;
}
```
① while循环条件的求值是执行表达式`std::cin >> value`，从标准输入读进一个值，保存在value中，然后**输入运算符返回左侧istream对象std::cin**，因此**该循环实质上在检查istream对象std::cin的状态**。

使用一个istream对象作为条件时，效果是**检测流的状态**，如果流有效，即未遇到错误就是true；当遇到文件结束符eof，或者**这里**遇到一个无效的输入(非整数)时流对象状态就会无效，并使条件变为false。

② 文件可以有end of file，那么从键盘输入呢？windows约定使用先敲CTRL+Z，然后按enter表示结束。

### 1.5 类简介
创建属于我们自己的类类型！`为了访问标准库设施，我们必须包含相关的头文件，类似的，我们也需要头文件来访问为自己的应用程序所定义的类`。**头文件习惯上用其中定义的类的名字来命名**。

> 有很多垃圾书在C/C++的多文件编程上面都没讲清楚，或者一笔带过，或者从头到尾都是在.cpp文件中打转。像我以前看到一本《21天精通C》？？？浪费时间！

SalesItem类作用在于表示一本书的总销售额、售出册数和平均售价。它可以执行许多操作(当然，第一章还没有介绍到那一步)：
> 调用isbn函数从SalesItem对象提前ISBN书号；
> 输入>>和输出<<读、写SalesItem对象；
> 赋值=将一个SalesItem对象赋给另一个SalesItem对象；
> 加法+将两个SalesItem对象相加，它们必须表示同一本书(ISBN相同)，结果是一个新的SalesItem对象，ISBN不变，总销售额和售出册数为对应值之和；
> +=将一个SalesItem对象加到另外一个对象上。
 
