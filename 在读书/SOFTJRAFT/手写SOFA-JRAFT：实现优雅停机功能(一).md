在阅读本篇文章的之前，大家一定要去语雀的网盘链接中下载我更新之后的最新的 sofajraft 框架的代码，我为 sofajraft 框架迭代了 13 个版本代码，其中 12、13 这两个版本经过大篇幅的重构，增加了许多新的内容，如果大家仍然使用旧版本的代码学习，那么在第 12 版本代码中，根本找不到本篇文章中实现的很多功能，所以大家一定要先把我提供的最新的 13 个版本代码下载了，然后再阅读本篇文章的内容。

  

## 简单分析 jraft 集群节点停机的过程

本章的核心内容就是要实现 sofajraft 框架优雅停机功能。我相信对大家来说，停机的功能对大家来说已经很熟悉了，优雅不优雅这个就另说了。因为停机的功能非常容易实现，假如一个框架运行的时候在内部启动了很多线程，那么停机的时候只要让这些线程都停止工作就行了，这就是停机的本质。我相信如果要让大家自己给 sofajraft 框架实现停机功能，那么每一位朋友都可以做到。稍微想一下就能发现，到目前为止，我们自己实现的 sofajraft 框架在 raft 集群节点上工作时已经启动了很多组件，比如说我们已经启动了 LogManagerImpl 日志管理器组件，FSMCallerImpl 状态机组件，ReadOnlyServiceImpl 线性一致读服务组件，RpcServer 节点服务端组件，那么当 raft 集群中的某个要停机的时候，就要对这几个组件做一些处理。原因很简单，**既然当前节点要停机了，那么作为网络通信的 RpcServer 服务端、RpcClient 客户端自然就可以关闭了，在关闭的过程中肯定会停止其内部线程的工作，而 LogManagerImpl 日志管理器和 FSMCallerImpl 状态机组件、ReadOnlyServiceImpl 线性一致读组件中都用到了 Disruptor 框架，而 Disruptor 框架之所以能够发挥作用，是因为其内部批处理的线程一直在努力工作，所以节点停机的时候也需要让 LogManagerImpl 日志管理器和 FSMCallerImpl 状态机组件、ReadOnlyServiceImpl 线性一致读组件使用的 Disruptor 框架批处理器的线程停止工作**。我相信刚才分析的这些操作流程大家应该都能理解吧，也就是说，如果只是实现节点停机功能，那我们现在就可以实现这个功能了。比如我们可以给 RpcServer、RpcClient、LogManagerImpl、FSMCallerImpl、ReadOnlyServiceImpl 都定义一个 shutDown() 方法，然后在该方法中停止其内部线程的工作。我把相关内容也都实现了，并且展示在下面的代码块中了。

首先是 RpcServer 类，请看下面代码块。

```
//RpcServer的实现类就是BoltRpcServer
package com.alipay.sofa.jraft.rpc.impl;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/23
 * @Description:Raft集群内部进行RPC通信时使用的服务端
 */
public class BoltRpcServer implements RpcServer {

    //省略该类其他内容
    
    private final com.alipay.remoting.rpc.RpcServer rpcServer;


    @Override
    public void shutdown() {
        this.rpcServer.shutdown();
    }

    //省略该类其他内容
}
```

然后是 RpcClient 类，请看下面代码块。

```
//RpcClient的实现类就是BoltRpcClient
package com.alipay.sofa.jraft.rpc.impl;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/23
 * @Description:Raft集群内部进行RPC通信时使用的客户端，这个是暴露给sofajraft框架使用的
 * 实际上这个类的rpcClient成员变量已经是bolt框架内部的组件了
 */
public class BoltRpcClient implements RpcClient {

    //省略该类其他内容

    private final com.alipay.remoting.rpc.RpcClient rpcClient;

    @Override
    public void shutdown() {
        this.rpcClient.shutdown();
    }

    //省略该类其他内容
}
```

然后是 LogManagerImpl 类，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/30
 * @Description:日志管理组件，这个类的对象是用来管理日志的，提供了根据索引得到日志，得到日志的任期
 * 把日志存放到数据库中等等方法，日志存储系统其实分三个层次，一个是日志快照，一个是内存，最后就是硬盘
 * 第三个版本会实现完整的日志落盘功能，第二版本只实现了日志组件初始化
 */
public class LogManagerImpl implements LogManager {

    //省略该类其他内容


    private Disruptor<StableClosureEvent> disruptor;

    //环形队列
    private RingBuffer<StableClosureEvent> diskQueue;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：停止组件工作的方法
     */
    @Override
    public synchronized void shutdown() {
        //停止disruptor框架工作
        //其内部批处理器线程就会停止了
        this.disruptor.shutdown();
    }
    
