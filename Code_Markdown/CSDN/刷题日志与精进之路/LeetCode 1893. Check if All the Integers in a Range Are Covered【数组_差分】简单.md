<p>You are given a 2D integer array <code>ranges</code> and two integers <code>left</code> and <code>right</code>. Each <code>ranges[i] = [start<sub>i</sub>, end<sub>i</sub>]</code> represents an <strong>inclusive</strong> interval between <code>start<sub>i</sub></code> and <code>end<sub>i</sub></code>.</p>

<p>Return <code>true</code> <em>if each integer in the inclusive range</em> <code>[left, right]</code> <em>is covered by <strong>at least one</strong> interval in</em> <code>ranges</code>. Return <code>false</code> <em>otherwise</em>.</p>

<p>An integer <code>x</code> is covered by an interval <code>ranges[i] = [start<sub>i</sub>, end<sub>i</sub>]</code> if <code>start<sub>i</sub> &lt;= x &lt;= end<sub>i</sub></code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: ranges = [[1,2],[3,4],[5,6]], left = 2, right = 5
Output: true
Explanation: Every integer between 2 and 5 is covered:
- 2 is covered by the first range.
- 3 and 4 are covered by the second range.
- 5 is covered by the third range.
```
  
<p><strong>Example 2:</strong></p>

```cpp
Input: ranges = [[1,10],[10,20]], left = 21, right = 21
Output: false
Explanation: 21 is not covered by any range.
```
 
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= ranges.length &lt;= 50</code></li>
	<li><code>1 &lt;= start<sub>i</sub> &lt;= end<sub>i</sub> &lt;= 50</code></li>
	<li><code>1 &lt;= left &lt;= right &lt;= 50</code></li>
</ul>

题意：给你一个二维整数数组&nbsp;<code>ranges</code>&nbsp;和两个整数&nbsp;<code>left</code>&nbsp;和&nbsp;<code>right</code>&nbsp;。每个&nbsp;<code>ranges[i] = [start<sub>i</sub>, end<sub>i</sub>]</code>&nbsp;表示一个从&nbsp;<code>start<sub>i</sub></code>&nbsp;到&nbsp;<code>end<sub>i</sub></code>&nbsp;的&nbsp;<strong>闭区间</strong>&nbsp;。

<p>如果闭区间&nbsp;<code>[left, right]</code>&nbsp;内每个整数都被&nbsp;<code>ranges</code>&nbsp;中&nbsp;<strong>至少一个</strong>&nbsp;区间覆盖，那么请你返回&nbsp;<code>true</code>&nbsp;，否则返回&nbsp;<code>false</code>&nbsp;。</p>

<p>已知区间 <code>ranges[i] = [start<sub>i</sub>, end<sub>i</sub>]</code> ，如果整数 <code>x</code> 满足 <code>start<sub>i</sub> &lt;= x &lt;= end<sub>i</sub></code>&nbsp;，那么我们称整数<code>x</code>&nbsp;被覆盖了。</p>

---
### 解法1 暴力
暴力枚举 `[left, right]` 区间内的每一个整数，看是否有区间覆盖了它：
```cpp
class Solution {
public:
    bool isCovered(vector<vector<int>>& ranges, int left, int right) {
        for (int i = left; i <= right; ++i) {
            bool flag = false;
            for (const vector<int>& range : ranges) {
                if (i >= range[0] && i <= range[1]) {
                    flag = true; 
                    break;
                }
            }
            if (!flag) return false; //没有区间覆盖这一整数
        }
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了89.49% 的用户
内存消耗：8.6 MB, 在所有 C++ 提交中击败了48.34% 的用户
```
时间复杂度为：$O((right - left + 1) \times ranges.length)$ ，空间复杂度为：$O(1)$ 。

另一种暴力方法是，遍历所有的区间，将它们覆盖的点标记在数轴上，然后查看 `[left, right]` 这一区间是否全部被标记：
```cpp
class Solution {
public:
    bool isCovered(vector<vector<int>>& ranges, int left, int right) {
        bitset<60> bst;
        for (const vector<int>& range : ranges)
            for (int i = range[0]; i <= range[1]; ++i) bst[i] = true;
        for (int i = left; i <= right; ++i) if (bst[i] == false) return false;
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了42.04% 的用户
内存消耗：8.6 MB, 在所有 C++ 提交中击败了51.11% 的用户
```
时间复杂度为：$O(ranges.length \times \sum^{ranges.length-1}_{i=0} (end_i - start_i +1) + (right - left + 1))$ ，空间复杂度为：$O(max(end_i))$ 。不难发现，上一种暴力解法的优势更大。

---
### 解法2 差分
在第二种暴力做法上的优化，用空间换时间，用差分代替暴力标记：
```cpp
class Solution {
public:
    bool isCovered(vector<vector<int>>& ranges, int left, int right) {
        vector<int> d(60, 0);
        for (const vector<int>& range : ranges) ++d[range[0]], --d[range[1] + 1];
        for (int i = 1; i < 60; ++i) d[i] += d[i - 1]; 
        for (int i = left; i <= right; ++i) if (d[i] <= 0) return false;
        return true;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了85.51% 的用户
```
时间复杂度为：$O(ranges.length + max(end_i) + (right - left + 1))$ ，空间复杂度为：$O(max(end_i))$ 。
