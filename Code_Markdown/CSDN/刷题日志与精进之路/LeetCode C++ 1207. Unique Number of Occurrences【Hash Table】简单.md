Given an array of integers `arr`, write a function that returns `true` if and only if the number of occurrences of each value in the array is unique.

 

**Example 1:**

```swift
Input: arr = [1,2,2,1,1,3]
Output: true
Explanation: The value 1 has 3 occurrences, 2 has 2 and 3 has 1. No two values have the same number of occurrences.
```

**Example 2:**

```swift
Input: arr = [1,2]
Output: false
```

**Example 3:**

```swift
Input: arr = [-3,0,1,-3,1,1,1,-3,10,0]
Output: true
```

**Constraints:**
- `1 <= arr.length <= 1000`
 - `-1000 <= arr[i] <= 1000`

 题意：对于一个整数数组，其中每个数都有一个出现次数。如果每个数的出现次数都是独一无二的，则返回 `true` ，否则返回 `false` 。


---
### 解法1 计数数组
由数据范围和数据规模得到下面的解法：
```cpp
class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        int cnt[2010] = {0}, ccnt[1010] = {0};
        for (const int &v : arr) ++cnt[v + 1000];
        for (int i = 0; i <= 2000; ++i) {
            if (cnt[i]) ++ccnt[cnt[i]]; //对于出现过的数的次数,计算出现次数
            if (ccnt[cnt[i]] > 1) return false;
        } 
        return true;
    }
};
```
效率不错：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了65.10% 的用户
```
---
### 解法2 哈希表
```cpp
class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        unordered_map<int, int> record, rrecord;
        for (const int &v : arr) ++record[v];
        for (auto &&it : record) {
            ++rrecord[it.second];
            if (rrecord[it.second] > 1) return false;
        }
        return true;
    }
};
```
效率稍微低一点：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了92.47% 的用户
内存消耗：8.5 MB, 在所有 C++ 提交中击败了14.11% 的用户
```
