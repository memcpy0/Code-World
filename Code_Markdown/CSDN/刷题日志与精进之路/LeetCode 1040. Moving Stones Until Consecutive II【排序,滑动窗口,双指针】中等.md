> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304071602714.png)
There are some stones in different positions on the X-axis. You are given an integer array `stones`, the positions of the stones.

Call a stone an **endpoint stone** if it has the smallest or largest position. In one move, you pick up an **endpoint stone** and move it to an unoccupied position so that it is no longer an **endpoint stone**.
-   In particular, if the stones are at say, `stones = [1,2,5]`, you cannot move the endpoint stone at position `5`, since moving it to any position (such as `0`, or `3`) will still keep that stone as an endpoint stone.

The game ends when you cannot make any more moves (i.e., the stones are in three consecutive positions).

Return _an integer array_ `answer` _of length_ `2` _where_:
-   `answer[0]` _is the minimum number of moves you can play, and_
-   `answer[1]` _is the maximum number of moves you can play_.

**Example 1:**
```java
Input: stones = [7,4,9]
Output: [1,2]
Explanation: We can move 4 -> 8 for one move to finish the game.
Or, we can move 9 -> 5, 4 -> 6 for two moves to finish the game.``
```
**Example 2:**
```java
Input: stones = [6,5,4,3,10]
Output: [2,3]
Explanation: We can move 3 -> 8 then 10 -> 7 to finish the game.
Or, we can move 3 -> 7, 4 -> 8, 5 -> 9 to finish the game.
Notice we cannot move 10 -> 2 to finish the game, because that would be an illegal move.
```
**Constraints:**
-   `3 <= stones.length <= 10^4`
-   `1 <= stones[i] <= 10^9`
-   All the values of `stones` are **unique**.

题意：X轴上一些石头在不同的位置。数组 `stones` 描述了这些石头的下标位置。如果一个石头有最小或最大的下标位置，则叫它**端点** *endpoint stone* 。一次移动中，我们可以选择一个端点石头、并将它移动到一个空位置、使它不再是一个端点。无法进行更多移动时，游戏结束，返回数组 `answer[2]` ，`answer[0]` 表示能进行的最少移动次数，`answer[1]` 表示能移动的最多次数。

---
### 解法 滑窗+同向双指针
题目的限制可以概括为两点：
1. 只能移动端点。
2. 端点移动后不能还是端点，且只能移动到空位上。

且**移动后端点间距离一定会变小，除非游戏结束**。如下所示，移动前端点距离为 $6$ ，移动后端点距离为 $4$ ：
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304072345937.png)
由于 $n \ge 3$ ，如果右边有空位，可以让左端点移过去；如果左边有空位，可以让右端点移过去。==没有空位时无法移动，此时所有石头都紧密相连：石头个数等于石头序列的长度==。

下面讨论怎样做到最大移动次数。显然，如果像老太太慢悠悠一步一步走路来移动，就能得到最大移动次数。即==每次移动都只让端点距离缩小 $1$ ，那么移动次数都能达到最大==。但**第一步可能无法这样移动**、会多移动一点，还有一些情况下某个端点也无法做到这一点。比如下图，第一步要么 $1$ 移动到 $4 \sim 6$（此后 $1$ 不是端点），要么 $7$ 移动到 $2$ （此后 $7$ 不是端点）；显然，$7$ 移动到 $2$ ，后面就无法移动了；$1$ 移到 $4$ ，后面就可像下跳棋一样、借助后面的石头一步一步“跳”过去，从而多移动几次。
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304072352001.png)
上图中移动的最大次数为 $3$ ，是通过**不断移动左端点到 $(s[1], s[n-1])$ 区间中的空位**达成的。因此最大移动次数的结论（简记 $stones$ 为 $s$ ）：
> 先将数组 $s$ 从小到大排序，将石头整齐排列。最大移动次数一定等于如下两种情况的最大值：
> - 从 $s[0]$ 到 $s[n - 2]$ 之间的空位个数。
> - 从 $s[1]$ 到 $s[n-1]$ 之间的空位个数。
> - 即 $\max ( s[n - 2] - s[0] - n + 2, s[n - 1] - s[1] - n + 2)$ 。

这是什么意思？
- **要往左移动端点 $s[n-1]$ ，由于移动后不能还让它是端点，就只能移到开区间 $(s[0], s[n-2])$ 中**，这一开区间中有 $s[n - 2] - s[0] - 1$ 个位置，去掉已知的三颗石头 $s[0], s[n - 2], s[n-1]$ ，剩下还有 $n - 3$ 颗石头占有开区间中的位置，剩下的空位就是 $s[n - 2] - s[0] - 1- (n-3) = s[n - 2] - s[0] - n + 2$ 。
-  **要往右移动端点 $s[0]$ ，由于移动后不能还让它是端点，就只能移到开区间 $(s[1], s[n-1])$ 中**，这一开区间中有 $s[n - 1] - s[1] - 1$ 个位置，去掉已知的三颗石头 $s[n-1], s[0], s[1]$ ，剩下还有 $n - 3$ 颗石头占有开区间中的位置，剩下的空位就是 $s[n - 1] - s[1] - 1 - (n-3) = s[n - 1] - s[1] - n + 2$ 。

