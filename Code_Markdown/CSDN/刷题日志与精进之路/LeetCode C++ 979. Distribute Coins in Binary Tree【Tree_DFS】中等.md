Given the `root` of a binary tree with `N` nodes, each `node` in the tree has `node.val` coins, and there are `N` coins total.

In one move, we may choose two adjacent nodes and move one coin from one node to another.  (The move may be from parent to child, or from child to parent.) Return the number of moves required to make every node have exactly one coin.

 

**Example 1:**
<img src="https://img-blog.csdnimg.cn/20200922140318254.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
```cpp
Input: [3,0,0]
Output: 2
Explanation: From the root of the tree, we move one coin to its left child, and one coin to its right child.
```

**Example 2:**
<img src="https://img-blog.csdnimg.cn/20200922140334434.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
```cpp
Input: [0,3,0]
Output: 3
Explanation: From the left child of the root, we move two coins to the root [taking two moves].  Then, we move one coin from the root of the tree to the right child.
```

**Example 3:**
<img src="https://img-blog.csdnimg.cn/20200922140345435.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">
```cpp
Input: [1,0,2]
Output: 2
```

**Example 4:**
<img src="https://img-blog.csdnimg.cn/20200922140354100.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="35%">

```cpp
Input: [1,0,0,null,3]
Output: 4
```

**Note:**
-  `1<= N <= 100`
-  `0 <= node.val <= N`
 
题意：给出一棵二叉树，有 `N` 个结点，每个结点有 `node.val` 枚硬币，总共有 `N` 枚硬币。在一次移动中，可以将一枚硬币从一个结点移动到相邻的某个结点，即子结点到父结点或父结点到子结点。**返回使得每个结点上只有一枚硬币所需要的移动次数。

---
思路：如果一个结点 `node` 上有多于一枚的硬币，就需要从 `node` 移动硬币到其他结点；如果一个结点 `node` 上没有硬币，就需要从其他结点移动硬币到 `node` ；如果一个结点 `node` 上只有一枚硬币，就不需要移入也不需要移出。我就只想到这里，具体的做法还是看了答案才知道的。

使用递归后序遍历的方法，在处理当前结点之前，已经递归处理完成了左右子树的所有结点，然后设当前结点的硬币数目为 `x` ，剩余 `x - 1` 枚硬币会在与父结点之间进行移动：
- 如果 `x < 1` ，说明硬币不够，要从父结点获得，因此子结点会移入 `|x - 1|` 枚硬币，而得到一枚硬币。即 `|x - 1|` 个入方向的操作，移动次数 `+ |x - 1|` ；
- 如果 `x == 1` ，就不需要移入或移出；
- 如果 `x > 1` ，说明硬币多余，需要交给父结点，因此子结点会移出 `|x - 1|` 枚硬币，只留下一枚硬币。即 `|x - 1|` 个出方向的操作，移动次数 `+ |x - 1|` 。
- 左右子树和当前结点处理完毕之后，这一子树也处理完成；最后，需要递归向上传递移入/移出的硬币数目，修改当前结点的父结点的 `val` 值。
- 于是移动次数为所有结点硬币的移入和移出次数之和。**注意，这一过程中需要修改结点的 `val` 值**。

以示例4为例，后序遍历首先到结点 `C` ，发现有 `3` 枚硬币，会有 `3 - 1 = 2` 次移出，于是 `C.val = 1, B.val = 2` ；然后到结点 `B` ，发现有 `2` 枚硬币，就需要 `1` 次移出，于是移动到了 `A` 处，`B.val = 1, A.val = 2` ；接着是 `D` 结点，发现没有硬币，需要一次移入，于是 `D.val = 1, A.val = 1` ；最后是 `A` 结点，发现有一枚硬币，就不需要移动。
<img src="https://img-blog.csdnimg.cn/20200922142211509.png#pic_center" width="35%">
 
 代码如下：
```cpp
class Solution {
public:
    int moveNum = 0;
    int distributeCoins(TreeNode* root) {
        postorder(root);
        return moveNum; //返回移动的次数
    }
    int postorder(TreeNode *root) { //后序遍历
        if (root == nullptr) return 0;
        //处理子树,子树多余的硬币送到父结点,子树缺少的硬币会从父结点扣除
        root->val += postorder(root->left); 
        root->val += postorder(root->right);
        //此时左右子树均已递归处理完成,它们的结点值均为1
        int coinsMoved = root->val - 1; //当前子树需要移出abs(val-1)枚硬币到父结点/从父结点移入abs(val-1)枚硬币
        root->val = 1;                  //当前子树根结点的值为1
        moveNum += abs(coinsMoved);     //移动次数+abs(val-1)
        return coinsMoved;              //返回从root移出到父结点/从父结点移入到root的硬币数目
    }
};
```
效率如下：
```cpp
执行用时：12 ms, 在所有 C++ 提交中击败了49.42% 的用户
内存消耗：13.6 MB, 在所有 C++ 提交中击败了51.11% 的用户
```
