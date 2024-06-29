@[toc]
Remove **all elements** from a linked list of **integers** that have value val.

Example:
```
Input:  1->2->6->3->4->5->6, val = 6
Output: 1->2->3->4->5
```

题意：从给出的单链表中**删除所有值**为$val$的结点。

思路：平常我们写链表的算法时，写的删除函数往往是删除某个位序$index$上的元素；而且往往只删除一个元素。因此，我们需要小心一些。

### 写法一 统一处理
一起手，我写的代码如下。由于需要特判头结点，而且加上删除实际结点的代码，整个代码又臭又长。

就是考虑到了，删除结点需要一个pre指针，还需要一个cur指针，都初始化为$head$头指针。由于全部用一个循环包括进去了，我们**需要在pre指针的变化中下一点功夫**。
- 如果cur指向的结点的值为$val$，需要删除它。如果cur为头指针，就将$head$移动到`head->next`，删除cur，然后`pre=cur=head`，将pre和cur都指向新的头结点位置；如果cur不是头指针，就和平常的删除一样，`pre->next=cur->next; cur = pre->next;`
- 不是$val$的话，就将pre和cur都移动到下一个位置。
```java
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
            if (cur->val == val) { //首先检查头结点，如果结点值与val相等，那么把头指针后移
                if (cur == head) {
                    head = head->next;
                    delete cur;
                    pre = cur = head;
                }
                else {
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
### 写法二 分开处理两种情况
我仔细想了一下，删除无头单链表需要特判头结点和删除中间结点两种情形。

而且删除中间结点不需要cur这个指针，实际上只需要pre指向要删除结点的前一个元素就可以了，每次检查`pre->next == NULL`判断下一个是否还有结点，然后`Node *t = pre->next; pre->next = pre->next->next; delete t;`。

但是删除头结点，不可能用`pre->next == head`特判，所以需要cur....然后删除头结点后还要移动pre和cur指向新的头结点...

为了将这两种情形统一起来，不知不觉就写出了这种代码....

为此，需要分开处理这两种情况：
```cpp 
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) { 
        while (head && head->val == val) {
            ListNode *delNode = head;
            head = head->next;
            delete delNode;
        }
        if (head == NULL) return NULL;
        ListNode *pre = head; //pre指向的结点一定不是val值
        while (pre->next) {
            if (pre->next->val == val) {
                ListNode *delNode = pre->next;
                pre->next = pre->next->next; //进行删除后pre不动
                delete delNode;
            } else pre = pre->next; //不进行删除时pre往后移动
        }
        return head; 
    }
};
```
如果不写删除的代码，可以简化成如下，照样能过：`执行用时 :12 ms, 在所有 C++ 提交中击败了99.80% 的用户`
```cpp 
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) { 
        while (head && head->val == val)  
            head = head->next; 
        if (head == NULL) return NULL;
        ListNode *pre = head; //pre指向的结点一定不是val值
        while (pre->next) {  
            if (pre->next->val == val) pre->next = pre->next->next; //进行删除后pre不动  
            else pre = pre->next; //不进行删除时pre往后移动
        }
        return head; 
    }
};
```
### 写法三 使用虚拟头结点统一操作
这里写删除的代码，毕竟这是C++，不是Java。正是由于有个无意义节点作为头结点会统一操作（把头结点当做普通节点），所以实际链表设计过程中几乎有个无意义头结点的。
```cpp 
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) { 
        if (head == NULL) return NULL; 
        ListNode *dummyHead = new ListNode(-1), *pre = dummyHead; 
        dummyHead->next = head; //有了一个虚拟头结点 
        while (pre->next) { //检查下一个结点是否是val
            if (pre->next->val == val) { 
                ListNode *p = pre->next;
                pre->next = pre->next->next; //连pre都不需要了
                delete p;
            } else pre = pre->next;
        }
        head = dummyHead->next; 
        delete dummyHead; //new的对象记得删除
        return head;
    }
};
```
### 写法四 递归删除所有值为val的结点
我们知道，用递归遍历链表，相当于有两趟过程——递去一趟，归来一趟。

在递去的时候解决问题，即，先检查当前结点值是否为val，然后做出不同的操作。
```cpp 
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) { 
        if (head == NULL) return NULL;
        if (head->val == val) head = removeElements(head->next, val);
        else head->next = removeElements(head->next, val);
        return head; 
    }
};
```

在归来的时候解决问题：从**调用removeElements()返回后**，如果当前的结点值为val，就返回当前结点的$next$作为结果，相当于删除了当前结点；不是val的话，就直接返回当前结点。由于删除要求的结点, 可能会改变某些个结点的$next$，因此我们看到归来的时候会修改当前结点的$next$。
```cpp 
class Solution {
public: 
    ListNode* removeElements(ListNode* head, int val) {  
        if (head == NULL) return NULL; //递归边界
        head->next = removeElements(head->next, val);  
        if (head->val == val) return head->next;  //如果头结点是val值
        return head; 
    }
};
```
