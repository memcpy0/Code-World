常说面试造火箭，入职拧螺丝。但你真的有造火箭的本事吗，大部分都是不敢承认自己的知识盲区和技术瓶颈以及经验不足的自嘲。

面试时：我希望你懂数据结构，因为这样的你在使用 HashMap、ArrayList、LinkedList，更加得心应手。
我希望你懂散列算法，因为这样的你在设计路由时，会有很多选择；除法散列法、平方散列法、斐波那契（Fibonacci）散列法等。
我希望你懂开源代码，因为这样的你在遇到问题时，可以快速定位，还可能创造出一些系统服务的中间件，来更好的解耦系统。
我希望你懂设计模式，因为这样的你可以写出可扩展、易维护的程序，让整个团队都能向更好的方向发展。

所以，从不是 CRUD 选择了你，也不是造螺丝让你成为工具人。而是你的技术能力决定你的眼界，眼界又决定了你写出的代码！
# 1. 面试题
谢飞机，小记 还没有拿到 offer 的飞机，早早起了床，吃完两根油条，又跑到公司找面试官取经！
面试官：飞机，听坦克说，你最近贪黑起早的学习呀。
谢飞机：嗯嗯，是的，最近头发都快掉没了！
面试官：那今天我们聊聊 ThreadLocal，一般可以用在什么场景中？
谢飞机：嗯，ThreadLocal 要解决的是**线程内资源共享** (This class provides thread-local variables.)，所以一般会**用在全链路监控中**，或者是像日志框架 MDC 这样的组件里。
面试官：飞机不错哈，最近确实学习了。那你知道 ThreadLocal 是怎样的数据结构吗，采用的是什么散列方式？
谢飞机：数组？嗯，怎么散列的不清楚...
面试官：那 ThreadLocal 有内存泄漏的风险，是怎么发生的呢？另外你了解在这个过程的，探测式清理和启发式清理吗？
谢飞机：这...，盲区了，盲区了，可乐我放桌上了，我回家再看看书！

Regenerate response
# 2. ThreadLocal 分析
ThreadLocal，作者：Josh Bloch and Doug Lea，两位大神👍

如果仅是日常业务开发来看，这是一个比较冷门的类，使用频率并不高。并且它提供的方法也非常简单，一个功能只是潦潦数行代码。但，如果深挖实现部分的源码，就会发现事情并不那么简单。这里涉及了太多的知识点，包括；数据结构、拉链存储、斐波那契散列、神奇的 0x61c88647、弱引用 Reference、过期 key 探测清理和启发式清理等等。

接下来，我们就逐步学习这些盲区知识。本文涉及了较多的代码和实践验证图稿，
## 2.1 应用场景-SimpleDateFormat
```java
private SimpleDateFormat f = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
public void seckillSku(){
	String dateStr = f.format(new Date());
	// 业务流程
}
```
你写过这样的代码吗？如果还在这么写，那就已经犯了一个线程安全的错误。**SimpleDateFormat，并不是一个线程安全的类**。
## 2.1.1 线程不安全验证
```java
private static SimpleDateFormat f = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
public static void main(String[] args) {
	while (true) {
		new Thread(() -> {
			String dateStr = f.format(new Date());
			try {
				Date parseDate = f.parse(dateStr);
				String dateStrCheck = f.format(parseDate);
				boolean equals = dateStr.equals(dateStrCheck);
				if (!equals) {
					System.out.println(equals + " " + dateStr + " " + dateStrCheck);
				} else {
					System.out.println(equals);
				}
			} catch (ParseException e) {
				System.out.println(e.getMessage());
			}
		}).start();
	}
}
```
这是一个多线程下 SimpleDateFormat 的验证代码。**当 equals 为 false 时，证明线程不安全**。运行结果如下；
```java
true
true
false 2020-09-23 11:40:42 2230-09-23 11:40:42
true
true
false 2020-09-23 11:40:42 2020-09-23 11:40:00
false 2020-09-23 11:40:42 2020-09-23 11:40:00
false 2020-09-23 11:40:00 2020-09-23 11:40:42
true
```
### 2.1.2 线程安全优化
使用 ThreadLocal 优化
**为了线程安全最直接的方式，就是每次调用都直接 new SimpleDateFormat**。但这样的方式终究不是最好的，所以我们使用 ThreadLocal ，来优化这段代码。
```java
package com.memcpy0.interview;  
import java.text.ParseException;  
import java.text.SimpleDateFormat;  
import java.util.Date;  
public class Scene {  
    private static SimpleDateFormat f = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
    private static ThreadLocal<SimpleDateFormat> threadLocal = ThreadLocal.withInitial(() -> new SimpleDateFormat("yyyy-MM-dd HH:mm:ss")); 
    public static void main(String[] args) {  
        while (true) {  
            new Thread(() -> {  
//                String dateStr = f.format(new Date());  
                String dateStr = threadLocal.get().format(new Date());  
                try {  
//                    Date parseDate = f.parse(dateStr);  
//                    String dateStrCheck = f.format(parseDate);  
                    Date parseDate = threadLocal.get().parse(dateStr);  
                    String dateStrCheck = threadLocal.get().format(parseDate);  
                    boolean equals = dateStr.equals(dateStrCheck);  
                    if (!equals) {  
                        System.out.println(equals + " " + dateStr + " " + dateStrCheck);  
                    } else {  
                        System.out.println(equals);  
                    }  
                } catch (ParseException e) {  
                    System.out.println(e.getMessage());  
                }  
            }).start();  
        }  
    }  
}
```
如上我们把 `SimpleDateFormat` ，放到 ThreadLocal 中进行使用，**既不需要重复 new 对象，也避免了线程不安全问题**。测试结果如下；
```java
true
true
true
true
true
true
```

