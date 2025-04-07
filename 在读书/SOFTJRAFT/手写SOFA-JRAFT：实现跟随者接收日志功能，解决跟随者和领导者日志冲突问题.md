大家好，这一章我将为大家实现跟随者节点接收日志功能，跟随者节点接收日志之后肯定需要将这一批日志进行持久化，但是将日志持久化到硬盘的功能，在前面的章节我已经为大家实现了，所以这一章的内容并不多。但是大家别忘了在上一章结束的时候，有一个功能我并没有为大家实现，就是当领导者内部又产生了新的日志之后， Replicator 复制器对象怎么能感知到这批日志，然后继续向跟随者节点传输日志。正好这一章内容并不是很多，所以我就把这个功能放到这一章来为大家实现。除此之外还有另一个原因，在第四版本代码中，我忘记重构 LogManagerImpl 日志管理器对象中的 appendEntries 方法，这就导致 Replicator 复制器对象不能及时感知到领导者内部产生了新的日志。这个 appendEntries 方法在第五版本代码中被我重构了，所以 Replicator 复制器对象继续向跟随者节点传输日志的功能在第五版本代码中才算是完全实现了。好了，多余的话就不多说了，让我们先回归正题吧，先把跟随者节点接收日志的功能实现了，然后再实现 Replicator 复制器对象继续向跟随者节点传输日志的功能。

## 实现跟随者节点接受日志功能

课程进行到这里，我为大家分析某些功能的实现思路时，已经不用再写一大堆文字分析了，因为很多知识在前面已经讲完了，铺垫工作都已经做好了。就比如说现在我要跟大家实现跟随者节点接收日志功能，我们首先应该想到，**跟随者节点肯定是接收到领导者发送过来的日志传输请求，也就是 AppendEntriesRequest 请求，然后在一个方法中处理这个请求**，具体的处理流程肯定就是：**从 AppendEntriesRequest 请求中获取领导者传输过来的这批日志，将这批日志进行持久化，最后也别忘了给领导者一个响应**。这个逻辑大家应该可以理解。那么跟随者节点应该在哪个方法中处理 AppendEntriesRequest 请求呢？这个就可以直说了，肯定还是在 NodeImpl 对象的 handleAppendEntriesRequest 方法中处理 AppendEntriesRequest 请求，原因也很简答，因为心跳请求和探针请求使用的都是 AppendEntriesRequest 请求体，并且也都是由 NodeImpl 对象的 handleAppendEntriesRequest 来处理的，所以轮到日志传输的时候，因为日志数据也是封装在了 AppendEntriesRequest 请求体中，所以同样也是由 NodeImpl 对象的 handleAppendEntriesRequest 方法来处理这个请求的。

讲到这里，我相信肯定会有一些朋友感到困惑，目前为止，我们已经使用 AppendEntriesRequest 请求体实现了三个功能，**一个是心跳检测功能，一个是探针功能，最后就是日志传输功能**。这个三个功能都使用了 AppendEntriesRequest 请求，并且都是由 handleAppendEntriesRequest 方法处理的，那么，跟随者节点究竟是怎么区分 handleAppendEntriesRequest 方法当前处理的是哪个功能的请求呢？会不会发生乱序的情况呢？这么问好像有点模糊，让我用一个具体的例子为大家分析一下。

在 sofajraft 框架内部，各个 raft 节点想要交流，需要通过各个节点内部集成的 bolt 框架来进行网络通信。而 bolt 框架实际上就是对 netty 做了一层封装，所以跟随者节点要想接收到领导者传输过来的请求，实际上跟随者节点内部还是部署了 netty 构建的服务器。既然是 netty 构建的服务器，根据我们对 netty 的了解，**netty 内部肯定由自己的 IO 单线程执行器来接收领导者发送过来的请求，当请求发送过来之后，为了提高 IO 单线程执行器的效率，还会创建一个业务线程池，这样一来，IO 单线程执行器接收到领导者发送过来的请求后，就可以直接把处理请求的操作提交给业务线程池，让业务线程池处理业务逻辑， IO 单线程执行器就可以只负责处理 IO 事件**。这个逻辑也是很常规的逻辑，大家可以自己思考思考。

我在这里之所以给大家分析 netty 内部的工作流程，是因为假如有一个业务线程池，专门处理领导者发送过来的请求，我能想到的是，不管这个请求代表的是什么操作，可能是心跳检测，可能是探针请求，也可能是日志传输。就比如说**领导者一直在向跟随者节点发送请求，并且发送的都是 AppendEntriesRequest 请求，一下子发送了 6 个 AppendEntriesRequest 请求。这 6 个 AppendEntriesRequest 请求被 netty 的 IO 单线程执行器接收到了之后，就会把处理请求的操作封装为一个个任务，然后提交给业务线程池，也就是放到业务线程池的队列中**。假如这 5 个请求中，第 1 个，第 3 个，第 4 个都是心跳检测请求，第 2 个是探针请求，第 5 个，第 6 个是日志传输请求。现在业务线程池要处理这些请求了，请大家想一想，我们的程序会出现什么问题呢？

这时候我们就该想想业务线程池执行任务的具体情况了，**业务线程池中有一个任务队列，而 netty 的 IO 单线程执行器会把处理每一个请求的操作封装为任务提交给业务线程池，当业务线程池要执行任务的时候，业务线程池中的多个线程就会从任务队列中取出每一个任务，而线程处理每一个任务的步骤也很直接，如果跟随者节点接收到的是 AppendEntriesRequest 请求，那么业务线程池中的线程肯定就会调用 NodeImpl 对象的 handleAppendEntriesRequest 方法处理 AppendEntriesRequest 请求**。

如果是这样的话，当业务线程池中的多个线程同时处理请求 1，请求 2，或者是同时处理前 4 个请求时，这种情况还好说，因为不管是心跳请求还是探针请求，都是 handleAppendEntriesRequest 方法处理的，也就是说业务线程池中的线程在并发处理前 4 个请求的时候，都会调用 handleAppendEntriesRequest 方法，而 handleAppendEntriesRequest 方法内部是有同步锁的，所以并不会发生并发问题。**注意我这里说的不会发生并发问题，指的是不会因为处理请求顺序错乱而发生问题。因为 handleAppendEntriesRequest 方法内部加了同步锁，就涉及到多个线程强锁的情况了，假如业务线程 1 处理的是请求 1，业务线程 2 处理的是请求 2，但是在执行 handleAppendEntriesRequest 方法的时候，业务线程 2 先抢到了同步锁，这个时候请求 2 就会先于请求 1 处理，但按照发送顺序来说，明明是请求 1 先被领导者发送到跟随者节点**。我想强调的是，**对于心跳检测和探针消息请求处理顺序发生改变的情况，并不会导致 raft 程序出现什么问题**。handleAppendEntriesRequest 方法的同步锁如下所示。

```
@Override
public Message handleAppendEntriesRequest(final RpcRequests.AppendEntriesRequest request, final RpcRequestClosure done) {
    //这些重复逻辑就不添加注释了
    boolean doUnlock = true;
    //一进方法就加锁了
    this.writeLock.lock();


    //省略其他内容

}
```

