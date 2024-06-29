@[toc]

---
# 1. 参考文档
两个常用的C++参考文档：
- `cplusplus.com`：<a href="http://www.cplusplus.com/reference/functional/function/"><u>http://www.cplusplus.com/reference/functional/function/</u></a>
- `cppreference.com`：<a href="https://en.cppreference.com/w/cpp/utility/functional/function"><u>https://en.cppreference.com/w/cpp/utility/functional/function</u></a>

---
# 2. `std::funtion` 简介
`std::function` 是一个类模板，其声明如下：
```cpp
// MS C++ 2013
template<class _Fty> class function;
template<class _Fty> class function : public _Get_function_impl<_Fty>::type { ... }

// GCC 4.8.2
template<typename _Signature>                  class function;
template<typename _Res, typename... _ArgTypes> class function<_Res(_ArgTypes...)>
    : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>, private _Function_base { ... }

// cplusplus.com
template <class T> function;     // undefined
template <class Ret, class... Args> class function<Ret(Args...)>;
```
关于此类模板的说明：
- `std::function` 是一个**函数包装器模板** `function wrapper template` ，原本来自于 `boost` 库，对应其中的 `boost::function` 函数包装器，C++11将其纳入了标准库之中。
- `std::function` 能包装任何类型的**可调用元素** `callable element` ，例如**普通函数** `function` 和**函数对象** `function object` 。包装器对象的类型仅仅依赖于其**调用特征** `call signature` ，而不依赖于**可调用元素**自身的类型 `callable element type itself` 。
- 一个 `std::function` 类型的实例对象可以包装这样几种**可调用类型** `callable object` ：**普通函数** `function` 、**普通函数指针** `function pointer` 、**类成员函数指针** `pointer to member` 或者**任意类型的函数对象** `function object`（例如定义了 `operator()` 并且拥有函数闭包）。
- `std::function` 对象可以被**拷贝** `copied` 和**移动** `moved` ，并且可以使用指定的**调用特征**直接调用**可调用类型**。
- 当 `std::function` 对象没有包装任何实际的**可调用元素**时，即为**空函数** `empty function` ，调用该对象将**抛出** `std::bad_function_call` **异常**。

模板参数的说明：
- `T`：通用类型，但实际通用类型模板 `generic template` 并没有被定义，只有当 `T` 的类型为形如 `Ret(Args...)` 的函数类型才能工作。
- `Ret`：调用函数返回值的类型。
- `Args`：函数参数类型。这是一个能包装任意数量类型的模板参数。对于指向成员函数的指针，第一个类型应为指向成员的类类型的引用 `a reference to the class type the member pointed is a member` 。 

---
# 3. `std::function` 样例
## ① 包装普通函数
**非模板类型**：
```cpp
#include <iostream>
#include <functional>

int sub(int i, int j) { return i - j; } 

int main() {
	std::function<int(int, int)> f = sub;
	std::cout << f(1, 2) << std::endl; //-1
	return 0;
} 
```
**模板类型**：
```cpp
#include <iostream>
#include <functional>

template <typename T> 
T sub(T i, T j) { return i - j; } 

int main() {
	std::function<double(double, double)> f = sub<double>; //传递模板参数
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
## ② 包装普通函数指针
**非模板类型**：
```cpp
#include <iostream>
#include <functional>

int sub(int i, int j) { return i - j; } 

int main() {
	std::function<int(int, int)> f = &sub;
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
**模板类型**：
```cpp
#include <iostream>
#include <functional>

template <typename T> 
T sub(T i, T j) { return i - j; } 

int main() {
	std::function<double(double, double)> f = &sub<double>; //传递模板参数
	std::cout << f(1, 2) << std::endl;	
	return 0;
}  
```
## ③ 包装类成员函数指针
### a. 类静态成员函数
**非模板类型**：
```cpp
#include <iostream>
#include <functional>

class Ops {
public:
	static int sub(int i, int j) { return i - j; } 
};

int main() {
	std::function<int(int, int)> f = &Ops::sub;
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
**模板类型**：
```cpp
#include <iostream>
#include <functional>

class Ops {
public:
	template <typename T>
	static T sub(T i, T j) { return i - j; } 
};

int main() {
	std::function<double(double, double)> f = &Ops::sub<double>;
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
### b. 类对象成员函数
**非模板类型**：
```cpp
#include <iostream>
#include <functional>

class Ops {
public:
	int sub(int i, int j) { return i - j; } 
};

int main() {
	Ops m;
	std::function<int(int, int)> f = std::bind(&Ops::sub, 
		&m, std::placeholders::_1, std::placeholders::_2); //return m.sub
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
**模板类型**：
```cpp
#include <iostream>
#include <functional>

class Ops {
public:
	template <typename T>
	T sub(T i, T j) { return i - j; } 
};

int main() {
	Ops m;
	std::function<double(double, double)> f = std::bind(&Ops::sub<double>, 
		&m, std::placeholders::_1, std::placeholders::_2); //return m.sub<double> 
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
## ④ 包装函数对象
**非模板类型**：
```cpp
#include <iostream>
#include <functional>

struct Ops {
	int operator()(int i, int j) {
		return i - j;
	}
};

int main() {
	std::function<int(int, int)> f = Ops();
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
**模板类型**：
```cpp
#include <iostream>
#include <functional>

template <typename T> 
struct Ops {
	T operator()(T i, T j) {
		return i - j;
	}
};

int main() {
	std::function<double(double, double)> f = Ops<double>();
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
## ⑤ 包装lambda表达式对象
```cpp
#include <iostream>
#include <functional>

auto sub = [](int i, int j) { return i - j; };

int main() {
	std::function<int(int, int)> f = sub;
	std::cout << f(1, 2) << std::endl;	
	return 0;
} 
```
