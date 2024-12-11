大家好，在日志落盘功能实现之后，这一章我将为我们的程序实现领导者向跟随者传输日志功能。本章日志传输功能的实现逻辑非常简单，当然，后面还会对该功能进行重构。当然，实现的逻辑非常简单并不意味着代码编写也会很简单，实际上，在代码中也有很多细节，并且日志传输功能的实现也离不开池化技术的运用。好了，话不多说，让我们尽快开始这一章内容的学习吧。

## 剖析 Replicator 发送探针请求全流程

课程进行到这里，不知道大家对 Replicator 复制器对象还有没有印象。我希望大家别觉得我啰嗦，我相信可能很多朋友对之前讲过的知识、实现的功能肯定记忆犹新，觉得我这么问大家，或者总部在新的章节中回顾旧知识有点啰嗦，甚至怀疑我有水字数的嫌疑🐶。**天地良心！我真的没有在水字数，如果我水字数，就让我去红浪漫当一辈子保安！！！**我之所以在新的章节中习惯性地帮助大家回顾旧知识，原因有两点：

1 就我目前的状态来说，也许真的是记忆里下降了，也可能是代码比较复杂，我对我研究过的框架，在过了一个月之后，只能记得框架的设计思路，以及包含了哪些组件，核心功能是怎么实现的，运用了哪些技术实现的。至于代码的具体细节，我也忘得差不多了。我想在诸位身上，这种情况也不是不可能出现，而 Replicator 复制器对象是我在过年之前为大家讲解的知识，所以我认为我有必要帮助大家重新回顾一下。

2 第二个原因就很简单了，本章要实现的日志传输就是 Replicator 复制器对象中的功能，所以我在这里先为大家回复一下 Replicator 复制器对象的知识，然后引入本章要实现的功能。

好了，说起 Replicator 复制器对象，我相信大家也都还记得，**这个对象只有在领导者节点才会被创建，当 raft 集群中的一个节点成为领导者之后，在领导者内部，就会根据集群配置信息，将集群中的其他节点都包装成一个个 Replicator 复制器对象，也就意味着每一个 Replicator 复制器对象对应着集群中的一个跟随者节点。领导者就通过每一个 Replicator 复制器对象向集群中的各个跟随者节点发送心跳消息，以及进行日志传输**。

