从单机到**集群**再到高可用集群，手把手带你走入消息中间件的世界。**消息队列是一门重要而复杂的技术**，MQ全称为Message Queue，即消息队列，是**分布式系统中一种通信手段**。RabbitMQ是MQ众多产品中表现比较优秀的一种，它的作用和概念稍后讲解。

先了解一下MQ的重要性，重要是因为它功能强劲、能解决很多问题，复杂是因为由于引入MQ诞生更多的问题，这些问题需要我们解决。MQ是中高级程序员必备的技能。

保证MQ中消息的可靠性、幂等性？

---
# 0. 同步与异步通讯
**同步通讯单一模式**，类似打视频电话，只能和一个妹子聊天，任何动作对方都能实时看到。**异步通讯海王模式**，是时间管理大师的必备技能，类似与妹子微信聊天，有可能妹子还没看到，或者看到了不回，时效性不好，但能同时聊多个。两种方式各有优劣，打电话可以立即得到响应，但你却不能跟多个人同时通话。发送邮件可以同时与多个人收发邮件，但是往往响应会有延迟。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071146055.png)
## 0.1 同步通讯
我们之前学习的**Feign调用就属于同步方式**，虽然调用可以实时得到结果，但存在下面的问题。同步调用的优点：
- 时效性较强，可以立即得到结果

同步调用的问题：
- 耦合度高
- 性能和吞吐能力下降
- 有额外的资源消耗
- 有级联失败问题

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071149056.png)
# 0.2 异步通讯方案
**异步调用常见实现就是事件驱动-发布订阅模式**。支付服务完成后，发布支付成功事件，就不管其他事了。然后一个大喇叭宣布这件事，**其他服务增加后，只需订阅事件，就可接收通知；如果不要服务，就取消订阅即可**。事件基本不会发送失败，有个发布确认机制，没发成功会接着发。（服务是服务集群）

我们以购买商品为例，用户支付后需要调用订单服务完成订单状态修改，调用物流服务，从仓库分配响应的库存并准备发货。在事件模式中，支付服务是**事件发布者**（publisher），在支付完成后只需要发布一个支付成功的事件（event），事件中带上订单id。订单服务和物流服务是**事件订阅者**（Consumer），订阅支付成功的事件，监听到事件后完成自己业务即可。

==为了解除事件发布者与订阅者之间的耦合，两者并不是直接通信，而是有一个中间人==（Broker）。发布者发布事件到Broker，不关心谁来订阅事件。订阅者从Broker订阅事件，不关心谁发来的消息。
![image-20210422095356088](assets/image-20210422095356088.png)
Broker 是一个像数据总线一样的东西，所有的服务要接收数据和发送数据都发到这个总线上，这个总线就像协议一样，让服务间的通讯变得标准和可控。

优势一：服务解耦，每个服务都可以灵活插拔，可替换
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071201274.png)
优势二：性能提升，吞吐量提高。调用间没有阻塞，不会造成无效的资源占用；无需等待订阅者处理完成，响应更快速
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071209974.png)

优势三：故障隔离：服务没有强依赖，服务没有直接调用，不担心级联失败问题。
优势四：流量削峰：不管发布事件的流量波动多大，都由Broker接收，订阅者可以按照自己的速度去处理事件

异步通信的缺点：
- 依赖于Broker的可靠性、安全性、吞吐能力，系统可用性降低
- 架构变复杂，业务没有明显的流程线，不好追踪管理

大多数情况下还是使用同步通讯，对时效性要求高；只有少数并发情况下使用异步通讯。

好在现在开源软件或云平台上Broker的软件是非常成熟的，比较常见的一种就是我们今天要学习的MQ技术。 

---
## 1. MQ的基本概念
## 1.1 MQ概述
MQ全称Message Queue（消息队列），**是在消息**（软件系统中，一些小的数据）**的传输过程中保存消息的**（队列是一种容器）**容器**（存储消息的容器就可以称为消息队列）。消息队列多用于分布式系统（包括微服务）之间进行通信。

这里说到分布式系统的通信问题，分布式系统中有两种通信方式。==这里有两个系统（每个系统可以是集群），它们职责不同、干的事不一样，但它们可以组成一个大的系统，我们称为分布式系统==。A和B称为分布式系统的子系统，它们之间要完成通信、有两种方式。第一种是，A直接通过远程调用的方式访问B系统；除了这种方式，A系统还能通过第三方完成通信，A将数据发给C，C再将数据发给B。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212031853694.png)

MQ消息队列就属于后者。在A系统和B系统中引入一个MQ，这个时候A先将数据发送给MQ，MQ再把数据给B系统。这样也可以完成A、B之间的间接通信。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212031906803.png)

那么第一种直接通信、和第二种引入第三方（类似于MQ）完成间接通信，区别在哪里？后面重点讲解，现在只需知道这个概念就行了。

回到这个通信模型，A系统是生产消息的，称为生产者、B系统是消费消息的，称为消费者，中间这个MQ称为中间件，存储消息的中间件。
## 1.2 MQ的优势和劣势
优势：
- 应用解耦
- 异步提速
- 削峰填谷
- 故障隔离

劣势：
- 系统可用性降低
- 系统复杂度提高
- 一致性问题

不太好理解。看这个例子：应用耦合是，支付服务要依次调用订单服务、仓储服务等，如果产品要求加新需求如短信，支付服务程序员不仅需要编写新的短信服务，还要修改支付服务，在调用仓储服务后增加对短信服务的调用。性能下降就是这些服务依次调用，同步执行，耗时大大提高，而且有大量系统资源在浪费。如果仓储服务挂了，那么支付服务也执行不下去，新用户的请求都完不成。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071153602.png)
### 1.2.1 应用解耦
设计系统或写代码，都追求“高内聚，低耦合”，耦合性降低了，各自代码的可维护性就会提高，分布式系统中也在追求解耦。

这里有个订单系统、以及库存系统、支付系统、物流系统、以及其他系统。当用户下订单后，会访问订单系统，而订单系统要去查询库存、调用支付系统等，所以它有两种调用方式，一种是直接用远程调用方式……这样订单系统就和库存、支付、物流三个系统耦合在一起。耦合在一起后，有什么问题呢？其实有两个问题。

第一个问题，我们考虑一下，假如库存系统出现异常、它挂了，那么订单系统在下订单时，这整个链路就走不通了，从而导致订单系统也可能出现一些问题，从而用户下单失败。**库存系统故障，订单系统也会失败，我们认为这整个系统的容错性比较低**。

除了第一个容错性问题以外，还有第二个问题。假设产品经理找到订单系统的程序员说，我们还要在整个订单流程中加一个X系统，那么订单系统程序员只能修改订单系统、让它去访问X系统。这次修改完成了，过了两天，又要加新系统，再过几天，又说X系统不要了。订单系统程序员就不停的修改系统的代码，我们认为**订单系统的可维护性就比较低**。

