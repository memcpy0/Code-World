> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

A sequence of numbers is called **arithmetic** if it consists of at least two elements, and the difference between every two consecutive elements is the same. More formally, a sequence `s` is arithmetic if and only if `s[i+1] - s[i] == s[1] - s[0]` for all valid `i`.

For example, these are **arithmetic** sequences:
```java
1, 3, 5, 7, 9
7, 7, 7, 7
3, -1, -5, -9
```
The following sequence is not **arithmetic**:
```java
1, 1, 2, 5, 7
```
You are given an array of `n` integers, `nums`, and two arrays of `m` integers each, `l` and `r`, representing the `m` range queries, where the `ith` query is the range `[l[i], r[i]]`. All the arrays are **0-indexed**.

Return _a list of_ `boolean` _elements_ `answer`_, where_ `answer[i]` _is_ `true` _if the subarray_ `nums[l[i]], nums[l[i]+1], ... , nums[r[i]]` _can be **rearranged** to form an **arithmetic** sequence, and_ `false` _otherwise._

**Example 1:**
```java
Input: nums = `[4,6,5,9,3,7]`, l = `[0,0,2]`, r = `[2,3,5]`
Output: `[true,false,true]`
Explanation:
In the 0th query, the subarray is [4,6,5]. This can be rearranged as [6,5,4], which is an arithmetic sequence.
In the 1st query, the subarray is [4,6,5,9]. This cannot be rearranged as an arithmetic sequence.
In the 2nd query, the subarray is `[5,9,3,7]. This` can be rearranged as `[3,5,7,9]`, which is an arithmetic sequence.
```
**Example 2:**
```java
Input: nums = [-12,-9,-3,-12,-6,15,20,-25,-20,-15,-10], l = [0,1,6,4,8,7], r = [4,4,9,7,9,10]
Output: [false,true,false,false,true,true]
```
**Constraints:**
-   `n == nums.length`
-   `m == l.length`
-   `m == r.length`
-   `2 <= n <= 500`
-   `1 <= m <= 500`
-   `0 <= l[i] < r[i] < n`
-   `-10^5 <= nums[i] <= 10^5`

题意：给出一个由 `n` 个整数组成的数组 `nums`，和两个由 `m` 个整数组成的数组 `l` 和 `r`，后两个数组表示 `m` 组范围查询，第 `i` 个查询对应范围 `[l[i], r[i]]` 。所有数组的下标都是 **从 0 开始** 的。

返回 `boolean` 元素构成的答案列表 `answer` 。如果子数组 `nums[l[i]], nums[l[i]+1], ... , nums[r[i]]` 可以 **重新排列** 形成 **等差数列** ，`answer[i]` 的值就是 `true`；否则 `answer[i]` 的值就是 `false` 。

---
### 解法 多次遍历 + 枚举
要想判断某个子数组是否可以重新排列成等差数列，最简单的方法**是建立一个等长度的临时数组，将子数组进行拷贝并排序**。对于排完序后的子数组，我们**只需要进行一次遍历，判断相邻两个元素的差值是否保持不变**，就能判断出它是否为等差数列。

然而这样做的单次时间复杂度是 $O(l \log l)$ ，其中 $l$ 是子数组的长度。我们可以给出时间复杂度更低的 $O(l)$ 算法。我们首先进行一次遍历，找到子数组中的最小值 $a$ 和最大值 $b$ 。显然，**$a$ 和 $b$ 就是等差数列的首项和末项，并且 $d = \dfrac{b-a}{l-1}$​ 就是等差数列的公差**。
- **如果 $b-a$ 不能被 $l-1$ 整除**，那么我们直接知道子数组不可能重新排列成等差数列（2 2 3 5，a=2，b=5，b-a=3，l-1=4-1=3）
- **如果 $a = b$（即 $d= 0$）**，那么子数组中的每个元素都应该相同，说明它是一个等差数列。
- 否则，我们已经知道了等差数列的首项、公差和长度，这个等差数列唯一确定。我们再进行一次遍历，对于遍历到的元素 $x$ ，我们可以通过：
$$t = \frac{x-a}{d}$$
	获取它是等差数列中的第 $t~(0 \leq t < l)$ 项。**如果 $x−a$ 不能被 $d$ 整除，那么我们同样知道子数组不可能重新排列成等差数列，可以退出遍历**。否则，当我们遍历完整个子数组后，第 $0, 1, 2, \cdots, l-1$ 项应该均出现了一次，这里我们**可以使用哈希表或者一个长度为 $l$ 的数组进行判断**。
	这样一来，==我们使用两次遍历以及一个长度为 $l$ 的辅助数组，就可以判断子数组是否可以重新排列成等差数列==，时间复杂度为 $O(l)$ 。

```cpp
class Solution {
public:
    vector<bool> checkArithmeticSubarrays(vector<int>& nums, vector<int>& l, vector<int>& r) {
        int n = l.size();
        vector<bool> ans;
        for (int i = 0; i < n; ++i) {
            int left = l[i], right = r[i];
            int minv = *min_element(nums.begin() + left, nums.begin() + right + 1);
            int maxv = *max_element(nums.begin() + left, nums.begin() + right + 1);

            if (minv == maxv) {
                ans.push_back(true);
                continue;
            }
            if ((maxv - minv) % (right - left) != 0) {
                ans.push_back(false);
                continue;
            }

            int d = (maxv - minv) / (right - left);
            bool flag = true;
            vector<bool> seen(right - left + 1);
            for (int j = left; j <= right; ++j) {
                if ((nums[j] - minv) % d != 0) {
                    flag = false;
                    break;
                }
                int t = (nums[j] - minv) / d;
                if (seen[t]) {
                    flag = false;
                    break;
                }
                seen[t] = true;
            }
            ans.push_back(flag);
        }
        return ans;
    }
};
```
复杂度分析：
- 时间复杂度：$O(nm)$ 。其中 $n$ 是数组 $nums$ 的长度，$m$ 是查询的次数。
- 空间复杂度：$O(n)$ ，即为辅助数组/哈希表需要使用的空间。
 