如何阅读C/C++中的复杂类型声明呢？一般采用**英语语序法**。有以下规则：
- 从变量开始，读作 `变量 is` ，然后先读右部的修饰符，再读左边的修饰符，如果遇到括号就跳出去重复这一动作。
- 对于 `[N]` ，读作 `an array of N` ；
- 对于 `()` ，读作 `a function that returns` 或者 `a function returning` ；
- 对于 `T *` ，读作 `a pointer to T` ；
- 其他类型名按原意阅读，有 `const` 修饰符就加上；
- 按照处理顺序，将翻译出的所有语句连接起来，就是完整的类型声明；
- 如有余力，还可以处理一下指针/函数的单复数，以及函数的参数。 <b></b> <b></b>

上述的规则用于C语言的类型声明，已经足够了，完全可以按照这个规则写一个小工具读声明。应用这一方法的现成网站是[https://cdecl.org/](https://cdecl.org/)。C++就复杂很多了，水太深难以把握。

下面就此给出几个示例。 比如 `char * const (*(* const bar)[5])(int)` ：
- 变量是 `bar` ，右边没有，左边是 `*const` ，读作 `bar is a const pointer to` ；
- 遇到括号跳出，右边是 `[5]` ，读作 `an array of 5` ，左边是 `*` ，读作 `a pointer to` ，所以是 `an array of 5 pointers to` ；
- 遇到括号跳出，右边是 `(int)` ，读作 `a function that returns` ，左边是 `char * const` ，读作 `const pointer to char` ；
- 完整的类型声明大致是， `bar` 是 `a const pointer to an array of 5 pointers to a function that returns const pointer to char` ；
- cdecl网站的翻译：`declare bar as const pointer to array 5 of pointer to function (int) returning const pointer to char` ，没有处理单复数（无 `a/an` ），对于 `[N]` 翻译得不够好。 

再比如 `int (*func(int i))[10]` ：
- 变量是 `func` ，右边是 `(int i)` ，读作 `a function returning` ，左边是 `*` ，读作 `a pointer to` ；
- 遇到括号跳出，右边是 `[10]` ，读作 `an array of 10` ，左边是 `int` ；
- 于是完整的类型声明，`func` 是 `a function returning a pointer to an array of 10 int` ；
- 由于cdecl支持的是C89标准，会报错……其实这就是一个**可改进之处**。 

再比如 `int *((*i)())[5]` ：
- 变量是 `i` ，于是 `i is` ；
- 右边没有，左边是 `*` ，读作 `a pointer to` ；
- 遇到括号跳出，右边是 `()` ，读作 `a function that returns` ；
- 遇到括号跳出，右边是 `[5]` ，读作 `an array of 5` ，左边是 `int *` ，读作 `a pointer to int` ；
 - 完整的类型声明是，`i is a pointer to a function that returns an array of 5 pointers to int` 。

以后有时间，可能会写一个读C99/C++11类型声明的小工具吧，首先要看我到时候能不能掌握这些语法知识。
