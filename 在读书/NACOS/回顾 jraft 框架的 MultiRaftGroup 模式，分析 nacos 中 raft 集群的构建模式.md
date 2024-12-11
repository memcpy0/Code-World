在阅读本章内容之前一定要看完前面实现的 sofajraft 框架，否则很多内容看不明白。

前面几个章节讲完之后，我们已经为 nacos 服务端成功搭建起了 Distro 集群，这个集群是一个典型的 ap 模式的集群，它并不能保证数据的实时一致性，只能保证最终一致性，因为在 Distro 集群中节点之间备份数据是存在延迟的，别忘了我们为 Distro 集群定义延迟任务和延迟任务执行引擎的作用；除此之外就是 Distro 集群中的每一个节点都的身份都是平等的，都可以接收来自客户端的数据，这就导致了在某个时刻，集群中的各个节点存储的数据可能是不一致的，而且这种不一致的情况可能是比较常见的；**尤其是在集群中发生网络分区的时候，有些节点无法接收来自其他节点备份的数据，那么集群中各个节点的数据肯定就不一致了。所以，我们说 Distro 集群不能保证数据实时一致性，称其为 ap 模式的集群**。

既然有牺牲数据一致性的集群，那肯定就有保证数据一致性的集群，我相信大家对此也都清楚了，也就是 cp 模式集群，raft 集群就是一个典型的 cp 模式集群，这也就是我们本章要为 nacos 服务端构建的集群。在 raft 集群中，每一个要被写入的数据都要先在集群各个节点之间达成一致才能被成功写入，因此在 raft 集群的各个节点中，它们存储的数据都是一致的。当然，有些时候可能某些节点上存储数据的进度有些落后，但因为 raft 集群投票制度的存在，这些被写入的数据已经在集群的大多数节点中达成了一致，所以我们也认为 raft 集群中各个节点存储的数据总体上都是一致的。**如果 raft 集群发生网络分区了，那根据 raft 集群选取新领导者的方式(具体的流程我就不展开讲解了，之前为大家实现 sofajraft 框架的时候，已经对 raft 集群的各个功能讲解的非常详细了，如果大家没有印象了，可以先回顾一下 sofajraft 框架再来看本章的内容)，选出的新的领导者一定存储着集群中所有已经被达成共识的数据，所以即便在 raft 集群发生网络分区时，整个集群也可以保证数据的一致性，然后对外提供服务**。这也就是 cp 集群模式的典型特征。这一章，我们就来一起为 nacos 服务端构建一个 cp 模式的集群，也就是一个 raft 集群。而构建的方式非常简单，就是使用我们之前实现过的 sofajraft 框架来构建。如果大家对 sofajraft 框架的内容非常熟悉，那么本章内容对大家来说将会非常简单。

## 剖析为 nacos 服务端构建 raft 集群的原因

在开始为 nacos 服务端构建 raft 集群之前，我认为还是有必要先分析一下，为什么要为 nacos 服务端构建一个 raft 集群，为什么就不能一直使用 Distro 集群呢？也许大家对此有很多想法，会从灵活性、稳定性、扩展性等很多方面对两种集群模式进行对比，然后分析出要为 nacos 服务端构建 raft 集群的原因。但就我自己的观点来说，并没有多么复杂，在我看来，为 nacos 服务端构建 raft 集群的原因无非两点：

**1 Raft 集群比 Distro 集群更能保证数据一致性，有些情况下程序能够完美运行非常依赖集群提供的数据一致性服务。**

**2 在 nacos 服务端有些数据需要持久化，而 Distro 集群并没有提供数据持久化服务，但 Raft 集群以持久化日志的方法把要备份的数据做到持久化了**。

以上两点就是我认为的需要为 nacos 服务端构建 Raft 集群的原因，也许大家现在对这两个观点理解得还不太透彻，接下来就让我为大家仔细分析一下。

  

