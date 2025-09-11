<p>You are given two strings <code>s1</code> and <code>s2</code> of equal length. A <strong>string swap</strong> is an operation where you choose two indices in a string (not necessarily different) and swap the characters at these indices.</p>

<p>Return <code>true</code> <em>if it is possible to make both strings equal by performing <strong>at most one string swap </strong>on <strong>exactly one</strong> of the strings. </em>Otherwise, return <code>false</code>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: s1 = "bank", s2 = "kanb"
Output: true
Explanation: For example, swap the first character with the last character of s2 to make "bank". 
```

<p><strong>Example 2:</strong></p>

```clike
Input: s1 = "attack", s2 = "defend"
Output: false
Explanation: It is impossible to make them equal with one string swap.
```
 
<p><strong>Example 3:</strong></p>

```clike
Input: s1 = "kelb", s2 = "kelb"
Output: true
Explanation: The two strings are already equal, so no string swap operation is required.
```

 
<p><strong>Example 4:</strong></p>

```clike
Input: s1 = "abcd", s2 = "dcba"
Output: false
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s1.length, s2.length &lt;= 100</code></li>
	<li><code>s1.length == s2.length</code></li>
	<li><code>s1</code> and <code>s2</code> consist of only lowercase English letters.</li>
</ul>

题意：给你长度相等的两个字符串 <code>s1</code> 和 <code>s2</code> 。一次<strong> 字符串交换 </strong>操作的步骤如下：选出某个字符串中的两个下标（不必不同），并交换这两个下标所对应的字符。
<p>如果对 <strong>其中一个字符串</strong> 执行 <strong>最多一次字符串交换</strong> 就可以使两个字符串相等，返回 <code>true</code> ；否则，返回 <code>false</code> 。</p>

---
### 解法1 暴力
暴力交换 `s1` 中的任意两个字符，如果 `s1 == s2` 就返回 `true` 。没有结果就返回 `false` 。这种做法的效率是 $O(n^3)$ ：
```cpp
class Solution {
public:
    bool areAlmostEqual(string s1, string s2) {
        if (s1 == s2) return true;
        int n = s1.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                swap(s1[i], s1[j]);
                if (s1 == s2) return true;
                swap(s1[i], s1[j]);
            }
        }
        return false;
    }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了53.81% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了55.40% 的用户
```
---
### 解法2 哈希计数+串比较
通过哈希计算每个小写字母的出现次数，如果出现次数不一致则返回 `false` 。然后对比 `s1` 和 `s2` ，如果两个字符串不同之处为 `2` ，就可以通过交换让 `s1` 等于 `s2` 。这种做法的时间效率是 $O(n)$ ：
```cpp
class Solution {
public:
    bool areAlmostEqual(string s1, string s2) {
        if (s1 == s2) return true;
        int n = s1.size(), frac[26] = {0}, cnt = 0;
        for (int i = 0; i < n; ++i) {
            ++frac[s1[i] - 'a'];
            --frac[s2[i] - 'a'];
        } //如果字符的出现次数不一致,则返回false
        for (int i = 0; i < 26; ++i) if (frac[i] != 0) return false;
        for (int i = 0; i < n; ++i) if (s1[i] != s2[i]) ++cnt;
        return cnt == 2; //不同之处等于2,就可以通过交换让s1等于s2
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了92.45% 的用户
```
