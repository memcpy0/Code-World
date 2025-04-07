## 引入 RaftConfig 配置类

上一章我已经为大家引入 nacos 服务端的 Raft 集群要使用的状态机组件，也就是 NacosStateMachine 类，我把该类的内容搬运过来了，帮助大家简单回顾一下，请看下面代码块。

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

除了 nacos 要使用的状态机组件，我还为大家引入了 nacos 服务端各个数据类型对应的数据处理器组件，也就是 RequestProcessor 抽象类，当然，我并没有把各个数据处理器都实现了，而是只实现了处理持久化服务实例信息的数据处理器，也就是 PersistentClientOperationServiceImpl 类。我把该类的内容也搬运过来了，请看下面代码块。

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

**在我们为 nacos 服务端的每一种数据构建 Raft 集群时，会为这些数据构建不同的 Raft 集群组，使用 sofajraft 框架构建即可。在构建每一个集群组的时候，还会为每个集群组都创建一个 NacosStateMachine 状态机对象，与此同时还会把各个集群组要使用的数据处理器对象创建出来，交给 NacosStateMachine 状态机对象使用。这样一来，每一个 Raft 集群组的状态机就可以处理自己的数据了**。

好了，关于为 nacos 构建的 Raft 集群组要使用的状态机就分析到这里吧，接下来就可以正式为 nacos 服务端构建 Raft 集群组了。这时候我们就可以按照集群工作流程来分析了，首先要把集群中各个节点的信息收集完毕，然后根据集群节点的各个信息构建 Raft 集群，然后启动 Raft 集群，接着是领导者选举等等工作。其实分析到这里大家应该也能意识到，**使用 jraft 框架构建 Raft 集群是一件非常简单省事的工作，因为我们只需要把 Raft 集群各个节点的信息交给 jraft 框架就行了，剩下的一切工作 jraft 框架都会为我们完成**。当然，通过之前对 jraft 框架的学习，大家应该也都清楚 jraft 框架在构建 Raft 集群的过程中做了什么。**所以我们现在要做的就是把要构建为 Raft 集群的各个服务器的信息收集起来即可，也就是获得 Raft 集群配置信息**。

这就很好办了，**比如说我们就可以先定义一个 RaftConfig 类，这个类就是 jraft 框架构建 Raft 集群的配置信息类，这个类的对象中封装了 Raft 集群中所有节点的信息**，可以定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:jraft要用到的配置信息类
 */
@Component
@ConfigurationProperties(prefix = RaftSysConstants.RAFT_CONFIG_PREFIX)
public class RaftConfig implements Config<RequestProcessor4CP> {
    
    private static final long serialVersionUID = 9174789390266064002L;

    //当前节点自己的信息
    private String selfAddress;

    //raft集群中所有节点的信息都以字符串的形式存放在这个集合中
    private Set<String> members = Collections.synchronizedSet(new HashSet<>());

    //把当前节点的信息，以及raft集群的配置信息设置到RaftConfig对象中
    @Override
    public void setMembers(String self, Set<String> members) {
        this.selfAddress = self;
        this.members.clear();
        this.members.addAll(members);
    }
    
    @Override
    public String getSelfMember() {
        return selfAddress;
    }
    
    @Override
    public Set<String> getMembers() {
        return members;
    }


