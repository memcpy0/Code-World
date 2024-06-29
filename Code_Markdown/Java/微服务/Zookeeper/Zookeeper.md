# 1. 初识Zookeeper
## 1.1 Zookeeper概念
Zookeeper 是 Apache Hadoop 项目下的一个子项目，是一个**树形目录服务**。Zookeeper 翻译过来就是 动物园管理员，他是用来管 Hadoop（大象）、Hive(蜜蜂)、Pig(小 猪)的管理员。简称zk。

Zookeeper 是一个分布式的、开源的**分布式应用程序的协调服务**。Zookeeper 提供的主要功能包括：
- 配置管理
- **分布式锁**
- 集群管理
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092047457.png)
## 1.2 ZooKeeper安装
**1. 环境准备**
ZooKeeper服务器是用Java创建的，它运行在JVM之上。需要安装JDK 7或更高版本。
**2. 上传**
将下载的ZooKeeper放到/opt/ZooKeeper目录下
```shell

#上传zookeeper alt+p
put f:/setup/apache-zookeeper-3.5.6-bin.tar.gz
#打开opt目录
cd /opt
#创建zooKeeper目录
mkdir  zooKeeper
#将zookeeper安装包移动到 /opt/zooKeeper
mv apache-zookeeper-3.5.6-bin.tar.gz /opt/zookeeper/
```
**3. 解压**
将tar包解压到/opt/zookeeper目录下
```shell
tar -zxvf apache-ZooKeeper-3.5.6-bin.tar.gz 
```
**4. 配置zoo.cfg**
进入到conf目录拷贝一个zoo_sample.cfg并完成配置
```shell
#进入到conf目录
cd /opt/zooKeeper/apache-zooKeeper-3.5.6-bin/conf/
#拷贝
cp  zoo_sample.cfg  zoo.cfg
```
修改zoo.cfg：
```shell
#打开目录
cd /opt/zooKeeper/
#创建zooKeeper存储目录
mkdir  zkdata
#修改zoo.cfg
vim /opt/zooKeeper/apache-zooKeeper-3.5.6-bin/conf/zoo.cfg
```
> 大家注意一下目录名的大小写，老师给的文档有错误

修改存储目录：dataDir=/opt/zookeeper/zkdata
**5. 启动ZooKeeper**
```shell
cd /opt/zooKeeper/apache-zooKeeper-3.5.6-bin/bin/
#启动
 ./zkServer.sh  start
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081859244.png)
看到上图表示ZooKeeper成功启动
**6. 查看ZooKeeper状态**
```shell
./zkServer.sh status
```
zookeeper启动成功。standalone代表zk没有搭建集群，现在是单节点
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081900106.png)
zookeeper没有启动：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081900639.png)

下面是ZooKeeper客户端：
```bash
./zkcli.sh 
```

---
# 2. ZooKeeper命令操作
## 2.1 Zookeeper 数据模型
ZooKeeper 是一个**树形目录服务**，其数据模型和Unix的文件系统目录树很类似，拥有一个**层次化结构**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092153440.png)

这里面的每一个节点都被称为： ZNode，**每个节点上都会保存自己的数据和节点信息**。 节点可以拥有子节点，同时也允许少量（1MB）数据存储在该节点之下。

节点可以分为四大类：
- `PERSISTENT` 持久化节点。
- `EPHEMERAL` 临时节点 ：-e 。Zookeeper客户端一关闭，临时创建的结点就消失。
- `PERSISTENT_SEQUENTIAL` 持久化顺序节点 ：-s 。会在结点后面添加顺序编号。
- `EPHEMERAL_SEQUENTIAL` 临时顺序节点  ：-es 。

## 2.2 Zookeeper服务端常用命令
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092154154.png)
启动 ZooKeeper 服务: `./zkServer.sh start`
查看 ZooKeeper 服务状态: `./zkServer.sh status`
停止 ZooKeeper 服务: `./zkServer.sh stop `
重启 ZooKeeper 服务: `./zkServer.sh restart `
## 2.3 Zookeeper客户端常用命令
`value` 是节点的数据，不写时为null。
![700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091958290.png)
```bash
# 创建临时节点
create -e /节点path value
# 创建顺序节点
create -s /节点path value
# 查询节点详细信息
ls –s /节点path (曾经是ls2 path)
```
无论是 `-s` 还是 `-es` ，所有这种节点创建后，节点后被添加的顺序编号都在同一个顺序中。

节点属性：
- czxid：节点被创建的事务ID 
- ctime: 创建时间 
- mzxid: 最后一次被更新的事务ID 
- mtime: 修改时间 
- pzxid：子节点列表最后一次被更新的事务ID
- cversion：子节点的版本号 
- dataversion：数据版本号 
- aclversion：权限版本号 
- ephemeralOwner：用于临时节点，代表临时节点的事务ID，如果为持久节点则为0 
- dataLength：节点存储的数据的长度 
- numChildren：当前节点的子节点个数 

---
# 3. ZooKeeper JavaAPI操作
## 3.1 Curator介绍
Curator是Apache ZooKeeper的Java客户端库。常见的ZooKeeper Java API ：
- 原生Java API
- ZkClient
- Curator

Curator项目的目标是简化 ZooKeeper 客户端的使用。Curator 最初是 Netfix 研发的，后来捐献了 Apache 基金会，目前是 Apache 的顶级项目。官网：http://curator.apache.org/

Curator API的版本要和ZooKeeper版本对应起来。Curator高版本可向下兼容低版本ZooKeeper。
## 3.2 Curator API常用操作
导入以下依赖：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.itheima</groupId>
    <artifactId>curator-zk</artifactId>
    <version>1.0-SNAPSHOT</version>
 
    <dependencies>

        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.10</version>
            <scope>test</scope>
        </dependency>

        <!--curator-->
        <dependency>
            <groupId>org.apache.curator</groupId>
            <artifactId>curator-framework</artifactId>
            <version>4.0.0</version>
        </dependency>

        <dependency>
            <groupId>org.apache.curator</groupId>
            <artifactId>curator-recipes</artifactId>
            <version>4.0.0</version>
        </dependency>
        <!--日志-->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>1.7.21</version>
        </dependency>

        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-log4j12</artifactId>
            <version>1.7.21</version>
        </dependency> 
    </dependencies>
 
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.1</version>
                <configuration>
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
        </plugins>
    </build> 

</project>
```
### 3.2.1 建立连接
`sessionTimeout` 是客户端与ZooKeeper服务端建立会话的超时时间。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092311851.png)
`CuratorFrameworkFactory.builder().namespace("itheima")` 名称空间，将来做的所有操作，都默认使用「名称空间指定的字符串」为根目录和根节点；当这个根节点下面没有子节点后，根节点也会被自动删除：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092314849.png)

