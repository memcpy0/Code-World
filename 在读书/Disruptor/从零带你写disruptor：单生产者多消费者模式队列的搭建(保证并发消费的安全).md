朋友们，大家好，距离上一章更新已经过去了一个星期了，今天我就为大家继续更新 Disruptor 框架的第八章。因为前七章讲解的都是单生产者单消费者模式下的本地队列的搭建，并且已经搭建起了一个非常完善的骨架，实际上，这个骨架的细节，我也为大家填充得差不多了，也是时候继续向前推进我们自己搭建的框架了。所以，这一章，我会为大家详细分析一下，在单生产者多消费者模式下，本地队列该增添哪些新的功能和组件，以及这些功能是如何实现的。

**多消费者带来的并发问题**

在上一章我为大家最终搭建起了一个单生产者单消费者模型的本地队列，并且为大家提供了测试类，详细分析了队列的工作流程。已经过去一个星期了，如果大家没什么印象了，我就把上一章的测试类搬运过来，帮助大家简单回顾一下。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/8
 * @Description:单消费者单生产者的测试例子
 */
public class SingleConsumerTest {


    public static void main(String[] args) {
        //首先把事件工厂创建出来，这里可以看出泛型是String
        //这也就意味着Event中的data就是String
        SimpleEventFactory<String> eventFactory = new SimpleEventFactory<>();
        
        //设置环形数组的长度，其实就是RingBuffer中数组的容量。注意，这个环形数组的长度必须是2的n次幂
        int ringBufferSize = 128;
        
        //创建线程工厂，这个线程工厂是专门创建消费者线程的
        ThreadFactory threadFactory = Executors.defaultThreadFactory();
        
        //设置等待策略
        WaitStrategy waitStrategy = new SleepingWaitStrategy();
       
        //创建disruptor，注意，这里使用的就是单生产者模式
        Disruptor<Event<String>> disruptor = new Disruptor<>(eventFactory, ringBufferSize, threadFactory, SINGLE, waitStrategy);
        
        //创建消费者处理器，因为是单消费者模式，所以只创建一个消费者处理器即可
        EventHandler<Event<String>> eventHandler = new SimpleEventHandler<>();
       
        //把消费者设置到disruptor中，然后被包装成一个BatchEventProcessor对象
        disruptor.handleEventsWith(eventHandler);
        
        //创建用户自己定义的异常处理器
        ExceptionHandler<Event<String>> exceptionHandler = new SimpleExceptionHandler<>();
        
        //把异常处理器设置到程序内部，替换程序内部默认的异常处理器
        disruptor.setDefaultExceptionHandler(exceptionHandler);
        
        //现在就可以启动程序了，注意，启动程序，其实就是启动的消费者线程，但是现在很明显还没有数据被放到环形数组中
        //消费者是没有数据可以消费的，所以就会根据等待策略等待生产者的数据到来
        disruptor.start();
        
        //下面这个是一个事件转换器，就是把用户定义的字符串给Event对象的data属性赋值，这样，Event对象就终于完整了
        //在程序内部，Event对象一旦创建出来就不会销毁了，只要程序不终止就会一直存在，而真正要被消费的数据，是
        //Event对象中的data字符串，字符串会当作生产者生产的数据放进程序内部，也就是放到Event对象中
        EventTranslatorOneArg<Event<String>, String> eventTranslatorOneArg =
                new EventTranslatorOneArg<Event<String>, String>() {
                    @Override
                    public void translateTo(Event<String> event, long sequence, String arg0) {
                        event.setData(arg0);
                    }
                };
        
        
        //发布十条数据，生产者一旦发布了数据，消费者就会直接去获取数据然后消费了
        for (int i = 0; i < 10; i++) {
            disruptor.publishEvent(eventTranslatorOneArg, "第"+i+"条");
        }
        
        //终止程序
        disruptor.shutdown();
    }
}
```

在上面的代码块中，因为是单消费者模型，所以我就定义了一个 SimpleEventHandler 消费者处理器。当程序启动之后，这个消费者处理器会被包装在 BatchEventProcessor 对象中，然后消费者就可以等待生产者生产数据，一旦生产了数据，消费者就可以去消费了。我为什么要把这个测试类搬运过来呢？除了帮助大家简单回顾一下之前的知识，我还希望能通过这个测试类对之前的知识做一下延伸。虽然我在第七章并没有给大家具体展示出来，但是我想大家在结合文章学习第一版本代码的时候，肯定也能意识到，在第一版本代码的测试类中，用户并非只能创建一个消费者处理器。实际上，用户完全可以创建多个消费者处理器，这些消费者处理器在程序内部都会被包装成一个个 BatchEventProcessor 对象。假如生产者生产了 10 条数据，用户定义了 5 个消费者处理器，那么当程序启动之后，这 5 个消费者处理器就会分别去消费这 10 条生产者数据。虽说在上一章我没有明确给出测试类，但是我在为大家重构单生产者序号生成器 SingleProducerSequencer 类的时候，引入了 AbstractSequencer 类，并且在这个类中使用一个存放所有消费者进度的 gatingSequences 数组，取代了之前在 SingleProducerSequencer 中定义的消费者进度序号 consumer。具体实现请看下面代码块。

```
public abstract class AbstractSequencer implements Sequencer {
    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/19
     * @Description:原子更新器，这个原子更新器更新的就是gatingSequences数组，gatingSequences就是该类
     * 的一个成员变量，用于寻找消费者中最慢的那个消费进度，这个成员变量后面会用到，大家要对它有个印象
     */
    private static final AtomicReferenceFieldUpdater<AbstractSequencer, Sequence[]> SEQUENCE_UPDATER =
            AtomicReferenceFieldUpdater.newUpdater(AbstractSequencer.class, Sequence[].class, "gatingSequences");

