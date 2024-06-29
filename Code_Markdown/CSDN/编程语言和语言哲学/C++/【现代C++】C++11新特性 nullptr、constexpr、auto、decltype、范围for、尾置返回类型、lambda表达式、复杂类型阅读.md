@[toc]

C++最标准的参考资料之一，虽然有点难度：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621135850215.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

---
# 1. 新的空指针字面量 `nullptr`
关键词 `nullptr` 是 `std::nullptr_t` 类型的**纯右值**，表示空指针**字面量**，从 `nullptr` 到任意指针类型和任意成员指针类型都存在**隐式转换**——这一转换同样适用于任何空指针**常量**，空指针**常量**包括 `std::nullptr_t` 类型的值和空指针**宏常量**  `NULL` 。

> 在C++14的头文件 `<type_traits>` 中有一个类模板 `is_null_pointer` ，可以检查**类型**是否为
> `std::nullptr_t` ： 
> ```cpp
> #include <iostream>
> #include <type_traits> 
> 
> int main() { 	
> 	std::cout << std::boolalpha 	<<
> 	std::is_null_pointer<decltype(nullptr)>::value << ' '  	<<
> 	std::is_null_pointer<std::nullptr_t>::value << ' ' 	<<
> 	std::is_null_pointer<volatile std::nullptr_t>::value << ' ' 	<<
> 	std::is_null_pointer<const volatile std::nullptr_t>::value << '\n' 	<<
> 	std::is_null_pointer<int*>::value << ' ';  	
> 	return 0; 
> }
> ```
> 运行结果如下：
> ![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621142102134.png)
> 类似的还有 `is_void, is_array, is_pointer, is_enum, is_union, is_class, is_function, is_object` ，全部都是用于**类型**检查的类模板。


原先初始化一个空指针要赋值为 `NULL` ，但是C++中的 `NULL` 是一个宏常量，其值实际上为 `0` （C中的 `NULL` 是转换为 `void *` 类型的 `0` ；不过C++中不能像C一样把 `void *` **隐式转换**成其他类型的指针，为了解决空指针的表示问题，C++干脆引入了 `0` 来表示空指针），相关的宏定义如下：
```cpp
#ifndef __cplusplus //如果没有定义__cplusplus宏,说明正在编译C语言
#define NULL ((void *)0)
#else   /* C++ */
#define NULL 0
#endif  /* C++ */
```
这样问题就来了，一个 `int` 型、值为 `0` 的字面量，在函数重载时必定会出现非预期的结果。考虑一段代码如下：
```cpp
#include <iostream>
int f(int x) {
	std::cout << "int x" << std::endl;
}
int f(int *x) { //希望调用的函数
	std::cout << "int *x" << std::endl;
}
int main() {
	f(NULL); //此处会输出"int x",与我们的想法不同,因为NULL既可以转换为指针,也相当于0
}
```
 `nullptr` 解决了 `NULL` 指代空指针时的二义性问题，下面的代码会调用 `int f(int *x)` ：
```cpp
#include <iostream> 
int f(int x) {
	std::cout << "int x" << std::endl;
}
int f(int *x) {
	std::cout << "int *x" << std::endl;
}
int main() {
	f(nullptr); 
}
```
总得来说，条件允许的前提下尽量使用 `nullptr` 。
> 此处涉及到的问题有：
> - C和C++中 `NULL` 的区别；
> - C++中 `NULL` 和 `nullptr` 的区别；
> - C++中 `nullptr` 的用处


---
# 2. 区分 `constexpr` 与 `const` 
**我们所想的常量**不一定是**程序意义上的“常量”**，因此可能引发一些意想不到的错误。考虑以下代码：
```cpp
#include <iostream>
int main() {
	int a;
	std::cin >> a;
	const int b = a + 233; //或者 int b = a + 233;  
	int c[b];  
	//这两行代码,在以前的编译器中会报错,现在倒是能够用变量或者
	//const常量定义数组长度了,即VLA(variable length array)
	return 0;
}
```
个人认为，为了避免误解，C++中的 `const` 关键字应该改为 `readonly` ，表示**只读变量**，即**变量的值在程序执行期间不会被修改**，但是每次执行程序时 `const` 类型的值**不一定相同**；`constexpr` 才是真正的**常量**类型，或者现在称为**常量表达式**类型，即**每次执行程序时都为同一个值，且程序执行期间无法被修改**，限定更加严格。

