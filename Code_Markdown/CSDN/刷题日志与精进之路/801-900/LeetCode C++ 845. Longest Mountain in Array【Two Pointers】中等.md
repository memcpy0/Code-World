Let's call any (contiguous) subarray B (of A) a *mountain* if the following properties hold:
- `B.length >= 3`
- There exists some `0 < i < B.length - 1` such that `B[0] < B[1] < ... B[i-1] < B[i] > B[i+1] > ... > B[B.length - 1]`

(Note that `B` could be any subarray of `A` , including the entire array `A` .)

Given an array `A` of integers, return the length of the longest *mountain*. 

Return 0 if there is no mountain.

**Example 1:**

```swift
Input: [2,1,4,7,3,2,5]
Output: 5
Explanation: The largest mountain is [1,4,7,3,2] which has length 5.
```

**Example 2:**

```swift
Input: [2,2,2]
Output: 0
Explanation: There is no mountain.
```

**Note:**
- `0 <= A.length <= 10000`
 - `0 <= A[i] <= 10000`

**Follow up:**
- Can you solve it using only one pass?
- Can you solve it in `O(1)` space?


题意：把数组 `A` 中符合下列属性的任意连续子数组 `B` 称为山脉：
- `B.length >= 3`
- 存在 `0 < i < B.length - 1` 使得 `B[0] < B[1] < ... B[i-1] < B[i] > B[i+1] > ... > B[B.length - 1]`
- `B` 可以是 `A` 的任意子数组，包括整个数组 `A` 

给出一个整数数组 `A` ，返回最长山脉的长度。如果不含有山脉则返回 `0` 。 


---
### 解法1 额外空间
使用额外空间和前缀和，得到每个元素的最长上升子数组长度、最长下降子数组长度，然后合并两个数组，得到山脉的长度。以 `Input = [2, 1, 4, 7, 3, 2, 5]` 为例：
- 上升的前缀和：`[1, 1, 2, 3, 1, 1, 2]` 
- 下降的后缀和：`[2, 1, 1, 3, 2, 1, 1] ` 
```cpp
class Solution {
public:
    int longestMountain(vector<int>& A) {
        int n = A.size();
        vector<int> up(n, 1); //分别找每个元素的最长上升子数组
        vector<int> down(n, 1); //和最长下降子数组
        for (int i = 1; i < n; ++i) 
            if (A[i] > A[i - 1]) up[i] = up[i - 1] + 1;
        for (int i = n - 2; i >= 0; --i) 
            if (A[i] > A[i + 1]) down[i] = down[i + 1] + 1;
        int res = 0;
        for (int i = 0; i < n; ++i) {
            if (up[i] <= 1 || down[i] <= 1) continue;
            res = max(res, up[i] + down[i] - 1);
        }  
        return res;
    }
};
```
效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了29.55% 的用户
内存消耗：19.5 MB, 在所有 C++ 提交中击败了5.12% 的用户
```
