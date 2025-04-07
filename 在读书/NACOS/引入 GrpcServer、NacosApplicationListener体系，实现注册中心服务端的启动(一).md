这一章就正式进入到 nacos 注册中心服务端的知识模块了。虽说我们接下来要实现的是注册中心服务端，但是我希望大家不要把这个注册中心服务端的功能和配置中心服务端割裂开。实际上，在我们实现注册中心服务端的过程中，有一些配置中心功能也会被我们实现。说得确切一点，我们实现的某些功能，nacos 的配置中心也会用到。所以，我现在其实可以很负责任地和大家说一句，当我们把注册中心服务端的全部核心功能都实现了，等到要实现配置中心的功能时，就会简单很多了。因为配置中心的脉络非常清晰，它主要和 web 控制台交互，所有方法调用和功能实现的源头都很清楚，直接从源头入手，功能逻辑就非常容易分析了。好了，我又有点扯远了，接下来还是让我们尽快回到 nacos 注册中心服务端的功能实现上来吧。我会用很多篇文章为大家详细剖析、实现注册中心服务端，当然，大家不要觉得内容多，就认为这些功能实现起来比较困难。对于这一点，我也可以很负责任地告诉大家，**nacos 最多两星，内容多仅仅是因为功能多，并不是因为技术难**。也许我说得再多大家可能也不会完全相信，所以 nacos 的具体难度，就交给大家自己体会吧。接下来，我们就正式开始本章内容吧。

  

## 引入 BaseRpcServer 类，构建 Grpc 服务端体系

既然现在要实现 nacos 注册中心服务端，**首先要实现的就是服务端最基础的功能，那就是服务端接收客户端连接功能**。这一点大家应该也都想到了，因为在我们实现 nacos 注册中心客户端功能的时候，首先就是构建了客户端本身，然后实现了客户端访问服务端，建立客户端和服务端之间的连接的功能。只有当客户端和服务端的连接建立了，客户端和服务端才能正常通信，之后的所有功能，比如：健康检测，注册服务实例，订阅服务实例信息等等功能才能实现。而我们搭建 nacos 注册中心客户端的时候也并没有进行特别复杂的编码，因为 nacos 内部使用了 grpc-java 框架作为通信组件，所以我们就相当于直接启动了 grpc 的客户端，然后围绕着这个 grpc 的客户端为 nacos 注册中心定义了一些网络通信的功能。比如实现了 nacos 注册中心的连接重建功能、定义了各种请求对象和响应对象、定义了同步发送请求和异步发送请求的方法等等。这些都是旧知识了，我在这里也只是帮助大家简单回顾一下，并不会再花费很长的篇幅讲解它们了。大家如果忘记前面章节的内容了，可以再回顾回顾之前的内容。总之，说了这么多，主要是为了体现出我们本章的重点内容，那就是为了和我们实现注册中心客户端功能时的步骤吻合，这一章我会先为大家实现注册中心的服务端。当然，**最终实现的功能就是编写 grpc-java 服务端，然后启动这个服务端，让这个服务端能够接收注册中心客户端连接就行了**。

接下来要做的就很简单了，无非就是编写一下 grpc 的服务端，只要启动程序，服务端就可以直接接收客户端的连接。当然，说到启动程序，我还要多说一句，在源码中，nacos 服务端的核心代码，核心类几乎都在 core 模块下，这个模块又被其他的模块依赖了，比如说 console 这个模块就依赖了 core 模块；console 其实就是控制台模块，也就是前端 web 页面。当我们手动编译了 nacos 源码，然后打算启动服务端时，一般来说都会直接启动 console 这个模块下的启动类，也就是 springboot 的启动类，随着 springboot 的启动，nacos 控制台的 web 服务器以及注册中心服务端就都启动了。web 服务器端口号为 8848，这个服务器主要是和控制台交互；而我们本章要实现得注册中心服务端，主要是和 nacos 注册中心客户端，也就是 sdk 客户端交互。大家还是要把不同模块的作用梳理清楚。当然，我想就算我不说，大家肯定也都清楚这些内容，毕竟大家都是开发高手了，而我之所以提起这些，是因为我想告诉大家，**nacos 服务端深度依赖了 springboot，用到了大量的 springboot 的注解，以及 springboot 的监听器和其他扩展接口**，这些内容在我为大家实现服务端的过程中，都会详细地展示出来。