```cpp
#include <iostream>
int main() {
	int a;
	std::cin >> a;
	const int b = a + 10;
	constexpr int c = b + 10; //错误! b为常量,但不是常量表达式
	return 0;
}
```
```cpp
#include <iostream>
int main() {
	const int a = 10; //int a = 10; 也会报错 
	const int b = a + 10;
	constexpr int c = b + 10; //正确
	return 0;
}
```
当然，这方面的水很深，比如C++20新出的 `consteval, constinit` 关键字……反正我现在是把握不住的。

---
# 3. `auto` 类型指示符——自动推断类型和值初始化
遇到太长的类型名，难以拼写，浪费时间；遇到不记得的函数返回类型，无法保存返回值……这些问题都可以用 `auto` 解决。比如：
```cpp
//原来要写
vector<int> v;
for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
	//do something
}
//现在可以写成
vector<int> v;
for (auto it = v.begin(); it != v.end(); ++it) {
	//do something
}
```
`auto` 的作用，一方面**根据给出的值推断变量类型**（所以 `auto` 变量必须要有初始值），另一方面**使用给出的值初始化变量**。因此 `auto` 需要实际执行表达式：
```cpp
#include <iostream>
int f() { 
	std::cout << "Hello auto!" << std::endl;
	return 111;
}
int main() { 
	auto b = f(); 
	//b的值为111,会输出"Hello auto!",因为f被实际执行 
	std::cout << b << std::endl;
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621160046744.png)

`auto` 的水也很深，这里只讲一点和**引用**相关的。当引用被用于 `auto` 变量初始化时，真正参与初始化的是**引用对象的值**（**引用即别名**），编译器将以**引用对象的类型**作为 `auto` 变量的类型，以**引用对象的值**作为 `auto` 变量的初始值。因此下面的代码中，`p` 的类型是 `int` ，而非 `i` 变量的引用，改变 `p` 的值不会影响到 `i` ：
```cpp
#include <iostream>
int main() {
	int i = 1, &r = i; //变量i和i的引用r
	auto p = r; //p的类型为int,值为1,不是i的引用
	std::cout << "i=" << i << ", " << "p=" << p << std::endl;
	p = 3; 
	std::cout << "i=" << i << ", " << "p=" << p << std::endl;
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2021062115360227.png)
**如果想要用 `auto` 得到一个引用，需要添加引用修饰符**，此时修改 `p` 的值会影响到 `i` ：
```cpp
#include <iostream>
int main() {
	int i = 1, &r = i; //变量i和i的引用r
	auto &p = r; //p的类型为int,值为1,不是i的引用
	std::cout << "i=" << i << ", " << "p=" << p << std::endl;
	p = 3; 
	std::cout << "i=" << i << ", " << "p=" << p << std::endl;
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621153956300.png)


---
# 4. `decltype` 类型指示符——自动推断类型
如果我们只想要**用表达式的类型定义一个变量**，却不想**用表达式的值初始化这个变量**，就可以使用 `decltype` 。用法如下：
```cpp
int a = 100;
decltype(a) b;
b = 233;  
```
注意，`decltype` **只会用表达式的返回值进行类型推断**，**不会执行表达式**。

```cpp
#include <iostream>
int f() {
	std::cout << "Hello delctype!" << std::endl;
	return 111;
}
int main() { 
	decltype(f()) b = 233; 
	//b的值为233,不是111,也不会输出"Hello delctype!",因为f没有实际执行 
	std::cout << b << std::endl;
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210621155330813.png)
`decltype` 和 `auto` 都可以用于**类型推断**，两者之间除了**是否使用表达式的值初始化**、**是否实际执行表达式**这两个区别以外，还有以下不同之处：
- 处理引用：`decltype` 根据引用推断出的类型，包含引用修饰符，**此时必须初始化，不然无法编译通过**；而 `auto` 推断出的类型，会忽视掉引用，为引用对象的类型。可以说，**除了在 `decltype`下，其他情况的引用都可以视为引用对象本身。**
	```cpp
	#include <iostream>
	int main() {
		int i = 1, &r = i; //变量i和i的引用r
		decltype(r) p = r; //p的类型为int&,即为int的引用; 由于是引用,必须初始化! 值为1
		std::cout << "i=" << i << ", " << "p=" << p << std::endl; //i=1, p=1
		p = 3; 
		std::cout << "i=" << i << ", " << "p=" << p << std::endl; //i=3, p=3
		return 0;
	}
	```

