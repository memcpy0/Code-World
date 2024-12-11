大家好，从这一章开始，就正式进入到 sofajraft 框架的手写实现领域了。在开始正式实现之前，我想先介绍一下接下来要带领大家实现的这个框架，这个框架是一个 Raft 共识算法框架，一般来说，这种框架只会作为其他框架的一个核心依赖使用，并不会和我们的业务打交道。就像 Netty 一样，可能在很多框架中都能见到 Netty，但是在真正的工作中，你很少有机会直接使用 Netty 对你的业务做一些操作。而且我还可以很负责任地和大家说一句，Sofa-Jraft框架内部虽然有很多细节，并且对 Raft 算法本身做了一些优化，学习起来可能要花点时间，但是，这个框架没有一点难度，内部的所有逻辑都很顺畅，只要你掌握了 Raft 共识算法，彻底掌握这个框架也是易如反掌的事。当然，还是需要付出时间的，就像我说我可以跑 1000 米，这很简单，有两条腿，只要会迈开腿就能跑 1000 米。但让我现在跑，我体力跟不上啊，可能需要几天时间练习一下，就可以跑完 1000 米了。Sofa-Jraft 框架简单得就像迈开腿跑步一样，只要你花几天时间学习，最后肯定可以完全掌握。

**引入 Node 接口，分析 Node 节点的状态转换**

  

既然现在已经正式进入实现 sofa-jraft 框架阶段了，有一些概念还是最好和 raft 共识算法中的概念保持一致比较好。在前三章我虽然为大家举了很多例子，但是所有例子都是围绕着主从复制，也就是单主复制来展开的。在单主复制模式下构建的集群，集群主节点可以处理来自客户端的写操作指令，然后将写操作指令包装成日志传输给从节点，所有从节点只需要将日志持久化到本地即可。如果集群的主节点突然故障了，那么其他从节点就会根据各自的随机超时选举时间，决定自己什么时候触发超时选举，进入超时选举阶段，从而选举出一个新的主节点。可以看到，在前三章的内容中，**我们只是以主节点和从节点来区分集群中各个节点的身份。**集群中只能有一个主节点，其他的都是从节点。正常提供服务的集群中，只会有两种类型的节点，那就是主节点和从节点；如果主节点宕机了，在选举新的主节点的时候，集群中所有节点的身份都是从节点。这么做当然没问题，因为前三章我就是在这个模式下，为大家举的例子。但是，现在既然是要正式构建一个工业级别的框架了，显然是越正规越好，越正是越好，越详细越好。

在 raft 共识算法中，把一个集群中的节点分为三种身份，**第一种身份就是领导者，也就是主节点，集群中只能有一个领导者；第二种身份就是跟随者，也就是从节点；领导者负责处理客户端的写指令，将写指令包装成带索引的日志，然后把日志传输给集群中的所有跟随着，跟随者会将日志持久化到本地；第三种身份就是候选者，这个候选者听起来有些特殊，实际上很容易理解，当集群中的领导者宕机后，率先触发了超时选举，进入领导者选举阶段的节点，就是候选者。所谓候选者，就是领导者的候选者。**可以看到，虽然在 raft 共识算法中节点的身份是用另一种概念来解释的，但其承担的职责，以及工作模式，和前三章的主节点、从节点都是一样并且对应的。

我在这里忽然解释这个，并不是说想为大家再详细讲解一遍 raft 共识算法，这个算法的大部分内容在前三章都已经讲解完毕了，只不过有些术语并不是按照 raft 共识算法论文中那样解释的。术语或者概念的不同并不会影响大家构建 sofa-jraft 框架。我解释 raft 共识算法定义集群中节点身份的方式，是希望大家能帮助大家形成一种认识：**集群中的节点可以被叫做主节点，也可以被叫做从节点，也可以被叫做领导者，被叫做跟随者，被叫做候选者，你可以使用任何称谓，只要你觉得合适。但是隐藏在这些称谓背后的，实际上是节点状态的改变。**

好了，到这里让我们来讨论一下，所谓的节点是什么？在我看来，**节点就是服务器的抽象。**我们都知道，程序都是部署在服务器上的，集群构建成功之后，集群中的各个服务器会根据其他服务器的状态改变自己的状态，当然，集群一切正常的时候，各个服务器正常工作即可。但有一个服务器无法提供正常服务，可能就会让另一个服务器进入到领导者选举阶段，从而改变自己的状态。在程序运行的过程中，在集群工作的时候，我们希望某个服务器作为领导者，也就是主节点，希望某些服务器可以作为跟随者，也就是从节点，希望某个服务器可以在适当的时候成为候选者，这些情况我们肯定会遇到。但是，我们又不能直接操纵服务器本身，所以就在程序中定义了一个 Node 节点，用这个节点来作为服务器的抽象。给这个 Node 定义了一些方法和属性，人为地规定这个节点在某些情况下只能干什么，不能干什么，这样一来，集群中的一套规则也就构建完整了。这就和创建的线程池差不多，线程池在某个状态下只能做固定的工作，如果一个线程池处于已经关闭的状态，肯定就不能执行任务了。

接下来再举一个例子，比如说，一个集群被构建完成了，我启动了这个集群。集群刚刚启动的时候，集群中的每个节点都是刚刚被创建，这时候集群中没有领导者，也没有跟随者，也没有候选者。**集群中各个节点的状态就应该都是未初始化的状态。在经过了一段时间之后，某个节点率先触发了超时选举，进入了领导者选举阶段，这时候，这个节点就要变成候选者状态，如果成功当选为领导者，那么这个节点就要变更自己的状态为领导者，如果没能当选成功，就要变更自己的状态为跟随者。而节点所处的状态，决定了节点本身可不可以将指令包装为日志，也就是能否生产日志。如果不能生产日志，那么节点只能复制日志，被动应用每一条客户端指令。**

很好，现在我们已经为节点定义了 4 种状态了，分别是：**未初始化状态，领导者状态，候选者状态，跟随者状态。**也许有的朋友认为这样就已经够了，但要我说，这几种状态定义得还是不够全面，还要再添加几种。比如说节点运行过程中出现错误不能正常工作了，至少应该定义一个错误状态；总有一个时刻，节点也可能要结束工作了，就像线程池那样，所以也应该定义一个正在停止工作状态，还有一个已经停止工作状态。这种套路大家应该都已经很熟悉了吧？优雅停机那一套大家应该已经见得够多了。所以，现在我们已经给节点定了一 7 种状态了，分别是：**未初始化状态，领导者状态，候选者状态，跟随者状态，错误状态，正在停止工作状态，已经停止工作状态。**如果用代码来表示的话，可以定义成一个 State 枚举类，并且我们还可以在枚举类中定义一个判断当前节点是否处于活跃状态的方法。请看下面代码块。

```
 //节点状态枚举类
public enum State {
    //领导者状态
    STATE_LEADER,
    //候选者状态
    STATE_CANDIDATE,
    //跟随者状态
    STATE_FOLLOWER,
    //当前状态表示节点出现了错误
    STATE_ERROR,
    //表示节点还未初始化
    STATE_UNINITIALIZED,
    //表示节点正在停止工作
    STATE_SHUTTING,
    //已经停止工作
    STATE_SHUTDOWN,

    //该方法判断当前节点是否处于活跃状态
    public boolean isActive() {
        //原理很简单，就是判断当前状态的枚举对象是否小于STATE_ERROR的值
        //ordinal方法用于返回一个int，排在前面的枚举对象的这个int值小于排在后面的
        //只要是小于STATE_ERROR，就意味着当前节点还在正常工作，大于STATE_ERROR，当前节点不是出错，就是要停止工作了
        return this.ordinal() < STATE_ERROR.ordinal();
    }
}
```

到此为止，节点的几种状态就已经展示完毕了。接下来，我们就应该重点分析节点的启动过程了。

**引入 Node 接口**

