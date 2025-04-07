上一章结尾我跟大家提出了三个问题：

1 **领导者节点在优雅停机的时候需要从集群其他节点中找到一个可以成为候选者的节点，以便于集群尽快产生新的领导者，那么领导者节点该怎么找到可以成为候选者的节点呢？**

**2 领导者内部还管理者非常多的 Replicator 复制器对象，每一个 Replicator 复制器对象都对已经集群一个跟随者节点，领导者停机的时候，这些 Replicator 复制器对象要怎么办呢？**

**3 raft 集群节点把日志持久化到本地的时候，是要存储在 RocksDB 数据库中，领导者停机的时候，相关数据库的资源该如何释放呢**？

接下来我就为大家依次分析一下。

## 实现 Replicator 复制器对象的关闭操作

在上一章我已经给大家分析了，当 jraft 集群领导者节点要停机的时候，就不能再向集群的跟随者节点传输任何日志了；当 jraft 集群启动的时候，领导者会在自己内部创建多个 Replicator 复制器对象，每一个 Replicator 复制器对象就对应着集群中的一个跟随者节点，领导者就是使用内部创建的每一个 Replicator 复制器对象和集群中的每一个跟随者节点通信。所谓通信就包括发送心跳消息、传输日志、安装快照等等。但我们之前也分析了，领导者在停机的时候就不能再向集群的跟随者节点发送新的数据了，这就意味着当 jraft 集群领导者停机的时候，它就不能再使用内部的每一个 Replicator 复制器对象和对应的跟随者节点通信。这个时候领导者就要执行一个最简单直接的操作：**那就是在它优雅停机的时候，首先把内部所有的 Replicator 复制器对象都关闭了**。这个操作非常重要，接下来就让我为大家分析一下，执行这个操作之后，领导者节点内部会发生什么变化。

当 jraft 集群的领导者执行了优雅停机操作时，需要让内部的 Replicator 复制器对象都停止工作，这么做的原因我们也都很清楚了，在领导者停机的时候，领导者就不能再向集群中的跟随者节点传输任何新的数据了，所以要关闭内部的所有 Replicator 跟随者节点。这样一来，jraft 集群的领导者就需要执行下面这段代码，请看下面代码块。

```
//这里是一段伪代码，大家先理解意思即可
public void stopAllReplicator() {

     //遍历领导者内部的所有Replicator复制器对象
    //每一个Replicator复制器对象内部都有一个ThreadId线程Id对象
    //这里遍历的每一个ThreadId线程Id对象其实就对应了每一个Replicator复制器对象
    for (final ThreadId r : this.replicatorMap.values()) {
        //终止当前复制器对象工作
        Replicator.stop(r);
    }
 }
```

jraft 集群领导者停机的时候，就应该先执行上面代码块中的方法，把内部创建的所有 Replicator 复制器对象全部关闭，这样一来，从这一刻起，领导者就不能再通过 Replicator 复制器对象和集群中对应的跟随者节点通信了。当然，我上面只是给大家展示了一段伪代码，到目前为止，大家也仅仅是知道 jraft 集群领导者在停机时要终止内部所有 Replicator 复制器对象工作，而调用上面代码块中展示方法就可以实现这个功能。但是上面代码块中的方法如何被调用，以及 Replicator 复制器对象的 stop() 方法被调用了之后会发生什么，大家都还不知道。接下来我就为大家分析一下当领导者节点执行了 stopAllReplicator() 方法后，程序内部会发生什么。

当领导者节点执行了 stopAllReplicator() 方法后，领导者内部的每一个 Replicator 复制器对象的 stop() 对象就会被调用。而当每一个 Replicator 复制器对象调用了 stop() 方法之后，要执行的操作其实非常简单，和我们之前总结的三个要点没什么区别，只不过这里还涉及到了部分任务的取消。我同样为大家总结了三点，展示在下面了：

**1 把当前 Replicator 复制器组件要终止工作的标志更新了，一旦该标志被更新，那么当前 Replicator 复制器对象就不能再把日志组件中的日志传输给对应的跟随者节点了，也不必再远程暗转快照文件了。**

**2 如果 Replicator 复制器对象要关闭的时候，该对象正在把一批数据传输给对应的跟随者节点，那么当前 Replicator 复制器对象就不能关闭，也就意味着不能跟新自己的关闭标志，等到当前 Replicator 复制器对象把数据传输给对应的跟随者节点了，把 Replicator 复制器对象持有的同步锁释放了，当前 Replicator 复制器对象才能执行真正的关闭操作。**

**3 取消部分任务的执行**。

在阅读了以上三个要点之后，接下来我就为大家展示一下 Replicator 复制器 stop() 方法的内容，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/23
 * @Description:整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
 * 复制器对象，心跳检测，日志传输都是通过复制器对象进行的
 */
@ThreadSafe
public class Replicator implements ThreadId.OnError {

    //省略该类的其他内容

    //这个其实就可以看成一个句柄，复制器对象的引用被ThreadId对象持用
    //该对象会交给复制器组管理
    protected ThreadId id;

    //所有发给跟随者的请求的信息都会被包装到Inflight对象中，在还没有收到对应的响应前，请求会一直存放在这个队列中
    //请求的响应被处理了，才会将请求从队列中移除
    //每次添加Inflight对象，都会从队列尾部添加，这样序号越小的Inflight对象，就会放在队首了
    private final java.util.ArrayDeque<Inflight> inflights = new ArrayDeque<>();

    //这个成员变量会被记录最后发送给跟随者的请求，也可以说是最新的请求
    private Inflight rpcInFly;

    //心跳定时器
    private ScheduledFuture<?> heartbeatTimer;
    
    //正在执行的心跳任务
    private Future<Message> heartbeatInFly;

    //复制器状态阻塞时要用到的定时器
    private ScheduledFuture<?> blockTimer;

    //当前领导者中没有日志的时候，复制器无法向跟随者发送日志消息，于是就会等待日志产生
    //这个就是为当前复制器分配的等待ID
    private long waitId = -1L;

    //当前复制器的状态，这个状态
    private volatile State state;


    //这个枚举类中的对象代表的是复制器当前的状态
    public enum State {
        //复制器已被创建
        Created,
        //复制器发送了探针消息
        Probe,
        //复制器正在给跟随者安装快照
        Snapshot,
        //复制器正在向跟随者传输日志
        Replicate,
        //复制器已被销毁
        Destroyed
    }

    

    //该方法最终就会调用到
    public static boolean stop(final ThreadId id) {
        id.setError(id,this,RaftError.ESTOP.getNumber());
        return true;
    }


