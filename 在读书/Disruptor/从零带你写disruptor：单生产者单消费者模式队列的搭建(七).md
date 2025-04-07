大家好，这一章我将继续为大家讲解 Disruptor 框架的知识。前两章围绕着如何解决伪共享问题，我为大家讲解了高速缓存和字节内存对齐的知识。要想解决伪共享问题，首先就要知道什么是伪共享，而伪共享和高速缓存相关，所以要先简单讲解高速缓存的部分知识。知道了这些前置知识，还不足以解决伪共享问题，因为伪共享问题说到底就是一些数据在内存中的位置比较接近，很容易被放到同一个高速缓存行中，从而造成伪共享问题。解决的方法就是将这些数据的内存地址分隔开，最好能分隔一个高速缓存行的距离。所以这就要求我们了解 Java 对象的各个字段在内存中是怎样排序的。为大家讲解了 JVM 对字段在内存中的排序规则后，我们就可以按照自己的想法填充无效字节解决伪共享了。我们可以填充 long 字节，也可以填充 int 字节，甚至可以视情况填充 byte 字节，并且还会定义三个具有父子类关系层级的类，来包装有效数据和要填充的无效字节。因为在 JVM 给对象字段排序的时候，如果对象有父类，那么会先在该对象中为其父类的字段分配属性，然后才会为该对象的字段分配属性，最后达到这样一种效果：父类填充无效字节——中间子类定义有效数据——子类填充无效字节。这样一来，用户直接创建第三层的子类对象，这个子类对象就可以使用有效数据，并且无效字节也都填充好了。这一章，我们就使用这种方式，看看如何解决 Disruptor 框架中的伪共享问题。当然，要想解决伪共享问题，首先得知道在 Disruptor 框架中，哪里会发生伪共享问题。接下来，就让我来为大家分析一下吧。

**分析 Disruptor 框架中的伪共享问题**

在正式分析这个问题之前，我有一个小问题，想问问大家。请大家想想，在我们编写一个程序的时候，最希望什么数据能够存放到高速缓存中呢？这应该没必要讨论，肯定是经常被 CPU 使用的数据应该放到高速缓存中，最好能一直放到高速缓存中，对吧？比如说现在有一个 People 类型的对象 p，大小为 24 个字节，要被 CPU 一直使用，它在内存中的地址是 0-23，那它肯定会被放到高速缓存的第 0 行的第 0-23 位，CPU 使用该对象数据的时候，只需访问高速缓存即可。这个对象中的数据要经常被 CPU 使用，所以我希望这个对象能一直在高速缓存中放着，并且还有一点，CPU 也会十分频繁地修改这个对象中的数据，但是没关系，反正根据高速缓存的写回策略，只要把更新后的新值仍然放到高速缓存中即可，等到不得不替换的时候，才把它写回内存就行了，注意啊，把新值写回内存，并不意味着这个新值不在高速缓存中了，要理清楚这一点。这个我相信大家都可以理解吧。但现在这个对象只有 24 个字节，高速缓存行一整行有 64 个字节，这就意味着这个 p 对象肯定是要和其他数据共享一个缓存行的，这时候问题就来了，如果其他数据被其他核心修改了，那么频繁使用 p 对象的核心缓存的 p 对象就失效了，不得不再次从内存中读取数据。所以，这种情况下，我只让 p 对象独占一个缓存行即可，p 对象总大小是 24 字节，我只要在 People 类型内部填充额外 40 个无效字节即可，其实就可以填充 5 个 long 字段。当然，这种情况的前提是 p 对象的内存地址为 0-23，并且放在高速缓存第 0 行的 0-23 位。但实际上对象在内存中的地址并不是确定的，或者说并不是程序员能够指定的，所以为了保险起见，我们仍然需要在这个 People 类中填充 14 个 long 字段。并且使用三层级的父子类来填充无效字节，具体实现请看下面代码块。

```
public class Human {
    
    //在父类填充无效字节
    protected long p1, p2, p3, p4, p5, p6, p7;
}


public class value extends Human{
    
    //在中间类定义有效数据
    protected long age;
}


public class People extends value{
    
    //在子类填充无效字节
    protected long p9, p10, p11, p12, p13, p14, p15;
}
```

按照上面代码块中的实现方式，假如创建了 People 类型的 p 对象，那么这个对象中就包含了父类和子类的字段，这样一来就可以使用有效数据，同时也填充了无效字节解决了伪共享，不 p 对象怎么被 CPU 读取到内存，不管 p 对象的内存地址是什么，总之，p 对象的 age 属性肯定是和其他无效字段独占一个高速缓存行的。这样一来，就不会被其他数据干扰了。

如果上面这个例子大家都清楚了，接下来，我想问问大家，在 Disruptor 框架中，有哪个类型的对象会被 CPU 频繁使用呢？或者说是哪个变量最容易被 CPU 频繁访问？首先我就想到了一个类型，那就是 Sequence 进度序号类型。这个类型的具体内容如下。请看下面代码块。