    //下面这几个成员变量就是原来我定义在SingleProducerSequencer中的成员变量，在真正的源码中
    //它们是被定义在抽象父类中的
    
    //环形数组的容量
    protected final int bufferSize;
    //消费者的等待策略
    protected final WaitStrategy waitStrategy;
    //生产者的当前进度，也就是生产者分配到第几个序号了
    protected final Sequence cursor = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);
    
    //序号数组，这个数组中存储的都是消费者的消费进度，通过这个数组，就能找到所有消费者中，消费最慢的那个进度
    //这个就是上面讲到的gatingSequences数组，取代了之前在SingleProducerSequencer中定义的消费者进度序号consumer
 	protected volatile Sequence[] gatingSequences = new Sequence[0];

    //其他内容省略
}
```

在上面代码块的第 24 行，就是最新定义的用来存放所有消费者进度的数组。这个数组的出现，其实就已经帮助大家把我们搭建的队列过度到了单生产者和多消费者模式。至于这个数组的具体作用，我在上一章也讲解过了。如果有的朋友已经忘了，可以通过下面的内容简单回顾一下。

![](https://cdn.nlark.com/yuque/0/2023/png/26725125/1696845076059-3735db26-2098-481c-9de5-1792a882f31f.png)

至于这些消费者消费进度并不一致，原因就很简单的，最直接的原因就是线程被 CPU 调度的时间和次数并不相同。因为用户定义的每一个消费处理器都会被包装成一个 BatchEventProcessor 对象，该对象其实就是一个 Runnable，最后要被线程来执行。也就是说，用户定义的每一个消费者处理器都需要一个线程来执行，线程的调度并非是程序员可以控制的，所以有的消费者消费的进度快，有的消费者消费的进度慢。如果大家理解了这个逻辑，接下来，我就为大家提供一个测试类，这个测试类就是单生产者多消费者模式的测试类。请看下面代码块。

```
public class MultiConsumerTest {


    public static void main(String[] args) {
        //首先把事件工厂创建出来，这里可以看出泛型是String
        //这也就意味着Event中的data就是String
        SimpleEventFactory<String> eventFactory = new SimpleEventFactory<>();
        
        //设置环形数组的长度，其实就是RingBuffer中数组的容量。注意，这个环形数组的长度必须是2的n次幂
        int ringBufferSize = 128;
        
        //创建线程工厂，这个线程工厂是专门创建消费者线程的
        ThreadFactory threadFactory = Executors.defaultThreadFactory();
        
        //设置等待策略
        WaitStrategy waitStrategy = new SleepingWaitStrategy();
        
        //创建disruptor，注意，这里使用的就是单生产者模式
        Disruptor<Event<String>> disruptor = new Disruptor<>(eventFactory, ringBufferSize, threadFactory, SINGLE, waitStrategy);


        
        //上面的逻辑都很简单，也和上一章测试类的逻辑相同，下面就有点不一样了
        //创建消费者处理器，因为是多消费者模式，所以创建两个消费者处理器
        EventHandler<Event<String>> eventHandler = new SimpleEventHandler<>();
        EventHandler<Event<String>> eventHandler1 = new SimpleEventHandler<>();
        //把消费者设置到disruptor中，然后被包装成一个个BatchEventProcessor对象
        //handleEventsWith方法要传入的本来就是一个可变参数，所以可以穿入两个消费处理器
        //这一点在上一章也已经分析过了
        disruptor.handleEventsWith(eventHandler,eventHandler1);



        //下面的逻辑和上一章测试类的逻辑仍然相同
        //创建用户自己定义的异常处理器
        ExceptionHandler<Event<String>> exceptionHandler = new SimpleExceptionHandler<>();
        
        //把异常处理器设置到程序内部，替换程序内部默认的异常处理器
        disruptor.setDefaultExceptionHandler(exceptionHandler);
        
        //现在就可以启动程序了，注意，启动程序，其实就是启动的消费者线程，但是现在很明显还没有数据被放到环形数组中
        //消费者是没有数据可以消费的，所以就会根据等待策略等待生产者的数据到来
        disruptor.start();

        
        //下面这个是一个事件转换器，就是把用户定义的字符串给Event对象的data属性赋值，这样，Event对象就终于完整了
        //在程序内部，Event对象一旦创建出来就不会销毁了，只要程序不终止就会一直存在，而真正要被消费的数据，是
        //Event对象中的data字符串，字符串会当作生产者生产的数据放进程序内部，也就是放到Event对象中
        EventTranslatorOneArg<Event<String>, String> eventTranslatorOneArg =
                new EventTranslatorOneArg<Event<String>, String>() {
                    @Override
                    public void translateTo(Event<String> event, long sequence, String arg0) {
                        event.setData(arg0);
                    }
                };
        //发布十条数据，生产者一旦发布了数据，消费者就会直接去获取数据然后消费了
        for (int i = 0; i < 10; i++) {
            disruptor.publishEvent(eventTranslatorOneArg, "第"+i+"条");
        }

        //这里其实可以让主线程睡一会再终止程序
        //终止程序
        disruptor.shutdown();
    }
}
```

启动程序之后，得到了下面的测试结果。请看下面代码块。

```
消费者线程pool-1-thread-2已经消费了第0条数据了！
消费者线程pool-1-thread-1已经消费了第0条数据了！
消费者线程pool-1-thread-2已经消费了第1条数据了！
消费者线程pool-1-thread-2已经消费了第2条数据了！
消费者线程pool-1-thread-2已经消费了第3条数据了！
消费者线程pool-1-thread-1已经消费了第1条数据了！
消费者线程pool-1-thread-1已经消费了第2条数据了！
消费者线程pool-1-thread-2已经消费了第4条数据了！
消费者线程pool-1-thread-2已经消费了第5条数据了！
消费者线程pool-1-thread-2已经消费了第6条数据了！
消费者线程pool-1-thread-2已经消费了第7条数据了！
消费者线程pool-1-thread-1已经消费了第3条数据了！
消费者线程pool-1-thread-1已经消费了第4条数据了！
消费者线程pool-1-thread-1已经消费了第5条数据了！
消费者线程pool-1-thread-1已经消费了第6条数据了！
消费者线程pool-1-thread-1已经消费了第7条数据了！
消费者线程pool-1-thread-1已经消费了第8条数据了！
消费者线程pool-1-thread-1已经消费了第9条数据了！
消费者线程pool-1-thread-2已经消费了第8条数据了！
消费者线程pool-1-thread-2已经消费了第9条数据了！