**系统的耦合性越高**，容错性就越低，可维护性就越低。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212031924483.png)
下面还是这么一个订单操作，我们引入一个MQ在中间。当用户点击按钮下订单，订单系统只需要给MQ发一条消息，然后这个时候我们就可以告诉用户：下单成功了。不对啊，后面的库存、支付、物流，都没有执行啊。没有关系，这些系统只需要从MQ中分别把订单消息数据在自己的系统中消费了，就可以了。

让我们再想一想，假设后面的库存系统错误了，这个时候订单系统不会受到影响，它和后面几个系统间被隔离了。那下单不还是失败了吗？没有关系，库存系统产生错误可能是短暂的，当它修复好后，再从MQ中取出那个消息进行消费，最终数据肯定是正常的。这个时候我们系统的容错性就提高了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212031924955.png)
针对新增的X系统，我们只需要让X系统从MQ中把消息取出来进行消费即可，订单系统的代码一行都不用改，可维护性大大提高。

由此可见，**使用 MQ 使得应用间解耦，提升容错性和可维护性**。
### 1.2.2 异步提速
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032033083.png)

还是这个订单系统。当用户下完订单后，订单系统它要保存到自己的数据库，20ms，然后通过远程调用的方式，访问这些系统，分别耗用……这个时候是一种同步方式，它需要先访问库存、返回后再访问支付、最后再访问物流系统。整个调用都成功后，才返回给用户，说成功了。一个下单操作耗时：20 + 300 + 300 + 300 = 920ms。用户点击完下单按钮后，需要等待920ms才能得到下单响应，对互联网系统太慢了！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032041058.png)
为了提升这个速度，我们用MQ解决。订单系统要保存数据库，保存它自己的数据，然后订单系统花5ms将消息发给MQ，这时订单系统就可以直接告诉用户说成功下单了。它不需要管后台的其他系统是否执行成功了。整体只花了25ms，用户体验得到提升，系统吞吐量也大大增强。
## 1.2.3 削峰填谷
假设A系统的运营要做一个活动（比如说秒杀活动），于是突增大量请求，每秒5000个，A系统处理不了、瞬间宕机，用户体验太差。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032055629.png)
一旦引入MQ，在请求增多时，它们对接的是MQ，A系统会慢慢从MQ中取出消息来处理。关键是，MQ能否承载这么多请求呢？完全能！MQ将这些请求存入缓冲区，避免大量请求直接让A系统崩溃。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032057162.png)
上面是削峰，填谷呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032338221.png)
使用了MQ之后，限制消费消息的速度为1000，这样一来，高峰期产生的数据势必会被积压在 MQ 中，高峰就被“削”掉了，但因为消息积压，在高峰期过后的一段时间内，消费消息的速度还是会维持在1000，直到消费完积压的消息，这就叫做“填谷”。

**使用MQ后，可以提高系统稳定性**。
### 1.2.4 系统可用性降低
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032346519.png)
系统引入的外部依赖越多，系统稳定性越差。一旦 MQ 宕机，就会对业务造成影响。**如何保证MQ的高可用**？
### 1.2.5 系统复杂度提高
MQ 的加入大大增加了系统的复杂度，以前系统间是同步的远程调用，现在是通过 MQ 进行异步调用。**如何保证消息没有被重复消费？怎么处理消息丢失情况？那么保证消息传递的顺序性？**
### 1.2.6 一致性问题
A 系统处理完业务，通过 MQ 给B、C、D三个系统发消息数据，如果 B 系统、C 系统处理成功，D 系统处理失败。如何保证消息数据处理的一致性？

## 总结
既然 MQ 有优势也有劣势，那么使用 MQ 需要满足什么条件呢？
① **生产者不需要从消费者处获得反馈**。==引入消息队列之前的直接调用，其接口的返回值应该为空==（A系统调用B系统，不需要、不用等它的反馈），这才让明明下层的动作还没做，上层却当成动作做完了继续往后走，即所谓异步成为了可能。
② ==容许短暂的不一致性==。即容忍数据短暂的不一致。
③ 确实有效果。即使用MQ后，解耦、提速、削峰这些方面的收益，超过引入MQ和管理MQ等成本。

## 1.5 常见的MQ产品
目前业界有很多的 MQ 产品，例如 RabbitMQ、RocketMQ、ActiveMQ、Kafka、ZeroMQ、MetaMq等，也有直接使用 Redis 充当消息队列的案例，而这些消息队列产品各有侧重，在实际选型时，需要结合自身需求及 MQ 产品特征，综合考虑。

单机吞吐量（每秒处理的请求数）最差的是ActiveMQ，老牌产品。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032354231.png)
由于 RabbitMQ 综合能力强劲，所以接下来的课程中，我们将主要学习 RabbitMQ。

MQ产品都是中间件，要想操作它们，需要客户端。
|            | **RabbitMQ**            | **ActiveMQ**                   | **RocketMQ** | **Kafka**  |
| ---------- | ----------------------- | ------------------------------ | ------------ | ---------- |
| 公司/社区  | Rabbit                  | Apache                         | 阿里         | Apache     |
| 开发语言   | Erlang                  | Java                           | Java         | Scala&Java |
| 协议支持   | AMQP，XMPP，SMTP，STOMP | OpenWire,STOMP，REST,XMPP,AMQP | 自定义协议   | 自定义协议 |
| 可用性     | 高                      | 一般                           | 高           | 高         |
| 单机吞吐量 | 一般                    | 差                             | 高           | 非常高     |
| 消息延迟   | 微秒级                  | 毫秒级                         | 毫秒级       | 毫秒以内   |
| 消息可靠性 | 高                      | 一般                           | 高           | 一般       |

追求可用性：Kafka、 RocketMQ 、RabbitMQ
追求可靠性：RabbitMQ、RocketMQ
追求吞吐能力：RocketMQ、Kafka
追求消息低延迟：RabbitMQ、Kafka

---
## 1.6 RabbitMQ简介
不仅了解RabbitMQ的基本概念和各种工作模式，还要知道AMQP这种消息协议的基本知识。
### 1.6.1 AMQP 
*Advanced Message Queuing Protocol, AMQP*（高级消息队列协议），是一个网络协议，是应用层协议的一个开放标准，**为面向消息的中间件设计**。基于此协议实现的客户端与消息中间件可传递消息，并不受客户端/中间件不同产品，不同的开发语言等条件的限制。2006年，AMQP规范发布。可以类比HTTP。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212032356561.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071243013.png)
- Publisher：发布者
- Exchange：翻译过来是交换机，它是用来分发网络请求的。而在AMQP中也是类似的，它路由消息到不同的Queue（一种容器，存储消息）中。交换机分发的过程就是Routes，路由
- Queue：缓存消息
- Virtual Host：**虚拟主机，是对Queue、Exchange等资源的逻辑分组**，隔离不同租户的exchange、queue、消息的隔离
- Consumer：监听队列，并取走消息

