> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

# [3151. 特殊数组 I](https://leetcode.cn/problems/special-array-i/)
如果数组的每一对相邻元素都是两个奇偶性不同的数字，则该数组被认为是一个 **特殊数组** 。

Aging 有一个整数数组 `nums`。如果 `nums` 是一个 **特殊数组** ，返回 `true`，否则返回 `false`。

**示例 1：**
```rust
输入：nums = [1]
输出：true
解释：
只有一个元素，所以答案为 `true`。
```
**示例 2：**
```rust
输入：nums = [2,1,4]
输出：true
解释：
只有两对相邻元素： `(2,1)` 和 `(1,4)`，它们都包含了奇偶性不同的数字，因此答案为 `true`。
```
**示例 3：**
```rust
输入：nums = [4,3,1,6]
输出：false
解释：
`nums[1]` 和 `nums[2]` 都是奇数。因此答案为 `false`。
```
**提示：**
- `1 <= nums.length <= 100`
- `1 <= nums[i] <= 100`

### 方法：遍历判断相邻元素
遍历数组，如果存在一对相邻元素，它们的奇偶性不同，返回 `false` 。如果不存在，返回 `true` 。
```python
class Solution:
    def isArraySpecial(self, nums: List[int]) -> bool:
        return all(x % 2 != y % 2 for x, y in pairwise(nums))
```
```java
class Solution {
    public boolean isArraySpecial(int[] nums) {
        for (int i = 1; i < nums.length; i++) {
            if (nums[i - 1] % 2 == nums[i] % 2) {
                return false;
            }
        }
        return true;
    }
}
```
```cpp
class Solution {
public:
    bool isArraySpecial(vector<int>& nums) {
        for (int i = 1; i < nums.size(); i++) {
            if (nums[i - 1] % 2 == nums[i] % 2) {
                return false;
            }
        }
        return true;
    }
};
```
```go
func isArraySpecial(nums []int) bool {
    for i := 1; i < len(nums); i++ {
        if nums[i-1]%2 == nums[i]%2 {
            return false
        }
    }
    return true
}
```
```rust
impl Solution {
    pub fn is_array_special(nums: Vec<i32>) -> bool {
        for i in 0 .. nums.len() - 1 {
            if nums[i] % 2 == nums[i + 1] % 2 {
                return false;
            }
        }
        return true;
    }
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 的长度。
- 空间复杂度：$O(1)$ 。

---
# [3152. 特殊数组 II](https://leetcode.cn/problems/special-array-ii/)
如果数组的每一对相邻元素都是两个奇偶性不同的数字，则该数组被认为是一个 **特殊数组** 。

你有一个整数数组 `nums` 和一个二维整数矩阵 `queries`，对于 `queries[i] = [fromi, toi]`，请你帮助你检查 子数组 `nums[fromi..toi]` 是不是一个 **特殊数组** 。

返回布尔数组 `answer`，如果 `nums[fromi..toi]` 是特殊数组，则 `answer[i]` 为 `true` ，否则，`answer[i]` 为 `false` 。

**示例 1：**
```c
输入：nums = [3,4,1,2,6], queries = [[0,4]]
输出：[false]
解释：
子数组是 `[3,4,1,2,6]`。2 和 6 都是偶数。
```
**示例 2：**
```c
输入：nums = [4,3,1,6], queries = [[0,2],[2,3]]
输出：[false,true]
解释：
1. 子数组是 `[4,3,1]`。3 和 1 都是奇数。因此这个查询的答案是 `false`。
2. 子数组是 `[1,6]`。只有一对：`(1,6)`，且包含了奇偶性不同的数字。因此这个查询的答案是 `true`。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^5`
- `1 <= queries.length <= 10^5`
- `queries[i].length == 2`
- `0 <= queries[i][0] <= queries[i][1] <= nums.length - 1`

### 方法1：前缀和
如果子数组存在一对相邻元素，它们的奇偶性相同，那么这个子数组就不是特殊数组。怎么**快速**知道是否有奇偶性相同的相邻元素？
> 考虑这样一个问题：给你一个只包含 $0$ 和 $1$ 的数组，如何快速判断一个子数组是否全为 $0$ ？
解答：如果子数组的**元素和**等于 $0$ ，那么子数组**一定**全为 $0$ ；如果子数组的元素和大于 $0$ ，那么子数组一定包含 $1$ 。如何快速计算子数组元素和？这可以用 **前缀和** 解决。

对于本题，定义长为 $n−1$ 的数组 $a$ ，其中
$$a[i]=\begin{cases}
0, \quad nums[i]\bmod 2 \ne nums[i+1] \bmod 2 \\
1, \quad nums[i] \bmod 2 = nums[i+1]\bmod 2
\end{cases}
$$
如果 $a$ 的下标**从 $from$ 到 $to−1$ 的子数组和**等于 $0$ ，就说明 $nums$ 的下标从 $from$ 到 $to$ 的这个子数组，其所有相邻元素的**奇偶性都不同**，该子数组为特殊数组。

计算 $a$ 的前缀和 $s$ ，可以快速判断子数组和是否为 $0$ ，也就是判断
$$s[to]−s[from]=0$$
即
$$s[from]=s[to]$$
```cpp
class Solution {
public:
    vector<bool> isArraySpecial(vector<int>& nums, vector<vector<int>>& queries) {
        vector<int> s(nums.size());
        for (int i = 1; i < nums.size(); i++) {
            s[i] = s[i - 1] + (nums[i - 1] % 2 == nums[i] % 2);
        }
        vector<bool> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) {
            auto& q = queries[i];
            ans[i] = s[q[0]] == s[q[1]];
        }
        return ans;
    }
};
```
```python
class Solution:
    def isArraySpecial(self, nums: List[int], queries: List[List[int]]) -> List[bool]:
        s = list(accumulate((x % 2 == y % 2 for x, y in pairwise(nums)), initial=0))
        return [s[from_] == s[to] for from_, to in queries]
