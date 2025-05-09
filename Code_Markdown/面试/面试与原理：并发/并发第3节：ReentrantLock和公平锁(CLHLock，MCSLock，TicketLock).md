> Java 学多少才能找到工作？
> 最近经常有小伙伴问我，以为我的经验来看，学多少够，好像更多的是看你的野
> 心有多大。如果你只是想找个 10k 以内的二线城市的工作，那还是比较容易的。也不需要学数据结构、也不需要会算法、也需要懂源码、更不要有多少项目经验。
> 但反之我遇到一个国内大学 TOP2 毕业的娃，这货就是 Offer 收割机：腾讯、阿里、字节还有国外新加坡的工作机会等等，薪资待遇也是贼高，可能超过你对白菜价的认知。上学无用、学习无用，纯属扯淡！
> 你能在这条路上能付出的越多，能努力的越早，收获就会越大！


# 一、面试题
谢飞机，小记，刚去冬巴拉泡完脚放松的飞机，因为耐克袜子丢了，骂骂咧咧的赴约面试官。
面试官：咋了，飞机，怎么看上去不高兴。
谢飞机：没事，没事，我心思我学的 synchronized 呢！
面试官：那正好，飞机你会锁吗？
谢飞机：啊。。。我没去会所呀！！！你咋知道
面试官：我说 Java 锁，你想啥呢！你了解公平锁吗，知道怎么实现的吗，给我说说！
谢飞机：公平锁！？嗯，是不 ReentrantLock 中用到了，我怎么感觉似乎有印象，但是不记得了。
面试官：哎，回家搜搜 CLH 吧！
# 二、ReentrantLock 和 公平锁
## 1. ReentrantLock 介绍
鉴于上一篇小傅哥已经基于，HotSpot 虚拟机源码分析 synchronized 实现和相应 核 心 知 识 点 ， 本 来 想 在 本 章 直 接 介 绍 下 ReentrantLock 。 但 因 为 ReentrantLock 知识点较多，因此会分几篇分别讲解，突出每一篇重点，避免猪八戒吞枣。

介绍：ReentrantLock 是一个**可重入且独占式锁**，具有**与 synchronized 监视器 (monitor enter、monitor exit)锁基本相同的行为和语意**。但与 synchronized 相比，它更加灵活、强大、增加了轮询、**超时、中断**等高级功能以及可以创建公平和非公平锁。
## 2. ReentrantLock 知识链条
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305301200494.png)
ReentrantLock 是基于 Lock 实现的可重入锁，**所有的 Lock 都是基于 AQS 实现的，AQS 和 Condition 各自维护不同的对象**，在使用 Lock 和 Condition 时，其实就是两个队列的互相移动。**它所提供的共享锁、互斥锁都是基于对 state 的操作**。而它的**可重入是因为实现了同步器 Sync**，在 Sync 的两个实现类中，包括了公平锁和非公平锁。

这个公平锁的具体实现，就是我们本章节要介绍的重点，了解什么是公平锁、公平锁的具体实现。学习完基础的知识可以更好的理解 ReentrantLock
## 3. ReentrantLock 公平锁代码
### 3.1 初始化
```java
ReentrantLock lock = new ReentrantLock(true); // true：公平锁
lock.lock();
try {
    // todo
} finally {
    lock.unlock();
}
```
- 初始化构造函数入参，选择是否为初始化公平锁。
- 其实一般情况下并不需要公平锁，除非你的场景中需要保证顺序性。
- 使用 ReentrantLock 切记需要在 finally 中关闭，lock.unlock()。

### 3.2 公平锁、非公平锁，选择
```java
public ReentrantLock(boolean fair) {
    sync = fair ? new FairSync() : new NonfairSync();
}
```
构造函数中选择公平锁（FairSync）、非公平锁（NonfairSync）。