- 处理顶层 `const` ：所谓顶层 `const` 指的是对象本身是 `const` 的；与之相对的是底层 `const` ，指的是对象所指向的对象是 `const` 的，一个例子是 `const int, int const` 类型和 `int *const` 类型（注意，**`const` 限定词适用于紧靠左侧的类型**，除非左侧没有任何内容，才适用于紧靠右侧的类型）。以指针为示例，区分顶层和底层 `const` （阅读时从右往左读）：
	- `int const*` ：底层 `const` ，`pointer to const int` ；
	- `int const *const` ：顶层和底层 `const` ，`const pointer to const int` ；
	- `int *const` ：顶层 `const` ，`const pointer to int` ；
	- `int *const *` ：底层 `const` ，`pointer to const pointer to int`  
	- ……
	
	**`auto` 会忽视掉顶层 `const` 但是保留底层 `const` ，如果要让 `auto` 变量的类型为顶层 `const` ，需要自行添加 `const` 修饰符**。 而 `decltype` 会返回表达式返回值的类型，包括引用和顶层 `const` 。
	```cpp
	#include <iostream>
	int main() {
		int const i = 0, &r = i; //顶层const
		auto a = i; //a为int,忽略顶层const
		auto b = r; //b为int,忽略顶层const和引用
		auto c = &i; //c为int const*,即指向常量int的指针,保留底层const
	//	*c = 2; //错误,对只读变量*c赋值! 
		auto const d = i; //添加const修饰符,d为int const类型,即常量int
	
		decltype(i) x = 2; //保留顶层const,x为int const,必须初始化
		decltype(r) y = x; //保留顶层const和引用,y为int const&,是对常量的引用,引用必须初始化  
	//	decltype(r) z;	//错误,z是一个对常量的引用,引用必须初始化！ 
		return 0;
	}
	```

