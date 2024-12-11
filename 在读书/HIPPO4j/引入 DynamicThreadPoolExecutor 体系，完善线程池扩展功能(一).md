书接上回，在上一章的结尾，我跟大家简单讨论了一下 ThreadPoolExecutor 的扩展点，当然，具体的扩展方法我并没有讲解，只是提出了几个功能。比如可以通过线程池的扩展点得到线程池执行每一个任务的耗时；或者得到线程池拒绝过的任务的数量等等。这一章，我就为大家一一实现这些功能，在实践中为大家展示 ThreadPoolExecutor 中可扩展的方法。

## 实现计算线程池执行任务耗时功能

  

不知道大家有没有这样一种感觉：很多听起来比较高深，甚至令人生畏的概念或者名词，其背后的本质往往都特别简单；那些令人惊叹的技术，其背后的原理可能连初学者都觉得没有难度。我自己常常有这种感觉，尤其是前段时间整理响应式系列课程的代码时，很多代码表面上看上去非常神奇，奇妙的功能令人不可思议，但抽丝剥茧，解开一层层神秘的面纱后，我发现功能得以实现的方式非常简单。也许响应式本身的概念有点复杂和晦涩，但是当我看完代码之后，我相信我可以让任何一个编程初学者(掌握了并发编程就算初学者了，当然，长得再漂亮一点更有利于学习)都能轻而易举地理解响应式，并且开发一个自己的响应式框架。好了，话题扯远了，我在这里之所以啰嗦了一会，是因为想到我们即将实现的这个功能，如果这个功能放到动态线程池框架中，我们完全可以给这个功能取一个高深的名字，比如就叫做：线程池执性能监测功能。但真正实现起来是怎么样的呢？很简单，**就是在线程池执行每一个任务之前得到任务开始执行的时间，任务执行完毕之后得到当前时间，然后让当前时间减去任务开始执行的时间，这样就得到了线程池执行任务的耗时。接下来就更简单了，如果耗时较短，那线程池的工作性能就不错，如果耗时较长，那性能就一般般**。当然这个性能和线程池本身的关系可能不大，在线程池线程数量合适的情况下，程序的性能反应的是任务本身是否合理。

现在我们已经明确了计算线程池执行任务耗时的方式，接下来就应该在代码层面实现一下了。不过在真正开始实现之前请大家想一下，计算线程池执行任务耗时无非就是用执行结束的时间减去执行之前的时间，那这个操作应该定义在哪里？要定义在任务本身中吗？就像下面代码块展示的这样。

```
new Runnable() {
    @Override
    public void run() {
        long start = System.currentTimeMillis();

        //在这里执行真正的业务逻辑
        //假如下面这个循环就是业务逻辑
        for (int i = 0; i < 10000000; i++) {
                //逻辑省略
        }

        long end = System.currentTimeMillis();

        //得到任务执行耗时
        long duration = end - start;
        System.out.println("任务执行耗时:" + duration + "毫秒")
    }
}
```

上面代码块中展示的方法当然是可行的，但我们肯定不会真的这么做，因为我们开发一个框架让用户使用，肯定只让用户编写业务逻辑即可，任务监测的工作应该由框架本身来负责。如果是这样的话，那就只能从 ThreadPoolExecutor 中寻找突破口了。因为所有任务最终是要在 ThreadPoolExecutor 中被执行的，接下来就请大家看看 ThreadPoolExecutor 执行任务的关键方法。请看下面代码块。

```
public class ThreadPoolExecutor extends AbstractExecutorService {



    //其余内容省略


     final void runWorker(Worker worker) {
         
        Thread currentThread = Thread.currentThread();
        Runnable runnableTask = worker.firstTask;
        worker.firstTask = null;
        worker.unlock(); 
        boolean completedAbruptly = true;
        try {
            while(true) {
                if(runnableTask == null) {
                    //从任务队列中获得任务
                    runnableTask = getTask();
                    if(runnableTask == null) {
                        break;
                    }
                }
                worker.lock();
                if(runStateAtLeast(ctl.get(), STOP)) {
                    if(!currentThread.isInterrupted()) {
                        currentThread.interrupt();
                    }
                } else {
                    Thread.interrupted();
                }
        
                try {
                    beforeExecute(currentThread, runnableTask);
                    try {//这里就是真正执行任务的操作
                        runnableTask.run();
                        afterExecute(runnableTask, null);
                    } catch(Throwable ex) {
                        afterExecute(runnableTask, ex);
                        throw ex;
                    }
                } finally {
                    runnableTask = null;
                    worker.completedTasks++;
                    worker.unlock();
                }
            }
            completedAbruptly = false;
        } finally {
            processWorkerExit(worker, completedAbruptly);
        }
    }


    //该方法会在执行用户任务之前被回调
    protected void beforeExecute(Thread workerThread, Runnable runnableTask) {
        
    }

    //该方法会在执行用户任务之后被回调
    protected void afterExecute(Runnable runnableTask, Throwable ex) {
        
    }
    



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


        //执行任务的方法
        public void run() {
            runWorker(this);
        }
}
```

在上面代码块的第 36 行有 runnableTask.run() 这样一行代码，这行代码就是真正执行用户提交的任务的操作。所以，如果我们想计算任务的耗时，只需要在这个方法前后获得任务执行开始时间和结束时间，然后再得到时间差即可。这样一来，我们要做的似乎就是在 runnableTask.run() 这行代码前后添加一些代码，以此实现我们需要的功能。可这是行不通的，因为我们不可能直接修改 ThreadPoolExecutor 类的源码。别急，虽然我们不能直接修改 ThreadPoolExecutor 的源码，但是在上面代码块的第 34 和 37 行，我们可以看到这样两行代码，请看下面代码块。

```
beforeExecute(currentThread, runnableTask);
//runnableTask.run();
afterExecute(runnableTask, null);
```