```
public class Sequence {

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

在 Sequence 类中，其实真正定义的有效成员变量就是一个 value，本来我是打算直接用 long 整数来定义这个进度序号的，但是考虑到创建成对象之后，进度序号才容易在各个方法和类之间传递，所以才定义了这个 Sequence 类，并且在这个类中定义了有效数据 value。这个想必大家应该还有印象。那么，我为什么说这个 Sequence 类型的对象更容易被 CPU 频繁使用呢？请大家仔细想想，这个 Sequence 类型的对象会在哪里创建呢？肯定是在被包装过后的消费者中创建，也会在序号生成器中被创建。因为进度序号就是要用来表示消费者和生产者的进度呀。我们就以消费者为例吧，我把被包装过后的消费者的代码再次搬运了一下，大家可以看看下面的代码块。

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

从上面的代码块中可以看到，Sequence 类型的对象 sequence 是被包装过后的消费者 BatchEventProcessor 内部的成员变量。并且在 BatchEventProcessor 类的 processEvents 方法中，可以看到，当消费者每消费了一个生产者数据之后，其消费进度都会加 1，最后通过 sequence.set(availableSequence) 这行代码给消费者进度序号对象 sequence 赋值。所谓赋值就是修改其内部成员变量 value 的值。这个逻辑相比大家都不陌生了。为什么单独把这个逻辑再复述一遍呢？因为这就是 CPU 对消费者所做的处理中的重要一步。在生产者消费者模型中，当生产者和消费者的速度相当时，大概是生产者生产了多少数据，消费者就要消费多少数据，消费者的消费速度应该是很快的，这也就意味着消费者的消费进度肯定会被频繁修改，也就意味着消费者的消费进度对象 sequence 肯定也会被 CPU 频繁访问。既然是这样，那就应该把 sequence 对象放到高速缓存中呀，这样一来 CPU 就可以直接从高速缓存中获取该对象中的 value，然后修改它的值，当然，根据写回策略，肯定也不必立刻把它写回到内存中。这样 CPU 只要使用 sequence 对象中的数据，只需要跟高速缓存打交道即可。如果是这样的话，显然不能让其他数据和 sequence 对象中的 value 变量共享缓存行，免得产生伪共享问题。因为说到底，程序员也根本不知道 sequence 对象会被分配的内存的哪个地方，内存地址是多少，也不知道这个对象附近的数据都是什么。现在程序中确实只有一个消费者线程，如果处理器的一个核心在执行这个线程的任务，那么这个核心就会把 sequence 对象的数据和其附近的数据缓存到自己的高速缓存行中。但假如有其他的核心恰好要访问 sequence 对象附近的数据呢？不正好也把 sequence 对象缓存到它的高速缓存中了吗？只要对数据进行修改，伪共享问题就出现了。所以，我们能做的就是填充无效字节，使 sequence 对象中的 value 变量独占缓存行。既然是这样，那么我们就可以这样重构 Sequence 类。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/24
 * @Description:生产者和消费者使用的序号对象，其实就是一个long整数，但是为了解决伪共享，就把这个long整数封装在一个Sequence对象中了
 */
class LhsPadding {   
    //在父类填充空字节
    protected long p1, p2, p3, p4, p5, p6, p7;
}

class Value extends LhsPadding {
    //value前后都有7个空的long整数，也就是56个空白字节，加上当前的value正好是64个字节
    //高速缓存一行缓存64个字节，这样不管怎么读取这个value，它都会自己独占一个缓存行，不会出现伪共享了
    protected volatile long value;
}

class RhsPadding extends Value {   
    //在子类填充空字节
    protected long p9, p10, p11, p12, p13, p14, p15;
}

//在上面，我创建了三个具有父子类层级关系的类，并且填充了无效字节，解决了value变量的伪共享问题
//接下来就是Sequence类了
public class Sequence extends RhsPadding {  
    
    //消费进度初始值，默认为-1
    static final long INITIAL_VALUE = -1L;
    //Unsafe对象，这个Unsafe对象定义也有一点深意，就是直接通过value在sequence对象中的内存偏移量
    //来直接修改value的值，使用Unsafe的话可以调用Unsafe的putLongVolatile和putOrderedLong方法
    //关于这两个方法的介绍我也写在下面方法的注释中了
    private static final Unsafe UNSAFE;
    //value在对象中的内存偏移量
    private static final long VALUE_OFFSET;

    static {
        //得到Unsafe对象
        UNSAFE = Util.getUnsafe();
        try {   
            
            //该方法的作用就是得到value在对象中的内存偏移量，这样就能直接根据内存偏移量，让Unsafe对象直接操纵这个变量了
            VALUE_OFFSET = UNSAFE.objectFieldOffset(Value.class.getDeclaredField("value"));
        }
        catch (final Exception e) {
            throw new RuntimeException(e);
        }
    }
    
    //构造方法
    public Sequence() {
        this(INITIAL_VALUE);
    }

    //设置value的初始值，也就是-1L
    public Sequence(final long initialValue) {
        UNSAFE.putOrderedLong(this, VALUE_OFFSET, initialValue);
    }

    //返回value
    public long get() {
        return value;
    }

    //设置value的值
    public void set(final long value) { 
        //putOrderedLong方法并不会保证内存的立即可见性，只会保证不被指令重拍，也就是说，更新了value，该值可能并不会立刻被其他线程可见
        //因此该方法更新value会比下面的方法稍微快一点，这也就导致了消费者不能总是立刻得到最新的生产者进度，但这没有关系
        //反正总会被消耗到。由此也可以看出，这个框架为了尽量提升性能，真的是用尽了很多手段
        //当然，这些手段其他框架也都用过，实际上Netty中也有这样的方法
        //当然，我还要强调一点，这个方法设置不会被其它线程立刻看到，但并不是说不会看到，只是有一定延迟，可能会有几纳秒
        UNSAFE.putOrderedLong(this, VALUE_OFFSET, value);
    }

    
    //使用Volatile设置value的值，因为是Volatile，所以该方法就保证了内存可见性，同时也禁止了指令重排
    //因为保证了立即可见性，更新的value是要写入到主存中的，这样才能被各个线程看见，所以写入的动作会稍微慢一下
    //所以该方法并不会被经常使用，只会在生产者因为消费进度比较落后的时候，调用次方法更新一下生产者最新进度
    //让消费者看到，然后让消费者及时去消费
    public void setVolatile(final long value) {
        UNSAFE.putLongVolatile(this, VALUE_OFFSET, value);
    }

    //该方法是在多生产者模式下使用的方法
    public boolean compareAndSet(final long expectedValue, final long newValue) {
        return UNSAFE.compareAndSwapLong(this, VALUE_OFFSET, expectedValue, newValue);
    }


    public long incrementAndGet() {
        return addAndGet(1L);
    }


    public long addAndGet(final long increment) {
        long currentValue;
        long newValue;

        do {
            currentValue = get();
            newValue = currentValue + increment;
        }
        while (!compareAndSet(currentValue, newValue));

        return newValue;
    }

    @Override
    public String toString() {
        return Long.toString(get());
    }
}
```

Sequence 类在上面代码块中经过重构之后，在被包装过后的消费者 BatchEventProcessor 中创建 Sequence 类的对象，显然就不会再有伪共享问题了。如果这个逻辑理解了，接下来，我再为大家分析另一处需要重构的地方。其实这个非常明显，你想啊，需要频繁被 CPU 频繁访问的不仅仅是进度序号，既然消费者要消费生产者数据，消费了生产者数据之后才会修改进度序号，那说明生产者数据肯定更要被 CPU 频繁访问了。所以为了避免伪共享问题，只好把生产者数据也放进高速缓存行中了。但是生产者数据是存放在环形数组中的，这也很好办，那就把这个数组放到高速缓存行中就行了。向数组的前后填充无效字节，使数组独占缓存行即可。

**重构 RingBuffer 环形数组**

