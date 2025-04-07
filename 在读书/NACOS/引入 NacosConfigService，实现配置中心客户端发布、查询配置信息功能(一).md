从本章开始，我们要就开始一起构建 nacos 的配置中心了。说实话，其实 nacos 的配置中心我都不太想更新了，原因有三点：

1 nacos 的配置中心设计得非常简单，不管是配置中心客户端还是服务端，都非常简单，根本没有什么复杂的逻辑，代码量也很少。

2 nacos 的配置中心模块用到了很多以前已经实现的功能组件，比如说构建 nacos 配置中心客户端和服务端，用的都是我们在实现注册中心时构建的 RpcClient 和 RpcServer 等等组件，总之，很多重要的组件，甚至是构建配置中心客户端的方式，都和注册中心没什么区别，我一会给大家展示一下代码，大家就清楚了。

3 在更新 nacos 框架之前，我已经把 hippo4j 动态线程池框架给大家更新完毕了，在更新 hippo4j 框架的时候，我就跟大家说过，这个框架大量借鉴和复制了 nacos 配置中心的代码，就动态变更配置功能来说，这两个框架的设计原理以及用到的功能组件不能说是一模一样，也能说是差不多了。无非就是 hippo4j 使用的是长轮询，nacos 配置中心使用的是长连接，而长连接比长轮询更简单了。还有一点，那就是 hippo4j 框架是我们这个系列课程中最简单的，如果大家把 hippo4j 这个框架看完了，那么在学习 nacos 配置中心时，就会感觉非常简单，甚至不必看文章，直接去看我提供的第十四和十五版本代码即可。

以上就是我认为的没必要更新 nacos 配置中心的原因，或者说没必要详细更新 nacos 配置中心的原因。当然，我也能猜到，就算我列完以上三个原因，还是有朋友会觉得我有点夸张了，或者认为我想偷懒，不想认认真真保证质量地更新课程。我可以跟大家保证，我绝对没有这种意思，如果大家不相信，可以先看看本章的内容，**在本章我会先为大家把 nacos 配置中心客户端构建完毕，并且实现向服务端发布配置信息和读取配置信息功能**，看完之后，大家了解了 nacos 配置中心有多简单，就会相信我刚才所说的了。

## 引入 nacos 配置中心 NacosConfigService 服务组件

  

请大家先回忆一下，在构建 nacos 注册中心客户端的时候，我们是怎么做的呢？也许大家已经记不太清楚了，没关系我帮大家简单回忆一下。实际上，我们并没有直接创建 nacos 注册中心客户端对象，而是首先定义了一个 NacosNamingService 类，这个类的对象专门提供和 nacos 注册中心相关的各种服务，比如注册服务实例信息到服务端，向服务端订阅某个 Service 等等。而在创建 NacosNamingService 对象的过程中，nacos 客户端也就被创建成功，并且被启动了，之后 nacos 注册中心客户端就可以和服务端成功建立连接，展开通信。我把相关的代码搬运到这里了，帮助大家回顾一下，请看下面代码块。

