在前面两章，我已经为大家把 nacos 服务端订阅服务实例的功能实现完毕了，除此之外，服务端注册服务实例的功能也已经实现了。这两个可以说是整个 nacos 框架中最核心的功能了，当然，随着这两个功能的实现，剩下的最后一个重要功能也要被立即实现了。**那就是 nacos 服务端把最新的服务实例信息主动推送给订阅该服务端的客户端功能**。在这一章我就会为大家实现这个功能。

## 分析 nacos 服务端主动推送客户端服务信息功能的实现思路

在真正开始实现 nacos 服务端主动推送服务实例信息给客户端的功能之前，**我还是想先为大家简单分析一下，nacos 服务端在什么情况下才会向客户端主动推送最新的服务实例信息**。当然，我也能想到很多朋友对此都已经非常熟悉了，所以正如我刚才所说，我只是为大家简单解释一下。大家肯定都能意识到，我们即将要实现的服务端主动推送服务实例信息给客户端的功能，实际上是在订阅功能的基础上衍生出来的。假如没有任何一个 nacos 客户端向服务端订阅了某个服务下的所有服务实例信息，那即便在 nacos 服务端，某些服务实例信息发生变更了，nacos 服务端也不会主动把最新的服务实例信息推送给任何一个客户端，因为没有任何一个客户端订阅了它们。这也就是说，**只有当 nacos 客户端向服务端订阅了某个服务信息，nacos 服务端才有可能向客户端主动推送最新的服务实例信息。有客户端向 nacos 服务端订阅服务实例信息，这个是服务端主动向客户端推送服务实例信息的必要条件**。好了，这个大的前提分析完毕了，接下来就该思考思考 nacos 服务端在什么情况下才会主动向客户端推送服务实例信息了。

这个其实就很好分析了，**最直接的一种情况，肯定就是当 nacos 服务端发现自己保存的服务实例信息发生变更，这个时候肯定就会向订阅了该服务的客户端主动发送服务实例变更请求，在请求中封装最新的服务实例信息，把请求发送给客户端，客户端就知道最新的服务实例信息了**。这种情况我相信每一位朋友都能想到，但是在 nacos 源码中还有另一种情况，nacos 服务端也会主动向订阅了某个服务的客户端发送最新的服务实例信息，**那就是当某个客户端向服务端执行了订阅服务实例操作时，服务端会立刻发布一个 ServiceSubscribedEvent 服务订阅事件，对应的事件订阅者接收到该事件后，就会在处理该事件的过程中把最新的服务实例信息发送给订阅了该服务的客户端**。这种情况大家现在可能还不太理解，不用急，后面我会为大家详细讲解这种情况下的程序执行的流程。总之，现在我们已经明确了两种 nacos 服务端要向客户端主动推送服务实例信息的情况，**一种是 nacos 服务端发现服务实例信息发生变更了；另一种就是有客户端向服务端执行了订阅服务信息的操作**。接下来，我们就可以围绕这两种情况来实现服务端向客户端主动推送服务实例信息功能了。