### 1.6.2 RabbitMQ基础架构
2007年，Rabbit技术公司基于AMQP标准开发的RabbitMQ 1.0发布。RabbitMQ 采用 Erlang 语言开发。
> Erlang 语言由 Ericson 设计，专门为开发高并发和分布式系统的一种语言，在电信领域使用广泛。

RabbitMQ 基础架构如下图（类似上图，但复杂一点）：分为三部分，
第一部分：broker 经纪人，中间人，代理人，是RabbitMQ的Server，而Producer和Consumer都是客户端，客户端通过TCP Connection与服务端Broker连接。如果每通信一次就建立一次连接，那么将占用太多资源，为此每个Connection中都有许多Channel，可以通过这些Channel进行通信。

RabbitMQ server中又有许多Virtual Hose虚拟机，每个host中都有许多Exchange和Queue，每个交换机都可以绑定Binding到队列中
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212040019424.png)
- Broker：**接收和分发消息的应用**，RabbitMQ Server就是 Message Broker
- Exchange：message 到达 broker 的第一站，根据分发规则，匹配查询表中的 routing key，分发消息到 queue 中去。常用的类型有：direct (point-to-point), topic (publish-subscribe) and fanout (multicast) 
- Queue：消息最终被送到这里等待 consumer 取走
- Virtual host：出于多租户和安全因素设计的，把AMQP的基本组件划分到一个虚拟的分组中，类似于网络中的 `namespace` 概念。当多个不同的用户使用同一个 `RabbitMQ server` 提供的服务时，可以划分出多个vhost ，每个用户在自己的 vhost 创建 exchange／queue 等
- Binding：**exchange 和 queue 之间的虚拟连接**，binding 中可以包含 routing key。Binding 信息被保存到 exchange 中的查询表中，用于 message 的分发依据
- Connection：publisher／consumer 和 broker 之间的 **TCP 连接**
- Channel：如果每一次访问 RabbitMQ 都建立一个 Connection，在消息量大的时候建立 TCP Connection的开销将是巨大的，效率也较低。Channel 是在 connection 内部建立的逻辑连接，如果应用程序支持多线程，通常每个thread创建单独的 channel 进行通讯，AMQP method 包含了channel id 帮助客户端和message broker 识别 channel，所以 channel 之间是完全隔离的。Channel 作为轻量级的 Connection ，极大减少了操作系统建立 TCP connection 的开销

### 1.6.3 工作模式
其实就是生产和消费消息的不同方式。官网对应模式介绍：https://www.rabbitmq.com/getstarted.html。RabbitMQ提供了6种工作模式：
- 简单模式或基本消息队列（BasicQueue）（一对一）、工作消息队列（WorkQueue）（一对多个消费者）
	![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071251195.png)
- 发布订阅（Publish、Subscribe），又根据交换机类型不同分为三种：
	- Fanout Exchange：广播。Publish/Subscribe发布与订阅模式（一对多，引入交换机）
	- Direct Exchange：路由。Routing路由模式
	- Topic Exchange：主题。Topics 主题模式
	![650](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071250568.png)
RPC 远程调用模式（远程调用，不太算 MQ；暂不作介绍）。
![800](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212040033736.png)

1.简单模式 HelloWorld
一个生产者、一个消费者，不需要设置交换机（使用默认的交换机）。

2.工作队列模式 Work Queue
一个生产者、多个消费者（**竞争关系**），不需要设置交换机（使用默认的交换机）。

3.发布订阅模式 Publish/subscribe
需要设置类型为 fanout 的交换机，并且交换机和队列进行绑定，当发送消息到交换机后，交换机会将消息发送到绑定的队列。

4.路由模式 Routing
需要设置类型为 direct 的交换机，交换机和队列进行绑定，并且指定 routing key，当发送消息到交换机后，交换机会根据 routing key 将消息发送到对应的队列。

5.通配符模式 Topic
需要设置类型为 topic 的交换机，交换机和队列进行绑定，并且指定通配符方式的 routing key，当发送消息到交换机后，交换机会根据 routing key 将消息发送到对应的队列。

### 1.6.4 HelloWord案例
简单队列模式的模型图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071425429.png)
官方的HelloWorld是基于最基础的消息队列模型来实现的，只包括三个角色：
- publisher：消息发布者，将消息发送到队列queue
- queue：消息队列，负责接受并缓存消息
- consumer：订阅队列，处理队列中的消息

课前资料提供了一个Demo工程，mq-demo，导入后可以看到结构如下。包括三部分：
- mq-demo：父工程，管理项目依赖
- publisher：消息的发送者
- consumer：消息的消费者
	![250](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071423688.png)

publisher实现思路：
- 建立连接Connection
- 创建Channel
- 利用Channel声明队列Queue
- 利用Channel向队列发送消息
- 关闭连接和Channel

代码实现：
```java
package cn.itcast.mq.helloworld;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;
import org.junit.Test;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class PublisherTest {
    @Test
    public void testSendMessage() throws IOException, TimeoutException {
        // 1.建立连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        // 1.1.设置连接参数，分别是：主机名、端口号、vhost、用户名、密码
        factory.setHost("192.168.150.101");
        factory.setPort(5672);
        factory.setVirtualHost("/"); // 默认虚拟机
        factory.setUsername("itcast");
        factory.setPassword("123321");
        // 1.2.建立连接
        Connection connection = factory.newConnection();

        // 2.创建通道Channel
        Channel channel = connection.createChannel();

        // 3.创建队列
        String queueName = "simple.queue";
        channel.queueDeclare(queueName, false, false, false, null);

        // 4.发送消息
        String message = "hello, rabbitmq!";
        channel.basicPublish("", queueName, null, message.getBytes());
        System.out.println("发送消息成功：【" + message + "】");

        // 5.关闭通道和连接
        channel.close();
        connection.close();
    }
}
```
consumer实现代码思路：
- 建立连接Connection
- 创建Channel
- 利用Channel声明队列
- 利用Channel将消费者与队列绑定
- 并定义consumer的消费行为 `handleDelivery()` ，订阅消息
 
代码实现：
```java
package cn.itcast.mq.helloworld;

import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class ConsumerTest {
    public static void main(String[] args) throws IOException, TimeoutException {
        // 1.建立连接
        ConnectionFactory factory = new ConnectionFactory();
        // 1.1.设置连接参数，分别是：主机名、端口号、vhost、用户名、密码
        factory.setHost("192.168.150.101");
        factory.setPort(5672);
        factory.setVirtualHost("/");
        factory.setUsername("itcast");
        factory.setPassword("123321");
        // 1.2.建立连接
        Connection connection = factory.newConnection();

        // 2.创建通道Channel
        Channel channel = connection.createChannel();

        // 3.创建队列
        String queueName = "simple.queue";
        channel.queueDeclare(queueName, false, false, false, null);

        // 4.订阅消息
        channel.basicConsume(queueName, true, new DefaultConsumer(channel){
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope,
                                       AMQP.BasicProperties properties, byte[] body) throws IOException {
                // 5.处理消息
                String message = new String(body);
                System.out.println("接收到消息：【" + message + "】");
            }
        });
        System.out.println("等待接收消息。。。。");
    }
}
```
消息是阅后即焚的，一旦被使用就会被从队列中删除。注意：消息一旦消费就会从队列删除，RabbitMQ没有消息回溯功能。

