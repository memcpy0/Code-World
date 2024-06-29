

Given a non-empty, singly linked list with head node `head`, return a middle node of linked list.

If there are two middle nodes, return the second middle node.

 

**Example 1:**

```swift
Input: [1,2,3,4,5]
Output: Node 3 from this list (Serialization: [3,4,5])
The returned node has value 3.  (The judge's serialization of this node is [3,4,5]).
Note that we returned a ListNode object ans, such that:
ans.val = 3, ans.next.val = 4, ans.next.next.val = 5, and ans.next.next.next = NULL.
```

**Example 2:**

```swift
Input: [1,2,3,4,5,6]
Output: Node 4 from this list (Serialization: [4,5,6])
Since the list has two middle nodes with values 3 and 4, we return the second one.
```
**Note:**  The number of nodes in the given list will be between `1` and `100` .


题意：给定一个非空单链表，返回链表的中间结点。如果有两个中间结点，则返回第二个中间结点。

---
### 解法 快慢指针
```cpp
class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        ListNode *lo = head, *hi = head;
        while (hi && hi->next) {
            hi = hi->next->next;
            lo = lo->next;
        }
        return lo;
    }
};
```
效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：7 MB, 在所有 C++ 提交中击败了5.03% 的用户
```
