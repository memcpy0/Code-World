学习本章之前，一定要重新下载一份最新的代码，我把网盘中的代码更新了一下，添加了几个类，修改了一些注释。

大家好，这一章我将为大家实现 raft 框架线性一致读的功能，这个功能也是一个成熟的 raft 框架必备的功能，也可以说是一个至关重要的功能。之前我们实现的所有功能基本上都是围绕着数据备份来展开的，当然，本门课程的前几章实现的是领导者选举的功能。总的来说，过去我们实现的所有功能，基本上都和处理客户端写指令有关，但是该怎么处理客户端写指令，这就是我们还未涉足的领域了。接下来，就请大家跟我一起看看，所谓的线性一致读究竟是什么，该怎么解释这个概念，又该怎么实现这个功能。

## 线性一致读的实现原理

在实现线性一致读功能之前，首先应该弄清楚什么是线性一致读，这个是肯定的。因为在使用 raft 框架的过程中，客户端除了向 raft 集群的领导者发送写指令，也就是向服务器写入数据之外，肯定也需要在某个时刻从服务器中把数据读取出来。让我来为大家举一个很直观的例子，假如我们要在生产环境使用一个 kv 数据库，并且要搭建这个 kv 数据库的集群，并且搭建的还是 raft 集群。也就是说，这个 kv 数据库集成了我们开发的 raft 共识算法框架。在这种情况下，当 kv 数据库的客户端向 raft 集群中的领导者发送了写指令，也就是 put 或者 set 指令，这条写指令就会被集群的领导者包装成一条日志，然后把日志复制给集群中的其他跟随者节点，当这条日志在集群中达成共识之后，这条写指令就会被领导者提交，然后应用到业务状态机上，在业务状态机中，这条指令写入的数据就会真正存放到 kv 数据库中。这就是一个完整的写指令在 raft 集群中被包装直到最终被执行的整个流程。但现在我们面临的是另一种情况，那就是 kv 数据库的客户端需要时不时地把数据从服务器读取出来，也就是向服务器发送 get 读指令，当然，这条读指令肯定也是发送给 raft 集群的，这种情况下 raft 集群要怎么处理这条读指令呢？也是由领导者处理这条读指令吗？这时候我们就要仔细分析分析了。

首先让我们来明确一个观点，当 kv 数据库的客户端向服务端发送写指令时，必须发送到 raft 集群的领导者节点，这是因为只有把指令发送到领导者，才能保证这条指令在集群中各个节点传输的顺序性，也就可以保证指令应用的顺序性，不至于出现数据在多个节点备份的过程中出现数据错乱的情况。可以说，把客户端的写指令直接发送给集群领导者，这是 raft 共识算法的强硬要求，写指令只能由领导者处理。那么，现在，轮到客户端发送写指令了，还有必要把写指令也发送给 raft 集群的领导者吗？我最初的观点很明确，肯定不用再发送给领导者啊。原因有三点：

**1 在一个 raft 集群中，如果让领导者既处理写指令又处理读指令，那么领导者将会面临很大的压力，本来处理客户端的写请求已经够累了，现在所有的读请求也要由领导者处理，超过领导者的处理能力之后，显然会降低集群对外提供服务的效率。**

**2 通过前面章节的学习，我们都已经知道了，集群中的跟随者节点也有数据呀，因为领导者会把自己的数据持续备份到跟随者节点，也就是说领导者存储了什么数据，最终跟随者节点也会有这些数据。所以，客户端直接把读请求发送给跟随者节点，也可以获得数据。**

**3 raft 集群中只有一个领导者，跟随者有很多个，每一个跟随者都备份了领导者的数据，也就意味着每一个跟随者都可以处理客户端的读请求，显然可以提高读请求的处理效率。**

所以在我总结了上面三个原因之后，我做出了一个大胆的决定，**那就是客户端向服务器发送读请求时，可以发送给集群中的任何一个节点，如果发送给了领导者，那么直接从领导者读取数据即可，如果发送给了跟随者节点，因为跟随者节点备份了领导者的数据，所以也可以直接从跟随者节点读取数据。这样一来，就相当于集群中的每一个节点都可以处理客户端的读请求了。当然，这么做有一个大前提，那就是节点状态必须是健康的，能正常对外提供服务的**。怎么样，我这个决定大胆吧？

如果真能像我分析得这样就好了，很快我就发现这条路根本行不通，我意识到我忽略了一个最重要的因素，**那就是领导者向跟随者备份数据并不总是及时的，或者说领导者向每一个跟随者节点备份数据并不总是同步的**。当客户端向领导者发送了多个写指令之后，这些写指令会被领导者包装成日志赋值给集群中的各个跟随者节点，这其实也就实现了数据的备份。但是，集群中存在多个节点，可能每个节点的网络环境都不相同，假如集群中有 5 个节点，领导者将日志传输给其他四个节点的时候，有的节点能及时接收到日志，有的节点却接收不到，需要领导者重新传输日志。最后可能出现这样一种情况，领导者明明已经把索引为 20 的日志提交了，也就是领导者的 lastCommittedIndex 为 20，但是节点 3 才复制到索引为 11 的日志。假如客户端直接向节点 3 发送读请求，显然不可能获得最新的数据信息。这并不是我们希望看到的情况，我希望的是，**当客户端向整个集群发送请求的时候，如果集群的领导者的 lastCommittedIndex 为 20，也就是意味着整个集群对前 20 条日志达成了共识，那么客户端在这一刻发出了请求，集群前 20 条日志对应的数据对这个客户端请求都是可见的。这就是所谓的线性一致读**。简单来说，集群中存储的数据可能是一直变化的，有可能客户端刚刚读取了一个数据，但紧接着这个数据就被改变了，所以我们不能以简单的过期和没有过期来对客户端读取的数据进行判别，如果是这样的话，集群存储的数据一直在更新，那么客户端先发送的请求读取到的可能都是过期数据。所以，**我们要保证的应该是在客户端向集群读取数据的那一刻，保证客户端读取到的数据是最新的就行了**。这就是线性一致读的本质，而要提供这个保证，就必须要使客户端知道集群中哪个节点的日志进度是最新的，**注意，我这里说的日志进度指的是整个集群达成共识的日志进度，因为只有达成共识的日志才意味着备份成功，数据不会丢失**。

