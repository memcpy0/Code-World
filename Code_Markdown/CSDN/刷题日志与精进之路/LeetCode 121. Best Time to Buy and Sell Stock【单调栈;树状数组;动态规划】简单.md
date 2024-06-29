> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Say you have an array for which the i^th^ element is the price of a given stock on day *i* .

If you were only permitted to complete at most one transaction (i.e., buy one and sell one share of the stock), design an algorithm to find the maximum profit.

Note that you cannot sell a stock before you buy one.

Example 1:
```cpp
Input: [7,1,5,3,6,4]
Output: 5
Explanation: Buy on day 2 (price = 1) and sell on day 5 (price = 6), profit = 6-1 = 5.
             Not 7-1 = 6, as selling price needs to be larger than buying price.
```
Example 2:
```cpp
Input: [7,6,4,3,1]
Output: 0
Explanation: In this case, no transaction is done, i.e. max profit = 0.
```

题意：最多可以交易一次，求买卖股票得到的最大利润是多少。

---
股票题目有六道：
- [121. 买卖股票的最佳时机](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock)：最多一次交易，每次一股股票，不能同时参与多笔交易
- [122. 买卖股票的最佳时机 II](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-ii/)：多次交易，每次一股股票，不能同时参与多笔交易
- [123. 买卖股票的最佳时机 III](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii/)：最多两笔交易，每次一股股票，不能同时参与多笔交易
- [188. 买卖股票的最佳时机 IV](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iv/)：最多可以完成 `k` 笔交易，每次一股股票，不能同时参与多笔交易
- [309. 最佳买卖股票时机含冷冻期](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-cooldown/)：多次交易，有1天冷冻期，每次一股股票，不能同时参与多笔交易
- [714. 买卖股票的最佳时机含手续费](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/)：多次交易，每次交易要付一次手续费，每次一股股票，不能同时参与多笔交易

### 解法1 暴力
暴力求解每对下标的差值，找到最大值。结果超出时间限制。具体代码如下：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int maxSum = 0;
        for (int i = 0; i < prices.size(); ++i) 
            for (int j = i + 1; j < prices.size(); ++j) 
                if (prices[j] - prices[i] > maxSum)
                    maxSum = prices[j] - prices[i];
        return maxSum; 
    }
};
```
复杂度分析：
- 时间复杂度：$O(n^2)$
- 空间复杂度：$O(1)$

---
### 解法2 树状数组
从解法1可以发现，我们从前往后枚举 $i$ ，然后嵌套遍历 $i$ 之后的 $prices[j]$ 。**我们可以利用树状数组消除掉内嵌的循环，直接求出 $prices[i + 1, n)$ 的最大值**。
> 注意，树状数组下标一般是从 $1$ 开始的。

int mmin[30005];

int lowbit(int x) {return x & -x;}

void add(int p, int v) {for (int i = p; i <= 30000; i += lowbit(i)) mmin[i] = min(mmin[i], v);}

int get(int p) {
    int ret = 0x7f7f7f7f;
    for (int i = p; i >= 1; i -= lowbit(i)) ret = min(ret, mmin[i]);
    return ret;
}

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        
        memset(mmin, 0x7f, sizeof(mmin));
        for (int i = 0; i < n; i++) add(i + 1, prices[i]);
        
        int ans = 0;
        for (int i = n - 1; i >= 0; i--) ans = max(ans, prices[i] - get(i));
        return ans;
    }
};
时间复杂度：O(nlogn)O(nlogn)O(nlogn)。 空间复杂度：O(n)O(n)O(n)。有一个长度为n的mminmminmmin数组以及常数个变量。


---
### 解法2 动态规划
在上面的求解过程中，存在许多重复的子问题。如：
```
Input : [7,1,5,3,6,4]	  下面都是差值：
maxProfit[0] = 0, p[i] = prices[i]
第一次：  p[1] - p[0]   = maxProfit[1] 
第二次：  max(p[2] - p[1], p[2] - p[0], maxProfit[1]) = maxProfit[2]
第三次：  max(p[3] - p[2], p[3] - p[1], p[3] - p[0], maxProfit[2]) = maxProfit[3]
...
```
每次求解中，都要分别**以当前的价格和前面的所有价格相减**(太慢！)，求出各差值，然后和**之前得到的最大利润**比较，从而得到当前的最大利润。

思考一下，就会发现，和前面的所有价格相减没有必要，**只需要和前部分中的最小价格相减，就一定可以得到最大差值**。然后和之前的最大利润比较即可。于是有了下面的代码：
```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) { 
        if (prices.empty()) return 0;  
        //dp[i]为前i天中最多一次交易可得的最大收益
        //前i天的最大收益 = max{前i-1天的最大收益，第i天的股价-前i-1天中的最小股价}
        //dp[i] = max(dp[i - 1], prices[i] - minPrice)
        int mx = 0, mi = prices[0]; //dp[0]=0, 最小的股价=prices[0]
        for (int i = 1; i < prices.size(); ++i) { 
            mx = max(mx, prices[i] - mi); //dp[i]
            mi = min(mi, prices[i]);
        }
        return mx;
    }
};
```
对应的Java版本如下：
```cpp
class Solution {
    public int maxProfit(int[] prices) {
        if (prices == null || prices.length <= 1) return 0;
        int maxprofit = 0, minprice = prices[0];
        //dp[i]是第i天得到的最高利润,只能交易一次,因此此时的最高利润为max(maxProfit, 此时的价格-之前的最低价格)
        for (int i = 1; i < prices.length; ++i) { 
            maxprofit = Math.max(maxprofit, prices[i] - minprice);
            minprice = Math.min(minprice, prices[i]);
        }
        return maxprofit;
    }
}
```

 