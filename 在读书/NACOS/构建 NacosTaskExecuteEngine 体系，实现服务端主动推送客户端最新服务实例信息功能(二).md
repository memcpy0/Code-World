上一章我为大家总结了实现 nacos 服务端主动向客户端推送最新服务实例信息功能时，程序必须执行的四个操作，这四个操作就是：

**1 首先 nacos 服务端要主动感知到保存的服务实例信息发生了变化。**

**2 根据更新的服务实例所在服务的信息，找到订阅了该服务的 nacos 客户端。**

**3 更新** **ServiceStorage 服务信息存储器中缓存的服务实例信息，并且从 ServiceStorage 服务信息存储器中得到最新的服务实例信息。**

**4 把 nacos 客户端订阅的 Service 服务下的最新的服务实例信息全部发送给对应的 nacos 客户端。**

并且我们已经实现了第一个操作对应的功能，那就是 nacos 服务端已经可以在服务实例信息发生变更时立刻发布 ServiceChangedEvent 事件，新引入的 NamingSubscriberServiceV2Impl 订阅者会接收并处理该事件，NamingSubscriberServiceV2Impl 的内容如下，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：这个类的对象也是一个订阅者，订阅了ServiceChangedEvent和ServiceSubscribedEvent事件
 * 这样一来，只要服务端的服务实例信息发生变更，或者有客户端订阅了服务信息，这个类的对象就发起主动推送服务实例信息给客户端的操作
 */
@org.springframework.stereotype.Service
public class NamingSubscriberServiceV2Impl extends SmartSubscriber implements NamingSubscriberService {



    //构造方法
    public NamingSubscriberServiceV2Impl() {
       //在这里把自己注册到通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：当前订阅者订阅的事件类型
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //订阅了服务实例变更事件
        result.add(ServiceEvent.ServiceChangedEvent.class);
        //订阅了服务订阅事件
        result.add(ServiceEvent.ServiceSubscribedEvent.class);
        return result;
    }





    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：处理具体事件的方法
     */
    @Override
    public void onEvent(Event event) {
        if (event instanceof ServiceEvent.ServiceChangedEvent) {

            //如果是服务实例变更事件，就在这里处理服务实例变更事件
            
        } else if (event instanceof ServiceEvent.ServiceSubscribedEvent) {


            //如果发布的是服务订阅事件，就在这里处理服务订阅事件
            //ServiceSubscribedEvent事件是在ClientServiceIndexesManager类的
            //addSubscriberIndexes()方法中发布的，刚才已经提示过大家了，现在大家应该还有一些印象
            //虽然当前处理ServiceSubscribedEvent事件的功能还没有实现，但现在我可以为大家简单解释一下
            //在本章一开始我跟大家说了，当某个客户端向服务端执行了订阅服务实例操作时，
            //服务端会立刻发布一个ServiceSubscribedEvent服务订阅事件，对应的事件订阅者接收到该事件后
            //就会在处理该事件的过程中把最新的服务实例信息发送给订阅了该服务的客户端，这里就是执行这个操作的入口
            //但在本章我们还无法实现这个功能，下一章大家就知道怎么回事了
        }
    }



