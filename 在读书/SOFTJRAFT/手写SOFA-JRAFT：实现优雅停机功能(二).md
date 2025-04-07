上一章我已经为大家分析完毕了 jraft 集群节点在优雅停机时应该执行哪些操作，也跟大家明确了实现优雅停机功能的思路。这个功能实现起来非常简单，只需要模仿线程池关闭时的操作即可：

**1 首先定义一个优雅停机的标志，当 jraft 集群节点执行停机操作时，就更新该标志，从此之后，当前节点就可以不必再接收新的数据了。**

**2 如果当前节点内部还有尚未处理的数据，那么当前节点就要把待处理的数据处理完毕。**

**3 把待处理的数据处理完毕之后，当前节点就可以执行终止内部所有线程工作的操作了**。

只要遵循上面三个步骤，我们就可以马上为 jraft 集群的节点实现优雅停机功能，并且，在以上三个要点的帮助下，我在上一章已经为大家把 NodeImpl 对象的优雅停机功能实现完毕了。但是，在 jraft 集群的节点中，还有很多组件和 NodeImpl 对象一样需要和数据打交道，这也就意味着 jraft 集群节点在停机的时候，内部的很多组件也需要像 NodeImpl 对象那样先把待处理的数据处理了，然后再执行终止内部线程工作的操作。所以现在问题就来了：**在 sofajraft 框架，有哪些组件是需要和数据打交道的呢**？

在上一章结尾我已经为大家分析出了三个组件，一个就是 LogManagerImpl 日志管理器，一个就是 FSMCallerImpl 状态机组件，还有一个就是 LocalRaftMetaStorage 节点元数据保存器。这三个组件都需要和数据打交道，所以在 jraft 集群节点优雅停机的时候，要等待这三个组件把内部尚未处理的数据处理完毕了，当前节点才能执行真正停机的操作。但在 jraft 集群节点内部，除了这三个组件，还有一些其他的组件也都需要和数据打交道，**比如说 SnapshotExecutorImpl 快照执行器组件，这个组件就是用来生成快照和安装快照文件的，稍微想想就能意识到，为了保证数据的完整性，如果当前节点正在安装某个快照文件，那么在安装快照文件的过程中，肯定不能立刻执行停机操作；如果说当前节点正在生成某个快照文件，那么在生成快照文件的过程中，肯定也不能立刻执行停机操作；当前节点能做到的是，在执行停机操作时，可以不再生成新的快照文件，或者安装新的快照文件，但是已经在执行过程中的操作，就必须要执行完毕**。这一点大家应该都可以理解吧？这不就是上一章优雅停机的核心内容吗？

当然，**除了 SnapshotExecutorImpl 快照执行器组件，还有 ReadOnlyServiceImpl 线性一致读组件**，这个 ReadOnlyServiceImpl 线性一致读组件内部也使用了 Disruptor 框架的批处理器来处理数据，那么当节点优雅停机时，我们就可以让 ReadOnlyServiceImpl 线性一致读组件不再接收新的请求，但是要把内部没有处理完的读请求处理完毕。这些也很好理解吧？

所以，这样分析下来，我们就可以总结出来，**当 jraft 集群的节点执行停机操作时，要先对 NodeImpl、LogManagerImpl、FSMCallerImpl、LocalRaftMetaStorage、SnapshotExecutorImpl、ReadOnlyServiceImpl 这几个组件执行相关操作，等这些组件把内部未处理完的数据都处理完了，然后再执行响应的关闭各个组件内部工作线程的操作**。

## 重构 NodeImpl 类的 shutdown() 方法

很好，现在我们已经把 jraft 集群优雅停机时要处理的各个组件明确了，接下来，我就可以先为大家重构一下 NodeImpl 类的 shutdown() 方法，让大家从代码层面上看一下当 jraft 集群节点停机时，需要对其内部的哪些组件执行 shutdown() 方法。除了重构 NodeImpl 类的 shutdown() 方法，我还想把 jraft 集群节点的状态也引入进来，就像线程池拥有几种状态，线程池根据内部状态的变更判断是否要执行终止工作的操作，jraft 集群的节点也有很多状态，并且这些状态我在之前已经为大家展示过了。现在我就再为大家展示一遍，而且我可以告诉大家，很快我们就在程序中要用到这几种状态了。

接下来我先为大家把 jraft 集群节点内部的几种状态展示给大家，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/20
 * @Description:节点状态枚举类
 */
public enum State {
    //当前节点为领导者
    STATE_LEADER,
    //当前状态表示正在移交领导权
    STATE_TRANSFERRING,
    //候选者
    STATE_CANDIDATE,
    //跟随者
    STATE_FOLLOWER,
    //当前状态表示节点出现了错误
    STATE_ERROR,
    //表示节点还未初始化
    STATE_UNINITIALIZED,
    //表示节点正在停止工作
    //当jraft集群节点执行优雅停机操作时，就会把节点当前的状态更新为STATE_SHUTTING
    //表示当前节点正在停止工作，从此之后，当前节点就可以不再接收任何外来的数据了
    STATE_SHUTTING,
    //已经停止工作
    STATE_SHUTDOWN,
    //代表状态转换已经结束了吧，这个枚举对象在源码中没有被用到
    STATE_END;