好了，让我们言归正传吧。想必大家都还记得，在实现 nacos 客户端的时候，我们引入了一系列的客户端类，比如 RpcClient、GrpcClient、以及 GrpcSdkClient 类。当然，在 nacos 源码中实际上定义了两种通信方式，一种是 http 通信方式，另一种就是 rpc 通信方式。因为 http 通信是比较旧的 nacos 版本采用的方式，新版本 nacos 客户端使用的是 rpc 和服务端进行通信，所以在我为大家提供的代码中，我就把 http 通信组件给省略了；最后，在 rpc 通信组件中，大家可以看到 RpcClient、GrpcClient、以及 GrpcSdkClient 这三个核心类。这三个类的关系也很简答， **RpcClient 是 GrpcClient 的父类，GrpcClient 是 GrpcSdkClient 的父类**。其中 RpcClient 更像是 nacos 自己客户端的一个实现，我这么说倒不是想把 RpcClient 和 GrpcClient、GrpcSdkClient 这两个类分隔开，我的意思是从功能、或者说职责上来说，RpcClient 更像是 nacos 自己的注册中心客户端。我们都知道 nacos 内部使用 grpc 当作通信组件，可以这么说，只要是和服务端交互的操作，都离不开 grpc 框架的帮助。**但是我们不能因为 nacos 注册中心客户端直接使用了 gprc 客户端，就想当然地认为 nacos 注册中心客户端就是一个 grpc 客户端**。说到底，nacos 只是使用 grpc 框架进行网络通信，nacos 客户端应该有自己特殊的功能。比如说：**健康检测功能，连接重建功能，连接重建事件检测功能，连接状态监控功能，自动更换服务器地址功能，客户端能力重置功能，同步发送请求、异步发送请求等等功能。这些都是 nacos 注册中心客户端自己的功能，这些功能的实现离不开 grpc 框架的帮助，但并不是 grpc 框架的功能，所以 nacos 定义了一个 RpcClient，把这些功能都定义在了 RpcClient 中**。

**而 GrpcClient 中定义的就是 grpc 框架自己的内容了，在 GrpcClient 类中，主要就是搭建了 grpc 框架的客户端，实现了客户端和服务端建立连接的功能；至于最后的 GrpcSdkClient 这个子类，只是提供了要访问的服务端的端口号的偏移量**。从功能的角度分析下来，我想大家对 RpcClient、GrpcClient、以及 GrpcSdkClient 这三个类应该有了更清楚地认识。当然，在源码中，nacos 客户端其实还有一个类，那就是 GrpcClusterClient 类，这个类也是 GrpcClient 的子类，是 nacos 的集群客户端。因为我还没有为大家引入集群，所以就把这个类给省略了。如果把这个类加上，那么在 nacos 注册中心客户端，这四个类的关系就可以展示成下面这样。请看下面代码块。

```
                        RpcClient
                            |
                        GrpcClient
                  | 					 | 
           GrpcSdkClient          GrpcClusterClient
```

好了，客户端的知识回顾完了，接下来让我们再回到 nacos 注册中心服务端，实际上 nacos 注册中心服务端的类结构和客户端非常相似，比如说，**相对于 RpcClient 这个客户端类，我可以自己定义一个 BaseRpcServer 当作 nacos 服务端的基础类，因为在 nacos 源码中，服务端并没那么多花里胡哨的功能，所以我为自己程序定义的这个 BaseRpcServer 类的内容就可以简单一些**。比如我就在这个类中定义启动服务、终止服务等等方法，具体实现请看下面代码块。