Process finished with exit code 0
```

如果将上面结果按顺序简单排列一下之后，会发现用户定义了两个消费处理器，对应的就会在程序中创建了两个消费者线程，生产者生产了 10 条数据，那么这两个消费者线程会各自去消费这 10 条数据。注意，这时候是每一个线程都会消费 10 条数据，互相不会干扰对方的消费进度。这就是最简单也是最基础的一种多消费者模式。我把它称为并行消费者模式，所谓并行消费，就意味着程序中所有的消费者线程消费的数据互补干扰，程序内部有多少条生产者数据，每一个消费者线程就会消耗多少数据。

但是，有时候我们还会面临另一种情况，那就是当生产的数据太多，要尽快消费完这些数据的时候，就会多创建一些消费者线程，这些消费者线程不再是并行消费了，而是共同并发消费这些生产者数据。举一个简单的例子，假如现在程序中有 1000 条生产者数据，只让一个线程消费这些数据速度太慢了，因为生产者还在源源不断地生产数据，消费者消费得太慢，环形数组中迟早没有空间存放新的生产者数据。这时候我就可以多创建几个线程，比如创建 20 个线程，让这 20 个线程共同消费这 1000 条生产者数据。既然要创建 20 个消费者线程，就意味着用户要在程序中定义 20 个消费处理器，当然，每个消费处理器的逻辑都是相同的，都是消费生产者数据。只不过在消费的时候，这些消费处理器不能消费相同的数据，并且要保证在这个阶段，这 20 个消费者处理器消费的总的生产者数据为 1000 条。至于哪个消费者消费的多，哪个消费者消费的少，程序员就不必操心了，这就交给调度消费者线程的 CPU 来负责吧。我把这种多消费者模式称为并发消费者模式，这种模式也是我们这一章要讲解的主题。因为和刚才的并行消费者模式相比，并发消费者模式显然很容易出现并发问题，说得直接一点，当多个线程去消费同一批数据，并且是并发消费，就意味着会同时操作这些数据，如果没有一些同步措施作为保障，很可能就会出现并发问题。而解决这些并发问题，就是接下来要讲解的内容。

**引入 WorkerPool 和 WorkProcessor**

解决并发问题的方法，我想大家应该非常熟悉了，无非就是使用同步锁，或者使用 CAS，再或者就是使用线程的 ThreadLocalMap 等等。也许有的朋友已经迫不及待想看我使用代码重构程序了。但还是让我说一句吧，现在就从代码层面上重构程序，未免有些操之过急。因为有一个最直接的问题我们还没有解决，请大家想一想，如果要使用同步锁保证并发安全，这个同步锁应该加在哪里呢？如果我们连同步锁加在哪里都不知道，怎么重构程序呢？所以，要想重构代码，首先得知道同步锁应该加在程序的什么地方。接下来，就让我为大家分析一下吧。

请大家仔细思考思考，当我们的程序过渡到并发消费者模式之后，现在的程序和之前的程序有什么不同？我们现在知道的是，不管是单生产者单消费者模式还是单生产者并行消费者模式，它们的消费者线程的工作流程都是一样的。每一个消费者都持有一个序号屏障，通过序号屏障可以知道当前的生产者进度，也就能知道可以消费到第几个进度了，而生产者则根据所有消费者中消费最慢的进度来存放生产者数据。在这两种模式下，每个消费者线程根本不必关心其他消费者线程的消费进度，它们消费到多少了与自己无关，自己只需要按照自己的进度消费即可，并且每一个消费者线程也不会干扰其他消费者线程消费。因为从本质上来说，这些消费者线程是要分别去消费程序中所有的生产者数据，并不是总共要消费程序中所有的消费者。这一点我相信是非常容易理解的。

但现在程序来到了并发消费者模式，所谓并发消费者模式，就意味着程序中所有的消费者线程不能再单独行动了，而是要联合起来，一起去消费程序中所有的数据。比如，程序中一共有 100 条数据，5 个消费者线程，在这些消费者线程消费的过程中，有的消费者线程可能消费了 10 条数据，有的消费者线程可能消费了 50 条数据，总之最后这 5 个消费者线程总的消费数据为 100 条。并且，有一点非常重要，在程序运行的整个流程中一定要时刻关注，那就是当一个线程消费了某个生产者进度后，其他线程不能再消费这个进度了。比如说现在有一个生产者进度 5 的数据，被某个线程消费了，剩下的 4 个线程就不能再消费这个生产者进度 5 的数据了。按照这种思路分析，我想大家应该也可以意识到，既然现在要让所有的消费者线程共同消费一批数据，并且这些数据不能被重复消费，这就意味着这些消费者不能再单独行动了，每一个消费者的消费进度都会互相有所关联，至少每个消费者都得知道其他消费者的消费进度，只有这样才能保证不会发生重复消费的情况。否则一个消费者消费了进度 5 的数据，而其他消费者并不知道，岂不是又会消费进度 5 的数据。这样看来，如果我们要重构代码，就要从这一点入手，让每个消费者都知道其他消费者的消费进度。那该怎么重构呢？难道要在每一个包装消费处理器的 BatchEventProcessor 对象中定义一个存放其他消费者消费进度的数组吗？就像下面这样。

```
public final class BatchEventProcessor<T>{