    //省略该类其他内容
    
}
```

然后是 FSMCallerImpl 类，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {


    //省略该类其他内容
    
    //disruptor启动器
    private Disruptor<ApplyTask> disruptor;
    //disruptor的环形数组
    private RingBuffer<ApplyTask> taskQueue;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：停止组件工作的方法
     */
    @Override
    public synchronized void shutdown() {
        //停止disruptor框架工作
        //其内部批处理器线程就会停止了
        this.disruptor.shutdown();
    }


    //省略该类其他内容
}
```

然后是 ReadOnlyServiceImpl 类，请看下面代码块。

```

//线性一致读服务组件
public class ReadOnlyServiceImpl implements ReadOnlyService, FSMCaller.LastAppliedLogIndexListener {


     //省略该类其他内容

    
    //又看见了disruptor框架，这说明只读请求肯定也是放在环形队列中，使用批处理器一次处理很多读请求
    private Disruptor<ReadIndexEvent> readIndexDisruptor;
    private RingBuffer<ReadIndexEvent> readIndexQueue;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：停止组件工作的方法
     */
    @Override
    public synchronized void shutdown() {
        //停止disruptor框架工作
        //其内部批处理器线程就会停止了
        this.readIndexDisruptor.shutdown();
    }


    
     //省略该类其他内容
    
}
```

当我为上面五个类都定义好 shutdown() 方法之后，只要 raft 集群中的节点依次调用这几个组件的 shutdown() 方法，那么节点内部运行的线程就会停止工作，这样一来，节点也就达到了停机的状态。而集群中的节点在代码层面会被抽象成一个 NodeImpl 对象，那我要想使当前节点停机，就可以在 NodeImpl 类中定义一个 shutdown() 方法，然后在 NodeImpl 对象的 shutdown() 方法中依次调用上面五个组件的 shutdown() 方法，就像下面展示的这样，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容

    
    //大家别忘了，在NodeImpl对象中还使用了Disruptor框架，所以在节点停机的时候
    //也要把NodeImpl对象中的Disruptor框架停止工作了，这个操作我也定义在该类的shutdown()方法中了
    //这个Disruptor就是用来将日志落盘的入口
    private Disruptor<LogEntryAndClosure> applyDisruptor;
    
    //Disruptor框架要使用的任务队列，也就是环形数组
    private RingBuffer<LogEntryAndClosure> applyQueue;




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/27
     * @方法描述：优雅停机的入口方法，该方法被调用的时候，用户可以自己定义一个回调方法传递到该方法中
     * 用户自己定义的回调方法会在优雅停机操作执行完毕之后被调用，可以根据优雅停机操作的成功或失败执行不同的操作
     */
    @Override
    public void shutdown(Closure done) {

        //这里先写成伪代码

        //先停止NodeImpl对象内部的Disruptor框架的批处理器线程
        applyDisruptor.shutdown();
        //后面依次调用五个组件的shutdown()方法即可
        boltRpcClient.shutdown();

        boltRpcClient.shutdown();

        logManagerImpl.shutdown();

        fSMCallerImpl.shutdown();

        readOnlyService.shutdown();
    }

    //省略该类其他内容
}
```

上面代码块展示完毕之后，我就把 jraft 集群节点停机功能实现完毕了。当然，实现到这里，我忽然意识到还少点什么，有一部分内容被我漏掉了。请大家想一想，当 **jraft 集群节点启动的时候，这个节点会创建并启动很多定时器，比如有进行投票选举的定时器，有处理投票超时的定时器，还有检测领导者是否需要下台的定时器，以及生成快照的定时器**，这些定时器内部也都是线程在执行任务，**所以当节点要停机的时候，这些定时器也必须都停止工作**，所以我们还要在 NodeImpl 类的 shutdown() 方法中，让这些定时器都停止工作。具体的代码我也实现完毕了，接下来就请大家看一下重构完毕的 NodeImpl 类的 shutdown() 方法，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容

    
    //大家别忘了，在NodeImpl对象中还使用了Disruptor框架，所以在节点停机的时候
    //也要把NodeImpl对象中的Disruptor框架停止工作了，这个操作我也定义在该类的shutdown()方法中了
    //这个Disruptor就是用来将日志落盘的入口
    private Disruptor<LogEntryAndClosure> applyDisruptor;
    
    //Disruptor框架要使用的任务队列，也就是环形数组
    private RingBuffer<LogEntryAndClosure> applyQueue;

    //全局定时任务调度器
    private Scheduler timerManager;
    
    //选举超时定时器
    private RepeatedTimer electionTimer;

    //快照生成定时器
    private RepeatedTimer snapshotTimer;


    //如果当前是候选者节点，那么在候选者节点中有一个voteTimer定时器，这个定时器会定期执行定时任务，判断
    //当前节点是不是候选者，如果是候选者，那么接着就会判断当前候选者的获得其他节点投票的操作是否超时了
    //如果超时了就对当前候选者执行降级处理。所谓降级处理就会调用NodeImpl对象的stepDown()方法
    //但是我为了简化逻辑，在为大家迭代的代码中，并没有引入这个定时器，大家知道该定时器的设计原理即可
    //感兴趣的话可以直接去源码中看一看
    private RepeatedTimer voteTimer;


    //如果当前节点是领导者，那么当前节点中会有一个stepDownTimer定时器，这个定时器会定期判断
    //领导者是否要下台了，判断的原理也很简单，就是判断领导者内部保存的Replicator复制器对象是否都还存活
    //如果有Replicator复制器对象死亡了，也就是在限制时间之内没有顺利进行通信，那就可以认为与Replicator复制器对应的节点失联了
    //如果存活的Replicator复制器对象小于集群节点总数的一半，就可以认为当前领导者应该下台了，然后执行相应的操作即可
    //但是我在为大家迭代的代码中，也没有引入这个定时器，因为要引入的相关方法有点多，所以我就省略了，大家理解该定时器的设计原理即可
    private RepeatedTimer stepDownTimer;




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/27
     * @方法描述：优雅停机的入口方法，该方法被调用的时候，用户可以自己定义一个回调方法传递到该方法中
     * 用户自己定义的回调方法会在优雅停机操作执行完毕之后被调用，可以根据优雅停机操作的成功或失败执行不同的操作
     */
    @Override
    public void shutdown(Closure done) {

        //这里先写成伪代码

        //先停止NodeImpl对象内部的Disruptor框架的批处理器线程
        applyDisruptor.shutdown();
        //后面依次调用五个组件的shutdown()方法即可
        boltRpcClient.shutdown();

        boltRpcClient.shutdown();

        logManagerImpl.shutdown();

        fSMCallerImpl.shutdown();

        //关闭全局定时任务调度器
        timerManager.shutdown();

        //停止所有定时器工作
        List<RepeatedTimer> timers = stopAllTimers();

        //销毁所有定时器
        destroyAllTimers(timers);
    }

    //停止节点内部所有定时器工作的方法
    private List<RepeatedTimer> stopAllTimers() {
        final List<RepeatedTimer> timers = new ArrayList<>();
        if (this.electionTimer != null) {
            this.electionTimer.stop();
            timers.add(this.electionTimer);
        }
        if (this.voteTimer != null) {
            this.voteTimer.stop();
            timers.add(this.voteTimer);
        }
        if (this.stepDownTimer != null) {
            this.stepDownTimer.stop();
            timers.add(this.stepDownTimer);
        }
        if (this.snapshotTimer != null) {
            this.snapshotTimer.stop();
            timers.add(this.snapshotTimer);
        }
        return timers;
    }

    //销毁所有定时器的方法
    private void destroyAllTimers(final List<RepeatedTimer> timers) {
        for (final RepeatedTimer timer : timers) {
            timer.destroy();
        }
    }

    

    //省略该类其他内容
}
```

