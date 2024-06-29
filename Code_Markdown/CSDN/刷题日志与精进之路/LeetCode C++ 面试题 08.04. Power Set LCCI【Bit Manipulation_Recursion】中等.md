

Write a method to return all subsets of a set. The elements in a set are pairwise distinct.

**Note:** The result set should not contain duplicated subsets.

**Example:**

```swift
Input:  nums = [1,2,3]
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
题意：编写一种方法，返回某集合的所有子集。原始集合中**不包含重复的元素**。解集中也**不能包含重复的子集**。

---
### 解法1 二进制迭代枚举
```cpp
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        if (nums.empty()) return {};
        int n = nums.size(), tot = (1 << n);
        vector<vector<int>> ans;
        for (int i = 0; i < tot; ++i) {
            vector<int> temp;
            int ti = i;
            for (int j = 0; j < n; ++j) {
                if ((i >> j) & 1) temp.push_back(nums[j]);
            }
            ans.push_back(temp);
        }
        return ans;
    }
};
```
对应的效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了64.75% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了61.46% 的用户
```
---
### 解法2 递归生成子集
对每个元素，都存在选择和不选两种可能。因此下述代码**实质上是对下图中的子集树进行递归中序遍历，并且记录其叶子节点**：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020112815290394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

```cpp
class Solution {
private:
    vector<vector<int>> ans;
    void dfs(vector<int>& nums, int i, int n, vector<int>& temp) {
        if (i >= n) { ans.emplace_back(temp); return; }
        temp.push_back(nums[i]);
        dfs(nums, i + 1, n, temp);
        temp.pop_back();
        dfs(nums, i + 1, n, temp);
    }
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        if (nums.empty()) return {};
        vector<int> t;
        dfs(nums, 0, nums.size(), t);
        return ans;
    }
};
```
执行结果如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了64.75% 的用户
内存消耗：11.3 MB, 在所有 C++ 提交中击败了5.84% 的用户
```
