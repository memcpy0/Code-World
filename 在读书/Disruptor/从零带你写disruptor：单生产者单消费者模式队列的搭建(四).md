朋友们，今天继续为大家讲解 disruptor 框架，距离前三篇文章发布已经过去一个星期了，这一个星期照样还是很忙，签约给出版社的 Netty 书稿还没写完，内容总是越写越多，想到写完之后还要修改，还要进行删减就头大。但是这件事不得不做，毕竟已经签了合同。想想还有好多课程没有更新，还准备了很多想上线的课。比如 Java 字节码，汇编，操作系统，为什么想讲解操作系统呢？因为最近在写 JUC 系列的时候，意识到有些方法的底层深入到操作系统中了，我正在写的第一篇手写 AQS，这个队列的设计原理其实就是从操作系统的 mutex 与 condition 延伸过来的。当然我也不会直接讲解操作系统的源码，我有我自己的讲解思路和风格，怎样讲解不重要，重要的是最后让大家把知识都掌握了。既然要讲解操作系统肯定要使用 C 语言，所以我也想出一门课程专门讲解 C 语言，可是 C 语言都讲了，为什么不把 C++ 也讲了呢？为什么不把 Redis 也讲一下呢？后来我又想到，反正都讲了这么多了，为什么不把我之前自己写的编译器和虚拟机，把这两个小玩具拿出来讲给大家听，然后再正式给大家讲解 Javac 和 JVM。

可能会有朋友觉得，我不过才工作八年，今年正好是第八年，真的能掌握这么多知识吗？就算是掌握了，恐怕也未必有多精通。我对此有一套自己的看法，我觉得大多数知识都是重复的，虽然框架换了一个又一个，但是用到的知识并没什么大的变化，要说有变化，可能只是使用的语言不一样，也就是说调用的 API 不同的，但 API 底层的原理还是类似的。如果我掌握了 IO 多路复用机制，Netty 和 Redis 的相关之知识我想我应该都能看懂，如果有什么不同，只是 API 不一样而已。而 API 是最容易学习的。如果我掌握了 Java 的 SPI 机制，我想 Dubbo 和 SpringBoot 中的相关机制我也能看明白，当然，它们对 Java 的 SPI 都做了扩展和优化。那么这些扩展和优化就是我要学习的新知识。当我的知识积累到一定的程度时，我研究一个自己非常熟悉的框架源码时，可能一个星期或者半个月就看完了。如果框架很简单，可能一下午就研究得差不多了。就比如 XXL-JOB，Disruptor 等等。当然，自己看的速度是非常快的，如果想把它详细地梳理一下，写成文章，这就慢很多了。再说，我只是工作了八年，上大学和读研这么多年我也一直在学习。我想只要利用好时间，十几年的功夫足够你掌握太多知识了。

再来谈谈我为什么想更新这么多课程，本来一开始我们的系列很简单，只有几门课程，然后加上了虚拟机和编译器。但随着更新，添加进来的课程或者说被我提上日程的课程也越来越多。我想给大家解释一下为什么我开始这么做了，首先我得承认，大家的热情和鼓励，甚至是肯定和赞扬激发了我的创作欲望，给了我一种前所未有的成就感。说实话，在更新从零带你写 Netty 之前，我本人没写过什么教学文章，更别提录制视频了。因为我从来没想过自己能写这种文章，也不知道写出来会不会有人看，所以虽然自己脑子里确实有那么一丁点知识，但是从来没想过要写出来分享一下。让我没想到是，课程发布之后，大家学得非常认真，经常有朋友来问我问题，和我交流，还有朋友专门来夸赞我，说我的课程大大降低了 Netty 的学习门槛，渐进式教学非常棒等等，甚至有朋友过分地称赞我讲的 Netty 比闪电侠还清楚。我真的真的受容若惊，同时也更有压力，要认真对待每一门课程，不想破坏自己的课程在大家心中的形象。同时我也觉得自己分享一些知识，真的能让大家学到东西，我觉得自己在做很有意义的事，如果后面课程真的做大了，很多人都会看我讲解的知识，我感觉我真的做了一件很有意义的事。我最喜欢的一部电视剧就是士兵突击，我喜欢这种专心做事，等待这件事开花结果的过程。信念是做出来的，不是说出来的。这句话一直被我记在心里。

另一个原因就是我希望更新的课程越来越多，这样才能吸引更多的朋友来学习，学的人多了，才能进一步把课程做大，才能宣传出去。其他的原因也没什么了，我想我做系列课程的原因应该很纯粹了，一是挣钱，二是在服务大家的同时，也能收获极大的满足感，之所以有满足感是因为我获得了大家的肯定和鼓励，所以，为了这份满足感，我就更不能辜负大家的信任。好了，说了这么多，还是快点进入这一章的正题吧。

**简单回顾前几章的知识**

在正式开始讲解之前，还是很有必要回顾一下前几章讲解的知识，引入的那些类都是怎么定义的。倒不是我担心大家在学习这一章之前把之前讲解的知识忘记了，而是我已经忘记之前引入的类是怎么定义的了。过了一个星期才写第四章，前面我讲了什么内容，我都有点记不清了。继续讲解的思路也没那么连贯了，所以我要把之前的代码搬运过来，先看一看，然后再继续向下讲解。如果大家对前面的内容都还有印象，就可以直接跳过这一小节，从下一小节开始学习。简单来说，这一小节是为我自己写的。

首先是 Sequence 类。这个就是我引入的进度序号。

```
public class Sequence{

    //还是用long来表示序号进度，只不过用对象包装了一下
    protected volatile long value;

    //初始化进度，不管是消费者还是生产者，初始化的进度都是-1
    static final long INITIAL_VALUE = -1L;

    //构造方法
    public Sequence(){
        this(INITIAL_VALUE);
    }

    //设置value的初始值，也就是-1L
    public Sequence(final long initialValue){
        this.value = initialValue;
    }

    //返回value
    public long get(){
        return value;
    }

    //设置value的值,当生产者或者消费者进度加1的时候，就可以把加1之后的值设置进来了
    public void set(final long value){
        this.value = value;
    }
}
```

然后是序号屏障 ProcessingSequenceBarrier。

```
final class ProcessingSequenceBarrier {

    //等待策略，既然是序号屏障在协调消费者和生产者的进度，那么当生产者没有发布数据，消费者没有事件可消费的时候
    //肯定就应该等待了，等待生产者继续发布了数据，消费者才能继续消费
    private final WaitStrategy waitStrategy;

    //生产者的生产进度
    private final Sequence cursorSequence;

     //构造方法
    ProcessingSequenceBarrier(final WaitStrategy waitStrategy,final Sequence cursorSequence,){
         this.waitStrategy = waitStrategy;
         this.cursorSequence = cursorSequence;
    }

    
    //该类的核心方法，sequence参数就是消费者期望可以消费的最小的序号，比如在消费者当前的
    //消费进度是4，那么在BatchEventProcessor类的processEvents方法中加1，期望消费的就是5
    //这个5就会当作参数传到下面这个方法中，然后这个方法就会以这个消费者希望消费的最小参数
    //和生产者进度做对比，最后返回一个消费者能消费的最大进度，这个进度也可能还是5，总之得
    //和生产者的进度对比才行
    public long waitFor(final long sequence){
        //使用具体的等待策略来判断消费者可以消费的最大序号，返回的这个序号就是消费可以消费的最大序号
        long availableSequence = waitStrategy.waitFor(sequence, cursorSequence);
        //下面这个判断就是看一下返回的最大序号是否比消费者申请的最小序号还小，算是一个保底操作吧
        //实际上不可能发生，如果真的发生了，那就还是返回availableSequence即可
        if (availableSequence < sequence){
            return availableSequence;
        }
        return availableSequence;
    }
}
```

