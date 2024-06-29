
Given a linked list, rotate the list to the right by k places, where k is non-negative.

**Example 1:**
```swift
Input: 1->2->3->4->5->NULL, k = 2
Output: 4->5->1->2->3->NULL
Explanation:
rotate 1 steps to the right: 5->1->2->3->4->NULL
rotate 2 steps to the right: 4->5->1->2->3->NULL
```

**Example 2:**
```swift
Input: 0->1->2->NULL, k = 4
Output: 2->0->1->NULL
Explanation:
rotate 1 steps to the right: 2->0->1->NULL
rotate 2 steps to the right: 1->2->0->NULL
rotate 3 steps to the right: 0->1->2->NULL
rotate 4 steps to the right: 2->0->1->NULL
```

题意：给定一个链表，旋转链表，将链表每个节点向右移动 `k` 个位置，其中 `k` 是非负数。

---
### 解法 快慢指针+虚拟头结点
这个题目实际上是数组旋转在链表上的扩展。数组设为 `ab` ，要得到 `ba` ，需要 `rev(rev(a) + rev(b)) = rev(rev(b)) + rev(rev(a)) = ba` 。但是本题使用的是单链表，操作就一目了然：只需要把后部分 `k` 个结点组成的链表作为新链表的首部，把前部分 `n - k` 个结点组成的链表作为新链表的尾部。 

于是使用快慢指针+虚拟头结点，快慢指针从虚拟头结点出发，目的是让 `lo` 指向前 `n - k` 个结点组成的链表的尾部，`lo->next` 就是新链表的头部，`hi` 指向后 `k` 个结点组成的链表的尾部。然后就是简单的断链和链接操作。具体代码如下：
```cpp
class Solution {
private:
    int getLength(ListNode* head) { 
        int len = 0;
        while (head) {
            ++len;
            head = head->next;
        }
        return len;
    }
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (head == nullptr || head->next == nullptr || k == 0) return head;
        k %= getLength(head); 
        if (k == 0) return head; //不需要旋转
        ListNode dummy(0); dummy.next = head; //虚拟头结点
        ListNode *lo = &dummy, *hi = &dummy;
        for (int i = 0; i < k; ++i) hi = hi->next;
        while (hi->next) { //hi此时一定非空
            hi = hi->next;
            lo = lo->next;
        } //此时lo->next指向了后面的k个结点组成的链表,hi指向最后一个结点
        ListNode *newHead = lo->next;
        lo->next = nullptr;
        hi->next = dummy.next;
        return newHead;
    }
};
```
效率如下，$O(n)$ 时间，$O(1)$ 空间：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了94.67% 的用户
内存消耗：7.3 MB, 在所有 C++ 提交中击败了6.41% 的用户
```
