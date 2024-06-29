### 1. Index
如果你正在使用 C++ 出一道算法竞赛题目，Testlib 是编写相关程序（generator, validator, checker, interactor）时的优秀辅助工具。它是俄罗斯和其他一些国家的出题人的必备工具，许多比赛也都在用它：ROI、ICPC 区域赛、所有 Codeforces round……

Testlib 库仅有 `testlib.h` 一个文件，使用时仅需在所编写的程序开头添加 `#include "testlib.h"` 即可。

Testlib 的具体用途：

-   编写 [Generator](./generator.md)，即数据生成器。
-   编写 [Validator](./validator.md)，即数据校验器，判断生成数据是否符合题目要求，如数据范围、格式等。
-   编写 [Interactor](./interactor.md)，即交互器，用于交互题。
-   编写 [Checker](./checker.md)，即 [Special Judge](../special-judge.md)。

Testlib 与 Codeforces 开发的 [Polygon](https://polygon.codeforces.com/) 出题平台完全兼容。

`testlib.h` 在 2005 年移植自 `testlib.pas`，并一直在更新。Testlib 与绝大多数编译器兼容，如 VC++ 和 GCC g++，并兼容 C++11。

**本文主要翻译自 [Testlib - Codeforces](https://codeforces.com/testlib)。`testlib.h` 的 GitHub 存储库为 [MikeMirzayanov/testlib](https://github.com/MikeMirzayanov/testlib)。**
### 2. 通用
本页面介绍 Testlib checker/interactor/validator 的一些通用状态/对象/函数、一些用法及注意事项。请在阅读其他页面前完整阅读本页面的内容。

## 通用状态

| 结果                 | Testlib 别名   | 含义                                                                                     |
| ------------------ | ------------ | -------------------------------------------------------------------------------------- |
| Ok                 | `_ok`        | 答案正确。                                                                                  |
| Wrong Answer       | `_wa`        | 答案错误。                                                                                  |
| Presentation Error | `_pe`        | 答案格式错误。注意包括 Codeforces 在内的许多 OJ 并不区分 PE 和 WA。                                          |
| Partially Correct  | `_pc(score)` | 答案部分正确。仅限于有部分分的测试点，其中 `score` 为一个正整数，从 $0$（没分）到 $100$（可能的最大分数）。                        |
| Fail               | `_fail`      | validator 中表示输入不合法，不通过校验。<br>checker 中表示程序内部错误、标准输出有误或选手输出比标准输出更优，需要裁判/出题人关注。（也就是题目锅了） |

通常用程序的返回值表明结果，但是也有一些其他方法：创建一个输出 xml 文件、输出信息到 stdout 或其他位置……这些都通过下方函数表中的 `quitf` 函数来完成。

## 通用对象

| 对象    | 含义    |
| ----- | ----- |
| `inf` | 输入文件流 |
| `ouf` | 选手输出流 |
| `ans` | 参考输出流 |

## 通用函数

非成员函数：

| 调用                                                                                              | 含义                                                  |
| ----------------------------------------------------------------------------------------------- | --------------------------------------------------- |
| `void registerTestlibCmd(int argc, char* argv[])`                                               | 注册程序为 checker                                       |
| `void registerInteraction(int argc, char* argv[])`                                              | 注册程序为 interactor                                    |
| `void registerValidation()`/`void registerValidation(int argc, char* argv[])`                   | 注册程序为 validator                                     |
| `void registerGen(int argc, char* argv[], int randomGeneratorVersion)`                          | 注册程序为 generator<br>`randomGeneratorVersion` 推荐为 `1` |
| `void quit(TResult verdict, string message)`/`void quitf(TResult verdict, string message, ...)` | 结束程序，返回 `verdict`，输出 `message`                      |
| `void quitif(bool condition, TResult verdict, string message, ...)`                             | 如果 `condition` 成立，调用 `quitf(verdict, message, ...)` |

流成员函数：

| 调用                                                                                                                                                                | 含义                                                                                 |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- |
| `char readChar()`                                                                                                                                                 | 读入一个字符                                                                             |
| `char readChar(char c)`                                                                                                                                           | 读入一个字符，必须为 `c`                                                                     |
| `char readSpace()`                                                                                                                                                | 等同于 `readChar(' ')`                                                                |
| `string readToken()`/`string readWord()`                                                                                                                          | 读入一个串，到空白字符（空格、Tab、EOLN 等）停止                                                       |
| `string readToken(string regex)`/`string readWord(string regex)`                                                                                                  | 读入一个串，必须与 `regex` 匹配                                                               |
| `long long readLong()`                                                                                                                                            | 读入一个 64 位整数                                                                        |
| `long long readLong(long long L, long long R)`                                                                                                                    | 读入一个 64 位整数，必须在 $[L,R]$ 之间                                                         |
| `vector<long long> readLongs(int n, long long L, long long R)`                                                                                                    | 读入 $N$ 个 64 位整数，必须均在 $[L,R]$ 之间                                                    |
| `int readInt()`/`int readInteger()`                                                                                                                               | 读入一个 32 位整数                                                                        |
| `int readInt(int L, int R)`/`int readInteger(L, R)`                                                                                                               | 读入一个 32 位整数，必须在 $[L,R]$ 之间                                                         |
| `vector<int> readInts(int n, int L, int R)`/`vector<int> readIntegers(int n, int L, int R)`                                                                       | 读入 $N$ 个 32 位整数，必须均在 $[L,R]$ 之间                                                    |
| `double readReal()`/`double readDouble()`                                                                                                                         | 读入一个双精度浮点数                                                                         |
| `double readReal(double L, double R)`/`double readDouble(double L, double R)`                                                                                     | 读入一个双精度浮点数，必须在 $[L,R]$ 之间                                                          |
| `double readStrictReal(double L, double R, int minPrecision, int maxPrecision)`/`double readStrictDouble(double L, double R, int minPrecision, int maxPrecision)` | 读入一个双精度浮点数，必须在 $[L,R]$ 之间，小数位数必须在 $[minPrecision,maxPrecision]$ 之间，不得使用指数计数法等非正常格式 |
| `string readString()`/`string readLine()`                                                                                                                         | 读入一行（包括换行符），同时将流指针指向下一行的开头                                                         |
| `string readString(string regex)`/`string readLine(string regex)`                                                                                                 | 读入一行，必须与 `regex` 匹配                                                                |
| `void readEoln()`                                                                                                                                                 | 读入 EOLN（在 Linux 环境下读入 `LF`，在 Windows 环境下读入 `CR LF`）                                |
| `void readEof()`                                                                                                                                                  | 读入 EOF                                                                             |
| `void quit(TResult verdict, string message)`/`void quitf(TResult verdict, string message, ...)`                                                                   | 结束程序，若 `Stream` 为 `ouf` 返回 `verdict`，否则返回 `_fail`；输出 `message`                     |
| `void quitif(bool condition, TResult verdict, string message, ...)`                                                                                               | 如果 `condition` 成立，调用 `quitf(verdict, message, ...)`                                |

未完待续……

## 极简正则表达式

上面的输入函数中的一部分允许使用「极简正则表达式」特性，如下所示：

-   字符集。如 `[a-z]` 表示所有小写英文字母，`[^a-z]` 表示除小写英文字母外任何字符。
-   范围。如 `[a-z]{1,5}` 表示一个长度在 $[1,5]$ 范围内且只包含小写英文字母的串。
-   「或」标识符。如 `mike|john` 表示 `mike` 或 `john` 其一。
-   「可选」标识符。如 `-?[1-9][0-9]{0,3}` 表示 $[-9999,9999]$ 范围内的非零整数（注意那个可选的负号）。
-   「重复」标识符。如 `[0-9]*` 表示零个或更多数字，`[0-9]+` 表示一个或更多数字。
-   注意这里的正则表达式是「贪婪」的（「重复」会尽可能匹配）。如 `[0-9]?1` 将不会匹配 `1`（因为 `[0-9]?` 将 `1` 匹配上，导致模板串剩余的那个 `1` 无法匹配）。

## 首先 include testlib.h

请确保 testlib.h 是你 include 的 **第一个** 头文件，Testlib 会重写/禁用（通过名字冲突的方式）一些与随机有关的函数（如 `random()`），保证随机结果与环境无关，这对于 generator 非常重要，[generator 页面](./generator.md) 会详细说明这一点。

## 使用项别名

推荐给 `readInt/readInteger/readLong/readDouble/readWord/readToken/readString/readLine` 等的有限制调用最后多传入一个 `string` 参数，即当前读入的项的别名，使报错易读。例如使用 `inf.readInt(1, 100, "n")` 而非 `inf.readInt(1, 100)`，报错信息将为 `FAIL Integer parameter [name=n] equals to 0, violates the range [1, 100]`。

## 使用 `ensuref/ensure()`

这两个函数用于检查条件是否成立（类似于 `assert()`）。例如检查 $x_i \neq y_i$，我们可以使用

```cpp
ensuref(x[i] != y[i], "Graph can't contain loops");
```

还可以使用 C 风格占位符如

```cpp
ensuref(s.length() % 2 == 0,
        "String 's' should have even length, but s.length()=%d",
        int(s.length()));
```

它有一个简化版 `ensure()`，我们可以直接使用 `ensure(x> y)` 而不添加说明内容（也不支持添加说明内容），如果条件不满足报错将为 `FAIL Condition failed: "x > y"`。很多情况下不加额外的说明的这种报错很不友好，所以我们通常使用 `ensuref()` 并加以说明内容，而非使用 `ensure()`。

???+ warning
    注意全局与成员 `ensuref/ensure()` 的区别
    
    全局函数 `::ensuref/ensure()` 多用于 generator 和 validator 中，如果检查失败将统一返回 `_fail`。
    
    成员函数 `InStream::ensuref/ensure()` 一般用于判断选手和参考程序的输出是否合法。当 `InStream` 为 `ouf` 时，返回 `_wa`；为 `inf`（一般不在 checker 中检查输入数据，这应当在 validator 中完成）或 `ans` 时，返回 `_fail`。详见 [Checker - 编写 readAns 函数](./checker.md#好的实现)。

**本文主要翻译并综合自 [Testlib - Codeforces](https://codeforces.com/testlib) 系列。`testlib.h` 的 GitHub 存储库为 [MikeMirzayanov/testlib](https://github.com/MikeMirzayanov/testlib)。**

### 3. Generator
Generator，即数据生成器。当数据很大，手造会累死的时候，我们就需要它来帮助我们自动造数据。

## 简单的例子

生成两个 $[1,n]$ 范围内的整数：

```cpp
// clang-format off

#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  registerGen(argc, argv, 1);
  int n = atoi(argv[1]);
  cout << rnd.next(1, n) << " ";
  cout << rnd.next(1, n) << endl;
}
```

## 为什么要使用 Testlib？

有人说写 generator 不需要用 Testlib，它在这没什么用。实际上这是个不正确的想法。一个好的 generator 应该满足这一点： **在任何环境下对于相同输入它给出相同输出** 。写 generator 就避免不了生成随机值，平时我们用的 `rand()` 或 C++11 的 `mt19937/uniform_int_distribution` ，当操作系统不同、使用不同编译器编译、不同时间运行等，它们的输出都可能不同（对于非常常用的 `srand(time(0))` ，这是显然的），而这就会给生成数据带来不确定性。

需要注意的是，一旦使用了 Testlib，就不能再使用标准库中的 `srand()` ， `rand()` 等随机数函数，否则在编译时会报错。因此， **请确保所有与随机相关的函数均使用 Testlib 而非标准库提供的。** 

而 Testlib 中的随机值生成函数则保证了相同调用会输出相同值，与 generator 本身或平台均无关。另外。它给生成各种要求的随机值提供了很大便利，如 `rnd.next("[a-z]{1,10}")` 会生成一个长度在 $[1,10]$ 范围内的串，每个字符为 `a` 到 `z` ，很方便吧！

## Testlib 能做什么？

在一切之前，先执行 `registerGen(argc, argv, 1)` 初始化 Testlib（其中 `1` 是使用的 generator 版本，通常保持不变），然后我们就可以使用 `rnd` 对象来生成随机值。随机数种子取自命令行参数的哈希值，对于某 generator `g.cpp` ， `g 100` (Unix-Like) 和 `g.exe "100"` (Windows) 将会有相同的输出，而 `g 100 0` 则与它们不同。

 `rnd` 对象的类型为 `random_t` ，你可以建立一个新的随机值生成对象，不过通常你不需要这么做。

该对象有许多有用的成员函数，下面是一些例子：

| 调用                                | 含义                                                                                                                                                                                                                                                      |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|  `rnd.next(4)`                    | 等概率生成一个 $[0,4)$ 范围内的整数                                                                                                                                                                                                                                  |
|  `rnd.next(4, 100)`               | 等概率生成一个 $[4,100]$ 范围内的整数                                                                                                                                                                                                                                |
|  `rnd.next(10.0)`                 | 等概率生成一个 $[0,10.0)$ 范围内的浮点数                                                                                                                                                                                                                              |
|  `rnd.next("one | two | three")`  | 等概率从 `one` , `two` , `three` 三个串中返回一个                                                                                                                                                                                                                   |
|  `rnd.wnext(4, t)`                |  `wnext()` 是一个生成不等分布（具有偏移期望）的函数， $t$ 表示调用 `next()` 的次数，并取生成值的最大值。例如 `rnd.wnext(3, 1)` 等同于 `max({rnd.next(3), rnd.next(3)})` ； `rnd.wnext(4, 2)` 等同于 `max({rnd.next(4), rnd.next(4), rnd.next(4)})` 。如果 $t<0$ ，则为调用 $-t$ 次，取最小值；如果 $t=0$ ，等同于 `next()` 。 |
|  `rnd.any(container)`             | 等概率返回一个具有随机访问迭代器（如 `std::vector` 和 `std::string` ）的容器内的某一元素的引用                                                                                                                                                                                          |

附：关于 `rnd.wnext(i,t)` 的形式化定义：

$$
\operatorname{wnext}(i,t)=\begin{cases}\operatorname{next}(i) & t=0 \\\max(\operatorname{next}(i),\operatorname{wnext}(i,t-1)) & t>0 \\\min(\operatorname{next}(i),\operatorname{wnext}(i,t+1)) & t<0\end{cases}
$$

另外，不要使用 `std::random_shuffle()` ，请使用 Testlib 中的 `shuffle()` ，它同样接受一对迭代器。它使用 `rnd` 来打乱序列，即满足如上「好的 generator」的要求。

## 示例：生成一棵树

下面是生成一棵树的主要代码，它接受两个参数——顶点数和伸展度。例如，当 $n=10,t=1000$ 时，可能会生成链；当 $n=10,t=-1000$ 时，可能会生成菊花。

```cpp
#define forn(i, n) for (int i = 0; i < int(n); i++)

registerGen(argc, argv, 1);

int n = atoi(argv[1]);
int t = atoi(argv[2]);

vector<int> p(n);

/* 为节点 1..n-1 设置父亲 */
forn(i, n) if (i > 0) p[i] = rnd.wnext(i, t);

printf("%d\n", n);

/* 打乱节点 1..n-1 */
vector<int> perm(n);
forn(i, n) perm[i] = i;
shuffle(perm.begin() + 1, perm.end());

/* 根据打乱的节点顺序加边 */
vector<pair<int, int> > edges;
for (int i = 1; i < n; i++)
  if (rnd.next(2))
    edges.push_back(make_pair(perm[i], perm[p[i]]));
  else
    edges.push_back(make_pair(perm[p[i]], perm[i]));

/* 打乱边 */
shuffle(edges.begin(), edges.end());

for (int i = 0; i + 1 < n; i++)
  printf("%d %d\n", edges[i].first + 1, edges[i].second + 1);
```

## 一次性生成多组数据

跟不使用 Testlib 编写的时候一样，每次输出前重定向输出流就好，不过 Testlib 提供了一个辅助函数 `startTest(test_index)` ，它帮助你将输出流重定向到 `test_index` 文件。

## 一些注意事项

- 严格遵循题目的格式要求，如空格和换行，注意文件的末尾应有一个换行。
- 对于大数据首选 `printf` 而非 `cout` ，以提高性能。（不建议在使用 Testlib 时关闭流同步）
- 不使用 UB（Undefined Behavior，未定义行为），如本文开头的那个示例，输出如果写成 `cout << rnd.next(1, n) << " " << rnd.next(1, n) << endl;` ，则 `rnd.next()` 的调用顺序没有定义。

## 新特性：解析命令行参数

在之前，我们通常使用类似 `int n = atoi(argv[3]);` 的代码，但是这样并不好。有以下几点原因：

- 不存在第三个命令行参数的时候是不安全的；
- 第三个命令行参数可能不是有效的 32 位整数。

现在，你可以这样写： `int n = opt<int>(3)` 。与此同时，你也可以使用 `int64_t m = opt<int64_t>(1);` ， `bool t = opt<bool>(2);` 和 `string s = opt(4);` 等。

另外，testlib 同时也支持命名参数。如果有很多参数，这样 `g 10 20000 a true` 的可读性就会比 `g -n10 -m200000 -t=a -increment` 差。

在这种情况下，现在你可以在 generator 中使用以下代码：

```cpp
int n = opt<int>("n");
long long n = opt<long long>("m");
string t = opt("t");
bool increment = opt<bool>("increment");
```

你可以自由地混合使用按下标和按名称读取参数的方式。

支持的用于编写命名参数的方案有以下几种：

-  `--key=value` 或 `-key=value` ；
-  `--key value` 或 `-key value` ——如果 `value` 不是新参数的开头（不以连字符 `-` 开头或一个/两个连字符后没有跟随字母）；
-  `--k12345` 或 `-k12345` ——如果 key `k` 是一个字母，且后面是一个数字；
-  `-prop` 或 `--prop` ——启用 bool 属性。

下面是一些例子：

```text
g1 -n1
g2 --len=4 --s=oops
g3 -inc -shuffle -n=5
g4 --length 5 --total 21 -ord
```

## 更多示例

可以在 [GitHub](https://github.com/MikeMirzayanov/testlib/tree/master/generators) 中找到。

 **本文主要翻译自 [Генераторы на testlib.h - Codeforces](https://codeforces.com/blog/entry/18291) 。新特性翻译自 [Testlib: Opts—parsing command line options](https://codeforces.com/blog/entry/72702) 。 `testlib.h` 的 GitHub 存储库为 [MikeMirzayanov/testlib](https://github.com/MikeMirzayanov/testlib) 。** 

### 4. Validator
前置知识：[通用](./general.md)

本页面将简要介绍 validator 的概念与用法。

## 概述

Validator（中文：校验器）用于检验造好的数据的合法性。当造好一道题的数据，又担心数据不合法（不符合题目的限制条件：上溢、图不连通、不是树……）时，出题者通常会借助 validator 来检查。[^ref1]

因为 Coderforces 支持 hack 功能，所以所有 Codeforces 上的题目都必须要有 validator。UOJ 也如此。[Polygon](../polygon.md) 内建了对 validator 的支持。

## 使用方法

直接在命令行输入 `./val` 即可。数据通过 stdin 输入。如果想从文件输入可 `./val < a.in`。

若数据没有问题，则什么都不会输出且返回 0；否则会输出错误信息并返回一个非 0 值。

## 提示

-   写 validator 时，不能对被 validate 的数据做任何假设，因为它可能包含任何内容。因此，出题者要对各种不合法的情况进行判断（使用 Testlib 会大大简化这一流程）。
    -   例如，输入一个点数为 $n$ 的树，主要工作是判断 $n$ 是否符合范围和判断输入的是树与否。但是切不可在判断过 $n$ 范围之后就不对接下来输入的边的起点与终点的范围进行判断，否则可能会导致 validator RE。
    -   即使不会 RE 也不应该不判断，这会导致你的报错不正确。如上例，如果不判断，报错可能会是「不是一棵树」，但是正确的报错应当是「边起点/终点不在 $[1,n]$ 之间」。
-   不能对选手的读入方式做任何假设。因此，必须保证能通过 validate 的数据完全符合输入格式。
    -   例如，选手可能逐字符地读入数字，在数字与数字之间只读入一个空格。所以在编写 validator 时，数据中的每一个空白字符都要在 validator 中显式地读入（如空格和换行）。
-   结束时不要忘记 `inf.readEof()`。
-   如果题目开放 hack（或者说，validator 的错误信息会给别人看），请使报错信息尽量友好。
    -   读入变量时使用「项别名」。
    -   在判断使用的表达式不那么易懂时，使用 ensuref 而非 ensure。

## 示例

以下是 [CF Gym 100541A - Stock Market](https://codeforces.com/gym/100541/problem/A) 的 validator：

```cpp
#include "testlib.h"

int main(int argc, char* argv[]) {
  registerValidation(argc, argv);
  int testCount = inf.readInt(1, 10, "testCount");
  inf.readEoln();

  for (int i = 0; i < testCount; i++) {
    int n = inf.readInt(1, 100, "n");
    inf.readSpace();
    inf.readInt(1, 1000000, "w");
    inf.readEoln();

    for (int i = 0; i < n; ++i) {
      inf.readInt(1, 1000, "p_i");
      if (i < n - 1) inf.readSpace();
    }
    inf.readEoln();
  }

  inf.readEof();
}
```

## 外部链接

-   [Validator 的更多示例](https://github.com/MikeMirzayanov/testlib/tree/master/validators)
-   [`testlib.h` 的 GitHub 存储库 MikeMirzayanov/testlib](https://github.com/MikeMirzayanov/testlib)

## 参考资料与注释

[^ref1]: [Validators with testlib.h - Codeforces](https://codeforces.com/blog/entry/18426)

### 5. Interactor
Interactor，即交互器，用于交互题与选手程序交互。交互题的介绍见 [题型介绍 - 交互题](../../contest/problems.md#)。

???+ note
    Testlib 仅支持 Codeforces 形式交互题，即两程序交互。不支持 NOI 形式的选手编写函数与其他函数交互。

请在阅读下文前先阅读 [通用](./general.md)。

Testlib 为 interactor 提供了一个特殊的流 `std::fstream tout`，它是一个 log 流，你可以在 interactor 中向它写入，并在 checker 中用 `ouf` 读取。

在 interactor 中，我们从 `inf` 读取题目测试数据，将选手程序（和标程）的标准输入写入 `stdout`（在线），从 `ouf` 读选手输出（在线），从 `ans` 读标准输出（在线）。

如果 interactor 返回了 ok 状态，checker（如果有的话）将接管工作，检查答案合法性。

## 用法

Windows:

```bat
interactor.exe <Input_File> <Output_File> [<Answer_File> [<Result_File> [-appes]]],
```

Linux:

```bash
./interactor.out <Input_File> <Output_File> [<Answer_File> [<Result_File> [-appes]]],
```

## 简单的例子

???+ note 题目
    interactor 随机选择一个 $[1,10^9]$ 范围内的整数，你要写一个程序来猜它，你最多可以询问 $50$ 次一个 $[1,10^9]$ 范围内的整数。
    
    interactor 将返回：
    
    `1`：询问与答案相同，你的程序应当停止询问。
    
    `0`：询问比答案小。
    
    `2`：询问比答案大。

注意在此题中我们不需要 `ans`，因为我们不需要将标准输出与其比较；而在其他题中可能需要这么做。

```cpp
int main(int argc, char** argv) {
  registerInteraction(argc, argv);
  int n = inf.readInt();  // 选数
  cout.flush();           // 刷新缓冲区
  int left = 50;
  bool found = false;
  while (left > 0 && !found) {
    left--;
    int a = ouf.readInt(1, 1000000000);  // 询问
    if (a < n)
      cout << 0 << endl;
    else if (a > n)
      cout << 2 << endl;
    else
      cout << 1 << endl, found = true;
    cout.flush();
  }
  if (!found) quitf(_wa, "couldn't guess the number with 50 questions");
  ouf.readEof();
  quitf(_ok, "guessed the number with %d questions!", 50 - left);
}
```

**本文主要翻译自 [Interactors with testlib.h - Codeforces](https://codeforces.com/blog/entry/18455)。`testlib.h` 的 GitHub 存储库为 [MikeMirzayanov/testlib](https://github.com/MikeMirzayanov/testlib)。**


### 6. Checker
Checker，即 [Special Judge](../special-judge.md)，用于检验答案是否合法。使用 Testlib 可以让我们免去检验许多东西，使编写简单许多。

Checker 从命令行参数读取到输入文件名、选手输出文件名、标准输出文件名，并确定选手输出是否正确，并返回一个预定义的结果：

请在阅读下文前先阅读 [通用](./general.md)。

## 简单的例子

???+ note 题目
    给定两个整数 $a,b$（$-1000 \le a,b \le 1000$），输出它们的和。

这题显然不需要 checker 对吧，但是如果一定要的话也可以写一个：

```cpp
#include "testlib.h"

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);

  int pans = ouf.readInt(-2000, 2000, "sum of numbers");

  // 假定标准输出是正确的，不检查其范围
  // 之后我们会看到这并不合理
  int jans = ans.readInt();

  if (pans == jans)
    quitf(_ok, "The sum is correct.");
  else
    quitf(_wa, "The sum is wrong: expected = %d, found = %d", jans, pans);
}
```

## 编写 readAns 函数

假设你有一道题输入输出均有很多数，如：给定一张 DAG，求 $s$ 到 $t$ 的最长路并输出路径（可能有多条，输出任一）。

下面是一个 **不好** 的 checker 的例子。

### 不好的实现

```cpp
// clang-format off

#include "testlib.h"
#include <map>
#include <vector>
using namespace std;

map<pair<int, int>, int> edges;

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);
  int n = inf.readInt();  // 不需要 readSpace() 或 readEoln()
  int m = inf.readInt();  // 因为不需要在 checker 中检查标准输入合法性
                          //（有 validator）
  for (int i = 0; i < m; i++) {
    int a = inf.readInt();
    int b = inf.readInt();
    int w = inf.readInt();
    edges[make_pair(a, b)] = edges[make_pair(b, a)] = w;
  }
  int s = inf.readInt();
  int t = inf.readInt();

  // 读入标准输出
  int jvalue = 0;
  vector<int> jpath;
  int jlen = ans.readInt();
  for (int i = 0; i < jlen; i++) {
    jpath.push_back(ans.readInt());
  }
  for (int i = 0; i < jlen - 1; i++) {
    jvalue += edges[make_pair(jpath[i], jpath[i + 1])];
  }

  // 读入选手输出
  int pvalue = 0;
  vector<int> ppath;
  vector<bool> used(n, false);
  int plen = ouf.readInt(2, n, "number of vertices");  // 至少包含 s 和 t 两个点
  for (int i = 0; i < plen; i++) {
    int v = ouf.readInt(1, n, format("path[%d]", i + 1).c_str());
    if (used[v - 1])  // 检查每条边是否只用到一次
      quitf(_wa, "vertex %d was used twice", v);
    used[v - 1] = true;
    ppath.push_back(v);
  }
  // 检查起点终点合法性
  if (ppath.front() != s)
    quitf(_wa, "path doesn't start in s: expected s = %d, found %d", s,
          ppath.front());
  if (ppath.back() != t)
    quitf(_wa, "path doesn't finish in t: expected t = %d, found %d", t,
          ppath.back());
  // 检查相邻点间是否有边
  for (int i = 0; i < plen - 1; i++) {
    if (edges.find(make_pair(ppath[i], ppath[i + 1])) == edges.end())
      quitf(_wa, "there is no edge (%d, %d) in the graph", ppath[i],
            ppath[i + 1]);
    pvalue += edges[make_pair(ppath[i], ppath[i + 1])];
  }

  if (jvalue != pvalue)
    quitf(_wa, "jury has answer %d, participant has answer %d", jvalue, pvalue);
  else
    quitf(_ok, "answer = %d", pvalue);
}
```

这个 checker 主要有两个问题：

1.  它确信标准输出是正确的。如果选手输出比标准输出更优，它会被判成 WA，这不太妙。同时，如果标准输出不合法，也会产生 WA。对于这两种情况，正确的操作都是返回 Fail 状态。
2.  读入标准输出和选手输出的代码是重复的。在这道题中写两遍读入问题不大，只需要一个 `for` 循环；但是如果有一道题输出很复杂，就会导致你的 checker 结构混乱。重复代码会大大降低可维护性，让你在 debug 或修改格式时变得困难。

读入标准输出和选手输出的方式实际上是完全相同的，这就是我们通常编写一个用流作为参数的读入函数的原因。

### 好的实现

```cpp
// clang-format off

#include "testlib.h"
#include <map>
#include <vector>
using namespace std;

map<pair<int, int>, int> edges;
int n, m, s, t;

// 这个函数接受一个流，从其中读入
// 检查路径的合法性并返回路径长度
// 当 stream 为 ans 时，所有 stream.quitf(_wa, ...)
// 和失败的 readXxx() 均会返回 _fail 而非 _wa
// 也就是说，如果输出非法，对于选手输出流它将返回 _wa，
// 对于标准输出流它将返回 _fail
int readAns(InStream& stream) {
  // 读入输出
  int value = 0;
  vector<int> path;
  vector<bool> used(n, false);
  int len = stream.readInt(2, n, "number of vertices");
  for (int i = 0; i < len; i++) {
    int v = stream.readInt(1, n, format("path[%d]", i + 1).c_str());
    if (used[v - 1]) {
      stream.quitf(_wa, "vertex %d was used twice", v);
    }
    used[v - 1] = true;
    path.push_back(v);
  }
  if (path.front() != s)
    stream.quitf(_wa, "path doesn't start in s: expected s = %d, found %d", s,
                 path.front());
  if (path.back() != t)
    stream.quitf(_wa, "path doesn't finish in t: expected t = %d, found %d", t,
                 path.back());
  for (int i = 0; i < len - 1; i++) {
    if (edges.find(make_pair(path[i], path[i + 1])) == edges.end())
      stream.quitf(_wa, "there is no edge (%d, %d) in the graph", path[i],
                   path[i + 1]);
    value += edges[make_pair(path[i], path[i + 1])];
  }
  return value;
}

int main(int argc, char* argv[]) {
  registerTestlibCmd(argc, argv);
  n = inf.readInt();
  m = inf.readInt();
  for (int i = 0; i < m; i++) {
    int a = inf.readInt();
    int b = inf.readInt();
    int w = inf.readInt();
    edges[make_pair(a, b)] = edges[make_pair(b, a)] = w;
  }
  int s = inf.readInt();
  int t = inf.readInt();

  int jans = readAns(ans);
  int pans = readAns(ouf);
  if (jans > pans)
    quitf(_wa, "jury has the better answer: jans = %d, pans = %d\n", jans,
          pans);
  else if (jans == pans)
    quitf(_ok, "answer = %d\n", pans);
  else  // (jans < pans)
    quitf(_fail, ":( participant has the better answer: jans = %d, pans = %d\n",
          jans, pans);
}
```

注意到这种写法我们同时也检查了标准输出是否合法，这样写 checker 让程序更短，且易于理解和 debug。此种写法也适用于输出 YES（并输出方案什么的），或 NO 的题目。

???+ note
    对于某些限制的检查可以用 `InStream::ensure/ensuref()` 函数更简洁地实现。如上例第 23 至 25 行也可以等价地写成如下形式：
    
    ```cpp
    stream.ensuref(!used[v - 1], "vertex %d was used twice", v);
    ```

???+ warning
    请在 `readAns` 中避免调用 **全局** 函数 `::ensure/ensuref()`，这会导致在某些应判为 WA 的选手输出下返回 `_fail`，产生错误。

## 建议与常见错误

-   编写 `readAns` 函数，它真的可以让你的 checker 变得很棒。

-   读入选手输出时永远限定好范围，如果某些变量忘记了限定且被用于某些参数，你的 checker 可能会判定错误或 RE 等。

    -   反面教材

    ```cpp
    // ....
    int k = ouf.readInt();
    vector<int> lst;
    for (int i = 0; i < k; i++)  // k = 0 和 k = -5 在这里作用相同（不会进入循环体）
      lst.push_back(ouf.readInt());
    // 但是我们并不想接受一个长度为 -5 的 list，不是吗？
    // ....
    int pos = ouf.readInt();
    int x = A[pos];
    // 可能会有人输出 -42, 2147483456 或其他一些非法数字导致 checker RE
    ```

    -   正面教材

    ```cpp
    // ....
    int k = ouf.readInt(0, n);  // 长度不合法会立刻判 WA 而不会继续 check 导致 RE
    vector<int> lst;
    for (int i = 0; i < k; i++) lst.push_back(ouf.readInt());
    // ....
    int pos = ouf.readInt(0, (int)A.size() - 1);  // 防止 out of range
    int x = A[pos];
    // ....
    ```

-   使用项别名。

-   和 validator 不同，checker 不用特意检查非空字符。例如对于一个按顺序比较整数的 checker，我们只需判断选手输出的整数和答案整数是否对应相等，而选手是每行输出一个整数，还是在一行中输出所有整数等格式问题，我们的 checker 不必关心。

## 使用方法

通常我们不需要本地运行它，评测工具/OJ 会帮我们做好这一切。但是如果需要的话，以以下格式在命令行运行：

```bash
./checker <input-file> <output-file> <answer-file> [<report-file> [<-appes>]]
```

## 一些预设的 checker

很多时候我们的 checker 完成的工作很简单（如判断输出的整数是否正确，输出的浮点数是否满足精度要求），[Testlib](https://github.com/MikeMirzayanov/testlib/tree/master/checkers) 已经为我们给出了这些 checker 的实现，我们可以直接使用。

一些常用的 checker 有：

-   ncmp：按顺序比较 64 位整数。
-   rcmp4：按顺序比较浮点数，最大可接受误差（绝对误差或相对误差）不超过 $10^{-4}$（还有 rcmp6，rcmp9 等对精度要求不同的 checker，用法和 rcmp4 类似）。
-   wcmp：按顺序比较字符串（不带空格，换行符等非空字符）。
-   yesno：比较 YES 和 NO，大小写不敏感。

    **本文主要翻译自 [Checkers with testlib.h - Codeforces](https://codeforces.com/blog/entry/18431)。`testlib.h` 的 GitHub 存储库为 [MikeMirzayanov/testlib](https://github.com/MikeMirzayanov/testlib)。**