    //该方法判断当前节点是否处于活跃状态
    //这个方法很重要，很快就要用到了
    public boolean isActive() {
        //原理很简单，就是判断当前状态的枚举对象是否小于STATE_ERROR的值
        //ordinal方法用于返回一个int，排在前面的枚举对象的这个int值小于排在后面的
        //只要是小于STATE_ERROR，就意味着当前节点还在正常工作，大于等于STATE_ERROR，当前节点不是出错，就是要关闭了
        return this.ordinal() < STATE_ERROR.ordinal();
    }
}
```

接下来是 NodeImpl 类的 shutdown() 方法，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/27
     * @方法描述：优雅停机的入口方法，该方法被调用的时候，用户可以自己定义一个回调方法传递到该方法中
     * 用户自己定义的回调方法会在优雅停机操作执行完毕之后被调用，可以根据优雅停机操作的成功或失败执行不同的操作
     */
    @Override
    public void shutdown(Closure done) {

        //如果节点当前的状态小于State.STATE_SHUTTING，那么当前节点就可以执行关闭节点的操作了
        if (this.state.compareTo(State.STATE_SHUTTING) < 0) {
            //把当前节点从节点管理器中移除
            NodeManager.getInstance().remove(this);

            //因为要停机了，在这里更新节点状态为State.STATE_SHUTTING
            //表示正在停止工作
            this.state = State.STATE_SHUTTING;

            //停止所有定时器工作
            List<RepeatedTimer> timers = stopAllTimers();

            //以下就是执行各个组件shutdown()方法的操作，注意，这里只是执行了各个组件的
            //shutdown()方法，该方法一旦执行，并不意味着各个组件立刻被关闭了，实际上该方法的
            //执行，会让各个组件尽快处理内部尚未处理完毕的数据
            //关闭线性一致读读服务组件
            if (this.readOnlyService != null) {
                this.readOnlyService.shutdown();
            }//关闭日志管理器组件
            if (this.logManager != null) {
                this.logManager.shutdown();
            }//关闭元数据存储器组件
            if (this.metaStorage != null) {
                this.metaStorage.shutdown();
            }//关闭快照执行器组件
            if (this.snapshotExecutor != null) {
                this.snapshotExecutor.shutdown();
            }//关闭状态机组件
            if (this.fsmCaller != null) {
                this.fsmCaller.shutdown();
            }//关闭rpc客户端服务组件，关闭rpc服务端组件的操作先不给大家展示了
            //因为在源码中rpc服务端的关闭并不是在这里执行的，后面我会为大家展示出来
            if (this.rpcService != null) {
                this.rpcService.shutdown();
            }
            //下面是对NodeImpl对象的Disruptor组件执行优雅关闭的操作
            if (this.applyQueue != null) {
                //创建CountDownLatch对象
                final CountDownLatch latch = new CountDownLatch(1);
                //给shutdownLatch成员变量赋值
                this.shutdownLatch = latch;
                //向Disruptor的环形队列中发布一个LogEntryAndClosure对象
                //并且把刚刚创建的latch对象赋值给LogEntryAndClosure对象的shutdownLatch成员变量
                //这样一来，当Disruptor批处理器在消费环形队列中的LogEntryAndClosure对象时
                //就可以判断LogEntryAndClosure对象的shutdownLatch成员变量有没有被赋值，如果被赋值了
                //就意味着当前节点要关闭了，这个时候就可以不用等待攒够32个LogEntryAndClosure对象
                //就可以直接处理数据了
                this.applyQueue.publishEvent((event, sequence) -> event.shutdownLatch = latch);
            }

            //关闭全局定时任务调度器
            timerManager.shutdown();

             //销毁所有定时器
            destroyAllTimers(timers);
        
            //等待以上组件都处理完待处理的数据
            join();
            
        }
       
    }



    //等待所有组件真正停止工作的方法
    @Override
    public synchronized void join() throws InterruptedException {
        if (this.shutdownLatch != null) {

            //在LogEntryAndClosureHandler处理器的onEvent()方法中
            //执行了shutdownLatch.countDown()方法，当前join()方法就可以继续向下执行了
            //也就可以把disruptor框架的批处理线程终止了
            this.shutdownLatch.await();
            
            //在这里把disruptor的批处理器线程停止了
            this.applyDisruptor.shutdown();
            this.applyQueue = null;
            this.applyDisruptor = null;
            this.shutdownLatch = null;
        }

        //该方法的其他内容暂时不做实现
    }
 

    

    //省略该类其他内容
}
```

上面代码块展示的就是我重构完毕的 NodeImpl 类的 shutdown() 方法，在该方法中可以看到，当 jraft 集群节点停机时，需要对非常多的组件执行 shutdown() 操作。但是，这里我一定要给大家解释清楚，**就算在这里调用了各个组件的 shutdown() 方法，这也不意味着这些组件就要立刻终止工作了**。别忘了，我们最终要实现的是优雅停机功能。在停机之前，各个组件要对内部尚未处理的数据进行处理。**所以在这里调用了各个组件的 shutdown() 方法之后，各个组件就会开始尽快处理内部尚未处理的数据**。就比如说 LocalRaftMetaStorage 节点元数据存储器吧，**这个组件会把当前节点的任期信息和为集群哪个节点投票信息记录在本地文件中，当 jraft 集群优雅停机的时候，这个组件就会立刻执行它内部的 save() 方法**，好在停机之前把节点元信息保存在本地，避免出现任何可能因素造成的数据丢失问题。我把 LocalRaftMetaStorage 节点元数据存储器的相关代码展示在下面代码块了，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.impl;

/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/20
 * @Description:元数据存储器
 */