### 3.2.2 添加节点
节点有持久/临时/顺序之分，还可设置数据。下面的 `client` 是用 `@Before` 修饰的 `testConnect()` 方法中创建的 `CuratorFrameworkFactory` 对象。
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092318127.png)
基本创建做法如下：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092321427.png)
带有数据：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092323496.png)
设置节点类型（临时节点在当前会话结束后，立即消失；这里这个方法执行完则会话结束）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092324653.png)
会话结束后，`app3` 也消失：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092326013.png)
创建多级节点：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100001358.png)

### 3.2.3 删除节点
删除单个节点；删除带有子节点的节点
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100037181.png)
必须成功的删除：防止网络抖动，本质就是重试。
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100038330.png)
回调函数：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100039120.png)

### 3.2.4 修改节点
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100017465.png)
可能有多个客户端操作ZooKeeper服务端的数据，给节点中的数据加上版本，然后根据版本修改。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100013083.png)

### 3.2.5 查询节点
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100005331.png)
要存储状态信息，命令要用 `ls -s` ，API则如下：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100008987.png)

### 3.2.6 Watch事件监听
ZooKeeper允许**用户在指定节点上注册一些Watcher**，并且在一些特定事件触发的时候，ZooKeeper服务端会将事件通知到感兴趣的客户端上去，==该机制是 ZooKeeper实现分布式协调服务的重要特性==。

==ZooKeeper中引入了Watcher机制来实现了发布/订阅功能，能让多个订阅者同时监听某一个对象，当一个对象自身状态变化时，会通知所有订阅者==。
ZooKeeper原生支持通过注册Watcher来进行事件监听，但是其使用并不是特别方便，需要开发人员自己反复注册Watcher，比较繁琐。

Curator引入了Cache来实现对ZooKeeper服务端事件的监听。ZooKeeper提供了三种Watcher：
- `NodeCache` ：只是监听某一个特定的节点
- `PathChildrenCache` ：监控一个ZNode的子节点，但感知不到自身的变动
- `TreeCache` ：可以监控整个树上的所有节点，类似于PathChildrenCache和NodeCache的组合
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092241891.png)

在之前代码的基础上，介绍 `NodeCache` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100050144.png)
对节点数据进行操作，会打印出“节点变化了”的消息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100051373.png)
取出修改后的数据：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100053111.png)

然后是 `PathChildrenCache` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100057661.png)
如下修改，先创建 `/app2`：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100057553.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100059777.png)
过滤一下要处理的事件类型：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100102563.png)

最后是 `TreeCache` ： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100105125.png)


## 3.3 分布式锁实现
在我们进行单机应用开发，涉及并发同步的时候，==我们往往采用synchronized或者Lock的方式来解决多线程间的代码同步问题，这时多线程的运行都是在同一个JVM之下，没有任何问题==。

