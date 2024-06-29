> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091348039.png)
给你链表的头节点 `head` ，每 `k` 个节点一组进行翻转，请你返回修改后的链表。

`k` 是一个正整数，它的值小于或等于链表的长度。如果节点总数不是 `k` 的整数倍，那么请将最后剩余的节点保持原有顺序。

你不能只是单纯的改变节点内部的值，而是需要实际进行节点交换。

**示例 1：**
![](https://assets.leetcode.com/uploads/2020/10/03/reverse_ex1.jpg)

```java
输入：head = [1,2,3,4,5], k = 2
输出：[2,1,4,3,5]
```
**示例 2：**
![](https://assets.leetcode.com/uploads/2020/10/03/reverse_ex2.jpg)
```java
输入：head = [1,2,3,4,5], k = 3
输出：[3,2,1,4,5]
```
**提示：**
- 链表中的节点数目为 `n`
- `1 <= k <= n <= 5000`
- `0 <= Node.val <= 1000`

**进阶：** 你可以设计一个只用 `O(1)` 额外内存空间的算法解决此问题吗？
 
---
### 解法1 额外空间
使用辅助的栈来帮助翻转，注意**保持剩余结点的顺序**：
```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *newHead = nullptr, *temp = head;
        stack<ListNode*> st;
        vector<ListNode*> rev;
        for (ListNode*cur = head; cur; cur = temp) {
            st.push(cur);
            temp = cur->next; //指向下一个结点的指针
            if (st.size() >= k) {
                while (!st.empty()) {
                    ListNode *t = st.top(); st.pop();
                    if (!rev.empty()) rev.back()->next = t;
                    rev.push_back(t);
                }
                rev.back()->next = nullptr; //断开这一段尾结点的原链接
            }
        }
        vector<ListNode*> last;
        while (!st.empty()) { //剩下少于k个结点
            last.push_back(st.top());
            st.pop();
        }
        while (!last.empty()) { //少于k个结点不进行翻转
            rev.back()->next = last.back();
            rev.push_back(last.back());
            last.pop_back();
        }
        return rev.front(); //新的头结点指针
    }
};
```
提交后运行效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了29.02% 的用户
内存消耗：12.6 MB, 在所有 C++ 提交中击败了5.22% 的用户
```
更进一步，`vector<ListNode*> rev` 的空间可以被省略：
```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *newHead = nullptr, *rear = nullptr, *temp = head;
        stack<ListNode*> st; 
        for (ListNode*cur = head; cur; cur = temp) {
            st.push(cur);
            temp = cur->next; 
            if (st.size() >= k) {
                while (!st.empty()) {
                    ListNode *t = st.top(); st.pop();
                    if (newHead == nullptr) newHead = rear = t;
                    else { rear->next = t; rear = rear->next; }
                }
                rear->next = nullptr; //断开这一段尾结点的原链接
            }
        }
        vector<ListNode*> last;
        while (!st.empty()) { //剩下少于k个结点
            last.push_back(st.top());
            st.pop();
        }
        while (!last.empty()) { //少于k个结点不进行翻转
            rear->next = last.back();
            rear = rear->next;
            last.pop_back();
        }
        return newHead;
    }
};
```
运行的时间效率提高了-_-||：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了89.64% 的用户
内存消耗：12.1 MB, 在所有 C++ 提交中击败了5.22% 的用户
```
---
### 解法2 常量空间
日后有时间就更新相应的代码。

虚拟头节点：（最优写法！）
```java
class Solution {
    public static ListNode reverseKGroup(ListNode head, int k) {
        ListNode dummy = new ListNode(-1, head), prev = dummy;
        while (true) {
            // 检查剩余节点是否有k个，不足则返回
            ListNode last = prev;
            for (int i = 0; i < k; i++) {
                last = last.next;
                if (last == null) {
                    return dummy.next;
                }
            }

            // 翻转k个节点
            ListNode curr = prev.next, next;
            for (int i = 0; i < k - 1; i++) {
                next = curr.next;
                curr.next = next.next;
                
                next.next = prev.next; // 头插法
                prev.next = next;
            }
            prev = curr;
        }
    }
}
```

