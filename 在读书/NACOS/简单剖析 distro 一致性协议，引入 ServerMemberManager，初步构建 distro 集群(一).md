从这一章开始，我们就要开始为 nacos 服务端搭建集群了。也许对某些朋友来说，集群是一个有些可怕的名词。搭建集群的过程中会涉及到很多细节，要考虑很多方面，代码编写得非常复杂，逻辑晦涩难懂。我能理解大家的这种感觉，因为我曾经初次面对集群时，也觉得这方面的知识非常棘手。随着我进入编程世界的时间越来越长，编写的代码越来越多，我发现集群的构建越来越复杂了，要注意的细节也更多了，越来越多的名词和概念让我对市面上已有的各个框架和项目的集群望而却步。有时候我心里真的会产生这样一种想法：搞出来这些玩意的人真爱装逼啊，弄一大堆让人根本看不懂的词，然后说这是最流行的方案。最烦装逼的人了！可没别的办法，不知道是流行促就了权威，还是权威促就了流行，总之某些集群模式就变得既流行又权威了。当然，我只是讨厌那些装逼的概念和名词，并没有否认这些集群的价值。

随着我接触的集群模式越来越多，我也渐渐开始用自己的方式思考集群的本质，也希望能够用自己的方式定义一种集群。这时候，也许有朋友就想知道，我自己思考出来的集群的本质是什么。我很愿意说一说，这也没什么不好意思的，一提到什么什么的本质，我相信这时候总结出的论断往往都比较简单，我相信每一位朋友肯定也都像我这样对集群的本质进行思考；当然，论断虽然简单，但并不意味着它幼稚，因为最本质的东西就是最简单直接的。好了，说了这么多，接下来就让我阐述一下自己对集群的思考：**我认为一个最基本的集群只需要从两个方面即可构建成功，那就是完备的数据和可用的功能**。也许大家对这句话的含义感到模糊，接下来就让我来为大家具体解释一下。

一提到集群，我想大多数朋友首先想到的肯定是数据备份，确实，在我自己的思维里，我也把数据备份作为集群的首要功能，**集群各个节点间的数据应该是最终一致的，这个就是所谓的完备的数据；至于可用的功能就更简单了，集群中的节点肯定要对外提供服务，当集群中的一个节点因为故障不可用时，可以随时切换另一个节点继续提供服务**。就比如说我们可以为微服务的某个服务模块构建集群，当前节点不能再提供服务了，可以立刻使用另一个节点继续提供服务；如果举一个更具体一点的例子，那就是生产中的情况了，大家在生产中肯定会经常遇到构建 Redis 集群，或者是 MongoDB 集群的情况，假如构建的都是最基本的主从集群，那么 Redis 主节点宕机了，在使用哨兵模式的情况下，从节点就会成为主节点继续提供写服务，并且主从的数据也是一致的；如果构建的是更复杂的数据分片集群，在使用哈希策略把数据存储到集群中的各个节点上之后，某个节点出现故障之后，该节点的数据会根据哈希策略转移到集群中的其他节点上，当然，我们也可以给各个节点构建主从集群，实现数据备份和读写分离功能；以上这些肯定都是大家再熟悉不过的场景了。如果再换一种集群模式，假如说构建的是 Raft 集群，那这种集群对数据的完备性和功能的可用性就体现得更明显了：数据在 Raft 集群各个节点间的复制这个就不必再说了吧？更新 Jraft 框架的时候都讲解过了，而当领导者不可用的时候，集群会再推举出一个新的领导者，保证集群可以提供写服务，Raft 共识算法会保证新的领导者的数据是齐全的，这个就是功能可用性的体现。

看完了以上列举的种种例子，现在大家应该清楚了我对集群总结的完备的数据和可用的功能是什么意思了吧？并且我认为只要依据这两点，就可以构建一个最基本的集群。当然，考虑到庞大的数据量，为了使集群更加稳健、扩展性更好、可用性更高、工作效率更高，人们会对最简单的集群进行不断重构，给集群添加配置变更、数据分片，快照生成、负载均衡等等功能，但是在我们的文章中，这些功能都不是我们要考虑的。没错，**这一章我将会带领大家为 nacos 服务端自定义一个非常基础的集群，保证集群的数据完备性和功能可用性**，在最基础的功能完善之后，我会逐渐丰富这个集群的内容和功能，努力为大家彻底揭开集群的面纱。

  

