<p>You are given a very large integer <code>n</code>, represented as a string,​​​​​​ and an integer digit <code>x</code>. The digits in <code>n</code> and the digit <code>x</code> are in the <strong>inclusive</strong> range <code>[1, 9]</code>, and <code>n</code> may represent a <b>negative</b> number.</p>

<p>You want to <strong>maximize </strong><code>n</code><strong>'s numerical value</strong> by inserting <code>x</code> anywhere in the decimal representation of <code>n</code>​​​​​​. You <strong>cannot</strong> insert <code>x</code> to the left of the negative sign.</p>

<ul>
	<li>For example, if <code>n = 73</code> and <code>x = 6</code>, it would be best to insert it between <code>7</code> and <code>3</code>, making <code>n = 763</code>.</li>
	<li>If <code>n = -55</code> and <code>x = 2</code>, it would be best to insert it before the first <code>5</code>, making <code>n = -255</code>.</li>
</ul>

<p>Return <em>a string representing the <strong>maximum</strong> value of </em><code>n</code><em>​​​​​​ after the insertion</em>.</p>

 
<p><strong>Example 1:</strong></p>

```cpp
Input: n = "99", x = 9
Output: "999"
Explanation: The result is the same regardless of where you insert 9. 
```

<p><strong>Example 2:</strong></p>

```cpp
Input: n = "-13", x = 2
Output: "-123"
Explanation: You can make n one of {-213, -123, -132}, and the largest of those three is -123.
```

 
 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n.length &lt;= 10<sup>5</sup></code></li>
	<li><code>1 &lt;= x &lt;= 9</code></li>
	<li>The digits in <code>n</code>​​​ are in the range <code>[1, 9]</code>.</li>
	<li><code>n</code> is a valid representation of an integer.</li>
	<li>In the case of a negative <code>n</code>,​​​​​​ it will begin with <code>'-'</code>.</li>
</ul>

题意：给你一个非常大的整数 <code>n</code> 和一个整数数字 <code>x</code> ，大整数 <code>n</code>&nbsp;用一个字符串表示。<code>n</code> 中每一位数字和数字 <code>x</code> 都处于闭区间 <code>[1, 9]</code> 中，且 <code>n</code> 可能表示一个 <strong>负数</strong> 。

<p>你打算通过在 <code>n</code> 的十进制表示的任意位置插入 <code>x</code> 来 <strong>最大化</strong> <code>n</code> 的 <strong>数值</strong> ​​​​​​。但 <strong>不能</strong> 在负号的左边插入 <code>x</code> 。返回插入操作后，用字符串表示的&nbsp;<code>n</code> 的最大值。</p>

---
### 解法 贪心
贪心的插入字符串 `x` 到 `n` 中。具体来说，当 `n` 是正数的时候，要使得结果最大，就必须从高位到低位（从左到右）遍历，遇到一个字符小于 `x` 时，将 `x` 插入到该位置；如果字符等于 `x` ，不确定后面有无小于 `x` 的字符（有的话应该插入那个位置，插入此处可能使得整个数变小），因此不插入；如果到末尾都没有这种字符，则插到字符串结尾。

当 `n` 是负数时，要使得结果最大，就必须从高位到低位（从左到右从 `1` 下标开始）遍历，遇到一个字符大于 `x` 时，将 `x` 插入到该位置；如果字符等于 `x` ，不确定后面有无大于 `x` 的字符（有的话应该插入那个位置，插入此处可能使得整个数变小），因此不插入；如果到末尾都没有这种字符，则插到字符串结尾。

不过上述做法都局限于 `x` 只有一位数的情况，当 `x` 也是一个超过一位数的整数时，代码应该这样写：
```cpp
class Solution {
public:
    string maxValue(string n, int x) {
        string sx = to_string(x);
        bool flag = n[0] == '-';
        int xl = sx.size(), i = flag;
        for (int nl = n.size(); i < nl; ++i) {
            string s = n.substr(i, xl);
            if ((flag && s > sx) || (!flag && s < sx))
                return n.substr(0, i) + sx + n.substr(i); //负数
        }
        return n + sx;
    }
};
```
运行效率如下：
```cpp
执行用时：84 ms, 在所有 C++ 提交中击败了26.33% 的用户
内存消耗：33.9 MB, 在所有 C++ 提交中击败了51.03% 的用户
```
由于 `x` 只有一位数，比较 `n[i]` 和 `x[0]` 即可：
```cpp
class Solution {
public:
    string maxValue(string n, int x) {
        bool flag = n[0] == '-';
        for (int i = flag, nl = n.size(); i < nl; ++i) 
            if ((flag && n[i] > x + '0') || (!flag && n[i] < x + '0'))
                return n.substr(0, i) + to_string(x) + n.substr(i); //负数
        return n + to_string(x);
    }
};
```
运行效率如下：
```cpp
执行用时：72 ms, 在所有 C++ 提交中击败了68.40% 的用户
内存消耗：33.4 MB, 在所有 C++ 提交中击败了60.88% 的用户
```
