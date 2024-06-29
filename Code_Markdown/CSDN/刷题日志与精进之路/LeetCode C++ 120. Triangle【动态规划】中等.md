Given a triangle, find the minimum path sum from top to bottom. Each step you may **move to adjacent numbers on the row below**.

For example, given the following triangle
```swift
[
     [2],
    [3,4],
   [6,5,7],
  [4,1,8,3]
]
```
The minimum path sum from top to bottom is `11` (i.e., `2 + 3 + 5 + 1 = 11`).

**Note**: Bonus point if you are able to do this using only O(n) extra space, where n is the total number of rows in the triangle.

题意：给出一个三角形，找到从三角形顶部到底部的最小路径，其中，每一步都可以移动到下一行相邻的(两个)数字上。

---
思路1：这道题目，如果是DFS的话，一旦三角形一大，时间复杂度就会急剧上升。

思路2：贪心。如果每一步都选择可以到达的最小数字，结果是最小路径。可能有这样的想法，但是这里贪心是错误的。如下，从顶部选择 `2->3->6->1` ，合起来是 `12` ，但是最小路径是 `2->4->3->2` 是 `11` 。从底部开始向上选择，也一样是错误的。
```
[
     [2],
    [3,4],
   [6,7,3],
  [4,1,8,2]
]
```
仅仅举出上面的例子，就可以让使用"贪心"的想法破产了。因为，这里的贪心无法累计局部最优达到全局最优，它忽略了太多的信息。

思路3：动态规划。令 `dp[i][j]` 是从坐标 `(i,j)` 向下到达底部的最小路径长度，然后从底部向上递推即可。

代码：
```cpp
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if (triangle.empty() || triangle[0].empty()) return 0;
        int n = triangle.size(); 
        vector<vector<int>> dp(n, vector<int>(triangle[n - 1].size(), 0));
        for (int i = 0; i < triangle[n - 1].size(); ++i)
            dp[n - 1][i] = triangle[n - 1][i];
        for (int i = n - 2; i >= 0; --i) {
            for (int j = 0; j < triangle[i].size(); ++j) {
                dp[i][j] = min(dp[i + 1][j], dp[i + 1][j + 1]) + triangle[i][j];
            }
        }
        return dp[0][0];
    }
};
```
效率：
```
执行用时：12 ms, 在所有 C++ 提交中击败了48.21% 的用户
内存消耗：8.6 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
优化：题目中说，可以用三角形行数的额外空间解决问题。我仔细看了下题目，发现每次递推时只需要下一行的 `dp` 数组就可以了。**最大需要的空间等于三角形底部数组的大小**，或者说三角形的行数。优化后的代码如下：
```cpp
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if (triangle.empty() || triangle[0].empty()) return 0;
        int n = triangle.size(); 
        vector<int> dp(n);
        for (int i = 0; i < triangle[n - 1].size(); ++i)
            dp[i] = triangle[n - 1][i];
        for (int i = n - 2; i >= 0; --i) {
            for (int j = 0; j < triangle[i].size(); ++j) {
                dp[j] = min(dp[j], dp[j + 1]) + triangle[i][j];
            }
        }
        return dp[0];
    }
};
```
此时的效率如下：
```
执行用时：8 ms, 在所有 C++ 提交中击败了92.90% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了100.00% 的用户
```
还可以进一步优化以减少内存消耗，如果允许改变 `triangle` 的话，可以直接在 `triangle` 上面递推。代码如下：
```cpp
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        if (triangle.empty() || triangle[0].empty()) return 0; 
        for (int i = triangle.size() - 2; i >= 0; --i) 
            for (int j = 0; j < triangle[i].size(); ++j) 
                triangle[i][j] = min(triangle[i + 1][j], triangle[i + 1][j + 1]) + triangle[i][j]; 
        return triangle[0][0];
    }
};
```
