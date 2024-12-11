前两章我已经为大家实现了 nacos 注册中心服务端的启动，并且引入了 nacos 自己的监听器体系，还有 nacos 的 EnvUtil 组件，看起来实现了不少功能了，但这仅仅只是一个开始，还有更多的功能等着我们去实现。就比如说到目前为止，我们也只是刚刚实现了启动 nacos 注册中心的服务端，服务端启动之后肯定要执行更核心的操作呀，**比如接收客户端连接，处理接收到的客户端连接，接收客户端请求，处理客户端请求等等**，这些功能我们都没有实现呢。这一章就让我为大家继续实现这些核心功能。

## 引入 GrpcRequestAcceptor 单向流存根请求接收器

我相信，如果有朋友曾经有过使用 grpc 框架的经验，肯定会对我目前构建的 nacos 服务端感到困惑，认为我构建的 nacos 端非常奇怪，在功能上有致命的缺陷。原因很简单，**我根本就没有为服务端设置提供服务的对象，说到底 grpc-java 也是一个 rpc 框架，grpc 客户端执行远程调用，grpc 服务端提供对应的目标方法，也就是具体的服务**。但现在我构建的 nacos 服务端根本就没有给 grpc 服务端设置提供服务的功能。还记得我们在构建 nacos 客户端的时候，我为大家简单讲解的 grpc 框架的基础知识吗？当时我给大家举了一个例子，展示了几个代码块，现在我们那些代码块再次搬运到这里了，帮助大家简单回顾一下。请看下面代码块。

首先是 grpc 框架的服务端。

```
public class GrpcServer {

    
    public static void main(String[] args) throws IOException, InterruptedException {
        //创建和启动gRPC服务端
        Server server = ServerBuilder.forPort(8888)
                //添加具体的提供服务的对象
                .addService(new GreeterImpl())
                .build()
                .start();

        System.out.println("Grpc服务器启动了!");

        //防止主线程退出
        server.awaitTermination();
    }

    
    //实现服务内容
    static class GreeterImpl extends GreeterGrpc.GreeterImplBase {

        //这个sayHello方法就是服务端提供的服务，客户端使用存根直接远程调用sayHello方法后
        //请求到达服务端后，就会调用真正的sayHello方法来处理请求，然后回复响应
        @Override
        public void sayHello(HelloRequest request, StreamObserver<HelloResponse> responseObserver) {
            String name = request.getName();
            HelloResponse response = HelloResponse.newBuilder()
                    .setMessage("Hello, " + name)
                    .build();

            //返回响应
            responseObserver.onNext(reply);
            //发送响应结束
            responseObserver.onCompleted();
        }
    }
}
```

接下来是 Grpc 客户端，这里我展示的是发送单向流请求的客户端，请看下面代码块。

```
public class GrpcClient {

    
    public static void main(String[] args) {
        //创建客户端与服务端之间的channel，注意，这里只是把channel创建出来了，客户端和服务端
        //的连接并没有建立，具体细节得看源码才知道，后面给大家讲解grpc框架的时候再细说吧
        ManagedChannel channel = ManagedChannelBuilder.forAddress("localhost", 8888)
                .usePlaintext()
                .build();

        //创建单向流存根，所谓的这个单向流存根，其实就是用来发送请求给服务器的
        //注意，这里的存根只能处理请求响应模式，是单向流，并不是双向流
        GreeterGrpc.GreeterFutureStub futureStub = GreeterGrpc.newFutureStub(channel);

        //创建请求
        HelloRequest request = HelloRequest.newBuilder()
                .setName("Nacos")
                .build();

        //发送请求
        try {//这里调用了get方法，这就会导致主线程阻塞，直到响应到来
            //你也可以给future定义回调方法，让它成为一个完全异步的操作，这时候就变成异步发送请求了
            //下面就直接调用了sayHello方法，服务端有sayHello方法的具体实现
            HelloResponse response = futureStub.sayHello(request).get();
            //阻塞结束后，输出结果
            System.out.println("Response: " + response.getMessage());
        } catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
        }

        //关闭通道
        channel.shutdown();
    }
}
```

