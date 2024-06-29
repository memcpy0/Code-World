> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091140763.png)

给你两个 **非空** 的链表，表示两个非负的整数。它们每位数字都是按照 **逆序** 的方式存储的，并且每个节点只能存储 **一位** 数字。

请你将两个数相加，并以相同形式返回一个表示和的链表。

你可以假设除了数字 0 之外，这两个数都不会以 0 开头。

**示例 1：**
![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2021/01/02/addtwonumber1.jpg)

```java
输入：l1 = [2,4,3], l2 = [5,6,4]
输出：[7,0,8]
解释：342 + 465 = 807.
```
**示例 2：**
```java
输入：l1 = [0], l2 = [0]
输出：[0]
```
**示例 3：**
```java
输入：l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
输出：[8,9,9,9,0,0,0,1]
```
**提示：**
- 每个链表中的节点数在范围 `[1, 100]` 内
- `0 <= Node.val <= 9`
- 题目数据保证列表表示的数字不含前导零

---
### 解法1 递归
把虚线内要计算的内容，可以理解为一个和原问题相似的，规模更小的子问题，所以非常适合用递归解决。

每次把两个节点值 l1.val, l2.vall_1.\textit{val},\ l_2.\textit{val}l 
1.val, l 
2
​
 .val 与进位值 carry\textit{carry}carry 相加，除以 101010 的余数即为当前节点需要保存的数位，除以 101010 的商即为新的进位值。

代码实现时，有一个简化代码的小技巧：如果递归中发现 l2l_2l 
2
​
  的长度比 l1l_1l 
1
​
  更长，那么可以交换 l1l_1l 
1
​
  和 l2l_2l 
2
​
 ，保证 l1l_1l 
1
​
  不是空节点，从而简化代码逻辑。

作者：灵茶山艾府
链接：https://leetcode.cn/problems/add-two-numbers/solutions/2327008/dong-hua-jian-ji-xie-fa-cong-di-gui-dao-oe0di/
```c++
class Solution {
public:
    // l1 和 l2 为当前遍历的节点，carry 为进位
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2, int carry = 0) {
        if (l1 == nullptr && l2 == nullptr) // 递归边界：l1 和 l2 都是空节点
            return carry ? new ListNode(carry) : nullptr; // 如果进位了，就额外创建一个节点
        if (l1 == nullptr) // 如果 l1 是空的，那么此时 l2 一定不是空节点
            swap(l1, l2); // 交换 l1 与 l2，保证 l1 非空，从而简化代码
        carry += l1->val + (l2 ? l2->val : 0); // 节点值和进位加在一起
        l1->val = carry % 10; // 每个节点保存一个数位
        l1->next = addTwoNumbers(l1->next, (l2 ? l2->next : nullptr), carry / 10); // 进位
        return l1;
    }
}; 
```
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 l1l_1l 
1
​
  长度和 l2l_2l 
2
​
  长度的最大值。
空间复杂度：O(n)\mathcal{O}(n)O(n)。递归需要 O(n)\mathcal{O}(n)O(n) 的栈空间。
 
解法：遍历两个链表。如果两个都非空，就加它们的 `val` ，存在进位再加以前的进位；如果一个为空，就只加另一个值，存在进位再加以前的进位。然后移动 `p` 和 `q` 分别指向下面的结点，如果 `p` 或 `q` 为空，就跳过。此后申请新的结点并存储计算值，再向链表尾部插入结点。

