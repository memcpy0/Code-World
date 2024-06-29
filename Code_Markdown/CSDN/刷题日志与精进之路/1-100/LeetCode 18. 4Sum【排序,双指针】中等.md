> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091345814.png)

给你一个由 `n` 个整数组成的数组 `nums` ，和一个目标值 `target` 。请你找出并返回满足下述全部条件且**不重复**的四元组 `[nums[a], nums[b], nums[c], nums[d]]` （若两个四元组元素一一对应，则认为两个四元组重复）：
- `0 <= a, b, c, d < n`
- `a`、`b`、`c` 和 `d` **互不相同**
- `nums[a] + nums[b] + nums[c] + nums[d] == target`

你可以按 **任意顺序** 返回答案 。

**示例 1：**
```java
输入：nums = [1,0,-1,0,-2,2], target = 0
输出：[[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
```
**示例 2：**
```java
输入：nums = [2,2,2,2,2], target = 8
输出：[[2,2,2,2]]
```
**提示：**
- `0 <= nums.length <= 200`
- <code>-10<sup>9</sup> <= nums[i] <= 10<sup>9</sup></code>
- <code>-10<sup>9</sup> <= target <= 10<sup>9</sup></code>
 
如果这就是面试题，我们必须思考：
- 数组索引从 `0` 还是从 `1` 开始；
- 没有解的时候怎么办，返回异常吗；
- 是否有唯一解；
- 有多个解的时候，返回哪一个解；
- 是否能够使用同一个元素两次；
- ……

本题的题目和示例给出了解答：可能没有解，返回空向量，也可能有一个或多个解；**不能返回重复的四元组**；**同一个元素不能使用多次**。

