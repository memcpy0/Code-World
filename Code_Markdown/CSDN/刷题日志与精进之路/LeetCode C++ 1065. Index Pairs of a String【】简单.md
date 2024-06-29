<p>Given a <code>text</code>&nbsp;string and <code>words</code> (a list of strings), return all index pairs <code>[i, j]</code> so that the substring <code>text[i]...text[j]</code>&nbsp;is in the list of <code>words</code>.</p>
 
<p><strong>Example 1:</strong></p>

```haskell
Input: text = "thestoryofleetcodeandme", words = ["story","fleet","leetcode"]
Output: [[3,7],[9,13],[10,17]]
```
<p><strong>Example 2:</strong></p>

```haskell
Input: text = "ababa", words = ["aba","ab"]
Output: [[0,1],[0,2],[2,3],[2,4]]
Explanation: 
Notice that matches can overlap, see "aba" is found in [0,2] and [2,4].
```

 
 

<p><strong>Note:</strong></p>

<ol>
	<li>All strings contains only lowercase English letters.</li>
	<li>It's guaranteed that all strings in <code>words</code> are different.</li>
	<li><code>1 &lt;= text.length &lt;= 100</code></li>
	<li><code>1 &lt;= words.length &lt;= 20</code></li>
	<li><code>1 &lt;= words[i].length &lt;= 50</code></li>
	<li>Return the pairs <code>[i,j]</code> in sorted order (i.e. sort them by their first coordinate in case of ties sort them by their second coordinate).</li>
</ol>

题意：给出字符串 `text` 和字符串列表 `words`, 返回所有的索引对 `[i, j]` 使得在索引对范围内的子字符串 `text[i]...text[j]`（包括 `i` 和 `j`）属于字符串列表 `words` 。

 

