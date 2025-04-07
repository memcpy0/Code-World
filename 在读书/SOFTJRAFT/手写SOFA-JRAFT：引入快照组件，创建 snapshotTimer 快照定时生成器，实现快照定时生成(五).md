上一章结束的时候，我为大家总结了 9 个问题，我把这 9 个问题给大家展示在下面了，接下来我们就来解决这些问题。

**1 同步或者异步生成快照的功能并没有在程序中体现出来，也就是说，并没有把同步或者异步生成快照的操作交给用户来选择。**

**2 快照定时器每一次要执行的定时任务，都必须真的执行吗？什么情况下可以不执行本次生成快照文件的定时任务呢？**

**3 生成了快照文件之后，那么与快照文件存储数据对应的日志条目应该怎么删除呢？**

**4 SaveSnapshotClosure 回调对象该怎么创建呢？**

**5 SaveSnapshotClosure 回调对象该怎么交给 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法使用？**

**6 LocalSnapshotStorage 快照存储器具体在哪里创建，初始化呢？**

**7 LocalSnapshotWriter 快照写入器要在什么时候被创建呢？**

**8 LocalSnapshotWriter 快照存储器的 close() 方法怎么在 SaveSnapshotClosure 对象的方法中被调用呢？**

**9 LocalSnapshotWriter 快照写入器怎么才能交给 SaveSnapshotClosure 回调对象使用呢？**

  

在开始解决这 9 个问题之前，我还要再为大家展示一下我们最开始实现的快照生成定时器的相关内容，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


public class NodeImpl implements Node, RaftServerService {

    //省略其他成员变量
    
    //快照生成定时器
    private RepeatedTimer snapshotTimer;

