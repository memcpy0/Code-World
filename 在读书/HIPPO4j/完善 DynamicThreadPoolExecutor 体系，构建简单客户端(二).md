在上一章的末尾，我已经决定为我们的动态线程池框架开发一个客户端，但因为每一章篇幅有限，所以就把客户端的开发与实现放到了本章来完成。当然，开发一个动态线程池的客户端其实也没什么难度，如果只关注核心功能的话，这个动态线程池客户端需要提供收集线程池运行信息，并且把这些运行信息和线程池本身的配置信息上报到服务端的功能，也可以根据服务端线程池配置信息的变化及时更新客户端本地使用的线程池的信息。简而言之，客户端具备的核心功能有以下三点：

**1 能够收集线程池运行时的工作信息，把信息上报给服务端**

**2 把用户在项目中创建的线程池的配置信息上报给服务端，其实就是注册线程池服务实例到服务端**

**3 根据服务端线程池配置信息的变化，及时更新客户端本地线程池的信息**

接下来，我就要从这三点着手，为大家开发出动态线程池的客户端。不过在真正开发客户端之前，我想先跟大家探讨一下功能的设计思路；当我们在业务项目中继承了动态线程池框架的客户端之后，如果想在业务中使用线程池，并且是可以动态更新配置的线程池，那么我们就可以创建一个 ExtensibleThreadPoolExecutor 线程池对象。这个 ExtensibleThreadPoolExecutor 对象大家应该没有忘记吧？这个 ExtensibleThreadPoolExecutor 类和插件体系如今都是客户端的一部分了。本来直接使用 ExtensibleThreadPoolExecutor 类也没什么关系，但目前我开发的是一个动态线程池框架，为了让用户使用这个框架的时候更方便，更直接，**我其实可以在动态线程池框架中再定义一个新的类，就叫做 DynamicThreadPoolExecutor，并且让这个类继承 ExtensibleThreadPoolExecutor。DynamicThreadPoolExecutor 就代表线程池可动态更新的意思，这样一来，用户只要想在业务中使用可动态更新的线程池，直接创建 DynamicThreadPoolExecutor 线程池对象即可**。

**当然，为了和 DynamicThreadPoolExecutor 配合，我其实还可以再定义一个 @DynamicThreadPool 注解，当我们要创建一个动态线程池对象的时候，就可以把注解添加在动态线程池对象上面。接下来就很简单了，我们只需要在程序启动的过程中，判断已经创建好的对象是否属于 DynamicThreadPoolExecutor 类型，并且对象上是否存在 @DynamicThreadPool 注解，如果这两个条件都满足，那程序就可以断定当前对象就是一个动态线程池对象，它的配置信息需要上报给服务端，也就是注册到服务端**。这样一来，我们就实现了动态线程池客户端核心功能中的第二个功能。好了，接下来我就从代码层面上为大家实现这个功能，至于其他的两个功能，后面很快就会为大家实现。

## 引入 DynamicThreadPoolExecutor，完善动态线程池

  

现在我们要实现的客户端功能是：**把用户在项目中创建的线程池的配置信息上报给服务端，其实就是注册线程池服务实例到服务端**。并且刚才我也为大家分析了这个功能的实现思路，**首先就是创建一个线程池，如果用户想使用动态线程池，那么直接创建 DynamicThreadPoolExecutor 动态线程池，并且在该线程池对象上添加 @DynamicThreadPool 注解即可**。接下来，我就先为大家把 DynamicThreadPoolExecutor 类引入到动态线程池框架中。请看下面代码块(这里我多说一句，因为 hippo4j 这个框架模块和包比较多，所以接下来我会在每个代码块中把代码块中的类所属的包都展示出来，大家根据这个信息直接去我提供的代码中查看代码即可。)。

```
package cn.hippo4j.core.executor;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：动态线程池类
 */
@Slf4j
public class DynamicThreadPoolExecutor extends ExtensibleThreadPoolExecutor implements DisposableBean {

    //关闭线程池时是否等待任务执行完毕
    @Getter
    @Setter
    public boolean waitForTasksToCompleteOnShutdown;

    //构造方法
    //这个构造方法中有两个参数，一个是executeTimeOut，这个就是用户设置的任务执行的超时时间，这个参数该怎么发挥作用呢？
    //另一个是awaitTerminationMillis，代表线程池关闭时，等待剩余任务执行的最大时间，那这个参数又该怎么发挥作用呢？
    public DynamicThreadPoolExecutor(
                                     int corePoolSize, int maximumPoolSize,
                                     long keepAliveTime, TimeUnit unit,
                                     long executeTimeOut, boolean waitForTasksToCompleteOnShutdown, long awaitTerminationMillis,
                                     @NonNull BlockingQueue<Runnable> blockingQueue,
                                     @NonNull String threadPoolId,
                                     @NonNull ThreadFactory threadFactory,
                                     @NonNull RejectedExecutionHandler rejectedExecutionHandler) {
        super(//在这里创建了默认的插件管理器DefaultThreadPoolPluginManager对象
                threadPoolId, new DefaultThreadPoolPluginManager(),
                corePoolSize, maximumPoolSize, keepAliveTime, unit,
                blockingQueue, threadFactory, rejectedExecutionHandler);
        log.info("Initializing ExecutorService {}", threadPoolId);
        this.waitForTasksToCompleteOnShutdown = waitForTasksToCompleteOnShutdown;
    }


    //销毁动态线程池的方法
    @Override
    public void destroy() {
        //如果等待任务执行完毕再终止线程池工作，就调用shutdown方法
        if (isWaitForTasksToCompleteOnShutdown()) {
            super.shutdown();
        } else {
            super.shutdownNow();
        }//在这里清空插件管理器中的插件
        getThreadPoolPluginManager().clear();
    }

    //其他方法逻辑都非常简单，都是和插件有关的set和get方法，大家去我提供的代码中简单看看即可
}
```

在上面的代码块中，可以看到，我们新引入的这个 DynamicThreadPoolExecutor 类其实非常简单，仅仅是继承了 ExtensibleThreadPoolExecutor 而已。**当然，在 DynamicThreadPoolExecutor 类中也不是没有值得关注的信息，在上面代码块的第 29 行，可以看到向父类构造方法传入了一个 threadPoolId 参数，threadPoolId 就是动态线程池的 Id。每一个动态线程池都有自己的 Id，用来和其他动态线程池做区分**。但是在上一章，我为大家重构完整的 ExtensibleThreadPoolExecutor 类中，并没有一个 threadPoolId 成员变量，所以接下来我还要再次简单重构一下 ExtensibleThreadPoolExecutor 类，为 ExtensibleThreadPoolExecutor 类补全 threadPoolId 成员变量。请看下面代码块。

