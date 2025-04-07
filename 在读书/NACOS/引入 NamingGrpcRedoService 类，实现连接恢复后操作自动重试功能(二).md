## 分析操作重试功能的执行情况

  

在上一章结尾，我提出了一个问题，那就是从一开始我就说**这个 nacos 客户端的操作重试是在客户端与新的服务端连接重建之后，要执行相应的操作重试操作；但重构之后的程序呈现出来的根本不是这个效果**，**我之所以这么说是因为上一章引入的 RedoScheduledTask 定时任务执行时机实际上非常模糊**。这个 RedoScheduledTask 类的内容如下，我再次搬运过来了，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/23
 * @方法描述：这个类的对象就是用来周期检测是否又需要重新执行的操作
 */
public class RedoScheduledTask extends AbstractExecuteTask {

    //客户端代理
    private final NamingGrpcClientProxy clientProxy;

    //重做服务对象
    private final NamingGrpcRedoService redoService;


    public RedoScheduledTask(NamingGrpcClientProxy clientProxy, NamingGrpcRedoService redoService) {
        this.clientProxy = clientProxy;
        this.redoService = redoService;
    }


    @Override
    public void run() {
        //首先判断客户端和服务端的连接是否健康，如果不健康就直接退出该方法
        //假如客户端和服务端仍然处于断链的情况，你再怎么重试也没用啊
        if (!redoService.isConnected()) {
            LogUtils.NAMING_LOGGER.warn("Grpc Connection is disconnect, skip current redo task");
            return;
        }
        try {
            //执行和服务实例相关的重试操作
            redoForInstances();
            //执行和订阅相关的重试操作，因为服务订阅的功能我还没有实现
            //所以在第三版本中把下面这行代码注释了
            //redoForSubscribes();
        } catch (Exception e) {
            LogUtils.NAMING_LOGGER.warn("Redo task run with unexpected exception: ", e);
        }
    }

