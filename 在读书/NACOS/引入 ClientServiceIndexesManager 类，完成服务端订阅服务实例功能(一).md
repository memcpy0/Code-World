大家好，上一章我们已经把 nacos 服务端注册服务实例功能完全实现了，想必大家也结合着文章，把我提供的第七版本代码也看完了。这一章，我们将接着往后实现，按顺序来说，也该轮到实现 nacos 服务端订阅服务实例功能了。大家也许还记得我在前几章重构 GrpcRequestAcceptor 单向流请求接收器时为大家引入了三个请求处理器，分别是 HealthCheckRequestHandler、InstanceRequestHandler、SubscribeServiceRequestHandler 这三个请求处理器，随着前几篇文章的完结，现在我们已经把 nacos 服务端的 HealthCheckRequestHandler 健康检测请求处理器和 InstanceRequestHandler 服务实例注册请求处理器实现完毕了，就剩下 SubscribeServiceRequestHandler 请求处理器了，而在 nacos 源码中，这个处理器就是用来处理 nacos 客户端发送过来的订阅服务实例信息请求的。所以，我们接下来要为自己的 nacos 服务端实现订阅服务实例功能的话，肯定也是要围绕着 SubscribeServiceRequestHandler 订阅服务实例信息请求处理器来展开的。好了，话不多说，就让我们开始本章内容吧。

## 引入 Subscriber，在服务端创建订阅者对象

既然我们本章的内容要围绕 SubscribeServiceRequestHandler 订阅服务实例信息请求处理器来展开，那么接下来我想先请大家回顾一下，之前我为大家定义的 SubscribeServiceRequestHandler 请求处理器是什么样的，我把对应的代码搬运到这里了，请看下面代码块。

```
package com.cqfy.nacos.naming.remote.rpc.handler;




@Component
public class SubscribeServiceRequestHandler extends RequestHandler<SubscribeServiceRequest, SubscribeServiceResponse> {


     /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/20
    * @方法描述：核心处理方法
    */
    @Override
    public SubscribeServiceResponse handle(SubscribeServiceRequest request, RequestMeta meta) throws NacosException {

        //该方法暂时不做实现
    }
    
}
```

可以看到，这个 SubscribeServiceRequestHandler 请求处理器中只有一个核心的 handle() 方法，该方法目前没有实现，在这个方法中可以得到 nacos 客户端发送过来的封装了订阅者信息的 SubscribeServiceRequest 请求对象。**所谓订阅者信息就是 nacos 客户端要订阅的服务实例所在服务的信息，比如服务名称，服务所在的命名空间等等**。只有把这些信息收集起来，nacos 服务端才知道发送 SubscribeServiceRequest 请求过来的客户端想要订阅的服务实例信息是什么，然后才能在服务实例更新之后，把最新的服务实例信息主动推送给客户端。

如果是这样的话，那么我们为 nacos 服务端实现服务实例信息订阅功能也就简单多了，**反正只要让 nacos 服务端知道每一个执行了订阅服务实例操作的客户端要订阅的服务实例信息是哪些就行了**，**那我们就可以在 nacos 服务端定义一个新的对象，比如说就是 Subscriber 对象，这个对象内部封装了客户端要订阅的服务实所在服务的信息**，**当然还有执行订阅服务实例信息操作的 nacos 客户端本身的信息**，我们可以把它称为订阅者对象，然后把 Subscriber 订阅者对象存放在 nacos 服务端即可。Subscriber 的代码我也定义好了，请看下面代码块。

```
package com.cqfy.nacos.naming.pojo;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：封装客户端订阅者信息的对象，在第七版本代码中还用不到
 */
public class Subscriber implements Serializable {

    private static final long serialVersionUID = -6256968317172033867L;

    //客户端网络地址
    private String addrStr;

    //订阅服务实例信息的客户端版本好
    private String agent;

    //客户端应用名称
    private String app;

    //客户端IP地址
    private String ip;

    //客户端端口号
    private int port;

    //客户端要订阅的服务实例所在的命名空间
    private String namespaceId;

    //客户端要定义的服务实例的名称
    private String serviceName;

    //客户端要定于的服务实例所在集群
    private String cluster;



    //以下都是对应的get、set方法，就不在文章中展示了
}
```

好了，以上代码块展示的就是服务端为执行了订阅操作的客户端创建的订阅者对象。**现在订阅者对象定义好了，那该对象应该如何创建，以及如何在 nacos 服务端存储呢**？现在我们可能对此没什么头绪，但有一点我相信大家都很清楚，那就是当 nacos 客户端将一个服务实例注册到服务端的时候，会把这个服务实例所在的服务信息封装到一个 Service 对象中，然后把 Service 对象和真正的服务实例信息以键值对的方式存放到 nacos 客户端在服务端对应的 ConnectionBasedClient 客户端对象中。这是前几章的内容，我相信大家都还有印象，现在轮到客户端订阅服务实例信息了，并且我们已经把客户端的 Subscriber 订阅者对象定义完毕了，这个订阅者对象封装的就是执行了订阅操作的 nacos 客户端的信息，以及要订阅的服务实例的部分信息。**如果我们想要让 nacos 服务端清楚地知道每一个订阅者，也就是每一个客户端订阅了哪些服务信息，最直接的方式不就是仿照 nacos 服务端缓存客户端注册过来的服务实例信息那样，也定义一个 map，然后把 map 的 key 定义为一个 Serivce 对象，Serivce 对象就封装了 nacos 客户端要订阅的服务的信息，而 value 就是真正的 Subscriber 订阅者对象。这样一来，通过这个 map，nacos 服务端就知道每一个客户端订阅了哪个服务实例信息了**，就像下面代码块展示的这样，请看下面代码块。

```

protected final ConcurrentHashMap<Service, Subscriber> subscribers = new ConcurrentHashMap<>();
```

以上代码块展示的就是我们要为 Subscriber 订阅者对象定义的 map。map 的 key 是一个 Service 对象，而 Service 对象的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.pojo;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/16
 * @方法描述：封装客户端注册的服务实例信息的对象，其实就是服务实例元数据对象
 */
public class Service implements Serializable {

    private static final long serialVersionUID = -990509089519499344L;

    //服务实例的命名空间
    private final String namespace;

    //服务实例所在的分组名称
    private final String group;

    //服务实例提供的服务名称
    private final String name;

    //该服务实例是否为临时服务实例
    private final boolean ephemeral;