既然要分析节点的启动过程了，首先还是先把节点本身定义一下吧。在上一小节我们已经把服务器本身抽象成了一个 Node 节点，但实际上，这个 Node 在我们正在搭建的 raft 框架中，只是一个接口，在这个接口中会定义很多方法，这些方法都会由这个接口的实现类 NodeImpl 来实现。当然，现在只是搭建框架的起步阶段，我还没想好 Node 接口中要定义什么方法，所以就先写一个空接口吧。请看下面代码块。

```
public interface Node {


}
```

接下来就是 NodeImpl 实现类了，这里我就不得不解释一下，虽然 Node 接口现在是空的，这并不意味着 NodeImpl 实现类也是空的。接口中一般只定义方法，但现在让我们想一想，就算我们还没有想好要给 Node 节点定义什么方法，这并不妨碍给 NodeImpl 实现类定义一些应该具备的属性。比如说，我们是在构建一个完整的集群，集群中有很多节点，每一个 NodeImpl 都是节点本身，是对服务器的抽象，并且集群中节点与节点之间是需要通信的。既然要通信，就应该定义一个类来封装每一个节点的 IP 地址和端口号，比如，**我们还是使用前三章的那个 Endpoint 对象来封装节点的 IP 地址和端口号。**具体实现请看下面代码块。

```
//该类的对象会封装节点的IP地址和端口号信息
public class Endpoint implements Copiable<Endpoint>, Serializable {

    private static final long serialVersionUID = -7329681263115546100L;
    //初始化的ip地址
    private String ip = Utils.IP_ANY;
    //端口号
    private int port;
    //ip地址端口号的字符串形式
    private String str;

    public Endpoint() {
        super();
    }

    public Endpoint(String address, int port) {
        super();
        this.ip = address;
        this.port = port;
    }

    public String getIp() {
        return this.ip;
    }

    public int getPort() {
        return this.port;
    }

    @Override
    public String toString() {
        if (str == null) {
            str = this.ip + ":" + this.port;
        }
        return str;
    }

    @Override
    public Endpoint copy() {
        return new Endpoint(this.ip, this.port);
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + (this.ip == null ? 0 : this.ip.hashCode());
        result = prime * result + this.port;
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Endpoint other = (Endpoint) obj;
        if (this.ip == null) {
            if (other.ip != null) {
                return false;
            }
        } else if (!this.ip.equals(other.ip)) {
            return false;
        }
        return this.port == other.port;
    }
}
```

很好，现在让我们继续来思考另一个问题，Endpoint 对象是用来封装一个节点的 IP 地址和端口号的，也就是封装一个服务器的 IP 地址和端口号信息，但现在我们是在构建一个集群，如果某个节点属于一个集群，并且正在对外提供服务，我们还应该再对这个 Endpoint 对象做一层封装。现在让我来解释一下为什么要做一层简单的封装：**如果我们只是按照 raft 论文实现一个简单的 raft 共识算法框架，也不考虑一些其他的优化或者细节，那么其实对 Endpoint 封不封装也无所谓，但如果可以自定义一些优化呢？比如我打算给节点本身添加一个额外的属性，就是节点优先级，本来在 raft 集群中，每一个节点都有可能成为领导者，但我们都知道，领导者要处理大量的写操作，所有写操作的指令都会访问到领导者，从这一点上来说，领导者本身的压力是比较大的。如果现在我有 5 台服务器，第 1 台服务器硬件配置最好，其他 4 台性能都一般，只要我脑子不傻，我肯定会选择第 1 台服务器作为集群中的领导者。这时候我们就可以在每一个节点初始化的时候，给每一个节点定义一个优先级，在节点触发了超时选举之后，即将进入领导者选举阶段时，会判断一下自己的优先级是不是集群中最大的优先级，如果是就进入领导者选举阶段，如果不是就不进行领导者选举。这样一来，就可以按照我们的意思指定某个节点成功当选领导者了。**当然，在我们的第一版本代码中，我并没有引入这个节点优先级机制，但不是说这个机制很麻烦，恰恰相反，这个机制非常简单，只需要一个成员变量，一个不到十行的方法就实现了。我之所以没有引入是想先按照比较纯粹的 raft 算法来实现我们这个框架，在第十三版本代码中，我把这个优先级机制实现了，到时候大家可以自己看看，或者直接去源码中查看。

**引入集群节点信息类 PeerId**

分析了这么多，那么这个新的类该怎么定义呢？首先，这个类肯定需要持有 Endpoint 成员变量，然后就是刚才分析的节点优先级属性，当然，这个属性我们暂时还用不上。所以，我们可以把这个类定义成下面这样，类名就叫做 PeerId 。请看下面代码块。

```
//这个类就是用来解析当前节点信息的，当前节点的IP地址和端口号，节点参与选举的优先级，这些信息都会封装在该类的对象中
public class PeerId implements Serializable{

    private static final long serialVersionUID = 8083529734784884641L;

    private static final Logger LOG = LoggerFactory.getLogger(PeerId.class);

    public static final String IP_ANY = "0.0.0.0";
    
    //当前节点的IP地址和端口号就封装在这个成员变量对象中
    private Endpoint endpoint = new Endpoint(IP_ANY, 0);
    //该类对象的toString结果的缓存
    private String str;
    //节点初始化的时候，默认选举优先级功能是禁止的
    private int priority = ElectionPriority.Disabled;


	//构造方法
    public PeerId() {
      
    }

    //创建一个空节点对象
    public static PeerId emptyPeer() {
        return new PeerId();
    }

    //设置当前节点选举优先级
    public void setPriority(int priority) {
        this.priority = priority;
        this.str = null;
    }


	//还有一些get方法就全省略了



}
```

至于选举优先级的具体定义，请看下面代码块。

```
//节点选举的优先级
public class ElectionPriority {


    //这些优先级信息都会封装在PeerId对象中，在第一版本中并没有展示这方面的功能
    //优先级为-1时，表示该节点已禁用了按照优先级选举功能
    public static final int Disabled   = -1;

    //为0时，表示该节点永远不会参数选举，也就意味着永远不会成为领导者
    public static final int NotElected = 0;

    //选举优先级的最小值，选举的时候优先级最低，比如在集群中就有一台的服务器性能最好，我就希望这个服务器当作领导者
    //那就可以通过优先级配置，使这个服务器成为领导者的概率更大
    public static final int MinValue   = 1;
}
```

本来这个 PeerId 类的内容，到这里就简单介绍完毕了，如果还要再定义什么方法，那也是后面小节的内容。但是，在 sofajraft 框架中还有一个扩展点，这个是 sofajraft 框架本身的一个扩展点，那就是在 PeerId 类中还定义了一个成员变量 idx，根据官方文档的解释，这个成员变量的作用是为了支持同一个端口号启动不同的 raft 节点，节点的身份就用 idx 的值来区分。但是，**这个成员变量只是一个预留的扩展，在程序中并没有被真正使用到，一直就是默认值 0(说实话，我也不知道它这个最后要实现成什么样子，这里我强调一下，这个 idx 成员变量和 sofajraft 框架的 MULTI-RAFT-GROUP 模式并没有很明显的关系，，因为 sofajraft 内嵌的 kv 数据库框架使用的就是 MULTI-RAFT-GROUP 模式，每一个 Store 就是一个独立的物理存储节点，一个 Store 中有多个 Region，所有 Region 共享 Store 的端口号，但是每一个 Region 的 GroupId 并不相同，通过这个不同的 GroupId，服务器把信息分发给对应的 Region，这是我亲自从源码中看到的。我这里解释了这么多，大家可能不知道什么意思，大家可以先对这些东西混个眼熟，后面还会提到)。在 sofajraft 框架的所有测试类中，并没有真正使用到这个 idx 成员变量，在 Nacos 框架中，也没有对 idx 这个成员变量进行什么赋值。**这个大家就简单了解一下就行了。我在这里之所以提到它，并且把它加上，是为了代码的完整性。所以，我们新定义的 PeerId 类就变成了下面这样。请看下面代码块。