发送双向流的客户端我就不再为大家展示了，之前也已经展示过了。总之，通过以上两个代码块，我们可以再次认识到，**grpc 是一个 rpc 远程调用框架，客户端执行目标方法的远程调用操作，服务端要提供对应的目标方法，也就是提供目标服务**。但现在的情况是，我们构建的 nacos 的客户端功能已经比较完善了，客户端向服务端发送请求就是直接调用 request 方法即可，这个 request 方法就是 grpc 客户端远程调用的目标方法。**但是我们前两章构建好的 nacos 服务端，确切的说是 nacos 使用的 grpc 服务端，并没有定义一个和 request 同名的目标方法提供服务，也就是说，我们构建好的 grpc 服务端根本无法接收并处理来自客户端的任何请求**。这就是我们目前构建的 nacos 服务端的最致命的缺陷。这是我们目前发现的构建好的 nacos 服务端的一个重大问题。

这个问题说完了之后，接下来让我们再来想一想，假如说我们就是先不给 nacos 服务端定义目标服务方法，或者说是服务对象，这对我们当前的程序有什么影响吗？首先来看看服务端和客户端连接的建立，我想这个功能应该不会受到什么影响，因为 nacos 使用了 grpc 框架作为网络通信组件，而 grpc 框架底层又使用了 netty 构建了客户端和服务端。**而对 netty 构建的客户端和服务端来说，只要服务端绑定了端口号，netty 内部的 ServerSocketChannel 创建成功，服务端就可以接收来自客户端的连接，并且为该客户端创建被服务端管理的 SocketChannel；而客户端只需要知道服务端的 IP 地址和端口号，就可以访问服务端，和服务端建立网络连接**。就我们目前构建的 nacos 服务端而言，**我们在 BaseGrpcServer 类中构建的 grpc 服务端已经构建成功了，并且启动成功了，所以我们目前构建的 grpc 服务端完全可以接收来自 grpc 客户端的连接**。也就是说，**我们实现的 nacos 服务端完全可以接收来自 nacos 注册中心客户端的连接**。这一点是毋庸置疑的。当然，客户端连接的接收是 grpc 框架内部的操作，在 nacos 框架中是展示不出来的，现在我们也没必要深究，等后面实现 grpc 框架的时候，我再来为大家讲解。

好了，既然服务端接收客户端连接没有问题，那目前的存在的就只有刚才提到的问题了。那就是我们目前构建的 nacos 服务端根本无法处理来自 nacos 注册中心客户端的任何请求，因为我们根本没有为 nacos 使用的 grpc 服务端定义提供目标服务的方法。**那对于我们现在构建的 nacos 服务端来说，它能做到的就是接收客户端连接，但无法接收并处理来自客户端的任何请求**。所以接下来我们要实现的功能，就是让 nacos 服务端可以接收并处理来自 nacos 客户端的请求。

很好，终于要实现一些比较核心的功能了，相比于前两章实现的功能，接下来的功能就稍微重要一些，也有意思一些了。不过在实现之前我还是要再强调一下，nacos 服务端接收并处理客户端连接的功能还是和 grpc 框架有关，**因为其本质就是为 grpc 服务端定义一个提供服务的目标方法，nacos 注册中心客户端发送请求的是 request 方法，所以也应该给 grpc 服务端定义一个同名的 request 方法，来接收并处理客户端发送过来的请求**。所以，接下来我们直接这么做就可以了，而这一步也正是需要我们格外关心的。因为很快我们就会意识到，**我们曾经构建的 nacos 客户端向服务端发送请求的时候，远程调用了两个目标方法。一个就是 request 方法，这个方法发送的都是单向流请求；另一个方法就是 requestBiStream 方法，这个方法发送的都是双向流的请求和响应。考虑到这一点，我们在 grpc 服务端定义目标服务方法的时候，也应该定义两个，一个是 request 方法，这个方法专门接收并处理客户端发送的单向流请求；另一个定义为 requestBiStream 方法，这个方法专门接收并处理客户端发送的双向流请求和响应**。大家可以仔细品味品味这一点，我为大家把 grpc 框架的 proto 文件搬运到下面了，大家一看就清楚了。请看下面代码块。

