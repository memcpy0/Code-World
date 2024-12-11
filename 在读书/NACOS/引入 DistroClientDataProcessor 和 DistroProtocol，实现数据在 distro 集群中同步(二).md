在上一章我跟大家把 Distro 集群备份数据的大概过程分析了一下，并且在文章结尾，我也为大家总结了三个要点：

**1 Distro 集群中需要备份的数据只有服务实例信息。**

**2 理解清楚直连客户端和非直连客户端的概念。**

**3 Distro 集群节点在备份数据的时候，会把服务实例注册来源的客户端信息一起备份给其他节点，让其他节点根据客户端信息创建 Client 客户端对象，这样才能存放从其他节点备份过来的服务实例信息**

以上三个要点大家都理解了的话，这一章，我们就要正式开始实现 Dietro 集群备份数据的功能了。要实现这个功能，就要从功能的源头开始出发，开始分析，那么这个功能的源头是哪里呢？这不用说，肯定就是 Distro 集群节点开始备份数据的操作的时机，这个大家应该都同意吧？也就是说，我们首先要分析在什么情况下，Distro 集群中的节点才会执行备份数据的操作，只有这个源头找到了，接下来才能开始具体实现这个功能。所以，就让我们一起来分析一下 Distro 集群节点开始备份数据的时机吧。

## 分析 Distro 集群节点开始备份数据的时机

要分析 Distro 集群节点开始备份数据的时机其实也很容易，我不用说大家应该也能想到，**只要 nacos 服务端，也就是集群某个节点内部存储的服务实例信息发生变化，这时候就要触发节点备份数据的操作，因为节点要备份的数据就是服务实例信息，所以服务实例信息一旦发生变化，当前节点就要执行向其他节点备份数据的操作**。那么 nacos 服务端内部存储的服务实例信息发生变化有哪几种情况呢？**某个 nacos 客户端向服务端新注册了服务实例信息，这就是服务实例信息变化的一种情况；某个 nacos 客户端从服务端注销了某个服务实例信息，这也是服务实例信息发生变化的情况；某个 nacos 客户端向服务端更新了某个服务实例信息，这也是服务实例信息发生了变化的情况**。所以，现在我们可以总结一句：**只要 nacos 服务端存储的服务实例信息发生了增删改的情况，就意味着节点内部的服务实例信息发生了变化，这时候节点就要把最新的服务实例信息同步给集群的其他节点**。那么当前节点怎么就知道自己内部存储的服务实例信息发生了变化呢？

这个也太简单了，大家应该也已经猜到了，**现在又轮到万能的事件通知机制，也就是 NotifyCenter 通知中心登场了**。**如果我们在 nacos 服务端内部存储的服务实例发生变更的时候，发布一个事件，然后我们再定义一个订阅者，专门订阅这个事件。当订阅者接收到这个事件之后，就可以处理事件，然后对集群的其他节点发起数据备份的操作**。这个流程想必每一位朋友都能理解了吧？