领导者向集群中各个跟随者节点发送心跳消息的流程非常简单，那就是当领导者根据集群配置信息，创建了一个个 Replicator 复制器对象之后，就会立刻通过每一个 Replicator 复制器对象，向对应的跟随者节点们发送心跳消息。这里我把之前的发送心跳消息的代码简单搬运了一部分到这里，帮助大家回顾一下，请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容



    //该类的核心方法，在该方法中创建了复制器对象
    public static ThreadId start(final ReplicatorOptions opts, final RaftOptions raftOptions) {
        //对相关组件进行判空操作，看看日志管理器，投票箱，当前节点是否为空
        if (opts.getLogManager() == null || opts.getNode() == null) {
            throw new IllegalArgumentException("Invalid ReplicatorOptions.");
        }
        //创建复制器对象，这时候这两个配置参数对象就派上用场了
        final Replicator r = new Replicator(opts, raftOptions);
        //在这里远程连接一下这个复制器对象对应的节点，连接失败返回null
        if (!r.rpcService.connect(opts.getPeerId().getEndpoint())) {
            LOG.error("Fail to init sending channel to {}.", opts.getPeerId());
            return null;
        }
        //创建ThreadId句柄对象，这个对象持有了刚才创建的复制器对象
        r.id = new ThreadId(r, r);
        //这里的代码很有意思，直接解释的话就是给当前复制器对象上了一个锁
        //上锁期间，其他任何线程都不能操纵当前的复制器对象，准确的说不是不能操作当前的复制器对象
        //而是不能对当前的复制器对象中的属性进行修改，实际上，为每一个复制器对象创建的ThreadId对象
        //就相当于当前复制器对象的可重入锁对象，因为在复制器对象工作的时候，还会有其他线程对复制器对象进行操作
        //比如NodeImpl类中就有一些方法要被其他线程调用，然后修改复制器对象中的属性，这时候就要加锁，防止
        //并发问题出现
        r.id.lock();
        //记录一条日志
        LOG.info("Replicator={}@{} is started", r.id, r.options.getPeerId());
        //记录最后一次发送信息的时间
        r.lastRpcSendTimestamp = Utils.monotonicMs();
        
        //启动心跳定时器，这个定时器会定时向其他节点发送心跳消息
        r.startHeartbeatTimer(Utils.nowMs());
        
        
        
        //发送探针请求，这里我想多解释几句，探针请求是领导者刚刚当选之后就要向集群中其他节点发送的，这个探针请求的作用也很简单
        //就是通知其他节点领导者已经选举出来了，其他节点会更新它们的领导者信息
        //其他节点在回复这个探针的响应时，还会把自己的日志信息告诉领导者，让领导者决定传输日志的方式
        //看是正常传输还是使用快照，如果是正常传输，应该从第几条日志开始传输
        r.sendProbeRequest();
        //返回ThreadId对象
        return r.id;
    }






    //省略其他内容
}
```

可以看到，在上面代码块的第 38 行启动了心跳定时器，也就是执行了 r.startHeartbeatTimer(Utils.nowMs()) 这行代码。这行代码的内部逻辑我就不再具体展开了，大家可以直接从代码中回顾，本章对应的代码为第四版本代码，所以，大家也可以直接去第四版本代码中回顾旧的知识。

我在这里为什么非要提到发送心跳消息呢？原因也很简单，**因为随着心跳定时器的开启，领导者紧接着还要向跟随者发送一个探针消息**。这个探针消息我在之前也跟大家简单提过一次，但并没有展开讲解，现在是时候将将它了。请大家想一想，当 raft 集群中一个节点成为了领导者，并且将其他节点都包装成了一个个 Replicator 对象，这个领导者节点接下来应该做什么？毫无疑问，应该立刻开始向各个跟随者节点传输日志了，对吧？当然，假如整个 raft 集群是第一次启动，启动之后客户端也没有向领导者发送指令，领导者内部就不会创建 LogEntry 日志条目，这个时候领导者内部是没有日志可以向跟随者节点传输的。这时候应该怎么做呢？也很简单，**既然没有日志传输，那么就让领导者向跟随者节点发送日志的线程阻塞就行，直到领导者内部产生日志了，再唤醒这个线程，继续发送日志即可**。如果这个逻辑不是那么容易理解，请大家再思考另一个场景，假如整个 raft 集群已经启动了一段时间了，集群中的领导者突然宕机了，经过领导者选举之后，集群中产生了新的领导者，这个刚刚当选的领导者肯定也需要根据集群的配置信息，重新在节点内部创建一个个 Replicator 复制器对象，然后就要进行心跳检测和日志传输。这个场景应该很容易理解吧。因为 raft 集群已经运行了一段时间了，整个集群内部肯定有了很多日志，新当选的领导者肯定包含已经提交了的日志，而集群中有的节点日志可能还没有同步完毕，**所以，当一个 raft 集群中的节点成为领导者之后，紧接着就开始执行向各个跟随者节点传输日志的操作是很合理的，我们应该把它当成一个顺理成章的操作**。我想这个流程应该也是比较容易理解的。

但是，这个传输日志的流程和我刚才提出的探针消息又什么关系呢？请听我慢慢道来。还是请大家再想象一个场景，假如一个 raft 集群已经运行了一段时间，整个集群由 5 个节点组成，节点 1 为领导者，其他 4 个节点为跟随者。在某个时刻，节点 1，也就是领导者本地最新日志索引为 12，这也就意味着领导者内部有 12 条日志；节点 2 的最新日志索引为 9，节点 3 的最新日志索引为 8，节点 4 的最新日志索引为 6，节点 5 的最新日志索引为 5。在这种情况下，假如领导者突然宕机了，因为集群中其他跟随者节点拥有的日志都落后于节点 2，所以经过一段时间之后，集群中的节点 2 当选称为新的领导者，这个时候，节点 2 就会根据集群配置信息创建一个个 Replicator 复制器对象，然后通过每一个 Replicator 复制器对象，开始向其他跟随者节点继续传输日志。但节点 2 刚刚当选为领导者，它怎么知道每一个跟随者节点需要接收哪一条日志呢？

按照我刚才举的例子来说，节点 3 目前的最新日志索引为 8，要从领导者接收的下一条日志索引肯定为 9，节点 4 和节点 5 分别要接收的下一条日志索引为 7 和 6。但节点 2 刚刚成为领导者，肯定不知道这些信息，所以，**这个时候节点 2 就会先通过每一个 Replicator 对象，向对应的跟随者节点发送探针请求，跟随者节点接收到探针请求之后，会在响应中把自己本地最新的日志索引告诉领导者，这样一来，领导者在接收到每一个跟随者节点的探针请求的响应后，就知道接下来要给每一个跟随者节点发送索引为几的日志了**。其实从这里也能进一步看出来，领导者内部创建的这个 Replicator 对象的具体作用。**在领导者内部，每一个 Replicator 对象负责向自己对应的跟随者节点发送心跳消息，并且也会向自己对应的跟随者节点传输日志。但是集群中各个跟随者节点从领导者接收到日志的情况可能并不相同，有的节点接收到的日志多，有的节点接收到的少，这就使得领导者向各个跟随者节点传输日志的时候，需要根据各个节点要接收的下一条日志的索引来传输日志。那么领导者怎么知道各个跟随者节点需要接收的下一条日志的索引呢？就是通过探针消息获得的，并且，在每一个 Replicator 对象中都定义着一个 nextIndex 成员变量，当领导者通过探针请求的响应获得了每一个跟随者节点需要接收的下一条日志的索引，就会赋值给各个跟随者节点对应的 Replicator 复制器对象的 nextIndex 成员变量。然后，领导者每向跟随者节点发送一条日志，nextIndex 成员变量按顺序递增 1 即可**。

好了，刚才用文字分析了那么多，不管写多少，都不如直接从代码中看得直观，所以，接下来，我就为大家展示一下所谓的探针请求，在代码中究竟是怎么定义，并且是如何被发送的。请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容



    //构造方法
    public Replicator(final ReplicatorOptions replicatorOptions, final RaftOptions raftOptions) {
        super();
        this.options = replicatorOptions;
        //要发送的下一条日志的索引默认初始值为领导者最后一条日志索引加1
        //如果整个集群刚刚启动，this.options.getLogManager().getLastLogIndex()的默认值就是0
        //所以这个时候领导者向集群中其他跟随着节点要发送的下一条日志默认就是索引为1的日志
        this.nextIndex = this.options.getLogManager().getLastLogIndex() + 1;
        this.timerManager = replicatorOptions.getTimerManager();
        this.raftOptions = raftOptions;
        this.rpcService = replicatorOptions.getRaftRpcService();
    }
    

    //该类的核心方法，在该方法中创建了复制器对象
    public static ThreadId start(final ReplicatorOptions opts, final RaftOptions raftOptions) {
        //对相关组件进行判空操作，看看日志管理器，投票箱，当前节点是否为空
        if (opts.getLogManager() == null || opts.getNode() == null) {
            throw new IllegalArgumentException("Invalid ReplicatorOptions.");
        }
        //创建复制器对象，这时候这两个配置参数对象就派上用场了
        final Replicator r = new Replicator(opts, raftOptions);
        //在这里远程连接一下这个复制器对象对应的节点，连接失败返回null
        if (!r.rpcService.connect(opts.getPeerId().getEndpoint())) {
            LOG.error("Fail to init sending channel to {}.", opts.getPeerId());
            return null;
        }
        //创建ThreadId句柄对象，这个对象持有了刚才创建的复制器对象
        r.id = new ThreadId(r, r);
        //这里的代码很有意思，直接解释的话就是给当前复制器对象上了一个锁
        //上锁期间，其他任何线程都不能操纵当前的复制器对象，准确的说不是不能操作当前的复制器对象
        //而是不能对当前的复制器对象中的属性进行修改，实际上，为每一个复制器对象创建的ThreadId对象
        //就相当于当前复制器对象的可重入锁对象，因为在复制器对象工作的时候，还会有其他线程对复制器对象进行操作
        //比如NodeImpl类中就有一些方法要被其他线程调用，然后修改复制器对象中的属性，这时候就要加锁，防止
        //并发问题出现
        r.id.lock();
        //记录一条日志
        LOG.info("Replicator={}@{} is started", r.id, r.options.getPeerId());
        //记录最后一次发送信息的时间
        r.lastRpcSendTimestamp = Utils.monotonicMs();
        
        //启动心跳定时器，这个定时器会定时向其他节点发送心跳消息
        r.startHeartbeatTimer(Utils.nowMs());
        
        
        
        //发送探针请求，这里我想多解释几句，探针请求是领导者刚刚当选之后就要向集群中其他节点发送的，这个探针请求的作用也很简单
        //就是通知其他节点领导者已经选举出来了，其他节点会更新它们的领导者信息
        //其他节点在回复这个探针的响应时，还会把自己的日志信息告诉领导者，让领导者决定传输日志的方式
        //看是正常传输还是使用快照，如果是正常传输，应该从第几条日志开始传输
        r.sendProbeRequest();
        
        
        //返回ThreadId对象
        return r.id;
    }



    //发送探针请求的方法
    private void sendProbeRequest() {
        //发送探针请求其实和发送心跳请求使用的是一个方法，只不过在这里传入一个false参数
        //意味着本次调用的sendEmptyEntries方法并不是发送心跳请求的方法，而是探针请求方法
        sendEmptyEntries(false);
    }


    
    private void sendEmptyEntries(final boolean isHeartbeat) {
        //这里要注意一下，下面这个方法中的第一个方法参数是false，是从上面的方法中传进来的
        sendEmptyEntries(isHeartbeat, null);
    }



    //发送心跳或者探针请求的方法，根据方法中的第一个参数判断是否发送心跳还是探针消息，第二个是收到响应后的回调方法
    private void sendEmptyEntries(final boolean isHeartbeat, final RpcResponseClosure<RpcRequests.AppendEntriesResponse> heartBeatClosure) {
        
        //开始构建AppendEntriesRequest请求，该请求是共用的，日志传输，心跳消息，探针消息都会使用这个请求封装消息内容
        final RpcRequests.AppendEntriesRequest.Builder rb = RpcRequests.AppendEntriesRequest.newBuilder();
        
        //填充请求中的公共属性，如果填充失败就执行安装快照的操作
        //fillCommonFields方法中省略了一段校验是否需要安装快照的逻辑
        if (!fillCommonFields(rb, this.nextIndex - 1, isHeartbeat)) {
            //这里暂时删掉了安装快照的操作
            if (isHeartbeat && heartBeatClosure != null) {
                RpcUtils.runClosureInThread(heartBeatClosure, new Status(RaftError.EAGAIN,
                        "Fail to send heartbeat to peer %s", this.options.getPeerId()));
            }
            return;
        }
        try {
            //当前发送请求的时间
            final long monotonicSendTimeMs = Utils.monotonicMs();
            
            //根据方法中的参数判断是否为心跳请求
            if (isHeartbeat) {
                LOG.info("LEADER发送了心跳消息！");
                //创建AppendEntriesRequest对象封装消息内容
                final RpcRequests.AppendEntriesRequest request = rb.build();
                //心跳计数器自增
                this.heartbeatCounter++;
                //定义一个回调方法，该方法会在接收到心跳响应后被回调
                RpcResponseClosure<RpcRequests.AppendEntriesResponse> heartbeatDone;
                //判断参数中的回调方法是否为空
                if (heartBeatClosure != null) {
                    heartbeatDone = heartBeatClosure;
                } else {//如果为空就创建一个新的
                    heartbeatDone = new RpcResponseClosureAdapter<RpcRequests.AppendEntriesResponse>() {

                        @Override
                        public void run(final Status status) {
                            //在创建的新的回调方法中，收到心跳响应之后，会回调这个方法，在这个方法中会提交下一个心跳消息任务给全局定时器
                            onHeartbeatReturned(Replicator.this.id, status, request, getResponse(), monotonicSendTimeMs);
                        }
                    };
                }
                //发送心跳消息
                this.heartbeatInFly = this.rpcService.appendEntries(this.options.getPeerId().getEndpoint(), request,
                        this.options.getElectionTimeoutMs() / 2, heartbeatDone);


                //上面的代码都是旧知识了，在我为大家实现心跳检测功能时，就展示这些代码了，所以上面的代码大家简单回顾一下就行，主要是看下面else分支中的代码
            } else {



                
                //走到这里意味着不是心跳消息，而是探针消息，因为是探针消息，所以该请求并不会携带日志，所以在AppendEntriesRequest请求中
                //把要传输的有效日志数据设置为空
                rb.setData(ByteString.EMPTY);
                
                //创建AppendEntriesRequest对象封装探针消息
                final RpcRequests.AppendEntriesRequest request = rb.build();
                
                //发送探针消息
                final Future<Message> rpcFuture = this.rpcService.appendEntries(this.options.getPeerId().getEndpoint(),
                        request, -1, new RpcResponseClosureAdapter<RpcRequests.AppendEntriesResponse>() {

                            @Override
                            public void run(final Status status) {
                                //onRpcReturned方法会在当前领导者节点接收到对应的跟随者节点回复的探针请求响应后被回调
                                //该方法具体该如何实现，暂时还没必要展开讲解
                                onRpcReturned(Replicator.this.id, status, request, getResponse(), monotonicSendTimeMs);
                            }
                        });

            }
            LOG.debug("Node {} send HeartbeatRequest to {} term {} lastCommittedIndex {}", this.options.getNode()
                    .getNodeId(), this.options.getPeerId(), this.options.getTerm(), rb.getCommittedIndex());
        } finally {
            //释放锁，这里释放的是sendHeartbeat(final ThreadId id)方法中获得的锁，就是这个方法
            //private static void sendHeartbeat(final ThreadId id) {
            //        //发送之前先上锁
            //        final Replicator r = (Replicator) id.lock();
            //        if (r == null) {
            //            return;
            //        }
            //        r.sendEmptyEntries(true);
            //    }
            unlockId();
        }
    }





    //填充AppendEntriesRequest请求中公共字段的方法
    private boolean fillCommonFields(final RpcRequests.AppendEntriesRequest.Builder rb, long prevLogIndex, final boolean isHeartbeat) {
        //这里要注意一种情况，这个地方很重要，如果是整个集群刚启动的情况下，领导者中还没有日志的时候
        //第一条日志的索引默认为1，前一条日志的索引肯定就是0，根据前一条日志索引获取前一条日志的任期
        //这里得到的就是0，这个要理清楚，这样在跟随者那里校验索引和日志的时候，才能校验成功
        final long prevLogTerm = this.options.getLogManager().getTerm(prevLogIndex);
        //该方法在这里省略了一段校验是否需要安装快照的逻辑，在代码的第7版本将会为大家补全
        rb.setTerm(this.options.getTerm());
        //当前领导者节点所属的集群Id
        rb.setGroupId(this.options.getGroupId());
        //当前领导者节点的节点信息
        rb.setServerId(this.options.getServerId().toString());
        //要把消息发送给的目标节点的节点信息
        rb.setPeerId(this.options.getPeerId().toString());
        //上一条发送给跟随者节点的日志的索引
        rb.setPrevLogIndex(prevLogIndex);
        //上一条发送给跟随者节点日志的任期
        rb.setPrevLogTerm(prevLogTerm);
        //rb.setCommittedIndex(this.options.getBallotBox().getLastCommittedIndex());
        //上面被注释掉的那行代码才是真正的源码，在第四版本代码中，我还没有为大家引入投票箱
        //所以在发送领导者应用的最新日志索引时，先默认发送0
        rb.setCommittedIndex(0);
        return true;
    }


    //省略其他内容
}
```

