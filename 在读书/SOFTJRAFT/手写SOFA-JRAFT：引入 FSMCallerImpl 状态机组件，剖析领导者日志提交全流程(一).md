大家好，这一章我将为我们的程序引入状态机组件。众所周知，状态机是 raft 共识算法框架中非常重要的一个组件。也许有的朋友对状态机这个名词的概念还有些陌生，不知道什么是状态机，所以我想先为大家简单解释一下状态的本质。过去我们的文章都是围绕着 raft 共识算法来讲解的，**这个 raft 共识算法要求集群中只能有一个领导者，并且客户端只能和领导者打交道。当领导者接收到客户端的一条指令时，应该把这条指令包装成一条日志，然后把这条日志传输给集群中的其他节点，当领导者接收到超过集群半数节点的成功响应后，就可以认为这条日志包装的指令在集群各个节点之间达成了共识，然后提交这条日志即可**。注意，这里有一个很重要的概念，**那就是日志包装的指令在集群的各个节点之间达成了共识，这个其实就是 raft 共识算法的核心所在，raft 共识算法能做的只是让某条日志在集群的各个节点之间达成共识，并不负责其他的功能，那么达成共识之后呢**？这条已经被达成共识的指令该怎么应用呢？

在我们真正开发中，不管是业务开发还是框架开发，假如要研发一个分布式 kv 数据库，当数据库以 raft 集群模式开始对外提供服务的时候——也就是说我们开发的这个分布式 kv 内部集成了 raft 共识算法框架——如果用户向集群的领导者发送了一条指令，比如就是 map.put(name,"奔波儿灞")，这条指令肯定会先按照 raft 共识算法的流程在各个节点之间达成共识，然后才会被应用。如果这条指令要被应用，毫无疑问，肯定是应用在 kv 数据库中，也就是会在身份为领导者的 kv 数据库服务器上执行 map.put(name,"奔波儿灞") 这行代码。**这时候我们就可以说身份为领导者的 kv 数据库服务器的状态发生了改变，因为它存放了一条新数据，如果集群中的其他节点也想和领导者状态保持一致，就要尽快应用刚才达成共识的那条指令**。由此可见，**一旦我们说到某个节点的状态改变了，实际上就是这个节点的业务层有了变化，存储新的数据或者删除数据，或者是更新数据，总之是节点的业务层发生了变化**。**所谓业务层就是 kv 数据库这个层面的一切功能，对数据库的增删改查都属于业务层的活动**。而状态机就是 raft 共识算法层专门暴露给业务层的一个接口，或者说就是 raft 共识算法层暴露给 kv 数据库的一个接口，这个接口的实现类可以由业务程序开发者自己定义。**当客户端的某条指令先发送给 raft 集群的领导者节点之后，这条指令会先在节点的 raft 算法层经过一系列的流程，最终在集群的各个节点之间达成共识，领导者要应用这条指令的时候，就会把指令传递给业务程序员自己定义的状态机实现类中，在状态机实现类中把指令应用了。这个状态机实现类，就是状态机本身，因为通过这个类应用客户端指令，就可以改变节点的状态，从这个角度理解状态机这个概念也许更容易**。

好了，朋友们，可以看到，这一章的前言有些长，**总结下来就是一条指令要想最终被应用到节点上，会现在 raft 算法层转一圈，等这条指令被集群各个节点达成共识了，就会交给业务层去应用，而具体应用指令的工具就是这个状态机**。现在只是文字分析出了一个指令被应用的大概流程，知识点完全没有展开讲解呢，功能也没有实现，接下来，就要进入正题，开始为大家认真分析流程和实现功能了。我希望在我分析的时候，刚才总结的指令被应用的大概流程能对大家理解各种细节知识有一点帮助。

## 引入 BallotBox 投票箱，实现日志计数功能

在学习完前十八章后，大家肯定对领导者向跟随者节点复制日志的功能了如指掌了。领导者使用 Pipeline 模式向跟随者节点批量传输日志，然后处理跟随者节点回复给领导者的响应。我在上一章已经为大家把这个流程实现了，当然，我知道实现的并不完善，比如说我在实现这个功能的时候，只是把 onRpcReturned 方法使用 Pipeline 模式简单重构了一下，至于这个方法中的 onAppendEntriesReturned 方法，我并没有为大家实现，而是让大家自己去第六版本代码中查看。我不知道大家在学习本章的时候有没有把第六版本代码看完了，如果看完了大家也许已经知道在第六版本代码中，onAppendEntriesReturned 方法进行了怎样的重构，这样一来学习本章就会更加容易；如果没有看完也没关系，因为接下来我将从这个 onAppendEntriesReturned 方法入手，为大家引出本章的内容。至于要讲解 onAppendEntriesReturned 方法的原因也很简单，因为跟随者节点回复给领导者的日志请求响应只有在 onAppendEntriesReturned 方法中才会被真正处理了，而这个真正处理的流程我在之前并没有为大家认真分析过。所以，现在我希望把这个缺口填补上。

