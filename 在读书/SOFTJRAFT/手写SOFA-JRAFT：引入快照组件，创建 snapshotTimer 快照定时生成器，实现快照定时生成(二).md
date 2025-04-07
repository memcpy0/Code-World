在上一章文章结束之前，为了提高 jraft 框架构建的 raft 集群对外提供服务的效率，我们为 FSMCallerImpl 状态机组件生成快照文件提供了异步操作的功能。我把相关代码再次搬运过来，帮助大家回顾一下，请看下面代码块。

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

在展示了上面代码块的内容之后，我在上一篇文章的最后还跟大家总结了一下状态机组件生成快照文件操作的两种情况，并且针对这两种情况提出了新的问题：**在 FSMCallerImpl 状态机组件执行生成快照文件的操作时，应该分为两种情况，一种是同步生成快照文件的操作，那就是使用 FSMCallerImpl 状态机组件的内部线程直接生成快照，这样一来在生成快照的时候无法应用新的日志，那么生成的快照文件保存的数据就是当前时刻最新的数据；当然，为了提高 raft 集群对外提供服务的效率，也可以采用异步生成快照的操作，只不过异步生成的时候，当前生成的快照并不是集群中最新的数据，因为在生成快照的同时，客户端可能还会像 kv 数据库添加新的数据，这个操作显然会让快照文件中保存的数据不准确**。那么对于这两种不同的生成快照的情况，我们该如何取舍呢？或者说该如何选择呢？

## 继续分析 **FSMCallerImpl 状态机组件**同步、异步生成快照文件的情况

  

因为上一篇文章的篇幅已经很长了，所以在文章最后，我没办法对以上总结的同步和异步生成快照文件的操作展开详细分析，实际上，同步和异步生成快照文件并不是我刚才总结的那么简单，如果操作不当，很可能会出现影响数据准确性的问题。当然，FSMCallerImpl 状态机组件同步生成快照文件的操作确实没什么可分析的，**因为 FSMCallerImpl 状态机组件使用其内部的线程生成快照时，无法把新的日志应用到状态机上，所以同步生成快照的操作无非就是影响 jraft 框架构建的 raft 集群的工作效率，并不会对数据的准确性造成什么影响。FSMCallerImpl 状态机同步生成快照文件的时候，生成的快照文件就代表当前集群节点的状态机的最新状态，生成的快照文件和当前时刻的状态机的状态、也就是保存的数据是绝对一致的**。这一点大家肯定能够理解吧？

好了，说完了 FSMCallerImpl 状态机组件同步生成快照的情况，接下来要关注的异步生成快照文件的操作就有的说了。**我之前跟大家说当 FSMCallerImpl 状态机组件异步生成快照的时候，状态机还可以同时应用新的日志，这就会造成一种情况，那就是生成的快照文件保存的状态机中的数据可能不是最新的，这个操作显然会让快照文件中保存的数据不准确**。我相信很多朋友肯定会对这句话感到困惑，觉得理解不了这句话的意思，因为不管怎么思考，不管是同步还是异步生成快照，只要生成快照文件的那一瞬间，快照文件保存的数据是状态机当前的数据即可。就比如说还是一个 kv 数据库，这个数据库中有以下三个数据，请看下面代码块。

```
kv.put("球星","罗纳尔迪尼奥")
kv.put("游戏","黑悟空")
kv.put("傻子","小仙女")
```

假如现在我们要使用 FSMCallerImpl 状态机组件异步生成快照，但是在生成快照的时候，kv 数据库的客户端向数据库发送了一个新的指令，要更新其中的一个数据，比如说更新成下面这样，请看下面代码块。

```
kv.put("球星","罗纳尔迪尼奥")
kv.put("游戏","黑八戒")
kv.put("傻子","小仙女")
```

