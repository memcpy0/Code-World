> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个数组 `nums` ，数组中的数字 **要么** 出现一次，**要么** 出现两次。

请你返回数组中所有出现两次数字的按位 `XOR` 值，如果没有数字出现过两次，返回 0 。

**示例 1：**
```c
输入：nums = [1,2,1,3]
输出：1
解释：
`nums` 中唯一出现过两次的数字是 1 。
```
**示例 2：**
```c
输入：nums = [1,2,3]
输出：0
解释：
`nums` 中没有数字出现两次。
```
**示例 3：**
```c
输入：nums = [1,2,2,1]
输出：3
解释：
数字 1 和 2 出现过两次。`1 XOR 2 == 3` 。
```
**提示：**
- `1 <= nums.length <= 50`
- `1 <= nums[i] <= 50`
- `nums` 中每个数字要么出现过一次，要么出现过两次。

### 方法：一次遍历 + $O(1)$ 空间
遍历 $nums$ ，同时用一个 $vis$ 集合记录遇到的数字。
- 设 $x=nums[i]$ 。
- 如果 $x$ 不在 $vis$ 中，说明是第一次遇到，加入 $vis$ 。
- 如果 $x$ 在 $vis$ 中，说明是第二次遇到（注意每个数至多出现两次），加入答案（异或）。

代码实现时，由于 $nums[i]≤50$ ，可以用二进制数表示集合。
```python
class Solution:
    def duplicateNumbersXOR(self, nums: List[int]) -> int:
        ans = vis = 0
        for x in nums:
            if vis >> x & 1:
                ans ^= x
            else:
                vis |= 1 << x
        return ans
```
```java
class Solution {
    public int duplicateNumbersXOR(int[] nums) {
        int ans = 0;
        long vis = 0;
        for (int x : nums) {
            if ((vis >> x & 1) > 0) ans ^= x;
            else vis |= 1L << x;
        }
        return ans;
    }
}
```
```cpp
class Solution {
public:
    int duplicateNumbersXOR(vector<int>& nums) {
        long long rec = 0;
        int ans = 0;
        for (int num : nums) {
            if (rec >> num & 1) ans ^= num; // 已经出现过
            else rec |= (1LL << num);
        }
        return ans;
    }
};
```
```go
func duplicateNumbersXOR(nums []int) (ans int) {
    vis := 0
    for _, x := range nums {
        if vis>>x&1 > 0 {
            ans ^= x
	} else {
    	vis |= 1 << x
	}
    }
    return
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $nums$ 的长度。
- 空间复杂度：$O(1)$ 。