通过方法的名字我们就能看出来，beforeExecute 和 afterExecute 方法好像就是专门为了处理目标方法执行的前后逻辑而设计的。并且如果我们去 ThreadPoolExecutor 中查看的话，会发现不管是 beforeExecute 还是 afterExecute 方法，都被设计成了空方法，好像就是 ThreadPoolExecutor 留给我们的拓展方法，可以让我们自己定义其中的逻辑。**如果我们在 beforeExecute 方法中得到任务执行的开始时间，在 afterExecute 方法中得到任务执行的结束时间，然后得到时间差，这不就得到任务执行的耗时了吗**？当然，要在这两个方法中定义自己的逻辑，肯定需要重写这两个方法，也就不得不自己定义一个线程池类，并且这个线程池类必须继承 ThreadPoolExecutor。看起来是有点麻烦，但麻烦归麻烦，逻辑倒是非常简单。既然我们自定义的线程池类必须继承原有的 ThreadPoolExecutor，并且我们自己定义的线程池类是在原有的 ThreadPoolExecutor 上做扩展，不如就把我们自己定义的类叫做 ExtensibleThreadPoolExecutor 吧，翻译过来就是可扩展的 ThreadPoolExecutor。

就目前的情况来说，我们自己定义的这个 ExtensibleThreadPoolExecutor 类还不必多复杂，我们的目标只是实现计算线程池执行任务耗时功能，所以只需要重写父类的 beforeExecute 和 afterExecute 方法即可。但是在重写方法之前，我们还是需要再思考一个问题，我们要在 beforeExecute 方法中得到线程执行任务的开始时间，要在 afterExecute 方法中得到任务执行的结束时间，并且计算出时间差，那怎么在 afterExecute 方法中得到当前线程执行任务的结束时间呢？最常规的做法就是当一个线程在 beforeExecute 中得到了执行任务的开始时间后，把这个时间存放在一个容器中，然后在 beforeExecute 方法中获取。**可随之而来的问题就是线程池中有很多线程，每个线程都在执行任务，你怎么知道哪个开始时间对应着哪个结束时间呢？这时候就轮到老朋友 ThreadLocal 登场了**。我相信当大家看到 ThreadLocal 的一刻就知道怎么回事了，所以具体逻辑我就不再赘述了，请大家直接看下面的代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：扩展线程池类，这个类为jdk的原生线程池提供了非常多的扩展点，基本上每一个重要操作都提供了拓展点
 */
public class ExtensibleThreadPoolExecutor extends ThreadPoolExecutor{

    //引入ThreadLocal
    private final ThreadLocal<Long> startTimes = new ThreadLocal<>();

    //构造方法
    public ExtensibleThreadPoolExecutor(int corePoolSize, int maximumPoolSize,long keepAliveTime, TimeUnit unit, BlockingQueue<Runnable> workQueue, ThreadFactory threadFactory, RejectedExecutionHandler handler) {
        super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue, threadFactory, handler);
    }




    //任务开始执行之前会执行这个方法，在这个方法中把任务开始执行时间放到线程本地map中
    @Override
    public final void beforeExecute(Thread thread, Runnable runnable) {
        startTimes.set(currentTime());
    }




    //该方法会在任务执行之后被调用
    @Override
    public final void afterExecute(Runnable runnable, Throwable throwable) {
        try {
            //从线程本地map中得到任务的开始时间
            Optional.ofNullable(startTimes.get())
                    //计算出耗时时间
                    .map(startTime -> currentTime() - startTime)
                    //交给processTaskTime方法处理
                    .ifPresent(this::processTaskTime);
        } finally {
            //清除线程本地map
            startTimes.remove();
        }
    }


    //得到当前时间的方法
    protected long currentTime() {
        return SystemClock.now();
    }



    //该方法作为一个拓展方法，可以在该方法中把得到的任务耗时信息上报到监控系统
    //也可以在该方法中判断任务执行是否超时，如果超时就通知用户
    //这个方法暂时还不实现
    protected void processTaskTime(long taskExecuteTime) {
       
    }
    
    
    
}
```

在上面的代码块中，大家可以很清楚地看到，我使用了 ThreadLocal 来使线程池中的每一个线程可以获得自己执行任务的开始时间，并且在 afterExecute 方法中计算出了任务耗时情况后，移除了线程本地 map 中的数据，防止内存泄漏。当然，我还在 ExtensibleThreadPoolExecutor 类中额外定义了一个 processTaskTime 方法，这个方法在获得任务执行耗时后可以执行一些额外操作，比如判断任务是否执行超时，然后决定是否通知用户。虽然这个 processTaskTime 方法还没有实现，但就目前展示的代码块来说，我们自己定义的 ExtensibleThreadPoolExecutor 线程池已经比 ThreadPoolExecutor 增加了一些功能了。当用户想要在业务中使用线程池的时候，就可以直接创建一个 ExtensibleThreadPoolExecutor 对象，这样一来 ExtensibleThreadPoolExecutor 对象仍然具有 ThreadPoolExecutor 的所有功能，同时还可以计算每一个任务执行耗费的时间。这个逻辑想必已经很清晰了。

但是看到这里大家会不会觉得哪里不对劲？反正我的第一直觉就是我把这个 ExtensibleThreadPoolExecutor 类写死了。这个 ExtensibleThreadPoolExecutor 类本来的含义是可扩展的 ThreadPoolExecutor，但从我为大家展示的代码来看，这个 ExtensibleThreadPoolExecutor 类也就扩展了一个功能，那就是计算线程池执行任务耗时。如果我定义的这个 ExtensibleThreadPoolExecutor 类只能扩展这一个功能，那还不如叫做 TaskTimerThreadPoolExecutor，也就是只能计算任务耗时的线程池，何必把名字定义为 ExtensibleThreadPoolExecutor，搞得这么全面的一个名字，结果只多了一个计算任务执行耗时的功能，难免让人失望。

而且，请大家再发散一下思维，仔细想想 ThreadPoolExecutor 为我们预留的 beforeExecute 和 afterExecute 方法，预留这两个方法的本意是为了让我们能定义自己想要的逻辑，也就是说，我不仅仅可以通过重写这两个方法，实现线程池任务计时功能，我还可以实现任何我想实现的功能。我要是想追踪任务执行轨迹，那么我就在这两个方法中记录任务执行日志，这也是可以的吧？但现在的情况是，我已经把 ExtensibleThreadPoolExecutor 类写死了，如果我想实现记录任务日志的功能，肯定还需要再定义一个 LoggingThreadPoolExecutor，如果用户想实现追踪任务执行轨迹，那就可以在程序中创建一个 LoggingThreadPoolExecutor 对象，使用这个对象执行任务。这么一来，假如我希望对 ThreadPoolExecutor 实现更多的扩展功能，难道就要实现定义好非常多的 XxxxxThreadPoolExecutor 类吗？这未免也太麻烦了，也不是我想要的效果。那么正确的做法是什么呢？

## 引入ThreadPoolPlugin ，重构 ExtensibleThreadPoolExecutor 类

  

**正确的做法当然是引入一个接口，然后把任何用户想自定义的扩展功能插件化，然后让我们自定义的线程池持有这些插件，在任务执行前后执行插件中的方法即可**。我知道大家可能有点不理解这句话的意思，别着急，很快我就会为大家解释清楚。在解释之前我还要再多说一句，这种插件化的方式实际上也是 nacos 中的惯用操作。所以我说这个 hippo4j 框架借鉴了很多 nacos 的内容，并不是空穴来风。当然，我并没有说不能这么借鉴，我也不认为这个 hippo4j 框架代码水平不行。恰恰相反，我认为在适当借鉴别人代码的基础上，能开发出一个功能全面的属于自己的框架，这是很令人赞赏的技术。

好了，让我们言归正传，我知道已经有人在等着我解释什么叫插件化了，而引入的接口，又是什么接口呢？这个解释起来就简单很多了，因为我敢才说了，用户可能希望对 ThreadPoolExecutor 实现各种扩展功能，扩展的方法已经被 ThreadPoolExecutor 预留出来了，那么我们其实只需要对这些方法实现不同的逻辑，也就相当于对 ThreadPoolExecutor 做了很多不同的扩展。**这样一来，我其实完全可以引入一个接口，接口中就定义 beforeExecute 和 afterExecute 这两个方法，用户可以自己定义接口的实现类，当然，有一些实现类可以预置在框架中**。这样一来，对 ThreadPoolExecutor 的扩展不就自有很多了吗？比如我就把要引入的接口称为 ThreadPoolPlugin，在这个接口中定义 beforeExecute 和 afterExecute 两个方法，具体实现请看下面代码块。

```