但当我们的应用是分布式集群工作的情况下，属于多JVM下的工作环境，==跨JVM之间已经无法通过多线程的锁解决同步问题==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092242590.png)
那么就需要一种更加高级的锁机制，来处理**跨机器的进程之间的数据同步问题**——这就是分布式锁。实现方式有下面几种：如Redis（基于缓存）、乐观锁/悲观锁（数据库层面）。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092244270.png)
### 3.3.1 ZooKeeper分布式锁原理
核心思想：==当客户端要获取锁，则创建节点，使用完锁，则删除该节点==。
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092248008.png)
1. 客户端获取锁时，在lock节点下创建**临时** **顺序**节点。——
	为什么是临时？假设client1,client2,client3都期望拿到锁，处于竞争关系，并都与ZooKeeper Server连接起来了。假设client1创建了一个/lock/1持久节点，并获取到锁，可能会发生问题：client1处理过程中宕机了，就不能把这个节点删除掉，锁也无法释放，其他client无法获取到锁。
	为什么是顺序？见后面。顺序获取前驱被删除的通知。
2. 然后获取lock下面的所有子节点，==客户端获取到所有的子节点之后，如果发现自己创建的子节点序号最小==，那么就认为该客户端获取到了锁。**使用完锁后，将该节点删除**。
3. 如果发现自己创建的节点并非lock所有子节点中最小的，说明自己还没有获取到锁，此时==客户端需要找到**比自己小的那个节点**，同时对其注册事件监听器，监听删除事件==。
4. 如果发现比自己小的那个节点被删除，则客户端的Watcher会收到相应通知，此时==再次判断自己创建的节点是否是lock子节点中序号最小的，如果是则获取到了锁==，如果不是则重复以上步骤，继续获取到比自己小的一个节点并注册监听。

Curator实现分布式锁API。在Curator中有五种锁方案：
- `InterProcessSemaphoreMutex`：分布式排它锁（非可重入锁）
- `InterProcessMutex`：分布式**可重入**排它锁（客户端获取到共享资源，进去一次可再进去？重入几次就要释放几次资源）
- `InterProcessReadWriteLock`：分布式读写锁
- `InterProcessMultiLock`：将多个锁作为单个实体管理的容器
- `InterProcessSemaphoreV2`：共享信号量

### 3.3.2 模拟12306售票案例
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092249856.png)
以多个线程模拟其他客户端对12306的访问，有几张票卖多了：
![550](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100134538.png)
由于12306部署在很多台机器上，synchronized和Lock是没有用的。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100136044.png)

现在的 `Ticket12306.java` 如下所示：
```java
package com.itheima.curator;

import org.apache.curator.RetryPolicy;
import org.apache.curator.framework.CuratorFramework;
import org.apache.curator.framework.CuratorFrameworkFactory;
import org.apache.curator.framework.recipes.locks.InterProcessMutex;
import org.apache.curator.retry.ExponentialBackoffRetry;

import java.util.concurrent.TimeUnit;

public class Ticket12306 implements Runnable { 
    private int tickets = 10;//数据库的票数 
    private InterProcessMutex lock ;
 
    public Ticket12306(){
        //重试策略
        RetryPolicy retryPolicy = new ExponentialBackoffRetry(3000, 10);
        //2.第二种方式
        //CuratorFrameworkFactory.builder();
        CuratorFramework client = CuratorFrameworkFactory.builder()
                .connectString("192.168.149.135:2181")
                .sessionTimeoutMs(60 * 1000)
                .connectionTimeoutMs(15 * 1000)
                .retryPolicy(retryPolicy)
                .build();

        //开启连接
        client.start(); 
        lock = new InterProcessMutex(client,"/lock");
    }

    @Override
    public void run() { 
        while(true){
            //获取锁
            try {
                lock.acquire(3, TimeUnit.SECONDS);
                if(tickets > 0){                    
	                System.out.println(Thread.currentThread()+":"+tickets);
                    Thread.sleep(100);
                    tickets--;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }finally {
                //释放锁
                try {
                    lock.release();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```
报的异常暂时不管。下面有两个临时顺序节点：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303100149855.png)

---
# 4. ZooKeeper集群搭建
## 4.1 集群介绍
Leader选举：
- Serverid：服务器ID
  比如有三台服务器，编号分别是1,2,3。
  **编号越大在选择算法中的权重越大**。
- Zxid：数据ID
  服务器中存放的最大数据ID。**值越大说明数据越新**，在选举算法中数据越新权重越大。
- 在Leader选举的过程中，如果某台ZooKeeper获得了超过半数（总体半数）的选票，则此ZooKeeper就可以成为Leader了。

下面5个节点中谁做Leader？
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092249405.png)


---
# 5. ZooKeeper核心理论
Zookeeper 集群角色
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303092250029.png)

在ZooKeeper集群服中务中有三个角色：
Leader 领导者 ：          
1. **处理事务请求**
2. 集群内部各服务器的调度者

Follower 跟随者 ：
1. 处理客户端非事务请求，**转发事务请求给Leader服务器**
2. **参与Leader选举投票**

Observer 观察者：
1. 处理客户端非事务请求，**转发事务请求给Leader服务器**
