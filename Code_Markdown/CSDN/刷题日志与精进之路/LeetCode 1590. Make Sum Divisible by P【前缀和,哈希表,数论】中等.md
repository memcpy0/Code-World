> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given an array of positive integers `nums` , remove the **smallest** subarray (possibly **empty**) such that the **sum** of the remaining elements is divisible by `p` . It is **not** allowed to remove the whole array.

Return *the length of the smallest subarray that you need to remove, or -1 if it's impossible.*

A **subarray** is defined as a contiguous block of elements in the array.

**Example 1:**
```cpp
Input: nums = [3,1,4,2], p = 6
Output: 1
Explanation: The sum of the elements in nums is 10, which is not divisible by 6. We can remove the subarray [4], and the sum of the remaining elements is 6, which is divisible by 6.
```
**Example 2:**
```cpp
Input: nums = [6,3,5,2], p = 9
Output: 2
Explanation: We cannot remove a single element to get a sum divisible by 9. The best way is to remove the subarray [5,2], leaving us with [6,3] with sum 9.
```
**Example 3:**
```cpp
Input: nums = [1,2,3], p = 3
Output: 0
Explanation: Here the sum is 6. which is already divisible by 3. Thus we do not need to remove anything.
```
**Example 4:**
```cpp
Input: nums = [1,2,3], p = 7
Output: -1
Explanation: There is no way to remove a subarray in order to get a sum divisible by 7.
```
**Example 5:**
```cpp
Input: nums = [1000000000,1000000000,1000000000], p = 3
Output: 0
```
**Constraints:**
- <code>1 <= nums.length <= 10<sup>5</sup></code>
- <code>1 <= nums[i] <= 10<sup>9</sup></code>
 - <code>1 <= p <= 10<sup>9</sup></code>

题意：给出一个正整数数组 `nums` ，移除 **最短子数组**（可以为空），使得剩余元素的和能被 `p` 整除。 不允许将整个数组都移除。返回这个最短子数组的长度。

---
### 解法 前缀和+同余
思路：这一题和模运算有关系，可以使用前缀和进行算法优化。整个算法的步骤如下：
- 计算整个数组的和 ${sum}$ ，设 $x = sum \bmod p$ ，如果 $x$ 为 $0$ ，就不需要移出任何子数组，直接返回 $0$ ；
- ==如果我们要移除某个子数组，使剩余元素的和能被 $p$ 整除，或者说剩余元素的和是 $p$ 的整数倍==（倍数设为 $k$ ）——根据同余的定义可知，**子数组的和 $subsum$ 应与整个数组的和 $sum$ 模 $p$ 同余**，剩余元素的和模 $p$ 才为零，即：$$subsum \equiv sum\bmod{p}$$或者由模运算对减法的封闭性：$$(sum \bmod {p - subsum} \bmod p) \bmod p =(sum - subsum) \bmod p = 0$$
可知，$sum$ 和 $subsum$ 模 $p$ 的余数需要相等。于是，问题转换为： 怎样判断有没有这样的子数组，**如何找到模 $p$ 的余数等于 $x$ 的子数组**？似乎做过类似的题目：523. Continuous Subarray Sum ——只不过那道题是寻找有没有模 $k$ 的余数为 $0$ 的子数组。

拓展一下，假设现在手中有一段前缀和 $sum[j]$ ，它模 $p$ 的余数为 $q$ ，要寻找有没有「**模 $p$ 的余数等于 $x$ 的子数组 $arr[i,j]$** 」，就只需要判断是否存在一个前缀和 $sum[i-1]$ ，它模 $p$ 的值等于 $(q - x)\ \bmod\ p$ 。
$$(sum[j] - sum[i-1])\ \%\ p = (sum[j]\ \%\  p - sum[i - 1]\ \%\  p)\ \%\  p\\
=(q - sum[i-1]\ \%\ p)\ \%\  p = x$$

于是，我们只需要用映射，在其中寻找有没有前缀和模 $p$ 的余数，等于当前的前缀和 $sum[j]$ 模 $p$ 的余数 $q$ 减去 $x$ ：
- 如果没有找到，就记录当前位置前缀和 $\%\ p$ 的值及其最右位置；
- 如果找到，且长度更短，就更新最短的子数组长度。

以 `nums = [3,1,4,2], p = 6` 为例：
```cpp
前缀和 % p 的序列 = [3,4,2,4]
整个数组的和 %6 等于 4
--------------------------------------------------------------------------
sum[1] = 4, sum[1] % 6 = 4 时, 要判断有没有某个子数组和模 6 等于 4
只需要判断有没有某个前缀和模 6 等于 0.
最初的前缀和等于 0, 所以此时移除子数组 nums[0:2) , 长度为2.
--------------------------------------------------------------------------
sum[2] = 8, sum[2] % 6 = 2 时, 要判断有没有某个子数组和模 6 等于 4
只需要判断有没有某个前缀和模 6 等于 (2 - 4) % 6 = 4
可知sum[1] = 3 + 1 = 4, sum[1] % 6 = 4, 于是移除子数组 nums[2], 长度为1, 符合题意 
```
整个代码如下：
```cpp
class Solution {
public:
    int minSubarray(vector<int>& nums, int p) {
        int sum = 0, n = nums.size();
        for (int i = 0; i < n; ++i) sum = (sum + nums[i]) % p;  
        if (sum % p == 0) return 0; //整个正整数数组的和已经可以被p整除

        int ansLen = -1, now = 0; //最短子数组的长度
        unordered_map<int, int> record;
        record[0] = 0;
        //如果sum % p == x, 那么移除的子数组之和t应该满足t % p == x
        //(sum % p - t % p) % p = 0 ==> sum % p = t % p
        for (int i = 0; i < n; ++i) {
            now = (now + nums[i]) % p; //此处前缀和 % p == now
            //如果有一个前缀和 %p == (now - x) % p, 那么存在子数组 %p == x
            int last = (now - sum + p) % p; //+p,防止now-sum为负数,出现取余和取模的不一致
            unordered_map<int, int>::iterator it = record.find(last);
            if (it != record.end()) 
                if (ansLen == -1 || i + 1 - it->second < ansLen) 
                    ansLen = i + 1 - it->second; //更新最短子数组的长度
            record[now] = i + 1; //记录前缀和和这一前缀和的数组长度
        }
        if (ansLen == n) return -1; //不可以移除整个数组
        return ansLen;
    }
}; 
```
提交结果如下：
```cpp
执行用时：420 ms, 在所有 C++ 提交中击败了70.96% 的用户
内存消耗：65.2 MB, 在所有 C++ 提交中击败了91.90% 的用户
```
