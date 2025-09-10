> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给定一个大小为 `n` 的数组 `nums` ，返回其中的多数元素。多数元素是指在数组中出现次数 **大于** `⌊ n/2 ⌋` 的元素。

你可以假设数组是非空的，并且给定的数组总是存在多数元素。

**示例 1：**
```js
输入：nums = [3,2,3]
输出：3
```
**示例 2：**
```js
输入：nums = [2,2,1,1,1,2,2]
输出：2
```
**提示：**
- `n == nums.length`
- `1 <= n <= 5 * 10^4`
- `-109 <= nums[i] <= 10^9`

**进阶：** 尝试设计时间复杂度为 O(n)、空间复杂度为 O(1) 的算法解决此问题。

---
题目集合：
- [169. 多数元素](https://leetcode.cn/problems/majority-element/)
- [面试题 17.10. Find Majority Element LCCI](https://leetcode.cn/problems/find-majority-element-lcci/)
- [229. 多数元素 II](https://leetcode.cn/problems/majority-element-ii/)
- 1150. Check If a Number Is Majority Element in a Sorted Array
- [1157. 子数组中占绝大多数的元素](https://leetcode.cn/problems/online-majority-element-in-subarray/)：困难
### 解法 Boyer-Moore 投票算法
由于题目要求时间复杂度 $O(n)$ 和空间复杂度 $O(1)$ ，因此符合要求的解法只有 Boyer-Moore 投票算法。这一投票算法在求出现次数大于 $\lfloor n / 2 \rfloor$ 的元素 $x$ 时很好理解：**如果我们把 $x$ 记为 $+1$ ，把其他数记为 $-1$ ，将它们全部加起来，显然和大于 $0$** ，从结果本身我们可以看出 $x$ 比其他数多。

我们首先给出 Boyer-Moore 算法的详细步骤：
1. 维护一个**候选主要元素** $candidate$ 和候选主要元素的出现次数 $count$ ，**初始时 $candidate$ 为任意值**，$count=0$ ；
2. 遍历数组 $\textit{nums}$ 中的所有元素，**遍历到元素 $x$ 时**，在判断 $x$ 之前，**如果 $count=0$ ，我们先将 $x$ 的值赋给 $candidate$** ，否则不更新 $candidate$ 的值。随后我们判断 $x$ ：
    1. 如果 $x=candidate$ ，则将 $count$ 加 $1$ ；
    2. 如果 $x\ne candidate$ ，则将 $count$ 减 $1$ 。
3. 遍历结束之后，**如果数组 $nums$ 中存在主要元素，则 $candidate$ 即为主要元素**，否则 $candidate$ 可能为数组中的任意一个元素。
4. ==由于不一定存在主要元素，因此需要第二次遍历数组，验证 $candidate$ 是否为主要元素==。第二次遍历时，统计 $candidate$ 在数组中的出现次数，如果出现次数大于数组长度的一半，则 $candidate$ 是主要元素，返回 $candidate$ ，否则数组中不存在主要元素，返回 $-1$ 。

举一个具体的例子，例如下面的这个数组：
```js
[7, 7, 5, 7, 5, 1 | 5, 7 | 5, 5, 7, 7 | 7, 7, 7, 7]
```
在遍历到**数组中的第一个元素**以及**每个在 `|` 之后的元素**时，$candidate$ 都会因为 $count$ 的值变为 $0$ 而发生改变。最后一次 $candidate$ 的值从 $5$ 变为 $7$ ，也就是这个数组中的主要元素。

Boyer-Moore 算法的正确性较难证明，这里给出一种较为详细的用例子辅助证明的思路，供参考：
1.首先根据算法步骤中对 $count$ 的定义，可以发现：在对整个数组进行遍历的过程中，$count$ 的值一定非负。这是因为==如果 $count$ 的值为 $0$ ，那么在这一轮遍历的开始时刻，我们会将 $x$ 的值赋予 $candidate$ 并在接下来的一步中将 $count$ 的值增加 $1$ ==。因此 **$count$ 的值在遍历过程中一直保持非负**。

2.那么 $count$ 本身除了计数器之外，还有什么更深层次的意义呢？我们还是以数组：
```js
[7, 7, 5, 7, 5, 1 | 5, 7 | 5, 5, 7, 7 | 7, 7, 7, 7]
```
作为例子，首先写下它在每一步遍历时 $candidate$ 和 $count$ 的值：
```js
nums:      [7, 7, 5, 7, 5, 1 | 5, 7 | 5, 5, 7, 7 | 7, 7, 7, 7]
candidate:  7  7  7  7  7  7   5  5   5  5  5  5   7  7  7  7
count:      1  2  1  2  1  0   1  0   1  2  1  0   1  2  3  4
```
我们再定义**一个变量 $value$ ，它和真正的主要元素 $maj$ 绑定。在每一步遍历时，如果当前的数 $x$ 和 $maj$ 相等，那么 $value$ 的值加 $1$ ，否则减 $1$** 。$value$ 的实际意义即为：**到当前的这一步遍历为止，主要元素出现的次数比非主要元素多出了多少次**。我们将 $value$ 的值也写在下方：
```js
nums:      [7, 7, 5, 7, 5, 1 | 5, 7 | 5, 5, 7, 7 | 7, 7, 7, 7]
value:      1  2  1  2  1  0  -1  0  -1 -2 -1  0   1  2  3  4
```
有没有发现什么？我们将 $count$ 和 $value$ 放在一起：
```js
nums:      [7, 7, 5, 7, 5, 1 | 5, 7 | 5, 5, 7, 7 | 7, 7, 7, 7]
count:      1  2  1  2  1  0   1  0   1  2  1  0   1  2  3  4
value:      1  2  1  2  1  0  -1  0  -1 -2 -1  0   1  2  3  4
```
发现**在每一步遍历中，$count$ 和 $value$ 要么相等，要么互为相反数**！并且在候选主要元素 $candidate$ 就是 $maj$ 时，它们相等，$candidate$ 是其它的数时，它们互为相反数！

为什么会有这么奇妙的性质呢？这并不难证明：我们**将候选主要元素 $candidate$ 保持不变的连续的遍历称为「一段**」。在同一段中，$count$ 的值是根据 $candidate == x$ 的判断进行加减的。那么如果 $candidate$ 恰好为 $maj$ ，那么在这一段中，$count$ 和 $value$ 的变化是同步的；如果 $candidate$ 不为 $maj$ ，那么在这一段中 $count$ 和 $value$ 的变化是相反的。因此就有了这样一个奇妙的性质。

这样以来，由于：
- 我们证明了 $count$ 的值一直为非负，在最后一步遍历结束后也是如此；
- 由于 $value$ 的值与真正的主要元素 $maj$ 绑定，并且它表示「主要元素出现的次数比非主要元素多出了多少次」，那么**在最后一步遍历结束后，$value$ 的值为正数**；
- 在最后一步遍历结束后，$count$ 非负，$value$ 为正数，所以它们不可能互为相反数，**只可能相等**，即 $count == value$ 。因此在最后「一段」中，$count$ 的 $value$ 的变化是同步的，也就是说，**$candidate$ 中存储的候选主要元素就是真正的主要元素 $maj$** 。

```cpp
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candidate = 0, count = 0;
        for (int num : nums) {
            if (count == 0) candidate = num;
            if (candidate == num) ++count;
            else --count;
        }
        count = 0;
        for (int num : nums) if (num == candidate) ++count;
        return count * 2 > nums.size() ? candidate : -1;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是数组 $nums$ 的长度。
- 空间复杂度：$O(1)$ 。只需要常数的额外空间。