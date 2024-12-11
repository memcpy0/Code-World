大家好，从零带你写框架的第五门课程正式开始更新，这是我的从零带你写框架系列中分布式模块的课程，本课程更新完毕之后，会紧接着更新从零手写 Nacos、RocketMq 这两门课程。如果从类和接口的数量上来说，SOFA-JRAFT 框架本身体量不大，但要从框架的内容来说，学习这个框架还是有一定难度的。抛开 RAFT 共识算法不说，要想学习这个框架，至少还需要掌握时间轮、对象池以及 Disruptor 框架的运用等等知识。当然，我的从零带你写框架系列的前三门课程已经包含了这些知识，如果还有朋友没有学习前三门课，可以先把前三门课程学习完毕后，再来学习 SOFA-JRAFT 框架。

当然，也许有的朋友可能会觉得奇怪，为什么这个框架要在 RAFT 算法的名字前面加一个 J，因为这个 SOFA-JRAFT 框架是用 Java 语言开发的，这就是唯一的原因。好了，让我们尽快回到 SOFA-JRAFT 框架本身吧，剖析框架各个组件的作用非常容易，但是把框架中每行代码的用意讲清楚就没那么容易了，再加上整个框架其实是建立在 RAFT 这个共识算法之上，可以说，这个算法是这个框架的理论基础。所以，要想掌握这个框架，势必要对 RAFT 共识算法有很深的理解。但是，坦诚地说，对理论的讲解并不是我的强项，因为我自己的在阅读一些理论书籍的时候，也会觉得无比枯燥，都是耐着性子把书读完，有时候甚至就读不完，读一个大概就赶紧去撸代码了，我都是在实践代码的过程中逐步形成我自己对理论的一套理解。再说，空讲理论，而没有去代码中实践，无异于纸上谈兵，也只是浪费时间。但是，话又说回来，脱离了理论直接去代码中实践，不但会走一些弯路，还会让我们的知识散乱不成体系，好像我为大家讲解的知识点都是临时想到的。所以，我还是更愿意保持我以往的风格，以一个小例子打开整个课程的局面，在例子中展示代码，由代码引出要讲解的理论。我想，和纯粹地阅读《深入理解分布式系统》或者其他一些严肃的理论书籍，或者是直接去研究 RAFT 共识算法论文比起来，我的这种讲解方式，也许更有助于大家对知识的理解和掌握。

当我想要以一个小例子开始我们的课程时，我其实是很开心的，因为随着这门课程的更新，我的从零带你写框架这个系列的进度也会进一步完善，这也就意味着我可以继续涨价了，已经有很长时间没涨价了，是时候涨一波了。当然，涨价之后，我也可以挣更多的钱，挣更多的前就意味着我可以攒更多的钱。既然是要攒更多的钱，当然就应该背着老婆攒，否则攒钱没有乐趣，也就称不上是真正的攒钱。也许到了人生的某个时刻，很多朋友就会跟我一样，会在背着媳妇攒钱这项活动中获得前所未有的乐趣。就比如说吧，在双十二前两天，媳妇希望我送她一份 500 多块钱的护肤品，然后我就拿出我的小米手机、华为手机和苹果手机给她看。在媳妇再三确认这三个手机的存款加起来不超过 750 元之后，不得不打消了让我送她礼物的念头。因为平时我的大部分钱都存到她的小金库里了，所以我当时表现得很委屈，以为能借此机会从她那里要点钱，但她对此表现得漠不关心，这就让人有点恼火了。

当时我脑海里就有一个想法，我要用自己的私藏的小金库里的钱买一个 3300 块钱的 4060Ti 显卡，并且在收到显卡后一定要去她面前哈哈大笑十几分钟。后来我真的就是这么做的，只不过跟预想的稍微有点不同，我拿到显卡之后还没笑呢，就被她按在沙发上拷问了十几分钟。我的回答跟以前一样，我没有小金库，我就只有一点零用钱，显卡是 320 块钱买的。总之，当她开始拷问我的时候，我就可以装傻了，如果她继续咄咄逼人，我也会适当地激怒她，比如我会偶尔说这么一两句话：有时候我的前女友们也会给我钱花；你以为我只买了这一件东西吗？其实我还买了一把 1650 块钱的键盘，被我藏起来了，能找到算你厉害；我身上的钱远远超过你的想象，昨天我刚花 7600 块钱买了一把芬达电吉他，怎么样，气不气？当然，这句是假话，电吉他我也没买，偷偷攒钱很辛苦，花那么多钱买电吉他我也有点心疼。但是，真真假假，虚虚实实，有时候确实需要一些特别的方法来治一治这个脾气很大的恶女人。总之，最后能达到这样一种效果，我就心满意足了，那就是钱我也攒了，也被我花了，媳妇也知道了，但是她无可奈何，只能生闷气。她一生闷气，我内心就有一种快感。

当然，我扯了这么多闲话并不是想跟大家传授我私藏小金库的经验，我只是觉得很开心，因为在 XXL-JOB 更新完毕这么久之后，我又一次要和钱打交道了。我目前的意思很简单，那就是写一个可以累计我挣了多少钱的程序，然后部署在服务器上，每当我卖出去一份课程，小程序就会把挣到的钱加上，当然，当我花出去一笔钱之后，小程序也会从总额中减去相应的数目。简而言之，这个小程序就是我小金库的管理者。当然，现在我的课程还不是很火爆，每天卖个一两份的，没必要专门开发一个小程序专门记录营业额，别说我了，就我媳妇那个智商，也能计算清楚自己每天挣了多少钱(超过十份我就不知道她能不能计算清楚了)。做事情总要未雨绸缪，也许当我的这个系列开始更新 RocketMq 时，就火爆起来了呢？也许是在我开始手写虚拟机时，这个课程的销量会进一步提升，这都是有可能的。好了，现在不是做白日梦的时候，还是先让我把这个简单的小程序实现一下吧。