假如说集群领导者的 lastCommittedIndex 为 20，集群中的另一个跟随者节点的 lastCommittedIndex 也为 20，这种情况下，客户端把读请求发送给领导者或者是另一个跟随者节点都可以，假如现在有另一个跟随者节点，比如就是节点 3，这个节点的 lastCommittedIndex 为 11，这时候客户端就只能把读请求发送给另外两个节点，而不能发送给节点 3，因为节点 3 的日志进度不是最新的，客户端无法在发送请求的这一刻读取到最新的数据。**这样分析下来，我发现处理客户端读请求的方式，似乎就是直接让客户端把读请求发送给领导者。原因也很简单：不管客户端怎么发送读请求，在哪个时刻发送读请求，正常情况下，集群中的领导者的日志进度肯定是最新的，所以只要从领导者直接读取数据，就可以读取到最新的**。兜兜转转，似乎又回到了原点，就像体育生沿着赤道跑了一圈。唉，没办法，只好继续从让客户端从领导者读取数据了。

如果大家理解了上面分析的流程，接下来请大家思考下一个问题，那就是当客户端直接从集群领导者读取消息的时候，要怎么读取呢？**如果换一种故弄玄虚的问法，那就是客户端真的可以从领导者读取到这一刻最新的数据吗**？要弄清楚这个问题，就要从领导者的 lastCommittedIndex 和 lastAppliedIndex 来入手了。经过前面章节的学习，这时候大家应该都很清楚了，lastCommittedIndex 就是已经被领导者提交的日志的索引，也就是整个集群中最新的被达成共识的日志索引；lastAppliedIndex 是领导者内部被应用的最新的日志索引。

那为什么突然提起这两个日志索引呢？请大家结合上一章状态机的内容想一想，当状态机在应用日志的时候，应用的肯定是已经达成共识的日志，也就是说，当一条日志被整个集群的节点达成共识了，才能被应用到状态机上。简单来说，一条日志肯定是要先被 commit，然后才能被 apply。 通过这一点我们可以准确地意识到：**在领导者内部，lastAppliedIndex 肯定是要小于等于 lastCommittedIndex，绝不对可能出现大于 lastCommittedIndex 的情况**。在第 7 版本代码中，状态机应用日志的流程也体现出这一点，在状态机应用日志的核心方法 doApplyTasks 方法中，就是先更新了 lastCommittedIndex 的值，然后再应用了一批日志，最后更新了 lastAppliedIndex。具体逻辑请看下面代码块。

```
public class FSMCallerImpl implements FSMCaller {


    //最新的被应用的日志的ID，初始化为0
    private final AtomicLong lastAppliedIndex;
    
    //最新的被提交的日志的ID，初始化为0
    private final AtomicLong lastCommittedIndex;

    
    //省略部分内容



    //根据日志索引，将对应的日志应用到状态机中的方法，committedInde是已提交的最新的日志的索引
    private void doCommitted(final long committedIndex) {


        //给最新提交的日志的索引赋值
        this.lastCommittedIndex.set(committedIndex);

        
        //省略应用日志的逻辑


         //更新lastAppliedIndex
        setLastApplied(lastIndex, lastTerm)
        
    }



    //日志被应用到状态机后，更新对应的成员变量的值，这个就不再添加注释了，很简单
    void setLastApplied(long lastIndex, final long lastTerm) {
        final LogId lastAppliedId = new LogId(lastIndex, lastTerm);
        this.lastAppliedIndex.set(lastIndex);
        this.lastAppliedTerm = lastTerm;
        this.logManager.setAppliedId(lastAppliedId);
    }
}
```

当然，除了这一点还有一个更明显操作，能够表明 lastCommittedIndex 是先于 lastAppliedIndex 更新的。那就是在 BallotBox 类中先更新了 lastCommittedIndex 的值。大家应该都清楚，BallotBox 投票箱对象是应用日志到状态机的入口，每当一批日志在集群中达成共识之后，就可以通过 BallotBox 的 commitAt 方法把这批日志应用到状态机上。而在 commitAt 方法中，就会先把 lastCommittedIndex 的值更新了。具体的代码我就不展示了，大家可以直接去第十版本代码的 BallotBox 类中查看。总之，我更大家说了这么多，**就是希望大家能够意识到在集群的任何一个节点中，通常来说，lastAppliedIndex 的值是要小于等于 lastCommittedIndex 的**。这就会给我们刚才达成的共识造成一点困扰。

请大家想一想，假如说客户端向集群的领导者发送了一条读请求，在发送这个请求的瞬间，集群内领导者的 lastCommittedIndex 是 20，也就是说已经有 20 条日志被达成共识了，那么对于这个客户端请求来说，这 20 条日志对应的数据都应该是可见的。但是，现在领导者的 lastAppliedIndex 是 10，出现这种情况也很容易理解。那就是领导者刚刚提交了索引为 11-20 的日志，把 lastCommittedIndex 更新为 20 了，但是还没有完全把这批日志应用到状态机上，所以 lastAppliedIndex 仍然是 10，等这批日志被应用了之后，lastAppliedIndex 也就会被更新为 20 了。那么，在这批日志还没被应用到状态机的情况下，领导者去处理这个客户端请求，显然也会读取到旧的数据，因为最新达成共识的日志索引为 20，这就意味着领导者要在这个基础上处理客户端请求，但实际情况是这 20 条日志中还有 10 条日志的没有应用到状态机上。假如要满足线性一致读，领导者必须等待自己的 lastAppliedIndex 赶上了 lastCommittedIndex，才能处理这个客户端请求。这个时候客户端请求读取到的肯定就是在发送请求的一瞬间最新的数据了。

如果让我简单总结一下，那么领导者处理客户端读请求的流程可以概括成这样：**当领导者接收到客户端的读请之后，会先查看自己的 lastCommittedIndex，然后再查看自己的 lastAppliedIndex，如果发现 lastAppliedIndex 小于 lastCommittedIndex，就必须等待 lastAppliedIndex 追上 lastCommittedIndex 之后，才能处理客户端读请求**。这就是线性一致读的实现原理。

大家可以仔细品味品味这个逻辑，如果这个逻辑理清楚了，接下来，就让我们换个角度，再次思考一下，有没有什么方法，能够让跟随者节点处理客户端请求，同时也满足线性一致读呢？请大家一定要理解，我不断地追求让跟随者节点处理客户端读请求，没有别的原因，就是不希望领导者"996"啊，我同情领导者，心疼领导者，虽然是机器，也不该被逼到 "996" 的地步！在我上高中的时候，我读了卡夫卡的变形记，算了，远了就不扯了，我怕扯下去，最后回忆起我小学第一次牵女孩子手的情景，还是回归正题吧。因为我发现，在线性一致读的定义下，让跟随者节点处理客户端请求也不是没可能的。原因也很简单：**现在的情况似乎还有点简化，那就是处理客户端请求的时候，只要知道集群中最新被应用的日志索引即可，也就是 lastCommittedIndex，这个只能从领导者那里获得。这很好办，当跟随者节点接收到一个客户端请求之后，立刻向领导者发送一个获取 lastCommittedIndex 的请求获得最新的 lastCommittedIndex 即可。这样跟随者节点就知道最新的被应用的日志索引，接下来就很简单了，只要这个跟随者节点的状态是健康的，网络环境也是健康的，这个跟随者节点总会应用日志，把自己的 lastAppliedIndex 更新为和获得的 lastCommittedIndex 一样的值。这个时候，跟随者节点就可以处理这个客户端请求了**。

