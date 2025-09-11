<p>We can represent a&nbsp;sentence as an array of words, for example, the&nbsp;sentence <code>"I am happy with leetcode"</code> can be represented as <code>arr = ["I","am",happy","with","leetcode"]</code>.</p>

<p>Given two&nbsp;sentences&nbsp;<code>sentence1</code> and&nbsp;<code>sentence2</code> each represented as a string array and given an array of string pairs <code>similarPairs</code> where&nbsp;<code>similarPairs[i] = [x<sub>i</sub>, y<sub>i</sub>]</code>&nbsp;indicates that the two words&nbsp;<code>x<sub>i</sub></code> and&nbsp;<code>y<sub>i</sub></code> are similar.</p>

<p>Return <em><code>true</code> if&nbsp;<code>sentence1</code>&nbsp;and&nbsp;<code>sentence2</code>&nbsp;are similar, or <code>false</code> if they are not similar</em>.</p>

Two sentences are similar if:
<ul>
	<li>They have <strong>the same length</strong> (i.e. the same number of words)</li>
	<li><code>sentence1[i]</code> and&nbsp;<code>sentence2[i]</code>&nbsp;are similar.</li>
</ul>

<p>Notice that a word is always similar to itself, also notice that the similarity relation is not transitive. For example, if the words&nbsp;<code><font face="monospace">a</font></code>&nbsp;and <code>b</code> are similar and the words <code>b</code>&nbsp;and <code>c</code> are similar, <code>a</code> and <code>c</code> are&nbsp;<strong>not&nbsp;necessarily similar</strong>.</p>

<p><strong>Example 1:</strong></p>

```css
Input: sentence1 = ["great","acting","skills"], sentence2 = ["fine","drama","talent"], similarPairs = [["great","fine"],["drama","acting"],["skills","talent"]]
Output: true
Explanation: The two sentences have the same length and each word i of sentence1 is also similar to the corresponding word in sentence2.
```

 
<p><strong>Example 2:</strong></p>

```css
Input: sentence1 = ["great"], sentence2 = ["great"], similarPairs = []
Output: true
Explanation: A word is similar to itself.
```

 
<p><strong>Example 3:</strong></p>

```css
Input: sentence1 = ["great"], sentence2 = ["doubleplus","good"], similarPairs = [["great","doubleplus"]]
Output: false
Explanation: As they don't have the same length, we return false.
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;=&nbsp;sentence1.length,&nbsp;sentence2.length &lt;= 1000</code></li>
	<li><code>1 &lt;=&nbsp;sentence1[i].length,&nbsp;sentence2[i].length &lt;= 20</code></li>
	<li><code>sentence1[i]</code> and&nbsp;<code>sentence2[i]</code>&nbsp;consist of lower-case and upper-case English letters.</li>
	<li><code>0 &lt;=&nbsp;similarPairs.length &lt;= 1000</code></li>
	<li><code>similarPairs[i].length == 2</code></li>
	<li><code>1 &lt;=&nbsp;x<sub>i</sub>.length,&nbsp;y<sub>i</sub>.length&nbsp;&lt;= 20</code></li>
	<li><code>x<sub>i</sub></code> and <code>y<sub>i</sub></code>&nbsp;consist of lower-case and upper-case English letters.</li>
	<li>All the pairs <code>(x<sub>i</sub>,<sub>&nbsp;</sub>y<sub>i</sub>)</code> are <strong>distinct</strong>.</li>
</ul>



题意：给定两个句子 <code>words1, words2</code> （每个用字符串数组表示），和一个相似单词对的列表&nbsp;<code>pairs</code>&nbsp;，判断是否两个句子是相似的。

<p>例如，当相似单词对是 <code>pairs = [["great", "fine"], ["acting","drama"], ["skills","talent"]]</code>的时候，"great acting skills" 和 "fine drama talent" 是相似的。</p>

<p>注意相似关系是不具有传递性的。例如，如果 "great" 和&nbsp;"fine" 是相似的，"fine" 和&nbsp;"good" 是相似的，但是&nbsp;"great" 和 "good" 未必是相似的。</p>

<p>但是，相似关系是具有对称性的。例如，"great" 和 "fine" 是相似的相当于&nbsp;"fine" 和&nbsp;"great" 是相似的。</p>

<p>而且，一个单词总是与其自身相似。例如，句子 <code>words1 = ["great"], words2 = ["great"], pairs = []</code> 是相似的，尽管没有输入特定的相似单词对。</p>

<p>最后，句子只会在具有相同单词个数的前提下才会相似。所以一个句子 <code>words1 = ["great"]</code> 永远不可能和句子 <code>words2 = ["doubleplus","good"]</code> 相似。</p>
 
