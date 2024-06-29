<p>Implement the <code>MapSum</code> class:</p>

<ul>
	<li><code>MapSum()</code> Initializes the&nbsp;<code>MapSum</code> object.</li>
	<li><code>void insert(String key, int val)</code> Inserts the <code>key-val</code> pair into the map. If the <code>key</code> already existed, the original <code>key-value</code> pair will be overridden to the new one.</li>
	<li><code>int sum(string prefix)</code> Returns&nbsp;the sum of all the pairs' value whose <code>key</code> starts with the <code>prefix</code>.</li>
</ul>
 
<p><strong>Example 1:</strong></p>

```clike
Input
["MapSum", "insert", "sum", "insert", "sum"]
[[], ["apple", 3], ["ap"], ["app", 2], ["ap"]]
Output
[null, null, 3, null, 5]

Explanation
MapSum mapSum = new MapSum();
mapSum.insert("apple", 3);  
mapSum.sum("ap");           // return 3 (apple = 3)
mapSum.insert("app", 2);    
mapSum.sum("ap");           // return 5 (apple + app = 3 + 2 = 5)
```

 
<p><strong>Constraints:</strong></p>

<ul>
	<li><code>1 &lt;= key.length, prefix.length &lt;= 50</code></li>
	<li><code>key</code> and <code>prefix</code> consist of only lowercase English letters.</li>
	<li><code>1 &lt;= val &lt;= 1000</code></li>
	<li>At most <code>50</code> calls will be made to <code>insert</code> and <code>sum</code>.</li>
</ul>

题意：实现一个 `MapSum` 类，支持两个方法，`insert` 和 `sum` ：
<ul>
	<li><code>MapSum()</code> 初始化 <code>MapSum</code> 对象</li>
	<li><code>void insert(String key, int val)</code> 插入 <code>key-val</code> 键值对，字符串表示键 <code>key</code> ，整数表示值 <code>val</code> 。如果键 <code>key</code> 已经存在，那么原来的键值对将被替代成新的键值对。</li>
	<li><code>int sum(string prefix)</code> 返回所有以该前缀 <code>prefix</code> 开头的键 <code>key</code> 的值的总和。</li>
</ul>

---
### 解法 前缀树
这里使用了一个变量 `total` 记录以字符串 `s` 为前缀的所有键对应的值之和，需要注意的是，如果键已经存在，要更新键对应的值，以及整个字符串路径上的 `total` ：
```cpp
class Trie {
private:
    bool last; //是否存在以这个结点结尾的字符串
    int v, total; //v记录键对应的值,total记录以此前缀开始的键对应的值之和
    Trie *next[26]; 
public:
    Trie() : last(false), total(0) { for (int i = 0; i < 26; ++i) next[i] = nullptr; }
    void insert(char const* s, int val) { //插入键值对,字符串表示key,整数表示val
        if (*s) {
            int i = *s - 'a';
            if (next[i] == nullptr) next[i] = new Trie;
            next[i]->total += val;  
            next[i]->insert(s + 1, val);
        } else { last = true, v = val; } //标记该字符串的结尾,更新对应的值
    }
    bool find(char const* s) { //检查这个键是否存在
        if (*s) {
            int i = *s - 'a';
            if (next[i] == nullptr) return false;
            return next[i]->find(s + 1);
        } else return last;
    }    
    int value(char const* s) { //返回键对应的值
        if (*s) {
            int i = *s - 'a';
            if (next[i] == nullptr) return 0; 
            return next[i]->value(s + 1);
        } else return v;
    }
    int sum(char const* s) { //返回所有以该前缀s开头的键的值的总和
        if (*s) {
            int i = *s - 'a';
            if (next[i] == nullptr) return 0; 
            return next[i]->sum(s + 1);
        } else return total;
    }
};
class MapSum {
private:
    Trie *t;
public:
    MapSum() { t = new Trie; }  
    void insert(string key, int val) { 
        char const* s = key.c_str();
        if (t->find(s)) t->insert(s, -t->value(s)); //已经存在这个键,需要减去相应路径上的原值
        t->insert(s, val); //随后更新键对应的值,以及路径上的total值
    }   
    int sum(string prefix) { return t->sum(prefix.c_str()); }
};
```
运行效率如下：
```cpp
执行用时：4 ms, 在所有 C++ 提交中击败了76.08% 的用户
内存消耗：7.9 MB, 在所有 C++ 提交中击败了56.70% 的用户
```
