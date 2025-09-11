<p>Given the&nbsp;<code>head</code>&nbsp;of a linked list and two integers <code>m</code> and <code>n</code>. Traverse the linked list and remove some nodes&nbsp;in the following way:</p>

<ul>
	<li>Start with the head as the current node.</li>
	<li>Keep the first <code>m</code> nodes starting with the current node.</li>
	<li>Remove the next <code>n</code> nodes</li>
	<li>Keep repeating steps 2 and 3 until you reach the end of the list.</li>
</ul>

<p>Return the head of the modified list after removing the mentioned nodes.</p>

<p><strong>Follow up question:</strong> How can you solve this problem by modifying the list in-place?</p>

 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 620px; height: 95px;" src="https://assets.leetcode.com/uploads/2020/06/06/sample_1_1848.png" alt=""></strong></p>

```haskell
Input: head = [1,2,3,4,5,6,7,8,9,10,11,12,13], m = 2, n = 3
Output: [1,2,6,7,11,12]
Explanation: Keep the first (m = 2) nodes starting from the head of the linked List  (1 ->2) show in black nodes.
Delete the next (n = 3) nodes (3 -> 4 -> 5) show in read nodes.
Continue with the same procedure until reaching the tail of the Linked List.
Head of linked list after removing nodes is returned.
```

 
<p><strong>Example 2:</strong></p>

```haskell
Input: head = [1,2,3,4,5,6,7,8,9,10,11], m = 1, n = 3
Output: [1,5,9]
Explanation: Head of linked list after removing nodes is returned.
```

 
<p><strong><img style="width: 620px; height: 123px;" src="https://assets.leetcode.com/uploads/2020/06/06/sample_2_1848.png" alt=""></strong></p>

 
<p><strong>Example 3:</strong></p>

```haskell
Input: head = [1,2,3,4,5,6,7,8,9,10,11], m = 3, n = 1
Output: [1,2,3,5,6,7,9,10,11]
```
 
<p><strong>Example 4:</strong></p>

```haskell
Input: head = [9,3,7,7,9,10,8,2], m = 1, n = 2
Output: [9,7,8]
```

<p><strong>Constraints:</strong></p>

<ul>
	<li>The given linked list will contain between&nbsp;<code>1</code>&nbsp;and&nbsp;<code>10^4</code>&nbsp;nodes.</li>
	<li>The value of each node in the linked list will be in the range<code>&nbsp;[1, 10^6]</code>.</li>
	<li><code>1 &lt;= m,n &lt;=&nbsp;1000</code></li>
</ul>

题意：给定链表 `head` 和两个整数 `m` 和 `n` ，遍历该链表并按照如下方式删除节点：
- 开始时以头节点作为当前节点
-  保留以当前节点开始的前 `m` 个节点
-  删除接下来的 `n` 个节点
-  重复步骤 `2` 和 `3`，直到到达链表结尾

在删除了指定结点之后，返回修改过后的链表的头节点。

 
