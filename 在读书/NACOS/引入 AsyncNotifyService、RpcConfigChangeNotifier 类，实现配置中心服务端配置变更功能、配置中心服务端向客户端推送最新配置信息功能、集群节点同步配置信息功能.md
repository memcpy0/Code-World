这一章要实现的就是服务端配置变更功能，我记得在上一章结尾我跟大家强调了，配置变更功能本身并不重要，重要的是配置变更之后的操作该如何执行。**比如说服务端主动通知客户端有配置发生变更了，或者是服务端把变更后的配置同步给集群的其他节点**，这才是我们要实现的功能，当然，还有一个非常重要的功能要实现：**因为配置信息最初肯定是在数据库中更新的，数据都存储在数据库中，那么在某个配置信息更新之后，要把最新的配置信息更新到服务端的缓存系统和文件系统中**。这样一来，客户端从服务端查询配置信息的时候，才能查询到最新的配置信息。配置变更本身的功能实际上已经被实现了，客户端向服务端发布新的配置信息，或者是更新、移除了某个配置信息，这些功能我们都实现完毕了吧？而客户端只要向服务端执行这些操作，那服务端存储的配置信息可不就发生变更了吗？当然，web 控制台也可以直接对服务端存储的配置信息执行更新或移除的操作，这个功能我会放到 nacos 支线中为大家实现。好了，那现在重点已经明确了，那就是实现配置变更之后的三个重要功能：

**1 服务端主动通知客户端有配置发生变更了。**

**2 在某个配置信息更新之后，要把最新的配置信息更新到服务端的缓存系统和文件系统中。**

**3 服务端把变更后的配置同步给集群的其他节点。**

除了这三个操作之外，好像也没有特别重要的操作，要在配置变更之后执行了。接下来，就让我们看看第一个功能是怎么实现的。

## 引入 ConfigChangeBatchListenRequestHandler

  

要想让服务端主动通知有配置发生变更了，需要满足两个条件，第一就是服务端需要知道哪个配置发生变更了；第二就是需要知道哪个客户端监听了该配置信息。第一个很好解决，**我们只需要在配置发生变更的那一刻，让 nacos 内部的事件通知中心发布一个配置变更事件不就行了？然后再为这个配置变更事件定义一个订阅者，订阅者处理该事件的时候，就会把发生变更的配置的关键信息发送给客户端**。这个功能模式大家应该非常熟悉了吧？现在对我们来说，要重点关注的是第二个条件，那就是服务端怎么就知道是哪个客户端监听了哪个配置信息呢？总不能只要有一个配置发生了变更，服务端就通知所有客户端所有配置都发生变更了吧。所以，服务端必须精准地知道哪个客户端监听了哪个配置信息，这样一来，当服务端发现自己存储的某个配置信息发生变更了，就可以立刻通知对应的客户端它监听的某个配置发生变更了。那这个功能该怎么实现呢？这就要从客户端向服务端发送的监听配置信息的请求说起了。

在我们为配置中心客户端实现监听配置信息功能时，会让客户端向服务端发送一个 ConfigBatchListenRequest 请求，这个大家应该还有印象吧？在该请求中封装了该客户端要监听的所有配置的关键信息。也就是说，**通过这个请求，服务端就可以知道客户端要监听的每一个配置信息的 dataId、group、tenant，这三个信息组合起来就可以确定唯一的一个配置信息**。既然是这样，那服务端在得到这些信息之后，把它们保存起来，当服务端某个配置发生变更之后，不就可以直接从保存的这些信息中找到是哪个客户端订阅了该配置信息吗？这个逻辑大家应该也可以理解吧？如果这个逻辑理解了，那现在的新问题就是，**服务端该怎么保存客户端监听了哪些配置的信息呢**？

最简单直接的方式就是使用 map 存储，map 的 key 就是客户端的连接 Id，value 就是客户端监听的配置信息的组合键，也就是 dataId + group + tenant 组成的 key。这样一来，通过 key 就可以知道客户端的信息，通过 value 就可以知道该客户端监听的配置的关键信息。这下不就解决刚才的问题了吗？如果在程序执行的整个过程中，服务端存储的每一个配置信息只有一个客户端监听，那这么做确实解决刚才的问题了，但问题是，在程序运行的过程中，很可能有多个客户端监听同一个配置信息，这样一来，就不能使用刚才的存储方式了。当然，**我们肯定还是要使用 map，但是 map 的 key 就变成了配置信息的 dataId + group + tenant 构成的组合键，value 是一个集合，该集合中存放的就是监听了该配置信息的客户端的连接 Id**。这样一来，**服务端就可以根据变更的配置信息的组合键，找到订阅了该配置信息的所有客户端的信息，然后通知这些客户端它们监听的某个配置信息发生变更了**。这个逻辑理解起来应该也很简单吧？如果大家都理解了这个逻辑，那接下来我就把这个 map 定义出来，实际上，**在 nacos 配置中心源码中专门定义了一个对象来存储客户端和订阅的配置信息的映射关系，这个对象就是 ConfigChangeListenContext，翻译过来就是配置变更监听器上下文的意思，在这个 ConfigChangeListenContext 类中定义了一个 map 成员变量，map 的 key 就是配置信息的 dataId + group + tenant 构成的组合键，value 就是存放了监听该配置信息的客户端连接 Id 的集合**，这个类的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.config.server.remote;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:这个是配置中心服务端使用的配置变更监听上下文对象，客户端只要向配置中心执行了监听配置变更的操作后
 * 那么客户端要监听的配置的信息以及客户端的信息都会被封装到该类的对象中，服务端在通知客户端有信息发生变更时，就会根据发生变更的配置信息从该类的对象中找到对应的客户端的信息。
 */
@Component
public class ConfigChangeListenContext {

    //key是配置信息组合键，value就是订阅了该配置信息的客户端连接ID的集合，通过客户端连接ID就可以从连接管理器中得到客户端的IP地址
    private ConcurrentHashMap<String, HashSet<String>> groupKeyContext = new ConcurrentHashMap<>();

    //key是配置信息组合键，value的map中的key是配置信息组合键，value是配置信息对应的MD5的值
    private ConcurrentHashMap<String, HashMap<String, String>> connectionIdContext = new ConcurrentHashMap<>();

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:把客户端要监听的配置的信息存储到groupKeyContext和connectionIdContext中的方法
     */
    public synchronized void addListen(String groupKey, String md5, String connectionId) {
        Set<String> listenClients = groupKeyContext.get(groupKey);
        if (listenClients == null) {
            groupKeyContext.putIfAbsent(groupKey, new HashSet<>());
            listenClients = groupKeyContext.get(groupKey);
        }
        listenClients.add(connectionId);
        HashMap<String, String> groupKeys = connectionIdContext.get(connectionId);
        if (groupKeys == null) {
            connectionIdContext.putIfAbsent(connectionId, new HashMap<>(16));
            groupKeys = connectionIdContext.get(connectionId);
        }
        groupKeys.put(groupKey, md5);

    }


    //移除指定监听信息的方法
    public synchronized void removeListen(String groupKey, String connectionId) {
        Set<String> connectionIds = groupKeyContext.get(groupKey);
        if (connectionIds != null) {
            connectionIds.remove(connectionId);
            if (connectionIds.isEmpty()) {
                groupKeyContext.remove(groupKey);
            }
        }
        HashMap<String, String> groupKeys = connectionIdContext.get(connectionId);
        if (groupKeys != null) {
            groupKeys.remove(groupKey);
        }
    }


    //得到所有监听了groupKey对应的配置信息的客户端连接Id集合
    public synchronized Set<String> getListeners(String groupKey) {
        HashSet<String> strings = groupKeyContext.get(groupKey);
        if (CollectionUtils.isNotEmpty(strings)) {
            Set<String> listenConnections = new HashSet<>();
            safeCopy(strings, listenConnections);
            return listenConnections;
        }
        return null;
    }