如果让我简单总结一下，那么跟随者处理客户端读请求的流程可以概括成这样：**跟随者接收到客户端的读请求之后，会先向领导者发送一个请求，以便获得当前集群中最新的 lastCommittedIndex，然后只需要等待自己的 lastAppliedIndex 追上 lastCommittedIndex 的值，就可以处理客户端读请求了。**这样一来，也完全满足了线性一致读。可能有的跟随者节点比领导者处理读请求稍微慢点，但是在一个健康的集群中，网络环境健康的话，跟随者节点也可以做到实时复制领导者数据，这样一来，无非就是跟随者节点要向领导者发送一次请求获得 lastCommittedIndex 的值而已。

**在 sofajraft 中，默认实现的线性一致读的流程就是我刚才分析的这样，客户端既可以向领导者发送读请求，也可以向跟随者发送读请求。如果客户端的读请求发送给领导者了，那么领导者就会先查看自己的 lastCommittedIndex，然后再查看自己的 lastAppliedIndex，如果发现 lastAppliedIndex 小于 lastCommittedIndex，就必须等待 lastAppliedIndex 追上 lastCommittedIndex 之后，才能处理客户端读请求；如果客户端的读请求发送给跟随者节点了，那么跟随者就会先向领导者发送一个请求，以便获得当前集群中最新的 lastCommittedIndex，然后只需要等待自己的 lastAppliedIndex 追上 lastCommittedIndex 的值，就可以处理客户端读请求了**。

到此为止，我就为大家把线性一致读的实现原理分析完毕了，接下来我们就从代码层面上，亲自实现一下线性一致读功能。

  

## 实现线性一致读功能

理解了上一小节的线性一致读原理分析，实现这个功能就很简单。我们首先要做的就是定义一个方法，这个方法作为处理客户端请求的入口方法。所谓入口方法，就是当某一个客户端向集群中的某一个节点发送了一个读请求，这个节点改用什么方法来处理这个请求。这里我就直接把 sofajraft 框架源码中的方法复制过来了，在 sofajraft 框架中，处理客户端读请求的入口方法定义在了 NodeImpl 类中，就是 readIndex 方法。该方法具体实现如下所示，请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    //省略其他内容

    //封装了整个jraft框架需要的一些配置参数
    private RaftOptions raftOptions;

    //处理只读服务的组件
    private ReadOnlyService readOnlyService;

    //处理客户端读请求的方法
    @Override
    public void readIndex(final byte[] requestContext, final ReadIndexClosure done) {
        readIndex(this.raftOptions.getReadOnlyOptions(), requestContext, done);
    }


    @Override
    public void readIndex(ReadOnlyOption readOnlyOptions, byte[] requestContext, ReadIndexClosure done) {
        Requires.requireNonNull(done, "Null closure");
        //在这里调用只读服务处理读请求
        this.readOnlyService.addRequest(readOnlyOptions, requestContext, done);
    }
    
}
```

在上面的代码块中，大家肯定注意到了三个地方，第一就是第一个 readIndex 方法中的两个参数，一个是 requestContext，另一个是 ReadIndexClosure 类型的回调对象。这两个参数的作用我想先解释一下，第一个参数是请求上下文，这算是一个小小的数据扩展点，在节点处理客户端请求的时候，可以传递一些处理请求时用到的数据，这些数据会伴随整个请求处理的流程，可以随时获得；而 ReadIndexClosure 类型的回调对象，它的作用就比较重要了，**它封装的就是具体处理客户端请求以及向客户端回复响应的方法**，**这个回调对象中的方法会在客户端读请求被处理完毕后回调，处理客户端请求，然后回复给客户端读请求的响应，读取到的数据也会封装在响应中一起回复给客户端**。现在我跟大家解释这些方法参数的作用，大家可能一时间还无法理解，或者觉得有点抽象，这没关系，我会在文章的末尾给大家提供一一个源码中的测试例子，到时候大家就明白了。

接下来，让我来给大家简单解释一下上面代码块中第三个值得注意的地方，那就是 this.raftOptions.getReadOnlyOptions() 这行代码。在 safajraft 框架中，实际上还有一个 ReadOnlyOption.ReadOnlySafe 枚举类，这个枚举类如下所示，请看下面代码块。

```
//处理只读请求的枚举类
public enum ReadOnlyOption {


    //这个枚举对象对应的就是按照默认策略处理只读请求，也就是文章分析的线性一致读流程
    //该处理策略是sofajraft框架中默认的
    ReadOnlySafe,
    
    //这里使用的就是租约模式来处理读请求，租约模式我就不讲解了，大家感兴趣可以自己去了解一下
    //最常用的还上面这个策略
    ReadOnlyLeaseBased;


    //将枚举对象转换为protocol buffer通信类型的对象
    public static EnumOutter.ReadOnlyType convertMsgType(ReadOnlyOption option) {
        return ReadOnlyOption.ReadOnlyLeaseBased.equals(option) ? EnumOutter.ReadOnlyType.READ_ONLY_LEASE_BASED
                : EnumOutter.ReadOnlyType.READ_ONLY_SAFE;
    }



