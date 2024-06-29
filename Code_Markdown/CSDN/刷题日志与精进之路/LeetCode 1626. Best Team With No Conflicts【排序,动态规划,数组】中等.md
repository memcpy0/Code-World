> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

You are the manager of a basketball team. For the upcoming tournament, you want to choose the team with the highest overall score. The score of the team is the **sum** of scores of all the players in the team.

However, the basketball team is not allowed to have **conflicts**. A **conflict** exists if a younger player has a **strictly higher** score than an older player. A conflict does **not** occur between players of the same age.

Given two lists, `scores` and `ages`, where each `scores[i]` and `ages[i]` represents the score and age of the `ith` player, respectively, return _the highest overall score of all possible basketball teams_.

**Example 1:**
```java
Input: scores = [1,3,5,10,15], ages = [1,2,3,4,5]
Output: 34
Explanation: You can choose all the players.
```
**Example 2:**
```java
Input: scores = [4,5,6,5], ages = [2,1,2,1]
Output: 16
Explanation: It is best to choose the last 3 players. Notice that you are allowed to choose multiple people of the same age.
```
**Example 3:**
```java
Input: scores = [1,2,3,5], ages = [8,9,10,1]
Output: 6
Explanation: It is best to choose the first 3 players. 
```
**Constraints:**
-   `1 <= scores.length, ages.length <= 1000`
-   `scores.length == ages.length`
-   `1 <= scores[i] <= 10^6`
-   `1 <= ages[i] <= 1000`

题意：给你两个列表 `scores` 和 `ages`，其中每组 `scores[i]` 和 `ages[i]` 表示第 `i` 名球员的分数和年龄。然而，球队中的矛盾会限制球员的发挥，所以必须选出一支 **没有矛盾** 的球队。如果一名年龄较小球员的分数 **严格大于** 一名年龄较大的球员，则存在矛盾。同龄球员之间不会发生矛盾。球队的得分是球队中所有球员的分数 **总和** ，返回 **所有可能的无矛盾球队中得分最高那支的分数** 。