那接下来就好办了，我们直接定义一个新的事件，当 nacos 服务端内部存储的服务实例发生变更时，直接发布这个事件即可。那现在我想说的是，其实这个事件早就定义好了，大家也早就见过了。因为大家都知道，nacos 服务端内部存储的服务实例发生变更时，最终操作都会执行到每一个 nacos 客户端在服务端对应的 Client 对象中，我把相关的代码搬运到这里了，请看下面代码块。

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
        //事件通知中心发布客户端变更事件，现在大家应该清楚这个事件的作用了吧？
        //节点存储的服务实例发生变更时，就会发布这个事件，相关的订阅者接收到该事件之后
        //就会处理该事件，然后执行向其他节点备份数据的操作，注意，这里发布客户端变更事件的时候
        //把当前客户端对象封装到该事件中了，因为要备份该客户端对象中的所有数据给集群的其他节点
        //所以要把客户端对象封装到这个事件中，方便处理事件的时候得到这个客户端对象
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
    

    
    //其他内容省略
}
```

从上面的代码块大家可以看到，当 nacos 服务端内部存储的服务实例信息发生改变时，最后的操作总会来到每一个 nacos 客户端在服务端对应的 Client 对象中，**如果是新注册了服务实例信息、或者是更新了某个服务实例信息，调用的就是 Client 客户端对象的 addServiceInstance() 方法，如果是注销了某个服务实例信息，调用的就是 Client 客户端对象的 removeServiceInstance() 方法**。这两个方法大家肯定都非常熟悉了，与此同时，**大家肯定能在这两个方法中看到一个相同点，那就是当服务实例信息成功变更之后，NotifyCenter 事件通知中心都发布了同一个事件，在上面代码块的第 43 行和第 68 行，事件通知中心都发布了一个 ClientChangedEvent 事件，ClientChangedEvent 就是客户端变更事件的意思**。现在我可以告诉大家，**这个事件其实就是我们本来要发布的新事件，也就是当 nacos 服务端内部存储的服务实例发生变更时，NotifyCenter 事件通知中心直接发布这个 ClientChangedEvent 事件即可，调表该客户端内部存储的服务实例信息发生变更了，要把这个客户端内部存储的服务实例信息同步给集群的其他节点**。现在大家应该清楚这个 ClientChangedEvent 事件的作用了吧？其实大家很久之前就见过这个事件了，只不过我一直没跟大家解释这个事件的具体作用，现在大家应该清楚了。

好了，当 nacos 服务端内部存储的服务实例发生变更时，NotifyCenter 事件通知中心要发布的 ClientChangedEvent 客户端变更事件已经定义完毕了，那接下来就该定义一个专门订阅这个事件的订阅者，用来接收并处理这个事件，而具体处理的逻辑，就是开始执行向集群其他节点备份服务实例最新信息的操作。那么这个订阅者对象该怎么定义呢？

## 引入 DistroClientDataProcessor 客户端数据处理器

如果大家对这个订阅者对象如何定义没有什么头绪，那就让我们使用最简单的思路分析分析。首先来给这个订阅者对象起一个名字吧，**既然这个订阅者对象是用来发起备份集群数据操作的，并且是以一个个 Client 客户端对象为单位开始备份的，那就把这个订阅者命名为 DistroClientDataProcessor 吧，翻译过来就是 Distro 集群的 Client 客户端数据处理器**。起这个名字想必大家也都可以接受。好了，现在这个订阅者的名字定义完毕了，接下来就该从这个订阅者要执行的操作出发，想想还要给这个订阅者定义什么功能。

这个其实也很简单，既然它是一个订阅者，那肯定要继承订阅者的抽象类，也就是 Subscriber 类，所以这个 DistroClientDataProcessor 客户端数据处理器可以先定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.naming.consistency.ephemeral.distro.v2;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群客户端数据处理器，这里大家一定要注意，这个处理器同时也是一个订阅者，因为它继承了SmartSubscriber类
 */
public class DistroClientDataProcessor extends Subscriber{


    //表示当前处理器要处理的数据类型，也就是Client客户端对象存储的数据
    //v2就是v2版本的意思，v1版本是旧版本nacos使用的版本
    public static final String TYPE = "Nacos:Naming:v2:ClientData";


    //构造方法
    public DistroClientDataProcessor() {
        //别忘了当前处理器本身也是一个订阅者，在这里把订阅者注册到事件通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:当前订阅者关注的事件
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //目前这个订阅者只关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:订阅者处理各个事件的方法
     */
    @Override
    public void onEvent(Event event) {
        //该方法暂时不做实现
    }



    //其他内容暂时不做实现
    
}
```

从上面代码块中可以看到，DistroClientDataProcessor 处理器实现了 Subscriber 抽象类的 subscribeTypes() 方法和 onEvent() 方法，当然，onEvent() 还没有具体实现，不过实现的逻辑也非常简单。反正这个订阅者对象订阅了 ClientChangedEvent 客户端变更事件，只要一接收到这个事件，就可以执行向集群其他节点备份数据的操作了。所以接下来我们就可以直接实现这个 onEvent() 方法。

但是在实现之前，我认为还有一个小细节应该考虑一下，那就是假如当前节点，也就是 nacos 服务端并不是以集群方式启动的，也就是说，当前服务器是以单机的方式启动的；那么当它内部服务实例信息发生变更之后，肯定就不需要把最新的服务实例信息备份给集群的其他节点。这一点大家可以理解吧？所**以要在 onEvent() 方法中判断一下当前节点是否是以集群模式启动的，如果是集群模式启动的，才需要把最新的服务实例信息备份给集群的其他节点，如果不是集群模式启动的，就可以直接退出当前方法**。那么这个 onEvent() 方法就可以先定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.naming.consistency.ephemeral.distro.v2;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群客户端数据处理器，这里大家一定要注意，这个处理器同时也是一个订阅者，因为它继承了SmartSubscriber类
 */
