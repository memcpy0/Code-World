这一章，我将为大家实现 Tomcat、Dubbo 这些框架内部线程池的配置变更与信息收集。虽然要实现的是和 tomcat、dubbo 这些框架有关的功能，但是我想跟大家说，这些其实也都是老功能，旧知识。不管要收集的是什么框架的线程池配置信息，不管要动态变更的是什么框架的线程池，其实只要得到了这个框架的线程池，其他的一切都好说。接下来，我就先为大家实现 tomcat 容器线程池的配置信息的收集与动态变更。

## 实现 tomcat 容器线程池的配置信息的收集与变更

要想实现 tomcat 容器线程池配置信息的收集，首先应该得到 tomcat 线程池。那怎么得到 tomcat 线程池呢？这里我就不卖关子了，就直接开始表演了。因为这些功能之前都实现过了，实现思路和程序执行流程大家也都清楚了，所以我就梳理一下要引入的新的类，展示展示代码。就不再想本套课程前七章剖析得那么详细了。这时候写得越多就越啰嗦。好了，**接下来我就先为客户端引入两个新的类，一个叫做 TomcatWebThreadPoolHandler。这个类就是用来收集 tomcat 线程池的配置信息，以及动态刷新 tomcat 线程池的配置信息的。当然，这个类的对象也可以用来获得 tomcat 的内部线程池**。**另一个叫做 AbstractWebThreadPoolService，TomcatWebThreadPoolHandler 是 AbstractWebThreadPoolService 的子类。**具体实现请看下面代码块。

首先是 AbstractWebThreadPoolService 类。

```
package cn.hippo4j.adapter.web;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/5/9
 * @方法描述：web容器线程池处理器的抽象父类，这个父类实现了springboot的ApplicationRunner接口，springboot容器启动完毕之后，这个接口中的方法才会被回调
 */
@Slf4j
public abstract class AbstractWebThreadPoolService implements WebThreadPoolService, ApplicationRunner {

    //tomcat容器线程池
    protected volatile Executor executor;

    /**
     * Get web thread pool by server
     *
     * @param webServer
     * @return
     */
    protected abstract Executor getWebThreadPoolByServer(WebServer webServer);


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/5/9
     * @方法描述：得到web容器线程池的方法
     */
    @Override
    public Executor getWebThreadPool() {
        if (executor == null) {
            synchronized (AbstractWebThreadPoolService.class) {
                if (executor == null) {
                    //调用子类的方法得到web容器线程池
                    executor = getWebThreadPoolByServer(getWebServer());
                }
            }
        }
        return executor;
    }


    //得到web线程池的方法
    @Override
    public WebServer getWebServer() {
        ApplicationContext applicationContext = ApplicationContextHolder.getInstance();
        WebServer webServer = ((WebServerApplicationContext) applicationContext).getWebServer();
        return webServer;
    }


    //在这里回调了ApplicationRunner接口中的方法
    @Override
    public void run(ApplicationArguments args) {
        try {//在这里得到了web容器的线程池
            getWebThreadPool();
        } catch (Exception ignored) {
        }
    }
}
```

接下来是 TomcatWebThreadPoolHandler 类。

```
package cn.hippo4j.adapter.web;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/5/9
 * @方法描述：tomcat线程池处理器，这个处理器主要负责收集tomcat线程池的信息，以及动态刷新tomcat线程池的配置信息
 */
@Slf4j
@RequiredArgsConstructor
public class TomcatWebThreadPoolHandler extends AbstractWebThreadPoolService {


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/5/9
     * @方法描述：得到tomcat线程池的方法
     */
    @Override
    protected Executor getWebThreadPoolByServer(WebServer webServer) {
        if (cacheFlag.get()) {
            log.warn("Exception getting Tomcat thread pool. Exception message: {}", EXCEPTION_MESSAGE);
            return null;
        }
        Executor tomcatExecutor = null;
        try {
            tomcatExecutor = ((TomcatWebServer) webServer).getTomcat().getConnector().getProtocolHandler().getExecutor();
        } catch (Exception ex) {
            cacheFlag.set(Boolean.TRUE);
            EXCEPTION_MESSAGE = ex.getMessage();
            log.error("Failed to get Tomcat thread pool. Message: {}", EXCEPTION_MESSAGE);
        }
        return tomcatExecutor;
    }



    //其余方法暂时不做实现
}
```

