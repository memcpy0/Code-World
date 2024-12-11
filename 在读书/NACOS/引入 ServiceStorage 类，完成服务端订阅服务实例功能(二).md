上一章我已经为大家引入了 ClientServiceIndexesManager 这个组件，这个组件维护了客户端订阅和注册服务的关键信息，也就是说，现在只要给定了一个服务信息，也就是 Service 对象，根据我们上一章定义完毕的 ClientServiceIndexesManager 组件，nacos 服务端就可以清楚地知道有哪些客户端向 Service 这个服务下注册了服务实例信息，有哪些客户端订阅了 Service 服务下的所有服务实例信息。回顾到这里，我不知道大家对之前章节遗留的一个问题还有印象吗，在第 14 章，我为大家引入 ConnectionManager 连接管理器组件的时候，当时也引入很多其他组件，比如双向流、单向流请求接收器等等。总之当时我引入了非常多的组件，但是那些组件在一开始都不完善，因此随着组件的引入，我也提出了很多新的问题，随着问题的解决，我们引入的新组件也会不断得到完善。我把在第 14 章提出的 5 个问题搬运到这里了，请大家简单看看：

**1 服务端怎么给接收到的每一个客户端连接建立唯一连接 Id 呢？**

**2 服务端想给客户端 B 主动发送消息时，怎么就知道客户端 B 的连接 Id 是多少呢？**

**3 存放客户端连接 Id 和对应的连接对象的 map 究竟是什么呢？难道就只是一个普通的 map 吗？这个 map 应该定义在哪里呢？**

**4 客户端和服务端建立的连接通道，也就是所谓的连接对象，究竟是什么呢？**

**5 客户端的连接 Id 和对应的连接对象在什么时候被放到 map 中呢？**

如果大家还有印象的话，一定还记得，在以上的 5 个问题中，只有第 2 个问题迟迟没有得到解决，其他 4 个问题早就解决了。而我在之前也为大家简单解释过，就在第 15 章里，请看下面图片。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1723776839329-a5c2718e-6ab7-4e32-bf30-57fedbc42422.png)

在第 15 章的时候，我就跟大家说了要想解决上面的第 2 个问题，就必须先引入 ClientServiceIndexesManager 组件。现在我们已经把 ClientServiceIndexesManager 组件引入了，我相信对大家来说，第 2 个问题也就顺利解决了。**因为我们引入的这个组件维护了服务信息与客户端连接 Id 的映射关系，根据 Service 对象完全可以知道是哪些客户端注册或者订阅了该服务下的服务实例信息，这样一来 nacos 服务端不就能得到相对应的客户端连接 Id，然后再根据客户端连接 Id 得到客户端连接对象，然后就可以使用连接对象向 nacos 客户端主动发送消息**。这个流程已经十分清晰了，我想大家应该都能理解了。

## 引入 ServiceStorage 存储器，实现服务信息缓存功能

好了，遗留了很多章节的问题终于得到解决了，但是大家也别忘了，我们上一章最终的目的是什么呢？我们引入 ClientServiceIndexesManager 组件，或者说实现了 ClientServiceIndexesManager 类，只能说我们完成了 nacos 服务端订阅服务实例功能最关键的一步，因为在 nacos 服务端把订阅了服务实例信息的客户端信息存储完毕之后，还要把客户端订阅的 Service 下的所有服务实例信息搜集出来，然后返回给 nacos 客户端。上一章我们仅仅是把 ClientServiceIndexesManager 组件引入了，并没有真正的把订阅了 Service 服务下的所有服务实例信息返回给客户端。所以这一章我们就要着手解决这个问题，**也就是要把某个 Service 服务下的所有服务实例查找出来返回给订阅了该服务的客户端**。

那么该怎么实现这个功能呢？我相信经过上面的简单分析，大家已经心里有数了。ClientServiceIndexesManager 组件已经定义完毕了，有了这个组件，**我们就可以准确地知道哪些 nacos 客户端向 Service 服务下注册了服务实例信息，也就是可以得到相对应的客户端连接 Id 的集合，得到了这些客户端连接 Id，就可以得到各自的 ConnectionBasedClient 客户端对象，然后从 ConnectionBasedClient 客户端对象中根据 Service 得到 nacos 客户端注册的服务实例信息**。这样一来，nacos 服务端就可以把各个客户端注册的服务实例信息返回给订阅了 Service 服务的客户端了。这个流程大家都可以理解吧？

