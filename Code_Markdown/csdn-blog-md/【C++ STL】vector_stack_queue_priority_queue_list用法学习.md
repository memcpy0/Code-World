数组作为基本的数据结构，有静态数组和动态数组两种类型。比赛中，空间足够，能用静态数组就静态，不然就用vector，最后才是用指针管理动态数组。

vector是一个模板类，其中的元素连续存储。

使用方法如下：
1. 定义：常用的定义方法如下：

 例子 | 说明 |
|:--|:--
| vector<int> a; |                               默认初始化，a空
|vector<int> b(a);  | 用a定义b
|  vector<int> a(100); | a有100个默认初始化为0的元素 
| vector<int> a(100, -1); | a用100个值为-1的元素
| vector<string> a(100, "hello"); | a有100个值为"hello"的元素 
| vector<node> a; | a可以保存自定义的结构体类型

2. 常见操作方法：

 例子 | 说明 |
|:--|:--
| a.push_back(100); | 尾部插入一个元素
| a.emplace_back(100); | 尾部构造一个元素
| a.size() | 元素个数
| bool a.empty() | 是否为空
| a.insert(a.begin() + i, k); | 插入一个元素
| a.insert(a.end(), k, j); | 在尾部插入k个值为j的元素
| a.pop_back(); | 删除尾部
| a.erase(a.begin() + i, a.end() + j);  | 删除区间
| a.erase(a.begin() + i);  | 删除元素
| a.resize(n); | 调整大小
| a.clear(); | 清空
| reverse(a.begin(), a.end()); | 翻转
| sort(a.begin(), a.end()); | 排序

---
栈是基本的数据结构：
```cpp
stack<Type> s; 
s.push(item);  
s.pop(); // 删除栈顶的元素但不返回
s.top(); // 返回栈顶的元素但不删除
s.size();
s.empty();
```
队列的操作差不多：
```cpp
queue<Type> q;
q.push(item);  
q.front(); // 返回队首元素但不删除
q.pop();  // 删除队首元素
q.back(); // 返回队尾元素但不删除
q.size(); 
q.empty();
```
优先队列，可以是大顶堆，也可以是小顶堆。下面的pop/push都是O(logn)。可以用优先队列进行堆排序和Dijkstra算法的优化。
```cpp
priority_queue<Type> q;
q.top();  // 返回具有最高优先级的元素值但不删除 
q.pop();  // 删除最高优先级元素
q.push(item); // 插入新元素
```

---
1.关于list容器

list是一种序列式容器。list容器完成的功能实际上和数据结构中的双向链表是极其相似的，list中的数据元素是通过链表指针串连成逻辑意义上的线性表，也就是list也具有链表的主要优点，即：在链表的任一位置进行元素的插入、删除操作都是快速的。list的实现大概是这样的：list的每个节点有三个域：前驱元素指针域、数据域和后继元素指针域。前驱元素指针域保存了前驱元素的首地址；数据域则是本节点的数据；后继元素指针域则保存了后继元素的首地址。其实，list和循环链表也有相似的地方，即：头节点的前驱元素指针域保存的是链表中尾元素的首地址，list的尾节点的后继元素指针域则保存了头节点的首地址，这样，list实际上就构成了一个双向循环链。由于list元素节点并不要求在一段连续的内存中，显然在list中是不支持快速随机存取的，因此对于迭代器，只能通过“++”或“--”操作将迭代器移动到后继/前驱节点元素处。而不能对迭代器进行+n或-n的操作，这点，是与vector等不同的地方。


我想把三个常用的序列式放在一起对比一下是有必要的：

vector ： vector和built-in数组类似，拥有一段连续的内存空间，能非常好的支持随即存取，即[]操作符，但由于它的内存空间是连续的，所以在中间进行插入和删除会造成内存块的拷贝，另外，当插入较多的元素后，预留内存空间可能不够，需要重新申请一块足够大的内存并把原来的数据拷贝到新的内存空间。这些影响了vector的效率，但是实际上用的最多的还是vector容器，建议大多数时候使用vector效率一般是不错的。

list：      list就是数据结构中的双向链表(根据sgi stl源代码)，因此它的内存空间是不连续的，通过指针来进行数据的访问，这个特点使得它的随即存取变的非常没有效率，因此它没有提供[]操作符的重载。但由于链表的特点，它可以以很好的效率支持任意地方的删除和插入。

