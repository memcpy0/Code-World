A popular masseuse receives a sequence of back-to-back appointment requests and is debating which ones to accept. She needs a break between appointments and therefore she cannot accept any adjacent requests. Given a sequence of back-to-back appoint­ ment requests, find the optimal (highest total booked minutes) set the masseuse can honor. Return the number of minutes.

Note: This problem is slightly different from the original one in the book.

 

**Example 1:**

```swift
Input:  [1,2,3,1]
Output:  4
Explanation:  Accept request 1 and 3, total minutes = 1 + 3 = 4
```

**Example 2:**

```swift
Input:  [2,7,9,3,1]
Output:  12
Explanation:  Accept request 1, 3 and 5, total minutes = 2 + 9 + 1 = 12
```

**Example 3:**

```swift
Input:  [2,1,4,5,3,1,1,3]
Output:  12
Explanation:  Accept request 1, 3, 5 and 8, total minutes = 2 + 4 + 3 + 3 = 12
```

 

题意：一个受欢迎的女按摩师收到了一系列连续的预约请求，她在每次预约按摩之后需要休息，因此不能接受任何时间相邻的请求。请帮助她找到最优的预约集合，使得总预约时间最长，并返回总的时间。

----
### 解法 动态规划
这道题是很简单的动态规划。设 `dp[i]` 为 `nums[0 : i]` 中最长的总预约时间，于是 `dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]` ，即要么接受前一个预约（放弃当前预约）、要么放弃前一个预约（接受当前预约）。实际代码如下：
```cpp
class Solution {
public:
    int massage(vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        vector<int> dp(nums.size());
        dp[0] = nums[0];
        dp[1] = max(nums[1], dp[0]);
        for (int i = 2; i < nums.size(); ++i)
            dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]);
        return dp.back();
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了43.73% 的用户
内存消耗：7.9 MB, 在所有 C++ 提交中击败了13.06% 的用户
```
由于我们只需要 `dp[n - 1]` ，因此使用两个变量进行递推，以节省空间：
```cpp
class Solution {
public:
    int massage(vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        int a = nums[0], b = max(nums[1], a);
        for (int i = 2; i < nums.size(); ++i) {
            int t = b;
            b = max(a + nums[i], b);
            a = t;
        }
        return b;
    }
};
```
效率有所提高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8 MB, 在所有 C++ 提交中击败了5.94% 的用户
```