如果上面的流程大家都理解了，那我们现在就可以直接使用 ClientManager 客户端管理器，根据客户端连接 Id 得到对应的 ConnectionBasedClient 客户端对象，然后从 ConnectionBasedClient 客户端对象中根据 Service 得到 nacos 客户端注册的服务实例即可。但是这么做还是存在一个小小的弊端，请大家想一想，**根据以上流程，我们执行的操作最终会得到某个 Service 服务下的所有服务实例的信息，这些服务实例信息可以封装到一个 ServiceInfo 对象中**。**如果要把某个 Service 服务下的所有服务实例信息返回给订阅了该 Service 服务的 nacos 客户端，那就可以把 ServiceInfo 对象封装到响应中回复给客户端**。这个流程大家应该也能理清楚吧？那现在问题就来了，请大家想一想，通过以上操作，**我们就可以得到一个 Service——ServiceInfo 的键值对，也就是得到了该服务下所有的服务实例信息，那么除了要把最终得到的 ServiceInfo 返回给订阅了该 Service 的 nacos 客户端，我们还需要对这个 ServiceInfo 做些什么吗**？

在真正的生产环境中，可能并不是只有一个 nacos 客户端会订阅某个 Service 服务，假如有多个 nacos 客户端都订阅了 Service 服务，并且订阅的先后顺序也不相同，那么每当有客户端向服务端订阅相同的 Service 服务时，难道 nacos 服务端都要重新执行一遍查找 Service 服务下所有服务实例信息的操作吗？ 这样未免有些麻烦，**假如我们把查询得到的 Service——ServiceInfo 以键值对的方式缓存起来，那么当 nacos 客户端订阅了 Service 服务时，就可以直接从缓存中返回该 Service 服务下所有服务实例的信息了**。这样岂不是更方便？所以，为了实现这个功能，我想再引入一个新的组件，**那就是 ServiceStorage，翻译过来就是服务信息存储器的意思**。我可以在这个 ServiceStorage 存储器中先定义一个存放 Service——ServiceInfo 键值对的 map，这个 map 就用来缓存服务实例信息。具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.index;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/20
 * @方法描述：服务信息存储器
 */
@Component
public class ServiceStorage {