```
package cn.hippo4j.core.executor;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：扩展线程池类，这个类为jdk的原生线程池提供了非常多的扩展点，基本上每一个重要操作都提供了拓展点
 */
public class ExtensibleThreadPoolExecutor extends ThreadPoolExecutor{


    //线程池Id
    @Getter
    private final String threadPoolId;

    //插件管理器对象，当前线程池用到的所有插件都会注册到这个管理器中
    @Getter
    private final ThreadPoolPluginManager threadPoolPluginManager;


    //构造方法
    public ExtensibleThreadPoolExecutor(
            @NonNull String threadPoolId,
            @NonNull ThreadPoolPluginManager threadPoolPluginManager,
            int corePoolSize, int maximumPoolSize,
            long keepAliveTime, TimeUnit unit,
            @NonNull BlockingQueue<Runnable> workQueue,
            @NonNull ThreadFactory threadFactory,
            @NonNull RejectedExecutionHandler handler) {
        super(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue, threadFactory, handler);
        //给线程池Id赋值
        this.threadPoolId = threadPoolId;
        this.threadPoolPluginManager = threadPoolPluginManager;
    }
    

    //其余方法省略
    
}
```

现在，ExtensibleThreadPoolExecutor 类最后一点空缺终于被我们填上了。到此为止，DynamicThreadPoolExecutor 的内容就已经展示得差不多了，接下来就该真正实现创建线程池的功能了。但是这个还不着急，让我再来跟大家探讨一个问题。还记得上一章我刚刚为大家引入插件体系的时候，我是不是为大家提供了一个测试类，具体代码如下。

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

在上面这个测试类中，我创建了一个计算任务执行耗时的插件对象，又创建了插件管理器对象，把功能扩展插件手动注册到了插件管理器中，接着创建了 ExtensibleThreadPoolExecutor 动态线程池，然后手动把插件管理器交给线程池使用。这只是一个简单的测试例子，只是为了让大家看清插件体系是怎么工作的。但现在我们已经在开发动态线程池框架了，难道还要让用户手动注册一些功能扩展插件吗？这显然不够优雅。**而且我在上一章也跟大家简单说过，我们开发的这个动态线程池框架，本身就应该内置一些功能扩展插件，同时也应该提供让用户自定义功能扩展插件的功能**，**而且，不管是用户自己定义的功能扩展插件，还是动态线程池框架内置的功能扩展插件，最后都应该注册到插件管理器中，而动态线程池又持有了插件管理器，这样一来，不管是用户自己定义的插件还是框架内置的插件，最后都可以被动态线程池使用**。

现在我们先不必考虑怎么把用户自己定义的功能扩展插件交给动态线程池使用，让我们先来想想，动态线程池内置的一些插件，怎么自动交给动态线程池使用？要想解决这个问题，就要该先明确，要给动态线程池框架内置哪些功能扩展插件。如果是这个问题的话，那就简单多了，**计算线程池执行任务耗时的功能，对 ThreadPoolExecutor 的 Shutdown、shutdownNow、execute 等等方法的扩展功能，这些都可以定义为动态线程池内置的插件**。那这些插件要怎么交给动态线程池使用呢？现在我已经为动态线程池框架引入了 DynamicThreadPoolExecutor 类，用户最终要创建的动态线程池就是 DynamicThreadPoolExecutor 对象，我目前的想法是，**当用户创建好 DynamicThreadPoolExecutor 对象的时候，就立刻把动态线程池框架内置的插件都交给动态线程池使用，而这个操作就在 DynamicThreadPoolExecutor 类的构造方法中执行**。这样一来，我就该对动态线程池这个类再进行一番重构。

首先我想为动态线程池框架引入一个新的类，那就是 DefaultThreadPoolPluginRegistrar 类，并且在这个类中定义一个 doRegister 方法，方法参数为 DynamicThreadPoolExecutor 对象。当调用 DefaultThreadPoolPluginRegistrar 对象的 doRegister 方法时，就会把动态线程池内置的各个功能扩展插件对象都交给 DynamicThreadPoolExecutor 动态线程池使用。DefaultThreadPoolPluginRegistrar 类的具体实现请看下面代码块。

```
package cn.hippo4j.core.plugin.manager;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/28
 * @方法描述：默认的线程池插件注册器，通过这个注册器对象，把线程池的所有插件都注册到线程池的插件管理器中
 */
@NoArgsConstructor
@AllArgsConstructor
public class DefaultThreadPoolPluginRegistrar implements ThreadPoolPluginRegistrar {

    //这个就是用户设置的任务执行的超时时间
    private long executeTimeOut;

    //线程池关闭时，等待剩余任务执行的最大时间
    private long awaitTerminationMillis;


    //这个就是把动态线程池各个内置的功能扩展插件注册到动态线程池的内部的插件管理器成员变量中的方法
    //下面这5个功能扩展插件对象我就不在文章中展示了，我提供的第一版本代码中，每一个插件对象类中都有注释
    //大家可以直接去代码中查看内置插件的作用
    @Override
    public void doRegister(DynamicThreadPoolExecutor support) {
        //这里调用了DynamicThreadPoolExecutor对象的register方法，但是在我们目前的程序中
        //动态线程池中根本没有register方法，所以还需要在动态线程池中再添加一个register方法
        //具体实现我就写在当前这个代码块中了，请大家继续向下看
        support.register(new TaskDecoratorPlugin());
        //executeTimeOut就是执行超时时间
        support.register(new TaskTimeoutNotifyAlarmPlugin(support.getThreadPoolId(), executeTimeOut, support.getThreadPoolExecutor()));
        support.register(new TaskRejectCountRecordPlugin());
        support.register(new TaskRejectNotifyAlarmPlugin());
        //awaitTerminationMillis就是等待剩余任务执行的最大时间，这两个参数是怎么被赋值的呢？
        support.register(new ThreadPoolExecutorShutdownPlugin(awaitTerminationMillis));
    }
}






//重构后的DynamicThreadPoolExecutor
package cn.hippo4j.core.executor;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：动态线程池类
 */
@Slf4j
public class DynamicThreadPoolExecutor extends ExtensibleThreadPoolExecutor implements DisposableBean {

    //关闭线程池时是否等待任务执行完毕
    @Getter
    @Setter
    public boolean waitForTasksToCompleteOnShutdown;

    //构造方法
    //这个构造方法中有两个参数，一个是executeTimeOut，这个就是用户设置的任务执行的超时时间，这个参数该怎么发挥作用呢？
    //另一个是awaitTerminationMillis，代表线程池关闭时，等待剩余任务执行的最大时间，那这个参数又该怎么发挥作用呢？
    public DynamicThreadPoolExecutor(
                                     int corePoolSize, int maximumPoolSize,
                                     long keepAliveTime, TimeUnit unit,
                                     long executeTimeOut, boolean waitForTasksToCompleteOnShutdown, long awaitTerminationMillis,
                                     @NonNull BlockingQueue<Runnable> blockingQueue,
                                     @NonNull String threadPoolId,
                                     @NonNull ThreadFactory threadFactory,
                                     @NonNull RejectedExecutionHandler rejectedExecutionHandler) {
        super(//在这里创建了默认的插件管理器DefaultThreadPoolPluginManager对象
                threadPoolId, new DefaultThreadPoolPluginManager(),
                corePoolSize, maximumPoolSize, keepAliveTime, unit,
                blockingQueue, threadFactory, rejectedExecutionHandler);
        log.info("Initializing ExecutorService {}", threadPoolId);
        this.waitForTasksToCompleteOnShutdown = waitForTasksToCompleteOnShutdown;
         //注意，这里有一行非常重要的代码，这里的操作非常重要，就是在这里，把所有插件都注册到当前动态线程池对象的插件管理器中了
        //这时候，executeTimeOut和awaitTerminationMillis参数也就可以发挥作用了
        new DefaultThreadPoolPluginRegistrar(executeTimeOut, awaitTerminationMillis).doRegister(this);
    }


    //销毁动态线程池的方法
    @Override
    public void destroy() {
        //如果等待任务执行完毕再终止线程池工作，就调用shutdown方法
        if (isWaitForTasksToCompleteOnShutdown()) {
            super.shutdown();
        } else {
            super.shutdownNow();
        }//在这里清空插件管理器中的插件
        getThreadPoolPluginManager().clear();
    }


    public void register(ThreadPoolPlugin plugin) {
        //动态线程池持有了插件管理器，这个插件管理器是定义在ExtensibleThreadPoolExecutor类中的
        //并且使用了lombok的@Getter注解，所以就可以直接调用getThreadPoolPluginManager()方法得到插件管理器
        //然后把插件注册到插件管理器中即可
        getThreadPoolPluginManager().register(plugin);
    }

    
    //其他方法逻辑都非常简单，都是和插件有关的set和get方法，大家去我提供的代码中简单看看即可
}
```

