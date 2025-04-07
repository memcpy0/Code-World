朋友们，大家好，这一章我将为大家讲解 Disruptor 框架最后一个核心功能的实现，那就是顺序消费功能。我相信如果大家根本就没学过 Disruptor 框架的实现原理，这个顺序消费的功能非常有可能直接把你唬住。但是既然都学到第十章了，肯定对 Disruptor 框架的实现原理已经了如指掌了，现在再来学习顺序消费肯定是易如反掌了。因为这一章只有一个知识点，所以我也就不起什么小标题了，接下来我们直入主题，开始实现顺序依赖消费功能。

在实现这个功能之前，我们先来明确一下，顺序依赖消费功能的使用情景，简单来说，顺序依赖消费功能是无法在并发消费模式下使用的，因为并发消费模式下是多个消费者线程共同消费一批数据，已经被消费过的数据不可以再被其他消费者线程消费，但顺序依赖消费就意味着这个数据显然是可以被多个消费者线程重复消费的，只不过消费有先后顺序。所以，这里我们就已经明确了，顺序依赖消费功能只能在并行消费模式下使用，也就是说，只能在 BatchEventProcessor 这个消费者批处理器中体现出顺序消费的功能。

那么，接下来我再来为大家分析一下，顺序依赖消费的本质是什么。直接解释顺序依赖消费其实很简单，就比如说程序中有三个消费者线程，分别为 1、2、3，环形数组中有很多数据待消费，每个消费者线程都可以消费环形数组中的所有数据。但是现在，我希望指定一个规则，就是 3 号线程必须在 1 号和 2 号线程消费了数据之后，才能进行消费。如果 3 号线程想消费环形数组中的 4 号数据，首先得判断一下 1 号和 2 号线程是否消费了 4 号数据，如果 1 号和 2 号线程没有消费，那么 3 号线程就无法消费 4 号数据，如果 1 号和 2 号线程消费了，3 号线程就可以消费 4 号数据了。这就是顺序依赖消费的简单介绍。

根据以上介绍我们可以得到什么信息呢？我为大家总结了四点，大家可以品味一下。

第一：在顺序依赖消费中，1 号和 2 号消费者线程是并行消费的关系，也就是说，这两个消费者线程可以随意消费环形数组中的数据，没有先后关系。3 号消费者线程依赖 1 号和 2 号消费者线程的消费进度，必须在 1 号和 2 号消费者线程消费了之后才能消费。

第二：既然 3 号线程依赖 1 号和 2 号消费者线程的消费进度，也就是说 3 号消费者线程消费的数据肯定都是 1 号和 2 号消费者线程已经消费过的了。也就意味着，3 号消费者线程消费这些数据的时候，不必再去和生产者的进度做对比了，做对比的这个工作已经被 1 号和 2 号线程做完了，3 号线程只管跟在这两个消费者线程身后消费就行。

第三：3 号消费者线程不必关心生产者进度了，也就是意味着 3 号线程在获得了它下一个可以消费的进度序号之后，并不需要再拿着这个进度序号去和生产者进度做对比了。

第四：既然 3 号线程并不需要再拿着自己获得下一个可消费的进度序号去和生产者进度做对比了，为了判断 3 号线程获得的进度序号是否可以被消费，就需要拿着这个进度序号去和它所依赖的 1 号和 2 号线程的消费进度做对比。如果 1 号线程的消费进度为 5，2 号消费者线程的消费进度为 2，当前 3 号消费者线程获得的消费进度为 3，这个进度大于 2 号消费者线程的消费进度，这就意味着 3 号线程此时获得的这个进度序号对应的数据还无法被 3 号消费者线程消费。这就是说，3 号线程要想消费自己获得的进度序号对应的数据，就需获得 1 号和 2 号消费者中最慢的那个消费进度，然后使用自己获得的进度序号和最慢的进度序号做对比，如果大于最慢的进度序号，那么 3 号线程就无法消费对应的数据。

综上所述，**顺序依赖消费的本质其实就是用依赖消费者的消费进度，也就是 3 号消费者线程的消费进度，去和被依赖消费者中最慢的消费进度做对比，也就是 1 号和 2 号消费者中最慢的消费进度，如果前者大于后者，则意味着无法消费，如果前者小于后者，说明后者已经消费了这个进度，前者就可以继续消费了。**

