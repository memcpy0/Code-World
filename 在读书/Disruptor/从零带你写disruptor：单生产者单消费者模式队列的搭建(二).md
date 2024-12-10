大家好，在上一章的结尾，我苦于程序中存在太多的问题，迫切地希望给程序来一个翻天覆地的变革和重构。并且，我还为自己的队列找准了一个切入点，就是把 ArrayBlockingQueue 类中的 count 成员变量去掉。改为直接比较读写指针来判断我的队列是否可读可写。当然，我知道有的朋友可能会说，只是一个简单的 count 成员变量而已，费不着大动干戈，其实只要把 count 定义为 AtomicInteger 类型的，然后让它使用 getAndIncrement 方法自增就行了，用 CAS 方式解决并发问题。这确实是个好办法，但是，我相信我大刀阔斧重构之后的程序会非常完美，完全可以胜过 Doug Lea 的作品，所以，我宁愿按照自己的方式来重构程序。

那么，话说回来，为什么我会忽然产生这样一种想法，或者说我的编程思路怎么突然转变了呢？因为我终于意识到，避免并发问题的终极解决方法，也是最简单的方法，就是不让多个线程产生操作数据的交汇，不让它们操作相同的数据。这一点一旦明确了，很快我就发现我之前在 ArrayBlockingQueue 类中定义的 count 成员变量的唯一作用就是用来和容器数组的长度进行判断，然后决定队列目前是否可读或可写，然后再进一步决定生产者或消费者线程是否应该阻塞。事实上，这个 count 也只起到这个作用了，但就是只起到这个作用的 count 成员变量，带来了让人头疼的并发问题，必须用锁或原子类才能解决这一问题。但是，说到底，在单生产者和消费者模式下，它们存在的问题也只有速度跟得上和跟不上，不是消费者跟不上生产者，就是生产者跟不上消费者，只要能将消费的进度和生产的进度做一下对比，不就知道怎么回事了吗？

如果现在生产者的生产进度是 7，消费进度是 5，那就说明消费者还可以继续消费，对吧？这不就是直接拿队列的读写指针做对比吗？而 count 完全没有发挥作用。所以，我想 count 确实已经可以去掉了。那么，从此以后，就真的不再关心队列存储数据的个数了吗？也不完全是这样，接下来，我想回过头，为大家分析分析队列中的这个容器数组。

**引入环形数组概念**

在开始讲解之前呢，我还是先为大家把上一章的代码搬运过来，因为一会要对比着代码讲解，所以，大家可以先回顾一下上一章的代码。首先是 ArrayBlockingQueue 类。

```
public class ArrayBlockingQueue<E>{

    //存放数据的数组
    final Object[] items;
	//队列中存放数据的个数，这个成员变量要被去掉了
    //但这时候还没有删掉
    int count;

    //读指针
    int takeIndex;

    //写指针
    int putIndex;

    //阻塞策略,程序默认的
    WaitStrategy wait = new SleepingWaitStrategy();

    //队列的构造方法
    public ArrayBlockingQueue(int capacity) {
        //初始化数组
        this.items = new Object[capacity];
    }

    //队列的构造方法,如果用户设置了阻塞策略，就把阻塞策略替换成用户设置的
    public ArrayBlockingQueue(int capacity,WaitStrategy waitStrategy) {
        //初始化数组
        this.items = new Object[capacity];
        this.wait = waitStrategy;
    }


    public void put(E e) throws InterruptedException {
        //队列存放数据的个数等于容器数组容量，就意味着队列满了
         while(count == items.length) {
             //每过100纳秒线程就会自动醒过来，判断存放数据
             //的个数是否和数组容量相等，如果相等说明没有空间，仍然会睡一会
             wait.waitFor();
         }
         //走到这里就意味着线程可能没有阻塞
         //也可能是阻塞结束了，总之，不管线程有没有真的阻塞过
         //都要在这里重置一下SleepingWaitStrategy的retries属性
         wait.setRetries(200);
         //把数据放入队列中
         enqueue(e);
    }
    
    
    public E take() throws InterruptedException {
         while(count == 0) {
             wait.waitFor();
         }
        wait.setRetries(200);
        return dequeue();
    }

    //把数据添加到数组的方法，按顺序从前往后添加
    private void enqueue(E e) {
        //得到容器数组
        final Object[] items = this.items;
        //把数据放到写指针的索引位置
        items[putIndex] = e;
        //写指针加1，然后判断写指针是否等于容器长度
        if(++putIndex == items.length) {
            //等于就重置写指针为0
            putIndex = 0;
        }
        //容器存储数据的个数加1
        count++;
    }

    //把数据从队列容器中拿走的方法，按顺序从前往后拿走
    private E dequeue() {
        //得到容器数组
        final Object[] items = this.items;
        //根据读指针得到要取走的数据
        E e = (E) items[takeIndex];
        //把数组中的对应位置置为null
        items[takeIndex] = null;
        //判断读指针是否等于数组长度，如果等于数组长度，说明读到数组结尾了
        if(++takeIndex == items.length) {
            //重置读指针，可以从数组头部重新读取数据了
            takeIndex = 0;
        }
        //数组存储数据的个数减1
        count--;
        //返回要取走的数据
        return e;
    }
}
```

