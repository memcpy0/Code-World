这一章是 nacos 注册中心客户端内容的最后一章，在前面几个章节，我已经为大家把 nacos 注册中心客户端的功能实现得差不多了，服务注册，服务订阅，操作失败重试，订阅内容刷新，服务信息缓存等等，这些功能我们都已经实现了，只剩下服务实例注销，服务实例取消订阅这两个功能还没有实现。这一章我就为大家实现这两个功能，当然，已经看过我提供的第六版本代码的朋友应该也发现了，这两个功能实现起来非常简单，因为它们的核心都不在客户端，而在服务端，客户端只是简单发送请求，发送服务实例注销请求，或者是取消服务实例订阅请求，服务端接收到这些请求之后，才会执行具体的操作。所以大家应该也能想到了，虽说我们本章的内容是要实现这两个功能，实际上我可能只会给大家简单展示一下代码，把客户端发送注销服务实例请求和取消服务实例订阅请求的流程梳理一下，这一章的内容就是这么简单。当然，如果说还有一个值得实现的功能，那就是客户端故障转移功能了。这个故障转移功能实现得也非常简单，接下来，我们就先来看看这个故障转移功能的实现吧。

## 引入 FailoverReactor，实现故障转移功能

在具体实现故障转移功能之前，我想先请大家想一想，按照我们的理解，或者按照常规思路，所谓的故障转移功能，应该是什么样的呢？就拿最简单的 RPC 框架的例子来说吧，假如 RPC 框架客户端想要远程调用某个目标方法，当客户端远程调用服务实例提供的方法失败时，RPC 客户端可以自动选择其他健康的服务实例，完成远程调用目标方法的操作，这就是故障转移功能最直接的体现。但现在我们开发的是 nacos 注册中心客户端，**注册中心客户端并不需要远程调用某个方法，注册中心客户端只需要访问服务端，从服务端订阅服务实例信息，然后把服务实例信息缓存到本地，那些集成了注册中心客户端的程序能够通过注册中心客户端获得这些服务实例信息就行**。这样一来，对于注册中心客户端来说，最重要的就是可以获得订阅到的服务实例信息。而客户端订阅到的服务实例信息都被 ServiceInfoHolder 组件持有。所以，**注册中心客户端的故障转移功能可以从这方面入手，那就是另外再增加一个故障转移组件，这个组件也缓存一份从服务端订阅到的服务实例信息，当无法从 ServiceInfoHolder 缓存的数据中得到服务实例信息时，这个时候就可以直接从故障转移组件中获取对应的信息**。这就是 nacos 注册中心客户端故障转移组件实现的基本思路。

我知道肯定有朋友会觉得之前实现 ServiceInfoHolder 组件的时候，不仅把服务实例信息缓存到了 ServiceInfoHolder 组件内部的 map 中，还对这些服务实例进行本地持久化。除此之外，还定义了 ServiceInfoUpdateService 服务实例更新组件，这个组件会定期从服务端查询最新的服务实例信息，然后更新到客户端本地，这么做难道还不够吗？怎么可能无法从 ServiceInfoHolder 组件中获得缓存的服务实例信息呢？真正的故障转移功能难道不应该从注册中心客户端和服务端建立连接的功能入手实现吗？简单来说就是 nacos 注册中心客户端发现自己和服务端的连接断开了，可以自动选择另外健康的服务器重建连接，这不也是故障转移功能的一种体现吗？是的，大家确实可以这么理解，并且我们也实现了这个功能。**当客户端发现服务器出现故障后，会自动选择健康的服务器重建连接**。这是前五章的知识了，不管怎么说吧，故障转移这个概念本身就比较宽泛，表现形式也多种多样，**但其本质无非就是发现某个功能无法使用了，程序可以立刻切换到备用的组件或者功能，或者是服务器，只要程序可以继续完美运行，数据没有发生紊乱即可**。好了，话题又扯远了，接下来就让我们回到正题，看看 nacos 注册中心客户端的故障转移组件该如何实现吧。

在 nacos 注册中心客户端中，把故障转移组件定义为了 FailoverReactor，也就是 FailoverReactor 类的对象就是故障转移组件。请大家想一想，FailoverReactor 作为故障转移组件，它应该提供什么功能？这都不用思考就能说出来，**FailoverReactor 肯定应该提供存储服务实例信息的功能，原因很简单，这个故障转移组件肯定是作为数据备份使用的，当用户无法从 ServiceInfoHolder 组件中获得缓存的服务实例信息时，就要直接从故障转移组件中获得**。这个想必是容易理解的吧？所以，我可以先把 FailoverReactor 类简单定义一下，之后再逐渐向里面填充更多内容。请看下面代码块。