public interface ThreadPoolPlugin {

    
    default void beforeExecute(Thread thread, Runnable runnable) {
        
    }

    
    default void afterExecute(Runnable runnable, Throwable throwable) {
        
    }
    
}
```

如果用户想对线程池扩展计算任务执行耗时功能，那就可以定义一个实现类，实现这个 ThreadPoolPlugin 接口。比如就把实现类定义为 TaskTimerPlugin，具体实现请看下面代码块。

```
//计算任务执行耗时，并且判断是否超时的插件
public abstract class TaskTimerPlugin implements ThreadPoolPlugin {


    private final ThreadLocal<Long> startTimes = new ThreadLocal<>();

    //在这里定义一个成员变量，代表任务超时时间
    private Long executeTimeOut;


    //构造方法
    public TaskTimerPlugin(Long executeTimeOut){
        this.executeTimeOut = executeTimeOut;    
    }
    

    //任务开始执行之前会执行这个方法，在这个方法中把任务开始执行时间放到线程本地map中
    @Override
    public final void beforeExecute(Thread thread, Runnable runnable) {
        startTimes.set(currentTime());
    }


    //该方法会在任务执行之后被调用
    @Override
    public final void afterExecute(Runnable runnable, Throwable throwable) {
        try {
            //从线程本地map中得到任务的开始时间
            Optional.ofNullable(startTimes.get())
                    //计算出耗时时间
                    .map(startTime -> currentTime() - startTime)
                    //交给processTaskTime方法处理
                    .ifPresent(this::processTaskTime);
        } finally {
            //清除线程本地map
            startTimes.remove();
        }
    }

   
    //得到当前时间的方法
    protected long currentTime() {
        return SystemClock.now();
    }


    //设置任务执行超时时间
    public void setExecuteTimeOut(Long executeTimeOut){
        this.executeTimeOut = executeTimeOut;
    }


    //该方法作为一个拓展方法，可以在该方法中把得到的任务耗时信息上报到监控系统
    //也可以在该方法中判断任务执行是否超时，如果超时就通知用户
    protected void processTaskTime(long taskExecuteTime) {

        //判断任务执行耗时是否超时了，没超时则直接返回
        if (taskExecuteTime <= executeTimeOut) {
            return;
        }

        //超时了则向用户报警
        System.out.println("通知用户任务执行超时了！");
       
    }
}
```

可以看到，**我新定义的这个 TaskTimerPlugin 类的逻辑和之前定义的 ExtensibleThreadPoolExecutor 类的逻辑没有什么大的不同，无非就是多了一个 executeTimeOut 成员变量，这个成员变量的出现可以让 TaskTimerPlugin 对象判断是否要通知用户任务执行超时了**。这时候肯定会有朋友感到困惑，那之前定义的 ExtensibleThreadPoolExecutor 该怎么使用呢？现在对 ThreadPoolExecutor 扩展的功能都被封装到了一个个 ThreadPoolPlugin 对象中，ExtensibleThreadPoolExecutor 还需要定义之前的计算任务执行耗时的逻辑吗？显然是不需要了。现在我需要重构 ExtensibleThreadPoolExecutor 类。

  

我重构 ExtensibleThreadPoolExecutor 的思路也很简单，因为这次我并不准备把这个类写死了，**而是在这个类内部定义一个 ThreadPoolPluginManager 插件管理器成员变量，这个成员变量专门存放用户定义的各种 ThreadPoolPlugin 对象，然后在 ExtensibleThreadPoolExecutor 类的 beforeExecute 和 afterExecute 两个方法中执行插件对象中的逻辑**。具体实现请看下面代码块。

  

首先是 ThreadPoolPluginManager 类。

```
//线程池插件管理器，和线程池有关的所有插件都会注册到这个插件管理器中，每一个线程池都持有一个插件管理器对象
//这个类的逻辑非常简单，就是向集合中添加数据移除数据的操作，大家自己看看就成
public class ThreadPoolPluginManager {

