上一章的结尾我为大家引出了消费者，告诉大家说我要包装消费者了。但是篇幅有限，只好把内容放到这一章讲。为了大家的思路能连贯，所以就不再多逼逼了，直接实现消费者！

**包装消费者**

我要包装消费者的原因很简单，因为之前我为自己的程序引入进度序号 Sequence 时，我说现在消费者有了自己的消费进度，看起来更加立体了，所以想包装它一下。但是现在我想再跟大家提出另一个要包装它的重要原因。这个原因当然也很简单，那就是消费逻辑都是用户自己定义的，这就决定了它的身份比较特殊，不能只是一个简单的线程执行一个任务。当然，最后用户定义的消费者逻辑，或者说消费者任务肯定是要交给线程执行的。但是，从用户提交消费者任务，到线程最终开始执行这个任务之间会发生什么呢？让我来给大家分析一下。

比如说，还是沿用上一章的例子，环形数组中存放的都是一个个 Request 对象，而生产者真正生产的是 Integer 对象。这就意味着消费者最后要消费的就是这个 Integer 对象。比如说消费者的消费逻辑就是得到这个 Integer 对象，然后在控制台输出一句“我消费了第+Integer+个数据了”，如果是这样的话，那么这个消费逻辑是不是就应该定义到一个 Runnable 当中，然后把这个 Runnable 交给一个消费者线程来执行？那么在线程执行这个任务之前，是不是应该知道，这个任务消费的应该是环形数组中的哪个数据，对吧？所以，查找可以消费的进度就是包装后的消费者的职责，这个逻辑肯定不能让用户来实现吧，因此，消费者必须要让程序开发者事先包装一下。而且，进一步考虑，在单生产者和消费者模式下，消费者线程只有一个，但是环形数组中的数据会有很多，消费者线程只执行一个 Runnable 任务，怎么可能把所有数据都消费了呢？因此，不用想也知道，用户定义的这个消费者任务肯定是要在一个循环中执行的。在这个循环中，消费者在每一次循环的时候都会先得到可以消费的进度，然后再执行这个 Runnable。也就是说，有多少可以消费的进度，就要执行多少次 Runnable 任务。而每消费一个数据，消费者的消费进度就会加 1。这个逻辑，应该很清楚了吧？学过 Netty 和 XXL-JOB 之后，我相信这点逻辑对大家来说应该都是最常规的逻辑了。至于用户怎么定义消费者的逻辑，那就更简单了，暴露一个接口给用户就行了，接口中只定义一个方法。现在，大家应该明白了为什么要包装消费者了吧？如果都明白了，那么记下来，就让我用代码来实现刚才为大家分析的逻辑。请看下面的代码块。

首先定义一个暴露给用户的接口，让用户实现消费者逻辑。接口就定义为 EventHandler。

```
//消费者的事件处理器，专门用来处理生产者发布的数据的
public interface EventHandler<T>{

    void onEvent(T event, long sequence) throws Exception;
}
```

然后再定义一个接口，这个接口继承了 Runnable，其实现类就是线程要执行的任务。接口名称就定义为 EventProcessor。

```
public interface EventProcessor extends Runnable{
    //在这个接口中，我定义了一个获得消费者进度的方法
    Sequence getSequence();
}
```

接下来就是 EventProcessor 接口的实现类，而这个实现类就是包装过后的消费者。这个实现类叫做 BatchEventProcessor。意思为批处理器的意思，单线程消费者就可以用这个处理器来消费环形数组中的所有数据，所以叫批处理。

