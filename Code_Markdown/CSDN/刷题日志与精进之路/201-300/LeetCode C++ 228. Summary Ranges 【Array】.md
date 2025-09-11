Given a sorted integer array without duplicates, return the summary of its ranges.

Example 1:
```
Input:  [0,1,2,4,5,7]
Output: ["0->2","4->5","7"]
Explanation: 0,1,2 form a continuous range; 4,5 form a continuous range.
```
Example 2:
```
Input:  [0,2,3,4,6,8,9]
Output: ["0","2->4","6","8->9"]
Explanation: 2,3,4 form a continuous range; 8,9 form a continuous range.
```
题意：给出一个排序的数组，数组中没有重复元素，返回数据范围的概括。[0, 1, 2, 4, 5, 7]是0-2，4-5，7三个范围。对于包含连续元素的一段区间。如果**相邻的元素之间的差值大于1**，那么这两个元素肯定不属于同一段区间。因为数组没有重复元素，因此差值不会为0，只会为1和大于1。

方法：双指针，一个指向连续数据范围的开头，一个指向结尾。不断移动即可。当然，双指针的写法有多种，但都不要忘记把最后一段区间也放进结果里面，可以在循环里通过一个特定的判断条件来加入或者在循环结束后加入。

执行用时 :0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗 :8.6 MB, 在所有 C++ 提交中击败了5.00%的用户
```c
class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> vi;
        if (nums.empty()) return vi;
        int b = 0, e = 0; // 双指针  
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == nums[i - 1] + 1) e = i; // 整数仍在连续变化
            else {  
                if (b == e) vi.push_back(to_string(nums[b])); // 范围内只有一个数
                else vi.push_back(to_string(nums[b]) + "->" + to_string(nums[e])); // 范围中有多个数
                b = e = i; // 移动双指针
            }
        } 
        if (b == e) vi.push_back(to_string(nums[b]));  // 最后的一个范围
        else vi.push_back(to_string(nums[b]) + "->" + to_string(nums[e])); 
        return vi;
    }
};
```