## 2.2 链路追踪 
近几年基于[谷歌 Dapper 论文](https://bigbully.github.io/Dapper-translation/)实现非入侵全链路追踪，使用的越来越广了。简单说这就是一套监控系统，但不需要你硬编码的方式进行监控方法，而是**基于它的设计方案采用 javaagent + 字节码 插桩的方式，动态采集方法执行信息**。如果你 想 了 解 字 节 码 插 桩 技 术 ， 可 以 阅 读  字 节 码 编 程 专 栏 ：
https://bugstack.cn/itstack-demo-agent/itstack-demo-agent.html

重点，**动态采集方法执行信息。这块是主要部分，跟 ThreadLocal 相关**。字节码插桩解决的是非入侵式编程，那么在一次服务调用时，在各个系统间以及系统内多个方法的调用，都需要进行采集。这个时候就需要使用 ThreadLocal 记录方法执行 ID，当然这里还有**跨线程调用使用的也是增强版本的 ThreadLocal**，但无论如何基本原理不变。
### 2.2.1 追踪代码
这里举例全链路方法调用链追踪，部分代码
```java
public class TrackContext {
	private static final ThreadLocal<String> trackLocal = new ThreadLocal<>();
	public static void clear(){
		trackLocal.remove();
	}
	public static String getLinkId(){
		return trackLocal.get();
	}
	public static void setLinkId(String linkId){
		trackLocal.set(linkId);
	}
	@Advice.OnMethodEnter()
	public static void enter(@Advice.Origin("#t") String className, @Advice.Origin("#m"
		) String methodName) {
		Span currentSpan = TrackManager.getCurrentSpan();
		if (null == currentSpan) {
		String linkId = UUID.randomUUID().toString();
		TrackContext.setLinkId(linkId);
		}
		TrackManager.createEntrySpan();
	}
	@Advice.OnMethodExit()
	public static void exit(@Advice.Origin("#t") String className, @Advice.Origin("#m")
		String methodName) {
		Span exitSpan = TrackManager.getExitSpan();
		if (null == exitSpan) return;
		System.out.println("链路追踪(MQ)：
		" + exitSpan.getLinkId() + " " + className + "." + methodName + " 耗时：
		" + (System.currentTimeMillis() - exitSpan.getEnterTime().getTime()) + "ms");
	}
}
```
以上这部分就是非入侵监控中，链路追踪的过程。具体的案例和代码可以参考阅读，系列专题文章《基于 JavaAgent 的全链路监控》

这也只是其中一个实现方式，字节码插桩使用的是 byte-buddy，其实还是使用，ASM 或者 Javassist。
### 2.2.2 测试结果
配置参数：-javaagent:E:\itstack\GIT\itstack.org\itstack-demo-
agent\itstack-demo-agent-06\target\itstack-demo-agent-06-1.0.0-
SNAPSHOT.jar=testargs
```java 
public void http_lt1(String name) {
	try {
		Thread.sleep((long) (Math.random() * 500));
	} catch (InterruptedException e) {
		e.printStackTrace();
	}
	System.out.println("测试结果：hi1 " + name);
	http_lt2(name);
}
public void http_lt2(String name) {
	try {
		Thread.sleep((long) (Math.random() * 500));
	} catch (InterruptedException e) {
		e.printStackTrace();
	}
	System.out.println("测试结果：hi2 " + name);
	http_lt3(name);
}
```
运行结果
```java
onTransformation：class org.itstack.demo.test.ApiTest
测试结果：hi2 悟空
测试结果：hi3 悟空
链路追踪(MQ)：90c7d543-c7b8-4ec3-af4d-
b4d4f5cff760 org.itstack.demo.test.ApiTest.http_lt3 耗时：104ms
init: 256MB max: 3614MB used: 44MB committed: 245MB use rate: 18%
init: 2MB max: 0MB used: 13MB committed: 14MB use rate: 95%
name: PS Scavenge count:0 took:0 pool name:[PS Eden Space, PS Survivor Space]
name: PS MarkSweep count:0 took:0 pool name:[PS Eden Space, PS Survivor Space, P
S Old Gen]
-----------------------------------------------------------------------------------
--------------
链路追踪(MQ)：90c7d543-c7b8-4ec3-af4d-
b4d4f5cff760 org.itstack.demo.test.ApiTest.http_lt2 耗时：233ms
init: 256MB max: 3614MB used: 44MB committed: 245MB use rate: 18%
init: 2MB max: 0MB used: 13MB committed: 14MB use rate: 96%
name: PS Scavenge count:0 took:0 pool name:[PS Eden Space, PS Survivor Space]
name: PS MarkSweep count:0 took:0 pool name:[PS Eden Space, PS Survivor Space, P
S Old Gen]
```
以上是链路追踪的测试结果，可以看到两个方法都会打出相应的编码 ID：90c7d543-c7b8-4ec3-af4d-b4d4f5cff760。

这部分也就是全链路追踪的核心应用，而且还可以看到这里打印了一些系统简单的JVM 监控指标，这也是监控的一部分。

咳咳，除此之外**所有需要活动方法调用链的，都需要使用到 ThreadLocal**，例如 MDC 日志框架等。接下来我们开始详细分析 ThreadLocal 的实现。

# 3. 数据结构
Java的ThreadLocal是**一个线程级别的变量**，它可以**让每个线程都拥有自己的变量副本**。在多线程并发编程中，线程安全是一个常见的问题，而**使用ThreadLocal可以方便地解决一些线程安全的问题**。

**ThreadLocal可以理解为一个线程独立的存储空间 thread-local variables**，不是ThreadLocal变量则意味着每个线程都共享同个变量。每个线程都有自己的ThreadLocal变量，线程之间互不干扰。ThreadLocal通常被定义为静态变量，每个线程可以通过它的get()和set()方法来访问它所拥有的变量副本。
- 当一个线程访问ThreadLocal的get()方法时，它会先获取当前线程的ThreadLocalMap对象，然后通过ThreadLocal对象作为key来获取对应的value，这个value就是当前线程所拥有的变量副本。
- 当一个线程访问ThreadLocal的set()方法时，它会先获取当前线程的ThreadLocalMap对象，然后将ThreadLocal对象作为key，将要设置的值作为value存入ThreadLocalMap中。这样，每个线程都可以独立地访问自己的ThreadLocal变量，互不干扰。

```java
This class provides thread-local variables. These variables differ from their normal counterparts in that each thread that accesses one (via its get or set method) has its own, independently initialized copy of the variable. ThreadLocal instances are typically private static fields in classes that wish to associate state with a thread (e.g., a user ID or Transaction ID).
For example, the class below generates unique identifiers local to each thread. A thread's id is assigned the first time it invokes ThreadId.get() and remains unchanged on subsequent calls.
  import java.util.concurrent.atomic.AtomicInteger;
 
  public class ThreadId {
      // Atomic integer containing the next thread ID to be assigned
      private static final AtomicInteger nextId = new AtomicInteger(0);
 
      // Thread local variable containing each thread's ID
      private static final ThreadLocal<Integer> threadId =
          new ThreadLocal<Integer>() {
              @Override protected Integer initialValue() {
                  return nextId.getAndIncrement();
          }
      };
 
      // Returns the current thread's unique ID, assigning it if necessary
      public static int get() {
          return threadId.get();
      }
  }
  
Each thread holds an implicit reference to its copy of a thread-local variable as long as the thread is alive and the ThreadLocal instance is accessible; after a thread goes away, all of its copies of thread-local instances are subject to garbage collection (unless other references to these copies exist).
```
使用ThreadLocal可以方便地解决一些多线程并发编程中的问题，例如**线程安全的日期格式化**、**数据库连接管理**等。但需要注意的是，**使用ThreadLocal也可能引发内存泄漏的问题**。因为==每个ThreadLocal变量都只能由对应的线程访问，如果线程结束时没有正确地清理ThreadLocal变量，就可能导致ThreadLocalMap中存储的ThreadLocal对象没有被垃圾回收，从而引发内存泄漏问题==。因此，在使用ThreadLocal时需要注意**正确地清理ThreadLocal变量**。

了解一个功能前，先了解它的数据结构。这就相当于先看看它的地基，有了这个根本也就好往后理解了。以下是 ThreadLocal 的简单使用以及部分源码。
`new ThreadLocal<String>().set("小傅哥");`
```java
private void set(ThreadLocal<?> key, Object value) {
	Entry[] tab = table;
	int len = tab.length;
	int i = key.threadLocalHashCode & (len-1);
	for (Entry e = tab[i];
		e != null;
		e = tab[i = nextIndex(i, len)]) {
	...
}
```
从这部分源码中可以看到，ThreadLocal 底层采用的是数组结构存储数据，同时还有哈希值计算下标，这说明它是一个散列表的数组结构，演示如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305072211137.png)
如上图是 ThreadLocal 存放数据的底层数据结构，包括知识点如下；
1. 它是一个数组结构。
2. Entry，这里没有再打开，其实**它是一个弱引用实现**，`static class Entry extends WeakReference<ThreadLocal<?>>` 。这说明**只要没有强引用存在，发生 GC 时就会被垃圾回收**。
3. 数据元素采用哈希散列方式进行存储，不过这里的散列使用的是 **斐波那契**（Fibonacci）散列法，后面会具体分析。
4. 另外**由于这里不同于 HashMap 的数据结构，发生哈希碰撞不会存成链表或红黑树，而是使用~~拉链法~~进行存储**。也就是同一个下标位置发生冲突时，则+1 向后寻址，直到找到空位置或垃圾回收位置进行存储。