```
public final class BatchEventProcessor<T> implements EventProcessor{


    //用户定义的消费handler，在该类的run方法中，其实执行的也是该handler中实现的方法
    private final EventHandler<? super T> eventHandler;


    //消费者的消费序号，其实就是消费的进度
    //默认初始值为-1，大家可以看一下上一章定义的Sequence对象，里面有一个初始值-1
    //就是INITIAL_CURSOR_VALUE这个成员变量
    private final Sequence sequence = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);

    //构造方法
    public BatchEventProcessor(final EventHandler<? super T> eventHandler){
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
        //在一个循环中消费所有能消费的数据
        while (true){
            //我也不知道具体会发生什么，但是让我们按照最常规的思路来分析一下
            //首先，我先让消费者获得下一个可以消费的进度
            //sequence是当前消费者自身的消费进度，这个get方法得到的就是当前的消费进度
        	//因为消费进度是long整数，所以要+1L，这样就能得到下一个应该消费的进度了
        	//举个例子，现在环形数组中前5个索引位置都有待消费的事件
        	//当前消费者消费到第3个了，那么加一得到的就是要消费的第四个
        	long nextSequence = sequence.get() + 1L;
            //得到了这个可以消费的序号之后，是不是就应该和环形数组的掩码计算数组下标，然后
            //得到消费者数据，接着消费就行了，可是这里无法得到环形数组呀，那这样好了，就假装
            //这里可以得到，先把环形数组放在这里,环形数组get方法逻辑，大家忘了可以去上一章
            //回顾一下，get方法就可以根据序号进度得到数组对应位置的对象
            T event = ringBuffer.get(nextSequence)
            //得到了这个对象就该消费对象中的数据了呀
            //这里的这个eventHandler不就是构造函数中传进来的用户自己定义的消费逻辑吗？
            //在这里用户自己定义的消费逻辑就执行了
            eventHandler.onEvent(event, nextSequence);
            //消费完了之后，再把当前消费者的消费进度赋值成最新的
            sequence.set(nextSequence);
            //然后就可以进入下一轮循环，消费下一个进度了
        }
    }
}
```

在上面的 processEvents 方法中，我把包装过后的消费者的逻辑大概梳理了一下，写了一段为代码。如果说用户真的定义了一个消费逻辑，实现了 EventHandler 接口，然后把消费逻辑传递到 BatchEventProcessor 对象中，再创建一个消费者线程去执行这个 BatchEventProcessor 对象。就比如说下面这段代码。

```
public class SimpleEventHandler<T> implements EventHandler<Request<T>> {

    @Override
    public void onEvent(Request<T> event, long sequence) throws Exception {
        //这里，我就只打印一句话了
        System.out.println("消费者线程"+Thread.currentThread().getName()+"已经消费了"+event.getData()+"数据了！");
    }
}

//创建SimpleEventHandler对象
EventHandler<Request<Integer>> eventHandler = new SimpleEventHandler<>();

//创建批处理器
EventProcessor processor = new BatchEventProcessor(eventHandler);

//创建消费者线程执行任务
new Thread(processor).start();
```

消费者线程显然就会立刻执行到 BatchEventProcessor 的 processEvents 方法中。伪代码的逻辑似乎没什么差错，逻辑还是很顺畅的。但是，逻辑没有差错并不意味着逻辑就是完美的，最直接的一个缺陷就是，消费者线程真的可以一直消费下去吗？

**分析 BatchEventProcessor 的缺陷**

当然，消费者线程很可能会一直消费下去，只要环形数组中有可以消费的数据，消费线程就在循环中永远不必停歇。大家也看到了，这种假设成立的前提是环形数组中一直有数据可以消费。可假如环形数组中没有数据了呢？或者说生产者的速度赶不上消费者的速度了呢？显然，消费者线程就该阻塞一会，对吧？可是在我们上面的代码块中，根本没有先消费者线程阻塞的逻辑，换句话说，当消费者拿到了下一个要消费的进度序号后，根本就没有判断这个序号是不是可用的，对吧？这显然是一个重大的缺陷。所以，在 long nextSequence = sequence.get() + 1L 这行代码之后，显然要补充一段逻辑，就是消费者根据自己的消费进度得到了下一个想要消费的序号，这时候，应该拿这个序号去和生产者的进度做对比，如果消费者进度小于生产者的进度，显然就是可以消费的，如果消费者想要消费的进度，大于生产者当前的进度了，说明生产者还没生产到这里呢，因此，就要让消费者线程阻塞一段时间。这个逻辑我想大家理解起来应该也没什么问题吧？那么这是不是就意味着阻塞策略就要派上用场了？在上一章重构好的阻塞策略，在这里就终于有用武之地了。

好了，除了线程阻塞的问题，processEvents 方法中还存在哪些缺陷呢？很明显，不用我说大家也知道，那就是 processEvents 方法中用到了环形数组 RingBuffer，根据得到的可用的消费者序号去环形数组中获得对应下标位置的对象，但是在 BatchEventProcessor 类中显然得不到 RingBuffer 对象。这该怎么办呢？很简单呀，既然得不到，那就让它得到好了，在 BatchEventProcessor 中定义一个 RingBuffer 成员变量就好了，在创造 BatchEventProcessor 的时候，把 RingBuffer 对象的引用传递进来不就好了？这个也很简单吧。但是，这里我有一个疑问，我们都知道，在包装过后的消费者中，其实只需要用到 RingBuffer 类的 get 方法就行了。具体方法逻辑如下。

