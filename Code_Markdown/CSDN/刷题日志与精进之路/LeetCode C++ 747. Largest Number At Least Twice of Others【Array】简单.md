

In a given integer array `nums`, there is always exactly one largest element.

Find whether the largest element in the array is at least twice as much as every other number in the array.

If it is, return the **index** of the largest element, otherwise return -1.

**Example 1:**

```swift
Input: nums = [3, 6, 1, 0]
Output: 1
Explanation: 6 is the largest integer, and for every other number in the array x,
6 is more than twice as big as x.  The index of value 6 is 1, so we return 1.
```

 

**Example 2:**

```swift
Input: nums = [1, 2, 3, 4]
Output: -1
Explanation: 4 isn't at least as big as twice the value of 3, so we return -1.
```
**Note:**
- nums will have a length in the range `[1, 50]` .
 - Every nums[i] will be an integer in the range `[0, 99]` .




题意：在一个给定的数组 `nums` 中，总是存在一个最大元素 。查找数组中的最大元素，并确认其是否至少是数组中每个其他数字的两倍。如果是，则返回最大元素的索引，否则返回 `-1` 。

---
### 解法1 两次遍历
扫描数组以找到唯一的最大元素 `m` ，其索引为 `maxIndex` 。再次扫描数组，如果找到一个值 `x != m` 且 `m < 2 * x` ，则返回 `-1` 。否则应该返回 `maxIndex` 。具体代码如下：
```cpp
class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        if (nums.size() == 1) return 0;
        int maxIdx = 0, n = nums.size();
        for (int i = 1; i < n; ++i) 
            if (nums[i] > nums[maxIdx]) 
                maxIdx = i;
        for (int i = 0; i < n; ++i) 
            if (i != maxIdx && nums[i] * 2 > nums[maxIdx])
                return -1;
        return maxIdx;
    }
};
```
运行效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了38.96% 的用户
内存消耗：11.3 MB, 在所有 C++ 提交中击败了18.20% 的用户
```
另外，此题评论区中有人认为题目描述不够严谨：比如只有一个数时，最大值显而易见，但是不存在其他元素，需要返回 `-1` 还是 `0` ？还有人认为：如果数组中存在多个重复的最大值，那么结果怎么办？前一个疑问是有道理的，后一个则明显没有认真读题——`there is always exactly one largest element` 。

 

---
### 解法2 一次遍历
两次遍历的做法稍微慢一点。其实我们找到最大值后，不需要和数组中的其他每个值都进行比较。只要**顺手找到第二大的元素**，将它的两倍值与最大值进行比较，就能证明最大值是否大于其他所有元素两倍。
```cpp
class Solution {
public:
    int dominantIndex(vector<int>& nums) {
        if (nums.size() == 1) return 0;
        int n = nums.size(), maxIdx = -1, max = -1, sec = -2; //最大,次大
        for (int i = 0; i < n; ++i) {
            if (nums[i] > max) {
                sec = max;      //不断求出次大值
                max = nums[i];  //更新最大值
                maxIdx = i;
            } else if (nums[i] > sec) sec = nums[i]; //更新次大值
        }
        return max >= sec * 2 ? maxIdx : -1;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了82.99% 的用户
内存消耗：11.4 MB, 在所有 C++ 提交中击败了9.33% 的用户
```
