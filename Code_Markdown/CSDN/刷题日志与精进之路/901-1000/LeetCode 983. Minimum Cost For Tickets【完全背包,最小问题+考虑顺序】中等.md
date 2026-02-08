> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309102049851.png)

在一个火车旅行很受欢迎的国度，你提前一年计划了一些火车旅行。在接下来的一年里，你要旅行的日子将以一个名为 `days` 的数组给出。每一项是一个从 `1` 到 `365` 的整数。

火车票有 **三种不同的销售方式** ：
- 一张 **为期一天** 的通行证售价为 `costs[0]` 美元；
- 一张 **为期七天** 的通行证售价为 `costs[1]` 美元；
- 一张 **为期三十天** 的通行证售价为 `costs[2]` 美元。

通行证允许数天无限制的旅行。 例如，如果我们在第 `2` 天获得一张 **为期 7 天** 的通行证，那么我们可以连着旅行 7 天：第 `2` 天、第 `3` 天、第 `4` 天、第 `5` 天、第 `6` 天、第 `7` 天和第 `8` 天。

返回 _你想要完成在给定的列表 `days` 中列出的每一天的旅行所需要的最低消费_ 。

**示例 1：**
```js
输入：days = [1,4,6,7,8,20], costs = [2,7,15]
输出：11
解释： 
例如，这里有一种购买通行证的方法，可以让你完成你的旅行计划：
在第 1 天，你花了 costs[0] = $2 买了一张为期 1 天的通行证，它将在第 1 天生效。
在第 3 天，你花了 costs[1] = $7 买了一张为期 7 天的通行证，它将在第 3, 4, ..., 9 天生效。
在第 20 天，你花了 costs[0] = $2 买了一张为期 1 天的通行证，它将在第 20 天生效。
你总共花了 $11，并完成了你计划的每一天旅行。
```
**示例 2：**
```js
输入：days = [1,2,3,4,5,6,7,8,9,10,30,31], costs = [2,7,15]
输出：17
解释：
例如，这里有一种购买通行证的方法，可以让你完成你的旅行计划： 
在第 1 天，你花了 costs[2] = $15 买了一张为期 30 天的通行证，它将在第 1, 2, ..., 30 天生效。
在第 31 天，你花了 costs[0] = $2 买了一张为期 1 天的通行证，它将在第 31 天生效。 
你总共花了 $17，并完成了你计划的每一天旅行。
```
**提示：**
- `1 <= days.length <= 365`
- `1 <= days[i] <= 365`
- `days` 按顺序严格递增
- `costs.length == 3`
- `1 <= costs[i] <= 1000`

---
### 解法 动态规划
方法一：记忆化搜索（日期变量型）
思路和算法

我们用 dp(i)\textit{dp}(i)dp(i) 来表示从第 iii 天开始到一年的结束，我们需要花的钱。考虑到一张通行证可以让我们在「接下来」的若干天进行旅行，所以我们「从后往前」倒着进行动态规划。

对于一年中的任意一天：

如果这一天不是必须出行的日期，那我们可以贪心地选择不买。这是因为如果今天不用出行，那么也不必购买通行证，并且通行证越晚买越好。所以有 dp(i)=dp(i+1)\textit{dp}(i) = \textit{dp}(i + 1)dp(i)=dp(i+1)；

如果这一天是必须出行的日期，我们可以选择买 111，777 或 303030 天的通行证。若我们购买了 jjj 天的通行证，那么接下来的 j−1j - 1j−1 天，我们都不再需要购买通行证，只需要考虑第 i+ji + ji+j 天及以后即可。因此，我们有

dp(i)=min⁡{cost(j)+dp(i+j)},j∈{1,7,30}\textit{dp}(i) = \min\{\textit{cost}(j) + \textit{dp}(i + j)\}, \quad j \in \{1, 7, 30\}
dp(i)=min{cost(j)+dp(i+j)},j∈{1,7,30}
其中 cost(j)\textit{cost}(j)cost(j) 表示 jjj 天通行证的价格。为什么我们只需要考虑第 i+ji+ji+j 天及以后呢？这里和第一条的贪心思路是一样的，如果我们需要购买通行证，那么一定越晚买越好，在握着一张有效的通行证的时候购买其它的通行证显然是不划算的。

由于我们是倒着进行动态规划的，因此我们可以使用记忆化搜索，减少代码的编写难度。我们使用一个长度为 366366366 的数组（因为天数是 [1,365][1, 365][1,365]，而数组的下标是从 000 开始的）存储所有的动态规划结果，这样所有的 dp(i)\textit{dp}(i)dp(i) 只会被计算一次（和普通的动态规划相同），时间复杂度不会增大。

最终的答案记为 dp(1)\textit{dp}(1)dp(1)。

Java
Python3
C++
Golang
class Solution {
    unordered_set<int> dayset;
    vector<int> costs;
    int memo[366] = {0};

public:
    int mincostTickets(vector<int>& days, vector<int>& costs) {
        this->costs = costs;
        for (int d: days) {
            dayset.insert(d);
        }
        memset(memo, -1, sizeof(memo));
        return dp(1);
    }

    int dp(int i) {
        if (i > 365) {
            return 0;
        }
        if (memo[i] != -1) {
            return memo[i];
        }
        if (dayset.count(i)) {
            memo[i] = min(min(dp(i + 1) + costs[0], dp(i + 7) + costs[1]), dp(i + 30) + costs[2]);
        } else {
            memo[i] = dp(i + 1);
        }
        return memo[i];
    }
};
复杂度分析