大家都知道，onAppendEntriesReturned 方法既可以处理探针请求响应也可以处理日志请求响应，这是因为不管领导者是向跟随者节点发送探针请求还是发送日志请求，都会使用 AppendEntriesRequest 请求体来传输消息或数据，并且都会向 AppendEntriesRequest 请求体中封装一些公共数据，而且不管是探针请求还是日志请求，在跟随者节点内部，都会使用 handleAppendEntriesRequest 方法来处理，因为 handleAppendEntriesRequest 方法就是专门用来处理 AppendEntriesRequest 请求的。这就造成了一种局面，**跟随者节点在使用 handleAppendEntriesRequest 方法在处理探针请求和日志请求时，会执行一些公共的操作来处理这两个请求，也就是说这两个请求有很多相同的处理流程**。当跟随者节点处理完请求之后，不管是处理探针请求还是日志请求，最后都会回复给领导者节点一个 AppendEntriesResponse 响应，并且在这个响应中都会封装相同的信息。比如都会封装处理请求的节点的当前任期，最后一条日志的索引，响应是否成功的标志等等。因为，当领导者收到跟随者回复的 AppendEntriesResponse 响应后，不管这个响应是来自探针请求还是日志请求，都会被领导者的 onAppendEntriesReturned 方法处理。这也就意味着**，在领导者处理这两个响应时，也会执行一些公共操作**。所以我们在分析 onAppendEntriesReturned 方法的工作流程时，需要把对两个响应的处理都考虑到。

比如，当 onAppendEntriesReturned 方法在处理响应的时候，**可以先判断响应状态是否为失败，这就是一个公共操作，因为不管是探针请求响应还是日志请求响应，都封装者响应的状态**。**如果响应状态是失败，那就可以再继续判断失败的原因是什么，如果是跟随者节点处理日志超过负载了，这时候处理的就是日志请求的响应，那么执行相应的操作即可；如果是因为响应中的任期大于领导者的任期了，这就是个公共的操作，也执行相应操作即可**。我当然不会再把 onAppendEntriesReturned 方法的代码全部展示到文章中了，因为这个方法很长，内容很多，我一次次地展示代码，只会使文章冗长枯燥，大家在文章中看代码的效果也不是很好。我之所以在这里为大家这样分析，是希望大家在亲自阅读 onAppendEntriesReturned 方法的时候，能结合注释分析出该方法中每一个操作的用意，处理的是哪个响应，或者说这个操作是公共的。**当然，接下来还可以继续分析，看看是不是响应中的日志索引和复制器对象中的 nextIndex 不匹配，如果不匹配说明当前处理的就是探针请求，并且要发送的下一条日志索引没有匹配成功。以上都是响应不成功的状态，响应成功的状态就很简单了，如果是探针请求的响应成功了，那就直接更新 nextIndex 的值即可，如果是日志请求的响应成功了，也是直接更新 nextIndex 的值，计算下一批要发送的日志的起始索引即可**。这些流程应该也挺容易理解的，如果用伪代码来表示，可以写成下面这样，请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
@ThreadSafe
public class Replicator implements ThreadId.OnError {

    //省略其他内容


