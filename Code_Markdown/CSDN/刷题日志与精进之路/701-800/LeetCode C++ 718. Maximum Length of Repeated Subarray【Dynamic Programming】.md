Given two integer arrays `A` and `B` , return the **maximum length** of an subarray that appears in both arrays.

Example 1:
```cpp
Input:
A: [1,2,3,2,1]
B: [3,2,1,4,7]
Output: 3
Explanation: 
The repeated subarray with maximum length is [3, 2, 1].
```
Note:
-    `1 <= len(A), len(B) <= 1000`
-    `0 <= A[i], B[i] < 100`

题意：求出最长的公共子数组。

思路：动态规划，$\text{O(nm)}$ 。

代码：
```cpp
class Solution {
public:
    int findLength(vector<int>& A, vector<int>& B) {
        if (A.empty() || B.empty()) return 0;
        vector<vector<int>> dp(A.size() + 1, vector<int>(B.size() + 1, 0));
        int maxLen = 0;
        for (int i = 0; i < A.size(); ++i) {
            for (int j = 0; j < B.size(); ++j) { 
                if (A[i] == B[j]) dp[i + 1][j + 1] = dp[i][j] + 1;
                else dp[i + 1][j + 1] = 0; 
                maxLen = max(dp[i + 1][j + 1], maxLen);
            }
        } 
        return maxLen;
    }
};
```
效率：
```
执行用时：476 ms, 在所有 C++ 提交中击败了22.14% 的用户
内存消耗：109 MB, 在所有 C++ 提交中击败了33.33% 的用户
```