public class DistroClientDataProcessor extends Subscriber{


    //表示当前处理器要处理的数据类型，也就是Client客户端对象存储的数据
    //v2就是v2版本的意思，v1版本是旧版本nacos使用的版本
    public static final String TYPE = "Nacos:Naming:v2:ClientData";


    //构造方法
    public DistroClientDataProcessor() {
        //别忘了当前处理器本身也是一个订阅者，在这里把订阅者注册到事件通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:当前订阅者关注的事件
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //目前这个订阅者只关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:订阅者处理各个事件的方法
     */
    @Override
    public void onEvent(Event event) {
        
        //判断当前节点是否以单机模式启动
        if (EnvUtil.getStandaloneMode()) {
            //如果以单机模式启动就直接退出当前方法，不执行和Distro集群有关的任何操作
            return;
        }
        
    
        //走到这里意味着是集群模式启动的，直接处理事件即可
        //所谓的直接处理，就是把发生变更的Client内的服务实例信息同步给集群的其他节点
        syncToAllServer((ClientEvent) event);
        
    }



    //其他内容暂时不做实现
    
}
```

可以看到，我们已经初步实现了 DistroClientDataProcessor 客户端数据处理器的 onEvent() 方法，**在该方法中判断当前节点如果是以集群模式启动的，就紧接着执行 syncToAllServer() 方法，虽然该方法我们还没有实现，但是我已经在注释中解释了该方法的作用，在这个方法中就会把发生变更的 Client 内的服务实例信息同步给集群的其他节点**。如果这个逻辑大家都理解了，那么接下来我们就可以着手实现 syncToAllServer() 方法了。

## 区分直连客户端和非直连客户的重要性

好了，现在我们可以开始实现这个 syncToAllServer() 方法了，但是在实现之前还有一个非常重要的问题需要我们一起探讨一下，请大家根据刚才两个小节展示的内容思考一下，**假如集群中有一个节点接收到了来自其他节点备份的数据之后，是不是最终操作也会执行到这些服务实例信息所在的 Client 客户端对象中**？上一章我已经为大家明确了，不管是集群中的哪个节点，只要它是 nacos 服务端，那么它存储的服务实例信息最终肯定都会存储在对应的 Client 客户端对象中。所以一个节点接收到集群中其他节点备份过来的数据后，也会把这些数据存储在对应的 Client 客户端对象中，如果这个节点内部没有对应的 Client 客户端对象，就会先把这个 Client 客户端对象创建出来，然后把服务实例信息存储在里面。这个逻辑大家都理解吧？

如果是这样的话，那么 Distro 集群中的一个节点接收到来自其他节点备份过来的数据，不管是新增了服务实例信息还是注销了某个服务实例信息，最后肯定是执行到这个节点的 Client 客户端对象的 addServiceInstance() 方法或者是 removeServiceInstance() 方法，而在这两个方法中，不管是哪一个方法，在服务实例信息变更之后都会让 NotifyCenter 通知中心发布一个 ClientChangedEvent 客户端变更事件，这个逻辑大家也能理解吧？

其实问题就出在这里，别忘了，当前节点，也就是接收集群其他节点备份过来的数据的节点内部也有 DistroClientDataProcessor 订阅者对象，既然存在这个对象，这就意味着当前节点在接收到其他节点备份过来的数据之后，当前节点的 NotifyCenter 通知中心发布了 ClientChangedEvent 客户端变更事件之后，当前节点的 DistroClientDataProcessor 订阅者对象也会接收并处理这个事件，然后再把接收到的最新的服务实例信息备份给其他节点。奥找我们刚才分析的逻辑来说，程序的这段执行流程大家应该都能理解吧？很好，如果大家都能理解这段流程，那也就知道问题出在哪里了。

**当前节点明明是从其他节点接收到了备份过来的最新数据，然后这个节点还要再把这个数据备份给集群中的其他节点，如果按照这个逻辑，那么其他节点节点也接收到了备份过来的数据，是不是还要再备份给另外的节点呢？这样一来这一份数据就会在集群各个节点之间无休无止地备份下去了，也就是会出现备份风暴的情况**。换了谁来说，都会觉得这种情况非常离谱。显然应该对集群节点备份数据添加一个限制，禁止备份风暴这种情况出现。这一点大家应该也能理解吧？

好了，那我们现在要做的就是给集群的每一个节点添加限制，而这时候就要用到上一章我为大家引入的直连客户端和非直连客户端的概念了。原理其实很简单：**假如 Distro 集群中的一个节点接收到了新的服务实例信息，接收服务实例信息的节点就要判断一下这些服务实例信息要存储的客户端是不是 Client 直连客户端，如果是直连客户端，就意味着这些数据是从 nacos 客户端发送过来的，这时候 nacos 服务端就会发生服务实例更新的情况，就要把这些服务实例信息备份给集群的其他节点；如果判断出这些服务实例信息要存储的 Client 客户端对象是非直连客户端，这就意味着这些数据是从集群的其他节点那里备份过来的，就不必把这些数据再备份给集群的其他节点**。 这就是直连客户端和非直连客户端的重要作用！

如果以上直连客户端和非直连客户端的作用大家都理解了，那么接下来我就为大家解答一下，大家心里的另一个疑惑，那就是一个 Client 客户端怎么就成为直连客户端了？判断它是不是直连客户端的时候，又是怎么判断的呢？这部分的内容非常简单，我就简单解释一下吧。如果大家把前面多个版本代码都看完了，肯定会发现实际上在 Client 客户端对象中有一个 boolean 成员变量 isNative ，它表示当前客户端是否为本地的意思。如果为 true，就代表当前客户端为本地客户端，也就是直连客户端，否则就是非直连客户端。具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client.impl;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/17
 * @方法描述：客户端连接对象，服务器会为每一个接收到的客户端连接创建客户端连接对象，该客户端向服务器注册的服务实例，订阅的服务实例信息的订阅者信息，都会存放到它自己的客户端对象中
 */
public class ConnectionBasedClient extends AbstractClient {

    //客户端连接Id
    private final String connectionId;

    //表示当前客户端连接是否是直接连接到当前服务器的客户端的标志
    private final boolean isNative;

    //客户端最新的活跃时间，这个成员变量在集群情况下才会用到，目前还不会使用
    private volatile long lastRenewTime;

    //构造方法
    public ConnectionBasedClient(String connectionId, boolean isNative, Long revision) {
        super(revision);
        this.connectionId = connectionId;
        this.isNative = isNative;
        lastRenewTime = getLastUpdatedTime();
    }

    @Override
    public String getClientId() {
        return connectionId;
    }


    //是否为临时客户端
    @Override
    public boolean isEphemeral() {
        return true;
    }


    //是否为本地客户端
    public boolean isNative() {
        return isNative;
    }
}
```

从上面代码块中，我们还可以看到，ConnectionBasedClient 客户端对象的 isNative 成员变量会在构造方法中被复制，所以我们要想知道一个 ConnectionBasedClient 客户端对象怎么成为直连客户端，就要从 ConnectionBasedClient 客户端对象创建时的操作来入手了。所以，接下来我们就一起来看看 ConnectionBasedClient 客户端对象创建的时机，请看下面代码块。

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


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：同步客户端连接的方法，当前节点如果接收到Distro集群其他节点备份过来的数据后
     * 会为这些数据创建对应的客户端对象，这时候就会调用当前方法创建客户端对象，该方法在下一章就会用到
     */
    @Override
    public boolean syncClientConnected(String clientId, ClientAttributes attributes) {
        String type = attributes.getClientAttribute(ClientConstants.CONNECTION_TYPE);
        ClientFactory clientFactory = ClientFactoryHolder.getInstance().findClientFactory(type);
        return clientConnected(clientFactory.newSyncedClient(clientId, attributes));
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：判断当前要断开连接的客户端是否是直接连接到当前服务器的客户端的方法
     */
    @Override
    public boolean isResponsibleClient(Client client) {
        return (client instanceof ConnectionBasedClient) && ((ConnectionBasedClient) client).isNative();
    }


    

    //该类的其他内容省略
}
```

上面代码块展示了 ConnectionBasedClient 客户端对象创建的时机，可以看到，在上面代码块的第 59 行，也就是 clientConnected() 方法中，ConnectionBasedClient 对象是被 ClientFactory 客户端工厂对象创建的。除此之外，**我们还可以看到在 ConnectionBasedClientManager 客户端管理器中还添加了一个判断某个 Client 客户端对象是否为直连客户端的方法，也就是 isResponsibleClient() 方法**。大家可以认真看看这个方法的逻辑，其实就是判断这个 Client 客户端对象的 isNative 成员变量是否为 true。除此之外，我们还能在上面代码块中看到一个 syncClientConnected() 方法，这个方法的作用也很清楚了：**那就是当前节点如果接收到 Distro 集群其他节点备份过来的数据后，会为这些数据创建对应的客户端对象，这时候就会调用这个 syncClientConnected() 方法创建客户端对象**。当然，这个方法具体的调用时机我们现在还不必深究，下一章大家就会清楚了。总之，现在我们从 ConnectionBasedClient 类中发现，不管是在 clientConnected() 方法中，还是在 syncClientConnected() 方法中，Client 客户端对象都是由 ClientFactory 客户端工厂对象创建的，所以这个时候我们就要到 ClientFactory 工厂对象中看看 ConnectionBasedClient 客户端对象具体是怎么创建的了。我把相关的代码也搬运过来了，请看下面代码块。

```
package com.cqfy.nacos.naming.core.v2.client.factory.impl;



import static com.cqfy.nacos.naming.constants.ClientConstants.REVISION;


public class ConnectionBasedClientFactory implements ClientFactory<ConnectionBasedClient> {

