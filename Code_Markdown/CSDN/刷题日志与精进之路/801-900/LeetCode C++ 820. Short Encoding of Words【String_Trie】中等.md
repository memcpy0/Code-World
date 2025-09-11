<p>A <strong>valid encoding</strong> of an array of <code>words</code> is any reference string <code>s</code> and array of indices <code>indices</code> such that:</p>

<ul>
	<li><code>words.length == indices.length</code></li>
	<li>The reference string <code>s</code> ends with the <code>'#'</code> character.</li>
	<li>For each index <code>indices[i]</code>, the <strong>substring</strong> of <code>s</code> starting from <code>indices[i]</code> and up to (but not including) the next <code>'#'</code> character is equal to <code>words[i]</code>.</li>
</ul>

<p>Given an array of <code>words</code>, return <em>the <strong>length of the shortest reference string</strong> </em><code>s</code><em> possible of any <strong>valid encoding</strong> of </em><code>words</code><em>.</em></p>


<p><strong>Example 1:</strong></p>

```clike
Input: words = ["time", "me", "bell"]
Output: 10
Explanation: A valid encoding would be s = "time#bell#" and indices = [0, 2, 5].
words[0] = "time", the substring of s starting from indices[0] = 0 to the next '#' is underlined in "time#bell#"
words[1] = "me", the substring of s starting from indices[1] = 2 to the next '#' is underlined in "time#bell#"
words[2] = "bell", the substring of s starting from indices[2] = 5 to the next '#' is underlined in "time#bell#"
```

<p><strong>Example 2:</strong></p>

```clike
Input: words = ["t"]
Output: 2
Explanation: A valid encoding would be s = "t#" and indices = [0]. 
```

<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= words.length &lt;= 2000</code></li>
	<li><code>1 &lt;= words[i].length &lt;= 7</code></li>
	<li><code>words[i]</code> consists of only lowercase letters.</li>
</ul>

题意： 单词数组&nbsp;<code>words</code> 的 <strong>有效编码</strong> 由任意助记字符串 <code>s</code> 和下标数组 <code>indices</code> 组成，且满足：  
<ul>
	<li><code>words.length == indices.length</code></li>
	<li>助记字符串 <code>s</code> 以 <code>'#'</code> 字符结尾</li>
	<li>对于每个下标 <code>indices[i]</code> ，<code>s</code> 的一个从 <code>indices[i]</code> 开始、到下一个 <code>'#'</code> 字符结束（但不包括 <code>'#'</code>）的 <strong>子字符串</strong> 恰好与 <code>words[i]</code> 相等</li>
</ul>

<p>给你一个单词数组&nbsp;<code>words</code> ，返回成功对 <code>words</code> 进行编码的最小助记字符串 <code>s</code> 的长度 。</p>


---
### 解法 Trie
这道题的题意有点难懂，其实就是对字符串数组 `words[]` 进行编码。对于字符串 `s` ，其在 `words[]` 中的位置是 `i` 。如果 `s` 能够与 `t` 的后缀重合，就可以在助记字符串中只记录 `t` ，并在 `indices[]` 数组中 `s` 对应的位置 `indices[i]` 上，记录 `s` 出现在 `t` 中的开始位置。

例如 `words = ["time", "me", "bell"]` ，`words[1] = "me"` 出现在 `words[0] = "time"` 的 `2` 位置，于是助记字符串为 `time#` ，`indices[1] = 2` 。也不要忘记 `indices[0] = 0` 。这种做法的实质就是，**尽可能地匹配字符串后缀**。

题目要求出，使用这种编码方法得到的最短助记字符串的长度。于是我们使用前缀树，将所有的原始字符串倒着插入到前缀树中，然后计算最长的那些字符串的长度，加上 `'#'` 的个数，就是最短助记字符串的长度。
```cpp
class Trie {
private:
    Trie *next[26];
    size_t len;
public:
    Trie() : len(0) { memset(next, 0, sizeof(next)); }
    void insert(const string& s) {
        Trie *cur = this;
        for (int i = s.size() - 1; i >= 0; --i) {
            int index = s[i] - 'a';
            if (cur->next[index] == nullptr) cur->next[index] = new Trie;
            cur = cur->next[index];
        }
        cur->len = s.size();
    }
    int getLengthEncoding() {
        int lensum = 0;
        queue<Trie*> q;
        for (int i = 0; i < 26; ++i) if (next[i]) q.push(next[i]);
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                Trie *t = q.front(); q.pop();
                bool isleaf = true;
                for (int j = 0; j < 26; ++j) 
                    if (t->next[j]) { q.push(t->next[j]); isleaf = false; }
                if (isleaf) lensum += t->len + 1; //除了单词长度外还有一个'#'
            }
        }
        return lensum;
    }
};
class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        if (words.size() == 1) return words[0].size() + 1;
        Trie dict;
        for (const string &word : words) dict.insert(word);
        return dict.getLengthEncoding();
    }
};
```
运行效率如下：
```cpp
执行用时：60 ms, 在所有 C++ 提交中击败了58.07% 的用户
内存消耗：73.9 MB, 在所有 C++ 提交中击败了30.43% 的用户
```
