**看这一章之前，请大家去网盘中下载最新的版本的代码，我在写这一章和前两章的时候，更新完善了代码的部分注释，有些注释涉及到了文章中并没有讲解的内容，请大家最好去下载最新的代码阅读。**

自从 nacos 的文章更新以来，到目前为止，我们已经完全实现了 nacos 注册中心客户端的基本功能，那就是把服务实例注册到注册中心服务端的功能。只有客户端把服务实例注册到注册中心服务端，那么消费者才可以从注册中心订阅这些服务实例，服务实例才能提供服务。既然我们已经把注册中心客户端注册服务实例的功能实现完毕了，接下来，就该实现客户端订阅服务实例的功能了。而注册中心客户端订阅服务实例的功能实现起来非常简单，因为订阅服务实例的核心逻辑并不在客户端这边，客户端只需要给服务端发送订阅服务实例的请求，服务端那边执行一些核心操作，然后把客户端订阅的服务实例返回给客户端即可。所以，这一章我们的任务非常轻松，所有要实现的功能都很简单，等到实现服务端功能的时候，我们再一起看看服务端是怎么把对应的服务实例挑选出来，然后返回给客户端的。而这一章，我会给大家梳理一些客户端订阅服务实例的流程，然后把一些简单功能实现了，大家再简单看看代码即可。

## 引入 ServiceInfo，实现服务实例信息的封装

既然注册中心客户端要向服务端订阅服务实例信息，那么总该有对象来封装这些服务实例信息吧？不然服务端怎么把这些信息返回给客户端呢？这时候我就要引入一个新的类，那就是 ServiceInfo 类，这个类的对象封装的就是注册到服务端的服务实例的信息。这个 ServiceInfo 类的内容非常简单，无非就是定义一些分组名称，服务实例的名称，集群名称，服务实例的网络地址等成员变量，唯一需要解释的一点就是：**这个 ServiceInfo 对象可能会封装多个服务实例的信息**。就拿 RPC 远程调用框架来举例吧，远程调用框架可能会提供多个功能一样的服务者，这些服务者都会被注册到注册中心服务端，并且服务名称、分组名称也都一样。这样一来，RPC 框架的消费者订阅了这些服务实例的信息后，在执行远程调用时，还可以使用负载均衡功能选择合适的服务提供者，然后远程调用目标方法。所以，注册中心的客户端在订阅服务实例信息时，可能会在要订阅的分组名称以及服务名称下订阅到好几个服务实例信息，这些服务实例的信息都会被封装到 ServiceInfo 对象中，然后由服务端返回给客户端。接下来，就请大家看看 ServiceInfo 类的具体内容，请看下面代码块。

```
package com.cqfy.nacos.api.naming.pojo;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/23
 * @方法描述：封装服务实例信息的类，凡是注册到注册中心的服务实例，这些服务实例的信息被传输给客户端之后，都会被包装成在一个ServiceInfo对象中，在第三版本这个ServiceInfo对象的作用还不太明显
 * 第四版本实现了订阅功能，就能立刻体现出这个ServiceInfo对象的作用了
 */
@JsonInclude(JsonInclude.Include.NON_NULL)
public class ServiceInfo {

    //groupName在key中的位置
    private static final int GROUP_POSITION = 0;

    //service name在key中的位置
    private static final int SERVICE_POSITION = 1;

    //集群名称在key中的位置
    private static final int CLUSTER_POSITION = 2;

    //key的长度
    //以上几个成员变量在本类的ServiceInfo(key)方法中会被用到，直接去这个方法中就能很清楚地明白这几个变量的作用
    private static final int FILE_NAME_PARTS = 3;

    private static final String EMPTY = "";

    private static final String DEFAULT_CHARSET = "UTF-8";

    //服务名称
    private String name;

    //组名
    private String groupName;

    //集群名称
    private String clusters;

    //当前服务实例要在服务端缓存的时常，其实就是服务信息的有效期
    private long cacheMillis = 1000L;

    //当前ServiceInfo对象持有的所有服务实例
    private List<Instance> hosts = new ArrayList<>();

    //最新引用时间
    private long lastRefTime = 0L;

    //校验和
    private String checksum = "";

    //该ServiceInfo对象是否持有所有服务实例信息
    private volatile boolean allIPs = false;

    //是否到达保护阈值
    private volatile boolean reachProtectionThreshold = false;


    public ServiceInfo() {
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：该构造方法会解析key，给对象中的成员变量赋值
     */
    public ServiceInfo(final String key) {
        //将key转换为数组
        String[] keys = key.split(Constants.SERVICE_INFO_SPLITER);
        //判断数组长度是否大于等于3
        if (keys.length >= FILE_NAME_PARTS) {
            //从数组中获得groupName
            this.groupName = keys[GROUP_POSITION];
            //从数组中获得serviceName
            this.name = keys[SERVICE_POSITION];
            //从数组中获得集群信息
            this.clusters = keys[CLUSTER_POSITION];
        } else if (keys.length == CLUSTER_POSITION) {
            //走到这里意味着数组长度为2
            //那集群信息就是空了
            this.groupName = keys[GROUP_POSITION];
            this.name = keys[SERVICE_POSITION];
            this.clusters = null;
        } else {
            throw new IllegalArgumentException("Can't parse out 'groupName',but it must not be null!");
        }
    }

    public ServiceInfo(String name, String clusters) {
        this.name = name;
        this.clusters = clusters;
    }




    //其他get/set方法我都省略了，大家直接看我提供的第四版本代码即可



}
```

到此为止，这个 ServiceInfo 类的内容就展示完毕了。现在我们已经知道了用什么对象封装所有从服务端传输过来的服务实例信息，接下来，我就可以直接实现客户端订阅服务实例功能了。**所谓订阅服务实例功能，其实就是在客户端再定义一个 subscribe 方法，方法参数为要订阅的服务实例的名称、分组名称、集群名称**；客户端只要调用这个 subscribe 方法，就可以直接向服务端订阅对应的服务实例信息。当然，**这个 subscribe 方法还是要被最外层的 NacosNamingService 对象调用，然后执行内部的一系列调用链路，经过 NamingClientProxyDelegate 对象的调用，再经过 NamingGrpcClientProxy 对象的调用，最终向服务端发送一个 SubscribeServiceRequest 请求，这个请求就是专门用来表示服务订阅的请求，服务端接收到这个请求之后，会执行对应的操作，把对应的服务实例信息封装到 ServiceInfo 对象中，然后把 ServiceInfo 对象封装到 SubscribeServiceResponse 服务订阅响应中，回复给客户端**。这就是客户端订阅服务功能的完整流程，接下来，我就给大家展示一下具体的代码。

