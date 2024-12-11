## 实现客户端对服务端的心跳检测功能

大家好，又到了新的一章，如果大家已经看完前面五章，并且亲自启动我提供的第一版本代码进行调试，想必大家应该都发现了，我提供的第一版本代码虽然可以启动，也可以把服务实例注册到服务端，但是启动一会之后，会自动断开和服务端的连接。在上一章结尾我也跟大家提前解释过这种情况了，因为我还没有把注册中心客户端和服务端的心跳检测功能实现。**在这里我想正是纠正一下，实际上这个功能不应该叫做心跳检测，而应该称为健康检查，并且这个健康检查是 nacos 自己内部定义的**。这句话又该怎么解释呢？还是听我慢慢道来。

  

在第二章我正式开始搭建注册中心客户端之前，提出了注册中心客户端应该具备的 5 个功能组件，这些内容想必大家还有印象吧，我把那 5 个要点再次搬运过来了：

**1 注册中心客户端需要和服务端进行网络通信，所以网络通信组件必不可少。**

**2 既然涉及到网络通信，那么各种通信处理器也必不可少。**

**3 注册中心客户端需要把服务实例注册到服务端的注册表中，所以注册中心客户端需要具备注册服务实例的功能。**

**4 为了与注册中心服务端的判活功能对接，注册中心客户端还需要提供向服务端发送心跳信息的功能。只有让服务端定期都接收到心跳信息，这样一来，服务端才会认为该客户端注册的所有服务实例都存活。**

**5 最后仍然是 init-start 开发模式。**

现在我已经把注册中心的客户端给大家构建完毕了，什么网络组件、注册服务实例功能、通信处理器、init-start 模式，这些都功能组件我都已经实现了。当然，我对通信处理器的实现可能有些模糊，在文章中讲解的篇幅比较少，但是通信处理器这个概念以及作用，也就是处理对应请求的 handler 的功能以及发挥功能的方式，我相信我已经给大家讲解清楚了，并且在第一版本代码中也引入了两个客户端处理服务端请求的通信处理器，如果大家仔细查看我提供的第一版本代码，应该能看到这两个处理器。在**以上 5 个功能组件中，我唯一没有实现的就是第 4 个，也就是所谓的心跳检测功能**。而我之所以在前几章没有实现这个功能，是因为从严格意义上来说，Grpc 框架客户端与服务端连接一旦建立了，**Grpc 框架就为我们提供了一个基本的心跳检测功能，准确地说其实就是使用了 Keepalive 机制。这样一来，在 Grpc 框架内部就会自己判断客户端与服务端连接是否存活**。但是这个 Keepalive 机制的时间可能有些长了，比如说 nacos 中使用的这个 Grpc 框架就把 Keepalive 的时间定义为了 6 分钟，也就是 6 分钟才检测一次连接状态。这恐怕是 nacos 注册中心不能接受的。**所以在 Grpc 框架的 Keepalive 机制之上，nacos 注册中心又定义了自己的健康检查机制，或者我们可以把 Grpc 框架看作通信基础层，而 nacos 的注册中心客户端与服务端看作应用层。这样一来，我们就可以说 nacos 框架实现了应用层的健康检查机制**。而我们这一章的首要内容，就是要先实现 nacos 实现得应用层的健康检查机制。

当然，nacos 自己实现的健康检查机制并不复杂，虽说需要客户端与服务端一起配合才能完成这个功能，但实现起来并不复杂，基本逻辑就是启动一个定时任务，定期执行健康检查操作。如果说得具体一点，**那就是注册中心客户端启动之后会和服务端建立连接，并且这个连接的元信息会存放在服务端；所谓连接元信息就包括客户端与服务端连接的最新活跃时间，而所谓最新活跃时间，就是客户端最后一次向服务端发送消息的时间。只要客户端向服务端发送消息了，不管是什么请求消息，服务端都会刷新连接元信息中的最后活跃时间**。以上就是客户端与服务端心跳检测的基础条件，接下来就简单了，**客户端启动之后会开启一个定时任务，在定时任务中定期向服务端发送健康检查请求，也就是 HealthCheckRequest 请求，而服务端接收到这个请求之后，会直接使用 HealthCheckRequestHandler 处理器回复给客户端一个 HealthCheckResponse，并且服务端在接收到客户端发送过来的请求时，还会刷新客户端连接的元信息。如果客户端向服务端发送了 HealthCheckRequest 请求，但是并没有接收到 HealthCheckResponse 响应，重试了几次之后依然如此，这时候客户端就可以认为自己和服务端的连接出现问题，那么就通过服务地址管理器得到一个新的服务地址，和新的服务器建立连接即可**。

以上就是 nacos 注册中心客户端健康检测的全部流程，当然，客户端分析完了，接下来就该分析一下服务端。**服务端的逻辑更简单，也是启动一个定时任务，定期判断客户端与服务端连接的最新活跃时间是否过期了，如果过期了，服务端就可以把连接注销**。这只是一个大概的流程，因为现在我还不会为大家引入服务端的功能，所以就只是先简单讲解一下，如果大家感兴趣，可以先自行看一下服务端 GrpcRequestAcceptor、ConnectionManager、NacosRuntimeConnectionEjector、ConnectionMeta 这几个类的内容，最好把 NacosRuntimeConnectionEjector 类当作突破口。