    //该类的其他方法省略
}
```

好了，现在我们已经把存储客户端监听配置的信息的对象定义完毕了，那该怎么往这个 ConfigChangeListenContext 对象中添加数据呢？这时候其实就可以为配置中心服务端定义一个 ConfigBatchListenRequest 请求处理器了，该请求处理器专门处理从客户端发送过来的监听配置信息的 ConfigBatchListenRequest 请求。处理器要执行的逻辑就是解析 ConfigBatchListenRequest 请求，从请求中得到客户端要监听的配置信息，然后把客户端信息和它监听的配置信息存放到 ConfigChangeListenContext 对象中。我把这个请求处理器定义为了 ConfigChangeBatchListenRequestHandler，具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.config.server.remote;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:处理客户端监听配置信息请求的处理器
 */
@Component
public class ConfigChangeBatchListenRequestHandler extends RequestHandler<ConfigBatchListenRequest, ConfigChangeBatchListenResponse> {

    //配置变更监听器上下文对象
    @Autowired
    private ConfigChangeListenContext configChangeListenContext;


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:处理ConfigBatchListenRequest请求的方法
     */
    @Override
    public ConfigChangeBatchListenResponse handle(ConfigBatchListenRequest configChangeListenRequest, RequestMeta meta) throws NacosException {
        //得到发送请求过来的客户端的连接Id，并且把连接Id字符串在串池缓存一下
        String connectionId = StringPool.get(meta.getConnectionId());
        //从请求头中得到用户定义的标签信息，如果没有定义，就是null
        //但实际上，在源码中这个tag根本就没用上，nacos中有很多这样的代码，在类中或者方法中经常定义一些无用变量
        //可能是代码迭代时没有移除干净
        String tag = configChangeListenRequest.getHeader(Constants.VIPSERVER_TAG);

        //创建ConfigChangeBatchListenResponse响应对象，该对象中就会封装发生了变更的配置关键信息的集合
        ConfigChangeBatchListenResponse configChangeBatchListenResponse = new ConfigChangeBatchListenResponse();
        
        
        //遍历请求中的configListenContexts集合
        for (ConfigBatchListenRequest.ConfigListenContext listenContext : configChangeListenRequest.getConfigListenContexts()) {
            //得到客户端要监听的配置信息的组合键
            String groupKey = GroupKey2.getKey(listenContext.getDataId(), listenContext.getGroup(), listenContext.getTenant());
            //把组合键缓存到串池中
            groupKey = StringPool.get(groupKey);
            //得到客户端发送过来的要监听的配置信息的MD5的值，并且缓存到串池中
            String md5 = StringPool.get(listenContext.getMd5());
            //判断请求类型
            if (configChangeListenRequest.isListen()) {
                //如果是监听请求，那就把要监听的配置的关键信息存放到configChangeListenContext对象中
                configChangeListenContext.addListen(groupKey, md5, connectionId);
                //紧接着对比客户端发送过来的配置信息的MD5的值和服务端缓存的是否相等，不相等意味配置信息发生变更了
                boolean isUptoDate = ConfigCacheService.isUptodate(groupKey, md5, meta.getClientIp(), tag);
                if (!isUptoDate) {
                    //如果两个MD5的值不相等，那就意味配置信息可能发生变更了，把发生变更的配置关键信息封装到响应对象中
                    configChangeBatchListenResponse.addChangeConfig(listenContext.getDataId(), listenContext.getGroup(),
                            listenContext.getTenant());
                }
            } else {
                //执行到这里就意味着是移除配置监听操作，直接移除即可
                configChangeListenContext.removeListen(groupKey, connectionId);
            }
        }
        
        //返回响应对象
        return configChangeBatchListenResponse;
    }

}
```

上面代码块中的注释，非常详细，大家自己看看就行。好了，**现在服务端已经能够清楚地知道哪个客户端监听了哪个配置信息，只要服务端发现自己存储的配置信息发生变更了，就可以直接从 ConfigChangeListenContext 对象中得到监听了该配置信息的客户端的信息，然后通知这些客户端监听的配置信息发生变更即可，然后客户端就会主动向服务端查询发生变更的配置的最信息信息**。这个功能实现了之后，现在我们就该回过头，分析分析如何让服务端及时感知到有配置发生变更了，并且感知到了之后能立刻通知对应的客户端有配置发生变更了。

这个其实就没什么可说的了，刚才已经分析过了，直接定义配置变更事件吧，然后定义一个该事件的订阅者就行。这个逻辑大家应该都很熟悉了吧？并且，我相信如果大家对十六、十七版本代码非常熟悉了，那肯定就还记得，实际上我们已经在代码中定义了两个配置变更事件，**一个是 ConfigDataChangeEvent 事件，会在 ConfigPublishRequestHandler 请求处理器处理完客户端发布过来的 ConfigPublishRequest 请求之后，发布 ConfigDataChangeEvent 配置变更事件**，具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.config.server.remote;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/24
 * @Description:配置中心服务端的配置发布处理器，客户端发送给服务端的ConfigPublishRequest请求都会被这个处理器处理
 * 这个处理器就会把客户端发布的配置信息存储到数据库中
 */
@Component
public class ConfigPublishRequestHandler extends RequestHandler<ConfigPublishRequest, ConfigPublishResponse> {

    //专门用来将客户端发布的配置信息存储的数据库的对象
    //其实就是用来和mysql数据库打交道的组件
    private final ConfigInfoPersistService configInfoPersistService;

    //构造方法
    public ConfigPublishRequestHandler(ConfigInfoPersistService configInfoPersistService) {
        this.configInfoPersistService = configInfoPersistService;
    }


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/24
     * @Description:处理客户端发送的ConfigPublishRequest请求的方法
     */
    @Override
    public ConfigPublishResponse handle(ConfigPublishRequest request, RequestMeta meta) throws NacosException {

        //该方法前面处理ConfigPublishRequest请求的内容全部省略，只展示成功处理了ConfigPublishRequest请求后的代码

        //这里有一个非常重要的操作，就是发布配置变更事件，原因很简单，不管是插入新的配置信息还是更新旧的配置信息，都意味着配置信息发生变更了
        //所以要发布一个配置变更事件
        ConfigChangePublisher.notifyConfigChange(new ConfigDataChangeEvent(false, dataId, group, tenant, configOperateResult.getLastModified()));
    }
    
}




//接下来是ConfigDataChangeEvent事件本身的内容
package com.cqfy.nacos.config.server.model.event;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/24
 * @Description:配置变更事件，这个事件是用来给配置中心集群备份最新的配置信息的
 */
public class ConfigDataChangeEvent extends Event {

    public final boolean isBeta;

    public final boolean isBatch;

    public final String dataId;

    public final String group;

    public final String tenant;

    public final String tag;

    public final long lastModifiedTs;

    public ConfigDataChangeEvent(String dataId, String group, long gmtModified) {
        this(false, dataId, group, gmtModified);
    }

    public ConfigDataChangeEvent(boolean isBeta, String dataId, String group, String tenant, long gmtModified) {
        if (null == dataId || null == group) {
            throw new IllegalArgumentException("dataId is null or group is null");
        }
        this.isBeta = isBeta;
        this.dataId = dataId;
        this.group = group;
        this.tenant = tenant;
        this.tag = null;
        this.isBatch = false;
        this.lastModifiedTs = gmtModified;
    }

    public ConfigDataChangeEvent(boolean isBeta, String dataId, String group, long gmtModified) {
        this(isBeta, dataId, group, StringUtils.EMPTY, gmtModified);
    }

    public ConfigDataChangeEvent(boolean isBeta, String dataId, String group, String tenant, String tag,
                                 long gmtModified) {
        if (null == dataId || null == group) {
            throw new IllegalArgumentException("dataId is null or group is null");
        }
        this.isBeta = isBeta;
        this.dataId = dataId;
        this.group = group;
        this.tenant = tenant;
        this.tag = tag;
        this.isBatch = false;
        this.lastModifiedTs = gmtModified;
    }

