# 一、面试题
谢飞机，小记！，上次吃亏在线程上，这次可能一次坑掉两次了！
谢飞机：你问吧，我准备好了！！！
面试官：嗯，线程池状态是如何设计存储的？
谢飞机：这！下一个，下一个！
面试官：Worker 的实现类，为什么不使用 ReentrantLock 来实现呢，而是自己继承 AQS？
谢飞机：我...！
面试官：那你简述下，execute 的执行过程吧！
谢飞机：再见！

# 二、线程池讲解
## 1. 先看个例子
```java
ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(10, 10, 0L, TimeUnit
.MILLISECONDS, new ArrayBlockingQueue<>(10));

threadPoolExecutor.execute(() -> {
    System.out.println("Hi 线程池！");
});

threadPoolExecutor.shutdown();
// Executors.newFixedThreadPool(10);
// Executors.newCachedThreadPool();
// Executors.newScheduledThreadPool(10);
// Executors.newSingleThreadExecutor();
```
这是一段用于创建线程池的例子，相信你已经用了很多次了。
线程池的核心目的就是**资源的利用**，**避免重复创建线程带来的资源消耗**。（和连接池一样）。因此引入一个池化技术的思想，避免重复创建、销毁带来的性能开销。

那么，接下来我们就通过实践的方式分析下这个池子的构造，看看它是如何处理线程的。
## 2. 手写一个线程池
### 2.1 实现流程
为了更好的理解和分析关于线程池的源码，我们先来按照线程池的思想，手写一个非常简单的线程池。

其实很多时候一段功能代码的核心主逻辑可能并没有多复杂，但为了让核心流程顺利运行，就需要额外添加很多分支的辅助流程。就像我常说的，为了保护手才把擦屁屁纸弄那么大！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222221605.png)
关于图 21-1，这个手写线程池的实现也非常简单，只会体现出核心流程，包括：
1. 有 n 个一直在运行的线程，相当于**我们创建线程池时允许的线程池大小**。
2. **把线程提交给线程池运行**。
3. **如果运行线程池已满，则把线程放入队列中**。
4. 最后**当有空闲时，则获取队列中线程进行运行**。

### 2.2 实现代码
```java
public class ThreadPoolTrader implements Executor {
private final AtomicInteger ctl = new AtomicInteger(0);
private volatile int corePoolSize;
private volatile int maximumPoolSize;
private final BlockingQueue<Runnable> workQueue;
public ThreadPoolTrader(int corePoolSize, int maximumPoolSize, BlockingQueue<Ru
nnable> workQueue) {
this.corePoolSize = corePoolSize;
this.maximumPoolSize = maximumPoolSize;
this.workQueue = workQueue;
}
@Override
public void execute(Runnable command) {
int c = ctl.get();
if (c < corePoolSize) {
if (!addWorker(command)) {
reject();
}
return;
}
if (!workQueue.offer(command)) {
if (!addWorker(command)) {
reject();
}
}
}
private boolean addWorker(Runnable firstTask) {
if (ctl.get() >= maximumPoolSize) return false;
Worker worker = new Worker(firstTask);
worker.thread.start();
ctl.incrementAndGet();
return true;
}
private final class Worker implements Runnable {
final Thread thread;
Runnable firstTask;
public Worker(Runnable firstTask) {
this.thread = new Thread(this);
this.firstTask = firstTask;
}
@Override
public void run() {
Runnable task = firstTask;
try {
while (task != null || (task = getTask()) != null) {
task.run();
if (ctl.get() > maximumPoolSize) {
break;
}
task = null;
}
} finally {
ctl.decrementAndGet();
}
}
private Runnable getTask() {
for (; ; ) {
try {
System.out.println("workQueue.size：" + workQueue.size());
return workQueue.take();
} catch (InterruptedException e) {
e.printStackTrace();
}
}
}
}
private void reject() {
throw new RuntimeException("Error！ctl.count：
" + ctl.get() + " workQueue.size：" + workQueue.size());
}
public static void main(String[] args) {
ThreadPoolTrader threadPoolTrader = new ThreadPoolTrader(2, 2, new ArrayBlo
ckingQueue<Runnable>(10));
for (int i = 0; i < 10; i++) {
int finalI = i;
threadPoolTrader.execute(() -> {
try {
Thread.sleep(1500);
} catch (InterruptedException e) {
e.printStackTrace();
}
System.out.println("任务编号：" + finalI);
});
}
}
}
```
// 测试结果
任务编号：1
任务编号：0
workQueue.size：8
workQueue.size：8
任务编号：3
workQueue.size：6
任务编号：2
workQueue.size：5
任务编号：5
workQueue.size：4
任务编号：4
workQueue.size：3
任务编号：7
workQueue.size：2
任务编号：6
workQueue.size：1
任务编号：8
任务编号：9
workQueue.size：0
workQueue.size：0

以上，关于线程池的实现还是非常简单的，从测试结果上已经可以把最核心的池化思想体现出来了。主要功能逻辑包括：
- ctl，用于记录线程池中线程数量。
- corePoolSize、maximumPoolSize，用于限制线程池容量。
- workQueue，线程池队列，也就是**那些还不能被及时运行的线程**，会被装入到这个队列中。
- execute，用于提交线程，这个是通用的接口方法。在这个方法里主要实现的就是，**当前提交的线程是加入到 worker、队列还是放弃**
- addWorker，主要是类 Worker 的具体操作，**创建并执行线程**。这里还包括了 getTask() 方法，也就是**从队列中不断的获取未被执行的线程**。