可以看到，在上面代码块的第 60 行，程序执行了 r.sendProbeRequest() 这行代码，sendProbeRequest 就是发送探针请求的意思。当然，大家很快就能意识到在 sendProbeRequest 方法内部，**实际上调用了发送心跳请求的 sendEmptyEntries 方法，这也就意味着实际上发送心跳请求和探针请求使用的是同一个方法**。并且在上面的代码块中，大家也能看到，**探针请求和心跳请求使用的实际上是同一个请求体，那就是 AppendEntriesRequest 对象**。在进入到真正发送请求的 sendEmptyEntries 方法之后，也就是上面代码块的第 86 行，在进入到该方法之后，**首先创建了一个 AppendEntriesRequest 请求构建器，然后就执行了 fillCommonFields 方法，fillCommonFields 方法的作用是向 AppendEntriesRequest 请求对象中填充一些公共的属性，所谓公共属性，就是心跳请求和探针请求都需要封装的属性，比如领导者当前的任期、领导者发送给跟随者节点的前一条日志的索引和任期、领导者最新应用的日志的索引等等。**将这些属性封装在 AppendEntriesRequest 中的原因很简单，**因为 raft 共识算法是个强一致性算法，这个算法牺牲了集群的可用性，保证了集群的数据一致性和有限度地容忍网络分区的特性。集群的一致性可以说就是靠领导者严格按照索引顺序创建日志条目，然后按照日志索引顺序把日志条目传输给各个跟随者节点保证的，而网络环境又是波动的，所以领导者需要在跟跟随者节点每一次通信的时候，都把领导者的关键信息发送给跟随者节点，这样跟随者节点就可以时刻校验，看看集群中的领导者是否发生了变化**。

