> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的正整数数组 `nums` 。请你找出并统计满足下述条件的三元组 `(i, j, k)` 的数目：
- `0 <= i < j < k < nums.length`
- `nums[i]`、`nums[j]` 和 `nums[k]` **两两不同** 。
    - 换句话说：`nums[i] != nums[j]`、`nums[i] != nums[k]` 且 `nums[j] != nums[k]` 。

返回满足上述条件三元组的数目。

**示例 1：**
```java
输入：nums = [4,4,2,4,3]
输出：3
解释：下面列出的三元组均满足题目条件：
- (0, 2, 4) 因为 4 != 2 != 3
- (1, 2, 4) 因为 4 != 2 != 3
- (2, 3, 4) 因为 2 != 4 != 3
共计 3 个三元组，返回 3 。
注意 (2, 0, 4) 不是有效的三元组，因为 2 > 0 。
```
**示例 2：**
```java
输入：nums = [1,1,1,1,1]
输出：0
解释：不存在满足条件的三元组，所以返回 0 。
```
**提示：**
- `3 <= nums.length <= 100`
- `1 <= nums[i] <= 1000`

---
### 解法1 暴力循环
暴力三重循环：
```java
class Solution {
    public int unequalTriplets(int[] nums) {
        int ans = 0;
        for (int i = 0; i < nums.length; ++i)
            for (int j = i + 1; j < nums.length; ++j)
                for (int k = j + 1; k < nums.length; ++k)
                    if (nums[i] != nums[j] && nums[j] != nums[k] && nums[i] != nums[k]) ++ans;
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n^3)$
- 空间复杂度：$O(1)$

---
### 解法2 排序+分组统计
对于 $x$ ，设：
- 小于 $x$ 的数有 $a$ 个；
- 等于 $x$ 的数有 $b$ 个；
- 大于 $x$ 的数有 $c$ 个。

那么 $x$ 对答案的贡献是 $a\times b \times c$ 。累加所有贡献，得到答案。

代码实现时，可通过排序快速求出 $a\ b\ c$ 。然后从头开始遍历，并分段计算「相同数的个数」。
```java
class Solution {
    public int unequalTriplets(int[] nums) {
        Arrays.sort(nums);
        int ans = 0, start = 0;
        for (int i = 0; i + 1 < nums.length; ++i) {
            if (nums[i] != nums[i + 1]) { // 到一段的末尾
                ans += start * (i - start + 1) * (nums.length - 1 - i);
                start = i + 1;
            }
        } 
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n\log n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$O(1)$ ，忽略排序时的栈开销，仅用到若干变量。

---
### 解法3 计数统计
由于元素的位置是不重要的，我们可以直接用哈希表/数组统计，之前的 $a\ b\ c$ 重定义为，**按顺序从小到大遍历**：
- 在 $x$ 之前遍历过的数有 $a$ 个；
- （当前遍历的）等于 $x$ 的数有 $b$ 个；
- 在 $x$ 之后遍历过的数有 $c$ 个。

```java
class Solution {
    public int unequalTriplets(int[] nums) {
        int[] count = new int[1001];
        for (int i : nums) ++count[i];
        int a = 0, b = 0, c = nums.length;
        int ans = 0;
        for (int i = 1; i <= 1000; ++i) {
            if (count[i] != 0) {
                c -= count[i];
                ans += a * count[i] * c;
                a += count[i];
            }
        }
        return ans;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $\textit{nums}$ 的长度。
- 空间复杂度：$O(1000)$ 。