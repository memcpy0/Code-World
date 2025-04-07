大家好，这一章对应我为大家提供的第三版本代码，也就是说，这一章看完之后，大家就可以去查看我提供的第三版本代码了。但是有些话我得写在前面，**那就是我在第三版本代码中添加的注释并不是很全面，无法完全把第三版本代码实现的功能的核心原理展示出来，因为这部分功能有点绕，只适合放在文章中详细讲解。所以大家在阅览第三版本代码之前，一定要把文章看明白，看懂，而且也要把我在文章展示的代码注释都看完了，之后再去看第三版本代码**，我相信就会易如反掌了。还有一点就是，如果大家在看本篇文章时有任何疑问，可以先把这些疑问暂时记在心里，等下一章看完了，看看这些疑惑是否解开了。

当然，我刚才说第三版本代码实现的功能有点绕，这确实是真的；但是这个功能虽然从逻辑上来说有点绕，并不意味着这个功能实现起来非常复杂。恰恰相反，这个功能实现起来非常简单，只不过其中涉及到的几种情况，确实需要大家仔细分析分析。好了，说了这么多，第三版本代码要实现的功能究竟是什么呢？这一点本章标题已经体现得非常清楚了，**那就是实现连接恢复后操作自动重试功能**。这么说可能会让大家有些迷糊，什么叫连接恢复后操作自动重试功能？接下来就让我来给大家解释一下。

经过上一章的学习，我相信现在大家应该都清楚客户端和服务端连接异常的情况了吧？比如说注册中心客户端去连接服务端的时候，连接失败了，就要从服务地址管理器中获得一个新的服务地址，然后去连接这个服务地址；还有一种情况就是注册中心客户端向服务器发送请求消息的时候，消息发送失败，这个时候也会检查连接情况，如果连接出现问题就要切换一个新的连接，和新的服务器重新建立连接。如果这个逻辑大家都理解了，接下来请大家思考这样一个问题：**那就是假如客户端正在执行服务注册任务，把一个服务实例注册到注册中心的服务端，但是在注册的时候客户端和服务端连接出现异常断开了。等注册中心客户端和新的服务器重新建立了连接，这个时候要执行什么操作呢**？不用说，**肯定就是重新执行刚才的注册服务实例到服务端的操作**，这个操作当然可以让用户手动来执行，但是一个优秀的框架肯定会内置了这个功能，**就是所谓的失败重试功能**。当程序执行任务失败了，程序会自动重试，重新执行这个任务，当然**，我们这里还添加了一个条件，那就是等待客户端和服务端的连接重新建立之后，才会重新执行刚才的任务**。现在大家应该明白了刚才我所说的"**实现连接恢复后操作自动重试功能**"是什么意思了吧？说到底它就是一个失败重试功能。我之所以搞了个奇怪的名称是因为我实在不想再讲解什么失败重试功能了，这个功能在过去实现的框架中已经讲过很多次了，我总是重复实现相同的功能，别人就会以为我只会讲这种简单的知识。所以我才在文章标题中换了一个故弄玄虚的名字。当然，我也要承认，nacos 注册中心的失败重试功能设计得还是有点意思的，也值得一讲。接下来，就让我们一起看看 nacos 注册中心的失败重试功能如何实现吧。

## 引入 RedoData，完成重试对象的创建

要实现 nacos 注册中心的失败重试功能，首先应该明确一个前提，**那就是这个失败重试功能是建立在客户端与服务端连接断开，然后又重建的基础之上的**。也就是说，**客户端程序发现连接断开了，并且连接断开之前的操作没有完成，现在连接恢复了，就要把之前失败的操作执行完毕**。那客户端怎么就知客户端连接断开了？怎么就知道某些操作在连接断开的时候失败了呢？**只有知道这些操作是连接断开前的失败操作，客户端才会在连接重建之后重新执行这些操作**。这个就要从注册中心客户端向服务端发送消息说起了。

请大家想一想，如果注册中心客户端在启动时和服务端建立连接失败了，这个时候客户端就要从服务地址管理器中获得一个新的服务器地址，和新的服务器建立连接。这个过程没有问题，但是在这个过程中，**注册中心的客户端还没有完全启动，既然没有完全启动，肯定也就无法执行什么操作，比如注册服务实例到注册中心服务端，或者是注销某个服务实例，或者是订阅某个命名空间下的服务实例等等，这些操作都无法被执行，客户端都没启动呢，怎么可能执行这些操作呢？既然不执行这些操作，也就不存在什么失败操作。所以就算后面客户端和新的服务端的连接建立成功了，客户端也没有什么要重新执行的操作**。所以，从这个角度来分析的话，当注册中心在启动时和服务端建立连接失败的这种情况，在失败重试功能中是没必要关心的。我想这个还是比较容易理解的。