可以看到，TomcatWebThreadPoolHandler 是 AbstractWebThreadPoolService 的子类，而 AbstractWebThreadPoolService 又实现了 SpringBoot 的 ApplicationRunner 扩展接口。当 AbstractWebThreadPoolService 实现的 ApplicationRunner 接口的 run 方法被回调的时候，就会调用 AbstractWebThreadPoolService 类自己的 getWebThreadPool 方法，接着会调用子类 TomcatWebThreadPoolHandler 的 getWebThreadPoolByServer 方法，最后就得到了 tomcat 容器的 web 线程池，大家可以结合代码品味一下这个逻辑。当然，既然 SpringBoot 的 ApplicationRunner 扩展接口要发挥作用，这就意味着 TomcatWebThreadPoolHandler 类的对象应该交给 SpringBoot 的容器来管理。所以我又引入了一个 SpringBoot 容器配置类，就是 WebAdapterConfiguration，把 TomcatWebThreadPoolHandler 类的对象定义在这个配置类中了。具体实现请看下面代码块。

```
package cn.hippo4j.adapter.web;



@Configuration
@RequiredArgsConstructor
public class WebAdapterConfiguration {

    @Bean
    public TomcatWebThreadPoolHandler tomcatWebThreadPoolHandler(WebThreadPoolRunStateHandler webThreadPoolRunStateHandler) {
        return new TomcatWebThreadPoolHandler(webThreadPoolRunStateHandler);
    }
}
```

现在，我们已经可以得到 tomcat 的线程池了。接下来就可以直接收集这个线程池的配置信息，然后上报给服务端，服务端再展示给前端用户了。本来逻辑应该是这样的，但是在 hippo4j 框架中并不是这么做的，实际上在这个框架中，作者并没有把 web 容器线程池的配置信息注册到服务端，而是采用了和上一章一样的思路。当用户想在前端页面获得 tomcat 线程池的配置信息时，就需要在前端点击容器线程池页面，就像下面这样。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717418862934-b798bbf3-0821-4cb9-8378-d5e25f5d5dec.png)

如果想要查看 tomcat 容器线程池，就可以点击容器线程池下的 Tomcat，然后就能得到 Tomcat 线程池的配置信息了，就像下面这样。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717419043822-a683a6e8-a0d7-47d6-aec5-51b02fe09580.png)

现在大家看到的是我已经实现的功能，但是这个功能究竟是怎么实现的，前后端是怎么交互的，接下来就让我为大家详细解释一下。实际上，当用户想要查看某个部署了客户端的 web 容器中的线程池配置信息时，就会向服务端发送一个请求，这个请求会被服务端的 ThreadPoolController 控制器接收。讲到这里，大家是不是觉得很熟悉？因为在上一章，我刚为大家引入了这个 ThreadPoolController 控制器对象。在上一章这个控制器对象中定义了两个方法，一个是 runState，另一个是 runThreadState，这两个方法是用来和动态线程池打交道的**。在这一章我们要和 Tomcat 容器线程池打交道了，所以要定义三个新的方法，一个是 getPoolBaseState 方法，一个是 getPoolRunState 方法，还有一个就是 updateWebThreadPool 方法**。这三个方法的具体作用我已经为大家展示在下面的代码块中了，请看下面代码块。

