还是老样子，在本章内容开始之前，先把上一章遗留的几个问题展示一下：

**1 虽然引入了过滤器和拦截器组件，但我们不知道如何让这两个组件发挥作用，不知道怎么把它们设置到 grpc 的服务端中。**

**2 虽然创建了客户端连接 Id，也知道了连接对象是什么，但我们还不知道服务端在哪里为接收到的每一个连接创建 GrpcConnection 连接对象，也不知道在哪里把客户端连接 Id 和对应的 GrpcConnection 连接对象注册到 ConnectionManager 连接管理器中。**

**3 GrpcConnection 类的 StreamObserver 成员可能会让大家感到困惑。因为我们还没有为 grpc 服务端实现处理双向流消息的功能，双向流模式都没有引入呢，那这个可以主动向客户端发送消息的流式对象是从哪传递过来的呢？**

**4 之前为大家实现的 GrpcRequestAcceptor 单向流请求接收器的功能并没有实现完整**。

**5 服务端想给客户端 B 主动发送消息时，怎么就知道客户端 B 的连接 Id 是多少呢**？

以上 5 个问题就是我们尚未解决的问题，也是我们本章要解决的重点问题。随着这些问题的解决，很多新的组件也会不断引入到我们的程序中，我们的程序也会不断重构完善，直到和 nacos 框架源码一致。接下来，就让我们着手解决这些问题吧。

## 引入 GrpcBiStreamRequestAcceptor 双向流消息接收器

  

这一章我并不打算从第一个问题开始解决，因为在上一章结尾我已经跟大家稍微透露了一些重点内容：**那就是随着 grpc 服务端实现了双向流处理客户端消息的功能，这些未解决的问题几乎就全都可以解决了，大家也会看到在双向流模式中，我们之前引入的这几个组件是如何串联起来，协同工作的**。所以，这一章我打算直接就为 grpc 服务端实现处理客户端双向流消息功能，也许真的会向我说得这样，随着双向流功能的实现，所有问题都迎刃而解了。

在前面我们已经为 grpc 服务端实现了处理客户端单向流请求的功能，因为客户端单向流请求调用的目标方法就是 request 方法，所以我们在 grpc 服务端定义了一个单向流请求接收器，也就是 GrpcRequestAcceptor 组件。现在我们要为 grpc 服务端实现处理客户端双向流的功能，其实就仿照着单向流接收器来实现就行。**我们已经知道了客户端发送双向流的目标方法是 requestBiStream 方法，那就可以直接为 grpc 服务端定义一个双向流消息接收器，然后在这个接收器中实现 requestBiStream 方法。比如我们就可以把这个双向流消息接收器定义为 GrpcBiStreamRequestAcceptor，也就是流式请求接收器的意思，然后在这个类中定义提供服务的目标方法**。具体实现请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：双向流请求接收器，这个接收器主要负责处理服务端接收到的客户端连接，是非常核心的一个组件
 */
