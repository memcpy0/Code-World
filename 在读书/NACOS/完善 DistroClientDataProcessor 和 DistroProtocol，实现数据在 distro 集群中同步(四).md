在上一章结尾，我们留下了三个问题等待解决，这三个问题就是：

**1 DistroClientDataProcessor 集群客户端数据处理器对象的创建时机。**

**2 DistroClientDataProcessor 对象该怎么使用延迟任务执行引擎呢？**

**3 DistroDelayTask 延迟任务处理器，也就是 DistroDelayTaskProcessor 类该怎么定义呢**？

其中第一个问题非常关键，如果 DistroClientDataProcessor 集群客户端数据处理器不被 springboot 容器管理，那么它就不会被自动创建，它的构造方法也就不会被调用，也就不会把自己这个订阅者注册到 NotifyCenter 事件通知中心，那么当 Distro 集群的某个节点内部发布了 ClientChangedEvent 客户端变更事件、ClientDisconnectEvent 客户端连接断开事件之后，当前节点的 DistroClientDataProcessor 订阅者就无法接收并处理这两个事件，那么把数据备份给集群其他节点的操作也就无从谈起了。所以，当务之急是先明确 DistroClientDataProcessor 集群客户端数据处理器的创建时机，至于剩下的两个问题则非常容易解决，所以暂时先放在一边。

## 再次重构 DistroClientDataProcessor 类

  

现在我们要分析 DistroClientDataProcessor 对象的创建时机，但是如果对着这个问题硬想，我相信一时半会大家也没什么头绪。所以接下来，我想先抛开这个问题，先分析分析别的。首先还是请大家先回顾一下我们上一章重构之后的 DistroClientDataProcessor 类，请看下面代码块。

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
        //关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        //关注客户端连接断开事件
        result.add(ClientEvent.ClientDisconnectEvent.class);
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
        //判断具体的事件类型
        if (event instanceof ClientEvent.ClientDisconnectEvent) {
            //走到这里意味着是客户端连接断开事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);

            //接下来就要定义一个DistroDelayTask延迟任务，
            //然后把延迟任务交给DistroDelayTaskExecuteEngine延迟任务执行引擎
            
        } else if (event instanceof ClientEvent.ClientChangedEvent) {
            //走到这里意味着是客户端变更事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);

            //接下来就要定义一个DistroDelayTask延迟任务，
            //然后把延迟任务交给DistroDelayTaskExecuteEngine延迟任务执行引擎
        }
    }



    //其他内容暂时不做实现
    
}
```

也许有的朋友有点疑惑，为什么我总是让大家回顾这个 DistroClientDataProcessor 类的内容。文章更新到这里，这个 DistroClientDataProcessor 类的内容似乎也没有发生什么大的变化。无非就是作为订阅者来说，订阅的事件多了一个，到此为止，这个 DistroClientDataProcessor 客户端数据处理器仍然没有实现把延迟任务提交给延迟任务执行器的操作。好了，不管怎么说吧，先让我们把这些遗留的问题放到一边，因为我忽然在这个 DistroClientDataProcessor 类上发现了一点新的东西，想跟大家分享一下。

大家看完刚才展示的代码块，对 DistroClientDataProcessor 类的印象肯定非常清晰，那就是它是一个订阅者的身份存在的。这个应该没有异议吧，毕竟从它内部定义的方法来看，它现在完全就是一个提供了订阅者功能的组件。但是接下来，**请大家把视角定格在 DistroClientDataProcessor 这个类的类名上，类名翻译过来是 Distro 集群客户端数据处理器的意思，所谓客户端其实就是每一个节点为 nacos 客户端连接创建的 Client 客户端对象**。 也许有朋友很早之前就感到困惑了，为什么明明是一个订阅者对象，却定义了一个客户端数据处理器的类名，如果大家之前对此没什么感觉，那现在我把这个问题提出来了，应该也会觉得疑惑了吧？所以，请大家想一想，**为什么从身份上来说，DistroClientDataProcessor 明明是一个订阅者，订阅了 ClientChangedEvent 客户端变更事件和 ClientDisconnectEvent 客户端连接断开事件，确把它的类名定义成了客户端数据处理器的意思呢**？

当然，如果思考得稍微粗暴一些，这个问题的答案也是显而易见的，**既然把一个订阅者定义为了客户端数据处理器，就意味着 DistroClientDataProcessor 类的对象有两种身份，一种是订阅者，一种是客户端数据处理器，两种身份就意味着这个 DistroClientDataProcessor 类的对象要承担两种职责**，这个答案应该说是合情合理的吧？

好了，如果大家同意我刚才给出的答案，那么现在问题就来了，**从目前实现的 DistroClientDataProcessor 类来看，我们只看到这个类的对象承担了订阅者的职责，并没有没看到这个类的对象对 Client 客户端对象存储的数据进行任何处理啊，也就是说 DistroClientDataProcessor 完全没有体现出客户端数据处理器的职责**。对于 DistroClientDataProcessor 类的名称来说，这显然是一个败笔。也许有朋友会觉得我对 DistroClientDataProcessor 类的名称进行一番分析非常可笑，这个类什么名称还不是我自己定义的，既然这个类的内容完全没体现出客户端数据处理器的职责，那就给这个类再换一个名称就好了，纠结这些无关紧要的内容干什么？没错，我确实也可以给 DistroClientDataProcessor 这个类再换一个名字，让这个类只负责订阅者的功能即可，但既然说到这里了，索性就敞开解释一下，**我为大家引入的这个 DistroClientDataProcessor 类就是按照 nacos 源码来命名的，换句话说，在 nacos 源码中就有 DistroClientDataProcessor 这个类，这个类既是订阅者又是客户端数据处理器**，所以我也是按照 nacos 源码中的 DistroClientDataProcessor 类的内容来给大家分析的。当然，大家尽可以当我是在马后炮。不过这些并不重要，能把功能分析清楚就行，接下来就请大家继续思考一个问题：**既然 DistroClientDataProcessor 类作为订阅者的同时，也需要体现出客户端数据处理器的功能，那怎么重构 DistroClientDataProcessor 类，给 DistroClientDataProcessor 类添加什么功能，才能让它体现出客户端数据处理器的功能呢**？

很好，问题已经明确了，就意味着我们要研究的方法明确了，接下来就很好办了。**要想让 DistroClientDataProcessor 类的对象体现出客户端数据处理器的功能，那我们就应该考虑一下，一个客户端数据处理器应该具备什么功能**。如果仍然从类名上研究，所谓的客户端处理器就是专门处理 Client 客户端对象存储的数据的处理器，那对于 Client 客户端对象存储的数据，究竟要执行什么操作呢？这个就很明显了，Distro 集群的每一个节点内部可能都创建了多个 Client 客户端对象，每一个 Client 客户端对象都存储了很多服务实例信息，而在 Distro 集群中，要对每一个节点的 Client 对象执行的操作非常简单：**那就是当 Distro 集群中某一个节点的 Client 客户端对象内部存储的服务实例信息发生变更了，该节点要把最新的服务实例信息同步给集群的其他节点。在这个操作执行的过程中，需要从服务实例发生变更的 Client 客户端对象中获得最新的所有服务实例信息；而当这些服务实例信息被同步给集群的其他节点之后，其他节点需要接收这些服务实例信息，然后更新自己内部对应的 Client 客户端对象存储的服务实例信息**。这个逻辑大家可以理解吧？如果这个逻辑大家理解了，那么 DistroClientDataProcessor 客户端数据处理器要定义的新功能也就明确了。

根据我们刚才的分析，DistroClientDataProcessor 客户端数据处理器中显然应该新定义两个功能：**第一个功能就是当 Distro 集群的一个节点要把某个 Client 客户端对象内部的服务实例信息备份给集群的其他节点时，DistroClientDataProcessor 客户端数据处理器可以直接得到该 Client 客户端对象内存放的所有服务实例信息；第二个要添加的功能就是当 Distro 集群中的某个节点接收到了从其他节点备份过来的数据，那么该节点可以直接使用 DistroClientDataProcessor 客户端数据处理器把这些数据更新给对应的 Client 客户端对象**。这就是应该向 DistroClientDataProcessor 客户端数据处理器中新添加的功能。既然 DistroClientDataProcessor 客户端数据处理器要和 Client 客户端对象打交道，而 Client 客户端对象需要从客户端管理器中获得，**那我们就可以把 ClientManager 客户端管理器定义为 DistroClientDataProcessor 类的成员变量**。当然，只实现了这些还不够，我们还应该再定义一个新的对象，这个对象就是用来封装从 Client 客户端中获得的所有服务实例信息的。原因很简单，从 Client 客户端对象中获得了最新的服务实例信息之后，要把这些服务实例信息备份给 Distro 集群中的其他节点呀，我们定义的这个新对象正好可以用来网络传输这些数据。我把这个对象定义为了 DistroData，具体实现请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro.entity;



public class DistroData {

    //数据标识符，标识该数据属于哪个客户端，以及是什么类型的数据
    private DistroKey distroKey;

    //操作类型符
    private DataOperation type;

    //真正要被处理的数据信息，也就是Client客户端存储的所有服务实例的信息
    private byte[] content;
    
    public DistroData() {
    }
    
    public DistroData(DistroKey distroKey, byte[] content) {
        this.distroKey = distroKey;
        this.content = content;
    }
    
    public DistroKey getDistroKey() {
        return distroKey;
    }
    
    public void setDistroKey(DistroKey distroKey) {
        this.distroKey = distroKey;
    }
    
    public DataOperation getType() {
        return type;
    }
    
    public void setType(DataOperation type) {
        this.type = type;
    }
    
    public byte[] getContent() {
        return content;
    }
    
    public void setContent(byte[] content) {
        this.content = content;
    }
}
```