    //其他内容省略
}
```

可以看到，一个 Service 对象中就封装着服务实例所在的服务的信息，这个对象当然可以用来封装客户端订阅者要定于的服务实例所在服务的信息，而我们定义的 map 的 value 就是客户端订阅者本身，这样一来，大家就清楚 nacos 服务端是以怎么的方式维护客户端订阅者的了吧？如果这个逻辑大家理解了，那么接下来请大家想一想，nacos 客户端也许要订阅多个服务实例信息，并且多个服务实例信息可能属于不同的服务，也就是多个服务实例所属的 Service 信息并不相同，这样一来，**那么多个 nacos 客户端都向服务端订阅了多个不同服务的服务实例信息，这时候 nacos 服务端该怎么缓存客户端订阅者的信息呢**？问题看起来似乎有些复杂，但其实没什么可分析的了，在前几章我们不是已经让 nacos 服务端为每一个建立了连接的客户端都创建了一个 ConnectionBasedClient 客户端对象吗？那就让每一个 ConnectionBasedClient 客户端对象维护与自己对应的 nacos 客户端的信息呀，**我们只需要把刚才定义的 map 定义到 ConnectionBasedClient 客户端对象中，这样一来，只要 nacos 客户端向服务端执行了订阅服务实例的操作，nacos 服务端这边就可以把客户端定于的服务实例所在的服务信息，也就是 Service 对象，以及 Subscriber 客户端订阅者对象存放到 ConnectionBasedClient 客户端对象自己的 map 中**。我把 ConnectionBasedClient 类稍微重构了一下，给大家展示在下面了，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client.impl;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：客户端连接对象，服务器会为每一个接收到的客户端连接创建客户端连接对象，该客户端向服务器注册的服务实例，订阅的服务实例信息的订阅者信息，都会存放到它自己的客户端对象中
 */
public class ConnectionBasedClient {


    //客户端连接Id
    private final String connectionId;

    //大家肯定都看过第七版本代码了，知道以下两个成员变量其实是定义在本类的父类，也就是AbstractClient类中

    //客户端存放到注册到服务端的服务实例信息的map，key就是服务实例元信息，value就是对应的服务实例信息
    protected  final ConcurrentHashMap<Service, InstancePublishInfo> publishers = new ConcurrentHashMap<>(16, 0.75f, 1);


    //客户端订阅了服务器的服务实例信息时，服务端会把订阅者信息存放到客户端自己的map中
    protected final ConcurrentHashMap<Service, Subscriber> subscribers = new ConcurrentHashMap<>(16, 0.75f, 1);

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



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：从客户端对象移除服务实例的方法
     */
    @Override
    public InstancePublishInfo removeServiceInstance(Service service) {

        InstancePublishInfo result = publishers.remove(service);
        if (null != result) {
            if (result instanceof BatchInstancePublishInfo) {
                MetricsMonitor.decrementIpCountWithBatchRegister(result);
            } else {
                MetricsMonitor.decrementInstanceCount();
            }
            NotifyCenter.publishEvent(new ClientEvent.ClientChangedEvent(this));
        }
        Loggers.SRV_LOG.info("Client remove for service {}, {}", service, getClientId());
        return result;
    }


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
```

以上代码块展示的就是重构之后的 ConnectionBasedClient 客户端对象，现在大家应该已经清楚了 nacos 服务端是怎么维护订阅了服务实例的客户端的信息的。**只要我们调用 ConnectionBasedClient 客户端对象的 addServiceSubscriber() 方法，就可以把客户端订阅者订阅的服务信息以及订阅者本身，以键值对的方式存放到客户端对象的 subscribers 成员变量中**。所以现在问题就成了 addServiceSubscriber() 方法会在什么时候被调用，这个也很简单，上一章我们不是还引入了一个 EphemeralClientOperationServiceImpl 组件吗？它就是客户端服务组件，如果 nacos 客户端向服务端注册了临时服务实例信息，就是通过这个客户端服务组件，也就是调用 EphemeralClientOperationServiceImpl 对象的 registerInstance() 方法，把客户端注册过来的服务实例信息存放到 ConnectionBasedClient 客户端对象中。**现在要把 Subscriber 客户端订阅对象存放到 ConnectionBasedClient 客户端对象中，依然使用这个 EphemeralClientOperationServiceImpl 提供了客户端服务的组件即可**。所以，我也把 EphemeralClientOperationServiceImpl 类简单重构了一下，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.service.impl;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：客户端服务组件，这个类的对象专门用来和客户端执行的操作交互，比如注册服务实例信息，订阅服务实例信息，注销服务实例信息等等，都是这个类的对象执行的
 */
@Component("ephemeralClientOperationService")
public class EphemeralClientOperationServiceImpl implements ClientOperationService {

    //客户端管理器，这个管理器持有者服务器创建的所有客户端对象
    private final ClientManager clientManager;

    //构造方法
    public EphemeralClientOperationServiceImpl(ClientManagerDelegate clientManager) {
        this.clientManager = clientManager;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：注册服务实例到客户端对象的方法
     */
    @Override
    public void registerInstance(Service service, Instance instance, String clientId) throws NacosException {
        //对客户端注册的服务实例做判断，校验是否合规
        NamingUtils.checkInstanceIsLegal(instance);

        //从服务实例元数据管理器中获得服务实例元数据对象
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        if (!singleton.isEphemeral()) {
            //如果不是临时服务实例则抛出异常
            throw new NacosRuntimeException(NacosException.INVALID_PARAM, String.format("Current service %s is persistent service, can't register ephemeral instance.", singleton.getGroupedServiceName()));
        }
        //从客户端管理器中得到对应的客户端对象
        Client client = clientManager.getClient(clientId);
        //校验客户端是否合法
        if (!clientIsLegal(client, clientId)) {
            return;
        }
        //把服务实例信息包装成InstancePublishInfo对象，这个是注册中心服务端用来封装服务实例信息的对象
        InstancePublishInfo instanceInfo = getPublishInfo(instance);
        //把服务实例注册到客户端对象中
        client.addServiceInstance(singleton, instanceInfo);
        //更新客户端的最新活跃时间
        client.setLastUpdatedTime();
        //更新客户端状态变更次数
        client.recalculateRevision();


        //下面发布了一个事件，这个操作非常重要，这里大家可以暂时记住这个事件，下一章我们就会围绕这个事件对一系列功能展开实现
        
        //事件通知中心发布客户端注册服务信息事件
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientRegisterServiceEvent(singleton, clientId));
        
    }



    //客户端订阅服务实例信息的方法
    @Override
    public void subscribeService(Service service, Subscriber subscriber, String clientId) {
        Service singleton = ServiceManager.getInstance().getSingletonIfExist(service).orElse(service);
        Client client = clientManager.getClient(clientId);
        if (!clientIsLegal(client, clientId)) {
            return;
        }//在这里调用了客户端对象的addServiceSubscriber()方法，把客户端订阅对象添加到客户端map中了
        client.addServiceSubscriber(singleton, subscriber);
        client.setLastUpdatedTime();
        //在这里发布了客户端订阅事件，这个事件非常重要，大家先记住这个事件即可
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientSubscribeServiceEvent(singleton, clientId));
    }

    
    //以下就是批量注册服务实例信息，注销服务实例信息，取消订阅服务实例信息的方法，这几个方法在第七版本代码中还用不到，所以我就先不为大家展示了

    
}
```

好了，现在重构之后的 EphemeralClientOperationServiceImpl 类也展示完毕了，我知道现在肯定有朋友会问，那么 EphemeralClientOperationServiceImpl 类的 subscribeService() 方法什么时候被调用呢？只有调用了这个方法，nacos 客户端要订阅的服务的信息以及客户端订阅对象才会被存放到与 nacos 客户端对应的 ConnectionBasedClient 客户端对象中。这时候就要回到 SubscribeServiceRequestHandler 服务订阅请求处理器中了，接下来也不用我过多分析和讲解，我只需要把重构之后的 SubscribeServiceRequestHandler 请求处理器展示给大家，大家就全都明白了，请看下面代码块。

```
package com.cqfy.nacos.naming.remote.rpc.handler;




@Component
public class SubscribeServiceRequestHandler extends RequestHandler<SubscribeServiceRequest, SubscribeServiceResponse> {



    //客户端服务组件，这个成员变量会被构造方法自动注入
    private final EphemeralClientOperationServiceImpl clientOperationService;