而真正需要我们关心的，是注册中心客户端完全启动之后，执行某些操作的时候和服务器的连接断开了的情况。就比如说注册中心客户端启动之后，想把一个服务实例注册到注册中心的服务端，这时候，客户端肯定会调用 NamingService 的 registerInstance 方法。而这个方法会进一步执行，之后会来到 NamingGrpcClientProxy 类中，调用该类的 registerInstance 方法，就像下面代码块展示的这样，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;






/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //其他内容省略





   /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册服务实例到服务端的方法
     */
    @Override
    public void registerService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[REGISTER-SERVICE] {} registering service {} with instance {}", namespaceId, serviceName,
                instance);
        //判断要注册的服务实例是否为临时的
        if (instance.isEphemeral()) {
            //如果是临时的直接调用下面这个方法即可
            registerServiceForEphemeral(serviceName, groupName, instance);
        } else {
            //走到这里意味着要注册的服务实例是持久的，这时候调用下面这个方法即可
            //但在第一版本我还未给大家实现该方法，后面会重构完整
            //doRegisterServiceForPersistent(serviceName, groupName, instance);
        }
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册临时服务实例到服务端的方法
     */
    private void registerServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {
        doRegisterService(serviceName, groupName, instance);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法同上，就是在该方法中创建了一个InstanceRequest请求，然后把请求发送给服务端了
     */
    public void doRegisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        //创建了InstanceRequest请求对象，这个对象中封装了各种参数，命名空间，服务名称，分组名称等等
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.REGISTER_INSTANCE, instance);
        //在这里把请求发送给服务端，并且指定了要返回的对象类型
        requestToServer(request, Response.class);
    }






     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法就是把请求发送给服务端的方法，发送请求之后要返回一个响应对象，该方法的第二个参数就指定了要返回的对象的类型
     */
    private <T extends Response> T requestToServer(AbstractNamingRequest request, Class<T> responseClass)
            throws NacosException {
        //定义一个局部变量，接收服务器响应
        Response response = null;
        try {
            //设置请求头，请求头中的信息最终会设置到请求对象中，发送给服务端
            //其实这里只是填充了一个应用名称，也就是appName
            request.putAllHeader(getSecurityHeaders());
            //判断用户是否定义了请求超时时间，如果requestTimeout小于0则意味着没有定义
            //那么发送请求的时候就不必使用超时时间
            //这里就会把请求发送给服务器，并且得到一个响应
            response = requestTimeout < 0 ? rpcClient.request(request) : rpcClient.request(request, requestTimeout);
            //判断响应是否成功，不成功则抛出异常
            if (ResponseCode.SUCCESS.getCode() != response.getResultCode()) {
                throw new NacosException(response.getErrorCode(), response.getMessage());
            }
            //判断响应类型是否与方法指定的参数类型匹配
            if (responseClass.isAssignableFrom(response.getClass())) {
                return (T) response;
            }
            //下面就是异常情况了，大家看看即可
            NAMING_LOGGER.error("Server return unexpected response '{}', expected response should be '{}'",
                    response.getClass().getName(), responseClass.getName());
            throw new NacosException(NacosException.SERVER_ERROR, "Server return invalid response");
        } catch (NacosException e) {
            throw e;
        } catch (Exception e) {
            throw new NacosException(NacosException.SERVER_ERROR, "Request nacos server failed: ", e);
        }
    }




    //其他内容省略
}
```

通过上面代码块，大家可以看到，当 NamingGrpcClientProxy 对象调用了它的 registerService 方法后，很快就会调用到该类的 doRegisterService 方法，在这个 doRegisterService 方法中执行了 requestToServer 方法，而在 requestToServer 方法中，就会使用 nacos 的客户端，也就是 rpcClient 对象，把注册服务实例的请求发送给注册中心服务端。如果大家已经看过第二版本代码，**就会知道 rpcClient 客户端对象发送请求消息给服务端时，会判断请求发送是否失败，如果重试几次仍然失败，就意味着客户端和服务端的连接出现问题了，这个时候，客户端就会异步和新的服务器重新建立连接**。具体逻辑大家可以去 RpcClient 的 request 方法中查看，因为这个 request 方法内容太长了，所以我就不在文章中展示了，第二版本代码已经给大家展示得非常详细了。

好了，这个讲完之后，大家应该就明白了，我们需要关心的其实只有**注册中心客户端完全启动之后，执行某些操作的时候和服务器的连接断开了**的这种情况，在这种情况下，才需要在客户端与新的服务器连接重建后，执行失败重试的操作，也就是把连接断开之前的操作再执行一遍。**如果连接断开之前，客户端希望把某个服务实例注册到注册中心服务端，那么连接重建之后，客户端就要再把这个操作执行一遍，这样服务实例才能真正被注册到注册中心服务端**。那现在问题就来了，客户端怎么就知道有哪些操作执行失败了呢？比如说客户端正要把一个服务实例注册到服务端，已经调用了 registerService 方法了，但是在 rpcClient 最终要把注册服务实例的请求发送给注册中心服务端时，发现客户端和服务端的连接中断了，在客户端和新的服务端建立了新的连接之后，客户端怎么就知道之前有一个注册服务实例的操作需要重新执行呢？接下来就让我跟大家仔细分析一下这个功能该怎么实现。

其实实现这个功能的突破口很容易寻找，请大家想一想，客户端把请求发送给服务端，最终肯定要调用 RpcClient 的 request 方法，而 RpcClient 的 request 方法会在 NamingGrpcClientProxy 对象的 requestToServer 方法被调用时才会被进一步执行；也就是说，requestToServer 方法实际上是发起服务实例注册的方法。那**如果我在这个 requestToServer 方法执行之前对要被注册到服务端的服务实例对象做一个标记，就比如说定义一个 registered 标记，初始值为 false，意味着该服务实例还没有成功注册到服务端。接下来就要执行 requestToServer 方法了，如果这个方法成功执行了，那服务实例肯定就成功注册到服务端了，紧接着我只要把客户端这边的服务实例对象的 registered 标记设置为 true 即可**。这样一来，不就可以凭借这个标志来判断客户端服务实例对象是否把服务实例注册到注册中心了吗？这个思路显然是没问题的吧？要注册到服务端的信息都会封装到 Instance 对象中了，现在我们只需给这个 Instance 对象定义一个 registered 标志即可。当然，我肯定不会再重构 Instance 对象了，这个对象只需要封装服务实例的信息即可，现在我们要实现的是失败重试功能，没必要 Instance 对象和失败重试功能产生关联。其实我们可以专门为失败重试功能定义一些失败重试对象，把 registered 标记定义在失败重试对象中。

  

很好，既然要专门为失败重试功能定义失败重试对象了，那我们可以认真想想，程序中都存在什么和注册中心服务端打交道的操作，**我首先想到的就是注册服务实例和注销服务实例的操作，其次就是订阅服务实例和取消订阅服务实例的操作**；虽然注销服务实例、订阅服务实例、取消订阅服务实例的功能我们还没实现，但不妨碍我们先从失败重试的功能来理解它们。经过刚才的分析，**我现在的打算就是给注册服务实例和注销服务实例的操作定义一个失败重试对象，这个对象专门负责执行注册服务实例和注销服务实例操作的失败重试，我把这个对象定义为 InstanceRedoData**；至于订阅服务实例、取消订阅服务实例的操作，我把它们的失败重试对象定义为 SubscriberRedoData，这个 SubscriberRedoData 类我就不在文章中展示了，大家直接去看我提供的第三版本代码即可，或者等到第 6 版本代码的时候，大家再去看这一模块的功能。

可能有朋友还是会困惑，为什么我会把注册服务实例和注销服务实力的失败重试操作定义在同一个对象中，也就是 InstanceRedoData 对象，这里我给大家简单解释一下：服务实例既然可以注册到注册中心服务端，那么这个服务实例总会有注销的时候，也就是说，注册服务实例与注销服务实例从某种程度上来说，其实是一对捆绑操作。现在我们已经决定在 InstanceRedoData 对象中定义一个 registered 标记了。如果服务实例注册成功，这个 registered 标记就会被设置为 true，**那假如我在 InstanceRedoData 对象中再定义一个 unregistering 标志，也就是表示是否正在注销的标记，该标记初始化为 fasle；只要客户端执行了服务实例注销的操作，就会把对应的 InstanceRedoData 对象中的 unregistering 标志设置为 true，表示服务实例正在注销；当服务实例注销成功之后，就可以接着把 InstanceRedoData 对象中的 registered 设置为 fasle，表示服务实例没有注册，已经注销**。这样一来，客户端是否就可以根据这个标志，并且结合 registered 标志来判断服务实力注销的操作是否应该重试了呢？**也就是说，一个 InstanceRedoData 对象要负责服务实例注册操作重试，同时也要负责服务实例注销操作重试**。

我能想到大家可能还是有点迷糊了，接下来请让我举一个例子，假如客户端已经把一个服务实例注册到服务端了，这也就意味着 InstanceRedoData 失败重试对象中的 registered 标志为 true，而 unregistering 初始值默认为 fasle，所以现在 InstanceRedoData 对象中的 registered 为 true，unregistering 为 fasle，表示服务实例已注册到服务端，但是并没有执行注销操作；过了一会，客户端希望注销这个服务实例，但是在注销的过程中，客户端和服务端的连接出现问题了，那么在执行 requestToServer 方法之前，客户端只是把 InstanceRedoData 对象的 unregistering 成员变量设置为 true 了，表示服务实例正在注销，但是 requestToServer 方法并没有执行成功，反而是客户端和一个新的服务端建立了新的连接，这时候肯定要执行失败重试操作了，**然后客户端就会发现有一个 InstanceRedoData 对象，这个对象的 registered 是 true，unregistering 也是 true，这两个标志同时为 true 就表示有服务实例注册到服务端了，并且正在注销中。这就意味着有一个注销服务实例的操作没有执行成功，因为一旦注销成功，InstanceRedoData 对象的 registered 就会被设置为 fasle 了**。所以，客户端就会根据这两个标志的状况重新执行注销服务实力的操作。大家可以根据这个例子，再仔细品味品味我刚才分析的逻辑，当然，大家也可以先看看下面的代码块，因为说了这么多，我还没有给大家真正展示 InstanceRedoData 该怎么定义，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo.data;


//该类的对象表示的就是服务实例重做对象，封装了注册服务实例到服务端的一些信息，当客户端重新连接到服务端之后
//就会从当前类的对象中取出这些信息，把这个服务实例重新注册到服务端
//可与看待这个InstanceRedoData类继承了RedoData类
//实际上SubscriberRedoData类也会继承RedoData类，RedoData类才是核心，所以接下来我会为大家展示RedoData类的代码
public class InstanceRedoData extends RedoData<Instance> {

    protected InstanceRedoData(String serviceName, String groupName) {
        super(serviceName, groupName);
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/23
     * @方法描述：这个就是构建InstanceRedoData对象的方法，可以看到这个对象封装了一个服务实例的全部信息
     */
    public static InstanceRedoData build(String serviceName, String groupName, Instance instance) {
        InstanceRedoData result = new InstanceRedoData(serviceName, groupName);
        result.set(instance);
        return result;
    }
}
```

