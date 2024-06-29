<p>Given a singly linked list <em>L</em>: <em>L</em><sub>0</sub>→<em>L</em><sub>1</sub>→…→<em>L</em><sub><em>n</em>-1</sub>→<em>L</em><sub>n</sub>,<br>
reorder it to: <em>L</em><sub>0</sub>→<em>L</em><sub><em>n</em></sub>→<em>L</em><sub>1</sub>→<em>L</em><sub><em>n</em>-1</sub>→<em>L</em><sub>2</sub>→<em>L</em><sub><em>n</em>-2</sub>→…</p>

<p>You may <strong>not</strong> modify the values in the list's nodes, only nodes itself may be changed.</p>

<p><strong>Example 1:</strong></p>

```swift
Given 1->2->3->4, reorder it to 1->4->2->3.
```
<p><strong>Example 2:</strong></p> 

```swift
Given 1->2->3->4->5, reorder it to 1->5->2->4->3.
```

题意：给定一个单链表&nbsp;<em>L</em>：<em>L</em><sub>0</sub>→<em>L</em><sub>1</sub>→…→<em>L</em><sub><em>n</em>-1</sub>→<em>L</em><sub>n ，</sub> 将其重新排列后变为： <em>L</em><sub>0</sub>→<em>L</em><sub><em>n</em></sub>→<em>L</em><sub>1</sub>→<em>L</em><sub><em>n</em>-1</sub>→<em>L</em><sub>2</sub>→<em>L</em><sub><em>n</em>-2</sub>→……注意，不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。</p>

 
---
### 解法1 双端队列+拷贝链表指针
C++的写法如下，复制链表指针，然后改变指针走向。**注意，最后新链表的尾结点的指针仍然指向原来的下一个结点，需要改为指向空**：
```cpp
class Solution {
public:
    void reorderList(ListNode* head) {
        if (head == nullptr) return;
        list<ListNode*> deque;
        while (head) {
            deque.push_back(head);
            head = head->next;
        }
        while (!deque.empty()) {
            if (head == nullptr) { //此时head指向空
                head = deque.front();
                deque.pop_front();
            } else {
                head->next = deque.front();
                head = head->next;
                deque.pop_front();
            } 
            if (!deque.empty()) {
                head->next = deque.back();
                deque.pop_back();
                head = head->next;
            }  
        }
        if (head != nullptr) head->next = nullptr;
    }
};
```
效率很低，不忍直视：
```cpp
执行用时：100 ms, 在所有 C++ 提交中击败了8.88% 的用户
内存消耗：20.9 MB, 在所有 C++ 提交中击败了5.04% 的用户
```
如果不使用 `list` 而使用 `vector` 的话，有下面的代码，写得更加简洁：
```cpp
class Solution {
public:
    void reorderList(ListNode* head) {
        if (head == nullptr) return;
        vector<ListNode*> v;
        while (head) {
            v.push_back(head);
            head = head->next;
        }
        int left = 0, right = v.size() - 1;
        while (left < right) {
            v[left]->next = v[right];
            v[right--]->next = v[++left];
        }
        v[left]->next = nullptr;
    }
};
```
效率稍微高一点：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了13.14% 的用户
内存消耗：18.8 MB, 在所有 C++ 提交中击败了5.04% 的用户
```
用Java写这一代码：
```java
class Solution {
    public void reorderList(ListNode head) {
        LinkedList<ListNode> deque = new LinkedList<>();
        ListNode cur = head;
        while (cur != null) {
            deque.addLast(cur);
            cur = cur.next;
        }
        while (!deque.isEmpty()) {
            if (cur == null) { //插入前半部分结点
                cur = deque.pollFirst();
            } else {
                cur.next = deque.pollFirst();
                cur = cur.next;
            }
            cur.next = deque.pollLast(); //插入链表尾部结点
            cur = cur.next;
        }
        if (cur != null) cur.next = null;
    }
}
```
效率如下，差别太大了：
```java
执行用时：3 ms, 在所有 Java 提交中击败了36.50% 的用户
内存消耗：41.8 MB, 在所有 Java 提交中击败了41.45% 的用户
```
---
### 解法2 快慢指针+交换
题目原意是希望交换，因此不用双端队列重新拷贝的方法，也可以完成这一道题。做法是：在原链表上操作，首先**快慢指针找到中点**，然后**拆成两个链表**，**将后面的链表反转**，接着**遍历两个链表**并把后面的结点插入前面的缝隙中。
```cpp
class Solution {
private:
    ListNode* reverseRecur(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *newHead = reverseRecur(head->next);
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
    ListNode* reverseIter(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *pre = nullptr, *post = head;
        while (post) {
            ListNode *temp = post->next;
            post->next = pre;
            pre = post;
            post = temp;
        }
        return pre;
    }
public:
    void reorderList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return;
        ListNode dummy(0); dummy.next = head;
        ListNode *lo = &dummy, *hi = &dummy;
        while (hi && hi->next) { //快慢指针找到链表的中点
            hi = hi->next->next;
            lo = lo->next;
        } //此时快慢指针将链表分为两段
        ListNode *postHead = lo->next;
        lo->next = nullptr; 
        postHead = reverseRecur(postHead);   
        ListNode *cur = head, *rear = postHead; 
        while (cur && rear) { //插入前端链表的缝隙
            ListNode *rearNext = rear->next, *curNext = cur->next;
            rear->next = curNext;
            cur->next = rear;
            rear = rearNext, cur = curNext;
        }
    }
};
```
虽然时间复杂度仍然是 $O(n)$ ，但是空间复杂度为 $O(1)$ ：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了43.59% 的用户
内存消耗：18.4 MB, 在所有 C++ 提交中击败了7.86% 的用户
```