    //其他内容省略

    public Sequence[] sequences = new Sequence[0];
}
```

其他消费者线程消费了哪些进度都会存放到每个 BatchEventProcessor 对象新定义的 sequences 数组中，这样之后呢？好像也没什么头绪了，当前消费者每次消费之前都去数组中查看一下，如果数组中已经有了自己得到要消费的消费者进度，那么自己就无法消费者个进度了，只能进入下一次循环，获得下一个进度然后再对比，如果数组中还没有这个进度，那就可以消费这个进度，然后把消费的进度放到数组中。但是这样随之而来就有一个新的问题，每一个消费者线程怎么把自己消费的进度传递给其他消费者线程呢？也就是说每一个消费者线程自己的 BatchEventProcessor 对象怎么把消费进度传递到其他每个消费者线程的 BatchEventProcessor 对象中呢？如果真要这么做，好像每一个消费者线程的 BatchEventProcessor 对象也要知道其他所有消费者线程的 BatchEventProcessor 对象。难道要在每个消费者线程的 BatchEventProcessor 对象中再定义一个存放其他消费者线程的 BatchEventProcessor 对象的 BatchEventProcessor 数组吗？就像下面这样。

```
public final class BatchEventProcessor<T>{

    //其他内容省略

    public Sequence[] sequences = new Sequence[0];

