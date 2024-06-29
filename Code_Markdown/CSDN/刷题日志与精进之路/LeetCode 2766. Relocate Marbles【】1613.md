> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` ，表示一些石块的初始位置。再给你两个长度 **相等** 下标从 **0** 开始的整数数组 `moveFrom` 和 `moveTo` 。

在 `moveFrom.length` 次操作内，你可以改变石块的位置。在第 `i` 次操作中，你将位置在 `moveFrom[i]` 的所有石块移到位置 `moveTo[i]` 。

完成这些操作后，请你按升序返回所有 **有** 石块的位置。

**注意：**

- 如果一个位置至少有一个石块，我们称这个位置 **有** 石块。
- 一个位置可能会有多个石块。

**示例 1：**

**输入：**nums = [1,6,7,8], moveFrom = [1,7,2], moveTo = [2,9,5]
**输出：**[5,6,8,9]
**解释：**一开始，石块在位置 1,6,7,8 。
第 i = 0 步操作中，我们将位置 1 处的石块移到位置 2 处，位置 2,6,7,8 有石块。
第 i = 1 步操作中，我们将位置 7 处的石块移到位置 9 处，位置 2,6,8,9 有石块。
第 i = 2 步操作中，我们将位置 2 处的石块移到位置 5 处，位置 5,6,8,9 有石块。
最后，至少有一个石块的位置为 [5,6,8,9] 。

**示例 2：**

**输入：**nums = [1,1,3,3], moveFrom = [1,3], moveTo = [2,2]
**输出：**[2]
**解释：**一开始，石块在位置 [1,1,3,3] 。
第 i = 0 步操作中，我们将位置 1 处的石块移到位置 2 处，有石块的位置为 [2,2,3,3] 。
第 i = 1 步操作中，我们将位置 3 处的石块移到位置 2 处，有石块的位置为 [2,2,2,2] 。
由于 2 是唯一有石块的位置，我们返回 [2] 。

**提示：**

- `1 <= nums.length <= 10^5`
- `1 <= moveFrom.length <= 10^5`
- `moveFrom.length == moveTo.length`
- `1 <= nums[i], moveFrom[i], moveTo[i] <= 10^9`
- 测试数据保证在进行第 `i` 步操作时，`moveFrom[i]` 处至少有一个石块。



```cpp
class Solution {
public:
    vector<int> relocateMarbles(vector<int>& nums, vector<int>& moveFrom, vector<int>& moveTo) {
        map<int, int> rec;
        for (int i : nums) ++rec[i];
        for (int i = 0, n = moveFrom.size(); i < n; ++i) {
            if (moveFrom[i] == moveTo[i]) continue;
            rec[ moveTo[i] ] += rec[moveFrom[i]];
            rec[moveFrom[i]] = 0;
        }
        vector<int> ans;
        for (auto it : rec) {
            if (it.second)
            // for (int i = 1; i <= it.second; ++i)
                ans.push_back(it.first);
        }
        return ans;
    }
};
```