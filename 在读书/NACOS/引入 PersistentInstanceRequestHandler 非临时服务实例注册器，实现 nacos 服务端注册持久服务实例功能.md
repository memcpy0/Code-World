在我们为 nacos 服务端构建完毕 Raft 集群之后，这一章我们一起来实现 nacos 客户端向服务端注册服务实例功能，在实现这个功能的过程中，就会用到我们上一章实现 nacos Raft 集群。所以，这一章的内容其实非常简单，就是对 Raft 集群程序执行流程的一个梳理。因此，这一章我就进行得快一点，按照梳理源码的方式为大家直接实现 nacos 客户端向服务端注册持久服务实例的功能。只要大家掌握了前面所有章节的内容，在学习本章的时候，会觉这一章其实就是在帮助大家回顾之前的知识。好了朋友们，话不多说，直接开始本章的内容吧。

  

## 实现 nacos 客户端注册持久服务实例功能

既然是要让 nacos 客户端向服务端注册持久服务实例，那肯定要从 nacos 客户端入手，看看 nacos 客户端怎么包装持久服务实例信息，然后怎么把持久信息发送给 nacos 服务端。这个非常容易实现，请大家先回顾一下，在我们之前实现 nacos 客户端向服务端注册临时服务实例信息时是怎么做的呢？是不是定义了一个 Instance 对象，然后把服务实例信息封装在了 Instance 对象中，然后把该对象封装在了 InstanceRequest 请求中发送给 nacos 服务端，这个操作流程大家应该还记得吧？我把相关的代码展示在下面了，帮助大家回顾一下，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //该类的其他内容省略



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册服务实例到服务端的方法
     */
    @Override
    public void registerService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[REGISTER-SERVICE] {} registering service {} with instance {}", namespaceId, serviceName,
                instance);
        //判断要注册的服务实例是否为临时的
        if (instance.isEphemeral()) {
            //如果是临时的直接调用下面这个方法即可，在之前我们一直注册的是临时服务实例信息
            //所以会进入下面这个方法
            registerServiceForEphemeral(serviceName, groupName, instance);
        } else {
            //注册持久的服务实例
            doRegisterServiceForPersistent(serviceName, groupName, instance);
        }
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册临时服务实例到服务端的方法
     */
    private void registerServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {
        //在第三版本中，这里多了一行代码，这行一调用，就会重建一个服务实例重做对象，这个对象会被存放到重做服务对象的map中
        redoService.cacheInstanceForRedo(serviceName, groupName, instance);
        doRegisterService(serviceName, groupName, instance);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法同上，就是在该方法中创建了一个InstanceRequest请求，然后把请求发送给服务端了
     */
    public void doRegisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        //创建了InstanceRequest请求对象，这个对象中封装了各种参数，命名空间，服务名称，分组名称等等
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.REGISTER_INSTANCE, instance);
        //在这里把请求发送给服务端，并且指定了要返回的对象类型
        requestToServer(request, Response.class);
        //这里就是把服务实例重做对象的注册标志设置为已注册，意味着已经把一个服务实例成功注册到服务端了
        redoService.instanceRegistered(serviceName, groupName);
    }

    

    //该类的其他内容省略
}







package com.cqfy.nacos.api.naming.pojo;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/3/31
 * @方法描述：该类的对象就封装了一个服务实例的全部信息，包括实例的唯一标识符，IP地址，端口号，权重，健康状态，集群信息，服务信息，用户扩展属性等。
 */
@JsonInclude(Include.NON_NULL)
public class Instance implements Serializable {

    private static final long serialVersionUID = -742906310567291979L;

    //服务实例的唯一标识符
    private String instanceId;

    //IP地址
    private String ip;

    //端口号
    private int port;

    //服务实例的权重
    private double weight = 1.0D;

    //健康状态，默认为true
    private boolean healthy = true;

    //该服务实例是否为启用状态
    private boolean enabled = true;

    //该实例是否为临时的服务实例，是临时实例的话，就会直接把服务实例存放到内存中，而不是存放到数据库中
    //这一点大家可以先记住，等后面实现服务端代码时，大家就知道是怎么回事了
    private boolean ephemeral = true;

    //服务实例所在的集群
    private String clusterName;

    //该服务实例的服务名称
    private String serviceName;

    //额外的实例元数据信息
    private Map<String, String> metadata = new HashMap<>();


    //该类的其他内容省略
}
```

从上面代码块中可以看到，nacos 客户端要注册到服务端的临时服务实例信息，会被封装在一个 Instance 对象中，并且会把该对象的 ephemeral 成员变量设置为 true，表示为临时服务实例信息；在把服务实例信息发送给服务端时，会在 NamingGrpcClientProxy 类的 registerService() 方法中根据 Instance 对象 ephemeral 属性是 true 还是 false 来判断究竟调用哪个方法把服务实例信息注册到服务端。如果是临时服务实例信息，就会调用 NamingGrpcClientProxy 类的 registerServiceForEphemeral() 方法注册服务实例信息到服务端，其实就是把 Instance 对象封装到 InstanceRequest 请求中，把请求发送给服务端即可。这就是 nacos 客户端向服务端注册临时服务实例信息的流程，我想大家应该都非常清楚了。**我们本章要实现的 nacos 客户端向服务端注册持久服务实例信息功能的流程和上面展示的流程几乎一样，只不过就是要把封装了持久服务实例信息的 Instance 对象的 ephemeral 属性设置为 false；这样一来，在 NamingGrpcClientProxy 类的 registerService() 方法中就会调用 doRegisterServiceForPersistent() 方法把持久服务实例信息注册到服务端了**，而这个 doRegisterServiceForPersistent() 方法我还没有为大家实现，接下来我就为大家实现一下，实现的逻辑非常简单，**就是定义一个专门封装持久服务实例信息的 PersistentInstanceRequest 请求，然后把请求发送给 nacos 服务端即可**，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //该类的其他内容省略



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册服务实例到服务端的方法
     */
    @Override
    public void registerService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[REGISTER-SERVICE] {} registering service {} with instance {}", namespaceId, serviceName,
                instance);
        //判断要注册的服务实例是否为临时的
        if (instance.isEphemeral()) {
            //如果是临时的直接调用下面这个方法即可，在之前我们一直注册的是临时服务实例信息
            //所以会进入下面这个方法
            registerServiceForEphemeral(serviceName, groupName, instance);
        } else {
            //注册持久的服务实例
            doRegisterServiceForPersistent(serviceName, groupName, instance);
        }
    }


    //注册持久服务实例到服务端的方法
    public void doRegisterServiceForPersistent(String serviceName, String groupName, Instance instance) throws NacosException {
        PersistentInstanceRequest request = new PersistentInstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.REGISTER_INSTANCE, instance);
        requestToServer(request, Response.class);
    }

    

    //该类的其他内容省略
}
```