     private static boolean onAppendEntriesReturned(final ThreadId id, final Inflight inflight, final Status status,final RpcRequests.AppendEntriesRequest request,final RpcRequests.AppendEntriesResponse response, final long rpcSendTime,final long startTimeMs, final Replicator r) {


         //响应失败，这里处理的响应和日志传输的请求有关
         //处理的是响应内容失败，而不是响应本身有问题
         //这个判断就是一个公共操作，对探针请求响应和日志请求响应都生效的操作
        if (!response.getSuccess()) {

             //响应的状态码是EBUSY，这种情况就是跟随者节点是繁忙状态，所谓繁忙状态就是日志处理不过来，日志超负载了
            //这个判断是针对日志请求响应的
            if(response.getErrorResponse().getErrorCode() == RaftError.EBUSY.getNumber()) {

                //具体处理省略，可以去第六或者第七版本代码中查看
            }

            //这里仍然是在响应失败的分支中呢，这时候判断一下响应中的跟随者节点的任期是否比当前领导者大
            //如果大显然就是发生过网络分区，这时候就要让当前领导者进行身份降级
            //这个判断就是一个公共操作
            if (response.getTerm() > r.options.getTerm()) {

                 //具体处理省略，可以去第六或者第七版本代码中查看
            }


            //下面的判断是针对日志请求响应的，大家应该还记得，这个方法在发送探针消息和复制日志请求，收到响应后都会被回调
            //判断跟随者的要接收的下一条日志是否比领导者要发送的下一条日志索引小
            //如果小则意味着跟随者节点进度落后
            if (response.getLastLogIndex() + 1 < r.nextIndex) {
                LOG.debug("LastLogIndex at peer={} is {}", r.options.getPeerId(), response.getLastLogIndex());
                //这里就把下一条要发送的日志索引设置为跟随者节点的，
                r.nextIndex = response.getLastLogIndex() + 1;
            } else {
                //走到这里意味着跟随者的下一条要接接收的日志索引比领导者还大
                if (r.nextIndex > 1) {
                    LOG.debug("logIndex={} dismatch", r.nextIndex);
                    //这个要从跟随者的handleAppendEntriesRequest方法中查看一下，是怎么根据领导者要发送的下一条日志的前一条日志的索引
                    //从自己的日志组件中获得对应日志的，如果获取不到，其实就是获取到了，但是跟随者节点的日志的任期比领导者还小
                    //这时候需要让领导者的要发送的下一条日志递减，直到可以跟跟随者匹配到相同任期的日志，然后让领导者开始传输复制即可
                    //之前跟随者的日志比较大可能是因为旧的领导者发送的，比如整个集群就发送给了这一个节点，但是还没有提交，还没来得及提交旧的领导者就宕机了
                    r.nextIndex--;
                } else {
                    LOG.error("Peer={} declares that log at index=0 doesn't match, which is not supposed to happen",
                            r.options.getPeerId());
                }
            }

            //这里发送的就是探针消息，发送这个是为了开启下一次发送日志的操作
            r.sendProbeRequest();
            return false;
        }



         //走到这里，就意味着不管是探针请求响应还是日志请求响应，都成功回复了
         //下面就是公共操作了
         
         //得到这次请求批量发送的日志的数量
        final int entriesSize = request.getEntriesCount();

         //请大家想一想，这里是不是少了什么操作呢？？？
         
       
        //更新接下来要发送的日志的索引，然后会跳出该方法，来到外层onRpcReturned方法，在finally中继续发送日志给跟随者节点
        r.nextIndex += entriesSize;
        return true;
         
     }