    //该类的其他方法省略
}
```

好了，现在 RaftConfig 类定义完毕了，并且我刚才也跟大家说了，**这个类的对象中封装了 Raft 集群中所有节点的信息，当我们要使用 jraft 框架构建 Raft 集群时，就可以把这个 RaftConfig 配置类的对象交给 jraft 框架使用**。这一点理解起来并没有什么难度，但是我们刚才的问题还没有解决呢？究竟该怎么收集 Raft 集群所有节点的信息呢？只有把这些节点信息收集起来，交给 RaftConfig 配置类的对象保管，jraft 才能使用 RaftConfig 对象构建并启动 Raft 集群呀。

这个时候就让我们来回顾一下 Distro 集群的节点信息是怎么收集的吧，这个很简单，**我们会在 nacos 服务器中定义一个本地文件，也就是 cluster.conf 集群配置文件，这个集群配置文件中就定义了 Distro 集群所有节点的信息**，这个大家应该还记得吧？**当我们构建 Distro 集群的时候，会先启动集群节点寻址器，在我为大家提供的代码中，我实现的是 FileConfigMemberLookup 文件寻址器，这个寻址器一旦开始工作，就会从本地的 cluster.conf 集群配置文件中把所有节点信息加载到内存中，最后把收集到的节点信息交给 ServerMemberManager 集群成员管理器来管理**。这个流程想必大家都还记得吧？如果有朋友忘记了可以回顾一下之前的文章，我就不在本章中展示具体的代码了。总之，**当 Distro 集群构建完毕之后，该集群的所有节点信息都会保存在 ServerMemberManager 集群成员管理器中**。

很好，这一点清楚了之后，那么请大家想一想，**Distro 集群的节点信息是不是就是 nacos 的 Raft 集群的节点信息**？假如我们启动了 5 台 nacos 服务器，这 5 台服务器是一个 Distro 集群，Distro 集群用来备份 nacos 服务端的临时服务实例信息，现在要为 nacos 服务端构建 Raft 集群了，用 Raft 集群来备份持久服务实例信息，难道这个时候 nacos 服务器就会减少到三台了吗？肯定不会这样吧？仍然是 5 台 nacos 服务器，我们要用这 5 台 nacos 服务器构建 Raft 集群。如果是这样的话，**那直接从 ServerMemberManager 集群成员管理器中得到 Distro 集群成员的信息，然后把这些信息交给 RaftConfig 对象来管理，这样不就得到了要构建的 Raft 集群的配置信息了吗**？这个逻辑并不难理解吧？**所以现在我们要做的就是 ServerMemberManager 集群成员管理器中获得所有成员的信息，然后把这些信息交给 RaftConfig 对象来保管，这样 jraft 框架就可以使用 RaftConfig 配置对象成功创建 Raft 集群了**。那这个操作该怎么实现呢？这个时候我劝大家先别急，先让我们思考另一个问题，**那就是当我们使用 RaftConfig 对象成功收集到了 Raft 集群的配置信息之后，怎么把它交给 jraft 框架使用呢**？

  

## 引入 JRaftProtocol 和 JRaftServer 组件

我在文章中为大家反复强调过了，在 jraft 框架的帮助下，nacos 服务端 Raft 集群的构建变得异常简单，所以我想先把构建 Raft 集群时用到所有组件先定义出来，然后直接把它们组装一下，这些组件就可以协同工作了。而我在上一小节提出的问题：**也就是当我们使用 RaftConfig 对象成功收集到了 Raft 集群的配置信息之后，怎么把它交给 jraft 框架使用**？这个问题就会 nacos 服务端引出两个非常重要的 Raft 集群组件。

请大家想一想，如果我们要在 nacos 中使用 jraft 框架，应该怎么使用？一般来说是不是应该定义一个和 jraft 框架相关的组件，把 jraft 框架提供的服务封装在这个组件中，然后 nacos 直接通过这个组件使用 jraft 提供的功能，就像在 nacos 中使用 grpc 框架，也是定义了 grpc 的服务组件，这个可以理解吧？**就比如说我们可以定义一个 JRaftServer 类，在这个类中定义 jraft 框架构建 Raft 集群，启动 Raft 集群的功能，除了这两个功能，还可以定义 jraft 框架处理客户端请求的功能，比如说读请求或者写请求，这个也可以理解吧？总之我们定义的这个 JRaftServer 类就是纯粹的提供 jraft 框架功能的组件**。

如果 JRaftServer 组件的作用大家都清楚了，也都知道这个组件要创建并启动 Raft 集群，那么这个 JRaftServer 组件是不是肯定就要用到 RaftConfig 配置信息对象了？因为 Raft 集群配置信息都封装在这个对象中，而 jraft 框架只有得到集群配置信息之后，才能构建集群，启动集群，**所以这个 JRaftServer 组件显然就要获得 RaftConfig 配置信息对象**。就目前分析的情况来说，这个 JRaftServer 组件可以先简单定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/27
 * @Description:为nacos服务端提供了jraft集群服务的组件
 */
@SuppressWarnings("all")
public class JRaftServer {

    //封装了jraft集群配置信息的成员变量，创建raft集群的时候
    //就会根据这个配置信息类中的节点信息创建
    private Configuration conf;

    //jraft集群要使用的配置信息类
    private RaftConfig raftConfig;

    //当前节点的IP地址
    private String selfIp;

    //当前集群节点使用的端口号
    private int selfPort;

    //当前节点的网络地址，在jraft集群中使用PeerId对象来封装一个节点的信息
    private PeerId localPeerId;


    //请求处理器集合，每一个请求处理器处理的数据就对应一个jraft集群组
    //大家应该都还记得上一章的内容吧？也知道在nacos服务端有多个数据类型需要用Raft集群备份
    //每一种数据都有自己的数据处理器，也都有自己的集群状态机，这个集合中存放的就是各个数据的数据处理器
    //只不过在我们的文章中只实现了持久服务实例信息的数据处理器，也就是PersistentClientOperationServiceImpl对象
    private Collection<RequestProcessor> processors = Collections.synchronizedSet(new HashSet<>());


    //构造方法
    public JRaftServer() {
        //在这里创建了封装jraft集群配置信息的对象
        this.conf = new Configuration();
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:初始化jraft服务组件的方法，在这个方法中创建了jraft集群要使用的多个组件
     */
    void init(RaftConfig config) {

        //给jraft集群要使用的配置对象赋值
        this.raftConfig = config;

        //得到当前节点的网络地址
        final String self = config.getSelfMember();
        //得到当前节点IP地址和端口号数组
        String[] info = InternetAddressUtil.splitIPPortStr(self);
        //得到当前节点IP地址
        selfIp = info[0];
        //得到当前节点使用端口号
        selfPort = Integer.parseInt(info[1]);
        //得到当前节点的地址解析成jraft集群中的节点对象
        localPeerId = PeerId.parsePeer(self);
        
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:启动jraft集群组件的方法
     */
    synchronized void start() {

        //省略部分内容

        //得到jraft集群中的节点管理器
        com.alipay.sofa.jraft.NodeManager raftNodeManager = com.alipay.sofa.jraft.NodeManager.getInstance();
        //先得到集群中的所有成员，然后遍历成员，注意，这个时候大家应该对ServerMemberManager集群成员管理器和即将启动的jraft集群的区别有点认识了
        //ServerMemberManager集群成员管理器存放的也是集群中的所有节点信息，但是这个集群成员管理器更像是为nacos服务端定义的组件，是在nacos层面上存放集群所有节点信息的管理器
        //不管是distro集群还是jraft集群，它们节点的信息都会存放到ServerMemberManager管理器中，至于jraft的节点管理器，也就是刚才得到的raftNodeManager对象，存放的就是jraft集群
        //的节点信息
        for (String address : raftConfig.getMembers()) {
            //把当前节点的网络地址解析为jraft集群的节点对象
            //在jraft集群中，节点都是以PeerId对象的方式存在的
            PeerId peerId = PeerId.parsePeer(address);
            //把得到的jraft集群的节点添加到jraft的配置对象中
            //当这个for循环遍历完了，这个配置对象就封装了jraft集群的全部配置信息
            conf.addPeer(peerId);
            //把节点的网络地址添加的jraft的节点管理器中
            raftNodeManager.addAddress(peerId.getEndpoint());
        }


        //这里的操作非常重要，接下来，就让我好好跟大家分析一下，这里的操作有什么重要作用
        //如果大家把第12版代码本对应的文章全部看完了，现在一定知道在nacos服务端中存在多个数据处理器
        //比如处理nacos客户端注册过来的持久服务实例的PersistentClientOperationServiceImpl处理器
        //还有处理服务实例元数据的InstanceMetadataProcessor处理器，服务实例元数据就可以用来实现nacos灰度发布功能
        //还有处理Service，也就是服务元信息的ServiceMetadataProcessor处理器，所谓服务元信息就包含了健康的服务实例数量阈值，路由选择器等等，这个也是用来实现nacos灰度发布功能的关键组件
        //总之，我列举了这么多数据处理器，这就意味着nacos服务端要处理这些数据，而在nacos服务端，这些数据都是要被备份到集群的其他节点中的，所以要为这些数据搭建一致性集群进行备份
        //在nacos服务端为这些数据构建的就是jraft集群，但是请大家想一想，这些数据各不相同，最终处理的方式也各不相同。因为这些数据会被包装成日志，而日志最终会被应用到状态机上
        //如果是写数据的操作，最后应用状态机的时候，不同数据要执行的操作可能并不相同。nacos服务端为了把处理这些数据的操作隔离开，就为这每一个数据都构建了一个jraft集群
        //当然，这么说也不准确，实际上只构建了一个jraft集群，但是别忘了，jraft框架具有MultiRaftGroup的功能，也就是多个集群组的功能，这里就是使用了jraft框架的集群组的功能
        //为每一个要处理的数据都构建了jraft集群组，这些集群组都在一个jraft集群中，我们可以根据要处理的数据所在集群组的组名得到处理数据的集群组，然后再处理数据即可
        //这里执行的操作就是为多个数据处理器处理的数据分别创建jraft集群组
        //processors这个map中存放的就是nacos服务端处理不同数据的处理器
        createMultiRaftGroup(processors);

        


        //省略该方法的部分内容
    }



     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:为nacos多个数据处理器处理的数据分别创建jraft集群组的方法，这里还要补充一下
     * 该方法还会被各个数据处理器调用，也就是说，数据处理器调用这个方法主动为自己处理的数据构建jraft集群组
     */
    synchronized void createMultiRaftGroup(Collection<RequestProcessor> processors) {

        //在文章中展示的该方法中的逻辑是大大简化过的，也有一些伪代码
        
        //判断当前组件是否启动，如果没启动就把数据处理器添加到processors这个map中
        //然后等待当前组件启动的时候创建jraft集群组即可
        if (!this.isStarted) {
            this.processors.addAll(processors);
            return;
        }

        //省略中间的内容


        //遍历所有数据处理器，这里大家肯定有些疑惑，processors集合中的数据是怎么添加的呢？
        //我现在展示的代码并没有把nacos服务端的所有数据处理器都添加到processors成员变量中啊，这是怎么回事？
        //没有数据怎么能直接遍历吗？请大家一定要记住这个问题，最后就会解决它！
        for (RequestProcessor processor : processors) {

            //省略部分和jraft框架相关的操作

            //得到当前遍历到的数据处理器的组名，这个组名就是为它构建的jraft集群组的组名
            final String groupName = processor.group();
            //得到jraft节点配置信息
            NodeOptions copy = new NodeOptions();

            //创建当前集群组要使用的状态机，可以看到，这里把与当前构建集群组对应的数据处理器交给状态机使用了
            //因为最终处理数据还是要用数据处理器来处理，所以在状态机应用日志的时候，最终会调用数据处理器来处理数据
            NacosStateMachine machine = new NacosStateMachine(this, processor);
            //设置状态机到jraft节点配置信息中
            copy.setFsm(machine);
            //设置集群配置信息到节点配置信息中
            copy.setInitialConf(conf);

            //创建Raft集群组服务组件
             RaftGroupService raftGroupService = new RaftGroupService(groupName, localPeerId, copy, true);
            //在这里启动了集群组，然后返回了当前服务器在集群组中的节点对象，当前节点的网络地址就是刚才交给raftGroupService组件使用的localPeerId
            //这里的方法参数为false，是因为我们已经把jraft节点的服务器启动了，启动集群组的过程中就不用再次启动服务器了
            Node node = raftGroupService.start(false);
            
            //剩余部分逻辑全部省略，到此为止，大家应该就清楚nacos的Raft集群组是怎么创建的了
        }
    }

    
     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:jraft集群组处理读请求的方法，在该方法中并不会判断当前节点是领导者还是跟随者，而是直接使用线性一致读处理读请求
     * 如果线性一致读操作失败，再寻找集群组中的领导者，把读请求发送给领导者，让领导者处理读请求
     */
    CompletableFuture<Response> get(final ReadRequest request) {

        //该方法暂时不做实现
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:让集群组的领导者处理读请求的方法
     */
    public void readFromLeader(final ReadRequest request, final CompletableFuture<Response> future) {
        commit(request.getGroup(), request, future);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:提交读、写请求给jraft集群组领导者的方法，让集群组的领导者处理读请求
     */
    public CompletableFuture<Response> commit(final String group, final Message data,final CompletableFuture<Response> future) {

        //该方法暂时不做实现
        
    }


    //省略该类的其他内容
}
```