```
package cn.hippo4j.console.controller;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/4/29
 * @Description:这个控制器就是第一版本最核心的一个控制器
 */
@RestController
@RequiredArgsConstructor
@RequestMapping(Constants.BASE_PATH + "/thread/pool")
public class ThreadPoolController {


    //线程池服务类，这个服务类中封装了去数据库中查询线程池信息的操作
    private final ThreadPoolService threadPoolService;

    //服务发现组件
    private final BaseInstanceRegistry baseInstanceRegistry;



    //web界面web容器线程池配置信息
    @GetMapping("/web/base/info")
    public Result getPoolBaseState(@RequestParam(value = "mark") String mark,
                                   @RequestParam(value = "clientAddress") String clientAddress) {
        String urlString = StringUtil.newBuilder(HTTP, clientAddress, "/web/base/info", "?mark=", mark);
        return HttpUtil.get(urlString, Result.class);
    }

    //web界面web容器线程池实时运行时信息
    @GetMapping("/web/run/state")
    public Result getPoolRunState(@RequestParam(value = "clientAddress") String clientAddress) {
        String urlString = StringUtil.newBuilder(HTTP, clientAddress, "/web/run/state");
        return HttpUtil.get(urlString, Result.class);
    }

    //web界面web容器线程池更新处理方法
    @PostMapping("/web/update/pool")
    public Result<Void> updateWebThreadPool(@RequestBody WebThreadPoolReqDTO requestParam) {
        for (String each : requestParam.getClientAddressList()) {
            String urlString = StringUtil.newBuilder(HTTP, each, "/web/update/pool");
            HttpUtil.post(urlString, requestParam);
        }
        return Results.success();
    }

    //其他方法省略
}
```

可以看到，**这三个方法分别是用来获得 web 容器线程池的配置信息、以及获得 web 容器线程池运行时信息、还有就是动态修改 web 容器线程池信息的**。**并且都是前端把亲求发送给服务器，服务器再去访问客户端，然后客户端返回相应的信息，或者是刷新 web 容器线程池配置信息**。这就是 web 容器线程池和服务端打交道的方式。

好了，这个核心知识已经讲解完毕了，接下来我们还是回到刚才要实现的功能上，那就是在前端展示给用户 web 容器线程池的配置信息。当用户在前端想查看 web 容器线程池的配置信息时，就会发送请求给服务器，这个请求会被服务器 ThreadPoolController 控制器对象的 getPoolBaseState 方法接收到。在这个方法内，服务端会主动访问客户端，让客户端收集 web 容器线程池的配置信息，然后再返回给服务端，服务端再把这些信息展示给前端用户。

那么，服务端主动访问客户端，想让客户端收集 web 容器线程池的配置信息时，客户端要怎么接收服务器发送过来的请求呢？关于这个功能，我已经给客户端定义好了一个新的类，就是 WebThreadPoolController 类。这个类的对象是一个控制器，具体实现如下，请看下面代码块。

```
package cn.hippo4j.springboot.start.controller;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/5/9
 * @方法描述：这个控制器是专门用来和服务端的ThreadPoolController类发出的请求打交道的
 * 当用户需要在web界面得到web容器线程池的信息，服务端就会访问客户端，客户端使用当前端控制器对象收集web容器线程池信息
 * 然后返回给服务器，服务器再返回前端展示给用户
 */
@CrossOrigin
@RestController
@AllArgsConstructor
public class WebThreadPoolController {

    //这个选择器的代码我就不给大家展示了，其实这个选择器定义得也很鸡肋，就是用来从SpringBoot容器中获得我刚才为客户端引入的
    //TomcatWebThreadPoolHandler类，然后调用TomcatWebThreadPoolHandler的方法收集web容器线程池配置信息、运行时信息，以及动态更新web容器线程池
    private final WebThreadPoolHandlerChoose webThreadPoolServiceChoose;

    //得到web容器线程池的运行信息
    @GetMapping("/web/base/info")
    public Result<ThreadPoolBaseInfo> getPoolBaseState(@RequestParam(value = "mark") String mark) {
        WebThreadPoolService webThreadPoolService = webThreadPoolServiceChoose.choose();
        if (webThreadPoolService != null && webThreadPoolService.getClass().getSimpleName().contains(mark)) {
            //因为TomcatWebThreadPoolHandler类实现了WebThreadPoolService接口，所以这调用的是接口中的simpleInfo方法
            return Results.success(webThreadPoolService.simpleInfo());
        }
        return Results.success(null);
    }

    //得到web容器线程池的运行状态
    @GetMapping("/web/run/state")
    public Result<ThreadPoolRunStateInfo> getPoolRunState() {
        ThreadPoolRunStateInfo result = webThreadPoolServiceChoose.choose().getWebRunStateInfo();
        return Results.success(result);
    }

    //动态更新web容器线程池的方法
    @PostMapping("/web/update/pool")
    public Result<Void> updateWebThreadPool(@RequestBody ThreadPoolParameterInfo threadPoolParameterInfo) {
        webThreadPoolServiceChoose.choose().updateWebThreadPool(threadPoolParameterInfo);
        return Results.success();
    }
}
```