这个逻辑是没问题的，但是有一点我需要向大家解释一下，高速缓存行每行只能缓存 64 个字节，如果一个数组存放的有效数据是 512 个字节怎么办？这时候这个数组的有效数据显然要占据 8 个缓存行了。如果在这个数组前后各填充 7 个 long 整数，是不是就可以保证数组的有效数据肯定是独占这 8 个缓存行的？这个显然是可以确定的吧？既然数组的有效数据独占了 8 个缓存行，显然也不会有其他数据来污染数组了，对吧？所以，就这样解决数组的伪共享问题吧。当然，先不要急着重构代码，上面的分析有一个很明显的漏洞，那就是在数组前后各填充 7 个 long 整数，也就是 56 个字节，这真的可以保证数据独占 8 个缓存行吗？这就得看情况了呀，如果数组存放的也都是 long 整数，那么 7 个 long 整数加上数组的第一个 long 整数正好是 64 个字节，数组最后一位 long 整数加上后面填充的 7 个 long 整数，正好是 64 个字节，这就保证了数组独占 8 个缓存行。但是，数组存放的如果不是 long 整数呢？如果是 int 整数，或者 byte 呢？或者是 4 个字节大小的对象引用呢？这不就坏菜了吗？所以，怎么向数组前后填充无效字节，以及填充什么无效字节，这个显然是要针对具体情况具体分析了。

在 Disruptor 框架中，因为给环形数组 RingBuffer 引入了泛型，所以存放在环形数组中的数据肯定是引用，还以前几节课中使用的 Request 对象来举例，环形数组中每一位存放的显然应该是 Request 对象的引用，而引用的大小为 4 个字节，所以，环形数组中每个位置的字节大小就为 4 字节。既然确定了环形数组每一位的字节大小，那就可以填充无效字节了。这样一来，我们是不是只要在环形数组前后各填充 60 个字节，也就是 15 个 int 整数就行了？理论上来说是这样的，但是请大家再仔细想想，要把环形数组的有效数据放到 8 个高速缓存行中，注意我说的是有效数据，这意味着数组对象的对象头这些数据是不会独占缓存行的，就像 Sequence 进度序号对象，也只是让 value 独占了缓存行，sequence 对象的对象头可没有独占缓存行。所以这时候我们就应该让环形数组的有效数据和它的对象头以有效数据之后的其他数据分隔开。具体情况如下所示，请看下面代码块。

```
｜数组对象头｜数组存放的有效数据，也就是每一个Request对象的引用｜其他字段数据

//我们现在要做的就是在有效数据前后填充无效字节，使有效数据，假如还用之前512个字节为例
//那就是让512个字节独占8个缓存行，具体实现如下。
｜数组对象头｜填充的60个无效字节｜512个有效数据｜填充的60个无效字节｜其他字段数据
```

本来按照上面的方式就可以真正解决数组有效数据的伪共享问题了，但是在 Disruptor 框架中，开发者考虑的比较多，它考虑到不同计算机平台的高速缓存实现存在差异，比如有的计算机高速缓存的缓存行为 64 字节大小，有的计算机高速缓存的缓存行为 128 字节大小。64 * 2 = 128。换句话说如果填充 128 个无效字节，肯定也是兼容缓存行为 64 字节的高速缓存的。因此，Disruptor 框架的开发者就稍微粗暴了一些，也不考虑数组对象头的大小是多少了，直接在数组有效数据的前后各填充了 128 个无效字节，也不是使用的 int 整数，就是创建环形数组的时候把数组的长度设置为用户定义的环形数组大小加上填充的 128 * 2 个字节了。然后计算出环形数组中有效数据在数组对象中的内存偏移量，直接用 Unsafe 对象根据偏移量填充数据，假如用户定义的环形数组的有效数据是 512 字节，那么有效数据只填充 512 个字节，其他没填充的就是无效字节了，正好起到了解决伪共享问题的作用。具体实现如下。

```
｜数组对象头｜填充的128个无效字节｜512个有效数据｜填充的128个无效字节｜其他字段数据
                        	｜填充有效数据的时候｜
							｜直接使用Unsafe对象｜
							｜从此处开始填充，填充｜
							｜完512个字节后即停止｜
							｜不再向后填充，之后就｜
							｜可以循环填充这512个字节了｜
```

当然，解决伪共享仍然要采用我之前为大家设计的具有父子关系层级的三个类方式。因此，RingBuffer 类应该以下面代码块中的方式进行重构，请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/23
 * @Description:环形数组的实现类，在该类中，用不到的方法都注释了，虽然这个类看着内容很多，其实在第一版本中内容很少
 */
abstract class RingBufferPad { 
    
    //要为数组的有效数据解决伪共享，字节填充应该在有效数据前后
    //但这里仍然使用老方法填充了7个long，这个填充当然不能说是多余的
    //结合其子类RingBuffer类中填充的7个long字段，至少可以保证RingBufferFields类中的所有字段
    //独占缓存行吧，比如说可以让数组的引用也放到高速缓存中
    protected long p1, p2, p3, p4, p5, p6, p7;
}

//有效数据，也就是环形数组定义在这个中间字类中了
abstract class RingBufferFields<E> extends RingBufferPad {
    
    //要在数组自身中填充的空数据的个数，如果这个值是32，数组的每一位都是4字节的对象引用，那就意味着
    //要在有效数据之前填充32*4=128个空字节，要在有效数据之后也填充128个空字节
    private static final int BUFFER_PAD;
    //对于数组这个对象来说，其内存布局分为数组对象头和有效数据部分，所谓有效数据就是数组真正存储的那部分数据
    //下面这个属性就是数组存储的第一个有效数据，在数组这个对象中的内存偏移量
    private static final long REF_ARRAY_BASE;
    //这个属性是用来计算数组中应该填充的空白字节大小的，会被2赋值
    //实际上就是计算数组每一位的大小，因为数组存放的都是对象引用，所以每一位大小为4字节
    private static final int REF_ELEMENT_SHIFT;
    //用Unsafe类的对象操纵数组
    private static final Unsafe UNSAFE = Util.getUnsafe();

    static {   
        //计算出数组每个位置的字节大小，每一位都是4个字节
        final int scale = UNSAFE.arrayIndexScale(Object[].class);
        if (4 == scale) {
            //这里会把REF_ELEMENT_SHIFT赋值为2，下面会用到
            REF_ELEMENT_SHIFT = 2;
        }
        else if (8 == scale) {
            REF_ELEMENT_SHIFT = 3;
        }
        else {
            throw new IllegalStateException("Unknown pointer size");
        }
        //这里得到的是32，说明要填充的空数据的个数是32个，这里的意思就是有效数据之前填充32个4字节
        //有效数据之后填充32个4字节
        BUFFER_PAD = 128 / scale;
        //BUFFER_PAD << REF_ELEMENT_SHIFT这行代码的意思就是填充32个空数据，每个数据是4字节，所以左移2位，就是乘以4，得到的就是128
        //也就是填充的这些数据一共是128个字节，而UNSAFE.arrayBaseOffset(Object[].class)得到的是数组中有效的首元素
        //在数组中的偏移量，首元素的地址加上要填充的字节数，得到的就是首元素相对于数组对象的最终偏移量
        //注意，这里的32个4字节都是要填充到数组头部的，也就是有效首元素的前面，所以才能这样计算首元素的偏移量
        REF_ARRAY_BASE = UNSAFE.arrayBaseOffset(Object[].class) + (BUFFER_PAD << REF_ELEMENT_SHIFT);
    }
    //用来进行位运算求取数组索引的掩码
    private final long indexMask;
    //存放数据的数组，可以抽象成环形数组
    private final Object[] entries;
    //环形数组的容量，必须是2的n次幂
    protected final int bufferSize;
    //序号生成器，用来给生产者分配可用序号的
    //SingleProducerSequencer序号生成器实际上实现了Sequencer接口
    protected final Sequencer sequencer;