好，那么以上呢，就是这个简单线程池实现的具体体现。但如果深思熟虑就会发现这里需要很多完善，比如：
- 线程池状态呢，不可能一直奔跑呀！？
- 线程池的锁呢，不会有并发问题吗？
- 线程池拒绝后的策略呢？

这些问题都没有在主流程解决，也正因为没有这些流程，所以上面的代码才更容易理解。

接下来，我们就开始分析线程池的源码，与我们实现的简单线程池参考对比，会更加容易理解😄！
## 3. 线程池源码分析
### 3.1 线程池类关系图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222315183.png)
以围绕核心类 ThreadPoolExecutor 的实现展开的类之间实现和继承关系，如图21-2 线程池类关系图。
- 接口 Executor、ExecutorService，定义线程池的基本方法。尤其是execute(Runnable command) 提交线程池方法。
- **抽象类 AbstractExecutorService，实现了基本通用的接口方法**。
- **ThreadPoolExecutor，是整个线程池最核心的工具类方法，所有的其他类和接口，为围绕这个类来提供各自的功能**。
- Worker，是任务类，也就是**最终执行的线程的方法**。
- RejectedExecutionHandler，是拒绝策略接口，有四个实现类；AbortPolicy(抛异常方式拒绝)、DiscardPolicy(直接丢弃)、DiscardOldestPolicy(丢弃存活时间最长的任务)、CallerRunsPolicy(谁提交谁执行)。
- Executors，是用于**创建我们常用的不同策略的线程池**，newFixedThreadPool、newCachedThreadPool、newScheduledThreadPool、newSingleThreadExecutor。

### 3.2 高 3 位与低 29 位、线程池状态
图 22-3 线程池状态，高 3 位与低 29 位
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222318741.png)
```java
private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
private static final int COUNT_BITS = Integer.SIZE - 3;
private static final int CAPACITY = (1 << COUNT_BITS) - 1;
private static final int RUNNING = -1 << COUNT_BITS;
private static final int SHUTDOWN = 0 << COUNT_BITS;
private static final int STOP = 1 << COUNT_BITS;
private static final int TIDYING = 2 << COUNT_BITS;
private static final int TERMINATED = 3 << COUNT_BITS;
```
**在 ThreadPoolExecutor 线程池实现类中，使用 AtomicInteger 类型的 ctl 记录线程池状态和线程池数量**。==在一个类型上记录多个值，它采用的分割数据区域==，高 3 位记录状态，低 29 位存储线程数量，默认 RUNNING 状态，线程数为 0 个。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222320558.png)
图 22-4 是线程池中的状态流转关系，包括如下状态：
- RUNNING：运行状态，接受新的任务并且处理队列中的任务。
- SHUTDOWN：关闭状态(调用了 shutdown 方法)。**不接受新任务，但是要处理队列中的任务**。
- STOP：停止状态(调用了 shutdownNow 方法)。**不接受新任务，也不处理队列中的任务，并且要中断正在处理的任务**。
- TIDYING：所有的任务都已终止了，workerCount 为 0，**线程池进入该状态后会调 terminated() 方法进入 TERMINATED 状态**。
- TERMINATED：终止状态，terminated() 方法调用**结束后的状态**。

### 3.3 提交线程(execute)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222321608.png)
```java
public void execute(Runnable command) {
    if (command == null)
    throw new NullPointerException();
    int c = ctl.get();
    if (workerCountOf(c) < corePoolSize) {
    if (addWorker(command, true))
    return;
    c = ctl.get();
    }
    if (isRunning(c) && workQueue.offer(command)) {
    int recheck = ctl.get();
    if (! isRunning(recheck) && remove(command))
    reject(command);
    else if (workerCountOf(recheck) == 0)
    addWorker(null, false);
    }
    else if (!addWorker(command, false))
reject(command);
}
```
在阅读这部分源码的时候，可以参考我们自己实现的线程池。其实最终的目的都是一样的，就是这段被提交的线程，启动执行、加入队列、决策策略，这三种方式。
- ctl.get()，取的是记录线程状态和线程个数的值，最终需要使用方法 workerCountOf()，来获取当前线程数量。`workerCountOf` 执行的是 c & CAPACITY 运算
- 根据当前线程池中线程数量，与核心线程数 corePoolSize 做对比，**小于则进行添加线程到任务执行(队列)**。
- 如果说此时（核心）线程数已满，那么则需要判断线程池是否为运行状态 isRunning(c)。**如果是运行状态则把不能被执行的线程放入工作队列中**。
- 放入工作队列以后，还需要重新判断线程池是否运行，不是运行就进行移除操作，如果非运行且移除，则进行拒绝策略。否则判断线程数量为 0 后添加新线程。
- 最后就是再次尝试添加任务执行，此时方法 addWorker 的第二个入参是 false，最终会影响添加执行任务数量判断。如果添加失败则进行拒绝策略。