好了，现在要开始构建集群了，问题也就来了，该怎么构建这个集群呢？或者要怎么踏出第一步呢？也许大家对此没什么头绪，我的思路倒是比较清晰，因为我也是见多识广了，不管是抄袭还是借鉴吧，我写过的项目也有一些了，所以我对集群的构建可能看得更清楚一些。请大家想一想，集群是由什么构成的？**这不用说，肯定是由多个节点构成的**，这个没人反对吧？当然，你也可以说一个节点也是集群，只不过是单节点集群；没错，确实是这样，但你听我一句劝，没必要抬这个杠，单机启动就单机启动，没必要说启动了一个单节点集群。

好了，既然集群是由多个节点构成的，并且我们现在想要实现集群的两个功能，**希望集群之间的各个节点可以备份数据，也希望集群的节点可以稳定地提供服务**，这就给我们要定义的操作提供了一个很明显的线索，**那就是我们要构建的这个集群中的各个节点应该知道彼此的存在，也就是知道彼此的信息，确切地说，就是每一个节点的网络地址**。原因很简单，只有各个节点知道彼此的信息，才能根据这些信息进行网络通信，备份数据。这也就意味着，**集群中的每一个节点在启动的时候，就要把集群中全部节点的信息都收集起来，存储在自己内部**。需要和集群中的哪个节点通信的时候，就可以直接从这些信息中找到目标节点的网络地址，然后直接通信即可。

所以，**现在我们首先要做的就是让集群中的每一个节点在启动的时候，可以收集到集群中全部节点的信息，只有先实现了这个功能，后面的功能才有了可以实现的基础**。那怎么让集群中的节点在启动时收集到其他所有节点的信息呢？那方法就多了，市面上有很多方案随便你抄，你要是觉得抄袭不好听，那我们也可以称自己想借鉴某些方案的做法。当然，我们也没必要真的抄袭，眼下就有一个最简单最直接的方法，**我们完全可以给每个节点定义一个本地配置文件，比如说就把这个文件命名为 cluster.conf，然后把集群所有节点的信息都定义在这个集群配置文件中。当集群中每一个节点启动的时候，就可以让节点加载本地配置文件，把集群其他节点的信息加载到内存中，然后保存起来**。这个逻辑大家可以理解吧？

好了，现在我们从编程思路，或者说设计集群的原理上已经实现了每一个节点获得集群中其他节点信息的功能，那么当前节点知道了集群中其他节点的网络地址之后呢？这也没什么可说的，肯定就是可以通信了呗，**这也就意味着当前节点要和其他节点构建通信方式了。当然，通信方式也有很多，你可以使用 http，也可以使用 grpc，哪一种方式都可以，随你意愿即可**。一旦集群中的各个节点可以直接通信了，那么接下来能做的就多了。**首先集群中的各个节点可以互相通信，判断彼此是否仍然存在吧？也就是是否可以正常工作，实现这一点是很有必要的，不健康的节点肯定不能在集群中工作，也不能备份数据，所以要把不健康的节点从集群中排除**。然后集群就可以真正开始工作了，这就要求我们必须尽快集群的数据备份和功能可用这两个功能了。当然，还是没必要这么着急，让我们先回过头，把编码思路再梳理一下。

  

分析到这里，也许很多朋友都有一种感觉，那就是我们刚才分析的这些思路简直太顺畅了，好像根本不用怎么思考，这些设计思路就自动涌现到脑海中了，一切都是那么顺理成章。也许每一位朋友的体会都不一样，但就我自己而言，我是真的有这种感觉，好像一个集群天生就该这么构建：**先定义本地配置文件，存储集群中所有节点的信息，在集群启动的时候让每个节点可以根据配置文件收集到其他节点的信息；再根据其他节点的信息，让当前节点和其他节点建立连接，构建通信渠道；然后定期检查集群中是否存在不健康的节点，把不健康的节点从集群中排除。这一切都执行完毕之后，集群就可以真正开始工作了**。当然，经验丰富的朋友可能会认为，这些操作流程并不复杂，因为这就是一个集群正常启动时必须执行的操作，我们刚才分析的正是集群启动时应该执行的各种必要操作。那接下来就让我们先把集群如何工作放到一边，先把集群启动的全流程给实现了，然后再考虑其他的事情。

## 引入 Member 类，将服务器抽象为集群的节点

在具体开始实现集群启动功能之前，让我再来为大家梳理一下，实现该功能时要执行的几个关键操作：

**1 定义本地配置文件，也就是 cluster.conf 文件，然后把集群所有节点的配置信息都定义在这个配置文件中。**

**2 在节点启动的时候，把 cluster.conf 配置文件的内容加载到内存中，并且保存配置文件中的节点信息。**