deque： deque是一个double-ended queue，它的具体实现不太清楚，但知道它具有以下两个特点：它支持[]操作符，也就是支持随即存取，并且和vector的效率相差无几，它支持在两端的操作：push_back,push_front,pop_back,pop_front等，并且在两端操作上与list的效率也差不多。


因此在实际使用时，如何选择这三个容器中哪一个，应根据你的需要而定，具体可以遵循下面的原则：
1. 如果你需要高效的随即存取，而不在乎插入和删除的效率，使用vector
2. 如果你需要大量的插入和删除，而不关心随即存取，则应使用list
3. 如果你需要随即存取，而且关心两端数据的插入和删除，则应使用deque。


2.list中常用的函数

2.1list中的构造函数：

list() 声明一个空列表；

list(n) 声明一个有n个元素的列表，每个元素都是由其默认构造函数T()构造出来的

list(n,val) 声明一个由n个元素的列表，每个元素都是由其复制构造函数T(val)得来的

list(n,val) 声明一个和上面一样的列表

list(first,last) 声明一个列表，其元素的初始值来源于由区间所指定的序列中的元素

2.2 begin()和end()：通过调用list容器的成员函数begin()得到一个指向容器起始位置的iterator，可以调用list容器的 end() 函数来得到list末端下一位置，相当于：int a[n]中的第n+1个位置a[n]，实际上是不存在的，不能访问，经常作为循环结束判断结束条件使用。

2.3 push_back() 和push_front()：使用list的成员函数push_back和push_front插入一个元素到list中。其中push_back()从list的末端插入，而 push_front()实现的从list的头部插入。

2.4 empty()：利用empty() 判断list是否为空。

2.5 resize()： 如果调用resize(n)将list的长度改为只容纳n个元素，超出的元素将被删除，如果需要扩展那么调用默认构造函数T()将元素加到list末端。如果调用resize(n,val)，则扩展元素要调用构造函数T(val)函数进行元素构造，其余部分相同。

2.6 clear()： 清空list中的所有元素。

2.7 front()和back()： 通过front()可以获得list容器中的头部元素，通过back()可以获得list容器的最后一个元素。但是有一点要注意，就是list中元素是空的时候，这时候调用front()和back()会发生什么呢？实际上会发生不能正常读取数据的情况，但是这并不报错，那我们编程序时就要注意了，个人觉得在使用之前最好先调用empty()函数判断list是否为空。

2.8 pop_back和pop_front()：通过删除最后一个元素，通过pop_front()删除第一个元素；序列必须不为空，如果当list为空的时候调用pop_back()和pop_front()会使程序崩掉。