```
//这个类就是用来解析当前节点信息的，当前节点的IP地址和端口号，节点参与选举的优先级，这些信息都会封装在该类的对象中
public class PeerId implements Serializable {

    private static final long serialVersionUID = 8083529734784884641L;

    private static final Logger LOG = LoggerFactory.getLogger(PeerId.class);

     public static final String IP_ANY = "0.0.0.0";
    
    //当前节点的IP地址和端口号就封装在这个成员变量对象中
    private Endpoint endpoint = new Endpoint(IP_ANY, 0);
    
    //这个是在同样的IP地址下，用来区分集群中不同节点的
    //比如在某个特定环境下，三个节点使用的IP地址是相同的，这时候就可以通过这个idx来区分这三个节点了
    //比如IP地址和端口号都是localhost:8080
    //idx分别为1，2，3这样就区分开了
    //在我们的课程中不会讨论这种情况，实际上，我们应该不会这样来部署自己的程序
    private int idx;
    
    //该类对象的toString结果的缓存
    private String str;
    //节点初始化的时候，默认选举优先级功能是禁止的
    private int priority = ElectionPriority.Disabled;
    //这个成员变量起到一个标识符的作用，具体使用的时候我会为大家讲解的
    public static final PeerId ANY_PEER = new PeerId();


    public PeerId() {
        super();
    }

    //创建一个空节点对象
    public static PeerId emptyPeer() {
        return new PeerId();
    }

    //深拷贝方法
    public PeerId copy() {
        return new PeerId(this.endpoint.copy(), this.idx, this.priority);
    }

    //下面都是一些构造方法
    public PeerId(final Endpoint endpoint, final int idx) {
        super();
        this.endpoint = endpoint;
        this.idx = idx;
    }

    public PeerId(final String ip, final int port) {
        this(ip, port, 0);
    }

    public PeerId(final String ip, final int port, final int idx) {
        super();
        this.endpoint = new Endpoint(ip, port);
        this.idx = idx;
    }

    public PeerId(final Endpoint endpoint, final int idx, final int priority) {
        super();
        this.endpoint = endpoint;
        this.idx = idx;
        this.priority = priority;
    }

    public PeerId(final String ip, final int port, final int idx, final int priority) {
        super();
        this.endpoint = new Endpoint(ip, port);
        this.idx = idx;
        this.priority = priority;
    }
}
```

分析完了 PeerId 这个类，接下来，我又有了一个新的问题，刚才我也在文章里提到了，集群之间，在某些情况下各个节点可能都是要互相交流的，领导者选举的时候，候选者要和所有的跟随者交流，领导者选举成功之后，领导者要和所有的跟随者节点交流。虽然我们已经把集群中的节点包装成一个个 PeerId 对象了，但是到现在也没有具体展示集群中各个节点的交流方式，因为最重要的前提条件我们还没实现：**集群中的各个节点根本没有彼此的信息，也就是说，集群中的各个节点根本不知道对方的存在。**

**引入集群配置信息类 Configuration**

打个比方，你要和一个人联系，至少得知道人家的联系方式或者家庭住址，你能找到这个人，才能和这个人联系吧？而在我们目前的集群中，各个节点都不知道彼此的信息，说得直接一点，大家都不知道彼此服务器的地址，IP 地址和端口号全都一无所知，想发送 RPC 消息也不可能。因为我们还需要定义一个新的类，用这个类来封装集群中各个节点的信息，并且每一个节点启动的时候，都会得到一个这个类的对象。这样，每一个节点就都知道集群中的各个成员都是谁了，也就可以通信了。讲到这里，我想大家应该也明白了，说了这么多，**实际上我们要做的就是把集群的配置信息统一封装起来，集群中有多少节点，每个节点的网络地址是什么，这些就是集群的配置信息，集群添加了新的节点或者以为故障减少了节点，这些都可以称为集群配置信息的变更，**我决定把新引入的这个类叫做 Configuration，这个类的对象就是用来封装集群的配置信息的，所以我也把它成为集群配置信息类。具体实现请看下面代码块。

```
public class Configuration{
    
    private static final Logger LOG = LoggerFactory.getLogger(Configuration.class);


    //存放PeerId的集合，因为集群中各个节点都会被包装成一个个PeerId对象
    //所有PeerId对象都存放在这个集合中，也就意味着Configuration对象获得了
    //集群中所有节点的信息
    private List<PeerId> peers = new ArrayList<>();

    //下面是构造方法
    public Configuration(final Iterable<PeerId> conf) {
        Requires.requireNonNull(conf, "conf");
        for (final PeerId peer : conf) {
            //把每个节点添加到集合中
            this.peers.add(peer.copy());
        }
    }


    public Configuration copy() {
        return new Configuration(this.peers, this.learners);
    }


     //重制配置信息的方法
    public void reset() {
        this.peers.clear();
    }

    public List<PeerId> listPeers() {
        return new ArrayList<>(this.peers);
    }

    public List<PeerId> getPeers() {
        return this.peers;
    }

    public void setPeers(final List<PeerId> peers) {
        this.peers.clear();
        for (final PeerId peer : peers) {
            this.peers.add(peer.copy());
        }
    }

     public boolean addPeer(final PeerId peer) {
        return this.peers.add(peer);
    }

    //还有很多操纵peers集合的方法，就不再一一展示了，当然，这类中还有一个学习者集合
    //这些概念后面我们都会讲到，大家看我提供的代码时，可以先不关注这个集合
}
```

好了，这个 Configuration 类已经定义完毕了，看起来已经比较完美了，集群中的配置信息也可以被封装起来了。但是我觉得只有这个 Configuration 类还是不够的。刚才我就说了，集群的配置信息并不是一成不变的，也是会发生变更的。所以，如果我们要封装集群中的配置信息，实际上封装的也只能是某个时刻的，如果集群配置发生了变更，比如集群中增加了新的节点，这个时候，集群的配置信息就需要发生变化了，对每个节点来说，集群配置信息就需要更新了。如果是这样的话，我的建议是，**再定义一个封装配置信息的类，这个类的对象封装的配置信息都是实时的，如果配置信息发生了变更，那么就更新这个类的对象封装的配置信息。我想把这个新的类定义为 ConfigurationEntry，字面意思就是配置信息条目类，类的对象就是配置信息条目对象。**我觉得这个类名还是挺形象的，每一个 ConfigurationEntry 对象封装的都是当前的集群配置信息，如果信息忽然发生变更了，那么就要创建新的 ConfigurationEntry 对象，更新集群配置信息。这个类的实现也很简单，具体实现请看下面代码块。

```
//该类的对象封装的是当前集群的配置信息，这个配置分为新配置和旧配置，因为集群的配置也是有可能变更的
//这个类在第一版本其实没有发挥什么作用，因为我们也没有对集群进行配置变更，这是后面版本代码要展示的内容
//所以，在第一个版本，大家就简单看看这个类即可
public class ConfigurationEntry {

    private static final Logger LOG = LoggerFactory.getLogger(ConfigurationEntry.class);

    //当前生效的配置
    private Configuration conf = new Configuration();
    //旧的配置
    private Configuration oldConf = new Configuration();


    //构造方法
    public ConfigurationEntry() {
        super();
    }

    public Configuration getConf() {
        return this.conf;
    }


    public void setConf(final Configuration conf) {
        this.conf = conf;
    }


    public Configuration getOldConf() {
        return this.oldConf;
    }


    public void setOldConf(final Configuration oldConf) {
        this.oldConf = oldConf;
    }


    //判断旧的配置是否为空，如果旧配置为空，说明当前集群的配置没有进行过变更，也就代表当前集群是很稳定的
    public boolean isStable() {
        return this.oldConf.isEmpty();
    }

    //判断当前配置是否为空
    public boolean isEmpty() {
        return this.conf.isEmpty();
    }

    //把当前配置和就配置中的所有PeerId以集合的形式返回
    public Set<PeerId> listPeers() {
        final Set<PeerId> ret = new HashSet<>(this.conf.listPeers());
        ret.addAll(this.oldConf.listPeers());
        return ret;
    }


    //判断某个PeerId是否存在于当前配置或旧配置中
    public boolean contains(final PeerId peer) {
        return this.conf.contains(peer) || this.oldConf.contains(peer);
    }

	//这个类中也有一些和学习者有关的方法，大家也可以暂时忽略
}
```