ThreadLocal依赖于附加到每个线程的「线性探测哈希Map」，Thread.threadLocals and inheritableThreadLocals。ThreadLocal对象充当键（仅在ThreadLocalMaps中有用），**通过实例属性threadLocalHashCode进行搜索**。
```java
public class ThreadLocal<T> {  
    /**  
     * ThreadLocals rely on per-thread linear-probe hash maps attached     
     * to each thread (Thread.threadLocals and     
     * inheritableThreadLocals).  The ThreadLocal objects act as keys,     
     * searched via threadLocalHashCode.  This is a custom hash code     
     * (useful only within ThreadLocalMaps) that eliminates collisions     
     * in the common case where consecutively constructed ThreadLocals     
     * are used by the same threads, while remaining well-behaved in     
     * less common cases.     */    
     private final int threadLocalHashCode = nextHashCode();
```
在Thread类中：
```java
/* ThreadLocal values pertaining to this thread. This map is maintained  
 * by the ThreadLocal class. */
ThreadLocal.ThreadLocalMap threadLocals = null;  
  
/*  
 * InheritableThreadLocal values pertaining to this thread. This map is 
 * maintained by the InheritableThreadLocal class. */
 ThreadLocal.ThreadLocalMap inheritableThreadLocals = null;

/**  
 * This method is called by the system to give a Thread * a chance to clean up before it actually exits. */
private void exit() {  
    if (threadLocals != null && TerminatingThreadLocal.REGISTRY.isPresent()) {  
        TerminatingThreadLocal.threadTerminated();  
    }  
    if (group != null) {  
        group.threadTerminated(this);  
        group = null;  
    }  
    /* Aggressively null out all reference fields: see bug 4006245 */  
    target = null;  
    /* Speed the release of some of these resources */  
    threadLocals = null;  
    inheritableThreadLocals = null;  // 在线程结束时清理这些ThreadLocal信息
    inheritedAccessControlContext = null;  
    blocker = null;  
    uncaughtExceptionHandler = null;  
}
```
接着看ThreadLocal.ThreadLocalMap：**一个自定义的哈希映射，仅适用于维护thread local values**。==这个类是**包私有的**，允许在类Thread中声明字段threadLocals和inheritableThreadLocals==。为了帮助处理very large和long-lived的使用，这个哈希表的Entry（包装了键ThreadLocal和值v）使用WeakReference作为key（或者说**Entry是继承了弱引用 `WeakReference<ThreadLocal<?>>` 类的子类，会在构造时创建一个弱引用、指向传入的 `ThreadLocal<?>` 对象**）。但是，由于不使用引用队列，因此只有当表开始空间不足时，才能保证删除过时的条目。
```java 
/**  
 * ThreadLocalMap is a customized hash map suitable only for * maintaining thread local values. No operations are exported * outside of the ThreadLocal class. The class is package private to * allow declaration of fields in class Thread.  To help deal with * very large and long-lived usages, the hash table entries use * WeakReferences for keys. However, since reference queues are not * used, stale entries are guaranteed to be removed only when * the table starts running out of space. */
class ThreadLocal {
	static class ThreadLocalMap {  
	  
		/**  
		 * The entries in this hash map extend WeakReference, using     
		 * its main ref field as the key (which is always a     
		 * ThreadLocal object).  Note that null keys (i.e. entry.get()     
		 * == null) mean that the key is no longer referenced, so the     
		 * entry can be expunged from table.  Such entries are referred to     
		 * as "stale entries" in the code that follows.     
		 */    
		static class Entry extends WeakReference<ThreadLocal<?>> {  
		/** The value associated with this ThreadLocal. */  
		Object value;  
  
		Entry(ThreadLocal<?> k, Object v) {  
			super(k);  
			value = v;  
		}  
	}
}

...

  
public class WeakReference<T> extends Reference<T> {  
    /**  
     * Creates a new weak reference that refers to the given object.  The new     * reference is not registered with any queue.     *     * @param referent object the new weak reference will refer to  
     */    public WeakReference(T referent) {  
        super(referent);  
    }  
  
    /**  
     * Creates a new weak reference that refers to the given object and is     * registered with the given queue.     *     * @param referent object the new weak reference will refer to  
     * @param q the queue with which the reference is to be registered,  
     *          or {@code null} if registration is not required  
     */    public WeakReference(T referent, ReferenceQueue<? super T> q) {  
        super(referent, q);  
    }  
  
}
```
# 4. 散列算法
既然 ThreadLocal 是基于数组结构的拉链法存储，那就一定会有哈希的计算。但我们翻阅源码后，发现这个哈希计算与 HashMap 中的散列求数组下标计算的哈希方式不一样。
## 4.1 神秘的数字0x61c88647
当我们查看 ThreadLocal 执行设置元素时，有这么一段计算哈希值的代码；
```java   
private final int threadLocalHashCode = nextHashCode();
/**  
 * The next hash code to be given out. Updated atomically. Starts at 
 * zero. */
private static AtomicInteger nextHashCode =  // 下个被给出的哈希码，原子更新
    new AtomicInteger();  
  
/**  
 * The difference between successively generated hash codes - turns * implicit sequential thread-local IDs into near-optimally spread * multiplicative hash values for power-of-two-sized tables. */
private static final int HASH_INCREMENT = 0x61c88647;  // 连续生成的哈希码间的差，将隐式顺序的thread-local IDs转换为？？？
  
/**  
 * Returns the next hash code. */
 private static int nextHashCode() {  
    return nextHashCode.getAndAdd(HASH_INCREMENT);  
}
```
以ThreadLocalMap的两个构造函数之一为例：
```java
/**  
 * Construct a new map initially containing (firstKey, firstValue). 
 * ThreadLocalMaps are constructed lazily, so we only create 
 * one when we have at least one entry to put in it. 
 **/
ThreadLocalMap(ThreadLocal<?> firstKey, Object firstValue) {  // 只有在有entry要放进去时，才懒构造ThreadLocalMap
    table = new Entry[INITIAL_CAPACITY];  
    int i = firstKey.threadLocalHashCode & (INITIAL_CAPACITY - 1);  // threadLocalHashCode哈希值 & 容量-1 作为下标
    table[i] = new Entry(firstKey, firstValue);  
    size = 1;  // entry个数为1
    setThreshold(INITIAL_CAPACITY);  
}
```
另一个构造函数是、从给定的一个父ThreadLocalMap中，遍历其还存在的Entry、对其中不为null的key及其对应的value，重新hash `key.threadLocalHashCode & (len - 1);` 并放进表中：
```java
/**  
 * Construct a new map including all Inheritable ThreadLocals * from given parent map. Called only by createInheritedMap. 
 * 
 * @param parentMap the map associated with parent thread.  
 */private ThreadLocalMap(ThreadLocalMap parentMap) {  
    Entry[] parentTable = parentMap.table;  
    int len = parentTable.length;  
    setThreshold(len);  
    table = new Entry[len];  
  
    for (Entry e : parentTable) {  
        if (e != null) {  
            @SuppressWarnings("unchecked")  
            ThreadLocal<Object> key = (ThreadLocal<Object>) e.get();  
            if (key != null) {  
                Object value = key.childValue(e.value);  
                Entry c = new Entry(key, value);  
                int h = key.threadLocalHashCode & (len - 1);  
                while (table[h] != null)  // 已经有值
                    h = nextIndex(h, len);   // h++ % len寻找新的空位置
                table[h] = c;  
                size++;  
            }  
        }  
    }  
}
```
看到这里你一定会有这样的疑问，这是什么方式计算哈希？这个数字怎么来的？