```
package com.cqfy.nacos.core.remote;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/9
 * @方法描述：nacos服务端的抽象父类，在这个类中定义了启动nacos服务端的方法
 */
public abstract class BaseRpcServer {

    //服务端启动之初，就把Payload的各种实现类加载到内存中，这个在注册中心客户端已经见过了
    //Payload的各种实现类，其实就是nacos自己定义的各种请求体和响应体
    static {
        PayloadRegistry.init();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：启动服务器的方法，因为当前类是GrpcSdkServer的父类，而GrpcSdkServer又是springboot容器的bean对象
     * GrpcSdkServer对象在创建之后会调用父类的这个方法，这是@PostConstruct注解的作用，然后就开始正式启动服务端了
     * 在启动的过程中，又会调用到子类的启动服务端的方法
     */
    public void start() throws Exception {
        //得到当前类的名称，用于日志记录
        String serverName = getClass().getSimpleName();
        //记录日志信息，表示服务端正在启动
        Loggers.REMOTE.info("Nacos {} Rpc server starting at port {}", serverName, getServicePort());
        
        //调用子类方法，进一步启动nacos服务端
        startServer();
        
        //记录启动完毕的日志信息
        Loggers.REMOTE.info("Nacos {} Rpc server started at port {}", serverName, getServicePort());
        
        //给JVM添加一个钩子函数，在JVM关闭时，调用nacos服务端的stopServer方法，停止服务器工作
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            //记录日志信息
            Loggers.REMOTE.info("Nacos {} Rpc server stopping", serverName);
            try {
                //调用stopServer方法，停止服务器工作
                BaseRpcServer.this.stopServer();
                //记录日志信息
                Loggers.REMOTE.info("Nacos {} Rpc server stopped successfully...", serverName);
            } catch (Exception e) {
                //有异常也记录日志信息
                Loggers.REMOTE.error("Nacos {} Rpc server stopped fail...", serverName, e);
            }
        }));

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：得到连接类型的方法，这里得到的就是grpc方式，因为现在已经使用grpc作为nacos内部网络通信组件了
     */
    public abstract ConnectionType getConnectionType();



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：启动服务器的方法，这个方法由子类BaseGrpcServer实现
     */
    public abstract void startServer() throws Exception;


    //得到nacos服务端端口号偏移量的方法，注意，这里说得nacos服务端并不是相对于web界面的服务端，web界面服务端使用的端口号是8848
    //这里说的服务端对应的是nacos客户端而言，端口号会在8848的基础上加上一个偏移量，得到新的端口号就是nacos服务端的端口号
    //也可以称它为grpc服务器使用的端口号
    public abstract int rpcPortOffset();


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：得到grpc服务器端口号
     */
    public int getServicePort() {
        //可以看到从环境变量中得到了springboot启动web容器的端口号，然后在这个基础上加了一个偏移量，得到的端口号就是grpc服务器的端口号
        //大家可以暂时忽略这里的EnvUtil工具类，在下一小节就会讲到，到时候大家就知道它是什么了
        return EnvUtil.getPort() + rpcPortOffset();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：停止服务器工作的方法
     */
    public final void stopServer() throws Exception {
        shutdownServer();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：终止服务器工作的方法，交给子类实现
     */
    @PreDestroy
    public abstract void shutdownServer();

}
```

好了，从上面代码块可以看出来，我们目前引入的这个 BaseRpcServer 类确实非常简单，只是定义了一些最基本的方法，并且有些重要方法还是交给子类去实现的。比如说 startServer() 和 shutdownServer() 这两个方法，都是交给 BaseRpcServer 的子类实现的。那么，BaseRpcServer 的子类是什么呢？**如果是对标客户端的 GrpcClient 类的话，BaseRpcServer 的子类就可以定义为 BaseGrpcServer。从名字上就可以看出来，这个类已经和 grpc 框架产生联系了，实际上也正是这样，在 nacos 框架源码中，BaseGrpcServer 这个类中定义的全是和 grpc 框架服务端有关的内容，确切地说，grpc 框架服务端就是在 BaseGrpcServer 类中构建完毕的**。关于这个 BaseGrpcServer 类的内容，我们先不必急着去看。现在先让我们看一下目前引入的两个类，一个是顶级父类，也就是 BaseRpcServer，另一个是子类 BaseGrpcServer；在这两个类中，**BaseRpcServer 可以称得上是 nacos 注册中心自己的服务端，而 BaseGrpcServer 就是 grpc 框架的内容了，也就是用来构建 grpc 框架服务端的**。我想到目前为止，大家对这两个类的作用应该有了一个简单的了解了，尽管 BaseGrpcServer 这个类我还没有为大家实现。

  

现在让我们再次回过头，再看一看刚才我为大家分析的 nacos 注册中心客户端的几个 Client 核心类，请看下面代码块。

```
                        RpcClient
                            |
                        GrpcClient
                  | 					 | 
           GrpcSdkClient          GrpcClusterClient
```

为了对标之前实现的客户端的这几个类，刚才我引入了服务端的两个类，一个是 BaseRpcServer 类，对标客户端的 RpcClient 类；另一个是 BaseGrpcServer 类，对标客户端的 GrpcClient 类。如果继续按照客户端的类结构分析，那么接下来我们肯定在服务端的 BaseGrpcServer 类下面继续细分，**为服务端引入一个对标 GrpcSdkClient 的类，我就把它定义为 GrpcSdkServer，这个类是 BaseGrpcServer 的子类，专门为 grpc 服务端提供一些配置参数用的；我相信从名字上大家就能看出来，这个 GrpcSdkServer 类就是 sdk 服务端，这个服务端接收的就是来自 skd 客户端的连接；当然，有了 sdk 服务端，就应该有对应的集群服务端，如果对标客户端的 GrpcClusterClient 类的话，我可以为服务端引入一个 GrpcClusterServer 类，这个类就是集群服务端，集群服务端专门用来接收集群客户端的连接的**。光看文字可能还是有点乱，接下来我就仿照客户端那样，把服务端的类结构展示给大家。请看下面代码块。