    public ConfigDataChangeEvent(String dataId, String group, String tenant, boolean isBatch, long gmtModified) {
        if (null == dataId || null == group) {
            throw new IllegalArgumentException();
        }
        this.isBeta = false;
        this.dataId = dataId;
        this.group = group;
        this.tenant = tenant;
        this.tag = null;
        this.isBatch = isBatch;
        this.lastModifiedTs = gmtModified;
    }

}
```

当然，除了这个配置变更事件，配置中心服务端还会在缓存系统，也就是 ConfigCacheService 中发布配置变更事件，**当服务端把数据库中的配置信息更新到缓存系统和本地文件系统时，如果 ConfigCacheService 发现更新过来的配置信息发生变更了，就会在该类的 updateMd5() 方法中发布一个 LocalDataChangeEvent 事件**，具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.config.server.service;

/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/28
 * @Description:配置信息缓存系统
 */
public class ConfigCacheService {

    //该类的其他内容省略


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:把从数据库中查询到的配置信息更新到缓存中，同时也会写入到本地的硬盘文件中的方法
     */
    public static boolean dump(String dataId, String group, String tenant, String content, long lastModifiedTs,
                               String type, String encryptedDataKey) {
        return dumpWithMd5(dataId, group, tenant, content, null, lastModifiedTs, type, encryptedDataKey);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:把配置信息更新到缓存系统和本地文件系统中的方法
     */
    public static boolean dumpWithMd5(String dataId, String group, String tenant, String content, String md5,
                                      long lastModifiedTs, String type, String encryptedDataKey) {
        //得到配置信息的组合键
        String groupKey = GroupKey2.getKey(dataId, group, tenant);
        //根据配置信息的组合键得到和配置信息对应的CacheItem缓存项，如果缓存项不存在，就为该配置信息创建CacheItem缓存项
        CacheItem ci = makeSure(groupKey, encryptedDataKey);
        //设置配置信息类型到缓存项中
        ci.setType(type);
        //获取该配置信息的锁
        final int lockResult = tryWriteLock(groupKey);
        //获取锁失败则直接退出当前方法
        if (lockResult < 0) {
            DUMP_LOG.warn("[dump-error] write lock failed. {}", groupKey);
            return false;
        }

        try {
            //走到这里意味着获取该配置项的锁成功了，在这里判断一下方法中传入的配置信息的最新修改时间是否小于配置项中缓存的配置信息的最新修改时间
            boolean lastModifiedOutDated = lastModifiedTs < ConfigCacheService.getLastModifiedTs(groupKey);
            //如果小于则意味着配置项缓存的对应的配置信息是最新更新过的，本次方法调用不必更新该配置信息在缓存项中的信息
            if (lastModifiedOutDated) {
                DUMP_LOG.warn("[dump-ignore] timestamp is outdated,groupKey={}", groupKey);
                //直接退出当前方法即可，返回true表示方法调用成功
                return true;
            }
            //程序执行到这里会再判断一下方法中传入的配置信息的最新修改时间是否大于配置项中缓存的配置信息的最新修改时间
            //大于的话就可以更新缓存项中的信息，同时更新本地文件中存储的配置信息了
            boolean newLastModified = lastModifiedTs > ConfigCacheService.getLastModifiedTs(groupKey);
            //对方法传入的md5的值进行判空处理
            if (md5 == null) {
                md5 = MD5Utils.md5Hex(content, ENCODE_UTF8);
            }
            //得到缓存项中配置信息的MD5的值
            String localContentMd5 = ConfigCacheService.getContentMd5(groupKey);
            //判断MD5的值是否发生了变化
            boolean md5Changed = !md5.equals(localContentMd5);
            //如果发生变化了，就意味着配置信息发生了变更，所以就要把最新的配置信息保存到本地的文件系统中
            //刚才已经判断过配置信息的最后修改时间了，如果方法参数传进来的修改时间小，就会直接退出当前方法了，根本不会执行到这里
            //如果执行到这里，就意味着方法参数中传进来的配置信息的修改时间肯定是不小于配置项中的时间的，所以在发现MD5不同之后
            //肯定就可以断定是配置信息发生变更了
            if (md5Changed) {
                //如果程序在查询配置信息时，并不是直接从数据库系统中查询的，而是从本地文件系统中查询的，这个时候就要把配置信息更新到本地文件系统中
                if (!PropertyUtil.isDirectRead()) {
                    DUMP_LOG.info("[dump] md5 changed, save to disk cache ,groupKey={}, newMd5={},oldMd5={}", groupKey, md5, localContentMd5);
                    ConfigDiskServiceFactory.getInstance().saveToDisk(dataId, group, tenant, content);
                } else {
                    //如果是直接从数据库中读取，那就可以什么都不做，因为配置信息发生更新时，首先就会把数据库中的信息更新了，然后才会更新缓存系统和文件系统中的信息
                }
            } else {
                //如果MD5相同，则只记录日志信息
                DUMP_LOG.warn("[dump-ignore] ignore to save to disk cache. md5 consistent,groupKey={}, md5={}", groupKey, md5);
            }
            //接下来还是判断MD5是否发生变化了，如果发生变化，就把最新的MD5的值更新到缓存项中，然后执行一个最重要的操作，那就是发布配置变更事件
            //这事件一旦发布，配置中心服务端就可以主动通知客户端有配置发生变更了
            if (md5Changed) {
                DUMP_LOG.info(
                        "[dump] md5 changed, update md5 and timestamp in jvm cache ,groupKey={}, newMd5={},oldMd5={},lastModifiedTs={}",
                        groupKey, md5, localContentMd5, lastModifiedTs);
                //把最新的MD5的值更新到缓存项中，然后发布配置变更事件
                updateMd5(groupKey, md5, lastModifiedTs, encryptedDataKey);
            } else if (newLastModified) {
                //走到这里意味着配置信息的MD5没有发生变更，但是配置信息的最新修改时间发生变化了
                DUMP_LOG.info(
                        "[dump] md5 consistent ,timestamp changed, update timestamp only in jvm cache ,groupKey={},lastModifiedTs={}",
                        groupKey, lastModifiedTs);
                //那就直接更新配置信息缓存项的最新修改时间
                updateTimeStamp(groupKey, lastModifiedTs, encryptedDataKey);
            } else {
                //走到这里意味着MD5和配置信息修改时间都没有发生变化
                DUMP_LOG.warn(
                        "[dump-ignore] ignore to save to jvm cache. md5 consistent and no new timestamp changed.groupKey={}",
                        groupKey);
            }
            //返回处理成功标志
            return true;
        } catch (IOException ioe) {
            //出现异常情况后记录错误信息即可
            DUMP_LOG.error("[dump-exception] save disk error. " + groupKey + ", " + ioe);
            //分析出现错误的原因
            if (ioe.getMessage() != null) {
                String errMsg = ioe.getMessage();
                //判断是空间不足还是超出磁盘限额
                if (NO_SPACE_CN.equals(errMsg) || NO_SPACE_EN.equals(errMsg) || errMsg.contains(DISK_QUOTA_CN)
                        || errMsg.contains(DISK_QUOTA_EN)) {
                    FATAL_LOG.error("Local Disk Full,Exit", ioe);
                    //空间不足直接终止程序
                    System.exit(0);
                }
            }
            //返回执行失败结果
            return false;
        } finally {
            //释放该配置信息缓存项的锁
            releaseWriteLock(groupKey);
        }

    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:把最新的MD5的值更新到缓存项中，然后发布配置变更事件的方法
     */
    public static void updateMd5(String groupKey, String md5Utf8, long lastModifiedTs, String encryptedDataKey) {

        CacheItem cache = makeSure(groupKey, encryptedDataKey);
        if (cache.getConfigCache().getMd5Utf8() == null || !cache.getConfigCache().getMd5Utf8().equals(md5Utf8)) {
            cache.getConfigCache().setMd5Utf8(md5Utf8);
            cache.getConfigCache().setLastModifiedTs(lastModifiedTs);
            cache.getConfigCache().setEncryptedDataKey(encryptedDataKey);
            //发布配置变更事件
            NotifyCenter.publishEvent(new LocalDataChangeEvent(groupKey));
        }
    }



    //其他内容省略
}






//接下来是LocalDataChangeEvent事件本身的内容
package com.cqfy.nacos.config.server.model.event;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/27
 * @Description:服务端配置变更事件，这个事件是让服务端通知客户端发生配置变更了
 */
public class LocalDataChangeEvent extends Event {

    public final String groupKey;

    public final boolean isBeta;

    public final List<String> betaIps;

    public final String tag;

    public final boolean isBatch;

    public final int delimiter;

    public LocalDataChangeEvent(String groupKey) {
        this.groupKey = groupKey;
        this.isBeta = false;
        this.betaIps = null;
        this.tag = null;
        this.isBatch = false;
        this.delimiter = 0;
    }

    public LocalDataChangeEvent(String groupKey, boolean isBeta, List<String> betaIps) {
        this.groupKey = groupKey;
        this.isBeta = isBeta;
        this.betaIps = betaIps;
        this.tag = null;
        this.isBatch = false;
        this.delimiter = 0;
    }

    public LocalDataChangeEvent(String groupKey, String tag) {
        this.groupKey = groupKey;
        this.isBeta = false;
        this.betaIps = null;
        this.tag = tag;
        this.isBatch = false;
        this.delimiter = 0;
    }

    public LocalDataChangeEvent(String groupKey, boolean isBatch, int delimiter) {
        this.groupKey = groupKey;
        this.isBeta = false;
        this.betaIps = null;
        this.tag = null;
        this.isBatch = isBatch;
        this.delimiter = delimiter;
    }
}
```

大家可以回顾回顾之前的内容，然后再继续往下看。好了，现在 nacos 配置中心服务端发布了两个配置变更事件，这是怎么回事呢？现在让我来为大家解释一下，LocalDataChangeEvent 本地数据变更事件是在 ConfigCacheService 对象中发布的，并且是在 ConfigCacheService 对象把数据库中的配置信息更新到缓存系统和本地文件系统时，发现配置发生变更了，就会发送该事件。在配置中心服务端中，这个事件的订阅器就是用来主动告诉客户端有配置发生变更了，也就是我们接下来要实现的功能。**而在 ConfigPublishRequestHandler 请求处理器中发布的 ConfigDataChangeEvent 事件则是针对配置中心集群来说的，这个事件的订阅者会把当前节点内变更的配置信息同步给集群的其他节点**。现在大家应该清楚这两个事件的具体作用了吧？

  

