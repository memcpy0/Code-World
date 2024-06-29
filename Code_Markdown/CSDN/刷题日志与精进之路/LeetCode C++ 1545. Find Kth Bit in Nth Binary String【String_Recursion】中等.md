
<p>Given two positive integers&nbsp;<code>n</code>&nbsp;and <code>k</code>,&nbsp;the binary string&nbsp;&nbsp;<code>S<sub>n</sub></code>&nbsp;is formed as follows:</p>

<ul>
	<li><code>S<sub>1</sub>&nbsp;= "0"</code></li>
	<li><code>S<sub><span style="font-size: 10.8333px;">i</span></sub>&nbsp;=&nbsp;S<sub><span style="font-size: 10.8333px;">i-1</span></sub>&nbsp;+ "1" + reverse(invert(S<sub><span style="font-size: 10.8333px;">i-1</span></sub>))</code>&nbsp;for&nbsp;<code>i &gt; 1</code></li>
</ul>

<p>Where&nbsp;<code>+</code>&nbsp;denotes the concatenation operation,&nbsp;<code>reverse(x)</code>&nbsp;returns the reversed string <font face="monospace">x,</font>&nbsp;and&nbsp;<code>invert(x)</code>&nbsp;inverts all the bits in <font face="monospace">x</font> (0 changes to 1 and 1 changes to 0).</p>

<p>For example, the first 4 strings in the above sequence are:</p>

<ul>
	<li><code>S<sub>1&nbsp;</sub>= "0"</code></li>
	<li><code>S<sub>2&nbsp;</sub>= "0<strong>1</strong>1"</code></li>
	<li><code>S<sub>3&nbsp;</sub>= "011<strong>1</strong>001"</code></li>
	<li><code>S<sub>4</sub> = "0111001<strong>1</strong>0110001"</code></li>
</ul>

<p>Return <em>the</em> <code>k<sup>th</sup></code> <em>bit</em> <em>in</em>&nbsp;<code>S<sub>n</sub></code>. It is guaranteed that&nbsp;<code>k</code>&nbsp;is valid for the given&nbsp;<code>n</code>.</p>
 
<p><strong>Example 1:</strong></p>

```go
Input: n = 3, k = 1
Output: "0"
Explanation: S3 is "0111001". The first bit is "0". 
```
<p><strong>Example 2:</strong></p>

```go
Input: n = 4, k = 11
Output: "1"
Explanation: S4 is "011100110110001". The 11th bit is "1".
```
 
<p><strong>Example 3:</strong></p> 

```go
Input: n = 1, k = 1
Output: "0" 
```

<p><strong>Example 4:</strong></p>

```go
Input: n = 2, k = 3
Output: "1"
```

 <p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= n &lt;= 20</code></li>
	<li><code>1 &lt;= k &lt;= 2<sup>n</sup> - 1</code></li>
</ul>

题意：给你两个正整数 <code>n</code> 和 <code>k</code>，二进制字符串&nbsp; <code>S<sub>n</sub></code> 的形成规则如下：
<ul>
	<li><code>S<sub>1</sub>&nbsp;= "0"</code></li>
	<li>当 <code>i &gt; 1</code> 时，<code>S<sub>i</sub>&nbsp;=&nbsp;S<sub>i-1</sub>&nbsp;+ "1" + reverse(invert(S<sub>i-1</sub>))</code></li>
</ul>

<p>其中 <code>+</code> 表示串联操作，<code>reverse(x)</code> 返回反转 <code>x</code> 后得到的字符串，而 <code>invert(x)</code> 则会翻转 x 中的每一位（0 变为 1，而 1 变为 0）。符合上述描述的序列的前 4 个字符串依次是：</p>

<ul>
	<li><code>S<sub>1&nbsp;</sub>= "0"</code></li>
	<li><code>S<sub>2&nbsp;</sub>= "0<strong>1</strong>1"</code></li>
	<li><code>S<sub>3&nbsp;</sub>= "011<strong>1</strong>001"</code></li>
	<li><code>S<sub>4</sub> = "0111001<strong>1</strong>0110001"</code></li>
</ul>

<p>请你返回&nbsp; <code>S<sub>n</sub></code> 的 <strong>第 <code>k</code> 位字符</strong> ，题目数据保证 <code>k</code> 一定在 <code>S<sub>n</sub></code> 长度范围以内。</p>

---
### 解法1 模拟
由于数据量不大，因此可以直接模拟。不过也要稍微优化一下空间，比如提前退出，不创建临时字符串来 `invert` 和 `reverse` 。
```cpp
class Solution {
public:
    char findKthBit(int n, int k) {
        if (k == 1) return '0';
        string s = "0";
        for (int i = 2; i <= n; ++i) {
            int len = s.size();
            if (len >= k) return s[k - 1]; //提前退出
            s.push_back('1');
            for (int j = 0; j < len; ++j) s.push_back(((s[len - j - 1] - '0') ^ 1) + '0');
        }
        return s[k - 1];
    }
};
```
运行效率如下：
```cpp
执行用时：28 ms, 在所有 C++ 提交中击败了33.95% 的用户
内存消耗：28.3 MB, 在所有 C++ 提交中击败了33.95% 的用户
```
---
### 解法2 递归+找规律
找规律，从第二层开始，第 `i` 层中间的 `1` 的位置在 `2^(i-1)` 处，找右边的去同层左边找然后取反，找左边的去上层同位置找：
```cpp
class Solution {
public:
    char findKthBit(int n, int k) {
        if (k == 1) return '0';
        if (!(k & (k - 1))) return '1'; //k是2的幂,即连接si和reverse(invert(si))中间的'1'
        if (k < pow(2, n - 1)) return findKthBit(n - 1, k); //在左边
        return findKthBit(n, pow(2, n) - k) == '0' ? '1' : '0';
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.7 MB, 在所有 C++ 提交中击败了83.33% 的用户
```