    //存放线程池插件对象的集合
    private final List<ThreadPoolPlugin> threadPoolPluginList = new CopyOnWriteArrayList<>();


    //注册插件对象到集合中的方法
    public void register(ThreadPoolPlugin plugin) {

         threadPoolPluginList.add(plugin);
         
    }


    //得到所有插件的方法
    public Collection<ThreadPoolPlugin> getThreadPoolPluginList() {

        return threadPoolPluginList;
    }


    //其他方法省略
    
}
```

接下来是重构之后的 ExtensibleThreadPoolExecutor 类，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：扩展线程池类，这个类为jdk的原生线程池提供了非常多的扩展点，基本上每一个重要操作都提供了拓展点
 */
public class ExtensibleThreadPoolExecutor extends ThreadPoolExecutor{

    //插件管理器对象，当前线程池用到的所有插件都会注册到这个管理器中
    @Getter
    private final ThreadPoolPluginManager threadPoolPluginManager;


    //构造方法
    public ExtensibleThreadPoolExecutor(ThreadPoolPluginManager threadPoolPluginManager, int corePoolSize, int maximumPoolSize,long keepAliveTime, TimeUnit unit, BlockingQueue<Runnable> workQueue, ThreadFactory threadFactory, RejectedExecutionHandler handler) {
        super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue, threadFactory, handler);
        this.threadPoolPluginManager = threadPoolPluginManager;
    }
    
     //该方法会在线程执行任务之前被调用
    @Override
    protected void beforeExecute(Thread thread, Runnable runnable) {
        //这里从插件处理器中得到了ThreadPoolPlugin类型的所有插件，这些插件的对象有的是用来计算任务耗时和任务是否超时了
        //有的可能是用来记录任务执行日志的
        Collection<ThreadPoolPlugin> threadPoolPluginList = threadPoolPluginManager.getThreadPoolPluginList();
        //执行了插件对象中的beforeExecute方法
        threadPoolPluginList.forEach(aware -> aware.beforeExecute(thread, runnable));
    }



    //该方法会在任务执行完毕之后被调用
    @Override
    protected void afterExecute(Runnable runnable, Throwable throwable) {
        //这里从插件处理器中再次得到了ThreadPoolPlugin类型的所有插件
        Collection<ThreadPoolPlugin> threadPoolPluginList = threadPoolPluginManager.getThreadPoolPluginList();
        //执行插件对象中的afterExecute方法
        threadPoolPluginList.forEach(aware -> aware.afterExecute(runnable, throwable));
    }
    
}
```

到此为止，我相信围绕着 ExtensibleThreadPoolExecutor 类展开的知识脉络都已经很清晰了。**我不再把 ExtensibleThreadPoolExecutor 写死了，而是为我们的程序引入了一个 ThreadPoolPlugin 接口，用户可以自己定义 ThreadPoolExecutor 的各种功能拓展插件，同时在 ExtensibleThreadPoolExecutor 中定义一个 ThreadPoolPluginManager 插件管理器对象，用户定义的各种功能扩展插件都会存放到这个插件管理器中。最后用户只需要在程序中创建 ExtensibleThreadPoolExecutor 线程池对象，注册号插件，就可以启动线程池，而线程池在运行的过程中，回在恰当的时机通过 beforeExecute 和 afterExecute 方法回调所有插件对象中的同名方法**。这就是目前程序的执行流程，为了帮助大家梳理思路，我还为大家提供了一个测试类，请看下面代码块。

```
public class Test{

    public static void main(String[] args) {

        //创建一个计算任务耗时的插件对象,超时时间为1000ms
        ThreadPoolPlugin taskTimerPlugin = new TaskTimerPlugin(1000);
        //创建插件管理器对象
        ThreadPoolPluginManager manager = new ThreadPoolPluginManager();
        //注册插件对象
        manager.register(taskTimerPlugin);
        //创建ExtensibleThreadPoolExecutor动态线程池,构造方法中其余的参数省略，注意，这里传进的队列
        //已经可以使用上一章我们自定义的ResizableCapacityLinkedBlockingQueue队列了
        ThreadPoolExecutor threadPool = new ExtensibleThreadPoolExecutor(manager,........);
        //接下来就可以向线程池提交任务了，这部分逻辑就省略了
    }
} 
```