因为说到底，handleAppendEntriesRequest 方法处理心跳请求和探针请求的流程是一摸一样的，我在为大家实现探针请求功能的时候，就跟大家讲过，探针请求和心跳请求在本质上没什么却别，这两个请求都是由 AppendEntriesRequest 请求体封装的，AppendEntriesRequest 请求体中的公共数据都一样，被 handleAppendEntriesRequest 方法处理的流程也一样。只不过在领导者的 Replicator 复制器对象中，处理这两个请求响应的回调方法不同，并且探针请求基本上只发送一次，只要日志定位成功了，就不会再像心跳请求那样持续发送了。大家可以品味一下这个逻辑，如果这个逻辑理解了，那么接下来，请跟我思考另一个场景。

当业务线程池中的两个线程分别处理请求 5 和请求 6，也就是说处理的是日志传输请求。如果是日志传输请求，那么 AppendEntriesRequest 请求体中肯定封装着领导者传输过来的日志数据。假如说请求 5 中封装着 10 条日志，请求 6 中封装着 5 条日志。在领导者内部，请求 5 中的日志索引是排在请求 6 中日志索引前面的，也就是说跟随者在处理日志传输请求时，要将日志按照索引持久化到本地，就必须要先处理请求 5 中的日志，然后再处理请求 6 中的日志。但是按照刚才对探针请求和心跳请求处理流程的分析，因为并发情况下多个线程要争抢同步锁，如果处理请求 6 的线程先抢到了同步锁，那么请求 6 就会在请求 5 之前被处理。这种情况下，显然就出现了日志处理错乱的情况。那这种情况应该怎么解决呢？

**sofajraft 框架中的处理方式非常直接，既然不希望出现日志处理乱序的情况，索性就把业务线程池也定义成一个单线程执行器。这样一来，netty 的 IO 单线程执行器就会按照请求到来的顺序把对请求的处理封装成一个个任务，然后提交给业务单线程执行器中。而业务单线程执行器也会按照请求到来的顺序处理每一个请求了**。这样在处理日志传输请求的时候，就不会出现日志处理顺序错乱的情况。这个逻辑很简单吧，确实也没什么可讲的，理解了这一点，本章剩下的内容就没什么难度了，都是常规逻辑。当然，我还要再补充一点，为什么这里把业务线程池定义为单线程执行器是可行的，**最重要的原因就是心跳请求和探针请求实际上并不需要什么 IO 操作，直接处理完就可以回复响应了，所以单线程执行器完全处理得过来；而处理日志传输请求需要进行 IO 操作，但是日志落盘的操作是由 Disruptor 框架的批处理器负责的，也就是说业务单线程执行器如果处理的是日志请求，会把从领导者获得的日志直接提交给 Disruptor 框架来处理，这样一来，业务单线程执行器就可以立即去处理后面的心跳请求或者下一个日志请求。而 Disruptor 框架的批处理器也是单线程的，这就保证了日志在被处理时仍然是按照请求到来的顺序被处理的。所以，将业务线程池定义为单线程执行器并不会降低跟随者处理请求的性能**。这个逻辑也请大家再思考思考。

好了，关于跟随者节点内部处理日志传输请求的线程是怎么安排的，我已经跟大家分析完毕了。但在本章我还不会展示相关代码，因为在下一章实现 Pipeline 模式传输日志的时候才会展示相关代码，说白了，刚才分析的内容实际上是下一章的知识，在本章讲解算是为下一章做一个铺垫吧。接下来还是让我们回到正题，看看 handleAppendEntriesRequest 方法究竟是怎么接收领导者传输过来的日志。

领导者传输日志的方式大家也都掌握了，在 AppendEntriesRequest 请求中，按照日志索引的顺序封装着每一条日志的元数据信息，也封装着所有日志条目的 data 数据。AppendEntriesRequest 请求的结构如下。请看下面代码块。

```
//required表示的是必填的字段
message AppendEntriesRequest {
  required string group_id = 1;
  required string server_id = 2;
  required string peer_id = 3;
  required int64 term = 4;
  required int64 prev_log_term = 5;
  required int64 prev_log_index = 6;
  //这个就是封装元数据的字段，这个字段被repeated修饰
  //意思就是可以封装一个或多个EntryMeta对象，也就是可以封装一个或多个日志条目的元数据信息
  repeated EntryMeta entries = 7;
  required int64 committed_index = 8;
  //这个字段就是用来封装所有日志条目的data数据
  //optional就是附加数据的意思
  optional bytes data = 9;
};
```

封装日志元数据信息的 EntryMeta 结构如下。请看下面代码块。

```
message EntryMeta {
    required int64 term = 1;
    required EntryType type = 2;
    repeated string peers = 3;
    //这个就是一条日志封装的data字节的长度，也就是客户端指令的字节长度
    optional int64 data_len = 4;
    repeated string old_peers = 5;
    optional int64 checksum = 6;
    //这个learners学习者大家可以暂时忽略
    repeated string learners = 7;
    repeated string old_learners = 8;
};
```

回顾了这两个数据类型的结构之后，请大家思考一下，假如领导者向跟随者发送了 5 条日志，每一条日志都是一个 LogEntry 对象，跟随者节点在接收到日志传输请求之后，怎么才能从日志请求中把日志还原成 5 个 LogEntry 对象呢？如果让我来设计代码，我并不会觉得有任何难度。因为没一个 LogEntry 对象的元数据信息都在，欸一个 EntryMeta 中，我可以从 EntryMeta 中获得对应的 LogEntry 对象的任期，集群配置信息等等，至于 LogEntry 对象的 data 数据，则可以直接根据元数据中的 data_len 的值，从 AppendEntriesRequest 请求的 data 字段中获得。

这里还是让我再来为大家举一个例子，假如说领导者向跟随者发送了 3 条日志，那么领导者在将这 3 条日志封装到 AppendEntriesRequest 请求中的时候，会严格按照日志索引的顺序封装日志数据到 AppendEntriesRequest 请求体中。如果这三条日志的索引分别为 1、2、3，那就会先得到日志 1 的元数据对象，然后把索引为 1 的日志的 data 放到一个 ByteBuffer 对象中，处理索引为 2 的日志时，会把索引为 2 的日志的 data 累加到索引为 1 的日志后面。就像下面这样。

```
领导者发送给跟随者节点的日志： LogEntry1   LogEntry2   LogEntry3

repeated EntryMeta entries：EntryMeta1  EntryMeta2  EntryMeta3

optional bytes data：		  data1       data2        data3
```

所以，**当跟随着节点接收到 AppendEntriesRequest 请求之后，从请求中得到的 EntryMeta 元数据集合和 bytes 数据，其内部的数据和对象也都是按照日志索引顺序排列好了。跟随者节点从 EntryMeta 元数据集合中得到的第一个 EntryMeta 元数据就是索引为 1 的日志的，然后根据EntryMeta 元数据中的 data_len 从 bytes 中得到的字节数据就是索引为 1 的日志的有效数据，将这两个数据组合起来，就可以还原出一个完整的 LogEntry 对象了**。接下来的日志对象都可以按照这个流程来还原。这里还有一个小问题，还原出来的每一个 LogEntry 日志条目对象的日志索引该怎么赋值呢？这时候请大家想一想，**领导者向跟随者传输每一批日志的时候，是不是也会在 AppendEntriesRequest 请求中封装一些公共数据，这些公共数据之中就有一个 prev_log_index，代表着领导者最后一次发送给跟随者的日志的索引。得到了这个值，也就能明确跟随者节点本地最新日志的索引，所以就从这个值开始递增，这样不就可以给跟随者解析出来的每一个 LogEntry 对象的索引赋值了吗**？这个问题就算是解决了。

