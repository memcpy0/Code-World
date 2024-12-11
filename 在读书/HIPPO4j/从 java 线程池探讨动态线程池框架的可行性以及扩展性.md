当你们在网上看到一张图片，图片上是一个性感的美女时，你们会说什么？不用想肯定也是一些"一分钟之内，给我关于这个女人所有信息"诸如此类的话；当然，我肯定不会说这样的话，原因有三个：第一我只爱我媳妇，对其他我不感兴趣的女人都不感兴趣；第二，一般来说也没人能给我提供这个女人的信息；第三，就算提供了信息，我也不知道信息的真假。如果信息是真的，要是被我媳妇发现我搜集其他女人的信息后，我下场很惨；如果信息是假的，我却信以为真，就会显得我很蠢；所以，我对这种事完全不感兴趣。可在座的诸位就不一样了，凭我在内部群中对大家的了解，我相信有些朋友对这种事情兴趣十分强烈。我在此提到这种事并不是要批判或者指责大家什么，我也不是什么自命清高的人，我的意思很简单，与其把事件花费在网络世界中，倒不如多关注点实际的事情。比如说，你可能不会在一分钟之内得到一个美女的全部信息，但是只要你技术过硬，你完全可以在一秒中之内，得到某个线程池运行的全部信息。当然，前提是你技术必须过硬，就比如说我技术非常硬，读到这里，也许有的朋友会审视一下自己，觉得硬不过我，有点灰心。朋友，如果你被我说中了，请别灰心，继续往下看，本套课程就是专门为你量身定做的，教你如何让自己硬起来！在技术层面硬起来！

在正式开始本章内容之前，请允许我再多说几句，简单讲评一下本门课程。如果难度分为 5 个星级，最高难度为 5 星，这动态线程池框架的课程难度应该在 1 星，比 xxl-job 还要简单。我这么说并不是想博人眼球，而是这个 hippo4j 框架确实非常简单。随着课程的进展，大家就会赞同我这个观点了；除此之外我还想强调一点，那就是本套课程并不设计线程池的具体使用，因为这是每个业务程序员的事，如何使用线程池，创建几个线程池，让线程池执行什么任务，这些业务逻辑并不在本套课程的讲解范围之内，我也无法讲解，业务种类繁杂，我自己都搞不定，更别说给大家讲解了。这样一来，本套课程专注的内容就非常简单了。**无非就是收集线程池运行时的信息，根据这些信息判断线程池的配置是否需要更改，如果需要动态更改线程池配置，怎么执行这个操作**。当然，在课程的第一章还没必要全面探讨这些内容，还是让我们先来明确一下本套课程的最终目标吧。

我们这套课程的最终目的是希望为大家实现一个动态线程池框架，所谓动态线程池，简而言之就是可以根据线程池运行时信息动态更新线程池的配置参数。假如我们在项目中创建了一个线程池，把这个线程池的核心线程数量设置为了 100，最大线程数设置为了 200，并且也采取了某些手段一直收集这个线程池的工作信息。观察了一段时间之后发现这个线程池执行的任务非常少，为了避免线程资源被浪费，于是我们打算在不终止程序的情况下，把这个线程池的配置信息动态更新一下，把线程池的核心线程数量更新为 10，最大线程数量更新为 20。如果我们在程序中使用了动态线程池框架，当然就可以很顺利地执行线程池配置信息动态变更的操作。但现在我们还没有这个框架，所以就要再等等才能实现这个功能。不过从我刚才举的例子中，大家应该也能看出来，对于动态线程池框架来说，最重要的功能有两个：**一个就是收集线程池工作过程中的信息，我通常把这个称为线程池运行时信息；第二个功能就是根据收集到的信息来动态更新线程池的配置参数**。只要满足了这两个功能，一个动态线程池框架就算搞定了，如果你实现的这个框架还包含其他的功能，比如通知报警等等功能，那么我只会由衷地说一句你的技术比我还硬！接下来，就让我们把目光集中在刚才的两个功能上，请大家想一想，假如我们在业务中使用了一个线程池，**要怎么收集这个线程池的运行时信息呢？或者说要收集线程池的什么运行时信息呢？又该怎么根据这些运行时信息动态更新线程池配置参数呢**？

## 线程池的更新与运行信息收集的方式

很好，速度不慢，现在终于开始步入正题了，我能想到，对一些没有经验的朋友来说，面对上面的几个问题肯定没有头绪。**怎么收集一个线程池的运行时信息？或者说要收集线程池的什么运行时信息呢？又该怎么根据这些运行时信息动态更新线程池配置参数**？就我自己而言，我在研究一些新的知识，或者说框架的时候，很喜欢把复杂问题简单化。就比如说刚才的问题，当一个线程池工作的时候，如果我不知道首先要收集这个线程池的什么信息，我就会思考这个线程池有什么信息可以被我收集；如果我不知道线程池的配置信息该怎么动态修改，我就会先思考在常规情境下，一个线程池的配置参数应该怎么修改。这时候问题就很简单了，jdk 的线程池就是 ThreadPoolExecutor，而 ThreadPoolExecutor 对外给用户暴露了很多 get、set 方法，**get 方法可以让我直接获得一些线程池的运行时信息，比如线程池过去完成任务的总数量，线程池目前活跃线程数量等等；而 set 方法就可以直接让我设置线程池的核心配置参数，如果我要修改线程池的核心线程数，我就可以直接调用 setCorePoolSize() 方法来执行修改操作**。由此可见，这些方法已经成为我实现动态线程池框架的突破口了。当然，也许大家对 ThreadPoolExecutor 类没什么清晰的印象了，接下来，还是先让我来帮助大家一起回顾一下，ThreadPoolExecutor 类的内容，然后再继续分析我刚才提出来的几个问题。请看下面代码块。

