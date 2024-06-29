Given an unsorted array `nums` , reorder it such that `nums[0] < nums[1] > nums[2] < nums[3]...` . 

**Example 1:**

```swift
Input: nums = [1, 5, 1, 1, 6, 4]
Output: One possible answer is [1, 4, 1, 5, 1, 6].
```

**Example 2:**

```swift
Input: nums = [1, 3, 2, 2, 3, 1]
Output: One possible answer is [2, 3, 1, 3, 1, 2].
```

**Note:** You may assume all input has valid answer.

**Follow Up:** Can you do it in `O(n)` time and/or in-place with `O(1)` extra space?



题意：给定一个无序的数组 `nums` ，将它重新排列成 `nums[0] < nums[1] > nums[2] < nums[3]...` 的顺序。

---
### 解法1 排序+贪心
先复制原数组到 `temp[]` ，然后对临时数组升序排序。之后穿插回原数组中，用到贪心思想：偶数位置应为相对左右位置较小的数，因此对 `temp[0 : half]` （较小的一半）**从高到低**插回原数组；奇数位置应为相对于左右位置较大的数，因此对 `temp[half+1 : ]` （较大的一半）**从高到低**插回原数组。即使用较大的大数匹配较大的小数，较小的大数匹配较小的小数，而且**最关键的是从高往低插入，避免出现重叠**。

举一个例子就是 `[4, 5, 5, 6]` ，如果从高往低插入就是 `[5, 6, 4, 5]` ，正确；如果从低往高插入就是 `[4, 5, 5, 6]` ，错误。具体代码如下：
```cpp
class Solution {
public:
    void wiggleSort(vector<int>& nums) {
        vector<int> temp(nums.begin(), nums.end());
        sort(temp.begin(), temp.end());
        int idx = temp.size() - 1;
        for (int i = 1; i < temp.size(); i += 2) //从高到低分配大数 
            nums[i] = temp[idx--]; 
        for (int i = 0; i < temp.size(); i += 2) //从高到低分配小数
            nums[i] = temp[idx--];
    }
};
```
$O(n\log n)$ 的时间复杂度和 $O(n)$ 的空间复杂度，结果如下，比较低：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了25.41% 的用户
内存消耗：17.3 MB, 在所有 C++ 提交中击败了47.97% 的用户
```