```
                        BaseRpcServer
                             |
                        BaseGrpcServer
                  | 					 | 
           GrpcSdkServer          GrpcClusterServer
```

这四个服务端的类展示出来之后，对比着客户端的那几个类，现在大家应该对服务端的 Server 类结构有一个大概的了解了。虽然目前我只给大家实现了 BaseRpcServer 类，其他三个类都没有实现。确切地说，应该是其他两个类，也就是 BaseGrpcServer 和 GrpcSdkServer 这两个类没有实现。和客户端一样，因为目前还没有引入集群模式，所以我也不会为服务端引入 GrpcClusterServer 类，现在展示在这里只是先让大家对服务端的类结构有一个清楚地认识，先混个眼熟，后面引入集群后再真正实现 GrpcClusterServer 类。

  

好了，多于话就不说了，现在还是让我们回到服务端的功能实现上来。刚才我已经引入了 BaseRpcServer，并且在这个类中定义了几个最基本的方法，接下来就应该看看 BaseRpcServer 的子类，也就是 BaseGrpcServer 类究竟该如何实现了。其实也没什么好说的，这个类主要就是实现了父类的几个方法，构建了真正的 grpc 的服务端；比如说 startServer()，shutdownServer() 这两个方法时一定要实现的。既然构建的是 grpc 的服务端，那肯定就要用到 grpc 框架的组件了。所以，这个 BaseGrpcServer 类，目前可以先简单定义成下面这样。请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/10
 * @方法描述：这个类是nacos服务端的grpc服务端基础类，在这个类中定义了服务器的多个重要组件，可以说是nacos服务端的核心类
 */
public abstract class BaseGrpcServer extends BaseRpcServer {


    //grpc服务端对象，这个grpc框架的组件
    private Server server;



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
        //以下都是grpc框架的代码，如果不了解grpc框架用法的话，可以暂时只看看我列出的代码注释
        //等后面讲解grpc框架的时候，大家就清楚这些代码的作用了。现在就算不清楚下面这些代码的作用
        //也不会影响大家学习nacos框架，因为下面这些代码的作用非常纯粹，就是构建grpc服务端的，和nacos的功能没什么关系
        
        //创建一个grpc的服务注册表对象
        final MutableHandlerRegistry handlerRegistry = new MutableHandlerRegistry();
      
        //因为grpc底层使用的也是netty构建得服务器，所以这里先得到一个netty服务构建器，设置服务器端口号以及要用到的线程池
        NettyServerBuilder builder = NettyServerBuilder.forPort(getServicePort()).executor(getRpcExecutor());
       
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



    //以下这几个配置参数都是程序默认的，用户也可以自己配置这些配置参数
    
     //获得客户端向服务端发送ping的频率
    protected long getPermitKeepAliveTime() {
        return GrpcServerConstants.GrpcConfig.DEFAULT_GRPC_PERMIT_KEEP_ALIVE_TIME;
    }


    //获得连接最大的检测时间
    protected long getKeepAliveTime() {
        return GrpcServerConstants.GrpcConfig.DEFAULT_GRPC_KEEP_ALIVE_TIME;
    }


    //获得务端接收客户端探活响应的超时时间
    protected long getKeepAliveTimeout() {
        return GrpcServerConstants.GrpcConfig.DEFAULT_GRPC_KEEP_ALIVE_TIMEOUT;
    }

    //获得服务器最大入站消息大小
    protected int getMaxInboundMessageSize() {

        Integer property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.MAX_INBOUND_MSG_SIZE_PROPERTY, Integer.class);
        if (property != null) {
            return property;
        }
        return GrpcServerConstants.GrpcConfig.DEFAULT_GRPC_MAX_INBOUND_MSG_SIZE;
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


