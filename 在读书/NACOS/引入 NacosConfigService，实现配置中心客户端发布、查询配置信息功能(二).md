  

## 引入配置文件过滤器，实现配置文件的加密和解密

  

上一章结尾我为大家分析了 NacosConfigService 类的 publishConfigInner() 方法，**跟大家说当 nacos 配置中心要把配置信息发布到服务端时，都会执行对配置文件加密的操作，而加密的操作就应该定义在 NacosConfigService 类的 publishConfigInner() 中**。原因很简单，**因为在这个方法中调用了 ClientWorker 类的 publishConfig() 方法，在调用该方法的时候，需要把配置文件的加密密钥传递进去**。就像下面代码块中展示的这样，请看下面代码块。

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

    //省略该类的其他内容


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:配置中心客户端发布配置文件的方法，这个方法的参数中有一个casMd5字符串，如果客户端在发布配置信息时计算了配置文件的md5
     * 那么配置文件被服务端接收后，就会比较配置文件的md5和之前存储的配置文件的md5是否不一致，如果不一致说明客户端发布的配置文件是更新过的
     * 同时我们也能看到该方法中还有几个参数，比如说betaIps、tag等等，tag是配置信息的标签，是在dataId之下对配置信息更细粒度的划分
     * betaIps就是灰度发布的IP地址，这些内容在nacos主线章节中并不会涉及，等到我为大家更新nacos支线章节时，会为大家实现和这些参数相关的功能
     */
    private boolean publishConfigInner(String tenant, String dataId, String group, String tag, String appName,
                                       String betaIps, String content, String type, String casMd5) throws NacosException {
        //对配置文件所在组名判空，为空则使用默认组名
        group = blank2defaultGroup(group);
        //判断配置文件是否合规
        ParamUtils.checkParam(dataId, group, content);
        //在这里使用客户端工作组件把配置文件发布到服务端
        //ClientWorker类的publishConfig方法中传进去了一个encryptedDataKey参数
        //但是我们目前根本不知道这个参数是从哪里得到的
        return worker.publishConfig(dataId, group, tenant, appName, tag, betaIps, content,encryptedDataKey, casMd5, type);
    }

}
```

从上面代码块中可以看到，虽然在 NacosConfigService 对象的 publishConfigInner() 方法中调用了 ClientWorker 类的 publishConfig() 方法，把配置信息发布给服务端，但是在执行 ClientWorker 类的 publishConfig() 方法时，需要向该方法中传进去一个 encryptedDataKey 参数，但是在 NacosConfigService 对象的 publishConfigInner() 方法中，我们根本就没有得到这个 encryptedDataKey 参数。这是因为我们还没有实现对配置信息进行加密的功能。接下来我就为大家实现这个功能。

实际上，在 nacos 配置中心源码中，当客户端要向服务端发布配置信息的时候，并不会只对配置信息进行文件加密的操作，加密操作只是其中的一个环节。其实是这样的，**在 nacos 配置中心客户端启动的时候，会创建一个过滤器管理器对象，这个对象内部持有很多过滤器，构成了一个过滤链，当配置文件要被客户端发布到服务端之前，配置文件就要先经过过滤器管理器的过滤链处理，在处理的过程中，过滤链中的每一个过滤器都会对配置文件进行处理，而对配置文件进行加密操作，就是其中一个过滤器要执行的操作**。这就是配置中心客户端在发布配置信息到服务端之前，要对配置信息执行的重要操作。

当然，**nacos 配置中心所做的也只是构建了一个过滤链而已，过滤链中每一个过滤器，其实还是交给用户来实现了。也就是说，用户可以自定义任何过滤器，对要发布的配置信息执行相应的操作，就算要对配置信息进行加密的过滤器，或者说加密器，也是由用户自己定义的。用户自己定义的各种过滤器，最终会以 SPI 的方式加载到 nacos 配置中心的过滤器管理器中**。接下来我就为大家把具体的代码展示一下。

首先是 nacos 配置中心暴露给用户的过滤器的借口，就是 IConfigFilter，请看下面代码块。

```
package com.cqfy.nacos.api.config.filter;