那当领导者发送过来的本批次日志全部都还原成 LogEntry 对象了之后呢？这就很好说了，直接把这批日志交给 LogManagerImpl 日志管理器组件，调用 LogManagerImpl 对象的 appendEntries 方法，将这批日志落盘即可。这不就是前几章实现的功能吗？到这里，我们的知识就串联起来了。到这里就把日志落盘的大概流程讲解完毕了，当然，**按照 raft 共识算法，日志落盘成功之后，肯定要给领导者回复一个成功响应，只有这样，领导者才能知道这批日志是否可以被提交**。不用说，**发送成功响应的操作肯定应该被定义成一个回调方法，当跟随者节点的日志落盘成功之后，就执行这个回调方法**。这一切是不是都很熟悉？前几章我为大家实现日志持久化功能的时候，也为大家介绍了一些回调方法被回调的时机，其中就有一个回调方法会在日志持久化成功之后被回调。大家可以看看前面的文章或代码简单回顾一下。好了，接下来就让我来为大家展示一下重构好的 handleAppendEntriesRequest 方法吧。请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {



    //其他内容省略


    //当前节点接收到领导者发送过来的消息时，不管消息是日志复制还是心跳消息，还是探针消息，都会调用该方法进行处理
    //换句话说，其实领导者要发送的日志复制消息，以及心跳消息探针消息，使用的都是同一个RPC请求，也就是该方法中的第一个参数对象
    @Override
    public Message handleAppendEntriesRequest(final RpcRequests.AppendEntriesRequest request, final RpcRequestClosure done) {

        boolean doUnlock = true;
        this.writeLock.lock();
         //从请求中获得领导者传送过来的日志的条数
        final int entriesCount = request.getEntriesCount();

        try {


            //部分内容省略

            //获得请求中的前一条日志索引
            final long prevLogIndex = request.getPrevLogIndex();
            //获得领导者节点的前一条日志的任期
            final long prevLogTerm = request.getPrevLogTerm();


            

            //如果请求中的日志条目为0，说明当前的请求是心跳或者是探针消息
            if (entriesCount == 0) {
                //在处理探针消息的逻辑中也删除了一些和日志有干的代码，后面会加上
                //创建响应对象
                final RpcRequests.AppendEntriesResponse.Builder respBuilder = RpcRequests.AppendEntriesResponse.newBuilder()
                        .setSuccess(true)
                        .setTerm(this.currTerm)
                        .setLastLogIndex(this.logManager.getLastLogIndex());
                doUnlock = false;
                this.writeLock.unlock();
                //直接返回心跳或者探针请求的响应
                return respBuilder.build();
            }



            //下面把处理日志操作的逻辑全部加上了
            //快速检查日志组件是否超负荷了，所谓超负荷就是判断日志管理器中的环形数组是否还有足够的空间放下这一批要存放的日志
            //方法参数是1，意味着只要环形数组还有一个位置能让生产者发布数据，就不算超负荷
            //这个方法其实就是判断定义在LogManagerImpl中的disruptor的环形数组队列中是否还能放的下一个数据
            if (!this.logManager.hasAvailableCapacityToAppendEntries(1)) {
                //日志组件超负荷则记录日志
                LOG.warn("Node {} received AppendEntriesRequest but log manager is busy.", getNodeId());
                //返回响应，响应的错误状态码为忙碌
                return RpcFactoryHelper
                        .responseFactory()
                        .newResponse(RpcRequests.AppendEntriesResponse.getDefaultInstance(), RaftError.EBUSY,
                                "Node %s:%s log manager is busy.", this.groupId, this.serverId);
            }


            
            //走到这里意味着日志组件可以正常工作，就下来就应该把从领导者传输过来的日志在跟随者进行落盘了
            //得到从领导者传过来的前一条日志的索引
            long index = prevLogIndex;
            
            //创建存放日志条目对象的集合，这个集合会交给日志管理器组件，日志管理器组件会将这个集合中的日志
            //进行落盘
            final List<LogEntry> entries = new ArrayList<>(entriesCount);
            
            //这里的逻辑和跟随者发送日志时，对日志做的包装的逻辑类似，只不过这里是将日志从包装中取出来
            ByteBuffer allData = null;
            //把日志从请求中取出来，然后全部交给allData对象
            if (request.hasData()) {
                allData = request.getData().asReadOnlyByteBuffer();
            }
            
            
            //获取这一次发送过来的这批日志的元数据的集合，这个应该还有印象吧
            final List<RaftOutter.EntryMeta> entriesList = request.getEntriesList();
            
            
            //下面就是具体从请求中解析出每一条日志的逻辑
            for (int i = 0; i < entriesCount; i++) {
                //这里就得到了要解析的日志的每一条索引，index本来是领导者传递过来的前一条日志的索引
                //自增之后，就可以当作传输过来的第一条日志的索引了
                index++;
                //从元数据集合中获取当前正在遍历的这个日志条目的元数据
                final RaftOutter.EntryMeta entry = entriesList.get(i);
                //这里就是根据元数据和上面的allData对象，将索引为index的日志条目对象完整解析出来了
                final LogEntry logEntry = logEntryFromMeta(index, allData, entry);
                //判断刚才解析出来的日志条目对象是否为空
                if (logEntry != null) {
                    //如果不为空，则在这里根据校验和判断一下日志是否有损坏
                    if (this.raftOptions.isEnableLogEntryChecksum() && logEntry.isCorrupted()) {
                        //如果损坏了，就计算一下现在的校验和
                        long realChecksum = logEntry.checksum();
                        //记录错误日志
                        LOG.error("Corrupted log entry received from leader, index={}, term={}, expectedChecksum={}, realChecksum={}",
                                logEntry.getId().getIndex(), logEntry.getId().getTerm(), logEntry.getChecksum(),
                                realChecksum);
                        //给领导者返回影响，响应的错误状态码为无效参数
                        return RpcFactoryHelper.responseFactory().newResponse(RpcRequests.AppendEntriesResponse.getDefaultInstance(), RaftError.EINVAL,
                                        "The log entry is corrupted, index=%d, term=%d, expectedChecksum=%d, realChecksum=%d",
                                        logEntry.getId().getIndex(), logEntry.getId().getTerm(), logEntry.getChecksum(),
                                        realChecksum);
                    }
                    //把解析出来的日志条目放到日志条目集合中
                    entries.add(logEntry);
                }
            }


            
            //接下来就是将日志落盘的具体逻辑了，但是，这里请大家注意一下，现在是跟随者节点要把日志落盘，日志落盘成功后，肯定要向领导者回复落盘成功的消息
            //所谓落盘成功其实就是返回一个true，也就代表着跟随者节点给这条日志投票了，这个逻辑就在下面创建的封装回调方法的对象中实现了
            //当然，还有一个最重要的逻辑，那就是跟随者根据领导者传递过来的committedIndex，也就是已经可以应用到状态机的日志的索引，将对应日志应用到状态机
            //这个逻辑也在FollowerStableClosure封装的回调方法中实现了，但是在第五版本代码中还用不到状态机，所以大家忽略committedIndex这个逻辑即可
            //FollowerStableClosure是NodeImpl中的一个内部类，代码就不展示了，大家去第五版本代码中查看就行，或者等我讲解第六版本代码的时候再看
            //因为回复日志请求响应的逻辑还和Pipeline模式有关，现在看还有点早
            final FollowerStableClosure closure = new FollowerStableClosure(request, RpcRequests.AppendEntriesResponse.newBuilder()
                    .setTerm(this.currTerm), this, done, this.currTerm);
            //在这里把存放了日志条目的集合交给日志管理器去落盘了
            this.logManager.appendEntries(entries, closure);
           

            //这里的逻辑暂且不用考虑，下一章会详细讲解
            //这里返回null，就和AppendEntriesRequestProcessor处理器的processRequest0方法接应上了
            //因为日志落盘是一个复杂的操作，肯定不能同步等待落盘成功的响应，所以这里直接返回null，让日志落盘异步执行
            //然后异步返回响应，返回响应的逻辑就在上面创建的FollowerStableClosure对象封装的回调方法中
            return null;
            
        }finally {
          if (doUnlock) {
              this.writeLock.unlock();
          }
        }
    }




    //解析出一个完整日志条目的方法
    private LogEntry logEntryFromMeta(final long index, final ByteBuffer allData, final RaftOutter.EntryMeta entry) {
        //从存放了日志元信息的对象中判断日志条目类型，如果类型不是未知的，就继续向下执行
        if (entry.getType() != EnumOutter.EntryType.ENTRY_TYPE_UNKNOWN) {
            //创建一个日志条目对象，用于存放下面解析出来的信息
            final LogEntry logEntry = new LogEntry();
            //创建日志ID，日志的索引和日志任期都已经可以获得了
            //然后把日志ID设置到刚刚创建出来的logEntry对象中
            logEntry.setId(new LogId(index, entry.getTerm()));
            //设置日志类型
            logEntry.setType(entry.getType());
            //设置日志校验和
            if (entry.hasChecksum()) {
                logEntry.setChecksum(entry.getChecksum());
            }//从元数据对象中获取日志有效信息的字节长度
            final long dataLen = entry.getDataLen();
            //如果字节长度大于0
            if (dataLen > 0) {
                //从allData中获得真正的日志有效信息
                final byte[] bs = new byte[(int) dataLen];
                assert allData != null;
                allData.get(bs, 0, bs.length);
                //把日志有效信息，也就是真正的业务执行设置到日志条目对象中
                logEntry.setData(ByteBuffer.wrap(bs));
            }//接下来就是判断有没有集群节点信息了
            if (entry.getPeersCount() > 0) {
                //如果有集群节点信息，那就判断日志类型是不是配置变更类型
                //如果不是则报错，由此可见，只有配置变更类型的日志才会有集群节点信息
                if (entry.getType() != EnumOutter.EntryType.ENTRY_TYPE_CONFIGURATION) {
                    throw new IllegalStateException(
                            "Invalid log entry that contains peers but is not ENTRY_TYPE_CONFIGURATION type: "
                                    + entry.getType());
                }//如果日志类型就是集群配置变更类型，就在下面这个方法中把集群节点信息填充到日志条目对象中
                fillLogEntryPeers(entry, logEntry);
            } else if (entry.getType() == EnumOutter.EntryType.ENTRY_TYPE_CONFIGURATION) {
                //走到这里意味着当前日志条目类型为配置变更类型，但是该日志中并没有集群节点信息，所以也会直接报错
                throw new IllegalStateException(
                        "Invalid log entry that contains zero peers but is ENTRY_TYPE_CONFIGURATION type");
            }
            return logEntry;
        }
        return null;
    }




    //这个方法就是把集群节点信息，从元数据对象中解析出来，然后设置到日志条目对象中
    //逻辑非常简单，就不再添加注释了
    private void fillLogEntryPeers(final RaftOutter.EntryMeta entry, final LogEntry logEntry) {
        // TODO refactor
        if (entry.getPeersCount() > 0) {
            final List<PeerId> peers = new ArrayList<>(entry.getPeersCount());
            for (final String peerStr : entry.getPeersList()) {
                final PeerId peer = new PeerId();
                peer.parse(peerStr);
                peers.add(peer);
            }
            logEntry.setPeers(peers);
        }
        if (entry.getOldPeersCount() > 0) {
            final List<PeerId> oldPeers = new ArrayList<>(entry.getOldPeersCount());
            for (final String peerStr : entry.getOldPeersList()) {
                final PeerId peer = new PeerId();
                peer.parse(peerStr);
                oldPeers.add(peer);
            }
            logEntry.setOldPeers(oldPeers);
        }
        if (entry.getLearnersCount() > 0) {
            final List<PeerId> peers = new ArrayList<>(entry.getLearnersCount());
            for (final String peerStr : entry.getLearnersList()) {
                final PeerId peer = new PeerId();
                peer.parse(peerStr);
                peers.add(peer);
            }
            logEntry.setLearners(peers);
        }
        if (entry.getOldLearnersCount() > 0) {
            final List<PeerId> peers = new ArrayList<>(entry.getOldLearnersCount());
            for (final String peerStr : entry.getOldLearnersList()) {
                final PeerId peer = new PeerId();
                peer.parse(peerStr);
                peers.add(peer);
            }
            logEntry.setOldLearners(peers);
        }
    }

    //其他内容省略
}
```

好了，朋友们，到此为止我就把跟随者节点处理日志传输请求的功能也实现了，本章的主要内容差不多也结束了。按照文章一开始的规划，接下来我就要给大家实现上一章遗漏的一个小功能。但是我不得不说一句，目前我们实现的跟随者节点处理领导者传输日志请求的功能并不完美，确切的说，存在着很大的问题。

## 解决跟随者和领导者日志冲突问题

我先为大家把存在的问题点明，**这个问题就是领导者和跟随者节点日志冲突问题**。我相信大家肯定会对此感到不解，为什么还会产生日志冲突呢？之前在实现探针请求功能的时候，不是已经把日志冲突解决了吗？探针请求已经让领导者成功定位了应该发送给跟随者节点的下一条日志索引，之后就一直顺利发送即可，为什么跟随者和领导者还会出现日志冲突的情况呢？请大家先别急，首先让我来为大家解释一下，我在这里说的日志冲突，是一个很广义的概念，包含了多种情况。实际上，我把 raft 集群中可能出现的日志冲突分为了四种情况。

**1** **日志出现了遗漏，比如说跟随者节点要接收的下一条日志索引为 5，领导者发送过来的下一批日志的起始索引是从 10 开始的，这就是典型的日志丢失、遗漏的情况。当然，这种情况可能很快就被会处理，因为领导者每一次向跟随者传输日志的时候，都会通过上一次传输给跟随者节点的日志索引判断日志传输是否有遗漏**。就像下面这样(下面代码块中的第 22 行添加了一些新的注释，一定要看一下)。

```
public class NodeImpl implements Node, RaftServerService {



