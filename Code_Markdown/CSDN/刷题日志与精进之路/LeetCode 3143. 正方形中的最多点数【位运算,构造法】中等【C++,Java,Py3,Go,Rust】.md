> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个整数 `n` 和 `x` 。你需要构造一个长度为 `n` 的 **正整数** 数组 `nums` ，对于所有 `0 <= i < n - 1` ，满足 `nums[i + 1]` **大于** `nums[i]` ，并且数组 `nums` 中所有元素的按位 `AND` 运算结果为 `x` 。

返回 `nums[n - 1]` 可能的 **最小** 值。

**示例 1：**
```c
输入：n = 3, x = 4
输出：6
解释：
数组 `nums` 可以是 `[4,5,6]` ，最后一个元素为 `6` 。
```
**示例 2：**
```c
输入：n = 2, x = 7
输出：15
解释：
数组 `nums` 可以是 `[7,15]` ，最后一个元素为 `15` 。
```
**提示：**
- `1 <= n, x <= 10^8`

---
### 方法：位运算，两种简洁解法
从集合的视角看，$x$ 是每个 $nums[i]$ 的**子集**。换句话说，$nums[i]$ 一定是 $x$ 的**超集**。例如 $x=100100$ ，那么 $nums[i]$ 一定在如下序列中：
$$1\underline{00}1\underline{00}, 1\underline{00}1\underline{01}, 1\underline{00}1\underline{10},  1\underline{00}1\underline{11}, 1\underline{01}1\underline{00}, 1\underline{01}1\underline{01},\ \dots$$
​
**只看下划线上的数，是一个自然数序列**
$$0000,0001,0010,0011,0100,0101,⋯$$
为了让 $nums[n−1]$ 尽量小，我们应当选择 **$x$ 的超集中最小的 $n$ 个数**。

所以**把 $x$ 的二进制中的 $0$ 视作「空位」，往空位上填入 $n−1$** ，即为**最小**的 $nums[n−1]$ 。如果空位不足，往 $x$ 的前面添加前导零即可。
```python
class Solution:
    def minEnd(self, n: int, x: int) -> int:
        n -= 1  # 先把 n 减一，这样下面讨论的 n 就是原来的 n-1
        i = j = 0
        while n >> j:
            # x 的第 i 个比特值是 0，即「空位」
            if (x >> i & 1) == 0:
                # 空位填入 n 的第 j 个比特值
                x |= (n >> j & 1) << i
                j += 1
            i += 1
        return x
```
```java
class Solution {
    public long minEnd(int n, int x) {
        n--; // 先把 n 减一，这样下面讨论的 n 就是原来的 n-1
        long ans = x;
        int i = 0, j = 0;
        while ((n >> j) > 0) {
            // x 的第 i 个比特值是 0，即「空位」
            if ((ans >> i & 1) == 0) {
                // 空位填入 n 的第 j 个比特值
                ans |= (long) (n >> j & 1) << i;
                j++;
            }
            i++;
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    long long minEnd(int n, int x) {
        n--; // 先把 n 减一，这样下面讨论的 n 就是原来的 n-1
        long long ans = x;
        int i = 0, j = 0;
        while (n >> j) {
            // x 的第 i 个比特值是 0，即「空位」
            if ((ans >> i & 1) == 0) {
                // 空位填入 n 的第 j 个比特值
                ans |= (long long) (n >> j & 1) << i;
                j++;
            }
            i++;
        }
        return ans;
    }
};
```
```go
func minEnd(n, x int) int64 {
	n-- // 先把 n 减一，这样下面讨论的 n 就是原来的 n-1
	i, j := 0, 0
	for n>>j > 0 {
		// x 的第 i 个比特值是 0，即「空位」
		if x>>i&1 == 0 {
			// 空位填入 n 的第 j 个比特值
			x |= n >> j & 1 << i
			j++
		}
		i++
	}
	return int64(x)
}
```
```rust
impl Solution {
    pub fn min_end(n: i32, x: i32) -> i64 {
        let mut tn: i64 = n as i64 - 1;
        let mut tx: i64 = x as i64;
        let mut i: i64 = 0;
        let mut j: i64 = 0;
        while tn >> j != 0 {
            if (tx >> i & 1) == 0 {
                tx |= (tn >> j & 1) << i;
                j += 1;
            }
            i += 1;
        }
        tx
    }
}
```
**复杂度分析**
- 时间复杂度：$O(\log x+\log n)$ 。
- 空间复杂度：$O(1)$ 。

**优化**：把 $x$ 取反，用 $lowbit$ 枚举其中的 $1$ 的值，就是要填的空位。
```python
class Solution:
    def minEnd(self, n: int, x: int) -> int:
        n -= 1
        j = 0
        t = ~x
        while n >> j:
            lb = t & -t
            x |= (n >> j & 1) * lb
            j += 1
            t ^= lb
        return x
```
```cpp
class Solution {
public:
    long long minEnd(int n, int x) {
        n--;
        long long ans = x;
        int j = 0;
        for (long long t = ~x, lb; n >> j; t ^= lb) {
            lb = t & -t;
            ans |= (long long) (n >> j++ & 1) * lb;
        }
        return ans;
    }
};
```
```java
class Solution {
    public long minEnd(int n, int x) {
        n--;
        long ans = x;
        int j = 0;
        for (long t = ~x, lb; (n >> j) > 0; t ^= lb) {
            lb = t & -t;
            ans |= (long) (n >> j++ & 1) * lb;
        }
        return ans;
    }
}
```
```go
func minEnd(n, x int) int64 {
	n--
	j := 0
	for t, lb := ^x, 0; n>>j > 0; t ^= lb {
		lb = t & -t
		x |= n >> j & 1 * lb
		j++
	}
	return int64(x)
}
```
**复杂度分析**
- 时间复杂度：$O(\log n)$ 。循环次数只和入参 $n$ 有关。
- 空间复杂度：$O(1)$ 。

**更快的做法**？《Hacker's Delight》第 7.5 节。

**思考题**
额外输入一个 $forbidden$ 数组，表示禁止出现在 $nums$ 中的数。在这种额外约束下，$nums[n−1]$ 的最小值是多少？
答：出现在 `nums` 中的数无疑能满足相与后为 $x$ ，**禁止的这些数相与也是 $x$** ，禁止这些数出现后还要相与为 $x$ 。因此先剥离出 $forbidden$ 数组中每个数出现在【 $x$ 的 $0$ 位】上的值组成新数组，排序，遍历新数组，如果值小于 $k$（初始为 $k = n - 1$ ），则 $k++$ 。最后，往空位上填入 $k$ 。
