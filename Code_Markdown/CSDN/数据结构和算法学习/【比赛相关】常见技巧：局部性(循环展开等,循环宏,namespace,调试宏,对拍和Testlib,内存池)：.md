author: H-J-Granger, Ir1d, ChungZH, Marcythm, StudyingFather, billchenchina, Suyun514, Psycho7, greyqz, Xeonacid, partychicken

本页面主要列举一些竞赛中的小技巧。

## 利用局部性

局部性是指程序倾向于引用邻近于其他最近引用过的数据项的数据项，或者最近引用过的数据项本身。局部性分为时间局部性和空间局部性。

具体可参见 [循环展开 (Loop Unroll)](../lang/optimizations.md#循环展开-loop-unroll)、[代码布局优化 (Code Layout Optimizations)](../lang/optimizations.md#代码布局优化-code-layout-optimizations) 等内容

## 循环宏定义

如下代码可使用宏定义简化：

```cpp
for (int i = 0; i < N; i++) {
  // 循环内容略
}

// 使用宏简化
#define f(x, y, z) for (int x = (y), __ = (z); x < __; ++x)

// 这样写循环代码时，就可以简化成 `f(i, 0, N)` 。例如：
// a is a STL container
f(i, 0, a.size()) { ... }
```

另外推荐一个比较有用的宏定义：

```cpp
#define _rep(i, a, b) for (int i = (a); i <= (b); ++i)
```

## 善用 namespace

使用 namespace 能使程序可读性更好，便于调试。

??? note "例题：NOI 2018 屠龙勇士"
    ```cpp
    // NOI 2018 屠龙勇士 40分部分分代码
    #include <algorithm>
    #include <cmath>
    #include <cstring>
    #include <iostream>
    using namespace std;
    long long n, m, a[100005], p[100005], aw[100005], atk[100005];
    
    namespace one_game {
    // 其实namespace里也可以声明变量
    void solve() {
      for (int y = 0;; y++)
        if ((a[1] + p[1] * y) % atk[1] == 0) {
          cout << (a[1] + p[1] * y) / atk[1] << endl;
          return;
        }
    }
    }  // namespace one_game
    
    namespace p_1 {
    void solve() {
      if (atk[1] == 1) {  // solve 1-2
        sort(a + 1, a + n + 1);
        cout << a[n] << endl;
        return;
      } else if (m == 1) {  // solve 3-4
        long long k = atk[1], kt = ceil(a[1] * 1.0 / k);
        for (int i = 2; i <= n; i++)
          k = aw[i - 1], kt = max(kt, (long long)ceil(a[i] * 1.0 / k));
        cout << k << endl;
      }
    }
    }  // namespace p_1
    
    int main() {
      int T;
      cin >> T;
      while (T--) {
        memset(a, 0, sizeof(a));
        memset(p, 0, sizeof(p));
        memset(aw, 0, sizeof(aw));
        memset(atk, 0, sizeof(atk));
        cin >> n >> m;
        for (int i = 1; i <= n; i++) cin >> a[i];
        for (int i = 1; i <= n; i++) cin >> p[i];
        for (int i = 1; i <= n; i++) cin >> aw[i];
        for (int i = 1; i <= m; i++) cin >> atk[i];
        if (n == 1 && m == 1)
          one_game::solve();  // solve 8-13
        else if (p[1] == 1)
          p_1::solve();  // solve 1-4 or 14-15
        else
          cout << -1 << endl;
      }
      return 0;
    }
    ```

## 使用宏进行调试

编程者在本地测试的时候，往往要加入一些调试语句。而在需要提交到 OJ 时，为了不使调试语句的输出影响到系统对程序输出结果的判断，就要把它们全部删除，耗时较多。这种情况下，可以通过定义宏的方式来节省时间。大致的程序框架是这样的：

```cpp
#define DEBUG
#ifdef DEBUG
// do something when DEBUG is defined
#endif
// or
#ifndef DEBUG
// do something when DEBUG isn't defined
#endif
```

`#ifdef` 会检查程序中是否有 `#define` 定义的对应标识符，如果有定义，就会执行后面的语句。而 `#ifndef` 会在没有定义相应标识符的情况下执行后面的语句。

这样，只需在 `#ifdef DEBUG` 里写好调试用代码，`#ifndef DEBUG` 里写好真正提交的代码，就能方便地进行本地测试。提交程序的时候，只需要将 `#define DEBUG` 一行注释掉即可。也可以不在程序中定义标识符，而是通过 `-DDEBUG` 的编译选项在编译的时候定义 `DEBUG` 标识符。这样就可以在提交的时候不用修改程序了。

不少 OJ 都开启了 `-DONLINE_JUDGE` 这一编译选项，善用这一特性可以节约不少时间。

## 对拍

对拍是一种进行检验或调试的方法，通过对比两个程序的输出来检验程序的正确性。可以将自己程序的输出与其他程序的输出进行对比，从而判断自己的程序是否正确。

对拍过程要多次进行，因此需要通过批处理的方法来实现对拍的自动化。

具体而言，对拍需要一个 [数据生成器](../tools/testlib/generator.md) 和两个要进行输出结果比对的程序。

每运行一次数据生成器都将生成的数据写入输入文件，通过重定向的方法使两个程序读入数据，并将输出写入指定文件，最后利用 Windows 下的 `fc` 命令比对文件（Linux 下为 `diff` 命令）来检验程序的正确性。如果发现程序出错，可以直接利用刚刚生成的数据进行调试。

对拍程序的大致框架如下：

```cpp
#include <stdio.h>
#include <stdlib.h>

int main() {
  // For Windows
  // 对拍时不开文件输入输出
  // 当然，这段程序也可以改写成批处理的形式
  while (true) {
    system("gen > test.in");  // 数据生成器将生成数据写入输入文件
    system("test1.exe < test.in > a.out");  // 获取程序1输出
    system("test2.exe < test.in > b.out");  // 获取程序2输出
    if (system("fc a.out b.out")) {
      // 该行语句比对输入输出
      // fc返回0时表示输出一致，否则表示有不同处
      system("pause");  // 方便查看不同处
      return 0;
      // 该输入数据已经存放在test.in文件中，可以直接利用进行调试
    }
  }
}
```

## 内存池

当动态分配内存时，频繁使用 `new`/`malloc` 会占用大量的时间和空间，甚至生成大量的内存碎片从而降低程序的性能，可能会使原本正确的程序 TLE/MLE。

这时候需要使用到「内存池」这种技巧：在真正使用内存之前，先申请分配一定大小的内存作为备用。当需要动态分配时直接从备用内存中分配一块即可。

在大多数 OI 题当中，可以预先算出需要使用到的最大内存并一次性申请分配。

示例：

```cpp
// 申请动态分配 32 位有符号整数数组：
int* newarr(int sz) {
  static int pool[maxn], *allocp = pool;
  return allocp += sz, allocp - sz;
}

// 线段树动态开点的代码：
Node* newnode() {
  static Node pool[maxn << 1], *allocp = pool - 1;
  return ++allocp;
}
```

## 参考资料

[洛谷日报 #86](https://studyingfather.blog.luogu.org/some-coding-tips-for-oiers)

《算法竞赛入门经典 习题与解答》

---
# 1. 什么是对拍、何时使用对拍
算法竞赛中，特别是在OI赛制下，我们的程序能通过样例，是否意味着就一定能AC呢？多数情况下都是行不通的，除非你是超级dalao。所以就需要自己设计一些数据来测试程序。一般而言，竞赛选手几乎是无师自通地使用这种做法——我们总是可以手工计算几组小数据和边界数据，用来检验程序的正确性。

但是就像小农经济无法与大工厂流水线竞争一样——有的题目数据范围很大，我们手工计算又非常费时费力；有的程序输出众多，肉眼检验程序计算的结果是否正确相当麻烦……在紧张的比赛中，我们该如何应对呢？所以有了对拍。

**对拍**，简单来说，就是写完一道题目的正解程序（如 `solution.cpp`）后，再写一个用暴力方法求解该题目的程序（如 `brute.cpp` ），然后写一个随机数据生成程序（如 `gendata.cpp` ）用来生成测试数据，最后写一个对比程序（如 `compare.bat`）来编译运行正解和暴力解法、输入同样的数据、对比输出数据、报告差异之处。如果输出结果不同，就意味着程序有问题，需要修改程序！用了这么几倍的精力，就是为了提高通过率，这就是对拍。

注意，使用对拍进行测试**当且仅当**你能够做出题目时，即能给出题目的暴力解法和正确解法。如果题都看不懂、看懂了但码力不够写不出来（比如大模拟）、看懂了但不知道要用的算法、看懂了但不知道正确解法……凡此种种，都用不到对拍！只有你知道题目的暴力解法和正确解法，此时才需要用到对拍来检查。

---
# 2. 简单程序对拍示例
输入 $n$ 个整数，现在有 $m$ 个形如 $[x,y]$ 的提问，即问第 $x$ 个数到第 $y$ 个数之和是多少？现在需要你写一程序对每个提问做出快速回答，$1\le n\le 100000, 1\le m\le 50000$ 。
- 输入格式： 第一行，两个整数 $n$ 和 $m$ 。第二行，$n$ 个空格间隔的整数，每个整数的范围在 $[-10000,10000]$ 之间。接下来 $m$ 行，每行两个整数 $x$ 和 $y$ ，表示一次询问的区间（$x\le y$）
- 输出格式： $m$ 行，每行一个整数，对应一次提问的答案
- 输入样例： 
	```cpp
	5 3 
	1 3 2 7 9
	1 2 
	2 5 
	3 4
	```
- 输出样例：
	```cpp
	4
	21
	9
	```

## 2.1 书写正确解法
第一步。此题显然用到了前缀和，于是先写正解程序 `solution.cpp` 。避免浪费太多时间在先写暴力解法、测试数据生成程序、对拍程序，导致没有写完正解，这就很搞笑了。
```cpp
#define LOCAL_TEST
#include <bits/stdc++.h>
using namespace std;
int n, m, temp, x, y, sum[100005];

int main() {
#ifdef LOCAL_TEST
	freopen("data.in", "r", stdin); 	   //从文件data.in中读入数据
	freopen("solution.out", "w", stdout);  //写入到solution.out文件中
#endif
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &temp);
		sum[i + 1] = sum[i] + temp;
	}
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &x, &y);
		printf("%d\n", sum[y] - sum[x - 1]);
	}
	return 0;
}
```
## 2.2 书写暴力解法
第二步。这个程序是否正确呢？我们再写一个暴力程序 `brute.cpp` 来验证它，这里写的是暴力双重循环。我们不在乎暴力程序的效率，只需要保证它的结果正确就行了。
```cpp
#include <bits/stdc++.h>
using namespace std;
int n, m, x, y, a[100005];

int main() {
	freopen("data.in", "r", stdin); 	//从文件data.in中读入数据
	freopen("brute.out", "w", stdout);  //写入到brute.out文件中
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &x, &y);
		int ans = 0;
		for (int j = x; j <= y; ++j) ans += a[j];
		printf("%d\n", ans);
	}
	return 0;
}
```
## 2.3 书写随机数据生成程序
第三步。现在还需要一个生成随机数据的程序 `gendata.cpp` ，它将按照题目给定的格式生成随机数据。一般来说，过时的C++程序会选择 `<ctime>` 的 `time(NULL)` 函数作为随机种子传到 `srand()` 函数中、使用 `cstdlib` 库中的 `rand()` 函数生成随机数据。但是我们不建议这样做，`rand()` 只能生成 `[0, 32767]` 之间的随机整数，使用起来相当不便。是时候拥抱现代C++了！

```cpp
#include <bits/stdc++.h>
using namespace std;
unsigned seed = chrono::system_clock::now().time_since_epoch().count(); //随机数种子
default_random_engine gen(seed);
uniform_int_distribution<int> ud;

int main() {
	freopen("data.in", "w", stdout);	//生成的随机数据写入到data.in文件中
	ud.param(uniform_int_distribution<>::param_type {1, 100000});
	int n = ud(gen);
	ud.param(uniform_int_distribution<>::param_type {1, 50000});
	int m = ud(gen);
	printf("%d %d\n", n, m); 			//写入随机整数n,m到data.in文件中
	ud.param(uniform_int_distribution<>::param_type {-10000, 10000});
	for (int i = 0; i < n; ++i) 		//生成n个[-10000,10000]间的数字
		printf(" %d" + !i, ud(gen));
	printf("\n");
	ud.param(uniform_int_distribution<>::param_type {1, n});
	for (int i = 0; i < m; ++i) {		//生成m个x y,保证1<=x<=y<=n
		int x = ud(gen);
		int y = ud(gen);
		if (y < x) swap(x, y);
		printf("%d %d\n", x, y);
	}
	return 0;
}
```
## 2.4 书写对拍程序
第四步。写Windows上的对拍文件 `compare.bat` ：
```handlebars
@echo off		//关闭回显
:loop			//执行循环
gendata.exe		//调用gendata.exe,生成随机数据文件data.in
solution.exe	//调用solution.exe,读入data.in中的数据并输出结果到solution.out
brute.exe		//调用brute.exe,读入data.in中的数据并输出结果到brute.out
fc solution.out brute.out //对比两个结果文件的内容
if not errorlevel 1 goto loop //若内容一致,说明两个程序的结果相同,执行循环,继续对拍
pause			//若内容不一致,则对拍程序暂停,显示出错的地方
:end			
```
或者简单一点，我们采用这种写法：
```handlebars
@echo off		 
gendata.exe 
solution.exe 
brute.exe 
fc solution.out brute.out 
pause		
```
## 2.5 运行对拍程序
第5步。双击 `compare.bat` ，开始对拍。生成的测试数据：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f676806ee80a4337afb669ecbd1ebd7d.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
正解的输出：
![在这里插入图片描述](https://img-blog.csdnimg.cn/6c9b5dbe6b6b4eb297f24a8adc3b9ad7.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
暴力解法的输出：
![在这里插入图片描述](https://img-blog.csdnimg.cn/0f6cc13dddbb45a3b54645ac0d0d59c5.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
如果完全一致，对拍程序就直接结束：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c0e83bc49add49cba569591afd0db80b.png)
## 2.6 注意事项
由于对拍程序 `compare.bat` 中直接使用了可执行文件名，我们在对拍之前要注意先编译所有C++文件，并且保证它们的 `.exe` 文件出现在同一文件夹下，然后就可以愉快地开始对拍了！！！

---
# 3. 更高级的对拍技巧
学习了上一节，大部分简单题我们就可以自己写对拍了。但是还有不足：这里学的是在Windows下的对拍，如果在Linux系统中编写程序，就不太一样了；仅能生成简单测试数据，无法应对比较复杂的树、图数据生成……以后有时间，我会更新这方面的内容。

## 3.1 使用文件重定向机制
此时正确解法的代码如下： 
```cpp 
#include <bits/stdc++.h>
using namespace std;
int n, m, temp, x, y, sum[100005];

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; ++i) {
		scanf("%d", &temp);
		sum[i + 1] = sum[i] + temp;
	}
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &x, &y);
		printf("%d\n", sum[y] - sum[x - 1]);
	}
	return 0;
}
```
暴力解法的代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int n, m, x, y, a[100005];

int main() {
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= n; ++i) scanf("%d", &a[i]);
	for (int i = 0; i < m; ++i) {
		scanf("%d%d", &x, &y);
		int ans = 0;
		for (int j = x; j <= y; ++j) ans += a[j];
		printf("%d\n", ans);
	}
	return 0;
}
```
随机数据生成程序的代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
unsigned seed = chrono::system_clock::now().time_since_epoch().count(); //随机数种子
default_random_engine gen(seed);
uniform_int_distribution<int> ud;

int main() { 
	ud.param(uniform_int_distribution<>::param_type {1, 100000});
	int n = ud(gen);
	ud.param(uniform_int_distribution<>::param_type {1, 50000});
	int m = ud(gen);
	printf("%d %d\n", n, m); 			//写入随机整数n,m到data.in文件中
	ud.param(uniform_int_distribution<>::param_type {-10000, 10000});
	for (int i = 0; i < n; ++i) 		//生成n个[-10000,10000]间的数字
		printf(" %d" + !i, ud(gen));
	printf("\n");
	ud.param(uniform_int_distribution<>::param_type {1, n});
	for (int i = 0; i < m; ++i) {		//生成m个x y,保证1<=x<=y<=n
		int x = ud(gen);
		int y = ud(gen);
		if (y < x) swap(x, y);
		printf("%d %d\n", x, y);
	}
	return 0;
}
```
对拍程序如下：
```handlebars
@echo off		//关闭回显
:loop			//执行循环
//调用gendata.exe,生成随机数据文件data.in
gendata.exe	> data.in	
//调用solution.exe,读入data.in中的数据并输出结果到solution.out
solution.exe < data.in > solution.out	
//调用brute.exe,读入data.in中的数据并输出结果到brute.out
brute.exe	< data.in > brute.out 
//对比两个结果文件的内容
fc solution.out brute.out 
//若内容一致,说明两个程序的结果相同,执行循环,继续对拍
if not errorlevel 1 goto loop 
//若内容不一致,则对拍程序暂停,显示出错的地方
pause			
:end		
```