所以，现在我们实现这个功能的突破口就是要让 3 号线程的消费进度和 1 号 2 号消费者线程的消费进度做对比。那该怎么对比呢？或者说在哪里对比呢？理清头绪十分重要，让我们想一想，当没有顺序依赖消费功能的时候，3 号消费者线程获得了下一个可以消费的进度序号之后，是不是就要和生产者进度做对比了？这个对比是在哪里实现的呢？毫无疑问，是在序号屏障 ProcessingSequenceBarrier 中实现的(这些都是以前的知识，这时候，我就不会再对这些知识啰嗦了)。在下面的代码块中，我为大家把实现的部分代码展示出来了。请看下面代码块。

首先是 BatchEventProcessor 类的部分代码。

```
public final class BatchEventProcessor<T> implements EventProcessor {

    //序号屏障，每一个消费者都有一个序号屏障
    private final SequenceBarrier sequenceBarrier;



    //其他内容省略


    @Override
    public void run(){

        //省略部分代码
        processEvents();
        
    }


     private void processEvents(){

         //省略部分代码
        
         //先定义要消费的事件
        T event = null;
        //sequence是当前消费者自身的消费进度，这个get方法得到的就是当前的消费进度
        //因为消费进度是long整数，所以要+1L，这样就能得到下一个应该消费的进度了
        //举个例子，现在环形数组中前5个索引位置都有待消费的事件
        //当前消费者消费到第3个了，那么加一得到的就是第四个
        long nextSequence = sequence.get() + 1L;
        //在一个循环中开始执行真正的消费任务了
        while (true){

            //这里以nextSequence当作参数，从序号屏障中获得最大的可以消费的序号
            //再举个例子，当前的消费者消费到第3个生产者发布的事件了，马上要去消费第四个，所以就把四当作参数
            //让序号屏障去判断生产者发布的事件是不是比4还要多。如果现在生产者已经发布了8个事件
            //那么就返回8，这就意味着环形数组中8和8之前的事件都可以被消费了，当然是被当前的消费者批量消费
            //如果没有发布那么多，或者说只发布到第4个了，那就把4原样返回就行
            //这里大家应该也认识到序号屏障的作用了，就是协调消费者和生产者的关系，保证消费者能消费已发布的事件
            //如果生产者还没有发布事件，消费者肯定不能消费
            //这里就是最关键的地方，用当前消费者得到的下一个可以消费的进度序号去和生产者的进度做对比
            //判断是否可以消费
            final long availableSequence = sequenceBarrier.waitFor(nextSequence);
            
        }

        //省略部分代码
     }
    
}
```

接下来就是序号屏障 ProcessingSequenceBarrier 类的部分代码。