当探针请求发送出去的时候，还是在上面代码块中，在第 147 行，为这个探针请求创建了一个回调对象，也就是 RpcResponseClosureAdapter 对象，该对象内部的 run 方法会在领导者接收到探针响应后被回调(具体回调的逻辑我放在视频中讲解了)，当然，实际上最终回调的是 run 方法内部调用 onRpcReturned 方法。领导者就是在 onRpcReturned 方法中，处理对应的跟随者节点回复的探针响应，在这个响应中，领导者就可以知道接下来要发送给跟随者节点的日志的索引究竟是多少了。到这里，我实际上就为大家把探针请求的发送功能实现完毕了。我可以想到到此为止有多少朋友会感到疑惑，搞什么啊，什么探针请求，实际上跟心跳请求没什么区别啊，**在真正发送请求的 sendEmptyEntries 方法中，最终构建的心跳请求和探针请求不都一样吗？通过 fillCommonFields 方法填充了请求的公共属性，并且无论是心跳请求还是探针请求，都没有日志的有效数据，这两个请求的内容可以说是一模一样，只不过最后处理它们响应的方法不同，为什么要弄两个一摸一样的请求，然后故意将其中一个称为心跳请求，将另一个称为探针请求呢**？接下来我为大家讲解一下探针请求和心跳请求的作用。

**1 心跳请求和探针请求最大的不同就是，心跳请求是定期发送的，而探针请求基本上是一次性的，领导者根据探针请求知道了要发送给跟随者的下一条日志的索引之后，没有意外情况，就不会再发送探针请求了。**

**2 心跳请求一直在承担跟随者节点和领导者节点沟通的作用，如果心跳请求一旦停止，跟随者节点就有可能触发超时选举，选出新的领导者。而探针请求的主要作用其实就是让领导者定位要发送给跟随者节点的下一条日志的索引。当一个节点成功当选领导者之后，领导者需要立刻向所有跟随者节点发送探针请求，才能知道自己要给各个跟随者节点发送的下一条日志的索引。所以说，探针请求主要是用来给领导者做要发送的日志定位的，定位成功之后，就不需要再发送探针请求了。**

以上两点就是探针请求和心跳请求的区别。当然，讲解到这里，肯定还是有很多朋友感到困惑，对这个探针请求的发送讲解了这么多内容，但是从代码上看，这个请求的发送很简单，那么，发送之后的具体流程呢？也就是说，**发送之后，是怎么根据探针请求的响应来给领导者要发送给跟随着的下一条日志做定位的呢**？诸位别急，接下来，我就要为大家实现之后的功能和流程。

## 重构 NodeImpl 类的 handleAppendEntriesRequest 方法

在上一小节，我为大家实现了 Replicator 复制器对象发送探针请求的功能，通过这个功能，大家也知道了心跳请求和探针请求实际上使用的是同一个请求对象，那就是 AppendEntriesRequest 对象。当这个请求对象从领导者发送出去之后，就会到达跟随者节点，那么这个请求会被跟随者节点的哪个方法接收并处理呢？这就要用到前几章的知识了。在前几章我为大家实现心跳检测功能的时候，当领导者发送了一个心跳请求，这个请求最后是被跟随者节点的 NodeImpl 对象的 handleAppendEntriesRequest 方法接收并处理了。现在我要为大家实现探针请求的后续流程，而探针请求实际上也是一个 AppendEntriesRequest 对象，所以，**我们仍然可以使用 NodeImpl 对象的 handleAppendEntriesRequest 方法来处理领导者发送过来的探针请求**。那么 handleAppendEntriesRequest 方法要怎么处理这个探针请求呢？其实也很简答，从流程上来说，和处理心跳请求也没什么不同。因为探针请求和心跳请求都封装着相同的公共属性，这些属性都会在 handleAppendEntriesRequest 方法中被用到。

