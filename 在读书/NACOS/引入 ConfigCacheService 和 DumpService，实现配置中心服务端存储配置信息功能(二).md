上一章我们已经实现了配置中心服务端把客户端发布的配置信息存储在 mysql 数据库中的功能，这个功能实现起来非常简单，我们基本上是一路平推，没遇到任何困难就把相应功能实现了。不过，这个功能的的实现，也只能算是完成了服务端存储配置信息的第一步，后面还有一些很重要的功能没有实现呢。我在上一章结尾也更大家预先透漏了：**在 nacos 配置中心源码中，除了把客户端发布过来的配置信息持久化到数据库中之外，还会把配置信息缓存在内存中，同时也会把配置信息保存在本地硬盘的对应文件中**。现在，我们先不管这两个操作是如何实现的，我认为我们首先应该思考的是，为什么要执行这两个操作呢？

如果仅仅从数据备份的角度来考虑，除了把配置信息存储在数据库中，同时也在内存中缓存一份，还在本地硬盘文件中存储一份，这些操作无疑显著降低了数据丢失的风险。但我们都知道，如果不想让配置信息丢失，完全可以给 mysql 数据库构建集群，让 mysql 集群自己备份数据，这种做法也是很常规，而且很简便的。那在 nacos 源码中为什么还要在本地和缓存中保存配置信息呢？接下来就让我为大家详细解释一下吧。

## 引入 ConfigCacheService 和 ConfigDiskServiceFactory

我相信其实就算我不解释，大家应该也能很容易就理解把配置信息缓存在内存中的操作。**把信息缓存在内存中只有一个原因，那就是查询起来更快**！举一个很直接的例子，假如配置中心客户端想从服务端查询自己监听的配置信息有没有发生变更，那客户端其实就可以把自己监听的配置信息的 MD5 的值发送给服务端，服务端直接使用客户端发送过来的配置信息的 MD5 的值和自己保存的对应配置信息的 MD5 的值做对比，如果不一样就意味着配置可能发生变更了。但在上一章实现配置信息存储功能时，**我们把客户端传输过来的配置信息存储在数据库中了，配置信息的 MD5 的值也存储在数据库中了**。**这就意味着假如客户端每次向服务端查询有没有配置信息发生变更，服务端就要访问数据库**，从数据库中得到配置信息的 MD5 的值，然后再和客户端传输过来的 MD5 的值做对比。这显然就有点麻烦了，如果我们把配置信息的 MD5 的值缓存在内存中，那么客户端向服务端查询配置信息是否发生变更时，只需要直接和服务端内存中保存的配置信息的 MD5 的值做对比即刻。所以，为了提升服务端工作效率，我们确实有必要把配置信息在内存中缓存一份。这个逻辑应该可以理解吧？如果大家理解这个操作了，那接下来就有另一个非常重要的问题，**那就是当我们把配置信息缓存在配置中心服务端的内存中时，需要把配置信息本身的内容也缓存到内存中吗**？

我的回答非常干脆，那就是肯定不能把配置信息本身的内容也缓存在内存中。配置信息的内容本身就比较丰富，如果多个客户端向服务端发布了非常多的配置信息，这些配置信息全缓存在内存中，很容易造成内存溢出的情况。所以，**我们没必要把配置信息本身的内容缓存在内存中，只缓存配置的关键信息即可，比如配置信息的 dataId，所在组，租户信息，MD5 的值，密钥，配置类型以及配置信息最新的修改时间等等**。这样一来，客户端要从服务端查询配置是否发生变更，就可以直接和服务端内存中缓存的配置信息的 MD5 的值做对比了。这个逻辑应该也很容易理解吧？

当然，看到这里有些朋友肯定还是会有些疑惑，如果配置中心客户端就想从服务端查询指定的配置信息呢？在实现配置中心客户端的时候，我们确实实现了这个功能。那这个时候，**客户端把获取指定配置信息的请求发送给服务端了，可是服务端并没有把配置信息本身的内容缓存在内存中，这样一来，服务端还是需要远程访问 mysql 数据库，从数据库查询指定的配置信息，然后把配置信息返回给客户端**。这么来看的话，服务端把配置信息缓存在内容中的操作似乎又没有发挥什么作用。那这又改如何理解呢？这时候就要从本地文件保存配置信息的功能说起了。

