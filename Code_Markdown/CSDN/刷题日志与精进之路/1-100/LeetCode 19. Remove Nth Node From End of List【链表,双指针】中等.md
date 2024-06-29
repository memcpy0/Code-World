> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091345871.png)

Given the `head` of a linked list, remove the <code>n<sup>th</sup></code> node from the end of the list and return its head.

**Follow up:** Could you do this in one pass?

**Example 1:**
<img src="https://img-blog.csdnimg.cn/20201018095345284.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="45%">
```swift
Input: head = [1,2,3,4,5], n = 2
Output: [1,2,3,5]
```
**Example 2:**
```swift
Input: head = [1], n = 1
Output: []
```
**Example 3:**
```swift
Input: head = [1,2], n = 1
Output: [1]
```
**Constraints:**
- The number of nodes in the list is `sz` .
- `1 <= sz <= 30`
- `0 <= Node.val <= 100`
- `1 <= n <= sz`

**进阶：** 你能尝试使用一趟扫描实现吗？

题意：给定一个链表，删除链表的倒数第 `n` 个节点，并且返回链表的头结点。

---
前言
在对链表进行操作时，一种常用的技巧是添加一个哑节点（dummy node），它的 next\textit{next}next 指针指向链表的头节点。这样一来，我们就不需要对头节点进行特殊的判断了。

例如，在本题中，如果我们要删除节点 yyy，我们需要知道节点 yyy 的前驱节点 xxx，并将 xxx 的指针指向 yyy 的后继节点。但由于头节点不存在前驱节点，因此我们需要在删除头节点时进行特殊判断。但如果我们添加了哑节点，那么头节点的前驱节点就是哑节点本身，此时我们就只需要考虑通用的情况即可。

特别地，在某些语言中，由于需要自行对内存进行管理。因此在实际的面试中，对于「是否需要释放被删除节点对应的空间」这一问题，我们需要和面试官进行积极的沟通以达成一致。下面的代码中默认不释放空间。

方法一：计算链表长度
思路与算法

一种容易想到的方法是，我们首先从头节点开始对链表进行一次遍历，得到链表的长度 LLL。随后我们再从头节点开始对链表进行一次遍历，当遍历到第 L−n+1L-n+1L−n+1 个节点时，它就是我们需要删除的节点。

为了与题目中的 nnn 保持一致，节点的编号从 111 开始，头节点为编号 111 的节点。

为了方便删除操作，我们可以从哑节点开始遍历 L−n+1L-n+1L−n+1 个节点。当遍历到第 L−n+1L-n+1L−n+1 个节点时，它的下一个节点就是我们需要删除的节点，这样我们只需要修改一次指针，就能完成删除操作。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307022343845.png)



代码

C++
Java
Python3
Golang
C
class Solution {
public:
    int getLength(ListNode* head) {
        int length = 0;
        while (head) {
            ++length;
            head = head->next;
        }
        return length;
    }

    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* dummy = new ListNode(0, head);
        int length = getLength(head);
        ListNode* cur = dummy;
        for (int i = 1; i < length - n + 1; ++i) {
            cur = cur->next;
        }
        cur->next = cur->next->next;
        ListNode* ans = dummy->next;
        delete dummy;
        return ans;
    }
};
复杂度分析

时间复杂度：O(L)O(L)O(L)，其中 LLL 是链表的长度。

空间复杂度：O(1)O(1)O(1)。

方法二：栈
思路与算法

我们也可以在遍历链表的同时将所有节点依次入栈。根据栈「先进后出」的原则，我们弹出栈的第 nnn 个节点就是需要删除的节点，并且目前栈顶的节点就是待删除节点的前驱节点。这样一来，删除操作就变得十分方便了。


1 / 10
代码

C++
Java
Python3
Golang
C
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* dummy = new ListNode(0, head);
        stack<ListNode*> stk;
        ListNode* cur = dummy;
        while (cur) {
            stk.push(cur);
            cur = cur->next;
        }
        for (int i = 0; i < n; ++i) {
            stk.pop();
        }
        ListNode* prev = stk.top();
        prev->next = prev->next->next;
        ListNode* ans = dummy->next;
        delete dummy;
        return ans;
    }
};
复杂度分析

时间复杂度：O(L)O(L)O(L)，其中 LLL 是链表的长度。

空间复杂度：O(L)O(L)O(L)，其中 LLL 是链表的长度。主要为栈的开销。

方法三：双指针
思路与算法