    //得到grpc要使用的线程池
    public abstract ThreadPoolExecutor getRpcExecutor();
    
}
```

以上就是目前的 BaseGrpcServer 类的内容，可以看到，在 BaseGrpcServer 类中定义的全是和 grpc 服务端相关的方法和成员变量，因为要构建的是 grpc 的服务端，最终启动的也是 grpc 的服务端，所以直接在 BaseGrpcServer 类中定义了 grpc 的服务端组件成员变量，也就是 Server 对象；然后就是实现了父类的 startServer() 和 shutdownServer() 方法，shutdownServer() 方法的逻辑非常简单，就是直接终止 grpc 的服务端工作即可，startServer() 方法的逻辑就有点复杂了，如果大家不太了解 grpc 框架的使用方法，有些代码理解起来可能还比较陌生。不过我在代码注释中也跟大家解释了，这些代码都是纯粹的构建 grpc 服务端的代码，设置了 grpc 服务端需要的配置参数，这些代码和 nacos 本身的功能没有多大关系，所以大家不理解也没有多大的关系，后面更新 grpc 的时候就理解了。总之，现在我也已经把 BaseGrpcServer 这个类的内容展示给大家了。如果只从 grpc 服务端的角度来讲，**这个类的内容也非常简单，因为它就做了一件重要的事，那就是在 startServer() 方法中构建了 grpc 的服务端，然后启动了服务端**。这个类实现之后，接下来就该实现最终的子类，也就是 sdk 服务端 GrpcSdkServer 类了。

说到这个 GrpcSdkServer 类，其实这个类的实现也非常简单，它是用来和客户端的 GrpcSdkClient 类对标的，客户端的 GrpcSdkClient 类的内容就非常简单，无非就是提供了一些配置信息和要访问的服务端的端口号的偏移量；**这个服务端的 GrpcSdkServer 类的作用也是一样，仅仅是为 grpc 服务端提供了一些配置参数而已**。刚才我在为大家展示 BaseGrpcServer 类的内容时，大家可以看到在 startServer() 方法中为 grpc 的服务端设置了很多配置参数，这些配置参数都是通过 BaseGrpcServer 类的其他方法获得的，并且获得的都是程序内置的默认值。这一点我在上面代码块中给大家解释了。我们都知道，一个优秀的框架不仅仅会给这些配置信息定义一些默认值，还会让用户根据自己的需求自定义配置信息的值。nacos 框架也不例外，**在 nacos 框架中，用户为 grpc 服务端定义的配置信息的值就可以从 grpc 服务端的最终子类，也就是 GrpcSdkServer 对象中获得。grpc 服务端需要的配置参数我们已经知道了，就是在 BaseGrpcServer 类的 startServer() 方法中需要的那几个，所以我们只需要在 GrpcSdkServer 类中重写父类 BaseGrpcServer 的获得配置信息的那几个方法，父类不就可以调用子类方法，得到子类提供的配置信息的值了吗**？所以 GrpcSdkServer 可以定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：这个是真正的grpc服务端的实现类，这个类要交给springboot容器来管理
 */
public class GrpcSdkServer extends BaseGrpcServer {


   /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/15
    * @方法描述：得到grpc服务端端口号偏移量的方法
    */
    @Override
    public int rpcPortOffset() {
        return Constants.SDK_GRPC_PORT_DEFAULT_OFFSET;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：得到grpc框架要使用的线程池的方法
     */
    @Override
    public ThreadPoolExecutor getRpcExecutor() {
        return GlobalExecutor.sdkRpcExecutor;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：获得连接最大的检测时间
     */
    @Override
    protected long getKeepAliveTime() {
        //先从环境变量中获得
        //这里的这个EnvUtil大家要先记住，很快就会实现它
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_KEEP_ALIVE_TIME_PROPERTY, Long.class);
        //如果得到了就直接返回
        if (property != null) {
            return property;
        }
        //如果没有得到就调用父类方法，获得程序内置的默认时间
        return super.getKeepAliveTime();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：获得务端接收客户端探活响应的超时时间
     */
    @Override
    protected long getKeepAliveTimeout() {
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_KEEP_ALIVE_TIMEOUT_PROPERTY, Long.class);
        if (property != null) {
            return property;
        }
        return super.getKeepAliveTimeout();
    }

   /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/15
    * @方法描述：获得服务器最大入站消息大小
    */
    @Override
    protected int getMaxInboundMessageSize() {
        Integer property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_MAX_INBOUND_MSG_SIZE_PROPERTY, Integer.class);
        if (property != null) {
            return property;
        }
        int size = super.getMaxInboundMessageSize();

        if (Loggers.REMOTE.isWarnEnabled()) {
            Loggers.REMOTE.warn("Recommended use '{}' property instead '{}', now property value is {}",
                    GrpcServerConstants.GrpcConfig.SDK_MAX_INBOUND_MSG_SIZE_PROPERTY,
                    GrpcServerConstants.GrpcConfig.MAX_INBOUND_MSG_SIZE_PROPERTY, size);
        }

        return size;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：获得客户端向服务端发送ping的频率
     */
    @Override
    protected long getPermitKeepAliveTime() {
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_PERMIT_KEEP_ALIVE_TIME, Long.class);
        if (property != null) {
            return property;
        }
        return super.getPermitKeepAliveTime();
    }

}
```

到此为止，这个 GrpcSdkServer 类我也为大家实现完毕了。nacos 注册中心服务端的三个核心类就展示完毕了，类结构如下，请看下面代码块。