既然 nacos 服务端要向客户端发送服务实例信息，并且是在服务实例发生变更的情况下向订阅了该服务的客户端发送服务实例信息，那我们不妨从源头上思考一下，这个功能要以哪里为切入点开始实现。所谓源头其实也很明了，我们都很清楚，**nacos 服务端只有在发现自己保存的服务实例信息发生变更了，才会主动向订阅了该服务的客户端推送最新的服务实例信息**，**那么显然服务实例信息发生变更就是所谓的源头**，换句话说，**我们要重构程序，能够让 nacos 服务端主动发现自己保存的服务实例信息发生变更了**，这样才能执行后续的操作。而后续的操作也很简单，首先 nacos 服务端已经发现有服务实例信息发生变更了，接下来肯定就要找到哪些客户端订阅了该服务，然后才能把变更的服务实例信息发送给这些客户端。**所以后续操作就是根据变更的服务实例所在服务的信息，找到订阅了该服务的客户端**，然后把最新的服务实例信息主动发送给对应的客户端。**而把最新的服务实例信息主动发送给对应的客户端，这也就是第三步要执行的操作**。那么问题这就来了，nacos 服务端怎么得到要发送给客户端的服务实例信息呢？这时候就要结合前面两章讲解的内容了，在我为大家实现 nacos 服务端订阅服务实例功能时，我引入了一个非常重要的组件，那就是 ServiceStorage 类，我把这个类的对象称为服务信息存储器，这个存储器中存放的就是所有注册到服务端的服务实例信息。我把这个 ServiceStorage 类的部分内容搬运到这里了，帮助大家简单回顾一下这个服务信息存储器，请看下面代码块。

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


    
    //其他内容省略
}
```

在上面这个代码块展示完毕之后，我相信大家多少能回忆起来一点上一章的内容。在上一章我跟大家说：**Service——ServiceInfo 键值对信息缓存在 ServiceStorage 存储器中了，只要有 nacos 客户端向服务端订阅了 Service 服务，或者客户端主动向服务端拉取 Service 服务下的信息时，ServiceStorage 存储器就会把缓存的 Service 服务下的所有服务实例都返回给 nacos 客户端**。这一点想必大家都还有印象。但现在我们探讨的是 nacos 服务端怎么得到要发送给客户端的服务实例信息，难道也是从 ServiceStorage 这个服务实例信息存储器中获得吗？这时候就该大胆承认了，**当 nacos 服务端要把最新的服务实例信息主动推送给客户端时，就是从这个 ServiceStorage 服务实例信息缓存器中获取了对应的服务实例信息，然后把信息发送给订阅了该服务的客户端的**。因为 ServiceStorage 本身就缓存了所有注册到服务端的服务实例信息，并且还维护了 Service——ServiceInfo 的映射关系，根据这个映射关系就可以把订阅了 Service 服务下的所有最新服务实例信息全部发送给订阅了该服务的客户端。这个逻辑想必也是容易理解的。

当然，这么做也不是没有隐患，虽然 ServiceStorage 服务信息缓存器缓存了各个 Service 服务下的服务实例信息，nacos 服务端可以很容易就从这个缓存器中得到某个 Service 服务下的所有服务实例信息，但是请大家想一想上一章遗留的一个问题，那就是当 nacos 服务端更新了 Service 服务下的服务实例信息，也就是服务实例信息发生变更了，服务端需要把最新的服务实例信息主动发送给客户端，但是我们在上一章并没有为 ServiceStorage 服务信息存储器实现更新服务实例信息的功能。这样一来，服务端在主动把服务实例信息发送给客户端的时候，从 ServiceStorage 中得到的可能还是旧的服务实例信息，那这该怎么办呢？**显然，在 nacos 服务端把服务实例信息主动发送给对应的客户端之前，还应该更新一下 ServiceStorage 服务信息存储器中缓存的服务实例信息**。**那么 ServiceStorage 存储器缓存的服务实例信息该怎么更新**，这是我们目前面临的一个很棘手的问题。当然，这个问题现在也不急着解决，时机并不太成熟，等后面开始真正实现 nacos 服务端主动向客户端推送最新服务实例信息功能时，大家就全都清楚了。

好了朋友们，在经过上面的分析之后，现在让我来为大家简单总结一下，要想实现 nacos 服务端主动向客户端推送最新服务实例信息功能，程序需要执行的几个操作，这几个操作就是接下来我们要实现的功能的大方向。我已经把这几个操作整理完毕，给大家展示在下面了：

**1 首先 nacos 服务端要主动感知到保存的服务实例信息发生了变化。**

**2 根据更新的服务实例所在服务的信息，找到订阅了该服务的 nacos 客户端。**

**3 更新** **ServiceStorage 服务信息存储器中缓存的服务实例信息，并且从 ServiceStorage 服务信息存储器中得到最新的服务实例信息。**

**4 把 nacos 客户端订阅的 Service 服务下的最新的服务实例信息全部发送给对应的 nacos 客户端。**

以上 4 点就是我们目前总结出来的要实现 nacos 服务端主动向客户端推送最新服务实例信息功能，程序需要执行的几个操作。接下来我们就根据这几个操作，开始真正实现 nacos 服务端主动向客户端推送最新服务实例信息功能。

## 引入 NamingSubscriberServiceV2Impl 订阅服务组件

  

首先我们先分析第一个操作该如何实现，在上一小节我已经跟大家说了，**nacos 服务端主动向客户端推送最新服务实例信息功能的源头，就是 nacos 服务端可以主动感知到客户端注册过来的服务实例信息发生了变更**。当然，还有另一个大前提，那就是必须有 nacos 客户端订阅了发生变更的服务实例信息所在的 Service，这一点已经重复过很多次了。好了，那接下来就开始实现吧，让我们想一想，应该怎么重构 nacos 服务端，就能让服务端主动感知到有服务实例信息发生了变更。当然，我写这些话其实有些多余了，因为这个功能已经差不多实现了。**在很久之前，我们实现 nacos 客户端的时候，就已经实现了 NotifyCenter 事件通知机制**。既然是这样，**那我们不妨就定义一个新的事件，比如就把它定义为 ServiceChangedEvent 服务信息变更事件，然后再定义一个订阅者，专门订阅这个事件。当 nacos 服务端保存的服务实例信息发生了变更，就立刻让 NotifyCenter 通知中心发布一个 ServiceChangedEvent 事件，这样一来，订阅者就可以接收并处理这个事件了，而处理事件的具体操作无非就是把最新的服务实例信息按照必要的操作发送给订阅了对应服务的 nacos 客户端**。大家可以品味一下这个流程，如果这个流程理解了，接下来我们就可以思考另外一个问题，**那就是 nacos 服务端服务实例信息发生变更的几种情况分别是什么**。因为只有清楚了服务实例信息发生变更的几种情况，才能在服务实例信息发生变更之后让 NotifyCenter 通知中心及时发布 ServiceChangedEvent 事件。

当然，也许跟大家讨论这个问题似乎有些多余，因为大家很容易就能想到，所谓的服务实例信息变更不就是有客户端又注册了新的服务实例吗？比如说有一个客户端 A 向注册中心服务端注册了某个 Service 服务下的服务实例信息，客户端 B 订阅了该 Service 服务；程序运行了一会，另一个客户端 C 又向该服务下注册了新的服务实例，这不就意味着 Service 服务下的服务实例信息发生变更了吗？这种情况最容易想到，但是除了这种情况，我还要再补充几种情况：比如说是客户端 A 更新了原本注册的服务实例信息，也许是服务实例的 IP 地址更新了，这也意味着 Service 服务下的服务实例信息发生变更了；又或者是 web 控制台直接更新了 Service 服务下的某个服务实例的元信息(这个功能会在后面的 nacos 支线内容中为大家实现)，这也是 Service 服务下的服务实例信息发生变更的情况。当然，客户端 A 注销了自己注册到服务端的服务实例信息，这也是 Service 服务下的服务实例信息发生变更的情况。总之，这几个操作都会使服务实例信息发生更新，大家一定要考虑全面。

也许有朋友会觉得我对服务实例发生变更情况的解释有些啰嗦了，但我确实有自己的用意，因为只要我稍加提示，大家就会发现，**不管是 web 控制台更新服务实例信息，还是某个客户端新注册了服务实例信息，还是某个服务实例信息被更新或者被注销了**，**这些操作最终都会使程序的执行流程来到 EphemeralClientOperationServiceImpl 类中**，具体逻辑大家可以去我提供的第 9 版本代码中查看，这些都是已经讲过的内容了，我就不再重复了。当然，这个 EphemeralClientOperationServiceImpl 类的作用我也不再重复了，大家肯定都很熟悉了。我把 EphemeralClientOperationServiceImpl 类的部分内容也搬运过来了，请看下面代码块。

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
        //事件通知中心发布客户端注册事件
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientRegisterServiceEvent(singleton, clientId));
        //事件通知中心发布服务实例元数据变化事件
        NotifyCenter.publishEvent(new MetadataEvent.InstanceMetadataEvent(singleton, instanceInfo.getMetadataId(), false));
    }


    //注销服务实例的方法
    @Override
    public void deregisterInstance(Service service, Instance instance, String clientId) {
        if (!ServiceManager.getInstance().containSingleton(service)) {
            Loggers.SRV_LOG.warn("remove instance from non-exist service: {}", service);
            return;
        }
        Service singleton = ServiceManager.getInstance().getSingleton(service);
        Client client = clientManager.getClient(clientId);
        if (!clientIsLegal(client, clientId)) {
            return;
        }
        InstancePublishInfo removedInstance = client.removeServiceInstance(singleton);
        client.setLastUpdatedTime();
        client.recalculateRevision();
        if (null != removedInstance) {
            //在这里发布了客户端服务注销事件，这个事件要记住，很快就会用到
            NotifyCenter.publishEvent(new ClientOperationEvent.ClientDeregisterServiceEvent(singleton, clientId));
            NotifyCenter.publishEvent(new MetadataEvent.InstanceMetadataEvent(singleton, removedInstance.getMetadataId(), true));
        }
    }


    //订阅服务实例信息的方法，这个方法大家一定要看一下
    @Override
    public void subscribeService(Service service, Subscriber subscriber, String clientId) {
        Service singleton = ServiceManager.getInstance().getSingletonIfExist(service).orElse(service);
        Client client = clientManager.getClient(clientId);
        if (!clientIsLegal(client, clientId)) {
            return;
        }
        client.addServiceSubscriber(singleton, subscriber);
        client.setLastUpdatedTime();
        //要记住这里发布了一个ClientSubscribeServiceEvent事件，只要有客户端订阅了某个Service服务
        //nacos服务端就会发布一个ClientSubscribeServiceEvent事件，这个事件很快就会被用到
        NotifyCenter.publishEvent(new ClientOperationEvent.ClientSubscribeServiceEvent(singleton, clientId));
    }


    //其他内容省略

}
```