时间复杂度：O(W)O(W)O(W)，其中 W=365W = 365W=365 是旅行计划中日期的最大值，我们需要计算 WWW 个解，而每个解最多需要查询 333 个其他的解，因此计算量为 O(3∗W)=O(W)O(3 * W)=O(W)O(3∗W)=O(W)。

空间复杂度：O(W)O(W)O(W)，我们需要长度为 O(W)O(W)O(W) 的数组来存储所有的解。

方法二：记忆化搜索（窗口变量型）
思路

方法一需要遍历一年中所有的天数，无论 days\textit{days}days 的长度是多少。

但是观察方法一的递推式，我们可以看到，如果我们查询 dp(i)\textit{dp}(i)dp(i)，而第 iii 天我们又不需要出行的话，那么 dp\textit{dp}dp 函数会一直向后计算 dp(i+1)=dp(i+2)=dp(i+3)\textit{dp}(i + 1) = \textit{dp}(i + 2) = \textit{dp}(i + 3)dp(i+1)=dp(i+2)=dp(i+3) 一直到一年结束或者有一天我们需要出行为止。那么我们其实可以直接跳过这些不需要出行的日期，直接找到下一个需要出行的日期。

算法

现在，我们令 dp(i)\textit{dp}(i)dp(i) 表示能够完成从第 days[i]\textit{days}[i]days[i] 天到最后的旅行计划的最小花费（注意，不再是第 iii 天到最后的最小花费）。令 j1j_1j 
1
​
  是满足 days[j1]>=days[i]+1\textit{days}[j_1] >= \textit{days}[i] + 1days[j 
1
​
 ]>=days[i]+1 的最小下标，j7j_7j 
7
​
  是满足 days[j7]>=days[i]+7\textit{days}[j_7] >= \textit{days}[i] + 7days[j 
7
​
 ]>=days[i]+7 的最小下标， j30j_{30}j 
30
​
  是满足 days[j30]>=days[i]+30\textit{days}[j_{30}] >= \textit{days}[i] + 30days[j 
30
​
 ]>=days[i]+30 的最小下标，那么就有：

dp(i)=min⁡(dp(j1)+costs[0],dp(j7)+costs[1],dp(j30)+costs[2])\textit{dp}(i) = \min(\textit{dp}(j_1) + \textit{costs}[0], \textit{dp}(j_7) + \textit{costs}[1], \textit{dp}(j_{30}) + \textit{costs}[2])
dp(i)=min(dp(j 
1
​
 )+costs[0],dp(j 
7
​
 )+costs[1],dp(j 
30
​
 )+costs[2])
Java
Python3
C++
Golang
class Solution {
private:
    vector<int> days, costs;
    vector<int> memo;
    int durations[3] = {1, 7, 30};
    
public:
    int mincostTickets(vector<int>& days, vector<int>& costs) {
        this->days = days;
        this->costs = costs;
        memo.assign(days.size(), -1);
        return dp(0);
    }

    int dp(int i) {
        if (i >= days.size()) {
            return 0;
        }
        if (memo[i] != -1) {
            return memo[i];
        }
        memo[i] = INT_MAX;
        int j = i;
        for (int k = 0; k < 3; ++k) {
            while (j < days.size() && days[j] < days[i] + durations[k]) {
                ++j;
            }
            memo[i] = min(memo[i], dp(j) + costs[k]);
        }
        return memo[i];
    }
};
复杂度分析

时间复杂度：O(N)O(N)O(N)，其中 NNN 是出行日期的数量，我们需要计算 NNN 个解，而计算每个解的过程中最多将指针挪动 303030 步，计算量为 O(30∗N)=O(N)O(30 * N)=O(N)O(30∗N)=O(N)。

空间复杂度：O(N)O(N)O(N)，我们需要长度为 O(N)O(N)O(N) 的数组来存储所有的解。

作者：力扣官方题解
链接：https://leetcode.cn/problems/minimum-cost-for-tickets/description/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
                                               对于本题不难想到应该用一个数组存储到当前某一天需要花费的最少费用，这里为了下标和天数对应，dp数组的长度选择 days 中最后一个天数多加 1 个长度，因为开始没有费用，所以初始化为 0 ，之后开始对 dp 数组进行更新，那么每到达一个位置首先考虑当前天数是否在days 中，如果不在那花费的费用肯定和它前一天花费的最少费用相同(这里用一个 idx 指标指示应该处理哪一个天数，这样就不必用 if i in days 这样的语句判断天数是否需要处理了，可以让程序快一些)，如果在的话，我们就要从三种购买方式中选择一种花费费用最少的，即你想到达第 i 天，你需要从 i 的前1或7或30天的后一位置花费对应cost[0]、cost[1]、cost[2]的钱才能到第 i 天。 具体细节见代码。

作者：莲子熊猫
链接：https://leetcode.cn/problems/minimum-cost-for-tickets/description/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```py
class Solution:
    def mincostTickets(self, days: List[int], costs: List[int]) -> int:
        dp = [0 for _ in range(days[-1] + 1)]  # dp数组，每个元素代表到当前天数最少钱数，为下标方便对应，多加一个 0 位置
        days_idx = 0  # 设定一个days指标，标记应该处理 days 数组中哪一个元素
        for i in range(1, len(dp)):
            if i != days[days_idx]:  # 若当前天数不是待处理天数，则其花费费用和前一天相同
                dp[i] = dp[i - 1]
            else:
                # 若 i 走到了待处理天数，则从三种方式中选一个最小的
                dp[i] = min(dp[max(0, i - 1)] + costs[0],
                            dp[max(0, i - 7)] + costs[1],
                            dp[max(0, i - 30)] + costs[2])
                days_idx += 1
        return dp[-1]  # 返回最后一天对应的费用即可
```