public class LocalRaftMetaStorage implements RaftMetaStorage {


    //元数据文件的名称
    private static final String RAFT_META = "raft_meta";
    //判断元数据是否已经初始化了没有，所谓初始化，就是当前节点在启动的时候
    //是否已经存本地元数据文件中把之前的元数据加载到内存中了
    private boolean isInited;
    //元数据文件的路径
    private final String path;
    //初始化好的任期，也就是当前节点之前的任期
    private long term;
    //最后一次给哪个节点投票了，PeerId封装的就是一个节点的信息，就可以代表raft集群中的一个节点
    //该成员变量初始化的时候为空，PeerId.emptyPeer()方法返回一个PeerId对象，但这个对象中的信息为空
    //所以我就直接称为空节点了
    private PeerId votedFor = PeerId.emptyPeer();
    //raft协议用到的配置参数对象
    private final RaftOptions raftOptions;


    //省略部分内容
    
    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/20
     * @Description:这个方法的作用是持久化元数据，也就是元数据落盘
     * 当程序在执行的时候，当前节点的任期可能会发生变化，也可能会给其他节点投票，只要出现了这些动作，那么这些数据就要持久化到硬盘中
     * 在NodeImpl这个类中，就会看到下面这些方法被调用了
     */
    private boolean save() {
        //记录持久化开始时间
        final long start = Utils.monotonicMs();
        //创建序列化数据，还是通过Protobuf协议进行落盘，所以要创建一个StablePBMeta对象
        final LocalStorageOutter.StablePBMeta meta = LocalStorageOutter.StablePBMeta.newBuilder()
                //在这里把当前节点任期穿进去
                .setTerm(this.term)
                //为哪个节点投票也传进去
                .setVotedfor(this.votedFor.toString())
                .build();
        //根据数据要存放的本地文件的路径和文件名获得ProtoBufFile对象
        final ProtoBufFile pbFile = newPbFile();
        try {//开始持久化操作
            if (!pbFile.save(meta, this.raftOptions.isSyncMeta())) {
                reportIOError();
                return false;
            }
            return true;
        } catch (final Exception e) {
            LOG.error("Fail to save raft meta", e);
            reportIOError();
            return false;
        } finally {//执行完毕后获得该操作耗费的时间
            final long cost = Utils.monotonicMs() - start;
            if (this.nodeMetrics != null) {
                this.nodeMetrics.recordLatency("save-raft-meta", cost);
            }//记录日志
            LOG.info("Save raft meta, path={}, term={}, votedFor={}, cost time={} ms", this.path, this.term,
                    this.votedFor, cost);
        }
    }


    //根据元数据文件的本地路径和元数据文件名称创建一个用来加载元数据文件的ProtoBufFile对象
    private ProtoBufFile newPbFile() {
        return new ProtoBufFile(this.path + File.separator + RAFT_META);
    }



    //关闭该组件的方法
    @Override
    public void shutdown() {
        if (!this.isInited) {
            return;
        }//关闭之前执行一次save()方法
        save();
        this.isInited = false;
    }