    //构造方法
    RingBufferFields(EventFactory<E> eventFactory,Sequencer sequencer) {
        
        this.sequencer = sequencer;
        this.bufferSize = sequencer.getBufferSize();

        if (bufferSize < 1) {
            throw new IllegalArgumentException("bufferSize must not be less than 1");
        }
        //这个方法的逻辑之前也讲过了，就是判断容量是否为2的n次幂
        if (Integer.bitCount(bufferSize) != 1) {
            throw new IllegalArgumentException("bufferSize must be a power of 2");
        }

        //给掩码赋值
        this.indexMask = bufferSize - 1;
        //下面这行代码很重要，可以看见，数组的容量并不真的是bufferSize，而是在bufferSize的基础上
        //加上了64，因为BUFFER_PAD是32，两个BUFFER_PAD就是64。这意味着bufferSize是真正有效数据的容量，而添加的64都是空数据，在有效数据之前填充32个4字节
        //在有效数据之后再填充32个4字节，这样就可以彻底保证数组的有效数据不会被其他属性伪共享了
        this.entries = new Object[sequencer.getBufferSize() + 2 * BUFFER_PAD];
        //初始化数组，创建数组中的对象，这个对象可以一直复用，在一定程度上减少垃圾回收
        //但是该对象中封装的对象，仍然会被垃圾回收，这个要搞清楚
        fill(eventFactory);
    }

    //在该方法中初始化数组，创建了数组中的每一个对象
    private void fill(EventFactory<E> eventFactory) { 
        //还是根据有效容量循环的
        for (int i = 0; i < bufferSize; i++) {
            //在这里数组的首位数据的索引就是BUFFER_PAD + i
            //因为BUFFER_PAD是有效数据之前填充的空数据的个数，所以要跳过这些空数据，然后进行真正的对象的创建
            //这样一来就做到了只给有效数据的位置赋值，其他空位置全没有赋值
            //这里我还要多说一句，这种方式创建数组中的对象，可以让这些对象在堆内存中的地址尽可能的连续一点
            //现在数组中存放的只是对象的引用，这个要记住，但同一时间创建很多同类型对象，这些对象的内存地址很可能是连续的
            entries[BUFFER_PAD + i] = eventFactory.newInstance();
        }
    }

    //该方法也很重要，就是根据序号得到数组相应位置的数据
    @SuppressWarnings("unchecked")
    protected final E elementAt(long sequence) {  
        //sequence & indexMask计算的是要获取的有效数据原本的下标索引，下标计算出来了之后，乘以每个索引的大小，也就是4字节，得到的就是下标
        //位置的数据在数组中的偏移量，这个偏移量再加上之前计算出来的第一个有效数据在数组中的偏移量，得到的就是要查询的真正数据在数组中的
        //偏移量，然后通过Unsafe获得就行了
        //   ==========         这里的偏移量要单独计算     ====          ====           ====         .......
        //数组对象头和填充的无效字节   第一个有效数据      第二个有效数据   第三个有效数据
        return (E) UNSAFE.getObject(entries, REF_ARRAY_BASE + ((sequence & indexMask) << REF_ELEMENT_SHIFT));
    }
}

//环形数组子类，在该类中填充了
public final class RingBuffer<E> extends RingBufferFields<E> implements Cursored, EventSequencer<E>, EventSink<E> {
    //填充字节，解决伪共享。这个字节和最开始父类填充的字节是为了不让中间的类，也就是定义了数组的那个类中的属性被其他类伪共享
    //要理解这一点，还得掌握Java对象在内存中的布局，以及怎么被读取到高速缓存中的知识
    protected long p1, p2, p3, p4, p5, p6, p7;