2.9 assign()：具体和vector中的操作类似，也是有两种情况，第一种是：l1.assign(n,val)将 l1中元素变为n个T(val）。第二种情况是：l1.assign(l2.begin(),l2.end())将l2中的从l2.begin()到l2.end()之间的数值赋值给l1。

2.10 swap()：交换两个链表(两个重载)，一个是l1.swap(l2); 另外一个是swap(l1,l2)，都可能完成连个链表的交换。

2.11 reverse()：通过reverse()完成list的逆置。

2.12 merge()：合并两个链表并使之默认升序(也可改)，l1.merge(l2，greater<int>()); 调用结束后l2变为空，l1中元素包含原来l1 和 l2中的元素，并且排好序，升序。其实默认是升序，greater<int>()可以省略，另外greater<int>()是可以变的，也可以不按升序排列。

看一下下面的程序：
 

```cpp
#include <iostream>
#include <list>
using namespace std;
int main()
{
     list<int> l1;
     list<int> l2(2,0);
     list<int>::iterator iter;
     l1.push_back(1);
     l1.push_back(2);
     l2.push_back(3);
     l1.merge(l2, greater<int>());//合并后升序排列，实际上默认就是升序
     for(iter = l1.begin() ; iter != l1.end() ; iter++)
     {
         cout<<*iter<<" ";
     }
     cout<<endl<<endl;
     if(l2.empty())
     {
         cout<<"l2 变为空 ！！";
     }
     cout<<endl<<endl;
     return 0;
}
```
2.13 insert()：在指定位置插入一个或多个元素(三个重载)：

l1.insert(l1.begin(),100); 在l1的开始位置插入100。

l1.insert(l1.begin(),2,200); 在l1的开始位置插入2个100。

l1.insert(l1.begin(),l2.begin(),l2.end());在l1的开始位置插入l2的从开始到结束的所有位置的元素。

2.14 erase()：删除一个元素或一个区域的元素(两个重载)

l1.erase(l1.begin()); 将l1的第一个元素删除。

l1.erase(l1.begin(),l1.end()); 将l1的从begin()到end()之间的元素删除。

---
string是STL中的字符串，虽然有点慢，但很多时候都很有用...

### 一、string的构造函数
```cpp
//生成空串
default (1)	      string();

//生成字符串为str的复制品
copy (2)	      string (const string& str);
//将字符串str中从下标pos开始、长度为len的部分作为字符串初值
substring (3)     string (const string& str, size_t pos, size_t len = npos);
//生成内容和C的char*类型一样的字符串
from c-string (4) string (const char* s);
//以C的char*类型s的前char_len个字符串作为字符串s的初值
from buffer (5)	  string (const char* s, size_t n);

//生成n个c字符的字符串
fill (6)	      string (size_t n, char c); 
```
此外还有：
```cpp
//生成一个相应位置的长度为len的子串
string substr (size_t pos = 0, size_t len = npos) const;  
 ```
## 二、string的大小和容量
```cpp 
size
length  ：Return length of string 返回string对象的长度(字符个数)

max_size：Return maximum size of string 返回string对象最大的字符容量

resize  ：Resize string 调整字符串的容量大小
capacity：Return size of allocated storage (public member function ) 返回已经分配的内存空间
clear   ：Clear string (public member function ) 清空字符串
empty   ：Test if string is empty (public member function ) 判断字符串是否为空
```
```cpp
#include <iostream>
#include <string>
using namespace std;

int main() {              
	string s("123456789"); 
	cout << "size = " << s.size() << endl;
	cout << "length = " << s.length() << endl;
	cout << "max_size = " << s.max_size() << endl;
	cout << "capacity = " << s.capacity() << endl;
	s.resize(14);
	cout << "size = " << s.size() << endl;
	cout << "length = " << s.length() << endl;
	cout << "max_size = " << s.max_size() << endl;
	cout << "capacity = " << s.capacity() << endl;	
	return 0;
}
```
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200217151159769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 ## 三、string的字符串比较
 用c的`char*`时只能用strcmp，麻烦，而string支持`>, >=, <, <=, ==, !=`的比较，甚至支持string和`char*`的比较，这种比较是根据字典序的，从前往后。一般这样比较就可以了。

## 四、string的字符访问 
```cpp
operator[]
    Get character of string (public member function )
at
    Get character in string (public member function )
back
    Access last character (public member function )
front
    Access first character (public member function )
```
可以用下标和at，当然，最常用的还是下标。
```cpp
void test6()
{
    string s1("abcdef"); // 调用一次构造函数

    // 方法一： 下标法

    for( int i = 0; i < s1.size() ; i++ ) 
        cout<<s1[i]; 
    cout<<endl;

    // 方法二：正向迭代器

    string::iterator iter = s1.begin();
    for( ; iter < s1.end() ; iter++) 
        cout<<*iter; 
    cout<<endl;

    // 方法三：反向迭代器
    string::reverse_iterator riter = s1.rbegin();
    for( ; riter < s1.rend() ; riter++) 
        cout<<*riter; 
    cout<<endl;
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200217153036934.png)
## 五、string的修改 
```cpp
operator+=
    Append to string (public member function )                 添加字符串
append
    Append to string (public member function )                 添加字符串
push_back
    Append character to string (public member function )       添加字符到字符串尾部

pop_back 
    Delete last character (public member function )            删除尾部字符
```
最常用的还是+=，还有+，以及append。

## 六、string的删除erase
```cpp
1. iterator erase(iterator p);   //删除字符串中p所指的字符                 
//删除字符串中迭代器区间[first,last)上所有字符
2. iterator erase(iterator first, iterator last); 
//删除字符串中从索引位置pos开始的len个字符 
3. string& erase(size_t pos = 0, size_t len = npos);  
```
## 七、string的查找：find
```cpp
//在当前字符串的pos索引位置开始，查找子串s，返回找到的位置索引, -1表示查找不到子串
1. size_t find (constchar* s, size_t pos = 0) const;
2. npos
    Maximum value for size_t 无法找到子串时返回的标志
```











