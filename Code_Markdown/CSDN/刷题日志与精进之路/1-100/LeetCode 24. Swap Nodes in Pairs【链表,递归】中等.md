> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091348071.png)

给你一个链表，两两交换其中相邻的节点，并返回交换后链表的头节点。你必须在不修改节点内部的值的情况下完成本题（即，只能进行节点交换）。

**示例 1：**
![](https://assets.leetcode.com/uploads/2020/10/03/swap_ex1.jpg)

```java
输入：head = [1,2,3,4]
输出：[2,1,4,3]
```
**示例 2：**
```java
输入：head = []
输出：[]
```
**示例 3：**
```java
输入：head = [1]
输出：[1]
```
**提示：**
- 链表中节点的数目在范围 `[0, 100]` 内
- `0 <= Node.val <= 100`

---
### 解法1 虚拟头结点
设立虚拟头结点并用 `p` 指向它，`node1, node2` 指向要交换的两个相邻结点，`next` 指向 `node2` 后一个位置的结点，代表后一部分的链表。感觉和 `LeetCode 92. Reverse Linked List II` 有点像。

然后 `node2->next` 指向 `node1` ，`node1->next` 指向 `next` 从而链接后一部分的链表，`p->next` 指向 `node2` 和前一部分的链表链接起来：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200823135519658.png)
之后，**`p` 指向 `node1` ，为下次交换做准备**。具体实现的代码如下：  
```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode *dummy = new ListNode(0);
        dummy->next = head;
        ListNode *p = dummy;
        while (p->next && p->next->next) {
            ListNode *node1 = p->next;
            ListNode *node2 = node1->next;
            ListNode *next  = node2->next;
            //交换node1和node2
            node2->next = node1;
            node1->next = next;
            p->next = node2;
            //为进行下次的交换,p指向靠后的那个结点
            p = node1;
        }
        head = dummy->next;
        delete dummy;
        return head;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了65.17% 的用户
内存消耗：6.6 MB, 在所有 C++ 提交中击败了16.03% 的用户
```
我们不难发现，其中 `next` 是不需要的，甚至于我们只需要两个额外的指针 `p1, p2` 。代码如下，虽然简洁一些，但是可读性不太强：
```cpp
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode *dummy = new ListNode(0);
        dummy->next = head;
        ListNode *p1 = dummy, *p2;      //p1位于要翻转的那对结点前一个结点,将p2、p2->next进行交换
        while (p1->next && p1->next->next) {
            p2 = p1->next;
            p1->next = p2->next;        //p1指向那对结点靠后的结点
            p2->next = p1->next->next;  //p2->next指向后一部分的链表
            p1->next->next = p2;
            p1 = p2;                    //更新p1
        } 
        return dummy->next;
    }
};
```
效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了65.17% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了81.87% 的用户
```

方法二：迭代 
也可以通过迭代的方式实现两两交换链表中的节点。

创建哑结点 dummyHead，令 dummyHead.next = head。令 temp 表示当前到达的节点，初始时 temp = dummyHead。每次需要交换 temp 后面的两个节点。

如果 temp 的后面没有节点或者只有一个节点，则没有更多的节点需要交换，因此结束交换。否则，获得 temp 后面的两个节点 node1 和 node2，通过更新节点的指针关系实现两两交换节点。

具体而言，交换之前的节点关系是 temp -> node1 -> node2，交换之后的节点关系要变成 temp -> node2 -> node1，因此需要进行如下操作。

temp.next = node2
node1.next = node2.next
node2.next = node1
完成上述操作之后，节点关系即变成 temp -> node2 -> node1。再令 temp = node1，对链表中的其余节点进行两两交换，直到全部节点都被两两交换。

两两交换链表中的节点之后，新的链表的头节点是 dummyHead.next，返回新的链表的头节点即可。

 
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode* dummyHead = new ListNode(0);
        dummyHead->next = head;
        ListNode* temp = dummyHead;
        while (temp->next != nullptr && temp->next->next != nullptr) {
            ListNode* node1 = temp->next;
            ListNode* node2 = temp->next->next;
            temp->next = node2;
            node1->next = node2->next;
            node2->next = node1;
            temp = node1;
        }
        return dummyHead->next;
    }
};
复杂度分析：
- 时间复杂度：$O(n)$ ，其中 $n$ 是链表的节点数量。需要对每个节点进行更新指针的操作。
- 空间复杂度：$O(1)$ 。 

---
### 解法3 递归
不需要虚拟头结点，使用递归优雅地解决这道题，递归的三部曲：
- 找终止条件：本题终止条件很明显，当递归到**链表为空或者链表只剩一个元素**的时候，不用交换，自然就终止了。
- 找返回值：返回给上一层递归的值，应该是**已经交换完成后的子链表**。
- 单次的过程：递归重复做一样的事情，所以从宏观上考虑，只用考虑某一步是怎么完成的。假设待交换的俩结点分别为 `head` 和 `next = head->next` ，对 `head->next->next` 进行递归会返回一个处理完成的子链表。于是等价于**一个含三个结点的链表交换前两个节点**。我们将原本的 `head->next` 指向下一级返回的子链表，`next->next` 指向 `head` ，然后返回 `next` 作为新的链表头结点即可。
 
 具体代码如下：
```cpp
// C++
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head; //基准条件
        ListNode *next = head->next;
        head->next = swapPairs(next->next); //递归处理后一部分的链表
        next->next = head;
        return next;
    }
};
// Java 
class Solution {
    public ListNode swapPairs(ListNode head) {
        if (head == null || head.next == null) return head;
        ListNode next = head.next;
        head.next = swapPairs(head.next.next);
        next.next = head;
        return next;
    }
} 
```
效率如下：
```csharp
执行用时：8 ms, 在所有 C++ 提交中击败了5.92% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了75.47% 的用户
```