从上面代码块中可以看到，只要是服务实例信息发生变更了，变更服务实例信息的操作最终就会执行到 EphemeralClientOperationServiceImpl 类中，**如果是有客户端注册了新的服务实例信息，或者是某个服务实例信息被更新了，那就执行 registerInstance() 方法；如果是某个服务实例信息被注销了，那就执行 deregisterInstance() 方法**。**只要这两个方法一旦被执行，那我就可以确定 nacos 服务端保存的服务实例信息肯定发生了变更**，这一点大家可以理解吧？刚才我们不是已经分析了吗：在 nacos 服务端定义一个新的 **ServiceChangedEvent 服务信息变更事件，然后再定义一个订阅者，专门订阅这个事件。当 nacos 服务端保存的服务实例信息发生了变更，就立刻让 NotifyCenter 通知中心发布一个 ServiceChangedEvent 事件，这样一来，订阅者就可以接收并处理这个事件了**。现在我们已经找到了服务实例信息发生变更的源头，**就是 EphemeralClientOperationServiceImpl 类的 registerInstance() 方法和 deregisterInstance() 方法**，**那么我只需要在这两个方法执行完毕之后，让** **NotifyCenter 通知中心发布一个 ServiceChangedEvent 事件，这样不就能够让 nacos 服务端感知到有服务实例发生变更了吗**？这个逻辑理解起来应该没什么问题吧？

