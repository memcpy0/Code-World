> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

There are `N` children standing in a line. Each child is assigned a rating value.

You are giving candies to these children subjected to the following requirements:
- Each child must have at least one candy.
- Children with a higher rating get more candies than their neighbors.

What is the minimum candies you must give?

**Example 1:**
```cpp
Input: [1,0,2]
Output: 5
Explanation: You can allocate to the first, second and third child with 2, 1, 2 candies respectively.
```
**Example 2:**
```cpp
Input: [1,2,2]
Output: 4
Explanation: You can allocate to the first, second and third child with 1, 2, 1 candies respectively.
             The third child gets 1 candy because it satisfies the above two conditions.
```
题意：老师想给孩子们分发糖果，有 `N` 个孩子站成了一条直线，老师会根据每个孩子的表现，预先给他们评分。按照以下要求，帮助老师给这些孩子分发糖果：
- 每个孩子至少分配到 `1` 个糖果。
 - 相邻的孩子中，评分高的孩子必须获得更多的糖果。

那么这样下来，老师至少需要准备多少颗糖果呢？

---
### 解法 贪心+两次遍历
> 感觉和**PAT（乙级）2020年秋季考试**中的**胖达与盆盆奶**差不多。

我们可以将「相邻的孩子中，评分高的孩子必须获得更多的糖果」这句话拆分为两个规则，分别处理。
1. 左规则：当 $ratings[i - 1]< ratings[i]$ 时，$i$ 号学生的糖果数量将比 $i - 1$ 号孩子的糖果数量多。
2. 右规则：当 $\textit{ratings}[i] > \textit{ratings}$ 时，$i$ 号学生的糖果数量将比 $i + 1$ 号孩子的糖果数量多。

我们遍历该数组两次，处理出每一个学生分别满足左规则或右规则时，**最少需要被分得的糖果数量**。每个人最终分得的糖果数量即为==这两个数量的最大值==。

具体地，以左规则为例：我们从左到右遍历该数组，假设当前遍历到位置 $i$ ，如果有 $ratings[i - 1]< ratings[i]$ 那么 $i$ 号学生的糖果数量将比 $i - 1$ 号孩子的糖果数量多，我们令 $left[i]=left[i−1]+1$ 即可，否则我们令 $left[i]=1$ 。

在实际代码中，我们先计算出左规则 $\textit{left}$ 数组，==在计算右规则时只需要用单个变量记录当前位置的右规则==，同时计算答案即可。
```cpp
class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        vector<int> left(n);
        for (int i = 0; i < n; i++) {
            if (i > 0 && ratings[i] > ratings[i - 1]) {
                left[i] = left[i - 1] + 1;
            } else {
                left[i] = 1;
            }
        }
        int right = 0, ret = 0;
        for (int i = n - 1; i >= 0; i--) {
            if (i < n - 1 && ratings[i] > ratings[i + 1]) {
                right++;
            } else {
                right = 1;
            }
            ret += max(left[i], right);
        }
        return ret;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是孩子的数量。我们需要遍历两次数组以分别计算满足左规则或右规则的最少糖果数量。
- 空间复杂度：$O(n)$ ，其中 $n$ 是孩子的数量。我们需要保存所有的左规则对应的糖果数量。

---
### 解法2 一次遍历+常数空间
注意到糖果总是尽量少给，且从 $1$ 开始累计，每次要么比相邻的同学多给一个，要么重新置为 $1$ 。依据此规则，我们可以画出下图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309131306444.png)
其中相同颜色的柱状图的高度总恰好为 $1,2,3 \dots$ 。

而高度也不一定一定是升序，也可能是 $\dots 3,2,1$ 的降序：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309131307304.png)
注意到在上图中，==对于第三个同学，他既可以被认为是属于绿色的升序部分，也可以被认为是属于蓝色的降序部分==，因为他同时比两边的同学评分更高。

我们对序列稍作修改，注意到右边的升序部分变长了，使得第三个同学不得不被分配 $4$ 个糖果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309131307329.png)
依据前面总结的规律，我们可以提出本题的解法。我们从左到右枚举每一个同学，记前一个同学分得的糖果数量为 $\textit{pre}$ ：
- **如果当前同学比上一个同学评分高，说明我们就在最近的递增序列中**，直接分配给该同学 $pre+1$ 个糖果即可。
- 否则我们就在一个递减序列中，==我们直接分配给当前同学一个糖果，并把该同学**所在的递减序列中**（位于前面）所有的同学都再多分配一个糖果==，以保证糖果数量还是满足条件。
  我们无需显式地额外分配糖果，**只需要记录当前的递减序列长度，即可知道需要额外分配的糖果数量**。
  同时注意，当前的递减序列长度和上一个递增序列等长时，需要把最近的递增序列的最后一个同学也并进递减序列中。

这样，我们只要记录当前递减序列的长度 $\textit{dec}$ ，最近的递增序列的长度 $\textit{inc}$ 和前一个同学分得的糖果数量 $\textit{pre}$ 即可。
```cpp
class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        int ret = 1;
        int inc = 1, dec = 0, pre = 1;
        for (int i = 1; i < n; i++) {
            if (ratings[i] >= ratings[i - 1]) {
                dec = 0;
                pre = ratings[i] == ratings[i - 1] ? 1 : pre + 1;
                ret += pre;
                inc = pre;
            } else {
                dec++;
                if (dec == inc) {
                    dec++;
                }
                ret += dec; // 给递减序列（包括当前同学）再分一个糖果
                pre = 1; // 表示已经给当前同学分一个糖果（上一步中）
            }
        }
        return ret;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是孩子的数量。
- 空间复杂度：$O(1)$ 。我们只需要常数的空间保存若干变量。