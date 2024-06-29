<div class="notranslate"><p>You are given a <strong>perfect binary tree</strong>&nbsp;where&nbsp;all leaves are on the same level, and every parent has two children. The binary tree has the following definition:</p>

```cpp
struct Node {
  int val;
  Node *left;
  Node *right;
  Node *next;
}
```

<p>Populate each next pointer to point to its next right node. If there is no next right node, the next pointer should be set to <code>NULL</code>.</p>

<p>Initially, all next pointers are set to <code>NULL</code>.</p>
 

<p><strong>Follow up:</strong></p>

<ul>
	<li>You may only use constant extra space.</li>
	<li>Recursive approach is fine, you may assume implicit stack space does not count as extra space for this problem.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

<p><img style="width: 640px; height: 218px;" src="https://assets.leetcode.com/uploads/2019/02/14/116_sample.png" width="70%" alt=""></p>

```cpp
Input:root = [1,2,3,4,5,6,7]
Output: [1,#,2,3,#,4,5,6,7,#]
Explanation: Given the above perfect binary tree (Figure A), your function should populate each next pointer to point to its next right node, just like in Figure B. The serialized output is in level order as connected by the next pointers, with '#' signifying the end of each level.
```
<p><strong>Constraints:</strong></p>

<ul>
	<li>The number of nodes in the given tree is less than <code>4096</code>.</li>
	<li><code>-1000 &lt;= node.val &lt;= 1000</code></li>
</ul>
</div>

题意：给定一个完美二叉树，**其所有叶子节点都在同一层**，**每个父节点都有两个子节点。** 填充每个结点的 `next` 指针，使其指向其下一个右侧节点。如果找不到下一个右侧节点，则设置为 `NULL` 。 

---
### 思路1 BFS+队列
给出的是一棵满二叉树，如果要填充指向下一个右侧结点的指针，就需要知道下一个右侧结点的位置，用广度优先遍历可以做到这一点。**从右到左遍历每一层**，即可解决这一题。代码如下：
```cpp
class Solution {
public:
    Node* connect(Node* root) {
        if (root == nullptr) return root;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            Node *nextRight = nullptr, *temp;
            for (int i = 0; i < size; ++i) { //每次遍历一层
                temp = q.front(); q.pop();
                temp->next = nextRight; //指向下一个右侧结点
                nextRight = temp;
                if (temp->right) q.push(temp->right);
                if (temp->left) q.push(temp->left);
            }
        }
        return root;
    }
};
```
效率如下：
```cpp
执行用时：36 ms, 在所有 C++ 提交中击败了50.34% 的用户
内存消耗：16.9 MB, 在所有 C++ 提交中击败了29.30% 的用户
```
----
### 思路2 BFS+常量空间
题目提示我们，只需使用常量的额外空间。因此想到，可以从上到下，**每次从该层的第一个结点开始，沿着已经设置好的 `next` 指针从左往右遍历这一层，设置下一层的 `next` 指针**，完成后指向下一层的第一个结点。下面的代码中，得到下一个右侧结点的方法很简单：
-  由于是满二叉树，因此当前结点的左孩子的 `next` 必然指向**当前结点的右孩子**；
- 当前结点的右孩子的 `next` 必然指向**当前结点的 `next` 指向的下一个右侧结点的左孩子**，由于 `next` 可能为 `NULL` ，因此也可能指向空。

当然，在117题这种做法会出现错误，因为当前结点的 `next` 指向的下一个右侧结点，不一定会有左右孩子，可能是叶子结点。但是这题由于题意，**如果当前结点不是叶子结点，就必然有左右孩子**；因为叶子结点在同一层，所以**下一个右侧结点也必定不是叶子结点，必然存在左右孩子**。
```cpp
class Solution { 
public:
    Node* connect(Node* root) {
        if (root == nullptr) return root;
        Node *temp = root;
        while (true) {
            bool isfirst = true;
            Node *newTemp = nullptr;
            for (Node *t = temp; t != nullptr; t = t->next) { //从左往右沿着next指针遍历这一层 
                if (t->left) {
                    if (isfirst) { newTemp = t->left; isfirst = false; } //指向下一层的第一个结点,作为下次遍历的起始结点 
                    t->left->next = t->right; 
                }
                if (t->right) {
                    if (isfirst) { newTemp = t->right; isfirst = false; } //指向下一层的第一个结点,作为下次遍历的起始结点 
                    t->right->next = t->next ? t->next->left : nullptr; 
                }
            }
            if (newTemp == temp) break; 
            temp = newTemp; //temp指向下一层第一个结点
        }
        return root;
    }
}; 
```
效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了88.99% 的用户
内存消耗：16.6 MB, 在所有 C++ 提交中击败了74.18% 的用户
```
---
### 思路3 DFS先序遍历
题目还提示说递归函数调用的栈空间不计算在内，因此可以使用递归算法。这里使用先序遍历，每次设置完当前结点的左、右孩子的 `next` 指针后，递归设置左子树和右子树的 `next` 指针。也就是**遵循传统的根-左-右遍历顺序**。

当然，在117题这样的做法同样存在上述问题。因为设置当前结点的孩子的 `next` 指针时，当前结点同一层的其他右侧结点的 `next` 指针没有全部设置好，一旦当前结点的 `next` 指向的下一个右侧结点是叶子结点，**需要寻找存在孩子的下一个右侧结点时**，就会无计可施。

这里说起来可能有点复杂，写下一题的时候看一下图就明白了。不过本题不存在这一问题，当前结点必然有左右孩子，当前结点的 `next` 指向的下一个右侧结点也必然存在左右孩子。**我们只需要将当前结点的左孩子结点的 `next` 指向右孩子结点，右孩子结点的 `next` 指向当前结点的 `next` 的左孩子结点**。然后递归即可。

本题的代码如下：
```cpp
class Solution { 
public:
    Node* connect(Node* root) {
        if (root == nullptr) return root; 
        if (root->left) root->left->next = root->right;
        if (root->right) root->right->next = root->next ? root->next->left : nullptr;
        connect(root->left);
        connect(root->right);
        return root;
    }
}; 
```
效率如下：
```cpp
执行用时：32 ms, 在所有 C++ 提交中击败了73.68% 的用户
内存消耗：16.7 MB, 在所有 C++ 提交中击败了59.02% 的用户
```
---
下一题117题的第一种做法一模一样，但是二、三、四种做法都不相同，尽管它们的代码也可以通过这一题。