---
本题与「[15. 三数之和](https://leetcode-cn.com/problems/3sum/)」相似，解法也相似。
### 解法 排序+双指针+去重优化
最朴素的方法是使用四重循环枚举所有的四元组，然后使用哈希表进行去重操作，得到不包含重复四元组的最终答案。假设数组的长度是 nnn，则该方法中，枚举的时间复杂度为 O(n4)O(n^4)O(n 
4
 )，去重操作的时间复杂度和空间复杂度也很高，因此需要换一种思路。

为了避免枚举到重复四元组，则需要保证每一重循环枚举到的元素不小于其上一重循环枚举到的元素，且在同一重循环中不能多次枚举到相同的元素。

为了实现上述要求，可以对数组进行排序，并且在循环过程中遵循以下两点：

每一种循环枚举到的下标必须大于上一重循环枚举到的下标；

同一重循环中，如果当前元素与上一个元素相同，则跳过当前元素。

使用上述方法，可以避免枚举到重复四元组，但是由于仍使用四重循环，时间复杂度仍是 O(n4)O(n^4)O(n 
4
 )。注意到数组已经被排序，因此可以使用双指针的方法去掉一重循环。

使用两重循环分别枚举前两个数，然后在两重循环枚举到的数之后使用双指针枚举剩下的两个数。假设两重循环枚举到的前两个数分别位于下标 iii 和 jjj，其中 i<ji<ji<j。初始时，左右指针分别指向下标 j+1j+1j+1 和下标 n−1n-1n−1。每次计算四个数的和，并进行如下操作：

如果和等于 target\textit{target}target，则将枚举到的四个数加到答案中，然后将左指针右移直到遇到不同的数，将右指针左移直到遇到不同的数；

如果和小于 target\textit{target}target，则将左指针右移一位；

如果和大于 target\textit{target}target，则将右指针左移一位。

使用双指针枚举剩下的两个数的时间复杂度是 O(n)O(n)O(n)，因此总时间复杂度是 O(n3)O(n^3)O(n 
3
 )，低于 O(n4)O(n^4)O(n 
4
 )。

具体实现时，还可以进行一些剪枝操作：

在确定第一个数之后，如果 nums[i]+nums[i+1]+nums[i+2]+nums[i+3]>target\textit{nums}[i]+\textit{nums}[i+1]+\textit{nums}[i+2]+\textit{nums}[i+3]>\textit{target}nums[i]+nums[i+1]+nums[i+2]+nums[i+3]>target，说明此时剩下的三个数无论取什么值，四数之和一定大于 target\textit{target}target，因此退出第一重循环；
在确定第一个数之后，如果 nums[i]+nums[n−3]+nums[n−2]+nums[n−1]<target\textit{nums}[i]+\textit{nums}[n-3]+\textit{nums}[n-2]+\textit{nums}[n-1]<\textit{target}nums[i]+nums[n−3]+nums[n−2]+nums[n−1]<target，说明此时剩下的三个数无论取什么值，四数之和一定小于 target\textit{target}target，因此第一重循环直接进入下一轮，枚举 nums[i+1]\textit{nums}[i+1]nums[i+1]；
在确定前两个数之后，如果 nums[i]+nums[j]+nums[j+1]+nums[j+2]>target\textit{nums}[i]+\textit{nums}[j]+\textit{nums}[j+1]+\textit{nums}[j+2]>\textit{target}nums[i]+nums[j]+nums[j+1]+nums[j+2]>target，说明此时剩下的两个数无论取什么值，四数之和一定大于 target\textit{target}target，因此退出第二重循环；
在确定前两个数之后，如果 nums[i]+nums[j]+nums[n−2]+nums[n−1]<target\textit{nums}[i]+\textit{nums}[j]+\textit{nums}[n-2]+\textit{nums}[n-1]<\textit{target}nums[i]+nums[j]+nums[n−2]+nums[n−1]<target，说明此时剩下的两个数无论取什么值，四数之和一定小于 target\textit{target}target，因此第二重循环直接进入下一轮，枚举 nums[j+1]\textit{nums}[j+1]nums[j+1]。

通过固定一个数 `nums[i]` ，可以将四元组的问题转换为三元组，使用前面的代码。同样，为了避免使用集合去重，做法是：**使用连续重复元素的第一个元素，然后在后续区间内进行双指针；得到某一个解后，跳过已经使用的解元素；双指针移动时跳过重复的元素**。这种做法，既顾及到了可能出现重复元素的四元组 ，也考虑到了重复使用同一元素的问题，而且避免了多次得到重复四元组。

还用到了更多的优化：
- 不符合条件时，在 `lo` 或者 `hi` 移动时跳过重复元素；
- 如果当前的固定数和后续数组最小的几个数相加，结果大于零，说明后面已经没有结果，直接退出；
- 如果当前的固定数和后续数组最大的几个数相加，结果小于零，跳过此次双指针搜索。

```cpp
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        if (nums.size() < 4) return {};
        vector<vector<int>> ans;
        sort(nums.begin(), nums.end());   //首先排序  
        const int n = nums.size();
        for(int i = 0; i < n - 3; ++i) { //只遍历到倒数第四个数为止
            if (i > 0 && nums[i] == nums[i - 1]) continue; //去重优化,使用连续重复元素的第一个
            if (nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target) break; //后续没有解
            if (nums[i] + nums[n - 3] + nums[n - 2] + nums[n - 1] < target) continue; //本次无解
            
            int newTarget = target - nums[i];   // 将四数之和转化为三数之和            
            for (int j = i + 1; j < n - 2; ++j) { //只遍历到倒数第三个数为止
                if (j > i + 1 && nums[j] == nums[j - 1]) continue; //去重优化,使用连续重复元素的第一个
                if (nums[j] + nums[j + 1] + nums[j + 2] > newTarget) break; //后续没有解
                if (nums[j] + nums[n - 2] + nums[n - 1] < newTarget) continue; //本次无解
                
                int newTarget2 = newTarget - nums[j], lo = j + 1, hi = n - 1;   //三数之和变成两数之和     
                while (lo < hi) { //两数之和直接套用代码
                    if (nums[lo] + nums[hi] == newTarget2) {
                        ans.push_back({nums[i], nums[j], nums[lo], nums[hi]});
                        while (lo < hi && nums[lo] == nums[lo + 1]) ++lo; //注意去重
                        while (lo < hi && nums[hi] == nums[hi - 1]) --hi;
                        ++lo, --hi;
                    } else if (nums[lo] + nums[hi] < newTarget2) {
                        while (lo < hi && nums[lo] == nums[lo + 1]) ++lo;
                        ++lo;
                    } else {
                        while (lo < hi && nums[hi] == nums[hi - 1]) --hi;
                        --hi;
                    }
                }
            }
        }
        return ans;
    }
};
```
效率是可喜的：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了92.45% 的用户
内存消耗：12.8 MB, 在所有 C++ 提交中击败了5.02% 的用户
```

```cpp
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        vector<vector<int>> ans;
        if (nums.size() < 4) return ans;
        sort(nums.begin(), nums.end());
        int n = nums.size();
        for (int i = 0; i < n - 3; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            if ((long long)nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target) break;
            if ((long long)nums[i] + nums[n - 3] + nums[n - 2] + nums[n - 1] < target) continue;
            for (int j = i + 1; j < n - 2; ++j) {
                if (j > i + 1 && nums[j] == nums[j - 1]) continue;
                if ((long long)nums[i] + nums[j] + nums[j + 1] + nums[j + 2] > target) break;
                if ((long long)nums[i] + nums[j] + nums[n - 2] + nums[n - 1] < target) continue;
                int l = j + 1, r = n - 1;
                while (l < r) {
                    long long sum = (long long) nums[i] + nums[j] + nums[l] + nums[r];
                    if (sum == target) {
                        ans.push_back({nums[i], nums[j], nums[l], nums[r]});
                        while (l < r && nums[l] == nums[l + 1]) ++l;
                        ++l;
                        while (l < r && nums[r] == nums[r - 1]) --r;
                        --r;
                    } else if (sum < target) ++l;
                    else --r;
                }
            }
        }
        return ans;
    }
};
```
复杂度分析

时间复杂度：O(n3)O(n^3)O(n 
3
 )，其中 nnn 是数组的长度。排序的时间复杂度是 O(nlog⁡n)O(n \log n)O(nlogn)，枚举四元组的时间复杂度是 O(n3)O(n^3)O(n 
3
 )，因此总时间复杂度为 O(n3+nlog⁡n)=O(n3)O(n^3+n\log n)=O(n^3)O(n 
3
 +nlogn)=O(n 
3
 )。

空间复杂度：O(log⁡n)O(\log n)O(logn)，其中 nnn 是数组的长度。空间复杂度主要取决于排序额外使用的空间。此外排序修改了输入数组 nums\textit{nums}nums，实际情况中不一定允许，因此也可以看成使用了一个额外的数组存储了数组 nums\textit{nums}nums 的副本并排序，空间复杂度为 O(n)O(n)O(n)。
 