    //在该方法中执行了关闭当前Replicator复制器对象的操作
    @Override
    public void onError(final ThreadId id, final Object data, final int errorCode) {
        //得到当前复制器对象
        final Replicator r = (Replicator) data;
        //判断当前复制器对象是不是要关闭了
        if (errorCode == RaftError.ESTOP.getNumber()) {
            try {//取消正在执行的任务，注意，接下来取消的所有任务并不是说会把任务终止
                //虽然取消了这些任务，但是任务都会执行完毕，只不过当前复制器对象不再关心任务的执行结果了
                for (final Inflight inflight : r.inflights) {
                    if (inflight != r.rpcInFly) {
                        inflight.rpcFuture.cancel(true);
                    }
                }
                if (r.rpcInFly != null) {
                    r.rpcInFly.rpcFuture.cancel(true);
                    r.rpcInFly = null;
                }
                if (r.heartbeatInFly != null) {
                    r.heartbeatInFly.cancel(true);
                    r.heartbeatInFly = null;
                }
                if (r.heartbeatTimer != null) {
                    r.heartbeatTimer.cancel(true);
                    r.heartbeatTimer = null;
                }
                if (r.blockTimer != null) {
                    r.blockTimer.cancel(true);
                    r.blockTimer = null;
                }
                if (r.waitId >= 0) {
                    //这里就会把存放到日志管理器的waitMap中的数据全部清除
                    r.options.getLogManager().removeWaiter(r.waitId);
                }
            } finally {
                //在这里执行销毁当前Replicator复制器对象的操作
                r.destroy();
            }
        }

        //省略该方法的其他内容
    }



    //销毁复制器对象的方法，注意这里的销毁并不是说直接把该复制器对象销毁了，而是设置一个销毁状态
    //这个复制器对象就不能再执行相关的操作了，至于释放该对象内部的资源，具体逻辑请从NodeImpl类的shutdown()方法看起
    void destroy() {
        final ThreadId savedId = this.id;
        LOG.info("Replicator {} is going to quit", savedId);
        releaseReader();
        //在这里把当前Replicator复制器的状态设置成已销毁
        setState(State.Destroyed);
        savedId.unlockAndDestroy();
        //把复制器对象要使用的线程Id对象置空
        this.id = null;
    }


    //设置当前复制器的状态
    void setState(final State state) {
        this.state = state;
    }
    

    //省略该类的其他内容
}
```

上面代码块实现的就是 Replicator 复制器对象执行了 stop() 方法之后要执行的操作的具体流程，可以看到，正如我之前为大家总结的那样，当 Replicator 复制器对象执行了它的 stop() 方法后，就会先把内部执行的一些任务给取消了，当然，**取消这些任务并不意味着任务不执行了，而是当前 Replicator 复制器对象不再关注任务的执行结果了，也就是说，如果 Replicator 复制器正在把一些数据传输给对应的跟随者节点，那这些数据仍然会传输完毕**。这一点，大家可以自己去我提供的第 12 版本代码或者源码中印证；**接着又在 stop() 方法中执行了 destroy() 方法，而该方法的执行，就会把当前 Replicator 复制器对象的状态更新为 Destroyed 已销毁状态，然后把当前 Replicator 复制器对象使用的 ThreadId 线程 Id 对象置空**。这些流程都很清晰，对大家来说应该都很容易理解。

看到这里，我相信大家都很好奇，当 Replicator 复制器对象的状态更新为已销毁，并且内部使用的线程 Id 对象也置空了，那么当前 Replicator 复制器对象怎么根据这些标志的更新，拒绝向对应的跟随者节点传输数据呢？接下来我就以 Replicator 复制器对象向对应的跟随者节点传输日志为例，让大家看看 Replicator 复制器对象在执行了 stop() 方法之后，是怎样不再继续向对应的跟随者节点传输日志的了。

当 jraft 集群领导者向跟随者节点传输日志的时候，会调用每一个 Replicator 复制器对象的 sendEntries() 方法，把日志管理器中的日志传输给对应的跟随者节点，具体代码如下，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/23
 * @Description:整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
 * 复制器对象，心跳检测，日志传输都是通过复制器对象进行的
 */
@ThreadSafe
public class Replicator implements ThreadId.OnError {


    //省略该类其他内容


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/7
     * @Description:发送日志给跟随者的方法，这里要再次强调一下，在sofajraft中，领导者并不会每次只发送一条日志给跟随者，而是会批量发送日志
     * 每次都会发送一批日志，每一次具体发送多少日志，实现逻辑请看sendEntries方法
     */
    void sendEntries() {
        boolean doUnlock = true;
        try {
            long prevSendIndex = -1;
            while (true) {
                //在一个循环中，不断获取要发送的日志的索引，然后发送日志，直到获取到的索引
                //超过了能够发送的日志，也就是说，领导者中的日志已经暂时发送完毕了
                //这时候就会跳出循环
                final long nextSendingIndex = getNextSendIndex();
                //这里我要多强调一句，这里很可能会发送多批次日志
                if (nextSendingIndex > prevSendIndex) {
                    
                    //在这里执行传输日志给跟随者节点的操作
                    if (sendEntries(nextSendingIndex)) {
                        prevSendIndex = nextSendingIndex;
                    } 
                    
                    
                    else {//走到这里意味着发送失败，可以退出该方法了
                        //但是这并不是意味着领导者就不再向跟随者发送日志了，有可能
                        //是领导者中目前没有日志了，这时候复制器会给领导者的日志管理器
                        //注册一个回调方法，当日志管理器中有新的日志放到内存了，就会通知
                        //复制器继续向跟随者复制日志，很快就会看到具体的逻辑了
                        doUnlock = false;
                        break;
                    }
                } else {//走到这里意味着获得的下一次要发送的日志的索引并不大于上一次
                    break;
                }
            }
        } finally {
            if (doUnlock) {
                unlockId();
            }
        }
    }



     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/7
     * @Description:批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志
     */
    private boolean sendEntries(final long nextSendingIndex) {

        //省略该方法其他内容


        Future<Message> rpcFuture = null;
        
        try {//传输日志给跟随者节点
            rpcFuture = this.rpcService.appendEntries(this.options.getPeerId().getEndpoint(), request, -1,
                    new RpcResponseClosureAdapter<RpcRequests.AppendEntriesResponse>() {

                        @Override
                        public void run(final Status status) {
                           
                            RecycleUtil.recycle(recyclable);
                            
                            //这个方法会在接收到响应后被回调，这里把本次请求的类型，复制器对象本身，响应状态码，本次请求对象，得到的响应
                            //请求序号，版本号，本次请求发送的时间都传到方法中了
                            onRpcReturned(Replicator.this.id, RequestType.AppendEntries, status, request, getResponse(),
                                    seq, v, monotonicSendTimeMs);
                        }
                    });
        } catch (final Throwable t) {
            RecycleUtil.recycle(recyclable);
            ThrowUtil.throwException(t);
        }
        //把本次请求的信息添加到inflights队列中
        addInflight(RequestType.AppendEntries, nextSendingIndex, request.getEntriesCount(), request.getData().size(),
                seq, rpcFuture);
        return true;

        
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/7
     * @Description:该方法会在探针请求收到响应后被回调，也会在给跟随者发送传输日志的请求，收到响应后被回调
     */
    @SuppressWarnings("ContinueOrBreakFromFinallyBlock")
    static void onRpcReturned(final ThreadId id, final RequestType reqType, final Status status, final Message request,final Message response, final int seq, final int stateVersion, final long rpcSendTime){

        //省略该方法其他内容


        //在经过一系列判断，发现前一批日志传输成功之后，就可以开始执行传输下一批日志的操作了
        //也就是在这里调用sendEntries()方法
       
        //在这里继续发送下一批日志
        r.sendEntries();
            
    }


    //省略该类其他内容
}
```