到此为止，我们就把 nacos 客户端向服务端注册持久服务实例的功能实现了，接下来，就该在 nacos 服务端实现对应的功能，也就是 nacos 服务端接收 PersistentInstanceRequest 请求，然后保存持久服务实例信息的功能。

## 为 nacos 服务端引入 PersistentInstanceRequestHandler

好了，现在我们已经让 nacos 客户端向服务端发送 PersistentInstanceRequest 请求，该请求中封装了 nacos 客户端要注册到服务端的持久服务实例信息，只要 nacos 服务端接收并处理这个 PersistentInstanceRequest 请求，就可以从请求中获得 nacos 客户端发送过来的持久服务实例信息，最后直接存储这些信息即可。那 nacos 服务端怎么处理 PersistentInstanceRequest 请求呢？很简单，**直接定义一个对应的请求处理器即可，所以接下来我们就为 PersistentInstanceRequest 请求定义一个对应的请求处理器，我把这个处理器的名字定义为 PersistentInstanceRequestHandler 了**，具体实现方式请看下面代码块。

```
package com.cqfy.nacos.naming.remote.rpc.handler;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/29
 * @Description:处理非临时服务实例注册请求的处理器
 */
@Component
public class PersistentInstanceRequestHandler extends RequestHandler<PersistentInstanceRequest, InstanceResponse> {

    //用来提供执行非临时性服务实例操作的服务组件
    //这个就是前两章我为大家引入的持久服务实例数据处理器，大家应该还有印象
    private final PersistentClientOperationServiceImpl clientOperationService;


    public PersistentInstanceRequestHandler(PersistentClientOperationServiceImpl clientOperationService) {
        this.clientOperationService = clientOperationService;
    }

    //处理请求的方法
    @Override
    public InstanceResponse handle(PersistentInstanceRequest request, RequestMeta meta) throws NacosException {
        //得到该非临时的服务实例所注册的服务的信息，注意，在这里就把服务实例所在服务设置成非临时的了
        Service service = Service.newService(request.getNamespace(), request.getGroupName(), request.getServiceName(), false);
        //设置服务实例Id
        InstanceUtil.setInstanceIdIfEmpty(request.getInstance(), service.getGroupedServiceName());
        //根据请求类型执行不同的操作
        switch (request.getType()) {
            case NamingRemoteConstants.REGISTER_INSTANCE:
                //执行注册服务实例操作
                return registerInstance(service, request, meta);
            case NamingRemoteConstants.DE_REGISTER_INSTANCE:
                //执行注销服务实例操作
                return deregisterInstance(service, request, meta);
            default:
                //请求未知，直接抛出异常
                throw new NacosException(NacosException.INVALID_PARAM,
                        String.format("Unsupported request type %s", request.getType()));
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:注册非临时服务实例操作的方法
     */
    private InstanceResponse registerInstance(Service service, PersistentInstanceRequest request, RequestMeta meta) {
        //得到封装服务实例信息的对象
        Instance instance = request.getInstance();
        //instance.toInetAddr()得到的就是持久服务实例的IP地址+端口号字符串信息
        //在这里得到存放当前非临时服务实例的客户端对象的Id，注意，这里大家可以看到存放非临时服务实例的客户端对象的Id其实就是
        //当前服务实例的网络地址加上非临时标志的字符串
        String clientId = IpPortBasedClient.getClientId(instance.toInetAddr(), false);
        //执行注册操作
        clientOperationService.registerInstance(service, instance, clientId);
        //回复客户端成功响应
        return new InstanceResponse(NamingRemoteConstants.REGISTER_INSTANCE);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:注销非临时服务实例的方法
     */
    private InstanceResponse deregisterInstance(Service service, PersistentInstanceRequest request, RequestMeta meta) {
        Instance instance = request.getInstance();
        String clientId = IpPortBasedClient.getClientId(instance.toInetAddr(), false);
        clientOperationService.deregisterInstance(service, instance, clientId);
        return new InstanceResponse(NamingRemoteConstants.DE_REGISTER_INSTANCE);
    }
}

```

上面代码块中的逻辑非常清楚，也非常常规，所以我就不再详细讲解了，课程进行到这里，大家对上面代码块中的内容应该非常清楚了。当然，有一点需要特别讲解一下，那就是大家会在上面代码块的 registerInstance() 方法中看到这样一行代码，请看下面代码块。

```
String clientId = IpPortBasedClient.getClientId(instance.toInetAddr(), false);
```

