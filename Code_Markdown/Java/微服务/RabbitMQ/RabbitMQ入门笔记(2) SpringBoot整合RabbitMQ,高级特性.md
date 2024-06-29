## 5. SpringBoot整合RabbitMQ：SpringAMQP
**SpringAMQP是基于RabbitMQ封装的一套模板**，并且还**利用SpringBoot对其实现了自动装配**，使用起来非常方便。SpringAmqp的官方地址：https://spring.io/projects/spring-amqp
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071435155.png)
SpringAMQP提供了三个功能：
- yml中配置基本信息，配置类中使用Bean的方式、自动声明队列Queue、交换机Exchange及其绑定关系Binding
- **封装了RabbitTemplate工具**用于发送消息，生产端直接注入RabbitTemplate完成消息发送
- **基于注解的监听器模式**异步接收消息（便捷的消息发送和接收），消费端直接使用@RabbitListener完成消息接收

实现步骤：
1. 创建SpringBoot工程，创建消费者和生产者两个模块。或者直接创建两个项目。
2. 引入start，依赖坐标
<dependency>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
3. 编写yml配置，基本信息配置
4. 定义监听类，使用@RabbitListener注解完成队列监听。
## 5.1 利用SpringAMQP实现HelloWorld中的Basic Queue简单队列模型
在父工程mq-demo中引入依赖，因为publisher和consumer服务都需要amqp依赖，因此这里把依赖直接放到父工程mq-demo中：
```xml
<!--AMQP依赖，包含RabbitMQ-->
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
```
### 5.1.1 消息发送
首先配置RabbitMQ地址，在publisher服务的 `application.yml` 中添加配置，添加mq连接信息：
```yaml
spring:
  rabbitmq:
    host: 192.168.150.101 # 主机名
    port: 5672 # 端口
    virtual-host: / # 虚拟主机
    username: itcast # 用户名
    password: 123321 # 密码
```
然后在publisher服务中编写测试类SpringAmqpTest，编写测试方法：利用RabbitTemplate实现消息发送到simple.queue这个队列：
```java
package cn.itcast.mq.spring;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

@RunWith(SpringRunner.class) //这个应该不用加了,ALT+INSERT快捷键
@SpringBootTest
public class SpringAmqpTest {
    @Autowired
    private RabbitTemplate rabbitTemplate;
    @Test
    public void testSimpleQueue() {
        // 队列名称
        String queueName = "simple.queue";
        // 消息
        String message = "hello, spring amqp!";
        // 发送消息
        rabbitTemplate.convertAndSend(queueName, message);
    }
}
```
### 5.1.2 消息接收
首先配置MQ地址，在consumer服务的application.yml中添加配置：
```yaml
spring:
  rabbitmq:
    host: 192.168.150.101 # 主机名
    port: 5672 # 端口
    virtual-host: / # 虚拟主机
    username: itcast # 用户名
    password: 123321 # 密码
```
然后在consumer服务的`cn.itcast.mq.listener`包中新建一个类SpringRabbitListener，代码如下：
```java
package cn.itcast.mq.listener;

import org.springframework.amqp.rabbit.annotation.RabbitListener;
import org.springframework.stereotype.Component;

@Component
public class SpringRabbitListener {
    @RabbitListener(queues = "simple.queue")
    public void listenSimpleQueueMessage(String msg) throws InterruptedException {
        System.out.println("spring 消费者接收到消息：【" + msg + "】");
    }
}
```
### 5.1.3 测试
启动consumer服务，然后在publisher服务中运行测试代码，发送MQ消息。
> 上节课没有消息的去queues里面创建一个消息 

## 5.2.WorkQueue
Work queues，也被称为（Task queues），任务模型。简单来说就是**让多个消费者绑定到一个队列，共同消费队列中的消息**。可以**提高消息处理速度，避免队列消息堆积**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071515372.png)
当消息处理比较耗时的时候，可能生产消息的速度会远远大于消息的消费速度。长此以往，消息就会堆积越来越多，无法及时处理。此时就可以使用 `work queue` 模型，多个消费者共同处理消息处理，速度就能大大提高了。
### 5.2.1 消息发送
这次我们循环发送，模拟大量消息堆积现象。在 `publisher` 服务中的 `SpringAmqpTest` 类中添加一个测试方法，每秒产生50条消息，发送到simple.queue：
```java
/**
 * workQueue
 * 向队列中不停发送消息，模拟消息堆积。
 */
@Test
public void testWorkQueue() throws InterruptedException {
    // 队列名称
    String queueName = "simple.queue";
    // 消息
    String message = "hello, message_";
    for (int i = 0; i < 50; i++) {
        // 发送消息
        rabbitTemplate.convertAndSend(queueName, message + i);
        Thread.sleep(20);
    }
}
```
### 5.2.2 消息接收
**要模拟多个消费者绑定同一个队列**，我们在consumer服务的SpringRabbitListener中添加2个新的方法，定义两个消息监听者，都监听simple.queue队列。消费者1每秒处理50条消息，消费者2每秒处理10条消息：
```java
@RabbitListener(queues = "simple.queue")
public void listenWorkQueue1(String msg) throws InterruptedException {
    System.out.println("消费者1接收到消息：【" + msg + "】" + LocalTime.now());
    Thread.sleep(20);
}

@RabbitListener(queues = "simple.queue")
public void listenWorkQueue2(String msg) throws InterruptedException {
    System.err.println("消费者2........接收到消息：【" + msg + "】" + LocalTime.now());
    Thread.sleep(100);
}
```
注意到这个消费者sleep了100毫秒，模拟任务耗时。
### 5.2.3 测试
启动ConsumerApplication后，在执行publisher服务中刚刚编写的发送测试方法testWorkQueue。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071513399.png)
可以看到消费者1很快完成了自己的25条消息。消费者2却在缓慢的处理自己的25条消息。也就是说消息是平均分配给每个消费者，并没有考虑到消费者的处理能力。这样显然是有问题的。

这是因为RabbitMQ的消息预取机制。
### 5.2.4 能者多劳
在spring中有一个简单的配置，可以解决这个问题。我们修改consumer服务的application.yml文件，设置preFetch这个值，可以控制预取消息的上限：
```yaml
spring:
  rabbitmq:
    listener:
      simple:
        prefetch: 1 # 每次只能获取一条消息，处理完成才能获取下一个消息
```
### 5.2.5 总结
Work模型的使用：
- ==多个消费者绑定到一个队列，同一条消息只会被一个消费者处理==（竞争）
- 通过设置prefetch来控制消费者预取的消息数量