很好，现在已经准备得差不多了，是时候真正引入 NodeImpl 实现类了。

**引入 NodeImpl 节点实现类**

在具体实现这个 NodeImpl 类之前，让我们根据前几小节的内容，简要分析一下，这个 NodeImpl 类中应该定义什么成员变量和方法。当然，Node 接口中还没有定义任何一个方法，但这并不意味着 NodeImpl 实现类不能拥有自己的方法。不过还是先让我们来分析分析应该给这个类定义什么成员变量吧。

我首先想到的就是集群 ID，原因很简单，我们最终要做的就是使用自己搭建出来 raft 框架构建一个完美的集群，总不能最后连一个集群 ID 都没有吧？到最后集群是定义出来了，但节点连自己在哪个集群都不知道，这就有点说不过去了。所以，**显然是要给 NodeImpl 实现类定义一个 groupId 成员变量；然后呢？节点状态也是必须定义的吧？否则之前引入的 State 枚举类就没什么用了；除此之外，还需要定义一个 PeerId 记录自己的节点信息；还需要一个 PeerId 记录领导者的节点信息；当然了，最后还需要一个 ConfigurationEntry 对象来记录集群当前生效的配置信息。**

这时候就可以正式引入 NodeImpl 实现类了，请看下面代码块。

```
//整个框架中最核心的类，关于节点的一切功能几乎都定义在这个类中了，不是在该类中直接实现
//就是由该类的某个方法做了一层代理，总之，都会通过这个类的对象去执行操作，这个类的名字也很直接，就是节点实现的意思
//在分布式集群中，每一个服务器都会被抽象为一个节点，而节点的功能就是当前类提供的，在第一版本我只为大家引入了领导选举的功能
//实际上，该类有近4000行代码，在第一版本我为大家删去了近3000行，删去的内容后面会逐渐补全
public class NodeImpl implements Node{

    private static final Logger LOG = LoggerFactory.getLogger(NodeImpl.class);


    //当前节点所在集群的Id
    private final String groupId;

    //当前节点的状态
    private volatile State state;

    //当前节点记录的领导者的PeerId
    private PeerId leaderId = new PeerId();

    //当前节点自己的节点信息
    private final PeerId serverId;

    //集群当前生效的配置信息，包括旧的配置信息
    private ConfigurationEntry conf;

    
}
```

好了，NodeImpl 类的成员变量定义得差不多了，让我们再接着想想，还可以定义什么方法呢？当然，首先就是构造方法，这个好说，那除了构造方法呢？大家应该还有一点点印象，那就是在前面我为大家定义 Node 状态的时候，跟大家简单提过一句，**节点刚刚创建出来的时候处于未初始化的状态**，既然有未初始化的状态，显然就应该有初始化节点的方法，**既然有初始化节点的方法，显然就应该有销毁节点的方法，或者说应该有让节点终止工作的方法。**到这里大家应该反应过来了，**要实现一个框架，框架中某些关键组件的生命周期是一定要考虑到的。**既然是生命周期，那就不妨再定义一个新的接口，把生命周期的方法定义在这个新的接口当中，比如，我就把这个新的接口定义为 Lifecycle，然后让 Node 接口继承这个生命周期接口。这样，NodeImpl 实现了 Node 接口时，就可以直接显现 Lifecycle 接口中的方法了。具体实现请看下面代码块。

```
//生命周期接口
public interface Lifecycle<T> {

    //初始化方法
    boolean init(final T opts);
	//终止方法
    void shutdown();
}
```

接下来是 Node 接口。

```
//这里大家先不用关心这个NodeOptions是干什么用的，很快就会讲到
public interface Node extends Lifecycle<NodeOptions>{


}
```

然后就是 NodeImpl 实现类，这一次我们要在实现类中把构造方法添加上。

```
public class NodeImpl implements Node{

    private static final Logger LOG = LoggerFactory.getLogger(NodeImpl.class);


    //当前节点所在集群的Id
    private final String groupId;

    //当前节点的状态
    private volatile State state;

    //当前节点记录的领导者的PeerId
    private PeerId leaderId = new PeerId();

    //当前节点自己的节点信息
    private final PeerId serverId;

    //集群当前生效的配置信息，包括旧的配置信息
    private ConfigurationEntry conf;


    //构造方法
    public NodeImpl() {
        this(null, null);
    }

    //构造方法
    public NodeImpl(final String groupId, final PeerId serverId) {
        //对集群Id判空
        if (groupId != null) {
            Utils.verifyGroupId(groupId);
        }//给集群id成员变量赋值
        this.groupId = groupId;
        //给当前节点的serverId赋值
        this.serverId = serverId != null ? serverId.copy() : null;
        //节点刚刚创建的时候，是未初始化状态
        this.state = State.STATE_UNINITIALIZED;
        LOG.info("The number of active nodes increment to {}.", num);
    }


    //该方法暂时不做实现，这里也不用关心这个NodeOptions
    @Override
    public boolean init(final NodeOptions opts) {
        
    }

    //该方法暂时不做实现
    @Override
    public void shutdown() {

    }

    
}
```

现在，NodeImpl 这个类也被我们实现得差不多了。本来这一小节应该结束了，要继续讲解，为我们的程序引入 NodeOptions 类，但是，我不得不补充一个知识点。**那就是除了以上在 NodeImpl 类中定义的成员变量，还应该再定义一个成员变量，那就是 NodeId**。这会大家肯定都还不知道这个 NodeId 是干什么用的，怎么会突然冒出来一个 NodeId 呢？我理解大家的疑惑，所以先让我来为大家阐述一下 NodeId 的作用吧：**一个服务器中可能会启动多个 Node 节点，确切地说，一个服务器的进程中可能会启动多个 Node 节点，而这个 NodeId 是用来给服务器进程中的多个节点做区分的，简而言之，就相当于当前节点在服务器中的唯一标识。**

很好，我相信在看完上面这句话之后，大家可能会更加困惑了。所以接下来让我们看一个小例子。假如我现在有20000 条 kv 数据，也就是键值对数据，然后我构建了一个 raft 集群，集群中有 3 个节点，这也就意味着每一个节点都要备份这 20000 条数据。具体情况请看下图。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1704774479633-f769c761-91d7-4b41-b61f-114b550a7b4f.jpeg)

这个集群正常运行了一段时间之后，我就对目前的这个集群感到不满了，因为领导者处理客户端写指令的速度太慢了，我受不了这个速度了。问题很明显啊，我这些都是键值对数据，如果我有两个客户端，一个客户端想要修改其中一条数据，另一个客户端也想修改其中一条数据。比如说，就修改成下面这样。

```
客户端1：map.put("肛泰","20元")
客户端2：map.put("太宁栓","30元")
```

本来这两条数据根本就没有直接关系，但是现在我构建的是一个 raft 集群，raft 集群有一个特点，**那就是只能由领导者处理客户端写指令，并且要把每一条写指令包装成索引递增的日志赋值给跟随者，日志达成共识之后，领导者才可以应用指令。因为日志是索引递增的，所以日志就是有顺序的，这样一来，指令的执行也就有了顺序。但是，我现在修改的两条指令根本就没有什么顺序上的关系，它们互不干扰呀。按理说这两条客户端指令完全可以并发执行，不必考虑顺序关系。客户端 1 指令执行的时候，客户端 2 的指令也可以同时执行。但没办法，按照 raft 算法的要求，这两条客户端指令都会被包装成日志，都在同一个日志系统里面，就只能按照先后顺序来执行。**如果这两条执行可以并发执行，那整个程序的响应速度不就提高了一个层次吗？