    //将protocol buffer通信类型的对象转换为枚举对象
    public static ReadOnlyOption valueOfWithDefault(EnumOutter.ReadOnlyType readOnlyType, ReadOnlyOption defaultOption) {
        if (readOnlyType == null) {
            return defaultOption;
        }
        return EnumOutter.ReadOnlyType.READ_ONLY_SAFE.equals(readOnlyType) ? ReadOnlyOption.ReadOnlySafe
                : ReadOnlyOption.ReadOnlyLeaseBased;
    }
}
```

this.raftOptions.getReadOnlyOptions() 这行代码得到的就是框架中处理客户端读请求的策略。这时候大家应该清楚第一个代码块中的逻辑了。当然，这时候我有必要再解释一句，因为我想大家可能会感到疑惑，既然是处理客户端请求，为什么不见 readIndex 方法参数中有客户端的请求对象呢？这是因为处理客户端请求是业务层的事情，**也就是 readIndex 方法的第二个回调对象参数中定义的方法中的逻辑**，这个业务层的组件是需要程序员自己实现的，这个组件需要持有状态机，处理请求的时候要从状态机中读取数据返回给客户端。我现在为大家展示的代码都是 raft 共识算法层的代码，简而言之，**现在我们要做的就是保证当前节点可以处理业务端请求，只要当前节点可以处理请求了，再去业务层执行处理请求的逻辑**。好了，现在让我们继续实现后面的代码吧。在第一个代码块中，我们可以看到当前节点最后调用了 this.readOnlyService.addRequest() 这个方法来处理只读请求。这时候就轮到 ReadOnlyService 这个组件登场了，我已经事先把这个组件定义在第一个代码块中了。这个组件也是在 NodeImpl 对象的 init 方法中创建并且初始化的，具体的代码我就不展示了，都是些老掉牙的知识，大家应该很熟悉了。接下来我想展示给大家的，是这个 ReadOnlyService 类本身的内容。

请大家想一想上一小节我为大家分析的流程，在上一小节，我跟大家说，当客户端发送给节点一个读请求时，节点需要知道集群中最新的 lastCommittedIndex，如果接收读请求的是跟随者节点，跟随者显然就要访问领导者，获得最新的 lastCommittedIndex。这个逻辑是没有问题的，但假如现在某一个跟随者节点不停地接收客户端读请求呢？难道跟随者每接收一个读请求就要访问一次领导者吗？可能这段时间领导者内部根本就没有日志产生，lastCommittedIndex 的值也没有更新，难道跟随者节点也要一直向领导者发送请求？这显然有点荒谬了。那该如何修复这个缺陷呢？我的做法非常直接，**就是参考前面章节处理日志的方式，比如说批量传输日志，批量将日志落盘，批量应用日志等方法，我们也可以批量处理客户端读请求**。

简而言之，**就是当跟随者接收到客户端的读请求之后，并不会立刻就在 raft 层面处理这个请求，除非这个在这个请求之后跟随者没有接收到其他的读请求了。跟随者要做的就是等待接收到的客户端读请求积攒到一定数量，然后再向领导则发送请求，以便获得 lastCommittedIndex 的值。这样一来跟随者节点访问领导者的次数会大大减少，并且推迟处理一些客户端请求，反而会让这些请求读取到更新的数据**。所以，这就是接下来我们编写代码的思路。当节点接收到客户端读请求之后，先不急着处理，而是引入我们的老朋友，那就是 Disruptor 组件，把和请求相关的对象全都存放到 disruptor 的环形数组中，让 Disruptor 的批处理器去批量消费。这样看来，给 ReadOnlyService 这个组件引入 Disruptor 组件是板上钉钉的事了。接下来，就请大家看看重构之后的 ReadOnlyService 类。请看下面代码块。

```
//提供只读服务的类，这里可以看到，ReadOnlyServiceImpl实现了一个LastAppliedLogIndexListener监听器接口
//为什么要把ReadOnlyServiceImpl定义成一个监听器呢？大家可以先思考思考
public class ReadOnlyServiceImpl implements ReadOnlyService, FSMCaller.LastAppliedLogIndexListener {


    //又看见了disruptor框架，这说明只读请求肯定也是放在环形队列中，使用批处理器一次处理很多读请求
    private Disruptor<ReadIndexEvent> readIndexDisruptor;
    
    private RingBuffer<ReadIndexEvent> readIndexQueue;

    private RaftOptions raftOptions;
    private NodeImpl node;
    private final Lock lock = new ReentrantLock();




    //该方法就是处理业务层要进行的读操作的方法，当NodeImpl对象调用了readIndex方法后
    //接着就会把和读请求有关的数据添加到disruptor的环形数组中
    @Override
    public void addRequest(final ReadOnlyOption readOnlyOptions, final byte[] reqCtx, final ReadIndexClosure closure) {
        if (this.shutdownLatch != null) {
            ThreadPoolsFactory.runClosureInThread(this.node.getGroupId(), closure, new Status(RaftError.EHOSTDOWN, "Was stopped"));
            throw new IllegalStateException("Service already shutdown.");
        }
        try {//定义一个disruptor框架要使用的数据传输器
            EventTranslator<ReadIndexEvent> translator = (event, sequence) -> {
                event.readOnlyOptions = readOnlyOptions;
                event.done = closure;
                event.requestContext = new Bytes(reqCtx);
                event.startTime = Utils.monotonicMs();
            };
            //下面就是把请求相关的信息提交到环形队列中的操作，这些代码在领导者提交日志给disruptor的时候已经见过了，就不再重复讲解了
            switch (this.node.getOptions().getApplyTaskMode()) {
                case Blocking:
                    this.readIndexQueue.publishEvent(translator);
                    break;
                case NonBlocking:
                default:
                    if (!this.readIndexQueue.tryPublishEvent(translator)) {
                        final String errorMsg = "Node is busy, has too many read-index requests, queue is full and bufferSize=" + this.readIndexQueue.getBufferSize();
                        ThreadPoolsFactory.runClosureInThread(this.node.getGroupId(), closure,
                                new Status(RaftError.EBUSY, errorMsg));
                        this.nodeMetrics.recordTimes("read-index-overload-times", 1);
                        LOG.warn("Node {} ReadOnlyServiceImpl readIndexQueue is overload.", this.node.getNodeId());
                        if (closure == null) {
                            throw new OverloadException(errorMsg);
                        }
                    }
                    break;
            }
        } catch (final Exception e) {
            ThreadPoolsFactory.runClosureInThread(this.node.getGroupId(), closure
                    , new Status(RaftError.EPERM, "Node is down."));
        }
    }
    
    


    //Disruptor的消费工厂，专门用来创建存放到环形数组中的待消费的对象的
    private static class ReadIndexEventFactory implements EventFactory<ReadIndexEvent> {

        @Override
        public ReadIndexEvent newInstance() {
            return new ReadIndexEvent();
        }
    }



    //disruptor的消费处理器，这个处理器中的方法会在Disruptor的批处理器中被不断执行
    private class ReadIndexEventHandler implements EventHandler<ReadIndexEvent> {


        //定义一个集合，集合中存放从环形数组中获得的要小费的数据，等到集合中的数据达到了限制，就批量处理集合中的所有消费数据
        private final List<ReadIndexEvent> events = new ArrayList<>(ReadOnlyServiceImpl.this.raftOptions.getApplyBatch());

        @Override
        public void onEvent(final ReadIndexEvent newEvent, final long sequence, final boolean endOfBatch)
                throws Exception {
            //把从环形数组中取出来的消费数据放到集合中
            this.events.add(newEvent);
            //判断集合中的数据是否超过32条，或者当前消费数据是环形数组中的最后一个
            //这时候就会进入分支，执行处理消费数据的方法，也就开始处理读请求了
            if (this.events.size() >= ReadOnlyServiceImpl.this.raftOptions.getApplyBatch() || endOfBatch) {
                //在这里进一步处理读请求
                executeReadIndexEvents(this.events);
                reset();
            }
        }