**比如说 AppendEntriesRequest 请求中封装着领导者节点本身的 serverId 信息，在跟随者节点的 handleAppendEntriesRequest 方法中，就要从探针请求中得到领导者的 serverId 信息，然后试着解析这个信息，如果解析失败，则直接返回探针请求的失败响应**；

**探针请求中还封装着领导者的任期，跟随者节点也会将自己本地的任期和请求中的任期做对比，正常情况下，跟随者和领导者的任期应该是相等的，如果领导者的任期小于跟随者的任期，那就意味着集群中曾经出现过网络分区，发送探针请求的领导者是旧的领导者，现在集群中有了新的领导者，并且任期比旧领导者大，这时候在探针响应中就会把最新的任期值告诉旧的领导者，让旧的领导者执行身份降级操作，成为跟随者节点**。**如果探针请求中的任期比接收请求的节点的任期大，那么接收探针请求的节点就要让自己执行身份降级操作，让自己成为跟随着节点**。

除此之外，探针请求中还封装着领导者向当前跟随者节点发送的前一条日志的索引和任期，这个是非常重要的信息，也正是探针消息用来实行日志定位的关键属性。注意，**这里的前一条日志指的是领导者最后发送给跟随者节点的日志，比如领导者要发送给跟随者节点的下一条日志索引为 5，那么前一条发送的日志索引就为 4。大家要理解这个意思**。那么，当跟随者节点从探针请求中获得了这个信息之后，应该执行什么操作呢？很简单，**跟随者只需要从本地获得自己最新日志的索引以及对应的任期，然后和请求中的日志索引和任期做对比即可**。这么说可能有点抽象，所以接下来，让我用一个例子为大家解释说明。

假如跟随者最新从领导者接收到的日志索引为 5，任期为 3，并且跟随者和领导者的任期都为 3。那么跟随者节点本地最新日志的索引就为 5，任期为 3。领导者向跟随者发送探针消息的时候，会把最后一次发送给跟随者节点的日志信息封装在请求中发送给跟随者节点，也就是会把日志索引为 5，任期为 3 的信息发送给跟随者。当跟随者接收到请求之后，发现本地最新日志的索引也是 5，任期也是 3，这就意味着跟随者拥有的日志和领导者拥有的日志是同步的，中间没有"缝隙"，日志是连贯的，没有出现缺少日志的情况。这时候，跟随者节点就可以直接回复给领导者一个成功响应。

如果跟随者节点发现本地最新日志的信息和探针请求中接收到的日志信息并不相等，比如说，探针请求中的日志索引为 5，任期为 3，但是跟随者节点本地最新的日志索引才为 3，任期为 3，这种情况就意味着领导者在向跟随者节点传输索引为 4 和 5 的日志时，传输失败了，需要重新传输。这时候，跟随者节点就会给领导者返回一个失败响应，并在响应中把自己的日志索引发送给领导者。当领导者接收到失败响应后，就知道应该从索引为 3 的日志重新开始传输给跟随者节点。

当然，**跟随者节点最新的日志索引也可能比探针请求中的大**，因为集群中可能还会出现这样一种情况：整个 raft 集群一共有 5 个节点，节点 1 为领导者。各个节点拥有的日志情况如下。

```
领导者  节点1  最新日志索引为3    日志任期为2    节点任期为2
跟随者  节点2  最新日志索引为2	日志任期为2	  节点任期为2
跟随者  节点3  最新日志索引为2	日志任期为2	  节点任期为2
跟随者  节点4  最新日志索引为2 	日志任期为2	  节点任期为2
跟随者  节点5  最新日志索引为5 	日志任期为1	  节点任期为2

//这里节点5的日志情况比较特殊，虽然节点5有5条日志，比其他节点的日志都多，但是这个节点的日志任期为1
//这就意味着节点5拥有的日志是旧的，节点5之前可能是领导者，内部产生了5条日志，但这5条日志还没来得及
//传输给其他4个跟随者节点，节点5就宕机或者发生网络分区了，等节点5恢复之后，集群中已经有了新的领导者
//那就是节点1，所以节点5只好以跟随者的身份重新加入集群
```

上面代码块中展示的就是整个集群目前的日志存储情况，也就是说，在上面的 raft 集群中，领导者内部已经存在 3 条日志了，日志的任期都为 2。领导者只把前两条日志复制给了集群中的2、3、4 节点，还没有来得及把索引为 3 的日志传输给 2、3、4 这三个个跟随者节点，并且节点 5 也没能从领导者那里复制到任何一条日志，领导者就宕机了。这种情况下，只有索引为 2，任期为 2 之前的日志是已经提交了的，所以触发超时选举之后，理论上来说，节点 2、3、4 都有机会成为新的领导者。最终节点 2 任期自增，经过选举后成为了新的领导者，集群中各个节点的任期也都变成了 2。于是整个集群中各个节点持有日志的情况就变成了下面这样。请看下面代码块。

```
宕机了  节点1  最新日志索引为3    日志任期为2    节点任期为2
领导者  节点2  最新日志索引为2	日志任期为2	  节点任期为3
跟随者  节点3  最新日志索引为2	日志任期为2	  节点任期为3
跟随者  节点4  最新日志索引为2 	日志任期为2	  节点任期为3
跟随者  节点5  最新日志索引为5 	日志任期为1	  节点任期为3
```

当节点 2 成为了新的领导者之后，会为集群中的每一个跟随者节点都创建 Replicator 复制器对象，在创建复制器对象的时候，会调用 Replicator 复制器对象的构造方法，就是下面代码块中展示的方法。

```
//构造方法
public Replicator(final ReplicatorOptions replicatorOptions, final RaftOptions raftOptions) {
    super();
    this.options = replicatorOptions;
    //要发送的下一条日志的索引默认初始值为领导者最后一条日志索引加1
    this.nextIndex = this.options.getLogManager().getLastLogIndex() + 1;
    this.timerManager = replicatorOptions.getTimerManager();
    this.raftOptions = raftOptions;
    this.rpcService = replicatorOptions.getRaftRpcService();
}
```

