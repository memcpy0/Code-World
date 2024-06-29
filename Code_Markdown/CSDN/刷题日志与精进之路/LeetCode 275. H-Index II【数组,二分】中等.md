> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `citations` ，其中 `citations[i]` 表示研究者的第 `i` 篇论文被引用的次数，`citations` 已经按照 **升序排列** 。计算并返回该研究者的 h 指数。

[h 指数的定义](https://baike.baidu.com/item/h-index/3991452?fr=aladdin)：h 代表“高引用次数”（high citations），一名科研人员的 `h` 指数是指他（她）的 （`n` 篇论文中）**总共**有 `h` 篇论文分别被引用了**至少** `h` 次。

请你设计并实现对数时间复杂度的算法解决此问题。

**示例 1：**
```js
输入：citations = [0,1,3,5,6]
输出：3 
解释：给定数组表示研究者总共有 `5` 篇论文，每篇论文相应的被引用了 0, 1, 3, 5, 6 次。
     由于研究者有 `3` 篇论文每篇 至少 被引用了 `3` 次，其余两篇论文每篇被引用 不多于 `3` 次，所以她的 h 指数是 `3` 。
```
**示例 2：**
```js
输入：citations = [1,2,100]
输出：2
```
**提示：**
- `n == citations.length`
- `1 <= n <= 10^5`
- `0 <= citations[i] <= 1000`
- `citations` 按 **升序排列**
 
---
### 解法 二分
这是274. H 指数的延伸题目。由于数组已经有序，所以对于给定的 `i` ，我们已知有 `citations.length - i` 篇论文的引用次数都大于等于 `citations[i]` ，`i` 篇论文的引用数小于等于 `citations[i]` 。

不妨设 `h = citations.length - i` ，即有 `h` 篇论文分别引用了至少 `citations[i]` 次，其他 `citations.length - h` 篇论文的引用次数不超过 `citations[i]` 次。**只要 `citations[i] >= h` ，就满足题意，有 `h` 篇论文的引用次数至少为 `h`** 。因此，我们可以用二分寻找第一个满足 `citations[i] >= citations.length - i` 的位置 `i` ，答案就是 `citations.length - i` 。
```cpp
//cplusplus
class Solution {
public:
    int hIndex(vector<int>& citations) {
        int n = citations.size(), l = 0, h = n;
        while (l < h) {
            int m = l + (h - l) / 2;
            if (citations[m] >= n - m) h = m;
            else l = m + 1;
        }
        return n - l;
    }
};
//执行用时：20 ms, 在所有 C++ 提交中击败了67.79% 的用户
//内存消耗：18.1 MB, 在所有 C++ 提交中击败了75.02% 的用户
```