那怎么才能让这两条指令并发执行呢？很快，我就想到了一个方法。之前无法并发执行是因为这两条指令包装成日志之后，都会在同一个日志系统里按照日志索引顺序等待应用。如果我想方法让这两条指令被包装成日志之后，并不在同一个日志系统中不就可以了吗？如果有两个日志系统，客户端 1 操作的数据在第一个日志系统中，客户端 2 操纵的数据在第二个日志系统中，这样一来，操纵这两条数据的指令就可以并发执行了。那怎么才能产生两个日志系统呢？很简单，一个 raft 集群对应一个日志系统，两个 raft 集群自然就会对应两个日志系统。现在，我们可以把这 2 万条数据拆分一下，用 3 个服务器构建 2 个 raft 集群，每个集群管理 1 万条数据。所以，集群就变成了下面这样。

![](https://cdn.nlark.com/yuque/0/2024/jpeg/26725125/1704782566789-a2b25d43-08bb-4e27-93b5-7c1007f92d6f.jpeg)

在上图中，我对每一个服务器部署的节点做了一些改动，每一个服务器不再部署一个 raft 节点了，而是部署了两个 raft 节点，并且这两个节点是由同一个进程启动的。这样一来，每一个服务器都部署了两个 raft 节点，最后形成了两个集群，每一个集群管理 1 万条数据。当客户端 1 要修改数据时，发现自己要修改的数据在集群 1 中，那就会把这条指令发送给集群 1 的领导者，客户端 2 会把自己的指令发送给集群 2 的领导者。这样一来，两个指令对应的日志分别属于不同的日志系统，也就意味着可以在一定程度上并发地执行这两条指令。当然，在上图中，集群 1 和集群 2 的领导者都位于同一个服务器，实际上，在真实的环境中，可能集群 1 的领导者位于服务器 1，而集群 2 的领导者位于服务器 3，这都是有可能的。大家可以仔细品味品味这个知识点。实际上，这个就是一个非常简单的 **Multi-Raft Group** 例子。当然，我么们的课程并不是围绕着 Multi-Raft Group 来展开的，我们的重点还是基于 raft 算法实现一个 raft 框架，Multi-Raft Group 的知识我会在视频中为大家补充讲解。

这里之所以简单介绍了一下 Multi-Raft Group，是希望能帮助大家形成一种认识：**那就是在 raft 集群中，一个服务器部署多个节点，并且是同一进程启动了这些节点，这种情况并不是不可能发生的。**当然，这时候可能有朋友还是困惑，一个进程怎么启动多个节点？**如果一个进程启动了多个节点，那么这些节点共同使用同一个端口号吗？**没错，确实是这样，当我们使用 Multi-Raft Group 模式构建集群的时候，一个服务器部署多个节点，这些节点会共享这个服务器的 IP 地址和端口号，那在这种情况下，不同节点怎么区分彼此呢？比如说，服务器 1 的集群 2 的领导者需要给服务器 2 的集群 2 的跟随者发送心跳消息，现在的情况是，服务器 2 的两个节点使用同一个端口号，那服务器 1 的集群 2 的领导者的消息到达服务器 2 之后，这条消息怎么才能正确交给集群 2 的跟随者呢？答案已经很明显了，**就是根据集群 Id。因为每一个服务器上不同的节点所属的集群是不同的，所以它们的集群 Id 也各不相同。在领导者向跟随者发送信息的时候，或者是候选者向跟随着发送信息的时候，每一次发送信息，都会把目标节点的集群 Id 封装在请求当中，这样，当目标服务器接收到请求之后，会根据请求中的集群 Id，把请求转发给正确的节点。**如果大家还是觉得有点困惑，可以先简单混个眼熟，接下来的两章我会用代码为大家实现这个过程，到时候还会讲解这个过程。而我现在要为大家引入的这个 NodeId，这个类封装了集群 Id 和节点自身的信息。所以，当一个服务器中部署了多个 raft 节点，就可以使用 NodeId 对象来区分每一个节点。而 NodeId 这个类我也已经定义好了，具体实现请看下面代码块。

```
//节点Id类，该类的对象中封装着当前节点的一些信息
public final class NodeId implements Serializable {

    private static final long serialVersionUID = 4428173460056804264L;

    //当前节点所在集群的id，是个字符串，其实就是我在main函数参数中配置的那个counter
    private final String groupId;
    //当前节点的PeerId
    private final PeerId peerId;
    //缓存当前对象的toString的结果
    private String str;

    public NodeId(String groupId, PeerId peerId) {
        super();
        this.groupId = groupId;
        this.peerId = peerId;
    }

    public String getGroupId() {
        return this.groupId;
    }

    @Override
    public String toString() {
        if (str == null) {
            str = "<" + this.groupId + "/" + this.peerId + ">";
        }
        return str;
    }

    public PeerId getPeerId() {
        return this.peerId;
    }

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + (this.groupId == null ? 0 : this.groupId.hashCode());
        result = prime * result + (this.peerId == null ? 0 : this.peerId.hashCode());
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final NodeId other = (NodeId) obj;
        if (this.groupId == null) {
            if (other.groupId != null) {
                return false;
            }
        } else if (!this.groupId.equals(other.groupId)) {
            return false;
        }
        if (this.peerId == null) {
            return other.peerId == null;
        } else {
            return this.peerId.equals(other.peerId);
        }
    }
}
```

NodeId 也定义好了，现在也应该重构一下 NodeImpl 这个类了，因为我之前说了，NodeId 也是 NodeImpl 类中的一个成员变量。请看下面代码块。

```
public class NodeImpl implements Node{

    private static final Logger LOG = LoggerFactory.getLogger(NodeImpl.class);


    //当前节点所在集群的Id
    private final String groupId;

    //当前节点的Id
    private NodeId nodeId;

    //当前节点的状态
    private volatile State state;

    //当前节点记录的领导者的PeerId
    private PeerId leaderId = new PeerId();

    //当前节点自己的节点信息
    private final PeerId serverId;

    //集群当前生效的配置信息，包括旧的配置信息
    private ConfigurationEntry conf;


    //构造方法
    public NodeImpl() {
        this(null, null);
    }

    //构造方法
    public NodeImpl(final String groupId, final PeerId serverId) {
        //对集群Id判空
        if (groupId != null) {
            Utils.verifyGroupId(groupId);
        }//给集群id成员变量赋值
        this.groupId = groupId;
        //给当前节点的serverId赋值
        this.serverId = serverId != null ? serverId.copy() : null;
        //节点刚刚创建的时候，是未初始化状态
        this.state = State.STATE_UNINITIALIZED;
        LOG.info("The number of active nodes increment to {}.", num);
    }


    //该方法暂时不做实现，这里也不用关心这个NodeOptions
    @Override
    public boolean init(final NodeOptions opts) {
        
    }

    //该方法暂时不做实现
    @Override
    public void shutdown() {

    }

    
}
```

好了，到此为止 NodeImpl 类的简单实现就已经完成了，成员变量以及初始化方法都已经定义了，当然，初始化方法还没有实现。所以接下来，我就要为大家分析，NodeImpl 类的 init 方法究竟该怎么实现。

**引入 NodeOptions，提供节点配置参数**

在上一小节，我为大家引入 NodeImpl 的 init 方法时，大家应该也注意到了，这个方法有一个参数，参数类型为 NodeOptions，我当时跟大家说可以先不用关心这个 NodeOptions。但现在必须得关心一下了，因为现在我们需要初始化 Node 节点了。当然，我说的关心也仅仅是关心一下，因为这个 NodeOptions 类并没有什么特殊的地方，从名字上就能看出来，这个类的对象封装的都是一些配置参数，节点在初始化过程中要用到的配置参数。比如说，raft 节点是通过日志复制来完成数据备份和节点状态一直的，每一条日志都需要持久化，那日志持久化的本地路径肯定需要告诉节点吧？除了日志本地持久化路径，还应该定义什么配置参数呢？有一个配置参数十分重要，那就是节点触发超时选举的时间，这个大家应该都还有印象吧？定义一个超时选举时间，然后在这个时间之上做随机操作。当然，除了这两个之外，NodeOptions 类中还应该定义一个成员变量，那就是 Configuration。为什么要定义这个？原因很简单，因为节点在启动的时候一定有一个最初的集群配置信息，这个配置信息就会被 Configuration 对象封装，然后在节点初始化的过程中，才会把这些配置信息交给 NodeImpl 类中的 ConfigurationEntry 对象来保管。

好了，现在我们已经给 NodeOptions 类定义了三个成员变量了，分别是：**日志本地持久化路径，超时选举时间，封装集群配置信息的对象。**在现阶段，有这三个成员变量就够了。但是，没错，又是一个但是，还需要再定义一个成员变量。**那就是节点元数据信息的持久化路径。**什么是节点元数据信息呢？这个很好解释，既然我们现在是在 raft 共识算法的基础上开发框架，一切就应该遵循 raft 算法本身的规范。大家应该还记得，在上一章我为大家讲解主节点选举的时候，给集群选举做了一个限制，那就是每个节点在每一轮选举中只能给一个节点回复成功响应。现在到了 raft 共识算法中，引入了任期这个概念，**所谓任期，其实就是轮数。**简单来说，就是在每一个任期中，如果发生了领导者选举，那么节点只能在当前任期中给某一个候选者节点投票，只要在当前任期中投过票了，就不能再投票了。可以看出，这个任期的作用和上一章的轮数一摸一样，只不过就是换了一个叫法，干的活都是一样的。这个任期解释完了之后，就该继续解释节点元数据信息了，**所谓节点元数据信息就是当前节点在当前的任期中给哪个节点投过票。**记录下来这一点非常重要，如果一个节点刚刚投了票之后就宕机了，如果没有持久化元信息，重启之后可能会发生重复投票的情况。所以，NodeOptions 类中还应该定义一个节点元信息持久化路径的成员变量。到这里 NodeOptions 也就暂时定义好了，下面就是具体实现，请看下面代码块。

```
//这个类封装的是Node节点初始化时需要用到的配置参数
public class NodeOptions{

    //超时选举时间，默认1000ms
    private int electionTimeoutMs = 1000;

    //集群配置信息对象在这里初始化，刚初始化时配置信息是空的
    private Configuration initialConf = new Configuration();

    //用户配置的存储日志文件的路径
    private String logUri;

    //元数据文件存储路径
    private String raftMetaUri;

    //get、set方法就省略了

}
```

因为刚才引入了任期的概念，还提到了应该记录当前节点在当前任期中为哪个节点投过票，所以 NodeImpl 类中还应该再添加两个成员变量。具体改动请看下面代码块。

```
public class NodeImpl implements Node{

    private static final Logger LOG = LoggerFactory.getLogger(NodeImpl.class);


    //当前节点所在集群的Id
    private final String groupId;

    //当前节点的Id
    private NodeId nodeId;

    //当前节点的状态
    private volatile State state;

    //节点的当前任期
    private long currTerm;

    //当前节点投过票的节点的PeerId，这个成员变量可以记录下来，当前节点为哪个节点投过票
    private PeerId votedId;

    //当前节点记录的领导者的PeerId
    private PeerId leaderId = new PeerId();

    //当前节点自己的节点信息
    private final PeerId serverId;

    //集群当前生效的配置信息，包括旧的配置信息
    private ConfigurationEntry conf;


    //构造方法
    public NodeImpl() {
        this(null, null);
    }

    //构造方法
    public NodeImpl(final String groupId, final PeerId serverId) {
        //对集群Id判空
        if (groupId != null) {
            Utils.verifyGroupId(groupId);
        }//给集群id成员变量赋值
        this.groupId = groupId;
        //给当前节点的serverId赋值
        this.serverId = serverId != null ? serverId.copy() : null;
        //节点刚刚创建的时候，是未初始化状态
        this.state = State.STATE_UNINITIALIZED;
        LOG.info("The number of active nodes increment to {}.", num);
    }


    //该方法暂时不做实现，这里也不用关心这个NodeOptions
    @Override
    public boolean init(final NodeOptions opts) {
        
    }

    //该方法暂时不做实现
    @Override
    public void shutdown() {

    }

    
}
```

进行到这里，启动 raft 集群的前置工作似乎都准备好了，接下来就应该启动集群了。

**启动 raft 集群**

假设我们现在构建的 raft 集群只有 3 个节点，这个集群的 Id 和三个节点的 IP 地址和端口号如下所示。

```
集群Id：group1
节点1：127.0.0.1:8081
节点2：127.0.0.1:8082
节点3：127.0.0.1:8083
```

日志和元数据的持久化路径如下所示。

```
节点1：/Users/chenqingyang/code/jraft/raft1
节点2：/Users/chenqingyang/code/jraft/raft2
节点3：/Users/chenqingyang/code/jraft/raft3

//这里需要注意一下，上面只是一个文件夹路径，比如说为节点1创建了raft1文件夹，还会在该文件夹中
//继续创建持久化日志的文件夹，以及持久化元数据信息的文件夹
//日志和元数据就存放在各自的文件夹中
```

现在需要的配置信息都定义好了，该怎么把这些信息交给每一个节点呢？或者说，该怎么让每一个节点看懂呢？这一点我就直接使用 sofajraft 框架定义好的规则来吧。请看下面代码块。

```
//实际上，在sofajraft中是按照下面这种方式给节点传输配置信息的
{dataPath} {groupId} {serverId} {initConf}
dataPath就是要持久化的数据的持久化路径
groupId就是集群Id
serverId就是当前节点的IP地址和端口号信息
initConf就是集群中所有节点的IP地址和端口号信息

//所以，按照上面的规则，如果要启动三个raft节点，构建一个名为group1的集群，就可以像下面这样给每一个节点传输配置信息
节点1：/Users/chenqingyang/code/jraft/raft1 group1 127.0.0.1:8081 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083
节点2：/Users/chenqingyang/code/jraft/raft2 group1 127.0.0.1:8082 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083
节点3：/Users/chenqingyang/code/jraft/raft3 group1 127.0.0.1:8083 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083
```

那该怎么把这些信息传输给每一个节点呢？一开始我们也没必要整得太麻烦，就用最简单最直接的，给 main 方法传递参数即可。这样节点启动的时候，就可以从 main 方法的参数中获得一个字符串数组，这个字符串数组就存放了全部的配置信息，然后解析数组中的各个字符串即可。就向下面这样。

![](https://cdn.nlark.com/yuque/0/2024/png/26725125/1704794152682-e47b1b81-ee5e-427f-a75c-ab19fac6c8cc.png)

复制三个节点，给每个节点传输配置信息，这样一来，当节点启动的时候，就可以直接通过 main 方法的参数，获得自己需要的配置参数以及集群的配置信息了。

所以，接下来的问题就变成了，该怎么解析 main 方法中的参数。先不急着讨论，让我先为大家把测试类展示出来，我们先看看是怎么回事。这个测试类我就定义为 ServerTest 吧。请看下面代码块。

```
//测试类，可以直接去看main方法，然后再回过头看看ServerTest的成员变量和构造方法
public class ServerTest {


    //集群服务类，这个类就是用来启动集群的，这个类的代码很快就会展示给大家
    private RaftGroupService raftGroupService;

    public ServerTest(final String dataPath, final String groupId, final PeerId serverId,
                      final NodeOptions nodeOptions) throws IOException {

        //根据用户配置的本地路径创建文件夹
        FileUtils.forceMkdir(new File(dataPath));
        //创建RPC服务器，这个服务器的具体实现先不必关心，后两章就会讲到
        final RpcServer rpcServer = RaftRpcServerFactory.createRaftRpcServer(serverId.getEndpoint());
        //设置日志存储文件的本地路径，File.separator就是路径分隔符
        nodeOptions.setLogUri(dataPath + File.separator + "log");
        //设置元数据存储文件的本地路径
        nodeOptions.setRaftMetaUri(dataPath + File.separator + "raft_meta");
        //创建集群服务对象并赋值
        this.raftGroupService = new RaftGroupService(groupId, serverId, nodeOptions, rpcServer);

    }
    

    public static void main(String[] args) throws InterruptedException, IOException {

        if (args.length != 4) {
            //如果用户没有传入正确的参数，或者传入的参数不足，就在控制台输出需要的参数形式
            System.out.println("Usage : java com.alipay.sofa.jraft.example.counter.CounterServer {dataPath} {groupId} {serverId} {initConf}");
            System.out.println("Example: java com.alipay.sofa.jraft.example.counter.CounterServer /tmp/server1 counter 127.0.0.1:8081 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083");
            //然后退出程序
            System.exit(1);
        }
        //解析命令行中传进来的字符串
        //解析出文件存放的本地路径
        final String dataPath = args[0];
        //要启动的集群服务的名字
        final String groupId = args[1];
        //当前节点的IP地址和端口号
        final String serverIdStr = args[2];
        //集群中所有节点的IP地址和端口号
        final String initConfStr = args[3];

        //创建的这个对象封装着当前节点需要的配置参数
        final NodeOptions nodeOptions = new NodeOptions();
        //设置超时选举时间，超过这个时间没有接收到领导者发送的信息，就会触发新一轮选举
        nodeOptions.setElectionTimeoutMs(1000);
        
        //创建的PeerId对象是用来封装代表当前服务器节点的信息的
        final PeerId serverId = new PeerId();
        
        //把当前节点的信息，比如IP地址，端口号等等都解析道PeerId对象中
        if (!serverId.parse(serverIdStr)) {
            throw new IllegalArgumentException("Fail to parse serverId:" + serverIdStr);
        }
        //创建集群配置类对象，这个对象中封装着整个集群中的节点信息
        final Configuration initConf = new Configuration();
        //把上面得到的字符串信息解析到该对象中，这时候配置类对象就拥有了集群中所有节点的信息
        if (!initConf.parse(initConfStr)) {
            throw new IllegalArgumentException("Fail to parse initConf:" + initConfStr);
        }
        //把集群初始配置信息设置到nodeOptions对象中
        nodeOptions.setInitialConf(initConf);
        //启动当前节点以及集群服务
        final ServerTest counterServer = new ServerTest(dataPath, groupId, serverId, nodeOptions);
        //通过集群服务类对象启动集群
        counterServer.raftGroupService.start();
    }
}
```

在上面代码块的第 53 行和 59 行，出现了两个 parse 方法，分别是 PeerId 和 Configuration 从字符串中解析节点信息和配置信息的方法。但是这两个方法之前我没有为大家讲解过，现在用到了，所以就展示给大家。请看下面代码块。

首先是 PeerId 类的 parse 方法。

```
public class PeerId implements Serializable {

    private static final long serialVersionUID = 8083529734784884641L;

    private static final Logger LOG = LoggerFactory.getLogger(PeerId.class);

     public static final String IP_ANY = "0.0.0.0";
    
    //当前节点的IP地址和端口号就封装在这个成员变量对象中
    private Endpoint endpoint = new Endpoint(IP_ANY, 0);
    
    //这个是在同样的IP地址下，用来区分集群中不同节点的
    //比如在某个特定环境下，三个节点使用的IP地址是相同的，这时候就可以通过这个idx来区分这三个节点了
    //比如IP地址和端口号都是localhost:8080
    //idx分别为1，2，3这样就区分开了
    //在我们的课程中不会讨论这种情况，实际上，我们应该不会这样来部署自己的程序
    private int idx;
    
    //该类对象的toString结果的缓存
    private String str;
    //节点初始化的时候，默认选举优先级功能是禁止的
    private int priority = ElectionPriority.Disabled;
    //这个成员变量起到一个标识符的作用，具体使用的时候我会为大家讲解的
    public static final PeerId ANY_PEER = new PeerId();


    //省略其他方法

    //该方法是本类最核心的方法，将传入进来的字符串解析为一个PeerId对象
    public boolean parse(final String s) {
        //输入的字符串不能为空，否则直接返回
        if (StringUtils.isEmpty(s)) {
            return false;
        }
        //注意，这里传进来的字符串的形式是IP地址，端口号，idx，优先级
        //默认是按照这种形式来解析字符串的
        //但是，我在测试类中传进来的都是 127.0.0.1:8081 这样的地址，并没有idx和当前节点的优先级
        //这里大家要弄清楚，idx和优先级都是可以被配置的，只不过我并没有设置而已
        //下面就是把字符串解析成字符串数组了
        final String[] tmps = Utils.parsePeerId(s);
        //如果字符串数组的长度小于2或者大于4则返回false
        //小于二则说明用户配置的节点信息不完整，最少要有IP地址和端口号吧，所以长度最小为2
        //如果idx和优先级也都配置了，那么最大长度就为4，大于4显然也不正确
        if (tmps.length < 2 || tmps.length > 4) {
            return false;
        }
        try {
            //把端口号转化为int
            final int port = Integer.parseInt(tmps[1]);
            //创建Endpoint对象封装IP地址和端口号信息
            this.endpoint = new Endpoint(tmps[0], port);
            //开始判断数组的长度了
            switch (tmps.length) {
                case 3:
                    //如果数组长度为3，说明用户配置了idx，给idx赋值即可
                    this.idx = Integer.parseInt(tmps[2]);
                    break;
                case 4:
                    //为4则说明优先级和idx都被配置了，先判断了一下idx是否为空
                    if (tmps[2].equals("")) {
                        //为空则赋值为0
                        this.idx = 0;
                    } else {
                        //不为空则直接给idx赋值
                        this.idx = Integer.parseInt(tmps[2]);
                    }
                    //给优先级赋值
                    this.priority = Integer.parseInt(tmps[3]);
                    break;
                default:
                    break;
            }
            this.str = null;
            return true;
        } catch (final Exception e) {
            LOG.error("Parse peer from string failed: {}.", s, e);
            return false;
        }
    }
}
```

接下来是 Configuration 类的 parse 方法。

```
public class Configuration{
    
    private static final Logger LOG = LoggerFactory.getLogger(Configuration.class);


    //存放PeerId的集合，因为集群中各个节点都会被包装成一个个PeerId对象
    //所有PeerId对象都存放在这个集合中，也就意味着Configuration对象获得了
    //集群中所有节点的信息
    private List<PeerId> peers = new ArrayList<>();

    //其他方法省略

    //该方法是本类中最重要的方法，就是在该方法中，会解析用户传递到main方法中的参数，也就是那一堆字符串
    //就是这些数据：127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083
    public boolean parse(final String conf) {
        //如果传入的字符串为空，直接返回false
        if (StringUtils.isBlank(conf)) {
            return false;
        }
        //重置集合的数据
        reset();
        //将长字符串解析为字符串数组
        final String[] peerStrs = StringUtils.split(conf, ',');
        //遍历数组，解析出每一个PeerId对象
        for (String peerStr : peerStrs) {
            //创建一个PeerId对象用来封装每一个节点的信息
            final PeerId peer = new PeerId();
            //把节点信息封装到刚才创建出来的PeerId对象中
            if (peer.parse(peerStr)) {
                addPeer(peer);
            } else {
                //解析失败记录日志
                LOG.error("Fail to parse peer {} in {}, ignore it.", peerStr, conf);
            }
        }
        return true;
    }
}
```

这两个类的 parse 方法展示完毕之后，接下来还有一个类需要为大家简单解释一下，那就是 RaftGroupService 类，之前大家没见过这个类，到这里启动测试类的时候，这个类突然就冒出来了。之所以之前没有讲解这个类，是因为这个类太简单，等到使用的时候再展示也不迟。简而言之，这个类的对象封装的就是集群中的全部信息，包括处于当前集群中的节点的网络地址，当前集群的 Id，当前集群的配置信息，处于当前集群的节点需要的配置参数，以及处于当前集群中的节点本身，当然，还有最重要的一个组件，那就是当前节点使用的服务端。因为一个集群中肯定有多个节点，节点与节点通信肯定需要客户端和服务端的参与，所以，服务端也被定义在这个类中了，客户端先不必关心。因此，这个 RaftGroupService 类可以定义成下面这样。请看下面代码块。

```
//框架的集群服务类，这个类中的start方法，就是启动集群的入口方法
public class RaftGroupService {


    private static final Logger LOG = LoggerFactory.getLogger(RaftGroupService.class);

    //集群服务是否启动的标志
    private volatile boolean started = false;

    //当节点的信息
    private PeerId serverId;

    //当前服务器被抽象为集群的一个节点
    //这个成员变量封装了节点需要的配置参数
    private NodeOptions nodeOptions;

    //节点与节点之间进行通信的RPC服务端
    //这个rpcServer内部实现可以暂时忽略，很快就会讲到
    private RpcServer rpcServer;

    //当前节点所在的组ID，也就是集群ID
    private String groupId;
    //当前节点本身
    private Node node;

	//构造方法
    public RaftGroupService(final String groupId, final PeerId serverId, final NodeOptions nodeOptions,
                            final RpcServer rpcServer) {
        this(groupId, serverId, nodeOptions, rpcServer, false);
    }

    public RaftGroupService(final String groupId, final PeerId serverId, final NodeOptions nodeOptions,
                            final RpcServer rpcServer, final boolean sharedRpcServer) {
        super();
        this.groupId = groupId;
        this.serverId = serverId;
        this.nodeOptions = nodeOptions;
        this.rpcServer = rpcServer;
        this.sharedRpcServer = sharedRpcServer;
    }


    //启动当前节点以及节点所在集群的方法
    public synchronized Node start() {
        return start(true);
    }

    
    //启动当前节点以及节点所在集群的方法
    public synchronized Node start(final boolean startRpcServer) {
        //判断集群服务是否已经启动过了
        if (this.started) {
            return this.node;
        }
        //对节点ID和集群ID做非空检验
        if (this.serverId == null || this.serverId.getEndpoint() == null
                || this.serverId.getEndpoint().equals(new Endpoint(Utils.IP_ANY, 0))) {
            throw new IllegalArgumentException("Blank serverId:" + this.serverId);
        }
        if (StringUtils.isBlank(this.groupId)) {
            throw new IllegalArgumentException("Blank group id:" + this.groupId);
        }
        //创建当前服务器代表的节点，在这里节点被真正创建了！！！
        this.node = createAndInitRaftNode(this.groupId, this.serverId, this.nodeOptions);
        if (startRpcServer) {
            //启动RPC服务端
            this.rpcServer.init(null);
        } else {
            LOG.warn("RPC server is not started in RaftGroupService.");
        }
        this.started = true;
        LOG.info("Start the RaftGroupService successfully.");
        return this.no
	}


    public Node createAndInitRaftNode(final String groupId, final PeerId serverId, final NodeOptions opts) {
        final Node ret = new NodeImpl(groupId, serverId);
         //初始化节点,节点在这里初始化了！！！
        if (!ret.init(opts)) {
            throw new IllegalStateException("Fail to init node, please see the logs to find the reason.");
        }
        return ret;
    }

}
```

现在，我再把测试类给大家搬运到这里，大家应该就能完全看懂测试类的执行流程了。请看下面代码块。

```
//测试类，可以直接去看main方法，然后再回过头看看ServerTest的成员变量和构造方法
public class ServerTest {


    //集群服务类，这个类就是用来启动集群的，这个类的代码很快就会展示给大家
    private RaftGroupService raftGroupService;

    public ServerTest(final String dataPath, final String groupId, final PeerId serverId,
                      final NodeOptions nodeOptions) throws IOException {

        //根据用户配置的本地路径创建文件夹
        FileUtils.forceMkdir(new File(dataPath));
        //创建RPC服务器，这个服务器的具体实现先不必关心，后两章就会讲到
        final RpcServer rpcServer = RaftRpcServerFactory.createRaftRpcServer(serverId.getEndpoint());
        //设置日志存储文件的本地路径，File.separator就是路径分隔符
        nodeOptions.setLogUri(dataPath + File.separator + "log");
        //设置元数据存储文件的本地路径
        nodeOptions.setRaftMetaUri(dataPath + File.separator + "raft_meta");
        //创建集群服务对象并赋值
        this.raftGroupService = new RaftGroupService(groupId, serverId, nodeOptions, rpcServer);

    }
    

    public static void main(String[] args) throws InterruptedException, IOException {

        if (args.length != 4) {
            //如果用户没有传入正确的参数，或者传入的参数不足，就在控制台输出需要的参数形式
            System.out.println("Usage : java com.alipay.sofa.jraft.example.counter.CounterServer {dataPath} {groupId} {serverId} {initConf}");
            System.out.println("Example: java com.alipay.sofa.jraft.example.counter.CounterServer /tmp/server1 counter 127.0.0.1:8081 127.0.0.1:8081,127.0.0.1:8082,127.0.0.1:8083");
            //然后退出程序
            System.exit(1);
        }
        //解析命令行中传进来的字符串
        //解析出文件存放的本地路径
        final String dataPath = args[0];
        //要启动的集群服务的名字
        final String groupId = args[1];
        //当前节点的IP地址和端口号
        final String serverIdStr = args[2];
        //集群中所有节点的IP地址和端口号
        final String initConfStr = args[3];

        //创建的这个对象封装着当前节点需要的配置参数
        final NodeOptions nodeOptions = new NodeOptions();
        //设置超时选举时间，超过这个时间没有接收到领导者发送的信息，就会触发新一轮选举
        nodeOptions.setElectionTimeoutMs(1000);
        
        //创建的PeerId对象是用来封装代表当前服务器节点的信息的
        final PeerId serverId = new PeerId();
        
        //把当前节点的信息，比如IP地址，端口号等等都解析道PeerId对象中
        if (!serverId.parse(serverIdStr)) {
            throw new IllegalArgumentException("Fail to parse serverId:" + serverIdStr);
        }
        //创建集群配置类对象，这个对象中封装着整个集群中的节点信息
        final Configuration initConf = new Configuration();
        //把上面得到的字符串信息解析到该对象中，这时候配置类对象就拥有了集群中所有节点的信息
        if (!initConf.parse(initConfStr)) {
            throw new IllegalArgumentException("Fail to parse initConf:" + initConfStr);
        }
        //把集群初始配置信息设置到nodeOptions对象中
        nodeOptions.setInitialConf(initConf);
        //启动当前节点以及集群服务
        final ServerTest counterServer = new ServerTest(dataPath, groupId, serverId, nodeOptions);
        //通过集群服务类对象启动集群
        counterServer.raftGroupService.start();
    }
}
```

上面代码块的执行流程我就不再重复讲解了，代码中注释非常详细。到此为止，节点启动前的准备工作我就给大家全部讲解完毕了。现在，程序经过一系列方法调用，最终就会执行到 raftGroupService 成员变量的 start 方法，在 start 方法中会创建 Node 节点，然后初始化这个节点，但是，现在 NodeImpl 类的 init 方法还没有被我们实现。程序就会卡在这里，无法继续向下运行了。接下来，我们的重点就是要实现这个 init 方法，不过这是下一章的内容了，这一章肯定讲不完了。好了诸位，我们下一章见。