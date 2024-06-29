Reverse a **singly** linked list.

Example:
```
Input: 1->2->3->4->5->NULL
Output: 5->4->3->2->1->NULL
```
Follow up:
- A linked list can be reversed either **iteratively or recursively.** Could you implement both?

题意：反转一条单链表。可以递归，也可以迭代。

### 解题方法一：迭代（头插法）
反转单链表，相当于从原来的单链表开始，**依次将结点从新链表的头部插入**，即头插法。
-  执行用时 :8 ms, 在所有 C++ 提交中击败了80.47% 的用户
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
    ListNode* reverseList(ListNode* head) { //iteratively reversed 头插法
        if (head == NULL || head->next == NULL) 
            return head; //空表/一个元素直接返回 
        ListNode *newHead = new ListNode(0); //新链表头结点不动, head为原链表表头指针
        while (head) { 
            ListNode *t = head->next;
            head->next = newHead->next; //cur指向原来的首结点
            newHead->next = head; //cur指向的结点变成新的首结点
            head = t;
        }
        return newHead->next;
    } 
};
```
这里是没有头结点的新单链表的写法，改了一下：
```cpp
class Solution {
public: 
    ListNode* reverseList(ListNode* head) { //iteratively reversed 
        if (head == NULL || head->next == NULL) 
            return head; //空表/一个元素直接返回 
        ListNode *newHead = NULL; //新链表头指针, 原链表表头
        while (head) { //新链表的头指针不断变化, 直到指向原链表的尾结点
            ListNode *t = head->next; //保存指向原链表部分的指针
            head->next = newHead; //这两句把原链表的表头结点变成了新链表的表头结点
            newHead = head;
            head = t; //处理下一个原链表的表头结点
        }
        return newHead;
    }  
};
```
### 解题方法二：递归
只需要找到递归的边界，返回指向原链表的尾部（即新链表的头结点）的指针；然后依次从后往前逆转结点即可。[单链表反转](https://blog.csdn.net/myRealization/article/details/103864256)中写的差不多。
- 执行用时 :8 ms, 在所有 C++ 提交中击败了80.47% 的用户
```cpp 
class Solution {
public: 
    ListNode* reverseList(ListNode* head) { //recursively reversed 
        if (head == NULL || head->next == NULL) //递归边界
            return head; //空表/一个元素直接返回  
        ListNode *newHead = reverseList(head->next); //反转链表的头结点, 不变
        head->next->next = head; //反转链表扩展新结点
        head->next = NULL; //反转链表的结束
        return newHead;
    }
};
```
