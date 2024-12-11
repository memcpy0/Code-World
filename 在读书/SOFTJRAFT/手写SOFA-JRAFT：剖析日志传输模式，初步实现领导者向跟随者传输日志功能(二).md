大家好，在上一章我围绕着探针请求，实现了和探针请求相关的全部功能，也就意味着领导者向跟随者节点发送日志之前的日志定位功能已经实现了，接下来，就可以直接给各个跟随者节点发送日志了。发送日志的流程也很简单，众所周知，把大象放到冰箱里需要三个步骤，发送日志仅仅比它多一步而已。

1 把日志封装在一个请求中

2 把请求发送给跟随者节点

3 跟随者节点从请求中获得日志

4 跟随者将日志落盘即可

这一章我为大家实现的只是前两个步骤的功能，后两个步骤将在下一章为大家实现。当然，很多事情都是说起来简单，做起来难，就比如你问我怎么能让一个女孩跟你结婚，我会对你说很简单，就一步，让这个女孩爱上你就行，但让女孩爱上你并不是那么容易就实现的，大家明白这个意思了吧？领导者向跟随者节点发送日志的流程虽然很简单，但是仍然有一些细节需要我们仔细分析，梳理。就比如说，当领导者向跟随者节点发送日志的时候，应该按照什么方式发送呢？

## 剖析日志传输模式

经过前几章的学习，我们都知道当领导者在接收到客户端指令的时候，会调用领导者节点 NodeImpl 对象的 apply 方法，在该方法内把每一条客户端指令都包装成一个 LogEntry 日志条目对象。当领导者节点接收到很多客户端指令时，就会在内部创建多个 LogEntry 日志条目对象，也就意味着领导者需要把这些日志条目对象都传输给跟随者节点，让跟随着节点持久化，这样才能保证 raft 集群中各个节点数据一致的状态。那么，当领导者向跟随者节点传输日志的时候，应该怎么传输呢？

按照常规思路来说，肯定是领导者将日志一条一条地发送给跟随者节点，等跟随者节点回复了领导者上一条日志地成功响应后，领导者再发送下一条日志。如果一条日志发送失败，就重新发送这一条日志即可。这样就可以保证日志绝对是按照索引顺序发送，并且不可能日志被遗漏了。比如说，领导者内部有 5 条日志，当领导者向跟随者节点传输日志的时候，就会先发送索引为 1 的日志，等跟随者节点将索引为 1 的日志持久化成功了，回复给领导者节点要给成功响应，领导者再发送给跟随者节点索引为 2 的日志。假如领导者发送到索引为 4 的日志时，迟迟没有收到跟随者节点回复的成功响应，领导者就可以认为这条日志发送失败，然后再次重新发送索引为 4 的日志。这样一来，跟随者节点接收到的日志肯定是连贯的，不会出现 1、2、3 日志之后，直接就是索引为 5 的日志。我想这个发送日志的流程应该是很简单的，大家都可以理解。

但是，我想大家也都想到了，我们开发的这个 raft 共识算法框架，肯定不会使用上面那种方式来进行日志传输。原因很简单，因为领导者要通过网络才能向跟随者节点传输日志，**这也就意味着在这个过程中发生了网络 IO，而我们都知道，既然是 IO，都是比较耗时的。发送 5 条日志给跟随者节点，每次都发送一条，产生 5 次网络 IO的耗时，肯定比只发送一次，一次发送 5 条产生的网络 IO 耗时多**。所以，在一次传输日志的过程中批量发送日志，尽量减少网络 IO 的次数，可以显著提高程序的性能。

当然，要想显著提高程序的性能，除了可以使用批量传输日志这个方法，还有一个方法也可以达到更好的效果。**那就是领导者不必同步等待跟随者节点回复上一批日志持久化成功的响应，也就是说，领导者只管发送日志，只要领导者内部产生日志条目了，就直接发送给跟随者节点，至于跟随者节点有没有回复成功响应，领导者怎么判断日志是否传输成功了，可以使用另外的方法来检验**。不过这是后面章节的知识了，在这里我只是简单提一下， 后面用 Pipeline 模式重构日志传输时，大家就会看到领导者真正向跟随者传输日志的模式了。当然，这一章我们要实现的功能也不涉及跟随者节点接收领导者日志，并且持久化日志到本地，然后回复领导者响应的流程。这是下一章的内容，这一章，我们只专注于领导者把日志发送给跟随者节点的功能实现。

好了，稍微说了一点题外话，让我们回归正题吧。现在我们已经明确了领导者向跟随者传输日志的方式，那就是批量传输，一次发送多条日志到跟随者节点。**发送的手段我们也可以想到，肯定就是把要传输到跟随者节点的日志条目对象序列化一下，然后把序列化之后的数据封装到一个请求体中，再把请求体传输给跟随者节点**。接下来我们似乎就可以按照这个思路构思一下代码如何编写了。但还是不急，编写代码只是最后一步，在开始编写代码之前，让我们把所有的准备工作都完善了，这样在编写代码的时候会更顺利。