当上面的代码块展示完毕之后，大家应该也就清楚了，**我又引入了一个新的 DefaultThreadPoolPluginRegistrar 类，这个类的对象会在 DynamicThreadPoolExecutor 动态线程池对象创建的时候，也就是在 DynamicThreadPoolExecutor 的构造方法中被创建，然后调用该对象的 doRegister() 方法，把动态线程池框架内置的各个功能插件注册到动态线程池内部**。这样一来，动态线程池内置的各个功能扩展插件就可以发挥作用了(不过我这里还要多补充一下，**实际上，在我提供的第一版本代码和源码中，register() 并不是直接定义在 DynamicThreadPoolExecutor 类中的，而是引入了一个新的 ThreadPoolPluginSupport 接口，在接口中定义了 register() 注册方法，然后又让动态线程池实现了这个接口而已。在文章中我就不展示全貌了，如果我全讲了，就相当于我为大家把 1+1=2 重复讲解了无数次，我想到时候大家也会觉得我讲得太罗嗦**，希望大家能够理解)。

好了，关于自动注册动态线程池内置插件的功能我也就为大家实现完毕了，这算是一个小插曲吧。接下来让我们言归正传，真正实现 DynamicThreadPoolExecutor 动态线程池的创建吧。因为到目前为止，我们也只是把动态线程池的类定义完毕了，还没有真正创建 DynamicThreadPoolExecutor 动态线程池。

## 引入 ThreadPoolBuilder，创建动态线程池

其实创建 DynamicThreadPoolExecutor 的功能非常简单，我本来不想使用一个小节来单独实现这个功能，但是上一小节的内容已经比较多了，所以只好使用新的小节来实现创建 DynamicThreadPoolExecutor 对象的功能。但这个功能本身没有一点新意，说到底就是创建一个线程池而已，不管是在 netty，还是 sofajraft，大家可能都见过很多次了，**创建线程池通常都是使用构建者模式，也就是使用一个 ThreadPoolBuilder 对象来创建线程池。所以，我们也可以直接在动态线程池框架中引入一个 ThreadPoolBuilder 类，用这个类的对象来创建 DynamicThreadPoolExecutor 动态线程池**。而 ThreadPoolBuilder 类的代码我也已经给大家准备好了，请看下面代码块。