请大家想象一个场景，目前我们已经把配置信息存储在 mysql 数据库中了，并没有把配置信息本身的内容缓存在内存中，也就是说，**现在 mysql 数据库既然负责存储数据，也就是写数据操作，也要负责读取数据的操作，当多个客户端更新和查询数据比较频繁的时候，mysql 数据库及要负责读又要负责写，那么数据库的压力显然就会很大，那这时候该怎么办呢**？这时候就轮到本地文件保存配置信息的功能登场了，**为了减轻数据库的压力，我们就可以把配置信息在配置中心服务器的本地硬盘上缓存一份，这样一来当客户端向服务端查询指定配置信息时，也就是读取信息时，就可以直接从服务端的硬盘上读取，而不必访问远程数据库了**。大多数时候数据库只负责写数据，这样一来压力不就小很多了吗？我想这个逻辑理解起来也很简单。现在大家就应该清楚了 nacos 配置中心服务端为什么要把配置信息在缓存和本地文件中都保存一份了吧。如果这些操作大家都理解了，那接下来就应该思考思考，怎么把配置信息缓存在内存以及持久化到本地硬盘的对应文件中。

这就很简单了，都是老一套的内容了，这些内容大家在 hippo4j 动态线程池框架中肯定见过。**如果我们想把配置的关键信息缓存在内存中，那就可以定义一个 CacheItem 缓存项，每一个缓存项对象都封装着一个配置的关键信息，然后再定义一个 ConfigCacheService 配置信息缓存组件，在这个类中定义一个 map，map 的 key 就是配置信息 dataId + group + tenant 的组合键，这三个值就可以确定一个唯一的配置信息，value 就是该配置信息对应的 CacheItem 缓存项对象。这样一来，就变成了配置的关键信息保存在 CacheItem 对象中，而 CacheItem 对象有保存在 ConfigCacheService 对象中，通过 ConfigCacheService 配置信息缓存组件，服务端就可以获得所有配置信息的关键信息**。大家可以品味品味这个逻辑，接下来就让我为大家把已经定义完毕的 CacheItem 和 ConfigCacheService 类展示一下。

首先是 CacheItem 类，请看下面代码块。

```
package com.cqfy.nacos.config.server.model;


public class CacheItem {

    //配置信息组合键
    final String groupKey;

    //配置信息类型
    public String type;

    //配置信息的关键信息，比如md5的值，密钥，最新修改时间，都存储在这个ConfigCache对象中
    //我把ConfigCache类的信息也展示在当前代码块中了
    ConfigCache configCache = new ConfigCache();

    //提供读写锁的成员变量，这个提供读写锁的成员变量需要解释一下。请大家想象一个场景，假如web控制台
    //正在修改某个配置信息，而客户端要读取这个配置信息，这种情况下就需要使用读写锁来解决对应的配置信息的并发问题
    //每一个配置信息的缓存项中都为对应的配置信息提供了一个读写锁成员变量，当该配置信息发生变更时
    //就可以尝试获取写锁，当要读取配置信息时，就可以获取读锁。而读锁和写锁不能同时被获得。这种操作应该也很常见吧？
    private final SimpleReadWriteLock rwLock = new SimpleReadWriteLock();

    //构造方法
    public CacheItem(String groupKey, String encryptedDataKey) {
        this.groupKey = StringPool.get(groupKey);
        //把密钥设置到configCache对象中
        this.getConfigCache().setEncryptedDataKey(encryptedDataKey);
    }

    public ConfigCache getConfigCache() {
        return configCache;
    }

    public SimpleReadWriteLock getRwLock() {
        return rwLock;
    }


    //该类的其他内容省略
}



//下面就是ConfigCache类的内容


package com.cqfy.nacos.config.server.model;


public class ConfigCache implements Serializable {

    //GBK编码的MD5的值
    volatile String md5Gbk = Constants.NULL;

    //Utf8编码的MD5的值
    volatile String md5Utf8 = Constants.NULL;

    //配置信息密钥
    volatile String encryptedDataKey;

    //配置信息最新修改时间
    volatile long lastModifiedTs;


    //get、set方法省略
}
```