```
/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：这个类是注册中心客户端的核心类，可以说是注册服务实例，订阅服务实例等等操作的入口类，很多重要的操作都是由这个类的对象发起的
 * 但在第一版本代码中，该类只提供了注册服务实例到注册中心的方法
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosNamingService implements NamingService {


    //其他成员变量省略


    //代理客户端，该客户端就是用来向注册中心服务器发送请求的
    //这里之所以叫代理客户端，是因为NamingClientProxy内部真正向服务器发送消息时，使用的是grpc构建的客户端
    private NamingClientProxy clientProxy;



    //构造方法，该构造方法通过Properties文件创建NacosNamingService对象，反射调用的就是这个构造方法
    //至于被反射调用的时机，就是在NamingFactory对象中被调用的
    public NacosNamingService(Properties properties) throws NacosException {
        init(properties);
    }


    //初始化NacosNamingService对象的方法，在该方法对NacosNamingService内部要使用的一些组件进行了创建和初始化
    private void init(Properties properties) throws NacosException {

        //该方法的其他内容省略
        
        //创建代理客户端对象
        this.clientProxy = new NamingClientProxyDelegate(this.namespace,serviceInfoHolder,nacosClientProperties,changeNotifier);
    }


    //该类的其他内容省略
}


//从上面的代码中可以看到，在创建NacosNamingService对象的过程中，会在NacosNamingService构造方法中
//创建NamingClientProxyDelegate对象，这就是客户端代理器，已经和客户端有关系了，所以我们继续往下看，看看在创建NamingClientProxyDelegate对象的
//过程中会做什么，请看下面代码块


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：代理客户端，这个客户端内部持有了grpcClientProxy客户端，真正发送服务实例给服务端的操作，是grpcClientProxy客户端执行的
 */
public class NamingClientProxyDelegate implements NamingClientProxy {

    //服务管理器，这里我要解释一下，这个服务管理器管理的是nacos服务器的，因为nacos本身就可以搭建成集群，并且还是raft集群
    //nacos客户端向nacos服务端发送消息的时候，如果接收消息的服务端故障了，这个时候nacos客户端就可以选择另外的服务端地址重新发送消息给服务端
    //这个nacos服务端地址可以定义在配置文件中，也可以定义在配置类中，总之会在ServerListManager兑现初始化的时候被设置到ServerListManager的serverList成员变量中
    private final ServerListManager serverListManager;

    //这个就是grpc客户端，这里我还要强调一下，实际上在nacos中还有一个http客户端，这个是旧版本的nacos中使用的客户端
    //但是我们是基于最新版本nacos框架来更新课程的，在2.x版本后，nacos引入了grpc框架来实现网络通信，所以我就直接把http客户端暂时移除了
    //不管我们要注册的服务实例是临时的还是持久化的，都可以使用grpc客户端来发送响应的注册服务实例请求给服务端
    private final NamingGrpcClientProxy grpcClientProxy;

    //服务实例信息保存器
    private final ServiceInfoHolder serviceInfoHolder;

    //客户端主动访问服务端，判断服务实例是否更新的组件
    private final ServiceInfoUpdateService serviceInfoUpdateService;


    public NamingClientProxyDelegate(String namespace, ServiceInfoHolder serviceInfoHolder, NacosClientProperties properties, InstancesChangeNotifier changeNotifier) throws NacosException {
        this.serviceInfoUpdateService = new ServiceInfoUpdateService(properties, serviceInfoHolder, this, changeNotifier);
        //nacos注册中心要用到的服务地址管理器
        this.serverListManager = new ServerListManager(properties, namespace);
        this.serviceInfoHolder = serviceInfoHolder;
        //可以看到，在这里创建了grpc客户端代理对象
        this.grpcClientProxy = new NamingGrpcClientProxy(namespace, serverListManager, properties,serviceInfoHolder);
    }

    //该类的其他内容省略
}


//从上面的NamingClientProxyDelegate类中可以发现，在创建NamingClientProxyDelegate对象的时候
//也就是在调用该对象的构造方法时，会创建NamingGrpcClientProxy这个grpc客户端代理对象，并且也把客户端要用到的ServerListFactory服务地址管理器创建完毕了
//所以我们接着往下看，看看在创建NamingGrpcClientProxy对象的过程中会发生什么，请看下面代码块


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //服务实例的命名空间
    private final String namespaceId;

    //要创建的rpc客户端的唯一标识
    private final String uuid;

    //请求超时时间
    private final Long requestTimeout;

    //rpc客户端
    private final RpcClient rpcClient;

    //该成员变量封装的就是断线重连后的操作，我愿意称它为重做服务对象
    private final NamingGrpcRedoService redoService;



    //构造方法，这里面的操作大多比较简单，注释就不加得很详细了
    public NamingGrpcClientProxy(String namespaceId, ServerListFactory serverListFactory, NacosClientProperties properties, ServiceInfoHolder serviceInfoHolder) throws NacosException {
        this.namespaceId = namespaceId;
        //创建rpc客户端的唯一标识
        this.uuid = UUID.randomUUID().toString();
        //从配置类中获得请求超时时间，如果没有定义，就返回默认值-1
        this.requestTimeout = Long.parseLong(properties.getProperty(CommonParams.NAMING_REQUEST_TIMEOUT, "-1"));

        //省略部分内容
        
        //在这里创建了rpc客户端
        //RpcClientTlsConfig.properties(properties.asProperties()得到了一个tls配置类对象，创建rpc客户端时会用到
        this.rpcClient = RpcClientFactory.createClient(uuid, ConnectionType.GRPC, labels, RpcClientTlsConfig.properties(properties.asProperties()));
        //创建断线重连操作的服务对象
        this.redoService = new NamingGrpcRedoService(this, properties);
        NAMING_LOGGER.info("Create naming rpc client for uuid->{}", uuid);
        //启动rpc客户端
        start(serverListFactory,serviceInfoHolder);
    }



    //启动rpc客户端工作的方法
    private void start(ServerListFactory serverListFactory, ServiceInfoHolder serviceInfoHolder) throws NacosException {
        //把nacos服务地址管理器交给rpc客户端使用，ServerListManager实现了ServerListFactory接口，所以这里传递的是一个接口
        rpcClient.serverListFactory(serverListFactory);
        //注册断线重连监听器到客户端，监听器就是redoService对象，当客户端连接成功或者连接断开后，监听器就会执行相应的方法
        //这个时候，大家应该就明白了，第二版本代码中的连接事件是干什么用的了
        rpcClient.registerConnectionListener(redoService);
        //在这里把处理服务端主动推送过来的服务信息变更请求的处理器添加到处理器集合中
        rpcClient.registerServerRequestHandler(new NamingPushRequestHandler(serviceInfoHolder));
        //启动rpc客户端
        rpcClient.start();
        //这里是把当前类的对象当成一个观察者了，专门观察服务地址变更事件，注意，这里的服务地址并不是注册中心注册的服务实例的地址
        //而是nacos服务端地址的变更事件，这行代码大家可以暂时忽略，后面版本才会讲到，这里引入进来是因为NamingGrpcClientProxy类的继承和实现关系重构完整了
        //为了表明这个类还是一个观察者，所以引入了这样一行代码
        NotifyCenter.registerSubscriber(this);
    }


    //该类的其他内容省略

}


//从上面代码块中可以看到，在创建NamingGrpcClientProxy对象的时候，在该对象的构造方法中
//终于创建了RpcClient客户端，这个就是注册中心要使用的客户端对象，并且也把ServerListFactory服务地址管理器
//交给RpcClient客户端对象使用了，在创建完毕RpcClient客户端对象之后，就启动了该客户端，这样一来nacos注册中心客户端
//就可以和服务端建立连接，然后通信了。
```