## 5.3 发布/订阅
发布订阅的模型如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071516063.png)
可以看到，在订阅模型中，多了一个exchange角色，而且过程略有变化：
- Publisher：生产者，也就是要发送消息的程序，但**不再发送到队列中，而是发给X（交换机）**
- Exchange：交换机，图中的X。一方面，接收生产者发送的消息。另一方面，知道==如何处理消息，例如递交给某个特别队列、递交给所有队列、或是将消息丢弃==。到底如何操作，取决于Exchange的类型。Exchange有以下3种类型：
	- Fanout：广播，将消息交给所有绑定到交换机的队列
	- Direct：定向，把消息交给符合指定 `routing key` 的队列
	- Topic：通配符，把消息交给符合 `routing pattern`（路由模式） 的队列
- Consumer：消费者，与以前一样，订阅队列，没有变化
- Queue：消息队列也与以前一样，接收消息、缓存消息。

**Exchange（交换机）只负责转发消息，不具备存储消息的能力**，因此==如果没有任何队列与Exchange绑定，或者没有符合路由规则的队列，那么消息会丢失==！
## 5.4 Fanout Exchange
Fanout，英文翻译是扇出，我觉得在MQ中叫广播更合适。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071539016.png)
在广播模式下，消息发送流程是这样的：
- 1）可以有多个队列
- 2）每个队列都要绑定到Exchange（交换机）
- 3）==生产者发送的消息，只能发送到交换机，交换机来决定要发给哪个队列，生产者无法决定==
- 4）==交换机把消息发送给绑定过的所有队列==
- 5）订阅队列的消费者都能拿到消息

我们的计划是这样的：
- 创建一个交换机 itcast.fanout，类型是Fanout
- 创建两个队列fanout.queue1和fanout.queue2，绑定到交换机itcast.fanout
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071544590.png)

### 5.4.1 声明队列和交换机
Spring提供了一个接口Exchange，来表示所有不同类型的交换机：

在consumer中创建一个类，利用代码声明队列、交换机，并将两者绑定：
```java
package cn.itcast.mq.config;

import org.springframework.amqp.core.Binding;
import org.springframework.amqp.core.BindingBuilder;
import org.springframework.amqp.core.FanoutExchange;
import org.springframework.amqp.core.Queue;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
public class FanoutConfig {
    /**
     * 声明交换机
     * @return Fanout类型交换机
     */
    @Bean
    public FanoutExchange fanoutExchange(){
        return new FanoutExchange("itcast.fanout");
    }

    /**
     * 第1个队列
     */
    @Bean
    public Queue fanoutQueue1(){
        return new Queue("fanout.queue1");
    }

    /**
     * 绑定队列和交换机
     */
    @Bean
    public Binding bindingQueue1(Queue fanoutQueue1, FanoutExchange fanoutExchange){
        return BindingBuilder.bind(fanoutQueue1).to(fanoutExchange);
    }

    /**
     * 第2个队列
     */
    @Bean
    public Queue fanoutQueue2(){
        return new Queue("fanout.queue2");
    }

    /**
     * 绑定队列和交换机
     */
    @Bean
    public Binding bindingQueue2(Queue fanoutQueue2, FanoutExchange fanoutExchange){
        return BindingBuilder.bind(fanoutQueue2).to(fanoutExchange);
    }
}
```
### 5.4.2 消息发送
在publisher服务的SpringAmqpTest类中添加测试方法：
```java
@Test
public void testFanoutExchange() {
    // 队列名称
    String exchangeName = "itcast.fanout";
    // 消息
    String message = "hello, everyone!";
    rabbitTemplate.convertAndSend(exchangeName, "", message);
}
```
### 5.4.3 消息接收
在consumer服务的SpringRabbitListener中添加两个方法，作为消费者分别监听fanout.queue1和fanout.queue2：
```java
@RabbitListener(queues = "fanout.queue1")
public void listenFanoutQueue1(String msg) {
    System.out.println("消费者1接收到Fanout消息：【" + msg + "】");
}

@RabbitListener(queues = "fanout.queue2")
public void listenFanoutQueue2(String msg) {
    System.out.println("消费者2接收到Fanout消息：【" + msg + "】");
}
```
### 5.4.4 总结
交换机的作用是什么？
- **接收publisher发送的消息**
- 将消息按照规则路由到**与之绑定的队列**
- 不能缓存消息，路由失败，消息丢失
- FanoutExchange会将消息路由到每个绑定的队列

声明队列、交换机、绑定关系的Bean是什么？
- Queue
- FanoutExchange
- Binding

## 5.5 Direct Exchange
在Fanout模式中，一条消息会被所有订阅的队列都消费。但在某些场景下，我们希望不同的消息被不同的队列消费。这时就要用到Direct类型的Exchange。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071559755.png)
Direct模型下：
- **队列与交换机的绑定**，不能是任意绑定了，而是**要指定一个 `RoutingKey`**（路由key）
- 消息的发送方在向 Exchange发送消息时，也必须指定消息的 `RoutingKey`。
- Exchange不再把消息交给每一个绑定的队列，而是根据消息的`Routing Key`进行判断，只有队列的`Routingkey`与消息的 `Routing key`完全一致，才会接收到消息

**案例需求如下**：
1. 利用@RabbitListener声明Exchange、Queue、RoutingKey
2. 在consumer服务中，编写两个消费者方法，分别监听direct.queue1和direct.queue2
3. 在publisher中编写测试方法，向itcast. direct发送消息

### 3.5.1.基于注解声明队列和交换机
==基于@Bean的方式声明队列和交换机比较麻烦，Spring还提供了基于注解方式来声明==。

在consumer的SpringRabbitListener中添加两个消费者，同时**基于注解来声明队列和交换机**：
> 这里是监听，不会自动创建。但是你在配置类配置的话就会创建。
```java
@RabbitListener(bindings = @QueueBinding(
    value = @Queue(name = "direct.queue1"),
    exchange = @Exchange(name = "itcast.direct", type = ExchangeTypes.DIRECT),
    key = {"red", "blue"}
))
public void listenDirectQueue1(String msg){
    System.out.println("消费者接收到direct.queue1的消息：【" + msg + "】");
}

@RabbitListener(bindings = @QueueBinding(
    value = @Queue(name = "direct.queue2"),
    exchange = @Exchange(name = "itcast.direct", type = ExchangeTypes.DIRECT),
    key = {"red", "yellow"}
))
public void listenDirectQueue2(String msg){
    System.out.println("消费者接收到direct.queue2的消息：【" + msg + "】");
}
```
> 不是在消费者里指定交换机的类型，他前面只是告诉你，spring可以支持注解快速的实现一个交换机，而不需要去配置里写一个bean然后返回一个对象这样
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071604107.png)

