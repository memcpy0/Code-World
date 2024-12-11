大家好，这一章我将再次重构我们的程序，没错，就是使用前面已经提到过多次的 Pipeline 模式来重构我们的程序。重构的原因很简单，之前我已经为大家实现了领导者向跟随者传输日志的功能，但这个功能存在很严重的缺陷。所以不得不使用 Pipeline 模式重构一下领导者向跟随者传输日志的方式。从 Pipeline 模式本身来说，本章内容也就两个模块，**一是实现领导者节点 Pipeline 模式传输日志，二是实现跟随者节点 Pipeline 模式回复响应**。当然，具体到每一个细节的话，本章的内容就非常多了。接下来，就让我们步入正题吧。

## Pipeline 模式要解决的问题

  

首先我要开门见山地提出一个问题，**为什么要使用 Pipeline 模式重构领导者向跟随者传输日志的方式呢？或者换另一种问法，为什么要使用 Pipeline 模式重构我们的程序**？理清楚这个问题非常重要，我们对程序做的每一次重构，每一次改动都是有逻辑，要弄清楚为什么要重构程序，否则明明已经实现了领导者向跟随者传输日志地功能，就让这个功能一直发挥作用吧，干嘛要费力气对它进行重构呢？所以，接下来就让我来为大家分析一下使用 Pipeline 模式重构日志传输功能的原因。当然，我知道大家现在肯定也都还不明白究竟什么是 Pipeline 模式，这个也不要紧，请大家先跟我一起思考一个场景。

按照我之前为大家实现的领导者向跟随者节点传输日志的功能，我们都已经知道了，**领导者向跟随者节点传输日志时，采用的方式是批量传输。领导者向跟随者节点批量传输日志时，每一次最多只允许传输 1024 条日志，传输的最大字节总量为 512 KB。当跟随者节点接收到领导者传输过来的这批日志之后，就会将这批日志落盘，落盘成功之后，会回复给领导者节点一个成功响应**。这个流程想必大家已经很清楚了，前几章我也为大家实现了整个流程，并且着重讲解了跟随者节点接收到领导者传输过来的日志之后的流程。但是有一点我确实没有为大家讲解，那就是当跟随者节点将领导者传输过来的一批日志成功落盘了，然后回复给领导者一个日志持久化成功的响应之后，领导者该如何处理这个响应呢？或者分析得更细致一些，假如跟随者节点的日志和领导者日志产生了冲突，并没有将日志落盘成功，回复给了领导者一个失败响应，这时候领导者要怎么处理跟随者回复的响应呢？总结下来就是，**领导者如何处理跟随者节点回复的日志传输的响应**？这个大家应该还没什么头绪吧？因为我确实没有为大家讲解过。前几章我没有为大家讲解，并非是觉得这个问题很复杂，需要花大量的篇幅来分析，仅仅是因为在前几章讲解这个问题的时机不太合适。那现在讲解这个问题的时机合适了，要从哪里开始分析呢？这时候就要从领导者向跟随者节点批量传输日志的方法说起了。请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;

    
    //批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志索引
    private boolean sendEntries(final long nextSendingIndex) {

        //其他内容省略

        try {//发送请求给跟随者
            rpcFuture = this.rpcService.appendEntries(this.options.getPeerId().getEndpoint(), request, -1,
                    new RpcResponseClosureAdapter<RpcRequests.AppendEntriesResponse>() {

                        @Override
                        public void run(final Status status) {
                            RecycleUtil.recycle(recyclable);
                            //接收到日子传输请求的响应后，onRpcReturned方法会被回调
                            onRpcReturned(Replicator.this.id, status, request, getResponse(),monotonicSendTimeMs);
                        }
                    });
        }


        //其他内容省略
    }




    
    //其他内容省略

}
```

在上面的代码块中，我们可以看到，当领导者向跟随者节点传输日志的时候，会一路执行到上面代码块中的 sendEntries(final long nextSendingIndex) 方法里，在该方法中将封装了日志信息的请求发送给跟随者节点。并且在发送请求的时候，还定义了一个 RpcResponseClosureAdapter 对象，该对象中的 run 方法会在接收到日志传输的响应后被回调，接着就会执行到 onRpcReturned 方法中。也就是说，onRpcReturned 方法就是领导者用来处理日志请求响应的方法。当然，如果大家对前几章的知识还有印象，肯定还记得，onRpcReturned 方法也是领导者用来处理探针请求的方法。在下面的代码块中，我把前几章实现的 onRpcReturned 方法也搬运过来了，请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;




    //该方法会在探针请求收到响应后被回调，也会在给跟随者发送传输日志的请求，收到响应后被回调
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
        final boolean isLogDebugEnabled = LOG.isDebugEnabled();
        StringBuilder sb = null;
        if (isLogDebugEnabled) {
            sb = new StringBuilder("Replicator ")
                    .append(r)
                    .append(" is processing RPC responses, ");
        }
        try {//在这里开始处理响应了，如果返回为true，说明上一次发送的请求没有问题
            //可以继续发送日志给跟随者了，onAppendEntriesReturned就是用来真正处理日志传输响应的
            //当然，它也用来处理探针请求的响应，这个是前几章实现的功能了
            continueSendEntries = onAppendEntriesReturned(id,(RpcRequests.AppendEntriesRequest)request,(RpcRequests.AppendEntriesResponse)response,rpcSendTime, r);
        } finally {
            if (isLogDebugEnabled) {
                sb.append("after processed, continue to send entries: ")
                        .append(continueSendEntries);
                LOG.debug(sb.toString());
            }
            //判断是否可以继续发送日志给跟随者节点
            if (continueSendEntries) {
                //在这里继续发送下一批日志
                r.sendEntries();
            }
        }
    }


    
    //其他内容省略

    
}
```

