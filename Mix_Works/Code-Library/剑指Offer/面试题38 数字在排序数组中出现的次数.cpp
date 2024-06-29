#include <bits/stdc++.h>
using namespace std;
class Solution {
public:
    int lower(const vector<int> &nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (nums[mid] >= target)
                hi = mid;
            else 
                lo = mid + 1;
        }
        return lo;
    }
    int upper(const vector<int> &nums, int target) {
        int lo = 0, hi = nums.size();
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (nums[mid] > target)
                hi = mid;
            else 
                lo = mid + 1;
        }
        return lo;
    }
    int search(vector<int>& nums, int target) {
        int left = lower(nums, target), right = upper(nums, target);
        return right - left;
    }
};
/*
如何用二分查找算法在数组中找到第一个k。二分查找算法总是用数组中间的数字和k比较
如果中间的数字比k大，那么k只可能出现在数组的前半段，下一轮在数组的前半段查找就可以了。

如果中间的数字比k小，那么k只可能出现在数组的后半段，下一轮我们只在数组的后半段查找就可以了。

如果中间的数字和k相等呢？
- 先判断它是否是第一个k:
	- 如果前面一个数字不是k，则中间的数字刚好是第一个k;
	- 如果前面一个数字是k，则第一个k肯定是在数组的前半段，下一轮仍然在前半段查找
	
递归代码找到排序数组中的第一个k的位置 
在函数GetFirstK中，如果数组不包含数字k，则返回-1；包含至少一个k，则返回第一个k在数组中的下标 
*/ 

int GetFirstK(int *data, int length, int k, int start, int end) {
	if (start > end) return -1;
	int middleIndex = (start + end) / 2;
	int middleData = data[middleIndex];
	if (middleData == k) {
		if ((middleIndex > 0 && data[middleIndex - 1] != k) || middleIndex == 0)
			return middleIndex;
		else 
			end = middleIndex - 1;
	} 
	else if (middleData > k) 
		end = middleIndex - 1;
	else 
		start = middleIndex + 1;
	return GetFirstK(data, length, k, start, end);
}
/*
如何用二分查找算法在数组中找到最后一个k。二分查找算法总是用数组中间的数字和k比较
如果中间的数字比k大，那么k只可能出现在数组的前半段，下一轮在数组的前半段查找就可以了。

如果中间的数字比k小，那么k只可能出现在数组的后半段，下一轮我们只在数组的后半段查找就可以了。

如果中间的数字和k相等呢？
- 先判断它是否是最后一个k:
	- 如果下一个数字不是k，则中间的数字刚好是最后一个k;
	- 如果下一个数字是k，则最后一个k肯定是在数组的后半段，下一轮仍然在后半段查找 

递归代码找到排序数组中的最后一个k的位置 
在函数GetLastK中，如果数组不包含数字k，则返回-1；包含至少一个k，则返回最后一个k在数组中的下标 
*/ 
int GetLastK(int *data, int length, int k, int start, int end) {
	if (start > end) return -1;	
	int middleIndex = (start + end) / 2;
	int middleData = data[middleIndex];
	
	if (middleData == k) {
		if ((middleIndex < length - 1 && data[middleIndex + 1] != k) || middleIndex == length - 1) 
			return middleIndex;
		else
			start = middleIndex - 1;	
	}
	else if (middleData < k) 
		start = middleIndex + 1;
	else
		end = middleIndex - 1;
	return GetLastK(data, length, k, start, end);
}
int GetNumberOfK(int *data, int length, int k) {
	int number = 0;
	if (data != nullptr && length > 0) {
		int first = GetFirstK(data, length, k, 0, length - 1);
		int last = GetLastK(data, length, k, 0, length - 1);
		if (first > -1 && last > -1)
			number = last - first + 1;
	}
	return number;
}
/*
功能测试：数组中包含查找的数字、没有查找的数字、查找的数字出现一次或者多次
边界值测试：查找数组的最大值、最小值，数组中只有一个数字
特殊输入测试：表示数组的指针为NULL指针 
*/
int main() { 
    return 0;
}
/*
LeetCode C++ 剑指 Offer 53 - I. 在排序数组中查找数字 I【二分】简单
统计一个数字在**排序数组**中出现的次数。

示例 1:
```cpp
输入: nums = [5,7,7,8,8,10], target = 8
输出: 2
```
示例 2:
```cpp
输入: nums = [5,7,7,8,8,10], target = 6
输出: 0
```
**限制：** `0 <= 数组长度 <= 50000`

 
---
思路1：使用哈希表计数，完全没有利用到题目的条件。代码如下：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        unordered_map<int, int> mp;
        for (const auto i : nums) ++mp[i];
        return mp[target];
    }
};
```
效率：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了89.16% 的用户
内存消耗：13.4 MB, 在所有 C++ 提交中击败了13.27% 的用户
```

---
思路2：顺序扫描，然后计数。代码如下：
```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int cnt = 0;
        for (const int i : nums) if (i == target) ++cnt;
        return cnt;
    }
};
```
效率如下，因为有条件判断，可能出现分支预测失败：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了64.38% 的用户
内存消耗：13.2 MB, 在所有 C++ 提交中击败了60.30% 的用户
```
---
思路3：二分找到最左边的值，然后顺序扫描到最右边的值。用 `lower_bound` 函数。代码如下：
```cpp

```
*/
