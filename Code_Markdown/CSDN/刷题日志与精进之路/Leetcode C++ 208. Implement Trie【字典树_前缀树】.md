Implement a trie with insert, search, and startsWith methods.

Example:
```
Trie trie = new Trie();

trie.insert("apple");
trie.search("apple");   // returns true
trie.search("app");     // returns false
trie.startsWith("app"); // returns true
trie.insert("app");   
trie.search("app");     // returns true
```
Note:
- You may assume that all inputs are consist of lowercase letters  `a-z` .
-   All inputs are guaranteed to be non-empty strings.

题意：实现一棵前缀字典树，所有输入字符都是小写字母，没有空字符串。

方法：套模板即可。

```cpp
class TrieNode {
public:
    TrieNode *next[26];
    int num; // 包含此前缀的单词数量
    bool terminate; // 终结符, 存在以这个字符结点为末字符的字符串
    TrieNode() : num(0), terminate(false) {
        memset(next, 0, sizeof(next)); 
    }
    ~TrieNode() {
        for (int i = 0; i < 26; ++i) {
            if (next[i]) 
                delete next[i];
        }
    }
};

class Trie {
public:
    /** Initialize your data structure here. */
    TrieNode *trie;
    Trie() {
        trie = new TrieNode;
    }
    ~Trie() {
        delete trie;
    }
    /** Inserts a word into the trie. */
    void insert(string word) {
        TrieNode *p = trie;
        for (const auto s : word) {
            int n = s - 'a';
            if (p->next[n] == nullptr) 
                p->next[n] = new TrieNode;
            p = p->next[n];
            p->num++;
        }
        p->terminate = true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        TrieNode *p = trie;
        for (const auto s : word) {
            int n = s - 'a';
            if (p->next[n] == nullptr) return false;
            p = p->next[n];
        }
        return p->terminate;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        TrieNode *p = trie;
        for (const auto s : prefix) {
            int n = s - 'a';
            if (p->next[n] == nullptr) return false;
            p = p->next[n];
        }
        return true;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */
```