### 5.5.2 消息发送
在publisher服务的SpringAmqpTest类中添加测试方法：
```java
@Test
public void testSendDirectExchange() {
    // 交换机名称
    String exchangeName = "itcast.direct";
    // 消息
    String message = "红色警报！日本乱排核废水，导致海洋生物变异，惊现哥斯拉！";
    // 发送消息
    rabbitTemplate.convertAndSend(exchangeName, "red", message);
}
```
### 5.5.3 总结
描述下Direct交换机与Fanout交换机的差异？
- Fanout交换机将消息路由给每一个与之绑定的队列
- Direct交换机根据RoutingKey判断路由给哪个队列
- 如果多个队列具有相同的RoutingKey，则与Fanout功能类似

基于@RabbitListener注解声明队列和交换机有哪些常见注解？
- @Queue
- @Exchange

## 5.6 Topic Exchange
### 5.6.1 说明
`Topic`类型的 `Exchange` 与`Direct`相比，都是可以根据 `RoutingKey` 把消息路由到不同的队列。只不过`Topic`类型 `Exchange` 可以**让队列在绑定 `Routing key` 时使用通配符**！

`Routingkey` 一般都是有一个或多个单词组成，多个单词之间以”.”分割，例如： `item.insert` 。通配符规则：
- `#`：匹配一个或多个词
- `*`：匹配不多不少恰好1个词

举例：`item.#`：能够匹配 `item.spu.insert` 或者 `item.spu` 。`item.*`：只能匹配`item.spu` 。图示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071611024.png)
解释：
- Queue1：绑定的是 `china.#` ，因此凡是以 `china.`开头的`routing key` 都会被匹配到。包括china.news和china.weather
- Queue2：绑定的是 `#.news` ，因此凡是以 `.news`结尾的 `routing key` 都会被匹配。包括china.news和japan.news

案例需求：实现思路如下：
1. 并利用@RabbitListener声明Exchange、Queue、RoutingKey
2. 在consumer服务中，编写两个消费者方法，分别监听topic.queue1和topic.queue2
3. 在publisher中编写测试方法，向itcast. topic发送消息
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303071612890.png)

### 5.6.2 消息发送
在publisher服务的SpringAmqpTest类中添加测试方法：
```java
/**
 * topicExchange
 */
@Test
public void testSendTopicExchange() {
    // 交换机名称
    String exchangeName = "itcast.topic";
    // 消息
    String message = "喜报！孙悟空大战哥斯拉，胜!";
    // 发送消息
    rabbitTemplate.convertAndSend(exchangeName, "china.news", message);
}
```
### 5.6.3 消息接收
在consumer服务的SpringRabbitListener中添加方法：
```java
@RabbitListener(bindings = @QueueBinding(
    value = @Queue(name = "topic.queue1"),
    exchange = @Exchange(name = "itcast.topic", type = ExchangeTypes.TOPIC),
    key = "china.#"
))
public void listenTopicQueue1(String msg){
    System.out.println("消费者接收到topic.queue1的消息：【" + msg + "】");
}

@RabbitListener(bindings = @QueueBinding(
    value = @Queue(name = "topic.queue2"),
    exchange = @Exchange(name = "itcast.topic", type = ExchangeTypes.TOPIC),
    key = "#.news"
))
public void listenTopicQueue2(String msg){
    System.out.println("消费者接收到topic.queue2的消息：【" + msg + "】");
}
```
### 5.6.4 总结
描述下Direct交换机与Topic交换机的差异？
- Topic交换机接收的消息 `RoutingKey` 必须是多个单词，以 `**.**` 分割
- Topic交换机与队列绑定时的 `bindingKey` 可以指定通配符
- `#`：代表0个或多个词
- `*`：代表1个词

## 5.7 消息转换器
之前说过，Spring会把你发送的消息序列化为字节发送给MQ，接收消息的时候，还会把字节反序列化为Java对象。
> 前面rabbitmq快速入门有手动序列化和反序列化过

只不过，默认情况下Spring采用的序列化方式是JDK序列化。众所周知，JDK序列化存在下列问题：
- 数据体积过大
- 有安全漏洞
- 可读性差

我们来测试一下。
### 5.7.1 测试默认转换器：发送Object类型消息
说明：在SpringAMQP的发送方法中，接收消息的类型是Object，也就是说我们可以发送任意对象类型的消息，**SpringAMQP会帮我们序列化为字节后发送**。

我们在consumer中利用@Bean声明一个队列：
```java
@Bean 
public Queue objectMessageQueue(){   
	return new Queue("object.queue");
}
```
在publisher中发送消息以测试。我们修改消息发送的代码，发送一个Map对象：
```java
@Test
public void testSendMap() throws InterruptedException {
    // 准备消息
    Map<String,Object> msg = new HashMap<>();
    msg.put("name", "Jack");
    msg.put("age", 21);
    // 发送消息
    rabbitTemplate.convertAndSend("simple.queue","", msg);
}
```
停止consumer服务，发送消息后查看控制台。

### 5.7.2 配置JSON转换器
Spring的对消息对象的处理是由 `org.springframework.amqp.support.converter.MessageConverter` 来处理的。而默认实现是SimpleMessageConverter，基于JDK的ObjectOutputStream完成序列化。显然，==JDK序列化方式并不合适。我们希望消息体的体积更小、可读性更高，因此可以使用JSON方式来做序列化和反序列化==。

如果要修改，只需要定义一个MessageConverter类型的Bean即可。推荐用JSON方式序列化，步骤如下。在publisher和consumer两个服务中都引入依赖：
```xml
<dependency>
    <groupId>com.fasterxml.jackson.dataformat</groupId>
    <artifactId>jackson-dataformat-xml</artifactId>
    <version>2.9.10</version>
</dependency>
```
配置消息转换器。

我们在publisher服务声明MessageConverter。在启动类中添加一个Bean即可：
```java
@Bean
public MessageConverter jsonMessageConverter(){
    return new Jackson2JsonMessageConverter();
}
```
我们在consumer服务定义MessageConverter：
```java
@Bean
public MessageConverter jsonMessageConverter() {    
	return new Jackson2JsonMessageConverter(); 
}
```
然后定义一个消费者，监听object.queue队列并消费消息：
```java
@RabbitListener(queues = "object.queue")
public void listenObjectQueue(Map<String, Object> msg) {
	System.out.println("收到消息：【" + msg + "】"); 
}
```

