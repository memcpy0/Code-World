> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


Write a function that takes a string as input and reverse only the vowels of a string.

**Example 1:**

```swift
Input: "hello"
Output: "holle"
```

**Example 2:**

```swift
Input: "leetcode"
Output: "leotcede"
```

**Note:** The vowels does not include the letter `"y"` .

题意：编写一个函数，以字符串作为输入，反转该字符串中的元音字母。


---
### 解法 双指针
很简单的双指针题目，只是需要注意**范围检查**和大小写的问题。代码如下：
```cpp
class Solution {
private:
    bool isvowel(char c) {
        c = tolower(c);
        return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    }
public:
    string reverseVowels(string s) {
        if (s.size() <= 1) return s;
        int left = 0, right = s.size() - 1;
        while (left < right) {
            while (left < right && !isvowel(s[left])) ++left;
            while (left < right && !isvowel(s[right])) --right;
            if (left < right) swap(s[left++], s[right--]); 
            else break;
        }
        return s;
    }
};
```
 效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了99.62% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了60.21% 的用户
```