        private void reset() {
            for (final ReadIndexEvent event : this.events) {
                event.reset();
            }
            this.events.clear();
        }
    }
    


    //该方法内调用了两次handleReadIndex方法，但是两次方法传递的参数不一致，一个传递的是只读策略，一个传递的是租约策略
    //这是考虑到发送过来的请求采用的擦略不同，所以处理两次，这样一来，不管是哪种策略的读请求都可以被处理到了
    //当然，这里调用两次方法并不意味着所有请求都会被处理两次，传递只读策略的handleReadIndex方法只会处理只读策略的请求
    //而传递租约策略的handleReadIndex方法只会处理租约策略的请求
    private void executeReadIndexEvents(final List<ReadIndexEvent> events) {
        if (events.isEmpty()) {
            return;
        }//这个就是真正处理读请求的方法
        handleReadIndex(ReadOnlyOption.ReadOnlySafe, events);
        handleReadIndex(ReadOnlyOption.ReadOnlyLeaseBased, events);
    }

    

    //处理客户端读操作的方法
    private void handleReadIndex(final ReadOnlyOption option, final List<ReadIndexEvent> events) {
        //在sofajraft框架中，只要是读操作，都要交给领导者处理一下，因为假如是跟随者节点，跟随者节点可不知道自己应用的日志是不是最新的
        //只有向领导者发送一个请求询问一下，领导者把自己提交的最新日志索引告诉跟随者节点，跟随者将自己状态机应用的最新日志索引
        //和领导者的对比一下，这样一来跟随者就知道自己是否应用到最新日志了，如果应用到了，那么跟随者就可以直接处理读操作了
        //如果还没有应用到领导者状态机的日志索引，跟随者节点就要等一等，等到应用到了再通知业务层执行读操作
        //现在就要创建一个ReadIndexRequest请求构建器，这个ReadIndexRequest请求就是要发送给领导者询问领导者最新应提交的日志索引
        //当然，如果当前节点就是领导者也会象征性的发送一个请求，其实就是直接调用NodeImpl对象的handleReadIndexRequest处理这个请求即可
        final RpcRequests.ReadIndexRequest.Builder rb = RpcRequests.ReadIndexRequest.newBuilder()
                .setGroupId(this.node.getGroupId())
                .setServerId(this.node.getServerId().toString())
                .setReadOnlyOptions(ReadOnlyOption.convertMsgType(option));
        //在这里根据ReadOnlyOption中的处理读请求的策略过滤请求，把和option相同的请求策略的请求封装到一个ReadIndexState对象中
        //最后返回一个存放了很多ReadIndexState对象的集合，这个集合会被封装到ReadIndexResponseClosure回调对象中
        //再处理领导者回复的ReadIndexRequest请求的响应时会被用到
        final List<ReadIndexState> states = events.stream()
                .filter(it -> option.equals(it.readOnlyOptions))
                .map(it -> {
                    rb.addEntries(ZeroByteStringHelper.wrap(it.requestContext.get()));
                    return new ReadIndexState(it.requestContext, it.done, it.startTime);
                }).collect(Collectors.toList());

        //判断上面得到的集合非空
        if (states.isEmpty()) {
            return;
        }
        //在这里创建了ReadIndexRequest请求
        final RpcRequests.ReadIndexRequest request = rb.build();
        //在这里把请求交给NodeImpl对象的handleReadIndexRequest方法来处理了，并且定义了一个回调对象ReadIndexResponseClosure
        //该对象中的run方法会在接收到ReadIndexResponse响应后被回调
        //在NodeImpl对象的handleReadIndexRequest方法中会判断当前节点是否为领导者节点
        //如果是领导者那么直接处理请求即可，如果是跟随者，跟随者就会把这个请求发送给领导者，让领导者处理
        this.node.handleReadIndexRequest(request, new ReadIndexResponseClosure(states, request));
    }


}
```

上面代码块中的方法虽然看起来很多，但大部分代码大家都已经熟悉得不能再熟悉了，在之前得其他组件中都见过很多次了。现在，我来给大家简单梳理一下上面代码块中程序地执行流程。当程序在 NodeImpl 对象中调用了 readIndex 方法之后，程序就会来到 ReadOnlyServiceImpl 类中，执行 ReadOnlyServiceImpl 对象的 addRequest 方法，把待处理的请求存放到 Disruptor 的环形数组队列中。当环形数组存在数据之后，Disruptor 的批处理器就会开始工作，把环形数组中的数据取出来，这时候，程序执行的就是 ReadIndexEventHandler 内部类中的 onEvent 方法。在 onEvent 方法中会判断从环形数组中取出的数据是否达到了上限，或者环形数组中已经没有请求了，只要符合其中任何一种情况，程序就会执行 executeReadIndexEvents 方法，然后调用 handleReadIndex 方法。在 handleReadIndex 方法中，就会构建一个 ReadIndexRequest 请求，这个请求就是用来发送给领导者的请求，领导者接收到这个请求之后，就会把自己最新的 lastCommittedIndex 回复给跟随者节点。

当然，假如当前接受客户端读请求的节点就是领导者，那么就会直接在自己内部判断了，看看 lastAppliedIndex 是否追上了 lastCommittedIndex。针对领导者和跟随者不同操作的细分，这个逻辑就要靠 handleReadIndexRequest 方法来实现了。也就是上面 handleReadIndex 方法中的最后一行代码，可以看到，最后调用了 NodeImpl 对象的 handleReadIndexRequest 方法来处理定义好的 ReadIndexRequest 请求，并且向方法传入了一个 ReadIndexResponseClosure 回调对象，当节点接收到 ReadIndexRequest 请求的响应后，这个回调对象中的方法就会被回调了。具体的逻辑我们先不必关心，接下来，让我们来看看 NodeImpl 对象的 handleReadIndexRequest 方法如何实现。

根据之前分析的逻辑，接下来的流程显然就该分析当前节点是跟随者还是领导者了，如果是领导者，那么就让 NodeImpl 自己处理这个 ReadIndexRequest 请求即可，如果是跟随者，就要把这个请求发送给领导者。所以，handleReadIndexRequest 方法可以定义成下面这样。请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    //省略其他内容

    //封装了整个jraft框架需要的一些配置参数
    private RaftOptions raftOptions;

    //处理只读服务的组件
    private ReadOnlyService readOnlyService;

    //处理客户端读请求的方法
    @Override
    public void readIndex(final byte[] requestContext, final ReadIndexClosure done) {
        readIndex(this.raftOptions.getReadOnlyOptions(), requestContext, done);
    }


    @Override
    public void readIndex(ReadOnlyOption readOnlyOptions, byte[] requestContext, ReadIndexClosure done) {
        Requires.requireNonNull(done, "Null closure");
        //在这里调用只读服务处理读请求
        this.readOnlyService.addRequest(readOnlyOptions, requestContext, done);
    }


     //这个就是节点真正处理读请求的方法
    @Override
    public void handleReadIndexRequest(final RpcRequests.ReadIndexRequest request, final RpcResponseClosure<RpcRequests.ReadIndexResponse> done) {
       
        this.readLock.lock();
        try {
            //判断当前节点状态
            switch (this.state) {
                case STATE_LEADER:
                    //走到这里意味着是领导者，那么直接处理读请求即可，处理完毕了回调done对象中的方法即可
                    readLeader(request, RpcRequests.ReadIndexResponse.newBuilder(), done);
                    break;
                case STATE_FOLLOWER:
                    //走到这里就意味着当前节点是跟随者节点，那就要给领导者发送一个ReadIndexRequest请求，让领导者回复最新的lastCommittedIndex
                    //而当前方法就是领导者处理ReadIndexRequest请求的方法，所以跟随者节点向领导者发送了ReadIndexRequest请求之后
                    //就会被领导者的NodeImpl对象的handleReadIndexRequest方法处理
                    readFollower(request, done);
                    break;
                case STATE_TRANSFERRING:
                    //如果当前节点正在转移领导权，那就回复一个繁忙的状态
                    done.run(new Status(RaftError.EBUSY, "Is transferring leadership."));
                    break;
                default:
                    //走到这里就意味着当前节点状态无效或者非法
                    done.run(new Status(RaftError.EPERM, "Invalid state for readIndex: %s.", this.state));
                    break;
            }
        } finally {
            this.readLock.unlock();
        }
    }


    //如果这个方法被调用了，就意味着当前节点是跟随者节点，跟随者节点无法判断自己能否处理当前请求，所以需要向领导者确认一下才行
    //于是就会向领导者发送一个ReadIndexRequest请求，让领导者返回lastCommittedIndex
    private void readFollower(final RpcRequests.ReadIndexRequest request, final RpcResponseClosure<RpcRequests.ReadIndexResponse> closure) {
        if (this.leaderId == null || this.leaderId.isEmpty()) {
            closure.run(new Status(RaftError.EPERM, "No leader at term %d.", this.currTerm));
            return;
        }
        //在这里发送了读请求给领导者
        final RpcRequests.ReadIndexRequest newRequest = RpcRequests.ReadIndexRequest.newBuilder()
                .mergeFrom(request)
                .setPeerId(this.leaderId.toString())
                .build();
        this.rpcService.readIndex(this.leaderId.getEndpoint(), newRequest, -1, closure);
    }

    
}
```

