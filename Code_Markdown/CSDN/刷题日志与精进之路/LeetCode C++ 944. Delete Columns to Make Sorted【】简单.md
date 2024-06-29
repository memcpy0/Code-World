<p>We are given an array&nbsp;<code>A</code> of <code>N</code> lowercase letter strings, all of the same length.</p>

<p>Now, we may choose any set of deletion indices, and for each string, we delete all the characters in those indices.</p>

<p>For example, if we have an array <code>A = ["abcdef","uvwxyz"]</code> and deletion indices <code>{0, 2, 3}</code>, then the final array after deletions is <code>["bef", "vyz"]</code>,&nbsp;and the remaining columns of <code>A</code> are&nbsp;<code>["b","v"]</code>, <code>["e","y"]</code>, and <code>["f","z"]</code>.&nbsp; (Formally, the <code>c</code>-th column is <code>[A[0][c], A[1][c], ..., A[A.length-1][c]]</code>).</p>

<p>Suppose we chose a set of deletion indices <code>D</code> such that after deletions, each remaining column in A is in <strong>non-decreasing</strong> sorted order.</p>

<p>Return the minimum possible value of <code>D.length</code>.</p>

 
<p><strong>Example 1:</strong></p>

```haskell
Input: A = ["cba","daf","ghi"]
Output: 1
Explanation: 
After choosing D = {1}, each column ["c","d","g"] and ["a","f","i"] are in non-decreasing sorted order.
If we chose D = {}, then a column ["b","a","h"] would not be in non-decreasing sorted order.
```
 
<p><strong>Example 2:</strong></p>

```haskell
Input: A = ["a","b"]
Output: 0
Explanation: D = {} 
```

<p><strong>Example 3:</strong></p>

```haskell
Input: A = ["zyx","wvu","tsr"]
Output: 3
Explanation: D = {0, 1, 2}
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= A.length &lt;= 100</code></li>
	<li><code>1 &lt;= A[i].length &lt;= 1000</code></li>
</ul>
 
题意：给定由 `N` 个小写字母字符串组成的数组 `A` ，其中每个字符串长度相等。需要选出一组要删掉的列 `D`，对 `A` 执行删除操作，使 `A` 中剩余的每一列都是**非降序**排列的，然后返回 `D.length` 的最小可能值。

**删除**操作的定义是：选出一组要删掉的列，删去 `A` 中对应列中的所有字符，形式上的第 `n` 列为 `[A[0][n], A[1][n], ..., A[A.length-1][n]]` 。