讲到这里，其实计算哈希的方式，绝不止是我们平常看到 String 获取哈希值的一种方式，还包括；除法散列法、平方散列法、斐波那契（Fibonacci）散列法、随机数法等。

而 ThreadLocal 使用的就是 **斐波那契**（Fibonacci）散列法 + 拉链法存储数据到数组结构中。之所以使用斐波那契数列，是为了让数据更加散列，减少哈希碰撞。
具体来自数学公式的计算求值，公式：
$$f(k) = ((k * 2654435769) >> X) << Y$$ 对于常见的 32 位整数而言，也就是 f(k) = (k * 2654435769) >> 28

第二个问题，数字 0x61c88647，是怎么来的？其实这是一个哈希值的黄金分割点，也就是 0.618，你还记得你学过的数学吗？
计算方式如下；
```java
// 黄金分割点：(√5 - 1) / 2 = 0.6180339887 1.618:1 == 1:0.618
System.out.println(BigDecimal.valueOf(Math.pow(2, 32) * 0.6180339887).intValue());
//-1640531527
```
学过数学都应该知道，黄金分割点是，(√5 - 1) / 2，取 10 位近似 0.6180339887。

之后用 2 ^ 32 * 0.6180339887，得到的结果是：-1640531527，也就是 16 进制的，$0x61c88647$ 。这个数呢也就是这么来的

## 4.2 验证散列
既然，Josh Bloch 和 Doug Lea，两位老爷子选择使用斐波那契数列，计算哈希值。那一定有它的过人之处，也就是**能更好的散列，减少哈希碰撞**。

接下来我们按照源码中获取哈希值和计算下标的方式，把这部分代码提出出来做验证。
### 4.2.1 部分源码
如上，源码部分采用的是 AtomicInteger，原子方法计算下标。我们不需要保证线程安全，只需要简单实现即可。另外 ThreadLocalMap 初始化数组长度是 16，我们也初始化这个长度。
```java
public class ApiTest {  
  
    private static final ThreadLocal<String> threadLocal = new ThreadLocal<>();  
  
    private static AtomicInteger nextHashCode = new AtomicInteger();  
    private static final int HASH_INCREMENT = 0x61c88647;
```
### 4.2.2 单元测试
```java
@Test  
public void test_idx() {  
    int hashCode = 0;  
    for (int i = 0; i < 16; ++i) {  
        hashCode = i * HASH_INCREMENT + HASH_INCREMENT;  
        System.out.println("fibonacci hash value: " +  
                (hashCode & 15) + " normal hash value: " + (String.valueOf(i).hashCode() & 15));  
    }  
}
```
测试代码部分，采用的就是斐波那契数列，同时我们加入普通哈希算法进行比对散列效果。当然 **String 这个哈希并没有像 HashMap 中进行扰动**

测试结果：发现没？，斐波那契散列的非常均匀，普通散列到 15 个以后已经开发生产碰撞。这也就是斐波那契散列的魅力，**减少碰撞也就可以让数据存储的更加分散，获取数据的时间复杂度基本保持在 O(1)**。
```java
fibonacci hash value: 7 normal hash value: 0
fibonacci hash value: 14 normal hash value: 1
fibonacci hash value: 5 normal hash value: 2
fibonacci hash value: 12 normal hash value: 3
fibonacci hash value: 3 normal hash value: 4
fibonacci hash value: 10 normal hash value: 5
fibonacci hash value: 1 normal hash value: 6
fibonacci hash value: 8 normal hash value: 7
fibonacci hash value: 15 normal hash value: 8
fibonacci hash value: 6 normal hash value: 9
fibonacci hash value: 13 normal hash value: 15
fibonacci hash value: 4 normal hash value: 0
fibonacci hash value: 11 normal hash value: 1
fibonacci hash value: 2 normal hash value: 2
fibonacci hash value: 9 normal hash value: 3
fibonacci hash value: 0 normal hash value: 4

进程已结束,退出代码0

```
# 5. 源码解读
## 5.1 初始化
```java
new ThreadLocal<>()
```
初始化的过程也很简单，可以按照自己需要的泛型进行设置。但在 `ThreadLocal` 的源码 中有一 点非 常重要 ，就是 获取 `threadLocal` 的 **哈希值的获取**，
threadLocalHashCode。
```java
private final int threadLocalHashCode = nextHashCode();
/**
 * Returns the next hash code.
 */
private static int nextHashCode() {
	return nextHashCode.getAndAdd(HASH_INCREMENT);
}
```
如源码中，**只要实例化一个 ThreadLocal ，就会获取一个相应的哈希值**，则我们做一个例子。
```java
@Test  
public void test_threadLocalHashCode() throws Exception {  
    for (int i = 0; i < 15; i++) {  
        ThreadLocal<Object> objectThreadLocal = new ThreadLocal<>();  
        Field threadLocalHashCode = objectThreadLocal.getClass().getDeclaredField("threadLocalHashCode");  
        threadLocalHashCode.setAccessible(true);  
        System.out.println("objectThreadLocal：" + (((int)threadLocalHashCode.get(objectThreadLocal)) & 15));  
    }  
}
```
因为 threadLocalHashCode ，是一个私有属性，所以我们实例化后通过上面的方式进行获取哈希值。
```java
objectThreadLocal：-1401181199
objectThreadLocal：239350328
objectThreadLocal：1879881855
objectThreadLocal：-774553914
objectThreadLocal：865977613
```
这个值的获取，也就是计算 **往ThreadLocalMap存储数据时，ThreadLocal 的数组下标**。只要是这同一个对象，在 set、get 时，就可以设置和获取对应的值。