接下来就是 InstanceRedoData 类的父类，也就是 RedoData 类的内容，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo.data;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/23
 * @方法描述：该类的对象就是重做操作对象
 */
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


    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        RedoData<?> redoData = (RedoData<?>) o;
        return registered == redoData.registered && unregistering == redoData.unregistering && serviceName
                .equals(redoData.serviceName) && groupName.equals(redoData.groupName) && Objects
                .equals(data, redoData.data);
    }


    @Override
    public int hashCode() {
        return Objects.hash(serviceName, groupName, registered, unregistering, data);
    }


}
```

我知道现在大家看到这个 RedoData 类，一时间肯定有点懵，因为这个 RedoData 类中不止定义了 registered 和 unregistering 这两个成员变量，还定义了其他一些成员变量，其中 expectedRegistered 和 data 成员变量最为重要。**data 的作用还好说，它其实就是服务实例对象，也就是说，客户端注册服务实例到注册中新服务端时，会创建一个对应的 InstanceRedoData 操作重试对象，这个时候就会把服务实例对象封装到 InstanceRedoData 对象中**。至于 expectedRegistered 这个成员变量，必须结合具体情境来讲解，所以现在大家可以暂时忽略它，很快我就会举例为大家讲解。当然，**在上面的 RedoData 类中，有一个最核心的方法，那就是 getRedoType() 方法，该方法会根据操作重试对象 registered 和 unregistering 这两个成员变量的值来判断操作重试对象应该执行什么操作，可能是重新注册服务实例的操作，也可能是注销服务实例的操作等等**。光这么说大家可能还是很抽象，接下来我就举一个简单的例子，来为大家展示一下这个操作重试对象的具体用法。

在之前我就说过，判断客户端是否要对某些操作执行重试的突破口就是 NamingGrpcClientProxy 类的 requestToServer 方法，因为该方法就是把客户端的请求消息发送给服务端的方法，该方法一旦执行成功，就意味着客户端的某些操作执行成功了。所以我可以在该方法前后执行一些额外操作：**比如我可以在执行 requestToServer 方法之前创建操作重试对象，并且初始化操作重试对象中一些成员变量的值；在该方法执行之后，根据具体执行的操作以及执行结果，更新操作重试对象的 registered 和 unregistering 这两个成员变量的值。当然，我们还要把创建的操作重试对象缓存起来，只有缓存起来，客户端才能查看它这些对象内部成员变量的值，然后判断是否要执行重试操作。所以，我打算先在 NamingGrpcClientProxy 类中定义一个新的成员变量，就是一个 Map，我把这个 map 命名为 registeredInstances，专门存放为某个操作创建的操作重试对象**。我接下来就以服务实例注册到注册中心为例，给大家展示一下操作重试对象的具体用法。请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;






/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //其他内容省略

    //存放注册操作的重做对象的map
    private final ConcurrentMap<String, InstanceRedoData> registeredInstances = new ConcurrentHashMap<>();




   /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册服务实例到服务端的方法
     */
    @Override
    public void registerService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[REGISTER-SERVICE] {} registering service {} with instance {}", namespaceId, serviceName,
                instance);
        //判断要注册的服务实例是否为临时的
        if (instance.isEphemeral()) {
            //如果是临时的直接调用下面这个方法即可
            registerServiceForEphemeral(serviceName, groupName, instance);
        } else {
            //走到这里意味着要注册的服务实例是持久的，这时候调用下面这个方法即可
            //但在第一版本我还未给大家实现该方法，后面会重构完整
            //doRegisterServiceForPersistent(serviceName, groupName, instance);
        }
    }




    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册临时服务实例到服务端的方法
     */
    private void registerServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {
        
        //在这里执行一些操作，创建操作重试对象，然后把对象存放到registeredInstances这个map中
        
        //先得到服务名称和命名空间组成的键
        String key = NamingUtils.getGroupedName(serviceName, groupName);

        //创建操作重试对象，这时候重试对象的registered成员变量为初始值fasle
        InstanceRedoData redoData = new InstanceRedoData(serviceName, groupName);
        //设置要注册的服务实例
        redoData.set(instance);

        //把操作重试对象存放到registeredInstances这个map中
        registeredInstances.put(key, redoData);

        /执行真正的注册方法
        doRegisterService(serviceName, groupName, instance);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法同上，就是在该方法中创建了一个InstanceRequest请求，然后把请求发送给服务端了
     */
    public void doRegisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        //创建了InstanceRequest请求对象，这个对象中封装了各种参数，命名空间，服务名称，分组名称等等
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,NamingRemoteConstants.REGISTER_INSTANCE, instance);
        
        
        //在这里把请求发送给服务端，并且指定了要返回的对象类型
        //注意，这里要强调一点，如果该方法没有执行成功，是会抛出异常的，所以一旦执行不成功
        //就不会再执行该方法后面的代码了，也就不可能更新操作重试对象内部成员变量的值
        requestToServer(request, Response.class);


        //如果上面的requestToServer方法执行成功了，就意味着注册服务实例的请求发送给服务端，并且成功注册了
        //所以要把注册服务的操作重试对象的registered成员变量更新为true，意味着注册成功

        //先得到服务名称和命名空间组成的键
        String key = NamingUtils.getGroupedName(serviceName, groupName);

        //从map中得到操作重试对象
        InstanceRedoData redoData = registeredInstances.get(key);

        //更新操作重试对象成员变量的值
        //这个时候，操作重试对象的registered成员变量就更新为true了
        redoData.registered();
    }






     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法就是把请求发送给服务端的方法，发送请求之后要返回一个响应对象，该方法的第二个参数就指定了要返回的对象的类型
     */
    private <T extends Response> T requestToServer(AbstractNamingRequest request, Class<T> responseClass)
            throws NacosException {
        //定义一个局部变量，接收服务器响应
        Response response = null;
        try {
            //设置请求头，请求头中的信息最终会设置到请求对象中，发送给服务端
            //其实这里只是填充了一个应用名称，也就是appName
            request.putAllHeader(getSecurityHeaders());
            //判断用户是否定义了请求超时时间，如果requestTimeout小于0则意味着没有定义
            //那么发送请求的时候就不必使用超时时间
            //这里就会把请求发送给服务器，并且得到一个响应
            response = requestTimeout < 0 ? rpcClient.request(request) : rpcClient.request(request, requestTimeout);
            //判断响应是否成功，不成功则抛出异常
            if (ResponseCode.SUCCESS.getCode() != response.getResultCode()) {
                throw new NacosException(response.getErrorCode(), response.getMessage());
            }
            //判断响应类型是否与方法指定的参数类型匹配
            if (responseClass.isAssignableFrom(response.getClass())) {
                return (T) response;
            }
            //下面就是异常情况了，大家看看即可
            NAMING_LOGGER.error("Server return unexpected response '{}', expected response should be '{}'",
                    response.getClass().getName(), responseClass.getName());
            throw new NacosException(NacosException.SERVER_ERROR, "Server return invalid response");
        } catch (NacosException e) {
            throw e;
        } catch (Exception e) {
            throw new NacosException(NacosException.SERVER_ERROR, "Request nacos server failed: ", e);
        }
    }




    //其他内容省略
}
```

