从本章开始，我们就要开始正式构建 nacos 配置中心的服务端了。在开始正式构建之前，还是先让我来为大家分析一下，我们都需要实现什么功能吧。首先肯定是启动配置中心服务端，这样一来服务端才可以接收客户端连接，以及后续客户端发送过来的各种请求；当然，既然要接收配置中心客户端发送过来的各种请求，那么服务端肯定就要定义各种请求的处理器，然后实现对应的功能，比如发布配置信息功能，其实就是存储客户端发送过来的配置信息功能，还有查询配置信息功能，移除配置信息功能，以及最重要的监听配置变更，并且及时通知客户端配置发生变更的功能；最后还要考虑到配置中心集群的构建，当集群中某个节点的配置信息发生变更之后，还要把最新的配置信息同步给集群的其他节点。这样分析下来之后，我们可以发现，我们接下来要为配置中心服务端实现以下几个功能：

**1 启动配置中心服务端。**

**2 实现配置中心服务端存储、移除配置信息的功能。**

**3 实现配置中心服务端查询配置信息的功能。**

**4 实现配置中心服务端监听配置变更，并且及时通知客户端配置发生变更的功能。**

**5 实现在集群节点中同步最新配置信息的功能**。

以上五个功能实现之后，一个功能基本完善的配置中心服务端也就构建完毕了。大家也许会怀疑难道就实现这几个功能，一个配置中心服务端就构建完毕了吗？我可以很负责任地告诉大家，确实就是这样，就算大家直接去看 nacos 配置中心服务端的源码，也会发现整个服务端也就是围绕着这五个功能来展开构建的。当然，nacos 源码在实现每一个功能时会进行更深层次的细分，比如说在实现存储配置信息功能的时候，不仅仅会考虑存储常规配置信息的情况，还会考虑灰度发布以及定义了标签的配置信息的存储情况。不过这些配置信息的存储原理都是相同的，都是存储在 mysql 或者 derby 数据库中，无非就是存储的表不一样。只要大家把存储常规配置信息的功能掌握了，那么存储其他模式配置信息的功能也就掌握了。**所谓常规配置信息，就是只通过 dataId，group，tenant 就能唯一确定的配置信息**，**灰度发布的配置信息还额外定义了灰度发布的服务器的 IP 地址集合，而定义了额外标签的配置信息其实就是在客户端发布配置信息的时候为配置信息定义了更细粒度的 tag**。这三种不同的配置信息最终存放在数据库中时，要存放在不同的表中，我在 nacos 主线课程中为大家实现的只是存储常规配置信息的功能。不管怎么说吧，我还是那句话，nacos 配置中心实现起来非常简单，有些功能实现得甚至非常简陋，大家看完文章之后就会认同我的观点了。好了，话不多说，直接开始本章的内容吧。

## 构建 nacos 配置中心服务端

在我为大家展示的要实现的 nacos 配置中心服务端的五个功能中，第一个就是启动配置中心服务端，也许有朋友会感到好奇，**我为什么说启动服务端，而不是构建服务端，因为只有先把配置中心服务端的核心组件定义出来，才能启动呀，就比如说为配置中心服务端定义什么 BaseRpcServer 等等**。但是我根本没有这么做，什么也没有定义就打算直接启动配置中心服务端，接下来就让我给大家解释一下，既然刚才提到了 BaseRpcServer，那么大家肯定还记得在我们一起构建 nacos 配置中心服务端的时候，就为服务端定义了这个组件；除此之外还有一些其他的类，比如 GrpcSdkServer 和 GrpcClusterServer，一个代表普通的服务端，一个代表集群节点的服务端。总之，通过这些内容大家肯定能回忆起来，**我们在构建注册中心服务端时，已经实现了一整套完整的服务端，那在实现配置中心服务端的时候，能不能直接把之前实现的服务端交给配置中心使用呢？当然是没问题的，实际上在 nacos 源码中也正是这么做的**。

如果大家对这一点没有异议，现在大家应该能梳理清楚了，**实际上我们之前实现的 nacos 注册中心服务端并不是专门为注册中心设计的，它是一个独立的功能，被定义在了 nacos 的 core 模块下，这个模块可以被其他模块使用**。nacos 源码中有一个 naming 模块，从 pom 文件中就可以看出这个模块就依赖了 core 模块，而 naming 模块提供的正是注册中心服务端的业务功能，**core 模块提供的就是服务端网络通信功能和服务端集群功能**。既然 naming 模块可以使用 core 模块提供的服务端功能，那么我们新定义的配置中心当然也可以直接使用 core 模块提供的服务端功能。**比如说我们就可以把配置中心服务端的业务功能定义在一个新的模块，也就是 config 模块下，然后让 config 模块直接使用 core 模块的服务端功能即可**。这样一来，配置中心服务端的网络通信功能和集群功能就构建完毕了，然后就可以直接启动了。**至于配置中心客户端，使用的仍然是 RpcClient 客户端对象，它直接和服务端建立连接，就可以直接通信了**。

我相信大家理解以上内容时，应该也没有那么困难，这可以说是最简单的逻辑了，就是在相应模块的 pom 文件中定义 core 模块的信息即可。至于随后启动服务端的过程大家肯定也都很清楚了，**nacos 这个框架深度依赖了 springboot 框架，在 springboot 启动的过程中，nacos 配置中心服务端会随着 springboot 的启动而启动**。到此为止，配置中心的服务端就构建完毕，并且启动完毕了。当然，我并没有给大家展示任何一行相关的代码，因为在之前已经展示过太多了，大家如果想回顾回顾，可以直接去我提供的第十六版本代码中回顾 core 模块下的代码。接下来我们就要实现文章开头展示的五个功能中的第二个了，也就是配置中心服务端存储客户端传输过来的配置信息的功能。

## 引入 ConfigPublishRequestHandler 处理器

既然要让配置中心服务端存储客户端传输过来的配置信息了，那么首先就要做到两点：

**1 配置中心服务端可以接收到客户端传输过来的要发布的配置信息。**

**2 配置中心服务端把配置信息存储到起来，至于存储到哪里，还可以具体分析，也许是存储到本地硬盘上的某个文件中，也许是直接存储到数据库中**。