    public SubscribeServiceRequestHandler(EphemeralClientOperationServiceImpl clientOperationService) {
        this.clientOperationService = clientOperationService;
    }

    
    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/20
    * @方法描述：核心处理方法
    */
    @Override
    public SubscribeServiceResponse handle(SubscribeServiceRequest request, RequestMeta meta) throws NacosException {
        
        //得到要订阅的服务的命名空间
        String namespaceId = request.getNamespace();
        //得到服务名称
        String serviceName = request.getServiceName();
        //得到服务所在组名
        String groupName = request.getGroupName();
        //得到客户端应用名称
        String app = request.getHeader("app", "unknown");
        //得到客户端订阅的服务的组合键
        String groupedServiceName = NamingUtils.getGroupedName(serviceName, groupName);
        //得到服务实例元信息对象
        Service service = Service.newService(namespaceId, groupName, serviceName, true);
        
        //在这里创建了订阅者对象，因为一旦订阅了服务端的服务实例的信息，之后服务实例有任何变动都会通知订阅者，所以服务端肯定要保存订阅者的信息
        Subscriber subscriber = new Subscriber(meta.getClientIp(), meta.getClientVersion(), app, meta.getClientIp(),
                namespaceId, groupedServiceName, 0, request.getClusters());
        
        //判断是订阅操作还是取消订阅操作
        if (request.isSubscribe()) {
            //在这里执行订阅操作
            clientOperationService.subscribeService(service, subscriber, meta.getConnectionId());
        } else {
            //执行取消订阅的操作
            clientOperationService.unsubscribeService(service, subscriber, meta.getConnectionId());
        }
        
        //回复给客户端订阅成功响应
        return new SubscribeServiceResponse(ResponseCode.SUCCESS.getCode(), "success");
    }
    
}
```

到此为止，大家应该已经知道了一个 nacos 客户端向服务端订阅了某个服务实例信息后，nacos 服务端会执行什么操作。**只要 nacos 客户端向服务端执行了订阅服务实例的操作，那么 nacos 服务端就会首先在 SubscribeServiceRequestHandler 服务订阅请求处理器中得到客户端要订阅的服务的信息，也就是得到一个 Service 对象，然后把订阅的客户端的信息封装到一个 Subscriber 对象中，最后调用 EphemeralClientOperationServiceImpl 客户端服务组件的 subscribeService() 方法，把 nacos 客户端要订阅的服务信息和订阅者对象存放到与 nacos 客户端对应的 ConnectionBasedClient 对象中**。当然，执行完这一切操作之后，最后还回复了 nacos 客户端一个 SubscribeServiceResponse 成功响应。对大家来说，我想这个逻辑应该是很简单的，有了之前 nacos 服务端存放客户端注册服务实例信息的功能为例子，大家应该很容易就能梳理清楚 nacos 服务端订阅服务实例功能的全流程。

好了，现在我们就把 nacos 服务端订阅服务实例功能也实现完毕了，**假如有多个 nacos 客户端都向服务端订阅了不同的服务实例信息，那么 nacos 服务端就会把这些客户端订阅的服务实例所在服务的信息，以及客户端订阅对象都存放到与各个客户端对应的 ConnectionBasedClient 对象中。也就是说，让 nacos 服务端为每一个客户端连接创建的 ConnectionBasedClient 对象来维护各个客户端的信息**。这个流程想必大家非常清楚了，但是，这并不意味着我们就把 nacos 服务端订阅服务实例的功能彻底实现了。

我这么说倒不是因为 nacos 服务端根据客户端订阅信息向客户端推送最新服务实例信息的功能还没实现，这个功能在后面章节很快就会被实现了。我现在关注的是另一个问题，实际上，我在刚才为大家展示的 SubscribeServiceRequestHandler 请求处理器并不是最终版本。在 nacos 源码中，如果一个 nacos 客户端向服务端订阅了某个服务下的服务实例信息，在服务端的 SubscribeServiceRequestHandler 请求处理器中，**不仅要把客户端订阅的服务实例所在服务的信息，以及客户端订阅对象都存放到与该客户端对应的 ConnectionBasedClient 对象中，还要在服务端把该客户端订阅的服务实例信息查询出来，然后返回给客户端**。也就是说，在 nacos 源码中，这个 SubscribeServiceRequestHandler 请求处理器应该定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.naming.remote.rpc.handler;




@Component
public class SubscribeServiceRequestHandler extends RequestHandler<SubscribeServiceRequest, SubscribeServiceResponse> {



    //客户端服务组件，这个成员变量会被构造方法自动注入
    private final EphemeralClientOperationServiceImpl clientOperationService;


    public SubscribeServiceRequestHandler(EphemeralClientOperationServiceImpl clientOperationService) {
        this.clientOperationService = clientOperationService;
    }

    
    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/20
    * @方法描述：核心处理方法
    */
    @Override
    public SubscribeServiceResponse handle(SubscribeServiceRequest request, RequestMeta meta) throws NacosException {
        
        //得到要订阅的服务的命名空间
        String namespaceId = request.getNamespace();
        //得到服务名称
        String serviceName = request.getServiceName();
        //得到服务所在组名
        String groupName = request.getGroupName();
        //得到客户端应用名称
        String app = request.getHeader("app", "unknown");
        //得到客户端订阅的服务的组合键
        String groupedServiceName = NamingUtils.getGroupedName(serviceName, groupName);
        //得到服务实例元信息对象
        Service service = Service.newService(namespaceId, groupName, serviceName, true);
        
        //在这里创建了订阅者对象，因为一旦订阅了服务端的服务实例的信息，之后服务实例有任何变动都会通知订阅者，所以服务端肯定要保存订阅者的信息
        Subscriber subscriber = new Subscriber(meta.getClientIp(), meta.getClientVersion(), app, meta.getClientIp(),
                namespaceId, groupedServiceName, 0, request.getClusters());



        //在这里得到了客户端订阅的服务实例的信息，这里写成了伪代码
        //这个ServiceInfo对象在实现nacos客户端的时候，大家应该就见过了，这个ServiceInfo对象封装了客户端订阅的服务下的所有服务实例的信息
        ServiceInfo serviceInfo = ServiceUtil.selectInstancesWithHealthyProtection(service);
        

        
        //判断是订阅操作还是取消订阅操作
        if (request.isSubscribe()) {
            //在这里执行订阅操作
            clientOperationService.subscribeService(service, subscriber, meta.getConnectionId());
        } else {
            //执行取消订阅的操作
            clientOperationService.unsubscribeService(service, subscriber, meta.getConnectionId());
        }
        
        //客户端只要发送订阅请求，服务端就会直接把客户端订阅的服务实例信息回复给客户端
        return new SubscribeServiceResponse(ResponseCode.SUCCESS.getCode(), "success", serviceInfo);
    }
    
}
```

在上面代码块的第 50 行，可以看到执行了一行伪代码，从 nacos 服务端查询出了客户端要订阅的服务下的所有服务实例信息，然后回复给客户端了。这显然就是 nacos 源码中的一个机制，nacos 源码的服务端在处理客户端发送过来的 SubscribeServiceRequest 订阅服务实例信息的请求时，除了会把订阅者对象存放到与客户端对应的 ConnectionBasedClient 对象中，还会立即为客户端查询一下其订阅的所有服务实例信息，然后返回给客户端。既然我们现在就是在仿写 nacos，从功能上来说，当然要和源码一致。所以，接下来，我还要为大家实现这个功能：**也就是 nacos 客户端根据客户端订阅的服务，查询该服务下所有服务实例信息，然后返回给客户端**。那么这个功能应该怎么实现呢？