---
# 6. RabbitMQ高级特性
生产者消费者都需要rabbitmq.properties（下面都是Spring项目）：
```properties
rabbitmq.host=172.16.98.133
rabbitmq.port=5672
rabbitmq.username=guest
rabbitmq.password=guest
rabbitmq.virtual-host=/
```
## 6.1 消息的可靠投递
在使用 RabbitMQ 的时候，**作为消息发送方希望杜绝任何消息丢失或者投递失败场景**。RabbitMQ 为我们提供了两种方式用来控制消息的投递可靠性模式。
- confirm **确认模式**
- return **退回模式**

rabbitmq 整个消息投递的路径为：
producer--->rabbitmq broker---> exchange--->queue--->consumer
- 消息从 producer 到 exchange 则会返回一个 `confirmCallback` 回调函数，不管消息是否成功投递，都会执行这个回调函数，只是看参数是 `true` 还是 `false` 。
- 消息从 exchange-->queue 投递失败则会返回一个 `returnCallback` 。

我们将利用这两个 callback 控制消息的可靠性投递。

➢ 设置 `ConnectionFactory` 的 `publisher-confirms="true"` 开启 确认模式。
➢ 使用 `rabbitTemplate.setConfirmCallback` 设置回调函数。**当消息发送到exchange后回调confirm方法**。在方法中判断ack，如果为true，则发送成功，如果为false，则发送失败，需要处理。

➢ 设置ConnectionFactory的 `publisher-returns="true"` 开启 退回模式。
➢ 使用 `rabbitTemplate.setReturnCallback` 设置退回函数，当消息从exchange路由到queue失败后，如果设置了 `rabbitTemplate.setMandatory(true)` 参数，则会将消息退回给 `producer` ；否则丢弃。并执行回调函数 `returnedMessage` 。

➢ 在RabbitMQ中也提供了事务机制，但是性能较差，此处不做讲解。
使用channel下列方法，完成事务控制：
txSelect(), 用于将当前channel设置成transaction模式
txCommit()，用于提交事务
txRollback(),用于回滚事务

`spring-rabbitmq-producer.xml` ：
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
	   virtual-host="${rabbitmq.virtual-host}"
	   publisher-confirms="true"
	   publisher-returns="true"
    />
    <!--定义管理交换机、队列-->
    <rabbit:admin connection-factory="connectionFactory"/>

    <!--定义rabbitTemplate对象操作可以在代码中方便发送消息-->
    <rabbit:template id="rabbitTemplate" connection-factory="connectionFactory"/>

    <!--消息可靠性投递（生产端）-->
    <rabbit:queue id="test_queue_confirm" name="test_queue_confirm"></rabbit:queue>
    <rabbit:direct-exchange name="test_exchange_confirm">
        <rabbit:bindings>
            <rabbit:binding queue="test_queue_confirm" key="confirm"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:direct-exchange>

    <!--ttl-->
    <rabbit:queue name="test_queue_ttl" id="test_queue_ttl">
        <!--设置queue的参数-->
        <rabbit:queue-arguments>
            <!--x-message-ttl指队列的过期时间-->
            <entry key="x-message-ttl" value="100000" value-type="java.lang.Integer"></entry>
        </rabbit:queue-arguments>
    </rabbit:queue>

    <rabbit:topic-exchange name="test_exchange_ttl" >
        <rabbit:bindings>
            <rabbit:binding pattern="ttl.#" queue="test_queue_ttl"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--
        死信队列：
            1. 声明正常的队列(test_queue_dlx)和交换机(test_exchange_dlx)
            2. 声明死信队列(queue_dlx)和死信交换机(exchange_dlx)
            3. 正常队列绑定死信交换机
                设置两个参数：
			* x-dead-letter-exchange：死信交换机名称
			* x-dead-letter-routing-key：发送给死信交换机的routingkey
    -->
    <!--
        1. 声明正常的队列(test_queue_dlx)和交换机(test_exchange_dlx)
    -->
    <rabbit:queue name="test_queue_dlx" id="test_queue_dlx">
        <!--3. 正常队列绑定死信交换机-->
        <rabbit:queue-arguments>
            <!--3.1 x-dead-letter-exchange：死信交换机名称-->
            <entry key="x-dead-letter-exchange" value="exchange_dlx" />

            <!--3.2 x-dead-letter-routing-key：发送给死信交换机的routingkey-->
            <entry key="x-dead-letter-routing-key" value="dlx.hehe" />

            <!--4.1 设置队列的过期时间 ttl-->
            <entry key="x-message-ttl" value="10000" value-type="java.lang.Integer" />
            <!--4.2 设置队列的长度限制 max-length -->
            <entry key="x-max-length" value="10" value-type="java.lang.Integer" />
        </rabbit:queue-arguments>
    </rabbit:queue>
    <rabbit:topic-exchange name="test_exchange_dlx">
        <rabbit:bindings>
            <rabbit:binding pattern="test.dlx.#" queue="test_queue_dlx"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--
       2. 声明死信队列(queue_dlx)和死信交换机(exchange_dlx)
   -->
    <rabbit:queue name="queue_dlx" id="queue_dlx"></rabbit:queue>
    <rabbit:topic-exchange name="exchange_dlx">
        <rabbit:bindings>
            <rabbit:binding pattern="dlx.#" queue="queue_dlx"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--
        延迟队列：
            1. 定义正常交换机（order_exchange）和队列(order_queue)
            2. 定义死信交换机（order_exchange_dlx）和队列(order_queue_dlx)
            3. 绑定，设置正常队列过期时间为30分钟
    -->
    <!-- 1. 定义正常交换机（order_exchange）和队列(order_queue)-->
    <rabbit:queue id="order_queue" name="order_queue">
        <!-- 3. 绑定，设置正常队列过期时间为30分钟-->
        <rabbit:queue-arguments>
            <entry key="x-dead-letter-exchange" value="order_exchange_dlx" />
            <entry key="x-dead-letter-routing-key" value="dlx.order.cancel" />
            <entry key="x-message-ttl" value="10000" value-type="java.lang.Integer" />
        </rabbit:queue-arguments>
    </rabbit:queue>
    <rabbit:topic-exchange name="order_exchange">
        <rabbit:bindings>
            <rabbit:binding pattern="order.#" queue="order_queue"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--  2. 定义死信交换机（order_exchange_dlx）和队列(order_queue_dlx)-->
    <rabbit:queue id="order_queue_dlx" name="order_queue_dlx"></rabbit:queue>
    <rabbit:topic-exchange name="order_exchange_dlx">
        <rabbit:bindings>
            <rabbit:binding pattern="dlx.order.#" queue="order_queue_dlx"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>
