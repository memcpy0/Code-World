> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>You are given <code>n</code> <code>points</code> in the plane that are all <strong>distinct</strong>, where <code>points[i] = [x<sub>i</sub>, y<sub>i</sub>]</code>. A <strong>boomerang</strong> is a tuple of points <code>(i, j, k)</code> such that the distance between <code>i</code> and <code>j</code> equals the distance between <code>i</code> and <code>k</code> <strong>(the order of the tuple matters)</strong>.</p>

<p>Return <em>the number of boomerangs</em>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: points = [[0,0],[1,0],[2,0]]
Output: 2
Explanation: The two boomerangs are [[1,0],[0,0],[2,0]] and [[1,0],[2,0],[0,0]].
```

<p><strong>Example 2:</strong></p>

```cpp
Input: points = [[1,1],[2,2],[3,3]]
Output: 2
```
<p><strong>Example 3:</strong></p>

```cpp
Input: points = [[1,1],[2,2],[3,3]]
Output: 2
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>n == points.length</code></li>
	<li><code>1 &lt;= n &lt;= 500</code></li>
	<li><code>points[i].length == 2</code></li>
	<li><code>-10<sup>4</sup> &lt;= x<sub>i</sub>, y<sub>i</sub> &lt;= 10<sup>4</sup></code></li>
	<li>All the points are <strong>unique</strong>.</li>
</ul>



题意：给定平面上<em> </em><code>n</code><em> </em>对 <strong>互不相同</strong> 的点 <code>points</code> ，其中 <code>points[i] = [x<sub>i</sub>, y<sub>i</sub>]</code> 。<strong>回旋镖</strong> 是由点 <code>(i, j, k)</code> 表示的元组 ，其中 <code>i</code> 和 <code>j</code> 之间的距离和 <code>i</code> 和 <code>k</code> 之间的距离相等（<strong>需要考虑元组的顺序</strong>）。返回平面上所有回旋镖的数量。 


---
### 解法 哈希表
这一题目要求出 `points[]` 点数组中，有多少个不同的三元组 `(i, j, k)` ，其中点 `i` 和 `j` 之间的距离与点 `i` 和 `k` 之间的距离相等。如果采用朴素三层循环的做法，在数据范围为 $500$ 时，肯定会超时。

对于每个回旋镖三元组来说，实质上我们在统计「**给定点 `i` 的前提下，与 `i` 距离相等的 `(j, k)` 点排列有多少个**」。通过使用哈希表预处理，可以消除一层循环——遍历循环所有点 `i` ，**预先计算出每个点 `i` 到其余点的距离，并以 `距离 : 个数` 键值对的形式存储在哈希表中**，然后对所有距离个数键值对进行排列计数。

为了方便哈希表的存储和查询，避免使用 `sqrt` 函数带来的精度损失，这里使用 $x^2 + y^2$ 代指两点间的距离。整个算法的时间复杂度为 $O(n^2)$ ，空间复杂度为 $O(n)$ ：
```cpp
//C++ version
class Solution {
public:
    int numberOfBoomerangs(vector<vector<int>>& points) {
        if (points.size() == 1) return 0;
        int ans = 0;
        unordered_map<int, int> rec;
        for (int i = 0, n = points.size(); i < n; ++i) {
            //对于points[i],计算其他点和它之间的距离,以x^2+y^2为距离
            int x1 = points[i][0], y1 = points[i][1];
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                int x2 = points[j][0], y2 = points[j][1];
                int dist = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
                ++rec[dist];
            }
            //如果有k个点和points[i]距离相同,则回旋镖数量+A(k,2)
            for (const auto &it : rec)
                if (it.second >= 2) ans += it.second * (it.second - 1);
            rec.clear();
        }
        return ans;
    }
};
//执行用时：404 ms, 在所有 C++ 提交中击败了97.17% 的用户
//内存消耗：83.2 MB, 在所有 C++ 提交中击败了92.22% 的用户
```