在上面的代码块展示出来之后，接下来的内容就变得简单多了，因为我们可以把程序的执行流程分为两种情况了。假如当前节点是跟随者节点，那么程序就会执行到上面代码块 handleReadIndexRequest 方法中的 readFollower 方法，在这个 readFollower 方法中，跟随者节点会把这个 ReadIndexRequest 请求发送给领导者，领导者会回复给跟随者节点 lastCommittedIndex。**领导者接收到跟随者发送过来的 ReadIndexRequest 请求之后，正是使用 NodeImpl 对象的 handleReadIndexRequest 方法来处理这个 ReadIndexRequest 请求的。而当接收到客户端读请求的节点是领导者时，领导者也是调用 NodeImpl 对象的 handleReadIndexRequest 方法来处理从 ReadOnlyServiceImpl 组件中传递过来的 ReadIndexRequest 请求的**。这样一来，情况确实就变得简单很多了，我们只需要分析清楚领导者处理 ReadIndexRequest 请求的逻辑，这一章的核心内容就算是讲解完毕了。所以，现在我们的注意力就不得不集中在 handleReadIndexRequest 方法中的 readLeader 方法上。

那么，当领导者节点执行 readLeader 方法的时候，首先应该考虑什么呢？我的想法很简单，那就是要先判断一下整个集群中是否只有一个节点，假如只有一个节点，那这个节点肯定就是领导者，领导者就可以直接处理这个请求了。具体的代码实现请看下面代码块。

```
public class NodeImpl implements Node, RaftServerService {

    //省略其他内容



    //领导者处理读请求的方法，如果该方法被调用了，说明当前节点就是领导者，有可能是领导者正在处理客户端的读请求
    //也有可能是客户端恰好就把读请求发送给领导者了，因为领导者中的数据都是最新的，状态机上应用的日志也都是最新的
    //所以领导者正在直接处理这个读请求，然后把lastCommittedIndex返回给跟随者节点
    private void readLeader(final RpcRequests.ReadIndexRequest request, final RpcRequests.ReadIndexResponse.Builder respBuilder,
                            final RpcResponseClosure<RpcRequests.ReadIndexResponse> closure) {
        //获得集群中超过一半的节点的数量
        final int quorum = getQuorum();
        if (quorum <= 1) {
            //走到这里意味着集群中只有一个节点，直接返回成功响应即可
            //注意，这里成功返回成功响应
            respBuilder.setSuccess(true)
                    //这里的操作最重要，把节点本身应用的最新日志索引返回
                    .setIndex(this.ballotBox.getLastCommittedIndex());
            closure.setResponse(respBuilder.build());
            //执行回调方法，注意，这里的这个回调方法一旦执行，逻辑就会重新回到ReadOnlyServiceImpl组件中
            //因为这个回调方法是从ReadOnlyServiceImpl对象的handleReadIndex方法中传递过来的
            //在ReadOnlyServiceImpl对象中执行的这个回调方法，就会通知业务层获取数据，上面设置的最新日志的索引也会在这个回调方法中用到
            closure.run(Status.OK());
            return;
        }

        //下面就是集群中存在多个节点的逻辑
    }
}
```

上面的判断很简单，就是看看集群中是否有一个节点，如果只有一个，那么直接就把领导者的 lastCommittedIndex 设置到响应中，然后把响应交给回调对象，最后执行回调对象中的回调方法，**程序就会重新来到 ReadOnlyServiceImpl 对象中，执行 ReadOnlyServiceImpl 对象中的回调方法，也就是刚才我跟大家提到过，让大家先不必关心的 ReadIndexResponseClosure 对象中的方法**。

