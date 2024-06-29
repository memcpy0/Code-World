读入一个正整数 $n$，计算其各位数字之和，用汉语拼音写出和的每一位数字。

**输入格式：**
每个测试输入包含 $1$ 个测试用例，即给出自然数 $n$ 的值。这里保证 $n$ 小于 $10^{​100}$ ​​。

**输出格式：**
在一行内输出 $n$ 的各位数字之和的每一位，拼音数字间有 $1$ 空格，但一行中最后一个拼音数字后没有空格。

**输入样例：**

```swift
1234567890987654321123456789
```

**输出样例：**

```swift
yi san wu
```
---
之前的代码：
```cpp
#include <cstdio>
#include <cstring>
int main()
{
    char s[120];
    scanf("%s", s);
    int len = strlen(s); 
    char r[][6] = {
        "ling", "yi", "er", "san", "si", 
        "wu", "liu", "qi", "ba", "jiu"
    };
    int carry_sum = 0; // 每位数的和不大
    for (int i = 0; i < len; i++) 
        carry_sum += (s[i] - '0');
    int a[5], t = 0; 
    while (carry_sum) {
        a[t] = carry_sum % 10;
        carry_sum /= 10;
        t++;
    }
    printf("%s", r[a[t - 1]]); // 第一个拼音
    for (int i = t - 2; i >= 0; i--) 
        printf(" %s", r[a[i]]);
    return 0;
}
```
现在的代码：
```cpp
#include <bits/stdc++.h>
using namespace std;
char bopomofo[10][5] = {"ling", "yi", "er", "san", "si", "wu", "liu", "qi", "ba", "jiu"};
int main() {
    int sum = 0, c; 
    while ((c = getchar()) != '\n') sum += (c - '0'); //或者使用string s; cin >> s; ...
    string str = to_string(sum); 
    for (int i = 0; i < str.size(); ++i) 
        printf(" %s" + !i, bopomofo[str[i] - '0']);
    return 0;
}
```