### 3.5 添加执行任务(addWorker)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222336516.png)
private boolean addWorker(Runnable firstTask, boolean core)
第一部分、**增加线程数量**
```java
retry:
for (;;) {
int c = ctl.get();
int rs = runStateOf(c);
// Check if queue empty only if necessary.
if (rs >= SHUTDOWN &&
! (rs == SHUTDOWN &&
firstTask == null &&
! workQueue.isEmpty()))
return false;
for (;;) {
int wc = workerCountOf(c);
if (wc >= CAPACITY ||
wc >= (core ? corePoolSize : maximumPoolSize))
return false;
if (compareAndIncrementWorkerCount(c))
break retry;
c = ctl.get(); // Re-read ctl
if (runStateOf(c) != rs)
continue retry;
// else CAS failed due to workerCount change; retry inner loop
}
}
```
- if (rs >= SHUTDOWN &&! (rs == SHUTDOWN &&firstTask == null &&! workQueue.isEmpty()))，判断当前线程池状态，是否为SHUTDOWN、STOP、TIDYING、TERMINATED 中的一个。并且当前状态不为SHUTDOWN、或传入的任务不为 null，或队列为空，那么就返回 false

第一部分、**创建启动线程**
```java
boolean workerStarted = false;
boolean workerAdded = false;
Worker w = null;
try {
w = new Worker(firstTask);
final Thread t = w.thread;
if (t != null) {
final ReentrantLock mainLock = this.mainLock;
mainLock.lock();
try {
int rs = runStateOf(ctl.get());
if (rs < SHUTDOWN ||
(rs == SHUTDOWN && firstTask == null)) {
if (t.isAlive()) // precheck that t is startable
throw new IllegalThreadStateException();
workers.add(w);
int s = workers.size();
if (s > largestPoolSize)
largestPoolSize = s;
workerAdded = true;
}
} finally {
mainLock.unlock();
}
if (workerAdded) {
t.start();
workerStarted = true;
}
}
} finally {
if (! workerStarted)
addWorkerFailed(w);
}
return workerStarted;
```
添加执行任务的流程可以分为两块看，上面代码部分是用于记录线程数量、下面代码部分是**在独占锁里创建执行线程并启动**。这部分代码如果不看锁、CAS 等操作，那么就和我们最开始手写的线程池基本一样了。
- compareAndIncrementWorkerCount，CAS 操作，增加线程数量，成功就会跳出标记的循环体。
- runStateOf(c) != rs，最后是线程池状态判断，决定是否循环。
- 在线程池数量记录成功后，则需要进入加锁环节，创建执行线程，并记录状态。
- 最后如果判断没有启动成功，则需要执行 addWorkerFailed 方法，剔除到线程方法等操作。
### 3.6 执行线程(runWorker)
```java
final void runWorker(Worker w) {
Thread wt = Thread.currentThread();
Runnable task = w.firstTask;
w.firstTask = null;
w.unlock(); // 允许中断
boolean completedAbruptly = true;
try {
while (task != null || (task = getTask()) != null)
w.lock();
if ((runStateAtLeast(ctl.get(), STOP) ||
(Thread.interrupted() &&
runStateAtLeast(ctl.get(), STOP))) &&
!wt.isInterrupted())
wt.interrupt();
try {
beforeExecute(wt, task);
Throwable thrown = null;
try {
task.run();
} finally {
afterExecute(task, thrown);
}
} finally {
task = null;
w.completedTasks++;
w.unlock();
}
}
completedAbruptly = false;
} finally {
processWorkerExit(w, completedAbruptly);
}
}
```
其实，有了手写线程池的基础，到这也就基本了解了，线程池在干嘛。到这最核心的点就是 task.run() 让线程跑起来。额外再附带一些其他流程如下；
- beforeExecute、afterExecute，线程执行的前后做一些统计信息。
- 另外这里的锁操作是 **Worker 继承 AQS 自己实现的不可重入的独占锁**。
- processWorkerExit，如果你感兴趣，类似这样的方法也可以深入了解下。在线程退出时 workers 做到一些移除处理以及完成任务数等，也非常有意思

