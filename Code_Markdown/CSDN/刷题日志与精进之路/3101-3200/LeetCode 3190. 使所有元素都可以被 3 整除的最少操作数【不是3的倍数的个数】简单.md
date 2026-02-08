> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `nums` 。一次操作中，你可以将 `nums` 中的 **任意** 一个元素增加或者减少 1 。

请你返回将 `nums` 中所有元素都可以被 3 整除的 **最少** 操作次数。

**示例 1：**
```c
输入：nums = [1,2,3,4]
输出：3
解释：
通过以下 3 个操作，数组中的所有元素都可以被 3 整除：
- 将 1 减少 1 。
- 将 2 增加 1 。
- 将 4 减少 1 。
```
**示例 2：**
```c
输入：nums = [3,6,9]
输出：0
```
**提示：**
- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
---
### 方法 不是3的倍数的元素个数
遍历 $nums$ ，按照元素模 $3$ 的余数分类：
- 如果 $nums[i] = 3k$ ，无需操作；
- 如果 $nums[i] = 3k+1$ ，减一得到 $3$ 的倍数；
- 如果 $nums[i] = 3k+2$ ，加一得到 $3$ 的倍数。
由此可见，对于不是 $3$ 的倍数的元素，只需操作一次就可变成 $3$ 的倍数，所以答案为不是 $3$ 的倍数的元素个数。

```java
class Solution {
    public int minimumOperations(int[] nums) {
        int ans = 0;
        for (int x : nums) {
            ans += x % 3 != 0 ? 1 : 0;
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int minimumOperations(vector<int>& nums) {
        int ans = 0;
        for (int x : nums) {
            ans += x % 3 != 0;
        }
        return ans;
    }
};
```
```python
class Solution:
    def minimumOperations(self, nums: List[int]) -> int:
        return sum(x % 3 != 0 for x in nums)
```
```rust
impl Solution {
    pub fn minimum_operations(nums: Vec<i32>) -> i32 {
        nums.into_iter().filter(|&x| x % 3 != 0).count() as _
    }
}
```
```go
func minimumOperations(nums []int) (ans int) {
	for _, x := range nums {
		if x%3 != 0 {
			ans++
		}
	}
	return
}
```
```c
int minimumOperations(int* nums, int numsSize) {
    int ans = 0;
    for (int i = 0; i < numsSize; i++) {
        ans += nums[i] % 3 != 0;
    }
    return ans;
}
```
```js
var minimumOperations = function(nums) {
    return _.sumBy(nums, x => (x % 3 !== 0 ? 1 : 0));
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 的长度。
- 空间复杂度：$O(1)$ 。
---
### 思考题
把题目中的 $3$ 改成 $4$ 呢？改成 $m$ 呢？
答：一个数可以通过加减往数轴两侧移动，取最小操作次数 $\min (x \bmod m, m - x \bmod m)$ 。