## 5.2 设置元素
### 5.2.1 流程图解
```java
new ThreadLocal<>().set("memcpy0");
```
设置元素的方法，也就这么一句代码。但设置元素的流程却涉及的比较多，在详细分析代码前，我们先来看一张设置元素的流程图，从图中先了解不同情况的流程之后再对比着学习源码。流程图如下；
![800](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305072312215.png)
乍一看可能感觉有点晕，我们从左往右看，分别有如下知识点； 0. 中间是ThreadLocal 的数组结构，之后在设置元素时分为四种不同的情况，==另外元素的插入是通过斐波那契散列计算下标值，进行存放的==。
1. 情况 1，待插入的下标，是空位置直接插入。
2. 情况 2，待插入的下标，不为空，key 相同，直接更新
3. 情况 3，待插入的下标，不为空，key 不相同，拉链法寻址
4. 情况 4，不为空，key 不相同，碰到过期 key。其实情况 4，遇到的是弱引用发生GC 时，产生的情况。碰到这种情况，ThreadLocal 会进行探测清理过期 key，这部分清理内容后续讲解。

### 5.2.2 源码解读
```java
// ThreadLocal中
/**  
 * Sets the current thread's copy of this thread-local variable 
 * to the specified value.  Most subclasses will have no need to 
 * override this method, relying solely on the {@link #initialValue}  
 * method to set the values of thread-locals. 
 * @param value the value to be stored in the current thread's copy of  
 *        this thread-local. 
 **/
public void set(T value) {  
    Thread t = Thread.currentThread();  
    ThreadLocalMap map = getMap(t);  
    if (map != null) {  
        map.set(this, value);  
    } else {  
        createMap(t, value);  // 线程中没有ThreadLocalMap时，即threadLocals字段为null时，由于要存储这对键值，就懒构造一个ThreadLocalMap
    }  
}

/**  
 * Create the map associated with a ThreadLocal. Overridden in * InheritableThreadLocal. * * @param t the current thread  
 * @param firstValue value for the initial entry of the map  
 */
void createMap(Thread t, T firstValue) {  
    t.threadLocals = new ThreadLocalMap(this, firstValue);  // 给threadLocals赋值
}

// ThreadLocalMap中
/**  
 * Set the value associated with key. * * @param key the thread local object  
 * @param value the value to be set  
 */
private void set(ThreadLocal<?> key, Object value) {  
  
    // We don't use a fast path as with get() because it is at  
    // least as common to use set() to create new entries as    
    // it is to replace existing ones, in which case, a fast    
    // path would fail more often than not.  
    Entry[] tab = table;  
    int len = tab.length;  
    int i = key.threadLocalHashCode & (len-1);  
  
    for (Entry e = tab[i];  
         e != null;  // 待插入的下标不为空
         e = tab[i = nextIndex(i, len)]) { // key 不相同，开放寻址
        if (e.refersTo(key)) {  // e作为弱引用，引用的键是key，即key相同，直接更新
            e.value = value;  
            return;        
        }  
  
        if (e.refersTo(null)) {  // 碰到过期 key。其实情况 4，遇到的是弱引用发生GC 时，产生的情况。碰到这种情况，ThreadLocal 会进行探测清理过期 key，这部分清理内容后续讲解。
            replaceStaleEntry(key, value, i);  
            return;        
        }  
    }  
  
    tab[i] = new Entry(key, value); // 空位置，直接插入 
    int sz = ++size;  
    if (!cleanSomeSlots(i, sz) && sz >= threshold)  
        rehash();  
}

/**  
 * Replace a stale entry encountered during a set operation with an entry for the specified key.  The value passed in * the value parameter is stored in the entry, whether or not * an entry already exists for the specified key. * * As a side effect, this method expunges all stale entries in the * "run" containing the stale entry.  (A run is a sequence of entries * between two null slots.) * 
 * @param  key the key  
 * @param  value the value to be associated with key  
 * @param  staleSlot index of the first stale entry encountered while  
 *         searching for key. 
 **/
 private void replaceStaleEntry(ThreadLocal<?> key, Object value,  
                               int staleSlot) {  // staleSlot是寻找key时碰到的第一个stale entry的下标
    Entry[] tab = table;  
    int len = tab.length;  
    Entry e;  
  
    // Back up to check for prior stale entry in current run.  
    // We clean out whole runs at a time to avoid continual    
    // incremental rehashing due to garbage collector freeing    
    // up refs in bunches (i.e., whenever the collector runs).    
    int slotToExpunge = staleSlot;  
    for (int i = prevIndex(staleSlot, len);  // 从staleSlot往前找，找到最前的stale entry，直到碰到null entry
         (e = tab[i]) != null;  
         i = prevIndex(i, len))  
        if (e.refersTo(null))  
            slotToExpunge = i;  
  
    // Find either the key or trailing null slot of run, whichever  
    // occurs first    
    for (int i = nextIndex(staleSlot, len);  // 往后寻找，要么找到key、要么是后面的null slot
         (e = tab[i]) != null;  
         i = nextIndex(i, len)) {  
		// If we find key, then we need to swap it  
		// with the stale entry to maintain hash table order.        
		// The newly stale slot, or any other stale slot        
		// encountered above it, can then be sent to expungeStaleEntry       
		// to remove or rehash all of the other entries in run.        
         if (e.refersTo(key)) {  // set key时碰到过期key（在staleSlot处）、需要清理，但不代表现在的key不存在于后面的位置，key和value可能在后面
            e.value = value;   // 往后找到key，将其与staleSlot位置的Entry交换，这样非空Entry连续了
  
            tab[i] = tab[staleSlot];  // 只可能把key交换到staleSlot处，而不会再往前交换
            tab[staleSlot] = e;  
  
            // Start expunge at preceding stale entry if it exists  
            if (slotToExpunge == staleSlot)  // 说明前面没有探寻到stale entry，而此时staleSlot已经更新为可用的Entry
                slotToExpunge = i;  // 从i往后清理过期Entry
            cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);  
            return;        
        }  
  
        // If we didn't find stale entry on backward scan, the  
        // first stale entry seen while scanning for key is the        
        // first still present in the run.        
        if (e.refersTo(null) && slotToExpunge == staleSlot)  // 又碰到一个过期Entry，并且前面没有stale entry
            slotToExpunge = i;  // 设置为staleSlot后面的第一个过期Entry的位置
    } 
    // If key not found, put new entry in stale slot  
    tab[staleSlot].value = null;  
    tab[staleSlot] = new Entry(key, value);  
  
    // If there are any other stale entries in run, expunge them  
    if (slotToExpunge != staleSlot)  
        cleanSomeSlots(expungeStaleEntry(slotToExpunge), len);  
}
```
1. `key.threadLocalHashCode & (len-1);`，斐波那契散列，计算数组下标。
2. Entry，**是一个弱引用对象的实现类**，`static class Entry extends WeakReference<ThreadLocal<?>>` ，所以在没有外部强引用下，会发生GC，删除 key。
3. for 循环判断元素是否存在，**当前下标不存在元素时，直接设置元素 `tab[i] = new Entry(key, value);`** 。
4. 如果元素存在，则会判断**是否 key 值相等** if (k == key)，相等则更新值。
5. 如果不相等，就到了我们的 `replaceStaleEntry` ，也就是上图说到的**探测式清理过期元素**（**后面查看**）。

