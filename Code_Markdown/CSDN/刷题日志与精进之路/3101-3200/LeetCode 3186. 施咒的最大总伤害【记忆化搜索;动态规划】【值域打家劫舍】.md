> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

一个魔法师有许多不同的咒语。

给你一个数组 `power` ，其中每个元素表示一个咒语的伤害值，可能会有多个咒语有相同的伤害值。

已知魔法师使用伤害值为 `power[i]` 的咒语时，他们就 **不能** 使用伤害为 `power[i] - 2` ，`power[i] - 1` ，`power[i] + 1` 或者 `power[i] + 2` 的咒语。

每个咒语最多只能被使用 **一次** 。

请你返回这个魔法师可以达到的伤害值之和的 **最大值** 。

**示例 1：**
```c
输入：power = [1,1,3,4]
输出：6
解释：
可以使用咒语 0，1，3，伤害值分别为 1，1，4，总伤害值为 6 。
```
**示例 2：**
```c
输入：power = [7,1,6,6]
输出：13
解释：
可以使用咒语 1，2，3，伤害值分别为 1，6，6，总伤害值为 13 。
```
**提示：**
- `1 <= power.length <= 10^5`
- `1 <= power[i] <= 10^9`
---
### 相似题目
本题和[740. 删除并获得点数](https://leetcode.cn/problems/delete-and-earn/)几乎一样，都是**值域**上的[198. 打家劫舍](https://leetcode.cn/problems/house-robber/)。
### 方法一 记忆化搜索
统计每个元素的出现次数，记到哈希表 $cnt$ 中。将哈希表的 $key$ 整理到数组 $a$ 中，将 $a$ 按照从小到大的顺序排序。

定义 $dfs(i)$ 表示从 $a[0]$ 到 $a[i]$ 中选择，可以得到的伤害值之和的最大值。

考虑 $a[i]$ 选或不选：
- 不选：问题变成从 $a[0]$ 到 $a[i - 1]$ 中选择，可以得到的伤害值之和的最大值，即 $dfs(i) = dfs(i - 1)$ 。
- 选：那么伤害值等于 $a[i] - 2$ 和 $a[i] - 1$ 的数不能选，问题变成从 $a[0]$ 到 $a[j - 1]$ 中选择，可以得到的伤害值之和的最大值，其中 $j$ 是最小的满足 $a[j] \ge a[i] - 2$ 的数。那么 $dfs(i) = dfs(j - 1) + a[i] \cdot cnt[a[i]]$ 。

两种情况取最大值，得：$$dfs(i) = \max(dfs(i - 1), dfs(j - 1) + a[i]\cdot cnt[a[i]])$$
递归边界：$dfs(-1) = 0$ ，没有数可以选，伤害值之和为 $0$ 。

递归入口：$dfs(n - 1)$ ，即答案。注意这里 $n$ 是 $a$ 的长度，即 $power$ 中不同元素个数。

代码实现时，$j$ 的计算可用二分查找，也可暴力倒序循环查找。
```cpp
class Solution {
public:
    long long maximumTotalDamage(vector<int>& power) {
        unordered_map<int, int> cnt;
        for (int x : power) {
            cnt[x]++;
        }

        vector<pair<int, int>> a(cnt.begin(), cnt.end());
        ranges::sort(a);

        int n = a.size();
        vector<long long> memo(n, -1); // -1 表示没有计算过
        auto dfs = [&](auto&& dfs, int i) -> long long {
            if (i < 0) {
                return 0;
            }
            long long& res = memo[i]; // 注意这里是引用
            if (res != -1) { // 之前计算过
                return res;
            }

            auto& [x, c] = a[i];
            int j = i;
            while (j && a[j - 1].first >= x - 2) {
                j--;
            }
            return res = max(dfs(dfs, i - 1), dfs(dfs, j - 1) + (long long) x * c);
        };
        return dfs(dfs, n - 1);
    }
};
```
```java
class Solution {
    public long maximumTotalDamage(int[] power) {
        Map<Integer, Integer> cnt = new HashMap<>();
        for (int x : power) {
            cnt.merge(x, 1, Integer::sum); // cnt[x]++
        }

        int n = cnt.size();
        int[] a = new int[n];
        int k = 0;
        for (int x : cnt.keySet()) {
            a[k++] = x;
        }
        Arrays.sort(a);

        long[] memo = new long[n];
        Arrays.fill(memo, -1); // -1 表示没有计算过
        return dfs(a, cnt, memo, n - 1);
    }

    private long dfs(int[] a, Map<Integer, Integer> cnt, long[] memo, int i) {
        if (i < 0) {
            return 0;
        }
        if (memo[i] != -1) { // 之前计算过
            return memo[i];
        }

        int x = a[i];
        int j = i;
        while (j > 0 && a[j - 1] >= x - 2) {
            j--;
        }
        return memo[i] = Math.max(dfs(a, cnt, memo, i - 1), dfs(a, cnt, memo, j - 1) + (long) x * cnt.get(x));
    }
}
```
```python
class Solution:
    def maximumTotalDamage(self, power: List[int]) -> int:
        cnt = Counter(power)
        a = sorted(cnt)

        @cache  # 缓存装饰器，避免重复计算 dfs（一行代码实现记忆化）
        def dfs(i: int) -> int:
            if i < 0:
                return 0
            x = a[i]
            j = i
            while j and a[j - 1] >= x - 2:
                j -= 1
            return max(dfs(i - 1), dfs(j - 1) + x * cnt[x])

        return dfs(len(a) - 1)
```
```go
func maximumTotalDamage(power []int) int64 {
	cnt := map[int]int{}
	for _, x := range power {
		cnt[x]++
	}

	n := len(cnt)
	a := make([]int, 0, n)
	for x := range cnt {
		a = append(a, x)
	}
	slices.Sort(a)

	memo := make([]int, n)
	for i := range memo {
		memo[i] = -1 // -1 表示没有计算过
	}
	var dfs func(int) int
	dfs = func(i int) int {
		if i < 0 {
			return 0
		}
		p := &memo[i]
		if *p != -1 { // 之前计算过
			return *p
		}

		x := a[i]
		j := i
		for j > 0 && a[j-1] >= x-2 {
			j--
		}
		*p = max(dfs(i-1), dfs(j-1)+x*cnt[x])
		return *p
	}
	return int64(dfs(n - 1))
}
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 是 $power$ 的长度。瓶颈在排序上，记忆化搜索的时间复杂度为 $O(n)$ 。
- 空间复杂度：$O(n)$ 。
### 方法二 1:1翻译成递推
我们可以去掉递归中的【递】，只保留【归】的部分，即自底向上计算。

具体来说，$f[i]$ 的定义和 $dfs(i)$ 的定义是一样的，都表示从 $a[0]$ 到 $a[i]$ 中选择，可以得到的伤害值之和的最大值。

相应的递推式（状态转移方程）也和 $dfs$ 一样：$$f[i] = \max(f[i - 1], f[j - 1] + a[i] \cdot cnt[a[i]])$$
但是，这种定义方式**没有状态能表示递归边界**，即 $i = -1$ 的情况。

解决办法：在 $f$ 数组的最左边插入一个 $0$ ，那么其余状态全部向右偏移一位，把 $f[i]$ 改为 $f[i + 1]$ 。

修改后 $f[i + 1]$ 表示从 $a[0]$ 到 $a[i]$ 中选择，可以得到的伤害值之和的最大值。此时 $f[0]$ 就对应递归边界了。

修改后的递推式为：$$f[i + 1] = \max(f[i], f[j] + a[i] \cdot cnt[a[i]])$$
> 问：为什么 $a$ 的下标不用变？
> 答：既然是在 $f$ 的最左边插入一个状态，那么就只需要修改和 $f$ 有关的下标，其余任何逻辑都无需修改。或者说，如果把 $a[i]$ 也改成 $a[i + 1]$ ，那么 $a[0]$ 就被我们忽略掉了。

初始值 $f[0] = 0$ ，翻译自递归边界 $dfs(-1) = 0$ 。

答案为 $f[n]$ ，翻译自递归入口 $dfs(n - 1)$ 。

代码实现时，$j$ 的计算可以用二分查找，也可暴力循环查找，也可用双指针，**双指针最快且适用性更广**（把题目的 $2$ 改成 $k$ 也可以过）。
```cpp
class Solution {
public:
    long long maximumTotalDamage(vector<int>& power) {
        unordered_map<int, int> cnt;
        for (int x : power) {
            cnt[x]++;
        }

        vector<pair<int, int>> a(cnt.begin(), cnt.end());
        ranges::sort(a);

        int n = a.size();
        vector<long long> f(n + 1);
        for (int i = 0, j = 0; i < n; i++) {
            auto& [x, c] = a[i];
            while (a[j].first < x - 2) {
                j++;
            }
            f[i + 1] = max(f[i], f[j] + (long long) x * c);
        }
        return f[n];
    }
};
```
```java
class Solution {
    public long maximumTotalDamage(int[] power) {
        Map<Integer, Integer> cnt = new HashMap<>();
        for (int x : power) {
            cnt.merge(x, 1, Integer::sum); // cnt[x]++
        }

        int n = cnt.size();
        int[] a = new int[n];
        int k = 0;
        for (int x : cnt.keySet()) {
            a[k++] = x;
        }
        Arrays.sort(a);

        long[] f = new long[n + 1];
        int j = 0;
        for (int i = 0; i < n; i++) {
            int x = a[i];
            while (a[j] < x - 2) {
                j++;
            }
            f[i + 1] = Math.max(f[i], f[j] + (long) x * cnt.get(x));
        }
        return f[n];
    }
}
```
```python
class Solution:
    def maximumTotalDamage(self, power: List[int]) -> int:
        cnt = Counter(power)
        a = sorted(cnt)
        f = [0] * (len(a) + 1)
        j = 0
        for i, x in enumerate(a):
            while a[j] < x - 2:
                j += 1
            f[i + 1] = max(f[i], f[j] + x * cnt[x])
        return f[-1]
```
```go
func maximumTotalDamage(power []int) int64 {
	cnt := map[int]int{}
	for _, x := range power {
		cnt[x]++
	}

	n := len(cnt)
	a := make([]int, 0, n)
	for x := range cnt {
		a = append(a, x)
	}
	slices.Sort(a)

	f := make([]int, n+1)
	j := 0
	for i, x := range a {
		for a[j] < x-2 {
			j++
		}
		f[i+1] = max(f[i], f[j]+x*cnt[x])
	}
	return int64(f[n])
}
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 是 $power$ 的长度。瓶颈在排序上。双指针的时间复杂度为 $O(n)$ ，因为 $j++$ 至多执行 $O(n)$ 次。
- 空间复杂度：$O(n)$ 。