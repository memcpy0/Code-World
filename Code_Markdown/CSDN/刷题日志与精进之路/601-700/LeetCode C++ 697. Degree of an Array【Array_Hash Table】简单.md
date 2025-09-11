Given a non-empty array of non-negative integers `nums`, the **degree** of this array is defined as the maximum frequency of any one of its elements.

Your task is to find the smallest possible length of a (contiguous) subarray of `nums`, that has the same degree as `nums`.

 

**Example 1:**

```swift
Input: nums = [1,2,2,3,1]
Output: 2
Explanation: 
The input array has a degree of 2 because both elements 1 and 2 appear twice.
Of the subarrays that have the same degree:
[1, 2, 2, 3, 1], [1, 2, 2, 3], [2, 2, 3, 1], [1, 2, 2], [2, 2, 3], [2, 2]
The shortest length is 2. So return 2.
```

**Example 2:**

```swift
Input: nums = [1,2,2,3,1,4,2]
Output: 6
Explanation: 
The degree is 3 because the element 2 is repeated 3 times.
So [2,2,3,1,4,2] is the shortest subarray, therefore returning 6.
```

 

**Constraints:**
- `nums.length` will be between `1` and `50,000`.
 - `nums[i]` will be an integer between `0` and `49,999`.


 题意：一个非空整数数组的度，被定义为数组里任一元素出现频数的最大值。找到与 `nums` 拥有**相同大小的度**的**最短连续子数组**，返回其长度。

---
### 解法1 哈希表
计算数组中每个元素出现的次数，同时使用哈希表记录**元素出现区间的左右端点**。然后遍历得到数组的度（任一元素出现频数的最大值）和**出现频数等于数组度的元素集合**。在这个元素集合中，找到长度最小的子数组，返回其长度：
```cpp
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        unordered_map<int, int> record; //val:freq
        unordered_map<int, pair<int, int>> ranges; //val:{leftPos, rightPos}
        int n = nums.size(), maxFreq = -1, minLenOfSubArray = INT_MAX;
        vector<int> maxOccurs; //发生频数等于数组的度(最大频数)的元素集合
        for (int i = 0; i < n; ++i) {
            ++record[nums[i]];
            if (ranges.find(nums[i]) != ranges.end()) //已经出现过
                ranges[nums[i]].second = i;           //更新元素出现区间的右端点
            else ranges[nums[i]] = pair<int, int>{i, i};
        }
        for (const auto &it : record) {
            if (it.second > maxFreq) { //发现更大频数
                maxOccurs.clear();     //清空之前的最大频数对应的元素集合
                maxOccurs.push_back(it.first);
                maxFreq = it.second;   //更新最大频数
            } else if (it.second == maxFreq) 
                maxOccurs.push_back(it.first);
        }
        for (const int &v : maxOccurs) //得到maxOccurs中的最短子数组
            minLenOfSubArray = min(minLenOfSubArray, ranges[v].second - ranges[v].first + 1);
        return minLenOfSubArray;
    } 
};
```
执行效率比较低：
```cpp
执行用时：116 ms, 在所有 C++ 提交中击败了63.19% 的用户
内存消耗：25.7 MB, 在所有 C++ 提交中击败了54.67% 的用户
```
---
### 解法2 计数数组
相对而言，使用哈希表比较浪费时间和空间——由于数据范围比较小，可以替代使用**计数数组和整数哈希**：
```cpp
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        if (nums.size() == 1) return 1;
        int n = nums.size(), cnt[50010] = {0}; //val:freq
        int ranges[50010][2] = {0};            //val:{leftPos, rightPos} 
        for (int i = 0; i < nums.size(); ++i) { 
            if (cnt[nums[i]] == 0) ranges[nums[i]][0] = i; //没有出现过,记录左端点
            ++cnt[nums[i]];
            ranges[nums[i]][1] = i; //没出现过时记录右端点; 出现过则更新右端点
        }
        int maxFreqIdx = 0, minLenOfSubArray = ranges[0][1] - ranges[0][0] + 1; 
        vector<int> maxOccurs; //发生频数等于数组的度(最大频数)的元素集合
        for (int i = 1; i < 50000; ++i) {
            if (cnt[i] > cnt[maxFreqIdx]) { //发现更大频数
                maxFreqIdx = i;             //更新最大频数对应的值
                minLenOfSubArray = ranges[i][1] - ranges[i][0] + 1; //更新更大频度对应的区间长度
            } else if (cnt[i] == cnt[maxFreqIdx]) { //频数相等
                minLenOfSubArray = min(ranges[i][1] - ranges[i][0] + 1,
                                       ranges[maxFreqIdx][1] - ranges[maxFreqIdx][0] + 1);
                if (minLenOfSubArray == (ranges[i][1] - ranges[i][0] + 1))
                    maxFreqIdx = i;
            }
        }
        return minLenOfSubArray;
    } 
};
```
效率大大提升：
```cpp
执行用时：76 ms, 在所有 C++ 提交中击败了95.10% 的用户
内存消耗：24.7 MB, 在所有 C++ 提交中击败了85.38% 的用户
```