可以看到，在 Replicator 复制器对象的构造方法中会初始化 Replicator 对象的 nextIndex 成员变量，也就是领导者要发送给对应跟随着节点的下一条日志的索引。节点 2 拥有的最新日志的索引为 2，所以要发送给跟随者节点的下一条日志的索引就是 3。当然，大家也别忘记了，在 raft 集群中，当一个节点成为领导者之后，会立刻通过每一个 Replicator 复制器对象向对应的跟随者节点发送探针请求，因为探针请求会封装领导者发送给跟随者节点的前一条日志的索引和任期，而这个索引实际上就是 nextIndex -1，所以节点 2 会通过探针请求把 prevLogTerm = 2 和 prevLogIndex = 2 发送给跟随者节点。集群中的跟随者节点 3、4、5 都会接收到来自节点 2 的探针请求。节点 3 和节点 4 都好说，因为这两个节点本地最新日志的索引和任期都为 2，和探针请求中的日志索引、任期相同，所以会直接返回给领导者一个成功响应。领导者在接收到成功响应后就知道日志定位成功了，如果内部产生了新的日志，下一条要发送的日志就从索引 3 开始发送。但是节点 5 却是另一种情况。因为节点 5 本地最新日志的索引为 5，但任期为 1，而领导者发送过来的日志索引为 2，任期也为 2。这种情况实际上就意味着跟随者和领导者的日志产生了冲突，这时候就有一个很重要的操作是需要领导者执行。

当跟随者节点的日志索引比领导者的还要大时，这时候跟随者节点就要回复领导者一个失败的探针响应，并且在响应中告诉领导者自己最新的日志索引。 就比如说节点 2 收到响应之后，知道跟随者最新的日志索引为 5，但是自己最多才拥有 2 条日志。刚才把日志索引 2 发送给节点 5，结果收到失败响应，接下来，领导者就需要将发送给跟随者节点的 prevLogIndex 减 1，然后再通过探针请求发送给跟随者节点。跟随者节点再次接收到探针请求中的 prevLogIndex 就为 1，prevLogTerm 仍然为 2。当然，跟随者节点使用 prevLogIndex 查询本地日志，会发现 prevLogIndex 对应的任期为 1，所以还是无法和领导者发送过来的探针请求中的日志信息匹配成功，于是又会回复一个失败响应。领导者在接收到失败响应后，就会把 prevLogIndex 为 0，prevLogTerm 为 0 通过探针请求发送给跟随者节点。这里领导者发送的 prevLogTerm 忽然变成 0 了，也许会有朋友不太理解，我简单解释一下，因为发送探针请求需要调用 fillCommonFields 方法填充请求对象的属性，在 fillCommonFields 方法内部会执行 this.options.getLogManager().getTerm(prevLogIndex) 代码得到要发送给跟随者节点的 prevLogTerm。prevLogIndex 这时候变成了 0，当 prevLogIndex 为 0 时，this.options.getLogManager().getTerm(prevLogIndex) 返回的任期就为 0，所以领导者发送给跟随者的 prevLogTerm 也就变成了 0。

这一次，跟随者节点从探针请求中得到了 prevLogIndex 为 0，prevLogTerm 为 0 的信息后，会发现自己本地日志索引为 0 的时候，对应的日志任期也为 0。因为跟随者节点也是通过 this.logManager.getTerm(prevLogIndex) 来得到该索引日志对应的任期的，如果日志索引为 0，那么返回的日志任期就为 0。这一次，跟随者节点终于和领导者发送过来的探针请求中的信息匹配成功了，所以会回复一个成功响应。领导者在接收到响应之后就会知道，源码节点 5 拥有的所有日志都是旧的，需要从头开始给节点 5 传输日志，于是就会从索引为 1 的日志开始传输给节点 5。

到这里其实也可以为大家总结一下了，**如果领导者发送了探针请求之后，发现跟随者拥有的最新日志的索引比自己还要大，这时候就要让领导者不断递减自己要发送的 prevLogIndex，直到发送的日志信息和跟随者本地的日志匹配成功，然后就以匹配成功的日志索引为起点，开始传输之后的日志给跟随者节点。至于匹配成功的条件，就是让跟随者节点使用 prevLogIndex 去查找自己本地对应日志的任期，如果本地日志任期和领导者发送过来的日志任期相同，就意味着日志匹配成功了**。