可以看到，在 onRpcReturned 方法中，又进一步调用了 onAppendEntriesReturned 方法来处理领导者接收到的日志请求的响应。onAppendEntriesReturned 方法我同样也为大家搬运到这里了，请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;


    
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


    
    //其他内容省略

    
}
```

onAppendEntriesReturned 方法的逻辑和我前几章为大家展示的逻辑一摸一样，**但现在 onAppendEntriesReturned 方法并不是仅仅被用来处理探针请求的响应了，还用来处理日志请求的响应**。这时候大家应该意识到，不**管是探针请求的响应还是日志请求的响应，领导者在处理它们的时候，会有一些公共流程。就像领导者发送探针请求和发送日志传输请求的时候，都会调用 fillCommonFields 方法填充 AppendEntriesRequest 请求体的公共属性，并且在跟随者节点的 NodeImpl 对象的 handleAppendEntriesRequest 方法中，也会采用一些公共流程处理探针请求和日志传输请求**。而 onAppendEntriesReturned 方法具体处理日志请求的表现就是上面代码块种的第 74 和 78 行代码，我把这几行代码截取出来了，单独展示在下面代码块中。请看下面代码块。

```
//得到这次请求批量发送的日志的数量，如果是探针请求，这里得到的就是0
final int entriesSize = request.getEntriesCount()
//更新接下来要发送的日志的索引，然后会跳出该方法，来到外层onRpcReturned方法，在finally中继续发送日志给跟随者节点
r.nextIndex += entriesSize;
```

在上面代码块中，第一行代码会通过发送给跟随者节点的日志传输请求获得本批次传输给跟随者节点的日志的数量，第二行代码就会根据本批次传输的日志数量，更新下一条要传输给跟随者节点的日志索引。到这里，大家应该就能意识到，**如果领导者想要发送下一批日志，按照现在实现的日志传输方式，领导者就必须要等到上一批日志的请求响应接收了，并且被处理了，在 onAppendEntriesReturned 方法中把下一批要发送跟随者节点的日志的起始索引更新了，然后领导者才能向跟随者节点发送下一批日志**。原因很简单，**领导者向跟随者节点传输日志，肯定要知道下一条要传输的日志索引，然后以这个索引为起点，开始批量传输日志，直到领导者内部没有日志了，或者本批次可以发送的日志达到了上限**。也就是说，按照我之前为大家实现的领导者传输日志的功能，**虽然领导者采用批量传输的方式向跟随者节点传输日志，但是这种传输情况，必须等到跟随者节点向领导者回复了成功响应，领导者在 onAppendEntriesReturned 方法中将 nextIndex 成员变量更新为最新的值，然后领导者才能继续发送下一批日志。当 onAppendEntriesReturned 方法执行完毕之后，程序就会重新来到 onRpcReturned 方法中，在 onRpcReturned 方法中，执行 r.sendEntries() 这行代码，发送下一批日志**。

我知道，讲解到这里，肯定有朋友会立刻指出来，说我在实现日志传输功能那一章根本不是这么实现的。没错，我当时确实不是这么实现的，我把在第 15 章为大家实现的日志传输功能的代码截取了一部分搬运到下面的代码块中了。请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;


    //发送日志给跟随者的方法，这里要再次强调一下，在sofajraft中，领导者并不会每次只发送一条日志给跟随者，而是会批量发送日志
    //每次都会发送一批日志，每一次具体发送多少日志，实现逻辑请看sendEntries(final long nextSendingIndex)方法
    void sendEntries() {
        boolean doUnlock = true;
        try {
            long prevSendIndex = -1;
            while (true) {
                //在一个循环中，不断获取要发送的日志的索引，然后发送日志，直到获取到的索引
                //超过了能够发送的日志，也就是说，领导者中的日志已经暂时发送完毕了
                //这时候就会跳出循环
                //getNextSendIndex()就是用来获得下一条可以发送的日志索引的方法
                final long nextSendingIndex = getNextSendIndex();
                if (nextSendingIndex > prevSendIndex) {
                    if (sendEntries(nextSendingIndex)) {
                        prevSendIndex = nextSendingIndex;
                    } else {
                        doUnlock = false;
                        break;
                    }
                } else {
                    break;
                }
            }
        } finally {
            if (doUnlock) {
                unlockId();
            }
        }
    }



    //得到下一个要发送的日志索引的方法
    long getNextSendIndex() {
        return this.nextIndex;
    }


    



    //批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志索引
    private boolean sendEntries(final long nextSendingIndex) {

        //创建一个AppendEntriesRequest请求构建器
        final RpcRequests.AppendEntriesRequest.Builder rb = RpcRequests.AppendEntriesRequest.newBuilder();
        
        //填充公共字段
        if (!fillCommonFields(rb, nextSendingIndex - 1, false)) {
            return false;
        }

        
        try {//开始在一个循环中封装要发送的日志条目了
            for (int i = 0; i < maxEntriesSize; i++) {
    
                final RaftOutter.EntryMeta.Builder emb = RaftOutter.EntryMeta.newBuilder();
                if (!prepareEntry(nextSendingIndex, i, emb, byteBufList)) {
                    break;
                }
                rb.addEntries(emb.build());
            }


             //更新下一条要发送的日志的索引
            //rb.getEntriesCount()是本批次要发送的日志条目
            //实际上就是AppendEntriesRequest请求中封装的EntryMeta元数据的数量
            //因为每一个LogEntry对应一个EntryMeta对象，所以有几个EntryMeta对象就意味着本批次发送了多少条日志
            this.nextIndex += rb.getEntriesCount();
  
        } 

        //省略部分代码
    }


    

    //其他内容省略
}
```

