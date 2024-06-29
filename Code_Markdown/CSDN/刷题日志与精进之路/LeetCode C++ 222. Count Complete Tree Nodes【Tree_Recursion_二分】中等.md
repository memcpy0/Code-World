

Given a **complete** binary tree, count the number of nodes.

**Note:** In a complete binary tree every level, except possibly the last, is completely filled, and all nodes in the last level are as far left as possible. It can have between 1 and 2^h^ nodes inclusive at the last level h.

**Example:**
```css
Input: 
    1
   / \
  2   3
 / \  /
4  5 6

Output: 6
```


题意：计算一棵完全二叉树的节点个数。

---
### 解法1 迭代遍历
迭代先序遍历的代码如下：
```cpp
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        int nodes = 0;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            ++nodes;
            root = root->right;
        }
        return nodes;
    }
};
```
效率如下:
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了76.60% 的用户
内存消耗：30.6 MB, 在所有 C++ 提交中击败了17.96% 的用户
```
迭代中序遍历的代码如下：
```cpp
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        int nodes = 0;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                ++nodes;
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            root = root->right;
        }
        return nodes;
    }
};
```
效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了39.32% 的用户
内存消耗：30.8 MB, 在所有 C++ 提交中击败了16.02% 的用户
```
迭代后续遍历：
```cpp
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        int nodes = 0;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            TreeNode *temp = st.top(); 
            root = temp->right;
            if (root == nullptr) {
                ++nodes; st.pop();
                while (!st.empty() && st.top()->right == temp) {
                    ++nodes;
                    temp = st.top(); st.pop();
                }
            } 
        }
        return nodes;
    }
};
```
效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了76.60% 的用户
内存消耗：30.7 MB, 在所有 C++ 提交中击败了16.64% 的用户
```
---
### 解法2 递归遍历
简单的代码，而且效率不高：
```cpp
private:
    int ans;
    void preorder(TreeNode* root) {
        if (root) {
            ++ans;
            preorder(root->left);
            preorder(root->right);
        }
    }
public:
    int countNodes(TreeNode* root) {
        preorder(root);
        return ans;
    }
};
//-----------------------------------------
class Solution {
private:
    int ans;
    void inorder(TreeNode* root) {
        if (root) {
            inorder(root->left);
            ++ans;
            inorder(root->right);
        }
    }
public:
    int countNodes(TreeNode* root) {
        inorder(root);
        return ans;
    }
};
//-----------------------------------------
class Solution {
private:
    int ans;
    void postorder(TreeNode* root) {
        if (root) {
            postorder(root->left);
            postorder(root->right);
            ++ans;
        }
    }
public:
    int countNodes(TreeNode* root) {
        postorder(root);
        return ans;
    }
};
```
其中后序遍历的效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了58.32% 的用户
内存消耗：30.5 MB, 在所有 C++ 提交中击败了18.68% 的用户
```
或者写成如下形式：
```cpp
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
    }
};
```
效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了39.26% 的用户
内存消耗：30.8 MB, 在所有 C++ 提交中击败了16.49% 的用户
```

---
### 解法3 广度优先遍历
```cpp
class Solution {
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        int nodes = 0;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode *t = q.front(); q.pop();
            ++nodes;
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
        }
        return nodes;
    }
};
```
效率如下：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了76.50% 的用户
内存消耗：31.5 MB, 在所有 C++ 提交中击败了5.32% 的用户
```
---
### 解法4 最优解法
利用完全二叉树的递归特征，可以将时间复杂度降低到 $\log n$ 级别左右。**这一点可能难以想到**。众所周知，完全二叉树中除了最底层节点可能没填满之外，其余每层的节点数都是最大值，并且最下面一层的节点都集中在该层最左边的若干位置（若最底层为第 $h$ 层，则该层有 $1 \sim 2^h$ 个节点）。不过更加有意思的是，**完全二叉树的子树也是完全二叉树**，于是我们可以递归求解完全二叉树的节点个数。