然后是阻塞策略。

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

   
    @Override
    public void waitFor(){
        
        //这意味着自旋次数是200，也就是线程刚要被阻塞之前
        //所谓自旋，就是让线程空转，就是下面这样，每空转1次，就让自旋次数减1
        if (retries > 100){
            --retries;
        }
        //如果自旋次数小于100，大于0了，说明已经自旋了很多次了
        //但还是不能继续向下工作，这时候尝试让该线程让出CPU
        else if (retries > 0){
            --retries;
            Thread.yield();
        }
        else{//走到这里意味着自旋次数到达200了，这时候就干脆让线程睡一会吧
            //睡的时间就是100纳秒，不能睡得太久，因为生产者可能随时发布新的数据
            LockSupport.parkNanos(sleepTimeNs);
        }
    }
}
```

接下来，就该重点分析队列中的容器数组了，请大家先看看下面的这幅简图。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/26725125/1694056726670-c9542b8f-8ade-418c-ba06-501a2979204a.jpeg)

在上面的简图中，我给出了一个长度为 6 的数组，还有两个指针，一个读指针，一个写指针。读指针指向数组的 1 号索引，写指针指向数组的 3 号索引。这就意味着当生产者线程要向队列中存放数据时，可以直接存放到 3 号索引的位置，而消费者线程读取数据时，可以直接从 1 号数据读取。对吧？因为我定义的是一个有界队列，所以数组的长度也是固定的，不会扩容。所以，现在，我就有一个问题了，当写指针的长度不断增加，当它指向 5 的时候，就意味着新存入的数据要放到 5 号索引的位置。那么，当它放完了之后呢？再有新的数据要存入该怎么办？这个问题也很熟悉了，上一章已经给出答案了，就是重置写指针为 0，从容器数组的头部重新按顺序存放数据。代码逻辑如下。

```
//把数据添加到数组的方法，按顺序从前往后添加
    private void enqueue(E e) {
        //得到容器数组
        final Object[] items = this.items;
        //把数据放到写指针的索引位置
        items[putIndex] = e;
        //写指针加1，然后判断写指针是否等于容器长度
        if(++putIndex == items.length) {
            //等于就重置写指针为0
            putIndex = 0;
        }
        //容器存储数据的个数加1
        count++;
    }