## 引入 ClientServiceIndexesManager

请大家思考一下，在真实的生产环境中，我们也许会启动多个 nacos 客户端，并且这些客户端都会访问 nacos 服务端，这样就意味着 nacos 服务端会为这些客户端都创建对应的 ConnectionBasedClient 对象，而这些 nacos 客户端向服务端注册的服务实例信息都会存放在各自对应的 ConnectionBasedClient 客户端对象中。我想让大家思考的是，启动的多个 nacos 客户端肯定会向服务端注册服务实例信息，也许这几个客户端注册的服务实例各不相同，但也许其中几个客户端注册的服务实例是相同的，服务实例所属的服务信息是一样的，提供的服务也是一样的。就比如说，现在有 nacos 客户端 1、2、3、4、5 这 5 个客户端，其中客户端 1、3、5 向服务端注册的服务实例都是一些样的。那么，这 5 个客户端向服务端注册的服务实例会以下面的方式存放在 nacos 服务端内部。请看下面代码块。

```
//nacos服务端会为每一个客户端创建对应的ConnectionBasedClient对象
//客户端注册的服务实例信息都会存放到与自己对应的ConnectionBasedClient客户端对象中
客户端1对应的ConnectionBasedClient对象：Service——InstancePublishInfo
客户端2对应的ConnectionBasedClient对象：Service2——InstancePublishInfo
客户端3对应的ConnectionBasedClient对象：Service——InstancePublishInfo
客户端4对应的ConnectionBasedClient对象：Service4——InstancePublishInfo
客户端5对应的ConnectionBasedClient对象：Service——InstancePublishInfo
```

从以上代码块中可以看到，当 nacos 客户端 1、3、5 向服务端注册了相同的服务实例信息，它们注册的服务实例信息存放到自己对应的 ConnectionBasedClient 对象中时会使用相同的 key，也就是相同的 Service 对象，因为服务实例所在服务的信息是一样的，当然，对应的 value，也就是封装了真正的服务实例信息的 InstancePublishInfo 对象，其内部封装的服务实例信息也是相同的。这一点想必大家都能理解，也没什么值得啰嗦的地方。

但是大家别忘了，我们现在要实现的是 nacos 服务端订阅服务功能，准确的说是要在 nacos 服务端，根据某个客户端订阅的服务信息，把该服务下所有的服务实例信息查找出来，然后回复给 nacos 客户端。如果大家还能回忆起我们之前实现的 nacos 客户端订阅服务实例功能，就会知道在 nacos 源码中，确实存在这个功能。我把 nacos 客户端订阅服务实例的相关代码搬运到这里了，帮助大家回顾一下，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：代理客户端，这个客户端内部持有了grpcClientProxy客户端，真正发送服务实例给服务端的操作，是grpcClientProxy客户端执行的
 */
public class NamingClientProxyDelegate implements NamingClientProxy {




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：订阅服务信息的方法
     */
    @Override
    public ServiceInfo subscribe(String serviceName, String groupName, String clusters) throws NacosException {
        NAMING_LOGGER.info("[SUBSCRIBE-SERVICE] service:{}, group:{}, clusters:{} ", serviceName, groupName, clusters);
        //将服务名称和组名合并
        String serviceNameWithGroup = NamingUtils.getGroupedName(serviceName, groupName);
        //根据服务名称和组名和集群名称得到一个key
        String serviceKey = ServiceInfo.getKey(serviceNameWithGroup, clusters);
        //在这里客户端还要实时主动关注服务端的服务实例信息是否发生了变化
        serviceInfoUpdateService.scheduleUpdateIfAbsent(serviceName, groupName, clusters);
        //根据key从serviceInfoHolder中得到对应的服务实例信息，这里可以看到，优先去服务实例保存器中获得了一次
        ServiceInfo result = serviceInfoHolder.getServiceInfoMap().get(serviceKey);
        
        //以下就是向服务端订阅服务实例信息的操作
        
        //如果获取不到，或者判断客户端并没有订阅对应服务
        if (null == result || !isSubscribed(serviceName, groupName, clusters)) {
            //在这里就真正订阅了该服务，并且从服务端得到了返回的服务实例信息
            result = grpcClientProxy.subscribe(serviceName, groupName, clusters);
        }
        
        
        //把得到的服务实例信息交给服务实例保存器保管
        serviceInfoHolder.processServiceInfo(result);
        return result;
    }

    //省略其他内容
}









package com.cqfy.nacos.client.naming.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：客户端订阅服务实例的方法
     */
    @Override
    public ServiceInfo subscribe(String serviceName, String groupName, String clusters) throws NacosException {
        if (NAMING_LOGGER.isDebugEnabled()) {
            NAMING_LOGGER.debug("[GRPC-SUBSCRIBE] service:{}, group:{}, cluster:{} ", serviceName, groupName, clusters);
        }//在开始执行定于操作之前，创建一个订阅操作的重做对象
        redoService.cacheSubscriberForRedo(serviceName, groupName, clusters);
        return doSubscribe(serviceName, groupName, clusters);
    }

    //在这个方法中，向服务端发送了订阅服务实例的请求
    public ServiceInfo doSubscribe(String serviceName, String groupName, String clusters) throws NacosException {
        SubscribeServiceRequest request = new SubscribeServiceRequest(namespaceId, groupName, serviceName, clusters,
                true);
        SubscribeServiceResponse response = requestToServer(request, SubscribeServiceResponse.class);
        //程序执行到这里就意味着订阅服务的操作成功了，就要把订阅操作的操作重做对象的registered标志设置为true
        redoService.subscriberRegistered(serviceName, groupName, clusters);
        //返回得到的服务实例信息
        return response.getServiceInfo();
    }



    //其他内容省略
}
```

好了，现在 nacos 客户端对应的代码看完了，现在我们也该重新回到 nacos 服务端，实现 nacos 服务端根据某个客户端订阅的服务信息，把该服务下所有的服务实例信息查找出来，然后回复给 nacos 客户端功能了。而根据我们之前的分析，我们已经知道了，假如在生产环境中启动了多个 nacos 客户端，并且 1、3、5 客户端向服务端注册了相同的服务信息，**如果现在有一个客户端 6 向服务端订阅了某个服务，并且订阅的恰好就是 nacos 客户端 1、3、5 注册的服务实例信息，这样一来，nacos 服务端肯定需要把客户端 1、3、5 注册的服务实例信息查询出来，然后回复给客户端 6**。这个逻辑大家应该都能理解吧？肯定不可能只返回某个客户端注册的服务实例信息，要返回就会把客户端 6 订阅的服务之下的所有服务实例信息都返回给客户端 6，这样才方便消费者使用具体的负载均衡策略，远程调用服务实例提供的目标方法。那请大家想一想，**现在有 5 个客户端都向 nacos 服务端注册了服务实例信息，我们自己知道客户端 1、3、5 注册的服务实例信息就是客户端 6 订阅的服务信息，但 nacos 服务端怎么知道客户端 6 订阅的服务实例信息就是客户端 1、3、5 注册的呢？怎么才能把这些服务实例信息找出来，然后封装到 ServiceInfo 对象中回复给客户端 6 呢**？

这时候就要回到 ConnectionBasedClient 客户端对象中来查找问题的解决方法了，我们都知道 ConnectionBasedClient 客户端对象是 nacos 服务端为每一个 nacos 客户端创建的对象，并且每一个 nacos 客户端执行的操作信息都会存放到服务端与自己对应的 ConnectionBasedClient 客户端对象中。nacos 客户端向服务端注册了服务实例信息，或者是向服务端订阅了服务实例信息，注册服务实例信息以及订阅者信息都会存放到对应的 ConnectionBasedClient 客户端对象中，就像下面展示的这样，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client.impl;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：客户端连接对象，服务器会为每一个接收到的客户端连接创建客户端连接对象，该客户端向服务器注册的服务实例，订阅的服务实例信息的订阅者信息，都会存放到它自己的客户端对象中
 */
public class ConnectionBasedClient {


    //客户端连接Id
    private final String connectionId;

    //大家肯定都看过第七版本代码了，知道以下两个成员变量其实是定义在本类的父类，也就是AbstractClient类中

    //客户端存放到注册到服务端的服务实例信息的map，key就是服务实例元信息，value就是对应的服务实例信息
    protected  final ConcurrentHashMap<Service, InstancePublishInfo> publishers = new ConcurrentHashMap<>(16, 0.75f, 1);


    //客户端订阅了服务器的服务实例信息时，服务端会把订阅者信息存放到客户端自己的map中
    protected final ConcurrentHashMap<Service, Subscriber> subscribers = new ConcurrentHashMap<>(16, 0.75f, 1);


    @Override
    public InstancePublishInfo getInstancePublishInfo(Service service) {
        return publishers.get(service);
    }
    

    //其他内容省略
}
```

