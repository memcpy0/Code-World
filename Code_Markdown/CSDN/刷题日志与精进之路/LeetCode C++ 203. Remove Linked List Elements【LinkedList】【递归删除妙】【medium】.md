

Remove **all** elements from a linked list of integers that have value **val**.

Example:
```
Input:  1->2->6->3->4->5->6, val = 6
Output: 1->2->3->4->5
```
题意：删除单链表中所有值为val的结点。

思路：这一题是单链表，无头结点。可以有几种方法。

### 解决方法一：分别处理头部结点和表中结点
由于单链表无头结点，因此删除头指针指向的结点时需要特殊处理：
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
    ListNode* removeElements(ListNode* head, int val) {
        if (head == NULL) return NULL;
        ListNode *pre = head, *cur = head;
        while (cur) {
            if (cur->val == val) { 
                if (cur == head) { //首先检查头指针，如果结点值与val相等，那么把头指针后移
                    head = head->next; //改变头指针
                    delete cur;		   //不能用free!!!
                    pre = cur = head;
                }
                else {
                    pre->next = cur->next; 
                    delete cur;
                    cur = pre->next;
                }
            }
            else {
                pre = cur; //继续移动
                cur = cur->next;
            }
        }
        return head; 
    }
}; 
```
还有一个小的写法，直接外面**先while循环检查一遍头部的结点**，如果是val值就直接删除。然后再来一个**while循环检查表内结点**。 不过差别不大。

### 解决方法二：创建一个虚拟头结点
**由于有个无意义节点作为头结点会统一操作**（把头结点当做普通节点），所以实际链表设计过程中都是有个无意义头结点的。**遇到头指针不好解决的问题，大家可以设一个节点试试**。 最后返回H->next就满足条件了。
- 执行用时 :24 ms, 在所有 C++ 提交中击败了77.75% 的用户
```cpp
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) {
        if (head == NULL) return NULL; 
        ListNode *tempHead = new ListNode(-1), *cur = tempHead; 
        tempHead->next = head; //有了一个虚拟头结点
   
        while (cur->next) { //检查下一个结点是否是val
            if (cur->next->val == val) { 
                ListNode *p = cur->next;
                cur->next = cur->next->next; //连pre都不需要了
                delete p;
            } else cur = cur->next;
        }
        head = tempHead->next; 
        delete tempHead; //new的对象记得删除
        return head;
    }
}; 
```
### ★★ 解决方法三：递归删除
链表作为天然的递归数据结构，很适合用递归解决问题。但是递归需要压栈，消耗一定的空间。
(1) 递归边界：链表为空，返回NULL；
(2) 由于要删除要求的结点，会改变某些结点的next，因此写成下面的形式；
(3) 先不断递归到链表尾部，然后在“归来”的过程中，如果是val，就删除当前head，返回head->next；不然直接返回head。
- 执行用时 :32 ms, 在所有 C++ 提交中击败了53.68% 的用户
```cpp
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) {  
        if (head == NULL) return head;
        head->next = removeElements(head->next, val);  
        if (head->val == val) { //如果头结点是val值
            ListNode *t = head->next;
            delete head;
            return t;
        } 
        return head;
};
```
### 总结
这一题也是链表的修改（删除），和第19题很像，比如**使用虚假头结点的想法、递归修改链表的形式等技巧**。[LeetCode C++ 19. Remove Nth Node From End of List【LinkedList】【medium】]()