```
final class ProcessingSequenceBarrier implements SequenceBarrier {

    //这是一个至关重要的成员变量，在之前的版本，这个成员变量会被当前的生产者进度赋值
    //这个成员变量的名字翻译过来就是依赖的进度序号
    private final Sequence dependentSequence;

    
    //构造方法，cursorSequence参数就是当前的生产者进度
    ProcessingSequenceBarrier(final Sequencer sequencer,final WaitStrategy waitStrategy,final Sequence cursorSequence,final Sequence[] dependentSequences){
        this.sequencer = sequencer;
        this.waitStrategy = waitStrategy;
        this.cursorSequence = cursorSequence;
        //这里有一个判断，那就是dependentSequences的长度是否为0.在前三版本中默认是0，所以会直接进入下面的分支
        //把生产者的生产进度赋值给dependentSequence
        //因为当前消费者不依赖其他消费者的话，那么消费者消费数据，只用看看生产者的进度是否能够消费就行了
        //所以，这里把依赖的进度直接用生产者的生产进度赋值就行了
        if (0 == dependentSequences.length){
            dependentSequence = cursorSequence;
        }
        
        //构造方法内的其他内容暂时省略
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/23
     * @Description:该类的核心方法，sequence参数就是消费者期望可以消费的最小的序号，也就是可以消费的
     * 最小的生产者进度序号
     */
    @Override
    public long waitFor(final long sequence)throws AlertException, InterruptedException, TimeoutException{   
        //检查是否有消费者终止的信号
        checkAlert();
        
        //使用具体的等待策略来判断消费者可以消费的最大序号，返回的这个序号就是消费可以消费的最大序号
        //这里就是使用当前消费者获得的进度序号去和生产者进度序号做对比
        //sequence参数就是当前消费者获得的下一个要消费的进度序号
        //dependentSequence在该类的构造方法中被生产者的进度序号赋值了
        //所以，这里就是用当前消费者获得的进度序号和生产者的进度序号做对比了
        //实际上就是sequence和dependentSequence在做对比
        long availableSequence = waitStrategy.waitFor(sequence, cursorSequence, dependentSequence, this);
        
        //下面这个判断就是看一下返回的最大序号是否比消费者申请的最小序号还小，算是一个保底操作吧
        //实际上不可能发生，如果真的发生了，那就还是返回availableSequence即可
        if (availableSequence < sequence){
            return availableSequence;
        }
        
        //下面这里就是返回最大的可以消费的生产者序号，所以肯定是返回availableSequence呀
        //当然，这是在单生产者模式下。只有一个生产者，发布的序号一定是连续的
        //但是在多生产者情况下就不一定了，所以在多生产者情况下，必须判断最大序号之前的序号是不是连续的
        //比如说现在可消费的最大的生产者序号是9，那么9之前的可消费的序号必须为12345678，如果是12345 789而没有6
        //那就要在中间截断，返回5
        return sequencer.getHighestPublishedSequence(sequence, availableSequence);
    }

        
    //省略部分代码
    
}
```

在上面代码亏的第 42 行，我们终于发现消费者要消费的进度序号和生产者进度做对比了。这些都是旧知识，代码块中注释也很详细，所以我也就不再详细讲解了。现在我们已经找到了做对比的地方，不过，在顺序依赖消费功能中，我们希望的并不是当前消费者获得进度序号和生产者的进度序号做对比，而是让消费者的进度序号和被依赖的消费者们的最慢进度序号做对比。那该怎么办呢？当然，还是要从上面的代码块中分析，请看上面代码块中的第 42 行，有这样一句注释：实际上就是sequence和dependentSequence在做对比。是的，这个就是我们最中分析出来的一个结果，实际上，消费者持有的进度序号和生产者进度序号做对比，其实就是 sequence 和 dependentSequence 这两个参数代表的值在做对比。sequence 的值我们已经知道了，就是当前消费者想要消费的下一个进度序号，可是 dependentSequence 的值呢？在上面代码块中，它被生产者的进度序号赋值了。但现在我们要实现的顺序依赖消费是需要当前消费者持有的要消费的进度序号和被依赖消费者中最慢的消费进度做对比，所以，让我们大胆假设一下，如果 dependentSequence 被当前消费者依赖的消费者们的最慢消费进度赋值，一切不都解决了吗？而 dependentSequence 正好是 Sequence 类型的，恰好可以被同类型的对象赋值。所以，现在我们要做的就是，让 dependentSequence 被当前消费者依赖的消费者们的最慢消费进度赋值。

那这个赋值该怎么实现呢？这时候，大家的目光就可以回到 ProcessingSequenceBarrier 类的构造方法中了，我把 ProcessingSequenceBarrier 类的构造方法单独列出来了，请看下面代码块。

```
//这是一个至关重要的成员变量，在之前的版本，这个成员变量会被当前的生产者进度赋值
//这个成员变量的名字翻译过来就是依赖的进度序号
private final Sequence dependentSequence;

//构造方法，cursorSequence参数就是当前的生产者进度
ProcessingSequenceBarrier(final Sequencer sequencer,final WaitStrategy waitStrategy,final Sequence cursorSequence,final Sequence[] dependentSequences){
    this.sequencer = sequencer;
    this.waitStrategy = waitStrategy;
    this.cursorSequence = cursorSequence;
    //这里有一个判断，那就是dependentSequences的长度是否为0.在前三版本中默认是0，所以会直接进入下面的分支
    //把生产者的生产进度赋值给dependentSequence
    //因为当前消费者不依赖其他消费者的话，那么消费者消费数据，只用看看生产者的进度是否能够消费就行了
    //所以，这里把依赖的进度直接用生产者的生产进度赋值就行了
    if (0 == dependentSequences.length){
        dependentSequence = cursorSequence;
    }
    
    //构造方法内的其他内容暂时省略
}
```

