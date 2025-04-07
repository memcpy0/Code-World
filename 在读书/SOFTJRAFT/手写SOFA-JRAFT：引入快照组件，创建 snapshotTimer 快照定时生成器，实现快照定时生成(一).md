在实现了 jraft 框架的线性一致读功能之后，这一章让我们再次回到数据备份的领域，一起来看看 Raft 集群中数据快照是怎么生成，以及怎么安装给集群的其他节点的。说到快照，我相信很多朋友对此应该都有一个清楚的认知了，**相对于日志来说，快照文件使用的存储空间会更小一点，因为日志存储的是对数据的操作记录，而快照存储的是数据当前的值**。比如说现在有一个 kv 数据库，用户向数据库中存放了一个键值对，请看下面代码块。

```
kv.put("球星","罗纳尔迪尼奥")
```

如果用户使用 kv 数据库的客户端向 kv 数据库存储上面的键值对，那这中间的操作肯定是：**客户端会向 kv 数据库服务端发送一个请求，并且把操作符和要操作的数据封装在请求中，所谓操作符就是 PUT，或者 ADD，而这个请求被 kv 数据库领导者节点接收到之后，肯定会被封装到日志中，领导者会把这条日志传输给集群中的其他跟随着节点，当这条日志在集群中达成共识之后，也就是日志在集群的大多数节点中落盘成功了，领导者的状态机就会解析应用这条日志，从日志中得到客户端发送过来的请求，然后从请求中得到要执行的操作符以及要操作的数据，最后把数据应用到状态机，也就是把 kv 数据添加到数据库中**。这个流程应该还算清晰，如果大家对这个流程感到模糊，可以去看一看已经完结的 nacos 课程的构建 jraft 集群的章节，在那些章节中，我为大家详细展示了 nacos 客户端发送给服务端的持久服务实例是如何经过 jraft 集群的处理，然后存储到 nacos 注册中心服务端全流程的。总之，通过刚才的分析，大家肯定可以清楚地意识到，**日志记录的并不是数据本身，而是要对数据执行的操作的痕迹，通过 jraft 框架记录的每一条日志，我们可以知道要对哪些数据执行什么操作**，假如某个节点宕机了，那么重新启动之后，可以凭借自己持久化到本地的日志，重新对这些数据执行对应的操作，这样一来数据不就恢复了吗？这就是 jraft 框架可以备份数据的核心原理。

当然，在体会到 jraft 框架日志备份数据功能好处的同时，我们也应该看到这个功能的缺陷，**服务器的容量肯定是有限的，也就是说，服务器能够存储的数据是有限的，不可能存储无限的日志**。如果大家已经看完了 hippo4j 动态线程池这个框架，那么大家就应该知道，就连这么小的框架还会定期删除线程池配置历史信息日志，以便恢复硬盘空间，更别说核心功能就是依靠日志来实现数据备份的 jraft 框架了。jraft 框架部署的服务器要存储非常多的日志，如果不对日志进行及时处理，那么越来越多的日志肯定会把硬盘空间消耗完毕，到时候服务器就无法存储新的日志了。所以，为了避免出现这种情况，**我们也应该对 jraft 框架持久化到本地的日志进行定期处理，定期清除一部分日志，好腾出硬盘空间存储新的日志**。但是，问题也就出现在这里，如果我们要定期清除 jraft 框架持久化到本地的部分日志，那么应该清除哪一部分日志呢？

这是个比较棘手的问题，按照常规逻辑来说，我们在清除某些日志信息时，肯定会定义一个过期时间，把该时间之前的信息都当作无用的历史信息直接清除了，但是在 jraft 框架中，每一条日志都代表要对某些数据执行的重要操作，换句话说，每一条日志都代表某些数据，我们不可能根据时间就断定哪些数据真的没有用了，哪些数据真的过期了。所以，我们就无法根据时间来定期清除 jraft 框架持久化到本地的日志。当然我们也不必担心，除了这个方向，我们还有其他的方法可以考虑，要节省硬盘的空间，并不一定要直接干脆地把数据删除，只要能让可用空间变大就行。就比如说，我们可以对日志信息进行压缩，之前存储的所有日志使用了 1G 的空间，经过压缩之后，存储这些日志只使用了 700MB 空间，这样一来不就节省了硬盘空间吗？

当然，以上只是一个可行的思路，并不意味着我们在本章真的要这么做，因为我们还有一个更好的方法节省硬盘空间。在文章一开始我就跟大家分析了，**jraft 框架中每一条日志存储的都是对数据的操作记录，日志内容包含操作符和要操作的数据本身**，如果只从日志本身包含的内容来看，我肯定不会觉得日志内容有些冗余，恰恰相反，我认为日志存储的都是至关重要的信息，缺失了哪一条信息，都会造成备份数据的不准确。所以就日志本身的内容来说，完全没有需要删减的地方。但是，如果从另外的角度来思考，就会觉得用日志存储对数据的操作记录的方式确实有些啰嗦，就比如说 kv 数据库的客户端把一个键值对添加到数据库中了，这个时候会产生一条日志；然后客户端又把这个键值对从数据库中移除了，这个时候又会产生一条日志；过了一会客户端把这个键值对又添加到数据库中了，这时候又会产生一条日志。也就是说，客户端向数据库存储了一个 kv 键值对，不管中间操作了几次吧，反正最后只有这个键值对存储在数据库中，按说只需要一条日志来记录这个数据信息即可，但是最后却产生了三条日志。这个时候，我就会觉得另外的两条日志是冗余的，虽然记录了对数据的操作记录，但是却没什么实际效果。假如说当前部署了 kv 数据库服务器的节点宕机了，那么服务器重启之后要恢复数据，在应用日志的时候会把这个键值对先添加到数据库中，然后再删除，然后再添加，这显然是在做无用功。倒不如直接就添加一次来得直接，这个弊端大家可以理解吧，**也就是说使用日志方式本分数据的话，也许会记录很多冗余操作**？如果大家理解了这一点，那么接下来我们就可以让快照功能登场了。

