Remove all elements from a linked list of integers that have value val.

**Example:**
```cpp
Input:  1->2->6->3->4->5->6, val = 6
Output: 1->2->3->4->5
```

题意：从一个单链表中删除所有等于给定值的结点。

---
思路1：迭代删除+无虚拟头结点。

代码：
```cpp
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) { 
        if (head == nullptr) return nullptr;
        ListNode *pre = head, *cur = head;
        while (cur) {
            if (cur->val == val) { //首先检查头结点，如果结点值与val相等，那么把头指针后移
                if (cur == head) {
                    head = head->next;
                    delete cur;
                    pre = cur = head;
                } else {
                    pre->next = cur->next;
                    delete cur;
                    cur = pre->next;
                }
            }
            else {
                pre = cur; 
                cur = cur->next;
            }
        }
        return head;
     } 
};
```
上面的代码太繁琐，我们把对头结点的特判拿出来，写成一个循环，如果头结点的值等于要删除的值，就修改头指针，**直到头指针为空或者头结点的值不为目标值**。然后用一个 `pre` 指针就可以了，如果 `pre->next->val == val` ，就修改 `pre->next = pre->next->next` ，而不需要一个 `cur` 指针。具体代码如下：
```cpp
class Solution {
public:  
    ListNode *removeElements(ListNode *head, int val) {
        while (head && head->val == val) {
            ListNode *delNode = head;
            head = head->next;
            delete delNode;
        }
        //头指针可能为空
        if (head == nullptr) return nullptr;
        //头指针不为空且值不等于val
        ListNode *pre = head;
        while (pre->next) {
            if (pre->next->val == val) {
                ListNode *delNode = pre->next;
                pre->next = pre->next->next;   //删除时pre不动
                delete delNode;
            } else pre = pre->next;            //不删除时pre向后移动
        }
        return head;
    }
};
```
效率：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了58.18% 的用户
内存消耗：15.2 MB, 在所有 C++ 提交中击败了98.31% 的用户
```
如果使用的是Java那种有GC的语言，可以写得更加简洁。在OJ上，C++可以不进行实际的 `delete` ，代码如下：
```cpp
class Solution {
public:  
    ListNode *removeElements(ListNode *head, int val) {
        while (head && head->val == val) 
            head = head->next; 
        //头指针可能为空
        if (head == nullptr) return nullptr;
        //头指针不为空且值不等于val
        ListNode *pre = head;
        while (pre->next) {
            if (pre->next->val == val) pre->next = pre->next->next;   //删除时pre不动 
            else pre = pre->next;            //不删除时pre向后移动
        }
        return head;
    }
};
```

---
思路2：迭代+虚拟头结点。上面的代码中，为了处理 `head` 的特殊情况，需要避开好些陷阱，而且代码也不够优雅简练。为此，我们建立一个虚拟头结点，统一所有的删除情况。

代码如下：
```cpp 
class Solution {
public:  
    ListNode* removeElements(ListNode* head, int val) {  
        if (head == nullptr) return nullptr; 
        ListNode *dummyHead = new ListNode(-1), *pre = dummyHead; 
        dummyHead->next = head; //有了一个虚拟头结点 
        while (pre->next) { 	//检查下一个结点是否是val
            if (pre->next->val == val) { 
                ListNode *delNode = pre->next;
                pre->next = pre->next->next; //连pre都不需要了
                delete delNode;
            } else pre = pre->next;
        }
        head = dummyHead->next; 
        delete dummyHead; //new的对象记得删除
        return head;
    }
};
```

---
思路3：递归删除。在归来的阶段进行删除，是更加优雅简练的做法。如果当前的头结点的值等于 `val` ，就返回 `head->next` 作为新的头指针。代码如下：
```cpp
class Solution {
public:  
    ListNode *removeElements(ListNode *head, int val) {
        if (!head) return head;
        //删除要求的结点, 可能会改变某些结点的next
        head->next = removeElements(head->next, val);
        if (head->val == val) {
            ListNode *newHead = head->next;
            delete head;
            return newHead;
        }
        return head;
    } 
};
```
或者说，如果当前结点的值等于 `val` ，就以递归返回的头指针作为新的头指针，否则，递归返回的头指针作为 `head->next` ：
```cpp
  ListNode* removeElements(ListNode* head, int val) { 
        if (head == nullptr) return nullptr;
        if (head->val == val) head = removeElements(head->next, val);
        else head->next = removeElements(head->next, val);
        return head;
    }
};
```
基本上，使用递归只需 `4` 行代码就可以删除所有值等于 `val` 的链表结点。
