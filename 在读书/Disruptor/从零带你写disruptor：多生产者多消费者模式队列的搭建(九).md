朋友们，大家好这一章我将为大家继续剖析 Disruptor 框架的内容。上一章我为大家讲解了单生产者并发消费模式，把消费者线程消费的几种模式也都讲解完毕了。这一章就让我来为大家剖析一下，生产者的另一种模式，之前八章讲解的队列都是在单生产者模式下实现的，这一章，我们来看看在多生产者情况下，怎么实现一个本地队列。

**解决多生产模式中的并发问题**

如果大家对上一章的知识还没忘记，一定还记得所谓的多消费者模式究竟是什么，所谓多消费者模式就是并发消费模式，也就是多个消费者线程共同消费一批数据。简单类比一下的话，这一章要为大家讲解的多生产者模式其实就是多个线程共同生产数据。生产数据的流程我想大家应该都非常清楚了吧，首先是单生产者序号生成器给生产者分配一个可以存放数据的进度序号，然后生产者将生产的数据存放到对应的进度序号中，其实也就是进度序号对应的环形数组的索引下标出。如果对这个流程非常清晰，那么肯定也可以想到，假如是多个线程共同生产数据，就意味着单生产者序号生成器分配出来的这个序号，可能同时会交给好几个生产者使用，多个生产者同时访问序号生成器，很容易就会发生并发问题。

如果大家还不理解并发问题是怎么产生的，就让我来为大家简单分析一下。比如说，现在有两个生产者都要去存放生产出来的数据，按照生产者存放数据的流程，肯定是要先执行下面这段代码。请看下面代码块，

```
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
```

在上面代码块的第 13 行，开始发布生产者数据。解下来就会来到 Disruptor 类中。请看下面代码块。

```
/发布生产者，其实就是发布数据让消费者去消费
//这里大家也要弄清楚EventTranslatorOneArg的作用，环形数组初始化好的那一刻，其实环形数组中每一个
//位置的事件对象都创建好了，只要程序不退出，这些对象是不会被垃圾回收的，而EventTranslatorOneArg
//对象就是把发布者发布的真正消息传递到环形数组的每一个事件对象中去
public <A> void publishEvent(final EventTranslatorOneArg<T, A> eventTranslator, final A arg){
    ringBuffer.publishEvent(eventTranslator, arg);
}
```

很明显，程序又要来到 RingBuffer 类中，请看下面代码块。

```
//发布生产者数据的核心方法，在第一版本代码中使用的方法
@Override
public <A> void publishEvent(EventTranslatorOneArg<E, A> translator, A arg0){
    //首先获得可用的生产者序号，生产者发布的信息就可以存放到该位置
    final long sequence = sequencer.next();
    //然后在这个序号对应的位置发布数据
    translateAndPublish(translator, sequence, arg0);
}
```

在上面代码块的第 5 行，生产者就会通过序号生成器获得下一个可以使用的进度序号，然后把生产者数据放到进度序号对应的环形数组下标中。这就是生产者生产数据的流程。如果只有一个生产者通过序号生成器获得了下一个可以使用的进度序号，就不会出现什么并发问题。但假如现在有两个生产者线程同时来访问序号生成器呢？两个生产者就意味着有两个线程，大家可以结合着 sequencer.next() 方法的执行流程来思考一下这个问题。我已经为大家把 sequencer.next() 方法逻辑展示在下面的代码块中了。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/24
 * @Description:申请指定数量的序号，交给生产者使用
 */
