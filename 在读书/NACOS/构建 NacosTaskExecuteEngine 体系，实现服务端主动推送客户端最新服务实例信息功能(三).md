  

## 引入 PushExecuteTask 推送任务

话不多说，让我们直接开始本章的内容。在上一章我为大家详细剖析了 nacos 服务端的执行引擎体系，并且也引入了处理延迟推送任务的 PushDelayTaskExecuteEngine 引擎，我们也知道了，这个延迟推送任务执行引擎并不会直接处理延迟推送任务，而是**合并相同的任务，等待任务度过限时等待时间，然后把任务提交给可以并发处理所有任务的新的执行引擎，也就是立即执行任务的引擎**。也就是说，上**一章我们一直没有实现的 PushDelayTaskExecuteEngine 引擎内部的 PushDelayTaskProcessor 处理器，应该执行的就是把任务提交给立即执行任务的引擎的操作**。PushDelayTaskExecuteEngine 类的内容如下，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务执行引擎
 */
public class PushDelayTaskExecuteEngine extends NacosDelayTaskExecuteEngine {

    //构造方法
    public PushDelayTaskExecuteEngine() {

        //在创建PushDelayTaskExecuteEngine对象的时候，就创建了一个延迟推送任务处理器对象
        //并且把这个处理器对象设置到当前类对象的顶级抽象父类AbstractNacosTaskExecuteEngine中了
        setDefaultTaskProcessor(new PushDelayTaskProcessor(this));
    }
    


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：延迟推送任务处理器
     */
    private static class PushDelayTaskProcessor implements NacosTaskProcessor {

        private final PushDelayTaskExecuteEngine executeEngine;

        public PushDelayTaskProcessor(PushDelayTaskExecuteEngine executeEngine) {
            this.executeEngine = executeEngine;
        }

        //处理延迟推送任务的方法
        @Override
        public boolean process(NacosTask task) {
            
            //在这里把得到的延迟推送任务提交给新的执行引擎
        }
    }


    