下面代码块展示的只是 ThreadPoolExecutor 类的部分内容。

```
public class ThreadPoolExecutor extends AbstractExecutorService {

    //这个成员变量非常重要，线程池就是用它来计算当前线程池的运行状态
    //以及工作线程数量，具体的位运算的逻辑，我在netty的文章第十二章中简单讲解了一下，不清楚的可以去看看netty对应的文章
    private final AtomicInteger ctl = new AtomicInteger(ctlOf(RUNNING, 0));
    
    //计算线程池运行状态的掩码
    private static final int COUNT_BITS = Integer.SIZE - 3;
    private static final int COUNT_MASK = (1 << COUNT_BITS) - 1; 
    
    //线程池状态，这几种状态，除了RUNNING，剩下的四种状态的区别要仔细梳理一下
    //正常工作状态
    private static final int RUNNING = -1 << COUNT_BITS;
    //停止工作状态，在这个状态下，线程池不会再接收新的任务，但是旧的任务仍会继续处理
    //调用shutdown()方法后线程池会进入此状态
    private static final int SHUTDOWN =  0 << COUNT_BITS;
    //立即停止状态，这个状态下的线程池既不会接收新任务，也不会执行未完成的任务，并且还会为工作线程设置中断标记
    //调用shutdownNow()方法后，线程池会进入此状态，具体逻辑和操作请看源码
    private static final int STOP =  1 << COUNT_BITS;
    //终止状态，该状态下的线程池所有线程都已经停止工作，进入此状态后的线程池会执行terminated()
    //方法，该方法可以由用户做额外的拓展操作
    private static final int TIDYING =  2 << COUNT_BITS; 
    //这个就是线程池的最终停止状态，在执行完terminated()方法之后，线程池就会进入最终状态
    private static final int TERMINATED = 3 << COUNT_BITS;




    //接下来就是线程池的一些重要的成员变量

    //这个成员变量其实就相当于真正意义上的线程池
    //因为Worker中定义了Thread成员变量，可以说一个Worker就对应着一个线程，线程池每创建
    //一个Worker对象就意味着创建了一个新的线程，并且会把新创建的Worker对象添加到workerPool中
    private final HashSet<Worker> workerPool = new HashSet<>();

    //任务队列
    private final BlockingQueue<Runnable> workQueue;

    //创建线程的工厂
    private volatile ThreadFactory threadFactory;

    //拒绝策略处理器
    private volatile RejectedExecutionHandler handler;

    //这个成员变量就是用来判断是否允许核心线程超时后被回收的标志
    //也就是说，当线程池中没有了任务，那么超过线程存活时间之后，线程池的核心线程也允许被回收
    private volatile boolean allowCoreThreadTimeOut;

    //线程的存活时间，这个一般是针对非核心线程的，如果allowCoreThreadTimeOut设置为true了
    //那么核心线程在经过keepAliveTime空闲时间之后，也会被回收
    private volatile long keepAliveTime;

    //线程池的核心线程数量
    private volatile int corePoolSize;

    //线程池最大线程数量
    private volatile int maximumPoolSize;

    //线程池中创建过的线程的最大值
    private int largestPoolSize;

    //线程池执行完毕的任务的数量
    private long completedTaskCount;

    //线程池默认的拒绝策略
    private static final RejectedExecutionHandler defaultHandler = new AbortPolicy();


    //构造方法就不展示了


    //执行任务的方法，以及线程池状态转换的方法就不展示了


    //下面是一些get、set方法
    
    //得到线程池任务队列
    public BlockingQueue<Runnable> getQueue() {
        return workQueue;
    }

    //得到创建线程的工厂
    public ThreadFactory getThreadFactory() {
        return threadFactory;
    }

    //更新线程工厂
    public void setThreadFactory(ThreadFactory threadFactory) {
        if(threadFactory == null) {
            throw new NullPointerException();
        }
        this.threadFactory = threadFactory;
    }

    //得到线程池的核心线程数
    public int getCorePoolSize() {
        return corePoolSize;
    }


    //更新线程池的核心线程数，方法的具体逻辑我就不展示了
    //毕竟我们的目的并不是为了讲解线程池本身，而且线程池也是很基础的知识了，我就不在这些知识上耗费时间了
    public void setCorePoolSize(int corePoolSize) {
        //方法省略
    }



    //获得线程池最大线程数量
    public int getMaximumPoolSize() {
        return maximumPoolSize;
    }



    //更新线程池的最大线程数
    public void setMaximumPoolSize(int maximumPoolSize) {
        
    }

    //得到拒绝策略处理器
    public RejectedExecutionHandler getRejectedExecutionHandler() {
        return handler;
    }


    //更新拒绝策略处理器
    public void setRejectedExecutionHandler(RejectedExecutionHandler handler) {
        if(handler == null) {
            throw new NullPointerException();
        }
        this.handler = handler;
    }


    //得到线程池是否设置了核心线程超时回收标志
    public boolean allowsCoreThreadTimeOut() {
        return allowCoreThreadTimeOut;
    }


    //更新线程池的核心线程超时回收标志
    public void allowCoreThreadTimeOut(boolean value) {
        
    }

    //得到线程池中线程的最大空闲时间
    public long getKeepAliveTime(TimeUnit unit) {
        return unit.convert(keepAliveTime, TimeUnit.NANOSECONDS);
    }


    //更新线程池线程的最大空闲时间
    public void setKeepAliveTime(long time, TimeUnit unit) {
        if(time<0) {
            throw new IllegalArgumentException();
        }
        if(time == 0 && allowsCoreThreadTimeOut()) {
            throw new IllegalArgumentException("Core threads must have nonzero keep alive times");
        }
        long keepAliveTime = unit.toNanos(time);
        long delta = keepAliveTime - this.keepAliveTime;
        this.keepAliveTime = keepAliveTime;
        if(delta<0) {
            interruptIdleWorkers();
        }
    }


    //得到线程池当前的线程数量
    public int getPoolSize() {
        mainLock.lock();
        try {
            return runStateAtLeast(ctl.get(), TIDYING) ? 0 : workerPool.size();
        } finally {
            mainLock.unlock();
        }
    }


    //得到线程池曾创建过的线程的最大数量
    public int getLargestPoolSize() {
        mainLock.lock();
        try {
            return largestPoolSize;
        } finally {
            mainLock.unlock();
        }
    }


    //得到线程池当前活跃线程数量，注意这里得到的是活跃线程数量，而上面的getPoolSize()方法得到的是线程数量
    public int getActiveCount() {
        mainLock.lock();
        try {
            int n = 0;
            for(Worker w : workerPool) {
                //这里判断了一下worker对象是否上锁了
                //如果上锁了就意味着当前线程正在执行任务，也就是活跃线程
                if(w.isLocked()) {
                    //只要是活跃线程，就对n自增
                    ++n;
                }
            }
            return n;
        } finally {
            mainLock.unlock();
        }
    }


    //得到线程池当前这个时刻要执行的所有任务和已经执行过的任务的总数量
    public long getTaskCount() {
        mainLock.lock();
        try {
            //得到线程池执行过的任务的数量，注意，这里我再多数一句，在ThreadPoolExecutor中
            //这个completedTaskCount成员变量会在processWorkerExit方法中被赋值，而processWorkerExit方法
            //会在一个空闲线程超过存活时间被回收的时候调用，这个时候被回收的线程就会把它执行过的任务的总数累加到completedTaskCount成员变量上
            //所以这里要获得completedTaskCount成员变量，这样一来就能得到线程池过去执行完毕的任务的总数
            long n = completedTaskCount;
            for(Worker w : workerPool) {
                //这里得到的每个存活的线程执行过的任务的总数
                n += w.completedTasks;
                //这里判断线程是否活跃，如果活跃意味着线程正在执行一个任务
                if(w.isLocked()) {
                    //线程正在执行的这个任务也要累加上
                    ++n;
                }
            }
            
            //最后再加上任务队列中剩余任务的数量
            //这样一来得到的就是线程池已经执行了和要执行的所有任务的总数
            //哎，说了不写注释的，结果又写了一堆，我怎么就控制不住自己的手呢！！！他娘的！！！
            return n + workQueue.size();
        } finally {
            mainLock.unlock();
        }
    }


    //得到线程池已经执行完毕的任务的数量
    public long getCompletedTaskCount() {
        mainLock.lock();
        try {
            long n = completedTaskCount;
            for(Worker w : workerPool) {
                n += w.completedTasks;
            }
            return n;
        } finally {
            mainLock.unlock();
        }
    }


    //其余内容省略



    //Worker类
    private final class Worker extends AbstractQueuedSynchronizer implements Runnable {
      
        private static final long serialVersionUID = 6138294804551838833L;
    
        //真正执行任务的线程
        final Thread workerThread;
        
        //要被执行的任务
        Runnable firstTask;
        
        //当前线程执行完毕的任务总数
        volatile long completedTasks;
        
        //构造方法
        Worker(Runnable firstTask) {
            setState(-1); 
            this.firstTask = firstTask;
            this.workerThread = getThreadFactory().newThread(this);
        }

        //内部类其余内容省略
}
```