首先来分析第一个原因，**那就是分析在 nacos 服务端集群中数据一致性的重要性**。在我们刚刚构建的 Distro 集群中，**我们已经知道了这个 Distro 集群备份的数据都是服务实例信息，并且都是临时服务实例信息**，我在为大家实现 Distro 集群的时候并没有强调这一点，但这一点确实是事实，大家可以自己去代码中印证一下。好了，还是说回这些临时服务实例信息吧，**Distro 集群在存储并且备份这些临时服务实例信息的时候，很多时候并不需要集群中的每一个节点存储的数据一直相同**。如果我们从宏观层面上概览一下 DIstro 集群每一个节点是如何工作的话，我们会发现，Distro 集群中每个节点的身份是相同的，集群中的每一个节点都可以接收 nacos 客户端连接，也可以被各自管理的 nacos 客户端注册临时服务实例信息。也许节点 1 被 nacos 客户端 A 注册了一些服务实例信息；节点 2 被 nacos 客户端 B 注册了一些服务实例信息；节点 3 被 nacos 客户端 C 注册了一些服务实例信息；**这就好像是 DIstro 集群中的每个节点各自负责一部分数据，然后通过数据备份的方式把各自的数据汇聚到一起，使每一个节点最后存储相同的临时服务实例信息**。可能在数据备份的过程中，有的节点存储的服务实例信息并不完整，还没有跟上其他节点的进度，那么假如这时候有 nacos 客户端向该节点订阅了对应的服务，这个节点可能推送给订阅了对应服务的 nacos 客户端的服务实例信息并不完整，这种情况也是很有可能发生的吧？但这种情况即便发生了也没什么，又不是不能收到可以使用的服务实例信息，只不过是服务实例信息不完整而已，只要访问这些目标服务的流量并不是很大，那么程序就可以完美运行下去。这一点大家应该可以理解吧？由此可见，仅仅考虑临时服务实例信息的话，即便 Distro 集群的每一个节点存储的数据不完全相同，也不会对程序运行造成太大的影响。

但是，大家千万别忘了，nacos 不仅仅具备注册中心的功能，也具备配置中心的功能，**所谓配置中心就是用来存储配置信息的，而配置信息决定了一个服务究竟该以怎样的方式启动和运行，所以，配置信息的完整性和一致性对程序启动以及运行来说是非常重要的**。这就要求我们在使用集群存储配置信息数据的时候，一定要保证集群对外提供的配置信息具有强一致性，如果还向 Distro 集群那样出现每个节点备份的配置信息可能不一致的情况，假如集群切换了另一个节点对外提供服务，提供的配置信息并不完整，可能这份配置信息在其他节点上已经更新了两次了，但该节点备份的配置信息还是旧版本的，那程序显然就不可能按照我们期望的那样运行了。所以，需要构建 Raft 集群来备份 nacos 配置信心要存储的配置信息。这一点在当前文章和第十二版本代码中还体现不出来，但到了后面，我为大家实现 nacos 配置中心功能时，大家就会发现 Raft 集群是怎么发挥作用的了，尤其是为 Derby 内嵌数据库构建集群时，会频繁使用 Raft 组件。好了，到此为止，第一点就分析完毕了，接下来我们继续看看第二个原因。

我在提出的第二个观点中，**强调了重要的信息需要持久化，而 Distro 集群没有提供数据持久化功能(我在构建 Distro 集群的时候也没有在文章中强调这一点，但这一点也是事实，大家可以自己去代码中印证一下)，所以要使用 Raft 集群**。刚才已经分析了，配置信息是非常重要的数据，所以肯定需要持久化。那么除了配置信息呢？别忘了我们现在的课程还没有引入配置信息，一直是在为 nacos 的注册中心构建集群。而注册中心存储的都是服务实例的信息，那么服务实例信息有没有别要持久化呢？**之前为 nacos 注册中心构建的 Distro 集群存储的都是临时服务实例的信息，这些信息在集群的每一个节点中都是直接存储在内存中的，并没有持久化到本地**。而且临时服务实例信息也不需要持久化到本地，原因很简单：**临时服务实例信息之所以是临时服务实例信息，就是因为这些服务实例信息可能是多变的，具有临时性，我们在业务开发中注册到 nacos 的很多服务实例信息都是临时的**。如果大家不明白这种临时性是什么意思，那就让我举一个非常直接的例子：比如说我们部署了一个服务提供某些功能，这个服务信息被注册到了 nacos 注册中心，正常情况下，这一个服务就可以承受全部流量了；但是在某些时候，访问服务的流量非常大，这时候就需要再额外部署多个相同的服务来分摊这些流量，那新部署的这些服务也会注册到 nacos 注册中心，并且是以临时服务实例模式注册的。等这一阵子流量过去之后，系统不需要这么多服务一起工作了，为了节省资源，就可以直接从注册中心注销多余的服务，然后关闭服务器。这是临时服务实例信息发挥作用的最直接的一个例子了吧？由此可见，**可以随时被注销的临时服务实例信息根本就不必持久化到本地**，大家可以仔细品味品味这个逻辑。

好了，经过刚才的分析，我们发现如果仅从临时服务实例信息的角度考虑，似乎根本不必引入 Raft 集群在集群节点备份数据的过程中通过日志的方法把数据持久化到本地。如果大家只知道临时服务实例信息这个概念的话，这么想也是情有可原。**但实际上，在 nacos 注册中心模块，nacos 客户端除了可以向注册中心注册临时服务实例信息，还可以向注册中心注册持久服务实例信息**。没错，现在我又为大家引入了一个持久服务实例信息的概念，那什么是持久服务实例信息呢？

