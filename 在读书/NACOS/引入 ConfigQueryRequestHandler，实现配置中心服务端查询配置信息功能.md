  

## 引入 DumpService 数据导出器

在上一章结尾留下了一个小尾巴，那就是定义一个定时任务，定时任务会定期数据库中存储的所有配置信息导入到服务端的缓存系统和文件系统，其实就是在每一次执行的时候，调用 ConfigCacheService 类的 dump() 方法。这个定时任务一旦定义完毕，并且成功启动，那么配置中心服务端存储客户端发布的配置信息的功能也就全部实现完毕了。但是在上一章我们并没有讨论出这个定时任务该在哪里定义，在什么时候启动，所以接下来，就让我们一起来看看这个定时任务的创建和启动时机。

首先让我们来分析一下这个定时任务的启动时机，因为定时任务的启动时机分析完毕之后，定时任务的创建时机也就清楚了，只需要在启动定时任务之前创建该任务即可。按照 nacos 框架深度依赖 springboot 的特性，并且这个定时任务执行的操作又是把数据库中的配置信息导入到服务端的缓存系统和文件系统，**那这个定时任务其实就可以在 springboot 程序启动的过程中被创建来，然后提交给定时任务执行器。当然，真正执行应该在程序完全启动之后才执行，所以应该给这个定时任务定义一个延迟执行时间**。同时还要考虑到这个定时任务每一次执行的时候，都会查询数据库中的所有配置信息，然后倒入到服务端的缓存系统和文件系统，这个工程量是比较大的，所以要把定时任务的执行间隔时间定义的长一些。这些细节 nacos 源码都考虑到了，所以在 nacos 配置中心源码中，就把这个定时任务的执行间隔时间定义为了 6 小时，并且定义了值为 10 分钟的最小延迟执行时间。这也就是说，**当程序在 springboot 启动的过程中会把这个定时任务创建出来，然后把它提交给执行器，而执行器至少会在十分钟之后才开始执行这个定时任务，并且每一个执行的时间间隔为 6 个小时**。如果大家理解了这个逻辑，接下来我就要展示相关的代码了。

**在 nacos 配置中心源码中，定义了一个 DumpService 数据导出服务组件，这个 DumpService 类的对象就是专门用来把数据库中的信息导入到服务端的缓存系统和文件系统中的**。DumpService 是一个抽象类，在这个类下还有两个子类，一个是 ExternalDumpService 类，这个类的对象就是用来把外部数据源，也就是 mysql 数据库中的配置信息导入到务端的缓存系统和文件系统中的；另一个是 EmbeddedDumpService 类，这个类的对象就是用来把内部数据源，也就是 derby 数据库中的配置信息导入到务端的缓存系统和文件系统中的。**在我为大家提供的第十七版本代码中，只实现了 ExternalDumpService 类，毕竟我们使用的是 mysql 数据库存储客户端发布的配置信息**。接下来我就先为大家展示一下 ExternalDumpService 类的内容，请看下面代码块。

```
package com.cqfy.nacos.config.server.service.dump;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/28
 * @Description:外部数据源的配置信息导出服务组件，与此类相对应的就是使用了内部数据源，也就是derby数据库的配置信息导出服务组件，也就是EmbeddedDumpService类
 * 这不过在主线内容中我并没有为大家引入该类，实现支线内容的时候，就会为大家引入该类的。当然，只要大家ExternalDumpService和DumpService的内容掌握了
 * 那么EmbeddedDumpService类的内容也就很容易看懂了，方法逻辑几乎都没什么区别，只不过就是使用的存储配置信息的数据库不同而已
 */
@Conditional(ConditionOnExternalStorage.class)
@Component
@DependsOn({"rpcConfigChangeNotifier"})
public class ExternalDumpService extends DumpService {


    public ExternalDumpService(ConfigInfoPersistService configInfoPersistService, HistoryConfigInfoPersistService historyConfigInfoPersistService, ServerMemberManager memberManager) {
        super(configInfoPersistService, historyConfigInfoPersistService, memberManager);
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:该方法会在当前类的对象被创建之后调用
     */
    @PostConstruct
    @Override
    protected void init() throws Throwable {
        //调用父类的方法
        dumpOperate(dumpAllProcessor);
    }


    //判断任务是否可以被执行的方法
    @Override
    protected boolean canExecute() {
        return memberManager.isFirstIp();
    }
}
```