```
package com.cqfy.nacos.client.naming.backups;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/25
 * @方法描述：故障转移服务组件，如果故障转移功能开启之后，客户端每次从本地获取订阅的服务信息，就要优先从故障转移组件中获取
 */
public class FailoverReactor implements Closeable {

    //故障转移组件存放服务实例信息的map
    //其中key就是服务实例的全键名，也就是serviceName、groupName、clusters组合起来的字符串
    //value就是对应的服务实例信息
    private Map<String, ServiceInfo> serviceMap = new ConcurrentHashMap<>();


    //根据key得到服务信息的方法
    public ServiceInfo getService(String key) {
        ServiceInfo serviceInfo = serviceMap.get(key);
        if (serviceInfo == null) {
            serviceInfo = new ServiceInfo();
            serviceInfo.setName(key);
        }
        return serviceInfo;
    }

    
}
```

上面代码块展示的 FailoverReactor 类就是我目前定义好的故障转移组件，类的内容非常简单，只定义了一个 map 用来缓存客户端从服务端订阅到的服务实例信息，并且定义了一个获得对应服务实例信息的方法。最基础的架子搭建完了，接下来就该继续向 FailoverReactor 类中填充内容了。我首先想到的就是，FailoverReactor 故障转移中的组件是怎么得到服务实例信息的，这个其实很容易就能想到，肯定是从 ServiceInfoHolder 组件中获得的，因为 nacos 注册中心客户端从服务端订阅的服务实例信息肯定是先存放到 ServiceInfoHolder 对象中。也就是说，**FailoverReactor 故障转移组件需要从 ServiceInfoHolder 服务实例缓存器中获得服务实例信息**。**如果我们把 ServiceInfoHolder 服务实例缓存器定义成 FailoverReactor 故障转移组件的一个成员变量，这么着的话，故障转移组件就可以直接从 ServiceInfoHolder 服务实例缓存器中得到缓存的服务实例信息了**。nacos 客户端源码就是这么做的。当然，具体的实现方式可能有点出乎大家意料。

实际上，**在 nacos 注册中心客户端源码中定义了一个定时任务，还在本地定义了一个故障转移文件，这个定时任务每天都会执行一次，每次执行都会把 ServiceInfoHolder 服务实例缓存器中的服务实例信息持久化到本地的故障转移文件中**。**与此同时 nacos 注册中心客户端还在 FailoverReactor 类中定义了另外一个定时任务，这个定时任务每隔 5 秒就会执行一次，执行的操作就是把本地故障转移文件中的服务实例信息加载到内存中，也就是 FailoverReactor 故障转移组件的 serviceMap 中**。当然，这个还不是 FailoverReactor 故障转移组件最终的实现方式，不过我们可以先缓一缓，看看以这种方式实现的故障转移组件的代码该如何编写。请看下面代码块。

