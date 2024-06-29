<p>Given an n-ary tree, return the <i>postorder</i> traversal of its nodes' values.</p>

<p><em>Nary-Tree input serialization&nbsp;is represented in their level order traversal, each group of children is separated by the null value (See examples).</em></p>
 

<p><strong>Follow up:</strong></p>

<p>Recursive solution is trivial, could you do it iteratively?</p>

 
<p><strong>Example 1:</strong></p>

<img style="width: 100%; max-width: 300px;" src="https://assets.leetcode.com/uploads/2018/10/12/narytreeexample.png" width="45%">

```cpp
Input: root = [1,null,3,2,4,null,5,6]
Output: [5,6,3,2,4,1]
```

<p><strong>Example 2:</strong></p>

<p><img style="width: 296px; height: 241px;" src="https://assets.leetcode.com/uploads/2019/11/08/sample_4_964.png" alt="" width="45%">

```cpp
Input: root = [1,null,2,3,4,5,null,null,6,7,null,8,null,9,10,null,null,11,null,12,null,13,null,null,14]
Output: [2,6,14,11,7,3,12,8,4,13,9,10,5,1]
```

<p>&nbsp;</p>
<p><strong>Constraints:</strong></p>

<ul>
	<li>The height of the n-ary tree is less than or equal to <code>1000</code></li>
	<li>The total number of nodes is between <code>[0,&nbsp;10^4]</code></li>
</ul>
 
题意：给定一个 `N` 叉树，返回其节点值的后续遍历。

---
### 思路1 递归后序遍历
```cpp
class Solution {
public:
    vector<int> ans;
    void postorderTraversal(Node* root) {
        if (root) {
            for (Node *&it : root->children)
                postorderTraversal(it);
            ans.push_back(root->val);
        }
    }
    vector<int> postorder(Node* root) {
        postorderTraversal(root);
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了33.29% 的用户
内存消耗：11.5 MB, 在所有 C++ 提交中击败了41.71% 的用户
```
---
### 思路2 非递归后序遍历
使用简易版（双栈法）非递归后序遍历，代码如下：
```cpp
class Solution {
public:
    vector<int> postorder(Node* root) {
        if (root == nullptr) return {};
        vector<int> ans;
        stack<Node*> st;
        st.push(root);
        while (!st.empty()) {
            Node* t = st.top(); st.pop();
            ans.push_back(t->val);
            for (Node* child : t->children)
                st.push(child);
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：44 ms, 在所有 C++ 提交中击败了75.95% 的用户
内存消耗：11.3 MB, 在所有 C++ 提交中击败了85.25% 的用户
```