在文章开头我也跟大家说了，**所谓快照文件存储的就是数据本身。也就是说当前 kv 数据库中存储了什么数据，那么快照文件就存储了什么数据**。这样一来，我们其实就可以这么做，如果当前节点存储了 10 条日志，最后一条有效日志的索引就是 10，不管这 10 条日志对数据执行了什么操作，总之，这 10 条日志被状态机应用后，不管哪些数据是先被添加，又被移除，又被添加等等，kv 数据库在当下存储的数据肯定是确定的，也是最新的。比如说 kv 数据库现在就存储了一下三条数据，请看下面代码块。

```
kv.put("球星","罗纳尔迪尼奥")
kv.put("游戏","黑悟空")
kv.put("傻子","小仙女")
```

**既然 kv 数据库在当下存储的数据肯定是确定的，那我就可以直接把数据库中当前的数据持久化到本地，也就是为这些数据生成快照文件。在生成了快照文件之后，我就可以把这三条数据对应的 10 条日志删除了**。这样一来，我们不就使用最少的空间，存储最新的数据了吗？**如果当前节点忽然宕机了，那么只要把快照文件中的内容 put 到 kv 数据库中不就行了**？数据就这样被恢复了，**这显然比应用一条条日志恢复数据速度快很多，效率也高很多**，就是这么简单。这就是快照文件在 raft 集群中发挥的重要作中之一。好了朋友们，现在快照文件的作用，以及日志方式的弊端我都已经分析完毕了，接下来我们就该真正实现这个快照功能了。

当然，我们在实现一个功能，尤其是一个非常重要的功能时，不能直接开始盲目地编写代码，而是应该先分析一下 raft 集群快照文件功能的实现思路，也就是说要先明确实现该功能的几个方向，以及在实现该功能的过程中应该注意什么，把所有细节问题都考虑到。

## 引入 snapshotTimer 快照生成定时器

比如说我首先就意识到如果我们要为 jraft 集群的节点生成快照文件，那么这个快照文件肯定不能只生成一次，原因很简单，**因为 kv 数据库中的数据肯定会被不断更新**，当前时间下，kv 数据库中存储了 100 条数据，也许 3 个小时之后，kv 数据库中存储的数据就更新成 300 条了。所以，**为 jraft 集群的节点生成快照文件的操作应该被定义为一个定时任务，也就是说，要定期为当前节点存储的数据生成快照**。当然了，每生成一次快照文件，那么这些快照存储的数据对应的日志就可以被删除了，这个逻辑可以理解吧？如果这个逻辑大家都理解了，接下来就可以着手实现一下，因为这个逻辑的核心就是定义一个定时任务，没有一点难度，所以我想先把这个定时任务定义出来，也算是为我们的工作起了一个好的开端。根据之前章节的学习，我们已经知道了，**jraft 框架中的很多定时任务都定义在了 NodeImpl 类中了，并且在 jraft 集群节点启动的过程中，定时任务就被启动了，也就是在 NodeImpl 类的 init() 方法中，定时任务就被启动了**。这一点想必大家都很熟悉了，那现在我们要为生成快照的操作定义定时任务了，**这个快照生成定时任务也可以定义在 NodeImpl 类中，并且也可以在该类的 init() 方法中启动**。**当然，启动了不一定意味着这个定时任务就要被执行，如果没有可以生成快照的数据，那就可以直接退出这个定时任务了**。这个逻辑也很容易理解吧？如果大家理解了这个逻辑，那接下来我们就从代码层面上实现一下快照生成定时任务。

**我把这个定时任务定义为 NodeImpl 类的成员变量了，其实就是一个 RepeatedTimer 定时器，我把它定义为 snapshotTimer 了**，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/21
 * @Description:整个框架中最核心的类，关于节点的一切功能几乎都定义在这个类中了，不是在该类中直接实现
 * 就是由该类的某个方法做了一层代理，总之，都会通过这个类的对象去执行操作，这个类的名字也很直接，就是节点实现的意思
 * 在分布式集群中，每一个服务器都会被抽象为一个节点，而节点的功能就是当前类提供的，在第一版本我只为大家引入了领导选举的功能
 * 实际上，该类有近4000行代码，在第一版本我为大家删去了近3000行，删去的内容后面会逐渐补全
 */
public class NodeImpl implements Node, RaftServerService {

    //省略其他成员变量
    
    //快照生成定时器
    private RepeatedTimer snapshotTimer;


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

        //启动快照定时器，快照定时器一旦启动，就会定期执行快照生成的定时任务
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