至于探针请求中封装的领导者最新应用的日志的索引，这个就先不讲解了，这个和状态机有关。关于状态机的知识，我会在第七版本代码对应的文章中再为大家讲解。好了，又分析了一大堆，接下来，就让我们真正从代码层面重构一下 handleAppendEntriesRequest 方法。请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {



    //省略部分内容




    //当前节点接收到领导者发送过来的消息时，不管消息是日志复制还是心跳消息，还是探针消息，都会调用该方法进行处理
    //换句话说，其实领导者要发送的日志复制消息，以及心跳消息探针消息，使用的都是同一个RPC请求，也就是该方法中的第一个参数对象
    @Override
    public Message handleAppendEntriesRequest(final RpcRequests.AppendEntriesRequest request, final RpcRequestClosure done) {
        //这些重复逻辑就不添加注释了
        boolean doUnlock = true;
        this.writeLock.lock();
        
        //从请求中获得领导者传送过来的日志的条数
        final int entriesCount = request.getEntriesCount();
        
        try {//判断当前节点是否活跃
            if (!this.state.isActive()) {
                LOG.warn("Node {} is not in active state, currTerm={}.", getNodeId(), this.currTerm);
                return RpcFactoryHelper
                        .responseFactory()
                        .newResponse(RpcRequests.AppendEntriesResponse.getDefaultInstance(), RaftError.EINVAL,
                                "Node %s is not in active state, state %s.", getNodeId(), this.state.name());
            }
            final PeerId serverId = new PeerId();
            
            //解析领导者节点信息
            if (!serverId.parse(request.getServerId())) {
                LOG.warn("Node {} received AppendEntriesRequest from {} serverId bad format.", getNodeId(),
                        request.getServerId());
                //解析失败直接返回失败响应
                return RpcFactoryHelper
                        .responseFactory()
                        .newResponse(RpcRequests.AppendEntriesResponse.getDefaultInstance(), RaftError.EINVAL,
                                "Parse serverId failed: %s.", request.getServerId());
            }


            
            //如果领导者的任期小于当前节点的任期，则直接忽略这条请求
            //这种情况也不是没可能出现，如果出现网络分区了，分区故障解决之后，另一个分区的领导者任期很大
            //把当前节点的任期修改了，修改的比当前节点原来的领导者的任期还要大，就会出现这种情况
            if (request.getTerm() < this.currTerm) {
                LOG.warn("Node {} ignore stale AppendEntriesRequest from {}, term={}, currTerm={}.", getNodeId(),
                        request.getServerId(), request.getTerm(), this.currTerm);
                return RpcRequests.AppendEntriesResponse.newBuilder()
                        .setSuccess(false)
                        .setTerm(this.currTerm)
                        .build();
            }
            
            
            //根据领导者的任期来判断当前节点是否需要进行身份降级，也就是变更身份为跟随者
            //在下面这个方法中很可能会把当前节点的领导者id设置为请求中传递过来的节点的id
            checkStepDown(request.getTerm(), serverId);
            
            
            //更新最后一次接收到领导者消息的时间
            updateLastLeaderTimestamp(Utils.monotonicMs());
            
            
            //获得请求中的前一条日志索引，注意，这里的请求肯定是来自领导者
            //领导者请求中的前一条日志，其实就是领导者在发送新的日志前的最后一条日志
            //如果根据索引获得到的任期也相同的话，这就意味着领导者和跟随者的日志是同步完整的
            final long prevLogIndex = request.getPrevLogIndex();
            //获得领导者节点的前一条日志的任期
            final long prevLogTerm = request.getPrevLogTerm();
            //获得当前跟随者节点的最后一条日志的任期，这里是根据领导者请求中的前一条日志索引获得的
            //这里其实就是对这个索引进行判断了
            final long localPrevLogTerm = this.logManager.getTerm(prevLogIndex);
            //接下来就是具体判断了，先判断当前节点的最后一条日志的任期是否和领导者请求中的任期不想等
            //不相等就说明日志不匹配
            if (localPrevLogTerm != prevLogTerm) {
                final long lastLogIndex = this.logManager.getLastLogIndex();
                LOG.warn(
                        "Node {} reject term_unmatched AppendEntriesRequest from {}, term={}, prevLogIndex={}, prevLogTerm={}, localPrevLogTerm={}, lastLogIndex={}, entriesSize={}.",
                        getNodeId(), request.getServerId(), request.getTerm(), prevLogIndex, prevLogTerm, localPrevLogTerm,
                        lastLogIndex, entriesCount);
                //这里返回的信息会被领导者处理
                return RpcRequests.AppendEntriesResponse.newBuilder()
                        .setSuccess(false)
                        .setTerm(this.currTerm)
                        .setLastLogIndex(lastLogIndex)
                        .build();
            }
            
            
            //这是我自己添加的代码
            LOG.info("FOLLOWER接收到了心跳消息！");
            
            
            //下面一大段代码都是和日志有关的，因为当前的请求消息可能就是日志复制消息，消息对象中封装着传入过来的日志
            //如果当前节点要复制主节点的日志，肯定要对比一下当前节点和请求中日志是否符合要求
            //第一版本中把这些代码删除了，这个方法中还有日志复制的逻辑，以及一些耗时的统计信息等等，这些代码在第一版本全部删除了
            //后面再补充完整
            //如果请求中的日志条目为0，说明当前的请求是心跳或者是探针消息
            if (entriesCount == 0) {
                //在处理探针消息的逻辑中也删除了一些和日志有关的代码，后面会加上
                //创建响应对象
                final RpcRequests.AppendEntriesResponse.Builder respBuilder = RpcRequests.AppendEntriesResponse.newBuilder()
                        .setSuccess(true)
                        .setTerm(this.currTerm)
                        .setLastLogIndex(this.logManager.getLastLogIndex());
                doUnlock = false;
                this.writeLock.unlock();
                return respBuilder.build();
            }
            return null;
        } finally {
            if (doUnlock) {
                this.writeLock.unlock();
            }
        }
    }





    //省略部分内容
}
```

上面代码块注释非常详细，并且 handleAppendEntriesRequest 方法的流程之前已经分析过了，所以就不再详细讲解 handleAppendEntriesRequest 方法了。好了，这个方法实现完毕之后，也只是跟随着节点处理了领导者发送的探针请求，并且回复给领导者一个探针响应，那么领导者怎么处理这个探针响应呢？这就要回到领导者内部的 Replicator 对象中了。

## Replicator 发送日志的时机

在上一小节，我已经为大家实现了 handleAppendEntriesRequest 方法，实现了跟随者节点处理领导者探针请求的功能。当跟随者节点处理完了探针请求之后，会回复给领导者一个探针请求的响应，这个响应最终仍然要被领导者内部的 Replicator 复制器对象来处理。这个也在第一小节讲解过了，当领导者发送探针请求的时候，会创建一个 RpcResponseClosureAdapter 对象，该对象内部的 run 方法会在领导者接收到探针响应后被回调，实际上最终执行的是 run 方法内部调用的 onRpcReturned 方法。所以，接下来，我只要为大家实现了这个 onRpcReturned 方法，那么领导者处理探针响应的功能也就实现了。这个 onRpcReturned 方法内部要执行的操作，实际上上一小节我已经为大家分析过了，也就是判断领导者定位日志是否成功，如果没有定位成功，就需要再次向跟随者发送探针消息，当然 prevLogIndex 和 prevLogTerm 的值也会适当改变，直到领导者知道应该发送的下一条日志的索引为几。所以，这里我就直接展示 onRpcReturned 的代码了，不再重复分析了。请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容




    //该方法会在探针请求收到响应后被回调，也会在给跟随者发送传输日志的请求，收到响应后被回调
    //在第四版本的代码中，该方法做了大量删减，很多方法参数用不上，第五版本会重构完整，这里这个status还没有用到，等到第六版本就会用到了
    //这个status还和Bolt框架有关系，所以牵扯到的知识还是有点复杂的，在实现了领导者传输日志给跟随者的功能之后再具体讲解
    @SuppressWarnings("ContinueOrBreakFromFinallyBlock")
    static void onRpcReturned(final ThreadId id, final Status status, final Message request, final Message response,final long rpcSendTime) {
        //对ThreadId判空
        if (id == null) {
            return;
        }
        Replicator r;
        //上锁，同时也对复制器对象进行了判空以及是否被销毁的判断处理
        if ((r = (Replicator) id.lock()) == null) {
            return;
        }
       
        
        //定义一个是否继续发送日志的标记
        boolean continueSendEntries = false;
       
        try {//在这里开始处理响应了，如果返回为true，说明上一次发送的请求没有问题
            //可以继续发送日志给跟随者了
            //onAppendEntriesReturned就是真正处理探针响应的方法，这个方法也处理日志传输响应
            continueSendEntries = onAppendEntriesReturned(id,(RpcRequests.AppendEntriesRequest)request,(RpcRequests.AppendEntriesResponse)response,rpcSendTime, r);
        } finally {
            
            //判断是否可以继续发送日志给跟随者节点
            if (continueSendEntries) {
                //在这里继续发送下一批日志
                r.sendEntries();
            }
        }
    }





    //具体处理领导者发送了AppendEntriesRequest请求后，接收到跟随者节点的响应的方法，这里我要多解释一下，这里的处理响应的方法
    //逻辑本身是很长的，只不过我在第四版本删减了一些，这个方法的逻辑大体上可以分为两部分，第一部分是是通过status判断响应本身有没有问题，着一部分和Bolt框架远程调用有关
    //可能是响应超时了，也可能是根本就没发出去，因为跟随者节点崩溃了，或者是其他的未知原因，这种情况下有一个统一的处理手段，那就是让当前发送日志的复制器阻塞一会
    //过了一定的时间后再向跟随者节点发送日志。第二部分逻辑就是判断响应的内容有没有问题，比如跟随者节点的日志任期和领导者不一致，跟随者节点的日志进度和领导者不一致等等
    //也许已经发生网络分区了，这也是响应失败的情况，这时候，就要具体情况具体处理了。当然，再有就是响应成功的情况，如果响应成功了
    //就说明日志已经被跟随者节点投票了，这时候就要根据投票结果判断日志是不是可以在领导者中提交状态机应用了
    private static boolean onAppendEntriesReturned(final ThreadId id, final RpcRequests.AppendEntriesRequest request, final RpcRequests.AppendEntriesResponse response, final long rpcSendTime,final Replicator r) {


         //响应失败，这里处理的响应和日志传输的请求有关
        //处理的是响应内容失败，而不是响应本身有问题，第一部分响应失败的逻辑被我删除了，第六版本会重构完整
        if (!response.getSuccess()) {

            //判断一下响应中的跟随者节点的任期是否比当前领导者大
            //如果大显然就是发生过网络分区，这时候就要让当前领导者进行身份降级
            if (response.getTerm() > r.options.getTerm()) {
                 //获取表示当前领导者的节点对象
                final NodeImpl node = r.options.getNode();
                //因为当前领导者要身份降级了，不是领导者显然就不能管理复制器对象
                //因此复制器对象也要销毁了，这个逻辑暂且不实现，后面再实现
                //r.destroy();
                //将自己的任期值增加，然后将自己身份降级，这些操作就会在increaseTermTo方法中执行
                node.increaseTermTo(response.getTerm(), new Status(RaftError.EHIGHERTERMRESPONSE,
                        "Leader receives higher term heartbeat_response from peer:%s", r.options.getPeerId()));
                return false;
            }


             //更新上一次向跟随者发送请求的时间
            if (rpcSendTime > r.lastRpcSendTimestamp) {
                r.lastRpcSendTimestamp = rpcSendTime;
            }
            
            
            //下面是处理探针响应，收到响应后判断跟随者的要接收的下一条日志是否比领导者要发送的下一条日志索引小
            //如果小则意味着跟随者节点进度落后
            if (response.getLastLogIndex() + 1 < r.nextIndex) {
                LOG.debug("LastLogIndex at peer={} is {}", r.options.getPeerId(), response.getLastLogIndex());
                //这里就把下一条要发送的日志索引设置为跟随者节点的，
                r.nextIndex = response.getLastLogIndex() + 1;
            } else {
                //走到这里意味着跟随者的下一条要接接收的日志索引比领导者还大
                if (r.nextIndex > 1) {
                    LOG.debug("logIndex={} dismatch", r.nextIndex);
                    //这个要从跟随者的handleAppendEntriesRequest方法中查看一下，是怎么根据领导者要发送的下一条日志的前一条日志的索引
                    //从自己的日志组件中获得对应日志的，如果获取不到，其实就是获取到了，但是跟随者节点的日志的任期比领导者小
                    //这时候需要让领导者的要发送的下一条日志递减，直到可以跟跟随者匹配到相同任期的日志，然后让领导者开始传输复制即可
                    //跟随者的日志索引比较大可能是因为旧的领导者发送的，比如整个集群就发送给了这一个节点，但是还没有提交，还没来得及提交旧的领导者就宕机了
                    r.nextIndex--;
                } else {
                    LOG.error("Peer={} declares that log at index=0 doesn't match, which is not supposed to happen",
                            r.options.getPeerId());
                }
            }
            //这里发送的就是探针消息，发送这个是为了开启下一次发送日志的操作
            //因为经过上面的流程，nextIndex的值已经更新了，所以需要重新发送探针消息来定位
            r.sendProbeRequest();
            return false;
        }

        //走到这里意味着探针请求响应是成功的
        //更新上一次向跟随者发送请求的时间
        if (rpcSendTime > r.lastRpcSendTimestamp) {
            r.lastRpcSendTimestamp = rpcSendTime;
        }  

        //得到这次请求批量发送的日志的数量，如果是探针请求，这里得到的就是0
        final int entriesSize = request.getEntriesCount();

        
        //更新接下来要发送的日志的索引，然后会跳出该方法，来到外层onRpcReturned方法，在finally中继续发送日志给跟随者节点
        r.nextIndex += entriesSize;
        return true;
    }






    //省略其他内容

    
}
```