首先是 SubscribeServiceRequest 请求本身，请看下面代码块。

```
package com.cqfy.nacos.api.naming.remote.request;


public class SubscribeServiceRequest extends AbstractNamingRequest {

    //这个成员变量表示的就是是否为订阅操作
    //为true则是订阅操作
    //为false，则为取消订阅操作
    private boolean subscribe;

    private String clusters;

    public SubscribeServiceRequest() {
    }

    public SubscribeServiceRequest(String namespace, String groupName, String serviceName, String clusters,
                                   boolean subscribe) {
        super(namespace, serviceName, groupName);
        this.clusters = clusters;
        this.subscribe = subscribe;
    }

    public String getClusters() {
        return clusters;
    }

    public void setClusters(String clusters) {
        this.clusters = clusters;
    }

    public boolean isSubscribe() {
        return subscribe;
    }

    public void setSubscribe(boolean subscribe) {
        this.subscribe = subscribe;
    }
}
```

接下来就是 SubscribeServiceResponse 响应对象，请看下面代码块。

```
package com.cqfy.nacos.api.naming.remote.response;


public class SubscribeServiceResponse extends Response {

    //封装了所有服务实例信息
    private ServiceInfo serviceInfo;

    public SubscribeServiceResponse() {
    }

    public SubscribeServiceResponse(int resultCode, String message, ServiceInfo serviceInfo) {
        super();
        setResultCode(resultCode);
        setMessage(message);
        this.serviceInfo = serviceInfo;
    }

    public ServiceInfo getServiceInfo() {
        return serviceInfo;
    }

    public void setServiceInfo(ServiceInfo serviceInfo) {
        this.serviceInfo = serviceInfo;
    }
}
```

请求和响应对象展示完毕之后，接下来就该展示具体的 subscribe 方法了。首先在 NacosNamingService 类中定义 subscribe 方法，请看下面代码块。

```
package com.cqfy.nacos.client.naming;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：这个类是注册中心客户端的核心类，可以说是注册服务实例，订阅服务实例等等操作的入口类，很多重要的操作都是由这个类的对象发起的
 * 但在第一版本代码中，该类只提供了注册服务实例到注册中心的方法
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosNamingService implements NamingService {



    
    //从我在测试类为大家提供的代码，大家也能看出来，实际上NacosNamingService类的对象是通过反射创建的
    //反射调用该类的构造方法，并且调用的是参数为Properties的构造方法，而在测试类的Properties中，设置了要创建的NacosNamingService对象
    //的命名空间，这就意味着每一个NacosNamingService对象都有自己的命名空间，下面这个成员变量表示的就是当前NacosNamingService对象的命名空间，在测试类中为创建的
    //NacosNamingService对象设置了默认的命名空间，也就是public
    private String namespace;

    //代理客户端，该客户端就是用来向注册中心服务器发送请求的
    //这里之所以叫代理客户端，是因为NamingClientProxy内部真正向服务器发送消息时，使用的是grpc构建的客户端
    private NamingClientProxy clientProxy;



    //其他内容省略



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：这个就是订阅服务信息的方法，这个方法是我自己写的，第五版本会重构，一般来说，这个方法都会传入一个监听器，监听订阅的服务实例的变更情况
     */
    public ServiceInfo subscribe(String serviceName, String groupName, String clusters) throws NacosException {
        return clientProxy.subscribe(serviceName, groupName, clusters);
    }

}
```

从上面代码块中可以看到，**如果 NacosNamingService 对象执行了 subscribe 方法，想要去服务端订阅某些服务实例的信息，接下来，就会在 NacosNamingService 对象的 subscribe 方法中继续调用 NamingClientProxyDelegate 对象的 subscribe 方法。所以，接下来我就要接着在 NamingClientProxyDelegate 类中再定义一个 subscribe 方法**，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote;



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




    //其他内容省略




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：订阅服务信息的方法
     */
    @Override
    public ServiceInfo subscribe(String serviceName, String groupName, String clusters) throws NacosException {
        NAMING_LOGGER.info("[SUBSCRIBE-SERVICE] service:{}, group:{}, clusters:{} ", serviceName, groupName, clusters);
        
        //在这里就真正定于了该服务，并且从服务端得到了返回的服务实例信息
        result = grpcClientProxy.subscribe(serviceName, groupName, clusters);

        //返回从服务端订阅到的服务实例信息
        return result;
    }

}
```

从上面代码块中，可以看到，一旦调用了 NamingClientProxyDelegate 对象的 subscribe 方法之后，就会进一步调用 NamingGrpcClientProxy 对象的 subscribe 方法，接下来就要在 NamingGrpcClientProxy 类中再定义一个 subscribe 方法，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //服务实例的命名空间
    private final String namespaceId;


    //该成员变量封装的就是断线重连后的操作，我愿意称它为重做服务对象
    private final NamingGrpcRedoService redoService;


    //省略其他成内容



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
        //这是操作重试的操作，现在大家也看到服务订阅操作的重试对象是怎么创建的了
        redoService.cacheSubscriberForRedo(serviceName, groupName, clusters);
        return doSubscribe(serviceName, groupName, clusters);
    }

    //在这个方法中，向服务端发送了订阅服务实例的请求
    public ServiceInfo doSubscribe(String serviceName, String groupName, String clusters) throws NacosException {
        //在这里创建订阅服务的请求
        SubscribeServiceRequest request = new SubscribeServiceRequest(namespaceId, groupName, serviceName, clusters,
                true);
        //把请求发送给服务端，得到了服务端回复的响应
        SubscribeServiceResponse response = requestToServer(request, SubscribeServiceResponse.class);
        //程序执行到这里就意味着订阅服务的操作成功了，就要把订阅操作的操作重做对象的registered标志设置为true
        redoService.subscriberRegistered(serviceName, groupName, clusters);
        //返回得到的服务实例信息
        return response.getServiceInfo();
    }



     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法就是把请求发送给服务端的方法，发送请求之后要返回一个响应对象，该方法的第二个参数就指定了要返回的对象的类型
     */
    private <T extends Response> T requestToServer(AbstractNamingRequest request, Class<T> responseClass)
            throws NacosException {
        //定义一个局部变量，接收服务器响应
        Response response = null;
        try {
            //设置请求头，请求头中的信息最终会设置到请求对象中，发送给服务端
            //其实这里只是填充了一个应用名称，也就是appName
            request.putAllHeader(getSecurityHeaders());
            //判断用户是否定义了请求超时时间，如果requestTimeout小于0则意味着没有定义
            //那么发送请求的时候就不必使用超时时间
            //这里就会把请求发送给服务器，并且得到一个响应
            response = requestTimeout < 0 ? rpcClient.request(request) : rpcClient.request(request, requestTimeout);
            //判断响应是否成功，不成功则抛出异常
            if (ResponseCode.SUCCESS.getCode() != response.getResultCode()) {
                throw new NacosException(response.getErrorCode(), response.getMessage());
            }
            //判断响应类型是否与方法指定的参数类型匹配
            if (responseClass.isAssignableFrom(response.getClass())) {
                return (T) response;
            }
            //下面就是异常情况了，大家看看即可
            NAMING_LOGGER.error("Server return unexpected response '{}', expected response should be '{}'",
                    response.getClass().getName(), responseClass.getName());
            throw new NacosException(NacosException.SERVER_ERROR, "Server return invalid response");
        } catch (NacosException e) {
            throw e;
        } catch (Exception e) {
            throw new NacosException(NacosException.SERVER_ERROR, "Request nacos server failed: ", e);
        }
    }

}
```