```
package com.cqfy.nacos.client.naming.backups;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/25
 * @方法描述：故障转移服务组件，如果故障转移功能开启之后，客户端每次从本地获取订阅的服务信息，就要优先从故障转移组件中获取
 */
public class FailoverReactor implements Closeable {

    //故障转移组件存放服务实例信息的map
    //其中key就是服务实例的全键名，也就是serviceName、groupName、clusters组合起来的字符串
    //value就是对应的服务实例信息
    private Map<String, ServiceInfo> serviceMap = new ConcurrentHashMap<>();


    //所谓故障转移就是把客户端缓存的服务信息存储到当前故障转移组件中的serviceMap中，同时还会把服务信息存储到本地文件中，本地文件的目录
    //就是这个成员变量表示的故障转移文件目录
    private static final String FAILOVER_DIR = "/failover";


    //故障转移也是定时任务，这里就是定义了定时任务的执行时间，也就是一天执行一次
    private static final long DAY_PERIOD_MINUTES = 24 * 60;

    //故障转移文件的完整目录
    private final String failoverDir;

    //服务实例信息保存器，故障转移组件要定期去这个保存器中获得服务信息
    private final ServiceInfoHolder serviceInfoHolder;

    //定时任务执行器
    private final ScheduledExecutorService executorService;



    //构造方法
    public FailoverReactor(ServiceInfoHolder serviceInfoHolder, String cacheDir) {
        
        this.serviceInfoHolder = serviceInfoHolder;
        
        //得到故障转移文件的完整目录
        this.failoverDir = cacheDir + FAILOVER_DIR;
        
        //创建定时任务执行器
        this.executorService = new ScheduledThreadPoolExecutor(1, r -> {
            Thread thread = new Thread(r);
            thread.setDaemon(true);
            thread.setName("com.alibaba.nacos.naming.failover");
            return thread;
        });

        //初始化故障转移组件，其实就是向定时任务执行器中提交定时任务
        this.init();
    }



    //初始化故障转移组件的方法
    public void init() {
        //向定时任务执行器提交一个每5秒执行一次的方法
        executorService.scheduleWithFixedDelay(new FailoverFileReader(), 0L, 5000L, TimeUnit.MILLISECONDS);
        
        //把客户端缓存的服务信息定期写入到本地文件中，这个定时任务每天执行一次
        executorService.scheduleWithFixedDelay(new DiskFileWriter(), 30, DAY_PERIOD_MINUTES, TimeUnit.MINUTES);
        
        //这里提交一个定时任务，这个定时任务会在程序启动10秒后执行，主要是判断一下本地是否存在故障转移文件，如果存在就直接向本地文件写入数据
        //如果不存在就把故障转移的文件创建出来，然后再写入数据
        executorService.schedule(() -> {
            try {
                File cacheDir = new File(failoverDir);
                //在这里判断文件夹是否存在，不存在就创建出来
                if (!cacheDir.exists() && !cacheDir.mkdirs()) {
                    throw new IllegalStateException("failed to create cache dir: " + failoverDir);
                }
                File[] files = cacheDir.listFiles();
                if (files == null || files.length <= 0) {
                    //在这里写入数据，其实就是把serviceInfoHolder中的数据写入到本地文件中
                    new DiskFileWriter().run();
                }
            } catch (Throwable e) {
                NAMING_LOGGER.error("[NA] failed to backup file on startup.", e);
            }

        }, 10000L, TimeUnit.MILLISECONDS);
    }
    


    
    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/25
     * @方法描述：这个定时任务就是把serviceInfoHolder中的服务信息写入到故障转移文件中
     */
    class DiskFileWriter extends TimerTask {

        @Override
        public void run() {
            Map<String, ServiceInfo> map = serviceInfoHolder.getServiceInfoMap();
            for (Map.Entry<String, ServiceInfo> entry : map.entrySet()) {
                ServiceInfo serviceInfo = entry.getValue();
                //将数据写入到故障转移文件中
                DiskCache.write(serviceInfo, failoverDir);
            }
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/25
     * @方法描述：这个就是定期把故障转移文件中的内容加载到内存中的定时任务，这个方法的逻辑很简单，就不添加详细注释了，作用就是把故障转移文件中的内容加载到内存中，也就是当前类的serviceMap中
     */
    class FailoverFileReader implements Runnable {

        @Override
        public void run() {
            Map<String, ServiceInfo> domMap = new HashMap<>(16);
            BufferedReader reader = null;
            try {//得到故障转移文件目录
                File cacheDir = new File(failoverDir);
                //判断文件夹是否存在，不存在则创建
                if (!cacheDir.exists() && !cacheDir.mkdirs()) {
                    throw new IllegalStateException("failed to create cache dir: " + failoverDir);
                }//得到目录下的所有文件
                File[] files = cacheDir.listFiles();
                if (files == null) {
                    return;
                }//遍历每一个文件
                for (File file : files) {
                    if (!file.isFile()) {
                        continue;
                    }
                    ServiceInfo dom = null;
                    try {
                        dom = new ServiceInfo(URLDecoder.decode(file.getName(), StandardCharsets.UTF_8.name()));
                        String dataString = ConcurrentDiskUtil.getFileContent(file, Charset.defaultCharset().toString());
                        reader = new BufferedReader(new StringReader(dataString));
                        String json;//读取文件每一行内容
                        if ((json = reader.readLine()) != null) {
                            try {//得到ServiceInfo对象
                                dom = JacksonUtils.toObj(json, ServiceInfo.class);
                            } catch (Exception e) {
                                NAMING_LOGGER.error("[NA] error while parsing cached dom : {}", json, e);
                            }
                        }
                    } catch (Exception e) {
                        NAMING_LOGGER.error("[NA] failed to read cache for dom: {}", file.getName(), e);
                    } finally {
                        try {
                            if (reader != null) {
                                reader.close();
                            }
                        } catch (Exception e) {
                            //ignore
                        }
                    }//判断ServiceInfo对象是否不为空，也就是ServiceInfo对象中存储的服务实例信息不为空
                    if (dom != null && !CollectionUtils.isEmpty(dom.getHosts())) {
                        //把对象放到map中
                        domMap.put(dom.getKey(), dom);
                    }
                }
            } catch (Exception e) {
                NAMING_LOGGER.error("[NA] failed to read cache file", e);
            }
            if (domMap.size() > 0) {
                //在这里把map赋值给serviceMap
                serviceMap = domMap;
            }
        }
    }

    

    //根据key得到服务信息的方法
    public ServiceInfo getService(String key) {
        ServiceInfo serviceInfo = serviceMap.get(key);
        if (serviceInfo == null) {
            serviceInfo = new ServiceInfo();
            serviceInfo.setName(key);
        }
        return serviceInfo;
    }

    
}
```