这次对 NamingGrpcClientProxy 类的重构，核心逻辑都在 registerServiceForEphemeral 和 doRegisterService 这两个方法中，**我们在 registerServiceForEphemeral 方法中为注册服务实例到注册中心的操作创建了对应的操作重试对象，并且把该对象的 registered 成员变量初始化为 false；然后在 doRegisterService 方法中等请求消息成功发送给服务端了，再把重做对象的 registered 成员变量更新为 true；这样一来，如果这个时候客户端来查看重做对象的 registered 成员变量，发现其为 true，就知道注册服务实例的操作已经成功了，也就没必要再执行操作重试了**。这就是操作重试对象完整的工作流程。

  

当然，服务实例注册的情况讲解完了，我可以稍微给大家延伸一下，简单讲讲服务实例注销的情况。这种情况很简单，其实就是在注销服务实例的时候，把对应的操作重试对象的 unregistering 更新为 true，表示该服务实例需要注销了，等到 requestToServer 方法成功执行完毕了，这时候就可以再把操作重试对象的 registered 更新为 false，表示服务实例没有注册到服务端。假如我们把服务实例注销的方法定义为 deregisterService，那么就可以在 NamingGrpcClientProxy 类中执行以下操作，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;






/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/10
 * @方法描述：grpc客户端，在第一版本代码中，我对这个类的内容做了大量删减，方法也做了很多简化
 */