可以看到，在上面展示的 ConnectionBasedClient 客户端对象中，存放了 nacos 客户端注册的服务实例信息，以及要订阅的服务实例信息，并且都是以键值对的方式存储的，**其中键值对的键，也就是 key 就是服务实例所在服务的信息**。看到这里，其实我已经有了一个想法，那就是在 nacos 服务端中，我们肯定可以知道客户端 6 究竟订阅了哪个服务下的服务实例信息，也就是客户端 6 订阅的服务实例所在服务的信息是可以确定的，也就意味着 ConnectionBasedClient 客户端对象的 subscribers 这个成员变量中的键，也就是 Service 是可以确定的。这就好办了，我可以直接去 1、2、3、4、5 这 5 个 nacos 客户端分别在服务端对应的 ConnectionBasedClient 客户端对象中查看其内部的 publishers 成员变量，这个成员变量存放着 nacos 客户端注册到服务端的服务实例的键值对信息，**只要我判断这些 ConnectionBasedClient 客户端对象的 publishers 成员变量中是否包含客户端 6 订阅的 Service 这个 key，不就可以知道在 1、2、3、4、5 这几个 nacos 客户端中，哪些客户端注册了 Service 服务下的服务实例信息了吗**？然后把这些服务实例信息找出来，一起返回给客户端 6 就行了。这就很简单了，**直接调用 ConnectionBasedClient 客户端对象的 getInstancePublishInfo(Service service) 方法，即可找到与该客户端对象对应的 nacos 客户端注册的 Service 服务下的服务实例**。

那么现在问题来了，那就是 nacos 服务端怎么得到客户端 1、2、3、4、5 在服务端对应的 ConnectionBasedClient 客户端对象呢？目前就有一个最笨的方法，或者说最直接的方法：**ClientManager 这个客户端管理器不是存放着 nacos 服务端为所有客户端创建的 ConnectionBasedClient 对象吗？这些 ConnectionBasedClient 客户端对象就存放在 ClientManager 客户端管理器内部的 clients 这个 map 成员变量中，那我直接遍历这个 map，也就可以得到每一个 ConnectionBasedClient 客户端对象，然后再调用客户端对象的 getInstancePublishInfo(Service service) 方法，就可以得到与该客户端对象对应的 nacos 客户端注册的 Service 服务下的服务实例。当然，如果 nacos 客户端并没有注册对应的服务实例信息，那得到的就是 null**。这个流程大家肯定都能理解，我相信就算我不特意讲解，大家也能想到这种方法获得客户端 6 订阅的服务下的所有服务实例信息。但是，这么做有一个缺点，那就是需要进行一些多余的查找操作，**当我们遍历客户端管理器的 clients 这个成员变量时，肯定会查询每一个 ConnectionBasedClient 客户端对象是否注册了 Service 服务下的服务实例信息，如果没有注册，得到的就是 null**。假如现在启动的不是 5 个 nacos 客户端，而是 10 个 nacos 客户端，并且只有一个客户端注册了客户端 6 订阅的服务实例信息，那么其他的几次查找就都是多余的了。我认为这是程序的一个小瑕疵，**那么有没有什么方法，能够让 nacos 服务端精确地知道究竟是哪几个客户端注册了客户端 6 订阅的 Service 下的服务实例呢**？

现在大家应该可以意识到了，**其实只需要 nacos 服务端精确地知道哪些客户端注册了客户端 6 订阅的 Service 服务下的服务实例即可，这就是问题的核心，只要解决了这个，就不需要再大费周章地遍历所有的 ConnectionBasedClient 客户端对象了**。那 nacos 服务端怎么就能知道呢？**要是有一个新的 map，这个 map 中存放的是服务信息与注册的客户端的映射关系就好了**。这个 map 就可以先定义成下面这样，请看下面代码块。

```
 ConcurrentHashMap<服务信息, 客户端信息> map = new ConcurrentHashMap<>();
```

上面代码块展示的就是我最新定义的 map，可以看到在上面这个 map 中，map 的 key 存放的就是服务信息，而服务信息其实就是 Service 对象，map 的 value 就是客户端信息，这个也很好说了，客户端信息完全可以用客户端连接 Id 来表示。所以上面这个 map 就可以重构成下面这样，请看下面代码块。

```
//key就是服务实例所在服务的信息，value就是客户端连接Id
ConcurrentHashMap<Service, String> map = new ConcurrentHashMap<>();
```

现在大家应该知道我定义这个 map 的作用了吧？通过这个 map，我们就可以准确地知道 nacos 的客户端都注册了什么服务实例到服务端。我们可以直接根据服务信息，也就是 Service 对象找到注册了该服务下的服务实例信息的客户端。当然，我们还要考虑到一种情况，就像我们之前举的例子，1、2、3、4、5 这 5 个 nacos 客户端都向服务端注册了服务实例信息，其中客户端 1、3、5 注册的服务实例都是相同的，也就是这几个客户端注册的服务实例都是在同一个 Service 服务下，这时候再用上面的 map 存放客户端注册服务实例信息显然就不合适了，**因为上面 map 的键值对的 value 只能存放一个客户端连接 Id，但现在有三个客户端注册了相同的服务实例信息，显然就该用一个集合来存放注册了相同服务实例的客户端 Id**。所以上面这个 map 就可以重构成下面这样，请看下面代码块。

```
//key就是服务实例所在服务的信息，value就是客户端连接Id的集合
ConcurrentHashMap<Service, Set<String>> map = new ConcurrentHashMap<>();
```

到此为止，这个 map 就定义完毕了。现在逻辑就很清晰了：**有了这个 map 之后，只要有 nacos 客户端订阅了某个服务下的服务实例信息，那么 nacos 服务端就可以使用封装了客户端订阅的服务的信息，也就是 Service 对象，到上面这个 map 中寻找对应的 value，如果存在对应的 value，这时候 nacos 服务端就可以准确地知道哪几个客户端注册了该服务下的服务实例信息，然后从这个客户端对应的 ConnectionBasedClient 对象中，找到客户端注册在 Service 服务下的服务实例信息**。大家可以仔细品味品味这个逻辑，这个逻辑理解之后，接下来就跟着我一起来分析一个新的问题，**那就是怎么向我们刚刚定义好的这个 map 中存放数据**？这是目前最关键的问题。