```
syntax = "proto3";

import "google/protobuf/any.proto";
import "google/protobuf/timestamp.proto";

option java_multiple_files = true;
option java_package = "com.cqfy.nacos.api.grpc.auto";

message Metadata {
  string type = 3;
  string clientIp = 8;
  map<string, string> headers = 7;
}

//传输请求和响应的对象
message Payload {
  //消息元数据  
  Metadata metadata = 2;
  //请求或响应本身
  google.protobuf.Any body = 3;
}

service Request {
  //为单向流存根定义的目标方法
  rpc request (Payload) returns (Payload) {
  }
}

service BiRequestStream {
  //为双向流存根定义的目标方法
  rpc requestBiStream (stream Payload) returns (stream Payload) {
  }
}
```

如果大家快忘了 nacos 注册中心客户端向服务端发送请求的方法了，可以直接去查看客户端的 GrpcClient 类和 GrpcConnection 类，这都是旧知识，我就不再文章中带大家回顾了。总之，大家一定要把客户端向服务端发送请求的两种方法弄清楚，其实就是把 grpc 客户端远程调用 grpc 服务端的两个目标方法弄清楚。理清楚了这一点，接下来的就很好说了。

好了，现在我们就可以为 grpc 服务端定义目标方法了，那就一个一个来吧，首先就来看看单向流存根发送的请求，该如何被 grpc 服务端处理吧，也就是说，**现在我们要为 grpc 服务端定义一个 request 方法，这个方法就为客户端远程调用的 request 方法提供了目标服务**。 而我之所以先实现单向流请求的处理方法，还有一个原因，那就是在 nacos 客户端构建成功，并且连接服务端的时候，也就是在客户端 GrpcClient 对象的 connectToServer 方法中，nacos 客户端首先向 nacos 服务端发送了一个 ServerCheckRequest 请求，这个请求就是单向流存根向服务端发送的，并且接收到了一个 ServerCheckResponse 响应，从这个响应中得到了一个客户端连接 Id(这个客户端连接 Id 是什么很快就会讲到，现在大家可以暂时忽略)，具体代码我也搬运到这里了，请看下面代码块。

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

以上就是 nacos 客户端刚刚建立，并且向服务端发送 ServerCheckRequest 请求的方法，客户端发送 ServerCheckRequest 请求的时候，就是直接调用单向流存根的 request 方法，这一点可以从上面代码块的第 81 行得到印证。除此之外，我还想补充一点，在客户端向服务端发送请求之前，会先执行上面代码块的第 79 行代码，也就是 Payload grpcRequest = GrpcUtils.convert(serverCheckRequest) 这一行代码。因为 ServerCheckRequest 是 nacos 框架自己定义的请求体，而在 grpc 框架的层面，请求和响应在网络中是以 Payload 为载体传输的。所以在客户端向服务端真正发送请求前，需要把 nacos 自己定义的请求体对象转换成 grpc 框架定义的 Payload 对象。接下来就让我们看一下在 nacos 自己定义的请求转换成 Payload 对象的过程中，具体执行了哪些操作。我把 GrpcUtils 工具类的 convert() 方法搬运到下面了，请看下面代码块。

```
package com.cqfy.nacos.common.remote.client.grpc;




public class GrpcUtils {

   

    //把nacos自己的消息对象转换成grpc需要的Payload对象的方法
    public static Payload convert(Request request) {

        //构建请求元数据对象
        //这个请求元数据对象封装了客户端向服务端发送的请求的类型，其实就是类名称
        //还封装了客户端的ip地址等等信息
        Metadata newMeta = Metadata.newBuilder().setType(request.getClass().getSimpleName())
                .setClientIp(NetUtils.localIP()).putAllHeaders(request.getHeaders()).build();

        //把nacos的请求对象序列化
        byte[] jsonBytes = convertRequestToByte(request);

        Payload.Builder builder = Payload.newBuilder();

        //构建Payload对象，Payload对象封装了请求元数据信息和真正的请求体数据
        return builder.setBody(Any.newBuilder().setValue(UnsafeByteOperations.unsafeWrap(jsonBytes)))
                .setMetadata(newMeta).build();

    }


    //其他内容省略

}
```

