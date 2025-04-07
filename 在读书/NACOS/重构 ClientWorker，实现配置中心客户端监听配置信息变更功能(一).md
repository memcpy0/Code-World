前面两章我已经为大家实现了 nacos 配置中心客户端向服务端发布、查询配置信息的功能了，这两个功能都比较简单，也没有那么重要，从文章中展示给大家的代码块就能看出来，大家学起来应该也很轻松。和前面两章实现的功能相比，本章要实现的功能就显得稍微复杂一点了，没错，我们本章要实现的配置中心客户端监听服务端配置变更的功能，从代码量上来说确实更多一点，所以显得稍微复杂一些。但是大家不必担心，它仅仅是代码量多一些，要执行的操作多一点，并不是意味着它难以理解。我可以非常负责任地跟大家保证，客户端监听配置信息变更功能的实现逻辑非常简单，很容易就能梳理清楚，如果大家已经把 hippo4j 动态线程池框架看完了，在实现 nacos 配置中心客户端监听配置信息变更的功能时，肯定会有一种似曾相识的感觉，在阅读本篇文章的过程中，这种感觉会越来越强烈。

  

## 为配置中心客户端定义监听配置信息变更的方法

现在请大家现在脑海里简单回顾一下，我们实现 hippo4j 动态线程池框架的时候，是怎么实现客户端监听服务端线程池信息变更的呢？其实非常简单，就是给动态线程池客户端定义了一个监听线程池信息变更的方法，还定义了一个监听器，调用该方法的时候把监听器对象传进去，这样一来，等待客户端监听到服务端配置发生变更之后，就会回调监听器中的方法，方便用户自己执行一些逻辑。现在我们要实现 nacos 配置中心客户端监听服务端配置信息变更的功能了，也可以定义一个监听方法和监听器，当然，这个监听方法肯定要先定义在配置中心客户端的 NacosConfigService 类中，因为这个类的对象就是专门用来和服务端打交道的，我已经把方法定义完毕了，请看下面代码块。

```
package com.cqfy.nacos.client.config;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:配置中心服客户端服务组件
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class NacosConfigService implements ConfigService {

    private static final Logger LOGGER = LogUtils.logger(NacosConfigService.class);

    //表示当前服务状态的标志
    //上线标志
    private static final String UP = "UP";

    //下线标志
    private static final String DOWN = "DOWN";

    //命名空间
    private String namespace;

    //客户端工作组件，这个组件提供了和服务端通信的功能
    //还提供了客户端监听服务端配置变更的功能，但是这个功能在第14版本代码中并没有用到
    private final ClientWorker worker;

    //配置文件过滤器管理器
    private final ConfigFilterChainManager configFilterChainManager;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:向指定的配置文件添加监听器的方法，也就是客户端监听配置文件变更的入口方法
     */
    @Override
    public void addListener(String dataId, String group, Listener listener) throws NacosException {
        //ClientWorker类的addTenantListeners()方法就是监听服务端指定配置信息的方法
        worker.addTenantListeners(dataId, group, Collections.singletonList(listener));
    }


    //省略该类的其他内容
}
```

从上面代码块中可以看到，我给 NacosConfigService 类添加了一个 addListener() 方法，该方法就是客户端用来监听服务端指定的配置信息的方法，并且我们还可以看到，我向该方法中传入了一个 Listener 监听器对象。当然，**NacosConfigService 服务组件只是操作的发起者，它发起的操作，只要是和配置中心服务端打交道的，最后都会被 ClientWorker 类的对象真正执行，因此我们还需要在 ClientWorker 类中定义一个 addTenantListeners() 方法**，也就是上面代码块中 addListener() 方法内展示的方法。

不过现在我们先不急着实现 ClientWorker 类的 addTenantListeners() 方法，还是老样子，我先给大家展示一个测试类，让大家看一看 nacos 配置中心客户端是怎么使用 NacosConfigService 对象监听服务端配置信息变更情况的。请看下面代码块。

```
package com.cqfy.nacos.example;

public class ConfigExample {

    public static void main(String[] args) throws NacosException, InterruptedException {
        //配置中心服务端地址
        String serverAddr = "localhost";

        //配置中心要发布的配置信息的dataId
        String dataId = "test";
        //配置信息所在分组
        String group = "DEFAULT_GROUP";

        //创建配置中心客户端需要的配置信息对象
        Properties properties = new Properties();
        properties.put("serverAddr", serverAddr);

        //在这里使用nacos工厂创建NacosConfigService对象
        //NacosConfigService类实现了ConfigService接口
        //我把NacosFactory工厂对象的内容也展示在当前代码块中了
        ConfigService configService = NacosFactory.createConfigService(properties);

        //客户端监听服务端指定的配置信息变更情况，监听器对象刻意由用户自己定义
        configService.addListener(dataId, group, new Listener() {

            //客户端监听到服务端配置信息发生变更之后，该方法就会被回调
            //方法参数configInfo就是从服务端传输过来的最新的配置信息
            @Override
            public void receiveConfigInfo(String configInfo) {

                //在这个方法中可以获得变更之后的最新的配置信息，那么给第三方框架动态刷新配置信息
                //的操作也就可以执行了
                System.out.println("receive:" + configInfo);
            }

            //如果该方法定义了执行器，那么监听器中的receiveConfigInfo()方法就会被该执行器异步执行
            @Override
            public Executor getExecutor() {
                return null;
            }
        });


    }
}
```

好了，随着上面代码块的展示，现在大家应该清楚了 nacos 配置中心客户端如何执行监听服务端指定配置信息的操作了。当然，现在我们所做的也只是表面功夫，内在功能还没有完全实现，接下来还是让我们继续把目光集中在 ClientWorker 类中，实现该类的 addTenantListeners() 方法，把配置中心客户端监听配置信息变更的功能完全实现了。那么，ClientWorker 类的 addTenantListeners() 方法究竟该如何实现呢？

  

如果只是考虑配置中心客户端监控服务端配置信息变更功能，那么这实现起来就太简单了，实现方式和 hippo4j 动态线程池框架没什么区别，**只需要定义一个监听服务端指定配置信息的请求对象，把客户端要监听的配置的重要信息封装在请求对象中，然后客户端直接把请求发送给服务端，服务端再定义一个对应的处理器用来处理该对象，也就是说服务端接收到了客户端要监听的配置信息，那么服务端就会在对应的配置信息发生变更之后，主动向客户端发送请求，通知客户端它监听的配置信息发生变更了，然后客户端再去服务端查询指定的配置信息，这样客户端就可以获得自己监听的最新的配置信息了**。这个流程大家应该都可以理解吧？实现起来也根本没什么难度，无非就是定义新的请求对象和响应对象而已，之后就是常规流程，按照常规实现即可。

但是，大家还是应该意识到，当我们要为某个框架实现某项功能时，确定这项功能的设计原理往往是最容易的，等到真正把设计原理落实到代码层面上，要考虑的细节就比较多了。比如说，我们都应该可以想到，**一旦 nacos 配置中心监听了服务端的某个配置信息，那么 nacos 客户端肯定就要从服务端把对应的配置信息缓存在自己内部**，这个大家都可以理解吧？**因为使用 nacos 配置中心客户端的程序肯定会根据配置中心客户端监控的配置信息的变更情况来动态刷新自己要使用的配置信息，这样一来，程序才能使用最新的配置信息运行**。所以，**nacos 配置中心客户端只要监控了服务端的某项配置信息，就要把对应的配置信息换存在自己内部，如果监控到服务端配置信息发生变更了，就要及时刷新自己缓存的配置信息，然后通知第三方程序动态刷新配置信息。而通知第三方程序动态刷新配置信息的操作，就可以定义在监听配置变更的监听器对象中**。如果大家都看过 hippo4j 动态线程池框架了，肯定就会觉得我这会儿非常啰嗦，总是在重复相同的知识点。我知道我自己也很啰嗦，但稍微啰嗦一点也不错，肯定有些朋友没有看过动态线程池框架。

好了， 还是让我们言归正传吧，既然 nacos 配置中心客户端要把监听的配置信息换存在本地，那该怎么缓存呢？缓存之后，nacos 配置中心客户端怎么才能判断自己缓存的配置信息确实发生变更了呢？那配置信息真的变更之后，配置中心客户端怎么回调用户定义的监听器中的方法呢？我一连串提出了三个问题，也许会让大家有点措手不及，但实际上这三个问题非常容易就能解决**。比如我可以先定义一个 CacheData 类，用这个类的对象来将从服务端获得配置信息缓存在客户端内部，每一个 CacheData 对象都保存一份配置信息**。这个 CacheData 类可以先定义成下面这样，请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/9/2
 * @Description:客户端缓存配置信息的对象
 */
