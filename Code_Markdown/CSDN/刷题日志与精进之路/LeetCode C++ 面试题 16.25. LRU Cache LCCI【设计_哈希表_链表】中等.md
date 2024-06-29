
Design and build a "least recently used" cache, which evicts the least recently used item. The cache should map from keys to values (allowing you to insert and retrieve a value associ­ated with a particular key) and be initialized with a max size. When it is full, it should evict the least recently used item.

You should implement following operations:  `get` and `put`.

Get a value by key: `get(key)` - If key is in the cache, return the value, otherwise return `-1`.
Write a key-value pair to the cache: `put(key, value)` - If the key is not in the cache, then write its value to the cache. Evict the least recently used item before writing if necessary.

**Example:**

```clike
LRUCache cache = new LRUCache( 2 /* capacity */ );

cache.put(1, 1);
cache.put(2, 2);
cache.get(1);       // returns 1
cache.put(3, 3);    // evicts key 2
cache.get(2);       // returns -1 (not found)
cache.put(4, 4);    // evicts key 1
cache.get(1);       // returns -1 (not found)
cache.get(3);       // returns 3
cache.get(4);       // returns 4
```
题意：实现一个LRU数据结构。

---
### 解法 哈希表+双向链表
```cpp
class LRUCache {
private:
    struct node {
        int key, val;
        node *prev, *next;
        node(int k = 0, int v = 0, node *pr = nullptr, node *ne = nullptr) : 
            key(k), val(v), prev(pr), next(ne) {}
    }; 
    node *head, *rear; 
    unordered_map<int, node*> rec;
    int capacity, size;
    void removeToTail(node *cur) { 
        cur->prev->next = cur->next; //断开联系
        cur->next->prev = cur->prev;
        cur->prev = rear->prev; cur->next = rear; //将访问过的键值对移动到链表尾
        cur->prev->next = cur; rear->prev = cur;
    }
public:
    LRUCache(int capacity) { 
        assert(capacity > 0);
        this->capacity = capacity; 
        size = 0;
        head = new node, rear = new node; //虚拟头、尾结点的指针
        head->next = rear, rear->prev = head;
    } 
    int get(int key) { 
        if (rec.find(key) == rec.end()) return -1; 
        removeToTail(rec[key]); //访问
        return rec[key]->val;
    } 
    void put(int key, int value) {
        if (rec.find(key) != rec.end()) { //值已经存在时,更新值
            rec[key]->val = value;
            removeToTail(rec[key]); //访问
            return; 
        }
        if (size == capacity) { //容量已满  
            node *rem = head->next;   
            head->next = rem->next; //断开联系
            rem->next->prev = head;
            rec.erase(rem->key); //删除最近最少使用的键值对
            delete rem;
            --size; 
        } 
        node *temp = new node(key, value, rear->prev, rear); //插入到链表尾
        temp->prev->next = temp, rear->prev = temp;
        rec[key] = temp; //记录新键值对
        ++size;
    }
};
```
运行效率如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210413193602960.png)