@Service
public class GrpcBiStreamRequestAcceptor extends BiRequestStreamGrpc.BiRequestStreamImplBase {


    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/15
    * @方法描述：该方法是本类最核心的方法，也是处理客户端连接的核心方法
    */
    @Override
    public StreamObserver<Payload> requestBiStream(StreamObserver<Payload> responseObserver) {

         //首先创建一个观察者对象，这个观察者就用来处理客户端发送过来的双向流请求
        StreamObserver<Payload> streamObserver = new StreamObserver<Payload>() {

            /**
             * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
             * @author：陈清风扬，个人微信号：chenqingfengyangjj。
             * @date:2024/7/15
             * @方法描述：客户端发送过来双向流消息后，就会被服务端的这个方法接收并处理
             */
            @Override
            public void onNext(Payload payload) {

                //该方法暂时不做实现

            }

            //处理错误情况的方法
            @Override
            public void onError(Throwable t) {

                //该方法暂时不做实现
            }


            //处理消息发送完成的方法
            @Override
            public void onCompleted() {

                //该方法暂时不做实现
            }
        };
        
        return streamObserver;
    }
        
}
```

在上面的代码块中，可以看到我们刚刚引入的这个 GrpcBiStreamRequestAcceptor 双向流消息接收器继承 grpc 框架的 BiRequestStreamGrpc.BiRequestStreamImplBase 类，这意味着我们定义的这个双向流消息接收器也是 grpc 框架要使用的组件，肯定也要设置到 grpc 的服务端中。**除此之外就是在双向流请求接收器中定义的 requestBiStream() 目标方法了，在这个目标方法中我又定义了一个 streamObserver 流式观察者对象，这个流式观察者对象就是真正用来处理消息的对象**。可以看到，我并没有实现这个流式观察者对象中的三个方法，因为在实现之前，我还有另外的内容想跟大家聊聊。

请大家再次回顾一下，我们构建的 nacos 客户端和 nacos 服务端成功建立连接之后做了什么呢？这个要从 nacos 客户端的 GrpcClient 类的 connectToServer 方法中寻找答案。实际上在我为大家实现 grpc 服务端单向流请求接收器时，就已经带大家回顾过一次了。现在我把 nacos 客户端的相关代码再次搬运到这里，请大家再回顾一下，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端的抽象实现类，该类中定义了grpc客户端的核心功能，连接服务端的操作都定义在这个类中了
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class GrpcClient extends RpcClient {


    //省略其他内容




     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：连接到gprc服务端的方法，连接成功后，该方法会返回一个Connection对象
     */
    @Override
    public Connection connectToServer(ServerInfo serverInfo) {
        //接收连接id
        String connectionId = "";
        try {
            //创建grpc要用到的线程池，这个线程池也用在异步发送请求到服务器时，处理回调方法使用的，在回调方法中，处理接收到的服务器响应
            //和sofajraft也有点像
            if (grpcExecutor == null) {
                this.grpcExecutor = createGrpcExecutor(serverInfo.getServerIp());
            }
            
            //得到grpc服务器的端口号，其实就是nacos服务器端口号加上1000的偏移量
            int port = serverInfo.getServerPort() + rpcPortOffset();
            
            //创建channel
            ManagedChannel managedChannel = createNewManagedChannel(serverInfo.getServerIp(), port);
            
            //创建单向流存根对象
            RequestGrpc.RequestFutureStub newChannelStubTemp = createNewChannelStub(managedChannel);
            
            //检查连接服务器是否成功，真正建立客户端和服务端的连接
            Response response = serverCheck(serverInfo.getServerIp(), port, newChannelStubTemp);
            
            //如果连接成功，这里应该接收到ServerCheckResponse响应
            if (!(response instanceof ServerCheckResponse)) {
                shuntDownChannel(managedChannel);
                return null;
            }
            
            //得到服务端回复的ServerCheckResponse响应
            ServerCheckResponse serverCheckResponse = (ServerCheckResponse) response;
            
            //从响应中获取客户端连接id
            connectionId = serverCheckResponse.getConnectionId();

            
            //下面是创建双向流存根，发送双向流请求的操作，就先不具体展示了
            //实现服务端的双向流请求处理方法时再回顾这一部分的操作

            //省略方法其他内容
        
        }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：检查服务器是否连接成功的方法，其实就是真正创建客户端和服务端连接的方法
     */
    private Response serverCheck(String ip, int port, RequestGrpc.RequestFutureStub requestBlockingStub) {
        try {//创建检查服务器连接是否成功的请求
            ServerCheckRequest serverCheckRequest = new ServerCheckRequest();
            //把请求转换为protobuf定义的请求
            Payload grpcRequest = GrpcUtils.convert(serverCheckRequest);
            //发送给服务器并接受响应
            ListenableFuture<Payload> responseFuture = requestBlockingStub.request(grpcRequest);
            Payload response = responseFuture.get(clientConfig.serverCheckTimeOut(), TimeUnit.MILLISECONDS);
            return (Response) GrpcUtils.parse(response);
        } catch (Exception e) {
            LoggerUtils.printIfErrorEnabled(LOGGER,
                    "Server check fail, please check server {} ,port {} is available , error ={}", ip, port, e);
            if (this.clientConfig != null && this.clientConfig.tlsConfig() != null && this.clientConfig.tlsConfig()
                    .getEnableTls()) {
                LoggerUtils.printIfErrorEnabled(LOGGER,
                        "current client is require tls encrypted ,server must support tls ,please check");
            }
            return null;
        }
    }



    //省略其他内容
}
```