        //该方法暂时不做实现
    }

    //省略该类的其他方法
}
```

如果大家对 RepeatedTimer 定时器的内容还有印象，那大家肯定就知道，其实这个定时器内部使用了 netty 的时间轮，也就是说，我们定义的这个快照生成任务会被 netty 的时间轮执行。关于这部分内容我就不再带领大家回顾了，如果有朋友忘记这一部分的内容了，可以回顾回顾前面的章节。总之通过上面代码块我们可以看到，我们在 NodeImpl 类中定义了一个 snapshotTimer 快照生成定时器，这个定时器会定时执行它的 onTrigger() 方法，也就是说，onTrigger() 方法会被定时器定时执行；而在该方法中，我又定义了一个 handleSnapshotTimeout() 方法，这个方法就是用来生成快照文件的入口方法**。而在 handleSnapshotTimeout() 方法中，我们可以看到，我使用了一个新的线程，让新的线程执行了一个 doSnapshot() 方法，而 doSnapshot() 方法就是生成快照文件的方法**，当然该方法我还没有实现，这个方法后面再分析。并且在注释中我也跟大家解释了，之所以使用新的线程执行 doSnapshot() 方法，**是因为 RepeatedTimer 定时器中的任务是被时间轮线程执行的，生成快照文件的操作比较复杂，也比较耗时，如果使用时间轮线程直接执行生成快照的操作，难免会影响时间轮线程执行定时任务的时间精准度**。这个逻辑大家应该可以理解吧？总之这番分析下来，我们最终可以明确一点，**那就是生成快照文件的 doSnapshot() 方法会被 RepeatedTimer 定时器定期执行**，这样一来，我们要定义的生成快照的定时任务也就定义成功了。大家可以再思考思考这个流程，如果大家理解我们定义的这个快照生成定时任务的执行逻辑，那么接下来，我们就可以来研究一下这个 doSnapshot() 方法应该如何实现了。

## 分析同步、异步生成快照文件的情况

在分析该方法如何实现之前，请大家先思考一下，**假如现在让我们抛弃一切操作流程，只关注快照生成的本质，那我们应该怎么把数据以快照的方式存储到硬盘本地呢**？就比如说还是一个 kv 数据库吧，数据库中有 3 条数据，就是下面代码块中展示的这三条数据，请看下面代码块。

```
kv.put("球星","罗纳尔迪尼奥")
kv.put("游戏","黑悟空")
kv.put("傻子","小仙女")
```

好了朋友们，假如说现在我们就关注怎么把 kv 数据库中的数据以快照的方式存储到本地文件中，那该怎么做呢？这时候我们就应该意识到，**既然是以纯粹的快照的方式，而快照文件就是对数据本身的记录**，那这就很好办了，直接把数据库中的数据写入到文件中不就行了？我给大家提供了一个测试类，请大家从代码层面上看看该操作是怎么执行的，请看下面代码块。

```
public class Test{

    public static void main(String[] args) {
        //kv数据库中的数据存储在map中
        Map<String, String> kv = new HashMap<>();
        kv.put("球星","罗纳尔迪尼奥")
        kv.put("游戏","黑悟空")
        kv.put("傻子","小仙女")

        //得到快照文件路径
        String filePath = "D:/Users/陈清风扬/code/jraft-log/server1";
        
        //得到map快照文件写入器
        MapSnapshotFile snapshot = new MapSnapshotFile(writer.getPath() + File.separator + "data");
        //把map中的数据写入到快照文件中
        snapshot.save(kv);
    }
    
}



public class MapSnapshotFile {
    
    private static final Logger LOG = LoggerFactory.getLogger(MapSnapshotFile.class);
    private String path;

    public MapSnapshotFile(String path) {
        this.path = path;
    }

    public String getPath() {
        return this.path;
    }


    //把map保存到指定文件中的方法
    public boolean save(Map<String, String> values) {
        try {//CommandCodec.encodeCommand(values)会对这个map进行编码
            FileUtils.writeByteArrayToFile(new File(path), CommandCodec.encodeCommand(values));
            return true;
        } catch (IOException e) {
            LOG.error("Fail to save snapshot", e);
            return false;
        }
    }


    //从指定文件读取快照内容到内存中的方法
    @SuppressWarnings("unchecked")
    public Map<String, String> load() throws IOException {
        byte[] bs = FileUtils.readFileToByteArray(new File(path));
        if (bs != null && bs.length > 0) {
            return CommandCodec.decodeCommand(bs, Map.class);
        }
        throw new IOException("Fail to load snapshot from " + path + ",content: " + Arrays.toString(bs));
    }
}




public class CommandCodec {
   