public class CacheData {

    //配置信息Id
    public final String dataId;

    //所在组
    public final String group;

    //租户信息，其实就是配置信息所属的命名空间
    public final String tenant;

    //配置信息
    private volatile String content;

    //配置信息类型
    private String type;

    //当前CacheData对象是否被废弃的标志，其实就是缓存的配置信息是否被废弃的标志
    //被废弃就不能再使用了
    private volatile boolean isDiscard = false;


    //get、set
}
```

上面代码块展示的就是我们目前定义完毕的 CacheData 类，这个类的对象就是用来在客户端内部存储配置信息的，这一点从 CacheData 类内部定义的成员变量就可以看出来，所以我就不再赘述了。当然，我们目前实现的 CacheData 类也只有保存从服务端获取的配置信息的功能，大家可别忘了，还有两个问题没解决呢，配置中心可以存储配置信息之后，nacos 配置中心客户端怎么才能判断自己缓存的配置信息确实发生变更了呢？而且配置信息真的变更之后，配置中心客户端怎么回调用户定义的监听器中的方法呢？这时候就需要对刚才定义的 CacheData 类进行一番重构了。

在重构之前，让我们先把判断配置信息是否发生变更的条件明确一下，按照常规方式来说，**肯定是把配置信息的内容本身进行处理，对其进行计算得到一个 MD5(MD5 的相关知识我就不介绍了，大家应该都清楚) 字符串。当客户端从服务端获得了变更之后的配置信息之后，就可以计算最新的配置信息的 MD5 的值，如果最新配置信息的 MD5 的值和客户端原本保存的旧的配置信息的 MD5 的值不一致，那就意味着配置信息确实是发生变更了**。当客户端感知到配置信息确实发生变更了，就可以刷新本地保存的配置信息，也就是把 CacheData 对象存储的配置信息更新一下，然后就可以调用监听该配置信息变更的监听器对象中的方法了。所以，按照这个分析，显然 CacheData 对象是要持有自己保存的配置信息的监听器对象，当然，也可能是多个监听器对象，因为用户很可能为某一个配置信息定义多个监听器。因此，我**们也应该在 CacheData 类中定义一个监听器集合，这个集合就用来存放自己保存的配置信息的监听器，等到配置信息发生变化之后，就可以直接在 CacheData 对象中调用自己保存的监听器对象中的方法**，于此同时，还要再定义一个向 CacheData 对象添加对应监听器的方法。这个大家应该也能理解吧？所以，接下来就请大家看一下我按照这个思路重构完毕的 CacheData 类，请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/9/2
 * @Description:客户端缓存配置信息的对象
 */
public class CacheData {

    //配置信息Id
    public final String dataId;

    //所在组
    public final String group;

    //租户信息，其实就是配置信息所属的命名空间
    public final String tenant;

    //配置信息
    private volatile String content;

    //配置信息类型
    private String type;

    //当前CacheData对象是否被废弃的标志，其实就是缓存的配置信息是否被废弃的标志
    //被废弃就不能再使用了
    private volatile boolean isDiscard = false;

    //配置信息的md5
    private volatile String md5;

    //是否正在初始化标志
    private volatile boolean isInitializing = true;

    //存储该配置信息监听器的集合，这里我要给大家解释一下
    //用户定义的监听器对象并不是直接以Listener对象的方式保存在CacheData对象内部
    //而是会被包装成一个ManagerListenerWrap对象，存储在下面这个集合成员变量中
    private final CopyOnWriteArrayList<ManagerListenerWrap> listeners;

    //当前CacheData对象存储的配置信息是否和服务端一致标志
    private final AtomicBoolean isConsistentWithServer = new AtomicBoolean();


    //构造方法
    public CacheData(String dataId, String group) {
        this(configFilterChainManager, envName, dataId, group, TenantUtil.getUserTenantForAcm());
    }


    //构造方法
    public CacheData(String dataId, String group,String tenant) {
        if (null == dataId || null == group) {
            throw new IllegalArgumentException("dataId=" + dataId + ", group=" + group);
        }
        
        this.dataId = dataId;
        this.group = group;
        this.tenant = tenant;
        this.listeners = new CopyOnWriteArrayList<>();
        this.isInitializing = true;
        this.md5 = getMd5String(this.content);
    }
    

    //根据配置信息本身的内容得到MD5字符串的方法
    public static String getMd5String(String config) {
        return (null == config) ? Constants.NULL : MD5Utils.md5Hex(config, Constants.ENCODE);
    }


    //更新当前CacheData对象存储的配置信息的方法
    public void setContent(String content) {
        this.content = content;
        //在该方法中会把配置信息的最新md5值一起更新了
        this.md5 = getMd5String(this.content);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:给当前CacheData对象添加配置信息监听器的方法
     */
    public void addListener(Listener listener) throws NacosException {
        if (null == listener) {
            throw new IllegalArgumentException("listener is null");
        }
        //监听器包装器，这个监听器包装对象也存放了配置文件的md5的值
        ManagerListenerWrap wrap = new ManagerListenerWrap(listener, md5);
        //把监听器包装对象添加到listeners集合中
        if (listeners.addIfAbsent(wrap)) {
            LOGGER.info("[{}] [add-listener] ok, tenant={}, dataId={}, group={}, cnt={}", envName, tenant, dataId,group, listeners.size());
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:校验CacheData对象存储的最新的md5和其监听器对象存储的md5的值是否相同，不相同则意味着配置信息发生了变更
     * 要执行对应监听器中的回调方法了
     */
    void checkListenerMd5() {
        for (ManagerListenerWrap wrap : listeners) {
            //判断CacheData对象持有的最新的MD5的值是否和每一个监听器包装对象持有的一致
            //如果不一致，就意味着配置信息发生变更了，那么监听器对象中的方法就要被回调
            if (!md5.equals(wrap.lastCallMd5)) {
                //在这里通知监听器执行回调方法
                safeNotifyListener(wrap);
            }
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:通知CacheData对象的监听器执行回调方法的方法
     */
    private void safeNotifyListener(ManagerListenerWrap listenerWrap) {

        //以下执行监听器中方法的操作经过了很大的简化，大家可以先通过文章理解每一个组件的功能以及程序的执行流程，
        //然后再去我提供的第十五版本代码中查看完整版代码。我之所以不在文章中展示，是因为代码逻辑非常非常简单
        //全部展示出来只会让文章变得臃肿，大家看了第十五版本代码中的内容后只会觉得我在水字数而已
        
        //从监听器包装器中得到真正的监听器对象
        final Listener listener = listenerWrap.listener;

        //调用监听器的回调方法，把CacheData对象保存的最新的配置信息交给监听器对象使用
        listener.receiveConfigInfo(content);
        //为监听器更新最新的md5的值
        listenerWrap.lastCallMd5 = md5;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:监听器包装类
     */
    private static class ManagerListenerWrap {

        //当前监听器是否正在执行通知操作，也就是执行回调方法
        boolean inNotifying = false;

        //真正的监听器对象
        final Listener listener;

        //md5
        String lastCallMd5 = Constants.NULL;

        //配置信息
        String lastContent = null;


        ManagerListenerWrap(Listener listener) {
            this.listener = listener;
        }


        ManagerListenerWrap(Listener listener, String md5) {
            this.listener = listener;
            this.lastCallMd5 = md5;
        }


        ManagerListenerWrap(Listener listener, String md5, String lastContent) {
            this.listener = listener;
            this.lastCallMd5 = md5;
            this.lastContent = lastContent;
        }


        @Override
        public boolean equals(Object obj) {
            if (null == obj || obj.getClass() != getClass()) {
                return false;
            }
            if (obj == this) {
                return true;
            }
            ManagerListenerWrap other = (ManagerListenerWrap) obj;
            return listener.equals(other.listener);
        }


        @Override
        public int hashCode() {
            return super.hashCode();
        }
    }

    

    

    //get、set方法省略
}
```

