@[toc]

# 1. 赋值运算符函数

题目：如下为类型 `CMyString` 的声明，请为该类型添加赋值运算符函数：
```cpp
class CMyString {
public:
	CMyString(char* pData = NULL);
	CMyString(const CMyString& str);
	~CMyString(void);
private:
	char* m_pData;
};
```
考点：
- C++的基础语法的理解：运算符函数、常量引用；
- 内存泄漏的理解；
- 异常安全性的理解。

---
定义一个**赋值运算符函数**，我们需要关注如下方面：
- 将**返回值类型**声明为**该类型的引用**，并在函数结束前**返回实例自身的引用** `*this` ：
	> **只有返回一个引用，才能够允许连续赋值**。如果函数的返回值是 `void` ，应用该赋值运算符将不能连续赋值：
	>```cpp
	> CMyString str1, str2, str3;
	> ...
	> str1 = str2 = str3;
	> ```
	> 上述的做法将无法通过编译。
	
- 将**传入的参数类型**声明为**常量引用**：
	> 原因在于：
	> - 传入的参数不是引用，而是实例，则**从形参到实参会调用一次复制构造函数**，声明为引用将避免无谓的消耗；
	> - 赋值运算符函数内**不会改变传入的实例的状态**，因此应该为传入的引用参数加上 `const` 。
	
- **释放实例自身已有的内存**：
	> 如果忘记在**分配新内存之前**释放已有的空间，程序就会导致内存泄漏。
	
- **判断**传入的参数和当前的实例 `*this` **是否是同一个**：
	> 如果是同一个就不进行赋值运算，直接返回；
	> 如果不经过判断就进行赋值，那么，**释放实例自身的内存时就会导致严重的问题**：如果 `*this` 和传入的参数是同一个实例，释放了自身的内存，等于传入的参数的内存也被释放了，我们找不到需要赋值的内容了。

## (1) 解法1 初级
小小的赋值运算符函数，竟然有这么多讲究！当我们完整考虑了这几个方面后，写成如下代码：
```cpp
//返回值类型为CMyString的引用, 传入的参数为CMyString的常量引用
CMyString& CMyString::operator= (const CMyString &str) {
	if (this == &str) return *this; //同一个实例,直接返回
	delete [] m_pData;				//释放已有的内存
	m_pData = nullptr;
	m_pData = new char[strlen(str.m_pData) + 1];
	strcpy(m_pData, str.m_pData);
	return *this;					//返回实例自身的引用
}
```
## (2) 解法2 异常安全性
更高级一点，我们需要考虑到**异常安全性**。前面的函数中，分配内存之前先用 `delete` 释放了 `m_pData` 的内存，如果此时内存不足，`new char[]` 就会抛出异常。

**一旦赋值运算符函数内部抛出一个异常，CMyString的实例将不再保持在有效状态**，就违反了异常安全性原则 `Exception Safety` 。

实现异常安全性，有两种方法。简单的：`new` 分配新内容，再 `delete` 已有的内容。这样保证在分配成功之后才释放原来的内容，**分配内存失败时我们确保CMyString的实例不会被修改**。


更好的方法是：先创建一个临时实例 `strTemp` ，用到了`CMyString` 的**构造函数**，其中会用 `new` 分配内存。接着交换 `strTemp.m_pData` 和实例自身的 `m_pData` 。代码如下：
```cpp
CMyString& CMyString::operator= (const CMyString &str) {
	if (this != &str) {
		CMyString strTemp(str);
		char *pTemp = strTemp.m_pData;
		strTemp.m_pData = m_pData;
		m_pData = pTemp;
	}
	return *this;
}
```
这里的 `strTemp` 是一个局部变量，当它超出 `if` 范围时就会自动调用 `strTemp` 的析构函数，释放 `strTemp.m_pData` 指向的内存，这时 `strTemp` 指向的内存是实例之前的 `m_pData` 的内存，相当于自动调用析构函数释放实例的内存。

如果使用 `CMyString` 的构造函数创建一个临时实例时，由于内存不足抛出如 `bad_alloc` 的异常，**还没有修改原来实例的状态，原来的实例还是有效的**。这就保证了异常安全性。