    public static byte[] encodeCommand(Object obj) {
        try {
            return SerializerManager.getSerializer(SerializerManager.Hessian2).serialize(obj);
        } catch (final CodecException e) {
            throw new IllegalStateException(e);
        }
    }

    
    public static <T> T decodeCommand(byte[] content, Class<T> clazz) {
        try {
            return SerializerManager.getSerializer(SerializerManager.Hessian2).deserialize(content, clazz.getName());
        } catch (final CodecException e) {
            throw new IllegalStateException(e);
        }
    }
}
```

上面代码块中的内容非常简单，就是把存储键值对的 map 编码，然后写入到本地快照文件中。这些操作执行下来，我们就已经把 kv 数据库中的数据存储在快照文件中了，并且存储的就是数据本身，只要这些数据被存储到快照文件内，那么这些数据对应的日志信息就可以被删除了。这些逻辑大家应该都已经非常清楚了。

很好，讲到这里，那我就要提出一个新的问题了，那就是我们快照文件是要在 raft 集群中生成的，上面代码块中展示的内容，其实就是把一个 map 存储到本地文件中的操作，这个操作过于简单和普通了。当然，**把 raft 集群中的数据以快照的方式存储在本地文件中的操作的本质和上面代码块中展示的内容确实没什么区别**，但是，让我们大家来思考一下，**我们怎么才能得到 raft 集群中的数据呢？要想把数据以快照的方式存储来本地，肯定要先得到 raft 集群存储的数据本身吧**？在上面的代码块中，我们直接创建了一个 map 作为 kv 数据库，所以可以直接得到数据库中存储的所有数据，那在 raft 集群中，怎么做才能顺利得到 raft 集群存储的数据呢？

也许大家对这个问题没什么头绪，没关系，接下来让我来为大家举一个简单的例子，仔细分析一下。假如说现在有一个内存数据库，就是类似于 redis 的内存数据库，是以键值对的方式存储数据的，其本质就是把数据存储在一个 map 中，这个大家可以理解吧？我们使用 sofajraft 框架为这个内存数据库构建了一个 raft 集群，当整个集群开始运转，开始对外提供服务后，用户使用内存数据库的客户端向 raft 集群的领导者发送了一个指令，要把一个键值对存储到数据库中。当 raft 集群领导者接收到客户端发送过来的指令后，肯定会把指令包装成日志，然后把日志传输给集群的其他节点，等这条日志在集群中达成共识了，**raft 集群的领导者会首先把这条指令应用到状态机上，而在状态机应用指令的时候，就会把指令中的数据 put 到内存数据库的 map 中**。这个流程大家可以理解吧？在之前我为大家实现 jraft 框架的状态机功能时，我就跟大家说过，**raft 集群的状态机就是衔接集群和业务功能的重要组件**，**在状态机，数据就会从集群层面被转移到所谓的业务层面，在我们这个例子中，就是数据库层面，数据会在状态机中存储到内存数据库中**。由此可见，**在 raft 集群中，通过状态机，肯定可以获得内存数据库中的所有数据，因为是状态机把所有数据应用到内存数据库中的，那么状态机肯定可以得到内存数据库，既然可以得到内存数据库，那就肯定可以得到内存数据库中的所有数据**。这个逻辑应该也很清晰了吧？所以，**我们只要在状态机中执行生成快照的操作，那就可以得到 raft 集群中的所有数据，然后把数据存储到本地的快照文件中**。和之前一样，我也为我们刚才分析的内容准备了一个测试类，请看下面代码块。

```
//jraft集群中的状态机，必须继承StateMachineAdapter状态机适配器
//而StateMachineAdapter状态机适配器则实现了StateMachine状态机接口
public class MapStateMachine  extends StateMachineAdapter{

    private static final Logger LOG = LoggerFactory.getLogger(MapStateMachine.class);

    //这个就是内存数据库存储数据的map
    private HashMap<String,String> map = new HashMap<>();


    //状态机应用日志的方法，在这个方法中，大家可以看到状态机把kv键值对存储到map成员变量中了
    @Override
    public void onApply(final Iterator iter) {
        //注意，这里实际上调用的是IteratorWrapper的hasNext方法
        //该方法会判断要处理的日志条目是不是EnumOutter.EntryType.ENTRY_TYPE_DATA类型的，如果不是就不会进入循环
        //由此可见，迭代器在真正的状态机中会把业务日志全部应用到状态机中
        //只有遇到非业务日志了，才会退出该方法，在FSMCallerImpl的doCommitted方法中继续之前的操作
        while (iter.hasNext()) {
            final Closure done = iter.done();
            final ByteBuffer data = iter.getData();
            String string = Charset.defaultCharset().decode(data).toString();
            if (done == null) {
                //走到这里意味着是跟随者将日志应用到状态机上，这里我跟大家解释一下，这个其实是第七版本代码
                //实现的状态机的测试类，我在这个测试类中其实只把kv键值对的value封装到日志中了，key是我自己写的
                //这么做是为了方便测试，大家明白这个意思即可，在真正的开发中，肯定是要把kv键值对一起封装到日志中的
                map.put("日志",string);
                System.out.println("领导者应用了日志==========================="+map.get("日志"));
            } else {
                //走到这里意味着是领导者将日志应用到状态机上
                map.put("日志",string);
                done.run(Status.OK());
                System.out.println("跟随者应用了日志==========================="+map.get("日志"));
            }//获取下一个日志条目
            iter.next();
        }
    }



