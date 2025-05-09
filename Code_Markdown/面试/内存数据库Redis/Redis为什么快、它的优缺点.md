- **内存存储**
Redis 是使用内存（in-memeroy）存储，数据存在内存中，类似于 HashMap，HashMap 的优势就是查找和操作的时间复杂度都是 O(1)。

**单线程实现**（Redis 6.0 以前）
Redis 使用单个线程处理请求，避免了多个线程之间**线程切换**和**锁资源争用**的开销。注意：==单线程是指的是在核心网络模型中，网络请求模块使用一个线程来处理==。

**非阻塞 IO**
Redis 使用**多路复用 I/O 技术**，将 epoll 作为 I/O 多路复用技术的实现，再加上 Redis 自身的事件处理模型将 epoll 中的连接、读写、关闭都转换为事件，不在网络 I/O 上浪费过多的时间。

**优化的数据结构**
Redis 有很多可以直接使用的**优化数据结构**的方法，应用层可以直接使用原生的数据结构提升性能。

**使用底层模型不同**
由于一般的系统调用系统函数会浪费一定的时间去移动和请求，Redis 直接自己构建了虚拟内存机制 。

### Redis的优缺点
Redis 本质上是一个 **Key-Value 类型的内存数据库**，整个数据库加载在内存当中操作，**定期通过异步操作把数据库中的数据 flush 到硬盘上进行保存**。

因为是纯内存操作，Redis 的性能非常出色，每秒可以处理超过 10 万次读写操作，是**已知性能最快的 Key-Value 数据库**。

优点：
- **读写性能极高**， Redis 能读的速度是 110000次/s，写的速度是 81000次/s。
- **支持数据持久化**，支持 AOF 和 RDB 两种持久化方式。
- 支持事务， **Redis 的所有操作都是原子性的**，意思就是要么成功执行要么失败完全不执行。单个操作是原子性的。**多个操作也支持事务**，即原子性，通过 MULTI 和 EXEC 指令包起来。
- **数据结构丰富**，除了支持 string 类型的 value 外，还支持 hash、set、zset、list 等数据结构。
- **支持主从复制**，主机会自动将数据同步到从机，**可以进行读写分离**。
- 丰富的特性 – Redis 还支持 publish/subscribe， 通知， key 过期等特性。

缺点：
- **数据库容量受到物理内存的限制**，不能用作海量数据的高性能读写，因此 Redis 适合的场景主要局限在**较小数据量的高性能操作**和运算上。
- **主机宕机，宕机前有部分数据未能及时同步到从机**，切换 IP 后还会引入数据不一致的问题，**降低了系统的可用性**。
 