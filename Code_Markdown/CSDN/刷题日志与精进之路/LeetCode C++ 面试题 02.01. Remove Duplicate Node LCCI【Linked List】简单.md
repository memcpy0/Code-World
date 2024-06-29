Write code to remove duplicates from an **unsorted** linked list.

Example1:

```cpp
 Input: [1, 2, 3, 3, 2, 1]
 Output: [1, 2, 3]
```

Example2:

```cpp
 Input: [1, 1, 1, 1, 2]
 Output: [1, 2]
```

**Note**:

-    The length of the list is within the range `[0, 20000]`.
-    The values of the list elements are within the range `[0, 20000]`.

**Follow Up**: How would you solve this problem if a temporary buffer is not allowed?

题意：将无序链表去重。

思路：如果不使用额外空间的话，$\text{O(n}^2)$ 。无法忍受。由于链表的结点值有范围，可以用散列表判断值是否重复。

可以 ① 在原链表的基础上进行修改，也可以 ② 重新创建一个无重复的链表。

代码①：
```cpp 
class Solution {
public:
    ListNode* removeDuplicateNodes(ListNode* head) {
        if (head == NULL || head->next == NULL) return head;
        vector<bool> hashTable(20010, false);
        ListNode *pre = head, *cur = head->next; //表头不重复
        hashTable[pre->val] = true;
        while (cur) {
            if (hashTable[cur->val] == false) {
                hashTable[cur->val] = true;
                pre = cur;
                cur = cur->next;
            } else {
                pre->next = cur->next;  //修改指针,没有实际free
                cur = pre->next;
            } 
        }
        return head;
    }
};
```

代码②：
```cpp  
class Solution {
public:
    ListNode* removeDuplicateNodes(ListNode* head) {
        vector<bool> hashTable(20010, false);
        ListNode *l = new ListNode(-1), *r = l, *p = head;
        while (p) {
            if (hashTable[p->val] == false) {
                r->next = new ListNode(p->val);
                r = r->next;
                hashTable[p->val] = true;
            }
            p = p->next;
        }
        return l->next;
    }
};
```
代码①的效率更好，因为删除结点的时候仅修改了指针，而代码②需要实际创建结点。
