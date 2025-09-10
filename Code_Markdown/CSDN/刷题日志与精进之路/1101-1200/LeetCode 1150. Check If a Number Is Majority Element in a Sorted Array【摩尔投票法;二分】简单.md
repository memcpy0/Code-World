> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给出一个按 **非递减** 顺序排列的数组 `nums`，和一个目标数值 `target`。假如数组 `nums` 中绝大多数元素的数值都等于 `target`，则返回 `True`，否则请返回 `False`。

所谓占绝大多数，是指在长度为 `N` 的数组中出现必须 **超过 `N/2`** **次**。

**示例 1：**
```js
输入：nums = [2,4,5,5,5,5,5,6,6], target = 5
输出：true
解释：
数字 5 出现了 5 次，而数组的长度为 9。
所以，5 在数组中占绝大多数，因为 5 次 > 9/2。
```
**示例 2：**
```js
输入：nums = [10,100,101,101], target = 101
输出：false
解释：
数字 101 出现了 2 次，而数组的长度是 4。
所以，101 不是 数组占绝大多数的元素，因为 2 次 = 4/2。
```
**提示：**
- `1 <= nums.length <= 1000`
- `1 <= nums[i] <= 10^9`
- `1 <= target <= 10^9`

---
题目集合：
- [169. 多数元素](https://leetcode.cn/problems/majority-element/)
- [面试题 17.10. Find Majority Element LCCI](https://leetcode.cn/problems/find-majority-element-lcci/)
- [229. 多数元素 II](https://leetcode.cn/problems/majority-element-ii/)
- [1150. 检查一个数是否在数组中占绝大多数](https://leetcode.cn/problems/check-if-a-number-is-majority-element-in-a-sorted-array/)
- [1157. 子数组中占绝大多数的元素](https://leetcode.cn/problems/online-majority-element-in-subarray/)：困难

### 解法1 两次二分查找
查找有序数组中的数字最快的方法还是二分查找，我们使用二分查找求目标数字的左右下标。
```cpp
class Solution {
public:
    bool isMajorityElement(vector<int>& nums, int target) {
        return (upper_bound(nums.begin(), nums.end(), target) - lower_bound(nums.begin(), nums.end(), target)) * 2 > nums.size();
    }
};
```
复杂度分析：
- 时间复杂度：$O(\log n)$ 。由于二分查找每次将搜索区间大约划分为两等分，所以时间复杂度为 $O(\log n)$ 。二分查找的过程被调用了两次，所以总的时间复杂度是对数级别的。其中 $n$ 为数组 $nums$ 的长度。
- 空间复杂度：$O(1)$ ，没有使用额外的空间。

---
### 解法2 一次二分查找（最优）
在方法1的基础上，还可以继续优化时间复杂度，**只需要使用一次二分查找，找到目标数字的最左边界下标 $left$ ，再判断边界下标加上数组一半的下标$left + len(nums) / 2$ 的数字、是否等于目标数字 $target$** 。如果等于，则说明长度大于一半。

需要注意判断相加后的下标是否越界。 
```cpp
class Solution {
public:
    bool isMajorityElement(vector<int>& nums, int target) {
        int left = lower_bound(nums.begin(), nums.end(), target) - nums.begin();
        int right = left + nums.size() / 2;
        return right >= nums.size() ? false : nums[right] == target;
    }
};
```