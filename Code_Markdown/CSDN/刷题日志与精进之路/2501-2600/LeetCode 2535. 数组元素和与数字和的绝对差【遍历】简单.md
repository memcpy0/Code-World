> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个正整数数组 `nums` 。
- **元素和** 是 `nums` 中的所有元素相加求和。
- **数字和** 是 `nums` 中每一个元素的每一数位（重复数位需多次求和）相加求和。

返回 **元素和** 与 **数字和** 的绝对差。

**注意**：两个整数 `x` 和 `y` 的绝对差定义为 `|x - y|` 。

**示例 1：**
```c
输入：nums = [1,15,6,3]
输出：9
解释：
nums 的元素和是 1 + 15 + 6 + 3 = 25 。
nums 的数字和是 1 + 1 + 5 + 6 + 3 = 16 。
元素和与数字和的绝对差是 |25 - 16| = 9 。
```
**示例 2：**
```c
输入：nums = [1,2,3,4]
输出：0
解释：
nums 的元素和是 1 + 2 + 3 + 4 = 10 。
nums 的数字和是 1 + 2 + 3 + 4 = 10 。
元素和与数字和的绝对差是 |10 - 10| = 0 。
```
**提示：**
- `1 <= nums.length <= 2000`
- `1 <= nums[i] <= 2000`

---
### 方法 遍历+利用性质简化代码
由于**元素值一定不小于其数位和**，所以答案就是元素和减去数位和。
```rust
impl Solution {
    pub fn difference_of_sum(mut nums: Vec<i32>) -> i32 {
        let mut ans = 0i32;
        for x in &mut nums {
            ans += *x;
            while *x != 0 {
                ans -= *x % 10;
                *x /= 10;
            }
        }
        ans
    }
}
```
```python
class Solution:
    def differenceOfSum(self, nums: List[int]) -> int:
        ans = 0
        for x in nums:
            ans += x
            while x:
                ans -= x % 10
                x //= 10
        return ans
```
```go
func differenceOfSum(nums []int) (ans int) {
	for _, x := range nums {
		for ans += x; x > 0; x /= 10 {
			ans -= x % 10
		}
	}
	return
}
```
**复杂度分析**
- 时间复杂度：$O(n\log U)$ ，其中 $n$ 为 $nums$ 的长度，$U=\max(nums)$ 。
- 空间复杂度：$O(1)$ ，仅用到若干额外变量。