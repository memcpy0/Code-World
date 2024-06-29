<div class="notranslate"><p>Given a binary tree</p>

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

<p><img style="width: 640px; height: 218px;" src="https://assets.leetcode.com/uploads/2019/02/15/117_sample.png" width="70%" alt=""></p>

```cpp
Input: root = [1,2,3,4,5,null,7]
Output: [1,#,2,3,#,4,5,7,#]
Explanation: Given the above binary tree (Figure A), your function should populate each next pointer to point 
to its next right node, just like in Figure B. The serialized output is in level order as connected by the 
next pointers, with '#' signifying the end of each level. 
```
<p><strong>Constraints:</strong></p>
<ul>
	<li>The number of nodes in the given tree is less than <code>6000</code>.</li>
	<li><code>-100&nbsp;&lt;= node.val &lt;= 100</code></li>
</ul>

题意：给定一个二叉树，填充每个节点的 `next` 指针，让这个指针指向其下一个右侧节点。如果找不到下一个右侧节点，则将 `next` 指针设置为 `NULL` 。**这种做法和线索化二叉树很相似，不过这里是方便层序遍历**。
 
----
### 思路1 BFS+队列
使用广度优先搜索+队列，**从右往左遍历每一层结点**，将每个结点的 `next` 指向它的下个右侧结点。代码如下：
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
                if (temp->right) q.push(temp->right); //先右后左
                if (temp->left) q.push(temp->left);
            }
        }
        return root;
    }
};
```
效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了72.92% 的用户
内存消耗：17.3 MB, 在所有 C++ 提交中击败了31.95% 的用户
```
**从左往右遍历每一层结点**的代码如下：
```c
class Solution {
public:
    Node* connect(Node* root) {
        if (root == nullptr) return root;
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            Node *temp;
            for (int i = 0; i < size; ++i) { //每次遍历一层
                temp = q.front(); q.pop();
                temp->next = (i < size - 1) ? q.front() : nullptr; //指向下一个右侧结点  
                if (temp->left) q.push(temp->left); //先左后右
                if (temp->right) q.push(temp->right);
            }
        }
        return root;
    }
}; 
```

---
### 思路2 BFS+常量空间+getNext
题目提示我们，只需使用常量的额外空间。因此想到，可以从上到下，**每次从该层的第一个结点开始，沿着已经设置好的 `next` 指针从左往右遍历这一层，设置下一层的 `next` 指针**，完成后指向下一层的第一个结点。下面的代码中使用 `getNext()` 得到下一个右侧结点：
-  对于当前结点的左孩子，如果右孩子存在，则 `next` 必然指向**当前结点的右孩子**，或者指向 `getNext()` 返回的结果；
- 对于当前结点的右孩子，其 `next` 需要指向**某个右侧结点的孩子**，即 `getNext()` 的结果。
- `getNext()` 函数会沿着当前结点的 `next` 往右遍历这一层，返回存在的第一个孩子结点，或者返回空。

这样，即使当前结点的 `next` 指向的下一个右侧结点，可能是叶子结点，也不会出现下图的错误：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200929003007482.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)

具体代码如下：
```cpp
class Solution {
private:
    Node* getNext(Node *root) { //得到下一个右侧结点
        for (Node *temp = root; temp; temp = temp->next) {
            if (temp->left) return temp->left;
            if (temp->right) return temp->right;
        }
        return nullptr;
    }
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
                    if (t->right) t->left->next = t->right;
                    else t->left->next = getNext(t->next);
                }
                if (t->right) {
                    if (isfirst) { newTemp = t->right; isfirst = false; } //指向下一层的第一个结点,作为下次遍历的起始结点 
                    t->right->next = getNext(t->next);
                }
            }
            if (isfirst) break;
            temp = newTemp; //temp指向下一层第一个结点
        }
        return root;
    }
};
```
效率如下：
```cpp
执行用时：20 ms, 在所有 C++ 提交中击败了72.92% 的用户
内存消耗：17.3 MB, 在所有 C++ 提交中击败了26.87% 的用户
```
---

### 思路3 BFS+常量空间+虚拟头结点优化
上述做法显得太过繁琐，且效率较低。如果我们**将每一层视作一个链表，遍历时将存在的结点通过 `next` 指针连接起来**，代码就会很简洁。为了方便，这里**使用虚拟头结点和前驱结点**。

代码如下，简洁明了：
```cpp
class Solution { 
public:
    Node* connect(Node* root) {
        if (root == nullptr) return root;
        Node *leftMost = root; //当前层最左边的节点
        while (leftMost) { //每次循环当前一层时链接下一层的节点
            Node head(0), *tail = &head; //head是下一层链表的虚拟头结点
            for (Node *cur = leftMost; cur; cur = cur->next) { 
                if (cur->left) {
                    tail->next = cur->left;
                    tail = tail->next;
                }
                if (cur->right) {
                    tail->next = cur->right;
                    tail = tail->next;
                }
            }
            leftMost = head.next;
        }
        return root;
    }
};
```
时间空间效率也很不错，应该算是这道题的最优解法：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了99.05% 的用户
内存消耗：17 MB, 在所有 C++ 提交中击败了91.02% 的用户
```

---
### 思路4 DFS先序遍历(根-右-左)
由于本题中递归程序占用的栈空间不算做额外的空间复杂度，所以可以使用递归解题。这里使用先序遍历，每次设置完当前结点的左、右孩子的 `next` 指针后，递归设置右子树和左子树的 `next` 指针。

和116题的做法不同，这样做能够**保证当前结点同一层的右侧结点的 `next` 已经完全连接**。在设置当前结点的孩子的 `next` 指针时，一旦当前结点的 `next` 指向的下一个右侧结点是叶子结点，**需要寻找存在孩子的下一个右侧结点时**，就能够继续往右寻找。
 

本题的递归遍历代码如下：
```cpp
class Solution {
private:
    Node* getNext(Node *root) {
        while (root) {
            if (root->left) return root->left;
            if (root->right) return root->right;
            root = root->next;
        }
        return nullptr;
    }
public:
    Node* connect(Node *root) {
        if (root == nullptr) return root;
        if (root->right) { //如果右孩子非空
            root->right->next = getNext(root->next);
            if (root->left) root->left->next = root->right; //就能确定左孩子的next
        } else if (root->left) root->left->next = getNext(root->next); //否则需要用getNext
        connect(root->right);
        connect(root->left);
        return root;
    }
}; 
```
效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了47.82% 的用户
内存消耗：17 MB, 在所有 C++ 提交中击败了89.01% 的用户
```

 