到此为止，我就把 sofajraft 框架停机的功能实现完毕了。可以看到，实现的逻辑非常简单，无非就是停止程序内部的所有线程，停机功能就完成了。我相信每一位朋友都可以理解其中的逻辑。很好，停机功能实现完毕之后，请大家跟我思考一个问题，那就是我们目前实现的 raft 集群节点停机功能有没有什么弊端呢？我相信很多朋友应该都意识到了，我目前实现的 jraft 集群节点停机功能非常粗暴，可以说是管杀不管埋，留下了一大堆隐患。如果批评得严厉一点，从我为 jraft 集群节点实现的停机功能就可以看出来，在工作中我是一个极度不负责任的程序员，在生活中，我也是一个极度没有责任心的人，就像小仙女一样。当然，也有可能我真的是一个菜狗，根本考虑不到那么多问题。如果可以称赞失败品的话，那么我实现的停机功能就是一个完美的失败品，接下来，就让我来为大家仔细分析分析其中存在的各种问题。

## 分析 jraft 集群节点优雅停机的细节

在具体分析我上一小节实现的停机功能的缺陷之前，我想先跟大家明确一点，很多时候我们声称某个服务器要停机了，或者下线了，并不是意味着这个服务器不会再被使用了，也不是意味着服务器上部署的程序不会再启动了。通常情况下，停机下线只是暂时的，就像故障恢复一样。jraft 集群中的某个节点可能因为某些原因需要短暂停机一会，然后它就会重新启动，然后重新加入集群继续工作了。**也就是说，我们不要把节点下线或者停机看成永久的，下线停机的操作也许只是暂时的，很快节点就会重新启动，节点上部署的程序也会重新运行**。如果是这种情况的话，那么我刚才实现的 jraft 集群节点停机的功能问题就大了。

