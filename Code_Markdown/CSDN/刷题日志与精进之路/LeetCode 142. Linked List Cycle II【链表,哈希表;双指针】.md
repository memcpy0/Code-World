> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given a linked list, return the node where the cycle begins. If there is no cycle, return `null` .

There is a cycle in a linked list if there is some node in the list that can be reached again by continuously following the `next` pointer. Internally, `pos` is used to denote the index of the node that tail's `next` pointer is connected to. **Note that** `pos` **is not passed as a parameter.**

**Notice** that you **should not modify** the linked list.

**Follow up:** Can you solve it using `O(1)` (i.e. constant) memory?

**Example 1:**
<img src="https://img-blog.csdnimg.cn/20201010093451894.png#pic_center" width="35%">

```clike
Input: head = [3,2,0,-4], pos = 1
Output: tail connects to node index 1
Explanation: There is a cycle in the linked list, where tail connects to the second node.
```
**Example 2:**
<img src="https://img-blog.csdnimg.cn/2020101009345948.png#pic_center" width="20%">

```clike
Input: head = [1,2], pos = 0
Output: tail connects to node index 0
Explanation: There is a cycle in the linked list, where tail connects to the first node.
```
**Example 3:**
<img src="https://img-blog.csdnimg.cn/20201010093503724.png#pic_center" width="7%">
```clike
Input: head = [1], pos = -1
Output: no cycle
Explanation: There is no cycle in the linked list.
```
**Constraints:**
- The number of the nodes in the list is in the range <code>[0, 10<sup>4</sup>]</code> .
- <code>-10<sup>5</sup> <= Node.val <= 10<sup>5</sup></code>
-  `pos` is `-1` or a **valid index** in the linked-list.

题意：给定一个链表，返回链表开始入环的第一个节点。如果链表无环，则返回 `null` 。不允许修改给定的链表。

---
### 解法1 哈希集合
记录前面访问过的指针，然后往后遍历，如果无环则最终为空指针；有环则会遇到已访问过的结点。具体的C++代码如下：
```cpp
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return nullptr;
        unordered_set<ListNode*> record;
        ListNode *temp = head;
        while (temp) {
            if (record.find(temp) == record.end()) record.insert(temp);
            else return temp;
            temp = temp->next;
        }
        return nullptr;
    }
};
```
Java代码如下：
```java
public class Solution {
    public ListNode detectCycle(ListNode head) {
        if (head == null || head.next == null) return null;
        Set<ListNode> record = new HashSet<>();
        while (head != null) {
            if (record.contains(head) == false) record.add(head);
            else return head;
            head = head.next;
        }
        return null;
    }
}
```
复杂度分析：
- 时间复杂度：$O(N)$ ，其中 $N$ 为链表中节点的数目。我们恰好需要访问链表中的每一个节点。
- 空间复杂度：$O(N)$ ，其中 $N$ 为链表中节点的数目。我们需要将链表中的每个节点都保存在哈希表当中。

---
### 解法2 利用堆空间特性
堆的地址从低到高，而LeetCode中的链表结点内存是顺序申请的。如果有环，则 `head->next` 的地址一定小于等于 `head` 的地址，此时返回 `head->next` 。

注意，这种做法不一定正确——因为堆中的内存反复 `new` 和 `delete` 之后，之前分配的地址不一定比新分配的地址小。当然，Leetcode上的内存是充足的，没有复杂的手动内存管理，它逐用例依次测试，每个测试中顺序申请链表结点，测试完一个用例之后整体删除。因此，这种做法才能通过测试：
```cpp
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        while (head) {
            if (!less<ListNode*>()(head, head->next)) // head的地址>=head->next
                return head->next;
            head = head->next;
        }
        return nullptr;
    }
};
```
提交后执行效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了65.98% 的用户
内存消耗：7.9 MB, 在所有 C++ 提交中击败了28.98% 的用户
```
---
### 解法3 快慢指针
上述代码用到了额外空间，实际上可以不使用。做法是：我们使用两个指针，$fast$ 与 $slow$。**它们起始都位于链表的头部**。随后，$slow$ 指针每次向后移动一个位置，而 $fast$ 指针向后移动两个位置。**如果链表中存在环，则 $fast$ 指针最终将再次与 $slow$ 指针在环中相遇**。

如下图所示，设链表中环外部分的长度为 $a$ 。$slow$ 指针进入环后，又走了 $b$ 的距离与 $fast$ 相遇。此时，$fast$ 指针已经走完了环的 $n$ 圈，因此它走过的总距离为 $a + n (b+c) + b = a + (n + 1) b + n c$ 。
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202308060307186.png)
根据题意，任意时刻，$fast$ 指针走过的距离都为 $slow$ 指针的 $2$ 倍。因此，我们有
$$a + (n + 1) b + nc = 2(a + b) \Leftrightarrow a = c + (n - 1) ( b + c)$$
有了 $a = c + (n - 1)(b+c)$ 的等量关系，我们会发现：**从相遇点到入环点的距离**加上 $n-1$ 圈的环长，恰好等于**从链表头部到入环点的距离**。

因此，当发现 $slow$ 与 $fast$ 相遇时，我们再额外使用一个指针 $ptr$ 。起始，它指向链表头部；随后，它和 $slow$ 每次向后移动一个位置。最终，它们会在入环点相遇。
```cpp
class Solution {
public:
    ListNode *detectCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return nullptr;
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                ListNode *ptr = head;
                while (ptr != slow) {
                    ptr = ptr->next;
                    slow = slow->next;
                }
                return ptr;
            }
        } 
        return nullptr;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为链表中节点的数目。在最初判断快慢指针是否相遇时，$slow$ 指针走过的距离不会超过链表的总长度；随后寻找入环点时，走过的距离也不会超过链表的总长度。因此，总的执行时间为 $O(n)$ 。
- 空间复杂度：$O(1)$ 。我们只使用了 $slow, fast, ptr$ 三个指针。