    //其他内容省略
}  
```

从上面代码块中可以看到，**NamingSubscriberServiceV2Impl 订阅者的 onEvent() 方法还没有实现，这也就意味着我们目前重构的 nacos 服务端只可以感知到有服务实例发生了变更，但是在服务实例变更之后，要执行的操作还没有定义**。当然，要执行的操作我们都已经清楚了，就是以上四个操作中，剩下的三个操作，但是这三个操作的具体流程究竟该怎么定义，还需要仔细地分析分析。这也正是本章的核心内容。

## 引入 PushDelayTask 延迟推送任务

按照顺序来说，我们现在应该关注之前总结的四个操作中的第二个，也就是：**根据更新的服务实例所在服务的信息，找到订阅了该服务的 nacos 客户端**。但实际上操作二和操作三的顺序并不具备依赖关系，操作三是**更新 ServiceStorage 服务信息存储器中缓存的服务实例信息，并且从 ServiceStorage 服务信息存储器中得到最新的服务实例信息**；操作四是**把 nacos 客户端订阅的 Service 服务下的最新的服务实例信息全部发送给对应的 nacos 客户端**。有一点可以明确，那就是操作四一定是最后执行的，但是操作二和操作三，从操作具体执行的内容来看，似乎操作二和操作三哪一个先执行都可以。那我们接下来应该先关注哪一个操作呢？

这里我就不卖关子了，实际上现在还轮不到关注操作二和操作三，原因很简单，操作二和操作三只是我们概括出来的功能要实现的大方向，就算我不总结，每一位朋友也能想到 nacos 服务端在感知到服务实例发生变更之后，应该执行这些操作。但是更细节的问题，大家就不一定能考虑到了，但正是这些更细节的问题促使程序更加完善，随着这些问题的解决，我们对程序的重构才会不断到达新的阶段；也许解决了一部分问题，就该轮到考虑怎么**根据更新的服务实例所在服务的信息，找到订阅了该服务的 nacos 客户端**。也许再解决另一部分问题，就可以考虑怎么**更新 ServiceStorage 服务信息存储器中缓存的服务实例信息了**。这么说可能不太容易理解，那就让我来为大家列举一个非常实际的小问题，就比如说上一章结尾我提出来的那个问题：**假如 nacos 服务端保存的某个 Service 下的服务实例信息频繁发生变更，几秒之内变更了十几次，难道服务端就要立刻给订阅了该服务的客户端一下子发送十几个服务变更请求吗？这显然又些不合理，那该怎么解决这种情况呢**？

我相信很多朋友在看到这个问题之后，一时间都没有头绪，不知道这个问题究竟是在问什么。但是让我换一种问法，大家就会恍然大悟了，**当 nacos 服务端要把最新的服务实例信息推送给客户端时，究竟应该怎么推送呢**？**是某个 Service 下的服务实例信息每变更一次就推送一次，还是服务实例信息变更好几次，只推送最后变更的最新服务实例信息呢**？**如果只推送最后变更的最新服务实例信息，那么 nacos 服务端要等待服务实例信息变更多少次之后再推送给客户端呢**？**如果多个 Service 服务下的服务实例信息都发生变更了，服务端要推送给不同的客户端最新服务实例信息，执行推送操作的时候需要启用线程池吗？只开启一个线程的话处理得过来吗**？你看，这些问题不就来了吗？如果更进一步思考，还会有更多问题等着我们。

所以说，如果直接关注操作二和操作三，那么每一位朋友都知道要执行的操作就是得到最新的服务实例信息，找出订阅了该服务的客户端，然后把最新的信息发送给客户端，这谁都知道，但问题是究竟该怎么发送呢？**难道是直接定义一个新的 NotifySubscriberRequest 请求(这个请求大家在 nacos 注册中心客户端应该都见过了，如果大家没有印象了，可以去看看注册中心客户端要用到的 NamingPushRequestHandler 请求处理器的内容)，然后把最新的服务实例信息封装到该请求中，再让服务端把请求发送给对应的客户端就行了吗**？当然，最后确实也会这么做，但是就像我之前说的，在真正发送之前，还有很多问题要讨论、解决，等所有问题都解决完毕了，才能真的开始发送服务实例信息给客户端。所以接下来，就让我们先一起分析分析**当 nacos 服务端要把最新的服务实例信息推送给客户端时，究竟应该怎么推送的问题**。当然，这个问题其实还是有些宽泛，其实我们目前更关注的是**假如 nacos 服务端保存的某个 Service 下的服务实例信息频繁发生变更，在这种情况下服务端该如何把最新的服务实例信息推送给订阅了该服务的客户端**。

对于这个问题，我的观点非常直接直接：**那就是肯定不能在服务实例信息每次变更时，nacos 服务端就直接推送给客户端一次，这就意味着要对服务端主动向客户端推送最新服务实例信息的频率加以限制**。确切地说，**那就是在某个时间范围内，如果某个 Service 服务下的服务实例信息频繁更新，nacos 服务端并不能在每次信息变更时就发送信息给客户端，这种做法对服务端资源消耗比较大**。注意，我再强调一下，我们设定的这个限制是有时间范围的，具体的时间范围还没有规定好，也许是 1 秒，也许是 0.5 秒，总之是有时间范围的；还有一点就是大家一定要理清楚，**我目前给 nacos 服务端推送服务实例信息到客户端操作设定的限制，只限于某个 Service 服务下服务实例信息频繁变更的情况**，如果有多个客户端都订阅了这个 Service 服务，那么服务端就要在短时间内对多个客户端频繁推送最新服务实例信息，这么做根本没有必要，有很多冗余操作；**但假如是多个 Service 服务下的服务实例信息几乎同时发生变更了，但多个 Service 服务下的服务实例信息变更并不频繁，那么不管有几个 Service 服务信息发生变更了，服务端仍然应该给客户端推送服务实例信息**。这个逻辑大家一定要梳理清楚，看起来可能有点绕，大家可以再仔细分析分析。

好了，回到问题本身，**刚才我已经针对那个问题提出了自己的观点，那就是当服务端的某个 Service 服务下的信息频繁发生变更时，要在一定时间内限制服务端向客户端推送最新服务实例信息的频率**。总结下来其实就是四个字：**限时等待**！让我给大家具体解释一下所谓限时等待究竟是什么意思：比如说，nacos 服务端存储了 Service 服务下的服务实例信息，在第 1 秒的时候，Service 服务下的实例信息发生变更了，这个时候服务端向订阅了该服务的客户端推送了一次最新的服务实例信息；如果我们把限制服务端向客户端推送最新服务实例信息的时间定义为 0.5 秒，这就意味着在 1—1.5 秒之间，不管服务端的 Service 服务下的实例信息怎么发生变更，变更了多少次，nacos 服务端也不会向客户端推送最新的服务实例信息。服务端只会耐心等待，等待 0.5 秒过去之后，才会再次向客户端推送最新的服务实例信息。不管期间变更了多少次，这个时候最新的服务实例信息是什么，服务端就会把这些信息发送给客户端。这个逻辑大家应该可以理解吧？

如果上面的逻辑大家都理解了，那么现在新的问题又来了，在 nacos 限时等待的时间里，服务端本身需要执行什么操作吗？难道就什么也不做，干等着时间过去 0.5 秒，然后把最新的服务实例信息推送给客户端吗？也许有朋友会觉得理论上确实可以什么都不做，只要服务端记录上一次推送某个 Service 服务下所有实例信息给订阅了该服务的客户端的时间，然后判断系统时间是否过了 0.5 秒，如果超过 0.5 秒，就可以让服务端再次把服务实例信息推送给客户端即可。这么做好像也挺简单的，但要我说这个理论根本站不住脚，稍微想想就能意识到，**要是在 0.5 秒之内服务端并没有服务实例信息发生变更呢**？或者说在服务端中一直没有服务实例信息发生变更，难道服务端也要每隔 0.5 秒就主动向客户端推送一次服务实例信息吗？这显然是在做无用功。

分析一圈之后，我相信大家都会有一个共识：**那就是不管怎么样，nacos 服务端只有在服务实例发生变更的时候才能主动向订阅了对应服务的客户端们发送最新服务实例信息，如果短时间内该服务下的服务实例信息变更特别频繁，那就只能限时等待，等过了时间限制之后，服务端才能再次向客户端发送服务实例信息**。根据刚才的分析，我们也能意识到，在服务端限时等待的时间里不能什么也不做，至少应该让服务端知道在限时等待的时间中，是否有某个 Service 下的服务实例信息频繁发生变更了，如果频繁发生变更，0.5 秒之后服务端就要把最新的服务实例信息发送给对应的客户端。**那么服务端怎么就知道在限时等待的时间中有某个 Service 下的服务实例信息频繁发生变更了呢**？

这就很好说了，接下来就让我们回到操作本身吧，大家肯定都清楚，不管是什么操作，最终肯定都是线程来执行，在 nacos 服务端保存的服务实例信息发生变更后，服务端把最新服务实例信息主动推送给客户端的操作肯定也是线程来执行。考虑到可能有多个客户端都订阅了服务端不同的 Service 服务，nacos 服务端很可能会出现要向很多客户端主动推送最新服务实例信息的情况。**如果推送服务实例信息的操作非常多，那么只启动一个线程肯定不能及时处理，最好的方法就是引入线程池，或者说是执行器**；**线程池肯定有任务队列，这样一来，只要在服务端有服务实例信息发生变更了，就可以把向客户端推送最新服务实例的操作封装成一个任务，把这个任务提交给执行器的任务队列**。假如有一个 Service 服务下的服务实例信息变更非常频繁，那么每一次变更就可以向执行器的任务队列中提交一个向客户端推送最新服务实例信息的任务，等到限时时间结束之后，服务端只需要查看执行器的任务队列中是否存在任务，如果存在就说明有服务实例信息发生变更了，那么执行器就应该执行这些任务，把最新的服务实例信息推送给对应的客户端。

看到这里，我相信每一位朋友心里都会有一个疑问，假如在 nacos 服务端有一个 Service 服务下的服务实例信息频繁发生变更，在 0.5 秒之内变更了 10 次，这样一来，肯定会向执行器提交 10 个任务，每个任务要执行的操作都是向订阅了该服务的客户端发送服务实例信息。等限时时间结束之后，难道执行器要把任务队列中的这 10 个任务都执行了吗？如果真的这么做了，那么我们分析了一大堆，引入了限时等待概念，以及执行器和任务队列，总之所做的这一切就全白费了。这不还是回到了起点，执行了一大堆冗余操作吗？所以执行器当然不能把这 10 个任务全部执行了。那这 10 个任务该怎么处理呢？接下来就是非常核心的操作了：**那就是当 nacos 服务端在限时等待的时候，如果某个 Service 服务下的服务实例信息变更非常频繁，以至于向执行器的任务队列中提交了非常多的任务，那么在服务端等待的过程中，会根据任务创建的先后，对这些任务进行合并。合并到最后剩下的那个任务，就会在限时时间结束之后，被执行器执行**。大家可以再认真梳理一下这个逻辑。

如果上面那个逻辑大家都理解了，接下来就要看看另一个问题了，那就是任务合并的时候，该以什么逻辑来合并呢？其实这个也没什么好分析的了，我们可以把 Service 服务的信息封装到任务对象中，同时给任务对象定义一个时间戳成员变量，用来代表这个任务对象的创建时间，这样一来，在合并任务的时候，就可以判断这些任务封装的 Service 服务信息是否相同，如果服务信息相同，说明这些任务要把信息推送给的客户端，都是订阅了同样的 Service 服务信息的客户端。比如说第一个任务要把服务实例信息推送给客户端 A 和客户端 B，第二个任务也要把服务实例信息推送给客户端 A 和客户端 B，这样的两个任务就可以合并。当然，在合并的时候也要查看一下这两个任务的创建时间，假如任务 1 的创建时间是第 1.1 秒，任务 2 的创建时间是 1.3 秒，限时等待时间为 0.5 秒，那么这两个任务在合并的时候就要把合并之后的任务的创建时间定义为 1.1 秒；这么做的原因也很简单，反正任务合并到最后会剩下一个任务，这个任务在被执行器执行的过程中或查找当前最新的 Service 服务下的服务实例信息发送给客户端，既然 nacos 服务端在经过限时时间之后，对 Service 服务信息的推送只会执行这一个任务了，也不会有什么冗余操作，那当然是越快执行越好。如果把合并之后的任务的创建时间定义为 1.3 秒，经过限时等待时间，合并之后的任务就会在 1.8 秒才会被执行，如果把合并之后的任务的创建时间定义为 1.1 秒，那么这个任务就会在 1.6 秒执行。总之就是越快通知客户端越好。到此为止，我就为大家把 nacos 服务端主动向客户端推送最新服务实例信息的核心逻辑分析完毕了，大家可以仔细品味品味这些流程，一定要梳理清楚了，再继续看后面的内容。

好了朋友们，文字分析已经展示完毕了，接下来就该在代码层面上实现我们刚才分析的一大堆流程了。在刚才的分析中，我们决定把服务端向客户端推送服务实例信息的操作封装成任务，提交给执行器的任务队列。所以接下来我们就要引入这个任务和执行器。定义任务其实非常简单，其实刚才我都已经告诉大家任务中的成员变量是什么了：**一个是服务端要推送的 Service 服务信息，一个就是任务的创建时间，当然，既然还要聚合任务，所以还要在任务中定义聚合任务的方法**。这个任务我已经定义完毕了，就把它定义为了 PushDelayTask 类，翻译过来就是延迟推送任务。这个应该很容易理解吧，因为推送服务实例信息的操作本来就不是立即执行的，要经过限时等待之后才可以执行。接下来我就给大家展示一下这个 PushDelayTask 类，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务类，可以看到，这个类其实继承了AbstractDelayTask类，AbstractDelayTask类的
 * 内容我也给大家展示在下面代码块中了，内容非常简单，大家可以把这两个类的内容一起看了，至于为什么要继承AbstractDelayTask类
 * 后面很快就会为大家解释
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





//下面就是AbstractDelayTask抽象类


package com.cqfy.nacos.naming.push.v2.task;



public abstract class AbstractDelayTask implements NacosTask {


    //处理两次任务的时间间隔
    private long taskInterval;

    //上次处理任务的时间，其实就是当前任务创建的时间
    private long lastProcessTime;

    //默认的两次任务之间的时间间隔
    protected static final long INTERVAL = 1000L;

    //聚合任务的方法，也就是合并任务的方法
    public abstract void merge(AbstractDelayTask task);

    public void setTaskInterval(long interval) {
        this.taskInterval = interval;
    }

    public long getTaskInterval() {
        return this.taskInterval;
    }

    public void setLastProcessTime(long lastProcessTime) {
        this.lastProcessTime = lastProcessTime;
    }

    public long getLastProcessTime() {
        return this.lastProcessTime;
    }

    //任务是否可以被处理的方法，这里就是判断距离任务被创建是否过去了规定的等待时间
    //只要超过等待时间，当前任务就可以被处理了
    @Override
    public boolean shouldProcess() {
        return (System.currentTimeMillis() - this.lastProcessTime >= this.taskInterval);
    }

}
```