相似题目
- [300. 最长递增子序列](https://leetcode.cn/problems/longest-increasing-subsequence/)
- [354. 俄罗斯套娃信封问题](https://leetcode.cn/problems/russian-doll-envelopes/)
- [1691. 堆叠长方体的最大高度](https://leetcode.cn/problems/maximum-height-by-stacking-cuboids/)
- [2407. 最长递增子序列 II](https://leetcode.cn/problems/longest-increasing-subsequence-ii/)

---
### 解法1 排序+动态规划
由于**球员在数组中的下标不重要**，所以可以按照分数或者年龄排序。这里按照分数从小到大排序，分数相同的按照年龄从小到大排序。假设第 $i$ 个人是**球队中**下标最大的，那么对于在球队中的下标比 $i$ 小的 $j$ ，必然有 $\textit{scores}[j]\le\textit{scores}$ ：
- 如果 $scores[j]=scores[i]$ ，由于分数相同的按照年龄从小到大排序，所以 $\textit{ages}[j]\le \textit{ages}[i]$ ；
- **如果 $\textit{scores}[j]<\textit{scores}[i]$ ，根据题目要求，必须满足 $ages[j]≤ages[i]$** 。所以 $ages[j]≤ages[i]$ 。

也就是说，需要从 $ages$ 中选择一个得分之和最大的递增子序列（允许有相同元素），这与[300. 最长递增子序列](https://leetcode.cn/problems/longest-increasing-subsequence/)类似。排序后，仿照最长递增子序列，定义 $f[i]$ 表示**以 $ages[i]$ 结尾的递增子序列的最大得分**，则有转移方程：
$$f[i]=max⁡(f[j])+scores[i]$$
其中 $j<i$ 且 $ages[j]≤ages[i]$ 。初始 $f[i]=0$ 。答案为 $max⁡(f[i])$ 。
> 问：能否先按照年龄，再按照分数排序？
> 答：可以的。分析过程同上。
```cpp
class Solution {
public:
    int bestTeamScore(vector<int> &scores, vector<int> &ages) {
        int n = scores.size();
        pair<int, int> a[n];
        for (int i = 0; i < n; ++i)
            a[i] = {scores[i], ages[i]};
        sort(a, a + n);
        int f[n]; memset(f, 0, sizeof(f));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j)
                if (a[j].second <= a[i].second) // 枚举年龄不超过ages[i]的队
                    f[i] = max(f[i], f[j]);
            f[i] += a[i].first;
        }
        return *max_element(f, f + n); 
    }
};
```
复杂度分析：’
- 时间复杂度：$O(n^2)$ ，其中 $n$ 为 $ages$ 的长度。
- 空间复杂度：$O(n)$ 。

---
### 解法2 基于值域计算
同样按照方法一排序。注意到 $\textit{ages}[i]$ 的范围比较小，考虑从值域来计算。在遍历球员的同时，维护数组 $\textit{maxSum}$ ，**其中 $maxSum[x]$ 表示球队中球员年龄最大值恰好等于 $x$ 的球队最大分数和**（即球队以年龄 $x$ 结尾）。

枚举年龄不超过 $ages[i]$ 的 $y$ ，有
$$\textit{maxSum}[\textit{ages}[i]] = \max\limits_{y=1}^{\textit{ages}[i]} \{\textit{maxSum}[y]\} + \textit{scores}[i]$$
初始 $maxSum[x]=0$ ，答案为 $\max(\textit{maxSum}[x])$ ——其实可以换成 **$f[ages[i]]$ 表示球队中球员年龄最大值恰好等于 $ages[i]$ 的球队最大分数和**，答案为 $\max (f[ages[i]])$ 。
> 问：怎么想到要从值域思考的，这是一个套路吗？
> 答：**LIS问题除了有 $O(n\log⁡n)$ 的二分做法外，还有基于值域的线段树做法**，具体可以看[2407. 最长递增子序列 II]()。
```cpp
class Solution {
public:
    int bestTeamScore(vector<int> &scores, vector<int> &ages) {
        int n = scores.size();
        pair<int, int> a[n];
        for (int i = 0; i < n; ++i)
            a[i] = {scores[i], ages[i]};
        sort(a, a + n);

        int u = *max_element(ages.begin(), ages.end());
        int max_sum[u + 1]; memset(max_sum, 0, sizeof(max_sum));
        for (auto &[score, age] : a)
            max_sum[age] = *max_element(max_sum, max_sum + age + 1) + score; // 枚举年龄不超过ages[i]的球队
        return *max_element(max_sum, max_sum + u + 1);
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n + nU)$ ，其中 $n$ 为 $\textit{ages}$ 的长度，$U=\max(\textit{ages})$ 。
- 空间复杂度：$O(n+U)$ 。
- 如果对 $\textit{ages}$ 离散化，可以做到 $O(n^2)$ 时间复杂度和 $O(n)$ 空间复杂度。

---
### 解法3 树状数组（变形）优化
解法2循环中计算的是 $maxSum$ 的**前缀中的最大值**，这可以用树状数组（或者线段树）优化至 $O(\log U)$（若先离散化，则为 $O(\log⁡n)$ 。
```cpp
```cpp
class Solution {
private:
    static constexpr int MAX_LEN = 1000;
    int tree[MAX_LEN + 1];
    void update(int i, int maxSum) { // 更新maxSum[i]为maxSum,
        for (; i <= MAX_LEN; i += (i & (-i))) // i后面的maxSum[j]也受到影响
            tree[i] = max(tree[i], maxSum);
    }
    int query(int i) { // 返回max(maxSum[0:i+1])
        int ans = 0;
        for (; i; i -= (i & (-i)))
            ans = max(tree[i], ans);
        return ans;
    }
public:
    int bestTeamScore(vector<int> &scores, vector<int> &ages) {
        int n = scores.size();
        pair<int, int> a[n];
        for (int i = 0; i < n; ++i)
            a[i] = {scores[i], ages[i]};
        sort(a, a + n);
        for (auto &[score, age] : a) { // 更新maxSum[age]]为max(maxsum[0:age+1])+score
            update(age, query(age) + score);
        }
        return query(MAX_LEN);
    }
};
```
复杂度分析：
- 时间复杂度：$O(n\log n + n\log U)$ ，其中 $n$ 为 $\textit{ages}$ 的长度，$U=\max(\textit{ages})$。排序的时间复杂度为 $O(n\log n)$ ，单次查询和修改树状数组的时间复杂度都是 $O(\log U)$ ，一共操作 $O(n)$ 次，所以总的时间复杂度为 $O(n\log n + n\log U)$ 。
- 空间复杂度：$O(n+U)$ 。
- 如果对 $\textit{ages}$ 离散化，可以做到 $O(n\log n)$ 时间复杂度和 $O(n)$ 空间复杂度。本题由于 $U$ 比较小，这一优化并不明显。