在上面的代码块中可以看到，服务端发送给客户端的收集 web 容器线程池配置信息的请求会被 WebThreadPoolController 控制器的 getPoolBaseState 方法接收并处理。在 getPoolBaseState 方法中，就会调用 TomcatWebThreadPoolHandler 类的 simpleInfo 方法得到 web 容器线程池的配置信息。所以现在我也可以为大家展示一下重构之后的 TomcatWebThreadPoolHandler 类了。请看下面代码块。

```
package cn.hippo4j.adapter.web;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/5/9
 * @方法描述：tomcat线程池处理器，这个处理器主要负责收集tomcat线程池的信息，以及动态刷新tomcat线程池的配置信息
 */
@Slf4j
@RequiredArgsConstructor
public class TomcatWebThreadPoolHandler extends AbstractWebThreadPoolService {


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/5/9
     * @方法描述：得到tomcat线程池的方法
     */
    @Override
    protected Executor getWebThreadPoolByServer(WebServer webServer) {
        if (cacheFlag.get()) {
            log.warn("Exception getting Tomcat thread pool. Exception message: {}", EXCEPTION_MESSAGE);
            return null;
        }
        Executor tomcatExecutor = null;
        try {
            tomcatExecutor = ((TomcatWebServer) webServer).getTomcat().getConnector().getProtocolHandler().getExecutor();
        } catch (Exception ex) {
            cacheFlag.set(Boolean.TRUE);
            EXCEPTION_MESSAGE = ex.getMessage();
            log.error("Failed to get Tomcat thread pool. Message: {}", EXCEPTION_MESSAGE);
        }
        return tomcatExecutor;
    }


    //得到web容器线程池配置信息的方法
    @Override
    public ThreadPoolBaseInfo simpleInfo() {
        ThreadPoolBaseInfo poolBaseInfo = new ThreadPoolBaseInfo();
        org.apache.tomcat.util.threads.ThreadPoolExecutor tomcatThreadPoolExecutor = (org.apache.tomcat.util.threads.ThreadPoolExecutor) executor;
        int corePoolSize = tomcatThreadPoolExecutor.getCorePoolSize();
        int maximumPoolSize = tomcatThreadPoolExecutor.getMaximumPoolSize();
        long keepAliveTime = tomcatThreadPoolExecutor.getKeepAliveTime(TimeUnit.SECONDS);
        BlockingQueue<?> blockingQueue = tomcatThreadPoolExecutor.getQueue();
        int queueSize = blockingQueue.size();
        int remainingCapacity = blockingQueue.remainingCapacity();
        int queueCapacity = queueSize + remainingCapacity;
        String rejectedExecutionHandlerName = executor instanceof ThreadPoolExecutor ? ((ThreadPoolExecutor) executor).getRejectedExecutionHandler().getClass().getSimpleName()
                : tomcatThreadPoolExecutor.getRejectedExecutionHandler().getClass().getSimpleName();
        poolBaseInfo.setCoreSize(corePoolSize);
        poolBaseInfo.setMaximumSize(maximumPoolSize);
        poolBaseInfo.setKeepAliveTime(keepAliveTime);
        poolBaseInfo.setQueueType(blockingQueue.getClass().getSimpleName());
        poolBaseInfo.setQueueCapacity(queueCapacity);
        poolBaseInfo.setRejectedName(rejectedExecutionHandlerName);
        return poolBaseInfo;
    }

    
    //得到web容器线程池实时运行时信息的方法
    @Override
    public ThreadPoolRunStateInfo getWebRunStateInfo() {
        if (executor instanceof ThreadPoolExecutor) {
            return webThreadPoolRunStateHandler.getPoolRunState(null, executor);
        }
        ThreadPoolRunStateInfo runStateInfo = new ThreadPoolRunStateInfo();
        org.apache.tomcat.util.threads.ThreadPoolExecutor tomcatThreadPoolExecutor = (org.apache.tomcat.util.threads.ThreadPoolExecutor) executor;
        int corePoolSize = tomcatThreadPoolExecutor.getCorePoolSize();
        int maximumPoolSize = tomcatThreadPoolExecutor.getMaximumPoolSize();
        int poolSize = tomcatThreadPoolExecutor.getPoolSize();
        int activeCount = tomcatThreadPoolExecutor.getActiveCount();
        int largestPoolSize = tomcatThreadPoolExecutor.getLargestPoolSize();
        long completedTaskCount = tomcatThreadPoolExecutor.getCompletedTaskCount();
        String currentLoad = CalculateUtil.divide(activeCount, maximumPoolSize) + "";
        String peakLoad = CalculateUtil.divide(largestPoolSize, maximumPoolSize) + "";
        BlockingQueue<Runnable> queue = tomcatThreadPoolExecutor.getQueue();
        int queueSize = queue.size();
        String queueType = queue.getClass().getSimpleName();
        int remainingCapacity = queue.remainingCapacity();
        int queueCapacity = queueSize + remainingCapacity;
        runStateInfo.setCoreSize(corePoolSize);
        runStateInfo.setPoolSize(poolSize);
        runStateInfo.setMaximumSize(maximumPoolSize);
        runStateInfo.setActiveSize(activeCount);
        runStateInfo.setCurrentLoad(currentLoad);
        runStateInfo.setPeakLoad(peakLoad);
        runStateInfo.setQueueType(queueType);
        runStateInfo.setQueueSize(queueSize);
        runStateInfo.setQueueCapacity(queueCapacity);
        runStateInfo.setQueueRemainingCapacity(remainingCapacity);
        runStateInfo.setLargestPoolSize(largestPoolSize);
        runStateInfo.setCompletedTaskCount(completedTaskCount);
        runStateInfo.setClientLastRefreshTime(LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")));
        runStateInfo.setTimestamp(System.currentTimeMillis());
        String rejectedExecutionHandlerName = executor instanceof ThreadPoolExecutor ? ((ThreadPoolExecutor) executor).getRejectedExecutionHandler().getClass().getSimpleName()
                : tomcatThreadPoolExecutor.getRejectedExecutionHandler().getClass().getSimpleName();
        runStateInfo.setRejectedName(rejectedExecutionHandlerName);
        return webThreadPoolRunStateHandler.supplement(runStateInfo);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/5/9
     * @方法描述：动态更新tomcat线程池配置信息的方法
     */
    @Override
    public void updateWebThreadPool(ThreadPoolParameterInfo threadPoolParameterInfo) {
        try {
            org.apache.tomcat.util.threads.ThreadPoolExecutor tomcatThreadPoolExecutor = (org.apache.tomcat.util.threads.ThreadPoolExecutor) executor;
            int originalCoreSize = tomcatThreadPoolExecutor.getCorePoolSize();
            int originalMaximumPoolSize = tomcatThreadPoolExecutor.getMaximumPoolSize();
            long originalKeepAliveTime = tomcatThreadPoolExecutor.getKeepAliveTime(TimeUnit.SECONDS);
            tomcatThreadPoolExecutor.setCorePoolSize(threadPoolParameterInfo.corePoolSizeAdapt());
            tomcatThreadPoolExecutor.setMaximumPoolSize(threadPoolParameterInfo.maximumPoolSizeAdapt());
            tomcatThreadPoolExecutor.setKeepAliveTime(threadPoolParameterInfo.getKeepAliveTime(), TimeUnit.SECONDS);
            log.info("[Tomcat] Changed web thread pool. corePoolSize: {}, maximumPoolSize: {}, keepAliveTime: {}",
                    String.format(ChangeThreadPoolConstants.CHANGE_DELIMITER, originalCoreSize, threadPoolParameterInfo.corePoolSizeAdapt()),
                    String.format(ChangeThreadPoolConstants.CHANGE_DELIMITER, originalMaximumPoolSize, threadPoolParameterInfo.maximumPoolSizeAdapt()),
                    String.format(ChangeThreadPoolConstants.CHANGE_DELIMITER, originalKeepAliveTime, threadPoolParameterInfo.getKeepAliveTime()));
        } catch (Exception ex) {
            log.error("Failed to modify the Tomcat thread pool parameter.", ex);
        }
    }
}
```