    //其他内容省略


    //当前节点接收到领导者发送过来的消息时，不管消息是日志复制还是心跳消息，还是探针消息，都会调用该方法进行处理
    //换句话说，其实领导者要发送的日志复制消息，以及心跳消息探针消息，使用的都是同一个RPC请求，也就是该方法中的第一个参数对象
    @Override
    public Message handleAppendEntriesRequest(final RpcRequests.AppendEntriesRequest request, final RpcRequestClosure done) {


        //省略其他内容


         final long prevLogIndex = request.getPrevLogIndex();
            //获得领导者节点的前一条日志的任期
            final long prevLogTerm = request.getPrevLogTerm();
            //获得当前跟随者节点对应日志的任期，这里是根据领导者请求中的前一条日志索引获得的
            final long localPrevLogTerm = this.logManager.getTerm(prevLogIndex);
            //注意，这里我要再解释一下，当前这个校验前一条日志的逻辑，其实只能判断日志有没有遗漏
            //并不能处理日志重复传输的情况，比如说领导者向跟随者节点传输了索引1-5的日志
            //跟随者节点已经把日志落盘成功了，但是回复的成功响应在网络中阻塞了，所以领导者节点
            //以为日志没有复制成功，就把索引1-5的日志重新传给了跟随者节点，这时候判断领导者请求中的前一条日志索引和任期
            //是否在跟随者节点存在，肯定是能判断成功的。但这批日志是重复传输过来的，不用再持久化了
            //所以跟随者节点要处理一下日志重复传输的情况，具体怎么处理文章中马上就会讲到
            //不相等则返回失败响应
            if (localPrevLogTerm != prevLogTerm) {
                final long lastLogIndex = this.logManager.getLastLogIndex();
                LOG.warn(
                        "Node {} reject term_unmatched AppendEntriesRequest from {}, term={}, prevLogIndex={}, prevLogTerm={}, localPrevLogTerm={}, lastLogIndex={}, entriesSize={}.",
                        getNodeId(), request.getServerId(), request.getTerm(), prevLogIndex, prevLogTerm, localPrevLogTerm,
                        lastLogIndex, entriesCount);
                return RpcRequests.AppendEntriesResponse.newBuilder()
                        .setSuccess(false)
                        .setTerm(this.currTerm)
                        .setLastLogIndex(lastLogIndex)
                        .build();
            }



        //省略其他内容
        
    }
```

**2 日志已经过期了，在分析日志过期情况之前，我需要先为大家讲解两种情况。第一就是日志应用，这个和第七版本代码的状态机有关，是后面章节的知识，所以我不会在本章讲解得十分详细，大家知道原理即可。当一条日志被集群多半数节点持久化成功之后，肯定就会被应用到状态机上，也就是我们说的提交。状态机其实也维护着一个索引，那就是最新被应用的日志的索引。如果领导者传输过来 10 条日志，这 10 条日志的最后一条日志索引为 15，但是现在跟随者节点状态机已经应用到索引为 20 的日志了，这种情况下，我们就可以说传输过来的这批日志过期了。那既然是过期日志，为什么领导者还会传输过来呢？这就是我要为大家讲解的第二种情况，当跟随者节点接收到了领导者的日志传输请求后，持久化日志成功了需要回复成功响应，但可能因为网络等原因，导致响应超时，领导者迟迟接收不到成功响应，就会认为之前传输的日志失败了，于是就会重新传输。但实际上这个跟随者节点早就把日志持久化成功了，并且一直在应用已经提交了的日志。这就是日志过期的情况。**

**3 日志重复传输了，这种情况其实可以和第二种情况归类到一起，但我还是想再重复一下。所谓日志重复传输其实就是同一批日志又一次传输过来了。比如跟随者节点刚刚接收到索引为 5-10 的日志，并且也持久化成功了，只不过回复的成功响应没有及时被领导者接收，所以领导者以为 1-10 索引的日志没有传输成功，于是把日志又重新传输了一遍。这就是日志重复传输的情况。**

第 4 种情况先不急着介绍，让我们先来思考思考，假如领导者和跟随者出现日志冲突时，是以上三种情况中的一种，应该怎么处理这种情况呢？逻辑其实很简单，**如果是第一种情况，也就是出现了日志遗漏的情况，那么跟随者节点直接回复给领导者一个日志传输的错误响应，领导者收到响应后就会重新发送探针请求定位日志，然后再根据最新定位的日志索引，开始向跟随者节点传输日志。如果是第 2、3 种情况，也就是日志重复发送的情况，这就更简单了，直接忽略这批日志即可**。当然，**针对第 2 种情况，其实可以给领导者回复一个成功响应，毕竟这批日志都已经应用了**。这三种情况的处理逻辑看起来非常容易理解。但是第 4 种日志冲突的情况就稍微麻烦一点了。

**4 日志重叠了，我记得我为大家实现探针请求功能的时候，特意举了个例子解释了一种跟随者节点和领导者节点发生日志冲突的情况，也就是跟随者节点最新的日志索引可能比探针请求中的大。**不知道大家对此还有印象吗，如果没有印象，可以看看下面的两张图片，我把当时分析的内容的截图放在下面供大家回顾。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1709295576464-aa2488bd-1c94-4e2f-a23a-5db8856d855e.png)

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1709295595491-0fa25fd8-cc02-4e19-a488-8e2ba575df57.png)

出现上面这种情况之后，领导者就需要将发送给跟随者节点的 prevLogIndex 减 1，然后再次发送探针消息给跟随者节点。如果匹配仍然失败，就再递减 prevLogIndex，直到能和跟随者节点的日志匹配成功。还是用上面图片中的例子来讲解，当领导者的探针请求终于定位日志成功之后，领导者知道应该从索引为 1 的日志开始向跟随者节点传输日志，领导者一共有 2 条日志，所以会把这 2 条日志都传输给跟随着节点。当跟随者接收到领导者传输过来的日志之后要怎么做呢？请大家注意，虽然跟随者节点从领导者那里接收到的日志都是最新的日志了，但是此刻跟随者节点内部还有 5 条旧日志，现在跟随者节点要把最新的 2 条日志持久化，这时候就应该先执行一个操作，**那就是把本地的旧日志全清除了。因为日志持久化可没有新的覆盖旧的这种说法，如果不把旧的日志清除，直接将新的日志进行持久化，就会出现日志索引错乱的情况。所以要想解决第 4 种日志冲突的情况，跟随者节点还应该根据请求中的日志索引，一条一条和本地日志检验。在当前这个例子中，领导者发送给跟随者节点的第一条日志的索引为 1，任期为 2，所以就从索引为 1 的日志开始，先让跟随着节点获得本地索引为 1 的日志的任期，发现本地索引为 1 的日志任期为 1。这就意味着从第一条日志就和领导者传输过来的日志不匹配，那么跟随者节点就可以直接从索引 1 开始，把包括索引 1 在内的和之后的日志全都清除了**。这就是我要给大家讲解的第 4 种日志冲突情况。

这四种情况讲解完毕之后，我还想再强调一下，**在跟随者节点中，日志请求是被单线程批处理器按照先后顺序处理的，处理完一批日志之后，就会立刻更新和日志组件有关的所有属性的值，所以跟随者节点在处理日志请求或者是日志冲突时，总是能以本地最新的状态来进行相关的检验和判断。**这一点大家可以再体会体会。

好了，到此为止，四种日志冲突的情况已经讲解完毕了，是时候在代码层面上实现解决日志冲突的功能了。解决日志冲突的方法我刚才也为大家分析了：

**如果是日志遗漏情况，跟随者节点直接返回领导者一个错误响应。**

**如果是日志过期情况，直接忽略这批日志，并给领导者回复一个成功响应。**

**如果是日志重复情况，直接忽略这批日志即可。**

**如果是日志出现重叠情况，就找出重叠的终点，然后将终点之后的日志全部清除，再持久化新接收到的日志。比如说跟随者要接收的下一条日志是 10，领导者发送过来的这批日志的第一条日志索引为 8，这就意味着跟随者节点的 8、9、10 索引的日志都是过期日志。重叠的终点就是索引为 7 的日志，直接把跟随者索引为 7 之后的日志全都清除即可。**

接下来，大家就带着我为大家总结的 4 点解决方案来阅读下面的代码吧。当然，在这之前我还是要跟大家讲一下，**解决跟随者和领导者日志冲突的功能在 checkAndResolveConflict 方法中实现了**。这个 checkAndResolveConflict 方法在第 13 章出现过一次。当时我在代码注释中跟大家简单解释了一下，告诉大家处理跟随者节点和领导者节点日志冲突的逻辑就定义在 checkAndResolveConflict 方法中。这个 checkAndResolveConflict 方法会在 LogManagerImpl 的 appendEntries 方法中被调用。接下来，就请大家看看这个 checkAndResolveConflict 方法重构之后的样子吧。请看下面代码块。

```
public class LogManagerImpl implements LogManager {


