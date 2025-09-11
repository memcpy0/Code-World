<p>A <a href="https://en.wikipedia.org/wiki/Trie"><strong>trie</strong></a> (pronounced as "try") or <strong>prefix tree</strong> is a tree data structure used to efficiently store and retrieve keys in a dataset of strings. There are various applications of this data structure, such as autocomplete and spellchecker.</p>

<p>Implement the Trie class:</p>

<ul>
	<li><code>Trie()</code> Initializes the trie object.</li>
	<li><code>void insert(String word)</code> Inserts the string <code>word</code> into the trie.</li>
	<li><code>boolean search(String word)</code> Returns <code>true</code> if the string <code>word</code> is in the trie (i.e., was inserted before), and <code>false</code> otherwise.</li>
	<li><code>boolean startsWith(String prefix)</code> Returns <code>true</code> if there is a previously inserted string <code>word</code> that has the prefix <code>prefix</code>, and <code>false</code> otherwise.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```clike
输入
["Trie", "insert", "search", "search", "startsWith", "insert", "search"]
[[], ["apple"], ["apple"], ["app"], ["app"], ["app"], ["app"]]
输出
[null, null, true, false, true, null, true]

解释
Trie trie = new Trie();
trie.insert("apple");
trie.search("apple");   // 返回 True
trie.search("app");     // 返回 False
trie.startsWith("app"); // 返回 True
trie.insert("app");
trie.search("app");     // 返回 True
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= word.length, prefix.length &lt;= 2000</code></li>
	<li><code>word</code> and <code>prefix</code> consist only of lowercase English letters.</li>
	<li>At most <code>3 * 10<sup>4</sup></code> calls <strong>in total</strong> will be made to <code>insert</code>, <code>search</code>, and <code>startsWith</code>.</li>
</ul>

题意：实现一个前缀树。

---
### 解法 字典树
```cpp
class Trie { 
public:
    bool isEnd;
    Trie *next[26]; //当字符集比较大时,用哈希表替代
    /** Initialize your data structure here. */
    Trie() { isEnd = false; memset(next, 0, sizeof(next)); }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        Trie *cur = this;
        int n = word.size();
        for (int i = 0; i < n; ++i) {
            int num = word[i] - 'a';
            if (cur->next[num] == nullptr) cur->next[num] = new Trie;
            cur = cur->next[num];
        }
        cur->isEnd = true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        Trie *cur = this;
        int n = word.size();
        for (int i = 0; i < n; ++i) {
            int num = word[i] - 'a';
            if (cur->next[num] == nullptr) return false;
            cur = cur->next[num];
        }
        return cur->isEnd;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        Trie *cur = this;
        int n = prefix.size();
        for (int i = 0; i < n; ++i) {
            int num = prefix[i] - 'a';
            if (cur->next[num] == nullptr) return false;
            cur = cur->next[num];
        }       
        return true;
    }
}; 
```
运行效率如下：
```cpp
执行用时：56 ms, 在所有 C++ 提交中击败了94.06% 的用户
内存消耗：44 MB, 在所有 C++ 提交中击败了24.06% 的用户
```