从上面的代码块中可以看到，我把 ServiceInfoHolder 服务实例信息缓存器定义为 FailoverReactor 故障转移组组件的成员变量了，并且在 FailoverReactor 类中定义了两个定时任务，一个是 DiskFileWriter，这个定时任务专门负责把 ServiceInfoHolder 对象中缓存的服务实例信息持久化到本地的 failoverDir 故障转移文件中；另一个定时任务就是 FailoverFileReader，这个定时任务专门负责把故障转移文件中的服务实例信息定期加载到内存中，也就是存放到 FailoverReactor 类的 serviceMap 成员变量中。现在大家应该对 nacos 注册中心客户端的故障转移功能有一个大概的认知了，并且可能会觉得这个功能设计得有点鸡肋。我们先别管它设计得怎么样，还是先继续向下看吧。**实际上 nacos 注册中心客户端的故障转移组件还有一个额外的功能，那就是故障转移开关**。只有在开关开启的情况下，FailoverReactor 故障转移组件才会发挥它的作用，把故障转移文件中的服务实例信息加载到 FailoverReactor 类的 serviceMap 中。换句话说，**只有在故障转移开关开启的情况下，FailoverFileReader 定时任务才会被执行**。

**这个故障转移开关也是由一个本地文件控制的，我把它叫做故障转移开关文件，只要文件中的内容是 1，就意味着故障转移开关打开了，FailoverFileReader 任务就可以被执行；如果文件内容是 0，就意味着故障转移开关没有开启，FailoverFileReader 任务就不会被执行**。刚才我为大家实现 FailoverFileReader 类的时候，在 FailoverFileReader 类的 init 方法中执行就提交了 FailoverFileReader 定时任务，实际上在 nacos 注册中心客户端源码中，FailoverFileReader 类还定义了第三个任务，那就是 SwitchRefresher 定时任务，这个定时任务每 5 秒就执行一次，具体要执行的操作就是定时监听故障转移开关文件的内容有没有变更，如果文件内容是 1，就执行 FailoverFileReader 任务，这样一来其实就相当于 FailoverFileReader 任务每隔 5 秒执行一次；如果文件内容是 0，就不执行什么有意义的操作，并不会把故障转移文件中的服务实例信息加载到 FailoverReactor 类的 serviceMap 成员变量中。而在 FailoverFileReader 类的 init 方法中提交的并不是 FailoverFileReader 任务，而是新定义的 SwitchRefresher 任务。这样一来，FailoverReactor 类就要重构成下面这样了，请看下面代码块。

