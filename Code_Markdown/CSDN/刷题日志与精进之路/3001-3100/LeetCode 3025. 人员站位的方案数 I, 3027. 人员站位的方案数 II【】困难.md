> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个  `n x 2` 的二维数组 `points` ，它表示二维平面上的一些点坐标，其中 `points[i] = [xi, yi]` 。

我们定义 x 轴的正方向为 **右** （**x 轴递增的方向**），x 轴的负方向为 **左** （**x 轴递减的方向**）。类似的，我们定义 y 轴的正方向为 **上** （**y 轴递增的方向**），y 轴的负方向为 **下** （**y 轴递减的方向**）。

你需要安排这 `n` 个人的站位，这 `n` 个人中包括 liupengsay 和小羊肖恩 。你需要确保每个点处 **恰好** 有 **一个** 人。同时，liupengsay 想跟小羊肖恩单独玩耍，所以 liupengsay 会以 liupengsay 的坐标为 **左上角** ，小羊肖恩的坐标为 **右下角** 建立一个矩形的围栏（**注意**，围栏可能 **不** 包含任何区域，也就是说围栏可能是一条线段）。如果围栏的 **内部** 或者 **边缘** 上有任何其他人，liupengsay 都会难过。

请你在确保 liupengsay **不会** 难过的前提下，返回 liupengsay 和小羊肖恩可以选择的 **点对** 数目。

**注意**，liupengsay 建立的围栏必须确保 liupengsay 的位置是矩形的左上角，小羊肖恩的位置是矩形的右下角。比方说，以 `(1, 1)` ，`(1, 3)` ，`(3, 1)` 和 `(3, 3)` 为矩形的四个角，给定下图的两个输入，liupengsay 都不能建立围栏，原因如下：

- 图一中，liupengsay 在 `(3, 3)` 且小羊肖恩在 `(1, 1)` ，liupengsay 的位置不是左上角且小羊肖恩的位置不是右下角。
- 图二中，liupengsay 在 `(1, 3)` 且小羊肖恩在 `(1, 1)` ，小羊肖恩的位置不是在围栏的右下角。

