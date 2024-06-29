> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你单链表的头指针 `head` 和两个整数 `left` 和 `right` ，其中 `left <= right` 。请你反转从位置 `left` 到位置 `right` 的链表节点，返回 **反转后的链表** 。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/02/19/rev2ex2.jpg)

```js
输入：head = [1,2,3,4,5], left = 2, right = 4
输出：[1,4,3,2,5]
```
**示例 2：**
```js
输入：head = [5], left = 1, right = 1
输出：[5]
```
**提示：**
- 链表中节点数目为 `n`
- `1 <= n <= 500`
- `-500 <= Node.val <= 500`
- `1 <= left <= right <= n`

**进阶：** 你可以使用一趟扫描完成反转吗？

---
### 解法 一次遍历「穿针引线」反转链表
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202309080018041.png)
下面具体解释如何实现。使用指针变量来记录反转的过程中需要的变量，它们的意义如下：
- `curr` ：指向**待反转区域的第一个节点**；
- `next` ：**永远指向 `curr` 的下一个节点**，循环过程中，`curr` 变化以后 `next` 会变化；
- `p0` ：永远指向待反转区域的第一个节点 `left` 的前一个节点，**在循环过程中不变**。
- `prev` ：永远指向**已反转区域的上个节点**，紧邻在 `curr` 左侧。

操作步骤：
1. 找到 `p0` ，令 `prev = nullptr, curr = p0->next` 。
2. 进入循环：
    1. 先将 `curr` 的下一个节点记录为 `next` ；
    2. 执行操作 ①：把 `curr.next` 指向上一个节点 `prev` ；
    3. 执行操作 ②：把 `prev` 指向当前节点 `curr` ；
    4. 执行操作 ③：把 `curr` 移动到下个节点 `next` 。
3.  超出待反转区域后，`p0->next` 指向已反转区域的尾部，设置 `p0->next->next` 为 `curr` ，再设置 `p0->next` 为 `prev` 。

```cpp
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        // 设置 dummy 是此类问题的一般做法
        ListNode* dummy = new ListNode(0, head), *p0 = dummy;
        for (int i = 0; i < left - 1; ++i) p0 = p0->next;

        ListNode *prev = nullptr, *curr = p0->next;
        for (int i = 0; i < right - left + 1; ++i) {
            ListNode *next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        p0->next->next = curr;
        p0->next = prev;
        return dummy->next;
    }
};
```
复杂度分析:
- 时间复杂度：$\mathcal{O}(n)$ ，其中 $n$ 为链表节点个数。
- 空间复杂度：$\mathcal{O}(1)$ ，仅用到若干额外变量。