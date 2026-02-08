> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
### 3349. 检测相邻递增子数组 I
给你一个由 `n` 个整数组成的数组 `nums` 和一个整数 `k`，请你确定是否存在 **两个** **相邻** 且长度为 `k` 的 **严格递增** 子数组。具体来说，需要检查是否存在从下标 `a` 和 `b` (`a < b`) 开始的 **两个** 子数组，并满足下述全部条件：
- 这两个子数组 `nums[a..a + k - 1]` 和 `nums[b..b + k - 1]` 都是 **严格递增** 的。
- 这两个子数组必须是 **相邻的**，即 `b = a + k`。

如果可以找到这样的 **两个** 子数组，请返回 `true`；否则返回 `false`。

**子数组** 是数组中的一个连续 **非空** 的元素序列。

**示例 1：**
```c
输入：nums = [2,5,7,8,9,2,3,4,3,1], k = 3
输出：true
解释：
- 从下标 `2` 开始的子数组为 `[7, 8, 9]`，它是严格递增的。
- 从下标 `5` 开始的子数组为 `[2, 3, 4]`，它也是严格递增的。
- 两个子数组是相邻的，因此结果为 `true`。
```
**示例 2：**
```c
输入：nums = [1,2,3,4,4,4,4,5,6,7], k = 5
输出：false
```
**提示：**
- `2 <= nums.length <= 100`
- `1 <= 2 * k <= nums.length`
- `-1000 <= nums[i] <= 1000`
### 3350. 检测相邻递增子数组 II
给你一个由 `n` 个整数组成的数组 `nums` ，请你找出 `k` 的 **最大值**，使得存在 **两个** **相邻** 且长度为 `k` 的 **严格递增** 子数组。具体来说，需要检查是否存在从下标 `a` 和 `b` (`a < b`) 开始的 **两个** 子数组，并满足下述全部条件：
- 这两个子数组 `nums[a..a + k - 1]` 和 `nums[b..b + k - 1]` 都是 **严格递增** 的。
- 这两个子数组必须是 **相邻的**，即 `b = a + k`。

返回 `k` 的 **最大可能** 值。

**子数组** 是数组中的一个连续 **非空** 的元素序列。

**示例 1：**
```c
输入：nums = [2,5,7,8,9,2,3,4,3,1]
输出：3
解释：
- 从下标 2 开始的子数组是 `[7, 8, 9]`，它是严格递增的。
- 从下标 5 开始的子数组是 `[2, 3, 4]`，它也是严格递增的。
- 这两个子数组是相邻的，因此 3 是满足题目条件的 最大 `k` 值。
```
**示例 2：**
```c
输入：nums = [1,2,3,4,4,4,4,5,6,7]
输出：2
解释：
- 从下标 0 开始的子数组是 `[1, 2]`，它是严格递增的。
- 从下标 2 开始的子数组是 `[3, 4]`，它也是严格递增的。
- 这两个子数组是相邻的，因此 2 是满足题目条件的 最大 `k` 值。
```
**提示：**
- `2 <= nums.length <= 2 * 10^5`
- `-10^9 <= nums[i] <= 10^9`
---
### 方法 一次遍历（3350题解）
我们遍历 $nums$ ，寻找严格递增段（子数组）。设当前严格递增段的长度为 $cnt$ ，上个严格递增段的长度为 $preCnt$ 。

答案有两种情况：
- 两个子数组属于同个严格递增段（或者说考虑将当前严格递增段分为两段），那么 $k$ 最大是 $\lfloor \dfrac {cnt} {2} \rfloor$ 。
- 两个子数组分别属于一对相邻的严格递增段，那么 $k$ 最大是 $\min(preCnt, cnt)$ 。

用3350题的做法解得 $k$ 的最大可能值 $ans$ 后，判断题目传入的 $k$ 是否 $\le$ 最大可能值 $ans$ 即可解决3349题。
```cpp
class Solution {
public:
    int maxIncreasingSubarrays(vector<int>& nums) {
        int ans = 0, pre_cnt = 0, cnt = 0;
        for (int i = 0; i < nums.size(); i++) {
            cnt++;
            if (i == nums.size() - 1 || nums[i] >= nums[i + 1]) { // i 是严格递增段的末尾
                ans = max({ans, cnt / 2, min(pre_cnt, cnt)});
                pre_cnt = cnt;
                cnt = 0;
            }
        }
        return ans;
    }
};
```
```java
class Solution {
    public int maxIncreasingSubarrays(List<Integer> nums) {
        int ans = 0;
        int preCnt = 0;
        int cnt = 0;
        for (int i = 0; i < nums.size(); i++) {
            cnt++;
            // i 是严格递增段的末尾
            if (i == nums.size() - 1 || nums.get(i) >= nums.get(i + 1)) {
                ans = Math.max(ans, Math.max(cnt / 2, Math.min(preCnt, cnt)));
                preCnt = cnt;
                cnt = 0;
            }
        }
        return ans;
    }
}
// 写法二
class Solution {
    public int maxIncreasingSubarrays(List<Integer> nums) {
        Integer[] a = nums.toArray(Integer[]::new); // 转成数组处理，更快
        int ans = 0;
        int preCnt = 0;
        int cnt = 0;
        for (int i = 0; i < a.length; i++) {
            cnt++;
            // i 是严格递增段的末尾
            if (i == a.length - 1 || a[i] >= a[i + 1]) {
                ans = Math.max(ans, Math.max(cnt / 2, Math.min(preCnt, cnt)));
                preCnt = cnt;
                cnt = 0;
            }
        }
        return ans;
    }
}
```
```rust
impl Solution {
    pub fn max_increasing_subarrays(nums: Vec<i32>) -> i32 {
        let mut ans = 0;
        let mut pre_cnt = 0;
        let mut cnt = 0;
        let n = nums.len();
        for i in 0..n {
            cnt += 1;
            if i == n - 1 || nums[i] >= nums[i + 1] { // i是严格递增段的末尾
                ans = ans.max((cnt / 2).max(pre_cnt.min(cnt)));
                pre_cnt = cnt;
                cnt = 0;
            }
        }
        ans
    }
}
```
```go
func maxIncreasingSubarrays(nums []int) (ans int) {
	preCnt, cnt := 0, 0
	for i, x := range nums {
		cnt++
		if i == len(nums)-1 || x >= nums[i+1] { // i 是严格递增段的末尾
			ans = max(ans, cnt/2, min(preCnt, cnt))
			preCnt = cnt
			cnt = 0
		}
	}
	return
}
```
```python
class Solution:
    def maxIncreasingSubarrays(self, nums: List[int]) -> int:
        ans = pre_cnt = cnt = 0
        for i, x in enumerate(nums):
            cnt += 1
            if i == len(nums) - 1 or x >= nums[i + 1]:  # i 是严格递增段的末尾
                ans = max(ans, cnt // 2, min(pre_cnt, cnt))
                pre_cnt = cnt
                cnt = 0
        return ans
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 的长度。
- 空间复杂度：$O(1)$ 。

---
### 专题训练
其他做法见分组循环。[滑动窗口（定长/不定长/多指针）](https://leetcode.cn/circle/discuss/0viNMK/)专题的最后。