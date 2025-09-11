Your are given an array of integers `prices`, for which the `i-th` element is the price of a given stock on day `i`; and a **non-negative** integer fee representing a transaction fee.

You may complete as many transactions as you like, but you need to pay the transaction fee for each transaction. You may not buy more than `1` share of a stock at a time (ie. you must sell the stock share before you buy again.)

Return the **maximum** profit you can make.

**Example 1:**
```css
Input: prices = [1, 3, 2, 8, 4, 9], fee = 2
Output: 8
Explanation: The maximum profit can be achieved by:
Buying at prices[0] = 1
Selling at prices[3] = 8
Buying at prices[4] = 4
Selling at prices[5] = 9
The total profit is ((8 - 1) - 2) + ((9 - 4) - 2) = 8.
```

**Note:**
- `0 < prices.length <= 50000`.
- `0 < prices[i] < 50000`.
- `0 <= fee < 50000`.
 
题意：买卖股票，可以进行任意次交易，但是不能同时进行多次交易，必须先卖出已有的股票再买新的股票。每次买卖会收取一定的手续费。求出扣除全部买卖缴纳的手续费后，能够获得的最大利润。

---
### 解法 动态规划
做过的动态规划的题目，只是稍微变了一下。最后，如果买卖得到的利润扣除手续费后小于零，则不进行买卖。具体代码如下：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        if (prices.size() <= 1) return 0;
        int n = prices.size();
        vector<vector<int>> dp(2, vector<int>(n));
        dp[0][0] = 0, dp[1][0] = -prices[0];
        for (int i = 1; i < n; ++i) {
            dp[0][i] = max(dp[0][i - 1], dp[1][i - 1] + prices[i] - fee);
            dp[1][i] = max(dp[1][i - 1], dp[0][i - 1] - prices[i]);
        }
        return dp[0][n - 1] > 0 ? dp[0][n - 1] : 0;
    }
};
```
执行效率如下：
```cpp
执行用时：236 ms, 在所有 C++ 提交中击败了65.90% 的用户
内存消耗：59 MB, 在所有 C++ 提交中击败了16.67% 的用户
```

上面的代码空间效率不够。通过观察可以发现：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200711004952466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
代码中，我们真正需要的就只有两个变量——蓝色和红色方框框出的变量，而不需要开辟一个二维数组。我们返回的结果，实际上是最后得到的红色变量。当然，在循环中，我们需要先保存之前的红色变量，然后对其进行更改，接着更改蓝色变量，如此保证结果的准确性。最终代码如下：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        if (prices.size() <= 1) return 0;
        int n = prices.size(), noHold = 0, hold = -prices[0];
        for (int i = 1; i < n; ++i) {
            int temp = noHold;
            noHold = max(noHold, hold + prices[i] - fee);
            hold = max(hold, temp - prices[i]);
        }
        return noHold > 0 ? noHold : 0;
    }
};
```
相对而言，上面的代码提高了一定的时间效率（不需要二维寻址，直接存取变量），大幅提高了空间效率。

---
**20201217 Update** 今日打卡更新代码：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        //hold[i]    表示当前持有股票的最大利润
        //nohold[i]   表示当前不持有股票的最大利润
        if (prices.empty()) return 0;
        int n = prices.size();
        vector<int> hold(n), nohold(n);
        hold[0] = -prices[0] - fee, nohold[0] = 0;
        for (int i = 1; i < n; ++i) {
            hold[i] = max(hold[i - 1], nohold[i - 1] - prices[i] - fee); //买入时支付手续费和股价
            nohold[i] = max(nohold[i - 1], hold[i - 1] + prices[i]); //卖出时得到当前股价的价值
        }
        return nohold[n - 1]; //最后不持有股票得到的利润更大
    }
};
```
运行效率如下：
```cpp
执行用时：196 ms, 在所有 C++ 提交中击败了91.69% 的用户
内存消耗：55.5 MB, 在所有 C++ 提交中击败了27.18% 的用户
```