public class NamingGrpcClientProxy extends AbstractNamingClientProxy {

    //其他内容省略

    //存放注册操作的重做对象的map
    private final ConcurrentMap<String, InstanceRedoData> registeredInstances = new ConcurrentHashMap<>();



    //注册服务实例的方法暂时省略






    //下面是注销服务实例的方法，当然，注销服务实力的功能我还没有给大家实现，在第六版本代码中才会实现
    //所以下面展示的方法并不会出现在第三版本代码中，大家在文章中看看即可
     @Override
    public void deregisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[DEREGISTER-SERVICE] {} deregistering service {} with instance: {}", namespaceId,
                serviceName, instance);
        if (instance.isEphemeral()) {
            //这里就是注销临时服务实例的方法
            deregisterServiceForEphemeral(serviceName, groupName, instance);
        } else {//这里是注销持久服务实例的操作
            doDeregisterServiceForPersistent(serviceName, groupName, instance);
        }
    }



    //注销临时服务实例的方法
    private void deregisterServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {
        
        //先得到服务名称和命名空间组成的键
        String key = NamingUtils.getGroupedName(serviceName, groupName);

        //从map中得到操作重试对象，因为注册服务实例的时候已经创建了操作重试对象
        //所以现在只需要从map中直接获得即可
        InstanceRedoData redoData = registeredInstances.get(key);

        //在这里把重试对象的unregistering更新为true，表示服务实例正在注销
        redoData.setUnregistering(true);

        //执行真正的注销服务实例的方法
        doDeregisterService(serviceName, groupName, instance);
        
    }



    //真正注销服务实力的方法
    public void doDeregisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.DE_REGISTER_INSTANCE, instance);
        //在这里把请求发送给服务器，执行服务实力注销操作
        requestToServer(request, Response.class);

        //操作成功之后
        //先得到服务名称和命名空间组成的键
        String key = NamingUtils.getGroupedName(serviceName, groupName);

        //从map中得到操作重试对象，因为注册服务实例的时候已经创建了操作重试对象
        //所以现在只需要从map中直接获得即可
        InstanceRedoData redoData = registeredInstances.get(key);
        //在unregistered()方法中，就会把redoData对象的registered更新为fasle
        //表示服务实例没有注册了
        redoData.unregistered();
    }


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法就是把请求发送给服务端的方法，发送请求之后要返回一个响应对象，该方法的第二个参数就指定了要返回的对象的类型
     */
    private <T extends Response> T requestToServer(AbstractNamingRequest request, Class<T> responseClass)
            throws NacosException {
        //具体内容省略
    }




    //其他内容省略
}
```

上面的代码块展示完毕之后，我相信大家都已经明白了操作重试对象是如何发挥作用的。如果说现在还有疑问困扰着大家，我相信一定是这个疑问：**那就是客户端什么时候判断操作重试对象中的标志，然后根据标志决定是否执行重试操作呢**？现在大家都知道了，为所有操作创建的操作重试对象都存放在 NamingGrpcClientProxy 的 registeredInstances 这个 map 中，客户端可以直接从这个 map 中查看每一个操作重试对象的重要成员变量的值，然后决定是否需要执行操作重试。但是，客户端应该什么时候去查看 registeredInstances 这个 map 中操作重试对象的成员变量呢？如果一个操作重试对象刚刚存放到 registeredInstances 中，客户端还没来得及执行 requestToServer 方法，真正执行目标操作呢，客户端就去 registeredInstances 中查看刚刚创建的操作重试对象的成员变量的值，这时候看有什么用呢？requestToServer 方法都没有执行呢，这就意味着操作肯定没有成功啊。所以，**客户端要想查看操作重试对象的成员变量的值，最好等到 requestToServer 方法执行完毕之后再查看**。requestToServer 方法执行完毕了，如果操作重试对象的成员变量的值还没有更新，那就意味着操作失败了，接下来应该操作重试了。这是我目前的思路，**但是在 nacos 中，判断操作重试的方法更简单直接，既然关键方法就在于这个 requestToServer 方法是否可以执行成功——大家应该还记得，客户端向服务端发送请求消息时有一个超时时间，如果 3 秒之内没有发送成功，就意味着客户端和服务端连接有问题，这时候就需要异步切换连接，和一个新的服务器建立新的连接——那就创建一个每 3 秒执行一次的定时任务，在这个定时任务中查看所有操作重试对象的成员变量的值，然后判断是否需要执行操作重试。如果客户端某个操作没有执行成功，并且达到了超时时间也没有发送成功，那么这个定时任务就一定可以扫描到有操作需要重试**。当然，光这样说还是不太直观，接下来我就以 nacos 中的编码思路，再次重构一下我们的程序。

## 引入 NamingGrpcRedoService 操作重试服务，创建 RedoScheduledTask 操作重试任务

相对于我们目前的程序来说，**实际上在 nacos 中有一个新的组件，那就是 NamingGrpcRedoService 类，这个类的对象提供的就是操作重试服务，并且在这个类中还定义了一个 RedoScheduledTask 定时任务，这个定时任务会每 3 秒执行一次，然后查看有哪些操作重试对象需要执行重试操作**。当 NamingGrpcRedoService 类引入之后，在上一小节我为大家展示的 NamingGrpcClientProxy 类就需要再次重构一下了，因为已经把 NamingGrpcRedoService 操作重试服务组件引入进来了，和操作重试相关的成员变量和方法都应该定义在这个组件中，**所以原本在 NamingGrpcClientProxy 类中的 registeredInstances 成员变量以及其他相关操作，我都重新定义在 NamingGrpcRedoService 类中了**。最后，只需要把 NamingGrpcRedoService 操作重试组件定义在 NamingGrpcClientProxy 类中，并且在 NamingGrpcClientProxy 类的构造方法中初始化 NamingGrpcRedoService 组件即可。现在大家应该也能渐渐明白为什么还要在 NamingClientProxyDelegate 类和 RpcClient 类之间再引入一个 NamingGrpcClientProxy 客户端代理类了吧？因为确实有一些额外的操作需要 NamingGrpcClientProxy 类的对象来执行。好了，接下来就请大家看看我重构之后的程序。

首先是新引入的 NamingGrpcRedoService 组件，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc.redo;





//这个类的对象就是重做服务对象，这个类实现了ConnectionEventListener监听器接口
//现在大家还不用知道这个监听器接口是干什么用的，耐心看下去，下一章就全清楚了
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

接下来就是 RedoScheduledTask 这个定时任务对象的内容，请看下面代码块。

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

最后是重构之后的 NamingGrpcClientProxy 类，请看下面代码块。

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





     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册服务实例到服务端的方法
     */
    @Override
    public void registerService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[REGISTER-SERVICE] {} registering service {} with instance {}", namespaceId, serviceName,
                instance);
        //判断要注册的服务实例是否为临时的
        if (instance.isEphemeral()) {
            //如果是临时的直接调用下面这个方法即可
            registerServiceForEphemeral(serviceName, groupName, instance);
        } else {
            //走到这里意味着要注册的服务实例是持久的，这时候调用下面这个方法即可
            //但在第一版本我还未给大家实现该方法，后面会重构完整
            //doRegisterServiceForPersistent(serviceName, groupName, instance);
        }
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：注册临时服务实例到服务端的方法
     */
    private void registerServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {
        //在第三版本中，这里多了一行代码，这行一调用，就会重建一个服务实例重做对象，这个对象会被存放到重做服务对象的map中
        redoService.cacheInstanceForRedo(serviceName, groupName, instance);
        doRegisterService(serviceName, groupName, instance);
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/10
     * @方法描述：该方法同上，就是在该方法中创建了一个InstanceRequest请求，然后把请求发送给服务端了
     */
    public void doRegisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        //创建了InstanceRequest请求对象，这个对象中封装了各种参数，命名空间，服务名称，分组名称等等
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.REGISTER_INSTANCE, instance);
        //在这里把请求发送给服务端，并且指定了要返回的对象类型
        requestToServer(request, Response.class);
        //这里就是把服务实例重做对象的注册标志设置为已注册，意味着已经把一个服务实例成功注册到服务端了
        redoService.instanceRegistered(serviceName, groupName);
    }





    //下面就是执行服务注销操作的方法，具体注释我就不添加了，和上面服务注册的注释基本一样，大家对比看看就成
    @Override
    public void deregisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[DEREGISTER-SERVICE] {} deregistering service {} with instance: {}", namespaceId,
                serviceName, instance);
        if (instance.isEphemeral()) {
            //注销临时服务实例
            deregisterServiceForEphemeral(serviceName, groupName, instance);
        } else {
            doDeregisterServiceForPersistent(serviceName, groupName, instance);
        }
    }

    private void deregisterServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {

        //在这里把重试对象的unregistering更新为true，表示服务实例正在注销
        redoService.instanceDeregister(serviceName, groupName);
        doDeregisterService(serviceName, groupName, instance);
        
    }
    
    /**
     * Execute deregister operation.
     *
     * @param serviceName service name
     * @param groupName   group name
     * @param instance    instance
     * @throws NacosException nacos exception
     */
    public void doDeregisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.DE_REGISTER_INSTANCE, instance);
        requestToServer(request, Response.class);
        //在这里就会把redoData对象的registered更新为fasle
        //表示服务实例没有注册了，并且已经注销完毕
        redoService.instanceDeregistered(serviceName, groupName);
    }



    //其他方法省略

}
```