    BatchEventProcessor[] batchEventProcessors = new BatchEventProcessor[0];
}
```

简直是太麻烦了，真的，不动脑子设计程序的话，最后就只会设计出这种垃圾程序。其实我们仔细想想就会恍然大悟，其实根本不用在每个消费者线程的 BatchEventProcessor 对象中定义什么数组，与其让每个消费者互相联系，或者说通知彼此，为什么不单独抽取一个组件来做这种事呢？如果是单独抽取一个组件，就可以把所有的消费者线程的 BatchEventProcessor 对象都交给这个组件来持有了，所有消费者的消费进度数组 sequences 也可以放在这个组件中了。并且，让我们进一步想一想，与其让每个消费者拿着自己得到的消费进度去这个存放所有消费者进度的 sequences 数组中做对比，为什么不让这个 sequences 数组给消费者分发进度呢？如果是分发进度，那就不需要 sequences 数组了，只需要定义一个 Sequence 对象即可。

这样一来，岂不是更加方便。反正消费者每次消费之前都要得到一个消费者进度，之前得到了消费者进度后要和生产者进度做对比，看能否消费这个进度，而在我们刚才分析的情况中，消费者得到了消费者进度后，首先要去 sequences 数组中查看一下得到的消费者进度是否和数组中的数据重复了，如果没有重复，才能和生产者进度做对比，看看能不能消费，显然有些麻烦。但现在我决定抽取一个单独的组件之后，我发现根本不必这么麻烦了，因为说到底现在的情况是所有消费者线程去共同消费一批数据，那么我就定义一个总的消费进度，也就是一个 Sequence 对象，并且这个总的消费者进度初始值也是 -1，每当消费者线程想要消费下一个进度后，就让这个总的进度分发一个进度序号出来，交给要消费的线程。这样，这个消费者线程得到了要消费的进度序号之后，只需要和生产者进度做对比，就可以知道是否能消费了。而且，这么做还有一个好处，那就是可以在分配序号的过程中就避免重复消费的情况。

举一个最直接的例子，比如说我们就在这个新定义的组件中定义一个 Sequence 对象，这个对象的初始值是 -1。程序中有 3 个消费者线程，当第一个消费者线程想要消费数据的时候，会先访问新定义的组件中的 Sequence 对象，这个对象的初始值为 -1，要分配下一个可以消费的序号，就会把 0 分配给第一个消费线程，第一个消费线程得到了进度序号 0 之后，就会去和生产者进度做对比，看看环形数组中 0 号位置是否有可消费的数据了，如果有，那么就可以直接消费了。如果没有那就阻塞等待 0 号为止被放置生产者数据。如果第一个线程刚刚取走了 0 号消费进度，第二个线程就来了，那这个 Sequence 对象就会把 1 号消费进度分配给第二个线程，之后就是把 2 号进度分配给第三个线程。到此为止，我相信大家已经明白了我们要重构程序的方向了，以及为什么可以这样重构了，因为说到底，并发消费者模式的本质就是多个消费者线程共同消费一批数据，不能发生重复消费的情况，必须要对每个消费者的消费进度进行协调，所以消费者进度也要有一个总的管理者，或者说是分发着。这样才能达到并发消费的效果。

好了，说了这么多，现在终于也该重构一下我们的代码了。既然我一直为大家强调要引入一个新的组件，那接下来我们就先把这个组件引入进来。从功能上来说，这个组件肯定是对众多消费者线程的消费者进度进行协调和管理的，换句话说，这个组件其实是用来管理众多消费者的。所以我就把这个组件定义为 WorkerPool，而用来包装消费处理器的 BatchEventProcessor 在并发消费者模式下也已经不合适了，我决定搞一个新的类型来包装消费处理器，这个新的类型仍然是一个 Runnable，就叫做 WorkProcessor。只要是在并发消费模式下，用户定义的每一个消费处理器都会被包装成一个个 WorkProcessor 对象，每一个 WorkProcessor 对象都会交给一个消费者线程来执行，而 WorkProcessor 内部持有者一个存放 WorkProcessor 对象的数组。具体逻辑请看下面代码块。

首先是 WorkerPool。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/27
 * @Description:消费池，其实就是把所有的消费者处理器，也就相当于消费者线程一起管理了
 */
public final class WorkerPool<T> {
    //消费池是否启动的标志
    private final AtomicBoolean started = new AtomicBoolean(false);

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/27
     * @Description:这个属性相当重要，就是用来给WorkerPool管理的每一个消费者分配消费序号的
     * 每当消费者希望能消费一个生产者数据时，都要去下面这个workSequence中申请。workSequence代表的就是
     * 已经分配到的消费进度，比如现在生产者发布的进度为15，而workSequence已经分配到7了，那么每一个
     * 消费者竞争消费这些生产者数据时，都会让workSequence来给它们分配可消费序号。比如现在一共有3个消费者要消费
     * 下一个数据了，因为是竞争消费，这就意味着下一个要消费的序号为8，所以这三个消费者
     * 线程消费下一个序号的生产者数据时，都会去workSequence中竞争下一个序号，竞争成功的话，就用CAS把workSequence
     * 可分配的消费进度更新一下，其实就是加1
     */
    private final Sequence workSequence = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);

    //环形数组
    private final RingBuffer<T> ringBuffer;

    //工作处理器数组，数组中的每一个工作处理器都对应一个消费着，因为WorkProcessor
    //会包装用户定义的handler
    private final WorkProcessor<?>[] workProcessors;

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/27
     * @Description:构造方法
     */
    @SafeVarargs
    public WorkerPool(final RingBuffer<T> ringBuffer,final SequenceBarrier sequenceBarrier,final ExceptionHandler<? super T> exceptionHandler,final WorkHandler<? super T>... workHandlers){
        //环形数组赋值
        this.ringBuffer = ringBuffer;
        //得到用户定义的handler的数量
        final int numWorkers = workHandlers.length;
        //创建WorkProcessor数组
        workProcessors = new WorkProcessor[numWorkers];
        //给工作数组的每一个位置初始化WorkProcessor对象
        for (int i = 0; i < numWorkers; i++){   
            //在这里，数组的初始化就完成了，也就意味着WorkerPool管理了用户定义的所有消费处理器
            //在这里就把该类的workSequence成员变量传递给WorkProcessor对象了
            workProcessors[i] = new WorkProcessor<>(ringBuffer,sequenceBarrier,workHandlers[i],exceptionHandler,workSequence);
        }
    }



     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/27
     * @Description:返回WorkerPool管理的每一个消费者的消费进度，以及WorkerPool自身中的workSequence的进度
     */
    public Sequence[] getWorkerSequences(){
        
        final Sequence[] sequences = new Sequence[workProcessors.length + 1];
        
        for (int i = 0, size = workProcessors.length; i < size; i++){
            sequences[i] = workProcessors[i].getSequence();
        }
        
        sequences[sequences.length - 1] = workSequence;
        
        return sequences;
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/27
     * @Description:启动消费者池，其实就是为每一个workProcessors启动一个线程，然后执行workProcessors任务
     */
    public RingBuffer<T> start(final Executor executor){
        if (!started.compareAndSet(false, true)){
            throw new IllegalStateException("WorkerPool has already been started and cannot be restarted until halted.");
        }
        //得到当前生产者的生产进度
        final long cursor = ringBuffer.getCursor();
        //给workSequence赋值，代表着现在这么多消费者，最多可以消费到cursor这个进度
        //这里我要详细解释一下，如果程序刚启动的时候，程序中还没有生产者的时候
        //那么当前生产者的进度就是-1，workSequence的初始进度也就是-1
        //在下面的循环中，每一个消费者进度也会被设置成-1
        //当消费者开始消费的时候，得到下一个可以消费的进度，也就是0，就会去环形数组0号位置查看是否有数据可以消费了
        //没数据则则阻塞等待，有数据则消费，这种情况我们已经比较熟悉了
        //但是，假如现在有这样一种情况，程序启动的时候是先让生产者线程发布了一批数据，环形数组的长度是1024，而生产者进度更新到第16了
        //这时候启动消费者，那么这个workSequence的进度就会被设置为16，也就是说消费者线程一启动，就意味着
        //进度16之前的数据都是可以消费的，并且在下面的for循环中每一个消费者线程的消费进度
        //也会被设置为16，可是进度0-16的数据根本还没被消费呀，这是怎么回事呢？
        //我希望大家再学习下面内容的时候，可以带着这个思考去看代码
        workSequence.set(cursor);
        
        for (WorkProcessor<?> processor : workProcessors){   
            //给管理的每一个消费者的消费进度都赋成这个值
            processor.getSequence().set(cursor);
            //启动每一消费者线程
            executor.execute(processor);
        }
        return ringBuffer;
    }

	//其他内容省略

}
```

接下来就是 WorkProcessor 类型。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/27
 * @Description:这个类就是用来包装用户定义的每一个消费handler的，就叫工作处理器吧
 */
public final class WorkProcessor<T> implements EventProcessor {

     //判断该工作处理器是否正在运行
    private final AtomicBoolean running = new AtomicBoolean(false);
    