在上面的代码块中，大家可以看到，领导者要向跟随者节点发送日志，首先需要调用 sendEntries() 方法，因为领导者在处理完探针请求的响应之后，会在 onRpcReturned 方法中，执行 r.sendEntries() 这行代码，开始批量发送日志。在执行 sendEntries() 方法的时候，会在该方法内部执行 getNextSendIndex() 方法，获得下一条要发送的日志索引，其实就是 nextIndex 成员变量。然后再根据这个 nextIndex 执行 sendEntries(final long nextSendingIndex) 方法，开始批量发送日志。在我之前为大家实现的代码中，**在发送日志的时候，也就是在 sendEntries(final long nextSendingIndex) 方法中就开始更新 nextIndex 的值了。就是上面代码块的第 82 行。这样领导者就可以直接根据这个跟新后的 nextIndex 开始传输下一批日志，好像根本不用关心上一批发送的日志是否接收到了正确的响应**。但是，我也在注释中解释了，第 82 行代码是我自己添加的，迭代第 15 对应的代码时，我还没有考虑领导者如何处理日志请求的响应。所以就自行添加了 83 行的代码。实际上，在 sofajraft 框架中根本没有这行代码。原因很简单，**如果领导者发送的某批日志失败了，本来需要等待重新传输的，但是程序已经把 nextIndex 的值更新了，那还怎么根据 nextIndex 的值重新传输发送失败的日志呢**？就比如说领导者发送的索引为 5-10 的日志失败了，本来这批日志要重新传输，但是现在 nextIndex 的值已经被更新成 11 了，领导者显然就没办法再根据 nextIndex 传输刚才 5-10 索引的日志给跟随者节点了。只有当 nextIndex 的值仍然为 5 时，领导者才能重新传输刚才的日志。所以，在 sofajraft 框架中，只有在领导者接收到跟随者节点回复的日志请求的响应之后，并且判断这是个成功响应之后，才会更新 nextIndex 的值。因为上一批日志已经传输成功了，可以继续传输下一批日志了。这个逻辑应该是比较清楚的，现在，我的问题是如果就按照这种方式来让领导者向跟随者传输日志，程序中会存在什么问题呢？