很好，分析到这里已经很不错了，源头已经找到，接下来只需要按照刚才分析的思路编写代码即可。但是，情况总是和我们预想的有一些出入，因为在 EphemeralClientOperationServiceImpl 类的 registerInstance() 方法和deregisterInstance() 方法中已经使用 NotifyCenter 通知中心发布了事件，当服务实例信息发生变更之后，在 registerInstance() 方法中已经发布了 ClientRegisterServiceEvent 客户端注册服务事件，在 deregisterInstance() 方法中发布了 ClientDeregisterServiceEvent 客户端注销服务事件。这两个事件的作用大家应该也都还记得，之前我们还引入了一个 ClientServiceIndexesManager 组件，这个组件就是一个订阅者，就订阅了 ClientRegisterServiceEvent、ClientDeregisterServiceEvent 这两个事件。ClientServiceIndexesManager 类的作用我也不再重复讲解了，大家可以自己看看前面的文章，简单回顾一下。

总之文章进行到这里，我们发现情况和我们预想得有些不同，EphemeralClientOperationServiceImpl 类的 registerInstance() 方法和deregisterInstance() 方法中已经使用 NotifyCenter 通知中心发布了其他事件。当然，并不是说在这两个方法中发布了其他事件，就不能再发布我们新定义的 ServiceChangedEvent 事件，我们完全可以对 EphemeralClientOperationServiceImpl 类的 registerInstance() 方法和deregisterInstance() 方法进行重构，让它们在执行完毕时使用 NotifyCenter 通知中心发布 ServiceChangedEvent 事件。但我们没必要这么急，让我们先顺着原本的逻辑往下看，看看订阅了 ClientRegisterServiceEvent、ClientDeregisterServiceEvent 这两个事件的 ClientServiceIndexesManager 订阅者做了什么。这个也是旧知识了，所以我也不再多重复什么，直接把 ClientServiceIndexesManager 类的内容搬运到这里了，请看下面代码块。

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
        } else if (event instanceof ClientOperationEvent.ClientDeregisterServiceEvent) {
            //如果是客户端注销服务实例事件，那就从publisherIndexes中移除对应的客户端Id
            removePublisherIndexes(service, clientId);
        } else if (event instanceof ClientOperationEvent.ClientSubscribeServiceEvent) {
            //如果是客户端订阅服务实例信息事件，这个ClientSubscribeServiceEvent事件就是
            //在EphemeralClientOperationServiceImpl类的subscribeService方法中发布的
            //就把客户端Id添加到subscriberIndexes中
            addSubscriberIndexes(service, clientId);
        } else if (event instanceof ClientOperationEvent.ClientUnsubscribeServiceEvent) {
            //如果是客户端取消服务实例订阅事件，就把客户端Id从subscriberIndexes移除
            removeSubscriberIndexes(service, clientId);
        }
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


    //从publisherIndexes中移除对应的客户端Id的方法
    private void removePublisherIndexes(Service service, String clientId) {
        publisherIndexes.computeIfPresent(service, (s, ids) -> {
            ids.remove(clientId);
            //这个方法也会发布服务实例变更事件，服务端也要主动给客户端推送最新的服务实例信息
            //注意，这里会把最新的服务实例主动推送给所有订阅了该服务的客户端
            NotifyCenter.publishEvent(new ServiceEvent.ServiceChangedEvent(service, true));
            return ids.isEmpty() ? null : ids;
        });
    }

    
    //添加订阅者的方法
    private void addSubscriberIndexes(Service service, String clientId) {
        subscriberIndexes.computeIfAbsent(service, key -> new ConcurrentHashSet<>());
        if (subscriberIndexes.get(service).add(clientId)) {

            //如果有新的客户端订阅服务了，就发布服务订阅事件，服务端就会把客户端订阅的服务主动推送给这个客户端
            NotifyCenter.publishEvent(new ServiceEvent.ServiceSubscribedEvent(service, clientId));
        }
    }


    //该类的其他内容就先省略了，大家可以直接去我提供的第八版本代码中查看

}