只有这个 map 中存放了数据，nacos 服务端才能根据 Service 查找到对应的客户端连接 Id 的集合。我们刚才也看到了，**这个 map 中存放的就是代表服务信息的 Service 对象与向该服务下注册了服务实例的客户端的连接 Id 集合的键值对**。也就是说，**当一个 nacos 客户端向服务端注册了服务实例信息时，除了把客户端注册的服务实例信息存放到与客户端对应的 ConnectionBasedClient 对象中，还要把客户端注册的服务实例所在服务的信息，以及客户端的连接 Id 存放到我们刚定义好的这个 map 中**。向我们刚定义好的 map 中存放数据的操作肯定是在 nacos 客户端向服务端注册服务实例时执行的，**那么 nacos 服务端怎么就知道有客户端向自己注册了服务实例呢**？这时候就要回顾一下之前的知识了，也许大家还记得上一章实现的 EphemeralClientOperationServiceImpl 这个组件，如果大家忘记了这个组件的作用，可以再去上一章回顾一下，我就不再重复解释这个组件的作用了。总之，**在这个组件中有一个非常重要的方法，那就是 registerInstance() 方法，在该方法中会把 nacos 客户端注册的服务实例信息存放到与客户端对应的 ConnectionBasedClient 对象中**，具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.service.impl;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：客户端服务组件，这个类的对象专门用来和客户端执行的操作交互，比如注册服务实例信息，订阅服务实例信息，注销服务实例信息等等，都是这个类的对象执行的
 */
@Component("ephemeralClientOperationService")
public class EphemeralClientOperationServiceImpl implements ClientOperationService {

    //客户端管理器，这个管理器持有者服务器创建的所有客户端对象
    private final ClientManager clientManager;

    //构造方法
    public EphemeralClientOperationServiceImpl(ClientManagerDelegate clientManager) {
        this.clientManager = clientManager;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：注册服务实例到客户端对象的方法
     */
    @Override
    public void registerInstance(Service service, Instance instance, String clientId) throws NacosException {
        //对客户端注册的服务实例做判断，校验是否合规
        NamingUtils.checkInstanceIsLegal(instance);

        //从服务实例元数据管理器中获得服务实例元数据对象
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        if (!singleton.isEphemeral()) {
            //如果不是临时服务实例则抛出异常
            throw new NacosRuntimeException(NacosException.INVALID_PARAM, String.format("Current service %s is persistent service, can't register ephemeral instance.", singleton.getGroupedServiceName()));
        }
        //从客户端管理器中得到对应的客户端对象
        Client client = clientManager.getClient(clientId);
        //校验客户端是否合法
        if (!clientIsLegal(client, clientId)) {
            return;
        }
        //把服务实例信息包装成InstancePublishInfo对象，这个是注册中心服务端用来封装服务实例信息的对象
        InstancePublishInfo instanceInfo = getPublishInfo(instance);
        //把服务实例注册到客户端对象中
        client.addServiceInstance(singleton, instanceInfo);
        //更新客户端的最新活跃时间
        client.setLastUpdatedTime();
        //更新客户端状态变更次数
        client.recalculateRevision();


        //下面发布了一个事件，这个操作非常重要，这里大家可以暂时记住这个事件，下一章我们就会围绕这个事件对一系列功能展开实现
        
        //事件通知中心发布客户端注册服务信息事件
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientRegisterServiceEvent(singleton, clientId));
        
    }



    //客户端订阅服务实例信息的方法
    @Override
    public void subscribeService(Service service, Subscriber subscriber, String clientId) {
        Service singleton = ServiceManager.getInstance().getSingletonIfExist(service).orElse(service);
        Client client = clientManager.getClient(clientId);
        if (!clientIsLegal(client, clientId)) {
            return;
        }//在这里调用了客户端对象的addServiceSubscriber()方法，把客户端订阅对象添加到客户端map中了
        client.addServiceSubscriber(singleton, subscriber);
        client.setLastUpdatedTime();
        //在这里发布了客户端订阅事件，这个事件非常重要，大家先记住这个事件即可
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientSubscribeServiceEvent(singleton, clientId));
    }

    
    //以下就是批量注册服务实例信息，注销服务实例信息，取消订阅服务实例信息的方法，这几个方法在第七版本代码中还用不到，所以我就先不为大家展示了

    
}
```

可以看到，在上面代码块中有一个 registerInstance() 方法，在该方法中会把 nacos 客户端注册的服务实例信息存放到与其对应的 ConnectionBasedClient 客户端对象中。这个时候，就意味着 nacos 客户端向服务端注册服务实例成功了。**而我们最终要实现的就是让 nacos 服务端知道有客户端向自己注册了服务实例**，这个操作在上面的代码块中已经实现了，在上面代码块的第 59 行执行了这样一行代码，请看下面代码块。

```

//事件通知中心发布客户端注册服务信息事件
NotifyCenter.publishEvent(new ClientOperationEvent.ClientRegisterServiceEvent(singleton, clientId));
```

这行代码的作用非常明显，**就是使用 NotifyCenter 事件通知中心发布了一个客户端注册服务实例事件，也就是 ClientRegisterServiceEvent 事件对象；并且把客户端注册的服务实例所在服务的信息，以及客户端连接 Id 都封装到事件对象中了**。这样一来，**我只要为该 ClientRegisterServiceEvent 事件对象再定义一个订阅者，或者说观察者，专门监听该事件，只要 nacos 服务端发布了该事件，那么我定义的这个观察者就可以执行某些操作，比如就是从发布的事件中得到客户端注册的服务实例所在服务的信息，也就是 Service 对象，也可以得到客户端连接 Id，然后再把它们以键值对的方式存放到我刚才定义的 map 中不就行了**？这样一来，问题就迎刃而解了。

当然，还有一点需要补充，那就是在我刚才为大家展示的 EphemeralClientOperationServiceImpl 组件中，大家可以看到在这一章我们为这个组件最新定义的 subscribeService() 方法中，也发布了一个事件对象，**那就是 ClientSubscribeServiceEvent 客户端订阅服务实例事件对象；并且把客户端要订阅的服务实例所在服务的信息以及客户端连接 Id 都封装到事件对象中了**。这是为什么呢？其实原理和发布 ClientRegisterServiceEvent 事件对象是一摸一样的。请大家想一想，nacos 客户端向服务端订阅了服务实例，这些信息都会存放在与客户端对应的 ConnectionBasedClient 客户端对象中。**但是假如多个 nacos 客户端都订阅了服务实例信息，并且有些客户端订阅的服务实例信息也是相同的，nacos 服务端要根据这些客户端订阅的服务实例所在服务的信息，也就是 Service 对象，把最新的服务实例信息推送给这些客户端**。那么，假如有些服务实例信息在服务端发生变更了，nacos 服务端怎么就能精确地知道是哪些客户端订阅了该服务实例呢？这样分析下来，**是不是还应该再定义一个 map，这个 map 和我们之前定义的 map 一样，key 仍然代表服务信息，也就是 Service 对象，value 也是一个集合，只不过这个集合中存放的就是订阅了该服务下所有服务实例信息的客户端的连接 Id 的集合**。就像下面代码块展示的这样，请看下面代码块。

```
//key就是服务实例所在服务的信息，value就是客户端连接Id的集合
ConcurrentHashMap<Service, Set<String>> map = new ConcurrentHashMap<>();
```

这个 map 展示完毕之后，那么怎么向这个 map 中存放数据呢？现在我们都知道了客户端向服务端成功订阅了服务实例信息时，会发布一个 ClientSubscribeServiceEvent 事件对象，**如果我们也为这个事件对象定义一个订阅者，也就是一个观察者，一直监听这个事件。只要 nacos 服务端发布了这个事件，这个观察者就可以执行相应的操作，把客户端订阅的服务实例所在服务的信息以及客户端连接 Id 存放到我们新定义的这个 map 中**。这个逻辑想必也是很容易理解的，对吧？

  

实际上在 nacos 源码中就是这么做的，**只不过 nacos 源码并没有为 ClientRegisterServiceEvent 和 ClientSubscribeServiceEvent 这两个事件都定义了订阅者对象**，**而是定义了一个订阅者对象，让这个订阅者对象可以同时监听这两个事件**。大家也许还记得我在实现 nacos 客户端的时候，把 NotifyCenter 事件通知机制实现了，关于这个机制我就不再赘述了，大家如果忘了就自己回顾一下之前的内容。在实现 NotifyCenter 事件通知中心的事件通知机制时，我引入了一个 InstancesChangeNotifier 服务实例变更通知器，这个通知器其实就是一个订阅者，专门订阅 InstancesChangeEvent 服务实例变更事件，就像下面展示的这样，请看下面代码块。

```
package com.cqfy.nacos.client.naming.event;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/24
 * @方法描述：这个类的对象就是一个服务实例更新通知器，当服务端的服务实例发生变化时，如果客户端订阅了服务端的服务实例，那么服务端的服务实例一旦发生变化就会通知客户端
 * 客户端在感知到服务实例发生变化之后，就会通过NotifyCenter发布InstancesChangeEvent事件，InstancesChangeNotifier观察器观察到事件后，就会执行内部的回调方法
 */
