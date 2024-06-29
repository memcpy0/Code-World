> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091344129.png)

给你一个长度为 `n` 的整数数组 `nums` 和 一个目标值 `target`。请你从 `nums` 中选出三个整数，使它们的和与 `target` 最接近。返回这三个数的和。

假定每组输入只存在恰好一个解。

**示例 1：**
```java
输入：nums = [-1,2,1,-4], target = 1
输出：2
解释：与 target 最接近的和是 2 (-1 + 2 + 1 = 2) 。
```
**示例 2：**
```java
输入：nums = [0,0,0], target = 1
输出：0
```
**提示：**
- `3 <= nums.length <= 1000`
- `-1000 <= nums[i] <= 1000`
- `-10^4 <= target <= 10^4`

---

```cpp
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        int ans, minDiff = INT_MAX;
        for (int i = 0; i < n - 2; ++i) {
            int j = i + 1, k = n - 1, newTarget = target - nums[i];
            while (j < k) {
                int diff = newTarget - nums[j] - nums[k];
                if (abs(diff) < minDiff) {   
                    ans = nums[i] + nums[j] + nums[k];
                    minDiff = abs(diff);
                 }
                if (diff == 0) return target;
                if (diff < 0) --k;
                else ++j;
            }
        }
        return ans;
    }
};
```
本题与 [15. 三数之和](https://leetcode-cn.com/problems/3sum/) 非常类似，可以使用「双指针」的方法来解决。但基于题解的独立性，这里还是会从零开始讲解。

方法一：排序 + 双指针
思路与算法

题目要求找到与目标值 target\textit{target}target 最接近的三元组，这里的「最接近」即为差值的绝对值最小。我们可以考虑直接使用三重循环枚举三元组，找出与目标值最接近的作为答案，时间复杂度为 O(N3)O(N^3)O(N 
3
 )。然而本题的 NNN 最大为 100010001000，会超出时间限制。

那么如何进行优化呢？我们首先考虑枚举第一个元素 aaa，对于剩下的两个元素 bbb 和 ccc，我们希望它们的和最接近 target−a\textit{target} - atarget−a。对于 bbb 和 ccc，如果它们在原数组中枚举的范围（既包括下标的范围，也包括元素值的范围）没有任何规律可言，那么我们还是只能使用两重循环来枚举所有的可能情况。因此，我们可以考虑对整个数组进行升序排序，这样一来：

假设数组的长度为 nnn，我们先枚举 aaa，它在数组中的位置为 iii；

为了防止重复枚举，我们在位置 [i+1,n)[i+1, n)[i+1,n) 的范围内枚举 bbb 和 ccc。

当我们知道了 bbb 和 ccc 可以枚举的下标范围，并且知道这一范围对应的数组元素是有序（升序）的，那么我们是否可以对枚举的过程进行优化呢？

答案是可以的。借助双指针，我们就可以对枚举的过程进行优化。我们用 pbp_bp 
b
​
  和 pcp_cp 
c
​
  分别表示指向 bbb 和 ccc 的指针，初始时，pbp_bp 
b
​
  指向位置 i+1i+1i+1，即左边界；pcp_cp 
c
​
  指向位置 n−1n-1n−1，即右边界。在每一步枚举的过程中，我们用 a+b+ca+b+ca+b+c 来更新答案，并且：

如果 a+b+c≥targeta+b+c \geq \textit{target}a+b+c≥target，那么就将 pcp_cp 
c
​
  向左移动一个位置；

如果 a+b+c<targeta+b+c < \textit{target}a+b+c<target，那么就将 pbp_bp 
b
​
  向右移动一个位置。

这是为什么呢？我们对 a+b+c≥targeta+b+c \geq \textit{target}a+b+c≥target 的情况进行一个详细的分析：

如果 a+b+c≥targeta+b+c \geq \textit{target}a+b+c≥target，并且我们知道 pbp_bp 
b
​
  到 pcp_cp 
c
​
  这个范围内的所有数是按照升序排序的，那么如果 pcp_cp 
c
​
  不变而 pbp_bp 
b
​
  向右移动，那么 a+b+ca+b+ca+b+c 的值就会不断地增加，显然就不会成为最接近 target\textit{target}target 的值了。因此，我们可以知道在固定了 pcp_cp 
c
​
  的情况下，此时的 pbp_bp 
b
​
  就可以得到一个最接近 target\textit{target}target 的值，那么我们以后就不用再考虑 pcp_cp 
c
​
  了，就可以将 pcp_cp 
c
​
  向左移动一个位置。

同样地，在 a+b+c<targeta+b+c < \textit{target}a+b+c<target 时：

如果 a+b+c<targeta+b+c < \textit{target}a+b+c<target，并且我们知道 pbp_bp 
b
​
  到 pcp_cp 
c
​
  这个范围内的所有数是按照升序排序的，那么如果 pbp_bp 
b
​
  不变而 pcp_cp 
c
​
  向左移动，那么 a+b+ca+b+ca+b+c 的值就会不断地减小，显然就不会成为最接近 target\textit{target}target 的值了。因此，我们可以知道在固定了 pbp_bp 
b
​
  的情况下，此时的 pcp_cp 
c
​
  就可以得到一个最接近 target\textit{target}target 的值，那么我们以后就不用再考虑 pbp_bp 
b
​
  了，就可以将 pbp_bp 
b
​
  向右移动一个位置。

实际上，pbp_bp 
b
​
  和 pcp_cp 
c
​
  就表示了我们当前可以选择的数的范围，而每一次枚举的过程中，我们尝试边界上的两个元素，根据它们与 target\textit{target}target 的值的关系，选择「抛弃」左边界的元素还是右边界的元素，从而减少了枚举的范围。这种思路与 11. 盛最多水的容器 中的双指针解法也是类似的。

小优化

本题也有一些可以减少运行时间（但不会减少时间复杂度）的小优化。当我们枚举到恰好等于 target\textit{target}target 的 a+b+ca+b+ca+b+c 时，可以直接返回 target\textit{target}target 作为答案，因为不会有再比这个更接近的值了。

另一个优化与 15. 三数之和的官方题解 中提到的类似。当我们枚举 a,b,ca, b, ca,b,c 中任意元素并移动指针时，可以直接将其移动到下一个与这次枚举到的不相同的元素，减少枚举的次数。

class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        int best = 1e7;

        // 根据差值的绝对值来更新答案
        auto update = [&](int cur) {
            if (abs(cur - target) < abs(best - target)) {
                best = cur;
            }
        };

        // 枚举 a
        for (int i = 0; i < n; ++i) {
            // 保证和上一次枚举的元素不相等
            if (i > 0 && nums[i] == nums[i - 1]) {
                continue;
            }
            // 使用双指针枚举 b 和 c
            int j = i + 1, k = n - 1;
            while (j < k) {
                int sum = nums[i] + nums[j] + nums[k];
                // 如果和为 target 直接返回答案
                if (sum == target) {
                    return target;
                }
                update(sum);
                if (sum > target) {
                    // 如果和大于 target，移动 c 对应的指针
                    int k0 = k - 1;
                    // 移动到下一个不相等的元素
                    while (j < k0 && nums[k0] == nums[k]) {
                        --k0;
                    }
                    k = k0;
                } else {
                    // 如果和小于 target，移动 b 对应的指针
                    int j0 = j + 1;
                    // 移动到下一个不相等的元素
                    while (j0 < k && nums[j0] == nums[j]) {
                        ++j0;
                    }
                    j = j0;
                }
            }
        }
        return best;
    }
};
复杂度分析

时间复杂度：O(N2)O(N^2)O(N 
2
 )，其中 NNN 是数组 nums\textit{nums}nums 的长度。我们首先需要 O(Nlog⁡N)O(N \log N)O(NlogN) 的时间对数组进行排序，随后在枚举的过程中，使用一重循环 O(N)O(N)O(N) 枚举 aaa，双指针 O(N)O(N)O(N) 枚举 bbb 和 ccc，故一共是 O(N2)O(N^2)O(N 
2
 )。

空间复杂度：O(log⁡N)O(\log N)O(logN)。排序需要使用 O(log⁡N)O(\log N)O(logN) 的空间。然而我们修改了输入的数组 nums\textit{nums}nums，在实际情况下不一定允许，因此也可以看成使用了一个额外的数组存储了 nums\textit{nums}nums 的副本并进行排序，此时空间复杂度为 O(N)O(N)O(N)。
 