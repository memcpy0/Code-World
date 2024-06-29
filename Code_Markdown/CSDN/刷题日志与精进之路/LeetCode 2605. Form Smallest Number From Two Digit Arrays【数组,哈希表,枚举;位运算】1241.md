> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个只包含 1 到 9 之间数字的数组 `nums1` 和 `nums2` ，每个数组中的元素 **互不相同** ，请你返回 **最小** 的数字，两个数组都 **至少** 包含这个数字的某个数位。

**示例 1：**
```js
输入：nums1 = [4,1,3], nums2 = [5,7]
输出：15
解释：数字 15 的数位 1 在 nums1 中出现，数位 5 在 nums2 中出现。15 是我们能得到的最小数字。
```
**示例 2：**
```js
输入：nums1 = [3,5,2,6], nums2 = [3,1,7]
输出：3
解释：数字 3 的数位 3 在两个数组中都出现了。
```
**提示：**
- `1 <= nums1.length, nums2.length <= 9`
- `1 <= nums1[i], nums2[i] <= 9`
- 每个数组中，元素 **互不相同** 。

---
### 解法1 哈希表+枚举
如果 $\textit{nums}_1$ 与 $nums_2$ 有交集，那么答案就是交集的最小值。如果没有交集，设 $nums_1$ 的最小值为 $x$ ，$nums_2$ 的最小值为 $y$ ，答案就是 $$\min(10x+y, 10y+x)$$
```python
class Solution:
    def minNumber(self, nums1: List[int], nums2: List[int]) -> int:
        s = set(nums1) & set(nums2)
        if s: return min(s) # 有交集
        x, y = min(nums1), min(nums2)
        return min(x * 10 + y, y * 10 + x)
```
---
### 解法2 位运算
```cpp
class Solution {
public:
    int minNumber(vector<int>& nums1, vector<int>& nums2) {
        int mask1 = 0, mask2 = 0;
        for (int x : nums1) mask1 |= 1 << x;
        for (int x : nums2) mask2 |= 1 << x;
        int m = mask1 & mask2;
        if (m) return __builtin_ctz(m); // 有交集
        int x = __builtin_ctz(mask1), y = __builtin_ctz(mask2);
        return min(x * 10 + y, y * 10 + x);
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n+m)$ ，其中 nnn 为 $nums_1$ 的长度，mmm 为 $nums_2$ 的长度。
- 空间复杂度：$\mathcal{O}(1)$ 。仅用到若干额外变量。