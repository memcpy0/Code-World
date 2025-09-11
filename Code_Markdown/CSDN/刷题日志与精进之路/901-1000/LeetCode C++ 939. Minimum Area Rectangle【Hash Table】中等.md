<p>Given a set of points in the xy-plane, determine the minimum area of a rectangle formed from these points, with sides parallel to the x and y axes.</p>

<p>If there isn't any rectangle, return 0.</p>

 

 
<p><strong>Example 1:</strong></p>

```clike
Input: [[1,1],[1,3],[3,1],[3,3],[2,2]]
Output: 4
```
<p><strong>Example 2:</strong></p>

```clike
Input: [[1,1],[1,3],[3,1],[3,3],[4,1],[4,3]]
Output: 2
```

<p><strong>Note</strong>:</p>
<ol>
	<li><code>1 &lt;= points.length &lt;= 500</code></li>
	<li><code>0 &lt;=&nbsp;points[i][0] &lt;=&nbsp;40000</code></li>
	<li><code>0 &lt;=&nbsp;points[i][1] &lt;=&nbsp;40000</code></li>
	<li>All points are distinct.</li>
</ol>
</div>
</div>

题意：给定在 `xy` 平面上的一组点，确定由这些点组成的矩形的最小面积，其中矩形的边平行于 `x` 轴和 `y` 轴。如果没有任何矩形，就返回 `0` 。

---
### 解法 哈希集合
如果暴力遍历整个数组，枚举每个四元组，看能不能形成矩形，能的话计算面积……$O(n^4)$ 的时间复杂度，太慢了。

不过题目中有说明，这些矩形的边都平行于X轴和Y轴，因此可以很简单地枚举左上和右下对角点，然后查询左下和右上对角点是否在哈希集合中，然后计算面积，最后取这些面积中的最小值。

说起来很简单，不过实现上有点麻烦。`unordered_map, unordered_set` 都没有提供 `vector<int>` 和 `pair<int, int>` 的接口，因此我们必须自己提供相应的哈希方法（一个函数对象/仿函数）。具体来说，就是要构造一个哈希值，**唯一地表示**每个点。由于点值的范围是 `[0, 40000]` ，我们将其视为一个 `40001` 进制的数，用 `x * 40001 + y` 就可以唯一地表示每个点 `(x, y)` 。

此外需要注意的是，左上角和右下角的点并非所有时候都可行，比如当这两个点同行或者同列时，会形成一条直线，这种情况需要排除。
```cpp
class Solution {
private:
    struct pair_hash {
        int operator()(const pair<int, int>& a) const {
            return a.first * 40001 + a.second;
        }
    }; 
public:
    int minAreaRect(vector<vector<int>>& points) {
        using point = pair<int, int>;
        unordered_set<point, pair_hash> rec;
        for (const vector<int>& p : points) rec.insert(make_pair(p[0], p[1]));
        int n = points.size(), ans = INT_MAX;
        bool flag = false;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                const vector<int> &lu = points[i], &rd = points[j];
                if (lu[0] == rd[0] || lu[1] == rd[1]) continue; //不能同行同列
                const point &&ld = point{lu[0], rd[1]}, &&ru = point{rd[0], lu[1]};
                if (rec.find(ld) != rec.end() && rec.find(ru) != rec.end()) {
                    flag = true; //有矩形
                    int width = abs(rd[0] - lu[0]), height = abs(lu[1] - rd[1]);
                    if (width * height < ans) ans = width * height;
                }
            }
        }
        return flag ? ans : 0;
    }
};
```
运行效率如下：
```cpp
执行用时：352 ms, 在所有 C++ 提交中击败了77.14% 的用户
内存消耗：16.9 MB, 在所有 C++ 提交中击败了72.86% 的用户
```