以上代码快展示的就是 Replicator 复制器对象向对应的跟随者节点传输日志的具体流程。当 Replicator 复制器对象调用了它的 sendEntries() 方法，就会开始向对应的跟随者节点传输日志，要传输的日志会被封装到 AppendEntriesRequest 请求对象中发送给跟随者节点，并且当跟随者节点回复了响应后，当前 Replicator 复制器对象的 onRpcReturned() 方法就会被回调。如果在处理响应的时候，当前 Replicator 复制器对象发现上一批日志传输成功了，那么就会接着调用 Replicator 复制器对象的 sendEntries() 方法，开始传输下一批日志。以上就是 Replicator 复制器对象向跟随者节点传输日志的核心流程。

好了朋友们，现在我们要分析的是当 Replicator 复制器对象优雅关闭的时候，Replicator 复制器对象是怎么停止向跟随者节点传输日志信息的。这个其实非常容易实现，我们刚才在实现 Replicator 复制器对象 stop() 关闭的功能时，把 Replicator 复制器对象内部使用的 ThreadId 线程对象置空了，这就意味着，**只要 ThreadId 线程对象为空了，就意味着 Replicator 对象要关闭了，这个时候 Replicator 复制器对象也就不应该再继续向对应的跟随者节点传输日志**。接下来就很简单了，**那我们直接就在 onRpcReturned() 方法中对当前 Replicator 复制器对象的 ThreadId 判空即可**。**如果发现线程对象为空，那就直接退出 onRpcReturned() 方法，不再继续传输下一批日志，这样一来，不就做到了让当前 Replicator 复制器对象把正在传输的日志传输完毕，但是不再继续传输新的日志给跟随者节点了吗**？我把重构之后的 onRpcReturned() 方法展示在下面的代码块中了，请看下面代码块。

```
@ThreadSafe
public class Replicator implements ThreadId.OnError {


    //省略该类其他内容

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/12/7
     * @Description:该方法会在探针请求收到响应后被回调，也会在给跟随者发送传输日志的请求，收到响应后被回调
     */
    @SuppressWarnings("ContinueOrBreakFromFinallyBlock")
    static void onRpcReturned(final ThreadId id, final RequestType reqType, final Status status, final Message request,final Message response, final int seq, final int stateVersion, final long rpcSendTime){

        

        //对ThreadId判空
        if (id == null) {
            //如果为空直接退出当前方法，不再继续向跟随者节点传输日志
            return;
        }

        //省略该方法其他内容

        //在经过一系列判断，发现前一批日志传输成功之后，就可以开始执行传输下一批日志的操作了
        //也就是在这里调用sendEntries()方法
       
        //在这里继续发送下一批日志
        r.sendEntries();
            
    }


    //省略该类其他内容
}
```

好了朋友们，到此为止，在本章开篇提出来的三个问题中的第二个问题，我就为大家解决完毕了。我们已经清楚地知道了，在 jraft 集群领导者优雅停机的时候，领导者是怎么处理内部创建的所有 Replicator 复制器对象的。当然，目前我们还不知道每一个 Replicator 复制器对象的 stop() 方法究竟应该在哪里被调用，这个也不必着急，很快我就会为大家展示了。接下来，让我们先看看本章开篇提出来的三个问题中的第一个问题，也就是：**领导者节点在优雅停机的时候需要从集群其他节点中找到一个可以成为候选者的节点，以便于集群尽快产生新的领导者，那么领导者节点该怎么找到可以成为候选者的节点呢**？

## 重构 ReplicatorGroupImpl

  

要想解决第一个问题，那我们就要重新回到 NodeImpl 类中，在前面的章节我已经为大家把 NodeImpl 类的 shutdown() 方法重构完整了，在该方法中，我们调用了 jraft 集群节点中各个组件的 shutdown() 方法，并且把当前节点的状态更新为正在关闭状态了。现在我想补充一点的是，很明显 NodeImpl 对象的 shutdown() 就是当前节点优雅停机的入口方法，如果当前节点是领导者，**那么也应该以 NodeImpl 对象的 shutdown() 为入口方法，在该方法中执行终止内部所有 Replicator 复制器对象工作的操作**。就像下面代码块中展示的这样，请看下面代码块。

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

            
            //如果当前节点的状态小于State.STATE_FOLLOWER，就说明当前节点不是领导者就是候选者，要不就是跟随者
            //这里是考虑到当前节点是领导者或者候选者，因为是领导者的话，当前节点需要销毁所有的复制器对象，清空投票箱等等，以及通知其他节点变成候选者
            //这样就可以尽快选出新的领导者了
            //如果是候选者结点的话，就需要停止选举超时定时器等等组件
            if (this.state.compareTo(State.STATE_FOLLOWER) <= 0) {
                //执行身份降级操作，注意，这里有一个很重要的操作，那就是方法中的第二个参数this.state == State.STATE_LEADER
                //这就意味着如果当前节点是领导者，也就意味着领导者正在优雅停机，那这里就传进去true，这个true应该怎么使用，请大家进入stepDown()方法中查看
                   stepDown(this.currTerm, this.state == State.STATE_LEADER,
                           new Status(RaftError.ESHUTDOWN, "Raft node is going to quit."));
            }

            
            //因为要停机了，在这里更新节点状态为State.STATE_SHUTTING
            //表示正在停止工作
            this.state = State.STATE_SHUTTING;

            //停止所有定时器工作
            List<RepeatedTimer> timers = stopAllTimers();

            //以下就是执行各个组件shutdown()方法的操作，注意，这里只是执行了各个组件的
            //shutdown()方法，该方法一旦执行，并不意味着各个组件立刻被关闭了，实际上该方法的
            //执行，会让各个组件尽快处理内部尚未处理完毕的数据


            //省略该部分内容

            
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

