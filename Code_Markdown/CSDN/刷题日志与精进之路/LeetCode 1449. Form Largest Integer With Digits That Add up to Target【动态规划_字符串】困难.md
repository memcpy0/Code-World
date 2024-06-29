<p>Given an array of integers <code>cost</code> and an integer <code>target</code>. Return the <strong>maximum</strong> integer you can paint&nbsp;under the following rules:</p>

<ul>
	<li>The cost of painting a&nbsp;digit (i+1) is given by&nbsp;<code>cost[i]</code>&nbsp;(0 indexed).</li>
	<li>The total cost used must&nbsp;be equal to <code>target</code>.</li>
	<li>Integer does not have digits 0.</li>
</ul>

<p>Since the answer may be too large, return it as string.</p>

<p>If there is no way to paint any integer given the condition, return "0".</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: cost = [4,3,2,5,6,7,2,5,5], target = 9
Output: "7772"
Explanation:  The cost to paint the digit '7' is 2, and the digit '2' is 3. Then cost("7772") = 2*3+ 3*1 = 9. You could also paint "977", but "7772" is the largest number.
Digit    cost
  1  ->   4
  2  ->   3
  3  ->   2
  4  ->   5
  5  ->   6
  6  ->   7
  7  ->   2
  8  ->   5
  9  ->   5
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: cost = [7,6,5,5,5,6,8,7,8], target = 12
Output: "85"
Explanation: The cost to paint the digit '8' is 7, and the digit '5' is 5. Then cost("85") = 7 + 5 = 12.
```
  
<p><strong>Example 3:</strong></p>

```cpp
Input: cost = [2,4,6,2,4,6,4,4,4], target = 5
Output: "0"
Explanation: It's not possible to paint any integer with total cost equal to target.
```
<p><strong>Example 4:</strong></p>

```cpp
Input: cost = [6,10,15,40,40,40,40,40,40], target = 47
Output: "32211"
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>cost.length == 9</code></li>
	<li><code>1 &lt;= cost[i] &lt;= 5000</code></li>
	<li><code>1 &lt;= target &lt;= 5000</code></li>
</ul>

题意：给你一个整数数组&nbsp;<code>cost</code>&nbsp;和一个整数&nbsp;<code>target</code>&nbsp;。请你返回满足如下规则可以得到的&nbsp;<strong>最大</strong>&nbsp;整数：
<ul>
	<li>给当前结果添加一个数位（<code>i + 1</code>）的成本为&nbsp;<code>cost[i]</code>&nbsp;（<code>cost</code>&nbsp;数组下标从 0 开始）。</li>
	<li>总成本必须恰好等于&nbsp;<code>target</code>&nbsp;。</li>
	<li>添加的数位中没有数字 0 。</li>
</ul>

<p>由于答案可能会很大，请你以字符串形式返回。</p>

<p>如果按照上述要求无法得到任何整数，请你返回 "0" 。</p> 


---
### 解法 完全背包
完全背包+恰好问题+输出答案，一开始的写法如下：
```cpp
class Solution {
public:
    string largestNumber(vector<int>& cost, int target) {
        vector<bool> dp(target + 1, false); 
        vector<string> ans(target + 1, "");
        dp[0] = true, ans[0] = "";
        //dp[0]=true, dp[i]表示是否能够满足成本恰好为i,此时得到的最大整数为ans[i] (完全背包+恰好问题)
        //从[物品]9个数位(1-9)选择,背包容量为成本大小,数位i+1的代价为cost[i],价值为对应的数位i+1
        for (int i = 0; i < 9; ++i) {
            for (int j = cost[i]; j <= target; ++j) {
                dp[j] = dp[j] || dp[j - cost[i]];
                if (dp[j - cost[i]]) {
                    string a = ans[j], b = ans[j - cost[i]], c = ""; //在b上添加数位i+1使其最大 
                    for (int k = 0; k < b.size(); ++k) {
                        if (('1' + i) > b[k]) {
                            c = b.substr(0, k) + to_string(i + 1) + b.substr(k);
                            break;
                        }
                    }
                    if (c.empty()) c = b + to_string(i + 1);
                    //比较字符串代表的整数的大小
                    if (a.size() < c.size()) ans[j] = c;
                    else if (a.size() == c.size()) ans[j] = max(a, c);  
                }
            } 
        }
        return dp[target] ? ans[target] : "0";
    }
};
```
运行效率很低：
```cpp
执行用时：684 ms, 在所有 C++ 提交中击败了5.28% 的用户
内存消耗：460.1 MB, 在所有 C++ 提交中击败了13.03% 的用户
```
后来发现，遍历的顺序由小到大，当前的数位一定是最大的，可以直接放到最高位。它一定大于等于整数的低位，从而保证了整数最大化，因此可以减少中间的 `for` 循环：
```cpp
class Solution {
public:
    string largestNumber(vector<int>& cost, int target) {
        vector<bool> dp(target + 1, false); 
        vector<string> ans(target + 1, "");
        dp[0] = true, ans[0] = ""; 
        for (int i = 0; i < 9; ++i) {
            for (int j = cost[i]; j <= target; ++j) {
                dp[j] = dp[j] || dp[j - cost[i]];
                if (dp[j - cost[i]]) {
                    string a = ans[j], &&b = to_string(i + 1) + ans[j - cost[i]]; //在b上添加数位i+1使其最大  
                    if (a.size() < b.size()) ans[j] = b;
                    else if (a.size() == b.size()) ans[j] = max(a, b);  
                }
            } 
        }
        return dp[target] ? ans[target] : "0";
    }
};
```
运行效率大大提升：
```cpp
执行用时：292 ms, 在所有 C++ 提交中击败了39.44% 的用户
内存消耗：343.8 MB, 在所有 C++ 提交中击败了34.86% 的用户
```
原先的 `dp[i]` 表示其中数位的 `cost` 相加是否等于 `i` ，而 `ans[i]` 用来输出对应的答案。新的优化来自于，我们可以用 `"#"` 表示不合法的情况，即无法得到的目标值，**避免不合法的转移**，同时初始化 `dp[0] = ""` ，这是合法的情况：
```cpp
class Solution {
public:
    string largestNumber(vector<int>& cost, int target) {
        //dp[0]=true, dp[i]表示是否能够满足成本恰好为i,此时得到的最大整数 (完全背包+恰好问题)
        //从[物品]9个数位(1-9)选择,背包容量为成本大小,数位i+1的代价为cost[i],价值为对应的数位i+1
        vector<string> dp(target + 1, "#");  
        dp[0] = "";
        for (int i = 0; i < 9; ++i) {
            for (int j = cost[i]; j <= target; ++j) {
                if (dp[j - cost[i]] != "#") {
                    string a = dp[j], &&b = to_string(i + 1) + dp[j - cost[i]]; //在b上添加数位i+1使其最大 
                    //比较字符串代表的整数的大小
                    if (a.size() < b.size()) dp[j] = b;
                    else if (a.size() == b.size()) dp[j] = max(a, b);  
                }
            } 
        }
        return dp[target] != "#" ? dp[target] : "0";
    }
};
```
运行效率如下：
```cpp
执行用时：256 ms, 在所有 C++ 提交中击败了46.83% 的用户
内存消耗：343.9 MB, 在所有 C++ 提交中击败了34.86% 的用户
```