好了，代码终于展示完毕了，虽然代码块比较长，但我可以很负责任地告诉大家，虽然代码块比较长，代码看起来比较多，但是代码内容非常简单，就是列举了 ThreadPoolExecutor 类中重要的成员变量，以及与这些成员变量相对应的 get、set 方法。在看过上面的代码块之后，我想我确实可以充满底气地说一句话：虽然目前我还不知道动态线程池框架怎么开发，我也不知道在线程池运行的过程中，究竟应该收集哪些信息，怎么动态更新线程池的核心配置参数，但是我至少可以通过这些 get、set 方法获得一些信息，并且及时更新一些成员变量的值。**实际上，对一个线程池来说，只要在恰当的时机，更新其内部成员变量的值，不就意味着线程池的配置参数动态更新了吗？这样一来，线程池就可以按照更新之后的状态继续执行任务了**。当然具体动态更新线程池成员变量的时机和方式我们目前还不清楚，但先让我来为大家总结一下，就目前我们掌握的信息来看，可以动态更新线程池的哪些信息，可以收集线程池的哪些信息。

这两点其实已经摆在大家面前了，**凡是可以 set 的成员变量，都可以被动态更新**：

**1 线程池的核心线程数量 corePoolSize 可以被动态更新。**

**2 线程池的最大线程数量 maximumPoolSize 可以被动态更新。**

