> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` 和一个整数 `k` 。

请你用整数形式返回 `nums` 中的特定元素之 **和** ，这些特定元素满足：其对应下标的二进制表示中恰存在 `k` 个置位。

整数的二进制表示中的 1 就是这个整数的 **置位** 。

例如，`21` 的二进制表示为 `10101` ，其中有 `3` 个置位。

**示例 1：**
```cpp
输入：nums = [5,10,1,5,2], k = 1
输出：13
解释：下标的二进制表示是： 
0 = 000
1 = 001
2 = 010
3 = 011
4 = 100 
下标 1、2 和 4 在其二进制表示中都存在 k = 1 个置位。
因此，答案为 nums[1] + nums[2] + nums[4] = 13 。
```
**示例 2：**
```cpp
输入：nums = [4,3,2,1], k = 2
输出：1
解释：下标的二进制表示是： 
0 = 00
1 = 01
2 = 10
3 = 11
只有下标 3 的二进制表示中存在 k = 2 个置位。
因此，答案为 nums[3] = 1 。
```
**提示：**
- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^5`
- `0 <= k <= 10`

---
### 解法 位操作
把所有【满足下标的二进制中的 $1$ 的个数等于 $k$ 的 $\textit{nums}[i]$ 】加起来，就是答案。
```cpp
// cpp
class Solution {
public:
    int sumIndicesWithKSetBits(vector<int>& nums, int k) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i)
            if (__builtin_popcount(i) == k) ans += nums[i];
        return ans;
    }
};
// java
class Solution {
    public int sumIndicesWithKSetBits(List<Integer> nums, int k) {
        int ans = 0;
        for (int i = 0, n = nums.size(); i < n; ++i) {
            if (Integer.bitCount(i) == k) ans += nums.get(i);
            // n = n & (n - 1)
        }
        return ans;
    }
}

// python
class Solution:
    def sumIndicesWithKSetBits(self, nums: List[int], k: int) -> int:
        return sum(x for i, x in enumerate(nums) if i.bit_count() == k)
// go
func sumIndicesWithKSetBits(nums []int, k int) (ans int) {
    for i, x := range nums {
        if bits.OnesCount(uint(i)) == k {
            ans += x
        }
    }
    return
}
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为 $nums$ 的长度。理由见视频。
- 空间复杂度：$\mathcal{O}(1)$ 。