我们也可以在不预处理出链表的长度，以及使用常数空间的前提下解决本题。

由于我们需要找到倒数第 nnn 个节点，因此我们可以使用两个指针 first\textit{first}first 和 second\textit{second}second 同时对链表进行遍历，并且 first\textit{first}first 比 second\textit{second}second 超前 nnn 个节点。当 first\textit{first}first 遍历到链表的末尾时，second\textit{second}second 就恰好处于倒数第 nnn 个节点。

具体地，初始时 first\textit{first}first 和 second\textit{second}second 均指向头节点。我们首先使用 first\textit{first}first 对链表进行遍历，遍历的次数为 nnn。此时，first\textit{first}first 和 second\textit{second}second 之间间隔了 n−1n-1n−1 个节点，即 first\textit{first}first 比 second\textit{second}second 超前了 nnn 个节点。

在这之后，我们同时使用 first\textit{first}first 和 second\textit{second}second 对链表进行遍历。当 first\textit{first}first 遍历到链表的末尾（即 first\textit{first}first 为空指针）时，second\textit{second}second 恰好指向倒数第 nnn 个节点。

根据方法一和方法二，如果我们能够得到的是倒数第 nnn 个节点的前驱节点而不是倒数第 nnn 个节点的话，删除操作会更加方便。因此我们可以考虑在初始时将 second\textit{second}second 指向哑节点，其余的操作步骤不变。这样一来，当 first\textit{first}first 遍历到链表的末尾时，second\textit{second}second 的下一个节点就是我们需要删除的节点。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307022344218.png)

### 解法1 迭代（快慢指针）
由于 `n` 一定有效，所以不用判断范围越界。另外，从一个非空无头结点链表的首元素结点一直访问到 `nullptr` ，需要走 `k` 步（每一次 `p = p->next` 算作一步）。于是快指针先走 `n` 步，离 `nullptr` 还有 `k - n` 步，倒数第 `n (n >= 1)` 个结点实际上就是顺数第 `k - n + 1` 个结点，走 `k - n` 步就可以从首元素结点到顺数第 `k - n + 1` 个结点。

为了删除顺数第 `k - n + 1` 个结点，需要找到第 `k - n` 个结点，于是慢指针和快指针一起走 `k - n - 1` 步，此后快指针指向链表尾结点，慢指针指向要删除结点的前一个结点，然后删除即可。

**一个特殊情况是**，如果要删除首元素结点，即顺数第 `1` 个、倒数第 `n = k` 个结点，则快指针会直接指向 `nullptr` ，离空指针 `0` 步，慢指针接着走 `0` 步到达要删除的结点，此时直接返回 `head->next` 即可。具体代码如下：
```cpp
class Solution {
public:  
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *lo = head, *hi = head;
        while (n--) hi = hi->next;
        while (hi && hi->next) {
            hi = hi->next;
            lo = lo->next;
        }
        if (hi == nullptr) return head->next; 
        lo->next = lo->next->next; 
        return head;
    } 
};
```
提交后效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了94.49% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了5.05% 的用户
```

```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *dummy = new ListNode(0, head);
        ListNode *left = dummy, *right = dummy;
        while (n--) right = right->next;
        while (right->next) {
            left = left->next;
            right = right->next;
        }
        left->next = left->next->next;
        return dummy->next;
    }
};
```
**复杂度分析**

- 时间复杂度：O(L)O(L)O(L)，其中 LLL 是链表的长度。
    
- 空间复杂度：O(1)O(1)O(1)。

---

思路1：先遍历一遍计算链表长度 `L` ；再遍历一遍删除倒数第 `N` 个结点或者正数第 `L - N + 1` 个结点，此时需找到第 `L - N` 个结点，它是倒数第 `N` 个结点的前一个结点。不过这里遍历了两次链表，效率略低。

代码如下：
```cpp
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *dummy = new ListNode(0);  //虚拟头结点
        dummy->next = head;
        int len = 0;  //计算链表长度
        ListNode *cur = dummy->next, *pre = dummy;
        while (cur) {
            ++len;
            cur = cur->next;
        } 
        int kthFromBegin = len - n + 1; //正数第k个,倒数第n个
        for (int i = 1; i < kthFromBegin; ++i)
            pre = pre->next;
        ListNode *delNode = pre->next;
        pre->next = pre->next->next;
        delete delNode;
        cur = dummy->next;
        delete dummy;
        return cur;
    }
};
```

---
思路2：快慢指针。

---
思路3：递归删除。