接下来是 SleepingWaitStrategy。

```
public final class SleepingWaitStrategy implements WaitStrategy{

    //默认的自旋次数，自旋就是空转
    private static final int DEFAULT_RETRIES = 200;
    //默认的睡眠时间
    private static final long DEFAULT_SLEEP = 100;
	//自旋次数
    private  int retries;
    //睡眠时间
    private final long sleepTimeNs;

    //无参构造方法
    public SleepingWaitStrategy(){
        this(DEFAULT_RETRIES, DEFAULT_SLEEP);
    }

    //这里多出来一个retries成员变量的set方法，因为线程结束阻塞后要重置retries属性为200
    public void setRetries(int retries) {
        this.retries = retries;
    }

    public SleepingWaitStrategy(int retries, long sleepTimeNs){
        //自旋次数被赋值为200
        this.retries = retries;
        //睡眠时间被赋值为100
        this.sleepTimeNs = sleepTimeNs;
    }

    
    //sequence就是想要消费的最小消费进度，cursor就是当前的生产者进度
	@Override
    public long waitFor(final long sequence, Sequence cursor){
        //定义消费者可使用的最大进度
        long availableSequence;
        
        //在这里得到默认的自旋次数
        int counter = retries;
        
        //在这里重构了一下，cursor.get()得到的就是生产者当前的进度，也就是消费者能消费的最大进度
        //然后赋值给availableSequence，再判断availableSequence是否大于了消费者想要消费的最小进度
        //如果大于了，说明可以跳出循环了
        //如果消费者想消费的最小进度为4，当前生产者的进度为9，那么直接就可以跳出循环，然后把9返回给消费者
        while ((availableSequence = cursor.get()) < sequence){
            //在这里让线程等待，直到生产者发布了新的数据，其进度更新了
            //超过或者等于了消费者要消费的进度
        	//这返回的counter很重要，是需要再下一次循环中再次传到applyWaitMethod方法中的
            counter = applyWaitMethod(counter);
        }
        
        //返回可用的最大进度
        return availableSequence;
    }

    
    
    private int applyWaitMethod(int counter){
        if (counter > 100){
            --counter;
        }
        //如果自旋次数小于100，大于0了，说明已经自旋了很多次了，但还是不能继续向下工作，这时候尝试让该线程让出CPU
        else if (counter > 0){
            --counter;
            Thread.yield();
        }
        else{ //走到这里意味着自旋次数到达200了，这时候就干脆让线程睡一会吧
            //睡的时间就是100纳秒，不能睡得太久，因为生产者可能随时发布新的数据
            LockSupport.parkNanos(sleepTimeNs);
        }

        //这里返回剩余的自旋次数，方便下一次循环的时候继续使用
        //这样，再多消费者情况下，只要外面的循环不结束，那么每个线程使用的都是自己剩余的自旋数
        return counter;
    }
}
```

接下来就是包装消费者的 BatchEventProcessor 处理器。请看下面代码块。

```
public final class BatchEventProcessor<T> implements EventProcessor{


    //用户定义的消费handler，在该类的run方法中，其实执行的也是该handler中实现的方法
    private final EventHandler<? super T> eventHandler;


    //这个成员变量会被环形数组赋值
    private final DataProvider<T> dataProvider;

    //序号屏障，每一个消费者都有一个序号屏障
    private final ProcessingSequenceBarrier sequenceBarrier;
    
    //消费者的消费序号，其实就是消费的进度
    //默认初始值为-1，大家可以看一下上一章定义的Sequence对象，里面有一个初始值-1
    //就是INITIAL_CURSOR_VALUE这个成员变量
    private final Sequence sequence = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);

    //构造方法
    public BatchEventProcessor(final DataProvider<T> dataProvider,final ProcessingSequenceBarrier sequenceBarrier,final EventHandler<? super T> eventHandler){
        this.dataProvider = dataProvider;
        this.sequenceBarrier = sequenceBarrier;
        this.eventHandler = eventHandler;
    }

    //得到当前消费者的消费进度，序号就是进度，实现接口中的方法
    @Override
    public Sequence getSequence(){
        return sequence;
    }

    //实现Runnable接口的run方法
    @Override
    public void run(){
        processEvents();
    }


    //消费者的具体逻辑，这个是该类最重要的方法
    private void processEvents(){
        T event = null;
        long nextSequence = sequence.get() + 1L;
        while (true){
             //这里的代码终于完善了，判断消费者线程是否阻塞，以及返回最大的可消费进度的功能
            //就在这个方法内实现了
             final long availableSequence = sequenceBarrier.waitFor(nextSequence);
                while (nextSequence <= availableSequence){   
                    event = dataProvider.get(nextSequence);
                    eventHandler.onEvent(event, nextSequence);
                    nextSequence++;
                }
            sequence.set(availableSequence);
        }
    }
}
```

接下来就是序号生成器 SingleProducerSequencer。请看下面代码块。

