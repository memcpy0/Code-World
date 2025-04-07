上一章我们实现了很多功能，成功把自己构建的集群启动起来了，这一章就该继续向下进行，实现集群的数据备份和功能可用两个特性了。但是在正式实现集群的这两个功能之前，我想先问问大家是否发现了了一个问题，**那就是我们目前构建的集群根本没有什么主节点和从节点的概念，也没有什么领导者和跟随者的概念**。这是否是一个潜在的隐患呢？意味着我们自己构建的集群在功能上存在很大的缺陷。

每一位经验丰富的朋友们肯定都知道，一般来说，我们在项目中构建了集群，集群中的节点肯定就会有主从身份，或者领导者和跟随者身份。而主从节点或者领导者和跟随者身份的好处是显而易见的，在这种模式下，**从节点或者跟随者会分摊集群中主节点或者领导者的流量，比如说可以让主节点或领导者只负责写入数据的操作，而从节点和跟随者节点可以提供读取数据的服务，这样一来，整个集群响应客户端请求的效率就会大大提高**。并且，主从模式和领导者和跟随者模式给数据备份提供一个很明显的方向，**在主从模式下，数据备份肯定是由主节点向从节点传输备份，在领导者和跟随者模式下，数据备份肯定是由领导者向跟随者传输备份**。这种模式还有一个好处，**那就是当集群的主节点或者领导者不能再发挥作用时，切换另一个健康节点继续提供服务时，选择目标节点的范围也很明确，肯定就是从跟随者或者从节点中选择一个即可**。但现在我们构建的集群根本没有引入主节点和从节点的概念，也没有领导者和跟随者的身份，这肯定意味着我们目前构建的集群缺少很重要的功能和特性。那这是不是意味着接下来我们要继续丰富自己构建的集群的内容，把所谓的主从节点、或者是领导者跟随者的身份添加到集群中呢？

## 引入 Distro 集群概念

我的观点很直接：**那就是没必要给我们自己构建的集群定义什么主节点从节点、或者是领导者和跟随者，因为从一开始我就跟大家说了，我们自己构建的是一个最基础的集群，这个集群只需要实现数据备份和功能可用这两个特性即可**。我知道，听到我不准备丰富集群的内容，大家对此肯定会有很多疑问，我也能想到大家心里的疑问有哪些，所以我总结了几个问题列在下面，然后让我们一起来讨论一下这几个问题该怎么解决。

**1 首先大家肯定很困惑，当客户端访问集群某个节点时，要访问哪一个节点呢？因为节点没有主从之分了，每个节点对外提供的服务一样吗？**

**2 我们的集群在备份数据的时候该怎么传输数据？也就是说，数据从集群的哪个节点传输到哪个节点呢？**

**3 集群对外提供服务的节点不可用后，那该怎么切换到另一个节点，让另一个节点继续提供服务呢？**

以上三个问题肯定是大家目前最困惑的问题。接下来就让我来为大家一一分析一下。

首先来分析第一个问题，**既然我们已经决定不为我们自己构建的集群引入主从模式、或者是领导者跟随者模式，那这就意味着集群中每一个节点的地位都是平等的，或者说每一个节点的身份都是相同的。既然是这样，那就表明了集群中每一个节点对外提供的服务都是相同的，每一个节点既可以提供写入数据的服务，也可以提供查询数据的服务，这个逻辑大家应该可以理解吧**？这样一来，**nacos 的客户端就可以随便访问集群中的任何一个节点了，也许客户端 A 访问集群中的节点 1，客户端 B 访问的就是集群中的节点 2，反正每一个节点都能提供相同的服务**。

如果大家理解了第一个问题的解决方案，那第二个问题就很简单了。**既然集群中每一个节点的地位都是平等了，每一个节点都可以被客户端访问，这就意味着每一个节点都可以被不同的 nacos 客户端写入数据。既然是这样的话，每一个节点被写入了数据之后，就要把数据传输给集群中的其它节点**。比如说集群中有三个节点，分别是节点 A、B、C，有三个 nacos 客户端，分别是客户端 1、2、3。客户端 1 访问的是节点 A，客户端 2 访问的节点 B，客户端 3 访问的是节点 C；程序运行了一会之后，这三个节点分别向自己访问的集群节点中写入了不同的数据，这个时候，节点 A 就要把数据同步给节点 B 和节点 C，节点 B 就要把接收到的数据同步给节点 A 和节点 C，节点 C 就要把接收到的数据同步给节点 A 和节点 B。这就是我们构建的集群备份数据的方式，这个逻辑大家可以再梳理梳理。