```
package cn.hippo4j.core.executor.support;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：线程池构建器
 */
public class ThreadPoolBuilder implements Builder<ThreadPoolExecutor> {

    //这个表示线程池构建器是否要创建快速线程池，所谓快速线程池
    //其实就是使用的任务队列特殊一点，使用了TaskQueue任务队列
    //这个队列有一个特点，那就是只要当提交给任务队列的数量大于线程池当前线程的数量了
    //但是当前线程数量仍然小于线程池的最大线程数量，这时候就可以不把任务添加到队列中
    //而是让线程池直接创建线程执行任务，所谓快速，就快速在这里。实际上在hippo4j框架中
    //不仅提供了动态线程池，还提供了FastThreadPoolExecutor快速线程池，最后还提供了普通线程池，也就是原生的ThreadPoolExecutor。
    //这三种线程池想创建哪一种就创建哪一种，只不过只有DynamicThreadPoolExecutor动态线程池会被注册到服务单，并且可以动态更新配置信息
    //快速线程池和普通线程池我就不在文章中展示了，反正所有线程池的创建都在ThreadPoolBuilder体系下，大家掌握了DynamicThreadPoolExecutor是如何创建的
    //其他的也就都掌握了
    private boolean isFastPool;

    //表示这个线程池构建器是否要创建动态线程池
    private boolean isDynamicPool;

    //默认的线程池的核心线程数量，这个数量是根据CPU核心数量计算出来的
    private int corePoolSize = calculateCoreNum();

    //默认的池最大线程数量
    private int maxPoolSize = corePoolSize + (corePoolSize >> 1);

    //默认存活时间
    private long keepAliveTime = 30000L;

    //默认时间单位
    private TimeUnit timeUnit = TimeUnit.MILLISECONDS;

    //默认执行任务超时时间
    private long executeTimeOut = 10000L;

    //默认队列容量
    private int capacity = 512;

    //队列类型，默认使用ResizableCapacityLinkedBlockingQueue队列
    private BlockingQueueTypeEnum blockingQueueType = BlockingQueueTypeEnum.RESIZABLE_LINKED_BLOCKING_QUEUE;

    //线程池队列
    private BlockingQueue workQueue;

    //默认拒绝策略
    private RejectedExecutionHandler rejectedExecutionHandler = new ThreadPoolExecutor.AbortPolicy();

    //是否为守护线程
    private boolean isDaemon = false;

    //线程池现成的前缀名称
    private String threadNamePrefix;

    //创建线程的线程工厂
    private ThreadFactory threadFactory;

    //线程池Id
    private String threadPoolId;
    

    //线程池关闭时，默认等待剩余任务执行的最大时间
    private Long awaitTerminationMillis = 5000L;

    //线程池关闭时是否等待正在执行的任务执行完毕
    private Boolean waitForTasksToCompleteOnShutdown = true;

    //是否允许超过存活时间的核心线程终止工作
    private Boolean allowCoreThreadTimeOut = false;


    //得到线程池核心线程数量的方法
    private Integer calculateCoreNum() {
        int cpuCoreNum = Runtime.getRuntime().availableProcessors();
        return new BigDecimal(cpuCoreNum).divide(new BigDecimal("0.2")).intValue();
    }



    //以下都是一些简单的设置线程池配置参数的方法


    //设置动态线程池标志为true，意味着这个构建器对象要构建的是一个动态线程池对象
    public ThreadPoolBuilder dynamicPool() {
        this.isDynamicPool = true;
        return this;
    }

    //设置动态线程池线程名称前缀的方法
    public ThreadPoolBuilder threadFactory(String threadNamePrefix) {
        this.threadNamePrefix = threadNamePrefix;
        return this;
    }


    //设置动态线程吃线程工厂的方法
    public ThreadPoolBuilder threadFactory(ThreadFactory threadFactory) {
        this.threadFactory = threadFactory;
        return this;
    }


    //设置核心线程数量
    public ThreadPoolBuilder corePoolSize(int corePoolSize) {
        this.corePoolSize = corePoolSize;
        return this;
    }


    //设置最大线程数量
    public ThreadPoolBuilder maxPoolNum(int maxPoolSize) {
        this.maxPoolSize = maxPoolSize;
        if (maxPoolSize < this.corePoolSize) {
            this.corePoolSize = maxPoolSize;
        }
        return this;
    }



    //设置线程池执行任务超时时间
    public ThreadPoolBuilder executeTimeOut(long executeTimeOut) {
        this.executeTimeOut = executeTimeOut;
        return this;
    }


    //设置时间单位和线程最大空闲时间
    public ThreadPoolBuilder keepAliveTime(long keepAliveTime, TimeUnit timeUnit) {
        this.keepAliveTime = keepAliveTime;
        this.timeUnit = timeUnit;
        return this;
    }


    //设置线程池任务队列类型和队列容量
    public ThreadPoolBuilder workQueue(BlockingQueueTypeEnum queueType, int capacity) {
        this.blockingQueueType = queueType;
        this.capacity = capacity;
        return this;
    }


    //设置拒绝策略
    public ThreadPoolBuilder rejected(RejectedExecutionHandler rejectedExecutionHandler) {
        this.rejectedExecutionHandler = rejectedExecutionHandler;
        return this;
    }


    //设置任务队列
    public ThreadPoolBuilder workQueue(BlockingQueue workQueue) {
        this.workQueue = workQueue;
        return this;
    }


    //设置线程池Id
    public ThreadPoolBuilder threadPoolId(String threadPoolId) {
        this.threadPoolId = threadPoolId;
        return this;
    }



    //设置线程池关闭时，等待剩余任务执行的最大时间
    public ThreadPoolBuilder awaitTerminationMillis(long awaitTerminationMillis) {
        this.awaitTerminationMillis = awaitTerminationMillis;
        return this;
    }


    //设置线程池关闭时是否等待正在执行的任务执行完毕标志
    public ThreadPoolBuilder waitForTasksToCompleteOnShutdown(boolean waitForTasksToCompleteOnShutdown) {
        this.waitForTasksToCompleteOnShutdown = waitForTasksToCompleteOnShutdown;
        return this;
    }



    //设置是否允许超过存活时间的核心线程终止工作的标志
    public ThreadPoolBuilder allowCoreThreadTimeOut(boolean allowCoreThreadTimeOut) {
        this.allowCoreThreadTimeOut = allowCoreThreadTimeOut;
        return this;
    }


    //得到线程池构建器的方法
    public static ThreadPoolBuilder builder() {
        return new ThreadPoolBuilder();
    }



    //实现了Builder接口中的方法，构建动态线程池的方法
    @Override
    public ThreadPoolExecutor build() {
        //判断要创建的是否为动态线程池
        if (isDynamicPool) {
            //如果要创建的是动态线程池，就直接创建
            return buildDynamicPool(this);
        }//这里就意味着要创建的不是动态线程池，那就判断要创建的是普通线程池还是快速线程池
        return isFastPool ? buildFastPool(this) : buildPool(this);
    }



    //创建动态线程池的方法
    private static ThreadPoolExecutor buildDynamicPool(ThreadPoolBuilder builder) {
        //AbstractBuildThreadPoolTemplate.buildDynamicPool()方法中又调用了当前构建器对象的buildInitParam(builder)方法
        //buildInitParam(builder)方法会得到一个ThreadPoolInitParam线程池初始化参数对象，该对象封装了线程池需要的配置参数
        //线程池构建器模板就会使用这个参数对象创建动态线程池
        //而线程池构建器模板就是AbstractBuildThreadPoolTemplate，至于为什么是抽象的，是因为这个模板不仅可以创建动态线程池
        //还可以创建动态线程池，还有普通线程池，这个就不再过多解释了
        return AbstractBuildThreadPoolTemplate.buildDynamicPool(buildInitParam(builder));
    }



    


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/27
     * @方法描述：创建ThreadPoolInitParam对象的方法，这个对象中封装了线程池的核心参数信息，然后会使用这个ThreadPoolInitParam对象去创建真正的线程池
     */
    private static AbstractBuildThreadPoolTemplate.ThreadPoolInitParam buildInitParam(ThreadPoolBuilder builder) {
        //定义一个ThreadPoolInitParam对象
        AbstractBuildThreadPoolTemplate.ThreadPoolInitParam initParam;
        //判断线程工厂是否为空，如果线程工厂为空，就判断线程池中线程前缀是否为空
        if (builder.threadFactory == null) {
            Assert.notEmpty(builder.threadNamePrefix, "The thread name prefix cannot be empty or an empty string.");
            //如果线程前缀不为空，则直接创建ThreadPoolInitParam对象即可，这里创建ThreadPoolInitParam对象的过程中，其实就把ThreadPoolInitParam
            //对象内部的ThreadFactory成员变量创建成功了
            initParam = new AbstractBuildThreadPoolTemplate.ThreadPoolInitParam(builder.threadNamePrefix, builder.isDaemon);
        } else {
            //如果线程工厂不为空，直接创建ThreadPoolInitParam对象即可
            initParam = new AbstractBuildThreadPoolTemplate.ThreadPoolInitParam(builder.threadFactory);
        }
        //接下来就要使用刚才得到的构造器对象给initParam中的其他成员变量赋值即可
        initParam.setCorePoolNum(builder.corePoolSize)
                //设置最大线程数
                .setMaxPoolNum(builder.maxPoolSize)
                //设置线程存活时间
                .setKeepAliveTime(builder.keepAliveTime)
                //设置队列容量
                .setCapacity(builder.capacity)
                //设置任务超时时间
                .setExecuteTimeOut(builder.executeTimeOut)
                //设置拒绝策略
                .setRejectedExecutionHandler(builder.rejectedExecutionHandler)
                //设置时间单位
                .setTimeUnit(builder.timeUnit)
                //设置核心线程超过存活时间是否存活
                .setAllowCoreThreadTimeOut(builder.allowCoreThreadTimeOut)
                //设置任务装饰器
                .setTaskDecorator(builder.taskDecorator);
        //判断用户要创建的是什么线程池
        if (builder.isDynamicPool) {
            //这里创建的就是动态线程池得到线程池Id
            String threadPoolId = Optional.ofNullable(builder.threadPoolId).orElse(builder.threadNamePrefix);
            //设置线程池Id
            initParam.setThreadPoolId(threadPoolId);
            //设置线程池关闭时是否等待正在执行的任务执行完毕
            initParam.setWaitForTasksToCompleteOnShutdown(builder.waitForTasksToCompleteOnShutdown);
            //设置线程池关闭时，等待剩余任务执行的最大时间
            initParam.setAwaitTerminationMillis(builder.awaitTerminationMillis);
        }
        return initParam;
    }



    //省略其他方法
    
}
```