```
public final class SingleProducerSequencer{

    //环形数组的容量，这个成员变量很关键，判断生产者数据是否会覆盖旧数据的时候
    //必须要用到，由此可见，这些框架整得花里胡哨，其实关键代码和JDK的阻塞队列的源码没什么区别
    //本质都是用count和数组长度做判断
    protected final int bufferSize;

    //消费者的当前进度,这个是需要从外面传进来的
    protected final Sequence consumer;

    //生产者的当前进度，也就是生产者分配到第几个序号了，初始值为-1
    protected final Sequence cursor = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);

    //该成员变量就是当前分配的可用的序号，可以看到初始值也是-1L
    long nextValue = Sequence.INITIAL_VALUE;

    //构造方法
    public SingleProducerSequencer(int bufferSize,Sequence consumer){
        this.bufferSize = bufferSize;
        this.consumer = consumer;
    }

    //申请一个可用的序号的方法
    public long next(){   
        //这里可以看到，默认只申请一个
        return next(1);
    }

   
    //申请指定数量的序号，交给生产者使用
    @Override
    public long next(int n){
        //判断是否小于1
        if (n < 1){
            //抛出异常
            throw new IllegalArgumentException("n must be > 0");
        }
        //先得到当前的序号，这个序号就是序号生成器已经分配好了的序号
        //其实也就可以代表生产者的最新进度，比如现在生产者的进度是13，那么这个nextValue就是13
        long nextValue = this.nextValue;
        
        //加上要申请的序号的个数，得到最终要申请到的序号的值
        //假如就申请一个可以使用的，那就是13+1=14，判断进度14可不可以存放生产者数据
        //如果一下在要申请7，那就是13+7=20，判断20和20之前的进度是不是都可以存放生产者数据
        long nextSequence = nextValue + n;
        
        //下面这里的计算很重要，现在我们知道nextValue序号是13了，比如说要申请的序号是7，那么13加7是20
        //就是要判断20和20之前的进度是不是都可以存放生产者数据
        //比如说bufferSize是8，那么nextSequence - bufferSize最后得到的差值就是12
        //这是不是就意味着，申请之后的生产者进度实际上比整个环形数组的容量大了12个
        //也就是说生产者的进度在第一次填满环形数组后，还能进行第二次填满，然后还多出来4个要填冲
        //也就是要填充第三轮的四个进度，如果不想覆盖旧数据，就必须保证第二轮的前4个数据都被消费了
        long wrapPoint = nextSequence - bufferSize;
        
        //现在wrapPoint是12，假如消费者的进度是11，是不是就意味着消费者该消费进度12了，也就是
        //环形数组第二轮填充的第4个数据，但现在wrapPoint是12，意味着生产者要填充的是环形数组
        //第三轮的第四个位置，但是消费者显然还没有消费这个数据呢，所以生产者不能发布数据，只能阻塞
        while (wrapPoint > consumer.get()){
            //源码在这里其实有一个TODO，打算用等待策略来扩展这里的LockSupport
            //但是一直没有实现，该类最后一次更新已经是去年了，这个扩展也没实现
            //这里就是生产者线程阻塞的实现，自己睡一纳秒，然后自动醒，在循环中判断
            //是否不会覆盖尚未消费的数据了
            LockSupport.parkNanos(1L);
        }
        
        //走到这里，说明当前的生产者线程已经不阻塞了，因为在上面循环中的等待，已经让消费者把该消费的数据都消费了
        //消费者的进度序号肯定已经更新了
        //所以，这里就可以直接把最新的要分配的序号赋值给nextValue成员变量了，意味着这个最新的值可以分配给生产者使用了
        this.nextValue = nextSequence;
        //返回最新的分配出来的序号，交给生产者使用
        return nextSequence;
    }


    //发布可以被消费的生产者序号的方法，消费者感知到了，就可以继续消费数据了
    public void publish(long sequence){
        //发布序号，也就是更新生产者的进度
        cursor.set(sequence);
    }
}
```

最后就是环形数组 RingBuffer 了。

```
public final class RingBuffer<E> {

    //用来进行位运算求取数组索引的掩码
    private final long indexMask;
    
    //存放数据的数组，可以抽象成环形数组
    private final Object[] entries;
    
    //环形数组的容量，必须是2的n次幂
    protected final int bufferSize;

    //序号生成器，用来给生产者分配可用序号的
    protected final SingleProducerSequencer sequencer;


    //构造方法,这里把创建数据对象的工厂传进来了
    RingBuffer(int bufferSize,EventFactory<E> eventFactory,SingleProducerSequencer sequencer) {

        if (bufferSize < 1){
            throw new IllegalArgumentException("bufferSize must not be less than 1");
        }
        if (Integer.bitCount(bufferSize) != 1){
            //数组容量不符合要求就直接抛异常
            throw new IllegalArgumentException("bufferSize must be a power of 2");
        }
        //序号生成器赋值成功
        this.sequencer = sequencer;
        //数组容量赋值
        this.bufferSize = bufferSize;
        //给掩码赋值
        this.indexMask = bufferSize - 1;
        //初始化数组
        this.entries = new Object[bufferSize];
        //初始化数组，创建数组中的对象，这个对象可以一直复用，在一定程度上减少垃圾回收
        //但是该对象中封装的对象，仍然会被垃圾回收，这个要搞清楚
        fill(eventFactory);
    }

    //在该方法中初始化数组，创建了数组中的每一个对象，这样，等之后生产者
    //生产数据时，只要生产对象中持有的那个对象就行了，在我们的例子中
    //生产者就可以只创建Request对象中的Integer对象就行了
    private void fill(EventFactory<E> eventFactory){
        for (int i = 0; i < bufferSize; i++){
            entries[i] = eventFactory.newInstance();
        }
    }


    //新添加的非常重要的方法，该方法是发布生产者数据的核心方法
    public <A> void publishEvent(EventTranslatorOneArg<E, A> translator, A arg0){
        
        //首先获得可用的生产者序号，生产者发布的信息就可以存放到该位置
        final long sequence = sequencer.next();
        //然后在这个序号对应的位置发布数据
        translateAndPublish(translator, sequence, arg0);
    }

    //这个是新添加的方法，大家现在可能还不知道怎么用，马上我就会为大家讲解
    private <A> void translateAndPublish(EventTranslatorOneArg<E, A> translator, long sequence, A arg0){
        try{
            //这里的这个translator，其实就是上面引入的EventTranslatorOneArg接口的实现类
            translator.translateTo(get(sequence), sequence, arg0);
        }
        finally{
            //更新生产者进度，让消费者知道，现在可以实现了
            //生产者的进度就在这个方法中更新了
            sequencer.publish(sequence);
        }
    }

    
    //根据序号获得环形数组中对应的元素
    public E get(long sequence){
        return elementAt(sequence);
    }

    //该方法也很重要，就是根据序号得到数组相应位置的数据
    protected final E elementAt(long sequence) {
       return (E) entries[(int) (sequence & indexMask)];
    }

    //其他方法暂时省略
    
}
```

到此为止，之间讲解过的重要组件，都已经搬运到这里了。

**引入 Disruptor，组装各个组件**

所有的组件都已经再次引入一遍了，现在我们要做的就是利用这些组件，使我们的程序真正运行起来。所谓的运行就是让生产者干生产者事，让消费者干消费者的事。如果还是沿用前几章的例子，比如用户定义的消费者逻辑仍然没变，环形数组中存放的仍然是 Request 对象，而生产者真正需要生产的是 Integer 对象。生产者生产了 Integer 对象后，然后把这个对象交给 Request 对象即可。请看下面代码块。

首先是用户定义的消费者逻辑。

```
public class SimpleEventHandler<T> implements EventHandler<Request<T>> {
    
    @Override
    public void onEvent(Request<T> event, long sequence) throws Exception {
        //这里，我就只打印一句话了
        System.out.println("消费者线程"+Thread.currentThread().getName()+"已经消费了"+event.getData()+"数据了！");
    }
}
```

接下来是环形数组中存放的对象。

```
//该对象中的泛型，会在恰当的时机被定义成Integer类型的
public class Request<T> {

    private T data;

    public T getData() {
        return data;
    }

    public Request<T> setData(T data) {
        this.data = data;
        return this;
    }
}
```

下面代码块中展示的就是事件工厂，用来生产 Request 对象的。