```
BaseRpcServer   nacos自己的服务端类，定义了最基础的启动服务端，终止服务端的方法
     |
BaseGrpcServer  grpc服务端类，构建和启动了grpc服务端
     | 					 
GrpcSdkServer   grpc服务端类，定义了获得grpc服务端配置信息的各个方法        
```

我知道随着 GrpcSdkServer 类的展示，大家现在肯定有很多困惑，首先就是刚刚展示的 GrpcSdkServer 类，这个类重写了父类的四个获得 grpc 服务端相关配置信息的方法，分别是 getKeepAliveTime()、getKeepAliveTimeout()、getMaxInboundMessageSize()、getPermitKeepAliveTime() 这几个方法，**在这几个方法中，都用到了一个 EnvUtil 组件，确切地说，最终都是从 EnvUtil 工具类中获得了用户自己定义的配置信息的值，如果从 EnvUtil 中获取不到对应的值，就意味着用户没有自定义配置信息，这时候就可以调用父类方法，使用程序内置的默认值即可**。这些逻辑本身没什么难度，大家肯定能看明白，让大家困惑的其实是这个 EnvUtil 工具类，从名字上来看，这个 EnvUtil 肯定和环境变量信息有关。**实际上在 nacos 源码中，这个 EnvUtil 封装着 nacos 需要的所有环境变量和配置信息，用户自定义的配置信息也会封装到这个 EnvUtil 组件中**。那这是怎么做到的呢？**我们启动的明明是一个 springboot 程序，用户定义的各种配置信息肯定会被 springboot 的环境变量组件封装起来，这些信息是怎么从 springboot 交给 nacos 的 EnvUtil 的呢**？ 这肯定是困扰大家的第一个问题，第二个问题也很容易就能想到，那就是 EnvUtil 该怎么定义呢？或者说 EnvUtil 该怎么封装那些配置信息和环境变量信息呢？第三个问题就更重要一些了，那就是我已经为大家实现并展示了 nacos 服务端的三个核心类，既然核心类都实现类，nacos 的服务端究竟该怎么启动呢？

总结下来就是这三个问题：

**1 如何使用 BaseRpcServer、BaseGrpcServer、GrpcSdkServer 这三个类，启动 nacos 的服务端？**

**2 EnvUtil 组件该如何定义，如何实现？**

**3 我们启动的 springboot 程序，怎么把环境变量信息和用户定义的配置信息交给 EnvUtil 组件使用？**

接下来就让我来为大家依次解决这三个问题，当然，我们的当务之急就是尽快启动 nacos 的服务端，不然我们本章引入的 BaseRpcServer、BaseGrpcServer、GrpcSdkServer 这三个类不就白实现了吗？所以我们先来解决第一个问题，真正启动 nacos 的服务端。

## 启动 nacos 注册中心服务端

其实启动 nacos 注册中心的服务端已经没什么好说的了，核心类都实现了，按照常规思路，肯定是创建服务端对象，然后调用 start 方法直接启动服务端即可。那么创建服务端对象时，要创建哪个类的对象呢？**首先就把 BaseRpcServer、BaseGrpcServer 这两个类排除了，因为这两个类都是抽象类，要创建只能创建子类 GrpcSdkServer 的对象**。也就是说，**GrpcSdkServer 类的对象就是我们最终要创建的服务端对象，因为 GrpcSdkServer 类继承了 BaseRpcServer 类，所以 GrpcSdkServer 对象就可以直接调用父类 BaseRpcServer 中的 start() 方法，在 start() 方法中，又会调用抽象子类 BaseGrpcServer 中的 startServer() 方法，启动真正的 grpc 服务端**。到此为止，nacos 注册中心服务端就终于启动了。所以我们接下来要做的就是创建 GrpcSdkServer 对象，然后调用该对象的 start() 方法。

这个时候，请大家想一想，我们要怎么实现这个操作呢？**别忘了我们启动的是一个 springboot 程序，如果能在 springboot 启动的过程中，把 nacos 的服务端启动了，这不就更完美了吗**？这个思路一旦明确了，那就很好实现了，**既然使用了 springboot，创建对象的方法就很简单了，直接把 GrpcSdkServer 类的对象交给 springboot 容器管理不就成了吗？也就是说，只要在 GrpcSdkServer 类上加一个 @Service 注解就行**。这样一来，随着 springboot 的启动，GrpcSdkServer 类的对象也会被创建，只要在对象创建完毕之后，也就是 GrpcSdkServer 类的构造方法调用完了，执行顶层父类的 start() 方法，就可以直接启动 nacos 服务端了。**这个也很好实现，jdk 为我们提供了一个很方便的注解，那就是 @PostConstruct 注解，被该注解标注的方法会在类对象的构造方法调用完毕之后被调用，所以我们只需要在 BaseRpcServer 类的 start() 方法上添加该注解即可。因为 GrpcSdkServer 是 BaseRpcServer 的子类，所以 springboot 创建完 GrpcSdkServer 类的对象后，会调用其父类标注着 @PostConstruct 注解的 start() 方法，这样一来，nacos 服务器也就启动成功了**。