## 1.7 JMS
- JMS即 **Java 消息服务**（JavaMessage Service）**应用程序接口**，是一个Java平台中关于面向消息中间件的API
- JMS是JavaEE规范中的一种，类比JDBC，是Java程序与数据库通信的一套API接口。设计时设想的是所有Java程序和所有消息队列通信时，都遵循这一套API，但RabbitMQ没有遵循
- 很多消息中间件都实现了JMS规范，例如：ActiveMQ。RabbitMQ官方没有提供 JMS 的实现包，但是开源社区有

## 1.8 小结
1. RabbitMQ是基于AMQP协议使用Erlang语言开发的一款消息队列产品。
2. RabbitMQ提供了6种工作模式，我们学习5种。这是今天的重点。
3. AMQP 是协议，类比HTTP。
4. JMS 是 API 规范接口，类比 JDBC。

---
# 2. RabbitMQ的安装和配置
Linux系统上如何安装？
RabbitMQ 官方地址：http://www.rabbitmq.com/
安装文档：资料/软件/安装 RabbitMQ.md

GetStart：7种工作模式
课程选用3.6.5.1版本
## 2.1 单机部署
我们在Centos7虚拟机中使用Docker来安装。
### 2.1.1 下载镜像
方式一：在线拉取
``` sh
docker pull rabbitmq:3-management
```
方式二：从本地加载。在课前资料已经提供了镜像包：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071235624.png)
上传到虚拟机中后，使用命令加载镜像即可：
```sh
docker load -i mq.tar
```
### 2.1.2 安装MQ
执行下面的命令来运行MQ容器：
```sh
docker run \
 -e RABBITMQ_DEFAULT_USER=itcast \
 -e RABBITMQ_DEFAULT_PASS=123321 \
 --name mq \
 --hostname mq1 \
 -p 15672:15672 \
 -p 5672:5672 \
 -d \
 rabbitmq:3-management
```
## 2.2 集群部署
接下来，我们看看如何安装RabbitMQ的集群。
### 2.2.1 集群分类
在RabbitMQ的官方文档中，讲述了两种集群的配置方式：
- 普通模式：普通模式集群不进行数据同步，每个MQ都有自己的队列、数据信息（其它元数据信息如交换机等会同步）。例如我们有2个MQ：mq1，和mq2，如果你的消息在mq1，而你连接到了mq2，那么mq2会去mq1拉取消息，然后返回给你。如果mq1宕机，消息就会丢失。
- 镜像模式：与普通模式不同，队列会在各个mq的镜像节点之间同步，因此你连接到任何一个镜像节点，均可获取到消息。而且如果一个节点宕机，并不会导致数据丢失。不过，这种方式增加了数据同步的带宽消耗。

我们先来看普通模式集群。
### 2.2.2 设置网络
首先，我们需要让3台MQ互相知道对方的存在。分别在3台机器中，设置 /etc/hosts文件，添加如下内容：
```
192.168.150.101 mq1
192.168.150.102 mq2
192.168.150.103 mq3
```
并在每台机器上测试，是否可以ping通对方。

---
## 3. RabbitMQ快速入门（Maven工程）
步骤：
① 创建工程（生产者、消费者）
② 分别添加依赖
③ 编写生产者发送消息
④ 编写消费者接收消息

`pom.xml` ：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.itheima</groupId>
    <artifactId>rabbitmq-producer</artifactId>
    <version>1.0-SNAPSHOT</version>

    <dependencies>
        <!--rabbitmq java 客户端-->
        <dependency>
            <groupId>com.rabbitmq</groupId>
            <artifactId>amqp-client</artifactId>
            <version>5.6.0</version>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.8.0</version>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
        </plugins>
    </build>

</project>
```
### 3.1 简单队列模式
下面使用的是如下的简单模式：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303102159567.png)
在上图的模型中，有以下概念（里面其实有个默认的交换机）：
- P：生产者，也就是要发送消息的程序
- C：消费者：消息的接收者，会一直等待消息到来
- queue：消息队列，图中红色部分。类似一个邮箱，可以缓存消息；生产者向其中投递消息，消费者从其中取出消息

`rabbitmq-producer/src/main/java/...producer/Producer_HelloWorld.java` ：
```java
package com.itheima.producer;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * 发送消息
 */
public class Producer_HelloWorld {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
        //5. 创建队列Queue
        /*
        queueDeclare(String queue, boolean durable, boolean exclusive, boolean autoDelete, Map<String, Object> arguments)
        参数：
            1. queue：队列名称
            2. durable:是否持久化，当mq重启之后，还在
            3. exclusive：
                * 是否独占。只能有一个消费者监听这队列
                * 当Connection关闭时，是否删除队列
            4. autoDelete:是否自动删除。当没有Consumer时，自动删除掉
            5. arguments：参数
         */
        //如果没有一个名字叫hello_world的队列，则会创建该队列，如果有则不会创建
        channel.queueDeclare("hello_world",true,false,false,null);
        /*
        basicPublish(String exchange, String routingKey, BasicProperties props, byte[] body)
        参数：
            1. exchange：交换机名称。简单模式下交换机会使用默认的 ""
            2. routingKey：路由名称
            3. props：配置信息
            4. body：发送消息数据
         */
        String body = "hello rabbitmq~~~";
        //6. 发送消息
        channel.basicPublish("","hello_world",null,body.getBytes());
        //7.释放资源
      channel.close();
      connection.close();
    }
}
```
`rabbitmq-consumer/src/main/java/...consumer/Consumer_HelloWorld.java` ：
```java
package com.itheima.consumer;

