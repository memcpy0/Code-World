> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的[仓库](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

<p>Given an integer array <code>nums</code> and an integer <code>k</code>, modify the array in the following way:</p>

<ul>
	<li>choose an index <code>i</code> and replace <code>nums[i]</code> with <code>-nums[i]</code>.</li>
</ul>

<p>You should apply this process exactly <code>k</code> times. You may choose the same index <code>i</code> multiple times.</p>

<p>Return <em>the largest possible sum of the array after modifying it in this way</em>.</p>
 
<p><strong>Example 1:</strong></p>

```cpp
Input: nums = [4,2,3], k = 1
Output: 5
Explanation: Choose index 1 and nums becomes [4,-2,3].
```
<p><strong>Example 2:</strong></p>

```cpp
Input: nums = [3,-1,0,2], k = 3
Output: 6
Explanation: Choose indices (1, 2, 2) and nums becomes [3,1,0,2].
```
<p><strong>Example 3:</strong></p>

```cpp
Input: nums = [2,-3,-1,5,-4], k = 2
Output: 13
Explanation: Choose indices (1, 4) and nums becomes [2,3,-1,5,4].
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= nums.length &lt;= 10<sup>4</sup></code></li>
	<li><code>-100 &lt;= nums[i] &lt;= 100</code></li>
	<li><code>1 &lt;= k &lt;= 10<sup>4</sup></code></li>
</ul>


题意：给你一个整数数组 `nums` 和一个整数 `k` ，按以下方法修改该数组：选择某个下标 `i` 并将 `nums[i]` 替换为 `-nums[i]` 。重复这个过程恰好 `k` 次。可以多次选择同一个下标 `i` 。以这种方式修改数组后，返回数组 **可能的最大和** 。 

---
### 解法 排序+贪心
假设整个数组的总和为 $sum$ ，取反任何一个值 $x$ 后，数组总和变为 $sum - 2x$ 。即取反一个负数会使结果变大，取反一个正数会使结果变小，取反零值对结果没有影响。

为了让取反 $k$ 次后的结果尽可能的大，我们应当取反令 $-2x$ 尽可能大的数值，即按照「负数从小到大的顺序」进行取反。我们对取反次数 $k$ 和负数个数 $cnt$ 分类讨论：
1. $k \le cnt$ ：按照负数从小到大的顺序，进行取反即可；
2. $k > cnt$ ：按照负数从小到大的顺序取反完成后，由于还剩下修改次数，要根据「是否存在零值」和「剩余取反次数的奇偶性」分类讨论：
	1. 存在零值，或剩余取反次数为偶数：直接返回当前取反数组的元素总和——零值可抵消任意次取反；将偶数次的取反操作，应用在同一数值上，结果不变。
	2. 不存在零值，且剩余取反次数为奇数：此时，从当前数值中，取出一个绝对值最小值（使用 $idx$ 记录该值下标）进行取反，得到最终的取反数组，最后对取反数组进行求和即可。

分类情况如上所述。实际的代码编写中，我们可以做得更简洁——==如果数组中有负数，就按照负数从小到大的顺序，尽可能将负数转为正数，然后加上取反后的负数和原来的正数（如果有未能取反的负数，也要加上），同时记录变换后数组的最小值==。遍历完整个数组后，还有修改次数时对应情况2，此时最小值要么为零、要么为正数，如果修改次数为奇数，再进行取反。
```cpp
//C++ version
class Solution {
public:
    int largestSumAfterKNegations(vector<int>& A, int K) {
        sort(A.begin(), A.end());
        int sum = 0, minval = INT_MAX;
        for (int i = 0, n = A.size(); i < n; ++i) {
            int val = A[i];
            if (val < 0 && K > 0) { // 按照负数从小到大的顺序进行取反
                --K;
                val = -val;
            }
            sum += val; // 计算数组总和(元素可能取反或未取反)
            minval = min(minval, val); // 记录进行全部的取反操作后,数组的最小值
        }
        if (K > 0 && (K & 1))   // 还有取反次数,且为奇数次(偶数次反转后还是原数,不需要考虑)
            sum -= 2 * minval;  // 减去2倍的最小值 
        return sum;
    }
};
//执行用时：4 ms, 在所有 C++ 提交中击败了87.64% 的用户
//内存消耗：8.7 MB, 在所有 C++ 提交中击败了92.30% 的用户
```
