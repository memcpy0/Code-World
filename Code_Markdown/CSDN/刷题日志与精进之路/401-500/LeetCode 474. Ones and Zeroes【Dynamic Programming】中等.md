<p>You are given an array of binary strings <code>strs</code> and two integers <code>m</code> and <code>n</code>.</p>

<p>Return <em>the size of the largest subset of <code>strs</code> such that there are <strong>at most</strong> </em><code>m</code><em> </em><code>0</code><em>'s and </em><code>n</code><em> </em><code>1</code><em>'s in the subset</em>.</p>

<p>A set <code>x</code> is a <strong>subset</strong> of a set <code>y</code> if all elements of <code>x</code> are also elements of <code>y</code>.</p>

 
<p><strong>Example 1:</strong></p>
 

```cpp
Input: strs = ["10","0001","111001","1","0"], m = 5, n = 3
Output: 4
Explanation: The largest subset with at most 5 0's and 3 1's is {"10", "0001", "1", "0"}, so the answer is 4.
Other valid but smaller subsets include {"0001", "1"} and {"10", "1", "0"}.
{"111001"} is an invalid subset because it contains 4 1's, greater than the maximum of 3.
```
 
<p><strong>Example 2:</strong></p>

```cpp
Input: strs = ["10","0","1"], m = 1, n = 1
Output: 2
Explanation: The largest subset is {"0", "1"}, so the answer is 2.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= strs.length &lt;= 600</code></li>
	<li><code>1 &lt;= strs[i].length &lt;= 100</code></li>
	<li><code>strs[i]</code> consists only of digits <code>'0'</code> and <code>'1'</code>.</li>
	<li><code>1 &lt;= m, n &lt;= 100</code></li>
</ul>




题意：给出一个二进制字符串数组 `strs` 和两个整数 `m` 和 `n` 。找出并返回 `strs` 的最大子集的大小，该子集中 **最多** 有 `m` 个 `0` 和 `n` 个 `1`（如果 `x` 的所有元素也是 `y` 的元素，集合 `x` 是集合 `y` 的 子集）。

---
### 解法 二维01背包
这道题如果只有一个限制条件 `m` 个 `0` 或者 `n` 个 `1` ，那就很简单了，一个纯粹的01背包。但是有两个条件，因此是多维01背包。具体代码如下：
```cpp
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        vector<pair<int, int>> nums01;
        for (int i = 0; i < len; ++i) {
            int zeros = 0;
            for (const char &c : strs[i]) zeros += (c == '0');
            nums01.push_back({zeros, strs[i].size() - zeros});
        }
        vector<vector<vector<int>>> dp(len + 1, vector<vector<int>>(m + 1, vector<int>(n + 1, 0)));
        //dp[i][j][k]: 遍历到第i个01字符串时,在0数目最多为j、1数目最多为k的情况下,最大的strs子集大小
        for (int i = 1; i <= len; ++i) {
            for (int j = 0; j <= m; ++j) {
                for (int k = 0; k <= n; ++k) {
                    if ((nums01[i - 1].first <= j) && (nums01[i - 1].second <= k)) //最大的0,1数目足够
                        dp[i][j][k] = max(dp[i - 1][j][k], dp[i - 1][j - nums01[i - 1].first][k - nums01[i - 1].second] + 1); //最大strs子集能否+1
                    else dp[i][j][k] = dp[i - 1][j][k]; //0太少或1太少或两者都太少
                }
            }
        }
        return dp[len][m][n];
    }
};
```
运行效率如下：
```cpp
执行用时：704 ms, 在所有 C++ 提交中击败了9.35% 的用户
内存消耗：100.8 MB, 在所有 C++ 提交中击败了5.25% 的用户
```
同样的，这里可以进行**滚动数组空间优化**：
```cpp
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        vector<pair<int, int>> nums01;
        for (int i = 0; i < len; ++i) {
            int zeros = 0;
            for (const char &c : strs[i]) zeros += (c == '0');
            nums01.push_back({zeros, strs[i].size() - zeros});
        }
        vector<vector<vector<int>>> dp(2, vector<vector<int>>(m + 1, vector<int>(n + 1, 0)));
        //dp[i][j][k]: 遍历到第i个01字符串时,在0数目最多为j、1数目最多为k的情况下,最大的strs子集大小
        for (int i = 1; i <= len; ++i) {
            for (int j = 0; j <= m; ++j) {
                for (int k = 0; k <= n; ++k) {
                    if ((nums01[i - 1].first <= j) && (nums01[i - 1].second <= k)) //最大的0,1数目足够
                        dp[i & 1][j][k] = max(dp[!(i & 1)][j][k], dp[!(i & 1)][j - nums01[i - 1].first][k - nums01[i - 1].second] + 1); //最大strs子集能否+1
                    else dp[i & 1][j][k] = dp[!(i & 1)][j][k]; //0太少或1太少或两者都太少
                }
            }
        }
        return dp[len & 1][m][n];
    }
};
```
运行效率如下，时空效率都有所提升，尤其是**空间消耗大大减少**：
```cpp
执行用时：572 ms, 在所有 C++ 提交中击败了16.11% 的用户
内存消耗：12 MB, 在所有 C++ 提交中击败了28.20% 的用户
```
继续优化空间，一部分 `dp[][]` 值的无用赋值次数减少，一定程度上优化了时间：
```cpp
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int len = strs.size();
        vector<pair<int, int>> nums01;
        for (int i = 0; i < len; ++i) {
            int zeros = 0;
            for (const char &c : strs[i]) zeros += (c == '0');
            nums01.push_back({zeros, strs[i].size() - zeros});
        }
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        //dp[i][j][k]: 遍历到第i个01字符串时,在0数目最多为j、1数目最多为k的情况下,最大的strs子集大小
        for (int i = 1; i <= len; ++i) 
            for (int j = m; j >= nums01[i - 1].first; --j) 
                for (int k = n; k >= nums01[i - 1].second; --k) //最大的0,1数目足够
                    dp[j][k] = max(dp[j][k], dp[j - nums01[i - 1].first][k - nums01[i - 1].second] + 1); //最大strs子集能否+1
        return dp[m][n];
    }
};
```
运行效率如下，时空效率进一步提升：
```cpp
执行用时：396 ms, 在所有 C++ 提交中击败了32.40% 的用户
内存消耗：10.1 MB, 在所有 C++ 提交中击败了32.51% 的用户
```