好了，这个 DistroData 类展示完毕之后，接下来就应该根据我们之前的分析，重构一下 DistroClientDataProcessor 类了。我首先把第一个要添加的功能重构到 DistroClientDataProcessor 类中，请看下面代码块。

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

    //客户端管理器
    private final ClientManager clientManager;

    //构造方法
    public DistroClientDataProcessor(ClientManager clientManager) {
        this.clientManager = clientManager;
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
        //关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        //关注客户端连接断开事件
        result.add(ClientEvent.ClientDisconnectEvent.class);
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
        //判断具体的事件类型
        if (event instanceof ClientEvent.ClientDisconnectEvent) {
            //走到这里意味着是客户端连接断开事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);

            //接下来就要定义一个DistroDelayTask延迟任务，
            //然后把延迟任务交给DistroDelayTaskExecuteEngine延迟任务执行引擎
            
        } else if (event instanceof ClientEvent.ClientChangedEvent) {
            //走到这里意味着是客户端变更事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);

            //接下来就要定义一个DistroDelayTask延迟任务，
            //然后把延迟任务交给DistroDelayTaskExecuteEngine延迟任务执行引擎
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:判断客户端是否无效的方法
     */
    private boolean isInvalidClient(Client client) {
        //!clientManager.isResponsibleClient(client)这行代码非常重要，它会判断客户端是否为直连的客户端，如果不是直连客户端就无效
        return null == client || !client.isEphemeral() || !clientManager.isResponsibleClient(client);
    }





    //下面就是我为该类新添加的功能
    //也就是从客户端管理器中得到某个Client客户端对象存储的所有服务实例信息的功能



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:得到指定的客户端对象的全部服务实例数据的方法
     */
    @Override
    public DistroData getDistroData(DistroKey distroKey) {
        //根据客户端连接Id得到客户端对象
        Client client = clientManager.getClient(distroKey.getResourceKey());
        if (null == client) {
            return null;
        }
        //在这里得到当前客户端保存的全部数据，然后对其进行序列化
        //这里我们会看到调用了Client客户端对象的generateSyncData方法得到了存放在自己内部的所有服务实例信息
        //但是这个generateSyncData方法我们之前并没有为Client客户端对象实现，因为这是第11版本代码新添加的方法
        //我把为Client客户端新添加的generateSyncData方法也展示在当前代码块中了，大家可以简单看一看
        byte[] data = ApplicationUtils.getBean(Serializer.class).serialize(client.generateSyncData());
        return new DistroData(distroKey, data);
    }
    
    



    //其他内容暂时不做实现
    
}





//接下来就是第11版本代码为Client对象新添加的generateSyncData方法的代码

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
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:得到要同步到其他节点的客户端存储的的全部服务实例信息
     */
    @Override
    public ClientSyncData generateSyncData() {

        List<String> namespaces = new LinkedList<>();
        List<String> groupNames = new LinkedList<>();
        List<String> serviceNames = new LinkedList<>();

        List<String> batchNamespaces = new LinkedList<>();
        List<String> batchGroupNames = new LinkedList<>();
        List<String> batchServiceNames = new LinkedList<>();

        List<InstancePublishInfo> instances = new LinkedList<>();
        List<BatchInstancePublishInfo> batchInstancePublishInfos = new LinkedList<>();
        BatchInstanceData batchInstanceData = new BatchInstanceData();


        for (Map.Entry<Service, InstancePublishInfo> entry : publishers.entrySet()) {
            InstancePublishInfo instancePublishInfo = entry.getValue();
            if (instancePublishInfo instanceof BatchInstancePublishInfo) {
                BatchInstancePublishInfo batchInstance = (BatchInstancePublishInfo) instancePublishInfo;
                batchInstancePublishInfos.add(batchInstance);
                buildBatchInstanceData(batchInstanceData, batchNamespaces, batchGroupNames, batchServiceNames, entry);
                batchInstanceData.setBatchInstancePublishInfos(batchInstancePublishInfos);
            } else {
                namespaces.add(entry.getKey().getNamespace());
                groupNames.add(entry.getKey().getGroup());
                serviceNames.add(entry.getKey().getName());
                instances.add(entry.getValue());
            }
        }//在这里大家会看到一个ClientSyncData对象，这个ClientSyncData对象的内容我也展示在当前代码块中了
        ClientSyncData data = new ClientSyncData(getClientId(), namespaces, groupNames, serviceNames, instances, batchInstanceData);
        data.getAttributes().addClientAttribute(REVISION, getRevision());
        return data;
    }


    //该类的其他内容省略
    
}




//下面就是ClientSyncData类的内容

package com.cqfy.nacos.naming.core.v2.client;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:该类的对象封装了客户端全部数据
 */
public class ClientSyncData implements Serializable {

    private static final long serialVersionUID = -5141768777704539562L;

    private String clientId;

    private ClientAttributes attributes;

    private List<String> namespaces;

    private List<String> groupNames;

    private List<String> serviceNames;

    private List<InstancePublishInfo> instancePublishInfos;

    private BatchInstanceData batchInstanceData;

    //构造方法
    public ClientSyncData() {
    }

    //有参构造
    public ClientSyncData(String clientId, List<String> namespaces, List<String> groupNames, List<String> serviceNames,
                          List<InstancePublishInfo> instancePublishInfos,
                          BatchInstanceData batchInstanceData) {
        this.clientId = clientId;
        this.namespaces = namespaces;
        this.groupNames = groupNames;
        this.serviceNames = serviceNames;
        this.instancePublishInfos = instancePublishInfos;
        this.batchInstanceData = batchInstanceData;
        this.attributes = new ClientAttributes();
    }

    //其他get、set方法就省略了

}
```

好了朋友们，通过以上代码块，我想大家应该就清楚了 DistroClientDataProcessor 客户端数据处理器是怎么获得指定 Client 客户端对象内部存储的所有服务实例信息的了。**只要调用了 DistroClientDataProcessor 对象的 getDistroData() 方法，就可以得到该 Client 客户端对象内部存储的所有服务实例信息**。好了，第一个要添加的功能已经定义完毕了，接下来我就要继续重构 DistroClientDataProcessor 客户端数据处理器，把第二个功能，**也就是把从 Distro 集群其他节点接收到数据更新给对应的 Client 客户端对象的功能给实现了**。具体实现请看下面代码块。

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

    //客户端管理器
    private final ClientManager clientManager;

    //构造方法
    public DistroClientDataProcessor(ClientManager clientManager) {
        this.clientManager = clientManager;
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
        //关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        //关注客户端连接断开事件
        result.add(ClientEvent.ClientDisconnectEvent.class);
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
        //判断具体的事件类型
        if (event instanceof ClientEvent.ClientDisconnectEvent) {
            //走到这里意味着是客户端连接断开事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);

            //接下来就要定义一个DistroDelayTask延迟任务，
            //然后把延迟任务交给DistroDelayTaskExecuteEngine延迟任务执行引擎
            
        } else if (event instanceof ClientEvent.ClientChangedEvent) {
            //走到这里意味着是客户端变更事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);

            //接下来就要定义一个DistroDelayTask延迟任务，
            //然后把延迟任务交给DistroDelayTaskExecuteEngine延迟任务执行引擎
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:判断客户端是否无效的方法
     */
    private boolean isInvalidClient(Client client) {
        //!clientManager.isResponsibleClient(client)这行代码非常重要，它会判断客户端是否为直连的客户端，如果不是直连客户端就无效
        return null == client || !client.isEphemeral() || !clientManager.isResponsibleClient(client);
    }





    //下面就是我为该类新添加的功能
    //也就是从客户端管理器中得到某个Client客户端对象存储的所有服务实例信息的功能



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:得到指定的客户端对象的全部服务实例数据的方法
     */
    @Override
    public DistroData getDistroData(DistroKey distroKey) {
        //根据客户端连接Id得到客户端对象
        Client client = clientManager.getClient(distroKey.getResourceKey());
        if (null == client) {
            return null;
        }
        //在这里得到当前客户端保存的全部数据，然后对其进行序列化
        //这里我们会看到调用了Client客户端对象的generateSyncData方法得到了存放在自己内部的所有服务实例信息
        //但是这个generateSyncData方法我们之前并没有为Client客户端对象实现，因为这是第11版本代码新添加的方法
        //我把为Client客户端新添加的generateSyncData方法也展示在当前代码块中了，大家可以简单看一看
        byte[] data = ApplicationUtils.getBean(Serializer.class).serialize(client.generateSyncData());
        return new DistroData(distroKey, data);
    }
    
    


    //下面是要添加的第二个功能


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:处理从其他节点同步过来的数据的方法，这里可以看到，同步过来的就是DistroData对象
     * 该对象内部封装着某个Client客户端内部的所有服务实例信息
     */
    @Override
    public boolean processData(DistroData distroData) {
        //得到数据操作符
        switch (distroData.getType()) {
                //这个ADD操作符对应的操作大家先不必关心，后面很快就会降到
            case ADD:
            case CHANGE:
                //走到这里意味着是客户端数据同步操作
                ClientSyncData clientSyncData = ApplicationUtils.getBean(Serializer.class)
                        .deserialize(distroData.getContent(), ClientSyncData.class);
                //在这里同步客户端数据到本节点即可
                handlerClientSyncData(clientSyncData);
                return true;
            case DELETE:
                //走到这里就意味着是同步客户端连接断开事件，这里得到了要断开连接的客户端连接Id
                String deleteClientId = distroData.getDistroKey().getResourceKey();
                Loggers.DISTRO.info("[Client-Delete] Received distro client sync data {}", deleteClientId);
                //在这里断开客户端连接
                clientManager.clientDisconnected(deleteClientId);
                return true;
            default:
                //操作未知直接返回false
                return false;
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把从其他节点传输过来的客户端的最新数据同步到当前节点内部
     */
    private void handlerClientSyncData(ClientSyncData clientSyncData) {
        //记录操作日志
        Loggers.DISTRO.info("[Client-Add] Received distro client sync data {}, revision={}", clientSyncData.getClientId(),
                clientSyncData.getAttributes().getClientAttribute(ClientConstants.REVISION, 0L));
        //同步客户端连接，也就是说，假如是集群中某个节点新接收到了一个客户端连接，然后把这个客户端连接同步到了集群中的当前节点了
        //当前节点就会调用下面这个方法在自己内部也创建相应的客户端对象，也就是ConnectionBasedClient对象
        clientManager.syncClientConnected(clientSyncData.getClientId(), clientSyncData.getAttributes());
        //得到对应的ConnectionBasedClient对象
        Client client = clientManager.getClient(clientSyncData.getClientId());
        //然后更新客户端的最新信息
        upgradeClient(client, clientSyncData);
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:根据从其他节点传输过来的最新的客户端的信息，更新当前节点对应的客户端信息，该方法我就不添加注释了，就是更新客户端信息的方法，看着很长，但逻辑非常简单，一步步看就能看懂
     * 在阅读这个方法之前，大家一定要先把AbstractClient类的generateSyncData方法看了，因为这个generateSyncData方法会创建ClientSyncData对象
     * 根据这个方法大家会清楚地了解ClientSyncData对象封装客户端数据的逻辑，看完了AbstractClient类的generateSyncData方法之后，再来看这个方法就会觉得非常简单了，无非就是又把数据从ClientSyncData对象中取出来了
     */
    private void upgradeClient(Client client, ClientSyncData clientSyncData) {
        Set<Service> syncedService = new HashSet<>();
        processBatchInstanceDistroData(syncedService, client, clientSyncData);
        List<String> namespaces = clientSyncData.getNamespaces();
        List<String> groupNames = clientSyncData.getGroupNames();
        List<String> serviceNames = clientSyncData.getServiceNames();
        List<InstancePublishInfo> instances = clientSyncData.getInstancePublishInfos();
        for (int i = 0; i < namespaces.size(); i++) {
            Service service = Service.newService(namespaces.get(i), groupNames.get(i), serviceNames.get(i));
            Service singleton = ServiceManager.getInstance().getSingleton(service);
            syncedService.add(singleton);
            InstancePublishInfo instancePublishInfo = instances.get(i);
            if (!instancePublishInfo.equals(client.getInstancePublishInfo(singleton))) {
                client.addServiceInstance(singleton, instancePublishInfo);
                NotifyCenter.publishEvent(new ClientOperationEvent.ClientRegisterServiceEvent(singleton, client.getClientId()));
                NotifyCenter.publishEvent(new MetadataEvent.InstanceMetadataEvent(singleton, instancePublishInfo.getMetadataId(), false));
            }
        }
        for (Service each : client.getAllPublishedService()) {
            if (!syncedService.contains(each)) {
                client.removeServiceInstance(each);
                NotifyCenter.publishEvent(
                        new ClientOperationEvent.ClientDeregisterServiceEvent(each, client.getClientId()));
            }
        }//更新完之后在这里也更新了这个客户端对象的版本号，这样一来这个客户端的版本后就和同步数据过来的客户端版本号一致了
        client.setRevision(clientSyncData.getAttributes().<Integer>getClientAttribute(ClientConstants.REVISION, 0));
    }
    


    //其他内容暂时不做实现
    
}
```

