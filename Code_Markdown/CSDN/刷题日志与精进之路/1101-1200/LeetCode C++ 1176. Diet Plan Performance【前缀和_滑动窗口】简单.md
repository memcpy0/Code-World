
<p>A dieter consumes&nbsp;<code>calories[i]</code>&nbsp;calories on the <code>i</code>-th day.&nbsp;</p>

<p>Given an integer <code>k</code>, for <strong>every</strong> consecutive sequence of <code>k</code> days (<code>calories[i], calories[i+1], ..., calories[i+k-1]</code>&nbsp;for all <code>0 &lt;= i &lt;= n-k</code>), they look at <em>T</em>, the total calories consumed during that sequence of <code>k</code> days (<code>calories[i] + calories[i+1] + ... + calories[i+k-1]</code>):</p>

<ul>
	<li>If <code>T &lt; lower</code>, they performed poorly on their diet and lose 1 point;&nbsp;</li>
	<li>If <code>T &gt; upper</code>, they performed well on their diet and gain 1 point;</li>
	<li>Otherwise, they performed normally and there is no change in points.</li>
</ul>

<p>Initially, the dieter has zero points. Return the total number of points the dieter has after dieting for <code>calories.length</code>&nbsp;days.</p>

<p>Note that the total points can be negative.</p>

 
<p><strong>Example 1:</strong></p>

```haskell
Input: calories = [1,2,3,4,5], k = 1, lower = 3, upper = 3
Output: 0
Explanation: Since k = 1, we consider each element of the array separately and compare it to lower and upper.
calories[0] and calories[1] are less than lower so 2 points are lost.
calories[3] and calories[4] are greater than upper so 2 points are gained.
```

 

<p><strong>Example 2:</strong></p>
 

```haskell
Input: calories = [3,2], k = 2, lower = 0, upper = 1
Output: 1
Explanation: Since k = 2, we consider subarrays of length 2.
calories[0] + calories[1] > upper so 1 point is gained.
```

 
<p><strong>Example 3:</strong></p>

```haskell
Input: calories = [6,5,0,0], k = 2, lower = 1, upper = 5
Output: 0
Explanation:
calories[0] + calories[1] > upper so 1 point is gained.
lower <= calories[1] + calories[2] <= upper so no change in points.
calories[2] + calories[3] < lower so 1 point is lost.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= k &lt;= calories.length &lt;= 10^5</code></li>
	<li><code>0 &lt;= calories[i] &lt;= 20000</code></li>
	<li><code>0 &lt;= lower &lt;= upper</code></li>
</ul>

题意：统计计划时间共 `calories.length` 天后得到的总分，返回作为评估结果。
 
---
### 解法1 前缀和
计算 `calories` 的前缀和，这样每次就能够用 $O(1)$ 时间得到**某天及之后的连续几天内消耗的总卡路里**：
```cpp
class Solution {
public:
    int dietPlanPerformance(vector<int>& calories, int k, int lower, int upper) {
        int ans = 0, n = calories.size();
        vector<int> partialsum(n + 1);
        partial_sum(calories.begin(), calories.end(), partialsum.begin() + 1);
        // for (int i = 0; i < n; ++i) partialsum [i + 1] = partialsum[i] + calories[i];
        for (int i = 0; i <= n - k; ++i) {
            int ksum = partialsum[i + k] - partialsum[i];
            if (ksum < lower) --ans;
            if (ksum > upper) ++ans;
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了50.00% 的用户
内存消耗：22.8 MB, 在所有 C++ 提交中击败了5.55% 的用户
```
---
### 解法2 滑动窗口
固定长度的滑动窗口：
```cpp
class Solution {
public:
    int dietPlanPerformance(vector<int>& calories, int k, int lower, int upper) {
        int ans = 0, n = calories.size(), partialsum = 0;
        for (int i = 0; i < n; ++i) {
            partialsum += calories[i]; //扩张
            if (i >= k - 1) {
                if (partialsum < lower) --ans;
                if (partialsum > upper) ++ans;
                partialsum -= calories[i + 1 - k]; //收缩
            } 
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了31.48% 的用户
内存消耗：22.1 MB, 在所有 C++ 提交中击败了24.07% 的用户
```
