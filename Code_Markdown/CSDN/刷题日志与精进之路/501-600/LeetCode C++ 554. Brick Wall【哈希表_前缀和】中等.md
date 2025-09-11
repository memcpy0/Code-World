<p>There is a rectangular brick wall in front of you with <code>n</code> rows of bricks. The <code>i<sup>th</sup></code> row has some number of bricks each of the same height (i.e., one unit) but they can be of different widths. The total width of each row is the same.</p>

<p>Draw a vertical line from the top to the bottom and cross the least bricks. If your line goes through the edge of a brick, then the brick is not considered as crossed. You cannot draw a line just along one of the two vertical edges of the wall, in which case the line will obviously cross no bricks.</p>

<p>Given the 2D array <code>wall</code> that contains the information about the wall, return <em>the minimum number of crossed bricks after drawing such a vertical line</em>.</p>
 
<p><strong>Example 1:</strong></p>
<img style="width: 493px; height: 577px;" src="https://assets.leetcode.com/uploads/2021/04/24/cutwall-grid.jpg" alt="" width="50%">

```clike
Input: wall = [[1,2,2,1],[3,1,2],[1,3,2],[2,4],[3,1,2],[1,3,1,1]]
Output: 2
```
 
<p><strong>Example 2:</strong></p>

```clike
Input: wall = [[1],[1],[1]]
Output: 3
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == wall.length</code></li>
	<li><code>1 &lt;= n &lt;= 10<sup>4</sup></code></li>
	<li><code>1 &lt;= wall[i].length &lt;= 10<sup>4</sup></code></li>
	<li><code>1 &lt;= sum(wall[i].length) &lt;= 2 * 10<sup>4</sup></code></li>
	<li><code>sum(wall[i])</code> is the same for each row <code>i</code>.</li>
	<li><code>1 &lt;= wall[i][j] &lt;= 2<sup>31</sup> - 1</code></li>
</ul>



题意：你的面前有一堵矩形的、由 <code>n</code> 行砖块组成的砖墙。这些砖块高度相同（也就是一个单位高）但是宽度不同。每一行砖块的宽度之和应该相等。 

<p>你现在要画一条 <strong>自顶向下 </strong>的、穿过 <strong>最少 </strong>砖块的垂线。如果你画的线只是从砖块的边缘经过，就不算穿过这块砖。<strong>你不能沿着墙的两个垂直边缘之一画线，这样显然是没有穿过一块砖的。</strong></p>

<p>给你一个二维数组 <code>wall</code> ，该数组包含这堵墙的相关信息。其中，<code>wall[i]</code> 是一个代表从左至右每块砖的宽度的数组。你需要找出怎样画才能使这条线 <strong>穿过的砖块数量最少</strong> ，并且返回 <strong>穿过的砖块数量</strong> 。</p>

---
### 解法 哈希映射+前缀和
一开始，我也没有太好的做法。暴力遍历每一列、计算经过的砖块数是 $O(n^2)$ 的做法，而且如何判断是否经过某个砖块也有点麻烦。后来画了个图，问题就解决了。以下为**Example 1**的图示：
<img src="https://img-blog.csdnimg.cn/20210506230309837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
我们不难观察到，上下砖块的边框对齐，往往说明它们的前缀和长度一致。于是为了划线时经过最少的砖块，我们必须挑选**砖块上下边框对齐**这种情况出现最多的那条线。甚至于我们不用关注这条线，只需要计算**出现次数最多的前缀和长度**，就可以知道**该在哪里划线**——同时，我们也知道了这条线经过多少砖块，只要用**行数**减去**砖块上下边框对齐的数目**（即该前缀和出现的次数）即可：
```cpp
class Solution {
public:
    int leastBricks(vector<vector<int>>& wall) {
        unordered_map<int, int> cnt;
        for (const vector<int>& row : wall) {
            int sum = 0, n = row.size();
            for (int i = 0; i < n - 1; ++i) {
                sum += row[i];
                ++cnt[sum];
            } 
        }
        int maxcnt = 0;
        for (auto it : cnt) maxcnt = max(maxcnt, it.second);
        return wall.size() - maxcnt;
    }
};
```
运行效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了50.04% 的用户
内存消耗：21 MB, 在所有 C++ 提交中击败了89.29% 的用户
```