    //省略该类其他内容
}
```

以上就是 LocalRaftMetaStorage 组件优雅关闭功能的全部实现，可以看到，实现逻辑非常简单。其实 jraft 集群节点内部其他组件的优雅关闭功能实现起来都是这么简单，接下来，我就以 LogManagerImpl 日志管理器为例子，为大家实现 LogManagerImpl 日志管理器组件的优雅关闭功能。

## 实现 LogManagerImpl 日志管理器优雅关闭功能

所谓 LogManagerImpl 日志管理器优雅关闭功能，其实就是在 jraft 集群节点执行停机操作的时候，让 LogManagerImpl 日志管理器把尚未处理完的日志处理完毕，然后再关闭 LogManagerImpl 组件内部使用的 Disruptor 框架的批处理器线程即可。当**然，LogManagerImpl 日志管理器对剩余日志的处理其实就是把这些日志从缓冲区中全部刷新到硬盘中**。这一点理解了之后，那么 LogManagerImpl 日志管理器组件优雅关闭的功能就非常容易实现了。还是按照之前分析的三个要点：

**1 当 LogManagerImpl 日志管理器的 shutdown() 方法被调用之后，就把 LogManagerImpl 日志管理组件关闭的标志更新了，这样一来，LogManagerImpl 日志管理组件就不会再接收新的数据。**

**2 LogManagerImpl 日志管理组件把缓冲区中的日志全部刷新到硬盘。**

**3 LogManagerImpl 日志管理组件把缓冲区中的数据刷新到硬盘之后，执行关闭内部 Disruptor 框架批处理器现成的操作**。

以上三个操作大家都可以理解吧？如果大家都理解了，那么接下来我就为大家展示一下重构之后的 LogManagerImpl 日志管理器组件。重构的思路和编码方式和 NodeImpl 对象的优雅关闭没什么区别，也用到了 CountDownLatch 对象，大家类比着看即可。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/30
 * @Description:日志管理组件，这个类的对象是用来管理日志的，提供了根据索引得到日志，得到日志的任期
 * 把日志存放到数据库中等等方法，日志存储系统其实分三个层次，一个是日志快照，一个是内存，最后就是硬盘
 */
public class LogManagerImpl implements LogManager {

    //日志管理器是否停止工作的标志
    private volatile boolean stopped;

    //下面这个成员变量是用来在内存中缓存条目日志对象的，所有的日志条目对象都会先缓存到内存里
    //然后就直接开始向跟随者传输日志了，这个其实就是日志缓冲区
    private final SegmentList<LogEntry> logsInMemory = new SegmentList<>(true);
    
    //又一个Disruptor框架，从NodeImpl的executeApplyingTasks方法中传递过来的批量日志
    //最终会被这个Disruptor异步落盘，但是这个disruptor并不只是处理日志落盘事件，还处理其他的一些事件
    //这些事件后面会慢慢重构完整
    private Disruptor<StableClosureEvent> disruptor;
    //环形队列
    private RingBuffer<StableClosureEvent> diskQueue;
    
    private volatile CountDownLatch shutDownLatch;

    //其余成员变量省略

    private enum EventType {
        //其他事件，日志落盘对应的就是这个事件
        OTHER,
        RESET,
        TRUNCATE_PREFIX,
        TRUNCATE_SUFFIX,
        //关闭日志管理器事件
        SHUTDOWN,
        //得到最后一条日志ID事件
        LAST_LOG_ID
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/1
     * @Description:初始化方法，这个方法的逻辑比较简单，所以注释也添加的简单一些
     */
    @Override
    public boolean init(final LogManagerOptions opts) {

        //创建disruptor
        this.disruptor = DisruptorBuilder.<StableClosureEvent> newInstance()
                .setEventFactory(new StableClosureEventFactory())
                .setRingBufferSize(opts.getDisruptorBufferSize())
                .setThreadFactory(new NamedThreadFactory("JRaft-LogManager-Disruptor-", true))
                .setProducerType(ProducerType.MULTI)
                 //设置阻塞策略，这里使用的是超时阻塞，超过10秒之后，就会抛异常
                //这个异常就会被下面设置的异常处理器捕捉，然后调用异常处理器中的reportError方法报告异常
                .setWaitStrategy(new TimeoutBlockingWaitStrategy(
                        this.raftOptions.getDisruptorPublishEventWaitTimeoutSecs(), TimeUnit.SECONDS))
                .build();
        //设置消费者处理器
        this.disruptor.handleEventsWith(new StableClosureEventHandler());
        //设置异常处理器
        this.disruptor.setDefaultExceptionHandler(new LogExceptionHandler<Object>(this.getClass().getSimpleName(),
                (event, ex) -> reportError(-1, "LogManager handle event error")));
        this.diskQueue = this.disruptor.start();

        //该方法其他内容省略
    }

   
    //当调用了这个shutdown方法之后，日志管理器组件就会开始执行停止disruptor批处理器工作的操作
    //并且创建shutDownLatch对象，shutDownLatch对象的countDown方法会在StableClosureEventHandler中被调用
    //当shutDownLatch对象的countDown方法在批处理器中被调用时，就意味着批处理器已经把缓冲区中的所有日志刷新到硬盘了
    @Override
    public void shutdown() {
        boolean doUnlock = true;
        this.writeLock.lock();
        try {
            if (this.stopped) {
                return;
            }//设置停止工作标志
            this.stopped = true;
            doUnlock = false;
        } finally {
            if (doUnlock) {
                this.writeLock.unlock();
            }
        }
        //停止disruptor组件批处理器线程工作
        //其实就是向Disruptor的环形数组中发布了一个SHUTDOWN事件
        //当批处理器从环形数组取出并处理这个SHUTDOWN事件时，就会把缓冲区中的日志全部刷新到硬盘中
        stopDiskThread();
    }


    //向Disruptor的环形数组中发布SHUTDOWN事件的方法
    private void stopDiskThread() {
        //这个计数器在这里被创建了，在StableClosureEventHandler中会被用到
        this.shutDownLatch = new CountDownLatch(1);
        ThreadPoolsFactory.runInThread(this.groupId, () -> this.diskQueue.publishEvent((event, sequence) -> {
            event.reset();
            event.type = EventType.SHUTDOWN;
        }));
    }



    //该方法会在NodeImpl对象的join()方法中调用
    //也就是说，等到日志管理器把缓冲区中的日志都刷新到硬盘后
    //才会真正关闭Disruptor框架的批处理器线程
    //这个时候才意味着日志管理器组件优雅关闭的操作执行结束了，那么调用NodeImpl对象join()方法
    //的线程就可以继续执行下去了
    @Override
    public void join() throws InterruptedException {
        if (this.shutDownLatch == null) {
            return;
        }
        this.shutDownLatch.await();
        this.disruptor.shutdown();
    }



     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/3
     * @Description:disruptor框架中要使用的消费者处理器
     */
    private class StableClosureEventHandler implements EventHandler<StableClosureEvent> {

        //这里还有几个成员变量呢，一定要看一下，第三个成员变量的作用非常重要，相当于日志落盘前的缓冲区
        //这个是存储到硬盘上的最后一条日志的ID，日志落盘后会返回最后一条ID，会给这个成员变量重新赋值
        LogId lastId = LogManagerImpl.this.diskId;
        //专门存放回调方法的集合，集合中存放的每一个对象都封装了回调方法
        List<StableClosure> storage = new ArrayList<>(256);
        //这个就是日志缓冲区对象了
        AppendBatcher ab = new AppendBatcher(this.storage, 256, new ArrayList<>(), LogManagerImpl.this.diskId);
        //在这个方法中，执行了日志落盘的操作
        @Override
        public void onEvent(final StableClosureEvent event, final long sequence, final boolean endOfBatch) throws Exception {
            //如果当前得到的消费数据类型为停机
            if (event.type == EventType.SHUTDOWN) {
                //先把缓冲区中的数据刷新到硬盘
                this.lastId = this.ab.flush();
                //设置硬盘上最后一条日志的id
                setDiskId(this.lastId);
                //当这个计数器执行了countDown方法后，本类的join方法就可以继续执行了
                LogManagerImpl.this.shutDownLatch.countDown();
                event.reset();
                return;
            }


            //其他内容省略
        
        }


        //该方法其他内容省略
        
    }


    //省略该类其他内容
    
}
```

