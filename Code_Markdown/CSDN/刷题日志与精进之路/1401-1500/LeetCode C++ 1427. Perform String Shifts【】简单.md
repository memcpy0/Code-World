<p>You are given a string <code>s</code>&nbsp;containing lowercase English letters, and a matrix&nbsp;<code>shift</code>, where&nbsp;<code>shift[i] = [direction, amount]</code>:</p>

<ul>
	<li><code>direction</code>&nbsp;can be <code>0</code>&nbsp;(for left shift) or <code>1</code>&nbsp;(for right shift).&nbsp;</li>
	<li><code>amount</code>&nbsp;is the amount by which string&nbsp;<code>s</code>&nbsp;is to be shifted.</li>
	<li>A left shift by 1 means remove the first character of <code>s</code> and append it to the end.</li>
	<li>Similarly, a right shift by 1 means remove the last character of <code>s</code> and add it to the beginning.</li>
</ul>

<p>Return the final string after all operations.</p>

 
<p><strong>Example 1:</strong></p>

```haskell
Input: s = "abc", shift = [[0,1],[1,2]]
Output: "cab"
Explanation: 
[0,1] means shift to left by 1. "abc" -> "bca"
[1,2] means shift to right by 2. "bca" -> "cab"
```

 
<p><strong>Example 2:</strong></p>

```haskell
Input: s = "abcdefg", shift = [[1,1],[1,1],[0,2],[1,3]]
Output: "efgabcd"
Explanation:  
[1,1] means shift to right by 1. "abcdefg" -> "gabcdef"
[1,1] means shift to right by 1. "gabcdef" -> "fgabcde"
[0,2] means shift to left by 2. "fgabcde" -> "abcdefg"
[1,3] means shift to right by 3. "abcdefg" -> "efgabcd"
```

 
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 100</code></li>
	<li><code>s</code> only contains lower case English letters.</li>
	<li><code>1 &lt;= shift.length &lt;= 100</code></li>
	<li><code>shift[i].length == 2</code></li>
	<li><code>0 &lt;= shift[i][0] &lt;= 1</code></li>
	<li><code>0 &lt;= shift[i][1] &lt;= 100</code></li>
</ul>

题意：给定一个包含小写英文字母的字符串 `s` 以及一个矩阵 `shift`，其中 `shift[i] = [direction, amount]` ，`direction` 可以为 `0` （表示左移）或 `1` （表示右移），`amount` 表示 `s` 左右移的位数。对这个字符串进行所有操作后，返回最终结果。
