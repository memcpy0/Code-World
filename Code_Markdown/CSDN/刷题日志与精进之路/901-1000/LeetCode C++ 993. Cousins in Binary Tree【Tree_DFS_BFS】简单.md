
In a binary tree, the root node is at depth `0`, and children of each depth `k` node are at depth `k+1`.

Two nodes of a binary tree are *cousins* if they have the same depth, but have **different parents**.

We are given the `root` of a binary tree with unique values, and the values `x` and `y` of two different nodes in the tree.

Return `true` if and only if the nodes corresponding to the values `x` and `y` are cousins.

 

**Example 1:**

```swift
Input: root = [1,2,3,4], x = 4, y = 3
Output: false
```

**Example 2:**

```swift
Input: root = [1,2,3,null,4,null,5], x = 5, y = 4
Output: true
```

**Example 3:**

```swift
Input: root = [1,2,3,null,4], x = 2, y = 3
Output: false
```

 

**Constraints:**
- The number of nodes in the tree will be between `2` and `100`.
- Each node has a unique integer value from `1` to `100`.

题意：在二叉树中，根节点位于深度 `0` 处，每个深度为 `k` 的节点的子节点位于深度 `k + 1` 处。如果二叉树的两个节点深度相同，但父节点不同，则它们是一对堂兄弟节点。给出**所有值都唯一的二叉树**的根节点 `root` ，以及树中两个不同节点的值 `x` 和 `y` ，如果对应的节点是堂兄弟节点则返回 `true` ，否则返回 `false` 。


---
### 解法1 DFS
边界条件是——如果有一个值对应的节点是根节点（**没有父节点**且**没有同深度的节点**），则这两个节点不可能是堂兄弟节点。然后分别对左、右子树进行DFS，得到 `x, y` 对应节点的深度和父节点，比较深度和父节点来判断是否是堂兄弟节点：
```cpp 
class Solution {
private:
    int xdepth = 0, ydepth = 0, xparent = 0, yparent = 0; //xparent,yparent是x,y父节点的值, 因为每个节点的值都不同
    void dfs(TreeNode* root, int x, int y, int parent, int depth) {
        if (root == nullptr) return;
        if (root->val == x) {
            xdepth = depth;
            xparent = parent;
        } else if (root->val == y) {
            ydepth = depth;
            yparent = parent;
        } else {
            dfs(root->left, x, y, root->val, depth + 1);
            dfs(root->right, x, y, root->val, depth + 1);
        }
    }
public:
    bool isCousins(TreeNode* root, int x, int y) {
        if (x == root->val || y == root->val) return false; //一个是根节点的值,就必然不是cousins
        dfs(root->left, x, y, root->val, 1); //分别遍历左右子树
        dfs(root->right, x, y, root->val, 1); //从第1层开始计数
        return (xdepth == ydepth) && (xparent != yparent);
    }
};
```
执行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了80.28% 的用户
内存消耗：11.2 MB, 在所有 C++ 提交中击败了31.15% 的用户
```
---
### 解法2 BFS
使用层序遍历，如果两个节点在同一层，它们就是堂兄弟节点。如果当前层只能找到其中一个节点，说明位于不同层，直接返回 `false` 。注意，同一层时还需要排除一种特殊情况，即这两个节点的父节点相同，此时返回 `false` ——具体处理方法是**使用序号标记父节点**：
```cpp
class Solution {
public:
    bool isCousins(TreeNode* root, int x, int y) {
        if (x == root->val || y == root->val) return false; //一个是根节点的值,就必然不是cousins
        queue<TreeNode*, list<TreeNode*>> q;
        q.push(root);
        while (!q.empty()) { //BFS
            int size = q.size();
            int tx = -1, ty = -1; //记录x,y的父节点的次序号,只要能区分二者父结点是否相同即可
            for (int i = 0; i < size; ++i) {
                TreeNode *rhs = q.front(); q.pop(); 
                if (rhs->left) {
                    if (rhs->left->val == x) tx = i;
                    else if (rhs->left->val == y) ty = i;
                    q.push(rhs->left);
                } 
                if (rhs->right) {
                    if (rhs->right->val == x) tx = i;
                    else if (rhs->right->val == y) ty = i;
                    q.push(rhs->right);
                } 
            } 
            if (tx != -1 && ty != -1) { 	//都在这一层
                if (tx == ty) return false; //同一个父节点
                else return true;           //非同一个父节点
            } else if (tx != -1 || ty != -1) return false; //一个不在这一层
            //else if (tx == -1 && ty == -1)//都不在这一层 
        }
        return false;
    }
};
```
效率如下，比较低：
```cpp
执行用时：8 ms, 在所有 C++ 提交中击败了36.53% 的用户
内存消耗：11.3 MB, 在所有 C++ 提交中击败了21.52% 的用户
```
其实遍历完一层之后，再判断是否处于同一层、同层时父节点是否相同，效率不高——**完全可以提前到循环中：**
```cpp
class Solution {
public:
    bool isCousins(TreeNode* root, int x, int y) {
        if (x == root->val || y == root->val) return false; //一个是根节点的值,就必然不是cousins
        queue<TreeNode*, list<TreeNode*>> q;
        q.push(root);
        while (!q.empty()) { //BFS
            int size = q.size();
            int tx = -1, ty = -1; //记录x,y的父节点的次序号,只要能区分二者父结点是否相同即可
            for (int i = 0; i < size; ++i) {
                TreeNode *rhs = q.front(); q.pop(); 
                if (rhs->left) {
                    if (rhs->left->val == x) tx = i;
                    else if (rhs->left->val == y) ty = i;
                    q.push(rhs->left);
                } 
                if (rhs->right) {
                    if (rhs->right->val == x) tx = i;
                    else if (rhs->right->val == y) ty = i;
                    q.push(rhs->right);
                } 
                if (tx != -1 && ty != -1) {     //同层
                    if (tx == ty) return false; //同父节点
                    else return true;           //不同父节点
                }
            }
            if (tx != -1 || ty != -1) return false; //一个不在这一层  
        }
        return false;
    }
};
```
效率提升了一些：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了80.28% 的用户
内存消耗：11.3 MB, 在所有 C++ 提交中击败了19.78% 的用户
```