## 引入 RpcConfigChangeNotifier

LocalDataChangeEvent 事件的作用清楚了之后，接下来我们就可以为该事件定义对应的订阅者了，该订阅者接收到该事件之后，就会从 ConfigChangeListenContext 对象中查询监听了发生变更的配置信息的客户端，然后向客户端发送请求，通知这些客户端有服务实例发生变更了。这个 LocalDataChangeEvent 事件的订阅者我已经定义完毕了，就定义为了 RpcConfigChangeNotifier，具体实现请看请看下面代码块。

```
package com.cqfy.nacos.config.server.remote;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/26
 * @Description:配置变更事件订阅者，这个订阅者会接收并处理LocalDataChangeEvent事件，处理该事件的时候，就会主动向订阅了变更事件的客户端发送请求，通知客户端订阅的配置信息发生变更了
 */
@Component(value = "rpcConfigChangeNotifier")
public class RpcConfigChangeNotifier extends Subscriber<LocalDataChangeEvent> {


    //构造方法，在该方法中把当前订阅者对象注册到事件通知中心了
    public RpcConfigChangeNotifier() {
        NotifyCenter.registerSubscriber(this);
    }

    //配置信息变更监听器上下文对象
    @Autowired
    ConfigChangeListenContext configChangeListenContext;

    //RPC推送服务组件，在实现nacos注册中心服务端主动推送变更的服务实例信息时，我们已经引入了这个组件了
    @Autowired
    private RpcPushService rpcPushService;

    //客户端连接管理器
    @Autowired
    private ConnectionManager connectionManager;

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:接收并处理LocalDataChangeEvent事件的方法
     */
    @Override
    public void onEvent(LocalDataChangeEvent event) {
        //从配置变更事件中得到配置信息的组合键
        String groupKey = event.groupKey;
        //从事件中的得到发生变更的配置信息是否为灰度发布的配置信息
        boolean isBeta = event.isBeta;
        //得到灰度发布的服务器IP地址集合
        List<String> betaIps = event.betaIps;
        //解析配置信息的组合键
        String[] strings = GroupKey.parseKey(groupKey);
        //得到配置信息的dataId
        String dataId = strings[0];
        //得到所在组
        String group = strings[1];
        //得到租户信息
        String tenant = strings.length > 2 ? strings[2] : "";
        //得到配置信息的标签，当前在我们目前的代码中并没有为配置信息定义灰度发布地址和标签
        String tag = event.tag;
        //处理配置变更数据
        configDataChanged(groupKey, dataId, group, tenant, isBeta, betaIps, tag);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:处理配置变更数据的方法
     */
    public void configDataChanged(String groupKey, String dataId, String group, String tenant, boolean isBeta,
                                  List<String> betaIps, String tag) {

        //从配置变更监听器上下文中根据组合键得到该配置信息对应的监听器对象集合
        //其实就是监听了该配置信息的客户端连接ID的集合
        Set<String> listeners = configChangeListenContext.getListeners(groupKey);
        //如果没有得到对应的监听器对象，那就可以直接退出当前方法，没必要把变更的配置信息主动通知给客户端
        //因为很可能这些配置信息是从集群中的其他节点同步过来的，同步过来的配置信息在当前节点内当然没有对应的监听器对象
        //既然是这样，那就应该由同步这部分数据的源节点负责通知对应的客户端有配置信息发生变更了
        if (CollectionUtils.isEmpty(listeners)) {
            return;
        }
        //定义一个变量，记录通知的客户端的数量
        int notifyClientCount = 0;
        //遍历监听器集合
        for (final String client : listeners) {
            //根据每一个客户端ID从连接管理器中得到对应的客户端连接对象
            Connection connection = connectionManager.getConnection(client);
            //连接对象不存在，直接跳过本次循环
            if (connection == null) {
                continue;
            }
            //得到连接源信息
            ConnectionMeta metaInfo = connection.getMetaInfo();
            //得到客户端IP地址
            String clientIp = metaInfo.getClientIp();
            //得到客户端元数据中定义的标签
            String clientTag = metaInfo.getTag();
            //判断当前遍历的IP地址是否为灰度发布的IP地址，如果不是则跳过本次循环
            if (isBeta && betaIps != null && !betaIps.contains(clientIp)) {
                continue;
            }
            //如果为配置信息定义了标签，但是标签和客户端中定义的不想等，也要跳过本次循环
            if (StringUtils.isNotBlank(tag) && !tag.equals(clientTag)) {
                continue;
            }
            //接下来就是构建ConfigChangeNotifyRequest请求对象的操作了，这个请求对象就会封装发生了变更的配置信息的关键信息
            //然后把请求发送给客户端，客户端会根据请求中的信息主动从服务端查询最新的配置信息
            ConfigChangeNotifyRequest notifyRequest = ConfigChangeNotifyRequest.build(dataId, group, tenant);
            //创建一个PRC推送任务，用来把notifyRequest请求发送个客户端
            RpcPushTask rpcPushRetryTask = new RpcPushTask(notifyRequest, ConfigCommonConfig.getInstance().getMaxPushRetryTimes(), client, clientIp, metaInfo.getAppName());
            //执行任务，其实就是把任务提交给执行器去执行
            push(rpcPushRetryTask);
            //自增通知的客户端数量
            notifyClientCount++;
        }
        Loggers.REMOTE_PUSH.info("push [{}] clients, groupKey=[{}]", notifyClientCount, groupKey);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:该订阅者定义的事件类型
     */
    @Override
    public Class<? extends Event> subscribeType() {
        return LocalDataChangeEvent.class;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:推送任务
     */
    class RpcPushTask implements Runnable {

        //要推送的请求对象
        ConfigChangeNotifyRequest notifyRequest;

        //重试次数
        int maxRetryTimes = -1;

        //已尝试次数
        int tryTimes = 0;

        //要推送的客户端的连接ID
        String connectionId;

        //要推送的客户端的IP地址
        String clientIp;

        //客户端应用名称
        String appName;


        public RpcPushTask(ConfigChangeNotifyRequest notifyRequest, int maxRetryTimes, String connectionId, String clientIp, String appName) {

            this.notifyRequest = notifyRequest;
            this.maxRetryTimes = maxRetryTimes;
            this.connectionId = connectionId;
            this.clientIp = clientIp;
            this.appName = appName;
        }

        //消息推送操作是否已超过最大重试次数
        public boolean isOverTimes() {
            return maxRetryTimes > 0 && this.tryTimes >= maxRetryTimes;
        }


        /**
         * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
         * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
         * @Date:2024/10/29
         * @Description:任务要执行的操作
         */
        @Override
        public void run() {
            //自增重试次数
            tryTimes++;

            //执行RPC发送请求的操作
            rpcPushService.pushWithCallback(connectionId, notifyRequest, new PushCallBack() {

                @Override
                public long getTimeout() {
                    return 3000;
                }

                //请求发送成功后回调该方法
                @Override
                public void onSuccess() {
                    //记录推送成功日志
                    Loggers.REMOTE_PUSH.info("Push success, dataId={}, group={}, tenant={}, clientId={}", notifyRequest.getDataId(),
                            notifyRequest.getGroup(), notifyRequest.getTenant(), connectionId);
                }

                //请求发送失败后回调该方法
                @Override
                public void onFail(Throwable e) {
                    //记录推送失败日志
                    Loggers.REMOTE_PUSH.warn("Push fail, dataId={}, group={}, tenant={}, clientId={}", notifyRequest.getDataId(),
                                        notifyRequest.getGroup(), notifyRequest.getTenant(), connectionId, e);
                    //在这里再次调用push方法，重新尝试把请求发送给客户端
                    push(RpcPushTask.this);
                }

            }, ConfigExecutor.getClientConfigNotifierServiceExecutor());


        }

    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:该方法就是把任务对象提交给执行器执行的方法
     */
    private void push(RpcPushTask retryTask) {
        //得到要发送的请求对象
        ConfigChangeNotifyRequest notifyRequest = retryTask.notifyRequest;
        //判断重试次数是否已到最大值，达到最大值就意味着这个客户端可能和服务端的连接出现问题了，就不再重新发送了
        if (retryTask.isOverTimes()) {
            //记录操作日志
            Loggers.REMOTE_PUSH
                    .warn("push callback retry fail over times. dataId={},group={},tenant={},clientId={}, will unregister client.",
                            notifyRequest.getDataId(), notifyRequest.getGroup(), notifyRequest.getTenant(),
                            retryTask.connectionId);
            //注销该客户端连接
            connectionManager.unregister(retryTask.connectionId);
            //如果还有可用重试次数，先判断客户端连接是否还存在
        } else if (connectionManager.getConnection(retryTask.connectionId) != null) {
            //如果客户端连接还正常存在，就把任务重新提交给执行器执行，只不过每次执行的延迟时间会增长
            ConfigExecutor.getClientConfigNotifierServiceExecutor().schedule(retryTask, retryTask.tryTimes * 2, TimeUnit.SECONDS);
        } else {
            //客户端连接被注销了就什么也不做
        }
    }
}
```