**3 根据配置文件中的节点信息，为当前节点和集群中的其他节点构建网络连接，使当前节点可以和集群中的其它节点进行网络通信。**

**4 定期判断集群中是否存在不健康的节点，如果存在就把该节点从集群中移除。**

在分析了以上 4 个操作之后，接下来就可以着手实现了它们了。第一个操作非常容易就能实现，cluster.conf 集群配置文件随时都可以定义，然后把集群其他节点的信息写在配置文件中就行。比如我们就可以把 cluster.conf 配置文件定义成下面这样，请看下面代码块。

```
//可以定义成下面这样
192.168.71.51:8848
192.168.71.52:8848
192.168.71.53:8848
192.168.71.54:8848
192.168.71.55:8848


//也可以把节点信息都写在一行，也就是定义成下面这样
//各个节点的信息用逗号分隔开
ip1:port,ip2:port。。。。
```

以上代码块展示的就是我们定义完毕的 cluster.conf 集群配置文件，文件定义完毕之后，就意味着第一步操作就已经实现完毕了。**接下来就该实现第二个操作，在节点启动的时候，把配置文件信息读取到内存中，然后解析配置文件，把集群其他节点的信息保存起来了**。这个时候，一个新的问题就来了：**那就是节点信息该怎么解析呢？如果问题更准确一点的话，就是该怎么把服务器抽象成集群的一个节点呢**？这个问题大家应该都能理解吧？因为一个服务器只有抽象成集群中的一个节点，它才能按照集群的模式发挥具体作用，这一点在我们之前实现 Jraft 框架的时候已经体现得很明显了。所以，接下来我们肯定要先定义一个组件，用这个组件来把服务器抽象成集群中的节点。这个组件我也定义好了，**其实就是一个非常简单的类，就是 Member 类，这个类中定义了几个成员变量，用来封装当前服务器的网络地址，当前节点的健康状态也定义在这个类中了**，该类的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.core.cluster;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/17
 * @Description:集群成员信息类，该类的对象封装了集群中一个成员的信息，就表示一个集群中的节点
 */
public class Member implements Comparable<Member>, Cloneable, Serializable {


    private static final long serialVersionUID = -6061130045021268736L;

    //当前集群成员的IP地址
    private String ip;

    //集群成员的端口号
    private int port = -1;

    //集群成员的状态，默认为上线状态
    private volatile NodeState state = NodeState.UP;

    //集群成员的扩展信息
    private Map<String, Object> extendInfo = Collections.synchronizedMap(new TreeMap<>());

    //集群成员的网络地址，其实就是完整的网络地址，IP+端口号
    private String address = "";

    //表示当前集群成员通信失败次数，当前节点和集群其他成员通信失败次数到达限制之后
    //当前节点的状态就会更新为不健康状态，从集群中移除了
    private transient int failAccessCnt = 0;

    //该成员变量表示当前集群成员是否使用grpc进行信息汇报，其实就是使用grpc方式和集群其他成员通信
    //因为在nacos服务端支持http通信和grpc通信，默认使用grpc通信方式
    //该标志为true，就意味着后面为当前节点和其他节点构建连接时，使用的是grpc框架构建的
    private boolean grpcReportEnabled;


    //各个成员变量的get、set方法，我就不在文章中展示了，大家直接看我提供的第十版本代码即可


    //得到MemberBuilder对象的方法，MemberBuilder构建者就是用来构建Member对象的
    public static MemberBuilder builder() {
        return new MemberBuilder();
    }



    //Member构建器对象，这个类中的内容都很简单，所以我就不添加详细注释了
    public static final class MemberBuilder {


        private String ip;


        private int port;


        private NodeState state;


        private Map<String, String> extendInfo = Collections.synchronizedMap(new TreeMap<>());


        private MemberBuilder() {
        }


        public MemberBuilder ip(String ip) {
            this.ip = ip;
            return this;
        }


        public MemberBuilder port(int port) {
            this.port = port;
            return this;
        }


        public MemberBuilder state(NodeState state) {
            this.state = state;
            return this;
        }


        public MemberBuilder extendInfo(Map<String, String> extendInfo) {
            this.extendInfo.putAll(extendInfo);
            return this;
        }