```

在判断了写指针的长度等于容器数组的长度后，就直接把写指针的长度重置为 0。代码逻辑已经很清晰了。既然是这样，那我想问问大家，这是不是就意味着我这个数组是需要循环利用的。只要写指针写完一遍，就可以从头继续写，我们可以称其为第二轮写入，这样的话，后面肯定还会有第三轮，第四轮，第 N 轮写入了。当然，这样一来也可能会发生一个问题，那就是当消费者速度特别慢的时候，写指针已经写完一遍数组了，有新的数据到来了，需要从头开始存放数据了，但是消费者还没开始消费呢，甚至连数组的首个数据还没消费呢，这时候如果要存放数据，不就把未消费的数据直接覆盖了吗？如果之后消费者开始消费数据，消费者就消费不到被覆盖的数据了，从而造成数据丢失。那么，在我目前的队列中，这个问题是怎么解决的呢？请看下面的代码块。

```
public void put(E e) throws InterruptedException {
    //队列存放数据的个数等于容器数组容量，就意味着队列满了
     while(count == items.length) {
         //每过100纳秒线程就会自动醒过来，判断存放数据
         //的个数是否和数组容量相等，如果相等说明没有空间，仍然会睡一会
         wait.waitFor();
     }
     wait.setRetries(200);
     //把数据放入队列中
     enqueue(e);
}
```

可以看到，在上面的 put 方法中，会使用容器数组存放数据的个数和数组长度做对比，因为每消费一个数据，容器存放数据的个数都会减一，当存放个数和容器数组长度一样时，就说明数组放满了。这时候想重新从头向尾存放数据，也没办法存放，因为数据没有消费，count 等于数组的长度。所以接下来生产者线程会阻塞。而队列的数据是先进先出的，只有当消费者开始消费，首先把数组首位数据消费了，将读指针指向数组的第二位。生产者才能继续工作，把数据放到数组首位。而读指针呢，会从数组的第二位继续向后消费，直到消费一轮结束了，又会被重置为 0，重新从数组首位开始消费，我们可以称它为第二轮消费。而第二轮写入的数据已经在此恭候多时了。如果这个逻辑大家都理解了，请大家仔细想想，我队列中的这个数组，其实就可以当作一个环形数组呀，反正数据填充完了，会从头再次填充，也会从头读取。就像下面这幅简图展示的这样。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/26725125/1694062129934-def20c07-775a-4632-97cb-514a398ef0f6.jpeg)

上面的简图中展示了一个环形数组，并且写指针指向 4 的位置，读指针指向 2 的位置。注意啊，这里我要强调一点，当前的读指针和写指针指向的都是应该操作的数据和位置，换句话说指向的都是下一位，都是 next。读指针指向 2，说明下一个可以消费的数据在容器数组的 2 号索引。写指针指向 4，说明下一个可以存放的位置在容器数组 4 号位置。这两个指针并不意味着当前的进度，而是下一个进度。这个要理清楚。当然，从整体上看，上面的这个环形数组和之前我给大家呈现的那幅简图中的线性数组其实么有本质区别，那么我说我自己定义的 ArrayBlockingQueue 类中的容器数组就是一个环形数组，谁赞成，谁反对？反正都有两个指针，都是可以进行循环写入和循环读取。如果是这样，我的队列又是一个有界队列，那么，只要我的队列的长度，也就是容器数组的长度一旦确定了，队列中能存放的最大数据的个数也就确定了，对吧？现在，我要跟大家说的是，知道这个最大存放数据的个数也就可以了。如果消费者速度很慢，生产者速度很快，消费者消费一个，生产者立刻就补充上去了，如果队列的容量是 32，大不了队列就一直存放 32 个数据呗。反正现在我们判断线程是否应该阻塞，也是直接根据读写指针的大小来判断。

那么，我们明确了环形数组这个概念，对我的队列有什么好处吗？如果这个问题是 Doug Lea 问的，我肯定就会跟他说：“Doug Lea，你等着嗷，看我给你亮个绝活！”明确这个概念当然是有用的，因为我已经厌烦了读写指针了，为什么如此厌烦呢？首先，我觉得读写指针太麻烦了，还得改来改去的，还得判断指针大小是不是等于数组长度。第二，读写指针是特么 Doug Lea 手写的 ArrayBlockingQueue 类中的成员变量，我不想再抄它的东西了！索性就把读写指针去掉吧。可是一旦去掉了读写指针，该怎么比较生产者和消费者的进度呢？怎么判断线程是应该阻塞还是唤醒呢？这个我已经设计好了，我就直接在程序中引入一个进度序号的概念，生产者持有一个进度序号，消费者也持有一个进度序号。生产者向队列中放入一个数据后，生产者序号就加 1，消费者每消费一个数据后，消费者序号也加 1。然后就可以用生产者和消费者序号直接做对比了。这时候，肯定会有人说，你这不是换汤不换药吗？抄袭就抄袭，换个名字，不对，换个马甲我就不知道你是抄的了吗？请诸位稍安勿躁，我的改造当然还没完呢。读写指针会自增，并且增长到和容器数组的长度一致时还要重置为 0，对吧？而我的这个进度序号可以一直自增，不管是生产者的进度序号，还是消费者的进度序号，永远不必重置为 0。可是这样一来，进度序号的大小和环形数组的容量一直了，该怎么继续存放数据呢？既然是环形数组，当然是从头开始存放，也就是进行第二轮写入。但这个时候存放位置的下标索引该怎么计算呢？

其实已经很明显了，我相信当大家看到环形数组的时候应该就猜到我的重构方向了。看到环形数组我就想到了时间轮，想到了时间轮我就想到了位运算确定数组索引下标。所以，知识是不是就此串联起来了。只要我们把环形数组的长度设置为 2 的 N 次幂，然后在类中定义一个计算数组下标的掩码，就是数组长度减一。这样，无限自增的序号就可以直接和这个掩码做与运算计算要存入的数组下标了，并且这个下标是一个循环。从头到尾结束之后，又会从头到尾来一遍，循环往复。(**看到这里，也许有人会说，呦，抄得还挺杂呀，又抄了一个时间轮的知识点。特娘的，说我抄袭，你怎么不让 Doug Lea 按照我这种方式改造它的队列呀。当然了，我是在开玩笑，但是大家应该也能意识到，编程思想和套路都是一样的，解决问题的手段也都差不多。什么环形数组，什么进度序号，我为大家引入的这些 disruptor 框架中的概念，说到底和 JDK 的 ArrayBlockingQueue 队列的设计方式也没什么本质区别。所以，大家不要被这些所谓的大名鼎鼎的框架迷惑了，看看源码就全都清楚了。即便是填充无效字节解决伪共享，JDK 的源码中也有这种技术，总之，JDK 是个宝库，大家可以认真学一学它的部分源码**)生产者存放数据的下标索引就可以计算出来了，消费者也可以根据自己的消费进度和掩码做位运算得到要消费的下标索引了。还有一点就是，我们也不必关心现在消费的是第几轮的数据，因为进度序号是可以无限递增的，假如说环形数组的长度是 8，生产者进度目前是 25，下一个要存放的进度是 26，消费者进度目前是 23，下一个要消费的是 24。你需要知道现在消费的是第几轮的数据吗？完全不需要呀，直接让进度和掩码做位运算，就能得到下标，然后把数据放在对应的下标位置，或者把数据从下标取出即可。我设计的这种模式本身就保证了消费者肯定把它当前消费进度之前的数据都消费完了。分析了这么多，当然还是要在代码层面展示实现一下才行。既然要开始修改代码了，不如我就彻底改造一下，我自己定义的类也不叫 ArrayBlockingQueue 了，我引入了环形数组概念，就把这个类定义为 RingBuffer 吧。请看下面重构后的代码。

```
public final class RingBuffer<E> {

