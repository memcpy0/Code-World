<p>Given the <code>root</code> of a Binary Search Tree and a target number <code>k</code>, return <em><code>true</code> if there exist two elements in the BST such that their sum is equal to the given target</em>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 562px; height: 322px;" src="https://assets.leetcode.com/uploads/2020/09/21/sum_tree_1.jpg" alt="" width="40%">

```cpp
Input: root = [5,3,6,2,4,null,7], k = 9
Output: true
```
<p><strong>Example 2:</strong></p>
<img style="width: 562px; height: 322px;" src="https://assets.leetcode.com/uploads/2020/09/21/sum_tree_2.jpg" alt=""  width="40%">

```cpp
Input: root = [5,3,6,2,4,null,7], k = 28
Output: false
```

<p><strong>Example 3:</strong></p>

```cpp
Input: root = [2,1,3], k = 4
Output: true
```

<p><strong>Example 4:</strong></p>

```cpp
Input: root = [2,1,3], k = 1
Output: false
```

<p><strong>Example 5:</strong></p>

```cpp
Input: root = [2,1,3], k = 3
Output: true
```
<p><strong>Constraints:</strong></p>
<ul>
	<li>The number of nodes in the tree is in the range <code>[1, 10<sup>4</sup>]</code>.</li>
	<li><code>-10<sup>4</sup>&nbsp;&lt;= Node.val &lt;= 10<sup>4</sup></code></li>
	<li><code>root</code> is guaranteed to be a <strong>valid</strong> binary search tree.</li>
	<li><code>-10<sup>5</sup>&nbsp;&lt;= k &lt;= 10<sup>5</sup></code></li>
</ul>



---
### 解法1 中序遍历+辅助数组+双指针
用数组记录二叉搜索树的中序遍历序列，然后对这一有序数列使用双指针，判断是否存在两个数，其和等于 `k` ：
```cpp
class Solution {
private:
    vector<int> nums;
    void inorder(TreeNode* root) {
        if (root) {
            inorder(root->left);
            nums.push_back(root->val);
            inorder(root->right);
        }
    }
public:
    bool findTarget(TreeNode* root, int k) {
        if (root == nullptr) return false;
        inorder(root);
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int v = nums[left] + nums[right];
            if (v == k) return true;
            else if (v < k) ++left;
            else --right;
        }
        return false;
    }
};
```
提交后效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了82.82% 的用户
内存消耗：36.5 MB, 在所有 C++ 提交中击败了22.31% 的用户
```
使用一个栈进行迭代中序遍历，则代码如下：
```cpp
class Solution { 
public:
    bool findTarget(TreeNode* root, int k) {
        if (root == nullptr) return false;
        vector<int> nums;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            nums.push_back(root->val);
            root = root->right;
        } 
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int v = nums[left] + nums[right];
            if (v == k) return true;
            else if (v < k) ++left;
            else --right;
        }
        return false;
    }
};
```
提交后结果如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了90.90% 的用户
内存消耗：36.6 MB, 在所有 C++ 提交中击败了21.52% 的用户
```
---
### 解法二 树遍历+哈希集合
先序遍历整棵二叉搜索树，每到一个节点 `root` ，在哈希集合中寻找 `k - root->val` 是否存在，出现了就返回 `true` ，没有出现则添加节点值到哈希集合中，然后对左右子树进行遍历。
```cpp
class Solution {
public:
    unordered_set<int> record;
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        if (record.find(k - root->val) != record.end())
            return true;
        record.insert(root->val);
        return findTarget(root->left, k) || findTarget(root->right, k);
    }
};
```
似乎效率比较低下：
```cpp
执行用时：80 ms, 在所有 C++ 提交中击败了33.62% 的用户
内存消耗：38.7 MB, 在所有 C++ 提交中击败了8.47% 的用户
```
中序遍历也可以：
```cpp
class Solution { 
public:
    unordered_set<int> record;
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        if (findTarget(root->left, k)) return true;
        if (record.find(k - root->val) != record.end())
            return true;
        record.insert(root->val);
        return findTarget(root->right, k);
    }
};
```
效率稍高一点：
```cpp
执行用时：64 ms, 在所有 C++ 提交中击败了73.36% 的用户
内存消耗：38.9 MB, 在所有 C++ 提交中击败了7.97% 的用户
```
后序遍历的代码如下：
```cpp
class Solution { 
public:
    unordered_set<int> record;
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        if (findTarget(root->left, k) || findTarget(root->right, k)) return true;
        if (record.find(k - root->val) != record.end()) return true;
        record.insert(root->val);
        return false;
    }
};
```
效率也比较低下：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了27.07% 的用户
内存消耗：39.5 MB, 在所有 C++ 提交中击败了5.08% 的用户
```
对应的迭代版本如下：
```cpp
//迭代先序遍历
class Solution { 
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        unordered_set<int> record;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                if (record.find(k - root->val) != record.end()) return true;
                record.insert(root->val);
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            root = root->right;
        } 
        return false;
    }
};
//迭代中序遍历
class Solution { 
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        unordered_set<int> record;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            root = st.top(); st.pop();
            if (record.find(k - root->val) != record.end()) return true;
            record.insert(root->val);
            root = root->right;
        } 
        return false;
    }
};
//迭代后序遍历
class Solution { 
public:
    bool findTarget(TreeNode* root, int k) {
        if (!root) return false;
        unordered_set<int> record;
        stack<TreeNode*> st;
        while (root || !st.empty()) {
            while (root) {
                st.push(root);
                root = root->left;
            }
            TreeNode *temp = st.top(); 
            root = temp->right;
            if (root == nullptr) { //右子树为空
                if (record.find(k - temp->val) != record.end()) return true;
                record.insert(temp->val); //访问当前子树根节点
                st.pop();  
                while (!st.empty() && st.top()->right == temp) {
                    temp = st.top(); st.pop();  
                    if (record.find(k - temp->val) != record.end()) return true;
                    record.insert(temp->val); 
                }
            } 
        }   
        return false;
    }
};
```
总的来说，和第一个Two Sum题目一样，**关键点是先查找目标值、后插入当前值。**