    //状态机组件
    private FSMCaller fsmCaller;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/21
     * @Description:本类最最核心的方法，初始化节点的方法，jraft框架中的所有组件都是在这个方法中初始化的，但是我们这是第一版本代码
     * 所以初始化的组件非常少，只有集群内部使用的客户端服务初始化了，还有元数据存储器初始化了
     */
    @Override
    public boolean init(final NodeOptions opts) {

        //省略该方法的其他内容

        //创建状态机组件
        this.fsmCaller = new FSMCallerImpl();

        //得到NodeId对象的toString方法结果
        final String suffix = getNodeId().toString();
        //得到快照生成定时器
        String name = "JRaft-SnapshotTimer-" + suffix;
        //创建快照生成定时器，这里定义了定时任务要执行的间隔时间
         this.snapshotTimer = new RepeatedTimer(name, this.options.getSnapshotIntervalSecs() * 1000,
                TIMER_FACTORY.getSnapshotTimer(this.options.isSharedSnapshotTimer(), name)) {

            private volatile boolean firstSchedule = true;

            @Override
            protected void onTrigger() {
                handleSnapshotTimeout();
            }

            @Override
            protected int adjustTimeout(final int timeoutMs) {
                if (!this.firstSchedule) {
                    return timeoutMs;
                }
                this.firstSchedule = false;
                if (timeoutMs > 0) {
                    int half = timeoutMs / 2;
                    return half + ThreadLocalRandom.current().nextInt(half);
                } else {
                    return timeoutMs;
                }
            }
        };
        //启动快照定时器
        this.snapshotTimer.start();

        //省略该方法的其他内容
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：快照生成定时器要执行的方法
     */
    private void handleSnapshotTimeout() {
        this.writeLock.lock();
        try {
            if (!this.state.isActive()) {
                return;
            }
        } finally {
            this.writeLock.unlock();
        }//在另一个线程中执行生成快照的操作，这么做是为了避免快照定时器的线程阻塞
        //快照定时器使用的可是时间轮线程，这个线程可不能阻塞，否则其他定时任务就不能及时执行了
        //这里的回调方法为null
        ThreadPoolsFactory.runInThread(this.groupId, () -> doSnapshot(null, false));
    }


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：执行快照生成的入口方法
     */
    private void doSnapshot(final Closure done, boolean sync) {

        fsmCaller.onSnapshotSave();
    }

    //省略该类的其他方法
}
```

从上面代码块中可以看到，我们直接在 NodeImpl 类的 doSnapshot() 方法中调用了 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法，这样一来快照定时器在每次执行定时任务的时候就会执行 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法，快照文件就会被定期生成了。这是我们目前实现的程序的执行流程。虽然在文章一开始我为大家展示了 9 个急需解决的问题，但现在我最想关注的是这 9 个问题中的第 2 个问题，也就是： **快照定时器每一次要执行的定时任务，都必须真的执行吗？什么情况下可以不执行本次生成快照文件的定时任务呢**？

而我之所以想先解决这个问题的原因也很简单，我不希望程序做无用功，因为在很多情况下，生成快照的定时任务完全可以不必执行。就比如说：raft 集群的节点在第一次生成快照文件的时候，状态机应用了 500 条日志，生成的快照文件会把这 500 条日志对应的数据保存在快照文件中。过了一段时间，raft 集群的节点要执行第二次生成快照文件的操作了，但是这个时候，状态机并没有应用新的日志，或者说状态机虽然应用了新的日志，但是只应用了 10 条日志，那在这种情况下，当前节点还有必要再生成新的快照文件吗？显然是没有必要的，因为状态机没有应用很多新日志，也就意味着状态机中的数据没怎么发生变更，生成的新的快照文件可能和旧快照文件保存的数据差别不大。所以，**当距离上一次生成快照之后，状态机并没有应用很多日志的情况下，就算快照生成定时器执行定时任务了，也可以直接退出定时任务，不必生成新的快照**。这一点我相信大家都是可以理解的。那么这个功能该怎么实现呢？目前有两个实现思路，一个就是直接在 NodeImpl 类的 doSnapshot() 方法中添加详细的条件判断，也就是对该方法做逻辑复杂的重构；另一个就是引入新的组件，把对是否执行定时任务的判断封装在新组件中，我们只需要在 NodeImpl 类的 doSnapshot() 调用新组件的方法即可。我个人更愿意是用第二种方式，所以接下来，就让我为我们的程序引入一个新的组件。

## 引入 SnapshotExecutorImpl 快照执行器

从当前小节的标题大家就能看出来，我要引入的新组件就是 SnapshotExecutorImpl 快照执行器，**从名字上就可以看出来，这个快照执行器主要是用来负责本次生成快照文件的操作是否要被执行**，**如果不必执行，那么直接退出当前方法即可，如果需要执行，这也很简单，直接调用 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法就行了；我可以给这个 SnapshotExecutorImpl 快照执行器定义一个 doSnapshot() 方法，把以上操作都定义在 doSnapshot() 方法中**。如果是这样的话，我们新定义的这个 SnapshotExecutorImpl 快照执行器肯定需要持有 FSMCallerImpl 状态机组件，可以直接在其内部的 doSnapshot() 方法中调用 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法执行生成快照文件的操作。当然，这样一来，我们就不必在最外层的 NodeImpl 类的 doSnapshot() 方法中调用 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法了，我们可以把 SnapshotExecutorImpl 快照执行器定义在 NodeImpl 类中，然后在 NodeImpl 类的 doSnapshot() 方法中调用 SnapshotExecutorImpl 快照执行器的 doSnapshot() 方法，我把相关代码都展示在下面了。

首先是新引入的 SnapshotExecutorImpl 快照执行器，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/19
 * @方法描述：日志快照组件，在第八版本代码中，我只引入了快照生成的有关代码，跟随者节点从领导者节点安装快照的操作并没有实现
 * 这个操作会在第九版本实现，所以我也没有把多余的代码删除，而是直接注释了，这样下个版本就可以直接使用了
 */
public class SnapshotExecutorImpl implements SnapshotExecutor {

    private static final Logger LOG = LoggerFactory.getLogger(SnapshotExecutorImpl.class);

    private final Lock lock = new ReentrantLock();

    //快照执行器是否已停止工作
    private volatile boolean stopped;

    //状态机组件，快照最终也是要作用到业务层的，比如跟随者节点从领导者安装了快照，肯定需要把快照的内容作用到状态机上
    private FSMCaller fsmCaller;

    //最新快照记录的日志索引，也就是在生成快照的时候，应用到那条日志了，这条日志是最新的
    //那么就会把这个日志索引记录在这里
    //这个成员变量如何被赋值，很快就会展示
    private long lastSnapshotIndex;

    //当前执行器是否正在保存快照
    private volatile boolean savingSnapshot;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：快照执行器初始化的方法
     */
    @Override
    public boolean init(final SnapshotExecutorOptions opts) {

        //对存储快照文件的uri判空
        if (StringUtils.isBlank(opts.getUri())) {
            LOG.error("Snapshot uri is empty.");
            return false;
        }

        this.fsmCaller = opts.getFsmCaller();
        return true;
    }

    

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：生成快照的方法
     */
    private void doSnapshot(final Closure done, boolean sync) {
        boolean doUnlock = true;
        //上锁
        this.lock.lock();

        try {
            //判断当前快照执行器是否已经停止工作
            if (this.stopped) {
                //如果已经停止工作，那就直接执行回调方法，如果是快照定时器定时生成快照，这里的这个done其实为null，具体逻辑可以从NodeImpl
                //的handleSnapshotTimeout方法查看
                ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), done, new Status(RaftError.EPERM,
                        "Is stopped."));
                return;
            }


            //如果执行器正在保存一个快照，也直接退出该方法
            if (this.savingSnapshot) {
                ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), done, new Status(RaftError.EBUSY,
                        "Is saving another snapshot."));
                return;
            }


            //这里判断一下，如果触发快照生成器定时任务时，发现状态机最后应用的日志索引和最后记录的快照的日志索引一样，这就说明
            //在这段时间内状态机没有什么变化，没有应用新的日志，所以也就没必要生成快照，直接退出即可
            if (this.fsmCaller.getLastAppliedIndex() == this.lastSnapshotIndex) {
                doUnlock = false;
                //释放锁
                this.lock.unlock();
                //执行回调方法
                ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), done);
                return;
            }



            //这里计算的是最后一次生成快照时记录的应用的日志索引距现在状态机应用的最新的日志的索引的差值
            final long distance = this.fsmCaller.getLastAppliedIndex() - this.lastSnapshotIndex;
            
            //如果差值小于用户配置的，就不必生成快照，比如距离上次生成快照之后，状态机仅仅应用了十几条日志，这就没必要生成快照了
            //这个差值是可以由用户配置的，默认是0
            if (distance < this.node.getOptions().getSnapshotLogIndexMargin()) {
                if (this.node != null) {
                    LOG.debug(
                            "Node {} snapshotLogIndexMargin={}, distance={}, so ignore this time of snapshot by snapshotLogIndexMargin setting.",
                            this.node.getNodeId(), distance, this.node.getOptions().getSnapshotLogIndexMargin());
                }
                doUnlock = false;
                //释放锁
                this.lock.unlock();
                //执行回调方法
                ThreadPoolsFactory.runClosureInThread(
                                getNode().getGroupId(),
                                done,
                                new Status(RaftError.ECANCELED,
                                        "The snapshot index distance since last snapshot is less than NodeOptions#snapshotLogIndexMargin, canceled this task."));
                return;
            }


            //接下来就是可以执行生成快照操作的内容

            //设置正在保存快照的标志
            this.savingSnapshot = true;

            //调用状态机组件的方法，执行生成快照文件的操作
            this.fsmCaller.onSnapshotSave();

            
        }  
            
        
        finally {
            if (doUnlock) {
                this.lock.unlock();
            }
        
        }
    
    }
}
```

下面是重构之后的 NodeImpl 类，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


public class NodeImpl implements Node, RaftServerService {

    //省略其他成员变量
    
