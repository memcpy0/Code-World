A sequence of numbers is called **a wiggle sequence** if the differences between successive numbers strictly alternate between positive and negative. The first difference (if one exists) may be either positive or negative. A sequence with fewer than two elements is trivially a wiggle sequence.

For example, `[1,7,4,9,2,5]` is a wiggle sequence because the differences `(6,-3,5,-7,3)` are alternately positive and negative. In contrast, `[1,4,7,2,5]` and `[1,7,4,5,5]` are not wiggle sequences, the first because its first two differences are positive and the second because its last difference is zero.

Given a sequence of integers, ***return the length of the longest subsequence that is a wiggle sequence***. A subsequence is obtained by deleting some number of elements (eventually, also zero) from the original sequence, leaving the remaining elements in their original order.

**Example 1:**

```swift
Input: [1,7,4,9,2,5]
Output: 6
Explanation: The entire sequence is a wiggle sequence.
```

**Example 2:**

```swift
Input: [1,17,5,10,13,15,10,5,16,8]
Output: 7
Explanation: There are several subsequences that achieve this length. One is [1,17,10,13,10,16,8].
```

**Example 3:**

```swift
Input: [1,2,3,4,5,6,7,8,9]
Output: 2
```

**Follow up:** Can you do it in `O(n)` time?

题意：如果连续数字之间的**差**严格地在正数和负数之间交替，则数字序列称为**摆动序列**。第一个差（如果存在的话）可能是正数或负数。少于两个元素的序列也是摆动序列。

例如， `[1,7,4,9,2,5]` 是一个摆动序列，因为差值 `(6,-3,5,-7,3)` 是正负交替出现的。相反 `[1,4,7,2,5]` 和 `[1,7,4,5,5]` 不是摆动序列，第一个序列是因为它的前两个差值都是正数，第二个序列是因为它的最后一个差值为零。

给定一个整数序列，返回作为摆动序列的**最长子序列**的长度。 通过从原始序列中删除一些（也可以不删除）元素来获得子序列，剩下的元素保持其原始顺序。 

---
### 解法 动态规划
连续数字之间的**差**严格地在正数和负数之间交替，这种说法不太直观——无非是 `A[i - 1] < A[i] > A[i + 1]` 或者 `A[i - 1] > A[i] < A[i + 1]` 这样的变化序列。

应用最长递增子序列的思想，得到 $O(n^2)$ 的一个解法。具体代码如下：
```cpp
class Solution {
public:
    int wiggleMaxLength(vector<int>& nums) {
        if (nums.empty()) return 0;
        int n = nums.size(), ans = 1;
        //dp[i][0]表示以nums[i]为上升结尾的最长摆动序列长度
        //dp[i][1]表示以nums[i]为下降结尾的最长摆动序列长度
        vector<vector<int>> dp(n, vector<int>(2));
        dp[0][0] = dp[0][1] = 1;
        for (int i = 1; i < n; ++i) {
            for (int j = 0; j < i; ++j) { 
                if (nums[i] < nums[j]) dp[i][1] = max(dp[i][1], dp[j][0] + 1); //能够以nums[i]为下降结尾
                else if (nums[i] > nums[j]) dp[i][0] = max(dp[i][0], dp[j][1] + 1); //能够以nums[i]为上升结尾
            }
            ans = max(ans, max(dp[i][0], dp[i][1]));
        }
        return ans;
    }   
};
```
运行效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了6.27% 的用户
内存消耗：7.9 MB, 在所有 C++ 提交中击败了5.06% 的用户
```
优化为 $O(n)$ 算法：
```cpp
class Solution {
public:
    int wiggleMaxLength(vector<int>& nums) {
        if (nums.empty()) return 0;
        int n = nums.size(), ans = 1; 
        vector<vector<int>> dp(n, vector<int>(2));
        dp[0][0] = dp[0][1] = 1;
        for (int i = 1; i < n; ++i) {
            if (nums[i] > nums[i - 1]) {//nums[i-1]到nums[i]上升
                dp[i][0] = max(dp[i - 1][1] + 1, dp[i - 1][0]);
                dp[i][1] = dp[i - 1][1];
            } else if (nums[i] < nums[i - 1]) {
                dp[i][0] = dp[i - 1][0];
                dp[i][1] = max(dp[i - 1][0] + 1, dp[i - 1][1]);
            } else {
                dp[i][0] = dp[i - 1][0];
                dp[i][1] = dp[i - 1][1];
            }
            ans = max(ans, max(dp[i][0], dp[i][1]));
        }
        return ans;
    }   
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了48.95% 的用户
内存消耗：7.7 MB, 在所有 C++ 提交中击败了5.06% 的用户
```
