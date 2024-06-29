<p>Design a HashMap without using any built-in hash table libraries.</p>

<p>Implement the <code>MyHashMap</code> class:</p>

<ul>
	<li><code>MyHashMap()</code> initializes the object with an empty map.</li>
	<li><code>void put(int key, int value)</code> inserts a <code>(key, value)</code> pair into the HashMap. If the <code>key</code> already exists in the map, update the corresponding <code>value</code>.</li>
	<li><code>int get(int key)</code> returns the <code>value</code> to which the specified <code>key</code> is mapped, or <code>-1</code> if this map contains no mapping for the <code>key</code>.</li>
	<li><code>void remove(key)</code> removes the <code>key</code> and its corresponding <code>value</code> if the map contains the mapping for the <code>key</code>.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```css
Input
["MyHashMap", "put", "put", "get", "get", "put", "get", "remove", "get"]
[[], [1, 1], [2, 2], [1], [3], [2, 1], [2], [2], [2]]
Output
[null, null, null, 1, -1, null, 1, null, -1]

Explanation
MyHashMap myHashMap = new MyHashMap();
myHashMap.put(1, 1); // The map is now [[1,1]]
myHashMap.put(2, 2); // The map is now [[1,1], [2,2]]
myHashMap.get(1);    // return 1, The map is now [[1,1], [2,2]]
myHashMap.get(3);    // return -1 (i.e., not found), The map is now [[1,1], [2,2]]
myHashMap.put(2, 1); // The map is now [[1,1], [2,1]] (i.e., update the existing value)
myHashMap.get(2);    // return 1, The map is now [[1,1], [2,1]]
myHashMap.remove(2); // remove the mapping for 2, The map is now [[1,1]]
myHashMap.get(2);    // return -1 (i.e., not found), The map is now [[1,1]]
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>0 &lt;= key, value &lt;= 10<sup>6</sup></code></li>
	<li>At most <code>10<sup>4</sup></code> calls will be made to <code>put</code>, <code>get</code>, and <code>remove</code>.</li>
</ul> 
<p><strong>Follow up:</strong> Please do not use the built-in HashMap library.</p>

题意：不使用任何内建的哈希表库设计一个哈希映射（`HashMap`）。 
- `MyHashMap()` 用空映射初始化对象
- `void put(int key, int value)` 向 `HashMap` 插入一个键值对 `(key, value)` 。如果 `key` 已经存在于映射中，则更新其对应的值 `value` 。
- `int get(int key)` 返回特定的 `key` 所映射的 `value` ；如果映射中不包含 `key` 的映射，返回 `-1` 。
- `void remove(key)` 如果映射中存在 `key` 的映射，则移除 `key` 和它所对应的 `value` 。

---
> 这道设计题有太多种解法，涉及到许多有意思的数据结构，如哈希表、二叉树、二叉搜索树、01字典树、AVL树、Splay、Treap、fhq Treap、跳表、红黑树等，慢慢更新吧。
> 
### 解法1 `unordered_map`
```cpp
class MyHashMap {
public:
    /** Initialize your data structure here. */
    unordered_map<int, int> rec;
    MyHashMap() { }
    
    /** value will always be non-negative. */
    void put(int key, int value) { rec[key] = value; }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        if (rec.find(key) != rec.end()) return rec[key];
        return -1;
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) { rec.erase(key); }
};
```
运行效率如下：
```cpp
执行用时：152 ms, 在所有 C++ 提交中击败了65.45% 的用户
内存消耗：51.2 MB, 在所有 C++ 提交中击败了41.78% 的用户
```

---
### 解法2 01字典树
按照键的二进制形式，将值插入到字典树结点中：
```cpp
class Trie01Node {
private:
    int val;
    Trie01Node *sons[2];
public:
    Trie01Node(int v) : val(v) { sons[0] = sons[1] = nullptr; }
    void insert(int d, int x) { //key,val(val非负数)
        if (d) {
            if (sons[d & 1] == nullptr) sons[d & 1] = new Trie01Node(-1);
            sons[d & 1]->insert(d >> 1, x); //递归插入
        } else val = x; //d==0
    }
    void erase(int d) {
        if (d) {
            if (sons[d & 1] == nullptr) return;
            sons[d & 1]->erase(d >> 1);
        } else val = -1;
    }
    int query(int d) {
        if (d) {
            if (sons[d & 1] == nullptr) return -1;
            return sons[d & 1]->query(d >> 1);
        } else return val;
    }
};
class MyHashMap {
private:
    Trie01Node* root;
public:
    /** Initialize your data structure here. */ 
    MyHashMap() { root = new Trie01Node(-1); }
    
    /** value will always be non-negative. */
    void put(int key, int value) { root->insert(key, value); }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) { return root->query(key); }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) { root->erase(key); }
}; 
```
运行效率如下：
```cpp
执行用时：140 ms, 在所有 C++ 提交中击败了70.70% 的用户
内存消耗：57.2 MB, 在所有 C++ 提交中击败了38.59% 的用户
```
---