    //快照生成定时器
    private RepeatedTimer snapshotTimer;

    //状态机组件
    private FSMCaller fsmCaller;

    //快照执行器
    private SnapshotExecutor snapshotExecutor;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/21
     * @Description:本类最最核心的方法，初始化节点的方法，jraft框架中的所有组件都是在这个方法中初始化的，但是我们这是第一版本代码
     * 所以初始化的组件非常少，只有集群内部使用的客户端服务初始化了，还有元数据存储器初始化了
     */
    @Override
    public boolean init(final NodeOptions opts) {

        //省略该方法的其他内容

        //创建状态机组件
        this.fsmCaller = new FSMCallerImpl();

        //得到NodeId对象的toString方法结果
        final String suffix = getNodeId().toString();
        //得到快照生成定时器
        String name = "JRaft-SnapshotTimer-" + suffix;
        //创建快照生成定时器，这里定义了定时任务要执行的间隔时间
         this.snapshotTimer = new RepeatedTimer(name, this.options.getSnapshotIntervalSecs() * 1000,
                TIMER_FACTORY.getSnapshotTimer(this.options.isSharedSnapshotTimer(), name)) {

            private volatile boolean firstSchedule = true;

            @Override
            protected void onTrigger() {
                handleSnapshotTimeout();
            }

            @Override
            protected int adjustTimeout(final int timeoutMs) {
                if (!this.firstSchedule) {
                    return timeoutMs;
                }
                this.firstSchedule = false;
                if (timeoutMs > 0) {
                    int half = timeoutMs / 2;
                    return half + ThreadLocalRandom.current().nextInt(half);
                } else {
                    return timeoutMs;
                }
            }
        };
        //启动快照定时器
        this.snapshotTimer.start();

        //省略该方法的其他内容
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：快照生成定时器要执行的方法
     */
    private void handleSnapshotTimeout() {
        this.writeLock.lock();
        try {
            if (!this.state.isActive()) {
                return;
            }
        } finally {
            this.writeLock.unlock();
        }//在另一个线程中执行生成快照的操作，这么做是为了避免快照定时器的线程阻塞
        //快照定时器使用的可是时间轮线程，这个线程可不能阻塞，否则其他定时任务就不能及时执行了
        //这里的回调方法为null
        ThreadPoolsFactory.runInThread(this.groupId, () -> doSnapshot(null, false));
    }


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：执行快照生成的入口方法
     */
    private void doSnapshot(final Closure done, boolean sync) {

        //这里直接调用快照执行器的doSnapshot()方法
        this.snapshotExecutor.doSnapshot(done);
    }

    //省略该类的其他方法
}
```

上面代码块展示完毕之后，现在大家肯定已经清楚了新引入的这个 SnapshotExecutorImpl 快照执行器的具体用法。当然，**现在我还没有为大家明确这个快照执行器的创建时机，以及初始化时机。这个还不着急，我们先来分析另外一个问题，然后再回过头看看 SnapshotExecutorImpl 快照执行器的创建时机**。 接下来请大家跟我思考另一个问题，那就是当快照执行器在它的 doSnapshot() 方法中调用了 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法后，就会执行生成快照的操作。但我们都知道，在 FSMCallerImpl 状态机组件执行了生成快照的操作之后，需要回调 SaveSnapshotClosure 对象中的方法，但现在我们迟迟没有定义这个 SaveSnapshotClosure 回调对象，也没有把这个 SaveSnapshotClosure 回调对象交给 FSMCallerImpl 状态机组件使用。如果我们可以定义出来这个 SaveSnapshotClosure 回调对象，就可以直接在 SnapshotExecutorImpl 快照执行器的 doSnapshot() 方法中把 SaveSnapshotClosure 回调对象交给 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法使用，就像下面展示的这样，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/19
 * @方法描述：日志快照组件，在第八版本代码中，我只引入了快照生成的有关代码，跟随者节点从领导者节点安装快照的操作并没有实现
 * 这个操作会在第九版本实现，所以我也没有把多余的代码删除，而是直接注释了，这样下个版本就可以直接使用了
 */
public class SnapshotExecutorImpl implements SnapshotExecutor {


    //省略部分内容

    

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：生成快照的方法
     */
    private void doSnapshot(final Closure done, boolean sync) {
        boolean doUnlock = true;
        //上锁
        this.lock.lock();

        try {

            //省略部分内容
            
            //接下来就是可以执行生成快照操作的内容

            //设置正在保存快照的标志
            this.savingSnapshot = true;

            //创建SaveSnapshotClosure回调对象
            SaveSnapshotClosure saveSnapshotDone = new SaveSnapshotClosure();
            
            //调用状态机组件的方法，执行生成快照文件的操作
            //当状态机组件生成完快照操作后，就可以直接调用saveSnapshotDone回调对象中的方法了
            this.fsmCaller.onSnapshotSave(saveSnapshotDone);

            
        }  
            
        
        finally {
            if (doUnlock) {
                this.lock.unlock();
            }
        
        }
    }
}
```

从上面代码块中可以看到，其实创建这个 SaveSnapshotClosure 回调对象也没什么困难，把它交给 FSMCallerImpl 状态机组件使用更是简单，我们现在要关注的是，**这个 SaveSnapshotClosure 回调对象应该怎么定义呢**？

## 重构 SnapshotExecutorImpl 快照执行器

  

既然现在我们要开始定义这个 SaveSnapshotClosure 回调对象了，那我们可以思考一下，应该怎么定义它。有一点很容易明确，那就是 SaveSnapshotClosure 应该定义在哪里。在之前的代码块中我已经给大家展示了，SaveSnapshotClosure 回调对象要在 SnapshotExecutorImpl 快照执行器的 doSnapshot() 方法中被创建，然后交给 FSMCallerImpl 状态机组件使用，既然是这样的话，**那我索性就把 SaveSnapshotClosure 定义为 SnapshotExecutorImpl 快照执行器的内部类吧**。我相信理解这一点对大家来说应该没什么难度，毕竟之前我们已经实现的很多组件中都把回调对象定义为内部类了。好了，现在我们已经确定了 SaveSnapshotClosure 类要定义的位置，接下来就应该继续思考，它应该怎么定义了。

其实也不用再做什么深层次的思考了，前几章我们已经把 SaveSnapshotClosure 回调对象中要定义的内容都分析完毕了。就比如说，当 FSMCallerImpl 状态机组件在生成了快照文件之后，要回调 SaveSnapshotClosure 对象中的方法，而在这个过程中，SaveSnapshotClosure 回调对象会在自己的方法中调用 LocalSnapshotWriter 快照写入器的 close() 方法，**这就意味着 SaveSnapshotClosure 对象肯定要持有 LocalSnapshotWriter 快照写入器。这很好办，我们直接把 LocalSnapshotWriter 快照写入器定义为 SaveSnapshotClosure 对象的成员变量不就行了**？**然后在 SaveSnapshotClosure 对象中定义一个回调方法，在该方法中调用快照写入器的 close() 方法**。这一点应该也能理解吧？

但如果我们真的这么做，那就会带来一个问题，在学习了上一章之后，我们已经知道了 LocalSnapshotWriter 快照写入器是被 LocalSnapshotStorage 快照存储器创建的。现在我们要在 SnapshotExecutorImpl 快照执行器的 doSnapshot() 方法中创建 SaveSnapshotClosure 回调对象，回调对象又要使用 LocalSnapshotWriter 快照写入器，而快照写入器又要被 LocalSnapshotStorage 快照存储器创建，这又该怎么操作呢？有一个最简单最直接的方式，**那就是把 LocalSnapshotStorage 快照存储器定义为 SnapshotExecutorImpl 快照执行器的成员变量，当快照执行器被创建的时候，恰好把 LocalSnapshotStorage 快照存储器也创建出来。这样一来，SnapshotExecutorImpl 快照执行器就可以直接在其内部的 doSnapshot() 方法中使用快照存储器创建快照写入器了，SaveSnapshotClosure 回调对象也就可以得到快照写入器了**。按照以上分析思路，接下来就让我来为大家把 SnapshotExecutorImpl 快照执行器再重新重构一下。

不过在重构之前，我还想到一个细节问题，那就是当 raft 节点成功生成快照文件之后，对应的日志条目应该怎么删除呢？到这个时候这个功能应该怎么实现已经非常明朗了。我们已经知道 SaveSnapshotClosure 回调对象应该怎么定义了，并且也知道一旦 SaveSnapshotClosure 回调对象中的方法被回调时，就意味着快照文件可能已经成功生成了，这个时候就可以把旧的快照文件删除，这一点我们已经实现了，同时我们也可以把对应的日志条目也都删除了。**所以，我们就可以把删除日志条目的操作也定义在 SaveSnapshotClosure 对象的回调方法中。当然，既然要和日志打交道，而日志是由 LogManagerImpl 日志管理器管理的，所以 LogManagerImpl 日志管理器也应该被 SnapshotExecutorImpl 快照执行器使用，索性也定义为快照执行器的成员变量吧**。好了，现在把日志删除的时机也分析完毕了，接下来就请大家看一看我重构完毕的 SnapshotExecutorImpl 快照执行器，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/19
 * @方法描述：日志快照组件，在第八版本代码中，我只引入了快照生成的有关代码，跟随者节点从领导者节点安装快照的操作并没有实现
 * 这个操作会在第九版本实现，所以我也没有把多余的代码删除，而是直接注释了，这样下个版本就可以直接使用了
 */
public class SnapshotExecutorImpl implements SnapshotExecutor {