    //省略其他内容


    //将一批日志持久化的方法
    @Override
    public void appendEntries(final List<LogEntry> entries, final StableClosure done) {



        //省略其他内容

         try {
            //走到这里会有一个判断，因为当前方法不管是领导者还是跟随者都适用，当跟随者接收到来自领导者的日志后
            //也会调用日志管理组件将日志落盘，所以这里会判断一下，如果这一批日志不为空，并且所有日志的索引都为0
            //那就意味着这批日志是业务日志，并且是领导者处理的。如果是跟随者接收到的日志，索引肯定已经被领导者填充了
            //所以索引肯定是有值的，具体的判断方法就是checkAndResolveConflict方法
            if (!entries.isEmpty() && !checkAndResolveConflict(entries, done, this.writeLock)) {
                //如果checkAndResolveConflict方法返回false，就直接退出当前方法
                entries.clear();
                return;
            }
         
         
         }


        //省略其他内容
        
    }
    
    
    





    //这个方法就是校验判断当前日志是被领导者还是跟随者处理的方法,在第五版本代码中，终于重构完整了
    @SuppressWarnings("NonAtomicOperationOnVolatileField")
    private boolean checkAndResolveConflict(final List<LogEntry> entries, final StableClosure done, final Lock lock) {

        //取出本批次日志中的第一条日志
        final LogEntry firstLogEntry = ArrayDeque.peekFirst(entries);
        
        //判断当前节点是否为领导者，就是根据第一条日志的索引是否为0来判断，如果是0，说明日志是领导者内部创建的
        //如果不是0，说明是从跟随着节点接收过来的，接收过来的日志的索引会在NodeImpl对象的handleAppendEntriesRequest方法中被赋值
        if (firstLogEntry.getId().getIndex() == 0) {
            //判断是否为领导者节点的日志处理逻辑，如果是领导者，就给每一条日志设置索引
            for (int i = 0; i < entries.size(); i++) {
                //一直让最后一条索引递增即可，如果是整个集群中的第一条日志，这时候lastLogIndex默认值为0
                //所以第一条日志的索引为1
                entries.get(i).getId().setIndex(++this.lastLogIndex);
            }
            return true;
        }



            
        //这里就是跟随者节点处理从领导者传输过来的日志的逻辑，比如判断传输过来的日志和本地有没有冲突，是需要覆盖还是需要截断等等
        else {
            

            //日志遗漏的情况
            
            //走到这里就意味着是跟随者节点在处理从领导者传输过来的日志
            //首先判断一下传输过来的这批日志中的第一条日志的索引是否大于当前跟随者节点想要接收的下一条日志的索引
            //不管是什么原因，总之跟随者和领导者的日志并不同步，中间缺失了一部分日志
            //如果没有出错，这两个值应该是想等着，出错了就返回给领导者错误响应
            if (firstLogEntry.getId().getIndex() > this.lastLogIndex + 1) {
                //在另一个线程中执行done包装的回调方法，其实就是给领导者回复错误的响应
                ThreadPoolsFactory.runClosureInThread(this.groupId, done, new Status(RaftError.EINVAL,
                        "There's gap between first_index=%d and last_log_index=%d", firstLogEntry.getId().getIndex(),
                        this.lastLogIndex));
                return false;
            }
            
            



            //日志过期的情况

            
            //这里先简单引入一下第七版本代码的知识，就是下面这个appliedId。这个成员变量代表着已经应用到状态机的最新的日志
            //在第七版本代码中，引入状态机之后，会看到状态机组件是怎么给这个成员变量赋值的
            //在这一版本还无法给这个成员变量赋值，但是大家直到他代表什么就行了
            //这里是得到当前跟随者节点已经应用到状态机中的最新日志的索引
            final long appliedIndex = this.appliedId.getIndex();
            //得到从领导者传递过来的最后一条日志的索引
            final LogEntry lastLogEntry = ArrayDeque.peekLast(entries);
            //然后判断这个索引和从领导者传递过来的这批日志中的第一条日志的索引的大小关系
            //如果从领导者传递过来的最后一条日志的索引小于当前跟随者节点已经应用了的日志
            //这就说明现在传递过来的这批日志已经过期了，因为在pipeline模式下，在某些情况下，领导者是有可能会向
            //跟随者节点发送重复的日志的，这个在第六版本会为大家详细讲解，总之，只要发现日志过期了，就不必再处理了
            if (lastLogEntry.getId().getIndex() <= appliedIndex) {
                LOG.warn(
                        "Received entries of which the lastLog={} is not greater than appliedIndex={}, return immediately with nothing changed.",
                        lastLogEntry.getId().getIndex(), appliedIndex);
                //这里给领导者回复响应，注意这里回复的是成功的响应，因为日志早就被跟随者处理了
                //当然可以回复成功的响应了
                ThreadPoolsFactory.runClosureInThread(this.groupId, done);
                return false;
            }
            
            
            


            //日志正常接收的情况
            
            //走到这里意味着从领导者传输过来的日志正好和跟随者当前要接收的日志对接上了
            if (firstLogEntry.getId().getIndex() == this.lastLogIndex + 1) {
                //设置当前跟随者节点的最后一条日志的索引，然后就直接返回true了，不必走下面的这个else分支了
                //之后在外层方法将日志放到内存，然后落盘就可以了
                this.lastLogIndex = lastLogEntry.getId().getIndex();
            } 
            
            
            
            
            
            
            //下面就是日志重叠的情况
            else {
                //走到这里有些棘手了，现在的情况是从领导者发送过来的这批日志中，第一条日志的索引是小于当前跟随者节点的最后一条日志索引加1
                //但是呢，又不等于当前跟随者节点要接收的下一条日志的索引，比如说当前跟随者节点的最后一条日志是10，那么下一条要接收的日志索引就是11
                //从领导者发送过来的这批日志的第一条日志的索引是8，这样一来，第一条日志的索引小于跟随者要接收的日志的索引，但是又不等于跟随者就要接收的这条日志
                //显然是跟随者和领导者的日志出现了重叠部分，出现这种情况的原因很简单，就是集群中的领导者发生过变更，旧的领导者向当前跟随者节点发送过日志
                //但还没来得及提交就宕机了，新的领导者被选举出来之后，产生了新的日志，肯定要覆盖其他跟随者节点的旧日志，这时候只有一种解决方法
                //那就是找到领导者和跟随者日志重叠的终点，在我举的例子中，就是跟随者的索引为7的日志，然后将当前跟随者节点的这个索引之后的日志全部截掉
                //这里大家可能会有些困惑，觉得为什么jraft框架不能把日志先放到内存中，等日志提交成功了再刷新到硬盘中呢？
                //这里我想简单说两句，这个jraft框架本身就是用来存放数据，使各个节点的数据保持一致的，和数据打交道的框架就要尽最大限度得保证数据丢失的风险
                //所以jraft框架会先把日志落盘，不管发生什么情况，总之要先把数据落盘，这样能尽量减少数据丢失的风险，尽管后面可能会有截掉的麻烦
                //下面就是找到跟随者和领导者日志冲突的交界点
                int conflictingIndex = 0;
                //在循环中遍历领导者传来的每一条日志，然后判断日志的索引
                for (; conflictingIndex < entries.size(); conflictingIndex++) {
                    //entries.get(conflictingIndex).getId().getIndex()的作用很明确，就是得到从领导者传来的日志的索引
                    //unsafeGetTerm方法就会根据领导者日志的索引去本地跟随者的日志组件中查看对应日志的任期
                    //entries.get(conflictingIndex).getId().getTerm()得到的就是领导者发送过来的日志任期
                    //然后将这两个任期做对比。这里为什么非要使用这个循环呢？找一遍的目的就是为了判断当前日志是不是重复传输了
                    if (unsafeGetTerm(entries.get(conflictingIndex).getId().getIndex()) != entries
                            .get(conflictingIndex).getId().getTerm()) {
                        break;
                    }
                }//走到这里就找到了要从哪个位置删除跟随者的日志，并且判断重叠的索引不等于日志条目
                //比如说领导者传递过来了索引为1-10的10条日志，并且这是第二次把这批日志传递给跟随者了，跟随者已经将这批日志落盘了，但是还没有应用呢
                //又一次收到这批日志之后，逻辑同样可以进行到这里，并且在走完了上面整个循环，也没找到两个相同索引的日志的任期不相等
                //最后conflictingIndex自增之后等于entries.size()了退出循环，所以，如果conflictingIndex = entries.size()的时候
                //就说明日志重复发送了，不用处理这批日志即可，如果不相等，才是日志有冲突的情况，这时候就要将跟随者节点的一些日志截取掉
                if (conflictingIndex != entries.size()) {
                    if (entries.get(conflictingIndex).getId().getIndex() <= this.lastLogIndex) {
                        //移除跟随者节点和领导者重叠的日志，可以看到，如果重叠位置是8，那就从8-1，也就是从7开始，把索引为7之后的日志全部移除了
                        unsafeTruncateSuffix(entries.get(conflictingIndex).getId().getIndex() - 1, lock);
                    }//设置跟随者最后一条日志索引
                    this.lastLogIndex = lastLogEntry.getId().getIndex();
                }
                
                
                
                
                
                


                //日志重复的情况
                
                //走到这里就意味着conflictingIndex = entries.size()，不用处理这批日志
                if (conflictingIndex > 0) {
                    //这里直接把重复的日志从集合中删除即可
                    entries.subList(0, conflictingIndex).clear();
                }
            }
            return true;
        }
    }