### 3.7 队列获取任务(getTask)
如果你已经开始阅读源码，可以在 runWorker 方法中，看到这样一句循环代码 `while (task != null || (task = getTask()) != null)` 。这与我们手写线程池中操作的方式是一样的，核心目的就是从队列中获取线程方法。
```java
private Runnable getTask() {
boolean timedOut = false; // Did the last poll() time out?
for (;;) {
int c = ctl.get();
int rs = runStateOf(c);
// Check if queue empty only if necessary.
if (rs >= SHUTDOWN && (rs >= STOP || workQueue.isEmpty())) {
decrementWorkerCount();
return null;
}
int wc = workerCountOf(c);
// Are workers subject to culling?
boolean timed = allowCoreThreadTimeOut || wc > corePoolSize;
if ((wc > maximumPoolSize || (timed && timedOut))
&& (wc > 1 || workQueue.isEmpty())) {
if (compareAndDecrementWorkerCount(c))
return null;
continue;
}
try {
Runnable r = timed ?
workQueue.poll(keepAliveTime, TimeUnit.NANOSECONDS) :
workQueue.take();
if (r != null)
return r;
timedOut = true;
} catch (InterruptedException retry) {
timedOut = false;
}
```
- getTask 方法从阻塞队列中获取等待被执行的任务，也就是一条条往出拿线程方法。
- if (rs >= SHUTDOWN ...，判断线程是否关闭。
- wc = workerCountOf(c)，wc > corePoolSize，如果工作线程数超过核心线程数量 corePoolSize 并且 workQueue 不为空，则增加工作线程。但如果超时未获取到线程，则会把大于 corePoolSize 的线程销毁掉????。
- timed，是 allowCoreThreadTimeOut 得来的。最终 timed 为 true 时，则通过阻塞队列的 poll 方法进行超时控制。
- 如果在 keepAliveTime 时间内没有获取到任务，则返回 null。如果为 false，则阻塞。

# 三、总结
这一章节并没有完全把线程池的所有知识点都介绍完，否则一篇内容会有些臃肿。

在这一章节我们从手写线程池开始，逐步的分析这些代码在 Java 的线程池中是如何实现的，涉及到的知识点也几乎是我们以前介绍过的内容，包括：队列、CAS、AQS、重入锁、独占锁等内容。所以这些知识也基本是环环相扣的，最好有一些根
基否则会有些不好理解。

除了本章介绍的，我们还没有讲到**线程的销毁过程、四种线程池方法的选择和使用**、以及在 CPU 密集型任务、IO 密集型任务时该怎么配置。另外在 Spring 中也有自己实现的线程池方法。这些知识点都非常贴近实际操作。

---
> 是不你总买五常大米，其实五常和榆树是挨着的，榆树大米也好吃，榆树还是天下第一粮仓呢！但是五常出名，所以只认识五常。

为什么提这个呢，**因为阿里不允许使用 Executors 创建线程池**！其他很多大厂也不允许，这么创建的话，控制不好会出现 OOM。

好，本篇就带你学习四种线程池的不同使用方式、业务场景应用以及如何监控线程。
# 一、面试题
谢飞机，小记！，上次从面试官那逃跑后，恶补了多线程，自己好像也内卷了，所以出门逛逛！
面试官：嗨，飞机，飞机，这边！
谢飞机：嗯？！哎呀，面试官你咋来南海子公园了？
面试官：我家就附近，跑步来了。最近你咋样，上次问你的多线程学了吗？
谢飞机：哎，看了是看了，记不住鸭！
面试官：嗯，不常用确实记不住。不过你可以选择跳槽，来大厂，大厂的业务体量较大！
谢飞机：我就纠结呢，想回家考教师资格证了，我们村小学要教 java 了！
面试官：哈哈哈哈哈，一起！
# 二、四种线程池使用介绍
Executors 是创建线程池的工具类，比较典型常见的四种线程池包括：
`newFixedThreadPool 、 newSingleThreadExecutor 、 newCachedThreadPool 、newScheduledThreadPool`。每一种都有自己特定的典型例子，可以按照每种的特
性用在不同的业务场景，也可以作为参照精细化创建线程池。
## 1. `newFixedThreadPool`
```java
public static void main(String[] args) {
ExecutorService executorService = Executors.newFixedThreadPool(3);
for (int i = 1; i < 5; i++) {
int groupId = i;
executorService.execute(() -> {
for (int j = 1; j < 5; j++) {
try {
Thread.sleep(1000);
} catch (InterruptedException ignored) {
}
logger.info("第 {} 组任务，第 {} 次执行完成", groupId, j);
}
});
}
executorService.shutdown();
}
```
// 测试结果
23:48:24.628 [pool-2-thread-1] INFO o.i.i.test.Test_newFixedThreadPool - 第 1 组任
务，第 1 次执行完成
23:48:24.628 [pool-2-thread-2] INFO o.i.i.test.Test_newFixedThreadPool - 第 2 组任
务，第 1 次执行完成
23:48:24.628 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 3 组任
务，第 1 次执行完成
23:48:25.633 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 3 组任
务，第 2 次执行完成
23:48:25.633 [pool-2-thread-1] INFO o.i.i.test.Test_newFixedThreadPool - 第 1 组任
务，第 2 次执行完成
23:48:25.633 [pool-2-thread-2] INFO o.i.i.test.Test_newFixedThreadPool - 第 2 组任
务，第 2 次执行完成
23:48:26.633 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 3 组任
务，第 3 次执行完成
23:48:26.633 [pool-2-thread-2] INFO o.i.i.test.Test_newFixedThreadPool - 第 2 组任
务，第 3 次执行完成
23:48:26.633 [pool-2-thread-1] INFO o.i.i.test.Test_newFixedThreadPool - 第 1 组任
务，第 3 次执行完成
23:48:27.634 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 3 组任
务，第 4 次执行完成
23:48:27.634 [pool-2-thread-2] INFO o.i.i.test.Test_newFixedThreadPool - 第 2 组任
务，第 4 次执行完成
23:48:27.634 [pool-2-thread-1] INFO o.i.i.test.Test_newFixedThreadPool - 第 1 组任
务，第 4 次执行完成
23:48:28.635 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 4 组任
务，第 1 次执行完成
23:48:29.635 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 4 组任
务，第 2 次执行完成
23:48:30.635 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 4 组任
务，第 3 次执行完成
23:48:31.636 [pool-2-thread-3] INFO o.i.i.test.Test_newFixedThreadPool - 第 4 组任
务，第 4 次执行完成

### 图解
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305222355719.png)
- 代码：`new ThreadPoolExecutor(nThreads, nThreads, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>())`

- 介绍：创建一个固定大小可重复使用的线程池，以 LinkedBlockingQueue 无界阻塞队列存放等待线程。

- 风险：随着线程任务不能被执行的的无限堆积，可能会导致 OOM。