到此为止，我们就把配置中心服务端主动通知客户端有配置发生变更的功能实现了。

## 引入 AsyncNotifyService

  

现在我们在文章开篇提出的三个功能的第一个就实现完毕了，接下来就该实现剩下的两个功能了，也就是下面这两个功能：

**2 在某个配置信息更新之后，要把最新的配置信息更新到服务端的缓存系统和文件系统中。**

**3 服务端把变更后的配置同步给集群的其他节点。**

虽然还剩下两个功能要实现，但是这两个功能实现起来非常简单，在源码中是组合在一起实现的。实际上这样的：ConfigPublishRequestHandler 请求处理器在处理了客户端发送过来的 ConfigPublishRequest 请求之后，就会发布一个 ConfigDataChangeEvent 配置变更事件。因为不管客户端执行的是什么操作，不管是新增配置信息、更新配置信息、移除配置信息，其实都意味着服务端存储的对应的配置信息要发生变更，所以要发布一个 ConfigDataChangeEvent 配置变更事件。而这个配置变更事件发布之后，有两个非常重要的操作要执行：**第一个就是把数据库中更新过的配置信息更新到服务端的缓存系统和文件系统中；第二个就是要把最新的配置信息同步给集群的其他节点**。这个逻辑大家应该可以理解吧？如果大家理解了这个逻辑，那么接下来我们为 ConfigDataChangeEvent 事件定义订阅器就容易多了，因为刚才分析的两个操作逻辑就是 ConfigDataChangeEvent 配置变更事件订阅器要执行的操作。

好了文字分析已经完毕了，接下来就从代码层面上实现一下吧。我已经为配置中心服务端定义了一个新的类，**就是标题中展示的 AsyncNotifyService 类**，我在这个类中定义了一个 ConfigDataChangeEvent 配置变更事件的订阅者，然后实现了这个订阅者处理 ConfigDataChangeEvent 事件的 onEvent() 方法。**在 onEvent() 方法中，程序会遍历 ServerMemberManager 集群成员管理器，得到配置中心集群中的所有成员，然后向每一个成员同步最新的配置信息。当然，当服务端在向集群中的每一个节点同步最新的配置信息时，如果发现要同步的目标节点就是自己，这个时候当前服务器就会直接执行把数据库中最新配置信息更新到缓存系统和本地文件系统的操作；如果不是自己，就会把最新配置的关键信息发送给集群的其他节点。也就是说，在 ConfigDataChangeEvent 事件的订阅器中，配置中心服务端就会把最新的配置信息更新到服务端的缓存系统和文件系统中，也会把变更后的配置同步给集群的其他节点**。大家可以品味品味这个逻辑，然后就可以看看我实现的 AsyncNotifyService 类了，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.notify;


//这个代码块中的内容比较多，但是逻辑很简单，按顺序阅读代码块中的内容就行。大家把重点放在AsyncNotifyService类的构造方法
//以及AsyncRpcTask内部类中即可。

/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:异步通知服务组件，该组件中定义了一个ConfigDataChangeEvent事件订阅者对象，所以只要事件通知中心发布了ConfigDataChangeEvent事件
 * 这个组件就会把变更的配置信息同步给集群的其他节点
 */
@Service
public class AsyncNotifyService {


    private static final Logger LOGGER = LoggerFactory.getLogger(AsyncNotifyService.class);

    //失败重试时间间隔
    private static final int MIN_RETRY_INTERVAL = 500;

    //每一次失败重试时增加的任务延迟执行时间
    private static final int INCREASE_STEPS = 1000;

    //最大的失败重试次数
    private static final int MAX_COUNT = 6;


    @Autowired
    private DumpService dumpService;

    //集群各个节点通信的客户端代理器，我把该类的内容也展示在当前代码块中了
    @Autowired
    private ConfigClusterRpcClientProxy configClusterRpcClientProxy;


    private ServerMemberManager memberManager;

    //存储节点状态的集合
    static final List<NodeState> HEALTHY_CHECK_STATUS = new ArrayList<>();

    //向节点状态集合中存放节点状态信息
    static {
        HEALTHY_CHECK_STATUS.add(NodeState.UP);
        HEALTHY_CHECK_STATUS.add(NodeState.SUSPICIOUS);
    }