    @Override
    public String getType() {
        return ClientConstants.DEFAULT_FACTORY;
    }


    //创建直连客户端的方法，在nacos服务端接收新的客户端连接时，就会调用这个方法为客户端连接创建客户端对象
    @Override
    public ConnectionBasedClient newClient(String clientId, ClientAttributes attributes) {
        long revision = attributes.getClientAttribute(REVISION, 0);
        ConnectionBasedClient connectionBasedClient = new ConnectionBasedClient(clientId, true, revision);
        connectionBasedClient.setAttributes(attributes);
        return connectionBasedClient;
    }


    //创建非直连客户端的方法
    @Override
    public ConnectionBasedClient newSyncedClient(String clientId, ClientAttributes attributes) {
        long revision = attributes.getClientAttribute(REVISION, 0);
        ConnectionBasedClient connectionBasedClient = new ConnectionBasedClient(clientId, false, revision);
        connectionBasedClient.setAttributes(attributes);
        return connectionBasedClient;
    }
}
```

从上面代码块中可以看到，在**使用 ClientFactory 客户端工厂对象创建 Client 客户端对象时，newClient() 方法创建的就是直连客户端，并且在创建的时候就在 ConnectionBasedClient 的构造方法中把 isNative 成员变量设置为 true 了；如果创建的是非直连客户端，那么在创建 ConnectionBasedClient 对象的时候就把 isNative 成员变量设置为 false 了**。到此为止，大家应该就彻底清除直连 Client 客户端对象和非直连客户端对象的创建过程了，也应该清楚怎样判断一个 ConnectionBasedClient 客户端对象是否为直连客户端了。好了，如果这些逻辑大家都清楚了，接下来我们就该继续重构 DistroClientDataProcessor 客户端数据处理器，也就是继续实现处理器的 syncToAllServer() 方法，之前已经分析了那么多，现在就没必要再仔细分析该方法的重构思路了，这时候直接看代码就行了，请看下面代码块。

```
package com.cqfy.nacos.naming.consistency.ephemeral.distro.v2;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群客户端数据处理器，这里大家一定要注意，这个处理器同时也是一个订阅者，因为它继承了SmartSubscriber类
 */
public class DistroClientDataProcessor extends Subscriber{