### 2. `newSingleThreadExecutor`
```java
public static void main(String[] args) {
ExecutorService executorService = Executors.newSingleThreadExecutor();
for (int i = 1; i < 5; i++) {
int groupId = i;
executorService.execute(() -> {
for (int j = 1; j < 5; j++) {
try {
Thread.sleep(1000);
} catch (InterruptedException ignored) {
}
logger.info("第 {} 组任务，第 {} 次执行完成", groupId, j);
}
});
}
executorService.shutdown();
}
```
// 测试结果
23:20:15.066 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 1 组
任务，第 1 次执行完成
23:20:16.069 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 1 组
任务，第 2 次执行完成
23:20:17.070 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 1 组
任务，第 3 次执行完成
23:20:18.070 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 1 组
任务，第 4 次执行完成
23:20:19.071 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 2 组
任务，第 1 次执行完成
23:23:280.071 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 2 组
任务，第 2 次执行完成
23:23:281.072 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 2 组
任务，第 3 次执行完成
23:23:282.072 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 2 组
任务，第 4 次执行完成
23:23:283.073 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 3 组
任务，第 1 次执行完成
23:23:284.074 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 3 组
任务，第 2 次执行完成
23:23:285.074 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 3 组
任务，第 3 次执行完成
23:23:286.075 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 3 组
任务，第 4 次执行完成
23:23:287.075 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 4 组
任务，第 1 次执行完成
23:23:288.075 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 4 组
任务，第 2 次执行完成
23:23:289.076 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 4 组
任务，第 3 次执行完成
23:20:30.076 [pool-2-thread-1] INFO o.i.i.t.Test_newSingleThreadExecutor - 第 4 组
任务，第 4 次执行完成

代码：`new ThreadPoolExecutor(1, 1, 0L, TimeUnit.MILLISECONDS, new LinkedBlockingQueue<Runnable>())`

介绍：**只创建一个执行线程任务的线程池，如果出现意外终止则再创建一个**。

风险：同样这也是一个无界队列存放待执行线程，无限堆积下会出现 OOM。

### 3. `newCachedThreadPool`
```java
public static void main(String[] args) throws InterruptedException {
ExecutorService executorService = Executors.newCachedThreadPool();
for (int i = 1; i < 5; i++) {
int groupId = i;
executorService.execute(() -> {
for (int j = 1; j < 5; j++) {
try {
Thread.sleep(1000);
} catch (InterruptedException ignored) {
}
logger.info("第 {} 组任务，第 {} 次执行完成", groupId, j);
}
});
}
executorService.shutdown();
```
// 测试结果
23:25:59.818 [pool-2-thread-
2] INFO o.i.i.test.Test_newCachedThreadPool - 第 2 组任务，第 1 次执行完成
23:25:59.818 [pool-2-thread-
3] INFO o.i.i.test.Test_newCachedThreadPool - 第 3 组任务，第 1 次执行完成
23:25:59.818 [pool-2-thread-
1] INFO o.i.i.test.Test_newCachedThreadPool - 第 1 组任务，第 1 次执行完成
23:25:59.818 [pool-2-thread-
4] INFO o.i.i.test.Test_newCachedThreadPool - 第 4 组任务，第 1 次执行完成
23:25:00.823 [pool-2-thread-
4] INFO o.i.i.test.Test_newCachedThreadPool - 第 4 组任务，第 2 次执行完成
23:25:00.823 [pool-2-thread-
1] INFO o.i.i.test.Test_newCachedThreadPool - 第 1 组任务，第 2 次执行完成
23:25:00.823 [pool-2-thread-
2] INFO o.i.i.test.Test_newCachedThreadPool - 第 2 组任务，第 2 次执行完成
23:25:00.823 [pool-2-thread-
3] INFO o.i.i.test.Test_newCachedThreadPool - 第 3 组任务，第 2 次执行完成
23:25:01.823 [pool-2-thread-
4] INFO o.i.i.test.Test_newCachedThreadPool - 第 4 组任务，第 3 次执行完成
23:25:01.823 [pool-2-thread-
1] INFO o.i.i.test.Test_newCachedThreadPool - 第 1 组任务，第 3 次执行完成
23:25:01.824 [pool-2-thread-
2] INFO o.i.i.test.Test_newCachedThreadPool - 第 2 组任务，第 3 次执行完成
23:25:01.824 [pool-2-thread-
3] INFO o.i.i.test.Test_newCachedThreadPool - 第 3 组任务，第 3 次执行完成
23:25:02.824 [pool-2-thread-
1] INFO o.i.i.test.Test_newCachedThreadPool - 第 1 组任务，第 4 次执行完成
23:25:02.824 [pool-2-thread-
4] INFO o.i.i.test.Test_newCachedThreadPool - 第 4 组任务，第 4 次执行完成
23:25:02.825 [pool-2-thread-
3] INFO o.i.i.test.Test_newCachedThreadPool - 第 3 组任务，第 4 次执行完成
23:25:02.825 [pool-2-thread-
2] INFO o.i.i.test.Test_newCachedThreadPool - 第 2 组任务，第 4 次执行完成
}

代码：`new ThreadPoolExecutor(0, Integer.MAX_VALUE, 60L, TimeUnit.SECONDS, new SynchronousQueue<Runnable>())`
介绍：首先 SynchronousQueue 是一个生产消费模式的阻塞任务队列，只要有任务就需要有线程执行，线程池中的线程可以重复使用。
风险：如果线程任务比较耗时，又大量创建，会导致 OOM