从上面代码块中可以看到，我给 CacheData 类定义了两个非常关键的方法，**一个是 addListener() 方法，该方法就是用来把用户自己定义的配置信息的监听器添加到保存该配置文件信息的 CacheData 对象中的；另一个非常重要的方法就是 checkListenerMd5() 方法，该方法一旦被调用，就意味着 CacheData 对象存储的配置信息可能发生了变更，这时候就会执行用户定义的监听器对象中的方法**。这些逻辑大家应该可以理解吧？CacheData 类中的其他内容我就不再为大家讲解了，上面代码块中的注释非常详细，大家就自己简单看看吧。实际上在 CacheData 类中，也就 addListener() 和 checkListenerMd5() 这两个方法比较重要，当然，文章中展示的代码确实做了一些简化，但就算大家去看我提供的第十五版本代码，大家也会发现我提供的代码中的 CacheData 类中的核心方法也就是这些了，再加上 safeNotifyListener() 方法吧，也就这三个核心方法而已。

好了，还是让我们回到正题吧，现在我们已经知道了，如果我们想让 nacos 配置中心客户端保存某个配置信息，那就创建一个对应的 CacheData 对象即可，把配置文件信息设置到创建好的 CacheData 对象中时，该配置信息的 MD5 字符串就计算完毕了。如果我们希望客户端可以监听该配置信息的变更情况，在该配置信息变更之后执行一些用户自定义的操作，那就可以把用户创建的监听器对象也交给 CacheData 对象来使用，只需要调用 addListener 类的 addListener() 方法即可，也就是说，**只有先创建了 CacheData 对象，才能向该对象中添加对应的监听器对象**。这个逻辑大家可以理解吧？如果大家理解了这个逻辑，那接下来就请大家想一想，nacos 配置中心客户端什么时候才会主动创建 CacheData 对象呢？创建 CacheData 对象就意味着配置中心客户端向把某个配置信息保存在自己内部，这个我们刚才已经分析过了，只有配置中心客户端想监听服务端某个配置信息变更情况时，才会从服务端获取对应的配置信息，然后保存在自己内部，这就意味着，**在客户端向服务端执行配置监听的操作时，才会为监听的配置信息创建一个 CacheData 对象，这个对象就会用来保存从服务端获取到的配置信息**。这个大家应该可以理解吧？很好，那接下来就是比较重要的问题了，nacos 配置中心客户端怎么才能执行向服务端监听指定配置信息的操作呢？

如果只关注配置中心客户端的 NacosConfigService 服务组件，那么客户端向服务端监听指定配置信息的功能已经实现了，**我们已经在 NacosConfigService 类中定义了一个 addListener() 方法，该方法就是客户端向服务端监听指定配置信息的入口方法**。但是我们又都知道，客户端向服务端发起的操作，真正的执行者其实是 ClientWorker 类，并且在 NacosConfigService 类的 addListener() 方法内部又调用了 ClientWorker 类的 addTenantListeners() 方法，而 ClientWorker 类的 addTenantListeners() 方法我们还没有实现。不过实现的思路已经有了，刚才不是分析了吗？**在客户端向服务端执行配置监听的操作时，才会为监听的配置信息创建一个 CacheData 对象，这个对象就会用来保存从服务端获取到的配置信息**。**而 ClientWorker 类的 addTenantListeners() 方法就是客户端向服务端监听指定配置信息时真正要执行的方法**，所以现在大家多少也能猜到 ClientWorker 类的 addTenantListeners() 方法的实现逻辑了吧？**就是在这个方法中为要监听的配置信息创建对应的 CacheData 对象，然后从服务端查询要监听的配置信息，把从服务端获得的当前配置信息保存在 CacheData 对象中之后，就可以向服务端发送监听该配置信息的请求，真正向服务端执行监听指定配置信息的操作了**。大家可以仔细品味品味这个逻辑，然后就请大家看看下面的代码块，我把已经实现完毕的 ClientWorker 类的 addTenantListeners() 方法展示在下面代码块中了，请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:客户端工作组件，在以前它使用长轮询机制监听配置文件变更情况，但在2.x版本之后，就不再使用长轮询，而是长连接了
 */
public class ClientWorker implements Closeable {


    //当前客户端工作组件的唯一标识
    private final String uuid = UUID.randomUUID().toString();


    //配置中心客户端
    private final RpcClient agent;

    //租户信息
    String tenant;