在上面代码块中，可以看到，在 ProcessingSequenceBarrier 类的构造方法中，会判断 dependentSequences 的长度，如果长度为 0，那就用生产者的当前进度给 dependentSequence 成员变量赋值。那么 dependentSequences 是什么呢？如果大家对前几个版本的代码还有印象，一定会记得，在前几个代码中 dependentSequences 数组的长度一直为 0，其源头应该从 Disruptor 类的 handleEventsWith 方法说起。请看下面代码块。

```
@SafeVarargs
public final EventHandlerGroup<T> handleEventsWith(final EventHandler<? super T>... handlers){
    //注意，下面方法的第一个参数为数组，这里默认使用的是空数组，这个数组的作用会在后面的版本讲到，现在先不解释了
    //这个方法就会把消费事件包装在事件处理器中，一个消费者对应着一个事件处理器，一个事件处理器最终会交给一个线程来执行
    return createEventProcessors(new Sequence[0], handlers);
}
```

在上面代码块的第 5 行，传进去了一个长度为 0 的 Sequence 数组。现在我就可以跟大家正式地揭开这个数组的面纱，这个数组最终就会被传递到 ProcessingSequenceBarrier 类的构造方法中。并且，这个数组表示的就是传递到 handleEventsWith 方法中的 handlers 所依赖的消费者们的消费进度，这些消费进度就被存放到一个 Sequence 数组中，从这个数组中，就可以找到当前消费者所依赖的消费者们的最慢消费进度。但是，现在这个数组的长度为 0，也就意味着当前消费者不依赖任何消费者消费。所以，在 ProcessingSequenceBarrier 类的构造方法中，dependentSequence 成员变量才会被生产者的进度序号赋值。那如果这个数组的长度不为 0 呢？也就意味着当前消费者确实依赖了一些消费者进行消费，那么 ProcessingSequenceBarrier 类的 dependentSequence 成员变量是不是就会被传递过来的、存放着当前消费者依赖的消费者们的消费进度的数组赋值呢？答案已经不言而喻了，肯定会被赋值，但是这时候就有一个问题了，dependentSequence 只是一个 Sequence 对象，而传递过来的是一个 Sequence 数组，这该怎么赋值呢？

答案很简单，就是定义一个新的类，让这个类继承 Sequence 类，并且在这个类中定义一个 Sequence 数组成员变量，这样不就可以成功赋值了吗？简单来说，就是对传递过来的 Sequence 数组包装一下。具体实现请看下面代码块。

首先是定义的新的类，叫做 FixedSequenceGroup。

```
public final class FixedSequenceGroup extends Sequence{

    //该成员变量就要被传递过来的Sequence数组赋值
    private final Sequence[] sequences;

    //构造方法
    public FixedSequenceGroup(Sequence[] sequences){
        this.sequences = Arrays.copyOf(sequences, sequences.length);
    }

    //得到顺序消费中，先消费的消费者们的最慢的消费进度
    @Override
    public long get(){
        //该方法就可以得到传递过来的Sequence数组中，最慢的那个消费进度
        //也就是得到了当前消费者依赖的所有消费者中，消费最慢的那个消费进度
        return Util.getMinimumSequence(sequences);
    }

    //其他内容省略

}
```

接下来就是重构之后的 ProcessingSequenceBarrier 类的构造方法。