    //用来进行位运算求取数组索引的掩码
    private final long indexMask;
    
    //存放数据的数组，可以抽象成环形数组
    private final Object[] entries;
    
    //环形数组的容量，必须是2的n次幂
    protected final int bufferSize;


    //构造方法
    RingBuffer(int bufferSize) {

        if (bufferSize < 1){
            throw new IllegalArgumentException("bufferSize must not be less than 1");
        }
        //Integer.bitCount方法的作用就是计算一个整数的二进制中有多少个1
        //单看这个好像没什么用，但是有一点大家一定要注意，那就是凡是2的n次幂的数，其二进制中1的个数永远是1
        //所以下面才会判断是不是等于1，如果不是1，说明环形数组的容量不是2的n次幂，那就无法使用长度-1做与运算的操作
        //计算相应的数组下标。其实从这里也能看到，一旦涉及到容量，在很多框架中容量为2的n次幂已经是一个标配了
        if (Integer.bitCount(bufferSize) != 1){
            //数组容量不符合要求就直接抛异常
            throw new IllegalArgumentException("bufferSize must be a power of 2");
        }
        //数组容量赋值
        this.bufferSize = bufferSize;
        //给掩码赋值
        this.indexMask = bufferSize - 1;
        //初始化数组
        this.entries = new Object[bufferSize];
    }


    //根据生产者的进度序号把数据放到数组对应的位置的方法
    //实际上就是发布数据的方法
    public void translateAndPublish(E object){
      long index = next();
       entries[index] = object;
    }

    //获得生产者下一个可用的进度序号，比如当前生产者的进度是25
    //下一个要存入的数据要放到26了，这个方法就会把26返回
    public long next(){
       //方法赞不实现，因为还不知道怎么获得生产者序号，更别说怎么获得下一个可用的生产者序号了
    }