那我们现在要思考的是什么呢？我的思路也很常规，那就是领导者怎么获得要发送给跟随者节点的每一条日志？因为领导者要向跟随者发送日志了，而日志是日志管理器组件来管理的，也就是 LogManagerImpl 对象，这个大家应该都还记得吧？**当领导者将客户端的指令包装成一个个 LogEntry 对象后，会将这些 LogEntry 对象交给 LogManagerImpl 日志管理器，让日志管理器去将这些日志进一步落盘。所以，当领导者要想把日志发送给跟随者节点的时候，也应该通过 LogManagerImpl 日志管理器对象来获得要发送的每一条日志。就通过 LogManagerImpl 对象的 getEntry(long index) 方法，根据索引获得对应的日志即可**。

现在也明确了领导者怎么获得要发送给跟随者节点的日志，但是还有一个细节需要展开来讲解，也许大家还记得，在第 13 章的时候，我为我们的程序引入了一个非常重要的组件，**那就是日志缓存池，也就是 SegmentList 对象。当 LogManagerImpl 日志管理器要将一批日志持久化之前，会先将这批日志放到日志缓存池中**，这个逻辑大家应该还有印象。当时我并没有多么详细地解释这个 SegmentList 组建的作用，现在我倒是可以给大家具体讲解一下了，**这个 SegmentList 日志缓存池的作用就是把日志缓存在内存中，这样当领导者要向跟随者节点发送日志的时候，会先访问日志缓存池组件，如果直接在这个组件中获得了对应索引的日志，那么就不需要再访问日志存储引擎，也就是 RocksDBLogStorage 对象，因为一旦访问 RocksDBLogStorage 对象，实际上就相当于要从本地把索引对应的日志加载到内存中，然后再发送给跟随者节点，加载日志到内存是比较耗时的操作**。

在讲解了这个细节之后，现在我们应该都明白了，领导者通过索引获取对应日志肯定还是要调用 LogManagerImpl 的 getEntry(long index) 方法，但现在要先从 SegmentList 日志缓存池中获得对应的日志，所以 LogManagerImpl 的 getEntry(long index) 方法势必要经过一番重构，之前在这个方法内部，是直接通过 RocksDBLogStorage 日志存储器来获得对应索引的日志。现在不必这么麻烦了，因为 SegmentList 恰好是 LogManagerImpl 类中的一个成员变量，所以在 LogManagerImpl 的 getEntry(long index) 方法内部，就可以先通过 SegmentList 日志缓存池获得对应索引的日志，获取不到再通过 RocksDBLogStorage 日志存储器获得，getEntry(long index) 方法就可以重构成下面这样。请看下面代码块。

```
public class LogManagerImpl implements LogManager {


    //其他内容省略


    //下面这个成员变量是用来在内存中缓存条目日志对象的，所有的日志条目对象都会先缓存到内存里
    //然后就直接开始向跟随者传输日志了
    private final SegmentList<LogEntry> logsInMemory = new SegmentList<>(true);

    //日志存储器
    private LogStorage logStorage;



    //根据索引获得日志条目的方法，该方法重构了，加上了从内存中获取日志的步骤
    @Override
    public LogEntry getEntry(long index) {
        //这里使用的是读锁，可以同时读，但不能修改
        this.readLock.lock();
        try {//检验日志索引是否合规，日志索引不能比最后一条索引大，不能比第一条索引小
            if (index > this.lastLogIndex || index < this.firstLogIndex) {
                return null;
            }
            
            //先从内存中获取日志，获取不到再从日志存储器中获取
            final LogEntry entry = getEntryFromMemory(index);
            if (entry != null) {
                return entry;
            }
        } finally {
            this.readLock.unlock();
        }
        
        //从日志存储器中获得指定索引的日志条目
        final LogEntry entry = this.logStorage.getEntry(index);
        if (entry == null) {
            //如果日志条目为空则报错
            reportError(RaftError.EIO.getNumber(), "Corrupted entry at index=%d, not found", index);
        }
        //这里判断了一下是否开启了校验和功能，如果开启了，就将从数据存储器中获得的日志的校验和之前记录的做一下对比
        //如果不想等则意味着日志有损坏
        if (entry != null && this.raftOptions.isEnableLogEntryChecksum() && entry.isCorrupted()) {
            String msg = String.format("Corrupted entry at index=%d, term=%d, expectedChecksum=%d, realChecksum=%d",
                    index, entry.getId().getTerm(), entry.getChecksum(), entry.checksum());
            reportError(RaftError.EIO.getNumber(), msg);
            throw new LogEntryCorruptedException(msg);
        }
        return entry;
    }



    //从日志缓存池中获取日志的方法
    protected LogEntry getEntryFromMemory(final long index) {
        LogEntry entry = null;
        if (!this.logsInMemory.isEmpty()) {
            //获取内存中的第一条日志索引
            final long firstIndex = this.logsInMemory.peekFirst().getId().getIndex();
            //获取内存中最后一条日志索引
            final long lastIndex = this.logsInMemory.peekLast().getId().getIndex();
            //如果日志的数量和内存中的不想等就报错
            if (lastIndex - firstIndex + 1 != this.logsInMemory.size()) {
                throw new IllegalStateException(String.format("lastIndex=%d,firstIndex=%d,logsInMemory=[%s]",
                        lastIndex, firstIndex, descLogsInMemory()));
            }
            //校验合规后从内存中获得索引对应的日志
            if (index >= firstIndex && index <= lastIndex) {
                entry = this.logsInMemory.get((int) (index - firstIndex));
            }
        }
        return entry;
    }


    




    //其他内容省略
}
```

