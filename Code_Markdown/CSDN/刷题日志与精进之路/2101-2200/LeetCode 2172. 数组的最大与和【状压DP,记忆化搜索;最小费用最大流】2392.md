> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的整数数组 `nums` 和一个整数 `numSlots` ，满足`2 * numSlots >= n` 。总共有 `numSlots` 个篮子，编号为 `1` 到 `numSlots` 。

你需要把所有 `n` 个整数分到这些篮子中，且每个篮子 **至多** 有 2 个整数。一种分配方案的 **与和** 定义为每个数与它所在篮子编号的 **按位与运算** 结果之和。
- 比方说，将数字 `[1, 3]` 放入篮子 **_`1`_** 中，`[4, 6]` 放入篮子 **_`2`_** 中，这个方案的与和为 `(1 AND 1) + (3 AND 1) + (4 AND 2) + (6 AND 2) = 1 + 1 + 0 + 2 = 4` 。

请你返回将 `nums` 中所有数放入 `numSlots` 个篮子中的最大与和。

**示例 1：**
```js
输入：nums = [1,2,3,4,5,6], numSlots = 3
输出：9
解释：一个可行的方案是 [1, 4] 放入篮子 1 中，[2, 6] 放入篮子 2 中，[3, 5] 放入篮子 3 中。
最大与和为 (1 AND 1) + (4 AND 1) + (2 AND 2) + (6 AND 2) + (3 AND 3) + (5 AND 3) = 1 + 0 + 2 + 2 + 3 + 1 = 9 。
```
**示例 2：**
```js
输入：nums = [1,3,10,4,7,1], numSlots = 9
输出：24
解释：一个可行的方案是 [1, 1] 放入篮子 1 中，[3] 放入篮子 3 中，[4] 放入篮子 4 中，[7] 放入篮子 7 中，[10] 放入篮子 9 中。
最大与和为 (1 AND 1) + (1 AND 1) + (3 AND 3) + (4 AND 4) + (7 AND 7) + (10 AND 9) = 1 + 1 + 3 + 4 + 7 + 8 = 24 。
注意，篮子 2 ，5 ，6 和 8 是空的，这是允许的。
```
**提示：**
- `n == nums.length`
- `1 <= numSlots <= 9`
- `1 <= n <= 2 * numSlots`
- `1 <= nums[i] <= 15`

---
### 解法 记忆化搜索/状压DP
> 数据范围很小，考虑状压DP。
> <b></b>
> 本题与1879. 两个数组最小的异或值之和【记忆化搜索,状压DP,位运算】2145，有些相似但也不同。通过对题目条件的转换，可以缩小二者之间的差别。

由于每个篮子至多可以放 $2$ 个整数，可以视为**有 $2numSlots$ 个篮子（组成一个数组，该数组中每个元素的值是篮子的原始编号），每个篮子至多可以放 $1$ 个整数（放了整数，才计算该整数与篮子编号的与值）**。

由于篮子个数很少，可以用二进制数 $x$ 表示这 $2 numSlots$ 个篮子中放了数字的篮子集合，其中 $x$ 从低到高的第 $i$ 位为 $1$ ，表示第 $i$ 个篮子放了数字、为 $0$ 表示没有放数字。

$memo[i][j]$ 表示对前 $0\sim i$ 个数，$j$ 代表的篮子可以被选中时的最大与和。记忆化搜索的代码如下：
```cpp
class Solution {
public:
    int maximumANDSum(vector<int>& nums, int numSlots) {
        // memo[i][j] 表示对前0-i个数，j代表的篮子可以被选中时的最大与和
        int n = nums.size();
        vector<vector<int>> memo(n, vector<int>(1 << (numSlots * 2), -1));
        function<int(int, int)> f = [&](int i, int j) -> int {
            if (i < 0) return 0;
            int &ans = memo[i][j];
            if (ans != -1) return ans;
            for (int k = 0; k < (numSlots * 2); ++k) {
                if (j >> k & 1) { // 这个篮子已经被选中
                    ans = max(ans, f(i - 1, j ^ (1 << k)) + (nums[i] & (k / 2 + 1)));
                }
            }
            return ans;
        };
        int ans = f(n - 1, (1 << (numSlots * 2)) - 1);
        return ans;
    }

};
```

设数字 $i$ 的二进制表示中 $1$ 的个数为 $c$ ，定义 $f[i]$ 表示**将 $nums$ 的前 $c$ 个数字放入篮子数组中的篮子里，且放了数字的篮子集合为 $i$ 时的最大与和**。初始值 $f[0] = 0$ 。