在上面代码块的第 27 到 32 行，**大家可以看到我在 NodeImpl 对象的 shutdown() 方法中添加了一个新的操作，那就是执行了 stepDown() 节点降级方法。不管是领导者，还是候选者，还是跟随者都可以执行这个方法**。如果当前要停机的是跟随者或者候选者节点，那就在 stepDown() 方法中就没什么要特别执行的相关操作，但如果当前节点是领导者，**那么我们就可以在领导者刚开始停机的时候，执行这个 stepDown() 方法，并且在 stepDown() 方法中让领导者节点终止内部所有 Replicator 复制器对象工作，并且从集群其他节点中找到可以成为候选者的节点，然后通知该节点进入领导者选举阶段**。根据刚才的分析，我把早就为大家实现完毕的 stepDown() 方法又重构了一番，接下来就为大家展示一下，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容


    //复制器组
    private ReplicatorGroup replicatorGroup;

    //领导者停机时找到的候选者节点
    private ThreadId wakingCandidate;


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

            
            //如果当前节点的状态小于State.STATE_FOLLOWER，就说明当前节点不是领导者就是候选者，要不就是跟随者
            //这里是考虑到当前节点是领导者或者候选者，因为是领导者的话，当前节点需要销毁所有的复制器对象，清空投票箱等等，以及通知其他节点变成候选者
            //这样就可以尽快选出新的领导者了
            //如果是候选者结点的话，就需要停止选举超时定时器等等组件
            if (this.state.compareTo(State.STATE_FOLLOWER) <= 0) {
                //执行身份降级操作，注意，这里有一个很重要的操作，那就是方法中的第二个参数this.state == State.STATE_LEADER
                //这就意味着如果当前节点是领导者，也就意味着领导者正在优雅停机，那这里就传进去true，这个true应该怎么使用，请大家进入stepDown()方法中查看
                   stepDown(this.currTerm, this.state == State.STATE_LEADER,
                           new Status(RaftError.ESHUTDOWN, "Raft node is going to quit."));
            }

            
            //因为要停机了，在这里更新节点状态为State.STATE_SHUTTING
            //表示正在停止工作
            this.state = State.STATE_SHUTTING;

            //停止所有定时器工作
            List<RepeatedTimer> timers = stopAllTimers();

            //以下就是执行各个组件shutdown()方法的操作，注意，这里只是执行了各个组件的
            //shutdown()方法，该方法一旦执行，并不意味着各个组件立刻被关闭了，实际上该方法的
            //执行，会让各个组件尽快处理内部尚未处理完毕的数据


            //省略该部分内容

            
            //关闭全局定时任务调度器
            timerManager.shutdown();

             //销毁所有定时器
            destroyAllTimers(timers);
        
            //等待以上组件都处理完待处理的数据
            join();
            
        }
       
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/11/22
     * @Description:检测当前节点是否需要进行身份降级的操作，学习这个方法时候，不要只看这个方法，而要看看
     * 这个方法会在哪些方法中被调用，结合外层方法一起学习该方法的功能和逻辑，否则你虽然看懂了这个方法的每一行代码
     * 但根本不明白每一行代码背后的具体作用，当然这个方法我也做了大量删减，会在第13版本代码中重构完整，让大家看看领导者下台时会执行什么善后操作
     */
    private void stepDown(final long term, final boolean wakeupCandidate, final Status status) {
        LOG.debug("Node {} stepDown, term={}, newTerm={}, wakeupCandidate={}.", getNodeId(), this.currTerm, term,
                wakeupCandidate);
        //检查当前节点的状态是否活跃。其实就是看看是否正在关闭或者有没有异常
        if (!this.state.isActive()) {
            return;
        }

        //判断当前节点是不是候选者
        if (this.state == State.STATE_CANDIDATE) {
            //如果当前是候选者节点，那么在候选者节点中有一个voteTimer定时器，这个定时器会定期执行定时任务，判断
            //当前节点是不是候选者，如果是候选者，那么接着就会判断当前候选者的获得其他节点投票的操作是否超时了
            //如果超时了就对当前候选者执行降级处理。所谓降级处理就会调用当前的stepDown()方法
            //既然降级了，当前节点肯定就不是候选者了，那就不用再继续启动这个定时器了，所以这里要停止该定时器工作
            //但是我在为大家迭代的代码中，并没有引入这个定时器，所以我就把下面这个操作注释掉了
            //stopVoteTimer();

            //这里就是判断当前节点是不是领导者，或者处于领导者权力转移阶段
        } else if (this.state.compareTo(State.STATE_TRANSFERRING) <= 0) {
            //如果当前节点是领导者，那么当前节点中会有一个stepDownTimer定时器，这个定时器会定期判断
            //领导者是否要下台了，判断的原理也很简单，就是判断领导者内部保存的Replicator复制器对象是否都还存活
            //如果有Replicator复制器对象死亡了，也就是在限制时间之内没有顺利进行通信，那就可以认为与Replicator复制器对应的节点失联了
            //如果存活的Replicator复制器对象小于集群节点总数的一半，就可以认为当前领导者应该下台了，然后执行相应的操作即可
            //但是我在为大家迭代的代码中，也没有引入这个定时器，因为要引入的相关方法有点多，所以我就把下面这个操作注释掉了
            //大家理解其中的设计原理即可
            //stopStepDownTimer();
            //清空领导者的投票箱
            this.ballotBox.clearPendingTasks();

            //判断当前节点是不是领导者
            if (this.state == State.STATE_LEADER) {
                //如果是领导者，现在领导者要下台或者停机了
                //那就让当前领导者的状态机停止工作
                onLeaderStop(status);
            }
        }

        //重置当前节点的领导者信息
        resetLeaderId(PeerId.emptyPeer(), status);
        //将当前节点的状态变更为跟随者
        this.state = State.STATE_FOLLOWER;
        //重置当前节点的配置信息的上下文
        this.confCtx.reset();
        //跟新最后一次收到领导者信息的时间，这里更新这个时间主要是因为stepDown方法会在
        //多个和投票选举有关的方法中被调用，既然正在投票选举，肯定就不能让超时选举定时器工作
        //所以每次都要更新这个时间，这样超时选举定时器就不会真正开始进行选主工作了
        updateLastLeaderTimestamp(Utils.monotonicMs());
        if (this.snapshotExecutor != null) {
            //打断下载快照的操作
            this.snapshotExecutor.interruptDownloadingSnapshots(term);
        }
        //如果方法参数中的任期必当前节点的任期大，这时候就意味着当前节点正在进行选举工作
        //这个stepDown方法可能正在被handleRequestVoteRequest方法调用
        if (term > this.currTerm) {
            //修改当前节点任期
            this.currTerm = term;
            //这里先把当前节点的投票记录设置为空，因为到了一个新的任期了，一个任期只能投一票
            //但是这个新的任期中还没有投呢，所以先设置为空，在外层的handleRequestVoteRequest方法中会投票
            //然后给当前节点的投票记录复制
            this.votedId = PeerId.emptyPeer();
            //持久化元数据信息
            this.metaStorage.setTermAndVotedFor(term, this.votedId);
        }
        
        
        
        //这里就用到了方法参数中的wakeupCandidate，如果wakeupCandidate为true，意味着当前节点就是领导者，这一点可以从外层方法查看
        //那当前领导者节点要下台了，这里的意思就是为jraft集群唤醒候选者，也就是要尽快让某个候选者成为集群新的领导者
        //也就是尽快为集群选举新的领导者的操作
        if (wakeupCandidate) {
            //在这里执行停止领导者内部所有复制器工作的操作
            //并且还要找到候选复制器，所谓候选复制器其实就是看看集群其他节点哪一个节点的选举优先级最高
            //哪一个节点内部存储的日志索引更大，就通知这个节点成为候选者，开始执行领导者选举的操作
            this.wakingCandidate = this.replicatorGroup.stopAllAndFindTheNextCandidate(this.conf);
            if (this.wakingCandidate != null) {
                //如果找到了这个候选复制器，那就向这个复制器对应的节点发送请求，通知这个复制器对应的节点率先进入领导者选举的操作
                Replicator.sendTimeoutNowAndStop(this.wakingCandidate, this.options.getElectionTimeoutMs());
            }
        } else {
            //领导者要下台了，但是不通知候选者进行选举，那就直接停止内部所有复制器对象工作
            this.replicatorGroup.stopAll();
        }
        
        
        
        //如果当前节点不是学习者，那就启动当前节点的超时选举定时器
        if (!isLearner()) {
            //启动超时选举定时器，走到这里，就意味着当前的stepDown方法，也可能是在NodeImpl类的init方法中被调用的
            //大家可以仔细研究研究，一定要理清楚是哪个外层方法在调用当前的stepDown方法
            //当然即便不是init方法中调用的，也需要重新启动超时选举定时器，因为该节点马上就要投完票了，这意味着新的领导者即将产生
            //如果没产生，那么就根据超时时间继续进行新一轮选举，这正是超时选举定时器的工作
            this.electionTimer.restart();
        } else {
            LOG.info("Node {} is a learner, election timer is not started.", this.nodeId);
        }
    }


    

    //省略该类其他内容
}
```

上面代码块中的注释非常详细，我就不再为大家重复解释其中的操作流程了。总之大家可以在上面代码块的第 143 行到 155 行看到，**如果程序判断出来当前节点是领导者节点，并且要停机的领导者需要唤醒集群中的候选者节点，也就是从其他节点中找到一个可以成为候选者的节点，那么领导者就会执行 ReplicatorGroup 复制器组的 stopAllAndFindTheNextCandidate() 方法，该方法就是用来停止领导者内部的 Replicator 复制器对象工作，然后找到可以成为候选者的节点的；如果找到了该候选者节点，就会调用 Replicator 复制器对象的 sendTimeoutNowAndStop() 方法，通知这个节点成为候选者，然后主动进入领导者选举阶段。如果领导者不需要唤醒集群中的候选者，那么就会执行执行 ReplicatorGroup 复制器组对象的 stopAll() 方法，终止领导者内部所有 Replicator 复制器对象工作**。到此为止，大家应该就清楚了领导者优雅停机时，是在什么时候终止内部所有 Replicator 复制器对象工作的，又是在什么时候找到了集群中可以成为候选者的节点。当然，现在我也只是刚刚把 ReplicatorGroup 复制器中要调用的方法名称展示给大家了，并没有真的为大家实现这些方法，也就是说，我还没有为大家重构 ReplicatorGroup 复制器组。但是说实话，这些方法实现起来非常简单。

**就比如说停止领导者内部所有 Replicator 复制器对象工作吧，ReplicatorGroup 复制器组管理者领导者内部所有 Replicator 复制器对象，那么直接遍历这些 Replicator 复制器对象，依次调用它们的 stop() 方法不就行了？而找打集群中可以成为候选者节点的操作就更简单了，无非就是看看集群中其他节点谁从领导者同步的日志进度最多，如果同步的进度一样，那就对比一下这些节点的优先级，优先级高的则可以成为候选者节点**。分析起来是不是非常简单？接下来我就把重构完毕的 ReplicatorGroup 复制器组展示给大家，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/23
 * @Description:该类的对象就是复制器组，复制器组对象管理着所有的复制器对象，复制器对象是相对于领导者来说的
 * 一个节点一旦成为领导者，那么这个节点就会根据配置类中封装的集群中节点的信息，把其他的跟随者节点包装成一个个复制器对象
 * 日志复制就是通过复制器对象实现的
 */
public class ReplicatorGroupImpl implements ReplicatorGroup {


    //这个Map中的value是一个ThreadId对象，这个对象并不真的是创建了一个线程，然后给这个线程一个ID
    //实际上，这个ThreadId对象内部就持有者PeerId对应的节点包装成的复制器对象，每一个复制器对象都是独立的个体
    //也不会共用线程处理复制器中的回调方法，因为就将每个复制器对象封装到了一个threadId对象中，代表独立的对象
    private final ConcurrentMap<PeerId, ThreadId> replicatorMap = new ConcurrentHashMap<>();


    //省略其他内容


    //停止领导者内部所有Replicator复制器对象工作的方法
    @Override
    public boolean stopAll() {
        final List<ThreadId> rids = new ArrayList<>(this.replicatorMap.values());
        this.replicatorMap.clear();
        this.failureReplicators.clear();
        for (final ThreadId rid : rids) {
            Replicator.stop(rid);
        }
        return true;
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/27
     * @方法描述：该方法会停止领导者内部复制器对象工作，并且找到一个可以当作候选者的节点，然后会把与这个候选者对应的Replicator复制器对象的线程返回
     */
    public ThreadId stopAllAndFindTheNextCandidate(final ConfigurationEntry conf) {
        //定义一个线程Id对象，用来保存找到的候选者节点对应的复制器对象线程Id
        ThreadId candidate = null;
        //通过节点的配置信息从集群其他节点中找到一个可以成为候选者的节点
        final PeerId candidateId = findTheNextCandidate(conf);
        //判断是否找到了可以成为候选者的节点
        if (candidateId != null) {
            //如果找到了就得到与该节点对应的线程Id对象
            candidate = this.replicatorMap.get(candidateId);
        } else {
            //没找到则记录日志信息
            LOG.info("Fail to find the next candidate.");
        }
        //遍历集群中的所有节点对应的线程Id对象
        for (final ThreadId r : this.replicatorMap.values()) {
            //因为现在领导者要下台了，这里要做的就是停止内部每一个Replicator复制器对象工作
            //但是，刚才找到的候选者节点对应的Replicator复制器对象还不能停止工作
            //因为即将下台的领导者还要使用候选者节点的Replicator对象和集群中的候选者节点通信
            //让该节点的身份变更为候选者，然后主动进入领导者选举阶段
            //而与该候选者节点对应的Replicator对象，则会在NodeImpl对象的shutdown()方法中被停止
            //大家可以直接看我提供的第12版本代码即可
            if (r != candidate) {
                //在这里把非候选者节点的其他节点对应的Replicator复制器对象停止了
                Replicator.stop(r);
            }
        }
        //以下就是释放资源的操作
        this.replicatorMap.clear();
        this.failureReplicators.clear();
        //返回候选者Replicator复制器对象使用的线程Id对象
        return candidate;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/27
     * @方法描述：从集群其他节点中找到下一个候选者的方法
     */
    @Override
    public PeerId findTheNextCandidate(final ConfigurationEntry conf) {
        //定义一个节点信息对象，用来接收找到的候选者节点
        PeerId peerId = null;
        //定义一个最小优先级
        int priority = Integer.MIN_VALUE;
        //定义一个最小日志索引
        long maxIndex = -1L;
        //遍历集群中的所有节点的信息
        for (final Map.Entry<PeerId, ThreadId> entry : this.replicatorMap.entrySet()) {
            //如果当前遍历到的节点的信息不在领导者内部保存的配置信息中，那就跳过本次循环
            if (!conf.contains(entry.getKey())) {
                continue;
            }
            //得到当前遍历的节点的选举优先级
            final int nextPriority = entry.getKey().getPriority();
            //如果选举优先级和ElectionPriority.NotElected相等，那就意味着当前遍历到的节点永远不会参与领导者选举
            //也就是说，这个节点永远不会成为候选者，它只能给候选者投票，这样一来，那就可以直接跳过本次循环了
            if (nextPriority == ElectionPriority.NotElected) {
                continue;
            }
            //得到当前遍历到的节点的下一条日志索引，其实就是领导者要发送给当前节点的下一条日志的索引
            //这也就代表了当前节点同步领导者数据的进度
            final long nextIndex = Replicator.getNextIndex(entry.getValue());
            //因为当前的操作会循环遍历集群中的节点，也就是说会查看集群中其他节点的同步数据的进度
            //肯定是同步数据进度最多的节点被提拔为候选者的概率大，所以下面就是找到同步数据最多的节点信息的操作
            if (nextIndex > maxIndex) {
                //更新最大索引
                maxIndex = nextIndex;
                //更新候选者节点信息，这里就是看看哪个节点同步数据更多，就让哪个节点率先成为候选者
                peerId = entry.getKey();
                //更新找到的节点的选举优先级
                priority = peerId.getPriority();
                //如果现在有两个节点，这两个节点同步数据的进度是一样的，那这种情况下就对比一下这两个节点谁的选举优先级高
                //高的那个就可以率先成为候选者
            } else if (nextIndex == maxIndex && nextPriority > priority) {
                //更新进度相同，谁的优先级高谁就可以先成为候选者节点
                peerId = entry.getKey();
                //找到的候选者节点的优先级信息
                priority = peerId.getPriority();
            }
        }
        if (maxIndex == -1L) {
            //执行到这里意味着日志索引没有更新，也就意味着在上面的操作中根本没有更新maxIndex变量
            //这也就意味着没有找到候选者节点，直接返回null即可
            return null;
        } else {
            //返回找到的候选者节点的信息
            return peerId;
        }
    }

    
}
```