分析完了这种情况，就该分析另一种情况了，那就是集群中有多个节点的情况。其实这种情况和刚才讲解的情况没有什么本质的不同，都是把领导者的 lastCommittedIndex 设置到响应中，只不过如果领导者处理的是跟随者节点发送过来的请求，需要把响应回复给跟随者，**然后跟随者节点再执行** **ReadOnlyServiceImpl 组件中 ReadIndexResponseClosure 对象中的方法**。如果是领导者自己接收到了客户端的读请求，那和刚才的逻辑一样，也是把 lastCommittedIndex 设置到响应中，然后回调 ReadIndexResponseClosure 中的方法即可。看到这里，大家肯定会觉得困惑，既然逻辑都一模一样，为什么还要把领导者处理读请求的操作分为两种情况呢？为什么要以集群中的节点数量来划分呢？这里就需要解释一下原因了，当集群中只有一个节点的时候，当然直接让这一个节点处理读请求即可，但假如集群中有多个节点，需要领导者来乾纲独断的时候，就必须要保证集群中的领导者是唯一的，并且可以正常工作。**所以，假如集群中有多个节点的时候，领导者必须向集群中的各个节点发送一次心跳请求，如果成功接收超过集群半数节点的响应了，才能保证当前的领导者可以乾纲独断**。之后的逻辑和上面代码块中展示的逻辑就没什么区别了。所以，处理集群中存在多个节点的流程代码我就不向大家展示了，无非就是比之前的多了一个心跳消息的发送，具体代码大家可以去我提供的第十版本代码中查看。接下来，让我们把注意力再移回到 ReadOnlyServiceImpl 对象中。

不管是领导者还是跟随者，只要 ReadIndexResponseClosure 中定义的方法被回调了，就说明 ReadOnlyServiceImpl 只读服务对象已经知道了当前集群中最新的 lastCommittedIndex 的值，这个时候就可以具体判断一下当前节点的 lastAppliedIndex 是否赶上了 lastCommittedIndex。**如果赶上了，那么 ReadOnlyServiceImpl 对象就可以直接通知业务层执行业务逻辑，处理读请求，也就是从状态机中获得数据，然后返回给客户端即可**。假如 lastAppliedIndex 没有赶上 lastCommittedIndex，那么当前节点就需要等待 lastAppliedIndex 赶上 lastCommittedIndex，才能通知业务层处理读请求。这时候大家可以想一想，**当前节点要怎么等待 lastAppliedIndex 赶上 lastCommittedIndex 呢？当前节点怎么就知道 lastAppliedIndex 赶上 lastCommittedIndex 了呢？大家可别忘了，lastAppliedIndex 可是在状态机组件中更新的。所以，肯定是状态机组件更新了 lastAppliedIndex，然后通知 ReadOnlyServiceImpl 对象，看看 lastAppliedIndex 是否赶上 lastCommittedIndex**。具体的逻辑我就不展开了，也不在文章中展示了，代码本身并不多，而且代码中的逻辑非常详细，这些细节就留给大家去第十版本代码中学习吧。当然，除了这两种情况之外，还有最后一种情况，那就是当前接收客户端请求的节点的 lastAppliedIndex 和集群中最新的 lastCommittedIndex 非常大，以至于当前节点要追赶很久才能处理客户端读请求，这个时候就可以直接回复给客户端一个失败响应，让它找别的节点发送读请求即可。接下来，我就为大家展示一下 ReadIndexResponseClosure 中回调方法的具体实现，至于其他更多的细节，就留给大家去我提供的代码中学习吧。请看下面代码块。

```
public class ReadOnlyServiceImpl implements ReadOnlyService, FSMCaller.LastAppliedLogIndexListener {


     private final TreeMap<Long, List<ReadIndexStatus>> pendingNotifyStatus = new TreeMap<>();

    //省略部分内容


    //该内部类中的方法会在当前节点接收到ReadIndexResponse响应后被回调
    class ReadIndexResponseClosure extends RpcResponseClosureAdapter<RpcRequests.ReadIndexResponse> {

        //这个ReadIndexState对象中封装了每一个请求的一些相关数据
        final List<ReadIndexState> states;
        //读请求本身
        final RpcRequests.ReadIndexRequest request;

        //构造方法
        public ReadIndexResponseClosure(final List<ReadIndexState> states, final RpcRequests.ReadIndexRequest request) {
            super();
            this.states = states;
            this.request = request;
        }



        @Override
        public void run(final Status status) {
            //判断读请求的响应是否成功，响应状态是失败的，就直接通知业务层读操作失败了
            if (!status.isOk()) {
                //通知业务层读操作失败
                notifyFail(status);
                return;
            }
            //得到只读请求的响应对象
            final RpcRequests.ReadIndexResponse readIndexResponse = getResponse();
            //判断响应本身是否成功
            if (!readIndexResponse.getSuccess()) {
                //响应失败则通知业务层操作失败
                notifyFail(new Status(-1, "Fail to run ReadIndex task, maybe the leader stepped down."));
                return;
            }
            //走到这里意味着响应是成功的，创建一个ReadIndexStatus对象，并且把领导者回复的最新应用的日志索引设置到ReadIndexStatus对象中
            //很快就会用到
            final ReadIndexStatus readIndexStatus = new ReadIndexStatus(this.states, this.request,
                    readIndexResponse.getIndex());
            for (final ReadIndexState state : this.states) {
                //遍历states集合，这个集合中存放着从客户端发送过来的请求的信息
                state.setIndex(readIndexResponse.getIndex());
            }
            boolean doUnlock = true;
            ReadOnlyServiceImpl.this.lock.lock();
            
            //下面就是我刚才为大家分析的三种情况
            try {
                //判断当前节点的状态机应用的日志索引是否大于等于领导者回复的日志索引
                //如果大于等于，说明当前节点已经应用了最新日志，就可以处理读请求了
                if (readIndexStatus.isApplied(ReadOnlyServiceImpl.this.fsmCaller.getLastAppliedIndex())) {
                    ReadOnlyServiceImpl.this.lock.unlock();
                    doUnlock = false;
                    //这里就可以通知业务层执行读操作了
                    notifySuccess(readIndexStatus);
                } else {
                    //如果当前节点状态及应用的日志索引比领导者小，说明当前节点的状态还不是最新的，并且这两个索引的差距非常大
                    //这时候就直接通知业务层读操作失败，因为等待当前节点应用到领导者日志索引可能要等很久，就直接回复失败响应了
                    if (readIndexStatus.isOverMaxReadIndexLag(ReadOnlyServiceImpl.this.fsmCaller.getLastAppliedIndex(), ReadOnlyServiceImpl.this.raftOptions.getMaxReadIndexLag())) {
                        ReadOnlyServiceImpl.this.lock.unlock();
                        doUnlock = false;
                        notifyFail(new Status(-1, "Fail to run ReadIndex task, the gap of current node's apply index between leader's commit index over maxReadIndexLag"));
                    } else  {
                        //走到这里就意味着当前节点状态及应用的日志索引比领导者小，但是有没有小很多
                        //这时候就可以把等待被执行的读操作的请求信息放到pendingNotifyStatus这个map中
                        //其中key就是读操作等待的索引，一旦当前节点应用到这个索引了，就可以通过注册在当前节点状态机上的监听方法通知业务层可以执行读操作了
                        //现在大家应该明白了本类的onApplied方法的作用了吧
                        //注意，这里添加的value是一个list，也就是说该日志索引对应的所有读操作都会被放到这个集合中，然后再放到map中
                        ReadOnlyServiceImpl.this.pendingNotifyStatus
                                .computeIfAbsent(readIndexStatus.getIndex(), k -> new ArrayList<>(10))
                                .add(readIndexStatus);
                    }
                }
            } finally {
                if (doUnlock) {
                    ReadOnlyServiceImpl.this.lock.unlock();
                }
            }
        }
}
```