public interface IConfigFilter {

    /**
     * Init.
     *
     * @param properties Filter Config
     */
    void init(Properties properties);

    /**
     * do filter.
     *
     * @param request     request
     * @param response    response
     * @param filterChain filter Chain
     * @throws NacosException exception
     */
    void doFilter(IConfigRequest request, IConfigResponse response, IConfigFilterChain filterChain)
            throws NacosException;

    /**
     * Get order.
     *
     * @return order number
     */
    int getOrder();

    /**
     * Get filterName.
     *
     * @return filter name
     */
    String getFilterName();

}
```

**用户可以为 IConfigFilter 该接口实现各种各样的实现类，这些过滤器实现类最终会以 SPI 的方式被加载到程序中，最后会被存储在 nacos 配置中心客户端的过滤器管理器中， 也就是我接下来要为大家展示的 ConfigFilterChainManager 类**，请看下面代码块。

```
package com.cqfy.nacos.client.config.filter.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:配置文件过滤器管理器
 */
public class ConfigFilterChainManager implements IConfigFilterChain {

    //存放所有过滤器的集合
    private final List<IConfigFilter> filters = new ArrayList<>();

    //nacos客户端配置文件
    private final Properties initProperty;


    //构造方法
    public ConfigFilterChainManager(Properties properties) {
        this.initProperty = properties;
        //使用spi机制加载所有的过滤器
        ServiceLoader<IConfigFilter> configFilters = ServiceLoader.load(IConfigFilter.class);
        //把所有过滤器对象添加到集合中
        for (IConfigFilter configFilter : configFilters) {
            addFilter(configFilter);
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:添加过滤器到集合中的方法
     */
    public synchronized ConfigFilterChainManager addFilter(IConfigFilter filter) {
        //初始化过滤器
        filter.init(this.initProperty);
        //以下就是根据过滤器的顺序，按照顺序把过滤器添加到集合中的方法
        int i = 0;
        while (i < this.filters.size()) {
            IConfigFilter currentValue = this.filters.get(i);
            //如果过滤器名称相同了，说明要添加的过滤器已经添加过了，直接退出循环即可
            if (currentValue.getFilterName().equals(filter.getFilterName())) {
                break;
            }
            //按照顺序给过滤器排序
            if (filter.getOrder() >= currentValue.getOrder() && i < this.filters.size()) {
                i++;
            } else {
                //走到这里意味着找到了正确的位置，直接按顺序把过滤器插入集合即可
                this.filters.add(i, filter);
                break;
            }
        }
        //程序执行到这里就意味着过滤器的顺序比较靠后，直接添加到集合末尾即可
        if (i == this.filters.size()) {
            this.filters.add(i, filter);
        }
        return this;
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:执行过滤的方法，ConfigRequest请求是nacos配置中心客户端专门为过滤器组件定义的请求对象
     * 这个请求对象中封装着要发布给服务端的配置信息，经过过滤器链过滤后，过滤器就会把密钥和加密后的配置文件更新到ConfigRequest对象中
     * 方中的第二个参数，也就是ConfigResponse响应对象，这个对象中封装着从服务器查询到的配置信息，如果过滤器链
     * 的doFilter()方法接收到的是一个响应对象，就意味着要为从服务端查询到的配置信息执行解密操作
     * 注意，我要强调一下，IConfigRequest和IConfigResponse对象都是在配置中心客户端使用的，并没有在网络传输中使用，这一点大家要弄清楚
     */
    @Override
    public void doFilter(ConfigRequest request, ConfigResponse response) throws NacosException {
        //先创建过滤器链对象，然后执行过滤方法
        new VirtualFilterChain(this.filters).doFilter(request, response);
    }




    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:过滤器链对象
     */
    private static class VirtualFilterChain implements IConfigFilterChain {


        private final List<? extends IConfigFilter> additionalFilters;

        //目前执行到的过滤器的位置
        private int currentPosition = 0;


        public VirtualFilterChain(List<? extends IConfigFilter> additionalFilters) {
            this.additionalFilters = additionalFilters;
        }


        //被过滤器链过滤的方法
        @Override
        public void doFilter(final ConfigRequest request, final ConfigResponse response) throws NacosException {
            if (this.currentPosition != this.additionalFilters.size()) {
                this.currentPosition++;
                IConfigFilter nextFilter = this.additionalFilters.get(this.currentPosition - 1);
                nextFilter.doFilter(request, response, this);
            }
        }
    }

}
```

在上面代码块中可以看到，**在 ConfigFilterChainManager 类的构造方法中，就使用 SPI 的方式收集到了用户定义的所有过滤器，并且在 ConfigFilterChainManager 类中还定义了一个 doFilter() 方法，在该方法中过滤器链就会对配置信息进行过滤处理**。**接下来我们只需要把 ConfigFilterChainManager 过滤器管理器定义为 NacosConfigService 类的成员变量，然后在 NacosConfigService 类的 publishConfigInner() 方法中把要发布的配置文件交给过滤器管理器处理**，这样不就可以对配置文件进行加密了吗？具体实现请看下面代码块。

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


     //构造方法
    public NacosConfigService(Properties properties) throws NacosException {
        //异步加载序列化工具
        PreInitUtils.asyncPreLoadCostComponent();
        //得到nacos自己的客户端配置对象
        final NacosClientProperties clientProperties = NacosClientProperties.PROTOTYPE.derive(properties);
        //校验配置信息合法性
        ValidatorUtils.checkInitParam(clientProperties);

         //创建过滤器管理器
        this.configFilterChainManager = new ConfigFilterChainManager(clientProperties.asProperties());
        
        //给命名空间赋值
        initNamespace(clientProperties);
        
        //创建服务地址管理器，这个管理器存放的都是nacos配置中心服务端的地址
        ServerListManager serverListManager = new ServerListManager(clientProperties);
        //启动服务地址管理器
        serverListManager.start();

        //创建客户端工作组件，现在这个ServerListManager服务地址管理器就被用到了
        this.worker = new ClientWorker(serverListManager, clientProperties);

    }


     //初始化命名空间的方法
    private void initNamespace(NacosClientProperties properties) {
        namespace = ParamUtil.parseNamespace(properties);
        properties.setProperty(PropertyKeyConst.NAMESPACE, namespace);
    }


   /**
    * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
    * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
    * @Date:2024/8/31
    * @Description:客户端得到指定的配置文件的方法
    */
    @Override
    public String getConfig(String dataId, String group, long timeoutMs) throws NacosException {
        return getConfigInner(namespace, dataId, group, timeoutMs);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:得到配置文件的方法，该方法的整体流程非常简单，其实就是先尝试从本地的故障转移文件中得到配置文件信息
     * 如果没有获取，就从nacos配置中心服务器获取
     */
    private String getConfigInner(String tenant, String dataId, String group, long timeoutMs) throws NacosException {

        //使用客户端工作组件从配置中心服务器得到配置文件，配置文件封装在了响应对象中
        ConfigResponse response = worker.getServerConfig(dataId, group, tenant, timeoutMs, false);
        return response.getContent();
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:客户端向服务端发布配置文件的方法，这个方法不可以指定客户端要发布的配置文件的类型
     * 只能发布默认类型，也就是text文件，方法参数content就是配置信息的内容
     */
    @Override
    public boolean publishConfig(String dataId, String group, String content) throws NacosException {
        //我把ConfigType枚举类的内容也展示在当前代码块中了
        return publishConfig(dataId, group, content, ConfigType.getDefaultType().getType());
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:客户端向服务端发布配置文件的方法，这个方法可以指定客户端要发布的配置文件的类型
     */
    @Override
    public boolean publishConfig(String dataId, String group, String content, String type) throws NacosException {
        return publishConfigInner(namespace, dataId, group, null, null, null, content, type, null);
    }
    


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:配置中心客户端发布配置文件的方法，这个方法的参数中有一个casMd5字符串，如果客户端在发布配置信息时计算了配置文件的md5
     * 那么配置文件被服务端接收后，就会比较配置文件的md5和之前存储的配置文件的md5是否不一致，如果不一致说明客户端发布的配置文件是更新过的
     * 同时我们也能看到该方法中还有几个参数，比如说betaIps、tag等等，tag是配置信息的标签，是在dataId之下对配置信息更细粒度的划分
     * betaIps就是灰度发布的IP地址，这些内容在nacos主线章节中并不会涉及，等到我为大家更新nacos支线章节时，会为大家实现和这些参数相关的功能
     */
    private boolean publishConfigInner(String tenant, String dataId, String group, String tag, String appName,
                                       String betaIps, String content, String type, String casMd5) throws NacosException {
        //对配置文件所在组名判空，为空则使用默认组名
        group = blank2defaultGroup(group);
        //判断配置文件是否合规
        ParamUtils.checkParam(dataId, group, content);
        //创建一个配置文件请求对象
        ConfigRequest cr = new ConfigRequest();
        //为请求对象的成员变量赋值
        cr.setDataId(dataId);
        cr.setTenant(tenant);
        cr.setGroup(group);
        cr.setContent(content);
        cr.setType(type);
        //把请求对象交给过滤器管理器处理，过滤器会对配置文件执行加密操作
        configFilterChainManager.doFilter(cr, null);
        //得到加密后的配置文件
        content = cr.getContent();
        //得到密钥
        String encryptedDataKey = cr.getEncryptedDataKey();
        //在这里使用客户端工作组件把配置文件发不到服务端
        return worker.publishConfig(dataId, group, tenant, appName, tag, betaIps, content, encryptedDataKey, casMd5, type);
    }

}
```

好了，到此为止，对配置信息进行加密的操作，我就为大家实现完毕了，但是仅仅实现到这个程度是不够的，因为真正对配置文件进行加密的过滤器，我还没有实现呢。这时候我就要为大家解释一下了，实际上，在 nacos 配置中心客户端有一个已经定义好的，可以对配置信息进行加密和解密的过滤器，就是 ConfigEncryptionFilter 过滤器，该过滤器实现了 IConfigFilter 接口，并且会以 SPI 的方式被存储到过滤器管理器中。确切地说，**ConfigEncryptionFilter 类的对象就是 nacos 配置中心客户端内置的对配置文件执行加密解密的组件**。该类的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.client.config.filter.impl;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/8/31
 * @Description:配置文件加密过滤器
 */
public class ConfigEncryptionFilter implements IConfigFilter {

    private static final String DEFAULT_NAME = ConfigEncryptionFilter.class.getName();

    @Override
    public void init(Properties properties) {
        //该方法源码中就没有实现
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:过滤方法
     */
    @Override
    public void doFilter(IConfigRequest request, IConfigResponse response, IConfigFilterChain filterChain)
            throws NacosException {
        //首先判断当前接收的是请求对象还是响应对象，如果是请求对象就要执行加密操作
        //说明是要向服务端发布配置嘻嘻
        if (Objects.nonNull(request) && request instanceof ConfigRequest && Objects.isNull(response)) {
            ConfigRequest configRequest = (ConfigRequest) request;
            //得到配置文件的Id
            String dataId = configRequest.getDataId();
            //得到配置文件的内容
            String content = configRequest.getContent();
            //对数据进行加密，发挥加密之后的键值对，大家先记住这个EncryptionHandler类，很快就会为大家讲解
            Pair<String, String> pair = EncryptionHandler.encryptHandler(dataId, content);
            //得到密钥
            String secretKey = pair.getFirst();
            //得到加密后配置文件的内容
            String encryptContent = pair.getSecond();
            //如果加密后的内容和之前不一样了，说明加密有效
            if (!StringUtils.isBlank(encryptContent) && !encryptContent.equals(content)) {
                //把加密后的文件内容设置到请求中
                ((ConfigRequest) request).setContent(encryptContent);
            }
            //设置加密密钥到请求中
            if (!StringUtils.isBlank(secretKey) && !secretKey.equals(((ConfigRequest) request).getEncryptedDataKey())) {
                ((ConfigRequest) request).setEncryptedDataKey(secretKey);
            } else if (StringUtils.isBlank(((ConfigRequest) request).getEncryptedDataKey()) && StringUtils.isBlank(secretKey)) {
                ((ConfigRequest) request).setEncryptedDataKey("");
            }
        }
        //这里判断当前处理的是否为响应对象，说明是从服务端查询到了指定的配置信息，就意味着要对配置文件进行解密
        if (Objects.nonNull(response) && response instanceof ConfigResponse && Objects.isNull(request)) {
            //如果是响应对象，就执行解密操作
            ConfigResponse configResponse = (ConfigResponse) response;
            //得到配置文件的Id
            String dataId = configResponse.getDataId();
            //得到密钥
            String encryptedDataKey = configResponse.getEncryptedDataKey();
            //得到加密的配置文件内容
            String content = configResponse.getContent();
            //对配置文件执行解密操作
            Pair<String, String> pair = EncryptionHandler.decryptHandler(dataId, encryptedDataKey, content);
            //得到解密后的密钥
            String secretKey = pair.getFirst();
            //得到解密后的配置文件内容
            String decryptContent = pair.getSecond();
            //设置解密后的配置文件到响应中
            if (!StringUtils.isBlank(decryptContent) && !decryptContent.equals(content)) {
                ((ConfigResponse) response).setContent(decryptContent);
            }
            //设置密钥到响应对象中
            if (!StringUtils.isBlank(secretKey) && !secretKey.equals(((ConfigResponse) response).getEncryptedDataKey())) {
                ((ConfigResponse) response).setEncryptedDataKey(secretKey);
            } else if (StringUtils.isBlank(((ConfigResponse) response).getEncryptedDataKey()) && StringUtils.isBlank(secretKey)) {
                ((ConfigResponse) response).setEncryptedDataKey("");
            }
        }
        //让下一个过滤器继续执行过滤操作
        filterChain.doFilter(request, response);
    }
    
    @Override
    public int getOrder() {
        return 0;
    }
    
    @Override
    public String getFilterName() {
        return DEFAULT_NAME;
    }
    
}
```

这个 ConfigEncryptionFilter 类的内容非常简单，就是对配置信息执行加密和解密的操作，这没什么可讲的，代码中的注释也非常详细，所以我就不再重复讲解了。但是我也知道大家肯定都很疑惑，因为我刚才跟大家说了，配置文件的加密解密组件也是可以由用户自己定义的，但现在我展示的这个 ConfigEncryptionFilter 过滤器好想把加密解密的操作已经定义完毕了，那留给用户自由发挥的空间在哪里呢？

这时候，就可以一起看看上面代码块的第 39 和 67 行的代码了，就是下面这两行，请看下面代码块。

```
//第39行代码，对数据进行加密的操作
Pair<String, String> pair = EncryptionHandler.encryptHandler(dataId, content);

//第67行代码，对数据进行解密的操作
Pair<String, String> pair = EncryptionHandler.decryptHandler(dataId, encryptedDataKey, content);
```

从上面代码块中可以看到，这两行代码就是对数据进行加密和解密操作的，并且加密和解密都用到了同一个组件，那就是 EncryptionHandler。现在就让我来为大家详细解释一下，**实际上在 nacos 配置中心有一个 EncryptionPluginService 接口，该接口的实现类就是提供加密服务的组件，并且该接口的实现类可以由用户自己定义，也就是说对配置文件进行加密的组件可以有用户自己定义，定义完毕的加密器就会以 SPI 的方式被 nacos 加载到程序内部**；**除了这个 EncryptionPluginService 接口之外，在 nacos 配置中心客户端还定义了一个 EncryptionPluginManager 加密插件管理器，这个管理器在被创建的时候，就会以 SPI 的方式把用户定义的所有 EncryptionPluginService 加密器都加载到内存中，然后保存在自己内部。也就是说，nacos 程序一旦启动，用户自己定义的所有配置文件加密器都会被 EncryptionPluginManager 加密插件管理器来管理**。这又是 nacos 插件化思想的一个体现。

那么，EncryptionPluginManager 得到了用户定义的所有加密插件之后会怎样呢？这时候就简单多了，在上面的代码块中不是已经展示了，EncryptionHandler 组件就是对配置信息进行加密和解密的，而用户定义的加密插件都被 EncryptionPluginManager 管理器保存着，**那么显然 EncryptionHandler 组件在给配置信息执行加密操作时，会从 EncryptionPluginManager 管理器中得到用户定义的加密插件，然后再执行加密操作**。这就是 nacos 配置中心客户端对要发布到服务端的配置信息执行加密操作的全流程，相关的代码我就不在展示了，因为代码也不多，而且内容都很简单，我就只在文章中给大家分析一下流程思路，大家按照我分析的流程去看代码即可。当然，我分析了这一大堆也有另外的原因，因为我在为大家提供的第十四版本代码中，并没有严格按照源码引入加密插件体系，只是从 nacos 提供的测试类中找了例子复制到 EncryptionHandler 组件中了。如果大家想看原汁原味的加密插件体系，大家可以直接去看 nacos 源码。源码中的内容就是我刚才分析的那些，非常简单，我相信每一位朋友都能看懂。好了，到此为止，nacos 配置中心客户端向服务端发布配置信息的功能，我就为大家全部实现完毕了。

## 实现配置中心客户端从服务端查询指定配置信息功能

好了，我们已经把 nacos 配置中心客户端发布配置信息的功能实现完毕了，接下来该实现客户端从服务端查询指定配置信息的功能了。有了配置发布功能做铺垫，配置信息查询功能实现起来就更简单了，还是定义一个请求对象，然后配置中心客户端把请求发送给服务端，服务端回复对应的响应即可。当然，响应中肯定封装着客户端要查询的配置信息。**因为配置信息查询的真正操作是在 ClientWorker 类的 getServerConfig() 方法中执行的，所以接下来我们只需要重构 ClientWorker 类的 getServerConfig() 方法即可**。重构的逻辑非常简单，就是创建一个请求对象，该对象封装了客户端要从服务端查询的指定配置信息，然后使用 RpcClient 客户端对象发送给服务端，然后接收到服务端回复的响应对象，响应中封装着客户端要查询的配置信息。按照这个逻辑来重构的话，我们首先要把和查询配置信息有关的请求对象和响应对象定义出来。这两个对象我也定义完毕了，接下来就展示给大家。

首先是请求体，我把它定义为了 ConfigQueryRequest，请看下面代码块。

```
package com.cqfy.nacos.api.config.remote.request;



public class ConfigQueryRequest extends AbstractConfigRequest {
    
    private String tag;
    
    
    public static ConfigQueryRequest build(String dataId, String group, String tenant) {
        ConfigQueryRequest request = new ConfigQueryRequest();
        request.setDataId(dataId);
        request.setGroup(group);
        request.setTenant(tenant);
        return request;
    }
    
    
    public String getTag() {
        return tag;
    }
    
   
    public void setTag(String tag) {
        this.tag = tag;
    }
    
    public boolean isNotify() {
        String notify = getHeader(Constants.Config.NOTIFY_HEADER, Boolean.FALSE.toString());
        return Boolean.parseBoolean(notify);
    }
}
```

然后是响应体，我把它定义为 ConfigQueryResponse，请看下面代码块。

```
package com.cqfy.nacos.api.config.remote.response;


public class ConfigQueryResponse extends Response {
    
    //响应错误码，这个代表没有在服务端找到指定配置信息的意思
    public static final int CONFIG_NOT_FOUND = 300;
    
    //这个代表服务端正在修改配置文件的意思
    public static final int CONFIG_QUERY_CONFLICT = 400;
    
    String content;
    
    String encryptedDataKey;
    
    String contentType;
    
    String md5;
    
    long lastModified;
    
    boolean isBeta;
    
    String tag;
    
    public ConfigQueryResponse() {
    }

    
    //get、set方法省略
    
}
```

请求体和响应体定义完毕了，接下来就该按照我们之前分析的逻辑实现 ClientWorker 类的 getServerConfig() 方法了，请看下面代码块。

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



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/9/1
     * @Description:构造方法，这个方法其实是伪代码，后面会对其进行重构
     */
    @SuppressWarnings("PMD.ThreadPoolCreationRule")
    public ClientWorker(ServerListManager serverListManager,final NacosClientProperties properties) throws NacosException {

        //在这里创建配置中心客户端
        agent = new RpcClient(properties, serverListManager);
       
        //启动配置中心客户端
        agent.start();

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

从上面代码块的 getServerConfig() 方法中，我们可以看到向服务单发送了一个 ConfigQueryRequest 请求，然后从服务端得到一个 ConfigQueryResponse 响应，这个响应对象中封装着客户端指定要查询的配置信息。当然，getServerConfig() 方法中执行的操作不只是这些，我们还可以看到，**当从服务端接收到 ConfigQueryResponse 响应对象后，nacos 配置中心客户端又创建了一个 ConfigResponse 对象，然后把从服务端得到了已经加密过的配置信息，以及密钥都设置到这个新创建的 ConfigResponse 对象中了，最后把 ConfigResponse 对象返回了**。如果我没有为大家详细讲解和配置文件加密相关的操作流程，大家看到 ClientWorker 类的 getServerConfig() 方法后肯定会感到疑惑，不知道为什么要这么做，**但现在大家已经知道了从客户端发布到服务端的配置信息要加密之后才能发布，那也就能理解从服务端得到指定的配置信息后，要对配置信息进行解密操作的原因了**。当然，对配置信息进行解密的操作并不是在 ClientWorker 类的 getServerConfig() 方法执行的，而是在调用了 ClientWorker 类的 getServerConfig() 方法的外层方法中执行的，也就是在 NacosConfigService 对象的 getConfigInner() 方法中执行的，所以接下来我们就要重构一下 NacosConfigService 对象的 getConfigInner() 方法。重构的逻辑也非常简单，**只要再次使用过滤器对从服务端获取的配置信息进行解密即可**，我把重构完毕的代码展示在下面了，请看下面代码块。

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


     //构造方法
    public NacosConfigService(Properties properties) throws NacosException {
        //异步加载序列化工具
        PreInitUtils.asyncPreLoadCostComponent();
        //得到nacos自己的客户端配置对象
        final NacosClientProperties clientProperties = NacosClientProperties.PROTOTYPE.derive(properties);
        //校验配置信息合法性
        ValidatorUtils.checkInitParam(clientProperties);

         //创建过滤器管理器
        this.configFilterChainManager = new ConfigFilterChainManager(clientProperties.asProperties());
        
        //给命名空间赋值
        initNamespace(clientProperties);
        
        //创建服务地址管理器，这个管理器存放的都是nacos配置中心服务端的地址
        ServerListManager serverListManager = new ServerListManager(clientProperties);
        //启动服务地址管理器
        serverListManager.start();

        //创建客户端工作组件，现在这个ServerListManager服务地址管理器就被用到了
        this.worker = new ClientWorker(serverListManager, clientProperties);

    }


     //初始化命名空间的方法
    private void initNamespace(NacosClientProperties properties) {
        namespace = ParamUtil.parseNamespace(properties);
        properties.setProperty(PropertyKeyConst.NAMESPACE, namespace);
    }


   /**
    * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
    * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
    * @Date:2024/8/31
    * @Description:客户端得到指定的配置文件的方法
    */
    @Override
    public String getConfig(String dataId, String group, long timeoutMs) throws NacosException {
        return getConfigInner(namespace, dataId, group, timeoutMs);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/8/31
     * @Description:得到配置文件的方法，该方法的整体流程非常简单，其实就是先尝试从本地的故障转移文件中得到配置文件信息
     * 如果没有获取，就从nacos配置中心服务器获取
     */
    private String getConfigInner(String tenant, String dataId, String group, long timeoutMs) throws NacosException {

       //对配置文件所在组名判空，如果为空得到默认组名
        group = blank2defaultGroup(group);
        //校验文件Id和组名是否合规
        ParamUtils.checkKeyParam(dataId, group);
        //创建一个配置响应对象
        ConfigResponse cr = new ConfigResponse();
        //为响应对象的重要成员变量赋值
        cr.setDataId(dataId);
        //租户信息其实就是命名空间
        cr.setTenant(tenant);
        cr.setGroup(group);
        
        //使用客户端工作组件从配置中心服务器得到配置文件，配置文件封装在了响应对象中
        ConfigResponse response = worker.getServerConfig(dataId, group, tenant, timeoutMs, false);
        //把配置文件设置到刚才创建的响应对象中
        cr.setContent(response.getContent());
        //设置密钥
        cr.setEncryptedDataKey(response.getEncryptedDataKey());
        //让过滤器处理配置文件
        configFilterChainManager.doFilter(null, cr);
        //得到过滤后的配置文件
        content = cr.getContent();
        //返回解密后的配置信息
        return content;
    }


    //该类的其他内容暂时省略
}
```

到此为止我就把 nacos 配置中心客户端发布和查询配置信息的功能实现完毕了，可以看到，这两个功能实现起来非常简单，几乎不用怎么思考，模仿者注册中心来就能实现。当然，对于 nacos 配置中心来说，这两个功能并不是最重要的，**最重要的当然是客户端向服务端订阅配置信息的功能，其实就是客户端监听服务端配置信息变更功能。因为只有客户端监听了指定的配置信息之后，被监听的配置信息才能缓存在客户端内部，那么配置信息动态更新的功能也就可以实现了。对于配置中心客户端来说，这个才是最重要的功能**。在下一章，我就会为大家实现这个功能，好了朋友们，我们下一章见！

附：现在大家已经可以去看我提供的第十四版本代码了，但是有一点我要解释一下，大家在阅读第十四版本代码的时候，会发现我提供的代码和 nacos 源码中，配置中心客户端的构建和我在文章中展示的稍微有点不一样，虽然都用到了 RpcClient 对象，但是在获得该对象之前，还做了点其他的操作；至于文章中展示的其他内容，就和第十四版本代码一样了。我之所以不在本章中把配置中心客户端实现的和源码一致，是因为对客户端进行重构需要掌握部分和配置信息订阅功能相关的知识，而这是下一章的知识，所以我就不在本章中继续对配置中心客户端进行重构了。第十四版本代码，大家愿意看就看看，不愿意看就再等等，反正内容也没多少，看文章也能看懂，等下一篇文章更新完毕了，我把配置中心客户端重构得和源码一致了，大家直接去看第十五版本代码即可，到时候就全清楚了。当然，出现这种情况主要也是因为我懒得改造源码了，因为十四十五版本代码联系非常紧密，而十四版本代码对应的内容又很简单，就偷了下懒，没有对源码的客户端部分进行改造，在这理跟大家说声抱歉了。