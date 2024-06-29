
我使用的主要语言是C/C++，Java学了一点，Python学了一点。如果要面试的话，基本上会考察C++。

扪心自问，我对C++的掌握也就是做做题的程度，对其语法规则什么的，了解的不是很多。所以考到了，很可能就挂了。

---
剑指Offer中说：语言考试会有三种类型：
① 直接询问面试者对C++概念的理解，如C++关键字：有哪4个和类型转换相关的关键字？
> （C++ Primer的 `4.11.3  Expressions/Type Conversions/Explicit Conversion` ）
> 有时，我们想要 `explicit force` 一个对象转换为一个不同的类型，比如，我们使用浮点除法：
> ```cpp
> int i, j;
> double slope = i / j;
> ```
> 为了这样做，我们需要一种方式 `explicit convert` 两个变量 `i and/or j` 为 `double` ，就要使用 `cast` ，`cast` 是 `an explicit conversion` 。
> 一个 `named cast` 形式如下：
> ```cpp
> cast-name<type>(expression);
> ```
> 这里 `type` 是转换的目标类型，`expression` 是要被转换的值。如果 `type` 是一个 `reference` ，结果就是一个左值 `lvalue` 。`cast-name` 有4种：`static_cast, dynamic_cast, const_cast, reinterpret_cast` ，它决定了哪种类型转换将被执行。
> - `static_cast` ：

还有关键字 `sizeof` ：定义一个空类型，里面没有任何成员变量和成员函数，对其求 `sizeof` ，结果是多少？

> （C++ Primer的 `4.9  Expressions/The sizeof Operator` ）
> `sizeof` 返回一个表达式或者类型的、以字节为单位的 `size` ，这个运算符是右结合 `right associative` 的。`sizeof` 的结果是一个类型为 `size_t` 的常量表达式 `constant expression` ，因此，我们可以使用 `sizeof` 的结果来指定数组的维度。
> 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200727230942131.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


---
