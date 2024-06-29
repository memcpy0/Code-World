> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的二维整数数组 `items` 和一个整数 `k` 。

`items[i] = [profiti, categoryi]`，其中 `profiti` 和 `categoryi` 分别表示第 `i` 个项目的利润和类别。

现定义 `items` 的 **子序列** 的 **优雅度** 可以用 `total_profit + distinct_categories2` 计算，其中 `total_profit` 是子序列中所有项目的利润总和，`distinct_categories` 是所选子序列所含的所有类别中不同类别的数量。

你的任务是从 `items` 所有长度为 `k` 的子序列中，找出 **最大优雅度** 。

用整数形式表示并返回 `items` 中所有长度恰好为 `k` 的子序列的最大优雅度。

**注意：** 数组的子序列是经由原数组删除一些元素（可能不删除）而产生的新数组，且删除不改变其余元素相对顺序。

**示例 1：**
```JAVA
输入：items = [[3,2],[5,1],[10,1]], k = 2
输出：17
解释：
在这个例子中，我们需要选出长度为 2 的子序列。
其中一种方案是 items[0] = [3,2] 和 items[2] = [10,1] 。
子序列的总利润为 3 + 10 = 13 ，子序列包含 2 种不同类别 [2,1] 。
因此，优雅度为 13 + 22 = 17 ，可以证明 17 是可以获得的最大优雅度。
``` 
**示例 2：**
```JAVA
输入：items = [[3,1],[3,1],[2,2],[5,3]], k = 3
输出：19
解释：
在这个例子中，我们需要选出长度为 3 的子序列。 
其中一种方案是 items[0] = [3,1] ，items[2] = [2,2] 和 items[3] = [5,3] 。
子序列的总利润为 3 + 2 + 5 = 10 ，子序列包含 3 种不同类别 [1, 2, 3] 。 
因此，优雅度为 10 + 32 = 19 ，可以证明 19 是可以获得的最大优雅度。
```
**示例 3：**
```JAVA
输入：items = [[1,1],[2,1],[3,1]], k = 3
输出：7
解释：
在这个例子中，我们需要选出长度为 3 的子序列。
我们需要选中所有项目。
子序列的总利润为 1 + 2 + 3 = 6，子序列包含 1 种不同类别 [1] 。
因此，最大优雅度为 6 + 12 = 7 。
```
**提示：**
- `1 <= items.length == n <= 10^5`
- `items[i].length == 2`
- `items[i][0] == profiti`
- `items[i][1] == categoryi`
- `1 <= profiti <= 10^9`
- `1 <= categoryi <= n`
- `1 <= k <= n`

---
### 解法 反悔贪心
按照利润从大到小排序。先把前 $k$ 个项目选上。

考虑选第 $k+1$ 个项目，为了选它，我们必须从前 $k$ 个项目中移除一个项目。由于已经按照利润从大到小排序，选这个项目不会让 $total\_profit$ 变大，所以我们重点考虑能否让 $distinct\_categories$ 变大。分类讨论：
1. 如果第 $k+1$ 个项目和前面某个已选项目的类别相同，那么无论怎么移除都不会让 $distinct\_categories$ 变大，所以无需选择这个项目。
2. 如果第 $k+1$ 个项目和前面任何已选项目的类别都不一样，考虑移除前面已选项目中的哪一个：
    1. 如果移除的项目的类别只出现一次，那么选第 $k+1$ 个项目后，$distinct\_categories$ 一减一增，保持不变，所以不考虑这种情况。
    2. 如果移除的项目的类别**重复出现多次**，那么选第 $k+1$ 个项目后，$distinct\_categories$ 会增加一，此时有可能会让优雅度变大，**一定要选择这个项目**。为什么说「**一定**」呢？因为 $total\_profit$ 只会变小，我们现在的目标就是让 $total\_profit$ 保持尽量大，同时让 $distinct\_categories$ 增加，那么**能让 $distinct\_categories$ 增加就立刻选上**！因为后面的利润更小，现在不选的话将来 $total\_profit$ 只会更小。

按照这个过程，继续考虑选择后面的项目。计算优雅度，取最大值，即为答案。

代码实现时，应移除已选项目中**类别和前面重复**且**利润最小**的项目，这可以用一个栈 $duplicate$ 来维护，==由于利润从大到小排序，所以栈顶就是最小的利润==；前面 $k$ 次中如果出现了多个重复项，则在栈中也会有多个项。注意，==对于后面的项目，由于我们只考虑之前没出现过的类别，也就是说这个后面的项目的类别只出现了一次==，所以不应当加到 $duplicate$ 中。
```cpp
class Solution {
public:
    long long findMaximumElegance(vector<vector<int>>& items, int k) {
        // 利润从大到小排序
        sort(items.begin(), items.end(), [&](const auto &a, const auto &b) {
            return a[0] > b[0];
        });
        long long totalProfit = 0, ans = 0;
        // 利润和 totalProfit 在最大 k 个利润的基础上不会变大
        unordered_set<int> vis; // 判断类别是否出现过
        stack<int> dup; // 重复类别的利润,栈顶最小
        for (int i = 0, n = items.size(); i < n; ++i) {
            int profit = items[i][0], category = items[i][1];
            if (i < k) {
                totalProfit += profit;
                if (vis.count(category)) dup.push(profit);
                else vis.insert(category);
            } // 如果新添加的项目的类别之前选过了,则distinct_categories不会变大 
            else if (!dup.empty() && !vis.count(category)) {
                // 如果新添加的项目的类别之前没有选过,distinct_categories^2可能变大
                vis.insert(category);
                // 我们移除最小利润的项目
                // 如果移除的项目的类别只有1个,则distinct_categories-1+1,不变,但总利润可能变小
                // 如果移除的项目的类别有多个,则distinct_categories+1,这种情况就是可行的
                totalProfit -= dup.top(); dup.pop(); // 移除掉一个重复且利润最小的项目
                totalProfit += profit; // 本项目目前只出现了一次,不应加入dup中;且以后出现也不会被考虑
            }
            ans = max(ans, totalProfit + (long long)(vis.size() * vis.size()));
        }
        return ans;
    }
};
```