    //从指定位置开始移除本地日志条目
    private void unsafeTruncateSuffix(final long lastIndexKept, final Lock lock) {
        //判断要移除的日志是否已经被提交了
        if (lastIndexKept < this.appliedId.getIndex()) {
            LOG.error("FATAL ERROR: Can't truncate logs before appliedId={}, lastIndexKept={}", this.appliedId,
                    lastIndexKept);
            return;
        }//先从内存缓存日志的组件中删除日志
        this.logsInMemory.removeFromLastWhen(entry -> entry.getId().getIndex() > lastIndexKept);
        this.lastLogIndex = lastIndexKept;
        final long lastTermKept = unsafeGetTerm(lastIndexKept);
        Requires.requireTrue(this.lastLogIndex == 0 || lastTermKept != 0);
        LOG.debug("Truncate suffix :{}", lastIndexKept);
        //从配置管理器中尝试删除日志
        this.configManager.truncateSuffix(lastIndexKept);
        lock.unlock();
        //这里也能看出来对锁粒度的控制，不需要锁的时候就直接释放锁
        final TruncateSuffixClosure c = new TruncateSuffixClosure(lastIndexKept, lastTermKept);
        //提交事件给环形数组，在这个里面，会执行真正从数据库组件中删除日志的操作，并且是被disruptor框架异步执行的
        //并且这个任务先于本批次日志落盘的任务提交给环形数组队列，也就会先被单线程批处理器执行，所以当本批日志落盘的时候
        //有冲突的日志已经从内存和数据库中都移除了
        offerEvent(c, EventType.TRUNCATE_SUFFIX);
        //需要的时候再获取，这里再次获取是因为外层函数还需要这个锁，最终释放，会在外层方法释放
        lock.lock();
    }












