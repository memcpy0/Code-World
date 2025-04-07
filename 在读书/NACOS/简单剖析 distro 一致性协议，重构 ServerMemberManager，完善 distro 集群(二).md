上一章我们已经为自己构建的集群引入了多个组件，比如可以把服务器抽象成集群节点的 Member 类、专门为 Member 对象服务的 MemberUtil 工具类、以及管理集群中所有节点的 ServerMemberManager 节点管理器、还有用来加载并解析 cluster.conf 配置文件的 FileConfigMemberLookup 文件地址寻址器。这些组件都引入完毕了，并且我也用自己的方式为大家剖析了这些组件协同工作的原理：**当节点启动的时候，文件寻址器就会把 cluster.conf 配置文件的集群节点信息读取到内存中，然后把加载得到的数据交给 MemberUtil 工具类解析，得到存放集群所有节点的 Member 对象集合，再调用文件寻址器抽象父类 AbstractMemberLookup 的 afterLookup() 的方法把集群的所有节点交给 ServerMemberManager 集群成员管理器来管理**。这就是我们期望的这些组件一起工作的流程。但是，上一章我们还遗留了两个问题，这两个问题不解决，程序就无法像我们期望的那样执行，而这两个问题就是：

**1 寻找合适的时机创建 FileConfigMemberLookup 文件地址寻址器。**

**2 调用 FileConfigMemberLookup 寻址器的 doStart() 方法的时机。**

所以接下来，就让我们一起来解决这两个问题。

## 引入 LookupFactory 寻址器工厂

  

虽说我刚才为大家展示了两个问题，但实际上这两个问题的联系非常紧密，第一个问题一旦解决了，第二个问题也就跟着解决了。原因很简单，只要我们找到了创建 FileConfigMemberLookup 文件寻址器的时机，创建了该寻址器对象，紧接着调用该对象的 doStart()，启动文件寻址器不就行了吗？所以，我们目前就把注意力全放在第一个问题上即可，分析分析文件寻址器的创建时机。

不过在分析文件寻址器创建时机之前，我忽然意识到其实还有一个问题值得讨论，那就是文件寻址器对象应该怎么创建呢？是直接调用 FileConfigMemberLookup 文件寻址器的无参构造方法创建吗？这时候我就要为大家再解释一下了，在上一章我已经告诉大家，FileConfigMemberLookup 文件寻址器还继承了 AbstractMemberLookup 抽象父类，并且在 nacos 源码中，并不仅仅只有一个寻址器查找集群中所有节点的信息，还存在另外一个服务器地址寻址器，也就是 AddressServerMemberLookup 组件。我在为大家提供的第十版本代码中之所以把 AddressServerMemberLookup 服务器地址寻址器省略了，是因为它用起来比 FileConfigMemberLookup 文件地址寻址器要麻烦一些。但是，既然存在多个寻址器，就意味着这几个寻址器都是可以被使用的，在程序运行的时候具体使用哪一个寻址器，就交给用户来决定了。也就是说，**用户可以自己定义配置信息，决定在程序运行的时候使用哪一个寻址器**。那在这种情况下，**就不能直接调用寻址器的无参构造方法来创建对象了，而是应该引入一个寻址器工厂，这个工厂可以根据用户定义的配置信息灵活创建对应的寻址器对象**。这个工厂对象我也已经定义完毕了，就定义为了 LookupFactory，具体实现请看下面代码块。

