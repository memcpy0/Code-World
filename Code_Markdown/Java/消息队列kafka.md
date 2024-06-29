### 1. Kafka和阻塞队列
先使用阻塞队列发送系统级通知，再使用Kafka优化。这些消息队列的框架深度封装，不太合适试手。不如先用阻塞队列试一试。

BlockingQueue是一个接口，用于解决线程通信（类似进程的消息队列，但是线程中共享的BlockingQueue在同个进程空间中）的问题；Object中的notify、wait等方法也可以解决（但更加原始和麻烦）。进程通信则可以用匿名/命名管道、消息队列、共享内存、信号量、信号、Socket进行。****

阻塞方法：put和take。生产者消费者模式：
- 生产者：产生数据的线程；
- 消费者：使用数据的线程。

实现类包括：
- ArrayBlockingQueue
- LinkedBlockingQueue
- PriorityBlockingQueue、SynchronousQueue、DelayQueue等

```java
package com.nowcoder.community;  
  
import java.util.Random;  
import java.util.concurrent.ArrayBlockingQueue;  
import java.util.concurrent.BlockingQueue;  
  
public class BlockingQueueTest {  
    public static void main(String[] args) {  
        // 生产者和消费者共用一个阻塞队列  
        BlockingQueue queue = new ArrayBlockingQueue(10);  
        new Thread(new Producer(queue)).start();  
        new Thread(new Consumer(queue)).start();  
        new Thread(new Consumer(queue)).start();  
        new Thread(new Consumer(queue)).start();  
    }  
}  
  
class Consumer implements Runnable {  
    private BlockingQueue<Integer> blockingQueue;  
    public Consumer(BlockingQueue<Integer> bq) {  
        blockingQueue = bq;  
    }  
    @Override  
    public void run() {  
        try {  
            while (true) {  
                Thread.sleep(new Random().nextInt(1000)); // 消费的时间间隔不一定相同  
                int v = blockingQueue.take();  
                System.out.println(Thread.currentThread().getName() + "消费: " + v + ", 现在的队列大小为" + blockingQueue.size());  
            }  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
}  
class Producer implements Runnable {  
    private BlockingQueue<Integer> blockingQueue;  
    public Producer(BlockingQueue<Integer> bq) {  
        blockingQueue = bq;  
    }  
    @Override  
    public void run() {  
        try {  
            for (int i = 0; i < 100; ++i) { // 生产100个数据  
                Thread.sleep(20); // 模拟间隔  
                blockingQueue.put(i);  
                System.out.println(Thread.currentThread().getName() + "生产: " + blockingQueue.size());  
            }  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }  
}
```
### 2. kafka测试
kafka是一个**分布式的流媒体平台**、基于发布订阅模式（不只是消息队列），可以应用到消息系统、日志收集、用户行为追踪、流式处理等。卡夫卡具有高吞吐量、消息持久化（写到硬盘上）、高可靠性（分布式部署）、高扩展性等特点。

kafka：
- Broker：kafka集群中的一台服务器
- ZooKeeper：用ZooKeeper管理kafka的集群。kafka内置了ZooKeeper
- Topic、Partition、Offset：
- Lead Replica、Follower Replica：为了保证高可靠，要对数据存多份

