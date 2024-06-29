> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。
 
You are given an array `nums` of size `n` consisting of **distinct** integers from `1` to `n` and a positive integer `k`.

Return _the number of non-empty subarrays in_ `nums` _that have a **median** equal to_ `k`.

**Note**:
-   The median of an array is the **middle** element after sorting the array in **ascending** order. If the array is of even length, the median is the **left** middle element.
    -   For example, the median of `[2,3,1,4]` is `2`, and the median of `[8,4,3,5,1]` is `4`.
-   A subarray is a contiguous part of an array.

**Example 1:**
```java
Input: nums = [3,2,1,4,5], k = 4
Output: 3
Explanation: The subarrays that have a median equal to 4 are: [4], [4,5] and [1,4,5].
```
**Example 2:**
```java
Input: nums = [2,3,1], k = 3
Output: 1
Explanation: [3] is the only subarray that has a median equal to 3.
```
**Constraints:**
-   `n == nums.length`
-   `1 <= n <= 105`
-   `1 <= nums[i], k <= n`
-   The integers in `nums` are distinct.

题意：给出一个长度为 `n` 的数组 `nums` ，该数组由从 `1` 到 `n` 的 **不同** 整数组成。另给一个正整数 `k` 。统计并返回 `nums` 中的 **中位数** 等于 `k` 的非空子数组的数目。**注意**：
-   数组的中位数是按 **递增** 顺序排列后位于 **中间** 的那个元素，如果数组长度为偶数，则中位数是位于中间靠 **左** 的那个元素。例如，`[2,3,1,4]` 的中位数是 `2` ，`[8,4,3,5,1]` 的中位数是 `4` 。
-   子数组是数组中的一个连续部分。