随着以上三个代码块的展示，我想大家现在对 nacos 注册中心客户端操作重试功能的理解更深了。总的来说，**就是在执行某个操作之前，先创建这个操作的操作重试对象，然后把对象存放到 NamingGrpcRedoService 操作重试服务组件的 map 中，同时启动了一个每 3 秒就执行一次的 RedoScheduledTask 定时任务，这个定时任务会定期扫描存放了操作重试对象的 map，查看操作重试对象某些成员变量的值，然后判断是否应该让某些操作重试对象执行重试的操作**。大家可以再好好品味品味上面展示的代码块，代码中注释非常详细，所以我就不再赘述了。本来，到此为止，这一章的内容就该结束了，或者 nacos 客户端操作重试的功能就该结束了，但是课程进行到这里，我相信在大家心里产生的疑点更多了。

也许大家会十分不解：这个 NamingGrpcRedoService 操作重试服务究竟是在什么情况下提供操作充实服务的呢？在课程一开始，我就跟大家说这个 nacos 客户端的操作重试是在客户端与新的服务端连接重建之后，要执行相应的操作重试操作；但重构之后的程序呈现出来的根本不是这个效果，我为什么这么说呢？这一章显然是讲解不完了，剩下的内容和疑点就留到下一章解释吧。我建议大家目前先别去查看第三版本代码，等下一章我把围绕着这个操作重试功能的所有疑点都解释清楚了，大家再去看第三版本代码。好了诸位，我们下一章见！