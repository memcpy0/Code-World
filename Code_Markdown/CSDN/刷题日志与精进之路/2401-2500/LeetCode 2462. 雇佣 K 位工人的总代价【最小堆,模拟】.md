> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `costs` ，其中 `costs[i]` 是雇佣第 `i` 位工人的代价。

同时给你两个整数 `k` 和 `candidates` 。我们想根据以下规则恰好雇佣 `k` 位工人：

- 总共进行 `k` 轮雇佣，且每一轮恰好雇佣一位工人。
- 在每一轮雇佣中，从最前面 `candidates` 和最后面 `candidates` 人中选出代价最小的一位工人，如果有多位代价相同且最小的工人，选择下标更小的一位工人。
    - 比方说，`costs = [3,2,7,7,1,2]` 且 `candidates = 2` ，第一轮雇佣中，我们选择第 `4` 位工人，因为他的代价最小 `[_3,2_,7,7,_1,2_]` 。
    - 第二轮雇佣，我们选择第 `1` 位工人，因为他们的代价与第 `4` 位工人一样都是最小代价，而且下标更小，`[_3,2_,7,_7,2_]` 。注意每一轮雇佣后，剩余工人的下标可能会发生变化。
- 如果剩余员工数目不足 `candidates` 人，那么下一轮雇佣他们中代价最小的一人，如果有多位代价相同且最小的工人，选择下标更小的一位工人。
- 一位工人只能被选择一次。

返回雇佣恰好 `k` 位工人的总代价。

**示例 1：**
<pre><b>输入：</b>costs = [17,12,10,2,7,2,11,20,8], k = 3, candidates = 4
<b>输出：</b>11
<b>解释：</b>我们总共雇佣 3 位工人。总代价一开始为 0 。
- 第一轮雇佣，我们从 [<strong><em>17,12,10,2</em></strong>,7,<strong><em>2,11,20,8</em></strong>] 中选择。最小代价是 2 ，有两位工人，我们选择下标更小的一位工人，即第 3 位工人。总代价是 0 + 2 = 2 。
- 第二轮雇佣，我们从 [<strong><em>17,12,10,7</em></strong>,<strong><em>2,11,20,8</em></strong>] 中选择。最小代价是 2 ，下标为 4 ，总代价是 2 + 2 = 4 。
- 第三轮雇佣，我们从 [<strong><em>17,12,10,7,11,20,8</em></strong>] 中选择，最小代价是 7 ，下标为 3 ，总代价是 4 + 7 = 11 。注意下标为 3 的工人同时在最前面和最后面 4 位工人中。
总雇佣代价是 11 。
</pre>

**示例 2：**
<pre><b>输入：</b>costs = [1,2,4,1], k = 3, candidates = 3
<b>输出：</b>4
<b>解释：</b>我们总共雇佣 3 位工人。总代价一开始为 0 。
- 第一轮雇佣，我们从 [<strong><em>1,2,4,1</em></strong>] 中选择。最小代价为 1 ，有两位工人，我们选择下标更小的一位工人，即第 0 位工人，总代价是 0 + 1 = 1 。注意，下标为 1 和 2 的工人同时在最前面和最后面 3 位工人中。
- 第二轮雇佣，我们从 [<strong><em>2,4,1</em></strong>] 中选择。最小代价为 1 ，下标为 2 ，总代价是 1 + 1 = 2 。
- 第三轮雇佣，少于 3 位工人，我们从剩余工人 [<strong><em>2,4</em></strong>] 中选择。最小代价是 2 ，下标为 0 。总代价为 2 + 2 = 4 。
总雇佣代价是 4 。
</pre>

**提示：**
- `1 <= costs.length <= 10^5`
- `1 <= costs[i] <= 10^5`
- `1 <= k, candidates <= costs.length`

---
### 解法 两个最小堆+模拟
雇佣过程可以用两个最小堆来模拟，一个负责维护 $\textit{costs}$ 剩余数字的最前面 $\textit{candidates}$ 个数的最小值，另一个负责维护 $\textit{costs}$ 剩余数字的最后面 $\textit{candidates}$ 个数的最小值。

