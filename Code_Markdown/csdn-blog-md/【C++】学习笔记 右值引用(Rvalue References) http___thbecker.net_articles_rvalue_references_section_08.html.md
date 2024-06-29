@[toc]

auto
http://thbecker.net/articles/auto_and_decltype/section_01.html
universal reference
https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers

---
# 1. 介绍
右值引用是C++的一项功能，在C ++ 11时被添加。右值引用难以理解的原因是，第一次查看它们时，不清楚**它们的目的是什么**或**它们解决了什么问题**。因此，这里不会马上讲解什么是右值引用。相反，从要解决的问题开始，然后说明右值引用如何提供解决方案。这样，右值引用的定义似乎是合理且自然的。

右值引用至少解决了两个问题：
- 实现移动语义 `move semantics`
- 完美转发 `Perfect forwarding`

这两者将在下面详细说明，先从移动语义开始。需要提醒的是，对于C++中的左值和右值，给出严格的定义非常困难。下面的解释足以满足当前的目的。

最早的时候，在C中左值和右值的原始定义如下：**左值和右值都是表达式 `expression` ；左值 `lvalue` 可以出现在赋值运算的左侧或右侧，而右值 `rvalue` 只能出现在赋值运算的右侧**。例如：
```cpp
int a = 42;
int b = 43;
//a和b都是左值
a = b; //ok
b = a; //ok
a = a * b; //ok
//a * b是右值
int c = a * b; //ok, 右值可以出现在赋值运算的右侧
a * b = 42;    //error, 右值出现在了赋值运算的左侧
```
在C++中，作为对左值和右值的第一种直观认识，这仍然生效。但是，C++及其用户定义类型引入了一些细微之处，考虑到 `modifiability` 和 `assignability` ，这会导致此定义不正确。我们没有必要进一步探讨这一点。这里给出一个替代定义（尽管它仍有可争议之处），能够处理右值引用：**左值 `lvalue` 是一个引用内存位置的表达式，它允许我们通过 `&` 运算符获取该内存位置的地址；右值 `rvalue` 则是一个不是左值的表达式**。例如：
```cpp
//lvalues:
int i = 42; 
i = 43; 	 //ok, i是一个左值
int* p = &i; //ok, i是一个左值
int& foo();
foo() = 42;  //ok, foo()是一个左值, foo是一个返回左值引用的函数
int* p1 = &foo(); //ok, foo()是一个左值

//rvalues:
int foobar();
int j = 0;
j = foobar(); //ok, foobar()是一个右值
int* p2 = &foobar(); //error, 不能取得一个右值的地址
j = 42; 	  //ok, 42是一个右值
```

---
# 2. 移动语义
假设 `X` 是一个类，其中持有一个指向某个资源（例如`m_pResource` ）的指针或句柄。所谓资源，是指任何需要花费大量精力进行构建、克隆或破坏的事物。一个很好的例子是 `std::vector` ，它保存了一组存在于分配的内存数组中的对象。然后从逻辑上讲，`X` 的复制赋值运算符 `copy assignment operator` 如下所示：
```cpp
X& X::operator=(X const &rhs) {
  //[...]
  //复制rhs.m_pResource所指的内容
  //销毁m_pResource引用的资源
  //将克隆附加到m_pResource
  //[...]
}
```
类似的推理适用于复制构造函数 `copy constructor` 。现在假设 `X` 的用法如下：
```cpp
X foo(); //一个返回右值X的函数
X x;
//可能以多种方式使用x
x = foo();
```
最后一行：
- 从 `foo` 返回的临时变量中克隆资源；
- 销毁 `x` 持有的资源，并用克隆替换它；
- 销毁临时变量，从而释放其资源。