**3 线程池的拒绝策略处理器 RejectedExecutionHandler 可以被动态更新。**

**4 线程池核心线程是否允许超时回收的标志 allowCoreThreadTimeOut 可以被动态更新。**

**5 线程池线程的最大空闲时间 keepAliveTime 可以被动态更新(之所以没有 threadFactory，是因为一般来说不会动态更新线程工厂)。**

**凡是可以 get 的成员变量，它们的信息都可以在线程池运行过程中被收集**：

**1 线程池的核心线程数 corePoolSize、最大线程数 maximumPoolSize 可以被收集。**

**2 线程池线程的空闲时间 keepAliveTime、核心线程是否允许超时回收 allowCoreThreadTimeOut 可以被收集。**

**3 线程池的拒绝策略 RejectedExecutionHandler 和任务队列 workQueue 可以被收集。**

**4 线程池当前创建的线程数量 poolSize、曾经创建线程的最大数量 largestPoolSize、当前活跃线程数量 activeCount 可以被回收。**

**5 线程池的执行的任务总数 taskCount、已经执行完毕的任务总数 completedTaskCount 可以被收集。**

很好，现在线程池哪些成员变量可以被动态更新，哪些运行时信息可以被收集，我们也都一目了然了。也许有朋友会觉得这也太简单了，好像我们根本就没做什么工作，只不过调用了 jdk 的 ThreadPoolExecutor 自身提供的一些方法，就这么简简单单实现了我们的目标。确实是这样的，本来就是这么简单，**比如说我们在线程池工作的时候收集到了一些信息，观测到线程池核心线程是 100，当前线程池活跃线程是 1，在这种情况下，我不想浪费太多的线程资源，就可以直接调用 ThreadPoolExecutor 的 setCorePoolSize() 方法，将线程池核心线程数量设置成 10；或者我也可以直接调用 ThreadPoolExecutor 的 allowCoreThreadTimeOut 方法，将 allowCoreThreadTimeOut 的值更新为 true，这样一来，线程池就会为内部的空闲线程设置中断标志，空闲的阻塞线程响应了中断标志后，经过 getTask() 和 runWorker() 方法的处理，最终会在 processWorkerExit() 方法中被释放**。这些就是线程池本身的逻辑了，大家可以自己去 ThreadPoolExecutor 中查看。

由此可见，**所谓的动态线程池框架，似乎也没什么技术含量，只要能一直收集线程池的运行信息，并且在恰当的时机根据线程池运行状况动态调整线程池某些成员变量的值即可**。确实是这样，但是，我们也不能想得太简单了。就比如说我上面列举出来可以收集的线程池的信息，以及可以动态更新的线程池的成员变量，请大家再仔细看看，也许有朋友会发现有一个成员变量十分值得关注，**没错，那线程池中最重要的一个成员变量，也就是任务队列 workQueue**。

在我上面列出来的线程池运行过程中可以被收集的信息中，第 3 条就指出了 workQueue 的的信息是可以被收集的，因为 ThreadPoolExecutor 提供了 getQueue() 方法，可以让我们得到 workQueue 本身，这样一来任务队列的信息就可以被我们收集到了。那么我们收集到的任务队列的信息能为我们做什么呢？当然了，这里我多说一句，**我们并不能动态更新 workQueue，就像没必要动态更新线程工厂一样，我们也没必要动态更新线程池的 workQueue 成员变量。我们不可能在线程池运行过程中忽然心血来潮，给线程池换一个阻塞队列，线程池要执行的所有任务可都在阻塞队列中呢**，如果真有人实现了这个功能，没准领导会跟你说：蝌蚪身上纹青蛙，你秀你妈呢！

好了，开个玩笑，让我们言归正传，既然不能动态更新线程池的 workQueue 成员变量，那就不更新了，反正我也没想着更新，只收集 workQueue 的信息就行了，那我们收集到的任务队列的信息能为我们做什么呢？**所谓 workQueue 的信息，就是 workQueue 的容量，也就是 capacity；存放的任务数量，也就是 size；队列的剩余容量，也就是 capacity - size**。

对线程池的动态更新来说，这是非常重要的信息。通过对这些信息的观测，我们很容易就知道应该怎么动态调整线程池的线程数量，如果线程池使用的是有界队列，我们观测到任务队列存放的任务非常多，已经快存满了，为了防止任务无法成功添加到队列中，这时候我们就可以增加线程池的最大线程数量，创建更多的线程快速处理任务队列中的任务；**如果线程资源紧张了，这时候我们还可以动态调整 workQueue 的最大容量，也就是队列的 capacity，这样就可以存放更多的任务等待线程执行**；只有在实在无法接收任务的情况下，可以使用拒绝策略处理器丢弃任务。

但眼下我们面临的问题是：我们确实可以通过 ThreadPoolExecutor 得到 workQueue，然后收集线程池运行时的 workQueue 的内部信息；**但是我们无法在需要对 workQueue 进行扩容时，动态更新 workQueue 的 capacity**。为什么这么说呢？请大家想一想 ThreadPoolExecutor 中使用的任务队列是什么类型的？很显然是 BlockingQueue 类型的，BlockingQueue 是一个接口，在 jdk 中存在众多实现类，所以对于线程池使用的任务队列，我们有多种选择。**但是有一点我必须指出来，不管是 BlockingQueue 接口的哪个实现类，都没有提供动态更新队列容量的方法**。在我们过去的经验中，线程池使用最多的任务队列可能是 ArrayBlockingQueue，也可能是 LinkedBlockingQueue，或者是 LinkedBlockingDeque，不管是哪一个吧，大家可以亲自去 jdk 中验证一下，这些队列确实没有提供动态更新队列容量的方法。