    //消费者自身的消费进度
    private final Sequence sequence = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);
    
    //环形数组
    private final RingBuffer<T> ringBuffer;
    
    //序号屏障
    private final SequenceBarrier sequenceBarrier;
    
    //用户定义的消费者处理器，是真正执行消费者逻辑的处理器
    private final WorkHandler<? super T> workHandler;
    
    //异常处理器
    private final ExceptionHandler<? super T> exceptionHandler;
    
    //WorkerPool自身的消费进度
    private final Sequence workSequence;

    //其他内容省略
}
```

可以看到，新引入的 WorkProcessor 类和第一版本中的 BatchEventProcessor 类没什么大的区别，只多出来一个 workSequence 成员变量，因为按照我们刚才的分析，现在消费者线程要消费数据的时候，应该首先去访问 workSequence 对象，请求这个对象分配一个可以消费的序号，然后再拿着这个序号和生产者进度做对比看是否可以消费。如果可以消费就直接消费即可。所以，接下来，我就为大家把 WorkProcessor 这个 Runnable 的 run 方法实现了，run 方法就是消费者线程具体的工作内容。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/27
 * @Description:这个类就是用来包装用户定义的每一个消费handler的，就叫工作处理器吧
 */
public final class WorkProcessor<T> implements EventProcessor {

     //判断该工作处理器是否正在运行
    private final AtomicBoolean running = new AtomicBoolean(false);
    
    //消费者自身的消费进度
    private final Sequence sequence = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);
    
    //环形数组
    private final RingBuffer<T> ringBuffer;
    
    //序号屏障
    private final SequenceBarrier sequenceBarrier;
    
    //用户定义的消费者处理器，是真正执行消费者逻辑的处理器
    private final WorkHandler<? super T> workHandler;
    
    //异常处理器
    private final ExceptionHandler<? super T> exceptionHandler;
    
    //WorkerPool自身的消费进度
    private final Sequence workSequence;




     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/27
     * @Description:可以说是第二版本代码中要讲解的最重要的方法了，其他新添加的内容都有第一版本的影子
     * 这个也不例外，但是这个的核心肯定和第一版本不同
     */
    @Override
    public void run() {
        
        //省略部分内容

         //这个默认为true，是用来判断当前线程是否可以去申请下一个消费者进度序号了
        boolean processedSequence = true;
        
        //这是一个判断值，最小的long整数，是个负数，-2的63次方，是小于0的。Long.MIN_VALUE只在循环的第一次使用，后面cachedAvailableSequence
        //就会被其他值赋值了
        long cachedAvailableSequence = Long.MIN_VALUE;
        

        //首先得到消费者当前的消费进度，如果是程序刚启动，并且是消费者线程先启动
        //生产者还没有发布数据时，这时候消费进度肯定是-1
        //只有消费了生产者后，才能更新自己的消费进度，而消费者启动是在生产者发布数据之前的
        //所以这时候消费者的进度还是初始值-1
        long nextSequence = sequence.get();

        //环形数组中存储的事件对象
        T event = null;


        //进入循环
        //在进入这个循环之后，我把注释分成了两部分，代表着两次循环的逻辑，当第一次循环的时候，大家可以看每行代码上
        //序号为1的注释，第二轮循环的时候，就看每行代码上序号为2的注释，两圈看下来，这个核心逻辑就明白了
        while (true){

            //1.先判断是否可以去申请下一个序号了，默认为true
            //2.这时候，processedSequence已经是false，不会进入下面的这个分支，因为刚才已经给这个
            //2.消费者线程分配了，就是nextSequence，也就是序号0，但是当前消费者显然没有消费成功呢
            //2.所以不能进入下面这个分支申请下一个可消费的序号
            if (processedSequence){   
                
                //1.但是一旦进入这个分支后，就会置为false
                processedSequence = false;
                  
                //1.在这个小的循环中，得到下一个要分配的消费进度序号，这里还是通过举例为大家讲解
                //1.如果是消费者刚启动的时候，workSequence中value的值肯定为-1，那么这里得到的
                //1.nextSequence就为0
                nextSequence = workSequence.get() + 1L;
                //1.给当前申请消费进度序号的消费进度赋值-1，注意啊，这里我们的各个数值都是针对消费者
                //1.刚启动，而生产者还未发布生产数据的时候，这个时候条理最清楚
                sequence.set(nextSequence - 1L);
        
                //1.这时候workSequence的值就设置成了0
                //1.代表着消费者进度序号0已经分配出去了
                workSequence.set(nextSequence));
            }

            
            //1.接下来就会判断之前得到cachedAvailableSequence是否大于nextSequence，注意，这时候nextSequence为0
            //1.消费者刚启动的时候，肯定是小于的，因为Long.MIN_VALUE是个负数，所以不会走下面的分支
            //2.第二次循环的时候，就会进入到这个分支中，因为现在cachedAvailableSequence已经被赋值成可消费的最大进度序号了
            //2.肯定是大于等于0的
            if (cachedAvailableSequence >= nextSequence){ 
                //2.取出0号位的对象
                event = ringBuffer.get(nextSequence);
                //2.执行真正的消费逻辑
                workHandler.onEvent(event);
                //2.把该值设置为true，这样当前线程就可以在下一次循环时申请下一个要消费的进度序号了
                //2.到此为止，一个线程争抢序号以及执行消费逻辑的过程，我就已经为大家分析完了
                processedSequence = true;
            }
                
            else
            {   //1.会走到这个分支，为什么走到这个分支呢？答案已经很明显了，因为是消费者刚启动，生产者还未发布生产数据
                //1.自然没数据可消费，所以会在这里等待生产者发布生产数据，并返回可消费的最大的进度序号
                //1.sequenceBarrier.waitFor(nextSequence)的逻辑和上个版本的一样，就不再讲解了
                //1.阻塞结束后，就意味着有生产者数据了，接下来，就该进入第二轮循环了
                //1.在这里cachedAvailableSequence会被赋值成可消费的最大进度序号
                cachedAvailableSequence = sequenceBarrier.waitFor(nextSequence);
            }
        }

        //省略部分逻辑
    }




    
    //其他内容省略
}
```