```
package com.cqfy.nacos.core.cluster.lookup;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/8/16
 * @方法描述：寻址器工厂类，这个寻址器工厂会根据用户的配置创建集群成员寻址器，寻址器的作用就是查找集群中有哪些节点
 * 然后把这些节点封装到一个个Member对象中，然后把这些Member对象交给ServerMemberManager集群成员管理器管理
 */
public final class LookupFactory {

    //寻址器的key，用这个key就可以从环境变量中得到用户配置的寻址器类型，然后就可以根据寻址器类型创建具体的寻址器了
    private static final String LOOKUP_MODE_TYPE = "nacos.core.member.lookup.type";

    //寻址器对象，别忘了寻址器的AbstractMemberLookup抽象父类还实现了MemberLookup接口
    @SuppressWarnings("checkstyle:StaticVariableName")
    private static MemberLookup LOOK_UP = null;

    //寻址器类型
    private static LookupType currentLookupType = null;


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：创建寻址器的方法
     */
    public static MemberLookup createLookUp(ServerMemberManager memberManager) throws NacosException {
        //判断nacos服务端是否以单机模式启动，如果不是以单机模式启动
        //就根据用户配置的寻址器类型创建对应的寻址器
        if (!EnvUtil.getStandaloneMode()) {
            //走到这里意味着不是单机模式启动nacos服务端，那就根据key得到用户配置的寻址器类型
            String lookupType = EnvUtil.getProperty(LOOKUP_MODE_TYPE);
            //根据类型选择正确的寻址器类型
            LookupType type = chooseLookup(lookupType);
            //在这里创建真正的寻址器对象
            LOOK_UP = find(type);
            //给寻址器类型赋值
            currentLookupType = type;
        } else {
            //走到这里意味着nacos服务器是以单机模式启动的，那就直接创建一个单机模式寻址器
            LOOK_UP = new StandaloneMemberLookup();
        }
        //把集群成员管理器交给寻址器使用，这个injectMemberManager()是寻址器的
        //AbstractMemberLookup抽象父类中的方法，这样一来，寻址器就得到了集群成员管理器了
        LOOK_UP.injectMemberManager(memberManager);
        //记录日志信息
        Loggers.CLUSTER.info("Current addressing mode selection : {}", LOOK_UP.getClass().getSimpleName());
        //返回创建的寻址器对象
        return LOOK_UP;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：根据用户配置的寻址器信息得到对应的寻址器类型枚举对象
     */
    private static LookupType chooseLookup(String lookupType) {
        //判断用户配置的寻址器信息非空
        if (StringUtils.isNotBlank(lookupType)) {
            //和寻址器枚举对象匹配，得到对应的寻址器类型枚举对象
            LookupType type = LookupType.sourceOf(lookupType);
            //得到了寻址器类型就把该寻址器类型返回
            if (Objects.nonNull(type)) {
                return type;
            }
        }
        //走到这里意味着没有得到寻址器类型，这时候就判断一下存储了集群配置信息的cluster.conf文件是否在本地存在
        File file = new File(EnvUtil.getClusterConfFilePath());
        //如果文件存在，或者用户在jvm参数配置了集群节点的网络地址，就像jraft配置的那样
        if (file.exists() || StringUtils.isNotBlank(EnvUtil.getMemberList())) {
            //那就返回文件寻址器类型
            return LookupType.FILE_CONFIG;
        }
        //否则使用默认的地址寻址器
        return LookupType.ADDRESS_SERVER;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：根据寻址器类型得到对应的寻址器
     */
    private static MemberLookup find(LookupType type) {
        //判断寻址器类型是不是文件寻址类型
        if (LookupType.FILE_CONFIG.equals(type)) {
            //如果是文件寻址类型，就创建一个文件寻址器
            LOOK_UP = new FileConfigMemberLookup();
            return LOOK_UP;
        }
        //判断寻址类型是不是地址寻址器
        if (LookupType.ADDRESS_SERVER.equals(type)) {
            //如果是地址寻址器，就创建一个地址寻址器对象
            //在我提供的第十版本代码中，我把地址服务器给省略了，也就是AddressServerMemberLookup类
            //要是使用地址服务器来查找集群中所有成员的信息，这就比较麻烦了，这就要求你还要单独启动一个服务器，把集群成员信息存放到那个服务器上，然后让当前节点去访问新启动的服务器，这样才能查询到集群中所有结点的信息
            //和文件寻址比起来显然太麻烦了，所以我就省略了，这个AddressServerMemberLookup类的内容也很简单，其实就是使用NacosRestTemplate，也就是http访问新启动的服务器，大家感兴趣的话可以自己看看源码
            //LOOK_UP = new AddressServerMemberLookup();
            return LOOK_UP;
        }
        //走到这里说明传入的寻址器类型错误，抛出异常即可
        throw new IllegalArgumentException();
    }


    //得到当前使用的寻址器对象
    public static MemberLookup getLookUp() {
        return LOOK_UP;
    }


    //销毁当前使用的寻址器
    public static void destroy() throws NacosException {
        Objects.requireNonNull(LOOK_UP).destroy();
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/8/16
     * @方法描述：这个枚举类定义了寻址类型的枚举对象
     */
    public enum LookupType {

        //表示文件寻址模式
        FILE_CONFIG(1, "file"),

        //表示地址寻址模式
        ADDRESS_SERVER(2, "address-server");


        private final int code;

        private final String name;


        LookupType(int code, String name) {
            this.code = code;
            this.name = name;
        }

        /**
         * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
         * @author：陈清风扬，个人微信号：chenqingfengyangjj。
         * @date:2024/8/16
         * @方法描述：根据用户配置的寻址类型信息得到对应的寻址类型枚举对象
         */
        public static LookupType sourceOf(String name) {

            for (LookupType type : values()) {

                if (Objects.equals(type.name, name)) {
                    return type;
                }
            }
            return null;
        }


        public int getCode() {
            return code;
        }


        public String getName() {
            return name;
        }


        @Override
        public String toString() {
            return name;
        }
    }
    
}
```

根据上面代码块我们可以发现，**只要调用了 LookupFactory 寻址器工厂的 createLookUp() 方法，寻址器工厂就会根据用户自己定义的配置信息，创建对应的寻址器对象**。更加具体的细节和流程我就不再重复讲解了，因为代码块中的注释非常详细。总之，我们已经知道寻址器对象是如何被创建的了，现在我们可以回到主线上，**开始分析 LookupFactory 寻址器工厂的 createLookUp() 方法调用的时机**，因为该方法一旦被调用，寻址器对象就会被创建，我们也就找到了寻址器对象被创建的时机了。那么，LookupFactory 寻址器工厂的 createLookUp() 方法应该在什么时候被调用呢？