    //存储配置文件监听对象的map，key就是dataId, group, tenant的组合键，value就是监听对象本身
    //也就是一个配置信息对应一个CacheData对象
    private final AtomicReference<Map<String, CacheData>> cacheMap = new AtomicReference<>(new HashMap<>());



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:构造方法，这个方法其实是伪代码，后面会对其进行重构
     */
    @SuppressWarnings("PMD.ThreadPoolCreationRule")
    public ClientWorker(ServerListManager serverListManager,final NacosClientProperties properties) throws NacosException {

        //得到命名空间
        this.tenant = properties.getProperty(PropertyKeyConst.NAMESPACE);
        
        //在这里创建配置中心客户端
        agent = new RpcClient(properties, serverListManager);
       
        //启动配置中心客户端
        agent.start();

    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:为租户添加监听器的方法
     */
    public void addTenantListeners(String dataId, String group, List<? extends Listener> listeners)throws NacosException {
        //对组名进行判空
        group = blank2defaultGroup(group);

        //得到与该配置文件对应的CacheData对象
        CacheData cache = addCacheDataIfAbsent(dataId, group, tenant);

        //把监听器添加到CacheData对象中
        synchronized (cache) {
            for (Listener listener : listeners) {
                cache.addListener(listener);
            }

            //设置CacheData对象没有被废弃标志
            cache.setDiscard(false);
            //设置对象存储的配置信息和服务端不一致标志
            cache.setConsistentWithServer(false);
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:得到指定配置文件的CacheData对象的方法
     */
    public CacheData getCache(String dataId, String group, String tenant) {
        if (null == dataId || null == group) {
            throw new IllegalArgumentException();
        }//GroupKey.getKeyTenant(dataId, group, tenant)就是得到这三个信息的组合key的方法
        return cacheMap.get().get(GroupKey.getKeyTenant(dataId, group, tenant));
    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:为指定的配置文件创建CacheData缓存数据对象的方法，监听配置文件是否发生变更的监听器就存放在这个CacheData对象中
     * 该方法和上面方法的逻辑一样，只不过上面的方法没有指定租户信息，如果用户没有配置的话，就使用默认的租户信息
     * 而该方法指定了租户信息，我就不重复添加注释了，大家自己看看就成
     */
    public CacheData addCacheDataIfAbsent(String dataId, String group, String tenant) throws NacosException {
        //先判断成员变量cacheMap中是否缓存了对应的CacheData对象
        CacheData cache = getCache(dataId, group, tenant);
        if (null != cache) {
            return cache;
        }
        //如果没有缓存则为该配置信息创建一个CacheData对象
        //这里的到组合key
        String key = GroupKey.getKeyTenant(dataId, group, tenant);
        //创建CacheData对象
        cache = new CacheData(dataId, group, tenant);
        //从服务端查询当前配置信息
        ConfigResponse response = getServerConfig(dataId, group, tenant, 3000L, false);
        //把配置信息设置到CacheData对象中，同时把MD5的值更新了
        //这里有一个问题，请大家思考一下，这里直接从服务端获得的配置信息还是加密过后的配置信息
        //并没有被解密，难道就直接把没有解密的配置信息存储到CacheData对象中吗？大家可以带着这个疑问
        //去看我提供的第十五版本代码
        cache.setContent(response.getContent());
        Map<String, CacheData> copy = new HashMap<>(this.cacheMap.get());
        copy.put(key, cache);
        //更新cacheMap成员变量
        cacheMap.set(copy);
        return cache;

        //之后的操作呢？客户端还没有向服务端发送监听配置信息的请求呢？还没有真正向服务端执行配置监听操作呢，这是怎么回事呢？
    }

    

    
    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端从服务端获取指定配置信息的方法，ConfigResponse这个响应中就封装了从服务端获得的配置信息
     */
    public ConfigResponse getServerConfig(String dataId, String group, String tenant, long readTimeout, boolean notify)throws NacosException {
        //创建向服务端发送的查询配置信息请求的对象
        ConfigQueryRequest request = ConfigQueryRequest.build(dataId, group, tenant);
        //使用客户端向服务端发送查询请求，并且得到从服务端返回的响应对象
        ConfigQueryResponse response = (ConfigQueryResponse) requestProxy(request);
        //创建一个响应对象，用来封装配置信息结果，这个响应对象是在客户端内部使用的，要被过滤器处理的响应对象
        ConfigResponse configResponse = new ConfigResponse();
        //判断服务端响应是否成功
        if (response.isSuccess()) {
            //把响应的配置文件信息设置到客户端自己使用的响应对象中
            configResponse.setContent(response.getContent());
            //设置配置文件的类型
            configResponse.setConfigType(configType);
            //从ConfigQueryResponse对象中得到服务端返回的配置文件加密的密钥
            String encryptedDataKey = response.getEncryptedDataKey();
            //把密钥设置到客户端自己使用的响应对象中
            configResponse.setEncryptedDataKey(encryptedDataKey);
            //返回客户端自己使用的响应对象
            return configResponse;
        } else {

            //处理服务端错误响应的操作就省略了
        }
        
    }

    

    //向服务端发送请求的方法，这个方法其实是伪代码，源码中并不是这样，下一章我就会为大家重构
    private Response requestProxyRequest request) throws NacosException {

        //在这里把请求发送给服务端，默认超时时间
        return agent.request(request, 3000);
    }


    //省略该类的其他内容

}
```

从上面代码块中，大家可以看到，我们已经实现了 ClientWorker 类 的 addTenantListeners() 方法，而且就是按照刚才分析的思路实现的，所以我就不再帮助大家梳理上面代码块中的逻辑了。唯一要关注的一点就是，**我们并没有在 addTenantListeners() 方法中真的向服务端发送监听配置信息的请求**，这一点肯定会让大家觉得奇怪。当然，我确实还没有把对应的请求对象定义出来，但眼下还不用关心这个，因为有一个更棘手的问题摆在我们面前，让我们现在无法直接使用客户端向服务端发送监听指定配置信息的请求。

## 重构 ClientWorker 中的客户端对象

请大家仔细思考思考，按照我们的理解，按照常规逻辑来说，**一旦配置中心客户端向服务端监听了指定的配置信息，那么服务端在发现自己存储的配置信息发生变更后，就要及时通知客户端监听的配置信息变更了，然后让客户端向服务端查询自己监听的最新的配置信息**，大家对这个流程肯定没意见吧？如果大家都认同这个逻辑，那么我们就可以这样认为：**目前我们实现的配置中心客户端只是向服务端监听了一个配置信息，服务端在发现该配置信息变更之后，就会主动向客户端发送一个通知客户端配置变更的请求。那如果某个客户端向服务端监听了两个配置信息，那服务端在发现两个配置信息发生变更后，就会向客户端发送两个请求，通知客户端它监听的配置信息发生变更了**。这个逻辑大家应该也认可吧？好的，如果大家认可这个逻辑，那就请大家再进一步思考一个极端情况，**假如有某个配置中心客户端向配置中心监听了 1 万个配置信息，那么当服务端的一万个配置信息都发生变更时，服务端就会向客户端发送一万个请求，通知客户端它监听的配置信息发生变更了**。一个客户端同时处理一万个请求，对工作效率难免会有些影响，如果客户端向服务端监听了两万个配置信息，那么客户端在同一时间要处理的服务端发送过来的配置变更请求会更多。这个时候，客户端处理请求的效率就大大降低了，那这个问题该怎么解决呢？

这时候我就要为大家引入一个新的概念，也是 nacos 配置中心客户端中的概念，**那就是使用客户端进行配置信息监控分片，换句话说，就是为配置中心定义多个 RpcClient 客户端对象，在执行监控配置信息变更操作时，每一个 RpcClient 客户端对象负责一定数量的配置信息**。也许大家还是对我刚才提出的概念感到困惑，接下来就让我为大家详细解释一下。

实际上，在 nacos 配置中心客户端是这么做的：**nacos 配置中心客户端会在自己内部创建多个 RpcClient 客户端对象，并且为每一个 RpcClient 客户端对象定义一个编号，编号从 0 开始，这些 RpcClient 客户端对象都会被存储起来；如果配置中心客户端要对服务端执行发布配置信息、查询配置信息、移除配置信息的操作，那么就会都是用编号为 0 的 RpcClient 对象向服务端发送对应的请求；如果客户端执行的是监控配置信息的操作，那就会开始对监控的配置信息开始计数，每监控 3000 个配置信息就会创建一个新的 RpcClient 客户端对象，也就是说每 3000 配置信息共享一个 RpcClient 客户端对象，并且客户端对象的编号是从 1 开始递增的，等到客户端要监控第 3001 个配置信息时，就会创建一个新的 RpcClient 客户端对象，并且把这个 RpcClient 客户端对象的编号定义为 2**。再使用多个 RpcClient 客户端对象对要监控的配置信息进行数据分片之后，每一个 RpcClient 客户端对象处理的服务端主动发送过来的请求就是有限的了，这样一来，程序的工作效率不就大大提高了吗？大家可以仔细品味品味我刚才为大家分析的逻辑，如果这些逻辑都清楚了，那接下来就不用说了，肯定要对我们之前为配置中心客户端构建的 RpcClient 通信方式进行重构了。

**这时候我就要对我们之前为配置中心客户端定义的 RpcClient 对象做一层包装，也就是引入一个新的类，我把名字定义为 ConfigRpcTransportClient，翻译过来就是配置中心客户端数据传输器的意思**。在 nacos 配置中心客户端源码中就是使用 ConfigRpcTransportClient 类的对象向服务端执行了发布配置、查询配置、监听配置等等操作，当然，**ConfigRpcTransportClient 类的对象肯定持有者真正的 RpcClient 客户端对象，因为最终客户端发送请求给服务端的操作是 RpcClient 客户端对象执行的。实际上也正是这样，创建多个 RpcClient 客户端对象、然后对配置信息进行分片的操作就是在 ConfigRpcTransportClient 类中定义的**。根据刚才的分析，大家肯定能想到了，这个类的对象将会取代我们之前定义的 RpcClient 对象成为 ClientWorker 类的成员变量，而在 nacos 配置中心源码中，这个 ConfigRpcTransportClient 类实际上 ClientWorker 的内部类。所以我们真正要重构的其实是这个 ClientWorker 类，我把重构完毕的 ClientWorker 类展示在下面了，该类中的每一个方法大家都要仔细阅读，看看哪些地方发生了变更。请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:客户端工作组件，在以前它使用长轮询机制监听配置文件变更情况，但在2.x版本之后，就不再使用长轮询，而是长连接了
 */
public class ClientWorker implements Closeable {


    //当前客户端工作组件的唯一标识
    private final String uuid = UUID.randomUUID().toString();

    //租户信息
    String tenant;

    //存储配置文件监听对象的map，key就是dataId, group, tenant的组合键，value就是监听对象本身
    //也就是一个配置信息对应一个CacheData对象
    private final AtomicReference<Map<String, CacheData>> cacheMap = new AtomicReference<>(new HashMap<>());

    //配置中心客户端数据传输器
    private final ConfigRpcTransportClient agent;


    //存放每一个taskId对应计数器的集合
    private final List<AtomicInteger> taskIdCacheCountList = new ArrayList<>();


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:构造方法，这个方法其实是伪代码，后面会对其进行重构
     */
    @SuppressWarnings("PMD.ThreadPoolCreationRule")
    public ClientWorker(ServerListManager serverListManager,final NacosClientProperties properties) throws NacosException {

        //得到命名空间
        this.tenant = properties.getProperty(PropertyKeyConst.NAMESPACE);
        
        //在这里创建配置中心客户端数据传输器
        agent = new ConfigRpcTransportClient(properties, serverListManager);
       
        //启动配置中心客户端数据传输器，其实就是启动了配置中心客户端
        agent.startInternal();

    }


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端从服务端获取指定配置信息的方法
     */
    public ConfigResponse getServerConfig(String dataId, String group, String tenant, long readTimeout, boolean notify)
            throws NacosException {
        if (StringUtils.isBlank(group)) {
            group = Constants.DEFAULT_GROUP;
        }//这里就是直接调用客户端数据传输器的queryConfig()方法了
        return this.agent.queryConfig(dataId, group, tenant, readTimeout, notify);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:客户端向服务端发布配置信息的方法
     */
    public boolean publishConfig(String dataId, String group, String tenant, String appName, String tag, String betaIps,
                                 String content, String encryptedDataKey, String casMd5, String type) throws NacosException {
        //这里就是直接调用客户端数据传输器的publishConfig()方法了
        return agent.publishConfig(dataId, group, tenant, appName, tag, betaIps, content, encryptedDataKey, casMd5, type);
    }


    //下面就是ConfigRpcTransportClient内部类的内容


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:配置中心客户端数据传输器的子类，这个类的对象就相当于配置中心客户端
     * 所有和服务端交互的操作，以及和服务端通信的操作，都是由这个类的对象执行的
     */
    public class ConfigRpcTransportClient extends ConfigTransportClient {

        //租户信息，其实就是命名空间
        String tenant;

        //服务器地址列表管理器
        final ServerListManager serverListManager;

        //客户端配置信息对象
        final Properties properties;


        //构造方法
        public ConfigRpcTransportClient(NacosClientProperties properties, ServerListManager serverListManager) {
            this.tenant = properties.getProperty(PropertyKeyConst.NAMESPACE);
            this.serverListManager = serverListManager;
            this.properties = properties.asProperties();
        }


        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/1
         * @Description:得到客户端和服务端连接类型的方法
         */
        private ConnectionType getConnectionType() {
            return ConnectionType.GRPC;
        }


        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/2
         * @Description:启动客户端数据传输器的方法，其实就是启动客户端向服务端执行的监听配置文件变更情况的定时任务的方法
         */
        @Override
        public void startInternal() {

            //该方法非常重要，是客户端监听配置变更功能中最重要的方法
            //但该方法暂时不做实现
        }


        /**
        * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
        * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
        * @Date:2024/9/1
        * @Description:初始化创建完毕的RPC客户端的方法
        */
        private void initRpcClientHandler(final RpcClient rpcClientInner) {



            /**
             * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
             * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
             * @Date:2024/9/2
             * @Description:向RPC客户端中注册处理通知配置变更请求的处理器，当服务端主动向客户端发送了ConfigChangeNotifyRequest配置变更请求后
             * 当前注册的处理器就会处理这个请求
             */
            rpcClientInner.registerServerRequestHandler((request, connection) -> {
                //判断请求类型是否为ConfigChangeNotifyRequest
                if (request instanceof ConfigChangeNotifyRequest) {
                    //进行强转
                    ConfigChangeNotifyRequest configChangeNotifyRequest = (ConfigChangeNotifyRequest) request;
                    //记录配置变更日志信息
                    LOGGER.info("[{}] [server-push] config changed. dataId={}, group={},tenant={}",
                            rpcClientInner.getName(), configChangeNotifyRequest.getDataId(),
                            configChangeNotifyRequest.getGroup(), configChangeNotifyRequest.getTenant());
                    //得到组合键
                    String groupKey = GroupKey.getKeyTenant(configChangeNotifyRequest.getDataId(),
                            configChangeNotifyRequest.getGroup(), configChangeNotifyRequest.getTenant());
                    //得到对应的CacheData对象
                    CacheData cacheData = cacheMap.get().get(groupKey);
                    if (cacheData != null) {
                        synchronized (cacheData) {
                            //在这里设置当前CacheData对象接收到了服务端发送过来的配置变更通知标志
                            //注意，这里只是设置了一个标志，并没有对当前CacheData对象进行配置变更，刷新当前CacheData对象缓存的数据
                            //等到客户端下一次执行配置变更监听的定时任务时，就会根据这个标注主动去服务端查询最新的配置信息
                            //然后刷线CacheData对象
                            cacheData.getReceiveNotifyChanged().set(true);
                            //因为配置发生变更了，所以肯定要设置CacheData当前缓存的配置文件和服务端不一致标志
                            //等到后面客户端执行配置变更监听定时任务时，发现配置没有发生变更，就会把这个标志更新为true了
                            cacheData.setConsistentWithServer(false);
                            //通知客户端向服务端执行下一次配置监听定时任务，其实就是向listenExecutebell阻塞队列中添加了一个变更信号
                            notifyListenConfig();
                        }

                    }
                    return new ConfigChangeNotifyResponse();
                }
                return null;
            });


            //向该客户端中设置服务端地址列表管理器，这样一来，客户端就知道可以和哪些配置中心服务器建立连接和通信了
            rpcClientInner.serverListFactory(new ServerListFactory() {


                @Override
                public String genNextServer() {
                    return ConfigRpcTransportClient.super.serverListManager.getNextServerAddr();
                }


                @Override
                public String getCurrentServer() {
                    return ConfigRpcTransportClient.super.serverListManager.getCurrentServerAddr();
                }


                @Override
                public List<String> getServerList() {
                    return ConfigRpcTransportClient.super.serverListManager.getServerUrls();
                }
            });

        }



        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/1
         * @Description:得到一个RPC客户端的方法，这里大家会看到方法参数中传进去了一个taskId，在第14版本代码中，这个taskId就是0
         * 等到了第15版本代码的时候，大家就会知道这个taskId的真正用法
         */
        private RpcClient ensureRpcClient(String taskId) throws NacosException {
            synchronized (ClientWorker.this) {

                //得到当前要创建的RPC客户端的标签信息，getLabels()方法我就不在文章中为大家展示了
                Map<String, String> labels = getLabels();
                Map<String, String> newLabels = new HashMap<>(labels);
                //把当前执行的任务Id设置到标签中，在第14版本中，这里的task就是0
                newLabels.put("taskId", taskId);
                //创建一个RPC客户端对象，这个客户端的key就是uuid + "_config-" + taskI，value就是创建的客户端对象本身
                //key-value会存放到RpcClientFactory工厂对象的map成员变量中
                //在RpcClientFactory.createClient()方法中会先判断一下taskId的客户端对象是否创建了
                //如果已经创建了就不会再创建了
                RpcClient rpcClient = RpcClientFactory.createClient(uuid + "_config-" + taskId, getConnectionType(),
                        newLabels, RpcClientTlsConfig.properties(this.properties));
                //判断刚刚创建完毕的客户端对象是否处于等待初始化的状态
                if (rpcClient.isWaitInitiated()) {
                    //如果是这个状态就初始化该客户端
                    initRpcClientHandler(rpcClient);
                    //设置租户信息
                    rpcClient.setTenant(getTenant());
                    //启动客户端，然后客户端就会和配置中心服务端建立连接了
                    rpcClient.start();
                }
                return rpcClient;
            }
        }

        

        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/1
         * @Description:得到一个当前正在运行的RPC客户端的方法
         */
        RpcClient getOneRunningClient() throws NacosException {
            //这里把taskId设置成0了，其实在配置中心客户端中，从服务端查询配置信息，发布配置信息到服务端，以及移除配置信息
            //使用的都是同一个RPC客户端对象，就是taskId为0的这个客户端对象
            return ensureRpcClient("0");
        }



        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/1
         * @Description:查询指定配置文件信息的方法
         */
        public ConfigResponse queryConfig(String dataId, String group, String tenant, long readTimeouts, boolean notify) throws NacosException {
            //创建向服务端发送的查询配置信息请求的对象
            ConfigQueryRequest request = ConfigQueryRequest.build(dataId, group, tenant);
            
            //得到一个正在运行的客户端，这里就是得到taskId为0的RPC客户端
            RpcClient rpcClient = getOneRunningClient();
            
            //使用客户端向服务端发送查询请求，并且得到从服务端返回的响应对象
            ConfigQueryResponse response = (ConfigQueryResponse) requestProxy(rpcClient,request);
            //创建一个响应对象，用来封装配置信息结果，这个响应对象是在客户端内部使用的，要被过滤器处理的响应对象
            ConfigResponse configResponse = new ConfigResponse();
            //判断服务端响应是否成功
            if (response.isSuccess()) {
                //把响应的配置文件信息设置到客户端自己使用的响应对象中
                configResponse.setContent(response.getContent());
                //设置配置文件的类型
                configResponse.setConfigType(configType);
                //从ConfigQueryResponse对象中得到服务端返回的配置文件加密的密钥
                String encryptedDataKey = response.getEncryptedDataKey();
                //把密钥设置到客户端自己使用的响应对象中
                configResponse.setEncryptedDataKey(encryptedDataKey);
                //返回客户端自己使用的响应对象
                return configResponse;
            } else {
    
                //处理服务端错误响应的操作就省略了
            }
            
        }



        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/1
         * @Description:客户端向服务端发布配置信息的方法，这个方法中的encryptedDataKey参数大家可以暂时忽略
         */
        public boolean publishConfig(String dataId, String group, String tenant, String appName, String tag, String betaIps,
                                     String content, String encryptedDataKey, String casMd5, String type) throws NacosException {
            try {
                //创建一个向服务端发送的发布配置文件信息的请求对象
                ConfigPublishRequest request = new ConfigPublishRequest(dataId, group, tenant, content);
                //设置配置文件md5的值，这个值就用来在服务端判断，新发布的配置文件信息是否和旧的配置文件信息不同，不同则意味着配置文件信息更新了
                request.setCasMd5(casMd5);
                //设置额外的标签tag参数
                request.putAdditionalParam(TAG_PARAM, tag);
                //设置应用名称
                request.putAdditionalParam(APP_NAME_PARAM, appName);
                //设置灰度发布的IP地址，如果没有设置，那么发布配置信息的操作将会直接在目标服务器执行
                request.putAdditionalParam(BETAIPS_PARAM, betaIps);
                //设置配置类型
                request.putAdditionalParam(TYPE_PARAM, type);
                //设置密钥，这一点非常重要，大家可能会觉得困惑，但没关系，我马上就为大家解释
                request.putAdditionalParam(ENCRYPTED_DATA_KEY_PARAM, encryptedDataKey == null ? "" : encryptedDataKey);
                //在这里把请求发送给服务端，并且从服务端接收到响应对象
                ConfigPublishResponse response = (ConfigPublishResponse) requestProxy(getOneRunningClient(), request);
                //判断响应是否成功
                if (!response.isSuccess()) {
                    //响应不成功则记录告警信息
                    LOGGER.warn("[{}] [publish-single] fail, dataId={}, group={}, tenant={}, code={}, msg={}",
                            this.getName(), dataId, group, tenant, response.getErrorCode(), response.getMessage());
                    return false;
                } else {
                    //响应成功的情况
                    LOGGER.info("[{}] [publish-single] ok, dataId={}, group={}, tenant={}, config={}", getName(),
                            dataId, group, tenant, ContentUtils.truncateContent(content));
                    return true;
                }
            } catch (Exception e) {
                LOGGER.warn("[{}] [publish-single] error, dataId={}, group={}, tenant={}, code={}, msg={}",
                        this.getName(), dataId, group, tenant, "unknown", e.getMessage());
                return false;
            }
        }


         //向服务端发送请求的方法
        private Response requestProxy(RpcClient rpcClientInner, Request request) throws NacosException {
            //在这里把请求发送给服务端，默认超时时间
            return agent.request(request, 3000);
        }
        
    
    }




    //下面就又来到ClientWorker类中了


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:为租户添加监听器的方法
     */
    public void addTenantListeners(String dataId, String group, List<? extends Listener> listeners)throws NacosException {
        //对组名进行判空
        group = blank2defaultGroup(group);

        //得到与该配置文件对应的CacheData对象
        CacheData cache = addCacheDataIfAbsent(dataId, group, tenant);

        //把监听器添加到CacheData对象中
        synchronized (cache) {
            for (Listener listener : listeners) {
                cache.addListener(listener);
            }

            //设置CacheData对象没有被废弃标志
            cache.setDiscard(false);
            //设置对象存储的配置信息和服务端不一致标志
            cache.setConsistentWithServer(false);
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:得到指定配置文件的CacheData对象的方法
     */
    public CacheData getCache(String dataId, String group, String tenant) {
        if (null == dataId || null == group) {
            throw new IllegalArgumentException();
        }//GroupKey.getKeyTenant(dataId, group, tenant)就是得到这三个信息的组合key的方法
        return cacheMap.get().get(GroupKey.getKeyTenant(dataId, group, tenant));
    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:为指定的配置文件创建CacheData缓存数据对象的方法，监听配置文件是否发生变更的监听器就存放在这个CacheData对象中
     * 该方法和上面方法的逻辑一样，只不过上面的方法没有指定租户信息，如果用户没有配置的话，就使用默认的租户信息
     * 而该方法指定了租户信息，我就不重复添加注释了，大家自己看看就成
     */
    public CacheData addCacheDataIfAbsent(String dataId, String group, String tenant) throws NacosException {
        //先判断成员变量cacheMap中是否缓存了对应的CacheData对象
        CacheData cache = getCache(dataId, group, tenant);
        if (null != cache) {
            return cache;
        }
        //如果没有缓存则为该配置信息创建一个CacheData对象
        //这里的到组合key
        String key = GroupKey.getKeyTenant(dataId, group, tenant);
        //创建CacheData对象
        cache = new CacheData(dataId, group, tenant);
        //计算要分配给新创建的的CacheData对象的taskId
        //至于为什么要分配，我很快就会为大家讲解，我把重构之后的CacheData对象的内容也展示在当前代码块中了
        //大家可以看一下
        int taskId = calculateTaskId();
        //对现在的到的这个taskId进行计数
        //这个taskId管理的配置信息达到3000之后，就会创建一个新的taskId
        //比如上一个taskId是2，当它被交给3000个CacheData对象使用之后
        //就会再创建一个值为3的taskId
        increaseTaskIdCount(taskId);
        //把taskId交给CacheData对象使用
        cache.setTaskId(taskId);
        //从服务端查询当前配置信息
        ConfigResponse response = getServerConfig(dataId, group, tenant, 3000L, false);
        //把配置信息设置到CacheData对象中，同时把MD5的值更新了
        //这里有一个问题，请大家思考一下，这里直接从服务端获得的配置信息还是加密过后的配置信息
        //并没有被解密，难道就直接把没有解密的配置信息存储到CacheData对象中吗？大家可以带着这个疑问
        //去看我提供的第十五版本代码
        cache.setContent(response.getContent());
        Map<String, CacheData> copy = new HashMap<>(this.cacheMap.get());
        copy.put(key, cache);
        //更新cacheMap成员变量
        cacheMap.set(copy);
        return cache;

        //注意，这里还是没有实现真正向服务端发送监听配置信息的请求，那该怎么做呢？
        //之后的操作呢？客户端还没有向服务端发送监听配置信息的请求呢？还没有真正向服务端执行配置监听操作呢，这是怎么回事呢？
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:计算要分配的taskId的方法
     */
    private int calculateTaskId() {
        //得到最大的分片数量，也就是多少给CacheData共享一个taskId的数量
        //这里得到的默认值是3000
        int perTaskSize = (int) ParamUtil.getPerTaskConfigSize();
        for (int index = 0; index < taskIdCacheCountList.size(); index++) {
            //如果当前的taskId一直不满3000，就使用当前的taskId，这样才能达到3000个CacheData对象共享一个taskId的效果
            if (taskIdCacheCountList.get(index).get() < perTaskSize) {
                return index;
            }
        }
        //如果3000分配完毕了，就创建一个新的计数器，计算下一个taskId是否分配满3000了
        taskIdCacheCountList.add(new AtomicInteger(0));
        return taskIdCacheCountList.size() - 1;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/2
     * @Description:对这个taskId进行计数的方法
     */
    private void increaseTaskIdCount(int taskId) {
        taskIdCacheCountList.get(taskId).incrementAndGet();
    }


    //省略该类的其他内容

}




//以下就是重构之后的CacheData类


package com.cqfy.nacos.client.config.impl;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/9/2
 * @Description:客户端缓存配置信息的对象
 */
public class CacheData {

    //配置信息Id
    public final String dataId;

    //所在组
    public final String group;

    //租户信息，其实就是配置信息所属的命名空间
    public final String tenant;

    //配置信息
    private volatile String content;

    //配置信息类型
    private String type;

    //当前CacheData对象是否被废弃的标志，其实就是缓存的配置信息是否被废弃的标志
    //被废弃就不能再使用了
    private volatile boolean isDiscard = false;

    //配置信息的md5
    private volatile String md5;

    //是否正在初始化标志
    private volatile boolean isInitializing = true;

    //存储该配置信息监听器的集合，这里我要给大家解释一下
    //用户定义的监听器对象并不是直接以Listener对象的方式保存在CacheData对象内部
    //而是会被包装成一个ManagerListenerWrap对象，存储在下面这个集合成员变量中
    private final CopyOnWriteArrayList<ManagerListenerWrap> listeners;

    //当前CacheData对象存储的配置信息是否和服务端一致标志
    private final AtomicBoolean isConsistentWithServer = new AtomicBoolean();

    //以下是新添加的成员变量
    
    //和其他CacheData对象共享的taskId
    //现在就让我来为大家解释一下，为什么要给CacheData对象定义一个taskId成员变量，之前我就已经跟大家说过，为了提高客户端的工作效率
    //尤其是在执行和监听服务端配置信息变更操作的效率，nacos配置中心客户端使用RpcClient客户端分片的方式来监听服务端配置信息的变更
    //Id为0的RpcClient客户端专门负责发布配置信息、查询配置信息、移除配置信息的操作，Id为1的RpcClient客户端就负责前3000分配置信息的监听
    //id为2的RpcClient客户端负责3001-6000配置信息的监听，以此类推即可
    //我们都知道，客户端每向服务端监听一个配置信息，就会为这个配置信息创建一个对应的CacheData对象，监听3000个配置信息
    //就会创建3000个CacheData对象，而这3000个CacheData对象监听的配置信息都是由同一个RpcClient客户端对象负责的
    //所以要给CacheData对象定义一个taskId成员变量，用来表示当前CacheData对象保存的配置信息的监听是由哪个RpcClient客户端
    //对象负责的，也就是说，每3000个CacheData对象共享一个taskId，而它们共享的这个taskId，实际上就是负责它们配置信息监听的
    //RpcClient客户端的Id，现在大家应该稍微清楚一些了吧。
    private int taskId;

    //该类的其他内容省略
}
```

上面代码块展示的就是重构之后的 ClientWorker 类，可以看到，代码块中的内容非常多，但内容多并不意味着就难以理解，经过我之前为大家分析的重构程序的思路，再加上代码中非常详细的注释，我相信大家只要认真阅读，都能看懂上面代码块中的内容，所以我就不再为大家重复讲解整个代码块的内容了，只讲解一下我认为重要的方法。实际上，大家仔细阅读了上面代码块的内容之后，会发现很多方法，比如向服务端查询配置信息的方法、向服务端发布配置信息的方法，这些方法的整体逻辑并没有什么大的变动。说到底，上面展示的 ClientWorker 类其实还是围绕着新引入的 ConfigRpcTransportClient 客户端数据传输器来重构的。

在我们新引入的这个 ConfigRpcTransportClient 客户端数据传输器中，我们可以清楚地看到 nacos 配置中心使用的 RpcClient 客户端对象是怎么创建的，又是如何对要监控的配置信息进行分片的，更重要的是，我们还看到了创建出来的 RpcClient 客户端对象是如何初始化的，在对创建完毕的 RpcClient 客户端对象执行初始化的操作时，有一个非常关键的步骤，那就是向 RpcClient 客户端对象中注册了一个请求处理器，也就是下面这段代码展示的内容，请看下面代码块。

```
 /**
  * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
  * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
  * @Date:2024/9/1
  * @Description:初始化创建完毕的RPC客户端的方法
  */
  private void initRpcClientHandler(final RpcClient rpcClientInner)   
      /**
       * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
       * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
       * @Date:2024/9/2
       * @Description:向RPC客户端中注册处理通知配置变更请求的处理器，当服务端主动向客户端发送了ConfigChangeNotifyRequest配置变更请求后
       * 当前注册的处理器就会处理这个请求
       */
      rpcClientInner.registerServerRequestHandler((request, connection) -> {
          //判断请求类型是否为ConfigChangeNotifyRequest
          if (request instanceof ConfigChangeNotifyRequest) {
              //进行强转
              ConfigChangeNotifyRequest configChangeNotifyRequest = (ConfigChangeNotifyRequest) request;
              //记录配置变更日志信息
              LOGGER.info("[{}] [server-push] config changed. dataId={}, group={},tenant={}",
                      rpcClientInner.getName(), configChangeNotifyRequest.getDataId(),
                      configChangeNotifyRequest.getGroup(), configChangeNotifyRequest.getTenant());
              //得到组合键
              String groupKey = GroupKey.getKeyTenant(configChangeNotifyRequest.getDataId(),
                      configChangeNotifyRequest.getGroup(), configChangeNotifyRequest.getTenant());
              //得到对应的CacheData对象
              CacheData cacheData = cacheMap.get().get(groupKey);
              if (cacheData != null) {
                  synchronized (cacheData) {
                  //在这里设置当前CacheData对象接收到了服务端发送过来的配置变更通知标志
                    //注意，这里只是设置了一个标志，并没有对当前CacheData对象进行配置变更，刷新当前CacheData对象缓存的数据
                    //等到客户端下一次执行配置变更监听的定时任务时，就会根据这个标注主动去服务端查询最新的配置信息
                    //然后刷线CacheData对象
                    cacheData.getReceiveNotifyChanged().set(true);
                    //因为配置发生变更了，所以肯定要设置CacheData当前缓存的配置文件和服务端不一致标志
                    //等到后面客户端执行配置变更监听定时任务时，发现配置没有发生变更，就会把这个标志更新为true了
                    cacheData.setConsistentWithServer(false);
                    //通知客户端向服务端执行下一次配置监听定时任务，其实就是向listenExecutebell阻塞队列中添加了一个变更信号
                    notifyListenConfig();
                }
            }
            return new ConfigChangeNotifyResponse();
        }
        return null;
    });
    //省略该方法的其他内容
    
    
}
```

在上面代码块中，**我们可以看到在初始化 RpcClient 客户端对象时，向该对象内部注册了一个专门处理 ConfigChangeNotifyRequest 请求的请求处理器**。而 ConfigChangeNotifyRequest 请求就是服务端主动发送给客户端的，确切地说，**当配置中心服务端发现自己保存的配置信息发生变更之后，就会主动向客户端发送一个 ConfigChangeNotifyRequest 请求，在请求中封装了发生变更的配置的关键信息，当客户端获得这些信息之后，就可以直接根据这些信息去服务端查询对应的配置信息，然后就可以获得变更之后的最新的配置信息了**。当然，如果这个操作能被成功执行的话，那就必须遵守一个大前提，那就是配置中心客户端向服务端执行了监听某个配置信息的操作，也就是说，**配置中心客户端必须把监听配置信息的请求发送给服务端，服务端处理了这个请求之后，服务端才知道在哪些配置信息变更之后，主动给哪个客户端发送配置变更的 ConfigChangeNotifyRequest 请求**。但是在刚才的整个 ClientWorker 类中，大家肯定可以发现，我们仍然没有实现客户端向服务端发送监听配置信息的请求。而且，**在上面展示的向 RpcClient 客户端对象注册请求处理器的代码中，我们也能发现，就算客户端接收到了服务端 ConfigChangeNotifyRequest 请求，知道了自己监听的配置信息发生了变更，但是客户端并没有直接去服务端查询对应的配置信息，这是为什么呢**？

这时候就要从 ConfigRpcTransportClient 客户端数据传输器中没有实现的那个 startInternal() 方法说起了。在 ClientWorker 类的构造方法中，我们可以看到，**一旦创建了 ConfigRpcTransportClient 客户端数据传输器之后，就会紧接着调用数据传输器的 startInternal() 方法，也就是启动数据传输器**。那么我们启动数据传输器的作用是什么呢？接下来就让我为大家分析清楚。

请大家想一想，当配置中心客户度监听了服务端的某个配置信息，我们就当作客户端已经把监听配置信息的请求发送给服务端了，服务端也处理了该请求。这样一来，当服务端发现自己保存的配置信息发生变更时，就会主动给客户端发送通知请求，让客户端知道自己监听的配置信息发生变更了。但是，请大家想一想，客户端向服务端执行了监听配置信息的操作之后，难道就什么都不做了，就等着服务端主动通知吗？当然可以这么设计，**但假如服务端要通知的客户端非常多，或者服务端要发送给某个客户端的 ConfigChangeNotifyRequest 请求非常多，那么客户端在接收对应的 ConfigChangeNotifyRequest 请求时可能就不会那么及时。这就会造成一种情况，那就是客户端不能及时更新自己存储的配置信息**。为了避免这种情况，我们就可以让客户端多执行一个必要的操作：**那就是开启一个任务，在任务中客户端会循环访问服务端，所谓访问就是客户端会不断向服务端发送监听配置信息的请求，服务端接收到这个请求后，会把该客户端的信息以及该客户端要监听的配置的信息都保存起来，同时会在自己内部判断一下该客户端要监听的配置信息有没有发生变更，如果发生变更了，就在回复给客户端的响应中把发生变更的配置的信息告诉客户端，然后客户端就会根据配置的信息向服务端查询该配置的最新信息**。也就是说，**除了让服务端主动通知客户端有配置发生变更了，客户端还要一直访问服务端，判断是否有配置发生变更了**，这样才能及时更新自己监听的配置信息。**而这些操作就要在 ConfigRpcTransportClient 客户端数据传输器中没有实现的那个 startInternal() 方法中执行**，这就是 ConfigRpcTransportClient 客户端数据传输器的 startInternal() 方法的作用。当然，配置中心客户端肯定不会只监听一个配置信息，我们只需要看看 ClientWorker 类的 cacheMap 成员变量中存储了多少 CacheData 对象，就能根据这些 CacheData 对象知道客户端要主动向服务端查询看看自己监听的哪些配置信息是否发生变更了。这就是我为大家分析的 ConfigRpcTransportClient 客户端数据传输器的 startInternal() 方法的作用，也是该方法的实现思路。接下来，就让我们在代码层面上实现该方法吧，请看下面代码块。

```
package com.cqfy.nacos.client.config.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:客户端工作组件，在以前它使用长轮询机制监听配置文件变更情况，但在2.x版本之后，就不再使用长轮询，而是长连接了
 */
public class ClientWorker implements Closeable {


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:配置中心客户端数据传输器的子类，这个类的对象就相当于配置中心客户端
     * 所有和服务端交互的操作，以及和服务端通信的操作，都是由这个类的对象执行的
     */
    public class ConfigRpcTransportClient extends ConfigTransportClient {

        //租户信息，其实就是命名空间
        String tenant;

        //服务器地址列表管理器
        final ServerListManager serverListManager;

        //客户端配置信息对象
        final Properties properties;


        //构造方法
        public ConfigRpcTransportClient(NacosClientProperties properties, ServerListManager serverListManager) {
            this.tenant = properties.getProperty(PropertyKeyConst.NAMESPACE);
            this.serverListManager = serverListManager;
            this.properties = properties.asProperties();
        }


        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/1
         * @Description:得到客户端和服务端连接类型的方法
         */
        private ConnectionType getConnectionType() {
            return ConnectionType.GRPC;
        }


        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/9/2
         * @Description:启动客户端数据传输器的方法，其实就是启动客户端向服务端执行的监听配置文件变更情况的定时任务的方法
         */
        @Override
        public void startInternal() {

            //注意，在文章中展示的该方法经过了非常多的简化，大家在看完文章之后
            //还是要仔细阅读第十五版本代码，代码中注释非常详细，大家只要按照顺序阅读代码，肯定能理解每一个步骤的作用和意义

            //循环执行
            while (true) {
                //定义一个存放需要执行监听操作的map，key是CacheData对应要使用的RpcClient的ID
                //value就是共享该Id的CacheData对象的集合
                Map<String, List<CacheData>> listenCachesMap = new HashMap<>(16);
                //遍历所有的CacheData对象，也就得到了所有要监听的配置文件的相关信息
                for (CacheData cache : cacheMap.get().values()) {
                    //判断CacheData对象是否被废弃了
                    if (!cache.isDiscard()) {
                        //没有被废弃则添加到执行监听操作的map中，之后向服务端执行监听配置文件操作，就是为与这些CacheData对象对应的配置文件执行的
                        List<CacheData> cacheDatas = listenCachesMap.computeIfAbsent(String.valueOf(cache.getTaskId()), k -> new LinkedList<>());
                        cacheDatas.add(cache);
                    }
                }


                
                //在下面对需要执行监听操作的的map中存放的所有CacheData对象执行监听操作，就是向服务端查询，看看这些配置文件的信息是否发生变更了
                //对数据判空
                if (!listenCachesMap.isEmpty()) {

                     //遍历要执行监听操作的CacheData数据
                    for (Map.Entry<String, List<CacheData>> entry : listenCachesMap.entrySet()) {
                        //得到这一批CacheData对象共享的taskId
                        String taskId = entry.getKey();
                        //得到这一批要校验的CacheData对象集合
                        List<CacheData> listenCaches = entry.getValue();
                        //构建一个配置变更监听请求，而且是批量监听的请求，因为客户端要监听服务端配置信息的变化
                        //服务端还要主动通知客户端有配置文件信息发生变化了，那么服务端就得知道要对哪些配置文件进行监听
                        //所以客户端要发送请求告诉服务端要监听哪些配置信息
                        ConfigBatchListenRequest configChangeListenRequest = buildConfigRequest(listenCaches);
                        //设置监听标志，这个标志就意味着当前发送的ConfigBatchListenRequest请求执行的是配置监听的操作
                        //如果定义为false，就表示执行的是移除对应配置的操作
                        configChangeListenRequest.setListen(true);
                        
                        //得到这一批CacheData对象共享的RPC客户端
                        RpcClient rpcClient = ensureRpcClient(taskId);
                        //把请求发送给服务端，并收到响应,这个响应中封装着发生了变更的配置的关键信息
                        ConfigChangeBatchListenResponse listenResponse = (ConfigChangeBatchListenResponse) requestProxy(rpcClient, configChangeListenRequest);
                        //判断响应是否成功
                        if (listenResponse != null && listenResponse.isSuccess()) {
                            //定义一个集合，存储配置发生变更的组合key
                            Set<String> changeKeys = new HashSet<String>();
                            //从响应中得到配置发生变更的文件的信息
                            List<ConfigChangeBatchListenResponse.ConfigContext> changedConfigs = listenResponse.getChangedConfigs();
                            //如果不为空则意味着有配置信息发生变更了
                            if (!CollectionUtils.isEmpty(changedConfigs)) {
                            
                                for (ConfigChangeBatchListenResponse.ConfigContext changeConfig : changedConfigs) {
                                    
                                    //在这里得到发生配置变更的文件的组合键
                                    String changeKey = GroupKey.getKeyTenant(changeConfig.getDataId(),changeConfig.getGroup(), changeConfig.getTenant());
                                    
            
                                    //接下来就是访问服务端，从服务端查询最新的配置信息，然后缓存到客户端本地，顺便更新CacheData对象md5的值的操作
                                    if (cacheMap.get() != null && cacheMap.get().containsKey(groupKey)) {
                                        //得到指定的CacheData对象
                                        CacheData cacheData = cacheMap.get().get(groupKey);
                                        //从服务端获取最新的配置文件的信息
                                        ConfigResponse response = getServerConfig(cacheData.dataId, cacheData.group, cacheData.tenant, 3000L, notify);
                                        //判断CacheData对象存储的配置文件的md5的值是否和监听器存储的md5不同
                                        //不同则意味着配置文件发生了变化，要执行配置文件监听器的回调方法
                                        //这个方法终于被调用了
                                        cacheData.checkListenerMd5();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        //省略该类的其他内容
    }


    //省略该类的其他内容
    
}
```

到此为止，我就为大家把 ConfigRpcTransportClient 客户端数据传输器的 startInternal() 方法实现完毕了，也就意味着我把 nacos 配置中心客户端监听服务端配置信息变更的功能实现完毕了。总之总结下来就两点：**第一，配置中心客户端监听了某个配置信息之后，服务端在发现配置变更了，就会主动给客户端发送通知请求；第二，客户端会不断地访问服务端，主动查询自己监听的配置信息是否发生变更了**。当然，我相信在看完本章的内容之后，大家心里肯定会有两个疑问：

1 在我刚才为大家展示的 ConfigRpcTransportClient 客户端数据传输器的 startInternal() 方法中，我们可以看到客户端会在一个循环中不断地访问服务端，判断自己监听的配置信息是否发生变更，这是一个死循环，也就意味着客户端要一刻不停地执行相同的操作，大家肯定觉得这是不合理的，假如服务端保存的配置信息很长一段时间内都没有发生变更，难道客户端也要一直访问服务端吗？我可以很负责任的告诉大家，这里并不是一个死循环，只不过我在文章中为了简化代码，写成了死循环。大家可以去看我提供的第十五版本代码，看完之后就知道客户端是以什么样的频率循环访问服务端了。

2 在初始化 RpcClient 客户端对象时，向客户端注册了专门处理 ConfigChangeNotifyRequest 请求的处理器。这个处理器在接收到服务端主动发送过来的通知客户端配置变更的请求后，并没有访问服务端，直接从服务端获取最新配置信息的操作，这是为什么呢？这种情况下，客户端怎么才能从服务端获得变更后的配置信息呢？我相信大家肯定也会对这个问题感到困惑。但这个问题我也不在文章中为大家解释了，还是把问题留给大家，大家可以去我提供的第十五版本代码中寻找答案。

总的来说，本章对应的第十五版本代码，从代码量上来说内容确实有点多，但说到底也仅仅是内容多一点而已，功能的实现逻辑以及程序执行流程还是非常清晰的。我在文章中确实省略了很多无关紧要的功能和一些细节内容，但是我把客户端监听配置变更功能需要用到的所有组件都展示在文章中了，来来回回就那么几个类而已。而且这些类中的核心方法我也都为大家实现了，即便实现得不太全面，但是整体的实现思路我分析得非常详细。所以，剩下的那点知识就交给大家自己去查看吧，这也是一个巩固本章内容的机会。第十四和十五版本代码我都为大家提供了测试类，在启动这两个测试类时，大家可以先启动 nacos 配置中心源码的服务端，然后再启动我的测试类就行了。这两个版本代码主要实现的是配置中心客户端的功能，没有涉及到服务端，从下一章开始，我们就要正式实现配置中心服务端的内容了，还是老样子，非常简单，无非就是从客户端接收到数据，存放到数据库中，和常规的业务流程非常相似，我很快就会为大家实现。好了朋友们，本章就到此为止了，我们下一章见！