```
//这是一个至关重要的成员变量，在之前的版本，这个成员变量会被当前的生产者进度赋值
//这个成员变量的名字翻译过来就是依赖的进度序号
private final Sequence dependentSequence;

//构造方法，cursorSequence参数就是当前的生产者进度
ProcessingSequenceBarrier(final Sequencer sequencer,final WaitStrategy waitStrategy,final Sequence cursorSequence,final Sequence[] dependentSequences){
    this.sequencer = sequencer;
    this.waitStrategy = waitStrategy;
    this.cursorSequence = cursorSequence;
    //这里有一个判断，那就是dependentSequences的长度是否为0.在前三版本中默认是0，所以会直接进入下面的分支
    //把生产者的生产进度赋值给dependentSequence
    //因为当前消费者不依赖其他消费者的话，那么消费者消费数据，只用看看生产者的进度是否能够消费就行了
    //所以，这里把依赖的进度直接用生产者的生产进度赋值就行了
    if (0 == dependentSequences.length){
        dependentSequence = cursorSequence;
    }
    else{  
        //如果传递过来的dependentSequences数组长度不为0，说明当前消费者确实存在消费依赖
        //所以就把dependentSequences数组传递到FixedSequenceGroup对象中包装一下
        dependentSequence = new FixedSequenceGroup(dependentSequences);
    }
}
```

到此为止，只要当前消费者存在消费依赖，我们就可以在当前消费者拥有的序号屏障中得到了当前消费者所依赖的消费者们的消费进度。并且，当调用到 ProcessingSequenceBarrier 类的 waitFor 方法时，dependentSequence 已经被 FixedSequenceGroup 对象赋值了，而通过 FixedSequenceGroup 对象的 get 方法，就可以得到当前消费者依赖的所有消费者中最慢的消费进度。具体流程请看下面代码块。

```
@Override
public long waitFor(final long sequence)throws AlertException, InterruptedException, TimeoutException{   
    //检查是否有消费者终止的信号
    checkAlert();
    
    //使用具体的等待策略来判断消费者可以消费的最大序号，返回的这个序号就是消费可以消费的最大序号
    //这里就是使用当前消费者获得的进度序号去和生产者进度序号做对比
    //sequence参数就是当前消费者获得的下一个要消费的进度序号
    //dependentSequence在该类的构造方法中被生产者的进度序号赋值了
    //所以，这里就是用当前消费者获得的进度序号和生产者的进度序号做对比了
    //实际上就是sequence和dependentSequence在做对比
    long availableSequence = waitStrategy.waitFor(sequence, cursorSequence, dependentSequence, this);
    
    //下面这个判断就是看一下返回的最大序号是否比消费者申请的最小序号还小，算是一个保底操作吧
    //实际上不可能发生，如果真的发生了，那就还是返回availableSequence即可
    if (availableSequence < sequence){
        return availableSequence;
    }
    
    //下面这里就是返回最大的可以消费的生产者序号，所以肯定是返回availableSequence呀
    //当然，这是在单生产者模式下。只有一个生产者，发布的序号一定是连续的
    //但是在多生产者情况下就不一定了，所以在多生产者情况下，必须判断最大序号之前的序号是不是连续的
    //比如说现在可消费的最大的生产者序号是9，那么9之前的可消费的序号必须为12345678，如果是12345 789而没有6
    //那就要在中间截断，返回5
    return sequencer.getHighestPublishedSequence(sequence, availableSequence);
}



//这里是waitFor方法的实现逻辑，这个方法本来在阻塞策略中，这里只是为了讲解方便，搬运到了这里
/**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/23
     * @Description:获取可消费的最大进度额核心方法
     */
    @Override
    public long waitFor(final long sequence, Sequence cursor, final Sequence dependentSequence, final SequenceBarrier barrier)
            throws AlertException{
        long availableSequence;
        //在这里得到默认的自旋次数
        int counter = retries;
        //下面的判断条件中，dependentSequence是消费者要依赖的其他消费者的消费进度，而sequence就是当前消费者
        //申请的想使用的消费序号。在第一版本中，我们并没有考虑消费者的顺序消费情况，所以消费者并不依赖其他任何消费者
        //只需要判断生产者的进度即可，所以在ProcessingSequenceBarrier类中dependentSequence会被生产者的进度赋值，所以
        //这里dependentSequence.get()得到的就是生产者当前的进度序号，并且是最新进度的序号，这个序号会赋值给availableSequence
        //并且判断是否小于sequence，如果大于则说明生产进度超过了消费进度，那就把可用的生产序号直接返回给消费者消费就行
        //如果小于，则说明当前生产者的进度是落后消费者的，比如说消费者想消费的进度是5，但生产者只发布到了4，显然，消费者
        //应该等待一会，等待生产者的进度到5或者超过5，才能继续消费

        //dependentSequence.get()方法，就在这里得到了当前消费者依赖的消费者们的最慢消费进度
        //然后和sequence做对比，sequence就是当前消费者要消费的下一个进度序号
        while ((availableSequence = dependentSequence.get()) < sequence){
            //在这里让线程等待，直到生产者发布了新的数据，其进度更新了，超过了消费者的进度
            counter = applyWaitMethod(barrier, counter);
        }
        //返回可用的最大进度
        return availableSequence;
    }
```