![](https://assets.leetcode.com/uploads/2024/01/04/example0alicebob-1.png)

**示例 1：**

![](https://assets.leetcode.com/uploads/2024/01/04/example1alicebob.png)

**输入：**points = [[1,1],[2,2],[3,3]]
**输出：**0
**解释：**没有办法可以让 liupengsay 的围栏以 liupengsay 的位置为左上角且小羊肖恩的位置为右下角。所以我们返回 0 。

**示例 2：**

**[![](https://pic.leetcode.cn/1706880313-YelabI-example2.jpeg)](https://pic.leetcode.cn/1706880313-YelabI-example2.jpeg)**

**输入：**points = [[6,2],[4,4],[2,6]]
**输出：**2
**解释：**总共有 2 种方案安排 liupengsay 和小羊肖恩的位置，使得 liupengsay 不会难过：
- liupengsay 站在 (4, 4) ，小羊肖恩站在 (6, 2) 。
- liupengsay 站在 (2, 6) ，小羊肖恩站在 (4, 4) 。
不能安排 liupengsay 站在 (2, 6) 且小羊肖恩站在 (6, 2) ，因为站在 (4, 4) 的人处于围栏内。

**示例 3：**

**[![](https://pic.leetcode.cn/1706880311-mtPGYC-example3.jpeg)](https://pic.leetcode.cn/1706880311-mtPGYC-example3.jpeg)**

**输入：**points = [[3,1],[1,3],[1,1]]
**输出：**2
**解释：**总共有 2 种方案安排 liupengsay 和小羊肖恩的位置，使得 liupengsay 不会难过：
- liupengsay 站在 (1, 1) ，小羊肖恩站在 (3, 1) 。
- liupengsay 站在 (1, 3) ，小羊肖恩站在 (1, 1) 。
不能安排 liupengsay 站在 (1, 3) 且小羊肖恩站在 (3, 1) ，因为站在 (1, 1) 的人处于围栏内。
注意围栏是可以不包含任何面积的，上图中第一和第二个围栏都是合法的。

**提示：**

- `2 <= n <= 50`
- `points[i].length == 2`
- `0 <= points[i][0], points[i][1] <= 50`
- `points[i]` 点对两两不同。

和周赛第四题是一样的，[题解点我](https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solution/on2-you-ya-mei-ju-by-endlesscheng-z86d/)。

---
给你一个  `n x 2` 的二维数组 `points` ，它表示二维平面上的一些点坐标，其中 `points[i] = [xi, yi]` 。

我们定义 x 轴的正方向为 **右** （**x 轴递增的方向**），x 轴的负方向为 **左** （**x 轴递减的方向**）。类似的，我们定义 y 轴的正方向为 **上** （**y 轴递增的方向**），y 轴的负方向为 **下** （**y 轴递减的方向**）。

你需要安排这 `n` 个人的站位，这 `n` 个人中包括 liupengsay 和小羊肖恩 。你需要确保每个点处 **恰好** 有 **一个** 人。同时，liupengsay 想跟小羊肖恩单独玩耍，所以 liupengsay 会以 liupengsay 的坐标为 **左上角** ，小羊肖恩的坐标为 **右下角** 建立一个矩形的围栏（**注意**，围栏可能 **不** 包含任何区域，也就是说围栏可能是一条线段）。如果围栏的 **内部** 或者 **边缘** 上有任何其他人，liupengsay 都会难过。

请你在确保 liupengsay **不会** 难过的前提下，返回 liupengsay 和小羊肖恩可以选择的 **点对** 数目。

**注意**，liupengsay 建立的围栏必须确保 liupengsay 的位置是矩形的左上角，小羊肖恩的位置是矩形的右下角。比方说，以 `(1, 1)` ，`(1, 3)` ，`(3, 1)` 和 `(3, 3)` 为矩形的四个角，给定下图的两个输入，liupengsay 都不能建立围栏，原因如下：

- 图一中，liupengsay 在 `(3, 3)` 且小羊肖恩在 `(1, 1)` ，liupengsay 的位置不是左上角且小羊肖恩的位置不是右下角。
- 图二中，liupengsay 在 `(1, 3)` 且小羊肖恩在 `(1, 1)` ，小羊肖恩的位置不是在围栏的右下角。

![](https://assets.leetcode.com/uploads/2024/01/04/example0alicebob-1.png)

**示例 1：**

![](https://assets.leetcode.com/uploads/2024/01/04/example1alicebob.png)

**输入：**points = [[1,1],[2,2],[3,3]]
**输出：**0
**解释：**没有办法可以让 liupengsay 的围栏以 liupengsay 的位置为左上角且小羊肖恩的位置为右下角。所以我们返回 0 。

**示例 2：**

**[![](https://pic.leetcode.cn/1706880313-YelabI-example2.jpeg)](https://pic.leetcode.cn/1706880313-YelabI-example2.jpeg)**

**输入：**points = [[6,2],[4,4],[2,6]]
**输出：**2
**解释：**总共有 2 种方案安排 liupengsay 和小羊肖恩的位置，使得 liupengsay 不会难过：
- liupengsay 站在 (4, 4) ，小羊肖恩站在 (6, 2) 。
- liupengsay 站在 (2, 6) ，小羊肖恩站在 (4, 4) 。
不能安排 liupengsay 站在 (2, 6) 且小羊肖恩站在 (6, 2) ，因为站在 (4, 4) 的人处于围栏内。

**示例 3：**

**[![](https://pic.leetcode.cn/1706880311-mtPGYC-example3.jpeg)](https://pic.leetcode.cn/1706880311-mtPGYC-example3.jpeg)**

**输入：**points = [[3,1],[1,3],[1,1]]
**输出：**2
**解释：**总共有 2 种方案安排 liupengsay 和小羊肖恩的位置，使得 liupengsay 不会难过：
- liupengsay 站在 (1, 1) ，小羊肖恩站在 (3, 1) 。
- liupengsay 站在 (1, 3) ，小羊肖恩站在 (1, 1) 。
不能安排 liupengsay 站在 (1, 3) 且小羊肖恩站在 (3, 1) ，因为站在 (1, 1) 的人处于围栏内。
注意围栏是可以不包含任何面积的，上图中第一和第二个围栏都是合法的。

**提示：**

- `2 <= n <= 1000`
- `points[i].length == 2`
- `-109 <= points[i][0], points[i][1] <= 109`
- `points[i]` 点对两两不同。

---

### 解法 O(n^2) 优雅枚举
将 points\textit{points}points 按照横坐标从小到大排序，横坐标相同的，按照纵坐标从大到小排序。

如此一来，在枚举 points[i]\textit{points}[i]points[i] 和 points[j]\textit{points}[j]points[j] 时（i<ji<ji<j），就只需要关心纵坐标的大小。

固定 points[i]\textit{points}[i]points[i]，然后枚举 points[j]\textit{points}[j]points[j]：

如果 points[j][1]\textit{points}[j][1]points[j][1] 比之前枚举的点的纵坐标都大，那么矩形内没有其它点，符合要求，答案加一。
如果 points[j][1]\textit{points}[j][1]points[j][1] 小于等于之前枚举的某个点的纵坐标，那么矩形内有其它点，不符合要求。
所以在枚举 points[j]\textit{points}[j]points[j] 的同时，需要维护纵坐标的最大值 maxY\textit{maxY}maxY。这也解释了为什么横坐标相同的，按照纵坐标从大到小排序。这保证了横坐标相同时，我们总是优先枚举更靠上的点，不会误把包含其它点的矩形也当作符合要求的矩形。

Python3
Java
C++
Go
```cpp
class Solution:
    def numberOfPairs(self, points: List[List[int]]) -> int:
        points.sort(key=lambda p: (p[0], -p[1]))
        ans = 0
        for i, (_, y0) in enumerate(points):
            max_y = -inf
            for (_, y) in points[i + 1:]:
                if max_y < y <= y0:
                    max_y = y
                    ans += 1
        return ans

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solutions/2630655/on2-you-ya-mei-ju-by-endlesscheng-z86d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
    public int numberOfPairs(int[][] points) {
        Arrays.sort(points, (p, q) -> p[0] != q[0] ? p[0] - q[0] : q[1] - p[1]);
        int ans = 0;
        for (int i = 0; i < points.length; i++) {
            int y0 = points[i][1];
            int maxY = Integer.MIN_VALUE;
            for (int j = i + 1; j < points.length; j++) {
                int y = points[j][1];
                if (y <= y0 && y > maxY) {
                    maxY = y;
                    ans++;
                }
            }
        }
        return ans;
    }
}


作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solutions/2630655/on2-you-ya-mei-ju-by-endlesscheng-z86d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

class Solution {
public:
    int numberOfPairs(vector<vector<int>> &points) {
        ranges::sort(points, [](const auto &p, const auto &q) {
            return p[0] != q[0] ? p[0] < q[0] : p[1] > q[1];
        });
        int ans = 0, n = points.size();
        for (int i = 0; i < n; i++) {
            int y0 = points[i][1];
            int max_y = INT_MIN;
            for (int j = i + 1; j < n; j++) {
                int y = points[j][1];
                if (y <= y0 && y > max_y) {
                    max_y = y;
                    ans++;
                }
            }
        }
        return ans;
    }
};

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solutions/2630655/on2-you-ya-mei-ju-by-endlesscheng-z86d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

func numberOfPairs(points [][]int) (ans int) {
	slices.SortFunc(points, func(p, q []int) int {
		if p[0] != q[0] {
			return p[0] - q[0]
		}
		return q[1] - p[1]
	})
	for i, p := range points {
		y0 := p[1]
		maxY := math.MinInt
		for _, q := range points[i+1:] {
			y := q[1]
			if y <= y0 && y > maxY {
				maxY = y
				ans++
			}
		}
	}
	return
}

作者：灵茶山艾府
链接：https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solutions/2630655/on2-you-ya-mei-ju-by-endlesscheng-z86d/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```
复杂度分析
时间复杂度：O(n2)\mathcal{O}(n^2)O(n 
2
 )，其中 nnn 为 points\textit{points}points 的长度。
空间复杂度：O(1)\mathcal{O}(1)O(1)。忽略排序的栈开销和 Python 切片开销。

这题定7分本意很可能是二维差分前缀和+离散化，但存在某一维坐标相同的点会导致枚举顺序影响答案，这点用枚举所有点对的做法也是可处理的……不过确实不好想，通过率这么高很大程度是比赛期间case太弱的锅……
python 离散化+二维前缀和

Code
Python3
class Solution:
    def numberOfPairs(self, points: List[List[int]]) -> int:
        srow,scol=set(),set()
        d=defaultdict()
        for i,j in points:
            srow.add(i)
            scol.add(j)
        brow,bcol=sorted(list(srow)),sorted(list(scol))
        nr,nc=len(brow),len(bcol)
        m=[[0]*nc for _ in range(nr)]
        for i,j in points:#离散化
            bi=bisect_left(brow,i)
            bj=bisect_left(bcol,j)
            d[(i,j)]=(bi,bj)
            m[bi][bj]+=1
        temp=[[0]*(nc+1) for _ in range(nr+1)]#二维前缀和
        for i in range(nr):
            for j in range(nc):
                temp[i+1][j+1]=temp[i][j+1]+temp[i+1][j]-temp[i][j]+m[i][j]
        n=len(points)
        ans=0
        for i in range(n):#二维前缀和等于2代表只有两个点
            for j in range(n):
                if i!=j and points[i][0]<=points[j][0] and points[i][1]>=points[j][1]:
                    r1,c1=d[(points[i][0],points[i][1])]
                    r2,c2=d[(points[j][0],points[j][1])]
                    if temp[r2+1][c1+1]-temp[r1][c1+1]-temp[r2+1][c2]+temp[r1][c2]==2:
                        ans+=1
        return ans

作者：cheturtle
链接：https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solutions/2630756/python-chi-san-hua-er-wei-qian-zhui-he-b-tabm/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。


[加强版](https://leetcode.cn/link/?target=https%3A%2F%2Fcodeforces.com%2Fproblemsets%2Facmsguru%2Fproblem%2F99999%2F512)  512. Friendly Points

### cdq 分治 O(N * log^2 N)
思路
首先看懂 O(N2)O(N^2)O(N 
2
 ) 排序做法（可以参考灵神题解

然后考虑优化

解题方法
考虑排序后的一对下标 i<ji \lt ji<j 可以统计进答案，当且仅当

yi≥yjy_i \geq y_jy 
i
​
 ≥y 
j
​
 
不存在下标 kkk，满足 i<k<ji \lt k \lt ji<k<j，且 yi≥yk≥yjy_i \geq y_k \geq y_jy 
i
​
 ≥y 
k
​
 ≥y 
j
​
 
在 cdq 分治后，统计分治中心两侧的合法点对

对于当前区间，将所有点按照 yyy 从大到小排序后遍历（yyy 相等时优先左侧

对于分治中心左侧的点 iii，必须满足右下角区域没有点，因此可用的点形成一个上凸壳

可使用单调栈维护
对于分治中心右侧的点 jjj，需找出分治中心到 jjj 之间，已遍历的 yyy 的最小值，记为 boundboundbound

可使用树状数组维护
此时合法的 iii 为，左侧凸壳上，yiy_iy 
i
​
  严格小于 boundboundbound 的点数，可使用二分计算
因此对于每个区间，统计维护的复杂度为 O(NlogN)O(N log N)O(NlogN)

复杂度
时间复杂度:

O(Nlog2N)O(N log^2 N)O(Nlog 
2
 N)

空间复杂度:

O(NlogN)O(N log N)O(NlogN)

Code
C++
class Solution {
public:
    int numberOfPairs(vector<vector<int>>& points) {
      sort(points.begin(), points.end(), [&](const auto &p, const auto &q) {
        return p[0] != q[0]? p[0] < q[0]: p[1] > q[1];
      });
      long long ret = 0;
      function<void(int, int)> cdq = [&](int l, int r) {
        if (l == r) return;
        int mid = (l + r) / 2;
        cdq(l, mid);
        vector<int> id(r - l + 1);
        iota(id.begin(), id.end(), l);
        sort(id.begin(), id.end(), [&](int i, int j) {
          if (points[i][1] != points[j][1]) return points[i][1] > points[j][1];
          return i < j;
        });
        deque<int> deq;
        auto binary_search = [&](int bound) {
          int l = 0, r = deq.size();
          while (l < r) {
            int mid = (l + r) / 2;
            if (points[deq[mid]][1] >= bound) r = mid;
            else l = mid + 1;
          }
          return r;
        };
        vector<int> c(r - mid + 1, INT_MAX);
        auto update = [&](int i, int x) {
          for (; i <= r - mid; i += i & -i) c[i] = min(c[i], x);
        };
        auto query = [&](int i) {
          int ret = INT_MAX;
          for (; i; i -= i & -i) ret = min(ret, c[i]);
          return ret;
        };
        for (int i: id) {
          if (i <= mid) {
            while (!deq.empty() && deq.front() < i) deq.pop_front();
            deq.push_front(i);
          } else {
            ret += binary_search(query(i - mid));
            update(i - mid, points[i][1]);
          }
        }
        cdq(mid + 1, r);
      };
      cdq(0, (int)points.size() - 1);
      return ret;
    }
};


### 扫描线+平衡树动态维护下台阶状的结构就一个 log 了
另外有个 O(nlog⁡n)O(n \log n)O(nlogn) 的做法，是用 orthogonal range skyline counting。论文见这里：Optimal Planar Orthogonal Skyline Counting Queries

作者：hqztrue
链接：https://leetcode.cn/problems/find-the-number-of-ways-to-place-people-ii/solutions/2633074/bao-li-yi-ji-on-log-nde-suan-fa-by-hqztr-jwpi/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。