上面代码块中的注释非常详细，而且之前给大家分析得也比较透彻，并且已经有一个现成的 NodeImpl 对象的 Disruptor 框架批处理器关闭的例子，所以我就不再重复解释上面代码块中的逻辑了。上面代码块展示的逻辑和 NodeImpl 对象的 Disruptor 框架批处理器关闭的逻辑几乎一样，大家简单看看就行。当然，这个时候也许有朋友会问，不是说当组件一旦开始优雅关闭时，就不会再接收新的数据了吗？但是在我上面为大家展示的 LogManagerImpl 日志管理器中，好像并没有体现出这一点。现在就让我来解释一下，其实相关功能我都已经实现了，只不过需要展示的相关代码比较多，所以我就不在文章中给大家展示了，大家可以直接看我提供的第十二版本代码，直接去该版本代码中查看 LogManagerImpl 日志管理器这个组件，**只需要看看该组件中哪里判断了 stopped 这个成员变量的值即**可，因为只有当 LogManagerImpl 日志管理器关闭的时候，stopped 成员变量才会被赋值为 true。**而新的数据每一次被发布到 Disruptor 的环形队列中时，都会判断 stopped 成员变量是否为 true，如果为 true，就不会把数据发布到 Disruptor 的环形队列中，而是会直接退出发布数据的方法**。这个就是 LogManagerImpl 日志管理器关闭时拒绝处理新数据的核心逻辑，大家直接去我提供的源码中查看即可。

好了，到此为止，我就为大家把 jraft 集群节点的 LogManagerImpl 日志管理器组件优雅关闭的功能实现完毕了。至于 jraft 集群节点中的其他组件优雅关闭的功能，我就不在文章中为大家依次实现和展示了，**因为每个组件优雅关闭的核心操作都一样，就像 LogManagerImpl 日志管理器和 NodeImpl 优雅关闭的逻辑那样，都是用到了 CountDownLatch 对象，在组件处理完内部数据之后，调用 CountDownLatch 对象的 countDown() 方法，然后真正关闭组件**。大家把文章中展示内容全掌握了，那么在阅读第 12 版本代码的时候，就会非常顺利了。

也许大家以为本章内容进行到这里，应该就结束了，因为 LogManagerImpl 日志组件优雅关闭的功能也实现了，jraft 集群几点其他组件优雅关闭的核心逻辑我也和大家分析清楚了，接下来好像就没什么可讲的内容了。如果真的是这样就好了，我就轻松很多了。其实还有很多内容需要跟大家讲解清楚。就比如说，我们都已经知道了，假如 jraft 集群的领导者优雅停机的时候，在 NodeImpl 对象的 apply() 方法中会判断领导者是否正在停机，如果正在停机就不会再接收新的客户端指令。这一点大家肯定已经很清楚了。但是我刚才为大家实现的 LogManagerImpl 日志管理器组件优雅关闭的功能时，并没有为大家区分领导者、跟随着的情况。

