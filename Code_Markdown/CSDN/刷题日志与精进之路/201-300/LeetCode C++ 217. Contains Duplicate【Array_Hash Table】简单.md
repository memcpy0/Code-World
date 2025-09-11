
Given an array of integers, find if the array contains any duplicates.

Your function should return `true` if any value appears at least twice in the array, and it should return `false` if every element is distinct.

**Example 1:**

```haskell
Input: [1,2,3,1]
Output: true
```

**Example 2:**

```haskell
Input: [1,2,3,4]
Output: false
```

**Example 3:**

```haskell
Input: [1,1,1,3,3,4,3,2,4,2]
Output: true
```


题意：给定一个整数数组，判断是否存在重复元素。如果任意一值在数组中出现至少两次，函数返回 `true` 。如果数组中每个元素都不相同，则返回 `false` 。

---
### 解法 哈希表
```cpp
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_map<int, bool> rec;
        for (const int &v : nums) {
            if (rec[v] == false) rec[v] = true;
            else return true;
        }
        return false;
    }
};
```
运行效率如下：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了59.11% 的用户
内存消耗：19.4 MB, 在所有 C++ 提交中击败了47.40% 的用户
```