以上就是我重构之后的代码，现在我再给大家提供一个测试类，请大家看一下目前的注册中心客户端想要从服务端订阅服务实例信息，究竟应该执行什么操作。请看下面代码块。

```
package com.cqfy.nacos.example;



public class NamingExample {

    private static final String INSTANCE_SERVICE_NAME = "cqfy.test.3";

    private static final String INSTANCE_IP = "11.11.11.12";

    private static final int INSTANCE_PORT = 8889;

    private static final String INSTANCE_CLUSTER_NAME = "TEST1";

    public static void main(String[] args) throws NacosException, InterruptedException {

        Properties properties = new Properties();
        //在这里设置了要注册到注册中心的服务实例的地址和命名空间
        properties.setProperty("serverAddr", System.getProperty("serverAddr", "localhost"));
        properties.setProperty("namespace", System.getProperty("namespace", "public"));
        //根据配置类创建了一个NamingService对象，在第一版本中，NamingService对象提供了将服务实例注册到注册中心的功能
        NamingService naming = NamingFactory.createNamingService(properties);

        //在这里发起了注册服务实例到注册中心的操作
        //方法中的这几个参数都是用来构建服务实例对象的，也就是Instance对象
        naming.registerInstance(INSTANCE_SERVICE_NAME, INSTANCE_IP, INSTANCE_PORT, INSTANCE_CLUSTER_NAME);

       
        
        //下面是和服务订阅有关的代码
        NacosNamingService namingService = (NacosNamingService)naming;
        //订阅服务，返回服务实例信息
        ServiceInfo serviceInfo =namingService.subscribe(INSTANCE_SERVICE_NAME, Constants.DEFAULT_GROUP,INSTANCE_CLUSTER_NAME);
        //输出到控制台
        System.out.println("服务实例全部信息==============================================="+serviceInfo.getHosts());

        Thread.sleep(100000);
    }
}
```

到此为止，我就为大家把客户端订阅服务实例的功能实现了，大家应该也明白了客户端订阅服务实例操作的执行流程。那么接下来，我想在提出一个新的问题，请大家想一想：**客户端向服务端订阅了对应的服务实例之后，并且确实得到了封装了所有服务实例信息的 ServiceInfo 对象，那么客户端应该怎么保存这些订阅到的服务实例信息呢**？按照常规思路，客户端肯定要把订阅到的服务实例信息保存起来了，这样当 RPC 远程调用框架的消费者想要执行远程调用操作时，就可以直接从缓存中选取合适的服务地址了。如果不保存订阅到的服务实例信息，这也就意味着远程调用框架消费者每次执行远程调用时，还要再从服务端查询提供了对应服务的服务实例信息，这未免也太麻烦了。所以，**最好的做法就是客户端把从服务端订阅到的服务实例信息全部缓存起来**。当然，这就引出了一个新的问题，那就是客户端缓存的服务实例信息过期了怎么办？这个很好说，实际上，当服务端发现订阅到自己内部的服务实例信息发生了变更，会主动向客户端发送请求，把最新的服务实例信息告诉客户端，这个功能后面我也会为大家实现，现在我们还不用考虑这些。**目前我们要考虑的就是客户端怎么把从服务端订阅到的服务实例信息全部缓存起来**。这个时候，我就要为我们的程序引入一个新的组件了，也就是 ServiceInfoHolder 类。**这个 ServiceInfoHolder 类的对象就保存着客户端从服务端订阅到的所有服务实例的信息，我把它称为服务实例信息缓存器；在 ServiceInfoHolder 类中有一个 map 成员变量，客户端订阅到的所有服务实例信息就都存放在这个 map 中**。接下来，我就先为大家展示一下这个 ServiceInfoHolder 类的具体内容。

## 引入 ServiceInfoHolder 类，实现服务实例信息缓存功能

刚才我说这个 ServiceInfoHolder 类中定义了一个 map 成员变量，其实除了这个成员变量之外，还定义了一些其他的成员变量，**比如说有一个比较重要的文件路径成员变量，也就是 cacheDir。根据这个 cacheDir 文件路径成员变量就可以找到硬盘上对应的文件，而文件的内容就是客户端从服务端订阅到的所有服务实例的信息**。其实讲到这里大家应该也就清楚了，实际上，**当客户端从服务端订阅了服务实例信息之后，不仅会把服务实例信息缓存在 ServiceInfoHolder 对象的 map 中，同时还会对这些服务实例信息进行持久化，就存储在 cacheDir 文件路径对应的文件中。这样一来，当客户端所在的服务器宕机重启之后，可以立刻把订阅的服务实例信息从硬盘加载到内存中，使订阅到的服务实例继续发挥作用，不会直接就丢失了**。了解了这个作用之后，接下来就可以让大家看看 ServiceInfoHolder 类的具体内容了，请看下面代码块。