```
//根据序号获得环形数组中对应的元素
public E get(long sequence){
    return elementAt(sequence);
}


protected final E elementAt(long sequence) {
   return (E) entries[(int) (sequence& indexMask)];
}
```

而且，我也不希望消费者拿到 RingBuffer 对象后，还能操纵 RingBuffer 对象的其他方法，因为说到底 RingBuffer 对象的其他方法大多和生产者有关系，按照身份和隔离原则来说，本来这都不是消费者应该接触到的。所以我想了一下，能不能即把 RingBuffer 对象的引用传递到消费者中，同时只能让消费者持有的 RingBuffer 只操作 get 方法，其他的方法都不能使用呢？解决的思路很简单，那么究竟可行吗？实际上也是可行的。这时候就又该接口闪亮登场了，让一个类实现多个接口，就意味着这个类同时拥有了不同的功能，各个接口定义的方法不同，那么这个实现类在根据接口转换不同的身份后能使用的方法也就受到了限制。类中的方法肯定是最全面的，实现了所有接口中的方法，但是各个接口中的方法就没那么全面了。当然，在我们目前的程序中，RingBuffer 还没有实现任何接口，身份是唯一的，就是 RingBuffer 本身，其对象可以使用类中的所有方法。那么现在，我定义一个 DataProvider 接口，意思为数据提供，该接口中只有一个 get 方法。请看下面代码块。

```
//环形数组会实现这个接口
public interface DataProvider<T>{
    T get(long sequence);
}
```

然后我让环形数组 RingBuffer 实现这个接口。

```
public final class RingBuffer<E> implements DataProvider<E>{

    //其他方法暂时省略
    
    //根据序号获得环形数组中对应的元素
    @Override
    public E get(long sequence){
        return elementAt(sequence);
    }

    //该方法也很重要，就是根据序号得到数组相应位置的数据
    protected final E elementAt(long sequence) {
       return (E) entries[(int) (sequence& indexMask)];
    }
    
}
```

根据上面代码块中展示的内容，当 RingBuffer 的身份为 DataProvider 时，就只能调用 get 方法了。如果我在消费者 BatchEventProcessor 类中定义 DataProvider 类型的成员变量，然后再把 RingBuffer 的引用传递进来，这样一来，在 BatchEventProcessor 中的 RingBuffer 就只能调用它的 get 方法，其他方法都不能调用。这么做的话，不就做到了生产者和消费者的功能隔离了吗？消费者就算持有 RingBuffer，但是也无法使用 RingBuffer 中任何与生产者相关的方法。很完美，已经解决了，good！

很好，又解决了一个问题，这时候，我想 BatchEventProcessor 的 processEvents 方法中应该不会再有什么缺陷了吧。线程组的逻辑也分析了，通过环形数组得到消费数据的逻辑也分析了，消费完数据就可以进入下一轮循环了，看起来 processEvents 方法确实没什么缺陷了。但是，想必大家都有这个意识了，只要出现了但是，就意味着又有什么幺蛾子了。那么，请大家想一想，目前在我的消费者处理器 BatchEventProcessor 的 processEvents 方法中，是怎么消费数据的呢？是不是每循环一次，消费的进度就加 1，然后根据这个序号去和生产者进度做对比，如果可以消费，那就直接消费这个进度的数据。这样一来，就意味着没循环一次，就消费一个环形数组中的数据，如果环形数组中有 1024 个数据要消费，那消费者线程就要循环 1024 次才能消费完一轮。这样是不是就有点慢了？