    //其他方法暂时省略
    
}
```

在上面的代码块中，大家可以发现我重构后的代码似乎没有一点之前代码的影子了。先不说 RingBuffer 类还有很多不完善的地方，先来看看最大的变动吧。对 RingBuffer 类来说，把所有和消费者相关的方法都抽离了。队列中仅有的两个方法 translateAndPublish 和 next 都是和发布数据有关的，这就意味着它们是属于生产者的方法。和消费者有关的方法一个也没有。原因很简单，既然我要认真重构我的程序了，就要按照统一的风格，或者说使用通用的编程思想来编码。既然是统一的编程思想，大家可以想一想，消费者的消费逻辑一般都是由用户自己定义的吧？编程套路也很简单了，就是暴露一个接口出去，给用户自己实现消费逻辑。然后把这个接口交给消费者线程执行即可。当然，在执行之前，肯定要去环形数组中获得要消费的数据，对吧？现在，我把这些都当成消费者本身的工作，希望让消费者自己来干自己的活。所以，我就在环形数组中把生产者的方法和消费者的方法分割了。当然，还有另一个原因促使我这么做，那就是我的程序引入了进度序号这个概念，并且生产者和消费者各自持有自己的进度序号。消费者有了自己的消费进度，这就给了我一种感觉，好像它是一个立体的结构了，不再只是一个线程执行一个简单的动作。当消费者消费的时候，需要判断一些条件了，也可能需要借助一些成员变量的帮助，总之，意味着消费者需要包装一下了。当然，包装消费者还并不太急，在包装消费者之前，还有一个非常重要的问题需要解决，那就是进度序号该怎么定义。

**引入进度序号**

进度序号是我为自己的程序引入的一个概念，本质上和读写指针没什么区别，但是进度序号会无限自增，消费者和生产者各自只有自己的进度序号。那么，这个进度序号该怎么定义呢？用基本数据类型吗？如果要使用基本数据类型，那就最好别用 int 了，最好用 long。因为这个序号是要一直递增的，换个大点的 long，可以递增得更久。好了，如果真的要使用基本数据类型，就让我们仔细思考思考，基本数据类型是否合适。如果大家不知道思考的方向，就让我来简单提醒一下。想象一个场景，当消费者的进度赶上了生产者的进度时，消费者就不得不阻塞一会了。因为没有新的数据给它消费，只能阻塞等待新数据到来。而阻塞的过程中势必要用到上一章我定义的阻塞策略。并且我说过，我要继续重构我的阻塞策略，针对当前的场景，我的重构方向就是在阻塞策略中一直判断，生产者的进度是不是大于了消费者的进度。如果是使用基本数据类型，那么请大家看下面的代码块。

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


    //a是生产者进度，比如现在生产者的进度是25，该往第26个序号位置存放数据了
    //b是消费者进度，并且是将要消费的进度，也就是说b也消费到25了，该继续消费26的位置了
    //那这里传进来的b就是26，但是现在26没有数据
    //所以消费者线程要阻塞，但是一旦判断生产者的进度不小于消费者要消费的26进度了，就说明
    //有数据可以消费了
    @Override
    public void waitFor(long a,long b){

        //在这里得到默认的自旋次数
        int counter = retries;
        
    	//如果a小于b，就是生产者进度现在还小于消费者要消费的26进度
        //那就让消费者线程在循环中阻塞一会
        //一旦大于或者等于，就说明生产者进度加1了
        while (a < b){
           //在这里让线程等待，直到生产者发布了新的数据，其进度更新了
           //超过或者等于了消费者要消费的进度
            //这返回的counter很重要，是需要再下一次循环中再次传到applyWaitMethod方法中的
            counter = applyWaitMethod(counter);
        }         
    }

    //这是新添加的方法，在这个方法中，大家可以看到，retries成员变量的并发问题已经解决了
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

请大家仔细品味品味上面的代码块，看看在多消费者情况下 retries 成员变量的并发问题是怎么解决的。除此之外，就是我们要讨论的问题了。请大家想一想，现在的阻塞策略需要在 waitFor 方法的循环中，一直判断生产者的进度是否大于或者等于了消费者紧接着要消费的进度。逻辑十分简单，但是很遗憾，我可以很明确的告诉大家，这个 while 循环会永远无法打破了。我们都知道，生产者进度肯定是会一直发生变化的，只要生产者线程往队列中存放了数据，生产者进度序号就会加一。改变了之后，消费者线程就能跳出循环继续工作了吗？如果要跳出循环，那么阻塞策略就必须感知到生产者进度改变了，这样才能跳出循环。但是我传递给阻塞策略 waitFor 方法的两个参数都是基本数据类型，传过来值就确定了，不会再改变了，就算生产者进度改变了，阻塞策略的 waitFor 方法也感知不到呀。所以，肯定不会跳出循环，显然，也不能使用基本数据类型来定义进度序号。基本数据类型不能使用，那就用对象呀。对象传递的是引用，把生产者的序号对象传递给阻塞策略了，当生产者的序号对象改变了，持有对象引用的阻塞策略肯定是可以感知到的。所以，用对象准没错！

既然叫做进度序号，那我就定义一个新的类，叫做 Sequence。把序号进度包装在这个对象中，请看下面的代码块。

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

序号的对象定义好了，接下来请大家看一下重构之后的阻塞策略。

```
public final class SleepingWaitStrategy implements WaitStrategy{

    //其他的内容省略

