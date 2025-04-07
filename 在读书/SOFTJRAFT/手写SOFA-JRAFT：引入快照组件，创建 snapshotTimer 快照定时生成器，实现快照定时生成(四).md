朋友们，到这篇文章为止，我们已经围绕着 sofajraft 框架生成快照功能写了三篇文章了，这一章是第四篇。由此可见，jraft 框架生成快照的功能实现起来确实有些繁琐，需要引入的组件比较多，并且在实现各个的组件的过程中，需要分析的细节问题也比较多。总之到这个时候我们可以发现，虽然实现 sofajraft 框架生成快照功能的文章越写越多，实现的功能组件越来越多，也重构得越来越完善，也解决了一些问题，但是与此同时产生的问题也越来越多。在上一章一开始我就为大家展示了五个需要我们解决的问题，我把这五个问题搬运到这里了：

**1 虽然在 jraft 集群启动的时候可以由用户自己定义存储快照文件的路径，但是用户定义的快照文件的存储路径并没有在我们实现的功能中用到。**

**2 同步或者异步生成快照的功能并没有在程序中体现出来，也就是说，并没有把同步或者异步生成快照的操作交给用户来选择。**

**3 快照定时器每一次要执行的定时任务，都必须真的执行吗？什么情况下可以不执行本次生成快照文件的定时任务呢？**

**4 生成了快照文件之后，那么与快照文件存储数据对应的日志条目应该怎么删除呢？**

**5 快照文件的生成是一个定时任务，也就是说，在 sofajraft 框架构建的 raft 集群中，只要集群一直正常运行，对外提供服务，那么每一个节点都会应用新的日志，每一个节点备份的数据会发生变化，也就意味着会生成新的快照文件。那么新的快照文件生成了，旧的快照文件该怎么删除呢？毕竟每一个快照文件存储的都是生成快照文件那一刻状态机中保存的所有数据，既然是这样，那么新生成的快照文件中的数据一定是状态机中保存的最新的数据，所以旧快照文件也就可以删除了，这一点大家应该可以理解吧？那么旧的快照文件应该怎么删除呢**？

在上一章结束之后，上面这五个问题中的第一个和第五个问题算是得到解决了，也就是说，还剩下三个问题没有解决。但是在上一章结束之前，又产生了几个新的问题，这样一来没有解决的问题反而更多了。我把产生的几个新的问题哈之前遗留的三个问题合并了一下，给大家总结在下面了：

**1 同步或者异步生成快照的功能并没有在程序中体现出来，也就是说，并没有把同步或者异步生成快照的操作交给用户来选择。**

**2 快照定时器每一次要执行的定时任务，都必须真的执行吗？什么情况下可以不执行本次生成快照文件的定时任务呢？**

**3 生成了快照文件之后，那么与快照文件存储数据对应的日志条目应该怎么删除呢？**

**4 SaveSnapshotClosure 回调对象该怎么创建呢？**

**5 SaveSnapshotClosure 回调对象该怎么交给 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法使用？**

**6 LocalSnapshotStorage 快照存储器具体在哪里创建，初始化呢？**

**7 LocalSnapshotStorage 快照存储器的 close() 方法怎么在 SaveSnapshotClosure 对象的方法中被调用呢？**

**8 怎么把 LocalSnapshotStorage 快照存储器中的存储快照文件的临时文件夹路径交给 FSMCallerImpl 状态机组件使用呢？**

以上就是我们目前要解决的问题，而我们目前明确的程序生成快照的流程是这样的：

**快照文件是由 FSMCallerImpl 状态机组件生成的，而 FSMCallerImpl 状态机组件成功生成完快照之后，会回调 SaveSnapshotClosure 对象中的方法，而在 SaveSnapshotClosure 对象的方法中，会执行 LocalSnapshotStorage 快照存储器的 close() 方法，该方法一旦执行，就会把新生成的快照从临时文件夹中移动到正式文件夹中，并且还会视情况删除旧的快照文件**。以上就是我之前为大家确定下来的程序执行生成快照文件的操作流程，大家可以再回顾一下，然后再让我们来看看上面问题中的第八个问题应该怎么解决。

当然，之所以先关注第八个问题，我在上一章已经为大家解释过原因了：**我们并不清楚怎么把 LocalSnapshotStorage 快照存储器中的存储快照文件的临时文件夹路径交给 FSMCallerImpl 状态机组件使用**，**我们都知道快照文件是被 FSMCallerImpl 状态机组件生成的，但是生成的时候要先把快照文件存储在临时文件夹中，而临时文件夹的路径被 LocalSnapshotStorage 快照存储器持有，所以必须让 LocalSnapshotStorage 快照存储器把临时文件夹的路径交给 FSMCallerImpl 状态机组件使用，我们之前实现的一切功能才能正常发挥作用**。所以我们要先实现这个功能，要让 LocalSnapshotStorage 快照存储器可以把自己持有的临时文件夹路径交给 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法使用。那这个功能该怎么实现呢？这时候我就要再为我们的程序引入一个新的组件了。

## 引入 LocalSnapshotWriter 快照写入器