        public Member build() {

            Member serverNode = new Member();
            if (Objects.nonNull(this.extendInfo)) {
                serverNode.extendInfo.putAll(this.extendInfo);
            }

            serverNode.state = this.state;
            serverNode.ip = this.ip;
            serverNode.port = this.port;
            serverNode.address = this.ip + ":" + this.port;
            return serverNode;
        }
    }
    
}
```

好了，上面代码块展示的就是 Member 类的核心内容了，随着 Member 类的定义，标志着我们可以把服务器抽象成集群中的节点了。但是这个时候又有一个新的问题了：**我们都知道要把服务器抽象成集群中的一个节点，这样服务器才可以遵循集群的模式发挥作用，但是，真要到了解析的时候，该怎么解析呢**？**cluster.conf 集群配置文件中定义的都是集群中每一个节点的网络信息，那怎么把每个节点的网络信息解析成一个个 Member 对象呢**？这个时候其实就没有什么特别好的方法了，就只能按照最常规的逻辑，定义一个解析集群节点信息的方法来解析了：**当节点启动时会把 cluster.conf 配置文件中的信息加载到内存里，并且以字符串的形式加载到内存中，那么我们要定义的解析集群节点信息的方法其实就是解析字符串的方法，根据字符串中的内容得到集群中每一个节点的 IP 地址和端口号**。这个方法我也定义完毕了，其实我不仅把方法定义完毕了，**我还新引入了一个 MemberUtil 工具类，从名字上就可以看出来，这个工具类专门就是为 Member 类服务的。我把具体解析节点信息的方法定义为了 singleParse() 方法，就定义在了 MemberUtil 工具类中**，具体实现请看下面代码块。

```
package com.cqfy.nacos.core.cluster;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/17
 * @Description:集群成员对象工具类
 */
public class MemberUtil {

    //集群客户端和服务器连接失败时，会抛出该异常信息
    protected static final String TARGET_MEMBER_CONNECT_REFUSE_ERRMSG = "Connection refused";

    //服务端口号
    private static final String SERVER_PORT_PROPERTY = "server.port";

    //服务器默认端口号
    private static final int DEFAULT_SERVER_PORT = 8848;

    //raft集群中，服务器端口号偏移量
    private static final int DEFAULT_RAFT_OFFSET_PORT = 1000;

    //集群客户端访问服务端最大失败次数的key
    private static final String MEMBER_FAIL_ACCESS_CNT_PROPERTY = "nacos.core.member.fail-access-cnt";

    //这个就是集群客户端访问服务端最大失败次数，通过上面的key可以得到这个值
    private static final int DEFAULT_MEMBER_FAIL_ACCESS_CNT = 3;



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:把从集群配置文件中读取到内存的集群成员字符串信息，解析为一个个Member对象，然后返回这个Member对象集合
     * 注意，该方法会在cluster.conf文件被加载到内存后调用，cluster.conf文件被读取到内存后，文件中的各个节点信息都被封装在了
     * Collection<String>字符串集合中，集合中的每一个字符串都代表一个节点的信息，当前方法就是把字符串集合中的每一个字符串解析为一个个Member对象
     */
    public static Collection<Member> readServerConf(Collection<String> members) {

        Set<Member> nodes = new HashSet<>();
        for (String member : members) {
            //解析字符串为Member对象的操作
            Member target = singleParse(member);
            nodes.add(target);
        }
        return nodes;
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:解析集群节点信息的方法，方法中传入的参数就是集群节点的网络地址信息，该方法所做的就是把这些信息封装到一个Member对象中
     */
    @SuppressWarnings("PMD.UndefineMagicConstantRule")
    public static Member singleParse(String member) {
        //得到当前解析的集群节点的默认端口号，这里默认为8848
        //我想跟大家简单解释一下，也许大家看到8848会觉得很奇怪，会困惑为什么集群节点所部属的服务器的端口号和nacos控制台服务器的端口号一样
        //这是因为最终得到集群节点所部属的服务器的端口号时，就是以8848和一个偏移量的差值来计算的。这一点大家在阅读nacos注册中心客户端的代码时应该也见到了
        int defaultPort = EnvUtil.getProperty(SERVER_PORT_PROPERTY, Integer.class, DEFAULT_SERVER_PORT);
        //这里得到当前正在解析的集群节点的网络地址，接下来就要解析这个地址了
        String address = member;
        //如果用户没有设置端口号，就会使用默认端口号8848
        int port = defaultPort;
        //分割网络地址，得到IP地址和端口号数组
        String[] info = InternetAddressUtil.splitIPPortStr(address);
        if (info.length > 1) {
            address = info[0];
            port = Integer.parseInt(info[1]);
        }
        //在这里构建了一个Member对象，然后设置了该对象的IP地址和端口号，以及集群节点状态
        Member target = Member.builder().ip(address).port(port).state(NodeState.UP).build();
        //定义集群节点扩展信息对象
        Map<String, Object> extendInfo = new HashMap<>(4);
        //设置raft端口号，在第十版本还用不下面这些信息，等构建raft集群的时候，就会用上了
        extendInfo.put(MemberMetaDataConstants.RAFT_PORT, String.valueOf(calculateRaftPort(target)));
        //设置准备上线标志
        extendInfo.put(MemberMetaDataConstants.READY_TO_UPGRADE, true);
        target.setExtendInfo(extendInfo);
        //这里设置的就是当前节点集群客户端会使用grpc方式和集群其他节点通信
        target.setGrpcReportEnabled(true);
        return target;
    }



    //其他内容暂时不做实现


}
```

好了到此为止，我们就把节点信息解析成 Member 对象的方法也定义完毕了。**接下来我们要做的就是把 cluster.conf 配置文件的内容读取到内存中，然后把解析配置文件之后得到的 Member 对象集合保存起来就行**。那这个该怎么实现呢？

## 引入 ServerMemberManager 集群成员管理器

  

其实把解析配置文件之后得到的 Member 对象集合保存起来的操作很容易实现，反正封装了集群所有节点信息的 Member 对象集合已经得到了，直接保存这个集合就行，之后要是想使用集群中某个节点的信息，直接到该集合中查询即可。问题就是我们该怎么保存这个 Member 对象集合，如果是我的话，**我肯定愿意再给我们构建的集群引入一个新的组件，就把这个组件定义为 ServerMemberManager，翻译过来就是成员管理器的意思**。这样一来我们的程序中就有了一个专门的组件来管理集群中的所有节点。当然，目前要实现的 ServerMemberManager 集群成员管理器还非常简单，它只需要可以保存我们得到的 Member 对象集合即可。这个 ServerMemberManager 集群成员管理器我也实现完毕了，请看下面代码块。

```
package com.cqfy.nacos.core.cluster;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/17
 * @Description:集群成员节点管理器，注意这个集群成员节点管理器实现了springboot的ApplicationListener监听器接口，并且监听WebServerInitializedEvent事件
 */
@Component(value = "serverMemberManager")
public class ServerMemberManager {