    //因为消费者要消费的进度是不变的，消费者想消费第26个进度，就要判断一下第26个进度
    //是否可以消费，就要和当前的生产者进度做对比，所以消费者进度b仍然只传过来一个数值就行
    //就是调用消费者进度对象的get方法，然后加1，再把这个数值传到阻塞策略中就行了
    //b = sequence.get();sequence是消费者的进度对象
    //这里的a就是生产者进度对象的引用，直接把生产者进度对象的引用传过来，这样生产者对象中的值
    //发生了变化，阻塞策略就可以感知到了
    @Override
    public void waitFor(Sequence a,long b){

        //在这里得到默认的自旋次数
        int counter = retries;
        //在这里重构了一下
        while (a.get() < b){
           //在这里让线程等待，直到生产者发布了新的数据，其进度更新了
           //超过或者等于了消费者要消费的进度
            //这返回的counter很重要，是需要再下一次循环中再次传到applyWaitMethod方法中的
            counter = applyWaitMethod(counter);
        }         
    }
}
```

到此为止，我就为大家把序号对象讲解完毕了，也把阻塞策略重构完了。现在，我的程序已经引入了环形数组和序号对象，这其实就相当于程序已经实现了 ArrayBlockingQueue 类的容器数组和读写指针。只要我紧接着实现了生产者和消费者，就可以把这些组件拼接起来，程序就可以运行了。所以，接下来就要马不停蹄地实现消费者和生产者了。当然，还是不急，程序一步步迭代到现在，Sequence 也都实现了，消费者和生产者还会远吗？做人做事都要踏踏实实，一口吃不成胖子，急着赶进度并不会让你的代码变得多么优秀，只会让你忽视了程序中存在的瑕疵。彻底解决完每一个问题之后再赶路，这样会省很多麻烦。我说了这么多，大家肯定也看出来我另有所指了。是的，我想在 Sequence 定义好之后，带着大家回顾一下刚才实现的残次品环形数组 RingBuffer 类。因为我发现了可以优化它的地方。

**重构环形数组**

请大家把目光再次集中到环形数组身上，所谓环形数组，就是可以循环利用的数组，数组的容量是固定的，这也就意味着我的队列是一个有界队列。那么请大家思考这样一个场景，假如我的队列容量是 1024，能存放 1024 个数据。同时，我在程序中定义了一个类型，就叫 Request。其内部结构如下。

```
public class Request{

    private Integer id;

    private String name;

    public Request(){
        
    }

    public Request(Integer id,String name){
        this.id = id;
        this.name = name;
    }

    //get和set方法省略
}
```

如果我在程序中创建了 Request 类型的对象，并且就把这个对象存放到队列中，也就是说生产者生产的是 Request 对象，而这个对象中的 Integer 成员变量的值就是消费者线程要消费的数据。如果是这种情况，那么请大家想一想，当生产者要把一个 Request 对象放到队列中，也就是放到环形数组中时，是不是就意味着程序中要创建一个 Request 对象，而创建 Request 对象的同时，因为 Request 中定义了一个 Integer 对象，所以肯定还要创建一个 Integer 对象，再把该对象的引用赋值给 Request 对象中的 id 成员变量。总的说下来，就是每当生产者要向环形数组中存放一个数据时，就要创建两个对象，一个是 Request 对象，一个是 Integer 对象。而当这个数据被消费者消费了之后，按照我们之前设计 ArrayBlockingQueue 时的做法，这个数据就要从容器数组中移除了。如果环形数组也是这样设计的话，那么，就意味着 Request 对象被消费了之后，就要从环形数组中移除，数组指向该对象的引用也就被置为 null 了。接下来会发生什么，大家应该都明白了吧，失去了引用的对象肯定就会被垃圾回收呀，而且不仅 Request 对象会被回收，Integer 对象也要被回收。现在环形数组的长度是 1024，如果生产者要生产很多数据放到环形数组中，就会在程序中创建很多对象，这就意味着当数据被消费后，大量的对象都会被垃圾回收，如果消费者消费得速度很快，这些对象很可能是在一个集中的时间段内被垃圾回收。对程序的性能来说肯定是一种拖累。既然这样，为了改善程序，我们要做的就是尽可能减少垃圾回收的数量，很简单了，最直接的方法就是减少对象的创建。说得再直接一点，就是当生产者要把一个数据放到环形数组中时，要尽可能的少创建对象。

就拿我们的例子来说吧，当生产者要把一个数据放到环形数组中时，就要创建 Request 和 Integer 两个对象，那么，有没有什么方法少创建一个呢？当然有，而且很简单，发挥环形数组的优势就行了。反正这个数组是可以循环填充数据的，放完一轮又放一轮，而消费者真正消费的只是 Request 对象中的 Integer。既然这样，我为何不在环形数组初始化的时候，给数组的每一个位置都创建一个对应的 Request 对象，然后把对象的引用放到数组中呢？之后生产者向环形数组中添加数据，只要修改对应位置的 Request 对象中 Integer 对象的值就行了。环形数组的容量是 1024，这样一来在程序中就只会创建 1024 个 Request 对象，之后每次向环形数组存放数据，只会创建一个 Integer 对象，这不就减少了对象的创建吗？也就减少了垃圾回收的次数。这样一来，只要程序一直运行，环形数组的所有索引就会持有 1024 个 Request 对象的引用，Request 对象不会被垃圾回收，就可以一直复用了。因为消费者真正消费的是 Integer 对象，所以，每次生产数据时，只要修改 Request 对象中的 Integer 对象即可。当然，真正的生产者数据肯定是用户自己定义的。恰好环形数组还定义了泛型，用户可以自己任何想要的对象当作生产者数据，所以，逻辑就在此圆满了。逻辑已经分析得差不多了，接下来，我就要在代码层面实现一下了。没错，我将重构环形数组 RingBuffer 类。请看下面代码块。

首先引入一个特殊的接口，就是 EventFactory 接口。这个工厂就是专门用来生产生产者数据的，也就是要放到环形数组中的对象。

```
public interface EventFactory<T>{
    T newInstance();
}
```

再引入另一个特殊的接口。

```
//传输器，就是将生产者发布的真正数据，传输到对应的对象中
//在我们的例子中，生产者真正创建的是Integer对象，那么Integer对象就要
//借助这个传输器传到环形数组对应位置的Request对象中
//具体怎么使用下面会详细讲解
public interface EventTranslatorOneArg<T, A>{
    void translateTo(T event, long sequence, A arg0);
}
```

接下来就是重构后的环形数组了。

```
public final class RingBuffer<E> {

