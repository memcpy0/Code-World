> 本文属于「现代C++学习实践」系列文章之一，这一系列正式开始于2021/09/04，着重于**现代C++**（即C++11、14、17、20、23等新标准）和**Linux C++服务端开发**的学习与实践。众所周知，「**C++难就难在：在C++中你找不到任何一件简单的事**」。因此，本系列将至少持续到作者本人「精通C++」为止（笑）。由于文章内容随时可能发生更新变动，欢迎关注和收藏[现代C++系列文章汇总目录](https://memcpy0.blog.csdn.net/article/details/120103284)一文以作备忘。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/Modern-Cxx-Learning-Path](https://github.com/memcpy0/Modern-Cxx-Learning-Path)。在这一仓库中，你可以看到本人学习C++的全过程，包括C++书籍源码、练习实现、小型项目等。
> <b></b> 
需要特别说明的是，为了透彻理解和全面掌握现代C++，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - C++ Primer 中文版（第5版），Stanley B. Lippman、Barbara E. Moo等著，王刚、杨巨峰译，叶劲峰、李云、刘未鹏等审校，电子工业出版社； 
> - [Bjarne Stroustrup](https://www.stroustrup.com/)老爷子的个人网站。包括[Thriving in a Crowded and Changing World: C++ 2006–2020](https://www.stroustrup.com/hopl20main-p5-p-bfc9cd4--final.pdf)及其中文版——[在拥挤和变化的世界中茁壮成长：C++ 2006–2020](https://github.com/Cpp-Club/Cxx_HOPL4_zh)（一份了解标准化背后故事、以及C++未来发展方向的绝佳材料）
> - 侯捷老师的公开课；
>    - C++面向对象高级开发上、下：正确理解面向对象的精神和实现手法，涵盖对象模型、关键机制、编程风格、动态分配；
>    - STL标准库与范型编程：深入剖析STL标准库之六大部件、及其之间的体系结构，并分析其源码，引导高阶泛型编程。
>    - C++新标准C++11/14：在短时间内深刻理解C++2.0的诸多新特性，涵盖语言和标准库两层
>    - C++内存管理机制：学习由语言基本构件到高级分配器的设计与实作，并探及最低阶`malloc` 的内部实现。
>    - C++ Startup揭密：C++程序的生前和死后。认识Windows平台下的Startup Code(启动码)，完全通透C++程序的整个运行过程。

@[toc]

> 本章所有练习已经施工完毕，见[Chapter 8. The IO Library](https://github.com/memcpy0/Modern-Cxx-Learning-Path/blob/main/Cxx-Primer/Solutions/ch08/README.md)。不保证完全正确，如有不同意见可以提Issue反馈。

C/C++不直接处理输入输出，而是通过一族定义在标准库中的类型来处理IO，这些类型支持从设备读取数据、向设备写入数据的IO操作，设备可以是文件/控制台窗口，以及某些允许内存IO的类型，如 `string`（即从 `string` 读取数据、向 `string` 写入数据）。

IO库定义了读写内置类型值的操作，如 `string` 的一些类也会定义类似的IO操作，来读写自己的对象。不过，本章只介绍IO库的基本内容，第14章介绍如何编写自己的输入输出运算符，第17章介绍如何控制输出格式、如何对文件进行随机访问。

---
# 1. IO类
到目前为止，我们之前已经使用过很多IO库设施了，包括：
- `istream`（输入流）类型，提供输入操作；
- `ostream`（输出流）类型，提供输出操作；
- `cin` ，一个 `istream` 对象，从标准输入读取数据；
- `cout` ，一个 `ostream` 对象，向标准输出写入数据；
- `cerr` ，一个 `ostream` 对象，通常用于输出程序错误消息，写入到标准错误；
- `>>` 运算符，用来从一个 `istream` 对象读取输入数据；
- `<<` 运算符，用来向一个 `ostream` 对象写入输出数据；
- `getline` 函数，从一个给定的 `istream` 读取一行数据，存入一个给定的 `string` 对象中

只是这些IO类型和对象**都是操纵 `char` 数据的**，默认情况下这些对象**都关联到用户的控制台窗口**。不过，我们不能限制实际应用程序仅从控制台窗口进行IO操作，应用程序基本都需要**读写命名文件**、**使用IO操作处理 `string` 中的字符**、**读写需要宽字符支持的语言**。

为了支持这些不同种类的IO处理操作，标准库定义了以下IO类型（表8.1），分别定义在三个独立的头文件中：`<iostream>` 定义了用于读写**流**的基本类型（控制台IO），`<fstream>` 定义了读写**命名文件**的类型（命名文件IO），`<sstream>` 定义了读写**内存 `string` 对象**的类型（内存 `string` IO）。**宽字符版本的类型、对象和函数的名字以 `w` 开始**，且与其对应的普通 `char` 版本的类型定义在同一个头文件中，用于操作 `wchar_t` 类型的数据（见2.1.1节）、支持使用宽字符的语言，**`wcin, wcout, wcerr` 是分别对应 `cin, cout, cerr` 的宽字符版对象**。
| 头文件 | 类型
|:--|:--
| `iostream` | `istream, wistream` 从流读取数据
|  | `ostream, wostream` 向流写入数据
|   | `iostream, wiostream` 读写流
|  `fstream` | `ifstream, wifstream` 从文件读取数据
|   | `ofstream, wofstream` 向文件写入数据
|   | `fstream, wfstream` 读写文件
| `sstream` | `istringstream, wistringstream` 从 `string` 读取数据
|   | `ostringstream, wostringstream` 向 `string` 写入数据
|   | `stringstream, wstringstream` 读写 `string` 。

==注：C++23中增加了数组 IO 实现，定义在头文件 `<spanstream>` ，包括 `ispanstream, ospanstream, spanstream` 以及对应的宽字符版本。==
## 1.1 IO类型间的关系
概念上，**设备类型**和**字符大小**都不会影响我们要执行的IO操作。例如用 `>>` 读取数据，不用管是从一个控制台窗口、一个磁盘文件、或是一个 `string` 。类似的，我们也不用管读取的字符能否存入一个 `char` 对象内、还是要一个 `wchar_t` 对象来存储。本章的这些标准库特性，可以**无差别地应用于普通流、文件流、`string` 流，以及 `char` 或宽字符流版本**。

标准库使我们能忽略这些不同类型的流之间的差异，是通过**继承机制** `inheritance` 实现的（第15章、第18.3节介绍C++如何支持继承机制）。利用模板（见3.3节），可以使用**具有继承关系的类**，而不必了解继承机制如何工作的细节。总之，继承机制让我们可以声明一个特定的、继承自另一个类的类，且通常可以**将一个派生类（继承类）对象当做基类（所继承的类）对象来使用**。

**输入类 `ifstream, istringstream` 都继承自 `istream` ，输出类 `ofstream, ostringstream` 都继承自 `ostream`** 。因此，可以像使用 `istream/ostream` 对象一样来使用 `ifstream, istringstream/ofstream, ostringstream` 对象，即如何使用 `cin/cout` ，就可以同样地使用这些类型的对象。例如，可以对一个 `ifstream, istringstream` 对象调用 `getline` ，或使用 `>>` 从一个 `ifstream, istringstream` 对象中读取数据，还可以使用 `<<` 向一个 `ofstream, ostringstream` 对象中写入数据。

## 1.2 IO对象无拷贝或赋值
如在7.1.3节所见，我们**不能拷贝或对IO对象赋值**。由于不能拷贝IO对象，因此也**不能将形参或返回类型设置为流类型**（见6.2.1节），进行IO操作的函数通常传递和返回「**流的引用**」。读写一个IO对象会改变其状态，因此**传递和返回的引用不能是 `const` 的**。
```cpp
ofstream out1, out2;
out1 = out2; 				// 错误：不能对流对象赋值
ofstream print(ofstream);   // 错误：不能用ofstream参数进行初始化
out2 = print(out2);   		// 错误：不能拷贝流对象
```


## 1.3 条件状态（函数和标志）
IO操作一个天然的问题是**可能发生错误**。一些错误可以恢复，其他错误则发生在系统深处，超出了应用程序可以修正的范围。IO类定义的一些函数和标志如下所示（表8.2），可以帮助我们访问和操纵流的**条件状态** `condition state` ：
| IO类的函数或标志 | 说明|
|:--|:--
| `strm::iostate` | `strm` 是一种IO类型，在表8.1中已列出，`iostate` 是一种机器无关的类型，提供了表达条件状态的完整功能
| `strm::badbit` | `strm::badbit` 用来指出流已崩溃
| `strm::failbit` | `strm::failbit` 用来指出一个IO操作失败了
| `strm::eofbit` | `strm::eofbit` 用来指出流已经到达了文件结束
| `strm::goodbit` | `strm::goodbit` 用来指出流未处于错误状态。此值保证为零。
| `s.bad()` | 若流 `s` 的 `badbit` 置位，则返回 `true`
| `s.fail()` | 若流 `s` 的 `failbit` 或 `badbit` 置位，则返回 `true`
| `s.eof()` | 若流 `s` 的 `eofbit` 置位，则返回 `true` 
| `s.good()` | 若流 `s` 处于有效状态，则返回 `true`
| `s.clear()` | 将流 `s` 中所有条件状态位复位，**将流的状态设置为有效**。返回 `void` 
| `s.clear(flags)` | 根据给定的 `flags` 标志位，将流 `s` 中对应条件状态位复位。`flags` 的类型为 `strm::iostate` 。返回 `void` 
| `s.setstate(flags)` | 根据给定的 `flags` 标志位，将流 `s` 中对应条件状态位置位。`flags` 的类型为 `strm::iostate` 。返回 `void` 
| `s.rdstate()` | 返回流 `s` 的当前条件状态，返回值类型为 `strm::iostate` 

如下举一个IO错误的例子，如在标准输入上键入 `Boo` ，读操作就会失败。代码中的输入运算符期待读取一个 `int` ，却得到了一个字符 `B` 。这样 `cin` 会进入错误状态。类似地，如果输入一个文件结束标识，`cin` 也会进入错误状态。
```cpp
int ival;
cin >> ival;
```
**一个流一旦发生错误，其上后续的IO操作都会失败**。只有一个流处于无错状态时，才可以从它读取数据、向它写入数据。由于流可能处于错误状态，因此**代码通常应在使用一个流之前，检查它是否处于良好状态**——最简单的、确定一个流对象的状态的方法，就是**将它当做一个条件来使用**。如下所示，while 循环检查 `>>` 表达式返回的流的状态，如果输入操作成功，流保持有效状态，则条件为真：
```cpp
while (cin >> word) 
	// ok: 读操作成功...
```

### 1.3.1 查询流的状态
将流作为条件使用，只能告诉我们「流是否有效」，无法告诉我们具体发生了什么。有时也要知道流为什么失败。比如键入EOF文件结束符后我们的应对措施，（可能）与遇到一个IO设备错误的处理方式是不同的。

IO库定义了一个**与机器无关的** `iostate` 类型，它提供了表达流状态的完整功能。 这一类型可作为一个**位图**来使用，使用方式与**4.8节**的 `quiz1` 一样。IO库定义了4个 `iostate` 类型的 `constexpr` 值（参见**2.4.4节**）`badbit, failbit, eofbit, goodbit` ，表示特定的位模式。这些值用来表示**特定类型的IO条件**，可以与位运算符（参见**4.8节**）一起使用来一次性检测或设置多个标志位。
- `badbit` 表示系统级错误，如不可恢复的读写错误。一旦 `badbit` 被置位，流通常就无法再使用了。
- `failbit` 表示可恢复错误，发生这类错误后，`failbit` 被置位。如期望读取数值、却读出一个字符等错误。这种问题通常是可修正的，（修正后）流还可以继续使用。
- **如果到达文件结束位置，`eofbit, failbit` 都会被置位**。
- `goodbit` 的值为 `0` ，表示流未发生错误。
- 如果 `badbit, failbit, eofbit` 任一个被置位，则检测流状态的条件会失败，所有后续输入输出操作都不能执行，直到错误被纠正或程序直接崩溃。

标准库还定义了一组函数，以查询这些标志位的状态。方法 `s.good()` 在所有错误位均未置位时返回 `true` ，而 `s.bad(), s.fail(), s.eof()` 则在对应错误位被置位时返回 `true` 。此外，**在 `badbit` 被置位时，`s.fail()` 也会返回 `true`** 。因此，**使用 `s.good(), s.fail()` 是确定流的总体状态的正确方法**。实际上，**我们将流当做条件使用的代码，等价于 `!s.fail()`** ，而 `s.eof(), s.bad()` 方法只能表示特定的错误。

### 1.3.2 管理条件状态
流对象的 `rdstate()` 返回一个 `iostate` 值，对应流的当前状态。`setstate(flags)` 操作将给定条件位置位，表示发生了对应错误。`clear()` 成员是一个重载的成员（参见**6.4节**）：有一个不接受参数的版本，另一个则接受一个 `iostate` 类型的参数。

`clear()` 不接受参数的版本，清除（复位）所有错误标志位。执行 `clear()` 后，调用 `good()` 会返回 `true` 。用法如下：
```cpp
auto old_state = cin.rdstate(); // 记住cin的当前状态
cin.clear();					// 使cin有效
process_input(cin);				// 使用cin
cin.setstate(old_state);	    // 将cin置为原有状态
```
带参数的 `clear()` 版本接受一个 `iostate` 值，表示流的新状态。为了复位单一的条件状态位，首先用 `rdstate()` 读出当前条件状态，然后用位操作将所需位复位，以生成新的状态。下面的代码将 `failbit, badbit` 复位，但保持 `eofbit` 不变：
```cpp
// 复位failbit和badbit, 保持其他标志位不变
cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);
```
## :star:8.1.2节练习
练习8.1：编写函数，接受一个 `istream&` 参数，返回值类型也是 `istream&` 。此函数要从给定流中读取数据，直至遇到文件结束标识时停止。它将读取的数据打印在标准输出上。完成这些操作后，在返回流之前，对流进行复位，使其处于有效状态。

练习8.2：测试函数，调用参数为 `cin` 。

练习8.3：什么情况下，下面的 while 循环会终止？
```cpp
while (cin >> i) /* ... */
```
---
## 1.4 管理输出缓冲
每个输出流都管理一个缓冲区，用来保存程序读写的数据。例如执行下面的代码，文本串可能立即打印出来，也可能被操作系统保存在缓冲区中、随后再打印。
```cpp
os << "please enter a value: ";
```
**有了缓冲机制，操作系统可以将程序的多个输出操作组合成单一的系统级改写操作**。由于设备的写操作相当耗时，（组合后的）单一的系统级改写操作（相比多个单独操作）可以带来很大的性能提升。

导致**缓冲刷新**（即数据真正写到输出设备或文件）的原因有很多：
- 程序正常结束，作为 `main` 函数的 `return` 操作的一部分，缓冲刷新被执行；
- 缓冲区满时，需要刷新缓冲，而后新的数据才能继续写入缓冲区；
- 使用操纵符如 `endl`（见**1.2节**）来显式刷新缓冲区；
- 在每个输出操作之后，使用操纵符 `unitbuf` 设置流的内部状态，来清空缓冲区。**默认情况下对 `cerr` 是设置 `unitbuf` 的**，所以写到 `cerr` 的内容都是立即刷新的；
- 另一个流可能关联到**输出流**。这种情况下，当读写到关联的流时，被关联到的流的缓冲区会被刷新。例如，**默认情况下 `cin, cerr` 都关联到 `cout`** 。因此读 `cin` 或写 `cerr` 都会导致 `cout` 的缓冲区刷新。

**一方面，我们需要避免缓冲更新，以提升系统性能；另一方面，我们又要依赖缓冲更新**——譬如在程序异常终止时，输出缓冲区是不会被刷新的。因此当一个程序崩溃后，它所输出的数据很可能停留在输出缓冲区中，等待打印。我们在调试一个已经崩溃的程序时，需要确认那些你认为已经输出的数据确实已经刷新了，否则可能将大量时间浪费在追踪代码为什么没有执行上——实际上代码已经执行了，只是程序崩溃后缓冲区没有被刷新，输出数据被挂起、没有打印而已。
### 1.4.1 刷新输出缓冲区
我们已经使用过操纵符 `endl` ，它完成换行并刷新缓冲区的工作。不过IO库中还有两个类似的操纵符：`flush, ends` 。`flush` 刷新缓冲区，但不输出任何额外的字符；`ends` 向缓冲区插入一个空字符，然后刷新缓冲区：
```cpp
cout << "hi!" << endl;  // 输出hi和一个换行符，然后刷新缓冲区
cout << "hi!" << flush; // 输出hi，然后刷新缓冲区，不附加任何额外字符
cout << "hi!" << ends;  // 输出hi和一个空字符，然后刷新缓冲区
```

### 1.4.2 `unitbuf` 操纵符
如果想在每次输出操作后都刷新缓冲区，可以使用操纵符 `unitbuf` ，它告诉流在接下来的每次写操作之后，都进行一个 `flush` 操作。而操纵符 `nounitbuf` 则重置流，使其恢复使用正常的、系统管理的缓冲区刷新机制：
```cpp
cout << unitbuf;   // 所有输出操作后都会立即刷新缓冲区
// 任何输出都立即刷新，无缓冲
cout << nounitbuf; // 回到正常的缓冲方式
```

### 1.4.3 关联输入流和输出流
当一个输入流被关联到一个输出流时，任何试图从输入流中读取数据的操作，都会先刷新关联到的输出流——**交互式系统通常应该关联输入流和输出流，即意味着所有输出、包括用户提示信息，都会在读操作之前被打印出来**。具体来说，标准库将 `cin` 和 `cout` 关联在一起，因此这一语句 `cin >> ival;` 导致 `cout` 的缓冲区被刷新。

`tie` 有[两个重载版本](https://zh.cppreference.com/w/cpp/io/basic_ios/tie)（参见**6.4节**）：
- 一个版本不带参数，返回指向输出流的指针，如果本对象当前关联到一个输出流，则返回的就是指向这个流的指针；如对象未关联到流，则返回空指针。
- 另一个版本则接受一个指向 `ostream` 的指针，将自己关联到此 `ostream` ，如 `x.tie(&o)` 将流 `x` 关联到输出流 `o` ，并且返回操作前的联系流，若无联系流则返回空指针。

我们既可以将一个 `istream` 对象关联到另一个 `ostream` ，也可以将一个 `ostream` 关联到另一个 `ostream` 。只不过**每个流同时最多关联到一个流，但多个流可以同时关联到同一个 `ostream`**。下面的代码中，为了彻底解开流的关联，我们传递了一个空指针；为了将一个给定的流关联到一个新的输出流，我们将新流的指针传递给了 `tie` ： 
```cpp
cin.tie(&cout);					      // 仅用于展示；标准库已将cin和cout关联在一起
// 如果有的话，old_tie指向当前关联到cin的流（即cout）
ostream *old_tie = cin.tie(nullptr);  // cin不再与其他流关联
// 将cin与cerr关联；这不是一个好主意，因为cin应该关联到cout
cin.tie(&cerr);						  // 读取cin会刷新cerr，而不是cout
cin.tie(old_tie);					  // 重建cin和cout间的正常关联
```

---
# 2. 文件输入输出
头文件 `<fstream>` 定义了三个类型来支持文件IO：`ifstream` 从一个给定文件读取数据，`ofstream` 向一个给定文件写入数据，`fstream` 可以读写给定文件。在**17.5.3节**中，介绍如何对同一个文件流既读又写。

这些类型提供的操作与之前使用的 `cin, cout` 的操作一样。特别地，我们可以用IO运算符 `<<, >>` 来读写文件，用 `getline`（参见**3.2.2节**）从一个 `ifstream` 读取数据，**8.1节**介绍的内容也都适用于这些类型。

除了继承自 `iostream` 类型的行为外，`fstream` 中定义的类型还增加了一些新的成员，以管理与流关联的文件（表8.3），这些操作可以对 `fstream, ifstream, ofstream` 对象调用，但不能对其他IO类型调用：
| `fstream` 特有的操作 | 说明
|:--|:--
| `fstream fstrm;`        |  创建一个未绑定的文件流，`fstream` 是头文件 `<fstream>` 中定义的一个类型
| `fstream fstrm(s);`  | 创建一个 `fstream` ，并打开名为 `s` 的文件。`s` 可以是 `string` 类型或一个指向C风格字符串的指针。这些构造函数都是 `explicit` 的（参见**7.5.4节**，第265页）。默认的文件模式 `mode` 依赖于 `fstream` 的类型
| `fstream fstrm(s, mode);` | 与前一个构造函数类似，但按指定 `mode` 打开文件
| `fstrm.open(s)`  | 打开名为 `s` 的文件，并将文件与 `fstrm` 绑定。`s` 类型同上。默认的文件模式 `mode` 依赖于 `fstream` 的类型。返回 `void`
| `fstrm.close()`   | 关闭与 `fstrm` 绑定的文件。返回 `void`
| `fstrm.is_open()` | 返回一个 `bool` 值，指出与 `fstrm` 关联的文件是否成功打开、且尚未关闭
 
## 2.1 使用文件流对象
想要读写一个文件时，可以定义一个文件流对象，同时将对象与文件关联起来。每个文件类都定义了一个名为 `open` 的成员函数，它完成系统相关的操作，来定位给定的文件，并视情况打开为读或写模式。创建文件流对象时，可以提供文件名（可选的）。如果提供了文件名，则 `open` 自动被调用：
```cpp
ifstream in(file);	// 构造一个ifstream并打开给定文件
ofstream out;		// 输出文件流未关联到任何文件
```
这段代码定义了一个输入流 `in` ，初始化为从文件读取数据，文件名为 `string` 类型的参数 `file` 。第二条语句定义了一个输出流 `out` ，未与任何文件关联。
> 新C++标准中，文件名既可以是库类型 `string` 对象，也可以是C风格字符数组（参见**3.5.4节**）。旧版本的标准库只允许C风格字符数组。

### 2.1.1 `iostream&` 形参可用 `fstream` 实参代替
在要求使用基类型对象的地方，可以用**继承类型的对象**来替代。即接受一个 `iostream/istream/ostream` 类型引用（或指针）参数的函数，可以传入一个对应的 `fstream/ifstream/ofstream`（或 `stringstream/istringstream/ostringstream` ）类型的对象来调用。

例如用**7.1.3节**中的 `read, print` 函数来读写命名文件，假定输入和输出文件的名字是通过传递给 `main` 函数的参数来指定的（参见**6.2.5节**，第196页）：
```cpp
ifstream input(argv[1]);   // 打开销售记录文件 
ofstream output(argv[2]);  // 打开输出文件
Sales_data total;		   // 保存销售总额的数量
if (read(input, total)) {  // 读取第一条销售记录
	Sales_data trans;	   // 保存下一条销售记录
	while (read(input, trans)) { 		  // 读取剩余记录
		if (total.isbn() == trans.isbn()) // 检查isbn
			total.combine(trans);		  // 更新销售总额
		else {
			print(output, total) << endl; // 打印结果
			total = trans; 				  // 处理下一本书
		}
	print(output, total) << endl; 		  // 打印最后一本书的销售额
} else						// 文件中无输入数据
	cerr << "No data?!" << endl;
```
除了读写的是命名文件外，这段程序同之前的诸多版本是完全相同的。重要的是对 `read, print` 的调用，这两个函数定义时指定的形参分别是 `istream&` 和 `ostream&` ，但我们可以向它们传递 `fstream` 对象。
### 2.1.2 成员函数 `open` 和 `close`
如果定义了一个空文件流对象，可以随后调用 `open` 来将它与文件关联起来：
```cpp
ifstream in(ifile); 		// 构造一个ifstream并打开给定文件
ofstream out;				// 输出文件流未与任何文件相关联
out.open(ifile + ".copy");  // 打开指定文件
```
如果调用 `open` 失败，则 `failbit` 被置位。因为 `open` 调用可能失败，**检测 `open` 是否成功通常是一个好习惯**。下面的条件判断与之前将 `cin` 用作条件相似。如果 `open` 失败，条件会为假，就不会去使用 `out` 了：
```cpp
if (out) // 检查open是否成功
		 // open成功，我们可以使用文件了
```
一旦一个文件流已经打开，它就保持与对应文件的关联。实**际上对一个已经打开的文件流调用 `open` 会失败，并导致 `failbit` 被置位**，随后的试图使用文件流的操作都会失败。想要将文件流关联到另外一个文件，必须首先关闭已经关联的文件，一旦文件成功关闭，我们可以打开新的文件：
```cpp
in.close();	 		  // 关闭文件
in.open(ifile + "2"); // 打开另一个文件
```
**如果 `open` 成功，则 `open` 会设置流的状态，使得 `good()` 为 `true`** 。

### 2.1.3 自动构造和析构
考虑这样一个程序，它的 `main` 函数接受一个要处理的文件列表（参见**6.2.5节**）。这种程序可能有如下的循环：
```cpp
// 对每个传递给程序的文件执行循环操作
for (auto p = argv + 1; p != argv + argc; ++p) {
	ifstream input(*p); // 创建输出流并打开文件
	if (input) 			// 如果文件打开成功，则处理此文件
		process(input);
	else 
		cerr << "couldn't open: " + string(*p);
} // 每个循环步input都会离开作用域，因此会被销毁
```
每个循环步都构造一个新的名为 `input` 的 `ifstream` 对象，并打开它来读取给定的文件。我们像之前一样检查 `open` 是否成功。如果成功，则将文件传递给一个函数，该函数负责读取并处理输入数据。如果失败，则打印一条错误消息并继续处理下一个文件。

由于 `input` 是 while 循环的局部变量，它在每个循环步中都要创建和销毁一次（参见**5.4.1节**）。**当一个 `fstream` 对象离开其作用域时，会被自动析构，同时会自动调用 `close` ，关闭与之关联的文件**。
## :star:8.2.1节练习
练习8.4：编写函数，以读模式打开一个文件，将其内容读入到一个 `string` 的 `vector` 中，将每一行作为一个独立的元素存入 `vector` 中。

练习8.5：重写上面的程序，将每个单词作为一个独立的元素进行存储。

练习8.6：重写7.1.1节的书店程序（第229页），从一个文件中读取交易记录。将文件名作为一个参数传递给 `main`（参见**6.2.5节**，第196页）。

---
## 2.2 文件模式
每个流都有一个关联的**文件模式** `file mode` ，用来指出如何使用文件（表8.4）：
| 文件模式 | 含义说明
|:--|:--
| `in` | 以读方式打开
| `out` | 以写方式打开
| `app` | 每次写操作前均定位到文件末尾
| `ate` | 打开文件后立即定位到文件末尾
| `trunc` | 截断文件
| `binary` | 以二进制方式进行IO

无论用哪种方式打开文件，我们都可以指定文件模式。调用 `open` 打开文件时可以，用一个文件名初始化流、隐式打开文件时也可以。只是指定文件模式有如下限制：
- 只可以对 `ofstream, fstream` 对象设定 `out` 模式；
- 只可以对 `ifstream, fstream` 对象设定 `in` 模式；
- **只有当 `out` 也被设定时，才可设定 `trunc` 模式**；
- **只要 `trunc` 没被设定，就可以设定 `app` 模式**。`app` 模式下，即使没有显式指定 `out` 模式，文件也总是以输出方式打开；
- 默认情况下，**即使没有指定 `trunc` ，以 `out` 模式打开的文件也会被截断**。为了保留以 `out` 模式打开的文件的内容，必须**同时显式指定 `app` 模式**，这样只会将数据追加写到文件末尾；或者**同时显式指定 `in` 模式**，即打开文件同时进行读写操作（参见**17.5.3节**，介绍对同一个文件既输入又进行输出的方法）
- `ate, binary` 模式可用于任何类型的文件流对象，且可以与其他任何文件模式组合使用。

**每个文件流类型都定义了一个默认的文件模式**。每次打开文件时都要设置文件模式，可能是显式地设置，也可能是隐式地设置。当程序未指定文件模式时，将使用默认模式：
- 与 `ifstream` 关联的文件默认以 `in` 模式打开；
- 与 `ofstream` 关联的文件默认以 `out` 模式打开；
- 与 `fstream` 关联的文件默认以 `in, out` 模式打开

### 2.2.1 以 `out` 模式打开文件会丢弃已有数据
默认情况下，打开一个 `ofstream` 时，文件内容会被丢弃。阻止一个 `ofstream` 清空给定文件内容的方法是，同时指定 `app` 模式：
```cpp
// 在这几条语句中，file1都被截断
ofstream out("file1");	// 隐含以out模式打开文件并截断文件
ofstream out2("file1", ofstream::out); // 隐含地截断文件
ofstream out3("file1", ofstream::out | ofstream::trunc); //只有当out被设定时，才可设定trunc模式
// 为了保留文件内容，必须显式指定app模式
ofstream app("file2", ofstream::app); // 隐含为out模式
ofstream app("file2", ofstream::out | ofstream::app);
```
### 2.2.2 每次调用 `open` 时都可确定文件模式
一个给定流的模式不是一成不变、不可更改的。每当打开文件时，都可以改变其文件模式。下面的代码中，第一个 `open` 调用未显式指定文件模式，于是对 `ofstream` 关联的文件默认地以 `out` 模式打开，这通常意味着同时使用 `trunc` 模式，因此当前目录下文件 `scratchpad` 的内容将被清空。打开文件 `precious` 时，我们指定了 `append` 模式，文件中已有的数据都得以保留，所有写操作都在文件末尾进行。
```cpp
ofstream out; // 未指定文件打开模式
out.open("scratchpad"); // 模式隐含设置为out和trunc
out.close();  // 关闭out，以便我们将其用于其他文件
out.open("precious", ofstream::app); // 模式为out和app
out.close();
```
## :star:8.2.2节练习
练习8.7：修改上一节的书店程序，将结果保存到一个文件中。将输出文件名作为第二个参数传递给 `main` 函数。

练习8.8：修改上一题的程序，将结果追加到给定的文件末尾。对同一个输出文件，运行程序至少两次，检验数据是否得以保留。


---
# 3. `string` 流
`<sstream>` 头文件定义了三个类型，以支持内存IO。这些类型可以向 `string` 写入数据、从 `string` 读取数据，就像 `string` 是一个IO流一样。

`istringstream` 从 `string` 读取数据，`ostringstream` 向 `string` 写入数据，`stringstream` 即可从 `string` 读数据、也可向 `string` 写数据。与 `fstream` 类型相似，`<sstream>` 头文件中定义的类型都继承自 `<iostream>` 头文件中定义的类型。除了继承来的操作，`<sstream>` 中定义的类型，还增加了一些成员来管理与流相关联的 `string` 。表8.5列出了这些操作，可以对 `stringstream` 对象调用这些操作，但不能对其他IO类型调用这些操作。
| `stringstream` 特有的操作 | 含义说明 |
|:--|:--
| `sstream strm;` | `strm` 是一个未绑定的 `stringstream` 对象，`sstream` 是头文件 `<sstream>` 中定义的一个类型
| `sstream strm(s);` | `strm` 是一个 `sstream` 对象，保存 `string s` 的一个拷贝。此构造函数是 `explicit` 的（参见**7.5.4节**）
| `strm.str()` | 返回 `strm` 保存的 `string` 的拷贝
| `strm.str(s)` | 将 `string s` 拷贝到 `strm` 中。返回 `void`

## 3.1 使用 `istringstream`
如果我们的某些工作是对整行文本进行处理，而其他一些工作是处理行内的单个单词时，通常可以使用 `istringstream` 。

一个例子是，假设有一个文件，列出了一些人和他们的电话号码。文件中每条记录都以一个人名开始，后面跟随一个或多个电话号码：
```cpp
morgan 2015552368 8625550123
drew 9735550130
lee 6095550132 2015550175 8005550000
```
首先定义一个简单的类 `PersonInfo` 来描述输入数据，其中一个成员 `name` 表示人名，`vector` 保存此人的所有电话号码：
```cpp
// 成员默认公有（参见7.2节）
struct PersonInfo {
	string name;
	vector<string> phones;
};
```
程序会读取数据文件，并创建一个 `PersonInfo` 的 `vector` ，`vector` 中每个元素对应文件中的一条记录。我们在一个循环中处理输入数据，每个循环步用 `getline` 从标准输入读取一条记录，如果 `getline` 调用成功，则 `line` 中保存着从输入文件读来的一条记录。

接下来定义了一个局部 `PersonInfo` 对象，保存当前记录中的数据，并将一个 `istringstream` 与刚刚读取的文本行进行绑定，这样就可以在此 `istringstream` 上使用 `>>` 运算符来读取当前记录中的每个元素。首先读取人名，然后用一个 while 循环读取此人的若干电话号码。读取完 `line` 中所有数据后，内层 while 循环就结束了。此循环的工作方式与前面章节中读取 `cin` 的循环很相似，不同的是此循环从一个 `string` 而非标准输入读取数据。当 `string` 中的数据全部读出后，同样会触发文件结束信号，在 `record` 上的下一个输入操作会失败。之后，将刚刚处理好的 `PersonInfo` 追加到 `vector` 中，外层 while 循环的一个循环步就随之结束了。外层 while 循环会持续执行，直到遇到 `cin` 的文件结束标识。
```cpp
string line, word;		    // 分别保存来自输入的一行和单词
vector<PersonInfo> people;  // 保存来自输入的所有记录
// 逐行从输入读取数据，直至cin遇到文件尾（或其他错误）
while (getline(cin, line)) {
	PersonInfo info;	    // 创建一个保存此记录数据的对象
	istringstream record(line); // 将记录绑定到刚读入的行
	record >> info.name;    // 读取名字
	while (record >> word)  // 读取电话号码
		info.phones.push_back(word); // 保持他们
	people.push_back(info); // 将此记录追加到people末尾
}
```
## :star:8.3.1节练习
练习8.9：使用你为8.1.2节第一个练习所编写的函数，打印一个 `istringstream` 对象的内容。

练习8.10：编写程序，将来自一个文件中的行保存在一个 `vector<string>` 中，然后使用一个 `istringstream` 从 `vector` 读取数据元素，每次读取一个单词。

练习8.11：本节的程序在外层 while 循环中定义了 `istringstream` 对象。如果 `record` 对象定义在循环之外，你需要对程序进行怎样的修改？重写程序，将 `record` 的定义移到 while 循环之外，验证你设想的修改方法是否正确。

练习8.12：我们为什么没有在 `PersonInfo` 中使用类初始化？

---
## 3.2 使用 `ostringstream`
当逐步构造输出、希望最后一步打印时，`ostringstream` 是很有用的。对上一节的例子，我们可能想逐个验证电话号码并改变其格式——如果所有号码都是有效的，希望输出到一个新的文件，包括改变格式后的号码。对于那些无效的号码，我们不会将它们输出到新文件中，而是打印一条包含人名和无效号码的错误消息。

由于不希望输出有无效号码的人，因此对每个人，直到验证完所有电话号码后才可以进行输出操作。但是可以先将输出内容写入到一个内存 `ostringstream` 中。此程序还假定已有两个函数 `valid` 和 `format` ，分别完成电话号码验证和改变格式的功能：
```cpp
for (const auto &entry : people) { // 对people中每一项
	ostringstream formatted, badNums; // 每个循环步创建的对象
	for (const auto &nums : entry.phones) { // 对每个数
		if (!valid(nums))
			badNums << " " << nums; // 将数的字符串形式存入badNums
		else	// 将格式化的字符串写入formatted
			formatted << " " << format(nums);
	}
	if (badNums.str().empty())	 		// 没有错误的数
		os << entry.name << " "			// 打印名字
		   << formatted.str() << endl;  // 和格式化的数
	else // 否则，打印名字和错误的数
		cerr << "input error: " << entry.name
			 << " invalid number(s) " << badNums.str() << endl;
}
```
有趣之处在于对字符串流 `formatted, badNums` 的使用。我们用标准的输出运算符 `<<` 向这些对象写入数据，但这些写入操作实际上转换为 `string` 操作，分别向 `formatted` 和 `badNums` 中的 `string` 对象添加字符。

## :star:8.3.2节练习
练习8.13：重写本节的电话号码程序，从一个命名文件而非 `cin` 读取数据。

练习8.14：我们为什么将 `entry` 和 `nums` 定义为 `const auto&` ？