    //省略其他内容
    
}
```

很好，到此为止，我就为大家把解决日志冲突的功能也实现了。逻辑还是有些复杂的，大家一定要结合代码多思考思考。写了这么多，我确实也很累。一给窝里giaogiao！！！

## 实现 Replicator 复制器对象继续传输日志功能

本章的最后一小节，当然是把上一章遗漏的功能实现了。其实写到这里，本章的内容已经够多了，至少要消化的知识确实挺多的了。相对于上一小节的知识，Replicator 复制器对象继续传输日志的功能实现起来就容易多了，所以我就只展示一下关键的方法，就不再讲解得那么细致了，大家可以直接去第五版本代码中结合注释查看功能的实现。这个功能的逻辑非常简单，我想结合注释，大家肯定可以看明白。

这个功能的实现分为两部分，**一是当领导者内部没有日志让 Replicator 复制器对象继续发送了，这个时候复制器对象应该执行一些操作。二是领导者内部的日志都会通过 LogManagerImpl 日志管理器持久化到本地，但是在持久化到本地之前，会把日志先存放到日志缓存池中，所以当日志缓存池中有了新的日志，LogManagerImpl 对象也应该执行一些操作**。这时候这个逻辑就很明显了吧？并且这个逻辑对我们来说应该也是很常规了，一个回调方法就可以实现 Replicator 复制器继续传输日志功能。

**当领导者内部没有日志的时候，可以让 Replicator 复制器对象向 LogManagerImpl 提交一个回调方法，这个方法中定义这继续发送日志给跟随者的功能；当 LogManagerImpl 持有的日志缓存池中有了新的日志，LogManagerImpl 对象就立刻执行 Replicator 提交的回调方法，这样一来，Replicator 复制器对象就可以继续向跟随者节点发送日志了**。

Replicator 复制器对象向 LogManagerImpl 中注册回调方法的逻辑就在 Replicator 的 sendEntries 方法中。因为 sendEntries 方法就是用来发送日志给跟随者节点的，当领导者内部没有日治之后，可以直接在 sendEntries 方法中向 LogManagerImpl 中注册回调方法。具体逻辑请看下面代码块。

```
public class Replicator implements ThreadId.OnError {