    //省略其他内容
}
```

上面代码块中呈现的代码，就是领导者处理探针请求响应或者日志请求响应的大概流程，当然，我并没有展示全部代码，大家理解这个流程即可。现在我的问题是，看到上面的代码块，大家有没有觉得缺少了哪些操作呢？在上面代码块的第 68 行，我还特意写了一行注释，提醒大家也许在上面的方法中缺少了一些必要的操作。请大家想一想，假如跟随者节点回复给领导者的是一个日志请求的响应，比如说领导者向跟随者传输了一批索引为 1-10 的日志，现在领导者接收到这批日志持久化成功的响应了，除了更新复制器对象的 nextIndex 的值，还应该执行什么操作，**按照 raft 共识算法的要求，当一批日志被持久化成功之后，领导者其实就可以判断这批日志是否可以被提交了，对吧**？但是在上面的方法中，显然没有展示出这个操作。

就在上面代码块的第 68 行，程序执行到那里的时候就已经表明接收到的响应肯定是成功的，**如果通过 request.getEntriesCount() 这个方法得到的这个请求发送的日志数量大于 0，说明当前处理的响应就是一个日志请求响应，显然就可以执行日志提交的操作了。领导者一旦提交了日志，就可以把最新被应用的日志索引传输给各个跟随者节点，跟随者节点也就可以应用这条日志了**。当然，这时候肯定需要更加严谨**，因为日志包装的指令被应用的前提是这条日志在集群的各个节点中达成一致，而达成一致的具体表现就是领导者接收到超过半数节点回复的成功响应，现在我们只是在领导者的一个 Replicator 复制器对象中接收到的了日志请求成功的响应，也就意味着这只是集群中的一个跟随者节点向领导者回复了成功响应，仅凭这一个节点的成功响应，还不足以直接让领导者应用这条日志。正确的操作显然是将领导者内部所有的 Replicator 对象接收的日志请求的成功响应收集起来，只有这些响应超过集群节点过半的数量，领导者才能应用对应的日志**。

那在代码中应该怎么实现呢？**怎么把领导者内部的所有 Replicator 对象接收到的成功响应收集起来，并且判断成功响应数量超过了集群过半节点呢**？这个实现起来其实还比较简单，经过上面的分析，我们都已经知道了，只要程序能执行到上面代码块的第 68 行，就意味着当前 Replicator 复制器对象接收到的跟随者节点的响应一定是成功的，**所以只要在上面代码块的第 68 行，把这个成功响应收集起来就行，领导者中的 Replicator 复制器对象都执行这个操作，最终就会把领导者接收到的关于某批日志的成功响应收集起来，然后判断成功响应的数量是否超过了集群半数节点即可**。如果用伪代码来表示，可以写成下面这样，请看下面代码块。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
@ThreadSafe
public class Replicator implements ThreadId.OnError {

    //省略其他内容


     private static boolean onAppendEntriesReturned(final ThreadId id, final Inflight inflight, final Status status,final RpcRequests.AppendEntriesRequest request,final RpcRequests.AppendEntriesResponse response, final long rpcSendTime,final long startTimeMs, final Replicator r) {

         //省略部分内容

         
         //走到这里，就意味着不管是探针请求响应还是日志请求响应，都成功回复了
         //下面就是公共操作了
         
         //得到这次请求批量发送的日志的数量
        final int entriesSize = request.getEntriesCount();

        //在这里就可以把日志请求的成功响应收集起来
        //如果数量大于0，就说明这次的请求肯定是日志复制的请求，并且也成功了
        if (entriesSize > 0) {    
           //在这里把成功响应收集起来，然后判断是否可以成功提交日志了
        }
         
       
        //更新接下来要发送的日志的索引，然后会跳出该方法，来到外层onRpcReturned方法，在finally中继续发送日志给跟随者节点
        r.nextIndex += entriesSize;
        return true;
         
     }


    //省略其他内容
}
```

理解了上面代码块中的逻辑之后，现在又有一个新的问题，我一直在跟大家说把成功的响应收集起来，那么这个响应要怎么收集起来呢？收集到哪里呢？或者说把响应收集起来，难道真的就是把领导者接收到的每一个 AppendEntriesResponse 响应对象收集起来吗？这就是我们目前面临的问题。面对这个问题，大家也不必觉得太难，因为我们并不是毫无头绪，而是有前例可循的。请大家回顾一下之前的知识，当集群中的某个节点开始执行领导者选举的操作时，也会收集接收到的成功响应，然后计算票数，如果获得的票数超过了集群过半节点，那么这个节点就可以当选领导者。当时我们使用了一个投票计数器来计算票数，也就是 Ballot 对象。而所谓的收集成功响应，也并不是真的收集响应对象，而是每接收到一个成功响应，就在 Ballot 对象中记上一票，当获得的票数大于集群中一般的节点之后就可以成功当选领导者。现在，为了帮助大家回顾一下曾经的知识，我把这个 Ballot 类的代码搬运到下面，请看下面代码块。

