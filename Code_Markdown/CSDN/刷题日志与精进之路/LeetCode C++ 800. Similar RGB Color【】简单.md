<p>In the following, every capital letter represents some hexadecimal digit from <code>0</code> to <code>f</code>.</p>

<p>The red-green-blue color <code>"#AABBCC"</code>&nbsp;can be written&nbsp;as&nbsp;<code>"#ABC"</code> in&nbsp;shorthand.&nbsp; For example, <code>"#15c"</code> is shorthand for the color <code>"#1155cc"</code>.</p>

<p>Now, say the similarity between two colors <code>"#ABCDEF"</code> and <code>"#UVWXYZ"</code> is <code>-(AB - UV)^2 -&nbsp;(CD - WX)^2 -&nbsp;(EF - YZ)^2</code>.</p>

<p>Given the color <code>"#ABCDEF"</code>, return a 7 character color&nbsp;that is most similar to <code>#ABCDEF</code>, and has a shorthand (that is, it can be represented as some <code>"#XYZ"</code></p>

<strong>Example 1:</strong> 
```css
Input: color = "#09f166"
Output: "#11ee66"
Explanation:  
The similarity is -(0x09 - 0x11)^2 -(0xf1 - 0xee)^2 - (0x66 - 0x66)^2 = -64 -9 -0 = -73.
This is the highest among any shorthand color. 
```
  
<p><strong>Note:</strong></p>

<ul>
	<li><code>color</code> is a string of length <code>7</code>.</li>
	<li><code>color</code> is a valid RGB color: for <code>i &gt; 0</code>, <code>color[i]</code> is a hexadecimal digit from <code>0</code> to <code>f</code></li>
	<li>Any answer which has the same (highest)&nbsp;similarity as the best answer will be accepted.</li>
	<li>All inputs and outputs should use lowercase letters, and the output is 7 characters.</li>
</ul>



题意：`RGB` 颜色用十六进制来表示的话，每个大写字母都代表了某个从 `0` 到 `f` 的 `16` 进制数。

`RGB` 颜色 `"#AABBCC"` 可以简写成 `"#ABC"` 。例如，`"#15c"` 其实是 `"#1155cc"` 的简写。

现在，假如我们分别定义两个颜色 `"#ABCDEF"` 和 `"#UVWXYZ"`，则他们的相似度可以通过这个表达式 `-(AB - UV)^2 - (CD - WX)^2 - (EF - YZ)^2` 来计算。

那么给定颜色 `"#ABCDEF"`，返回一个与 `"#ABCDEF"` 最相似的 `7` 个字符代表的颜色，并且它是可以被简写形式表达的。（比如，可以表示成类似 `"#XYZ"` 的形式）