    //构造方法
    RingBuffer(EventFactory<E> eventFactory,Sequencer sequencer) {
        super(eventFactory, sequencer);
    }

    
	//其他内容暂时省略
}
```

以上代码块中的代码，就把 RingBuffer 类重构完了，所有逻辑在注释中讲解得非常详细，所以就不再重复讲解了。当这里，我相信大家对 Disruptor 框架中的一些奇奇怪怪的代码总算是可以看懂了吧？到此为止，解决伪共享应该是大家都已经掌握的本领了。如果大家对上面的逻辑都理解了，那么接下来，我们就看本章的最后一个问题。

**为消费者进度定义缓存变量**

最后一个问题就稍微有点绕了，在开始讲解之前，请大家先看看下面的两个代码块。

首先是被包装后的消费者，也就是 BatchEventProcessor 类。请看下面代码块。

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

接下来请看序号生成器 SingleProducerSequencer 类。

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

在上面两个代码块中，大家可以看到，消费者内部持有着自己的进度序号 sequence，而序号生成器内部也持有消费者进度序号的引用 consumer。这个是一目了然的，虽然还没讲到多线程消费者模型，但是现在我不得不先引入一下后面的知识，来为大家纠正一个现有的问题。实际上，在 SingleProducerSequencer 内部并不是持有着消费者的进度序号的引用 consumer，而是持有着一个存放消费者序号进度的数组，就像下面这样。

```
//序号数组，这个数组中存储的都是消费者的消费进度，通过这个数组，就能找到所有消费者中，消费最慢的那个进度
protected volatile Sequence[] gatingSequences = new Sequence[0];
```

为什么要持有一个存放所有消费者进度序号的数组呢？因为以后我们总要把框架过渡到多消费者模式，不可能永远是一直是一个消费者吧，这样消费起来多慢呀。所以要把程序中的所有消费者的消费进度收集起来，放到一个数组中，然后把这个数组交给序号生成器，就相当于生产者掌握了所有消费者的消费进度，当生产者往环形队列中存放生产者数据时，只需要去这个消费者进度数组中看一眼，哪个消费者的进度最慢，比如有 3 个消费者，1 号消费者消费到进度 3 了，2 号消费者消费到进度 5 了，3 号消费者消费到进度 6 了，这就意味着 1 号消费者还有 4、5、6 号数据没消费呢。因此生产者存放生产者数据时只能放到序号 3 的位置，不能放到 4、5、6的位置，因为 1 号消费者还没消费这些数据呢。这个逻辑大家可以品味一下。如果这个逻辑理解呢，那么就接着往下看。

现在的问题是什么，如果程序中有非常多的消费者，比如说有 10 个消费者，每个消费者都要创建自己的消费者进度，那就意味着程序有 10 个 sequence 对象。而程序中还是只有一个生产者，生产者持有者这个存放着 10 个sequence 对象引用的 gatingSequences 数组。假如说，现在有一种非常极端的情况，那就是处理器有两个核心，其中 1 号核心运行着 10 个消费者线程，2 号核心运行着 1 个生产者线程。这就意味着 1 号处理器会把 10 个消费者的 10 个 sequence 序号对象(其实是对象内部的 value 属性，这里就简化一下了，大家理解即可)放到高速缓存行中，每个都是独占一个缓存行，这个可以理解吧？而 2 号核心呢？很不巧，因为 2 号核心运行着生产者线程，生产者线程要用到序号生成器，并且要通过序号生成器持有的 gatingSequences 数组访问所有消费者进度，要知道这些消费者的进度，以此来决定生产的数据能放到哪个位置。生产者如果一直生产数据的话，这就意味着生产者线程要非常频繁地访问这些消费者的序号进度对象。那这种情况下，运行着生产者线程的 2 号核心肯定也会把这 10 个 sequence 对象放到自己的高速缓存中呀。这个能理解吧？

最大的问题来了，偏偏消费者进度是会经常被 1 号核心修改的，因为消费者每消费了数据就会更新自己的消费进度呀，更新后的新值会放到 1 号核心的高速缓存中，并不会立刻写入内存。但是这些 sequence 对象的值一被修改，就会通知 2 号核心缓存的 10 个 sequence 对象全部失效，2 号核心运行的生产者线程再想访问 10 个 sequence 对象的值，显然就要重新去内存中访问了。这样一来，2 号核心的高速缓存对于 10 个 sequence 对象来说不就全失效了吗？也就是缓存没有命中，如果 10 个 sequence 对象内部的 value 的值频繁被 1 号线程修改，那么 2 号核心缓存的 10 个 sequence 对象的 value 的值就会频繁失效，这不是白忙一场吗**(与此同时还会出现一种情况，那就是核心 2 要去内存访问 10 个 sequence 对象的数据时，内存中的数据是旧的，这时候就需要 1 号核心把高速缓存中的新数据更新到内存中，如果 2 号核心的缓存经常不命中，就意味着它要经常去访问内存，也就意味着 1 号核心要经常把数据同步到内存，这对 1 号核心来说显然也是一种性能上的拖累，这种情况，大家可以仔细品味一下)**？占着高速缓存但是发挥不了高速缓存的作用，真是浪费呀，倒不如减少这些数据放到核心 2 的高速缓存中的概率。所以，Disruptor 框架就对此做了一个优化。

所谓的优化，就是在 SingleProducerSequencer 序号生成器类中还定义了一个 long 基本类型的 cachedValue 变量，如下所示。

```
/**
     * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
     * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
     * @Date:2023/8/24
     * @Description:这个成员变量也很重要，大家应该还记得gatingSequences成员变量吧，是个数组
     * 这个数组中存储的都是消费者的消费进度，通过这个数组，就能找到所有消费者中，消费最慢的那个进度。最慢的消费者进度就会被赋值给下面的这个
     * 成员变量，所以查找最慢的消费者变量时并不是直接去数组中查看，而是根据这个缓存值获得
     * 这个缓存值的初始值也是-1，这样根据缓存的这个最慢消费者进度去判断生产者数据放到那个位置，这样一来就不用频繁访问gatingSequences
     * 数组中的每个值了，也就不会一直触发缓存不命中，只有当cachedValue的值不能让生产者继续生产数据时，比如消费者进度是4，消费了4，该消费5了，
     * 生产者的进度也是4，该往5存放数据了，但5还没被消费者消费，才会真正去访问gatingSequences数组
     * 得到一个最新的最慢消费者进度，然后再赋值给这个cachedValue变量
     */
    long cachedValue = Sequence.INITIAL_VALUE;
```

当然，我们目前的第一版本代码，搭建的是单生产者单消费者队列，既然是单消费者队列，那么程序中显然只会有一个消费者进度，所以 SingleProducerSequencer 内部定义的 gatingSequences 数组只会存放一个消费者进度序号的引用。这一点，大家在学习代码的时候要理清楚。以上的知识都讲解完了，最后我还要再给大家补充一点。在上面的代码块中，我们使用了一个 cachedValue 来承接了程序中最慢的消费者进度。这就意味着缓存的这个值肯定是要被运行生产者线程的 2 号核心频繁访问的，这也就意味着这个 cachedValue 是完全可以放到高速缓存行中的，并且需要解决伪共享问题。那么，这样一来单生产者序号生成器 SingleProducerSequencer 类不就可以重构成具有父子类层级关系的三个类吗？具体重构方式请看下面代码块。

首先是 AbstractSequencer 类。为什么会有一个抽象类呢？原因很简单，在我们目前的程序中，使用的一直都是 SingleProducerSequencer 类，这个类翻译过来就是单生产者序号生成器，是在单生产者模式下使用的序号生成器。后面程序还会过渡到多生产者模式，多个生产者线程要同时向环形数组中存放数据，显然就需要多生产模式下的序号生成器了。所以就给这两个序号生成器搞了一个抽象父类，定义了一些公共属性和模版方法。请看下面代码块。

```
/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/19
 * @Description:序号生成器的抽象父类，该类再往下细分会有多生产者序号生成器和单生产者序号生成器
 * 在第一版本代码中，我只为大家引入了单生产者序号生成器，也就是SingleProducerSequencer类
 */
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

    //构造方法
    public AbstractSequencer(int bufferSize, WaitStrategy waitStrategy) {
        //环形数组的容量小于1直接抛出异常
        if (bufferSize < 1) {
            throw new IllegalArgumentException("bufferSize must not be less than 1");
        }
        //Integer.bitCount方法的作用就是计算一个整数的二进制中有多少个1
        //单看这个好像没什么用，但是有一点大家一定要注意，那就是凡是2的n次幂的数，其二进制中1的个数永远是1
        //所以下面才会判断是不是等于1，如果不是1，说明环形数组的容量不是2的n次幂，那就无法使用长度-1做与运算的操作
        //计算相应的数组下标。其实从这里也能看到，一旦涉及到容量，在很多框架中容量为2的n次幂已经是一个标配了
        if (Integer.bitCount(bufferSize) != 1) {
            //数组容量不符合要求就直接抛异常
            throw new IllegalArgumentException("bufferSize must be a power of 2");
        }
        //在这里给环形数组容量和等待策略赋值
        this.bufferSize = bufferSize;
        this.waitStrategy = waitStrategy;
    }


    //其他内容暂时省略
}
```

接下来是 AbstractSequencer 的子类，请看下面代码块。

```
//序号生成器类，该类也用到了空白字节填充来解决伪共享问题
abstract class SingleProducerSequencerPad extends AbstractSequencer {	
    //填充无效字节，解决伪共享
    protected long p1, p2, p3, p4, p5, p6, p7;