我要引入的这个新的组件就是 LocalSnapshotWriter 快照写入器，我先来跟大家解释一下我引入这个 LocalSnapshotWriter 快照写入器的作用：**LocalSnapshotWriter 快照写入器就持有了 LocalSnapshotStorage 快照存储器中的临时文件夹的路径，当 FSMCallerImpl 状态机组件执行生成快照文件操作的时候，就会使用这个 LocalSnapshotWriter 快照写入器中持有的临时文件夹的路径，然后把生成的快照文件存放到临时文件夹中**。尽管我为大家解释了 LocalSnapshotWriter 快照写入器的作用，但是现在大家肯定还是对这个 LocalSnapshotWriter 快照写入器感到困惑，接下来我先给大家展示一段代码，给大家看看这个 LocalSnapshotWriter 快照写入器究竟是怎么工作的，然后大家再看我解释引入这个 LocalSnapshotWriter 快照写入器的具体原因。

之前我已经为大家实现了 FSMCallerImpl 状态机组件，并且也给大家展示了真正的 StateMachine 状态机是怎么生成快照文件的。而现在我跟大家说要引入一个 LocalSnapshotWriter 快照写入器，并且这个快照写入器内部持有了LocalSnapshotStorage 快照存储器中的临时文件夹的路径，所以我目前可以把这个 LocalSnapshotWriter 快照写入器定义成下面这样，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot.local;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/21
 * @方法描述：快照写入器
 */
public class LocalSnapshotWriter extends SnapshotWriter {

    private static final Logger LOG = LoggerFactory.getLogger(LocalSnapshotWriter.class);


    //快照文件要存储的临时路径
    private final String path;

    //构造方法
    public LocalSnapshotWriter(String path) {
        this.path = path;
    }

    @Override
    public String getPath() {
        return this.path;
    }
}
```

以上就是目前我新引入的 LocalSnapshotWriter 快照写入器，好了，快照写入器展示完毕了，接下来我为大家展示一下它的具体使用方法，我把重构之后的 FSMCallerImpl 状态机组件生成快照文件的代码展示在下面了，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    //用户真正定义的状态机
    private StateMachine fsm;

    //省略该类其他内容

    
    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/21
     * @方法描述：生成快照的入口方法，该方法中的SaveSnapshotClosure参数对象，大家先不必关注，很快我就会为大家讲到
     */
    @Override
    public boolean onSnapshotSave(final SaveSnapshotClosure done) {
        //把生成快照的操作封装为任务发布到disruptor的环形数组中
        return enqueueTask((task, sequence) -> {
            task.type = TaskType.SNAPSHOT_SAVE;
            task.done = done;
        });
    }


    
     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    private void doSnapshotSave(final SaveSnapshotClosure done) {

        //生成快照元信息文件的操作暂时省略

        //从回调对象中得到快照写入器，大家可以暂时先不思考怎么从回调对象中得到快照写入器的
        //先记住这一点即可，很快我就会为大家真正实现SaveSnapshotClosure回调对象了
        final SnapshotWriter writer = done.start();
        
        //状态机生成快照
        this.fsm.onSnapshotSave(writer, done);
    }

    


    //省略该类其他内容
}



//下面是我定义的MapStateMachine状态机对象，这个对象的onSnapshotSave()方法被重构了

//jraft集群中的状态机，必须继承StateMachineAdapter状态机适配器
//而StateMachineAdapter状态机适配器则实现了StateMachine状态机接口
public class MapStateMachine  extends StateMachineAdapter{

    private static final Logger LOG = LoggerFactory.getLogger(MapStateMachine.class);

    //这个就是内存数据库存储数据的map
    private HashMap<String,String> map = new HashMap<>();

    //在这里创建了一个线程池
    private static ThreadPoolExecutor executor   = ThreadPoolUtil
            .newBuilder()
            .poolName("JRAFT_TEST_EXECUTOR")
            .enableMetric(true)
            .coreThreads(3)
            .maximumThreads(5)
            .keepAliveSeconds(60L)
            .workQueue(new SynchronousQueue<>())
            .threadFactory(new NamedThreadFactory("JRaft-Test-Executor-", true)).build();



    //重构之后的onSnapshotSave()方法，SaveSnapshotClosure本身就是一个接口，它继承了Closure接口
    @Override
    public void onSnapshotSave(final SnapshotWriter writer, final Closure done) {
        //创建一个新的map
        final Map<String, String> values = new HashMap<>();
        //把map成员变量中的数据存储到新创建的map中
        for (final Map.Entry<String, String> entry : this.map.entrySet()) {
            values.put(entry.getKey(), entry.getValue());
        }

        //在这里异步把新创建的values中的数据写入到临时快照文件夹中了，快照文件名称为data
        executor.submit(() -> {
            //writer.getPath()得到的就是快照写入器持有的临时文件夹的路径
            //在前面章节的一些代码块中，大家可能在状态机当前方法中见过writer.getPath()代码
            //当时我没有为大家解释，实际上是我忽略了这一段代码，把没有重构的状态机生成快照文件的代码直接复制到文章中了
            //所以当时大家肯定会有些困惑，现在看到这里，大家应该知道writer.getPath()究竟是什么意思了吧？
            MapSnapshotFile snapshot = new MapSnapshotFile(writer.getPath() + File.separator + "data");
            //把新创建的values中的数据写入到快照文件中
            if (snapshot.save(values);) {
                //生成快照成功，则执行回调方法，在该方法中就会把快照文件从临时目录移动到正是目录
                done.run(Status.OK());
            } else {//生成快照失败要执行的操作
                done.run(new Status(RaftError.EIO, "Fail to save counter snapshot %s", snapshot.getPath()));
            }
        });
    }
 

    //省略该类的其他内容
}
```