好了，到此为止，我就为大家把线性一致读的知识讲解完毕了。大家可以结合多看看代码，其实本章并没有多少内容，而且都很简单，只不过定义了一些回调方法，大家看的时候可以多看几遍，理清楚各个回调方法先后被调用的顺序。好了，在文章的最后，我给大家提供一个 sofajraft 框架源码中的例子，让大家看看读请求究竟是怎么被处理了。请看下面代码块。

```
//这个类和业务层以及raft曾都有交互，应该交给程序员自己来定义
public class AtomicRangeGroup {

 final static Logger LOG = LoggerFactory.getLogger(AtomicRangeGroup.class);

    private final RaftGroupService raftGroupService;
    private final Node node;
    //可以看到，这里持有了程序员自己定义的业务状态机
    private final AtomicStateMachine fsm;

    private final long minSlot;                                                   
    private final long maxSlot;                                                    
    private final AtomicInteger requestId = new AtomicInteger(0);

    public long getMinSlot() {
        return this.minSlot;
    }

    public long getMaxSlot() {
        return this.maxSlot;
    }

    public AtomicRangeGroup(String dataPath, String groupId, PeerId serverId, long minSlot, long maxSlot,
                            NodeOptions nodeOptions, RpcServer rpcServer) throws IOException {
        // Init file path
        FileUtils.forceMkdir(new File(dataPath));
        this.minSlot = minSlot;
        this.maxSlot = maxSlot;

        // Init statemachine
        this.fsm = new AtomicStateMachine();

        // Set statemachine to bootstrap options
        nodeOptions.setFsm(this.fsm);
        nodeOptions.setEnableMetrics(true);
        nodeOptions.getRaftOptions().setReplicatorPipeline(true);
        nodeOptions.getRaftOptions().setSync(true);
        nodeOptions.getRaftOptions().setReadOnlyOptions(ReadOnlyOption.ReadOnlySafe);

        // Set the data path
        // Log, required
        nodeOptions.setLogUri(dataPath + File.separator + "log");
        // Metadata, required
        nodeOptions.setRaftMetaUri(dataPath + File.separator + "raft_meta");
        // Snapshot, not required, but recommend
        nodeOptions.setSnapshotUri(dataPath + File.separator + "snapshot");
        // Init raft group service framework
        this.raftGroupService = new RaftGroupService(groupId, serverId, nodeOptions, rpcServer);
        // Startup node
        this.node = this.raftGroupService.start();

        final ConsoleReporter reporter = ConsoleReporter.forRegistry(node.getNodeMetrics().getMetricRegistry())
            .convertRatesTo(TimeUnit.SECONDS).convertDurationsTo(TimeUnit.MILLISECONDS).build();
        reporter.start(60, TimeUnit.SECONDS);

    }

    //这个就是处理客户端读请求的方法
    public void readFromQuorum(final String key, RpcContext asyncContext) {
        final byte[] reqContext = new byte[4];
        //在这里设置了请求上下文，其实就是一个请求ID
        Bits.putInt(reqContext, 0, requestId.incrementAndGet());
        //在这里调用NodeImpl的readIndex方法，开始在raft处理读请求
        this.node.readIndex(reqContext, new ReadIndexClosure() {

            @Override
            public void run(Status status, long index, byte[] reqCtx) {
                if (status.isOk()) {
                    try {//如果在raft层处理请求成功，就意味着当前节点可以处理客户端读请求
                        //直接从状态机中得到对应的value，然后返回给客户端
                        asyncContext.sendResponse(new ValueCommand(fsm.getValue(key)));
                    } catch (final KeyNotFoundException e) {
                        asyncContext.sendResponse(GetCommandProcessor.createKeyNotFoundResponse());
                    }
                } else {
                    asyncContext.sendResponse(new BooleanCommand(false, status.getErrorMsg()));
                }
            }
        });
    }

    public AtomicStateMachine getFsm() {
        return this.fsm;
    }

    public Node getNode() {
        return this.node;
    }

    public RaftGroupService RaftGroupService() {
        return this.raftGroupService;
    }

    /**
     * Redirect request to new leader
     * @return
     */
    public BooleanCommand redirect() {
        final BooleanCommand response = new BooleanCommand();
        response.setSuccess(false);
        response.setErrorMsg("Not leader");
        if (node != null) {
            final PeerId leader = node.getLeaderId();
            if (leader != null) {
                response.setRedirect(leader.toString());
            }
        }

        return response;
    }

    public static AtomicRangeGroup start(StartupConf conf, RpcServer rpcServer) throws IOException {

        final NodeOptions nodeOptions = new NodeOptions();
        // Set election timeout to 1 second
        nodeOptions.setElectionTimeoutMs(1000);
        // Close cli service
        nodeOptions.setDisableCli(false);
        // A snapshot saving would be triggered every 30 seconds
        // nodeOptions.setSnapshotIntervalSecs(30);
        // Parsing Options
        final PeerId serverId = new PeerId();
        if (!serverId.parse(conf.getServerAddress())) {
            throw new IllegalArgumentException("Fail to parse serverId:" + conf.getServerAddress());
        }
        final Configuration initConf = new Configuration();
        if (!initConf.parse(conf.getConf())) {
            throw new IllegalArgumentException("Fail to parse initConf:" + conf.getConf());
        }
        // Set the initial cluster configuration
        nodeOptions.setInitialConf(initConf);
        // Startup node
        final AtomicRangeGroup node = new AtomicRangeGroup(conf.getDataPath(), conf.getGroupId(), serverId,
            conf.getMinSlot(), conf.getMaxSlot(), nodeOptions, rpcServer);
        LOG.info("Started range node[{}-{}] at port:{}", conf.getMinSlot(), conf.getMaxSlot(), node.getNode()
            .getNodeId().getPeerId().getPort());
        return node;
    }
}
```

好了朋友们，到此为止本章就终于结束了。接下来我还会为大家更新快照、配置变更、优雅停机的内容。这些都是很简单的知识，其中只有快照生成和安装的流程稍微复杂一些，但是逻辑非常清晰，下一章大家就知道了。诸位，我们下一章见。