    //其他内容暂时省略
    
}
```

通过上面代码块可以看到，我们还是没有实现 PushDelayTaskProcessor 处理器的 process() 方法，虽然我们知道最终任务是要被新的执行引擎执行，PushDelayTaskProcessor 处理器的 process() 方法要做的就是把延迟推送任务提交给新的执行引擎，这些操作似乎还挺容易实现的，无非就是把新的执行引擎定义出来，然后把任务提交到新引擎的任务队列中就行了。但是请大家思考一个问题：**那就提交给新执行引擎的究竟是什么任务呢？是直接从 PushDelayTaskExecuteEngine 任务队列中取出来的延迟推送任务吗**？

我请大家思考这个问题的原因很简单，因为任务一旦被提交给新的执行引擎，也就是立即执行任务的引擎，这就意味着任务要被执行了。而执行任务的那一套模式我们都已经很熟悉了，无非就是线程执行 Runnable 任务，可是我们目前定义的 延迟推送任务根本不是一个 Runnable 啊。我把 PushDelayTask 类给大家搬运到这里了，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务类，这个类的父类AbstractDelayTask也没有实现Runnable接口，这就意味着当前类根本不是一个Runnable
 */
public class PushDelayTask extends AbstractDelayTask {

    //要推送的服务的元信息
    private final Service service;

    //是推送给所有订阅了该服务的客户端，还是只推送给个别客户端的标志
    //这里我想解释一下为什么会存在这个成员变量，之前我就跟大家说过，当一个客户端向服务端执行了
    //订阅服务信息的操作时，服务端会主动向该客户端推送一次最新的服务实例信息
    //这个推送的操作也会被封装为PushDelayTask任务，如果多个客户端同时都订阅了某个Service
    //这就意味着服务端需要向这些客户端都推送最新的服务实例信息，这个时候也可以把推送的任务合并一下
    private boolean pushToAll;

    //这个成员变量就是要推送的目标客户端，处理客户端订阅操作时，这个成员变量才会被赋值
    //如果当前任务是把最新的服务实例信息推送给订阅了Service的所有客户端，那这个成员变量就不会被赋值
    private Set<String> targetClients;

    //构造方法，这个构造方法创建的任务，就会把服务实例信息推送给所有订阅了服务的客户端
    public PushDelayTask(Service service, long delay) {
        this.service = service;
        pushToAll = true;
        targetClients = null;
        //设置任务执行时间间隔
        setTaskInterval(delay);
        //设置上一次任务被处理的时间，其实就是任务被创建的时间
        setLastProcessTime(System.currentTimeMillis());
    }

    
    //构造方法，这个构造方法创建的任务，就会把服务实例推送给个别客户端，并不会全部推送，这个构造方法会在处理服务订阅事件时被调用
    public PushDelayTask(Service service, long delay, String targetClient) {
        this.service = service;
        this.pushToAll = false;
        this.targetClients = new HashSet<>(1);
        this.targetClients.add(targetClient);
        //设置任务执行时间间隔
        setTaskInterval(delay);
        //设置上一次任务被处理的时间
        setLastProcessTime(System.currentTimeMillis());
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：聚合两个任务的方法，我知道大家对这个方法会有疑问，觉得Service根本没有用上，怎么知道合并的两个任务推送的就是相同的服务实例信息呢？
     * 请先把疑问记在心里，继续向后看，后面会解决这个疑问
     */
    @Override
    public void merge(AbstractDelayTask task) {
        //判断要聚合的任务是否为延迟推送任务
        if (!(task instanceof PushDelayTask)) {
            //如果不是该类型的，就可以直接退出当前方法
            return;
        }//得到旧的延迟推送任务
        PushDelayTask oldTask = (PushDelayTask) task;
        //判断旧的任务是否要推送给所有订阅了服务的客户端
        //同时也会判断当前任务是否要推送给所有订阅了服务的客户端
        if (isPushToAll() || oldTask.isPushToAll()) {
            //只要有一个满足，就把当前任务的pushToAll设置为true，表示推送给所有订阅了该服务的客户端
            //这个操作其实没什么深意，反正有两个任务，假如一个任务要推送给所有客户端，一个任务只推送一个
            //推送的都是最新信息，那肯定就执行推送所有客户端的任务，只要这个任务执行了，就不必再执行单独推送的任务了
            pushToAll = true;
            targetClients = null;
        } else {//走到这里就意味着两个任务都要推送给目标客户端
            //所以这里就把目标客户端合并一下，也就是把旧任务的目标客户端添加到targetClients集合中
            targetClients.addAll(oldTask.getTargetClients());
        }//在这里判断一下，哪个任务先被创建出来，就用哪个任务的创建时间作为上一次被处理的时间
        //这样经过限时等待之后，任务就可以尽快被执行了
        setLastProcessTime(Math.min(getLastProcessTime(), task.getLastProcessTime()));
        Loggers.PUSH.info("[PUSH] Task merge for {}", service);
    }

    public Service getService() {
        return service;
    }

    //判断是否要推送给所有订阅了服务的客户端，还是推送给目标客户端
    public boolean isPushToAll() {
        return pushToAll;
    }

    public Set<String> getTargetClients() {
        return targetClients;
    }
}
```

从上面代码块可以看到，PushDelayTask 类的对象根本就不是 Runnable 任务，没办法被执行器执行，在 PushDelayTask 类中我们也看不到有什么 run 方法。所以，**如果我们直接把 PushDelayTask 延迟推送任务提交给新的执行引擎，也就是立即执行任务的引擎，新的执行引擎根本没办法执行这些任务**。这一点大家应该可以理解吧？那这时候该怎么办呢？不能直接提交延时推送任务了，那要提交什么任务给新的执行引擎呢？

其实进行到这里大家应该也能意识到了，**实际上完全没必要再把 PushDelayTask 延迟推送任务提交给新的执行引擎来执行。因为该任务最主要的功能就是用来合并相同的延迟推送任务，任务合并完成之后，就不需要再使用 PushDelayTask 任务来封装把服务实例信息推送给客户端的操作了**。我们最终的目标就是要让 nacos 服务端把最新的服务实例信息主动推送给订阅了对应 Service 服务的客户端，在这个过程中，**我们需要根据延迟推送任务中封装的 Service 信息找出订阅了该服务的所有客户端，还要更新 ServiceStorage 中缓存的服务实例信息，然后从 ServiceStorage 中得到最新的服务实例信息，把这些信息推送给对应的客户端**。这些流程在前面的章节我就为大家总结完毕了，还记得我为大家总结的那四个操作吧？第一个操作已经实现完毕了，还剩下三个操作待实现，**现在我们要做的显然就是再定义一个新的任务对象，这个新的任务对象就是一个 Runnable，然后把剩下的三个操作定义在这个新任务对象的 run 方法中，我们只需要把这个新的任务对象提交给新的执行引擎即可**。这样一来，新的执行引擎就会执行新的任务，把最新的服务实例信息推送给对应的客户端。