好了朋友们，现在 LocalSnapshotWriter 快照写入器如何使用，我相信通过上面代码块，大家肯定都清楚了。代码块中注释非常详细，我就不再为大家重复解释其中的逻辑了。我能想到的是，现在大家肯定有两点非常困惑：**第一就是为什么可以在 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法中，通过 SaveSnapshotClosure 回调对象的 start() 方法就可以得到 LocalSnapshotWriter 快照写入器对象**？**第二就是 LocalSnapshotWriter 快照写入器究竟该怎么被创建，又怎么被封装到 SaveSnapshotClosure 回调对象中呢？并且这个快照写入器还有什么其他作用吗**？接下来就让我为大家详细解答一下。

## 重构 LocalSnapshotWriter 快照写入器

请大家想一想，目前我们实现的生成快照文件的功能，其实只是把状态机中的数据直接保存在快照文件中了，但我们之前说过，生成快照文件的时候，除了要把状态机中的数据保存到快照文件中，还要生成一个快照文件元信息文件，这一点大家应该都还有印象吧？我把快照元信息文件记录的内容搬运过来了，请看下面代码块。

```
//快照元信息
message SnapshotMeta {
    //生成快照的一瞬间，状态机应用的最后一条日志的索引
    required int64 last_included_index = 1;
    //生成快照的一瞬间，状态机应用的最后一条日志的任期
    required int64 last_included_term = 2;
    //生成快照的一瞬间，集群的节点信息，其实就是配置信息
    repeated string peers = 3;
    //生成快照的一瞬间，集群旧的成员列表
    repeated string old_peers = 4;
    //生成快照的一瞬间，集群学习者信息
    repeated string learners = 5;
    //生成快照的一瞬间，集群旧的学习者信息
    repeated string old_learners = 6;
}
```

把状态机中保存的数据写入到快照文件中，这个操作实际上是需要程序员自己实现的，在使用 sofajraft 框架构建 raft 集群的时候，我们需要实现 jraft 框架提供的 StateMachine 状态机接口，然后实现该接口的 onSnapshotSave() 方法，在该方法中生成快照文件，把状态机中保存的数据写入到快照文件中。这些操作流程大家肯定都很熟悉了，但是到目前为止，我们还不知道快照文件的元信息应该怎么保存呢，也就是说，我们还不知道快照信息原文件应该如何生成以及如何记录信息。**而记录快照文件元信息的功能，就被我定义在了 LocalSnapshotWriter 快照文件写入器中**。接下来就请大家跟我一起看看重构之后的 LocalSnapshotWriter 快照文件写入器的内容，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot.local;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/21
 * @方法描述：快照写入器
 */
public class LocalSnapshotWriter extends SnapshotWriter {

    private static final Logger LOG = LoggerFactory.getLogger(LocalSnapshotWriter.class);

    //快照元数据表，这个类的内容我就不展开了，快照元数据文件中的数据被加载到内存中后，就会保存在该类的对象中
    private final LocalSnapshotMetaTable metaTable;

    //快照文件要存储的临时路径
    private final String path;

    //快照文件元信息
    private RaftOutter.SnapshotMeta currentMeta;

    //快照存储器，注意，这里可以看到，快照写入器持有了快照存储器
    //至于为什么这么设计，很快我就会为大家讲解
    private final LocalSnapshotStorage snapshotStorage;

