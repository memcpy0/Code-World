Given the `head` of a linked list, return *the list after sorting it in **ascending** order*.

**Follow up:** Can you sort the linked list in `O(n logn)` time and `O(1)` memory (i.e. constant space)?

 

**Example 1:**

```swift
Input: head = [4,2,1,3]
Output: [1,2,3,4]
```

**Example 2:**

```swift
Input: head = [-1,5,3,4,0]
Output: [-1,0,3,4,5]
```

**Example 3:**

```swift
Input: head = []
Output: []
```

 

**Constraints:**
- The number of nodes in the list is in the range `[0, 5 * 10^4]`.
- `-10^5 <= Node.val <= 10^5`

题意：对链表进行升序排序，要求 $O(n\log n)$ 的时间复杂度和 $O(1)$ 的空间复杂度。

---
### 思路1 自顶向下归并排序
Java代码如下：
```cpp  
class Solution {
    public ListNode sortList(ListNode head) {
        return head == null ? null : mergeSort(head);
    }
    private ListNode mergeSort(ListNode head) {
        if (head.next == null) return head;
        ListNode p = head, q = head, pre = null;
        while (q != null && q.next != null) {
            pre = p;
            p = p.next;
            q = q.next.next;
        }
        //q指向末尾,p指向链表中间,pre指向p前一位
        pre.next = null;
        ListNode l = mergeSort(head);
        ListNode r = mergeSort(p);
        return merge(l, r);
    }
    ListNode merge(ListNode l, ListNode r) {
        ListNode dummyHead = new ListNode(0);
        ListNode cur = dummyHead;
        while (l != null & r != null) {
            if (l.val <= r.val) {
                cur.next = l;
                l = l.next;
            } else {
                cur.next = r;
                r = r.next;
            }
            cur = cur.next;
        }
        if (l != null) cur.next = l;
        if (r != null) cur.next = r;
        return dummyHead.next;
    }
}
```
提交结果如下：
```cpp
执行用时：4 ms, 在所有 Java 提交中击败了58.75% 的用户
内存消耗：40.9 MB, 在所有 Java 提交中击败了67.31% 的用户
```