    //这个方法就是把内存数据库map中的数据存储到本地快照文件中的方法
    //该方法本来就是状态机接口StateMachine中定义的方法
    //因为状态机使用户自己实现的，所以该方法也是用户自己实现的，也就是说用户可以自己在状态机中
    //定义生成快照文件的逻辑
    @Override
    public void onSnapshotSave() {
        //得到map快照文件写入器
        MapSnapshotFile snapshot = new MapSnapshotFile(writer.getPath() + File.separator + "data");
        //把map中的数据写入到快照文件中
        snapshot.save(map);
    }
    
    
}
```

阅读了上面的代码块之后，现在大家应该彻底清楚了吧？**生成快照的操作其实本来就定义在状态机中**，好了，绕了一个大圈子，兜兜转转，最后还是要使用状态机执行生成快照的操作呀。**如果我们可以直接得到用户为 jraft 集群的节点定义的 StateMachine 状态机对象，并且用户也实现了状态机的 onSnapshotSave() 方法，那么我们就可以直接调用 StateMachine 的 onSnapshotSave() 方法生成快照文件**。但是，必须强调的一点就是，**虽然 jraft 集群节点的状态机需要用户自己定义，但是用户定义的状态机要交给 FSMCaller 状态机管理器使用才能真正发挥作用**，就向下面代码块展示的这样，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    private static final Logger LOG = LoggerFactory.getLogger(FSMCallerImpl.class);

    //用户真正定义的状态机
    private StateMachine fsm;

    //省略其他成员变量


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：初始化状态机组件的方法，这个方法就不添加注释了，因为目前为止已经见过很多这样的方法了，日志组件初始化的逻辑也是这一套，状态机组件初始化的逻辑也是如此
     * 都是设置一些最初的配置参数，然后创建disruptor框架，启动disruptor框架
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean init(final FSMCallerOptions opts) {
    
        this.fsm = opts.getFsm();

        //省略该方法的其他内容
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：将业务日志应用到状态机上的方法
     */
    private void doApplyTasks(final IteratorImpl iterImpl) {
        //这里对日志迭代器做了点简单的包装，其实就是可以获得真正的有效日志了，也就是data部分
        //这部分解码之后们就是用户的操作执行
        final IteratorWrapper iter = new IteratorWrapper(iterImpl);
        final long startApplyMs = Utils.monotonicMs();
        final long startIndex = iter.getIndex();
        try {//在这里将业务日志应用到状态机上，同时也会把日志对应的回调方法在状态机中回调
            //这部分逻辑是用户自己实现的
            this.fsm.onApply(iter);
        } finally {
            this.nodeMetrics.recordLatency("fsm-apply-tasks", Utils.monotonicMs() - startApplyMs);
            this.nodeMetrics.recordSize("fsm-apply-tasks-count", iter.getIndex() - startIndex);
        }
        if (iter.hasNext()) {
            LOG.error("Iterator is still valid, did you return before iterator reached the end?");
        }
        //得到下一个日志条目
        iter.next();
    }


    //省略该类的其他内容
}
```