```
package com.cqfy.nacos.client.naming.cache;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/24
 * @方法描述：该类的对象就是客户端的服务信息保存器，从服务端订阅的所有服务实例的信息都会存放在这个类的对象中
 */
public class ServiceInfoHolder implements Closeable {

    //用来从系统环境变量中得到本地用来存储服务实例信息的文件目录的键
    private static final String JM_SNAPSHOT_PATH_PROPERTY = "JM.SNAPSHOT.PATH";

    //本地存储服务实例信息的文件目录的一部分
    private static final String FILE_PATH_NACOS = "nacos";

    //本地存储服务实例信息的文件目录的一部分
    private static final String FILE_PATH_NAMING = "naming";

    //用来从系统环境变量中得到用户主目录的键
    private static final String USER_HOME_PROPERTY = "user.home";

    //这个map中缓存了从服务端获得的服务实例的信息，把服务实例缓存在内存中，同时也会存储在本地硬盘上
    //string就是服务名称，ServiceInfo封装着该服务下所有服务实例信息
    private final ConcurrentMap<String, ServiceInfo> serviceInfoMap;

    //最终将服务实例信息存放到本地硬盘上的文件路径
    private String cacheDir;

    //构造方法
    public ServiceInfoHolder(String namespace, String notifierEventScope, NacosClientProperties properties) {
        
        //初始化本地存储文件的目录
        initCacheDir(namespace, properties);
        
        //判断是否在启动程序的时候从本地文件先把之前存储的服务实例信息加载到内存中
        if (isLoadCacheAtStart(properties)) {
            //这里就是加载的操作，DiskCache是一个工具类，可以向本地文件写入数据，也可以把数据加载到内存
            this.serviceInfoMap = new ConcurrentHashMap<>(DiskCache.read(this.cacheDir));
        } else {//这里就是不加载的操作
            this.serviceInfoMap = new ConcurrentHashMap<>(16);
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：初始化本地存储文件的路径，这个方法中的操作可能大家硬看也看不出最终目录是什么，大家可以自己打断点调试一下
     */
    private void initCacheDir(String namespace, NacosClientProperties properties) {
        //从配置文件中得到用户自定义的文件目录
        String jmSnapshotPath = properties.getProperty(JM_SNAPSHOT_PATH_PROPERTY);
        String namingCacheRegistryDir = "";
        //如果用户还定义了文件的部分目录，就把这个也拼接到完整的目录中
        if (properties.getProperty(PropertyKeyConst.NAMING_CACHE_REGISTRY_DIR) != null) {
            namingCacheRegistryDir = File.separator + properties.getProperty(PropertyKeyConst.NAMING_CACHE_REGISTRY_DIR);
        }
        //下面就是拼接得到完整文件目录的操作
        if (!StringUtils.isBlank(jmSnapshotPath)) {
            //这里得到的是用户自定义的文件目录
            cacheDir = jmSnapshotPath + File.separator + FILE_PATH_NACOS + namingCacheRegistryDir
                    + File.separator + FILE_PATH_NAMING + File.separator + namespace;
        } else {
            //这里得到的就是默认的文件目录，这里面只有一个是用户自己定义的，那就是namingCacheRegistryDir这个目录
            //如果用户没有自定义任何目录，最后得到的存储服务实例信息的目录就是这样的：${user.home}/nacos/naming/public
            cacheDir = properties.getProperty(USER_HOME_PROPERTY) + File.separator + FILE_PATH_NACOS + namingCacheRegistryDir
                    + File.separator + FILE_PATH_NAMING + File.separator + namespace;
        }
    }



    //是否在程序启动时把本地存储的服务实例信息加载到内存中
    private boolean isLoadCacheAtStart(NacosClientProperties properties) {
        boolean loadCacheAtStart = false;
        if (properties != null && StringUtils.isNotEmpty(properties.getProperty(PropertyKeyConst.NAMING_LOAD_CACHE_AT_START))) {
            loadCacheAtStart = ConvertUtils.toBoolean(properties.getProperty(PropertyKeyConst.NAMING_LOAD_CACHE_AT_START));
        }
        return loadCacheAtStart;
    }




    //获取客户端缓存的所有服务实例信息的方法
    public Map<String, ServiceInfo> getServiceInfoMap() {
        return serviceInfoMap;
    }


    //根据服务信息得到对应的服务实例信息的方法
    public ServiceInfo getServiceInfo(final String serviceName, final String groupName, final String clusters) {
        String groupedServiceName = NamingUtils.getGroupedName(serviceName, groupName);
        String key = ServiceInfo.getKey(groupedServiceName, clusters);
        return serviceInfoMap.get(key);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：这是本类中最重要的方法，就是处理ServiceInfo对象的方法，这个ServiceInfo对象一般都是从服务端返回的
     * 该方法的逻辑很简单，那就是看看新得到的这个ServiceInfo对象相对于本地缓存的ServiceInfo对象是否产生了变化，如果产生变化就要发布服务实例变更事件
     */
    public ServiceInfo processServiceInfo(ServiceInfo serviceInfo) {
        
        //得到ServiceInfo的key
        String serviceKey = serviceInfo.getKey();
        if (serviceKey == null) {
            return null;
        }
        
        //从map中得到客户端缓存的旧的ServiceInfo对象
        ServiceInfo oldService = serviceInfoMap.get(serviceInfo.getKey());
        
        //走到这里就意味着新的ServiceInfo对象没有问题，这时候就要先更新缓存中的ServiceInfo对象
        serviceInfoMap.put(serviceInfo.getKey(), serviceInfo);
        
        //判断新的ServiceInfo对象相对于旧的是否发生了变化
        boolean changed = isChangedServiceInfo(oldService, serviceInfo);
        
        //这里是设置一下新的ServiceInfo对象中的json串，后面将ServiceInfo对象写到本地文件中时要用到
        if (StringUtils.isBlank(serviceInfo.getJsonFromServer())) {
            serviceInfo.setJsonFromServer(JacksonUtils.toJson(serviceInfo));
        }
        
        //判断服务实例是否发生变化
        if (changed) {
            NAMING_LOGGER.info("current ips:({}) service: {} -> {}", serviceInfo.ipCount(), serviceInfo.getKey(),
                    JacksonUtils.toJson(serviceInfo.getHosts()));
            
            //因为ServiceInfo更新了，所以要发送服务实例更新事件，通知订阅了该服务的监听器执行回调方法，但这个功能在第五版本代码才实现了，所以这里先把下面这行代码注释了
            //NotifyCenter.publishEvent(new InstancesChangeEvent(notifierEventScope, serviceInfo.getName(), serviceInfo.getGroupName(), serviceInfo.getClusters(), serviceInfo.getHosts()));
            
            //如果发生了变化，就把最新的ServiceInfo写入到本地文件中，更新本地文件的内容
            DiskCache.write(serviceInfo, cacheDir);
        }
        
        return serviceInfo;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：判断新的ServiceInfo对象相对于旧的ServiceInfo对象是否更新了，这个方法虽然看起来很长，但逻辑非常简单，所以注释就简单点了
     */
    private boolean isChangedServiceInfo(ServiceInfo oldService, ServiceInfo newService) {
        if (null == oldService) {
            NAMING_LOGGER.info("init new ips({}) service: {} -> {}", newService.ipCount(), newService.getKey(),
                    JacksonUtils.toJson(newService.getHosts()));
            return true;
        }
        //如果新ServiceInfo的最新引用事件小于旧ServiceInfo的，就意味着这个ServiceInfo信息不是最新的，直接返回false即可
        if (oldService.getLastRefTime() > newService.getLastRefTime()) {
            NAMING_LOGGER.warn("out of date data received, old-t: {}, new-t: {}", oldService.getLastRefTime(),
                    newService.getLastRefTime());
            return false;
        }
        boolean changed = false;
        //存放旧ServiceInfo对象中的所有服务实例
        Map<String, Instance> oldHostMap = new HashMap<>(oldService.getHosts().size());
        for (Instance host : oldService.getHosts()) {
            oldHostMap.put(host.toInetAddr(), host);
        }
        //存放新ServiceInfo对象中所有服务实例
        Map<String, Instance> newHostMap = new HashMap<>(newService.getHosts().size());
        for (Instance host : newService.getHosts()) {
            newHostMap.put(host.toInetAddr(), host);
        }
        Set<Instance> modHosts = new HashSet<>();
        Set<Instance> newHosts = new HashSet<>();
        Set<Instance> remvHosts = new HashSet<>();
        //下面就是具体对比新旧ServiceInfo对象是否不同的操作
        List<Map.Entry<String, Instance>> newServiceHosts = new ArrayList<>(newHostMap.entrySet());
        for (Map.Entry<String, Instance> entry : newServiceHosts) {
            Instance host = entry.getValue();
            String key = entry.getKey();
            //判断是否为更新的服务实例
            if (oldHostMap.containsKey(key) && !StringUtils.equals(host.toString(), oldHostMap.get(key).toString())) {
                modHosts.add(host);
                continue;
            }
            //判断是否新增了服务实例
            if (!oldHostMap.containsKey(key)) {
                newHosts.add(host);
            }
        }
        //判断是否删除了服务实例
        for (Map.Entry<String, Instance> entry : oldHostMap.entrySet()) {
            Instance host = entry.getValue();
            String key = entry.getKey();
            if (!newHostMap.containsKey(key)) {
                remvHosts.add(host);
            }
        }
        //只要上面的三个判断有一个满足，就意味着ServiceInfo对象更新了
        if (newHosts.size() > 0) {
            //设置发生变化标志
            changed = true;
            NAMING_LOGGER.info("new ips({}) service: {} -> {}", newHosts.size(), newService.getKey(),
                    JacksonUtils.toJson(newHosts));
        }
        if (remvHosts.size() > 0) {
            changed = true;
            NAMING_LOGGER.info("removed ips({}) service: {} -> {}", remvHosts.size(), newService.getKey(),
                    JacksonUtils.toJson(remvHosts));
        }
        if (modHosts.size() > 0) {
            changed = true;
            NAMING_LOGGER.info("modified ips({}) service: {} -> {}", modHosts.size(), newService.getKey(),
                    JacksonUtils.toJson(modHosts));
        }
        return changed;
    }


    @Override
    public void shutdown() throws NacosException {
        String className = this.getClass().getName();
        NAMING_LOGGER.info("{} do shutdown begin", className);
        NAMING_LOGGER.info("{} do shutdown stop", className);
    }

}
```