到此为止，我就为大家把领导者获取日志的功能重构完毕了。好了，现在我们已经确定了领导者向跟随者节点发送日志的方式，使用的是批量传输的方式，并不是一条一条地发送；也对领导者获取日志的方法进行了重构，接下来就可以正式实现领导者向跟随者节点传输日志的功能了。

## 实现日志传输功能

  

在上一章，我为大家实现领导者处理探针响应的功能时，为大家引入了一个 onRpcReturned 方法，该方法会在领导者接收到探针请求的响应后被回调，具体流程请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;



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



    //其他内容省略
}
```

可以看到，在上面的代码块中，**如果领导者发现接收到的探针响应是成功的，就会执行代码块中第 39 行的代码，也就是 r.sendEntries() 这行代码，r 就是当前 Replicator 复制器对象本身，所以这里就是调用了 Replicator 复制器对象的 sendEntries() 方法，在该方法内部，执行了领导者向跟随者传输日志的操作**。那么在 sendEntries 方法内部发送日志时，具体的流程是什么呢？换句话说，在 sendEntries 方法内部，要给跟随者节点发送日志，要执行的具体步骤有哪些呢？接下来，就让我为大家分析一下。

领导者要向跟随者节点发送日志，首先要知道自己应该从第几个索引开始传输日志，而这个索引在上一章我也为大家分析过了，**就是 Replicator 复制器对象的 nextIndex 成员变量的值，这个 nextIndex 成员变量会在探针请求的响应中被更新为正确值，然后领导者就可以根据这个 nextIndex 开始传输日志了**。但是，知道这个 nextIndex 值之后，还不能立刻传输日志，因为还有一点没有确定，那就是领导者要向跟随者节点传输多少条日志？关于这个问题我可以给出两点解答。

1 领导者向跟随者传输日志虽然是批量传输的，但是每一次批量传输的日志也是有字节限制的，综合原因有很多，比如传输的数据越多，网络传输得就会越慢；如果本批次日志传输失败，那么这一批日志中的所有日志都要重新传送，假如每一批日志字节总量非常大，那么重新传送也会很麻烦。如果每一批只传输 5 条日志，即便传输失败了，那么重新传送这 5 条日志就行了，如果每一批传送 5000 条日志，每一次传输失败，就意味着 5000 条日志要重新传输，哪种方式更加有利显然是一目了然的。所以，**在 sofajraft 框架中已经对领导者每一次向跟随者节点传输日志的字节总量和日志条目数量做了一个限制，每次最多只允许传输 512 KB，每一次最多允许传输 1024 条日志，超过这两个限制就不能再传输了**。这里我们实现日志传输功能时，也直接采用这个限制即可。

2 就算领导者再怎么想传输日志给跟随者节点，也要遵循客观限制，说得直接一点，假如客户端没有向领导者发送指令，领导者内部就不会创建 LogEntry 对象，也就根本没办法向跟随者节点传输日志。当然，**假如领导者正在批量发送一批日志，所有日志都序列化完毕了，还没有超过 512 KB 大小，那么就会直接把这批日志发送给跟随者节点，不会等待有新的日志到来，不需要积累 512 KB 数据后再发送**。

所以，将上面两点整合一下，概括下来就是：**领导者向跟随者传输日志的时候，如果自身内部根本就没有日志，就无法向跟随者节点传输日志，如果存在日志，每一次最多发送 512 KB 数据给跟随者节点，如果日志总字节数没达到 512 KB，也会直接把这一批日志发送给跟随者节点；如果日志总字节数没达到 512 KB，但是本批次发送的日志条目数量达到 1024 条了，也不会再传输更多的日志，而是直接把本批次日志发送给跟随者节点**。

这个明确了之后，好像就应该开始传输日志了，那么现在就有一个问题需要我们解决，怎样把日志传输给跟随者节点呢？按照我之前为大家分析的思路，显然是应该把要发送的日志封装到一个请求体中，然后把请求体发送给跟随者节点。那么，应该把日志封装到什么请求体中呢？我想经过前几章的学习，答案已经不言而喻了，**要发送给跟随者节点的日志仍然应该封装到 AppendEntriesRequest 请求体中**。这个知识在前几章的代码中多少也提到过一些了。

到目前为止，我们已经在程序中见过很多次 AppendEntriesRequest 请求体对象了，但是这个请求体对象的内部结构我还没有为大家详细介绍过，虽然通过过去几章的学习，我们已经掌握得差不多了。比如说，我们都知道不管是领导者向跟随者节点发送心跳请求还是探针请求，都要封装一些公共属性在 AppendEntriesRequest 请求体中，**领导者所在集群的 Id、领导者自己的 server_Id、跟随者节点的 peer_Id、领导者当前的任期、prev_log_term 和 prev_log_index，最后还有领导者最新应用的日志的索引，也就是 committed_index**。所以，综合下来，AppendEntriesRequest 对象内部肯定也定义着上面这些成员变量。如果遵循 Protobuf 语法来定义这个 AppendEntriesRequest 请求体，就可以定义成下面这样，请看下面代码块。

```
//required表示的是必填的字段
message AppendEntriesRequest {
  required string group_id = 1;
  required string server_id = 2;
  required string peer_id = 3;
  required int64 term = 4;
  required int64 prev_log_term = 5;
  required int64 prev_log_index = 6;
  required int64 committed_index = 7;
};
```

上面这个代码块的结构可以说是一目了然，好了，现在我们要使用这个 AppendEntriesRequest 请求发送日志，并且是批量发送日志，只在 AppendEntriesRequest 请求体中定义这几个字段，显然是不够用的。因为要发送日志了，但现在 AppendEntriesRequest 请求中根本没有字段可以封装日志数据。所以，现在的问题就变成了要在 AppendEntriesRequest 请求体中定义什么字段来封装日志数据。这就要从日志条目对象本身的结构来分析了，接下来请大家再次回顾一下 LogEntry 对象的代码，请看下面代码块。

```
//该类的对象就是一个日志条目
public class LogEntry {


