<p>Given a string&nbsp;<code>text</code>, you want to use the characters of&nbsp;<code>text</code>&nbsp;to form as many instances of the word <strong>"balloon"</strong> as possible.</p>

<p>You can use each character in <code>text</code> <strong>at most once</strong>. Return the maximum number of instances that can be formed.</p>
 
<p><strong>Example 1:</strong></p>

<p><strong><img style="width: 132px; height: 35px;" src="https://assets.leetcode.com/uploads/2019/09/05/1536_ex1_upd.JPG" alt=""></strong></p>

```haskell
Input: text = "nlaebolko"
Output: 1 
```

<p><strong>Example 2:</strong></p>

<p><strong><img style="width: 267px; height: 35px;" src="https://assets.leetcode.com/uploads/2019/09/05/1536_ex2_upd.JPG" alt=""></strong></p>
 

```haskell
Input: text = "loonbalxballpoon"
Output: 2
```

<p><strong>Example 3:</strong></p>
 

```haskell
Input: text = "leetcode"
Output: 0 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= text.length &lt;= 10^4</code></li>
	<li><code>text</code>&nbsp;consists of lower case English letters only.</li>
</ul>

题意：给出一个字符串 `text`，需要使用 `text` 中的字母来拼凑尽可能多的单词 `"balloon"`（气球）。字符串 `text` 中的每个字母最多只能被使用一次。返回最多可以拼凑出多少个单词 `"balloon"`。

---