@Override
public long next(int n){   //判断是否小于1
    if (n < 1){   
    	//抛出异常
        throw new IllegalArgumentException("n must be > 0");
    }
    //先得到当前的序号，这个序号就是序号生成器已经分配好了的序号
    //其实也就可以代表生产者的最新进度
    long nextValue = this.nextValue;
    //加上要申请的序号的个数，得到最终要申请到的序号的值
    long nextSequence = nextValue + n;
    //这里的逻辑其实和上面hasAvailableCapacity分析的逻辑一样
    //都是为了判断是否会覆盖尚未被消费的数据
    long wrapPoint = nextSequence - bufferSize;
    //得到消费者中最慢的那个消费进度
    long cachedGatingSequence = this.cachedValue;
    //和之前hasAvailableCapacity分析的逻辑一摸一样，就不再重复注释了
    if (wrapPoint > cachedGatingSequence || cachedGatingSequence > nextValue){   
        //这里有点不一样，如果发现快要覆盖未消费的数据了，就立刻更新一下生产者当前的进度
        //并且保证内存可见
        cursor.setVolatile(nextValue);
        //定义一个记录最慢消费进度的局部变量
        long minSequence;
        //这里就和hasAvailableCapacity方法的逻辑不同了，注意，这里是要真正的分配序号了，如果分配不了，就要让线程等待，直到能够分配为止
        //minSequence = Util.getMinimumSequence(gatingSequences, nextValue)得到的是最新的最慢消费者的进度
        //所以下面会判断，wrapPoint是不是一直大于最新的最慢消费者进度，如果大于，就不能分配，只有小于的时候，才能分配
        while (wrapPoint > (minSequence = Util.getMinimumSequence(gatingSequences, nextValue))){   
            //源码在这里其实有一个TODO，打算用等待策略来扩展这里的LockSupport
            //但是一直没有实现，该类最后一次更新已经是去年了，这个扩展也没实现
            LockSupport.parkNanos(1L);
        }
        //把最新的最慢消费者进度的缓存更新一下
        this.cachedValue = minSequence;
    }
    //走到这里，说明当前的生产者线程已经不阻塞了，因为在上面循环中的等待，已经让消费者把该消费的数据都消费了
    //所以，这里就可以直接把最新的要分配的序号赋值给nextValue成员变量了，意味着这个最新的值可以分配给生产者使用了
    this.nextValue = nextSequence;
    //返回最新的分配出来的序号，交给生产者使用
    return nextSequence;
}
```

在上面的代码块中，假设传进来的参数 n 的值为 1，如果有两个生产者都来申请可以使用的进度序号，并且环形数组中还没有存放任何一个数据，那么在上面代码块的第 15 行，这两个生产者都会得到 this.nextValue 的初始值，也就是 -1。执行到 16 行代码的时候，这两个生产者都会得到下一个可用的进度序号 0。因为这时候环形数组中还没有数据呢，所以肯定不会发生进度覆盖的情况，直接把这个进度序号 0 返回给生产者即可。这样一来，两个生产者就同时得到了进度序号 0 来存放生产的数据。两个生产者数据放到同一个位置，其中一个数据必定会被另一个数据覆盖，这不就是典型的并发问题吗？现在，我们要解决的就是这个问题。

要解决这个问题其实也很简单，按照常规思路，只需要给每一个生产者分配一个唯一的可用进度就行了。现在的情况是有多个线程都会来向生产者序号申请可用的生产进度，也就是多个生产者线程同时竞争一个可用的生产者序号，如果我们可以在生产者序号的 next 方法内部做一些操作，可以保证每次只会有一个生产者线程成功竞争到本次分配的生产者序号，而其他竞争失败的生产者线程只需要继续下一次竞争即可。兜兜转转，大家应该也意识到了，这不就又回到了上一章解决并发消费时使用的方法了吗？在本章这种解决方式可以延续下来，也就是说，我们可以继续使用 CAS 方式，让多个生产者去竞争同一个可用的生产者进度，竞争成功者则获得可用的生产者进度序号，竞争失败的则进入下一次循环，直到竞争成功才能退出循环。当然，既然是多生产模式，所以我们也可以定义一个新的生产者序号生成器，比如说就叫做 MultiProducerSequencer，翻译过来就是多生产者序号生成器的意思，这个序号生成器专门在多生产者模式下使用。当然，该类也继承了 AbstractSequencer 类。具体的细节大家可以从第三版本代码中学习，这里我只展示该类的部分代码，也就是使用 CAS 重构之后的 next 方法。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/28
 * @Description:第三版本代码中新添加进来的核心类，就是多生产者模式下的序号生成器
 */
public final class MultiProducerSequencer extends AbstractSequencer{

    //其他内容省略


    //还记得SingleProducerSequencerPad类中的cachedValue成员变量吗？
    //该变量是用来缓存所有消费者中，消费最慢的那个进度，但在多生产模式下，这个缓存最慢进度
    //的成员变量变成了Sequence类型的对象，Sequence中有一个value，为什么不直接使用cachedValue呢？
    //说实话，我也没考虑出一个很合理的结果，如果说是因为这个值在多生产者模式下会被并发访问
    //但使用了Sequence对象，也不会解决什么并发问题，最多时更新该值的时候，可以选择是Volatile更新
    //还是延迟更新，以提高一点性能，除此之外，就是这个Sequence也解决伪共享了
    private final Sequence gatingSequenceCache = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/24
     * @Description:申请指定数量的序号，交给生产者使用
     */
    @Override
    public long next(int n){   
        //判断一下，申请的序号小于1抛出异常
        if (n < 1){
            throw new IllegalArgumentException("n must be > 0");
        }
        //当前已经分配到的生产者序号
        long current;
        //接下来要分配到的生产者序号
        long next;
        do{   
            //得到当前分配到的生产者序号
            current = cursor.get();
            //加上用户要申请的序号个数，得到了要分配到的序号，比如得到的是10
            //其实就是意味着从cursor到10之间的序号，都可以被消费了
            next = current + n;
            //和hasAvailableCapacity方法中的逻辑一样，判断是不是会覆盖未消费的数据
            long wrapPoint = next - bufferSize;
            //得到当前线程看见的最慢的消费者进度
            long cachedGatingSequence = gatingSequenceCache.get();
            //这里的逻辑在hasAvailableCapacity方法中分析过了，就不再重复注释了
            //相比于单生产者序号生成器，cachedGatingSequence > current这行代码在多生产者序号生成器中
            //的作用非常大，但具体逻辑我写在hasAvailableCapacity方法中了，并且举了一个小例子
            //我之前在群里跟大家说重新上传了一份代码，就是添加这部分的注释了，大家可以下载最新版本的代码
            //看看hasAvailableCapacity方法中这行代码的注释，我就不再文章中讲解了
            if (wrapPoint > cachedGatingSequence || cachedGatingSequence > current){
                //走到这里意味着会发生覆盖数据的情况，所以要去获得最新的最慢消费者进度，这个进度
                //很可能是比当前缓存的进度大的
                long gatingSequence = Util.getMinimumSequence(gatingSequences, current);
                //如果获得了最新的消费者进度还是会发生覆盖数据的情况
                if (wrapPoint > gatingSequence){   
                    //在这里等待
                    LockSupport.parkNanos(1);
                    //然后进入下一次循环，判断是否不会发生覆盖进度的情况了
                    continue;
                }
                //走到这里就意味着wrapPoint < gatingSequence，把最新的消费者进度更新一下
                //其实这里就可以去获得生产者序号了，就是用CAS去修改cursor的值，但是作者没有这样处理
                gatingSequenceCache.set(gatingSequence);
            }
            //走到这里就意味着不会发生覆盖数据的情况，那么直接去获得序号即可
            //当然，多线程情况下肯定是要用CAS来修改的，这时候cursor的值也就更新了
            //这一点很重要，一定要记住，下面就要用到
            else if (cursor.compareAndSet(current, next)){
                //修改数据成功，就可以直接推出循环了
                break;
            }
        } while (true);
        //返回分配到的序号，这样就解决了多线程生产数据的并发问题。
        //其实本质仍然是给每个生产者线程分配一个进度序号，生产者线程直接把这个数据放到对应的进度序号中就行了
        return next;
    }

    
}
```

