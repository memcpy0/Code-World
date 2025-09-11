Given two arrays, write a function to compute their intersection.

**Example 1:**
```swift
Input: nums1 = [1,2,2,1], nums2 = [2,2]
Output: [2]
```
**Example 2:**
```swift
Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
Output: [9,4]
```
**Note:** 
-    Each element in the result **must be unique**.
-    The result can be **in any order.**

 

题意：给出两个数组，要求找出其交集，结果中的元素必须是唯一的，可以无序。

---
### 解法1 暴力方法+一个哈希集合
这道题是道简单题目，一眼就可以看出来要怎样做。暴力的思路：遍历第一个数组 `nums1[]` ，并检查每个值是否存在于 `nums2[]` 内。如果存在则将值添加到集合中，最后将集合的元素全部添加到结果向量中。
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> st;
        for (const int &v : nums1) {
            bool exist = false;
            for (const int &k : nums2) {
                if (v == k) { exist = true; break; }
            }
            if (exist) st.insert(v);
        }
        return vector<int>(st.begin(), st.end());
    }
};
```
这样的方法会导致 $O(n \times m )$ 的时间复杂度，其中 $n$ 和 $m$ 分别是数组的长度。当然，插入集合和最后复制为向量也会有一定的时间消耗。具体效率如下：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了5.86% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了63.63% 的用户
```

---
### 解法2 两个哈希集合
好一点的方法是：将两个数组的元素分别添加到两个 `set` 中去重，然后遍历第一个 `set` 中的元素，检查其是否出现在了第二个集合中，是则添加到结果向量中。
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        unordered_set<int> st1, st2;
        vector<int> ans;
        for (const int &v : nums1) st1.insert(v);
        for (const int &v : nums2) st2.insert(v);
        for (auto &&it = st1.begin(); it != st1.end(); ++it) 
            if (st2.find(*it) != st2.end()) ans.push_back(*it);
        return ans;
    }
};
```
如果使用 `set` 的话，时间复杂度为 $O(n\log n + m\log m + n\log m)$ 。不过这里使用的是 `unordered_set` ，按理来说时间复杂度应该为 $O(n + m)$ 。具体效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了39.19% 的用户
内存消耗：11.1 MB, 在所有 C++ 提交中击败了5.05% 的用户
```

---
### 解法3 还是一个哈希集合

**更好的方法是**：将 `nums1[]` 数组元素全部放入 `set` 中去重，再遍历 `nums2` ，看其元素是否也存在于 `set` 中，是则添加到结果向量中，**添加后要从 `set` 中删除该元素，防止添加 `nums2` 中的重复数字了**。

```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<int> ans;
        unordered_set<int> st;
        for (const int &v : nums1) st.insert(v); //得到nums1中的所有非重复元素
        for (const int &v : nums2) {
            if (st.find(v) != st.end()) {
                ans.push_back(v);
                st.erase(v);
            }
        }
        return ans;
    }
};
```
使用 `set` 的时间复杂度是 $O(n\log n + m\log n)$ ，如果使用 `unordered_set` ，时间复杂度为 $O(n + m)$ 。具体效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了96.25% 的用户
内存消耗：10.7 MB, 在所有 C++ 提交中击败了13.65% 的用户
```
---
### 解法4 排序+双指针
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());
        int i = 0, j = 0;
        unordered_set<int> st;
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] == nums2[j]) {
                st.insert(nums1[i]);
                ++i;
                ++j;
            } 
            else if (nums1[i] < nums2[j]) ++i;
            else ++j;
        }
        return vector<int>(st.begin(), st.end());
    }
};
```
时间复杂度为 $O(n\log n + m\log m+ n + m)$ ，具体效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了96.25% 的用户
内存消耗：10.5 MB, 在所有 C++ 提交中击败了39.07% 的用户
```

---
### 解法5 排序+二分查找
```cpp
class Solution {
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        sort(nums1.begin(), nums1.end());  
        unordered_set<int> st;
        for (const int &v : nums2) {
            auto it = lower_bound(nums1.begin(), nums1.end(), v);
            if (it != nums1.end() && *it == v) st.insert(*it); //nums1中存在第一个>=v的值
        }
        return vector<int>(st.begin(), st.end());
    }
};
```
时间复杂度为 $O(n\log n+ m\log n)$ ，具体效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了96.25% 的用户
内存消耗：10.5 MB, 在所有 C++ 提交中击败了36.44% 的用户
```