请看下面代码块。

```
public class NodeImpl{

    //这个成员变量记录的就是我一共挣到了多少钱
    private Integer totalMoney;

    public NodeImpl() {
       
    }

    //每挣到一笔钱，就可以调用这个方法，把钱累加到totalMoney中
    public void saveMoney(Integer amount) {
        totalMoney += amount;
    }


    //得到存款总额地方法
	public Integer getTotalMoney() {
        return totalMoney;
    }
} 
```

这就是我设计的一个最简单的小金库程序，现在我的系列课程售价为 400 元，但总有一天会涨价到 1000 元的，不如现在就按 1000 元来计算。这样的话，每卖出一份课程，就会调用程序的 saveMoney 方法，把挣到的钱累加到 NodeImpl 的 totalMoney 成员变量上，这样一来，就可以记录我一共挣了多少钱了。当然，也许有朋友已经注意到了，上面代码块的类名比较特殊，本来是一个管理小金库的程序，为什么不把类名定义为 SecretMoney 或者其他的名字，偏偏叫什么 NodeImpl。原因很简单，这个程序不是用来记录我媳妇每天花了多少钱，也不是用来记录我有几个前女友，更不是用来记录我每天刷 B 站看了多少女人跳舞，它是用来管理我小金库的！这个程序的存在可以在我忙碌了一整天之后，在上厕所的时候偷偷看一眼迄今为止攒了多少钱时，给我很深的慰藉！确切地说，这个程序记录的金钱数额，给了我莫大的动力，支撑着我每天努力工作。我把这个程序看得如此重要，所以这个程序的稳定性也就不言而喻了。我当然不希望双十一临近的时候，我想从小程序上看看自己的存款够不够买一个 VR 眼镜时，却意外地发现程序崩溃了，程序一旦崩溃就意味着不能继续工作，如果这时候有朋友买课，那么挣到的钱就无法记录到总营业额中了，这部分数据就永远丢失了，同时也意味着以前的数据也可能丢失了。当然，有人可能会建议我把数据及时落盘，存储到硬盘之后，即便服务器崩溃了，那也只是暂时的，重启服务器之后就可以重新把数据从硬盘加载到内存中了。这当然是一个方法，但是，假如服务器的硬盘也损坏了呢，只有一份数据显然是无法令我安心的。

最稳妥的方法是，把相同的程序分别部署在多台服务器上，比如我现在有三台服务器，这三台服务器上都部署了这个小程序。我会从这三个服务器中指定一个作为这三台服务器的主节点，剩下的两台服务器作为从节点，既然是节点，所以我就直接把类名定义为了 NodeImpl。在这三个节点中，只有主节点可以执行累加营业额的操作，与此同时，主节点也会及时把自己的营业额数值，也就是 totalMoney 成员变量传送给两个从节点，从节点在接收到这个数值后会更新自己的 totalMoney 成员变量，也会把数据进行持久化。这样一来，我就运用最简单的主从复制的知识，构建了一个小小的集群，增加了数据的备份，之前只有一份数据，现在有三份数据存储在三个不同的服务器上。并且，当主节点突然无法工作后，我可以立刻选取一个从节点成为主节点，接替原来的主节点继续工作，也许可以做到无缝衔接。当然，具体的细节我还没有想好，**比如主节点的数据怎么向从节点传输，主节点宕机了怎么选举新的主节点，能不能多个节点同时执行累加营业额的操作，也就是集群中有多个主节点，或者干脆让从节点也可以处理业务，主节点的数据向从节点传输失败了该怎么办？**可以看到，确实有很多问题等待我解决，显然我给自己找了一个大麻烦，可开发程序就是这样，你越想做到完美，越想使程序尽量不会出问题，就必须先解决很多问题。不过还是让我们先退一步吧，因为现在还不是解决这些问题的时候，我心里很清楚，现在最主要的问题，就是把之前的小程序尽快重构一下。

**在集群模式下，每个节点都拥有自己的身份，虽然每个程序的代码是一模一样的，但是每个节点会根据自己的身份执行不同的代码，首先把每个节点的身份定义出来，这一点才是当务之急。解决了这个问题之后，还有一个问题急需解决，那就是目前的程序过于简单了。只能进行累加营业额的操作，如果我花了一笔钱，这个小金库该怎么减去这部分钱呢？显然应该给每一个操作定义一个指令，然后主节点根据操作的指令判断是该进行累加营业额还是减去花销，同时还应该给程序定义接收从主节点传过来的数据的方法，或者就直接给每个节点定义接收指令和应用指令的方法。**

很好，现在重构程序的思路已经明确了，可以给我们的程序先定义几个新的类。既然刚才说到了集群中节点的身份，那我就首先定义一个枚举类吧，这个枚举类的对象代表的就是集群中节点的身份。请看下面代码块。

```
//节点状态枚举类
public enum State {
    //当前节点为主节点
    STATE_MASTER,
    //当前节点为从节点
    STATE_SLAVE;
}
```