我想上面的代码块已经为大家把消费者线程的工作流程和逻辑剖析得非常清楚了，所以就不再赘述了。当然，我知道肯定有朋友会非常困惑，如果只是做到上面那种程度，明显还有一个致命的缺陷，那就是无法保证多线程不会重复消费数据。如果程序中的线程可以任凭我们安排，一个接一个乖乖地向 workSequence 申请下一个要消费的进度序号，而 workSequence 每次加 1，把下一个要消费的进度序号分配给消费者线程，这样肯定就不会发生重复消费数据的情况。可目前的情形是多个线程同时访问 workSequence，并且访问结束之后还会同时执行 workSequence.set(nextSequence) 这行代码，如果有三个线程都执行成功了这行代码，就意味着这三个线程都可以去消费进度序号为 0 的数据了，这怎么可能不发生并发问题呢？那么该怎么解决这个并发问题呢？

**使用 CAS，解决重复消费数据问题**

好像绕了一个圈，终于又回到了最开始的那个问题，使用什么样的同步手段来解决程序中可能出现的并发问题。之前我们本来就像解决这个问题的，打算使用同步锁，但是并不知道在哪里加锁，所以才绕了一个大圈子，现在才可以直面这个问题。

不过，现在我的问题稍有改变，我想问的是，我们还需要使用同步锁来保证并发安全吗？同步措施肯定是要使用的，但是同步锁真的还合适吗？其实我心里已经有了决定，肯定不会再使用同步锁来保证并发安全了，而是使用 CAS 手段。原因有二：第一，使用同步锁保证线程安全，会阻塞线程，使用 CAS 会导致线程空转，浪费 CPU 资源，但是，如果程序中并发的线程数量并不多呢？即便都在空转，也浪费不了多少资源，所以，在程序中并发线程比较少的情况下，完全可以使用 CAS 保证并发安全；第二点就比较关键了，需要从代码中具体分析，我们来分析一下在 WorkProcessor 的 run 方法中，并发问题主要会出现在哪里？显而易见，肯定是 workSequence.set(nextSequence) 这行代码，对吧？在这行代码之前，run 方法所做的只是使用 workSequence 加 1，得到下一个可以分配的进度序号，并没有对 workSequence 本身做什么改变。那么，如果我们保证了 workSequence.set(nextSequence) 这行代码的并发安全，也就意味着我们保证了序号分配的安全，这样一来，并发问题不就解决了吗？

举一个简单的例子，现在程序中的问题是多个线程都会去访问 workSequence 对象，获取下一次要消费的进度，如果有 3 个线程都去访问，并且 workSequence 的初始值是 -1，那么这三个线程都会执行 workSequence.set(nextSequence) 这行代码，并且都会执行成功，也就意味着都分配到了进度为 0 的序号。现在我希望只有一个线程能得到 0 这个序号，这样一来，是不是就意味着只能有一个线程执行这行代码成功呢？而执行不成功的则放弃这次申请，直接开始申请下一个要消费的序号。这不就是典型的 CAS 解决并发的流程吗？所以，根据这两点原因，我选择使用 CAS 来保证并发安全。

代码的改动非常简单，只需要在相关的地方加上 CAS 处理方式即可，具体实现请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/27
 * @Description:这个类就是用来包装用户定义的每一个消费handler的，就叫工作处理器吧
 */
public final class WorkProcessor<T> implements EventProcessor {


     //其他内容省略