</beans>
```
`Producer` 的测试类：
```java
package com.itheima.test;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.amqp.AmqpException;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessagePostProcessor;
import org.springframework.amqp.rabbit.connection.CorrelationData;
import org.springframework.amqp.rabbit.core.RabbitTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:spring-rabbitmq-producer.xml")
public class ProducerTest {
    @Autowired
    private RabbitTemplate rabbitTemplate;
    /**
     * 确认模式：
     * 步骤：
     * 1. 确认模式开启：ConnectionFactory中开启publisher-confirms="true"
     * 2. 在rabbitTemplate定义ConfirmCallBack回调函数
     */
    @Test
    public void testConfirm() {
        //2. 定义回调,故意发给不存在的交换机
        rabbitTemplate.setConfirmCallback(new RabbitTemplate.ConfirmCallback() {
            /**
             * @param correlationData 相关配置信息
             * @param ack   exchange交换机 是否成功收到了消息。true 成功，false代表失败
             * @param cause 失败原因
             */
            @Override
            public void confirm(CorrelationData correlationData, boolean ack, String cause) {
                System.out.println("confirm方法被执行了....");
                if (ack) {
                    //接收成功
                    System.out.println("接收成功消息" + cause);
                } else {
                    //接收失败
                    System.out.println("接收失败消息" + cause);
                    //做一些处理，让消息再次发送。
                }
            }
        });
        //3. 发送消息
        rabbitTemplate.convertAndSend("test_exchange_confirm111", "confirm", "message confirm....");
    }

    /**
     * 回退模式： 当消息发送给Exchange后，Exchange路由到Queue失败是 才会执行 ReturnCallBack
     * 步骤：
     * 1. 开启回退模式:publisher-returns="true"
     * 2. 设置ReturnCallBack
     * 3. 设置Exchange处理消息的模式：
     * 1. 如果消息没有路由到Queue，则丢弃消息（默认）
     * 2. 如果消息没有路由到Queue，返回给消息发送方ReturnCallBack
     */
    @Test
    public void testReturn() {
        //设置交换机处理失败消息的模式
        rabbitTemplate.setMandatory(true); //返回给生产者
        //2.设置ReturnCallBack
        rabbitTemplate.setReturnCallback(new RabbitTemplate.ReturnCallback() {
            /**
             * @param message   消息对象
             * @param replyCode 错误码
             * @param replyText 错误信息
             * @param exchange  交换机
             * @param routingKey 路由键
             */
            @Override
            public void returnedMessage(Message message, int replyCode, String replyText, String exchange, String routingKey) {
                System.out.println("return 执行了....");
                System.out.println(message);
                System.out.println(replyCode);
                System.out.println(replyText);
                System.out.println(exchange);
                System.out.println(routingKey);
                //处理
            }
        });
        //3. 发送消息
        rabbitTemplate.convertAndSend("test_exchange_confirm", "confirm", "message confirm....");
    }

    @Test
    public void testSend() {
        for (int i = 0; i < 10; i++) {
            // 发送消息
            rabbitTemplate.convertAndSend("test_exchange_confirm", "confirm", "message confirm....");
        }
    }

    /**
     * TTL:过期时间
     *  1. 队列统一过期
     *  2. 消息单独过期
     * 如果设置了消息的过期时间，也设置了队列的过期时间，它以时间短的为准。
     * 队列过期后，会将队列所有消息全部移除。
     * 消息过期后，只有消息在队列顶端，才会判断其是否过期(移除掉)
     *
     */
    @Test
    public void testTtl() {
      /*  for (int i = 0; i < 10; i++) {
            // 发送消息
            rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....");
        }*/
      // 消息后处理对象，设置一些消息的参数信息
        MessagePostProcessor messagePostProcessor = new MessagePostProcessor() {

            @Override
            public Message postProcessMessage(Message message) throws AmqpException {
                //1.设置message的信息
                message.getMessageProperties().setExpiration("5000");//消息的过期时间
                //2.返回该消息
                return message;
            }
        };
        //消息单独过期
        //rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....",messagePostProcessor);
        for (int i = 0; i < 10; i++) {
            if(i == 5){
                //消息单独过期
                rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....",messagePostProcessor);
            }else{
                //不过期的消息
                rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....");
            }
        }
    }
    /**
     * 发送测试死信消息：
     *  1. 过期时间
     *  2. 长度限制
     *  3. 消息拒收
     */
    @Test
    public void testDlx(){
        //1. 测试过期时间，死信消息
        //rabbitTemplate.convertAndSend("test_exchange_dlx","test.dlx.haha","我是一条消息，我会死吗？");
        //2. 测试长度限制后，消息死信
       /* for (int i = 0; i < 20; i++) {
	        rabbitTemplate.convertAndSend("test_exchange_dlx","test.dlx.haha","我是一条消息，我会死吗？");
        }*/
        //3. 测试消息拒收
        rabbitTemplate.convertAndSend("test_exchange_dlx","test.dlx.haha","我是一条消息，我会死吗？");
    }