    SingleProducerSequencerPad(int bufferSize, WaitStrategy waitStrategy) {
        super(bufferSize, waitStrategy);
    }
}

//下面这个类定义了有效数据
abstract class SingleProducerSequencerFields extends SingleProducerSequencerPad {
    
    SingleProducerSequencerFields(int bufferSize, WaitStrategy waitStrategy) {
        super(bufferSize, waitStrategy);
    }

    
    //该成员变量就是当前分配的可用的序号，可以看到初始值也是-1L
    long nextValue = Sequence.INITIAL_VALUE;

    
	//这个成员变量也很重要，大家应该还记得AbstractSequencer类中的gatingSequences成员变量吧，是个数组
    //这个数组中存储的都是消费者的消费进度，通过这个数组，就能找到所有消费者中，消费最慢的那个进度。最慢的消费者进度就会被赋值给下面的这个
    //成员变量，所以查找最慢的消费者变量时并不是直接去数组中查看，而是根据这个缓存值获得
    //这个缓存值的初始值也是-1
    long cachedValue = Sequence.INITIAL_VALUE;
}

/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/24
 * @Description:单生产者使用的序号生成器
 */
public final class SingleProducerSequencer extends SingleProducerSequencerFields {
    //解决伪共享填充的无效字节
    protected long p1, p2, p3, p4, p5, p6, p7;

    //构造方法
    public SingleProducerSequencer(int bufferSize, WaitStrategy waitStrategy) {
        super(bufferSize, waitStrategy);
    }

    //之后就是一些方法了，和之前SingleProducerSequencer类中定义的方法都一样，就不再展示了
    @Override
    public boolean hasAvailableCapacity(int requiredCapacity) {
        return hasAvailableCapacity(requiredCapacity, false);
    }


    //省略部分内容
}
```

**最终梳理一次程序的执行流程**

课程进行到这里，如果梳理执行流程，肯定就不能把全部流程都梳理一遍了，因为现在有很多类了，全部梳理要讲解的流程就太多了。所以，这一次我们只看程序的启动流程，看看各个组件是怎么组装在一起的，然后再看看如今程序启动，和之前我们讲解的有什么不同。在下面的代码块中，我为大家准备了一个测试类，请看下面代码块。

首先是环形数组中要存放的对象，这里我重新定义了一个 Event 类，如下所示。

```
public class Event<T> {

    private T data;

    public T getData() {
        return data;
    }

    public Event<T> setData(T data) {
        this.data = data;
        return this;
    }
}
```

接下来是生产该对象的工厂 SimpleEventFactory 类。

```
public class SimpleEventFactory<T> implements EventFactory<Event<T>> {

    @Override
    public Event<T> newInstance() {
        return new Event<>();
    }
}


//这里多说一句，在测试类中创建该工厂的时候，我会把泛型定义为String，这也就意味着生产者线程
//真正生产的数据就是String
SimpleEventFactory<String> eventFactory = new SimpleEventFactory<>();
```

接着就是用户自己定义的消费者处理器 SimpleEventHandler，请看下面代码块。

```
public class SimpleEventHandler<T> implements EventHandler<Event<T>> {

    @Override
    public void onEvent(Event<T> event, long sequence, boolean endOfBatch) throws Exception {
        //这里，我就只打印一句话了
        System.out.println("消费者线程"+Thread.currentThread().getName()+"已经消费了"+event.getData()+"数据了！");
    }
}
```

接下来就是测试类了，请看下面代码块。

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

上面测试类的每一个步骤都很清楚吧，可以说到目前为止，测试类中的逻辑和我们之前讲解的没什么不同，我重点要为大家梳理的也不是这个，在上面的代码块中，有两行最重要的代码，全靠这两行代码，我们定义的各个组件才能按照最优的顺序完整拼装起来。就是下面这两行代码。

```
//创建disruptor，注意，这里使用的就是单生产者模式
Disruptor<Event<String>> disruptor = new Disruptor<>(eventFactory, ringBufferSize, threadFactory, SINGLE, waitStrategy);

//把消费者设置到disruptor中，然后被包装成一个BatchEventProcessor对象
disruptor.handleEventsWith(eventHandler);
```

接下来，我会围绕着这两行代码，为大家剖析程序的执行流程，其他的代码和细节就留给大家自己去我提供的源码中学习吧。首先我们看上面代码块中的第一行代码，也就是创建了一个 Disruptor 对象。

在创建这个对象的时候，大家会看见一个 SINGLE 参数传递到了该对象的构造方法中，这个参数之前我从来没有跟大家讲过，其实它就是一个枚举类型的对象，请看下面代码块。

```
//生产者的类型枚举类
public enum ProducerType {
    //单生产者模式
    SINGLE,

    //多生产者模式
    MULTI
}
```

在第一版本代码中，我传递进去的是单生产者模式，那么在创建 Disruptor 对象的过程中，还会创建序号生成器，到时候就会根据传递的这个 SINGLE 枚举对象创建单生产者序号生成器。我们可以进一步看一下创建 Disruptor 对象的整个流程。

首先就是 Disruptor 的构造方法，请看下面代码块。

```
public Disruptor(final EventFactory<T> eventFactory,final int ringBufferSize,final ThreadFactory threadFactory,final ProducerType producerType,final WaitStrategy waitStrategy) {
        
    this(
        //第一个参数为生产者类型，分单生产者模式和多生产者模式
        //第二个参数就是事件创建工厂，其实这每一个事件就是消费者要消费的东西，也就是消费者线程要执行的任务
        //第三个参数就是RingBuffer的容量
        //第四个参数就是等待策略
        //在这里创建真正存储数据的容器RingBuffer
        RingBuffer.create(producerType, eventFactory, ringBufferSize, waitStrategy),
        //创建线程工厂
        new BasicExecutor(threadFactory));
}


 //在这里给Disruptor中的两个成员变量赋值