结合上面的测试类，大家可以再仔细品味品程序目前的逻辑。如果这个逻辑理解通顺了，那么接下来，请大家再跟我思考另一个问题。当你们看到 ThreadPoolExecutor 预留出来的 beforeExecute 和 afterExecute 方法时，脑海里会想到什么呢？beforeExecute 和 afterExecute 方法作用就是在目标方法，也就是线程池执行任务的方法前后额外执行一些逻辑，这些逻辑是用户自定义的。**这种模式让我想到了 Spring 的 AOP 编程模式中的环绕通知，所谓环绕通知，就可以让用户在目标方法执行前后自定义一些操作，这种方式无疑使我们的程序、以及线程池本身的扩展性得到极大的提升**。刚才我们自定义的 ExtensibleThreadPoolExecutor 类配合上 ThreadPoolPlugin 插件对象，已经为 ThreadPoolExecutor 的 runnableTask.run() 代码实现了类似于 Spring 环绕通知的模式，但既然都已经联想到 Spring 的环绕通知编程模式了，我忍不住想为 ThreadPoolExecutor 实现更多的扩展功能。**本来 ThreadPoolExecutor 中就有很多重要方法，除了执行用户提交任务的方法，能够主动改变线程池状态的方法都称得上比较重要的方法，比如说 shutdown、shutdownNow、terminated 等等方法(这几个方法的具体作用大家可以去 ThreadPoolExecutor 中查看，我就不再赘述了，这属于线程池的基础知识，我就不再为此耗费时间了)。如果我想模仿 Spring 中的环绕通知模式为 ThreadPoolExecutor 的重要方法都提供扩展功能呢**，这样一来我又该怎么做呢？

## 重构 ThreadPoolPlugin 体系，实现线程池多个扩展点

  

在上一小节我提出一个想法，我希望我可以为 ThreadPoolExecutor 的多个方法都实现扩展点，因为这个想法是受了 Spring 中环绕通知编程模式的启发，所以，我希望也使用类似的模式为 ThreadPoolExecutor 实现多个扩展点。当然，我也简单解释了一下扩展点本身**，比如 ThreadPoolExecutor 的 shutdown、shutdownNow、terminated 等等方法，都可以成为扩展的目标。当然，我们也别忘了 ThreadPoolExecutor 中最重要的一个方法，那就是 execute 方法，这个方法也可以作为我们需要扩展的目标方法之一**。

看到这里，也许有朋友又会感到困惑，我对这些扩展点喋喋不休了这么久，那么实现这些扩展点究竟可以做什么呢？怎么实现扩展呢？毕竟 ThreadPoolExecutor 并没有为其他方法预留扩展方法呀？先别急，对 ThreadPoolExecutor 的 任何一个方法做功能扩展其实是最简单的，现在我们先来讨论一下为其他方法实现扩展功能究竟有什么用。在前两小节我们为线程池执行任务实现的扩展功能对程序运行确实很有帮助，不管是记录任务执行日志，还是计算任务执行耗时，以便决定是否向用户发送告警通知，这些都是很重要的措施。但是，假如我们要为 shutdown、shutdownNow、terminated 这些方法实现功能扩展，那我们可以为它们扩展什么功能呢？

这就有的说了，最简单的，我们仍然可以在这些方法执行前后记录响应的日志信息，这些日志信息对线程池状态的监控来说，是非常有用的信息；当然，也可以继续实现某些性能监测功能，不过这个功能对于我们要实现的动态线程池框架来说可能就有些没有必要了，因为动态线程池框架会在线程池运行期间一直监控线程池的运行状态，没必要在线程池结束工作的时候再收集线程池的运行信息；还有一些重要的功能，比如资源释放等等，如果任务队列中仍有任务，可以清空任务队列，或者释放线程池中线程使用的外部资源；这些都可以成为以上几个方法扩展的功能，并且这些功能都可以让用户根据业务自己定义，只要用户创建相应的 ThreadPoolPlugin 对象即可。但是，问题也就出在这里，请大家想一想，刚才我为大家定义的 ThreadPoolPlugin 接口，其具体内容如下，请看下面代码块。

```

public interface ThreadPoolPlugin {

    
    default void beforeExecute(Thread thread, Runnable runnable) {
        
    }

    
    default void afterExecute(Runnable runnable, Throwable throwable) {
        
    }
    
}
```

可以看到，这个 ThreadPoolPlugin 接口中定义的是 beforeExecute 和 afterExecute 方法，这两个方法主要是为 ThreadPoolExecutor 的 runnableTask.run() 方法做功能扩展，**可现在我想为 ThreadPoolExecutor 的 shutdown 方法实现功能扩展，之后还要为 execute 方法实现功能扩展，还有别忘了拒绝策略，我也想为拒绝策略实现功能扩展。所以，现在的问题是，程序内部的功能扩展插件类型太单一了，我不希望所有方法的功能扩展插件都使用同一个接口，希望能从类别上将插件对象做一下区分**。那我该怎么办呢？很简单，多定义几个插件接口就行了。首先 ThreadPoolPlugin 接口不用变，仍然是这个名字，只不过这个接口中先不定义方法了(**实际上在源码中和我提供的第一版本代码中这个接口并不是空的，也定义了一些方法，只不过这些方法和动态线程池本身有关，比如获得线程池的 Id，但现在我还没有为大家引入动态线程池这个类，所以就先让 ThreadPoolPlugin 接口空着**)，接下来我就可以在该接口下面继续定义一些接口，也就是说，我下面要定义的接口都继承了 ThreadPoolPlugin 接口，整个插件体系开始从 ThreadPoolPlugin 接口向下细分而已。

我之前为 ThreadPoolExecutor 的 runnableTask.run() 方法做的功能扩展就要重构一下了，**现在我重新定义了一个插件接口，名字为 ExecuteAwarePlugin，这个接口实现了 ThreadPoolPlugin，该接口就是用来对 ThreadPoolExecutor 的 runnableTask.run() 方法做功能扩展的**。具体实现请看下面代码块。

```
public interface ExecuteAwarePlugin extends ThreadPoolPlugin {

   
    default void beforeExecute(Thread thread, Runnable runnable) {
        
    }

    
    default void afterExecute(Runnable runnable, Throwable throwable) {
        
    }
}
```

如果我想定义一个插件对象，计算线程池执行任务的耗时，并且判断是否要给用户发送任务超时告警通知，那我就可以创建 ExecuteAwarePlugin 接口的实现类对象。所以之前的 TaskTimerPlugin 类就该继承 ExecuteAwarePlugin 接口了，具体实现请看下面代码块。