    @Test
    public  void testDelay() throws InterruptedException {
        //1.发送订单消息。 将来是在订单系统中，下单成功后，发送消息
        rabbitTemplate.convertAndSend("order_exchange","order.msg","订单信息：id=1,time=2019年8月17日16:41:47");
        /*//2.打印倒计时10秒
        for (int i = 10; i > 0 ; i--) {
            System.out.println(i+"...");
            Thread.sleep(1000);
        }*/
    }
}
```

## 6.2 Consumer Ack
ack指Acknowledge，确认。 表示消费端收到消息后的确认方式。有三种确认方式：
- 自动确认：acknowledge="none"，默认是这种方式
- 手动确认：acknowledge="manual"
- 根据异常情况确认：acknowledge="auto"，（这种方式使用麻烦，不作讲解）

其中自动确认是指，当消息一旦被Consumer接收到，则**自动确认收到**，并将相应 message 从 RabbitMQ 的消息缓存中移除。==但在实际业务处理中，很可能消息接收到，业务处理出现异常，那么该消息就会丢失==。

如果设置了手动确认方式，则需要在业务处理成功后，调用 `channel.basicAck()` ，手动签收，如果出现异常，则调用 `channel.basicNack()` 方法，**让其自动重新发送消息**。

下面的给消息的确认方式实验。
- 在rabbit:listener-container标签中设置acknowledge属性，设置ack方式 none：自动确认，manual：手动确认
- 如果在消费端没有出现异常，则调用 `channel.basicAck(deliveryTag,false);` 方法确认签收消息
- 如果出现异常，则在catch中调用 basicNack或 basicReject，拒绝消息，让MQ重新发送消息。
`spring-rabbitmq-consumer.xml` ：
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
	<!-- 包扫描 -->
    <context:component-scan base-package="com.itheima.listener" />

    <!--定义监听器容器-->
    <rabbit:listener-container connection-factory="connectionFactory" acknowledge="manual" prefetch="1" >
       <!-- <rabbit:listener ref="ackListener" queue-names="test_queue_confirm"></rabbit:listener>-->
       <!-- <rabbit:listener ref="qosListener" queue-names="test_queue_confirm"></rabbit:listener>-->
        <!--定义监听器，监听正常队列-->
        <!--<rabbit:listener ref="dlxListener" queue-names="test_queue_dlx"></rabbit:listener>-->

        <!--延迟队列效果实现：  一定要监听的是 死信队列！！！-->
        <rabbit:listener ref="orderListener" queue-names="order_queue_dlx"></rabbit:listener>
    </rabbit:listener-container>
</beans>
```
注册为Bean，并在 `spring-rabbitmq-consumer.xml` 中的监听器容器中引用这个Bean：
```java
package com.itheima.listener;

import com.rabbitmq.client.Channel;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.core.MessageListener;
import org.springframework.amqp.rabbit.listener.api.ChannelAwareMessageListener;
import org.springframework.stereotype.Component;

import java.io.IOException;
/**
 * Consumer ACK机制：
 *  1. 设置手动签收。acknowledge="manual"
 *  2. 让监听器类实现ChannelAwareMessageListener接口
 *  3. 如果消息成功处理，则调用channel的 basicAck()签收
 *  4. 如果消息处理失败，则调用channel的basicNack()拒绝签收，broker重新发送给consumer
 */
@Component
public class AckListener implements ChannelAwareMessageListener {
    @Override
    public void onMessage(Message message, Channel channel) throws Exception {
        long deliveryTag = message.getMessageProperties().getDeliveryTag();
        try {
            //1.接收转换消息
            System.out.println(new String(message.getBody()));

            //2. 处理业务逻辑
            System.out.println("处理业务逻辑...");
            int i = 3/0; //出现错误
            //3. 手动签收
            channel.basicAck(deliveryTag, true);
        } catch (Exception e) {
            //e.printStackTrace();

            //4.拒绝签收
            /* 第三个参数：requeue：重回队列。如果设置为true，则消息重新回到queue，broker会重新发送该消息给消费端 */
            channel.basicNack(deliveryTag,true,true);
            //channel.basicReject(deliveryTag,true);
        }
    }
}
```
## 6.3 消息可靠性总结
1.持久化
- exchange要持久化
- queue要持久化
- message要持久化。当Broker重启后，这些都在

2.生产方确认Confirm
3.消费方确认Ack
4.Broker高可用

出了问题没有处理，后面讲。

## 6.4 消费端限流
> 他这里的图简化了，实际上用户和中间件中间肯定是有其他微服务的

![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110122955.png)
Consumer限流机制，见上面的配置和下面的QosListener。
在`<rabbit:listener-container>` 中配置 prefetch属性，设置消费端一次拉取多少消息
➢ **消费端的确认模式一定为手动确认**。`acknowledge="manual"`
```java
package com.itheima.listener;

import com.rabbitmq.client.Channel;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.listener.api.ChannelAwareMessageListener;
import org.springframework.stereotype.Component;

/**
 * Consumer限流机制
 *  1. 确保ack机制为手动确认。
 *  2. listener-container配置属性
 *      perfetch = 1,表示消费端每次从mq拉去一条消息来消费，直到手动确认消费完毕后，才会继续拉去下一条消息。
 */
@Component
public class QosListener implements ChannelAwareMessageListener {
    @Override
    public void onMessage(Message message, Channel channel) throws Exception {
        Thread.sleep(1000);
        //1.获取消息
        System.out.println(new String(message.getBody()));
        //2. 处理业务逻辑
        //3. 签收
        channel.basicAck(message.getMessageProperties().getDeliveryTag(),true);
    }
}
```

## 6.5 TTL 
TTL 全称 Time To Live（存活时间/过期时间）。
➢ 当消息到达存活时间后，还没有被消费，会被自动清除。
➢ RabbitMQ可以对消息设置过期时间，也可以对整个队列（Queue）设置过期时间。
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110129858.png)
➢设置队列过期时间使用参数：x-message-ttl，单位：ms(毫秒)，会对整个队列消息统一过期。
➢ 设置消息过期时间使用参数：expiration。单位：ms(毫秒)，当该消息在队列头部时（消费时），会单独判断这一消息是否过期。
➢ 如果两者都进行了设置，以时间短的为准。

可以在界面的管理控制台上新建立Exchange和Queue：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110135019.png)

队列的统一过期：
```xml
<rabbit:queue name="test_queue_ttl" id="test_queue_ttl">
	<!--设置queue的参数-->
	<rabbit:queue-arguments>
		<!--x-message-ttl指队列的过期时间-->
		<entry key="x-message-ttl" value="100000" value-type="java.lang.Integer"></entry>
	</rabbit:queue-arguments>
</rabbit:queue>

<rabbit:topic-exchange name="test_exchange_ttl" >
	<rabbit:bindings>
		<rabbit:binding pattern="ttl.#" queue="test_queue_ttl"></rabbit:binding>
	</rabbit:bindings>
</rabbit:topic-exchange>
```
消息的单独过期：
```java
/**
 * TTL: 过期时间
 *  1. 队列统一过期
 *  2. 消息单独过期
 * 如果设置了消息的过期时间，也设置了队列的过期时间，它以时间短的为准。
 * 队列过期后，会将队列所有消息全部移除。
 * 队列设置ttl是指每个进入这个队列的消息过期时间都一样吧，不是指到了这个时间以后直接删除队列中全部的消息吧？出队的时候判断下过期了就不发给消费者就行了
 * 和redis一样的，redis也是惰性删除。只有用到的时候才检验是否过期。前面的，redis过期机制包括惰性和周期删除
 * 消息过期后，只有消息在队列顶端,将被消费，才会判断其是否过期(移除掉); 轮询队列所有消息看是否过期太耗时
 */
@Test
public void testTtl() {
  /*  for (int i = 0; i < 10; i++) {
		// 发送消息
		rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....");
	}*/
  // 消息后处理对象，设置一些消息的参数信息
	MessagePostProcessor messagePostProcessor = new MessagePostProcessor() {
		@Override
		public Message postProcessMessage(Message message) throws AmqpException {
			//1.设置message的信息
			message.getMessageProperties().setExpiration("5000");//消息的过期时间
			//2.返回该消息
			return message;
		}
	};
	
	//消息单独过期
	//rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....", messagePostProcessor);
	for (int i = 0; i < 10; i++) {
		if(i == 5){
			//消息单独过期
			rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....",messagePostProcessor);
		} else{
			//不过期的消息
			rabbitTemplate.convertAndSend("test_exchange_ttl", "ttl.hehe", "message ttl....");
		}
	}
} 
```

