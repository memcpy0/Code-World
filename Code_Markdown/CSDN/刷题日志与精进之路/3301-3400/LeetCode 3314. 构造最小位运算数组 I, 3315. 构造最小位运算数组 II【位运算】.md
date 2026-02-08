> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的质数数组 `nums` 。你的任务是返回一个长度为 `n` 的数组 `ans` ，对于每个下标 `i` ，以下 **条件** 均成立：
- `ans[i] OR (ans[i] + 1) == nums[i]`

除此以外，你需要 **最小化** 结果数组里每一个 `ans[i]` 。

如果没法找到符合 **条件** 的 `ans[i]` ，那么 `ans[i] = -1` 。

**质数** 指的是一个大于 1 的自然数，且它只有 1 和自己两个因数。

**示例 1：**
```java
输入：nums = [2,3,5,7]
输出：[-1,1,4,3]
解释：
- 对于 `i = 0` ，不存在 `ans[0]` 满足 `ans[0] OR (ans[0] + 1) = 2` ，所以 `ans[0] = -1` 。
- 对于 `i = 1` ，满足 `ans[1] OR (ans[1] + 1) = 3` 的最小 `ans[1]` 为 `1` ，因为 `1 OR (1 + 1) = 3` 。
- 对于 `i = 2` ，满足 `ans[2] OR (ans[2] + 1) = 5` 的最小 `ans[2]` 为 `4` ，因为 `4 OR (4 + 1) = 5` 。
- 对于 `i = 3` ，满足 `ans[3] OR (ans[3] + 1) = 7` 的最小 `ans[3]` 为 `3` ，因为 `3 OR (3 + 1) = 7` 。
```
**示例 2：**
```java
输入：nums = [11,13,31]
输出：[9,12,15]
解释：
- 对于 `i = 0` ，满足 `ans[0] OR (ans[0] + 1) = 11` 的最小 `ans[0]` 为 `9` ，因为 `9 OR (9 + 1) = 11` 。
- 对于 `i = 1` ，满足 `ans[1] OR (ans[1] + 1) = 13` 的最小 `ans[1]` 为 `12` ，因为 `12 OR (12 + 1) = 13` 。
- 对于 `i = 2` ，满足 `ans[2] OR (ans[2] + 1) = 31` 的最小 `ans[2]` 为 `15` ，因为 `15 OR (15 + 1) = 31` 。
```
**提示：**
- `1 <= nums.length <= 100`
- `2 <= nums[i] <= 10^9`（对题I来说上限是 `100` ）
- `nums[i]` 是一个质数。
---
### 解法 $O(1)$ 计算每个数
例如 $x = 100111$ ，那么 $x\ |\ (x+1) = 100111\ |\ 101000= 101111$ 。可以发现，$x\ |\ (x+1)$ 的本质是把二进制最右边的 $0$ 置为 $1$ 。

反过来，如果已知 $x\ |\ (x+1) = 101111$ ，那么倒推 $x$ ，只需把 $10 1111$ 中的某个 $1$ 变成 $0$ 。
```java
100111
101011
101101
101110
```
其中最小的是 $100111$ ，也就是把 $101111$ 的最右边的 $0$ 的右边的 $1$ 置为 $0$ 。

无解的情况：由于 $x\ |\ (x+1)$ 最低位一定是 $1$（因为 $x$ 和 $x+1$ 中必有一个奇数），所以如果 $nums[i]$ 是偶数（质数中只有 $2$ ），那么无解。

举例 $101111$ 取反，得 $01 0000$ ，其 $\text{lowbit} = 1 0000$ ，右移一位得 $1000$ 。把 $10 1111$ 与 $1000$ 异或，即可得到 $100 111$ 。
```java
class Solution {
    public int[] minBitwiseArray(List<Integer> nums) {
        int n = nums.size();
        int[] ans = new int[n];
        for (int i = 0; i < n; i++) {
            int x = nums.get(i);
            if (x == 2) {
                ans[i] = -1;
            } else {
                int t = ~x;
                int lowbit = t & -t;
                ans[i] = x ^ (lowbit >> 1);
            }
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    vector<int> minBitwiseArray(vector<int>& nums) {
        for (int& x : nums) { // 注意这里是引用
            if (x == 2) {
                x = -1;
            } else {
                int t = ~x;
                x ^= (t & -t) >> 1;
            }
        }
        return nums;
    }
};
```
```python
class Solution:
    def minBitwiseArray(self, nums: List[int]) -> List[int]:
        for i, x in enumerate(nums):
            if x == 2:
                nums[i] = -1
            else:
                t = ~x
                nums[i] ^= (t & -t) >> 1
        return nums
```
```rust
impl Solution {
    pub fn min_bitwise_array(nums: Vec<i32>) -> Vec<i32> {
        let n = nums.len();
        let mut ans = Vec::new();
        for i in 0..n {
            if nums[i] == 2 {
                ans.push(-1);
            } else {
                let t = !nums[i];
                let lowbit = t & -t;
                ans.push(nums[i] ^ (lowbit >> 1));
            }
        }
        ans
    }
}
```
```go
func minBitwiseArray(nums []int) []int {
	for i, x := range nums {
		if x == 2 {
			nums[i] = -1
		} else {
			t := ^x
			nums[i] ^= t & -t >> 1
		}
	}
	return nums
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 的长度。
- 空间复杂度：$O(1)$ 。