    private static final Logger LOG = LoggerFactory.getLogger(SnapshotExecutorImpl.class);

    private final Lock lock = new ReentrantLock();

    //快照执行器是否已停止工作
    private volatile boolean stopped;

    //状态机组件，快照最终也是要作用到业务层的，比如跟随者节点从领导者安装了快照，肯定需要把快照的内容作用到状态机上
    private FSMCaller fsmCaller;

    //最新快照记录的日志索引，也就是在生成快照的时候，应用到那条日志了，这条日志是最新的
    //那么就会把这个日志索引记录在这里
    //这个成员变量如何被赋值，很快就会展示
    private long lastSnapshotIndex;

    //快照应用最后一条日志的任期
    private long lastSnapshotTerm;

    //当前执行器是否正在保存快照
    private volatile boolean savingSnapshot;

    //日志管理器
    private LogManager logManager;

    //快照存储器，这个存储器的作用就是生成快照以及加载快照
    private SnapshotStorage snapshotStorage;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：快照执行器初始化的方法
     */
    @Override
    public boolean init(final SnapshotExecutorOptions opts) {

        //对存储快照文件的uri判空
        if (StringUtils.isBlank(opts.getUri())) {
            LOG.error("Snapshot uri is empty.");
            return false;
        }
        //给成员变量赋值
        this.logManager = opts.getLogManager();
        this.fsmCaller = opts.getFsmCaller();
        //快照存储器在这里创建了，得到的是一个LocalSnapshotStorage对象
        //并且在这里也把快照存储路径传递给了快照存储器
        //这个时候我们就知道快照存储器的创建时机了
        this.snapshotStorage = this.node.getServiceFactory().createSnapshotStorage(opts.getUri(), this.node.getRaftOptions());
        //在这里初始化快照存储器
        if (!this.snapshotStorage.init(null)) {
            LOG.error("Fail to init snapshot storage.");
            return false;
        }
        
        return true;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：生成快照的方法
     */
    private void doSnapshot(final Closure done, boolean sync) {
        boolean doUnlock = true;
        //上锁
        this.lock.lock();

        try {

            //省略部分内容
            
            //接下来就是可以执行生成快照操作的内容

            //使用快照存储器创建一个快照写入器
            final SnapshotWriter writer = this.snapshotStorage.create();
            if (writer == null) {
                ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), done, new Status(RaftError.EIO,
                        "Fail to create writer."));
                reportError(RaftError.EIO.getNumber(), "Fail to create snapshot writer.");
                return;
            }

            //设置正在保存快照的标志
            this.savingSnapshot = true;

            //创建一个快照生成完成后的回调对象，快照生成完毕后要回调这个对象中的方法
            final SaveSnapshotDone saveSnapshotDone = new SaveSnapshotDone(writer, done, null);
            
            //调用状态机组件的方法，执行生成快照文件的操作
            //当状态机组件生成完快照操作后，就可以直接调用saveSnapshotDone回调对象中的方法了
            this.fsmCaller.onSnapshotSave(saveSnapshotDone);

            
        }  
            
        
        finally {
            if (doUnlock) {
                this.lock.unlock();
            }
        
        }
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：保存快照的操作完成后要回调的方法，就封装在这个内部类中
     * SaveSnapshotDone实现了SaveSnapshotClosure接口
     */
    private class SaveSnapshotDone implements SaveSnapshotClosure {
        //快照写入器
        SnapshotWriter writer;