## 6.6 死信队列
死信队列，英文缩写：DLX 。Dead Letter Exchange（死信交换机），当消息成为Dead message（过期后，但队列绑定了一个死信交换机）后，可以被重新发送到另一个交换机，这个交换机就是DLX。==死信交换机和死信队列和普通的没有区别==。**ttl + 死信队列 可以做延时业务**，如自动取消超时订单功能。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110205716.png)
消息成为死信的三种情况，==当消息成为死信后，如果该队列绑定了死信交换机，则消息会被死信交换机重新路由到死信队列==：
1. **队列消息长度到达限制**；
2. **消费者拒接消费消息**，basicNack/basicReject ，**并且不把消息重新放入原目标队列**，requeue=false；
3. **原队列存在消息过期设置，消息到达超时时间未被消费**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110300592.png)
	
队列绑定死信交换机：给队列设置参数： x-dead-letter-exchange 和 x-dead-letter-routing-key
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110226153.png)
配置：
```xml
    <!--
        死信队列：
            1. 声明正常的队列(test_queue_dlx)和交换机(test_exchange_dlx)
            2. 声明死信队列(queue_dlx)和死信交换机(exchange_dlx)
            3. 正常队列绑定死信交换机
                设置两个参数：
			* x-dead-letter-exchange：死信交换机名称
			* x-dead-letter-routing-key：发送给死信交换机的routingkey
    -->
    <!--
        1. 声明正常的队列(test_queue_dlx)和交换机(test_exchange_dlx)
    -->
    <rabbit:queue name="test_queue_dlx" id="test_queue_dlx">
        <!--3. 正常队列绑定死信交换机-->
        <rabbit:queue-arguments>
            <!--3.1 x-dead-letter-exchange：死信交换机名称-->
            <entry key="x-dead-letter-exchange" value="exchange_dlx" />

            <!--3.2 x-dead-letter-routing-key：发送给死信交换机的routingkey-->
            <entry key="x-dead-letter-routing-key" value="dlx.hehe" />

            <!--4.1 设置队列的过期时间 ttl-->
            <entry key="x-message-ttl" value="10000" value-type="java.lang.Integer" />
            <!--4.2 设置队列的长度限制 max-length -->
            <entry key="x-max-length" value="10" value-type="java.lang.Integer" />
        </rabbit:queue-arguments>
    </rabbit:queue>
    <rabbit:topic-exchange name="test_exchange_dlx">
        <rabbit:bindings>
            <rabbit:binding pattern="test.dlx.#" queue="test_queue_dlx"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--
       2. 声明死信队列(queue_dlx)和死信交换机(exchange_dlx)
   -->
    <rabbit:queue name="queue_dlx" id="queue_dlx"></rabbit:queue>
    <rabbit:topic-exchange name="exchange_dlx">
        <rabbit:bindings>
            <rabbit:binding pattern="dlx.#" queue="queue_dlx"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>
```
生产者测试方法：
```java
/**
 * 发送测试死信消息：
 *  1. 过期时间
 *  2. 长度限制
 *  3. 消息拒收
 */
@Test
public void testDlx(){
	//1. 测试过期时间，死信消息
	//rabbitTemplate.convertAndSend("test_exchange_dlx","test.dlx.haha","我是一条消息，我会死吗？");
	//2. 测试长度限制后，消息死信
   /* for (int i = 0; i < 20; i++) {
		rabbitTemplate.convertAndSend("test_exchange_dlx","test.dlx.haha","我是一条消息，我会死吗？");
	}*/
	//3. 测试消息拒收
	rabbitTemplate.convertAndSend("test_exchange_dlx","test.dlx.haha","我是一条消息，我会死吗？");
}
```
消费者：
```java
package com.itheima.listener;

import com.rabbitmq.client.Channel;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.listener.api.ChannelAwareMessageListener;
import org.springframework.stereotype.Component;

@Component
public class DlxListener implements ChannelAwareMessageListener {
    @Override
    public void onMessage(Message message, Channel channel) throws Exception {
        long deliveryTag = message.getMessageProperties().getDeliveryTag();

        try {
            //1.接收转换消息
            System.out.println(new String(message.getBody()));
            //2. 处理业务逻辑
            System.out.println("处理业务逻辑...");
            int i = 3/0;//出现错误
            //3. 手动签收
            channel.basicAck(deliveryTag,true);
        } catch (Exception e) {
            //e.printStackTrace();
            System.out.println("出现异常，拒绝接受");
            //4.拒绝签收，不重回队列 requeue=false
            channel.basicNack(deliveryTag,true,false);
        }
    }
}
```

## 6.7 延迟队列（重点！！！）
延迟队列，即消息进入队列后不会立即被消费，只有到达指定时间后、被延迟一定时间，才会被消费。需求：
1. **下单后，30分钟未支付，取消订单，回滚库存**。
2. 新用户注册成功7天后，发送短信问候。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110301426.png)

实现方式：
1. 定时器
2. 延迟队列
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110239779.png)

很可惜，在RabbitMQ中并未提供延迟队列功能。但是可以使用：TTL+死信队列 组合实现延迟队列的效果。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110240137.png)
> 主动取消或支付了肯定会有逻辑处理啊，总不可能按下按钮后没有后端业务逻辑处理吧？那你这个按钮就真的只是单纯的一个按钮。一分钟取消了肯定会有对应的操作库存操作，==MQ场景是针对预定后30分钟未取消，若支付则取消订单==。支付了的话，判断订单状态会发现已经完成了