    //构造方法
    public LocalSnapshotWriter(String path, LocalSnapshotStorage snapshotStorage) {
        this.path = path;
        this.snapshotStorage = snapshotStorage;
        //在这里创建了快照元数据表
        this.metaTable = new LocalSnapshotMetaTable(raftOptions);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/21
     * @方法描述：初始化快照写入器的方法
     */
    @Override
    public boolean init(final Void v) {
        //创建快照要存储的文件目录，这里就是临时目录，逻辑已经在LocalSnapshotStorage类中讲过了
        final File dir = new File(this.path);
        try {//创建临时目录，创建的其实就是个文件夹
            FileUtils.forceMkdir(dir);
        } catch (final IOException e) {
            LOG.error("Fail to create directory {}.", this.path, e);
            setError(RaftError.EIO, "Fail to create directory  %s", this.path);
            return false;
        }
        //得到存放快照元数据文件的路径，快照元数据文件名为 "__raft_snapshot_meta
        final String metaPath = path + File.separator + JRAFT_SNAPSHOT_META_FILE;
        final File metaFile = new File(metaPath);
        try {
            if (metaFile.exists()) {
                //判断快照元数据文件是否存在，如果存在则加载快照元数据
                return metaTable.loadFromFile(metaPath);
            }
        } catch (final IOException e) {
            LOG.error("Fail to load snapshot meta from {}.", metaPath, e);
            setError(RaftError.EIO, "Fail to load snapshot meta from %s", metaPath);
            return false;
        }
        return true;
    }


    //从快照元数据表中得到本次生成的快照记录的最后应用的日志索引
    public long getSnapshotIndex() {
        return this.currentMeta.getLastIncludedIndex();
    }


    
    //在该方法中会调用快照存储器的关闭方法
    @Override
    public void close() throws IOException {
        //注意这里调用了快照存储器的close方法，并且把快照写入器传递到方法参数中了
        //这一点一定要记住，很快就要用到
        this.snapshotStorage.close(this);
    }


    //设置本次快照元数据到元数据表中
    @Override
    public boolean saveMeta(final RaftOutter.SnapshotMeta meta) {
        this.metaTable.setMeta(meta);
        return true;
    }
    

    //将快照元数据信息保存到硬盘文件中
    public boolean sync() throws IOException {
        //这里的代码就不展开了，大家可以直接去我提供的第八版本代码中查看
        //JRAFT_SNAPSHOT_META_FILE就是"__raft_snapshot_meta"文件名称
       return this.metaTable.saveToFile(this.path + File.separator + JRAFT_SNAPSHOT_META_FILE);
    }

    
    
    @Override
    public String getPath() {
        return this.path;
    }

    //设置快照文件元信息给currentMeta成员变量
    void setCurrentMeta(RaftOutter.SnapshotMeta currentMeta) {
        this.currentMeta = currentMeta;
    }


    //其他内容暂时不做实现
}
```

到此为止，我就为大家把 LocalSnapshotWriter 快照写入器重构完毕了，通过上面代码我们可以总结出三点：

**1 只要调用了 LocalSnapshotWriter 快照写入器的 setCurrentMeta() 方法，就可以把生成好的快照元信息交给快照写入器保存。**

**2 只要调用了 LocalSnapshotWriter 快照写入器的 sync() 方法，就可以把快照写入器保存的快照元信息保存在本地文件中。**

**3 只要调用 LocalSnapshotWriter 快照写入器的 close() 方法，就会在该方法内部调用 LocalSnapshotStorage 快照存储器的 close() 方法，并且在这个过程中，快照存储器的 close 方法还会得到 LocalSnapshotWriter 快照写入器。如果我们在 LocalSnapshotStorage 快照存储器中调用 LocalSnapshotWriter 快照写入器的 sync() 方法，这样一来，不就可以直接在 FSMCallerImpl 状态机组件生成快照之后，把快照文件从临时文件夹移动到正式文件夹中，也可以直接把快照元数据信息写入到本地文件中了吗**？

就像下面代码块展示的这样，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot.local;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/21
 * @方法描述：快照存储器
 */
public class LocalSnapshotStorage implements SnapshotStorage {

    private static final Logger LOG = LoggerFactory.getLogger(LocalSnapshotStorage.class);

    //存放快照文件的临时路径
    private static final String TEMP_PATH = "temp";

    //用户在程序启动时定义的存放快照的路径
    //其实就是/User/chenqingyang/code/jraft-log/server1/snapshot路径
    //只要把这个路径和上面的temp拼接起来，就可以得到存放临时快照文件的文件夹路径
    private final String path;

    //快照文件记录的最后应用的日志的索引
    private long lastSnapshotIndex;
    
    private final Lock lock;
    
    private final RaftOptions raftOptions;

    //快照引用计数表，key是快照文件应用的最后一条日志的索引，value是对这个索引的引用计数
    private final ConcurrentMap<Long, AtomicInteger> refMap = new ConcurrentHashMap<>();


    //构造方法
    public LocalSnapshotStorage(String path, RaftOptions raftOptions) {
        this.path = path;
        //快照文件记录的最后日志索引初始值为0
        this.lastSnapshotIndex = 0;
        this.raftOptions = raftOptions;
        this.lock = new ReentrantLock();
    }


    //重构之后的close()方法

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/21
     * @方法描述：关闭快照存储器的方法，这个方法非常重要，该方法会被LocalSnapshotWriter快照写入器的close方法调用，而LocalSnapshotWriter的close方法会在快照生成之后被调用
     * 在这个方法中，会把快照临时快照文件移动到正式的快照文件中
     */
    void close(final LocalSnapshotWriter writer) throws IOException {

        //得到旧的快照文件应用的最后一条日志的索引
        final long oldIndex = getLastSnapshotIndex();
        
        //从快照写入器保存的快照元信息中得到新生成的快照文件应用的最后一条日志的索引
        final long newIndex = writer.getSnapshotIndex();
        
        //判断旧的快照文件应用的最后一条日志索引是否等于新快照文件的最后一条日志索引
        //如果相等，意味着快照生成出现问题了
        if (oldIndex == newIndex) {
            LOG.error("Fail to saveSnapshot");   
        }

        //在这里把本次生成快照的元数据写入到硬盘文件中，这里是同步写入
        writer.sync()
        
        //得到存放新生成的快照的临时文件夹的路径
        final String tempPath = this.path + File.separator + TEMP_PATH;

        //根据新生成的快照文件应用的最后一条日志索引得到存放新生成的快照的正式文件夹的路径
        final String newPath = getSnapshotPath(newIndex);
        
        //把新生成的快照文件从临时文件夹中移动到正式文件夹中
        Utils.atomicMoveFile(new File(tempPath), new File(newPath), true);

        //给最新的快照记录的应用日志的索引增加引用
        ref(newIndex);

        //这里上锁是为了保证领导者节点给跟随者节点安装快照时lastSnapshotIndex的正确性
        this.lock.lock();
        
        try {//校验一下旧的快照记录的日志索引是不是当前快照存储器之前记录的那个
            Requires.requireTrue(oldIndex == this.lastSnapshotIndex);
            //如果是就说明快照生成没问题，然后给lastSnapshotIndex赋最新值
            //更新快照文件应用的最后一条日志索引的值，也就是更新lastSnapshotIndex成员变量的值
            this.lastSnapshotIndex = newIndex;
        } finally {
            this.lock.unlock();
        }

        //减少旧的快照文件记录日志索引的引用，如果引用为0了，就可以直接删除该快照文件了
        unref(oldIndex);
    }
    

    //该类的其他内容暂时不做实现
}
```

我相信上面代码块中的内容大家都能看懂，现在大家感到困惑的是 LocalSnapshotWriter 快照写入器怎么得到快照元信息，这个功能其实我早就为大家实现了，只不过之前没有引入 LocalSnapshotWriter 快照写入器，所以我把相关代码省略了。实际上，**记录要生成的快照文件的元信息，以及把元信息交给 LocalSnapshotWriter 快照文件写入器的操作，就是在 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法中执行的**。具体实现请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    //用户真正定义的状态机
    private StateMachine fsm;