    public static final ByteBuffer EMPTY_DATA = ByteBuffer.wrap(new byte[0]);

    //日志ID，注意，日志ID中封装着日志的索引和任期
    private LogId id = new LogId(0, 0);

    //真正的日志信息，初始化时为空
    private ByteBuffer data = EMPTY_DATA;

    //日志类型，可能是业务日志，也可能是配置变更日志
    private EnumOutter.EntryType type;

    //当前集群中的节点，这里也许有朋友会感到困惑，日志条目不是只包含日志锁云和任期以及日志内容吗？
    //为什么还要包含集群中的节点信息，这里解释一下，在jraft框架中除了业务日志，还有配置变更日志
    //配置变更日志也是使用当前类的对象来封装的，因此会有以下几个成员变量
    //并且配置日志也是会传输给跟随者的
    private List<PeerId> peers;
    
    //旧配置中的节点
    private List<PeerId> oldPeers;

    //校验和，这个成员变量很重要，在日志传输之后，会在跟随者节点判断校验和是否和之前的相等，以此来判断日志条目是否有损坏
    //这个成员变量可以暂时忽略，等用到的时候我会为大家讲解
    private long checksum;
    
    //是否有校验和
    private boolean hasChecksum;


    public LogId getId() {
        return this.id;
    }

    public void setId(final LogId id) {
        this.id = id;
    }

    public ByteBuffer getData() {
        return this.data;
    }

    public void setData(final ByteBuffer data) {
        this.data = data;
    }

    public List<PeerId> getPeers() {
        return this.peers;
    }

    public void setPeers(final List<PeerId> peers) {
        this.peers = peers;
    }

    public List<PeerId> getOldPeers() {
        return this.oldPeers;
    }

