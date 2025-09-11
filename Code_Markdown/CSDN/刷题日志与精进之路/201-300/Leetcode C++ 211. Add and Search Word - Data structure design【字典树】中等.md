Design a data structure that supports the following two operations:
```cpp
void addWord(word)
bool search(word)
```
`search(word)` can search a literal word or a regular expression string containing only letters `a-z` or `.` . A `.` means it can represent any one letter.

Example:
```cpp
addWord("bad")
addWord("dad")
addWord("mad")
search("pad") -> false
search("bad") -> true
search(".ad") -> true
search("b..") -> true
```
**Note:** You may assume that all words are consist of lowercase letters `a-z` .

```cpp
class TrieNode {
    TrieNode *next[26];
    int num;
    int terminate;
    TrieNode() : num(0), terminate(false) {
        memsert(next, 0, sizeof(next));
    }
    ~TrieNode() {
        for (int i = 0; i < 26; ++i) 
            if (next[i])
                delete next[i];
    }
}

class WordDictionary {
public:
    /** Initialize your data structure here. */
    TrieNode *trie;
    WordDictionary() {
        trie = new TrieNode;
    }
    
    /** Adds a word into the data structure. */
    void addWord(string word) {
        if (word.empty()) return;
        TrieNode *p = trie;
        for (const auto s : word) {
            if (p->next[s - 'a'] == nullptr) 
                p->next[s - 'a'] = new TrieNode;
            p = p->next[s - 'a'];
            p->num++;
        }
        p->terminate = true;
    }
    
    /** Returns if the word is in the data structure. A word could contain the dot 
    character '.' to represent any one letter. */
    bool search(string word) {
        if (word.empty()) return false;
        TrieNode *p = trie;
        for (const auto s : word) {
            if (s != '.' || p->next[s - 'a'] == nullptr) return false;
            if (s != '.') p = p->next[s - 'a']; 
            else 
        }
        return p->terminate;
    }
};

/**
 * Your WordDictionary object will be instantiated and called as such:
 * WordDictionary* obj = new WordDictionary();
 * obj->addWord(word);
 * bool param_2 = obj->search(word);
 */
 ```