通过上面代码块的 simpleInfo 方法，客户端就可以把 web 容器线程池的配置信息收集成功，然后返回给服务端，服务端再展示给前端用户。当然，假如用户想在前端得到 web 容器线程池的实时运行时信息，或者是动态变更 web 容器线程池的配置信息。那就可以点击如下图所示的两个按钮，请看下面图片。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717421071466-60f79831-685c-4a8a-ba35-1f24b9c2a99d.png)

点击查看，得到的就是 web 容器线程池实时运行时信息，如下图所示。

  

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717421175559-6ceacd72-c553-4954-9c92-eff01012887d.png)

点击编辑，就是要执行 web 容器线程池配置信息动态更新操作，如下图所示。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717421201868-9834d9bc-4565-4b60-8f33-1885274d7f6c.png)

这两个操作的流程我就不再详细讲解了，毕竟都是一样的思路，而且具体执行操作的方法我也在前面的代码块中展示完毕了，大家简单看看就成。这些内容都没什么难度，我就不再耗费过多时间了。至于 Dubbo 线程池配置信息的收集、运行时信息的收集、以及配置信息动态变更，这些操作的流程和 tomcat 的线程池没什么区别，我也就不再展示了，大家在看第七版本代码块的时候，直接以 DubboThreadPoolAdapter 类为中心，看一看这个类以及围绕着这个类工作的其他几个类的内容即可。接下来，让我来为大家再补充一点之前应该讲解，但是被遗漏了的内容。