第二个问题分析完毕之后，再分析第三个问题就更简单了，**既然集群中每一个节点的地位都是一样的，并且每一个节点备份的数据也都是一样的，那这就好办了，根本不用切换什么从节点，或者是重新选举领导者，当某个节点不能对客户端提供服务的时候，只需要给客户端提供一个可以访问的新的节点的地址即可，让客户端访问新的节点，然后执行写入数据或者读取数据的操作**。这也就是我们构建的集群的功能可用的特性的具体表现，并且我还可以告诉大家，实际上我们构建的集群的功能可用的特性已经实现完毕了，在我们构建 nacos 客户端的时候就已经实现了。也许大家对此没有印象了，但接下来只要我把具体的代码展示一下，大家就全都清楚了。

我给大家展示的是 nacos 注册中心客户端的部分代码，首先是注册中心客户端要用到的 ServerListManager 服务地址管理器，请看下面代码块。

```
package com.cqfy.nacos.client.naming.core;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：nacos服务地址管理器，这个类的逻辑非常简单，大家自己看看就行
 */
public class ServerListManager implements ServerListFactory, Closeable {

    //在目前这个类中，有些成员变量根本没有用到，大家直接忽略即可，后面版本就会用到
    private final long refreshServerListInternal = TimeUnit.SECONDS.toMillis(30);

    private final String namespace;

    private final AtomicInteger currentIndex = new AtomicInteger();

    private final List<String> serverList = new ArrayList<>();

    private volatile List<String> serversFromEndpoint = new ArrayList<>();

    private ScheduledExecutorService refreshServerListExecutor;

    private String endpoint;

    private String contentPath = ParamUtil.getDefaultContextPath();

    private String serverListName = ParamUtil.getDefaultNodesPath();

    private String nacosDomain;

    private long lastServerListRefreshTime = 0L;

    public ServerListManager(Properties properties) {
        this(NacosClientProperties.PROTOTYPE.derive(properties), null);
    }

    //在这个构造方法中，使用NacosClientProperties对象初始化ServerListManager对象
    public ServerListManager(NacosClientProperties properties, String namespace) {
        this.namespace = namespace;
        initServerAddr(properties);
        if (getServerList().isEmpty() && StringUtils.isEmpty(endpoint)) {
            throw new NacosLoadException("serverList is empty,please check configuration");
        } else {
            currentIndex.set(new Random().nextInt(getServerList().size()));
        }
    }

    //在这个方法中，就是把配置文件中定义的nacos服务端的地址列表存放到ServerListManager对象的serverList成员变量中
    private void initServerAddr(NacosClientProperties properties) {
        this.endpoint = InitUtils.initEndpoint(properties);
        if (StringUtils.isNotEmpty(endpoint)) {
            //省略这部分的逻辑，用不上
        } else {//在这里把定义在配置信息中的集群的地址存放到服务地址管理器的serverList成员变量中
            String serverListFromProps = properties.getProperty(PropertyKeyConst.SERVER_ADDR);
            if (StringUtils.isNotEmpty(serverListFromProps)) {
                this.serverList.addAll(Arrays.asList(serverListFromProps.split(",")));
                if (this.serverList.size() == 1) {
                    this.nacosDomain = serverListFromProps;
                }
            }
        }
    }


    public boolean isDomain() {
        return StringUtils.isNotBlank(nacosDomain);
    }

    public String getNacosDomain() {
        return nacosDomain;
    }

    @Override
    public List<String> getServerList() {
        return serverList.isEmpty() ? serversFromEndpoint : serverList;
    }

    //得到下一个nacos服务器地址的方法，nacos注册中心客户端当前访问的集群节点不能正常提供服务之后
    //就会切换下一个节点，当前客户端就会和新的集群节点建立连接
    @Override
    public String genNextServer() {
        int index = currentIndex.incrementAndGet() % getServerList().size();
        return getServerList().get(index);
    }

    @Override
    public String getCurrentServer() {
        return getServerList().get(currentIndex.get() % getServerList().size());
    }

    @Override
    public void shutdown() throws NacosException {
        String className = this.getClass().getName();
        NAMING_LOGGER.info("{} do shutdown begin", className);
        if (null != refreshServerListExecutor) {
            ThreadUtils.shutdownThreadPool(refreshServerListExecutor, NAMING_LOGGER);
        }
        NAMING_LOGGER.info("{} do shutdown stop", className);
    }
}
```