        //这里的回调方法为null，只有在用户主动调用NodeImpl类的snapshot(Closure done)或者snapshotSync(Closure done)方法时，
        //用户可以自己定义这个回调对象，这个回调对象中的方法也会在快照生成之后在SaveSnapshotClosure回调对象的方法中被回调
        Closure done;

        //快照元数据信息
        RaftOutter.SnapshotMeta meta;

        public SaveSnapshotDone(final SnapshotWriter writer, final Closure done, final RaftOutter.SnapshotMeta meta) {
            super();
            this.writer = writer;
            this.done = done;
            this.meta = meta;
        }

        //快照生成完成后，回调这个方法
        //该方法的status参数代表快照生成的状态，也就是生成成功或者失败了
        @Override
        public void run(final Status status) {
            ThreadPoolsFactory.runInThread(getNode().getGroupId(), () -> continueRun(status));
        }
        
        //进一步回调到这个方法
        void continueRun(final Status st) {
            //这个是最重要的方法，在这个方法中，会对日志和生成的快照进行处理
            final int ret = onSnapshotSaveDone(st, this.meta, this.writer);
            if (ret != 0 && st.isOk()) {
                st.setError(ret, "node call onSnapshotSaveDone failed");
            }
            if (this.done != null) {
                ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), this.done, st);
            }
        }


        //这里的start方法就是用来给回调对象的快照元数据成员变量赋值的方法
        //该方法也会把快照元信息交给快照写入器使用
        //现在大家应该知道了，FSMCallerImpl状态机组件的doSnapshotSave()方法在得到了快照元信息之后
        //执行SnapshotWriter writer = done.start(metaBuilder.build());这行代码的意图了吧？
        @Override
        public SnapshotWriter start(final RaftOutter.SnapshotMeta meta) {
            this.meta = meta;
            this.writer.setCurrentMeta(meta);
            return this.writer;
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：当快照生成完成之后，要回调这个方法
     */
    int onSnapshotSaveDone(final Status st, final RaftOutter.SnapshotMeta meta, final SnapshotWriter writer) {

        int ret;
        this.lock.lock();
        
        //得到保存快照后的执行结果
        ret = st.getCode();

        //关闭快照写入器，这里需要注意一下，在关闭的过程中，还会执行本地快照存储器的关闭，在关闭的时候
        //会把快照元数据写入磁盘，把快照文件从临时文件移动到正式文件中
        //还会删除旧的快照文件
        writer.close();

        try {
            //这里判断快照是否保存成功，为0表示成功
            if (ret == 0) {
                //如果成功了，就更新快照应用的最后一条日志的索引和任期
                this.lastSnapshotIndex = meta.getLastIncludedIndex();
                this.lastSnapshotTerm = meta.getLastIncludedTerm();
                doUnlock = false;
                //这里释放锁的原因很简单，要处理日志了，为了减少快照执行器内部的并发情况
                //这里就把锁释放了，方便其他线程操作快照执行器内部的数据
                this.lock.unlock();
                //在这里调用日志管理器的方法，清除旧日志，设置lastSnapshotId的值等等
                //处理日志的方法就不在文章中展开了
                this.logManager.setSnapshot(meta);
                doUnlock = true;
                //操作又回到快照执行器中了，所以重新上锁
                this.lock.lock();
            }
            if (ret == RaftError.EIO.getNumber()) {
                reportError(RaftError.EIO.getNumber(), "Fail to save snapshot.");
            }
            //生成快照的操作已经完成了，把正在生成快照的标志设置为false
            this.savingSnapshot = false;
            return ret;
        } finally {
            if (doUnlock) {
                this.lock.unlock();
            }
        }
        
    }


    
}
```

上面代码块中的注释非常详细，而且之前我也为大家把实现思路分析得非常明确了，所以我就不再重复讲解上面代码块中的逻辑了，大家自己看看就行。如果非要对上面代码块的内容做一点补充，那就请大家把目光集中到上面代码块的第 212 行，也就是程序执行的 this.logManager.setSnapshot(meta) 这行代码。这行代码的作用非常清晰，就是根据快照元信息把对应的日志条目删除了，现在我想问大家的是：**当 raft 集群的节点生成了新的快照文件之后，真的要删除快照文件对应的日志条目信息吗**？举一个很直接的例子，比如说本次生成的快照文件对应的日志条目是 100—1000，难道快照文件生成之后，就要立刻把索引为 100—1000 的日志条目都删除吗？

假如说现在有这样一种情况：raft 集群中的某个节点处于故障恢复的情况，也就是刚才宕机了，现在已经重新启动了。启动之后这个节点要从集群领导者那里追赶数据进度，这个时候有两种方法，一种是直接远程安装领导者生成的快照文件，一种是复制领导者内部的日志条目。当前节点在追赶数据的时候发现自己落后领导者的数据并不多，也就 10 条日志的数据，于是当前节点就采用从领导者中复制日志的方式追赶进度。碰巧的是，领导者内部刚刚生成了最新的快照文件，快照文件生成之后直接把对应的日志条目全部删除了，跟随者节点到领导者这里复制日志的时候，发现根本没有日志可复制了，只能安装快照文件。这显然就非常麻烦了。明明领导者向跟随者传输 10 条日志就能同步进度了，现在却要把存储了状态机所有数据的快照文件安装给跟随者节点，这显然效率太低了。我相信分析到这里，大家应该能意识到 raft 集群的节点生成了快照文件之后，立刻把对应的日志条目删除这个操作的弊端了，显然我们不能这么做。那么我们应该怎么删除与生成的快照文件对应的日志呢？

我是这样解决这个问题的：**每一次生成新的快照文件之后，并不会把当前生成的快照文件对应的日志条目全部删除了，而是把上一次生成的快照文件对应的日志条目删除了。因为生成快照的定时任务的执行间隔是以小时来计算的，在两次生成快照操作之间，已经流出了足够的时间给 raft 集群的跟随者节点从领导者复制日志来同步进度**。也就是说，在上面代码块中 212 行执行的 this.logManager.setSnapshot(meta) 这行代码，删除的并不是当前生成的快照对应的日志条目，而是上一次生成的快照对应的日志条目。那这个功能是怎么实现的呢？这个也很简单，我在 LogManagerImpl 日志管理器中定义了一个 lastSnapshotId 成员变量，这个成员变量就代表上一次生成的快照应用的最后一条日志 ID，所以在每次生成快照文件之后，使用日志管理组件删除日志条目信息时，就根据 lastSnapshotId 成员变量删除即可。当然，每一次快照成功生成之后，我都会使用快照元信息更新日志管理器中的 lastSnapshotId 成员变量的值。具体实现请看下面代码块。

```
package com.alipay.sofa.jraft.storage.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/30
 * @Description:日志管理组件，这个类的对象是用来管理日志的，提供了根据索引得到日志，得到日志的任期
 * 把日志存放到数据库中等等方法，日志存储系统其实分三个层次，一个是日志快照，一个是内存，最后就是硬盘
 * 第三个版本会实现完整的日志落盘功能，第二版本只实现了日志组件初始化
 */
public class LogManagerImpl implements LogManager {