### 4. `newScheduledThreadPool`
```java
public static void main(String[] args) {
ScheduledExecutorService executorService = Executors.newScheduledThreadPool(1);
executorService.schedule(() -> {
logger.info("3 秒后开始执行");
}, 3, TimeUnit.SECONDS);
executorService.scheduleAtFixedRate(() -> {
logger.info("3 秒后开始执行，以后每 2 秒执行一次");
}, 3, 2, TimeUnit.SECONDS);
executorService.scheduleWithFixedDelay(() -> {
logger.info("3 秒后开始执行，后续延迟 2 秒");
}, 3, 2, TimeUnit.SECONDS);
}
```
// 测试结果
23:28:32.442 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行
23:28:32.444 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行，以后每 2 秒执行一次
23:28:32.444 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行，后续延迟 2 秒
23:28:34.441 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行，以后每 2 秒执行一次
23:28:34.445 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行，后续延迟 2 秒
23:28:36.440 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行，以后每 2 秒执行一次
23:28:36.445 [pool-2-thread-1] INFO o.i.i.t.Test_newScheduledThreadPool - 3 秒后开始
执行，后续延迟 2 秒


- 代码：`public ScheduledThreadPoolExecutor(int corePoolSize) { super(corePoolSize, Integer.MAX_VALUE, 0, NANOSECONDS, new ScheduledThreadPoolExecutor.DelayedWorkQueue()); }`
- 介绍：这就是一个比较有意思的线程池了，它可以延迟定时执行，有点像我们的定时任务。同样它也是一个无限大小的线程池 Integer.MAX_VALUE。它提供的调用方法比较多，包括：scheduleAtFixedRate、scheduleWithFixedDelay，可以按需选择延迟执行方式。
- 风险：同样由于这是一组无限容量的线程池，所以依旧有 OOM 风险。
# 三、线程池使用场景说明
什么时候使用线程池？

说简单是当为了给老板省钱的时候，因为**使用线程池可以降低服务器资源的投入，让每台机器尽可能更大限度的使用 CPU**。
😄那你这么说肯定没办法升职加薪了！
所以如果说的高大上一点，那么是在**符合科特尔法则和阿姆达尔定律** 的情况下，引入线程池的使用最为合理。啥意思呢，还得简单说！
假如：我们有一套电商服务，用户浏览商品的并发访问速率是：1000 客户/每分钟，平均每个客户在服务器上的耗时 0.5 分钟。根据利特尔法则，在任何时刻，服务端都承担着 `1000*0.5=500` 个客户的业务处理量。过段时间大促了，并发访问的用户扩了一倍 2000 客户了，那怎么保障服务性能呢？
1. 提高服务器并发处理的业务量，即提高到 `2000×0.5=1000`
2. 减少服务器平均处理客户请求的时间，即减少到：`2000×0.25=500`

所以：在有些场景下会把串行的请求接口，压缩成并行执行，如图 22-5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305230002822.png)
但是，线程池的使用会随着业务场景变化而不同，如果你的业务需要大量的使线程池，并非常依赖线程池，那么就不可能用 Executors 工具类中提供的方法。

因为这些线程池的创建都不够精细化，也非常容易造成 OOM 风险，而且随着业务场景逻辑不同，会有 IO 密集型和 CPU 密集型。

最终，大家使用的线程池都是使用 new ThreadPoolExecutor() 创建的，当然也有基于 Spring 的线程池配置
org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor。

可你想过吗，同样一个接口在有活动时候怎么办、有大促时候怎么办，可能你当时设置的线程池是合理的，但是==一到流量非常大的时候就很不适合了，所以如果能动态调整线程池就非常有必要了==。而且**使用 new ThreadPoolExecutor() 方式创建的线程池是可以通过提供的 set 方法进行动态调整的**。有了这个动态调整的方法后，就可以把线程池包装起来，在配合动态调整的页面，动态更新线程池参数，就可以非常方便的调整线程池了。

# 四、获取线程池监控信息
你收过报警短信吗？
收过，半夜还有报警机器人打电话呢！崴，你的系统有个机器睡着了，快起来看看！！！
所以，如果你高频、高依赖线程池，那么有一个完整的监控系统，就非重要了。总不能线上挂了，你还不知道！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305230004101.png)
1. 重写线程池方式监控
如果我们想监控一个线程池的方法执行动作，最简单的方式就是继承这个类，重写方法，在方法中添加动作收集信息。

伪代码：
```java
public class ThreadPoolMonitor extends ThreadPoolExecutor {
@Override
public void shutdown() {
// 统计已执行任务、正在执行任务、未执行任务数量
super.shutdown();
}
@Override
public List<Runnable> shutdownNow() {
// 统计已执行任务、正在执行任务、未执行任务数量
return super.shutdownNow();
}
@Override
protected void beforeExecute(Thread t, Runnable r) {
// 记录开始时间
}
@Override
protected void afterExecute(Runnable r, Throwable t) {
// 记录完成耗时
}
...
}
```
### 2. 基于 JVMTI 方式监控
这块是监控的重点，因为我们**不太可能让每一个需要监控的线程池都来重写的方式记录**，这样的改造成本太高了。

那么除了这个笨方法外，可以选择使用基于 JVMTI 的方式，进行开发监控组件。
JVMTI：JVMTI(JVM Tool Interface)位于 jpda 最底层，是 **Java 虚拟机所提供的 native 编程接口**。JVMTI 可以提供性能分析、debug、内存管理、线程分析等功能。