节点身份枚举类定义好了，接下来我还要定义一个指令枚举类，这个枚举类的对象代表的就是不同的业务指令。请看下面代码块。

```
//指令枚举类
public enum CommandType {
    //这三个枚举类对象分别表示一个指令
    //add表示累加营业额
    ADD,
    //sub表示减去花销
    SUB,
    //get表示用户想知道目前的总营业额是多少
    GET;

    //把指令转换为对应的字节
    public byte toByte() {
        switch (this) {
            case ADD:
                return (byte) 0;
            case SUB:
                return (byte) 1;
            case GET:
                return (byte) 2;
        }
        throw new IllegalArgumentException();
    }

    //根据字节解析指令
    public static CommandType parseByte(byte b) {
        switch (b) {
            case 0:
                return ADD;
            case 1:
                return SUB;
            case 2:
                return GET;
        }
        throw new IllegalArgumentException();
    }

}
```

然后再创建一个 Task 类，这个类的对象就是用来包装客户端发送给主节点的请求信息的，这个请求信息中包含了要对主节点进行的指令操作，以及要操作的金钱的数额。请看下面代码块。

```
//包装客户端请求的类
public class Task implements Serializable {

    //本次操作的指令
    private byte command;
    //要操作的金钱的数额
    private Integer money;

    public Task() {

    }

    public Task(byte command, Integer money) {
        this.command = command;
        this.money = money;
    }

    public byte getCommand() {
        return command;
    }

    public void setCommand(byte command) {
        this.command = command;
    }

    public Integer getMoney() {
        return this.money;
    }

    public void setMoney(Integer money) {
        this.money = money;
    }

}
```

当然，还有一个类也要定义一下，那就是封装节点 IP 地址和端口号信息的类。因为主节点要把数据复制给从节点，所以肯定也要知道集群中其他节点的 IP 地址才能进行远程通信呀。我把这个类定义为了 Endpoint，请看下面代码块。

```
//存储节点IP地址的类
public class Endpoint implements Serializable {

    private static final long serialVersionUID = -7329681263115546100L;
    //初始化的ip地址
    private String ip;
    //端口号
    private int port;
    //ip地址端口号的字符串形式
    private String str;

    public Endpoint() {
        super();
    }

    public Endpoint(String address, int port) {
        this.ip = address;
        this.port = port;
    }

    public String getIp() {
        return this.ip;
    }

    public int getPort() {
        return this.port;
    }

    //深拷贝方法
    public Endpoint copy() {
        return new Endpoint(this.ip, this.port);
    }


    @Override
    public String toString() {
        if (str == null) {
            str = this.ip + ":" + this.port;
        }
        return str;
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

最后就是 NodeImpl 类的重构了，这一次重构，我在这个类中定义了几个新的成员变量，添加了应用客户端指令的方法，把数据传输给从节点的方法，具体实现请看下面代码块。

```
//节点实现类
public class NodeImpl{

    //这个成员变量记录的就是我一共挣到了多少钱
    private Integer totalMoney;
    
    //当前节点的状态，是主节点还是从节点
    private State state;
    
    //记录集群中所有节点IP地址的成员变量
    List<Endpoint> list = new ArrayList<>();
    
    //记录集群中主节点的IP地址
    Endpoint master;
    
    //当前节点自己的IP地址
    Endpoint ip;


    public NodeImpl() {

    }

    //构造方法
    public NodeImpl(State state, List<Endpoint> list, Endpoint ip) {
        this.state = state;
        this.list = list;
        this.ip = ip;
    }

    //应用客户端指令的方法，客户端发送过来的指令会被这个方法处理
    //假装已经经过反序列化了，这里直接就可以得到反序列化之后的Task
    public void onApply(Task task) {
        if (this.state == STATE_MASTER) {
            //直接开始执行指令
            execute(task);
            //执行完指令之后把指令发送给从节点
            sendCommandToSlave(task);
        }
    }


    //真正执行指令的方法
    private void execute(Task task) {
        //得到要操作的数据
        Integer money = task.getMoney();
        //解析指令
        CommandType commandType = CommandType.parseByte(task.getCommand());
        switch (commandType) {
            case ADD:
                totalMoney += money;
                break;
            case SUB:
                totalMoney -= money;
                break;
            case GET:
                //如果是主节点才执行这个操作，从节点就不必执行了，否则指令已经在主节点应用过了
                //没必要从节点再返回给用户一次营业总额
                if (this.state == STATE_MASTER) {
                    System.out.println("在另一个线程中把营业总额返回给用户！");
                }
                break;
        }
    }


     //主节点向从节点发送消息的方法
    private void sendCommandToSlave(Task task) {
        for (int i = 0; i <list.size() ; i++) {
            Endpoint endpoint = list.get(i);
            if (endpoint.equals(ip)) {
                //如果当前遍历到的节点信息和当前节点信息相等，就直接跳过本次循环
                //不可能自己给自己发送消息
                continue;
            }
            String ip = endpoint.toString();
            System.out.println("主节点向从节点" + ip +"发送了消息");
        }
    }
    

    //从节点接收主节点传递过来的指令的方法
    private void acceptCommand(Task task) {
        //如果当前节点是从节点，就开始执行指令
        if (this.state == STATE_SLAVE) {
            execute(task);
        }
    }

   


    //得到存款总额地方法
    private Integer getTotalMoney() {
        return totalMoney;
    }