显然，在 `x` 和临时对象之间交换资源指针（句柄），然后让临时对象的析构函数销毁 `x` 的原始资源，这是可以的，而且效率更高（[剑指Offer 面试题1：赋值运算符函数](https://blog.csdn.net/myRealization/article/details/107624267)）。换句话说，在特殊情况下，赋值运算的右侧是一个右值，我们希望复制赋值运算符的行为如下：
```cpp
//[...]
//swap m_pResource and rhs.m_pResource
//[...]
```
这被称为**移动语义** `move semantics` 。使用C++11，可以通过重载来实现这种 `conditional behavior` ：
```cpp
x& x::operator=(<mystery type> rhs) {
	//[...]
	//swap this->m_pResource and rhs.m_pResource
	//[...]
}
```
由于我们正在定义**复制赋值运算符的重载函数**，因此这里的"神秘类型"本质上**必须是一个引用**，我们当然希望右侧通过引用传递给我们。此外，我们期望这一神秘类型具有以下行为：当在两个重载函数之间进行选择时，其中一个重载函数是普通引用，另一个重载函数是神秘类型，则 `rvalues` 必须优先选择神秘类型，而 `lvalues` 必须优先选择普通引用。

如果现在在上面用 `rvalue reference` 代替 `<mystery type>` ，那么实际上看到的就是右值引用的定义。

---
# 3. 右值引用
如果 `X` 是任意类型，则 `X&&` 称为对 `X` 的右值引用。为了更好区分，普通引用 `X&` 现在也称为左值引用。

一个右值引用 `X&&` 是一种类型，其行为与普通引用 `X&` 相似，但有一些例外。最重要的是，对于函数重载解析，左值更喜欢旧式左值引用，而右值更喜欢新的右值引用：

```cpp
void foo(X& x);  //lvalue优先选择这一函数重载
void foo(X&& x); //rvalue优先选择这一函数重载

X x;
X foo();

foo(x); 		//参数是一个左值,调用foo(X&)
foo(foobar());  //参数是一个右值,调用foo(X&&)
```
因此，要点是：**右值引用允许函数在编译期（通过重载解析）在以下条件上分支："是否在左值或右值上调用我？"**
 
诚然，我们可以通过这种方式重载任何函数，如上所示。但是在绝大多数情况下，**这种重载应该仅发生在复制构造函数 `copy constructor` 和赋值运算符 `copy assignment operator` 中，以实现移动语义**：

```cpp
X& X::operator=(X const &rhs); //classical实现
X& X::operator=(X &&rhs) {
	//move semantics: 交换this和rhs的内容
	return *this;
}
```
为复制构造函数实现右值引用重载是类似的。
> `Caveat` 警告：C++中经常发生这种情况，乍看之下似乎还算是完美的东西仍有瑕疵。事实证明，在某些情况下，在上面的复制赋值运算符的实现中，`this` 和 `rhs` 之间的简单内容交换还不够好。在下面的第4节"强制移动语义"中再次讨论这一点。
 
注意：
- 如果实现 `void foo(X&);` ，而不是 `void foo(X&&);` ， 行为当然是不变的。`foo()` 可以在左值上调用，但不能在右值上调用。
- 如果实现 `void foo(X const&);` ，而不是 `void foo(X&&);` ，行为也不会改变。可以在左值和右值上调用 `foo()` ，但是不可能区分左值和右值。
- 只有实现 `void foo(X&&);` ， 但没有一个 `void foo(X&);` 和 `void foo(X const &);` ，然后根据C++11的最终版本，可以在右值上调用 `foo()` ，尝试在左值上调用 `foo()` 会触发编译错误。

---
# 4. 强制移动语义
众所周知，C++标准的第一修正案指出： ***"委员会不得制定任何规则，以防止C++程序员发怒。"*** 不用多说，在给程序员更多的控制权和避免他们的粗心大意之间进行选择时，C++倾向于提供更多控制权。秉承这种精神，C++11允许你**不仅在右值上使用移动语义**，还可以根据自己的判断**在左值上使用移动语义**。

一个很好的例子是STL的函数 `std::swap` 。和以前一样，让 `X` 为一个类，我们为其重载了复制构造函数和复制赋值运算符，以实现右值的移动语义：
```cpp
template<class T>
void swap(T& a, T& b) {
	T tmp(a);
	a = b;
	b = temp;
}

X a, b;
swap(a, b);
```
这里没有右值。因此，**交换中的三行语句都使用非移动语义**。但是我们知道移动语义会很合适：无论何处，一个变量作为复制构造或赋值的源出现时，该变量要么不再使用，要么仅用作赋值的目标。`wherever a variable occurs as the source of a copy construction or assignment, that variable is either not used again at all, or else it is used only as the target of an assignment.`

在C++11中，有一个名为 `std::move` 的标准库函数，可以帮助我们解决问题。**该函数无需执行任何其他操作，即可将其参数转换为右值**。因此在C++11中，标准库函数 `std::swap` 如下所示：
```cpp
template<class T>
void swap(T& a, T& b) {
	T tmp(std::move(a));
	a = std::move(b);
	b = std::move(tmp);
}

X a, b;
swap(a, b);
```
现在这三行代码都使用了移动语义。注意，对于那些没有实现移动语义（即**没有使用右值引用版本重载其复制构造函数和赋值运算符函数**）的类型，新的 `swap` 行为与旧的 `swap` 行为相同。

`std::move` 是一个非常简单的函数。但不幸的是，这里还不能展示实现，我们稍后会再讨论。另外，如上面的 `swap` 函数所示，尽可能使用 `std::move` 可以为我们带来以下重要的好处：
 - 对于实现移动语义的那些类型，许多标准算法和操作将使用移动语义，因此**可能会获得显著的性能提升**。一个重要的例子是原地排序：原地排序算法除了交换元素外几乎没有做任何其他事情，这种交换现在将利用所有提供了 `move semantics` 的类型的移动语义。
- STL通常要求某些类型的可复制性 `copyability` ，例如可用作容器元素的类型。经过仔细检查，结果发现在许多情况下，可移动性 `moveability` 就足够了。因此，我们现在可以在以前不允许使用的许多地方使用可移动但不可复制 `moveable but not copyable` 的类型（如 `unique_pointer` ）。例如，现在可以将这些类型作为STL容器的元素。

现在我们了解了 `std::move` ，可以看到为什么前面显示的**复制赋值运算符函数使用右值引用进行重载的实现**仍然有问题。考虑变量之间的简单分配，如下所示：
```cpp
a = b;
```
希望在这里发生什么？希望 `a` 持有的对象被 `b` 的副本替换，并且在替换过程中，希望 `a` 之前持有的对象被析构。现在考虑线
```cpp
a = std::move(b);
```
如果将移动语义实现为简单的交换，则其作用是在 `a` 和 `b` 之间交换由 `a` 和 `b` 持有的对象。没有任何东西被破坏。当然，先前由 `a` 持有的对象最终会被破坏，也就是说，当 `b` 超出范围时。除非 `b` 成为移动的目标，在这种情况下，先前由 `a` 持有的对象将再次传递。因此，就复制赋值运算符的实现者而言，尚不清楚何时由 `a` 持有的对象将被销毁。

因此从某种意义上讲，我们已经进入了析构不确定 `non-deterministic destruction` 的世界：一个变量被赋值，但是以前由该变量持有的对象仍然在某个地方。只要该对象的销毁没有任何外部可见的副作用，就还不错。

但是有时候析构函数确实有这种副作用，一个例子是释放析构函数中的锁。因此，对象析构中任何具有副作用的部分，都应该在复制赋值运算符的右值引用重载版本中被显式执行：
```cpp
X& X::operator=(X&& rhs) {
 	// Perform a cleanup that takes care of at least those parts of the
  	// destructor that have side effects. Be sure to leave the object
  	// in a destructible and assignable state.

    // Move semantics: exchange content between this and rhs
  	return *this;
}
```
 
 
---
# 5. 右值引用是右值吗？
和以前一样让 `X` 为一个类，我们已经**为其重载了复制构造函数和复制赋值运算符**以实现移动语义。现在考虑：
```cpp
void foo(X&& x){
	X anotherX = x;
	//...
}
```
有趣的问题是：在 `foo()` 的主体中调用了 `X` 的复制构造函数的哪个重载？在此，`x` 是一个声明为右值引用的变量，即优先且通常（尽管不一定！）引用一个右值。因此，可以预期 `x` 本身也应该像右值那样绑定，即 `X(X&& rhs);` 应该被调用。换句话说，可能会期望任何声明为右值引用的东西本身就是右值。

右值引用的设计者选择了一个比这更微妙的解决方案：**声明为右值引用的事物可以是左值或右值**。区别标准是：**如果有名称则为左值，否则它是一个右值**。
 
在上面的示例中，**声明为右值引用的事物具有名称**，因此它是左值：
```cpp
void foo(X&& x) {
	X anotherX = x; //调用 X(X const &rhs)
```
这是一个**声明为右值引用且没有名称**的示例，因此是右值：
```cpp
X&& goo();
X x = goo(); //调用 X(X&& rhs) 因为右边的对象没有名字
```
这是设计背后的基本原理：**允许移动语义有默契地被应用于具有名称的对象**，例如：
```cpp
X anotherX = x;
//x仍在作用域scope中
```

这会造成危险的混乱，并且容易出错，**因为我们刚刚移动的东西（即我们刚刚窃取的东西）在后续代码行中仍然可以访问**。但是移动语义的全部要点是：仅在"无关紧要"的地方应用它，从某种意义上说，**我们移动的物体会在移动之后立即死去并消失**。因此，规则为“如果有名称，则为左值”。

那么其他部分呢，"如果没有名称，那么它就是右值？"继续使用上面的 `goo` 示例，从技术上讲，示例的第二行中的 `goo()` 表达式可能（尽管不太可能）指向从其移走后仍可访问的对象。但是回想一下上一节：有时这就是我们想要的！**我们希望能够根据自己的判断强制在左值上移动语义**，而正是这样的规则："如果没有名称，那就是右值''，使我们能够以受控的方式实现这一目标。

这就是函数 `std::move` 的工作方式。尽管现在展示确切的实现还为时过早，但我们离了解 `std::move` 又近了一步。它通过引用直接传递其参数，不进行任何操作，其结果类型为右值引用。所以表达
```cpp
std::move(x)
```
**被声明为右值引用，并且没有名称，因此，它是一个右值**。这样，`std::move` 将其参数转换为右值（即使参数不是），并通过隐藏名称来实现这一点。

这是一个示例，显示意识到 `if-it-has-a-name` 规则的重要性。假设您已经编写了一个 `Base` 类，并且通过重载 `Base` 的复制构造函数和赋值运算符来实现了移动语义：
```cpp 
Base(Base const &rhs); //non-move semantics
Base(Base&& rhs);      //move semantics
```
现在编写一个派生自 `Base` 的类 `Derived` 。为了确保将移动语义应用于 `Derived` 对象的 `Base` 部分，还必须重载 `Derived` 的拷贝构造函数和拷贝赋值运算符。让我们看一下拷贝构造函数，拷贝赋值操作符的处理类似。左值的版本很简单：
```cpp
Derived(Derived const &rhs) : Base(rhs) {
	//Derived-specific stuff
}
```
右值的版本具有很大的微妙之处。这是一个不知道 `if-it-has-a-name` 规则的人可能会做的事情：
```cpp
Derived(Derived&& rhs) : Base(rhs) { //wrong: rhs是一个左值
	//Derived-specific stuff
}
```
如果我们要这样编码，则将调用 `Base` 的拷贝构造函数的非移动版本，因为具有名称的 `rhs` 是左值。我们要调用的是 `Base` 的移动拷贝构造函数，而实现该目标的方法是编写这样的代码：
```cpp
Derived(Derived&& rhs) : Base(std::move(rhs)) { //good, 调用Base(Base&& rhs)
	//Derived-specific stuff
}
```
---
# 6. 移动语义和编译器优化
考虑以下函数定义：
```cpp
X foo() {
	X x;
	//perhaps do something to x
	return x;
}
```
现在假设像以前一样，`X` 是一个类，我们已经为其重载了拷贝构造函数和拷贝赋值运算符以实现移动语义。如果将上面的函数定义当做表面值，你可能会想说，从 `x` 到 `foo()` 返回值的位置之间发生了一次值拷贝。让我确保我们使用的是移动语义：
```cpp
X foo() {
	X x;
	//perhaps do something to x
	return std::move(x); //making it worse!
```
不幸的是，那会使事情变得更糟而不是更好。任何现代的编译器都会将**返回值优化 `return value optimization`** 应用于原始函数定义。换句话说，编译器将直接在 `foo()` 返回值的位置构造 `X` 对象，而不是在本地构造 `X` 然后将其复制出来。显然，这甚至比移动语义更好。

如您所见，为了真正**以最佳方式使用右值引用和移动语义**，需要充分理解并考虑当今编译器的特殊效果，例如返回值优化和复制省略。有关详细讨论，参见 `Scott Meyers` 的书 `Effective Modern C++` 中第25和41条。它的确很微妙，但是，出于某种原因，我们选择C++作为我们的语言，对吗？我们整理好了床铺，现在让我们躺在床上。

---
# 7. 完美转发：问题
除了移动语义之外，右值引用旨在解决的另一个问题是完美转发。考虑以下简单的工厂函数：
```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Arg arg) {
	return shared_ptr<T>(new T(arg));
}
```
显然，这里的目的是将参数 `arg` 从工厂函数转发到 `T` 的构造函数。理想情况下，就 `arg` 而言，一切都应该表现得好像没有工厂函数，而构造函数是直接在客户端代码中调用的：完美转发。

上面的代码不幸地失败了：它引入了一个额外的按值调用，如果构造函数通过引用接受其参数，则更加糟糕。最常见的解决方案，例如通过 `boost::bind` ，是让外部函数通过引用来接受参数：

```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Arg& arg) {
	return shared_ptr<T>(new T(arg));
}
```
这种做法更好，但是不完美。问题在于，现在无法在右值上调用工厂函数：
```cpp
factory<X>(hoo()); //error if hoo returns by value
factor<X>(41); 	   //error
```
这可以通过提供一个重载来解决，该重载使用常量引用 `const refer` 作为参数：
```cpp
template<typename T, typename Arg>
shared_ptr<T> factory(Arg const &arg) {
	return shared_ptr<T>(new T(arg));
}
```
这种方法有两个问题。首先，如果工厂没有一个而是几个参数，则必须为各种参数的非 `const` 和 `const` 引用的所有组合提供重载。因此，该解决方案扩展性极差，无法使用带有多个参数的函数。

其次，这种转发并不完美，因为它阻止了移动语义：工厂函数的主体中，`T` 的构造函数的参数是左值。因此，即使没有 `wrap` 函数，移动语义也永远不会发生。

事实证明，右值引用可用于解决这两个问题。它们使得无需使用重载，即可实现真正完美的转发。为了了解怎样做到这一点，我们需要查看另外两个有关右值引用的规则。

---
#   完美转发：解决方案
#    右值引用和异常
#    隐性举动的情况
#    致谢和进一步阅读