//以下就是本章新引入的ServiceChangedEvent事件的具体内容
package com.cqfy.nacos.naming.core.v2.event.service;

public class ServiceEvent extends Event {

    private static final long serialVersionUID = -9173247502346692418L;

    private final Service service;

    public ServiceEvent(Service service) {
        this.service = service;
    }

    public Service getService() {
        return service;
    }

    /**
     * Service data changed event.
     */
    public static class ServiceChangedEvent extends ServiceEvent {

        private static final long serialVersionUID = 2123694271992630822L;

        public ServiceChangedEvent(Service service) {
            this(service, false);
        }

        public ServiceChangedEvent(Service service, boolean incrementRevision) {
            super(service);
            //在这里设置了服务实例最新的更新时间，这个时间之后会在PushExecuteTask类中用到，大家要留一个印象
            service.renewUpdateTime();
            if (incrementRevision) {
                service.incrementRevision();
            }
        }
    }

    /**
     * Service is subscribed by one client event.
     */
    public static class ServiceSubscribedEvent extends ServiceEvent {

        private static final long serialVersionUID = -2645441445867337345L;

        private final String clientId;

        public ServiceSubscribedEvent(Service service, String clientId) {
            super(service);
            this.clientId = clientId;
        }

        public String getClientId() {
            return clientId;
        }
    }

}
```

看完了上面的代码块之后，大家肯有一种恍然大悟的感觉，因为在上面代码块中可以清楚地看到，**当 ClientServiceIndexesManager 这个订阅者在处理 ClientRegisterServiceEvent、ClientDeregisterServiceEvent 这两个事件的过程中，在更新了 publisherIndexes 成员变量的内容之后，就会紧接着就会让 NotifyCenter 通知中心发布我们之前新定义的 ServiceChangedEvent 事件，表示在 nacos 服务端有服务实例信息发生变更了。具体实现就在 ClientServiceIndexesManager 类的 addPublisherIndexes()、removePublisherIndexes() 这两个方法中**。到此为止，本章的内容就和前面几章的内容联系起来了，也许有些朋友们对此还有印象，在前面几章我为大家引入 ClientServiceIndexesManager 类的时候，我还特意在 addPublisherIndexes() 方法的注释中跟大家强调，要先记住该方法中发布的 ServiceChangedEvent 事件。

好了，现在真正发布 ServiceChangedEvent 事件的时机也彻底弄清楚了，接下来就让我来为大家梳理一下 ServiceChangedEvent 事件发布的流程：**首先是有客户端注册了新的服务实例信息，或者是某个服务实例信息被更新或者注销了，总之就是在 nacos 服务端发生了服务实例信息变更情况，不管是哪种情况，最终操作都会执行到 EphemeralClientOperationServiceImpl 类中，如果是有客户端注册了新的服务实例信息，或者是某个服务实例信息被更新了，那就执行 registerInstance() 方法；如果是某个服务实例信息被注销了，那就执行 deregisterInstance() 方法；在执行 registerInstance() 方法 和 deregisterInstance() 方法的过程中，NotifyCenter 通知中心会发布 ClientRegisterServiceEvent 和 ClientDeregisterServiceEvent 这两个事件，而 ClientServiceIndexesManager 恰好是这两个事件的订阅者，在 ClientServiceIndexesManager 组件处理这两个事件的过程中，最终会在 addPublisherIndexes() 和 removePublisherIndexes() 方法中发布 ServiceChangedEvent 事件，表示在 nacos 服务端有服务实例信息发生变更了**。如果用一幅简图来概括以上总结的流程，那就可以展示成下面这样，请看下面代图片。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1726631306130-bbb8a5e2-593a-44c7-87e4-e2a7a3276961.jpeg)

大家可以结合上面的简图仔细品味品味程序的执行流程，如果大家理解了 nacos 服务端发布 ServiceChangedEvent 事件的流程，那么接下来请大家跟着我思考另一个问题：那就是怎么能让 nacos 服务端感知到有服务实例信息发生了变更呢？这个问题可能有些不太妥当，因为我们已经实现了 nacos 服务端发布 ServiceChangedEvent 服务信息变更事件功能。确切地说，现在我们面临的问题，**是如何让 nacos 服务端接收并处理这个 ServiceChangedEvent 服务信息变更事件**。因为只有让 nacos 服务端感知到发布的这个 ServiceChangedEvent 事件，服务端才可以针对这个事件执行一系列后续操作。当然，让 nacos 服务端感知到其内部发布的 ServiceChangedEvent 事件的方法也很简单，那就是为这个 ServiceChangedEvent 事件专门定义一个订阅者对象，这样一来，一旦 NotifyCenter 在 nacos 服务端内部发布了 ServiceChangedEvent 事件，这个订阅者就会接收并处理这个事件。这个订阅者我也已经定义完毕了，就把它定义为了 NamingSubscriberServiceV2Impl 类。具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：这个类的对象也是一个订阅者，订阅了ServiceChangedEvent和ServiceSubscribedEvent事件
 * 这样一来，只要服务端的服务实例信息发生变更，或者有客户端订阅了服务信息，这个类的对象就发起主动推送服务实例信息给客户端的操作
 */
@org.springframework.stereotype.Service
public class NamingSubscriberServiceV2Impl extends SmartSubscriber implements NamingSubscriberService {



    //构造方法
    public NamingSubscriberServiceV2Impl() {
       //在这里把自己注册到通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：当前订阅者订阅的事件类型
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //订阅了服务实例变更事件
        result.add(ServiceEvent.ServiceChangedEvent.class);
        //订阅了服务订阅事件
        result.add(ServiceEvent.ServiceSubscribedEvent.class);
        return result;
    }





    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：处理具体事件的方法
     */
    @Override
    public void onEvent(Event event) {
        if (event instanceof ServiceEvent.ServiceChangedEvent) {

            //如果是服务实例变更事件，就在这里处理服务实例变更事件
            
        } else if (event instanceof ServiceEvent.ServiceSubscribedEvent) {


            //如果发布的是服务订阅事件，就在这里处理服务订阅事件
            //ServiceSubscribedEvent事件是在ClientServiceIndexesManager类的
            //addSubscriberIndexes()方法中发布的，刚才已经提示过大家了，现在大家应该还有一些印象
            //虽然当前处理ServiceSubscribedEvent事件的功能还没有实现，但现在我可以为大家简单解释一下
            //在本章一开始我跟大家说了，当某个客户端向服务端执行了订阅服务实例操作时，
            //服务端会立刻发布一个ServiceSubscribedEvent服务订阅事件，对应的事件订阅者接收到该事件后
            //就会在处理该事件的过程中把最新的服务实例信息发送给订阅了该服务的客户端，这里就是执行这个操作的入口
            //但在本章我们还无法实现这个功能，下一章大家就知道怎么回事了
        }
    }



    //其他内容省略
}  
```