光从名字上我们大概就能猜到，**和临时服务实例信息比起来，这个持久服务实例信息肯定不能随意注销，确切地说，这些服务实例信息肯定是要持久化到本地的**。这个其实没什么可说的，nacos 注册中心就是这么规定的，所以这个也不是我们应该关注的重点，现在引出了持久服务实例信息的概念之后，我们应该重点关注的是什么服务实例信息才能称为持久服务实例信息。这个其实也很好说，我都已经总结好了，就一个要点：**凡是被定义为持久服务实例的服务，这些服务基本上都需要长期稳定的运行，服务信息也不会频繁发生变更，是我们开发项目得以运行的基石**。当然，这个要点可能也比较模糊，所以还是让我再举一个例子，为大家解释一下。

假如我们在项目中构建了 mysql 集群，并且把这几个 mysql 服务信息定义为持久服务实例注册到了 nacos 的注册中心，现在我想问问大家，mysql 作为存储数据的重要服务器，肯定不能随便宕机，就算真的忽然宕机了，出现问题了，也应该能够被开发人员立刻定位到，这个可以理解吧？除此之外，这几个 mysql 服务器的地址信息也不能随便更改吧？因为数据都存放在 mysql 服务器上，随便更换 mysql 服务器的地址，就肯定会涉及到数据的迁移和备份，会使项目更加复杂，这也就意味着这些服务器肯定会被长期启动并使用，这个逻辑大家也可以理解吧？所以一旦我们在项目中启动了类似 mysql 这样的服务器，并且把服务地址当成持久服务实例信息注册到 nacos 注册中心了，nacos 注册中心就应该很好地保存这些服务器的信息。

很好，如果上面的逻辑大家都理解了，那现在请大家来思考一下，假如我们仍然使用 Distro 集群来存储和备份服务实例信息，**也就是说我们仍然把 mysql 服务实例信息当成临时服务实例信息来处理，在这种情况下，服务实例信息都是缓存在内存中的，假如存储这些服务实例信息的某个集群节点忽然宕机了，或者注册这些服务实例信息的客户端和服务端的连接忽然断开了，也就是出现健康检测失败的情况后，这些服务实例信息就直接丢失了**。当节点重启之后，要么从其他节点再把数据备份过来，要么重新注册一下，总之是有点麻烦的。如果直接使用 Raft 集群，那么这些服务实例信息就会以日志的形式持久化在本地，节点宕机重启之后，直接状态机应用本地日志就可以恢复数据。当然，除此之外，**和临时服务实例相比起来，持久服务实例注册到 nacos 注册中心后，假如持久服务实例出现健康检测失败的情况，这些持久服务实例并不会像临时服务实例那样直接从当前节点移除了，而是被标记为不健康状态，并且显示在 web 控制台让开发人员及时看到，这对开发人员定位程序出现的问题起到了至关重要的作用**。

到此为止我就把持久服务实例信息的概念和相关作用为大家介绍完了，不管以上文字有多复杂吧，总之大家只需要记住两点：

**1 在 nacos 注册中心服务端，持久服务实例需要使用 Raft 集群进行存储和备份。**

**2 即便出现了健康检测失败的情况，持久服务实例信息也不会从注册中心服务器清除，而是被标记为不健康状态，并且展示在 web 控制台，方便开发人员及时定位问题，然后解决问题**。

至于持久服务实例怎么创建，以及 nacos 服务端怎么对持久服务实例对应的服务器进行健康检测，这就是第 13 版本代码对应的功能了，就放到后面再讲解吧，目前我们正在实现的是第 12 版本代码对应的功能。好了，到此为止大家应该清楚了为什么要为 nacos 服务端构建 Raft 集群了吧？如果这个原因理解清楚了，那么接下来让我们思考思考另一个问题，那就是如何为 nacos 服务端构建 Raft 集群。

## 剖析为 nacos 服务端构建 raft 集群的模式

  

在开始分析如何为 nacos 服务端构建 Raft 集群之前，我希望大家可以暂时抛开任何关于 nacos 的问题，只思考一个和 Raft 集群有关的问题：**那就是假如我们要使用 3 个服务器为多份数据构建 Raft 集群，也就是说整个 Raft 集群只有三个节点，这个时候，应该怎么构建 Raft 集群来备份数据呢**？