当 FSMCallerImpl 状态机组件在异步生成快照文件的时候，如果是在快照文件已经快生成完毕了，或者说快照文件已经把状态机中的旧数据 kv.put("游戏","黑悟空") 存储到文件中了，然后状态机才把封装了客户端最新指令的日志应用了。也就是说**，异步生成的快照文件保存的其实是状态机没有应用日志之前的旧的数据，当快照文件生成完毕之后，这个快照文件保存的数据是旧的，状态机中的数据是最新的**，这一点大家可以理解吧？而我之前跟大家说这种情况会让快照文件中保存的数据和状态机中的数据不一致，影响数据的准确性。现在，我想请大家思考一下，这种异步生成快照文件的情况，这种情况会影响当前节点保存的数据的准确性吗？

要想弄清楚这个问题，我们就必须抓住快照文件的本质，在上一章我跟大家说，**raft 集群中的日志记录的是对数据执行的操作的痕迹，快照文件记录的是数据本身**，**不管是日志还是快照，都是 raft 集群中备份数据的核心手段。无非就是快照文件占用的硬盘小一点，在 raft 集群节点恢复数据的时候速度快一点**。这些大家肯定都非常熟悉了，并且我们也都知道，当 raft 集群的领导者使用 FSMCallerImpl 状态机组件生成了快照文件后，就要把快照文件对应的日志都删除了，以此来腾出硬盘空间。但是，我们也应该清楚地认识到，在 raft 集群中，快照文件和日志条目并不是互斥的，并不是说只要存在了快照文件，所有日志条目都要删除。实际上在一个 raft 集群中，快照文件和日志条目往往是并存的。比如说 raft 集群的领导者节点保存了 100 条日志，在集群运行了一会之后，领导者节点为这 100 条日志对应的数据生成了一个快照文件，在快照文件生成完毕之后，领导者就把与这些数据对应的 100 条日志都删除了。不过有一点需要注意的是，FSMCallerImpl 状态机在异步生成快照的时候又应用了 10 条新的日志。这样一来，领导者最终就凭借一个快照文件和 10 个日志条目保存了所有的数据。假如领导者宕机了，当它重启之后，以新节点的身份加入到 raft 集群时，会首先加载本地的快照文件，把快照文件中的数据应用到状态机，然后会接着应用额外的 10 条日志封装的操作指令，等这些操作执行完毕了，那么这个节点的状态机就恢复到宕机之前的状态了。这些操作大家肯定都可以理解吧？

如果这些逻辑大家都理解了，那么请大家再次思考一下上面的那个问题，当 FSMCallerImpl 状态机组件异步生成快照文件的时候，状态机应用了新的日志，新的日志可能会更新状态机中的数据，但是最终快照文件保存的仍然是旧的数据，这种情况会影响当前节点保存的数据的准确性吗？我的答案是肯定不会，原因很简单：**因为就算 FSMCallerImpl 状态机组件异步生成快照文件，只要快照文件保存的是状态机那一刻的数据就可以了，因为新的数据被新的日志保存了，也就是说，快照文件加上新的日志，就保存了当前节点接收到的所有数据**。刚才我为大家举的例子已经印证了我的这个观点，我想大家也都会认同这一点。很好，经过漫长的分析之后，那我们现在是不是可以认为，就算 FSMCallerImpl 状态机组件异步生成快照，那么这个操作也不会影响当前节点备份的数据的准确性呢？也许有朋友会认同这个观点，也许有朋友提出怀疑，这很正常，而我的观点非常明确：**FSMCallerImpl 状态机组件异步生成快照的操作可能会影响当前节点备份的数据的准确性**。这个观点显然和我刚才举的例子的结果相违背，也许会让更多的朋友感到困惑。别急朋友们，接下来就让我再来为大家举一个例子，来印证我这个观点的正确性。

现在我们不再以 kv 数据库为例子了，也就是不再为 kv 数据库构建 raft 集群了。假如现在我们构建了一个 raft 集群，这个集群专门用来记录我的小金库，也就是背着媳妇偷偷攒下的私房钱的金额，那么我就可以使用 jraft 框架简单构建三个节点作为服务器集群，这三个节点构成了一个最简单的 raft 集群。每当我向小金库存了一笔钱，就会向 raft 集群的领导者发送一个 add 指令，指令中包含要存进去的金钱的数额，raft 集群的领导者把指令包装成日志，然后传输给集群的其他节点。这条日志在集群间达成一致之后，领导者的状态机就可以应用这条指令了，也就是对我小金库中的金额进行累加；当然，如果我从小金库中取出一笔钱消费了，那么领导者的状态机就要对小金库中的金额做减法。这个逻辑大家应该都能理解。

