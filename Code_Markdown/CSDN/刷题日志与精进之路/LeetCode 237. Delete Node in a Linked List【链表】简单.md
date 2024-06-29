> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


<p>Write a function to <strong>delete a node</strong> in a singly-linked list. You will <strong>not</strong> be given access to the <code>head</code> of the list, instead you will be given access to <strong>the node to be deleted</strong> directly.</p>

<p>It is <strong>guaranteed</strong> that the node to be deleted is <strong>not a tail node</strong> in the list.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 450px; height: 322px;" src="https://assets.leetcode.com/uploads/2020/09/01/node1.jpg" alt="">

```cpp
Input: head = [4,5,1,9], node = 5
Output: [4,1,9]
Explanation: You are given the second node with value 5, the linked list should become 4 -> 1 -> 9 after calling your function.
```

 
<p><strong>Example 2:</strong></p>
<img style="width: 450px; height: 354px;" src="https://assets.leetcode.com/uploads/2020/09/01/node2.jpg" alt="">

```cpp
Input: head = [4,5,1,9], node = 1
Output: [4,5,9]
Explanation: You are given the third node with value 1, the linked list should become 4 -> 5 -> 9 after calling your function.
```

 

<p><strong>Example 3:</strong></p>

```cpp
Input: head = [1,2,3,4], node = 3
Output: [1,2,4]
```

<p><strong>Example 4:</strong></p>

```cpp
Input: head = [0,1], node = 0
Output: [1]
```
 
<p><strong>Example 5:</strong></p>

```cpp
Input: head = [-3,5,-99], node = -3
Output: [5,-99]
```

<p><strong>Constraints:</strong></p>
<ul>
	<li>The number of the nodes in the given list is in the range <code>[2, 1000]</code>.</li>
	<li><code>-1000 &lt;= Node.val &lt;= 1000</code></li>
	<li>The value of each node in the list is <strong>unique</strong>.</li>
	<li>The <code>node</code> to be deleted is <strong>in the list</strong> and is <strong>not a tail</strong> node</li>
</ul>



题意：删除单链表中给定的某个**非末尾的有效的**结点。函数的唯一参数就是**要删除的结点**，函数不会返回任何值。

---
### 解法 模拟
如果要删除某个结点，就一定需要它之前的结点指针。这里只有要删除的结点指针，emmmm……，我们必须创造条件得到一个 `pre` 指针。由于给出的结点不是**末尾结点**，所以我们可以将 `node->val` 改为其后的结点的值，然后删除其后的结点。

代码很简单，时间复杂度和空间复杂度都为 $O(1)$ ：
```cpp
//C++ version
class Solution {
public:
    void deleteNode(ListNode* node) {
        node->val = node->next->val;
        ListNode *delNode = node->next;
        node->next = node->next->next;
        delete delNode;
    }
};
//执行用时：20 ms, 在所有 C++ 提交中击败了39.14% 的用户
//内存消耗：8.1 MB, 在所有 C++ 提交中击败了14.61% 的用户
```

```js
//JavaScript version
/**
 * Definition for singly-linked list.
 * function ListNode(val) {
 *     this.val = val;
 *     this.next = null;
 * }
 */
/**
 * @param {ListNode} node
 * @return {void} Do not return anything, modify node in-place instead.
 */
var deleteNode = function(node) {
    node.val = node.next.val;
    node.next = node.next.next;
};
//执行用时：64 ms, 在所有 JavaScript 提交中击败了99.05% 的用户
//内存消耗：39.7 MB, 在所有 JavaScript 提交中击败了26.15% 的用户
```
工程中可能的Java写法如下：
```java
public class Node {
	public int val;
	public Node next;
	public Node(int data) { this.val = data; }
}

public void deleteNode(Node node) {
	if (node == null) return;
	Node next = node.next;
	if (next == null) //是结尾结点
	    throw new RuntimeException("Can't remove last node.");
	node.val = next.val;
	node.next = next.next;
	next.next = null;
}
```

----
### 拓展思考
如果只是单纯的给出一个链表中的结点 `node` ，要求在链表中删除它，就会出现许多问题。
- 问题1：**这种删除方式无法删除最后一个结点**。比如，链表 `1->2->3->null` ，只知道删除结点 `3` ，但是它作为最后的结点，没有下一个结点来代替它被删除，只有让结点 `2` 的 `next` 指向 `null` 这一种方法，但是我们根本找不到结点 `2` 。又或者这样想，直接删除结点 `3` 使其在内存上的区域变成 `null` ，似乎相当于让结点 `2` 的 `next` 指针指向 `null` ，起到删除结点 `3` 的效果？**不可能**！`null` 是系统的一个特定区域，想要让结点 `2` 的 `next` 指针指向 `null` ，就必须找到结点 `2` ！
- 问题2：这种方式本质上不是删除 `node` 结点，而是改变 `node` 结点的值，然后删除它的下一个结点。**在实际工程中使用这种方法可能造成大问题**。比如，工程的某个结点代表很复杂的结构，结点值的复制比较复杂；或者禁止改变结点值；或者某个结点对外界有很多依赖等等。
