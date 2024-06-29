Given a sorted linked list, delete all duplicates such that each element appear only once.

- Example 1:
```
Input: 1->1->2
Output: 1->2
```
- Example 2:
```
Input: 1->1->2->3->3
Output: 1->2->3
```

题意：删除有序链表中的重复元素，在原链表上面进行。
方法：直接法，由于链表已经有序，因此将后续结点的值同当前结点比较，如果相等，就重复，修改当前结点的next使其指向后续结点的下一个结点。
```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) { 
        if (head == nullptr) return head;
        ListNode *p = head, *temp = head->next;
        while (temp) {
            if (temp->val == p->val) {
                 p->next = temp->next;
                 // free(temp); // 这里不能使用free释放内存
                 temp = p->next;
             } else {
                 p = temp;
                 temp = temp->next;
             }
         }
         return head;
    }
};
```
下面写的更加精简：
```cpp
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        ListNode *current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->val == current->next->val) 
                current->next = current->next->next;
            else 
                current = current->next;
        }
        return head;
     }
  };
  ```