     @Override
    public void run() {
        
        //省略部分内容

         //这个默认为true，是用来判断当前线程是否可以去申请下一个消费者进度序号了
        boolean processedSequence = true;
        
        //这是一个判断值，最小的long整数，是个负数，-2的63次方，是小于0的。Long.MIN_VALUE只在循环的第一次使用，后面cachedAvailableSequence
        //就会被其他值赋值了
        long cachedAvailableSequence = Long.MIN_VALUE;
        

        //首先得到消费者当前的消费进度，如果是程序刚启动，并且是消费者线程先启动
        //生产者还没有发布数据时，这时候消费进度肯定是-1
        //只有消费了生产者后，才能更新自己的消费进度，而消费者启动是在生产者发布数据之前的
        //所以这时候消费者的进度还是初始值-1
        long nextSequence = sequence.get();

        //环形数组中存储的事件对象
        T event = null;


        //进入循环
        //在进入这个循环之后，我把注释分成了两部分，代表着两次循环的逻辑，当第一次循环的时候，大家可以看每行代码上
        //序号为1的注释，第二轮循环的时候，就看每行代码上序号为2的注释，两圈看下来，这个核心逻辑就明白了
        while (true){

            //1.先判断是否可以去申请下一个序号了，默认为true
            //2.这时候，processedSequence已经是false，不会进入下面的这个分支，因为刚才已经给这个
            //2.消费者线程分配了，就是nextSequence，也就是序号0，但是当前消费者显然没有消费成功呢
            //2.所以不能进入下面这个分支申请下一个可消费的序号
             if (processedSequence)
                {   //1.但是一旦进入这个分支后，就会置为false
                    processedSequence = false;
                    do
                    {   //1.在这个小的循环中，得到下一个要分配的消费进度序号，这里还是通过举例为大家讲解
                        //1.如果是消费者刚启动的时候，workSequence中value的值肯定为-1，那么这里得到的
                        //1.nextSequence就为0
                        nextSequence = workSequence.get() + 1L;
                        //1.给当前争抢消费进度序号的消费进度赋值-1，注意啊，这里我们的各个数值都是针对消费者
                        //1.刚启动，而生产者还未发布生产数据的时候，这个时候条理最清楚
                        sequence.set(nextSequence - 1L);
                    }//1.CAS赋值，如果是当前消费者线程修改成功，那么就会把上面得到的nextSequence赋值给workSequence
                     //1.这时候workSequence的值就变成了0
                    while (!workSequence.compareAndSet(nextSequence - 1L, nextSequence));
                }

            
            //1.接下来就会判断之前得到cachedAvailableSequence是否大于nextSequence，注意，这时候nextSequence为0
            //1.消费者刚启动的时候，肯定是小于的，因为Long.MIN_VALUE是个负数，所以不会走下面的分支
            //2.第二次循环的时候，就会进入到这个分支中，因为现在cachedAvailableSequence已经被赋值成可消费的最大进度序号了
            //2.肯定是大于等于0的
            if (cachedAvailableSequence >= nextSequence){ 
                //2.取出0号位的对象
                event = ringBuffer.get(nextSequence);
                //2.执行真正的消费逻辑
                workHandler.onEvent(event);
                //2.把该值设置为true，这样当前线程就可以在下一次循环时申请下一个要消费的进度序号了
                //2.到此为止，一个线程争抢序号以及执行消费逻辑的过程，我就已经为大家分析完了
                processedSequence = true;
            }
                
            else
            {   //1.会走到这个分支，为什么走到这个分支呢？答案已经很明显了，因为是消费者刚启动，生产者还未发布生产数据
                //1.自然没数据可消费，所以会在这里等待生产者发布生产数据，并返回可消费的最大的进度序号
                //1.sequenceBarrier.waitFor(nextSequence)的逻辑和上个版本的一样，就不再讲解了
                //1.阻塞结束后，就意味着有生产者数据了，接下来，就该进入第二轮循环了
                //1.在这里cachedAvailableSequence会被赋值成可消费的最大进度序号
                cachedAvailableSequence = sequenceBarrier.waitFor(nextSequence);
            }
        }

        //省略部分逻辑
    }

    


    
    //其他内容省略
}
```

重构后的代码块就完美解决了程序中可能出现的并发问题，注释非常详细，所以我也就不再重复讲解了。

到此为止，这一章的核心知识点我就给大家讲解完毕了，在结束之前，我还想就上面的代码块总结三点。第一：和上一章的 BatchEventProcessor 类做对比，可以发现本章引入的 WorkProcessor 类只能一次处理一个任务，因为 workSequence 对象每次只加 1，然后把得到的序号分配个消费者线程去消费。而 BatchEventProcessor 翻译过来是批处理的意思，这个处理器可以一次消费多个可以消费的数据，这是两者最大的不同。第二点就比较隐晦了，不知道大家注意到了吗？在上面代码块的 run 方法中，每一次 workSequence 分配了一个序号之后，比如说 workSequence 的初始值是 -1，第一次分配序号的时候会把 0 分配出去，但是分配出去之后，虽然 workSequence 的序号进度变成 0 了，但是得到 0 这个进度的消费者线程，它的消费进度仍然为 -1。直到把序号 0 对应的数据消费完了，等待该线程又一次来申请要消费的进度序号时，才会更新自己的消费进度。为什么要这样处理呢？其实原因也很简单，当 workSequence 把要消费的序号分配给消费者线程后，并不知道消费者线程是否可以成功消费这个序号，或者不知道该线程是否还会继续持有 CPU 资源，可以继续执行，如果把序号分配给消费者线程之后，该线程却迟迟没有消费该数据，但是已经把该线程的消费进度更新了，那么生产者线程存放生产者数据的时候，很有可能就会覆盖了尚未被消费的数据。所以，要保证消费者线程确实把数据消费完了，才能对齐消费进度进行更新，只有等进入下一次循环的时候，才能保证该线程已经消费了其分配到的进度序号对应的数据，也就可以更新其消费进度了。

第三我想跟大家谈论的是之前在注释中问过大家的问题，如果程序中生产者线程先启动了，消费者线程后启动，那么，消费者线程怎么消费之前生产的数据呢？我的回答是无法消费，确切地说，在并发消费者模式下，如果消费者线程后启动，那么就无法消费之前生产的数据了。因为 WorkProcessor 一次只能分配一个要消费的序号，并且每次分配都是根据当前 workSequence 的进度来分配下一个要消费的进度序号的，每次分配的序号相对于上一个序号加 1。这就意味着每个消费者线程每次只能消费 1 个数据，并且是当前消费进度之后的数据，之前的数据就无法消费了。也就是说，如果使用的是并发消费者模式，那么在编写启动类的时候，务必要让消费者线程先启动，然后再开始发生产者数据。大家可以自己在测试类中调整一下生产数据和启动消费者线程的代码顺序，检测一下。

好了，本章的内容到这里就结束了，有关本章的更多细节，大家就去我提供的第二版本代码中学习吧。如果大家对多生产者模式感兴趣，那么大家其实就可以直接去看第三版本的代码了。因为多生产者模式无非就是并发竞争生产者序号，和本章并发竞争消费进度序号原理是一样的。这个框架没有什么晦涩的知识点了，大家可以自己研究甚至对该框架二次开发，重构一些功能。好了，我们下一章见。