具体算法如下：
- 设 $\textit{costs}$ 的长度为 $n$ 。**如果 $candidates⋅2+k>n$ ，我们一定可以选到 $\textit{costs}$ 中最小的 $k$ 个数，所以直接返回 $\textit{cost}$ 的前 $k$ 小之和**。
- 初始化答案 $ans=0$ 。初始化最小堆 $\textit{pre}$ 为 $\textit{costs}$ 最前面的 $\textit{candidates}$ 个数，初始化最小堆 $\textit{suf}$ 为 $\textit{costs}$ 最后面的 $\textit{candidates}$ 个数。初始化下标 $i=candidates, j=n−1−candidates$ 。
- 循环 $k$ 次。每次循环，如果 $pre$ 的堆顶小于等于 $suf$ 的堆顶，则弹出 $pre$ 的堆顶，加入答案，然后把 $costs[i]$ 加入 $pre$ ，$i$ 增加 $1$ ；如果 $suf$ 的堆顶小于 $pre$ 的堆顶，则弹出 $suf$ 的堆顶，加入答案，然后把 $costs[j]$ 加入 $suf$ ，$j$ 减少 $1$ 。
- 返回答案。

```python
class Solution:
    def totalCost(self, costs: List[int], k: int, candidates: int) -> int:
        n = len(costs)
        if candidates * 2 + k > n:
            # 也可以sum(nsmallest(k, costs)), 但效率不如直接排序
            costs.sort()
            return sum(costs[:k])
        
        pre = costs[:candidates]
        suf = costs[-candidates:]
        heapify(pre)
        heapify(suf)
        
        ans = 0
        i, j = candidates, n - 1 - candidates
        for _ in range(k):
            if pre[0] <= suf[0]:
                ans += heapreplace(pre, costs[i])
                i += 1
            else:
                ans += heapreplace(suf, costs[j])
                j -= 1
        return ans
```

```java
class Solution {
    public long totalCost(int[] costs, int k, int candidates) {
        int n = costs.length;
        long ans = 0;
        if (candidates * 2 + k > n) {
            Arrays.sort(costs);
            for (int i = 0; i < k; ++i) ans += costs[i];
            return ans;
        }
        PriorityQueue<Integer> pre = new PriorityQueue<>();
        PriorityQueue<Integer> suf = new PriorityQueue<>();
        for (int i = 0; i < candidates; ++i) {
            pre.offer(costs[i]);
            suf.offer(costs[n - 1 - i]);
        }
        int i = candidates;
        int j = n - 1 - candidates;
        while (k-- > 0) {
            if (pre.peek() <= suf.peek()) {
                ans += pre.poll();
                pre.offer(costs[i++]);
            } else {
                ans += suf.poll();
                suf.offer(costs[j--]);
            }
        }
        return ans;
    }
}
```

```cpp
class Solution {
public:
    long long totalCost(vector<int>& costs, int k, int candidates) {
        int n = costs.size();
        if (candidates * 2 + k > n) {
            ranges::nth_element(costs, costs.begin() + k);
            return reduce(costs.begin(), costs.begin() + k, 0LL);
        }
        priority_queue<int, vector<int>, greater<>> pre, suf;
        for (int i = 0; i < candidates; ++i) {
            pre.push(costs[i]);
            suf.push(costs[n - 1 - i]);
        }
        long long ans = 0;
        int i = candidates, j = n - 1 - candidates;
        while (k--) {
            if (pre.top() <= suf.top()) {
                ans += pre.top();
                pre.pop();
                pre.push(costs[i++]);
            } else {
                ans += suf.top();
                suf.pop();
                suf.push(costs[j--]);
            }
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$\mathcal{O}((c+k)\log (c+k))$ ，其中 $c=\textit{candidate}$ 。
- 空间复杂度：$\mathcal{O}(c)$ 。