    //用来进行位运算求取数组索引的掩码
    private final long indexMask;
    
    //存放数据的数组，可以抽象成环形数组
    private final Object[] entries;
    
    //环形数组的容量，必须是2的n次幂
    protected final int bufferSize;


    //构造方法,这里把创建数据对象的工厂传进来了
    RingBuffer(int bufferSize,EventFactory<E> eventFactory,) {

        if (bufferSize < 1){
            throw new IllegalArgumentException("bufferSize must not be less than 1");
        }
        if (Integer.bitCount(bufferSize) != 1){
            //数组容量不符合要求就直接抛异常
            throw new IllegalArgumentException("bufferSize must be a power of 2");
        }
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

    
    //根据序号获得环形数组中对应的元素
    public E get(long sequence){
        return elementAt(sequence);
    }

    //该方法也很重要，就是根据序号得到数组相应位置的数据
    protected final E elementAt(long sequence) {
       return (E) entries[(int) (sequence& indexMask)];
    }

    //获得生产者下一个可用的进度序号，比如当前生产者的进度是25
    //下一个要存入的数据要放到26了，这个方法就会把26返回
    public long next(){
       //方法赞不实现，因为还不知道怎么获得生产者序号，更别说怎么获得下一个可用的生产者序号了
    }

    //其他方法暂时省略
    
}
```

到此为止 RingBuffer 类就重构完了。接下来，我来举一个具体的例子，为大家梳理一下当前生产者发布生产数据的具体流程。

**梳理本章程序的运行流程**

要运行程序，首先肯定要定义一个数据对象，这个数据对象就是要放在环形数组中的数据对象。并且，这个对象是需要用户自己来定义的。比如，我们就定义一个 Request 对象。请看下面代码块。

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

接下来就是定义一个数据对象工厂，也就是 EventFactory 接口的实现类。

```
public class SimpleEventFactory<T> implements EventFactory<Request<T>> {

    @Override
    public Request<T> newInstance() {
        return new Request<>();
    }
}
```

接下来就是测试类了。

```
public class Test {