请大家思考一下，假如要停机的节点是领导者，**那么在 NodeImpl 对象的 apply() 方法中就会停止接收新的客户端指令，既然不会再接收新的指令，NodeImpl 也就不会再把指令封装成日志提交给 LogManagerImpl 日志管理器处理**，所以从这一点上来说，领导者节点只要执行了优雅停机操作，它的 LogManagerImpl 日志管理器组件就不会再接收到新的日志数据。但假如当前要停机的是跟随者节点呢？大家可别忘了，跟随者节点要接收的日志是从领导者那里传输过来的，当跟随者节点停机的时候，领导者可能还正向跟随者节点传输日志呢，而节点优雅停机又要求停机的节点不能再接收新数据了。那么，跟随者节点优雅停机时，如何拒绝来自领导者节点的日志呢？这个功能我也已经为大家实现了，**因为跟随者节点是在 handleAppendEntriesRequest() 方法中接收领导者传输过来的日志，那么我们就可以直接在 handleAppendEntriesRequest() 方法中判断当前节点是否已经开始执行停机操作了，如果开始执行，就不接收领导者传输的日志即可**。我把相关功能的代码展示在下面了，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/22
     * @Description:当前节点接收到领导者发送过来的消息时，不管消息是日志复制还是心跳消息，还是探针消息，都会调用该方法进行处理
     * 换句话说，其实领导者要发送的日志复制消息，以及心跳消息探针消息，使用的都是同一个RPC请求，也就是该方法中的第一个参数对象
     */
    @Override
    public Message handleAppendEntriesRequest(final RpcRequests.AppendEntriesRequest request, final RpcRequestClosure done) {
        //这些重复逻辑就不添加注释了
        boolean doUnlock = true;
        this.writeLock.lock();
        //得到操作开始时间
        final long startMs = Utils.monotonicMs();
        //从请求中获得领导者传送过来的日志的条数
        final int entriesCount = request.getEntriesCount();
        //定义一个变量，标志日志处理是否成功
        boolean success = false;

        
        try {//判断当前节点是否活跃，大家可以回过头看看之前为大家展示的State枚举类的内容
            //如果这里返回false，就意味着当前节点不是运行出错了，就是要关闭了
            //那这个时候就可以不必再接收来自领导者的任何数据，直接回复响应即可
            if (!this.state.isActive()) {
                LOG.warn("Node {} is not in active state, currTerm={}.", getNodeId(), this.currTerm);
                return RpcFactoryHelper
                        .responseFactory()
                        .newResponse(RpcRequests.AppendEntriesResponse.getDefaultInstance(), RaftError.EINVAL,
                                "Node %s is not in active state, state %s.", getNodeId(), this.state.name());
            }
        
        }
    

        //省略该类其他内容
    
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/27
     * @方法描述：优雅停机的入口方法，该方法被调用的时候，用户可以自己定义一个回调方法传递到该方法中
     * 用户自己定义的回调方法会在优雅停机操作执行完毕之后被调用，可以根据优雅停机操作的成功或失败执行不同的操作
     */
    @Override
    public void shutdown(Closure done) {

        //如果节点当前的状态小于State.STATE_SHUTTING，那么当前节点就可以执行关闭节点的操作了
        if (this.state.compareTo(State.STATE_SHUTTING) < 0) {
            //把当前节点从节点管理器中移除
            NodeManager.getInstance().remove(this);

            //因为要停机了，在这里更新节点状态为State.STATE_SHUTTING
            //表示正在停止工作
            this.state = State.STATE_SHUTTING;

            //停止所有定时器工作
            List<RepeatedTimer> timers = stopAllTimers();

            //以下就是执行各个组件shutdown()方法的操作，注意，这里只是执行了各个组件的
            //shutdown()方法，该方法一旦执行，并不意味着各个组件立刻被关闭了，实际上该方法的
            //执行，会让各个组件尽快处理内部尚未处理完毕的数据
            //关闭线性一致读读服务组件
            if (this.readOnlyService != null) {
                this.readOnlyService.shutdown();
            }//关闭日志管理器组件
            if (this.logManager != null) {
                this.logManager.shutdown();
            }//关闭元数据存储器组件
            if (this.metaStorage != null) {
                this.metaStorage.shutdown();
            }//关闭快照执行器组件
            if (this.snapshotExecutor != null) {
                this.snapshotExecutor.shutdown();
            }//关闭状态机组件
            if (this.fsmCaller != null) {
                this.fsmCaller.shutdown();
            }//关闭rpc客户端服务组件，关闭rpc服务端组件的操作先不给大家展示了
            //因为在源码中rpc服务端的关闭并不是在这里执行的，后面我会为大家展示出来
            if (this.rpcService != null) {
                this.rpcService.shutdown();
            }
            //下面是对NodeImpl对象的Disruptor组件执行优雅关闭的操作
            if (this.applyQueue != null) {
                //创建CountDownLatch对象
                final CountDownLatch latch = new CountDownLatch(1);
                //给shutdownLatch成员变量赋值
                this.shutdownLatch = latch;
                //向Disruptor的环形队列中发布一个LogEntryAndClosure对象
                //并且把刚刚创建的latch对象赋值给LogEntryAndClosure对象的shutdownLatch成员变量
                //这样一来，当Disruptor批处理器在消费环形队列中的LogEntryAndClosure对象时
                //就可以判断LogEntryAndClosure对象的shutdownLatch成员变量有没有被赋值，如果被赋值了
                //就意味着当前节点要关闭了，这个时候就可以不用等待攒够32个LogEntryAndClosure对象
                //就可以直接处理数据了
                this.applyQueue.publishEvent((event, sequence) -> event.shutdownLatch = latch);
            }

            //关闭全局定时任务调度器
            timerManager.shutdown();

             //销毁所有定时器
            destroyAllTimers(timers);
        
            //等待以上组件都处理完待处理的数据
            join();
            
        }
       
    }



   //等待所有组件真正停止工作的方法，该方法到此就重构完毕了
    @Override
    public synchronized void join() throws InterruptedException {
        if (this.shutdownLatch != null) {
            if (this.readOnlyService != null) {
                this.readOnlyService.join();
            }
            if (this.logManager != null) {
                this.logManager.join();
            }
            if (this.snapshotExecutor != null) {
                this.snapshotExecutor.join();
            }
            this.shutdownLatch.await();
            //在这里把disruptor的批处理器线程停止了
            this.applyDisruptor.shutdown();
            this.applyQueue = null;
            this.applyDisruptor = null;
            this.shutdownLatch = null;
        }
        if (this.fsmCaller != null) {
            this.fsmCaller.join();
        }
    }
 

    

    //省略该类其他内容
}
```

在上面的代码块中，我就把跟随者节点优雅停机时，拒绝接收来自领导者日志的功能实现完毕了，并且我还把重构完毕的 join() 方法展示在上面代码块中了，大家可以简单看看。

好了，解决了跟随者节点在优雅停机不再接收新的日志的问题后，接下来我想再为大家补充上一章未解决的一个问题。在上一章我们实现 jraft 集群领导者节点优雅停机的时候，我跟大家说领导者节点一旦决定停机，就不会再接收来自客户端的新指令，并且在优雅停机的过程中，还会让 NodeImpl 对象内部使用的 Disruptor 框架的批处理器把环形队列中尚未处理完的数据处理完毕，这样一来，就可以避免客户端指令丢失情况的出现。这个逻辑是没有问题的，但是我记得我在上一章还未大家补充了，**那就是即便领导者在优雅停机的过程中处理了环形队列中尚未处理的客户端指令，这些客户端指令也不会被封装成日志传输给集群的其他跟随者节点；领导者在处理这些指令的时候，会直接恢复客户端指令执行失败的响应，这样一来，客户端就知道操作执行失败了。这么做虽然不会执行指令，但并没有造成指令丢失的情况，至少客户端知道自己发出的指令执行失败了**，这个逻辑大家可以理解吧？在上一章我只是把领导者在优雅停机的过程中处理执行的过程给大家简单叙述了一下，接下来我先不跟大家解释为什么领导者要这么做，而是先把对应功能的代码展示给大家，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容

    
    //大家别忘了，在NodeImpl对象中还使用了Disruptor框架，所以在节点停机的时候
    //也要把NodeImpl对象中的Disruptor框架停止工作了，这个操作我也定义在该类的shutdown()方法中了
    //这个Disruptor就是用来将日志落盘的入口
    private Disruptor<LogEntryAndClosure> applyDisruptor;
    
    //Disruptor框架要使用的任务队列，也就是环形数组
    private RingBuffer<LogEntryAndClosure> applyQueue;

    private volatile CountDownLatch shutdownLatch;

    //日志管理组件
    private LogManager logManager;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/3
     * @Description:Disruptor狂简要使用的事件处理器，这个事件处理器会被Disruptor框架的批处理器持有
     * 然后就开始批量处理任务了
     */
    private class LogEntryAndClosureHandler implements EventHandler<LogEntryAndClosure> {
        //创建一个用于批量处理日志的集合，这个集合的容量默认是32
        private final List<LogEntryAndClosure> tasks = new ArrayList<>(NodeImpl.this.raftOptions.getApplyBatch());

        //该方法其实会在disruptor的BatchEventProcessor任务批处理器中被循环调用
        //该方法就是Disruptor的批处理器要调用的真正消费数据的方法，这个方法就是将日志落盘的入口方法
        @Override
        public void onEvent(final LogEntryAndClosure event, final long sequence, final boolean endOfBatch)throws Exception {


            //这里是判断当前节点是否要下线了，如果要下线，并且批处理的环形队列中还有未处理的指令
            //就要尽快把这些指令都处理了
            if (event.shutdownLatch != null) {
                if (!this.tasks.isEmpty()) {
                    //在这个方法中开始处理剩余的客户端指令，把指令交给日志管理器处理
                    executeApplyingTasks(this.tasks);
                    reset();
                }
                final int num = GLOBAL_NUM_NODES.decrementAndGet();
                LOG.info("The number of active nodes decrement to {}.", num);
                //在这里执行countDown()操作，让join()方法中停机的操作可以继续执行下去
                event.shutdownLatch.countDown();
                return;
            }


            
            //一直向这个集合中添加事件，每一个事件中都包含一条要存储到本地的日志
            this.tasks.add(event);
            //直到满足32条日志后，就把32条日志成批处理，endOfBatch表示是否最后一个，当集合中的数据等于32条的时候
            //就要开始进行批量写入本地了
            if (this.tasks.size() >= NodeImpl.this.raftOptions.getApplyBatch() || endOfBatch) {
                //在这个方法中开始处理业务日志
                executeApplyingTasks(this.tasks);
                //清空tasks集合，并且重置每一个event对象
                reset();
            }
        }

        
        //重置可循环利用对象
        private void reset() {
            for (final LogEntryAndClosure task : this.tasks) {
                //这里重置event对象是为了循环利用这个对象
                task.reset();
            }//这里清空集合，是为了下一次处理32个日志做准备
            this.tasks.clear();
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/3
     * @Description:批处理日志的方法，其实在这个方法中也只不过是做了一点点校验，然后就设置了一下日志的任期和类型
     */
    private void executeApplyingTasks(final List<LogEntryAndClosure> tasks) {


        this.writeLock.lock();
        try {//这个tasks集合中存放着32个LogEntryAndClosure对象
            //每个LogEntryAndClosure对象中存放着一个logEntry对象
            final int size = tasks.size();
            
            
            //判断当前节点是不是领导者，如果不是领导者不能处理日志
            //如果当前节点是领导者，并且已经优雅停机了，那么节点状态在shutdown()方法中就会被
            //更新为State.STATE_SHUTTING，表示领导者正在停止工作，所以会直接进入下面的分支
            //执行回调方法，告诉客户端指令执行失败了
            if (this.state != State.STATE_LEADER) {
                final Status st = new Status();
                //判断领导者是不是正在转移领导者权力
                if (this.state != State.STATE_TRANSFERRING) {
                    st.setError(RaftError.EPERM, "Is not leader.");
                } else {
                    st.setError(RaftError.EBUSY, "Is transferring leadership.");
                }
                LOG.debug("Node {} can't apply, status={}.", getNodeId(), st);
                //这个就是获得每一个LogEntryAndClosure对象中包装每一个Task中的回调方法
                //这个回调方法是用来通知业务层日志处理是否成功的
                final List<Closure> dones = tasks.stream().map(ele -> ele.done)
                        .filter(Objects::nonNull).collect(Collectors.toList());
                //回调每一个方法，通知业务层日志处理失败，其实也就是指令操作失败，因为访问的节点已经不是领导者了
                ThreadPoolsFactory.runInThread(this.groupId, () -> {
                    for (final Closure done : dones) {
                        done.run(st);
                    }
                });
                return;
            }

            //该方法其他内容省略
        }
    }
    


    @Override
    public void shutdown(Closure done) {

        //如果节点当前的状态小于State.STATE_SHUTTING，那么当前节点就可以执行关闭节点的操作了
        if (this.state.compareTo(State.STATE_SHUTTING) < 0) {
            //把当前节点从节点管理器中移除
            NodeManager.getInstance().remove(this);

            //因为要停机了，在这里更新节点状态为State.STATE_SHUTTING
            //表示正在停止工作
            this.state = State.STATE_SHUTTING;
        

            //该方法其余内容省略
        
        }


}
```