这两个功能都很好实现，比如说第一个功能吧，在我们实现配置中心客户端向服务端发布配置信息时，我们是把配置信息封装到一个 ConfigPublishRequest 请求对象中，然后把 ConfigPublishRequest 请求对象发送给服务端。既然是这样，**那我们就可以在配置中心服务端定义一个 ConfigPublishRequest 请求处理器，在这个处理器中处理客户端发送过来的 ConfigPublishRequest 请求**。而处理该请求的逻辑也很简单，就是把用户发送过来的配置信息存储到服务端内部。所以，我们要新引入的这个 ConfigPublishRequest 请求处理器可以先定义成下面这样，我把它定义为了 ConfigPublishRequestHandler 类，请看下面代码块。

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

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/24
     * @Description:处理客户端发送的ConfigPublishRequest请求的方法
     */
    @Override
    public ConfigPublishResponse handle(ConfigPublishRequest request, RequestMeta meta) throws NacosException {

        try {
            //从请求中得到客户端发布的配置信息的dataId
            String dataId = request.getDataId();
            //得到组名
            String group = request.getGroup();
            //得到客户端要发布的配置的内容
            String content = request.getContent();
            //得到租户信息
            final String tenant = request.getTenant();
            //得到发布配置信息的客户端的IP地址
            final String srcIp = meta.getClientIp();
            //得到应用的IP地址
            final String requestIpApp = request.getAdditionParam("requestIpApp");
            //得到配置信息的标签，在我们的代码中，客户端
            //并没有为发布的配置信息定义标签，所以这里得到的就是null
            //大家理解这个意思即可
            final String tag = request.getAdditionParam("tag");
            //得到应用配置信息的应用名称
            final String appName = request.getAdditionParam("appName");
            //得到配置信息的类型
            final String type = request.getAdditionParam("type");
            //得到发布配置信息的用户
            final String srcUser = request.getAdditionParam("src_user");
            //得到加密配置信息的密钥
            final String encryptedDataKey = request.getAdditionParam("encryptedDataKey");
            //对配置信息的各个参数进行校验
            ParamUtils.checkParam(dataId, group, "datumId", content);
            //对配置信息的标签进行校验，确保其不为空的时候格式正确
            ParamUtils.checkParam(tag);
            //得到客户端发送过来的请求中是否包含了测试IP地址，也就是灰度发布服务器的IP地址
            //在我们的代码中，客户端并没有为发布的配置信息定义灰度发布模式，所以这里得到的就是null
            //大家理解这个意思即可
            String betaIps = request.getAdditionParam("betaIps");

            //这些信息得到了之后，接下来就该把最重要的信息，也就是配置信息的dataId，group，tenant
            //content、encryptedDataKey存储起来了，但在这里我们还暂时不实现具体的存储功能

            //该方法的其他内容暂不实现
        
        }
}
```

好了，上面代码块展示完毕之后，我相信大家都理解了所谓的 ConfigPublishRequestHandler 请求处理器的作用：**该处理器就是要处理客户端发送过来的 ConfigPublishRequest 请求，解析该请求的时候，会从该请求中得到客户端发布过来的配置信息，然后把配置信息存储起来即可**。当然，存储配置信息的功能我并没实现，大家也可以从上面的代码块中看到，ConfigPublishRequestHandler 请求处理器的 handle() 方法并没有实现完整。原因很简单，我们还没有对配置中心服务端存储配置信息讨论出一个明确的结果。那接下来就让我们围绕着上面展示的第二个要点讨论讨论。

从最常规的逻辑来分析，服务端存储客户端发布过来的配置信息时，无非可以使用三种方式进行存储，**第一种就是直接缓存在内存中，就像注册中心存储临时服务实例信息那样；第二种就是直接存储在本地硬盘的指定文件中，比如可以根据配置信息的 dataId，group，tenant，为每一个配置信息创建对应的文件，然后把配置信息存储在对应的文件中；当然，第三种就是最最方便，也是最常见的方式，那就是直接把配置信息存储在数据库中**。那么在 nacos 源码中究竟使用了哪种方式存储客户端发送过来的配置信息呢？我可以给大家一个很干脆的答案，那就是这三种都使用了，也许大家会感到困惑，**考虑到配置信息是非常重要的信息，肯定需要进行持久化操作，显然存储在数据库中是最方便的操作，那为什么还要把配置信息同步存储在本地文件系统，并且换存在内存中呢**？这个问题等下一章我再为大家解决，并且我也会给大家实现对应功能，让大家知道配置中心服务端的本地文件系统和缓存系统究竟是怎么工作的。

好的，现在让我们继续回到 ConfigPublishRequestHandler 请求处理器中，刚才我们已经分析了，现在我们要把客户端发送过来的配置信息直接存储到数据库中，那接下来就很好操作了。**既然要和数据库打交道，那就直接定一个 service 层，或者说是一个 service 组件，这个组件中使用 mapper 和数据库交互，把数据存储在数据库中，这不就是很常见的业务逻辑吗**？当然，我们还要考虑好究竟要使用什么数据库来存储对应的配置信息，如果为了方便存储数据，我们可以直接使用一些嵌入在程序内部的数据库来存储配置信息，比如 derby 数据库，但这样的护数据库功能不是很全面；如果想使用功能更全面的数据库，那就可以使用 mysql 这样的数据库。**在我为大家提供的第十六版本代码中，使用的就是 mysql 数据库**。很好，现在要使用的数据库也确定了，接下来要执行的操作也就更明确了：**首先要创建可以访问 mysql 数据库的数据源对象，这个大家可以理解吧？没有这个数据源对象，nacos 程序根本就没办法远程访问 mysql 数据库服务器呀；然后就要为存储的配置信息建了，确定表的字段等等；还需要定义和数据库交互的 mapper 对象；最后定义我们刚才提到的 service 组件，这个组件中使用 mapper 和数据库交互，把数据存储在数据库中**。这些操作大家应该都能理解吧？如果大家都可以理解，接下来就让我们来一一实现吧。

首先是就是定义可以访问 mysql 数据库的数据源对象，这个非常容易实现，我就直接给大家展示具体的代码吧，其实 nacos 配置中心的各个功能都非常简单，基本上都是一套模式，而且和我们之前实现的 hippo4j 动态线程池框架服务端存储线程池配置信息的功能代码几乎一样，所以也没什么可讲的了。所以我就尽量加快一些文章的节奏，分析完功能的实现思路以及为什么要实现该功能之后，我就直接为大家展示对应的代码了。好了，接下来让我们看一下 nacos 配置中心服务端用来访问 mysql 数据库的数据源该如何实现。

实际上，**在 nacos 源码中定义了两个数据源对象，nacos 根据数据库是否内嵌在程序中，把数据源对象分为了内部数据源和外部数据源对象；如果使用的是 derby 内嵌数据库，那么就使用内部数据源对象，或者说使用本地数据源对象；如果使用的是 mysql 数据库存储配置信息，那 nacos 就会使用对应的外部数据源对象**。然后还提供了一个动态数据源对象，这个动态数据源对象就可以根据用户的配置，决定使用哪种数据源对象。这个动态数据源被定义为了 DynamicDataSource，具体实现如下，请看下面代码块。

```
package com.cqfy.nacos.persistence.datasource;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/25
 * @Description:动态数据源对象，这个类的对象就会根据nacos使用的是mysql外部数据源还是derby嵌入数据库，得到真正提供数据源服务的组件
 */
public class DynamicDataSource {

    //本地数据源服务组件，这个服务组件会帮助nacos程序访问derby嵌入式数据库
    private DataSourceService localDataSourceService = null;

    //外部数据源服务组件，这个服务组件会帮助nacos程序访问mysql数据库
    private DataSourceService basicDataSourceService = null;

    //单例模式
    private static final DynamicDataSource INSTANCE = new DynamicDataSource();

    //构造方法
    private DynamicDataSource() {}

