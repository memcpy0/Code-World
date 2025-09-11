<p>Given the <code>root</code> of a binary tree, the level of its root is <code>1</code>,&nbsp;the level of its children is <code>2</code>,&nbsp;and so on.</p>

<p>Return the <strong>smallest</strong> level <code>X</code> such that the sum of all the values of nodes at level <code>X</code> is <strong>maximal</strong>.</p>

 
<p><strong>Example 1:</strong></p>
<img style="width: 200px; height: 175px;" src="https://assets.leetcode.com/uploads/2019/05/03/capture.JPG" alt="">

```swift
Input: root = [1,7,0,7,-8,null,null]
Output: 2
Explanation: 
Level 1 sum = 1.
Level 2 sum = 7 + 0 = 7.
Level 3 sum = 7 + -8 = -1.
So we return the level with the maximum sum which is level 2. 
```

<p><strong>Example 2:</strong></p>

```swift
Input: root = [989,null,10250,98693,-89388,null,null,null,-32127]
Output: 2
```
<p><strong>Constraints:</strong></p>
<ul>
	<li>The number of nodes in the tree is in the range <code>[1, 10<sup>4</sup>]</code>.</li>
	<li><code>-10<sup>5</sup> &lt;= Node.val &lt;=&nbsp;10<sup>5</sup></code></li>
</ul>
</div>

题意：给定一个二叉树的根节点 `root` 。设根节点位于二叉树的第 `1` 层，而根节点的子节点位于第 `2` 层，依此类推。找出层内元素之和**最大**的那几层（可能只有一层）的层号，并返回其中**最小**的层号。 


---
### 解法 BFS
```cpp
class Solution {
public:
    int maxLevelSum(TreeNode* root) {
        if (!root) return 0;
        queue<TreeNode*> q;
        q.push(root);
        int ans = 0, maxsum = INT_MIN, level = 0;
        while (!q.empty()) {
            ++level;
            int size = q.size(), cursum = 0;
            for (int i = 0; i < size; ++i) {
                TreeNode *t = q.front(); q.pop();
                cursum += t->val;
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            if (cursum > maxsum) {
                maxsum = cursum;
                ans = level;
            }
        }
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：312 ms, 在所有 C++ 提交中击败了78.41% 的用户
内存消耗：106.3 MB, 在所有 C++ 提交中击败了36.67% 的用户
```
