Print a binary tree in an `m*n` 2D string array following these rules:
- The row number `m` should be equal to the height of the given binary tree.
 - The column number `n` should always be an odd number.
 - The root node's value (in string format) should be put in the exactly middle of the first row it can be put. The column and the row where the root node belongs will separate the rest space into two parts (**left-bottom part and right-bottom part**). You should print the left subtree in the left-bottom part and print the right subtree in the right-bottom part. The left-bottom part and the right-bottom part should have the same size. Even if one subtree is none while the other is not, you don't need to print anything for the none subtree but still need to leave the space as large as that for the other subtree. However, if two subtrees are none, then you don't need to leave space for both of them.
-  Each unused space should contain an empty string `""` .
- Print the subtrees following the same rules.

**Example 1:**

```swift
Input:
     1
    /
   2
Output:
[["", "1", ""],
 ["2", "", ""]]
```

**Example 2:**

```swift
Input:
     1
    / \
   2   3
    \
     4
Output:
[["", "", "", "1", "", "", ""],
 ["", "2", "", "", "", "3", ""],
 ["", "", "4", "", "", "", ""]]
```

**Example 3:**

```swift
Input:
      1
     / \
    2   5
   / 
  3 
 / 
4 
Output:

[["",  "",  "", "",  "", "", "", "1", "",  "",  "",  "",  "", "", ""]
 ["",  "",  "", "2", "", "", "", "",  "",  "",  "",  "5", "", "", ""]
 ["",  "3", "", "",  "", "", "", "",  "",  "",  "",  "",  "", "", ""]
 ["4", "",  "", "",  "", "", "", "",  "",  "",  "",  "",  "", "", ""]]
```

**Note:** The height of binary tree is in the range of `[1, 10]` . 
 
题意：在一个 `m*n` 的二维字符串数组中输出二叉树。规则如下：
1. 行数 `m` 应当等于给定二叉树的高度。
2. 列数 `n` 应当总是奇数。
3. 根节点的值（以字符串格式给出）应当放在可放置的第一行正中间。根节点所在的行与列会将剩余空间划分为两部分（**左下部分和右下部分**）。应该将左子树输出在左下部分，右子树输出在右下部分。左下和右下部分应当有相同的大小，即使一个子树为空而另一个非空。不需要为空的子树输出任何东西，但仍需要为另一个子树留出足够的空间。然而，如果两个子树都为空则不需要为它们留出任何空间。
4. 每个未使用的空间应包含一个空的字符串 `""` 。
5. 用相同的规则输出子树。


---
### 解法 填充字符串矩阵
解决方法很简单，唯一需要注意的是字符串矩阵的列数：
```cpp
class Solution {
public:
    vector<vector<string>> printTree(TreeNode* root) {
        int m = getDepth(root), n = (1 << m) - 1; //m层,最低层可能的节点数量为2^(m-1)
        //需要考虑到其他层的节点,因此字符串矩阵范围为m行n列(n = 2^m-1)
        vector<vector<string>> res(m, vector<string>(n));
        fillStringMatrix(res, root, 0, 0, n - 1);
        return res;
    } 
private:
    int getDepth(TreeNode* root) {
        if (!root) return 0;
        return max(getDepth(root->left), getDepth(root->right)) + 1;
    }
    void fillStringMatrix(vector<vector<string>>& res, TreeNode* root, int depth, int l, int r) {
        if (!root) return;
        string s = to_string(root->val);
        int pos = (l + r) / 2;
        res[depth][pos] = s;
        fillStringMatrix(res, root->left, depth + 1, l, pos - 1);
        fillStringMatrix(res, root->right, depth + 1, pos + 1, r);
    }
}; 
```