然后是注册中心的 RpcClient 客户端，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/11
 * @方法描述：客户端的顶级抽象父类，这个类中定义了一些公共方法，让http客户端和grpc客户端实现类去实现
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {



    //naocs服务器地址管理器
    private ServerListFactory serverListFactory;


    //通过服务地址管理器得到下一个nacos服务器地址的方法
    protected ServerInfo nextRpcServer() {
        String serverAddress = getServerListFactory().genNextServer();
        return resolveServerInfo(serverAddress);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：启动rpc客户端的方法
     */
    public final void start() throws NacosException {

         //注意，这个启动客户端的方法是在NamingGrpcClientProxy对象的构造器中调用的，调用之前还调用了本类的serverListFactory方法，在serverListFactory方法中会将客户端状态更新为初始化完毕状态
        //这就意味着这个时候客户端其实已经初始化完毕了，所以当前的客户端状态应该为初始化完毕状态，这里要做一个状态的判断
        boolean success = rpcClientStatus.compareAndSet(RpcClientStatus.INITIALIZED, RpcClientStatus.STARTING);
        if (!success) {
            //状态错误直接退出当前方法
            return;
        }
        //在这里创建了客户端事件执行器对象，这里定义了两个线程，但在第二版本代码中只会用到一个线程，第三版本就会用到第二个线程了
        clientEventExecutor = new ScheduledThreadPoolExecutor(2, r -> {
            Thread t = new Thread(r);
            t.setName("com.alibaba.nacos.client.remote.worker");
            t.setDaemon(true);
            return t;
        });

        //省略中间的部分逻辑


        //下面就是程序刚启动时，客户端连接服务端的具体操作了
        //定义一个连接对象，用来接收客户端到服务端的连接
        Connection connectToServer = null;
        //更新客户端状态为开始状态
        rpcClientStatus.set(RpcClientStatus.STARTING);
        //获取连接重试次数
        int startUpRetryTimes = rpcClientConfig.retryTimes();
        //在循环中开始连接服务端，循环次数就为连接重试次数
        while (startUpRetryTimes >= 0 && connectToServer == null) {
            try {
                //连接失败一次，则次数减1
                startUpRetryTimes--;
                
                //因为上一次连接失败了，肯定是nacos服务端有问题，所以这里从服务地址管理器中获得一个新的服务端地址
                //ServerInfo是一个内部类，里面封装nacos服务器地址信息
                ServerInfo serverInfo = nextRpcServer();
                
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Try to connect to server on start up, server: {}",
                        rpcClientConfig.name(), serverInfo);
                //连接服务器，连接成功则返回一个连接对象
                connectToServer = connectToServer(serverInfo);
            } catch (Throwable e) {
                LoggerUtils.printIfWarnEnabled(LOGGER,
                        "[{}] Fail to connect to server on start up, error message = {}, start up retry times left: {}",
                        rpcClientConfig.name(), e.getMessage(), startUpRetryTimes, e);
            }
        }



        //省略该方法的其他逻辑
        
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：真正向服务端发送请求的方法
     */
    public Response request(Request request, long timeoutMills) throws NacosException {
        //重试次数初始为0
        int retryTimes = 0;
        //接收响应对象
        Response response;
        //接收异常信息
        Throwable exceptionThrow = null;
        //得到操作的开始时间
        long start = System.currentTimeMillis();
        //在循环中发送请求到服务器，这里之所以是在循环中执行操作，是因为请求发送有可能失败
        //这时候就要重新发送请求，所以就要在循环中判断是否超过了规定的请求重试次数
        //以及是否超过了请求超时时间，这两个都没有超过才能继续重新发送请求
        while (retryTimes <= rpcClientConfig.retryTimes() && (timeoutMills <= 0|| System.currentTimeMillis() < timeoutMills + start)) {

            //循环发送请求给服务端的操作
        }

        //走到这里意味着连接出问题了，这时候就要把客户端状态更新为不健康状态，等待客户端异步连接服务器成功
        if (rpcClientStatus.compareAndSet(RpcClientStatus.RUNNING, RpcClientStatus.UNHEALTHY)) {
            //然后切换服务器，异步去连接服务器，异步连接服务器其实就会从serverListFactory服务地址管理器
            //中得到一个新的集群节点的地址，然后和新的节点建立连接
            switchServerAsyncOnRequestFail();
        }
        if (exceptionThrow != null) {
            throw (exceptionThrow instanceof NacosException) ? (NacosException) exceptionThrow
            : new NacosException(SERVER_ERROR, exceptionThrow);
        } else {
            throw new NacosException(SERVER_ERROR, "Request fail, unknown Error");
        }
    
    }
    
}
```

上面代码块的第 68 行、116 行就体现出了 nacos 注册中心客户端在和服务端集群中的某个节点断开连接后，就会从集群中选择一个新的节点建立连接。这不就是集群节点可用性的完美体现吗？也许大家觉得我实现的这个功能太简单了，但我想说的是很多功能本身实现得就不难，这也应该是大家学习 nacos 这个框架的体会。以上代码块展示的都是二十几章之前的旧知识了，大家如果没印象的话，可以再翻翻前面的文章回顾回顾。

到此为止，我们上面提出的三个问题就全都解决了，并且也实现了我们自己构建的集群的功能可用的特性。如果大家理解了之前我分析的三个问题的解决方法，**并且大家也能接受一个集群中各个节点都是平等的模式，每一个节点都可以被客户端访问，每一个节点都可以被不同的 nacos 客户端写入数据，读取数据。而且每一个节点被写入了数据之后，就要把数据传输给集群中的其它节点**；如果以上这些集群节点的操作大家都可以接受并且理解了，那么现在我终于可以说一句，我已经为大家把 nacos 服务端的 Distro 集群的核心内容讲解完毕了。大家可以回顾一下，现在我们是不是已经知道了 Distro 集群的启动流程，是怎么收集各个节点信息的，又是怎么和集群其他节点构建网络连接的，还知道了集群是怎么执行健康检查操作的，以及健康检查操作是什么。现在我们也知道了 Distro 集群数据备份和功能可用是怎么实现的了，并且 Distro 集群节点功能可用的特性我们已经实现完毕了。当然，Distro 集群数据备份的功能我还没有为大家实现，我只是简单分析了一下该集群备份数据的方式。接下来我们就要具体实现这个功能了，这个功能一旦实现完毕，那么 nacos 服务端自研的 Distro 集群的核心内容也就讲解完毕了。朋友们，兜兜转转，我们还是回到了节点备份数据这个问题上，由此可以看到，**一个集群中最重要的功能也就是节点之间备份数据了**。

  

## 详细剖析 Distro 集群各个节点备份数据的方式

在一小节我给大家举了一个 Distro 集群备份数据的方式，我把这个备份数据的过程再次给大家展示一下，请看下面代码块。

```
比如说集群中有三个节点，分别是节点 A、B、C，有三个 nacos 客户端，分别是客户端 1、2、3。
客户端 1 访问的是节点 A，客户端 2 访问的节点 B，客户端 3 访问的是节点 C；
程序运行了一会之后，这三个节点分别向自己访问的集群节点中写入了不同的数据，这个时候，
节点 A 就要把数据同步给节点 B 和节点 C，节点 B 就要把接收到的数据同步给节点 A 和节点 C，
节点 C 就要把接收到的数据同步给节点 A 和节点 B。这就是我们构建的集群备份数据的方式。
```

上面代码块展示的集群节点备份数据的流程非常简单，我相信每一位朋友都能理解。但是，这仅仅是一个大概流程，如果深入分析，那要考虑的内容就非常多了。就比如说，**我们都知道，nacos 注册中心客户端向服务端注册了服务实例之后，这些服务实例信息是存放到 nacos 服务端管理的每一个客户端对象中的**。我把相关代码给大家搬运过来了，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：服务端这边的客户端实现类，服务端会为接收到的每一个客户端连接都创建一个客户端对象，该客户端注册的服务实例信息和订阅者信息都会存放到自己的客户端对象中
 */
public abstract class AbstractClient implements Client {

     //客户端存放到注册到服务端的服务实例信息的map，key就是服务实例元信息，value就是对应的服务实例信息
    protected final ConcurrentHashMap<Service, InstancePublishInfo> publishers = new ConcurrentHashMap<>(16, 0.75f, 1);



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：注册服务实例到客户端对象的方法
     */
    @Override
    public boolean addServiceInstance(Service service, InstancePublishInfo instancePublishInfo) {
        //判断是否为批量服务实例信息
        if (instancePublishInfo instanceof BatchInstancePublishInfo) {
            //在这里把服务实例信息存放到map中
            InstancePublishInfo old = publishers.put(service, instancePublishInfo);
            //更新服务实例监控信息
            MetricsMonitor.incrementIpCountWithBatchRegister(old, (BatchInstancePublishInfo) instancePublishInfo);
        } else {
            //走到这里意味着不是批量服务实例信息，这时候也是把服务实例信息存放到map中
            if (null == publishers.put(service, instancePublishInfo)) {
                //这行代码是我自己添加的，用来验证第七版本代码运行没有问题
                System.out.println("服务实例注册成功！！！");
                //更新监控信息
                MetricsMonitor.incrementInstanceCount();
            }
        }
        //事件通知中心发布客户端变更事件，这个事件究竟怎么被处理，很快就会在后面的代码版本实现
        NotifyCenter.publishEvent(new ClientEvent.ClientChangedEvent(this));
        //记录操作日志
        Loggers.SRV_LOG.info("Client change for service {}, {}", service, getClientId());
        return true;
    }


    //其他内容省略
}
```

从上面代码块中可以看到，只要某个 nacos 客户端对象向服务实例注册了服务实例信息，这些服务实例信息就会被存放在 nacos 服务端为这个客户端创建的 Client 客户端对象中。这都是旧知识了，想必大家已经非常熟悉了。那现在问题就来了，**一个 nacos 服务端可能会接收多个客户端的数据，也就是说，nacos 服务端集群的某个节点会接收多个客户端的数据，而这些数据都存放在每一个 nacos 客户端在服务端的 Client 客户端对象中，那么这个节点要想把每个 Client 客户端存储的服务实例信息传输备份给其他节点时，该怎么备份呢？是直接传输每一个 Client 客户端保存的服务实例信息呢？还是说也要把 Client 客户端对象复制过去呢**？

大家可别忘了，客户端对象是怎么被服务端创建的，**当 nacos 服务端每接收到一个 nacos 客户端连接时，才会在自己内部为这个 nacos 客户端 Client 对象**，我把对应的代码也搬运到这里了，请看下面代码块。

```
package com.cqfy.nacos.core.remote;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：客户端连接管理器，这个类的对象是服务端非常重要的一个组件，这个类的对象也要交给springboot容器来管理，在第七版本代码中，这个连接管理器只存放来自SDK客户端的连接对象
 */
@Service
public class ConnectionManager {

    private static final Logger LOGGER = Loggers.CONNECTION;

    //这个map是用来存放每一个客户端IP地址建立连接的数量的，这么说可能有些绕，为什么一个客户端IP地址会和服务器建立多个连接呢？
    //请大家把这里的客户端IP地址想象成一个服务器的IP地址，一个服务器是不是可以部署多个注册中心客户端，这样一来一台服务器不就可以和注册中心服务端建立多个连接了吗？
    //这个map存放的就是一个客户端IP地址和注册中心服务端建立连接的数量
    private Map<String, AtomicInteger> connectionForClientIp = new ConcurrentHashMap<>(16);

  

    //客户端连接事件监听器注册表，这个注册表中存放着连接事件监听器，连接事件监听器也是一个非常总要的组件，其中ConnectionBasedClientManager就是非常重要的一个监听器
    //关于ConnectionBasedClientManager类的功能，我会在后面版本代码中为大家讲解和实现
    private ClientConnectionEventListenerRegistry clientConnectionEventListenerRegistry;

    //构造方法
    public ConnectionManager(ClientConnectionEventListenerRegistry clientConnectionEventListenerRegistry) {
        this.clientConnectionEventListenerRegistry = clientConnectionEventListenerRegistry;
    }


    //判断某个客户端操作是否需要被追踪的方法，该方法在源码中默认返回false，所以我这里就直接使用默认值了
    //如果按照源码实现，那就要引入太多组件了，有点麻烦，我就省略了
    public boolean traced(String clientIp) {

        return false;
    }

    //判断某个客户端Id是否有效
    public boolean checkValid(String connectionId) {
        return connections.containsKey(connectionId);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：注册客户端连接的方法
     */
    public synchronized boolean register(String connectionId, Connection connection) {
        //判断客户端与服务端的连接是否真的建立了
        if (connection.isConnected()) {
            //如果真的建立了，就从连接的元数据对象中获得客户端IP信息
            String clientIp = connection.getMetaInfo().clientIp;
            //然后判断connections这个map中是否已经存放了当前的客户端连接对象
            if (connections.containsKey(connectionId)) {
                //如果已经存放了直接退出当前方法即可
                return true;
            }
            //走到这里意味着没有存放客户端连接，这时候就要判断一下服务端接收的客户端连接是否符合限制
            //如果并不符合服务端规定的限制，就返回false，并不注册客户端连接到连接管理器中
            if (checkLimit(connection)) {
                return false;
            }
            //判断是否需要追踪当前的客户端IP，如果需要追踪，就设置可追踪标志到连接对象中
            if (traced(clientIp)) {
                connection.setTraced(true);
            }
            //在这里把客户端连接Id和服务端为其创建的连接对象添加到映射表中
            connections.put(connectionId, connection);
            //接下来判断connectionForClientIp这个map中是否已经存放了对应的客户端IP地址
            if (!connectionForClientIp.containsKey(clientIp)) {
                //如果没有数据就创建一个原子对象放进去
                connectionForClientIp.put(clientIp, new AtomicInteger(0));
            }
            //如果已经有数据了，就意味着当前客户端IP已经和服务器建立了连接了，现在又有新的连接建立了，因此需要给客户端IP地址和服务器建立的连接数量自增1
            connectionForClientIp.get(clientIp).getAndIncrement();
            
            //在这里为当前接收到的连接创建了客户端对象
            //通知客户端连接事件监听器有客户端和服务器建立连接了，应该执行监听器对象中的回调方法了
            clientConnectionEventListenerRegistry.notifyClientConnected(connection);
            //记录操作日志
            LOGGER.info("new connection registered successfully, connectionId = {},connection={} ", connectionId, connection);
            return true;
        }
        return false;

    }


    //该类其他内容省略

}
```

上面代码块的第 84 行就会为 nacos 服务端接收到的客户端连接创建 Client 客户端对象了。好了，旧的知识回顾完毕之后，让我们再把目光收回到眼前，现在的问题是，**如果直接把集群中一个节点创建的 Client 客户端对象复制给集群的其他节点，可其他节点根本就没有接收到对应的客户端连接呀，既然没有接收到对应的客户端连接，又怎么能在服务端保存对应的 nacos 客户端的 Client 客户端对象呢**？这个问题大家应该都能够想到，但是话又说回来，**nacos 服务端的内部结构决定了它必须使用 Client 客户端对象存储服务实例信息**，这似乎又告诉我们，在 Distro 集群备份数据的时候，应该把 Client 客户端对象复制给集群的其他节点。但真的是这样吗？

接下来就让我为大家剖析一下这个问题的答案，首先有一点已经明确了，**根据 nacos 服务端内部存储服务实例信息的方式，假如说 Distro 集群中的一个节点要把服务实例数据传输给集群中的其他节点，那么其他节点肯定也要保存这些服务实例所在的 Client 客户端对象**。这一点应该是我们目前的共识，**否则 nacos 服务端就没办法存储服务实例信息，服务实例信息是不能脱离于 Client 客户端对象而存储的**。那么我们就应该让集群节点在备份数据时直接把 Client 客户端对象传输给其他节点吗？在 nacos 服务端源码中并不是这么做的。实际上在 nacos 服务端源码中，**当 Distro 集群的一个节点要把自己存储的某些服务实例备份给其他节点时，会直接把这些服务实例数据封装到一个对象中，至于是什么对象，目前我们还没必要研究得这么仔细，与此同时，也会把服务实例所在客户端的信息封装起来，然后把这个封装了所有数据信息的对象传输给集群中的其他节点；集群中的其他节点在接收到封装了服务实例信息的对象后，在保存服务实例信息的之前会从封装了服务实例信息的对象中获得这些服务实例信息注册来源的客户端信息，然后判断当前节点内部是否为这个客户端创建了 Client 客户端对象，如果没有创建则先创建对应的 Client 对象，然后再把服务实例信息存放到 Client 客户端对象中**。现在大家应该清楚了在 Distro 集群中是怎么备份服务实例信息的吧？总之绕了一圈之后，**我们发现服务实例信息不管在哪个节点中，都要存放在服务实例注册来源的 nacos 客户端的 Client 对象中，如果 Distro 集群中的某个节点没有创建对应的 Client 客户端对象，那就先创建该对象，然后再存储服务实例信息**。大家可以仔细品味品味这个逻辑，如果这个逻辑理解了，那接下来请大家跟我一起思考另一个问题。

刚才我们分析 Distro 集群节点在备份数据的时候，接收到数据的节点会先从封装数据的对象中得到这些服务实例信息注册来源客户端的信息，然后判断自己内部是否存在对应的 Client 客户端对象，如果不存在则创建 Client 客户端对象，然后再把其他节点备份过来的服务实例信息存储在 Client 对象中。我不知道大家刚才在思考这个流程的时候，有没有想过，这个客户端信息是什么呢？接收数据的节点怎么根据其他节点传输过来的客户端信息创建 Client 客户端对象呢？如果大家对这个问题没有头绪，那就请大家回顾一下，一个正常的 nacos 服务端，怎么为接收到的 nacos 客户端连接创建 Client 对象呢？我把相关代码搬运过来了，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client.manager.impl;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：客户端管理器，这个客户端管理器持有者服务端为每一个客户端连接创建的客户端对象
 */
@Component("connectionBasedClientManager")
public class ConnectionBasedClientManager extends ClientConnectionEventListener implements ClientManager {


    //这个map就是用来存放客户端对象的，key为客户端连接的Id，value就是具体的客户端对象
    private final ConcurrentMap<String, ConnectionBasedClient> clients = new ConcurrentHashMap<>();

    //构造方法
    public ConnectionBasedClientManager() {
        //在构造方法中提交了ExpiredClientCleaner定时任务，定期清理过期客户端对象
        GlobalExecutor.scheduleExpiredClientCleaner(new ExpiredClientCleaner(this), 0, Constants.DEFAULT_HEART_BEAT_INTERVAL, TimeUnit.MILLISECONDS);
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：服务端接收到新的客户端连接时该方法就会被回调，这个方法参数就是服务端为客户端创建的客户端连接对象
     */
    @Override
    public void clientConnected(Connection connect) {
        //判断当前处理的客户端连接对象，其连接来源是否为命名模块，也就是naming模块，注册中心肯定是属于naming模块
        //配置中心就是config模块的
        if (!RemoteConstants.LABEL_MODULE_NAMING.equals(connect.getMetaInfo().getLabel(RemoteConstants.LABEL_MODULE))) {
            return;
        }
        //创建客户端属性对象
        ClientAttributes attributes = new ClientAttributes();
        //把客户端连接类型设置到属性对象中
        attributes.addClientAttribute(ClientConstants.CONNECTION_TYPE, connect.getMetaInfo().getConnectType());
        //把客户端连接的元数据信息设置到属性对象中那个
        attributes.addClientAttribute(ClientConstants.CONNECTION_METADATA, connect.getMetaInfo());
        //得到该客户端连接的连接Id，然后为该客户端创建客户端对象，也就是ConnectionBasedClient对象
        clientConnected(connect.getMetaInfo().getConnectionId(), attributes);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：为指定客户端Id创建对象对应的客户端对象的方法
     */
    @Override
    public boolean clientConnected(String clientId, ClientAttributes attributes) {
        //得到客户端连接类型
        String type = attributes.getClientAttribute(ClientConstants.CONNECTION_TYPE);
        //根据客户端连接类型得到对应的客户端对象
        ClientFactory clientFactory = ClientFactoryHolder.getInstance().findClientFactory(type);
        //创建对应的客户端对象
        return clientConnected(clientFactory.newClient(clientId, attributes));
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：把客户端Id和对应的客户端对象存放到map中的方法
     */
    @Override
    public boolean clientConnected(final Client client) {
        clients.computeIfAbsent(client.getClientId(), s -> {
            Loggers.SRV_LOG.info("Client connection {} connect", client.getClientId());
            return (ConnectionBasedClient) client;
        });
        return true;
    }

    

    //该类的其他内容省略
}
```

从上面代码块的第二个方法可以看到，**nacos 服务端为新接收到的客户端连接创建对应的 Client 客户端对象时，只需要知道客户端连接的唯一 Id 即可**。这一点大家都清楚了之后，那么换成 Distro 集群接收备份数据的节点也是一样，只要让这个节点知道接收到的服务实例注册来源客户端的连接 Id，就可以为这个客户端创建对应的 Client 对象。所以，**所谓的客户端信息，其实就是客户端连接唯一 Id，在节点向集群中的其他节点传输数据时，只需要把服务实例注册来源的 nacos 客户端的连接 Id 一起备份给集群中的其他节点，其它节点就可以使用客户端连接 Id 为这些服务实例信息创建 Client 对象了，然后再把服务实例信息存放在 Client 对象中即可**。

以上逻辑梳理清楚之后，这时候大家可能就会有另一个问题，在 Distro 集群中的各个节点中，可能有些节点内部的 Client 客户端对象并不是自身的，而是从其他节点备份数据时创建的，这就意味着这个节点根本就没有和对应的 nacos 客户端建立连接，但是内部却创建了与这个客户端对应的 Client 客户端对象。而且，这个节点内部肯定也会和一些 nacos 客户端建立连接，肯定就会为建立连接的这些客户端创建对应的 Client 对象。这就意味着，**这个节点内部会存在多个 Client 客户端对象，但这些 Client 对象有的是真正对应着某个客户端连接，而有的却是从其他节点备份数据时创建的，那该怎么区分这两种 Client 客户端对象呢**？这时候我就要引入一个概念了，那就是直连客户端对象和非直连客户端对象。

**所谓直连客户端就是：假如在节点内部有一个 Client 客户端对象，这个 Client 客户端对象真正对应着某一个客户端连接，我们就把这种客户端对象称为直连客户端对象；而非直连客户端就是：节点内部的某个 Client 对象，是当前节点从其他节点备份数据时创建的，这种客户端对象就是非直连客户端对象**。在下一章，我们就会在程序里使用代码具体区分这两种客户端对象，这个时候大家肯定会好奇，为什么要区分这两种对象呢？这就是下一章的内容了，**我可以提前告诉大家，区分直连客户端和非直连客户端对节点备份数据非常重要，只有这个判断清楚了，才知道节点要不要主动向其他节点备份某个 Client 客户端对象内部存储的数据**，这些就留到下一章为大家实现吧。

  

好了，到此为止，关于 Distro 集群备份数据的过程就剩下最后一个问题了，**那就是当集群中的一个节点要把自己内部的数据备份给其他节点时，究竟应该把什么数据备份过去**？我们刚才所有的分析都是围绕着服务实例信息来展开的，**所以注册到节点内部的服务实例信息肯定就是要备份给其他节点的数据**，这一点是毋庸置疑的吧？在 nacos 注册中心模块中，服务实例信息就是最重要的数据了，肯定要对其进行备份。但是，注册中心服务端并不会只存放服务实例信息呀，如果有哪个 nacos 客户端订阅了服务端的某个 Service 服务，那么订阅者的信息也会被存放到服务端内部，就存放到与该 nacos 客户端对应的 Client 客户端对象中，具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：服务端这边的客户端实现类，服务端会为接收到的每一个客户端连接都创建一个客户端对象，该客户端注册的服务实例信息和订阅者信息都会存放到自己的客户端对象中
 */
public abstract class AbstractClient implements Client {

     //客户端存放到注册到服务端的服务实例信息的map，key就是服务实例元信息，value就是对应的服务实例信息
    protected final ConcurrentHashMap<Service, InstancePublishInfo> publishers = new ConcurrentHashMap<>(16, 0.75f, 1);


    //客户端订阅了服务器的服务实例信息时，服务端会把订阅者信息存放到客户端自己的map中
    protected final ConcurrentHashMap<Service, Subscriber> subscribers = new ConcurrentHashMap<>(16, 0.75f, 1);


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：添加订阅者到客户端对象的方法
     */
    @Override
    public boolean addServiceSubscriber(Service service, Subscriber subscriber) {
        if (null == subscribers.put(service, subscriber)) {
            MetricsMonitor.incrementSubscribeCount();
        }
        return true;
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：从客户端对象取消某个订阅者的方法
     */
    @Override
    public boolean removeServiceSubscriber(Service service) {
        if (null != subscribers.remove(service)) {
            MetricsMonitor.decrementSubscribeCount();
        }
        return true;
    }


    //该类的其他内容省略
}
```

在阅读了上面代码块之后，请大家思考一下，这些订阅者信息也要备份给集群中的其他节点吗？这个问题就不用大家思考了，我直接宣布答案即可：**订阅者信息不用同步给集群中的其他节点**，至于为什么，下一章我再给大家解释。现在来看，**Distro 集群中需要备份的数据只有服务实例信息了**，这一点大家要记住。

好了，到此为止，本章的内容就全部结束了，可以看到本章几乎没展示什么代码，也没实现什么功能，都是在对集群的设计原理进行分析。本章结束之后，大家只要掌握两个要点即可：**第一就是理清楚直连客户端和非直连客户端的概念；第二就是理解集群节点在备份数据的时候，会把客户端信息一起备份给其他节点，让节点根据客户端信息创建 Client 客户端对象，这样才能存放从其他节点备份过来的服务实例信息**。

当然，通篇都是文字分析难免有些枯燥，下一章我就用代码为大家实现具体的功能了，到时候要分析，要考虑的细节就会更多了，比如 Distro 集群的某个节点什么时候触发向其他节点备份数据的操作，如果有一个 nacos 客户端和这个节点建立了客户端连接，也注册了很多服务实例信息，但程序运行了一会，它们之间的连接忽然断开了，那么已经备份给其他节点的数据该怎么处理呢？总之有很多功能等待我们去实现，当然，大家也不必心急，现在大家还看不了我提供的第十一版本代码，等下一章结束之后，应该就可以去看了。大家可以耐心等一等，好了朋友们，我们下一章见！