上面代码块展示完毕之后，我们就可以知道了，**当 Distro 集群中的一个节点把数据备份给其他节点之后，接收备份数据的节点最终会把数据交给 DistroClientDataProcessor 客户端数据处理器来处理，调用该处理器的 processData() 方法，在该方法中判断要执行的备份操作是什么，然后对指定的 Client 客户端对象执行对应的操作**。这个逻辑想必也非常清楚了。当然，**接收备份数据的节点并不是直接就调用 DistroClientDataProcessor 处理器的 processData() 方法来接收数据，实际上还有外层的组件在工作，是外层的组件接收备份过来的数据，最后把数据交给 DistroClientDataProcessor 客户端数据处理器来处理**。这一点大家很快就会清楚了，现在大家先不必关心外层接收备份过来的数据的组件是什么，总之，现在我们已经把 DistroClientDataProcessor 类重构完毕了，这个类也承担了客户端数据处理器的职责，而且我们还可以再一次发现，**当 Distro 集群中的某个节点向其他节点备份数据的时候，确实是以 Client 客户端对象为单位进行数据备份的，每一次备份都会把对应的 Client 客户端对象下的所有服务实例信息全部备份给集群的其他节点**。

  

好了，到此为止，DistroClientDataProcessor 客户端数据处理器也就重构得差不多了，这个类已经同时具备了订阅者功能和客户端数据处理功能，我知道这时候肯定有朋友会好奇，如此大费周章地重构这个类，并且为这个 DistroClientDataProcessor 类定义处理 Client 客户端存储的数据的功能，究竟是为了什么呢？别急，接下来就让我为大家详细解答这个问题。

## 引入 DistroProtocol 组件

实际上，在 nacos 源码中为 Distro 集群中的各个组件划分了非常清晰的职责界限，就比如说在上一小节我为大家重构完毕的 DistroClientDataProcessor 客户端数据处理器，**在 nacos 源码中，这个类的对象就专门负责和 Client 客户端数据打交道，所谓打交道其实就是从对应的 Client 对象中获得所有服务实例信息，或者把从其他节点备份过来的服务实例信息更新到本节点对应的 Client 客户端对象中。所以这个类的名称才会定义为 DistroClientDataProcessor，也就是客户端数据处理器的意思**。好了，现在大家应该已经从 nacos 源码的角度清楚了 DistroClientDataProcessor 客户端数据处理器的由来，并且也知道了这个处理器只负责和 Client 客户端数据打交道，那这时候就应该有所意识了：**那就是把数据备份给 Distro 集群其他节点的操作并不是由该 DistroClientDataProcessor 处理器发起的，也就是说，我们并不能把 Distro 集群要用的延迟任务执行器交给 DistroClientDataProcessor 处理器使用，更不应该在该处理器的 syncToAllServer() 方法中创建延迟任务，然后把延迟任务提交给延迟任务执行器处理**。这个逻辑理解起来应该也没有那么困难吧？如果大家理解了这个逻辑，那么现在就请大家思考一下，**把数据备份给 Distro 集群其他节点的操作应该由哪个组件来执行呢？也就是说，应该把延迟任务执行器交给哪个组件使用，并且在这个组件中创建延迟任务，然后把延迟任务提交给延迟任务执行器呢**？这就是我们目前面临的问题。

既然我已经为大家引入了 nacos 源码的某些内容，所以我就不在这个问题上卖关子了。实际上，**在 nacos 源码中还定义了一个新的组件，就是本小节标题中写明的 DistroProtocol 组件，翻译过来就是 Distro 集群协议组件的意思**。**从这个组件的名字上也能看出来，在 nacos 源码中，这个组件就负责 Distro 集群层面上的所有操作**，那什么是 Distro 集群层面的操作呢？就目前的情况来说，Distro 集群层面的操作无非就两个：**一个就是当前节点把某个 Client 客户端内部最新的服务实例备份给集群的其他节点，这个操作的实现就要使用延迟任务和延迟任务执行引擎；另一个操作就是当前节点接收到从集群其他节点备份过来的某个 Client 客户端下的最新的服务实例信息，在这个操作执行的过程中，DistroProtocol 组件会把接收到的服务实例信息交给 DistroClientDataProcessor 客户端数据处理器，让客户端数据处理器把服务实例信息更新到对应的 Client 客户端对象内部**。由此可见，在源码中 Distro 集群中的各个组件的功能划分是非常清楚的。并且通过刚才的分析，我们也已经清楚了，**既然 Distro 集群备份数据的操作是由新定义的 DistroProtocol 组件负责的，那么延迟任务执行器肯定要交给这个 DistroProtocol 组件使用，并且延迟任务肯定也是在这个组件中创建的，然后提交给延迟任务执行器处理**。这个逻辑清楚了之后，接下来我们就趁热打铁，先把 DistroProtocol 组件给简单定义一下。当然一开始先不用关注太多功能，**就先实现当前节点把某个 Client 客户端内部最新的服务实例备份给集群的其他节点的功能即可**。请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro协议器，这个组件就是用来处理Distro集群中请求的
 */
@Component
public class DistroProtocol {


    //集群成员管理器
    private final ServerMemberManager memberManager;

    //Distro集群的任务执行引擎持有者，这个组件大家都还有印象吧？
    //上一章已经为大家展示过了，这个组件持有了Distro集群要用到的延迟任务执行引擎和非延迟任务执行引擎
    private final DistroTaskEngineHolder distroTaskEngineHolder;