当我构建的 raft 集群运行了一段时间之后，领导者节点本地存储的日志已经非常多了，于是我决定使用 FSMCallerImpl 状态机组件生成一个快照文件。当我生成快照文件之后，就可以把对应的日志条目都删除了。假如现在小金库中存储的金额是 1000 元，这 1000 元对应着 5 条日志；当 FSMCallerImpl 状态机组件异步生成快照文件的时候，我忽然又从小金库中取走了 100 元，当 FSMCallerImpl 状态机应用了对应的日志后，也就是第六条日志，状态机中存储的金钱数额就变成了 900。因为快照是异步生成的，应用第 6 条日志的操作恰好在快照生成之前，所以生成快照的时候就把状态机中最新的金钱数额 900 元保存在快照文件中了。注意，**这里情况就发生了变化，快照文件存储的并不是生成快照那一刻状态机保存的数据，而是应用了新的日志后的数据**，当快照生成完毕之后，要把保存的数据对应的日志条目都删除了，这个时候 raft 集群的领导者就把前 5 条日志删除了。现在 raft 集群状态机中的数据是 900 元，快照文件中保存的数据也是 900 元，第 6 条日志封装着从小金库中 -100 元的操作指令。这个流程应该很容易理解吧？

如果大家理解了刚才例子中发生了什么，那现在请大家再跟我思考一个情况，假如我小金库 raft 集群的领导者节点忽然宕机了，等它重新启动之后需要凭借本地快照文件和日志恢复自身数据了，当前节点会先加载快照文件，加载完毕之后状态机中存储的数据就是 900，然后还要再应用第 6 条日志，这条日志的指令是从小金库中 -100，应用完日志之后，状态机中存储的数据就变成了 800 元。看，问题出现了吧？本来状态机中的数据应该是 900 元，但是就因为异步生成快照的操作出现了并发问题，结果我存在小金库里的前莫名其妙少了 100 元。说真的，如果我不是程序员，如果这个小金库程序和集群不是我构建的，那我小金库里的钱是怎么没的我都不知道！！！现在大家应该能认同我刚才表达的观点了吧？也就是说：**FSMCallerImpl 状态机组件异步生成快照的操作确实会影响当前节点备份的数据的准确性**。那这个问题应该怎么解决呢？

其实解决起来非常简单，通过以上几个例子的分析，我相信大家都能意识到，同步生成快照的操作之所以不会出现问题，就是因为 FSMCallerImpl 状态机组件同步生成快照的时候无法应用新的日志，这样一来快照文件保存的就是生成快照那一刻状态机中的数据；如果 FSMCallerImpl 状态机组件异步生成快照，并且生成快照的时候状态机要应用新的日志，新日志如果在对应的数据存储在快照文件之后才被应用到状态机上，就不会出现并发问题；新日志先应用到状态机上，对应的数据才被存储到快照文件上，那就很可能因为并发操作出现备份的数据不准确的问题。总结下来我们可以发现，**不管是 FSMCallerImpl 状态机组件同步生成快照还是异步生成快照，只要生成快照文件的那一刻，快照文件保存的数据就是那一刻状态机保存的数据，这样一来，就肯定不会出现当前节点备份数据的准确性丢失的情况**。那要怎么做到这一点呢？怎么让快照文件存储的就是生成快照那一刻状态机保存的数据呢？当然，同步生成快照文件肯定就不用考虑了，这个操作肯定不会出现问题，我们只关注异步生成快照文件的情况，这种情况下该怎么做呢？