好了朋友们，现在我们为 jraft 集群领导者成功实现了从集群其他节点中找到可以成为候选者节点的功能，但是仅仅实现了这个功能还不够，因为领导者已经优雅停机了，**raft 集群没有领导者就不能正常工作。所以领导者在优雅停机时还要主动通知找到的这个节点尽快把自己的身份更新为候选者，这样一来该节点就可以主动进入领导者选举阶段了**。而这个操作需要调用与目标节点对应的 Replicator 复制器对象的 sendTimeoutNowAndStop() 方法才能实现，所以接下来我就为大家实现一下 Replicator 复制器对象的 sendTimeoutNowAndStop() 方法。请看下面代码块。

```
public class Replicator implements ThreadId.OnError {


    //省略该类其他内容

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/28
     * @方法描述：即将下机的领导者向被选中的候选者节点发送统治的方法，目标节点接收到通知之后，就可能会成为候选者节点，率先进入领导者选举阶段
     */
    public static boolean sendTimeoutNowAndStop(final ThreadId id, final int timeoutMs) {
        final Replicator r = (Replicator) id.lock();
        if (r == null) {
            return false;
        }
        //在这里发送通知给目标节点
        r.sendTimeoutNow(true, true, timeoutMs);
        return true;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/28
     * @方法描述：给目标候选者节点发送TimeoutNowRequest请求的方法
     */
    private void sendTimeoutNow(final boolean unlockId, final boolean stopAfterFinish, final int timeoutMs) {
        //创建TimeoutNowRequest请求构建器
        final RpcRequests.TimeoutNowRequest.Builder rb = RpcRequests.TimeoutNowRequest.newBuilder();
        //设置领导者节点的任期
        rb.setTerm(this.options.getTerm());
        //设置所在集群组的Id
        rb.setGroupId(this.options.getGroupId());
        //设置领导者自己的节点信息
        rb.setServerId(this.options.getServerId().toString());
        //设置目标节点的节点信息
        rb.setPeerId(this.options.getPeerId().toString());
        try {
            //判断stopAfterFinish的值，如果为fasle，那就意味着在发送请求之后该操作还不能立即结束
            if (!stopAfterFinish) {
                //其实就是得到发送请求后返回的Future，以便于在程序出现错误时，可以直接取消该操作
                this.timeoutNowInFly = timeoutNow(rb, false, timeoutMs);
            } else {
                //直接发送请求的操作
                timeoutNow(rb, true, timeoutMs);
            }
        } finally {
            if (unlockId) {
                unlockId();
            }
        }
    }
    

    //省略该类其他内容
}
```