最后存在进位就再存储进位。时间复杂度为 $O(n+m)$ 。还是对链表的操作不熟悉，自己不细心也是一方面。

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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *ans = new ListNode(0), *r = ans;
        ListNode *p = l1, *q = l2;
        int carry = 0, temp = 0;
        while (p || q) {
            if (p) {
                temp = temp + p->val;
                p = p->next;
            }
            if (q) {
                temp = temp + q->val;
                q = q->next;
            }
            temp += carry; carry = temp / 10;
            r->next = new ListNode(temp % 10);  
            r = r->next;
            temp = 0; 
        }
        if (carry)  
            r->next = new ListNode(carry);   
        return ans->next;
    }
};
```
效率如下：
```cpp
执行用时 :20 ms, 在所有 cpp 提交中击败了98.97% 的用户`
内存消耗 :10.4 MB, 在所有 cpp 提交中击败了78.82%的用户`
```

仔细想一下，如果题目一开始给出的是正序的链表呢？我们需要将给出的两个链表反序。链表逆序可以迭代实现，也可以递归实现。见下文：[单链表反转实现](https://blog.csdn.net/myRealization/article/details/103864256)

---
**2020/10/4 Update** 今日打卡，所以更新：

首先请看如何遍历一个链表，代码框架如下：

Python3
Java
C++
Go
// 遍历链表 l1
while (l1) { // 从链表头节点开始向后遍历，直到遇到空节点
    cout << l1->val << endl; // 当前节点值
    l1 = l1->next; // 准备遍历下一个节点
}
迭代的思路是，初始化答案为一个「空链表」，每次循环，向该链表末尾添加一个节点（保存一个数位）。

循环即遍历链表 l1l_1l 
1
​
  和 l2l_2l 
2
​
 ，每次把两个节点值 l1.val, l2.vall_1.\textit{val},\ l_2.\textit{val}l 
1
​
 .val, l 
2
​
 .val 与进位值 carry\textit{carry}carry 相加，除以 101010 的余数即为当前节点需要保存的数位，除以 101010 的商即为新的进位值。

需要注意的是，在第一次循环时，我们无法往一个空节点的末尾添加节点。这里的技巧是，创建一个哨兵节点（dummy node），当成初始的「空链表」。循环结束后，**哨兵节点**的下一个节点就是最终要返回的链表头节点。
 
```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        if (!l1 || !l2) return l1 ? l1 : l2;
        ListNode dummyHead(0), *rear = &dummyHead;
        int carry = 0; // 进位
        while (l1 || l2) { 
            int sum = carry;
            if (l1) { sum += l1->val; l1 = l1->next; }
            if (l2) { sum += l2->val; l2 = l2->next; } 
            rear->next = new ListNode(sum % 10);
            rear = rear->next;
            carry = sum / 10;
        }
        if (carry) rear->next = new ListNode(carry);
        return dummyHead.next;
    }
};
```
相同的题目还有 `LeetCode 67. 二进制求和` 。

```
class Solution {
public:
    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
        auto dummy = new ListNode(); // 哨兵节点
        auto cur = dummy;
        int carry = 0; // 进位
        while (l1 || l2 || carry) { // 有一个不是空节点，或者还有进位，就继续迭代
            carry += (l1 ? l1->val : 0) + (l2 ? l2->val : 0); // 节点值和进位加在一起
            cur = cur->next = new ListNode(carry % 10); // 每个节点保存一个数位
            carry /= 10; // 新的进位
            if (l1) l1 = l1->next; // 下一个节点
            if (l2) l2 = l2->next; // 下一个节点
        }
        return dummy->next; // 哨兵节点的下一个节点就是头节点
    }
};
```
 
复杂度分析
时间复杂度：O(n)\mathcal{O}(n)O(n)，其中 nnn 为 l1l_1l 
1
​
  长度和 l2l_2l 
2
​
  长度的最大值。
空间复杂度：O(1)\mathcal{O}(1)O(1)。返回值不计入。
思考题
本题的链表是从数字的最低位开始的，如果改成从最高位开始，要怎么做呢？欢迎在评论区发表你的思路。

206. 反转链表
445. 两数相加 II

作者：灵茶山艾府
链接：https://leetcode.cn/problems/add-two-numbers/solutions/2327008/dong-hua-jian-ji-xie-fa-cong-di-gui-dao-oe0di/
来源：力扣（LeetCode）
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。