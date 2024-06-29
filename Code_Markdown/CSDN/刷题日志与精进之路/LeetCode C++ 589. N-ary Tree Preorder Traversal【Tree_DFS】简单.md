<p>Given an n-ary tree, return the <i>preorder</i> traversal of its nodes' values.</p>

<p><em>Nary-Tree input serialization&nbsp;is represented in their level order traversal, each group of children is separated by the null value (See examples).</em></p>
 

<p><strong>Follow up:</strong></p> 
<p>Recursive solution is trivial, could you do it iteratively?</p>

 
<p><strong>Example 1:</strong></p>

<img style="width: 100%; max-width: 300px;" src="https://assets.leetcode.com/uploads/2018/10/12/narytreeexample.png" width="45%">

```cpp
Input: root = [1,null,3,2,4,null,5,6]
Output: [1,3,5,6,2,4]
```
<p><strong>Example 2:</strong></p>

<img style="width: 296px; height: 241px;" src="https://assets.leetcode.com/uploads/2019/11/08/sample_4_964.png" alt="" width="45%">

```
Input: root = [1,null,2,3,4,5,null,null,6,7,null,8,null,9,10,null,null,11,null,12,null,13,null,null,14]
Output: [1,2,3,6,7,11,14,4,8,12,5,9,13,10]
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li>The height of the n-ary tree is less than or equal to <code>1000</code></li>
	<li>The total number of nodes is between <code>[0,&nbsp;10^4]</code></li>
</ul> 

题意：对N叉树进行先序遍历。

---
### 思路1 递归遍历 
```cpp
class Solution {
public:
    vector<int> ans;
    vector<int> preorder(Node* root) {
        traversal(root);
        return ans;
    }
    void traversal(Node* root) {
        if (root) {
            ans.push_back(root->val);
            for (int i = 0; i < root->children.size(); ++i) 
                preorder(root->children[i]);
        }
    }
};
```
效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了75.59% 的用户
内存消耗：104.3 MB, 在所有 C++ 提交中击败了22.77% 的用户
```

---
### 思路2 非递归遍历
如果使用教科书版本的非递归先序遍历，很难想到解题思路。但是如果使用简单版本，就很容易解决这道题：
- 二叉树的非递归先序遍历中，每次将当前结点的右孩子节点和左孩子节点依次压入栈中，注意是先右后左。然后将出栈节点输出，并且在将其右子节点和左子节点压入栈中。
- 推广到N叉树，就是将当前结点的孩子节点由右到左依次压入栈中。然后将出栈节点输出，并且将其孩子节点依次压入栈中。

时间和空间复杂度都是 $O(n)$ 。代码如下：
```cpp
class Solution {
public:
    vector<int> preorder(Node* root) {
        if (root == nullptr) return {};
        vector<int> ans;
        stack<Node*> st;
        st.push(root);
        while (!st.empty()) {
            Node *temp = st.top(); st.pop();
            ans.push_back(temp->val); //使用右值引用,注意:it是一个左值
            for (auto &&it = temp->children.rbegin(); it != temp->children.rend(); ++it) 
                st.push(*it); 
        }
        return ans;
    } 
};
```
效率如下，没有函数调用的栈空间消耗：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了75.59% 的用户
内存消耗：11.4 MB, 在所有 C++ 提交中击败了58.87% 的用户
```