为了不给大家造成一种我在故弄玄虚的错觉，首先我要说明一点，我在第 15 章为大家实现的传输日志功能没有任何致命的错误，不会让程序崩溃或者出错。因为领导者向跟随者发送日志是严格按照 **发出请求 ——> 接收响应 ——> 处理响应 ——> 再发送下一批日志** 的方式发送日志的，用更专业的术语来说，**领导者发送日志的动作是同步的，只有在接收到上一批日志发送成功的响应后，才能更新 nextIndex，然后再发送下一批日志**。这么做当然没有问题，不仅没有问题，还能保证程序肯定能顺利执行下去，不会出一点差错。但是我还是要再提出一个问题，领导者采用这种同步模式传输日志给跟随者节点，会对程序造成什么影响呢？从我最开始为大家讲解 Netty 的时候，我就经常重复一个观点，开发一个程序，从大方面来说只需要保证两方面。一是安全，二是性能。所谓安全就是程序的代码在逻辑上编写得没有问题，所谓性能就是当程序可以顺利执行的时候，就要思考使用什么手段能够让程序执行得更快。现在我们讨论的正是第二点，也就是我们开发的这个 raft 框架的性能问题。那么，目前使用同步手段传输日志，会对程序的性能造成什么影响呢？

要理清楚这个问题，就要回到 raft 共识算法本身了。请大家想一想，根据 raft 共识算法的设定，领导者会把客户端指令包装成一条条日志，然后把日志传输给跟随者节点。领导者把日志传输给跟随者节点的目的是为了使 raft 集群之间保持数据同步，为了是数据得到备份，这个是 raft 共识算法的目的之一。如果不考虑日志传输失败的情况，对于领导者来说，肯定内部有多少日志，就要一直向跟随者节点传输日志，最好能尽快把日志传输完毕。如果每发送一批日志就要等待跟随者节点回复成功响应，首先会降低领导者传输日志的效率；其次有可能造成数据丢失，因为领导者可能还没来得及传输日志就宕机或者发生网络分区了。说到底，领导者要做的就是把日志又快又安全地复制给跟随者节点即可，早点把日志复制过去，数据就能早点同步成功。那这样分析下来，是不是就意味着领导者不需要关心跟随者节点回复的日志请求响应了呢？只需要专心一批一批地发送日志即可？当然不是，**领导者还需要根据跟随者节点回复的日志传输响应判断是否要提交这一批日志呢**！这个是非常重要的操作，领导者怎么可能不关心日志传输响应呢？那该怎么办呢？接下来就让我来为大家阐述一下我的观点。

**我希望可以创造出一种伪异步的情况，让领导者不必同步等待每一批日志传输的响应，只需要专心发送日志。等待跟随者节点的响应真的被接收到了，领导者再去处理这个响应，如果发现有哪一批日志传输失败了，就以传输失败的这个日志索引为起点，开始重新传输之后的日志。那为什么可以采用伪异步的模式呢？原因有两点：第一就是领导者接收跟随者节点的日志传输响应的速度不是由领导者决定的，而是由跟随者节点和网络环境决定的，所以没必要让领导者干等着；第二就是领导者传输日志失败毕竟是少数情况，不会经常出现，所以也不用让领导者每次都同步等待跟随者节点的响应，专心发送日志即可**。那说了这么多，究竟什么是伪异步情况呢？

请大家不要着急，这个伪异步是什么先不急着解释，请大家先来思考这个问题：**那就是 nextIndex 成员变量什么时候更新**？如果让领导者专心发送日志，领导者内部有很多条日志的话，领导者每发送一批日志之后，必须更新 nextIndex 成员变量的值，才能继续发送下一批日志。可按照我们上面的分析，在 sofajraft 框架源码中，只有当领导者接收到跟随者节点回复的成功响应后，才能更新 nextIndex 的值。但我觉得这种同步等待响应的操作会拖累日志传输的性能，我希望让领导者能一直传输日志，不必等待上一批日志的响应是否成功。既然这样，就应该在领导者发送日志的时候，每发送一批日志，就更新一下 nextIndex 成员变量的值，然后再发送下一批日志。这样一来就会带来一个问题，如果领导者一直在批量发送日志，已经发送了很多批了，突然收到一个响应告诉领导者索引为 11-20 的日志发送失败了，但是现在 nextIndex 已经更新到 61 了，这样一来，领导者肯定就没办法重新传输失败的日志了，这就很棘手了。所以，**现在要解决的问题就成了什么时候更新这个 nextIndex 成员变量**。