    //假装是启动节点服务器的方法
    public void start() {
        System.out.println("启动当前节点服务器，当前节点可以收发消息了！");
    }
}
```

一下子展示了这么多类，可能很多朋友一时间理解不过来，所以接下来我为大家准备了一个测试例子，为大家梳理一下目前这个小程序的执行流程。当然，大家应该也能看到，上面展示的这几个类非常简陋，还夹杂着一些伪代码，更没有 RPC 通信机制的代码，这一点请大家理解一下，毕竟这只是一个小例子，后面的章节会给大家提供工业级别的正式代码。请大家先看下面的测试类代码。

```
//测试类
public class Test {
    
	//封装集群节点配置信息的集合
    private static List<Endpoint> list;

    //创建三个节点信息对象
    private static Endpoint ip1 =  new Endpoint("127.0.0.1", 8080);
    private static Endpoint ip2 =  new Endpoint("127.0.0.1", 8081);
    private static Endpoint ip3 =  new Endpoint("127.0.0.1", 8082);

    //把节点配置添加到集合中
    static {
        list.add(ip1);
        list.add(ip2);
        list.add(ip3);
    }

    public static void main(String[] args) {
       
        //创建第一个节点实现类，把这个节点设置为主节点
        NodeImpl node1 = new NodeImpl(State.STATE_MASTER,list,ip1);
        //假装启动了节点，实际上根本没有RPC机制。。
        node1.start();
        
        //创建两个从节点
        NodeImpl node2 = new NodeImpl(State.STATE_SLAVE,list,ip2);
        node2.start();
        NodeImpl node3 = new NodeImpl(State.STATE_SLAVE,list,ip3);
        node3.start();
    }
}
```

很好，到此为止集群中的三个节点就已经启动了，这也就意味着主节点可以接收来自客户端的指令消息，而两个从节点就可以接收来自主节点的消息。当客户端向主节点发送消息时，消息内容会被包装在一个 Task 对象中，这个对象内部封装这本次操作的指令和要操作的金额数值，主节点会通过 onApply 方法接收到这条消息，也就是下面的这个方法。

```
//因为代码中并没有提供RPC机制，所以就请大家当作这个消息传送到主节点的这个方法时，已经被反序列化了
//这样就可以直接从task对象中获得对应的指令和操作数了
public void onApply(Task task) {
	//这里判断一下当前节点是不是主节点，如果是主节点，才进一步执行指令
    if (this.state == STATE_MASTER) {
        //直接开始执行指令
        execute(task);
        //执行完指令之后把指令发送给从节点
        sendCommandToSlave(task);
    }
}

//主节点执行指令时会调用下面这个方法
//真正执行指令的方法
    private void execute(Task task) {
        //得到要操作的数据
        Integer money = task.getMoney();
        //解析指令
        CommandType commandType = CommandType.parseByte(task.getCommand());
        switch (commandType) {
            case ADD:
                totalMoney += money;
                break;
            case SUB:
                totalMoney -= money;
                break;
            case GET:
                //如果是主节点才执行这个操作，从节点就不必执行了，否则指令已经在主节点应用过了
                //没必要从节点再返回给用户一次营业总额
                if (this.state == STATE_MASTER) {
                    System.out.println("在另一个线程中把营业总额返回给用户！");
                }
                break;
        }
    }
