You are given an integer array nums and you have to return a new `counts` array. The counts array has the property where `counts[i]` is the number of smaller elements to the right of `nums[i]` .

**Example**:
```cpp
Input: [5,2,6,1]
Output: [2,1,1,0] 
Explanation:
To the right of 5 there are 2 smaller elements (2 and 1).
To the right of 2 there is only 1 smaller element (1).
To the right of 6 there is 1 smaller element (1).
To the right of 1 there is 0 smaller element.
```
题意：给出一个整数数组 `nums` ，返回一个 `count` 数组，其中 `count[i]` 是 `nums[i]` 右部中小于 `nums[i]` 的数的数量。

---
思路1：题意很简单。但是要实现高效快速的算法还是有点麻烦。最简单的思路就是二重循环，每次对 `nums[i]` ，计算其 `nums[i + 1 : -1]` 中有多少小于 `nums[i]` 的数。$O(n^2)$ ，在困难题中绝对会超时。

思路2：正向查找比较麻烦。如果反向插入的话，其右边的数已经准备好了，容易计算。以题目给出的 `[5,2,6,1]` 为例：
```
插入1 [1]      : 小于1的数量: 0
插入6 [6,1]    : 小于6的数量: 1
插入2 [2,6,1]  : 小于2的数量: 1
插入5 [5,2,6,1]: 小于5的数量: 2
```
现在，只要找到一种容易插入且方便计数的数据结构就可以了。这里先用二叉搜索树，需要注意的有：
- ① 反序加入二叉搜索树。对于 `nums[i]` 来说，进入BST时，它右边的全部元素都在树中了，此时可以在向右子树插入的过程中不断计算出 `count[i]` —— 递归往右子树插入时，所有**根节点和其左子树的节点数量**就是小于 `nums[i]` 的元素数量。
- ② 需要实现的代码是二叉搜索树的节点 `node` ，和插入方法 `insert` ；`insert` 需要有如下功能：
	- 构建二叉搜索树；
	- 维护每个节点中存储的其左子树节点数量值 `cnt` ；如果新加入的节点 `<=` 当前节点值，则插入当前节点的左子树（重复元素也要有节点；这样做就不会计算错误），同时当前节点的 `cnt++` ；
	- 新加入节点 `>` 当前节点，则插入右子树，计算出新加入节点 `nums[i]` 的“右侧小于当前元素的个数”，即当前节点+它存储的 `cnt` 。
- ③ 另外我试过，递归插入的过程中，遇到右子树，DFS求出其左子树的节点数量……但是这样太慢。

代码2：
```cpp
struct node {
    int val; //节点值
    int cnt; //左子树的节点数量
    node *left, *right;
    node(int v) : val(v), cnt(0), left(NULL), right(NULL) {}
};

class Solution {
public:    
    void insert(node *&root, int x, vector<int> &ans, int index) {
        if (!root) {
            root = new node(x);  
        } else if (x <= root->val) {           //<,二叉搜索树,插到左边;
            ++root->cnt;                       //root左侧节点数量加1
            insert(root->left, x, ans, index); //=即重复元素,往左边插入
        } else {                               //x大于root->val,插入右边
            ans[index] += (root->cnt + 1);     //x>root及其左子树的全部结点,<x的数量+=根和其左子树的结点数量
            insert(root->right, x, ans, index); 
        }
    }

    vector<int> countSmaller(vector<int>& nums) {
        node *root = nullptr;
        vector<int> ans(nums.size(), 0);
        for (int i = nums.size() - 1; i >= 0; --i) 
            insert(root, nums[i], ans, i); //bst的根节点,反向加入bst的值,结果序列,要填入的下标
        return ans;
    }
};
```
另外，还可以用线段树、树状数组、归并(分治)、二分的方法求解这一道题。日后有时间再写。