```
```java
class Solution {
    public boolean[] isArraySpecial(int[] nums, int[][] queries) {
        int[] s = new int[nums.length];
        for (int i = 1; i < nums.length; i++) {
            s[i] = s[i - 1] + (nums[i - 1] % 2 == nums[i] % 2 ? 1 : 0);
        }
        boolean[] ans = new boolean[queries.length];
        for (int i = 0; i < queries.length; i++) {
            int[] q = queries[i];
            ans[i] = s[q[0]] == s[q[1]];
        }
        return ans;
    }
}
```
```go
func isArraySpecial(nums []int, queries [][]int) []bool {
	s := make([]int, len(nums))
	for i := 1; i < len(nums); i++ {
		s[i] = s[i-1]
		if nums[i-1]%2 == nums[i]%2 {
			s[i]++
		}
	}
	ans := make([]bool, len(queries))
	for i, q := range queries {
		ans[i] = s[q[0]] == s[q[1]]
	}
	return ans
}
```
```rust
impl Solution {
    pub fn is_array_special(nums: Vec<i32>, queries: Vec<Vec<i32>>) -> Vec<bool> {
        let mut sum: Vec<i32> = vec![0; nums.len()];
        for i in 1..nums.len() {
            sum[i] = sum[i - 1] + (if nums[i - 1] % 2 != nums[i] % 2 { 0 } else { 1 });
        }
        let mut ans: Vec<bool> = Vec::new();
        for q in queries {
            ans.push(sum[q[0] as usize] == sum[q[1] as usize]);
        }
        ans
    }
}
```
也可以用**位运算**计算 $a[i]$ ：相邻两数的异或和，其最低位取反即为 $a[i]$ 。
```python
class Solution:
    def isArraySpecial(self, nums: List[int], queries: List[List[int]]) -> List[bool]:
        s = list(accumulate(((x ^ y ^ 1) & 1 for x, y in pairwise(nums)), initial=0))
        return [s[from_] == s[to] for from_, to in queries]
