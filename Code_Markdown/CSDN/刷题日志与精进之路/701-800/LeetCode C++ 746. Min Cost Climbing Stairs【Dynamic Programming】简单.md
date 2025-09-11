On a staircase, the `i-th` step has some non-negative cost `cost[i]` assigned (`0` indexed).

Once you pay the cost, you can either climb one or two steps. You need to find minimum cost to reach the top of the floor, and you can either start from the step with index 0, or the step with index `1` .

**Example 1:**

```swift
Input: cost = [10, 15, 20]
Output: 15
Explanation: Cheapest is start on cost[1], pay that cost and go to the top.
```

**Example 2:**

```swift
Input: cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1]
Output: 6
Explanation: Cheapest is start on cost[0], and only step on 1s, skipping cost[3].
```

**Note:**
- cost will have a length in the range `[2, 1000]` .
- Every cost[i] will be an integer in the range `[0, 999]` .

题意：在楼梯上，第 `i` 级台阶有一个非负的代价 `cost[i]` ，一旦支付这一代价，就可以爬到第 `i + 1` 阶或者第 `i + 2` 阶，也就是爬到上一级或者上两级台阶。题意要求我们找到到达楼层顶部的最小成本，可以从第 `0` 阶开始，也可以从第 `1` 阶开始。

---
### 解法 动态规划
这道题的中文翻译烂透了，完全搞不懂题意。动态规划的思路很简单，设置一个 `dp[]` 数组，表示到达第 `i` 阶台阶需要多少代价，楼层顶部为 `n` 。
- `dp[0] = dp[1] = 0` ，因为可以直接到达；
- 然后站在第 `i` 级台阶上，如果从此出发支付代价 `cost[i]` ，能更轻松地到达上两级，就更新 `dp[i + 1], dp[i + 2]` 的最小花费。最后返回 `dp[n]` 。

具体代码如下：
```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        vector<int> dp(cost.size() + 2, INT_MAX);
        dp[0] = dp[1] = 0;
        int n = cost.size();
        for (int i = 0; i < n; ++i) {
            dp[i + 1] = min(cost[i] + dp[i], dp[i + 1]);
            dp[i + 2] = min(cost[i] + dp[i], dp[i + 2]);
        } 
        return dp[n];
    }
};
```
运行结果如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了64.08% 的用户
内存消耗：13.2 MB, 在所有 C++ 提交中击败了49.65% 的用户
```
或者写成下面这样，第 `i` 级台阶可以从第 `i - 1` 级支付 `cost[i - 1]` 或者从第 `i - 2` 级支付 `cost[i - 2]` 到达，选择其中较小的那个即可：
```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = dp[1] = 0;
        for (int i = 2; i <= n; ++i) 
            dp[i] = min(dp[i - 1] + cost[i - 1], dp[i - 2] + cost[i - 2]);
        return dp[n];
    }
}; 
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了93.98% 的用户
内存消耗：13.3 MB, 在所有 C++ 提交中击败了34.19% 的用户
```
----
**20201221 Update** 今日打卡更新，$O(n)$ 时空的代码如下：
```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size();
        vector<int> dp(n + 1); //dp[i]是到达第i阶楼梯时需要支付的最少消耗
        dp[0] = dp[1] = 0; //相当于最前面有一个0,支付0消耗可以到达第0,1阶
        for (int i = 2; i <= n; ++i) 
            dp[i] = min(dp[i - 2] + cost[i - 2], dp[i - 1] + cost[i - 1]);            
        return dp[n];
    }
};
```
$O(1)$ 空间的代码如下：
```cpp
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size(), dp0 = 0, dp1 = 0, dp2; //相当于最前面有一个0,支付0消耗可以到达第0,1阶
        for (int i = 2; i <= n; ++i) {
            dp2 = min(dp0 + cost[i - 2], dp1 + cost[i - 1]);
            dp0 = dp1;
            dp1 = dp2;
        }            
        return dp1;
    }
};
```
运行结果如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了92.51% 的用户
内存消耗：13.4 MB, 在所有 C++ 提交中击败了53.07% 的用户
```
