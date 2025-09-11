<p>Given a binary string <code>s</code> <strong>​​​​​without leading zeros</strong>, return <code>true</code>​​​ <em>if </em><code>s</code><em> contains <strong>at most one contiguous segment of ones</strong></em>. Otherwise, return <code>false</code>.</p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: s = "1001"
Output: false
Explanation: The ones do not form a contiguous segment.
```

 
<p><strong>Example 2:</strong></p>

```clike
Input: s = "110"
Output: true
```

 
 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 100</code></li>
	<li><code>s[i]</code>​​​​ is either <code>'0'</code> or <code>'1'</code>.</li>
	<li><code>s[0]</code> is&nbsp;<code>'1'</code>.</li>
</ul>




题意：给你一个二进制字符串 `s` ，该字符串 不含前导零 。如果 `s` 最多包含 一个由连续的 `'1'` 组成的字段 ，返回 `true​​​` 。否则，返回 `false` 。
 
---
### 解法1 直接遍历
计算连续 `'1'` 的片段个数。或者可以小优化一下，在遇到第二个片段时就退出：
```cpp
class Solution {
public:
    bool checkOnesSegment(string s) {
        int n = s.size(), start = -1, count = 0;
        for (int i = 0; i < n; ++i) {
            switch (s[i]) {
                case '1': 
                    if (start == -1) start = i, ++count;
                    break;
                case '0': start = -1; break;
            } 
        }
        return count <= 1;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了11.54% 的用户
```
---
### 解法2 字符串查找
由于没有前导零，只能是 `'1'` 字符开头，所以出现第二个连续的 `'1'` 片段，就说明存在 `"01"` ：
```cpp
class Solution {
public:
    bool checkOnesSegment(string s) {
        return s.find("01") == string::npos;
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：5.9 MB, 在所有 C++ 提交中击败了84.85% 的用户
```