```

当主节点把指令执行完毕后，主节点的总营业额数值已经被更新了，然后再通过 sendCommandToSlave 方法把这条指令发送给两个从节点，让两个从节点执行指令，从节点会通过 acceptCommand 方法接收来自主节点的消息，也就是下面代码块中的方法。

```
//从节点接收主节点传递过来的指令的方法
private void acceptCommand(Task task) {
	//在这个方法中，会先判断一下当前节点是不是从节点，如果是，再执行指令
    //如果当前节点是从节点，就开始执行指令
    if (this.state == STATE_SLAVE) {
        execute(task);
    }
}
```

这时候，两个从节点的数据也就更新成功了。当然数据持久化的功能还没有实现，我也不准备在这个简单的例子中实现持久化了，一般来说，可以采用同步或者异步的方式进行持久化，在这个例子中，我们就当作有一个异步线程在一直持久化数据吧，也就是把总的营业额落盘。

到此为止，我就使用简单的主从复制，实现了数据的多重备份，并且使用的是单主复制模式。如果把程序开发到这种程度就要结束了，那我以后肯定没脸在别人面前说我是一个有修养的程序员，而是自我介绍为码农。我认为我是一个自我修养很高的程序员，所以，我应该有这样一种意识和能力，那就是不断迭代自己的程序，使自己的程序越来越健壮，然后再追求程序性能的提升。那么，目前为止，我开发的这个小程序有什么问题呢？当然有，还记得重构程序之前我提出的那几个问题吗？接下来，我们一个一个地分析。

**主节点地数据怎么向从节点传输**

**首先是主节点地数据怎么向从节点传输**，剖析清楚这一点对我来说十分重要。因为我一开始引入集群，配置了三个节点，就是希望程序中的数据可以多几个备份，但是备份的方式，是不是可以再研究研究呢？数据备份的本质不就是主节点拥有什么数据，从节点就要拥有什么数据，或者主节点的数据是什么，从节点的数据就要是什么吗？既然是这样，我又看了看自己的程序，发现我的程序其实最终要备份的数据就只有一个总的营业额，也就是 totalMoney。如果是这样的话，其实在主节点的 totalMoney 每一次更新之后，直接把这个 totalMoney 发送给两个从节点不就好了，这样从节点就直接获得了最新的值，就可以直接给自己的 totalMoney 成员变量赋最新值了，再也不用执行什么指令了。这样岂不是更方便？而我现在的程序是怎么做的？主节点在应用了客户端发送过来的指令后，再把 Task 对象发送给两个从节点，然后从节点执行指令，显然是有点麻烦，远不如直接传输 totalMoney 的最新值给从节点来的干脆。

那我是不是就可以按照刚才的思路重构我的程序了？显然还不可以，因为我还没能确定主节点向从节点传输 totalMoney 成员变量的频率。让我们思考一个场景，当我的课程售卖得并不频繁的时候，每售卖一份，客户端就会向主节点发送一条指令，来更新主节点的 totalMoney 成员变量。主节点更新了 totalMoney 成员变量之后，可以立刻把这个更新后的值传输给两个从节点，这是没问题的。但是，假如我课程卖得十分频繁呢？比如一分钟内卖了 1000 份(开个玩笑哈)，那主节点在一分钟内不仅要频繁更新 totalMoney 得值，还要在每次更新完 totalMoney 之后，把 totalMoney 发送给两个从节点。这岂不是更麻烦，反正主节点只需要把 totalMoney 成员变量的最新值传输给从节点就行了，为什么不在主节点的 totalMoney 最新值更新完毕后再传输给从节点呢？也就是说，当这一分钟过去了，没人买课了，这时候再把 totalMoney 传输给两个从节点。这样一来，只需要传输一次就好了，比每更新一次 totalMoney 就传输给从节点一次省事多了。这么做挺好的，但很快我就意识到我不能这么做，因为一旦主节点在这一分钟内突然故障宕机了，那 totalMoney 数据不管更新了多少，只要没有异步落盘成功，就全丢失了。而这时候从节点还没有接收到主节点传输的数据，所以这些已经更新的数据就算是永远丢失了。由此可见，**最稳妥的做法仍然是主节点的数据每更新一次，就向从节点传输一次。**

好了，麻烦就麻烦点吧，反正每次主节点的 totalMoney 一旦更新了，就传输给从节点呗。那现在可以着手重构程序了吧？很抱歉，目前还是不可以，原因也很简单，我只顾着自己爽了，没有考虑到一个程序开发最基本得要求，那就是一个优秀的程序，需要有很广泛的适用性或者是扩展性，我不可能开发一个框架，这个框架只能计算我售卖课程的总营业额。私藏小金库当然很有意思，但程序的功能如此单一，这么做既没什么意思，对我的三台服务器也是一种浪费。

假如说，我现在又希望我的这个程序并不仅仅局限于计算营业总额，而是增添了一个 Map 功能，这个 Map 记录的是我用私房钱偷偷购买的物品藏在什么位置了，或者是前女友很久之前送我的礼物，这个 Map 记录的就是我的秘密物品藏身位置。具体逻辑请看下面。

首先 Task 类应该重构一下，重构后的 Task 类中增加了两个成员变量，分别是 stuff 和 place。请看下面代码块。

```
public class Task implements Serializable {

    //本次操作的指令
    private byte command;
    //物品
    private String stuff;
    //物品存放位置
    private String place;
    //要操作的金钱的数额
    private Integer money;

    public Task() {

    }


    public Task(byte command, String stuff, String place, Integer money) {
        this.command = command;
        this.stuff = stuff;
        this.place = place;
        this.money = money;
    }

    public byte getCommand() {
        return command;
    }

    public void setCommand(byte command) {
        this.command = command;
    }

    public Integer getMoney() {
        return this.money;
    }

    public void setMoney(Integer money) {
        this.money = money;
    }

    public String getStuff() {
        return stuff;
    }

    public void setStuff(String stuff) {
        this.stuff = stuff;
    }

    public String getPlace() {
        return place;
    }

    public void setPlace(String place) {
        this.place = place;
    }
}
```

紧接着是 NodeImpl 类的重构，重构后的 NodeImpl 类增加了一个 map 成员变量，execute 执行指令的方法有了一些改变。

```
//节点实现类
public class NodeImpl{

    //这个成员变量记录的就是我一共挣到了多少钱
    private Integer totalMoney;
    
    //记录每一个私人物品藏在哪个位置了
    private Map<String,String> map = new HashMap<>();
    
    //当前节点的状态，是主节点还是从节点
    private State state;
    
    //记录集群中所有节点IP地址的成员变量
    List<Endpoint> list = new ArrayList<>();
    
    //记录集群中主节点的IP地址
    Endpoint master;
    
    //当前节点自己的IP地址
    Endpoint ip;


    public NodeImpl() {

    }

    //构造方法
    public NodeImpl(State state, List<Endpoint> list, Endpoint ip) {
        this.state = state;
        this.list = list;
        this.ip = ip;
    }

    //应用客户端指令的方法，客户端发送过来的指令会被这个方法处理
    //假装已经经过反序列化了，这里直接就可以得到反序列化之后的Task
    public void onApply(Task task) {
        if (this.state == STATE_MASTER) {
            //直接开始执行指令
            execute(task);
            //执行完指令之后把指令发送给从节点
            //注意，这时候发送数据就要把map也发送给从节点了
            //map的数据也需要做备份,这里就写成这种伪代码吧
            sendCommandToSlave(task,map);
        }
    }