考虑 $nums[c]$ 放到一个空篮子中的状态转移方程（下标从 $0$ 开始，此时 $nums[c]$ 还没被放入篮子中），我们可以枚举 $i$ 中的 $0$ ，即空篮子位置 $j$ ，该篮子对应的编号为 $\dfrac{j}{2} +1$ ，则有：
$$f[i + 2^j] = \max( f[i + 2^j],\ f[i] + (\dfrac{j}{2} + 1)\ \&\ nums[c])$$
设 $\textit{nums}$ 的长度为 $n$ ，最后答案为 $\max_{c=n}(f)$ 。
> 代码实现时需要注意，若 $c\ge n$ 则 $f[i]$ 无法转移，需要跳过。 
```cpp
class Solution {
public:
    int maximumANDSum(vector<int> &nums, int numSlots) {
        int ans = 0;
        vector<int> f(1 << (numSlots * 2));
        for (int i = 0; i < f.size(); ++i) {
            int c = __builtin_popcount(i);
            if (c >= nums.size()) continue;
            for (int j = 0; j < numSlots * 2; ++j) {
                if ((i & (1 << j)) == 0) { // 枚举空篮子 j
                    int s = i | (1 << j);
                    f[s] = max(f[s], f[i] + ((j / 2 + 1) & nums[c]));
                    ans = max(ans, f[s]);
                }
            }
        }
        return ans;
    }
};
```
---
### 解法2 最小费用最大流
此外还可用最小费用最大流解决，这其实是一种比较典型的建图技巧。

设集合 $A$ 为数字，集合 $B$ 为篮子，额外建立超级源点和超级汇点：
- 从源点连容量为 $1$ 费用为 $0$ 的边到 $A$ 中各点；
- 从 $B$ 中各点连容量为 $2$ 费用为 $0$ 的边到汇点；
- 从 $A$ 的每个数字 $\textit{nums}[i]$ 向 $B$ 的每个篮子 $j$ 连边，容量为 $+\infty$ ，费用为 $-\textit{nums}[i]\& j$ ，取负号是为了求最小费用最大流。

这样跑最小费用最大流得到的结果的相反数就是匹配 $A$ 中所有数字的最大花费，即最大与和。时间复杂度 $O(nm(n+m))$ 。Go 的实现：
```go
func maximumANDSum(nums []int, numSlots int) (ans int) {
	const inf int = 1e9

	// 集合 A 和 B 的大小
	n, m := len(nums), numSlots

	// 建图
	type neighbor struct{ to, rid, cap, cost int } // 相邻节点、反向边下标、容量、费用
	g := make([][]neighbor, n+m+2)
	addEdge := func(from, to, cap, cost int) {
		g[from] = append(g[from], neighbor{to, len(g[to]), cap, cost})
		g[to] = append(g[to], neighbor{from, len(g[from]) - 1, 0, -cost})
	}
	start := n + m   // 超级源点
	end := start + 1 // 超级汇点
	for i, num := range nums {
		addEdge(start, i, 1, 0)
		for j := 1; j <= m; j++ {
			addEdge(i, n+j-1, inf, -(num & j))
		}
	}
	for i := 0; i < m; i++ {
		addEdge(n+i, end, 2, 0)
	}

	// 下面为最小费用最大流模板
	dist := make([]int, len(g))
	type vi struct{ v, i int }
	fa := make([]vi, len(g))
	spfa := func() bool {
		for i := range dist {
			dist[i] = inf
		}
		dist[start] = 0
		inQ := make([]bool, len(g))
		inQ[start] = true
		q := []int{start}
		for len(q) > 0 {
			v := q[0]
			q = q[1:]
			inQ[v] = false
			for i, e := range g[v] {
				if e.cap == 0 {
					continue
				}
				w := e.to
				if newD := dist[v] + e.cost; newD < dist[w] {
					dist[w] = newD
					fa[w] = vi{v, i}
					if !inQ[w] {
						q = append(q, w)
						inQ[w] = true
					}
				}
			}
		}
		return dist[end] < inf
	}
	for spfa() {
		// 沿 start-end 的最短路尽量增广
		minFlow := inf
		for v := end; v != start; {
			p := fa[v]
			if c := g[p.v][p.i].cap; c < minFlow {
				minFlow = c
			}
			v = p.v
		}
		for v := end; v != start; {
			p := fa[v]
			e := &g[p.v][p.i]
			e.cap -= minFlow
			g[v][e.rid].cap += minFlow
			v = p.v
		}
		ans -= dist[end] * minFlow
	}
	return
}
```
---
### 解法3 3进制状态压缩
……