到此为止，我就为大家把顺序依赖的实现逻辑剖析完了。当然，还有最后一个小尾巴，那就是在 Disruptor 的 handleEventsWith 方法中，会继续调用 createEventProcessors 方法，但是这个方法默认传进去的是一个长度为 0 的 Sequence 数组，现在既然我们已经实现了顺序依赖消费的功能，那怎么才能传进去真正存放着当前消费者依赖的所有消费者们的消费进度的 Sequence 数组呢？

```
@SafeVarargs
public final EventHandlerGroup<T> handleEventsWith(final EventHandler<? super T>... handlers){
    //注意，下面方法的第一个参数为数组，这里默认使用的是空数组，这个数组的作用会在后面的版本讲到，现在先不解释了
    //这个方法就会把消费事件包装在事件处理器中，一个消费者对应着一个事件处理器，一个事件处理器最终会交给一个线程来执行
    return createEventProcessors(new Sequence[0], handlers);
}
```

这个功能的实现也没有难度，实际上到这里已经没什么知识可讲解了，剩下的就是一点点源码逻辑，直接看源码即可，没有任何知识点了。所以，接下来我就简单把代码逻辑为大家梳理一下，然后大家就可以自己去看第四版本代码了。请看下面代码块。

在 Disruptor 框架的 Disruptor 提供了一个方法，那就是 handleEventsWith(final EventHandler<? super T>... handlers) 方法，这个方法是我们一直在用的。大家肯定已经很熟悉了。

```
public class Disruptor<T>{


    //其他内容省略

    @SafeVarargs
    public final EventHandlerGroup<T> handleEventsWith(final EventHandler<? super T>... handlers){
        //注意，下面方法的第一个参数为数组，这里默认使用的是空数组，这个数组的作用会在后面的版本讲到，现在先不解释了
        //这个方法就会把消费事件包装在事件处理器中，一个消费者对应着一个事件处理器，一个事件处理器最终会交给一个线程来执行
        return createEventProcessors(new Sequence[0], handlers);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/19
     * @Description:该方法会把用户定义的消费者handler包装到EventProcessor对象中，是这个类中最重要的一个方法
     */
    EventHandlerGroup<T> createEventProcessors(final Sequence[] barrierSequences,final EventHandler<? super T>[] eventHandlers){

        //省略其他内容

        //定义一个Sequence数组，Sequence其实就是消费者的消费进度，也可以说是消费者的消费序号
        //这个数组的长度就是用户定义的eventHandlers的长度
        final Sequence[] processorSequences = new Sequence[eventHandlers.length];

        //。。。。。。
        
        for (int i = 0, eventHandlersLength = eventHandlers.length; i < eventHandlersLength; i++){
            
            final EventHandler<? super T> eventHandler = eventHandlers[i];
            //注意，在创建好batchEventProcessor对象的时候，消费者的序号就已经初始化好了
            final BatchEventProcessor<T> batchEventProcessor = new BatchEventProcessor<>(ringBuffer, barrier, eventHandler);

            //。。。。。。
            
            //这时候每一个消费者的消费序号已经初始化好了，直接可以从batchEventProcessor对象中得到
            //然后就可以放到最上面定义的数组中了
            processorSequences[i] = batchEventProcessor.getSequence();
        }
        
        //该方法最后返回一个EventHandlerGroup对象
        return new EventHandlerGroup<>(this, consumerRepository, processorSequences);
    }
}
```

