> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a word, you need to judge whether the usage of capitals in it is right or not.

We define the usage of capitals in a word to be right when one of the following cases holds:
1.  All letters in this word are capitals, like "USA".
2. All letters in this word are not capitals, like "leetcode".
 3. Only the first letter in this word is capital, like "Google".

Otherwise, we define that this word doesn't use capitals in a right way.

 

**Example 1:**

```swift
Input: "USA"
Output: True
```

 

**Example 2:**

```swift
Input: "FlaG"
Output: False
```
**Constraints**:
- `1 <= word.length <= 100`
- `word` consists of lowercase and uppercase English letters.

题意：给定一个单词，判断单词的大写使用是否正确。在以下情况时，单词的大写用法是正确的：
- 全部字母都是大写
- 单词中所有字母都不是大写
- 如果单词不只含有一个字母，只有首字母大写

---
### 解法 简单遍历
我的写法如下，尽管遍历了两次。时间复杂度为 $O(n)$ ，空间复杂度为 $O(1)$ 。
```cpp
//C++ version
class Solution {
public:
    bool detectCapitalUse(string word) {
        bool isAllCapital = true, isAllNotCapital = true, isFirstCapital = true;
        for (int i = 0; i < word.size(); ++i) {
            if (isupper(word[i])) isAllNotCapital = false;
            if (islower(word[i])) isAllCapital = false;
        }
        if (isAllCapital != isAllNotCapital) return true; //要么全大写,要么全小写
        for (int i = 0; i < word.size(); ++i) {
            if (i == 0 && !isupper(word[i])) return false; //首字母不大写
            else if (i && isupper(word[i])) return false; //其他字母大写
        }
        return true;
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了73.58% 的用户
```
通过统计大写字母的个数，只需遍历一次即可解决本题：
```cpp
//C++ version
class Solution {
public:
    bool detectCapitalUse(string word) {
        int u = count_if(word.begin(), word.end(), [](const char &c) {
            return isupper(c);
        }); // 全大写、全小写、一个大写、只有首字母大写
        return u == word.size() || !u || (u == 1 && isupper(word[0]));
    }
};
//执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
//内存消耗：5.9 MB, 在所有 C++ 提交中击败了80.34% 的用户
```