通过上面代码块，我们可以看到，**只要调用了 FSMCallerImpl 状态机组件的 doApplyTasks() 方法，就可以把日志应用到状态机上，这也就意味着数据就会被存储到 kv 数据库中了**。现在请大家想一想，如果用户定义的 StateMachine 状态机只能通过 FSMCallerImpl 对象使用，**那如果我们在 FSMCallerImpl 类中定义一个 onSnapshotSave() 方法，并且在该方法中调用 StateMachine 状态机的 onSnapshotSave() 方法，这样一来，不就可以直接通过 FSMCallerImpl 状态机组件生成快照文件了吗**？就像下面代码块中展示的这样，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    private static final Logger LOG = LoggerFactory.getLogger(FSMCallerImpl.class);

    //用户真正定义的状态机
    private StateMachine fsm;


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    @Override
    private void onSnapshotSave() {

         this.fsm.onSnapshotSave();
    }


    //省略该类的其他内容
}
```

在阅读了上面代码块的内容之后，现在我们应该意识到了，**如果用户定义的 StateMachine 状态机对象必须要通过 FSMCallerImpl 才能发挥作用，那我们就可以在 FSMCallerImpl 类中定义一个 onSnapshotSave() 方法，然后在该方法中调用 StateMachine 状态机对象的 onSnapshotSave() 方法，这样一来，我们就可以直接使用 FSMCallerImpl 状态机组件生成快照文件了，也就是说，只要调用 FSMCallerImpl 对象的 onSnapshotSave() 方法，就可以为 jraft 集群的节点生成快照文件**。这个逻辑应该很好理解吧？如果大家理解了这个逻辑，那接下来就很好说了。现在比较巧的是，**在 jraft 框架中，FSMCallerImpl 状态机组件被定义在 NodeImpl 类中了，通过 FSMCallerImpl 就可以使用 StateMachine 状态机，而这个 FSMCallerImpl 对象恰好就是 NodeImpl 类的成员变量**，这不就意味着我们可以直接在 NodeImpl 类中使用它吗？而我们要在 NodeImpl 类中使用 FSMCallerImpl 状态机组件的逻辑非常简单，**因为我们之前定义在 NodeImpl 类中的 doSnapshot() 方法还没有实现呢**，我把相关代码再次搬运到这里了，请看下面代码块。

```
package com.alipay.sofa.jraft.core;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/21
 * @Description:整个框架中最核心的类，关于节点的一切功能几乎都定义在这个类中了，不是在该类中直接实现
 * 就是由该类的某个方法做了一层代理，总之，都会通过这个类的对象去执行操作，这个类的名字也很直接，就是节点实现的意思
 * 在分布式集群中，每一个服务器都会被抽象为一个节点，而节点的功能就是当前类提供的，在第一版本我只为大家引入了领导选举的功能
 * 实际上，该类有近4000行代码，在第一版本我为大家删去了近3000行，删去的内容后面会逐渐补全
 */
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

        //该方法暂时不做实现
    }

    //省略该类的其他方法
}
```

通过之前的分析，**我们知道了只要调用 FSMCallerImpl 对象的 onSnapshotSave() 方法，就可以为 jraft 集群的节点生成快照文件**，那我们直接在 NodeImpl 类的 doSnapshot() 方法中调用 FSMCallerImpl 对象的 onSnapshotSave() 方法不就行了？就像下面代码块展示的这样，请看下面代码块。

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

可以看到，我们直接在 NodeImpl 类的 doSnapshot() 方法中调用了状态机组件的 onSnapshotSave() 方法，因为 doSnapshot() 方法会被快照生成定时器定时调用，而 状态机组件的 onSnapshotSave() 方法会生成快照，所以现在定时生成快照文件的功能就实现完毕了。大家可以仔细品味品味这个逻辑，当然，我相信大家仔细思考以上实现的功能逻辑之后，很快就会发现一个问题：**那就是 FSMCallerImpl 状态机组件执行生成快照文件时要考虑同步操作和异步操作的情况**。我知道有些朋友可能对这个问题感到困惑，接下来就让我来为大家解释一下。

如果我们仅仅关注使用 FSMCallerImpl 状态机组件执行生成快照文件的操作，那么似乎没有什么问题需要考虑，但是通过前面章节对 FSMCallerImpl 状态机组件的实现和学习，我们已经清楚地知道了，这个 FSMCallerImpl 状态机组件会把 jraft 集群中的日志应用到用户定义的状态机上，并且，**在 FSMCallerImpl 状态机组件内应用了 Disruptor 框架的单线程批处理器来把集群中的日志应用到状态机上。也就是说，在 FSMCallerImpl 状态机内部有一个单线程在不断地工作，把日志应用到用户定义的状态机上**，就像下面代码块展示的这样，我把相关的代码搬运到下面，帮助大家回顾一下曾经的内容，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    private static final Logger LOG = LoggerFactory.getLogger(FSMCallerImpl.class);

    //用户真正定义的状态机
    private StateMachine fsm;

    //状态机执行任务的线程
    private volatile Thread fsmThread;

    //disruptor启动器
    private Disruptor<ApplyTask> disruptor;
    
    //disruptor的环形数组
    private RingBuffer<ApplyTask> taskQueue;

    //省略其他成员变量


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：初始化状态机组件的方法，这个方法就不添加注释了，因为目前为止已经见过很多这样的方法了，日志组件初始化的逻辑也是这一套，状态机组件初始化的逻辑也是如此
     * 都是设置一些最初的配置参数，然后创建disruptor框架，启动disruptor框架
     */
    @SuppressWarnings("unchecked")
    @Override
    public boolean init(final FSMCallerOptions opts) {
    
        this.fsm = opts.getFsm();

        //省略该方法的其他内容
    }


     //事件工厂，该工厂会创建ApplyTask，然后将ApplyTask放到disruptor框架的环形数组中
    private static class ApplyTaskFactory implements EventFactory<ApplyTask> {

        @Override
        public ApplyTask newInstance() {
            return new ApplyTask();
        }
    }


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：要提交给disruptor框架的异步任务
     */
    private static class ApplyTask {
        //任务类型
        TaskType type;
        //提交的日志ID
        long committedIndex;
        //任期
        long term;
        Status status;
        LeaderChangeContext leaderChangeCtx;
        //要回调的方法
        Closure done;
        CountDownLatch shutdownLatch;

        public void reset() {
            this.type = null;
            this.committedIndex = 0;
            this.term = 0;
            this.status = null;
            this.leaderChangeCtx = null;
            this.done = null;
            this.shutdownLatch = null;
        }
    }
    

    //事件处理器，也就是disruptor框架要用的消费者处理器
    private class ApplyTaskHandler implements EventHandler<ApplyTask> {
        boolean firstRun = true;
        //这个是最大的可以应用的日志的索引，初始化为-1，随着日志不断被应用，这个值也会被不断更新
        //注意，这个是消费者处理器中的成员变量，消费者处理器会处理所有要被应用的日志，所以这个成员变量是被所有日志共享的
        //当然，这个共享也是有限制了，就是被同一批处理的日志共享，在每一批新的日志应用的时候，这个值会被重置
        private long maxCommittedIndex = -1;

        @Override
        public void onEvent(final ApplyTask event, final long sequence, final boolean endOfBatch) throws Exception {
            //在这里给FSMCallerImpl类的fsmThread成员变量赋值
            setFsmThread();
            //在这里执行每一个异步任务，这个runApplyTask方法就是把日志应用到状态机上的入口方法
            this.maxCommittedIndex = runApplyTask(event, this.maxCommittedIndex, endOfBatch);
        }

        private void setFsmThread() {
            if (firstRun) {
                //在这里给状态机的线程赋值了，其实就是disruptor中的线程
                fsmThread = Thread.currentThread();
                firstRun = false;
            }
        }
    }




    //该方法就是把已经提交成功的日志，应用到状态机中的入口方法
    //在BallotBox的commitAt方法中调用的就是下面这个方法
    @Override
    public boolean onCommitted(final long committedIndex) {
        return enqueueTask((task, sequence) -> {
            //设置为COMMITTED类型
            task.type = TaskType.COMMITTED;
            //committedIndex是从BallotBox类的commitAt方法中传递过来的参数
            //就是最新被提交的日志的索引
            task.committedIndex = committedIndex;
        });
    }


    //该方法会把生产者数据存放到disruptor的环形队列中
    private boolean enqueueTask(final EventTranslator<ApplyTask> tpl) {
        //在这里把要被应用的这批日志提交到disruptor的环形数组中
        this.taskQueue.publishEvent(tpl);
        return true;
    }


    //本类中的核心方法，也是disruptor框架的消费处理器中要调用的方法
    //在第七版本只会触发COMMITTED事件，所以我们就只看这个
    //这里我要强调一下，文章中的这个方法是最简化的，我提供的第七版本代码中的该方法比文章中
    //展示的方法复杂了很多，大家掌握了实现原理，更多的细节可以去第七版本代码中仔细学习
    @SuppressWarnings("ConstantConditions")
    private long runApplyTask(final ApplyTask task, long maxCommittedIndex, final boolean endOfBatch) {

        //在这里判断task是否为COMMITTED，如果是的话，就给maxCommittedIndex赋值
        if (task.type == TaskType.COMMITTED) {
            if (task.committedIndex > maxCommittedIndex) {
                //这时候maxCommittedIndex就不是-1了
                maxCommittedIndex = task.committedIndex;
            }
            task.reset();
        } 
        
        else {//执行到这里，意味着当前提交的异步任务并不是COMMITTED类型的
            //但是要判断一下当前的批处理中的maxCommittedIndex是否大于0，如果大于0说明现在有日志要被应用到状态机上
            //所以尽管要处理的是其他类型的任务，也要先把日志应用到状态机上
            if (maxCommittedIndex >= 0) {
                this.currTask = TaskType.COMMITTED;
                //在这里执行真正的提交日志的方法
                doCommitted(maxCommittedIndex);
                //重置maxCommittedIndex的值
                maxCommittedIndex = -1L;
            }
        
            //其他逻辑省略
        }

        
        //在这里会有一个总的判断，判断当前要消费的数据是不是本批数据中的最后一个，这是disruptor框架中的知识
        //如果是最后一个，并且maxCommittedIndex大于0，说明肯定有一些日志要被应用到状态机上
        //那就直接应用这批日志即可，如果不是本批数据中的最后一个，那就什么也不做
        //除非有其他非COMMITTED类型的任务被提交了，这时候就会在上面的else分支中把还未应用的日志都应用到状态机上
        if (endOfBatch && maxCommittedIndex >= 0) {
            this.currTask = TaskType.COMMITTED;
            //应用日志到状态机上
            doCommitted(maxCommittedIndex);
            //重置maxCommittedIndex为-1
            maxCommittedIndex = -1L;
        }
        this.currTask = TaskType.IDLE;
        return maxCommittedIndex;
        
    }


    //根据日志索引，将对应的日志应用到状态机中的方法，committedInde是已提交的最新的日志的索引
    private void doCommitted(final long committedIndex) {
        //省略该类的大部分内容
        
        //走到这里意味着是业务日志，那就在下面这个方法中将日志应用到状态机上
        doApplyTasks(iterImpl);
        
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/18
     * @方法描述：将业务日志应用到状态机上的方法
     */
    private void doApplyTasks(final IteratorImpl iterImpl) {
        //这里对日志迭代器做了点简单的包装，其实就是可以获得真正的有效日志了，也就是data部分
        //这部分解码之后们就是用户的操作执行
        final IteratorWrapper iter = new IteratorWrapper(iterImpl);
        final long startApplyMs = Utils.monotonicMs();
        final long startIndex = iter.getIndex();
        try {//在这里将业务日志应用到状态机上，同时也会把日志对应的回调方法在状态机中回调
            //这部分逻辑是用户自己实现的
            this.fsm.onApply(iter);
        } finally {
            this.nodeMetrics.recordLatency("fsm-apply-tasks", Utils.monotonicMs() - startApplyMs);
            this.nodeMetrics.recordSize("fsm-apply-tasks-count", iter.getIndex() - startIndex);
        }
        if (iter.hasNext()) {
            LOG.error("Iterator is still valid, did you return before iterator reached the end?");
        }
        //得到下一个日志条目
        iter.next();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    @Override
    private void onSnapshotSave() {

         this.fsm.onSnapshotSave();
    }

    

    //省略该类的其他内容
}
```