    //表示当前处理器要处理的数据类型，也就是Client客户端对象存储的数据
    //v2就是v2版本的意思，v1版本是旧版本nacos使用的版本
    public static final String TYPE = "Nacos:Naming:v2:ClientData";


    //构造方法
    public DistroClientDataProcessor() {
        //别忘了当前处理器本身也是一个订阅者，在这里把订阅者注册到事件通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:当前订阅者关注的事件
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //目前这个订阅者只关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:订阅者处理各个事件的方法
     */
    @Override
    public void onEvent(Event event) {
        
        //判断当前节点是否以单机模式启动
        if (EnvUtil.getStandaloneMode()) {
            //如果以单机模式启动就直接退出当前方法，不执行和Distro集群有关的任何操作
            return;
        }
        
    
        //走到这里意味着是集群模式启动的，直接处理事件即可
        //所谓的直接处理，就是把发生变更的Client内的服务实例信息同步给集群的其他节点
        syncToAllServer((ClientEvent) event);
        
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把客户端信息同步到Distro集群其他所有节点中的方法
     */
    private void syncToAllServer(ClientEvent event) {
        
        
        //从事件对象中得到要同步信息的客户端对象
        Client client = event.getClient();
        //判断客户端是否无效，这里其实就是判断当前客户端是否为直连客户端
        //如果不是直连客户端，那就没必要执行数据同步操作，这就防止了Distro集群中某个节点同步了其他节点最新的客户端数据之后，再次把同步过来的数据同步到另外的节点中
        if (isInvalidClient(client)) {
            return;
        }
        
        
        //接下来就是把客户端中的服务实例信息同步给集群其他节点的操作
    }



    //其他内容暂时不做实现
    
}
```

可以看到，在上面代码块我们确实把 syncToAllServer() 方法定义出来了，但是定义出来的方法只实现了一半，**那就是判断了一下要备份给其他节点的服务实例数据所在客户端是否为直连客户端，如果是非直连客户端则直接退出当前方法，不必执行数据备份的操作；如果是直连客户端，才继续向下执行，把客户端的服务实例信息同步给集群的其他节点**。我之所以没有把 syncToAllServer() 方法实现完毕，是因为我又遇到了一些问题。请大家想一想，我现在要把 Client 客户端对象内部存放的服务实例信息同步给集群的其他节点了，那要怎么同步呢？直接把 Client 客户端对象内部的数据都封装起来，然后传输给集群的其他节点吗？当然，最后我们肯定是要这么做的，但有必要现在就这么做吗？

我认为没有必要，请大家想一想，当前节点向集群其他节点备份数据的操作需要什么来执行？肯定需要执行器来执行，如果当前节点某个 Client 客户端对象内部服务实例信息变更特别频繁，难道每变更一次，就立刻向集群其他节点备份一次数据吗？显然不应该这么做。这时候大家应该回过味来了，这不就和我们之前刚刚实现的 nacos 服务端主动向 nacos 客户端推送最新的服务实例信息时的情况是一样的吗？**当时不是定义了一个延时任务执行器，还定义了一个非延迟任务执行器，延迟任务执行器就是把相同的任务聚合一下，然后等待任务延迟时间过去，再把任务提交给非延迟任务执行器执行，在非延迟任务执行器执行任务的过程中，才去 ServiceStorage 服务实例信息缓存器中查询最新的服务实例信息，然后把信息推送给 nacos 客户端**。这个逻辑大家应该还有印象吧？

如果 nacos 服务端主动向客户端推送最新服务实例信息的操作流程大家都还记得，那么现在 Distro 集群的某个节点把某个 Client 客户端对象最新服务实例信息备份给其他节点的流程也就很容易理解了。**首先我们肯定要为 Distro 集群定义一个延迟任务执行器，然后还要再为 Distro 集群定义一个非延迟任务执行器。然后再定义一个延迟任务对象，延迟任务对象应该封装要备份的服务实例所在客户端的信息，然后把延迟任务对象提交给延迟任务执行器；延迟任务执行器会把持有相同 Client 客户端信息的延迟任务聚合一下，因为两个任务持有的 Client 客户端对象相同，就意味着这两个任务都是要把同一个 Client 客户端下的服务实例信息备份给集群的其他节点，无非是这两个延迟任务创建有先后顺序而已；在等待了对应的延迟时间之后，延迟任务执行器会把延迟任务重新包装，然后提交给非延迟任务执行器，非延迟任务执行器执行任务的时候，会得到任务中持有的 Client 客户端信息，然后根据 Client 客户端信息取出该客户端对象内部所有的服务实例信息，再把这些服务实例信息备份给集群的其他节点即可**。大家可以仔细品味品味我阐述的这段逻辑，如果这段逻辑大家都梳理清楚了，那么 Distro 集群备份数据的核心内容也就被大家掌握了。

当然，我刚才给大家梳理的仍然是一个大概流程，或者说是编码思路和方向，具体实现还是要落实到代码上，肯定要为 Distro 集群引入很多重要组件。这些组件一时半会是实现不完的，所以接下来我们还是从最小的细节出发，看看我们下面要做些什么。刚才我们已经明确了，**不能在还未实现的 syncToAllServer() 方法中直接根据得到的 Client 客户端对象，就直接从这个对象中把服务实例信息取出来，然后备份给集群的其他节点**。如果该 Client 客户端对象内部的服务实例信息频繁变更，总不能每变更一次就把该对象内部所有服务实例信息取出来一次吧？**肯定是先把这个 Client 客户端对象的信息收集起来，等到过了任务延迟时间，真正要把这个 Client 客户端对象中的服务实例信息备份给集群其他节点时，再去这个 Client 客户端对象中查询最新的信息，然后备份给其他节点即可**。很好，分析到这里，**这个时候我们要定义的操作就明确了，那就是在 syncToAllServer() 方法中把得到的 Client 客户端的信息收集一下，然后把收集到的信息封装到延迟执行任务中，再把延迟执行任务提交给延迟任务执行器**。这个逻辑理解起来应改也没有什么问题吧？Client 客户端的重要信息其实就是客户端连接的唯一 Id，我也定义好了一个对象来封装客户端信息，新定义的对象就是 DistroKey 对象，该对象的具体内容请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro.entity;



public class DistroKey {

    //要处理的数据所属的客户端，也就是客户端连接Id
    private String resourceKey;

    //要处理的数据的类型
    private String resourceType;

    //数据要传输的目标节点
    private String targetServer;
    
    public DistroKey() {
    }
    
    public DistroKey(String resourceKey, String resourceType) {
        this.resourceKey = resourceKey;
        this.resourceType = resourceType;
    }
    
    public DistroKey(String resourceKey, String resourceType, String targetServer) {
        this.resourceKey = resourceKey;
        this.resourceType = resourceType;
        this.targetServer = targetServer;
    }
    
    //成员变量的get和set方法省略
}
```

接下来就是在 DistroClientDataProcessor 客户端数据处理器的 syncToAllServer() 方法中具体创建 DistroKey 对象的操作，请看下面代码块。

```
package com.cqfy.nacos.naming.consistency.ephemeral.distro.v2;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群客户端数据处理器，这里大家一定要注意，这个处理器同时也是一个订阅者，因为它继承了SmartSubscriber类
 */
public class DistroClientDataProcessor extends Subscriber{


    //表示当前处理器要处理的数据类型，也就是Client客户端对象存储的数据
    //v2就是v2版本的意思，v1版本是旧版本nacos使用的版本
    public static final String TYPE = "Nacos:Naming:v2:ClientData";


    //构造方法
    public DistroClientDataProcessor() {
        //别忘了当前处理器本身也是一个订阅者，在这里把订阅者注册到事件通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:当前订阅者关注的事件
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //目前这个订阅者只关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:订阅者处理各个事件的方法
     */
    @Override
    public void onEvent(Event event) {
        
        //判断当前节点是否以单机模式启动
        if (EnvUtil.getStandaloneMode()) {
            //如果以单机模式启动就直接退出当前方法，不执行和Distro集群有关的任何操作
            return;
        }
        
    
        //走到这里意味着是集群模式启动的，直接处理事件即可
        //所谓的直接处理，就是把发生变更的Client内的服务实例信息同步给集群的其他节点
        syncToAllServer((ClientEvent) event);
        
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把客户端信息同步到Distro集群其他所有节点中的方法
     */
    private void syncToAllServer(ClientEvent event) {
        
        
        //从事件对象中得到要同步信息的客户端对象
        Client client = event.getClient();
        //判断客户端是否无效，这里其实就是判断当前客户端是否为直连客户端
        //如果不是直连客户端，那就没必要执行数据同步操作，这就防止了Distro集群中某个节点同步了其他节点最新的客户端数据之后，再次把同步过来的数据同步到另外的节点中
        if (isInvalidClient(client)) {
            return;
        }

        //创建了DistroKey对象
        DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);
        //接下来就是创建延迟执行任务，然后把延迟任务提交给延迟任务执行器的操作
    }



    //其他内容暂时不做实现
    
}
```

可以看到，到此为止，DistroClientDataProcessor 客户端数据处理器的 syncToAllServer() 方法还是没有实现完毕，因为 Distro 集群的延迟执行任务和延迟任务执行器我们都还没有定义，要先把这两个重要组件定义完毕，才能继续实现 DistroClientDataProcessor 客户端数据处理器的 syncToAllServer() 方法。但是这两个组件在这一章显然是定义不完了，就留到下一章实现吧。这一章的内容也非常简单，功能实现也推进得非常慢，下一章节奏就会稍微快一下，因为核心知识已经梳理完毕了，剩下的只是代码实现了。现在大家仍然不能去看第 11 版本的代码，下一章结束之后应该就可以看我提供的第 11 版本代码了。好了朋友们，我们下一章见！