上面代码块中的注释非常详细，所以我就不再重复讲解代码中的逻辑了，大家自己看看就行。当然，我也能想到，就算上面代码块中的注释非常详细，大家肯定对某些地方还是感到疑惑：**首先我们定义的这个 PushDelayTask 延迟推送任务并不是传统的任务，虽然它是一个 Task，但是这个 Task 并不是什么 Runnable，换句话说，这个任务根本没有什么 run 方法可被执行；在这个任务中，我们根本看不见哪里执行了把最新服务实例信息推送给对应的客户端的操作；而且我们也不知道聚合两个任务的方法，也就是 merge() 方法究竟在哪里被调用了；并且聚合两个任务的时候根本没有用到任务对象的 Service 成员变量，似乎任何两个 PushDelayTask 任务就可以直接聚合，这难道不会导致服务端推送给客户端的服务实例信息混乱吗**？大家心里肯定会有这些疑问，我还是那句话，先别着急，这些疑问我也都想到了，并且很快就都会被解决。毕竟现在我们只是刚刚把 PushDelayTask 延迟推送任务引入了，执行器还没有引入呢，接下来就让我们一起来看看所谓的执行器究竟是什么组件。

## 引入 PushDelayTaskExecuteEngine 延迟任务执行引擎

刚才我们说要引入可以执行 PushDelayTask 任务的执行器，如果是按照源码来说的话，要引入的组件其实并不能成为执行器，而是执行引擎。我们先不管这个新引入的组件究竟该怎么命名吧，先让我们讨论一下，这个组件中要定义什么成员变量。**我们已经知道了这个组件要承担执行器，或者说是线程池的作用，那么这个组件中肯定要定义一个任务队列，**这个不难理解吧？**任务队列用来存放提交给执行器的任务**。那么除了任务队列之外呢？还要定义什么呢？**肯定还要定义提交任务的方法**，这个不难理解吧？否则怎么把任务提交到执行器的任务队列中呢？除了这些还要再定义什么呢？**当然是最重要的处理 PushDelayTask 任务的方法啊**，不然的话提交到任务队列的任务怎么被执行呢？好了，现在我们已经给这个所谓的执行器、或者说是执行引擎定义了三部分内容：

**1 定义一个任务队列。**

**2 定义提交任务的方法。**

**3 定义处理 PushDelayTask 任务的方法。**

当然，除了以上三部分内容，执行器中肯定还要定义一些基本的常规方法，等一会我会把这些直接在代码中展示出来。好了，现在又分析了一波，接下来就该在代码层面上简单实现一下这个执行器了。其实要新引入的执行器的名称已经展示在本小节的标题中了，**这个执行器就是 PushDelayTaskExecuteEngine 组件，翻译过来就是延迟推送任务执行引擎的意思**。这个执行引擎的具体内容我也定义完毕了，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务执行引擎
 */
public class PushDelayTaskExecuteEngine{


    //存放所有延迟推送任务的map，其中key是服务的Service，value就是对应的延迟推送任务
    protected final ConcurrentHashMap<Object, AbstractDelayTask> tasks;

    //同步锁
    protected final ReentrantLock lock = new ReentrantLock();

    //构造方法
    public PushDelayTaskExecuteEngine() {

        tasks = new ConcurrentHashMap<>(32);
    }