```
package com.cqfy.nacos.client.naming.backups;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/25
 * @方法描述：故障转移服务组件，如果故障转移功能开启之后，客户端每次从本地获取订阅的服务信息，就要优先从故障转移组件中获取
 */
public class FailoverReactor implements Closeable {

    //故障转移组件存放服务实例信息的map
    //其中key就是服务实例的全键名，也就是serviceName、groupName、clusters组合起来的字符串
    //value就是对应的服务实例信息
    private Map<String, ServiceInfo> serviceMap = new ConcurrentHashMap<>();


    //所谓故障转移就是把客户端缓存的服务信息存储到当前故障转移组件中的serviceMap中，同时还会把服务信息存储到本地文件中，本地文件的目录
    //就是这个成员变量表示的故障转移文件目录
    private static final String FAILOVER_DIR = "/failover";


    //故障转移也是定时任务，这里就是定义了定时任务的执行时间，也就是一天执行一次
    private static final long DAY_PERIOD_MINUTES = 24 * 60;

    //故障转移文件的完整目录
    private final String failoverDir;

    //服务实例信息保存器，故障转移组件要定期去这个保存器中获得服务信息
    private final ServiceInfoHolder serviceInfoHolder;

    //定时任务执行器
    private final ScheduledExecutorService executorService;


     //故障转移功能当前状态，这个表示开启状态
    private static final String IS_FAILOVER_MODE = "1";
    //这个代表关闭状态
    private static final String NO_FAILOVER_MODE = "0";

    //这个就是用来获得故障转移是否开启的key
    private static final String FAILOVER_MODE_PARAM = "failover-mode";

    //当前map存储的就是故障转移是否开启的信息
    //key就是FAILOVER_MODE_PARAM，表示故障转移功能目前状态，value就是对应的true或者false，表示是否开启
    private final Map<String, String> switchParams = new ConcurrentHashMap<>();



    //构造方法
    public FailoverReactor(ServiceInfoHolder serviceInfoHolder, String cacheDir) {
        
        this.serviceInfoHolder = serviceInfoHolder;
        
        //得到故障转移文件的完整目录
        this.failoverDir = cacheDir + FAILOVER_DIR;
        
        //创建定时任务执行器
        this.executorService = new ScheduledThreadPoolExecutor(1, r -> {
            Thread thread = new Thread(r);
            thread.setDaemon(true);
            thread.setName("com.alibaba.nacos.naming.failover");
            return thread;
        });

        //初始化故障转移组件，其实就是向定时任务执行器中提交定时任务
        this.init();
    }



    //初始化故障转移组件的方法，这个方法经过重构了，把FailoverFileReader任务替换成了SwitchRefresher任务
    public void init() {
        //向定时任务执行器提交一个每5秒执行一次的方法，这个方法主要关注故障转移组件的开关状态
        executorService.scheduleWithFixedDelay(new SwitchRefresher(), 0L, 5000L, TimeUnit.MILLISECONDS);
        
        //把客户端缓存的服务信息定期写入到本地文件中，这个定时任务每天执行一次
        executorService.scheduleWithFixedDelay(new DiskFileWriter(), 30, DAY_PERIOD_MINUTES, TimeUnit.MINUTES);
        
        //这里提交一个定时任务，这个定时任务会在程序启动10秒后执行，主要是判断一下本地是否存在故障转移文件，如果存在就直接向本地文件写入数据
        //如果不存在就把故障转移的文件创建出来，然后再写入数据
        executorService.schedule(() -> {
            try {
                File cacheDir = new File(failoverDir);
                //在这里判断文件夹是否存在，不存在就创建出来
                if (!cacheDir.exists() && !cacheDir.mkdirs()) {
                    throw new IllegalStateException("failed to create cache dir: " + failoverDir);
                }
                File[] files = cacheDir.listFiles();
                if (files == null || files.length <= 0) {
                    //在这里写入数据，其实就是把serviceInfoHolder中的数据写入到本地文件中
                    new DiskFileWriter().run();
                }
            } catch (Throwable e) {
                NAMING_LOGGER.error("[NA] failed to backup file on startup.", e);
            }

        }, 10000L, TimeUnit.MILLISECONDS);
    }




     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/4/25
     * @方法描述：这个类的对象是一个定时任务，这个定时任务就是用来定期检查服务组件开关状态的，其实是这样的，故障转移组件的开关是靠一个本地文件的内容来确定的，就是Paths.get(failoverDir,UtilAndComs.FAILOVER_SWITCH)这个路径的文件
     * 只要部署nacos客户端的服务器本地存在这个文件，并且文件中有开启标志，那么就意味着开启了故障转移功能，如果这个文件不存在，那故障转移功能就没开启，这个文件的存在与否可以由程序员来控制，文件内容也可以由程序员控制
     */
    class SwitchRefresher implements Runnable {
        //故障转移功能开关文件最新修改时间
        long lastModifiedMillis = 0L;
        @Override
        public void run() {
            try {
                //得到故障转移开关文件
                File switchFile = Paths.get(failoverDir, UtilAndComs.FAILOVER_SWITCH).toFile();
                //判断文件是否存在
                if (!switchFile.exists()) {
                    //如果文件不存在则意味着没有开启故障转移功能，直接在map中设置故障转移功能状态为false，也就是关闭即可
                    switchParams.put(FAILOVER_MODE_PARAM, Boolean.FALSE.toString());
                    NAMING_LOGGER.debug("failover switch is not found, {}", switchFile.getName());
                    return;
                }
                //得到文件最新修改时间
                long modified = switchFile.lastModified();
                //判断文件是否更新了
                if (lastModifiedMillis < modified) {
                    //如果更新了则给最后更新时间按赋值
                    lastModifiedMillis = modified;
                    //读取文件内容
                    String failover = ConcurrentDiskUtil.getFileContent(switchFile.getPath(),
                            Charset.defaultCharset().toString());
                    //接下来就是判断文件是否有内容，如果有内容就判断文件中是否有故障转移功能开启标志
                    if (!StringUtils.isEmpty(failover)) {
                        String[] lines = failover.split(DiskCache.getLineSeparator());
                        for (String line : lines) {
                            //去掉空格
                            String line1 = line.trim();
                            //判断这一行的内容是不是1，如果是1就意味着开启了故障转移功能
                            if (IS_FAILOVER_MODE.equals(line1)) {
                                //把故障转移功能的开关设置为开启状态
                                switchParams.put(FAILOVER_MODE_PARAM, Boolean.TRUE.toString());
                                NAMING_LOGGER.info("failover-mode is on");
                                //在这里把故障转移文件中的内容读取到内存中，也就是serviceMap中
                                //注意，当前任务是一个定时任务，每5秒执行一次，这也就意味着每5秒就会把故障转移文件中的数据加载到内存中一次
                                new FailoverFileReader().run();
                            } else if (NO_FAILOVER_MODE.equals(line1)) {
                                //如果文件中的内容为0，也就意味着没有开启故障转移文件，那就把故障转移功能的开关设置为false
                                switchParams.put(FAILOVER_MODE_PARAM, Boolean.FALSE.toString());
                                NAMING_LOGGER.info("failover-mode is off");
                            }
                        }
                    } else {
                        //走到这里意味着文件没有内容，那么也把故障转移功能开关设置为false
                        switchParams.put(FAILOVER_MODE_PARAM, Boolean.FALSE.toString());
                    }
                }
            } catch (Throwable e) {
                NAMING_LOGGER.error("[NA] failed to read failover switch.", e);
            }
        }
    }



    //判断故障转移功能是否开启的方法
    public boolean isFailoverSwitch() {
        return Boolean.parseBoolean(switchParams.get(FAILOVER_MODE_PARAM));
    }

    //根据key得到服务信息的方法
    public ServiceInfo getService(String key) {
        ServiceInfo serviceInfo = serviceMap.get(key);
        if (serviceInfo == null) {
            serviceInfo = new ServiceInfo();
            serviceInfo.setName(key);
        }
        return serviceInfo;
    }

    
    //其他内容省略


}
    
```