为什么说慢呢？因为在程序中很可能出现这种情况，那就是生产者的速度很快，比如说生产者的进度已经到达 32 了，消费者的进度才到达 6，如果让消费者根据自身的当前进度判断，那么下一个可以消费的进度就是 7，得到了 7 这个进度序号之后要和生产者的当前进度做对比，当然是小于生产者进度的，所以消费者就可以直接去消费进度 7 的数据了。但是，这个时候生产者的进度明明已经到了 32 了，消费者本来可以直接把 7 到 32之间的所有数据全都消费了，然后再把自身的消费进度设置为 32。但是按照目前程序的设计，消费者必须消费完 7 了，再判断 8 能不能消费，再判断 9，显然很拖沓。我希望消费者可以一次就判断出最大的可消费的进度，然后直接消费到这个进度，之后再用这个最新的进度去判断下一次可消费的最大进度。举个简单的例子就是，当前消费者的消费进度为 6，生产者进度为 32，那么消费者在一次循环中直接判断出最大的可消费的进度为 32，然后在这次循环中把 7 - 32 的数据全都消费了，这时候，消费者的消费进度就会被设置为 32 了。接着在下一次循环中，再去判断能消费的最大进度，如果这时候生产者的进度是 63 了，那么，消费者就直接把 33 - 63 的数据全都消费了，然后进入下一次循环。这样才能体现出批处理的特性呀。

按照上面的分析的几种情况，就该再次重构我的 BatchEventProcessor 类了。请大家带着对以上几种情况的分析来阅读下面的代码。请看下面的代码块。

```
public final class BatchEventProcessor<T> implements EventProcessor{


    //用户定义的消费handler，在该类的run方法中，其实执行的也是该handler中实现的方法
    private final EventHandler<? super T> eventHandler;


    //这个成员变量会被环形数组赋值
    private final DataProvider<T> dataProvider;
    
    //消费者的消费序号，其实就是消费的进度
    //默认初始值为-1，大家可以看一下上一章定义的Sequence对象，里面有一个初始值-1
    //就是INITIAL_CURSOR_VALUE这个成员变量
    private final Sequence sequence = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);

    //构造方法
    public BatchEventProcessor(final DataProvider<T> dataProvider,final EventHandler<? super T> eventHandler){
        this.dataProvider = dataProvider;
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
        //先定义要消费的数据对象
        T event = null;
        //根据当前的消费进度，得到下一个要消费的进度序号
        //比如当前消费到4了，加1得到下一个要消费的5
        //但是这个5能不能消费，还要和生产者的进度对比一下
        long nextSequence = sequence.get() + 1L;
        //在一个循环中消费所有能消费的数据
        while (true){
            //这里就可以拿着得到的要消费的进度序号，去和生产者进度做对比
            //对比的时候，如果判断序号大于生产者进度了，就意味着这个序号不能消费
            //消费者线程需要阻塞一会。如果对比的时候发现消费者要消费的进度小于生产者
            //进度了，比如消费者要消费的是5，生产者的进度是9，那就直接把9返回给消费者
            //让消费者知道当前可以消费的最大进度是9。可是这一段逻辑该怎么实现呢？
            //算了先不实现了，就写一行伪代码吧，假装得到了可以消费的最大进度
            final long availableSequence = System.out.println("用得到的 nextSequence 的值去和生产者进度对比，也许阻塞，也许不阻塞，也许是阻塞完了，总之得到了可以消费的最大进度");
            	//下面就是批处理的逻辑了，在一个循环中判断，直到消费者要消费的进度等于刚才返回的最大的进度，就意味着
                //不能再消费了，因为生产者可能还没有继续发布数据
                while (nextSequence <= availableSequence){   
                    //之前在构造方法中说过，其实dataProvider就是环形数组
                    //这里就是根据序号从环形数组中把生产者发布的事件取出来消费
                    event = dataProvider.get(nextSequence);
                    //真正消费事件的方法，就是我在测试类中定义的SimpleEventHandler
                    eventHandler.onEvent(event, nextSequence);
                    //下一个要消费的进度加1，注意，如果要退出这个小的while循环时，下面nextSequence
                    //的自增，其实就已经得到了消费者要消费的下一个进度，然后再用这个进度去执行51行的代码
                    nextSequence++;
                }
            //消费完了之后，再把当前消费者的消费进度赋值成最新的，之后进入下一轮循环
            //在这里也能看到，消费者的进度，就是在这更新的，仍然是消费者线程操作自己的消费进度
            sequence.set(availableSequence);
        }
    }
}
```