    public void setOldPeers(final List<PeerId> oldPeers) {
        this.oldPeers = oldPeers;
    }
    
}
```

在上面这个代码块中，我们可以看到在 LogEntry 类中定义了很多成员变量。有什么日志 Id 成员变量，日志类型，还有集群中的配置信息，也就是节点信息等等。不用我说大家也都知道，这些成员变量中，最重要的成员变量肯定就是那个 ByteBuffer 类型的 data 成员变量，因为这个成员变量真正封装了客户端指令，并且这个客户端指令已经以字节数组的形式封装好了。发现了这一点之后，我脑海里产生了一个想法，反正每一条日志封装的最重要的数据就是这个客户端指令了，客户端指令已经是字节数组了，**如果我将每一个 LogEntry 对象的 data 成员变量封装的数据和其他数据分隔开**，这样传输日志会更加方便吗？

比如说我为每一个 LogEntry 对象再定义一个新的元数据对象，这个元数据对象就叫做 EntryMeta，意思为每一个日志条目的元数据。这个 EntryMeta 元数据对象中封装了每一个 LogEntry 对象的元数据。那么所谓的元数据是什么呢？这个就更简单了，**所谓的元数据就是 LogEntry 日志条目的任期，日志类型，配置节点信息，日志校验和，以及最重要的一个数据，那就是本条日志对象封装的客户端指令的字节长度，也就是 LogEntry 对象的 data 成员变量的字节长度**。如果用 Protobuf 语法来定义这个 EntryMeta 元数据对象，可以写成下面这样，请看下面代码块。

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

现在这个 EntryMeta 元数据对象也定义好了，那么具体该怎么使用呢？接下来让我为大家举一个例子，为大家详细讲解一下这个 EntryMeta 对象是怎么使用的。**假如现在有 5 条日志要从领导者传输给跟随者节点，按照我们刚才分析的结果，我会为这 5 条日志分别创建 EntryMeta 对象，计算出每一个 LogEntry 对象中封装的客户端指令的字节长度，接着给 EntryMeta 元数据对象中的各个字段赋值，然后按照日志的索引顺序把这几个 EntryMeta 对象放到一起，比如说就放到一个集合中；之后就可以把这 5 条日志中封装的客户端指令，也就是每个 LogEntry 对象的 data 成员变量存放到一起，因为 data 是 ByteBuffer 类型的，现在要把每个日志的 data 数据汇总到一起了，就可以把它们放到一个更大的 ByteBuffer 对象中，也就是用一个更大的字节数组来存放这些数据**。就像下面代码块中展示的这样。

```
LogEntry1  元数据：EntryMeta1   有效数据：data1
LogEntry2  元数据：EntryMeta2   有效数据：data2
LogEntry3  元数据：EntryMeta3   有效数据：data3
LogEntry4  元数据：EntryMeta4   有效数据：data4
LogEntry5  元数据：EntryMeta5   有效数据：data5