```
```java
class Solution {
    public boolean[] isArraySpecial(int[] nums, int[][] queries) {
        int[] s = new int[nums.length];
        for (int i = 1; i < nums.length; i++) {
            s[i] = s[i - 1] + ((nums[i] ^ nums[i - 1] ^ 1) & 1);
        }
        boolean[] ans = new boolean[queries.length];
        for (int i = 0; i < queries.length; i++) {
            int[] q = queries[i];
            ans[i] = s[q[0]] == s[q[1]];
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<bool> isArraySpecial(vector<int>& nums, vector<vector<int>>& queries) {
        vector<int> s(nums.size());
        for (int i = 1; i < nums.size(); i++) {
            s[i] = s[i - 1] + ((nums[i] ^ nums[i - 1] ^ 1) & 1);
        }
        vector<bool> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) {
            auto& q = queries[i];
            ans[i] = s[q[0]] == s[q[1]];
        }
        return ans;
    }
};
```
```go
func isArraySpecial(nums []int, queries [][]int) []bool {
    s := make([]int, len(nums))
    for i := 1; i < len(nums); i++ {
        s[i] = s[i-1] + (nums[i]^nums[i-1]^1)&1
    }
    ans := make([]bool, len(queries))
    for i, q := range queries {
        ans[i] = s[q[0]] == s[q[1]]
    }
    return ans
}
```
**复杂度分析**
- 时间复杂度：$O(n+q)$ ，其中 $n$ 是 $nums$ 的长度，其中 $q$ 是 $queries$ 的长度。
- 空间复杂度：$O(n)$ 。返回值不计入。

### 方法二：递推记录最近一次奇偶性相同的位置
考虑对于每个位置（下标）$i$ ，记录**最近一次**奇偶性相同的位置。

定义 $lastSame[i]$ 为 $≤i$ 的**最大**下标 $j$ ，满足 $nums[j−1] \bmod 2=nums[j] \bmod 2$ 。如果没有这样的 $j$ ，则 $lastSame[i]=0$ 。

例如 $nums=[1,1,2,1,1,2]$ ，对应的 $lastSame=[0,1,1,1,4,4]$ 。

对于这个例子，回答询问 $from=2, to=3$ ，因为 $lastSame[3]=1≤from$ ，所以子数组 $[from,to]$ 内没有奇偶性相同的相邻元素。又例如 $from=0, to=3$ ，因为 $lastSame[3]=1>from$ ，所以子数组 $[from,to]$ 包含奇偶性相同的相邻元素。
> ⚠**注意**：当 $lastSame[to]=from$ 时，由于定义是比较 $j−1$ 和 $j$ 这两个位置，而 $j−1<from$ 在子数组范围外，所以在这种情况下，子数组内没有奇偶性相同的相邻元素。

怎么计算 $lastSame[i]$ ？分类讨论：
- 如果 $nums[i−1] \bmod 2=nums[i]\bmod 2$ ，那么根据定义，$lastSame[i]=i$ 。
- 如果 $nums[i−1] \bmod 2 \ne nums[i] \bmod 2$ ，说明定义中的 $j≤i−1$ ，所以 $lastSame[i]=lastSame[i−1]$ 。

所以可以用**递推**的方法计算出 $lastSame$ 。
```python
class Solution:
    def isArraySpecial(self, nums: List[int], queries: List[List[int]]) -> List[bool]:
        n = len(nums)
        last_same = [0] * n
        for i in range(1, n):
            last_same[i] = i if nums[i - 1] % 2 == nums[i] % 2 else last_same[i - 1]
        return [last_same[to] <= from_ for from_, to in queries]
```
```cpp
class Solution {
public:
    vector<bool> isArraySpecial(vector<int>& nums, vector<vector<int>>& queries) {
        int n = nums.size();
        vector<int> last_same(n);
        for (int i = 1; i < n; i++) {
            last_same[i] = nums[i - 1] % 2 == nums[i] % 2 ? i : last_same[i - 1];
        }
        vector<bool> ans(queries.size());
        for (int i = 0; i < queries.size(); i++) {
            auto& q = queries[i];
            ans[i] = last_same[q[1]] <= q[0];
        }
        return ans;
    }
};
```
```java
class Solution {
    public boolean[] isArraySpecial(int[] nums, int[][] queries) {
        int n = nums.length;
        int[] lastSame = new int[n];
        for (int i = 1; i < n; i++) {
            lastSame[i] = nums[i - 1] % 2 == nums[i] % 2 ? i : lastSame[i - 1];
        }
        boolean[] ans = new boolean[queries.length];
        for (int i = 0; i < queries.length; i++) {
            int[] q = queries[i];
            ans[i] = lastSame[q[1]] <= q[0];
        }
        return ans;
    }
}
```
```go
func isArraySpecial(nums []int, queries [][]int) []bool {
	n := len(nums)
	lastSame := make([]int, n)
	for i := 1; i < n; i++ {
		if nums[i-1]%2 == nums[i]%2 {
			lastSame[i] = i
		} else {
			lastSame[i] = lastSame[i-1]
		}
	}
	ans := make([]bool, len(queries))
	for i, q := range queries {
		ans[i] = lastSame[q[1]] <= q[0]
	}
	return ans
}
```
```rust
impl Solution {
    pub fn is_array_special(nums: Vec<i32>, queries: Vec<Vec<i32>>) -> Vec<bool> {
        let mut lastSame: Vec<i32> = vec![0; nums.len()];
        for i in 1..nums.len() {
            lastSame[i] = if nums[i - 1] % 2 == nums[i] % 2 { i as i32 } else { lastSame[i - 1] };
        }
        let mut ans: Vec<bool> = Vec::new();
        for q in queries {
            ans.push(lastSame[q[1] as usize] <= q[0]);
        }
        ans
    }
}
```
**复杂度分析**
- 时间复杂度：$O(n+q)$ ，其中 $n$ 是 $nums$ 的长度，其中 $q$ 是 $queries$ 的长度。
- 空间复杂度：$O(n)$ 。返回值不计入。