上面代码块的内容虽然比较多，但是每个方法都很简单，**最核心的方法就是 processServiceInfo 方法，这个方法接收一个封装了所有服务实例信息的 ServiceInfo 对象，在这个方法中会把这个 ServiceInfo 对象缓存到成员变量 map 中，这就完成了客户端缓存服务实例信息的操作；然后会对比新接收到的服务实例信息和缓存的旧的服务实例信息是否一致，如果不一致就意味着在服务端那边发生了服务信息变更，这个时候就要把新的服务实例信息写入到本地文件中，更新本地文件的内容**。照这么分析下来，大家应该也能想到了，**ServiceInfoHolder 对象的 processServiceInfo 方法肯定是在客户端接收到服务端回复的最新服务实例信息后被调用**，那这个怎么体现在代码中呢？还有一点，那就是 ServiceInfoHolder 对象在哪里被创建了呢？接下来就请大家带着这两个疑问，看看再次被我重构之后的代码。

首先给大家展示 ServiceInfoHolder 对象被创建的代码，实际上，这个 ServiceInfoHolder 对象是 NacosNamingService 类的成员变量，具体创建的操作如下所示，请看下面代码块。

```
package com.cqfy.nacos.client.naming;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：这个类是注册中心客户端的核心类，可以说是注册服务实例，订阅服务实例等等操作的入口类，很多重要的操作都是由这个类的对象发起的
 * 但在第一版本代码中，该类只提供了注册服务实例到注册中心的方法
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosNamingService implements NamingService {



    
    //从我在测试类为大家提供的代码，大家也能看出来，实际上NacosNamingService类的对象是通过反射创建的
    //反射调用该类的构造方法，并且调用的是参数为Properties的构造方法，而在测试类的Properties中，设置了要创建的NacosNamingService对象
    //的命名空间，这就意味着每一个NacosNamingService对象都有自己的命名空间，下面这个成员变量表示的就是当前NacosNamingService对象的命名空间，在测试类中为创建的
    //NacosNamingService对象设置了默认的命名空间，也就是public
    private String namespace;

    //代理客户端，该客户端就是用来向注册中心服务器发送请求的
    //这里之所以叫代理客户端，是因为NamingClientProxy内部真正向服务器发送消息时，使用的是grpc构建的客户端
    private NamingClientProxy clientProxy;


    //服务实例信息保存器，当客户端向服务端订阅了某个服务之后，这个服务下的服务实例信息都会传输给客户端
    //客户端拿到这些服务实例信息后就会存放在该成员变量中
    private ServiceInfoHolder serviceInfoHolder;

    //通知事件的唯一标识，这个标识在第四版本代码还用不到，大家可以暂时忽略第五版本代码就会用到了
    private String notifierEventScope;


    //其他内容省略


     //构造方法，该构造方法通过Properties文件创建NacosNamingService对象，反射调用的就是这个构造方法
    public NacosNamingService(Properties properties) throws NacosException {
        init(properties);
    }


    //初始化NacosNamingService对象的方法，在该方法对NacosNamingService内部要使用的一些组件进行了创建和初始化
    private void init(Properties properties) throws NacosException {
        
        //异步加载Jackson组件
        PreInitUtils.asyncPreLoadCostComponent();
        
        //创建一个Nacos客户端的配置文件类
        final NacosClientProperties nacosClientProperties = NacosClientProperties.PROTOTYPE.derive(properties);
        
        //给当前NacosNamingService对象的命名空间赋值
        this.namespace = InitUtils.initNamespaceForNaming(nacosClientProperties);

        //生成通知事件唯一标识
        this.notifierEventScope = UUID.randomUUID().toString();
        
        //创建服务实例信息保存器
        this.serviceInfoHolder = new ServiceInfoHolder(namespace, this.notifierEventScope, nacosClientProperties);
        
        //创建代理客户端对象
        this.clientProxy = new NamingClientProxyDelegate(this.namespace,serviceInfoHolder,nacosClientProperties);
    }

    


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：这个就是订阅服务信息的方法，这个方法是我自己写的，第五版本会重构，一般来说，这个方法都会传入一个监听器，监听订阅的服务实例的变更情况
     */
    public ServiceInfo subscribe(String serviceName, String groupName, String clusters) throws NacosException {
        return clientProxy.subscribe(serviceName, groupName, clusters);
    }

}
```