其实我们可以采取一些措施，**在 FSMCallerImpl 状态机组件异步生成快照的时候，对状态机中的数据执行类似于写时复制的操作，这样一来一份数据就可以继续被处理，应用新的日志，而另一份数据，也就是生成快照那一刻状态机保存的数据，就可以被写入到快照文件中；其实仔细想想，生成快照文件的操作，不就和 redis 把数据异步落盘的操作是一样的吗？redis 把数据异步持久化到本地不就使用了写时复制技术吗？总之不管使用什么方法，你要先得到生成快照那一刻的状态机中的数据，然后再开始异步生成快照**。在 jraft 框架的源码中有一个 AtomicStateMachine 类，这个类就展示了对应的操作，在生成快照文件的时候直接把状态机保存的数据复制了一份，然后把这份数据写入到快照文件中了。我把对应的代码搬运到自己的测试类中了，大家简单看看即可，请看下面代码块。

```

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
        //创建一个新的map
        final Map<String, String> values = new HashMap<>();
        //把map成员变量中的数据存储到新创建的map中，虽然我还没有为大家真正实现FSMCallerImpl
        //状态机组件生成快照的功能，但这里我要解释一下，这里的操作是在FSMCallerImpl状态机的内部线程中执行的
        //当这些代码执行的时候，FSMCallerImpl不可能应用新的日志，因为其内部是单线程执行各种操作的
        //也就是说这里把map中的数据复制到新的map中的操作不会出现并发问题，这个新的map存储的数据就是
        //快照生成的这一刻状态机中存储的数据
        for (final Map.Entry<String, String> entry : this.map.entrySet()) {
            values.put(entry.getKey(), entry.getValue());
        }

    
        //在这里异步把新创建的values中的数据写入到临时快照文件夹中了，快照文件名称为data
        //这里才开始异步生成快照，异步生成快照的时候状态机已经可以应用新的日志了
        //但是生成快照那一刻状态及存储的数据已经保存下来了，可以直接被写入到快照文件中
        executor.submit(() -> {
            //得到map快照文件写入器
            MapSnapshotFile snapshot = new MapSnapshotFile(writer.getPath() + File.separator + "data");
            //把新创建的values中的数据写入到快照文件中
            snapshot.save(values);
        });
    }
 

    //省略该类的其他内容
}
```

上面代码块中的注释非常详细，我就不再重复解释了。当然，除了上面代码块中展示的内容，**我认为在 FSMCallerImpl 状态机组件生成快照的时候，不管是同步生成还是异步生成，一定要把快照存储的内容和日志的界限划分清楚，不能有一丁点混淆；也就是说，生成的快照文件，一定要代表生成快照那一刻状态机中保存的所有数据**。我之前为大家举的例子，不就是在新应用了日之后，快照文件存储了对应的数据，结果出现问题了吗？如果快照文件只保存生成快照文件那一刻状态机保存的数据，比如说状态机存储的数据就对应前 100 条日志，那快照文件就保存这 100 条日志对应的数据，后面不管有多少条新日志被应用，都和当前生成的快照文件没有关系。这样一来，当前生成的快照文件就和后面新的日志构成了当前节点要备份的全部数据。大家可以品味品味这个逻辑。

到此为止，我就为大家把 FSMCallerImpl 状态机组件生成快照文件时同步异步的情况分析清楚了，也许大家会觉得我在这一部分内容花费了太多篇幅，而且是只用文字干讲，没有真正实现什么功能。这里我要解释一下，我并不是在罗里吧嗦地凑字数，我希望通过我这些分析，大家能清楚地认识到 raft 集群中的节点在执行生成快照的操作时，有很多问题需要注意，并且这些问题都是我们这些程序员要面临的，原因很简单，在我们使用 sofajraft 框架构建 raft 集群时，集群节点的状态机可是要我们亲自实现的，既然是我们亲自实现，那就意味着把日志应用到状态机的操作，生成快照文件的操作也都要我们亲自实现。不把各种情况考虑全面，放任 raft 集群出现各种各样的问题，这还怎么称自己为技术高超的程序员呢？所以大家在实现 raft 集群节点状态机的时候，一定要小心谨慎，防止可能出现的并发问题对节点备份的数据的准确性造成影响。

  