### 3.3 hasQueuedPredecessors
```java
static final class FairSync extends Sync {
    protected final boolean tryAcquire(int acquires) {
        final Thread current = Thread.currentThread();
        int c = getState();
        if (c == 0) {
            if (!hasQueuedPredecessors() &&
                compareAndSetState(0, acquires)) {
                setExclusiveOwnerThread(current);
                return true;
            }
        }
    ...
    }
}
```
公平锁和非公平锁，主要是**在方法 tryAcquire 中**，**是否有 !hasQueuedPredecessors() 判断**。
### 3.4 队列首位判断
```java
public final boolean hasQueuedPredecessors() {
    Node t = tail; // Read fields in reverse initialization order
    Node h = head;
    Node s;
    return h != t &&
        ((s = h.next) == null || s.thread != Thread.currentThread());
}
```
- 在这个判断中主要就是看当前线程是不是同步队列的首位，是：true、否：false
- 这部分就涉及到了公平锁的实现，CLH（Craig，Landin andHagersten）。三个作者的首字母组合

# 三、什么是公平锁
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305301206190.png)
公平锁就像是马路边上的卫生间，上厕所需要排队。当然如果有人不排队，那么就是非公平锁了，比如领导要先上。

**CLH 是一种基于单向链表的高性能、公平的自旋锁**。AQS 中的队列是 CLH 变体的虚拟双向队列（FIFO），AQS 是通过将**每条请求共享资源的线程**封装成一个节点来实现锁的分配。

为了更好的学习理解 CLH 的原理，就需要有实践的代码。接下来一 CLH 为核心分别介绍 4 种公平锁的实现，从而掌握最基本的技术栈知识。
# 四、公平锁实现
## 1. CLH
CLH（Craig，Landin and Hagersten），是一种基于链表的可扩展、高性能、公平的自旋锁。
### 1.1 看图说话
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305301208068.png)
### 1.2 代码实现
在 这 段 代 码 的 实 现 过 程 中 ， 相 当 于 是 **虚 拟 出 来 一 个 链 表 结 构** ， 由 **AtomicReference** 的方法 getAndSet 进行**衔接**。getAndSet 获取当前元素，设置新的元素
```java
public class CLHLock implements Lock {
    private final ThreadLocal<CLHLock.Node> prev;
    private final ThreadLocal<CLHLock.Node> node;
    private final AtomicReference<CLHLock.Node> tail = new AtomicReference<>(new CLHLock.Node());
    private static class Node {
        private volatile boolean locked;
    }
    public CLHLock() {
        this.prev = ThreadLocal.withInitial(() -> null);
        this.node = ThreadLocal.withInitial(CLHLock.Node::new);
    }
    @Override
    public void lock() {
        final Node node = this.node.get();
        node.locked = true;
        Node pred_node = this.tail.getAndSet(node);
        this.prev.set(pred_node);
        // 自旋
        while (pred_node.locked);
    }
    @Override
    public void unlock() {
        final Node node = this.node.get();
        node.locked = false;
        this.node.set(this.prev.get());
    }
}
```
lock()
- 通过 this.node.get() 获取当前线程拥有的 `ThreadLocal` 的节点 `node` ，并设置 locked 为 true。
- 接着调用 this.tail.getAndSet(node)，获取当前尾部节点 pred_node，同时把新加入的节点设置成尾部节点。
- 之后就是把 this.prev 设置为之前的尾部节点，也就相当于链路的指向。
- 最后就是**自旋 while (pred_node.locked)，直至程序释放**。 

unlock()
- 释放锁的过程就是拆链，把释放锁的节点设置为 false node.locked = false。
- 之后最重要的是把当前节点设置为上一个节点（不断将上个节点或者说 **第一次tail.getAndSet得到的我们传入的虚拟CLHLock头结点**设置过来），这样就相当于把自己的节点拆下来了，等着垃圾回收。

CLH 队列锁的优点是**空间复杂度低**，在 SMP（Symmetric Multi-Processor）对称多处理器结构（一台计算机由多个 CPU 组成，并共享内存和其他资源，所有的 CPU 都可以平等地访问内存、I/O 和外部中断）效果还是不错的。但在 NUMA(Non-Uniform Memory Access) 下效果就不太好了，这部分知识可以自行扩展。

