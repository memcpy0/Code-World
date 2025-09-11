<p>A string is a <strong>valid parentheses string</strong> (denoted <strong>VPS</strong>) if it meets one of the following:</p>
<ul>
	<li>It is an empty string <code>""</code>, or a single character not equal to <code>"("</code> or <code>")"</code>,</li>
	<li>It can be written as <code>AB</code> (<code>A</code> concatenated with <code>B</code>), where <code>A</code> and <code>B</code> are <strong>VPS</strong>'s, or</li>
	<li>It can be written as <code>(A)</code>, where <code>A</code> is a <strong>VPS</strong>.</li>
</ul>

<p>We can similarly define the <strong>nesting depth</strong> <code>depth(S)</code> of any VPS <code>S</code> as follows:</p>

<ul>
	<li><code>depth("") = 0</code></li>
	<li><code>depth(A + B) = max(depth(A), depth(B))</code>, where <code>A</code> and <code>B</code> are <strong>VPS</strong>'s</li>
	<li><code>depth("(" + A + ")") = 1 + depth(A)</code>, where <code>A</code> is a <strong>VPS</strong>.</li>
</ul>

<p>For example, <code>""</code>, <code>"()()"</code>, and <code>"()(()())"</code> are <strong>VPS</strong>'s (with nesting depths 0, 1, and 2), and <code>")("</code> and <code>"(()"</code> are not <strong>VPS</strong>'s.</p>

<p>Given a <strong>VPS</strong> represented as string <code>s</code>, return <em>the <strong>nesting depth</strong> of </em><code>s</code>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp 
Input:s = "(1+(2*3)+((<u>8</u>)/4))+1"
Output: 3
Explanation: Digit 8 is inside of 3 nested parentheses in the string.
```

<p><strong>Example 2:</strong></p>

```cpp 
Input:s = "(1)+((2))+(((<u>3</u>)))"
Output: 3
```

<p><strong>Example 3:</strong></p>

```cpp 
Input:s = "1+(<u>2</u>*3)/(2-1)"
Output: 1
```

<p><strong>Example 4:</strong></p>

```cpp 
Input:s = "<u>1</u>"
Output: 0
```
<p><strong>Constraints:</strong></p>
<ul>
	<li><code>1 &lt;= s.length &lt;= 100</code></li>
	<li><code>s</code> consists of digits <code>0-9</code> and characters <code>'+'</code>, <code>'-'</code>, <code>'*'</code>, <code>'/'</code>, <code>'('</code>, and <code>')'</code>.</li>
	<li>It is guaranteed that parentheses expression <code>s</code> is a <strong>VPS</strong>.</li>
</ul>


题意： 如果字符串满足以下条件之一，则可以称之为 <strong>有效括号字符串</strong><strong>（valid parentheses string</strong>，可以简写为 <strong>VPS</strong>）： 

<ul>
	<li>字符串是一个空字符串 <code>""</code>，或者是一个不为 <code>"("</code> 或 <code>")"</code> 的单字符。</li>
	<li>字符串可以写为 <code>AB</code>（<code>A</code> 与 <code>B</code>&nbsp;字符串连接），其中 <code>A</code> 和 <code>B</code> 都是 <strong>有效括号字符串</strong> 。</li>
	<li>字符串可以写为 <code>(A)</code>，其中 <code>A</code> 是一个 <strong>有效括号字符串</strong> 。</li>
</ul>

类似地，可以定义任何有效括号字符串&nbsp;<code>S</code> 的 <strong>嵌套深度</strong> <code>depth(S)</code>：
<ul>
	<li><code>depth("") = 0</code></li>
	<li><code>depth(A + B) = max(depth(A), depth(B))</code>，其中 <code>A</code> 和 <code>B</code> 都是 <strong>有效括号字符串</strong></li>
	<li><code>depth("(" + A + ")") = 1 + depth(A)</code>，其中 <code>A</code> 是一个 <strong>有效括号字符串</strong></li>
</ul>

 例如：<code>""</code>、<code>"()()"</code>、<code>"()(()())"</code> 都是 <strong>有效括号字符串</strong>（嵌套深度分别为 0、1、2），而 <code>")("</code> 、<code>"(()"</code> 都不是 <strong>有效括号字符串</strong> 。给出一个 <strong>有效括号字符串</strong> <code>s</code>，返回该字符串的<em> </em><code>s</code> <strong>嵌套深度</strong> 。 

---
### 解法一
这个题目虽然比较长，但是很简单，因为保证了是有效字符串。我们只需知道**VPS中任一字符的深度等于它及之前的左括号个数-右括号个数**这一事实即可：
```cpp
class Solution {
public:
    int maxDepth(string s) {
        if (s.size() <= 1) return 0;
        stack<char> st;
        int maxLevel = 0;
        for (const char &c : s) {
            if (c == '(') st.push(c);
            else if (c == ')') st.pop();
            int t = st.size();
            maxLevel = max(maxLevel, t);
        }
        return maxLevel;
    }
};
```
或者简写如下，遇到左括号就加一，遇到右括号就减一，得到当前字符的深度，然后取最大深度作为VPS的嵌套深度：
```cpp
class Solution {
public:
    int maxDepth(string s) {
        int maxDepth = 0, curDepth = 0;
        for (const char &c : s) {
            if (c == '(') ++curDepth;
            else if (c == ')') --curDepth;
            maxDepth = max(maxDepth, curDepth);
        }
        return maxDepth;
    }
};
```
提交后效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了23.99% 的用户
```
总的来说，题目对有效字符串的递归定义蛮有趣的。