好了，现在完整的故障转移组件我已经给大家实现完毕了，那么这个 FailoverReactor 故障转移组件的对象究竟要在那里被创建呢？反正它只要一被创建就会在构造方法中调用 init 初始化方法，然后向定时任务执行器提交定时任务。那么它的创建时机是什么呢？这个也没什么好讲解的，FailoverReactor 类持有了 ServiceInfoHolder 对象，在 FailoverReactor 类的构造方法中把 ServiceInfoHolder 服务实例信息缓存器传输进来了。所以，**在 nacos 源码中，干脆就让这个 FailoverReactor 类的对象在 ServiceInfoHolder 类中创建了**。具体实现请看下面代码块。

```
package com.cqfy.nacos.client.naming.cache;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/24
 * @方法描述：该类的对象就是客户端的服务信息保存器，从服务端订阅的所有服务实例的信息都会存放在这个类的对象中
 */
public class ServiceInfoHolder implements Closeable {


    /**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/24
 * @方法描述：该类的对象就是客户端的服务信息保存器，从服务端订阅的所有服务实例的信息都会存放在这个类的对象中
 */
public class ServiceInfoHolder implements Closeable {

   

    //这个map中缓存了从服务端获得的服务实例的信息，把服务实例缓存在内存中，同时也会存储在本地硬盘上
    //string就是服务名称，ServiceInfo封装着该服务下所有服务实例信息
    private final ConcurrentMap<String, ServiceInfo> serviceInfoMap;

    //最终将服务实例信息存放到本地硬盘上的文件路径
    private String cacheDir;
    
    //故障转移器
    private final FailoverReactor failoverReactor;

    //构造方法
    public ServiceInfoHolder(String namespace, String notifierEventScope, NacosClientProperties properties) {
        //初始化本地存储文件的目录
        initCacheDir(namespace, properties);
        
        //创建故障转移器
        this.failoverReactor = new FailoverReactor(this, cacheDir);
       //省略其他内容
    }




    //根据服务信息得到对应的服务实例信息的方法
    public ServiceInfo getServiceInfo(final String serviceName, final String groupName, final String clusters) {
        NAMING_LOGGER.debug("failover-mode: {}", failoverReactor.isFailoverSwitch());
        
        //得到服务组合名称
        String groupedServiceName = NamingUtils.getGroupedName(serviceName, groupName);
        
        //得到服务完整名称生成的key
        String key = ServiceInfo.getKey(groupedServiceName, clusters);
        
        //判断是否开启了故障转移模式
        if (failoverReactor.isFailoverSwitch()) {
            //如果开启了就优先从故障转移器中获得缓存的服务实例信息
            return failoverReactor.getService(key);
        }
        
        //如果没开启才从客户端缓存中获取
        return serviceInfoMap.get(key);
    }
    


    //省略其他内容
}
```