public class InstancesChangeNotifier extends Subscriber<InstancesChangeEvent> {


    //得到该订阅者订阅的事件类型，InstancesChangeNotifier继承了Subscriber抽象类，所以是一个订阅者
    @Override
    public Class<? extends Event> subscribeType() {
        return InstancesChangeEvent.class;
    }

    
    //省略其他内容
}
```

可以看到，这个 InstancesChangeNotifier 服务实例变更通知器继承 Subscriber 抽象类，表示这个通知器就是一个订阅者，专门订阅 InstancesChangeEvent 事件。可以看到，这个 InstancesChangeEvent 订阅者能订阅的事件非常单一，只可以订阅一个事件。但是在 nacos 源码中，还有一个 SmartSubscriber 抽象类，我记得在实现 nacos 客户端的时候为大家简单提到过这个类，这个 SmartSubscriber 类的对象也是一个订阅者，只不过这个订阅者可以订阅多个事件，表示对多个事件都感兴趣。**我们接下来要引入的订阅者，就是 SmartSubscriber 类的子类，这个订阅者订阅了 ClientRegisterServiceEvent 和 ClientSubscribeServiceEvent 这两个事件，这样一来，只要 nacos 服务端使用 NotifyCenter 事件通知中心发布了这两个事件，我们新定义的这个订阅者对象就可以处理这两个事件了**。**而我们要新引入的这个订阅者就是 ClientServiceIndexesManager 类**，具体实现也很简单，接下来就让我为大家展示一下。

当然，在展示具体的 ClientServiceIndexesManager 类之前，还是应该先把刚才的 ClientRegisterServiceEvent 和 ClientSubscribeServiceEvent 两个事件对象展示一下，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.event.client;



public class ClientOperationEvent extends Event {

    private static final long serialVersionUID = -4582413232902517619L;

    private final String clientId;

    private final Service service;

    public ClientOperationEvent(String clientId, Service service) {
        this.clientId = clientId;
        this.service = service;
    }

    public String getClientId() {
        return clientId;
    }

    public Service getService() {
        return service;
    }



    //这个内部类的对象表示客户端注册服务实例事件
    public static class ClientRegisterServiceEvent extends ClientOperationEvent {

        private static final long serialVersionUID = 3412396514440368087L;

        public ClientRegisterServiceEvent(Service service, String clientId) {
            super(clientId, service);
        }
    }


    //这个内部类的对象表示客户端注销服务实例事件
    public static class ClientDeregisterServiceEvent extends ClientOperationEvent {

        private static final long serialVersionUID = -4518919987813223120L;

        public ClientDeregisterServiceEvent(Service service, String clientId) {
            super(clientId, service);
        }
    }


    //这个内部类的对象表示客户端订阅服务实例事件
    public static class ClientSubscribeServiceEvent extends ClientOperationEvent {

        private static final long serialVersionUID = -4518919987813223120L;

        public ClientSubscribeServiceEvent(Service service, String clientId) {
            super(clientId, service);
        }
    }


    //这个内部类的对象表示客户端取消订阅服务实例事件
    public static class ClientUnsubscribeServiceEvent extends ClientOperationEvent {

        private static final long serialVersionUID = -4518919987813223120L;

        public ClientUnsubscribeServiceEvent(Service service, String clientId) {
            super(clientId, service);
        }
    }


    //这个内部类的对象表示客户端连接释放事件
    public static class ClientReleaseEvent extends ClientOperationEvent {

        private static final long serialVersionUID = -281486927726245701L;

        private final Client client;

        private final boolean isNative;

        public ClientReleaseEvent(Client client, boolean isNative) {
            super(client.getClientId(), null);
            this.client = client;
            this.isNative = isNative;
        }

        public Client getClient() {
            return client;
        }

        public boolean isNative() {
            return isNative;
        }
    }
}
```

从上面代码块中可以看到，我不仅仅为大家展示了文章中用到的两个事件对象，还把其他事件类型也展示出来了，这些都是我们后面要用到的事件对象，很快就会用到了。好了朋友们，在事件对象展示完毕之后，接下来就该真正展示我们为 ClientRegisterServiceEvent 和 ClientSubscribeServiceEvent 两个事件对象定义的订阅者了，也就是 ClientServiceIndexesManager 类，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.index;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/20
 * @方法描述：客户端服务信息索引管理器
 */
@Component
public class ClientServiceIndexesManager extends SmartSubscriber {


    //这个map存放的就是服务实例元信息和客户端Id的键值对信息
    //这里大家可能有点不清楚，为什么要存放这样一个键值对呢？请大家想一想，注册中心可能有多个客户端都注册了相同的服务实例
    //这样一来一个服务下面就会存在多个服务实例信息，当有客户端订阅这些服务实例信息的时候，肯定要把该服务下的所有服务实例信息都返回给订阅信息的客户端
    //所以要找到这些服务实例的元信息，元信息是服务实例共有的，然后再把所有服务实例所在客户端的连接Id收集起来，等到要返回服务实例信息给客户端时
    //就可以根据每一个服务实例的客户端连接Id，去服务端创建的客户端对象中获得对应的服务实例信息对象，然后再返回给客户端即可
    private final ConcurrentMap<Service, Set<String>> publisherIndexes = new ConcurrentHashMap<>();