## 简单介绍前后端交互逻辑以及日志管理功能

本来预定的动态线程池框架的文章是十四章，但是大家也看到了，讲到后面，可讲的内容越来越少了。所以我把一些章节合并了一下，非常简单的内容就放在其他章节中简单解释一下。就比如现在要讲解的框架前后端交互逻辑，以及日志管理功能。当然，这里我也可以跟大家说一下，整个框架最终完整文章为十一篇，当前是第十章，下一章是通知告警功能的实现，这个功能还是值得单独开一章来讲解的，十一章之后，整个动态线程池框架课程就完结了。好了，让我们言归正传，首先让我来为大家解释一下动态线程池框架的日志管理功能是什么？

所谓日志管理就是当动态线程池框架执行一些重要操作的时候，都会把这些操作记录到数据库中，数据库中的这些信息就是操作日志，并且这些操作日志都可以在 web 界面展示给用户。当用户点击 web 界面的日志管理页面时，可以看到这样的信息。请看下面的图片。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717422375105-5e8fa562-9973-42be-961b-67d1a0d553aa.png)

如果点击每一栏日志的查看按钮，可以看到这样的信息，请看下面的图片。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717422400861-9d04b122-a67e-47a5-8317-1d20c7fbbcaa.png)

这些信息就是日志管理功能的体现。那么，前端是怎么从后端获得这些日志信息的呢？实际上在后端有一个专门和前端日志信息展示打交道的控制器，就是 LogRecordController 控制器，这个控制器实现如下，请看下面代码块。