```
public class SimpleEventFactory<T> implements EventFactory<Request<T>> {

    @Override
    public Request<T> newInstance() {
        return new Request<>();
    }
}
```

而消费者直接消费 Request 中的 Integer 对象即可。现在，先让我来为大家写一个测试类，看看我们会遇到什么困难。请看下面代码块。

```
public class Test {

    public static void main(String[] args) {

        //首先是生产者的逻辑
        
        //创建数据对象工厂对象，这个Integer，就是生产者真正要发布的数据
        SimpleEventFactory<Integer> eventFactory = new SimpleEventFactory<>();
        //定义环形数组的长度
        int ringBufferSize = 128;
        //创建一个序号生成器
        SingleProducerSequencer sq = new SingleProducerSequencer(ringBufferSize,消费者进度);
        //可以发现，创建序号生成器的时候，无法得到消费者进度，先不管了，直接向下执行吧。
        //创建环形数组
        RingBuffer buffer = new RingBuffer(ringBufferSize,eventFactory,sq);
        //创建传输器对象，也就是EventTranslatorOneArg接口的实现类
         EventTranslatorOneArg<Request<Integer>, Integer> eventTranslatorOneArg =
                new EventTranslatorOneArg<Request<Integer>, Integer>() {
                    @Override
                    public void translateTo(Request<Integer> request, long sequence, Integer arg0) {
                        //在这里就把Integer对象设置到Request对象中了
                        request.setData(arg0);
                    }
                };

        //环形数组发布数据发布128条数据，其实就是0-127这些数值，生产者一旦发布了数据
        //消费者就会直接去获取数据然后消费了，在这里也能看出来，生产者其实就是主线程
        for (int i = 0; i < 128; i++) {
             buffer.publishEvent(eventTranslatorOneArg,i);
        }



        //接下来是消费者的逻辑

        
        //创建一个消费逻辑，这里就用本章之前给出的SimpleEventHandler
        EventHandler<Request<Integer>> eventHandler = new SimpleEventHandler<>();
        //创建一个阻塞策略
        WaitStrategy wait = new SleepingWaitStrategy();
        //得到环形数组，上面已经创建了，直接赋值给DataProvider接口
        DataProvider bf = buffer;
        //得到生产者进度，这里就要考虑一下生产者进度该怎么获得了，上面我们已经获得了序号生成器
        //并且我们也知道生产者进度就是在序号生成器中初始化的，所以得到了序号生成器肯定就能得到生产者进度
        //大不了在序号生成器中定义一个get方法，把生产者进度暴露出来，对吧？所以，这里我们就当是得到了生产者进度
    	Sequence sequence = SingleProducerSequencer.get("生产者进度成员变量");
        //创建一个序号屏障
        ProcessingSequenceBarrier barrier = new ProcessingSequenceBarrier(wait,sequence);
        //创建消费者处理器
        BatchEventProcessor processor = new BatchEventProcessor(bf,barrier,eventHandler);
        //创建一个线程执行消费者处理器任务
        new Thread(processor).start();
    }
}
```

测试类终于写完了，还真是麻烦呀。写的过程中我就发现了，要创建的对象和定义的变量还挺多的，创建好了还不行，还要把它们按照顺序组装起来，就是谁引用谁，谁依赖谁，这个关系一定要搞清楚，要按照这个关系把对象或者变量传递到另一个对象中。简直是太麻烦了呀，并且在上面的代码块中还暴露出了一个问题，那就是当我实现生产者流程的时候，我发现必须要得到消费者进度，而当我实现消费者流程的时候，我必须要得到生产者进度。说实话，如果我没看过 disruptor 的源码，不知道 disruptor 类是怎么整合这些组件的，按照什么关系和流程整合这些组件的，我真的怀疑我前几章设计的代码十分愚蠢！如果我们什么都不做，就这样放任这个程序，你告诉我，有几个用户愿意使用你开发的框架？使用之前还得把框架的源码研究一遍，使用的时候还要把这几个组件按照正确的流程组合起来。如果真的有用户愿意使用你开发的框架，没别的原因，肯定是那个用户爱上你了。

其实问题已经暴露得很明显了，我们肯定不能就这样把这个程序交付给用户，最妥当的做法，就是也像 Netty 那样搞一个 ServeBootstrap 启动类，这个启动类就负责把各个组件拼接起来，所有复杂的操作和流程都封装在启动类内部，也许启动类内部还会调用其他类的方法，让调用链路向程序内部延伸。这样一来，用户只需要定义好自己的需要的组件，然后交给 ServeBootstrap 类，就可以高枕无忧地启动程序了。不过我肯定不能直接定义一个 ServeBootstrap 类，对吧，毕竟这个是 Netty 中的类，Disruptor 框架显然应该有自己的启动类，类的名字就叫 disruptor，说实话我也不明白怎么会起这么个名字，也许是有分配者的含义吧，扮演了把用户提交的组件分配到正确的地方。所以，接下来就让我直接来为大家引入这个 disruptor 类吧。

要引入这个类十分简单，直接创建一个新的类，起好类名就行了。难的是类中的内容该怎么定义，这个就要结合上面测试类的代码块来具体分析了。请大家仔细想一想，在上面的代码块中，我们先实现了生产者的流程，然后又实现了消费者的流程。请大家先别去考虑获取生产者和消费者进度顺序的问题，只思考一个问题，那就是生产者线程的启动和消费者线程启动的顺序问题。也就是下面这个代码块中呈现的内容。

```
public class Test {

    public static void main(String[] args) {

        //首先是生产者的逻辑
        
        //环形数组发布数据发布128条数据，其实就是0-127这些数值，生产者一旦发布了数据
        //消费者就会直接去获取数据然后消费了，在这里也能看出来，生产者其实就是主线程
        for (int i = 0; i < 128; i++) {
             buffer.publishEvent(eventTranslatorOneArg,i);
        }



        //接下来是消费者的逻辑

        //创建一个线程执行消费者处理器任务
        new Thread(processor).start();
    }
}
```

请大家想一想，当双方的组件都组合完成时，各种变量也都获得了，只需要启动生产者和消费者，程序就可以顺利执行了。那么我想问问大家，是先启动生产者线程呢？还是先启动消费者线程呢？换句话说，生产者线程和消费者线程的启动顺序对程序的顺利执行有影响吗？仔细分析一下就能得出结论，肯定是没有影响的。如果是生产者线程先启动，要把数据放到环形数组中，在存放的时候肯定会判断存放的进度是否会覆盖尚未被消费的旧数据，但这时候都没有旧数据，也就是说生产者进度还未填满环形数组一轮呢，可以放心大胆地存放旧数据，不必担心消费者是否消费了数据。当然如果一开始放入的数据就很多，很快就填充了环形数组一轮了，第二轮填充数据的时候就该考虑是否会覆盖旧数据了，如果这个时候消费者还未启动，那么生产者线程一定会阻塞。