经过上面的分析之后，最终我们实现的服务端的几个类可以再简单重构一下，当然，BaseGrpcServer 没有什么需要重构的地方，所以我就只为大家展示重构之后的 BaseRpcServer 类和 GrpcSdkServer 类了。首先是重构之后的 BaseRpcServer 类，请看下面代码块。

```
package com.cqfy.nacos.core.remote;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/9
 * @方法描述：nacos服务端的抽象父类，在这个类中定义了启动nacos服务端的方法
 */
public abstract class BaseRpcServer {

    //服务端启动之初，就把Payload的各种实现类加载到内存中，这个在注册中心客户端已经见过了
    //Payload的各种实现类，其实就是nacos自己定义的各种请求体和响应体
    static {
        PayloadRegistry.init();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/9
     * @方法描述：启动服务器的方法，因为当前类是GrpcSdkServer的父类，而GrpcSdkServer又是springboot容器的bean对象
     * GrpcSdkServer对象在创建之后会调用父类的这个方法，这是@PostConstruct注解的作用，然后就开始正式启动服务端了
     * 在启动的过程中，又会调用到子类的启动服务端的方法
     */
    @PostConstruct
    public void start() throws Exception {
        //得到当前类的名称，用于日志记录
        String serverName = getClass().getSimpleName();
        //记录日志信息，表示服务端正在启动
        Loggers.REMOTE.info("Nacos {} Rpc server starting at port {}", serverName, getServicePort());
        
        //调用子类方法，进一步启动nacos服务端
        startServer();
        
        //记录启动完毕的日志信息
        Loggers.REMOTE.info("Nacos {} Rpc server started at port {}", serverName, getServicePort());
        
        //给JVM添加一个钩子函数，在JVM关闭时，调用nacos服务端的stopServer方法，停止服务器工作
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            //记录日志信息
            Loggers.REMOTE.info("Nacos {} Rpc server stopping", serverName);
            try {
                //调用stopServer方法，停止服务器工作
                BaseRpcServer.this.stopServer();
                //记录日志信息
                Loggers.REMOTE.info("Nacos {} Rpc server stopped successfully...", serverName);
            } catch (Exception e) {
                //有异常也记录日志信息
                Loggers.REMOTE.error("Nacos {} Rpc server stopped fail...", serverName, e);
            }
        }));

    }



    //省略其他方法

}
```

然后是重构之后的 GrpcSdkServer 类，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/15
 * @方法描述：这个是真正的grpc服务端的实现类，这个类要交给springboot容器来管理
 */
@Service
public class GrpcSdkServer extends BaseGrpcServer {