好了，这些问题分析完之后，接下来让我们轻松一下，来实现一个简单的功能。刚才我跟大家说为了保证状态机生成快照文件数据的准确性，要在 FSMCallerImpl 状态机组件生成快照的时候，不管是同步生成还是异步生成，**一定要把快照存储的内容和日志的界限划分清楚，不能有一丁点混淆**。那么这个功能应该如何实现呢？

## 分析快照文件的存储模式

  

在上一小节结尾，我跟大家提出了一个问题，那就是怎么把快照文件存储的内容和日志界限划分的非常清楚。其实这个问题还是有些模糊，可能会让一些朋友不理解其中的意思。那我就把这个问题再展开解释一下，比如说有一个 raft 集群的领导者，这个领导者内部存储了 100 条日志，当领导者的状态机组件生成快照文件时，就要把这 100 条日志对应的数据都保存到快照文件中，这个流程大家都熟悉了。但我现在想说的是，当领导者生成了快照文件之后，就要删除对应的日志条目，也就是要删除这 100 条日志。在执行这个操作的时候，领导者节点必须要准确地知道刚刚生成的快照文件保存的数据对应的是哪些日志条目，如果在删除日志条目的时候，多删和漏删任何一条日志都可能会对备份的数据的准确性造成影响。所以，我们必须要实现一些功能，把快照文件存储的内容和日志的界限划分清楚，也就是说，**要清楚地知道快照文件保存的数据对应哪些日志条目**。那么这个功能应该如何实现呢？这时候大家应该也意识到了，**那就是生成快照文件的时候，肯定不能只把数据本身存储到快照文件中，还要把这些数据对应的日志条目也存储起来，简而言之，我们要记录快照文件的元信息**。

还是举一个简单的例子，假如 raft 集群的领导者持久化了 100 个日志条目，这 100 个日志条目对应的数据已经应用到状态机上了。现在领导者要生成快照文件，那么当快照文件生成的时候，不能只记录状态机保存的这 100 条日志对应的数据，还应该把这些数据对应的日志条目的信息记录下来，而这些数据本身之外的信息，就是生成的快照文件的元信息。**比如说这 100 条日志对应的状态机中的数据是一些键值对信息，那么 raft 集群的领导者生成快照的时候(当然，集群的跟随者节点也可以生成快照，确切地说，也需要定期生成快照，我一直用领导者为大家举例子，但大家千万别以为在 raft 集群中，只有领导者节点能够生成快照文件)，就可以在本地创建两个文件，一个文件记录状态中的数据，这个文件就是快照文件本身；一个文件记录当前生成的快照文件对用的日志索引等等信息，这个文件就是快照文件的元信息**。如果过了一段时间之后，领导者节点要生成新的快照文件了，那么新生成的快照文件同样是这样，一个文件存储状态机中的数据，一个文件记录快照文件的元信息，也就是快照文件存储的数据对应的日志条目信息。这个逻辑理解起来应该没有问题了吧？如果大家理解了这些逻辑，那么接下来我就跟大家讲解一下在 sofajraft 框架中，FSMCallerImpl 状态机组件生成的快照文件的格式和模式。

我相信大家应该还记的，在启动 jraft 框架构建的 raft 集群时，需要为每个节点设置一些启动参数，就像下面代码块展示的这样，请看下面代码块。

```
//以下就是集群启动时要为每个节点设置的参数
C:/Users/陈清风扬/code/jraft-log/server1 counter 127.0.0.1:8081 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083

//上面这行参数可以做以下拆分：

//C:/Users/陈清风扬/code/jraft-log/server1代表文件存储路径，文件就是日志文件、节点元数据文件、快照文件；
//节点元数据文件就是节点的当前任期以及为哪个节点投过票

//counter代表的就是要启动的集群服务的名称

//127.0.0.1:8081代表的是当前节点的IP地址和端口号

//127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083代表的是集群中所有节点的IP地址和端口号，其实就是集群的配置信息
```