好了，CacheItem 配置项的内容展示完毕了，接下来就该展示 ConfigCacheService 类的内容了，请看下面代码块。

```
package com.cqfy.nacos.config.server.service;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/28
 * @Description:配置信息缓存系统
 */
public class ConfigCacheService {

    //中文错误信息
    private static final String NO_SPACE_CN = "设备上没有空间";

    //英文错误信息
    private static final String NO_SPACE_EN = "No space left on device";

    //中文错误信息
    private static final String DISK_QUOTA_CN = "超出磁盘限额";

    //英文错误信息
    private static final String DISK_QUOTA_EN = "Disk quota exceeded";

    //缓存配置信息的Map，key是配置信息的组合键，value就是对应的配置信息项，这里我提醒一下，这个配置信息项并不缓存配置信息本身的内容
    //而是缓存配置信息的MD5的值和密钥等扽信息
    private static final ConcurrentHashMap<String, CacheItem> CACHE = new ConcurrentHashMap<>();



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:确保与groupKey组合键的配置信息对应的缓存项存在的方法，如果不存在则创建对应的缓存项对象
     */
    static CacheItem makeSure(final String groupKey, final String encryptedDataKey) {
        //从map中获取对应的缓存项
        CacheItem item = CACHE.get(groupKey);
        //如果存在则可以直接返回该对象
        if (null != item) {
            return item;
        }
        //如果不存在则创建对应的缓存项对象
        CacheItem tmp = new CacheItem(groupKey, encryptedDataKey);
        //把缓存项存放到map中
        item = CACHE.putIfAbsent(groupKey, tmp);
        //返回配置信息对应的缓存项
        return (null == item) ? tmp : item;
    }
    

     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:尝试获得指定配置信息缓存项的读锁
     */
    public static int tryReadLock(String groupKey) {
        CacheItem groupItem = CACHE.get(groupKey);
        int result = (null == groupItem) ? 0 : (groupItem.getRwLock().tryReadLock() ? 1 : -1);
        if (result < 0) {
            DEFAULT_LOG.warn("[read-lock] failed, {}, {}", result, groupKey);
        }
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:释放指定配置信息缓存项的读锁
     */
    public static void releaseReadLock(String groupKey) {
        CacheItem item = CACHE.get(groupKey);
        if (null != item) {
            item.getRwLock().releaseReadLock();
        }
    }


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:尝试获得指定配置信息缓存项的写锁
     */
    static int tryWriteLock(String groupKey) {
        CacheItem groupItem = CACHE.get(groupKey);
        int result = (null == groupItem) ? 0 : (groupItem.getRwLock().tryWriteLock() ? 1 : -1);
        if (result < 0) {
            DEFAULT_LOG.warn("[write-lock] failed, {}, {}", result, groupKey);
        }
        return result;
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:释放指定配置信息缓存项的写锁
     */
    static void releaseWriteLock(String groupKey) {
        CacheItem groupItem = CACHE.get(groupKey);
        if (null != groupItem) {
            groupItem.getRwLock().releaseWriteLock();
        }
    }

    //其他功能暂时不做实现

    
}
```

好了，现在大家从上面代码块中就可以看到，**我已经为 ConfigCacheService 服务组件定义了获取指定配置信息的读锁或者写锁的方法，读锁和写锁可以用来解决处理配置信息时可能会发生的并发问题；除此之外，我还定义了一个 makeSure() 方法，该方法会为指定的配置信息创建 CacheItem 对象，然后把该对象存放在 ConfigCacheService 对象的 CACHE 成员变量中**。到此为止，我们就已经初步实现了把配置的关键信息缓存在内存中的功能。这个功能实现完毕之后，接下来就应该实现把配置信息持久化到本地硬盘上的功能了。