可以看到，ServiceInfoHolder 对象首先在 NacosNamingService 对象的 init 方法中被创建了。现在 NacosNamingService 类的 init 方法中又展示了一个重要组件。当然，紧接着我们也会看到，刚刚创建好的 ServiceInfoHolder 对象传递给了 NamingClientProxyDelegate 对象的构造方法，这就意味着 ServiceInfoHolder 对象实际上也是 NamingClientProxyDelegate 类的一个成员变量。接下来，就请大家看一看重构之后的 NamingClientProxyDelegate 类，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote;



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


    public NamingClientProxyDelegate(String namespace, ServiceInfoHolder serviceInfoHolder,NacosClientProperties properties) throws NacosException {

        this.serverListManager = new ServerListManager(properties, namespace);
        this.serviceInfoHolder = serviceInfoHolder;
        this.grpcClientProxy = new NamingGrpcClientProxy(namespace, serverListManager, properties);
    }


    //其他内容省略




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
        
       
        //根据key从serviceInfoHolder中得到对应的服务实例信息，这里可以看到，优先去服务实例保存器中获得了一次
        ServiceInfo result = serviceInfoHolder.getServiceInfoMap().get(serviceKey);
       
        //如果获取不到，或者判断客户端并没有订阅对应服务
        if (null == result || !isSubscribed(serviceName, groupName, clusters)) {
            //在这里就真正订阅了该服务，并且从服务端得到了返回的服务实例信息
            result = grpcClientProxy.subscribe(serviceName, groupName, clusters);
        }
        
        //把得到的服务实例信息交给服务实例保存器保管
        serviceInfoHolder.processServiceInfo(result);
        return result;
    }

}
```

从上面的代码块可以看到，NamingClientProxyDelegate 对象的 subscribe 方法重构了。重构之后的 subscribe 方法逻辑非常清晰，**如果客户端想从服务器订阅某些服务实例的信息，首先会从本地的 serviceInfoHolder 缓存中查看有没有对应的服务实例信息，如果有就说明这些服务实例已经被订阅了，不用重复订阅；如果没有再执行真正订阅服务实例的方法，然后服务端会返回对应的服务实例信息，这些服务实例信息会被封装在 ServiceInfo 对象中；接下来就调用了 serviceInfoHolder 对象的 processServiceInfo 方法，处理从服务端返回的最新的服务实例信息**。到此为止，大家应该就清楚了 ServiceInfoHolder 服务实例信息缓存器对象的创建时机，以及该对象的 processServiceInfo 方法的调用时机。

实际上，ServiceInfoHolder 对象的 processServiceInfo 方法不仅仅只有这一个调用时机，请大家想一想，现在我们实现的都是客户端主动订阅服务实例信息的功能，也就是说是客户端主动去服务端拉去服务实例信息；但实际上，**客户端订阅服务实例信息成功之后，服务端在发现对应的服务实例发生变更了，会主动把最新的服务实例信息推送给客户端，当客户端接收到服务端推送过来的最新服务实例信息时，也会调用 ServiceInfoHolder 对象的 processServiceInfo 方法，对接收到的服务实例进行处理**。这个具体操作就定义在一个新的客户端处理器中，**也就是 NamingPushRequestHandler 处理器。这个处理器专门用来处理服务端主动推送过来的服务实例信息**，这个处理器本来应该定义在第四版本代码中，但是我迭代代码的时候把代码弄混了，把这个处理器放到第五版本代码中了，所以大家想看的话，可以直接去第五版本代码中查看。当然，现在大家已经完全可以去查看第四版本代码了。接下来，我为大家把 NamingPushRequestHandler 处理器的内容展示一下，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/24
 * @方法描述：处理服务端主动推送服务实例信息请求的处理器
 */
public class NamingPushRequestHandler implements ServerRequestHandler {

    //这个处理器肯定要持有服务实例保存器，因为这个处理器会接收到从服务端发送过来的最新的ServiceInfo对象
    //这个对象要交给serviceInfoHolder去处理，看看服务实例信息是否更新了，如果更新了还要更新缓存和本地文件中的服务实例信息
    private final ServiceInfoHolder serviceInfoHolder;


    public NamingPushRequestHandler(ServiceInfoHolder serviceInfoHolder) {
        this.serviceInfoHolder = serviceInfoHolder;
    }


    @Override
    public Response requestReply(Request request, Connection connection) {
        //判断服务端发送过来的是否为NotifySubscriberRequest请求
        if (request instanceof NotifySubscriberRequest) {
            NotifySubscriberRequest notifyRequest = (NotifySubscriberRequest) request;
            //从请求中得到ServiceInfo对象，交给服务实例保存器去处理
            serviceInfoHolder.processServiceInfo(notifyRequest.getServiceInfo());
            //回复服务端响应
            return new NotifySubscriberResponse();
        }
        return null;
    }
}
```