好了，现在 NamingSubscriberServiceV2Impl 类也定义完毕了，可以看到，这个类也添加了 springboot 的 @Service 注解，这就意味着这个类的对象也要交给 springboot 的容器来管理。并且在该类的构造方法中，当前订阅者被注册到了 NotifyCenter 通知中心。NamingSubscriberServiceV2Impl 这个订阅者订阅了 ServiceChangedEvent 服务信息变更事件，这样一来，只要 NotifyCenter 通知中心在 nacos 服务端发布了 ServiceChangedEvent 事件，那么 NamingSubscriberServiceV2Impl 这个订阅者就会接收并处理该事件。当然，具体处理 ServiceChangedEvent 事件的 onEvent() 方法我还没有为大家实现。因为还有一些细节问题需要继续探讨、解决。但不管怎么说吧，现在我们已经实现了上一小节总结的四个操作中的第一个操作。为了实现 nacos 服务端主动向客户端推送最新服务实例信息功能，我们总结出了程序必须执行的四个操作，这四个操作就是：

**1 首先 nacos 服务端要主动感知到保存的服务实例信息发生了变化。**

**2 根据更新的服务实例所在服务的信息，找到订阅了该服务的 nacos 客户端。**

**3 更新** **ServiceStorage 服务信息存储器中缓存的服务实例信息，并且从 ServiceStorage 服务信息存储器中得到最新的服务实例信息。**