换另一个角度，如果首先启动的是消费者线程，当消费者线程要去消费数据的时候，会和生产者的进度做对比，发现要消费的数据根本还没有填充呢，这时候，生产者线程就会直接阻塞，并且在阻塞策略 SleepingWaitStrategy 的 waitFor 方法中循环判断生产者的序号是否大于或者等于了消费者希望消费的序号，如果大于或者等于了，消费者线程就会结束阻塞，跳出循环，开始真正消费环形数组中的数据。所以，生产者线程和消费者线程的启动顺序不会对程序造成影响。这个问题解决了，我们就只剩下一个问题要解决了。那就是怎么在实现生产者流程时获得消费者进度，在实现消费者流程时，获得生产者进度。

其实这个问题其实并没有那么难解决，因为不管你这个人的选择困难症有多严重，最终你都要做出一个选择，那就是必须选出一个顺序，看看是先创建消费者，还是先创建序号生成器。因为消费者的进度就定义在 BatchEventProcessor 类中，只有创建了 BatchEventProcessor 类的对象，才能得到消费者进度。如果先创建消费者，消费者又要在创建的时候获得生产者进度，而生产者进度是定义在序号生成器中的，这样一来，好像就要先创建序号生成器了。反过来，如果要先创建序号生成器，就意味着要先获得消费者进度，但是这意味着要先创建 BatchEventProcessor 对象才行，可是创建 BatchEventProcessor 对象又需要生产者进度。这似乎是一个悖论，对吧？但真的是这样吗？如果我给序号生成器中的生产者进度成员变量定义一个 get 方法，给其中的消费者进度定义一个 set 方法，给 BatchEventProcessor 类中的消费者进度定义一个 get 方法，给 ProcessingSequenceBarrier 中的生产者进度定义一个 set 方法。那么，我先创建序号生成器时，把序号生成器中的消费者进度成员变量初始化为 0 即可，后面通过 get 方法得到了消费者进度后，然后再通过 set 方法给消费者进度赋值即可。当然，重构程序的时候，我也许不会真的定义 get 或者 set 方法，但是从这里确实可以意识到，先创建谁后创建谁其实没那么重要，都是可以通过一些方法来调整的。这样一来，好像问题也没那么难了，我们目前要做的，就是分析出组件创建的最佳顺序。其实这个顺序我也已经确定好了。让我们的目光再次回到之前那个测试类中，请看下面代码块。

```
public class Test {

    public static void main(String[] args) {

        //首先是生产者的逻辑
        
        //创建数据对象工厂对象，这个Integer，就是生产者真正要发布的数据
        SimpleEventFactory<Integer> eventFactory = new SimpleEventFactory<>();
        //定义环形数组的长度
        int ringBufferSize = 128;
        //创建一个序号生成器
        SingleProducerSequencer sq = new SingleProducerSequencer(ringBufferSize,消费者进度);
        //可以发现，创建序号生成器的时候，无法得到消费者进度，先不管了，直接向下执行吧。
        //创建环形数组
        RingBuffer buffer = new RingBuffer(ringBufferSize,eventFactory,sq);
        //创建传输器对象，也就是EventTranslatorOneArg接口的实现类
         EventTranslatorOneArg<Request<Integer>, Integer> eventTranslatorOneArg =
                new EventTranslatorOneArg<Request<Integer>, Integer>() {
                    @Override
                    public void translateTo(Request<Integer> request, long sequence, Integer arg0) {
                        //在这里就把Integer对象设置到Request对象中了
                        request.setData(arg0);
                    }
                };

        //环形数组发布数据发布128条数据，其实就是0-127这些数值，生产者一旦发布了数据
        //消费者就会直接去获取数据然后消费了，在这里也能看出来，生产者其实就是主线程
        for (int i = 0; i < 128; i++) {
             buffer.publishEvent(eventTranslatorOneArg,i);
        }



        //接下来是消费者的逻辑

        
        //创建一个消费逻辑，这里就用本章之前给出的SimpleEventHandler
        EventHandler<Request<Integer>> eventHandler = new SimpleEventHandler<>();
        //创建一个阻塞策略
        WaitStrategy wait = new SleepingWaitStrategy();
        //得到环形数组，上面已经创建了，直接赋值给DataProvider接口
        DataProvider bf = buffer;
        //得到生产者进度，这里就要考虑一下生产者进度该怎么获得了，上面我们已经获得了序号生成器
        //并且我们也知道生产者进度就是在序号生成器中初始化的，所以得到了序号生成器肯定就能得到生产者进度
        //大不了在序号生成器中定义一个get方法，把生产者进度暴露出来，对吧？所以，这里我们就当是得到了生产者进度
    	Sequence sequence = SingleProducerSequencer.get("生产者进度成员变量");
        //创建一个序号屏障
        ProcessingSequenceBarrier barrier = new ProcessingSequenceBarrier(wait,sequence);
        //创建消费者处理器
        BatchEventProcessor processor = new BatchEventProcessor(bf,barrier,eventHandler);
        //创建一个线程执行消费者处理器任务
        new Thread(processor).start();
    }
}
```

如果就按照我们目前的顺序来创建组件和整合组建的话，效果会怎样呢？假如我们要先实现消费者流程，那就意味着要先创建消费处理器了，但是在创建消费处理器的过程中，也就是上面代码块的第 38 到 52 行，我们会发现，不管怎样，都要先得到一个 DataProvider 类型的对象，其实也就是 RingBuffer 对象，所以说肯定要先创建一个 RingBuffer 对象，但是 RingBuffer 对象要持有序号生成器，也就意味着要先创建序号生成器，序号生成器创建了生产者进度也就有了，那么就可以直接把生产者进度交给 ProcessingSequenceBarrier 对象使用了。至于序号生成器要使用消费者进度，就等待后面消费者创建成功后再回过头赋值吧。所以，按照这个流程恰好就顺应了组件的依赖关系。所以，现在我要为程序引入的 disruptor 类就会按照这个顺序来整个各个组件。顺序就是：**先创建环形数组，在创建环形数组的过程中创建序号生成器，然后创建 ProcessingSequenceBarrier 序号屏障对象，然后创建 BatchEventProcessor 消费者处理器，最后把消费者进度赋值到序号生成器中。**当然，有一些组件也会根据我们之前的分析，进行一下微小的重构。首先要重构的就是 RingBuffer 类，这个类中添加了四个新的方法。请看下面代码块。