    //批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志
    private boolean sendEntries(final long nextSendingIndex) {

        //其他内容省略


         try {//开始在一个循环中封装要发送的日志条目了
            for (int i = 0; i < maxEntriesSize; i++) {
                //创建封装日志元信息的对象，注意，每一个要发送的日志条目对象都对应着一个元信息对象
                //这些日志条目对象的元信息对象会构成一个元信息集合，最后会被放到AppendEntriesRequest请求中
                //发送给跟随者
                final RaftOutter.EntryMeta.Builder emb = RaftOutter.EntryMeta.newBuilder();
                //准备发送日志，在prepareEntry方法中，会把日志条目的元信息和日志条目的data分开存放
                //元信息存放到emb对象中，data存放到byteBufList集合的数组中
                if (!prepareEntry(nextSendingIndex, i, emb, byteBufList)) {
                    //准备失败就意味着没有日志可以发送了，直接退出循环即可
                    break;
                }//在这里把本批次发送的日志条目的元信息集合设置到AppendEntriesRequest请求中
                rb.addEntries(emb.build());
            }
            //这里也是自己添加的，重置下一条要发送的日志的索引，主要是为了配合getNextSendIndex方法的使用
            //第7个版本就不会有这行代码了， rb.getEntriesCount()是本批次要发送的日志条目
            this.nextIndex += rb.getEntriesCount();
            
             
             //下面就是向LogManagerImpl注册回调方法的逻辑
             
             //如果AppendEntriesRequest对象发现在经历了上面的填充日志信息的循环后
            //自己根本没有获得任何日志条目，就意味着领导者目前根本没哟日志可以发送给跟随者
            if (rb.getEntriesCount() == 0) {
                //这里本来有一个判断是否安装快照的操作，但是被我省略了
                //如果领导者内部真的没有日志，这时候就可以让复制器向日志管理器组件提交一个回调方法
                //等有日志了就通知复制器对象继续发送日志，具体的逻辑就在下面的方法中
                waitMoreEntries(nextSendingIndex);
                //因为没有日志可发送，这里就直接退出当前方法了
                return false;
            }
         
         
         }



        //其他内容省略
        
    }    



    //当前的领导者节点中暂时没有日志可以发送给跟随者节点时，复制器对象会向日志管理组件提交一个回调方法，当
    //日志管理器组件有日志落盘的时候，会通知复制器继续发送日志，这时候是直接从内存中获得日志的
    private void waitMoreEntries(final long nextWaitIndex) {
        try {
            LOG.debug("Node {} waits more entries", this.options.getNode().getNodeId());
            //如果等待ID大于0，意味着当前复制器对象已经向日志管理器提交过回调方法了
            if (this.waitId >= 0) {
                //直接退出即可
                return;
            } //向日志管理器提交一个回调方法，日志管理器的wait方法会返回一个waitID
             //这continueSending方法我就不再具体展示了，逻辑非常简单，大家直接去第五版本代码中查看即可
            this.waitId = this.options.getLogManager().wait(nextWaitIndex - 1,
                    (arg, errorCode) -> continueSending((ThreadId) arg, errorCode), this.id);
            //设置当前复制器的运行状态
            this.statInfo.runningState = RunningState.IDLE;
        } finally {
            unlockId();
        }
    }


    


    //其他内容省略
}
```

接下来就要实现当日主缓存池中有了新的日志之后，LogManagerImpl 执行回调方法的逻辑。具体实现请看下面代码块。

```
public class LogManagerImpl implements LogManager {


    //省略其他内容


    //将一批日志持久化的方法
    @Override
    public void appendEntries(final List<LogEntry> entries, final StableClosure done) {



        //省略其他内容

         try {
            //走到这里会有一个判断，因为当前方法不管是领导者还是跟随者都适用，当跟随者接收到来自领导者的日志后
            //也会调用日志管理组件将日志落盘，所以这里会判断一下，如果这一批日志不为空，并且所有日志的索引都为0
            //那就意味着这批日志是业务日志，并且是领导者处理的。如果是跟随者接收到的日志，索引肯定已经被领导者填充了
            //所以索引肯定是有值的，具体的判断方法就是checkAndResolveConflict方法
            if (!entries.isEmpty() && !checkAndResolveConflict(entries, done, this.writeLock)) {
                //如果checkAndResolveConflict方法返回false，就直接退出当前方法
                entries.clear();
                return;
            }


             if (!entries.isEmpty()) {
               
                //日志在正式落盘之前，会把日志向内存中存放一份，这样访问起来更快更方便
                //当日志成功提交后，内存中的日志就会被清除掉了
                this.logsInMemory.addAll(entries);
            }


            //这个逻辑是用来唤醒复制器的，复制器会一直将日志向跟随者传输，当领导者中没有
            //日志的时候，复制器就会停止发送日志，并且会注册一个回调函数监听领导者是不是有日志了，当有日志的时候就会通知复制器
            //复制器就会直接从刚才存放了日志的缓存组件logsInMemory中，把日志直接传输给跟随者
            //这wakeupAllWaiter方法我就不再具体展示了，逻辑非常简单，大家直接去第五版本代码中查看即可
            wakeupAllWaiter(this.writeLock);
         
         }


        //省略其他内容
        
    }



    //其他内容省略


}
```

到此为止，朋友们，我就把 Replicator 复制器对象继续向跟随者节点传输日志的功能实现了。最后这一小节讲的比较简单，实际上也没什么可分析的了，本身的逻辑就是这么简单，很常规的逻辑。这一章也就到此结束了，我得养精蓄锐，休息一天。因为接下来的几篇文章，Pipeline 模式重构日志传输模式，状态机应用日志会上点难度。也比较难写，我需要构思构思。好了诸位，我们下一章见！