可以看到，在上面展示的 JRaftServer 组件中，定义了几个 Raft 集群处理数据要用到的方法，还把 jraft 框架构建 Raft 集群的关键对象，也就是 conf 成员变量定义出来了。而且从上面代码块中我们还可以看到，在 JRaftServer 组件初始化的时候，也就是调用了该组件的 init() 方法，就会把一个封装了 Raft 集群配置信息的 RaftConfig 对象赋值给 JRaftServer 的 raftConfig 成员变量。这样一来，等到调用了 JRaftServer 组件的 start() 方法，真正启动 JRaftServer 服务组件时，就可以从 raftConfig 成员变量中解析出 Raft 集群的每一个节点的信息，然后设置到 conf 成员变量中了。当 jraft 构建的 Raft 集群启动时，就会从 conf 成员变量中获得集群当前的配置信息。这也就是上面代码块中 createMultiRaftGroup() 方法的逻辑，该方法就是用来构建 Raft 集群组并启动每一个集群组的，代码块中的注释非常详细，所以我就不再赘述了，如果大家看得比较仔细，通过以上代码块，应该就能清楚 nacos 服务端 Raft 集群组具体的构建流程了。大家可以仔细品味品味这个流程。

当然，上面的代码块看完之后，我相信还会有朋友感到困惑，因为我现在为大家展示的 JRaftServer 组件根本没有定义多少和 jraft 框架有关的东西，这个我自己也知道，我不定义太多和 jraft 框架相关的组件是有原因的：首先我们本章的核心是使用 jraft 框架为 nacos 服务端构建 Raft 集群，这个是我们的目标，如果我现在就把 JRaftServer 中用到的 jraft 框架的各种组件都定义出来，然后一一分析讲解，这不就变成带领大家回顾 jraft 框架的知识了吗？这就偏离主题了。第二个原因就是 JRaftServer 类中定义的和 jraft 框架相关的组件确实有点多，我无法在文章中全部展示给大家，就比如说 jraft 框架要用到的 RpcServer 集群节点服务器，还有 CliService 集群客户端，以及用来得到和定期刷新 Raft 集群领导者的 RouteTable 路由表组件等等。这些我都没有定义出来，而这些组件的作用非常重要，就比如说nacos 服务端 Raft 集群的某个节点接收了一个写请求指令，写请求指令只能被集群领导者处理，如果当前节点不是领导者，肯定需要从 RouteTable 路由表组件中找到集群的领导者，然后把写请求转发给领导者，让领导者处理。这些都是我后来为大家补充的 sofajraft 框架第十三版本代码对应的内容，如果大家忘记了，可以去 sofajraft 课程中回顾回顾对应的文章和代码。总之，还是我之前那句话，大家一定要熟练掌握 jraft 框架之后再来阅读本章的内容，这时候大家会觉得本章内容非常简单。