**当用户使用 ThreadPoolBuilder 对象构建动态线程池的时候，设置好了动态线程池的一切配置参数后，最后会调用 build() 方法构建动态线程池，经过 buildDynamicPool(ThreadPoolBuilder builder) 方法的处理之后，会得到一个封装了动态线程池所需的所有配置参数信息的 ThreadPoolInitParam 对象，然后把这个对象交给 AbstractBuildThreadPoolTemplate 线程池构建模板去构建真正的动态线程池即可**。所以，接下来的逻辑就来到了 AbstractBuildThreadPoolTemplate 类中，而这个 AbstractBuildThreadPoolTemplate 类的代码我也给大家准备好了。请看下面代码块。

```
package cn.hippo4j.core.executor.support;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/27
 * @方法描述：线程池模板类，框架中的线程池就是由这类创建的，不管是动态线程池还是快速线程池还是普通线程池，都是由这个类创建的
 */
@Slf4j
public class AbstractBuildThreadPoolTemplate {


    //省略创建快速线程池和普通线程池的方法

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/27
     * @方法描述：创建动态线程池的方法
     */
    public static DynamicThreadPoolExecutor buildDynamicPool(ThreadPoolInitParam initParam) {
        Assert.notNull(initParam);
        DynamicThreadPoolExecutor dynamicThreadPoolExecutor;
        try {
            dynamicThreadPoolExecutor = new DynamicThreadPoolExecutor(
                    initParam.getCorePoolNum(),
                    initParam.getMaxPoolNum(),
                    initParam.getKeepAliveTime(),
                    initParam.getTimeUnit(),
                    initParam.getExecuteTimeOut(),
                    initParam.getWaitForTasksToCompleteOnShutdown(),
                    initParam.getAwaitTerminationMillis(),
                    initParam.getWorkQueue(),
                    initParam.getThreadPoolId(),
                    initParam.getThreadFactory(),
                    initParam.getRejectedExecutionHandler());
        } catch (IllegalArgumentException ex) {
            throw new IllegalArgumentException(String.format("Error creating thread pool parameter. threadPool id: %s", initParam.getThreadPoolId()), ex);
        }//在这里设置了任务装饰器
        dynamicThreadPoolExecutor.setTaskDecorator(initParam.getTaskDecorator());
        dynamicThreadPoolExecutor.allowCoreThreadTimeOut(initParam.allowCoreThreadTimeOut);
        return dynamicThreadPoolExecutor;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/27
     * @方法描述：这个内部类的对象封装了线程池的核心参数，最后就是用它来创建线程池的
     */
    @Data
    @Accessors(chain = true)
    public static class ThreadPoolInitParam {

        private Integer corePoolNum;

        private Integer maxPoolNum;

        private Long keepAliveTime;

        private TimeUnit timeUnit;

        private Long executeTimeOut;

        private Integer capacity;

        private BlockingQueue<Runnable> workQueue;

        private RejectedExecutionHandler rejectedExecutionHandler;

        private ThreadFactory threadFactory;

        private String threadPoolId;

        private TaskDecorator taskDecorator;

        private Long awaitTerminationMillis;

        private Boolean waitForTasksToCompleteOnShutdown;

        private Boolean allowCoreThreadTimeOut = false;

        public ThreadPoolInitParam(String threadNamePrefix, boolean isDaemon) {
            this.threadFactory = ThreadFactoryBuilder.builder()
                    .prefix(threadNamePrefix)
                    .daemon(isDaemon)
                    .build();
        }

        public ThreadPoolInitParam(ThreadFactory threadFactory) {
            this.threadFactory = threadFactory;
        }
    }
}
```

到此为止我相信我就把创建 DynamicThreadPoolExecutor 动态线程池的功能已经实现完毕了，并且也把创建的流程展示清楚了。当然，我也可以给大家提供一个测试类，帮助大家再梳理一下 DynamicThreadPoolExecutor 创建的流程。请看下面代码块。

```
public class Test{

    public static void main(String[] args) {

        //设置线程池Id
        String threadPoolId = "test-consume";
        //构建动态线程池，因为DynamicThreadPoolExecutor动态线程池就是ThreadPoolExecutor的子类
        //所以这里使用ThreadPoolExecutor接收动态线程池对象
        ThreadPoolExecutor customExecutor = ThreadPoolBuilder.builder()
                //设置要创建动态线程池的标志
                .dynamicPool()
                //设置线程池中线程的名称前缀
                .threadFactory(threadPoolId)
                //设置线程池Id
                .threadPoolId(threadPoolId)
                //设置线程池执行任务超时时间
                .executeTimeOut(800L)
                //设置等待任务执行完毕后再关闭线程池标志
                .waitForTasksToCompleteOnShutdown(true)
                //设置等待剩余任务执行的最大时间
                .awaitTerminationMillis(5000L)
                //构建动态线程池
                .build();
    }
}  
```