    //其余成员便零省略

    //上一次生成的快照应用的最后一条日志ID
    private volatile LogId lastSnapshotId = new LogId(0, 0);

    //读写锁
    private final ReadWriteLock lock = new ReentrantReadWriteLock();
    
    private final Lock writeLock = this.lock.writeLock();


    //省略部分方法


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/22
     * @方法描述：给日志管理器设置快照元数据信息，其实就是把本次生成快照中记录的最后应用的日志索引赋值，也就是给lastSnapshotId赋值
     * 然后把已经变成快照的日志从日志组件中删除，这里删除的是上一次生成的快照对应的日志，并不是本次生成的快照对应的
     * 每一次删除的都是上一次生成的快照对应的日志
     */
    @Override
    public void setSnapshot(final RaftOutter.SnapshotMeta meta) {
        LOG.debug("set snapshot: {}.", meta);
        boolean doUnlock = true;
        //上锁
        this.writeLock.lock();
        try {
            
            //meta.getLastIncludedIndex()得到的是本次生成的快照记录的最后应用日志的索引，这个值是要赋值给lastSnapshotId这个成员变量的
            //但是现在还没有赋值，lastSnapshotId是旧值，是上一次生成的快照记录的最后应用的日志的索引
            //如果这两个值相等，或者刚刚生成的快照的这个值比旧值还小，这就意味着快照生成重复或者延迟了，直接退出该方法即可
            if (meta.getLastIncludedIndex() <= this.lastSnapshotId.getIndex()) {
                return;
            }

            
            //根据本次生成快照的元数据获得快照应用的最后一条日志对应的任期
            final long term = unsafeGetTerm(meta.getLastIncludedIndex());
            //得到上一次快照应用的最后一条日志的索引
            final long savedLastSnapshotIndex = this.lastSnapshotId.getIndex();
            
            //在这里把本次生成快照的元数据更新给lastSnapshotId了，现在lastSnapshotId就是最新的了
            this.lastSnapshotId.setIndex(meta.getLastIncludedIndex());
            this.lastSnapshotId.setTerm(meta.getLastIncludedTerm());

            
            //走到这里term == meta.getLastIncludedTerm()，意味着是本地生成了快照
            //所以要把上一次生成快照对应的日志条目从本地删除
            if (term == meta.getLastIncludedTerm()) {
                if (savedLastSnapshotIndex > 0) {
                    doUnlock = false;
                    //清除上一次生成快照对应的日志条目
                    //这里就是把savedLastSnapshotIndex + 1之前的日志全部删除
                    //上一次生成的快照应用的最后一条日志是100，这里就是把101之前的所有日志都删除了，当然，不包括101
                    //我就不在文章中为大家展示truncatePrefix方法了，大家去我提供的第八版本代码中直接查看就行，注释非常详细
                    truncatePrefix(savedLastSnapshotIndex + 1, this.writeLock);
                }
            } 
            
        } finally {
            if (doUnlock) {
                this.writeLock.unlock();
            }
        }

    }


