@[toc]

---
# L1-001 Hello World (5 分)

这道超级简单的题目没有任何输入。

你只需要在一行中输出著名短句“Hello World!”就可以了。

**输入样例：**
无

**输出样例：** 
```go
Hello World!
```
代码如下：

```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
    printf("Hello World!");
    return 0;
}
```
---
# L1-002 打印沙漏 (20 分)

本题要求你写个程序把给定的符号打印成沙漏的形状。例如给定17个“*”，要求按下列格式打印：
```cpp
*****
 ***
  *
 ***
*****
```

所谓“沙漏形状”，是指每行输出奇数个符号；各行符号中心对齐；相邻两行符号数差2；符号数先从大到小顺序递减到1，再从小到大顺序递增；首尾符号数相等。

给定任意N个符号，不一定能正好组成一个沙漏。要求打印出的沙漏能用掉尽可能多的符号。

**输入格式:** 
输入在一行给出1个正整数N（$≤1000$）和一个符号，中间以空格分隔。

**输出格式:**
首先打印出由给定符号组成的最大的沙漏形状，最后在一行中输出剩下没用掉的符号数。

**输入样例:**

```cpp
19 *
```

**输出样例:**

```cpp
*****
 ***
  *
 ***
*****
2
```
代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    char c;
    scanf("%d %c", &n, &c);
    int k = sqrt((n + 1) / 2), t = 2 * k - 1; //从最高层到只有1个符号的层数,上半部分 //t为最高层字符数
    for (int i = 1; i <= k; ++i) {
        for (int j = 1; j < i; ++j) printf(" ");
        for (int j = t; j > 0; --j) printf("%c", c);
        printf("\n");
        t -= 2;
    }
    t = 1;
    for (int i = 2; i <= k; ++i) {
        for (int j = k - i; j > 0; --j) printf(" ");
        t += 2;
        for (int j = t; j > 0; --j) printf("%c", c);
        printf("\n");
    }
    printf("%d\n", n - 2 * k * k + 1);
    return 0;
}
```
---
# L1-003 个位数统计 (15 分)

给定一个 $k$ 位整数 $N=d_{k-1}10^{k-1} + \dots + d_110^1+d_0$ $(0\le d_i\le 9, i = 0,\dots,k-1,d_{k-1}>0)$  ，请编写程序统计每种不同的个位数字出现的次数。例如：给定 $N=100311$ ，则有 $2$ 个 $0$ ，$3$ 个 $1$ ，和 $1$ 个 $3$ 。

**输入格式：** 
每个输入包含 1 个测试用例，即一个不超过 1000 位的正整数 $N$ 。

**输出格式：** 
对 $N$ 中每一种不同的个位数字，以 `D:M` 的格式在一行中输出该位数字 `D` 及其在 `N` 中出现的次数 `M` 。要求按 `D` 的升序输出。

**输入样例：** 
```cpp
100311
```
**输出样例：**

```cpp
0:2
1:3
3:1
```
代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {    
    int cnt[10] = {0};
    char c;
    while (~scanf("%c", &c)) ++cnt[c - '0'];
    for (int i = 0; i < 10; ++i) if (cnt[i]) printf("%d:%d\n", i, cnt[i]);
    return 0;
}
```

---
# L1-045 宇宙无敌大招呼 (5 分) 
据说所有程序员学习的第一个程序都是在屏幕上输出一句“Hello World”，跟这个世界打个招呼。作为天梯赛中的程序员，你写的程序得高级一点，要能跟任意指定的星球打招呼。

**输入格式：**
输入在第一行给出一个星球的名字 `S` ，是一个由不超过7个英文字母组成的单词，以回车结束。

**输出格式：** 
在一行中输出 `Hello S` ，跟输入的 `S` 星球打个招呼。

**输入样例：**

```cpp
Mars
```

**输出样例：**

```cpp
Hello Mars
```
代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
int main() {
    string s;
    cin >> s;
    cout << "Hello " << s;
    return 0;
}
```


---
持续更新中……
