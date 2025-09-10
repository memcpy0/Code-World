> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Alice and Bob are traveling to Rome for separate business meetings.

You are given 4 strings `arriveAlice`, `leaveAlice`, `arriveBob`, and `leaveBob`. Alice will be in the city from the dates `arriveAlice` to `leaveAlice` (**inclusive**), while Bob will be in the city from the dates `arriveBob` to `leaveBob` (**inclusive**). Each will be a 5-character string in the format `"MM-DD"`, corresponding to the month and day of the date.

Return _the total number of days that Alice and Bob are in Rome together._

You can assume that all dates occur in the **same** calendar year, which is **not** a leap year. Note that the number of days per month can be represented as: `[31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]`.

**Example 1:**
```java
Input: arriveAlice = "08-15", leaveAlice = "08-18", arriveBob = "08-16", leaveBob = "08-19"
Output: 3
Explanation: Alice will be in Rome from August 15 to August 18. Bob will be in Rome from August 16 to August 19. They are both in Rome together on August 16th, 17th, and 18th, so the answer is 3.
```
**Example 2:**
```java
Input: arriveAlice = "10-01", leaveAlice = "10-31", arriveBob = "11-01", leaveBob = "12-31"
Output: 0
Explanation: There is no day when Alice and Bob are in Rome together, so we return 0.
```
**Constraints:**
-   All dates are provided in the format `"MM-DD"`.
-   Alice and Bob's arrival dates are **earlier than or equal to** their leaving dates.
-   The given dates are valid dates of a **non-leap** year.

题意：Alice 和 Bob 计划分别去罗马开会。给出四个字符串 `arriveAlice` ，`leaveAlice` ，`arriveBob` 和 `leaveBob` 。Alice 会在日期 `arriveAlice` 到 `leaveAlice` 之间在城市里（**日期为闭区间**），而 Bob 在日期 `arriveBob` 到 `leaveBob` 之间在城市里（**日期为闭区间**）。每个字符串都包含 5 个字符，格式为 `"MM-DD"` ，对应着一个日期的月和日。

返回 Alice和 Bob 同时在罗马的天数。可以假设所有日期都在 **同一个** 自然年，而且 **不是** 闰年。每个月份的天数分别为：`[31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]` 。

---
### 解法 前缀和+容斥原理
设计一个 `f` 来计算输入中的每个日子在一年中是第几天。计算输入中的每个日子在一年中是第几天时，可以利用**前缀和数组**来降低每次计算的复杂度。知道每个日子是一年中的第几天后，比较算出**两人到达日子的最大值，离开日子的最小值**，然后利用减法计算重合的日子。这也是容斥原理的应用。
```cpp
class Solution {
private:
    int days[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
public:
    int countDaysTogether(string arriveAlice, string leaveAlice, string arriveBob, string leaveBob) {
        function<int(string)> f = [&](const string &a) {
            int m = (a[0] - '0') * 10 + (a[1] - '0');
            int d = (a[3] - '0') * 10 + (a[4] - '0');
            return days[m - 1] + d;
        };
        return max(min(f(leaveAlice), f(leaveBob)) - max(f(arriveAlice), f(arriveBob)) + 1, 0);
    }
};
```