上面代码块中 next 方法的逻辑和 SingleProducerSequencer 类中 next 方法逻辑，基本上没什么差别，唯一的不同就是在 MultiProducerSequencer 类中 next 方法中，使用了 CAS 方式为每一个生产者分配可用的生产者序号。哪个生产者线程 CAS 赋值成功，哪个生产者线程就退出 while 循环。这样就完美解决了并发问题。在这个 MultiProducerSequencer 类中，还有一个很重要的方法，那就是 hasAvailableCapacity 方法，而在 SingleProducerSequencer 类中也有同名方法。这个 hasAvailableCapacity 方法的详细讲解我放在代码中了，基本上和 SingleProducerSequencer 类中的同名方法没什么不同，唯一值得注意的就是 cachedGatingSequence > current 这行代码有了更重要的作用。大家可以下载最新版本的代码，查看其中的注释讲解。我就不放在文章中讲解了，因为大部分都是重复的知识，没必要占用篇幅。

好了，到此为止，我就把多生产者模式下的序号生成器定义完成了。其实这一章的内容到此也就可以结束了，我在前面就说过，学习这个框架的门槛只在前七章，前七章十分全面地剖析的整个框架的架构和运行流程，如果前七章的内容全部掌握了，剩下的知识完全可以自学。但是，实际上，这一章的内容还不能就此结束。