综上，就是元素存放的全部过程，整体结构的设计方式非常赞👍，极大的利用了散列效果，也把弱引用使用的非常 6！

## 5.3 扩容机制
### 5.3.1 扩容条件
只要使用到数组结构，就一定会有扩容
```java
if (!cleanSomeSlots(i, sz) && sz >= threshold)
	rehash();
```
在我们阅读设置元素时，有以上这么一块代码，**判断是否扩容**。
- 首先，进行**启发式清理** *cleanSomeSlots*，把过期元素清理掉（**后面查看**）
- 之后，判断 `sz >= threshold` ，其中 `threshold = len * 2 / 3` ，也就是说数组中填充的元素，大于 `len * 2 / 3` ，就需要扩容了。
- 最后，就是我们要分析的重点，`rehash();` ，**扩容重新计算元素位置**。

```java
/**  
 * The next size value at which to resize. */
private int threshold; // Default to 0  
  
/**  
 * Set the resize threshold to maintain at worst a 2/3 load factor. */
private void setThreshold(int len) {  
    threshold = len * 2 / 3;  
}
```
### 5.3.2 源码分析-探测式清理和校验
这部分是主要是**探测式清理过期元素**，以及判断**清理后是否满足扩容条件**，`size >= threshold * 3/4` 。满足后执行扩容操作，其实扩容完的核心操作就是**重新计算哈希值，把元素填充到新的数组中**：清理过期元素时，如果发现一个Entry引用的键为null，则expungeStaleEntry会清理掉该项、且往后探测（如果后面有非空项，键为null就清除、键非null就重新计算Hash，说不定原先碰撞的位置被清空了呢）、直到碰到空项为止：
```java
private void rehash() {
	expungeStaleEntries();
	// Use lower threshold for doubling to avoid hysteresis
	if (size >= threshold - threshold / 4)
		resize();
}
private void expungeStaleEntries() {  
    Entry[] tab = table;  
    int len = tab.length;  
    for (int j = 0; j < len; j++) {  
        Entry e = tab[j];  
        if (e != null && e.refersTo(null))  // entry持有的ThreadLocal为null
            expungeStaleEntry(j);  
    }  
}

/**  
 * Expunge a stale entry by rehashing any possibly colliding entries 
 * lying between staleSlot and the next null slot.  This also expunges 
 * any other stale entries encountered before the trailing null.  See 
 * Knuth, Section 6.4 
 * 
 * @param staleSlot index of slot known to have null key  
 * @return the index of the next null slot after staleSlot  
 * (all between staleSlot and this slot will have been checked * for expunging). */
 private int expungeStaleEntry(int staleSlot) {  
    Entry[] tab = table;  
    int len = tab.length;  
  
    // expunge entry at staleSlot  
    tab[staleSlot].value = null;  // 设置值为null，断开和值的关系
    tab[staleSlot] = null;  // 设置Entry引用为null
    size--;  // 个数-1
  
    // Rehash until we encounter null  
    Entry e;  
    int i;  
    for (i = nextIndex(staleSlot, len);  
         (e = tab[i]) != null;  // 探测清理
         i = nextIndex(i, len)) {  
        ThreadLocal<?> k = e.get();  
        if (k == null) {  
            e.value = null;  
            tab[i] = null;  
            size--;  
        } else {  
            int h = k.threadLocalHashCode & (len - 1);  // 看key的ThreadLocal的哈希码
            if (h != i) {  
                tab[i] = null;
                // Unlike Knuth 6.4 Algorithm R, we must scan until  
                // null because multiple entries could have been stale.                
	            while (tab[h] != null)  
                    h = nextIndex(h, len);  
                tab[h] = e;  
            }  
        }  
    }  
    return i;  
}
```
### 5.3.3 rehash() 内调用的扩容resize
```java
/**  
 * Double the capacity of the table. */
 private void resize() {  
    Entry[] oldTab = table;  
    int oldLen = oldTab.length;  
    int newLen = oldLen * 2;  
    Entry[] newTab = new Entry[newLen];  
    int count = 0;  
  
    for (Entry e : oldTab) {  
        if (e != null) {  // 非空项
            ThreadLocal<?> k = e.get();  
            if (k == null) { // 引用空的键
                e.value = null; // Help the GC  
            } else {  // 重新hash并填到数组中
                int h = k.threadLocalHashCode & (newLen - 1);  
                while (newTab[h] != null)  
                    h = nextIndex(h, newLen);  
                newTab[h] = e;  
                count++;  // 计算剩余元素
            }  
        }  
    }  
  
    setThreshold(newLen);  // 重新设置阈值
    size = count;  
    table = newTab;  
}
```
以上，代码就是扩容的整体操作，具体包括如下步骤；
1. 首先把数组长度扩容到原来的 2 倍，oldLen * 2，实例化新数组。
2. 遍历 for，所有的旧数组中的元素，重新放到新数组中。
3. **在放置数组的过程中，如果发生哈希碰撞，则链式法顺延**。
4. 同时这**还有检测 key 值的操作 if (k == null)，方便 GC**。