import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class Consumer_HelloWorld {
    public static void main(String[] args) throws IOException, TimeoutException {

        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
        //5. 创建队列Queue
        /*
        queueDeclare(String queue, boolean durable, boolean exclusive, boolean autoDelete, Map<String, Object> arguments)
        参数：
            1. queue：队列名称
            2. durable:是否持久化，当mq重启之后，还在
            3. exclusive：
                * 是否独占。只能有一个消费者监听这队列
                * 当Connection关闭时，是否删除队列
            4. autoDelete:是否自动删除。当没有Consumer时，自动删除掉
            5. arguments：参数。

         */
        //如果没有一个名字叫hello_world的队列，则会创建该队列，如果有则不会创建
        channel.queueDeclare("hello_world",true,false,false,null);

        /*
        basicConsume(String queue, boolean autoAck, Consumer callback)
        参数：
            1. queue：队列名称
            2. autoAck：是否自动确认
            3. callback：回调对象
         */
        // 接收消息
        Consumer consumer = new DefaultConsumer(channel){
            /*
                回调方法，当收到消息后，会自动执行该方法
                1. consumerTag：标识
                2. envelope：获取一些信息，交换机，路由key...
                3. properties:配置信息
                4. body：数据
             */
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
                System.out.println("consumerTag："+consumerTag);
                System.out.println("Exchange："+envelope.getExchange());
                System.out.println("RoutingKey："+envelope.getRoutingKey());
                System.out.println("properties："+properties);
                System.out.println("body："+new String(body));
            }
        };
        channel.basicConsume("hello_world",true,consumer);
        //关闭资源？不要
    }
}
```
### 3.2 工作队列模式
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303102200174.png)
Work Queues：与入门程序的简单模式相比，多了一个或一些消费端，多个消费端共同（轮询）消费同一个队列中的消息。

应用场景：==对于任务过重或任务较多情况，使用工作队列可以提高任务处理的速度==。Work Queues与入门程序的简单模式的代码几乎是一样的。可以完全复制，并多复制一个消费者进行多个消费者同时对消费消息的测试。
1. 在一个队列中如果有多个消费者，那么消费者之间对于同一个消息的关系是**竞争**的关系。 
2. Work Queues对于任务过重或任务较多情况，使用工作队列可以提高任务处理的速度。例如：短信服务部署多个，只需要有一个节点成功发送即可。

`rabbitmq-producer/src/main/java/...producer/Producer_WorkQueues.java` ：
```java
package com.itheima.producer;

import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * 发送消息
 */
public class Producer_WorkQueues {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
        //5. 创建队列Queue
        /*
        queueDeclare(String queue, boolean durable, boolean exclusive, boolean autoDelete, Map<String, Object> arguments)
        参数：
            1. queue：队列名称
            2. durable:是否持久化，当mq重启之后，还在
            3. exclusive：
                * 是否独占。只能有一个消费者监听这队列
                * 当Connection关闭时，是否删除队列
            4. autoDelete:是否自动删除。当没有Consumer时，自动删除掉
            5. arguments：参数
         */
        //如果没有一个名字叫hello_world的队列，则会创建该队列，如果有则不会创建
        channel.queueDeclare("work_queues",true,false,false,null);
        /*
        basicPublish(String exchange, String routingKey, BasicProperties props, byte[] body)
        参数：
            1. exchange：交换机名称。简单模式下交换机会使用默认的 ""
            2. routingKey：路由名称
            3. props：配置信息
            4. body：发送消息数据
         */
        for (int i = 1; i <= 10; i++) {
            String body = i+"hello rabbitmq~~~";
            //6. 发送消息
            channel.basicPublish("","work_queues",null,body.getBytes());
        }
        //7.释放资源
      channel.close();
      connection.close();
    }
}
```
`rabbitmq-consumer/src/main/java/...consumer/Consumer_WorkQueues.java` ，这个对象运行几次，形成几个消费者：
```java
package com.itheima.consumer;

import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class Consumer_WorkQueues1 {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
        //5. 创建队列Queue
        /*
        queueDeclare(String queue, boolean durable, boolean exclusive, boolean autoDelete, Map<String, Object> arguments)
        参数：
            1. queue：队列名称
            2. durable:是否持久化，当mq重启之后，还在
            3. exclusive：
                * 是否独占。只能有一个消费者监听这队列
                * 当Connection关闭时，是否删除队列
            4. autoDelete:是否自动删除。当没有Consumer时，自动删除掉
            5. arguments：参数
         */
        //如果没有一个名字叫hello_world的队列，则会创建该队列，如果有则不会创建
        channel.queueDeclare("work_queues",true,false,false,null);

        /*
        basicConsume(String queue, boolean autoAck, Consumer callback)
        参数：
            1. queue：队列名称
            2. autoAck：是否自动确认
            3. callback：回调对象
         */
        // 接收消息
        Consumer consumer = new DefaultConsumer(channel){
            /*
                回调方法，当收到消息后，会自动执行该方法
                1. consumerTag：标识
                2. envelope：获取一些信息，交换机，路由key...
                3. properties:配置信息
                4. body：数据
             */
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
              /*  System.out.println("consumerTag："+consumerTag);
                System.out.println("Exchange："+envelope.getExchange());
                System.out.println("RoutingKey："+envelope.getRoutingKey());
                System.out.println("properties："+properties);*/
                System.out.println("body："+new String(body));
            }
        };
        channel.basicConsume("work_queues",true,consumer);
        //关闭资源？不要
    }
}
```
## 3.3 发布订阅模式
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303102203562.png)
在订阅模型中，多了一个 `Exchange` 角色，而且过程略有变化：
- P：生产者，也就是要发送消息的程序，但不再发送到队列中，而是发给X（交换机）
- C：消费者，消息的接收者，**会一直等待消息到来**
- Queue：消息队列，接收消息、缓存消息
- Exchange：交换机（X）。一方面，接收生产者发送的消息。另一方面，知道如何处理消息，例如==递交给某个特别队列、递交给所有队列、或是将消息丢弃==。到底如何操作，取决于Exchange的类型。Exchange有常见以下3种类型：
	➢ Fanout：广播，将消息交给**所有绑定到交换机的队列**
	➢ Direct：定向，把消息交给**符合指定 `routing key` 的队列**
	➢ Topic：通配符，把消息交给**符合 `routing pattern`（路由模式） 的队列**
==Exchange（交换机）只负责转发消息，不具备存储消息的能力==，因此如果没有任何队列与 Exchange 绑定，或者没有符合路由规则的队列，那么消息会丢失！

发布订阅模式中：交换机需要与队列进行绑定，绑定之后；**一个消息可以被多个消费者都收到**。发布订阅模式与工作队列模式的区别：
- 工作队列模式不用定义交换机，而发布/订阅模式需要定义交换机
- 发布/订阅模式的生产方是面向交换机发送消息，工作队列模式的生产方是面向队列发送消息（底层使用默认交换机）
- 发布/订阅模式需要设置队列和交换机的绑定 `channel.queueBind` ，工作队列模式不需要设置，实际上工作队列模式会将队列绑定到默认的交换机

`rabbitmq-producer/src/main/java/...producer/Producer_PubSub.java` ：
```java
package com.itheima.producer;

import com.rabbitmq.client.BuiltinExchangeType;
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * 发送消息
 */