对这个问题，我的解决方法也很明确，那就是**仍然让领导者在处理跟随者节点回复的日志成功响应中更新 nextIndex 成员变量，而在领导者发送日志的时候，使用另一种方式记录每一批日志的起始索引和本批次发送日志的数量**。这样一来，当领导者发送下一批日志的时候，就可以直接根据上一批日志发送的起始索引加上发送数量，直接计算出要发送的下一批日志的起始索引。比如说领导者发送第一批日志的时候，将这批日志的起始索引 1 和发送的日志数量 20 记录下来，发送下一批日志的时候，就可以直接用 1 + 20 得到下一批要发送的日志的起始索引 21。这么做看起来挺简单的，而且也解决了领导发送日志时确定下一批要发送的日治索引的问题。但这么做又会带来三个新的问题：

**第一，那就是每一批发送的日志的起始索引信息和发送的日志数量该怎么记录呢？我刚才说要定义一个对象封装这些信息，暂且我们就称这个对象为日志请求元信息对象吧，那要定义一个什么样的元信息对象来封装这些信息呢？**

**第二，请大家想一想，每当领导者发送出去一批日志之后，肯定就要为这批日志请求创建一个元信息对象，并且，在没有接收到这批日志请求对应的响应之前，与日志请求对应的元信息对象就要一直保存在内存中，原因很简单，在没有接收到对应的响应之前，领导者并不知道发送的某批日志是否成功了。如果成功了，直接就可以垃圾回收对应请求的元信息对象了，如果没有成功，那还需要根据这个元信息对象记录的起始索引来重新发送这批日志。那么，这些日志请求的元信息对象要怎么存放呢？**

**第三，也是最重要的一点，那就是领导者发送的每一批日志的请求怎么和跟随者节点回复的响应对应呢？比如说领导者一共发送了 5 个日志传输请求给跟随者节点。第三个请求发送的是索引为 16-40 的日志，当领导者接收到跟随者的日志响应时，怎么就知道当前接收到的响应是和第三个发送的日志请求对应的呢？**

只要以上三个问题解决了，本章的核心内容就算讲解完毕了。当然，一下子看到上面三个问题，肯定也不会立刻就有什么解决思路，根据我自己的感受，我也只能按照过去的某些经验来发散自己的思维。就比如说上面的第三个问题，这个问题的本质就是请求和响应如何对应的的问题，单看这个问题的话就会简单很多了，因为这并不是什么新问题，反而是很常见的场景，并且也有很常规的解决方法。举个最直接最简单例子，**在 RPC 框架中，都会给请求响应定义唯一序号，好让请求和响应对应。这个对大家来说应该都是常识了，那么在解决上面第三个问题时，也可以使用这种方法，给每一个请求和响应定义唯一序号**。领导者发送了序号为 5 的请求之后，只有接收到序号也为 5 的响应，就知道目前处理的响应对应的是序号为 5 的请求。**并且我们就可以把这个请求序号也定义在请求的元信息对象中，这样通过请求序号和响应序号，就可以立刻知道当前响应对应的是哪一批日志**。按照这种方式，接下来，我们可以先尝试着解决一下上面的第一和第三个问题。

首先，让我们先试着定义一下日志请求的元信息对象。根据刚才的分析，这个对象要封装本批次发送的日志的起始索引，还有对应的日志条数，当然，还有一个请求序号。但我觉着仅仅有这些还不够，既然是元信息，就应该把本次发送的日志的总字节也封装了。本来封装这些信息就够了，但是在 sofajraft 框架中，还为这个元信息对象多定义了两个成员变量，**一个是本次发送请求的类型，因为领导者不仅仅会向跟随着节点传输日志，还会根据情况向跟随者节点安装快照，所以，定义一个请求类型成员变量也是应该的**。除此之外，sofajraft 框架还定义了一个成员变量，那就是一个 Future 类型的成员变量 rpcFuture。我知道这个 rpcFuture 成员变量肯定会让大家感到困惑，不知道它是怎么来的，但我可以告诉大家，其实你们早就见过这个 rpcFuture 了。如果大家已经看过前面几个版本代码的话，肯定都已经知道了，不管是领导者发送心跳请求还是探针请求，或者是日志请求，发送请求时都会返回一个 Future 类型的对象。就像下面代码块中展示的这样。请看下面代码块。