**比如说我们就把这个新的任务定义为 PushExecuteTask，这个任务实现了 Runnable 接口，可以直接被线程执行**。在这个任务的 run 方法中，我们要执行的操作有三个：

**1 更新 ServiceStorage 服务信息存储器中的服务实例信息，然后从 ServiceStorage 中得到发生变更的 Service 下的最新的服务实例信息。**

**2 根据 Service 找出订阅了该服务的所有客户端。**

**3 把最新的服务实例信息推送给所有客户端。**

以上就是 PushExecuteTask 任务要执行的操作，接下来就让我们根据刚才的分析，把 PushExecuteTask 类定义出来，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：服务端向客户端推送服务实例信息的任务
 */
public class PushExecuteTask implements Runnable {

    //要推送给客户端的服务实力的元信息对象
    private final Service service;

    //延迟执行推送任务的执行引擎
    private final PushDelayTaskExecuteEngine delayTaskEngine;

    //延迟推送任务
    private final PushDelayTask delayTask;

    //构造方法
    public PushExecuteTask(Service service, PushDelayTaskExecuteEngine delayTaskEngine, PushDelayTask delayTask) {
        this.service = service;
        this.delayTaskEngine = delayTaskEngine;
        this.delayTask = delayTask;
    }

    //当前任务要执行的操作
    @Override
    public void run() {
        try {

            //这里的generatePushData()方法就会更新ServiceStorage缓存的服务实例信息，然后返回最新的服务实例信息
            //得到一个服务信息包装对象，得到的这个包装对象已经把要推送给客户端的服务实例信息都封装好了
            PushDataWrapper wrapper = generatePushData();
            
            

            //接下来要执行的操作就是得到订阅了Service的客户端，也就是要把信息推送给的客户端
            //从延迟执行引擎中得到客户端管理器，如果有朋友还没看过第九版本的部分代码，可能会对这里有些困惑
            //不知道为什么延迟推送任务执行引擎怎么就能忽然获得客户端管理器了，在前面章节我为大家实现的执行引擎中并没有客户端管理器这个成员变量
            //这是因为我把部分成员变量给省略了，实际上我在文章中省略了很多组件中的成员变量，只展示了该组件最核心的功能
            //大家可以结合第九版本代码阅读文章，省略的成员变量来来回回就那么几个，大家肯定都很熟悉，而且省略的逻辑都非常简单，大家一看代码就清楚了
            ClientManager clientManager = delayTaskEngine.getClientManager();
            //getTargetClientIds()方法会得到要推送的客户端Id的集合
            //然后在循环中把服务实例信息推送给遍历到的每一个客户端
            for (String each : getTargetClientIds()) {
                //根据客户端Id得到对应的客户端对象
                Client client = clientManager.getClient(each);
                if (null == client) {
                    //如果客户端对象为空则跳过本次循环
                    continue;
                }//从客户端对象中得到订阅了该服务的订阅者
                Subscriber subscriber = client.getSubscriber(service);
                //如果订阅者为空则跳过本次循环
                if (subscriber == null) {
                    continue;
                }


                //下面就是把服务实例信息推送给订阅者的操作了，在下面的操作中，delayTaskEngine.getPushExecutor()这行代码
                //最终会得到一个RPC推送服务，其实就是一个PushExecutorRpcImpl对象，然后调用该对象的doPushWithCallback()方法
                //把服务实例信息推送出去。当然，之后还会用到真正发送消息给客户端的RpcPushService组件，这些我就不在文章中展示了
                //在文章中我已经把最核心的发送服务实例信息给客户端的核心功能实现了，剩下的流程都非常简单，大家自己看看第九版本代码即可
                
                //在这里先得到了推送执行器代理对象，也就是PushExecutorDelegate对象，PushExecutorDelegate对象又调用了PushExecutorRpcImpl对象的doPushWithCallback()方法
                //再执行了推送服务实例信息的操作，执行操作的时候还定义了一个回调对象ServicePushCallback，这个回调对象中的方法会在接收到客户端响应后被回调，但回调对象我就在文中省略了，其实就是当前类的一个内部类
                //用来计算推送任务耗时的
                delayTaskEngine.getPushExecutor().doPushWithCallback(each, subscriber, wrapper, new ServicePushCallback(each, subscriber, wrapper.getOriginalData(), delayTask.isPushToAll()));
            }
        } catch (Exception e) {
            Loggers.PUSH.error("Push task for service" + service.getGroupedServiceName() + " execute failed ", e);
            delayTaskEngine.addTask(service, new PushDelayTask(service, 1000L));
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：得到封装了要推送到客户端的服务实例信息的服务信息包装对象的方法
     */
    private PushDataWrapper generatePushData() {
        //从服务信息存储器中得到服务实例信息，ServiceStorage服务信息存储器的getPushData()方法会更新ServiceStorage服务信息存储器存储的服务实例信息
        ServiceInfo serviceInfo = delayTaskEngine.getServiceStorage().getPushData(service);
        //把这些信息封装到服务信息包装对象中
        return new PushDataWrapper(serviceMetadata, serviceInfo);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：得到要推送服务实例信息的客户端Id的集合
     */
    private Collection<String> getTargetClientIds() {
        //判断是推送到全部客户端还是部分客户端，然后再获得对应的客户端Id即可
        //如果要推送到全部客户端，就从服务索引管理器中得到订阅了该服务的所有客户端Id
        //如果推送给部分客户端，直接从延迟推送任务中获得targetClients目标客户端集合即可
        return delayTask.isPushToAll() ? delayTaskEngine.getIndexesManager().getAllClientsSubscribeService(service) : delayTask.getTargetClients();
    }





    //其他内容暂时省略
}
```

到此为止，我就为大家把 PushExecuteTask 任务定义完毕了，也把 nacos 服务端向客户端推送最新服务实例信息的核心操作定义完成了。因为上面代码块中的注释非常详细，而且在展示代码块之前我也给大家把功能的设计思路分析得十分详细了，所以就不再为大家重复解释上面代码块的逻辑了。总之，现在我们就剩下最后一个问题没有解决：**那就是新定义的这个 PushExecuteTask 任务该怎么被执行呢**？很简单，**当然就是交给新引入的可以立即执行任务的引擎去执行，也就是我上一章为大家引入的 NacosExecuteTaskExecuteEngine 这个执行引擎**。虽然这个 NacosExecuteTaskExecuteEngine 引擎的具体内容还没有跟大家展示，但是大家现在肯定都清楚这个引擎的具体作用是什么了。这一点理解了之后，那我们就可以再思考思考这个 PushExecuteTask 任务怎么被创建，又怎么提交给新引入的执行引擎呢？**这时候就可以把 PushDelayTaskExecuteEngine 延迟推送任务执行引擎的 PushDelayTaskProcessor 处理器的 process() 方法实现一下了，因为创建 PushExecuteTask 任务，并且把任务提交给新引入的执行引擎的操作就是在 process() 方法中执行的**。

## 引入 NacosExecuteTaskExecuteEngine 执行引擎

接下来我就先为大家展示一下实现之后的 PushDelayTaskExecuteEngine 延迟推送任务执行引擎的 PushDelayTaskProcessor 处理器的 process() 方法，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务执行引擎
 */
public class PushDelayTaskExecuteEngine extends NacosDelayTaskExecuteEngine {

    //构造方法
    public PushDelayTaskExecuteEngine() {

        //在创建PushDelayTaskExecuteEngine对象的时候，就创建了一个延迟推送任务处理器对象
        //并且把这个处理器对象设置到当前类对象的顶级抽象父类AbstractNacosTaskExecuteEngine中了
        setDefaultTaskProcessor(new PushDelayTaskProcessor(this));
    }
    


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：延迟推送任务处理器
     */
    private static class PushDelayTaskProcessor implements NacosTaskProcessor {

        private final PushDelayTaskExecuteEngine executeEngine;

        public PushDelayTaskProcessor(PushDelayTaskExecuteEngine executeEngine) {
            this.executeEngine = executeEngine;
        }

        //处理延迟推送任务的方法
        @Override
        public boolean process(NacosTask task) {
            PushDelayTask pushDelayTask = (PushDelayTask) task;
            //得到要推送的服务元信息对象
            Service service = pushDelayTask.getService();
            //分发并且执行任务，其实这里就是创建了一个PushExecuteTask任务，这个任务没有延迟，提交给执行器后就会被立刻执行
            //一旦执行，就会把服务实例信息推送给客户端，注意，这里创建PushExecuteTask任务对象的时候，把当前类的对象，也就是延时任务执行引擎传进去了
            NamingExecuteTaskDispatcher.getInstance().dispatchAndExecuteTask(service, new PushExecuteTask(service, executeEngine, pushDelayTask));
            return true;
        }
    }


    

    //其他内容暂时省略
    
}
```

在上面代码块中，我们就可以看到确实是在 PushDelayTaskProcessor 处理器的 process() 方法中创建了真正要被执行的 PushExecuteTask 任务，这一点大家肯定再清楚不过了。但是这个 PushExecuteTask 任务不是要提交给新引入的 NacosExecuteTaskExecuteEngine 立即执行任务的引擎吗？在上面代码块中并没有体现出这一点。而是执行了 NamingExecuteTaskDispatcher.getInstance().dispatchAndExecuteTask() 这行代码，大家别急，**实际上这行代码就是把创建的 PushExecuteTask 任务分发给 NacosExecuteTaskExecuteEngine 引擎了**。我把 NamingExecuteTaskDispatcher 类的内容展示一下大家就立刻清楚了，请看下面代码块。

```
package com.cqfy.nacos.naming.misc;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：任务分发器
 */
public class NamingExecuteTaskDispatcher {

    //单例模式
    private static final NamingExecuteTaskDispatcher INSTANCE = new NamingExecuteTaskDispatcher();


    //非延时执行任务的引擎，也就是本小节我为大家新引入的引擎
    private final NacosExecuteTaskExecuteEngine executeEngine;

    //构造方法
    private NamingExecuteTaskDispatcher() {
        //在构造方法中创建了nacos任务执行引擎，这个任务执行引擎会直接执行任务，并不会延迟执行任务
        //这就是它和延迟任务执行引擎的区别
        executeEngine = new NacosExecuteTaskExecuteEngine(EnvUtil.FUNCTION_MODE_NAMING, Loggers.SRV_LOG);
    }

    public static NamingExecuteTaskDispatcher getInstance() {
        return INSTANCE;
    }

    //分发任务的方法
    public void dispatchAndExecuteTask(Object dispatchTag, AbstractExecuteTask task) {
        //其实就是把PushExecuteTask任务提交到executeEngine这个任务执行引擎中
        executeEngine.addTask(dispatchTag, task);
    }

    //得到引擎中所有工作的线程的状态
    public String workersStatus() {
        return executeEngine.workersStatus();
    }

    public void destroy() throws Exception {
        executeEngine.shutdown();
    }
}
```

NamingExecuteTaskDispatcher 类的内容展示了之后，最后，也轮到展示这个 NacosExecuteTaskExecuteEngine 引擎了，这个引擎就是用来真正执行 PushExecuteTask 任务的，把最新的服务实例信息推送给订阅了对应的 Service 的客户端。这个 NacosExecuteTaskExecuteEngine 引擎就和我们之前看到过的各种执行器的常规组件一样了，举一个最直接的例子，n**etty 中的 EventLoopGroup 大家都还记得吧？EventLoopGroup 管理着一个单线程执行器数组，每一个单线程执行器就是用来真正执行任务的。NacosExecuteTaskExecuteEngine 执行引擎的结构和 EventLoopGroup 类似，它也管理者一个单线程执行器数组，所谓单线程执行器就是 TaskExecuteWorker 对象，TaskExecuteWorker 类的内容我也会为大家展示出来**，这些内容都很简单，我把代码一展示大家就全明白了。这些组件大家应该都见过很多次了，是最常见的执行器组件，我在过去的课程中也给大家展示过类似的组件，所以就不再重复讲解 NacosExecuteTaskExecuteEngine 执行引擎了，大家自己看看代码就行。

首先是 NacosExecuteTaskExecuteEngine 的内容，请看下面代码块。

```
package com.cqfy.nacos.common.task.engine;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：nacos任务执行引擎，这个执行引擎执并不会执行延迟任务，延迟任务会交给NacosDelayTaskExecuteEngine执行引擎来执行
 */
public class NacosExecuteTaskExecuteEngine extends AbstractNacosTaskExecuteEngine<AbstractExecuteTask> {

    //执行任务的线程执行器组
    private final TaskExecuteWorker[] executeWorkers;

    //构造方法
    public NacosExecuteTaskExecuteEngine(String name, Logger logger) {
        //在这里设置了executeWorkers线程执行器组数组的容量
        this(name, logger, ThreadUtils.getSuitableThreadCount(1));
    }

    //构造方法
    public NacosExecuteTaskExecuteEngine(String name, Logger logger, int dispatchWorkerCount) {
        //继续调用父类方法
        super(logger);
        //在这里创建了工作线程执行器组对象
        executeWorkers = new TaskExecuteWorker[dispatchWorkerCount];
        for (int mod = 0; mod < dispatchWorkerCount; ++mod) {
            //在这里创建了每一个单线程执行器对象
            executeWorkers[mod] = new TaskExecuteWorker(name, mod, dispatchWorkerCount, getEngineLog());
        }
    }

    //得到执行引擎要执行的任务的数量
    @Override
    public int size() {
        int result = 0;
        //遍历线程组
        for (TaskExecuteWorker each : executeWorkers) {
            //得到每一个线程执行器要执行的任务的数量。累加即可
            result += each.pendingTaskCount();
        }//返回总的任务数量
        return result;
    }

    //判断执行引擎有没有要执行的任务
    @Override
    public boolean isEmpty() {
        return 0 == size();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：把任务添加到执行引擎中的方法
     */
    @Override
    public void addTask(Object tag, AbstractExecuteTask task) {
        //先得到处理该任务的处理器
        NacosTaskProcessor processor = getProcessor(tag);
        //如果有处理器
        if (null != processor) {
            //就直接让处理器处理任务
            //在执行PushExecuteTask任务时，这里是得不到对应的处理器的
            processor.process(task);
            return;
        }
        //如果没有获得对应的处理器，那就从线程组中获得一个线程执行器
        TaskExecuteWorker worker = getWorker(tag);
        //把任务交给这个线程执行器来处理
        worker.process(task);
    }

    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：从线程组中获得一个线程执行器的方法
     */
    private TaskExecuteWorker getWorker(Object tag) {
        //计算索引
        int idx = (tag.hashCode() & Integer.MAX_VALUE) % workersCount();
        //根据索引获得线程组中的线程执行器
        return executeWorkers[idx];
    }

    //得到线程组创建线程执行器的数量
    private int workersCount() {
        return executeWorkers.length;
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：移除对应任务的方法
     */
    @Override
    public AbstractExecuteTask removeTask(Object key) {
        throw new UnsupportedOperationException("ExecuteTaskEngine do not support remove task");
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：得到所有任务的key的方法，注意，这个方法和上面的方法，当前执行引擎并不支持这两种操作
     */
    @Override
    public Collection<Object> getAllTaskKeys() {
        throw new UnsupportedOperationException("ExecuteTaskEngine do not support get all task keys");
    }

   /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/22
    * @方法描述：终止执行引擎工作的方法
    */
    @Override
    public void shutdown() throws NacosException {
        for (TaskExecuteWorker each : executeWorkers) {
            each.shutdown();
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：得到所有工作线程执行器状态的方法
     */
    public String workersStatus() {
        StringBuilder sb = new StringBuilder();
        for (TaskExecuteWorker worker : executeWorkers) {
            sb.append(worker.status()).append('\n');
        }
        return sb.toString();
    }
}
```

接下来是 TaskExecuteWorker 单线程执行器的内容，请看下面代码块。

```
package com.cqfy.nacos.common.task.engine;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：执行任务的线程执行器，这个执行器就是一个单线程执行器
 */
public final class TaskExecuteWorker implements NacosTaskProcessor, Closeable {

    //执行器存放任务的任务队列的容量
    private static final int QUEUE_CAPACITY = 1 << 15;

    //日志工具
    private final Logger log;

    //执行器名称
    private final String name;

    //任务队列
    private final BlockingQueue<Runnable> queue;

    //表示执行器是否关闭的标志
    private final AtomicBoolean closed;

    //真正执行任务的工作线程
    private final InnerWorker realWorker;

    //构造方法
    public TaskExecuteWorker(final String name, final int mod, final int total) {
        this(name, mod, total, null);
    }

    //构造方法
    public TaskExecuteWorker(final String name, final int mod, final int total, final Logger logger) {
        this.name = name + "_" + mod + "%" + total;
        this.queue = new ArrayBlockingQueue<>(QUEUE_CAPACITY);
        this.closed = new AtomicBoolean(false);
        this.log = null == logger ? LoggerFactory.getLogger(TaskExecuteWorker.class) : logger;
        realWorker = new InnerWorker(this.name);
        //在这里启动了线程
        realWorker.start();
    }

    public String getName() {
        return name;
    }


    //处理任务队列中任务的方法
    @Override
    public boolean process(NacosTask task) {
        if (task instanceof AbstractExecuteTask) {
            putTask((Runnable) task);
        }
        return true;
    }

    //把任务存放到任务队列中的方法
    private void putTask(Runnable task) {
        try {
            queue.put(task);
        } catch (InterruptedException ire) {
            log.error(ire.toString(), ire);
        }
    }

    //得到待处理的任务数量的方法
    public int pendingTaskCount() {
        return queue.size();
    }


    //得到单线程执行器状态的方法
    public String status() {
        return getName() + ", pending tasks: " + pendingTaskCount();
    }

    //终止执行器工作的方法
    @Override
    public void shutdown() throws NacosException {
        queue.clear();
        closed.compareAndSet(false, true);
        realWorker.interrupt();
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：这个内部类的对象就是一个线程
     */
    private class InnerWorker extends Thread {

        InnerWorker(String name) {
            //设置为非守护线程
            setDaemon(false);
            //设置线程名称
            setName(name);
        }

        /**
         * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
         * @author：陈清风扬，个人微信号：chenqingfengyangjj。
         * @date:2024/7/22
         * @方法描述：线程处理任务队列中人物的方法，也是线程启动之后要执行的方法
         */
        @Override
        public void run() {
            //判断执行器是否已经关闭
            while (!closed.get()) {
                try {//没有关闭则循环从队列中获取任务
                    Runnable task = queue.take();
                    //得到操作开始时间
                    long begin = System.currentTimeMillis();
                    //执行任务，这个时候执行的就是PushExecuteTask任务对象中的run方法了
                    task.run();
                    //得到操作结束时间
                    long duration = System.currentTimeMillis() - begin;
                    //执行超过1000毫秒则记录告警日志
                    if (duration > 1000L) {
                        log.warn("task {} takes {}ms", task, duration);
                    }
                } catch (Throwable e) {
                    log.error("[TASK-FAILED] " + e, e);
                }
            }
        }
    }
}
```

到此为止，我就为大家把 nacos 服务端主动推送最新服务实例信息给客户端的功能全部实现了，现在大家已经可以阅读我提供的第九版本代码的全部内容了。在阅读代码的过程中，大家可以对比一下我在文章中实现的功能，展示的代码，想对于我提供的第九版本代码在哪些地方做了简化，以及没有在文章中全部展示的组件究竟是什么样的。这一章的内容非常简单，大家可以当成一次休息站，下一章我们就要正式开始搭建 nacos 服务端集群了，为框架引入 Distro 集群。

  

当然，我还是那句话，不管课程进度推进到哪里，整个 nacos 框架可以说是一点难度都没有，就算来到集群模块了，无非就是要实现的功能繁琐一些，要定义的操作也丰富一些，其他的也就没有什么了。如果有朋友一篇篇文章看到这里，之前的版本代码也都看完了，你们可以问一问自己，学到这里难道有哪一块的知识真的很难吗？我想大家应该都是平推过来的。所有代码没有一点技巧可言、没有一点新意可言，都是最普通的代码；很多类都是加一个 springboot 注释，被 springboot 容器管理，然后启动定时任务执行器执行任务，需要提高效率就使用线程池，搞来搞去，一直都是这一套。所以，继续跟下去就完事了。好了朋友们，我们下一章见！