好了，现在大家也都清楚了 Payload 对象中封装的信息是什么了，接下来就可以真正为 grpc 服务端实现接收和处理客户端单向流请求的 request 方法了。当然，**我们肯定不是直接定义一个 request 方法，这个方法肯定也是定义在一个类中，然后创建这个类的对象，把这个类的对象当作提供目标服务的对象，然后调用 grpc 服务端的 addService() 方法，把服务对象设置到 grpc 服务端之中，这个服务对象就可以发挥作用了**。由此可见，我们肯定要为 nacos 的服务端引入一个新的类了，**这个类的类名我已经想好了，就叫做 GrpcRequestAcceptor，翻译过来就是 grpc 请求接收器，这个请求接收器只接收并处理客户端单向流村更发送过来的请求对象**。具体实现如下所示，请看下面代码块。

```
package com.cqfy.nacos.core.remote.grpc;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/16
 * @方法描述：单向流请求接收器，这个接收器专门处理客户端发送过来的单向流请求
 */
@Service
public class GrpcRequestAcceptor extends RequestGrpc.RequestImplBase {


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：本类最核心的方法，服务端接收并处理客户端发送过来的单向流请求的方法
     */
    @Override
    public void request(Payload grpcRequest, StreamObserver<Payload> responseObserver) {

        //方法的具体内容暂时不做实现
    }
}
```

可以看到，我们刚刚引入的这个 GrpcRequestAcceptor 类继承了 grpc 框架的类，这是肯定的，因为最终我们创建的 GrpcRequestAcceptor 请求接收器对象也会作为提供服务的对象设置到 grpc 客户端中，说到底，GrpcRequestAcceptor 请求接收器本来就是为 grpc 服务端提供的组件，肯定要继承 grpc 自己的类，和 grpc 框架产生联系。好了，现在 GrpcRequestAcceptor 请求接收器也引入了，并且我在该类中定义了一个 request 方法，虽然该方法还未做实现。但是我们已经可以把其中的流程梳理清楚了：**只要我们把 GrpcRequestAcceptor 对象设置到 grpc 框架的服务端中，那么当 grpc 服务端接收到客户端发送过来的单向流请求，就会调用 GrpcRequestAcceptor 请求接收器的 request 方法处理该请求，并且返回客户端一个响应**。所以现在摆在我们面前的有两个待实现的功能：

**1 把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端中**

**2 实现 GrpcRequestAcceptor 请求接收器的 request 方法**

这两个功能先实现哪一个都可以，为了保证内容上的连贯性，我决定先把第二个功能实现了，请大家看看 GrpcRequestAcceptor 请求接收器的 request 方法是怎么处理客户端发送过来的单向流请求的。

