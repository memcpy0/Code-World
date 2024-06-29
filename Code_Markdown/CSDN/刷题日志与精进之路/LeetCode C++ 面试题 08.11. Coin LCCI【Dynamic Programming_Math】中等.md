Given an infinite number of quarters (`25` cents), dimes (`10` cents), nickels (`5` cents), and pennies (`1` cent), write code to calculate the number of ways of representing `n` cents. (The result may be large, so you should return it modulo `1000000007`)

**Example1:**

```swift
Input: n = 5
Output: 2
Explanation: There are two ways:
5=5
5=1+1+1+1+1
```
**Example2:**
```swift
Input: n = 10
Output: 4
Explanation: There are four ways:
10=10
10=5+5
10=5+1+1+1+1+1
10=1+1+1+1+1+1+1+1+1+1
```

**Notes:** You can assume: `0 <= n <= 1000000`

题意：给定数量不限的硬币，币值为25分、10分、5分和1分，编写代码计算n分有几种表示法。(结果可能会很大，需要将结果模上1000000007)

---
### 解法 动态规划
完全背包的恰好问题：
```cpp 
// dp[i][j] 使用前i种硬币计算j分的表示法种数 令coins=[25, 10, 5, 1]
// dp[i][j] = dp[i-1][j] + dp[i-1][j-coins[i]] + dp[i-1][j-2*coins[i]] + ... dp[i-1][j-k*coins[i]]
// j >= k*coins[i]
// dp[i][j-coins[i]] = dp[i-1][j-coins[i]] + dp[i-1][j-2*coins[i]] + ... dp[i-1][j-k*coins[i]]
// dp[i][j] - dp[i][j-coins[i]] = dp[i-1][j]
// dp[i][j] = dp[i][j-coins[i]] + dp[i-1][j]
const int maxn = 1e6 + 10, mod = 1000000007;
int w[4] = {1, 5, 10, 25};
class Solution {
public:
    int waysToChange(int n) {
        vector<int> dp(n + 1); //注意dp的初始化,dp[i]表示凑出i分的方式数量
        dp[0] = 1; //dp[0] = 1表示没有硬币凑0分,有1种方式
        for (int i = 0; i < 4; ++i)
            for (int j = w[i]; j <= n; ++j)
                dp[j] = (dp[j] + dp[j - w[i]]) % mod;
        return dp.back() % mod; //dp[n]
    }
};
```
运行效率如下：
```cpp
执行用时：88 ms, 在所有 C++ 提交中击败了51.90% 的用户
内存消耗：14.5 MB, 在所有 C++ 提交中击败了38.88% 的用户
```