## 重构 ServerMemberManager，初步启动集群

我认为 LookupFactory 寻址器工厂的 createLookUp() 方法的调用时机非常明确，肯定是在我们构建的集群启动的时候就调用该方法。原因很简单：**根据我们对市面上大多数集群的了解就能意识到，一个常规的、流程合理的集群，肯定会在启动的时候就把集群中所有节点的信息收集起来，然后为集群各个节点建立网络连接，打开通信通道，之后集群就可以正常工作了**。**这也就意味着 LookupFactory 寻址器工厂的 createLookUp() 方法在集群刚刚启动的时候就要被调用**，那所谓的集群刚刚启动的时机该怎么判断呢？

这就很好说了，我们的构建的程序深度依赖了 springboot 框架，所以集群肯定也是伴随着 springboot 启动而启动的，也就是说在 springboot 启动的过程中，我们构建的集群也会随着启动。**而集群启动完毕的标志肯定就是当前节点已经得到了集群中所有节点的信息，也就是说 ServerMemberManager 集群成员管理器已经保存了集群中所有节点的信息，并且集群中各个节点的的连接通道也建立完毕，集群的各个节点已经可以互相通信了**。很好，既然集群启动完毕的时候要把集群节点的信息全部交给 ServerMemberManager 管理器来管理，并且 FileConfigMemberLookup 文件寻址器在解析完毕配置文件的信息后，还要把集群中所有节点的信息交给 ServerMemberManager 集群成员管理器来管理，这就意味着在 FileConfigMemberLookup 文件寻址器开始解析配置文件信息之前，ServerMemberManager 集群成员管理器已经创建完毕了。也就是说，**要等到 ServerMemberManager 集群成员管理器创建完毕之后，才能调用 LookupFactory 寻址器工厂的 createLookUp() 方法创建 FileConfigMemberLookup 文件寻址器，然后紧接着调用文件寻址器的 doStart() 方法，让文件寻址器开始解析配置文件中的集群节点的信息**。