## 5.4 获取元素
### 5.4.1 流程图解
```java
new ThreadLocal<>().get();
```
同样获取元素也就这么一句代码，如果没有分析源码之前，你能考虑到它在不同的数据结构下，获取元素时候都做了什么操作吗。我们先来看下图，分为如下情况；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305080044576.png)
### 5.4.2 源码分析
```java
// ThreadLocal

/**  
 * Returns the value in the current thread's copy of this * thread-local variable.  If the variable has no value for the * current thread, it is first initialized to the value returned * by an invocation of the {@link #initialValue} method.  
 * * @return the current thread's value of this thread-local  
 */
public T get() {  
    Thread t = Thread.currentThread();  
    ThreadLocalMap map = getMap(t);  
    if (map != null) {  
        ThreadLocalMap.Entry e = map.getEntry(this);  
        if (e != null) {  
            @SuppressWarnings("unchecked")  
            T result = (T)e.value;  
            return result;  
        }  
    }  
    return setInitialValue();  
}

/**  
 * Variant of set() to establish initialValue. Used instead * of set() in case user has overridden the set() method. * * @return the initial value  
 */private T setInitialValue() {  
    T value = initialValue();  
    Thread t = Thread.currentThread();  
    ThreadLocalMap map = getMap(t);  
    if (map != null) {  
        map.set(this, value);  
    } else {  
        createMap(t, value);  
    }  
    if (this instanceof TerminatingThreadLocal) {  
        TerminatingThreadLocal.register((TerminatingThreadLocal<?>) this);  
    }  
    return value;  
}

/**  
 * Returns the current thread's "initial value" for this * thread-local variable.  This method will be invoked the first * time a thread accesses the variable with the {@link #get}  
 * method, unless the thread previously invoked the {@link #set}  
 * method, in which case the {@code initialValue} method will not  
 * be invoked for the thread.  Normally, this method is invoked at * most once per thread, but it may be invoked again in case of * subsequent invocations of {@link #remove} followed by {@link #get}.  
 * * <p>This implementation simply returns {@code null}; if the  
 * programmer desires thread-local variables to have an initial * value other than {@code null}, {@code ThreadLocal} must be  
 * subclassed, and this method overridden.  Typically, an * anonymous inner class will be used. * * @return the initial value for this thread-local  
 */
protected T initialValue() {  
    return null;  
}

/**  
 * Creates a thread local variable. The initial value of the variable is * determined by invoking the {@code get} method on the {@code Supplier}.  
 * * @param <S> the type of the thread local's value  
 * @param supplier the supplier to be used to determine the initial value  
 * @return a new thread local variable  
 * @throws NullPointerException if the specified supplier is null  
 * @since 1.8  
 */
public static <S> ThreadLocal<S> withInitial(Supplier<? extends S> supplier) {  
    return new SuppliedThreadLocal<>(supplier);  
}
/**  
 * An extension of ThreadLocal that obtains its initial value from * the specified {@code Supplier}.  
 */
static final class SuppliedThreadLocal<T> extends ThreadLocal<T> {  
  
    private final Supplier<? extends T> supplier;  
  
    SuppliedThreadLocal(Supplier<? extends T> supplier) {  
        this.supplier = Objects.requireNonNull(supplier);  
    }  
  
    @Override  
    protected T initialValue() {  
        return supplier.get();  
    }  
}
```
#### 测试
```java
@Test  
public void test_set_get() throws Exception {  
    ThreadLocal<String> threadLocal = new ThreadLocal<>();  
    System.out.println(threadLocal.get());  
}
// null
```

```java
// ThreadLocalMap
  
/**  
 * Get the entry associated with key.  This method * itself handles only the fast path: a direct hit of existing * key. It otherwise relays to getEntryAfterMiss.  This is * designed to maximize performance for direct hits, in part * by making this method readily inlinable. * * @param  key the thread local object  
 * @return the entry associated with key, or null if no such  
 */
private Entry getEntry(ThreadLocal<?> key) {  
    int i = key.threadLocalHashCode & (table.length - 1); // 数组中下标
    Entry e = table[i];  
    if (e != null && e.refersTo(key))  // 非空项且key相同
        return e;  
    else return getEntryAfterMiss(key, i, e);  // 在冲突后调用该方法
}  
  
/**  
 * Version of getEntry method for use when key is not found in * its direct hash slot. * * @param  key the thread local object  
 * @param  i the table index for key's hash code  
 * @param  e the entry at table[i]  
 * @return the entry associated with key, or null if no such  
 */
private Entry getEntryAfterMiss(ThreadLocal<?> key, int i, Entry e) {  
    Entry[] tab = table;  
    int len = tab.length;  
  
    while (e != null) {  // 直到碰到空项就不找了，说明表中不存在key对应的值
        if (e.refersTo(key))  // key相同
            return e;  
        if (e.refersTo(null))  // 对应的key为空，是一个stale entry，需要回收
            expungeStaleEntry(i);  
        else i = nextIndex(i, len);  
        e = tab[i];  // 往后找
    }  
    return null;  
}
```
好了，这部分就是获取元素的源码部分，和我们图中列举的情况是一致的。==expungeStaleEntry，是发现有 key == null 时，进行清理过期元素，并把后续位置的元素，前移==。

## 5.5 元素清理
### 5.5.1 探测清理 `expungeStaleEntry`
探测式清理，是以当前遇到的 GC 元素开始，向后不断的清理。直到遇到 null 为止，才停止 rehash 计算 Rehash until we encounter null 。
```java
/**  
 * Expunge a stale entry by rehashing any possibly colliding entries * lying between staleSlot and the next null slot.  This also expunges * any other stale entries encountered before the trailing null.  See * Knuth, Section 6.4 * * @param staleSlot index of slot known to have null key  
 * @return the index of the next null slot after staleSlot  
 * (all between staleSlot and this slot will have been checked * for expunging). 
 **/
private int expungeStaleEntry(int staleSlot) {  
    Entry[] tab = table;  
    int len = tab.length;  
  
    // expunge entry at staleSlot  
    tab[staleSlot].value = null;  
    tab[staleSlot] = null;  
    size--;  
  
    // Rehash until we encounter null  
    Entry e;  
    int i;  
    for (i = nextIndex(staleSlot, len);  
         (e = tab[i]) != null;  
         i = nextIndex(i, len)) {  
        ThreadLocal<?> k = e.get();  
        if (k == null) {  
            e.value = null;  
            tab[i] = null;  
            size--;  
        } else {  
            int h = k.threadLocalHashCode & (len - 1);  
            if (h != i) {  
                tab[i] = null;  
                // Unlike Knuth 6.4 Algorithm R, we must scan until  
                // null because multiple entries could have been stale.                
	            while (tab[h] != null)  
                    h = nextIndex(h, len);  
                tab[h] = e;  
            }  
        }  
    }  
    return i;  
}
```
以上，探测式清理在获取元素和设置元素中使用到； `new ThreadLocal<>().get() -> map.getEntry(this) -> getEntryAfterMiss(key, i, e) -> expungeStaleEntry(i)` 。