下面讨论最小移动次数，既然端点可以移动到中间任意空位，那么将所有石头都**一步到位**就是最快的，最后 $n$ 个石头形成数轴长为 $n$ 的序列。于是问题转换为：==找到长为 $n$ 且具有最少空位的窗口，这样不在窗口的石头就最少，移动次数也最少==。计算长度为 $n$ 的窗口中最少有多少个空位，等价于 $n$ 减去窗口内的最多石头个数。由于窗口滑动到下个石头前，窗口中的石头不会变多，就可以**直接滑到下一颗石头**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304080925874.png)
从而最小移动次数的结论是：
> 数组 $stones$ 简记为 $s$ ，将 $s$ 从小到大排序。
> - **如果从 $s[0]$ 到 $s[n-2]$ 之间或从 $s[1]$ 到 $s[n - 1]$ 之间没有空位，那么答案为 $2$** ，但这不应当比最大移动次数还大，比如 $[1,2,4]$ 只需移动 $1$ 次。所以这种情况的最小移动次数为 $\min(2, maxMove)$ 。
> - 如果都有空位，那么用同向双指针（即滑动窗口）来做。==枚举窗口右端点所在石头，计算窗口内最大石头个数，最后用 $n$ 减去窗口内最大石头个数，即为最小移动次数==。

特殊情况就是：下图中无法直接把 $1$ 移到窗口中的空位 $5$ ，即**第一步出现问题**。为此需要先移动另外一个端点、将窗口改变，才能把 $1$ 移动到 $5$ 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304080127961.png)
 ```cpp
class Solution {
public:
    vector<int> numMovesStonesII(vector<int> &s) {
        sort(s.begin(), s.end());
        int n = s.size();
        int e1 = s[n - 2] - s[0] - n + 2;
        int e2 = s[n - 1] - s[1] - n + 2;
        int max_move = max(e1, e2);
        if (e1 == 0 || e2 == 0) // 特殊情况：没有空位
            return {min(2, max_move), max_move};
        int max_cnt = 0, left = 0;
        for (int right = 0; right < n; ++right) { // 滑动窗口：枚举右端点
            while (s[right] - s[left] + 1 > n) // 窗口大小大于 n
                ++left;
            max_cnt = max(max_cnt, right - left + 1); // 维护窗口内的最大石子数
        }
        return {n - max_cnt, max_move};
    }
};
```
复杂度分析
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 为 $stones$ 的长度。瓶颈在排序上。
- 空间复杂度：$O(1)$ 。忽略排序时的栈开销，仅用到若干额外变量。
> [灵神](https://leetcode.cn/problems/moving-stones-until-consecutive-ii/solutions/2212638/tu-jie-xia-tiao-qi-pythonjavacgo-by-endl-r1eb/)问：
> 问：在窗口右边界 $s[right]$ 比较小时（比如 $right=0$ ），窗口内部的左边都是空的，无法把石子移过去，为什么代码中没有判断这种不合法的情况呢？
> 答：无需考虑这种情况。如果窗口内部左边都是空的，那么继续向右滑动窗口，在窗口左边界有石子之前，窗口内的石子个数是不会减少的，后面算出来的 $maxCnt$ 必然更大，所以无需考虑窗口内部左边都是空的情况。代码实现时，在 $right$ 比较小时也做了计算，因为在取 $\max$⁡ 的情况下，这对最终的 $maxCnt$ 是没有影响的。
> <b></b>
> 问：对于最小移动次数，除了图中的「特殊情况」外，是否还存在其它的特殊情况？你能构造出一个具体的移动方式吗？
> 答：没有其它的特殊情况了，因为一定可以通过如下方式完成移动：
> - 首先，按照算法流程，**窗口右边界（$\textit{right}$）一定可以位于一颗石子上**。因为在窗口滑到下一颗石子之前，窗口内的石子不会增加，所以只需要考虑窗口右边界在石子上的情况。
> - 情况一：窗口左边界也有石子。那么窗口内任意空位的左右两侧都有石子，**窗口外的石子怎么移动都行**。注意，窗口左边界是 $s[right] - n + 1$ ，它 $\le s[left]$ 。
> - 情况二：窗口左边界没有石子，且窗口右边界的右侧还有石子。那么**把右端点石子移到窗口的左边界上，就能转换到情况一**。
> - 情况三：窗口左边界没有石子，且窗口右边界的右侧也没有石子（即窗口右边界的石子就是右端点石子）。由于不是特殊情况，窗口左侧必然有至少 $2$ 颗石子，那么左端点石子可以移到窗口的左边界上，这样就转换到了情况一。（具体见「最小移动次数讨论」的第三幅图。）
> <b></b>
> 问：为什么不在滑动窗口内判断特殊情况？
> 答：不需要，在滑动窗口之前判断就行。这是因为如果不是特殊情况，窗口内至少有 $2$ 个空位，所以在窗口滑动时，是无论如何都不会出现特殊情况的。
> <b></b>
> 问：你是如何想到本题的做法的？是否有一些通用的思考方式？
> 灵神答：个人觉得这题有点构造的味道（想算出答案，要大致知道怎么移动石子）。对于构造题，通常是先从最基本的情况开始思考，比如本题就是从 $n=3$ 开始思考。在纸上多画一画，比较不同的移动方案，猜想出一个大致的结论。接着思考 $n=4,5,⋯$ 的情况，验证/修正你的结论。这就是「**从特殊到一般**」。如果你想做更多的构造题，可以去Codeforces搜索 tag：`constructive algorithms` 。

相似题目：
- 3. 无重复字符的最长子串 
- 209. 长度最小的子数组 
- 713. 乘积小于 K 的子数组 
- 1004. 最大连续 1 的个数 III
- 1234. 替换子串得到平衡字符串 
- 1658. 将 x 减到 0 的最小操作数 


