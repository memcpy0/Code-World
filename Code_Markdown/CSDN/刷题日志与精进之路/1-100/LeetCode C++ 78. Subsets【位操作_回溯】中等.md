Given a set of **distinct** integers, nums, return all possible subsets (the power set).

Note: The solution set **must not contain duplicate subsets**.

**Example:**
```cpp
Input: nums = [1,2,3]
Output:
[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
```
题意：给出一个整数数组，元素不重复，返回子集集合。

---
### 思路1
二进制向量法。代码如下：
```cpp
class Solution {
public:  
    vector<vector<int>> subsets(vector<int>& nums) {
        int n = nums.size(), tot = (1 << nums.size());
        vector<vector<int>> ans;
        vector<int> vi;
        for (int i = 0; i < tot; ++i) {
            vi.clear();
            for (int j = 0; j < n; ++j) 
                if (i & (1 << j))
                    vi.push_back(nums[j]);
            ans.push_back(vi);
        }
        return ans; 
    }
};
```
效率较高：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.7 MB, 在所有 C++ 提交中击败了97.60% 的用户
```

---
### 思路2
递归方法。代码如下：
```cpp
class Solution {
public:
    vector<vector<int>> ans;
    vector<int> vi;
    void dfs(int pos, const vector<int> &nums) {
        if (pos >= nums.size()) {
            ans.push_back(vi);
            return;
        }
        vi.push_back(nums[pos]);
        dfs(pos + 1, nums);
        vi.pop_back();
        dfs(pos + 1, nums);
    }
    vector<vector<int>> subsets(vector<int>& nums) { 
        dfs(0, nums);
        return ans; 
    }
};
```
或者写成下面这种形式，先序遍历解答树（以后有时间画一下解答树）：
```cpp
class Solution {
public: 
    vector<vector<int>> ans;
    vector<int> vi;
    void dfs(int pos, const vector<int> &nums) {
        ans.push_back(vi);
        for (int i = pos; i < nums.size(); ++i) {
            vi.push_back(nums[i]);
            dfs(i + 1, nums);
            vi.pop_back();
        }
    } 
    vector<vector<int>> subsets(vector<int>& nums) { 
        dfs(0, nums);
        return ans; 
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.9 MB, 在所有 C++ 提交中击败了78.22% 的用户
```