上面代码块的内容虽然很多，但是只要大家仔细阅读，肯定能回忆起来以前的知识。这里我要解释一下，我把这些代码搬运到这里，并不是为了水文章字数，其实就算没有上面的代码块，文章篇幅异常超过1.1万字了。所以，我真的不是为了水字数，才把 FSMCallerImpl 状态机组件的相关内容搬运到这里。我之所以这么做是因为现在更新的这篇 jraft 框架的文章是后来补充的，而现在举例我为大家更新 jraft 状态机的文章已经过去好几个月了，我担心大家把之前的知识全部忘光了，所以才把之前的内容搬运过来了。当然，如果有的朋友们对 FSMCallerImpl 状态机组件的内容还很熟悉，可以直接跳过以上代码块，继续阅读下面的内容即可。

  

好了朋友们，总之，在阅读了上面的代码块之后，我相信大家对 FSMCallerImpl 状态机组件的内容多少都能回忆起来一些了。这时候大家应该回想起来了，**FSMCallerImpl 状态机中很多操作都是被其内部的 fsmThread 线程执行的，而这个线程其实就是 Disruptor 批处理器中的线程**。既然是这样，我相信这时候大家肯定会思考，**既然应用日志是被 fsmThread 线程执行的，那么状态机生成快照的操作也是 fsmThread 线程执行的吗**？现在我可以先把这个问题的答案告诉大家，虽然我还没有在我们的文章中为大家实现对应的功能，但实际上确实就是这样，**在 sofajraft 框架源码中，使用 FSMCallerImpl 状态机生成快照文件的时候，也是用其内部的 fsmThread 线程执行的，也就是说，状态机生成快照的操作，也是由 Disruptor 框架的批处理器的线程执行的**。明白了这一点，这就意味着我们之前实现的 FSMCallerImpl 状态机组件要重构一下，让 Disruptor 框架的批处理器可以执行生成快照文件的操作，其实实现起来非常简单，**就是让 FSMCallerImpl 状态机组件使用的 Disruptor 框架的 ApplyTaskHandler 处理器执行 onSnapshotSave() 方法即可**。但是现在先让我们把这个重构放在一边，我知道有一个更重要的问题正困惑着大家，**那就是如果 FSMCallerImpl 状态机组件使用的批处理器是单线程，既然是单线程那么线程执行操作的时候就是顺序执行的，这也就意味着 FSMCallerImpl 状态机组件在应用日志的时候就不能生成快照，而在执行生成快照的操作时，就不能执行应用日志的操作**。这个逻辑大家应该都可以理解吧？那这样一来，假如 FSMCallerImpl 状态机组件正在执行生成快照的操作，jraft 集群恰好生成了一批新的日志需要应用到状态机上，这个时候要怎么办呢？