到此为止，这个故障转移功能我就为大家实现完毕了，并且通过上面代码块的 getServiceInfo 方法，可以看到，**假如 nacos 注册中心客户端开启了故障转移功能，无非就是获得服务实例信息的时候，首先从故障转移组件中获得**。说实话，我还没参透这个故障转移组件这样设计的机制，所以也无法给大家讲得特别详细，大家可以先看看我提供的代码，现在已经可以阅读我提供的第六版本代码了，在思考思考。我就不再这个故障转移功能浪费更多时间了，因为接下来还有很多服务端的功能等待我实现，所以这个故障转移功能就到此为止吧。

## 实现客户端注销服务实例功能

从文章一开始我就跟大家说了，客户端注销服务实例功能实现起来非常简单，因为在客户端这边只是发送一个注销服务实例的请求给服务端即可，核心操作都在服务端那边，等实现服务端功能的时候大家就清楚了。在客户端这边实现服务实例注销功能，无非就是在 NamingGrpcClientProxy 类中定义 deregisterService 注销服务实例的方法。当然，执行注销服务实例操作的时候，肯定是从最外层的 NacosNamingService 对象中一路调用 deregisterService 方法，直到调用到 NamingGrpcClientProxy 类中，把注销服务实例的请求发送给服务端即可。所以在 NacosNamingService 类和 NamingClientProxyDelegate 类中也要实现对应的 deregisterService 方法，但这些代码我就不在文章中展示了。太简单的内容就留给大家直接去第六版本代码中查看吧。接下来我给大家展示一下在 NamingGrpcClientProxy 类中，deregisterService 方法是如何实现的，请看下面代码块。

```
package com.cqfy.nacos.client.naming.remote.grpc;



public class NamingGrpcClientProxy extends AbstractNamingClientProxy {


    //注销服务实例的方法
    @Override
    public void deregisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        NAMING_LOGGER.info("[DEREGISTER-SERVICE] {} deregistering service {} with instance: {}", namespaceId,
                serviceName, instance);
        if (instance.isEphemeral()) {
            //注销临时实例
            deregisterServiceForEphemeral(serviceName, groupName, instance);
        } else {
            //注销持久实例，这个方法暂时不实现，后面更新到服务端时再展开
            //doDeregisterServiceForPersistent(serviceName, groupName, instance);
        }
    }


    //注销临时服务实例的方法
    private void deregisterServiceForEphemeral(String serviceName, String groupName, Instance instance) throws NacosException {
        String key = NamingUtils.getGroupedName(serviceName, groupName);
        InstanceRedoData instanceRedoData = redoService.getRegisteredInstancesByKey(key);
        if (instanceRedoData instanceof BatchInstanceRedoData) {
            List<Instance> instances = new ArrayList<>();
            if (null != instance) {
                instances.add(instance);
            }//批量注销的方法
            //batchDeregisterService(serviceName, groupName, instances);
        } else {
            redoService.instanceDeregister(serviceName, groupName);
            //注销某个服务的方法
            doDeregisterService(serviceName, groupName, instance);
        }
    }


    //真正注销服务实例的方法
    public void doDeregisterService(String serviceName, String groupName, Instance instance) throws NacosException {
        //这里虽然使用的还是InstanceRequest请求对象，但是在请求对象中设置了deregisterInstance标志
        //NamingRemoteConstants.DE_REGISTER_INSTANCE常量就是"deregisterInstance"注销服务实例字符串
        InstanceRequest request = new InstanceRequest(namespaceId, serviceName, groupName,
                NamingRemoteConstants.DE_REGISTER_INSTANCE, instance);
        //在这里把请求发送给服务端
        requestToServer(request, Response.class);
        redoService.instanceDeregistered(serviceName, groupName);
    }


    //其他内容省略
}
```