    //其余方法省略
}
```

上面代码块看完之后，现在我们已经清楚了，**那就是 raft 集群的节点在每次生成快照之后，会把上一次生成的快照文件对应的日志条目删除了**。如果大家理解了这一点，那我现在又有一个新的问题，**那就是假如 raft 集群的节点一直没有应用新的日志呢？这也就意味着这个节点不会执行生成快照的定时任务，不执行生成快照的定时任务，也就不会在 SaveSnapshotClosure 对象中回调删除上一次生成的快照文件对应日志条目的方法**。那这个问题该怎么解决呢？很简单，**就在每一次生成快照的定时任务执行的时候判断一下，如果本次定时任务不必执行，那就清理一下上一次生成的快照对应的日志条目**。我把相关代码也重构好了，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/19
 * @方法描述：日志快照组件，在第八版本代码中，我只引入了快照生成的有关代码，跟随者节点从领导者节点安装快照的操作并没有实现
 * 这个操作会在第九版本实现，所以我也没有把多余的代码删除，而是直接注释了，这样下个版本就可以直接使用了
 */
public class SnapshotExecutorImpl implements SnapshotExecutor {

    private static final Logger LOG = LoggerFactory.getLogger(SnapshotExecutorImpl.class);

    private final Lock lock = new ReentrantLock();

    //快照执行器是否已停止工作
    private volatile boolean stopped;

    //状态机组件，快照最终也是要作用到业务层的，比如跟随者节点从领导者安装了快照，肯定需要把快照的内容作用到状态机上
    private FSMCaller fsmCaller;

    //最新快照记录的日志索引，也就是在生成快照的时候，应用到那条日志了，这条日志是最新的
    //那么就会把这个日志索引记录在这里
    //这个成员变量如何被赋值，很快就会展示
    private long lastSnapshotIndex;

    //快照应用最后一条日志的任期
    private long lastSnapshotTerm;

    //当前执行器是否正在保存快照
    private volatile boolean savingSnapshot;

    //日志管理器
    private LogManager logManager;

    //快照存储器，这个存储器的作用就是生成快照以及加载快照
    private SnapshotStorage snapshotStorage;

    //省略部分内容


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：生成快照的方法
     */
    private void doSnapshot(final Closure done, boolean sync) {
        boolean doUnlock = true;
        //上锁
        this.lock.lock();

        try {

            //省略部分内容

            //这里判断一下，如果触发快照生成器定时任务时，发现状态机最后应用的日志索引和最后记录的快照的日志索引一样，这就说明
            //在这段时间内状态机没有什么变化，没有应用新的日志，所以也就没必要生成快照，直接退出即可
            if (this.fsmCaller.getLastAppliedIndex() == this.lastSnapshotIndex) {
                doUnlock = false;
                this.lock.unlock();
                //这里的这个方法是清除日志缓冲区中的日志，这里我也要解释一下，快照生成后清除日志的操作并不是真的在这里进行
                //我想解释的是快照生成后旧日志清楚的时机，这个逻辑其实是在LogManagerImpl的setSnapshot方法中
                //每次生成快照后，并不会立即就把对应的旧日志全部删除，而是删除前一次快照生成后对应的旧日志，换句话说，每一次生成快照文件，其对应的旧日志文件都会在下一次快照生成时被删除
                //为什么不在快照生成后立刻就把已经被记录到快照中的日志全清楚了呢？这里还考虑到可能有的节点是从故障中恢复过来的
                //也可能节点本身并不落后领导者多少，可能只落后几十条日志，但是领导者在生成快照后直接把旧的日志全清楚了
                //那么这个跟随者就不得不从领导者安装快照了，快照文件可能会很大，这样就难免有些麻烦了，所以在生成快照后并不会立刻把旧的日志清除
                //但是，回到我们上面所说的，如果在快照定时器还没触发的这段时间中一直没有日志被应用到状态机上，也就是说下一次不会进行快照生成的操作
                //那怎么删除上一次快照生成留下的旧的日志文件呢？现在大家应该清楚了，就是在这里删除的，这也就是下面这行代码的意义所在
                this.logManager.clearBufferedLogs();
                //执行回调方法
                ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), done);
                return;
            }
            
            //接下来就是可以执行生成快照操作的内容

            //省略部分内容
        }  
            
        
        finally {
            if (doUnlock) {
                this.lock.unlock();
            }
        
        }
    }


    //省略部分内容

}
```

到此为止，我就为大家把 raft 集群节点生成快照文件的核心功能都实现完毕了，当然，我还没有为大家展示 SnapshotExecutorImpl 快照执行器的创建时机，这个我其实都不想展示了，不用说，肯定是在 raft 节点启动的过程中创建的，也就是在 NodeImpl 类的 init() 方法中创建的。具体内容如下，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


public class NodeImpl implements Node, RaftServerService {

    //省略其他成员变量
    
    //快照生成定时器
    private RepeatedTimer snapshotTimer;

