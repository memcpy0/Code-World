<p>You are given two strings <code>word1</code> and <code>word2</code>. Merge the strings by adding letters in alternating order, starting with <code>word1</code>. If a string is longer than the other, append the additional letters onto the end of the merged string.</p>

<p>Return <em>the merged string.</em></p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: word1 = "abc", word2 = "pqr"
Output: "apbqcr"
Explanation: The merged string will be merged as so:
word1:  a   b   c
word2:    p   q   r
merged: a p b q c r  
```

<p><strong>Example 2:</strong></p>

```clike
Input: word1 = "ab", word2 = "pqrs"
Output: "apbqrs"
Explanation: Notice that as word2 is longer, "rs" is appended to the end.
word1:  a   b 
word2:    p   q   r   s
merged: a p b q   r   s 
```

<p><strong>Example 3:</strong></p>

```clike
Input: word1 = "ab", word2 = "pqrs"
Output: "apbqrs"
Explanation: Notice that as word2 is longer, "rs" is appended to the end.
word1:  a   b 
word2:    p   q   r   s
merged: a p b q   r   s 
```
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= word1.length, word2.length &lt;= 100</code></li>
	<li><code>word1</code> and <code>word2</code> consist of lowercase English letters.</li>
</ul>  

题意：给你两个字符串 `word1` 和 `word2` 。请从 `word1` 开始，通过交替添加字母来合并字符串。如果一个字符串比另一个字符串长，就将多出来的字母追加到合并后字符串的末尾。返回 **合并后的字符串** 。

---
### 解法 简单遍历
```cpp
class Solution {
public:
    string mergeAlternately(string word1, string word2) {
        string ans;
        int n = word1.size(), m = word2.size();
        for (int i = 0; i < n || i < m; ++i) {
            if (i < n) ans += word1[i];
            if (i < m) ans += word2[i];
        } 
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了46.12% 的用户
内存消耗：6.3 MB, 在所有 C++ 提交中击败了19.72% 的用户
```