```
//计算任务执行耗时，并且判断是否超时的插件
public abstract class TaskTimerPlugin implements ExecuteAwarePlugin {


    private final ThreadLocal<Long> startTimes = new ThreadLocal<>();

    //在这里定义一个成员变量，代表任务超时时间
    private Long executeTimeOut;


    //构造方法
    public TaskTimerPlugin(Long executeTimeOut){
        this.executeTimeOut = executeTimeOut;    
    }
    

    //任务开始执行之前会执行这个方法，在这个方法中把任务开始执行时间放到线程本地map中
    @Override
    public final void beforeExecute(Thread thread, Runnable runnable) {
        startTimes.set(currentTime());
    }


    //该方法会在任务执行之后被调用
    @Override
    public final void afterExecute(Runnable runnable, Throwable throwable) {
        try {
            //从线程本地map中得到任务的开始时间
            Optional.ofNullable(startTimes.get())
                    //计算出耗时时间
                    .map(startTime -> currentTime() - startTime)
                    //交给processTaskTime方法处理
                    .ifPresent(this::processTaskTime);
        } finally {
            //清除线程本地map
            startTimes.remove();
        }
    }

   
    //得到当前时间的方法
    protected long currentTime() {
        return SystemClock.now();
    }


    //设置任务执行超时时间
    public void setExecuteTimeOut(Long executeTimeOut){
        this.executeTimeOut = executeTimeOut;
    }


    //该方法作为一个拓展方法，可以在该方法中把得到的任务耗时信息上报到监控系统
    //也可以在该方法中判断任务执行是否超时，如果超时就通知用户
    protected void processTaskTime(long taskExecuteTime) {

        //判断任务执行耗时是否超时了，没超时则直接返回
        if (taskExecuteTime <= executeTimeOut) {
            return;
        }

        //超时了则向用户报警
        System.out.println("通知用户任务执行超时了！");
       
    }
}
```

**如果我要为 shutdown、shutdownNow 方法实现功能扩展，那我就可以再定义一个 ShutdownAwarePlugin 接口，该接口中定义的方法就会在 shutdown、shutdownNow 方法执行前后被调用**，具体实现请看下面代码块。

```
//该接口继承了ThreadPoolPlugin接口
public interface ShutdownAwarePlugin extends ThreadPoolPlugin {

    
    default void beforeShutdown(ThreadPoolExecutor executor) {
    }

    
    default void afterShutdown(ThreadPoolExecutor executor, List<Runnable> remainingTasks) {
    }

}
```

假如用户想要对 shutdown、shutdownNow 方法做功能扩展，就可以自己定义 ShutdownAwarePlugin 接口的实现类对象。**那如果我想为线程池的拒绝策略做功能扩展，我就可以定义一个 RejectedAwarePlugin 接口**，该接口具体实现如下，请看下面代码块。

```
//该接口继承了ThreadPoolPlugin接口
public interface RejectedAwarePlugin extends ThreadPoolPlugin {

    //这个接口中只有一个方法，该方法会在拒绝策略方法执行之前被回调
    default void beforeRejectedExecution(Runnable runnable, ThreadPoolExecutor executor) {
        
    }
}
```

到此为止，想必大家对整个 ThreadPoolPlugin 体系都已经清楚了，所以其余的功能扩展接口我就不一一展示了，在我提供的第一版本代码中引入了所有功能扩展接口，大家可以仔细查看，**并且我想着重提一个功能扩展接口，那就是专门对 ThreadPoolExecutor 的 execute 方法做功能扩展的接口，也就是 TaskAwarePlugin 接口，这个接口以及 TaskDecorator 装饰器对象提供的扩展功能很有意思，代码中注释很详细，大家可以先混个眼熟，因为这个接口的具体作用需要结合测试类才能了解，但是我在第五版本代码才提供了与其对应的测试类，到时候大家别忘了去看一下**。至于其他的功能扩展接口，大家也在第一版本代码中简单看看即可，逻辑都很简单，当然，简单的前提是你已经掌握了 ThreadPoolExecutor 线程池的知识，如果对 ThreadPoolExecutor 不太熟悉，可以先学习一下 ThreadPoolExecutor 的知识。

好了，现在功能扩展接口，也就是 ThreadPoolPlugin 体系已经重构完毕了，那怎么才能让这些方法和 ThreadPoolExecutor 中的方法产生联系呢？毕竟 ThreadPoolExecutor 并没有为其他方法预留扩展方法呀？**这时候就又要轮到 ExtensibleThreadPoolExecutor 类登场了，同时大家也比忘了我们之前定义的 ThreadPoolPluginManager 这个插件管理器**。之前我们定义 ThreadPoolPluginManager 插件管理器时，这个管理器中的内容非常简单，只有一个成员变量用来存放所有的 ThreadPoolPlugin 插件对象。但现在我们已经对不同功能的扩展做了细分，引入了很多插件类型，所以 ThreadPoolPluginManager 类中就不能再使用一个成员变量存放所有插件对象了。我把之前 ThreadPoolPluginManager 插件管理器中的内容展示在下面代码块中了，请看下面代码块。

```
//线程池插件管理器，和线程池有关的所有插件都会注册到这个插件管理器中，每一个线程池都持有一个插件管理器对象
//这个类的逻辑非常简单，就是向集合中添加数据移除数据的操作，大家自己看看就成
public class ThreadPoolPluginManager {

    //存放线程池插件对象的集合
    private final List<ThreadPoolPlugin> threadPoolPluginList = new CopyOnWriteArrayList<>();


    //注册插件对象到集合中的方法
    public void register(ThreadPoolPlugin plugin) {

         threadPoolPluginList.add(plugin);
         
    }


    //得到所有插件的方法
    public Collection<ThreadPoolPlugin> getThreadPoolPluginList() {

        return threadPoolPluginList;
    }


    //其他方法省略
    
}
```

而引入多个插件类型之后，ThreadPoolPluginManager 需要重构成下面这样，请看下面代码块。