    //该成员变量存放的就是集群中每个节点的信息，key就是集群节点的网络地址，value就是封装着节点信息的Member对象
    private volatile ConcurrentSkipListMap<String, Member> serverList;

    //存放集群节点的IP地址的集合，这里我还要多解释一句，这个set存放的其实都是节点状态为NodeState.UP的节点IP地址
    private volatile Set<String> memberAddressInfos = new ConcurrentHashSet<>();

    //标识当前节点是否在集群集合列表中
    private static volatile boolean isInIpList = true;

    //当前集群节点端口号
    private int port;

    //当前集群节点的网络地址，就是IP地址+端口号
    private String localAddress;

    //当前节点
    private volatile Member self;



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:构造方法
     */
    public ServerMemberManager(ServletContext servletContext) throws Exception {
        this.serverList = new ConcurrentSkipListMap<>();
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/18
     * @Description:更新集群成员信息的方法，该方法可以说是本类中非常核心的方法，该方法传入的参数就是最新的集群全部成员的信息
     */
    synchronized boolean memberChange(Collection<Member> members) {

        //对传入的集群最新成员信息进行判空
        if (members == null || members.isEmpty()) {
            return false;
        }

        //判断传入的集群最新成员信息是否包含当前节点
        boolean isContainSelfIp = members.stream().anyMatch(ipPortTmp -> Objects.equals(localAddress, ipPortTmp.getAddress()));

        //如果当前节点仍然在集群中，那就更新isInIpList标志
        if (isContainSelfIp) {
            isInIpList = true;
        } else {
            //如果当前节点不在集群中，那就设置isInIpList标志为fasle
            isInIpList = false;
            //然后把当前节点添加到最新的集群中
            members.add(this.self);
            //记录告警日志
            Loggers.CLUSTER.warn("[serverlist] self ip {} not in serverlist {}", self, members);
        }

        //这里就是比较一下旧的集群成员信息和最新的集群成员信息在数量上是否相等
        //不相等则意味着集群成员信息发生了变化
        boolean hasChange = members.size() != serverList.size();
        //定义一个map存放集群成员信息
        ConcurrentSkipListMap<String, Member> tmpMap = new ConcurrentSkipListMap<>();
        //定义一个集合，存放集群成员的IP地址
        Set<String> tmpAddressInfo = new ConcurrentHashSet<>();
        //遍历最新的集群成员信息
        for (Member member : members) {
            //得到当前遍历到的节点的网络地址
            final String address = member.getAddress();
            //然后判断当前节点是否也包含在旧的集群成员信息中
            Member existMember = serverList.get(address);
            //如果不存在，则意味着当前遍历到的节点是新添加的
            if (existMember == null) {
                //在这更新集群发生变更的标志
                hasChange = true;
                //把新节点添加到刚才创建的map中
                tmpMap.put(address, member);
            } else {
                //走到这里意味着当前遍历到的节点本身就是旧的集群成员中的节点
                //那也把当前遍历到的节点添加到刚才定义的map中
                tmpMap.put(address, existMember);
            }
            //判断当前遍历到的节点的状态是否健康，如果健康则添加到刚才定义的存放节点IP地址的集合中
            if (NodeState.UP.equals(member.getState())) {
                tmpAddressInfo.add(address);
            }
        }

        //把最新的集群成员信息赋值给serverList成员变量
        //这时候集群的成员信息就更新完毕了
        serverList = tmpMap;
        //集群节点的IP地址集合也更新完毕了
        memberAddressInfos = tmpAddressInfo;
        
        //返回集群节点成员信息是否发生变更的标志
        return hasChange;
    }
    
}
```

上面代码块中的注释非常详细，所以我就不再重复讲解其中方法的逻辑了。**唯一一点需要注意的就是虽然程序通过 MemberUtil 的 singleParse() 方法得到了存放集群所有节点的 Member 对象集合，但是最终把这个 Member 对象集合交给 ServerMemberManager 集群成员管理器来管理时，并不是直接把集合交给 ServerMemberManager 管理器**。在上面的代码块中我们可以看到：**ServerMemberManager 管理器中定义了一个 serverList 成员变量，这个成员变量是一个 map 类型，而且我们还在 ServerMemberManager 管理器中定义了一个 memberChange() 方法，该方法的作用就是接收存放集群所有节点的 Member 对象集合，然后解析该集合，把集群中的所有节点以键值对的方式存放到集群成员管理器的 serverList 成员变量中**。这个逻辑大家一定要梳理清楚，也就是说，**最终让 ServerMemberManager 集群成员管理器来管理集群的所有节点时，需要调用集群成员管理器的 memberChange() 方法才行**。除此之外，还有一点需要注意，**那就是 ServerMemberManager 管理器还添加了 springboot 的 @Component 注解，这就意味着当前类的对象也是被 springboot 容器管理的**。这一点大家也要认识到位。

## 引入 FileConfigMemberLookup 集群成员查找器

好了，**到此为止我们就已经把解析 cluster.conf 配置文件的功能，把服务器抽象成 Member 对象的功能，以及把存放集群所有节点的 Member 对象集合交给 ServerMemberManager 集群管理器的功能都实现了**，只差最后一步，我们就可以完全实现之前定义的 4 个操作中的第二个，也就是：**在节点启动的时候，把 cluster.conf 配置文件的内容加载到内存中，并且保存配置文件中的节点信息**。**而差的最后一步就是定义一个配置文件加载器，把 cluster.conf 配置文件加载到内存中，然后把加载得到的字符串集合交给 MemberUtil 工具类解析即可**。而这个配置文件加载器我也早就定义好了，因为这个加载器的内容非常简单，就是把 cluster.conf 配置文件加载到内存中，然后解析配置文件即可，所以我很快就定义完毕了。只不过这个文件加载器的名字有些奇怪，**我把它定义为了 FileConfigMemberLookup，翻译过来就是文件地址寻址器，就是使用配置文件查找集群中各个节点地址的意思**。但为什么定义成这个名称，以及这个类继承了什么抽象父类，就等到大家先把该类的代码看过之后，我再来解释吧。请看下面代码块。

```
package com.cqfy.nacos.core.cluster.lookup;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/8/16
 * @方法描述：集群成员文件查找器，这个其实就是文件寻址的组件，这个类继承了AbstractMemberLookup抽象父类，所以我把抽象父类的内容也展示在代码块中了
 * 至于为什么要继承这个类，我很快就会为大家解释
 */
public class FileConfigMemberLookup extends AbstractMemberLookup {