我想上面代码块的逻辑应该也不难理解，毕竟之前已经为大家分析了逻辑重构点。现在对我的程序来说，唯一棘手的地方就是，我还不知道怎样在消费者中根据消费者将要消费的进度，去和生产者进度做判断，然后处理之后的操作；比如是让消费者线程阻塞，还是返回当前最大的可消费的进度，也就是生产者的最新进度。唉，问题总是一个接着一个，说实话，我已经有些倦怠了。当然，我并不是对敲代码，对编程倦怠了，我是对遇到的这些问题倦怠了。太没意思了，太无聊了，都是相同的问题，没一点新意。某个时刻，我真希望这些问题能朝我大脑猛攻，这么久了，没有一个问题能难倒我。总是重复解决相同的，简单的问题，太消磨我的意志了。

**引入序号屏障 ProcessingSequenceBarrier，重构 BatchEventProcessor**

既然消费者必须知道生产者的进度，好让自己要消费的进度去和生产者的进度做对比，那就让消费者知道不就好了？直接在消费者的处理器中再定义一个生产者进度的成员变量，这样消费者不就能一直知道生产者的进度了吗？然后再进行一系列判断和操作就行了。没错，如果在单生产者消费者模式下，采用最直接简单的处理方式，肯定是毫不犹豫地在消费者中定义一个生产者进度的成员变量，因为进度序号是 Sequence 对象包装的，所以对方有了改动，消费者肯定能感知到。这样不就知道生产者的进度了吗？但是，既然是这样，那我能不能单独把这个功能抽取出来，设计成一个组件呢？这样一来，BatchEventProcessor 类中就不用定义太多的成员变量，方法也会简洁清晰很多，从类结构和职责上来说会更加明确。我其实就可以让这个组件持有生产者的进度，然后在内部方法中让消费进度和生产进度做对比，看看线程是否需要阻塞，如果不需要阻塞，那就返回消费者可以消费的最大进度。可以看到，这个组件其实还是更偏向于为消费者服务。那么消费者肯定就要持有这个组件了，消费者进度也要传递到组件的方法中，这样才能让生产者和消费者的进度做对比呀。当然，我想把功能抽取出来单独设计一个组件，并不仅仅是这点原因，我这么设计其实也是考虑了程序以后的发展，因为我们的程序终究是要完善到多生产者和多消费者模式的，也会引入消费者顺序依赖的消费方式，到那个时候，大家就会发现引入这个组件的真正好处了。好了，文字分析已经写了很多了，下面就让我来真正引入这个组件吧。请看下面的代码块。

我就把引入的这个组件定义为 ProcessingSequenceBarrier，意思为序号屏障。也就是说判断这个序号是否可用的。如果可用才分发给消费者。

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

接下来，我再把之前的阻塞策略搬运过来。当然，在内容上也要做一点小小改动了。

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

    //这个方法改动了，有了返回值，参数也有了变化
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

接下来，我们再把消费者 BatchEventProcessor 进行最后一步重构。请看下面代码块。

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

到此为止，关于消费者的所有逻辑，我就都为大家讲解完了。如果要写一个测试例子，梳理消费者的运行流程，就按下面的方式创建测试类即可。请看下面代码块。

```
public class Test {

    public static void main(String[] args) {
        //创建一个消费逻辑，这里就用本章之前给出的SimpleEventHandler
        EventHandler<Request<Integer>> eventHandler = new SimpleEventHandler<>();
        //创建一个阻塞策略
        WaitStrategy wait = new SleepingWaitStrategy();
        //得到生产者进度和环形数组，但是该怎么获得呢？这是个问题，就当作这里得到环形数组和生产者进度吧
        环形数组 = ringBuffer;
        //序号屏障
        生产者进度 = sequence;
        //创建一个序号屏障
        ProcessingSequenceBarrier barrier = new ProcessingSequenceBarrier(wait,sequence);
        //创建消费者处理器
        BatchEventProcessor processor = new BatchEventProcessor(ringBuffer,barrier,eventHandler);
        //创建一个线程执行消费者处理器任务
        new Thread(processor).start();
    }
}
```