```
//线程池插件管理器，和线程池有关的所有插件都会注册到这个插件管理器中，每一个线程池都持有一个插件管理器对象
//这个类的逻辑非常简单，就是向集合中添加数据移除数据的操作，大家自己看看就成
public class ThreadPoolPluginManager {



    //存放程序中所有插件对象的集合，这里面的key就是插件类型的字符串名称，value就是插件本身
    private final Map<String, ThreadPoolPlugin> registeredPlugins = new ConcurrentHashMap<>(16);

    //存放runnableTask.run()方法功能扩展插件对象的集合
    private final List<ExecuteAwarePlugin> executeAwarePluginList = new CopyOnWriteArrayList<>();

    //存放拒绝策略插件对象的集合
    private final List<RejectedAwarePlugin> rejectedAwarePluginList = new CopyOnWriteArrayList<>();

    //存放shutdown、shutdownNow方法功能扩展插件对象的集合
    private final List<ShutdownAwarePlugin> shutdownAwarePluginList = new CopyOnWriteArrayList<>();



    //注册插件对象到集合中的方法
    public void register(@NonNull ThreadPoolPlugin plugin) {
        //这里的getId，就是得到具体的插件对象的字符串名称，这个方法我并没有在文章中展示出来，大家可以去第一版本代码中查看
        //就是返回一个字符串名称而已
        String id = plugin.getId();
        registeredPlugins.put(id, plugin);
        //判断插件的具体类型，然后添加到对应的集合中
        if (plugin instanceof ExecuteAwarePlugin) {
            executeAwarePluginList.add((ExecuteAwarePlugin) plugin);
        }
        if (plugin instanceof RejectedAwarePlugin) {
            rejectedAwarePluginList.add((RejectedAwarePlugin) plugin);
        }
        if (plugin instanceof ShutdownAwarePlugin) {
            shutdownAwarePluginList.add((ShutdownAwarePlugin) plugin);
        }
    }



    //得到runnableTask.run()方法功能扩展插件对象的集合
    public Collection<ExecuteAwarePlugin> getExecuteAwarePluginList() {
      
        return executeAwarePluginList; 
    }


    //得到拒绝策略插件对象的集合
    public Collection<RejectedAwarePlugin> getRejectedAwarePluginList() {
        
        return rejectedAwarePluginList; 
    }


    //得到shutdown、shutdownNow方法功能扩展插件对象的集合
    public Collection<ShutdownAwarePlugin> getShutdownAwarePluginList() {
        
       return shutdownAwarePluginList; 
    }
    

    //其他方法省略
    
}
```

到此为止，ThreadPoolPluginManager 就重构得差不多了，接下来，就要让这些方法和 ThreadPoolExecutor 中的同名方法产生联系了，比如让 ThreadPoolExecutor 执行 shutdown 方法前，先执行对应功能扩展插件中的前置方法，执行完毕 shutdown 方法之后，再执行功能扩展插件中的后置方法。但是 ThreadPoolExecutor 并没有为这些方法预留扩展方法，那该怎么做呢？这就更简单了，仍然使用之前我们定义好的 ExtensibleThreadPoolExecutor 类即可。

**这个 ExtensibleThreadPoolExecutor 类继承了 ThreadPoolExecutor，只要 ExtensibleThreadPoolExecutor 类重写了 ThreadPoolExecutor 中的同名方法，然后在子类中调用父类方法即可，也就是 super 一下。同时在 super 目标方法前后执行功能扩展插件中的方法即可**。这样一来，ExtensibleThreadPoolExecutor 就可以重构成下面这样，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：扩展线程池类，这个类为jdk的原生线程池提供了非常多的扩展点，基本上每一个重要操作都提供了拓展点
 */
public class ExtensibleThreadPoolExecutor extends ThreadPoolExecutor{

    //插件管理器对象，当前线程池用到的所有插件都会注册到这个管理器中
    @Getter
    private final ThreadPoolPluginManager threadPoolPluginManager;


