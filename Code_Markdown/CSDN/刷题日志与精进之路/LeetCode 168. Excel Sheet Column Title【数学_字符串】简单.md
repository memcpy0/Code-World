<p>Given an integer <code>columnNumber</code>, return <em>its corresponding column title as it appears in an Excel sheet</em>.</p>

<p>For example:</p>

<pre>A -&gt; 1
B -&gt; 2
C -&gt; 3
...
Z -&gt; 26
AA -&gt; 27
AB -&gt; 28 
...
</pre>
 
<p><strong>Example 1:</strong></p>

```bash
Input: columnNumber = 1
Output: "A"
```
 
<p><strong>Example 2:</strong></p>

```bash
Input: columnNumber = 28
Output: "AB" 
```

<p><strong>Example 3:</strong></p>

```bash
Input: columnNumber = 701
Output: "ZY"
```

<p><strong>Example 4:</strong></p>

```bash
Input: columnNumber = 2147483647
Output: "FXSHRXW"
```


 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= columnNumber &lt;= 2<sup>31</sup> - 1</code></li>
</ul>


题意：给你一个整数 `columnNumber` ，返回它在 Excel 表中相对应的列名称。

---
### 解法 进制转换
有些类似于十进制到二十六进制的转换，不过也有不同，比如 `A -> Z` 对应的是 `1 -> 26` ，而 `27` 对应的是 `AA` 。假设给出的整数是 $n$ ，对应的列名称为 $a_{k} a_{k-1}\dots a_1a_0,\ a_i \in \text{[A, Z]}$ ，字符 $a_i$ 对应到数字 $(a_i - A + 1) \in [1, 26]$ ，于是转换规律如下：
$$(a_k - A + 1) \times 26^{k-1} + \dots + (a_1 - A + 1) \times 26 + (a_0 - A + 1) = n$$ 

因此等式左右两边同时减去 $1$ ，结果为：
$$(a_k - A + 1) \times 26^{k-1} + \dots + (a_1 - A + 1) \times 26 + (a_0 - A) = n - 1$$
 
此时 $(a_0 - A) \in [0, 25]$ ，通过对 $n - 1$ 模 $26$ 即可得到 $(a_0 - A)$ ，即有 $a_0 = (n - 1)\ \% \ 26 + A$ 。接着令 $n - 1$ 除以 $26$ 得到下式，继续重复前面的步骤直到求出 $a_k$ ：
$$(a_k - A + 1) \times 26^{k-2} + \dots + (a_1 - A + 1) = (n - 1) \times 26^{-1}$$

对应的代码如下：
```cpp
class Solution {
public:
    string convertToTitle(int columnNumber) {
        string ans;
        do { 
            ans.push_back('A' + (--columnNumber % 26));
            columnNumber /= 26;
        } while (columnNumber);
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了82.75% 的用户
```