实现 GrpcRequestAcceptor 请求接收器的 request 方法就没什么好分析的了，刚才回顾客户端的 connectToServer 方法时大家也都看到了，**客户端向服务端发送了一个 ServerCheckRequest 请求对象，这个 ServerCheckRequest 请求对象是单向流存根发送的，肯定就需要服务端的 GrpcRequestAcceptor 请求接收器的 request 方法处理，而处理的操作就是回复给客户端一个 ServerCheckResponse 响应，在响应中封装客户端连接的 Id**，所以，我们直接实现这部分的处理逻辑即可。请看下面代码块。

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


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/16
     * @方法描述：本类最核心的方法，服务端接收并处理客户端发送过来的单向流请求的方法
     */
    @Override
    public void request(Payload grpcRequest, StreamObserver<Payload> responseObserver) {

        //从请求元数据中得到客户端发送过来的请求的类型，其实就是请求的SimpleName
        String type = grpcRequest.getMetadata().getType();
        
        //判断服务器是否已经启动完毕了，还记得上一章结尾我给大家留的问题吗？
        //设置ApplicationUtils工具类的启动标志的作用是什么，这里就用到这个启动标志了
        if (!ApplicationUtils.isStarted()) {
            //如果服务器还没有启动完毕，就回复给客户端一个错误响应
            Payload payloadResponse = GrpcUtils.convert(ErrorResponse.build(NacosException.INVALID_SERVER_STATUS, "Server is starting,please try later."));
            //回复客户端错误响应
            responseObserver.onNext(payloadResponse);
            //表示信息回复完毕
            responseObserver.onCompleted();
            return;
        }



        //这里会判断接收到的是否为ServerCheckRequest请求，现在大家应该清楚了客户端和服务端建立连接的时候，发送的这个ServerCheckRequest请求是怎么被服务端处理的了
        if (ServerCheckRequest.class.getSimpleName().equals(type)) {

            //得到客户端连接Id,这里写成伪代码
            String clientId = "客户端连接Id";
            //创建成功响应，这里创建的ServerCheckResponse响应对象的第二个参数就是是否支持能力协商的标志
            Payload serverCheckResponseP = GrpcUtils.convert(new ServerCheckResponse(clientId, true));
            //回复客户端成功响应
            responseObserver.onNext(serverCheckResponseP);
            //表示信息回复完毕
            responseObserver.onCompleted();
            return;
        }
        
    }
}
```

以上就是我目前实现的 GrpcRequestAcceptor 请求接收器的 request 方法，这个方法成功处理了客户端发送过来的 ServerCheckRequest 请求，回复给客户端一个成功响应，是不是非常简单？但我不会沾沾仔细，现在还不是时候，因为我知道换了任何一位朋友都能看出来我目前实现的 GrpcRequestAcceptor 请求接收器的 request 方法是一个半成品，甚至连半成品都算不上。原因很简单：**客户端要发送给服务端的单向流请求可不止 ServerCheckRequest 一个，客户端注册服务实例时要发送给服务端的 InstanceRequest 请求，以及客户端执行健康检测时定期发送给服务端的 HealthCheckRequest 请求，还有很多其他的请求，这些都是单向流存根发送给 grpc 服务端的请求，都需要 GrpcRequestAcceptor 请求接收器的 request 方法处理；但我们目前实现的请求接收器的 request 方法只能处理 ServerCheckRequest 请求，稍微想想也能知道现在实现得这个方法并不完整，还有很多内容需要补充和重构**。

但这个还不是当前最重要的任务，反正内容非常多，一点点实现就行了，所以没必要急着一下子实现 GrpcRequestAcceptor 请求接收器的 request 方法的全部内容。因为还有一个很重要的问题等着我们解决，那就是 **如何把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端中**？当然，除此之外，我相信大家还会对一个小问题感到困惑，**那就是出现了好几次的客户端连接 Id 究竟是什么呢**？这个客户端连接 Id 从我们实现客户端功能的时候就出现了，我一直没有为大家讲解，现在实现服务端相关功能的时候又出现了好几次，大家肯定早就对这个客户端 Id 感到好奇了，所以接下来，我也会为大家仔细讲解一下客户端 Id 在整个程序中发挥的作用。但在这之前，还是要先把刚刚定义好的 GrpcRequestAcceptor 请求接收器对象设置到 grpc 服务端中。

## 设置 GrpcRequestAcceptor 请求接收器到 grpc 服务端中

说实话，把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端中的功能实现起来非常简单，其实根本就不用单独用一个小节展示出来，但是后面我们会对这个功能进行重构，所以我就把这个功能先用一个小节单独展示一下，这样大家印象还深刻一些。当然，还有另一个原因让我不得不单独展示这个功能，**因为我们接下来要执行的操作是把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端中，这本来就是构建 grpc 服务端的操作，编写的也都是 grpc 框架的代码，所以，如果大家不熟悉 grpc 框架的用法，可能对这一部分的代码还是感到晦涩**。因此我把它放到单独的一个小节，并且在代码中加上详细注释，大家可以简单看看，如果看不明白也没关系，大家只要知道接下来展示的代码就是把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端的操作即可，并且只要设置成功，那么请求接收器就可以按照我们预想的方式工作，接收并处理 nacos 客户端单向流存根发送过来的请求了。通过前两章构建的 nacos 注册中心的服务端，**我们可以知道 nacos 使用的 grpc 服务端的构建主要定义在 BaseGrpcServer 类中，所以把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端中的操作也应该定义在 BaseGrpcServer 类中**，具体实现请看下面代码块。

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


    //自动注入grpc请求接收器，这个请求接收器接收的是单向流存根发送的请求
    @Autowired
    private GrpcRequestAcceptor grpcCommonRequestAcceptor;


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

        //这里多添加了一行代码，调用了addServices方法，把grpcCommonRequestAcceptor成员变量添加到服务注册表中
        //注意，这里写的其实是伪代码，这一部分后面还会重构
        addServices(handlerRegistry);
        
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

        //创建单向流服务提供对象，准确地说，ServerServiceDefinition其实就是定义要提供的服务的对象
        //所以这里创建的是定义提供的服务的对象
        final ServerServiceDefinition serviceDefOfUnaryPayload = ServerServiceDefinition
                //设置服务名称
                .builder(GrpcServerConstants.REQUEST_SERVICE_NAME)
                //设置方法描述符和方法处理器
                .addMethod(unaryPayloadMethod, payloadHandler)
                .build();

        //将提供服务的对象添加到服务注册表中
        //这里其实是伪代码，后面会进行重构
        handlerRegistry.addService(serviceDefOfUnaryPayload);

    }
    
    
    //其他内容省略
}
```