```
//发送探针请求，appendEntries方法返回了一个Future类型的对象
final Future<Message> rpcFuture = this.rpcService.appendEntries(this.options.getPeerId().getEndpoint(),
        request, -1, new RpcResponseClosureAdapter<RpcRequests.AppendEntriesResponse>(
            @Override
            public void run(final Status status) {
                onRpcReturned(Replicator.this.id, status, request, getResponse(), monotonicSendTimeMs);
            }
        });
```

之前我一直没有跟大家讲解过这个 rpcFuture 对象的作用，现在我可以简单解释一下，**这个 rpcFuture 对象的 cancel 方法可以取消发送日志请求的操，当然并不是真正意义上的取消，要是在接收到本次请求的响应之前，调用了与这个请求对应的 rpcFuture 的 cancel 方法，那么在领导者就会忽略这个请求的响应**。具体的逻辑可以看我为大家录制的视频，因为这个逻辑还和 bolt 框架有一点关系，所以都放在视频中讲解了。当然，大家也可以直接去看代码，代码中注释非常详细，也可以从代码中掌握这部分的逻辑。最后我还想在再补充一点，实际上发送请求返回的 rpcFuture 对象在 sofajraft 框架中并没有被使用，只在测试类中简单展示了一下 rpcFuture 的使用方法，大家简单看看即可。

总之，为了进一步向 sofajraft 框架靠拢，现在我已经决定要在日志请求的元数据信息中定义六个成员变量，分别是：**本批次发送的日志的起始索引，本批次发送的日志条目数量，本批次发送的日志的总字节数，日志请求的序号，日志请求的类型，还有刚才讲解的 rpcFuture**。这六个成员变量确定好了之后，现在就要确定一下这个元信息对象的名称，这里就可以直接采用 sofajraft 框架中定义的名称，就把这个对象的类型定义为 Inflight 即可。所谓 Inflight，就是还在途中的意思，也就是说发送出去的请求还没有收到响应，响应还在途中的。接下来，我就把这个 Inflight 类的具体内容展示给大家。请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //其他内容省略






    //Inflight类是Replicator的一个内部类
    //该类的对象封装的就是每一个发送给跟随者请求的信息
    static class Inflight {
        
        //本次请求中发送的日志条目的数量
        final int count;
        
        //本批次日志的第一条日志的索引
        final long startIndex;
        
        //本批次发送日志的总字节大小
        final int size;
        
        //请求所对应的Future对象
        final Future<Message> rpcFuture;
        
        //发送的请求的类型，也许是传输日志请求，也许是安装快照请求
        final RequestType requestType;
        
        //本次发送请求的请求序号
        final int seq;
        
        //构造方法
        public Inflight(final RequestType requestType, final long startIndex, final int count, final int size,
                        final int seq, final Future<Message> rpcFuture) {
            this.seq = seq;
            this.requestType = requestType;
            this.count = count;
            this.startIndex = startIndex;
            this.size = size;
            this.rpcFuture = rpcFuture;
        }

        
        @Override
        public String toString() {
            return "Inflight [count=" + this.count + ", startIndex=" + this.startIndex + ", size=" + this.size
                    + ", rpcFuture=" + this.rpcFuture + ", requestType=" + this.requestType + ", seq=" + this.seq + "]";
        }

        //判断本次请求是否发送了日志条目
        boolean isSendingLogEntries() {
            return this.requestType == RequestType.AppendEntries && this.count > 0;
        }
    }
    


    
}
```

好了，随着 Inflight 内部类的实现，我刚才提出的三个问题中的第一个问题显然已经解决了，还剩下第二个和第三个问题。接下来，我们就看一下第二个问题怎么解决，也就是分析分析为每一个日志请求创建的 Inflight 对象要存放到哪里？在我看来，这个问题要和第三个问题联系起来，因为 Inflight 对象存放的方式，也许会影响到响应和请求的对应手段。比如说，按照我自己的观点，**我更愿意把每一个 Inflight 对象放到一个优先级队列中，并且是按照每一个 Inflight 对象中的请求序号存放**。**我会在 Replicator 类中定义一个 int 类型的成员变量，这个成员变量可以一直自增，充当每一个日志请求的请求序号。当领导者向跟随者发送每发送一批日志时，就会得到一个唯一的请求序号，然后为这个发送日志的请求创建一个 Inflight 对象，这个对象中封装了这批日志的元信息。这时候，我其实还可以在 Replicator 类中定义一个优先级队列，根据每一个请求的请求序号把请求存放到优先级队列中，序号小的排在队列前面。这样一来，每一个 Inflight 对象就会按照请求序号由小到大的顺序存放到优先级队列中。我定义的这个优先级队列，也就可以称为 inflights 队列，反正是用来存放每一个 inflight 对象的**。

那么就这样定义了一个 Inflights 队列之后有什么用呢？这时候就要和响应的处理连接上了。我的意思是，**除了为请求定义一个队列，我还可以为领导者接收到的响应也定义一个优先级队列，就把这个队列名称定义为 pendingResponses，当领导者接收到每一个响应时，并不会直接处理这个响应，而是会把自己接收到的响应按照响应序号由小到大的方式存放到这个 pendingResponses 优先级队列中，这样响应序号小的响应就会放在队列的首位。然后领导者再从响应队列中取出队首的响应，再从请求队列中取出队首的 Inflight 对象，对比它们的序号是否相同，如果相同就说明请求和响应是一一对应的，如果不相同就说明本次到达的响应并不是领导者要处理的响应，继续等待和请求对应的响应到来即可**。

这样一来，就相当于用两个队列把请求和响应存放起来了，就像下面展示的这样。

```
请求Queue：Inflight1    Inflight2     Inflight3      Inflight4 ......  