把配置信息持久化到本地硬盘的功能实现起来就更简单了，**在 nacos 配置中心源码中定义了两个可以将数据持久化到本地的硬盘信息对象，一个是 ConfigRawDiskService 对象，这个对象提供的数据持久化功能比较原生，就是把配置信息写入到硬盘对应的配置文件中；另一个是 ConfigRocksDbDiskService 硬盘对象，这个对象可以把配置信息持久化到 RocksDb 数据库中。除此之外，nacos 配置中心还定义了一个 ConfigDiskServiceFactory 工厂对象，这个工厂可以根据用户的配置得到具体向硬盘写入数据的对象**。接下来我就为大家把这几个类展示一下，首先是 ConfigDiskServiceFactory，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.dump.disk;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/26
 * @Description:本地文件系统工厂对象，该对象会根据用户配置得到对应的硬盘服务组件，也就是向硬盘写入数据和读取数据的组件
 */
public class ConfigDiskServiceFactory {

    //硬盘服务组件
    static ConfigDiskService configDiskService;

    //原生硬盘服务组件
    private static final String TYPE_RAW_DISK = "rawdisk";

    //基于RocksDB数据库的硬盘服务组件
    private static final String TYPE_ROCKSDB = "rocksdb";


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/26
     * @Description:得到ConfigDiskService对象的方法
     */
    public static ConfigDiskService getInstance() {
        if (configDiskService == null) {
            synchronized (ConfigDiskServiceFactory.class) {
                //二次判断
                if (configDiskService == null) {
                    //得到用户设置的配置信息
                    String type = System.getProperty("config_disk_type", TYPE_RAW_DISK);
                    //根据配置信息创建对应的ConfigDiskService对象
                    if (type.equalsIgnoreCase(TYPE_ROCKSDB)) {
                        configDiskService = new ConfigRocksDbDiskService();
                    } else {//在我为大家提供的第十七版本代码中，使用的就是ConfigRawDiskService原生的硬盘服务对象
                        configDiskService = new ConfigRawDiskService();
                    }
                }
                return configDiskService;
            }
        }
        return configDiskService;
    }

}
```

接下来就是 ConfigRawDiskService 类，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.dump.disk;


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/28
 * @Description:原始配置信息文件系统，这个类的内容非常简单，就是先根据配置信息的dataId，group等等信息找到目标文件，然后向对应的文件中写入数据即可
 * 查询数据也是一样的道理
 */
@SuppressWarnings("PMD.ServiceOrDaoClassShouldEndWithImplRule")
public class ConfigRawDiskService implements ConfigDiskService {
    
    private static final String BASE_DIR = File.separator + "data" + File.separator + "config-data";
    
    private static final String TENANT_BASE_DIR = File.separator + "data" + File.separator + "tenant-config-data";
    

    //保存配置信息本身内容到硬盘的目标文件中的方法
    public void saveToDisk(String dataId, String group, String tenant, String content) throws IOException {
        //根据配置信息的dataId、group、tenant找到存储配置信息的目标文件
        File targetFile = targetFile(dataId, group, tenant);
        //把配置信息写入到目标文件中
        FileUtils.writeStringToFile(targetFile, content, ENCODE_UTF8);
    }
    

    //找到配置信息目标文件的方法
    private static File targetFile(String dataId, String group, String tenant) {
        File file = null;
        if (StringUtils.isBlank(tenant)) {
            file = new File(EnvUtil.getNacosHome(), BASE_DIR);
        } else {
            file = new File(EnvUtil.getNacosHome(), TENANT_BASE_DIR);
            file = new File(file, tenant);
        }
        file = new File(file, group);
        file = new File(file, dataId);
        return file;
    }



    //其他读取配置信息内容，移除配置信息内容的方法就省略了
}
```

ConfigRocksDbDiskService 类的内容我就不在文章中展示了，大家感兴趣的话，可以去我提供的第十七版本代码中查看，或者直接去 nacos 源码中查看。到此为止，我就为大家把配置信息缓存到内存、持久化到本地硬盘的功能也实现了。现在，大家应该都清楚了为什么要实现这两个功能，以及如何实现这两个功能了吧？