    //真正执行指令的方法
    private void execute(Task task) {
        //得到要操作的数据
        Integer money = task.getMoney();
        //如果money为0，说明本次客户端请求并不是更新总营业额
        //而是记录物品存放位置
        if (money != null && money == 0) {
            //得到物品
            String stuff = task.getStuff();
            //得到要存放的位置
            String place = task.getPlace();
            //记录物品存放位置
            map.put(stuff,place);
            //直接退出方法，不执行后续代码
            return;
        }
        //解析指令，这里就不考虑既要操作营业总额，又要记录物品存放位置的指令了
        CommandType commandType = CommandType.parseByte(task.getCommand());
        switch (commandType) {
            case ADD:
                totalMoney += money;
                break;
            case SUB:
                totalMoney -= money;
                break;
            case GET:
                //如果是主节点才执行这个操作，从节点就不必执行了，否则指令已经在主节点应用过了
                //没必要从节点再返回给用户一次营业总额
                if (this.state == STATE_MASTER) {
                    System.out.println("在另一个线程中把营业总额返回给用户！");
                }
                break;
        }
    }

    //省略其他方法
}
```

好了，现在程序又一次重构好了，并且实现了记录私人物品存放位置的功能。比如客户端发送过来一个 Task，这个 Task 对象中并没有给 command 和 money 赋值，只是封装了一件物品要存放位置的信息。那么主节点通过 onApply 方法接收到这个请求后，就会把物品和物品要存放的位置存放到 NodeImpl 的 map 成员变量中。比如说我连续向 NodeImpl 的 map 中记录了一些信息，就像下面代码块展示的这样。

```
map.put("上午挣到的1000元","藏到电脑机箱里吧");
map.put("下午挣到的1000元","藏到现代编译原理这本书里吧");
map.put("偷偷买的VR眼镜","藏到电吉他的琴箱里吧");
```

如果是上面这种情况，当 NodeImpl 节点的 map 成员变量中记录的数据还比较少的时候，我确实可以在主节点每一次更新 map 的时候，把 map 中存放的所有最新数据都发送给两个从节点。但是，如果 map 中的数据特别多呢？就比如接连好几天都向这个 map 中存放了很多数据，就像下面这样。

```
map.put("上午挣的1000元","藏到深度探索C++对象模型这本书里吧");
map.put("中午挣的1000元","藏到程序员的自我修养这本书里吧");
。。。。。。
。。。。。。
。。。。。。
map.put("前女友们的相片","这个就贴身带着吧");
map.put("前女友们的相片","衣服要洗了，这个暂时藏到书包里吧");
map.put("前女友们送我的礼物","藏到鞋盒里吧");
map.put("前女友们留给我的礼物","被媳妇发现了，逼我把礼物都扔，就扔到垃圾箱里吧");
map.put("前女友们留给的礼物","假装扔到垃圾箱里，实际上把礼物交给最信任的朋友保管了");
。。。。。。
。。。。。。
。。。。。。
map.put("我自己的手机","敲代码太入迷，忘了回复媳妇消息，为了不让媳妇生气，把手机藏到电脑机箱下面，就说找不着了");
。。。。。。
。。。。。。
。。。。。。
```

可以看到，在上面的代码块中，map 中存放的数据越来越多，难道在 map 中存放了成千上万条消息后，每一次更新主节点的 map，都要把 map 中的全部数据都发送给从节点吗？显然不能这么做，绕了个弯子，我发现最好的方法仍然是当主节点每接收到一条客户端的指令之后，就把这条指令传输给从节点。**实际上，只需要让从节点主节点都干了什么，然后让从节点也执行相应的操作，那么主从节点的数据一定就是一致的，这样也就做到了数据备份。**所以，兜兜转转，我决定不在传输数据的层面对程序进行重构了，最终还是保持了原来的方式，**每当主节点接收到客户端的一条指令，就把这条指令发送给两个从节点(批量发送的情况后面再分析)，让从节点自己执行指令，从而达到和主节点数据一直的效果。**当然，还有一种情况需要进一步考虑一下，比如说集群中需要新添加一个从节点，这时候主节点中的 map 已经存放了很多条信息了。这种情况下就可以直接把主节点的数据本身直接传输给从节点，也就是把 totalMoney 和存放着数据的 map 都发送给从节点。等从节点更新了自己的数据之后，再通过主节点传递过来的每一条指令，和主节点的数据保持同步，承担数据备份功能。当然，目前我们还不需要考虑这种情况，文章后面的章节会对这种情况展开详细讲解，并且实现这个功能。现在我考虑的是另一个问题，当主节点向从节点传输指令消息时，传输失败了该怎么办呢？

**处理主节点向从节点传输数据失败的情况**

**当主节点向从节点传输指令消息时，传输失败了该怎么办呢？**考虑这个问题是很有必要，毕竟网络是波动的，很有可能主节点向从节点发送的消息根本没发送出去，也可能消息在网络传输过程中丢失了。所以，要想知道消息是否传输成功了，首先得明确一下，当从节点接收到来自主节点的消息时，需要给主节点回复一个响应。我们先不管这个响应的内容是什么，总之需要给主节点回复一个响应，告诉主节点自己收到消息了。如果主节点迟迟没有收到从节点回复的响应，超过一定时间之后，主节点就可以认为消息发送失败，这时候，我采取的做法就是重新发送刚才的指令，直到主节点接收到从节点的响应了。我想这应该也是比较主流和传统的手段，失败重试，超时重试不都是这种手段吗？既然是这样，还是先让我对 NodeImpl 类做一点小小的改动。请看下面代码块。

```
//节点实现类
public class NodeImpl{

    //这个成员变量记录的就是我一共挣到了多少钱
    private Integer totalMoney;
    