---
**子数组统计问题**常用技巧：等价转换。
> 相似题目（等价转换）
> - [面试题 17.05. 字母与数字](https://leetcode.cn/problems/find-longest-subarray-lcci/)
> - [525. 连续数组](https://leetcode.cn/problems/contiguous-array/)
> - [1124. 表现良好的最长时间段](https://leetcode.cn/problems/longest-well-performing-interval/)

### 解法 哈希表+子数组统计
> 如果这道题要求的是「递增排序后中位数等于 $k$ 的子序列」的数目——统计数 $k$ 左右两边小于 $k$ 的数和大于 $k$ 的数，它们的数目各设为 $left, right$ 。由于 $k$ 是中位数，则左右两边的数应满足这一关系：$left + 1 \le right$ ……

先计算**子数组长为奇数**的情况。由于题目保证 $nums$ 中的整数互不相同、为 $[1, n]$ ，==「$k$ 是长为奇数的递增排序后子数组的中位数」**等价于**「子数组中小于 $k$ 的数的个数 $=$ 大于 $k$ 的数的个数」==，无论排序前后这一点都不会变。这相当于 $\Leftrightarrow$「左侧小于 $+$ 右侧小于 $=$ 左侧大于 $+$ 右侧大于」。变形得到 $\Leftrightarrow$「左侧小于 $-$ 左侧大于 $=$ 右侧大于 $-$ 右侧小于」。为了方便计算，把这四类数字等价转换：
-   左侧小于：在 $k$ 左侧且比 $k$ 小的视作1；
-   左侧大于：在 $k$ 左侧且比 $k$ 大的视作-1；
-   右侧大于：在 $k$ 右侧且比 $k$ 大的视作1；
-   右侧小于：在 $k$ 右侧且比 $k$ 小的视作-1。
- 此外，把 $k$ 视作0。

用示例1来说明。$[3,2,1,4,5]$ 可以视作 $[1,1,1,0,1]$ 。由于**子数组一定要包含中位数**，因此：
- 从下标 $3$ 开始**倒序枚举子数组左端点，计算「左侧小于 $-$ 左侧大于」的值 $x$** ，依次为 $0,1,2,3$ ，记到一个哈希表 $cnt$ 中
- 从下标 $3$ 开始**正序枚举子数组右端点，计算「右侧大于 $-$ 右侧小于」的值 $x$** ，依次为 $0,1$ 。
- ==**对每个右端点的 $x$ ，去看有多少个一样的左端点的 $x$**==。所以中位数为 $4$ 且长为奇数的子数组个数为 $cnt[0]+cnt[1]=1+1=2$ ，对应的子数组为 $[4]$ 和 $[1,4,5]$ 。

对于子数组长为偶数的情况，==「$k$ 是长为偶数的递增排序后子数组的中位数」等价于「左侧小于 $+$ 右侧小于 $=$ 左侧大于 $+$ 右侧大于 $-1$」==，即「左侧小于 $-$ 左侧大于 $=$ 右侧大于 $-$ 右侧小于 $-1$」。相比奇数的情况，等号右侧多了个 $-1$ ，对「右侧大于 $-$ 右侧小于」的值 $x$ 来说，**$cnt[x−1]$ 就是该右端点对应的符合题目要求的长为偶数的子数组个数**。累加这些 $cnt[x−1]$ ，就是子数组长为偶数时的答案。

例如 $[3,2,1,4,5]$ 中就有 $cnt[1−1]=1$ 个，对应的子数组为 $[4,5]$ 。

把子数组长为奇数的和偶数的个数相加，即为答案。
```cpp
class Solution {
public:
    int countSubarrays(vector<int>& nums, int k) {
        int n = nums.size(), kpos = find(nums.begin(), nums.end(), k) - nums.begin();
        // i=pos时x是0,直接记到cnt中,这样下面不是大于k就是小于k
        unordered_map<int, int> cnt;  
        cnt[0] = 1;
        for (int i = kpos - 1, x = 0; i >= 0; --i) { // 从pos-1开始累加k
            x += nums[i] < k ? 1 : -1;
            ++cnt[x]; 
        }
        // i=pos时x是0,直接加到答案中,这样下面不是大于k就是小于k
        int ans = cnt[0] + cnt[-1]; // cnt[0]=1,就是k本身
        for (int i = kpos + 1, x = 0; i < n; ++i) { // 从pos+1开始累加x
            x += nums[i] > k ? 1 : -1;
            ans += cnt[x] + cnt[x - 1];
        }
        return ans;
    }
}; 
```
由于 $x$ 的范围在 $[−(n−1),n−1]$ 之间，所以可以用数组代替哈希表，加快运行速度。考虑到还需访问 $cnt[x−1]$ ，所以实际的范围为 $[−n,n−1]$ ，因此需要一个长为 $2n$ 的数组，此时 $x$ 应当初始化为 $n$ 。
```cpp
class Solution {
public:
    int countSubarrays(vector<int>& nums, int k) {
        int n = nums.size(), kpos = find(nums.begin(), nums.end(), k) - nums.begin();
        // i=pos时x是n,直接记到cnt中,这样下面不是大于k就是小于k 
        int cnt[2 * n];
        memset(cnt, 0, sizeof(cnt));
        cnt[n] = 1;
        for (int i = kpos - 1, x = n; i >= 0; --i) { // 从pos-1开始累加k
            x += nums[i] < k ? 1 : -1;
            ++cnt[x]; 
        }
        // i=pos时x是n,直接加到答案中,这样下面不是大于k就是小于k
        int ans = cnt[n] + cnt[n - 1]; // cnt[n]=1,就是k本身
        for (int i = kpos + 1, x = n; i < n; ++i) { // 从pos+1开始累加x
            x += nums[i] > k ? 1 : -1;
            ans += cnt[x] + cnt[x - 1];
        }
        return ans;
    }
}; 
```
复杂度分析
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $nums$ 的长度。
- 空间复杂度：$O(n)$ ，不计算答案空间。
> 答案的大小看上去是 $O(n^2)$ ，这会不会爆 `int`（超过 $2^{31}-1$ ）？
> **答**：这是个有趣的问题，怎么构造能让答案尽量大呢？
> - 既然 $k$ 是中位数，不妨取 $k=\left[\dfrac{n}{2}\right]$ 且位于 $nums$ 中间，从而让尽量多的子数组包含 $k$ ；
> - 小于 $k$ 和大于 $k$ 的数**交替排布**，从而产生大量重复的 $x$ 。
>  
> 例如 $n=10$ 的时候，取 $k=5$ ，并构造如下 $nums$ ：$[1,6,2,7,5,8,3,9,4,10]$ ，转换成 $[1,−1,1,−1,0,1,−1,1,−1,1]$ 。按照上面的算法，从中间开始向左累加，可以得到大约 $\dfrac{n}{4}$​ 个 $0$ 和 $\dfrac{n}{4}$ 个 $-1$ ；从中间开始向右累加，可以得到大约 $\dfrac{n}{4}$ 个 $0$ 和 $\dfrac{n}{4}$ 个 $1$ 。所以中位数为 $k$ 的奇数长度子数组约有 $\dfrac{n}{4}\times \dfrac{n}{4}$ 个，中位数为 $k$ 的偶数长度子数组约有 $2\times\dfrac{n}{4}\times \dfrac{n}{4}$ 个。
> 答案约为 $\dfrac{3n^2}{16}$ ，代入 $n=10^5$ 得 $1.875\times 10^9 < 2^{31} \approx 2.1\times 10^9$ ，所以不会爆 `int` 。