和消费者有关的所有组件我都定义在上面的测试类中了，具体的逻辑我就不为大家梳理了，否则要写的内容就太多了。大家用语雀的网页版直接学习吧，这样也方便学习文中的代码。目前大家还不够去直接看第一版源码，因为目前程序还有很多一些组件没有引入，即便是引入了的，有些小细节和第一版本的源码也不一样。还要讲解好几章，才能去看第一版本源码。所以就先辛苦大家看文章中的代码了。当然，在上面的测试类中，有两个非常重要的组件我写成了伪代码。因为我还不知道怎么得到。从这里大家应该也能稍微感觉到了，类结构和关系其实已经开始变得复杂了。所以，我才给大家讲得这么细，甚至有些啰嗦，一点点地演变迭代，就是担心讲得快了，大家不明白类关系。类关系要是不明白，源码也就无从学起呀。在我最后为大家引入第一版本的所有组件后，会为大家画一张类关系的结构图，大家可以结合着那张图回顾这几章的知识，等把知识彻底掌握了，就直接撸源码吧。

好了，偏题有点远了。说了这么多，上面代码块中最重要的两个问题都还没解决呢，一个就是不知道怎么得到生产者的进度，另一个就是不知道怎么得到环形数组。这两个组件如果得不到，程序就根本无法向下运行。但是也不必着急，让我们想一想，看看有没有什么方法能得到这两个组件。首先看看环形数组吧，之所以看它，是因为在上一章我已经为自己的程序引入了环形数组了。现在要得到环形数组，最直接的方法其实就是直接创建一个环形数组，也就是创建一个 DataProvider 对象，再把这个对象交给包装过后的消费者 BatchEventProcessor。所以说，得到环形数组的问题还算是容易解决的。那么生产者进度呢？这个该怎么解决？这就比较麻烦了，我一直在跟大家说生产者进度，但是连生产者怎么包装，我都还没跟大家说过呢，或者说，我还从来没跟大家讨论过生产者用不用包装的问题。

**引入序号生成器 SingleProducerSequencer**

