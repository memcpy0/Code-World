### 1. 线程池7个参数
1. `corePoolSize` 核心线程数目 - 池中会保留的最多线程数
2. `maximumPoolSize` 最大线程数目 = 核心线程（执行完任务后需要保留在线程池）+救急线程（执行完任务后不需要保留在线程池）的最大数目
3. `keepAliveTime` 生存时间 - **救急线程的生存时间，生存时间内没有新任务，此线程资源会释放**
4. `unit` 时间单位 - **救急线程的生存时间单位**，如秒、毫秒等
5. `workQueue` - ==当没有空闲核心线程时，又来了新来任务，会加入到此队列排队，队列满会创建救急线程执行任务==
6. `threadFactory` 线程工厂 - **可以定制线程对象的创建**，例如设置线程名字、是否是守护线程等
7. `handler` 拒绝策略。当所有线程都在繁忙，`workQueue` 也放满时，会触发拒绝策略
	1. 抛出异常。java.util.concurrent.ThreadPoolExecutor.`AbortPolicy`
	2. 由调用者执行任务。java.util.concurrent.ThreadPoolExecutor.`CallerRunsPolicy`
	3. 丢弃任务。java.util.concurrent.ThreadPoolExecutor.`DiscardPolicy`
	4. 丢弃最早排队任务 java.util.concurrent.ThreadPoolExecutor.`DiscardOldestPolicy`
	   ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241206160.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241225958.png)
> ***代码说明***
>day02.TestThreadPoolExecutor 以较为形象的方式演示了线程池的核心组成


![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241221088.png)
任务队列演示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241222986.png)
这里队满了，任务5插队了（因为队列满了  如果不执行后到的  就可能造成任务消息丢失），它先被救急队列执行了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241223456.png)
拒绝策略：AbortPolicy
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241227095.png)
CallerRunsPolicy：这里是主线程调用的，主线程执行！

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241228873.png)
DiscardPolicy：任务六被抛弃
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303241229235.png)
DiscardOldestPolicy：把队列中最老的任务抛弃掉，然后新任务进去

### 2. 提供的线程池
new**Cached**ThreadPool()
new**Fixed**ThreadPool(int nThreads)
new**Single**ThreadExecutor()
newSingleThreadScheduledExecutor() 和 newScheduledThreadPool(int corePoolSize)
newWorkStealingPool(int parallelism)

### 3. 线程池中的几个参数，比如核心线程数、最大线程数，如果让你定，你会怎么样去设定这些值？
一般多线程执行的任务类型可以分为 CPU 密集型和 I/O 密集型（阻塞多），根据不同的任务类型，我们计算线程数的方法也不一样。假设 **N 为 CPU 核心数**，WT 为线程等待时间，ST 为线程运行时间
- CPU 密集型任务：主要消耗 CPU 资源，可以额外设置一个线程，一旦任务暂停，CPU 就会处于空闲状态，额外的线程可以充分利用 CPU 的空闲时间，线程数为 N+1
- I/O 密集型任务： I/O 交互的处理消耗较大，而线程在处理 I/O 时不会占用 CPU 来处理，因此可以多配置一些线程，线程数 2N。

在一般的业务应用场景中，我们可以用下列公式计算线程数：线程数 = N * (1 + WT / ST)

我们可以根据实际业务场景，从 “N+1” 和 “2N” 中选出一个适合的，计算出一个大概的线程数，之后通过实际压测进行增大或减小线程数调整，然后观察具体的运行时间变化，最终确定一个具体的线程数。 