```
//计算投票结果的类，整个类的原理实际上是做减法，就是得到集群中所有参与选举的节点
//然后计算出最少需要的票数，每得到一票就从需要得到的票数中减一，直到减为0，就说明获得足够的票数了
//比如说一个集群中有5个节点，那么当前节点至少需要3票就可以成功当选领导者了，所以没得到1票就让3减1，直到结果为0，就说明可以成功当选了
public class Ballot {

    //该内部类对象是用来表示peerId在集合中未知的，方便快速查找
    public static final class PosHint {
        //这个成员变量表示节点在peers集合中的位置
        //初始化为-1
        int pos0 = -1;
        //如果一个节点是旧配置中的，那么就用下面这个成员变量代表
        //其在oldPeers中的位置
        int pos1 = -1;
    }

    //这个内部类的对象会封装一个个节点，并且给这个节点添加一个未找到的标记
    //然后把这个节点存放到集合中，如果节点是属于当前配置中的就存放奥peers集合中
    //否则存放到oldPeers集合中
    //等当前节点收到集合中存放的节点的投票时，会去集合中查找，看看有没有对应的节点
    //如果有就把该节点标记为已找到，并且把投票数计入结果，说明成功收到投票了
    public static class UnfoundPeerId {
        //节点信息
        PeerId peerId;
        //节点是否被找到，UnfoundPeerId对象刚创建的时候
        //默认这个成员变量为false，代表着该节点对象还没被找到
        boolean found;
        //节点索引
        int index;

        //构造方法
        public UnfoundPeerId(PeerId peerId, int index, boolean found) {
            super();
            this.peerId = peerId;
            this.index = index;
            this.found = found;
        }
    }

    //存放当前配置中的节点信息
    private final List<UnfoundPeerId> peers = new ArrayList<>();
    //当前配置中需要收到的最小票数，才能成功当选领导者或者进入正式投票节点
    private int quorum;
    //存放旧配置中的节点信息
    private final List<UnfoundPeerId> oldPeers = new ArrayList<>();
    //旧配置中需要收到的最小票数，才能成功当选领导者或者进入正式投票节点
    private int oldQuorum;


    //该类最核心的方法，初始化该类的对象时，会把集群中的节点信息封装到该类中，并且根据集群中
    //节点数量计算出当前节点至少需要收到多少票数，才能成功当选领导者或者进入正式投票阶段
    public boolean init(final Configuration conf, final Configuration oldConf) {
        //先清空两个集合
        this.peers.clear();
        this.oldPeers.clear();
        //需要的最小票数也清零
        this.quorum = this.oldQuorum = 0;
        //定义节点索引，该索引会在创建UnfoundPeerId对象时一直递增，当作每一个节点的索引
        int index = 0;
        //将当前配置中的节点信息存放到集合中
        if (conf != null) {
            for (final PeerId peer : conf) {
                this.peers.add(new UnfoundPeerId(peer, index++, false));
            }
        }
        //计算出最少需要的票数才能成功当选或者进入正式投票阶段
        //这里的成员变量是针对当前配置的
        this.quorum = this.peers.size() / 2 + 1;
        //下面判断旧配置是否为空
        if (oldConf == null) {
            return true;
        }
        index = 0;
        //如果旧配置不为空，则执行和上面相同的逻辑
        for (final PeerId peer : oldConf) {
            this.oldPeers.add(new UnfoundPeerId(peer, index++, false));
        }
        this.oldQuorum = this.oldPeers.size() / 2 + 1;
        return true;
    }


    //根据给订的PeerId，判断集合中是否存在对应节点
    private UnfoundPeerId findPeer(final PeerId peerId, final List<UnfoundPeerId> peers, final int posHint) {
        //这里先判断了一下代表节点在集合中的位置是否是正确的，当一个节点被包装成UnfoundPeerId对象的时候，它在集合中的位置是-1
        //显然是不合理的，这种情况下应该遍历集合，根据对应的peerId找出对应节点，然后根据节点的index来更新自己在集合中的位置
        //也就是更新PosHint对象中的索引
        if (posHint < 0 || posHint >= peers.size() || !peers.get(posHint).peerId.equals(peerId)) {
            for (final UnfoundPeerId ufp : peers) {
                if (ufp.peerId.equals(peerId)) {
                    return ufp;
                }
            }
            return null;
        }
        //如果走到这里说明节点在集合中的位置已经更新了，直接根据位置获得节点即可
        return peers.get(posHint);
    }


    //给当前节点投票的方法
    public PosHint grant(final PeerId peerId, final PosHint hint) {
        //在peers集合中查找对应的节点
        UnfoundPeerId peer = findPeer(peerId, this.peers, hint.pos0);
        if (peer != null) {
            //找到的话意味着方法参数中的那个peerId节点的投票是有效的
            //这个peerId在集群中
            if (!peer.found) {
                //将这个节点标记为已找到，或者说是被发现
                peer.found = true;
                //需要的票数减1，减到0则意味着票数够了
                this.quorum--;
            }
            //更新节点在集合中的位置
            hint.pos0 = peer.index;
        } else {
            //如果没找到，就把位置设置为-1
            hint.pos0 = -1;
        }
        //判断一下旧配置的节点集合是否为空
        if (this.oldPeers.isEmpty()) {
            hint.pos1 = -1;
            return hint;
        }
        //如果不为空，就执行和上面相同的逻辑，这里的当前配置和旧配置先不必关心
        //后面将配置变更的时候，会在文章里详细讲解
        peer = findPeer(peerId, this.oldPeers, hint.pos1);
        if (peer != null) {
            if (!peer.found) {
                peer.found = true;
                this.oldQuorum--;
            }
            hint.pos1 = peer.index;
        } else {
            hint.pos1 = -1;
        }
        return hint;
    }


    //给当前节点投票的方法，方法中传进来的这个peerId，就是给当前节点投票的节点
    public void grant(final PeerId peerId) {
        //在下面这个方法中，真正执行投票的逻辑
        //注意，这里创建了一个PosHint对象，这个对象的作用
        //是用来表示peerId在集合中未知的，方便快速查找
        grant(peerId, new PosHint());
    }


    //该方法就是用来判断是否收到足够的票数了，因为没收到一票quorum就会减1，直到为0则意味着收到足够票数了
    public boolean isGranted() {
        return this.quorum <= 0 && this.oldQuorum <= 0;
    }
}
```

