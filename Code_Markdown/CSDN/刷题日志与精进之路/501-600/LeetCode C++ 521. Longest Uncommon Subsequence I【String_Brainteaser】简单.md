<p>Given two strings <code>a</code>&nbsp;and <code>b</code>, find the length of the&nbsp;<strong>longest uncommon subsequence</strong>&nbsp;between them.</p>

<p>A&nbsp;<b>subsequence</b>&nbsp;of&nbsp;a string&nbsp;<code>s</code>&nbsp;is a string that can be obtained after deleting any number of characters from <code>s</code>. For example, <code>"abc"</code>&nbsp;is a subsequence of <code>"aebdc"</code>&nbsp;because you can delete the underlined characters in&nbsp;<code>"a<u>e</u>b<u>d</u>c"</code>&nbsp;to get <code>"abc"</code>. Other subsequences of&nbsp;<code>"aebdc"</code>&nbsp;include&nbsp;<code>"aebdc"</code>,&nbsp;<code>"aeb"</code>,&nbsp;and&nbsp;<code>""</code>&nbsp;(empty string).</p>

<p>An&nbsp;<strong>uncommon subsequence</strong>&nbsp;between two strings&nbsp;is a string that is a <strong>subsequence of one&nbsp;but not the other</strong>.</p>

<p>Return <em>the length of the <strong>longest uncommon subsequence</strong>&nbsp;between <code>a</code>&nbsp;and <code>b</code></em>. If the longest uncommon subsequence doesn't exist, return <code>-1</code>.</p>

**Example 1:**

```swift
Input: a = "aba", b = "cdc"
Output: 3
Explanation: One longest uncommon subsequence is "aba" because "aba" is a subsequence of "aba" but not "cdc".
Note that "cdc" is also a longest uncommon subsequence.
```

**Example 2:**

```swift
Input: a = "aaa", b = "bbb"
Output: 3
Explanation: The longest uncommon subsequences are "aaa" and "bbb".
```

**Example 3:**

```swift
Input: a = "aaa", b = "aaa"
Output: -1
Explanation: Every subsequence of string a is also a subsequence of string b. Similarly, every subsequence of string b is also a subsequence of string a.
```
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= a.length, b.length &lt;= 100</code></li>
	<li><code>a</code> and <code>b</code> consist of lower-case English letters.</li>
</ul>


题意：给出两个字符串，从这两个字符串中找出**最长非公共子序列**。


---
### 解法 脑筋急转弯
虽然看起来很麻烦，可能会想到**最长公共子序列**。但其实很简单——如果两个字符串相等，就不存在非公共子序列，返回 `-1` ；否则返回两个字符串中较长字符串的长度：
```cpp
class Solution {
public:
    int findLUSlength(string a, string b) {
        return a == b ? -1 : max(a.size(), b.size());
    }
};
```
提交结果如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.5 MB, 在所有 C++ 提交中击败了5.04% 的用户
```
