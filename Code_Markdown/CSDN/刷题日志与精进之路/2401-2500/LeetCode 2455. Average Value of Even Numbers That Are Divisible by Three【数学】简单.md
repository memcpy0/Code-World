> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个由正整数组成的整数数组 `nums` ，返回其中可被 `3` 整除的所有偶数的平均值。

注意：`n` 个元素的平均值等于 `n` 个元素 **求和** 再除以 `n` ，结果 **向下取整** 到最接近的整数。

**示例 1：**
```java
输入：nums = [1,3,6,10,12,15]
输出：9
解释：6 和 12 是可以被 3 整除的偶数。(6 + 12) / 2 = 9 。
```
**示例 2：**
```java
输入：nums = [1,2,4,7,10]
输出：0
解释：不存在满足题目要求的整数，所以返回 0 。
```
**提示：**
-   `1 <= nums.length <= 1000`
-   `1 <= nums[i] <= 1000`

---
### 解法 遍历
能被 $3$ 和 $2$ 整除的数，就是能被最小公倍数 $\text{LCM}(2, 3)=6$ 整除的数。遍历 $\textit{nums}$ ，统计 $6$ 的倍数的和 $\textit{sum}$ ，以及 $6$ 的倍数的个数 $\textit{cnt}$ 。答案就是 $\left\lfloor\dfrac{\textit{sum}}{\textit{cnt}}\right\rfloor$ 。如果 $cnt=0$ ，返回 $0$ 。
```java
class Solution {
    public int averageValue(int[] nums) {
        int sum = 0, cnt = 0;
        for (int x : nums) {
            if (x % 6 == 0) {
                sum += x;
                cnt++;
            }
        }
        return cnt > 0 ? sum / cnt : 0;
    }
}
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $nums$ 的长度
- 空间复杂度：$O(1)$ ，仅用到若干额外变量。