```
public final class RingBuffer<E> {
    //其他内容省略


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/24
     * @Description:创建环形数组和单生产者序号生成器的方法
     */
    public static <E> RingBuffer<E> create(EventFactory<E> factory,int bufferSize,WaitStrategy waitStrategy){
        
        //创建单生产者模式下的序列号生成器
        return createSingleProducer(factory, bufferSize, waitStrategy);
    }


    public static <E> RingBuffer<E> createSingleProducer(EventFactory<E> factory,int bufferSize,WaitStrategy waitStrategy){
        //创建单生产者序列号生成器，在这里可以看到序号生成器的构造方法变了
        //之前构造方法中的参数是环形数组额长度和消费者进度，现在变成了环形数组长度和阻塞策略
        //既然有了变动，说明序号生成器肯定也要重构了
        SingleProducerSequencer sequencer = new SingleProducerSequencer(bufferSize, waitStrategy);
        //创建环形数组
        return new RingBuffer<E>(factory, sequencer);
    }
    

    //为消费者创建序号屏障
    public ProcessingSequenceBarrier newBarrier(){
        //可以发现，这里实际上调用的是序号生成器的newBarrier方法，所以序号生成器也许添加一个方法
        return sequencer.newBarrier();
    }

    //把消费者的进度设置到序号生成器中
    public void addGatingSequences(Sequence sequence){
        //就是在序号生成器的addGatingSequences方法中设置的
        //这里大家肯定会对这个方法名有疑惑，说实话，本来可以定义set方法的
        //但是为了和源码方法名称保持一致，就弄成这个名字了
        //由此可见，我们现在所讲的内容，其实还很简单，并不是真正意义的源码
        sequencer.addGatingSequences(sequence);
    }
}
```

接下来就是序号生成器 SingleProducerSequencer 要进行的重构。

```
public final class SingleProducerSequencer{

    //下面三个是之前定义的成员变量
    protected final int bufferSize;

    //消费者的当前进度,这个是需要从外面传进来的
    protected final Sequence consumer;

    //生产者的当前进度，也就是生产者分配到第几个序号了，初始值为-1
    protected final Sequence cursor = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);

	//其他内容省略，下面就是新增的成员变量和重构的方法

    //增加了一个成员变量，那就是阻塞策略，之所以把阻塞策略放在这里，是要在
    //序号生成器中创建序号屏障，阻塞策略可以直接传递给序号屏障
    protected final WaitStrategy waitStrategy;

    
    //构造方法有所改变
    //构造方法
    public SingleProducerSequencer(int bufferSize, WaitStrategy waitStrategy){
        //在这里给环形数组容量和等待策略赋值
        this.bufferSize = bufferSize;
        this.waitStrategy = waitStrategy;
    }
    
    //新增的方法，为消费者创建序号屏障
    @Override
    public ProcessingSequenceBarrier newBarrier(){
        //这里穿进去两个参数，一个是阻塞策略，一个是生产者进度，这时候，序号屏障就获得了生产者进度了
        //一个难题解决了，只要最后把消费者的进度赋值给序号生成器就没事了
        return new ProcessingSequenceBarrier(waitStrategy, cursor);
    }

    //把消费者的进度赋值该成员变量consumer
    public final void addGatingSequences(Sequence sequence){
        this.consumer = sequence;
    }
    
}
```

接下来就没什么要重构的地方了，所以，我可以放心大胆地引入 disruptor 类了。为了尽可能的模仿 Netty 的 ServerBootStarp 类，我在 disruptor 类中还定义了一个 start 方法，并且引入了线程工厂，当调用该类的 start 方法时，线程工厂就会为每一个消费者创建线程，然后启动线程，这时候消费者就可以开始工作了。所以，在真正引入 disruptor 类之前，先让我把线程工厂为大家引入了。当然，说是线程工厂，其实这个工厂就是 JDK 中默认的 ThreadFactory 类，只不过被包装在一个名称为 BasicExecutor 的类中了。具体构造请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/17
 * @Description:该类就是用来给消费者创建线程的。每一个消费者都会被包装成BatchEventProcessor对象
 * 而BatchEventProcessor对象实际上继承了Runnable接口，所以在该类中就可以创建一个新的线程，然后把EventProcessor
 * 对象当作任务交给线程来执行
 */
public class BasicExecutor implements Executor
{
    //创建线程的工厂
    private final ThreadFactory factory;
    //该成员变量存储了创建的所有消费者线程，其实也没什么特别重要的意义
    //在该类的dumpThreadInfo方法中，会为存储的这些线程设置一些信息，方便toString时候输出
    private final Queue<Thread> threads = new ConcurrentLinkedQueue<>();

