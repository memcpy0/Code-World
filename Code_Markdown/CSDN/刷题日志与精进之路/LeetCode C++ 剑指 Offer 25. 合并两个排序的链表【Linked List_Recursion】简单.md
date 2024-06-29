输入两个递增排序的链表，合并这两个链表并使新链表中的节点仍然是递增排序的。

**示例1：**

```swift
输入：1->2->4, 1->3->4
输出：1->1->2->3->4->4
```

**限制：** `0 <= 链表长度 <= 1000`

---
### 解法1 迭代归并
```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1 || !l2) return l1 ? l1 : l2;
        ListNode dummy(0), *rear = &dummy;
        while (l1 && l2) {
            if (l1->val <= l2->val) {
                rear->next = l1;
                l1 = l1->next;
            } else {
                rear->next = l2;
                l2 = l2->next;
            }
            rear = rear->next;
        }
        if (l1) rear->next = l1;
        if (l2) rear->next = l2;
        return dummy.next;
    }
};
```
提交后效率如下：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了96.19% 的用户
内存消耗：19.2 MB, 在所有 C++ 提交中击败了29.37% 的用户
```
---
### 解法2 递归归并
```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1->val <= l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        } 
    }
};
```
提交后效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了71.76% 的用户
内存消耗：19.2 MB, 在所有 C++ 提交中击败了24.54% 的用户
```