    //构造方法
    public DistroProtocol(ServerMemberManager memberManager, DistroTaskEngineHolder distroTaskEngineHolder) {
        this.memberManager = memberManager;
        this.distroTaskEngineHolder = distroTaskEngineHolder;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把数据同步到集群其他节点的入口方法
     */
    public void sync(DistroKey distroKey, DataOperation action) {
        sync(distroKey, action, DistroConfig.getInstance().getSyncDelayMillis());
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把数据同步到集群其他节点的方法
     */
    public void sync(DistroKey distroKey, DataOperation action, long delay) {
        //遍历除自己之外的集群的所有节点
        for (Member each : memberManager.allMembersWithoutSelf()) {
            //把数据同步给遍历到的节点
            syncToTarget(distroKey, action, each.getAddress(), delay);
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把当前节点的客户端数据同步给Distro集群中目标节点的方法
     */
    public void syncToTarget(DistroKey distroKey, DataOperation action, String targetServer, long delay) {
        //创建要同步的数据的Distro集群key
        DistroKey distroKeyWithTarget = new DistroKey(distroKey.getResourceKey(), distroKey.getResourceType(), targetServer);
        //创建Distro集群延时任务对象
        DistroDelayTask distroDelayTask = new DistroDelayTask(distroKeyWithTarget, action, delay);
        //把Distro集群延时任务提交给延时任务执行引擎
        distroTaskEngineHolder.getDelayTaskExecuteEngine().addTask(distroKeyWithTarget, distroDelayTask);
        //记录日志信息
        if (Loggers.DISTRO.isDebugEnabled()) {
            Loggers.DISTRO.debug("[DISTRO-SCHEDULE] {} to {}", distroKey, targetServer);
        }
    }


    //其他内容暂时不做实现
}
```

从上面代码块中可以看到，DistroProtocol 这个类上添加了 springboot 的 @Component 注解，这就意味着这个 DistroProtocol 类的对象也是要交给 springboot 容器管理的。除此之外，我们还能看到这个 DistroProtocol 协议组件中定义了一些方法，这些方法就是用来执行把数据备份给集群其他节点操作的。通过上面代码块中的注释，我相信每一位朋友对程序的执行流程已经非常清楚了，**那就是只要当 DistroProtocol 组件的 sync() 方法一被调用，该组件就可以执行备份数据给其他节点的其实操作了，最后程序会执行到该组件的 syncToTarget() 方法中，在该方法中，就会创建向其他节点同步数据的延迟任务，然后把任务提交给延迟任务执行引擎**。这个流程理解起来应该也没有难度了吧？如果这个流程大家已经理解了，**那么现在我们就要想一想这个 DistroProtocol 的 sync() 方法究竟在哪里会被调用呢？因为只要这个方法被调用了，当前节点才会真正开始执行向其他节点备份数据的操作**。这就很好说了，还是寻找操作的起始源头即可。Distro 集群的一个节点什么时候才会把数据备份给集群的其他节点呢？当然是某个 Client 客户端存储的数据发生变更时，那 Client 客户端存储的数据什么时候会发生变更呢？当然是程序的 NotifyCenter 事件通知中心发布了 ClientChangedEvent 或者 ClientDisconnectEvent 事件呀？那这些事件会被谁处理呢？当然是 DistroClientDataProcessor 客户端数据处理器呀。答案就这样分析出来了，接下来，**我们只需要在 DistroClientDataProcessor 客户端数据处理器处理这两个事件的时候调用 DistroProtocol 组件的 sync() 方法即可**。**这也就意味着 DistroClientDataProcessor 客户端数据处理器是要持有 DistroProtocol 协议组件的**。好了，接下来就让我为大家展示一下最后重构的 DistroClientDataProcessor 类，请看下面代码块。

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

    //客户端管理器
    private final ClientManager clientManager;

    //Distro集群协议器
    private final DistroProtocol distroProtocol;

    //构造方法
    public DistroClientDataProcessor(ClientManager clientManager, DistroProtocol distroProtocol) {
        this.clientManager = clientManager;
         this.distroProtocol = distroProtocol;
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
        //关注客户端变更事件
        result.add(ClientEvent.ClientChangedEvent.class);
        //关注客户端连接断开事件
        result.add(ClientEvent.ClientDisconnectEvent.class);
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


    //接下来就是最终重构之后的syncToAllServer()方法

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
        //判断具体的事件类型
        if (event instanceof ClientEvent.ClientDisconnectEvent) {
            //走到这里意味着是客户端连接断开事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);
            //那就把这个客户端信息同步到集群其他节点
            distroProtocol.sync(distroKey, DataOperation.DELETE);
        } else if (event instanceof ClientEvent.ClientChangedEvent) {
            //走到这里意味着是客户端变更事件
            DistroKey distroKey = new DistroKey(client.getClientId(), TYPE);
            //在这里把客户端最新信息同步给集群其他节点
            distroProtocol.sync(distroKey, DataOperation.CHANGE);
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:判断客户端是否无效的方法
     */
    private boolean isInvalidClient(Client client) {
        //!clientManager.isResponsibleClient(client)这行代码非常重要，它会判断客户端是否为直连的客户端，如果不是直连客户端就无效
        return null == client || !client.isEphemeral() || !clientManager.isResponsibleClient(client);
    }


    //其他内容暂时省略
    
}
```

到此为止，DistroClientDataProcessor 客户端数据处理器才算重构得差不多了，要用到的各个组件也都定义为它的成员变量了，大家也应该清楚了 Distro 的节点备份数据的操作流程了吧？当然，还有后续的流程我没有给大家实现，比如延迟任务该怎么被处理，然后怎么提交给非延迟任务执行引擎执行等等。这个马上就会讲到了，大家没必要着急。接下来，让我们把目光继续集中在 DistroProtocol 组件上，看看它的第二个功能要怎么实现。所谓第二个功能，**就是当前节点接收从集群其他节点备份过来的某个 Client 客户端下的最新的服务实例信息，在这个操作执行的过程中，DistroProtocol 组件会把接收到的服务实例信息交给 DistroClientDataProcessor 客户端数据处理器，让客户端数据处理器把服务实例信息更新到对应的 Client 客户端对象内部**。这一点之前已经分析过了，接下来我们就一起来看看这个功能该怎么实现。

如果大家对之前的内容还有印象，肯定还记得，**当 Distro 集群中的某一个节点把服务实例备份给集群的其他节点时，会把要备份的服务实例信息封装到一个 DistroData 对象中，然后把这个对象传输给集群的其他节点。这样一来，当集群的其他节点接收到备份过来的数据时，肯定也是接收到了一个 DistroData 对象**。既然是这样，那 DistroProtocol 组件的第二个功能就很好实现了，**我们直接给这个组件定义一个 onReceive() 方法，在这个方法中接收从其他节点传输过来的 DistroData 对象，然后把该对象交给 DistroClientDataProcessor 客户端数据处理器处理即可。如果这个逻辑大家都能理解，那么就应该能想到，DistroClientDataProcessor 客户端数据处理器显然就应该交给 DistroProtocol 协议组件使用**。理解这一点也没问题吧？好了，接下来我就为大家展示一下实现了第二个功能的 DistroProtocol 组件，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro协议器，这个组件就是用来处理Distro集群中请求的
 */
@Component
public class DistroProtocol {


    //集群成员管理器
    private final ServerMemberManager memberManager;

    //Distro集群的任务执行引擎持有者，这个组件大家都还有印象吧？
    //上一章已经为大家展示过了，这个组件持有了Distro集群要用到的延迟任务执行引擎和非延迟任务执行引擎
    private final DistroTaskEngineHolder distroTaskEngineHolder;

    //虽然把客户端数据处理器定义为DistroProtocol的成员变量了，但是这个客户端处理器该怎么创建呢？
    //实际上这个DistroClientDataProcessor对象并不是DistroProtocol的成员变量，我先写成这样
    //后面讲解之后大家就知道是怎么回事了
    private final DistroClientDataProcessor dataProcessor;


    //构造方法
    public DistroProtocol(ServerMemberManager memberManager, DistroTaskEngineHolder distroTaskEngineHolder) {
        this.memberManager = memberManager;
        this.distroTaskEngineHolder = distroTaskEngineHolder;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把数据同步到集群其他节点的入口方法
     */
    public void sync(DistroKey distroKey, DataOperation action) {
        sync(distroKey, action, DistroConfig.getInstance().getSyncDelayMillis());
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把数据同步到集群其他节点的方法
     */
    public void sync(DistroKey distroKey, DataOperation action, long delay) {
        //遍历除自己之外的集群的所有节点
        for (Member each : memberManager.allMembersWithoutSelf()) {
            //把数据同步给遍历到的节点
            syncToTarget(distroKey, action, each.getAddress(), delay);
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把当前节点的客户端数据同步给Distro集群中目标节点的方法
     */
    public void syncToTarget(DistroKey distroKey, DataOperation action, String targetServer, long delay) {
        //创建要同步的数据的Distro集群key
        DistroKey distroKeyWithTarget = new DistroKey(distroKey.getResourceKey(), distroKey.getResourceType(), targetServer);
        //创建Distro集群延时任务对象
        DistroDelayTask distroDelayTask = new DistroDelayTask(distroKeyWithTarget, action, delay);
        //把Distro集群延时任务提交给延时任务执行引擎
        distroTaskEngineHolder.getDelayTaskExecuteEngine().addTask(distroKeyWithTarget, distroDelayTask);
        //记录日志信息
        if (Loggers.DISTRO.isDebugEnabled()) {
            Loggers.DISTRO.debug("[DISTRO-SCHEDULE] {} to {}", distroKey, targetServer);
        }
    }




    //下面就是添加的第二个功能


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:同步从集群中其他节点传输过来的数据的方法
     */
    public boolean onReceive(DistroData distroData) {
        //记录日志信息
        Loggers.DISTRO.info("[DISTRO] Receive distro data type: {}, key: {}", distroData.getType(), distroData.getDistroKey());
        //判断处理器是否为null
        if (null == dataProcessor) {
            Loggers.DISTRO.warn("[DISTRO] Can't find data process for received data {}", resourceType);
            return false;
        }
        //使用刚才得到的数据处理器处理数据，把其他节点传输过来的数据同步到当前节点中
        return dataProcessor.processData(distroData);
    }

    
    

    //其他内容暂时不做实现
}
```

上面代码块展示完毕之后，现在大家应该也清楚了 DistroProtocol 协议组件是怎么接收从集群其他节点备份过来的数据的了。只需要调用 DistroProtocol 对象的 onReceive() 方法就可以接收到从其他节点传输过来的 DistroData 对象，而 DistroData 对象封装了需要备份的服务实例信息，把它交给客户端数据处理器直接处理即可。这个逻辑非常简单，大家肯定都能理解。但是，随着上面代码块的展示，我相信很多朋友心里肯定产生了更多的疑问，至少有两个疑问：

1 **那就是在我们刚刚展示的 DistroProtocol 类中，虽然把 DistroClientDataProcessor 客户端数据处理器定义为它的成员变量了，但是我并没有在 DistroProtocol 类中把 DistroClientDataProcessor 对象创建出来，因为我们到现在也没有 DistroClientDataProcessor 对象的创建时机，这个时机不确定，DistroClientDataProcessor 对象就无法被创建，也就无法在 DistroProtocol 协议组件和 NotifyCenter 事件通知中心中发挥作用**。

2 **我们都知道，只要 DistroProtocol 组件的 onReceive() 方法被调用了，当前节点就可以处理从其他节点接收到的备份过来的数据，那 DistroProtocol 组件的 onReceive() 方法应该在哪里被调用呢？只有这个方法被调用的时机弄清楚了，我们才知道当前节点究竟是怎么接收到其他节点备份过来的数据的**。

我相信目前困扰着大家的就是这两个问题。当然，我们在上一章遗留的三个问题还有两个也没有得到解决，就是以下两个问题：

**1 DistroClientDataProcessor 集群客户端数据处理器对象的创建时机。**

**2 DistroDelayTask 延迟任务处理器，也就是 DistroDelayTaskProcessor 类该怎么定义呢？**

本来上一章遗留了三个问题，但这三个问题中的第二个问题是 **DistroClientDataProcessor 对象该怎么使用延迟任务执行引擎？现在我们已经知道了延迟任务执行引擎并不能交给 DistroClientDataProcessor 对象使用，而应该交给 DistroProtocol 组件使用**，所以这个问题就算是解决了。总之到目前为止，我们还剩下四个问题没有解决。

并且我们还可以看到，在这四个问题中，有两个问题都和 DistroClientDataProcessor 客户端数据处理器的创建时机有关。但这个问题我们迟迟没有解决，接下来就应该正式解决它了。之前一直不解决这个问题是因为时机还不太成熟，现在时机已经成熟了。我这么说也是有原因的，因为我们已经为 Distro 集群定义了非常多的组件了，接下来就让我们从这方面入手，看看这个问题该怎么解决。

  

## 引入 DistroDataRequestHandler 处理器和 Distro 集群组件持有者 DistroComponentHolder

首先让我们先来看看，到目前为止我们为了实现 Distro 集群备份数据的操作，已经为 Distro 集群引入了多少组件了。**DistroProtocol 协议组件已经引入了，DistroClientDataProcessor 客户端数据处理器也引入了，持有者延迟任务执行引擎和非延迟任务执行引擎的 DistroTaskEngineHolder 引擎持有者也引入了**。总的来说，到目前为止已经引入了三个非常重要的组件。

**其中 DistroProtocol 协议组件就是用来执行 Distro 集群层面的操作的；DistroClientDataProcessor 客户端数据处理器就是专门用来和 Client 客户端数据打交道的；DistroTaskEngineHolder 引擎持有者就是为其他组件提供延迟任务执行引擎和非延迟任务执行引擎的**。这三个组件的作用非常清楚。但是，仅仅靠这三个组件，就目前的情况来说，我们还无法彻底完成 Distro 集群备份数据给其他节点的功能。原因很简单，也很直接，**虽然这三个组件负责的功能非常重要，但还欠缺最后一个非常重要的组件，那就是网络数据传输组件，没有这个网络数据传输组件，节点也不可能把要备份的数据传输给集群的其他节点啊**。也许有朋友会说，网络数据传输组件不就是网络通信组件吗？这个组件在前面构建 Distro 集群的时候不是已经实现了吗？就是所谓的 ClusterRpcClientProxy 组件，我把这个 ClusterRpcClientProxy 集群客户端代理器的内容也搬运过来了，帮助大家回顾一下，请看下面代码块。

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

在看完上面代码块展示的内容之后，大家应该也会同意我直接把 ClusterRpcClientProxy 类的对象当成 Distro 集群的数据传输组件，反正从这个 ClusterRpcClientProxy 类的对象中就可以获得集群其他节点的客户端，就可以使用这些客户端和集群中各自对应的节点进行通信，传输要备份的数据，这么一来，网络数据传输组件不就早实现了吗？没错，确实是这个道理，**但是在 nacos 源码中，又对这个 ClusterRpcClientProxy 类做了一层封装，也就是说为 Distro 集群定义了一个专门的网络数据传输组件，这个组件内部持有了 ClusterRpcClientProxy 对象，这个新定义的网络数据传输组件真正发送数据的时候使用的仍然是 ClusterRpcClientProxy 对象，但是在发送数据之前还做了一些额外的操作，比如说检查集群中的目标节点是否处于健康状态**。接下来我就为大家展示一下专门为 Distro 集群新定义的这个网络数据传输组件，我把它定义为了 DistroClientTransportAgent 类，具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.consistency.ephemeral.distro.v2;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群客户端数据传输代理器
 */
public class DistroClientTransportAgent implements DistroTransportAgent {


    //集群客户端对象，用来和集群的各个节点进行通信
    private final ClusterRpcClientProxy clusterRpcClientProxy;

    //集群成员管理器
    private final ServerMemberManager memberManager;

    //构造方法
    public DistroClientTransportAgent(ClusterRpcClientProxy clusterRpcClientProxy,
                                      ServerMemberManager serverMemberManager) {
        this.clusterRpcClientProxy = clusterRpcClientProxy;
        this.memberManager = serverMemberManager;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:把数据同步到目标节点的方法
     */
    @Override
    public boolean syncData(DistroData data, String targetServer) {
        //判断目标服务器是否不存在
        if (isNoExistTarget(targetServer)) {
            //不存在则默认同步数据成功
            return true;
        }
        //创建请求对象
        DistroDataRequest request = new DistroDataRequest(data, data.getType());
        //根据目标节点信息得到目标节点对象
        Member member = memberManager.find(targetServer);
        //检查目标节点的状态是否健康
        if (checkTargetServerStatusUnhealthy(member)) {
            Loggers.DISTRO.warn("[DISTRO] Cancel distro sync caused by target server {} unhealthy, key: {}", targetServer, data.getDistroKey());
            return false;
        }
        try {
            //在这里使用集群客户端代理器把请求发送给目标节点，并且得到响应
            Response response = clusterRpcClientProxy.sendRequest(member, request);
            //检查响应是否成功
            return checkResponse(response);
        } catch (NacosException e) {
            Loggers.DISTRO.error("[DISTRO-FAILED] Sync distro data failed! key: {}", data.getDistroKey(), e);
        }
        return false;
    }
    


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:带有回调方法的同步数据到目标节点的方法，该方法逻辑和上面的方法类似，我就不重复添加注释了
     */
    @Override
    public void syncData(DistroData data, String targetServer, DistroCallback callback) {

        if (isNoExistTarget(targetServer)) {
            callback.onSuccess();
            return;
        }
        DistroDataRequest request = new DistroDataRequest(data, data.getType());
        Member member = memberManager.find(targetServer);
        if (checkTargetServerStatusUnhealthy(member)) {
            Loggers.DISTRO.warn("[DISTRO] Cancel distro sync caused by target server {} unhealthy, key: {}", targetServer, data.getDistroKey());
            callback.onFailed(null);
            return;
        }
        try {
            //在这里执行异步发送请求给目标节点的操作，同时也定义了一个回调对象
            clusterRpcClientProxy.asyncRequest(member, request, new DistroRpcCallbackWrapper(callback, member));
        } catch (NacosException nacosException) {
            callback.onFailed(nacosException);
        }
    }


    //其他内容暂时不做实现
    
}
```

在看到 DistroClientTransportAgent 组件的内容之后，现在大家是不是觉得思路越来越清晰了？**当 Distro 集群的某个节点要把数据备份给集群中的其他节点时，最终就是通过 DistroClientTransportAgent 组件的 syncData() 方法发送出去的，在该方法中，创建了一个 DistroDataRequest 请求对象，并且把要备份的数据封装到该对象中传输给目标节点了**。这个逻辑大家应该也清楚了吧？但随着这个 DistroClientTransportAgent 组件的定义，现在又出现了两个新的问题，**那就是这个 DistroClientTransportAgent 组件该怎么被创建呢？它可没有添加什么 springboot 的注解，也就意味着这个类的对象需要 nacos 自己创建；第二个问题就是现在我们终于发现集群节点备份数据的时候，是通过 DistroDataRequest 请求把数据传输给目标节点的，那么这个请求到了目标节点之后该怎么被接收呢**？

第二个问题非常容易解决，**直接定义一个请求处理器即可，反正在 nacos 框架中有非常多的请求处理器，每一个请求处理器都对应着一个请求，当节点接收到 DistroDataRequest 请求之后，就使用对应的请求处理器处理该请求即可。这个请求处理器我也定义完毕了，就定义为了 DistroDataRequestHandler 请求处理器**，该处理器的具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.remote.rpc.handler;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:distro集群的请求处理器，只要是在distro集群中发送的各个请求，都交给这个处理器来接收并处理
 */
@Component
public class DistroDataRequestHandler extends RequestHandler<DistroDataRequest, DistroDataResponse> {

    //distro集群协议器，可以看到，这个请求处理器持有了DistroProtocol组件
    //要在这个类的方法中调用DistroProtocol组件的onReceive()方法处理接收到的数据
    private final DistroProtocol distroProtocol;


    public DistroDataRequestHandler(DistroProtocol distroProtocol) {
        this.distroProtocol = distroProtocol;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:本类核心方法，处理distro集群请求的方法，这里大家也可以看到，实际上在distro集群中只发送一个请求
     * 那就是DistroDataRequest请求，只不过这个请求可能封装了不同的操作符，以此来表明该请求代表的是什么操作
     */
    @Override
    public DistroDataResponse handle(DistroDataRequest request, RequestMeta meta) throws NacosException {
        try {
            //首先从请求中得到操作符
            //看到这个方法的时候，大家应该也就清楚了，实际上在Distro集群中存在着很多操作，比如验证每个节点备份的数据是否一致的操作
            //新加入集群的节点从某个节点下载快照的操作，服务实例变更备份数据的操作，查询数据的操作等等
            //我们这几篇文章都是围绕着数据备份来讲解的，其他操作都没有讲解，但是其他操作的执行流程和数据备份的流程几乎一样
            //所以大家只要把数据备份的流程掌握了，其他的操作自己看代码也能看懂
            //我提供的第11版本代码注释非常详细，这篇文章结束之后，第11版本代码的内容就全都可以查看了
            switch (request.getDataOperation()) {
                case VERIFY:
                    //走到这里意味着是验证请求，也就是验证集群中客户端信息是否一致的请求
                    //这里就是处理验证请求的操作
                    return handleVerify(request.getDistroData(), meta);
                case SNAPSHOT:
                    //走到这里就意味着是快照请求，处理快照请求即可
                    return handleSnapshot();
                case ADD:
                case CHANGE:
                case DELETE:
                    //走到这里意味着可能是数据同步请求，所谓数据同步请求其实就是客户端信息增加、变更、移除等等请求
                    return handleSyncData(request.getDistroData());
                case QUERY:
                    //走到这里就是查询请求
                    return handleQueryData(request.getDistroData());
                default:
                    //走到这里意味着请求未知，直接返回一个空响应对象
                    return new DistroDataResponse();
            }
        } catch (Exception e) {
            //以下就是出现异常的情况，返回一个错误响应即可
            Loggers.DISTRO.error("[DISTRO-FAILED] distro handle with exception", e);
            DistroDataResponse result = new DistroDataResponse();
            result.setErrorCode(ResponseCode.FAIL.getCode());
            result.setMessage("handle distro request with exception");
            return result;
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:处理客户端信息验证的方法
     */
    private DistroDataResponse handleVerify(DistroData distroData, RequestMeta meta) {
        //首先创建一个响应对象
        DistroDataResponse result = new DistroDataResponse();
        //使用distro协议器验证客户端信息
        if (!distroProtocol.onVerify(distroData, meta.getClientIp())) {
            result.setErrorInfo(ResponseCode.FAIL.getCode(), "[DISTRO-FAILED] distro data verify failed");
        }//返回响应对象即可
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:处理快照的请求，其实这些请求都会被distro协议器进一步处理，所以我就不在这里添加详细注释了
     * 大家可以直接去DistroProtocol对象中找到处理请求的方法，然后查看具体逻辑即可，我把详细逻辑都添加在DistroProtocol对象中了
     */
    private DistroDataResponse handleSnapshot() {
        DistroDataResponse result = new DistroDataResponse();
        DistroData distroData = distroProtocol.onSnapshot(DistroClientDataProcessor.TYPE);
        result.setDistroData(distroData);
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:处理数据同步请求的方法
     */
    private DistroDataResponse handleSyncData(DistroData distroData) {
        DistroDataResponse result = new DistroDataResponse();
        //可以看到，在这里调用了distroProtocol协议组件的onReceive()方法处理接收到的数据
        //看到这里，之前留下的4个问题中的第二个问题就解决了
        if (!distroProtocol.onReceive(distroData)) {
            result.setErrorCode(ResponseCode.FAIL.getCode());
            result.setMessage("[DISTRO-FAILED] distro data handle failed");
        }
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:处理查询请求的方法
     */
    private DistroDataResponse handleQueryData(DistroData distroData) {
        DistroDataResponse result = new DistroDataResponse();
        DistroKey distroKey = distroData.getDistroKey();
        DistroData queryData = distroProtocol.onQuery(distroKey);
        result.setDistroData(queryData);
        return result;
    }
}
```

好了，现在 DistroDataRequestHandler 请求处理器也定义完毕了，大家也知道了封装着要备份的数据的 DistroDataRequest 请求究竟是怎么被接收的了。接下来还是回过头看看刚才的问题，那就是DistroClientTransportAgent 组件该怎么被创建呢？这个问题我就直接解答了吧？现在我们已经为 Distro 集群引入了非常多的组件了，并且通过刚才展示的几个组件的内容，**大家可以看到有些组件也会被其他组件使用，有些组件被 springboot 容器管理了，而有的组件并没有被 springboot 容器管理**。就目前的情况来看，这几个组件是比较散乱的，没有被系统的管理，所以接下来我要定义一个新的组件，**那就是 DistroComponentHolder 类，翻译过来就是 Distro 集群所有组件持有者的意思，这个类的对象就持有了我们之前定义的所有要被 Distro 集群用到的组件，这样一来，只要程序哪个地方需要用到某个 Distro 集群的组件，直接就可以从 DistroComponentHolder 对象中获取即可。而 DistroComponentHolder 对象则被 springboot 容器管理，这样它就可以随时被其它对象或者组件获取了**。接下来就请大家看一下我新定义的 DistroComponentHolder 类，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro.component;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群组件持有者，这个类的对象持有了在Distro集群中会用到的全部组件
 */
@Component
public class DistroComponentHolder {

    //存储网络数据传输器
    private final Map<String, DistroTransportAgent> transportAgentMap = new HashMap<>();

    //存储客户端数据处理器
    private final Map<String, DistroDataStorage> dataStorageMap = new HashMap<>();

    //存储Distro集群操作失败充实处理器
    private final Map<String, DistroFailedTaskHandler> failedTaskHandlerMap = new HashMap<>();

    //这个也是存储客户端数据处理器
    private final Map<String, DistroDataProcessor> dataProcessorMap = new HashMap<>();
    
    public DistroTransportAgent findTransportAgent(String type) {
        return transportAgentMap.get(type);
    }
    
    public void registerTransportAgent(String type, DistroTransportAgent transportAgent) {
        transportAgentMap.put(type, transportAgent);
    }
    
    public DistroDataStorage findDataStorage(String type) {
        return dataStorageMap.get(type);
    }
    
    public void registerDataStorage(String type, DistroDataStorage dataStorage) {
        dataStorageMap.put(type, dataStorage);
    }
    
    public Set<String> getDataStorageTypes() {
        return dataStorageMap.keySet();
    }
    
    public DistroFailedTaskHandler findFailedTaskHandler(String type) {
        return failedTaskHandlerMap.get(type);
    }
    
    public void registerFailedTaskHandler(String type, DistroFailedTaskHandler failedTaskHandler) {
        failedTaskHandlerMap.put(type, failedTaskHandler);
    }
    
    public void registerDataProcessor(DistroDataProcessor dataProcessor) {
        dataProcessorMap.putIfAbsent(dataProcessor.processType(), dataProcessor);
    }
    
    public DistroDataProcessor findDataProcessor(String processType) {
        return dataProcessorMap.get(processType);
    }
}
```

可以看到，我新定义的新的 DistroComponentHolder 组件持有了网络数据传输器和客户端数据处理器，至于 DistroProtocol 协议组件和 DistroComponentHolder 任务执行引擎持有者则并没有被 DistroComponentHolder组件管理，原因也很简单，因为这两个组件都被 springboot 容器管理了。当然，**我现在只是把 DistroComponentHolder 组件定义完毕了，那么网络数据传输器和客户端数据处理器怎么被设置到这个 DistroComponentHolder 组件中呢**？很好，这时候终于涉及到这两个组件的创建时机了。

实际上在 nacos 服务端源码中还定义了一个被 springboot 容器管理的组件，那就是 DistroClientComponentRegistry 集群客户端组件注册器，这个注册器会创建 Distro 集群要用到的、并且是没有被 springboot 容器管理的组件，然后把组件设置到 DistroComponentHolder 组件持有器中。这个 DistroClientComponentRegistry 集群客户端组件注册器我也定义完毕了，接下来就展示给大家，请看下面代码块。

```
package com.cqfy.nacos.naming.consistency.ephemeral.distro.v2;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群组件注册器
 */
@Component
public class DistroClientComponentRegistry {

    //集群成员管理器
    private final ServerMemberManager serverMemberManager;

    //Distro集群协议器
    private final DistroProtocol distroProtocol;

    //Distro集群组件持有者
    private final DistroComponentHolder componentHolder;

    //Distro集群任务执行引擎持有者
    private final DistroTaskEngineHolder taskEngineHolder;

    //客户端管理器
    private final ClientManager clientManager;

    //集群代理客户端
    private final ClusterRpcClientProxy clusterRpcClientProxy;

    //构造方法
    public DistroClientComponentRegistry(ServerMemberManager serverMemberManager, DistroProtocol distroProtocol,
                                         DistroComponentHolder componentHolder, DistroTaskEngineHolder taskEngineHolder,
                                         ClientManagerDelegate clientManager, ClusterRpcClientProxy clusterRpcClientProxy) {
        this.serverMemberManager = serverMemberManager;
        this.distroProtocol = distroProtocol;
        this.componentHolder = componentHolder;
        this.taskEngineHolder = taskEngineHolder;
        this.clientManager = clientManager;
        this.clusterRpcClientProxy = clusterRpcClientProxy;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:该方法会在当前类的对象创建完毕后被回调
     */
    @PostConstruct
    public void doRegister() {
        //创建Distro集群客户端数据处理器
        DistroClientDataProcessor dataProcessor = new DistroClientDataProcessor(clientManager, distroProtocol);
        //创建Distro集群客户端数据传输代理器
        DistroTransportAgent transportAgent = new DistroClientTransportAgent(clusterRpcClientProxy, serverMemberManager);
        //创建Distro集群客户端任务失败处理器
        DistroClientTaskFailedHandler taskFailedHandler = new DistroClientTaskFailedHandler(taskEngineHolder);
        //以下就是把刚才创建好的组件注册给Distro集群组件持有者的操作
        componentHolder.registerDataStorage(DistroClientDataProcessor.TYPE, dataProcessor);
        componentHolder.registerDataProcessor(dataProcessor);
        componentHolder.registerTransportAgent(DistroClientDataProcessor.TYPE, transportAgent);
        componentHolder.registerFailedTaskHandler(DistroClientDataProcessor.TYPE, taskFailedHandler);
    }
} 
```

现在，网络数据传输器和客户端数据处理器的创建时机，大家就都清楚了吧？也就是说，**当 springboot 程序启动的时候，当 springboot 容器创建 DistroClientComponentRegistry 对象的时候，就会把 Distro 集群要用到的网络数据传输器和客户端数据处理器创建完毕，然后交给 DistroComponentHolder 组件管理使用。Distro 集群要使用某些组件的时候，只要可以获得 DistroComponentHolder 对象就能直接从该对象中得到自己想要使用的组件，而 DistroComponentHolder 对象也是被 springboot 容器管理的，所以这个对象也可以随时被其它对象或组件获得**。到此为止，之前遗留的所有问题基本上都解决完毕了，只剩下最后一个问题，**那就是DistroDelayTask 延迟任务处理器，也就是 DistroDelayTaskProcessor 类该怎么定义呢**？这个问题大家没有忘记吧？**现在我们已经清楚 Distro 集群的一个节点发起数据备份操作的时机了，也知道数据备份的操作是哪个组件执行的了，也知道延迟任务是怎么提交给延迟任务执行器的了，也知道了要备份数据的请求是怎么网络传输数据器被发送给目标节点的，也知道目标节点是怎么接收并处理要备份的数据的，现在就差怎么把要备份的数据交给网络传输数据器发送了，这一点大家可以理解吧**？所以，**我们接下来要实现的就是把要备份的数据交给 DistroClientTransportAgent 网络数据传输器发送的功能**。而这个功能不用我说大家也能猜到了，这肯定就是处理延时任务的处理器要承担的责任，所以接下来，就让我们实现它吧。

## 实现 DistroDelayTaskProcessor 延迟任务处理器

在具体实现这个延迟任务执行器之前，我还是要先帮助大家回顾一下，这个延迟任务执行器的创建时机，大家应该都清楚了，因为上一章我已经展示给大家了。就在上一章引入的 DistroTaskEngineHolder 任务执行器持有者中创建的，具体的内容请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro.task;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群的任务执行引擎持有者
 */
@Component
public class DistroTaskEngineHolder implements DisposableBean {

    //Distro集群延时任务执行引擎
    private final DistroDelayTaskExecuteEngine delayTaskExecuteEngine = new DistroDelayTaskExecuteEngine();

    //Distro集群非延时任务执行引擎，只要提交给该引擎的任务就会被立刻执行
    private final DistroExecuteTaskExecuteEngine executeWorkersManager = new DistroExecuteTaskExecuteEngine();

    public DistroTaskEngineHolder(DistroComponentHolder distroComponentHolder) {

        //DistroDelayTaskProcessor就是用来处理延迟任务的处理器
        //这里可以看到，处理延迟任务的处理器在这里被创建了
        DistroDelayTaskProcessor defaultDelayTaskProcessor = new DistroDelayTaskProcessor(this, distroComponentHolder);
        //在这里被交给延迟任务执行引擎使用。所以，上一小节最后提出的三个问题中的第二个也解决了
        //只剩下最后一个问题需要解决了，也就是把处理DistroDelayTask延迟任务的处理器定义出来
        delayTaskExecuteEngine.setDefaultTaskProcessor(defaultDelayTaskProcessor);
    }


    public DistroDelayTaskExecuteEngine getDelayTaskExecuteEngine() {
        return delayTaskExecuteEngine;
    }


    public DistroExecuteTaskExecuteEngine getExecuteWorkersManager() {
        return executeWorkersManager;
    }


    public void registerNacosTaskProcessor(Object key, NacosTaskProcessor nacosTaskProcessor) {
        this.delayTaskExecuteEngine.addProcessor(key, nacosTaskProcessor);
    }


    @Override
    public void destroy() throws Exception {
        this.delayTaskExecuteEngine.shutdown();
        this.executeWorkersManager.shutdown();
    }
}
```

在上面代码块中可以看到这个延迟任务处理器就是 DistroDelayTaskProcessor 对象，它的创建时机也非常清楚，接下来就该实现它了。**实现它的过程也没什么好分析的了，就是针对不同的操作创建不同的可以被立即执行任务，然后把任务提交给立即执行任务的执行引擎即可**。具体实现请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro.task.delay;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群延迟任务处理器
 */
public class DistroDelayTaskProcessor implements NacosTaskProcessor {

    //Distro集群执行引擎持有者
    private final DistroTaskEngineHolder distroTaskEngineHolder;

    //Distro集群各个组件持有者
    private final DistroComponentHolder distroComponentHolder;


    public DistroDelayTaskProcessor(DistroTaskEngineHolder distroTaskEngineHolder,
                                    DistroComponentHolder distroComponentHolder) {
        this.distroTaskEngineHolder = distroTaskEngineHolder;
        this.distroComponentHolder = distroComponentHolder;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:处理Distro集群延迟任务的方法
     */
    @Override
    public boolean process(NacosTask task) {
        //首先判断当前处理的任务是否为Distro集群延迟任务
        if (!(task instanceof DistroDelayTask)) {
            //如果不是则直接退出当前方法
            return true;
        }
        //把传入的任务转换为Distro延迟执行任务
        DistroDelayTask distroDelayTask = (DistroDelayTask) task;
        //得到任务的key
        DistroKey distroKey = distroDelayTask.getDistroKey();
        //得到延迟执行任务的操作符
        switch (distroDelayTask.getAction()) {
            //判断是否为删除操作
            case DELETE:
                //如果是删除操作则创建一个Distro集群同步删除操作的任务
                DistroSyncDeleteTask syncDeleteTask = new DistroSyncDeleteTask(distroKey, distroComponentHolder);
                //把任务提交给非延迟任务执行引擎，执行引擎会即可执行该任务
                distroTaskEngineHolder.getExecuteWorkersManager().addTask(distroKey, syncDeleteTask);
                return true;
            //判断是否为数据同步任务
            case CHANGE:
            case ADD:
                //如果是数据同步任务，则创建一个同步数据变更任务对象
                DistroSyncChangeTask syncChangeTask = new DistroSyncChangeTask(distroKey, distroComponentHolder);
                //把该任务提交给非延迟任务执行引擎
                distroTaskEngineHolder.getExecuteWorkersManager().addTask(distroKey, syncChangeTask);
                return true;
            //走到这里意味着操作未知，直接返回fals，表示处理任务失败
            default:
                return false;
        }
    }
}
```

可以看到，延迟任务处理器在处理延迟任务的时候，会根据任务的操作符创建不同的可以被立即执行的任务对象，**如果是客户端连接断开的操作，那就创建 DistroSyncDeleteTask 任务对象提交给立即执行任务的引擎去执行；如果是同步数据的操作，就创建 DistroSyncChangeTask 任务对象提交给立即执行任务的引擎去执行**。这些逻辑已经非常清楚了。所以接下来我们就要看看这些可以立即被执行的任务是怎么被定义的，在这些任务中，也许就有把要备份的数据交给网络传输数据器的操作。在文章中我就只展示 DistroSyncChangeTask 任务对象了，另外的任务对象和它的逻辑大同小异，就留给大家自己去我提供的第 11 版本代码中查看吧。DistroSyncChangeTask 类的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro.task.execute;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群同步数据变更任务，可以看到这个类继承了AbstractDistroExecuteTask抽象父类
 * 我把这个抽象父类的代码也展示在当前代码块中了
 */
public class DistroSyncChangeTask extends AbstractDistroExecuteTask {

    //当前任务的操作符
    private static final DataOperation OPERATION = DataOperation.CHANGE;

    //构造方法
    public DistroSyncChangeTask(DistroKey distroKey, DistroComponentHolder distroComponentHolder) {
        super(distroKey, distroComponentHolder);
    }


    @Override
    protected DataOperation getDataOperation() {
        return OPERATION;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:同步数据变更操作的方法，该方法会在父类中被调用，要从父类开始查看这个任务的执行逻辑
     */
    @Override
    protected boolean doExecute() {
        //得到要同步的数据的类型
        String type = getDistroKey().getResourceType();
        //根据数据类型得到要同步的客户端的数据
        DistroData distroData = getDistroData(type);
        //判断数据是否为空
        if (null == distroData) {
            Loggers.DISTRO.warn("[DISTRO] {} with null data to sync, skip", toString());
            //为空则直接退出当前方法
            return true;
        }
        //不为空则从Distro集群组件持有者中得到数据传输器，然后把数据同步到目标节点即可
        //getDistroKey().getTargetServer()这行代码就可以得到数据要同步的目标节点
        //在这里把要备份的数据交给网络数据传输器去发送了，逻辑终于衔接成功了！
        return getDistroComponentHolder().findTransportAgent(type).syncData(distroData, getDistroKey().getTargetServer());
    }


       /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:根据数据类型得到客户端数据的方法
     */
    private DistroData getDistroData(String type) {
        //根据数据类型从Distro集群组件持有者中得到了Distro集群客户端数据处理器
        //然后从Distro集群客户端数据处理器中得到了要同步的客户端的信息
        DistroData result = getDistroComponentHolder().findDataStorage(type).getDistroData(getDistroKey());
        //判断最终得到的DistroData是否不为空
        if (null != result) {
            //不为空则设置操作符
            result.setType(OPERATION);
        }
        //返回要同步给集群其他节点的客户端数据
        return result;
    }

    //其他内容暂时省略
}





//接下来是AbstractDistroExecuteTask类的内容


package com.cqfy.nacos.core.distributed.distro.task.execute;





/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro集群任务的抽象父类，在父类中定义了任务要执行的核心操作
 * 这个类的父类AbstractExecuteTask就实现了Runnable接口，可以直接被线程执行
 */
public abstract class AbstractDistroExecuteTask extends AbstractExecuteTask {

    //要处理的Distro集群数据的key，这个key封装了要同步的目标节点，以及要处理的数据的类型，是一个非常关键的对象
    private final DistroKey distroKey;

    //Distro集群组件持有者
    private final DistroComponentHolder distroComponentHolder;

    //构造方法
    protected AbstractDistroExecuteTask(DistroKey distroKey, DistroComponentHolder distroComponentHolder) {
        this.distroKey = distroKey;
        this.distroComponentHolder = distroComponentHolder;
    }

    protected DistroKey getDistroKey() {
        return distroKey;
    }


    protected DistroComponentHolder getDistroComponentHolder() {
        return distroComponentHolder;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:Distro集群向其他节点同步数据任务要执行的操作
     */
    @Override
    public void run() {
        //得到要同步的数据的类型
        String type = getDistroKey().getResourceType();
        //根据数据类型得到Distro集群客户端数据传输器
        DistroTransportAgent transportAgent = distroComponentHolder.findTransportAgent(type);
        //如果数据传输器为null则记录告警日志，并且直接退出当前方法
        if (null == transportAgent) {
            Loggers.DISTRO.warn("No found transport agent for type [{}]", type);
            return;
        }
        //记录操作开始执行日志
        Loggers.DISTRO.info("[DISTRO-START] {}", toString());
        //判断Distro集群客户端数据传输器是否支持回调任务的通信方式
        if (transportAgent.supportCallbackTransport()) {
            //如果支持就使用可以定义回调任务的方式向其他节点传输数据
            //该方法就不再展示了
            doExecuteWithCallback(new DistroExecuteCallback());
        } else {
            //如果不支持则使用普通方式向集群其他节点传输数据
            executeDistroTask();
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:使用普通方式向集群其他节点传输数据的方法
     */
    private void executeDistroTask() {
        try {
            //执行同步操作，这时候就会执行到子类中
            boolean result = doExecute();
            //判断操作是否执行成功
            if (!result) {
                //这里就是操作执行失败了，失败则执行任务重试操作
                handleFailedTask();
            }
            //记录操作结束日志
            Loggers.DISTRO.info("[DISTRO-END] {} result: {}", toString(), result);
        } catch (Exception e) {
            Loggers.DISTRO.warn("[DISTRO] Sync data change failed.", e);
            //有异常则执行失败重试操作
            handleFailedTask();
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:同步操作失败后要执行的方法
     */
    protected void handleFailedTask() {
        //得到处理的数据的类型
        String type = getDistroKey().getResourceType();
        //根据数据类型得到Distro集群任务失败重试处理器
        DistroFailedTaskHandler failedTaskHandler = distroComponentHolder.findFailedTaskHandler(type);
        //找不到处理器则记录告警日志，并退出当前方法
        if (null == failedTaskHandler) {
            Loggers.DISTRO.warn("[DISTRO] Can't find failed task for type {}, so discarded", type);
            return;
        }
        //找到了则执行失败重试操作
        failedTaskHandler.retry(getDistroKey(), getDataOperation());
    }

    //其他内容暂时省略
    
}
```

到此为止，我就为大家把 Distro 集群备份数据的全功能都实现完毕了，说实话，内容非常多，写到这里我也已经心力交瘁了，所以越到后面功能实现进展得就越快，有些内容根本来不及展开，**比如验证每个节点备份的数据是否一致的操作，新加入集群的节点从某个节点下载快照的操作，向集群中的其他节点查询某些数据的操作等等，这些操作的执行流程和数据备份的流程几乎一样，所以就都留到我提供的第十一版本代码中**，给大家自己查看吧。现在第十一版本代码的内容大家都可以查看了。大家可以认真读几遍文章，自己画画流程图，把这些知识都消化了。

当然，有些内容来不及展开，但并不意味着一点都不讲解，**就比如说验证每个节点备份的数据是否一致的功能，新加入集群的节点从某个节点下载快照的功能**，这两个功能其实非常有必要详细讲解，在文章中一点点地剖析它们的实现思路，然后实现它们，因为它们是很重要的功能**。Distro 集群备份了数据之后，肯定需要经常验证每一个节点存放的数据是否一致，如果不一致就要执行某些操作，使数据一致，所以集群启动的时候还要启动一个定期验证节点数据是否一致的定时任务；还有一个新的节点添加到 Distro 集群中，肯定要以下载快照的方式使自己尽快和集群其他节点保存的数据同步了，那是不是得启动一个远程加载快照的任务呢**？这些功能我都没有在文章中为大家讲解，这是因为现在的篇幅实在是太长了，写不下了，所以就省略了。但这些功能的实现思路非常简单，都是封装成任务，让执行器去执行。并且这些操作就定义在了 DistroProtocol 组件中，我把对应的代码搬运到这里了，请看大家看一看，请看下面代码块。

```
package com.cqfy.nacos.core.distributed.distro;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/25
 * @Description:Distro协议器，这个组件就是用来处理Distro集群中请求的
 */
@Component
public class DistroProtocol {


    //集群成员管理器
    private final ServerMemberManager memberManager;

    //Distro集群的任务执行引擎持有者，这个组件大家都还有印象吧？
    //上一章已经为大家展示过了，这个组件持有了Distro集群要用到的延迟任务执行引擎和非延迟任务执行引擎
    private final DistroTaskEngineHolder distroTaskEngineHolder;


    //构造方法
    public DistroProtocol(ServerMemberManager memberManager, DistroTaskEngineHolder distroTaskEngineHolder) {
        this.memberManager = memberManager;
        this.distroTaskEngineHolder = distroTaskEngineHolder;
        //这里有一个非常重要的操作，那就是启动Distro集群中的任务
        startDistroTask();
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:启动Distro集群任务
     */
    private void startDistroTask() {
        //首先判断当前服务器是以什么模式启动的，如果是单机模式启动，那就直接退出当前方法，什么集群操作也不执行
        if (EnvUtil.getStandaloneMode()) {
            //单机模式启动，也就意味着集群中只有一个节点，节点已经启动了，所以旧标记Distro集群初始化完毕
            isInitialized = true;
            //退出当前方法
            return;
        }
        //走到这里意味着是以集群模式启动的
        //这时候就先启动Distro集群下的验证集群各个节点数据是否同步的定时任务
        startVerifyTask();
        //启动加载任务，所谓加载任务，就是在集群启动的时候，当前节点主动去集群的其他节点中把其他节点管理的数据快照远程加载到自己内部
        //如果一个节点刚刚加入到集群中的话，这个方法将会帮助该节点尽快同步其他节点的数据
        startLoadTask();
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:启动Distro集群下的验证集群各个节点数据是否同步的定时任务，如果发现有节点的数据和当前节点并不同步，那么当前节点就会把自己
     * 最新的客户端信息同步给目标节点
     */
    private void startVerifyTask() {
        //提交定时任务给执行期，这个任务并不是立刻就执行的，而是在程序启动5秒之后才执行
        GlobalExecutor.schedulePartitionDataTimedSync(new DistroVerifyTimedTask(memberManager, distroComponentHolder, distroTaskEngineHolder.getExecuteWorkersManager()),
                DistroConfig.getInstance().getVerifyIntervalMillis());
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/25
     * @Description:启动Distro集群加载任务
     */
    private void startLoadTask() {
        //定义加载任务的回调方法
        DistroCallback loadCallback = new DistroCallback() {
            //加载成功后回调该方法
            @Override
            public void onSuccess() {
                //在这里把Distro集群启动初始化标志设置为true
                isInitialized = true;
            }

            //加载操作失败后该方法被回调
            @Override
            public void onFailed(Throwable throwable) {
                isInitialized = false;
            }
        };
        //在这里把加载任务提交给全剧执行器执行
        GlobalExecutor.submitLoadDataTask(new DistroLoadDataTask(memberManager, distroComponentHolder, DistroConfig.getInstance(), loadCallback));
    }


    //其他内容暂时省略
    

}
```

执行集群节点数据验证和下载快照的任务我就不在文章中展示了，大家直接去看我提供的第十一版本代码即可。好了朋友们，Distro 集群的内容到此就全部结束了，下一章就要构建 jraft 集群了，jraft 集群的内容更简单，很快就能实现完毕。朋友们，我们下一章见！

附：其实文章中有非常多的细节都省略了，如果全都铺展开讲解实现，那么十章内容也讲不完。所以大家一定要认真看文章，然后再去看我提供的代码，代码中注释非常详细，大家只要把文章掌握了，那么所有代码都能看明白。至于省略的知识，我就给大家简单列举一下，这样大家也能有目的地去看我提供的代码，比如说 Distro 集群成员变更了，添加了新的节点之后，要怎么做呢？如果大家仔细阅读 Member 工具类中的代码，那大家就会清楚集群成员变更之后，本地集群配置文件中的内容也会更新。还有一点，nacos 把 Distro 集群定义为 ap 模式的集群，大家可以仔细阅读 Distro 集群备份数据的代码，然后思考一下为什么这个 Distro 集群被定义为了 ap 模式的集群？好了，多余的话我就不说了，大家自己去代码中寻找答案吧，源码之中，没有秘密可言。