好了，分析完 JRaftServer 组件内容简陋的原因之后，接下来让我们再来分析，**JRaftServer 组件的 init() 方法应该在什么时候被调用**。因为经过刚才的分析，我们都已经知道了，只有 JRaftServer 对象的 init() 方法被调用了，封装了 Raft 集群所有节点信息的 RaftConfig 配置对象才能交给 JRaftServer 组件使用，JRaftServer 组件才能在 createMultiRaftGroup() 方法中为每一种数据构建 Raft 集群组，然后启动集群。当然，我还是要再多说一句，**在 JRaftServer 对象的 createMultiRaftGroup() 中，在开始遍历封装了各个数据处理器的 processors 成员变量时，这里大家肯定会对 processors 集合中的数据是怎么添加的感到疑惑？我在 JRaftServer 类中并没有展示任何能把 nacos 服务端的所有数据处理器都添加到 processors 成员变量中代码和方法，那这是怎么回事呢**？还会我写在代码注释中的那句话，请大家一定要记住这个问题，最后我就会解决它。好了，言归正传，还是让我们继续分析JRaftServer 组件的 init() 方法应该在什么时候被调用吧，**其实这时候就要引入另一个组件了，也就是标题中展示的 JRaftProtocol 类**。

其实这个 JRaftProtocol 组件存在的原因非常简单，就像 Distro 集群拥有一个用来负责 DIstro 协议层面操作的 DistroProtocol 组件一样，nacos 的 Raft 集群也有一个用来负责 Raft 协议层面操作的组件。注意，**我这里说的用来负责 Raft 协议层面操作指的并不是和 jraft 框架提供的关于 Raft 协议的相关操作，如果是这样的话直接使用 JRaftServer 组件不就完了**？**这里的 Raft 协议层面的操作指的是 nacos 业务层面和 Raft 协议层面衔接的一些操作，也就是说我们新定义的这个 JRaftProtocol 组件要在 nacos 的业务层面被使用，而这个 JRaftProtocol 组件被使用之后，程序就会从 nacos 业务层面转到了 Raft 集群层面了**。光这么说肯定让大家有些模糊，所以接下来我直接把 JRaftProtocol 类的代码给大家展示一下，大家就清楚了，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/27
 * @Description:该类的对象提供了和raft共识算法有关的服务，确切地说，它提供了和jraft集群的有关服务
 */
