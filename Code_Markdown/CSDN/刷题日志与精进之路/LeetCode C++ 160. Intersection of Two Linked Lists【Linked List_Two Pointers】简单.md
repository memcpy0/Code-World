

Write a program to find the node at which the intersection of two singly linked lists begins.

For example, the following two linked lists:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201117003957368.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

begin to intersect at node c1.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201117004010743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: intersectVal = 8, listA = [4,1,8,4,5], listB = [5,6,1,8,4,5], skipA = 2, skipB = 3
Output: Reference of the node with value = 8
Input Explanation: The intersected node's value is 8 (note that this must not be 0 if the two lists intersect). From the head of A, it reads as [4,1,8,4,5]. From the head of B, it reads as [5,6,1,8,4,5]. There are 2 nodes before the intersected node in A; There are 3 nodes before the intersected node in B.
```

 

**Example 2:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201117004024186.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: intersectVal = 2, listA = [1,9,1,2,4], listB = [3,2,4], skipA = 3, skipB = 1
Output: Reference of the node with value = 2
Input Explanation: The intersected node's value is 2 (note that this must not be 0 if the two lists intersect). From the head of A, it reads as [1,9,1,2,4]. From the head of B, it reads as [3,2,4]. There are 3 nodes before the intersected node in A; There are 1 node before the intersected node in B.
```

 

**Example 3:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/202011170040406.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

```swift
Input: intersectVal = 0, listA = [2,6,4], listB = [1,5], skipA = 3, skipB = 2
Output: null
Input Explanation: From the head of A, it reads as [2,6,4]. From the head of B, it reads as [1,5]. Since the two lists do not intersect, intersectVal must be 0, while skipA and skipB can be arbitrary values.
Explanation: The two lists do not intersect, so return null.
```

 

**Notes:**
- If the two linked lists have no intersection at all, return `null`.
 - The linked lists **must retain** their original structure after the function returns.
 - You may assume there are **no cycles** anywhere in the entire linked structure.
 -  Each value on each linked list is in the range `[1, 10^9]`.
 -  Your code should preferably run in `O(n)` time and use only `O(1)` memory.

题意：编写一个满足 `O(n)` 时间复杂度、且仅用 `O(1)` 内存的程序，找到两个单链表相交的起始节点（注意，整个链表结构中没有环）。如果两个链表没有交点，返回 `null` 。在返回结果后，两个链表仍须保持原有的结构。


----
### 解法 双指针
这里我们定义两个指针。第一轮让两个指针不断前进，到达末尾节点后使其指向另一个链表的头部。接着是第二轮移动，如果相遇则为交点，有交点就返回；无交点则最终两个指针都等于 `nullptr` ，它们相等从而退出循环。
```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if (headA == nullptr || headB == nullptr) return nullptr; //无交点
        ListNode *pa = headA, *pb = headB; 
        while (pa != pb) {
            pa = (pa == nullptr) ? headB : pa->next;
            pb = (pb == nullptr) ? headA : pb->next;
        }
        return pa;
    }
};
```
为什么这样做是正确的？**因为两个指针都完成第一轮移动后，恰好抹除了长度差**。以下图为例，第一轮移动后，`pb` 先完成第一轮移动并指向 `0` ，此时 `pa` 指向 `2` ；当 `pa` 完成第一轮移动并指向 `3` 时，`pb` 指向 `1` 。显然，它们再次移动，一定会相遇。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20201117010352206.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
两个指针都移动了相同的距离，最坏情况下移动的距离等于两个单链表的长度之和，因此时间复杂度为 $O(n + m)$ ，空间复杂度为 $O(1)$ 。运行结果如下：
```cpp
执行用时：48 ms, 在所有 C++ 提交中击败了94.76% 的用户
内存消耗：14.7 MB, 在所有 C++ 提交中击败了16.03% 的用户
```