    //文件查找器要查找的文件名称，当我们启动了nacos集群的时候，会把集群成员的网络信息写在这个文件中，文件寻址器，也就是当前类的对象会读取该文件
    //然后解析文件中的内容，把地址解析成一个个节点，封装到一个个Member对象中，然后把Member对象添加到集群成员管理器中，也就是ServerMemberManager对象中
    private static final String DEFAULT_SEARCH_SEQ = "cluster.conf";

    //文件监控器
    private FileWatcher watcher = new FileWatcher() {
        //当文件发生变化的时候，该方法就会被回调
        //监控的就是存储了集群成员信息的cluster.conf文件
        @Override
        public void onChange(FileChangeEvent event) {
            //这里就是从硬盘中读取cluster.conf文件的内容
            //也就是说只要这个文件监控器监控到cluster.conf文件发生变化了
            //这也就意味着集群配置信息发生变化了，这个时候就会重新加载cluster.conf文件到内存中
            //而集群的配置信息也会更新了
            readClusterConfFromDisk();
        }

        //表示该文件监控器是否对某个文件感兴趣的方法
        @Override
        public boolean interest(String context) {
            //其实就是判断传递进来的字符串是否包含cluster.conf文件名称
            return StringUtils.contains(context, DEFAULT_SEARCH_SEQ);
        }
    };


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：启动文件寻址器的方法
     */
    @Override
    public void doStart() throws NacosException {
        //从本地硬盘加载cluster.conf文件的内容到内存
        readClusterConfFromDisk();
        try {
            //注册文件监控器，监控cluster.conf文件是否发生变化
            WatchFileCenter.registerWatcher(EnvUtil.getConfPath(), watcher);
        } catch (Throwable e) {
            //出现异常则记录错误日志信息
            Loggers.CLUSTER.error("An exception occurred in the launch file monitor : {}", e.getMessage());
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：这个方法就是用来表示是否使用地址服务器来得到集群成员信息的。在我提供的第十版本代码中，我把地址服务器给省略了，也就是AddressServerMemberLookup类
     * 要是使用地址服务器来查找集群中所有成员的信息，这就比较麻烦了，这就要求你还要单独启动一个服务器，把集群成员信息存放到那个服务器上，然后让当前节点去访问新启动的服务器，这样才能查询到集群中所有结点的信息
     * 和文件寻址比起来显然太麻烦了，所以我就省略了，这个AddressServerMemberLookup类的内容也很简单，其实就是使用NacosRestTemplate，也就是http访问新启动的服务器，大家感兴趣的话可以自己看看源码
     */
    @Override
    public boolean useAddressServer() {

        //因为当前类的对象就是一个文件寻址器，所以这里直接返回false，表示不使用地址服务器来获得集群成员信息
        return false;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：销毁文件寻址器的方法
     */
    @Override
    protected void doDestroy() throws NacosException {
        //在这里注销了文件监控器
        WatchFileCenter.deregisterWatcher(EnvUtil.getConfPath(), watcher);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：从硬盘中加载cluster.conf文件，解析文件中的内容的方法
     */
    private void readClusterConfFromDisk() {
        Collection<Member> tmpMembers = new ArrayList<>();
        try {
            //加载cluster.conf文件中的内容到内存中，这里就是把文件中的集群成员地址的字符串信息存放到这个字符串集合中了
            List<String> tmp = EnvUtil.readClusterConf();
            //使用工具类解析地址集合，这个MemberUtil工具类会把集合中的每一个String字符串解析好，解析得到的结果
            //就是集群成员的网络地址信息，然后把信息封装到一个Member对象中，这就意味着一个集群成员解析完毕了
            //最后把解析得到的多个Member对象存放到一个集合中，再赋值给刚才定义的tmpMember变量
            tmpMembers = MemberUtil.readServerConf(tmp);
        } catch (Throwable e) {
            //解析失败则记录错误日志
            Loggers.CLUSTER.error("nacos-XXXX [serverlist] failed to get serverlist from disk!, error : {}", e.getMessage());
        }
        //执行解析之后的操作，这里就会把解析到的集群成员信息交给ServerMemberManager集群成员管理器管理
        afterLookup(tmpMembers);
    }
}





//下面就是AbstractMemberLookup抽象父类的内容


package com.cqfy.nacos.core.cluster;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/8/16
 * @方法描述：寻址器的抽象父类，抽象父类实现了MemberLookup接口
 */
public abstract class AbstractMemberLookup implements MemberLookup {

    //集群成员管理器
    protected ServerMemberManager memberManager;

    //表示寻址器是否开始工作的标志
    protected AtomicBoolean start = new AtomicBoolean(false);


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：给及群成员管理器赋值的方法，该方法什么时候被调用大家也可以思考一下，下一章就会实现对应功能调用该方法
     */
    @Override
    public void injectMemberManager(ServerMemberManager memberManager) {
        this.memberManager = memberManager;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：寻址器解析完毕集群成员之后要执行的方法
     */
    @Override
    public void afterLookup(Collection<Member> members) {
        //把解析完毕的集群成员交给集群成员管理器来管理
        this.memberManager.memberChange(members);
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：销毁寻址器的方法
     */
    @Override
    public void destroy() throws NacosException {

        if (start.compareAndSet(true, false)) {
            doDestroy();
        }
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：启动寻址器开始工作的方法
     */
    @Override
    public void start() throws NacosException {

        if (start.compareAndSet(false, true)) {
            doStart();
        }
    }


    protected abstract void doStart() throws NacosException;


    protected abstract void doDestroy() throws NacosException;
}

```

可以看到，**我们在 FileConfigMemberLookup 类中定义了一个 readClusterConfFromDisk() 方法，该方法就是把 cluster.conf 配置文件内容加载到内存的方法，除此之外还有一个 doStart() 方法，在该方法中会调用 readClusterConfFromDisk() 方法，这就意味着只要 FileConfigMemberLookup 文件地址寻址器的 doStart() 方法一被调用，文件地址寻址器一旦开始工作，就会把 cluster.conf 配置文件内容加载到内存中**。我相信这个逻辑每一位朋友都能看明白，但现在的问题是这个 FileConfigMemberLookup 文件寻址器的 doStart() 方法什么时候会被调用呢？而且 FileConfigMemberLookup 文件地址寻址器为什么要继承 AbstractMemberLookup 抽象父类呢？还有最重要的一个问题，FileConfigMemberLookup 文件地址寻址器对象什么时候被创建呢？接下来就让我为大家好好解释一番。

实际上在 nacos 自己构建的 distro 集群中，不仅仅会使用配置文件的方式收集集群所有节点信息，除此之外还定义了服务器寻址的方法来收集集群所有节点的信息。具体操作是这样的：**单独启动一个服务器，把集群成员信息存放到这个服务器上，然后让当前节点去访问新启动的服务器，这样才能查询到集群中所有节点的信息，具体方式就是使用 NacosRestTemplate，用 http 的方式访问单独启动的服务器，查询集群所有节点的信息**。**这些操作就被定义在了 AddressServerMemberLookup 类中，也就是服务器地址寻址器**，因为和文件寻址比起来显然太麻烦了，所以我就在第十版本把这个类省略了。但这个类 AddressServerMemberLookup 的内容很简单，大家感兴趣的话可以自己看看 nacos 源码。既然在 nacos 源码中存在多个寻址器收集集群所有节点的信息，所以就又定义了一个 AbstractMemberLookup 抽象父类，定义了寻址器的基础方法，在抽象类下面继续细分，定义了文件地址寻址器和服务器地址寻址器，现在大家应该清楚 FileConfigMemberLookup 文件地址寻址器继承 AbstractMemberLookup 抽象父类的原因了吧？如**果大家阅读上面代码块非常仔细的话，肯定可以在 AbstractMemberLookup 抽象类中看见一个 ServerMemberManager 集群成员管理器成员变量，以及一个 afterLookup() 方法，该方法就是把集群所有节点的 Member 对象集合交给 ServerMemberManager 集群成员管理器管理的方法**。到此为止，所有操作是不是都联系起来了？大家可以仔细品味品味整体的流程。

当然，现在还有两个问题没有解决，那就是不知道 FileConfigMemberLookup 文件地址寻址器什么时候被创建，以及该寻址器的 doStart() 方法什么时候被调用，只有文件地址寻址器被创建了，然后调用了该寻址器的 doStart() 方法，文件地址寻址器才能开始工作，这样一来，程序才会按照我们预想的那样执行：**文件寻址器把 cluster.conf 配置文件的集群节点信息读取到内存中，然后把加载得到的数据交给 MemberUtil 工具类解析，得到存放集群所有节点的 Member 对象集合，再调用文件寻址器抽象父类 AbstractMemberLookup 的 afterLookup() 的方法把集群的所有节点交给 ServerMemberManager 集群成员管理器来管理**。这个逻辑现在已经非常清晰了吧？所以我们目前只要解决了剩下的两个问题，寻找合适的时机创建 FileConfigMemberLookup 文件地址寻址器，然后调用该寻址器的 doStart() 方法即可，但这两个问题在本章是解决不了了，还要引入新的组件，然后重构已有的组件，内容比较多，所以就留到下一章讲解吧。诸位，我们下一章见！