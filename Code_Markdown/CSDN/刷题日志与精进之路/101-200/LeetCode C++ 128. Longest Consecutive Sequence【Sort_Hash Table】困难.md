
Given an unsorted array of integers `nums`, return the length of the longest consecutive elements sequence.

**Follow up**: Could you implement the `O(n)` solution? 

 

**Example 1:**

```swift
Input: nums = [100,4,200,1,3,2]
Output: 4
Explanation: The longest consecutive elements sequence is [1, 2, 3, 4]. Therefore its length is 4.
```

**Example 2:**

```swift
Input: nums = [0,3,7,2,5,8,4,6,0,1]
Output: 9
```

 

**Constraints:**
- <code>0 <= nums.length <= 10<sup>4</sup></code>
 - <code>-10<sup>9</sup> <= nums[i] <= 10<sup>9</sup></code>

题意：给定一个未排序的整数数组 `nums` ，找出**数字连续**的最长序列（**不要求序列元素在原数组中连续**）的长度。

 ---
### 解法1 排序
需要注意的是，这一题中的最长连续序列（不是子数组），甚至不要求序列中元素的顺序性。因此我们对整个数组进行排序，然后遍历即可：
```cpp
class Solution {
public: 
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        sort(nums.begin(), nums.end());
        int maxLen = 0, curLen = 1;
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == nums[i - 1]) continue; //相等的元素,不计算长度
            else if (nums[i] == nums[i - 1] + 1) ++curLen; //连续数字
            else {
                maxLen = max(maxLen, curLen);
                curLen = 1;
            }
        }
        return max(maxLen, curLen);
    }
};
```
时间复杂度为 $\text{O(nlogn)}$，空间复杂度为 $\text{O(n)}$ ，执行效率如下（可能是数据比较水吧）：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了99.48% 的用户
内存消耗：9.9 MB, 在所有 C++ 提交中击败了93.69% 的用户
```
**20201104 Update** 这一次做题时，针对重复元素使用的是**去重操作**：
```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) return 0;
        sort(nums.begin(), nums.end()); //排序
        int len = unique(nums.begin(), nums.end()) - nums.begin(); //去重
        int ans = 1, tempLen = 1;
        for (int i = 1; i < len; ++i) {
            if (nums[i] == nums[i - 1] + 1) ++tempLen; //连续数字
            else tempLen = 1;
            ans = max(ans, tempLen);
        }
        return ans;
    }
};
```
速度慢了一些：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了52.07% 的用户
内存消耗：9.9 MB, 在所有 C++ 提交中击败了92.80% 的用户
```
 
---
### 解法2 空间换时间
进阶要求是时间复杂度为 $O(n)$ 。为此可以用哈希表存储**每个端点值对应连续区间的长度**。每次扫描数组中的一个元素 `x`：
- 如果 `x` 已经存在于哈希表中，说明有重复元素，`x` 已经处在某个区间中，不做处理；
- 如果 `x`  是新数：
	- 取出其**左右相邻数**已有的连续区间长度 `left` 和 `right` ；不存在相邻数时，已有的连续区间长度 `left = right = 0` ；
	- 计算当前数的区间长度为 `cur_length = left + right + 1` ；
	- 根据 `cur_length` 更新**最大长度** `max_length` 的值；
	- 更新**区间两端点**的长度值。之所以**只更新连续区间的端点长度值**，而不更新连续数字区间中每个点中的值，是为了：
		- 降低时间复杂度，使更新操作为常数级；
		- **更新区间两个端点的值**就足够了。如果下一个数 `y` 已经存在于区间中，`y` 不会影响到结果；如果 `y` 在区间之外，和本区间形成连续的区间，**取出的左右相邻点只会是两个端点之一**，随后 `y` 成为新的端点之一，同时更新另外一个端点对应的长度值。

这种做法确实很像并查集。具体代码如下：
```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_map<int, int> ranges;
        int maxLen = 0;
        for (const int &v : nums) {
            if (!ranges[v]) {
                int l = ranges[v - 1], r = ranges[v + 1]; //左右相邻数已有的连续数字区间长度
                int leftLen = l ? l : 0, rightLen = r ? r : 0;
                int curLen = leftLen + rightLen + 1; //当前这一点对应的连续数字区间长度
                maxLen = max(maxLen, curLen);
                ranges[v] = ranges[v - leftLen] = ranges[v + rightLen] = curLen; //更新v以及这一连续数字区间的左右端点对应的区间长度
            }
        }
        return maxLen;
    }
};
```
效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了79.65% 的用户
内存消耗：11.1 MB, 在所有 C++ 提交中击败了10.12% 的用户
```
以后更新并查集的写法。