原因也很简单，因为 jdk 队列的设计者之一，也就是 Doug Lea 可能把大家都当成高手了，如果你是高手，你在使用队列的时候最好就能经过全方面的考虑之后，给队列定义一个合理的、符合预期的容量，这样一来我们使用的就是有界队列，当队列满了的时候，仍然向队列中继续添加任务，这时候大不了就阻塞住；当然，Doug Lea 也考虑到你可能是个菜鸟了，所以你还可以不定义队列容量，这样一来你使用的就是无界队列，既然是无界队列，那就没什么可担心的了，大不了最后内存溢出呗。当然，这不是你的问题，是硬件不行，是没有优秀的内存分配组件，是 Doug Lea 没有做好队列开发的工作。总之，这个 Doug Lea 有点二极管，他不是把你当成高手，就是把你当成菜鸟，反正就是没有提供动态更新队列容量的方法，这可是一个难题，需要认真思考思考。

当然，也许有的朋友对这一点满不在乎，觉得能动态更新线程池本身的成员变量就行了，大不了一直调整线程池的核心线程数和最大线程数，或者就直接使用无界队列呗，我就不信真的会内存溢出，没必要追求完美，非要动态调整 workQueue 的 capacity。确实，这种观点也有道理，而且也很轻松，不用再做一些额外的工作了。但我却非要解决这个问题，我这么做并不是想证明我是高手，而是要告诉别人，我不是菜鸟！那问题该怎么解决呢？实际上也没有其他的办法，我们只能是实现 BlockingQueue 接口，自己编写一个 BlockingQueue 接口的实现类，给我们自己实现的任务队列定义 setCapacity() 方法，然后让 ThreadPoolExecutor 使用我们自己定义的任务队列即可。思路一旦确定，接下来的工作就容易多了，反正已经有很多线程的队列展现在我们面前了，直接抄呗。算了，抄袭太难听了，还是改成借鉴吧。

## 自定义任务队列，实现 workQueue 容量的动态更新

很好，我们现在要做的就是借鉴某些队列，开发出一个可以动态更新最大容量的队列。可 BlockingQueue 接口的实现类有很多，究竟应该借鉴哪个队列呢？如果一时间没有头绪，那最好就是用排除法。首先看看队列存储任务的容器是不是数组，如果是数组就排除这些队列。原因很简单，**在 jdk 中，数组的内存是连续的，一个数组一旦创建成功其大小就不能再动态变化，想要扩容就要创建一个新的数组，然后把旧数组中的任务再复制到新数组中，在复制期间，如果线程池仍然在不断接收新的任务，这些任务也要添加到数组中**。光是这一系列操作可能出现的并发问题就让我头痛，我只是想证明我不是菜鸟，我可没说我是高手啊，所以就先把存放任务的容器为数组的队列排除了。不常用的队列我也不参考，排除到最后，也就剩下一个 LinkedBlockingQueue 队列可以借鉴。这也正合我意，LinkedBlockingQueue 足够简单，我肯定看得懂，**最重要的一点是 LinkedBlockingQueue 使用的是链表，如果需要动态扩容，只需要继续向链表后添加新的节点即可**，当然，最基本的并发安全还是要保证的，而且这些保证 LinkedBlockingQueue 已经为我们写好了，我们直接照抄即可。

好了，可以被我们借鉴的队列已经确定了，接下来就要明确一下我们自己定义的任务队列的名字，既然这个队列的容量可以动态调整，并且借鉴了 jdk 的 LinkedBlockingQueue 队列，**索性就把这个队列称为 ResizableCapacityLinkedBlockingQueue 吧**。这时候我知道肯定有朋友会惊讶我的行为，给自己定义的队列起这个名字不就等于明明白白告诉别人我抄袭了 LinkedBlockingQueue 吗？我理解大家的惊讶，但我可以十分坦诚地告诉大家，这就是我的目的！我说了我要证明自己不是菜鸟，那我肯定是给比我更菜的人证明的，在高手眼里，不管我怎么证明，我仍然是菜鸟啊。谁会比我更菜呢？肯定是连 LinkedBlockingQueue 都看不明白的人比我更菜。我给自己定义的队列起这个名字就是要告诉比我更菜的人，我看懂了 LinkedBlockingQueue 的源码，并且借鉴它的代码开发了一个新的、并且 capacity 可以动态更新的队列。这就好比开发 hippo4j 框架，我跟大家说过这个框架借鉴了大量的 nacos 的代码，如果你从来没看过 nacos 的代码，或者看了但是没有看懂，你肯定会觉得这个框架的作者太厉害了，不仅能看懂 nacos 的源码，还能运用它的源码开发新的框架；但假如你也透彻掌握了 nacos 的源码，再回过头看这个 hippo4j 的代码，你可能就会觉得不就是借鉴啊，谁不会啊。当然，这只是玩笑话，我一写文章就喜欢扯点闲话，开几个玩笑，说白了，我喜欢敲键盘的感觉。光是买键盘，我都已经花了好几万了，不多敲点字，真的对不住我买的那么多键盘。

  