对于一棵完全二叉树，要么是一棵满二叉树（此时左右子树都是满二叉树，不过这一点难以直接判断），要么左子树是满二叉树、右子树是完全二叉树，要么右子树是满二叉树、左子树是完全二叉树，甚至左右子树都是满二叉树：
```cpp
//满二叉树,左右子树都是满二叉树
     1
   /   \
  2     3
 / \   / \
4  5  6   7
//完全二叉树,左子树是满二叉树、右子树是完全二叉树
//右子树的左子树是满二叉树、右子树的右子树也是满二叉树
     1
   /   \
  2     3
 / \   /  
4  5  6    
//完全二叉树,左子树是完全二叉树、右子树是满二叉树
//左子树的左子树是满二叉树、左子树的右子树也是满二叉树
     1
   /   \
  2     3
 /    
4   
```
由于完全二叉树中更可能存在的是左子树，于是以子树的左路径长度为层数，当左右子树的层数相等时，知道**最后一层的最后一个节点一定在右子树中**，于是左子树是满二叉树，运用公式求出**左子树节点个数**，加上**根节点**以及**递归求出的右子树节点个数**即可；当左右子树的层数不等时，显然左子树的层数更深，于是**最后一层的最后一个节点一定在左子树**，右子树是满二叉树，运用公式求出**右子树节点个数**，加上**根节点**以及**递归求出的左子树节点个数**即可。实际代码如下：
```cpp
class Solution {
private: //完全二叉树由于最后一层节点都集中在最左边,于是以左子树的层数为depth
    int getLeftDepth(TreeNode* root) {
        int depth = 0;
        while (root) {
            root = root->left;
            ++depth;
        }
        return depth;
    }
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        int lLeft = getLeftDepth(root->left);
        int rLeft = getLeftDepth(root->right);
        //左右子树层数相等时,说明左子树是满二叉树,递归查看右子树
        if (lLeft == rLeft)
            return (pow(2, lLeft) - 1) + 1 + countNodes(root->right);
        //左右子树层数不等(左子树层数更大),说明右子树为满二叉树,递归查看左子树
        else
            return (pow(2, rLeft) - 1) + 1 + countNodes(root->left); 
    }
};
```
简练一点的代码如下：
```cpp
class Solution {
	//......
    int countNodes(TreeNode* root) {
        //.....
        if (lLeft == rLeft) return (1 << lLeft) + countNodes(root->right); 
        else return (1 << rLeft) + countNodes(root->left); 
    }
};
```
这样做效率还是不高，原因是有的节点会多次重复统计层数：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了76.50% 的用户
内存消耗：30.6 MB, 在所有 C++ 提交中击败了18.06% 的用户
```
为此使用非递归方法：
```cpp
class Solution { 
private:
    int getDepth(TreeNode* root) {
        int depth = 0;
        while (root) {
            ++depth;
            root = root->left;
        }
        return depth;
    }
public:
    int countNodes(TreeNode* root) {
        if (!root) return 0;
        TreeNode *cur = root;
        int left = getDepth(root->left), right, ret = 0;
        while (cur) {
            right = getDepth(cur->right); //当前子树右子树的层数
            if (left == right) { //左右子树高度相同,说明左子树满,右子树可能满或不满
                ret += (1 << left);  //加上左子树的节点数+根节点
                cur = cur->right;    //迭代右子树
                left = right - 1;    //右子树的左子树层数可知
            } else {
                ret += (1 << right); //加上右子树的节点数+根节点
                cur = cur->left;     //迭代左子树
                left = left - 1;     //左子树的左子树层数可知
            }
        }
        return ret;
    }
};
```
效率如下，按理来说应该更高：
```cpp
执行用时：52 ms, 在所有 C++ 提交中击败了76.40% 的用户
内存消耗：30.6 MB, 在所有 C++ 提交中击败了18.25% 的用户
```
