
Given the array `nums`, for each `nums[i]` find out how many numbers in the array are smaller than it. That is, for each `nums[i]` you have to count the number of valid `j's` such that `j != i` and `nums[j] < nums[i]` . Return the answer in an array.

 

**Example 1:**

```swift
Input: nums = [8,1,2,2,3]
Output: [4,0,1,1,3]
Explanation: 
For nums[0]=8 there exist four smaller numbers than it (1, 2, 2 and 3). 
For nums[1]=1 does not exist any smaller number than it.
For nums[2]=2 there exist one smaller number than it (1). 
For nums[3]=2 there exist one smaller number than it (1). 
For nums[4]=3 there exist three smaller numbers than it (1, 2 and 2).
```

**Example 2:**

```swift
Input: nums = [6,5,4,8]
Output: [2,1,0,3]
```

**Example 3:**

```swift
Input: nums = [7,7,7,7]
Output: [0,0,0,0]
```

 

**Constraints:**
- `2 <= nums.length <= 500`
- `0 <= nums[i] <= 100`


题意：给出一个数组，对于其中每个元素，统计数组中比它小的所有数字的数目，并以数组形式返回答案。 

---
### 解法1 暴力
```cpp
class Solution {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        vector<int> ans;
        for (int i = 0; i < nums.size(); ++i) {
            int smallerNumbers = 0;
            for (int j = 0; j < nums.size(); ++j) 
                if (j != i && nums[j] < nums[i]) ++smallerNumbers;
            ans.push_back(smallerNumbers);
        }
        return ans;
    }
};
```
效率比较低下，不值一提：
```cpp
执行用时：96 ms, 在所有 C++ 提交中击败了6.12% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了7.11% 的用户
```

---
### 解法2 排序+二分
复制原数组并进行排序，然后对原数组中的每个元素，在新的有序数组中二分查找其位置（找到第一个大于等于它的数的位置）：
```cpp
class Solution {
private:
    int binarySearch(const vector<int>& nums, int lo, int hi, int target) {
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] >= target) hi = mid; //注意二分的条件
            else lo = mid + 1; 
        }
        return lo;
    }
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int n = nums.size();
        vector<int> temp(nums.begin(), nums.end()), ans;
        sort(temp.begin(), temp.end()); 
        for (const int &v : nums) 
            ans.push_back(binarySearch(temp, 0, n, v));
        return ans;
    }
};
```
$O(n\log n)$ 时间，$O(n)$ 空间。效率如下，大大提高：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了70.52% 的用户
内存消耗：10.4 MB, 在所有 C++ 提交中击败了5.06% 的用
```
---
### 解法3 计数排序+前缀和
这里看到数据范围 `<= 100` ，可以使用计数排序+前缀和，得到对于每个元素 `nums[i]` 而言小于它的数的数量。这也是**基数排序**中用过的一个小技巧：
```cpp
class Solution {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        int buckets[105] = {0};
        vector<int> ans;
        for (const int &v : nums) ++buckets[v];
        for (int i = 1; i <= 100; ++i) buckets[i] += buckets[i - 1];
        for (const int &v : nums) 
            ans.push_back(!v ? 0 : buckets[v - 1]); 
        return ans;
    }
};
```
效率如下，对于这种规模的数据， $O(n)$ 和 $O(n\log n)$ 的时间复杂度区别不大：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了85.34% 的用户
内存消耗：10.3 MB, 在所有 C++ 提交中击败了5.30% 的用户
```
