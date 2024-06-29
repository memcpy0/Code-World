> 本文属于「征服LeetCode」系列文章之一，这一系列正式开始于2021/08/12。由于LeetCode上部分题目有锁，本系列将至少持续到刷完所有无锁题之日为止；由于LeetCode还在不断地创建新题，本系列的终止日期可能是永远。在这一系列刷题文章中，我不仅会讲解多种解题思路及其优化，还会用多种编程语言实现题解，涉及到通用解法时更将归纳总结出相应的算法模板。
> <b></b>
> 为了方便在PC上运行调试、分享代码文件，我还建立了相关的仓库：[https://github.com/memcpy0/LeetCode-Conquest](https://github.com/memcpy0/LeetCode-Conquest)。在这一仓库中，你不仅可以看到LeetCode原题链接、题解代码、题解文章链接、同类题目归纳、通用解法总结等，还可以看到原题出现频率和相关企业等重要信息。如果有其他优选题解，还可以一同分享给他人。
> <b></b>
> 由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏[征服LeetCode系列文章目录](https://memcpy0.blog.csdn.net/article/details/119656559)一文以作备忘。


---
### 解法 哈希映射+二维双向链表
这一题是之前做过的 `LRU Cache` 的扩展，尽管都会使用哈希映射和双向链表，但是本题更加复杂——需要使用一个哈希映射和一个二维双向链表；哈希映射记录 `key` 到双向链表结点指针的键值对；一级链表维护 `LFU` ，按照频次排序；二级链表维护一个 `LRU` ，每一条二级链表代表具有相同使用次数的缓存；`put, get` 时更新结点的使用频次、最近是否使用的性质。具体代码如下，比较长：
```cpp
struct FreqNode;  //结构体前向声明
struct DataNode { //数据双向链表结点
    int key, val; //key:val
    int freq = 1; //使用次数,以便实现LFU;通过二级链表中的顺序实现LRU
    DataNode *prev, *next;  //前后指针 
    FreqNode *freqList;     //数据结点所在的频率结点
    DataNode(int k = 0, int v = 0, DataNode *p = nullptr, DataNode *n = nullptr, FreqNode *f = nullptr) 
        : key(k), val(v), prev(p), next(n), freqList(f) {}
};
struct FreqNode { //频率双向链表结点
    int freq;     //这一条数据结点双向链表的频率
    FreqNode *prior, *after; //前后指针
    DataNode *head, *tail; 	 //管理数据结点双向链表的头尾
    FreqNode(int freq = 0) {
        this->freq = freq;
        head = new DataNode();
        tail = new DataNode();
        head->next = tail;  
        tail->prev = head; 
    }
    ~FreqNode() {
        delete head;
        delete tail;
    }
    void removeDataNode(DataNode *node) { //当某一数据结点被使用,就会从链表中移除
        node->prev->next = node->next; //插入到更高使用频率的频率结点管理的数据结点双向链表的表头
        node->next->prev = node->prev;
    }
    void addDataNodeFirst(DataNode *node) { //最近使用的结点放到数据双向链表表头
        node->next = head->next;
        head->next->prev = node;
        node->prev = head;
        head->next = node;
        node->freqList = this;
    }
};

class LFUCache {
private:
    unordered_map<int, DataNode*> cache; //记录key:数据结点指针的键值对
    FreqNode *DHead, *DTail; //频率结点双向链表的头指针,尾指针
    int _capacity;
    void freqInc(DataNode* node) { //将node从原freq对应的数据结点双向链表中移除,如果链表空了则删除链表
        FreqNode *linkedlist = node->freqList;
        FreqNode *preList = linkedlist->prior;
        linkedlist->removeDataNode(node); //移除node
        if (linkedlist->head->next == linkedlist->tail) //该链表已经为空,则删除掉它
            removeFreqNode(linkedlist); //以使得DTail->prior始终非空,方便移除最不经常使用的、最近最少使用的结点
        ++node->freq; //将node加入新freq对应的链表中,若该链表不存在,则先创建该链表
        if (preList->freq != node->freq) {
            FreqNode *newList = new FreqNode(node->freq);
            addFreqNode(newList, preList);
            newList->addDataNodeFirst(node);
        } else preList->addDataNodeFirst(node);
        
    }
    void removeFreqNode(FreqNode *node) { //频率结点双向链表中表头代表最高的使用频率
        node->prior->after = node->after; //表尾代表最低的使用频率
        node->after->prior = node->prior; 
    }
    void addFreqNode(FreqNode *newList, FreqNode *preList) { //往频率结点双向链表插入结点
        newList->after = preList->after;
        newList->after->prior = newList;
        newList->prior = preList;
        preList->after = newList;
    }
public:
    LFUCache(int capacity) {
        _capacity = capacity;
        DHead = new FreqNode();
        DTail = new FreqNode();
        DHead->after = DTail;
        DTail->prior = DHead;
    }
    ~LFUCache() {
        delete DHead;
        delete DTail;
    }
    int get(int key) {
        if (cache.find(key) != cache.end()) {
            DataNode *node = cache[key];
            freqInc(node); //增加使用频率、最近使用过
            return node->val;
        } 
        return -1;
    }
    void put(int key, int value) {
        if (_capacity == 0) return; 
        if (cache.find(key) != cache.end()) { //已存在该key
            DataNode *node = cache[key];
            node->val = value; //修改值
            freqInc(node); //增加使用频率、最近使用过
        } else { //不存在该key
            if (cache.size() == _capacity) { //容量已满
                DataNode *temp = DTail->prior->tail->prev; //频率最低、最近最少使用的键值
                cache.erase(temp->key); //移除这一映射
                //如果缓存满了,则删除DTail->prior链表中的tail->prev的node
                DTail->prior->removeDataNode(temp);
                delete temp; //如果该链表已被删空,则删除该链表
                if (DTail->prior->head->next == DTail->prior->tail)
                    removeFreqNode(DTail->prior);
            }
            DataNode *newNode = new DataNode(key, value);
            cache[key] = newNode; //记录key:newNode之间的映射
            if (DTail->prior->freq != 1) { //新数据结点要插入使用频率为1次的数据结点双向链表中
                FreqNode *newList = new FreqNode(1);
                addFreqNode(newList, DTail->prior); //在DTail->prior之后插入新链表
                newList->addDataNodeFirst(newNode);
            } else DTail->prior->addDataNodeFirst(newNode); //插入到DTail->prior中
        }
    }
}; 
```
要注意的是，我们使用了二维双向链表，即双向链表的结点也是双向链表。通过使用双向链表（添加虚拟头结点和虚拟尾结点是为了**统一操作**），可以对任意已知结点进行 $O(1)$ 删除——对于LRU来说，删除功能用于**移除结点以更新最近使用的性质**（然后添加到表头）、**在容量已满时从表尾删除结点**；对LFU来说，删除功能用于**删除空的数据结点双向链表**（容量已满时删除数据结点导致空表、移动结点到更高频率链表上导致空表）。可以看出，这两者都需要**在任意位置进行删除**。

还需要的是添加结点的功能。对于LRU而言，**总是在表头添加结点**，做到 $O(1)$ 是很简单的；对于LFU而言，一方面需要**在表尾添加代表新的最低频率的结点**，另一方面由于增加结点使用次数，**要求移动结点到更高频率链表**（的表头，LRU实现的方法），**需要创建并插入新的频率结点**，于是需要**在任意位置进行插入**。这一点有所不同。

最后，通过键查找值的功能是由哈希映射实现的，它也承担着表达LFU缓存大小的功能。如果容量已满，就需要移除映射；否则直接插入新的键值对。