    //日志管理器组件
    private LogManager logManager;

    //省略该类其他内容

    
    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/21
     * @方法描述：生成快照的入口方法，该方法中的SaveSnapshotClosure参数对象，大家先不必关注，很快我就会为大家讲到
     */
    @Override
    public boolean onSnapshotSave(final SaveSnapshotClosure done) {
        //把生成快照的操作封装为任务发布到disruptor的环形数组中
        return enqueueTask((task, sequence) -> {
            task.type = TaskType.SNAPSHOT_SAVE;
            task.done = done;
        });
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    private void doSnapshotSave(final SaveSnapshotClosure done) {
        
        //对回调对象判空
        Requires.requireNonNull(done, "SaveSnapshotClosure is null");


        //生成快照元信息文件的操作在这里执行

        
        //在这里得到要生成新的快照文件时，状态机应用的最后一条日志的索引
        //其实这里得到的就是新生成的快照文件应用的最后一条日志索引
        final long lastAppliedIndex = this.lastAppliedIndex.get();
        //创建快照元信息构建器
        final RaftOutter.SnapshotMeta.Builder metaBuilder = RaftOutter.SnapshotMeta.newBuilder()
                //设置本次生成快照的元数据信息，本次快照要记录的最后一条日志索引其实就是当前状态机最新应用的日志的索引
                .setLastIncludedIndex(lastAppliedIndex)
                .setLastIncludedTerm(this.lastAppliedTerm);
        
        //下面就是把集群节点信息设置到元数据中了，这里就是从最新应用的日志条目中获得配置变更日志，这里我要多解释一下，实际上在ConfigurationManager类中根据索引获得小于该索引的最新配置条目
        //然后再从配置变更日志中获得集群配置信息
        final ConfigurationEntry confEntry = this.logManager.getConfiguration(lastAppliedIndex);
        if (confEntry == null || confEntry.isEmpty()) {
            LOG.error("Empty conf entry for lastAppliedIndex={}", lastAppliedIndex);
            ThreadPoolsFactory.runClosureInThread(getNode().getGroupId(), done, new Status(RaftError.EINVAL,
                    "Empty conf entry for lastAppliedIndex=%s", lastAppliedIndex));
            return;
        }
        for (final PeerId peer : confEntry.getConf()) {
            metaBuilder.addPeers(peer.toString());
        }
        for (final PeerId peer : confEntry.getConf().getLearners()) {
            metaBuilder.addLearners(peer.toString());
        }
        if (confEntry.getOldConf() != null) {
            for (final PeerId peer : confEntry.getOldConf()) {
                metaBuilder.addOldPeers(peer.toString());
            }
            for (final PeerId peer : confEntry.getOldConf().getLearners()) {
                metaBuilder.addOldLearners(peer.toString());
            }
        }


        //从回调对象中得到快照写入器，大家可以暂时先不思考怎么从回调对象中得到快照写入器的
        //先记住这一点即可，很快我就会为大家真正实现SaveSnapshotClosure回调对象了
        //在这里 done.start(metaBuilder.build())就会把快照元信息设置到快照写入器中
        //大家可以先记住这一点，后面会讲解实现原理
        final SnapshotWriter writer = done.start(metaBuilder.build());
        if (writer == null) {
            done.run(new Status(RaftError.EINVAL, "snapshot_storage create SnapshotWriter failed"));
            return;
        }
        
        //这里可以看到，快照生成的操作其实是在用户自己定义的状态集中实现的
        //状态机生成快照
        this.fsm.onSnapshotSave(writer, done);
    }

    //省略该类其他内容
}
```

看完了以上代码块之后，我想我们大家也就知道了快照文件元信息是怎么生成和保存的了。而当用户定义的 StateMachine 状态机执行了生成快照的操作之后，SaveSnapshotClosure 回调对象中的方法就会被回调，这个时候，如果我们并不在 SaveSnapshotClosure 回调对象中直接调用 LocalSnapshotStorage 快照存储器的 close() 方法，而是调用 LocalSnapshotWriter 快照写入器的 close() 方法，那么快照写入器的 close() 方法就会进一步调用 LocalSnapshotStorage 快照存储器的 close() 方法。这样一来，不仅可以把快照文件从临时文件夹中移动到正式文件夹中，还可以把快照元信息写入到本地文件中了。所以，现在大家应该清楚了，**当 FSMCallerImpl 状态机组件生成快照文件之后，我们不应该在 SaveSnapshotClosure 回调对象的方法中调用 LocalSnapshotStorage 快照存储器的 close() 方法，而是应该调用 LocalSnapshotWriter 快照写入器的 close() 方法**。大家可以仔细品味品味这个逻辑，如果这个流程的逻辑掌握了，那么接下来，大家就可以思考下一个问题，那就是 LocalSnapshotWriter 快照写入器是怎么被创建的。

这个问题也很容易解决，我们已经清楚了，LocalSnapshotWriter 快照写入器会持有存储快照文件的临时文件夹路径，而这个临时文件夹的路径是在 LocalSnapshotStorage 快照存储器中生成的，这也就意味着，创建 LocalSnapshotWriter 快照写入器的时候，LocalSnapshotStorage 快照存储器应该把自己持有的临时文件夹路径交给 LocalSnapshotWriter 快照写入器使用。既然是这样，那我直接在 LocalSnapshotStorage 快照存储器中定义一个新的方法，比如就定义为 creat() 方法，该方法就是用来创建 LocalSnapshotWriter 快照写入器的。也就是说，LocalSnapshotWriter 快照写入器是被 LocalSnapshotStorage 快照存储器创建的。我把对应的功能已经实现了，接下来请大家看一看再次重构之后的 LocalSnapshotStorage 类的代码，请看下面代码块。

```
package com.alipay.sofa.jraft.storage.snapshot.local;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/21
 * @方法描述：快照存储器
 */
public class LocalSnapshotStorage implements SnapshotStorage {