    //状态机组件
    private FSMCaller fsmCaller;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/21
     * @Description:本类最最核心的方法，初始化节点的方法，jraft框架中的所有组件都是在这个方法中初始化的，但是我们这是第一版本代码
     * 所以初始化的组件非常少，只有集群内部使用的客户端服务初始化了，还有元数据存储器初始化了
     */
    @Override
    public boolean init(final NodeOptions opts) {

        //省略该方法的其他内容

        //创建状态机组件
        this.fsmCaller = new FSMCallerImpl();

        //得到NodeId对象的toString方法结果
        final String suffix = getNodeId().toString();
        //得到快照生成定时器
        String name = "JRaft-SnapshotTimer-" + suffix;
        //创建快照生成定时器，这里定义了定时任务要执行的间隔时间
         this.snapshotTimer = new RepeatedTimer(name, this.options.getSnapshotIntervalSecs() * 1000,
                TIMER_FACTORY.getSnapshotTimer(this.options.isSharedSnapshotTimer(), name)) {

            private volatile boolean firstSchedule = true;

            @Override
            protected void onTrigger() {
                handleSnapshotTimeout();
            }

            @Override
            protected int adjustTimeout(final int timeoutMs) {
                if (!this.firstSchedule) {
                    return timeoutMs;
                }
                this.firstSchedule = false;
                if (timeoutMs > 0) {
                    int half = timeoutMs / 2;
                    return half + ThreadLocalRandom.current().nextInt(half);
                } else {
                    return timeoutMs;
                }
            }
        };
        //启动快照定时器
        this.snapshotTimer.start();



        //初始化快照组件
        if (!initSnapshotStorage()) {
            LOG.error("Node {} initSnapshotStorage failed.", getNodeId());
            return false;
        }

        //这里有一个操作很重要，就是检验日志索引一致性的操作
        //检查日志索引一致性，这里的检查索引一致性其实没那么复杂
        //当引入快照机制之后，每一次生成快照，都会把已生成快照的日志删除，所以
        //每次节点启动的时候，都会判断快照中的最后一条日志的索引和数据库中的第一条日志的索引是否
        //无缝衔接，如果中间有空隙，比如快照的最后一条索引为23，数据库的第一条索引为29
        //这显然就对不上了，数据肯定有丢失
        //该方法二具体内容我就不在文章中为大家展示了，大家可以直接去我提供的第八版本代码中查看
        //注释非常详细
        final Status st = this.logManager.checkConsistency();
        if (!st.isOk()) {
            LOG.error("Node {} is initialized with inconsistent log, status={}.", getNodeId(), st);
            return false;
        }

        //省略该方法的其他内容
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：初始化快照组建的方法
     */
    private boolean initSnapshotStorage() {
        if (StringUtils.isEmpty(this.options.getSnapshotUri())) {
            LOG.warn("Do not set snapshot uri, ignore initSnapshotStorage.");
            return true;
        }//创建快照执行器
        this.snapshotExecutor = new SnapshotExecutorImpl();
        final SnapshotExecutorOptions opts = new SnapshotExecutorOptions();
        opts.setUri(this.options.getSnapshotUri());
        opts.setFsmCaller(this.fsmCaller);
        opts.setNode(this);
        opts.setLogManager(this.logManager);
        opts.setAddr(this.serverId != null ? this.serverId.getEndpoint() : null);
        opts.setInitTerm(this.currTerm);
        opts.setFilterBeforeCopyRemote(this.options.isFilterBeforeCopyRemote());
        opts.setSnapshotThrottle(this.options.getSnapshotThrottle());
        return this.snapshotExecutor.init(opts);
    }


    //省略该类的其他方法
}
```

到此为止，本章的内容就全部结束了。在文章一开始我给大家展示的 9 个问题基本上都解决完毕了，就剩下一个问题没有解决，那就是：**同步或者异步生成快照的功能并没有在程序中体现出来，也就是说，并没有把同步或者异步生成快照的操作交给用户来选择**。这个问题我不准备在文章中讲解了，因为这个问题解决起来非常简单，并且我在提供的第八版本代码中添加了非常详细的注释，大家可以直接从 NodeImpl 类的 snapshot(final Closure done) 方法和 snapshotSync(Closure done) 方法看起，当然，要解决这个问题，NodeImpl 类的 doSnapshot() 方法也会再次被简单重构一下。我把相关的代码也给大家展示一下吧，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


public class NodeImpl implements Node, RaftServerService {

    //省略其他成员变量
    
    //快照生成定时器
    private RepeatedTimer snapshotTimer;

    //状态机组件
    private FSMCaller fsmCaller;

    //快照执行器
    private SnapshotExecutor snapshotExecutor;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：快照生成定时器要执行的方法
     */
    private void handleSnapshotTimeout() {
        this.writeLock.lock();
        try {
            if (!this.state.isActive()) {
                return;
            }
        } finally {
            this.writeLock.unlock();
        }//在另一个线程中执行生成快照的操作，这么做是为了避免快照定时器的线程阻塞
        //快照定时器使用的可是时间轮线程，这个线程可不能阻塞，否则其他定时任务就不能及时执行了
        //这里的回调方法为null，如果用户想自己定义回调方法，那可以直接调用NodeImpl类的snapshot()方法或者snapshotSync()方法
        //就是下面的两个方法
        ThreadPoolsFactory.runInThread(this.groupId, () -> doSnapshot(null, false));
    }



    //直接生成快照文件的方法，用户直接调用NodeImpl类的当前方法，就可以自己定义方法参数中的回调对象了
    @Override
    public void snapshot(final Closure done) {
        doSnapshot(done, false);
    }


    //直接同步生成快照文件的方法，如果是同步生成快照，这就意味着是使用FSMCallerImpl状态机组件直接生成快照
    //那么该方法就可以在状态机应用日志的操作中被回调，这样一来，肯定就是状态机的线程在执行同步生成快照的操作
    @Override
    public void snapshotSync(Closure done) {
        doSnapshot(done, true);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/19
     * @方法描述：执行快照生成的入口方法
     */
    private void doSnapshot(final Closure done, boolean sync) {
        if (this.snapshotExecutor != null) {
            //判断是同步生成还是异步生成
            if (sync) {//同步生成
                this.snapshotExecutor.doSnapshotSync(done);
            } else {//这里就是异步生成
                this.snapshotExecutor.doSnapshot(done);
            }
        } else {
            if (done != null) {
                final Status status = new Status(RaftError.EINVAL, "Snapshot is not supported");
                ThreadPoolsFactory.runClosureInThread(this.groupId, done, status);
            }
        }
    }
    
}
```

好了朋友们，本章内容就到此结束了，可以看到，sofajraft 框架生成快照文件的操作还是有点复杂的，组件多，逻辑绕，所以我会为大家提供该功能的程序执行流程图，我会抽时间完成的。下一章我将为大家实现 sofajraft 集群节点从领导者安装快照文件的功能，这个功能使用的组件也比较多，朋友们，我们下一章见！

附：**现在我提供的第八版本代码的内容就对大家全部开放了，大家可以直接去看了。在看之前，我建议大家一定要去网盘中下载一份最新的代码，因为我在写文章的过程中对第八版本代码的内容做了部分重构和修正，大家最好下载一下最新版本，否则文章中出现的部分内容并没有在旧版本代码中展示**。