在上面的代码块中，我们通常是调用第一个方法，传入第一个方法消费处理器默认是没有依赖其他处理器进行消费的，因为会创建一个长度为 0 的 Sequence 数组传入 createEventProcessors 方法中，并且用户定义的这些消费者处理器也会一并传到 createEventProcessors 方法中，然后每一个消费者处理器会被一个 BatchEventProcessor 对象包装，这些都是旧知识了，所以我把大部分代码都省略了。在 createEventProcessors 方法的结尾，会为用户返回一个 EventHandlerGroup 对象，这个 EventHandlerGroup 类的代码在前几个版本我已经为大家展示过了，但是一直没有讲解。现在我们就来看看这个 EventHandlerGroup 是什么。这里我要强调一下，在创建 EventHandlerGroup 对象的时候，已经把 processorSequences 数组传到这个对象的构造方法中了，而 processorSequences 数组中存放的是本次用户定义的所有消费者的消费进度。好了，接下来我们就看看 EventHandlerGroup 类中的内容。请看下面代码块。

```
public class EventHandlerGroup<T>{

    private final Disruptor<T> disruptor;

    private final ConsumerRepository<T> consumerRepository;

    //这个就是该类中最核心的成员变量，就是一组消费者的消费进度
    private final Sequence[] sequences;

    EventHandlerGroup(final Disruptor<T> disruptor,final ConsumerRepository<T> consumerRepository,final Sequence[] sequences){
        this.disruptor = disruptor;
        this.consumerRepository = consumerRepository;
        this.sequences = Arrays.copyOf(sequences, sequences.length);
    }


    //其他内容省略

    
    //这个方法就是用来顺序依赖消费的
    @SafeVarargs
    public final EventHandlerGroup<T> then(final EventHandler<? super T>... handlers){
        return handleEventsWith(handlers);
    }


    
    @SafeVarargs
    public final EventHandlerGroup<T> handleEventsWith(final EventHandler<? super T>... handlers){
        return disruptor.createEventProcessors(sequences, handlers);
    }

}
```

在上面 EventHandlerGroup 类的代码块中，我省略了一些方法，只展示了接下来要用到的一些方法。这时候有的朋友可能对上面这个 EventHandlerGroup 类如何使用还不太清楚，接下来，就让我举一个测试类，给大家具体展示一下 EventHandlerGroup 类在程序中起到了什么作用。请看下面测试类。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/29
 * @Description:不规则消费者测试类，这个类就是第四版本新添加的
 */
public class IrregularConsumerTest {