可以看到，在这行代码中，**先通过 IpPortBasedClient 对象为 nacos 服务端当前处理的持久服务实例信息创建了一个客户端连接的唯一 Id，然后再使用 clientOperationService 持久服务实例数据处理器执行了保存服务实例信息的操作**。我先不跟大家解释为什么要这么做，也不跟大家解释 IpPortBasedClient 对象究竟代表什么，而是先让大家看一下 IpPortBasedClient 类的 getClientId() 方法的具体内容，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/29
 * @Description:基于IP地址和端口号的客户端
 */
public class IpPortBasedClient extends AbstractClient {


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:得到客户端连接Id的方法
     */
    public static String getClientId(String address, boolean ephemeral) {
        //将传进来的网络地址和是否临时的标志拼接起来
        return address + ID_DELIMITER + ephemeral;
    }

    //该类的其他内容暂时不做实现
}
```

从上面代码块中可以看到，IpPortBasedClient 类的 getClientId() 方法就是把一个地址字符串和是否为临时服务标志拼接起来。而 getClientId() 方法会在 PersistentInstanceRequestHandler 处理器的 registerInstance() 方法中被调用，传进 getClientId() 方法中的参数正是要被服务端保存的持久服务实例的地址信息。也就是说，**nacos 服务端在保存每一个持久服务实例信息的时候，会根据这个持久服务实例信息的 IP 地址和端口号为这个持久服务实例创建一个客户端 Id**。那请大家想一想，为什么要这么做呢？

好了，现在我们就遇到了本章内容最关键的一个问题，这个问题一旦解决，那么本章就没有什么重要的知识了。请大家先回顾一下，当 nacos 服务端接收到客户端注册过来的临时服务实例信息时会怎么做？肯定是先根据注册信息过来的客户端的连接 Id 找到与该客户端对应的 Client 对象，然后把临时服务实例信息存储在与注册服务实例信息过来的 nacos 客户端对应的 Client 客户端对象中，这个逻辑很清楚吧？也就是说，**nacos 服务端保存临时服务实例的时候，都会把服务实例保存在与注册源客户端对应的 Client 客户端对象中**。那么换成持久服务实例信息也是一样的，也要把持久服务实例信息存放到对应的 Client 客户端对象中，但是，**nacos 服务端不会把持久服务实例信息保存在与注册源客户端对应的 Client 对象中**。这就要从持久服务实例信息的特性说起了。

在前面为 nacos 服务端构建 Raft 集群的时候，我就跟大家说过，**nacos 把持久服务实例注册到 nacos 注册中心后，假如持久服务实例出现健康检测失败的情况，这些持久服务实例并不会像临时服务实例那样直接从当前节点移除了，而是被标记为不健康状态，并且显示在 web 控制台让开发人员及时看到，这对开发人员定位程序出现的问题起到了至关重要的作用**。我想持久服务实例信息的这一点特性应该没什么难理解的，大家肯定都很清楚了，但是在这个特性中有一个关键的地方我没有为大家展开讲解，**那就是 nacos 服务端判断持久服务实例健康检测失败的方式，确切地说，我们还不知道 nacos 服务端怎么对持久服务实例进行健康检查**。

当 nacos 客户端把临时服务实例信息注册到服务端之后，服务端怎么对这些服务实例进行健康检查呢？大家可以稍微想一想就能回忆起来，**实际上 nacos 服务端并不是直接对临时服务实例进行健康检查，而是对注册这些服务实例的客户端进行健康检查，也就是说一个 nacos 客户端和服务端建立连接之后，就会开启一个定时任务，定期执行健康检测，这个操作大家应该都还有印象吧？我就不在文章中展示具体的代码了，都是很久之前的旧知识了，大家自己回顾回顾。当 nacos 服务端发现某个 nacos 客户端和自己的连接断开了，也就是发现该客户端健康检测失败了，就会从客户端管理器中移除与断开连接的 nacos 客户端对应的 Client 客户端对象，也就顺便清除了该客户端注册到服务端的临时服务实例信息；也就是说 nacos 服务端对临时服务实例执行的健康检测手段，其实就是对注册这些临时服务实例的 nacos 客户端执行健康检测，注册它们的 nacos 客户端的状态决定了这些临时服务实例信息的状态**。大家可以自己回顾回顾这些操作。

很好，在回顾了 nacos 服务端对临时服务实例执行健康检查操作之后，那请大家再思考一下，和临时服务实例不同的是，持久服务实例信息会被持久化到服务端本地，而且最重要的一个不同就是，**当服务端对持久服务实例健康检测失败之后，持久服务实例并不会从服务端移除，而是被标记为不健康状态，等状态恢复之后，持久服务实例还可以继续被使用**。**持久服务实例也不会因为注册它的 nacos 客户端出现问题了，就会从服务端清除**。由此可见，**持久服务实例只要被注册到服务端之后，就和注册它的 nacos 客户端没什么关系了，注册源客户端只负责把它注册到服务端，至于注册源客户端之后究竟怎样，都和这个持久服务实例没关系**。这一点大家可以理解吧？如果大家理解了这一点，那我现在就可以告诉大家了，实际上在 nacos 服务端正是这么做的：**当 nacos 服务端接收到某个客户端注册过来的持久服务实例信息后，会跳过注册它的客户端，而是直接和持久服务实例建立 TCP 连接，对该持久服务实例执行健康检测，健康检测失败则把该持久服务实例标记为不健康状态**。那么这些操作该怎么实现呢？这就要再回过头，从持久服务实例存储的方式来说起了。

刚才我已经跟大家分析了，**nacos 客户端注册到服务端的持久服务实例信息也会被存储在对应的 Client 对象中，但并不是与注册源客户端对应的 Client 客户端对象中**。原因我们已经分析过了，只要持久服务实例被注册到服务端之后，就和注册它的客户端没什么关系了，客户端的状态是否健康都不会影响它的状态。所以肯定就不必再把这些持久服务实例存储在与注册它们的 nacos 客户端对应的 Client 对象中。那 nacos 服务端是怎么存储持久服务实例信息的呢？非常简单，那就是我在上面代码块中展示的那样，**先使用 IpPortBasedClient 类的 getClientId() 方法为这个持久服务实例创建了一个客户端 Id**，之后要做的就很清楚了，**那就是为这个客户端 Id 创建一个对应的 Client 客户端对象，也就是为这个持久服务实例所在的服务器创建了一个 Client 客户端对象，然后把持久服务实例存储在这个 Client 客户端对象中**。到最后，**就是每一个持久服务实例，都由与自己所在服务器对应的 Client 客户端对象保存**。那么 nacos 服务端对这些服务实例执行的健康检查操作在哪里执行呢？**就在持久服务实例所在的客户端对象中，也就是 IpPortBasedClient 客户端对象中**。现在大家应该也清楚了，IpPortBasedClient 对象其实就是用来存储对应的持久服务实例信息的，从 IpPortBasedClient 类继承了 AbstractClient 抽象类这一点就能看出来了。

接下来，就该把上面的文字分析落实到代码层面了，在我之前为大家展示的代码中，**我们只是从 IpPortBasedClient 类的 getClientId() 方法中得到了要存储持久服务实例信息的 Client 对象 Id，并没有使用这个客户端 Id 创建对应的 Client 客户端对象，接下来我们就该实现这个操作，并且在创建了对应的 Client 客户端对象后，还要把持久服务实例信息存储在这个 Client 客户端对象中**。**而创建的操作就被定义在了持久服务实例信息数据处理器中了，也就是 PersistentClientOperationServiceImpl 类中，并且就定义在这个类的 registerInstance() 方法中**，因为在我之前展示的代码块中，就把从 IpPortBasedClient 得到的客户端 Id 交给 PersistentClientOperationServiceImpl 对象的 registerInstance() 方法使用了。所以，我们要对之前引入的 PersistentClientOperationServiceImpl 类进行一番重构，尤其是对该类的 registerInstance() 方法进行重构，重构的逻辑似乎就是使用客户端 Id 创建 Client 客户端对象，然后把持久服务实例存储到 Client 客户端对象中了，请大家想一想应该这么做吗？

## 梳理 Raft 集群备份持久服务实例信息的工作流程

如果有朋友认为应该这么做，显然是对前两章的内容没有理解透彻，别忘了，**我们已经为 nacos 服务端构建起 Raft 集群组了，这就意味着只要是 nacos 服务端的写入操作，都要先把操作交给 Raft 集群封装成日志处理，当日志在集群中达成一致后，才能执行写入操作。而我们目前要执行的不正是把持久服务实例信息写入到 nacos 服务端内部对应的 Client 客户端对象的操作吗**？所以，**我们要先把这个写操作先交给 Raft 集群来处理**。这才是 PersistentClientOperationServiceImpl 类的 registerInstance() 方法要被重构的逻辑，当然，具体内容我已经重构完毕了，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.service.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:在第13版本实现服务端注册持久服务实例功能时实现了该类的大部分方法
 */
@Component("persistentClientOperationServiceImpl")
public class PersistentClientOperationServiceImpl extends RequestProcessor {

    //在文章中，展示也是非常简陋的内容

    private final CPProtocol protocol;

    private final ReentrantReadWriteLock lock = new ReentrantReadWriteLock();

    private final ReentrantReadWriteLock.ReadLock readLock = lock.readLock();

    //临时服务实例Client客户端管理器是ConnectionBasedClientManager对象
    //下面这个成员变量就是持久服务实例Client客户端的管理器，这个管理器我就不在文章中为大家展示了
    //大家去看我提供的第13版本代码即可，内容真的很简单，和ConnectionBasedClientManager类的内容差不多
    //大家自己看看即可
    private final PersistentIpPortClientManager clientManager;

    //springboot容器创建该类的对象时，会执行这个无参构造方法
    public PersistentClientOperationServiceImpl(final PersistentIpPortClientManager clientManager) {
        this.clientManager = clientManager;
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



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:注册服务实例到客户端对象中的方法
     */
    public void registerInstance(Service service, Instance instance, String clientId) {
        //得到要注册的服务实例所在的服务
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        //判断服务是否为临时的，临时的直接抛出异常，因为目前注册的是非临时的服务实例
        if (singleton.isEphemeral()) {
            throw new NacosRuntimeException(NacosException.INVALID_PARAM,
                    String.format("Current service %s is ephemeral service, can't register persistent instance.",
                            singleton.getGroupedServiceName()));
        }
        //创建存储非临时的服务实例的请求对象，这个请求要交给jraft集群来处理了
        final InstanceStoreRequest request = new InstanceStoreRequest();
        request.setService(service);
        request.setInstance(instance);
        request.setClientId(clientId);
        //在这里创建写请求，因为是非临时的，所以服务实例信息要在jraft集群中以日志的方法持久化
        //这里把接收并处理该写请求的集群Id设置到写请求中了
        final WriteRequest writeRequest = WriteRequest.newBuilder().setGroup(group())
                .setData(ByteString.copyFrom(serializer.serialize(request))).setOperation(DataOperation.ADD.name())
                .build();
        try {
            //把写请求提交给jraft协议组件处理
            protocol.write(writeRequest);
            Loggers.RAFT.info("Client registered. service={}, clientId={}, instance={}", service, clientId, instance);
        } catch (Exception e) {
            throw new NacosRuntimeException(NacosException.SERVER_ERROR, e);
        }
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

从上面代码块中可以看到，**在我们重构完毕的 PersistentClientOperationServiceImpl 类的 registerInstance() 方法中，把存储持久服务实例信息的操作包装成了一个 WriteRequest 请求，并且把要处理的数据所属的集群 Id 封装到写请求中了，这一点非常重要，大家一定要记住，然后把把这个请求交给 JRaftProtocol 组件处理了。这个操作其实就相当于向 Raft 集群发送了一个写指令**。那么这个 JRaftProtocol 对象得到写请求后是怎么做的呢？或者说它应该做什么呢？肯定是再把这个 WriteRequest 请求交给真正提供 Raft 服务的组件处理吧？也就是交给 JRaftServer 对象来处理。所以接下来我们就来看看 JRaftProtocol 对象得到 WriteRequest 请求之后的具体操作，也就是看看 JRaftProtocol 对象在它的 write() 方法中执行了什么操作，请看下面代码块。

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

从上面代码块中可以看到，JRaftProtocol 对象得到 WriteRequest 写请求之后，会在 write() 方法中把它交给真正提供 Raft 服务的 JRaftServer 组件来处理，也就是调用 JRaftServer 对象的 commit() 方法来处理这个写请求。很好，现在程序终于要执行到 Raft 集群层面了。那么请大家先想一想，当 JRaftServer 组件得到写请求之后，要怎么处理呢？

其实逻辑已经非常清楚了，**首先肯定要根据 WriteRequest 请求对象中封装到数据所属集群组的 Id 来找到要处理这些持久化服务实例信息的 Raft 集群组**，这个是肯定的吧？然后就要做出一个非常重要的判断，**那就是判断当前处理持久服务实例信息的节点是不是 Raft 集群中的领导者，如果是领导者，那么当前节点就可以直接处理这个写请求，如果不是领导者，那就要找到 Raft 集群组的领导者，再把这个写请求发送给领导者，让领导者处理**。这一点应该也不难理解吧？因为 nacos 服务端集群的每一个节点都可能接收客户端连接，被客户端注册持久服务实例信息，而在 Raft 集群中，只有领导者能处理写请求，所以必须要判断处理写请求的当前节点是否为领导者，如果当前节点不是领导者，就要把写请求转发给领导者处理。这些操作都要交给 JRaftServer 组件来执行，所以接下来我们就看看 JRaftServer 组件具体是怎么做的，请看下面代码块。

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


    //省略其他内容

   

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:提交读请求给jraft集群组领导者的方法，让集群组的领导者处理读请求
     */
    public CompletableFuture<Response> commit(final String group, final Message data,
                                              final CompletableFuture<Response> future) {
        //记录请求信息到日志中
        LoggerUtils.printIfDebugEnabled(Loggers.RAFT, "data requested this time : {}", data);
        //根据组名得到要处理当前读请求的集群组对象
        final RaftGroupTuple tuple = findTupleByGroup(group);
        //找不到集群组对象则给future设置错误执行结果
        if (tuple == null) {
            future.completeExceptionally(new IllegalArgumentException("No corresponding Raft Group found : " + group));
            return future;
        }
        //定义一个回调对象，该对象的方法会在操作有结果之后被回调
        FailoverClosureImpl closure = new FailoverClosureImpl(future);
        //从集群组对象中得到集群组中的当前服务器节点
        final Node node = tuple.node;
        //判断当前节点是否为集群组中的领导者
        if (node.isLeader()) {
            //如果是领导者，那直接就让领导者处理这个请求，流程就是把请求包装成日志，在集群节点间达成一致，最后应用到nacos的状态机上
            applyOperation(node, data, closure);
        } else {
            //如果当前节点不是领导者，那就把领导者找出来，然后把读请求发送给领导者，让领导者处理
            invokeToLeader(group, data, rpcRequestTimeoutMs, closure);
        }
        return future;
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:领导者节点直接处理亲求的方法
     */
    public void applyOperation(Node node, Message data, FailoverClosure closure) {
        //以下就是jraft框架的操作逻辑了，定义一个task任务，把请求封装到这个task任务中
        //然后把任务提交给领导者去执行，执行的过程中就会再把任务封装到日志中，然后对日志执行一致性协议操作，最后应用日志到状态机
        final Task task = new Task();
        //设置NacosClosure回调方法，该方法会在状态机应用了日志之后被回调
        task.setDone(new NacosClosure(data, status -> {
            NacosClosure.NacosStatus nacosStatus = (NacosClosure.NacosStatus) status;
            //设置异常对象
            closure.setThrowable(nacosStatus.getThrowable());
            //设置响应对象
            closure.setResponse(nacosStatus.getResponse());
            //在这里执行回调方法，这里执行的就是FailoverClosure中的回调方法，具体逻辑大家可以去FailoverClosure的实现类，也就是FailoverClosureImpl类中查看
            closure.run(nacosStatus);
        }));
        //定义一个存放请求类型的字节数组
        byte[] requestTypeFieldBytes = new byte[2];
        //设置请求类型的读取标志
        requestTypeFieldBytes[0] = ProtoMessageUtil.REQUEST_TYPE_FIELD_TAG;
        //根据请求类型设置具体的请求类型的标志
        if (data instanceof ReadRequest) {
            //设置读请求标志
            requestTypeFieldBytes[1] = ProtoMessageUtil.REQUEST_TYPE_READ;
        } else {//如果是写请求就设置写请求标志
            requestTypeFieldBytes[1] = ProtoMessageUtil.REQUEST_TYPE_WRITE;
        }
        //把请求对象转换为字节数组
        byte[] dataBytes = data.toByteArray();
        //封装数据到task任务中
        task.setData((ByteBuffer) ByteBuffer.allocate(requestTypeFieldBytes.length + dataBytes.length)
                .put(requestTypeFieldBytes).put(dataBytes).position(0));
        //把任务提交给领导者
        node.apply(task);
    }





    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:得到指定集群组的领导者节点的方法
     */
    protected PeerId getLeader(final String raftGroupId) {
        //使用路由表组件得到指定集群组的领导者
        return RouteTable.getInstance().selectLeader(raftGroupId);
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/27
     * @Description:把请求发送给jraft集群组领导者方法，如果是读请求，就会被领导者节点的NacosReadRequestProcessor读请求处理器接收并处理
     * 如果是写请求，就会被NacosWriteRequestProcessor写请求处理器接收并处理
     */
    private void invokeToLeader(final String group, final Message request, final int timeoutMillis,
                                FailoverClosure closure) {
        try {
            //先得到指定集群组中的领导者节点的IP地址，如果找不到则抛出异常
            final Endpoint leaderIp = Optional.ofNullable(getLeader(group))
                    .orElseThrow(() -> new NoLeaderException(group)).getEndpoint();

            //使用集群客户端把请求直接发送给领导者
            cliClientService.getRpcClient().invokeAsync(leaderIp, request, new InvokeCallback() {

                //该方法会在收到响应后被回调
                @Override
                public void complete(Object o, Throwable ex) {
                    //响应异常要执行的操作
                    if (Objects.nonNull(ex)) {
                        closure.setThrowable(ex);
                        closure.run(new Status(RaftError.UNKNOWN, ex.getMessage()));
                        return;
                    }

                    //响应失败要执行的操作
                    if (!((Response)o).getSuccess()) {
                        closure.setThrowable(new IllegalStateException(((Response) o).getErrMsg()));
                        closure.run(new Status(RaftError.UNKNOWN, ((Response) o).getErrMsg()));
                        return;
                    }

                    //响应成功要执行的操作
                    closure.setResponse((Response) o);
                    closure.run(Status.OK());
                }

                //得到执行回调方法的执行器
                @Override
                public Executor executor() {
                    return RaftExecutor.getRaftCliServiceExecutor();
                }
            }, timeoutMillis);
        } catch (Exception e) {
            closure.setThrowable(e);
            closure.run(new Status(RaftError.UNKNOWN, e.toString()));
        }
    }

    

    //省略该类的其他内容
}
```