@SuppressWarnings("all")
public class JRaftProtocol{


    //表示JRaftProtocol对象是否初始化完毕的标志
    private final AtomicBoolean initialized = new AtomicBoolean(false);

    //jraft集群要用到的配置对象
    private RaftConfig raftConfig;

    //为nacos服务器提供jraft集群服务的组件
    private JRaftServer raftServer;

    //构造方法
    public JRaftProtocol() throws Exception {
        //在这里创建了jraft服务组件
        this.raftServer = new JRaftServer();
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:初始化JRaftProtocol对象的方法，该方法会在ProtocolManager类的initCPProtocol()方法中被调用
     */
    public void init(RaftConfig config) {
        //保证JRaftProtocol对象只被初始化一次
        if (initialized.compareAndSet(false, true)) {
            //给jraft集群要使用的配置信息对象赋值
            this.raftConfig = config;
            //在这里初始化jraft服务组件
            this.raftServer.init(this.raftConfig);
            //启动jraft服务组件
            this.raftServer.start();
        }

        //该方法其他内容省略
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:为某个数据处理器创建jraft集群组的方法，这个方法非常重要，大家要先混个眼熟
     */
    @Override
    public void addRequestProcessors(Collection<RequestProcessor4CP> processors) {
        raftServer.createMultiRaftGroup(processors);
    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:从jraft集群中读取数据的方法
     */
    @Override
    public Response getData(ReadRequest request) throws Exception {

        CompletableFuture<Response> future = aGetData(request);
        //限时等待读取的结果
        return future.get(5_000L, TimeUnit.MILLISECONDS);
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:从jraft集群中读取数据的方法
     */
    @Override
    public CompletableFuture<Response> aGetData(ReadRequest request) {
        return raftServer.get(request);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:向jraft集群中写入数据的方法
     */
    @Override
    public Response write(WriteRequest request) throws Exception {
        CompletableFuture<Response> future = writeAsync(request);
        //限时等待操作结果
        return future.get(10_000L, TimeUnit.MILLISECONDS);
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:向jraft集群中写入数据的方法
     */
    @Override
    public CompletableFuture<Response> writeAsync(WriteRequest request) {
        //在这里把写请求提交给jraft集群服务组件处理
        return raftServer.commit(request.getGroup(), request, new CompletableFuture<>());
    }


    //该类的其他内容省略
    
}
```

通过以上代码块可以看到，**JRaftProtocol 对象持有了 JRaftServer 对象，这样一来，nacos 业务层面的操作就可以通过 JRaftProtocol 对象交给 Raft 层面的 JRaftServer 组件去执行，而且我们还可以看到在 JRaftProtocol 类中也定义了一个 RaftConfig 成员变量，以及一个 init() 方法，并且也是 init() 方法被调用之后，JRaftProtocol 对象的 RaftConfig 成员变量就会被赋值了**；**同时在 init() 方法中，还会对 JRaftServer 组件进行初始化操作，然后启动 JRaftServer 服务，开始构建 Raft 集群并且启动 Raft 集群**。那现在问题就变成了 JRaftProtocol 对象的 init() 方法在什么时候被调用呢？只有这个 JRaftProtocol 对象的 init() 方法被调用了，JRaftProtocol 才会获得封装了 Raft 集群配置信息的 RaftConfig 对象，然后 JRaftServer 才能获得对应的 RaftConfig 对象，Raft 集群才能被顺利构建和启动。这一连串的流程都要依靠这个 RaftConfig 对象，那接下来，我就为大家展示一下 JRaftProtocol 对象是如何被创建的，又是在什么时候获得了 RaftConfig 对象。**这时候我就不卖关子了，直接引入 ProtocolManager 协议管理器对象，在 ProtocolManager 协议管理器对象中创建 JRaftProtocol 对象，然后从 ServerMemberManager 中获得 Raft 集群需要的配置信息，封装到 RaftConfig 对象中，再把 RaftConfig 对象交给 JRaftProtocol 对象使用**。具体内容请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:一致性协议管理器，这个管理器本来负责管理CP一致性和AP一致性，但是nacos中默认使用的是CP一致性
 * 也就是jraft组件，所以我就把AP协议组件给省略了
 */
@SuppressWarnings("all")
@Component(value = "ProtocolManager")
public class ProtocolManager{

    //CP协议组件，这个成员变量会被JRaftProtocol对象赋值
    private CPProtocol cpProtocol;

    //集群成员管理器，该对象会被springboot自动注入
    private final ServerMemberManager memberManager;

    //CP协议组件是否初始化完毕的标志
    private boolean cpInit = false;

    //构造方法
    public ProtocolManager(ServerMemberManager memberManager) {
        this.memberManager = memberManager;
    }


    //请按顺序阅读以下方法，方法阅读顺序就是程序执行的逻辑顺序，按顺序看完就明白了

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:得到CP协议组件，这里得到的就是JRaftProtocol对象，这里我要多解释一句，在我为大家提供的第12版本代码中
     * 只引入了一个PersistentClientOperationServiceImpl类，这个类就是用来处理nacos客户端注册的持久化的服务实例请求的
     * 这个类继承了RequestProcessor类，也就是说这个类是一个请求处理器。请求处理器对象会被springboot容器管理，也就是说springboot
     * 会主动创建请求处理器对象，在创建对象的过程中，就会调用当前协议管理器的getCpProtocol()方法，从springboot容器中得到JRaftProtocol
     * 对象，然后初始化该对象。在下面这个方法中，大家可以看到一个同步代码块，也许大家会好奇为什么这里要加锁。现在我给大家解释一下，实际上在nacos
     * 源码中，有多个请求处理器，也就是说RequestProcessor类有多个子类，这些子类分别处理不同的集群数据，而每一个请求处理器对象都会被springboot
     * 容器管理，它们在被springboot容器创建的时候，也都会调用协议管理器的getCpProtocol()方法。因此，程序中可能出现多次调用下面这个方法的情况
     * 因此使用同步代码块，防止JRaftProtocol对象重复初始化
     */
    public CPProtocol getCpProtocol() {
        synchronized (this) {
            //如果协议组件未初始化
            if (!cpInit) {
                //在这里进行初始化
                initCPProtocol();
                //设置初始化完成标志
                cpInit = true;
            }
        }
        return cpProtocol;
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:初始化CP协议组件的方法
     */
    private void initCPProtocol() {
        //从springboot容器中得到CPProtocol对象，这时候大家肯定很好奇，JRaftProtocol类上并没有什么springboot的注解
        //说明这个类的对象不被springboot容器管理，拿它是怎么被创建的呢？又是怎么被放到springboot容器中的呢？
        //请先记住这个问题，马上就会为大家解决
        ApplicationUtils.getBeanIfExist(CPProtocol.class, protocol -> {
            //得到CPProtocol类的泛型类型，因为该类的泛型类型就是该协议的配置类
            Class configType = ClassUtils.resolveGenericType(protocol.getClass());
            //这里就会根据类型得到具体的对象，得到的其实就是RaftConfig配置类
            Config config = (Config) ApplicationUtils.getBean(configType);
            //把集群节点信息设置到RaftConfig配置类对象中
            injectMembers4CP(config);
            //初始化JRaftProtocol对象
            protocol.init(config);
            //给当前类的成员变量赋值
            ProtocolManager.this.cpProtocol = protocol;
        });
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:把集群成员信息设置到jraft集群配置类的方法
     */
    private void injectMembers4CP(Config config) {
        //从集群成员管理器中得到当前节点
        final Member selfMember = memberManager.getSelf();
        //得到当前节点自己的网络地址，注意，这里得到的IP地址是当前节点在jraft集群中的网络地址
        //这里会通过selfMember.getExtendVal(MemberMetaDataConstants.RAFT_PORT)这行代码得到当前节点在jraft集群中的端口号
        //至于端口号是什么时候设置到当前节点的member对象中，具体逻辑请看MemberUtil类的singleParse()方法，这是第十章的知识
        final String self = selfMember.getIp() + ":" + Integer.parseInt(String.valueOf(selfMember.getExtendVal(MemberMetaDataConstants.RAFT_PORT)));
        //得到集群中的所有节点
        Set<String> others = toCPMembersInfo(memberManager.allMembers());
        //把当前节点和所有节点的信息设置到jraft配置类中
        config.setMembers(self, others);
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:把集群的每一个成员转换成jraft框架的节点，也就是把Member节点转换成jraft框架的node节点
     * 本质并没有改变，都是集群的一个节点而已，这里虽然没有直接转换成node节点，但是已经把集群成员信息收集了，这就是在为
     * 转换成jraft集群的node节点做准备
     */
    public static Set<String> toCPMembersInfo(Collection<Member> members) {
        Set<String> nodes = new HashSet<>();
        members.forEach(member -> {
            //得到节点的IP地址
            final String ip = member.getIp();
            //得到在jraft集群中的端口号
            final int raftPort = MemberUtil.calculateRaftPort(member);
            //添加到nodes集合中
            nodes.add(ip + ":" + raftPort);
        });
        return nodes;
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:终止JRaftProtocol对象工作的方法
     */
    @PreDestroy
    @Override
    public void destroy() {
        if (Objects.nonNull(cpProtocol)) {
            cpProtocol.shutdown();
        }
    }

    //该类的其他内容省略
}





//以下就是JRaftProtocol类的对象被创建的具体实现
//其实在nacos服务端有一个被springboot容器管理的配置类，在这个类中创建了JRaftProtocol类的对象，然后放到springboot容器中了
//这个类就是ConsistencyConfiguration，具体实现如下



package com.cqfy.nacos.core.distributed.raft;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:一致性组件配置类，这个配置类中创建了CP协议的分布式组件，也就是提供了jraft服务的对象
 */
@Configuration
public class ConsistencyConfiguration {

    //在这个方法中创建了JRaftProtocol对象，把对象交给springboot容器管理了
    @Bean(value = "strongAgreementProtocol")
    public CPProtocol strongAgreementProtocol(ServerMemberManager memberManager) throws Exception {
        final CPProtocol protocol = getProtocol(CPProtocol.class, () -> new JRaftProtocol(memberManager));
        return protocol;
    }
    
    private <T> T getProtocol(Class<T> cls, Callable<T> builder) throws Exception {
        Collection<T> protocols = NacosServiceLoader.load(cls);
        
        // Select only the first implementation
        
        Iterator<T> iterator = protocols.iterator();
        if (iterator.hasNext()) {
            return iterator.next();
        } else {
            return builder.call();
        }
    }
    
}
```

上面代码块中的内容非常详细，只要大家是按照方法顺序来阅读的，肯定就知道 JRaftProtocol 对象是怎么被创建的，也知道 JRaftProtocol 对象的 init() 方法是在哪里被调用的了。代码块中的注释非常详细，所以我就不再跟大家重复梳理逻辑了。总之，**只要在 ProtocolManager 的 initCPProtocol() 方法中调用 JRaftProtocol 对象的 init() 方法，程序就会像我们之前分析的那样运行，nacos 服务端各个数据的 Raft 集群组也就会被构建成功了。而 ProtocolManager 的 initCPProtocol() 方法又会在该类的 getCpProtocol() 方法中被调用，所以现在我们只需要知道该类的 getCpProtocol() 方法在什么时候被调用就行了**。这时候就需要重构一下处理持久服务实例信息的数据处理器了，也就是对 PersistentClientOperationServiceImpl 类进行重构。

根据之前我为大家展示的 PersistentClientOperationServiceImpl 类的代码块，大家可以知道该类上面添加了 springboot 的 @Component 注解，这也就意味着它的对象会被 springboot 容器管理，会被 springboot 自动创建。实际上，在该类中还定义了一个无参构造方法，在该方法中执行了一些重要操作，具体实现请看下面代码块。

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

    //在第12版本代码中只用到了这个协议组件，这个成员变量就会被JRaftProtocol对象赋值
    private final CPProtocol protocol;

    //springboot容器创建该类的对象时，会执行这个无参构造方法
    public PersistentClientOperationServiceImpl() {
        //在这里从容器中得到JRaftProtocol对象，然后执行了初始化操作
        this.protocol = ApplicationUtils.getBean(ProtocolManager.class).getCpProtocol();
        //在这里为处理注册持久的服务实例的请求创建了一个jraft集群
        this.protocol.addRequestProcessors(Collections.singletonList(this));
    }

    

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

从上面代码块中可以看到，**当 PersistentClientOperationServiceImpl 类的对象被 springboot 容器创建的时候，就会执行无参构造方法，而在执行无参构造方法的时候，就会从 springboot 容器中得到 ProtocolManager 对象，调用该对象的 getCpProtocol() 方法；紧接着还会把自己这个持久的服务实例信息处理器添加到 JRaftServer 对象的 processors 成员变量中**。现在大家应该清楚数据处理器是怎么添加到 JRaftServer 组件的 processors 成员变量中了吧？**在本章中我只为大家展示了持久服务实例信息的数据处理器，其实在其他数据的处理器中也是这么做的，它们也都被 springboot 容器管理，也会在自己的无参构造方法中执行上面代码块的构造方法中的操作，都是一样的，都是通过这种方式把自己添加到 JRaftServer 对象的 processors 成员变量中**。到此为止，本章展示的所有组件的逻辑就全都串联起来了，nacos 服务端的 Raft 集群也就构建完毕了，本章的内容也就到此为止了。

当然，目前我们只是把 Raft 集群构建完毕了，可能有很多构建过程中的细节大家都还不清楚，这没关系，只要大家完全掌握了 sofajraft 框架，那么大家就可以直接去看我提供的第十二版本代码了，代码中注释非常详细，我相信大家都能看懂其中的逻辑。当然，有些方法可能大家现在还不太明白怎么使用，比如 JRaftProtocol 对象的向 Raft 集群写入数据的 write() 方法，还有从 Raft 集群中读取数据的方法，这些大家可能都还不清楚怎么使用。因为我们虽然在本章成功构建并启动了 Raft 集群，但是并没有给大家提供一个 nacos 服务端 Raft 集群工作的具体例子，这没关系，下一章我就会为大家实现 nacos 客户端向服务端注册持久服务实例的功能了，到时候大家就知道持久服务实例的 Raft 集群组在 nacos 服务端究竟是怎么工作的了。好了朋友们，这一章就到此为止吧，我们下一章见！