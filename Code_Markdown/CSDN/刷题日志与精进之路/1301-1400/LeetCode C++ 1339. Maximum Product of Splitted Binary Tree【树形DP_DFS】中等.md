<p>Given a binary tree <code>root</code>.&nbsp;Split the binary tree into two subtrees by removing&nbsp;1 edge such that the product of the sums of the subtrees are maximized.</p>

<p>Since the answer&nbsp;may be too large,&nbsp;return it modulo&nbsp;10^9 + 7.</p>

 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 495px; height: 200px;" src="https://assets.leetcode.com/uploads/2020/01/21/sample_1_1699.png" alt=""></strong></p>

```clike
Input: root = [1,2,3,4,5,6]
Output: 110
Explanation: Remove the red edge and get 2 binary trees with sum 11 and 10. Their product is 110 (11*10)
```
<p><strong>Example 2:</strong></p>

<p><img style="width: 495px; height: 200px;" src="https://assets.leetcode.com/uploads/2020/01/21/sample_2_1699.png" alt=""></p>

```clike
Input: root = [1,null,2,3,4,null,null,5,6]
Output: 90
Explanation:  Remove the red edge and get 2 binary trees with sum 15 and 6.Their product is 90 (15*6) 
```

<p><strong>Example 3:</strong></p>

```clike
Input: root = [2,3,9,10,7,8,6,5,4,11,1]
Output: 1025
```

<p><strong>Example 4:</strong></p>

```clike
Input: root = [1,1]
Output: 1 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li>Each tree has at most <code>50000</code> nodes and at least <code>2</code> nodes.</li>
	<li>Each node's value is between <code>[1, 10000]</code>.</li>
</ul>


题意：给你一棵二叉树，它的根为 `root` 。删除 `1` 条边，使二叉树分裂成两棵子树，且它们子树和的乘积尽可能大。 由于答案可能会很大，请将结果对 `10^9 + 7` 取模后再返回。 

---
### 解法 树形DP
很简单的题目，只要对每个根节点，判断左子树和剩余子树的乘积、右子树和剩余子树的乘积哪个更大，然后取所有更大乘积中最大的乘积即可。麻烦的是，需要注意取模和溢出的问题。
```cpp
#define mod(x) ((x) % 1000000007)
#define ll long long
class Solution {
private:
    ll ans = 0, tot = 0;
    ll sum(TreeNode* root) {
        if (root == nullptr) return 0;
        return sum(root->left) + sum(root->right) + root->val;
    }        
    int solve(TreeNode* root) {
        if (root == nullptr) return 0;
        ll lsum = solve(root->left), rsum = solve(root->right);   
        ans = max(ans, max(mod(lsum) * mod(tot - lsum), mod(rsum) * mod(tot - rsum)));
        return lsum + rsum + root->val;
    } 
public:
    int maxProduct(TreeNode* root) {
        tot = sum(root);
        solve(root);
        return mod(ans);
    }
};
```
运行效率如下：
```cpp
执行用时：144 ms, 在所有 C++ 提交中击败了85.27% 的用户
内存消耗：76.2 MB, 在所有 C++ 提交中击败了58.90% 的用户
```