总之，现在我已经为大家把注册中心客户端健康检测的逻辑分析完毕了，接下来就为大家实现一下。在 nacos 源码中，这个功能定义在了 RpcClient 这个类中，实际上就是一个名为 healthCheck 的方法，接下来我就仿照源码，也把这个 healthCheck 健康检测方法定义在上一章我已经实现了的 RpcClient 类中。请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/11
 * @方法描述：客户端的顶级抽象父类，这个类中定义了一些公共方法，让http客户端和grpc客户端实现类去实现
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {


    //naocs服务器地址管理器
    private ServerListFactory serverListFactory;

    //源自引用类表示客户端的状态，客户端状态发生变更时可以安全更新
    protected volatile AtomicReference<RpcClientStatus> rpcClientStatus = new AtomicReference<>(RpcClientStatus.WAIT_INIT);

    //表示和服务器构建的连接对象
    protected volatile Connection currentConnection;

    //最新的收到服务器消息的时间戳
    private long lastActiveTimeStamp = System.currentTimeMillis();

    //rpc客户端的配置类
    protected RpcClientConfig rpcClientConfig;

    //这个集合存放了用于处理服务器发送的请求的处理器
    //服务器可能会向客户端主动发送各种各样的请求，所以就在客户端定义了一些处理器，专门用于处理对应的服务器发送过来的请求
    protected List<ServerRequestHandler> serverRequestHandlers = new ArrayList<>();



    //省略其他内容
    

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：启动rpc客户端的方法
     */
    public final void start() throws NacosException {
        
        //因为本类的serverListFactory方法会比start方法先调用，具体逻辑很快就会展示，在serverListFactory方法中会将客户端状态更新为初始化完毕状态
        //这就意味着这个时候客户端其实已经初始化完毕了，所以当前的客户端状态应该为初始化完毕状态，这里要做一个状态的判断
        boolean success = rpcClientStatus.compareAndSet(RpcClientStatus.INITIALIZED, RpcClientStatus.STARTING);
        if (!success) {
            //状态错误直接退出当前方法
            return;
        }
        
        
        //定义一个连接对象，用来接收客户端到服务端的连接
        Connection connectToServer = null;
        //更新客户端状态为开始状态
        rpcClientStatus.set(RpcClientStatus.STARTING);
        
        
        //获取连接重试次数
        int startUpRetryTimes = rpcClientConfig.retryTimes();
        //在循环中开始连接服务端，循环次数就为连接重试次数
        while (startUpRetryTimes >= 0 && connectToServer == null) {
            try {
                //连接失败一次，则次数减1
                startUpRetryTimes--;
                
                //因为上一次连接失败了，肯定是nacos服务端有问题，所以这里从服务地址管理器中获得一个新的服务端地址
                //ServerInfo是一个内部类，里面封装nacos服务器地址信息
                ServerInfo serverInfo = nextRpcServer();
                
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Try to connect to server on start up, server: {}",rpcClientConfig.name(), serverInfo);
                
                //连接服务器，连接成功则返回一个连接对象
                connectToServer = connectToServer(serverInfo);
            } catch (Throwable e) {
                
                LoggerUtils.printIfWarnEnabled(LOGGER,
                        "[{}] Fail to connect to server on start up, error message = {}, start up retry times left: {}",
                        rpcClientConfig.name(), e.getMessage(), startUpRetryTimes, e);
            }
        }

        //连接重试也失败的逻辑我并没有给大家展示，因为那是第二版本代码的功能，下一章才会为大家讲解
        
        //下面是连接成功的逻辑
        
        if (connectToServer != null) {
            LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Success to connect to server [{}] on start up, connectionId = {}",
                    rpcClientConfig.name(), connectToServer.serverInfo.getAddress(),
                    connectToServer.getConnectionId());
            
            //走到这里意味着连接成功，给currentConnection赋值即可
            this.currentConnection = connectToServer;
            
            //更新客户端运行状态
            rpcClientStatus.set(RpcClientStatus.RUNNING);
        }
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：客户端向服务端发送请求的方法
     */
    public Response request(Request request) throws NacosException {
        //超时时间，如果用户没有定义，那么默认为3秒，这个3秒默认时间是客户端配置类中定义好的
        return request(request, rpcClientConfig.timeOutMills());
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：真正向服务端发送请求的方法
     */
    public Response request(Request request, long timeoutMills) throws NacosException {
        
        //重试次数初始为0
        int retryTimes = 0;
        //接收响应对象
        Response response;
        //接收异常信息
        Throwable exceptionThrow = null;
        //得到操作的开始时间
        long start = System.currentTimeMillis();
        
        
        //在循环中发送请求到服务器，这里之所以是在循环中执行操作，是因为请求发送有可能失败
        //这时候就要重新发送请求，所以就要在循环中判断是否超过了规定的请求重试次数
        //以及是否超过了请求超时时间，这两个都没有超过才能继续重新发送请求
        while (retryTimes <= rpcClientConfig.retryTimes() && (timeoutMills <= 0 || System.currentTimeMillis() < timeoutMills + start)) {
            
            //定义一个是否等待重新连接服务器的标志
            boolean waitReconnect = false;
            
            try {
                //判断当前客户端是否处于运行状态，对当前连接判空
                if (this.currentConnection == null || !isRunning()) {
                    //有一个为真则意味着客户端与服务端连接出问题，需要进行重连
                    waitReconnect = true;
                    throw new NacosException(NacosException.CLIENT_DISCONNECT,
                            "Client not connected, current status:" + rpcClientStatus.get());
                }
                
                
                //在这里把请求发送给了服务器，并得到了响应
                response = this.currentConnection.request(request, timeoutMills);
                
                
                //下面是一些响应失败要执行的操作
                if (response == null) {
                    throw new NacosException(SERVER_ERROR, "Unknown Exception.");
                }
                if (response instanceof ErrorResponse) {
                    if (response.getErrorCode() == NacosException.UN_REGISTER) {
                        synchronized (this) {
                            waitReconnect = true;
                            if (rpcClientStatus.compareAndSet(RpcClientStatus.RUNNING, RpcClientStatus.UNHEALTHY)) {
                                LoggerUtils.printIfErrorEnabled(LOGGER,
                                        "Connection is unregistered, switch server, connectionId = {}, request = {}",
                                        currentConnection.getConnectionId(), request.getClass().getSimpleName());
                                //这里是异步切换服务地址重新建立连接的操作
                                //switchServerAsync();
                            }
                        }
                    }
                    throw new NacosException(response.getErrorCode(), response.getMessage());
                }
                
                
                //更新客户端最新接收到服务端消息的时间戳
                lastActiveTimeStamp = System.currentTimeMillis();
               
                //走到这里意味着响应没有问题，直接退出当前方法即可
                return response;
            } catch (Throwable e) {
                
                //走到这里说明上面抛出异常了，这意味着客户端与服务端连接有问题，需要重新等待客户端连接服务端
                if (waitReconnect) {
                    try {
                        //让线程睡一会，但是在第一版本代码中，客户端重连服务端的功能我还没有为大家实现，下一版本才会为大家实现
                        //是在定时任务中执行的
                        Thread.sleep(Math.min(100, timeoutMills / 3));
                    } catch (Exception exception) {
                    }
                }
                LoggerUtils.printIfErrorEnabled(LOGGER,"Send request fail, request = {}, retryTimes = {}, errorMessage = {}", request, retryTimes,e.getMessage());
                exceptionThrow = e;
            }
            //自增请求重试次数
            retryTimes++;
        }
       
        
        //走到这里意味着连接出问题了，这时候就要把客户端状态更新为不健康状态，等待客户端异步连接服务器成功
        if (rpcClientStatus.compareAndSet(RpcClientStatus.RUNNING, RpcClientStatus.UNHEALTHY)) {
            //这个异步连接服务器的功能也放在后面实现了
            //switchServerAsyncOnRequestFail();
        }
        if (exceptionThrow != null) {
            throw (exceptionThrow instanceof NacosException) ? (NacosException) exceptionThrow
            : new NacosException(SERVER_ERROR, exceptionThrow);
        } else {
            throw new NacosException(SERVER_ERROR, "Request fail, unknown Error");
        }
    }





    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：健康检查的方法，其实就可以把这个当成nacos中的心跳检测
     */ 
    private boolean healthCheck() {
        //创建一个健康检查请求
        HealthCheckRequest healthCheckRequest = new HealthCheckRequest();
        //判断当前客户端和服务端的连接是否为空
        if (this.currentConnection == null) {
            return false;
        }
        //从配置对象中获得健康检查的重试次数
        int reTryTimes = rpcClientConfig.healthCheckRetryTimes();
        //创建一个随机数生成器
        Random random = new Random();
        //在循环中进行健康检查，直到重试次数耗尽
        while (reTryTimes >= 0) {
            reTryTimes--;
            try {
                //随机睡一会，这里是因为如果上一次健康检查失败了，然后紧接着就开始下一次健康检查
                //不给客户端和服务端反应时间，那么得到的结果可能依然是失败
                //也许睡得这一会，客户端和服务端的连接就恢复了，或者网络环境也不波动了等等
                if (reTryTimes > 1) {
                    Thread.sleep(random.nextInt(500));
                }
                //向服务端发送健康检查请求
                Response response = this.currentConnection.request(healthCheckRequest, rpcClientConfig.healthCheckTimeOut());
                //这是我自己加的一段代码，用于大家在控制台观察结果
                if (response instanceof HealthCheckResponse){
                    System.out.println("健康检查成功！！！！！");
                }
                //响应成功则返回true
                return response != null && response.isSuccess();
            } catch (Exception e) {
                // ignore
            }
        }
        //重试完所有次数之后仍然没有返回成功响应，直接返回false即可
        return false;
    }






    //省略其他内容
    
    
}
```

可以看到，在上面的代码块中，我定义了一个 healthCheck 方法，healthCheck 方法的逻辑非常简单，就是使用 currentConnection 连接对象向注册中心服务端发送一个 HealthCheckRequest 健康检查请求，如果收到 HealthCheckResponse 健康检查的成功响应，就意味着客户端和服务端之间的连接没有问题，方法返回 true 即可，代表健康检查成功；如果重试几次都没有收到 HealthCheckResponse 响应，就意味着健康检查失败，返回 false 即可。

healthCheck 方法本身的逻辑就是这么简单，我相信每一个朋友都能看懂，但是该方法在哪里才会被调用呢？这才是我们接下来要考虑的重点，并且我在前面给大家分析客户端健康检测实现的原理时就说了，**客户端会开启一个定时任务，在定时任务中定期执行健康检查操作，这样才能及时发现客户端与服务端的连接是否出现问题，以及通过发送 HealthCheckRequest 请求，定期刷新存放在服务端的连接最新活跃时间**。但在上面的代码块中我只给大家展示了 healthCheck 方法，并没有展示 healthCheck 方法被调用了时机。所以接下来，就让我们一起来探讨一下，看看这个 healthCheck 方法会在哪里被调用。

## 完善 RpcClient 组件，实现客户端到服务端的连接重建功能

在开始讲解之前，首先我想先纠正一个概念，大家应该都看到了，本章标题为：完善 RpcClient 类，实现客户端心跳检测、断线重连功能。这里面提到了一个断线重连功能，心跳检测的方法我们已经实现了，接下来就该实现断线重连功能了。这个断线重连功能也正是我要纠正的功能，实际上，**不应该称其为断线重连功能，而应该称为连接重建功能**。 在我们的理解中，所谓断线重连，肯定是处于某种原因，客户端和服务端连接断开了，要想让客户端和服务端恢复通信，就应该让客户端重新连接服务端。这就是我们理解的短线重现，是发生在相同的客户端与服务端之间的操作。但是在 nacos 中，为服务端引入了集群这个概念，并且还引入了一个 ServerListFactory 服务地址管理器，这个地址管理器管理者服务端集群中所有服务器的地址。**假如 nacos 客户端首先和集群中的某个服务器建立了连接，但是出于某种原因，连接断开了，因为 nacos 服务器搭建了集群，这个时候，nacos 客户端就会从 ServerListFactory 服务地址管理器中得到一个新的服务地址，和新的服务器建立连接，并不是和原来的服务器进行断线重连操作**。所以我才把这个操作称为连接重建，而不是断线重连，**所谓连接重建，就是和一个新的服务器创建一个新的连接**，这个概念大家要纠正一下。

我在这里突然解释连接重建功能的概念是有原因的，和上一小节我们实现的 healthCheck 方法也有联系。请大家思考一下，假如客户端和服务端的连接断开了，还需要执行健康检查操作吗？显然就没要执行了是吧？健康检查操作的目的是检查连接存货情况，现在连接都断开了，还检查什么啊。换句话说，**只有在客户端和服务端连接正常建立，并且存活的情况下，才需要执行健康检查操作，判断连接的状态是否依然存活，并且定期刷新服务端那边的连接最新活跃时间；也就是说，只有客户端和服务端连接正常时，才可以在定时任务中定期调用 healthCheck 方法**。那程序怎么就知道客户端和服务端的连接是正常的呢？这个问题可能会让大家有些发懵，什么叫程序怎么知道客户端和服务端连接是否正常？在没有意外的情况下，难道客户端和服务端连接不是一直正常吗？如果这个问题有些绕脑筋，那我就换一种问法，**程序怎么就知道目前客户端和服务端的连接是不正常的呢**？如果程序知道客户端和服务端的连接是不正常的，那就可以不用再执行定义任务中的健康检测方法了。这样就容易理解了吧？那程序怎么就知道客户端和服务端连接异常了呢？

我的做法非常简单，首先请大家思考一下，程序执行的过程中，什么时候会触发连接异常的情况？就我们目前实现的注册中心客户端来说，有两个地方会触发连接异常的情况：**第一种就是客户端刚启动的时候，客户端从 ServerListFactory 服务地址管理器中得到了一个服务地址，要和对应的服务器建立连接了，结果连接建立失败，这个时候，就会发生连接异常的情况。按照常规操作，这个时候需要从 ServerListFactory 服务地址管理器中获得一个新的服务器地址，然后让客户端和它建立连接。这个功能在上一章并没有为大家实现；第二种情况就是客户端向服务器发送请求消息时，消息发送失败，重试了几次后依然失败并且抛出了 NacosException 连接异常对象，这个时候程序也会从 ServerListFactory 中得到一个新的服务器地址，然后建立新的连接**。

以上出现连接异常的两种情况大家应该都能理解，**并且当出现连接异常之后，程序都会选择从 ServerListFactory 中再获得一个新的服务器地址，和这个地址对应的服务器重新建立连接**。接下来我想执行一个重要的操作：那就是我像定义一个新的类，就是 ReconnectContext 类，意思是重连上下文。这个类的具体实现如下，请看下面代码块。

```
//这个类其实是RpcClient的内部类
//该类的对象就是重连上下文对象
static class ReconnectContext {

    //构造方法
    public ReconnectContext(ServerInfo serverInfo, boolean onRequestFail) {
        this.onRequestFail = onRequestFail;
        this.serverInfo = serverInfo;
    }

    //这个成员变量标识本次重连是不是因为请求发送失败而触发的
    boolean onRequestFail;

    //需要重连的新的服务端地址信息
    ServerInfo serverInfo;

}
```

当程序中出现连接异常的情况，**我就创建一个 ReconnectContext 对象，如果是因为发送请求出现的连接异常情况，就把 ReconnectContext 对象的 onRequestFail 设置为 true，否则就设置为 false；而 serverInfo 成员变量就封装着一个新的服务器地址，客户端就是要和这个新的服务器地址建立连接**。到此为止，这个操作看起来还没什么作用，那紧接着，我**再在 RpcClient 类中定义一个 reconnectionSignal 队列，这个队列专门存放 ReconnectContext 重连上下文对象，只要程序中出现了连接异常的情况，我创建了 ReconnectContext 对象，然后就把 ReconnectContext 对象存放到 reconnectionSignal 队列中。这样一来，我就可以在执行健康检查的定时任务中首先判断这个 reconnectionSignal 队列中是否存在数据，如果存在数据就意味着当前客户端和服务端的连接不健康，也就不必执行什么健康检测了，直接执行客户端和新的服务端连接重建的操作；如果 reconnectionSignal 队列中不存在数据，就意味着客户端和服务端连接可能正常，那就执行健康检测操作，同时也能刷新服务端那边连接的最新时间**。大家可以仔细品味品味这个逻辑，如果掌握了这个逻辑，接下来就看看我重构之后的 RpcClient 类。

实际上，根据上面的分析，我还可以再给 RpcClient 类定义三个新的方法，**比如说把 ReconnectContext 对象添加到 reconnectionSignal 队列的方法，我可以定义为 switchServerAsync 方法**，意思为异步连接新的服务器的方法；当然，**如果是因为发送请求时发现连接异常了，那我可以再定义一个 switchServerAsyncOnRequestFail 方法**，这个方法和 switchServerAsync 方法一样，只不过会在发送请求连接异常时被调用；**最后我还要定义一个 reconnect 方法，这个方法就是客户端和新的服务器建立连接的方法**。这几个新的方法定义完毕之后，接下来就轮到健康检查定时任务登场了，**那我自然要给 RpcClient 类再定义一个新的成员变量，就是 clientEventExecutor 定时任务执行器，这个执行器就负责定期执行健康检查方法**。其实我刚才也已经为大家分析过这个定时任务的逻辑了，**在定时任务中首先判断 reconnectionSignal 队列中是否存在数据，如果存在数据意味着连接已经异常了，直接执行 reconnect 方法即可，让客户端和新的服务器建立连接；如果 reconnectionSignal 队列中不存在数据，这时候再执行 healthCheck 方法，检查客户端和服务端连接是否正常，如果 healthCheck 方法返回 true，则意味着连接正常；如果返回 false，则意味心跳检测失败，客户端与服务端连接出现问题了，这时候直接创建一个 ReconnectContext 对象，然后执行 reconnect 方法，让客户端和新的服务器建立连接即可**。**而向 clientEventExecutor 定时任务执行器中提交定时任务的操作，就被我定义在了 RpcClient 类的 start 方法中，也就是说，客户端一启动，健康检查定时任务也随之启动了**。接下来，就请大家看一看我重构后的 RpcClient 类，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/11
 * @方法描述：客户端的顶级抽象父类，这个类中定义了一些公共方法，让http客户端和grpc客户端实现类去实现
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {


    //naocs服务器地址管理器
    private ServerListFactory serverListFactory;

    //源自引用类表示客户端的状态，客户端状态发生变更时可以安全更新
    protected volatile AtomicReference<RpcClientStatus> rpcClientStatus = new AtomicReference<>(RpcClientStatus.WAIT_INIT);

    //表示和服务器构建的连接对象
    protected volatile Connection currentConnection;

    //最新的收到服务器消息的时间戳
    private long lastActiveTimeStamp = System.currentTimeMillis();

    //rpc客户端的配置类
    protected RpcClientConfig rpcClientConfig;

    //这个集合存放了用于处理服务器发送的请求的处理器
    //服务器可能会向客户端主动发送各种各样的请求，所以就在客户端定义了一些处理器，专门用于处理对应的服务器发送过来的请求
    protected List<ServerRequestHandler> serverRequestHandlers = new ArrayList<>();


    //客户端事件执行器，这个执行器的作用非常大，在第二版本代码中，健康检查，客户端与服务端重连都是这个执行器执行的
    protected ScheduledExecutorService clientEventExecutor;

    //这个阻塞队列存放的都是重新上下文对象，因为网络环境是波动的，客户端和服务端的连接可能会失败，或者服务端主动让客户端更换连接地址，这些情况下
    //就会创建一个ReconnectContext重新上下文对象，这个上下文对象中封装着一个新的服务器地址，客户端只需要重新连接这个服务器即可
    private final BlockingQueue<ReconnectContext> reconnectionSignal = new ArrayBlockingQueue<>(1);



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


        //接下来就是创建clientEventExecutor定时任务执行器的逻辑
         
        //在这里创建了客户端事件执行器对象，这里定义了两个线程，但在第二版本代码中只会用到一个线程，第三版本就会用到第二个线程了
        clientEventExecutor = new ScheduledThreadPoolExecutor(2, r -> {
            Thread t = new Thread(r);
            t.setName("com.alibaba.nacos.client.remote.worker");
            t.setDaemon(true);
            return t;
        });




        //接下来就是向定时任务执行器中添加健康检查定时任务方法的操作




        //开始向执行器提交健康检查定时任务
        clientEventExecutor.submit(() -> {
            
            
            //注意，这个任务会在一个循环中一直被执行，除非执行过程中出错，从而退出循环，这个时候任务才会终止
            while (true) {
                try {
                    //检查一下客户端是否已经关闭了，如果关闭了也退出循环
                    if (isShutdown()) {
                        break;
                    }
                    
                    //看到这里，大家应该就意识到，这个任务其实就是重连任务，也就是说，在这个循环中，程序会一直判断是否需要重新连接服务端
                    //在这里从重连阻塞队列中获取一个重连上下文对象，并且是有限时的获取
                    ReconnectContext reconnectContext = reconnectionSignal.poll(rpcClientConfig.connectionKeepAlive(), TimeUnit.MILLISECONDS);
                    
                    
                    //如果重连上下文对象为null，说明连接正常，客户端和服务端目前不需要重新连接，直接执行健康检查操作就行
                    if (reconnectContext == null) {
                        
                        //如果不需要重新连接，就进一步判断一下客户端这边的活跃时间戳是否应该更新了，也就是是否应该进行健康检查了
                        //如果超过了规定的存活时间，就进行健康检查，这个健康检查就可以看作心跳检测
                        if (System.currentTimeMillis() - lastActiveTimeStamp >= rpcClientConfig.connectionKeepAlive()) {
                            
                            //这是我自己添加的一段代码，用于大家在控制台观察结果
                            System.out.println("健康检查执行了！！！！！");
                            
                            //在这里进行健康检查，调用了healthCheck()方法
                            boolean isHealthy = healthCheck();
                            
                            //然后判断健康检查是否成功了
                            if (!isHealthy) {
                                //走到这里意味着健康检查失败了，但这个时候还要再判断一下当前客户端和服务端的连接是否为null
                                //如果为null，说明客户端还没有和服务端建立连接呢，也许程序刚刚启动，这个时候从reconnectionSignal重连队列中肯定获得的是null
                                //健康检查也肯定会失败，所以当currentConnection为null的时候，就跳过本次循环，等待客户端和服务端连接成功了，才执行正式检测流程
                                //这里大家可能会有疑问，为什么客户端和服务端还没有链接呢，就开始检查呢，别忘了，这是个异步执行的任务，当这个任务开始执行的时候，也许客户端正在连接服务端
                                if (currentConnection == null) {
                                    continue;
                                }
                                
                                //走到这里意味着健康检查失败，并且客户端和服务端有连接了，这时候先记录错误日志
                                LoggerUtils.printIfInfoEnabled(LOGGER,"[{}] Server healthy check fail, currentConnection = {}",rpcClientConfig.name(), currentConnection.getConnectionId());
                                
                                //得到客户端当前的运行状态
                                RpcClientStatus rpcClientStatus = RpcClient.this.rpcClientStatus.get();
                                
                                //如果客户端已经关闭，就直接退出循环即可
                                if (RpcClientStatus.SHUTDOWN.equals(rpcClientStatus)) {
                                    break;
                                }
                               
                                //走到这里意味着客户端没有关闭，但是健康检查失败了，于是把客户端状态更新为不健康状态
                                boolean statusFLowSuccess = RpcClient.this.rpcClientStatus.compareAndSet(rpcClientStatus, RpcClientStatus.UNHEALTHY);
                                
                                //更新状态成功后
                                if (statusFLowSuccess) {
                                    //在这里创建了一个重连上下文对象，用于后续客户端和服务端重连
                                    reconnectContext = new ReconnectContext(null, false);
                                } else {//走到这里意味着刚才客户端状态没有更新成功，这里就直接进入下一次循环，在下一次循环中再次判断然后更新
                                    continue;
                                }
                            } else {
                                
                                //走到这里意味着健康检查成功了，那就直接更新客户端最新的活跃时间即可
                                lastActiveTimeStamp = System.currentTimeMillis();
                                //进入下一次循环
                                continue;
                            }
                        } else {//走到这里意味着客户端最新的活跃时间还没过期，直接进入下一次循环
                            continue;
                        }
                    }


                    //下面就是重新建立连接的操作


                    //程序走到这里分为两种情况，第一种是从reconnectionSignal队列中得到的reconnectContext对象不为null，那么程序就会直接执行到这里
                    //还有一种就是从reconnectionSignal队列中得到的对象为null，但是健康检查失败，仍然要进行重连操作
                    //在这里执行重新连接服务端的操作
                    reconnect(reconnectContext.serverInfo, reconnectContext.onRequestFail);
                } catch (Throwable throwable) {

                }
            }
        });

        
        
        
        //定义一个连接对象，用来接收客户端到服务端的连接
        Connection connectToServer = null;
        //更新客户端状态为开始状态
        rpcClientStatus.set(RpcClientStatus.STARTING);
        
        
        //获取连接重试次数
        int startUpRetryTimes = rpcClientConfig.retryTimes();
        //在循环中开始连接服务端，循环次数就为连接重试次数
        while (startUpRetryTimes >= 0 && connectToServer == null) {
            try {
                //连接失败一次，则次数减1
                startUpRetryTimes--;
                
                //因为上一次连接失败了，肯定是nacos服务端有问题，所以这里从服务地址管理器中获得一个新的服务端地址
                //ServerInfo是一个内部类，里面封装nacos服务器地址信息
                ServerInfo serverInfo = nextRpcServer();
                
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Try to connect to server on start up, server: {}",rpcClientConfig.name(), serverInfo);
                
                //连接服务器，连接成功则返回一个连接对象
                connectToServer = connectToServer(serverInfo);
            } catch (Throwable e) {
                
                LoggerUtils.printIfWarnEnabled(LOGGER,
                        "[{}] Fail to connect to server on start up, error message = {}, start up retry times left: {}",
                        rpcClientConfig.name(), e.getMessage(), startUpRetryTimes, e);
            }
        }
        
        //下面是连接成功的逻辑
        
        if (connectToServer != null) {
            LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Success to connect to server [{}] on start up, connectionId = {}",
                    rpcClientConfig.name(), connectToServer.serverInfo.getAddress(),
                    connectToServer.getConnectionId());
            
            //走到这里意味着连接成功，给currentConnection赋值即可
            this.currentConnection = connectToServer;
            
            //更新客户端运行状态
            rpcClientStatus.set(RpcClientStatus.RUNNING);
        }


        //下面就是连接失败的操作，调用switchServerAsync方法，向reconnectionSignal队列中添加了一个ReconnectContext对象
        else {
            //走到这里意味着客户端连接服务端失败了，那就切换成异步去连接服务端
            //所谓异步连接客户端，其实就是向reconnectionSignal重连队列中添加了一个new ReconnectContext(recommendServerInfo, onRequestFail)对象
            //这个时候就和刚才clientEventExecutor执行器执行的异步任务串起来了，大家可以再品味一下这个逻辑
            switchServerAsync();
        }
    }



    //该方法就是异步连接新的服务器的入口方法
    public void switchServerAsync() {
        //这里大家肯定会对这个null感到疑惑，因为要创建ReconnectContext对象了
        //这里传进去null，那么ReconnectContext对象的serverInfo成员变量不就为null了
        //没有新的服务器的地址，客户端怎么向新的服务端建立连接啊？大家可以先保留这个疑问，很快我就会为大家解释
        switchServerAsync(null, false);
    }




    //该方法就是异步连接服务器的方法。其实就是向重连队列中添加了一个重连上下文对象
    //然后让客户端事件执行器去异步连接服务器
    protected void switchServerAsync(final ServerInfo recommendServerInfo, boolean onRequestFail) {
        reconnectionSignal.offer(new ReconnectContext(recommendServerInfo, onRequestFail));
    }


    


    //该方法会在请求发送失败时，异步连接另一个服务器
    public void switchServerAsyncOnRequestFail() {
        switchServerAsync(null, true);
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
   




    
    


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：客户端向服务端发送请求的方法
     */
    public Response request(Request request) throws NacosException {
        //超时时间，如果用户没有定义，那么默认为3秒，这个3秒默认时间是客户端配置类中定义好的
        return request(request, rpcClientConfig.timeOutMills());
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：真正向服务端发送请求的方法
     */
    public Response request(Request request, long timeoutMills) throws NacosException {
        
        //重试次数初始为0
        int retryTimes = 0;
        //接收响应对象
        Response response;
        //接收异常信息
        Throwable exceptionThrow = null;
        //得到操作的开始时间
        long start = System.currentTimeMillis();
        
        
        //在循环中发送请求到服务器，这里之所以是在循环中执行操作，是因为请求发送有可能失败
        //这时候就要重新发送请求，所以就要在循环中判断是否超过了规定的请求重试次数
        //以及是否超过了请求超时时间，这两个都没有超过才能继续重新发送请求
        while (retryTimes <= rpcClientConfig.retryTimes() && (timeoutMills <= 0 || System.currentTimeMillis() < timeoutMills + start)) {
            
            //定义一个是否等待重新连接服务器的标志
            boolean waitReconnect = false;
            
            try {
                //判断当前客户端是否处于运行状态，对当前连接判空
                if (this.currentConnection == null || !isRunning()) {
                    //有一个为真则意味着客户端与服务端连接出问题，需要进行重连
                    waitReconnect = true;
                    throw new NacosException(NacosException.CLIENT_DISCONNECT,
                            "Client not connected, current status:" + rpcClientStatus.get());
                }
                
                
                //在这里把请求发送给了服务器，并得到了响应
                response = this.currentConnection.request(request, timeoutMills);
                
                
                //下面是一些响应失败要执行的操作
                if (response == null) {
                    throw new NacosException(SERVER_ERROR, "Unknown Exception.");
                }
                if (response instanceof ErrorResponse) {
                    if (response.getErrorCode() == NacosException.UN_REGISTER) {
                        synchronized (this) {
                            waitReconnect = true;
                            if (rpcClientStatus.compareAndSet(RpcClientStatus.RUNNING, RpcClientStatus.UNHEALTHY)) {
                                LoggerUtils.printIfErrorEnabled(LOGGER,
                                        "Connection is unregistered, switch server, connectionId = {}, request = {}",
                                        currentConnection.getConnectionId(), request.getClass().getSimpleName());
                                //这里是异步切换服务地址重新建立连接的操作
                                //switchServerAsync();
                            }
                        }
                    }
                    throw new NacosException(response.getErrorCode(), response.getMessage());
                }
                
                
                //更新客户端最新接收到服务端消息的时间戳
                lastActiveTimeStamp = System.currentTimeMillis();
               
                //走到这里意味着响应没有问题，直接退出当前方法即可
                return response;
            } catch (Throwable e) {
                
                //走到这里说明上面抛出异常了，这意味着客户端与服务端连接有问题，需要重新等待客户端连接服务端
                if (waitReconnect) {
                    try {
                        //让线程睡一会，但是在第一版本代码中，客户端重连服务端的功能我还没有为大家实现，下一版本才会为大家实现
                        //是在定时任务中执行的
                        Thread.sleep(Math.min(100, timeoutMills / 3));
                    } catch (Exception exception) {
                    }
                }
                LoggerUtils.printIfErrorEnabled(LOGGER,"Send request fail, request = {}, retryTimes = {}, errorMessage = {}", request, retryTimes,e.getMessage());
                exceptionThrow = e;
            }
            //自增请求重试次数
            retryTimes++;
        }
       
        
        //走到这里意味着连接出问题了，这时候就要把客户端状态更新为不健康状态，等待客户端异步连接服务器成功
        if (rpcClientStatus.compareAndSet(RpcClientStatus.RUNNING, RpcClientStatus.UNHEALTHY)) {
            //这个异步连接服务器的功能也可以使用了
            switchServerAsyncOnRequestFail();
        }
        if (exceptionThrow != null) {
            throw (exceptionThrow instanceof NacosException) ? (NacosException) exceptionThrow
            : new NacosException(SERVER_ERROR, exceptionThrow);
        } else {
            throw new NacosException(SERVER_ERROR, "Request fail, unknown Error");
        }
    }





    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：健康检查的方法，其实就可以把这个当成nacos中的心跳检测
     */ 
    private boolean healthCheck() {
        //创建一个健康检查请求
        HealthCheckRequest healthCheckRequest = new HealthCheckRequest();
        //判断当前客户端和服务端的连接是否为空
        if (this.currentConnection == null) {
            return false;
        }
        //从配置对象中获得健康检查的重试次数
        int reTryTimes = rpcClientConfig.healthCheckRetryTimes();
        //创建一个随机数生成器
        Random random = new Random();
        //在循环中进行健康检查，直到重试次数耗尽
        while (reTryTimes >= 0) {
            reTryTimes--;
            try {
                //随机睡一会，这里是因为如果上一次健康检查失败了，然后紧接着就开始下一次健康检查
                //不给客户端和服务端反应时间，那么得到的结果可能依然是失败
                //也许睡得这一会，客户端和服务端的连接就恢复了，或者网络环境也不波动了等等
                if (reTryTimes > 1) {
                    Thread.sleep(random.nextInt(500));
                }
                //向服务端发送健康检查请求
                Response response = this.currentConnection.request(healthCheckRequest, rpcClientConfig.healthCheckTimeOut());
                //这是我自己加的一段代码，用于大家在控制台观察结果
                if (response instanceof HealthCheckResponse){
                    System.out.println("健康检查成功！！！！！");
                }
                //响应成功则返回true
                return response != null && response.isSuccess();
            } catch (Exception e) {
                // ignore
            }
        }
        //重试完所有次数之后仍然没有返回成功响应，直接返回false即可
        return false;
    }




     //该类的对象就是重连上下文对象
    static class ReconnectContext {


        public ReconnectContext(ServerInfo serverInfo, boolean onRequestFail) {
            this.onRequestFail = onRequestFail;
            this.serverInfo = serverInfo;
        }

        //这个成员变量标识本次重连是不是因为请求发送失败而触发的
        boolean onRequestFail;

        //需要重连的新的服务端地址信息
        ServerInfo serverInfo;

    }



    //省略其他内容
    
    
}
```

到此为止，我就把 RpcClient 类重构完毕了，上面代码中的注释非常详细，并且在重构 RpcClient 类之前，我也为大家分析过程序重构的逻辑，以及各个新定义的方法逻辑，所以我就不再对上面代码进行重复讲解了，大家可以多看几遍上面的代码块。其实本章的内容到这里就结束了，但是我能想到在上面的代码块中，一定给大家留下了一个疑问，那就是上面代码块的 switchServerAsync 方法，请看下面代码块。

```
//该方法就是异步连接新的服务器的入口方法
public void switchServerAsync() {
    //这里大家肯定会对这个null感到疑惑，因为要创建ReconnectContext对象了
    //这里传进去null，那么ReconnectContext对象的serverInfo成员变量不就为null了
    //没有新的服务器的地址，客户端怎么向新的服务端建立连接啊？大家可以先保留这个疑问，很快我就会为大家解释
    switchServerAsync(null, false);
}
```

在注释中我也把大家可能的疑惑点写出来了，那就是给 ReconnectContext 对象的 serverInfo 成员变量赋值时，为什么要使用 null 来赋值？并且既然这里已经定义为了 null，为什么又要在上面代码块的第 303 行执行这样一行代码： serverInfo = recommendServer.get() == null ? nextRpcServer() : recommendServer.get()？明明已经定义为 null 了，为什么又要判断 ReconnectContext 对象的 serverInfo 成员变量是否不为 null？

现在我来给大家解开这个疑惑，**实际上，在 nacos 注册中心的服务端可能会发生这样一种情况，那就是有多个客户端都和自己建立了连接，多个客户端把自己内部的服务实例信息都注册到了服务端，这种情况是很可能出现的。这个时候，注册中心的服务端就会定时查看自己的负载能力，如果服务端接收客户端的连接超过了一定阈值，就会主动向注册中心客户端发送一个 ConnectResetRequest 请求，也就是连接重置请求，这个请求封装了服务端发送给客户端的新的服务器地址。而注册中心客户端会使用 ConnectResetRequestHandler 请求处理器来处理这个 ConnectResetRequest 请求，在处理的过程中，会把服务器发送过来的新的服务器地址封装到一个 ServerInfo 对象中，然后会调用 switchServerAsync 方法，把 ServerInfo 对象传进去，这样一来就会创建一个 serverInfo 成员变量被赋值了的 ReconnectContext 对象，然后把 ReconnectContext 对象添加到 reconnectionSignal 队列中**。ConnectResetRequestHandler 处理器的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/11
 * @方法描述：客户端的顶级抽象父类，这个类中定义了一些公共方法，让http客户端和grpc客户端实现类去实现
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class RpcClient implements Closeable {


    //其他内容省略





    /** 
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：这个就是客户端处理服务端重连请求的处理器
     */ 
    class ConnectResetRequestHandler implements ServerRequestHandler {
        
        @Override
        public Response requestReply(Request request, Connection connection) {
            //判断接收到的请求是否为重新连接新的服务端请求
            if (request instanceof ConnectResetRequest) {
                try {//这里为什么加同步锁呢？我不理解。。
                    synchronized (RpcClient.this) {
                        //判断当前客户端是否处于运行状态
                        if (isRunning()) {
                            //类型转换为ConnectResetRequest
                            ConnectResetRequest connectResetRequest = (ConnectResetRequest) request;
                            //判断从服务端传输过来的服务地址是否不为空
                            if (StringUtils.isNotBlank(connectResetRequest.getServerIp())) {
                                //解析服务地址
                                ServerInfo serverInfo = resolveServerInfo(connectResetRequest.getServerIp() + Constants.COLON + connectResetRequest.getServerPort());
                                //异步连接服务，其实就是向重连队列中添加了一个重现上下文对象，让客户端事件执行器去异步连接了
                                switchServerAsync(serverInfo, false);
                            } else {
                                //如果没有传过来要连接的服务地址，也是直接让客户端事件执行器异步连接
                                //只不过这时候使用的就是客户端持有的服务地址管理器中的一个地址了
                                switchServerAsync();
                            }
                            //这个扩展点可以有用户自己定义逻辑
                            afterReset(connectResetRequest);
                        }
                    }
                } catch (Exception e) {
                    LoggerUtils.printIfErrorEnabled(LOGGER, "[{}] Switch server error, {}", rpcClientConfig.name(), e);
                }
                //返回给服务端响应
                return new ConnectResetResponse();
            }
            //如果不是期望的ConnectResetRequest类型，直接返回null
            return null;
        }
    }








    //其他内容省略
}






    //客户端处理服务端请求的操作在哪里大家应该清楚了吧？第一版本代码已经展示了，就在GrpcClient类中定义的请求观察者的onNext方法中
    //就是下面展示的这样
     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：这个方法中都是grpc代码的逻辑，还没有基础过grpc的朋友们可以去找找grpc的资料，简单来说，当grpc的服务端向客户端发送了请求之后，程序就会调用这个方法
     * 确切地说，是调用该方法内部的onNext方法，来处理服务端主动发送过来的请求
     */
    private StreamObserver<Payload> bindRequestStream(final BiRequestStreamGrpc.BiRequestStreamStub streamStub,
                                                      final GrpcConnection grpcConn) {

        return streamStub.requestBiStream(new StreamObserver<Payload>() {

            @Override
            public void onNext(Payload payload) {

                LoggerUtils.printIfDebugEnabled(LOGGER, "[{}]Stream server request receive, original info: {}",
                        grpcConn.getConnectionId(), payload.toString());
                try {
                    Object parseBody = GrpcUtils.parse(payload);
                    final Request request = (Request) parseBody;
                    
                    
                    if (request != null) {
                        try {//这里还会判断一下请求是否为SetupAckRequest类型的
                            if (request instanceof SetupAckRequest) {
                                //如果是就执行requestReply方法，大家别忘了，setupRequestHandler本身就是一个服务请求处理器
                                //在这个处理器的requestReply方法中，会执行recAbilityContext.release这个方法
                                //这个时候就会把服务器发送过来的能力信息告诉客户端了，并且结束线程阻塞，因为会执行CountDownLatch的countDown方法
                                //这里大家可能会有疑问，为什么之前在connectToServer方法中发送的是一个ConnectionSetupRequest请求，难道不应该接收到一个服务端的响应吗？
                                //为什么这里反而接收到了一个请求？这是因为在服务端那边就是这么处理的，服务端接收到ConnectionSetupRequest请求后，就会在服务端的GrpcBiStreamRequestAcceptor类中
                                //发送一个SetupAckRequest请求给客户端，作为ConnectionSetupRequest请求的响应
                                setupRequestHandler.requestReply(request, null);
                                //退出当前方法
                                return;
                            }


                            //下面就是根据请求查找具体的处理器，然后处理请求的操作
                            
                            //在这里处理了服务器发送过来的消息，交给服务请求处理器去匹配然后处理
                            Response response = handleServerRequest(request);
                            
                            
                            if (response != null) {
                                response.setRequestId(request.getRequestId());
                                //回复给服务器响应
                                sendResponse(response);
                            } else {
                                LOGGER.warn("[{}]Fail to process server request, ackId->{}", grpcConn.getConnectionId(),
                                        request.getRequestId());
                            }
                        } catch (Exception e) {
                            LoggerUtils.printIfErrorEnabled(LOGGER, "[{}]Handle server request exception: {}",
                                    grpcConn.getConnectionId(), payload.toString(), e.getMessage());
                            Response errResponse = ErrorResponse
                                    .build(NacosException.CLIENT_ERROR, "Handle server request error");
                            errResponse.setRequestId(request.getRequestId());
                            sendResponse(errResponse);
                        }

                    }
                } catch (Exception e) {
                    LoggerUtils.printIfErrorEnabled(LOGGER, "[{}]Error to process server push response: {}",
                            grpcConn.getConnectionId(), payload.getBody().getValue().toStringUtf8());
                    // remove and notify
                    recAbilityContext.release(null);
                }
            }  


             //该方法其他内容省略
        });
    }





    //下面这个方法其实定义在了RpcClient类中，这里只是为了逻辑连贯，所以也展示在这里了
    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/11
     * @方法描述：客户端处理服务器主动发送给过来的请求的方法
     */
    protected Response handleServerRequest(final Request request) {
        LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Receive server push request, request = {}, requestId = {}",
                rpcClientConfig.name(), request.getClass().getSimpleName(), request.getRequestId());
        //更新客户端最新一次接收到服务器消息的时间戳
        lastActiveTimeStamp = System.currentTimeMillis();
        //遍历服务请求处理器
        //serverRequestHandlers是RpcClient类中的成员变量，是一个集合，存放的都是请求处理器
        for (ServerRequestHandler serverRequestHandler : serverRequestHandlers) {
            try {
                //把接收到的请求交给每一个服务请求处理器去处理，当然，大家不要以为每一个服务请求处理器都会处理这个请求
                //实际上在每一额处理器中，都会先判断这个歌请求类型是否匹配自己可以处理的类型，不匹配则直接退出了
                //所以最后还是对用的处理器处理对应的请求
                Response response = serverRequestHandler.requestReply(request, currentConnection);
                if (response != null) {
                    LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] Ack server push request, request = {}, requestId = {}",
                            rpcClientConfig.name(), request.getClass().getSimpleName(), request.getRequestId());
                    return response;
                }
            } catch (Exception e) {
                LoggerUtils.printIfInfoEnabled(LOGGER, "[{}] HandleServerRequest:{}, errorMessage = {}",
                        rpcClientConfig.name(), serverRequestHandler.getClass().getName(), e.getMessage());
                throw e;
            }
        }
        return null;
    }


           