从上面的代码块中可以看到，当 nacos 客户端和 nacos 服务端构建连接时，会先通过 serverCheck() 方法向服务端发送一个 ServerCheckRequest 单向流请求，服务端的单向流接收器处理完 ServerCheckRequest 请求后，会回复给客户端一个 ServerCheckResponse 响应。在我为大家实现 grpc 服务端的单向流请求接收器时，就是按照客户端执行的操作实现了对应的流程，因此我们目前实现的 grpc 服务端单向流请求接收器还并不完善，只能处理客户端发送过来的 ServerCheckRequest 请求；**现在要实现 grpc 服务端的双向流消息接收器了，我还是想用原来的方式，也就是先看看 nacos 客户端和服务端构建连接成功之后对服务端执行了什么双向流操作，然后我在 grpc 服务端的双向流接收器中再实现对应的处理不就行了吗**？所以，现在我想顺着 nacos 客户端的 GrpcClient 类的 connectToServer 方法继续往下看，因为接下来就是 nacos 客户端双向流执行的一些操作，我把相关的代码也搬运过来了，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端的抽象实现类，该类中定义了grpc客户端的核心功能，连接服务端的操作都定义在这个类中了
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class GrpcClient extends RpcClient {


    //省略其他内容




     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：连接到gprc服务端的方法，连接成功后，该方法会返回一个Connection对象
     */
    @Override
    public Connection connectToServer(ServerInfo serverInfo) {
        //接收连接id
        String connectionId = "";
        try {
            //创建grpc要用到的线程池，这个线程池也用在异步发送请求到服务器时，处理回调方法使用的，在回调方法中，处理接收到的服务器响应
            //和sofajraft也有点像
            if (grpcExecutor == null) {
                this.grpcExecutor = createGrpcExecutor(serverInfo.getServerIp());
            }
            
            //得到grpc服务器的端口号，其实就是nacos服务器端口号加上1000的偏移量
            int port = serverInfo.getServerPort() + rpcPortOffset();
            
            //创建channel
            ManagedChannel managedChannel = createNewManagedChannel(serverInfo.getServerIp(), port);
            
            //创建单向流存根对象
            RequestGrpc.RequestFutureStub newChannelStubTemp = createNewChannelStub(managedChannel);
            
            //检查连接服务器是否成功，真正建立客户端和服务端的连接
            Response response = serverCheck(serverInfo.getServerIp(), port, newChannelStubTemp);
            
            //如果连接成功，这里应该接收到ServerCheckResponse响应
            if (!(response instanceof ServerCheckResponse)) {
                shuntDownChannel(managedChannel);
                return null;
            }
            
            //得到服务端回复的ServerCheckResponse响应
            ServerCheckResponse serverCheckResponse = (ServerCheckResponse) response;
            
            //从响应中获取客户端连接id
            connectionId = serverCheckResponse.getConnectionId();


            
            //下面是创建双向流存根，发送双向流请求的操作，就先不具体展示了


            
            //创建了一个双向的流式存根，这个存根对象也是grpc框架中的对象，这个对象既可以接收服务器发送过来的消息
            //也可以向服务器发送消息
            BiRequestStreamGrpc.BiRequestStreamStub biRequestStreamStub = BiRequestStreamGrpc.newStub(newChannelStubTemp.getChannel());
            
            //在这里创建了nacos客户端自己定义的连接对象
            GrpcConnection grpcConn = new GrpcConnection(serverInfo, grpcExecutor);
            
            //设置连接id
            grpcConn.setConnectionId(connectionId);
            
            
            //判断是否要重置当前连接支持的能力
            if (serverCheckResponse.isSupportAbilityNegotiation()) {
                //如果要重置连接能力就先执行reset方法，这里有一个比较关键的地方，那就是在reset方法中会创建一个new CountDownLatch(1)对象
                //下面就会用到
                this.recAbilityContext.reset(grpcConn);
                //将当前连接的能力置为null
                grpcConn.setAbilityTable(null);
            }

            
            //将grpcConn交给双向流存根使用
            StreamObserver<Payload> payloadStreamObserver = bindRequestStream(biRequestStreamStub, grpcConn);
            
            //接下来就是向nacos自己定义的请求中设置属性的操作了，持有上面创建的对象，grpcConn才可以向服务端发送消息
            grpcConn.setPayloadStreamObserver(payloadStreamObserver);
            grpcConn.setGrpcFutureServiceStub(newChannelStubTemp);
            grpcConn.setChannel(managedChannel);
            
            //创建一个ConnectionSetupRequest请求对象，这个请求对象很重要，因为客户端的很多信息都会封装在这个请求对象中
            ConnectionSetupRequest conSetupRequest = new ConnectionSetupRequest();
            
            //设置客户端版本信息
            conSetupRequest.setClientVersion(VersionUtils.getFullClientVersion());
            //设置客户端附加信息，其实这个就可以成为客户端元数据信息
            conSetupRequest.setLabels(super.getLabels());
            //设置租户
            conSetupRequest.setTenant(super.getTenant());
            
            //在这里发送请求给服务器，服务器接收到请求后就知道客户端的详细信息了
            grpcConn.sendRequest(conSetupRequest);
            
            //同步等待响应，这里肯定是同步等待了，因为之前调用recAbilityContext的reset方法时，设置了同步等待标志
            if (recAbilityContext.isNeedToSync()) {
                //阻塞当前线程
                recAbilityContext.await(this.clientConfig.capabilityNegotiationTimeout(), TimeUnit.MILLISECONDS);
                //对grpcConn进行判空操作，如果服务端发送回来的能力为空，则客户端会主动断开连接
                if (!recAbilityContext.check(grpcConn)) {
                    return null;
                }
            } else {
                //这里是为了兼容旧版服务器
                Thread.sleep(100L);
            }
            //返回连接对象
            return grpcConn;
        } catch (Exception e) {
            LOGGER.error("[{}]Fail to connect to server!,error={}", GrpcClient.this.getName(), e);
            recAbilityContext.release(null);
        }
        return null;
        
    }

    //省略其他内容
}
```

上面代码块 62 行之后，都是 nacos 客户端执行的双向流的操作，可以看到，nacos 客户端创建了一个 ConnectionSetupRequest 请求，然后使用 nacos 客户端创建的 GrpcConnection 连接对象，调用了该对象的 sendRequest() 方法，把请求发送给服务端了。注意**，在 nacos 客户端连接对象 GrpcConnection 的 sendRequest() 方法内部，就是使用双向流存根对象把请求发送给 nacos 服务端的，所以我才说这里发送得是一个双向流请求**。既然这个操作流程梳理清楚了，那么接下来我们实现 grpc 服务端双向流消息接收器时就很简单了。我只需要在 grpc 服务端的 GrpcBiStreamRequestAcceptor 接收器中实现对应的操作就行，比如我就可以让双向流消息接收器处理客户端发送过来的 ConnectionSetupRequest 请求即可。这么做完全没有问题，但是，我还是要再说一句，如果我们为 grpc 服务端定义的 GrpcBiStreamRequestAcceptor 双向流消息接收器就只有处理客户端发送过来的 ConnectionSetupRequest 请求的功能，那这个 GrpcBiStreamRequestAcceptor 接收器也未免太没用了。所以，这个 GrpcBiStreamRequestAcceptor 双向流消息接收器肯定不会定义得这么简单**，处理客户端发送过来的 ConnectionSetupRequest 请求的功能肯定要实现**，那除此之外，双向流消息接收器还要实现什么其他功能呢？这时候就又要从 nacos 客户端的角度来思考了。

请大家思考一下，就目前我们为 grpc 服务端实现的这两个接收器，一个是单向流请求接收器，一个是双向流消息接收器，不管是哪一个接收器，在实现它们的时候，都是从 nacos 客户端执行的操作出发，然后实现了服务端接收器组件的对应内容。如果我们再观察得仔细一点就会发现，在 nacos 客户端的 GrpcClient 类的 connectToServer 方法中执行的操作都发生在客户端和服务端连接构建完毕之后。也就是说，当 nacos 客户端和服务端的连接成功建立了，客户端就会向服务端执行一些操作，**比如就是发送一个双向流 ConnectionSetupRequest 请求，关于这个请求的作用我在代码中展示的非常详细，就是让客户端告诉服务端自己的一些信息，比如客户端版本号，客户端应用名称以及客户端支持的能力等等**。总之，这个请求是在 nacos 客户端和服务端连接构建成功之后立刻执行的操作，并且这个双向流消息会被 grpc 服务端的双向流消息接收器处理，这样一来，服务端就可以从 ConnectionSetupRequest 请求中获得客户端的信息了。**那 grpc 服务端获得到的客户端信息可以用来做什么呢？当然就是用来创建连接对象了，也就是服务端为每一个客户端创建的 GrpcConnection 连接对象**。

分析到这里，我相信大家已经非常清楚了，**当 nacos 客户端和服务端连接构建成功之后，客户端会向服务端发送一个 ConnectionSetupRequest 双向流请求，grpc 服务端的 GrpcBiStreamRequestAcceptor 双向流消息接收器会从这个请求中获得客户端的一些信息，比如客户端版本号，客户端应用名称以及客户端支持的能力等等。得到了这些客户端的信息之后，grpc 服务端就可以为这个客户端创建连接对象了，然后把客户端 Id 和对应的连接对象存放到 ConnectionManager 连接管理器中**。这也就是说，**grpc 服务端为每一个客户端连接创建连接对象的操作，以及把客户端 Id 和对应的连接对象注册到连接管理器的操作，都是在 GrpcBiStreamRequestAcceptor 双向流消息接收器中执行的**。我能想到，看到这里有些朋友可能会觉得困惑，为什么 grpc 服务端必须从客户端发送过来的 ConnectionSetupRequest 请求中获得客户端的附加信息呢？上一章我为大家展示的服务端为客户端连接创建的 GrpcConnection 连接对象好像并不需要这些客户端信息呀，**这个连接管理器似乎只需要两个成员变量，一个就是 netty 创建的客户端 Channel 成员变量，一个就是流式观察者对象**。我把上一章引入的 GrpcConnection 连接对象的代码搬运过来了，请看下面代码块。

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

现在我把 GrpcConnection 类的代码展示完毕了，然后我想让大家注意一下，在上面代码块的第 20 行，可以看到 GrpcConnection 类的构造方法的第一个参数是一个 ConnectionMeta 类型的对象，这些代码在上一章就为大家展示了，但是我并没有讲解，直到现在要实现双向流消息接收器了，我才会大家讲解。**实际上这个 ConnectionMeta 对象就是客户端连接的连接元信息对象，这个客户端连接元信息对象就封装了从客户端的 ConnectionSetupRequest 请求中传输过来的信息，还有就是 grpc 过滤器中获得的客户端的连接 Id，IP 地址等等信息**。我就不在文章中展示这个 ConnectionMeta 类的内容了，大家可以去我提供的第七版本代码中查看，这个类的内容非常简单，就是定义了一些成员变量和一些对应的 get、set 方法而已，除此之外，就只有一个刷新客户端连接最新的活跃时间的功能，这个大家也可以先去代码中看看，不看也没关系，后面我也会在文章中为大家实现。

好了，说了这么多，现在终于要实现 grpc 服务端的 GrpcBiStreamRequestAcceptor 双向流消息接收器了，经过刚才一系列分析，现在我对要实现的 GrpcBiStreamRequestAcceptor 双向流消息接收器的思路已经非常明确了。**首先从拦截器创建的 Context 对象中获得客户端的连接 Id、IP 地址、端口号等等信息，再从客户端发送过来的 ConnectionSetupRequest 请求中获得客户端的附加信息，紧接着就可以创建 ConnectionMeta 连接元信息对象，然后为当前客户端连接创建 GrpcConnection 连接对象，把连接对象注册到连接管理器中就行了**。这就是我明确的实现 grpc 服务端 GrpcBiStreamRequestAcceptor 双向流消息接收器的思路。具体实现请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：双向流请求接收器，这个接收器主要负责处理服务端接收到的客户端连接，是非常核心的一个组件
 */
@Service
public class GrpcBiStreamRequestAcceptor extends BiRequestStreamGrpc.BiRequestStreamImplBase {



    //客户端连接管理器，这个也是服务端内部非常核心的一个组件
    @Autowired
    ConnectionManager connectionManager;


    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/15
    * @方法描述：该方法是本类最核心的方法，也是处理客户端连接的核心方法
    */
    @Override
    public StreamObserver<Payload> requestBiStream(StreamObserver<Payload> responseObserver) {

         //首先创建一个观察者对象，这个观察者就用来处理客户端发送过来的双向流请求
        StreamObserver<Payload> streamObserver = new StreamObserver<Payload>() {


            //从Context中获得客户端连接Id等信息的操作


            //GrpcServerConstants.CONTEXT_KEY_CONN_ID.get()这行代码就会从Context中获得已经生成的客户端连接ID
            //注意，这个时候grpc客户端和服务端的连接都已经建立了，这时候大家其实可以回过头看看注册中心客户端那边
            //发送了什么请求和服务端建立了连接，然后又发送了什么双向流请求，需要服务端双向流接收器处理这个双向流请求，这里我提示一下，其实就是ConnectionSetupRequest请求
            final String connectionId = GrpcServerConstants.CONTEXT_KEY_CONN_ID.get();
            //从Context中得到本地端口号
            final Integer localPort = GrpcServerConstants.CONTEXT_KEY_CONN_LOCAL_PORT.get();
            //从Context中得到客户端端口号
            final int remotePort = GrpcServerConstants.CONTEXT_KEY_CONN_REMOTE_PORT.get();
            //从Context中得到客户端IP地址，这里得到的就是 127.0.0.1，因为我是本机测试，客户端和服务端都在同一台机器上
            String remoteIp = GrpcServerConstants.CONTEXT_KEY_CONN_REMOTE_IP.get();
            //定义一个字符串，用来接收真正的客户端IP地址
            String clientIp = "";
            

            /**
             * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
             * @author：陈清风扬，个人微信号：chenqingfengyangjj。
             * @date:2024/7/15
             * @方法描述：客户端发送过来双向流消息后，就会被服务端的这个方法接收并处理
             */
            @Override
            public void onNext(Payload payload) {

                //首先从请求元数据中获得真实的客户端IP地址
                clientIp = payload.getMetadata().getClientIp();


                
                //定义一个变量接收客户端发送过来的请求
                Object parseObj;
                try {//在这里把请求解析成Object对象
                    parseObj = GrpcUtils.parse(payload);
                } catch (Throwable throwable) {
                    //有异常则记录日志
                    Loggers.REMOTE_DIGEST.warn("[{}]Grpc request bi stream,payload parse error={}", connectionId, throwable);
                    return;
                }
                //如果解析之后的对象为空，则记录告警日志
                if (parseObj == null) {
                    Loggers.REMOTE_DIGEST.warn("[{}]Grpc request bi stream,payload parse null ,body={},meta={}", connectionId, payload.getBody().getValue().toStringUtf8(), payload.getMetadata());
                    return;
                }



                //处理客户端发送过来的ConnectionSetupRequest请求的操作


                 //判断客户端发送过来的请求是否为ConnectionSetupRequest请求，大家可以回顾回顾客户端什么时候会向服务端发送ConnectionSetupRequest请求
                //这些内容在实现客户端的时候都讲解过了，如果大家忘了可以翻翻以前的文章回顾一下
                if (parseObj instanceof ConnectionSetupRequest) {
                    
                    //如果是则将对象转换为ConnectionSetupRequest类型的请求对象
                    ConnectionSetupRequest setUpRequest = (ConnectionSetupRequest) parseObj;
                    //得到从客户端传输过来的标签map，这个标签中封装了客户端的一些基本信息，比如该客户端是sdk客户端，并不是集群客户端等等
                    Map<String, String> labels = setUpRequest.getLabels();
                    String appName = "-";
                    //得到从客户端传输过来的应用名称
                    if (labels != null && labels.containsKey(Constants.APPNAME)) {
                        appName = labels.get(Constants.APPNAME);
                    }




                    //在下面创建了连接元信息对象

                    
                    //创建客户端连接元数据对象，这个连接元数据对象的作用并不是非常大，在刷新客户端连接最新活跃时间的时候会用到，很快我就会为大家展示这个对象的具体使用方法
                    ConnectionMeta metaInfo = new ConnectionMeta(connectionId, payload.getMetadata().getClientIp(),
                            remoteIp, remotePort, localPort, ConnectionType.GRPC.getType(),
                            setUpRequest.getClientVersion(), appName, setUpRequest.getLabels());
                    //设置租户信息
                    metaInfo.setTenant(setUpRequest.getTenant());




                    //创建GrpcConnection客户端连接对象的操作
                    
                    
                    //创建grpc连接对象，这个连接对象就是专门用来给客户端发送消息的，在注册中心客户端不是也有一个连接对象吗？那个连接对象是用来向服务端发送消息的
                    //GrpcServerConstants.CONTEXT_KEY_CHANNEL.get()得到的就是netty创建的客户端Channel
                    //现在大家应该知道GrpcConnection对象中的流式通信对象是从哪里来的了吧
                    GrpcConnection connection = new GrpcConnection(metaInfo, responseObserver, GrpcServerConstants.CONTEXT_KEY_CHANNEL.get());
                    
                    //检查从客户端传输过来的客户端能力表是否不为空
                    if (setUpRequest.getAbilityTable() != null) {
                        //如果不为空则把客户端能力设置到链接对象中
                        connection.setAbilityTable(setUpRequest.getAbilityTable());
                    }



                    
                    //从连接元数据对象中判断当前客户端连接是否来自sdk客户端，并且判断程序是否已经启动完毕，这个时候通过ApplicationUtils.isStarted()这行代码，大家应该就清楚了nacos定义的StartingApplicationListener监听器
                    //是怎么发挥作用的了吧？
                    //如果这两个条件有一个不满足，就意味着服务端在刚刚启动的时候拒绝接收来自sdk客户端的连接
                    boolean rejectSdkOnStarting = metaInfo.isSdkSource() && !ApplicationUtils.isStarted();



                    //下面就是把刚刚创建好的GrpcConnection连接对象注册到ConnectionManager连接管理器中的操作

                    
                    //如果服务端拒绝接收连接，或者客户端连接注册到连接管理器时失败了，就记录告警日志，并且关闭客户端连接
                    //connectionManager.register(connectionId, connection)这行代码就把客户端连接对象注册到连接管理器中了
                    //key就是客户端连接ID，vale就是刚才创建的GrpcConnection对象
                    if (rejectSdkOnStarting || !connectionManager.register(connectionId, connection)) {
                        try {
                            //记录告警日志，告警信息为服务端还未完全启动，或者服务端负载过高
                            Loggers.REMOTE_DIGEST.warn("[{}]Connection register fail,reason:{}", connectionId,rejectSdkOnStarting ? " server is not started" : " server is over limited.");
                            //在这里关闭客户端连接
                            connection.close();
                        } catch (Exception e) {
                            //关闭连接出现异常则记录告警日志
                            if (connectionManager.traced(clientIp)) {Loggers.REMOTE_DIGEST.warn("[{}]Send connect reset request error,error={}", connectionId, e);
                            }
                        }
                    } else {
                        try {//走动这里就意味着连接成功注册到服务端的连接管理器中了
                            //这个时候直接向客户端回复成功请求即可，就使用刚才创建的GrpcConnection对象发送无需应答的请求即可
                            //NacosAbilityManagerHolder.getInstance().getCurrentNodeAbilities(AbilityMode.SERVER))这行代码得到的就是服务端的能力列表
                            //把它发送给客户端，客户端就可以同步服务端的能力了
                            connection.sendRequestNoAck(new SetupAckRequest(NacosAbilityManagerHolder.getInstance().getCurrentNodeAbilities(AbilityMode.SERVER)));
                        } catch (Exception e) {

                        }
                    }

                }
                    
                    

                //以下就是处理grpc服务端接收到的双向流响应的操作，这一部分就不在文章中展示了，等真正实现的时候再为大家展示
                    
                    
                //走到这里意味着服务端接收到的消息并不是ConnectionSetupRequest类型的请求，所以就要判断一下接收到的消息是否为响应对象
                else if (parseObj instanceof Response) {
                  //这里就暂时不做实现了
                }
                    
                else {
                    //走到这里就意味着服务端双向流处理器接收到的消息并不是预期的消息，这时候就记录告警日志
                    Loggers.REMOTE_DIGEST.warn("[{}]Grpc request bi stream,unknown payload receive ,parseObj={}", connectionId, parseObj);
                }

                

            }

            //处理错误情况的方法
            @Override
            public void onError(Throwable t) {

                //该方法就不展示了，去我提供的第七版本代码中查看即可
            }


            //处理消息发送完成的方法
            @Override
            public void onCompleted() {

                //该方法就不展示了，去我提供的第七版本代码中查看即可
            }
        };
        
        return streamObserver;
    }
        
}
```

