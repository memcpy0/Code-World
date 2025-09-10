> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Write a function that reverses a string. The input string is given as an array of characters `char[]` .

Do not allocate extra space for another array, you must do this by **modifying the input array** `in-place` with `O(1)` extra memory.

You may assume all the characters consist of printable ascii characters.

**Example 1:**
```swift
Input: ["h","e","l","l","o"]
Output: ["o","l","l","e","h"]
```
**Example 2:**
```swift
Input: ["H","a","n","n","a","h"]
Output: ["h","a","n","n","a","H"]
```
题意：编写一个函数，其作用是将输入的字符串反转过来。要求必须原地修改输入数组、使用 $O(1)$ 的额外空间解决这一问题。

---
### 解法一 STL
使用万能的STL大法：
```cpp
class Solution {
public:
    void reverseString(vector<char>& s) {
        reverse(s.begin(), s.end());
    }
};
```
效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了68.97% 的用户
内存消耗：22.7 MB, 在所有 C++ 提交中击败了48.85% 的用户
```
---
### 解法二 $O(1)$ 空间
很简单的代码：
```cpp
class Solution {
public:
    void reverseString(vector<char>& s) {
        for (int i = 0, j = s.size() - 1; i < j; ++i, --j)
            swap(s[i], s[j]);
    }
};
```
结果如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了68.97% 的用户
内存消耗：22.8 MB, 在所有 C++ 提交中击败了37.87% 的用户
```