在回顾了上面这个 Ballot 类的代码之后，现在请大家想一想，虽然此时程序执行的并不是领导者选举操作了，而是领导者向跟随者复制日志。但是对于操作成功的判断都是一样的，一条日志如果想被成功提交，也应该收到超过集群半数的成功响应，如果是这样的话**，那么领导者每向跟随者节点发送一条日志，就为这条日志创建一个 Ballot 对象，当领导者接收到跟随者节点的成功响应了，就增加这条日志对应的 Ballot 计数器对象中的票数，直到票数超过集群半数节点，这条日志就可以直接被应用了**。这个逻辑应该还算容易理解吧，所以接下来我们就可以朝这个方向重构代码，当领导者向跟随者节点发送日志的时候，为日治创建一个 Ballot 对象。

但就当我想直接这么做的时候，问题又来了，领导者向跟随者节点发送日志的时候可不是只发送一条呀，而是一批一批地发送，如果只发送一条日志还好，创建一个 Ballot 投票计数器对象即可。但面对发送很多批日志的情况该怎么办呢？最直接的方法就是为每一条日志都创建一个 Ballot 投票计数器对象，比如说领导者向集群中的各个跟随者节点发送了一批日志，这批日志包含 3 个日志条目，所以领导者要创建 3 个 Ballot 对象。当领导者接收到这些日志的响应后，就会在所有的 Replicator 复制器对象中会通过这 3 个 Ballot 对象计算哪个请求可以应用，哪个请求还不能被应用。那么，创建的这 3 个 Ballot 对象应该存放到哪里呢？难不成要定义成 Replicator 复制器对象的成员变量吗？这种做法也太蠢了，或者在 Replicator 类中定义一个泛型为 Ballot 类型的集合，把领导者为每一条日志创建的 Ballot 对象都放到这个集合中，这样可以吗？显然，这么做也是不行的，原因也很简单，让我们把例子变得简单一点，假如领导者只向跟随者节点发送了一条日志，肯定就要为这条日志创建一个 Ballot 对象，如果真的在 Replicator 复制器对象中定义一个存放 Ballot 对象的集合，那么就会把这个 Ballot 对象存放到集合中。但是，请大家别忘了，**领导者内部实际上存在很多 Replicator 对象，每一个跟随者节点都对应一个 Replicator 对象，如果在 Replicator 中定义一个队列存放 Ballot 对象，就意味着每一个 Replicator 复制器对象中都存在一个存放 Ballot 对象的队列，这不就乱套了吗？本来领导者发送一条日志，为这条日志创建一个 Ballot 对象，所有 Replicator 复制器对象在处理日志请求的响应时共同使用唯一的一个 Ballot 投票计数器对象，现在每一个复制器对象都拥有自己的队列以及 Ballot 对象了，那还怎么统计成功响应的数量呢**？

所以，我的观点是，存放 Ballot 对象的队列不应该定义在复制器对象中，而应该是一个独立的组件，这个组件可以通过某些方式，比如封装到一些 options 参数对象中传递给复制器对象，让每一个复制器对象可以获得使用即可。如果让我来设计(实际上是抄袭)的话，**我会定义一个新的类，叫做 BallotBox，翻译过来就是投票箱的意思，在这个投票箱中定义一个存放 Ballot 对象的队列。并且把 BallotBox 对象封装在 ReplicatorOptions 参数对象中传递给 Replicator 复制器对象，这样一来所有 Replicator 复制器对象就都可以获得这个 BallotBox 投票箱中的投票计数器了，每一个投票计数器相对于每一条日志来说也就是唯一的了**。具体实现请看下面代码块。