到此为止，我就为大家把 grpc 服务端的 GrpcBiStreamRequestAcceptor 双向流消息接收器实现完毕了，上面代码块中的注释非常详细，而且在实现之前我把实现的思路分析得非常透彻，所以我就不再重复讲解上面代码块的内容了，大家可以再仔细品味品味。

如果上面的逻辑大家都清楚了，接下来让我来猜一猜，我想也许有朋友心里又有了新的问题，可能会疑惑：grpc 服务端的单向流请求接收器和双向流消息接收器都是接收器，都可以处理来自客户端的请求，并且 nacos 客户端和服务端构建连接的时候会向这两个接收器发送不同的请求，执行不同的操作，那么，为什么不把创建客户端连接对象，注册客户端连接对象的操作定义在单向流请求接收器中呢？这是因为 nacos 服务端已经为这两个接收定好明确的分工了，**GrpcRequestAcceptor 单向流请求接收器主要接收并处理客户端发送的功能请求，比如注册服务实例信息、订阅服务实例信息、取消订阅服务实例信息等等操作，它的功能仅限于此，非常单一，在后面的章节中大家就会体会到这一点；而 GrpcBiStreamRequestAcceptor 双向流消息接收器主要负责向客户端主动发送消息，处理客户端回复的双向流响应，当然还有创建客户端连接对象，以及注册客户端连接对象，当然，主动向客户端发送消息肯定是双向流消息接收器最频繁的操作，比如向客户端发送最新的服务实例信息、最新的配置信息等等**。