当用户调用 ThreadPoolBuilder.builder() 方法得到了一个线程池构建器之后，就可以调用构建器的方法设置线程池的各个参数，如果有些参数没有设置也没有关系，因为在 ThreadPoolBuilder 对象定义了一些线程池的默认参数，如果没有设置，就会使用默认参数来创建线程池。当用户设置好线程池的各个配置参数后，就可以调用 build() 方法构建线程池对象。随着 build() 方法的调用，会继续在方法内部调用 ThreadPoolBuilder 对象的 buildDynamicPool(this) 方法。之后就会得到一个封装了线程池所需的所有配置信息的 ThreadPoolInitParam 对象，逻辑就会来到 AbstractBuildThreadPoolTemplate 线程池构建模板器中，执行真正创建动态线程池的 buildDynamicPool(ThreadPoolInitParam initParam) 方法，到此为止动态线程池就创建完毕了。我想这个逻辑还是非常简单的。

好了，现在线程池也可以创建了，接下来就该实现本章最重要的一个功能，**那就是把创建好的动态线程池的配置信息上报到服务端，其实就是把动态线程池注册到服务端，只有把客户端本地的线程池注册到服务端，服务端发现用户在 web 界面更改了某个线程池的配置信息后，才能根据线程池的信息，及时通知客户端更新本地对应线程池的配置信息啊**。所以说，把客户端线程池注册到服务端的功能是最重要的，也是本章第一小节，我要为大家实现的功能。那么这个功能该怎么实现呢？之前我跟大家分析的思路是：**当我们要创建一个 DynamicThreadPoolExecutor 动态线程池对象的时候，可以把 @DynamicThreadPool 注解添加在动态线程池对象上面。在程序启动的过程中，判断已经创建好的对象是否属于 DynamicThreadPoolExecutor 类型，并且对象上是否存在 @DynamicThreadPool 注解，如果这两个条件都满足，那程序就可以断定当前对象就是一个动态线程池对象，它的配置信息需要上报给服务端，也就是注册到服务端。**

思路非常简单，其实看到这里的时候，大家应该也可以想到了，在程序启动的过程中执行这个操作，并且是根据注解和对象类型进行判断，这不就是我们经常使用的 Spring 那一套吗？**利用 Spring 为我们提供的扩展点，也就是 BeanPostProcessor 对象处理器接口，我们就可以自己定义 bean 对象的前置和后置处理方法，只要在对象的后置处理方法中判断创建好的对象是否为 DynamicThreadPoolExecutor 类型的，并且对象上存在 @DynamicThreadPool 注解，就可以执行注册动态线程池到服务端的操作**。接下来，就让我使用 SpringBoot，为大家实现动态线程池注册服务端的功能吧。

## 创建 bean 处理器，实现动态线程池配置信息上报服务端功能

**实现的步骤非常简单，我直接在动态线程池框架中定义一个 DynamicThreadPoolPostProcessor 类，让该类实现 SpringBoot 中的 BeanPostProcessor 接口，实现 BeanPostProcessor 接口中的前置方法 postProcessBeforeInitialization 和后置方法 postProcessAfterInitialization 即可**。**当然，在我们的程序中，实际上只需要实现 postProcessAfterInitialization 后置处理方法即可。因为我们是在动态线程池对象创建完毕后，对这些对象进行判断和处理的**。下面就让我为大家把 DynamicThreadPoolPostProcessor 类定义出来吧。请看下面代码块。

```
package cn.hippo4j.springboot.start.support;


@Slf4j
@AllArgsConstructor
public final class DynamicThreadPoolPostProcessor implements BeanPostProcessor {


    //bean前置处理方法
    @Override
    public Object postProcessBeforeInitialization(Object bean, String beanName) {
        return bean;
    }



   //bean后置处理方法
    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
       //该方法暂时不做实现
    }

    
}
```

可以看到，在上面的代码块中，实际上也没有展示什么实质的内容，这是因为我忽然想到了另外的几个问题，想跟大家再探讨探讨。我想提醒大家一下，当我们引入了 SpringBoot 之后，我们要开发的这个动态线程池框架实际上就变成了一个 SpringBoot 项目，SpringBoot 中的一切功能都可以为我们自己开发的框架服务了。比如说我们可以使用 SpringBoot 的配置文件，还可以使用 SpringBoot 的自动注入，也可以使用 SpringBoot 的配置类来创建并管理对象。**我之所以忽然提到这个，是因为我意识到客户端要把线程池的信息注册到服务端，肯定需要和服务端通信吧？那么通信组件怎么实现呢？而且将来我们的这个框架可能会部署在多个项目中，也就是说可能会在服务端注册为不同项目服务的线程池实例，要怎么区分和定位这些线程池呢？既然使用了 SpringBoot，那么之后再创建动态线程池对象的时候，不就可以直接使用配置类创建对象了？这样一来动态线程池就可以直接交给 SpringBoot 容器来管理**。

如果大家能理清楚这个逻辑，察觉到某些操作的必要性，那么接下来我就要在具体实现 DynamicThreadPoolPostProcessor 类之前，再往我们的动态线程池框架中引入一些新的组件，并且是使用 SpringBoot 的功能引入的。首先为了能区分注册在服务端的多个线程池的不同实例，**我们开发的这个动态线程池框架完全可以仿照 nacos 的 namespace、Group、以及具体的 InstanceId 那样来确定一个唯一的服务实例，自己也定义 namespace、Group 以及具体的线程池实例 Id 来确定一个唯一的线程池实例**。当然，我也不想完全照搬 nacos，因此可以做一点小小的改动，**namespace 还可以定义为 namespace，Group 可以定义为 itemId**，总之，最后和 nacos 达到的效果应该是一致的。而且我们也使用 SpringBoot，所以这些配置信息完全可以定义在 SpringBoot 的配置文件中，就像下面这样，请看下面代码块。

```
//动态线程池服务端的地址
spring.dynamic.thread-pool.server-addr=http://localhost:6691
//客户端namespace
spring.dynamic.thread-pool.namespace=prescription
//itemId
spring.dynamic.thread-pool.item-id=dynamic-threadpool-example
//访问服务端时需要的用户名和密码
spring.dynamic.thread-pool.username=admin
spring.dynamic.thread-pool.password=123456
```

当然，既然在配置文件中定义了配置信息，那就要使用对应的配置对象来接收封装。所以，我要在框架中定义一个 BootstrapProperties 类，用这个类的对象来封装定义在配置文件中的数据。BootstrapProperties 类的具体实现请看下面代码块。