到此为止，我们就实现了把 GrpcRequestAcceptor 请求接收器设置到 grpc 服务端中的功能，**就是直接把提供服务的对象添加到 grpc 框架的服务注册表中了**。从纯代码的角度来说，这个功能实现的非常简单，因为都是 grpc 框架最基本的构建服务端的代码，只要大家熟悉 grpc 框架的用法，那这些代码对大家就没有一点难度。当然，如果大家不熟悉 grpc 框架的用法，也不必焦虑，以上代码块展示的代码的功能对大家来说是明确的，大家知道这些代码的作用即可。这些代码对我们实现并掌握 nacos 框架服务端的其他功能没有什么影响。

好了，现在我们已经为 grpc 服务端定义好了处理单向流请求的服务了，按照流程来说，接下来就该趁热打铁，接着实现 grpc 服务端处理双向流消息的功能，和 grpc 服务端处理单向流请求的功能类似，处理双向流请求的目标方法可定也要定义在一个接收器对象中。但是，**我想我们还是先把处理双向流消息的功能放在一边，先不管双向流请求的接收器怎么定义，让我们先来看看在上一小节结尾提到的客户端连接 Id，让我来为大家分析一下所谓的客户端连接 Id 究竟是什么**。等这个分析完了，再回过头实现服务端处理客户端发送过来的双向流请求的功能，到时候大家就明白我为什么要先分析客户端连接 Id，再实现处理双向流请求功能了。

## 分析客户端连接 Id 在服务端的作用

我们刚才实现了 grpc 服务端处理客户端发送过来的单向流请求的功能，这个功能实现非常简单，因为它本身就是简单的请求——响应模式，客户端发送请求，服务端处理请求，直接回复响应即可。如果我们启动了 nacos 服务端，然后只启动了一个 nacos 注册中心客户端，让这一个客户端和服务端交互，发送单向流请求，服务端只需要处理请求然后直接回复响应，那我们刚才为 nacos 使用的 grpc 服务端实现得功能已经完全够用了。**但假如我现在为程序引入了双向流模式呢？然后启动了多个注册中心客户端，这些客户端有的向服务端注册了服务实例信息，有的向服务端订阅了服务实例信息，并且未来还未有新的客户端启动，然后向服务端注册新的服务实例信息**。如果是这种情况，那我们刚才为 grpc 服务端实现的处理客户端单向流请求的功能就不够用了，原因很简单，当有客户端向服务端订阅了某些服务实例信息时，这就意味着服务端一旦发现这些服务实例信息发生变更了，就要主动推送给客户端最新的服务实例信息，服务端主动向客户端推送消息，这显然是双向流提供的功能。所以**，要想让服务端顺利推送给客户端最新的服务实例信息，肯定要为 grpc 服务端引入双向流处理消息功能，也就是双向流接收或者主动发送消息的功能**。

但是我们刚才也说了，目前还不打算实现 grpc 服务端双向流功能，我在这里之所以又提到双向流，是因为我想到了一个问题。请大家思考一下，既然使用了 grpc 双向流功能，就意味着 grpc 服务端可以随时向客户端主动发送消息，假如现在有两个客户端，分别是客户端 A 和客户端 B 和注册中心服务端建立了连接，其中客户端 A 注册了一个服务实例信息，客户端 B 订阅了客户端 A 注册的服务实例信息，现在来看，服务端已经和两个客户端建立连接了。过了一会又启动了一个客户端 C，客户端 C 启动之后也注册了一个服务实例信息，这个服务实例信息和客户端 A 注册的服务实例信息是相同的，都是同一个服务之下的服务实例信息，这时候服务端肯定就会检查到客户端 B 订阅的服务实例信息发生了变更，要把客户端 A 和客户端 C 注册的服务实例信息都发送给客户端 B。所以，这个时候服务端就要主动向客户端 B 发送消息了。那现在问题就来了，注册中心服务端和三个客户端建立了连接，当服务端想要主动给客户端 B 发送消息的时候，服务端怎么就知道要和哪一个客户端发送消息呢？或者说，服务端怎么就知道自己维护的三个连接哪一个是客户端 B 的呢？

