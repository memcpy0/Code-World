在上一章最后一小节，我为大家实现了 nacos 服务端的 HealthCheckRequestHandler 健康检测请求处理器，这个处理器专门处理 nacos 客户端发送过来的健康检测请求，并且我也跟大家解释了我实现这个 HealthCheckRequestHandler 健康检测请求处理器的原因。这个处理器实现得非常简单，就是直接给 nacos 客户端回复一个成功响应即可，这没什么可说的，重点在于 nacos 客户端每一次向服务端发送 HealthCheckRequest 健康检测请求，这个单向流请求总会被 nacos 服务端的单向流请求接收器接收并处理，在处理的过程中会刷新该客户端连接对象在服务端的最新活跃时间，具体代码如下，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/16
 * @方法描述：单向流请求接收器，这个接收器专门处理客户端发送过来的单向流请求
 * 可以看到，这个GrpcRequestAcceptor类上添加了springboot的@Service注解，这就意味着这个请求接收器也是被springboot容器管理的对象
 */
@Service
public class GrpcRequestAcceptor extends RequestGrpc.RequestImplBase {


    //请求处理器注册表，这个注册表对象内部持有着服务端所有的请求处理器
    @Autowired
    RequestHandlerRegistry requestHandlerRegistry;


    //连接管理器
    @Autowired
    private ConnectionManager connectionManager;


    

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：本类最核心的方法，服务端接收并处理客户端发送过来的单向流请求的方法
     */
    @Override
    public void request(Payload grpcRequest, StreamObserver<Payload> responseObserver) {



        //省略该方法其他内容
        
        //走到这里意味着解析到的对象就是请求类型的，这里就直接把对象转换为请求
        Request request = (Request) parseObj;
        try {
            //从请求管理器中获得发送请求的客户端的连接对象
            Connection connection = connectionManager.getConnection(GrpcServerConstants.CONTEXT_KEY_CONN_ID.get());
            //构建客户端请求的元数据对象
            RequestMeta requestMeta = new RequestMeta();
            //设置客户端IP
            requestMeta.setClientIp(connection.getMetaInfo().getClientIp());
            //设置客户端Id
            requestMeta.setConnectionId(GrpcServerConstants.CONTEXT_KEY_CONN_ID.get());
            //设置客户端版本号
            requestMeta.setClientVersion(connection.getMetaInfo().getVersion());
            //设置客户端标签
            requestMeta.setLabels(connection.getMetaInfo().getLabels());
            //设置客户端能力表
            requestMeta.setAbilityTable(connection.getAbilityTable());
            
            //在这里刷新客户端连接的最新活跃时间，这里就和我之前为大家讲解的客户端的知识串联起来了
            //我当初在讲解客户端的时候跟大家说客户端每次向服务端发送健康检查请求，都会刷新服务端创建的客户端连接对象的最新活跃时间
            //这个流程就体现在这里，健康检查请求是单向流请求，就会被当前接收器对象接收并处理，在具体处理之前就会先刷新客户端链接对象的最新活跃时间
            connectionManager.refreshActiveTime(requestMeta.getConnectionId());
            
            //在这里处理客户端请求，并且获得响应
            //如果是注册服务实例的请求，就会在这里找到InstanceRequestHandler处理器，执行服务实力注册的操作
            //如果是健康检查请求，那这里就会获得HealthCheckRequestHandler处理器，执行健康检查的操作
            Response response = requestHandler.handleRequest(request, requestMeta);


            //下面就是回复客户端相应的操作，首先把响应转换为用于网络传输的Payload对象
            Payload payloadResponse = GrpcUtils.convert(response);
            //判断是否需要追踪响应
            traceIfNecessary(payloadResponse, false);
            //这里再判断一下，如果响应码表示超过server端的限流阈值，那现在就不能立刻回复客户端响应
            if (response.getErrorCode() == NacosException.OVER_THRESHOLD) {
                //而是提交一个定时任务，稍后回复客户端响应
                //这里的这段代码，大家可能会觉得很困惑，怎么突然和限流又有关系了，实际上在nacos中还定义了一个TpsControlRequestFilter过滤器，并且
                //在每一个请求处理器的方法上都添加了@TpsControl注解，在服务端处理客户端单向流请求之前，TpsControlRequestFilter过滤器会首先处理客户端请求
                //看看服务器的tps是否达到阈值了，如果达到阈值了，就不能处理本次请求，而是直接回复客户端服务端限流的响应
                //这个过滤器的体系我在第七版本还没有为大家实现，因为要引入更多的组件，这个过滤器体系我会在后面版本为大家逐渐完善
                RpcScheduledExecutor.CONTROL_SCHEDULER.schedule(() -> {
                    traceIfNecessary(payloadResponse, false);
                    responseObserver.onNext(payloadResponse);
                    responseObserver.onCompleted();
                }, 1000L, TimeUnit.MILLISECONDS);
            } else {
                //没有限流则立刻回复客户端响应
                traceIfNecessary(payloadResponse, false);
                responseObserver.onNext(payloadResponse);
                responseObserver.onCompleted();
            }
        } 

        //省略该方法其他内容
    }