    //构造方法
    public BasicExecutor(ThreadFactory factory){
        this.factory = factory;
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/17
     * @Description:真正执行消费者任务的方法
     */
    @Override
    public void execute(Runnable command){
        //在这里创建了一个新线程，并且执行了Runnable任务
        //其实这里的Runnable任务就是EventProcessor对象
        final Thread thread = factory.newThread(command);
        
        if (null == thread){
            throw new RuntimeException("Failed to create thread to run: " + command);
        }
        
        //启动线程
        thread.start();
        //把线程添加到队列中
        threads.add(thread);
    }

     @Override
    public String toString(){
        
        return "BasicExecutor{" +
                "threads=" + dumpThreadInfo() +
                '}';
    }

    private String dumpThreadInfo(){
        final StringBuilder sb = new StringBuilder();
        final ThreadMXBean threadMXBean = ManagementFactory.getThreadMXBean();
        for (Thread t : threads){
            ThreadInfo threadInfo = threadMXBean.getThreadInfo(t.getId());
            sb.append("{");
            sb.append("name=").append(t.getName()).append(",");
            sb.append("id=").append(t.getId()).append(",");
            sb.append("state=").append(threadInfo.getThreadState()).append(",");
            sb.append("lockInfo=").append(threadInfo.getLockInfo());
            sb.append("}");
        }
        return sb.toString();
    }
   
}
```

实话实说，我当初一看到这样代码就觉得很熟悉，因为 Netty 中也有线程工厂对吧？所以我一看到线程工厂，就能猜到这个类究竟是要干什么。这大概也是编程套路的体现之一吧。

接下来就是 disruptor 类了。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/18
 * @Description:这个类是该框架的核心类之一，就相当于Netty的ServerBootStarp，虽然自己没有多么核心的逻辑要处理
 * 但是核心逻辑的启动方法都在该类中
 */
public class Disruptor<T>{

    //鼎鼎大名的环形数组
    private final RingBuffer<T> ringBuffer;
    
    //创建消费者线程的执行器，这里只是一个接口，其实创建的是它的实现类BasicExecutor对象
    private final Executor executor;

    //定义包装过后的消费者对象，源码中这里其实定义的是一个消费者仓库，专门存储所有的消费者
    //但我们第一版本是单消费者模式，所以就只定义一个对象了，原理都是一样的，后面的版本再重构就行
    BatchEventProcessor batchEventProcessor;

    //Disruptor启动状态的标记，因为在整个程序中要确保Disruptor只启动一次，因为所有消费者线程都是被Disruptor
    //的start方法启动的，Disruptor只启动一次，其实是保证了所有的线程只启动一次
    //Disruptor究竟要翻译成什么比较好呢？其实我也没有一个特别好的意见
    private final AtomicBoolean started = new AtomicBoolean(false);


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/18
     * @Description:构造方法，参数的具体作用写在下面的注释中了
     */
    public Disruptor(final EventFactory<T> eventFactory,final int ringBufferSize,final ThreadFactory threadFactory,final WaitStrategy waitStrategy){
        //create方法第一个参数就是事件工厂
        //第二个参数就是RingBuffer的容量
        //第三个参数就是等待策略
        //在这里创建真正存储数据的容器RingBuffer，创建RingBuffer对象的同时
        //序号生成器也就创建了，具体逻辑在RingBuffer类的create方法中				//创建BasicExecutor对象
        this(RingBuffer.create(eventFactory, ringBufferSize, waitStrategy),new BasicExecutor(threadFactory));
    }


     //在这里给Disruptor中的两个成员变量赋值
    private Disruptor(final RingBuffer<T> ringBuffer, final Executor executor){
        //环形数组类赋值
        this.ringBuffer = ringBuffer;
        //创建消费者线程的执行器赋值
        this.executor = executor;
    }



     /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/18
     * @Description:把用户定义的消费事件包装成一个个消费者runnable任务，这些任务就会交给线程来执行
     * 这个方法的参数是个可变参数，因为用户可能会定义多个消费者事件，也就是多个消费者，但在第一版本代码中
     * 只有一个消费者，因为是单消费者模式
     */
    public final void handleEventsWith(final EventHandler<? super T>... handlers){
        
        //这个方法就会把消费事件包装在事件处理器中，一个消费者对应着一个事件处理器，一个事件处理器最终会交给一个线程来执行
        createEventProcessors(handlers);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/19
     * @Description:该方法会把用户定义的消费者handler包装到EventProcessor对象中，是这个类中最重要的一个方法
     */
    void createEventProcessors(final EventHandler<? super T>[] eventHandlers){
        //检查Disruptor是否启动过，因为要保证Disruptor只能启动一次
        //创建EventProcessor对象的动作肯定要在Disruptor启动之前，所以这里必须检查一下
        checkNotStarted();
        //定义消费者进度对象
        Sequence sequence = null;
        //创建消费者的序号屏障
        final SequenceBarrier barrier = ringBuffer.newBarrier(barrierSequences);
        //把用户定义的每一个eventHandlers包装成BatchEventProcessor对象
        for (int i = 0, eventHandlersLength = eventHandlers.length; i < eventHandlersLength; i++){
            final EventHandler<? super T> eventHandler = eventHandlers[i];
            //创建BatchEventProcessor对象，每一个BatchEventProcessor对象都是一个消费者
            batchEventProcessor = new BatchEventProcessor<>(ringBuffer, barrier, eventHandler);
            //得到消费者对象
            sequence = batchEventProcessor.getSequence();
        }
    	//把消费者进度赋值给序号生成器中的成员变量
        ringBuffer.addGatingSequences(sequence);
    }

    //检查程序是否启动了，如果已经启动就抛出异常
    private void checkNotStarted(){
        if (started.get()){
            throw new IllegalStateException("All event handlers must be added before calling starts.");
        }
    }


    
    //发布生产者，其实就是发布数据让消费者去消费
    //这里大家也要弄清楚EventTranslatorOneArg的作用，环形数组初始化好的那一刻，其实环形数组中每一个
    //位置的事件对象都创建好了，只要程序不退出，这些对象是不会被垃圾回收的，而EventTranslatorOneArg
    //对象就是把发布者发布的真正消息传递到环形数组的每一个事件对象中去
    //在这里也能看到，publishEvent方法被Disruptor类代理了，然后才会调用ringBuffer中的同名方法
    public <A> void publishEvent(final EventTranslatorOneArg<T, A> eventTranslator, final A arg){
        ringBuffer.publishEvent(eventTranslator, arg);
    }


    //启动程序的方法，也就是启动每一个消费者线程
    public RingBuffer<T> start(){
        //确保程序只启动一次
        checkOnlyStartedOnce();
        //启动消费者线程
        executor.executor(batchEventProcessor);
        return ringBuffer;
    }

    //确保程序只启动一次
    private void checkOnlyStartedOnce(){
        
        if (!started.compareAndSet(false, true)){
            throw new IllegalStateException("Disruptor.start() must only be called once.");
        }
    }
    
}
```

到此为止，我就为大家把 Disruptor 类实现完整了。我相信这时候大家都还很迷糊，所以，接下来我就写一个测试类，帮助大家梳理一下代码的运行流程。按照以往的流程，其实现在大家去看第一版本源码就行了，但是现在实现的代码和第一版本代码还有很大的差别，所以大家暂时还看不了源码，所以我愿意多花点功夫，为大家详细剖析一下整个程序的运行流程。最后还会为大家配上衣服简图，展示各个组件的依赖关系。在学习测试类的时候，请大家一定要记着上面我为大家剖析的组件创建的流程，这个流程就是：**先创建环形数组，在创建环形数组的过程中创建序号生成器，然后创建 ProcessingSequenceBarrier 序号屏障对象，然后创建 BatchEventProcessor 消费者处理器，最后把消费者进度赋值到序号生成器中，**希望大家能带着这个流程学习测试类。请看下面测试类。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/8
 * @Description:单消费者单生产者的测试例子
 */
public class SingleConsumerTest {