当然，在看完上面的诸多代码块之后，很多朋友还是会感到疑惑，虽说我们目前确实实现了把配置信息缓存到内存、持久化到本地硬盘的功能，但是这个功能并不完善，甚至说是残缺的，**把配置信息的内容持久化到硬盘中的功能实现得还差不多，只要调用 ConfigRawDiskService 对象的 saveToDisk() 方法就可以把配置信息持久化到硬盘的目标文件中了**；但是把配置的关键信息缓存到内存中的功能就差点意思了，在我们目前实现的 ConfigCacheService 类中，确实可以通过 makeSure() 方法为客户端发布到服务端的每一个配置信息创建CacheItem 缓存项对象，但是这个对象创建了之后呢？**该怎么把配置的关键信息设置到 CacheItem 对象中呢**？这个功能我们可并没有实现，除此之外，大家肯定也很关心把配置信息持久化到硬盘的文件系统中的时机，这个我也没有为大家分析。总之，用一个问题就能概括，**目前我们还不知道怎么把配置信息导入到我们已经实现完毕缓存系统和本地文件系统中**。那这个问题该怎么解决呢？

其实可以从两个角度来分析，**那就是首先分析有哪些数据可以被导入到缓存系统和本地文件系统；第二就要分析这些数据导入缓存系统和本地文件系统的时机**。第一个问题很好分析，**肯定是客户端发布的所有配置信息需要被导入到缓存系统和本地文件系统，而这些配置信息都是存储在 mysql 数据库中的，所以说到底，就是需要把 mysql 数据库中存储的配置信息导入到配置中心服务端的缓存系统和本地文件系统中**；至于配置信息从数据库导入到配置中心服务端的缓存系统和文件系统的时机，这个也很容易分析，按照常规操作，**肯定是定义一个定时任务，这个定时任务每隔一段时间执行一次，执行的时候，就会把数据库中存储的所有配置信息导入到服务端的缓存系统和文件系统，当然，为了保证缓存系统和文件系统中的配置信息的时效性，当服务端检测某个配置信息发生变更之后，需要立刻把变更的配置信息从数据库中导入到文件系统和缓存系统中**。这操作的逻辑大家应该可以理解吧？

如果这些操作理解了，那么接下来我想先重构一下 ConfigCacheService 类，**在这个类中新定义一个 dump() 方法，该方法就是用来把 mysql 数据库中存储的配置信息导入到本地文件系统和缓存系统中的**。这个方法一旦实现了，那接下来的工作就更轻松了，**我们只要定义一个定期把数据库中数据导入到缓存系统和文件系统的定时任务，就算是把服务端存储配置信息的功能全部实现完毕了**。当然，服务端根据配置信息的变更情况，及时更新缓存系统和文件系统中配置信息的功能还没实现，但这是第十八版本代码对应的功能，所以我就不在本章为大家实现了。好了，接下来就让我先为 ConfigCacheService 类定义 dump() 方法吧，请看下面代码块。

