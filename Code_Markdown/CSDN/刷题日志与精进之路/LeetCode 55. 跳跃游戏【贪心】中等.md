> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091742103.png)

Given an array of non-negative integers, you are initially positioned at the first index of the array.

Each element in the array represents your maximum jump length at that position.

Determine if you are able to reach the last index.

 

**Example 1:**

```swift
Input: nums = [2,3,1,1,4]
Output: true
Explanation: Jump 1 step from index 0 to 1, then 3 steps to the last index.
```

**Example 2:**

```swift
Input: nums = [3,2,1,0,4]
Output: false
Explanation: You will always arrive at index 3 no matter what. Its maximum jump length is 0, which makes it impossible to reach the last index.
```

 

**Constraints:**
- `1 <= nums.length <= 3 * 10^4`
 - `0 <= nums[i][j] <= 10^5`

题意：给出一个非负整数数组，最开始处于数组第一个位置，数组元素表示在该位置可以跳跃的最大长度。判断是否能够到达最后一个位置。


---
### 解法 从后往前
从后往前扫描数组，如果遇到零（表示到达这个位置后，无法跳跃），就继续往前查看数组，看是否存在某个位置可以跳过这个零，不存在这样的位置就返回 `false` ；存在则继续这一过程。显然，如果不存在零元素，就必然能够到达最后一个位置。

具体代码实现如下：
```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        if (nums.size() <= 1) return true;
        for (int i = nums.size() - 2; i >= 0; --i) {
            if (nums[i] != 0) continue; 
            bool canJumpOverZero = false;
            for (int j = i - 1; j >= 0; --j) {
                if (j + nums[j] > i) {  
                    canJumpOverZero = true;
                    i = j; //不再扫描中间这一段
                    break;
                }
            }
            if (canJumpOverZero == false) return false;
        }
        return true; //全都不为0;或者有零但是可以越过-->可以到达;
    }
};
```
效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了65.15% 的用户
内存消耗：13 MB, 在所有 C++ 提交中击败了8.43% 的用户
```