上面就是 NamingPushRequestHandler 处理器的全部内容，至于这个处理器在哪里被创建，又是怎么添加到客户端处理器集合中，这些操作就留给大家自己去第五版本代码中查看吧，这部分逻辑非常简单，我就不在文章中展开讲解了。

好了，到此为止，本章的内容其实就该结束了，客户端从服务端订阅服务实例的功能实现了，对订阅到的服务实例信息进行缓存以及本地持久化功能也实现了，甚至我也把客户端如何处理服务端主动推送过来的最新服务实例信息的处理器也展示了。内容讲解得也差不多了，这一章本应该结束了。但是，从文章标题就能看出来，**还剩下最后一个功能没有为大家实现，就是引入这个 ServiceInfoUpdateService 组件**。

## 引入 ServiceInfoUpdateService 组件，实现客户端服务实例信息定期刷新功能

我们先不讲解这个 ServiceInfoUpdateService 组件的具体作用，光从名字上来看，我们大家能猜到这个组件可能是用来更新客户端保存的服务实例信息的。那么在什么情况下，客户端要主动更新自己缓存的服务实例信息呢？客户端明明已经从服务端订阅了对应的服务实例信息，之后如果服务端对应的服务实例信息发生了变化，服务端会主动把最新的服务实例信息推送给客户端，到时候客户端就会更新本地缓存的服务实例信息了。这个功能的机制肯定是健全的，并且刚才已经通过 ServiceInfoHolder 对象的 processServiceInfo 方法给大家展示了。那既然有了这么健全的机制，为什么客户端还要搞一个 ServiceInfoUpdateService 组件，来主动更新客户端缓存的服务实例信息呢？

这是因为在 nacos 客户端还有一个机制，那就是客户端缓存的服务实例信息都有一个缓存时间，也许是担心服务端推送并不及时吧，总之，**客户端缓存的服务实例信息只要超过了缓存时间，客户端都会主动去服务端查看一下，看看订阅的服务实例信息是否发生了变更。其实就是从服务端拉取自己订阅的服务实例信息，然后把拉取到的服务实例信息 ServiceInfoHolder 对象的 processServiceInfo 方法处理。如果服务实例信息发生变更了，就更新客户端缓存和本地文件的内容**。这时候大家应该也是到了，**这个功能其实就是一个定时任务，客户端缓存的服务实例的缓存有效时间，就是定时任务的执行周期。如果服务实例的缓存有效时间是 1 分钟，那这个定时任务就会每隔 1 分钟执行一次，去服务端拉取一次订阅的服务实例的最新信息**。而这个定时任务就定义在 ServiceInfoUpdateService 类中，并且 ServiceInfoUpdateService 类中还有一个定时任务执行器成员变量，每当客户端向服务端订阅了某些服务实例的信息时，这个 ServiceInfoUpdateService 对象就会为这些服务实例信息创建一个对应的定时任务，并且把定时任务提交给定时任务执行器执行。接下来，我就给大家展示一下这个 ServiceInfoUpdateService 类的具体内容，请看下面代码块。

```
package com.cqfy.nacos.client.naming.core;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/24
 * @方法描述：服务信息更新器，这个更新器会在客户端中定期异步主动从服务端查询所订阅的所有服务下的服务实例是否发生了变更，并且在变更后更新本地服务信息
 */
public class ServiceInfoUpdateService implements Closeable {

    //定时任务默认的延迟时间
    private static final long DEFAULT_DELAY = 1000L;

    //计算下一次定时任务执行时间的赋值属性
    private static final int DEFAULT_UPDATE_CACHE_TIME_MULTIPLE = 6;

    //存储定时任务的map，key就是订阅的服务名称，value就是对应的定时任务
    //如果客户端订阅了多个服务的话，就会有多个定时任务定期从服务器查询这些服务信息是否更新了
    private final Map<String, ScheduledFuture<?>> futureMap = new HashMap<>();

    //服务实例信息保存器
    private final ServiceInfoHolder serviceInfoHolder;

    //定时任务执行器
    private final ScheduledExecutorService executor;

    //客户端代理对象
    private final NamingClientProxy namingClientProxy;

    //是否开启异步查询服务端服务实例信息的标志
    private final boolean asyncQuerySubscribeService;




    //构造方法
    public ServiceInfoUpdateService(NacosClientProperties properties, ServiceInfoHolder serviceInfoHolder,
                                    NamingClientProxy namingClientProxy, InstancesChangeNotifier changeNotifier) {
        //在这里给asyncQuerySubscribeService赋值，如果为true，那么定时任务执行器就会开始工作，定期异步访问服务端
        this.asyncQuerySubscribeService = isAsyncQueryForSubscribeService(properties);

        //创建定时任务执行器
        this.executor = new ScheduledThreadPoolExecutor(initPollingThreadCount(properties),
                new NameThreadFactory("com.alibaba.nacos.client.naming.updater"));
        
        this.serviceInfoHolder = serviceInfoHolder;
        
        this.namingClientProxy = namingClientProxy;
    }



    //给asyncQuerySubscribeService赋值的方法
    private boolean isAsyncQueryForSubscribeService(NacosClientProperties properties) {
        //这里可以看到，是否开启异步访问服务端的操作是可以由用户自己设置的
        //如果在配置文件中定义了PropertyKeyConst.NAMING_ASYNC_QUERY_SUBSCRIBE_SERVICE键的value为true，就会开启操作
        if (properties == null || !properties.containsKey(PropertyKeyConst.NAMING_ASYNC_QUERY_SUBSCRIBE_SERVICE)) {
            return false;
        }//这里是把配置文件中字符串转换成bool的操作
        return ConvertUtils.toBoolean(properties.getProperty(PropertyKeyConst.NAMING_ASYNC_QUERY_SUBSCRIBE_SERVICE),
                false);
    }

    //得到定时任务执行器的线程数量，也是可以从配置中获取，用户不定义，就是用默认值
    private int initPollingThreadCount(NacosClientProperties properties) {
        if (properties == null) {
            return UtilAndComs.DEFAULT_POLLING_THREAD_COUNT;
        }
        return ConvertUtils.toInt(properties.getProperty(PropertyKeyConst.NAMING_POLLING_THREAD_COUNT),
                UtilAndComs.DEFAULT_POLLING_THREAD_COUNT);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：向futureMap中添加对应服务的定时任务的方法
     */
    public void scheduleUpdateIfAbsent(String serviceName, String groupName, String clusters) {
        //这里判断了一下，如果没开启异步查询服务，就直接退出当前方法
        if (!asyncQuerySubscribeService) {
            return;
        }
        //根据服务信息得到key
        String serviceKey = ServiceInfo.getKey(NamingUtils.getGroupedName(serviceName, groupName), clusters);
        //判断futureMap中是否已经存在对应的定时任务了
        if (futureMap.get(serviceKey) != null) {
            return;
        }
        synchronized (futureMap) {
            //二次判断
            if (futureMap.get(serviceKey) != null) {
                return;
            }
            //没有则创建对应的定时任务，然后直接把定时任务提交给定时任务执行器
            ScheduledFuture<?> future = addTask(new UpdateTask(serviceName, groupName, clusters));
            futureMap.put(serviceKey, future);
        }
    }

    //创建并提交定时任务的方法
    private synchronized ScheduledFuture<?> addTask(UpdateTask task) {
        return executor.schedule(task, DEFAULT_DELAY, TimeUnit.MILLISECONDS);
    }




     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/24
     * @方法描述：这个内部类的对象就封装了具体要执行的定时任务的操作
     */
    public class UpdateTask implements Runnable {


        //这个类的具体内容暂时省略
    }




}
```

