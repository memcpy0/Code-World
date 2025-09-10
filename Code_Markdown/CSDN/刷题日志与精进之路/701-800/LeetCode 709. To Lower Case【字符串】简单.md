> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a string `s`, return *the string after replacing every uppercase letter with the same lowercase letter*.

**Example 1:**
```clike
Input: "Hello"
Output: "hello"
```
**Example 2:**
```clike
Input: "here"
Output: "here"
```
**Example 3:**
```clike
Input: "LOVELY"
Output: "lovely"
```
**Constraints:**
- `1 <= s.length <= 100`
- `s` consists of printable ASCII characters.

题意：将字符串中的大写字母转为小写。

---
### 解法1 库函数
C++直接用 `tolower` 函数：
```cpp
//C++ version
class Solution {
public:
    string toLowerCase(string str) { 
        for (char &c : str) c = tolower(c);
        return str;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.1 MB, 在所有 C++ 提交中击败了32.49% 的用户
```
Java可用 `String` 的 `toLowerCase()` ：
```java
//Jave version
class Solution {
    public String toLowerCase(String s) {
        return s.toLowerCase();
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：36.6 MB, 在所有 Java 提交中击败了41.90% 的用户
```

---
### 解法2 位运算
由ASCII码表可知，要令字母都变小写，只需 `ch |= 32` ；字母都变大写，则要 `ch &= -33` ，或者 `ch &= 0xdf` ；大小写翻转，写成 `ch ^= 32` 。
<img src="https://img-blog.csdnimg.cn/20200728115829914.png#pic_center" width="25%">

下面是以前写的代码：
```cpp
class Solution {
public:
    string toLowerCase(string str) { 
        for (char &c : str) c |= 32;
        return str;
    }
};
```
现在过不了了，因为字符串中不仅仅只有字母，还有其他可打印字符。修改后的代码如下：
```cpp
//C++ version
class Solution {
public:
    string toLowerCase(string s) {
        for (char &c : s) if (isalpha(c)) c |= 32;
        return s;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了84.58% 的用户
```
---
### 解法3 对不可变字符串的写法
由于Java的 `String` 不可变，又不想用库函数，就要写成如下形式：
```java
//Java version
class Solution {
    public String toLowerCase(String s) {
        StringBuilder sb = new StringBuilder();
        for (char c : s.toCharArray()) {
            if (c >= 'A' && c <= 'Z') sb.append((char)(c + 32));
            else sb.append(c);
        }
        return sb.toString();
    }
}
//执行用时：0 ms, 在所有 Java 提交中击败了100.00% 的用户
//内存消耗：36.2 MB, 在所有 Java 提交中击败了96.16% 的用户
```
