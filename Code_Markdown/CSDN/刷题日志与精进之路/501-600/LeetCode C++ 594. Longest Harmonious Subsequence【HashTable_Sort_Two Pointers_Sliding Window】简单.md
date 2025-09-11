We define a harmonious array as an array where the difference between its maximum value and its minimum value is **exactly** `1` .

Given an integer array `nums`, return *the length of its longest harmonious subsequence among all its possible subsequences.*

A **subsequence** of array is a sequence that can be derived from the array by deleting some or no elements without changing the order of the remaining elements.

 

**Example 1:**

```swift
Input: nums = [1,3,2,2,5,2,3,7]
Output: 5
Explanation: The longest harmonious subsequence is [3,2,2,2,3].
```

**Example 2:**

```swift
Input: nums = [1,2,3,4]
Output: 2
```

**Example 3:**

```swift
Input: nums = [1,1,1,1]
Output: 0
```

**Constraints:**
-  <code>1 <= nums.length <= 2 * 10<sup>4</sup></code>
 -  <code>-10<sup>9</sup> <= nums[i] <= 10<sup>9</sup></code>

题意： 和谐数组是指一个数组里元素的最大值和最小值之间的差别正好是 `1` 。给定一个整数数组，需要在所有可能的子序列中找到最长的和谐子序列的长度。

---
### 解法1 哈希表
使用哈希表计算出每个数的出现次数。然后判断是否存在和谐子序列——即最小值、最大值相差为 `1` （证明两者相邻）的子序列，计算最长和谐子序列的长度。
```cpp
class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> cnt;
        for (const int &v : nums) ++cnt[v];
        if (cnt.size() == 1) return 0; //只有一种值,构不成和谐序列
        int ans = 0; //最长和谐子序列的长度
        //bool haveHarmoniousSeq = false; //是否存在和谐子序列
        for (auto &it : cnt) {
            int v = it.first, count = it.second;
            if (cnt.find(v - 1) != cnt.end()) {
                //haveHarmoniousSeq = true;
                ans = max(ans, cnt[v - 1] + count);
            }
            if (cnt.find(v + 1) != cnt.end()) {
                //haveHarmoniousSeq = true;
                ans = max(ans, cnt[v + 1] + count);
            }   
        }  
        //return haveHarmoniousSeq ? ans : 0;
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：200 ms, 在所有 C++ 提交中击败了76.32% 的用户
内存消耗：39.5 MB, 在所有 C++ 提交中击败了52.10% 的用户
```
这样比较慢。事实上对每个元素，我们不需要向左右两边寻找，而只需**向右寻找**，因为每个数的出现次数已经被确定：
```cpp
class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> cnt;
        for (const int &v : nums) ++cnt[v];
        if (cnt.size() == 1) return 0; //只有一种值,构不成和谐序列
        int ans = 0; //最长和谐子序列的长度
        bool haveHarmoniousSeq = false; //是否存在和谐子序列
        for (auto &it : cnt) {
            int v = it.first, count = it.second; 
            if (cnt.find(v + 1) != cnt.end()) {
                haveHarmoniousSeq = true;
                ans = max(ans, cnt[v + 1] + count);
            }   
        }  
        return haveHarmoniousSeq ? ans : 0;
    }
};
```
提高了一点效率：
```cpp
执行用时：192 ms, 在所有 C++ 提交中击败了81.20% 的用户
内存消耗：39.7 MB, 在所有 C++ 提交中击败了50.10% 的用户
```
甚至于，我们不需要分为两个循环，分别计算每个数的出现次数和最长和谐子序列的长度，而是**合并为一个循环**。由于此时每个数的出现次数没有确定，因此对每个元素，**必须查看左右相邻数值的出现次数**。不然对 `[1,3,2,2,5,2,3,7]` 这样的测试用例，就会出错：
```cpp
class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> cnt;
        int ans = 0; //最长和谐子序列的长度
        for (const int &v : nums) {
            ++cnt[v];  
            int count = cnt[v]; 
            if (cnt.find(v - 1) != cnt.end())  
                ans = max(ans, cnt[v - 1] + count); 
            if (cnt.find(v + 1) != cnt.end())  
                ans = max(ans, cnt[v + 1] + count); 
        }  
        return ans;
    }
};
```
执行效率见下，反而降低了。一个因素可能是——测试用例中，数组内重复的元素比较多，分成两个循环时第二个循环相当于对**去重后的数组**进行操作；而合成一个循环后，需要对许多重复的元素进行哈希表查找：
```cpp
执行用时：256 ms, 在所有 C++ 提交中击败了49.06% 的用户
内存消耗：39.6 MB, 在所有 C++ 提交中击败了51.53% 的用户
```
---
### 解法2 排序+滑动窗口
先对数组进行排序，然后用两个指针，`begin` 指向小 `1` 的元素区间的最左端，`end` 在大 `1` 的元素区间上移动，此时不断更新最长和谐子序列的长度。当两个指针指向的元素相差大于 `1` 时，移动 `begin` 直到 `nums[begin]` 和 `nums[end]` 相差 `<= 1` ：
```cpp
class Solution {
public:
    int findLHS(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), begin = 0, res = 0;
        for (int end = 0; end < n; ++end) {
            while (nums[end] - nums[begin] > 1) ++begin; //将滑动窗口的左端移动到和nums[end]相差为1的元素位置
            if (nums[end] - nums[begin] == 1)
                res = max(res, end - begin + 1);
        }
        return res;
    }
};
```
提交后效率如下：
```cpp
执行用时：188 ms, 在所有 C++ 提交中击败了84.12% 的用户
内存消耗：32.1 MB, 在所有 C++ 提交中击败了93.04% 的用户
```