上面代码块中的注释非常详细，所以我就不再重复讲解其中的逻辑了，唯一需要补充的是，在上面代码块的第 149 行，也就是当前处理持久服务实例信息的节点不是领导者之后，要把写请求转发给集群中的领导者，这时候程序执行了下面这样一行代码，请看下面代码块。

```
cliClientService.getRpcClient().invokeAsync(leaderIp, request, new InvokeCallback() {
```

在上面的代码中，我们可以看到当前节点直接把写请求发送给 Raft 集群中的领导者了，那么 Raft 集群的领导者在接收到写请求之后该怎么办呢？**肯定是直接处理这个写请求，也就是把请求封装成日志，然后开始把这条日志复制给集群的其他节点，这个操作很容易就能想到**。那具体该怎么实现呢？**这时候我就要继续为 nacos 引入一个新的处理器了，也就是专门处理 WriteRequest 请求的 NacosWriteRequestProcessor 请求处理器**，**这个处理器就是 Raft 集群的领导者节点用来处理 WriteRequest 请求的**，具体实现请看下面代码块。

```
package com.cqfy.nacos.core.distributed.raft.processor;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:jraft框架中处理写请求的处理器，可以看到，这个处理器实现了RpcProcessor接口，那就意味着这个处理器会注册到
 * jraft框架的服务端，注意，是jraft框架的服务端，并不是nacos服务端，如果大家了解jraft框架服务端的工作原理
 * 那就会知道这个处理器是如何发挥作用的，这个处理器还继承了AbstractProcessor类，其实核心逻辑都定义在了AbstractProcessor父类中
 * 我把这个父类的代码也展示在当前代码块中了
 */
public class NacosWriteRequestProcessor extends AbstractProcessor implements RpcProcessor<WriteRequest> {
    
    private static final String INTEREST_NAME = WriteRequest.class.getName();
    
    private final JRaftServer server;
    
    public NacosWriteRequestProcessor(JRaftServer server, Serializer serializer) {
        super(serializer);
        this.server = server;
    }

    //领导者处理WriteRequest请求的方法
    @Override
    public void handleRequest(RpcContext rpcCtx, WriteRequest request) {
        //在这里会调用父类的方法
        handleRequest(server, request.getGroup(), rpcCtx, request);
    }
    
    @Override
    public String interest() {
        return INTEREST_NAME;
    }
}




//下面就是AbstractProcessor类的内容



package com.cqfy.nacos.core.distributed.raft.processor;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:jraft框架要用到的读请求和写请求处理器的抽象父类
 */
public abstract class AbstractProcessor {
    
    private final Serializer serializer;
    
    public AbstractProcessor(Serializer serializer) {
        this.serializer = serializer;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:处理jraft集群读请求和写请求的方法，其实不管是读请求还是写请求，在这里都是Message对象
     * 真正要处理请求，还是要等到这个请求在jraft集群中转了一圈，最后状态机应用日志，也就是处理请求的时候才会转换Message对象为具体的请求对象
     * 然后根据不同的请求执行不同的操作
     */
    protected void handleRequest(final JRaftServer server, final String group, final RpcContext rpcCtx, Message message) {
        try {//这里就是得到要处理当前请求的jraft集群，如果大家已经看过并且掌握了JRaftServer类的内容，就知道jraft框架的MultiRaftGroup是什么意思
            //也就明白了为什么一个服务器上可以存在多个jraft集群了
            final JRaftServer.RaftGroupTuple tuple = server.findTupleByGroup(group);
            //因为tuple对象中持有者要处理请求的集群的领导者，这么说有点不太准确，实际上这个对象中只持有
            //当前节点自己的信息，但是请大家想一想，现在已经是jraft集群某个节点的服务端在处理请求了
            //那么这个节点肯定就是领导者节点，因为只有领导者节点才能处理请求，并且在nacos中也是这么做的
            //只要有请求到达nacos服务端，nacos服务端会首先找到要处理这个请求的jraft集群，然后判断当前节点是否为集群领导者，如果不是就找到领导者
            //把请求发送给领导者，让领导者来处理；所以这里先对tuple对象判空，如果该对象为空
            //就意味着没有集群可以处理当前请求
            if (Objects.isNull(tuple)) {
                //回复错误响应即可
                rpcCtx.sendResponse(Response.newBuilder().setSuccess(false).setErrMsg("Could not find the corresponding Raft Group : " + group).build());
                return;
            }//在这里得到处理处理当前请求的集群的领导者节点
            //然后判断该节点究竟是不是jraft集群的领导者
            if (tuple.getNode().isLeader()) {
                //如果是则直接让领导者处理请求即可，处理的逻辑无非就是把请求包装成日志，然后让日志在jraft集群达成一致，最后把日志应用到状态机上
                execute(server, rpcCtx, message, tuple);
            } else {
                //走到这里意味着找到的节点不是领导者，那就直接回复找不到领导者的错误响应，
                rpcCtx.sendResponse(Response.newBuilder().setSuccess(false).setErrMsg("Could not find leader : " + group).build());
            }
        } catch (Throwable e) {
            Loggers.RAFT.error("handleRequest has error : ", e);
            rpcCtx.sendResponse(Response.newBuilder().setSuccess(false).setErrMsg(e.toString()).build());
        }
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/26
     * @Description:jraft集群领导者节点处理请求的方法
     */
    protected void execute(JRaftServer server, final RpcContext asyncCtx, final Message message,
            final JRaftServer.RaftGroupTuple tuple) {
        //定义一个回调方法，该方法会在jraft集群应用这条请求日志之后被回调
        FailoverClosure closure = new FailoverClosure() {
            
            Response data;
            
            Throwable ex;
            
            @Override
            public void setResponse(Response data) {
                this.data = data;
            }
            
            @Override
            public void setThrowable(Throwable throwable) {
                this.ex = throwable;
            }
            
            @Override
            public void run(Status status) {
                if (Objects.nonNull(ex)) {
                    //如果在处理请求中有异常，则直接返回错误响应
                    Loggers.RAFT.error("execute has error : ", ex);
                    asyncCtx.sendResponse(Response.newBuilder().setErrMsg(ex.toString()).setSuccess(false).build());
                } else {//返回正常响应
                    asyncCtx.sendResponse(data);
                }
            }
        };
        //在这里让JRaftServer对象处理请求，JRaftServer对象的applyOperation()方法不就是
        //Raft集群领导者直接处理请求的方法吗？这里就是正好让集群领导者处理请求的操作
        server.applyOperation(tuple.getNode(), message, closure);
    }
    
}
```