private Disruptor(final RingBuffer<T> ringBuffer, final Executor executor) {
    //环形数组类赋值
    this.ringBuffer = ringBuffer;
    //创建消费者线程的执行器赋值
    this.executor = executor;
}
```

在上面的代码块中，环形数组 RingBuffer 和线程执行工厂 BasicExecutor 就都创建成功了。这两个组件非常重要，一个代表了消费的数据，一个代表了消费者线程的创建和启动。当然，在创建 RingBuffer 环形数组的过程中，还通过 RingBuffer.create(参数······) 方法创建了单线程序号生成器，具体逻辑请看下面代码块。

```
public static <E> RingBuffer<E> create(ProducerType producerType,EventFactory<E> factory,int bufferSize,WaitStrategy waitStrategy) {
    switch (producerType) {
        //创建单生产者模式下的序列号生成器
        case SINGLE:
            //该方法的实现逻辑就在下面
            return createSingleProducer(factory, bufferSize, waitStrategy);
            
            //下面创建的是多生产者序号生成器，但多生产者还未引入，所以先注释了
//          case MULTI:
//              return createMultiProducer(factory, bufferSize, waitStrategy);
        
        default:
            throw new IllegalStateException(producerType.toString());
    }
}


/**
 * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
 * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
 * @Date:2023/8/23
 * @Description:创建单生产者的序号生成器，在该方法中，环形数组也被创建了
 */
public static <E> RingBuffer<E> createSingleProducer(EventFactory<E> factory,int bufferSize,WaitStrategy waitStrategy) {
    //创建单生产者序列号生成器
    SingleProducerSequencer sequencer = new SingleProducerSequencer(bufferSize, waitStrategy);
    //创建环形数组，并且序号生成器也交给环形数组持有了
    return new RingBuffer<E>(factory, sequencer);
}
```

好了，到此为止我们已经把创建 Disruptor 对象的流程看完了，在这个流程中，环形数组 RingBuffer，线程执行工厂 BasicExecutor 以及单生产者序号生成器 SingleProducerSequencer 组件都被成功创建并且组装完成了。接下来，就让我们看看 disruptor.handleEventsWith(eventHandler) 这行代码的逻辑。请看下面代码块。

```
//把用户定义的消费事件包装成一个个消费者runnable任务，这些任务就会交给线程来执行
//这个方法的参数是个可变参数，因为用户可能会定义多个消费者事件，也就是多个消费者
//虽然这里的参数是可变参数，但是在第一版本代码中，显然只有一个消费者，当然也就创建了一个消费者处理逻辑
//所以这里只会传进来一个handler，这个大家要弄清楚
public final EventHandlerGroup<T> handleEventsWith(final EventHandler<? super T>... handlers) {
    //注意，下面方法的第一个参数为数组，这里默认使用的是空数组，这个数组的作用会在后面的版本讲到，但是为了大家理解顺利
    //我要在这里简单解释一下，这个数组存放的明显也是进度序号对象的引用，实际上这个数组存放的是消费者的进度序号引用
    //因为这个框架后面还会增添顺序以来消费的功能，比如用户定义了1、2、3、4、5个消费者，其中消费者4、5必须在
    //消费者1、2、3之后才能消费，也就是说，假如有个生产者数据1，那么必须等待消费者1、2、3消费了这个数据1，消费者4、5
    //才能消费这条数据。所以其实用户每次包装消费者处理器的时候，都是可以指定这个消费者处理器在那些消费者
    //之后执行，那肯定就要知道先消费的消费者的进度吧？知道了它们的进度，才知道什么时候轮到自己这个消费者执行了
    //所以要把这些先执行的消费者的消费进度放在一个数组中，传递进来。但是在第一版本中根本用不到这个功能
    //所以我这里提供的代码就是一个空数组，当然源码中也是这样写的，如果要包装的这个消费者不需要依赖其他消费者的消费进度
    //那就不需要知道其他消费者的消费进度呀，所以定义一个空数组即可
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

   //检查Disruptor是否启动过，因为要保证Disruptor只能启动一次
   //创建EventProcessor对象的动作肯定要在Disruptor启动之前，所以这里必须检查一下
   checkNotStarted();
   
    //定义一个Sequence数组，Sequence其实就是消费者的消费进度，也可以说是消费者的消费序号
   //这个数组的长度就是用户定义的eventHandlers的长度，这个数组，以后就要赋值给AbstractSequencer类中的gatingSequences成员变量
    //别忘了，生产者肯定是要知道消费者进度的，这是之前讲解过的知识，所以在得到消费者的所有进度之后，要交给生产者使用呀，也就是交给序号生成器
    //当然，这里数组中只会有一个消费者序号进度对象的引用，因为只有一个消费者，并且这里也只是把数组创建进来，还没有真正填充数据
   final Sequence[] processorSequences = new Sequence[eventHandlers.length];
   
	//创建消费者的序号屏障，这个方法在第一版本还无法讲解得十分全面，本身序号屏障的工作就是协调生产者和消费者
   //的进度关系，消费者不能走到生产者前面。但是在下面的这个方法中，有一个barrierSequences参数，这个参数
   //是一个数组，这个数组的作用要在后面的版本才能讲解，这里只简单解释一下，比如说，现在有一些消费者
   //这些消费者要在另一些消费者消费完了某条消息后，才能消费这条消息，这种消费的顺序关系也是由
   //消费屏障来处理的，如果现在定义的这个消费者并不依赖其他消费者，所以这里的的这个数组只是一个空数组
    //关于ProcessingSequenceBarrier序号屏障这个类，也有一点小小的重构，大家可以从代码中看看
    //工作的逻辑和之前是一摸一样的。
   final SequenceBarrier barrier = ringBuffer.newBarrier(barrierSequences);
   
    //把用户定义的每一个eventHandlers包装成BatchEventProcessor对象，这里只有一个消费者处理器，所以只会循环一次
    //把handler包装成BatchEventProcessor对象
   for (int i = 0, eventHandlersLength = eventHandlers.length; i < eventHandlersLength; i++) {
       final EventHandler<? super T> eventHandler = eventHandlers[i];
       //创建BatchEventProcessor对象，每一个BatchEventProcessor对象都是一个消费者
       //BatchEventProcessor实现了EventProcessor接口，而EventProcessor接口继承了runnable接口
       //所以每一个EventProcessor对象就是一个Runnable任务，会交给每一个线程来执行
       //所以，每一个EventProcessor就对应一个线程，并且大家可以看到，每一个BatchEventProcessor
       //对象中都持有环形数组，序号屏障，还有eventHandler本身
       //注意，在创建好batchEventProcessor对象的时候，消费者的进度序号就已经初始化好了
       final BatchEventProcessor<T> batchEventProcessor = new BatchEventProcessor<>(ringBuffer, barrier, eventHandler);
       //判断异常处理器是否为空
       if (exceptionHandler != null) {   
           //不为空则设置异常处理器
           batchEventProcessor.setExceptionHandler(exceptionHandler);
       }
       //把创建好的batchEventProcessor对象添加到消费者仓库中，这个消费者仓库是一个很简单的类，这里就不做说明了
       //就是存放了程序中的所有消费者而已，当执行disruptor.start()代码启动程序时，其实是会从消费者仓库中启动每一个消费者线程
       consumerRepository.add(batchEventProcessor, eventHandler, barrier);
      
       //这时候每一个消费者的进度序号已经初始化好了，直接可以从batchEventProcessor对象中得到
       //然后就可以放到最上面定义的数组中了，在这里消费者进度数组才真正填充了数据
       processorSequences[i] = batchEventProcessor.getSequence();
   }
   
    //因为添加了新的消费者了，所以最慢消费者应该更新一下，新添加的消费者的消费进度肯定是最慢的
   //这里传进去了两个参数，有必要解释一下这两个参数
   //第二个参数很明显了，就是新添加进来的消费者的消费序号的数组，有了这些新添加进来的消费者的消费序号，才能知道现在
   //哪些消费者应该是消费进度比较慢的消费者
   //而第一个参数就是我之前跟大家简单解释了一下的数组，这里可以举一个简单的例子，比如说新添加进来5个消费者
   //这5个消费者必须要在另外3个消费者消费完事件之后，才能消费事件，以前那3个消费者可能是消费比较慢的
   //现在有了这个顺序消费的关系，新添加的5个消费者变成了消费最慢的消费者。这里传进去的第1个参数对应的
   //就是3个消费者，有了新的消费者添加进来了，这3个消费者就不再是最慢的消费者了，需要把它们的最慢进度删除

    //这个方法还有一个非常重要的作用，就是把上面创建好的消费者进度数组processorSequences交给序号生成器
    //该方法的具体逻辑就在下面
   updateGatingSequencesForNextInChain(barrierSequences, processorSequences);
   
    //EventHandlerGroup在第一版本还用不到，所以暂且先不讲解了，只要程序先不报错就行
   //其实也可以简单说一下，其实就是把这次添加进来的所有消费者handler当作一组了，所以自然要封装带一个group中
   return new EventHandlerGroup<>(this, consumerRepository, processorSequences);
}




