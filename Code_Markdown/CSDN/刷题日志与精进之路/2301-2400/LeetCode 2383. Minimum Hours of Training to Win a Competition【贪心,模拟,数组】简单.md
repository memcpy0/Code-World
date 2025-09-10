> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are entering a competition, and are given two **positive** integers `initialEnergy` and `initialExperience` denoting your initial energy and initial experience respectively.

You are also given two **0-indexed** integer arrays `energy` and `experience`, both of length `n`.

You will face `n` opponents **in order**. The energy and experience of the `ith` opponent is denoted by `energy[i]` and `experience[i]` respectively. When you face an opponent, you need to have both **strictly** greater experience and energy to defeat them and move to the next opponent if available.

Defeating the `ith` opponent **increases** your experience by `experience[i]`, but **decreases** your energy by `energy[i]`.

Before starting the competition, you can train for some number of hours. After each hour of training, you can **either** choose to increase your initial experience by one, or increase your initial energy by one.

Return _the **minimum** number of training hours required to defeat all_ `n` _opponents_.

**Example 1:**
```java
Input: initialEnergy = 5, initialExperience = 3, energy = [1,4,3,2], experience = [2,6,3,1]
Output: 8
Explanation: You can increase your energy to 11 after 6 hours of training, and your experience to 5 after 2 hours of training.
You face the opponents in the following order:
- You have more energy and experience than the 0th opponent so you win.
  Your energy becomes 11 - 1 = 10, and your experience becomes 5 + 2 = 7.
- You have more energy and experience than the 1st opponent so you win.
  Your energy becomes 10 - 4 = 6, and your experience becomes 7 + 6 = 13.
- You have more energy and experience than the 2nd opponent so you win.
  Your energy becomes 6 - 3 = 3, and your experience becomes 13 + 3 = 16.
- You have more energy and experience than the 3rd opponent so you win.
  Your energy becomes 3 - 2 = 1, and your experience becomes 16 + 1 = 17.
You did a total of 6 + 2 = 8 hours of training before the competition, so we return 8.
It can be proven that no smaller answer exists.
```
**Example 2:**
```java
Input: initialEnergy = 2, initialExperience = 4, energy = [1], experience = [3]
Output: 0
****Explanation:**** You do not need any additional energy or experience to win the competition, so we return 0.
```
**Constraints:**
-   `n == energy.length == experience.length`
-   `1 <= n <= 100`
-   `1 <= initialEnergy, initialExperience, energy[i], experience[i] <= 100`

题意：给出两个**正**整数 `initialEnergy` 和 `initialExperience` 分别表示你的初始精力和初始经验，两个下标从**0**开始、长度都为 `n` 的整数数组 `energy` 和 `experience` ，表示第 `i` 个对手的精力和经验。当你对上对手时，需要在经验和精力上都 **严格** 超过对手才能击败他们，然后在可能的情况下继续对上下一个对手。

击败第 `i` 个对手会使你的经验 **增加** `experience[i]`，但会将你的精力 **减少**  `energy[i]` 。在开始比赛前，可以训练几个小时，每训练一个小时，可以选择将增加经验增加 1 **或者** 将精力增加 1 。返回击败全部 `n` 个对手需要训练的 **最少** 小时数目。

---
### 解法 贪心+模拟
由于精力是有减无加的，所以我们需要将精力提升至 `sum(energy) + 1` 的程度，才能击败所有对手，这需要几个小时。此外，当碰到经验不足时，也需要增加训练时间。
```cpp
class Solution {
public:
    int minNumberOfHours(int initialEnergy, int initialExperience, vector<int>& energy, vector<int>& experience) {
        int n = energy.size(), sum = 0;
        for (int en : energy) sum += en;
        int ans = initialEnergy > sum ? 0 : (sum - initialEnergy + 1);
        for (int ex : experience) {
            if (initialExperience <= ex) {
                ans += (ex - initialExperience + 1);
                initialExperience = ex + 1;
            }
            initialExperience += ex;
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$ 