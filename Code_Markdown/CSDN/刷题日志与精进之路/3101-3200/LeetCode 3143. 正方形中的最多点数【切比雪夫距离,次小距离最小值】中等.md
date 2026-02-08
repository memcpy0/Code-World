> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个二维数组 `points` 和一个字符串 `s` ，其中 `points[i]` 表示第 `i` 个点的坐标，`s[i]` 表示第 `i` 个点的 **标签** 。

如果一个正方形的中心在 `(0, 0)` ，所有边都平行于坐标轴，且正方形内 **不** 存在标签相同的两个点，那么我们称这个正方形是 **合法** 的。

请你返回 **合法** 正方形中可以包含的 **最多** 点数。

**注意：**
- 如果一个点位于正方形的边上或者在边以内，则认为该点位于正方形内。
- 正方形的边长可以为零。

**示例 1：**
![](https://assets.leetcode.com/uploads/2024/03/29/3708-tc1.png)
```rust
输入：points = [[2,2],[-1,-2],[-4,4],[-3,1],[3,-3]], s = "abdca"
输出：2
解释：
边长为 4 的正方形包含两个点 `points[0]` 和 `points[1]` 。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2024/03/29/3708-tc2.png)
```cpp
输入：points = [[1,1],[-2,-2],[-2,2]], s = "abb"
输出：1
解释：
边长为 2 的正方形包含 1 个点 `points[0]` 。
```
**示例 3：**
```rust
输入：points = [[1,1],[-1,-1],[2,-2]], s = "ccd"
输出：0
解释：
任何正方形都无法只包含 `points[0]` 和 `points[1]` 中的一个点，所以合法正方形中都不包含任何点。
```
**提示：**
- `1 <= s.length, points.length <= 10^5`
- `points[i].length == 2`
- `-10^9 <= points[i][0], points[i][1] <= 10^9`
- `s.length == points.length`
- `points` 中的点坐标互不相同。
- `s` 只包含小写英文字母。

---
### 解法1 二分间接值
由于正方形边长越大，越不合法，**有单调性**，所以可以二分边长的一半。

在二分中统计遇到的字符，如果没有遇到重复的字符，说明正方形合法，用字符个数更新答案的最大值。
```python
class Solution:
    def maxPointsInsideSquare(self, points: List[List[int]], s: str) -> int:
        ans = 0
        def check(size: int) -> bool:
            vis = set()
            for (x, y), c in zip(points, s):
                if abs(x) <= size and abs(y) <= size:  # 点在正方形中
                    if c in vis:
                        return True
                    vis.add(c)
            nonlocal ans
            ans = len(vis)
            return False
        # 注意 range 并不会创建 list，它是 O(1) 的
        bisect_left(range(1_000_000_001), True, key=check)
        return ans
```
```cpp
class Solution {
public:
    int maxPointsInsideSquare(vector<vector<int>>& points, string s) {
        int l = 0, r = 1'000'000'000; // 中间到右边长度1e9
        int ans = 0;
        while (l <= r) {
            int mid = (l + r) / 2;
            int vis = 0;
            for (int i = 0, n = points.size(); i < n; ++i) {
                int x = points[i][0], y = points[i][1];
                if (abs(x) > mid || abs(y) > mid) continue;
                int idx = s[i] - 'a';
                if (vis >> idx & 1) {
                    vis = -1;
                    break;
                }
                vis |= (1 << idx);
            }
            if (vis >= 0) {
                l = mid + 1;
                ans = __builtin_popcount(vis); // 二段性
            } else r = mid - 1;
        }
        return ans;
    }
};
```
```java
class Solution {
    private int ans;

    public int maxPointsInsideSquare(int[][] points, String S) {
        char[] s = S.toCharArray();
        int left = -1;
        int right = 1_000_000_001;
        while (left + 1 < right) {
            int mid = (left + right) >>> 1;
            if (check(mid, points, s)) {
                left = mid;
            } else {
                right = mid;
            }
        }
        return ans;
    }

    boolean check(int size, int[][] points, char[] s) {
        int vis = 0;
        for (int i = 0; i < points.length; i++) {
            // 判断点是否在正方形中
            if (Math.abs(points[i][0]) <= size && Math.abs(points[i][1]) <= size) {
                int c = s[i] - 'a';
                if ((vis >> c & 1) > 0) { // c 在集合中
                    return false;
                }
                vis |= 1 << c; // 把 c 加入集合
            }
        }
        ans = Integer.bitCount(vis); // Integer.bitCount方法
        return true;
    }
}
```
```go
func maxPointsInsideSquare(points [][]int, s string) (ans int) {
    sort.Search(1_000_000_001, func(size int) bool {
        vis := 0
        for i, p := range points {
            if abs(p[0]) <= size && abs(p[1]) <= size { // 点在正方形中
                c := s[i] - 'a'
                if vis>>c&1 > 0 { // c 在集合中
                    return true
                }
                vis |= 1 << c // 把 c 加入集合
            }
        }
        ans = bits.OnesCount(uint(vis))
        return false
    })
    return
}

func abs(x int) int { if x < 0 { return -x }; return x }
```
复杂度分析：
- 时间复杂度：$O(n\log U)$ ，其中 $n$ 为 $s$ 的长度，$U=\max(|x_i| , |y_i|)$。
- 空间复杂度：$O(∣Σ∣)$ 或 $O(1)$ 。其中 $∣Σ∣$ 为字符集合的大小，本题字符均为小写字母，所以 $∣Σ∣=26$ 。

---
### 解法二：维护次小距离的最小值+切比雪夫距离
定义点 $(x,y)$ 到 $(0,0)$ 的切比雪夫距离为
$$\max(|x|,|y|)$$
定义 $\min D[c]$ 为标签为 $c$ 的所有点到 $(0,0)$ 的**最小**切比雪夫距离。
定义 $\min D_2[c]$ 为标签为 $c$ 的所有点到 $(0,0)$ 的**次小**切比雪夫距离。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202408040056122.png)
换句话说，可以在正方形中的点需要满足
$$\min D[c]< \min {}_2$$​代码实现时，无需维护 $\min D_2[c]$ ，而是直接维护 $\min {}_2$ 。
```python
class Solution:
    def maxPointsInsideSquare(self, points: List[List[int]], s: str) -> int:
        min_d = defaultdict(lambda: inf)
        min2 = inf
        for (x, y), c in zip(points, s):
            d = max(abs(x), abs(y))
            if d < min_d[c]:
                # d 是目前最小的，那么 min_d[c] 是次小的
                min2 = min(min2, min_d[c])
                min_d[c] = d
            else:
                # d 可能是次小的
                min2 = min(min2, d)
        return sum(d < min2 for d in min_d.values())
```
```java
class Solution {
    public int maxPointsInsideSquare(int[][] points, String s) {
        int[] minD = new int[26];
        Arrays.fill(minD, Integer.MAX_VALUE);
        int min2 = Integer.MAX_VALUE;
        for (int i = 0; i < points.length; i++) {
            int d = Math.max(Math.abs(points[i][0]), Math.abs(points[i][1]));
            int c = s.charAt(i) - 'a';
            if (d < minD[c]) {
                // d 是目前最小的，那么 minD[c] 是次小的
                min2 = Math.min(min2, minD[c]);
                minD[c] = d;
            } else {
                // d 可能是次小的
                min2 = Math.min(min2, d);
            }
        }
        int ans = 0;
        for (int d : minD) {
            if (d < min2) {
                ans++;
            }
        }
        return ans;
    }
}
```
```go
func maxPointsInsideSquare(points [][]int, s string) (ans int) {
    minD := [26]int{}
    for i := range minD {
        minD[i] = math.MaxInt
    }
    min2 := math.MaxInt
    for i, p := range points {
        d := max(abs(p[0]), abs(p[1]))
        c := s[i] - 'a'
        if d < minD[c] {
            // d 是目前最小的，那么 minD[c] 是次小的
            min2 = min(min2, minD[c])
            minD[c] = d
        } else {
            // d 可能是次小的
            min2 = min(min2, d)
        }
    }
    for _, d := range minD {
        if d < min2 {
            ans++
        }
    }
    return
}

func abs(x int) int { if x < 0 { return -x }; return x }
```
```cpp
class Solution {
public:
    int maxPointsInsideSquare(vector<vector<int>>& points, string s) {
        int minD[26], min2 = INT_MAX;
        ranges::fill(minD, INT_MAX);
        for (int i = 0; i < points.size(); ++i) {
            int d = max(abs(points[i][0]), abs(points[i][1]));
            char c = s[i] - 'a';
            if (d < minD[c]) {
                // d是目前最小的，那么minD[c]是次小的
                min2 = min(min2, minD[c]); // 次小距离的最小值
                minD[c] = d;
            } else {
                // d 可能是次小的
                min2 = min(min2, d);
            }
        }
        int ans = 0;
        for (int d : minD) ans += d < min2;
        return ans;
    }
};
```
**复杂度分析**：
- 时间复杂度：$O(n+|Σ|)$ ，其中 $n$ 为 $s$ 的长度，$|Σ|$ 为字符集合的大小，本题字符均为小写字母，所以 $|Σ|=26$ 。
- 空间复杂度：$O(|Σ|)$ 。
