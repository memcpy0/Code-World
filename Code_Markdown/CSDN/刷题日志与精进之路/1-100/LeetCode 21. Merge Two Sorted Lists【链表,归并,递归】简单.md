> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091346344.png)
将两个升序链表合并为一个新的 **升序** 链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。 

**示例 1：**
![](https://assets.leetcode.com/uploads/2020/10/03/merge_ex1.jpg)

```java
输入：l1 = [1,2,4], l2 = [1,3,4]
输出：[1,1,2,3,4,4]
```
**示例 2：**
```java
输入：l1 = [], l2 = []
输出：[]
```
**示例 3：**
```java
输入：l1 = [], l2 = [0]
输出：[0]
```
**提示：**
- 两个链表的节点数目范围是 `[0, 50]`
- `-100 <= Node.val <= 100`
- `l1` 和 `l2` 均按 **非递减顺序** 排列

---
思路1：双指针。通过多次使用三元表达式，简化代码。代码如下：
```cpp 
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode newHead(0), *rear = &newHead;
        while (l1 && l2) { //都不为空指针
            bool judge = l1->val <= l2->val; //多次用三元表达式
            rear->next = judge ? l1 : l2;
            judge ? (l1 = l1->next) : (l2 = l2->next); 
            rear = rear->next; 
        }
        rear->next = l1 ? l1 : l2;
        return newHead.next;
    }
};
```
---
思路2：双指针+交换。**通过交换指针，始终合并 `l1` 指向的结点，最后合并剩下的链表**。代码如下：
```cpp 
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode newHead(0), *rear = &newHead;
        while (l1 && l2) {
            if (l1->val > l2->val) swap(l1, l2);
            rear->next = l1;
            l1 = l1->next;
            rear = rear->next;
        }
        rear->next = l1 ? l1 : l2;
        return newHead.next;
    }
}; 
```
提交后效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了98.11% 的用户
内存消耗：14.1 MB, 在所有 C++ 提交中击败了21.45% 的用户
```

---
思路3：递归实现。递归基准是：都为空、或者一个链表指针为空。递归体是：
- 如果 `l1->val` 更小，就递归合并 `l1->next, l2` ，将返回的指针接到 `l1->next` ；
- 如果 `l2->val` 更小，就递归合并 `l1, l2->next` ，将返回的指针接到 `l2->next` ；

代码如下：
```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l1 == nullptr) return l2;
        if (l2 == nullptr) return l1;
        if (l1->val <= l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        }
    }
};
```
简化一下上述代码。**通过交换指针，始终合并 `l1` 指向的结点，然后递归合并 `l1->next` 和 `l2`**：
```cpp
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (l1 == nullptr) return l2;
        if (l2 == nullptr) return l1;
        if (l1->val > l2->val) swap(l1, l2);
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    }
};
```
复杂度分析：
- 时间复杂度：$O(n)$
- 空间复杂度：$O(1)$
