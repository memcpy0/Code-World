> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你单链表的头节点 `head` ，请你反转链表，并返回反转后的链表。

**示例 1：**
![](https://assets.leetcode.com/uploads/2021/02/19/rev1ex1.jpg)

```js
输入：head = [1,2,3,4,5]
输出：[5,4,3,2,1]
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2021/02/19/rev1ex2.jpg)

```js
输入：head = [1,2]
输出：[2,1]
```
**示例 3：**
```js
输入：head = []
输出：[]
```
**提示：**
- 链表中节点的数目范围是 `[0, 5000]`
- `-5000 <= Node.val <= 5000`

**进阶：** 链表可以选用迭代或递归方式完成反转。你能否用两种方法解决这道题。

---
### 解法 迭代
假设链表为 $1 \rightarrow 2 \rightarrow 3 \rightarrow \varnothing$ ，我们想要把它改成 $\varnothing \leftarrow 1 \leftarrow 2 \leftarrow 3$ 。

在遍历链表时，将当前节点的 $\textit{next}$ 指针改为**指向前一个节点**。==由于节点没有引用其前一个节点，因此必须事先存储其前一个节点==。在更改引用之前，==还需要存储后一个节点==。最后返回新的头引用。
```cpp
class Solution {
public:
    ListNode *reverseList(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *newHead = nullptr, *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = newHead;
            newHead = cur;
            cur = next;
        }
        return newHead;
    }
};
```
**复杂度分析**：
- 时间复杂度：$O(n)$ ，其中 $n$ 是链表的长度。需要遍历链表一次。
- 空间复杂度：$O(1)$ 。

---
### 解法2 递归
递归版本稍微复杂一些，其关键在于反向工作。假设链表的其余部分已经被反转，现在应该如何反转它前面的部分？

假设链表为： $$n_1\rightarrow \ldots \rightarrow n_{k-1} \rightarrow n_k \rightarrow n_{k+1} \rightarrow \ldots \rightarrow n_m \rightarrow \varnothing$$
若从节点 $n_{k+1}$ 到 $n_m$ 已经被反转，而我们正处于 $n_k$ 。
$$n_1\rightarrow \ldots \rightarrow n_{k-1} \rightarrow n_k \rightarrow n_{k+1} \leftarrow \ldots \leftarrow n_m$$
我们希望 $n_{k+1}$ 的下一个节点指向 $n_k$ 。

所以，$n_k.\textit{next}.\textit{next} = n_k$ 。

需要注意的是 **$n_1$ 的下一个节点必须指向 $\varnothing$** 。如果忽略了这一点，链表中可能会产生环。
```cpp
class Solution {
public:
    ListNode *reverseList(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *newHead = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是链表的长度。需要对链表的每个节点进行反转操作。
- 空间复杂度：$O(n)$ ，其中 $n$ 是链表的长度。空间复杂度主要取决于递归调用的栈空间，最多为 $n$ 层。