### 5.5.2 启发式清理
`set(ThreadLocal<?> key, Object value)` 和 `replaceStaleEntry` 方法中：启发式清理，有这么一段注释，大概意思是；试探的扫描一些单元格，寻找过期元素，也就是被垃圾回收的元素。当添加新元素或删除另一个过时元素时，将调用此函数。它执行对数扫描次数，作为不扫描（快速但保留垃圾）和与元素数量成比例的扫描次数之间的平衡，这将找到所有垃圾，但会导致一些插入花费 $O(n)$ 时间。
```java
/**  
 * Heuristically scan some cells looking for stale entries. * This is invoked when either a new element is added, or * another stale one has been expunged. It performs a * logarithmic number of scans, as a balance between no * scanning (fast but retains garbage) and a number of scans * proportional to number of elements, that would find all * garbage but would cause some insertions to take O(n) time. * 
 * @param i a position known NOT to hold a stale entry. The  
 * scan starts at the element after i. * 
 * @param n scan control: {@code log2(n)} cells are scanned,  
 * unless a stale entry is found, in which case * {@code log2(table.length)-1} additional cells are scanned.  
 * When called from insertions, this parameter is the number * of elements, but when from replaceStaleEntry, it is the * table length. (Note: all this could be changed to be either * more or less aggressive by weighting n instead of just * using straight log n. But this version is simple, fast, and * seems to work well.) * * @return true if any stale entries have been removed.  
 */
private boolean cleanSomeSlots(int i, int n) {  
    boolean removed = false;  
    Entry[] tab = table;  
    int len = tab.length;  
    do {  
        i = nextIndex(i, len);  
        Entry e = tab[i];  
        if (e != null && e.refersTo(null)) {  
            n = len;  
            removed = true;  
            i = expungeStaleEntry(i);  
        }  
    } while ( (n >>>= 1) != 0);  
    return removed;  
}
```
while 循环中不断的右移进行寻找需要被清理的过期元素，最终都会使用 expungeStaleEntry 进行处理，这里还包括元素的移位。

## 5.6 移除元素
```java
// ThreadLocal
/**  
 * Removes the current thread's value for this thread-local * variable.  If this thread-local variable is subsequently * {@linkplain #get read} by the current thread, its value will be  
 * reinitialized by invoking its {@link #initialValue} method,  
 * unless its value is {@linkplain #set set} by the current thread  
 * in the interim.  This may result in multiple invocations of the * {@code initialValue} method in the current thread.  
 * * @since 1.5  
 */ 
public void remove() {  
     ThreadLocalMap m = getMap(Thread.currentThread());  
     if (m != null) {  
         m.remove(this);  
     }  
}
// ThreadLocalMap
/**  
 * Remove the entry for key. 
 */
 private void remove(ThreadLocal<?> key) {  // 从Entry数组中找到对应的key，调用expungeStaleEntry探测清理。
    Entry[] tab = table;  
    int len = tab.length;  
    int i = key.threadLocalHashCode & (len-1);  
    for (Entry e = tab[i];  
         e != null;  
         e = tab[i = nextIndex(i, len)]) {  
        if (e.refersTo(key)) {  
            e.clear();  // e.clear() 最后调用 Reference的private native void clear0()方法，
            expungeStaleEntry(i);  
            return;        
        }  
    }  
}
```
# 总结
写到这算是把 ThreadLocal 知识点的一角分析完了，在 ThreadLocal 的家族里
- 还有Netty 中用到的FastThreadLocal。在**全链路跨服务线程间**获取**调用链路**，
- 还有 TransmittableThreadLocal，
- 另外还有 JDK 本身自带的一种**线程传递解决方案** InheritableThreadLocal。

但站在本文的基础上，了解了最基础的原理，在理解其他的拓展设计，就更容易接受了。

此外在我们文中分析时经常会看到探测式清理，**其实这也是非常耗时**。为此我们在使用 ThreadLocal 一定要记得 `new ThreadLocal<>().remove();` 操作。**避免弱引用发生 GC 后，导致内存泄漏的问题**。

最后，你发现了吗！我们学习这样的底层原理性知识，都离不开数据结构和良好的设计方案，或者说是算法的身影。这些代码才是支撑整个系统良好运行的地基，如果我们可以把一些思路抽取到我们开发的核心业务流程中，也是可以大大提升性能的。


### GC后内存泄露的问题
```java
    @Test  
    public void test_gc2() throws Exception {  
        new ThreadLocal<>().set("abc");  
//        System.gc();  
        Thread t = Thread.currentThread();  
        Class<? extends Thread> clz = t.getClass();  
        Field field = clz.getDeclaredField("threadLocals");  
        field.setAccessible(true);  
        Object threadLocalMap = field.get(t);  
        Class<?> tlmClass = threadLocalMap.getClass();  
        Field tableField = tlmClass.getDeclaredField("table");  
        tableField.setAccessible(true);  
        Object[] arr = (Object[]) tableField.get(threadLocalMap);  
        for (Object o : arr) {  
            if (o != null) {  
                Class<?> entryClass = o.getClass();  
                Field valueField = entryClass.getDeclaredField("value");  
                Field referenceField = entryClass.getSuperclass().getSuperclass().getDeclaredField("referent");  
                valueField.setAccessible(true);  
                referenceField.setAccessible(true);  
                System.out.println(String.format("弱引用key:%s,值:%s", referenceField.get(o), valueField.get(o)));  
            }  
        }  
  
    }  
  
    @Test  
    public void test_gc() throws Exception {  
//        Thread t = new Thread(()->test("abc",false));  
//        t.start();  
//        t.join();  
        System.out.println("--gc后--");  
//        Thread t2 = new Thread(() -> test("def", true));  
//        t2.start();  
        //t2.join();        test("abc", false);  
        test("def", true);  
    }  
  
    private static void test(String s, boolean isGC) {  
        try {  
//            new ThreadLocal<>().set(s);  
            ThreadLocal<Object> objectThreadLocal = new ThreadLocal<>();  
            objectThreadLocal.set(s);  
            if (isGC) {  
                System.gc();  
            }  
            Thread t = Thread.currentThread();  
            Class<? extends Thread> clz = t.getClass();  
            Field field = clz.getDeclaredField("threadLocals");  
            field.setAccessible(true);  
            Object threadLocalMap = field.get(t);  
            Class<?> tlmClass = threadLocalMap.getClass();  
            Field tableField = tlmClass.getDeclaredField("table");  
            tableField.setAccessible(true);  
            Object[] arr = (Object[]) tableField.get(threadLocalMap);  
            for (Object o : arr) {  
                if (o != null) {  
                    Class<?> entryClass = o.getClass();  
                    Field valueField = entryClass.getDeclaredField("value");  
                    Field referenceField = entryClass.getSuperclass().getSuperclass().getDeclaredField("referent");  
                    valueField.setAccessible(true);  
                    referenceField.setAccessible(true);  
                    System.out.println(String.format("弱引用key:%s,值:%s", referenceField.get(o), valueField.get(o)));  
                }  
            }  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }
```