在阅读了上面的代码块之后，大家肯定可以回忆起来，当 raft 集群启动之后，这个集群会在本地创建三个文件夹，这三个文件夹分别会用来存储日志信息，节点元数据信息，以及快照信息。日志信息和节点元数据信息我们已经讲过了，接下来我就为大家讲解一下 jraft 框架构建的 raft 集群是怎么在本地存储快照信息的。

实际上是这样的，如果文件路径是确定的，就是上面代码块中展示的 C:/Users/陈清风扬/code/jraft-log/server1 路径，**那么 raft 集群启动之后，集群中的每一个节点都会在本地创建一个 snapshot 文件夹来存储快照文件，也就是说，存储快照文件的文件夹的路径为 C:/Users/陈清风扬/code/jraft-log/server1/snapshot**。当然，存储快照文件的文件夹的名称也是可以由程序员自己定义的，我在第八版本代码的测试类中就提供了相关的代码，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/25
 * @Description:测试类
 */
public class ServerTest {

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

        //省略该方法
    }


    //省略测试类其他内容
}
```

好了，存储快照文件的文件夹有了之后，raft 集群领导者生成快照文件的时候，就可以向里面存储具体的快照文件了。这时候我就要详细解释一下了，当 jraft 框架构建的 raft 集群的领导者在生成快照文件时，**会首先在 C:/Users/陈清风扬/code/jraft-log/server1/snapshot 文件夹中创建一个 snapshot_X 文件夹，这个 X 对应的就是快照文件存储的数据对应的最后一条日志索引，比如说领导者本地有 100 条日志，最后一条日志索引为 100吗，生成快照文件时要把这 100 条日志对应的数据存储到快照文件中，那么创建的快照文件夹的 X 就是 100**。**当这个 snapshot_100 文件夹创建完毕后，领导者会把快照文件本身，以及快照文件的元数据这两个文件存储在 snapshot_100 文件夹中**。具体存储模式请看下面代码块。

```


//在sofajraft框架中，每一个真正的快照文件名称为snapshot_X，这个X就是每一个快照文件记录的应用的最后一条日志的索引
//如果第一个快照生成的时候，状态机应用到索引为31的日志了，那么第一个快照文件的名称就为snapshot_31，如果第二个快照文件生成的时候
//状态机最新应用的日志是108，那么第二个快照名称就为snapshot_108，注意，这里的snapshot_X仍然为文件夹的名称，文件夹中才是真正的快照元数据文件和快照文件




                                                                /snapshot_31/__raft_snapshot_meta(快照元数据文件)     data(快照文件)

//就像这样：/User/chenqingyang/code/jraft-log/server1/snapshot
                                                                /snapshot_108/__raft_snapshot_meta(快照元数据文件)     data(快照文件)