public class Producer_PubSub {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
       /*
       exchangeDeclare(String exchange, BuiltinExchangeType type, boolean durable, boolean autoDelete, boolean internal, Map<String, Object> arguments)
       参数：
        1. exchange:交换机名称
        2. type:交换机类型
            DIRECT("direct"), 定向
            FANOUT("fanout"), 扇形（广播），发送消息到每一个与之绑定队列。
            TOPIC("topic"), 通配符的方式
            HEADERS("headers") 参数匹配
        3. durable:是否持久化
        4. autoDelete:自动删除
        5. internal：内部使用。 一般false
        6. arguments：参数
        */
       String exchangeName = "test_fanout";
        //5. 创建交换机
        channel.exchangeDeclare(exchangeName, BuiltinExchangeType.FANOUT,true,false,false,null);
        //6. 创建队列
        String queue1Name = "test_fanout_queue1";
        String queue2Name = "test_fanout_queue2";
        channel.queueDeclare(queue1Name,true,false,false,null);
        channel.queueDeclare(queue2Name,true,false,false,null);
        //7. 绑定队列和交换机
        /*
        queueBind(String queue, String exchange, String routingKey)
        参数：
            1. queue：队列名称
            2. exchange：交换机名称
            3. routingKey：路由键，绑定规则
                如果交换机的类型为fanout ，routingKey设置为""
         */
        channel.queueBind(queue1Name,exchangeName,"");
        channel.queueBind(queue2Name,exchangeName,"");

        String body = "日志信息：张三调用了findAll方法...日志级别：info...";
        //8. 发送消息 routingkey都是""
        channel.basicPublish(exchangeName,"",null,body.getBytes());
        //9. 释放资源
        channel.close();
        connection.close();
    }
}
```
`rabbitmq-consumer/src/main/java/...consumer/Consumer_PubSub.java` （运行多个对象，或创建多个Java文件）：
```java
package com.itheima.consumer;

import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class Consumer_PubSub1 {
    public static void main(String[] args) throws IOException, TimeoutException {

        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();

        String queue1Name = "test_fanout_queue1";
        String queue2Name = "test_fanout_queue2";
        /*
        basicConsume(String queue, boolean autoAck, Consumer callback)
        参数：
            1. queue：队列名称
            2. autoAck：是否自动确认
            3. callback：回调对象
         */
        // 接收消息
        Consumer consumer = new DefaultConsumer(channel){
            /*
                回调方法，当收到消息后，会自动执行该方法
                1. consumerTag：标识
                2. envelope：获取一些信息，交换机，路由key...
                3. properties:配置信息
                4. body：数据
             */
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
              /*  System.out.println("consumerTag："+consumerTag);
                System.out.println("Exchange："+envelope.getExchange());
                System.out.println("RoutingKey："+envelope.getRoutingKey());
                System.out.println("properties："+properties);*/
                System.out.println("body："+new String(body));
                System.out.println("将日志信息打印到控制台.....");
            }
        };
        channel.basicConsume(queue1Name,true,consumer);
        //关闭资源？不要
    }
}
```
### 3.4 路由模式
模式说明：
- 队列与交换机的绑定，**不能是任意绑定了**，而是要指定一个 RoutingKey（路由key）
- **消息的发送方在向 Exchange 发送消息时，也必须指定消息的 RoutingKey**
- Exchange 不再把消息交给每一个绑定的队列，而是根据消息的 Routing Key 进行判断，只有队列的 Routingkey 与消息的 Routing key 完全一致，才会接收到消息

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303102209926.png)
- P：生产者，向 Exchange 发送消息，发送消息时，会指定一个routing key
- X：Exchange（交换机），接收生产者的消息，然后把消息递交给与 routing key 完全匹配的队列
- C1：消费者，其所在队列指定了需要 routing key 为 error 的消息
- C2：消费者，其所在队列指定了需要 routing key 为 info、error、warning 的消息

`rabbitmq-producer/src/main/java/...producer/Producer_Routing.java` ：
```java
package com.itheima.producer;

import com.rabbitmq.client.BuiltinExchangeType;
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * 发送消息
 */
public class Producer_Routing {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
       /*
       exchangeDeclare(String exchange, BuiltinExchangeType type, boolean durable, boolean autoDelete, boolean internal, Map<String, Object> arguments)
       参数：
        1. exchange:交换机名称
        2. type:交换机类型
            DIRECT("direct"),：定向
            FANOUT("fanout"),：扇形（广播），发送消息到每一个与之绑定队列。
            TOPIC("topic"),通配符的方式
            HEADERS("headers");参数匹配

        3. durable:是否持久化
        4. autoDelete:自动删除
        5. internal：内部使用。 一般false
        6. arguments：参数
        */

       String exchangeName = "test_direct";
        //5. 创建交换机
        channel.exchangeDeclare(exchangeName, BuiltinExchangeType.DIRECT,true,false,false,null);
        //6. 创建队列
        String queue1Name = "test_direct_queue1";
        String queue2Name = "test_direct_queue2";

        channel.queueDeclare(queue1Name,true,false,false,null);
        channel.queueDeclare(queue2Name,true,false,false,null);
        //7. 绑定队列和交换机
        /*
        queueBind(String queue, String exchange, String routingKey)
        参数：
            1. queue：队列名称
            2. exchange：交换机名称
            3. routingKey：路由键，绑定规则
                如果交换机的类型为fanout ，routingKey设置为""
         */
        //队列1绑定 error
        channel.queueBind(queue1Name,exchangeName,"error");
        //队列2绑定 info  error  warning
        channel.queueBind(queue2Name,exchangeName,"info");
        channel.queueBind(queue2Name,exchangeName,"error");
        channel.queueBind(queue2Name,exchangeName,"warning");
        String body = "日志信息：张三调用了delete方法...出错误了。。。日志级别：error...";
        //8. 发送消息
        channel.basicPublish(exchangeName,"warning",null,body.getBytes());
        //9. 释放资源
        channel.close();
        connection.close();
    }
}
```
`rabbitmq-consumer/src/main/java/...consumer/Consumer_Routing.java` （`Routing2` 可能是将数据存储到数据库）：
```java
package com.itheima.consumer;