    //存放服务和服务下所有服务实例信息的map
    private final ConcurrentMap<Service, ServiceInfo> serviceDataIndexes;

    
}
```

可以看到，我们新引入的这个 ServiceStorage 也添加了 Springboot 的 @Component 注解，这说明它也是被 srpingboot 容器管理的 bean 对象；我还在它内部定义了一个 serviceDataIndexes 成员变量，它是一个 map，专门用来缓存服务实例信息。现在 ServiceStorage 存储器非常简单，这也就意味着它还存在很多问题，比如最重要的一个问题就是：**我们怎么把 Service——ServiceInfo 数据添加到其内部定义的 map 中**。如果是单看这一个问题，那还挺容易解决的，刚才我们已经分析过思路了，不管怎么样，**nacos 服务端要想得到 Service——ServiceInfo 键值对信息，最终肯定是要使用 ClientServiceIndexesManager 组件，有了这个组件，我们就可以准确地知道哪些 nacos 客户端向 Service 服务下注册了服务实例信息，也就是可以得到相对应的客户端连接 Id 的集合，得到了这些客户端连接 Id，就可以使用 ClientManager 客户端管理器得到各自的 ConnectionBasedClient 客户端对象，然后从 ConnectionBasedClient 客户端对象中根据 Service 得到 nacos 客户端注册的服务实例信息**。这个流程大家肯定都很清楚了，既然最终要执行这个操作，而执行这个操作的过程中要用到 ClientServiceIndexesManager 和 ClientManager 组件，那我不防就把这两个组件定义成 ServiceStorage 类的成员变量，反正这两个组件的对象都是被 springboot 管理的 bean 对象，是可以被自动注入的。然后我们可以再为 ServiceStorage 存储器定义一些相关方法，这些方法就会根据 Service 服务信息最终查询到该服务下的所有服务实例信息，然后把这些服务实例信息存放到 serviceDataIndexes 成员变量中。我已经按照刚才的分析把 ServiceStorage 类重构完毕了，具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.index;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/20
 * @方法描述：服务信息存储器
 */
@Component
public class ServiceStorage {


    //存放服务和服务下所有服务实例信息的map
    //这里我还要多补充一句，并不仅仅是客户端向服务端订阅服务实例信息时，会从这个缓存器中把服务实例信息回复给客户端
    //不知道大家还记得nacos客户端会向服务端发送的ServiceQueryRequest请求吗？大家可以自己回顾一下这个请求的发送时机
    //简单来说nacos客户端会定期向服务端查看最新的服务实例信息，也就是拉取最新的服务实例信息
    //这时候就会发送ServiceQueryRequest请求，这个请求会被nacos服务端的ServiceQueryRequestHandler请求处理器处理
    //我在第八版本代码中也提供了这个请求处理器，这个请求处理器在处理ServiceQueryRequest请求时，就会从当前的缓存器中
    //把Service下的所有服务实例信息返回给客户端
    private final ConcurrentMap<Service, ServiceInfo> serviceDataIndexes;

    //客户端服务信息索引管理器，存储器对象会先从索引管理器中得到对应的客户端信息
    //然后再从服务端为客户端创建的客户端对象中获得具体的服务实例信息
    private final ClientServiceIndexesManager serviceIndexesManager;

    //客户端管理器
    private final ClientManager clientManager;


    //构造方法
    public ServiceStorage(ClientServiceIndexesManager serviceIndexesManager, ClientManagerDelegate clientManager) {
        this.serviceIndexesManager = serviceIndexesManager;
        this.clientManager = clientManager;
        //在这里创建了serviceDataIndexes对象
        this.serviceDataIndexes = new ConcurrentHashMap<>();
    }

    
    //以下方法按顺序阅读即可，我就是按照逻辑上的先后顺序展示各个方法的

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：本类非常重要的方法，得到该服务下所有服务实例信息的方法
     */
    public ServiceInfo getData(Service service) {
        //如果serviceDataIndexes中不包含对应的服务实例信息，那就调用getPushData()方法，从客户端对象中获取
        //如果包含，就直接从缓存中获取即可
        return serviceDataIndexes.containsKey(service) ? serviceDataIndexes.get(service) : getPushData(service);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：获取指定服务下所有服务实例信息的方法
     */
    public ServiceInfo getPushData(Service service) {
        //得到一个空的ServiceInfo对象，这个ServiceInfo对象已经设置了服务的元信息
        //但是具体的服务实例信息还没有设置
        ServiceInfo result = emptyServiceInfo(service);
        //如果服务管理器中不包含该服务元信息对象，则直接退出当前方法
        if (!ServiceManager.getInstance().containSingleton(service)) {
            return result;
        }
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        //这里就是通过getAllInstancesFromIndex()方法得到该服务下所有服务实例信息的方法
        result.setHosts(getAllInstancesFromIndex(singleton));
        //把得到的最新的服务实例信息缓存到serviceDataIndexes中
        serviceDataIndexes.put(singleton, result);
        return result;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：得到一个空的ServiceInfo对象的方法，ServiceInfo对象已经设置了服务名称，服务所在组名称，以及服务在客户端的缓存时间
     */
    private ServiceInfo emptyServiceInfo(Service service) {
        ServiceInfo result = new ServiceInfo();
        result.setName(service.getName());
        result.setGroupName(service.getGroup());
        //在这里设置了要传输给客户端的ServiceInfo对象的最新引入时间
        result.setLastRefTime(System.currentTimeMillis());
        //在这里设置了ServiceInfo对象的缓存有效时间，这些时间在客户端的ServiceInfoUpdateService组件中都会用到
        result.setCacheMillis(switchDomain.getDefaultPushCacheMillis());
        return result;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：得到该服务下的所有服务实例信息的方法
     */
    private List<Instance> getAllInstancesFromIndex(Service service) {
        Set<Instance> result = new HashSet<>();
        Set<String> clusters = new HashSet<>();
        //先从服务索引管理器中得到该服务下注册的所有服务实例的客户端Id
        for (String each : serviceIndexesManager.getAllClientsRegisteredService(service)) {
            //遍历每一个客户端Id，得到当前客户端Id的客户端注册的服务实例信息
            Optional<InstancePublishInfo> instancePublishInfo = getInstanceInfo(each, service);
            //判断instancePublishInfo是否非空，非空则意味着当前遍历到的客户端注册了Serivce服务下的服务实例
            if (instancePublishInfo.isPresent()) {
                InstancePublishInfo publishInfo = instancePublishInfo.get();
                //判断服务实例是不是BatchInstancePublishInfo类型的
                if (publishInfo instanceof BatchInstancePublishInfo) {
                    BatchInstancePublishInfo batchInstancePublishInfo = (BatchInstancePublishInfo) publishInfo;
                    //如果是BatchInstancePublishInfo类型的，则解析这些服务实例，把解析到的服务实例信息封装到集合中
                    List<Instance> batchInstance = parseBatchInstance(service, batchInstancePublishInfo, clusters);
                    //把得到的服务实例信息添加到上面创建好的set集合中
                    result.addAll(batchInstance);
                } else {
                    //走到这里意味着从客户端得到的服务实例对象并不是BatchInstancePublishInfo类型的
                    //这里就直接得到Instance对象
                    Instance instance = parseInstance(service, instancePublishInfo.get());
                    //把该对象添加到set集合中
                    result.add(instance);
                }
            }
        }
        //返回封装了所有服务实例信息的集合
        return new LinkedList<>(result);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：得到指定InstancePublishInfo对象的方法
     */
    private Optional<InstancePublishInfo> getInstanceInfo(String clientId, Service service) {
        //根据客户端Id得到对应的客户端对象
        Client client = clientManager.getClient(clientId);
        if (null == client) {
            return Optional.empty();
        }//从客户端对象中得到对应的InstancePublishInfo对象
        return Optional.ofNullable(client.getInstancePublishInfo(service));
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/21
     * @方法描述：根据InstancePublishInfo对象得到Instance对昂的方法
     */
    private Instance parseInstance(Service service, InstancePublishInfo instanceInfo) {
        //在这里把InstancePublishInfo对象解析成了一个Instance对象
        //这时候得到的Instance对象的成员变量几乎都已经赋值了
        Instance result = InstanceUtil.parseToApiInstance(service, instanceInfo);
        //接下来是处理该服务实例元数据的操作，该操作暂时还不必关心，也是要等到引入集群之后，才能真正执行这里的操作，现在服务实例对象的元数据都是空
        //无法在web控制台界面编辑
        Optional<InstanceMetadata> metadata = metadataManager.getInstanceMetadata(service, instanceInfo.getMetadataId());
        //在这里就是把用户在控制台web界面编辑的服务实例元数据设置到了Instance对象的metadata中
        metadata.ifPresent(instanceMetadata -> InstanceUtil.updateInstanceMetadata(result, instanceMetadata));
        return result;
    }
    
}
```