好了，最后一点要补充的内容也补充完毕了，接下来的操作就要全交给 jraft 框架来执行了，把日志复制给 Raft 集群中的其他节点，等待日志在集群中达成一致，这些操作都不是 nacos 能执行的了。nacos 能做的就是等到日志在 Raft 集群中达成一致了，Raft 集群要把日志应用到状态机上时，这个时候就又轮到 nacos 登场了。在前两章我已经为 nacos 引入了它要在 Raft 集群中使用的状态机对象，就是下面代码块中展示的内容，请看下面代码块。

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

从上面代码块中可以看到，**当 Raft 集群要把日志应用到 NacosStateMachine 状态机上时，其实最终是调用了数据处理器的 onApply() 方法来执行了对应的操作。而处理持久服务实例信息的数据处理器就是我们早就已经引入的 PersistentClientOperationServiceImpl 对象，同时我们也早就清楚数据处理器要执行的操作是什么。现在这些持久服务实例信息已经在 Raft 集群中达成一致了，并且也已日志的形式备份到每一个节点了，接下来就该真正把这些数据写入到 nacos 服务端了吧？所谓写入就是使用早就创建好的客户端 Id，为这些持久服务实例信息创建对应的 Client 客户端对象，然后把持久服务实例信息存储到 Client 客户端对象中**。那接下来我们只需要按照这些逻辑来重构 PersistentClientOperationServiceImpl 类即可，并且要从重构该类的 onApply() 方法开始，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.service.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/26
 * @Description:在第13版本实现服务端注册持久服务实例功能时实现了该类的大部分方法
 */