响应Queue：Response1    Response2     Response3      Response4 ......
```

如果按照上面响应队列的方式来处理跟随者节点回复的响应，**那么请求和响应就会形成一一对应的关系。领导者既可以专心发送日志请求，也不必同步等待日志请求的响应了。反正有响应队列存放响应，处理响应的的时候只需要根据响应序号和请求队列中相应 Inflight 对象的请求序号做对比，就知道当前处理的响应是哪一个请求的了。可能是两个队列形成了一个管道，所以就把这种传输日志的方式称为 Pipeline 日志传输模式**。至于为什么说是伪异步的，这个也很简答，**因为说到底领导者发送请求和响应都是由分配给 Replicator 复制器的单线程执行器执行的，领导者发送请求和处理响应并不是真正的异步，因为只有一个线程在干活，都是由领导者分配给 Replicator 复制器的单线程处理器干的。当领导者内部没有日志了，Replicator 复制器不能再继续发送日志了，这个单线程执行器就会去自己的任务队列中执行下一个任务，下一个任务也许就是要处理某个日志请求的响应。那单线程执行器中的任务是谁提交的呢？肯定就是 Netty 的 IO 线程呀**，这些和 bolt 有关的内容，我都录制在视频中了，大家可以去视频中查看。

到此为止，我就为大家把 Pipeline 日志传输模式为大家讲解完毕了。我相信到此为止，大家也应该明白了，为什么要引入 Pipeline 日志传输模式，Pipeline 传输日志解决了什么问题？同时大家也可以发散一下思维，思考一下，如何在代码层面实现 Pipeline 传输日志功能。

总的来说，**领导者使用 Pipeline 模式向跟随者节点传输日志，可以让领导者不必同步等待日志请求的响应，可以专心传输日志，提高日志传输效率。处理响应的时候，可以根据响应和请求的序号来判断响应与请求是否对应，如果请求与响应对应，并且该响应是一个失败响应，那么领导者就可以直接根据请求的 Inflight 对象封装的本批日志的起始索引，开始重新传输日志**。其实这里还有一个细节需要为大家补充，请大家想一下，如果要是重新传输日志，要传输多少日志呢？比如说领导者发送给跟随者节点的第 21-60 条日志传输失败了，当领导者处理这批日志的失败响应时，已经把日志发送到第 100 条了。这个时候，领导者重新传输日志的话，是只传输第 21-60 条，还是说从 21 条往后的日志都要重新传输呢？这个问题大家可以先思考一下，下一章我为大家实现 Pipeline 模式代码的时候，大家就知道究竟该怎么重新传输了。

接下来就应该从代码层面上为大家实现一下这个 Pipeline 传输日志模式了，当然，写了一大堆文字，实际上，我也只是为大家把领导者发送请求时的 Pipeline 流程简单分析了一下，至于回复的响应遵循怎样的 Pipeline 流程，我还没为大家分析。**比如跟随者节点回复的响应怎么放到响应队列中，回复的响应用不用定义什么相关的对象来封装，毕竟要给这个响应设置响应序号，还有最重要的一点就是这个响应序号要怎么设置**，响应和请求是一一对应的，领导者发送日志传输请求的时候如果能把请求的序号也传输给跟随者节点，跟随者节点回复响应的时候再把这个序号回复给领导者，这样领导者就能直接根据序号来确定响应和请求的关系。但是我们已经使用 protocol buffer 协议定义好了请求体和响应体，请求体和响应体中并没有定义序号字段，所以，如何让响应获得对应的请求序号就非常重要了。大家可以带着上面这几个问题，一起跟我实现 Pipeline 日志传输功能了。

## 实现领导者发送请求的 Pipeline 模式

  

根据我刚才为大家的分析，如果领导者要以 Pipeline 模式向跟随者节点发送请求，这时候我们就可以在 Replicator 类中定义两个新的成员变量，一个是可以一直自增的 int 请求序号，一个就是存放 Inflight 请求 inflights 的队列。这两个成员变量定义好了之后，就可以在领导者向跟随者节点发送日志请求之后，把请求的元信息都封装到一个 Inflight 对象中，然后把这个对象存放到 inflights 队列中。具体的流程就是：先分配一个请求序号，然后发送请求，再创建与请求对应的 Inflight 对象，最后把 Inflight 对象存放到 inflights 响应队列中。具体实现就在下面的代码块中，请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //其他内容省略


    //复制器对象给跟随者发送的每一条请求的序号，这个序号是随着请求递增的
    //并且，这个序列号十分重要，要使用Pipeline模式发送消息，就必须使用这个请求序号
    //这样一来，每一个请求和响应都会根据这个序号一一对应了，处理日志结果的时候也就不会乱序了
    private int reqSeq = 0;


    //所有发给跟随者的请求的元信息都会被包装到Inflight对象中，在还没有收到对应的响应前，请求会一直存放在这个队列中
    //请求的响应被处理了，才会将请求Inflight对象从队列中移除
    //每次添加Inflight对象，都会从队列尾部添加，这样序号越小的Inflight对象，就会放在队首了
    //Inflight只是一个双端队列，但是按照队尾添加的方式，就具备了优先级队列的效果
    //请求序号最小的Inflight对象放在队首，最大的序放到队尾
    private final java.util.ArrayDeque<Inflight> inflights = new ArrayDeque<>();



    //其他方法省略

    

    //批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志
    private boolean sendEntries(final long nextSendingIndex) {


        //省略部分内容


         //得到本次请求的请求序号
        final int seq = getAndIncrementReqSeq();

         try {//发送请求给跟随者
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





    //该方法的作用就是把一次请求的信息封装到一个Inflight对象中，然后把Inflight对象存放到inflights队列中
    private void addInflight(final RequestType reqType, final long startIndex, final int count, final int size,
                             final int seq, final Future<Message> rpcInfly) {
        //记录最新发送给跟随者节点的请求
        this.rpcInFly = new Inflight(reqType, startIndex, count, size, seq, rpcInfly);
        this.inflights.add(this.rpcInFly);
    }



    //Inflight类是Replicator的一个内部类
    //该类的对象封装的就是每一个发送给跟随者请求的信息
    static class Inflight {
        
        //本次请求中发送的日志条目的数量
        final int count;
        
        //本批次日志的第一条日志的索引
        final long startIndex;
        
        //本批次发送日志的总字节大小
        final int size;
        
        //请求所对应的Future对象
        final Future<Message> rpcFuture;
        
        //发送的请求的类型，也许是传输日志请求，也许是安装快照请求
        final RequestType requestType;
        
        //本次发送请求的请求序号
        final int seq;
        
        //构造方法
        public Inflight(final RequestType requestType, final long startIndex, final int count, final int size,
                        final int seq, final Future<Message> rpcFuture) {
            this.seq = seq;
            this.requestType = requestType;
            this.count = count;
            this.startIndex = startIndex;
            this.size = size;
            this.rpcFuture = rpcFuture;
        }

        
        @Override
        public String toString() {
            return "Inflight [count=" + this.count + ", startIndex=" + this.startIndex + ", size=" + this.size
                    + ", rpcFuture=" + this.rpcFuture + ", requestType=" + this.requestType + ", seq=" + this.seq + "]";
        }

        //判断本次请求是否发送了日志条目
        boolean isSendingLogEntries() {
            return this.requestType == RequestType.AppendEntries && this.count > 0;
        }
    }


    
    //省略其他内容
}
```

好了朋友们，现在我就把领导者向跟随者发送传输日志的请求用 Pipeline 模式重构了。接下来就应该为大家实现领导者使用 Pipeline 模式处理日志请求响应的功能了。但这一章的内容已经很多了，所以剩下的内容就留到下一章讲解吧。好了各位，我们下一章见！