以上就是我重构完整的 ServiceStorage 类，因为之前已经对该类的重构流程详细分析过了，并且代码块中注释非常详细，所以我就不再带着大家梳理上面代码的逻辑了。如果大家详细阅读了上面代码块的内容，并且掌握了其工作流程，大家就会知道：**只要 nacos 服务端调用 ServiceStorage 存储器的 getData(Service service) 方法，那么 Service——ServiceInfo 键值对的信息就可以存放到 ServiceStorage 存储器内部的 map 中**。那现在问题就来了，存储器的 getData(Service service) 方法什么时候会被调用呢？这个就没什么可讨论的了，**当然是在订阅了 Service 服务的 nacos 客户端第一次向服务端执行订阅操作时调用该方法**，**这样一来之后再有 nacos 客户端向服务端订阅了相同的 Service 服务，nacos 服务端就可以直接从 ServiceStorage 缓存器中把缓存的服务实例信息返回给客户端了**。那么具体执行操作的时机是哪里呢？这个也很好说，大家应该还对我们上一章实现的 SubscribeServiceRequestHandler 请求处理器有印象，这个处理器就是 nacos 服务端专门用来处理客户端发送的订阅服务实例的请求的。在这个请求处理器中，nacos 服务端会把客户端的订阅信息存放到与其对应的客户端对象中，然后还会查找到客户端订阅的 Service 服务下的所有服务实例信息返回给客户端。我把 SubscribeServiceRequestHandler 请求处理器的代码搬运过来了，帮助大家简单回顾一下，请看下面代码块。

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