好了，再次让我们言归正传，让我们把目光集中到 ResizableCapacityLinkedBlockingQueue 类上，来探讨一下这个任务队列应该怎么定义。按照 LinkedBlockingQueue 的内容来说，ResizableCapacityLinkedBlockingQueue 使用的也是链表，所以这个队列内部肯定也要和 LinkedBlockingQueue 一样，也应该定义一个 Node 内部类，线程池要执行的任务就会被包装成一个 Node 节点，而一个个 Node 节点就组成链表；既然是链表，ResizableCapacityLinkedBlockingQueue 队列中肯定也要定义头节点和尾节点；当然了，在节点入队和出队的时候，还要保证操作的并发安全，那么同步锁也应该是必不可少的成员变量；还有任务队列存放的数据的个数；最后别忘了我们定义新的任务队列的目的，就是为了动态更新任务队列的 capacity，所以，别忘了定义 capacity 成员变量。到目前为止，我们新定义的任务队列的成员变量和 LinkedBlockingQueue 中的成员变量一摸一样，也没必要不好意思，直接复制过来吧。接下来，就请大家先看一看我初步定义好的 ResizableCapacityLinkedBlockingQueue 类的内容。请看下面代码块。

```
public class ResizableCapacityLinkedBlockingQueue<E> extends AbstractQueue<E> implements BlockingQueue<E>,java.io.Serializable {

    private static final long serialVersionUID = -6903933977591709194L;
        



    //队列的最大容量，这个成员变量是可以动态更新的
    private int capacity;

    //当前队列存放的数据的个数
    private final AtomicInteger count = new AtomicInteger(0);

    //链表头节点
    private transient Node<E> head;

    //链表尾节点
    private transient Node<E> last;

    //数据出队的同步锁
    private final ReentrantLock takeLock = new ReentrantLock();

    //出队的条件对象
    private final Condition notEmpty = takeLock.newCondition();

    //数据入队的同步锁
    private final ReentrantLock putLock = new ReentrantLock();

    //入队的条件对象
    private final Condition notFull = putLock.newCondition();

    //无界队列构造方法
    public ResizableCapacityLinkedBlockingQueue() {
        this(Integer.MAX_VALUE);
    }

    //有界队列构造方法
    public ResizableCapacityLinkedBlockingQueue(int capacity) {
        if (capacity <= 0) {
            throw new IllegalArgumentException();
        }
        this.capacity = capacity;
        last = head = new Node<E>(null);
    }



    //内部类，Node对象就是链表中的节点，数据就包装在Node对象中
    //然后添加到链表中
    static class Node<E> {

        //数据本身
        volatile E item;

        //指向下一个节点
        Node<E> next;

        Node(E x) {
            item = x;
        }
    }


    //其他内容暂时省略
        
        
}
```

好了，现在 ResizableCapacityLinkedBlockingQueue 类最基本的成员变量已经定义好了，接下来就要定义一些核心方法，比如向队列中添加数据的 offer 方法，从队列中取出数据的 take 方法，得到任务队列存放任务数量的 size() 方法，得到任务队列剩余容量的 remainingCapacity() 方法。当然，我这里列举出来的只是我们要用到的方法，还有很多方法，比如 put、poll、限时的 offer、poll 等等方法，我就不一一展示了，大家可以直接看我提供的第一版本代码。请看下面代码块。

