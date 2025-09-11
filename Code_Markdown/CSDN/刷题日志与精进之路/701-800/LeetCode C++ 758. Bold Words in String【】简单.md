<p>Given a set of keywords <code>words</code> and a string <code>S</code>, make all appearances of all keywords in <code>S</code> bold. Any letters between <code>&lt;b&gt;</code> and <code>&lt;/b&gt;</code> tags become bold.</p>

<p>The returned string should use the least number of tags possible, and of course the tags should form a valid combination.</p>

<p>For example, given that <code>words = ["ab", "bc"]</code> and <code>S = "aabcd"</code>, we should return <code>"a&lt;b&gt;abc&lt;/b&gt;d"</code>. Note that returning <code>"a&lt;b&gt;a&lt;b&gt;b&lt;/b&gt;c&lt;/b&gt;d"</code> would use more tags, so it is incorrect.</p>

<p><b>Constraints:</b></p> 
<ul>
	<li><code>words</code> has length in range <code>[0, 50]</code>.</li>
	<li><code>words[i]</code> has length in range <code>[1, 10]</code>.</li>
	<li><code>S</code> has length in range <code>[0, 500]</code>.</li>
	<li>All characters in <code>words[i]</code> and <code>S</code> are lowercase letters.</li>
</ul>

<p><strong>Note:</strong> This question is the same as 616:&nbsp;<a href="https://leetcode.com/problems/add-bold-tag-in-string/">https://leetcode.com/problems/add-bold-tag-in-string/</a></p> 

题意：给定一个关键词集合 `words` 和一个字符串 `S`，将所有 `S` 中出现的关键词加粗。所有在标签 `<b>` 和 `</b>` 中的字母都会加粗。

返回的字符串需要使用尽可能少的标签，当然标签应形成有效的组合。例如，给定 `words = ["ab", "bc"]` 和 `S = "aabcd"`，需要返回 "a<b>abc</b>d"。注意返回 "a<b>a<b>b</b>c</b>d" 会使用更多的标签，因此是错误的。 
