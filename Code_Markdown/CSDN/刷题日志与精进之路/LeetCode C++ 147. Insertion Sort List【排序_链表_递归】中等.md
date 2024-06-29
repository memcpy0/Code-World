Sort a linked list using insertion sort.
> ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200917160556525.gif)
> A graphical example of insertion sort. The partial sorted list (black) initially contains only the first element in the list.
With each iteration one element (red) is removed from the input data and inserted in-place into the sorted list
 

Algorithm of Insertion Sort:
-  Insertion sort iterates, consuming one input element each repetition, and growing a sorted output list.
-  At each iteration, insertion sort removes one element from the input data, finds the location it belongs within the sorted list, and inserts it there.
- It repeats until no input elements remain.


**Example 1:**

```cpp
Input: 4->2->1->3
Output: 1->2->3->4
```

**Example 2:**

```cpp
Input: -1->5->3->4->0
Output: -1->0->3->4->5
```

题意：对链表进行插入排序。

----
### 解法1 递归插入排序
由于是单链表，因此寻常的插入排序算法中**往前比较和插入**的做法是**不可行的**。自然而然，我们想到了**向后比较**，寻找合适的插入位置。不过，从首元素节点开始向后比较并插入的做法，毫无疑问是错误的，因为后面没有形成一个有序区。

**正确的做法是递归处理单链表**，在归来的过程中，当前结点之后已经形成有序区域，我们将当前结点的值与后继结点的值相比较——如果当前结点的值小于直接后继结点的值，则当前结点仍是子链表的头结点；否则将当前结点插入到后面的有序区域中，返回新的头结点指针。具体代码如下：
```cpp
class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head; //空或者单结点,已经有序
        head->next = insertionSortList(head->next); //递归到尾结点,把右边作为有序区进行插入排序
        ListNode *cur, *pre, *sortedHead = head->next;  //寻找插入的位置
        for (cur = sortedHead; cur && head->val > cur->val; pre = cur, cur = cur->next);
        if (cur == sortedHead) return head; //作为新链表的头结点
        pre->next = head; //插入到新链表中
        head->next = cur;  
        return sortedHead; //返回新的头结点
    }
}; 
```
**用递归处理链表的好处是，代码简洁，逻辑自然，不需要使用虚拟头结点**。但是效率较低：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了43.77% 的用户
内存消耗：9.7 MB, 在所有 C++ 提交中击败了27.84% 的用户
```
---
### 解法2 迭代插入排序
前面说过，向前比较并插入是不可行的，因此我们这里的做法是：对每个(无序)结点，都从首元素结点开始**向后比较**，找到合适的位置插入，得到新的有序区。具体代码如下：
```cpp
class Solution {
public:
    ListNode* insertionSortList(ListNode* head) {
        ListNode dummy(0), *pre = &dummy; //pre记录要插入位置的前一个位置
        dummy.next = head;
        while (head != nullptr && head->next != nullptr) {
            if (head->val <= head->next->val) { //有序区略过
                head = head->next;				//用head指向升序区的最后一个位置
                continue; 
            } //此时head及之前的位置属于有序区,将head->next指向的结点插入到前面的有序区中
            pre = &dummy; //用pre->从表头开始循环,使用dummy头结点
            while (pre->next->val < head->next->val) pre = pre->next; 
            ListNode *cur = head->next; //断开head->next结点
            head->next = cur->next;
            cur->next = pre->next; //插入结点到有序区中
            pre->next = cur;
        }
        return dummy.next;
    }
}; 
```
执行效率高出许多：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了93.16% 的用户
内存消耗：9.9 MB, 在所有 C++ 提交中击败了11.78% 的用户
```