从上面的代码快可以看到，为客户端实现的服务实例信息注销功能最终其实就是向服务端发送了一个 InstanceRequest 请求，InstanceRequest 请求我们已经见过了，在客户端向服务端注册服务实例信息的时候会把服务实例信息封装到 InstanceRequest 对象中，发送给服务端。**注销服务实例之所以还使用 InstanceRequest 对象，是因为 InstanceRequest 对象中有一个 String 类型的 type 成员变量，给这个 type 用 "registerInstance" 字符串赋值，经过服务端解析后，服务端就会知道这个 InstanceRequest 对象代表着服务实例注册请求；如果用 "deregisterInstance" 字符串给 type 成员变量赋值，服务端就会知道这个对象代表服务实例信息注销请求。然后服务端就会执行服务实例注销操作**。InstanceRequest 类的具体内容如下，请看下面代码块。

```
package com.cqfy.nacos.api.naming.remote.request;

import com.cqfy.nacos.api.naming.pojo.Instance;

/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/4/7
 * @方法描述：当客户端向服务器注册实例的时候，会发送InstanceRequest对象作为请求
 */
public class InstanceRequest extends AbstractNamingRequest {

    //请求类型
    private String type;

    private Instance instance;

    public InstanceRequest() {
    }

    public InstanceRequest(String namespace, String serviceName, String groupName, String type, Instance instance) {
        super(namespace, serviceName, groupName);
        this.type = type;
        this.instance = instance;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getType() {
        return this.type;
    }

    public void setInstance(Instance instance) {
        this.instance = instance;
    }

    public Instance getInstance() {
        return instance;
    }
}
```

到此为止，客户端注销服务实例信息的功能就已经实现完整了。

## 实现客户端取消订阅服务实例功能

  

客户端取消订阅服务实例信息的功能和注销服务实例信息的功能类似，只不过就是在 NamingGrpcClientProxy 类中定义取消订阅服务实例信息的 unsubscribe 方法。当然，**客户端能做的无非是向服务端发送一个 SubscribeServiceRequest 请求，把 SubscribeServiceRequest 请求对象中的是否订阅服务实例信息的标志设置为 fasle，这就代表该请求要执行的是取消服务实例信息订阅的操作**。最终核心逻辑还是在服务端那边完成，这个后面再实现。接下来，就请大家看一下我已经在 NamingGrpcClientProxy 类中实现完毕的 unsubscribe 方法，请看下面代码快。

```
package com.cqfy.nacos.client.naming.remote.grpc;



public class NamingGrpcClientProxy extends AbstractNamingClientProxy {


     //取消订阅的方法
    @Override
    public void unsubscribe(String serviceName, String groupName, String clusters) throws NacosException {
        if (NAMING_LOGGER.isDebugEnabled()) {
            NAMING_LOGGER.debug("[GRPC-UNSUBSCRIBE] service:{}, group:{}, cluster:{} ", serviceName, groupName,
                    clusters);
        }
        redoService.subscriberDeregister(serviceName, groupName, clusters);
        doUnsubscribe(serviceName, groupName, clusters);
    }


    //真正取消订阅的方法
    public void doUnsubscribe(String serviceName, String groupName, String clusters) throws NacosException {
        //创建SubscribeServiceRequest请求，把订阅标志设置为false，代表要取消订阅
        SubscribeServiceRequest request = new SubscribeServiceRequest(namespaceId, groupName, serviceName, clusters,false);
        //在这里把请求发送给服务端
        requestToServer(request, SubscribeServiceResponse.class);
        redoService.removeSubscriberForRedo(serviceName, groupName, clusters);
    }


    //其他内容省略
}
```

SubscribeServiceRequest 类的内容如下所示，请看下面代码块。

```
package com.cqfy.nacos.api.naming.remote.request;


public class SubscribeServiceRequest extends AbstractNamingRequest {

    //订阅或者取消订阅的标志
    private boolean subscribe;

    private String clusters;

    public SubscribeServiceRequest() {
    }

    public SubscribeServiceRequest(String namespace, String groupName, String serviceName, String clusters,
                                   boolean subscribe) {
        super(namespace, serviceName, groupName);
        this.clusters = clusters;
        this.subscribe = subscribe;
    }

    public String getClusters() {
        return clusters;
    }

    public void setClusters(String clusters) {
        this.clusters = clusters;
    }

    public boolean isSubscribe() {
        return subscribe;
    }

    public void setSubscribe(boolean subscribe) {
        this.subscribe = subscribe;
    }
}
```

到此为止，nacos 注册中心客户端的基本功能我就为大家实现完毕了，接下来就要开始实现服务端的功能了，内容会非常多，但大家也没必要恐慌，因为内容都非常简单，只不过就是代码多一点而已。好了，现在大家也可以去查看第六版本代码了，这个版本代码就当是一个休息站吧，内容太简单了，等大家准备好后，就可以接着往下看服务端的章节了。朋友们，我们下一章见！