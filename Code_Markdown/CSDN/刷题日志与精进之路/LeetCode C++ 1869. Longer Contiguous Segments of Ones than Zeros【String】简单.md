<p>Given a binary string <code>s</code>, return <code>true</code><em> if the <strong>longest</strong> contiguous segment of </em><code>1</code><em>s is <strong>strictly longer</strong> than the <strong>longest</strong> contiguous segment of </em><code>0</code><em>s in </em><code>s</code>. Return <code>false</code><em> otherwise</em>.</p>

<ul>
	<li>For example, in <code>s = "<u>11</u>01<u>000</u>10"</code> the longest contiguous segment of <code>1</code>s has length <code>2</code>, and the longest contiguous segment of <code>0</code>s has length <code>3</code>.</li>
</ul>

<p>Note that if there are no <code>0</code>s, then the longest contiguous segment of <code>0</code>s is considered to have length <code>0</code>. The same applies if there are no <code>1</code>s.</p>

 
<p><strong>Example 1:</strong></p>

```bash
Input: s = "1101"
Output: true
Explanation:
The longest contiguous segment of 1s has length 2: "1101"
The longest contiguous segment of 0s has length 1: "1101"
The segment of 1s is longer, so return true.
```

<p><strong>Example 2:</strong></p>

```bash
Input: s = "111000"
Output: false
Explanation:
The longest contiguous segment of 1s has length 3: "111000"
The longest contiguous segment of 0s has length 3: "111000"
The segment of 1s is not longer, so return false.
```
 
<p><strong>Example 3:</strong></p>

```bash
Input: s = "111000"
Output: false
Explanation:
The longest contiguous segment of 1s has length 3: "111000"
The longest contiguous segment of 0s has length 3: "111000"
The segment of 1s is not longer, so return false. 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= s.length &lt;= 100</code></li>
	<li><code>s[i]</code> is either <code>'0'</code> or <code>'1'</code>.</li>
</ul>

题意：给你一个二进制字符串 <code>s</code> 。如果字符串中由 <code>1</code> 组成的 <strong>最长</strong> 连续子字符串 <strong>严格长于</strong> 由 <code>0</code> 组成的 <strong>最长</strong> 连续子字符串，返回 <code>true</code> ；否则，返回 <code>false</code><em> </em>。

---
### 解法 扫描字符串
两次扫描遍历：
```cpp
class Solution {
private:
    int findLCSOfChar(const string &s, char c) {
        int maxlen = 0, curlen = 0;
        for (int i = 0, n = s.size(); i < n; ++i) {
            if (s[i] == c) ++curlen;
            else { 
                maxlen = max(maxlen, curlen);
                curlen = 0;
            }
        }
        return max(maxlen, curlen);
    }
public:
    bool checkZeroOnes(string s) {
        return findLCSOfChar(s, '1') > findLCSOfChar(s, '0');
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.1 MB, 在所有 C++ 提交中击败了14.45% 的用户
```
合两次扫描为一次：
```cpp
class Solution {
public:
    bool checkZeroOnes(string s) {
        int max1 = 0, cur1 = 0, max0 = 0, cur0 = 0;
        for (int i = 0, n = s.size(); i < n; ++i) {
            if (s[i] == '0') {
                ++cur0;
                max1 = max(max1, cur1);
                cur1 = 0;
            } else {
                ++cur1;
                max0 = max(max0, cur0); 
                cur0 = 0;
            }
        }
        return max(cur1, max1) > max(cur0, max0);
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6 MB, 在所有 C++ 提交中击败了21.86% 的用户
```
