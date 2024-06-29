Given a string <b>s</b> and a list of strings <b>dict</b>, you need to add a closed pair of bold tag <code>&lt;b&gt;</code> and <code>&lt;/b&gt;</code> to wrap the substrings in s that exist in dict. If two such substrings overlap, you need to wrap them together by only one pair of closed bold tag. Also, if two substrings wrapped by bold tags are consecutive, you need to combine them.

<p><b>Example 1:</b></p>

```css
Input: 
s = "abcxyz123"
dict = ["abc","123"]
Output:
"<b>abc</b>xyz<b>123</b>"
```
<p><b>Example 2:</b></p>

```css
Input: 
s = "aaabbcc"
dict = ["aaa","aab","bc"]
Output:
"<b>aaabbc</b>c"
```
<p><b>Constraints:</b></p>
<ul>
	<li>The given dict won't contain duplicates, and its length won't exceed 100.</li>
	<li>All the strings in input have length in range [1, 1000].</li>
</ul>
<p><strong>Note:</strong> This question is the same as 758:&nbsp;<a href="https://leetcode.com/problems/bold-words-in-string/">https://leetcode.com/problems/bold-words-in-string/</a></p>

题意：给一个字符串 `s` 和一个字符串列表 `dict` ，你需要将在字符串列表中出现过的 `s` 的子串添加加粗闭合标签 `<b>` 和 `</b>` 。如果两个子串有重叠部分，你需要把它们一起用一个闭合标签包围起来。同理，如果两个子字符串连续被加粗，那么你也需要把它们合起来用一个加粗标签包围。
 