让我说的再直接一些吧，所谓多份数据，那就是现在有多种类型的数据要在 Raft 集群中做备份，比如说有 kv 键值对数据，还有纯粹的字符串数据，还有服务实例信息数据。现在要为这三种类型的数据构建 Raft 集群，应该怎么做呢？我相信很多朋友看到这个问题的第一个念头就是 Raft 集群中有更多的节点就好了，如果一共有九个节点，也就是有九个服务器，那么我们就可以用三个服务器为 kv 键值对数据构建一个 Raft 集群，用另外三个服务器为字符串构建一个 Raft 集群，最后用剩下的三个节点为服务实例信息数据构建一个 Raft 集群。这样一来，就为这三种类型的数据各自构建了 Raft 集群，大家互不干涉，非常完美。

  

但很可惜，我们目前只有三个节点，也就是只能使用三个服务器为这三种类型的数据构建 Raft 集群。**这样一来，显然这三种类型的数据要共享这三个服务器了。那这时候该怎么构建 Raft 集群呢**？其实这个也很容易分析，既然这三种类型的数据要共享三个服务器，而三个服务器正好是 Raft 集群最小的节点数，那就为这三种类型的数据构建一个 Raft 集群好了，换句话说，**就是让这三种类型的数据共享一个 Raft 集群**。这个思路并非是不可行的，**因为在 Raft 集群中，写入数据的操作都会被封装为一条条日志，日志在集群的节点中达成共识了，持久化到本地了，才会被状态机应用到程序中**。而在 Raft 集群中，**日志的格式肯定是固定的，要封装到日志中的数据肯定会被序列化，到最后都是一堆字节而已，所以不管这三种类型的数据是什么，肯定都可以被封装到日志中，然后在 Raft 集群中达成一致**。**如果我们要为这三种数据构建 Raft 集群，那我肯定使用我们之前刚刚实现的 sofajraft 框架来构建，而在 jraft 框架中，要备份的数据首先会被封装到一个 Task 对象中，然后再把 Task 对象封装到 LogEntry 日志对象中，只要 LogEntry 日志在集群中达成一致，那么这些数据就可以被状态机应用到程序中了**。所以，接下来我就为大家展示一下 jraft 框架的 Task 和 LogEntry 这两个类的内容，这样大家会对我刚才分析的思路理解得更透彻。

首先是 Task 类，请看下面代码块。

```
package com.alipay.sofa.jraft.entity;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/12/4
 * @Description:要存储的数据内容和响应的回调方法就封装在这个类的对象中，可以从测试类中学习该类的具体用法
 */
public class Task implements Serializable {

    private static final long serialVersionUID = 2971309899898274575L;

    //这个成员变量存储的就是要在集群中备份的数据
    private ByteBuffer data = LogEntry.EMPTY_DATA;

    private Closure done;

    private long expectedTerm = -1;

    public Task() {
        super();
    }


    public Task(final ByteBuffer data, final Closure done) {
        super();
        this.data = data;
        this.done = done;
    }


    public Task(final ByteBuffer data, final Closure done, final long expectedTerm) {
        super();
        this.data = data;
        this.done = done;
        this.expectedTerm = expectedTerm;
    }

    public ByteBuffer getData() {
        return this.data;
    }

    public void setData(final ByteBuffer data) {
        Requires.requireNonNull(data, "data should not be null, you can use LogEntry.EMPTY_DATA instead.");
        this.data = data;
    }

    public Closure getDone() {
        return this.done;
    }

    public void setDone(final Closure done) {
        this.done = done;
    }

    public long getExpectedTerm() {
        return this.expectedTerm;
    }

    public void setExpectedTerm(final long expectedTerm) {
        this.expectedTerm = expectedTerm;
    }

}




//下面提供一个简单的测试类


public class ServerTest {

    public static void main(String[] args) throws InterruptedException, IOException {

        //得到领导者
        Node node = getLeader();

        //生产要备份的数据，交给领导者处理
        //产生一条数据，测试落盘效果,这是集群中第一条日志，所以索引为1
        //如果当前节点为领导者，就把数据提交给领导者，让领导者把日志复制给集群的其他节点
        //判断日志是否达成一致
        if (STATE_LEADER == node.getNodeState()){
            //数据内容为hello
            for (int i = 0; i <10 ; i++) {
                final ByteBuffer data = ByteBuffer.wrap(("hello").getBytes());
                CountDownLatch latch = new CountDownLatch(10);
                //把数据和回调方法封装到Task对象中
                final Task task = new Task(data, new ExpectClosure(latch));
                //提交给领导者处理
                node.apply(task);
            }
            System.out.println("开始向硬盘存储数据！！！！！");
        }
    }
}
```

然后是 LogEntry 类，请看下面代码块。