    //这个是存放服务元信息和订阅该服务的客户端Id的键值对，理由同上就不再赘述了
    private final ConcurrentMap<Service, Set<String>> subscriberIndexes = new ConcurrentHashMap<>();

    
    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：构造方法，在该方法中把当前服务索引管理器注册到事件通知中心了，这个服务索引管理器就是一个订阅者，NamingEventPublisherFactory工厂会创建一个命名服务的事件发布器
     * 服务索引管理器这个订阅者就会被放到命名服务事件发布器中
     */ 
    public ClientServiceIndexesManager() {
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：这个服务信息索引管理器本身就是一个订阅者，这个方法就定义了该订阅者关注的事件类型，因为这个订阅者继承了SmartSubscriber类，所以这个订阅者可以关注多个使劲啊
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {

        List<Class<? extends Event>> result = new LinkedList<>();
        //添加客户端注册服务实例事件
        result.add(ClientOperationEvent.ClientRegisterServiceEvent.class);
        //添加客户端注销服务实例事件
        result.add(ClientOperationEvent.ClientDeregisterServiceEvent.class);
        //添加客户端订阅服务实例事件
        result.add(ClientOperationEvent.ClientSubscribeServiceEvent.class);
        //添加客户端取消订阅服务实例事件
        result.add(ClientOperationEvent.ClientUnsubscribeServiceEvent.class);
        //添加客户端连接释放事件
        result.add(ClientOperationEvent.ClientReleaseEvent.class);
        return result;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：该订阅者处理事件的方法，只要事件通知中心发布了该订阅者关注的事件，这个方法就会被回调
     */
    @Override
    public void onEvent(Event event) {
        //判断事件通知中心发布的事件是否为客户端连接释放事件
        if (event instanceof ClientOperationEvent.ClientReleaseEvent) {
            //如果是链接释放事件，则处理该事件
            handleClientDisconnect((ClientOperationEvent.ClientReleaseEvent) event);
        } else if (event instanceof ClientOperationEvent) {
            //执行到这里就意味着是其他的四种事件中的一个，直接处理这些事件即可
            handleClientOperation((ClientOperationEvent) event);
        }
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：处理客户端操作的事件
     */
    private void handleClientOperation(ClientOperationEvent event) {
        //从事件中得到客户端要操作的服务信息
        Service service = event.getService();
        //接着从事件中得到客户端的连接Id
        String clientId = event.getClientId();
        //接下来就该判断事件的具体类型了
        //首先判断是不是客户端注册服务实例事件
        if (event instanceof ClientOperationEvent.ClientRegisterServiceEvent) {
            //如果是客户端注册服务实例事件，那就是客户端Id添加到publisherIndexes中
            addPublisherIndexes(service, clientId);
        } else if (event instanceof ClientOperationEvent.ClientSubscribeServiceEvent) {
            //如果是客户端订阅服务实例信息事件
            //就把客户端Id添加到subscriberIndexes中
            addSubscriberIndexes(service, clientId);
        } 

        //该方法其他内容省略
    }



    private void addPublisherIndexes(Service service, String clientId) {
        publisherIndexes.computeIfAbsent(service, key -> new ConcurrentHashSet<>());
        publisherIndexes.get(service).add(clientId);

        //这里发布了服务变更事件，这个事件非常重要，大家可以先记住，后面两章我们就会用到这个事件
        //其实这里大家应该也能猜出来，客户端注册了新的服务实例信息过来，假如有客户端已经提前订阅了这个服务
        //这就意味着有服务实例发生变更了，所以要把最新的服务实例信息推送给客户端，而这里发布的这个事件，就会被相关订阅者处理
        //处理的具体逻辑就是向客户端推送最新的服务实例信息
        NotifyCenter.publishEvent(new ServiceEvent.ServiceChangedEvent(service, true));
    }



    private void addSubscriberIndexes(Service service, String clientId) {
        subscriberIndexes.computeIfAbsent(service, key -> new ConcurrentHashSet<>());
        if (subscriberIndexes.get(service).add(clientId)) {

            //这里发布的事件也非常重要，和上面方法中发布的事件的作用没什么区别，大家先记住即可
            NotifyCenter.publishEvent(new ServiceEvent.ServiceSubscribedEvent(service, clientId));
        }
    }
    
    

    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：获得所有注册了service服务下的服务实例的客户端Id的set集合
     */ 
    public Collection<String> getAllClientsRegisteredService(Service service) {
        return publisherIndexes.containsKey(service) ? publisherIndexes.get(service) : new ConcurrentHashSet<>();
    }

    
    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：获得所有订阅了service服务下的服务实例的客户端Id的set集合
     */
    public Collection<String> getAllClientsSubscribeService(Service service) {
        return subscriberIndexes.containsKey(service) ? subscriberIndexes.get(service) : new ConcurrentHashSet<>();
    }



    //该类的其他内容就先省略了，大家可以直接去我提供的第八版本代码中查看

}
```

从上面代码块中可以看到，我在 ClientServiceIndexesManager 类中定义了两个 map，这两个 map 就是我们之前新引入的 map，只不过我现在把它们摆在了正确的位置，也就是定义为了 ClientServiceIndexesManager 类的成员变量。大家还可以看到，这个 ClientServiceIndexesManager 类上添加了 springboot 的 @Component 注解，这就意味着这个类的对象也是被 springboot 容器管理器的 bean 对象，**也就是说这个类的对象会被 springboot 自动创建，创建的过程中会调用这个类的无参构造方法，而在本类的无参构造方法中，会把自己注册到事件通知中心里，因为本类的对象就是一个订阅者，并且订阅了 ClientRegisterServiceEvent 和 ClientSubscribeServiceEvent 这两个事件**。这样一来，等到 nacos 服务端使用 NotifyCenter 事件通知中心发布了这两个事件，ClientServiceIndexesManager 这个订阅者就会处理这两个事件，执行相应的操作了。NotifyCenter 事件通知中心以及事件发布器的具体工作流程我就不为大家重复讲解了，这些都是旧知识，在实现 nacos 注册中心客户端时都为大家讲解完毕了，大家自己回顾回顾就行。

总之，到此为止我们已经把客户端订阅和注册服务的关键信息维护完毕了，也就是说，**如果现在给定义一个 Service 对象，根据我们刚刚定义完毕的这个 ClientServiceIndexesManager 组件，nacos 服务端就可以清楚地知道有哪些客户端向 Service 这个服务下注册了服务实例信息，有哪些客户端订阅了 Service 服务下的所有服务实例信息**。 但是引入了 ClientServiceIndexesManager 组件只是第一步，别忘了我们最终的目的：**我们希望的是能够根据 Service 对象找到哪些 nacos 客户端向这个服务之下注册了服务实例信息，把这些注册了服务实例信息的客户端找出来之后，再从这些客户端对象对应的 ConnectionBasedClient 客户端对象中把对应的服务实例找出来，然后一起返回给订阅了该 Service 服务的客户端**。

现在我们已经获得了客户端订阅和注册服务的关键信息，以键值对的方式存储起来了，那么怎么把某个 Service 服务下的所有服务实例找出来返回给订阅了该服务的客户端呢？课程进行到这里，我相信对大家来说，这个功能其实很容易就能实现了，大家肯定有多种多样的方式实现这个功能。但我们还是要参考一下源码，以 nacos 源码的方式来实现这个功能。这一章内容显然已经够多了，篇幅也很长，所以这些内容就放到下一章讲解吧。这一章结束之后大家其实就可以去看看我提供的第八版本代码了，当然，第八版本代码中还有部分代码和功能我没有在文章中展示和实现，但这也不影响大家看看我们已经实现的功能。等下一章结束了，第八章代码的所有功能就全实现完毕了，如果大家想等一等，也可以等看完下一章了再去阅读第八版本代码。好了诸位，我们下一章见！