```
package cn.hippo4j.console.controller;



/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/4/29
 * @Description:处理查询日志记录信息请求，这个控制器目前还用不到大家不必关心
 */
@RestController
@AllArgsConstructor
@RequestMapping(Constants.BASE_PATH + "/log")
public class LogRecordController {

    private final OperationLogService operationLogService;

    @PostMapping("/query/page")
    public Result<IPage<LogRecordRespDTO>> queryPage(@RequestBody LogRecordQueryReqDTO queryReqDTO) {
        return Results.success(operationLogService.queryPage(queryReqDTO));
    }
}
```

上面代码块的 queryPage 方法就负责从数据库中查询日志操作信息，然后返回给前端。当然，查询的操作是 operationLogService 对象来执行的。这个 operationLogService 对象大家可能有点陌生，我之前也许没跟大家提到过。但是先让我展示一段代码，大家就知道怎么回事了。请看下面代码块。

```
package cn.hippo4j.config.service.biz.impl;




/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2024/4/29
 * @Description:从数据库中查询线程池配置信息的类
 */
@Slf4j
@Service
@AllArgsConstructor
public class ConfigServiceImpl implements ConfigService {

    //这个mapper查询的就是线程池的配置信息
    private final ConfigInfoMapper configInfoMapper;

    //这个mapper查询的就是线程池实例的配置信息
    private final ConfigInstanceMapper configInstanceMapper;

    //这个是用来记录线程池配置变更操作日志的类，会把日志记录到数据库中
    private final OperationLogService operationLogService;





    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2024/4/29
     * @Description:更新数据库中ConfigAllInfo的方法
     */
    public void updateConfigInfo(String identify, boolean isChangeNotice, ConfigAllInfo config) {
        LambdaUpdateWrapper<ConfigAllInfo> wrapper = Wrappers.lambdaUpdate(ConfigAllInfo.class)
                .eq(ConfigAllInfo::getTpId, config.getTpId())
                .eq(ConfigAllInfo::getItemId, config.getItemId())
                .eq(ConfigAllInfo::getTenantId, config.getTenantId());
        config.setGmtCreate(null);
        config.setContent(ContentUtil.getPoolContent(config));
        config.setMd5(Md5Util.getTpContentMd5(config));
        
        
        
        //这里记录一下线程池配置变更操作日志
        recordOperationLog(config);
        
        
        
        try {//这里的这个identify就是线程池实例对象的instanceId，这个instanceId马上就会在接下来的版本代码中讲解
            if (StringUtil.isNotBlank(identify)) {
                //如果instanceId不为空，说明更新线程池配置信息的时候，也要把线程池实例的信息也更新一下
                ConfigInstanceInfo instanceInfo = BeanUtil.convert(config, ConfigInstanceInfo.class);
                instanceInfo.setInstanceId(identify);
                //在这里更新
                configInstanceMapper.update(instanceInfo, Wrappers.lambdaQuery(ConfigInstanceInfo.class).eq(ConfigInstanceInfo::getInstanceId, identify));
            } else if (StringUtil.isEmpty(identify) && isChangeNotice) {
                //走到这里就意味着identify为空，接下来就是向数据库中添加一个ConfigInstanceInfo对象的操作
                List<String> identifyList = ConfigCacheService.getIdentifyList(config.getTenantId(), config.getItemId(), config.getTpId());
                if (CollectionUtil.isNotEmpty(identifyList)) {
                    for (String each : identifyList) {
                        ConfigInstanceInfo instanceInfo = BeanUtil.convert(config, ConfigInstanceInfo.class);
                        instanceInfo.setInstanceId(each);
                        configInstanceMapper.insert(instanceInfo);
                    }
                }
            }
            //更新ConfigAllInfo对象的数据库信息
            configInfoMapper.update(config, wrapper);
        } catch (Exception ex) {
            log.error("[db-error] message: {}", ex.getMessage(), ex);
            throw ex;
        }
    }







    //记录数据库配置变更操作日志的方法，这里就是把操作日志存放到数据库中了
    private void recordOperationLog(ConfigAllInfo requestParam) {
        LogRecordInfo logRecordInfo = LogRecordInfo.builder()
                .bizKey(requestParam.getItemId() + "_" + requestParam.getTpId())
                .bizNo(requestParam.getItemId() + "_" + requestParam.getTpId())
                .operator(Optional.ofNullable(UserContext.getUserName()).orElse("-"))
                .action(String.format("核心线程: %d, 最大线程: %d, 队列类型: %d, 队列容量: %d, 拒绝策略: %d", requestParam.getCoreSize(), requestParam.getMaxSize(), requestParam.getQueueType(),
                        requestParam.getCapacity(), requestParam.getRejectedType()))
                .category("THREAD_POOL_UPDATE")
                .detail(JSONUtil.toJSONString(requestParam))
                .createTime(new Date())
                .build();
        operationLogService.record(logRecordInfo);
    }


    //其余方法省略
}
```

