**要求**
* 能够说清楚 LinkedList 对比 ArrayList 的区别，并重视纠正部分错误的认知

**LinkedList**
1. **基于双向链表，无需连续内存**
2. **随机访问慢**（要沿着链表遍历）
3. **头尾插入删除性能高**
4. 占用内存多

**ArrayList**
1. **基于数组，需要连续内存**
2. **随机访问快**（指根据下标访问）
3. **尾部插入、删除性能可以**，其它部分插入、删除都会移动数据，因此性能会低
4. **可以利用 cpu 缓存，局部性原理**（随机访问是根据索引，而查询需要根据元素内容，所以ArrayList和LinkedList都需要O(n)时间查询）

> ***代码说明***
> ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303231855915.png)
> * day01.list.ArrayListVsLinkedList#randomAccess 对比随机访问性能
> * day01.list.ArrayListVsLinkedList#addMiddle 对比向中间插入性能
> * day01.list.ArrayListVsLinkedList#addFirst 对比头部插入性能
> * day01.list.ArrayListVsLinkedList#addLast 对比尾部插入性能
> * day01.list.ArrayListVsLinkedList#linkedListSize 打印一个 LinkedList 占用内存
> * day01.list.ArrayListVsLinkedList#arrayListSize 打印一个 ArrayList 占用内存