    //得到创建的延迟推送任务的数量
    public int size() {
        lock.lock();
        try {
            return tasks.size();
        } finally {
            lock.unlock();
        }
    }

    //判断当前引擎中是否没有要执行的任务
    public boolean isEmpty() {
        lock.lock();
        try {
            return tasks.isEmpty();
        } finally {
            lock.unlock();
        }
    }


    //得到所有任务的key，也就是任务要推送的服务实例所在的Service信息
    public Collection<Object> getAllTaskKeys() {
        Collection<Object> keys = new HashSet<>();
        lock.lock();
        try {
            keys.addAll(tasks.keySet());
        } finally {
            lock.unlock();
        }
        return keys;
    }


    //其他内容暂时不做实现
    
}
```

好了，上面代码块展示了我们初步定义的 PushDelayTaskExecuteEngine 执行引擎，**大家也可以看到我在执行引擎中定义了一个 map 成员变量，而这个 map 就是所谓的任务队列**。看到这个任务队列之后，大家可能还是会觉得奇怪，好像很少见到使用 map 作为执行器的任务队列的，但这里偏偏使用了 map。这个其实很好解释，我在注释中也写明了，**map 的 key 其实就是 Service 对象，也就是任务要推送的服务实例所在的 Service 信息，value 就是真正的 PushDelayTask 任务**。这么设计其实有一个非常明显的好处，**那就是向执行引擎提交任务的时候，可以直接根据任务要推送的服务实例所在的 Service 信息，从 map 判断是否已经存在推送任务了，如果存在，正好可以将两个任务合并**。这个好处是显而易见的吧？接下来就让我为执行引擎定义添加任务的方法，也就是 addTask() 方法，具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务执行引擎
 */
public class PushDelayTaskExecuteEngine{


    //存放所有延迟推送任务的map，其中key是服务的Service，value就是对应的延迟推送任务
    protected final ConcurrentHashMap<Object, AbstractDelayTask> tasks;

    //同步锁
    protected final ReentrantLock lock = new ReentrantLock();

    //构造方法
    public PushDelayTaskExecuteEngine() {

        tasks = new ConcurrentHashMap<>(32);
    }


    //得到创建的延迟推送任务的数量
    public int size() {
        lock.lock();
        try {
            return tasks.size();
        } finally {
            lock.unlock();
        }
    }

    //判断当前引擎中是否没有要执行的任务
    public boolean isEmpty() {
        lock.lock();
        try {
            return tasks.isEmpty();
        } finally {
            lock.unlock();
        }
    }


    //得到所有任务的key，也就是任务要推送的服务实例所在的Service信息
    public Collection<Object> getAllTaskKeys() {
        Collection<Object> keys = new HashSet<>();
        lock.lock();
        try {
            keys.addAll(tasks.keySet());
        } finally {
            lock.unlock();
        }
        return keys;
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：添加任务到执行引擎中的方法，方法参数key就是Service对象
     */
    public void addTask(Object key, AbstractDelayTask newTask) {
        lock.lock();
        try {
            //首先根据key判断是否已经创建了对应的任务，如果存在对应的任务
            //说明服务端已经要给一些客户端推送key代表的服务下的服务实例信息了
            //只不过可能是给某些客户端推送，也可能是给所有订阅了该服务的客户端推送
            AbstractDelayTask existTask = tasks.get(key);
            if (null != existTask) {
                //如果得到的任务不为空，就把旧的任务和当前要处理的任务合并一下
                //现在大家应该清楚了为什么在PushDelayTask任务的merge()方法中不需要
                //判断两个任务的Service，就可以直接合并了吧？因为在这里已经实现判断了
                newTask.merge(existTask);
            }
            //把合并后的新任务添加到任务map中
            tasks.put(key, newTask);
        } finally {
            lock.unlock();
        }
    }

    

    //其他内容暂时不做实现
    
}
```

好了，到此为止，PushDelayTaskExecuteEngine 执行引擎的任务队列和提交任务的功能就实现完毕了，可以看到，**任务合并的操作在任务提交给执行引擎任务队列的过程中就被执行了**。既然向执行引擎提交任务的方法都定义完毕了，我知道这时候大家肯定想知道这个方法在哪里被调用，所以接下来我就先为大家展示一下执行引擎的 addTask() 方法的调用时机。这个就不用分析了，直接回顾上一章的内容即可，上一章我已经为大家引入了 NamingSubscriberServiceV2Impl 组件，**这个组件订阅了 ServiceChangedEvent 服务信息变更事件。当 nacos 服务端保存的某个 Service 服务下的服务实例信息发生变更时，NotifyCenter 通知中心就会接收并处理 ServiceChangedEvent 事件**。但是 NamingSubscriberServiceV2Impl 组件具体处理该事件的方法我并没有为大家实现，现在大家应该清楚了，具体处理的操作其实就是向执行引擎提交一个 PushDelayTask 延迟推送任务，具体实现请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2;



/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：这个类的对象也是一个订阅者，订阅了ServiceChangedEvent和ServiceSubscribedEvent事件
 * 这样一来，只要服务端的服务实例信息发生变更，或者有客户端订阅了服务信息，这个类的对象就发起主动推送服务实例信息给客户端的操作
 */
@org.springframework.stereotype.Service
public class NamingSubscriberServiceV2Impl extends SmartSubscriber implements NamingSubscriberService {


    //延迟推送任务执行引擎
    private final PushDelayTaskExecuteEngine delayTaskEngine;


