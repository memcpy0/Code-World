定义一个函数，输入一个链表的头节点，反转该链表并输出反转后链表的头节点。

 

**示例:**

```swift
输入: 1->2->3->4->5->NULL
输出: 5->4->3->2->1->NULL
```

 

**限制：** `0 <= 节点个数 <= 5000`

---
### 解法1 递归
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *newHead = reverseList(head->next);
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了94.61% 的用户
内存消耗：8.8 MB, 在所有 C++ 提交中击败了5.13% 的用户
```
---
### 解法2 迭代
```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *cur = head, *newHead = nullptr;
        while (cur) {
            ListNode *temp = cur->next;
            cur->next = newHead;
            newHead = cur;
            cur = temp;
        }
        return newHead;
    }
};
```
效率如下：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了94.61% 的用户
内存消耗：8.4 MB, 在所有 C++ 提交中击败了30.40% 的用户
```