import com.rabbitmq.client.*;
import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class Consumer_Routing1 {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
        String queue1Name = "test_direct_queue1";
        String queue2Name = "test_direct_queue2";
        /*
        basicConsume(String queue, boolean autoAck, Consumer callback)
        参数：
            1. queue：队列名称
            2. autoAck：是否自动确认
            3. callback：回调对象
         */
        // 接收消息
        Consumer consumer = new DefaultConsumer(channel){
            /*
                回调方法，当收到消息后，会自动执行该方法
                1. consumerTag：标识
                2. envelope：获取一些信息，交换机，路由key...
                3. properties:配置信息
                4. body：数据
             */
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
              /*  System.out.println("consumerTag："+consumerTag);
                System.out.println("Exchange："+envelope.getExchange());
                System.out.println("RoutingKey："+envelope.getRoutingKey());
                System.out.println("properties："+properties);*/
                System.out.println("body："+new String(body));
                System.out.println("将日志信息打印到控制台.....");
            }
        };
        channel.basicConsume(queue2Name,true,consumer);
        //关闭资源？不要
    }
}
```
## 3.5 Topics通配符模式
模式说明：
- Topic 类型与 Direct 相比，都是可以根据 RoutingKey 把消息路由到不同的队列。只不过 Topic 类型 Exchange 可以让队列在绑定 Routing key 的时候使用通配符！
- Routingkey 一般都是有一个或多个单词组成，多个单词之间以 `.` 分割，例如： item.insert
- 通配符规则：# 匹配一个或多个词，* 匹配不多不少恰好1个词，例如：item.# 能够匹配 item.insert.abc 或者 item.insert，item.* 只能匹配 item.insert
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303102212611.png)
图解：
- 红色 Queue：绑定的是 usa.# ，因此凡是以 usa. 开头的 routing key 都会被匹配到
- 黄色 Queue：绑定的是 #.news ，因此凡是以 .news 结尾的 routing key 都会被匹配

Topic 主题模式可以实现 Pub/Sub 发布与订阅模式和 Routing 路由模式的功能，只是 Topic 在配置 routing key 的时候可以使用通配符，显得更加灵活。

`rabbitmq-producer/src/main/java/...producer/Producer_Topics.java` ：
```java
package com.itheima.producer;

import com.rabbitmq.client.BuiltinExchangeType;
import com.rabbitmq.client.Channel;
import com.rabbitmq.client.Connection;
import com.rabbitmq.client.ConnectionFactory;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

/**
 * 发送消息
 */
public class Producer_Topics {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();
       /* 
       exchangeDeclare(String exchange, BuiltinExchangeType type, boolean durable, boolean autoDelete, boolean internal, Map<String, Object> arguments)
       参数：
        1. exchange:交换机名称
        2. type:交换机类型
            DIRECT("direct"),：定向
            FANOUT("fanout"),：扇形（广播），发送消息到每一个与之绑定队列。
            TOPIC("topic"),通配符的方式
            HEADERS("headers");参数匹配

        3. durable:是否持久化
        4. autoDelete:自动删除
        5. internal：内部使用。 一般false
        6. arguments：参数
        */

       String exchangeName = "test_topic";
        //5. 创建交换机
        channel.exchangeDeclare(exchangeName, BuiltinExchangeType.TOPIC,true,false,false,null);
        //6. 创建队列
        String queue1Name = "test_topic_queue1";
        String queue2Name = "test_topic_queue2";
        channel.queueDeclare(queue1Name,true,false,false,null);
        channel.queueDeclare(queue2Name,true,false,false,null);
        //7. 绑定队列和交换机
        /*
        queueBind(String queue, String exchange, String routingKey)
        参数：
            1. queue：队列名称
            2. exchange：交换机名称
            3. routingKey：路由键，绑定规则
                如果交换机的类型为fanout ，routingKey设置为""
         */

        // routing key  系统的名称.日志的级别。
        //=需求： 所有error级别的日志存入数据库，所有order系统的日志存入数据库
        channel.queueBind(queue1Name,exchangeName,"#.error");
        channel.queueBind(queue1Name,exchangeName,"order.*");
        channel.queueBind(queue2Name,exchangeName,"*.*");

        String body = "日志信息：张三调用了findAll方法...日志级别：info...";
        //8. 发送消息
        channel.basicPublish(exchangeName,"goods.error",null,body.getBytes());
        //9. 释放资源
        channel.close();
        connection.close();
    }
}
```
`rabbitmq-consumer/src/main/java/...consumer/Consumer_Topic1.java` ：
```java
package com.itheima.consumer;

import com.rabbitmq.client.*;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class Consumer_Topic1 {
    public static void main(String[] args) throws IOException, TimeoutException {
        //1.创建连接工厂
        ConnectionFactory factory = new ConnectionFactory();
        //2. 设置参数
        factory.setHost("172.16.98.133");//ip  默认值 localhost
        factory.setPort(5672); //端口  默认值 5672
        factory.setVirtualHost("/itcast");//虚拟机 默认值/
        factory.setUsername("heima");//用户名 默认 guest
        factory.setPassword("heima");//密码 默认值 guest
        //3. 创建连接 Connection
        Connection connection = factory.newConnection();
        //4. 创建Channel
        Channel channel = connection.createChannel();

        String queue1Name = "test_topic_queue1";
        String queue2Name = "test_topic_queue2";
        /*
        basicConsume(String queue, boolean autoAck, Consumer callback)
        参数：
            1. queue：队列名称
            2. autoAck：是否自动确认
            3. callback：回调对象
         */
        // 接收消息
        Consumer consumer = new DefaultConsumer(channel){
            /*
                回调方法，当收到消息后，会自动执行该方法
                1. consumerTag：标识
                2. envelope：获取一些信息，交换机，路由key...
                3. properties:配置信息
                4. body：数据
             */
            @Override
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body) throws IOException {
              /*  System.out.println("consumerTag："+consumerTag);
                System.out.println("Exchange："+envelope.getExchange());
                System.out.println("RoutingKey："+envelope.getRoutingKey());
                System.out.println("properties："+properties);*/
                System.out.println("body："+new String(body));
                System.out.println("将日志信息存入数据库.......");
            }
        };
        channel.basicConsume(queue1Name,true,consumer);
        //关闭资源？不要
    }
}
```
---
## 4. Spring整合RabbitMQ
使用 Spring 整合 RabbitMQ ，将组件全部使用配置方式实现，简化编码。Spring 提供 RabbitTemplate 简化发送消息 API。使用**监听机制**简化消费者编码。

需求：使用 Spring 整合 RabbitMQ
步骤：生产者/消费者
①创建生产者/消费者工程
②添加依赖
③配置整合
④编写代码发送/接收消息
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.itheima</groupId>
    <artifactId>spring-rabbitmq-consumers</artifactId>
    <version>1.0-SNAPSHOT</version>
    
    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>5.1.7.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>org.springframework.amqp</groupId>
            <artifactId>spring-rabbit</artifactId>
            <version>2.1.8.RELEASE</version>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>5.1.7.RELEASE</version>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.8.0</version>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```