    private static final Logger LOG = LoggerFactory.getLogger(LocalSnapshotStorage.class);

    //存放快照文件的临时路径
    private static final String TEMP_PATH = "temp";

    //用户在程序启动时定义的存放快照的路径
    //其实就是/User/chenqingyang/code/jraft-log/server1/snapshot路径
    //只要把这个路径和上面的temp拼接起来，就可以得到存放临时快照文件的文件夹路径
    private final String path;

    //快照文件记录的最后应用的日志的索引
    private long lastSnapshotIndex;
    
    private final Lock lock;
    
    private final RaftOptions raftOptions;

    //快照引用计数表，key是快照文件应用的最后一条日志的索引，value是对这个索引的引用计数
    private final ConcurrentMap<Long, AtomicInteger> refMap = new ConcurrentHashMap<>();


    //构造方法
    public LocalSnapshotStorage(String path, RaftOptions raftOptions) {
        this.path = path;
        //快照文件记录的最后日志索引初始值为0
        this.lastSnapshotIndex = 0;
        this.raftOptions = raftOptions;
        this.lock = new ReentrantLock();
    }



     //创建快照写入器的入口方法
    @Override
    public SnapshotWriter create() {
        return create(true);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/21
     * @方法描述：真正创建快照写入器的方法
     */
    public SnapshotWriter create(final boolean fromEmpty) {
        LocalSnapshotWriter writer = null;
        do {
            //首先得到存储快照的临时路径，这里就可以看出来，每一次生成快照的时候，会先把快照存放到临时目录中
            //等快照生成完毕之后，才会把快照移动到正式的文件中，原因也很简单，就是不想让正在生成的快照对当前程序中的最新快照造成干扰
            //如果生成快照失败了，那么失败的快照也是在临时目录中，并没有在正式目录中
            final String snapshotPath = this.path + File.separator + TEMP_PATH;
            //上面已经得到了临时目录，然后就是判断临时目录是否存在，如果存在就删除
            //这里就是在每一次生成新快照之前，把旧的临时快照删除了的操作
            if (new File(snapshotPath).exists() && fromEmpty) {
                //在这里删除临时快照文件夹
                if (!destroySnapshot(snapshotPath)) {
                    break;
                }
            }
            //创建快照写入器，快照写入器持有的路径是临时路径
            //注意，这里创建快照写入器的时候，快照存储器把自己交给快照写入器使用了
            writer = new LocalSnapshotWriter(snapshotPath, this, this.raftOptions);
            //初始化快照写入器
            if (!writer.init(null)) {
                LOG.error("Fail to init snapshot writer.");
                writer = null;
                break;
            }
        } while (false);
        //返回快照写入器
        return writer;
    }
   
    

    //该类的其他内容省略
}
```

上面代码块展示的就是 LocalSnapshotStorage 快照存储器创建 LocalSnapshotWriter 快照写入器的具体功能，代码块中注释非常详细，我相信每一位朋友都能看懂，我就不再重复赘述了。当然，我能想到看到这个时候，大家可能已经头昏脑胀了。因为在这一章我又引入了新的组件，并且把一些组件又重构了一下，这就导致各个组件的依赖关系，以及回调方法中的逻辑更加复杂了，而 jraft 框架生成快照的功能始终没有实现完整，所以面对现在内容残缺的功能，仅凭文字讲解和代码展示，我相信这需要花费各位朋友很多时间，才能理解各个组件的作用和使用方式。为了帮助大家更好的掌握本章内容，接下来我想再花费点篇幅，为大家梳理一下目前程序的执行流程。当然，我会尽快把 sofajraft 框架生成快照文件功能的流程图画好，然后提供给大家，这一块的逻辑确实有点绕，也应该给大家提供清晰的流程图。

## 梳理本章程序执行流程

就目前的情况来说，我们已经引入了 LocalSnapshotStorage 快照存储器，引入了 LocalSnapshotWriter 快照写入器，引入了 FSMCallerImpl 状态机组件。我们目前知道的程序运行的内部原理是这样的：

**1 调用 LocalSnapshotStorage 快照存储器的 create() 方法，就可以创建一个 LocalSnapshotWriter 快照写入器，并且创建完毕的 LocalSnapshotWriter 快照写入器，可以得到 LocalSnapshotStorage 快照存储器的引用。**

**2 FSMCallerImpl 状态机组件在执行生成快照操作时，会执行它的 doSnapshotSave() 方法，在该方法中会先得到本次生成的快照的元信息，然后把元信息设置到 LocalSnapshotWriter 快照写入器中，这个时候，快照写入器就得到了本次生成的快照的元信息。然后用户定义的 StateMachine 状态机会执行真正生成快照文件的操作。**

**3 当 FSMCallerImpl 状态机组件执行完毕生成快照的操作之后，就会回调 SaveSnapshotClosure 对象中的方法，这个时候我们需要在 SaveSnapshotClosure 对象的回调方法中调用 LocalSnapshotWriter 快照写入器的 close() 方法。**

**4 当程序执行 LocalSnapshotWriter 快照写入器的 close() 方法时，就可以在该方法中继续调用 LocalSnapshotStorage 快照存储器的 close() 方法，并且在调用 LocalSnapshotStorage 快照存储器的 close() 方法时，会把已经得到了快照文件云信息的 LocalSnapshotWriter 快照写入器传递到 LocalSnapshotStorage 快照存储器的 close() 方法中。这样一来，再执行 LocalSnapshotStorage 快照存储器的 close() 方法的过程中，就可以把快照文件从临时文件夹移动到正式文件夹，然后调用 LocalSnapshotWriter 快照写入器的 sync() 方法，把快照文件元信息存储到本地文件中，最后操作成功之后，会视情况删除本地的旧快照文件**。

以上四个要点就是目前和 jraft 框架生成快照文件功能相关的四篇文章中的全部要点了。如果大家把上面四个要点的逻辑和流程全部掌握了，那么剩下的最后一点知识学起来就非常容易了。

到此为止，本章的内容也就结束了。就像本篇文章开头说的那样：虽然实现 sofajraft 框架生成快照功能的文章越写越多，实现的功能组件越来越多，也重构得越来越完善，也解决了一些问题，但是与此同时产生的问题也越来越多。所以，在文章的最后，我还是愿意把目前为止我们面临的问题汇总一下，这些问题就是下一篇文章中，我们要解决的问题，也是下一篇文章的核心内容，当然，这依然少不了要引入新的组件。在本篇文章开始，我为大家总结了 8 个本章要解决的问题，我把这几个问题再次搬运到下面了，让我们看看本章结束之后，这 8 个问题中的哪些问题被我们解决了，哪些问题仍然没有解决。

**1 同步或者异步生成快照的功能并没有在程序中体现出来，也就是说，并没有把同步或者异步生成快照的操作交给用户来选择。**

**2 快照定时器每一次要执行的定时任务，都必须真的执行吗？什么情况下可以不执行本次生成快照文件的定时任务呢？**

**3 生成了快照文件之后，那么与快照文件存储数据对应的日志条目应该怎么删除呢？**

**4 SaveSnapshotClosure 回调对象该怎么创建呢？**

**5 SaveSnapshotClosure 回调对象该怎么交给 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法使用？**

**6 LocalSnapshotStorage 快照存储器具体在哪里创建，初始化呢？**

**7 LocalSnapshotStorage 快照存储器的 close() 方法怎么在 SaveSnapshotClosure 对象的方法中被调用呢？**

**8 怎么把 LocalSnapshotStorage 快照存储器中的存储快照文件的临时文件夹路径交给 FSMCallerImpl 状态机组件使用呢**？

真的比较失败，因为在以上 8 个问题中，我发现这一章讲下来，我们似乎只把第 7、8 个问题解决了，剩下的 6 个问题全部没有解决。而且随着本章引入了新的组件，我发现到最后我们又多了三个新的问题：

**1 LocalSnapshotStorage 快照存储器的 create() 方法要在什么时候被调用呢？也就是说，LocalSnapshotWriter 快照写入器要在什么时候被创建呢？**

**2 LocalSnapshotWriter 快照存储器的 close() 方法怎么在 SaveSnapshotClosure 对象的方法中被调用呢？**

**3 LocalSnapshotWriter 快照写入器怎么才能交给 FSMCallerImpl 状态机组件要回调的 SaveSnapshotClosure 对象使用呢？因为在 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法中，我们是调用了 SaveSnapshotClosure 回调对象的 start() 方法才得到了 LocalSnapshotWriter 快照写入器。这就意味着 LocalSnapshotStorage 快照存储器创建了快照写入器后，要把快照写入器交给 SaveSnapshotClosure 回调对象使用。那怎么才能把 LocalSnapshotWriter 快照写入器交给 SaveSnapshotClosure 回调对象使用呢？**

我把相关代码再次搬运到下面了，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    //用户真正定义的状态机
    private StateMachine fsm;

    //日志管理器组件
    private LogManager logManager;

    //省略该类其他内容


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    private void doSnapshotSave(final SaveSnapshotClosure done) {

        //省略该方法部分内容

        
        //从回调对象中得到快照写入器，大家可以暂时先不思考怎么从回调对象中得到快照写入器的
        //先记住这一点即可，很快我就会为大家真正实现SaveSnapshotClosure回调对象了
        //在这里 done.start(metaBuilder.build())就会把快照元信息设置到快照写入器中
        //大家可以先记住这一点，后面会讲解实现原理
        final SnapshotWriter writer = done.start(metaBuilder.build());
        if (writer == null) {
            done.run(new Status(RaftError.EINVAL, "snapshot_storage create SnapshotWriter failed"));
            return;
        }
        
        //这里可以看到，快照生成的操作其实是在用户自己定义的状态集中实现的
        //状态机生成快照
        this.fsm.onSnapshotSave(writer, done);
    }

    //省略该类其他内容
}
```

所以，到目前为止，我们要解决的就变成了下面展示的九个问题：

**1 同步或者异步生成快照的功能并没有在程序中体现出来，也就是说，并没有把同步或者异步生成快照的操作交给用户来选择。**

**2 快照定时器每一次要执行的定时任务，都必须真的执行吗？什么情况下可以不执行本次生成快照文件的定时任务呢？**

**3 生成了快照文件之后，那么与快照文件存储数据对应的日志条目应该怎么删除呢？**

**4 SaveSnapshotClosure 回调对象该怎么创建呢？**

**5 SaveSnapshotClosure 回调对象该怎么交给 FSMCallerImpl 状态机组件的 doSnapshotSave() 方法使用？**

**6 LocalSnapshotStorage 快照存储器具体在哪里创建，初始化呢？**

**7 LocalSnapshotWriter 快照写入器要在什么时候被创建呢？**

**8 LocalSnapshotWriter 快照存储器的 close() 方法怎么在 SaveSnapshotClosure 对象的方法中被调用呢？**

**9 LocalSnapshotWriter 快照写入器怎么才能交给 SaveSnapshotClosure 回调对象使用呢？**

这就是我们最重要面临的 9 个问题，我相信如果大家阅读完本篇文章，或者说阅读完到目前为止和 jraft 框架生成快照功能相关的四篇文章，大家能把我提出这几个问题的原因掌握了，这也就意味着大家把这几篇文章实现的功能，讲解的知识都掌握了。因为思考是建立在理解的基础上，当你理解了你在思考什么，你就知道你要解决什么了。好了朋友们，本行的内容到这里就结束了。下一章我会为大家引入一个新的组件，确切地说，就差引入最后一个组件，sofajraft 框架生成快照的功能就实现完整了。并且这个组件会在 NodeImpl 类中被用到，这时候我想再提醒大家一句，虽然后面更新的这几篇文章都是在重构 LocalSnapshotStorage 快照存储器，LocalSnapshotWriter 快照写入器，以及 FSMCallerImpl 状态机组件，但是大家千万别忘了 jraft 框架生成快照文件功能的源头，别忘了我们最初在 NodeImpl 类中定义了一个快照生成定时器，启动了定时器之后，raft 集群节点才会定期执行生成快照的操作。我把相关的代码块再次搬运过来了，帮助大家回顾一下之前的内容，请看下面代码块。

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

下一章，我们将会回到最初的起点，也就是重新回到 NodeImpl 类中，引入新的组件，然后对 NodeImpl 类的部分内容进行重构，等这一切都实现完毕了，sofajraft 框架生成快照文件的功能也就实现完毕了。好了朋友们，我们下一章见！