到此为止，领导者节点处理探针响应的功能也实现了。大家可以再仔细看看上面代码块中的方法，重点是 onAppendEntriesReturned 方法的流程，要理解清楚。当然，我也能想到课程进行到这里，仍然没有涉及到领导者向跟随者节点传输日志的流程分析，讲解的内容似乎和文章标题并不匹配。还是那句话，请大家别急，我可以向大家坦诚地说一句，领导者向跟随者节点传输日志的功能非常简单，几乎没什么需要详细讲解的晦涩的细节知识。但是，在传输日志之前，领导者必须要知道自己向每一个跟随者节点传输的日志索引，这样才能正确而又连贯地根据日志索引向跟随者节点传输日志。那么当领导者处理完探针响应了，并且探针响应是成功的，也就意味着跟随者和领导者的日志进度是匹配的，领导者可以直接向跟随者传输日志了，这时候，要怎么开始传输日志呢？

答案**就在上面代码块的第 39 行，程序执行了 r.sendEntries() 这行代码，这里的 r 就是当前 Replicator 复制器对象，在 Replicator 复制器对象的 sendEntries 方法中，领导者终于开始向跟随者节点传输日志了**。那么日志究竟是怎么传输的呢？比如说，是一条一条传输，还是一批一批传输呢？这个问题也很值得讨论，但这一章显然是讲解不完了，下一章再为大家讲解吧。朋友们，我们下一章见！