> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202310091347976.png)

给你一个链表数组，每个链表都已经按升序排列。

请你将所有链表合并到一个升序链表中，返回合并后的链表。

**示例 1：**
```java
输入：lists = [[1,4,5],[1,3,4],[2,6]]
输出：[1,1,2,3,4,4,5,6]
解释：链表数组如下：
[
  1->4->5,
  1->3->4,
  2->6
]
将它们合并到一个有序链表中得到。
1->1->2->3->4->4->5->6
```
**示例 2：**
```java
输入：lists = []
输出：[]
```
**示例 3：**
```java
输入：lists = [[]]
输出：[]
```
**提示：**
- `k == lists.length`
- `0 <= k <= 10^4`
- `0 <= lists[i].length <= 500`
- `-10^4 <= lists[i][j] <= 10^4`
- `lists[i]` 按 **升序** 排列
- `lists[i].length` 的总和不超过 `10^4 

---

前置知识：合并两个有序链表
思路 在解决「合并K个排序链表」这个问题之前，我们先来看一个更简单的问题：如何合并两个有序链表？假设链表 aaa 和 bbb 的长度都是 nnn，如何在 O(n)O(n)O(n) 的时间代价以及 O(1)O(1)O(1) 的空间代价完成合并？ 这个问题在面试中常常出现，为了达到空间代价是 O(1)O(1)O(1)，我们的宗旨是「原地调整链表元素的 next\textit{next}next 指针完成合并」。以下是合并的步骤和注意事项，对这个问题比较熟悉的读者可以跳过这一部分。此部分建议结合代码阅读。

首先我们需要一个变量 head\textit{head}head 来保存合并之后链表的头部，你可以把 head\textit{head}head 设置为一个虚拟的头（也就是 head\textit{head}head 的 val\textit{val}val 属性不保存任何值），这是为了方便代码的书写，在整个链表合并完之后，返回它的下一位置即可。
我们需要一个指针 tail\textit{tail}tail 来记录下一个插入位置的前一个位置，以及两个指针 aPtr\textit{aPtr}aPtr 和 bPtr\textit{bPtr}bPtr 来记录 aaa 和 bbb 未合并部分的第一位。注意这里的描述，tail\textit{tail}tail 不是下一个插入的位置，aPtr\textit{aPtr}aPtr 和 bPtr\textit{bPtr}bPtr 所指向的元素处于「待合并」的状态，也就是说它们还没有合并入最终的链表。 当然你也可以给他们赋予其他的定义，但是定义不同实现就会不同。
当 aPtr\textit{aPtr}aPtr 和 bPtr\textit{bPtr}bPtr 都不为空的时候，取 val\textit{val}val 熟悉较小的合并；如果 aPtr\textit{aPtr}aPtr 为空，则把整个 bPtr\textit{bPtr}bPtr 以及后面的元素全部合并；bPtr\textit{bPtr}bPtr 为空时同理。
在合并的时候，应该先调整 tail\textit{tail}tail 的 next\textit{next}next 属性，再后移 tail\textit{tail}tail 和 *Ptr\textit{*Ptr}*Ptr（aPtr\textit{aPtr}aPtr 或者 bPtr\textit{bPtr}bPtr）。那么这里 tail\textit{tail}tail 和 *Ptr\textit{*Ptr}*Ptr 是否存在先后顺序呢？它们谁先动谁后动都是一样的，不会改变任何元素的 next\textit{next}next 指针。
代码

C++
Java
ListNode* mergeTwoLists(ListNode *a, ListNode *b) {
    if ((!a) || (!b)) return a ? a : b;
    ListNode head, *tail = &head, *aPtr = a, *bPtr = b;
    while (aPtr && bPtr) {
        if (aPtr->val < bPtr->val) {
            tail->next = aPtr; aPtr = aPtr->next;
        } else {
            tail->next = bPtr; bPtr = bPtr->next;
        }
        tail = tail->next;
    }
    tail->next = (aPtr ? aPtr : bPtr);
    return head.next;
}
复杂度分析

时间复杂度：O(n)O(n)O(n)。
空间复杂度：O(1)O(1)O(1)。
方法一：顺序合并
思路

我们可以想到一种最朴素的方法：用一个变量 ans\textit{ans}ans 来维护以及合并的链表，第 iii 次循环把第 iii 个链表和 ans\textit{ans}ans 合并，答案保存到 ans\textit{ans}ans 中。

代码

C++
Java
class Solution {
public:
    ListNode* mergeTwoLists(ListNode *a, ListNode *b) {
        if ((!a) || (!b)) return a ? a : b;
        ListNode head, *tail = &head, *aPtr = a, *bPtr = b;
        while (aPtr && bPtr) {
            if (aPtr->val < bPtr->val) {
                tail->next = aPtr; aPtr = aPtr->next;
            } else {
                tail->next = bPtr; bPtr = bPtr->next;
            }
            tail = tail->next;
        }
        tail->next = (aPtr ? aPtr : bPtr);
        return head.next;
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode *ans = nullptr;
        for (size_t i = 0; i < lists.size(); ++i) {
            ans = mergeTwoLists(ans, lists[i]);
        }
        return ans;
    }
};
复杂度分析

时间复杂度：假设每个链表的最长长度是 nnn。在第一次合并后，ans\textit{ans}ans 的长度为 nnn；第二次合并后，ans\textit{ans}ans 的长度为 2×n2\times n2×n，第 iii 次合并后，ans\textit{ans}ans 的长度为 i×ni\times ni×n。第 iii 次合并的时间代价是 O(n+(i−1)×n)=O(i×n)O(n + (i - 1) \times n) = O(i \times n)O(n+(i−1)×n)=O(i×n)，那么总的时间代价为 O(∑i=1k(i×n))=O((1+k)⋅k2×n)=O(k2n)O(\sum_{i = 1}^{k} (i \times n)) = O(\frac{(1 + k)\cdot k}{2} \times n) = O(k^2 n)O(∑ 
i=1
k
​
 (i×n))=O( 
2
(1+k)⋅k
​
 ×n)=O(k 
2
 n)，故渐进时间复杂度为 O(k2n)O(k^2 n)O(k 
2
 n)。
空间复杂度：没有用到与 kkk 和 nnn 规模相关的辅助空间，故渐进空间复杂度为 O(1)O(1)O(1)。
 
方法二：分治合并
思路

考虑优化方法一，用分治的方法进行合并。

将 kkk 个链表配对并将同一对中的链表合并；
第一轮合并以后， kkk 个链表被合并成了 k2\frac{k}{2} 
2
k
​
  个链表，平均长度为 2nk\frac{2n}{k} 
k
2n
​
 ，然后是 k4\frac{k}{4} 
4
k
​
  个链表， k8\frac{k}{8} 
8
k
​
  个链表等等；
重复这一过程，直到我们得到了最终的有序链表。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307032131856.png)


C++
Java
class Solution {
public:
    ListNode* mergeTwoLists(ListNode *a, ListNode *b) {
        if ((!a) || (!b)) return a ? a : b;
        ListNode head, *tail = &head, *aPtr = a, *bPtr = b;
        while (aPtr && bPtr) {
            if (aPtr->val < bPtr->val) {
                tail->next = aPtr; aPtr = aPtr->next;
            } else {
                tail->next = bPtr; bPtr = bPtr->next;
            }
            tail = tail->next;
        }
        tail->next = (aPtr ? aPtr : bPtr);
        return head.next;
    }

    ListNode* merge(vector <ListNode*> &lists, int l, int r) {
        if (l == r) return lists[l];
        if (l > r) return nullptr;
        int mid = (l + r) >> 1;
        return mergeTwoLists(merge(lists, l, mid), merge(lists, mid + 1, r));
    }

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        return merge(lists, 0, lists.size() - 1);
    }
};
复杂度分析

时间复杂度：考虑递归「向上回升」的过程——第一轮合并 k2\frac{k}{2} 
2
k
​
  组链表，每一组的时间代价是 O(2n)O(2n)O(2n)；第二轮合并 k4\frac{k}{4} 
4
k
​
  组链表，每一组的时间代价是 O(4n)O(4n)O(4n)......所以总的时间代价是 O(∑i=1∞k2i×2in)=O(kn×log⁡k)O(\sum_{i = 1}^{\infty} \frac{k}{2^i} \times 2^i n) = O(kn \times \log k)O(∑ 
i=1
∞
​
  
2 
i
 
k
​
 ×2 
i
 n)=O(kn×logk)，故渐进时间复杂度为 O(kn×log⁡k)O(kn \times \log k)O(kn×logk)。
空间复杂度：递归会使用到 O(log⁡k)O(\log k)O(logk) 空间代价的栈空间。

如果原地修改，分治的递归可改为循环

```cpp
	if(lists.size() == 0)
		return NULL;
        const int l = lists.size();
        for(int step = 1;step < l;step *= 2)
        {
            for(int i = step;i<l;i+=2 * step)
            {
                lists[i-step] = pairMerge(lists[i-step], lists[i]);
                lists[i] = NULL;
            }
        }
	return lists[0];