在上面代码块中，**可以看到当用户动态修改了线程池的配置信息时，也就是调用了 updateConfigInfo 方法，就会在 updateConfigInfo 方法中调用 recordOperationLog 方法记录线程池配置变更操作的信息**，而在这个过程中，就会调用 operationLogService 对象的 record 方法，把操作日志存放到数据库中。OperationLogService 类的代码我也准备好了，请看下面代码块。

```
package cn.hippo4j.config.service.biz.impl;



/**
 * Operation log service impl.
 */
@Service
@AllArgsConstructor
public class OperationLogServiceImpl implements OperationLogService {

    private final OperationLogMapper operationLogMapper;

    //从数据库中查询操作日志信息
    @Override
    public IPage<LogRecordRespDTO> queryPage(LogRecordQueryReqDTO pageQuery) {
        LambdaQueryWrapper<LogRecordInfo> queryWrapper = Wrappers.lambdaQuery(LogRecordInfo.class)
                .eq(StringUtil.isNotBlank(pageQuery.getBizNo()), LogRecordInfo::getBizNo, pageQuery.getBizNo())
                .eq(StringUtil.isNotBlank(pageQuery.getCategory()), LogRecordInfo::getCategory, pageQuery.getCategory())
                .eq(StringUtil.isNotBlank(pageQuery.getOperator()), LogRecordInfo::getOperator, pageQuery.getOperator())
                .orderByDesc(LogRecordInfo::getCreateTime);
        IPage<LogRecordInfo> selectPage = operationLogMapper.selectPage(pageQuery, queryWrapper);
        return selectPage.convert(each -> BeanUtil.convert(each, LogRecordRespDTO.class));
    }

    @Override
    public void record(LogRecordInfo requestParam) {
        //把数据插入到数据库中
        operationLogMapper.insert(requestParam);
    }
}
```

现在，大家应该明白了动态线程池框架的日志管理功能是怎么实现的了。

接下来我再为大家补充一下动态线程池框架前后端的交互逻辑，这里我就不在文章中展示什么代码或者 web 界面的图片了。我就直接跟大家说去第七版本的哪个包下看哪些代码了。大家可以直接去下图展示的模块中查看所有的控制器，这些控制器就分别负责和前端相应的界面交互。请看下图。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1717423133766-67b24f70-9caa-4772-acf1-eb7eca674c46.png)

并且我在每一个控制器中都添加了注释，在注释中说明了当前控制器和前端哪个页面对应，大家可以直接从代码中查看。好了，到此为止，我要补充的一些小知识点也讲完了。这一章的内容也就结束了，后面这三章讲得确实很快，还请大家理解一下，因为确实没什么东西可讲了。我总不能把已经讲过很多次的知识再啰里啰唆重复好几遍，也不能为了凑篇幅而水字数。我相信只要大家掌握了这门课程前面的内容，那么学习这一章和下一章的内容会非常容易。下一章我将为大家实现动态线程池的告警通知功能，等第十一章更新完毕，我们这套课程也就结束了。朋友们，我们下一章见！