/**
  * @author:B站UP主陈清风扬，从零带你写框架系列教程的作者，个人微信号：chenqingfengyangjj。
  * @Description:系列教程目前包括手写Netty，XXL-JOB，Spring，RocketMq，Javac，JVM等课程。
  * @Date:2023/8/19
  * @Description:这个方法就是更新最慢消费者序列的方法，该方法会在上面的方法中被调用，并且传进去两个参数
  */
 private void updateGatingSequencesForNextInChain(final Sequence[] barrierSequences, final Sequence[] processorSequences) {
     if (processorSequences.length > 0) {
         
         //把新添加的消费者的消费者进度，也就是消费者的进度序号全都添加到gatingSequences这个数组中
         //gatingSequences这个数组是AbstractSequencer类的成员变量
         //该数组中存储的就是每个消费者线程消费的进度，通过这个数组就可以找到消费最慢的线程的消费进度
         //这样给生产者分配序号的时候，把要分配的序号和每一个消费者的消费进度做对比
         //只要有比生产者序号小的，就不能分配生产者序号，这样才不至于将尚未被消费的数据覆盖
         //addGatingSequences(processorSequences)方法的逻辑就在下面
         ringBuffer.addGatingSequences(processorSequences);

         //在第一版本中只需要看到上面这个方法的逻辑即可，下面这些代码其实看不看都行
         
         //这里就是常规逻辑了，因为这些消费者成为了新添加的消费者的前置消费者，就可以不用观察这些消费者的消费进度了
         //只用观察最后消费的消费者进度即可，为什么这么说，比如说最后的消费者的消费进度是5，这就意味着前面的消费者肯定已经
         //把环形数组5号位置的数据消费了，所以，只用看最后的消费者即可
         for (final Sequence barrierSequence : barrierSequences) {
             ringBuffer.removeGatingSequence(barrierSequence);
         }
         
         //把这些消费者的最慢消费者身份更改掉，表示它不是最慢消费者了
         //说实话，这个框架中很多类名和方法名很难翻译，精通这个框架的人知道每个方法的作用是什么
         //但未必能讲清楚，大家还是要从文章中仔细品味，代码的注释只是起到一个辅助作用
         consumerRepository.unMarkEventProcessorsAsEndOfChain(barrierSequences);
     }
 }



//把新添加进来的消费者的消费序号添加到gatingSequences数组中
public void addGatingSequences(Sequence... gatingSequences) {
    //在这里，调用的就是序号生成器的方法了，把消费者的进度序号全部放到序号生成器的gatingSequences成员变量中
    sequencer.addGatingSequences(gatingSequences);
}


//把新添加进来的消费者的消费序号添加到gatingSequences数组中，这里的逻辑就来到AbstractSequencer类中了
@Override
public final void addGatingSequences(Sequence... gatingSequences) {
    //可以看见，这里使用到了SEQUENCE_UPDATER这个原子更新器来把数据添加到gatingSequences数组中
    //SEQUENCE_UPDATER原子更新器没忘吧，在展示AbstractSequencer类的代码的时候，我还特意跟大家强调过
    SequenceGroups.addSequences(this, SEQUENCE_UPDATER, this, gatingSequences);
}
```

到此为止，所有的逻辑就讲解完毕了。应该说，我用 1-7 章的篇幅为大家一步步搭建起了一个最简易的 Disruptor 框架，并且为大家深入分析了框架这样设计的原理，应该算是讲清楚了为什么，而不是直接告诉大家就这么做。明白为什么是非常有必要的，

我对学习框架有一套自己的逻辑和方法，学习的过程中弄清楚为什么，这个是最重要，要带着思考去看代码。有了这个思考的过程，知识才能真正变成自己的，更全面更严谨的思维来能建立成熟。比如学习了 Netty 的内存分配，让我自己设计一个内存分配系统，虽然 Netty 内存分配的代码我全忘了，但是我知道设计内存分配系统的时候要考虑什么了，比如我要先考虑是分配堆外内存还是堆内存，内存分配的规格是多大，要怎么减少外部内存碎片和内部内存碎片，内存分配中的并发问题怎么解决，怎么提高内存分配的效率，分配的内存该怎么管理，内存不够的时候怎么申请等等，这一系列问题都是我要考虑要解决的，并且这些问题的产生都不是盲目的，是经过我思考的，我知道一个完善的内存分配系统必须要处理好这些问题。只有知道了考虑的方向，才知道要解决什么问题，解决了这些问题，才知道自己设计的框架能达到什么水平，优化到什么水平。就能对自己开发的框架心里有数，当然也就有底气，更自信了。至于代码吗？无非就是实现的工具，有了设计思路，用代码实现也只是时间问题而已。当然，这只是我的一点小小感想，每个人都有自己的学习方式，我的也只是一家之言。

好了，话不多说了，这一章内容真的非常多，篇幅也很长，大家可以慢慢看，不明白的地方及时问我。我们下一章再见！