好了，我知道连接这两个字可能会让大家感觉困惑，不知道所谓的连接是以怎样的方式存在的。那我就说得更直白一点，**就算是 grpc 底层也是使用 netty 构建的 Channel 来发送消息的。每一个客户端和服务端建立连接，也就意味着在服务端为客户端创建了一个 SocketChannel 对象，也就意味着客户端和服务端的 Channel 建立了，可以直接发送消息了**。很好，那根据我们刚才举的例子，有三个客户端和注册中心服务端建立了连接，也就意味着在服务端创建了三个 SocketChannel 对象，那么现在服务端要主动向客户端 B 推送消息，服务端怎么就知道哪一个 SocketChannel 是服务端为客户端 B 创建的呢？只有找到了这个 Channel，服务端才能直接和客户端 B 通信。**换句话说，只要服务端在任何时候都能精确地找到客户端 B 对应的 SocketChannel，那么服务端就可以随时向客户端 B 发送消息**。那服务端怎么能随时精确找到客户端 B 对应的 SocketChannel 呢？这该怎么实现呢？这时候就轮到客户端 Id 登场了。

**实际上在 nacos 注册中心服务端，每接收到一个客户端连接，就会为这个客户端连接创建一个唯一的客户端 Id，还会封装这个客户端和服务端建立的连接通道，也就是连接对象**，当然，这里封装的就不是底层 netty 的 Channel 了，刚才我只是为了给大家举例子，才直接使用了 netty 的 Channel。在 grpc 中使用的就是流式对象，而流式对象发送消息最终还是使用客户端和服务端建立的 Channel 通道，这一点大家了解即可，不用深究，因为这是 grpc 框架的内容。那么服务端为每一个客户端创建了唯一的客户端 Id，封装了客户端和服务端建立的连接对象之后呢？**那就很简单了，把这些数据存放到一个 map 中，这样一来服务端就可以随时根据客户端唯一的连接 Id 找到对应的连接对象，然后使用连接对象向客户端主动发送消息即可**。假如说服务端知道客户端 B 的连接 Id，就可以直接从 map 中找到可以向客户端 B 发送消息的连接对象，然后直接向客户端发送消息即可。问题就这么解决了，非常简单。当然，**客户端连接 Id 在注册中心服务端很多地方都发挥了重要作用，我刚才在例子中展示的只是它作用的一部分**。

我知道文章进行到这里，大家还是会有五个疑问，**第一个问题就是服务端怎么给接收到的每一个客户端连接建立唯一连接 Id 呢？第二个问题就是服务端想给客户端 B 主动发送消息时，怎么就知道客户端 B 的连接 Id 是多少呢**？因为只有知道了连接 Id 才能去 map 中寻找对应的连接对象，才能使用连接对象向客户端 B 主动发送消息。**第三个问题就是存放客户端连接 Id 和对应的连接对象的 map 究竟是什么呢？难道就只是一个普通的 map 吗？这个 map 应该定义在哪里呢？第四个问题就是客户端和服务端建立的连接通道，也就是所谓的连接对象，究竟是什么呢**？**第五个问题就是客户端的连接 Id 和对应的连接对象在什么时候被放到 map 中呢？**我相信这五个问题一定是大家目前最关心的，当然，**关心这五个问题的同时也别忘了 grpc 服务端还有一个重要功能等着我们去实现，那就是处理双向流消息的功能**。这些内容在这一章是讲不完了，就留到下一章讲解吧。这时候大家应该也体会到了，要实现的功能非常多，但是每一个功能实现起来都很简单，并不是我故意简化难度，而是在 nacos 框架源码中就是这么写的，这个框架难度本身并不高。好了朋友们，我们下一章见吧！