```
package com.cqfy.nacos.config.server.service;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/28
 * @Description:配置信息缓存系统
 */
public class ConfigCacheService {

    //中文错误信息
    private static final String NO_SPACE_CN = "设备上没有空间";

    //英文错误信息
    private static final String NO_SPACE_EN = "No space left on device";

    //中文错误信息
    private static final String DISK_QUOTA_CN = "超出磁盘限额";

    //英文错误信息
    private static final String DISK_QUOTA_EN = "Disk quota exceeded";

    //缓存配置信息的Map，key是配置信息的组合键，value就是对应的配置信息项，这里我提醒一下，这个配置信息项并不缓存配置信息本身的内容
    //而是缓存配置信息的MD5的值和密钥等扽信息
    private static final ConcurrentHashMap<String, CacheItem> CACHE = new ConcurrentHashMap<>();


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
            //注意，这里有一个非常重要的操作，那就是发布了一个本地配置信息变更事件
            //也许大家还记得，在上一章实现ConfigPublishRequestHandler请求处理器时
            //在该处理器的handle方法结尾发布了一个ConfigDataChangeEvent配置信息变更事件
            //现在在这里又发布了一个LocalDataChangeEvent事件，那这两个事件的作用分别是什么呢？有什么不同呢？
            //大家可以暂时记住这个问题，等到了下一个版本代码我就会为大家解答这个问题
            NotifyCenter.publishEvent(new LocalDataChangeEvent(groupKey));
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:更新配置信息缓存项中的最新修改时间
     */
    public static void updateTimeStamp(String groupKey, long lastModifiedTs, String encryptedDataKey) {
        CacheItem cache = makeSure(groupKey, encryptedDataKey);
        cache.getConfigCache().setLastModifiedTs(lastModifiedTs);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:确保与groupKey组合键的配置信息对应的缓存项存在的方法，如果不存在则创建对应的缓存项对象
     */
    static CacheItem makeSure(final String groupKey, final String encryptedDataKey) {
        //从map中获取对应的缓存项
        CacheItem item = CACHE.get(groupKey);
        //如果存在则可以直接返回该对象
        if (null != item) {
            return item;
        }
        //如果不存在则创建对应的缓存项对象
        CacheItem tmp = new CacheItem(groupKey, encryptedDataKey);
        //把缓存项存放到map中
        item = CACHE.putIfAbsent(groupKey, tmp);
        //返回配置信息对应的缓存项
        return (null == item) ? tmp : item;
    }
    

     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:尝试获得指定配置信息缓存项的读锁
     */
    public static int tryReadLock(String groupKey) {
        CacheItem groupItem = CACHE.get(groupKey);
        int result = (null == groupItem) ? 0 : (groupItem.getRwLock().tryReadLock() ? 1 : -1);
        if (result < 0) {
            DEFAULT_LOG.warn("[read-lock] failed, {}, {}", result, groupKey);
        }
        return result;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:释放指定配置信息缓存项的读锁
     */
    public static void releaseReadLock(String groupKey) {
        CacheItem item = CACHE.get(groupKey);
        if (null != item) {
            item.getRwLock().releaseReadLock();
        }
    }


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:尝试获得指定配置信息缓存项的写锁
     */
    static int tryWriteLock(String groupKey) {
        CacheItem groupItem = CACHE.get(groupKey);
        int result = (null == groupItem) ? 0 : (groupItem.getRwLock().tryWriteLock() ? 1 : -1);
        if (result < 0) {
            DEFAULT_LOG.warn("[write-lock] failed, {}, {}", result, groupKey);
        }
        return result;
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:释放指定配置信息缓存项的写锁
     */
    static void releaseWriteLock(String groupKey) {
        CacheItem groupItem = CACHE.get(groupKey);
        if (null != groupItem) {
            groupItem.getRwLock().releaseWriteLock();
        }
    }

    //其他功能暂时不做实现

    
}
```

我在上面代码块中为 ConfigCacheService 类的 dump() 方法和该方法内部调用到的其他方法都添加了非常详细的注释，大家只要按照顺序阅读，肯定能明白其中的逻辑，因为这些方法都非常简单。我就不在文章中为大家解释其中的逻辑了，到此为止，我们就知道了 mysql 数据库中的配置信息究竟是怎么导入到服务端的缓存系统和本地文件系统中了。除此之外，倒入的时机我们也已经明确了：**就是定义一个定时任务，这个定时任务每隔一段时间执行一次，执行的时候，就会把数据库中存储的所有配置信息导入到服务端的缓存系统和文件系统，其实就是在每一次执行的时候，调用 ConfigCacheService 类的 dump() 方法即可**。那这个定时任务该在哪里定义呢？或者说在哪里创建呢？又在什么时候启动呢？这些问题就留到下一章为大家讲解吧。下一章我们不仅会实现这个定时任务、启动这个定时任务，还会把配置中心向服务端查询指定配置信息的功能也实现了。下一章结束之后，大家就可以去看我提供的第十七版本代码了。好了朋友们，我们下一章见！