对硬盘顺序读写性能很高，甚至高于内存的读写 > 硬盘随机读写。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304161653311.png)
从kafka.apache.org/download下载。在config目录下配置zookeeper.properties：
```properties
dataDir=D:/work/data/zookeeper
# the port at which the clients will connect
clientPort=2181
# disable the per-ip limit on the number of connections since this is a non-production config
maxClientCnxns=0
# Disable the adminserver by default to avoid port conflicts.
# Set the port to something non-conflicting if choosing to enable this
admin.enableServer=false
# admin.serverPort=8080
```
在server.properties下配置：
```properties
log.dirs=D:/work/data/kafka-logs
```
在kafka.apache.org/documentation/#quickstart中有命令介绍。启动ZooKeeper：
```bash
D:\java\kafka_2.13-3.4.0> bin\windows\zookeeper-server-start.bat config\zookeeper.properties
```
再开一个命令行窗口，启动kafka：
```bash
D:\java\kafka_2.13-3.4.0> bin\windows\kafka-server-start.bat config\server.properties
```
再启动一个命令行，使用kafka。kafka作为消息队列，使用发布订阅模式，要把消息发送到某个主题下，要先创建一个主题（代表一个位置，也代表消息的分类）：--create创建主题，--bootstrap-server ：在哪个服务器上创建主题，kafka默认端口是9092；副本：--replication-factor 创建1个副本；分区：1个；test主题：
```bash
PS D:\java\kafka_2.13-3.4.0\bin\windows> .\kafka-topics.bat --create --bootstrap-server localhost:9092 --replication-factor 1 --partitions 1 --topic test
Created topic test.

PS D:\java\kafka_2.13-3.4.0\bin\windows> .\kafka-topics.bat --list --bootstrap-server localhost:9092
test
```
发送消息时，以生产者的身份发送：--broker-list 服务器列表 --topic 哪些主题
```bash
PS D:\java\kafka_2.13-3.4.0\bin\windows> .\kafka-console-producer.bat --broker-list localhost:9092 --topic test
>hello
>kafka
>world
```
消费者：在哪个服务器上读哪个主题的数据，从头开始读
```bash
PS D:\java\kafka_2.13-3.4.0\bin\windows> .\kafka-console-consumer.bat --bootstrap-server localhost:9092 --topic test --from-beginning
hello
kafka
world
```
### 3. Spring整合kafka
引入依赖 spring-kafka
```xml
<dependency>  
   <groupId>org.springframework.kafka</groupId>  
   <artifactId>spring-kafka</artifactId>  
</dependency>
```
配置kafka：配置server、consumer：
```yml
spring:
	kafka:  
		bootstrap-servers: localhost:9092 # 服务器  
		consumer:  
		    group-id: community-consumer-group # 消费者分组ID  
		    enable-auto-commit: true # 消费者读消息是根据偏移量,自动提交消费者Offset  
		    auto-commit-interval: 3000 # 多久提交一次
```
访问kafka：
- 生产者：kafkaTemplate.send(topic, data);
- 消费者：@KafkaListener(topics = {"test"})
	public void handleMessage(ConsumerRecord record) {}

建立一个测试类：
```java
package com.nowcoder.community;  
  
import org.apache.kafka.clients.consumer.ConsumerRecord;  
import org.junit.jupiter.api.Test;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.boot.test.context.SpringBootTest;  
import org.springframework.kafka.annotation.KafkaListener;  
import org.springframework.kafka.core.KafkaTemplate;  
import org.springframework.stereotype.Component;  
import org.springframework.test.context.ContextConfiguration;  
  
@SpringBootTest  
@ContextConfiguration(classes = CommunityApplication.class)  
public class KafkaTests {  
    @Autowired  
    private KafkaProducer kafkaProducer;  
    @Test  
    public void testKafka() {  
        kafkaProducer.sendMessage("test", "hello");  
        kafkaProducer.sendMessage("test", "kafka");  
        kafkaProducer.sendMessage("test", "haha");  
        try {  
            Thread.sleep(10000);  
        } catch (InterruptedException e) {  
            e.printStackTrace();  
        }  
    }  
}  
  
@Component  
class KafkaProducer {  
    @Autowired  
    private KafkaTemplate kafkaTemplate;  
  
    public void sendMessage(String topic, String content) { // 生产者是主动发消息,什么时候发消息就什么时候调用它  
        kafkaTemplate.send(topic, content);  
    }  
}  
  
@Component  
class KafkaConsumer {  
    @KafkaListener(topics = {"test"}) // Spring启动后自动监听test主题下的消息,没有消息就阻塞  
    public void handleMessage(ConsumerRecord record) {  
        System.out.println(record.topic() + " " + record.value());  
    }  
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304162150612.png)