FSMCallerImpl 状态机组件正在生成快照，生成快照文件涉及到 IO 操作，显然会阻塞状态机组件内部的线程，这样一来，那它内部执行任务的线程肯定就无法及时把日志应用到状态机上，必须得等到生成快照文件的操作执行完毕了，状态机组件内部的才会执行应用日志到状态机的操作。那么客户端肯定会迟迟得不到成功响应，显然会影响 raft 集群对外提供服务的执行效率。那该怎么解决这个问题呢？

我能想到这个时候肯定有朋友会说，**使用异步执行生成快照的操作呀，虽然 FSMCallerImpl 状态机组件内部使用但线程执行所有操作，但是当这个内部线程执行生成快照操作的时候，可以在这个线程中再使用一个新的线程异步执行生成快照的任务呀**，就像下面代码块展示的这样，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/17
 * @方法描述：状态机组件的实现类
 */
public class FSMCallerImpl implements FSMCaller {

    private static final Logger LOG = LoggerFactory.getLogger(FSMCallerImpl.class);

    //用户真正定义的状态机
    private StateMachine fsm;


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    @Override
    private void onSnapshotSave() {
        //这里并没有什么变化，发生变化的是StateMachine状态机的onSnapshotSave()方法
        this.fsm.onSnapshotSave();
    }


    //省略该类的其他内容
}





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



    //这个方法就是把内存数据库map中的数据存储到本地快照文件中的方法
    //该方法本来就是状态机接口StateMachine中定义的方法
    //因为状态机使用户自己实现的，所以该方法也是用户自己实现的，也就是说用户可以自己在状态机中
    //定义生成快照文件的逻辑
    @Override
    public void onSnapshotSave() {
        
        //在这里异步把Map中的数据写入到临时快照文件夹中了，快照文件名称为data
        executor.submit(() -> {
            //得到map快照文件写入器
            MapSnapshotFile snapshot = new MapSnapshotFile(writer.getPath() + File.separator + "data");
            //把map中的数据写入到快照文件中
            snapshot.save(map);
        });
    }


    //省略该类的其他内容
    
    
}
```

在上面代码块中，大家可以看到，我重构了一下自己定义的 jraft 集群状态机的 onSnapshotSave() 方法，在该方法中使用线程池异步执行生成快照文件的操作。这样一来，FSMCallerImpl 状态机组件在执行生成快照的任务时，就会直接把生成快照的操作封装为一个 Runnable 提交给新创建的线程池，然后就可以直接返回执行其他操作了。这么操作下来，FSMCallerImpl 状态机组件不就可以同时生成快照文件，又可以应用日志到状态机上了吗？但是这么操作之后，我们就又面临一个新的问题，**那就是当 FSMCallerImpl 状态机组件在生成快照文件的时候，还可以继续应用日志，也就是说当我们把 kv 数据库中的数据存储到本地快照文件中时，客户端还不断向 kv 数据库中添加新的数据，这个操作显然会让快照文件中保存的数据不准确呀**。

这时候大家应该也明白了，实际上，**在 FSMCallerImpl 状态机组件执行生成快照文件的操作时，应该分为两种情况，一种是同步生成快照文件的操作，那就是使用 FSMCallerImpl 状态机组件的内部线程直接生成快照，这样一来在生成快照的时候无法应用新的日志，那么生成的快照文件保存的数据就是当前时刻最新的数据；当然，为了提高 raft 集群对外提供服务的效率，也可以采用异步生成快照的操作，只不过异步生成的时候，当前生成的快照并不是集群中最新的数据，因为在生成快照的同时，客户端可能还会像 kv 数据库添加新的数据**。那么对于这两种不同的生成快照的情况，我们该如何取舍呢？或者说该如何选择呢？这些内容在这一章显然是讲不完了，就留到下一章讲解吧，好了朋友们，我们下一章见！