可以看到，在上面代码块的第 50 行，查询 Service 服务下所有服务实例信息的代码写成了伪代码。而我们现在要解决的问题就是 **ServiceStorage 存储器的 getData(Service service) 方法的调用时机**，刚才我们也分析了，只要存储器的 getData(Service service) 方法一被调用，最终就会得到 Service——ServiceInfo 键值对信息，并且这些信息还会被缓存到存储器中，然后把得到的 Service 服务下的所有服务实例信息，也就是 ServiceInfo 对象返回给订阅了该服务的客户端即可。所以，**我们完全可以在上面代码块的第 50 行调用 ServiceStorage 存储器的 getData(Service service) 方法**。这样一来，SubscribeServiceRequestHandler 请求处理器就重构成了下面这样，请看下面代码块。

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



        ///在这里得到了客户端订阅的服务实例的信息，其实serviceStorage.getData(service)这个方法就得到了，ServiceUtil.selectInstancesWithHealthyProtection()方法只不过是对订阅的服务实例进行筛选、过滤
        //能让客户端得到健康的服务实例信息，以及路由正确的服务实例信息
        //可以看到，在下面方法中首先调用了serviceStorage.getData(service)方法，得到了Service下的所有服务实例信息，然后再使用ServiceUtil的
        //selectInstancesWithHealthyProtection()方法对这些服务实例进行过滤筛选，把健康的服务实例信息返回给客户端
        ServiceInfo serviceInfo = ServiceUtil.selectInstancesWithHealthyProtection(serviceStorage.getData(service),
                metadataManager.getServiceMetadata(service).orElse(null), subscriber.getCluster(), false,
                true, subscriber.getIp());
        

        
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

展示了上面代码块之后，现在大家应该也清楚了 ServiceUtil 工具类的 selectInstancesWithHealthyProtection() 方法的作用究竟是什么。因为我们已经使用 serviceStorage.getData(service) 这行代码从 ServiceStorage 存储器中得到了 Service 服务下的所有服务实例信息，然后把这些信息交给了 ServiceUtil 工具类的 selectInstancesWithHealthyProtection() 方法处理，s**electInstancesWithHealthyProtection() 方法的作用就是对这些服务实例信息进行筛选，把健康的服务实例筛选出来返回给客户端**。当然，我解释得可能过去简单了，要实现这个功能，还需要一些新的组件的帮助，并且其中也蕴含了一些细节知识。但是这些内容我就不在文章中展示了，因为这些功能的实现要用到服务员信息管理器，以及路由器，路由管理器等等组件，这些组件我都为大家展示在第八版本代码中了，并且添加了详细的注释，大家可以先自己看看，如果看不明白也没关系，这并不是我们的主线内容，可以先放一放，大家只需要掌握 nacos 服务端是怎么完全实现服务订阅功能就行了。

到此为止，我们这一章的核心内容就结束了。但是，我能想到大家心里肯定都有一个疑问，那就是我们把 Service——ServiceInfo 键值对信息缓存在 ServiceStorage 存储器中了，以后只要有 nacos 客户端向服务端订阅了 Service 服务，或者客户端主动向服务端拉取 Service 服务下的信息时，ServiceStorage 存储器就会把缓存的 Service 服务下的所有服务实例都返回给 nacos 客户端。那么问题来了，我们刚才实现的 ServiceStorage 存储器只是把 Service 服务下的服务实例信息缓存起来了，假如 nacos 服务端更新了 Service 服务下的服务实例信息呢？比如说有客户端向 Service 服务下注册了新的服务实例，这就意味着 Service 服务发生变更了，这个时候 ServiceStorage 存储器再想把自己缓存的服务实例返回给 nacos 客户端，肯定就要返回最新的服务实例信息，**那么 ServiceStorage 存储器缓存的服务实例信息该怎么更新呢**？我相信这个问题肯定困扰着大家。但是在这一章我不会为大家解决这个问题了，因为这是下一章要节点的问题，下一章我就会为大家实现 nacos 服务端服务实例信息变更，要主动把最新的服务实例信息推送给客户端功能，随着这个功能的实现，困扰大家的问题也就得到解决了。所以，大家也不用心急，这个问题在下一章就得到解决了。