---
# 5. 范围遍历的 `for` 语句
即使有了 `auto` 类型指示符，遍历容器和数组也有点麻烦。现在有了范围 `for` 语句，就可以写得很简洁了：
```cpp
//有了auto
vector<int> v;
for (auto it = v.begin((); it != v.end(); ++it) cout << *it << ' ';

//现在有了范围for
vector<int> v;
for (auto val : v) cout << val << ' ';
```
---
# 6. 尾置返回类型
普通函数基本上不需要**尾置返回类型**，不过如果函数的返回类型很复杂，尾置返回类型的用处就很大了。比如说 `int (*func(int i))[10]` 这种类型。关于如何阅读这些复杂类型，可见[此处](https://memcpy0.blog.csdn.net/article/details/118102194)。

不过有了尾置返回类型，就可以像寻常定义变量的写法一样写返回类型了：
```cpp
int (*func(int i))[10] {
	//do something
}
//代替一维数组
auto func(int i) -> int(*)[10] { //返回一维数组指针
	//do something
}
//代替二维数组
auto func(int i) -> int(*)[10][10] {
	//do something
}
//代替二重指针
auto func(int i) -> int ** { //这个可以不使用
	//do something
}
```
如果出现了更加复杂的返回类型，就可以用 `decltype + auto` 的双重组合拳。一个简要示例如下：
```cpp
auto func(int a, int b) -> decltype(a + b) {
	//do something
	return a + b; 
}
```
在C++14以后，连尾置返回类型都可以省略了，直接返回 `auto` 就可以了，编译器会解决的。**只要不过分挑剔，现代C++的写法完全可以像Python一样简洁优美！（正论）**

---
# 7. lambda表达式
lambda表达式即所谓的匿名函数，会生成一个 `function object` ，常用作C++ STL中函数模板、类模板的谓词，比如 `sort, transform, partial_sum` 等等。一个**完整**的lambda表达式的格式如下：
```cpp
[capture list] (params list) mutable exception -> return type { function body }
```
各个部分的具体格式为：
- `[capture list]` ：捕获外部变量列表，可以为空的 `[]` ，**不可以省略**；
- `(params list)` ：形式参数列表，可以为空的 `()` ，**不可以省略**；
- `mutable` 指示符：说明**是否可以修改捕获的变量**，**可以省略**；
- `exception` ：异常设定，**可以省略**；
- `return type` ：尾置返回类型，**可以省略**；
- `function body` ：函数体，可以为空的 `{}` ，**不可以省略**。

此处我们重点关注捕获列表和返回类型。
## 7.1 值捕获
类似于普通函数参数的值传递，被捕获变量的值将在**lambda表达式创建时**，通过**值拷贝**方式传入，**外部对该变量的修改**不会影响到lambda表达式内部的值，更重要的是，**lambda表达式内部不能修改值捕获变量的值**，或者说，**值捕获的变量均为 `read-only` 变量**。
```cpp
#include <iostream> 
int main() {
	int t = 123;
	auto func = [t]() {
//		t = 233; //错误！lambda不能修改值捕获变量 
		std::cout << t << std:: endl; 
	}; 
	t = 233; 
	func(); //输出233
	return 0;
}
```
如果要做到和**普通函数+值传递的行为**一致，就需要允许修改值捕获变量，我们要在函数体前加上 `mutable` 关键字。**此时对值捕获变量的修改，理所当然也不会影响到lambda表达式外部**。
```cpp
#include <iostream> 
int main() {
	int t = 123;
	auto func = [t]() mutable {
		t = 233; 
		std::cout << t << std:: endl; 
	};  
	func(); //输出233
	std::cout << t << std::endl; //输出123 
	return 0;
}
```
## 7.2 引用捕获
类似于普通函数参数的引用传递，**外部对该变量的修改**会影响到lambda表达式内部的值，**lambda表达式内部对该变量的修改**会影响到外部的值。
```cpp
#include <iostream> 
int main() { 
	int t = 123;
	auto func = [&t]() mutable { 
		std::cout << t << std:: endl; 
	};  
	func(); //输出123
	t = 233; //外部修改变量
	func(); //输出233  
	return 0;
}
```
```cpp
#include <iostream> 
int main() {
	int t = 123;
	auto func = [&t]() mutable { 
		t = 233;
	};  
	std::cout << t << std::endl; //输出123 
	func(); //lambda表达式内部修改变量 
	std::cout << t << std::endl; //输出233  
	return 0;
}
```
## 7.3 隐式捕获
不想写太多的捕获变量，就可以指定一种捕获类型，**要么全是值捕获，要么全是引用捕获**，让编译器推断需要捕获哪些变量。
```cpp
#include <iostream> 
int main() {
	int a = 11, b = 22, c = 33; 
	auto func = [=]() { //全是值捕获 
		std::cout << a << ' ' << b << ' ' << c << ' ' << std::endl; 
	};  
	func(); //输出11 22 33 
	return 0;
}
```
```cpp
#include <iostream> 
int main() {
	int a = 11, b = 22, c = 33; 
	auto func = [&]() { //全是引用捕获 
		a = 1, b = 2, c = 3; 
	};  
	func();   
	std::cout << a << ' ' << b << ' ' << c << ' ' << std::endl;  //输出1 2 3 
	return 0;
}
```
## 7.4 混合捕获
如果有些变量需要值捕获，有些需要引用捕获，就需要混合起来：
```cpp
auto func = [&a, b]() { //a引用捕获, b值捕获
	//do something
}
auto func = [&, a]() { //a值捕获, 其余变量引用捕获
	//do something
}
auto func = [=, &a]() { //a引用捕获, 其余变量值捕获
	//do something
}
```
## 7.5 返回类型
上述的lambda表达式都没有指定尾置返回类型，却能够通过编译，也是由于编译器的作用——根据 `return` 语句推断出了lambda表达式的返回类型，不过这种推断的能力有限：
```cpp
auto func = [](int t) {
	if (t == 1) return 233;
	return 332.5; //[Error] inconsistent types 'int' and 'double' deduced for lambda return type
}
```
此时编译报错。因此**对于有多个 `return` 语句的情况，需要自行指定lambda表达式的返回类型**：
```cpp
auto func = [](int t) -> double {
	if (t == 1) return 233;
	return 332.5;
}
```



lambda表达式看似复杂，其实也有点复杂，不过不深究就可以用得很爽，特别是用于**函数式编程**。