```

好了，现在大家应该清楚了在 sofajraft 框架中，快照文件的存储模式什么样的了吧？如果这个清楚了，那么接下来我就为大家把 jraft 框架构建的 raft 集群快照的元信息文件的内容补充一下。刚才我们说，在领导者节点生成快照时，会同时创建两个问题，一个是快照文件本身，一个是快照元信息文件。但是这个快照元信息文件的内容我一直没有为大家解释。其实在 sofajraft 框架中，是这样定义快照元信息文件的格式的，请看下面代码块。

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

上面代码块中的注释非常详细，我相信每一位朋友都能看懂，所以就不再重复解释了。好了朋友们，在 jraft 框架生成的快照文件的模式分析完了之后，现在我们似乎就可以对 FSMCallerImpl 状态机组件进行重构了。

## 重构 FSMCallerImpl 状态机组件

  

重构 FSMCallerImpl 状态机组件的原因非常简单，在上一章我们就开始围绕着 FSMCallerImpl 状态机组件生成日志快照的功能来讲解知识了，可以说，文章的大部分内容都是围绕着 FSMCallerImpl 状态机生成日志快照的功能来展开讲解的。但是到目前为止我们却还没有实现这个功能，这有点说不过去了。还有一个原因是因为实现 FSMCallerImpl 状态机组件生成快照的功能非常简单，上一章我就跟大家说了，FSMCallerImpl 生成快照的操作也是内部线程执行的，就是 FSMCallerImpl 状态机内部的 fsmThread 线程执行的，而该线程其实就是 FSMCallerImpl 内部使用的 Disruptor 框架的批处理器中的线程。当然，如果想异步生成快照，那就需要程序员自己在 StateMachine 状态机对象的 onSnapshotSave() 方法中使用异步线程执行生成快照的操作。

也就是说，**就算使用 FSMCallerImpl 状态机组件执行异步生成快照的操作，那么这个操作也是被 FSMCallerImpl 状态机内部的 fsmThread 线程执行，只不过是在 StateMachine 状态机对象的 onSnapshotSave() 方法中开启异步**。所以，我们现在要重构 FSMCallerImpl 状态机组件，让该组件可以执行生成快照的操作，并且是使用其内部的 fsmThread 线程执行的，这就简单很多了。我们完全可以仿照 FSMCallerImpl 状态机组件应用日志的操作那样，实现 FSMCallerImpl 状态机组件生成快照文件的功能。**其实只需要把生成快照的操作封装为一个任务发布到 FSMCallerImpl 状态机组件内部使用的 Disruptor 框架的环形队列中即可，这样一来，Disruptor 框架的批处理器中的线程，也就是 FSMCallerImpl 状态机组件内部的 fsmThread 线程就可以处理 Disruptor 框架环形数组中的任务了**。这就是重构 FSMCallerImpl 状态机组件的思路，是不是非常简单？我把重构之后的 FSMCallerImpl 类展示在下面代码块中了，请看下面代码块。

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
     * @方法描述：该枚举类中的对象表示的是提交到状态机组件的disruptor中事件的类型，在第七版本主要只使用COMMITTED这个枚举类对象，其他的后面版本再讲解
     */
    private enum TaskType {
        //空闲状态
        IDLE,
        //日志已提交
        COMMITTED,
        //快照保存
        SNAPSHOT_SAVE,

        //以下枚举对象还用不到，大家先混个眼熟吧
        
        //快照加载
        SNAPSHOT_LOAD,
        //领导者结束,下面这两个枚举对象实在转移领导权时使用的
        LEADER_STOP,
        //开始成为领导者
        LEADER_START,
        //开始成为跟随者
        START_FOLLOWING,
        //跟随者结束
        STOP_FOLLOWING,
        //停机
        SHUTDOWN,
        //刷新
        FLUSH,
        //运行错误
        ERROR;

        private String metricName;

        public String metricName() {
            if (this.metricName == null) {
                this.metricName = "fsm-" + name().toLowerCase().replaceAll("_", "-");
            }
            return this.metricName;
        }
    }
    


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

        //构建disruptor框架的启动器
        this.disruptor = DisruptorBuilder.<ApplyTask> newInstance()
                .setEventFactory(new ApplyTaskFactory())
                .setRingBufferSize(opts.getDisruptorBufferSize())
                .setThreadFactory(new NamedThreadFactory("JRaft-FSMCaller-Disruptor-", true))
                .setProducerType(ProducerType.MULTI)
                .setWaitStrategy(new BlockingWaitStrategy())
                .build();
        this.disruptor.handleEventsWith(new ApplyTaskHandler());
        this.disruptor.setDefaultExceptionHandler(new LogExceptionHandler<Object>(getClass().getSimpleName()));
        //启动disruptor框架
        this.taskQueue = this.disruptor.start();

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
        } else {
            
            switch (task.type) {
                //在这个switch中，只关注这一个分支即可，这里如果提交的任务是COMMITTED类型的
                case COMMITTED:
                    //会在下面的方法中报错，因为COMMITTED类型的任务已经在最开始的if分支中处理了，所以不应该进入下面这个else分支中
                    //如果真的执行到这里了就报错
                    Requires.requireTrue(false, "Impossible");
                    break;
                case SNAPSHOT_SAVE:
                    //在这里匹配，看看是不是生成快照的操作
                    onSnapshotSaveSync(task);
                    break;

                //其他逻辑省略
            }
            
        }

        //该方法的其他逻辑省略

    }



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


     //该方法会把生产者数据存放到disruptor的环形队列中
    private boolean enqueueTask(final EventTranslator<ApplyTask> tpl) {
        this.taskQueue.publishEvent(tpl);
        return true;
    }



    //大家不要被这个方法的名字干扰了，虽然名字是同步生成快照的方法
    //但实际上真正同步还是异步是由程序员在状态机的onSnapshotSave()方法中决定的
    private void onSnapshotSaveSync(final ApplyTask task) {
        //在这里先判断状态机组件运行过程中是否出过错
        if (passByStatus(task.done)) {
            //执行生成快照的操作
            doSnapshotSave((SaveSnapshotClosure) task.done);
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2023/12/20
     * @方法描述：该方法就是用来生成快照的
     */
    private void doSnapshotSave(final SaveSnapshotClosure done) {

        //生成快照元信息文件的操作暂时省略
        
        //状态机生成快照
        this.fsm.onSnapshotSave();
    }
    

    //省略该类的其他内容
}
```