```
package com.alipay.sofa.jraft.entity;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/12/1
 * @Description:该类的对象就是一个日志条目
 */
public class LogEntry implements Checksum {

    public static final ByteBuffer EMPTY_DATA = ByteBuffer.wrap(new byte[0]);

    //日志类型，可能是业务日志，也可能是配置变更日志
    private EnumOutter.EntryType type;
    //日志ID，注意，日志ID中封装着日志的索引和任期
    private LogId id = new LogId(0, 0);
    //当前集群中的节点，这里也许有朋友会感到困惑，日志条目不是只包含日志锁云和任期以及日志内容吗？
    //为什么还要包含集群中的节点信息，这里解释一下，在jraft框架中除了业务日志，还有配置变更日志
    //配置变更日志也是使用当前类的对象来封装的，因此会有以下几个成员变量
    //并且配置日志也是会传输给跟随者的
    private List<PeerId> peers;
    //旧配置中的节点
    private List<PeerId> oldPeers;
    //当前配置中的学习者
    private List<PeerId> learners;
    //旧配置中的学习者
    private List<PeerId> oldLearners;
    
    //真正的日志信息，初始化时为空，这个成员变量就会存储要备份的数据
    private ByteBuffer data = EMPTY_DATA;
    
    //校验和，这个成员变量很重要，在日志传输之后，会在跟随者节点判断校验和是否和之前的相等，以此来判断日志条目是否有损坏
    private long checksum;
    //是否有校验和
    private boolean hasChecksum;


    public void setData(final ByteBuffer data) {
        this.data = data;
    }

    
    //其他内容省略
}
```

最后我给大家展示一下 jraft 集群的领导者是怎么把要备份的数据封装到 LogEntry 日志对象中的，请看下面代码块。

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


    //其他内容省略


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

        //该方法的其他内容省略
    
    }


    //其他内容省略
}
```

回顾了以上代码块之后，现在大家应该能回忆起来 jraft 框架领导者处理日志的流程了。如果大家都对 jraft 框架非常熟悉，肯定知道领导者处理日志的流程，所以我就不再详细分析了，这些都是旧知识。如果有朋友忘记 jraft 框架的部分内容了了，就回顾回顾之前的 jraft 框架课程的文章和代码吧。

好了，让我们继续分析，现在我们发现不管是哪种数据类型，其实都能以字节的方式被 jraft 框架的日志封装，然后在日志被集群节点达成一致的过程中，这些数据也就被持久化成功了。现在看起来，不管是哪种数据，就算它们共享了一个 Raft 集群，那么这些数据也可以被顺利的封装成日志，然后顺利地持久化。那这是不是就意味着我们可以使用三个服务器直接为这三种类型的数据搭建一个共享的 Raft 集群了吗？显然不能这么做，原因很简单，**别忘了在 Raft 集群中还有一个非常重要的组件，那就是状态机，不管是什么数据，怎么被持久化到本地，最后都要被应用到状态机上，才意味着客户端发出的操作被执行成功了，而不同数据类型最终在状态机中要被执行的操作都不相同，不同数据肯定都拥有各自的状态机，从这个角度来分析，这三种类型的数据显然无法共享一个 Raft 集群**。接下来让我举一个更清楚的例子，来为大家说明这一点。

比如说客户端向服务端发出了一个写指令，要向服务端的 map 中存储一条 kv 键值对数据，而服务端恰好构建了一个 Raft 集群，那么 Raft 集群的领导者在接收到客户端的写指令之后，把这条指令封装到日志对象中，然后把日志复制给集群中的各个节点，日志达成一致后，也就意味着日志被持久化完毕了。最后还要把这条日志应用到状态机上，Raft 集群的状态机就是和业务曾打交道的组件，状态机应用了这条日志，客户端发送过来的 kv 键值对才能真正被写入到服务端的 map 中。我也准备了一个 jraft 框架状态机对象的例子，正好是一个应用 kv 键值对数据的状态机，请看下面代码块。

```
package com.test;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2023/12/18
 * @方法描述：这是第七版本，我为大家提供的一个状态机实现类
 */
public class MapStateMachine  extends StateMachineAdapter{

    private static final Logger LOG = LoggerFactory.getLogger(MapStateMachine.class);

    //存储数据的map
    private HashMap<String,String> map = new HashMap<>();

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
                //走到这里意味着是跟随者将日志应用到状态机上
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