最后发送给跟随者节点的数据：
List<EntryMeta> = EntryMeta1，EntryMeta2，EntryMeta3，EntryMeta4，EntryMeta5
bytes = data1 + data2 + data3 + data4 + data5
```

这时候，大家应该明白批量发送日志时究竟该怎么封装日志了吧(至于跟随者节点怎么接受日志，并且还原每一个 LogEntry 对象会在下一章为大家实现，本章先不必考虑)？如果这个清楚了，那么现在我们就面临一个新的问题，因为日志数据也是封装在 AppendEntriesRequest 请求体中的，但是这个 AppendEntriesRequest 请求体内部并没有字段可以封装每一条日志对象的元数据信息和要发送的总的 data 数据。所以，接下来，我还要对 AppendEntriesRequest 请求对象做一次重构，就重构成下面这样，请看下面的代码块。

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

好了，课程进行到这里，发送日志的前置工作才准备的差不多了，接下来就可以真正发送日志了。假如现在我要先定义一个方法，比如说这个方法就定义为 sendEntries(long nextSendingIndex)，注意，这个方法和刚才展示的 r.sendEntries() 方法并不相同，现在我要定义的这个方法带有一个 nextSendingIndex 参数，nextSendingIndex 参数就是开始传输日志的索引。我要定义的这个 sendEntries(long nextSendingIndex) 方法的大概流程是这样的：**那就是我在方法中先创建一个 AppendEntriesRequest 对象，然后填充对象的公共字段，这都是旧知识，就不再重复讲解了。填充完请求的公共字段之后，我就要将这一次批量发送的日志数据封装到 AppendEntriesRequest 请求体中了。具体的逻辑就是定义一个循环，最多循环 1024 次，因为领导者最多可以向跟随者节点发送 1024 条日志；在每一次循环中为 LogEntry 对象创建 EntryMeta 对象，并且使用一个集合存放 LogEntry 对象的 data 数据，这个集合就可以定义为 RecyclableByteBufferList。而且我还会在每一次循环中判断集合中存放的 data 数据总大小是否超过 512 KB，超过了就终止循环。总之，循环结束之后，把 RecyclableByteBufferList 集合中的所有 data 数据汇总到一个 ByteBuffer 对象中，然后再把这个 ByteBuffer 对象中的数据封装到 AppendEntriesRequest 请求中即可。最后把 AppendEntriesRequest 请求发送给跟随者节点，这样一来，领导者给跟随着节点传输日志的功能就算是实现完毕了**。

好了，我也很清楚，文字分析并不会让简单流程变得更清晰，所以我还是直接为大家展示代码吧，在代码层面展示一下 sendEntries(long nextSendingIndex) 方法的具体实现。请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;



    //该方法会在探针请求收到响应后被回调，也会在给跟随者发送传输日志的请求，收到响应后被回调
    //在第四版本的代码中，该方法做了大量删减，很多方法参数用不上，第五版本会重构完整，这里这个status还没有用到，等到第六版本就会用到了
    //这个status还和Bolt框架有关系，所以牵扯到的知识还是有点复杂的，在实现了领导者传输日志给跟随者的功能之后再具体讲解
    @SuppressWarnings("ContinueOrBreakFromFinallyBlock")
    static void onRpcReturned(final ThreadId id, final Status status, final Message request, final Message response,final long rpcSendTime) {

        //其他内容省略
       
        finally {
            
            //判断是否可以继续发送日志给跟随者节点
            if (continueSendEntries) {
                //在这里继续发送下一批日志
                r.sendEntries();
            }
        }
    }








    //批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志索引
    private boolean sendEntries(final long nextSendingIndex) {

        //创建一个AppendEntriesRequest请求构建器
        final RpcRequests.AppendEntriesRequest.Builder rb = RpcRequests.AppendEntriesRequest.newBuilder();
        
        //填充公共字段
        if (!fillCommonFields(rb, nextSendingIndex - 1, false)) {
            //填充失败就安装快照，但是这里把安装快照的代码省略了
            //这里也有一个安装快照的操作，这里的逻辑在第八版本代码在为大家讲解
            return false;
        }


        //定义一个ByteBufferCollector对象，要发送的所有日志条目中的data最终都会放到这个对象中
        //这个ByteBufferCollector对象内部有一个ByteBuffer类型的成员变量，并且该成员变量会自动扩容
        //实际上，每一个LogEntry对象的data数据最后都会放到这个对象的ByteBuffer成员变量中
        //这个ByteBufferCollector类非常简单，我就不具体展开了，大家可以直接去我提供的第四版本代码中学习
        ByteBufferCollector dataBuf = null;


        //得到单次可以发送给跟随者的日志条目的最大值，也就是1024条
        final int maxEntriesSize = this.raftOptions.getMaxEntriesSize();


         //得到一个RecyclableByteBufferList对象，因为要发送的LogEntry的data会先放到RecyclableByteBufferList对象中
        //这个RecyclableByteBufferList其实就是一个ArrayList，因为它继承了ArrayList类，每一个LogEntry的data数据先放到
        //RecyclableByteBufferList内部的数组中，最后从RecyclableByteBufferList内部的数组中都存放到刚才创建的ByteBufferCollector对象中
        //这个RecyclableByteBufferList类我也不给大家展开讲解了，大家可以直接去第四版本代码中查看，这个类也非常简单，就是一个ArrayList的逻辑
        //大家知道它的作用即可，这里我还要多说一句，从名字上也能看出来，这个RecyclableByteBufferList对象有自己的对象池，使用完毕之后需要归还给对象池
        final RecyclableByteBufferList byteBufList = RecyclableByteBufferList.newInstance();


        try {//开始在一个循环中封装要发送的日志条目了
            for (int i = 0; i < maxEntriesSize; i++) {
                //创建封装日志元数据对象的构建器，注意，每一个要发送的日志条目对象都对应着一个元信息对象
                //这些日志条目对象的元信息对象会构成一个元信息集合，最后会被放到AppendEntriesRequest请求中
                //发送给跟随者
                final RaftOutter.EntryMeta.Builder emb = RaftOutter.EntryMeta.newBuilder();
                //准备发送日志，在prepareEntry方法中，会把日志条目的元数据和日志条目的data分开存放
                //元数据存放到emb对象中，data存放到byteBufList集合的数组中
                //prepareEntry方法是当前方法中最重要的一个步骤，prepareEntry方法也展示在代码块中了
                if (!prepareEntry(nextSendingIndex, i, emb, byteBufList)) {
                    //准备失败就意味着没有日志可以发送了，直接退出循环即可
                    break;
                }//在这里把本批次发送的日志条目的元数据集合设置到AppendEntriesRequest请求中
                rb.addEntries(emb.build());
            }


             //更新下一条要发送的日志的索引
            //rb.getEntriesCount()是本批次要发送的日志条目
            //实际上就是AppendEntriesRequest请求中封装的EntryMeta元数据的数量
            //因为每一个LogEntry对应一个EntryMeta对象，所以有几个EntryMeta对象就意味着本批次发送了多少条日志
            this.nextIndex += rb.getEntriesCount();


            //在上面的for循环中，每一个日志条目对象的data都放到了byteBufList集合中
            //但这个集合中的数据还没有放到AppendEntriesRequest请求请求中呢，接下来的操作就是将所有的
            //data放到AppendEntriesRequest请求对象中
            //先判断byteBuffer集合中是否存放了数据
            if (byteBufList.getCapacity() > 0) {
                //如果存放了就根据集合中存放的总的数据的大小创建一个ByteBufferCollector对象
                //ByteBufferCollector对象终于要派上用场了
                dataBuf = ByteBufferCollector.allocateByRecyclers(byteBufList.getCapacity());
                //遍历这个byteBufList集合
                for (final ByteBuffer b : byteBufList) {
                    //把集合中的每个data对象都存放到ByteBufferCollector对象的内部成员变量bytebuffer中
                    dataBuf.put(b);
                }//得到ByteBufferCollector对象内部的ByteBuffer对象，这时候，这个ByteBuffer对象已经存放了所有日志条目的data
                final ByteBuffer buf = dataBuf.getBuffer();
                //设置ByteBuffer读模式
                BufferUtils.flip(buf);
                //在ZeroByteStringHelper的帮助下把ByteBuffer中的数据读取到ByteString对象中，这是protobuf协议传输数据用到的对象
                //然后把ByteString对象设置到请求中，这时候AppendEntriesRequest请求中就封装了所有日志条目的data数据
                rb.setData(ZeroByteStringHelper.wrap(buf));
            }
        } finally {//在这里将RecyclableByteBufferList对象回收到对象池中
            RecycleUtil.recycle(byteBufList);
        }



        //构建AppendEntriesRequest请求对象，接下来就要开始发送请求了
        final RpcRequests.AppendEntriesRequest request = rb.build();

        //发送请求给跟随者节点，把本批日志传输过去
        this.rpcService.appendEntries(this.options.getPeerId().getEndpoint(), request, -1,
                    new RpcResponseClosureAdapter<RpcRequests.AppendEntriesResponse>() {

                        @Override
                        public void run(final Status status) {
                            //该方法会在领导者节点收到日志传输响应后回调
                            onRpcReturned(Replicator.this.id, status, request, getResponse(),monotonicSendTimeMs);
                        }
                    });


        return true;
    }




    
    //在该方法中，会把日志条目的元信息和日志条目的data分开存放，日志条目的元信息存放到emb对象中，data存放到byteBufList集合的数组中
    //nextSendingIndex代表下一条要发送的日志索引，offset就是从外层方法传进来的那个i，初始值为0
    boolean prepareEntry(final long nextSendingIndex, final int offset, final RaftOutter.EntryMeta.Builder emb,
                         final RecyclableByteBufferList dateBuffer) {
        
        //判断dateBuffer集合中存放的数据的字节总量是否已经超过了最大值
        //raftOptions.getMaxBodySize()得到的就是512kb
        if (dateBuffer.getCapacity() >= this.raftOptions.getMaxBodySize()) {
            return false;
        } 
        
        //得到要发送的下一条日志的索引，offset就是外层方法中的i，是从0开始的
        final long logIndex = nextSendingIndex + offset;
        //根据索引获取日志条目对象，注意，这里会先从日志缓存池组件中获取对应日志
        final LogEntry entry = this.options.getLogManager().getEntry(logIndex);
        if (entry == null) {
            //没有日志则返回false
            return false;
        }
        
        //把日志条目的任期设置到元信息对象中
        emb.setTerm(entry.getId().getTerm());
        //计算校验和
        if (entry.hasChecksum()) {
            //设置校验和
            emb.setChecksum(entry.getChecksum());
        }
        
        //设置日志条目的类型，也许是配置变更日志，也许是业务日志
        emb.setType(entry.getType());
        //判断日志中是否有集群节点信息
        if (entry.getPeers() != null) {
            Requires.requireTrue(!entry.getPeers().isEmpty(), "Empty peers at logIndex=%d", logIndex);
            //填充元信息到emb对象中，这里就是把集群节点信息填充到元信息对象中
            fillMetaPeers(emb, entry);
        } else {//走到这里意味着日志条目对象中没有集群节点信息，这时候就要判断一下日志条目是不是配置变更日志
            //如果是配置变更日志就意味着出错了，如果是普通的业务日志，那就没问题
            Requires.requireTrue(entry.getType() != EnumOutter.EntryType.ENTRY_TYPE_CONFIGURATION,
                    "Empty peers but is ENTRY_TYPE_CONFIGURATION type at logIndex=%d", logIndex);
        }
        
        
        //接下来就该封装日志中的data数据了，这个才是有效的日志信息，也就是业务指令
        //这里得到data的字节长度
        final int remaining = entry.getData() != null ? entry.getData().remaining() : 0;
        //把字节长度填充到元信息对象中，在跟随者节点就是根据这个字节长度从ByteBuffer中获取每一条日志的data的，这个逻辑在第五版本会实现
        emb.setDataLen(remaining);
        if (entry.getData() != null) {
            //把data自身存放到dateBuffer集合中
            dateBuffer.add(entry.getData().slice());
        }
        
        //这里大家可能会疑惑，为什么方法都结束了，却没有把每一条日志的索引设置到元信息对象中呢？
        //这里我简单解释一下，这个逻辑并不是在领导者这一端实现的，而是在跟随者接收到日志之后
        //通过领导者发送过来的请求中的上一条日志的索引，用这个索引递增，然后给这一批日志设置索引
        return true;
    }



    

    //其他内容省略
}
```

