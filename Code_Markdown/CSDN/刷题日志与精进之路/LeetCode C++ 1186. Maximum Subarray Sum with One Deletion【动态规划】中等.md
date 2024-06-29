Given an array of integers, return the maximum sum for a **non-empty** subarray (contiguous elements) with at most one element deletion. In other words, you want to choose a subarray and optionally delete one element from it so that there is still at least one element left and the sum of the remaining elements is maximum possible.

Note: that the subarray needs to be **non-empty** after deleting one element.

 

Example 1:
```cpp
Input: arr = [1,-2,0,3]
Output: 4
Explanation: Because we can choose [1, -2, 0, 3] and drop -2, thus the subarray [1, 0, 3] becomes the maximum value.
```

Example 2:
```cpp
Input: arr = [1,-2,-2,3]
Output: 3
Explanation: We just choose [3] and it's the maximum sum.
```

Example 3:
```cpp
Input: arr = [-1,-1,-1,-1]
Output: -1
Explanation: The final subarray needs to be non-empty. You can't choose [-1] and delete -1 from it, then get an empty subarray to make the sum equals to 0.
```
**Constraints**:
-    `1 <= arr.length <= 10^5`
-    `-10^4 <= arr[i] <= 10^4`

题目：给出一个整数数组，返回所能得到的最大元素和——来自整数数组的某个非空子数组在执行一个可选的删除操作后(删除后不能为空)的结果。


思路：这是一道动态规划的题目。由于可选删除与否，因此用 `dp[i][0]` 表示 `arr[0-i]` 没有删除过；`dp[i][1]` 表示 `arr[0-i]` 删除过一次。另外，`dp[i][0/1]` 表示的是**最近的一个子序列**在一次可选的删除操作后的最大值。其他的注释写在代码中。

代码：
```cpp
class Solution {
public:
    int maximumSum(vector<int>& arr) {
        vector<vector<int>> dp(arr.size(), vector<int>(2, 0));
        //dp[i][0]表示i及之前的序列没有删除过,得到的最大子序列和
        //dp[i][1]表示i及之前的序列删除过,得到的最大子序列和
        dp[0][0] = arr[0], dp[0][1] = 0;
        int maxSum = arr[0], n = arr.size();               //删除后不能为空,因此最初最大子序列和是arr[0]
        for (int i = 1; i < n; ++i) {
            dp[i][0] = max(dp[i - 1][0] + arr[i], arr[i]); //没有删除过之前的和+现在的arr[i] 与 arr[i]; 如果arr[i]更大,则断开了两个子序列
            maxSum = max(dp[i][0], maxSum);                //所有没有删除过的子序列中找最大值
            dp[i][1] = max(dp[i - 1][0], dp[i - 1][1] + arr[i]); //之前没有删除+删除现在的值 与 之前删除过+现在不删除
            maxSum = max(dp[i][1], maxSum);                //所有删除过的子序列中找最大值
        } 
        return maxSum;
        /*          1   -2       0        3
        dp[i][0]    1   -1    |  0        3    //最大子序列和是3
        dp[i][1]    0   1(-2)    1        4(0) //最大子序列和是4,删除了[-2]
                        删除-2  删除-2  删除-2
                    1   -2      -2        3
        dp[i][0]    1   -1    | -2    |   3     //最大子序列和是3,没有删除
        dp[i][1]    0   1(-2)   -1        2(-2) //最大子序列和是2,删除任一[-2]
                        删除-2  删除-2
                   -1   -1      -1       -1
        dp[i][0]   -1 | -1    | -1    |  -1     //最大子序列和是-1
        dp[i][1]    0   -1      -1       -1
        */
    }
};
```