```
package cn.hippo4j.springboot.start.config;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/26
 * @方法描述：这个类封装的就是配置文件中的一些信息
 */
@Slf4j
@Getter
@Setter
@ConfigurationProperties(prefix = BootstrapProperties.PREFIX)
public class BootstrapProperties implements BootstrapPropertiesInterface {

    public static final String PREFIX = "spring.dynamic.thread-pool";

    //用户名
    private String username;

    //密码
    private String password;

    //服务端地址
    private String serverAddr;

    //netty服务器的端口号，这个是可配置的
    //在hippo4j框架，提供了两种通信方式，一种是http，一种就是netty
    //在该框架中默认使用的是http，所以我就不引入netty了
    private String nettyServerPort;

    //客户端上报给服务端线程池历史信息的方法，这个也可以使用netty的方式上报
    //我仍然使用内部默认的http了，不引入netty
    private String reportType;

    //命名空间
    private String namespace;

    //项目Id
    private String itemId;

    //是否启动动态线程池
    private Boolean enable = true;

    //是否在控制台打印hippo4j的启动图案
    private Boolean banner = true;

}

```

既然是要使用 BootstrapProperties 来封装配置类中的信息，**那么 BootstrapProperties 类的对象肯定要被 SpringBoot 容器来管理，所以接下来我要定义一个配置类，在这个配置类中，把 BootstrapProperties 的对象交给 SpringBoot 的容器来管理。这个配置类我也定义好了，DynamicThreadPoolAutoConfiguration**，具体实现请看下面代码块。

```
package cn.hippo4j.springboot.start.config;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/26
 * @方法描述：这个就是spring-starter中的核心类，这个类使用了大量springboot的功能，这个类上的springboot注解应该都是常用注解了，所以这些功能我就不写注释了
 */
@Configuration
@AllArgsConstructor
@ConditionalOnBean(MarkerConfiguration.Marker.class)
//这个注解会使BootstrapProperties类上的ConfigurationProperties注解生效，BootstrapProperties对象就可以被SpringBoot容器管理了
@EnableConfigurationProperties(BootstrapProperties.class)
@ConditionalOnProperty(prefix = BootstrapProperties.PREFIX, value = "enable", matchIfMissing = true, havingValue = "true")
public class DynamicThreadPoolAutoConfiguration {

    //在这里把配置文件中的相关信息封封装到这个成员变量中了
    //properties对象会被自动注入
    private final BootstrapProperties properties;


}
```

好了，现在动态线程池框架客户端的配置信息也封装好了，**接下来我还要定义一个通信组件，我打算使用 http 协议进行通信，所以就定义一个 HttpAgent 类，并且这个通信组件也可以交给 SpringBoot 容器来管理，也就是说，这个 HttpAgent 类的对象要定义在上面代码块中展示的 DynamicThreadPoolAutoConfiguration 类中，因为 DynamicThreadPoolAutoConfiguration 是一个配置类，这个类中的对象都可以交给 SpringBoot 容器来管理**。这样一来，动态线程池框架的客户端和服务端就可以通信了，具体实现请看下面代码块。

```
package cn.hippo4j.springboot.start.remote;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/26
 * @方法描述：用来访问服务端的http代理类，ServerHttpAgent实现了HttpAgent接口
 */
public class ServerHttpAgent implements HttpAgent {

    //配置信息对象
    private final BootstrapProperties dynamicThreadPoolProperties;

    //服务地址管理器，这个对象中封装着可用的服务端地址列表，当然，服务端地址的列表信息，需要用户提前定义在配置文件中
    //nacos中也有这个类
    private final ServerListManager serverListManager;

    //安全代理类，如果有朋友自己看过nacos客户端源码，肯定对这个类的作用不陌生，这就是要给安全代理类
    //主要作用就是用来访问服务端的，通过这个类从服务端获得token，以后每次访问服务端都会携带这个token
    //但是在我迭代的nacos客户端代码中，我把这个功能给省略了，如果大家感兴趣可以自己去看看nacos客户端的源码
    private SecurityProxy securityProxy;

    //定时任务执行器，这个定时任务执行器会定期刷新本地缓存的token
    private ScheduledExecutorService executorService;

    //定时任务执行间隔
    private final long securityInfoRefreshIntervalMills = TimeUnit.SECONDS.toMillis(5);

    public ServerHttpAgent(BootstrapProperties properties) {
        this.dynamicThreadPoolProperties = properties;
        this.serverListManager = new ServerListManager(dynamicThreadPoolProperties);
        this.securityProxy = new SecurityProxy(properties);
        //在这里已经先从服务端获取到了token了
        this.securityProxy.applyToken(this.serverListManager.getServerUrls());
        //创建定时任务执行器
        this.executorService = new ScheduledThreadPoolExecutor(new Integer(1),
                ThreadFactoryBuilder.builder().daemon(true).prefix("client.scheduled.token.security.updater").build());
        //向定时任务执行器提交了定期执行的任务
        this.executorService.scheduleWithFixedDelay(
                //定期访问服务端，刷新本地token
                () -> securityProxy.applyToken(serverListManager.getServerUrls()),
                0,
                securityInfoRefreshIntervalMills,
                TimeUnit.MILLISECONDS);
    }

    @Override
    public void start() {

    }


    @Override
    public String getTenantId() {
        return dynamicThreadPoolProperties.getNamespace();
    }

    @Override
    public String getEncode() {
        return null;
    }



    @Override
    public Result httpGetSimple(String path) {
        path = injectSecurityInfoByPath(path);
        return HttpUtil.get(buildUrl(path), Result.class);
    }


    //下面就是具体的访问服务端的方法了，这里只需要强调一点，那就是访问服务端之前
    //会调用本类的injectSecurityInfo方法，把本地token封装到请求中一起发送给服务端
    @Override
    public Result httpPost(String path, Object body) {
        isHealthStatus();
        path = injectSecurityInfoByPath(path);
        return HttpUtil.post(buildUrl(path), body, Result.class);
    }

    @Override
    public Result httpPostByDiscovery(String path, Object body) {
        isHealthStatus();
        path = injectSecurityInfoByPath(path);
        return HttpUtil.post(buildUrl(path), body, Result.class);
    }

    @Override
    public Result httpGetByConfig(String path, Map<String, String> headers, Map<String, String> paramValues, long readTimeoutMs) {
        isHealthStatus();
        injectSecurityInfo(paramValues);
        return HttpUtil.get(buildUrl(path), headers, paramValues, readTimeoutMs, Result.class);
    }

    @Override
    public Result httpPostByConfig(String path, Map<String, String> headers, Map<String, String> paramValues, long readTimeoutMs) {
        isHealthStatus();
        injectSecurityInfo(paramValues);
        return HttpUtil.post(buildUrl(path), headers, paramValues, readTimeoutMs, Result.class);
    }

    @Override
    public Result httpDeleteByConfig(String path, Map<String, String> headers, Map<String, String> paramValues, long readTimeoutMs) {
        return null;
    }

    private String buildUrl(String path) {
        return serverListManager.getCurrentServerAddr() + path;
    }

    private void isHealthStatus() {
       //该方法暂不实现
    }

    //把本地token封装到map中的方法
    private Map injectSecurityInfo(Map<String, String> params) {
        if (StringUtil.isNotBlank(securityProxy.getAccessToken())) {
            params.put(Constants.ACCESS_TOKEN, securityProxy.getAccessToken());
        }
        return params;
    }

    @Deprecated
    private String injectSecurityInfoByPath(String path) {
        String resultPath = HttpUtil.buildUrl(path, injectSecurityInfo(new HashMap<>()));
        return resultPath;
    }
}
```