**剖析多生产者引发的数据不连续问题**

接下来，我希望大家能跟随我的思路，再次把注意力集中在生产者生产数据的流程上。请大家想一想，我们刚才是分析到什么地方，才开始着手解决多生产者模式下的并发问题的？显然，是分析到序号生成器的 next 方法的时候。具体流程如下。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/24
 * @Description:发布生产者数据的核心方法
 */
@Override
public <A> void publishEvent(EventTranslatorOneArg<E, A> translator, A arg0){
    //首先获得可用的生产者序号，生产者发布的信息就可以存放到该位置
    final long sequence = sequencer.next();
    //然后在这个序号对应的位置发布数据
    translateAndPublish(translator, sequence, arg0);
}
```

在上面代码块的第 10 行，可能会发生并发问题，为了解决这个并发问题，我为自己的程序引入了 MultiProducerSequencer 这个类，并且使用 CAS 的方式完美解决了可能出现的并发问题。但是，在此我想问问大家，当程序执行到上面代码块的第 10 行时，仅仅是得到了一个可以使用的生产者序号，可以把生产者数据放到这个序号对应的环形数组的下标索引处。但是，这时候并没有真正把数据存放到环形数组中呢，数组真正存放到环形数组中，是在第 12 行代码中进行的，也就是 translateAndPublish 方法。该方法的具体逻辑如下。请看下面代码块。

```
private <A> void translateAndPublish(EventTranslatorOneArg<E, A> translator, long sequence, A arg0){
    try{
        //get(sequence)方法会得到环形数组中sequence位置的对象
        //然后把生产的数据放到该对象中，这时候，才算是把生产的数据真正放到了环形数组中
        translator.translateTo(get(sequence), sequence, arg0);
    }
    finally{
        //当sequence序号对应的生产者数据真正被放到了环形数组中了
        //才更新真正的生产者进度，消费线程就可以根据这个进度来获得可以消费的数据了
        sequencer.publish(sequence);
    }
}
```

如果大家理解了上面代码块的逻辑，那么接下来请想一想，虽说在上面代码块中的 finally 块中，会在真正把生产者数据存到了环形数组中，才更新生产者进度，但是，在 MultiProducerSequencer 类的 next 方法中，有这么一行代码，并且是至关重要的一行代码：cursor.compareAndSet(current, next)，我在上面的注释中还特意提醒大家这行代码非常重要。可以看到，在 next 方法中，cursor，也就是生产者进度序号已经被更新了，如果这时候有消费者线程想要消费数据，肯定就会根据这个 cursor 来判断，比如说发现 cursor 的进度被更新到 3 了，于是立刻去环形数组中消费，结果发现环形数组 3 的位置并没有存放数据。因为存放数据的操作是在 next 方法之后才进行的。但现在的情况是在还没有真正存放数据之前就把生产者进度更新了，这就是一个很大的隐患呀。那这个隐患该怎么解决呢？

第一个解决方法就是不在 MultiProducerSequencer 类的 next 方法中更新 cursor 的值，但显然不能这么做，因为这个是 CAS 保证并发安全的必须步骤。第二个解决方法就比较有意思了，那就是在 MultiProducerSequencer 类中定义一个新的成员变量，这个成员变量是一个 int 数组，数组的容量也等于真正的环形数组的容量。为什么要这么做呢？这就要结合上面代码块的 finally 块来理解了。在上面的代码块中，可以看到在 finally 块中，当生产者把生产的数据真正放到环形数组后，又通过 publish 方法更新了生产者进度。但是这次更新的并不是 cursor 的进度了，而是新定义的 int 数组中的进度。这个要更新到 int 数组中的进度，说白了就是当前生产者得到的生产者序号在环形数组中的圈数。这么说可能有些苍白，接下来我给大家举一个具体的例子。

在举例之前，有一点我要先提醒大家，并且希望大家能时时记住，那就是 finally 块中 sequencer.publish(sequence) 方法的 sequence 参数，其实就是序号生成器分配出来的序号，这个序号是一直递增的，如果其大小超过了环形数组的容量，说明持有该生产序号的生产者可能是第二轮或者是第三轮向环形数组中存放的生产数据了。好了，如果理解了这个，接下来请大家想想一个场景，假如程序中有一个环形数组，环形数组的容量是 8，有一个生产者线程(让我们暂时忘掉并发生产的情况)。这个生产者线程第一次得到生产序号，得到的生产序号肯定是 0，因为程序中只有它一个生产者，当这个生产者将生产的数据放到了环形数组之后，就要执行 finally 块中的代码，把序号 0 这个进度更新到 MultiProducerSequencer 类新定义的 int 数组中。因为现在生产者得到的是序号 0，这意味着现在生产的数据肯定是第一次放在环形数组中，也就意味着环形数组的第一圈还没有被填满呢，所以这个进度序号 0 要是放在环形数组中，它在环形数组中对应的值就是 1，代表着它是环形数组第一圈的一个数据，如果生产者接下来得到的生产者进度序号是 1，那么这个 1 在 int 数组中的值也是 1，因为它也是环形数组中第一圈的数据。当生产者得到的进度序号为 8 的时候，考虑到环形数组的容量为 8，这就意味着进度序号 8 对应的数据要重新放到环形数组的头部了，也就意味着是第二圈的数据，那么这个 8 在 int 数组中对应的值就是 2。大家可以仔细品味品味这个逻辑。

那为什么要实现这样一种逻辑呢？原因很简单，因为 cursor 会在生产者数据放到环形数组之前就被更新了，而消费者线程也会直接根据 cursor 来获得下一个可以使用的消费进度序号。重点来了，当消费者获得了这个可以消费的进度序号之后，还要拿着这个序号到 MultiProducerSequencer 类中的 int 数组里判断一下，先计算出消费者线程持有的进度序号是环形数组第几圈的数据，得到了这个圈数，然后根据这个消费者线程持有的消费进度计算出这个消费进度在 int 数组中对应的值，最后判断一下这个圈数和 int 数组中的值是否相等，如果相等，就意味着生产者数据已经被放到环形数组中了，所以，消费者线程就可以放心大胆地去环形数组中消费这个数据了。如果 int 数组中根本没有生产者线程持有的进度序号对应的值，就说明当前进度序号对应的数据还没有放到环形数组中呢，消费者也就不能去消费。当然，这个 int 数组其实也是有初始值了，这个 int 数组每一位的初始值都是 -1，代表着不可消费。

如果这个逻辑大家都理解了，接下来，我们就从代码层面上看一下，这些功能应该怎么实现。请看下面代码块。主要是在 MultiProducerSequencer 类中进行重构。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/28
 * @Description:第三版本代码中新添加进来的核心类，就是多生产者模式下的序号生成器
 */
public final class MultiProducerSequencer extends AbstractSequencer{

    //得到Unsafe对象
    private static final Unsafe UNSAFE = Util.getUnsafe();
    
    //得到数组中首个有效元素在数组中的偏移量
    private static final long BASE = UNSAFE.arrayBaseOffset(int[].class);
    
    //得到数组中每一个位置的内存大小，其实就是一个内存增量
    //现在显然操纵的是int数组，每一个int站4个字节，那么数组的每一个位置4个字节
    //这个成员变量和上面的成员变量配合使用，可以快速定位数组中的一个元素
    private static final long SCALE = UNSAFE.arrayIndexScale(int[].class);
    
    //还记得SingleProducerSequencerPad类中的cachedValue成员变量吗？
    //该变量是用来缓存所有消费者中，消费最慢的那个进度，但在多生产模式下，这个缓存最慢进度
    //的成员变量变成了Sequence，Sequence中有一个value，为什么不直接使用cachedValue呢？
    //说实话，我也没考虑出一个很合理的结果，如果说是因为这个值在多生产者模式下会被并发访问
    //但使用了Sequence对象，也不会解决什么并发问题，最多时更新该值的时候，可以选择是Volatile更新
    //还是延迟更新，以提高一点性能，除此之外，就是解决伪共享了
    private final Sequence gatingSequenceCache = new Sequence(Sequencer.INITIAL_CURSOR_VALUE);
    
    //真正用来表明生产者进度的数组，消费者消费生产者数据的时候，真正参考的是这个数组，这个数组中的数据如果是连续的
    //消费者才能消费。比如说，消费者得到的可消费的序号是8，当前的消费进度是3，那就意味着3到8之间
    //的数据都可以消费，但是在这个数组中，3到8之间并不连续，第6个位置没有被赋值，那么消费能消费的进度只能到6
    private final int[] availableBuffer;
    
    //用于计算上面那个数组下标的掩码
    private final int indexMask;
    
    //计算availableBuffer中可用标志圈数的辅助属性
    private final int indexShift;

    //构造方法
    public MultiProducerSequencer(int bufferSize, final WaitStrategy waitStrategy){
        super(bufferSize, waitStrategy);
        //创建availableBuffer数组
        availableBuffer = new int[bufferSize];
        //得到掩码的值
        indexMask = bufferSize - 1;
        //举个例子，如果环形数组的长度是8，那这个值就是3
        indexShift = Util.log2(bufferSize);
        //在构造方法中，把数组中的所有位置都设置为不可消费
        initialiseAvailableBuffer();
    }
    
    //其他内容省略



    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/24
     * @Description:申请指定数量的序号，交给生产者使用
     */
    @Override
    public long next(int n){   
        //判断一下，申请的序号小于1抛出异常
        if (n < 1){
            throw new IllegalArgumentException("n must be > 0");
        }
        //当前已经分配到的生产者序号
        long current;
        //接下来要分配到的生产者序号
        long next;
        do{   
            //得到当前分配到的生产者序号
            current = cursor.get();
            //加上用户要申请的序号个数，得到了要分配到的序号，比如得到的是10
            //其实就是意味着从cursor到10之间的序号，都可以被消费了
            next = current + n;
            //和hasAvailableCapacity方法中的逻辑一样，判断是不是会覆盖未消费的数据
            long wrapPoint = next - bufferSize;
            //得到当前线程看见的最慢的消费者进度
            long cachedGatingSequence = gatingSequenceCache.get();
            //这里的逻辑在hasAvailableCapacity方法中分析过了，就不再重复注释了
            //相比于单生产者序号生成器，cachedGatingSequence > current这行代码在多生产者序号生成器中
            //的作用非常大，但具体逻辑我写在hasAvailableCapacity方法中了，并且举了一个小例子
            //我之前在群里跟大家说重新上传了一份代码，就是添加这部分的注释了，大家可以下载最新版本的代码
            //看看hasAvailableCapacity方法中这行代码的注释，我就不再文章中讲解了
            if (wrapPoint > cachedGatingSequence || cachedGatingSequence > current){
                //走到这里意味着会发生覆盖数据的情况，所以要去获得最新的最慢消费者进度，这个进度
                //很可能是比当前缓存的进度大的
                long gatingSequence = Util.getMinimumSequence(gatingSequences, current);
                //如果获得了最新的消费者进度还是会发生覆盖数据的情况
                if (wrapPoint > gatingSequence){   
                    //在这里等待
                    LockSupport.parkNanos(1);
                    //然后进入下一次循环，判断是否不会发生覆盖进度的情况了
                    continue;
                }
                //走到这里就意味着wrapPoint < gatingSequence，把最新的消费者进度更新一下
                //其实这里就可以去获得生产者序号了，就是用CAS去修改cursor的值，但是作者没有这样处理
                gatingSequenceCache.set(gatingSequence);
            }
            //走到这里就意味着不会发生覆盖数据的情况，那么直接去获得序号即可
            //当然，多线程情况下肯定是要用CAS来修改的，这时候cursor的值也就更新了
            //这一点很重要，一定要记住，下面就要用到
            else if (cursor.compareAndSet(current, next)){
                //修改数据成功，就可以直接推出循环了
                break;
            }
        } while (true);
        //返回分配到的序号，这样就解决了多线程生产数据的并发问题。
        //其实本质仍然是给每个生产者线程分配一个进度序号，生产者线程直接把这个数据放到对应的进度序号中就行了
        return next;
    }

    //把availableBuffer数组中的所有位置都设置为不可消费，其实就是把每一个值设置成-1
    private void initialiseAvailableBuffer(){
        for (int i = availableBuffer.length - 1; i != 0; i--){
            setAvailableBufferValue(i, -1);
        }//把数组0号位置设置成-1
        setAvailableBufferValue(0, -1);
    }


    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/24
     * @Description:发布可以被消费的生产者序号的方法，然后通知其他消费者来消费
     * 注意，在多生产者模式中，该方法也是被ringBuffer调用的，最终会在ringBuffer中调用到下面这个方法
     *
     * private <A> void translateAndPublish(EventTranslatorOneArg<E, A> translator, long sequence, A arg0)
     *     {
     *         try
     *         {
     *             translator.translateTo(get(sequence), sequence, arg0);
     *         }
     *         finally
     *         {    下面这个方法在这里被调用了
     *             sequencer.publish(sequence);
     *         }
     *     }
     *     在单线程生产者中，在publish方法中只是更新了生产者的序号，也就是 cursor.set(sequence) 方法
     *     但是多线程生产者环境中，多个线程执行 cursor.set(sequence) 方法是会出现并发问题的，在该类的上面方法中
     *     都是使用CAS来该表 cursor的值的，所以才不会出现并发问题。但这里如果仍然是更新 cursor的值，就会出现并发问题了
     *     所以该框架的作者就用当前线程分配到的生产者序号，去更新availableBuffer数组中的值，这样就不会出现并发问题了
     *     可能有朋友会有疑问，明明在该类的next(n)方法中已经更新了cursor的值，为什么还要在这里更新
     *     我请大家注意一下，仅仅是更新了这个生产者序号是没用的，是需要真正把生产者数据填充到环形数组的对应位置的
     *     就是上面这行代码translator.translateTo(get(sequence), sequence, arg0)，但现在的情况是
     *     多个线程竞争生产者序号，有可能1号线程竞争到了5这个生产者序号，2号线程竞争到了9这个序号，2号线程已经把生产者
     *     数据写入到环形数组的对应位置了，但是1号线程还没写入，这样虽然生产者序号虽然更新到9了，但是5这个序号的数据并未填充
     *     如果这时候消费者直接得到了最新的可消费进度，也就是生产者的序号9，可是中间有数据是空的，显然就会出错
     *     所以要弄一个availableBuffer数组，用来把可以消费的位置连续起来
     */
    @Override
    public void publish(final long sequence){   
        //这个方法就是把能消费的进度设置到availableBuffer数组中
        //一旦设置成功，就意味着真正的生产者数据被填充到环形数组的对应位置了
        setAvailable(sequence);
        //通知消费者来消费
        waitStrategy.signalAllWhenBlocking();
    }


     //这个方法就是把能消费的进度设置到availableBuffer数组中
    private void setAvailable(final long sequence){   
        //这里得到的就是生产者序号在availableBuffer数组中的下标索引以及对应的圈数
        setAvailableBufferValue(calculateIndex(sequence), calculateAvailabilityFlag(sequence));
    }

    //真正把能消费的进度设置到availableBuffer数组中的方法
    private void setAvailableBufferValue(int index, int flag){   
        //快速定位到指定下标在数组中的位置
        long bufferAddress = (index * SCALE) + BASE;
        //把对应的圈数写到对应的位置
        UNSAFE.putOrderedInt(availableBuffer, bufferAddress, flag);
    }

    //判断该生产序号究竟可不可用
    @Override
    public boolean isAvailable(long sequence){   
        //根据生产序号找到该序号在availableBuffer数组中的位置
        int index = calculateIndex(sequence);
        //计算该序号对应的数组的圈数
        int flag = calculateAvailabilityFlag(sequence);
        //根据index得到该位置在数组中的偏移量
        long bufferAddress = (index * SCALE) + BASE;
        //从数组中对应位置取出数据，判断是否和生产序号计算的相等，如果相等就可用
        return UNSAFE.getIntVolatile(availableBuffer, bufferAddress) == flag;
    }

    /**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/29
     * @Description:这个方法很重要啊，这个就是在多生产者模式下，判断序号是否连续可用的方法
     */
    @Override
    public long getHighestPublishedSequence(long lowerBound, long availableSequence){
        for (long sequence = lowerBound; sequence <= availableSequence; sequence++){
            if (!isAvailable(sequence)){   
                //走到这里意味着这个位置的序号还没有填充数据，那该序号就不可用
                return sequence - 1;
            }
        }
        return availableSequence;
    }


    //计算可用标志在availableBuffer数组中是第几圈的方法
    private int calculateAvailabilityFlag(final long sequence){   
        
        //如果这里环形数组的长度是8，indexShift就是3，如果生产者序号是6，右移3，结果是0
        //其实换成二进制就一目了然了8的二进制是 前面省略...... 0000 1000，右移3位正好是1
        //如果生产者序号是9，说明是第一圈
        //如果生产者序号是17，右移3位，得到的就是2
        //17的二进制为 前面省略...... 0001 0001
        return (int) (sequence >>> indexShift);
    }

    //计算分配到的生产者序号在availableBuffer数组中的下标位置
    //这个availableBuffer数组和环形数组的长度是一样的
    private int calculateIndex(final long sequence){
        
        return ((int) sequence) & indexMask;
    }
}
```