    //记录每一个私人物品藏在哪个位置了
    private Map<String,String> map = new HashMap<>();
    
    //当前节点的状态，是主节点还是从节点
    private State state;
    
    //记录集群中所有节点IP地址的成员变量
    List<Endpoint> list = new ArrayList<>();
    
    //记录集群中主节点的IP地址
    Endpoint master;
    
    //当前节点自己的IP地址
    Endpoint ip;


    public NodeImpl() {

    }

    //构造方法
    public NodeImpl(State state, List<Endpoint> list, Endpoint ip) {
        this.state = state;
        this.list = list;
        this.ip = ip;
    }

   //其他方法省略


     //主节点向从节点发送消息的方法
    private void sendCommandToSlave(Task task) {
        for (int i = 0; i <list.size() ; i++) {
            Endpoint endpoint = list.get(i);
            if (endpoint.equals(ip)) {
                //如果当前遍历到的节点信息和当前节点信息相等，就直接跳过本次循环
                //不可能自己给自己发送消息
                continue;
            }
            String ip = endpoint.toString();
            //这里发送指令给从节点，需要考虑是同步还是异步
            //所谓同步就是要同步等待结果，直接收到响应后才能继续发送下一条指令
            //异步就是不关心是否收到响应，如果接收到响应后，可以在异步线程中执行一些通知操作
            //这里我们就先不关心是同步还是异步了，后面的文章会展开详细讲解
            System.out.println("主节点向从节点" + ip +"发送了消息");
        }
    }
    

    

    //从节点接收主节点传递过来的指令的方法
    //在这个方法中，从节点接收到了来自主节点传输过来的指令
    //需要立即向主节点回复一个响应
    private Response acceptCommand(Task task) {
        //如果当前节点是从节点，就开始执行指令
        if (this.state == STATE_SLAVE) {
            execute(task);
            //伪代码，回复主节点一个响应
       		 return response;
        }

        return null;
    }



    //省略其他方法
}
```

好了，虽然都写成伪代码了，但好在响应是有了，现在终于可以再次启动程序，让客户端给主节点发送一条指令，主节点执行了指令之后，再把这条指令传输给两个从节点，从节点收到指令之后就回复给主节点一个响应，表示自己收到指令了。就这么着，程序运行了一段时间，当然，中间也发生过网络波动，主节点也曾多次向从节点传输发送失败的指令。就是这种愚蠢的操作，给我带来了一个大麻烦。因为有一天我忽然意识到，本来上午只卖了一份课程，只攒了 1000 元，但是主节点中的 totalMoney 成员变量却增加了 3000，这是怎么回事呀？很快我就意识到了，肯定是出于网络原因，消息一直在途中，根本没法送到从节点，或者是从节点回复的响应也在途中耽搁了。总之就是响应超时了，主节点以为自己向从节点发送消息失败了，于是重新发送了消息，可能重试了 3 次，这也就意味着发送了 3 条指令，最后这 3 条指令都被发送到了从节点，从节点接收到消息之后就把相同的指令重复执行了 3 次，于是总的营业额就增加了 3000。我想问题的原因应该是找到了，解决的方法也很简单，这不就是一个最简单的幂等性问题吗？很好办，**我只要在主节点发送每一条指令给从节点的时候，给每一条指令消息都加上一个唯一索引不就好了，这个索引可以是一直自增的。从节点接收到每一条指令消息后都会判断之前是不是接收了这条消息，如果接收了，就不再进行处理。**这样一来，就需要再次对 NodeImpl 类进行重构了，首先应该添加一个代表消息指令索引的成员变量，其次也要对 sendCommandToSlave 和 acceptCommand 方法再进行一些改动。具体逻辑请看下面代码块。

```
//节点实现类
public class NodeImpl{

    //这个成员变量记录的就是我一共挣到了多少钱
    private Integer totalMoney;
    
    //记录每一个私人物品藏在哪个位置了
    private Map<String,String> map = new HashMap<>();
    
    //当前节点的状态，是主节点还是从节点
    private State state;
    
    //记录集群中所有节点IP地址的成员变量
    List<Endpoint> list = new ArrayList<>();
    
    //记录集群中主节点的IP地址
    Endpoint master;
    
    //当前节点自己的IP地址
    Endpoint ip;
	//最后发送或者是接收到的指令的索引
    private long lastIndex;


    public NodeImpl() {

    }

    //构造方法
    public NodeImpl(State state, List<Endpoint> list, Endpoint ip) {
        this.state = state;
        this.list = list;
        this.ip = ip;
    }

   //其他方法省略


     //主节点向从节点发送消息的方法,这个方法做了一些简单的修改
    private void sendCommandToSlave(Task task) {
        //这里发送指令的时候，要先给lastIndex自增，
         this.lastIndex++;
        for (int i = 0; i <list.size() ; i++) {
            Endpoint endpoint = list.get(i);
            if (endpoint.equals(ip)) {
                //如果当前遍历到的节点信息和当前节点信息相等，就直接跳过本次循环
                //不可能自己给自己发送消息
                continue;
            }
            String ip = endpoint.toString();
            //把lastIndex和task一起发送给从节点
           doSendCommandToSlave(task,ip,lastIndex);
        }
    }


    //新添加的，真正发送指令消息给从节点的方法
    //这里需要解释一下，当主节点发送指令失败，要重新发送指令时
    //就直接调用这个方法，这时候index并不会自增了，还是发送失败的指令对应的index
     private void doSendCommandToSlave(Task task,String ip,Long index){
         //暂时不做实现
     }

    

