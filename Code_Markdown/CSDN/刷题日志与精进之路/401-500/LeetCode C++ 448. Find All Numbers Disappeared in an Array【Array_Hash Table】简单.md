Given an array of integers where 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once.

Find all the elements of [1, n] inclusive that **do not appea**r in this array.

Could you do it **without extra space and in O(n) runtime**? You may assume the returned list does not count as extra space.

**Example:**
```css
Input:
[4,3,2,7,8,2,3,1]

Output:
[5,6]
```
题意：给定一个数据范围为 `[1, n]` (n为数组大小)的整型数组，数组中的元素一些出现了两次，另一些只出现一次。找到所有在 `[1, n]` 范围之间但却没有出现在数组中的整数。 

---
### 解法1 额外空间
如果使用额外空间，这道题就很简单：
```cpp
class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) { //哈希表法
        int n = nums.size();
        bool *arr = new bool[n + 1]{false};
        for (const auto& i : nums) arr[i] = true; 
        vector<int> ans;
        for (int i = 1; i <= n; ++i) if (!arr[i]) ans.push_back(i);
        return ans;
    } 
};
```
运行效率如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了97.36% 的用户
内存消耗：31.5 MB, 在所有 C++ 提交中击败了67.30% 的用户
```
---
### 解法2 交换法
如果不使用额外空间，同时满足 $O(n)$ 的运行时间，就需要一点小技巧。通常情况下，我们使用**原地修改原始数组**（存储某些关键信息）这种方法做到这一点：
```cpp
class Solution {
public: 
    /** 
    * 找出 1 - n 中没有出现的数字。不能使用额外的空间，两次循环时间复杂度为 2O(n)，即为 O(n)。 
    * 解题思路：交换法，如果这个数字没有出现在应该出现的位置，则将其交换到应该出现的位置；否则继续扫描
    *
    * [4,3,2,7,8,2,3,1] 初始数据
    *
    * [7,3,2,4,8,2,3,1] 第一个数据4出现在下标为0的地方，应该出现在下标为4-1=3的位置，交换对应的两个元素
    * [3,3,2,4,8,2,7,1] 第一个数据7同样需要交换
    * [2,3,3,4,8,2,7,1] 第一个数据3同样需要交换
    * [3,2,3,4,8,2,7,1] 第一个数据2同样需要交换
    * [3,2,3,4,8,2,7,1] 第一个数据3已经出现在了应该出现的位置，不交换；继续扫描, i指向4
    * [3,2,3,4,1,2,7,8] 第五个数据8同样需要交换
    * [1,2,3,4,3,2,7,8] 第五个数据1同样需要交换
    * [1,2,3,4,3,2,7,8] 第五个数据3已经出现在了应该出现的位置，不交换；继续扫描, i=5
    * [1,2,3,4,3,2,7,8] 第六个数据2已经出现在了应该出现的位置，不交换；继续扫描, 结束
    * 
    * 计算结束，数组的第五个、第六个元素不符合对应位置，证明 5,6 没有出现
    */
    vector<int> findDisappearedNumbers(vector<int>& nums) {  
        vector<int> ans; 
        int n = nums.size();
        for (int i = 0; i < n; ++i) 
            while (nums[nums[i] - 1] != nums[i]) // 要交换的目标值不等于现在指向的值  
                swap(nums[i], nums[nums[i] - 1]); 
        for (int i = 0; i < n; ++i) 
            if (i + 1 != nums[i]) 
                ans.push_back(i + 1);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了97.36% 的用户
内存消耗：31.4 MB, 在所有 C++ 提交中击败了72.92% 的用户
```

---
### 解法3 负数标记法
另一种原地修改原始数组的方法如下，简单将对应位置的元素(是正数则)取负：
```cpp
class Solution {
public: 
    /* 
     * 找出 1 - n 中没有出现的数字。不能使用额外的空间，两次循环时间复杂度为 2O(n)，即为 O(n)。  
     * 解题思路：使用数组的下标来标记数字的出现于否，通过一遍遍历即可标记出全部已经出现的数字
     *
     * [4,3,2,7,8,2,3,1] 初始数据
     * 第一个数据4出现，将数组的第四个也就是下标3的数据修改为负数。-7计算时，通过绝对值处理一下即可，不影响后续的计算
     * [4,3,2,-7,8,2,3,1] 
     * [4,3,-2,-7,8,2,3,1] 其他同理
     * [4,-3,-2,-7,8,2,3,1]
     * [4,-3,-2,-7,8,2,-3,1]
     * [4,-3,-2,-7,8,2,-3,-1]
     * [4,-3,-2,-7,8,2,-3,-1]
     * [4,-3,-2,-7,8,2,-3,-1]
     * [-4,-3,-2,-7,8,2,-3,-1]
     *
     * 计算结束，数组的第五个，第六个依然为正数，证明 5,6 没有出现
     */
    vector<int> findDisappearedNumbers(vector<int>& nums) {  
        vector<int> ans; 
        int n = nums.size();
        for (int i = 0; i < nums.size(); ++i) { //出现过的元素其对应的数组位置的值为负
            int j = abs(nums[i]) - 1;
            if (nums[j] > 0) nums[j] = -nums[j];
        } 
        for (int i = 0; i < nums.size(); ++i) 
            if (nums[i] > 0) 
                ans.push_back(i + 1);
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了87.74% 的用户
内存消耗：31.3 MB, 在所有 C++ 提交中击败了87.33% 的用户
```

