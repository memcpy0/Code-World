<p>A bus&nbsp;has <code>n</code> stops numbered from <code>0</code> to <code>n - 1</code> that form&nbsp;a circle. We know the distance between all pairs of neighboring stops where <code>distance[i]</code> is the distance between the stops number&nbsp;<code>i</code> and <code>(i + 1) % n</code>.</p>

<p>The bus goes along both directions&nbsp;i.e. clockwise and counterclockwise. Return the shortest distance between the given&nbsp;<code>start</code>&nbsp;and <code>destination</code>&nbsp;stops.</p>

<p><strong>Example 1:</strong></p>

<p><img style="width: 388px; height: 240px;" src="https://assets.leetcode.com/uploads/2019/09/03/untitled-diagram-1.jpg" alt=""></p>

```cpp
Input: distance = [1,2,3,4], start = 0, destination = 1
Output: 1
Explanation: Distance between 0 and 1 is 1 or 9, minimum is 1.
```
 

<p><strong>Example 2:</strong></p>

<p><img style="width: 388px; height: 240px;" src="https://assets.leetcode.com/uploads/2019/09/03/untitled-diagram-1-1.jpg" alt=""></p>

```cpp
Input: distance = [1,2,3,4], start = 0, destination = 2
Output: 3
Explanation: Distance between 0 and 2 is 3 or 7, minimum is 3.
```
 

<p><strong>Example 3:</strong></p>

<p><img style="width: 388px; height: 240px;" src="https://assets.leetcode.com/uploads/2019/09/03/untitled-diagram-1-2.jpg" alt=""></p>

```cpp
Input: distance = [1,2,3,4], start = 0, destination = 3
Output: 4
Explanation: Distance between 0 and 3 is 6 or 4, minimum is 4.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n&nbsp;&lt;= 10^4</code></li>
	<li><code>distance.length == n</code></li>
	<li><code>0 &lt;= start, destination &lt; n</code></li>
	<li><code>0 &lt;= distance[i] &lt;= 10^4</code></li>
</ul>

题意：环形公交路线上有 `n` 个站，按次序从 `0` 到 `n - 1` 进行编号，`distance[i]` 表示编号为 `i` 的车站和编号为 `(i + 1) % n` 的车站之间的距离。环线上的公交车都可以按顺时针和逆时针的方向行驶。返回乘客从出发点 `start` 到目的地 `destination` 之间的最短距离。

--- 
### 解法1 往两边扫描
```cpp
class Solution {
public:
    int distanceBetweenBusStops(vector<int>& dist, int s, int d) {
        int n = dist.size(), left = dist[d], right = 0;
        for (int i = (s - 1 + n) % n; i != d; i = (i - 1 + n) % n) left += dist[i]; //往左走
        for (int i = s; i != d; i = (i + 1) % n) right += dist[i]; //往右走
        return min(left, right);
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了98.68% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了97.34% 的用户
```
---
### 解法2 前缀和
如果要多次查询同一条环形公交车路线上的某两个站点间最短距离，可以先使用前缀和进行预处理：
```cpp
class Solution {
public:
    int distanceBetweenBusStops(vector<int>& dist, int s, int d) {
        if (s > d) swap(s, d); //必不可少
        int n = dist.size();
        vector<int> psum(n + 1); //lsum[i]表示dist[0:i)区间和,即从0到i的距离
        for (int i = 0; i < n; ++i) psum[i + 1] = psum[i] + dist[i];
        int right = psum[d] - psum[s], left = psum[n] - right;
        return min(left, right);
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了68.42% 的用户
内存消耗：8.6 MB, 在所有 C++ 提交中击败了93.36% 的用户
```

