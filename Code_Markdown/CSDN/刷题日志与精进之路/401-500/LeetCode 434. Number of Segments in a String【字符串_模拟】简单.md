> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are given a string `s`, return *the number of segments in the string.* 

A **segment** is defined to be a contiguous sequence of **non-space characters.**

 

**Example 1:**

```swift
Input: s = "Hello, my name is John"
Output: 5
Explanation: The five segments are ["Hello,", "my", "name", "is", "John"]
```

**Example 2:**

```swift
Input: s = "Hello"
Output: 1
```

**Example 3:**

```swift
Input: s = "love live! mu'sic forever"
Output: 4
```

**Example 4:**

```swift
Input: s = ""
Output: 0
```

 

**Constraints:**

-  `0 <= s.length <= 300`
-  `s` consists of lower-case and upper-case English letters, digits or one of the following characters `"!@#$%^&*()_+-=',.:"`.
- The only space character in `s` is `' '`.


题意：统计字符串中的单词个数，这里的单词指的是**连续的**、**不是空格**的字符。注意，字符串里不包括任何不可打印的字符，空格字符就是 `' '`。

---
### 解法1 分割单词序列
如果只是简单统计空格数量加一，一定会出错，比如 `"   "` ，有空格却没有一个字符。为此，可以分割字符串得到单词序列，然后返回单词的个数：
```cpp
//C++ version
class Solution {
public:
    int countSegments(string s) {
        if (s.empty()) return 0;
        vector<string> tokensList;
        int beginPos = s.find_first_not_of(' ');
        int endPos = s.find_first_of(' ', beginPos);
        while (beginPos != string::npos) {
            tokensList.push_back(s.substr(beginPos, endPos - beginPos));
            beginPos = s.find_first_not_of(' ', endPos);
            endPos = s.find_first_of(' ', beginPos);
        }
        return tokensList.size();
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.4 MB, 在所有 C++ 提交中击败了5.64% 的用户
```
---
### 解法2 模拟
由于单词是「连续的不是空格的字符」，我们可以从前往后处理字符串 `s` 并进行计数——跳过是空格的字符；对于非空格的字符，在遍历完一个完整单词（连续一段）后进行一次计数。整个算法的时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。

使用STL的写法如下：
```cpp
//C++ version
class Solution {
public:
    int countSegments(string s) {
        if (s.empty()) return 0;
        int tokensNum = 0;
        int beginPos = s.find_first_not_of(' ');
        int endPos = s.find_first_of(' ', beginPos);
        while (beginPos != string::npos) {
            ++tokensNum;
            beginPos = s.find_first_not_of(' ', endPos);
            endPos = s.find_first_of(' ', beginPos);
        }
        return tokensNum;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：6.1 MB, 在所有 C++ 提交中击败了54.60% 的用户
```
简单写法如下：
```cpp
//C++ version
class Solution {
public:
    int countSegments(string s) {
        if (s.empty()) return 0;
        int ans = 0;
        for (int i = 0, n = s.size(); i < n; ) {
            if (s[i] == ' ') { ++i; continue; }
            while (i < n && s[i] != ' ') ++i;
            ++ans;
        }
        return ans;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了92.35% 的用户
```