从上面代码块中可以看到，我实现的 Replicator 复制器对象的 sendTimeoutNowAndStop() 方法非常简单，其实就是向目标节点发送一个 TimeoutNowRequest 请求，其他的什么也没做。所以，接下来我们就要一起来看看目标节点是怎么处理领导者发送过来的 TimeoutNowRequest 请求的。这才书我们要关注的重点，而这个操作我也实现了，我在 NodeImpl 类中定义了一个新的方法，就是 handleTimeoutNowRequest() 方法，该方法就是用来处理领导者发送过来的 TimeoutNowRequest 请求的。具体内容如下，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    
    //省略该类其他内容

    //领导者停机时找到的候选者节点
    private ThreadId wakingCandidate;


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

            
            //如果当前节点的状态小于State.STATE_FOLLOWER，就说明当前节点不是领导者就是候选者，要不就是跟随者
            //这里是考虑到当前节点是领导者或者候选者，因为是领导者的话，当前节点需要销毁所有的复制器对象，清空投票箱等等，以及通知其他节点变成候选者
            //这样就可以尽快选出新的领导者了
            //如果是候选者结点的话，就需要停止选举超时定时器等等组件
            if (this.state.compareTo(State.STATE_FOLLOWER) <= 0) {
                //执行身份降级操作，注意，这里有一个很重要的操作，那就是方法中的第二个参数this.state == State.STATE_LEADER
                //这就意味着如果当前节点是领导者，也就意味着领导者正在优雅停机，那这里就传进去true，这个true应该怎么使用，请大家进入stepDown()方法中查看
                   stepDown(this.currTerm, this.state == State.STATE_LEADER,
                           new Status(RaftError.ESHUTDOWN, "Raft node is going to quit."));
            }

            
            //因为要停机了，在这里更新节点状态为State.STATE_SHUTTING
            //表示正在停止工作
            this.state = State.STATE_SHUTTING;

            //停止所有定时器工作
            List<RepeatedTimer> timers = stopAllTimers();

            //以下就是执行各个组件shutdown()方法的操作，注意，这里只是执行了各个组件的
            //shutdown()方法，该方法一旦执行，并不意味着各个组件立刻被关闭了，实际上该方法的
            //执行，会让各个组件尽快处理内部尚未处理完毕的数据


            //省略部分内容
            
            //关闭候选者节点的复制器
            if (this.wakingCandidate != null) {
                Replicator.stop(this.wakingCandidate);
            }

            
            //关闭全局定时任务调度器
            timerManager.shutdown();

             //销毁所有定时器
            destroyAllTimers(timers);
        
            //等待以上组件都处理完待处理的数据
            join();
            
        }
       
    }




     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/11/28
     * @方法描述：当前节点处理即将下机的领导者发送过来的TimeoutNowRequest请求的方法，如果当前节点接收到了TimeoutNowRequest请求
     * 就意味着当前节点有可能成为候选者
     */
    @Override
    public Message handleTimeoutNowRequest(final RpcRequests.TimeoutNowRequest request, final RpcRequestClosure done) {
        boolean doUnlock = true;
        this.writeLock.lock();
        try {

            
            //判断请求中发送过来的任期是否和当前节点自己的任期不相等
            if (request.getTerm() != this.currTerm) {
                final long savedCurrTerm = this.currTerm;
                //如果当前节点任期比即将下机的领导者任期小，那么当前节点就不能成为候选者节点
                //因为正常情况下，当前节点应该和领导者节点的任期一致，这里应该是一个兜底判断
                if (request.getTerm() > this.currTerm) {
                    stepDown(request.getTerm(), false, new Status(RaftError.EHIGHERTERMREQUEST,
                            "Raft node receives higher term request"));
                }
                //记录操作日志
                LOG.info("Node {} received TimeoutNowRequest from {} while currTerm={} didn't match requestTerm={}.",
                        getNodeId(), request.getPeerId(), savedCurrTerm, request.getTerm());
                //构建并返回失败响应
                return RpcRequests.TimeoutNowResponse.newBuilder()
                        .setTerm(this.currTerm)
                        .setSuccess(false)
                        .build();
            }


            
            //程序执行到这里意味着节点任期没有问题，接下来就可以判断当前节点是不是跟随者节点
            if (this.state != State.STATE_FOLLOWER) {
                //如果是跟随者节点才可以继续向下执行，如果不是则记录操作日志，构建失败响应
                LOG.info("Node {} received TimeoutNowRequest from {}, while state={}, term={}.", getNodeId(),
                        request.getServerId(), this.state, this.currTerm);
                //构建并返回失败响应
                return RpcRequests.TimeoutNowResponse.newBuilder()
                        .setTerm(this.currTerm)
                        .setSuccess(false)
                        .build();
            }

            
            //执行到这里意味着任期没问题，当前节点也是跟随者节点
            final long savedTerm = this.currTerm;
            //那就可以构建成功响应
            final RpcRequests.TimeoutNowResponse resp = RpcRequests.TimeoutNowResponse.newBuilder()
                    //注意，这里回复给领导者节点的任期增加了1
                    .setTerm(this.currTerm + 1)
                    .setSuccess(true)
                    .build();
            //在这里把响应回复给即将下机的领导者
            done.sendResponse(resp);
            doUnlock = false;
            //当前节点进入领导者选举操作，在该方法中，会把当前节点的状态更新为候选者
            electSelf();
            LOG.info("Node {} received TimeoutNowRequest from {}, term={}.", getNodeId(), request.getServerId(), savedTerm);
        } finally {
            if (doUnlock) {
                this.writeLock.unlock();
            }
        }//上面执行的done.sendResponse(resp);这行代码已经把响应返回给领导者了
        //所以这里直接返回null即可
        return null;
    }


    

    //省略该类其他内容
}
```

从上面代码块中可以看到，**目标节点在处理领导者发送过来的 TimeoutNowRequest 请求时，会先做一系列判断，如果节点任期和节点状态都没有问题，就意味着当前节点确实可以成为候选者节点，那么当前节点就会调用 electSelf() 方法，主动进入领导者选举阶段**。到此为止，我在本章开头展示的三个问题中的第一个问题也顺利解决了。最后，就剩下第三个问题没有解决了。

不过第三个问题我就不在文章中为大家讲解了，通过前面的内容大家也可以看出来，虽然本篇文章篇幅比较长，但说实话，很多内容都是对代码的展示，我几乎没有怎么为大家分析功能的实现思路，只是把相关代码展示一下就算把功能实现了。这倒不是因为本篇文章是 sofajraft 框架的最后一票文章，所以我想偷懒，而是剩下的这点内容真的太简单了，就算我想努力为大家分析，也实在是没什么内容值得分析了，所以我就采取了比较省事的策略来完成这篇文章了。那回到剩下的第三个问题，RocksDBLogStorage 日志存储器该怎么优雅关闭，也就是 RocksDB 数据库的资源该如何释放，大家只需要从 NodeImpl 类的 afterShutdown() 方法开始看起即可，同时也可以看看 RocksDBLogStorage 日志存储器的 shutdown() 方法都执行了什么操作。我在提供的第 12 版本代码中还保留了部分源码的英文注释，大家可以结合英文注释看一看。

到此为止本篇内容就全部结束了，当然，如果要说结束，还要再把一个问题解决了，那就是 jraft 集群节点的 RpcServer 服务端是怎么关闭的呢？现在我可以为大家展示一个测试类，给大家看看在源码中要对集群中的某个节点执行优雅停机操作，真正是怎么做的，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/25
 * @Description:测试类
 */
public class ServerTest {

    //我把RaftGroupService类的内容也展示在当前代码块中了
    //大家可以看一看
    private RaftGroupService raftGroupService;

    public ServerTest(final String dataPath, final String groupId, final PeerId serverId,
                      final NodeOptions nodeOptions) throws IOException {

        //根据用户配置的本地路径创建文件夹
        FileUtils.forceMkdir(new File(dataPath));
        //创建RPC服务器
        final RpcServer rpcServer = RaftRpcServerFactory.createRaftRpcServer(serverId.getEndpoint());
        //设置日志存储文件的本地路径
        nodeOptions.setLogUri(dataPath + File.separator + "log");
        //设置元数据存储文件的本地路径
        nodeOptions.setRaftMetaUri(dataPath + File.separator + "raft_meta");
        //设置用户自己定义的状态机对象
        nodeOptions.setFsm(new MapStateMachine());
        //设置快照路径
        nodeOptions.setSnapshotUri(dataPath + File.separator + "snapshot");
        //创建集群服务对象并赋值
        this.raftGroupService = new RaftGroupService(groupId, serverId, nodeOptions, rpcServer);

    }


    public static void main(String[] args) throws InterruptedException, IOException {

        if (args.length != 4) {
            //如果用户没有传入正确的参数，或者传入的参数不足，就在控制台输出需要的参数形式
            System.out.println("Usage : java com.alipay.sofa.jraft.example.counter.CounterServer {dataPath} {groupId} {serverId} {initConf}");
            System.out.println("Example: java com.alipay.sofa.jraft.example.counter.CounterServer /tmp/server1 counter 127.0.0.1:8081 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083");
            //然后退出程序
            System.exit(1);
        }
        //解析命令行中传进来的字符串
        //解析出文件存放的本地路径
        final String dataPath = args[0];
        //要启动的集群服务的名字
        final String groupId = args[1];
        //当前节点的IP地址和端口号
        final String serverIdStr = args[2];
        //集群中所有节点的IP地址和端口号
        final String initConfStr = args[3];

        //创建的这个对象封装着当前节点需要的配置参数
        final NodeOptions nodeOptions = new NodeOptions();
        //设置超时选举时间，超过这个时间没有接收到领导者发送的信息，就会触发新一轮选举
        nodeOptions.setElectionTimeoutMs(1000);
        //不禁用集群客户端
        nodeOptions.setDisableCli(false);
        //创建的PeerId对象是用来封装代表当前服务器节点的信息的
        final PeerId serverId = new PeerId();
        //把当前节点的信息，比如IP地址，端口号等等都解析道PeerId对象中
        if (!serverId.parse(serverIdStr)) {
            throw new IllegalArgumentException("Fail to parse serverId:" + serverIdStr);
        }
        //创建集群配置类对象，这个对象中封装着整个集群中的节点信息
        final Configuration initConf = new Configuration();
        //把上面得到的字符串信息解析到该对象中，这时候配置类对象就拥有了集群中所有节点的信息
        if (!initConf.parse(initConfStr)) {
            throw new IllegalArgumentException("Fail to parse initConf:" + initConfStr);
        }
        nodeOptions.setInitialConf(initConf);
        
        
        
        //启动当前节点以及集群服务
        final ServerTest counterServer = new ServerTest(dataPath, groupId, serverId, nodeOptions);
        Node node = counterServer.raftGroupService.start();

        //产生一条日志，测试落盘效果,这是集群中第一条日志，所以索引为1
        if (STATE_LEADER == node.getNodeState()){
            //日志内容为hello
            for (int i = 0; i <10 ; i++) {
                final ByteBuffer data = ByteBuffer.wrap(("hello").getBytes());
                CountDownLatch latch = new CountDownLatch(10);
                final Task task = new Task(data, new ExpectClosure(latch));
                node.apply(task);
            }
            System.out.println("开始向硬盘存储日志！！！！！");
        }

        //在这里让当前节点下机
        counterServer.raftGroupService.shutdown();
        counterServer.raftGroupService.join();
    
    
    }

}




//下面是RaftGroupService类的内容
package com.alipay.sofa.jraft;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/25
 * @Description:框架的集群服务类，这个类中的start方法，就是启动集群的入口方法
 */
public class RaftGroupService {


    //节点与节点之间进行通信的RPC服务端
    private RpcServer rpcServer;

    //省略该类部分内容

    //调用这个方法之后，raft集群的当前节点就要停机了
    public synchronized void shutdown() {
        if (!this.started) {
            return;
        }
        if (this.rpcServer != null) {
            try {
                if (!this.sharedRpcServer) {
                    //在这里停止了节点服务端工作
                    this.rpcServer.shutdown();
                }
            } catch (final Exception ignored) {
                // ignore
            }
            this.rpcServer = null;
        }
        this.node.shutdown();
        NodeManager.getInstance().removeAddress(this.serverId.getEndpoint());
        this.started = false;
        LOG.info("Stop the RaftGroupService successfully.");
    }


    public synchronized void join() throws InterruptedException {
        if (this.node != null) {
            this.node.join();
            this.node = null;
        }
    }


    //省略该类部分内容
}
```

朋友们，到此为止 sofajraft 框架的内容就全部结束了，如果有朋友一篇接一篇看到这里的话，肯定会觉得 sofajraft 这个框架本身难度并不大，只不过内部使用了太多的回调方法，而且在日志传输过程中做了太多谨慎的判断，以至于这个框架学起来有些漫长和枯燥。我想跟大家说的是，不管我使用多么通俗的语言来讲述框架中的各种知识，不管我如何想使文章读起来比较有趣，这对降低知识的难度并没有起到什么帮助，无非就是让大家阅读文章的时候感到轻松一下。不管怎么说，知识本身的复杂程度就摆在那里，如果你认真学进去，你一定会觉得好麻烦啊，这里该怎么理解啊，一点也不好学啊，对吧，大家肯定会有这样的牢骚。所以朋友们，如果你真的看到这里了，并且也把前面的知识都掌握了，那么你真的应该为你自己感到骄傲，我也衷心祝贺你，我很清楚这些知识的分量，我也知道你要掌握这些知识会付出多少努力，如果你能一直保持这种状态，保持这种耐性，我相信你会在你的职业生涯中走得更顺利、更远！朋友们，愿我们能够在 nacos 再相见！