```
public class ResizableCapacityLinkedBlockingQueue<E> extends AbstractQueue<E> implements BlockingQueue<E>,java.io.Serializable {

    private static final long serialVersionUID = -6903933977591709194L;

    //队列的最大容量，这个成员变量是可以动态更新的
    private int capacity;

    //当前队列存放的数据的个数
    private final AtomicInteger count = new AtomicInteger(0);

    //链表头节点
    private transient Node<E> head;

    //链表尾节点
    private transient Node<E> last;

    //数据出队的同步锁
    private final ReentrantLock takeLock = new ReentrantLock();

    //出队的条件对象
    private final Condition notEmpty = takeLock.newCondition();

    //数据入队的同步锁
    private final ReentrantLock putLock = new ReentrantLock();

    //入队的条件对象
    private final Condition notFull = putLock.newCondition();


    //内部类，Node对象就是链表中的节点，数据就包装在Node对象中
    //然后添加到链表中
    static class Node<E> {

        //数据本身
        volatile E item;

        //指向下一个节点
        Node<E> next;

        Node(E x) {
            item = x;
        }
    }


    //构造方法省略


    //数据入队的方法
    @Override
    public boolean offer(E o) {
        if (o == null) {
            throw new NullPointerException();
        }//得到队列当前存放数据的数量
        final AtomicInteger count = this.count;
        //如果存放的任务数量大于等于队列最大容量，直接返回false，这时候没办法再将数据入队
        if (count.get() >= capacity) {
            return false;
        }
        //走到这里意味着队列没有满
        int c = -1;
        final ReentrantLock putLock = this.putLock;
        //开始执行数据入队操作，为了保证并发安全，要获得同步锁
        putLock.lock();
        try {//在这里再次判断一下队列是否已满
            //这就是一个简单的同步锁双重判断
            if (count.get() < capacity) {
                //走到这里意味着队列没有满，直接把数据添加到队列中即可
                insert(o);
                //数据添加之后，得到添加数据之前队列的容量
                //注意这里是getAndIncrement，先得到值然后再自增
                c = count.getAndIncrement();
                //这里判断一下当有数据入队之后，队列中是否还有容量
                //如果还有容量，就唤醒正在阻塞中的线程
                if (c + 1 < capacity) {
                    //这里大家可能会有疑惑，为什么明明已经有线程添加数据成功了，怎么还会有添加数据的线程阻塞呢？
                    //这是因为可能有些线程向队列添加数据的时候，队列已经满了，这些线程就会直接被阻塞，具体逻辑可以去
                    //put、限时的offer方法中查看
                    //而当前线程添加数据的时候，有些数据已经从队列中取走了，所以要判断一下队列是否还有容量
                    //有的话唤醒正在阻塞的线程
                    notFull.signal();
                }
            }
        } finally {
            //释放锁
            putLock.unlock();
        }//这里的这个判断也很有必要，注意，c是数据添加到队列之前的队列容量
        //如果之前队列容量为0，就意味着想从队列中获取数据的线程会被阻塞，但现在添加了新的数据了
        //所以可以唤醒那些阻塞的出队线程了
        if (c == 0) {
            signalNotEmpty();
        }//程序走到这里会判断一下c是否不为-1，如果仍然为-1，就意味着根本没有进入try代码块中的if分支
        //也就意味着添加任务失败，返回false即可
        return c >= 0;
    }


    //添加数据到链表中的方法，添加到队尾
    private void insert(E x) {
        last = last.next = new Node<E>(x);
    }


    //唤醒出队线程的方法
    private void signalNotEmpty() {
        final ReentrantLock takeLock = this.takeLock;
        takeLock.lock();
        try {
            notEmpty.signal();
        } finally {
            takeLock.unlock();
        }
    }



    //数据出队的方法
    @Override
    public E take() throws InterruptedException {
        E x;
        int c = -1;
        final AtomicInteger count = this.count;
        final ReentrantLock takeLock = this.takeLock;
        takeLock.lockInterruptibly();
        try {
            try {//判断队列是否为空，如果为空则阻塞线程
                while (count.get() == 0) {
                    notEmpty.await();
                }
            } catch (InterruptedException ie) {
                notEmpty.signal();
                throw ie;
            }
            //走到这里意味着队列不为空，执行从队列中获得数据的操作
            x = extract();
            //更新队列容量
            c = count.getAndDecrement();
            if (c > 1) {
                //如果队列还有数据，唤醒其他正在阻塞的出队线程
                notEmpty.signal();
            }
        } finally {
            //释放同步锁
            takeLock.unlock();
        }//如果在将数据移除队列之前，队列的容量满了，那现在有数据移出队列了，可以唤醒阻塞的入队线程了
        if (c >= capacity) {
            signalNotFull();
        }
        return x;
    }


    //出队的方法。出队的时候从头节点出队
    private E extract() {
        Node<E> first = head.next;
        head = first;
        E x = first.item;
        first.item = null;
        return x;
    }


    //得到队列存放数据个数的方法
    @Override
    public int size() {
        return count.get();
    }

    //得到队列剩余容量的方法
    @Override
    public int remainingCapacity() {
        return capacity - count.get();
    }
        
}
```

上面代码块中的注释非常详细，所以我就不再赘述其中的逻辑了，而且大家对任务队列的内部结构肯定也很熟悉，所以简单看看就成。当然，我也能想到很多朋友看了我在上面代码块中展示的代码之后，会忍不住发自肺腑地感叹一句：你抄得也太不要脸了，你这个 ResizableCapacityLinkedBlockingQueue 类的代码和 LinkedBlockingQueue 类的代码有什么不同啊？这特么不是一模一样啊！对此我只能说一句：别急朋友，马上就会不一样了，别眨眼。

没错，前面展示得代码都是前菜，接下来才是真正进入正题的时刻，别忘了我们的目的是实现一个可以动态更新 capacity 的任务队列。现在队列的诸多方法已经实现了，就差最后动态更新 capacity 的方法了，所以，我最后只需给 ResizableCapacityLinkedBlockingQueue 类定义一个 setCapacity() 方法即可。而这个 setCapacity() 方法也很简答，就是直接给 ResizableCapacityLinkedBlockingQueue 类的 capacity 赋值。具体逻辑请看下面代码块。

```
public class ResizableCapacityLinkedBlockingQueue<E> extends AbstractQueue<E> implements BlockingQueue<E>,java.io.Serializable {



    //队列的最大容量，这个成员变量是可以动态更新的
    private int capacity;


    //省略其他内容


    //动态更新capacity的方法
    public void setCapacity(int capacity) {
        final int oldCapacity = this.capacity;
        //给capacity成员变量赋值
        this.capacity = capacity;
        final int size = count.get();
        if (capacity > size && size >= oldCapacity) {
            //因为队列扩容了，所以可以唤醒阻塞的入队线程了
            signalNotFull();
        }
    }



    //唤醒入队线程的方法
    private void signalNotFull() {
        final ReentrantLock putLock = this.putLock;
        putLock.lock();
        try {
            notFull.signal();
        } finally {
            putLock.unlock();
        }
    }
    
}
```