节点数组都用到了，亲测简单头插法实现反转.


思路与算法

本题的目标非常清晰易懂，不涉及复杂的算法，但是实现过程中需要考虑的细节比较多，容易写出冗长的代码。主要考查面试者设计的能力。

我们需要把链表节点按照 k 个一组分组，所以可以使用一个指针 head 依次指向每组的头节点。这个指针每次向前移动 k 步，直至链表结尾。对于每个分组，我们先判断它的长度是否大于等于 k。若是，我们就翻转这部分链表，否则不需要翻转。

接下来的问题就是如何翻转一个分组内的子链表。翻转一个链表并不难，过程可以参考「[206. 反转链表](https://leetcode-cn.com/problems/reverse-linked-list/)」。但是对于一个子链表，除了翻转其本身之外，还需要将子链表的头部与上一个子链表连接，以及子链表的尾部与下一个子链表连接。如下图所示：


![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307032154753.png)

因此，在翻转子链表的时候，我们不仅需要子链表头节点 head，还需要有 head 的上一个节点 pre，以便翻转完后把子链表再接回 pre。

但是对于第一个子链表，它的头节点 head 前面是没有节点 pre 的。太麻烦了！难道只能特判了吗？答案是否定的。没有条件，我们就创造条件；没有节点，我们就创建一个节点。我们新建一个节点，把它接到链表的头部，让它作为 pre 的初始值，这样 head 前面就有了一个节点，我们就可以避开链表头部的边界条件。这么做还有一个好处，下面我们会看到。

反复移动指针 head 与 pre，对 head 所指向的子链表进行翻转，直到结尾，我们就得到了答案。下面我们该返回函数值了。

有的同学可能发现这又是一件麻烦事：链表翻转之后，链表的头节点发生了变化，那么应该返回哪个节点呢？照理来说，前 k 个节点翻转之后，链表的头节点应该是第 k 个节点。那么要在遍历过程中记录第 k 个节点吗？但是如果链表里面没有 k 个节点，答案又还是原来的头节点。我们又多了一大堆循环和判断要写，太崩溃了！

等等！还记得我们创建了节点 pre 吗？这个节点一开始被连接到了头节点的前面，而无论之后链表有没有翻转，它的 next 指针都会指向正确的头节点。那么我们只要返回它的下一个节点就好了。至此，问题解决。


1 / 19
Python3
C++
Java
JavaScript
Golang
class Solution {
public:
    // 翻转一个子链表，并且返回新的头与尾
    pair<ListNode*, ListNode*> myReverse(ListNode* head, ListNode* tail) {
        ListNode* prev = tail->next;
        ListNode* p = head;
        while (prev != tail) {
            ListNode* nex = p->next;
            p->next = prev;
            prev = p;
            p = nex;
        }
        return {tail, head};
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* hair = new ListNode(0);
        hair->next = head;
        ListNode* pre = hair;

        while (head) {
            ListNode* tail = pre;
            // 查看剩余部分长度是否大于等于 k
            for (int i = 0; i < k; ++i) {
                tail = tail->next;
                if (!tail) {
                    return hair->next;
                }
            }
            ListNode* nex = tail->next;
            // 这里是 C++17 的写法，也可以写成
            // pair<ListNode*, ListNode*> result = myReverse(head, tail);
            // head = result.first;
            // tail = result.second;
            tie(head, tail) = myReverse(head, tail);
            // 把子链表重新接回原链表
            pre->next = head;
            tail->next = nex;
            pre = tail;
            head = tail->next;
        }

        return hair->next;
    }
};
复杂度分析

时间复杂度：O(n)O(n)O(n)，其中 nnn 为链表的长度。head 指针会在 O(⌊nk⌋)O(\lfloor \frac{n}{k} \rfloor)O(⌊ 
k
n
​
 ⌋) 个节点上停留，每次停留需要进行一次 O(k)O(k)O(k) 的翻转操作。

空间复杂度：O(1)O(1)O(1)，我们只需要建立常数个变量。

 