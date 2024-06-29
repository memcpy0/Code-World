<p>Given the <code>root</code> of a binary tree, turn the tree upside down and return <em>the new root</em>.</p>

<p>You can turn a binary tree upside down with the following steps:</p>

<ol>
	<li>The original left child becomes the new root.</li>
	<li>The original root becomes the new right child.</li>
	<li>The original right child&nbsp;becomes the new left child.</li>
</ol>

<p><img style="width: 100%; height: 100%;" src="https://assets.leetcode.com/uploads/2020/08/29/main.jpg" alt=""></p>

<p>The mentioned steps are done level by level, it is <strong>guaranteed</strong> that every node in the given tree has either <strong>0 or 2 children</strong>.</p>
 
<p><strong>Example 1:</strong></p>
<img style="width: 800px; height: 161px;" src="https://assets.leetcode.com/uploads/2020/08/29/updown.jpg" alt="">

```haskell
Input: root = [1,2,3,4,5]
Output: [4,5,2,null,null,3,1]
```


<p><strong>Example 2:</strong></p>

```haskell
Input: root = []
Output: []
```

<p><strong>Example 3:</strong></p>

```haskell
Input: root = [1]
Output: [1] 
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li>The number of nodes in the tree will be in the range <code>[0, 10]</code>.</li>
	<li><code>1 &lt;= Node.val &lt;= 10</code></li>
	<li><code>Every node has either 0 or 2 children.</code></li>
</ul>

