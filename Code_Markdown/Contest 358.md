> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


给你一个下标从 **0** 开始的整数数组 `nums` 。请你从 `nums` 中找出和 **最大** 的一对数，且这两个数数位上最大的数字相等。

返回最大和，如果不存在满足题意的数字对，返回 `-1` _。_

**示例 1：**
```java
输入：nums = [51,71,17,24,42]
输出：88
解释：
i = 1 和 j = 2 ，nums[i] 和 nums[j] 数位上最大的数字相等，且这一对的总和 71 + 17 = 88 。 
i = 3 和 j = 4 ，nums[i] 和 nums[j] 数位上最大的数字相等，且这一对的总和 24 + 42 = 66 。
可以证明不存在其他数对满足数位上最大的数字相等，所以答案是 88 。
```
**示例 2：**
```java
输入：nums = [1,2,3,4]
输出：-1
解释：不存在数对满足数位上最大的数字相等。
```
**提示：**
- `2 <= nums.length <= 100`
- `1 <= nums[i] <= 104`

用一个长为 101010 的数组 maxVal[i]\textit{maxVal}[i]maxVal[i] 维护最大数位为 iii 的元素的最大值。

当我们遍历到 nums[i]\textit{nums}[i]nums[i] 时，设其最大数位为 maxD\textit{maxD}maxD，那么用

nums[i]+maxVal[maxD] \textit{nums}[i] + \textit{maxVal}[\textit{maxD}]
nums[i]+maxVal[maxD]
更新答案。

Python3
Java
C++
Go
JavaScript
class Solution {
public:
    int maxSum(vector<int> &nums) {
        int ans = -1;
        vector<int> max_val(10, INT_MIN);
        for (int v: nums) {
            int max_d = 0;
            for (int x = v; x; x /= 10)
                max_d = max(max_d, x % 10);
            ans = max(ans, v + max_val[max_d]);
            max_val[max_d] = max(max_val[max_d], v);
        }
        return ans;
    }
};
复杂度分析
时间复杂度：O(nlog⁡U)\mathcal{O}(n\log U)O(nlogU)，其中 $n$ 为 nums\textit{nums}nums 的长度，U=max(nums)U=max(\textit{nums})U=max(nums)。
空间复杂度：$O(1)$。仅用到若干额外变量。

 
 

> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个 **非空** 链表的头节点 `head` ，表示一个不含前导零的非负数整数。

将链表 **翻倍** 后，返回头节点 `head` 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2023/05/28/example.png)