   /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/15
    * @方法描述：得到grpc服务端端口号偏移量的方法
    */
    @Override
    public int rpcPortOffset() {
        return Constants.SDK_GRPC_PORT_DEFAULT_OFFSET;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：得到grpc框架要使用的线程池的方法
     */
    @Override
    public ThreadPoolExecutor getRpcExecutor() {
        return GlobalExecutor.sdkRpcExecutor;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：获得连接最大的检测时间
     */
    @Override
    protected long getKeepAliveTime() {
        //先从环境变量中获得
        //这里的这个EnvUtil大家要先记住，很快就会实现它
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_KEEP_ALIVE_TIME_PROPERTY, Long.class);
        //如果得到了就直接返回
        if (property != null) {
            return property;
        }
        //如果没有得到就调用父类方法，获得程序内置的默认时间
        return super.getKeepAliveTime();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：获得务端接收客户端探活响应的超时时间
     */
    @Override
    protected long getKeepAliveTimeout() {
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_KEEP_ALIVE_TIMEOUT_PROPERTY, Long.class);
        if (property != null) {
            return property;
        }
        return super.getKeepAliveTimeout();
    }

   /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/15
    * @方法描述：获得服务器最大入站消息大小
    */
    @Override
    protected int getMaxInboundMessageSize() {
        Integer property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_MAX_INBOUND_MSG_SIZE_PROPERTY, Integer.class);
        if (property != null) {
            return property;
        }
        int size = super.getMaxInboundMessageSize();

        if (Loggers.REMOTE.isWarnEnabled()) {
            Loggers.REMOTE.warn("Recommended use '{}' property instead '{}', now property value is {}",
                    GrpcServerConstants.GrpcConfig.SDK_MAX_INBOUND_MSG_SIZE_PROPERTY,
                    GrpcServerConstants.GrpcConfig.MAX_INBOUND_MSG_SIZE_PROPERTY, size);
        }

        return size;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/15
     * @方法描述：获得客户端向服务端发送ping的频率
     */
    @Override
    protected long getPermitKeepAliveTime() {
        Long property = EnvUtil.getProperty(GrpcServerConstants.GrpcConfig.SDK_PERMIT_KEEP_ALIVE_TIME, Long.class);
        if (property != null) {
            return property;
        }
        return super.getPermitKeepAliveTime();
    }

}
```

好了，到此为止，我就为大家把 nacos 的服务端构建完毕，并且启动成功了。当然，我还要再补充一点：**目前我们只是实现了服务端的启动，并且还是最简单的服务端的启动，服务端目前的功能并不完善，我们还没有实现服务端接收并处理客户端连接，也没有实现接收并处理客户端请求等等功能**，这些功能等后面章节才会实现完整，因为我们现在还有两个问题急需解决，那就是：

**EnvUtil 组件该如何定义，如何实现？**

**启动的 springboot 程序，怎么把环境变量信息和用户定义的配置信息交给 EnvUtil 组件使用？**

并且我还可以跟大家说一句，这两个问题其实也和 springboot 程序的启动过程有关系，请大家想一想，当 nacos 服务端随着 springboot 启动而启动时，肯定需要从 GrpcSdkServer 对象中获得用户自己定义的配置信息，如果用户没有为 grpc 服务端定义配置信息，那就使用程序内置的默认值。假如用户定义了，就是用用户定义的。这也就意味着，**在 nacos 服务端真正启动之前，springboot 持有的配置信息和环境变量信息都要交给 nacos 的 EnvUtil 组件，因为 nacos 服务端是从 EnvUtil 组件中获得用户自定义的配置信息的**。那 nacos 服务端启动之前，或者说 nacos 正在向 grpc 服务端设置配置信息的时候，springboot 程序启动到哪个阶段了呢？**肯定是创建 bean 对象阶段**，这个不用过多解释吧？只有 GrpcSdkServer 对象创建完毕，才会调用顶层父类的 start() 方法。这也就是说，**肯定要在 springboot 创建 bean 对象之前，把 springboot 管理的环境变量和配置信息交给 nacos 的 EnvUtil 组件**。那这个操作该怎么实现呢？毕竟我们现在连 EnvUtil 该怎么定义都不知道。

当然，我也能想到也许有些朋友觉得我又开始把话题扯远了，不好好继续实现 nacos 注册中心服务端的相关功能，反而开始围绕着环境变量配置信息做文章。我必须要解释一句，这个功能非常重要，甚至在服务端接收客户端连接时，都需要使用这个功能来执行相关的判断。除此之外，**nacos 是单机模式还是集群模式启动，nacos 的数据源配置，这些都可以交给用户自己定义，并且定义的信息都会存放到 EnvUtil 组件中。然后 nacos 服务端需要获得什么配置信息了，就直接从 EnvUtil 组件中获取即可。我们其实就可以把 EnvUtil 当作 nacos 服务端专用的封装所有配置信息和环境变量的组件，就像是 springboot 的 ConfigurableEnvironment**。如果大家已经看过我之前更新的 springboot 课程的第三章，就知道 ConfigurableEnvironment 是什么了，说得直接一点，**springboot 封装的所有配置信息和环境变量信息都封装到了 ConfigurableEnvironment 组件中**。

那么我们现在要实现的功能就比较明确了，**就是在 springboot 创建 bean 对象之前，把 ConfigurableEnvironment 组件中封装的所有信息交给 nacos 的 EnvUtil 组件管理，这样一来，等 GrpcSdkServer 对象创建完毕，nacos 服务端启动之前，要从 EnvUtil 组件获得 grpc 框架的配置信息时，就可以直接从 EnvUtil 组件中获得了**。经过以上分析，这会儿大家应该认识到了 EnvUtil 组件的重要性了吧？所以兜兜转转又回到了刚才的两个问题：

**EnvUtil 组件该如何定义，如何实现？**

**启动的 springboot 程序，怎么把环境变量信息和用户定义的配置信息交给 EnvUtil 组件使用？**

这两个问题这一章是讲解不完了，就留到下一章讲解吧。朋友们，我们下一章见！

注意：现在大家还不能去看第七版本代码，继续看下一章吧，能看的时候我就在文章结尾告诉大家了。