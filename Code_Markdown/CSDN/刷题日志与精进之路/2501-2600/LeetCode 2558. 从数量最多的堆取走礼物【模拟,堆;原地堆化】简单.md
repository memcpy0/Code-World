> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个整数数组 `gifts` ，表示各堆礼物的数量。每一秒，你需要执行以下操作：
- 选择礼物数量最多的那一堆。
- 如果不止一堆都符合礼物数量最多，从中选择任一堆即可。
- 选中的那一堆留下平方根数量的礼物（向下取整），取走其他的礼物。

返回在 `k` 秒后剩下的礼物数量。

**示例 1：**
```js
输入：gifts = [25,64,9,4,100], k = 4
输出：29
解释：
按下述方式取走礼物：
- 在第一秒，选中最后一堆，剩下 10 个礼物。
- 接着第二秒选中第二堆礼物，剩下 8 个礼物。
- 然后选中第一堆礼物，剩下 5 个礼物。
- 最后，再次选中最后一堆礼物，剩下 3 个礼物。
最后剩下的礼物数量分别是 [5,8,9,4,3] ，所以，剩下礼物的总数量是 29 。
```
**示例 2：**
```js
输入：gifts = [1,1,1,1], k = 4
输出：4
解释：
在本例中，不管选中哪一堆礼物，都必须剩下 1 个礼物。 
也就是说，你无法获取任一堆中的礼物。 
所以，剩下礼物的总数量是 4 。
```
**提示：**
- `1 <= gifts.length <= 10^3`
- `1 <= gifts[i] <= 10^9`
- `1 <= k <= 10^3`

---
相似题目：
- [2558. 从数量最多的堆取走礼物](https://leetcode.cn/problems/take-gifts-from-the-richest-pile/)
- [2530. 执行 K 次操作后的最大分数](https://leetcode.cn/problems/maximal-score-after-applying-k-operations/)
- [1962. 移除石子使总数最小](https://leetcode.cn/problems/remove-stones-to-minimize-the-total/)
- [2208. 将数组和减半的最少操作次数](https://leetcode.cn/problems/minimum-operations-to-halve-array-sum/)
- [2233. K 次增加后的最大乘积](https://leetcode.cn/problems/maximum-product-after-k-increments/)
### 解法 模拟+堆
维护一些数的最大值，可以最大堆模拟。循环 $k$ 次。每次循环，弹出堆顶 $\textit{top}$ ，然后把 $\left\lfloor\sqrt{\textit{top}}\right\rfloor$  入堆。循环结束后，堆中所有元素之和就是答案。
```java
class Solution {
    public long pickGifts(int[] gifts, int k) {
        var pq = new PriorityQueue<Integer>(gifts.length, (Integer a, Integer b) -> {
            return b- a;
        });
        for (int gift : gifts) pq.add(gift);
        for (int i = 0; i < k; ++i) {
            long m = pq.poll();
            pq.add((int) Math.sqrt(m)); // 留下平方根数量的礼物
        }
        long ans = 0;
        while (!pq.isEmpty()) ans += pq.poll();
        return ans;
    }
} 
```
**优化**
1. 如果堆顶等于 $1$ ，说明堆中所有元素都为 $1$ 。由于 $\sqrt{1} = 1$ ，后续操作无法修改任何元素，可以直接退出循环。
2. 原地堆化 `heapify` 可以做到 $\mathcal{O}(1)$ 的空间复杂度。部分语言用的标准库自带的堆化函数。

```cpp
class Solution {
public:
    long long pickGifts(vector<int>& gifts, int k) {
        make_heap(gifts.begin(), gifts.end()); // 原地堆化,最大堆
        while (k-- && gifts[0] > 1) {
            pop_heap(gifts.begin(), gifts.end()); // 弹出堆顶并移到末尾
            gifts.back() = sqrt(gifts.back());
            push_heap(gifts.begin(), gifts.end()); // 把末尾元素入堆
        }
        return accumulate(gifts.begin(), gifts.end(), 0LL);
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}(n + k\log n)$ ，其中 $n$ 为 $\textit{gifts}$ 的长度。堆化需要 $\mathcal{O}(n)$ 的时间（证明见下）。每次修改堆顶，需要 $\mathcal{O}(\log n)$ 的时间。计算平方根有专门的 CPU 指令，可以视为 $\mathcal{O}(1)$ 时间。所以总的时间复杂度为 $\mathcal{O}(n + k\log n)$ 。
- 空间复杂度：$\mathcal{O}(1)$ ，仅用到若干额外变量。