上面代码块展示的就是 ServiceInfoUpdateService 类的具体内容，我相信这个类的各个成员变量以及各个方法都很容易理解，**唯一需要注意的方法就是 scheduleUpdateIfAbsent 方法，该方法就是向 futureMap 成员变量中添加定时任务的方法**。现在大家看这个方法可能还有点懵，不知道这个方法具体是怎么发挥作用的，但是刚才我说了，当客户端向服务端订阅某些服务实例信息时，就会为订阅的这些服务实例信息创建一个定时任务，所以，接下来我直接为大家展示一下 ServiceInfoUpdateService 方法的调用时机，大家就明白这个方法的具体作用了。这个方法是在 NamingClientProxyDelegate 对象的 subscribe 方法中被调用的，具体实现请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote;



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
        //现在大家也可以看到，这个ServiceInfoUpdateService组件定义在了NamingClientProxyDelegate中
        //并且就是在NamingClientProxyDelegate的构造方法中创建的
        this.serviceInfoUpdateService = new ServiceInfoUpdateService(properties, serviceInfoHolder, this, changeNotifier);
        this.serverListManager = new ServerListManager(properties, namespace);
        this.serviceInfoHolder = serviceInfoHolder;
        this.grpcClientProxy = new NamingGrpcClientProxy(namespace, serverListManager, properties,serviceInfoHolder);
    }


    //其他内容省略




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


        //在这里调用了serviceInfoUpdateService对象的scheduleUpdateIfAbsent方法
        //客户端还要实时主动关注服务端的服务实例信息是否发生了变化，其实就是为serviceName, groupName, clusters下的
        //服务实例信息创建了一个定时任务，serviceInfoUpdateService组件可以根据订阅到的服务实例的有效缓存时间，定期执行任务去服务端拉取最新的服务实例信息
        serviceInfoUpdateService.scheduleUpdateIfAbsent(serviceName, groupName, clusters);



        
        //根据key从serviceInfoHolder中得到对应的服务实例信息，这里可以看到，优先去服务实例保存器中获得了一次
        ServiceInfo result = serviceInfoHolder.getServiceInfoMap().get(serviceKey);
       
        //如果获取不到，或者判断客户端并没有订阅对应服务
        if (null == result || !isSubscribed(serviceName, groupName, clusters)) {
            //在这里就真正订阅了该服务，并且从服务端得到了返回的服务实例信息
            result = grpcClientProxy.subscribe(serviceName, groupName, clusters);
        }
        
        //把得到的服务实例信息交给服务实例保存器保管
        serviceInfoHolder.processServiceInfo(result);
        return result;
    }

}
```

看过了上面的代码块之后，现在大家应该清楚这个 ServiceInfoUpdateService 组件的真正作用了。当然，最核心的操作肯定在 UpdateTask 这个定时任务中，但我在文章中并不会在为大家展示这个定时任务的内容了，定时任务要执行的大概逻辑刚才已经分析过了，所以我就不讲解了。大家直接去看代码即可，和 NamingPushRequestHandler 处理器一样，我迭代代码的时候把代码弄混了，把这个 ServiceInfoUpdateService 组件也放到第五版本代码中了，所以大家想看的话，可以直接去第五版本代码中查看。当然，这个 UpdateTask 定时任务中还有一些其他的细节，比如大家可以先想想，**执行定时任务的时候有必要每一次都去服务端拉取最新的服务实例信息吗？假如服务端刚刚把最新的服务实例信息推送给客户端，客户端还有必要紧接着执行拉取最新服务实例信息的操作吗？那客户端怎么就知道自己持有的服务实例信息是服务端最新推送过来的呢？这就要从封装所有服务实例信息的 ServiceInfo 对象的 lastRefTime 最新引用时间这个成员变量入手了**。更具体的细节我就不再讲解了，所有细节我都写在代码注释中了，大家直接查看代码注释也可以看懂。好了朋友们，又一章结束了，下一章我将为大家实现整个 nacos 框架中最核心的一个功能机制，那就是时间通知机制，没有这个机制，nacos 框架就无法正常运转。我们下一章见！