    //该类的其他内容省略
}
```

从上面代码块中可以看到，只有日志被 jraft 集群各个节点的状态机应用了，客户端发出的指令才算是被执行成功了，服务端才能返回给客户端成功响应。但现在的问题是，我们要为三种不同类型的数据构建 Raft 集群，并且这三种数据最终被处理的方式，也就是最终应用到状态机上的方式也各不相同：**kv 数据肯定是被状态机写入到 map 中；字符串信息也许是写入到一个字符串集合中；而服务实例信息要写入到对应的 Client 客户端对象中；由此可见，这三种不同的数据类型其实就对应了三个不同的功能模块，那不同的功能模块的操作怎么能混杂在一起呢**？就算你硬要把它们混杂在一起，**也就是为这三种数据定义一个可以被共享的状态机，然后在状态机中判断当前得到的日志要操作的是哪个类型的数据，然后再寻找对应的功能模块应用这一条日志**。这么做当然是可以的，但假如我们真的这么做了，就会造成一个问题，就是需要设计一个对象，这个对象可以封装三种不同的数据类型，并且还需要定义一个成员变量表示当前对象封装的是什么数据类型，如果每个类型的数据都有附加信息的话，还需要把封装附加信息的成员变量定义出来。这么做显然有点麻烦，总之，**不管你怎么设计，只要让三种不同类型的数据共享一个状态机，那么总有一个组件会让三种类型的数据的功能耦合在一起**，这是我们最不想看到了，三个不同类型的数据对应三个不同的功能模块，最好是各自工作，互不干涉。

那这该怎么办呢？很简单，**那就为这三种不同类型的数据各自创建一个 Raft 集群好了，然后为三个集群定义各自的状态机，这样它们就只负责干好自己工作就可以了**。很好，三台服务器创建三个集群，这个方法可行吗？当然是可行的，还是那句话，如果大家对 sofajraft 框架非常熟悉了，就会知道，**jraft 框架提供了一个很棒的概念，那就是 MultiRaftGroup 集群模式**。**所谓 MultiRaftGroup 集群模式，就是一台服务器上可以定义多个 Raft 集群，每一个集群都可以成为集群组，每个集群组的组名是唯一的，这些集群组共享这一台服务器的 IP 地址和端口号，当客户端向 Raft 集群的某个节点发送请求时会把数据所属集群的集群组封装在请求中，这样一来，接收请求的服务器会根据数据所属的集群组的名称找到该集群组的节点，然后由该节点判断当前节点是否为领导者，如果不是领导者就把请求转发给领导者处理**。这就是 jraft 框架构建的 MultiRaftGroup 集群的工作流程。我还为大家准备了一张简图，帮助大家深入理解 sofajraft 框架的 MultiRaftGroup 集群模式。请看下面简图。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1728638260190-e6911442-dfee-42b8-97b2-d74dfe2ba458.jpeg)

如果通过以上简图，大家理解了 jraft 框架的 MultiRaftGroup 集群的工作模式，那接下来我就为大家展示一段具体的代码，也就是 jraft 集群的节点是怎么根据集群组的名称找到该集群中的节点，然后处理请求的，请看下面代码块。

```
package com.alipay.sofa.jraft.rpc.impl.core;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/11/24
 * @Description:处理请求的处理器，JRaft中节点服务端使用的组件
 */
public abstract class NodeRequestProcessor<T extends Message> extends RpcRequestProcessor<T> {

    public NodeRequestProcessor(Executor executor, Message defaultResp) {
        super(executor, defaultResp);
    }

    protected abstract Message processRequest0(final RaftServerService serviceService, final T request,
                                               final RpcRequestClosure done);

    protected abstract String getPeerId(final T request);

    protected abstract String getGroupId(final T request);

