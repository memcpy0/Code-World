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
    
    /** value will always be non-negative. val >= 0 */
    void put(int key, int value) { root->insert(key, value); }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) { return root->query(key); }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) { root->erase(key); }
};