在上面代码块的 executeApplyingTasks() 方法中，可以看到非常清楚的逻辑，那就是对当前节点的状态做了一下判断，判断当前节点是否仍然为领导者。如果仍然是领导者，那么当前节点就可以继续处理指令，如果当前节点不是领导者，那就可以直接通知业务层指令执行失败了。**而假如当前节点为领导者，并且已经执行了优雅停机的操作，就会在 shutdown() 方法中把自己的状态更新为 STATE_SHUTTING，表示正在停机。这样一来，就算领导者要处理剩余的客户端指令，也不会把指令封装成日志传输给跟随者节点了，而是直接通知业务层指令执行失败**。这个逻辑已经展示得非常清晰了，我相信每一位朋友都能理解。当然，我也能想到，可能有的朋友会很好奇为什么领导者在优雅停机的时候不把内部剩余指令封装成日志传输给领导者。其实这个问题很简单，**当一个 raft 集群领导者下线了，如果这个 raft 集群仍然想对外提供服务，那么这个集群势必就要选举出一个新的领导者。如果领导者在优雅停机的时候仍然把自己内部的日志传输给集群的跟随者节点，那势必会增加集群内部情况的复杂性。如果我们就以 jraft 集群节点为例子，那么我可以告诉大家，在 jraft 集群中，领导者下线的时候，为了保证集群能够继续堆外提供正常服务，领导者会在下线的过程中从集群其他节点中找到一个可以成为候选者的节点，然后通知该节点率先进入领导者选举阶段。这样一来，jraft 集群很快就会产生新的领导者，集群也就可以继续对外提供服务**。大家可以仔细品味品味这种情况。