配置：注意，**这里是设置的整个队列的过期时间，但是真正情况是当前用户当前订单的过期时间**，总不能一个订单使用一个队列吧？
> 如果你需要把不同过期时间的消息都放在同一个队列里面，那估计得需要用到优先队列，保证先过期的信息排在队列头部，这样就不会出现早就该过期的消息得不到消费。
> 老师的这个模型，支付了也会进死信队列啊，其实就是一个超时检查是否支付的模型吧，已支付啥也不干，没支付的把库存加回来，因为下了订单库存就已经减了
> 这儿的意思是不管订单是否支付，半个小时候都进入死信队列，监听死信队列的再去校验是否需要回退库存
```xml
    <!--
        延迟队列：
            1. 定义正常交换机（order_exchange）和队列(order_queue)
            2. 定义死信交换机（order_exchange_dlx）和队列(order_queue_dlx)
            3. 绑定，设置正常队列过期时间为30分钟
    -->
    <!-- 1. 定义正常交换机（order_exchange）和队列(order_queue)-->
    <rabbit:queue id="order_queue" name="order_queue">
        <!-- 3. 绑定，设置正常队列过期时间为30分钟-->
        <rabbit:queue-arguments>
            <entry key="x-dead-letter-exchange" value="order_exchange_dlx" />
            <entry key="x-dead-letter-routing-key" value="dlx.order.cancel" />
            <entry key="x-message-ttl" value="10000" value-type="java.lang.Integer" />
        </rabbit:queue-arguments>
    </rabbit:queue>
    <rabbit:topic-exchange name="order_exchange">
        <rabbit:bindings>
            <rabbit:binding pattern="order.#" queue="order_queue"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>

    <!--  2. 定义死信交换机（order_exchange_dlx）和队列(order_queue_dlx)-->
    <rabbit:queue id="order_queue_dlx" name="order_queue_dlx"></rabbit:queue>
    <rabbit:topic-exchange name="order_exchange_dlx">
        <rabbit:bindings>
            <rabbit:binding pattern="dlx.order.#" queue="order_queue_dlx"></rabbit:binding>
        </rabbit:bindings>
    </rabbit:topic-exchange>
</beans>
```
生产者：
```java
@Test
public  void testDelay() throws InterruptedException {
	//1.发送订单消息。 将来是在订单系统中，下单成功后，发送消息
	rabbitTemplate.convertAndSend("order_exchange","order.msg","订单信息：id=1,time=2019年8月17日16:41:47");
	/*//2.打印倒计时10秒
	for (int i = 10; i > 0 ; i--) {
		System.out.println(i+"...");
		Thread.sleep(1000);
	}*/
}
```
消费者：
```java
package com.itheima.listener;

import com.rabbitmq.client.Channel;
import org.springframework.amqp.core.Message;
import org.springframework.amqp.rabbit.listener.api.ChannelAwareMessageListener;
import org.springframework.stereotype.Component;
@Component
public class OrderListener implements ChannelAwareMessageListener {
    @Override
    public void onMessage(Message message, Channel channel) throws Exception {
        long deliveryTag = message.getMessageProperties().getDeliveryTag();
        try {
            //1.接收转换消息
            System.out.println(new String(message.getBody()));

            //2. 处理业务逻辑
            System.out.println("处理业务逻辑...");
            System.out.println("根据订单id查询其状态...");
            System.out.println("判断状态是否为支付成功");
            System.out.println("取消订单，回滚库存....");
            //3. 手动签收
            channel.basicAck(deliveryTag,true);
        } catch (Exception e) {
            //e.printStackTrace();
            System.out.println("出现异常，拒绝接受");
            //4.拒绝签收，不重回队列 requeue=false
            channel.basicNack(deliveryTag,true,false);
        }
    }
}
```
## 6.8 日志与监控
### 6.8.1 RabbitMQ日志
RabbitMQ默认日志存放路径： /var/log/rabbitmq/rabbit@xxx.log
日志包含了RabbitMQ的版本号、Erlang的版本号、RabbitMQ服务节点名称、cookie的hash值、RabbitMQ配置文件地址、内存限制、磁盘限制、默认账户guest的创建以及权限配置等等。

### 6.7.2 Web管控台监控
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110259560.png)
### 6.7.3 rabbitmqctl管理和监控
```bash
# 查看队列
rabbitmqctl list_queues
# 查看exchanges
rabbitmqctl list_exchanges
# 查看用户
rabbitmqctl list_users
# 查看连接
rabbitmqctl list_connections
# 查看消费者信息
rabbitmqctl list_consumers
# 查看环境变量
rabbitmqctl environment
# 查看未被确认的队列
rabbitmqctl list_queues name messages_unacknowledged
# 查看单个队列的内存使用
rabbitmqctl list_queues name memory
# 查看准备就绪的队列
rabbitmqctl list_queues name messages_ready
```

## 6.9 消息追踪
在使用任何消息中间件的过程中，难免会出现某条消息异常丢失的情况。对于RabbitMQ而言，可能是因为**生产者或消费者与RabbitMQ断开了连接**，而它们与RabbitMQ又采用了不同的确认机制；也有可能是因为**交换器与队列之间不同的转发策**略；甚至是交换器并没有与任何队列进行绑定，生产者又不感知或者没有采取相应的措施；另外RabbitMQ本身的集群策略也可能导致消息的丢失。这个时候就需要有一个较好的机制、跟踪记录消息的投递过程，以此协助开发和运维人员进行问题的定位。

在RabbitMQ中可以使用Firehose和rabbitmq_tracing插件功能来实现消息追踪。
### 6.9.1 消息追踪-Firehose
firehose的机制是==将生产者投递给rabbitmq的消息，rabbitmq投递给消费者的消息按照指定的格式发送到默认的exchange上==。这个默认的exchange的名称为amq.rabbitmq.trace，它是一个topic类型的exchange。

发送到这个exchange上的消息的 routing key为 publish.exchangename 和 deliver.queuename。其中exchangename和queuename为实际exchange和queue的名称，分别对应生产者投递到exchange的消息，和消费者从queue上获取的消息。

注意：打开 trace 会影响消息写入功能，适当打开后请关闭。
- `rabbitmqctl trace_on`：开启Firehose命令
- `rabbitmqctl trace_off`：关闭Firehose命令

### 6.9.2 消息追踪-rabbitmq_tracing
rabbitmq_tracing和Firehose在实现上如出一辙，只不过rabbitmq_tracing的方式比Firehose多了一层GUI的包装，更容易使用和管理。
启用插件：`rabbitmq-plugins enable rabbitmq_tracing` 。
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110306820.png)

Payload limit表示消息取多少个字节。Pattern为 `#` 表示接收所有消息，无论是发过来的还是消费的。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110307596.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110310863.png)

---
# 7. RabbitMQ应用问题
## 7.1 消息可靠性保障
需求：100%确保消息发送成功
消息补偿机制

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110242798.png)
最关键的是第三步。其实，这个模型用的不多，发送消息失败、消费消息失败了直接存进失败数据库中，后面用另一个服务检测、重发就行
## 7.2 消息幂等性保障
幂等性指**一次和多次请求某一个资源，对于资源本身应该具有同样的结果**。也就是说，其任意多次执行对资源本身所产生的影响均与一次执行的影响相同。在MQ中指，**消费多条相同的消息，得到与消费该消息一次相同的结果**。

乐观锁解决方案：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110243750.png)
还可给消息设置全局唯一id。

---
# 8. RabbitMQ集群搭建
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303110304082.png)