基于jvmti提供的接口服务，运用 C++代码(win32-add_library)在Agent_OnLoad 里开发监控服务，并生成 dll 文件。开发完成后在 java 代码中加入 agentpath，这样就可以监控到我们需要的信息内容。
环境准备：
1. Dev-C++
2. JetBrains CLion 2018.2.3
3. IntelliJ IDEA Community Edition 2018.3.1 x64
4. jdk1.8.0_45 64 位
5. jvmti（在 jdk 安装目录下 `jdk1.8.0_45\include` 里，把 include 整个文件夹复制到和工程案例同层级目录下，便于 include 引用）

配置信息：（路径相关修改为自己的）
1. C++开发工具 Clion 配置 
    1. 配置位置；Settings->Build,Execution,Deployment->Toolchains
    2. MinGW 配置：D:\Program Files (x86)\Dev-Cpp\MinGW64

2. java 调试时配置
    1. 配置位置：Run/Debug Configurations ->VM options
    2. 配置内容：-agentpath:E:\itstack\git\github.com\itstack-jvmti\cmake-build-debug\libitstack_jvmti.dll

### 2.1 先做一个监控例子
Java 工程
```java
public class TestLocationException {
public static void main(String[] args) {
Logger logger = Logger.getLogger("TestLocationException");
try {
PartnerEggResourceImpl resource = new PartnerEggResourceImpl();
Object obj = resource.queryUserInfoById(null);
logger.info("测试结果：" + obj);
} catch (Exception e) {
//屏蔽异常
}
}
}
class PartnerEggResourceImpl {
Logger logger = Logger.getLogger("PartnerEggResourceImpl");
public Object queryUserInfoById(String userId) {
logger.info("根据用户 Id 获取用户信息" + userId);
if (null == userId) {
throw new NullPointerException("根据用户 Id 获取用户信息，空指针异常");
}
return userId;
}
}
```
c++监控
```java
#include <iostream>
#include <cstring>
#include "jvmti.h"
using namespace std;
//异常回调函数
static void JNICALL
callbackException(jvmtiEnv *jvmti_env, JNIEnv *env, jthread thr, jmethodID methodId
, jlocation location,
jobject exception, jmethodID catch_method, jlocation catch_location) {
// 获得方法对应的类
jclass clazz;
jvmti_env->GetMethodDeclaringClass(methodId, &clazz);
// 获得类的签名
char *class_signature;
jvmti_env->GetClassSignature(clazz, &class_signature, nullptr);
//过滤非本工程类信息
string::size_type idx;
string class_signature_str = class_signature;
idx = class_signature_str.find("org/itstack");
if (idx != 1) {
return;
}
//异常类名称
char *exception_class_name;
jclass exception_class = env->GetObjectClass(exception);
jvmti_env->GetClassSignature(exception_class, &exception_class_name, nullptr);
// 获得方法名称
char *method_name_ptr, *method_signature_ptr;
jvmti_env->GetMethodName(methodId, &method_name_ptr, &method_signature_ptr, nullptr
);
//获取目标方法的起止地址和结束地址
jlocation start_location_ptr; //方法的起始位置
jlocation end_location_ptr; //用于方法的结束位置
jvmti_env->GetMethodLocation(methodId, &start_location_ptr, &end_location_ptr);
//输出测试结果
cout << "测试结果 - 定位类的签名：" << class_signature << endl;
cout << "测试结果 - 定位方法信息：
" << method_name_ptr << " -> " << method_signature_ptr << endl;
cout << "测试结果 - 定位方法位置：
" << start_location_ptr << " -> " << end_location_ptr + 1 << endl;
cout << "测试结果 - 异常类的名称：" << exception_class_name << endl;
cout << "测试结果-输出异常信息(可以分析行号)：" << endl;
jclass throwable_class = (*env).FindClass("java/lang/Throwable");
jmethodID print_method = (*env).GetMethodID(throwable_class, "printStackTrace", "()
V");
(*env).CallVoidMethod(exception, print_method);
}
JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
jvmtiEnv *gb_jvmti = nullptr;
//初始化
vm->GetEnv(reinterpret_cast<void **>(&gb_jvmti), JVMTI_VERSION_1_0);
// 创建一个新的环境
jvmtiCapabilities caps;
memset(&caps, 0, sizeof(caps));
caps.can_signal_thread = 1;
caps.can_get_owned_monitor_info = 1;
caps.can_generate_method_entry_events = 1;
caps.can_generate_exception_events = 1;
caps.can_generate_vm_object_alloc_events = 1;
caps.can_tag_objects = 1;
// 设置当前环境
gb_jvmti->AddCapabilities(&caps);
// 创建一个新的回调函数
jvmtiEventCallbacks callbacks;
memset(&callbacks, 0, sizeof(callbacks));
//异常回调
callbacks.Exception = &callbackException;
// 设置回调函数
gb_jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
// 开启事件监听(JVMTI_EVENT_EXCEPTION)
gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, nullptr
);
return JNI_OK;
}
JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm) {
}
```
测试结果
在 VM vptions 中 配 置 ： -agentpath:E:\itstack\git\github.com\itstack-
jvmti\cmake-build-debug\libitstack_jvmti.dll
十二月 16, 2020 23:53:27 下
午 org.itstack.demo.PartnerEggResourceImpl queryUserInfoById
信息: 根据用户 Id 获取用户信息 null
java.lang.NullPointerException: 根据用户 Id 获取用户信息，空指针异常
at org.itstack.demo.PartnerEggResourceImpl.queryUserInfoById(TestLocationException
.java:26)
at org.itstack.demo.TestLocationException.main(TestLocationException.java:13)
测试结果-定位类的签名：Lorg/itstack/demo/PartnerEggResourceImpl;
测试结果-定位方法信息：queryUserInfoById -> (Ljava/lang/String;)Ljava/lang/Object;
测试结果-定位方法位置：0 -> 43
测试结果-异常类的名称：Ljava/lang/NullPointerException;
测试结果-输出异常信息(可以分析行号)：