    //省略该类其他内容
}
```

可以看到，**在上面代码块的第 62 行，执行了 ConnectionManager 连接管理器的 refreshActiveTime() 方法，该方法就会刷新当前发送请求的客户端在服务端的客户端连接对象的最新活跃时间**。只要客户端在 nacos 服务端对应的客户端连接对象的最新活跃定期被刷新，那么客户端和服务端的连接就会一直处于健康状态，客户端和服务端也就可以正常通信。但是，我们目前实现的 ConnectionManager 连接管理器根本就没有实现对应的 refreshActiveTime() 方法，请看下面代码块。

```
package com.cqfy.nacos.core.remote;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：客户端连接管理器，这个类的对象是服务端非常重要的一个组件，这个类的对象也要交给springboot容器来管理，在第七版本代码中，这个连接管理器只存放来自SDK客户端的连接对象
 */
@Service
public class ConnectionManager {

    private static final Logger LOGGER = Loggers.CONNECTION;

    //这个map的作用就很直接了，key就是客户端连接ID，value就是建立的连接对象，服务端把接收到的客户端连接存放到连接管理器中，就是存放到这个map中
    Map<String, Connection> connections = new ConcurrentHashMap<>();




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：注册客户端连接的方法
     */
    public synchronized boolean register(String connectionId, Connection connection) {
        //判断客户端与服务端的连接是否真的建立了
        if (connection.isConnected()) {
            
            //在这里把客户端连接Id和服务端为其创建的连接对象添加到映射表中
            connections.put(connectionId, connection);
            
            //记录操作日志
            LOGGER.info("new connection registered successfully, connectionId = {},connection={} ", connectionId, connection);
            
            return true;
        }
        return false;

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：得到指定客户端ID的客户端连接对象
     */
    public Connection getConnection(String connectionId) {
        
        return connections.get(connectionId);
    }

    

    //其他内容暂时不做实现
}
```

从上面的代码块中可以看到，我们目前实现的 ConnectionManager 连接管理器确实没有 refreshActiveTime() 方法。因此，肯定要对 ConnectionManager 连接管理器进行重构。上一章结尾我就为大家讲解到这里了，并没有真正重构 ConnectionManager 连接管理器，这一章我们就先来重构 ConnectionManager 连接管理器，实现客户端连接对象最新活跃时间刷新功能。

## 重构 ConnectionManager，实现客户端连接最新活跃时间刷新功能

我知道在开始重构 ConnectionManager 连接管理器之前，大家肯定还是有些困惑，不知道所谓的客户端连接对象的最新活跃时间是怎么来的，因为我当时为大家引入客户端连接对象，也就是 GrpcConnection 对象时，没有把这个对象给大家展示全面。我们最终定义完毕的 GrpcConnection 客户端连接对象如下所示，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：服务端使用的请求对象，服务端会为接收到的每一个客户端连接创建一个连接对象，这个连接对象就会用来向客户端发送消息
 * 注意，这个服务端的连接管理器实现了Connection接口
 */
public class GrpcConnection extends Connection {

    //流双向流通信对象
    private StreamObserver streamObserver;

    //netty中的channel，其实就是netty中服务端为这个客户端创建的SocketChannel通道
    private Channel channel;

    public GrpcConnection(ConnectionMeta metaInfo, StreamObserver streamObserver, Channel channel) {
        super(metaInfo);
        this.streamObserver = streamObserver;
        this.channel = channel;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：发送无需等待应答的请求给客户端
     */
    public void sendRequestNoAck(Request request) throws NacosException {
        try {
            synchronized (streamObserver) {
                Payload payload = GrpcUtils.convert(request);
                //在这里把消息发送给了客户端
                streamObserver.onNext(payload);
            }
        } catch (Exception e) {
            if (e instanceof StatusRuntimeException) {
                throw new ConnectionAlreadyClosedException(e);
            }
            throw e;
        }
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：关闭客户端和服务端连接的方法
     */
    @Override
    public void close() {
        String connectionId = null;

        try {
            connectionId = getMetaInfo().getConnectionId();

            if (isTraced()) {
                Loggers.REMOTE_DIGEST.warn("[{}] try to close connection ", connectionId);
            }

            closeBiStream();
            channel.close();

        } catch (Exception e) {
            Loggers.REMOTE_DIGEST.warn("[{}] connection  close exception  : {}", connectionId, e);
        }
    }

    private void closeBiStream() {
        if (streamObserver instanceof ServerCallStreamObserver) {
            ServerCallStreamObserver serverCallStreamObserver = ((ServerCallStreamObserver) streamObserver);
            if (!serverCallStreamObserver.isCancelled()) {
                serverCallStreamObserver.onCompleted();
            }
        }
    }

    //判断channel是否处于活跃状态，其实就是判断客户端与服务端的连接是否真的建立了，这是netty中的方法
    @Override
    public boolean isConnected() {
        return channel != null && channel.isOpen() && channel.isActive();
    }


    //其他内容暂时不做实现

}
```

从上面代码块中可以看到，**创建一个 GrpcConnection 客户端连接对象，需要向其有参构造方法中传递 3 个参数，分别是 ConnectionMeta 连接元信息对象，StreamObserver 流式通信对象，以及 netty 为客户端连接创建的 SocketChannel**。这三个参数都是 GrpcConnection 连接对象内部的成员变量，当然，这几个成员变量有的定义在了 GrpcConnection 类中，有的定义在了 GrpcConnection 的父类 Connection 中，这都没关系，反正我们最终可以通过对应的 get 方法得到 GrpcConnection 对象的各个成员变量。当然，我们最终的目的并不是为了得到这几个成员变量中的某一个，而是为了刷新 GrpcConnection 连接对象的最新活跃时间，那么这个最新活跃时间定义在哪了呢？这时候我就可以对大家明说了，实际上，GrpcConnection 连接对象的最新活跃时间就定义在了 ConnectionMeta 连接元信息对象内部。而这个 ConnectionMeta 连接元信息对象我之前一直没有为大家展示，接下来就为大家展示一下这个 ConnectionMeta 的部分内容，请看下面代码块。

```
package com.cqfy.nacos.core.remote;


public class ConnectionMeta {

    /**
     * ConnectionType.
     */
    String connectType;

    /**
     * Client IP Address.
     */
    String clientIp;

    /**
     * Remote IP Address.
     */
    String remoteIp;

    /**
     * Remote IP Port.
     */
    int remotePort;

    /**
     * Local Ip Port.
     */
    int localPort;

    /**
     * Client version.
     */
    String version;

    /**
     * Identify Unique connectionId.
     */
    String connectionId;

    /**
     * create time.
     */
    Date createTime;

    //客户端连接最新的活跃时间
    long lastActiveTime;

    /**
     * String appName.
     */
    String appName;

    /**
     * tenant.
     */
    String tenant;


    //得到客户端连接最新活跃时间的方法
    public long getLastActiveTime() {
        return lastActiveTime;
    }

    //设置客户端连接最新活跃时间的方法
    public void setLastActiveTime(long lastActiveTime) {
        this.lastActiveTime = lastActiveTime;
    }

    
    //下面是一堆get、set方法，这些方法就省略了
}
```

从上面的代码块可以看出，设置客户端连接对象最新活跃时间的方法已经定义完毕了，**就是 setLastActiveTime() 方法，只要这个方法一被调用，那么客户端在服务端的客户端连接对象的最新活跃时间就会被刷新**， 那么该方法会在哪里被调用呢？这个我也按照 nacos 框架源码定义好了，该方法就在 GrpcConnection 的父类，也就是 Connection 抽象类中被调用了，具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.core.remote;



@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class Connection implements Requester {

    //这个就是当前客户端连接是否需要被追踪的标志，默认不追踪
    private boolean traced = false;

    private Map<String, Boolean> abilityTable;

    //这个就是连接元信息对象
    private final ConnectionMeta metaInfo;

    public Connection(ConnectionMeta metaInfo) {
        this.metaInfo = metaInfo;
    }

    public Map<String, String> getLabels() {
        return metaInfo.getLabels();
    }

    public boolean isTraced() {
        return traced;
    }

    public void setTraced(boolean traced) {
        this.traced = traced;
    }

    public void setAbilityTable(Map<String, Boolean> abilityTable) {
        this.abilityTable = abilityTable;
    }

    public Map<String, Boolean> getAbilityTable() {
        return this.abilityTable;
    }

    
    public abstract boolean isConnected();

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：更新客户端连接最新的活跃时间
     */
    public void freshActiveTime() {
        metaInfo.setLastActiveTime(System.currentTimeMillis());
    }

   
    public ConnectionMeta getMetaInfo() {
        return metaInfo;
    }

    @Override
    public String toString() {
        return "Connection{" + "traced=" + traced + ", abilities=" + abilityTable + ", metaInfo=" + metaInfo + '}';
    }
}
```

从上面代码块中可以看到，**在 freshActiveTime() 方法中，调用了客户端连接对象 ConnectionMeta 元信息对象的 setLastActiveTime() 方法，并且使用系统当前时间给客户端连接对象的最新活跃时间赋值了**。这时候大家肯定会追问，那 Connection 类的 freshActiveTime() 方法什么时候被调用呢？这个几乎不用怎么思考就能回答了，肯定是在 ConnectionManager 连接管理器中被调用啊，我们一开始的目的不就是想重构 ConnectionManager 连接管理器，为它定义一个 refreshActiveTime() 方法吗？**现在实现思路也明确了，就是在连接管理器中根据客户端连接 Id 得到对应的客户端连接对象，然后直接调用连接对象的 freshActiveTime() 方法即可**。好了，接下来就请大家看看我重构之后的 ConnectionManager 连接管理器，请看下面代码块。

```
package com.cqfy.nacos.core.remote;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：客户端连接管理器，这个类的对象是服务端非常重要的一个组件，这个类的对象也要交给springboot容器来管理，在第七版本代码中，这个连接管理器只存放来自SDK客户端的连接对象
 */
@Service
public class ConnectionManager {