    //构造方法
    public ExtensibleThreadPoolExecutor(ThreadPoolPluginManager threadPoolPluginManager, int corePoolSize, int maximumPoolSize,long keepAliveTime, TimeUnit unit, BlockingQueue<Runnable> workQueue, ThreadFactory threadFactory, RejectedExecutionHandler handler) {
        super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue, threadFactory, handler);
        this.threadPoolPluginManager = threadPoolPluginManager;
    }
    
    //该方法会在线程执行任务之前被调用
    @Override
    protected void beforeExecute(Thread thread, Runnable runnable) {
        //这里从插件处理器中得到了ExecuteAwarePlugin类型的所有插件
        Collection<ExecuteAwarePlugin> executeAwarePluginList = threadPoolPluginManager.getExecuteAwarePluginList();
        //执行了插件对象中的beforeExecute方法
        executeAwarePluginList.forEach(aware -> aware.beforeExecute(thread, runnable));
    }



    //该方法会在任务执行完毕之后被调用
    @Override
    protected void afterExecute(Runnable runnable, Throwable throwable) {
        //这里从插件处理器中再次得到了ExecuteAwarePlugin类型的所有插件
        Collection<ExecuteAwarePlugin> executeAwarePluginList = threadPoolPluginManager.getExecuteAwarePluginList();
        //执行插件对象中的afterExecute方法
        executeAwarePluginList.forEach(aware -> aware.afterExecute(runnable, throwable));
    }



    //该方法会在线程池被关闭的时候调用
    @Override
    public void shutdown() {
        //得到ShutdownAwarePlugin类型的插件
        Collection<ShutdownAwarePlugin> shutdownAwarePluginList = threadPoolPluginManager.getShutdownAwarePluginList();
        //执行插件中的前置扩展方法
        shutdownAwarePluginList.forEach(aware -> aware.beforeShutdown(this));
        //执行目标方法
        super.shutdown();
        //执行插件中的后置扩展方法
        shutdownAwarePluginList.forEach(aware -> aware.afterShutdown(this, Collections.emptyList()));
    }


    //同上，只不过这个方法是立即关闭线程池
    @Override
    public List<Runnable> shutdownNow() {
        Collection<ShutdownAwarePlugin> shutdownAwarePluginList = threadPoolPluginManager.getShutdownAwarePluginList();
        //执行插件中的前置方法
        shutdownAwarePluginList.forEach(aware -> aware.beforeShutdown(this));
        //这里执行的就是立即停止线程池工作的方法，该方法会把还未执行的任务封装到list中返回给用户
        List<Runnable> tasks = super.shutdownNow();
        //执行插件中的后置扩展方法
        shutdownAwarePluginList.forEach(aware -> aware.afterShutdown(this, tasks));
        return tasks;
    }


    //这个也是线程池中的一个拓展方法，在ThreadPoolExecutor中并没有实现，用户可以自己实现这个方法
    //该方法会在线程池转变为TERMINATED状态时被调用
    @Override
    protected void terminated() {
        super.terminated();
        Collection<ShutdownAwarePlugin> shutdownAwarePluginList = threadPoolPluginManager.getShutdownAwarePluginList();
        shutdownAwarePluginList.forEach(aware -> aware.afterTerminated(this));
    }
    
}
```

上面代码块中的逻辑可以说是一目了然，注释也很详细，我就不再解释了。总之，看到这里，我就为大家把插件体系展示得比较完整了。当然，这里我还要多解释一句，我在文章中展示的 ExtensibleThreadPoolExecutor 类的代码并不完整，比如对 execute 方法、newTaskFor 方法、拒绝策略功能扩展等等都没有展示。在我为大家提供的第一版本代码中，ExtensibleThreadPoolExecutor 类中的代码是完整的，和源码一致。我之所以没有在文章中把 ExtensibleThreadPoolExecutor 类中的代码完全展示出来，是因为我已经讲完了插件体系的核心知识，大家凭借这些知识足以去我提供的代码或者源码中自行掌握剩余的几个功能扩展方法。正如我在本套课程一开始说的那样，这套课程非常简单，没必要在逻辑相同的方法上耗费过多时间，我相信大家看明白文章后，绝对可以看懂我提供的代码和源码。

如果要再进一步拓展的话，其实就可以和 SpringBoot 结合起来了。我忽然提到 SpringBoot，是因为在之前讲解的内容中，我一直跟大家强调，用户可以自己定义功能扩展插件，然后在程序中使用，同时动态线程池框架中还内置了一些功能扩展插件，比如线程池执行任务耗时功能插件就是内置的，只要用户使用了动态线程池框架，动态线程池框架就会自动在内部为用户收集任务执行信息。那么用户自己定义的功能扩展插件怎么交给动态线程池使用呢？用户如何定义功能扩展插件，我们已经掌握了，但是怎么把功能扩展插件交给动态线程池使用我还没有为大家讲解。当然，之前我也给大家提供了一个测试类，在测试类中手动把插件注册到插件管理器中，然后把插件管理器交给 ExtensibleThreadPoolExecutor 对象使用了，但是在真正开发中，我们不可能真的让用户手动处理这一切，让动态线程池帮用户处理好这一切才是最优雅的方法。这时候就要轮到 SpringBoot 登场了，或者是使用 SPI，这些也都是 nacos 中的套路。实际上，这应该就是很多框架通用的套路。我在此罗嗦了这么多，并不是立刻就要为大家引入 SpringBoot，这些都是后面要实现的功能了，我只是想先让大家对动态线程池某些功能的实现有一个大概思路而已。好了，不管怎么说，我们终于稍微前进了一步，动态线程池框架的雏形终于完成了。

目前 ExtensibleThreadPoolExecutor 类、以及功能扩展插件体系已经被我们重构完整了，那么接下来应该做什么呢？如果大家一时间没有头绪，接下来就让我为大家梳理一下思路。我们最终要开发的是一个动态线程池框架，框架开发完毕后，用户就可以直接在业务中使用这个框架。使用的方式也很简单，**只要用户在业务中创建了一个 ExtensibleThreadPoolExecutor 线程池对象，并且使用这个线程池对象执行任务，那么动态线程池框架就会在线程池运行的过程中一直收集线程池的运行信息，并且把这些信息展示给用户，用户可以根据这些信息随时动态调控 ExtensibleThreadPoolExecutor 线程池的配置**。这就是我们要开发的动态线程池的大概功能。当然，随着这些功能的确定，这个动态线程池的内部组件也能确定下来了。

**首先我们需要一个客户端，然后再开发一个对应的服务端；客户端被集成在用户开发的项目中，只要用户在项目中创建了线程池，客户端就会把线程池的配置信息上报到服务端，并且会在线程池运行期间一直收集线程池的运行信息，把这些信息也上报给服务端；最后还需要开发一个前端组件，也就是 web 页面，这个页面会访问服务端，把线程池的运行信息和配置信息展示给用户；用户根据这些信息可以随时动态调整线程池的配置，调整的信息会发送给服务端，服务端判断线程池的配置发生了变化，就会通知客户端修改线程池配置信息**。到此为止，动态线程池内部组件的工作流程我就为大家讲解完毕了。当然这其中有很多细节，这些细节知识都会在后面的文章中为大家一一讲解。

按照动态线程池各个组件的难易程度，我决定先开发动态线程池的客户端。刚才我也说了，只要用户在项目中创建了 ExtensibleThreadPoolExecutor 线程池对象，客户端就会将这个线程池的配置信息以及运行时信息上报到服务端，而在这一章，我们已经把 ExtensibleThreadPoolExecutor 类给重构完毕了，所以紧接着开发客户端，直接就可以使用这个 ExtensibleThreadPoolExecutor 类。不过开发客户端的内容，这一章显然是讲不完了，就留到下一章再为大家讲解吧。好了朋友们，我们下一章见！