```java
输入：head = [1,8,9]
输出：[3,7,8]
解释：上图中给出的链表，表示数字 189 。返回的链表表示数字 189 * 2 = 378 。
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2023/05/28/example2.png)
```java
输入：head = [9,9,9]
输出：[1,9,9,8]
解释：上图中给出的链表，表示数字 999 。返回的链表表示数字 999 * 2 = 1998 。
```
**提示：**
- 链表中节点的数目在范围 `[1, 10^4]` 内
- `0 <= Node.val <= 9`
- 生成的输入满足：链表表示一个不含前导零的数字，除了数字 `0` 本身。

方法一
看成是 head\textit{head}head 与 head\textit{head}head 这两个链表相加。

直接调用 445. 两数相加 II 的代码即可。

Python3
Go
# https://space.bilibili.com/206214
class Solution:
    # 206. 反转链表
    # 视频讲解 https://www.bilibili.com/video/BV1sd4y1x7KN/
    def reverseList(self, head: Optional[ListNode]) -> Optional[ListNode]:
        pre = None
        cur = head
        while cur:
            nxt = cur.next
            cur.next = pre
            pre = cur
            cur = nxt
        return pre

    # 2. 两数相加：自己和自己相加
    # 题解 https://leetcode.cn/problems/add-two-numbers/solution/dong-hua-jian-ji-xie-fa-cong-di-gui-dao-oe0di/
    def double(self, l1: Optional[ListNode]) -> Optional[ListNode]:
        cur = dummy = ListNode()  # 哨兵节点
        carry = 0  # 进位
        while l1:  # 有一个不是空节点，或者还有进位，就继续迭代
            carry += l1.val * 2  # 节点值和进位加在一起
            cur.next = ListNode(carry % 10)  # 每个节点保存一个数位
            carry //= 10  # 新的进位
            cur = cur.next  # 下一个节点
            l1 = l1.next  # 下一个节点
        if carry:
            cur.next = ListNode(carry)
        return dummy.next  # 哨兵节点的下一个节点就是头节点

    def doubleIt(self, head: Optional[ListNode]) -> Optional[ListNode]:
        head = self.reverseList(head)
        res = self.double(head)  # 反转后，就变成【2. 两数相加】了
        return self.reverseList(res)
复杂度分析
时间复杂度：$O(n)$ ，其中 $n$ 为链表的长度。
空间复杂度：$O(1)$ 。返回值不计入。
方法二
如果不考虑进位，就是每个节点的值乘以 222。

什么时候会受到进位的影响呢？只有下一个节点大于 444 的时候，才会因为进位多加一。

特别地，如果链表头的值大于 444，那么需要在前面插入一个新的节点。

 
class Solution {
public:
    ListNode *doubleIt(ListNode *head) {
        if (head->val > 4)
            head = new ListNode(0, head);
        for (auto cur = head; cur; cur = cur->next) {
            cur->val = cur->val * 2 % 10;
            if (cur->next && cur->next->val > 4)
                cur->val++;
        }
        return head;
    }
};
复杂度分析
时间复杂度：$O(n)$，其中 $n$ 为链表的长度。
空间复杂度：$O(1)$。


> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个下标从 **0** 开始的整数数组 `nums` 和一个整数 `x` 。

请你找到数组中下标距离至少为 `x` 的两个元素的 **差值绝对值** 的 **最小值** 。

换言之，请你找到两个下标 `i` 和 `j` ，满足 `abs(i - j) >= x` 且 `abs(nums[i] - nums[j])` 的值最小。

请你返回一个整数，表示下标距离至少为 `x` 的两个元素之间的差值绝对值的 **最小值** 。

**示例 1：**
```java
输入：nums = [4,3,2,4], x = 2
输出：0
解释：我们选择 nums[0] = 4 和 nums[3] = 4 。
它们下标距离满足至少为 2 ，差值绝对值为最小值 0 。
0 是最优解。
```
**示例 2：**
```java
输入：nums = [5,3,2,10,15], x = 1
输出：1
解释：我们选择 nums[1] = 3 和 nums[2] = 2 。
它们下标距离满足至少为 1 ，差值绝对值为最小值 1 。
1 是最优解。
```
**示例 3：**
```java
输入：nums = [1,2,3,4], x = 3
输出：3
解释：我们选择 nums[0] = 1 和 nums[3] = 4 。
它们下标距离满足至少为 3 ，差值绝对值为最小值 3 。
3 是最优解。
```
**提示：**
- `1 <= nums.length <= 10^5`
- `1 <= nums[i] <= 10^9`
- `0 <= x < nums.length`

class Solution {
public:
    int minAbsoluteDifference(vector<int> &nums, int x) {
        int ans = INT_MAX, n = nums.size();
        set<int> s = {INT_MIN / 2, INT_MAX}; // 哨兵
        for (int i = x; i < n; i++) {
            s.insert(nums[i - x]);
            int y = nums[i];
            auto it = s.lower_bound(y); // 注意用 set 自带的 lower_bound，具体见视频中的解析
            ans = min(ans, min(*it - y, y - *--it));
        }
        return ans;
    }
};
复杂度分析
时间复杂度：O((n−x)log⁡(n−x))\mathcal{O}((n-x)\log (n-x))O((n−x)log(n−x))，其中 $n$ 为 nums\textit{nums}nums 的长度。
空间复杂度：$O(n-x)$ 。

 

> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你一个长度为 `n` 的正整数数组 `nums` 和一个整数 `k` 。

一开始，你的分数为 `1` 。你可以进行以下操作至多 `k` 次，目标是使你的分数最大：
- 选择一个之前没有选过的 **非空** 子数组 `nums[l, ..., r]` 。
- 从 `nums[l, ..., r]` 里面选择一个 **质数分数** 最高的元素 `x` 。如果多个元素质数分数相同且最高，选择下标最小的一个。
- 将你的分数乘以 `x` 。

`nums[l, ..., r]` 表示 `nums` 中起始下标为 `l` ，结束下标为 `r` 的子数组，两个端点都包含。

一个整数的 **质数分数** 等于 `x` 不同质因子的数目。比方说， `300` 的质数分数为 `3` ，因为 `300 = 2 * 2 * 3 * 5 * 5` 。

请你返回进行至多 `k` 次操作后，可以得到的 **最大分数** 。

由于答案可能很大，请你将结果对 `109 + 7` 取余后返回。

**示例 1：**
```java
输入：nums = [8,3,9,3,8], k = 2
输出：81
解释：进行以下操作可以得到分数 81 ：
- 选择子数组 nums[2, ..., 2] 。nums[2] 是子数组中唯一的元素。所以我们将分数乘以 nums[2] ，分数变为 1 * 9 = 9 。
- 选择子数组 nums[2, ..., 3] 。nums[2] 和 nums[3] 质数分数都为 1 ，但是 nums[2] 下标更小。所以我们将分数乘以 nums[2] ，分数变为 9 * 9 = 81 。
81 是可以得到的最高得分。
```
**示例 2：**
```java
输入：nums = [19,12,14,6,10,18], k = 3
输出：4788
解释：进行以下操作可以得到分数 4788 ：
- 选择子数组 nums[0, ..., 0] 。nums[0] 是子数组中唯一的元素。所以我们将分数乘以 nums[0] ，分数变为 1 * 19 = 19 。
- 选择子数组 nums[5, ..., 5] 。nums[5] 是子数组中唯一的元素。所以我们将分数乘以 nums[5] ，分数变为 19 * 18 = 342 。
- 选择子数组 nums[2, ..., 3] 。nums[2] 和 nums[3] 质数分数都为 2，但是 nums[2] 下标更小。所以我们将分数乘以 nums[2] ，分数变为  342 * 14 = 4788 。
4788 是可以得到的最高的分。
```
**提示：**
- `1 <= nums.length == n <= 10^5`
- `1 <= nums[i] <= 10^5`
- `1 <= k <= min(n * (n + 1) / 2, 10^9)`

本文接着 **贡献法**+单调栈 继续讲，请先阅读这篇题解，因为核心逻辑是一样的。

贪心地说，先考虑 nums\textit{nums}nums 中最大的元素 xxx，我们需要知道：有多少个子数组可以取 xxx 作为质数分数最高的元素。

我们可以先把 [1,105][1,10^5][1,10 
5
 ] 中的每个数的质数分数（不同质因子的数目）预处理出来，记作数组 omega\textit{omega}omega。

然后用单调栈求出每个 iii 左侧质数分数【大于等于】omega[nums[i]]\textit{omega}[\textit{nums}[i]]omega[nums[i]] 的最近的数的下标 left[i]\textit{left}[i]left[i]（不存在则为 −1-1−1），以及右侧质数分数【大于】omega[nums[i]]\textit{omega}[\textit{nums}[i]]omega[nums[i]] 的最近的数的下标 right[i]\textit{right}[i]right[i]（不存在则为 $n$）。

请注意，我们不能在 iii 左侧包含质数分数和 omega[nums[i]]\textit{omega}[\textit{nums}[i]]omega[nums[i]] 一样的数，否则不满足题目「如果多个元素质数分数相同且最高，选择下标最小的一个」的要求。所以对于左侧用【大于等于】。

那么子数组的左端点可以在 (left[i],i](\textit{left}[i],i](left[i],i] 内，子数组的右端点可以在 [i,right[i])[i,\textit{right}[i])[i,right[i]) 内。

根据 乘法原理，一共有

tot=(i−left[i])⋅(right[i]−i) \textit{tot} = (i-\textit{left}[i])\cdot (\textit{right}[i]-i)
tot=(i−left[i])⋅(right[i]−i)
个子数组以 nums[i]\textit{nums}[i]nums[i] 作为这个子数组的质数分数。

设 k′=min⁡(k,tot)k'=\min(k, \textit{tot})k 
′
 =min(k,tot)，则 nums[i]\textit{nums}[i]nums[i] 对答案的贡献为

nums[i]k′ \textit{nums}[i] ^ {k'}
nums[i] 
k 
′
 
 
上式可以用快速幂计算，具体请看 50. Pow(x, n)。

根据上式，按照 nums\textit{nums}nums 从大到小的顺序计算贡献，一边计算一边更新剩余操作次数 kkk。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308131808015.png)

Python3
Java
C++
Go
MOD = 10 ** 9 + 7
MX = 10 ** 5 + 1
omega = [0] * MX
for i in range(2, MX):  # 预处理 omega
    if omega[i] == 0:  # i 是质数
        for j in range(i, MX, i):
            omega[j] += 1  # i 是 j 的一个质因子

class Solution:
    def maximumScore(self, nums: List[int], k: int) -> int:
        n = len(nums)
        left = [-1] * n  # 质数分数 >= omega[nums[i]] 的左侧最近元素下标
        right = [n] * n  # 质数分数 >  omega[nums[i]] 的右侧最近元素下标
        st = []
        for i, v in enumerate(nums):
            while st and omega[nums[st[-1]]] < omega[v]:
                right[st.pop()] = i
            if st: left[i] = st[-1]
            st.append(i)

        ans = 1
        for i, v, l, r in sorted(zip(range(n), nums, left, right), key=lambda z: -z[1]):
            tot = (i - l) * (r - i)
            if tot >= k:
                ans = ans * pow(v, k, MOD) % MOD
                break
            ans = ans * pow(v, tot, MOD) % MOD
            k -= tot  # 更新剩余操作次数
        return ans
复杂度分析
时间复杂度：O(nlog⁡n)\mathcal{O}(n\log n)O(nlogn)，其中 $n$ 为 nums\textit{nums}nums 的长度。这里忽略预处理 omega\textit{omega}omega 的时间和空间。
空间复杂度：$O(n)$。
相似题目：单调栈+贡献法
907. **子数组的最小值之和**
1856. **子数组最小乘积的最大值**
2104. **子数组范围和**
2281. **巫师的总力量和**