```
//该类的对象就是一个投票箱
@ThreadSafe
public class BallotBox implements Lifecycle<BallotBoxOptions>, Describer {



    //这个队列存放的就是为每一个日志条目对象创建的Ballot对象，注意，这里使用了对象池技术
    //详情就不展开了，可以直接去代码中查看，这个对象池在之前已经见过了
    private final SegmentList<Ballot> pendingMetaQueue = new SegmentList<>(false);



    //要定义什么方法好呢？
}
```

好了，现在 BallotBox 类已经定义了一个成员变量，就是 pendingMetaQueue 队列，这个队列中存放的就是一个个 Ballot 对象，当领导者向跟随者节点发送了日志，每发送一条日志就要创建一个对应的 Ballot 投票计数器对象。那么，这个类中可以定义什么方法呢？**当然是提交日志，或者是应用日志的方法**。为什么这么做呢？因为每一条日志的票数结果都会在这个类中被统计出来，只要超过集群半数节点就可以被应用，既然可以被应用，所以就直接在这个类中执行日志被应用的操作好了。我们就可以把应用日志的方法定义为 commitAt 方法，如果想实现的简单点，就可以把判断日志是否可以被应用的逻辑也定义在该方法中。具体实现请看下面代码块。

```
//该类的对象就是一个投票箱
@ThreadSafe
public class BallotBox implements Lifecycle<BallotBoxOptions>, Describer {



    //这个队列存放的就是为每一个日志条目对象创建的Ballot对象，注意，这里使用了对象池技术
    //详情就不展开了，可以直接去代码中查看，这个对象池在之前已经见过了
    private final SegmentList<Ballot> pendingMetaQueue = new SegmentList<>(false);



    //定义的提交日志的方法
    //该类最核心的方法，提交firstLogIndex和lastLogIndex之间的日志，当然，在提交的时候还要判断这些日志是否收到了集群中过半节点的投票，该方法会被领导者调用
    public boolean commitAt(final long firstLogIndex, final long lastLogIndex, final PeerId peer) {

        //根据日志索引从pendingMetaQueue队列中取出每一条日志对应的Ballot投票计数器对象
        //如果这条日志对应的投票计数器收到了足够的票数，就应用这条日志，代码暂且不实现
        
    }
}
```

这时候肯定会有朋友感到奇怪，为什么在上面代码块的 commitAt 方法中，定义的方法参数有一个 firstLogIndex 和一个 lastLogIndex？我简单解释一下，**因为领导者向跟随者发送日志都是一批一批发送的，假如说有一批日志是从索引为 21 的日志开始，发送完索引为 45 的日志，那么当这批日志的响应被领导者接收之后，肯定也是对这一批日志一批判断，如果这一批日志收到的响应超过集群半数节点的数量了，那么这一批日志就可以全被应用**。如果理解了这个逻辑，大家在看我重构之后的 onAppendEntriesReturned 方法应该就没什么困惑了，请看下面代码块。

我们就先把 BallotBox 对象封装到 Replicator 的 ReplicatorOptions 成员变量中。