好了，这个问题分析完了，接下来就可以回头看看文章一开始列出的 5 个问题了：

**1 虽然引入了过滤器和拦截器组件，但我们不知道如何让这两个组件发挥作用，不知道怎么把它们设置到 grpc 的服务端中。**

**2 虽然创建了客户端连接 Id，也知道了连接对象是什么，但我们还不知道服务端在哪里为接收到的每一个连接创建 GrpcConnection 连接对象，也不知道在哪里把客户端连接 Id 和对应的 GrpcConnection 连接对象注册到 ConnectionManager 连接管理器中。**

**3 GrpcConnection 类的 StreamObserver 成员可能会让大家感到困惑。因为我们还没有为 grpc 服务端实现处理双向流消息的功能，双向流模式都没有引入呢，那这个可以主动向客户端发送消息的流式对象是从哪传递过来的呢？**

**4 之前为大家实现的 GrpcRequestAcceptor 单向流请求接收器的功能并没有实现完整**。

**5 服务端想给客户端 B 主动发送消息时，怎么就知道客户端 B 的连接 Id 是多少呢**？

目前来看，这 5 个问题中，有两个已经被解决了，也就是第 2 个和第 3 个问题被解决了，第一个问题还没有解决，并且我在上一章结尾还说过，随着双向流功能的引入，目前引入的所有组件都会被串联起来，协同工作。这个不假，只不过不是在双向流消息接收器中体现出来，而是在 grpc 服务端中体现出来。别忘了，我们目前引入的组件都是为 grpc 服务端引入的，需要把这些组件设置到 grpc 服务端中，这些组件才能发挥作用，一起工作，这也就意味着我们需要重构 grpc 服务端，也就是 BaseGrpcServer 类。