    //把该类的对象暴露出去
    public static DynamicDataSource getInstance() {
        return INSTANCE;
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:根据用户配置，得到真正的数据源服务组件
     */
    public synchronized DataSourceService getDataSource() {
        try {

            //判断是否使用嵌入数据库
            if (DatasourceConfiguration.isEmbeddedStorage()) {
                //如果使用嵌入数据库
                if (localDataSourceService == null) {
                    //就在这里创建LocalDataSourceServiceImpl对象，然后初始化该对象。在第十六版本代码中，虽然我引入了LocalDataSourceServiceImpl类
                    //但是我们并没有真的用到这个类，而且这个类和下面的ExternalDataSourceServiceImpl类的内容大同小异，大家看看ExternalDataSourceServiceImpl类的内容就行了，我们主要使用ExternalDataSourceServiceImpl类
                    localDataSourceService = new LocalDataSourceServiceImpl();
                    localDataSourceService.init();
                }
                return localDataSourceService;
            } else {
                //如果使用的是外部数据库，那就创建ExternalDataSourceServiceImpl类的对象，这个对象会帮助nacos程序访问mysql数据库
                if (basicDataSourceService == null) {
                    basicDataSourceService = new ExternalDataSourceServiceImpl();
                    basicDataSourceService.init();
                }
                return basicDataSourceService;
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

}
```

上面代码块中的逻辑非常简单，我就不再重复讲解了。总之，**只要使用 DynamicDataSource 对象，就可以根据用户配置获得真正能够访问数据库的数据源对象**。我在第十六版本代码中为大家把内部数据源对象，也就是 LocalDataSourceServiceImpl 对象，还有外部数据源对象，也就是 ExternalDataSourceServiceImpl 对象都引入了，因为我们使用的是 ExternalDataSourceServiceImpl 外部数据源对象访问 mysql 数据库，所以我就只给这个 ExternalDataSourceServiceImpl 类添加了非常详细的注释，而 LocalDataSourceServiceImpl 类就没添加什么注释了。但是这两个类中的方法几乎都是一致的，因为它们都实现了一个共同的数据源接口，也就是 DataSourceService 接口。大家只要掌握了 ExternalDataSourceServiceImpl 外部数据源对象的内容，也就能自己看明白 LocalDataSourceServiceImpl 内部数据源对象的内容。好了，说了这么多，接下来我们就应该真正看一下 ExternalDataSourceServiceImpl 外部数据源对象内容，以及它可以做什么，请看下面代码块。

```
package com.cqfy.nacos.persistence.datasource;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/25
 * @Description:提供外部数据源服务的组件
 */
public class ExternalDataSourceServiceImpl implements DataSourceService {

    private static final Logger LOGGER = LoggerFactory.getLogger(ExternalDataSourceServiceImpl.class);

    //jdbc查询超时时间
    private int queryTimeout = 3;

    //事务查询超时时间
    private static final int TRANSACTION_QUERY_TIMEOUT = 5;

    //数据库主节点选择阈值，数据库只能有一个主节点
    private static final int DB_MASTER_SELECT_THRESHOLD = 1;

    //存放数据源的集合
    private List<HikariDataSource> dataSourceList = new ArrayList<>();

    //访问数据库主节点的jdbc模版对象，HikariDataSource数据源对象就可以交给JdbcTemplate使用
    //HikariDataSource数据源对象连接的是哪个数据库，那么JdbcTemplate就可以直接访问哪个数据库了
    private JdbcTemplate jt;

    //数据源事务管理器
    private DataSourceTransactionManager tm;

    //事务模版
    private TransactionTemplate tjt;


     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:初始化ExternalDataSourceServiceImpl对象的方法
     */
    @Override
    public void init() {

        //该方法我就不在文章中展示了，其实就是根据用户的配置信息
        //创建JdbcTemplate对象，以及其他成员变量对象，大家去我提供的第十六版本代码中一看就知道了
        //都是非常简单常规的逻辑，而且代码注释非常详细
    }
}
```

从上面代码块中可以看到，**通过 ExternalDataSourceServiceImpl 数据源对象就可以得到对目标数据库执行操作的 JdbcTemplate 对象，这样一来，nacos 程序就可以访问数据库，并对数据库的某些表执行 sql 语句了**。很好，既然说到了数据库中的表，接下来就该为要存储的配置信息键表了，这里我就直接把 nacos 中为 mysql 数据库定义好的各个表搬运过来了，请看下面代码块。

```
config/src/main/resources/META-INF/nacos-db.sql



/*   数据库全名 = nacos_config   */
/*   表名称 = config_info   */
//这个config_info表存放的就是用来常规配置信息，所谓常规配置信息，就是只通过dataId，group，tenant
//三个字段就能唯一确定的配置信息
CREATE TABLE `config_info` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `data_id` varchar(255) NOT NULL COMMENT 'data_id',
  `group_id` varchar(128) DEFAULT NULL,
  `content` longtext NOT NULL COMMENT 'content',
  `md5` varchar(32) DEFAULT NULL COMMENT 'md5',
  `gmt_create` datetime NOT NULL DEFAULT '2010-05-05 00:00:00' COMMENT '创建时间',
  `gmt_modified` datetime NOT NULL DEFAULT '2010-05-05 00:00:00' COMMENT '修改时间',

   //以下几个字段要解释说明一下，当然，在我为大家提供的代码中，客户端根本就没有为配置信息的某些字段赋值
   //实际上在nacos源码中也没有设置，大家在使用nacos的过程中，可以自己设置一下
  `src_user` text COMMENT 'source user',
  `src_ip` varchar(20) DEFAULT NULL COMMENT 'source ip',
  `app_name` varchar(128) DEFAULT NULL,
  `tenant_id` varchar(128) DEFAULT '' COMMENT '租户字段',
   //配置信息的描述信息
  `c_desc` varchar(256) DEFAULT NULL,
  //配置信息的用途
  `c_use` varchar(64) DEFAULT NULL,
  //配置信息的生效条件
  `effect` varchar(64) DEFAULT NULL,
  //配置信息的类型，配置信息的类型客户端发布信息的时候定义了
  `type` varchar(64) DEFAULT NULL,
  //配置信息的数据格式
  `c_schema` text,
  `encrypted_data_key` text NOT NULL COMMENT '密钥',
  PRIMARY KEY (`id`),
  //这里定义了一个唯一约束，如果该表中新添加的配置信息的data_id，group_id，tenant_id已经存在了
  //就会抛出异常，这个约束也就在数据库中实现了通过dataId，group，tenant
  //三个字段确定唯一配置信息的功能
  UNIQUE KEY `uk_configinfo_datagrouptenant` (`data_id`,`group_id`,`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='config_info';





//接下来我要多解释几句，在nacos源码为mysql准备的sql脚本文件中，大家还会看到以下两个表信息
//大家看源码的时候可以简单看看


/******************************************/
/*   数据库全名 = nacos_config   */
/*   表名称 = config_info_beta   */
//这个表是专门存放灰度发布的配置信息的
/******************************************/
CREATE TABLE `config_info_beta` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `data_id` varchar(255) NOT NULL COMMENT 'data_id',
  `group_id` varchar(128) NOT NULL COMMENT 'group_id',
  `app_name` varchar(128) DEFAULT NULL COMMENT 'app_name',
  `content` longtext NOT NULL COMMENT 'content',
  `beta_ips` varchar(1024) DEFAULT NULL COMMENT 'betaIps',
  `md5` varchar(32) DEFAULT NULL COMMENT 'md5',
  `gmt_create` datetime NOT NULL DEFAULT '2010-05-05 00:00:00' COMMENT '创建时间',
  `gmt_modified` datetime NOT NULL DEFAULT '2010-05-05 00:00:00' COMMENT '修改时间',
  `src_user` text COMMENT 'source user',
  `src_ip` varchar(20) DEFAULT NULL COMMENT 'source ip',
  `tenant_id` varchar(128) DEFAULT '' COMMENT '租户字段',
  `encrypted_data_key` text NOT NULL COMMENT '密钥',
  PRIMARY KEY (`id`),
  UNIQUE KEY `uk_configinfobeta_datagrouptenant` (`data_id`,`group_id`,`tenant_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='config_info_beta';




/******************************************/
/*   数据库全名 = nacos_config   */
/*   表名称 = config_info_tag   */
//这个表是准们存放定义了标签信息的配置信息的
/******************************************/
CREATE TABLE `config_info_tag` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'id',
  `data_id` varchar(255) NOT NULL COMMENT 'data_id',
  `group_id` varchar(128) NOT NULL COMMENT 'group_id',
  `tenant_id` varchar(128) DEFAULT '' COMMENT 'tenant_id',
  `tag_id` varchar(128) NOT NULL COMMENT 'tag_id',
  `app_name` varchar(128) DEFAULT NULL COMMENT 'app_name',
  `content` longtext NOT NULL COMMENT 'content',
  `md5` varchar(32) DEFAULT NULL COMMENT 'md5',
  `gmt_create` datetime NOT NULL DEFAULT '2010-05-05 00:00:00' COMMENT '创建时间',
  `gmt_modified` datetime NOT NULL DEFAULT '2010-05-05 00:00:00' COMMENT '修改时间',
  `src_user` text COMMENT 'source user',
  `src_ip` varchar(20) DEFAULT NULL COMMENT 'source ip',
  PRIMARY KEY (`id`),
  //这里可以看到，唯一约束使用的是data_id、group_id、tenant_id、tag_id这四个字段
  //也就是说，假如一个配置文件的dataId，组名和租户信息一样，但是标签信息不一致，那么这个配置信息也可以存放在数据库中
  UNIQUE KEY `uk_configinfotag_datagrouptenanttag` (`data_id`,`group_id`,`tenant_id`,`tag_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='config_info_tag';
```

好了，现在 mysql 数据库存放配置信息的表也定义完毕了，那接下来就该定义 mapper 对象了，**所谓 mapper 对象，说白了就是为要操作的配置信息，提供执行增删改查等等 sql 语句命令的对象。但现在我们已经有了 JdbcTemplate 对象来直接对数据库执行 sql 语句了，那么 mapper 就可以只为成功增删改查对应的 sql 语句即可，sql 语句定义完毕了，就可以直接交给 JdbcTemplate 对象去执行了**。那么对 config_info 表的增删改查语句该怎么定义呢？这个我就不在文章中为大家展示了，因为定义的 sql 语句比较多，但是逻辑都很简单，所以我就只为大家简单分析一下该功能的实现思路，然后大家直接看我提供的第十六版本代码即可。

实际上，**在 nacos 配置中心源码中为 config_info 表定义了一个专门的类，该类的对象就用来为 config_info 表提供了各种增删改查 sql 语句，这个类就是 ConfigInfoMapperByMySql**。**当配置中心服务端要对数据库中的配置信息执行某种操作时，就会根据操作的类型从 ConfigInfoMapperByMySql 对象中得到对应的 sql 语句，然后把 sql 语句交给 JdbcTemplate 对象执行即可**。这个逻辑应该很好理解，但是有一点我要为大家补充一下，那就是刚才在上面的代码块中我们也看到了，数据库中会存在多张和配置信息相关的表，这就意味着程序中会有多个 mapper 对象和各自的表对应。这样一来，我们就不得不再定义一个新的对象，也就是 mapper 管理器，程序中所有的 mapper 对象都会被存放到 mapper 管理器中，这个 mapper 管理器倒是可以给大家展示一下，这个类的内容非常简单，它就是 MapperManager 类，请看下面代码块。

```
package com.cqfy.nacos.persistence.datasource;

/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/25
 * @Description:mapper管理器
 */
public class MapperManager {


    private static final Logger LOGGER = LoggerFactory.getLogger(MapperManager.class);

    //存储所有mapper的map，key为mapper对应的数据源类型，value就是表名和mapper的映射
    public static final Map<String, Map<String, Mapper>> MAPPER_SPI_MAP = new HashMap<>();

    private static final MapperManager INSTANCE = new MapperManager();

    //是否启动日志记录功能，如果启用就要对mapper做动态代理增强，执行mapper方法的时候就可以记录日志信息了
    private boolean dataSourceLogEnable;

    //构造方法
    private MapperManager() {
        //在创建mapper管理器的时候把程序要用到的mapper加载到MAPPER_SPI_MAP成员变量中
        loadInitial();
    }

    //把该类的对象暴露出去的方法
    public static MapperManager instance(boolean isDataSourceLogEnable) {

        INSTANCE.dataSourceLogEnable = isDataSourceLogEnable;
        return INSTANCE;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:把程序要用到的mapper加载到MAPPER_SPI_MAP成员变量中的方法
     */
    public synchronized void loadInitial() {
        //使用SPI加载所有mapper
        Collection<Mapper> mappers = NacosServiceLoader.load(Mapper.class);

        for (Mapper mapper : mappers) {
            putMapper(mapper);
            LOGGER.info("[MapperManager] Load Mapper({}) datasource({}) tableName({}) successfully.",
                    mapper.getClass(), mapper.getDataSource(), mapper.getTableName());
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:根据数据源和表名查找指定mapper的方法
     */
    public <R extends Mapper> R findMapper(String dataSource, String tableName) {

        LOGGER.info("[MapperManager] findMapper dataSource: {}, tableName: {}", dataSource, tableName);
        if (StringUtils.isBlank(dataSource) || StringUtils.isBlank(tableName)) {
            throw new NacosRuntimeException(FIND_DATASOURCE_ERROR_CODE, "dataSource or tableName is null");
        }

        Map<String, Mapper> tableMapper = MAPPER_SPI_MAP.get(dataSource);
        if (Objects.isNull(tableMapper)) {
            throw new NacosRuntimeException(FIND_DATASOURCE_ERROR_CODE,
                    "[MapperManager] Failed to find the datasource,dataSource:" + dataSource);
        }

        Mapper mapper = tableMapper.get(tableName);
        if (Objects.isNull(mapper)) {
            throw new NacosRuntimeException(FIND_TABLE_ERROR_CODE,
                    "[MapperManager] Failed to find the table ,tableName:" + tableName);
        }
        //如果启用了记录日志信息的功能，那就对mapper做动态代理
        if (dataSourceLogEnable) {
            return MapperProxy.createSingleProxy(mapper);
        }
        return (R) mapper;
    }
}
```

从上面代码块中可以看到，**程序中定义的所有 mapper 都会通过 SPI 的方法被加载到 MapperManager 管理器中，当配置中心服务端需要对配置信息执行某些操作时，就会调用 MapperManager 的 findMapper() 方法，根据配置信息对应的数据源和表名找到该配置信息的 mapper 文件，然后获得对应的 sql 语句执行即可**。好了，现在和 sql 有关的内容也实现完毕了，最后应该把以上所有实现的组件组合到一起，也就是引入和数据库打交道的 service 持久层服务组件，实现把配置信息存储在 mysql 数据库中的功能。

## 引入 ExternalConfigInfoPersistServiceImpl

这个 service 持久层的组件我也已经定义完毕了，**就定义为了 ExternalConfigInfoPersistServiceImpl 类，翻译过来就是外部数据源的配置信息持久化服务组件，这个类的对象就是专门用来和数据库中的 config_info 表打交道的**。既然有外部数据源的配置信息持久化服务组件，肯定就有内部数据源的配置信息持久化服务组件，所以在 nacos 源码中还定义了一个 EmbeddedConfigInfoPersistServiceImpl 类，这个类的对象就是专门用来把配置信息存储到 derby 数据库的 config_info 表中的。当然，我并没有在为大家提供的第十六版本代码中引入该类，毕竟我们并没有使用内嵌在程序中的 derby 数据库。不过大家只要把 ExternalConfigInfoPersistServiceImpl 类的内容掌握了，就可以自行掌握 EmbeddedConfigInfoPersistServiceImpl 类的内容，因为这两个类实现了共同的 ConfigInfoPersistService 接口，类中的方法几乎一致，大家感情去的话，可以直接去源码中看看 EmbeddedConfigInfoPersistServiceImpl 类。好了，接下来就让我为大家展示一下新定义的 ExternalConfigInfoPersistServiceImpl 类吧，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.repository.extrnal;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/25
 * @Description:和数据库打交道的组件，这个类的对象会将配置信息存储到数据库的`config_info`表中，这个类的对象就是用来和mysql数据库打交道的
 * 如果我们使用的是嵌入数据库，那么就会使用EmbeddedConfigInfoPersistServiceImpl类的对象把配置信息持久化道derby数据库中
 * 因为还没有实现和web控制台交互的功能，所以我省略了该类中的部分方法，比如给数据分页的方法等等
 */
@SuppressWarnings(value = {"PMD.MethodReturnWrapperTypeRule", "checkstyle:linelength"})
@Conditional(value = ConditionOnExternalStorage.class)
@Service("externalConfigInfoPersistServiceImpl")
public class ExternalConfigInfoPersistServiceImpl implements ConfigInfoPersistService {


    //数据源服务组件，通过这个对象就可以得到数据源
    private DataSourceService dataSourceService;

    //jdbc模版对象，通过数据源对象就可以得到jdbc模版
    protected JdbcTemplate jt;

    //事务模版对象
    protected TransactionTemplate tjt;

    //mapper管理器，在源码中，这个管理器管理着各种表的mapper，在我提供的代码中，我只引入了ConfigInfoMapperByMySql和HistoryConfigInfoMapperByMySql这两个mapper
    //剩下的在实现支线功能时引入
    private MapperManager mapperManager;

    //持久化配置历史信息的组件，这个组件就会把配置历史信息持久化到his_config_info表中
    private HistoryConfigInfoPersistService historyConfigInfoPersistService;

    //是否启用数据源日志的标志
    public static final String NACOS_PLUGIN_DATASOURCE_LOG = "nacos.plugin.datasource.log.enabled";

    //构造方法
    public ExternalConfigInfoPersistServiceImpl(@Qualifier("externalHistoryConfigInfoPersistServiceImpl") HistoryConfigInfoPersistService historyConfigInfoPersistService) {

        //通过动态数据源对象得到程序真正实用的数据源对象，如果程序使用的是mysql外部数据源，那么就会使用ExternalDataSourceServiceImpl数据源对象提供服务
        //如果使用的是derby数据库，那就会使用LocalDataSourceServiceImpl本地数据源对象提供服务，因为我们使用的是mysql数据库，所以这里得到的就是ExternalDataSourceServiceImpl数据源对象
        this.dataSourceService = DynamicDataSource.getInstance().getDataSource();

        //得到jdbc模版
        this.jt = dataSourceService.getJdbcTemplate();

        //得到事务模版
        this.tjt = dataSourceService.getTransactionTemplate();

        //得到是否启用数据源日志标志，启用这个标志之后，在mapper工作的时候，每次执行mapper中的方法，都会记录对应的日志
        //使用的就是动态代理，具体逻辑大家可以去MapperProxy类和MapperManager类的findMapper()方法中查看
        Boolean isDataSourceLogEnable = EnvUtil.getProperty(NACOS_PLUGIN_DATASOURCE_LOG, Boolean.class, false);

        //得到mapper管理器
        this.mapperManager = MapperManager.instance(isDataSourceLogEnable);

        //得到持久化历史配置信息的服务组件
        this.historyConfigInfoPersistService = historyConfigInfoPersistService;
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:使用cas方式添加或者更新数据库中配置信息的方法
     */
    @Override
    public ConfigOperateResult insertOrUpdateCas(String srcIp, String srcUser, ConfigInfo configInfo,
                                                 Map<String, Object> configAdvanceInfo) {
        try {
            //先尝试添加配置信息，如果配置信息添加失败，比如说主键冲突了，就意味着以ing存在对应的配置信息了，所以应该执行更新对应的配置信息的操作
            return addConfigInfo(srcIp, srcUser, configInfo, configAdvanceInfo);
        } catch (DuplicateKeyException ignore) {
            //`config_info` 这张表定义了UNIQUE KEY `uk_configinfo_datagrouptenant` (`data_id`,`group_id`,`tenant_id`)约束
            //所以只要插入的配置信息的data_id、group_id、tenant_id三个字段相同，那就会抛出异常，说明已经存在该配置信息了，应该执行更新操作
            //这里就是捕捉到了主键冲突异常，然后执行更新配置信息的操作，不过在config_info表中，主键其实是自增的
            //处理之外我还要再多解释一句，配置信息也可以定义额外的tag信息，在那种情况下，一个配置信息就需要用data_id、group_id、tenant_id、tag四个字段来确定唯一性
            //这一点在专门存储定义了标签的配置信息的`config_info_tag`表中体现的非常明显，也是定义了UNIQUE KEY `uk_configinfotag_datagrouptenanttag` (`data_id`,`group_id`,`tenant_id`,`tag_id`)约束
            //大家可以在源码中看看相关的代码，如果不想看，可以再等等，我更新nacos支线内容的时候会把这些全部实现了
            return updateConfigInfoCas(configInfo, srcIp, srcUser, configAdvanceInfo);
        } catch (DataIntegrityViolationException dive) {
            try {//这里也是违反了数据库的各种约束，也可能是因为数据库中已经存在相同的数据了
                //所以这里也是改为执行更新数据的操作
                return updateConfigInfoCas(configInfo, srcIp, srcUser, configAdvanceInfo);
            } catch (Exception e) {
                //仍然有异常则记录错误日志
                LogUtil.FATAL_LOG.error("[db-error] try to update config cas failed, with DataIntegrityViolationException, {}", dive.getMessage(), e);
                //抛出异常给外层方法
                throw e;
            }
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:以CAS的方式更新配置信息的方法
     */
    @Override
    public ConfigOperateResult updateConfigInfoCas(final ConfigInfo configInfo, final String srcIp,
                                                   final String srcUser, final Map<String, Object> configAdvanceInfo) {
        //使用事务模版保证操作的原子性
        return tjt.execute(status -> {
            try {
                //首先先从数据库中查找对应的配置信息，也就是得到旧的配置信息
                ConfigInfo oldConfigInfo = findConfigInfo(configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
                //如果旧的配置信息并不存在，那就直接记录错误日志，返回失败结果
                if (oldConfigInfo == null) {
                    if (LogUtil.FATAL_LOG.isErrorEnabled()) {
                        LogUtil.FATAL_LOG.error("expected config info[dataid:{}, group:{}, tenent:{}] but not found.",
                                configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
                    }
                    return new ConfigOperateResult(false);
                }
                //执行到这里意味着数据库中确实存储着旧的配置信息，在这里得到配置信息被使用的应用的名称
                String appNameTmp = oldConfigInfo.getAppName();
                //如果客户端新发布的配置信息并没有设置应用名称，就更新一下配置信息的应用名称
                if (configInfo.getAppName() == null) {
                    configInfo.setAppName(appNameTmp);
                }
                //CAS更新数据库中的配置信息，返回被操作影响的行数
                int rows = updateConfigInfoAtomicCas(configInfo, srcIp, srcUser, configAdvanceInfo);
                //如果一行也没有更新，那肯定就执行失败了，返回失败结果
                if (rows < 1) {
                    return new ConfigOperateResult(false);
                }
                //得到当前系统的时间戳
                Timestamp now = new Timestamp(System.currentTimeMillis());
                //把旧的配置信息存储到历史信息表中，方便以后在需要的时候，可以回滚配置信息
                historyConfigInfoPersistService.insertConfigHistoryAtomic(oldConfigInfo.getId(), oldConfigInfo, srcIp, srcUser, now, "U");
                //再次从数据库中查询以下最新的配置信息
                ConfigInfoStateWrapper configInfoLast = this.findConfigInfoState(configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
                //看看能不能查询到结果
                if (configInfoLast == null) {
                    return new ConfigOperateResult(false);
                }
                return new ConfigOperateResult(configInfoLast.getId(), configInfoLast.getLastModified());
            } catch (CannotGetJdbcConnectionException e) {
                LogUtil.FATAL_LOG.error("[db-error] " + e, e);
                throw e;
            }
        });
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:使用CAS更新数据库中配置信息的方法，这个方法的CAS主要体现在对比
     */
    private int updateConfigInfoAtomicCas(final ConfigInfo configInfo, final String srcIp, final String srcUser, Map<String, Object> configAdvanceInfo) {

        //判断应用名称是否为空，为空则使用空串
        String appNameTmp = StringUtils.defaultEmptyIfBlank(configInfo.getAppName());
        //同上
        String tenantTmp = StringUtils.defaultEmptyIfBlank(configInfo.getTenant());
        //在这里计算客户端发布的新配置信息的MD5值
        final String md5Tmp = MD5Utils.md5Hex(configInfo.getContent(), Constants.ENCODE);
        //从客户端发布的配置信息中得到配置的额外信息
        String desc = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("desc");
        String use = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("use");
        String effect = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("effect");
        String type = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("type");
        String schema = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("schema");

        try {
            //在这里从mapper管理器中得到专门和数据库中的"config_info"表打交道的mapper
            //这里得到的启示就是ConfigInfoMapperByMySql，这个类实现了ConfigInfoMapper接口
            ConfigInfoMapper configInfoMapper = mapperManager.findMapper(dataSourceService.getDataSourceType(), TableConstant.CONFIG_INFO);
            //得到系统当前时间
            Timestamp now = new Timestamp(System.currentTimeMillis());
            //创建一个mapper上下文对象，这个对象会封装要更新到数据库中的配置信息的各个字段的值
            //以下各个字段的用途我在前面已经跟大家讲解过了，所以就不添加详细注释了
            MapperContext context = new MapperContext();
            context.putUpdateParameter(FieldConstant.CONTENT, configInfo.getContent());
            context.putUpdateParameter(FieldConstant.MD5, md5Tmp);
            context.putUpdateParameter(FieldConstant.SRC_IP, srcIp);
            context.putUpdateParameter(FieldConstant.SRC_USER, srcUser);
            context.putUpdateParameter(FieldConstant.GMT_MODIFIED, now);
            context.putUpdateParameter(FieldConstant.APP_NAME, appNameTmp);
            context.putUpdateParameter(FieldConstant.C_DESC, desc);
            context.putUpdateParameter(FieldConstant.C_USE, use);
            context.putUpdateParameter(FieldConstant.EFFECT, effect);
            context.putUpdateParameter(FieldConstant.TYPE, type);
            context.putUpdateParameter(FieldConstant.C_SCHEMA, schema);
            context.putWhereParameter(FieldConstant.DATA_ID, configInfo.getDataId());
            context.putWhereParameter(FieldConstant.GROUP_ID, configInfo.getGroup());
            context.putWhereParameter(FieldConstant.TENANT_ID, tenantTmp);
            //把客户端发布过来的配置信息的MD5设置到context上下文中，注意，这里得到的就是客户端发布过来的旧的配置信息的MD5的值
            //这个值会用来和数据库中存储到旧的配置信息的MD5的值做对比，如果一致，才会继续更新配置信息，所谓CAS，就体现在这里
            context.putWhereParameter(FieldConstant.MD5, configInfo.getMd5());
            //使用mapper得到对应的sql语句
            MapperResult mapperResult = configInfoMapper.updateConfigInfoAtomicCas(context);
            //在这里执行更新数据库配置信息的操作，返回更新操作影响的数据行数
            return jt.update(mapperResult.getSql(), mapperResult.getParamList().toArray());
        } catch (CannotGetJdbcConnectionException e) {
            LogUtil.FATAL_LOG.error("[db-error] " + e, e);
            throw e;
        }
    }



    
    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:更新或者新增配置信息到数据库中的方法
     */
    public ConfigOperateResult insertOrUpdate(String srcIp, String srcUser, ConfigInfo configInfo,
                                              Map<String, Object> configAdvanceInfo) {
        try {
            //先尝试添加配置信息，如果配置信息添加失败，比如说主键冲突了，就意味着以ing存在对应的配置信息了，所以应该执行更新对应的配置信息的操作
            return addConfigInfo(srcIp, srcUser, configInfo, configAdvanceInfo);
        } catch (DuplicateKeyException ignore) {
            //这里就是捕捉到了主键冲突异常，然后执行更新配置信息的操作
            return updateConfigInfo(configInfo, srcIp, srcUser, configAdvanceInfo);
        } catch (DataIntegrityViolationException dive) {
            try {//这里捕捉到的是数据完整性异常，其实也是违反了数据库的各种约束，也可能是因为数据库中已经存在相同的数据了
                //所以这里也是改为执行更新数据的操作
                return updateConfigInfo(configInfo, srcIp, srcUser, configAdvanceInfo);
            } catch (Exception e) {
                LogUtil.FATAL_LOG.error("[db-error] try to update config failed, with DataIntegrityViolationException, {}", dive.getMessage(), e);
                throw e;
            }
        }
    }

    


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:把配置信息存储到数据库中的方法
     */
    @Override
    public ConfigOperateResult addConfigInfo(final String srcIp, final String srcUser, final ConfigInfo configInfo,
                                             final Map<String, Object> configAdvanceInfo) {
        //使用事务模版保证操作的原子性
        return tjt.execute(status -> {
            try {
                //把配置信息原子存储到数据库中，并且返回主键Id
                addConfigInfoAtomic(-1, srcIp, srcUser, configInfo, configAdvanceInfo);
                //得到系统当前时间
                Timestamp now = new Timestamp(System.currentTimeMillis());
                //把配置信息同时存储到历史信息表中，这个操作的主要作用就是记录配置信息的变更情况的，方便用户追踪配置信息的变更情况
                //"I"代表的是Insert的意思
                historyConfigInfoPersistService.insertConfigHistoryAtomic(0, configInfo, srcIp, srcUser, now, "I");
                //从数据库中查询配置信息，看是否能够查到，如果查到了就意味着配置信息添加没有问题
                ConfigInfoStateWrapper configInfoCurrent = this.findConfigInfoState(configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
                //如果查询结果为空，则返回错误结果
                if (configInfoCurrent == null) {
                    return new ConfigOperateResult(false);
                }
                //执行到这里意味着配置添加成功，返回成功结果即可
                //configInfoCurrent.getId()是配置的主键Id
                //configInfoCurrent.getLastModified()是配置的最新修改时间
                return new ConfigOperateResult(configInfoCurrent.getId(), configInfoCurrent.getLastModified());
            } catch (CannotGetJdbcConnectionException e) {
                LogUtil.FATAL_LOG.error("[db-error] " + e, e);
                throw e;
            }
        });
    }



    /**
    * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
    * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
    * @Date:2024/10/25
    * @Description:原子方式添加配置信息到数据库中的方法
    */
    @Override
    public long addConfigInfoAtomic(final long configId, final String srcIp, final String srcUser,
                                    final ConfigInfo configInfo, Map<String, Object> configAdvanceInfo) {
        //重复逻辑就不再添加注释了，上面的方法中已经解释过泪
        final String appNameTmp = StringUtils.defaultEmptyIfBlank(configInfo.getAppName());
        final String tenantTmp = StringUtils.defaultEmptyIfBlank(configInfo.getTenant());
        final String desc = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("desc");
        final String use = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("use");
        final String effect = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("effect");
        final String type = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("type");
        final String schema = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("schema");
        //这里得到了配置信息的密钥
        final String encryptedDataKey = configInfo.getEncryptedDataKey() == null ? StringUtils.EMPTY : configInfo.getEncryptedDataKey();
        //计算得到了配置信息的MD5的值
        final String md5Tmp = MD5Utils.md5Hex(configInfo.getContent(), Constants.ENCODE);
        //定义一个存储新添加到数据库中的配置信息主键的对象
        KeyHolder keyHolder = new GeneratedKeyHolder();
        //得到与"config_info"表对应的mapper
        ConfigInfoMapper configInfoMapper = mapperManager.findMapper(dataSourceService.getDataSourceType(), TableConstant.CONFIG_INFO);
        //在这里构建新增配置信息的sql语句
        final String sql = configInfoMapper.insert(Arrays
                .asList("data_id", "group_id", "tenant_id", "app_name", "content", "md5", "src_ip", "src_user",
                        "gmt_create", "gmt_modified", "c_desc", "c_use", "effect", "type", "c_schema",
                        "encrypted_data_key"));
        //得到配置信息在数据库中的主键字段的名称，其实就是"id"
        String[] returnGeneratedKeys = configInfoMapper.getPrimaryKeyGeneratedKeys();
        //执行数据库更新操作
        try {
            //在这里把配置信息添加到数据库中
            jt.update(new PreparedStatementCreator() {

                @Override
                public PreparedStatement createPreparedStatement(Connection connection) throws SQLException {

                    Timestamp now = new Timestamp(System.currentTimeMillis());
                    PreparedStatement ps = connection.prepareStatement(sql, returnGeneratedKeys);
                    ps.setString(1, configInfo.getDataId());
                    ps.setString(2, configInfo.getGroup());
                    ps.setString(3, tenantTmp);
                    ps.setString(4, appNameTmp);
                    ps.setString(5, configInfo.getContent());
                    ps.setString(6, md5Tmp);
                    ps.setString(7, srcIp);
                    ps.setString(8, srcUser);
                    ps.setTimestamp(9, now);
                    ps.setTimestamp(10, now);
                    ps.setString(11, desc);
                    ps.setString(12, use);
                    ps.setString(13, effect);
                    ps.setString(14, type);
                    ps.setString(15, schema);
                    ps.setString(16, encryptedDataKey);
                    return ps;
                }
            }, keyHolder);
            //得到配置信息的主键信息
            Number nu = keyHolder.getKey();
            //信息为空则抛出异常
            if (nu == null) {
                throw new IllegalArgumentException("insert config_info fail");
            }
            //返回主键
            return nu.longValue();
        } catch (CannotGetJdbcConnectionException e) {
            LogUtil.FATAL_LOG.error("[db-error] " + e, e);
            throw e;
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:非CAS更新数据库中配置信息的方法
     */
    @Override
    public ConfigOperateResult updateConfigInfo(final ConfigInfo configInfo, final String srcIp, final String srcUser,
                                                final Map<String, Object> configAdvanceInfo) {

        return tjt.execute(status -> {
            try {
                //先从数据库中查找对应的配置信息，看看配置信息是否存在，只有配置信息存在才能更新
                ConfigInfo oldConfigInfo = findConfigInfo(configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
                if (oldConfigInfo == null) {
                    if (LogUtil.FATAL_LOG.isErrorEnabled()) {
                        LogUtil.FATAL_LOG.error("expected config info[dataid:{}, group:{}, tenent:{}] but not found.",
                                configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
                    }
                    return new ConfigOperateResult(false);
                }
                String appNameTmp = oldConfigInfo.getAppName();
                if (configInfo.getAppName() == null) {
                    configInfo.setAppName(appNameTmp);
                }
                //在这里更新数据库的配置信息
                updateConfigInfoAtomic(configInfo, srcIp, srcUser, configAdvanceInfo);
                Timestamp now = new Timestamp(System.currentTimeMillis());
                historyConfigInfoPersistService.insertConfigHistoryAtomic(oldConfigInfo.getId(), oldConfigInfo, srcIp, srcUser, now, "U");
                return getConfigInfoOperateResult(configInfo.getDataId(), configInfo.getGroup(), configInfo.getTenant());
            } catch (CannotGetJdbcConnectionException e) {
                LogUtil.FATAL_LOG.error("[db-error] " + e, e);
                throw e;
            }
        });
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:非CAS更新数据库中配置信息的方法，该方法在更新配置信息的时候，并没有对比客户端传输过来的旧配置信息的MD5的值是否和数据库中存储的旧配置信息的MD5的值是否一致
     * 而是直接就更新了，该方法的逻辑和updateConfigInfoAtomicCas方法的逻辑几乎一致，所以我就不再重复添加注释了
     */
    @Override
    public void updateConfigInfoAtomic(final ConfigInfo configInfo, final String srcIp, final String srcUser, Map<String, Object> configAdvanceInfo) {

        String appNameTmp = StringUtils.defaultEmptyIfBlank(configInfo.getAppName());
        String tenantTmp = StringUtils.defaultEmptyIfBlank(configInfo.getTenant());
        final String md5Tmp = MD5Utils.md5Hex(configInfo.getContent(), Constants.ENCODE);
        String desc = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("desc");
        String use = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("use");
        String effect = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("effect");
        String type = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("type");
        String schema = configAdvanceInfo == null ? null : (String) configAdvanceInfo.get("schema");
        final String encryptedDataKey = configInfo.getEncryptedDataKey() == null ? StringUtils.EMPTY : configInfo.getEncryptedDataKey();

        try {

            ConfigInfoMapper configInfoMapper = mapperManager.findMapper(dataSourceService.getDataSourceType(), TableConstant.CONFIG_INFO);
            Timestamp now = new Timestamp(System.currentTimeMillis());
            jt.update(configInfoMapper.update(Arrays.asList("content", "md5", "src_ip", "src_user", "gmt_modified", "app_name", "c_desc", "c_use",
                                            "effect", "type", "c_schema", "encrypted_data_key"),
                            Arrays.asList("data_id", "group_id", "tenant_id")), configInfo.getContent(), md5Tmp, srcIp, srcUser,
                    now, appNameTmp, desc, use, effect, type, schema, encryptedDataKey, configInfo.getDataId(),
                    configInfo.getGroup(), tenantTmp);
        } catch (CannotGetJdbcConnectionException e) {
            LogUtil.FATAL_LOG.error("[db-error] " + e, e);
            throw e;
        }
    }

    

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:从数据库中查询指定配置信息状态的方法，其实就是看看能不能查到指定的配置信息而已
     */
    @Override
    public ConfigInfoStateWrapper findConfigInfoState(final String dataId, final String group, final String tenant) {
        String tenantTmp = StringUtils.isBlank(tenant) ? StringUtils.EMPTY : tenant;
        try {
            //直接查询指定配置信息
            return this.jt.queryForObject(
                    "SELECT id,data_id,group_id,tenant_id,gmt_modified FROM config_info WHERE data_id=? AND group_id=? AND tenant_id=?",
                    new Object[] {dataId, group, tenantTmp}, CONFIG_INFO_STATE_WRAPPER_ROW_MAPPER);
        } catch (EmptyResultDataAccessException e) {
            return null;
        } catch (CannotGetJdbcConnectionException e) {
            LogUtil.FATAL_LOG.error("[db-error] " + e.toString(), e);
            throw e;
        }
    }



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:从数据库中查找指定配置信息的方法
     */
    @Override
    public ConfigInfoWrapper findConfigInfo(final String dataId, final String group, final String tenant) {
        final String tenantTmp = StringUtils.isBlank(tenant) ? StringUtils.EMPTY : tenant;
        try {

            ConfigInfoMapper configInfoMapper = mapperManager.findMapper(dataSourceService.getDataSourceType(), TableConstant.CONFIG_INFO);
            return this.jt.queryForObject(configInfoMapper.select(Arrays.asList("id", "data_id", "group_id", "tenant_id", "app_name", "content", "md5", "type",
                                    "encrypted_data_key", "gmt_modified"), Arrays.asList("data_id", "group_id", "tenant_id")),
                    new Object[] {dataId, group, tenantTmp}, CONFIG_INFO_WRAPPER_ROW_MAPPER);

        } catch (EmptyResultDataAccessException e) {
            return null;
        } catch (CannotGetJdbcConnectionException e) {
            LogUtil.FATAL_LOG.error("[db-error] " + e, e);
            throw e;
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:从数据库中移除执行配置信息的方法
     */
    @Override
    public void removeConfigInfo(final String dataId, final String group, final String tenant, final String srcIp,
                                 final String srcUser) {

        tjt.execute(new TransactionCallback<Boolean>() {
            final Timestamp time = new Timestamp(System.currentTimeMillis());
            @Override
            public Boolean doInTransaction(TransactionStatus status) {
                try {
                    //先查找数据库中是否存在指定的配置信息
                    ConfigInfo configInfo = findConfigInfo(dataId, group, tenant);
                    if (configInfo != null) {
                        //找到了对应的配置信息才执行移除操作
                        removeConfigInfoAtomic(dataId, group, tenant, srcIp, srcUser);
                        //把移除的配置信息添加到历史信息表中，方便以后回滚配置信息
                        historyConfigInfoPersistService.insertConfigHistoryAtomic(configInfo.getId(), configInfo, srcIp, srcUser, time, "D");
                    }
                } catch (CannotGetJdbcConnectionException e) {
                    LogUtil.FATAL_LOG.error("[db-error] " + e, e);
                    throw e;
                }
                return Boolean.TRUE;
            }
        });
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/25
     * @Description:从数据库中移除指定配置信息的方法
     */
    @Override
    public void removeConfigInfoAtomic(final String dataId, final String group, final String tenant, final String srcIp, final String srcUser) {

        String tenantTmp = StringUtils.isBlank(tenant) ? StringUtils.EMPTY : tenant;
        try {
            ConfigInfoMapper configInfoMapper = mapperManager.findMapper(dataSourceService.getDataSourceType(), TableConstant.CONFIG_INFO);
            jt.update(configInfoMapper.delete(Arrays.asList("data_id", "group_id", "tenant_id")), dataId, group, tenantTmp);
        } catch (CannotGetJdbcConnectionException e) {
            LogUtil.FATAL_LOG.error("[db-error] " + e, e);
            throw e;
        }
    }
}
```

上面代码块中的内容非常多，但是逻辑都很简单，都是对数据库的 config_info 表执行的简单的增删改查操作，我就不再重复讲解其中的内容了，大家简单看看，或者直接去我提供的第十六版本代码中查看即可，唯一一点需要补充的是，**在上面代码块展示的 ExternalConfigInfoPersistServiceImpl 类中定义了一个 HistoryConfigInfoPersistService 成员变量，这个类的对象就是专门用来持久化历史配置信息到数据库的 his_config_info 表中，至于历史配置信息的作用也很简单，一个是用来帮助开发者查看配置变更的轨迹，另一个就是在需要的时候对某个配置信息进行版本回滚**。HistoryConfigInfoPersistService 组件的具体内容我就不在文章中展示了，相关的注释我都写在第十六版本代码中了，大家直接去十六版本代码中查看即可。好了，到此为止，我们把持久化配置信息的 service 持久层服务组件也定义完毕了。接下来就剩下最后一点工作要完成了，**那就是继续实现 ConfigPublishRequestHandler 请求处理器中的 handle() 方法，在该方法中，使用 ExternalConfigInfoPersistServiceImpl 对象，把接收到的客户端发布过来的配置信息持久化到数据库中**。

## 完善 ConfigPublishRequestHandler 处理器

  

继续实现 ConfigPublishRequestHandler 请求处理器中的 handle() 方法已经非常容易了，ExternalConfigInfoPersistServiceImpl 对象已经定义完毕了，直接调用该对象的向数据库中添加新配置信息的方法不就行了吗？既然我提出了这个问题，那么答案已经显而易见了，我们当然不能这么做，因为首先你得分析这次客户端发布过来的数据，究竟要执行的是新增操作还是更新操作，所以，**我们不能直接调用 ExternalConfigInfoPersistServiceImpl 对象的 addConfigInfo() 把配置信息直接存储到数据库中，而是应该调用该对象的 insertOrUpdate() 方法，在该方法中判断究竟应该执行新增操作还是更新操作**。如果大家理解了这个逻辑，那接下来就有一个新的问题，那就是在 ExternalConfigInfoPersistServiceImpl 对象中除了存在 insertOrUpdate() 方法，还存在另一个 insertOrUpdateCas() 方法，这两个方法都是新增或者更新数据库中配置信息的操作，只不过一个使用的是 CAS 的方式执行该操作，一个并没有使用 CAS 的方式执行该操作。那么所谓的 CAS 方式究竟是什么呢？这个解释起来非常简单，**那就是当配置中心客户端想要更新服务端的某个配置信息时，会把自己之前保存的配置信息的 MD5 的值一起发送给服务端，如果客户端发送的 MD5 的值和存储在数据库中保存的配置信息的 MD5 是一致的，就意味着 CAS 对比通过了，说明是期望更新的配置信息，那就可以直接更新配置信息了**。所以，**我们在实现 ConfigPublishRequestHandler 类的 handle() 方法时，还要判断客户端是否把配置信息的 MD5 的值也一起传输过来了，如果传输过来了就要调用 insertOrUpdateCas() 方法执行相应操作**。这就是 handle() 方法的完整逻辑，具体实现请看下面代码块。

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

    

     

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/24
     * @Description:处理客户端发送的ConfigPublishRequest请求的方法
     */
    @Override
    public ConfigPublishResponse handle(ConfigPublishRequest request, RequestMeta meta) throws NacosException {

        try {
            //从请求中得到客户端发布的配置信息的dataId
            String dataId = request.getDataId();
            //得到组名
            String group = request.getGroup();
            //得到客户端要发布的配置的内容
            String content = request.getContent();
            //得到租户信息
            final String tenant = request.getTenant();
            //得到发布配置信息的客户端的IP地址
            final String srcIp = meta.getClientIp();
            //得到应用的IP地址
            final String requestIpApp = request.getAdditionParam("requestIpApp");
            //得到配置信息的标签，在我们的代码中，客户端
            //并没有为发布的配置信息定义标签，所以这里得到的就是null
            //大家理解这个意思即可
            final String tag = request.getAdditionParam("tag");
            //得到应用配置信息的应用名称
            final String appName = request.getAdditionParam("appName");
            //得到配置信息的类型
            final String type = request.getAdditionParam("type");
            //得到发布配置信息的用户
            final String srcUser = request.getAdditionParam("src_user");
            //得到加密配置信息的密钥
            final String encryptedDataKey = request.getAdditionParam("encryptedDataKey");
            //对配置信息的各个参数进行校验
            ParamUtils.checkParam(dataId, group, "datumId", content);
            //对配置信息的标签进行校验，确保其不为空的时候格式正确
            ParamUtils.checkParam(tag);

            //定义一个map，用来存储客户端发布的配置信息的额外信息，当然，在我为大家提供的代码中，根本没有为配置信息设置以下这些
            //附加的信息，实际上在nacos源码中也没有设置，大家在使用nacos的过程中，可以自己设置一下
            Map<String, Object> configAdvanceInfo = new HashMap<>(10);
            //得到配置信息额外的标签信息
            MapUtil.putIfValNoNull(configAdvanceInfo, "config_tags", request.getAdditionParam("config_tags"));
            //得到配置信息的描述信息，这一点在web控制台发布配置信息的时候可以看到
            MapUtil.putIfValNoNull(configAdvanceInfo, "desc", request.getAdditionParam("desc"));
            //得到配置信息的用途
            MapUtil.putIfValNoNull(configAdvanceInfo, "use", request.getAdditionParam("use"));
            //得到配置信息的生效条件
            MapUtil.putIfValNoNull(configAdvanceInfo, "effect", request.getAdditionParam("effect"));
            //得到配置信息的类型
            MapUtil.putIfValNoNull(configAdvanceInfo, "type", type);
            //得到配置信息的数据格式
            MapUtil.putIfValNoNull(configAdvanceInfo, "schema", request.getAdditionParam("schema"));
            //对刚才得到的这些信息进行校验
            ParamUtils.checkParam(configAdvanceInfo);

            //创建ConfigInfo对象，当把客户端发布的配置信息存储到数据库时，就会把信息封装到ConfigInfo对象中
            //然后进行存储，这些数据会被存放到数据库的`config_info`表中
            ConfigInfo configInfo = new ConfigInfo(dataId, group, tenant, appName, content);
            //得到客户端发送过来的配置信息的MD5信息，当然，这种情况一般是客户端更新配置信息的时候才发生
            //如果是客户端发布配置信息，通常不会携带配置信息的MD5的值，而是在服务端内部计算，得到配置信息的MD5的值
            configInfo.setMd5(request.getCasMd5());
            //设置配置信息的类型
            configInfo.setType(type);
            //设置配置信息的密钥
            configInfo.setEncryptedDataKey(encryptedDataKey);
            //得到客户端发送过来的请求中是否包含了测试IP地址，也就是灰度发布服务器的IP地址
            //在我们的代码中，客户端并没有为发布的配置信息定义灰度发布模式，所以这里得到的就是null
            //大家理解这个意思即可
            String betaIps = request.getAdditionParam("betaIps");
            //定义一个对象，用来接收数据持久化操作的结果
            ConfigOperateResult configOperateResult = null;
            //定义一个持久化数据的事件标志，之后记录操作日志时会用到这个标志
            String persistEvent = ConfigTraceService.PERSISTENCE_EVENT;

            //判断betaIps是否为空，如果为空则意味着不执行灰度发布，直接正常发布配置信息，将数据持久化即可
            //在源码中这里有很多和灰度发布的内容，在我提供的代码中，我把那些内容都省略了，在更新nacos支线内容的时候
            //我会为大家把对应的功能都实现了
            if (StringUtils.isBlank(betaIps)) {
                //接下来判断配置信息的标签是否为空，如果标签也为空，则意味着当前发布的配置信息可以直接凭借datId+group+命名空间就可以确定
                if (StringUtils.isBlank(tag)) {
                    //接下来就可以直接把配置信息持久化即可，如果配置信息有对应的标签，就要使用configInfoTagPersistService服务组件
                    //把配置信息存储到数据库中，这个configInfoTagPersistService服务组件专门用来和数据库中的config_info_tag表打交道
                    //在我为大家提供的代码中同样把标签这部分的功能省略了，大家感兴趣的话可以自己看看源码，只要configInfoPersistService组件中的逻辑理解了
                    //那么和其他表打交道的服务组件的逻辑就都很好理解了
                    //接下来就是判断了一下客户端发送过来的请求中究竟有没有携带配置信息的MD5的值
                    //如果携带了就要先用客户端发送过来的MD5的值和服务端存储的配置信息的MD5的值对比一下，如果相同，则意味着CAS操作通过了，可以执行对应的更新操作了
                    if (StringUtils.isNotBlank(request.getCasMd5())) {
                        //这里就是需要先对比MD5的值，然后执行数据持久化的操作，执行完毕之后会返回一个操作结果
                        configOperateResult = configInfoPersistService.insertOrUpdateCas(srcIp, srcUser, configInfo, configAdvanceInfo);
                        //判断操作是否成功
                        if (!configOperateResult.isSuccess()) {
                            //失败则直接回复客户端操作失败响应
                            return ConfigPublishResponse.buildFailResponse(ResponseCode.FAIL.getCode(),
                                    "Cas publish fail,server md5 may have changed.");
                        }
                    } else {
                        //走到这里意味着客户端没有发送配置信息的MD5的值，那在持久化数据库之前，就不需要比较MD5的值
                        configOperateResult = configInfoPersistService.insertOrUpdate(srcIp, srcUser, configInfo, configAdvanceInfo);
                    }
                    //这里有一个非常重要的操作，就是发布配置变更事件，原因很简单，不管是插入新的配置信息还是更新旧的配置信息，都意味着配置信息发生变更了
                    //但这里后续怎么处理，我在第十六版本代码中还不为大家实现，等到第十八版本代码中在实现对应的功能，到时候也许大家会发现实现的功能可能跟你们想象的有些不一样
                    //大家先记住这个事件即可
                    ConfigChangePublisher.notifyConfigChange(new ConfigDataChangeEvent(false, dataId, group, tenant, configOperateResult.getLastModified()));
                }
            }
            //记录持久化配置信息的操作日志
            ConfigTraceService.logPersistenceEvent(dataId, group, tenant, requestIpApp,
                    configOperateResult.getLastModified(), srcIp, persistEvent, ConfigTraceService.PERSISTENCE_TYPE_PUB, content);
            //回复客户端操作成功响应
            return ConfigPublishResponse.buildSuccessResponse();
        } catch (Exception e) {
            Loggers.REMOTE_DIGEST.error("[ConfigPublishRequestHandler] publish config error ,request ={}", request, e);
            return ConfigPublishResponse.buildFailResponse(
                    (e instanceof NacosException) ? ((NacosException) e).getErrCode() : ResponseCode.FAIL.getCode(),
                    e.getMessage());
        }
    }
    
}
```

到此为止，我就为大家实现了配置中心服务端存储客户端发布过来的配置信息的功能。当然，目前只是初步实现了，只实现了把配置信息持久化到数据库中的功能，但在文章的前半部分我就跟大家说了，**在 nacos 配置中心源码中，除了把客户端发布过来的配置信息持久化到数据库中之外，还会把配置信息缓存在内存中，同时也会把配置信息保存在本地硬盘的对应文件中**。我们先不管这两个操作是如何实现的，我认为我们首先应该思考的是，为什么要执行这两个操作呢？把配置信息持久化到数据库中难道还不够吗？这就要从客户端向服务端查询指定配置信息的功能说起了。当然，这一章是讲不完这些内容了，就留到下一章为大家讲解吧。好了朋友们，我们下一章见！

附：看完本章之后，大家就可以看我提供的第十六版本代码了，该版本代码中的所有内容都可以看了。不过我要跟大家说声抱歉，因为在我为大家提供的十八个版本的 nacos 代码中，从第十版本开始，后面的八个版本代码我都没来得及提供测试类，有的虽然也提供了，但是需要配合 nacos 配置中心服务端源码使用，大家可以先看看代码中各个功能实现的具体逻辑，结合注释和文章应该很容易就能看懂。看懂了之后可以直接去 nacos 源码中运行测试类，或者再等一等，等我把 nacos 的文章全部更新完毕后，会抽时间为大家把后八个版本代码的测试类补充上。