    //执行和服务实例相关的重试操作的方法
    private void redoForInstances() {
        //得到所有需要操作重试的服务实例重做对象
        for (InstanceRedoData each : redoService.findInstanceRedoData()) {
            try {
                //对每一个服务实例重做对象执行操作重试
                redoForInstance(each);
            } catch (NacosException e) {
                LogUtils.NAMING_LOGGER.error("Redo instance operation {} for {}@@{} failed. ", each.getRedoType(),
                        each.getGroupName(), each.getServiceName(), e);
            }
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：执行服务实例重做操作
     */
    private void redoForInstance(InstanceRedoData redoData) throws NacosException {
        //获得需要重新执行的操作类型
        RedoData.RedoType redoType = redoData.getRedoType();
        //得到服务名称
        String serviceName = redoData.getServiceName();
        //得到组名
        String groupName = redoData.getGroupName();
        LogUtils.NAMING_LOGGER.info("Redo instance operation {} for {}@@{}", redoType, groupName, serviceName);
        //根据操作类型执行具体操作
        switch (redoType) {
            case REGISTER://注册服务实例的操作
                if (isClientDisabled()) {
                    //先判断客户端是否可用，不可用直接退出
                    return;
                }
                //注册操作重做
                processRegisterRedoType(redoData, serviceName, groupName);
                break;
            case UNREGISTER:
                if (isClientDisabled()) {
                    return;
                }
                //服务注销操作重做
                //clientProxy.doDeregisterService(serviceName, groupName, redoData.get());
                break;
            case REMOVE:
                //从队列中移除重做对象
                redoService.removeInstanceForRedo(serviceName, groupName);
                break;
            default:
        }
    }

    //执行服务注册的重试操作
    private void processRegisterRedoType(InstanceRedoData redoData, String serviceName, String groupName) throws NacosException {
        if (redoData instanceof BatchInstanceRedoData) {
            //判断是否为批量注册服务实例
            BatchInstanceRedoData batchInstanceRedoData = (BatchInstanceRedoData) redoData;
            //该方法并未引入，所以注释了
            //clientProxy.doBatchRegisterService(serviceName, groupName, batchInstanceRedoData.getInstances());
            return;
        }
        //重新执行服务注册操作
        clientProxy.doRegisterService(serviceName, groupName, redoData.get());
    }

        //判断客户端是否可用，就是看看客户端和服务端失去了连接，处于不可用状态，也就是不能连接服务端的状态
    //这个时候，肯定没办法向服务端发送注册请求
    private boolean isClientDisabled() {
        return !clientProxy.isEnable();
    }


}
```

这个定时任务会在 NamingGrpcRedoService 操作重试服务组件中创建，然后提交给 redoExecutor 定时任务执行器来执行，这个定时任务每过 3 秒就会执行一次。而 NamingGrpcRedoService 类的内容我也给大家搬运过来了，大家可以简单回顾一下，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo;





//这个类的对象就是重做服务对象
public class NamingGrpcRedoService implements ConnectionEventListener {

    //这个成员变量就是一直执行重做操作的现成的名称
    private static final String REDO_THREAD_NAME = "com.alibaba.nacos.client.naming.grpc.redo";

    //执行重做操作线程的数量
    private int redoThreadCount;

    //重做操作的延迟时间，重做操作其实就是一个定时任务，这里定义的就是向定时任务调度其中提交定时任务时设定的延迟时间
    private long redoDelayTime;

    //存放注册操作的重做对象的map
    private final ConcurrentMap<String, InstanceRedoData> registeredInstances = new ConcurrentHashMap<>();

    //存放订阅重做操作对象的map，虽然在第三版本中还没有实现订阅服务端服务实例的操作，但下一版本就要实现了，所以这一版本就先把部分代码引入进来了
    private final ConcurrentMap<String, SubscriberRedoData> subscribes = new ConcurrentHashMap<>();

    //执行重做操作的定时任务执行器
    private final ScheduledExecutorService redoExecutor;

    //表示当前客户端和服务端是否成功建立了连接
    private volatile boolean connected = false;


     //构造方法
    public NamingGrpcRedoService(NamingGrpcClientProxy clientProxy, NacosClientProperties properties) {
        //根据配置信息给成员变量赋值
        setProperties(properties);
        //创建了定时任务执行器
        this.redoExecutor = new ScheduledThreadPoolExecutor(redoThreadCount, new NameThreadFactory(REDO_THREAD_NAME));
        //在这里向定时任务执行器提交了一个定时任务，并且这个定时任务会每隔一段时间就执行一次
        //而在RedoScheduledTask这个定时任务中，就会根据客户端和服务端的连接状况一直判断当前客户端是否要进行一些重做操作了
        //RedoScheduledTask定时任务执行任务的周期是3秒，客户端发送请求的超时时间也是3秒，也就是说如果发生网络断连
        //并且超过3秒了，网络恢复之后，RedoScheduledTask定时任务肯定可以检测到需要重新执行的任务
        this.redoExecutor.scheduleWithFixedDelay(new RedoScheduledTask(clientProxy, this), redoDelayTime, redoDelayTime,
                TimeUnit.MILLISECONDS);
    }



     //为成员变量赋值的方法
    private void setProperties(NacosClientProperties properties) {
        redoDelayTime = properties.getLong(PropertyKeyConst.REDO_DELAY_TIME, Constants.DEFAULT_REDO_DELAY_TIME);
        redoThreadCount = properties.getInteger(PropertyKeyConst.REDO_DELAY_THREAD_COUNT,
                Constants.DEFAULT_REDO_THREAD_COUNT);
    }




     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：创建服务实例重做对象的方法，服务实例重做对象封装了注册到服务端的服务实例的全部信息，当客户端重连服务端之后，就可以从服务实例重做对象对象中获得所有信息，然后进行服务实例重新注册的操作
     */
    public void cacheInstanceForRedo(String serviceName, String groupName, Instance instance) {
        String key = NamingUtils.getGroupedName(serviceName, groupName);
        InstanceRedoData redoData = InstanceRedoData.build(serviceName, groupName, instance);
        synchronized (registeredInstances) {
            registeredInstances.put(key, redoData);
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：当客户端把一个服务实例成功注册到服务端之后，会把服务实例重做对象中的注册标志设置为true
     */
    public void instanceRegistered(String serviceName, String groupName) {
        String key = NamingUtils.getGroupedName(serviceName, groupName);
        synchronized (registeredInstances) {
            InstanceRedoData redoData = registeredInstances.get(key);
            if (null != redoData) {
                redoData.registered();
            }
        }
    }


    //下面是注销实例操作的一些方法，在实例注销的时候会进一步讲解
    //实力注销操作，虽然这一版本还没有调用这个方法，但是这个方法和上面的instanceRegistered方法类似，该方法会在客户端注销某个实力的时候被调用
    //调用的时候会把服务实例重做对象的是否正在被注销的标志设置为true
    public void instanceDeregister(String serviceName, String groupName) {
        String key = NamingUtils.getGroupedName(serviceName, groupName);
        synchronized (registeredInstances) {
            InstanceRedoData redoData = registeredInstances.get(key);
            if (null != redoData) {
                //设置正在注销的标志
                redoData.setUnregistering(true);
                redoData.setExpectedRegistered(false);
            }
        }
    }

    //这个方法和上面的instanceDeregister方法类似，该方法会在客户端注销某个实力之后被调用
    //调用的时候会把服务实例重做对象的是否被注册的标志设置为false，意味着已经注销完成了
    public void instanceDeregistered(String serviceName, String groupName) {
        String key = NamingUtils.getGroupedName(serviceName, groupName);
        synchronized (registeredInstances) {
            InstanceRedoData redoData = registeredInstances.get(key);
            if (null != redoData) {
                redoData.unregistered();
            }
        }
    }

    //从map中移除服务实例重做对象
    public void removeInstanceForRedo(String serviceName, String groupName) {
        String key = NamingUtils.getGroupedName(serviceName, groupName);
        synchronized (registeredInstances) {
            InstanceRedoData redoData = registeredInstances.get(key);
            if (null != redoData && !redoData.isExpectedRegistered()) {
                registeredInstances.remove(key);
            }
        }
    }


    //得到所有的需要操作重做的服务实例重做对象的方法
    public Set<InstanceRedoData> findInstanceRedoData() {
        Set<InstanceRedoData> result = new HashSet<>();
        synchronized (registeredInstances) {
            for (InstanceRedoData each : registeredInstances.values()) {
                if (each.isNeedRedo()) {
                    result.add(each);
                }
            }
        }
        return result;
    }




    //以下三个方法大家可以暂时忽略
    
    //判断当前客户端是否与服务端成功建立了连接的方法
    public boolean isConnected() {
        return connected;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：当客户端与服务端成功建立连接之后，这个方法就会被回调。还记得在NamingGrpcClientProxy类的start方法中的操作吗？就是rpcClient.registerConnectionListener(redoService)这个操作
     * 因为NamingGrpcRedoService对象本身就是要给监听器，专门监听客户端与服务端连接状态的，而在rpcClient中，有一个客户端事件执行器一直会判断客户端和服务端的连接状态是否发生了变化，并且根据变化执行监听器中的对应的回调方法
     * 这下子逻辑就串联起来了
     */
    @Override
    public void onConnected(Connection connection) {
        //当前方法被调用的时候，意味着客户端和服务端成功建立连接了
        connected = true;
        LogUtils.NAMING_LOGGER.info("Grpc connection connect");
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：当客户端与服务端连接断开时会调用这个方法
     */
    @Override
    public void onDisConnect(Connection connection) {
        //将连接标志社子为false
        connected = false;
        LogUtils.NAMING_LOGGER.warn("Grpc connection disconnect, mark to redo");
        synchronized (registeredInstances) {
            //因为连接断开了，这就意味着后面会进行重连，重连之后肯定要进行操作重做，把之前注册到服务端的实例重新注册到服务端
            //那怎么在重连之后判断服务实例要重新注册到服务端呢？这时候就可以做一个操作，那就是把服务实例重做对象中已注册的标志更新为false
            //等客户端重新连接了服务端之后，就会根据标志将服务实例重新注册到服务端
            registeredInstances.values().forEach(instanceRedoData -> instanceRedoData.setRegistered(false));
        }
        //下面和上面是同样的操作逻辑，只不过重置的是订阅操作的重做对象中的标志
        synchronized (subscribes) {
            subscribes.values().forEach(subscriberRedoData -> subscriberRedoData.setRegistered(false));
        }
        LogUtils.NAMING_LOGGER.warn("mark to redo completed");
    }


}
```

上面两个代码块看完之后，请大家跟着我的思路，再听我解释一遍这个 NamingGrpcRedoService 所谓的操作重试服务组件的作用。在上一章我首先跟大家模拟了一种情况，**那就是当注册中心客户端执行服务实例注册操作的时候，会创建一个对应的操作重试对象，然后再把服务实例注册请求发送给注册中心服务端，如果请求发送失败了，也就意味着操作没有执行成功，也就意味着客户端与服务端之间连接可能出现问题了；这就会造成一种情况，那就是客户端从服务地址列表中选择一个新的服务地址，客户端会和新的服务器建立连接。连接建立之后，就意味着连接恢复了，这个时候就要把之前没有完成的操作再重新执行一遍**。这也就是所谓的操作重试服务组件的功能。因为一个服务实例本来是要注册到服务端的，但是因为客户端和服务端连接断开了，操作没有执行成功。注册中心服务端并没有存放服务实例的信息，所以连接重建之后，肯定要把服务实例信息注册到服务端。这种情况显然是很容易理解的。

但是程序中可能还存在另外一种情况，**那就是客户端已经把服务实例注册到注册中心服务端了，但是因为某种原因客户端和服务端连接断开了，在这种情况下，客户端和新的服务器地址建立新的连接后，客户端还需要再把服务实例注册到服务端吗**？这也是连接恢复后的一种情况吧，**也就是说客户端早早就把服务实例注册到服务端了，并且也注册成功了，但是程序运行了一会，客户端和现有的服务端连接断开了，等客户端和新的服务器建立连接之后，客户端之前注册成功的服务实例，还需要再注册到新的服务器上吗**？我能想到大家的第一反应就是肯定不需要啊，既然客户端之前已经把服务实例成功注册到服务端了，**而 nacos 服务端又是集群，集群之间数据是可以复制的，这就意味着集群中的其他节点已经存放了客户端之前注册的服务实例的信息**，就算客户端和服务器集群中另外的节点建立了连接，也不用再把之前的服务实例信息重新注册一遍呀，这显然是有些多余了。大家有这种想法很正常，但是如果大家看过 nacos 服务端的源码之后，尤其是集群源码之后，就不会再这样想了。接下来，就让我为大家详细解释一下。

实际上，**当客户端和服务器集群中的某个服务器建立连接时，在服务器这边会专门为这个客户端建立一个 Client 对象，并且给它定义一个唯一的 ClientId；而当客户端把服务实例注册到该服务器上时，就会把服务实例存放在服务器为这个客户端建立的 Client 对象内部的 map 中，然后服务器的事件通知中心会发布对应的事件，比如说客户端变更事件，然后该服务器就会把客户端注册过来的服务实例信息同步到集群中的其他节点**。我现在讲解的这些内容大家不用去记啊，后面当我为大家实现服务端的时候，这些功能都会实现，现在大家只需要简单看看，对服务端存放服务实例信息有点了解就成。如果大家理解了刚才的内容后，接下来我还要再补充另外一些内容，假如客户端和服务端断开连接了，最终服务端那边会通过心跳机制感知到，感知到之后，服务端并不会直接把这个客户端注册的服务实例信息移除了，因为在整个集群中，可能并不是只有这一个客户端注册了这个服务实例，假如说有三个 RPC 框架的服务提供者都提供了相同的功能，那是不是这三个服务提供者都会把自己的服务实例信息注册到注册中心的服务端呢？这样一来不就有三个注册中心的客户端注册了三个相同的服务实例吗？如果仅仅因为一个客户端和服务器断开连接了，就把服务实例给移除了，那另外两个健康的客户端注册的服务实例不也就被移除了吗？所以，在 nacos 注册中心服务端这边，如果发现哪个客户端和服务端断开连接了，并不会移除对应的服务实例，而是会把服务器为客户端创建的 Client 对象的信息注销了。这个信息注销之后，这个客户端注册的服务实例信息也就不会再被订阅者持有了，而两外两个客户端注册的相同的服务实例信息还会被订阅者持有。这中间的过程等后面实现服务端功能的时候，大家就全清楚了，现在只要了解一个大概就行。并且，服务端感知到某个客户端断联之后，还会通过时间通知中心发布客户端断联事件，这个事件一旦发布，服务器中的集群处理器就会把客户端断联的消息告诉服务器集群中的其他节点，其他节点在接收到消息后，也会把对应的客户端连接给注销，这样一来集群中其他服务器也就相当于不再存储这个客户端注册的服务实例信息了。那么客户端和集群中新的服务器建立新的连接后，当然应该再把自己要注册的服务实例信息重新注册到注册中心服务端呀。不然 RPC 框架的消费者忽然少了一个服务提供者的信息，这对它内部的负载均衡也会造成一定影响。大家可以仔细品味品味这个逻辑，多看几遍我分析的这段文字。

  

如果大家理解了刚才我分析的服务端处理客户端连接断开的大概流程，现在应该就能意识到，**假如一个客户端之前已经把一个服务实例成功注册到注册中心服务端了，在和该服务端连接断开后，和新的服务器建立连接了，也要把服务实例再次注册一遍**的操作是合情合理的。那么这个功能该怎么实现呢？**这个功能显然应该严格监听客户端与服务端的连接是否断开了**，目前我为大家展示的程序显然还做不到这一点，目前框架实现的操作重试功能仅仅会在客户端执行操作过程中发现与服务器连接断开了，等连接重建之后要进行操作重试，也就是说，**目前我们的程序只能在某个具体操作执行失败之后，才会进行操作重试**；**而不能实时监控客户端与服务端连接，并且根据连接断开和重建情况，判断哪些操作需要重做，哪怕这些操作在之前已经成功了**。那现在程序该如何重构呢？

接下来就让我为大家简单分析一下这个功能的实现思路，其实我想说这个功能实现起来非常简单，但是这种话我已经说得够多了，我担心大家听烦了。但是，我还是要说，这个功能实现起来确实非常简单。根据刚才的分析，**我们都知道这个功能需要严格监听客户端与服务端的连接是否断开和重建，然后根据具体情况判断某些操作是否需要重做**。但是，这个时候请大家想一下，假如客户端已经把一个服务实例成功注册到了服务端，已经把操作重试对象中的 registered 成员变量更新为 true，表示注册成功了。就算连接断开了又重建了，操作重试对象的 registered 仍然为 true 呀，那么当 RedoScheduledTask 定时任务从 NamingGrpcRedoService 操作重试服务组件的 registeredInstances 成员变量中扫描到了对应的操作重试对象，结果发现操作重试对象的 registered 为 true，就会以为对应的服务实例已经注册成功了，就不会再执行重新注册的操作。这样一来，注册中心服务端不就少注册了一个服务实例吗？所以，我认为应该在客户端和服务端网络连接断开的时候执行一些操作，**比如说 NamingGrpcRedoService 操作重试服务组件监听到客户端和服务端网络连接断开了，就要立刻把成员变量 registeredInstances，也就是这个 map 中存放的所有操作重试对象的 registered 成员变量设置为 fasle，这样一来，不管这些操作重试对象对应的服务实例是否成功注册到服务端了，在连接重建之后都要再重新注册一遍。连接重建之后 RedoScheduledTask 定时任务从 NamingGrpcRedoService 操作重试服务组件的 registeredInstances 成员变量中扫描到了所有的操作重试对象，发现它们的 registered 都为 false，这时候 NamingGrpcRedoService 操作重试服务组件就会让所有操作重试对象执行重新注册服务实例到服务端的操作**。这个逻辑，大家可以认真品味一下。

如果上面这个逻辑大家都理清楚了，那么现在大家就会意识到，**解决问题的关键就在于怎么让 NamingGrpcRedoService 操作重试服务组件严格监听客户端与服务端的连接是否断开和重建，只有监听到客户端和服务端连接断开和重建了，NamingGrpcRedoService 操作重试服务组件才能在连接断开时把所有操作重试对象的 registered 成员变量更新为 fasle**。这时候，我们其实就应该定义一个监听器了，这个监听器专门监听客户端与服务端连接断开和重建事件。但是说实话，**这个所谓的监听器我早就给大家引入了，并且监听器中也定义了两个专门监听客户端与服务端连接断开和重建的方法，这个监听器接口就是 ConnectionEventListener，具体的实现类就是 NamingGrpcRedoService 操作重试服务组件**。现在大家应该明白 NamingGrpcRedoService 实现的 ConnectionEventListener 接口是做什么用的了吧？请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo;





//这个类的对象就是重做服务对象
public class NamingGrpcRedoService implements ConnectionEventListener {

    //这个成员变量就是一直执行重做操作的现成的名称
    private static final String REDO_THREAD_NAME = "com.alibaba.nacos.client.naming.grpc.redo";

    //执行重做操作线程的数量
    private int redoThreadCount;

    //重做操作的延迟时间，重做操作其实就是一个定时任务，这里定义的就是向定时任务调度其中提交定时任务时设定的延迟时间
    private long redoDelayTime;

    //存放注册操作的重做对象的map
    private final ConcurrentMap<String, InstanceRedoData> registeredInstances = new ConcurrentHashMap<>();

    //存放订阅重做操作对象的map，虽然在第三版本中还没有实现订阅服务端服务实例的操作，但下一版本就要实现了，所以这一版本就先把部分代码引入进来了
    private final ConcurrentMap<String, SubscriberRedoData> subscribes = new ConcurrentHashMap<>();

    //执行重做操作的定时任务执行器
    private final ScheduledExecutorService redoExecutor;

    //表示当前客户端和服务端是否成功建立了连接
    private volatile boolean connected = false;



    //其他内容省略



     //以下三个方法大家可以暂时忽略
    
    //判断当前客户端是否与服务端成功建立了连接的方法
    public boolean isConnected() {
        return connected;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：当客户端与服务端成功建立连接之后，这个方法就会被回调。还记得在NamingGrpcClientProxy类的start方法中的操作吗？就是rpcClient.registerConnectionListener(redoService)这个操作
     * 因为NamingGrpcRedoService对象本身就是要给监听器，专门监听客户端与服务端连接状态的，而在rpcClient中，有一个客户端事件执行器一直会判断客户端和服务端的连接状态是否发生了变化，并且根据变化执行监听器中的对应的回调方法
     * 这下子逻辑就串联起来了
     */
    @Override
    public void onConnected(Connection connection) {
        //当前方法被调用的时候，意味着客户端和服务端成功建立连接了
        connected = true;
        LogUtils.NAMING_LOGGER.info("Grpc connection connect");
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：当客户端与服务端连接断开时会调用这个方法
     */
    @Override
    public void onDisConnect(Connection connection) {
        //将连接标志社子为false
        connected = false;
        LogUtils.NAMING_LOGGER.warn("Grpc connection disconnect, mark to redo");
        synchronized (registeredInstances) {
            //因为连接断开了，这就意味着后面会进行重连，重连之后肯定要进行操作重做，把之前注册到服务端的实例重新注册到服务端
            //那怎么在重连之后判断服务实例要重新注册到服务端呢？这时候就可以做一个操作，那就是把服务实例重做对象中已注册的标志更新为false
            //等客户端重新连接了服务端之后，就会根据标志将服务实例重新注册到服务端
            registeredInstances.values().forEach(instanceRedoData -> instanceRedoData.setRegistered(false));
        }
        //下面和上面是同样的操作逻辑，只不过重置的是订阅操作的重做对象中的标志
        //这个逻辑大家可以在我提供的第六版本代码中查看，等后面讲完了第六版本代码再看吧，目前的第三版本代码还不太完善
        synchronized (subscribes) {
            subscribes.values().forEach(subscriberRedoData -> subscriberRedoData.setRegistered(false));
        }
        LogUtils.NAMING_LOGGER.warn("mark to redo completed");
    }


}
```

现在大家就知道了 NamingGrpcRedoService 操作重试组件中的 onConnected 方法和 onDisConnect 方法的具体作用了吧？onConnected 会在注册中心客户端和服务端连接重建的时候被回调，**而 onDisConnect 方法会在客户端和服务端连接断开时被回调。onDisConnect 方法被回调的时候，就会把所有操作重试对象的 registered 成员变量设置为 false；这样一来，等连接重建之后，客户端就会根据操作重试对象的 registered 的值，把服务实例重新注册到注册中心的服务端**。 好了，现在我们已经知道了 NamingGrpcRedoService 操作重试组件怎么根据客户端与服务端连接的断开和重建更新操作重试对象的成员变量的值了，**那么最关键的问题就来了，NamingGrpcRedoService 操作重试组件其实就是一个监听器，那么这个监听器怎么才能监听到客户端和服务端连接的断开和重建呢**？如果大家对这种所谓的成熟并且流行的开源框架已经祛魅了，那我相信大家很容易就能想到，**直接为客户端和服务端连接断开和重建添加监听器就行了**。

接下来的流程就很简单了，所以我就实现得快一点，请大家想一想，客户端和服务端连接的断开和重建是的操作在哪里执行？这就不用细想了，**肯定就是在 RpcClient 类的 reconnect 方法中执行的，客户端的健康检查机制会一直检测客户端和服务端连接是否健康，如果不健康了，就会执行 RpcClient 类的 reconnect 方法，和一个新的服务器建立新的连接**。具体逻辑请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;



@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {



    //省略其他内容



     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：重连服务器的方法
     */
    protected void reconnect(final ServerInfo recommendServerInfo, boolean onRequestFail) {
        try {
            
            
            //recommendServerInfo封装了要连接的新的服务器地址，当然也可能这个ServerInfo中什么信息也没封装
            AtomicReference<ServerInfo> recommendServer = new AtomicReference<>(recommendServerInfo);
            
            //判断onRequestFail是否为true，为true则意味着是客户端向服务端发送请求时一直失败，于是想要重新连接一个新的服务端
            //但这里还要再进行一下健康检测，如果健康检测成功，就说明当前连接是没有问题，可能是发送请求的过程出现问题了，这种情况是不会进行重连的
            if (onRequestFail && healthCheck()) {
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Server check success, currentServer is {} ",rpcClientConfig.name(), currentConnection.serverInfo.getAddress());
                //更新客户端状态
                rpcClientStatus.set(RpcClientStatus.RUNNING);
                //退出当前方法
                return;
            }

            
            //走到这里意味着确实需要重连，记录一下操作日志
            LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Try to reconnect to a new server, server is {}",rpcClientConfig.name(), recommendServerInfo == null ? " not appointed, will choose a random server.": (recommendServerInfo.getAddress() + ", will try it once."));
            
            
            //定义重连成功标志
            boolean switchSuccess = false;
            //定义重连次数
            int reConnectTimes = 0;
            //定义重试轮数
            int retryTurns = 0;
            //定义接收异常的变量
            Exception lastException;
            
            
            //在循环中执行重新连接服务端操作，除非重连成功或者客户端已经被关闭了才退出循环
            while (!switchSuccess && !isShutdown()) {
                ServerInfo serverInfo = null;
                try {
                    
                    
                    //判断recommendServer方法参数中是否封装了服务地址信息，如果封装了就是用方法参数对象中的服务地址
                    //如果没有封装则使用客户端服务地址管理器中的地址，这里大家就会发现，尽管在switchServerAsync方法中传入了参数null，这里依然可以从
                    //服务地址管理器中获得一个新的服务器地址
                    serverInfo = recommendServer.get() == null ? nextRpcServer() : recommendServer.get();
                    
                    
                    //连接服务器
                    Connection connectionNew = connectToServer(serverInfo);
                    
                    
                    //如果connectionNew不为null，说明连接创建成功了
                    if (connectionNew != null) {
                        //记录日志
                        LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Success to connect a server [{}], connectionId = {}",rpcClientConfig.name(), serverInfo.getAddress(),connectionNew.getConnectionId());
                        
                        //判断旧连接是否为null
                        if (currentConnection != null) {
                            
                            //记录日志
                            LoggerUtils.printIfInfoEnabled(LOGGER,"[{}] Abandon prev connection, server is {}, connectionId is {}",rpcClientConfig.name(), currentConnection.serverInfo.getAddress(),currentConnection.getConnectionId());
                            //抛弃旧连接
                            currentConnection.setAbandon(true);
                            //关闭旧连接
                            closeConnection(currentConnection);
                        }
                        
                        //更新currentConnection为新创建的连接对象
                        currentConnection = connectionNew;
                        
                        //更新客户端状态
                        rpcClientStatus.set(RpcClientStatus.RUNNING);
                        
                        //更新连接成功标志
                        switchSuccess = true;
                        
                        //退出循环
                        return;
                    }
                    
                    //走到这里意味着连接没有成功，那就判断一下客户端是否关闭了
                    if (isShutdown()) {
                        //如果客户端关闭，则关闭当前连接
                        closeConnection(currentConnection);
                    }
                    lastException = null;
                } catch (Exception e) {
                    lastException = e;
                } finally {
                    recommendServer.set(null);
                }
                
                
                //走到这里肯定意味着连接没有成功，接下来要再次重试了，既然是重试，肯定就要再次更换一个新的服务地址
                //所以先判断一下服务地址管理器是否为空
                if (CollectionUtils.isEmpty(RpcClient.this.serverListFactory.getServerList())) {
                    throw new Exception("server list is empty");
                }
                
                
                //这里的判断需要解释一下，reConnectTimes > 0很容易解释，意味着已经重试了好几次了
                //reConnectTimes % RpcClient.this.serverListFactory.getServerList().size() == 0就是重试的次数和服务地址管理器中的服务地址做除法，并且被整出了
                //这就意味着重试次数是服务地址列表的倍数，也就意味着肯定重试了有一轮了，并且仍然没有重试成功
                if (reConnectTimes > 0 && reConnectTimes % RpcClient.this.serverListFactory.getServerList().size() == 0) {
                    
                    //这时候记录以下操作日志
                    LoggerUtils.printIfInfoEnabled(LOGGER,"[{}] Fail to connect server, after trying {} times, last try server is {}, error = {}",rpcClientConfig.name(), reConnectTimes, serverInfo,lastException == null ? "unknown" : lastException);
                    
                    //这里判断一下重试的轮数，其实也是重试次数
                    //如果重试轮数等于最大整数值了
                    if (Integer.MAX_VALUE == retryTurns) {
                        //就把重试轮数重置为50
                        retryTurns = 50;
                    } else {//这里就是简单的自增重试轮数
                        retryTurns++;
                    }
                }
                
                //自增重试次数，这时候可能会有朋友觉得疑惑，想知道重试次数和重试轮数的区别，别急，下面就会看到了
                reConnectTimes++;
                
                try {
                    //判断客户端的状态，如果不是运行状态，说明客户端仍然没有成功连接到服务端
                    if (!isRunning()) {
                        //这时候就会让重连服务端的线程睡一会，具体睡多久，就根据刚才的retryTurns来定义，第一次重试让线程睡100ms，第二次睡200ms，最大睡5秒
                        //也许问题出在服务端那边，这样一来可以为服务端恢复自己的状态提供时间
                        Thread.sleep(Math.min(retryTurns + 1, 50) * 100L);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
            
            //每次重试的最后都会判断客户端是否已经关闭了，如果关闭就直接退出循环，不再重试
            if (isShutdown()) {
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Client is shutdown, stop reconnect to server", rpcClientConfig.name());
            }
        } catch (Exception e) {
            LoggerUtils.printIfWarnEnabled(LOGGER, "[{}] Fail to reconnect to server, error is {}", rpcClientConfig.name(), e);
        }
    }




    //省略其他内容
    
}
```

**在上面代码块的第 65 行，可以看到注册中心客户端和一个新的服务器建立了新的连接，在代码块的第 85 行，可以看到注册中心客户端在连接重建之后，把旧的连接关闭了，也可以认为是断开了**。这就是客户端和服务端连接断开和重建的关键操作。**如果我们在 RpcClient 类中定义一个客户端与服务端连接的事件队列、再定义一个连接事件监听器集合，并且定义一个连接事件对象，如果客户端和服务端新建连接了，就创建一个连接重建事件，把事件添加到事件队列中；如果客户端和服务端连接断开了，就创建一个连接断开事件，把这个事件也放到事件队列中。同时我们可以再定义一个定时任务，这个定时任务不断地从事件队列中取出连接事件，如果判断是连接重建事件，那就执行事件监听器中每一个监听器的 onConnected 方法；如果判断是连接断开事件，那就执行事件监听器中每一个监听器的 onDisConnect 方法**。

我把这个事件队列成员变量就定义为 eventLinkedBlockingQueue，把连接事件监听器集合成员变量定义为 ConnectionEvent；这两个成员变量都要定义在 RpcClient 类中；至于连接事件，我把它定义为 ConnectionEvent，它是 RpcClient 地一个内部类。接下来，就请大家看一下重构之后的 RpcClient 类，具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;



@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {




     //这个阻塞队列存放的是连接事件，当客户端访问服务端的时候，如果构建连接成功了，就会创建一个连接事件，存放到这个阻塞队列中
    //如果连接断开了，也会创建一个连接事件，只不过是断开连接事件，然后把这个连接事件存放到这个阻塞队列中
    protected BlockingQueue<ConnectionEvent> eventLinkedBlockingQueue = new LinkedBlockingQueue<>();


    //这个队列中存放的都是客户端连接事件监听器
    protected List<ConnectionEventListener> connectionEventListeners = new ArrayList<>();


    //客户端事件执行器
    protected ScheduledExecutorService clientEventExecutor;



    //连接事件内部类
    public static class ConnectionEvent {

        //该成员变量代表是连接成功事件
        public static final int CONNECTED = 1;

        //该成员变量代表是连接断开事件
        public static final int DISCONNECTED = 0;

        //事件类型
        int eventType;

        //连接对象
        Connection connection;

        public ConnectionEvent(int eventType, Connection connection) {
            this.eventType = eventType;
            this.connection = connection;
        }


        public boolean isConnected() {
            return eventType == CONNECTED;
        }


        public boolean isDisConnected() {
            return eventType == DISCONNECTED;
        }
    }


    

    //省略其他内容




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
        
        
        //这里就用到了上面执行其中的另一个线程，向客户端事件执行器中提交任务
        clientEventExecutor.submit(() -> {
            //在循环中执行这个任务，除非客户端被关闭了
            while (!clientEventExecutor.isTerminated() && !clientEventExecutor.isShutdown()) {
                ConnectionEvent take;
                try {
                    //从存放连接事件的队列中取出事件，如果没有连接事件，线程就会阻塞在这里，只要有数据可以返回
                    take = eventLinkedBlockingQueue.take();
                    //判断当前得到的连接事件是连接成功事件还是连接断开事件
                    if (take.isConnected()) {
                        //连接成功则执行notifyConnected方法
                        notifyConnected(take.connection);
                        //下面就是连接失败事件要执行notifyDisConnected方法的操作
                    } else if (take.isDisConnected()) {
                        notifyDisConnected(take.connection);
                    }
                } catch (Throwable e) {

                }
            }
        });
    

        //该方法其他内容省略
    
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：客户端与服务端连接成功时要执行的方法
     */
    protected void notifyConnected(Connection connection) {
        //首先判断连接状态队列中是否有数据，如果没有就直接退出当前方法
        if (connectionEventListeners.isEmpty()) {
            return;
        }
        LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Notify connected event to listeners.", rpcClientConfig.name());
        //走到这里意味着连接状态队列中存放着监听器对象，这时候就可以遍历队列，执行每一个监听器的onConnected方法
        for (ConnectionEventListener connectionEventListener : connectionEventListeners) {
            try {
                connectionEventListener.onConnected(connection);
            } catch (Throwable throwable) {
                LoggerUtils.printIfErrorEnabled(LOGGER, "[{}] Notify connect listener error, listener = {}",
                        rpcClientConfig.name(), connectionEventListener.getClass().getName());
            }
        }
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：当客户端与服务端连接断开的时候要执行的方法
     */
    protected void notifyDisConnected(Connection connection) {
        //首先判断连接状态队列中是否有数据，如果没有就直接退出当前方法
        if (connectionEventListeners.isEmpty()) {
            return;
        }
        LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Notify disconnected event to listeners", rpcClientConfig.name());
        //走到这里意味着连接状态队列中存放着监听器对象，这时候就可以遍历队列，执行每一个监听器的onDisConnect方法
        for (ConnectionEventListener connectionEventListener : connectionEventListeners) {
            try {
                connectionEventListener.onDisConnect(connection);
            } catch (Throwable throwable) {
                LoggerUtils.printIfErrorEnabled(LOGGER, "[{}] Notify disconnect listener error, listener = {}",
                        rpcClientConfig.name(), connectionEventListener.getClass().getName());
            }
        }
    }


    
    

    //连接事件就会在reconnect方法中被放到连接时间队列中




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：关闭客户端连接的方法，该方法会在下面的reconnect方法中被调用
     */
    private void closeConnection(Connection connection) {
        if (connection != null) {
            LOGGER.info("Close current connection " + connection.getConnectionId());
            //关闭客户端连接
            connection.close();
            //向连接事件队列中存放一个连接事件，这个事件就是断开连接的事件
            eventLinkedBlockingQueue.add(new ConnectionEvent(ConnectionEvent.DISCONNECTED, connection));
        }
    }

    

     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：重连服务器的方法
     */
    protected void reconnect(final ServerInfo recommendServerInfo, boolean onRequestFail) {
        try {
            
            
            //recommendServerInfo封装了要连接的新的服务器地址，当然也可能这个ServerInfo中什么信息也没封装
            AtomicReference<ServerInfo> recommendServer = new AtomicReference<>(recommendServerInfo);
            
            //判断onRequestFail是否为true，为true则意味着是客户端向服务端发送请求时一直失败，于是想要重新连接一个新的服务端
            //但这里还要再进行一下健康检测，如果健康检测成功，就说明当前连接是没有问题，可能是发送请求的过程出现问题了，这种情况是不会进行重连的
            if (onRequestFail && healthCheck()) {
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Server check success, currentServer is {} ",rpcClientConfig.name(), currentConnection.serverInfo.getAddress());
                //更新客户端状态
                rpcClientStatus.set(RpcClientStatus.RUNNING);
                //退出当前方法
                return;
            }

            
            //走到这里意味着确实需要重连，记录一下操作日志
            LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Try to reconnect to a new server, server is {}",rpcClientConfig.name(), recommendServerInfo == null ? " not appointed, will choose a random server.": (recommendServerInfo.getAddress() + ", will try it once."));
            
            
            //定义重连成功标志
            boolean switchSuccess = false;
            //定义重连次数
            int reConnectTimes = 0;
            //定义重试轮数
            int retryTurns = 0;
            //定义接收异常的变量
            Exception lastException;
            
            
            //在循环中执行重新连接服务端操作，除非重连成功或者客户端已经被关闭了才退出循环
            while (!switchSuccess && !isShutdown()) {
                ServerInfo serverInfo = null;
                try {
                    
                    
                    //判断recommendServer方法参数中是否封装了服务地址信息，如果封装了就是用方法参数对象中的服务地址
                    //如果没有封装则使用客户端服务地址管理器中的地址，这里大家就会发现，尽管在switchServerAsync方法中传入了参数null，这里依然可以从
                    //服务地址管理器中获得一个新的服务器地址
                    serverInfo = recommendServer.get() == null ? nextRpcServer() : recommendServer.get();
                    
                    
                    //连接服务器
                    Connection connectionNew = connectToServer(serverInfo);
                    
                    
                    //如果connectionNew不为null，说明连接创建成功了
                    if (connectionNew != null) {
                        //记录日志
                        LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Success to connect a server [{}], connectionId = {}",rpcClientConfig.name(), serverInfo.getAddress(),connectionNew.getConnectionId());
                        
                        //判断旧连接是否为null
                        if (currentConnection != null) {
                            
                            //记录日志
                            LoggerUtils.printIfInfoEnabled(LOGGER,"[{}] Abandon prev connection, server is {}, connectionId is {}",rpcClientConfig.name(), currentConnection.serverInfo.getAddress(),currentConnection.getConnectionId());
                            //抛弃旧连接
                            currentConnection.setAbandon(true);
                            //关闭旧连接
                            closeConnection(currentConnection);
                        }
                        
                        //更新currentConnection为新创建的连接对象
                        currentConnection = connectionNew;
                        
                        //更新客户端状态
                        rpcClientStatus.set(RpcClientStatus.RUNNING);
                        
                        //更新连接成功标志
                        switchSuccess = true;

                        //在这里向eventLinkedBlockingQueue事件队列中添加一个连接成功事件
                        eventLinkedBlockingQueue.add(new ConnectionEvent(ConnectionEvent.CONNECTED, currentConnection));
                        
                        //退出循环
                        return;
                    }
                    
                    //走到这里意味着连接没有成功，那就判断一下客户端是否关闭了
                    if (isShutdown()) {
                        //如果客户端关闭，则关闭当前连接
                        closeConnection(currentConnection);
                    }
                    lastException = null;
                } catch (Exception e) {
                    lastException = e;
                } finally {
                    recommendServer.set(null);
                }
                
                
                //走到这里肯定意味着连接没有成功，接下来要再次重试了，既然是重试，肯定就要再次更换一个新的服务地址
                //所以先判断一下服务地址管理器是否为空
                if (CollectionUtils.isEmpty(RpcClient.this.serverListFactory.getServerList())) {
                    throw new Exception("server list is empty");
                }
                
                
                //这里的判断需要解释一下，reConnectTimes > 0很容易解释，意味着已经重试了好几次了
                //reConnectTimes % RpcClient.this.serverListFactory.getServerList().size() == 0就是重试的次数和服务地址管理器中的服务地址做除法，并且被整出了
                //这就意味着重试次数是服务地址列表的倍数，也就意味着肯定重试了有一轮了，并且仍然没有重试成功
                if (reConnectTimes > 0 && reConnectTimes % RpcClient.this.serverListFactory.getServerList().size() == 0) {
                    
                    //这时候记录以下操作日志
                    LoggerUtils.printIfInfoEnabled(LOGGER,"[{}] Fail to connect server, after trying {} times, last try server is {}, error = {}",rpcClientConfig.name(), reConnectTimes, serverInfo,lastException == null ? "unknown" : lastException);
                    
                    //这里判断一下重试的轮数，其实也是重试次数
                    //如果重试轮数等于最大整数值了
                    if (Integer.MAX_VALUE == retryTurns) {
                        //就把重试轮数重置为50
                        retryTurns = 50;
                    } else {//这里就是简单的自增重试轮数
                        retryTurns++;
                    }
                }
                
                //自增重试次数，这时候可能会有朋友觉得疑惑，想知道重试次数和重试轮数的区别，别急，下面就会看到了
                reConnectTimes++;
                
                try {
                    //判断客户端的状态，如果不是运行状态，说明客户端仍然没有成功连接到服务端
                    if (!isRunning()) {
                        //这时候就会让重连服务端的线程睡一会，具体睡多久，就根据刚才的retryTurns来定义，第一次重试让线程睡100ms，第二次睡200ms，最大睡5秒
                        //也许问题出在服务端那边，这样一来可以为服务端恢复自己的状态提供时间
                        Thread.sleep(Math.min(retryTurns + 1, 50) * 100L);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
            
            //每次重试的最后都会判断客户端是否已经关闭了，如果关闭就直接退出循环，不再重试
            if (isShutdown()) {
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Client is shutdown, stop reconnect to server", rpcClientConfig.name());
            }
        } catch (Exception e) {
            LoggerUtils.printIfWarnEnabled(LOGGER, "[{}] Fail to reconnect to server, error is {}", rpcClientConfig.name(), e);
        }
    }




    //省略其他内容


    
}
```

上面代码块展示完毕之后，我相信大家都明白了连接事件监听器究竟是怎么发挥作用的。代码块中有非常详细的注释，所以我就不再重复讲解了。好了，到此为止，我们的问题就剩下一个了，**那就是 NamingGrpcRedoService 这个事件监听器究竟是怎么添加到 RpcClient 的 connectionEventListeners 集合中的呢**？NamingGrpcRedoService 实现了 ConnectionEventListener 监听器接口，所以 NamingGrpcRedoService 操作重试服务对象肯定是一个连接事件监听器，但是这个监听器只有存放到 RpcClient 的 connectionEventListeners 连接监听器集合中才能发挥作用。所以，接下来我们要实现的就是把 NamingGrpcRedoService 这个监听器对象添加到 RpcClient 的 connectionEventListeners 连接监听器集合中的功能。这个功能的实现也很简单，**我可以直接就在 RpcClient 中定义一个 registerConnectionListener 方法，这个方法就是用来把连接事件监听器添加到 RpcClient 的 connectionEventListeners 连接监听器集合中的**。具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;



@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {




     //这个阻塞队列存放的是连接事件，当客户端访问服务端的时候，如果构建连接成功了，就会创建一个连接事件，存放到这个阻塞队列中
    //如果连接断开了，也会创建一个连接事件，只不过是断开连接事件，然后把这个连接事件存放到这个阻塞队列中
    protected BlockingQueue<ConnectionEvent> eventLinkedBlockingQueue = new LinkedBlockingQueue<>();


    //这个队列中存放的都是客户端连接事件监听器
    protected List<ConnectionEventListener> connectionEventListeners = new ArrayList<>();


    //客户端事件执行器
    protected ScheduledExecutorService clientEventExecutor;




    //其他内容省略



    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：把连接事件监听器添加到RpcClient的connectionEventListeners连接监听器集合中的方法
     */ 
    public synchronized void registerConnectionListener(ConnectionEventListener connectionEventListener) {
        LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Registry connection listener to current client:{}",
                rpcClientConfig.name(), connectionEventListener.getClass().getName());
        this.connectionEventListeners.add(connectionEventListener);
    }





    //其他内容省略

}
```

至于 registerConnectionListener 方法的调用时机，这个也很好解决，**那就是在创建 NamingGrpcRedoService 这个事件监听器对象的时候，就直接把这个对象添加到 RpcClient 的 connectionEventListeners 连接监听器集合中即可**。**而 NamingGrpcRedoService 对象是在 NamingGrpcClientProxy 类中创建的，并且 RpcClient 对象也是在 NamingGrpcClientProxy类中创建的，所以正好可以在 NamingGrpcClientProxy 类中调用 RpcClient 对象的 registerConnectionListener 方法，把 NamingGrpcRedoService 这个连接事件监听器对象直接添加到 RpcClient 对象的 connectionEventListeners 集合中**。具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {


    //其他成员变量省略

    
    //该成员变量封装的就是断线重连后的操作，我愿意称它为重做服务对象
    private final NamingGrpcRedoService redoService;



    //构造方法，这里面的操作大多比较简单，注释就不加得很详细了
    public NamingGrpcClientProxy(String namespaceId, ServerListFactory serverListFactory, NacosClientProperties properties) throws NacosException {

        //其他内容省略

        //在这里创建了rpc客户端
        //RpcClientTlsConfig.properties(properties.asProperties()得到了一个tls配置类对象，创建rpc客户端时会用到
        this.rpcClient = RpcClientFactory.createClient(uuid, ConnectionType.GRPC, labels, RpcClientTlsConfig.properties(properties.asProperties()));
        
        //在这里创建断线重连操作的服务对象
        this.redoService = new NamingGrpcRedoService(this, properties);
        
        //启动rpc客户端
        start(serverListFactory);
    }



    //启动rpc客户端工作的方法
    private void start(ServerListFactory serverListFactory) throws NacosException {
        
        //把nacos服务地址管理器交给rpc客户端使用，ServerListManager实现了ServerListFactory接口，所以这里传递的是一个接口
        rpcClient.serverListFactory(serverListFactory);
        
        //注册断线重连监听器到客户端，监听器就是redoService对象，当客户端连接成功或者连接断开后，监听器就会执行相应的方法
        //这个时候，大家应该就明白了，第二版本代码中的连接事件是干什么用的了
        rpcClient.registerConnectionListener(redoService);
        
        //启动rpc客户端
        rpcClient.start();
    }


    //其他内容省略


}
```

到此为止，我就把**注册中心客户端实时监控自己与服务端连接，并且根据连接断开和重建情况，判断哪些操作需要重做的功能实现完毕了**。到此为止，实际上这个 NamingGrpcRedoService 操作重试服务组件已经具备了两种功能：**第一种就是在某个具体操作执行失败之后，根据操作重试对象中某些成员变量的值，判断是否进行操作重试**；**第二种就是实时监控客户端与服务端连接，并且根据连接断开和重建情况，判断哪些操作需要重做，哪怕这些操作在之前已经成功了**。这两种功能想必大家现在已经理解得差不多了。但是实际上，这个 NamingGrpcRedoService 操作重试服务组件还提供了第三种功能，第三种功能的成因也很直接，就是我在文章一开始就阐述的：**RedoScheduledTask 定时任务执行时机实际上非常模糊**。这句话单独理解可能有些晦涩，但是请大家思考一个具体的场景。

根据上一章讲解的知识，我们已经知道了 RedoScheduledTask 这个定期扫描操作重试对象是否需要执行操作重试的定时任务会在客户端启动三秒之后被第一次执行。具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo;



public class NamingGrpcRedoService implements ConnectionEventListener {


    //其他内容省略

    //执行重做操作线程的数量
    private int redoThreadCount;

    //重做操作的延迟时间，重做操作其实就是一个定时任务，这里定义的就是向定时任务调度其中提交定时任务时设定的延迟时间
    private long redoDelayTime;



    //构造方法
    public NamingGrpcRedoService(NamingGrpcClientProxy clientProxy, NacosClientProperties properties) {
        //根据配置信息给成员变量赋值
        setProperties(properties);
        //创建了定时任务执行器
        this.redoExecutor = new ScheduledThreadPoolExecutor(redoThreadCount, new NameThreadFactory(REDO_THREAD_NAME));
        //在这里向定时任务执行器提交了一个定时任务，并且这个定时任务会每隔一段时间就执行一次
        //而在RedoScheduledTask这个定时任务中，就会根据客户端和服务端的连接状况一直判断当前客户端是否要进行一些重做操作了
        //RedoScheduledTask定时任务执行任务的周期是3秒，客户端发送请求的超时时间也是3秒，也就是说如果发生网络断连
        //并且超过3秒了，网络恢复之后，RedoScheduledTask定时任务肯定可以检测到需要重新执行的任务
        this.redoExecutor.scheduleWithFixedDelay(new RedoScheduledTask(clientProxy, this), redoDelayTime, redoDelayTime,
                TimeUnit.MILLISECONDS);
    }


    
    //为成员变量赋值的方法
    private void setProperties(NacosClientProperties properties) {
        //这里就会给redoDelayTime赋值为3000L
        redoDelayTime = properties.getLong(PropertyKeyConst.REDO_DELAY_TIME, Constants.DEFAULT_REDO_DELAY_TIME);
        redoThreadCount = properties.getInteger(PropertyKeyConst.REDO_DELAY_THREAD_COUNT,
                Constants.DEFAULT_REDO_THREAD_COUNT);
    }



    //其他内容省略
 
}
```

很好，**既然 RedoScheduledTask 这个定时任务会在客户端启动第三秒之后会被执行，那假如我在客户端启动之后第二秒向服务端注册了一个服务实例，也就是说，在客户端启动第二秒，客户端就执行了注册服务实例的操作**；这个时候，肯定就会为这个操作创建一个对应的操作重试对象，并且把这个操作重试对象的 registered 成员变量初始化为 false。只有服务实力注册成功了，也就是说，只有 NamingGrpcClientProxy 对象的 requestToServer 方法执行成功了，才会把操作重试对象的 registered 成员变量更新为 true，表示服务实力注册成功了。但是，假如过客户端在发送注册服务实例请求给服务端的时候，网络出现了一点波动，这个请求已经发送了两秒了，还没有发送成功，但是也没有超时，**因为请求发送超时时间是 3 秒，所以不会触发连接重建的操作**。注意，客户端是在启动的第二秒发送注册服务实例请求的，这个请求在发送过程中又耗费了两秒，还没有成功。这时候，时间已经来到第 4 秒了，并且在第 3 秒的时候，**RedoScheduledTask 定时任务就已经启动了，并且开始扫描是否存在需要操作重试的对象**。这样一来，**客户端发送请求还没有完成，对应的操作重试对象的 registered 成员变量肯定为 false，而 RedoScheduledTask 定时任务已经开始工作。所以 RedoScheduledTask 定时任务肯定就会认为这个注册服务实例到服务端的操作没有成功，需要执行操作重试**。于是就会真的执行操作重试。我想这一点也是可以理解的。大家可以自己去代码中打断点制造这种情况，我已经亲自测试过了，这种情况肯定会发生。到此为止，我相信大家应该也能意识到了，**实际上在客户端内部，只要执行某个操作，发送请求的过程出现了一点波动，或者请求没有立刻完成，获得任务结果出现了延迟，这个 NamingGrpcRedoService 操作重试服务组件就会对某些操作进行重试**。

  

随着 NamingGrpcRedoService 操作重试服务组件的全部功能的实现，我想可能有朋友会觉得客户端操作重试的机会这么多，会不会对服务端的数据造成影响呢？这一点我也可以和大家解释一下，**实际上注册中心服务端做到了数据幂等性，也就是说就算客户端的某个操作真的执行了很多次，服务端存放的数据也不会被污染**。多余的内容我就不解释了，等我们一起实现注册中心服务端时，大家就知道是怎么回事了。现在让我来为大家总结一下 NamingGrpcRedoService 操作重试服务组件的作用，这个组件一共有三个作用：

**1 在客户端执行某些操作时，因为连接断开而操作失败了，在连接重建之后执行操作重试。**

**2 客户端执行的操作并没有失败，但是连接断开又重建了，这种情况下也会对操作进行重试。**

**3 客户端执行某些操作时，操作没有立刻成功，也没超时，这种情况下，操作重试服务组件也会对操作进行重试。**

以上就是 NamingGrpcRedoService 操作重试服务组件具备的三个功能。大家可以再仔细品味品味。接下来，还有最后一个知识点需要为大家补充。

## 分析操作重试对象成员变量的状态

在上一章，我为大家实现了 RedoData 类，并且也给大家展示了这个类的代码。这个类就是操作重试对象的抽象父类，重要的成员变量都定义在该类中。在展示这个类的代码时，有一个方法我一直没有为大家详细分析，就是下面代码块中的 getRedoType() 这个方法，**这个方法可以说是操作重试对象中最核心的方法，因为这个方法返回的就是操作重试对象应该执行的操作类型**。请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo.data;



@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RedoData<T> {




   //服务名称
    private final String serviceName;

    //组名
    private final String groupName;

    //最终期望的操作
    private volatile boolean expectedRegistered;

    //是否已注册
    private volatile boolean registered;

    //表示是否已注销
    private volatile boolean unregistering;

    //存放真正的数据，可能为Instance对象，也可能为String
    //这里之所以把服务实例对象设置进来，是因为操作重试对象要重试某些操作
    //比如说是注册服务实例的操作或者注销服务实例的操作，肯定要知道要操作的服务实例是什么吧
    
    private T data;


    //构造方法
    protected RedoData(String serviceName, String groupName) {
        this.serviceName = serviceName;
        this.groupName = groupName;
        this.expectedRegistered = true;
    }


    public String getServiceName() {
        return serviceName;
    }


    public String getGroupName() {
        return groupName;
    }


    //设置希望执行注册操作的标志
    public void setExpectedRegistered(boolean registered) {
        this.expectedRegistered = registered;
    }


    //判断希望执行注册操作的标志是true还是false
    public boolean isExpectedRegistered() {
        return expectedRegistered;
    }


    //判断本次服务注册操作是否已经成功执行
    public boolean isRegistered() {
        return registered;
    }


    //判断是否正在执行服务注销操作
    public boolean isUnregistering() {
        return unregistering;
    }


    //设置注册服务实例操作是否执行成功的标志
    public void setRegistered(boolean registered) {
        this.registered = registered;
    }


     //设置注销服务实例操作是否执行成功的标志
    public void setUnregistering(boolean unregistering) {
        this.unregistering = unregistering;
    }


    //得到服务实例对象
    public T get() {
        return data;
    }


    //设置服务实例对象
    public void set(T data) {
        this.data = data;
    }

    
    //设置已注册状态，在这里可以看到，如果一个服务实力注册成功了
    //就会把registered标志设置为true，把unregistering是否正在注销的标志设置为false
    public void registered() {
        this.registered = true;
        this.unregistering = false;
    }

    //设置已注销状态
    //这里可以看到，如果一个服务实例注销操作成功执行了，就会把registered已注册标志设置为false
    //把unregistering已注销标志设置为true
    public void unregistered() {
        this.registered = false;
        this.unregistering = true;
    }


    //判断操作是否需要重做的逻辑，核心逻辑就在getRedoType()方法中
    public boolean isNeedRedo() {
        return !RedoType.NONE.equals(getRedoType());
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：这个枚举类中的对象代表不同的要进行重试的操作
     */
    public enum RedoType {

        //需要进行注册操作
        REGISTER,

        //需要执行注销操作
        UNREGISTER,

        //不执行任何操作
        NONE,

        //从map中移除重做对象
        REMOVE;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：下面就是本类的核心逻辑了，判断在客户端与服务端重建连接之后，操作是否需要重做的逻辑，这个方法得到的就是当前重做对象需要执行的操作
     *  现在我还不给这个方法添加注释，因为很快我就要在文章中着重分析这里面的几种情况
     */
    public RedoType getRedoType() {
        
        if (isRegistered() && !isUnregistering()) {
            
            return expectedRegistered ? RedoType.NONE : RedoType.UNREGISTER;
        
        } else if (isRegistered() && isUnregistering()) {
            
            return RedoType.UNREGISTER;
           
        } else if (!isRegistered() && !isUnregistering()) {
           
            return RedoType.REGISTER;
        
        } else {
            
            return expectedRegistered ? RedoType.REGISTER : RedoType.REMOVE;
        }
    }

    
}
```

我之所以在上一章没有为大家分析这个方法，是因为大家还没有完全掌握操作重试服务组件执行操作重试的几种情况。但现在我们已经清楚了，所以接下来我就要为大家分析分着这个 getRedoType 方法了。

1 当 registered 为 true，unregistering 为 false 的时候，这就意味着服务实例已经注册到服务端了，并且服务实例并没有执行注销操作。因为只有在执行服务实例注销操作时，unregistering 才会被更新为 true，表示该服务实例正在注销。当注销操作完成后，才会把 registered 更新为 false，表示服务实例并没有注册。所以，**当 registered 为 true，unregistering 为 fasle 的时候，表示服务实例已经注册成功，那么服务实例什么操作也不用执行，直接返回 RedoType.NONE 枚举对象即可，意味着什么也不用做**。

2 当 registered 为 false，unregistering 为 false 时，首先看到 unregistering 为 false，就意味着并没有执行服务实例注销操作，只有在服务实例注销操作时，才会把 unregistering 更新为 true，这就意味着不管怎样，服务实例肯定要注册到服务端。接下来就分为两种情况，**一种是服务实例操作执行失败了，registered 成员变量没有机会被更新为 true；还有一种情况就是客户端早就把服务实例注册到服务端了，操作执行成功了，但是后来客户端和服务端连接发生中断了，中断的时候会调用事件监听器的 onDisConnect 方法，把操作重试对象的 registered 成员变量重新设置为 false**；总之不论是那种情况，都意味着应该进行操作重试。**所以这种情况应该返回 RedoType.REGISTER 枚举对象，意味着应该执行注册重试操作**。

3 当 registered 为 true，unregistering 也为 true 时，当 registered 为 true 时，就意味着服务实例已经成功注册到服务端了。但是紧接着会看见 unregistering 也为 true，**这就意味着客户端已经执行了服务实例注销操作，只不过操作没有成功，没有机会把 registered 更新为 false。所以这种情况下应该返回 RedoType.UNREGISTER 枚举对象，意味着应该重新执行注销服务实例操作**。

4 当 registered 为 false，unregistering 为 true 时，意味着服务实例注销操作已经执行完毕了，这个时候就不用再监听这个操作重试对象了，因为对应的服务实例已经注销了，不再提供服务了。**所以这个时候就直接返回 RedoType.REMOVE 枚举对象，意味着要从 NamingGrpcRedoService 的 registeredInstances 成员变量中移除操作重试对象，不用再对这个操作本身进行监控了**。当然，也可能是另一种情况，那就是服务实例本来已经成功注册到服务端了，并且后来又执行了服务注销操作，但是在执行服务注销操作时客户端与服务端连接断开了，操作执行失败，并且连接一断开，就把操作重试对象的 registered 更新为 false 了。这种情况也不是不可能出现，但即便是出现这种情况也无所谓，**因为本来客户端就要执行服务实例注销操作，现在连接断开了，服务端那边就不会再提供该客户端注册的服务实例信息了，也就相当于实现了服务实例注销操作。所以这种情况下，也是要返回 RedoType.REMOVE 枚举对象，意味着要从 NamingGrpcRedoService 的 registeredInstances 成员变量中移除操作重试对象**。

到此为止，我就把 getRedoType 方法为大家分析完毕了，当然，还有一个 expectedRegistered 成员变量没有分析，这个就交给大家自己分析吧，我就不再多解释了。大家只需要记住，不要太关心它的三元运算符是怎么写的，要弄清楚这个 expectedRegistered 成员变量和 registered、unregistering 这两个成员变量的关系，以及 expectedRegistered 成员变量的值会在什么情况下被更新。大家自己分析分析吧。

好了朋友们，这一章和上一章的内容其实并不怎么重要，但是占用了很长的篇幅，这两章内容都比较多，但其实这个操作重试功能实现得很简单，只不过其中有一些弯弯绕绕的逻辑，需要仔细分析才能理清楚，大家可以多看几遍文章，把整体逻辑理清楚就行，至于特别细节的地方，没必要研究得那么仔细，nacos 的这个失败重试功能设计得并不好，大家完全可以设计自己的失败重试功能。好了诸位，这一章就到此为止了，我们下一章见！