## 重构 BaseGrpcServer 类，实现完整的 grpc 服务端

其实重构 BaseGrpcServer 类已经没什么可分析的了，BaseGrpcServer 中构建的就是 grpc 服务端，而我们现在引入的组件，过滤器，拦截器，请求接收器等等都是 grpc 服务端要使用的组件，直接把它们设置到 grpc 服务端即可，而这正是 grpc 框架的内容，使用的也是 grpc 框架的编码方式。所以，我就直接给大家把代码展示在下面了，接下来就请大家看看重构之后的 BaseGrpcServer 类是什么样的，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/10
 * @方法描述：这个类是nacos服务端的grpc服务端基础类，在这个类中定义了服务器的多个重要组件，可以说是nacos服务端的核心类
 */
public abstract class BaseGrpcServer extends BaseRpcServer {

    //grpc服务端对象，这个grpc框架的类
    private Server server;

    //grpc请求接收器，这个请求接收器接收的是单向流存根发送的请求
    @Autowired
    private GrpcRequestAcceptor grpcCommonRequestAcceptor;

    //这个也是grpc请求接收器，只不过这个接收器接收的是grpc双向流存根发送得请求
    @Autowired
    private GrpcBiStreamRequestAcceptor grpcBiStreamRequestAcceptor;


    //得到连接类型的方法，使用的是grpc框架，所以这里返回的就是grpc类型的连接
    @Override
    public ConnectionType getConnectionType() {
        return ConnectionType.GRPC;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/10
     * @方法描述：启动服务器的方法
     */
    @Override
    public void startServer() throws Exception {
        //创建一个grpc的服务注册表对象
        final MutableHandlerRegistry handlerRegistry = new MutableHandlerRegistry();


        //在addServices()方法中把拦截器，两个请求接收器设置到grpc服务端中了
        
        //得到grpc框架的拦截器，并且把拦截器添加到服务注册表中，这里我解释一下，在使用grpc框架的时候，除了拦截器，还有过滤器
        //并且过滤器会先比拦截器发挥作用，在我为大家提供的第七版本代码中，grpc框架的过滤器和拦截器处理的工作都和客户端连接有关，很快大家就会看到了
        addServices(handlerRegistry, getSeverInterceptors().toArray(new ServerInterceptor[0]));
        
        //因为grpc底层使用的也是netty构建得服务器，所以这里先得到一个netty服务构建器，设置服务器端口号以及要用到的线程池
        NettyServerBuilder builder = NettyServerBuilder.forPort(getServicePort()).executor(getRpcExecutor());



        //下面就是设置过滤器到grpc服务端中的方法
        
        
        //接下来就是得到grpc要使用的所有过滤器，并且把过滤器设置到构建器中
        for (ServerTransportFilter each : getServerTransportFilters()) {
            builder.addTransportFilter(each);
        }
        
        //下面就是构建grpc服务器的操作了，设置grpc服务器最大入站消息大小
        server = builder.maxInboundMessageSize(getMaxInboundMessageSize())
                //设置grpc服务注册表
                .fallbackHandlerRegistry(handlerRegistry)
                //设置grpc消息压缩器注册表
                .compressorRegistry(CompressorRegistry.getDefaultInstance())
                //设置grpc消息解压缩器注册表
                .decompressorRegistry(DecompressorRegistry.getDefaultInstance())
                //设置连接最大的检测时间，如果在这个时间段之内客户端与服务端没有通信，那么在这个时间段之后，服务端向客户端发送探活消息
                .keepAliveTime(getKeepAliveTime(), TimeUnit.MILLISECONDS)
                //设置服务端接收客户端探活响应的超时时间
                .keepAliveTimeout(getKeepAliveTimeout(), TimeUnit.MILLISECONDS)
                //设置客户端向服务端发送ping的频率，keepAliveTime、keepAliveTimeout和permitKeepAliveTime都是grpc内部需要用到的参数
                //后面更新grpc的时候，大家会认识得更清楚
                .permitKeepAliveTime(getPermitKeepAliveTime(), TimeUnit.MILLISECONDS)
                //构建服务端
                .build();
        //在这里启动grpc服务器
        server.start();
    }


   

    //获得服务器要使用的拦截器的集合
    protected List<ServerInterceptor> getSeverInterceptors() {
        List<ServerInterceptor> result = new LinkedList<>();
        //在这里创建了一个GrpcConnectionInterceptor连接器对象，添加到集合中了
        result.add(new GrpcConnectionInterceptor());
        return result;
    }

    
    //获得grpc服务端要使用的传输过滤器集合
    protected List<ServerTransportFilter> getServerTransportFilters() {
        //在这里创建了一个AddressTransportFilter过滤器，这个AddressTransportFilter过滤器会在拦截器之前先被执行
        return Collections.singletonList(new AddressTransportFilter());
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：把grpc服务端提供的服务添加到handlerRegistry服务注册表中的方法
     */
    private void addServices(MutableHandlerRegistry handlerRegistry, ServerInterceptor... serverInterceptor) {
        //下面是创建单向流服务处理器的操作
        //首先定义一个方法描述符
        final MethodDescriptor<Payload, Payload> unaryPayloadMethod = MethodDescriptor.<Payload, Payload>newBuilder()
                //该方法处理的请求都是单向流请求，也就是请求响应模式的请求
                .setType(MethodDescriptor.MethodType.UNARY)
                //设置方法的全名称，名称其实就是request方法
                .setFullMethodName(MethodDescriptor.generateFullMethodName(GrpcServerConstants.REQUEST_SERVICE_NAME, GrpcServerConstants.REQUEST_METHOD_NAME))
                //设置请求和响应的编解码器
                .setRequestMarshaller(ProtoUtils.marshaller(Payload.getDefaultInstance()))
                .setResponseMarshaller(ProtoUtils.marshaller(Payload.getDefaultInstance()))
                //创建方法描述符
                .build();


        //下面就是创建具体的单向流方法处理器的操作
        final ServerCallHandler<Payload, Payload> payloadHandler = ServerCalls.asyncUnaryCall(
                //这里可以看到，请求是被grpcCommonRequestAcceptor接收器接收，并且调用request方法处理了
                (request, responseObserver) -> grpcCommonRequestAcceptor.request(request, responseObserver));

        //创建单向流服务提供对象
        final ServerServiceDefinition serviceDefOfUnaryPayload = ServerServiceDefinition
                //设置服务名称
                .builder(GrpcServerConstants.REQUEST_SERVICE_NAME)
                //设置方法描述符和方法处理器
                .addMethod(unaryPayloadMethod, payloadHandler)
                .build();

        //将提供服务对象和拦截器添加到服务注册表中
        handlerRegistry.addService(ServerInterceptors.intercept(serviceDefOfUnaryPayload, serverInterceptor));


        //下面就是创建双向流服务处理器的操作了
        //首先创建双向流方法处理器的操作
        final ServerCallHandler<Payload, Payload> biStreamHandler = ServerCalls.asyncBidiStreamingCall(
                //这里可以看出来，双向流的操作都是被grpcBiStreamRequestAcceptor接收器调用requestBiStream方法处理的
                (responseObserver) -> grpcBiStreamRequestAcceptor.requestBiStream(responseObserver));

        //创建双向流方法描述符
        final MethodDescriptor<Payload, Payload> biStreamMethod = MethodDescriptor.<Payload, Payload>newBuilder()
                //该方法处理的都是双向流请求
                .setType(MethodDescriptor.MethodType.BIDI_STREAMING)
                //设置方法全名称
                .setFullMethodName(MethodDescriptor.generateFullMethodName(GrpcServerConstants.REQUEST_BI_STREAM_SERVICE_NAME, GrpcServerConstants.REQUEST_BI_STREAM_METHOD_NAME))
                //设置双向流请求响应的编解码器
                .setRequestMarshaller(ProtoUtils.marshaller(Payload.newBuilder().build()))
                .setResponseMarshaller(ProtoUtils.marshaller(Payload.getDefaultInstance()))
                .build();

        //创建双向流的服务提供对象，这个逻辑和单向流的逻辑类似，就不重复添加注释了
        final ServerServiceDefinition serviceDefOfBiStream = ServerServiceDefinition
                .builder(GrpcServerConstants.REQUEST_BI_STREAM_SERVICE_NAME)
                .addMethod(biStreamMethod, biStreamHandler)
                .build();
        handlerRegistry.addService(ServerInterceptors.intercept(serviceDefOfBiStream, serverInterceptor));

        //以上都是和grpc服务端密切相关的内容，大家可能不知道方法描述符，不知道ServerCallHandler是怎么发挥作用的，这些和grpc有关的内容就放到后面再详细讲解吧
        //在nacos中，大家只要知道拦截器、过滤器、以及本类中的两个Acceptor接收器是怎么发挥作用的就行，因为和nacos客户端交互的逻辑都在这几个组件中
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：停止服务器工作的方法
     */
    @Override
    public void shutdownServer() {
        if (server != null) {
            server.shutdownNow();
        }
    }


    //省略其他方法

}
```

到此为止，BaseGrpcServer 类也重构完毕了，各个组件都设置到 grpc 服务端中了，这也就意味着我们为 nacos 服务端定义的 grpc 服务端终于完整了。**现在 nacos 服务端已经实现了启动、接收客户端连接、为每一个客户端连接创建 GrpcConnection 连接对象、把 GrpcConnection 连接对象注册到 ConnectionManager 连接管理器中、使用单向流和双向流接收器处理客户端消息、可以使用双向流功能随时发送消息给客户端等功能**。看起来已经有点样子了，当然，要实现的功能还有很多，就比如说在本章开始引入的 5 个问题，现在第 1、2、3 个问题都已经解决了，就剩下第 4 个和第 5 个问题没有解决了，让我们再回顾一下这两个问题：

**4 之前为大家实现的 GrpcRequestAcceptor 单向流请求接收器的功能并没有实现完整**。

**5 服务端想给客户端 B 主动发送消息时，怎么就知道客户端 B 的连接 Id 是多少呢**？

虽然还有两个问题等待我们解决，但我还是那句话，在 nacos 这个框架中，没有难解决的问题，只有解决不完的问题。我的意思是 nacos 中所有的问题解决起来都很简单，但是随着每一个问题的解决，都会引入新的组件，而这些新的组件又会引入其他问题，最后由一个个问题把 nacos 服务端需要的所有组件都引出来，构成完整的 nacos 服务端。我想从前面几章的内容，大家对这一点已经有所体会了。这一章的内容已经够多了，所以我就不再为大家解决第 4 个问题了，这个问题留到下一章解决。好了朋友们，我们下一章见！

附：这一章看完之后，大家就可以去我提供的第七版本代码中查看对应的代码了，当然有些类的代码可能比文章中展示得更丰富，内容更多，这是因为这些类的内容我还没有在文章中讲解，大家可以先把 grpc 服务端的核心类都看一看，其他的内容等我在文章中讲解了再仔细查看。还有一点，我建议大家一定要把最近几章引入 nacos 服务端的几个组件好好梳理一下，就从服务端启动开始，到服务端接收消息、处理消息结束，看看这中间 nacos 服务端都执行了什么操作，操作流程是怎样的。因为这几章内容确实比较多，实现的功能也很多，大家自己一定要串一下流程，我就不在文章中梳理了，因为梳理下来又要写好多文字，还不如留给大家自己去梳理。我相信如果大家亲自梳理了，并且梳理清楚了，那么这几章的内容大家就全部掌握了，并且会深深印在脑海中了，很长一段时间不会忘记。