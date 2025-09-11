Given a binary tree, we install cameras on the nodes of the tree. 

Each camera at a node can monitor **its parent, itself, and its immediate children.**

Calculate the **minimum** number of cameras needed to monitor all nodes of the tree.

 

**Example 1:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200927220130959.png)
```c
Input: [0,0,null,0,0]
Output: 1
Explanation: One camera is enough to monitor all nodes if placed as shown.
```

**Example 2:**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200927220206458.png)

```c
Input: [0,0,null,0,null,0,null,null,0]
Output: 2
Explanation: At least two cameras are needed to monitor all nodes of the tree. The above image shows one of the valid configurations of camera placement.
```

**Note:**
- The number of nodes in the given tree will be in the range `[1, 1000]` .
 - Every node has value `0` .

题意：给定一个二叉树，我们在树的节点上安装摄像头。节点上的每个摄影头都可以监视其父对象、自身及其直接子对象。计算监控二叉树的所有节点所需的**最小摄像头数量**。

 

---
### 思路 分析结点状态
后序遍历，详细分析见代码：
```cpp
class Solution {
public:
    int total = 0;
    //0:节点没有安装监视器,且监视得到,当前节点不需要安装监视器,暗示上层节点不需要装监视器 
    //1:节点没有安装监视器,且监视不到,暗示上层节点需要装监视器
    //2:节点安装了监视器,暗示上层节点不需要装监视器 
    int dfs(TreeNode *root) {
        if (root == nullptr) return 0; //如果为空,视作可以监视,但是上层不用安装监视器
        int left = dfs(root->left), right = dfs(root->right); //后序遍历左右子树,探查其节点状态
        //1 0, 0 1, 1 2, 2 1, 1 1
        if (left == 1 || right == 1) { //如果左右子树有一个没有安装监视器,且监视不到
            ++total;                   //当前节点需要按照监视器
            return 2;                  //节点安装了监视器,返回2,上层节点不用安装监视器
        } //0 2, 2 0, 2 2
        if (left == 2 || right == 2)  //如果左右子树有一个安装监视器而另一个监视得到或都装了,当前节点就不用安装监视器
            return 0;                 //当前节点可以被监视,上层节点不需要安装监视器,返回0
        return 1;                     //探查的左右节点状态都为0,于是返回1,暗示上层节点需要安装监视器
    }
    int minCameraCover(TreeNode* root) {
        if (root == nullptr) return total; //空树则不用监视器
        if (dfs(root) == 1) ++total; //如果根节点看不到,加一监视器
        return total;
    }
};
```