到此为止，我定义的 ResizableCapacityLinkedBlockingQueue 类的代码已经展示完毕了。也许有的朋友会说你这代码和 LinkedBlockingQueue 相比改变什么了？不就多了一个 setCapacity 方法吗？我的回答是你别管变多变少，你就说变没变吧，在我看来，变一点也是变，并且我定义的这个 setCapacity 方法，可是要在动态线程池框架中承担至关重要的职责。所以，叫我一声靓仔吧。

好了，现在可以动态更新最大容量的任务队列已经定义好了，只要让线程池使用这个任务队列，就可以弥补任务队列容量无法动态更新的遗憾，除此之外，线程池也没什么成员变量需要动态更新了。那么，这是不是就意味着我们终于可以着手开发动态线程池框架了呢？我仍然建议大家再等等，因为就线程池本身来说，还有一些信息值得我们关注。

## 探讨 ThreadPoolExecutor 的扩展点

在我们继续向下讨论之前，我想先问大家一个问题，我们即将开发的这个动态线程池框架，它最大的作用是什么？这个问题的答案想必大家都很清楚，**肯定是监测线程池运行时信息，并且动态调整线程池参数**。动态调整线程池参数已经没什么可说的了，线程池中能动态更新的成员变量我都已经讲解过了，不能动态更新的参数我也自定义了任务队列，让它能更新了。剩下的就是检测线程池运行时信息，而这一点在前两小节似乎也讲得差不多了。我把能收集到的信息都列出来了，就是下面展示的这样。请看下面代码块。

```
1 线程池的核心线程数 corePoolSize、最大线程数 maximumPoolSize 可以被收集。
2 线程池线程的空闲时间 keepAliveTime、核心线程是否允许超时回收 allowCoreThreadTimeOut 可以被收集。
3 线程池的拒绝策略 RejectedExecutionHandler 和任务队列 workQueue 可以被收集。
4 线程池当前创建的线程数量 poolSize、曾经创建线程的最大数量 largestPoolSize、当前活跃线程数量 activeCount 可以被回收。
5 线程池的执行的任务总数 taskCount、已经执行完毕的任务总数 completedTaskCount 可以被收集。
```

我在这里再次把这些简单的信息罗列出来，并不是为了凑字数，我只是想再跟大家再分析分析这些信息。就目前的信息来看，我们能收集到的都是一些很直接的，并且很简单的线程池运行信息，所谓很直接，就是没有加工过的意思。那什么叫做加工呢？**比如我想得到当前时刻线程池的负载，也想得到线程池线程的峰值，这两个信息该怎么获得呢？其实很简单，所谓线程池的负载，就是用线程池当前的活跃线程数比上最大线程数即可；而线程池的峰值就是用线程池曾创建的线程的最大数量比上最大线程数**。既然是要监控动态线程池的信息，肯定就要监控的全面一些，收集数据只是第一步，如果只收集数据，而不分析处理数据，那这些数据也没什么大的用处。得到线程池的负载以及峰值，就是处理数据的具体表现，这些处理过的数据会让线程池的运行情况更加一目了然。**由此可见，在我们开发的动态线程池框架中，还应该对收集到的数据进行精准处理，并且动态线程池框架监控的信息也应该尽量全面**。现在我们分析的只是和线程池本身有关的运行信息，假如我想把监控的信息涉及到业务层面呢？

当然，业务本身的逻辑并不是动态线程池框架可以涉及的，线程池执行什么任务，这是每一个程序员自己的事。但是，有没有一个方法可以让动态线程池框架监控到任务执行的耗时呢？这样一来不就相当于监控到了业务层操作执行的耗时吗？或者更进一步，我希望线程池可以收集到线程池执行任务的方法栈信息。这样一来当线程池执行的任务出现问题时，可以迅速定位任务的源头以及任务执行操作的轨迹。如果线程池执行任务时间过长，这些信息都可以被动态线程池框架收集到，也有助于程序员进一步优化自己的业务代码。当然，**如果想做得更完善一些，甚至可以让用户自己定义这些任务的执行超时时间，比如说用户根据业务情况，将线程池执行任务的超时时间定义为 500 毫秒，那么只要动态线程池框架检测到某个任务执行耗时超过 500 毫秒，就会立即向用户发送警报**。也许是发送邮箱、也许直接微信通知，总之你先别管我怎么实现告警通知功能，就像我骗我媳妇说先借我 1000 块钱，过两天还她 10000。别管我能不能还上，先把 1000 块钱借给我再说。现在回到动态线程池框架，我要的也是这个效果。别管我怎么实现告警通知功能，现在请大家想一想，我怎么让动态线程池框架监控线程池执行每个任务的耗时信息呢？

如果我不仅仅想给用户发送任务执行超时的告警通知，我还想给用户发送任务被拒绝的告警通知，也就是说，每当有任务被线程池拒绝的时候，就向用户及时通知这个情况，那这个功能我该怎么实现呢？这时候就涉及到线程池的功能扩展了，仅仅凭借 ThreadPoolExecutor 是无法实现以上两个功能的。当然，ThreadPoolExecutor 为我们提供了一些扩展点，我们可以继承 ThreadPoolExecutor，定义自己的线程池类，然后在自己定义的类中重写 ThreadPoolExecutor 中的扩展方法，这样一来，我们就可以定制自己的扩展操作了。那么，ThreadPoolExecutor 中的扩展点都有什么呢？又该如何继承 ThreadPoolExecutor，实现自己的线程池类呢？如何在自己的线程池中重写扩展方法呢？这一章显然是讲不完了，就留到下一章讲解吧。

朋友们，我们下一章见。