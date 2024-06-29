<p>Given a&nbsp;binary tree with the following rules:</p>

<ol>
	<li><code>root.val == 0</code></li>
	<li>If <code>treeNode.val == x</code> and <code>treeNode.left != null</code>, then <code>treeNode.left.val == 2 * x + 1</code></li>
	<li>If <code>treeNode.val == x</code> and <code>treeNode.right != null</code>, then <code>treeNode.right.val == 2 * x + 2</code></li>
</ol>

<p>Now the binary tree is contaminated, which means all&nbsp;<code>treeNode.val</code>&nbsp;have&nbsp;been changed to <code>-1</code>.</p>

<p>You need to first recover the binary tree and then implement the <code>FindElements</code> class:</p>

<ul>
	<li><code>FindElements(TreeNode* root)</code>&nbsp;Initializes the object with a&nbsp;contamined binary tree, you need to recover it first.</li>
	<li><code>bool find(int target)</code>&nbsp;Return if the <code>target</code> value exists in the recovered binary tree.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 320px; height: 119px;" src="https://assets.leetcode.com/uploads/2019/11/06/untitled-diagram-4-1.jpg" alt=""></strong></p>


```css
Input:
["FindElements","find","find"]
[[[-1,null,-1]],[1],[2]]
Output:
[null,false,true]
Explanation:
FindElements findElements = new FindElements([-1,null,-1]); 
findElements.find(1); // return False 
findElements.find(2); // return True 
```

 
<p><strong>Example 2:</strong></p>

<p><strong><img style="width: 400px; height: 198px;" src="https://assets.leetcode.com/uploads/2019/11/06/untitled-diagram-4.jpg" alt=""></strong></p>

 

```css
Input:
["FindElements","find","find","find"]
[[[-1,-1,-1,-1,-1]],[1],[3],[5]]
Output:
[null,true,true,false]
Explanation:
FindElements findElements = new FindElements([-1,-1,-1,-1,-1]);
findElements.find(1); // return True
findElements.find(3); // return True
findElements.find(5); // return False
```

 
<p><strong>Example 3:</strong></p>

<p><strong><img style="width: 306px; height: 274px;" src="https://assets.leetcode.com/uploads/2019/11/07/untitled-diagram-4-1-1.jpg" alt=""></strong></p>

```css
Input:
["FindElements","find","find","find","find"]
[[[-1,null,-1,-1,null,-1]],[2],[3],[4],[5]]
Output:
[null,true,false,false,true]
Explanation:
FindElements findElements = new FindElements([-1,null,-1,-1,null,-1]);
findElements.find(2); // return True
findElements.find(3); // return False
findElements.find(4); // return False
findElements.find(5); // return True
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>TreeNode.val == -1</code></li>
	<li>The height of the binary tree is less than or equal to <code>20</code></li>
	<li>The total number of nodes is between <code>[1,&nbsp;10^4]</code></li>
	<li>Total calls of <code>find()</code> is between <code>[1,&nbsp;10^4]</code></li>
	<li><code>0 &lt;= target &lt;= 10^6</code></li>
</ul>



题意：给出一个满足下述规则的二叉树：
- `root.val == 0`
- 如果 `treeNode.val == x` 且 `treeNode.left != null` ，那么 `treeNode.left.val == 2 * x + 1`
- 如果 `treeNode.val == x` 且 `treeNode.right != null` ，那么 `treeNode.right.val == 2 * x + 2`

现在这个二叉树受到「污染」，所有的 `treeNode.val` 都变成了 `-1` 。请先还原二叉树，然后实现 `FindElements` 类：
- `FindElements(TreeNode* root)` 用受污染的二叉树初始化对象，需要先把它还原。
- `bool find(int target)` 判断目标值 `target` 是否存在于还原后的二叉树中并返回结果。

---
### 解法 递归还原+哈希集合
```cpp
class FindElements {
private:
    unordered_set<int> rec;
    void rebuild(TreeNode* root, int val) {
        if (root == nullptr) return;
        root->val = val;
        rec.insert(val);
        rebuild(root->left, val * 2 + 1);
        rebuild(root->right, val * 2 + 2);
    }
public:
    FindElements(TreeNode* root) { rebuild(root, 0); }
    bool find(int target) { return rec.find(target) != rec.end(); } //O(1)
};
```
运行效率如下：
```cpp
执行用时：40 ms, 在所有 C++ 提交中击败了99.63% 的用户
内存消耗：31.3 MB, 在所有 C++ 提交中击败了42.22% 的用户
```
