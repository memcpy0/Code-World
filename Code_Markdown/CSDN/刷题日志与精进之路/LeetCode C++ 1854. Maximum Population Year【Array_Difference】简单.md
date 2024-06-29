<p>You are given a 2D integer array <code>logs</code> where each <code>logs[i] = [birth<sub>i</sub>, death<sub>i</sub>]</code> indicates the birth and death years of the <code>i<sup>th</sup></code> person.</p>

<p>The <strong>population</strong> of some year <code>x</code> is the number of people alive during that year. The <code>i<sup>th</sup></code> person is counted in year <code>x</code>'s population if <code>x</code> is in the <strong>inclusive</strong> range <code>[birth<sub>i</sub>, death<sub>i</sub> - 1]</code>. Note that the person is <strong>not</strong> counted in the year that they die.</p>

<p>Return <em>the <strong>earliest</strong> year with the <strong>maximum population</strong></em>.</p>
 
 
<p><strong>Example 1:</strong></p>

```clike
Input: logs = [[1993,1999],[2000,2010]]
Output: 1993
Explanation: The maximum population is 1, and 1993 is the earliest year with this population.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: logs = [[1950,1961],[1960,1971],[1970,1981]]
Output: 1960
Explanation: 
The maximum population is 2, and it had happened in years 1960 and 1970.
The earlier year between them is 1960.
```

  
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= logs.length &lt;= 100</code></li>
	<li><code>1950 &lt;= birth<sub>i</sub> &lt; death<sub>i</sub> &lt;= 2050</code></li>
</ul>

题意：求出 **人口最多** 且 **最早** 的年份。

---
### 解法1 遍历数组
暴力做法。对于每一年，寻找这一年活着的人数，最后找到所有年份中的最大存活人数：
```cpp
class Solution {
public:
    int maximumPopulation(vector<vector<int>>& logs) {
        int maxCnt = 0, maxYear = 0, n = logs.size();
        for (int i = 1950; i <= 2050; ++i) {
            int cnt = 0;
            for (int j = 0; j < n; ++j) if (i >= logs[j][0] && i < logs[j][1]) ++cnt;
            if (cnt > maxCnt) maxCnt = cnt, maxYear = i;
        }
        return maxYear;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了44.01% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了84.88% 的用户
```
使用STL的 `count_if` ：
```cpp
class Solution {
public:
    int maximumPopulation(vector<vector<int>>& logs) {
        int maxCnt = 0, maxYear = 0, n = logs.size();
        for (int i = 1950; i <= 2050; ++i) {
            int cnt = count_if(logs.begin(), logs.end(), [&](const vector<int>& log) { return i >= log[0] && i < log[1]; });
            if (cnt > maxCnt) maxCnt = cnt, maxYear = i;
        }
        return maxYear;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了91.83% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了52.53% 的用户
```
---
### 解法2 差分
设置一个 `pop[]` ，`pop[i]` 表示年份 `i` 的人口数目。于是可以将每个 `logs[i] = [birthi, deathi]` 视为对 `[birthi, deathi)` 这一年份区间的人口都加 `1` 。

因此，我们可以使用差分来优化。差分数组 `d[]` 是 `pop[]` 中前后元素的差组成的数组，即 `d[0] = pop[0], d[i] = pop[i] - pop[i - 1], i > 0` 。每个 `logs[i]` 都可以转化为 `d[birthi] += 1, d[deathi] -= 1` 这两个操作。

接着从 `d[0]` 开始，使用 `pop[0]  = d[0], pop[i] = d[i] + pop[i - 1]` 求前缀和的方法，恢复原始数组 `pop[]` ，并得到人口最多的年份。这一方法的时间和空间复杂度都为 $O(n)$ 。
```cpp
class Solution {
public:
    int maximumPopulation(vector<vector<int>>& logs) {
        int maxCnt = 0, maxYear = 0, n = logs.size(), curPop = 0;
        vector<int> d(101, 0);
        for (const vector<int>& log : logs) ++d[log[0] - 1950], --d[log[1] - 1950];
        for (int i = 0; i <= 100; ++i) {
            curPop += d[i];
            if (curPop > maxCnt) maxCnt = curPop, maxYear = i + 1950; 
        }
        return maxYear;
    }
};
```
由于数据量不大，运行效率没有显著的差别。类似的题目还有[1109. 航班预订统计](https://leetcode-cn.com/problems/corporate-flight-bookings/)，这一题就会显示出差分和暴力方法的效率差距。
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了91.81% 的用户
内存消耗：7.6 MB, 在所有 C++ 提交中击败了90.47% 的用户
```
需要说明的是，如果差分数组太大且数据比较稀疏，可以使用 `map<int, int>` 来替代。