到此为止，我就帮大家把nacos注册中心客户端构建并启动的过程回顾完毕了，接下来还是继续把目光集中在 nacos 配置中心上吧，现在要构建配置中心客户端，并且我之前也说了，nacos 配置中心客户端用到了很多以前实现过的组件，而且也是仿照着注册中心客户端的流程来构建的。那这就很好说了，**我们就可以仿照注册中心客户端，也给 nacos 配置中心客户端定义一个 Service 服务组件，这个组件专门负责 nacos 配置中心向服务端发布配置信息、以及获取配置信息、还有订阅配置信息等功能。这个服务组件就可以定义为 NacosConfigService**，具体实现可以仿照着 nacos 注册中心的 NacosNamingService 来实现。比如说，**我可以在专门为配置中心定义的 NacosConfigService 组件中定义该服务发布的配置信息所属的 namespace 命名空间，也可以在 NacosConfigService 中创建配置中心客户端要使用的 ServerListManager 服务地址管理器，还可以把发布配置信息，查询配置信息等等方法都先定义一下(发布配置信息这些基础知识应该不用我再详细讲解了吧？我相信只要使用过 nacos 配置中心的朋友们都知道发布配置信息的方法，你可以自己定义配置信息的 dataId，该配置信息所属的 group，以及 namespace。在相同的命名空间和 group 下，每一个发布到配置中心服务端的配置信息，或者说配置文件都有一个唯一的 dataId，这些知识大家应该都清楚了，我就不在文章中讲解了)**。所以，我们专门为 nacos 配置中心引入的 NacosConfigService 组件目前可以先简单定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.client.config;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:配置中心服客户端服务组件
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosConfigService implements ConfigService {

    private static final Logger LOGGER = LogUtils.logger(NacosConfigService.class);

    //表示当前服务状态的标志
    //上线标志
    private static final String UP = "UP";

    //下线标志
    private static final String DOWN = "DOWN";

    //命名空间
    private String namespace;


     //构造方法
    public NacosConfigService(Properties properties) throws NacosException {
        //异步加载序列化工具
        PreInitUtils.asyncPreLoadCostComponent();
        //得到nacos自己的客户端配置对象
        final NacosClientProperties clientProperties = NacosClientProperties.PROTOTYPE.derive(properties);
        //校验配置信息合法性
        ValidatorUtils.checkInitParam(clientProperties);
        //给命名空间赋值
        initNamespace(clientProperties);
        
        //创建服务地址管理器，这个管理器存放的都是nacos配置中心服务端的地址
        ServerListManager serverListManager = new ServerListManager(clientProperties);
        //启动服务地址管理器
        serverListManager.start();

    }


     //初始化命名空间的方法
    private void initNamespace(NacosClientProperties properties) {
        namespace = ParamUtil.parseNamespace(properties);
        properties.setProperty(PropertyKeyConst.NAMESPACE, namespace);
    }


   /**
    * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
    * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
    * @Date:2024/8/31
    * @Description:客户端得到指定的配置文件的方法
    */
    @Override
    public String getConfig(String dataId, String group, long timeoutMs) throws NacosException {
        //该方法暂时不做实现
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:客户端向服务端发布配置文件的方法，这个方法不可以指定客户端要发布的配置文件的类型
     * 只能发布默认类型，也就是text文件，方法参数content就是配置信息的内容
     */
    @Override
    public boolean publishConfig(String dataId, String group, String content) throws NacosException {
        //我把ConfigType枚举类的内容也展示在当前代码块中了
        return publishConfig(dataId, group, content, ConfigType.getDefaultType().getType());
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:客户端向服务端发布配置文件的方法，这个方法可以指定客户端要发布的配置文件的类型
     */
    @Override
    public boolean publishConfig(String dataId, String group, String content, String type) throws NacosException {
        //该方法暂时不做实现
    }

}





//下面就是ConfigType枚举类的内容

package com.cqfy.nacos.api.config;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:nacos客户端可以发布的配置文件的类型
 */
public enum ConfigType {

    /**
     * config type is "properties".
     */
    PROPERTIES("properties"),

    /**
     * config type is "xml".
     */
    XML("xml"),

    /**
     * config type is "json".
     */
    JSON("json"),

    /**
     * config type is "text".
     */
    TEXT("text"),

    /**
     * config type is "html".
     */
    HTML("html"),

    /**
     * config type is "yaml".
     */
    YAML("yaml"),

    /**
     * config type is "toml".
     */
    TOML("toml"),

    /**
     * not a real type.
     */
    UNSET("unset");

    private final String type;

    private static final Map<String, ConfigType> LOCAL_MAP = new HashMap<>();

    static {
        for (ConfigType configType : values()) {
            LOCAL_MAP.put(configType.getType(), configType);
        }
    }

    ConfigType(String type) {
        this.type = type;
    }

    public String getType() {
        return type;
    }

    public static ConfigType getDefaultType() {
        return TEXT;
    }

    /**
     * check input type is valid.
     *
     * @param type config type
     * @return it the type valid
     */
    public static Boolean isValidType(String type) {
        if (StringUtils.isBlank(type)) {
            return false;
        }
        return null != LOCAL_MAP.get(type);
    }
}
```

上面代码块就展示了我们目前为 nacos 配置中心定义好的 NacosConfigService 组件，在看完上面的代码块之后，大家肯定会有很多问题，就比如说，**上面的代码块中根本没有定义和客户端相关的任何成员变量**，大家肯定会对此感到困惑。当然，我自己把上面的 NacosConfigService 类定义完毕之后，心里也有很多问题，这也是合情合理的，毕竟 NacosConfigService 类目前只是个半成品，它还有很多不完善的地方，很多方法没有实现，也没有定义。就比如说，**我们在 NacosConfigService 对象构造方法中创建了配置中心客户端要使用的 ServerListManager 服务地址管理器，但是我们并没有使用这个服务地址管理器。这一点肯定会让大家感到困惑，而且还让大家感到困惑的可能是这个 ServerListManager 服务地址管理器本身**。虽然我在上面代码块中创建了 ServerListManager 服务地址管理器对象，但是大家目前还并不知道 nacos 配置中心要使用的服务地址管理器是怎么定义的，接下来就让我为大家简单解释一下。

首先让我来为大家解释第一个问题，那就是明明在新定义的 NacosConfigService 类的构造方法中创建了 ServerListManager 服务地址管理器，但是并没有用到它。其实原因非常简单，我们都知道，服务地址管理器肯定是要被客户端使用的，nacos 注册中心客户端就拥有自己的 ServerListManager 服务地址管理器，配置中心客户端当然也要拥有。但目前我们还没有把配置中心的客户端 NacosConfigService 类中，所以也就还没有用到创建完毕的这个配置中心客户端 ServerListManager 服务地址管理器。

至于 nacos 配置中心的 ServerListManager 服务地址管理器是如何定义的，这个就更简单了，和 nacos 注册中心客户端用到的 ServerListManager 服务地址管理器几乎一样，所以我就不在本章中重复讲解了，大家直接去看我提供的第十四版本代码即可，代码中注释非常详细。

好了，在简单回答了和配置中心客户端要使用的 ServerListManager 服务地址管理器的一些问题之后，接下来再让我为大家展示一个 NacosConfigService 服务组件的使用例子，**在这个例子中会为大家展示 NacosConfigService 类的对象如何被创建，以及如何使用 NacosConfigService 对象向配置中心服务端发布配置信息**。当然，我也知道大家信息很着急，**希望我能尽快实现 nacos 配置中心的客户端，并且实现配置中心客户端向服务端发布配置信息的功能，也就是实现上面代码块中的 publishConfig() 方法**。大家先别急，重要的功能和知识肯定是放到后面实现和讲解，等下一小节，我就会为实现这部分的功能了。接下来，还是请大家先看一下 NacosConfigService 对象的使用方式，请看下面代码块。

```
package com.cqfy.nacos.example;



public class ConfigExample {

    public static void main(String[] args) throws NacosException, InterruptedException {

        //配置中心服务端地址
        String serverAddr = "localhost";

        //配置中心要发布的配置信息的dataId
        String dataId = "test";
        //配置信息所在分组
        String group = "DEFAULT_GROUP";

        //创建配置中心客户端需要的配置信息对象
        Properties properties = new Properties();
        properties.put("serverAddr", serverAddr);

        //在这里使用nacos工厂创建NacosConfigService对象
        //NacosConfigService类实现了ConfigService接口
        //我把NacosFactory工厂对象的内容也展示在当前代码块中了
        ConfigService configService = NacosFactory.createConfigService(properties);

        //从服务端查询执行的配置信息，指定查询超时时间
        String content = configService.getConfig(dataId, group, 5000);
        System.out.println("[config content] " + content);

        //向服务端发布配置信息，配置信息内容为字符串content
        boolean isPublishOk = configService.publishConfig(dataId, group, "content");
        System.out.println("[publish result] " + isPublishOk);

    }
}



//以下是NacosFactory类的内容

package com.cqfy.nacos.api;


public class NacosFactory {

    
    public static ConfigService createConfigService(Properties properties) throws NacosException {
        //ConfigFactory类的内容也在当前代码块中
        return ConfigFactory.createConfigService(properties);
    }


    //该类的其他内容省略
}



//以下是ConfigFactory类的内容

package com.cqfy.nacos.api.config;


public class ConfigFactory {

   
    public static ConfigService createConfigService(Properties properties) throws NacosException {
        try {
            Class<?> driverImplClass = Class.forName("com.alibaba.nacos.client.config.NacosConfigService");
            Constructor constructor = driverImplClass.getConstructor(Properties.class);
            ConfigService vendorImpl = (ConfigService) constructor.newInstance(properties);
            return vendorImpl;
        } catch (Throwable e) {
            throw new NacosException(NacosException.CLIENT_INVALID_PARAM, e);
        }
    }

    //该类其他内容省略
}
```

上面代码块就展示了 nacos 配置中心的 NacosConfigService 组件的具体用法，结合上面代码块中的注释，大家应该也清楚了配置中心客户端怎么从服务端查询指定的配置信息，以及怎么向配置中心发布配置信息。当然，大家目前也只是清楚 NacosConfigService 对象的使用方式，还并不了解程序内部的运行原理，因为到目前为止，我还没有为 nacos 配置中心引入客户端对象，也没有真正实现 NacosConfigService 类的最重要的 publishConfig() 发布配置信息的方法。接下来就让我来实现这些功能吧。

## 引入 ClientWorker，启动配置中心客户端

我在上一小节一直没有为 NacosConfigService 组件定义客户端对象，**是因为在 nacos 源码中，配置中心客户端并不是定义在这个类中的**。之前我已经跟大家分析了，nacos 配置中心使用的也是我们早就构建完毕的 RpcClient 客户端对象，那看起来只要把 RpcClient 客户端对象定义在 NacosConfigService 组件中，然后在创建 NacosConfigService 对象的时候，同时也创建 RpcClient 对象，并且把早就创建完毕的 ServerListManager 服务地址管理器交给 RpcClient 对象使用，最后启动 RpcClient 客户端对象，这样一来，nacos 配置中心的客户端就启动成功了，也就可以和服务端建立连接，然后展开通信了。我这样分析下来，应该没有朋友会反对吧？这个流程非常普通，也很容易理解。但当大家看过 nacos 配置中心客户端的源码后，就会发现情况并不总是我们想象的这样，**在 nacos 配置中心源码中，还引入了一个新的组件，就是本小节标题中展示的 ClientWorker 类，翻译过来就是负责客户端工作的组件。而我们要为配置中心构建的客户端，其实就定义在了 ClientWorker 类中**，并且通过这个 ClientWorker 类的名字我们大概也能意识到，**既然这个组件是用来负责配置中心客户端工作的，它也持有了配置中心客户端对象，那么向配置中心服务端发送消息的任务肯定就会交给 ClientWorker 类的对象来执行**。这样分析下来，我相信每一位朋友都能想到，**真正向配置中心服务端发布配置信息，从服务端查询指定配置信息的操作其实也都是 ClientWorker 类的对象执行的**。所以这个 ClientWorker 类目前可以定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:客户端工作组件，在以前它使用长轮询机制监听配置文件变更情况，但在2.x版本之后，就不再使用长轮询，而是长连接了
 */
public class ClientWorker implements Closeable {


     //当前客户端工作组件的唯一标识
    private final String uuid = UUID.randomUUID().toString();


    //配置中心客户端
    private final RpcClient agent;



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:构造方法，这个方法其实是伪代码，后面会对其进行重构
     */
    @SuppressWarnings("PMD.ThreadPoolCreationRule")
    public ClientWorker(ServerListManager serverListManager,final NacosClientProperties properties) throws NacosException {

        //在这里创建配置中心客户端
        agent = new RpcClient(properties, serverListManager);
       
        //启动配置中心客户端
        agent.start();


    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端向服务端发布配置信息的方法，这个方法中的encryptedDataKey参数大家可以暂时忽略
     */
    public boolean publishConfig(String dataId, String group, String tenant, String appName, String tag, String betaIps,
                                 String content, String encryptedDataKey, String casMd5, String type) throws NacosException {
        //该方法暂时不做实现
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端从服务端获取指定配置信息的方法，ConfigResponse这个响应中就封装了从服务端获得的配置信息
     */
    public ConfigResponse getServerConfig(String dataId, String group, String tenant, long readTimeout, boolean notify)
            throws NacosException {
        //该方法暂时不做实现
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:移除指定配置文件的方法
     */
    public boolean removeConfig(String dataId, String group, String tenant, String tag) throws NacosException {
        //该方法暂时不做实现
    }


    //该类的其他内容暂时不做实现

}
```

上面代码块展示的就是我仿照源码定义完毕的 ClientWorker 类，可以看到，我们把配置中心要用到的 RpcClient 客户端对象定义 ClientWorker 类的成员变量了，并且在 ClientWorker 类的构造方法中创建并启动了配置中心客户端。这其实就意味着，**只要 ClientWorker 类的对象一被创建，配置中心客户端也就启动了**。那么 ClientWorker 类的对象应该在哪里被创建呢？

看到这里，我想大家应该也都能猜到了，**既然真正向配置中心服务端发布配置信息，从服务端查询指定配置信息的操作其实也都是 ClientWorker 类的对象执行的，而 NacosConfigService 类中也定义发布配置信息，查询配置信息的方法，这就意味着 NacosConfigService 服务组件只是操作的发起者，ClientWorker 类的对象才是真正的执行者**。所以，**我们只需要把 ClientWorker 对象定义为 NacosConfigService 类的成员变量，在创建 NacosConfigService 对象的时候，在构造方法中把 ClientWorker 对象创建出来即可，这样一来，配置中心客户端也就被创建启动了**。所以，NacosConfigService 类就可以重构成下面这样，请看下面代码块。

```
package com.cqfy.nacos.client.config;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:配置中心服客户端服务组件
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosConfigService implements ConfigService {

    private static final Logger LOGGER = LogUtils.logger(NacosConfigService.class);

    //表示当前服务状态的标志
    //上线标志
    private static final String UP = "UP";

    //下线标志
    private static final String DOWN = "DOWN";

    //命名空间
    private String namespace;

    //客户端工作组件，这个组件提供了和服务端通信的功能
    //还提供了客户端监听服务端配置变更的功能，但是这个功能在第14版本代码中并没有用到
    private final ClientWorker worker;


     //构造方法
    public NacosConfigService(Properties properties) throws NacosException {
        //异步加载序列化工具
        PreInitUtils.asyncPreLoadCostComponent();
        //得到nacos自己的客户端配置对象
        final NacosClientProperties clientProperties = NacosClientProperties.PROTOTYPE.derive(properties);
        //校验配置信息合法性
        ValidatorUtils.checkInitParam(clientProperties);
        //给命名空间赋值
        initNamespace(clientProperties);
        
        //创建服务地址管理器，这个管理器存放的都是nacos配置中心服务端的地址
        ServerListManager serverListManager = new ServerListManager(clientProperties);
        //启动服务地址管理器
        serverListManager.start();

        //创建客户端工作组件，现在这个ServerListManager服务地址管理器就被用到了
        this.worker = new ClientWorker(serverListManager, clientProperties);

    }


     //初始化命名空间的方法
    private void initNamespace(NacosClientProperties properties) {
        namespace = ParamUtil.parseNamespace(properties);
        properties.setProperty(PropertyKeyConst.NAMESPACE, namespace);
    }


   /**
    * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
    * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
    * @Date:2024/8/31
    * @Description:客户端得到指定的配置文件的方法
    */
    @Override
    public String getConfig(String dataId, String group, long timeoutMs) throws NacosException {
        return getConfigInner(namespace, dataId, group, timeoutMs);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:得到配置文件的方法，该方法的整体流程非常简单，其实就是先尝试从本地的故障转移文件中得到配置文件信息
     * 如果没有获取，就从nacos配置中心服务器获取
     */
    private String getConfigInner(String tenant, String dataId, String group, long timeoutMs) throws NacosException {

        //使用客户端工作组件从配置中心服务器得到配置文件，配置文件封装在了响应对象中
        ConfigResponse response = worker.getServerConfig(dataId, group, tenant, timeoutMs, false);
        return response.getContent();
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:客户端向服务端发布配置文件的方法，这个方法不可以指定客户端要发布的配置文件的类型
     * 只能发布默认类型，也就是text文件，方法参数content就是配置信息的内容
     */
    @Override
    public boolean publishConfig(String dataId, String group, String content) throws NacosException {
        //我把ConfigType枚举类的内容也展示在当前代码块中了
        return publishConfig(dataId, group, content, ConfigType.getDefaultType().getType());
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:客户端向服务端发布配置文件的方法，这个方法可以指定客户端要发布的配置文件的类型
     */
    @Override
    public boolean publishConfig(String dataId, String group, String content, String type) throws NacosException {
        return publishConfigInner(namespace, dataId, group, null, null, null, content, type, null);
    }
    


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:配置中心客户端发布配置文件的方法，这个方法的参数中有一个casMd5字符串，如果客户端在发布配置信息时计算了配置文件的md5
     * 那么配置文件被服务端接收后，就会比较配置文件的md5和之前存储的配置文件的md5是否不一致，如果不一致说明客户端发布的配置文件是更新过的
     * 同时我们也能看到该方法中还有几个参数，比如说betaIps、tag等等，tag是配置信息的标签，是在dataId之下对配置信息更细粒度的划分
     * betaIps就是灰度发布的IP地址，这些内容在nacos主线章节中并不会涉及，等到我为大家更新nacos支线章节时，会为大家实现和这些参数相关的功能
     */
    private boolean publishConfigInner(String tenant, String dataId, String group, String tag, String appName,
                                       String betaIps, String content, String type, String casMd5) throws NacosException {
        //对配置文件所在组名判空，为空则使用默认组名
        group = blank2defaultGroup(group);
        //判断配置文件是否合规
        ParamUtils.checkParam(dataId, group, content);
        //在这里使用客户端工作组件把配置文件发不到服务端
        return worker.publishConfig(dataId, group, tenant, appName, tag, betaIps, content, casMd5, type);
    }

}
```

上面代码块中的注释非常详细，所以我就不在文章中重复解释了。看完重构之后的 NacosConfigService 类，现在大家应该彻底清楚了 nacos 配置中心客户端 NacosConfigService 服务组件的工作原理了吧？确切地说，**重要的并不是 NacosConfigService 组件，这个组件只是和服务端打交道的操作的发起者，真正的执行者是 ClientWorker 对象，真正重要的也是 ClientWorker 对象**。所以接下来我们还是要再回到 ClientWorker 类中，把 ClientWorker 类中真正向服务端发布配置信息的方法、以及从服务端查询配置信息的方法实现了。当然，首先要实现的肯定是客户端向服务端发布配置信息的方法，只有发布了配置信息，才能查询呀，所以接下来，我们就一起来分析分析，ClientWorker 对象的 publishConfig() 方法该怎么实现。

  

其实这个也很好分析，毕竟我们已经有了实现 nacos 注册中心的经验。**在 nacos 框架中，不管注册中心还是配置中心，其客户端和服务端底层都是使用 grpc 框架进行通信的，通信的方式非常直接，客户端发送对应的请求给服务端，服务端有对应的请求处理器可以处理该请求，这是我们实现 nacos 注册中心时，实现的客户端和服务端的通信方式**。来到 nacos 的配置中心，现在配置中心客户端要和服务端进行通信了，并且要进行的是发布配置信息的通信，**那就直接定义一个请求对象，客户端把配置信息都封装在这个请求对象中，然后把请求对象发送给服务端，服务端使用对应的处理器处理该请求对象，从请求对象中解析出配置信息，然后保存即可**。这个逻辑大家应该都能立即吧？看起来非常简单，就是我们曾经实现过无数次的功能了。当然，为配置中心服务端定义对应的请求处理器处理客户端发布的配置信息，这个功能并不会在本章实现，后面我们构建配置中心服务端的时候，会实现对应的功能。那接下来，我们只需要把要发送给配置中心服务端，封装了配置信息的请求对象定义出来即可，有请求对象就意味着有对应的响应对象，所以我也会把与该请求对象对应的响应对象一起定义出来。

我把这个封装了配置信息的请求对象定义为 ConfigPublishRequest，具体实现请看下面代码块。

```
package com.cqfy.nacos.api.config.remote.request;


//可以看到，该类继承了AbstractConfigRequest类，我把AbstractConfigRequest类的内容也展示在当前代码块中了
public class ConfigPublishRequest extends AbstractConfigRequest {
    
    String content;
    
    String casMd5;
    
    private Map<String, String> additionMap;
    
    public ConfigPublishRequest() {
    
    }
    
    public ConfigPublishRequest(String dataId, String group, String tenant, String content) {
        this.content = content;
        super.setGroup(group);
        super.setTenant(tenant);
        super.setDataId(dataId);
    }
    

    
    //设置要发布的配置信息的额外参数信息
    public void putAdditionalParam(String key, String value) {
        if (additionMap == null) {
            additionMap = new HashMap<>(2);
        }
        additionMap.put(key, value);
    }

    //该类的其他内容省略
)



//下面就是AbstractConfigRequest类的内容

package com.cqfy.nacos.api.config.remote.request;

public abstract class AbstractConfigRequest extends Request {
    
    private String dataId;
    
    private String group;
    
    private String tenant;
    
    public String getDataId() {
        return dataId;
    }
    
    public void setDataId(String dataId) {
        this.dataId = dataId;
    }
    
    public String getGroup() {
        return group;
    }
    
    public void setGroup(String group) {
        this.group = group;
    }
    
    public String getTenant() {
        return tenant;
    }
    
    public void setTenant(String tenant) {
        this.tenant = tenant;
    }
    
    @Override
    public String getModule() {
        return Constants.Config.CONFIG_MODULE;
    }
}
```

与其对应的响应对象就是 ConfigPublishResponse，请看下面代码块。

```
package com.cqfy.nacos.api.config.remote.response;


public class ConfigPublishResponse extends Response {
    
    public ConfigPublishResponse() {
        super();
    }
    
    
    public static ConfigPublishResponse buildSuccessResponse() {
        return new ConfigPublishResponse();
    }
    
   
    public static ConfigPublishResponse buildFailResponse(int errorCode, String errorMsg) {
        ConfigPublishResponse configPublishResponse = new ConfigPublishResponse();
        configPublishResponse.setResultCode(ResponseCode.FAIL.getCode());
        configPublishResponse.setMessage(errorMsg);
        configPublishResponse.setErrorCode(errorCode);
        return configPublishResponse;
    }
}
```

好了，请求对象和响应对象都定义完毕之后，接下来就该真正实现 ClientWorker 类的 publishConfig() 方法了，具体的实现逻辑刚才我们已经分析过了，**就是创建一个 ConfigPublishRequest 请求对象，把配置中心客户端要发布的配置信息封装在请求对象中，然后使用 RpcClient 客户端对象发送给配置中心服务端即可**，请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:客户端工作组件，在以前它使用长轮询机制监听配置文件变更情况，但在2.x版本之后，就不再使用长轮询，而是长连接了
 */
public class ClientWorker implements Closeable {


     //当前客户端工作组件的唯一标识
    private final String uuid = UUID.randomUUID().toString();


    //配置中心客户端
    private final RpcClient agent;



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:构造方法，这个方法其实是伪代码，后面会对其进行重构
     */
    @SuppressWarnings("PMD.ThreadPoolCreationRule")
    public ClientWorker(ServerListManager serverListManager,final NacosClientProperties properties) throws NacosException {

        //在这里创建配置中心客户端
        agent = new RpcClient(properties, serverListManager);
       
        //启动配置中心客户端
        agent.start();


    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端向服务端发布配置信息的方法，这个方法中的encryptedDataKey参数大家可以暂时忽略
     */
    public boolean publishConfig(String dataId, String group, String tenant, String appName, String tag, String betaIps,
                                 String content, String encryptedDataKey, String casMd5, String type) throws NacosException {
        try {
            //创建一个向服务端发送的发布配置文件信息的请求对象
            ConfigPublishRequest request = new ConfigPublishRequest(dataId, group, tenant, content);
            //设置配置文件md5的值，这个值就用来在服务端判断，新发布的配置文件信息是否和旧的配置文件信息不同，不同则意味着配置文件信息更新了
            request.setCasMd5(casMd5);
            //设置额外的标签tag参数
            request.putAdditionalParam(TAG_PARAM, tag);
            //设置应用名称
            request.putAdditionalParam(APP_NAME_PARAM, appName);
            //设置灰度发布的IP地址，如果没有设置，那么发布配置信息的操作将会直接在目标服务器执行
            request.putAdditionalParam(BETAIPS_PARAM, betaIps);
            //设置配置类型
            request.putAdditionalParam(TYPE_PARAM, type);
            //设置密钥，这一点非常重要，大家可能会觉得困惑，但没关系，我马上就为大家解释
            request.putAdditionalParam(ENCRYPTED_DATA_KEY_PARAM, encryptedDataKey == null ? "" : encryptedDataKey);
            //在这里把请求发送给服务端，并且从服务端接收到响应对象
            //这行代码在源码中并不是这样的，这涉及到配置中心客户端对象的重构，下一章我会为大家具体展示
            ConfigPublishResponse response = (ConfigPublishResponse) requestProxy(request);
            //判断响应是否成功
            if (!response.isSuccess()) {
                //响应不成功则记录告警信息
                LOGGER.warn("[{}] [publish-single] fail, dataId={}, group={}, tenant={}, code={}, msg={}",
                        this.getName(), dataId, group, tenant, response.getErrorCode(), response.getMessage());
                return false;
            } else {
                //响应成功的情况
                LOGGER.info("[{}] [publish-single] ok, dataId={}, group={}, tenant={}, config={}", getName(),
                        dataId, group, tenant, ContentUtils.truncateContent(content));
                return true;
            }
        } catch (Exception e) {
            LOGGER.warn("[{}] [publish-single] error, dataId={}, group={}, tenant={}, code={}, msg={}",
                    this.getName(), dataId, group, tenant, "unknown", e.getMessage());
            return false;
        }
    }


    //向服务端发送请求的方法，这个方法其实是伪代码，源码中并不是这样，下一章我就会为大家重构
    private Response requestProxyRequest request) throws NacosException {

        //在这里把请求发送给服务端
        return agent.request(request, 3000);
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端从服务端获取指定配置信息的方法，ConfigResponse这个响应中就封装了从服务端获得的配置信息
     */
    public ConfigResponse getServerConfig(String dataId, String group, String tenant, long readTimeout, boolean notify)
            throws NacosException {
        //该方法暂时不做实现
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:移除指定配置文件的方法
     */
    public boolean removeConfig(String dataId, String group, String tenant, String tag) throws NacosException {
        //该方法暂时不做实现
    }


    //该类的其他内容暂时不做实现

}
```

上面代码块中的逻辑和流程都非常清晰，我就不在为大家重复讲解了。总之现在，我们已经实现了 nacos 配置中心客户端向服务端发布配置信息的功能，也许大家会觉得我实现得非常简单，但我可以很负责任地告诉大家，就算你看了 nacos 配置中心的源码，你会发现它真的就是这么简单，源码中配置中心客户端的内容和我为大家提供的第十四版本代码的内容没什么区别，并且就算你从源码中把 nacos 的整个配置中心客户端看完，你会发现它也就只有那么点的类，内容很少，也非常简单。当然，大家不必怀疑，我肯定没有忘记我在上面代码块的第 65 行展示了这样一行代码，请看下面代码块。

```
request.putAdditionalParam(ENCRYPTED_DATA_KEY_PARAM, encryptedDataKey == null ? "" : encryptedDataKey);
```

并且我还跟大家说，这行代码中的 encryptedDataKey 这个变量肯定会让大家感到疑惑，实际上，这个 encryptedDataKey 变量在之前大家就见过了，当时我说让大家先记住它，我很快就会解释，它本身就是 ClientWorker 类的 publishConfig() 方法中的一个重要参数。那这个参数是什么呢？现在我们就一起揭开它的面纱吧，**它就是 nacos 配置中心要发布到服务端的配置文件的密钥**。实际上，**在配置信息被客户端发布到服务端的时候，会经过严格的保密处理，也就是对配置信息进行加密，与其相关的密钥也会被一起传输给服务端，等需要的时候，就会使用这个密钥来给配置文件解密**。这就是 encryptedDataKey 变量在 nacos 配置中心客户端中的作用。大家肯定很好奇这个 encryptedDataKey 变量是从哪里传递过来的，因为现在大家看到的是，**这个 encryptedDataKey 变量是 ClientWorker 类的 publishConfig() 方法的一个重要参数**，那就很简单了，我们只需要看看 ClientWorker 类的 publishConfig() 方法在哪里被调用了，这就不用分析了，**就是在 NacosConfigService 类的 publishConfigInner() 方法中被调用了**，我把对应的代码又搬运过来了，请看下面代码块。

```
package com.cqfy.nacos.client.config;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:配置中心服客户端服务组件
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosConfigService implements ConfigService {

    //省略该类的其他内容


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:配置中心客户端发布配置文件的方法，这个方法的参数中有一个casMd5字符串，如果客户端在发布配置信息时计算了配置文件的md5
     * 那么配置文件被服务端接收后，就会比较配置文件的md5和之前存储的配置文件的md5是否不一致，如果不一致说明客户端发布的配置文件是更新过的
     * 同时我们也能看到该方法中还有几个参数，比如说betaIps、tag等等，tag是配置信息的标签，是在dataId之下对配置信息更细粒度的划分
     * betaIps就是灰度发布的IP地址，这些内容在nacos主线章节中并不会涉及，等到我为大家更新nacos支线章节时，会为大家实现和这些参数相关的功能
     */
    private boolean publishConfigInner(String tenant, String dataId, String group, String tag, String appName,
                                       String betaIps, String content, String type, String casMd5) throws NacosException {
        //对配置文件所在组名判空，为空则使用默认组名
        group = blank2defaultGroup(group);
        //判断配置文件是否合规
        ParamUtils.checkParam(dataId, group, content);
        //在这里使用客户端工作组件把配置文件发不到服务端
        return worker.publishConfig(dataId, group, tenant, appName, tag, betaIps, content, casMd5, type);
    }

}
```

但在上面的代码块中，也就是在 NacosConfigService 类的 publishConfigInner() 方法中，**大家既没有看到对要发布的配置信息进行加密的操作，因为只有对其进行加密了，才能得到对应的密钥；当然，没有 encryptedDataKey 密钥，更别说把 encryptedDataKey 密钥传入到 ClientWorker 对象的 publishConfig() 方法中了。还是那句话，大家先别急，现在什么都没有，是因为 NacosConfigService 类的 publishConfigInner() 方法肯定要经过重构，重构之后就全都有了，而且在具体重构该方法之前，我还要引入一个非常重要的功能组件，就是配置文件过滤器，这个过滤器就是对配置信息进行加密和解密的**。当然，这些内容在这一章是实现不了了，我就放到下一章为大家实现了，在下一章看完之后，我提供给大家的第十四版本代码，大家就全都可以看了。好了朋友们，我们下一章见！