    public static void main(String[] args) {

        SimpleEventFactory<String> eventFactory = new SimpleEventFactory<>();
        int ringBufferSize = 128;
        ThreadFactory threadFactory = Executors.defaultThreadFactory();
        WaitStrategy waitStrategy = new SleepingWaitStrategy();
        //创建disruptor，这里使用的就是单生产者模式
        Disruptor<Event<String>> disruptor = new Disruptor<>(eventFactory, ringBufferSize, threadFactory, SINGLE, waitStrategy);
        //创建多个消费者
        EventHandler[] eventHandlers = new EventHandler[5];
        for (int i = 0; i < eventHandlers.length; i++) {
            eventHandlers[i] = new SimpleEventHandler<>();
            SimpleEventHandler handler = (SimpleEventHandler)eventHandlers[i];
            handler.index = i;
        }

        //1，首先按照4-1的消费顺序，就是前4个handler并行消费，最后一个在它们之后消费
        //测试这个例子，会发现不管怎么测试，都是第2个处理器最后消费消息
        disruptor.handleEventsWith(eventHandlers[0],eventHandlers[1]).then(eventHandlers[2]);


        ExceptionHandler<Event<String>> exceptionHandler = new SimpleExceptionHandler<>();
        disruptor.setDefaultExceptionHandler(exceptionHandler);
        //这个的启动要在发布生产者消息之前
        disruptor.start();

        EventTranslatorOneArg<Event<String>, String> eventTranslatorOneArg =
                new EventTranslatorOneArg<Event<String>, String>() {
                    @Override
                    public void translateTo(Event<String> event, long sequence, String arg0) {
                        event.setData(arg0);
                    }
                };

        //为了看的清楚，就发布一条数据
        for (int i = 0; i < 1; i++) {
            disruptor.publishEvent(eventTranslatorOneArg, "第"+i+"条");
        }

        disruptor.shutdown();
    }
}
```

在上面测试类的第 27 行，我把定义好的两个消费处理器传递到了 handleEventsWith 方法中，这个 handleEventsWith 方法返回一个 EventHandlerGroup 类的对象，然后我又调用了该对象的 then 方法，把第三个消费处理器传进去了。这行代码的意思就是传到 then 方法中的第三个消费处理器必须要在传进 handleEventsWith 方法中的两个消费处理器消费完了之后，才能消费进行消费。为什么会出现这种情况呢？其实逻辑已经非常清晰了，首先传递到 handleEventsWith 方法中的两个消费处理器，它们的消费进度会放到一个数组中，然后这个数组会传递到 EventHandlerGroup 对象中，赋值给 EventHandlerGroup 对象的 sequences 成员变量，这时候 EventHandlerGroup 对象就拥有了前两个消费处理器的消费进度。而之后调用的 EventHandlerGroup 对象的 then 方法，将第三个消费处理器传递进去了，然后就会在 then 方法中执行到 EventHandlerGroup 对象的 handleEventsWith 方法，也就是下面代码块中的方法。

```
@SafeVarargs
public final EventHandlerGroup<T> handleEventsWith(final EventHandler<? super T>... handlers){
    //这里的sequences参数就是EventHandlerGroup对象刚刚获得的前两个消费者的消费进度
    return disruptor.createEventProcessors(sequences, handlers);
}
```

在该方法中，又会进一步调用到 Disruptor 类的 createEventProcessors 方法。这时候，传进去第一个参数就不再是长度为 0 的 Sequence[] 了，而是封装了前两个消费者消费进度的 Sequence 数组。这样一来，我们不就终于解决了之前的问题吗？第三个消费者也就知道自己依靠哪两个消费者进行消费了。到此为止，顺序依赖消费的功能就全部讲解完毕了。

到此为止，整个 Disruptor 框架的核心知识我就全为大家讲解完毕了，应该可以这么说，不管之前学过还是没学过这个 Disruptor 框架，阅读了我的文章后，基本上已经可以复刻出一个 Disruptor 框架了，更别说使用了。我相信我的文章应该达到了这种水平，同时，我也相信我的文章还是保持了一如既往的风格，那就是循序渐进的讲解，每一次重构，都剖析清楚为什么要重构，为什么要这样重构。从开始做这个系列教程以来，已经有很多朋友在微信私聊我，跟我交流学习情况，希望我可以坚持把这个系列做下去。因为我的文章风格和讲解知识的方式，让一些朋友真的觉得学习知识是很轻松的事，哪怕是以前觉得比较难的知识，读了我的文章之后，觉得非常简单。我这里没有自夸的意思，当然，得到大家的赞赏和夸奖真的让我受宠若惊，但是我最想说的是，任何知识其实都经不起拆分，不管这个知识多么复杂，拆分到最后，都是一个个简单的小知识。大家不要对一些知识感到畏惧。我面对未知的知识时，第一反应是兴奋，想立刻挑战这些知识。我想学习这个系列的朋友们肯定也都跟我一样，只不过希望挑战这些知识是一回事，但手边确实没有关于这些知识的优秀的详细的学习资料，我所做的就是为大家提供这些资料而已，说到底学习是个人的事，最后大家学会了这些知识，是通过自己的努力学会的，并不是我教会的，我只是提供了一点点学习资料，一点微小的工作，不足挂齿。

好了，这门课程的完结，标志着下一门课程就要开始更新了，实际上，第四门课程已经开始更新了。虽然更新的比较慢。但这门课程结束了，意味着我拥有了更多的时间，很快就可以为大家同步更新第四和第五门课了。第五门课将包含非常多的内容，我想，是时候帮助大家把分布式的难题一网打尽了。好了朋友们，我们下一门课再见。

附：这里解释一下，因为 Disruptor 框架的第五版本代码只是增加了几个阻塞策略，代码非常简单，所以就不单独写文章讲解了。我在第五版本的代码中都加了注释，大家可以直接从注释中学习。