    //构造方法，springboot容器创建该类的对象时，该方法就会被调用
    @Autowired
    public AsyncNotifyService(ServerMemberManager memberManager) {
        this.memberManager = memberManager;

        //在这里向注册中心创建了一个专门发布ConfigDataChangeEvent集群配置变更事件的发布器
        NotifyCenter.registerToPublisher(ConfigDataChangeEvent.class, NotifyCenter.ringBufferSize);

        //在这里向事件通知中心注册了一个专门订阅ConfigDataChangeEvent事件的订阅者，这个订阅者就会存放在刚才创建的ConfigDataChangeEvent事件发布器中
        NotifyCenter.registerSubscriber(new Subscriber() {

            //处理ConfigDataChangeEvent事件的方法
            @Override
            public void onEvent(Event event) {
                //判断接收到的是否为ConfigDataChangeEvent⌚️
                if (event instanceof ConfigDataChangeEvent) {
                    ConfigDataChangeEvent evt = (ConfigDataChangeEvent) event;
                    //如果是则从事件中得到变革配置的关键信息
                    long dumpTs = evt.lastModifiedTs;
                    String dataId = evt.dataId;
                    String group = evt.group;
                    String tenant = evt.tenant;
                    String tag = evt.tag;

                    //得到集群中的所有成员
                    Collection<Member> ipList = memberManager.allMembers();

                    //定义一个队列，这个队列存储的就是向集群中每一个节点发送配置变更通知请求的RPC任务
                    Queue<NotifySingleRpcTask> rpcQueue = new LinkedList<>();

                    //遍历集群成员
                    for (Member member : ipList) {
                        //为每一个成员创建一个NotifySingleRpcTask任务，然后把任务存储到刚才创建的队列中
                        //NotifySingleRpcTask并不是真正的任务，只是封装了变更的配置的关键信息，以及集群目标节点
                        //真正要被执行的任务是下面创建的AsyncRpcTask任务
                        rpcQueue.add(new NotifySingleRpcTask(dataId, group, tenant, tag, dumpTs, evt.isBeta, evt.isBatch, member));
                    }
                    if (!rpcQueue.isEmpty()) {
                        //判断队列中是否存在数据，如果存在就再定义一个异步通知RPC任务，然后把该任务提交给执行器执行
                        //执行器在执行该任务时，就会执行任务持有的队列中的每一个NotifySingleRpcTask任务
                        ConfigExecutor.executeAsyncNotify(new AsyncRpcTask(rpcQueue));
                    }
                }
            }

            //该订阅者感兴趣的事件
            @Override
            public Class<? extends Event> subscribeType() {
                return ConfigDataChangeEvent.class;
            }
        });
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:判断目标ID地址对应的节点是否处于不健康状态
     */
    private boolean isUnHealthy(String targetIp) {
        return !memberManager.stateCheck(targetIp, HEALTHY_CHECK_STATUS);
    }


    class AsyncRpcTask implements Runnable {


        private Queue<NotifySingleRpcTask> queue;


        public AsyncRpcTask(Queue<NotifySingleRpcTask> queue) {
            this.queue = queue;
        }


        @Override
        public void run() {
            //循环判断队列中是否还有NotifySingleRpcTask任务对象，如果没有则退出循环
            while (!queue.isEmpty()) {
                //从队列中取出任务处理
                NotifySingleRpcTask task = queue.poll();
                //创建ConfigChangeClusterSyncRequest请求对象，这个请求对象会封装发生变更的配置的关键信息，然后传输给集群的其他节点
                ConfigChangeClusterSyncRequest syncRequest = new ConfigChangeClusterSyncRequest();
                //设置配置dataId
                syncRequest.setDataId(task.getDataId());
                //设置配置所在组
                syncRequest.setGroup(task.getGroup());
                //设置是否为灰度发布配置
                syncRequest.setBeta(task.isBeta);
                //设置配置信息最新修改时间
                syncRequest.setLastModified(task.getLastModified());
                //设置配置信息标签
                syncRequest.setTag(task.tag);
                //设置是否为批处理任务
                syncRequest.setBatch(task.isBatch);
                //设置租户信息
                syncRequest.setTenant(task.getTenant());

                //得到要把请求发送给的目标节点
                Member member = task.member;
                //判断目标节点是否是自己本身
                if (memberManager.getSelf().equals(member)) {
                    //如果是自己本身，那就执行更新自己缓存系统和本地文件系统中配置信息的操作
                    //现在大家应该彻底梳理清楚了，如果服务端存储的配置信息发生变更了，肯定是数据库中的信息先发生变更，然后在ConfigPublishRequestHandler处理器中
                    //发布一个ConfigDataChangeEvent事件，表示有配置信息发生变更了，接下来ConfigDataChangeEvent事件的订阅者就会接收并处理该事件
                    //在处理该事件的过程中就会判断当前处理该事件的是否为本身这个节点，如果是，则直接根据数据库中最新的配置信息更新缓存系统和本地文件系统中的配置信息
                    //如果目标节点不是自身，那就意味着要把配置变更信息同步给集群的其他节点
                    dumpService.dumpFormal(syncRequest.getDataId(), syncRequest.getGroup(), syncRequest.getTenant(),
                            syncRequest.getLastModified(), NetUtils.localIP());

                    continue;
                }

                //执行到这里就意味着要把配置变更信息同步给集群的其他节点
                //得到通知集群其他节点要执行操作的类型
                String event = getNotifyEvent(task);
                //判断目标节点是否在集群中
                if (memberManager.hasMember(member.getAddress())) {
                    //判断目标节点是否不健康
                    boolean unHealthNeedDelay = isUnHealthy(member.getAddress());
                    if (unHealthNeedDelay) {
                        //不健康则记录日志信息
                        ConfigTraceService.logNotifyEvent(task.getDataId(), task.getGroup(), task.getTenant(), null,
                                task.getLastModified(), InetUtils.getSelfIP(), event,
                                ConfigTraceService.NOTIFY_TYPE_UNHEALTH, 0, member.getAddress());
                        //异步延迟执行任务，其实就是等待一段时间再发送请求给目标节点，也许到时候目标节点就恢复健康了
                        asyncTaskExecute(task);
                    } else {
                        try {//目标节点健康的话，直接就可以把请求发送给目标节点，并且定义了一个回调对象
                            configClusterRpcClientProxy.syncConfigChange(member, syncRequest, new AsyncRpcNotifyCallBack(task));
                        } catch (Exception e) {
                            //出现异常则稍后重试
                            asyncTaskExecute(task);
                        }
                    }
                } else {
                    //目标节点不在集群中则什么都不做
                }

            }
        }
    }



    //该任务就是用来封装变更配置的关键信息的，并没有真正的执行逻辑，也就是没有run方法
    //大家简单看看就行
    static class NotifySingleRpcTask extends NotifyTask {


        private Member member;

        private boolean isBeta;


        private String tag;

        private boolean isBatch;


        public NotifySingleRpcTask(String dataId, String group, String tenant, String tag, long lastModified,
                                   boolean isBeta, Member member) {
            super(dataId, group, tenant, lastModified);
            this.member = member;
            this.isBeta = isBeta;
            this.tag = tag;
        }


        public NotifySingleRpcTask(String dataId, String group, String tenant, String tag, long lastModified,
                                   boolean isBeta, boolean isBatch, Member member) {
            super(dataId, group, tenant, lastModified);
            this.member = member;
            this.isBeta = isBeta;
            this.tag = tag;
            this.isBatch = isBatch;
        }


        public boolean isBeta() {
            return isBeta;
        }


        public void setBeta(boolean beta) {
            isBeta = beta;
        }


        public String getTag() {
            return tag;
        }


        public void setTag(String tag) {
            this.tag = tag;
        }


        public boolean isBatch() {
            return isBatch;
        }


        public void setBatch(boolean batch) {
            isBatch = batch;
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:异步重新向集群目标节点同步最新配置信息的方法
     */
    private void asyncTaskExecute(NotifySingleRpcTask task) {
        //计算下一次任务延迟执行的时间
        int delay = getDelayTime(task);
        Queue<NotifySingleRpcTask> queue = new LinkedList<>();
        queue.add(task);
        AsyncRpcTask asyncTask = new AsyncRpcTask(queue);
        ConfigExecutor.scheduleAsyncNotify(asyncTask, delay, TimeUnit.MILLISECONDS);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:得到通知集群其他节点要执行操作的类型的方法
     */
    private String getNotifyEvent(NotifySingleRpcTask task) {

        String event = ConfigTraceService.NOTIFY_EVENT;
        if (task.isBeta) {
            event = ConfigTraceService.NOTIFY_EVENT_BETA;
        } else if (!StringUtils.isBlank(task.tag)) {
            event = ConfigTraceService.NOTIFY_EVENT_TAG + "-" + task.tag;
        } else if (task.isBatch) {
            event = ConfigTraceService.NOTIFY_EVENT_BATCH;
        }
        return event;
    }


    class AsyncRpcNotifyCallBack implements RequestCallBack<ConfigChangeClusterSyncResponse> {


        private NotifySingleRpcTask task;


        public AsyncRpcNotifyCallBack(NotifySingleRpcTask task) {
            this.task = task;
        }


        @Override
        public Executor getExecutor() {
            return ConfigExecutor.getConfigSubServiceExecutor();
        }


        @Override
        public long getTimeout() {
            return 1000L;
        }


        //该方法会在收到集群节点回复的响应后被回调，主要就是判断请求是否发送成功了，不成功则重新执行，大家简单看看就行
        @Override
        public void onResponse(ConfigChangeClusterSyncResponse response) {

            String event = getNotifyEvent(task);
            long delayed = System.currentTimeMillis() - task.getLastModified();
            if (response.isSuccess()) {
                ConfigTraceService.logNotifyEvent(task.getDataId(), task.getGroup(), task.getTenant(), null,
                        task.getLastModified(), InetUtils.getSelfIP(), event, ConfigTraceService.NOTIFY_TYPE_OK,
                        delayed, task.member.getAddress());
            } else {
                LOGGER.error("[notify-error] target:{} dataId:{} group:{} ts:{} code:{}", task.member.getAddress(),
                        task.getDataId(), task.getGroup(), task.getLastModified(), response.getErrorCode());
                ConfigTraceService.logNotifyEvent(task.getDataId(), task.getGroup(), task.getTenant(), null,
                        task.getLastModified(), InetUtils.getSelfIP(), event, ConfigTraceService.NOTIFY_TYPE_ERROR,
                        delayed, task.member.getAddress());
                //执行失败则重试
                asyncTaskExecute(task);
                LogUtil.NOTIFY_LOG.error("[notify-retry] target:{} dataId:{} group:{} ts:{}", task.member.getAddress(),
                        task.getDataId(), task.getGroup(), task.getLastModified());

            }
        }

        @Override
        public void onException(Throwable ex) {
            String event = getNotifyEvent(task);
            long delayed = System.currentTimeMillis() - task.getLastModified();
            LOGGER.error("[notify-exception] target:{} dataId:{} group:{} ts:{} ex:{}", task.member.getAddress(),
                    task.getDataId(), task.getGroup(), task.getLastModified(), ex);
            ConfigTraceService.logNotifyEvent(task.getDataId(), task.getGroup(), task.getTenant(), null,
                    task.getLastModified(), InetUtils.getSelfIP(), event, ConfigTraceService.NOTIFY_TYPE_EXCEPTION,
                    delayed, task.member.getAddress());
            asyncTaskExecute(task);
            LogUtil.NOTIFY_LOG.error("[notify-retry] target:{} dataId:{} group:{} ts:{}", task.member.getAddress(),
                    task.getDataId(), task.getGroup(), task.getLastModified());

        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:计算下一次任务延迟执行的时间的方法
     */
    private static int getDelayTime(NotifyTask task) {
        int failCount = task.getFailCount();
        int delay = MIN_RETRY_INTERVAL + failCount * failCount * INCREASE_STEPS;
        if (failCount <= MAX_COUNT) {
            task.setFailCount(failCount + 1);
        }
        return delay;
    }
}





//ConfigClusterRpcClientProxy类的内容如下
package com.cqfy.nacos.config.server.remote;

/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:配置中心集群用来向集群其他节点发送请求的客户端代理组件
 */
@Service
public class ConfigClusterRpcClientProxy {

    final ClusterRpcClientProxy clusterRpcClientProxy;


    public ConfigClusterRpcClientProxy(ClusterRpcClientProxy clusterRpcClientProxy) {
        this.clusterRpcClientProxy = clusterRpcClientProxy;
    }

    //把请求发送给集群中目标节点的方法
    public void syncConfigChange(Member member, ConfigChangeClusterSyncRequest request, RequestCallBack callBack) throws NacosException {
        clusterRpcClientProxy.asyncRequest(member, request, callBack);

    }
}
```

到此为止，我们就把配置中心服务端把最新的配置信息更新到缓存系统和文件系统中，以及把变更后的配置同步给集群的其他节点的功能就实现完毕了。当然，要说完全实现了也不太正确，**因为我们只看到了服务端把最新的配置信息更新到缓存系统和文件系统中的操作是如何发起的，真正的执行过程我们也不完全清楚。我们能看到服务端把变更后的配置同步给集群的其他节点的操作时如何执行的，但并不知道目标节点接收到同步过来的数据后该如何处理**。所以，接下来就让我来为大家分析一下这两个功能的后续操作。

首先还是分析服务端把最新的配置信息更新到缓存系统和文件系统中的后续操作，从上面代码块中我们可以发现，当前服务器开始执行把数据库中的最新信息更新到缓存系统和文件系统中时，执行了这样一行代码，请看下面代码块。

```
//如果是自己本身，那就执行更新自己缓存系统和本地文件系统中配置信息的操作
//现在大家应该彻底梳理清楚了，如果服务端存储的配置信息发生变更了，肯定是数据库中的信息先发生变更，然后在ConfigPublishRequestHandler处理器中
//发布一个ConfigDataChangeEvent事件，表示有配置信息发生变更了，接下来ConfigDataChangeEvent事件的订阅者就会接收并处理该事件
//在处理该事件的过程中就会判断当前处理该事件的是否为本身这个节点，如果是，则直接根据数据库中最新的配置信息更新缓存系统和本地文件系统中的配置信息
//如果目标节点不是自身，那就意味着要把配置变更信息同步给集群的其他节点
dumpService.dumpFormal(syncRequest.getDataId(), syncRequest.getGroup(), syncRequest.getTenant(),syncRequest.getLastModified(), NetUtils.localIP());
```

好了，现在程序执行到 DumpService 对象的 dumpFormal() 方法中了，那我们就进入该方法，看一看该方法究竟执行了什么操作，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.dump;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/28
 * @Description:本地和内存配置信息更新组件，这个组件会定期把存储到数据库中的配置信息更新到缓存以及本地硬盘的文件中
 */
@SuppressWarnings("PMD.AbstractClassShouldStartWithAbstractNamingRule")
public abstract class DumpService {


    //省略该类其他内容

    //把发生变更的配置信息更新到缓存系统和本地文件系统的任务处理器
    protected DumpProcessor processor;

    //配置变更数据更新任务管理器，这个任务管理器会在服务端存储的配置发生变更后开始执行任务
    //processor就会交给dumpTaskMgr这个延迟任务执行引擎使用
    private TaskManager dumpTaskMgr;


    //构造方法
    public DumpService(ConfigInfoPersistService configInfoPersistService, HistoryConfigInfoPersistService historyConfigInfoPersistService, ServerMemberManager memberManager) {

        //省略该方法其他内容
        
        this.processor = new DumpProcessor(this);
        this.dumpTaskMgr = new TaskManager("com.cqfy.nacos.server.DumpTaskManager");
        this.dumpTaskMgr.setDefaultTaskProcessor(processor);
        
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:把执行配置信息更新到缓存系统和本地文件系统的方法
     */
    public void dumpFormal(String dataId, String group, String tenant, long lastModified, String handleIp) {
        //得到配置信息的组合key
        String groupKey = GroupKey2.getKey(dataId, group, tenant);
        String taskKey = dataId + group + tenant;
        //向任务管理器中添加DumpTask任务，DumpTask是一个延迟执行任务，两个相同配置信息的DumpTask任务是可以合并的，假如某个配置信息变更比较频繁
        //那么再把该配置信息的最新信息更新到缓存系统和本地文件系统时，就可以使用延迟任务的方式更新，相同的配置信息任务可以合并，可以减少重复操作
        dumpTaskMgr.addTask(taskKey, new DumpTask(groupKey, false, false, false, null, lastModified, handleIp));
        DUMP_LOG.info("[dump] add formal task. groupKey={}", groupKey);
    }
    
}


//下面是DumpTask类的内容
package com.cqfy.nacos.config.server.service.dump.task;


public class DumpTask extends AbstractDelayTask {

    public DumpTask(String groupKey, boolean isBeta, boolean isBatch, boolean isTag, String tag, long lastModified,
                    String handleIp) {
        this.groupKey = groupKey;
        this.lastModified = lastModified;
        this.handleIp = handleIp;
        this.isBeta = isBeta;
        if (isTag) {
            this.tag = tag;
        } else {
            this.tag = null;
        }
        this.isBatch = isBatch;

        //retry interval: 1s
        setTaskInterval(1000L);
    }

    @Override
    public void merge(AbstractDelayTask task) {
    }

    final String groupKey;

    final long lastModified;

    final String handleIp;

    final boolean isBeta;

    final String tag;

    final boolean isBatch;

    public String getGroupKey() {
        return groupKey;
    }

    public long getLastModified() {
        return lastModified;
    }

    public String getHandleIp() {
        return handleIp;
    }

    public boolean isBeta() {
        return isBeta;
    }

    public String getTag() {
        return tag;
    }

    public boolean isBatch() {
        return isBatch;
    }
}

```

通过上面代码块我们可以知道，**DumpTask 任务提交给 TaskManager 延迟任务执行引擎之后，最终会被 DumpProcessor 处理器处理，那么该处理器会怎么处理 DumpTask 任务呢？其实到这里逻辑应该很清楚了，肯定就是根据 DumpTask 对象中封装的发生变更的配置的关键信息去数据库中查询最新的配置信息，然后把该配置信息更新到服务端的文件系统和缓存系统中**。而 DumpProcessor 处理器也正是这么做的，接下来就请大家看一看 DumpProcessor 处理器中的内容，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.dump.processor;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:把发生变更的配置信息更新到缓存系统和本地文件系统的任务处理器
 */
public class DumpProcessor implements NacosTaskProcessor {


    final DumpService dumpService;

    final ConfigInfoPersistService configInfoPersistService;


    public DumpProcessor(DumpService dumpService) {
        this.dumpService = dumpService;
        this.configInfoPersistService = dumpService.getConfigInfoPersistService();
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:处理DumpTask任务的方法
     */
    @Override
    public boolean process(NacosTask task) {
        //转换为DumpTask对象
        DumpTask dumpTask = (DumpTask) task;
        //从任务对象中得到配置的关键信息
        String[] pair = GroupKey2.parseKey(dumpTask.getGroupKey());
        //得到dataId
        String dataId = pair[0];
        //得到所在组
        String group = pair[1];
        //得到租户信息
        String tenant = pair[2];
        //得到最后修改时间
        long lastModifiedOut = dumpTask.getLastModified();
        //得到服务器的IP地址，这个IP地址可能是本机IP地址，也可能是集群中同步配置信息过来的节点的IP地址
        String handleIp = dumpTask.getHandleIp();
        //是否为灰度发布配置信息
        boolean isBeta = dumpTask.isBeta();
        //配置信息定义的标签
        String tag = dumpTask.getTag();
        //是否为批处理任务
        boolean isBatch = dumpTask.isBatch();
        //在这里创建一个ConfigDumpEvent事件构建器，并且把要更新的配置的关键信息封装在事件中，该事件对象会被DumpConfigHandler处理器处理
        ConfigDumpEvent.ConfigDumpEventBuilder build = ConfigDumpEvent.builder().namespaceId(tenant).dataId(dataId)
                .group(group).isBatch(isBatch).isBeta(isBeta).tag(tag).handleIp(handleIp);
        //定义配置类型，这个就是正式的意思，也就是说既不是灰度发布，也没有定义标签
        String type = "formal";
        //如果是灰度发布则更新配置类型，但是目前定义的这些信息在方法中都用不到，大家可以不必关心
        if (isBeta) {
            type = "beta";
            //如果定义了标签则更新配置类型
        } else if (StringUtils.isNotBlank(tag)) {
            type = "tag-" + tag;
        }
        LogUtil.DUMP_LOG.info("[dump] process {} task. groupKey={}", type, dumpTask.getGroupKey());



        //执行到这里就要从mysql数据库中查询对应的最新配置信息了
        ConfigInfoWrapper cf = configInfoPersistService.findConfigInfo(dataId, group, tenant);
        //解析来把从数据库中查询到的配置信息设置到ConfigDumpEventBuilder对象中
        //如果没有查到对应的配置信息，就意味着接下来要执行的是从缓存系统和本地文件系统移除对应配置信息的操作
        build.remove(Objects.isNull(cf));
        build.content(Objects.isNull(cf) ? null : cf.getContent());
        build.type(Objects.isNull(cf) ? null : cf.getType());
        build.encryptedDataKey(Objects.isNull(cf) ? null : cf.getEncryptedDataKey());
        build.lastModifiedTs(Objects.isNull(cf) ? lastModifiedOut : cf.getLastModified());
        //构建ConfigDumpEvent事件，然后把事件交给DumpConfigHandler处理器处理
        //我把DumpConfigHandler类的内容也展示在当前代码块中了
        return DumpConfigHandler.configDump(build.build());

    }
}





//以下是DumpConfigHandler类的内容
package com.cqfy.nacos.config.server.service.dump;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:ConfigDumpEvent事件处理器，该处理就会把ConfigDumpEvent事件中封装的配置信息更新到缓存系统和本地文件系统中
 */
public class DumpConfigHandler extends Subscriber<ConfigDumpEvent> {



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/29
     * @Description:更新配置信息到缓存系统和本地文件系统的方法
     */
    public static boolean configDump(ConfigDumpEvent event) {
        //得到配置信息的dataId
        final String dataId = event.getDataId();
        //得到所在组
        final String group = event.getGroup();
        //得到命名空间
        final String namespaceId = event.getNamespaceId();
        //得到配置信息本身的内容
        final String content = event.getContent();
        //得到配置信息类型
        final String type = event.getType();
        //得到配置信息最新修改时间
        final long lastModified = event.getLastModifiedTs();

        //定义操作结果
        boolean result;
        //判断事件类型，判断是不是移除配置信息操作
        if (!event.isRemove()) {
            //如果不是移除操作，那就直接把最新的配置信息更新到缓存系统和本地文件系统中
            result = ConfigCacheService.dump(dataId, group, namespaceId, content, lastModified, event.getType(),
                    event.getEncryptedDataKey());
            if (result) {
                //操作成功记录日志信息
                ConfigTraceService.logDumpEvent(dataId, group, namespaceId, null, lastModified, event.getHandleIp(),
                        ConfigTraceService.DUMP_TYPE_OK, System.currentTimeMillis() - lastModified, content.length());
            }
        } else {
            //如果是移除操作，直接执行即可
            result = ConfigCacheService.remove(dataId, group, namespaceId);
            if (result) {
                //操作成功记录日志信息
                ConfigTraceService.logDumpEvent(dataId, group, namespaceId, null, lastModified, event.getHandleIp(),
                        ConfigTraceService.DUMP_TYPE_REMOVE_OK, System.currentTimeMillis() - lastModified, 0);
            }
        }
        return result;

    }

    @Override
    public void onEvent(ConfigDumpEvent event) {
        configDump(event);
    }

    @Override
    public Class<? extends Event> subscribeType() {
        return ConfigDumpEvent.class;
    }
}
```

到此为止，大家应该彻底清楚了配置中心服务端在感知到配置变更之后，把配置信息更新到缓存系统和文件系统中的执行流程了吧？大家可以再看看文章中展示的代码，或者直接去第十八版本代码中查看，然后再自己梳理梳程序执行的整个流程，自己总结一下，这样会掌握得更牢固。

好了，现在大家已经清楚了服务端把配置信息更新到缓存系统和文件系统中的执行流程了，那接下来就该看看集群中的其他节点接收到同步过来的配置信息后应该执行的操作。在我之前为大家展示的 AsyncNotifyService 类中，**我们会发现当前节点把配置信息同步给集群的其他节点时，同步的也仅仅是发生变更的配置的关键信息，并不是配置信息本身的内容。而变更的配置的关键信息会被封装到 ConfigChangeClusterSyncRequest 请求对象中发送给目标节点**，大家可以回顾回顾 AsyncNotifyService 类中的内容。既然是这样，**那我们直接为 ConfigChangeClusterSyncRequest 请求定义一个请求处理器不就行了吗？在请求处理器中根据其他节点同步过来的配置的关键信息，去数据库中查询该配置的最新信息，然后把最新信息更新到目标节点的缓存系统和文件系统中**。这个逻辑大家也可以理解吧？当然，也不用太麻烦，只要我们调用了 DumpService 对象的 dumpFormal() 方法，那么以上我们分析的内容不就都会被执行吗？我刚才可是刚刚带领大家梳理了 DumpService 对象的 dumpFormal() 方法要执行的整个流程。**只要 DumpService 对象的 dumpFormal() 方法一被执行，那么程序最终就会根据变更的配置的关键信息去数据库中查询该配置的最新信息，然后把最新信息更新到缓存系统和文件系统中**。所以，**我们只需要在要定义的 ConfigChangeClusterSyncRequest 请求的处理器中调用 DumpService 对象的 dumpFormal() 方法即可**。我把专门处理 ConfigChangeClusterSyncRequest 请求的处理器定义为了 ConfigChangeClusterSyncRequestHandler，具体内容请看下面代码块。

```
package com.cqfy.nacos.config.server.remote;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/29
 * @Description:处理集群配置变更请求的处理器，该处理器会处理集群其他节点传输过来的配置信息变更请求，然后根据请求中的信息，更新自己保存的配置信息
 */
@Component
public class ConfigChangeClusterSyncRequestHandler
        extends RequestHandler<ConfigChangeClusterSyncRequest, ConfigChangeClusterSyncResponse> {

    private final DumpService dumpService;


    public ConfigChangeClusterSyncRequestHandler(DumpService dumpService) {
        this.dumpService = dumpService;
    }


    @Override
    public ConfigChangeClusterSyncResponse handle(ConfigChangeClusterSyncRequest configChangeSyncRequest, RequestMeta meta) throws NacosException {

        //从其他节点传输过来的请求中获得发生了变更的配置信息的dataId等等信息，然后就可以根据这些信息直接去mysql数据库中查询最新的配置信息，把最新的配置信息更新到缓存系统和本地文件系统了
        //也许大家会对此感到疑惑，为什么集群中的其他节点也可以去存储着最新配置信息的数据库中查询配置信息呢？
        //其实是这样的，构建nacos配置中心集群时，如果使用的是mysql集群，那么集群中所有的节点都要访问同一个mysql数据库
        //mysql数据库可以自己构建集群实现数据的备份。而集群中每一个节点内存和文件系统中的数据需要由其他节点同步。
        //也就是说，从数据库层面来说，集群中每一个节点都可以访问同一个数据库，从数据库中查询到的都是最新的配置信息
        //但是集群中各个节点内存和文件系统中缓存的配置信息是不是最新的，这就要依赖其他节点同步信息过来了
        //大家要理清楚这个逻辑
        //假如配资中心服务器使用derby数据库存储信息，因为该数据库是嵌入在程序内部的，和每一个服务器绑定
        //所以这种情况下构建集群，就需要使用raft共识算法保证数据的一致性，这个功能会在nacos支线内容中实现
        //我就不在本章中详细讲解了
        dumpService.dumpFormal(configChangeSyncRequest.getDataId(), configChangeSyncRequest.getGroup(), configChangeSyncRequest.getTenant(), configChangeSyncRequest.getLastModified(), meta.getClientIp());

        //返回成功响应
        return new ConfigChangeClusterSyncResponse();
    }

}
```

到此为止，nacos 配置中心的核心功能就全部实现完毕了。nacos 整个框架的核心功能也都实现完毕了，也就是主线内容全部讲解完了，当然，还有很多支线内容没有实现，这并不是因为支线内容难，而是因为支线内容也很多，光现在展示出来的支线内容的文章目录就有十篇文章了，还有什么 TPS 控制器，web 界面和后端交互的功能都没实现呢，如果把支线内容都更新完毕了，少说还得二十篇文章。但我们眼下肯定没有这么充裕的时间了。我相信只要一篇文章接一篇文章看到这里的朋友，都忽觉的 nacos 这个框架非常简单，之所以更新了四十多章，是因为这个框架的内容比较多。我相信大家心里肯定会有这种感觉。所以我希望大家能在理解了文章内容、掌握了我提供的十八个版本代码的基础上，可以自己尝试着去看看 nacos 的源码，把我们文章和我提供的代码中没有涉及到的部分看一看，如果你发现源码看起来也非常容易，那么恭喜你，nacos 这个框架对你来说已没有什么神秘可言了。好了朋友们，nacos 框架的主线内容就到此为止吧，接下来就该更新 rocketmq 这个框架了，这个框架的内容更多，大家可以休息一段时间，然后跟我一起探索新的知识领域吧，朋友们，我们 rocketmq 见！