从上面代码块中可以看到，ExternalDumpService 类的对象也被 springboot 容器管理，并且在 springboot 创建完该对象之后，就会调用该对象的 init() 方法，在该方法中，就会调用父类，也就是 DumpService 类的 dumpOperate() 方法。现在我可以告诉大家：**在 DumpService 类的 dumpOperate() 方法中，就会把我们之前分析的那个定时任务创建出来，然后提交给定时任务执行器**，与此同时也会创建一些其他的定时任务，比**如清除历史配置信息的定时任务；定期检查数据库中是否有配置发生变更，然后把变更的配置信息更新到服务端的缓存系统和文件系统中的定时任务等等**。这些定时任务我就不在文章中展开了，大家直接去我提供的第十七版本代码中查看即可，代码中注释非常详细，而且定时任务的逻辑非常简单，就是从数据库中查询配置信息，然后调用 ConfigCacheService 缓存组件的 dump() 方法，真的没什么可讲的。好了，话不多说，接下来就请大家看一看 DumpService 类的内容吧，请看下面代码块。

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


    private static final Logger LOGGER = LoggerFactory.getLogger(DumpService.class);

    //这个处理器就是用来把数据库中的信息导出到内存和本地硬盘文件中的
    protected DumpAllProcessor dumpAllProcessor;

    //持久化配置信息的服务组件
    protected ConfigInfoPersistService configInfoPersistService;

    //出久化配置历史信息的服务组件
    protected HistoryConfigInfoPersistService historyConfigInfoPersistService;

    //集群成员管理器
    protected final ServerMemberManager memberManager;

    //定期把配置信息更新到缓存和本地文件系统的时间间隔
    static final int DUMP_ALL_INTERVAL_IN_MINUTE = 6 * 60;

    //定期把变更的配置信息更新到缓存的时间间隔
    static final int DUMP_CHANGE_INTERVAL_IN_SECONDS = 15;

    //这个是定时任务的初始延迟执行时间
    static final int INITIAL_DELAY_IN_MINUTE = 6 * 60;

    //配置变更数据更新任务管理器，这个任务管理器会在服务端存储的配置发生变更后开始执行任务
    //在第十七版本代码中还用不到这个任务管理器，大家暂时忽略即可
    private TaskManager dumpTaskMgr;

    //这个就是任务管理器就是用来处理全量配置信息导出任务的，其实TaskManager就是一个延迟任务执行引擎
    //这个执行引擎会持有刚才的DumpAllProcessor全量配置信息导出任务处理器
    //至于它是一个延迟任务执行引擎的原因非常简单，配置变更比较频繁的时候，可以把相同的配置信息合并一下，减少冗余操作的次数，这一点在处理DumpTask任务的时候体现的非常明显
    //在第十八版本代码大家就清楚了
    private TaskManager dumpAllTaskMgr;

    //历史配置信息在数据库中保存的最大天数
    private int retentionDays = 30;


    //构造方法
    public DumpService(ConfigInfoPersistService configInfoPersistService, HistoryConfigInfoPersistService historyConfigInfoPersistService, ServerMemberManager memberManager) {

        this.configInfoPersistService = configInfoPersistService;
        this.historyConfigInfoPersistService = historyConfigInfoPersistService;
        this.memberManager = memberManager;
        this.dumpAllProcessor = new DumpAllProcessor(this);
        this.dumpTaskMgr = new TaskManager("com.cqfy.nacos.server.DumpTaskManager");
        this.dumpAllTaskMgr = new TaskManager("com.cqfy.nacos.server.DumpAllTaskManager");
        //在这里把DumpAllProcessor处理器交给全量配置信息导出任务管理器使用了
        this.dumpAllTaskMgr.setDefaultTaskProcessor(dumpAllProcessor);
        this.dumpAllTaskMgr.addProcessor(DumpAllTask.TASK_ID, dumpAllProcessor);
        //在这里还执行了一个操作，那就是得到nacos程序真正要使用的数据源对象
        DynamicDataSource.getInstance().getDataSource();
    }

    public ConfigInfoPersistService getConfigInfoPersistService() {
        return configInfoPersistService;
    }

    public HistoryConfigInfoPersistService getHistoryConfigInfoPersistService() {
        return historyConfigInfoPersistService;
    }


    //该方法会被子类实现，在子类的init()方法中会调用父类的dumpOperate()方法，开始执行全量配置信息导出任务，也就是
    //把配置信息更新到缓存和本地文件中
    protected abstract void init() throws Throwable;


    //接下来就是本类最重要的dumpOperate()方法了


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:该方法是该类最核心的方法，当nacos服务端启动的时候，该方法就会被调用，一旦该方法被调用
     * 那么程序就会开始定期把数据库中的配置信息更新到缓存系统和本地文件系统中，在第十七版本代码中，我把导出灰度发布配置信息和定义了标签的配置信息的功能给省略了
     * 后面实现支线功能时会为大家实现的
     */
    protected void dumpOperate(DumpAllProcessor dumpAllProcessor) throws NacosException {
        //定义将要执行的操作的名称
        String dumpFileContext = "CONFIG_DUMP_TO_FILE";
        //记录操作开始执行的事件
        TimerContext.start(dumpFileContext);
        try {
            //记录操作开始日志
            LogUtil.DEFAULT_LOG.warn("DumpService start");

            
            //定义一个异步任务，这个任务会交给定时任务执行器来执行，执行的逻辑就是把全量导出配置信息的任务交给TaskManager任务管理器
            //也就是延迟任务执行器来处理，这个就是全量导出配置信息到文件和缓存系统的任务，这个任务会被提交到dumpAllTaskMgr中
            //而dumpAllTaskMgr其实就是一个延迟任务执行器，TaskManager继承了NacosDelayTaskExecuteEngine，这一点大家可以直接去第十七版本代码中查看
            //我就不在文章中展开了，而提交到dumpAllTaskMgr中的任务最后会被DumpAllProcessor处理器对象处理，也就是被本类的dumpAllProcessor成员变量处理
            //我把DumpAllProcessor处理器的内容也展示在当前代码块中了，大家可以看一看，逻辑非常简单，就是把数据库中的配置信息查询出来
            //然后倒入到文件和缓存系统中的操作
            Runnable dumpAll = () -> dumpAllTaskMgr.addTask(DumpAllTask.TASK_ID, new DumpAllTask());
            
            
            //定义一个清理历史配置信息的任务
            Runnable clearConfigHistory = () -> {
                //记录操作开始执行的日志
                LOGGER.warn("clearConfigHistory start");
                //判断当前节点是否可以执行清楚历史配置信息的操作
                //这里的canExecute()方法的逻辑非常简单，就是memberManager.isFirstIp()这行代码，而这行代码的意义也很明确，就是判断当前节点的IP地址是否
                //是集群成员管理器的ConcurrentSkipListMap成员变量中的第一个键，接下来让我为大家解释一下这里为什么这么做
                //要想理解这一点，首先大家应该清楚，在构建nacos配置中心集群时，尤其是使用mysql外部数据源存储配置信息时，那么nacos集群的每一个节点都会访问同一个数据库
                //也就是数据库主节点，mysql数据库自己也可以构建自己的集群，用来备份数据，这一点大家应该清楚吧？如果是这样的话，那么nacos集群的每一个节点操纵的都是同一个mysql
                //数据库，新增、移除或者更新数据还好说，每一个节点处理自己的数据即可，但是在清楚配置历史信息的时候，尤其是这是一个定时任务，每一个节点都可以操作历史信息表
                //在这种情况下，如果每一个节点都启动清理历史配置信息的定时任务，那显然就操作重复了，所以这里就判断当前节点的网络地址是不是ConcurrentSkipListMap中的第一个键
                //因为集群节点的信息都会添加到集群成员管理器的ConcurrentSkipListMap成员变量中，而这些数据在ConcurrentSkipListMap中的排序方式是根据键来决定的
                //先添加的不一定就在第一位，是要经过计算的，所以只要集群节点的信息是固定的，那么ConcurrentSkipListMap中的第一个键就是固定的，所以每次只要这个键对应的节点执行定时清理历史配置信息
                //的任务即可，就不会出现操作重复的情况了
                if (canExecute()) {
                    try {
                        //得到要清理的历史配置信息的时间范围，getRetentionDays()方法得到的就是历史配置信息的最大保存时间
                        Timestamp startTime = getBeforeStamp(TimeUtils.getCurrentTime(), 24 * getRetentionDays());
                        //定义每一次从数据库中清理的历史配置信息的数量
                        int pageSize = 1000;
                        //记录操作日志
                        LOGGER.warn("clearConfigHistory, getBeforeStamp:{}, pageSize:{}", startTime, pageSize);
                        //执行清理历史配置信息的操作
                        historyConfigInfoPersistService.removeConfigHistory(startTime, pageSize);
                    } catch (Throwable e) {
                        LOGGER.error("clearConfigHistory error : {}", e.toString());
                    }
                }
            };
            //得到系统当前时间
            Timestamp currentTime = new Timestamp(System.currentTimeMillis());
            
            
            
            try {
                //在这里执行一次全量配置信息导出任务，也就是把数据库中的配置信息更新到缓存系统和本地文件系统中
                dumpConfigInfo(dumpAllProcessor);
            } catch (Exception e) {
                FATAL_LOG.error("Nacos Server did not start because dumpservice bean construction failure :\n" + e);
                throw new NacosException(NacosException.SERVER_ERROR,
                        "Nacos Server did not start because dumpservice bean construction failure :\n" + e.getMessage(), e);
            }


            
            //接着判断当前程序是否是以集群模式启动的
            if (!EnvUtil.getStandaloneMode()) {
                
                //得到一个初始延迟时间
                long initialDelay = new Random().nextInt(INITIAL_DELAY_IN_MINUTE) + 10;
                LogUtil.DEFAULT_LOG.warn("initialDelay:{}", initialDelay);
                //把全量导出配置信息的任务提交给定时任务执行器定期执行
                ConfigExecutor.scheduleConfigTask(dumpAll, initialDelay, DUMP_ALL_INTERVAL_IN_MINUTE, TimeUnit.MINUTES);
                //把配置变更信息导出任务提交给定时任务执行器执行，DumpChangeConfigWorker这个任务的内容我就不在文章中展示了
                //内容非常简单，大家直接看我提供的第十七版本代码即可
                ConfigExecutor.scheduleConfigTask(new DumpChangeConfigWorker(this, currentTime), 0, DUMP_CHANGE_INTERVAL_IN_SECONDS, TimeUnit.SECONDS);

            }
            //十分钟之后，开始定期执行历史配置信息清理任务
            ConfigExecutor.scheduleConfigTask(clearConfigHistory, 10, 10, TimeUnit.MINUTES);
        } finally {//计算操作结束时间，并记录日志
            TimerContext.end(dumpFileContext, DUMP_LOG);
        }

    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:把数据库中的配置信息导出到缓存系统和本地文件系统中的方法
     */
    private void dumpConfigInfo(DumpAllProcessor dumpAllProcessor) throws IOException {
        try {
            LogUtil.DEFAULT_LOG.info("start clear all config-info.");
            //清空本地文件的配置信息
            DiskUtil.clearAll();
            //把数据库中的信息导出
            dumpAllProcessor.process(new DumpAllTask());
        } catch (Exception e) {
            FATAL_LOG.error("dump config fail" + e.getMessage());
            throw e;
        }
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:得到可以清除的历史配置信息时间的方法
     */
    private Timestamp getBeforeStamp(Timestamp date, int step) {

        Calendar cal = Calendar.getInstance();
        cal.setTime(date);
        cal.add(Calendar.HOUR_OF_DAY, -step);
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        return Timestamp.valueOf(format.format(cal.getTime()));
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/28
     * @Description:得到历史配置信息保留天数的方法
     */
    private int getRetentionDays() {
        //先从用户定义的配置信息中获取，如果没有就使用默认值
        String val = EnvUtil.getProperty("nacos.config.retention.days");
        if (null == val) {
            return retentionDays;
        }
        int tmp = 0;
        try {
            tmp = Integer.parseInt(val);
            if (tmp > 0) {
                retentionDays = tmp;
            }
        } catch (NumberFormatException nfe) {
            FATAL_LOG.error("read nacos.config.retention.days wrong", nfe);
        }
        return retentionDays;
    }

    
    //该方法在子类中被实现了
    protected abstract boolean canExecute();
    
}





//接下来是DumpAllProcessor处理器的内容



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/26
 * @Description:把数据库中全部配置信息写入到本地文件的处理器，在写入的过程中，还会更新缓存中的配置信息
 */
public class DumpAllProcessor implements NacosTaskProcessor {


    public DumpAllProcessor(DumpService dumpService) {
        this.dumpService = dumpService;
        this.configInfoPersistService = dumpService.getConfigInfoPersistService();
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/26
     * @Description:把数据库中全部配置信息写入到本地的方法
     */
    @Override
    public boolean process(NacosTask task) {
        //查询得到当前数据库中配置信息的最大Id
        long currentMaxId = configInfoPersistService.findConfigMaxId();
        //从Id为0开始查起，大家也许会疑惑，每次执行定时任务的时候都从ID为0的配置信息查起
        //如果是这样的话，上一次定时任务已经把Id为1的配置信息更新到缓存系统和文件系统了，定时任务第二次执行的时候还要再查询id为1的配置信息
        //然后再更新到缓存系统和文件系统中吗？这个操作大家不必担心，因为在ConfigCacheService.dump()的方法中，会根据配置信息的MD5和最新修改时间
        //判断配置信息有没有发生过更新，如果没有变更过，是不会更新到文件和缓存系统中的
        long lastMaxId = 0;
        //在循环中查询数据库中的所有配置信息，直到查询的配置信息的Id是最大Id，这个时候就意味着查询到头了
        while (lastMaxId < currentMaxId) {
            //分页查询配置信息，查询一页数据
            Page<ConfigInfoWrapper> page = configInfoPersistService.findAllConfigInfoFragment(lastMaxId, PAGE_SIZE);
            //如果没有查到数据，就意味着查完了，可以结束循环了
            if (page == null || page.getPageItems() == null || page.getPageItems().isEmpty()) {
                break;
            }
            //循环遍历查询到的这一页数据
            for (ConfigInfoWrapper cf : page.getPageItems()) {
                //得到配置信息的Id
                long id = cf.getId();
                //对比Id，然后不断更新lastMaxId的值，直到lastMaxId等于配置信息的最大Id
                lastMaxId = Math.max(id, lastMaxId);
                //把从数据库中查询到的配置信息更新到缓存中，同时也会写入到本地的硬盘文件中
                ConfigCacheService.dump(cf.getDataId(), cf.getGroup(), cf.getTenant(), cf.getContent(), cf.getLastModified(), cf.getType(), cf.getEncryptedDataKey());
                //得到配置信息的MD5的值
                final String content = cf.getContent();
                final String md5 = MD5Utils.md5Hex(content, Constants.ENCODE);
                LogUtil.DUMP_LOG.info("[dump-all-ok] {}, {}, length={}, md5={}", GroupKey2.getKey(cf.getDataId(), cf.getGroup()), cf.getLastModified(), content.length(), md5);
            }
            DEFAULT_LOG.info("[all-dump] {} / {}", lastMaxId, currentMaxId);
        }
        return true;
    }

    //查询数据库每页查询的数据量
    static final int PAGE_SIZE = 1000;

    final DumpService dumpService;

    final ConfigInfoPersistService configInfoPersistService;
}
```

好了，到此为止，我们就终于实现了配置中心服务端存储客户端发布的配置信息全功能。总的来说，**就是配置中心服务端会把客户度发布的配置信息存储在数据库中，而在服务端内部会启动一个定时任务，该定时任务会定期把数据库中的配置信息导入到服务端的缓存系统和本地文件系统中**。通过这些操作大家也能看出来，**nacos 配置中心服务端其实是使用了缓存层、本地文件层、数据库层，三个层次来保存配置信息的**。大家可以再好好消化消化其中的原理和逻辑。

## 引入 ConfigQueryRequestHandler

在实现了配置中心服务端存储客户端发布的配置信息之后，接下来要就实现客户端从服务端查询指定配置信息的功能了。现在来看，这个功能实现起来就非常简单了。客户端向服务端查询指定配置信息时，**会向服务端发送一个 ConfigQueryRequest 请求，服务端会把客户端要查询的配置信息封装在 ConfigQueryResponse 响应中回复给客户端**。那我们就可以直接定一个专门处理 ConfigQueryRequest 请求的处理器，在该处理器中查询客户端指定的配置信息，当然，**现在我们就可以直接从本地文件中查询配置信息的具体内容了，而配置信息的 MD5 的值，以及密钥等信息都缓存在内存中，也可以直接从服务端内部得到**，这些操作就都可以定义在 ConfigQueryRequest 请求处理器中，最后把查询到的数据封装在 ConfigQueryResponse 响应对象中返回给客户端即可。我把服务端专门处理 ConfigQueryRequest 请求的处理器定义为了 ConfigQueryRequestHandler，具体内容也已经实现完毕了，请看下面代码块。

```
package com.cqfy.nacos.config.server.remote;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/10/26
 * @Description:配置中心服务端处理客户端查询配置信息请求的处理器
 */
@Component
public class ConfigQueryRequestHandler extends RequestHandler<ConfigQueryRequest, ConfigQueryResponse> {

    //尝试获取锁的最大次数
    private static final int TRY_GET_LOCK_TIMES = 9;

    //配置信息持久化服务组件
    private final ConfigInfoPersistService configInfoPersistService;


    public ConfigQueryRequestHandler(ConfigInfoPersistService configInfoPersistService) {
        this.configInfoPersistService = configInfoPersistService;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/26
     * @Description:处理ConfigQueryRequest请求的方法
     */
    @Override
    public ConfigQueryResponse handle(ConfigQueryRequest request, RequestMeta meta) throws NacosException {

        try {
            //在这里查询到对应的配置信息，然后返回给客户端
            return getContext(request, meta, request.isNotify());
        } catch (Exception e) {
            return ConfigQueryResponse.buildFailResponse(ResponseCode.FAIL.getCode(), e.getMessage());
        }

    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/26
     * @Description:根据客户端的请求，查询指定配置信息的方法
     */
    private ConfigQueryResponse getContext(ConfigQueryRequest configQueryRequest, RequestMeta meta, boolean notify) throws Exception {
        //从客户端发送过来的请求中得到要查询的配置信息的dataId
        String dataId = configQueryRequest.getDataId();
        //得到配置信息所在组
        String group = configQueryRequest.getGroup();
        //得到租户信息
        String tenant = configQueryRequest.getTenant();
        //得到客户端IP地址
        String clientIp = meta.getClientIp();
        //得到配置信息标签，在我们的代码中，并没有为配置信息设置标签
        String tag = configQueryRequest.getTag();
        //定义一个响应对象，后面会用它来封装查询结果
        ConfigQueryResponse response = new ConfigQueryResponse();

        //得到配置信息组合key
        final String groupKey = GroupKey2.getKey(configQueryRequest.getDataId(), configQueryRequest.getGroup(), configQueryRequest.getTenant());
        //得到客户端应用名称
        String requestIpApp = meta.getLabels().get(CLIENT_APPNAME_HEADER);
        //设置计算配置信息MD5的字符集
        String acceptCharset = ENCODE_UTF8;

        //根据配置信息的组合键尝试获取锁
        int lockResult = tryConfigReadLock(groupKey);
        //设置操作事件的类型，为拉取配置信息事件
        String pullEvent = ConfigTraceService.PULL_EVENT;
        //定义是否要获取灰度发布配置信息的标志
        boolean isBeta = false;

        //判断是否成功获得到锁，如果大鱼0，说明刚才成功得到锁了
        if (lockResult > 0) {
            try {
                String md5 = Constants.NULL;
                long lastModified = 0L;
                
                //根据配置信息的组合键，先从服务端缓存配置信息的服务组件中得到配置信息的缓存项
                //这里我要提醒大家以下，这里得到的这个CacheItem配置信息缓存项中并没有封装配置信息的内容
                //只不过是把配置信息的类型以及密钥，MD5的值缓存起来了
                CacheItem cacheItem = ConfigCacheService.getContentCache(groupKey);
                if (cacheItem != null) {
                    //如果配置信息缓存项存在，那就获取配置信息的类型
                    String configType = cacheItem.getType();
                    //把客户端要查询的配置信息的类型设置到响应对象中
                    response.setContentType((null != configType) ? configType : "text");
                }
                String content = null;
                
                //这个对象就是用来封装从数据库中查询到的配置信息的
                ConfigInfoBase configInfoBase = null;
                
                //判断配置信息是否有特定的标签，在我们的代码中显然没有配置标签
                if (StringUtils.isBlank(tag)) {
                    //从配置信息缓存项中获得配置信息的MD5的值
                    md5 = cacheItem.getConfigCache().getMd5(acceptCharset);
                    //从配置信息缓存项中得到配置信息最后被修改的时间
                    lastModified = cacheItem.getConfigCache().getLastModifiedTs();

                    //接下来判断是否要直接从数据库系统中读取数据
                    if (PropertyUtil.isDirectRead()) {
                        //走到这里就意味着要执行从数据库中读取数据，直接查询数据库即可
                        configInfoBase = configInfoPersistService.findConfigInfo(dataId, group, tenant);
                    } else {
                        //执行到这里就意味着要从本地硬盘的文件系统中读取数据
                        content = ConfigDiskServiceFactory.getInstance().getContent(dataId, group, tenant);
                    }
                    
                    
                    //设置操作事件类型
                    pullEvent = ConfigTraceService.PULL_EVENT;
                    //判断是否查询指定配置信息失败了
                    if (configInfoBase == null && content == null) {
                        //失败则记录操作日志
                        ConfigTraceService.logPullEvent(dataId, group, tenant, requestIpApp, -1, pullEvent,
                                ConfigTraceService.PULL_TYPE_NOTFOUND, -1, clientIp, false, "grpc");
                        //回复客户端错误响应，表示客户端要查询的配置信息不存在
                        response.setErrorInfo(ConfigQueryResponse.CONFIG_NOT_FOUND, "config data not exist");
                        return response;
                    }

                }

                //执行到这里意味着查询配置信息成功了，就把配置信息的MD5的值设置到响应对象中
                response.setMd5(md5);
                //得到配置信息的密钥
                String encryptedDataKey = getEncryptedDataKey(tag, clientIp, cacheItem, isBeta);
                //设置密钥到响应对象中
                response.setEncryptedDataKey(encryptedDataKey);
                //这里还会再次判断一下配置信息是从数据库系统中读取的，还是从本地文件系统中读取的
                if (PropertyUtil.isDirectRead()) {
                    //设置最后修改配置信息的时间到响应中
                    response.setLastModified(lastModified);
                    //设置配置信息到响应对象中
                    response.setContent(configInfoBase.getContent());
                    //设置成功响应
                    response.setResultCode(ResponseCode.SUCCESS.getCode());
                } else {
                    //设置最后修改配置信息的时间到响应中
                    response.setContent(content);
                    //设置配置信息到响应对象中
                    response.setLastModified(lastModified);
                    //设置成功响应
                    response.setResultCode(ResponseCode.SUCCESS.getCode());
                }
                //记录操作日志
                LogUtil.PULL_CHECK_LOG.warn("{}|{}|{}|{}", groupKey, clientIp, md5, TimeUtils.getCurrentTimeStr());
                //这里计算一下，从配置信息最后一次修改之后，过了多久客户端才主动查询到配置信息
                final long delayed = System.currentTimeMillis() - lastModified;
                if (notify) {
                    ConfigTraceService.logPullEvent(dataId, group, tenant, requestIpApp, lastModified, pullEvent,
                            ConfigTraceService.PULL_TYPE_OK, delayed, clientIp, notify, "grpc");
                } else {
                    ConfigTraceService.logPullEvent(dataId, group, tenant, requestIpApp, lastModified, pullEvent,
                            ConfigTraceService.PULL_TYPE_OK, System.currentTimeMillis(), clientIp, notify, "grpc");
                }
            } finally {
                //释放锁
                releaseConfigReadLock(groupKey);
            }
        } else if (lockResult == 0) {
            //执行到这里意味着获取锁失败了，可能意味着服务端根本没有客户端要查询的配置信息，因为锁都是从配置信息的缓存项中获得的
            //缓存项不存在，也就得不到锁，也就意味着没有缓存对应的配置信息
            ConfigTraceService.logPullEvent(dataId, group, tenant, requestIpApp, -1, pullEvent,
                    ConfigTraceService.PULL_TYPE_NOTFOUND, -1, clientIp, notify, "grpc");
            //返回错误响应，表示客户端不存在对应的配置信息
            response.setErrorInfo(ConfigQueryResponse.CONFIG_NOT_FOUND, "config data not exist");

        } else {//执行到这里意味着也没有获取到锁，但这里没获取到锁是因为配置信息可能正在被其他线程修改
            PULL_LOG.info("[client-get] clientIp={}, {}, get data during dump", clientIp, groupKey);
            //设置错误响应，表示配置信息在被并发修改
            response.setErrorInfo(ConfigQueryResponse.CONFIG_QUERY_CONFLICT,
                    "requested file is being modified, please try later.");
        }
        return response;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/26
     * @Description:从指定配置信息缓存项中得到密钥的方法
     */
    private String getEncryptedDataKey(String tag, String clientIp, CacheItem cacheItem, boolean isBeta) {
        if (cacheItem == null) {
            return null;
        }
        String encryptedDataKey;
        if (isBeta && cacheItem.getConfigCacheBeta() != null) {
            encryptedDataKey = cacheItem.getConfigCacheBeta().getEncryptedDataKey();
        } else {
            if (StringUtils.isBlank(tag)) {
                encryptedDataKey = cacheItem.getConfigCache().getEncryptedDataKey();

            } else {
                encryptedDataKey = cacheItem.getTagEncryptedDataKey(tag);
            }
        }
        return encryptedDataKey;
    }


    //释放锁的方法
    private static void releaseConfigReadLock(String groupKey) {
        ConfigCacheService.releaseReadLock(groupKey);
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/10/26
     * @Description:尝试获取锁的方法
     */
    private static int tryConfigReadLock(String groupKey) {

        //定义一个失败结果，如果返回-1，就说明获取锁失败了
        int lockResult = -1;
        //尝试获取锁，最多尝试10次
        for (int i = TRY_GET_LOCK_TIMES; i >= 0; --i) {
            //根据组合键获取对应配置信息的锁
            lockResult = ConfigCacheService.tryReadLock(groupKey);
            //返回0则代表该配置信息并没有缓存项，也就代表配置信息不存在
            if (0 == lockResult) {
                break;
            }
            //大于0则获取锁成功
            if (lockResult > 0) {
                break;
            }
            //以下是重试获取锁的逻辑
            if (i > 0) {
                try {
                    Thread.sleep(1);
                } catch (Exception e) {
                    LogUtil.PULL_CHECK_LOG.error("An Exception occurred while thread sleep", e);
                }
            }
        }
        return lockResult;
    }

}

```

到此为止，客户端从服务端查询指定配置信息的功能也实现完毕了。接下来就该实现配置变更功能了，其实这么说也不太准确，**配置变更与否是客户端和 web 控制台的，只要客户端更新了服务端存储的配置信息，或者用户直接在 web 控制台修改了某个配置信息，那么服务端存储的配置信息肯定就会发生变更**，这些功能我们都已经实现了，无非就是更新数据库中存储的配置信息而已。**所以说配置变更功能本身并不重要，重要的是配置变更之后要执行的各种操作：比如服务端发现有配置变更之后，可以通知客户端，让客户端知道有配置发生变更了；服务端还可以把最新的配置信息同步给集群中的其他节点**。这些功能是十八版本代码的内容，这里就不再展开讲解了。下一章我就会为大家实现这些功能。现在大家已经可以去看我提供的第十七版本代码了，好了朋友们，我们下一章见！