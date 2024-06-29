> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

Given `head` , the head of a linked list, determine if the linked list has a cycle in it.

There is a cycle in a linked list if there is some node in the list that can be reached again by continuously following the `next` pointer. Internally, `pos` is used to denote the index of the node that tail's `next` pointer is connected to. **Note that** `pos` **is not passed as a parameter**.

Return `true` *if there is a cycle in the linked list*. Otherwise, return `false` .

**Follow up:** Can you solve it using `O(1)` (i.e. constant) memory?

**Example 1:**
```swift
Input: head = [3,2,0,-4], pos = 1
Output: true
Explanation: There is a cycle in the linked list, where the tail connects to the 1st node (0-indexed).
```
**Example 2:**
```swift
Input: head = [1,2], pos = 0
Output: true
Explanation: There is a cycle in the linked list, where the tail connects to the 0th node.
```
**Example 3:**
```swift
Input: head = [1], pos = -1
Output: false
Explanation: There is no cycle in the linked list.
```
**Constraints:**
- The number of the nodes in the list is in the range <code>[0, 10<sup>4</sup>]</code> .
- <code>-10<sup>5</sup> <= Node.val <= 10<sup>5</sup></code> .
- `pos` is `-1` or a **valid index** in the linked-list.

题意：给定一个链表，判断链表中是否有环。如果链表中有某个节点，可以通过连续跟踪 `next` 指针再次到达，则链表中存在环。 如果链表中存在环，则返回 `true` 。 否则，返回 `false` 。

---
### 解法1 哈希表
最容易想到的方法是遍历所有节点，每次遍历到一个节点时，判断该节点此前是否被访问过。具体地，可以**使用哈希表来存储所有已经访问过的节点**。每次我们到达一个节点，如果该节点已经存在于哈希表中，则说明该链表是环形链表，否则就将该节点加入哈希表中。重复这一过程，直到我们遍历完整个链表即可。
```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        unordered_set<ListNode*> seen;
        while (head != nullptr) {
            if (seen.count(head)) return true;
            seen.insert(head);
            head = head->next;
        }
        return false;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是链表中的节点数。最坏情况下我们需要遍历每个节点一次。
- 空间复杂度：$O(n)$ ，其中 $n$ 是链表中的节点数。主要为哈希表的开销，最坏情况下需要将每个节点插入到哈希表中一次。

---
### 解法2 记录链表长度
不需要快慢指针，根据题目数据范围，由于题目说了结点数目最多 $1e4$ 个，那么我们直接使用一个指针即可，如果存在环，那么肯定会一直在走，边走边记录结点数量，当结点数量大于 $1e4$ ，那么必然存在环；如果没有到达 $1e4$ 就结束了自己跳出了循环，那么说明不存在环。
```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        int cnt = 0;
        while (head) {
            ++cnt;
            head = head->next;
            if(cnt > (int)1e4) return true;
        }
        return false;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$

---
### 解法3 快慢指针
本方法需要对「**Floyd 判圈算法**」（又称龟兔赛跑算法）有所了解。

假想「乌龟」和「兔子」在链表上移动，「兔子」跑得快，「乌龟」跑得慢。当「乌龟」和「兔子」从链表上的同一个节点开始移动时。
- 如果该链表中没有环，那么「兔子」将一直处于「乌龟」的前方；
- 如果该链表中有环，那么「兔子」会先于「乌龟」进入环，并且一直在环内移动。**等到「乌龟」进入环时，由于「兔子」的速度快，它一定会在某个时刻与乌龟相遇，即套了「乌龟」若干圈**。

总的来说，**两个指针一定会恰好相遇**——由于快指针一定先进入环，当慢指针刚刚进入环时，快指针已经在环里面。这就转换为**两个指针在环内**，是否一定会相遇？
- 假设两个指针从同一个地点开始移动， 那么第一次移动，$A$ 和 $B$ 的距离会增大 $1$ 个距离。第二次移动，又增大一个距离。那**当增大到环的长度的距离时，他们就再次相遇了**。
- 两个指针从不同地点开始移动，是否还会相遇呢？同样的，假设他们一开始就有距离 $x$ ，每一次移动增加一个距离，**当距离增加到环的长度时，就会相遇**。

根据上述思路来解决本题。具体地，我们定义两个指针，一快一慢，初始时都在位置 `head` 。慢指针每次只移动一步，而快指针每次移动两步。==如果在移动的过程中，快指针反过来从后面追上慢指针，就说明该链表为环形链表==。否则快指针将到达链表尾部，该链表不为环形链表。
```cpp
class Solution {
public:
    bool hasCycle(ListNode *head) {
        if (head == nullptr || head->next == nullptr) return false;
        ListNode *lo = head, *hi = head;
        while (hi && hi->next) { //判断快指针hi和hi->next是否存在
            lo = lo->next;
            hi = hi->next->next;
            if (lo == hi) return true;
        }
        return false;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$