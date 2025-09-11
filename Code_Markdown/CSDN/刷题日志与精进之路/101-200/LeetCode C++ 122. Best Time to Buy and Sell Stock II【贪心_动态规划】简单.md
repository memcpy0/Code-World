Say you have an array `prices` for which the i^th^ element is the price of a given stock on day *i*.

Design an algorithm to find the maximum profit. You may complete as many transactions as you like (i.e., buy one and sell one share of the stock multiple times).

**Note:** You may not engage in multiple transactions at the same time (i.e., you must sell the stock before you buy again).

**Example 1:**
```cpp
Input: [7,1,5,3,6,4]
Output: 7
Explanation: Buy on day 2 (price = 1) and sell on day 3 (price = 5), profit = 5-1 = 4.
             Then buy on day 4 (price = 3) and sell on day 5 (price = 6), profit = 6-3 = 3.
```

**Example 2:**
```cpp
Input: [1,2,3,4,5]
Output: 4
Explanation: Buy on day 1 (price = 1) and sell on day 5 (price = 5), profit = 5-1 = 4.
             Note that you cannot buy on day 1, buy on day 2 and sell them later, as you are
             engaging multiple transactions at the same time. You must sell before buying again.
```

**Example 3:**
```cpp
Input: [7,6,4,3,1]
Output: 0
Explanation: In this case, no transaction is done, i.e. max profit = 0.
```
**Constraints:**
-  `1 <= prices.length <= 3 * 10 ^ 4`
-  `0 <= prices[i] <= 10 ^ 4`


题意：允许多次买卖，求出最后可获得的最大利润。

---
### 解法1 暴力搜索
每次如果**持有，就可以卖出或者继续持有**；如果**未持有，就可以买入或者继续未持有**。枚举每种可能，算法正确性不容置疑。但是时间复杂度高达 $O(2^n)$ 。通不过LeetCode。具体代码如下：
```cpp
class Solution {
public: 
	vector<int> profit;
    int maxProfit(vector<int>& prices) { 
    	solve(0, false, 0, prices);
    	int profitMax = 0;
    	for (const int &i : profit)
    		profitMax = max(profitMax, i);
    	return profitMax;
    } 
    //have : false 未持有, true 持有
	void solve(int i, bool have, int profitSum, vector<int>& prices) {
		if (i >= prices.size())
			profit.push_back(profitSum);
		else {
			if (have) { //持有
				solve(i + 1, false, profitSum + prices[i]); //卖出
				solve(i + 1, true, profitSum);   			//继续持有
			} else {    //未持有
				solve(i + 1, false, profitSum);  			//继续未持有
				solve(i + 1, true, profitSum - prices[i]);  //买入
			}
		}
	}
};
```
---
### 解法2 贪心
容易知道：股票价格是波动的。如果每次都在价格的波谷买入，在下一个价格的波峰卖出，就能够将每次得到的利润最大化。这样，总的利润也是最大的。

可以用类似滑窗的方法找到下个连续上升的价格序列的第一个值，然后找到其最后一个值，求出差值，然后重复这样的操作。不过写起来麻烦。下面的写法：**如果今天的价格高于昨天的价格，就在昨天买入，今天卖出**。相当于将**一个波峰的上升沿**拆解成**许多个以天为单位的上升期**。这样第一天买入，第二天卖出。然后第二天买入，第三天卖出……
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int maxValue = 0;
        for (int i = 1; i < prices.size(); ++i) 
            if (prices[i] > prices[i - 1])
                maxValue += (prices[i] - prices[i - 1]); 
        return maxValue;
    }
};
```
时间效率为 $\text{O(n)}$ ：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7.2 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
对应的Java代码如下：
```cpp
class Solution {
    public int maxProfit(int[] prices) {
        if (prices == null || prices.length <= 1) return 0;
        int maxprofit = 0;
        for (int i = 1; i < prices.length; ++i)  
            if (prices[i] > prices[i - 1]) 
                maxprofit += prices[i] - prices[i - 1]; 
        return maxprofit;
    }
}
```
一次提交的执行效率如下：
```cpp
执行用时：1 ms, 在所有 Java 提交中击败了99.54% 的用户
内存消耗：38.1 MB, 在所有 Java 提交中击败了95.11% 的用户
```

---
### 解法3 动态规划
为区分**持有**或**不持有**股票，需要用二维数组：
- 当天不持有股票的状态，可能来自**昨天持有而今天卖出或者昨天也不持有**；
- 当天持有股票的状态，可能来自**昨天未持有而今天买入或者昨天也持有中**。
- 问题的结果是**最后一天的未持有股票状态下的最大利润**。

说到底，这不过是暴力搜索过程的记忆化。具体代码如下：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        int size = prices.size();
        //dp[i][0]为第i天结束时不持有股票的最大利润,dp[i][1]为持有股票的最大利润
        vector<vector<int>> dp(size, vector<int>(2, 0));
        dp[0][0] = 0, dp[0][1] = -prices[0];
        for (int i = 1; i < size; ++i) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][1] + prices[i]); 
            dp[i][1] = max(dp[i - 1][1], dp[i - 1][0] - prices[i]);
        }
        return dp[size - 1][0]; //返回最后一天未持有股票时的最大利润 
    }
};
```
效率是 $\text{O(n)}$ 的时间，$\text{O(2n)}$ 的空间：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了6.23% 的用户
内存消耗：9.6 MB, 在所有 C++ 提交中击败了7.27% 的用户
```
**20201108 Update** 事实上，只需要4个变量就可以完成整个推导过程：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        int size = prices.size();
        int dp00 = 0, dp10 = -prices[0], dp01, dp11; //00,01为dp0的替换; 10,11为dp1的替换
        for (int i = 1; i < size; ++i) {
            dp01 = max(dp00, dp10 + prices[i]); //现在不持有=max(以前不持有,以前持有+现在卖出)
            dp11 = max(dp10, dp00 - prices[i]); //现在持有=max(以前持有,以前不持有+现在买入)
            dp00 = dp01, dp10 = dp11; 
        }
        return dp00; //返回最后一天未持有股票时的最大利润 
    }
};
```
显而易见，时空效率都有所提升：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了93.79% 的用户
内存消耗：7.5 MB, 在所有 C++ 提交中击败了15.52% 的用户
```

之前的C++代码对应的Java版本如下：
```java
class Solution {
    public int maxProfit(int[] prices) {
        if (prices == null || prices.length <= 1) return 0;
        int[] dp0 = new int[prices.length], dp1 = new int[prices.length];
        dp0[0] = 0; 
        dp1[0] = -prices[0]; //dp1[i]为当天不持有股票时的最大利润,dp2[i]为当天持有股票时的最大利润
        for (int i = 1; i < prices.length; ++i) {
            dp0[i] = Math.max(dp0[i - 1], dp1[i - 1] + prices[i]);
            dp1[i] = Math.max(dp0[i - 1] - prices[i], dp1[i - 1]); 
        }
        return dp0[dp0.length - 1]; //返回最后一天未持有股票时的最大利润
    }
}
```
提交后的结果如下：
```cpp
执行用时：3 ms, 在所有 Java 提交中击败了14.80% 的用户
内存消耗：38.3 MB, 在所有 Java 提交中击败了90.10% 的用户
```