    //从节点接收主节点传递过来的指令的方法
    //这个方法也做了一点改动
    private Response acceptCommand(Long index,Task task) {
        //如果当前节点是从节点，就开始执行指令
        if (this.state == STATE_SLAVE) {
            //在这里再判断一下接收到的指令的索引是不是新的
            //这里本来应该判断index <= lastIndex,如果条件成立，就说明当前收到的指令是旧的
            //但在例子中就没必要进行这种判断了，直接判断是不是要接收的下一条消息就行，后面开始搭建我们
            //自己的raft框架时，大家会发现要做的判断会非常复杂，到时候再展开讲解
            if(index == lastIndex + 1){
                //给lastIndex赋值
                this.lastIndex = index;
                //执行指令
                execute(task);
            }
            return response;
        }
        return null;
    }



    //省略其他方法
}
```

好了，现在又对 NodeImpl 类重构了一下。重构到这里我忽然意识到，**既然主节点要把自己接收到的每一条客户端指令发送给从节点，以此来达到主从数据备份的效果。并且为了解决幂等性问题，需要给每一条指令添加一个唯一索引。这样一来，我其实完全可以再定义一个新的类，然后让这个类的对象来封装主节点要传递给从节点的指令和指令对应的索引。**这不就是日志的具体表现形式吗？在编程中，日志信息为我们开发程序、运行程序、调试程序提供了很多帮助，是一个非常重要的帮手。与此同时，日志又是一个十分宽泛的概念，并不是说，只有被 Log 记录下来的信息才叫做日志。实际上，程序运行信息，或者是程序的状态变化都可以当作日志记录下来。比如，当主节点的 map 要添加键值对了，添加完之后就意味着 map 的状态得到了更新，那么 map 添加键值对的操作就可以被当作日志记录下来。说了这么多，其实就一个意思，日志是一个很宽泛的概念，可以有多种表现形式，可以有索引，也可以没有索引，日志编解码方式也可以由你自己定义。如果你需要在程序中记录一些关键信息，或者是关键操作，每当你不知道该怎么命名这些信息的时候，就可以毫无顾虑地称它们为日志。就像我媳妇每次穿着新买的巨丑的衣服，问我衣服在她身上好看不好看，我不想说实话的时候，就会说还可以。

很好，日志的概念已经引出来了，接下来，我还想继续和大家讨论另一个问题，**那就是主从节点数据持久化时，究竟应该持久化什么数据？**按照之前的逻辑——当然，这个逻辑并没有在代码中表现出来，也只算能是一个口头逻辑——不管是在主节点还是从节点，都是直接对数据本身进行持久化，也就是对 NodeImpl 的 totalMoney 进行持久化，totalMoney 每更新一次，就要进行一次持久化(先不必在意是同步还是异步持久化)。但是，如今我为程序引入了一个 map 成员变量，这个成员变量可能会记录成千上万条数据，这时候显然就不能时时刻刻对这个 map 对象进行持久化，否则本地 IO 的时间就太长，也太频繁了，反而成为了数据持久化的风险。既然不能对数据本身进行持久化，那该怎么持久化呢？我想很多朋友应该也意识到了，**既然目前的主从节点是通过指令的传输，或者说是复制来保持主从数据一致的，使从节点承担数据备份的作用，那么我们完全可以对指令本身进行持久化。当然，经过刚才的分析，指令已经被我包装成日志了，所以，只要主节点和从节点对每一条日志进行持久化，就可以在一定程度上保证数据不丢失。原因很简单，只要日志持久化了，节点宕机重启后，可以从持久化的每一条日志中取出指令，依次执行，最终节点的 totalMoney 和 map 会恢复到和宕机前一样的状态。**大家可以品味品味这个过程。

好了，现在让我来为大家简单梳理一下目前程序的执行流程：**当主节点每接收到客户端发送的一条操作指令后，主节点可以应用这条指令，然后把这条指令包装成一条带索引的日志，然后把这条日志传输给从节点，当然，传输的同时可以在本地将日志异步持久化。从节点接收到这条日志后，需要执行这条日志中的指令，更新 lastIndex 成员变量，同时也要把这条日志本地持久化。这样一来，不管是哪个节点突然宕机了，只要每一条日志都持久化到本地了，重新启动后，就可以加载日志到内存，执行日志中的指令，最终使程序状态恢复到和宕机前一致。这时候大家应该也可以意识到，实际上给每一个操作指令定义对应的索引，并不是一个心血来潮的决定，因为索引是递增的，这个递增的顺序实际上也就代表了指令先后执行的顺序。当节点宕机后重启，就可以根据索引对应的指令来执行每一条指令，这样就可以保证最后节点的状态一定是和宕机前状态是一致的。**具体流程就是下图展示的这样。

![](https://cdn.nlark.com/yuque/0/2023/jpeg/26725125/1703752117349-28dbcd58-87cf-4365-8d80-bbda0b55078a.jpeg)

用文字分析思路，已经分析了不少了，是时候去代码中实践一下了。接下来，我就先来为自己的程序引入一个日志类，这个日志类就是用来包装客户端指令的，这个日志类就定义为 LogEntry 吧，也就是日志条目的意思，每一个客户端的操作指令都会包装成一个日志条目。当然，引入了 LogEntry 类之后，NodeImpl 类中的一些方法又需要再次重构一下了。这一章显然是讲解不完了，下一章再为大家继续讲解吧。下一章见！