从上面代码块中可以看到，**如果我们希望 FSMCallerImpl 状态机组件执行生成快照文件的操作，那就可以调用 FSMCallerImpl 对象的 onSnapshotSave() 方法，这个方法一被调用，生成快照的操作就会被封装为一个任务发布到 Disruptor 框架的环形数组中，这样一来，这个任务就等着被 Disruptor 框架的批处理器中的线程，也就是 fsmThread 线程执行即可**。这个逻辑应该是很清晰的吧？大家可以认真看看上面代码块中的代码，或者直接结合文章阅读我提供的第八版本代码，我相信大家肯定可以掌握其中的逻辑。因为这一块的逻辑和 FSMCallerImpl 状态机组件应用日志的逻辑几乎一样。好了朋友们，现在 FSMCallerImpl 状态机组件也重构完毕了，这时候我们再回过头看看我们在 NodeImpl 类中新添加的内容，也就是下面代码块中展示的内容，我们会发现好像 jraft 框架生成快照文件的功能我们已经实现完毕了。请看下面代码块。

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

可以看到，snapshotTimer 快照生成定时器每次执行 doSnapshot() 方法的时候，都会调用 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法，而 FSMCallerImpl 状态机组件的 onSnapshotSave() 方法一被调用，就意味着生成快照文件的操作要被执行了。所以，看到这里，jraft 框架生成快照文件的功能好像已经被实现完毕了。当然啦，不用我解释大家应该也能意识到，这只是我们的幻想，如今我们为程序新编写的代码还远远不足以实现 jraft 框架生成快照文件的功能。

就比如说，刚才我们分析的同步和异步生成快照的情况就没有在程序中用上，既然可以同步、异步生成快照，那么总得把选择同步或异步的权力交给 sofajraft 框架的用户吧？但我们的程序显然没有实现这一点，还有快照文件的存储路径，虽然在程序启动的时候可以由用户自己定义存储快照文件的路径，但是用户定义的路径根本没有在程序内部被使用到啊，没有准确的路径，快照文件该怎么存储呢？除此之外，还有很多其他的问题，比如说，生成了快照文件之后，那么和快照文件对应的日志条目该怎么删除呢？生成快照文件的操作是一个定时任务，如果第二次生成快照文件的时候，我们发现 raft 节点只应用了 10 条新的日志，那这种情况还有必要再生成新的快照吗？也就是说，难道生成快照的定时任务每一次都要执行吗？什么情况下可以不执行呢？你看，随便想想就又发现这么多没有解决的问题，所以，现在还很难说 jraft 框架生成快照文件的功能被我们实现了。只有我们把刚才提出的问题都解决了，jraft 框架生成快照文件的功能才能被我们重构得稍微完善一下。但这些内容在这一章显然是讲不完了，所以就留到下一章为大家讲解吧，好了朋友们，我们下一章见！