    @Override
    public Message processRequest(final T request, final RpcRequestClosure done) {
        //判断节点是否在集群中
        final PeerId peer = new PeerId();
        final String peerIdStr = getPeerId(request);
        if (peer.parse(peerIdStr)) {
            //从请求中获得要处理该请求的集群组的Id
            final String groupId = getGroupId(request);
            //然后从节点管理器中得到当前服务器中，该集群组中的节点
            final Node node = NodeManager.getInstance().get(groupId, peer);
            if (node != null) {
                //如果节点村在的话，则让该节点处理请求
                return processRequest0((RaftServerService) node, request, done);
            } else {
                return RpcFactoryHelper
                        .responseFactory()
                        .newResponse(defaultResp(), RaftError.ENOENT, "Peer id not found: %s, group: %s", peerIdStr,
                                groupId);
            }
        } else {
            return RpcFactoryHelper
                    .responseFactory()
                    .newResponse(defaultResp(), RaftError.EINVAL, "Fail to parse peerId: %s", peerIdStr);
        }
    }
}
```

现在大家应该回忆起来了 jraft 框架的内容了吧？通过集群组的方式，一台服务器可以部署多个集群的节点，并且每个节点参与自己集群的各种活动时，互相也不会干涉。**领导者选举，日志达成一致，状态机应用日志等等重要活动，都是每个集群组自己负责的**。好了，现在朋友们应该都清楚了如何使用有限的服务器为不同类型的数据构建 Raft 集群了吧？

当然，我能想到大家现在肯定很好奇，我为什么放着 nacos 服务端集群的知识不讲解，反而围绕着 Raft 集群分析半天，现在我就为大家解释一下：**实际上在 nacos 服务端，需要使用 Raft 集群存储备份的数据并不只有持久服务实例信息，除此之外，服务实例信息的元数据，以及服务信息，也就是 Service 信息元数据都需要使用 Raft 集群备份。也许有的朋友现在可能对服务实例元数据、Service 元信息都还不太了解，这个在 nacos 支线内容中我会为大家详细讲解。现在大家只需要知道，这三种类型的数据对应的状态机操作都是不同的，因此我们不能只为 nacos 服务端构建一个 Raft 集群，然后让这几种数据类型共享这一个 Raft 集群；而是应该为每一种数据类型都构建一个 Raft 集群，因此，我们就可以使用之前介绍的 sofajraft 框架的 MultiRaftGroup 集群模式，为这几种数据类型分别构建集群组**。这就是我花大量篇幅为大家引出 MultiRaftGroup 集群模式的原因，因为 nacos 在使用 jraft 框架构建 Raft 集群的时候，就会用到 MultiRaftGroup 集群模式。当然，在第十二版本代码中我并不会为大家引入和服务实例元信息、Service 元信息相关的任务组件，这些等到实现 nacos 支线功能时都会讲解，我在本章引出这些概念和数据只是为了让大家透彻理解使用 jraft 框架的 MultiRaftGroup 模式为 nacos 服务端构建 Raft 集群的原因。现在，大家应该理解得非常清楚了吧？

## 引入 NacosStateMachine 状态机

好了，nacos 服务端构建 Raft 集群的原因和模式都已经分析完毕了，接下来就该使用 jraft 框架为 nacos 服务端构建 Raft 集群了。这次构建集群我想先颠倒一下顺序，不按照集群的启动以及执行流程来实现集群了，而是紧接着前一小节的内容，先把 nacos 在 Raft 集群中要用到的状态机展示出来。我之所以先实现 nacos 在 Raft 集群中要使用的状态机的原因也很简单：**上一小节我们就是以状态机的不同为着手点，分析出要使用 MultiRaftGroup 模式为 nacos 服务端构建 Raft 集群，这也就意味着每一个数据类型在自己的 Raft 集群中使用的状态机是不同的。这也就要求我们要为每一种数据类型的集群定义一个自己的状态机，当然，这些状态机肯定要继承 jraft 框架自己定义的状态机适配器，也就是 StateMachineAdapter 类**。看样子我们要在 nacos 服务端中定义三个状态机了，但实际上在 nacos 中并不是这么做的，接下来我想趁热打铁，马上为大家定义一下 nacos 自己的状态机。

实际上，在 nacos 服务端只定义了一个状态机，**那就是标题中展示的 NacosStateMachine 对象**。NacosStateMachine 类继承了 jraft 框架的 StateMachineAdapter 状态机适配类，并且 nacos 的状态机中定义的都是常规逻辑，和我们在实现 jraft 框架时看到的一些状态机的功能都一样，都是循环获取日志，然后应用每一条日志。NacosStateMachine 类的具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/27
 * @Description:jraft集群组应用日志的状态机
 */
class NacosStateMachine extends StateMachineAdapter {


    //表示当前节点是否为领导者
    private final AtomicBoolean isLeader = new AtomicBoolean(false);

    //当前状态机要被使用的集群的集群组Id
    private final String groupId;

    //当前节点
    private Node node;

    //节点当前任期
    private volatile long term = -1;

    //集群组当前领导者IP地址
    private volatile String leaderIp = "unknown";


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:状态机应用日志的方法，以下其实就是jraft框架中的操作，这些步骤在jraft框架中我已经为大家展示过了
     * 所以就不添加特别详细的注释了，只添加关键注释
     */
    @Override
    public void onApply(Iterator iter) {

        int index = 0;
        int applied = 0;
        Message message;
        NacosClosure closure = null;
        try {
            while (iter.hasNext()) {

                //再循环中获取每一条日志，然后应用每一条日志
                
            }

            //其他内容就先省略了
        
    }

    

    //该类的其他内容省略

}
```

