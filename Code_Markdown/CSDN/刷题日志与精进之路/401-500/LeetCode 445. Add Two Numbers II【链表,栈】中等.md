> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

给你两个 **非空** 链表来代表两个非负整数。数字最高位位于链表开始位置。它们的每个节点只存储一位数字。将这两数相加会返回一个新的链表。

你可以假设除了数字 0 之外，这两个数字都不会以零开头。

**示例1：**

![](https://pic.leetcode-cn.com/1626420025-fZfzMX-image.png)

```java
输入：l1 = [7,2,4,3], l2 = [5,6,4]
输出：[7,8,0,7]
```
**示例2：**
```java
输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[8,0,7]
```
**示例3：**
```java
输入：l1 = [0], l2 = [0]
输出：[0]
```
**提示：**
- 链表的长度范围为 `[1, 100]`
- `0 <= node.val <= 9`
- 输入数据保证链表代表的数字无前导 0

**进阶：** 如果输入链表不能翻转该如何解决？

---
### 解法1 反转链表然后相加
反转链表 $l_1, l_2$ ，然后相加。这种题目中不恢复过来原链表也行，因此下面没有特意保存反转后链表的头节点：
```cpp
class Solution {
private:
    ListNode* reverseList(ListNode *head) {
        if (head == NULL || head->next == NULL) return head;
        ListNode *newHead = reverseList(head->next); // 传递不变的新链表头指针,即原链表尾指针
        head->next->next = head; // 把下个节点指向自己,即把新链表的指针连接上一个节点
        head->next = NULL; // 新节点作为新链表的结尾
        return newHead;
    } 
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *p = reverseList(l1), *q = reverseList(l2);
        ListNode dummy;
        int carry = 0; 
        while (p || q || carry) {
            if (p) { carry += p->val; p = p->next; }
            if (q) { carry += q->val; q = q->next; }
            dummy.next = new ListNode(carry % 10, dummy.next);
            carry /= 10;  
        } 
        return dummy.next;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $l_1, l_2$ 长度的最大值。
- 空间复杂度：$O(n)$ 。递归需要 $O(n)$ 的栈空间。
 
---
### 解法2 双栈+头插法
不修改原链表，而是使用两个栈分别存储两个数的数位，之后不断弹出对应数位，相加再插入新链表头部：
> 大佬锐评：![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307031924156.png)

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        stack<int> s1, s2;
        ListNode *t1 = l1, *t2 = l2;
        while (t1) { s1.push(t1->val); t1 = t1->next; }
        while (t2) { s2.push(t2->val); t2 = t2->next; }
        int c = 0;
        ListNode dummy;
        while (!s1.empty() || !s2.empty() || c) {
            if (!s1.empty()) { c += s1.top(); s1.pop(); }
            if (!s2.empty()) { c += s2.top(); s2.pop(); }
            dummy.next = new ListNode(c % 10, dummy.next);
            c /= 10;
        }
        return dummy.next;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 为 $l_1, l_2$ 长度的最大值。
- 空间复杂度：$O(n)$ 。递归需要 $O(n)$ 的栈空间。

还有一种[快慢指针，不用递归不用栈不翻转链表，原地计算](https://leetcode.cn/problems/add-two-numbers-ii/solutions/201416/c-bu-yong-di-gui-bu-yong-zhan-yuan-di-ji-suan-by-a/)的 $O(n)$ 做法。