这个流程的操作就很容易实现了，**ServerMemberManager 是被 springboot 容器管理的对象，它会被 springboot 创建，我们只要给这个管理器定义一个 init() 初始化方法，在这个 ServerMemberManager 管理器对象创建完毕之后，调用它的 init() 初始化方法，在初始化方法中创建 FileConfigMemberLookup 文件寻址器，然后启动文件寻址器不就行了吗**？当然，在 init() 方法中还仅仅可以定义和文件寻址器相关的操作，还可以定义一些和当前节点初始化相关的操作，就比如说把当前服务器抽象成 Member 节点的操作就可以定义在 init() 方法中。所以接下来我们就要对 ServerMemberManager 集群成员管理器进行一番重构，而重构之后的 ServerMemberManager 集群成员管理器就展示在下面代码块中，请看下面代码块。

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

    //集群成员信息寻址器
    private MemberLookup lookup;

    //当前集群是否使用服务地址寻址器
    private static boolean isUseAddressServer = false;



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:构造方法
     */
    public ServerMemberManager(ServletContext servletContext) throws Exception {
        this.serverList = new ConcurrentSkipListMap<>();
        //在这里调用了init()方法
        init();
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:初始化集群成员管理器的方法，该方法中的操作都很简单，我就不添加那么详细注释了
     */
    protected void init() throws NacosException {
        Loggers.CORE.info("Nacos-related cluster resource initialization");
        //得到端口号，默认是8848
        this.port = EnvUtil.getProperty(SERVER_PORT_PROPERTY, Integer.class, DEFAULT_SERVER_PORT);
        this.localAddress = InetUtils.getSelfIP() + ":" + port;
        //解析当前节点信息
        this.self = MemberUtil.singleParse(this.localAddress);
        //设置当前节点的版本信息
        this.self.setExtendVal(MemberMetaDataConstants.VERSION, VersionUtils.version);
        //开启grpc通信模式
        this.self.setGrpcReportEnabled(true);
        //把当前节点信息添加到serverList成员变量中
        serverList.put(self.getAddress(), self);

        
        //初始化并且启动集群节点信息寻址器
        initAndStartLookup();
        
        //判断集群服务列表是否为空，因为在启动集群节点信息寻址器的时候
        //寻址器就已经解析了集群的配置信息，把集群成员的节点信息都设置到serverList中了
        //如果现在判断到集群服务列表为空，那就意味着集群出现问题了，直接抛出异常即可
        if (serverList.isEmpty()) {
            throw new NacosException(NacosException.SERVER_ERROR, "cannot get serverlist, so exit.");
        }
        //记录操作日志
        Loggers.CORE.info("The cluster resource is initialized");
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:初始化并启动集群成员信息寻址器的方法
     */
    private void initAndStartLookup() throws NacosException {
        //创建寻址器对象
        this.lookup = LookupFactory.createLookUp(this);
        //判断寻址器对象是否是服务地址寻址器，给isUseAddressServer成员变量赋值
        isUseAddressServer = this.lookup.useAddressServer();
        //启动寻址器
        this.lookup.start();
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/18
     * @Description:得到集群中除了自己之外的所有节点的方法
     */
    public List<Member> allMembersWithoutSelf() {
        List<Member> members = new ArrayList<>(serverList.values());
        members.remove(self);
        return members;
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



    //其他内容暂时不做实现
    
}
```

到此为止，ServerMemberManager 集群成员管理器就重构完毕了。还记得上一章我为大家总结的一个集群在正常启动的过程中要执行的四个操作吗？我把它们搬运过来了：

**1 定义本地配置文件，也就是 cluster.conf 文件，然后把集群所有节点的配置信息都定义在这个配置文件中。**

**2 在节点启动的时候，把 cluster.conf 配置文件的内容加载到内存中，并且保存配置文件中的节点信息。**

**3 根据配置文件中的节点信息，为当前节点和集群中的其他节点构建网络连接，使当前节点可以和集群中的其它节点进行网络通信。**

**4 定期判断集群中是否存在不健康的节点，如果存在就把该节点从集群中移除。**

现在前两个操作我们已经实现完毕了，这也就意味着集群启动的流程我们已经实现了一半了，接下来就该实现第三个操作，也就是为集群中的各个节点构建网络连接，让集群中的各个节点可以通信。

## 引入 ClusterRpcClientProxy，实现集群节点的通信功能

为集群各个节点构建网络连接也没有什么难度，无非就是定义好服务端和客户端，然后让客户端和服务端建立连接即可，接下来客户端就可以随时和服务端通信了。现在的问题是，每一个节点的客户端和服务端应该如何构建呢？让我们来思考一下，假如我们构建的集群中有 5 个节点，这 5 个节点想要彼此都可以通信，**那么是不是每一个节点就要既定义成服务端，也要定义成客户端？每一个节点都要和集群中的其他四个节点建立连接**，换句话说，**当前节点除了要启动集群服务端，还要创建四个客户端对象，这个四个客户端对象各自对应集群中的一个节点，要访问对应节点启动的集群服务端，和服务端建立连接**。这个逻辑大家应该可以理解吧？如果这个逻辑理解了，那接下来要定义的操作就简单多了。

首先给节点定义服务端吧，如果大家还记得我们之前实现 nacos 注册中心服务端的内容，肯定对 BaseRpcServer 和 BaseGrpcServer 这两个类还有印象，其中 BaseGrpcServer 是 BaseRpcServer 的子类。而在 BaseGrpcServer 下面就是 GrpcSdkServer 子类，这个 GrpcSdkServer 类的对象就是注册中心服务端。如果大家忘记这一块的知识了，可以再回顾回顾前面的章节。现在我要跟大家讲解的是，**在 nacos 服务端源码中，BaseGrpcServer 类其实有两个子类，一个是 GrpcSdkServer，也就是注册中心服务端，另一个就是 GrpcClusterServer 类，也就是集群服务端。现在我们要为集群的节点定义集群服务端了，所以就要把 GrpcClusterServer 类引入进来。这个类的内容和之前展示过的 GrpcSdkServer 类的内容没什么区别，无非就是集群服务端的端口号和注册中心服务端端口号不一样**。我把 GrpcClusterServer 类的代码展示在下面了，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/8/16
 * @方法描述：集群服务端，这个服务端和sdk服务端的方法一样，作用也一样，大家自己看看就行
 * 我就不添加详细注释了
 */
@Service
public class GrpcClusterServer extends BaseGrpcServer {


    //得到集群服务端端口号的偏移量
    @Override
    public int rpcPortOffset() {
        return Constants.CLUSTER_GRPC_PORT_DEFAULT_OFFSET;
    }

    @Override
    public ThreadPoolExecutor getRpcExecutor() {
        if (!GlobalExecutor.clusterRpcExecutor.allowsCoreThreadTimeOut()) {
            GlobalExecutor.clusterRpcExecutor.allowCoreThreadTimeOut(true);
        }
        return GlobalExecutor.clusterRpcExecutor;
    }

    @Override
    protected long getKeepAliveTime() {
        Long property = EnvUtil
                .getProperty(GrpcServerConstants.GrpcConfig.CLUSTER_KEEP_ALIVE_TIME_PROPERTY, Long.class);
        if (property != null) {
            return property;
        }
        return super.getKeepAliveTime();
    }

    @Override
    protected long getKeepAliveTimeout() {
        Long property = EnvUtil
                .getProperty(GrpcServerConstants.GrpcConfig.CLUSTER_KEEP_ALIVE_TIMEOUT_PROPERTY, Long.class);
        if (property != null) {
            return property;
        }
        return super.getKeepAliveTimeout();
    }

    @Override
    protected long getPermitKeepAliveTime() {
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.CLUSTER_PERMIT_KEEP_ALIVE_TIME, Long.class);
        if (property != null) {
            return property;
        }
        return super.getPermitKeepAliveTime();
    }

    @Override
    protected int getMaxInboundMessageSize() {
        Integer property = EnvUtil
                .getProperty(GrpcServerConstants.GrpcConfig.CLUSTER_MAX_INBOUND_MSG_SIZE_PROPERTY, Integer.class);
        if (property != null) {
            return property;
        }

        int size = super.getMaxInboundMessageSize();
        if (Loggers.REMOTE.isWarnEnabled()) {
            Loggers.REMOTE.warn("Recommended use '{}' property instead '{}', now property value is {}",
                    GrpcServerConstants.GrpcConfig.CLUSTER_MAX_INBOUND_MSG_SIZE_PROPERTY,
                    GrpcServerConstants.GrpcConfig.MAX_INBOUND_MSG_SIZE_PROPERTY, size);
        }
        return size;
    }

    @Override
    protected List<ServerInterceptor> getSeverInterceptors() {
        List<ServerInterceptor> result = new LinkedList<>();
        result.addAll(super.getSeverInterceptors());
        result.addAll(NacosGrpcServerInterceptorServiceLoader
                .loadServerInterceptors(NacosGrpcServerInterceptor.CLUSTER_INTERCEPTOR));
        return result;
    }

    @Override
    protected List<ServerTransportFilter> getServerTransportFilters() {
        List<ServerTransportFilter> result = new LinkedList<>();
        result.addAll(super.getServerTransportFilters());
        result.addAll(NacosGrpcServerTransportFilterServiceLoader
                .loadServerTransportFilters(NacosGrpcServerTransportFilter.CLUSTER_FILTER));
        return result;
    }
}
```

好了，现在 GrpcClusterServer 集群服务端也定义完毕了，在 springboot 启动的过程中，集群节点的集群服务端也就会跟着启动，具体逻辑大家可以去 BaseGrpcServer 这个类中查看，因为在之前我就已经为大家详细剖析并实现这个类的内容了，所以我就不在这里重复讲解了。总之，现在集群节点的集群服务端已经定义完毕了，接下来就轮到定义集群节点的客户端了。

定义集群节点的客户端就更简单了，刚才我们已经分析了，**集群中除了当前节点还存在几个节点，那么当前节点就要为这几个节点都创建一个客户端对象，和对应的节点建立网络连接**。而创建客户端对象的操作我们早就已经实现过了，**无非就是为集群中其它的每一个节点创建一个对应的 RpcClient 对象，然后启动该客户端即可，启动之后，RpcClient 客户端就会和指定的目标节点建立连接了**。这些功能大家应该都还有印象，都是旧知识，所以定义集群节点客户端的操作我也已经实现了，并且我把这些操作定义在了一个新引入的组件，也就是 ClusterRpcClientProxy 类中，翻译过来就是集群客户端代理器的意思，当然，**既然是客户端代理器，肯定就要定义发送请求给其它节点的方法**，这些方法我也都实现了。该类的具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.core.cluster.remote;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/17
 * @Description:集群成员要使用的客户端代理对象，这里大家要理清楚逻辑，假如当前点是集群中的节点1，集群中还有节点2、3、4、5这几个成员
 * 那么节点1在启动的时候，就会根据本地集群配置文件读取集群的配置信息，然后把集群的每一个节点信息都封装到一个Member对象中
 * 并且节点1会为集群中的其他4个节点都创建一个集群客户端对象，这样一来，节点1想要和集群中的其他4个节点通信时，就可以使用各个节点对应的集群客户端对象和它们进行通信了
 */
@Service
public class ClusterRpcClientProxy{


    //默认请求超时时间
    private static final long DEFAULT_REQUEST_TIME_OUT = 3000L;

    //集群成员管理器
    @Autowired
    ServerMemberManager serverMemberManager;




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:该方法会在当前类的对象创建完毕后被调用
     */
    @PostConstruct
    public void init() {
        try {
            //从集群成员管理器中得到集群中除自身之外的所有节点
            List<Member> members = serverMemberManager.allMembersWithoutSelf();
            
            //刷新集群各个成员的集群客户端信息，其实集群中各个节点的集群客户端对象就是在refresh()方法中创建的
            refresh(members);
            //记录日志信息
            Loggers.CLUSTER.info("[ClusterRpcClientProxy] success to refresh cluster rpc client on start up,members ={} ", members);
        } catch (NacosException e) {
            //出现异常则记录告警日志
            Loggers.CLUSTER.warn("[ClusterRpcClientProxy] fail to refresh cluster rpc client,{} ", e.getMessage());
        }

    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:刷新集群各个成员集群客户端的方法，集群各个成员的集群客户端就是在该方法中创建的
     */
    private void refresh(List<Member> members) throws NacosException {

        //遍历集群中的成员
        for (Member member : members) {
            //在这里为每一个成员创建集群客户端，并且启动集群客户端，在启动客户端的过程中，各个成员的客户端就会根据各自的网络地址
            //去和目标服务器建立连接
            createRpcClientAndStart(member, ConnectionType.GRPC);
        }
        
        //从客户端工厂中得到集群各个成员集群客户端的键值对
        Set<Map.Entry<String, RpcClient>> allClientEntrys = RpcClientFactory.getAllClientEntries();
        //得到键值对迭代器对象
        Iterator<Map.Entry<String, RpcClient>> iterator = allClientEntrys.iterator();
        //得到集群成员键值对的key，也就是各个成员集群客户端的唯一标识，然后以集合的形式返回
        List<String> newMemberKeys = members.stream().map(this::memberClientKey).collect(Collectors.toList());
        
        //遍历各个节点的集群客户端
        while (iterator.hasNext()) {
            Map.Entry<String, RpcClient> next1 = iterator.next();
            //这里判断了以下遍历到的节点的集群客户端的唯一标识是否合规，并且还判断了遍历到的节点集群客户端的唯一标识是否存在于上面的集合中
            //这两点有任何一点不满足，说明当前遍历到的节点的集群客户端有错误，需要从客户端工厂中移除并关闭
            if (next1.getKey().startsWith("Cluster-") && !newMemberKeys.contains(next1.getKey())) {
                Loggers.CLUSTER.info("member leave,destroy client of member - > : {}", next1.getKey());
                //走到这里说明遍历到的节点集群客户端有问题
                RpcClient client = RpcClientFactory.getClient(next1.getKey());
                if (client != null) {
                    //如果集群客户端不为空，直接关闭该客户端
                    RpcClientFactory.getClient(next1.getKey()).shutdown();
                }//然后再从客户端工厂中移除该集群客户端
                iterator.remove();
            }
        }

    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:根据集群节点的网络地址生成集群客户端的唯一标识符
     */
    private String memberClientKey(Member member) {
        return "Cluster-" + member.getAddress();
    }





    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:创建并启动集群客户端的方法
     */
    private void createRpcClientAndStart(Member member, ConnectionType type) throws NacosException {
        //这个就是节点集群客户端的标签信息
        Map<String, String> labels = new HashMap<>(2);
        //设置了信息来源为集群客户端来源，也就是来源于集群客户端
        labels.put(RemoteConstants.LABEL_SOURCE, RemoteConstants.LABEL_SOURCE_CLUSTER);
        //在这里使用节点的网络地址生成了节点集群客户端的唯一标识符
        String memberClientKey = memberClientKey(member);
        
        
        //在这里创建了节点的集群客户端
        RpcClient client = buildRpcClient(type, labels, memberClientKey);
        
        
        //判断集群客户端构建连接的方式是否为指定的连接方式，在nacos中默认使用的就是grpc连接方式
        if (!client.getConnectionType().equals(type)) {
            Loggers.CLUSTER.info("connection type changed,destroy client of member - > : {}", member);
            //走到这里意味着不是grpc连接方式的，这时候就要调用客户端工厂的方法，销毁刚刚创建完毕的节点集群客户端
            RpcClientFactory.destroyClient(memberClientKey);
            //在这里再重新创建一个集群客户端
            client = buildRpcClient(type, labels, memberClientKey);
        }
        
        
        //判断集群客户端是否为等待初始化状态
        if (client.isWaitInitiated()) {
            Loggers.CLUSTER.info("start a new rpc client to member - > : {}", member);
            //走到这里意味着集群客户端是等待初始化状态，也就意味着集群客户端还没有启动呢
            //接下来为集群客户端设置了服务地址工厂，其实就是服务地址管理器
            //这里创建的这个服务地址管理器中管理的服务器地址其实就是当前创建集群客户端的节点的地址
            //这样一来，这个节点的集群客户端就可以直接和节点的服务器进行通信了
            client.serverListFactory(new ServerListFactory() {

                @Override
                public String genNextServer() {
                    //这里可以看到，生成下一个可用地址时，返回的总是当前正在创建集群客户端的目标节点的网络地址
                    return member.getAddress();
                }


                @Override
                public String getCurrentServer() {
                    return member.getAddress();
                }


                @Override
                public List<String> getServerList() {
                    return CollectionUtils.list(member.getAddress());
                }
            });
            
            //在这里启动了节点的集群客户端
            client.start();
        }
    }


    
    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:为集群中的节点创建集群客户端的方法，注意，这个方法中指定了客户端和服务端构建连接的方式，使用的就是grpc方式
     */
    private RpcClient buildRpcClient(ConnectionType type, Map<String, String> labels, String memberClientKey) {

        RpcClient clusterClient = RpcClientFactory.createClusterClient(
                memberClientKey,
                type,
                EnvUtil.getAvailableProcessors(2),
                EnvUtil.getAvailableProcessors(8),
                labels
        );
        return clusterClient;
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:判断集群中某个节点的集群客户端是否处于运行状态
     */
    public boolean isRunning(Member member) {
        RpcClient client = RpcClientFactory.getClient(memberClientKey(member));
        if (null == client) {
            return false;
        }
        return client.isRunning();
    }



    //以下就是当前节点向目标节点发送请求的方法


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:向集群中的指定节点发送请求的方法
     */
    public Response sendRequest(Member member, Request request) throws NacosException {
        //在这里设置了发送请求的超时时间
        return sendRequest(member, request, DEFAULT_REQUEST_TIME_OUT);
    }

    
    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:向集群中的指定节点发送消息的方法，并且定义了发送请求的超时时间
     */
    public Response sendRequest(Member member, Request request, long timeoutMills) throws NacosException {
        //得到目标节点的集群客户端对象
        RpcClient client = RpcClientFactory.getClient(memberClientKey(member));
        //对客户端进行判空
        if (client != null) {
            //在这里使用目标节点的客户端对象向集群中的目标节点发送了请求
            return client.request(request, timeoutMills);
        } else {
            //客户端不存在则直接抛出异常
            throw new NacosException(CLIENT_INVALID_PARAM, "No rpc client related to member: " + member);
        }
    }
    
}
```

到此为止，集群节点的客户端就定义完毕了，而且从上面的代码块中可以看到，**ClusterRpcClientProxy 集群客户端代理器也是被 springboot 管理的对象，在该对象被创建之后，会继续调用该类的 init() 方法，为集群中的其他节点创建对应的客户端对象**。并且我们也为 ClusterRpcClientProxy 集群客户端代理器定义了向集群其他节点发送请求的方法，这就意味着集群各个节点之前已经可以开始通信了。现在，我们之前定义的四个操作中的第三个操作也就实现完毕了。大家可以仔细看看上面代码块中的逻辑，如果把程序执行的流程梳理清楚了，接下来就该实现第四个操作了，**也就是定期判断集群中是否存在不健康的节点，如果存在就把该节点从集群中移除。**

## 引入集群健康检测定时任务

本小节要实现的就是集群健康检测定时任务，所谓健康检测定时任务，就是集群中的各个节点定期向彼此汇报自己的信息，如果信息汇报成功，就不会出现某个节点状态不健康的情况，如果信息汇报不成功达到了规定的次数，那就会把无法联系上的节点的状态更新为不健康状态。这就是集群健康检测定时任务的总的流程。我在这里之所以先把总流程概括出来，是因为我不能在文章中为大家实现集群健康检测的定时任务了，因为这一块的代码比较多，要是全实现的话，在文章中展示的内容就太多了，但是这一块的内容又非常简单，我在提供的第十版本代码中添加了非常详细的注释，所以大家直接去看我提供的第十版本代码即可。我在文章中只把健康检测定时任务的入口展示给大家，实际上，集群健康检测定时任务就定义在了 ServerMemberManager 集群成员管理器中，并且在集群启动的过程中，健康检测定时任务也被提交了，具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.core.cluster;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/17
 * @Description:集群成员节点管理器，注意这个集群成员节点管理器实现了springboot的ApplicationListener监听器接口，并且监听WebServerInitializedEvent事件
 */
@Component(value = "serverMemberManager")
public class ServerMemberManager implements ApplicationListener<WebServerInitializedEvent>{

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

    //集群成员信息寻址器
    private MemberLookup lookup;

    //当前集群是否使用服务地址寻址器
    private static boolean isUseAddressServer = false;


    //以下就是两个集群健康检测定时任务，并且这两个集群健康检测定时任务类都是当前集群管理器的内部类，大家直接在当前类中查看定时任务的具体逻辑即可
    
    
    //集群节点要执行的定时任务，这个任务就会把当前节点的信息汇报给集群中其他节点
    //也就是健康检测任务
    private final MemberInfoReportTask infoReportTask = new MemberInfoReportTask();

    //这个就是集群要执行的另一个健康检测定时任务，该任务会把不健康的节点从集群中移除
    private final UnhealthyMemberInfoReportTask unhealthyMemberInfoReportTask = new UnhealthyMemberInfoReportTask();



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:构造方法
     */
    public ServerMemberManager(ServletContext servletContext) throws Exception {
        this.serverList = new ConcurrentSkipListMap<>();
        //在这里调用了init()方法
        init();
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/18
     * @Description:别忘了当前类还实现了ApplicationListener监听器接口，专门监听WebServerInitializedEvent事件，该方法就会在springboot发布了WebServerInitializedEvent事件后被回调
     * 而WebServerInitializedEvent事件则会在web服务器启动完毕之后被springboot发布
     */
    @Override
    public void onApplicationEvent(WebServerInitializedEvent event) {
        
        //在这里为当前节点设置上线状态
        getSelf().setState(NodeState.UP);
        //判断当前节点是否以单机模式启动，如果不是则执行对应的集群模式操作
        if (!EnvUtil.getStandaloneMode()) {
            //走到这里意味着当前节点是以集群模式启动的，这个时候就可以提交两个集群模式下的定时任务了
            //这里提交的是集群节点信息汇报定时任务
            GlobalExecutor.scheduleByCommon(this.infoReportTask, DEFAULT_TASK_DELAY_TIME);
            //这里提交的事集群节点健康检查定时任务
            GlobalExecutor.scheduleByCommon(this.unhealthyMemberInfoReportTask, DEFAULT_TASK_DELAY_TIME);
        }
        //设置web服务的端口号到环境变量中
        EnvUtil.setPort(event.getWebServer().getPort());
        //设置本地地址
        EnvUtil.setLocalAddress(this.localAddress);
        //记录日志信息
        Loggers.CLUSTER.info("This node is ready to provide external services");
    }
    
    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:初始化集群成员管理器的方法，该方法中的操作都很简单，我就不添加那么详细注释了
     */
    protected void init() throws NacosException {
        Loggers.CORE.info("Nacos-related cluster resource initialization");
        //得到端口号，默认是8848
        this.port = EnvUtil.getProperty(SERVER_PORT_PROPERTY, Integer.class, DEFAULT_SERVER_PORT);
        this.localAddress = InetUtils.getSelfIP() + ":" + port;
        //解析当前节点信息
        this.self = MemberUtil.singleParse(this.localAddress);
        //设置当前节点的版本信息
        this.self.setExtendVal(MemberMetaDataConstants.VERSION, VersionUtils.version);
        //开启grpc通信模式
        this.self.setGrpcReportEnabled(true);
        //把当前节点信息添加到serverList成员变量中
        serverList.put(self.getAddress(), self);

        
        //初始化并且启动集群节点信息寻址器
        initAndStartLookup();
        
        //判断集群服务列表是否为空，因为在启动集群节点信息寻址器的时候
        //寻址器就已经解析了集群的配置信息，把集群成员的节点信息都设置到serverList中了
        //如果现在判断到集群服务列表为空，那就意味着集群出现问题了，直接抛出异常即可
        if (serverList.isEmpty()) {
            throw new NacosException(NacosException.SERVER_ERROR, "cannot get serverlist, so exit.");
        }
        //记录操作日志
        Loggers.CORE.info("The cluster resource is initialized");
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/17
     * @Description:初始化并启动集群成员信息寻址器的方法
     */
    private void initAndStartLookup() throws NacosException {
        //创建寻址器对象
        this.lookup = LookupFactory.createLookUp(this);
        //判断寻址器对象是否是服务地址寻址器，给isUseAddressServer成员变量赋值
        isUseAddressServer = this.lookup.useAddressServer();
        //启动寻址器
        this.lookup.start();
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/18
     * @Description:得到集群中除了自己之外的所有节点的方法
     */
    public List<Member> allMembersWithoutSelf() {
        List<Member> members = new ArrayList<>(serverList.values());
        members.remove(self);
        return members;
    }



    //其他内容暂时省略
    
}




//这里我还要再简单解释一下，当集群节点执行MemberInfoReportTask健康检测定时任务时，会在任务中向集群的其他节点发送MemberReportRequest请求
//该请求就会封装当前节点的信息，把信息发送给集群其他节点，让其他节点校验。那么，目标节点接收到MemberReportRequest请求之后，需要调用
//对应的处理器来处理MemberReportRequest请求，所以处理MemberReportRequest请求的处理器也要给大家展示一下
//这个处理器就是MemberReportHandler，请看下面代码块
package com.cqfy.nacos.core.cluster.remote;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/17
 * @Description:集群成员信息处理器
 */
@Component
public class MemberReportHandler extends RequestHandler<MemberReportRequest, MemberReportResponse> {

    //集群成员管理器
    private final ServerMemberManager memberManager;

    public MemberReportHandler(ServerMemberManager memberManager) {
        this.memberManager = memberManager;
    }



    @Override
    public MemberReportResponse handle(MemberReportRequest request, RequestMeta meta) throws NacosException {
        //从请求中获得发送请求信息的集群成员
        Member node = request.getNode();
        //判断成员网络地址是否合规
        if (!node.check()) {
            //如果集群成员网络地址不合规，就直接返回一个错误响应
            MemberReportResponse result = new MemberReportResponse();
            result.setErrorInfo(400, "Node information is illegal");
            return result;
        }//走到这里就意味着发送MemberReportRequest请求过来的集群成员信息是合规的
        //如果日志开启了debug等级模式，就记录日志信息
        LoggerUtils.printIfDebugEnabled(Loggers.CLUSTER, "node state report, receive info : {}", node);
        //设置集群成员为上线状态，可以正常提供服务
        node.setState(NodeState.UP);
        //重制节点的失败访问次数
        node.setFailAccessCnt(0);
        //更新集群管理器中该成员的最新信息
        memberManager.update(node);
        //返回成功响应
        return new MemberReportResponse(memberManager.getSelf());
    }

}
```

到此为止，我就把我们自己构建的集群的启动流程为大家全部实现完毕了，也就意味着集群的潜质工作已经准备完毕了。接下来，终于该轮到集群的正式工作登场了，我们可以开始着手实现集群的数据备份和功能可用这两个基本功能了。这也正是下一章的核心内容，好了朋友们，我们下一章见！

附：这一章的内容和上一章的内容都非常简单，我在文章中把各个功能都实现的比较完善，虽然也有一些内容没有完全展开，但是我在文章中把这些内容的核心知识点都展示出来了，并且在我提供的第十版本代码中，这些内容也都添加了非常详细的注释，大家可以直接去看我提供的第十版本代码。到此为止，第十版本的所有内容都可以供大家阅读了。下一篇文章对应的就是第十一版本代码了。