举一个最简单的例子，假如当前节点是领导者，领导者肯定要把接收到的客户端指令封装成日志，然后把日志传输给集群中的每一个节点。也许大家已经忘记了领导者把接收到的指令封装成日志，然后处理日志的操作流程，所以我把相关的代码展示在下面代码块中了，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容

    
    //大家别忘了，在NodeImpl对象中还使用了Disruptor框架，所以在节点停机的时候
    //也要把NodeImpl对象中的Disruptor框架停止工作了，这个操作我也定义在该类的shutdown()方法中了
    //这个Disruptor就是用来将日志落盘的入口
    private Disruptor<LogEntryAndClosure> applyDisruptor;
    
    //Disruptor框架要使用的任务队列，也就是环形数组
    private RingBuffer<LogEntryAndClosure> applyQueue;

     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/21
     * @Description:本类最最核心的方法，初始化节点的方法，jraft框架中的所有组件都是在这个方法中初始化的，但是我们这是第一版本代码
     * 所以初始化的组件非常少，只有集群内部使用的客户端服务初始化了，还有元数据存储器初始化了
     */
    @Override
    public boolean init(final NodeOptions opts) {

        //省略该方法部分内容

        //初始化一个disruptor队列，这里采用的是多生产者模式
        this.applyDisruptor = DisruptorBuilder.<LogEntryAndClosure> newInstance()
                //设置disruptor大小，默认16384
                .setRingBufferSize(this.raftOptions.getDisruptorBufferSize())
                //设置事件工厂
                .setEventFactory(new LogEntryAndClosureFactory())
                //设置线程工厂
                .setThreadFactory(new NamedThreadFactory("JRaft-NodeImpl-Disruptor-", true))
                //采用多生产者模式，也就是并发向环形队列中填充数据
                .setProducerType(ProducerType.MULTI)
                //阻塞策略
                .setWaitStrategy(new BlockingWaitStrategy())
                .build();
        //设置消费者处理器，使用的还是批处理器，因为这里只创建了一个LogEntryAndClosureHandler消费处理器
        //这个消费者处理器会被disruptor框架内不得BatchEventProcessor批处理器持有，然后开始处理任务
        //如果还有朋友不熟悉Disruptor框架，我强烈建议先去把Disruptor框架学习一下，否则你不明白日志处理的回调逻辑
        this.applyDisruptor.handleEventsWith(new LogEntryAndClosureHandler());
        //设置异常处理器
        this.applyDisruptor.setDefaultExceptionHandler(new LogExceptionHandler<Object>(getClass().getSimpleName()));
        //启动disruptor框架的消费者，启动之后消费者会阻塞，因为现在生产者还没有向环形数组中生产数据呢
        this.applyQueue = this.applyDisruptor.start();

        //省略该方法部分内容
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/3
     * @Description:Disruptor框架中的生产者生产的数据，在Disruptor中，会创建一个事件工厂，然后专门用来生产生产者生产的数据
     * 这个LogEntryAndClosure对象会被放到环形数组中，当作一个个要被消费者消费的数据，当然，真正要消费的数据还是这个类的对象中
     * 持有的LogEntry对象，那怎么把真正的要消费的数据放到这个类的对象中呢？这就要创建一个数据传输器对象了，很快我们就会见到了
     */
    private static class LogEntryAndClosure {
        LogEntry entry;
        Closure done;
        long expectedTerm;
        CountDownLatch shutdownLatch;

        public void reset() {
            this.entry = null;
            this.done = null;
            this.expectedTerm = 0;
            this.shutdownLatch = null;
        }
    }
    

    //事件工厂，需要在Disruptor框架中使用
    private static class LogEntryAndClosureFactory implements EventFactory<LogEntryAndClosure> {

        @Override
        public LogEntryAndClosure newInstance() {
            return new LogEntryAndClosure();
        }
    }

    


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
     * @Description:向jraft框架中提交日志的方法，这些日志就是从业务从传递到核心算法层的，日志以Task的形式在
     * jraft内部传输，每一个Tast对象中都包装了一条日志，当然，这个日志已经被包装成ByteBuffer了
     */
    @Override
    public void apply(final Task task) {
        Requires.requireNonNull(task, "Null task");
        //创建日志对象
        final LogEntry entry = new LogEntry();
        //把业务层传来的指令包装成一个日志条目，到这里一个业务指令就变成一条日志了
        //只不过现在的日志还没有索引，任期等等
        entry.setData(task.getData());
        //创建一个Disruptor框架的生产者需要使用的数据传输器，这个数据传输器的作用就是把日志条目对象
        //存放到LogEntryAndClosure对象中，而LogEntryAndClosure对象就会放到环形数组中
        final EventTranslator<LogEntryAndClosure> translator = (event, sequence) -> {
            event.reset();
            event.done = task.getDone();
            event.entry = entry;
            event.expectedTerm = task.getExpectedTerm();
        };
        //默认提交日志到Disruptor框架中是采用非阻塞模式
        switch(this.options.getApplyTaskMode()) {
            case Blocking:
                //这里阻塞的意思是，消费速度跟不上生产者速度时，当前发布生产者数据的线程就直接阻塞了
                //知道发不成功才结束阻塞
                this.applyQueue.publishEvent(translator);
                break;
            case NonBlocking:
            default:
                //这里非阻塞的意思是，当消费速度跟不上生产速度时，尝试向队列中发布生产者数据
                //如果发布不成功，就直接回复给客户端当前节点非常忙碌，并没有让线程阻塞
                if (!this.applyQueue.tryPublishEvent(translator)) {
                    String errorMsg = "Node is busy, has too many tasks, queue is full and bufferSize="+ this.applyQueue.getBufferSize();
                    ThreadPoolsFactory.runClosureInThread(this.groupId, task.getDone(),
                            new Status(RaftError.EBUSY, errorMsg));
                    LOG.warn("Node {} applyQueue is overload.", getNodeId());
                    this.metrics.recordTimes("apply-task-overload-times", 1);
                    if(task.getDone() == null) {
                        throw new OverloadException(errorMsg);
                    }
                }
                break;
        }
    }
 

    

    //省略该类其他内容
}
```

从上面代码块中可以看到，**apply() 方法就是领导者接收客户端指令的方法，而在该方法中，领导者会把接收到的客户端指令封装在 LogEntryAndClosure 对象中，然后把对象发布到 Disruptor 框架的环形数组中，这些数据就会被 LogEntryAndClosureHandler 处理器处理。而 LogEntryAndClosureHandler 处理器在消费这些数据的时候，会从环形队列中获得每一个 LogEntryAndClosure 对象，然后存储到一个集合中，只有当集合中存储的数据达到 32 个后，才会进一步处理这些 LogEntryAndClosure 对象**，后面就是把 LogEntryAndClosure 对象中的指令包装成日志，然后交给日志管理器处理的操作了，我就不再展开了，大家可以从我提供的代码中回顾回顾。通过上面的分析大家可以知道，**假如 Disruptor 框架的批处理器在使用 LogEntryAndClosureHandler 处理器处理环形队列中的数据时，如果数据量不满足 32，或者要处理的数据并不是环形数组中的最后一个，这个时候 Disruptor 框架的批处理器并不会立刻处理这些数据，而是要等待数据满足 32 或者发现环形数组中没有剩余数据了，才会进一步处理这些 LogEntryAndClosure 对象**。这个流程我相信大家也都可以理解吧？

很好，如果是这样的话，那么在 NodeImpl 对象的 Disruptor 框架的批处理器在等待 LogEntryAndClosure 对象满足 32 个的过程中，我对 jraft 集群的当前节点执行了停机操作，当操作执行的那一刻， NodeImpl 对象的 Disruptor 框架的批处理器线程就会停止工作，这也就意味着这些指令还没有被处理，当前节点就下线了。也就是说，**客户端明明向领导者发送了指令，但是因为领导者停机了，这些指令并没有被及时处理，所以这些指令就丢失了(这里我要多解释一句，就算领导者节点在停机的时候把环形数组中尚未被处理的指令处理了，这些客户端指令也不会被当成日志赋值给集群中的跟随者节点，而是会直接回复给客户端失败响应，告诉客户端指令执行失败了。大家还是理解清除，告诉客户端指令失败可不是指令丢失，如果什么都不回复才是指令丢失，大家要理解这一点。在文章后面的部分，我会为大家详细解释这一点。)**。这就是粗暴停机带来的问题。

如果上面的例子大家可以理解了，那接下来我再给大家举一个例子，我们都知道在 jraft 集群中，每一个节点在启动的时候都会创建并启动一个 LocalRaftMetaStorage 节点元数据存储器，这个存储器的作用非常简单，那就是把当前节点的任期以及给哪个节点投过票持久化到本地的元数据文件中。这样一来，当 jraft 集群中的某个接单宕机重启时，就可以加载本地的元数据文件，就可以知道当前节点在哪个任期给哪个节点投过票。这样一来也就不会发生重复投票的情况了。但假如当前节点刚给某个候选者节点投票了，还没有把元信息持久化到本地，我就对当前节点执行了停机操作，这样一来执行持久化元数据信息到本地的线程就会立刻停止工作，元数据还没有保存就停机了，那这部分显然就丢失了。当前节点重启之后，也不知道自己刚才给哪个候选者投过票。这个问题大家也可以理解吧？

可以看到，我刚才为大家举的例子都和数据丢失有关，这其实就在告诉我们，假如简单粗暴地直接执行停机操作，很容易就出现数据丢失的问题，也就是说破坏了数据的完整性。**所谓数据完整性，我们可以这样理解，假如一个服务器正在执行停机操作时，如果发现自己内部某些数据正在被写入到本地文件或者存储到数据库中，在这个时候，当前服务器就不能立即执行停机操作，等到这些数据全被保存成功之后，再执行停机操作，也就是停止内部线程工作操作，这样一来，就保证了数据的完整性**。节点重启之后，也不会产生丢失数据的问题。**如果我们把这个功能重构到我之前实现的停机功能中，那我就可以说我实现了 jraft 集群优雅停机功能**。

现在大家应该清楚了，对于 sofajraft 框架来说，优雅停机究竟是什么意思了吧？如果只实现停机功能，那每一位朋友都可以实现，所谓停机不就是释放资源吗？释放资源的操作谁不会呢？但在释放资源之前，应该如何正确处理数据，这就需要仔细想想了。朋友们，我们并不是不负责任的小仙女，我们应该对工作负责，应该对自己的职业有最基本的素养。就我自己来说，我非常并且十分抗拒一切形式的加班，但如果我接手的项目真的是在我手中出错了，那么我加班加点也要把问题解决了。好了，闲话少说，接下来就让我们一起思考一下，怎么为 jraft 集群的节点真正实现优雅停机的操作。

如果大家没有头绪，那我来给大家提醒一下，其实这个功能一点也不难，我们只需要模仿线程池终止工作的模式即可。请大家想一想，线程池在停止工作的时候，是不是可以先把自己的状态更新为正在终止工作的状态，一旦设置了这个状态之后，线程池就可以不必接收新的任务了，只需要专注把任务队列中的任务处理完毕了，然后就可以在真正停止工作了。这个流程大家应该都熟悉吧？好了，现在回到 jraft 集群的节点也是一样，我们希望 jraft 集群节点优雅停机能够到达一种效果：**那就是只要 jraft 集群中的节点开始执行优雅停机的操作了，当前节点就可以不用再接收新的数据，比如说如果当前节点是领导者，那么当前节点就可以不必再接收来自客户端的指令；如果当前节点是跟随者节点，那么当前节点就可以不必再接收来自领导者的日志；应该停机的节点只需要把自己内部尚未处理完毕的数据处理完，然后再终止内部各个组件的线程工作即可**。只要能做到这些，那么 jraft 集群的节点就算是实现了优雅停机的功能了。那这个功能具体该怎么实现呢？

这就很简单了，就比如说刚才我给大家举的领导者节点使用 Disruptor 框架的批处理器处理接收到的客户端指令吧，要想在优雅停机时让领导者不再接收来自客户端的指令，同时还能把待处理的指令交给日志管理器处理，那我们就可以在 NodeImpl 类中定义一个 bool 成员变量，如果节点执行了优雅停机操作，那么这个成员变量就可以被赋值为 true，这个时候领导者就可以不必再接收客户端发送过来的新指令了。等到内部待处理的指令都处理完毕了，这个时候就可以执行让 NodeImpl 对象内部使用的 Disruptor 框架的批处理器线程停止工作的操作。这些流程大家都可以理解吧？看起来好像一点也不难，很容易就能实现。

没错，实现起来确实非常简单，但是目前有一个问题困扰着我们，**那就是 NodeImpl 对象怎么就知道内部使用的 Disruptor 框架的批处理器线程处理完待处理的数据了呢？只有知道了这个，NodeImpl 才能决定终止 Disruptor 框架批处理线程工作**。这个时候，我们其实就可以使用 CountDownLatch 对象，**在 NodeImpl 对象执行 shutdown() 优雅停机操作时，可以创建一个 CountDownLatch(1) 对象，然后再执行 CountDownLatch 对象的 await() 方法，等到 Disruptor 框架的批处理器线程处理完数据了，再执行 CountDownLatch 对象的 countDown() 方法，然后再终止 Disruptor 框架的批处理器线程工作即可**。我已经按照这个思路把 NodeImpl 类重构完毕了，请看下面代码块。

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



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/3
     * @Description:Disruptor框架中的生产者生产的数据，在Disruptor中，会创建一个事件工厂，然后专门用来生产生产者生产的数据
     * 这个LogEntryAndClosure对象会被放到环形数组中，当作一个个要被消费者消费的数据，当然，真正要消费的数据还是这个类的对象中
     * 持有的LogEntry对象，那怎么把真正的要消费的数据放到这个类的对象中呢？这就要创建一个数据传输器对象了，很快我们就会见到了
     */
    private static class LogEntryAndClosure {
        LogEntry entry;
        Closure done;
        long expectedTerm;
        CountDownLatch shutdownLatch;

        public void reset() {
            this.entry = null;
            this.done = null;
            this.expectedTerm = 0;
            this.shutdownLatch = null;
        }
    }

    //事件工厂，需要在Disruptor框架中使用
    private static class LogEntryAndClosureFactory implements EventFactory<LogEntryAndClosure> {

        @Override
        public LogEntryAndClosure newInstance() {
            return new LogEntryAndClosure();
        }
    }
    


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
            //就要尽快把这些指令都处理了，这里大家要理清楚一点，如果当前从环形数组中取出的LogEntryAndClosure
            //对象的shutdownLatch被赋值了，就意味着当前数据是环形数组中的最后一条了，因为
            //一旦shutdownLatch被赋值了，就意味着领导者节点要停机了，也就不会再接收新的客户端指令
            //所以接下来直接处理剩余指令即可
            if (event.shutdownLatch != null) {
                if (!this.tasks.isEmpty()) {
                    //在这个方法中开始处理剩余的客户端指令，把指令交给日志管理器处理
                    executeApplyingTasks(this.tasks);
                    reset();
                }
                //下面这行代码大家暂时忽略掉
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
     * @Description:向jraft框架中提交日志的方法，这些日志就是从业务从传递到核心算法层的，日志以Task的形式在
     * jraft内部传输，每一个Tast对象中都包装了一条日志，当然，这个日志已经被包装成ByteBuffer了
     */
    @Override
    public void apply(final Task task) {


        //判断节点是不是要下线了，如果要下线就不接收新的指令了
        if (this.shutdownLatch != null) {
            ThreadPoolsFactory.runClosureInThread(this.groupId, task.getDone(), new Status(RaftError.ENODESHUTDOWN, "Node is shutting down."));
            throw new IllegalStateException("Node is shutting down");
        }

        
        Requires.requireNonNull(task, "Null task");
        //创建日志对象
        final LogEntry entry = new LogEntry();
        //把业务层传来的指令包装成一个日志条目，到这里一个业务指令就变成一条日志了
        //只不过现在的日志还没有索引，任期等等
        entry.setData(task.getData());
        //创建一个Disruptor框架的生产者需要使用的数据传输器，这个数据传输器的作用就是把日志条目对象
        //存放到LogEntryAndClosure对象中，而LogEntryAndClosure对象就会放到环形数组中
        final EventTranslator<LogEntryAndClosure> translator = (event, sequence) -> {
            event.reset();
            event.done = task.getDone();
            event.entry = entry;
            event.expectedTerm = task.getExpectedTerm();
        };
        //默认提交日志到Disruptor框架中是采用非阻塞模式
        switch(this.options.getApplyTaskMode()) {
            case Blocking:
                //这里阻塞的意思是，消费速度跟不上生产者速度时，当前发布生产者数据的线程就直接阻塞了
                //知道发不成功才结束阻塞
                this.applyQueue.publishEvent(translator);
                break;
            case NonBlocking:
            default:
                //这里非阻塞的意思是，当消费速度跟不上生产速度时，尝试向队列中发布生产者数据
                //如果发布不成功，就直接回复给客户端当前节点非常忙碌，并没有让线程阻塞
                if (!this.applyQueue.tryPublishEvent(translator)) {
                    String errorMsg = "Node is busy, has too many tasks, queue is full and bufferSize="+ this.applyQueue.getBufferSize();
                    ThreadPoolsFactory.runClosureInThread(this.groupId, task.getDone(),
                            new Status(RaftError.EBUSY, errorMsg));
                    LOG.warn("Node {} applyQueue is overload.", getNodeId());
                    this.metrics.recordTimes("apply-task-overload-times", 1);
                    if(task.getDone() == null) {
                        throw new OverloadException(errorMsg);
                    }
                }
                break;
        }
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

        //这里先写成伪代码

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
        
        //后面依次调用五个组件的shutdown()方法即可
        boltRpcClient.shutdown();

        boltRpcClient.shutdown();

        logManagerImpl.shutdown();

        fSMCallerImpl.shutdown();

        //关闭全局定时任务调度器
        timerManager.shutdown();

        //停止所有定时器工作
        List<RepeatedTimer> timers = stopAllTimers();

        //销毁所有定时器
        destroyAllTimers(timers);
        
        //等待以上组件都处理完待处理的数据
        join();
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

以上就是我重构完毕的 NodeImpl 类，从上面代码块中可以看到，只要 jraft 集群的领导者调用了 NodeImpl 对象的 shutdown() 方法，就会创建一个 CountDownLatch 对象，然后把这个 CountDownLatch 对象赋值给 NodeImpl 的成员变量 shutdownLatch。只要这个 shutdownLatch 成员变量被赋值了，当前要停机的领导者就不会在 apply() 方法中再接收新的客户端指令，而在 NodeImpl 对象内部使用的 Disruptor 框架批处理器处理环形队列中的数据时，也不会再等待数据攒够 32 个再处理。因为领导者马上就要停机了，批处理器会把环形队列中的数据赶紧处理了。 等这些操作都执行完了，批处理器就会执行 shutdownLatch.countDown() 操作，这样一来，NodeImpl 对象的 join() 方法就可以继续执行下去了，也就意味着终于可以执行关闭 Disruptor 框架批处理线程的操作了。大家可以仔细品味品味上面代码块的逻辑，如果以上代码快的逻辑全都掌握了，那么大家就可以思考另一个问题了。

那就是当 jraft 集群节点执行优雅停机操作时，在真正停机之前，哪些数据可以被视为待处理的数据呢？换句话说，**jraft 集群节点中有哪些组件需要在真正停机之前处理完内部待处理的数据呢**？

我之所以在这里提出这个问题，是因为我自己很清楚我在上面代码块中为大家重构的 NodeImpl 类并不是最终版本，也就是说刚才实现的仅仅针对 jraft 集群领导者处理客户端指令优雅停机的功能仍然不完善，大家肯定都很清楚，当 jraft 集群领导者调用了 NodeImpl 对象的 shutdown() 方法执行优雅停机操作后，就不会再接收来自客户端的新指令了，然后 NodeImpl 对象使用的 Disruptor 框架批处理器会把待处理的指令封装成日志提交给日志管理器处理，我把相关代码展示在下面代码块中了，请看下面代码块。

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
            //就要尽快把这些指令都处理了，这里大家要理清楚一点，如果当前从环形数组中取出的LogEntryAndClosure
            //对象的shutdownLatch被赋值了，就意味着当前数据是环形数组中的最后一条了，因为
            //一旦shutdownLatch被赋值了，就意味着领导者节点要停机了，也就不会再接收新的客户端指令
            //所以接下来直接处理剩余指令即可
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

        //省略该方法的部分内容

        //将设置好的日志交给日志管理器处理，在日志管理器中，这些日志将会被设置任期，然后进行落盘的操作
        //这个方法中的具体操作就是把日志提交给日志管理器内部使用的Disruptor框架的环形数组了
        this.logManager.appendEntries(entries, new LeaderStableClosure(entries));

        //省略该方法的部分内容
    }


    //省略该类其他内容
}
```

