You need to construct a string consists of parenthesis and integers from a binary tree with the preorder traversing way.

The `null` node needs to be represented by empty parenthesis pair `"()"` . And you need to omit all the empty parenthesis pairs that don't affect the one-to-one mapping relationship between the string and the original binary tree.

**Example 1:**

```swift
Input: Binary tree: [1,2,3,4]
       1
     /   \
    2     3
   /    
  4     

Output: "1(2(4))(3)"

Explanation: Originallay it needs to be "1(2(4)())(3()())", 
but you need to omit all the unnecessary empty parenthesis pairs. 
And it will be "1(2(4))(3)".
```

**Example 2:**

```swift
Input: Binary tree: [1,2,3,null,4]
       1
     /   \
    2     3
     \  
      4 

Output: "1(2()(4))(3)"

Explanation: Almost the same as the first example, 
except we can't omit the first parenthesis pair to break the one-to-one mapping relationship between the input and the output.
```

题意：从一棵二叉树中，使用先序遍历方式，构建一个由括号和整数组成的字符串。注意：空的结点需要用 `"()"` 代替，还需要省略所有的不影响字符串和原始二叉树之间一一对应关系的 `"()"` 。

> 一个已知的例子是LeetCode中给出的描述树结点的数组，使用层序遍历方式，从树中构建一个数组，省略所有不影响数组和原始树之间一一对应关系的 `null` 值。如 `[1,2,3,null,4]` ：
> ![在这里插入图片描述](https://img-blog.csdnimg.cn/20201006171025247.png#pic_center)
> 如果不使用层序序列，而是先序或后序序列的话，就需要两个数组，加上一个中序数组。因为单一数组无法表达树的递归关系。



---
### 思路 递归遍历
这里我们注意到，使用一个 `"()"` 分别包括左子树、右子树，令仅用一个线性表表达树关系成为可能。从示例中注意到：
- 如果一个结点是叶子结点，那么它的左右空子树无需表达，即省略对应的 `"()"` ；
- 如果一个结点有一个孩子，那么视情况而定：
	- 只有左孩子，那么空的右子树对应的 `"()"` 可以省略；
	- 只有右孩子，为了区分左右子树，空的左子树对应的 `"()"` 不可以省略；
- 如果一个结点有两个孩子，则都需要表达。

对应的代码很简单：
```cpp
class Solution {
public:
    string ans;
    void preorder(TreeNode* root) {
        ans.append(to_string(root->val));
        if (root->left) { //左子树非空
            ans.push_back('(');
            preorder(root->left);
            ans.push_back(')');
        } else if (root->right) ans.append("()"); //左子树为空但是右子树非空
        if (root->right) { //右子树非空
            ans.push_back('(');
            preorder(root->right);
            ans.push_back(')');
        }  
    }
    string tree2str(TreeNode* t) {
        if (t == nullptr) return "";
        preorder(t);
        return ans;
    }
};
```
效率如下：
```cpp
执行用时：24 ms, 在所有 C++ 提交中击败了94.61% 的用户
内存消耗：23.3 MB, 在所有 C++ 提交中击败了71.13% 的用户
```

 