    public static void main(String[] args) {
        //这也就意味着环形数组中存放的对象是Request对象
        SimpleEventFactory<Request> eventFactory = new SimpleEventFactory<>();
        
        //设置环形数组的长度，其实就是RingBuffer中数组的容量。注意，这个环形数组的长度必须是2的n次幂
        int ringBufferSize = 128;
        
        //创建线程工厂，这个线程工厂是专门创建消费者线程的
        ThreadFactory threadFactory = Executors.defaultThreadFactory();
        
        //设置阻塞策略
        WaitStrategy waitStrategy = new SleepingWaitStrategy();
        
        //创建disruptor对象
        Disruptor<Request<Integer>> disruptor = new Disruptor<>(eventFactory, ringBufferSize, threadFactory, waitStrategy);
        
        //创建消费者处理器，因为是单消费者模式，所以只创建一个消费者处理器即可
        EventHandler<Request<Integer>> eventHandler = new SimpleEventHandler<>();
        
        //把消费者设置到disruptor中，然后被包装成一个BatchEventProcessor对象
        disruptor.handleEventsWith(eventHandler);
       
        //现在就可以启动程序了，注意，启动程序，其实就是启动的消费者线程，但是现在很明显还没有数据被放到环形数组中
        //消费者是没有数据可以消费的，所以就会根据阻塞策略等待生产者的数据到来
        disruptor.start();
        
        //创建传输器对象，也就是EventTranslatorOneArg接口的实现类
         EventTranslatorOneArg<Request<Integer>, Integer> eventTranslatorOneArg =
                new EventTranslatorOneArg<Request<Integer>, Integer>() {
                    @Override
                    public void translateTo(Request<Integer> request, long sequence, Integer arg0) {
                        //在这里就把Integer对象设置到Request对象中了
                        request.setData(arg0);
                    }
                };
        
        //生产者发布128条数据，生产者一旦发布了数据，消费者就会直接去获取数据然后消费了
        for (int i = 0; i < 128; i++) {
            disruptor.publishEvent(eventTranslatorOneArg, i);
        }
        
        //终止程序先不实现
    }
}
```

上面的代码块就是我自己搭建的队列框架的最终测试类。接下来我就可以使用这个测试类为大家分析程序执行的流程了。在剖析程序执行流程之前，让我们再回顾一次组件创建和整合的流程：**先创建环形数组，在创建环形数组的过程中创建序号生成器，然后创建 ProcessingSequenceBarrier 序号屏障对象，再创建 BatchEventProcessor 消费者处理器，最后把消费者进度赋值到序号生成器中。**好了，接下来请大家跟着我的思路一起来看看程序是不是按照这个流程执行的。

当程序执行到上面代码块的第 12 行时，会创建一个生产者的数据工厂，专门用来生产 Request 对象，而环形数组中存放的就是 Request 对象。接下来程序会执行到第 15 行，定义环形数组的容量，接着又会创建出线程工厂和阻塞策略。然后就会在第 24 行创建 Disruptor 对象，所以程序就会来到这个对象的构造函数中。也就是下面的代码块。

```
public Disruptor(final EventFactory<T> eventFactory,final int ringBufferSize,final ThreadFactory threadFactory,final WaitStrategy waitStrategy){
    //create方法第一个参数就是事件工厂
    //第二个参数就是RingBuffer的容量
    //第三个参数就是等待策略
    //在这里创建真正存储数据的容器RingBuffer，创建RingBuffer对象的同时
    //序号生成器也就创建了，具体逻辑在RingBuffer类的create方法中				//创建BasicExecutor对象
    this(RingBuffer.create(eventFactory, ringBufferSize, waitStrategy),new BasicExecutor(threadFactory));
}
 //在这里给Disruptor中的两个成员变量赋值
private Disruptor(final RingBuffer<T> ringBuffer, final Executor executor){
    //环形数组类赋值
    this.ringBuffer = ringBuffer;
    //创建消费者线程的执行器赋值
    this.executor = executor;
}
```

在 Disruptor 的构造函数中，会根据传进来的参数创建好 RingBuffer 对象，而在创建 RingBuffer 对象的时候，会在 RingBuffer 类的 create 方法中，把序号生成器创建了。请看下面代码块。

```
public static <E> RingBuffer<E> create(EventFactory<E> factory,int bufferSize,WaitStrategy waitStrategy){
        
        //创建单生产者模式下的序列号生成器
        return createSingleProducer(factory, bufferSize, waitStrategy);
    }


public static <E> RingBuffer<E> createSingleProducer(EventFactory<E> factory,int bufferSize,WaitStrategy waitStrategy){
    //创建单生产者序列号生成器，在这里可以看到序号生成器的构造方法变了
    //之前构造方法中的参数是环形数组额长度和消费者进度，现在变成了环形数组长度和阻塞策略
    //既然有了变动，说明序号生成器肯定也要重构了
    SingleProducerSequencer sequencer = new SingleProducerSequencer(bufferSize, waitStrategy);
    //创建环形数组
    return new RingBuffer<E>(factory, sequencer);
}
```

这时候环形数组和序号生成器就都有了，生产者的进度也已经有了。等上面代码块的方法执行完后，最后又会回到 Disruptor 类中，给该类的 ringBuffer 和 executor 成员变量赋值。赋完值之后，程序又会回到测试类中，接下来就会执行测试类中的第 27 行代码，创建用户自己定义的消费者逻辑对象，接下来，就是最重要的一步了，执行第 30 行代码，把用户定义的消费者逻辑包装到 BatchEventProcessor 对象中。请看下面代码块。

```
public final void handleEventsWith(final EventHandler<? super T>... handlers){
    
    //这个方法就会把消费事件包装在事件处理器中，一个消费者对应着一个事件处理器，一个事件处理器最终会交给一个线程来执行
    createEventProcessors(handlers);
}


void createEventProcessors(final EventHandler<? super T>[] eventHandlers){
    //检查Disruptor是否启动过，因为要保证Disruptor只能启动一次
    //创建EventProcessor对象的动作肯定要在Disruptor启动之前，所以这里必须检查一下
    checkNotStarted();
    //定义消费者进度对象
    Sequence sequence = null;
    //创建消费者的序号屏障
    final ProcessingSequenceBarrier barrier = ringBuffer.newBarrier();
    //把用户定义的每一个eventHandlers包装成BatchEventProcessor对象
    for (int i = 0, eventHandlersLength = eventHandlers.length; i < eventHandlersLength; i++){
        final EventHandler<? super T> eventHandler = eventHandlers[i];
        //创建BatchEventProcessor对象，每一个BatchEventProcessor对象都是一个消费者
        batchEventProcessor = new BatchEventProcessor<>(ringBuffer, barrier, eventHandler);
        //得到消费者对象
        sequence = batchEventProcessor.getSequence();
    }
	//把消费者进度赋值给序号生成器中的成员变量
    ringBuffer.addGatingSequences(sequence);
}
```

在 createEventProcessors 方法中，程序就会先通过 ringBuffer.newBarrier(barrierSequences) 这行代码创建消费者需要的序号屏障，这时候序号屏障对象也有了，然后在上面代码块的第 20 行创建消费者处理器，创建的时候，就可以把序号屏障对象传递给消费者对象的构造函数了。这时候消费者处理器也有了，也就意味着消费者进度也有了。最后一步，就是代码块的第 25 行，把消费进度赋值给序号生成器中的成员变量。到此为止，各个组件创建和整个的流程已经执行完毕了。接下来，程序又会来到测试类中执行 disruptor.start() 代码，启动所有的消费者线程。在第一版本代码中，消费者线程只有一个，所以就会启动这个线程。虽然消费者线程启动了，但是没有数据可消费，所以线程还是会阻塞住，具体逻辑大家可以去 BatchEventProcessor 类中回顾。所以只启动消费者线程是远远不够的，接下来，程序就会在测试类中创建传输器对象，然后就可以真正发布生产者数据了。这些逻辑就留给大家自己看吧，我就不再赘述了。

当然，现在程序还有很多缺陷，比如我们只定义了启动消费者的方法，而终止消费者线程的方法根本就没有定义。这些就留在后面完善吧。总的来说，这张就讲解了一个知识点，就是引入 Disruptor 类，把各个组件按照最优的顺序整合到一起。文章之所以很长，是因为代码块比较多。如果大家觉得对这些类关系都掌握了，其实可以试着去看一看第一版本的代码，虽然还有很多不一样的地方，但是类关系是正确的，我已经给大家剖析完毕了。下一章，就会从性能优化的角度开始再次重构我们的程序，所有知识都会围绕着解决伪共享来讲解。这一章就到此为止吧。下面是我为这一章配的一幅简图，展示了各个类的依赖关系，并没有展示全部，只展示了关键的几个类。这个 Disruptor 框架的类关系并不能让人一目了然，所以这次我决定换另外一种画法来展示类关系。其实这些类互相依赖，或者说这个类中定义了哪个成员变量，而那个类中又定义了哪个成员变量，真正创建对象的时候，说到底只不过是持有哪个对象的引用而已。而对象是在堆中的，只有一个，所以，我这里就用堆来展示来关系吧。请看下图。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/26725125/1694853373409-8dec9488-d5d0-4195-9b09-c2f333237c35.jpeg)