## 简单梳理断开客户端连接的操作流程

其实本章的内容已经可以结束了，但是我想再用一小节的篇幅，为大家再简单补充点知识。到此为止，我们已经把 nacos 客户端和服务端连接成功建立了，并且在成功建立连接的基础上，实现了很多重要的功能，也引入了很多组件，这些组件协同工作，让 nacos 框架可以顺利运行，为我们提供服务。在实现的众多功能中，大家也能意识到，很多功能组件都和一个重要的变量有紧密的联系，那就是 nacos 服务端为客户端创建的客户端连接 Id。这个客户端连接 Id 作为一个唯一标识符号，成为获得很多组件时的关键 key。比如要获得 nacos 服务端为每一个客户端创建的 Connection 连接对象，这个时候就要把客户端连接 Id 作为 key，去 ConnectionManager 连接管理器中获得对应的连接对象；要获得 nacos 服务端为每一个客户端创建的 ConnectionBasedClient 客户端对象时，也是以客户端连接 Id 为 key，去 ConnectionBasedClientManager 客户端管理器中获得对应的客户端对象。还有最近几章我们引入的 ClientServiceIndexesManager 以及 ServiceStorage 组件，客户端连接 Id 都在其中发挥了至关重要的作用。这些流程大家肯定都非常熟练了，而我想强调的是，在客户端与服务端连接断开之后呢？**nacos 服务端要执行什么操作呢？客户端与服务端连接断开之后，服务端为该客户端创建的客户端连接 Id 就失去了它的作用，那么用到了这个客户端连接 Id 的组件，其内部存储的对应的键值对还有效吗？如果失效的话要怎么清除呢**？再比如说，**当一个 nacos 客户端向服务端注销、或者是取消订阅了某个服务信息，那 nacos 服务端又该执行什么操作呢**？

以上我提出的两个问题很值得大家去思考，去弄清楚，好在我为大家提供的第八版本代码中，这些问题都解决了，相对应的功能和代码我也都仿照源码为大家实现了，并且添加了详细的注释，大家可以直接从我提供的第八版本代码中查看。我就不在文章中为大家详细展示这些功能了，否则要讲的内容就太多了。这些功能实现流程也很简单，我相信大家只要把前面的知识都掌握了，肯定可以看懂这些流程。而我在此跟大家啰嗦这么多，其实就是想给大家理清楚头绪，或者给大家讲解一些学习这些功能的源头代码。

请大家想一想，客户端和服务端连接什么时候会断开？当然，这个属于网络问题，我们也不太好说。那就换另一种问法，客户端和服务端连接断开了之后，怎么就被 nacos 服务端检测到了？如果大家还记得我之前实现的服务端连接驱逐器，**也就是 RuntimeConnectionEjector 对象，大家就会意识到这个连接驱逐器负责 nacos 服务端这边的健康检查，当检测到有客户端连接过期时，或者检测到当前服务端过载时，就会主动注销相关的客户端连接。具体内容大家可以去 ConnectionManager 连接管理器和 RuntimeConnectionEjector 连接驱逐器中查看**。那注销了不健康的客户端连接之后呢？要执行什么操作，首先与该客户端对应的 ConnectionBasedClient 客户端对象也应该移除了吧？还有就是使用了该客户端连接 Id 的组件缓存的相关信息，也应该清除了吧？那应该怎么清除呢？这里我就不展开讲解了，否则我就把所有流程都讲完了，简单来说，nacos 服务端会在客户端连接断开之后，使用 NotifyCenter 事件通知中心发布 ClientReleaseEvent 客户端连接释放事件，大家可以看看哪些订阅者订阅了该事件，有针对该事件执行了什么操作，如何处理该事件。这些就留给大家自己去第八版本代码中查看吧。至于 nacos 客户端注销、取消服务信息，也是相同的流程，都是发布了相关事件，然后订阅者处理事件。

好了朋友们，这一章的内容到此就真的结束了，可以看到内容并不多，核心内容就是引入了一个 ServiceStorage 存储器。现在大家再去看第八版本代码，就会发现全部内容都可以看懂了。下一章我将为大家实现 nacos 服务端主动把更新之后的服务实例信息推送给客户端功能，诸位，我们下一章见！