    private static final Logger LOGGER = Loggers.CONNECTION;

    //这个map的作用就很直接了，key就是客户端连接ID，value就是建立的连接对象，服务端把接收到的客户端连接存放到连接管理器中，就是存放到这个map中
    Map<String, Connection> connections = new ConcurrentHashMap<>();




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：注册客户端连接的方法
     */
    public synchronized boolean register(String connectionId, Connection connection) {
        //判断客户端与服务端的连接是否真的建立了
        if (connection.isConnected()) {
            
            //在这里把客户端连接Id和服务端为其创建的连接对象添加到映射表中
            connections.put(connectionId, connection);
            
            //记录操作日志
            LOGGER.info("new connection registered successfully, connectionId = {},connection={} ", connectionId, connection);
            
            return true;
        }
        return false;

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：得到指定客户端ID的客户端连接对象
     */
    public Connection getConnection(String connectionId) {
        
        return connections.get(connectionId);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：刷新指定客户端连接的最新活跃时间
     */
    public void refreshActiveTime(String connectionId) {
        //根据客户端ID得到对应的客户端连接对象，也就是GrpcConnection对象
        Connection connection = connections.get(connectionId);
        //判断连接对象是否存在
        if (connection != null) {
            //如果存在则刷新连接对象的最新活跃时间，这里刷新活跃时间的时候，就会调用连接元数据对象的setLastActiveTime方法，给最新活跃时间赋值
            connection.freshActiveTime();
        }
    }

    

    //其他内容暂时不做实现
}
```

到此为止，我就把服务端刷新客户端连接最新活跃时间的功能实现完毕了。可以看到，这个功能还是非常简单的，甚至是非常不起眼的一个小功能，但我想说的是，**这个功能对服务端连接判活功能的实现至关重要**。那什么是服务端连接判活呢？这个解释起来也非常简单，之前我们在 nacos 客户端实现了健康检测功能，**所谓客户端健康检测就是 nacos 客户端定期向 nacos 服务端发送 HealthCheckRequest 健康检测请求，而该请求会被 nacos 服务端的单向流请求接收器接收，紧接着就会执行刷新客户端连接最新活跃时间的操作，这个是 nacos 客户端主动向服务端检测连接是否健康并刷新活跃时间的功能**；既然有 nacos 客户端主动向服务端检测连接是否健康，肯定就会有 nacos 服务端主动向客户端检测连接是否健康，那 nacos 服务端怎么主动向客户端检测连接是否健康呢？这个时候，服务端每一个客户端创建客户端连接对象的最新活跃时间就该派上用场了。

## 引入 NacosRuntimeConnectionEjector，实现服务端对连接主动探活功能

请大家想一想，假如 nacos 服务端想对客户端连接主动探活，是不是只需要判断客户端连接对象的最新活跃时间是否更新及时即可？如果客户端和服务端连接一直健康，那么 nacos 客户端肯定会定期成功发送健康检测请求，也就意味着会定期刷新服务端的客户端连接对象的最新活跃时间。**假如 nacos 服务端定义了一个连接过期时间，比如说这个时间定义为了 2 秒，nacos 客户端会定期发送健康检测请求刷新客户端连接的最新活跃时间，nacos 服务端则可以定义一个定时任务，定时任务定期检查系统当前时间和客户端连接的最新活跃时间的差值是否超过了 2 秒，如果超过 2 秒，就代表客户端连接对象的最新活跃时间过期了，可能就意味着客户端和服务端连接出现问题了**。我想这个逻辑应该非常简单吧？在过去完结的框架课程中，已经见过非常多的健康检查的方式了，这种方式应该是非常普通的一种。

如果上面的逻辑大家都理解了，那么接下来我们要做的事就非常简单了，比**如我们就可以定义一个连接驱逐器，也就是本小节标题中展示的 NacosRuntimeConnectionEjector 类**。在这个 NacosRuntimeConnectionEjector 连接驱逐器中定义一个判断连接是否过期的方法，然后创建定时任务，定期执行该方法即可。**比如说我们就把判断连接是否过期的方法定义为 ejectOutdatedConnection() 方法，在该方法中首先从 ConnectionManager 连接管理器中得到服务端创建的所有客户端连接对象，然后遍历这些客户端连接对象，依次判断每一个客户端连接对象的最新活跃时间是否过期**。这个思路应该没什么问题吧，并且根据刚才分析的思路，我们还可以延伸出两个要点：

1 NacosRuntimeConnectionEjector 连接驱逐器的 ejectOutdatedConnection() 方法肯定需要用到 ConnectionManager 连接管理器对象，原因很简单，因为在 ejectOutdatedConnection() 方法中需要获得服务端创建的所有客户端连接对象，而所有的客户端连接对象只能从 ConnectionManager 连接管理器对象中获得。所以，我们可以把 ConnectionManager 连接管理器对象定义为 ejectOutdatedConnection() 方法的参数，或者直接就把连接管理器定义为 NacosRuntimeConnectionEjector 连接驱逐器的一个成员变量。在 nacos 源码中，直接把连接管理器定义为 NacosRuntimeConnectionEjector 连接驱逐器的成员变量了，所以我也会采用这种方式实现 NacosRuntimeConnectionEjector 连接管理器。

2 还有一点需要考虑的就是，假如在 NacosRuntimeConnectionEjector 连接驱逐器的 ejectOutdatedConnection() 方法中发现有些客户端连接对象的最新活跃时间过期了，这可能就意味着对应的 nacos 客户端没能定期发送健康检测请求，没能定期刷新服务端最新的活跃时间，这也就意味着客户端和服务端之间的连接可能出现问题了。那这个时候 nacos 服务端需要做什么呢？直接注销出现问题的客户端连接吗？显然不能这么做，也许只是网络出现波动了，造成客户端连接最新活跃时间没能及时刷新，所以这个时候 nacos 服务端要做的就是使用对应的客户端连接对象主动向 nacos 客户端发送一个探活请求，如果可以接收到该请求的响应，这就意味着客户端和服务端的连接是没有问题的。如果接受不到客户端回复的响应，这时候就意味着客户端和服务端连接确实出问题了，那么 nacos 服务端的连接驱逐器就可以驱逐该客户端连接对象了。

好了，经过上面的分析之后，现在我们可以着手从代码层面实现这个 NacosRuntimeConnectionEjector 连接驱逐器，以及其中定义的 ejectOutdatedConnection() 方法，请看下面代码块。

```
package com.cqfy.nacos.core.remote;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/16
 * @方法描述：运行时连接拒绝器
 */
public class NacosRuntimeConnectionEjector extends RuntimeConnectionEjector {

    public NacosRuntimeConnectionEjector() {

    }


    //ConnectionManager连接管理器成员变量，这里我要多强调一句
    //连接管理器和下面的get、set方法，实际上都定义在了当前类的父类RuntimeConnectionEjector中
    //但这里我为了简化代码，所以就把这个成员变量和对应的set、get方法定义在这里了
    //本身RuntimeConnectionEjector类中也没什么特别重要的信息
    protected ConnectionManager connectionManager;

    public RuntimeConnectionEjector() {
    }

    public ConnectionManager getConnectionManager() {
        return connectionManager;
    }

    public void setConnectionManager(ConnectionManager connectionManager) {
        this.connectionManager = connectionManager;
    }

    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：这个方法会在ConnectionManager连接管理器启动的时候被调用
     */ 
    public void doEject() {
        //驱逐过期的客户端连接
        ejectOutdatedConnection();
    }



    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/17
     * @方法描述：驱逐过期客户端连接的方法
     */ 
    private void ejectOutdatedConnection() {
        try {
            
            //记录开始操作的日志
            Loggers.CONNECTION.info("Connection check task start");

            
            //以下是从连接管理器中得到所有客户端连接对象的操作
            
            //首先从连接管理器中获得服务端接收到的所有客户端连接
            Map<String, Connection> connections = connectionManager.connections;
            //得到接收到的连接总数，这里其实就是看看创建了多少个GrpcConnection对象，因为服务端每接收到一个客户端连接，就会为该连接创建一个GrpcConnection连接对象
            int totalCount = connections.size();
            //得到来自SDK客户端的连接数量
            int currentSdkClientCount = connectionManager.currentSdkClientCount();
            //记录日志信息
            Loggers.CONNECTION.info("Long connection metrics detail ,Total count ={}, sdkCount={},clusterCount={}", totalCount, currentSdkClientCount, (totalCount - currentSdkClientCount));

            //这里创建的这个集合是用来存放过期的客户端Id的
            Set<String> outDatedConnections = new HashSet<>();
            //得到当前时间
            long now = System.currentTimeMillis();
            
            
            //以下是遍历所有客户端连接对象，判断是否存在连接超时的客户端连接
            //所谓连接超时就是客户端连接对象的最新活跃时间没有被刷新
            
            
            //遍历所有客户端连接
            for (Map.Entry<String, Connection> entry : connections.entrySet()) {
                //获得客户端连接对象，也就是GrpcConnection对象
                Connection client = entry.getValue();
                //用当前时间减去该客户端连接的最新活跃时间，看看这个时间差是否超过了服务端规定的连接过期时间
                if (now - client.getMetaInfo().getLastActiveTime() >= KEEP_ALIVE_TIME) {
                    //如果连接已经过期，那就把这个客户端连接的Id添加到刚才创建的set集合中
                    outDatedConnections.add(client.getMetaInfo().getConnectionId());
                }
            }
            //记录过期连接数量到日志中
            Loggers.CONNECTION.info("Out dated connection ,size={}", outDatedConnections.size());



            //以下就是判断是否存在过期的客户端连接，如果存在，nacos服务端就要向客户端发送探活请求，判断连接是否真的出现问题了

            
            //判断是否存在过期的客户端连接
            if (CollectionUtils.isNotEmpty(outDatedConnections)) {
                //如果存在过期连接，服务端还不能直接驱逐这些过期连接，在真正驱逐之前，还要再检验一下连接是否真的出现问题了
                //所以服务端会主动向客户端发送探活请求，如果收到成功响应，就意味着连接没有问题，就不能驱逐
                Set<String> successConnections = new HashSet<>();
                //这里使用了CountDownLatch对象，主要是为了等待服务端对所有过期连接的检查执行完毕
                final CountDownLatch latch = new CountDownLatch(outDatedConnections.size());
                //遍历过期的客户端连接Id
                for (String outDateConnectionId : outDatedConnections) {
                    try {
                        //根据客户端连接Id得到对应的客户端连接对象
                        Connection connection = connectionManager.getConnection(outDateConnectionId);
                        //如果连接对象不为空
                        if (connection != null) {
                            //接下来就创建一个客户端探活请求对象
                            ClientDetectionRequest clientDetectionRequest = new ClientDetectionRequest();
                            //在这里异步发送请求给客户端，异步发送请求的时候还定义了一个回调方法
                            connection.asyncRequest(clientDetectionRequest, new RequestCallBack() {
                                @Override
                                public Executor getExecutor() {
                                    //得到执行回调方法的执行器，这里返回null，肯定就是直接使用调用回调方法的线程直接执行方法了
                                    return null;
                                }

                                @Override
                                public long getTimeout() {
                                    //得到发送请求的超时时间
                                    return 5000L;
                                }

                                
                                //这个就是真正的回调方法，在收到客户端回复的响应后，该方法就会被回调
                                @Override
                                public void onResponse(Response response) {
                                    //接收到响应后，CountDownLatch计数器就减一
                                    latch.countDown();
                                    //判断响应是否成功，如果响应成功
                                    if (response != null && response.isSuccess()) {
                                        //就在这里刷新连接的最新活跃时间
                                        connection.freshActiveTime();
                                        //然后把这个正常的连接Id添加到successConnections这个集合中
                                        successConnections.add(outDateConnectionId);
                                    }
                                }

                                @Override
                                public void onException(Throwable e) {
                                    //出现异常情况，计数器也减一
                                    latch.countDown();
                                }
                            });
                            //记录操作日志
                            Loggers.CONNECTION.info("[{}]send connection active request ", outDateConnectionId);
                        } else {
                            //如果程序走到这里，说明从连接管理器中获得的连接对象为空，这就意味着服务端已经注销了客户端连接对象，客户端和服务端的连接肯定断开了
                            //直接把计数器减一即可
                            latch.countDown();
                        }
                    } catch (ConnectionAlreadyClosedException e) {
                        //有异常也直接把计数器减一，这个异常是连接已经关闭异常
                        latch.countDown();
                    } catch (Exception e) {
                        Loggers.CONNECTION.error("[{}]Error occurs when check client active detection ,error={}", outDateConnectionId, e);
                        latch.countDown();
                    }
                }
                //这里计数器对象会限时等待，直到所有连接检查完毕或者超时
                latch.await(5000L, TimeUnit.MILLISECONDS);
                //记录经过检测后发现仍然正常工作的连接数量
                Loggers.CONNECTION.info("Out dated connection check successCount={}", successConnections.size());
                //接下来就该对比outDateConnectionId集合和successConnections集合存放的数据了
                for (String outDateConnectionId : outDatedConnections) {
                    //如果successConnections集合中没有遍历到的连接Id，就意味着这个客户端连接是真的过期了
                    if (!successConnections.contains(outDateConnectionId)) {
                        //记录操作日志
                        Loggers.CONNECTION.info("[{}]Unregister Out dated connection....", outDateConnectionId);
                        //连接管理器注解注销该客户端连接对象，连接管理器注销连接对象的unregister()方法之后会为大家实现
                        connectionManager.unregister(outDateConnectionId);
                    }
                }
            }
            Loggers.CONNECTION.info("Connection check task end");
        } catch (Throwable e) {
            Loggers.CONNECTION.error("Error occurs during connection check... ", e);
        }
    }

    
}
```

以上就是我为大家定义的 NacosRuntimeConnectionEjector 连接驱逐器，上面代码块中的注释非常详细，而且之前对连接驱逐器的实现思路分析得也非常详细，所以我就不再重复讲解上面代码块的逻辑了，大家认真看一看就成。当然，有一点需要简单提一下，那就是我相信有很多朋友都注意到了，在上面代码块的第 116 行创建了一个 ClientDetectionRequest 请求对象，然后 nacos 服务端使用客户端连接对象把这个 ClientDetectionRequest 请求发送给了客户端，这个 ClientDetectionRequest 请求就是 nacos 服务端向客户端主动探活请求。而我之所以忽然提到这个请求，是因为我想有些朋友可能会觉得这个 ClientDetectionRequest 请求非常熟悉，没错，因为我已经把 nacos 客户端接收并处理该请求的操作实现了，在前几个版本代码就展示给大家了，并且添加了很详细的注释。如果大家仔细看我提供的前六个版本代码了，肯定就知道 nacos 客户端在哪里接收并处理服务端发送过来的 ClientDetectionRequest 探活请求。如果有的朋友忘了，那就可以直接去 nacos 客户端的 RpcClient 类的 start() 方法中查看，具体代码我就不展示。

好了，现在 NacosRuntimeConnectionEjector 连接驱逐器也定义完毕了，**连接驱逐器的 ejectOutdatedConnection() 方法会执行真正的过期连接检测操作，也就是 nacos 服务端的健康检测操作**。但是实现了连接驱逐器只是第一步，重要的是如何让连接驱逐器开始工作，也就是说如何让连接驱逐器的 ejectOutdatedConnection() 方法被执行，并且还需要周期执行，健康检测肯定要定义为一个定时任务啊，这个大家肯定都清楚。那这个定时任务应该怎么定义呢？其实非常简单，**只需要执行连接驱逐器的 ejectOutdatedConnection() 方法即可，但从上面代码块中可以看到，该方法实际上会被连接驱逐器的 doEject() 方法调用，那我们就直接定义一个定时任务，然后在定时任务中直接执行连接驱逐器的 doEject() 方法即可**。这个思路大家应该也可以理解吧，如果理解了这一点，就可以继续思考新的问题，那就是我们要定义的定时任务最终是在哪里创建并提交给定时任务执行器的呢？这就没什么好分析的了，**在 nacos 源码中把服务端健康检测的定时任务定义在了 ConnectionManager 连接管理器中**，毕竟连接管理器，连接驱逐器都是有关联的组件，我们也模仿实现即可。接下来，就让我来给大家再次重构一下 ConnectionManager 连接管理器。

## 再次重构 ConnectionManager 连接管理器，实现服务端健康检测功能

因为要把服务端健康检测的定时任务定义在 ConnectionManager 连接管理器中，而该定时任务需要执行连接驱逐器的 doEject() 方法，所以我就把连接驱逐器定义为 ConnectionManager 连接管理器的一个成员变量了，这样一来在 ConnectionManager 连接管器器初始化的时候，就可以直接创建连接驱逐器对象给内部的连接驱逐器成员变量赋值，然后创建服务端健康检测定时任务提交给定时任务执行器即可。接下来，就请大家看看我重构完毕之后的 ConnectionManager 连接管理器，请看下面代码块。

```
package com.cqfy.nacos.core.remote;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：客户端连接管理器，这个类的对象是服务端非常重要的一个组件，这个类的对象也要交给springboot容器来管理，在第七版本代码中，这个连接管理器只存放来自SDK客户端的连接对象
 */
@Service
public class ConnectionManager {

    private static final Logger LOGGER = Loggers.CONNECTION;

    //这个map的作用就很直接了，key就是客户端连接ID，value就是建立的连接对象，服务端把接收到的客户端连接存放到连接管理器中，就是存放到这个map中
    Map<String, Connection> connections = new ConcurrentHashMap<>();


    //这个就是运行时连接驱逐器，具体作用请看这个类，主要作用就是用来定期驱逐过期的客户端连接以及在服务器过载时驱逐一些客户端连接
    private RuntimeConnectionEjector runtimeConnectionEjector;



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：启动连接管理器的方法，因为ConnectionManager连接管理器对象要交给springboot容器管理，这个启动方法上添加了@PostConstruct注解，所以当连接管理器对象创建完毕后
     * start方法就会被调用
     */
    @PostConstruct
    public void start() {
        //初始化运行时连接拒绝器
        initConnectionEjector();
        //向RpcScheduledExecutor定时任务执行器提交定时任务
        RpcScheduledExecutor.COMMON_SERVER_EXECUTOR.scheduleWithFixedDelay(() -> {
            //运行时连接拒绝器的doEject方法就是定时任务要执行的操作
            runtimeConnectionEjector.doEject();
        }, 1000L, 3000L, TimeUnit.MILLISECONDS);
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：初始化运行时连接拒绝器的方法
     */
    public void initConnectionEjector() {


        //这里我省略了一大段代码，省略的代码的作用就是使用spi加载用户自己定义的连接拒绝器，为了简化文章，所以我省略了这部分代码
        //大家知道意思即可，在我提供的第七版本代码中，这部分内容是完整的，并且注释非常详细，逻辑也非常简单，大家自己看看代码即可
        
        //判断连接拒绝器是否赋被赋值了
        if (runtimeConnectionEjector == null) {
            //如果没有找到就使用nacos内置的连接拒绝器即可，创建一个连接拒绝器对象，也就是我刚才提到的NacosRuntimeConnectionEjector对象
            //这个连接拒绝器的名称就是nacos
            NacosRuntimeConnectionEjector nacosRuntimeConnectionEjector = new NacosRuntimeConnectionEjector();
            //设置连接管理器
            nacosRuntimeConnectionEjector.setConnectionManager(this);
            //赋值给runtimeConnectionEjector成员变量
            runtimeConnectionEjector = nacosRuntimeConnectionEjector;
        }
    }

    

    

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：注册客户端连接的方法
     */
    public synchronized boolean register(String connectionId, Connection connection) {
        //判断客户端与服务端的连接是否真的建立了
        if (connection.isConnected()) {
            
            //在这里把客户端连接Id和服务端为其创建的连接对象添加到映射表中
            connections.put(connectionId, connection);
            
            //记录操作日志
            LOGGER.info("new connection registered successfully, connectionId = {},connection={} ", connectionId, connection);
            
            return true;
        }
        return false;

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：得到指定客户端ID的客户端连接对象
     */
    public Connection getConnection(String connectionId) {
        
        return connections.get(connectionId);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：刷新指定客户端连接的最新活跃时间
     */
    public void refreshActiveTime(String connectionId) {
        //根据客户端ID得到对应的客户端连接对象，也就是GrpcConnection对象
        Connection connection = connections.get(connectionId);
        //判断连接对象是否存在
        if (connection != null) {
            //如果存在则刷新连接对象的最新活跃时间，这里刷新活跃时间的时候，就会调用连接元数据对象的setLastActiveTime方法，给最新活跃时间赋值
            connection.freshActiveTime();
        }
    }

    

    //其他内容暂时不做实现
}
```

现在，ConnectionManager 连接管理器就已经重构完毕了，通过上面的代码块，大家就可以知道 nacos 服务端健康检测定时任务究竟是怎么创建的，又是怎么提交给定时任务执行器的，还有 ConnectionManager 连接管理器中的连接驱逐器成员变量是怎么被赋值的。如果这些逻辑大家都彻底掌握了，那么这一章的核心内容也就结束了。当然，本章内容进行到这里，大家肯定也能想到 ConnectionManager 连接管理器的内容绝非文章中展示的这么点，在 ConnectionManager 类中，实际上还有很多内容等待我们添加，也就是说，ConnectionManager 连接管理器肯定还会经过多次重构。确实是这样，就比如说之前我们看到的 ConnectionManager 连接管理器注销客户端连接对象的 unregister() 方法，我就没有为大家实现。除此之外，在 nacos 源码中 ConnectionManager 连接管理器还负责客户端 IP 计数，以及限制 nacos 服务端接收客户端连接数量等等，这些功能会在以后的文章中全部重构完整，这不是什么着急的任务，也不是什么难以实现的功能，顶多和 web 控制台界面交互一下而已。

现在我们还是回到正题吧，诸位，请别忘了上一章还有最后一个问题没有解决呢，那就是**服务端想给客户端 B 主动发送消息时，怎么就知道客户端 B 的连接 Id 是多少呢**？这个问题一直没有为大家解决，已经拖得够久了，怎么着也该轮到这个问题被解决了吧？当然，要解决这个问题也需要恰当的时机，而现在时机已经成熟了，在上一章我们已经为 nacos 服务端引入了 RequestHandler 请求处理器，并且也定义了各种各样的请求处理器，比如处理客户端注册服务实例请求的 InstanceRequestHandler 处理器、处理客户端订阅服务实例信息的 SubscribeServiceRequestHandler 请求处理器，处理客户端健康检测请求的 HealthCheckRequestHandl 请求处理器。在这三个处理器中，HealthCheckRequestHandl 请求处理器已经实现完毕了，还剩下 InstanceRequestHandler 和 SubscribeServiceRequestHandler 这两个请求处理器没有实现，按顺序来说，也该实现这两个请求处理器了，并且我会首先为大家实现 InstanceRequestHandler 注册客户端服务实例信息的请求处理器。因为只有客户端注册了服务实例信息，其他客户端才能从 nacos 服务端订阅相关的服务实例信息呀，一旦有客户端订阅了服务实例信息，服务端才可以根据服务实例信息的变化，判断是否要主动向对应的客户端发送最新的服务实例信息，这个时候我们就会真正解决一直遗留的那个问题。

以上这些都是后面章节的内容了，大家看完本章内容之后，仍然可以去我提供的第七版本代码中查看本章新引入的连接驱逐器以及重构之后的连接管理器的代码，只不过大家会发现，我提供的第七版本代码中的连接驱逐器和连接管理器，比文章展示的内容要丰富一些，大家可以结合详细的代码注释，仔细看看多出来的代码具体负责什么功能，内容并不难，就当是留给大家的练习吧。好了朋友们，我们下一章见！