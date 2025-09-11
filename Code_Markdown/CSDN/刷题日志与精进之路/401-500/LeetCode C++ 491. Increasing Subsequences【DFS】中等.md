Given an integer array, your task is to find all the **different** possible increasing subsequences of the given array, and the length of an increasing subsequence should be at least `2` .

**Example:**
```cpp
Input: [4, 6, 7, 7]
Output: [[4, 6], [4, 7], [4, 6, 7], [4, 6, 7, 7], [6, 7], [6, 7, 7], [7,7], [4,7,7]]
```
**Constraints:**
- The length of the given array will not exceed `15` .
- The range of integer in the given array is `[-100,100]` .
- The given array may contain duplicates, and **two equal integers** should also be considered as a special case of increasing sequence.

题意：找到所有该数组的递增子序列，递增子序列的长度至少是 `2` 。
 
---
思路1：用 `set` 去重，然后DFS+回溯解决。需要注意的是：**递增子序列必须是不同的**；**相等的数字也是递增的一种情况**；只要子序列递增并且大小 `>= 2` ，就可以插入到 `set` 中。整个代码如下：
```cpp
class Solution {
public:
    set<vector<int>> res;
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        vector<int> tmp;
        dfs(nums, 0, tmp);
        return vector<vector<int>>(res.begin(),res.end());
    }

    void dfs(vector<int>& nums, int i, vector<int> &tmp){
        if (tmp.size() >= 2) {
            res.insert(tmp);
        }
        for(int j = i; j < nums.size(); ++j){
            if (!tmp.empty() && tmp.back() > nums[j]) continue;
            tmp.push_back(nums[j]);
            dfs(nums, j + 1, tmp);
            tmp.pop_back();
        }
    }
};
```
效率较低：
```cpp
执行用时：228 ms, 在所有 C++ 提交中击败了34.51% 的用户
内存消耗：22.1 MB, 在所有 C++ 提交中击败了67.70% 的用户
```
还有其他解法，日后再更新。
