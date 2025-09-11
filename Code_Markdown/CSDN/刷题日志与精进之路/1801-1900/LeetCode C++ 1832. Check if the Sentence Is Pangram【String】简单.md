<p>A <strong>pangram</strong> is a sentence where every letter of the English alphabet appears at least once.</p>

<p>Given a string <code>sentence</code> containing only lowercase English letters, return<em> </em><code>true</code><em> if </em><code>sentence</code><em> is a <strong>pangram</strong>, or </em><code>false</code><em> otherwise.</em></p>
 
<p><strong>Example 1:</strong></p>

```clike
Input: sentence = "thequickbrownfoxjumpsoverthelazydog"
Output: true
Explanation: sentence contains at least one of every letter of the English alphabet. 
```

<p><strong>Example 2:</strong></p>

```clike
Input: sentence = "leetcode"
Output: false 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= sentence.length &lt;= 1000</code></li>
	<li><code>sentence</code> consists of lowercase English letters.</li>
</ul>

题意：**全字母句** 指包含英语字母表中每个字母至少一次的句子。给你一个仅由小写英文字母组成的字符串 `sentence` ，请你判断 `sentence` 是否为 **全字母句** 。如果是，返回 `true` ；否则，返回 `false` 。 


---
### 解法 直接遍历
使用布尔数组：
```cpp
class Solution {
public:
    bool checkIfPangram(string sentence) {
        vector<bool> vis(26);
        for (const char &c :sentence) vis[c - 'a'] = true;
        for (int i = 0; i < 26; ++i) if (vis[i] == false) return false;
        return true; 
    }
};
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.4 MB, 在所有 C++ 提交中击败了51.33% 的用户
```
使用位向量法：
```cpp
class Solution {
public:
    bool checkIfPangram(string sentence) {
        int ans = 0;
        for (const char &c :sentence) ans |= (1 << (c - 'a')); 
        return !(ans ^ 0x03ffffff); 
    }
}; 
```
运行效率如下：
```cpp
执行用时：0 ms, 在所有 C++ 提交中击败了100.00% 的用户
内存消耗：6.2 MB, 在所有 C++ 提交中击败了90.70% 的用户
```