从以上代码块中可以看到，NodeImpl 对象使用的 Disruptor 框架批处理器把数据提交给日志处理器之后，就直接执行了 event.shutdownLatch.countDown() 这行代码，这样一来，NodeImpl 对象的 join() 方法就可以继续执行，然后 NodeImpl 对象使用的 Disruptor 框架批处理器就可以被真正关闭了，这个流程大家肯定都很熟悉。但是，请大家注意，这个时候待处理的数据只是被提交给了日志管理器的 Disruptor 框架的环形队列，并没有真正被落盘呢，**如果我们在 NodeImpl 对象的 shutdown() 方法中继续关闭日志管理器，也就是执行 logManagerImpl.shutdown() 这行代码，为了实现真正的优雅停机功能，这个时候就要对 LogManagerImpl 日志管理器进行一下重构，按照我们刚才分析的思路，一旦 LogManagerImpl 日志管理器组件要关闭了，它就不能再接收新的日志数据了，然后还要把待处理的日志都刷新到硬盘中，这一切都执行完毕之后了，才能真正关闭 LogManagerImpl 日志管理器组件使用的 Disruptor 框架的批处理器线程**。这个逻辑应该也比较清楚吧？

如果大家理解了上面的逻辑，那以此类推，是不是 FSMCallerImpl 状态机组件的 shutdown() 方法也要被重构了？因为优雅停机之前，状态机组件必须把待处理的日志应用到状态机上。**总而言之，只要 jraft 集群节点启动的组件是和数据处理打交道的，不管是什么数据，都要在优雅停机的时候先把数据处理完毕了，然后才能真正停机**。那这个时候终极问题就来了：**在 sofajraft 框架，有哪些组件是需要和数据打交道的呢？除了 NodeImpl 对象本身，目前我们已经分析出另外的三个了，一个就是 LogManagerImpl 日志管理器，一个就是 FSMCallerImpl 状态机组件，还有一个就是之前简单提到的 LocalRaftMetaStorage 节点元数据保存器**。那除了这几个组件，还有哪些组件具备这样的特性呢？在找到了这些组件之后，这些组件在优雅停机的时候应该执行什么操作呢？这些问题在这一章显然是讲不完了，就留到下一章讲解吧。好了朋友们，我们下一章见！

  

  

附：本篇文章对应我为大家提供的第12版本代码，但是在阅读完本片文章之后，大家还不能完全看明白第12版本的代码，因为本篇文章只实现了第12版本代码的部分功能，等下一篇文章结束之后，也许大家就可以去阅读第12版本的全部代码了。