虽然我暂时展示的是伪代码，但在我提供的第十二版本代码中，NacosStateMachine 状态机的逻辑也并不复杂，就和上面代码块中的文字说明一样。nacos 自己定义的状态机的逻辑就是非常简单，在循环中获取获取每一条要应用的日志，然后应用就行了。这个逻辑我相信每一位朋友都能看懂，并且大家肯定会很疑惑，这和我们上一小节分析的一点也不一样啊，不是说要为每一个数据类型都定义一个状态机吗？为什么在 nacos 服务端只定义了一个呢？这样一来，**在为每一个数据类型构建 Raft 集群的时候，都要创建 NacosStateMachine 类的对象，然后把状态机设置到各自的集群中，那么每个数据类型对应的 Raft 集群使用的状态机不就是相同的了**？别急，接下来我就为大家具体解释一下，nacos 是怎么解决这个问题的。

其实在 nacos 服务端还定义了一个数据处理器抽象类，也就是 RequestProcessor 类，该抽象类下可以定各种子类。**在 nacos 服务端，就为每一种数据类型都定义了 RequestProcessor 数据处理器的子类**。比如说处理持久服务实例信息的数据处理器就是这么定义的，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.service.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:在第12版本代码中只做最简单实现，实现初始化JRaftProtocol对象即可，第13版本实现服务端注册持久服务实例功能时
 * 再实现该类的大部分方法
 */
@Component("persistentClientOperationServiceImpl")
public class PersistentClientOperationServiceImpl extends RequestProcessor {

    //在文章中，展示也是非常简陋的内容


    //在NacosStateMachine状态机的onApply()方法应用日志的时候，就会调用处理器的方法来处理
    @Override
    public Response onApply(WriteRequest log) {

        //判断是注册服务实例还是注销服务实例

        //如果是注册服务实例，就调用registerInstance()方法


        //如果是注销服务实例，就调用deregisterInstance()方法
    }



    @Override
    public void registerInstance(Service service, Instance instance, String clientId) throws NacosException {

    }


    @Override
    public void deregisterInstance(Service service, Instance instance, String clientId) {

    }


    //该数据处理器要被使用的集群组的唯一组名
    @Override
    public String group() {
        return Constants.NAMING_PERSISTENT_SERVICE_GROUP_V2;
    }
    
}
```

很好，现在大家应该清楚了，真正应用日志的其实是每一个数据对应的数据处理器，但是首先应用日志的是状态机呀，那状态机在应用日志的时候怎么调用数据处理器的 onApply() 方法呢？这就更好说了，**在 nacos 为每一个数据类型创建 Raft 集群时，会为各自的集群创建 NacosStateMachine 对象，然后把 NacosStateMachine 对象设置到各自的集群中。而在创建 NacosStateMachine 对象的时候，就会把各自对应的数据处理器设置到各自的 NacosStateMachine 对象中**。所以 NacosStateMachine 类其实要重构成下面这样，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/27
 * @Description:jraft集群组应用日志的状态机
 */
class NacosStateMachine extends StateMachineAdapter {


    //表示当前节点是否为领导者
    private final AtomicBoolean isLeader = new AtomicBoolean(false);

    //当前状态机要被使用的集群的集群组Id
    private final String groupId;

    //当前节点
    private Node node;

    //节点当前任期
    private volatile long term = -1;

    //集群组当前领导者IP地址
    private volatile String leaderIp = "unknown";

    //当前集群组处理数据的数据处理器
    protected final RequestProcessor processor;


    //构造方法
    NacosStateMachine(RequestProcessor processor) {
       
        this.processor = processor;
        this.groupId = processor.group();
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:状态机应用日志的方法，以下其实就是jraft框架中的操作，这些步骤在jraft框架中我已经为大家展示过了
     * 所以就不添加特别详细的注释了，只添加关键注释
     */
    @Override
    public void onApply(Iterator iter) {

        int index = 0;
        int applied = 0;
        Message message;
        NacosClosure closure = null;
        try {
            while (iter.hasNext()) {

                //再循环中获取每一条日志，然后应用每一条日志、
                //调用processor数据处理器的onApply()方法
                Response response = processor.onApply(message);
            }

            //其他内容就先省略了
        
    }

    

    //该类的其他内容省略

}
```

这样一来，nacos 为每一种数据类型创建的 Raft 集群就可以使用各自的状态机和数据处理器来真正处理数据了。大家可以仔细品味品味这个逻辑。也可以结合文章查看我提供的第十二版本代码来学习，当然，现在第十二版本代码的部分内容对大家来说还很陌生，等下一篇文章结束之后，第十二版本的代码就对大家完全开放了。本章的内容就到此为止了，下一章我们就会真正开始为 nacos 服务端构建 Raft 集群。好了朋友们，我们下一章见！