> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个整数数组 `arr`，找到 `min(b)` 的总和，其中 `b` 的范围为 `arr` 的每个（连续）子数组。

由于答案可能很大，因此 **返回答案模 `10^9 + 7`** 。

**示例 1：**
```java
输入：arr = [3,1,2,4]
输出：17
解释：
子数组为 [3]，[1]，[2]，[4]，[3,1]，[1,2]，[2,4]，[3,1,2]，[1,2,4]，[3,1,2,4]。 
最小值为 3，1，2，4，1，1，2，1，1，1，和为 17。
```
**示例 2：**
```java
输入：arr = [11,81,94,43,3]
输出：444
```
**提示：**
- `1 <= arr.length <= 3 * 10^4`
- `1 <= arr[i] <= 3 * 10^4`

---
- [907. 子数组的最小值之和](https://leetcode.cn/problems/sum-of-subarray-minimums/)
- [1508. 子数组和排序后的区间和](https://leetcode.cn/problems/range-sum-of-sorted-subarray-sums/)
- [1856. 子数组最小乘积的最大值](https://leetcode.cn/problems/maximum-subarray-min-product/)
- [2104. 子数组范围和](https://leetcode.cn/problems/sum-of-subarray-ranges/)
- [2281. 巫师的总力量和]
- [2681. Power of Heroes](https://leetcode.cn/problems/power-of-heroes/description/)
- [2818. 操作使得分最大](https://leetcode.cn/problems/apply-operations-to-maximize-score/)
### 解法 单调栈+贡献法
**提示 1**：**计算以 $arr[i]$ 为最小值的子数组的个数**
暴力做法是枚举所有子数组，算出每个子数组的最小值，但这太慢了。

不妨换个视角，对每个数，算出它是哪些子数组的最小值。例如 $arr=[1,4,2,3,1]$，其中 $2$ 是子数组 $[2],[4,2],[2,3],[4,2,3]$ 的最小值，那么 $2$ 对答案的**贡献**就是 $2\cdot 4=8$ 。

由于以 $2$ 为最小值的子数组，绝对不能包含比 $2$ 小的数字，因此我们可以**找到 $2$ 左右两侧比它小的数的下标，从而确定子数组的边界**。$2$ 对应的边界为开区间 $(0,4)$ ，即闭区间 $[1,3]$，只要在闭区间 $[1,3]$ 范围内且包含下标 $2$ 的子数组，就是以 $2$ 为最小值的子数组。

一般地，设 $arr[i]$ 对应的边界为**开区间** $(L,R)$ ，由于子数组必须包含 $arr[i]$：
- 子数组的左端点可以是 $L+1,L+2,...,i$ ，共有 $i-L$ 个；
- 子数组的右端点可以是 $i,i+1,\cdots,R-1$ ，共有 $R-i$ 个。

因此，**在 $arr$ 不含重复元素的前提下**，根据**乘法原理**，我们可以得出如下结论：以 $arr[i]$ 为最小值的子数组的个数为 $(i-L)\cdot(R-i)$ ，对答案的贡献为 $\textit{arr}[i]\cdot(i-L)\cdot(R-i)$ 。
> 注：**如果左侧没有比 $arr[i]$ 小的元素，则 $L=-1$ ；如果右侧没有比 $arr[i]$ 小的元素，则 $R=n$ 。**

下文讨论的边界均指的是开区间 $(L,R)$ 。

**提示 2：修改边界定义，避免重复统计子数组**
如果 $arr$ 有重复元素，例如 $\textit{arr}=[1,2,4,2,3,1]$ ，其中第一个 $2$ 和第二个 $2$ 对应的边界都是开区间 $(0,5)$ ，子数组 $[2,4,2,3]$ 都包含这两个 $2$，这样在计算答案时，就会**重复统计同一个子数组**，算出错误的结果。

为避免重复统计，可以修改边界的定义，把右边界改为「找**小于或等于** $arr[i]$ 的数的下标」，那么：
- 第一个 $2$ 对应的边界是 $(0,3)$ ，子数组需要在 $(0,3)$ 范围内且包含下标 $1$ ；
- 第二个 $2$ 对应的边界是 $(0,5)$ ，子数组需要在 $(0,5)$ 范围内且包含下标 $3$ 。

这样**以第一个 $2$ 为最小值的子数组，就不会「越界」包含第二个 $2$ 了**，从而解决了重复统计子数组的问题。
> 注：也可以把左边界改为 $\le$ ，右边界不变（仍为 $<$ ）。**根据对称性，算出来的答案是一样的**。

**提示 3：高效计算边界**
最后需要解决的问题是，如何高效计算每个 $arr[i]$ 对应的左右边界？

以计算左边界为例，从左到右遍历 $arr$，同时用某个合适的数据结构维护遍历过的元素，并及时移除无用的元素，如下图（$arr$ 简写为 aaa）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308220030963.png)
移除无用元素后，再把 $arr[i]$ 加到这个数据结构中。

由于该优化保证了数据结构中的元素会形成一个递增的序列，因此我们移除的是数据结构中的最右端的若干元素。我们需要一个数据结构，它支持移除最右端的元素，以及在最右端添加元素，故选用栈。

移除栈中 ≥arr[i]\ge\textit{arr}[i]≥arr[i] 的元素后，栈顶的下标就是 $arr[i]$ 的左边界，如果此时栈为空，那么左边界为 −1-1−1。

注 1：在遍历过程中会维护一个栈，移除的是当前的栈顶元素。遍历到 $arr[i]$ 时，$arr[i]$ 及其右侧元素尚未入栈。 注 2：由于栈内元素始终保持单调递增，因此这种数据结构也叫做单调栈。

右边界的计算是类似的，从右往左遍历 $arr$ 可以算出（根据提示 2，需要修改一下比较的符号，具体见代码）。

算出左右边界后，根据提示 1 的公式累加每个 $arr[i]$ 的贡献，即为答案。

代码实现时，有三种实现版本。注意实际入栈的是元素的下标。

版本一：三次遍历
Python3
Java
C++
Go


作者：灵茶山艾府
链接：https://leetcode.cn/problems/sum-of-subarray-minimums/solutions/1930857/gong-xian-fa-dan-diao-zhan-san-chong-shi-gxa5/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。