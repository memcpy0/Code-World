> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

有 `n` 个人前来排队买票，其中第 `0` 人站在队伍 **最前方** ，第 `(n - 1)` 人站在队伍 **最后方** 。

给你一个下标从 **0** 开始的整数数组 `tickets` ，数组长度为 `n` ，其中第 `i` 人想要购买的票数为 `tickets[i]` 。

每个人买票都需要用掉 **恰好 1 秒** 。一个人 **一次只能买一张票** ，如果需要购买更多票，他必须走到  **队尾** 重新排队（**瞬间** 发生，不计时间）。如果一个人没有剩下需要买的票，那他将会 **离开** 队伍。

返回位于位置 `k`（下标从 **0** 开始）的人完成买票需要的时间（以秒为单位）。

**示例 1：**
```c
输入：tickets = [2,3,2], k = 2
输出：6
解释：
- 队伍一开始为 [2,3,2]，第 k 个人以下划线标识。
- 在最前面的人买完票后，队伍在第 1 秒变成 [3,2,1]。
- 继续这个过程，队伍在第 2 秒变为[2,1,2]。
- 继续这个过程，队伍在第 3 秒变为[1,2,1]。
- 继续这个过程，队伍在第 4 秒变为[2,1]。
- 继续这个过程，队伍在第 5 秒变为[1,1]。
- 继续这个过程，队伍在第 6 秒变为[1]。第 k 个人完成买票，所以返回 6。
```
**示例 2：**
```c
输入：tickets = [5,1,1,1], k = 0
输出：8
解释：
- 队伍一开始为 [5,1,1,1]，第 k 个人以下划线标识。
- 在最前面的人买完票后，队伍在第 1 秒变成 [1,1,1,4]。
- 继续这个过程 3 秒，队伍在第 4 秒变为[4]。
- 继续这个过程 4 秒，队伍在第 8 秒变为[]。第 k 个人完成买票，所以返回 8。
```
**提示：**
- `n == tickets.length`
- `1 <= n <= 100`
- `1 <= tickets[i] <= 100`
- `0 <= k < n`

---
### 方法 O(n)一次遍历，简洁写法
想一想，当第 $k$ 个人完成买票的那一刻，在他前后的人，分别买了多少票？

假设第 $k$ 个人此时买了 $3$ 张票，那么排在他前面的人，此时也**至多**买了 $3$ 张票；排在他后面的人，此时**至多**买了 $2$ 张票。

把 $tickets$ 简记为 $t$ 。一般地，当第 $k$ 个人买了 $t_k$ 张票时：
- 排在他前面的人，买的票**不会超过** $t_k$ ，即 $\min(t_i,t_k)$ 。
- 排在他后面的人，买的票**不会超过** $t_k−1$ ，即 $\min(t_i,t_k−1)$ 。

累加所有购票数即为答案。
```python
class Solution:
    def timeRequiredToBuy(self, tickets: List[int], k: int) -> int:
        tk = tickets[k]
        return sum(min(t, tk - (i > k)) for i, t in enumerate(tickets))
```
```cpp
class Solution {
public:
    int timeRequiredToBuy(vector<int>& tickets, int k) {
        int ans = 0, tk = tickets[k];
        for (int i = 0; i < tickets.size(); i++) {
            ans += min(tickets[i], tk - (i > k));
        }
        return ans;
    }
};
```
```java
class Solution {
    public int timeRequiredToBuy(int[] tickets, int k) {
        int ans = 0;
        int tk = tickets[k];
        for (int i = 0; i < tickets.length; i++) {
            ans += Math.min(tickets[i], (i <= k ? tk : tk - 1));
        }
        return ans;
    }
}
```
```c
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int timeRequiredToBuy(int* tickets, int ticketsSize, int k) {
    int ans = 0;
    int tk = tickets[k];
    for (int i = 0; i < ticketsSize; i++) {
        ans += MIN(tickets[i], tk - (i > k));
    }
    return ans;
}
```
```go
func timeRequiredToBuy(tickets []int, k int) (ans int) {
    tk := tickets[k]
    for i, t := range tickets {
        if i <= k {
            ans += min(t, tk)
        } else {
            ans += min(t, tk-1)
        }
    }
    return
}
```
```js
var timeRequiredToBuy = function(tickets, k) {
    const tk = tickets[k];
    let ans = 0;
    for (let i = 0; i < tickets.length; i++) {
        ans += Math.min(tickets[i], (i <= k ? tk : tk - 1));
    }
    return ans;
};
```
```rust
impl Solution {
    pub fn time_required_to_buy(tickets: Vec<i32>, k: i32) -> i32 {
        let k = k as usize;
        let tk = tickets[k];
        tickets.iter()
            .enumerate()
            .map(|(i, &t)| t.min(if i <= k { tk } else { tk - 1 }))
            .sum()
    }
}
```
**复杂度分析**
- 时间复杂度：$O(n)$ ，其中 $n$ 是 $tickets$ 的长度。
- 空间复杂度：$O(1)$ 。

**思考题**
1. 输入一个正整数 $q$ ，返回第 $q$ 秒谁在买票。你需要返回这个人的编号，一个 $0$ 到 $n−1$ 中的数。保证此时队列中还有人。
2. 输入一个询问数组 $queries$ ，对于第 $i$ 个询问，计算第 $queries[i]$ 秒谁在买票。保证此时队列中还有人。

你需要做到时间复杂度**与 $tickets[i]$ 的值域无关**。$tickets$ 和 $queries$ 的长度不超过 $10^5$ 。
