Given an array `nums` with `n` objects colored red, white, or blue, sort them **in-place** so that objects of the same color are adjacent, with the colors in the order red, white, and blue.

Here, we will use the integers `0`, `1`, and `2` to represent the color red, white, and blue respectively.

**Follow up:**
- Could you solve this problem without using the library's sort function?
- Could you come up with a one-pass algorithm using only `O(1)` constant space?

 

**Example 1:**

```swift
Input: nums = [2,0,2,1,1,0]
Output: [0,0,1,1,2,2]
```

**Example 2:**

```swift
Input: nums = [2,0,1]
Output: [0,1,2]
```

**Example 3:**

```swift
Input: nums = [0]
Output: [0]
```

**Example 4:**

```swift
Input: nums = [1]
Output: [1]
```
**Constraints:**
- `n == nums.length` 
 - `1 <= n <= 300`
 - `nums[i]` is `0` , `1` , or `2` .

题意：给定一个包含红色 `0` 、白色 `1` 和蓝色 `2` ，一共 `n` 个元素的数组，原地对它们进行排序，使得相同颜色的元素相邻，并按照红色、白色、蓝色顺序排列。
 

---
### 解法1 SORT
```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) { 
        sort(nums.begin(), nums.end()); 
    }
};
```
时间复杂度为 $O(n\log n)$ ：
```cpp
执行用时 :4 ms, 在所有 C++ 提交中击败了78.45% 的用户
内存消耗 :10.7 MB, 在所有 C++ 提交中击败了5.83%的用户 
```
---
### 解法二 计数排序
```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {  
        int freq[3] = {0}, len = 0;
        for (const auto i : nums) ++freq[i];
        for (int i = 0; i < 3; ++i) 
           for (int j = 0; j < freq[i]; ++j) 
               nums[len++] = i;
    }
};
```
时间和复杂度都为 $O(n)$ ：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.3 MB, 在所有 C++ 提交中击败了24.65% 的用户
```
---
### 解法三 三路快排
有没有可能原地进行修改，不使用额外空间呢？可以使用快排，一次对数组以 `2` 为中心划分，第二次对数组以 `1` 为中心划分。但是这需要对数组进行两次扫描。最优解是对数组进行一次扫描（2021王道数据结构，P324第7题荷兰国旗问题）。

一种做法是三路快排，使用两个指针 `zero, two` ，需要使得 `nums[0:zero]` 为零，`nums[two:n-1]` 为二，中间部分为一。对数组 `nums[]` 从前往后扫描：
- 如果 `nums[i] == 1` ，越过；
- 如果 `nums[i] == 2` ，就和 `nums[--two]` 交换；
- 如果 `nums[i] == 0` ，就和 `nums[++zero]` 交换，换回来的一定是 `nums[i] == 0 or 1` ，于是 `++i` 。
```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {  
    	int zero = -1, two = nums.size();
    	for (int i = 0; i < two; ) {
    		if (nums[i] == 1) ++i;
    		else if (nums[i] == 2) swap(nums[i], nums[--two]);
    		else if (nums[i] == 0) {
    			swap(nums[i], nums[++zero];
    			++i;
    		}
    	}
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：8.1 MB, 在所有 C++ 提交中击败了69.21% 的用户
```
另一种比较优雅的实现方法如下：
```cpp
class Solution {
public:
    void sortColors(vector<int>& nums) {  
        int zero = -1, one = -1, two = -1;
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] == 2) ++two;
            else if (nums[i] == 1) {
                nums[++two] = 2;
                nums[++one] = 1;
            } else if (nums[i] == 0) {
                nums[++two] = 2;
                nums[++one] = 1;
                nums[++zero] = 0;
            }
        }
    }
};
```