```

ConnectResetRequestHandler 处理器中的操作有一个好处，**那就是客户端只凭借 ReconnectContext 对象的 serverInfo 成员变量有没有被赋值，就可以知道目前客户端和新的服务地址建立连接时，究竟是自己主动建立的连接，还是被服务端通知了，强制更新连接**。**如果客户端发现自己是被服务端通知了，强制更新连接，这个时候客户端就要判断一下服务端发送过来的新的服务器地址是否在 serverListFactory 服务地址管理器中，如果存在就可以直接连接，如果不存在就直接报错。因为 serverListFactory 服务地址管理器中存放了服务器集群中所有服务器的地址，服务端发送过来的新的地址不在集群中，贸然连接肯定就出问题了**。这就是为什么要在 switchServerAsync() 方法内部使用 null 给 ReconnectContext 对象的 serverInfo 成员变量赋值的根本原因。这个功能我就不在文章中具体展开了，大家可以直接去 RpcClient 类中查看，这一章看完，大家已经可以去看第二版本的代码了。其他方面的细节，大家也可以从第二版本代码中查看，就比如客户端判断服务端发送过来的地址是否在 serverListFactory 服务地址管理器中的操作，就都留给大家去代码中查看吧。总的来说，这一章的内容非常简单，虽然代码多一点，但基本上都是上一章讲解过的，新知识并不多。好了朋友们，这一章就到此结束了，我们下一章见！