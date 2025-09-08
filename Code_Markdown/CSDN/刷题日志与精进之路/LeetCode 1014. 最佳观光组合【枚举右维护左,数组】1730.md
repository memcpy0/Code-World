> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个正整数数组 `values`，其中 `values[i]` 表示第 `i` 个观光景点的评分，并且两个景点 `i` 和 `j` 之间的 **距离** 为 `j - i`。

一对景点（`i < j`）组成的观光组合的得分为 `values[i] + values[j] + i - j` ，也就是景点的评分之和 **减去** 它们两者之间的距离。

返回一对观光景点能取得的最高分。

**示例 1：**
```c
输入：values = [8,1,5,2,6]
输出：11
解释：i = 0, j = 2, values[i] + values[j] + i - j = 8 + 5 + 0 - 2 = 11
```
**示例 2：**
```c
输入：values = [1,2]
输出：2
```
**提示：**
- `2 <= values.length <= 5 * 10^4`
- `1 <= values[i] <= 1000`

---
### 方法 枚举右维护左
下文把 $values$ 简记为 $v$ 。得分 $v_i + v_j + i -j$ 可变形为 $(v_i+i) + (v_j- j)$ 。

我们可以枚举 $j$ ，同时**维护在 $j$ 左边的 $v_i+i$ 的最大值 $mx$** ，用 $mx + v_j - j$ 更新答案的最大值。
示例 1 的计算过程如下：

| $j$ | $v_j$ | $v_j +j$ | $mx$ | $v_j - j$ | $mx + v_j - j$ |
| :--: | :--: | :--: | :--: | :--: | :--: |
| $0$ | $8$ | $8$ | $- ∞$ | $8$ | $-∞$ |
| $1$ | $1$ | $2$ | $8$ | $0$ | $8$ |
| $2$ | $5$ | $7$ | $8$ | $3$ | $11$ |
| $3$ | $2$ | $5$ | $8$ | $-1$ | $7$ |
| $4$ | $6$ | $10$ | $8$ | $2$ | $10$ |
$j=2$ 时可以计算出最大的得分 $11$ 。

注意 $j=0$ 时，由于 $j$ 左边没有数，所以 **$mx$ 初始化成 $−∞$** 。

不过，**把 $mx$ 初始化成 $0$** 也是可以的。由于题目保证数组中至少有两个数且 $v_i ≥1$ ，所以答案至少为 $v_0+v_1 +0−1≥v_0$ 。**把 $mx$ 初始化成 $0$** 会导致答案一开始更新为 $v_0$ ，由于我们不会计算出比 $v_0$ 还小的答案，所以把 $mx$ 初始化成 $0$ 在本题数据范围下是正确的。

也可以把 $mx$ 初始化成 $v_0 +0=v_0$ ，然后从 $j=1$ 开始遍历数组。
> ⚠ 注意：要先更新答案，再更新 $mx$ 。这可以保证更新答案时 $mx$ 表示的是 $j$ 左边的 $v_i +i$ 的最大值，$v_j +j$ 尚未更新到 $mx$ 中。

```python
class Solution:
    def maxScoreSightseeingPair(self, values: List[int]) -> int:
        ans = mx = 0  # mx 表示 j 左边的 values[i] + i 的最大值
        for j, v in enumerate(values):
            ans = max(ans, mx + v - j)
            mx = max(mx, v + j)
        return ans
```
```java
class Solution {
    public int maxScoreSightseeingPair(int[] values) {
        int ans = 0;
        int mx = values[0]; // j 左边的 values[i] + i 的最大值
        for (int j = 1; j < values.length; ++j) {
            ans = Math.max(ans, mx + values[j] - j);
            mx = Math.max(mx, values[j] + j);
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int maxScoreSightseeingPair(vector<int>& values) {
        int ans = 0, mx = values[0]; // mx 表示 j 左边的 values[i] + i 的最大值
        for (int j = 1; j < values.size(); ++j) {
            ans = max(ans, mx + values[j] - j);
            mx = max(mx, values[j] + j);
        }
        return ans;
    }
};
```
```c
#define MAX(a, b) ((b) > (a) ? (b) : (a))
int maxScoreSightseeingPair(int* values, int valuesSize) {
    int ans = 0, mx = values[0]; // mx 表示 j 左边的 values[i] + i 的最大值
    for (int j = 1; j < valuesSize; ++j) {
        ans = MAX(ans, mx + values[j] - j);
        mx = MAX(mx, values[j] + j);
    }
    return ans;
}
```
```go
func maxScoreSightseeingPair(values []int) (ans int) {
    mx := 0 // j 左边的 values[i] + i 的最大值
    for j, v := range values {
        ans = max(ans, mx+v-j)
        mx = max(mx, v+j)
    }
    return
}
```
```js
var maxScoreSightseeingPair = function(values) {
    let ans = 0, mx = values[0]; // mx 表示 j 左边的 values[i] + i 的最大值
    for (let j = 1; j < values.length; j++) {
        ans = Math.max(ans, mx + values[j] - j);
        mx = Math.max(mx, values[j] + j);
    }
    return ans;
}
```
```rust
impl Solution {
    pub fn max_score_sightseeing_pair(values: Vec<i32>) -> i32 {
        let mut ans = 0i32;
        let mut mx = 0i32; // j 左边的 values[i] + i 的最大值
        for (j, &v) in values.iter().enumerate() {
            ans = ans.max(mx + v - j as i32);
            mx = mx.max(v + j as i32);
        }
        ans
    }
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $values$ 的长度。
- 空间复杂度：$O(1)$ 。

**思考题**
所有 $v_i + v_j + i - j$ 之和是多少？其中 $i < j$ 。
​答：这是求所有 $i <j$ 的 $(i, j)$ 组合的 $v_i + i + (v_j - j)$ 的和值。我们使用**贡献法**，考虑每个元素：
- 该元素作为 $v_j$ 时，贡献为左侧元素个数 $j \times (v_j - j)$ ；
- 该元素作为 $v_i$ 时，贡献为右侧元素个数 $(n - j - 1) \times (v_j + j)$  。

代码如下：
```cpp
void solve() {
    int ans = 0, n = values.size();
    for (int j = 0; j < n; ++j) {
        ans += j * (v[j] - j); // 作为vj的贡献
        ans += (n - j - 1) * (v[j] + j); // 作为vi的贡献
    }
    return ans;
}
```