下面是 `rabbitmq.properties` 和 `spring-rabbitmq-producer.xml` ：
```properties
rabbitmq.host=172.16.98.133
rabbitmq.port=5672
rabbitmq.username=heima
rabbitmq.password=heima
rabbitmq.virtual-host=/itcast
```
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:rabbit="http://www.springframework.org/schema/rabbit"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       https://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/rabbit
       http://www.springframework.org/schema/rabbit/spring-rabbit.xsd">
    <!--加载配置文件-->
    <context:property-placeholder location="classpath:rabbitmq.properties"/> <!-- spring提供的名称空间 -->

    <!-- 定义rabbitmq connectionFactory; 也可使用bean标签配置,但麻烦一些 -->
    <rabbit:connection-factory id="connectionFactory" host="${rabbitmq.host}"
                               port="${rabbitmq.port}"
                               username="${rabbitmq.username}"
                               password="${rabbitmq.password}"
                               virtual-host="${rabbitmq.virtual-host}"/>
    <!--定义管理交换机、队列-->
    <rabbit:admin connection-factory="connectionFactory"/>

    <!--定义持久化队列，不存在则自动创建；不绑定到交换机则绑定到默认交换机
    默认交换机类型为direct，名字为：""，路由键为队列的名称
    -->
    <!--
        id：bean的名称
        name：queue的名称
        auto-declare:自动创建
        auto-delete:自动删除。 最后一个消费者和该队列断开连接后，自动删除队列
        exclusive:是否独占
        durable：是否持久化
    -->

    <rabbit:queue id="spring_queue" name="spring_queue"    auto-declare="true"/>

    <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~广播；所有队列都能收到消息~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
    <!--定义广播交换机中的持久化队列，不存在则自动创建-->
    <rabbit:queue id="spring_fanout_queue_1" name="spring_fanout_queue_1" auto-declare="true"/>

    <!--定义广播交换机中的持久化队列，不存在则自动创建-->
    <rabbit:queue id="spring_fanout_queue_2" name="spring_fanout_queue_2" auto-declare="true"/>

    <!--定义广播类型交换机；并绑定上述两个队列-->
    <rabbit:fanout-exchange id="spring_fanout_exchange" name="spring_fanout_exchange"  auto-declare="true">
        <rabbit:bindings>
            <rabbit:binding  queue="spring_fanout_queue_1"  />
            <rabbit:binding queue="spring_fanout_queue_2"/>
        </rabbit:bindings>
    </rabbit:fanout-exchange>

    <!--<rabbit:direct-exchange name="aa" >
        <rabbit:bindings>
            &lt;!&ndash;direct 类型的交换机绑定队列  key ：路由key  queue：队列名称&ndash;&gt;
            <rabbit:binding queue="spring_queue" key="xxx"></rabbit:binding>
        </rabbit:bindings>

    </rabbit:direct-exchange>-->

    <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~通配符；*匹配一个单词，#匹配多个单词 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
    <!--定义广播交换机中的持久化队列，不存在则自动创建-->
    <rabbit:queue id="spring_topic_queue_star" name="spring_topic_queue_star"  auto-declare="true"/>
    <!--定义广播交换机中的持久化队列，不存在则自动创建-->
    <rabbit:queue id="spring_topic_queue_well" name="spring_topic_queue_well" auto-declare="true"/>
    <!--定义广播交换机中的持久化队列，不存在则自动创建-->
    <rabbit:queue id="spring_topic_queue_well2" name="spring_topic_queue_well2" auto-declare="true"/>

    <rabbit:topic-exchange id="spring_topic_exchange"  name="spring_topic_exchange" auto-declare="true">
        <rabbit:bindings>
            <rabbit:binding pattern="heima.*"  queue="spring_topic_queue_star"/>
            <rabbit:binding pattern="heima.#" queue="spring_topic_queue_well"/>
            <rabbit:binding pattern="itcast.#" queue="spring_topic_queue_well2"/>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--定义rabbitTemplate对象操作可以在代码中方便发送消息-->
    <rabbit:template id="rabbitTemplate" connection-factory="connectionFactory"/>
</beans>
```
生产者发送消息 `ProducerTest.java` ：
```java
package com.itheima;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:spring-rabbitmq-producer.xml")
public class ProducerTest {
    //1.注入 RabbitTemplate
    @Autowired
    private RabbitTemplate rabbitTemplate;

    @Test
    public void testHelloWorld(){
        //2.发送消息
        rabbitTemplate.convertAndSend("spring_queue","hello world spring....");
    }

    /**
     * 发送fanout消息
     */
    @Test
    public void testFanout(){
        //2.发送消息
        rabbitTemplate.convertAndSend("spring_fanout_exchange","","spring fanout....");
    }

    /**
     * 发送topic消息
     */
    @Test
    public void testTopics(){
        //2.发送消息
        rabbitTemplate.convertAndSend("spring_topic_exchange","heima.hehe.haha","spring topic....");
    }
}
```
消费者 `rabbitmq.properties` 和前一个一样；`spring-rabbit-consumer.xml` ：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:rabbit="http://www.springframework.org/schema/rabbit"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       https://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/rabbit
       http://www.springframework.org/schema/rabbit/spring-rabbit.xsd">
    <!--加载配置文件-->
    <context:property-placeholder location="classpath:rabbitmq.properties"/>

    <!-- 定义rabbitmq connectionFactory -->
    <rabbit:connection-factory id="connectionFactory" host="${rabbitmq.host}"
	   port="${rabbitmq.port}"
	   username="${rabbitmq.username}"
	   password="${rabbitmq.password}"
	   virtual-host="${rabbitmq.virtual-host}"/>

    <bean id="springQueueListener" class="com.itheima.rabbitmq.listener.SpringQueueListener"/>
    <!--<bean id="fanoutListener1" class="com.itheima.rabbitmq.listener.FanoutListener1"/>
    <bean id="fanoutListener2" class="com.itheima.rabbitmq.listener.FanoutListener2"/>
    <bean id="topicListenerStar" class="com.itheima.rabbitmq.listener.TopicListenerStar"/>
    <bean id="topicListenerWell" class="com.itheima.rabbitmq.listener.TopicListenerWell"/>
    <bean id="topicListenerWell2" class="com.itheima.rabbitmq.listener.TopicListenerWell2"/>
-->
	<!-- 监听器容器 -->
    <rabbit:listener-container connection-factory="connectionFactory" auto-declare="true">
        <rabbit:listener ref="springQueueListener" queue-names="spring_queue"/>
       <!-- <rabbit:listener ref="fanoutListener1" queue-names="spring_fanout_queue_1"/>
        <rabbit:listener ref="fanoutListener2" queue-names="spring_fanout_queue_2"/>
        <rabbit:listener ref="topicListenerStar" queue-names="spring_topic_queue_star"/>
        <rabbit:listener ref="topicListenerWell" queue-names="spring_topic_queue_well"/>
        <rabbit:listener ref="topicListenerWell2" queue-names="spring_topic_queue_well2"/>-->
    </rabbit:listener-container>
</beans>
```
在 `src/main/java` 中，定义一个监听器类：
```java
package com.itheima.rabbitmq.listener;

import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;

public class SpringQueueListener implements MessageListener {
    @Override
    public void onMessage(Message message) { // 实现接口
        //打印消息
        System.out.println(new String(message.getBody()));
    }
}
```
在 `src/main/test` 中，定义一个测试类：
```java
package com.itheima;


import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:spring-rabbitmq-consumer.xml")
public class ConsumerTest {
    @Test
    public void test1(){
        boolean flag = true; // 让它不断监听
        while (true) {
        }
    }
}
```