    //构造方法
    public NamingSubscriberServiceV2Impl() {

        //在这里创建了延迟推送任务执行引擎
        this.delayTaskEngine = new PushDelayTaskExecuteEngine();
       //在这里把自己注册到通知中心
        NotifyCenter.registerSubscriber(this, NamingEventPublisherFactory.getInstance());

    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：当前订阅者订阅的事件类型
     */
    @Override
    public List<Class<? extends Event>> subscribeTypes() {
        List<Class<? extends Event>> result = new LinkedList<>();
        //订阅了服务实例变更事件
        result.add(ServiceEvent.ServiceChangedEvent.class);
        //订阅了服务订阅事件
        result.add(ServiceEvent.ServiceSubscribedEvent.class);
        return result;
    }





    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：处理具体事件的方法
     */
    @Override
    public void onEvent(Event event) {
        if (event instanceof ServiceEvent.ServiceChangedEvent) {
            //处理服务实例变更事件的方法
            ServiceEvent.ServiceChangedEvent serviceChangedEvent = (ServiceEvent.ServiceChangedEvent) event;
            //得到变更的服务元信息
            Service service = serviceChangedEvent.getService();
            //注意，如果处理的是服务实例变更事件，就应该把最新的服务实例信息推送给订阅了该服务的所有客户端
            //在这里创建了一个PushDelayTask延迟推送任务对象，该对象中封装着要推送的服务元信息
            //并且还设定了这个任务延迟推送的时间，PushConfig.getInstance().getPushTaskDelay()得到的就是延迟推送时间，其实就是限时等待时间
            //然后把这个PushDelayTask任务提交给了延迟推送任务执行引擎去处理
            //这里请大家思考一下，为什么要创建一个延迟推送任务呢？也许大家还不太清楚后续的流程如何处理，但是现在大家肯定清楚一点
            //那就是服务端把最新服务实例信息推送给客户端的操作，要被延迟执行，这是为什么呢？其实也没什么特别的原因，主要还是为了防止服务端频繁向客户端推送消息
            //减轻服务端工作压力，假如有非常多的客户端订阅了某个服务，那么服务端只要感受到这个服务发生一点变化就立刻推送给全部客户端，这涉及到的网络开销，消耗的服务器资源还是比较大的
            //如果服务实例频繁变更，比如一直有新的客户端注册了新的服务实例，那么服务端就要一直给所有订阅了服务的客户端推送消息，这显然不太合适
            //所以这里弄了一个延迟任务，在延迟期间，如果服务实例仍然发生变化，就可以把要推送的任务聚合一下，然后执行聚合后的任务，以此来减轻服务器工作压力
            //PushDelayTask这个对象就定义了聚合任务的方法
            delayTaskEngine.addTask(service, new PushDelayTask(service, PushConfig.getInstance().getPushTaskDelay()));
        } else if (event instanceof ServiceEvent.ServiceSubscribedEvent) {
            //这里处理的就是服务订阅事件
            ServiceEvent.ServiceSubscribedEvent subscribedEvent = (ServiceEvent.ServiceSubscribedEvent) event;
            //得到订阅的服务元信息
            Service service = subscribedEvent.getService();
            //我之前不是跟大家说过吗？客户端向服务端执行了订阅服务实例操作时，服务端会把最新的服务实例信息主动推送给客户端
            //这一点就在这里体现出来了，现在大家应该彻底清除了吧
            //注意，假如是服务实例订阅事件，那么哪个客户端订阅了就把服务实例最新信息推送给哪个客户端，所以这里并不是把服务实例推送给全部订阅了该服务的客户端
            //这两个时间的处理区别要理清楚
            //当然，最终的处理方式都是差不多的，都是创建一个PushDelayTask任务，然后把这个任务提交给延迟推送任务执行引擎
            delayTaskEngine.addTask(service, new PushDelayTask(service, PushConfig.getInstance().getPushTaskDelay(), subscribedEvent.getClientId()));
        }
    }



    //其他内容省略
}  
```

现在 NamingSubscriberServiceV2Impl 组件也重构完毕了，我们也知道了延迟推送任务执行引擎的 addTask() 方法是在哪里被调用的。现在还是让我们再次回到主题上来，继续讨论关于延迟推送任务执行引擎的最后一部分内容，**那就是提交到任务队列中的延迟推送任务，究竟应该怎么被执行引擎执行呢**？

这时候显然就要为执行引擎**定义处理 PushDelayTask 任务的方法了**，那么这个方法究竟该怎么定义呢？这就要仔细分析分析了，首先执行引擎要执行任务队列中的延迟推送任务，肯定要先把任务从任务队列中取出来吧？也就是从 map 成员变量中取出任务。那取出来就能立刻执行任务吗？或者在从任务队列中取出任务时要判断什么条件呢？刚才我们实现执行引擎的 addTask() 方法时已经知道了，**任务合并的操作在任务提交给执行引擎任务队列的过程中就被执行了，而合并之后的任务的创建时间也会被更新**；我们也都清楚，**延时推送任务必须得等限时时间结束之后才能执行，因此，在从执行引擎的任务队列中获取任务时，要判断这个任务是否可以被处理，也就是判断距离任务被创建是否过去了规定的等待时间，只有超过了，才能从任务队列中取出这个任务**。这个操作肯定是不可或缺的。

假如这个任务可以被处理了，那该如何处理呢？这就很好说了，这不就都是常规操作了吗？还是我们最开始总结的那几个操作：**根据更新的服务实例所在服务的信息，找到订阅了该服务的 nacos 客户端**；**更新 ServiceStorage 服务信息存储器中缓存的服务实例信息，并且从 ServiceStorage 服务信息存储器中得到最新的服务实例信息**：**最后把 nacos 客户端订阅的 Service 服务下的最新的服务实例信息全部发送给对应的 nacos 客户端**。这一点理解起来应该没问题吧。现在要执行的操作已经明确了，可以开始定义处理延迟推送任务的方法了吧？还是那句话，先别急，任务肯定是要由线程来执行的，**可是在目前的 PushDelayTaskExecuteEngine 延迟推送任务执行引擎中根本就没有执行任务的线程，或者说是根本就没有真正执行任务的执行器，所以，接下来也要先把真正执行任务的执行器给定义出来**。这个也很好解决，**那就直接在 PushDelayTaskExecuteEngine 执行引擎中定义一个定时任务执行器，让定时任务执行器定期执行定时任务，而在定时任务中，就是真正处理延时推送任务的操作**。接下来就请大家看看我重构之后的 PushDelayTaskExecuteEngine，请看下面代码块。

```
package com.cqfy.nacos.naming.push.v2.task;


/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：延迟推送任务执行引擎
 */
public class PushDelayTaskExecuteEngine{


    //存放所有延迟推送任务的map，其中key是服务的Service，value就是对应的延迟推送任务
    protected final ConcurrentHashMap<Object, AbstractDelayTask> tasks;

    //同步锁
    protected final ReentrantLock lock = new ReentrantLock();

    //定时任务执行器
    private final ScheduledExecutorService processingExecutor;

    //任务处理器，这个新引入的处理器就是真正处理延迟推送任务的组件，是执行引擎的内部类
    private NacosTaskProcessor processor;

