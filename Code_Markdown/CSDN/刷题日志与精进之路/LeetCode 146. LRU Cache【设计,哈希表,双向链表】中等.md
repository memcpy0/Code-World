> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。

请你设计并实现一个满足  [LRU (最近最少使用) 缓存](https://baike.baidu.com/item/LRU) 约束的数据结构。

实现 `LRUCache` 类：
- `LRUCache(int capacity)` 以 **正整数** 作为容量 `capacity` 初始化 LRU 缓存
- `int get(int key)` 如果关键字 `key` 存在于缓存中，则返回关键字的值，否则返回 `-1` 。
- `void put(int key, int value)` 如果关键字 `key` 已经存在，则变更其数据值 `value` ；如果不存在，则向缓存中插入该组 `key-value` 。如果插入操作导致关键字数量超过 `capacity` ，则应该 **逐出** 最久未使用的关键字。

函数 `get` 和 `put` 必须以 `O(1)` 的平均时间复杂度运行。

**示例：**
```js
输入
["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
输出
[null, null, null, 1, null, -1, null, -1, 3, 4]

解释
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // 缓存是 {1=1}
lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
lRUCache.get(1);    // 返回 1
lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
lRUCache.get(2);    // 返回 -1 (未找到)
lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
lRUCache.get(1);    // 返回 -1 (未找到)
lRUCache.get(3);    // 返回 3
lRUCache.get(4);    // 返回 4
```
**提示：**
- `1 <= capacity <= 3000`
- `0 <= key <= 10000`
- `0 <= value <= 10^5`
- 最多调用 `2 * 10^5` 次 `get` 和 `put`

---
题目集合：
- [146. LRU 缓存](https://leetcode.cn/problems/lru-cache/)
- [460. LFU 缓存](https://leetcode.cn/problems/lfu-cache/)
### 解法1 哈希映射+双向链表
**LRU 缓存机制可以通过哈希表辅以双向链表实现**，我们用一个哈希表和一个双向链表维护所有在缓存中的键值对。
- **双向链表**按照「被使用的顺序」存储了这些键值对，靠近尾部的键值对是最近使用的，而靠近头部的键值对是最久未使用的。
- **哈希映射**为普通的哈希映射（HashMap），通过缓存数据的键映射到其在双向链表中的位置。

这样以来，我们首先使用哈希表进行定位，找出缓存项在双向链表中的位置，随后将其移动到双向链表的尾部，即可在 $O(1)$ 的时间内完成 `get` 或者 `put` 操作。具体的方法如下：
- 对于 `get` 操作，首先判断 `key` 是否存在：
    - 如果 `key` 不存在，则返回 `-1` ；
    - 如果 `key` 存在，则 `key` 对应的节点是最近被使用的节点。通过哈希表定位到该节点在双向链表中的位置，并将其移动到双向链表的尾部，最后返回该节点的值。
- 对于 `put` 操作，首先判断 `key` 是否存在：
    - 如果 `key` 不存在，使用 `key` 和 `value` 创建一个新的节点，在双向链表的尾部添加该节点，并将 `key` 和该节点添加进哈希表中。==然后判断双向链表的节点数是否超出容量，如果超出容量，则删除双向链表的头部节点，并删除哈希表中对应的项==；
    - 如果 `key` 存在，则与 `get` 操作类似，先通过哈希表定位，再将对应的节点的值更新为 `value` ，并将该节点移到双向链表的尾部。

上述各项操作中，访问哈希表的时间复杂度为 $O(1)$，在双向链表的尾部添加节点、在双向链表的头部删除节点的复杂度也为 $O(1)$。而将一个节点移到双向链表的尾部，可以分成「删除该节点」和「在双向链表的尾部添加节点」两步操作，都可以在 $O(1)$ 时间内完成。
> 在双向链表的实现中，使用一个伪头部 *dummy head* 和伪尾部 *dummy tail* 标记界限，这样在添加节点和删除节点时，就不需要检查相邻的节点是否存在。
```cpp
struct DLinkedNode {
    int key, value;
    DLinkedNode* prev, *next;
    DLinkedNode(): key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int _key, int _value): key(_key), value(_value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode* head, *tail;
    int size, capacity;
public:
    LRUCache(int _capacity) : capacity(_capacity), size(0) {
        // 使用dummy头部和尾部节点
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }
    int get(int key) {
        if (!cache.count(key)) return -1;
        // 如果key存在,先通过哈希表定位,再移动到尾部
        DLinkedNode* node = cache[key];
        moveToTail(node);
        return node->value;
    }
    void put(int key, int value) {
        if (!cache.count(key)) {
            // 如果key不存在，创建一个新的节点
            DLinkedNode* node = new DLinkedNode(key, value);
            // 添加进哈希表
            cache[key] = node;
            // 添加进双向链表的尾部
            addToTail(node);
            ++size;
            if (size > capacity) {
                // 如果超出容量，删除双向链表的头部节点
                DLinkedNode* removed = removeHead();
                // 删除哈希表中对应的项
                cache.erase(removed->key);
                // 防止内存泄露
                delete removed;
                --size;
            }
        } else {
            // 如果key存在，先通过哈希表定位，再修改value，并移到尾部
            DLinkedNode* node = cache[key];
            node->value = value;
            moveToTail(node);
        }
    }
    void addToTail(DLinkedNode* node) {
        node->prev = tail->prev;
        tail->prev->next = node;
        node->next = tail;
        tail->prev = node;
    }
    void removeNode(DLinkedNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    void moveToTail(DLinkedNode* node) {
        removeNode(node);
        addToTail(node);
    }
    DLinkedNode* removeHead() {
        DLinkedNode* node = head->next;
        removeNode(node);
        return node;
    }
};
```