那就让我们来分析一下，生产者究竟用不用包装，请大家想一想，通常来说，生产者干的事无法就是发布一个数据，把数据放到环形数组对应的位置。这个过程中有一个最核心的逻辑，不是创建要发布的数据对象，也不是把数据放到环形数组对应的位置。而是获取下一个可用的生产序号，然后判断这个序号是否可用，实际上，这个就是要判断生产者将要生产的数据是否会覆盖尚未消费的数据，如果会覆盖，那么就让生产者线程阻塞一会。请大家仔细品味一下，现在我描述的这个功能，是不是和刚才给消费者定义的序号屏障 ProcessingSequenceBarrier 的功能类似。只不过序号屏障 ProcessingSequenceBarrier 是判断消费者能否消费进度，不能消费则阻塞；而我刚才描述则是判断生产者是否能够发布数据，不能的话则阻塞生产者线程。太像了，真的，此时此刻，恰如彼时彼刻。既然这样，那我就不客气，我想反正现在程序还残缺着，能完善一点是一点，尽管生产者还没有，但是我十分愿意先把这个专门为生产者打造的功能组件定义出来。这个组件的功能就是用来判断生产者希望使用的下一个序号究竟能不能用，如果和消费者进度对比之后发现能用，就把这个序号分发出去，交给生产者使用，如果不能使用，则意味着继续发布数据会覆盖尚未被消费的旧数据，这样的话就需要让生产者线程阻塞一会。既然这样，这个组件肯定也要知道消费者的进度吧？就像 ProcessingSequenceBarrier 组件必须知道生产者的进度。很好，既然现在仍然是在单生产者模式下，而这个组件又是用来为生产者生成可使用的进度序号的，那我就把这个组件称为单线程序号生成器，类名定义为 SingleProducerSequencer。请看下面代码块。

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
    public long next(){   //这里可以看到，默认只申请一个
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

上面就是 SingleProducerSequencer 组件的所有内容，当然，我相信大家现在肯定还不清楚这个组件究竟要怎么用，在哪里使用。就算看懂了组件内部的逻辑，也还是一头雾水。不要担心，紧接着我就会给我自己的队列框架画上点睛的一笔。请大家想一想，上面这个组件主要是用来为生产者生产可用序号的，如果不可分发，生产者线程久阻塞一会，如果可分发，就把序号交给生产者使用。所以，要想弄清楚这个组件的用法，就必须知道生产者在哪里，生产者怎么使用这个组件，或者我们就直接查看，在程序之前的组件中，哪里需要使用生产者进度序号呢？答案已经很明显了，如果大家还记得上一章我为大家实现的那个残破的环形数组 RingBuffer 类，大家应该就能回忆起来，那个类中有两个地方还未实现。请大家看下面的代码。

```
public final class RingBuffer<E> {

    //其他的内容省略

    //新添加的非常重要的方法，该方法是发布生产者数据的核心方法
    public <A> void publishEvent(EventTranslatorOneArg<E, A> translator, A arg0){
        //首先获得可用的生产者序号，生产者发布的信息就可以存放到该位置
        //比如说当前生产者进度是25，next方法就会返回26让生产者使用
        //当然前提是消费进度跟得上，不过next方法我还没有实现
        final long sequence = next();
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
            //更新生产者进度，让消费者知道，暂时还无法实现
        }
    }


    //这个方法其实可以先略去了，因为这时候大家应该都知道，其实环形数组持有了序号生成器，然后
    //调用序号生成器获得的下一个可用的生产者序号
    public long next(){
       //方法赞不实现，因为还不知道怎么获得生产者序号，更别说怎么获得下一个可用的生产者序号了
    }
    
}
```

现在，我要补全这个漏洞了，因为我意识到 RingBuffer 不就是生产者中的一环吗？生产者线程只要拿到了 RingBuffer 对象，就可以发布数据了，这就意味着生产者开始工作了，生产者不需要其他包装了，只要给 RingBuffer 引入单线程序号生成器，一切问题就都迎刃而解了。请大家看下面的代码块，最好每一行都看一下。

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
       return (E) entries[(int) (sequence& indexMask)];
    }

    //其他方法暂时省略
    
}
```

到此为止，好像一个圆环终于首尾相接了。说实话，写到这里我也已经很累了，一周连续七天不间断地写，每天晚上都在写，前三篇文章写了有 4 万字。当然，字数多不能说明什么，但是构思讲解的思路，翻来覆去地修改文章的讲解流程，都是很耗时。因为 disruptor 这个框架本身类结构和关系就比较乱，当然，这种乱是一眼看上去非常乱，但是仔细梳理，会发现逻辑非常清晰，你会觉得每个变量出现的位置恰到好处。我所做的，就是要为大家把这个清晰的逻辑梳理出来，展现出来，迭代出来。

当然，现在的程序还是不完整，组件并没有全都引入，但是相对于第一版本源码来说，所有重要的组件其实也就差一个 Disruptor 类没有引入了。这个类其实就相当于 Netty 中的 ServeBootstrap，只是起到一个封装串联所有组件的作用。当然，Disruptor 这个类引入之后，之前为大家引入的各个组件的内容以及成员变量还会有一些变动。但是，大的变动不会太多了。当然，我讲解的过程中几乎把所有接口都略去了，实际上这些组件都有实现的接口，但是为了讲解流程考虑，只好先略去接口，等大家把这些组件都理清楚了，把程序的执行入口以及中间会发生什么都掌握了，到时候就可以直接去源码中学习了。

本来我是想为大家再写一个测试类，把引入的这些组件都串联一下。但是想了想还是先不写了，一个是实在是累了，还有一点就是下一章就要引入 Disruptor 了，既然要串联起所有组件，不如就等 Disruptor 引入了，我把组件的各个细节以及内部定义的变量真正完善了，再写一个测试类，把整体的逻辑和程序执行流程梳理一遍。到时候还会为大家配上一幅类关系的简图，帮助大家理解这个框架的核心流程。其实这里我不写测试类，大家也可以结合这几章的知识自己发散一下思维了。

比如说，我们讲到生产者进度是因为序号屏障要用到生产者进度，然后根据生产者进度又演变出了生产者序号生成器，而生产者进度是定义在生产者序号中的，那大家是不是可以大胆猜测一下，序号屏障这个 ProcessingSequenceBarrier 类的对象，很有可能就是在生产者序号生成器中创建的呀，这样才能直接在序号生成器内部把生产者进度传递到序号屏障对象中。我当然不是胡乱猜测，源码中就是这么写的。大家不妨自己再思考一下其他的组件，该怎么串联起来。比如说序号生成器中的消费者进度是怎么获得的。总之，大家可以先发散思维想一想，这些知识下一章都会讲解。

当下一章讲解完了，我就会为大家分析程序性能方面的一些问题。然后就要真正开始讲解高速缓存和内存对齐的知识，为讲解字节填充解决伪共享做铺垫。好了，这一章就到这里吧，我们下一章见。