到此为止，我就把领导者给跟随者节点批量发送日志的功能完整实现了。上面代码块中的注释非常详细，并且在展示代码之前，我也跟大家详细分析过实现思路了，所以就不再重复讲解上面代码中的逻辑了。当然，别忘了我们一开始要实现的是 r.sendEntries() 这个方法，那么这个方法怎么和上面代码块中实现的两个方法联系起来呢？这个也很简单，请大家先看看下面的代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
public class Replicator implements ThreadId.OnError {


    //省略其他内容


    //要发送到这个复制器对象代表的节点的下一条日志的索引
    private volatile long nextIndex;



    //该方法会在探针请求收到响应后被回调，也会在给跟随者发送传输日志的请求，收到响应后被回调
    //在第四版本的代码中，该方法做了大量删减，很多方法参数用不上，第五版本会重构完整，这里这个status还没有用到，等到第六版本就会用到了
    //这个status还和Bolt框架有关系，所以牵扯到的知识还是有点复杂的，在实现了领导者传输日志给跟随者的功能之后再具体讲解
    @SuppressWarnings("ContinueOrBreakFromFinallyBlock")
    static void onRpcReturned(final ThreadId id, final Status status, final Message request, final Message response,final long rpcSendTime) {

        //其他内容省略
       
        finally {
            
            //判断是否可以继续发送日志给跟随者节点
            if (continueSendEntries) {
                //在这里继续发送下一批日志
                r.sendEntries();
            }
        }
    }






