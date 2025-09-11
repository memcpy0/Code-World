

Given a singly linked list, group all odd nodes together followed by the even nodes. Please note here we are talking about the node number and **not the value** in the nodes.

You should try to do it in place. The program should run in `O(1)` space complexity and `O(nodes)` time complexity.

**Example 1:**

```swift
Input: 1->2->3->4->5->NULL
Output: 1->3->5->2->4->NULL
```

**Example 2:**

```swift
Input: 2->1->3->5->6->4->7->NULL
Output: 2->3->6->7->1->5->4->NULL
```

**Constraints:**

- The relative order inside both the even and odd groups should remain as it was in the input.
- The first node is considered odd, the second node even and so on ...
- The length of the linked list is between `[0, 10^4]` .

题意：给定一个单链表，把所有的奇数编号节点和偶数编号节点分别排在一起。使用原地算法完成，空间复杂度应为 $O(1)$ ，时间复杂度应为 $O(n)$ ，$n$ 为节点总数。

---
### 解法 顺序遍历
使用两个虚拟头结点，分别代表奇数编号链表的头部和偶数编号链表的头部，然后是两个用来进行尾插法的指针；同时使用 `for` 循环计算编号：
```cpp
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode edum(0), odum(0); //两个虚拟头结点,便于管理
        ListNode *ep = &edum, *op = &odum; //用于尾部插入
        ListNode *cur = head;
        for (int i = 1; cur; ++i) {
            if (i & 1) {
                op->next = cur;
                op = op->next;
            } else {
                ep->next = cur;
                ep = ep->next;
            }
            cur = cur->next;
        }
        op->next = ep->next = nullptr; //断掉可能的连接
        op->next = edum.next;
        return odum.next;
    }
};
```
提交后效率如下：
```cpp
执行用时：16 ms, 在所有 C++ 提交中击败了77.66% 的用户
内存消耗：10.8 MB, 在所有 C++ 提交中击败了5.16% 的用户
```
不使用虚拟头结点和 `for` 循环，更加简洁而清晰的代码如下：
```cpp
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head; 
        ListNode *e = head->next; //head为奇数编号链表首结点, e为偶数编号链表头结点
        ListNode *p = head, *q = head->next; //p为奇数编号链表尾结点,q为偶数编号链表尾结点
        while (p->next && q->next) { //p存在但p->next不存在时退出,此时q为空结点(1->2->3); p->next存在但q->next不存在时退出(1->2->3->4),此时q非空结点
            p->next = q->next; //q->next存在
            p = p->next;       //所以p也存在
            q->next = p->next; //此时p->next不一定存在
            q = q->next;       
        }
        p->next = e; //奇数编号链表尾部后+偶数编号链表
        return head; 
    }
};
```
提交后运行结果如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了45.44% 的用户
内存消耗：10.7 MB, 在所有 C++ 提交中击败了9.36% 的用户
```