## 2. MCSLock
MCS 来自于发明人名字的首字母： John Mellor-Crummey 和 Michael Scott。它也是一种基于链表的可扩展、高性能、公平的自旋锁。但**与 CLH 不同，它是真的有下一个节点 next**，添加这个真实节点后，它就**可以只在本地变量上自旋**，而 CLH 是**前驱节点的属性上自旋**。
### 2.1 代码实现
```java
public class MCSLock implements Lock {
    private AtomicReference<MCSLock.Node> tail = new AtomicReference<>(null);
    ;
    private ThreadLocal<MCSLock.Node> node;
    
    private static class Node {
        private volatile boolean locked = false;
        private volatile Node next = null;
    }
    public MCSLock() {
        node = ThreadLocal.withInitial(Node::new);
    }
    @Override
    public void lock() {
        Node node = this.node.get();
        Node preNode = tail.getAndSet(node);
        if (null == preNode) {
            node.locked = true; // 是首个结点，能在自己上加锁
            return;
        }
        node.locked = false; 
        preNode.next = node; // 设置前个结点的下个next为node，尾插法 
        while (!node.locked) ; // 自旋
    }
    @Override
    public void unlock() {
        Node node = this.node.get();
        if (null != node.next) { // 有下个结点时 
            node.next.locked = true; // 锁下个结点
            node.next = null;
            return;
        }
        if (tail.compareAndSet(node, null)) { // ?
            return;
        }
        while (node.next == null) ; // ?
    }
}
```
因为自旋节点的不同，导致 CLH 更适合于 SMP 架构、MCS 可以适合 NUMA 非一致存储访问架构。你可以想象成 CLH 更需要线程数据在同一块内存上效果才更好，MCS 因为是在本地变量自旋，所以无论数据是否分散在不同的 CPU 模块都没有影响。 

代码实现上与 CLH 没有太多差异，这里就不在叙述了，可以看代码学习。
## 3. TicketLock
### 3.1 看图说话
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305301225283.png)
图 16-4 银行排队叫号图
### 3.2 代码实现
TicketLock 就像你去银行、呷哺给你的一个排号卡一样，叫到你号你才能进去。属于严格的公平性实现，但是多处理器系统上，**每个进程/线程占用的处理器都在读写同一个变量**，每次读写操作都需要进行**多处理间的缓存同步**，非常消耗系统性能。

代码实现上也比较简单，**lock() 中设置拥有者的号牌，并进入自旋比对**。unlock() 中**使用 CAS 进行解锁操作，并处理移除**。
```java
public class TicketLock implements Lock {
    private AtomicInteger serviceCount = new AtomicInteger(0);
    private AtomicInteger ticketCount = new AtomicInteger(0);
    private final ThreadLocal<Integer> owner = new ThreadLocal<>();
    
    @Override
    public void lock() {
        owner.set(ticketCount.getAndIncrement()); // owner: 0, ticketCount: 1 发0号票
        while (serviceCount.get() != owner.get()); // serviceCount: 0 没叫到个人票号
    }
    @Override
    public void unlock() {
        serviceCount.compareAndSet(owner.get(), owner.get() + 1); // 该叫下一个票号的人
        owner.remove();
    }
}
```
# 五、总结
ReentrantLock 是**基于 Lock 实现的可重入独占锁**，对于公平锁 CLH 的实现，只是这部分知识的冰山一角，但有这一脚，就可以很好热身便于后续的学习。

**ReentrantLock 使用起来更加灵活，可操作性也更大，但一定要在 finally 中释放锁**，目的是保证在获取锁之后，最终能够被释放。同时**不要将获取锁的过程写在try 里面**。

公平锁的实现依据不同场景和 SMP、NUMA 的使用，会有不同的优劣效果。**在实际的使用中一般默认会选择非公平锁，即使是自旋也是耗费性能的**，一般会用在较少等待的线程中，避免自旋时过长。