    //发送日志给跟随者的方法，这里要再次强调一下，在sofajraft中，领导者并不会每次只发送一条日志给跟随者，而是会批量发送日志
    //每次都会发送一批日志，每一次具体发送多少日志，实现逻辑请看sendEntries方法
    void sendEntries() {
        boolean doUnlock = true;
        try {
            long prevSendIndex = -1;
            while (true) {
                //在一个循环中，不断获取要发送的日志的索引，然后发送日志，直到获取到的索引
                //超过了能够发送的日志，也就是说，领导者中的日志已经暂时发送完毕了
                //这时候就会跳出循环
                final long nextSendingIndex = getNextSendIndex();
                if (nextSendingIndex > prevSendIndex) {
                    if (sendEntries(nextSendingIndex)) {
                        prevSendIndex = nextSendingIndex;
                    } else {//走到这里意味着发送失败，可以退出该方法了
                        //但是这并不是意味着领导者就不再向跟随者发送日志了，有可能
                        //是领导者中目前没有日志了，这时候复制器会给领导者的日志管理器
                        //注册一个回调方法，当日志管理器中有新的日志放到内存了，就会通知
                        //复制器继续向跟随者复制日志，很快就会看到具体的逻辑了
                        doUnlock = false;
                        break;
                    }
                } else {//走到这里意味着获得的下一次要发送的日志的索引并不大于上一次
                    //这里的逻辑在第六版本代码才能看明白，在第四版本代码中，大家可以暂时忽略这个else分支
                    break;
                }
            }
        } finally {
            if (doUnlock) {
                unlockId();
            }
        }
    }




    
    //批量发送日志给跟随者的方法，nextSendingIndex是下一条要发送的日志索引
    private boolean sendEntries(final long nextSendingIndex) {

        //该方法就省略了
    }


    
    //其他内容省略

    
}
```

可以看到，我最终为大家实现的这个 sendEntries() 方法，其实就是在一个循环中不停获得下一次可以发送的日志的索引，如果这个索引并不大于上一次发送的日志索引，就意味着领导者内部暂时没有日志可以发送了，就可以直接退出循环了。到此位置，领导者向跟随者节点传输日志的全流程才展示完毕。大家可以再品味品味这个流程，里面还是蕴含了一些细节的，如果看文章中展示的代码不太方便，大家可以结合第四版本代码学习本章内容。到这里本章内容其实就可以结束了，虽然还有一个知识没有为大家讲解，**那就是当领导者内部没有日志之后，就不会再向跟随者节点传输日志了，但是，等到领导者内部再次产生了日志，领导者内部的每一个 Replicator 复制器对象怎么就知道可以继续传输日志了呢**？这个功能的实现也很简单，但本章没有时间讲解了，就留到下一章为大家实现吧。好了诸位，我们下一章见！