**4 把 nacos 客户端订阅的 Service 服务下的最新的服务实例信息全部发送给对应的 nacos 客户端。**

如今第一个操作总算是实现完毕了，在发生了服务实例信息变更之后，nacos 服务端可以发布 ServiceChangedEvent 事件，然后 NamingSubscriberServiceV2Impl 订阅者可以接收并处理这个事件，这样一来，nacos 服务端就可以主动感知到服务实例发生了变化。到此为止，本章核心内容就差不多结束了。

眼看本章内容就要结束了，我能想到很多朋友会有些心急，觉得我在这一章基本上没讲解什么干货，就算实现的发布 ServiceChangedEvent 事件这个功能，其实在前几章也早就给大家展示过了。而且我也能想到，本章实现的功能，以及梳理的程序执行流程对大家来说肯定没什么难度，就这样结束本章，好像有点戛然而止的感觉。所以接下来我要为自己简单辩解几句：本章的内容确实非常简单，顶多算是实现 nacos 服务端主动向客户端推送最新服务实例信息功能的前戏，当 NamingSubscriberServiceV2Impl 订阅者接收并处理 ServiceChangedEvent 事件时，这时候操作就变得复杂很多了，也就是说，下一章要真正引入的向客户端推送服务实例信息的任务体系比较复杂，而且有很多细节需要讨论。**就比如说假如 nacos 服务端保存的某个 Service 下的服务实例信息频繁发生变更，几秒之内变更了十几次，难道服务端就要立刻给订阅了该服务的客户端一下子发送十几个服务变更请求吗？这显然又些不合理，那该怎么解决这种情况呢**？这个问题在下一章就会得到解决。本篇文章结束之后，大家可以简单看一下第九版本的代码，当然，在第九版本代码中，还有很多功能我没有给大家在文章中剖析实现，这是后面章节的内容。所以如果大家不太着急，可以再等一等，等我把下一篇文章更新完毕了，大家再结合文章阅读第九版本代码。好了朋友们，本章内容就到此结束吧，我们下一章见！