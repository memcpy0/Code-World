<p>You are given a <strong>0-indexed</strong> string <code>s</code> that has lowercase English letters in its <strong>even</strong> indices and digits in its <strong>odd</strong> indices.</p>

<p>There is a function <code>shift(c, x)</code>, where <code>c</code> is a character and <code>x</code> is a digit, that returns the <code>x<sup>th</sup></code> character after <code>c</code>.</p>

<ul>
	<li>For example, <code>shift('a', 5) = 'f'</code> and <code>shift('x', 0) = 'x'</code>.</li>
</ul>

<p>For every <strong>odd</strong>&nbsp;index <code>i</code>, you want to replace the digit <code>s[i]</code> with <code>shift(s[i-1], s[i])</code>.</p>

<p>Return <code>s</code><em> after replacing all digits. It is <strong>guaranteed</strong> that </em><code>shift(s[i-1], s[i])</code><em> will never exceed </em><code>'z'</code>.</p>

 
<p><strong>Example 1:</strong></p>

```clike
Input: s = "a1c1e1"
Output: "abcdef"
Explanation: The digits are replaced as follows:
- s[1] -> shift('a',1) = 'b'
- s[3] -> shift('c',1) = 'd'
- s[5] -> shift('e',1) = 'f'
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: s = "a1b2c3d4e"
Output: "abbdcfdhe"
Explanation: The digits are replaced as follows:
- s[1] -> shift('a',1) = 'b'
- s[3] -> shift('b',2) = 'd'
- s[5] -> shift('c',3) = 'f'
- s[7] -> shift('d',4) = 'h'
```

 
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 100</code></li>
	<li><code>s</code> consists only of lowercase English letters and digits.</li>
	<li><code>shift(s[i-1], s[i]) &lt;= 'z'</code> for all <strong>odd</strong> indices <code>i</code>.</li>
</ul>

题意：给出一个下标从 <strong>0</strong>&nbsp;开始的字符串 <code>s</code>&nbsp;，它的 <strong>偶数</strong> 下标处为小写英文字母，<strong>奇数</strong>&nbsp;下标处为数字。

<p>定义一个函数&nbsp;<code>shift(c, x)</code>&nbsp;，其中&nbsp;<code>c</code>&nbsp;是一个字符且&nbsp;<code>x</code>&nbsp;是一个数字，函数返回字母表中&nbsp;<code>c</code>&nbsp;后面第 <code>x</code>&nbsp;个字符。对于每个 <strong>奇数</strong>&nbsp;下标&nbsp;<code>i</code>&nbsp;，你需要将数字&nbsp;<code>s[i]</code> 用&nbsp;<code>shift(s[i-1], s[i])</code>&nbsp;替换。替换所有数字以后，将字符串 <code>s</code>&nbsp;返回。题目 <strong>保证</strong><em>&nbsp;</em><code>shift(s[i-1], s[i])</code>&nbsp;不会超过 <code>'z'</code>&nbsp;。</p>
 
---
### 解法 直接遍历
遍历修改字符串即可：
```cpp
class Solution {
public:
    string replaceDigits(string s) {
        int n = s.size();
        for (int i = 0; i < n; ++i) 
            if (i & 1) s[i] = s[i - 1] + (s[i] - '0');
        return s;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了6.90% 的用户
```
