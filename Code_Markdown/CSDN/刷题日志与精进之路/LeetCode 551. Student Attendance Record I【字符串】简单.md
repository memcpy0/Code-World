> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。



You are given a string representing an attendance record for a student. The record only contains the following three characters:
1.  `'A'` : Absent.
 2. `'L'` : Late.
  3. `'P'` : Present.

A student could be rewarded if his attendance record doesn't contain **more than one 'A'** (absent) or **more than two *continuous* 'L'** (late).

You need to return whether the student could be rewarded according to his attendance record.

**Example 1:**

```swift
Input: "PPALLP"
Output: True
```

**Example 2:**

```swift
Input: "PPALLL"
Output: False
```

题意：给定一个仅包含 `A, L, P` 三种字符的字符串，表示学生的出勤记录（`Attendance Record`）。如果字符串中不包含超过一个 `A` 字符（缺勤），而且不包含超过两个连续的 `L` 字符（迟到），就会给予学生奖励。根据这个学生的出勤记录，判断他是否会被奖赏。

---
### 解法 直接遍历
这道题中，只要字符串中 `A` 的出现次数 `<= 1` 、而且不会出现 `LLL...` 这种连续的 `L` 子串（多个间断的 `L or LL` 是允许的），学生就会得到奖赏。
> 众所周知，如果一个学生连着迟到三次"LLL"就无法评优，但是如果他选择翘课而不是迟到"LAL"，那么他可以评优😀！

代码如下所示，注意**提前退出**：
```cpp
class Solution {
public:
    bool checkRecord(string s) {
        int a = 0; //absent
        for (int i = 0, n = s.size(); i < n; ++i) {
            if (s[i] == 'A' && ++a >= 2) return false;
            else if (s[i] == 'L' && i < n - 2 && s[i + 1] == 'L' && s[i + 2] == 'L') return false;
        }
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了36.65% 的用户
```
不过这样写，看起来太不优雅了，修改一下一行搞定：
```cpp
class Solution {
public:
    bool checkRecord(string s) {
        // return count(s.begin(), s.end(), 'A') <= 1 && s.find("LLL") == string::npos;
        return s.find("LLL") == -1 && s.find('A') == s.rfind('A');
    }
};
```
效率没有什么变化，虽然调用函数内部遍历了多次：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了5.15% 的用户
```
Java的一行解法如下：
```java
class Solution {
    public boolean checkRecord(String s) {
        return s.indexOf('A') == s.lastIndexOf('A') && s.indexOf("LLL") == -1;
    }
}
```