好了，如果在候选者节点进入了领导者选举阶段，而之前旧的领导者还在把自己内部未处理完毕的指令封装成日志传输给集群的其他节点，那还怎么执行领导者选举工作啊，先不说 raft 集群根本就没有考虑到这种情况，就算考虑到这种情况了，领导者是要根据各个节点最新日志索引和任期来选举出来的，而各个节点仍然在接收旧的领导者的日志，那么日志索引就会处在一个动态变化的情况下，那还怎么选举新的领导者啊？显然，让即将下线的领导者继续把自己内部未处理的执行封装成日志传输给 jraft 集群其他节点的操作是荒谬的，所以，才有了以上代码 executeApplyingTasks() 方法中展示的直接回复业务层指令执行失败的操作。其实分析了这么一大串，现在大家应该也很清楚了，总而言之，**jraft 集群领导者优雅停机的时候，就不能再把自己内部的数据传输给集群其他节点了；而 jraft 集群非领导者节点优雅停机的时候，就不能再接收来自集群其他节点的一切信息了，比如不能接收新的日志，也不能再处理其他节点索要投票的请求，也不能再安装来自其他节点的快照文件等等**，这一点大家可以直接去我提供的第12 版本的代码中查看，在具体的方法中，都会对当前节点的状态进行判断，逻辑非常简单，大家一看就明白了。

如果大家把以上内容全都理解了，这个时候大家就会意识到，在 jraft 集群中，如果是非领导者节点执行优雅停机的操作，那么这个过程要执行的操作都非常简单，也很容易理解；但如果是领导者节点执行了优雅停机操作，那在这个过程中要执行的操作就比较麻烦一些了。刚才我们已经分析了，**领导者节点在优雅停机的时候需要从集群其他节点中找到一个可以成为候选者的节点，以便于集群尽快产生新的领导者**，那么这个功能该怎么实现呢？我们也都清楚，**领导者内部还管理者非常多的 Replicator 复制器对象，每一个 Replicator 复制器对象都对已经集群一个跟随者节点，领导者停机的时候，这些 Replicator 复制器对象要怎么办呢**？**Replicator 复制器对象内部的资源该如何清理和释放呢**？除了这两个问题，还有最重要的一个问题，别忘了，**jraft 集群节点把日志持久化到本地的时候，是要存储在 RocksDB 数据库中，领导者停机的时候，相关数据库的资源该如何释放呢**？看吧，随便分析了一下，又产生了这么多问题，但这些问题在这一章是解决不完了，就放到下一章为大家解决吧。朋友们，我们下一章见！

附：本章内容结束之后，我提供给大家的第 12 版本代码的大部分内容就对大家放开了，实际上大家只要有充足的耐心，结合代码中的注释，那么掌握第 12 版本代码的所有内容都不成问题。所以，现在大家尽可以去阅读一下第 12 版本代码了，当然，如果有的朋友们并不着急，也可以再等一等，下一篇文章就是 sofajraft 框架的最后一篇文章，下一篇文章更新完毕，也就意味着 sofajraft 优雅停机的内容全部更新完毕了，所以大家也可以等到下一篇文章更新完毕后再看代码。