    public static void main(String[] args) {

        //创建数据对象工厂对象，这个Integer，就是生产者真正要发布的数据
        SimpleEventFactory<Integer> eventFactory = new SimpleEventFactory<>();
        //定义环形数组的长度
        int ringBufferSize = 128;
        //创建环形数组
        RingBuffer buffer = new RingBuffer(ringBufferSize,eventFactory);
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
        //消费者就会直接去获取数据然后消费了
        for (int i = 0; i < 128; i++) {
             buffer.publishEvent(eventTranslatorOneArg,i);
        }
        
    }
}
```

测试类已经创建好了，接下来就该启动程序，分析程序流程了。大家请看，当程序执行到上面代码块的第 6 行和第 10 行，环形数组就会创建成功了，对象创建工厂和环形数组的长度都会传到环形数组 RingBuffer 类的构造函数中。然后程序就会来到环形数组 RingBuffer 类的构造函数中。请看下面代码块。

```
//构造方法,这里把创建数据对象的工厂传进来了
    RingBuffer(int bufferSize,EventFactory<E> eventFactory,) {

        if (bufferSize < 1){
            throw new IllegalArgumentException("bufferSize must not be less than 1");
        }
        if (Integer.bitCount(bufferSize) != 1){
            //数组容量不符合要求就直接抛异常
            throw new IllegalArgumentException("bufferSize must be a power of 2");
        }
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
```

在上面的构造方法中，RingBuffer 的掩码被赋值成功了，容量也赋值成功了。接着就会执行到 fill 方法。在该方法中，之前创建的对象工厂就会环形数组的每一个位置都创建一个 Request 对象，把引用放到数组的每一个索引位置。代码如下。

```
private void fill(EventFactory<E> eventFactory){
    for (int i = 0; i < bufferSize; i++){
        entries[i] = eventFactory.newInstance();
    }
}
```

到此为止，构造函数就执行完了，程序就又会来到 Test 测试类中，执行 12 到 19行的代码，创建一个传输器对象，然后就会执行测试类的 23 到 25 行的代码，将数据发布到环形数组中，当然，刚才创建的传输器和真正的生产者数据，都会当作方法参数传递给 RingBuffer 的 publishEvent 方法。这个时候，主线程其实就充当了生产者的作用。接着，程序就会再次来到 RingBuffer 中，来到它的 publishEvent 方法中。代码如下。

```

    public <A> void publishEvent(EventTranslatorOneArg<E, A> translator, A arg0){
        final long sequence = next();
        translateAndPublish(translator, sequence, arg0);
    }
```

在上面的代码块中，首先会通过 next 方法获得下一个可用的生产者序号，这个序号会和环形数组的掩码做位运算得到具体的数组下标，这个下标位置就是生产者数据要存放的位置。但是 next 方法我还没有实现，这里大家就当已经实现了吧，继续向下，程序就会执行 translateAndPublish 方法。代码如下。

```
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
```

在上面代码块的方法中，程序会通过 get(sequence) 方法得到环形数组对应位置的那个 Request 对象。而参数 arg0 就是真正的要存放到 Request 对象中的 Integer 对象。直接调用传输器 translator 的 translateTo 方法，就能完成数据的传输赋值了。具体代码如下。

```
 EventTranslatorOneArg<Request<Integer>, Integer> eventTranslatorOneArg =
                new EventTranslatorOneArg<Request<Integer>, Integer>() {
                    @Override
                    public void translateTo(Request<Integer> request, long sequence, Integer arg0) {
                        //在这里就把Integer对象设置到Request对象中了
                        request.setData(arg0);
                    }
                };
```

到此为止，优化之后的环形数组就为大家讲解完毕了。当然，这个环形数组 RingBuffer 类还没有真正实现，还有很多残缺的地方。但是生产者发布数据的流程，我可以负责任地告诉大家，流程已经讲得差不多了。所以，接下来似乎应该把目光放到消费者身上了。我想大家应该也已经迫不及待了，我自己也已经迫不及待了。当然，还是不急，在讲解消费者之前，让我们看看我目前的程序都有哪些组件了。

首先进度序号 Sequence 已经有了，环形数组 RingBuffer 也已经有了，这个组件也包含了生产者的部分功能，除此之外，我的程序中还有一个阻塞策略，当然，阻塞策略我最终重构完了，好像一直也没怎么用过了。说到阻塞策略，我就不得不提醒一下各位，在我们的程序中有一个缺陷一直没有完善，那就是当生产者跟不上消费者速度的时候，消费者就可以使用之前设计的阻塞策略直接阻塞，对吧？但是，假如消费者远远跟不上生产者的速度呢？可能生产者都填充完环形数组一轮了，要填充第二轮了，结果消费者连第一轮的前 10 个数据都还没有消费呢。这时候该怎么避免未消费的数据被覆盖呢？按道理说，直接让生产者阻塞就行了，问题是，现在的程序怎么判断生产者是否要阻塞呢？毕竟现在没有了 count 成员变量，程序不知道环形数组存放数据的个数，没了 count 和环形数组长度做比较，也就无法判断继续存放数据，会不会覆盖还未被消费的数据呀，对吧，这是一个问题，但我一直没跟大家提起。但是不要紧，最终都会解决的，总之，现在我们的程序已经有了 Sequence，RingBuffer，SleepingWaitStrategy 三个组件，接下来我就要把消费者引入进来。当消费者引入进来之后，我就可以具体去分析该怎么协调生产者和消费者的进度关系了。所谓协调，就是根据情况让生产者或消费者阻塞。这样分析起来，好像我们需要一个协调者，来协调生产者和消费者的关系。这个协调者也许是个对象，也许是个其他的什么组件。这个协调者最好能同时持有生产者和消费者的进度，然后协调二者，决定二者中的哪个应该适时阻塞。而阻塞的时候，就调用具体的阻塞策略就行了，这样看来，这个协调者似乎还要持有阻塞策略。好了，说得已经太多的，还是点到为止吧，大家把这章讲解的知识消化消化就行了。消费者就留到下一章讲解吧。我们下一章见。