接下来就要把 HttpAgent 对象定义在 DynamicThreadPoolAutoConfiguration 类中了。具体实现请看下面代码块。

```
package cn.hippo4j.springboot.start.config;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/26
 * @方法描述：这个就是spring-starter中的核心类，这个类使用了大量springboot的功能，这个类上的springboot注解应该都是常用注解了，所以这些功能我就不写注释了
 */
@Configuration
@AllArgsConstructor
@ConditionalOnBean(MarkerConfiguration.Marker.class)
//这个注解会使BootstrapProperties类上的ConfigurationProperties注解生效，BootstrapProperties对象就可以被SpringBoot容器管理了
@EnableConfigurationProperties(BootstrapProperties.class)
@ConditionalOnProperty(prefix = BootstrapProperties.PREFIX, value = "enable", matchIfMissing = true, havingValue = "true")
public class DynamicThreadPoolAutoConfiguration {

    //在这里把配置文件中的相关信息封封装到这个成员变量中了
    //properties对象会被自动注入
    private final BootstrapProperties properties;


    //远程通信组件，使用的是http通信方式
    @Bean
    public HttpAgent httpAgent(BootstrapProperties properties) {
        return new ServerHttpAgent(properties);
    }

}
```

好了，现在 httpAgent 通信组件也实现了。之后我就要简单重构一下 DynamicThreadPoolPostProcessor 类，重构的原因很简单，因为我们已经明确了注册客户端线程池到服务端的思路，**那就是在 DynamicThreadPoolPostProcessor 类的 postProcessAfterInitialization 后置处理方法中判断当前 bean 对象是否为 DynamicThreadPoolExecutor 类型的，并且该对象头上是否存在 @DynamicThreadPool 注解，如果存在则把动态线程池的信息注册到服务端**。既然是要注册线程池信息到服务端，肯定要和服务端通信，因此 DynamicThreadPoolPostProcessor 肯定也需要持有 HttpAgent 通信对象。不仅如此，**既然要在 DynamicThreadPoolPostProcessor 类中和服务端进行通信，将动态线程池的信息注册到服务端，肯定要知道动态线程池所属的命名空间和项目Id，再加上线程池自己的 Id，这样才能确定一个唯一的动态线程池实例。所以还要让 DynamicThreadPoolPostProcessor 持有 BootstrapProperties 对象**。这样分析下来，DynamicThreadPoolPostProcessor 也就重构完了，具体实现请看下面代码块。

```
package cn.hippo4j.springboot.start.support;


@Slf4j
@AllArgsConstructor
public final class DynamicThreadPoolPostProcessor implements BeanPostProcessor {


    //配置信息对象
    private final BootstrapProperties properties;

    //客户端代理对象
    private final HttpAgent httpAgent;


    //bean前置处理方法
    @Override
    public Object postProcessBeforeInitialization(Object bean, String beanName) {
        return bean;
    }



   //bean后置处理方法
    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
       //该方法暂时不做实现
    }

    
}
```

最后，还需要把 DynamicThreadPoolPostProcessor 对象交给 SpringBoot 容器来管理，否则 DynamicThreadPoolPostProcessor 的处理 bean 的后置方法根本不会被回调，因此还需要再重构一下 DynamicThreadPoolAutoConfiguration 类，请看下面代码块。

```
package cn.hippo4j.springboot.start.config;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/26
 * @方法描述：这个就是spring-starter中的核心类，这个类使用了大量springboot的功能，这个类上的springboot注解应该都是常用注解了，所以这些功能我就不写注释了
 */
@Configuration
@AllArgsConstructor
@ConditionalOnBean(MarkerConfiguration.Marker.class)
//这个注解会使BootstrapProperties类上的ConfigurationProperties注解生效，BootstrapProperties对象就可以被SpringBoot容器管理了
@EnableConfigurationProperties(BootstrapProperties.class)
@ConditionalOnProperty(prefix = BootstrapProperties.PREFIX, value = "enable", matchIfMissing = true, havingValue = "true")
public class DynamicThreadPoolAutoConfiguration {

    //在这里把配置文件中的相关信息封封装到这个成员变量中了
    //properties对象会被自动注入
    private final BootstrapProperties properties;


    //远程通信组件，使用的是http通信方式
    @Bean
    public HttpAgent httpAgent(BootstrapProperties properties) {
        return new ServerHttpAgent(properties);
    }


    //动态线程池处理器，这个处理器其实是就是spring中的一个bean处理器，在这个bean处理器中把动态线程池包装成了DynamicThreadPoolRegisterWrapper对象
    //然后开始向服务端注册该动态线程池的信息
    @Bean
    @SuppressWarnings("all")
    public DynamicThreadPoolPostProcessor threadPoolBeanPostProcessor(HttpAgent httpAgent,
                                                                      ApplicationContextHolder hippo4JApplicationContextHolder) {
        return new DynamicThreadPoolPostProcessor(properties, httpAgent);
    }

} 
```

好了，到此为止动态线程池框架客户端的前置准备工作终于做完了，接下来，终于可以开始实现 DynamicThreadPoolPostProcessor 类，实现客户端向服务端注册动态线程池信息的功能了。但是，没错，最后还有一个但是，我又想到了一个问题，**假如用户在使用我们开发的动态线程池框架时，创建了第三方的线程池，但是在该线程池对象上加上了 @DynamicThreadPool 注解，这又该怎么办呢**？也许大家不太理解我这句话的意思，但是这一章的内容已经够多了，我就不在这一章解释了，在下一章我会为大家解释这个问题是什么意思，并且为大家实现客户端向服务端注册动态线程池信息的功能。好了诸位，我们下一章见。