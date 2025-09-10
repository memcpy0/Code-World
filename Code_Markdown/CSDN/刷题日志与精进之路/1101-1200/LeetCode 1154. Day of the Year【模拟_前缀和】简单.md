> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given a string <code>date</code> representing a <a href="https://en.wikipedia.org/wiki/Gregorian_calendar">Gregorian calendar</a> date formatted as <code>YYYY-MM-DD</code>, return the day number of the year.</p>

<p><strong>Example 1:</strong></p>

```cpp
Input: date = "2019-01-09"
Output: 9
Explanation: Given date is the 9th day of the year in 2019.
```
<p><strong>Example 2:</strong></p>

```cpp
Input: date = "2019-02-10"
Output: 41
```
<p><strong>Example 3:</strong></p>

```cpp
Input: date = "2003-03-01"
Output: 60
```
<p><strong>Example 4:</strong></p>

```cpp
Input: date = "2004-03-01"
Output: 61
```
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>date.length == 10</code></li>
	<li><code>date[4] == date[7] == '-'</code>, and all other <code>date[i]</code>'s are digits</li>
	<li><code>date</code> represents a calendar date between Jan 1st, 1900 and Dec 31, 2019.</li>
</ul>

题意：给你一个字符串 `date` ，按 `YYYY-MM-DD` 格式表示一个 [现行公元纪年法](https://leetcode-cn.com/problems/day-of-the-year/) 日期。请你计算并返回该日期是当年的第几天。

通常情况下，我们认为 1 月 1 日是每年的第 1 天，1 月 2 日是每年的第 2 天，依此类推。每个月的天数与现行公元纪年法（格里高利历）一致。

  
---
### 解法 模拟+前缀和
本题可根据题意进行模拟。由于每年月份的天数都相对固定，只有闰年的 $2$ 月不同，我们可以预先打表，即构造一个月份数组 `months[2][13]` ，平年查询 `months[0]` 的各个月份，闰年查询 `months[1]` 的各个月份。不断累加 `MM` 月之前的天数，加上该月的天数 `DD` ，就是答案：
```cpp
//C++ version
int months[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};
int isLeap(int y) {
    return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
}
class Solution {
public:
    int dayOfYear(string date) {
        int y = stoi(date.substr(0, 4));
        int m = stoi(date.substr(5, 2));
        int d = stoi(date.substr(8, 2));
        int ans = 0, pos = isLeap(y);
        for (int i = 1; i < m; ++i)
            ans += months[pos][i];
        return ans + d;
    }
};
//执行用时：16 ms, 在所有 C++ 提交中击败了61.56% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了81.91% 的用户
```
不难发现，还有优化的空间——**我们只需要构造平年的月份数组**，当年份为闰年且当前月份超过 $2$ 月时追加一天即可；我们不需要累加每个月份的天数，**预先打表的时候使用前缀和即可**。时间复杂度和空间复杂度都为 $O(1)$ 。
```cpp
//C++ version
int months[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
int isLeap(int y) {
    return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
}
class Solution {
public:
    int dayOfYear(string date) {
        int y = stoi(date.substr(0, 4));
        int m = stoi(date.substr(5, 2));
        int d = stoi(date.substr(8, 2));
        return (m > 2 && isLeap(y) ? months[m - 1] + 1 : months[m - 1]) + d;
    }
};
//执行用时：12 ms, 在所有 C++ 提交中击败了82.05% 的用户
//内存消耗：5.8 MB, 在所有 C++ 提交中击败了70.74% 的用户
```