    //构造方法
    public PushDelayTaskExecuteEngine() {

        tasks = new ConcurrentHashMap<>(32);
        //创建单线程的定时任务执行器，为什么是单线程的执行器很快就会解释
        processingExecutor = ExecutorFactory.newSingleScheduledExecutorService(new NameThreadFactory(name));
        //向定时任务执行其中提交定时任务，也就是ProcessRunnable()任务，每100毫秒执行一次
        processingExecutor.scheduleWithFixedDelay(new ProcessRunnable(), 100L, 100L, TimeUnit.MILLISECONDS);
        //在创建PushDelayTaskExecuteEngine对象的时候，就创建了一个延迟推送任务处理器对象
        this.processor = new PushDelayTaskProcessor(this);
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：添加任务到执行引擎中的方法，方法参数key就是Service对象
     */
    public void addTask(Object key, AbstractDelayTask newTask) {
        lock.lock();
        try {
            //首先根据key判断是否已经创建了对应的任务，如果存在对应的任务
            //说明服务端已经要给一些客户端推送key代表的服务下的服务实例信息了
            //只不过可能是给某些客户端推送，也可能是给所有订阅了该服务的客户端推送
            AbstractDelayTask existTask = tasks.get(key);
            if (null != existTask) {
                //如果得到的任务不为空，就把旧的任务和当前要处理的任务合并一下
                //现在大家应该清楚了为什么在PushDelayTask任务的merge()方法中不需要
                //判断两个任务的Service，就可以直接合并了吧？因为在这里已经实现判断了
                newTask.merge(existTask);
            }
            //把合并后的新任务添加到任务map中
            tasks.put(key, newTask);
        } finally {
            lock.unlock();
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：从存放任务的map中移除对应任务的方法
     */
    public AbstractDelayTask removeTask(Object key) {
        lock.lock();
        try {
            //得到对应的任务
            AbstractDelayTask task = tasks.get(key);
            //这里可以看到，在这里判断任务是否可以被处理了
            //具体逻辑就是用系统当前时间减去任务创建的时间
            //看看这个时间间隔是否超过了程序规定的时间间隔，如果超过或者等于就可以执行
            //也就是可以从map中移除任务，如果没有超过就意味着任务执行有点频繁，当前不能执行下一个任务
            if (null != task && task.shouldProcess()) {
                return tasks.remove(key);
            } else {
                return null;
            }
        } finally {
            lock.unlock();
        }
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：定时任务本身
     */
    private class ProcessRunnable implements Runnable {

        @Override
        public void run() {
            try {
                //在定时任务中会调用processTasks方法处理map中的延迟推送任务
                processTasks();
            } catch (Throwable e) {
                getEngineLog().error(e.toString(), e);
            }
        }
    }


    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/22
    * @方法描述：处理器处理延迟推送人物的方法
    */
    protected void processTasks() {
        //得到所有延迟推送任务的key，其实就是得到了延迟推送任务对应的Service
        Collection<Object> keys = getAllTaskKeys();
        for (Object taskKey : keys) {
            //根据key从map中得到对应的延迟推送任务
            AbstractDelayTask task = removeTask(taskKey);
            if (null == task) {
                //对任务判空，任务为空则跳过本次循环
                continue;
            }
            
            try {
                //处理器处理延迟推送任务
                if (!processor.process(task)) {
                    //处理失败则重新处理
                    retryFailedTask(taskKey, task);
                }
            } catch (Throwable e) {
                getEngineLog().error("Nacos task execute error ", e);
                retryFailedTask(taskKey, task);
            }
        }
    }


    //得到所有任务的key
    public Collection<Object> getAllTaskKeys() {
        Collection<Object> keys = new HashSet<>();
        lock.lock();
        try {
            keys.addAll(tasks.keySet());
        } finally {
            lock.unlock();
        }
        return keys;
    }


    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/22
    * @方法描述：重新执行失败任务的方法
    */
    private void retryFailedTask(Object key, AbstractDelayTask task) {
        //设置任务最后被处理的时间为系统当前时间
        task.setLastProcessTime(System.currentTimeMillis());
        //把任务提交到执行引擎的map中
        addTask(key, task);
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
            
            //真正处理任务的操作暂时不做实现
        }
    }


    

    //其他内容暂时省略
    
}
```

上面代码块的注释非常详细，而且之前也用分析把处理任务的思路分析清楚了，所以我就不再重复讲解上面代码块的逻辑了，大家自己看看就行，逻辑非常简单，都是很常规的逻辑。如果以上的逻辑大家都理解了，也对我们本章引入的延迟推送任务，延迟推送任务执行引擎有了基本认识，那么接下来我想告诉大家，实际上在 nacos 服务端源码中，PushDelayTaskExecuteEngine 执行引擎并不是我们这样定义的。当然，我为大家分析的定义 PushDelayTaskExecuteEngine 执行引擎的思路并没有问题，这一点和源码一致，但是最终呈现出来的样子和源码差别比较大。**我知道现在大家肯定很关心 PushDelayTaskProcessor 延迟推送任务执行器的 process() 方法究竟该怎么实现，这样大家才能知道延迟推送任务究竟该怎么被处理**；但是我不得不先把话题岔开一下，为大家详细分析一下 nacos 中的执行引擎体系是怎么设计的，以及为什么这么设计。

## 详细剖析 nacos 服务端的执行引擎体系

  

看到我又要开始分析一些莫名其妙的体系，也许有的朋友会觉得很枯燥，与其分析什么体系，倒不如直接上代码来的直接。是的，我也很讨厌面对着枯燥的文字，也不喜欢这种感觉，我在学一下个框架的时候也是能尽快看源码就尽快看源码。但是，有一点我必须解释清楚，那就是我喜欢直接看代码是因为我在看代码之前大概理解了设计思路，或者自己能猜到设计思路。我一直觉得代码是最简单的，是编程思路的体现，是设计原理的体现，深入理解了设计原理，看代码或者对框架二次开发的时候才会游刃有余。在我为大家更新 netty 课程时，我就不断跟大家强调：到最后，代码肯定会被忘记，但是设计原理会深深印在你脑海里。当然，话又说回来，我们又不得不承认，很多时候只有先看了代码，才能跟深入理解框架某个体系的设计原理。这也是无可奈何的事，当学习资料特别少的时候，只能直面代码，一点点琢磨。但现在我已经把设计原理掌握了，正在更新给大家，所以还是请大家耐心一些，把这些文字看完，我敢保证，这对大家之后的学习绝对有好处。**因为到了后面为 nacos 服务端引入集群模式时，在集群模式下执行各个任务使用的仍然是我即将为大家分析的引擎体系，现在掌握了这一块的知识，等到构建了 Distro 集群后，在集群中执行各种任务，基本上不用怎么看文章，大家直接看代码就能梳理清楚程序的执行流程和内在逻辑**。好了，话不多说，就让我尽快为大家分析一下所谓的 nacos 的执行引擎体系吧。

首先请大家思考一个问题：**刚才我们定义的 PushDelayTaskExecuteEngine 执行引擎的作用究竟是什么**？我问这个问题是有原因的，因为我发现在我为 nacos 服务端引入了这个 PushDelayTaskExecuteEngine 执行引擎之后，这个引擎在真正执行延迟推送任务的时候并没有发挥多大的作用。之前我已经跟大家说过，考虑到 nacos 服务端可能会向多个客户端推送服务实例信息，也就意味着会向执行引擎提交多个延迟执行任务，这些任务要想被及时处理，只用一个线程来执行恐怕是不够的。但是在我们定义的 PushDelayTaskExecuteEngine 执行引擎中，偏偏就是使用了一个单线程的执行器来定期处理任务队列中的任务。这么做显然不太合适，但是我定义的这个 PushDelayTaskExecuteEngine 执行引擎是仿照 nacos 源码实现的，既然源码使用的就是单线程执行器，肯定有它自己的原因，所以我们就该思考一下，为什么要这么做。

这时候其实就要从 PushDelayTaskExecuteEngine 执行引擎本身的作用入手了，请大家想一想，我们为这个执行引擎定义了任务队列，定义了单线程定时任务执行器，还定义了提交任务的方法，还定义了从任务队列中取出任务的方法。我们所做的这些努力，其实到最后只是完成了执行 PushDelayTask 延迟任务的前置操作：**那就是把相同的任务合并，提交到任务队列中，然后把到期的任务从任务队列中取出来，或者说把经过了限时时间的任务从任务队列中取出来，交给单线程定时任务执行器来执行**。而我们刚才分析了，只是用单线程执行器来执行众多的 PushDelayTask 任务，效率肯定不高。所以，单线程执行 PushDelayTask 任务的时候，并不能执行我们之前总结的常规操作，也就是说单线程执行器并不能直接把服务实例信息发送给对应的客户端，**而是应该引入一个新的执行器，这个执行器中使用多线程执行任务，最后也就是由这个新的执行器把服务实例信息发送给客户端。这样一来，程序执行的效率就会大大提高**。现在大家应该清楚了，**其实在引入了 PushDelayTaskExecuteEngine 延迟任务执行引擎之后，还需要再引入一个新的执行引擎，这个执行引擎会并发执行任务，把服务实例信息推送给对应的客户端**。

这样分析完毕之后，现在我可以为大家总结一下 PushDelayTaskExecuteEngine 执行引擎的真正作用了：**这个执行引擎的真正作用就是合并相同的任务，等待任务度过限时等待时间，然后把任务提交给可以并发处理所有任务的新的执行引擎**。之后任务怎么被处理就是新引入的执行引擎的工作了，现在我们还不比讨论。

  

那么新引入的这个执行引擎究竟是什么呢？这时候我就不卖关子了，PushDelayTaskExecuteEngine 是延迟推送任务执行引擎，这个引擎主要作用就是等待任务延迟时间结束，延迟时间结束之后，任务就要被执行了，**所以新引入的这个执行器就被定义为了 NacosExecuteTaskExecuteEngine，也就是专门执行需要即刻被执行的引擎**。大家可以仅仅从设计原理上品味品味这个逻辑，下一章我再从代码层面上实现这部分的功能，引入这个新的 NacosExecuteTaskExecuteEngine 执行引擎。

如果大家理解了上面的逻辑，那么接下来我还要再给大家扩展一下，实际上在 nacos 服务端源码中，有很多执行引擎。**而所有的执行引擎都有一个抽象父类，那就是 AbstractNacosTaskExecuteEngine 类，这个抽象类中定义了引擎的一些基本方法，并且还会往下继续细分，刚才我们新定义了一个 NacosExecuteTaskExecuteEngine 立即执行任务的引擎，与这个引擎相对的就是延迟执行任务的引擎，也就是 NacosDelayTaskExecuteEngine**。这几个类的关系如下，请看下面简图。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1726822952169-6faf5f29-2a4e-417b-b9bb-b13c62cfa6b6.jpeg)

这些引擎内部具体怎么定义大家先不用关心，先跟着我的思路往下走即可。好了，现在我已经从 nacos 源码中为大家展示了三个引擎了，到此为止，还是没看见我们本章实现的 PushDelayTaskExecuteEngine 延迟推送任务执行引擎，别急，很快就轮到它登场了。实际上，这个 PushDelayTaskExecuteEngine 延迟推送任务执行引擎就是 NacosDelayTaskExecuteEngine 的子类，类关系如下，请看下面简图。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1726823820043-03a49b01-7a4b-4bc5-a1c9-258eefc2cb26.jpeg)

现在我就把本章要用到的所有执行引擎都展示完毕了，**在上一小节结尾我跟大家说 nacos 源码的 PushDelayTaskExecuteEngine 类和我们文章中展示的并不一样，就是因为源码中 PushDelayTaskExecuteEngine 类还有两个父类。文章中在 PushDelayTaskExecuteEngine 类中展示的部分内容实际上是定义在它的父类中的，内容没有一点改变，只是位置变了一下而已**。但具体怎么定义，也就是 PushDelayTaskExecuteEngine 具体怎么重构成和源码一致，我就不在文章中为大家展示了，大家只要理解了这几个引擎的关系，很容易就能看明白我提供的第九版本中的相关代码。这些内容就留给大家自己去第九版本代码中查看吧。

现在简图也展示清楚了，**现在我再跟大家解释，延迟执行任务引擎的作用只是合并任务，等待任务延迟时间结束，然后把任务提交给立即执行任务的引擎，而立即执行任务的引擎才是真正执行处理任务的引擎**，这时候大家应该理解这究竟是怎么回事了吧？当然，这时候大家肯定还会很好奇上面简图中残缺的部分，大家可能会疑惑和 PushDelayTaskExecuteEngine 引擎对应的延迟任务执行引擎是什么，这里我就给大家彻底解释清楚了，反正下个版本代码就要构建 Distro 集群，就当是提前给大家铺垫一下吧。

请大家想一想，假如我们为 nacos 服务端成功构建了 Distro 集群，那么服务端集群中就会有多个节点，Distro 集群的每一个节点都可以接收来自客户端的信息，假如某个客户端向 Distro 集群中的某个节点注册了新的服务实例信息，集群直接各个节点的数据肯定是要备份的，因此接收了新服务实例信息的节点就要把最新的信息同步给集群其他节点，这个同步的操作也会被封装成一个任务(**现在大家应该清楚为什么 PushDelayTask 要继承 AbstractDelayTask 抽象父类了吧？因为 AbstractDelayTask 类还会向下继续细分，还有 DistroDelayTask 类也要继承 AbstractDelayTask 类，DistroDelayTask 类的对象就是 Distro 集群中的任务**)，交给专门处理 Distro 集群任务的引擎来执行。那假如某个节点的信息变更特别频繁，每次变更都要向执行引擎提交一个任务，这种情况下是不是也要把任务合并一下，定义一个限时等待时间？因此，Distro 集群也有自己的延迟推送任务，以及延迟推送任务执行引擎。当然，**延迟时间结束之后，延迟任务执行引擎就会把任务提交给立即执行任务的引擎，也就是提交给 NacosExecuteTaskExecuteEngine 引擎**。根据这段分析，接下来就让我为大家把执行引擎体系的类关系简图补充完整，请看下面简图。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1726825622001-9749f300-b75a-4149-98e7-376c52ee8630.jpeg)

到此为止，本章的内容就全部结束了，可以看到内容确实很多，篇幅有限，有些内容也没有彻底展开来讲解和实现，大家在看文章的时候可以结合代码一起看。现在第九版本的部分代码大家都可以看明白了，至少有一半都可以看明白。剩下的那一半，等下一章结束之后，大家就全都可以去看了。在文章的最后，我还是把按照 nacos 服务端源码重构之后的 PushDelayTaskExecuteEngine 类给大家展示一下吧，请看下面代码块。

首先是 PushDelayTaskExecuteEngine 延迟推送任务执行引擎。

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
            
            //真正处理任务的操作暂时不做实现
        }
    }


    

    //其他内容暂时省略
    
}
```

然后是 NacosDelayTaskExecuteEngine 父类。

```
package com.cqfy.nacos.common.task.engine;



/** 
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。 
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：PushDelayTaskExecuteEngine延迟推送任务引擎的父类，这个类还有一个子类，那就是DistroDelayTaskExecuteEngine集群延迟任务执行引擎
 * 在第九版本我们只引入了延迟推送任务执行引擎。延迟推送任务引擎负责向客户端推送服务实例信息，而集群延迟任务执行引擎负责向distro同步数据
 */ 
public class NacosDelayTaskExecuteEngine extends AbstractNacosTaskExecuteEngine<AbstractDelayTask> {


    //存放所有延迟推送任务的map，其中key是服务的Service，value就是对应的延迟推送任务
    protected final ConcurrentHashMap<Object, AbstractDelayTask> tasks;

    //同步锁
    protected final ReentrantLock lock = new ReentrantLock();

    //定时任务执行器
    private final ScheduledExecutorService processingExecutor;


    //构造方法
    public NacosDelayTaskExecuteEngine() {
        super(null);
        tasks = new ConcurrentHashMap<>(32);
        //创建单线程的定时任务执行器，为什么是单线程的执行器很快就会解释
        processingExecutor = ExecutorFactory.newSingleScheduledExecutorService(new NameThreadFactory(name));
        //向定时任务执行其中提交定时任务，也就是ProcessRunnable()任务，每100毫秒执行一次
        processingExecutor.scheduleWithFixedDelay(new ProcessRunnable(), 100L, 100L, TimeUnit.MILLISECONDS);
    }



    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：添加任务到执行引擎中的方法，方法参数key就是Service对象
     */
    @Override
    public void addTask(Object key, AbstractDelayTask newTask) {
        lock.lock();
        try {
            //首先根据key判断是否已经创建了对应的任务，如果存在对应的任务
            //说明服务端已经要给一些客户端推送key代表的服务下的服务实例信息了
            //只不过可能是给某些客户端推送，也可能是给所有订阅了该服务的客户端推送
            AbstractDelayTask existTask = tasks.get(key);
            if (null != existTask) {
                //如果得到的任务不为空，就把旧的任务和当前要处理的任务合并一下
                //现在大家应该清楚了为什么在PushDelayTask任务的merge()方法中不需要
                //判断两个任务的Service，就可以直接合并了吧？因为在这里已经实现判断了
                newTask.merge(existTask);
            }
            //把合并后的新任务添加到任务map中
            tasks.put(key, newTask);
        } finally {
            lock.unlock();
        }
    }


    /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：从存放任务的map中移除对应任务的方法
     */
    @Override
    public AbstractDelayTask removeTask(Object key) {
        lock.lock();
        try {
            //得到对应的任务
            AbstractDelayTask task = tasks.get(key);
            //这里可以看到，在这里判断任务是否可以被处理了
            //具体逻辑就是用系统当前时间减去任务创建的时间
            //看看这个时间间隔是否超过了程序规定的时间间隔，如果超过或者等于就可以执行
            //也就是可以从map中移除任务，如果没有超过就意味着任务执行有点频繁，当前不能执行下一个任务
            if (null != task && task.shouldProcess()) {
                return tasks.remove(key);
            } else {
                return null;
            }
        } finally {
            lock.unlock();
        }
    }


    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/22
    * @方法描述：处理器处理延迟推送人物的方法
    */
    protected void processTasks() {
        //得到所有延迟推送任务的key，其实就是得到了延迟推送任务对应的Service
        Collection<Object> keys = getAllTaskKeys();
        for (Object taskKey : keys) {
            //根据key从map中得到对应的延迟推送任务
            AbstractDelayTask task = removeTask(taskKey);
            if (null == task) {
                //对任务判空，任务为空则跳过本次循环
                continue;
            }
            //调用父类的getProcessor()方法，得到对应的任务处理器，这里得到的就是之前创建的PushDelayTaskProcessor处理器
            NacosTaskProcessor processor = getProcessor(taskKey);
            try {
                //处理器处理延迟推送任务
                if (!processor.process(task)) {
                    //处理失败则重新处理
                    retryFailedTask(taskKey, task);
                }
            } catch (Throwable e) {
                getEngineLog().error("Nacos task execute error ", e);
                retryFailedTask(taskKey, task);
            }
        }
    }


    //得到所有任务的key
    @Override
    public Collection<Object> getAllTaskKeys() {
        Collection<Object> keys = new HashSet<>();
        lock.lock();
        try {
            keys.addAll(tasks.keySet());
        } finally {
            lock.unlock();
        }
        return keys;
    }


    /**
    * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
    * @author：陈清风扬，个人微信号：chenqingfengyangjj。
    * @date:2024/7/22
    * @方法描述：重新执行失败任务的方法
    */
    private void retryFailedTask(Object key, AbstractDelayTask task) {
        //设置任务最后被处理的时间为系统当前时间
        task.setLastProcessTime(System.currentTimeMillis());
        //把任务提交到执行引擎的map中
        addTask(key, task);
    }


     /**
     * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
     * @author：陈清风扬，个人微信号：chenqingfengyangjj。
     * @date:2024/7/22
     * @方法描述：定时任务本身
     */
    private class ProcessRunnable implements Runnable {

        @Override
        public void run() {
            try {
                //在定时任务中会调用processTasks方法处理map中的延迟推送任务
                processTasks();
            } catch (Throwable e) {
                getEngineLog().error(e.toString(), e);
            }
        }
    }



    //其他内容暂时省略
}
```

最后是 AbstractNacosTaskExecuteEngine 抽象父类。

```
package com.cqfy.nacos.common.task.engine;




/**
 * @课程描述:从零带你写框架系列中的课程，整个系列包含netty，xxl-job，rocketmq，nacos，sofajraft，spring，springboot，disruptor，编译器，虚拟机等等。
 * @author：陈清风扬，个人微信号：chenqingfengyangjj。
 * @date:2024/7/22
 * @方法描述：nacos任务执行引擎抽象父类
 */
public abstract class AbstractNacosTaskExecuteEngine<T extends NacosTask> implements NacosTaskExecuteEngine<T> {

    private final Logger log;

    private final ConcurrentHashMap<Object, NacosTaskProcessor> taskProcessors = new ConcurrentHashMap<>();

    //任务处理器
    private NacosTaskProcessor defaultTaskProcessor;

    public AbstractNacosTaskExecuteEngine(Logger logger) {
        this.log = null != logger ? logger : LoggerFactory.getLogger(AbstractNacosTaskExecuteEngine.class.getName());
    }

    @Override
    public void addProcessor(Object key, NacosTaskProcessor taskProcessor) {
        taskProcessors.putIfAbsent(key, taskProcessor);
    }

    @Override
    public void removeProcessor(Object key) {
        taskProcessors.remove(key);
    }

    @Override
    public NacosTaskProcessor getProcessor(Object key) {
        return taskProcessors.containsKey(key) ? taskProcessors.get(key) : defaultTaskProcessor;
    }

    @Override
    public Collection<Object> getAllProcessorKey() {
        return taskProcessors.keySet();
    }

    @Override
    public void setDefaultTaskProcessor(NacosTaskProcessor defaultTaskProcessor) {
        this.defaultTaskProcessor = defaultTaskProcessor;
    }

    protected Logger getEngineLog() {
        return log;
    }
}
```

可以看到，虽然把最开始的 PushDelayTaskExecuteEngine 类按照源码拆分成三个类了，但是三个类中的方法和成员变量和文章中最开始定义的 PushDelayTaskExecuteEngine 类中的内容没什么区别，只是方法和成员变量的位置稍微挪动了一下而已。当然，上面展示的三个类中部分细节还是和我提供的第九版本代码不太一致，如果大家还有精力，可以先去我提供的第九版本代码中查看对比一下。到此为止，本章的内容才算真正结束了，唯一留下没有实现的就是 PushDelayTaskExecuteEngine 类中的 PushDelayTaskProcessor 处理器，就是下面代码块中展示的处理器，请看下面代码块。

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
            
            //真正处理任务的操作暂时不做实现
        }
    }


    

    //其他内容暂时省略
    
}
```

这个 PushDelayTaskProcessor 处理器究竟该怎么实现，这就是下一章的内容了，反正现在我们已经知道 PushDelayTaskExecuteEngine 执行引擎并不是直接执行任务，而是把任务提交给 NacosExecuteTaskExecuteEngine 立即执行任务的引擎，但究竟该怎么提交，就留到下一章讲解吧。好了朋友们，我们下一章见！