这就是基于 JVMTI 的方式进行监控，**这样的方式可以做到非入侵代码。不需要硬编码，也就节省了人力**，否则所有人都会进行开发监控内容，而这部分内容与业务逻辑并无关系。
### 2.2 扩展线程监控
其实方法差不多，都是基于 C++开发 DLL 文件，引入使用。不过这部分代码会监控方法信息，并采集线程的执行内容。
```java
static void JNICALL callbackMethodEntry(jvmtiEnv *jvmti_env, JNIEnv *env, jthread t
hr, jmethodID method) {
// 获得方法对应的类
jclass clazz;
jvmti_env->GetMethodDeclaringClass(method, &clazz);
// 获得类的签名
char *class_signature;
jvmti_env->GetClassSignature(clazz, &class_signature, nullptr);
//过滤非本工程类信息
string::size_type idx;
string class_signature_str = class_signature;
idx = class_signature_str.find("org/itstack");
gb_jvmti->RawMonitorEnter(gb_lock);
{
//must be deallocate
char *name = NULL, *sig = NULL, *gsig = NULL;
jint thr_hash_code = 0;
error = gb_jvmti->GetMethodName(method, &name, &sig, &gsig);
error = gb_jvmti->GetObjectHashCode(thr, &thr_hash_code);
if (strcmp(name, "start") == 0 || strcmp(name, "interrupt") == 0 ||
strcmp(name, "join") == 0 || strcmp(name, "stop") == 0 ||
strcmp(name, "suspend") == 0 || strcmp(name, "resume") == 0) {
//must be deallocate
jobject thd_ptr = NULL;
jint hash_code = 0;
gb_jvmti->GetLocalObject(thr, 0, 0, &thd_ptr);
gb_jvmti->GetObjectHashCode(thd_ptr, &hash_code);
printf("[线程监控]: thread (%10d) %10s (%10d)\n", thr_hash_code, name, hash_code);
}
}
gb_jvmti->RawMonitorExit(gb_lock);
}
JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *jvm, char *options, void *reserved) {
// 初始化
jvm->GetEnv((void **) &gb_jvmti, JVMTI_VERSION_1_0);
// 创建一个新的环境
memset(&gb_capa, 0, sizeof(jvmtiCapabilities));
gb_capa.can_signal_thread = 1;
gb_capa.can_get_owned_monitor_info = 1;
gb_capa.can_generate_method_exit_events = 1;
gb_capa.can_generate_method_entry_events = 1;
gb_capa.can_generate_exception_events = 1;
gb_capa.can_generate_vm_object_alloc_events = 1;
gb_capa.can_tag_objects = 1;
gb_capa.can_generate_all_class_hook_events = 1;
gb_capa.can_generate_native_method_bind_events = 1;
gb_capa.can_access_local_variables = 1;
gb_capa.can_get_monitor_info = 1;
// 设置当前环境
gb_jvmti->AddCapabilities(&gb_capa);
// 创建一个新的回调函数
jvmtiEventCallbacks callbacks;
memset(&callbacks, 0, sizeof(jvmtiEventCallbacks));
// 方法回调
callbacks.MethodEntry = &callbackMethodEntry;
// 设置回调函数
gb_jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
gb_jvmti->CreateRawMonitor("XFG", &gb_lock);
// 注册事件监听(JVMTI_EVENT_VM_INIT、JVMTI_EVENT_EXCEPTION、
JVMTI_EVENT_NATIVE_METHOD_BIND、JVMTI_EVENT_CLASS_FILE_LOAD_HOOK、
JVMTI_EVENT_METHOD_ENTRY、JVMTI_EVENT_METHOD_EXIT)
error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, (
jthread) NULL);
error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION,
(jthread) NULL);
error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_NATIVE_MET
HOD_BIND, (jthread) NULL);
error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE
_LOAD_HOOK, (jthread) NULL);
error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENT
RY, (jthread) NULL);
error = gb_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_EXI
T, (jthread) NULL);
return JNI_OK;
}
```
从监控的代码可以看到，这里有线程的 start、stop、join、interrupt 等，并可以记录执行信息。

另外这里监控的方法执行回调，SetEventCallbacks(&callbacks,
sizeof(callbacks)); 以及相应事件的添加。

# 五、总结
如果说你所经历的业务体量很小，那么几乎并不需要如此复杂的技术栈深度学习，甚至几乎不需要扩展各类功能，也不需要监控。但终究有一些需要造飞机的大厂，他们的业务体量庞大，并发数高，让原本可能就是一个简单的查询接口，也要做熔断、降级、限流、缓存、线程、异步、预热等等操作。

知其然才敢用，如果对一个技术点不是太熟悉，就不要胡乱使用，否则遇到的OOM 并不是那么好复现，尤其是在并发场景下。当然如果你们技术体系中有各种服务，比如流量复现、链路追踪等等，那么还好。