```

---
### 解法1 堆排序
这道题是**多路归并排序**。我们可以使用一个大小为 `k` 的最小堆优先队列，将链表的头指针都放进去，然后出队当前优先队列中的最小值，挂上结果链表的尾部，再让出队的那个结点的下一个入队。接着再出队当前最小堆堆顶元素……直到优先队列为空。具体代码如下：
```cpp
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int n = lists.size();
        ListNode dummy(0), *rear = &dummy;
        auto cmp = [](ListNode *a, ListNode *b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
        for (int i = 0; i < n; ++i) if (lists[i]) pq.push(lists[i]);
        while (!pq.empty()) {
            ListNode *t = pq.top(); pq.pop();
            rear->next = new ListNode(t->val);
            rear = rear->next;
            if (t->next) pq.push(t->next);
        }
        return dummy.next;
    }
};
```
效率如下，时间复杂度为 $O(n\log k)$ ：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了37.43% 的用户
内存消耗：14.2 MB, 在所有 C++ 提交中击败了16.87% 的用户
```

这个方法和前两种方法的思路有所不同，我们需要维护当前每个链表没有被合并的元素的最前面一个，kkk 个链表就最多有 kkk 个满足这样条件的元素，每次在这些元素里面选取 val\textit{val}val 属性最小的元素合并到答案中。在选取最小元素的时候，我们可以用优先队列来优化这个过程。
复杂度分析

时间复杂度：考虑优先队列中的元素不超过 kkk 个，那么插入和删除的时间代价为 O(log⁡k)O(\log k)O(logk)，这里最多有 knknkn 个点，对于每个点都被插入删除各一次，故总的时间代价即渐进时间复杂度为 O(kn×log⁡k)O(kn \times \log k)O(kn×logk)。
空间复杂度：这里用了优先队列，优先队列中的元素不超过 kkk 个，故渐进空间复杂度为 O(k)O(k)O(k)。
 

```cpp
class Solution {
public:
    struct comp {
        bool operator()(ListNode* a, ListNode* b) {
            return a->val > b->val;
        }
    };

    priority_queue<ListNode*, vector<ListNode*>, comp> q;

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        for (auto node: lists) {
            if (node) q.push(node);
        }
        ListNode* head = new ListNode();
        ListNode* tail = head;
        while (!q.empty()) {
            ListNode* node = q.top();
            q.pop();
            tail->next = node; 
            tail = tail->next;
            if (node->next) q.push(node->next);
        }
        return head->next;
    }
};
```