@Component("persistentClientOperationServiceImpl")
public class PersistentClientOperationServiceImpl extends RequestProcessor {

    //在文章中，展示也是非常简陋的内容

    private final CPProtocol protocol;

    private final ReentrantReadWriteLock lock = new ReentrantReadWriteLock();

    private final ReentrantReadWriteLock.ReadLock readLock = lock.readLock();

    //临时服务实例Client客户端管理器是ConnectionBasedClientManager对象
    //下面这个成员变量就是持久服务实例Client客户端的管理器，这个管理器我就不在文章中为大家展示了
    //大家去看我提供的第13版本代码即可，内容真的很简单，和ConnectionBasedClientManager类的内容差不多
    //大家自己看看即可
    private final PersistentIpPortClientManager clientManager;

    //springboot容器创建该类的对象时，会执行这个无参构造方法
    public PersistentClientOperationServiceImpl(final PersistentIpPortClientManager clientManager) {
        this.clientManager = clientManager;
        //在这里从容器中得到JRaftProtocol对象，然后执行了初始化操作
        this.protocol = ApplicationUtils.getBean(ProtocolManager.class).getCpProtocol();
        //在这里为处理注册持久的服务实例的请求创建了一个jraft集群
        this.protocol.addRequestProcessors(Collections.singletonList(this));
    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:nacos状态机在jraft集群应用日志时，最后会从日志中得到具体的写请求，然后把请求交给数据处理器来处理
     * 这个方法就是处理具体的写请求的方法
     */
    @Override
    public Response onApply(WriteRequest request) {
        final Lock lock = readLock;
        lock.lock();
        try {
            //得到封装了服务实例数据的请求对象
            final InstanceStoreRequest instanceRequest = serializer.deserialize(request.getData().toByteArray());
            //得到要对服务实例对象执行的操作
            final DataOperation operation = DataOperation.valueOf(request.getOperation());
            //根据操作类型执行不同的操作
            switch (operation) {
                case ADD:
                    //把服务实例注册到客户端对象中
                    onInstanceRegister(instanceRequest.service, instanceRequest.instance,
                            instanceRequest.getClientId());
                    break;
                case DELETE:
                    //删除服务实例的操作
                    onInstanceDeregister(instanceRequest.service, instanceRequest.getClientId());
                    break;
                case CHANGE:
                    //更新服务实例的操作，先判断指定的服务是否存在，然后再更新
                    if (instanceAndServiceExist(instanceRequest)) {
                        onInstanceRegister(instanceRequest.service, instanceRequest.instance,
                                instanceRequest.getClientId());
                    }
                    break;
                default:
                    //未知操作直接抛出异常
                    return Response.newBuilder().setSuccess(false).setErrMsg("unsupport operation : " + operation)
                            .build();
            }
            //操作成功返回成功响应
            return Response.newBuilder().setSuccess(true).build();
        } catch (Exception e) {
            Loggers.RAFT.warn("Persistent client operation failed. ", e);
            return Response.newBuilder().setSuccess(false)
                    .setErrMsg("Persistent client operation failed. " + e.getMessage()).build();
        } finally {
            lock.unlock();
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:注册服务实例的方法
     */
    private void onInstanceRegister(Service service, Instance instance, String clientId) {
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        //如果客户端管理器中没有为当前客户端Id创建客户端对象，那直接创建一个客户端对象
        //再把服务实例注册到客户端对象中
        if (!clientManager.contains(clientId)) {
            //这里创建出来的就是IpPortBasedClient对象
            clientManager.clientConnected(clientId, new ClientAttributes());
        }

        Client client = clientManager.getClient(clientId);
        InstancePublishInfo instancePublishInfo = getPublishInfo(instance);
        client.addServiceInstance(singleton, instancePublishInfo);
        client.setLastUpdatedTime();
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientRegisterServiceEvent(singleton, clientId));
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/29
     * @Description:注册服务实例到客户端对象中的方法
     */
    public void registerInstance(Service service, Instance instance, String clientId) {
        //得到要注册的服务实例所在的服务
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        //判断服务是否为临时的，临时的直接抛出异常，因为目前注册的是非临时的服务实例
        if (singleton.isEphemeral()) {
            throw new NacosRuntimeException(NacosException.INVALID_PARAM,
                    String.format("Current service %s is ephemeral service, can't register persistent instance.",
                            singleton.getGroupedServiceName()));
        }
        //创建存储非临时的服务实例的请求对象，这个请求要交给jraft集群来处理了
        final InstanceStoreRequest request = new InstanceStoreRequest();
        request.setService(service);
        request.setInstance(instance);
        request.setClientId(clientId);
        //在这里创建写请求，因为是非临时的，所以服务实例信息要在jraft集群中以日志的方法持久化
        //这里把接收并处理该写请求的集群Id设置到写请求中了
        final WriteRequest writeRequest = WriteRequest.newBuilder().setGroup(group())
                .setData(ByteString.copyFrom(serializer.serialize(request))).setOperation(DataOperation.ADD.name())
                .build();
        try {
            //把写请求提交给jraft协议组件处理
            protocol.write(writeRequest);
            Loggers.RAFT.info("Client registered. service={}, clientId={}, instance={}", service, clientId, instance);
        } catch (Exception e) {
            throw new NacosRuntimeException(NacosException.SERVER_ERROR, e);
        }
    }



    //该数据处理器要被使用的集群组的唯一组名
    @Override
    public String group() {
        return Constants.NAMING_PERSISTENT_SERVICE_GROUP_V2;
    }
    
}
```

到此为止，持久服务实例信息被 nacos 客户端注册到服务端的全部功能就实现完毕了，当然，整个功能的执行流程我也为大家梳理了一遍，本章内容也就到此结束了，至于 nacos 使用的 Raft 集群组件中其他的方法，就不在文章中展示了，大家可以直接看看我提供的第十三版本代码，或者直接去看源码，这些对大家来说都没什么难度了。当然，其实还有一个内容我没有为大家实现和展示，那就是 nacos 服务端对持久服务实例主动执行健康检查操作，我在前面的小节跟大家说这些操作被定义在了 IpPortBasedClient 对象中。确实是这样，**因为 IpPortBasedClient 客户端对象在被创建完毕之后，会调用该对象的 init() 方法，在 init() 方法中，会启动一个定时任务会对该 IpPortBasedClient 客户端对象内存储的持久服务实例进行健康检测**，这些代码很常规，所以我就不在文章中为大家展示了，大家只要按照我说的这些去第十三版本代码中查看即可。

好了朋友们，本章内容就到此结束了，下一章就要开始构建 nacos 的配置中心客户端了，我们下一章见！