```
//整个jraft框架中第二核心类，该类的对象就是一个复制器，如果当前节点是领导者，那么集群中的其他节点都会被领导者包装成一个个
//复制器对象，心跳检测，日志传输都是通过复制器对象进行的
@ThreadSafe
public class Replicator implements ThreadId.OnError {

    //省略其他内容

    private final ReplicatorOptions options;


     //构造方法
    public Replicator(final ReplicatorOptions replicatorOptions, final RaftOptions raftOptions) {
        super();
        //在这里给options成员变量赋值了，注意，options是被方法参数replicatorOptions赋值的
        //这就意味着在创建Replicator对象之前，BallotBox对象就已经创建了，并且封装到了
        //replicatorOptions对象中，然后在创建Replicator对象的时候，赋值给了该对象内部的options成员变量
        //那么，这个BallotBox对象究竟是在什么时候被创建的呢？
        this.options = replicatorOptions;
        //要发送的下一条日志的索引默认初始值为领导者最后一条日志索引加1
        this.nextIndex = this.options.getLogManager().getLastLogIndex() + 1;
        this.timerManager = replicatorOptions.getTimerManager();
        this.raftOptions = raftOptions;
        this.rpcService = replicatorOptions.getRaftRpcService();
    }


     private static boolean onAppendEntriesReturned(final ThreadId id, final Inflight inflight, final Status status,final RpcRequests.AppendEntriesRequest request,final RpcRequests.AppendEntriesResponse response, final long rpcSendTime,final long startTimeMs, final Replicator r) {

         //省略部分内容

         
         //走到这里，就意味着不管是探针请求响应还是日志请求响应，都成功回复了
         //下面就是公共操作了
         
         //得到这次请求批量发送的日志的数量
        final int entriesSize = request.getEntriesCount();

        //在这里就可以把日志请求的成功响应收集起来
        //如果数量大于0，就说明这次的请求肯定是日志复制的请求，并且也成功了
        if (entriesSize > 0) {
            
            //要在领导者判断发送成功的日志是否收到过半投票了
            //如果收到了就把这些日志应用到状态机中
            //r.nextIndex得到的就是本批传输日志的起始索引，r.nextIndex + entriesSize - 1得到的就是本批日志最后一条日志索引
            r.options.getBallotBox().commitAt(r.nextIndex, r.nextIndex + entriesSize - 1, r.options.getPeerId());
        }
         
       
        //更新接下来要发送的日志的索引，然后会跳出该方法，来到外层onRpcReturned方法，在finally中继续发送日志给跟随者节点
        r.nextIndex += entriesSize;
        return true;
         
     }


    //省略其他内容
}
```

当然，我知道尽管列出了上面的代码块，大家肯定还是会觉得困惑，既然在 onAppendEntriesReturned 方法中也是按批处理日志的，为什么还要给每一条日志都创建一个投票计数器对象呢？为什么不能为每一批日志创建一个 Ballot 对象，然后把 Ballot 对象放到 BallotBox 对象的队列中呢？这时候大家可能还会思考、甚至是怀疑 Ballot 投票计数器对象的创建时机，我刚才一直在跟大家说，Ballot 对象会在领导者向跟随者发送每一条日志的时候被创建，但真的是这样吗？难道不能有其它的创建时机吗？我相信除了这两个问题，大家还对另一个问题感到困惑，也就是我在上面代码块第 14-17 行提出的问题，BallotBox 投票箱对象究竟是在哪里被创建的呢？当然，可能还有朋友好奇，在 BallotBox 类中，除了目前的一个成员变量一个方法，还应该定义什么成员变量和方法呢？现在的 BallotBox 肯定不是完全体，太简陋了，简陋的连佛利萨第一形态都不如，这个类至少还应该再经过几次变身，才能展现完全面目。接下来，先让我为大家把刚才列出的几个问题简单总结一下，并且再拓展几个问题，总结完毕之后，我们接下来的思考方向也就明确了。

**1 为什么要给每一条日志都创建 Ballot 投票计数器对象，难道不能给每一批日志创建一个唯一的 Ballot 对象吗？**

**2 Ballot 对象究竟在哪里被创建呢？**

**3 BallotBox 对象究竟是在什么时候被创建的？**

**4 BallotBox 对象内部还应该定义什么成员变量和方法？**

**5 创建好的 Ballot 对象怎么存放到 BallotBox 对象的内部队列中呢？**

**6 BallotBox 对象要怎么应用已被提交的日志呢？肯定是把要被应用的日志交给状态机，但状态机在哪里呢？是怎么实现的呢？**

**7 BallotBox 对象内部队列中的 Ballot 投票计数器什么时候被清除呢？**

以上就是我为大家总结的 7 个问题，说实话，这 7 个问题并不难回答，而且代码实现也比较简单。但是互相关联，而且随着一个问题的解决，再次引出新的问题，换句话说，上面七个问题只是引子，随着问题的解决，更多问题会随着浮出水面，但大家不必惊慌，再多的问题都可以解决，再复杂的逻辑都有一条主线。我要做的就是帮助大家精准掌握这些内容。当然，这一章我就先不为大家分析这些内容了，因为这 7 个问题肯定分析不完，所以就留到下一章分析吧。反正状态机这一块的内容非常多，我已经做好了用三篇文章实现状态机所有功能的准备。所以，就让我们下一章见吧，到下一章大家就会发现，我们的程序就会又一次来到了 NodeImpl 类中，这个类是我们课程的起点，是征服 sofajraft 框架的起点。好了朋友们，我们下一章见。