接着就是真正发布数据的流程。

```
private <A> void translateAndPublish(EventTranslatorOneArg<E, A> translator, long sequence, A arg0){
    try{
        //get(sequence)方法会得到环形数组中sequence位置的对象
        //然后把生产的数据放到该对象中，这时候，才算是把生产的数据真正放到了环形数组中
        translator.translateTo(get(sequence), sequence, arg0);
    }
    finally{
        //当sequence序号对应的生产者数据真正被放到了环形数组中了
        //才更新真正的生产者进度，消费线程就可以根据这个进度来获得可以消费的数据了
        sequencer.publish(sequence);
    }
}
```

大家可以把上面代码块第 10 行代码当作学习重构之后的 MultiProducerSequencer 类的入口，具体逻辑我就不再讲解了，毕竟在上面代码块中添加了非常详细的注释，应该把逻辑都剖析清楚了。

如果这个问题解决了，接下来就让我们看看最后一个问题，其实我在 MultiProducerSequencer 类的 publish 方法的注释中也提到这个问题了，那就是生产者数据不连续的问题。现在是多生产者模式，如果程序中有 5 个生产者，这 5 个生产者通过 CAS 分别得到了 0、1、2、3、4 这几个生产者序号。但是得到了生产者序号是一回事，真正把生产者更新到环形数组又是另一回事，如果得到了进度序号 4 的生产者最先把自己的进度更新到了 MultiProducerSequencer 类的 int 数组中，也就意味着环形数组中进度序号 4 对应的索引位置的数据可以被消费了。但其他几个生产者还没有跟新，这就意味着环形数组中 0、1、2、3 这几个位置还没有存放生产者数据，是无法被消费的。可是在我们的框架中有一个消费者批处理器，那就是 BatchEventProcessor 类。这个类的对象会一次消费多个进度序号对应的数据。如果这个批处理器发现环形数组中进度 4 的数据是可以被消费的，那么这个批处理器就会一下子把进度 4 之前的数据全部消费了。但是很明显，现在 0、1、2、3 这几个位置没有数据呀。所以，在多生产者模式下，还要判断一些环形数组中的数据是否为连续的。如果有一组数据是 0、1、2、4、5、6，当批处理器在消费的时候，只会返回 0、1、2 这几个可以消费的进度，因为 3 号为止显然还没有真正存放数据，也许存放这个 3 号为止的生产者线程一直没有被 CPU 调度，总之存放数据不及时，这时候就不能连续消费 0-6 的数据，只能消费 0-2 的数据。这就需要一些方法来保证批处理器消费的数据是真正连续的。而实现这个功能的方法我已经定义到 MultiProducerSequencer 类中了，就在前面的代码块中。其根本逻辑仍然是拿着消费者获得的进度序号到 int 数组中查看对应的位置是否有对应的值。具体测试类我就不展示了，大家可以自己去第三版本代码中学习。都是些非常简单的知识，就不再重复讲解了。

到此为止，这一章就结束了。好了，我们下一章见。