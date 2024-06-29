# 学习介绍
为什么要学习微服务框架知识？
- 找工作：面试必问微服务，企业开发也都使用微服务
- 激发程序员：架构师和操作人员受性能驱动，使用最好的工具来解决他们遇到的技术和业务问题
- 降低成本，快速迭代：互联网发展迅速，业务更新迭代快，微服务符合敏捷开发需求

需要学习那些微服务知识？
![900](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041720190.png)
![900](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041721453.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041725709.png)


---
# 1. 认识微服务
随着互联网行业的发展，对服务的要求也越来越高，**服务架构也从单体架构逐渐演变为现在流行的微服务架构**。这些架构之间有怎样的差别呢？
## 1.1 学习目标
了解微服务架构的优缺点
### 1.1.1 单体架构
**单体架构**：将业务的所有功能集中在一个项目中开发，打成一个包部署。
单体架构的优缺点如下：
**优点：**
- 架构简单
- 部署成本低

**缺点：**
- **耦合度高**（维护困难、升级困难）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041729332.png)

### 1.1.2 分布式架构
**分布式架构**：根据业务功能对系统做拆分，每个业务功能模块作为独立项目开发，称为一个服务。分布式架构的优缺点：
**优点：**
- 降低服务耦合
- 有利于服务升级和拓展

**缺点：**
- 服务调用关系错综复杂
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041733145.png)


分布式架构虽然降低了服务耦合，但是服务拆分时也有很多问题需要思考：
- **服务拆分的粒度**如何界定？
- 服务之间**如何调用**？
- 服务的调用关系如何管理？
- 服务健康状态如何感知？

人们需要制定一套行之有效的标准来约束分布式架构。
## 1.2 微服务
微服务的架构特征：
- **单一职责**：微服务拆分粒度更小，**每一个服务都对应唯一的业务能力**，做到单一职责
- **自治**：团队独立、技术独立、数据独立（下面是独立的数据库），**独立部署和交付**
- **面向服务**：服务对外提供统一标准的业务与接口，与语言和技术无关
- **隔离性强**：服务调用做好**隔离、容错**、降级，避免出现级联问题
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041739393.png)

==微服务的上述特性其实是在给分布式架构制定一个标准，进一步降低服务之间的耦合度，提供服务的独立性和灵活性。做到高内聚，低耦合==。

因此，可以认为**微服务**是一种经过良好架构设计的**分布式架构方案** 。但方案该怎么落地？选用什么样的技术栈？全球的互联网公司都在积极尝试自己的微服务落地方案。其中在Java领域最引人注目的就是SpringCloud和阿里巴巴的Dubbo。

单体架构特点？
- 简单方便，高度耦合，扩展性差，适合小型项目。例如：学生管理系统

分布式架构特点？
- 松耦合，扩展性好，但架构复杂，难度大。适合大型互联网项目，例如：京东、淘宝

微服务：一种良好的分布式架构方案
- 优点：拆分粒度更小、服务更独立、耦合度更低
- 缺点：架构非常复杂，运维、监控、部署难度提高

## 1.3 微服务技术对比
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041744952.png)
企业需求不同：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303041753753.png)

## 1.4.SpringCloud
SpringCloud是目前国内使用最广泛的微服务框架。官网地址：https://spring.io/projects/spring-cloud。**SpringCloud集成了各种微服务功能组件，并基于SpringBoot实现了这些组件的自动装配**，从而提供了良好的开箱即用体验。

其中常见的组件包括：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042147114.png)
另外，SpringCloud底层是依赖于SpringBoot的，并且有版本的兼容关系，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042148015.png)
我们学习的版本是Hoxton.SR10，因此对应的SpringBoot版本是2.3.x版本。

---
# 2.服务拆分和远程调用
任何分布式架构都离不开服务的拆分，微服务也是一样。
## 2.1 服务拆分
### 2.1.1 服务拆分原则
这里我总结了微服务拆分时的几个原则：
- **不同微服务，不要重复开发相同业务**
- **微服务数据独立**，不要访问其它微服务的数据库
- **微服务可以将自己的业务暴露为接口**，供其它微服务调用
	 ![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042150554.png)
### 2.1.2 服务拆分示例
以资料中的微服务cloud-demo为例，其结构如下：
- cloud-demo：父工程，管理依赖
- order-service：订单微服务，负责订单相关业务
- user-service：用户微服务，负责用户相关业务
	![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042211421.png)

要求：
- 订单微服务和用户微服务**都必须有各自的数据库**，相互独立
- **订单服务和用户服务都对外暴露Restful的接口**
- 订单服务如果需要查询用户信息，只能调用用户服务的Restful接口，不能查询用户数据库 
 
### 2.1.3 导入SQL语句
首先，将资料提供的 `cloud-order.sql`和`cloud-user.sql` 导入到MySQL中：
 
cloud-user表中初始数据如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042217162.png)
cloud-order表中初始数据如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042217763.png)
cloud-order表中持有cloud-user表中的id字段。
### 2.1.4 导入demo工程
用IDEA导入课前资料提供的Demo：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042222092.png)
项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042222127.png)

导入后，会在IDEA右下角出现弹窗：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042223412.png)
点击弹窗，然后按下图选择：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042224174.png)
会出现这样的菜单：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042224656.png)
配置下项目使用的JDK：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042225044.png)

## 2.2 实现远程调用案例
在order-service服务中，有一个根据id查询订单的接口：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042226330.png)
根据id查询订单，返回值是Order对象，如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042227405.png)
其中的user为null

在user-service中有一个根据id查询用户的接口：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042228187.png)
查询的结果如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042229126.png)

### 2.2.1 案例需求
修改order-service中的根据id查询订单业务，**要求在查询订单的同时，根据订单中包含的userId查询出用户信息**，一起返回（不能直接查另一个服务的数据库，要远程调用）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042230655.png)
因此，我们需要在order-service中 向user-service发起一个http的请求，调用http://localhost:8081/user/{userId}这个接口。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042233528.png)
微服务远程调用大概的步骤是这样的：
- 注册一个 `RestTemplate` 的实例到Spring容器
- 修改order-service服务中的OrderService类中的queryOrderById方法，根据Order对象中的userId查询User
- 将查询的User填充到Order对象，一起返回

`bean` 的注册只能写在配置类中，然后可以注入到其他地方。

微服务调用方式
- 基于RestTemplate发起的http请求实现远程调用
- http请求做远程调用是与语言无关的调用，**只要知道对方的ip、端口、接口路径、请求参数即可**。

### 2.2.2 注册RestTemplate
首先，我们在 `order-service` 服务中的 `OrderApplication` 启动类中，注册 `RestTemplate` 实例：
```java
package cn.itcast.order;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.Bean;
import org.springframework.web.client.RestTemplate;

@MapperScan("cn.itcast.order.mapper")
@SpringBootApplication // 启动类也是配置类
public class OrderApplication {
    public static void main(String[] args) {
        SpringApplication.run(OrderApplication.class, args);
    }
    @Bean
    public RestTemplate restTemplate() {
        return new RestTemplate();
    }
}
```
### 2.2.3 实现远程调用
修改order-service服务中的cn.itcast.order.service包下的OrderService类中的queryOrderById方法：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042236622.png)
服务器和服务器之间http请求没有跨域，客户端发送axios请求会有跨域。跨域是浏览器对js的限制，后端不存在跨域问题。
## 2.3 提供者与消费者
在服务调用关系中，会有两个不同的角色：
- **服务提供者**：一次业务中，被其它微服务调用的服务。（提供接口给其它微服务）
- **服务消费者**：一次业务中，调用其它微服务的服务。（调用其它微服务提供的接口）
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042247816.png)

但是，服务提供者与服务消费者的角色并不是绝对的，而是相对于业务而言。如果服务A调用了服务B，而服务B又调用了服务C，服务B的角色是什么？
- 对于A调用B的业务而言：A是服务消费者，B是服务提供者
- 对于B调用C的业务而言：B是服务消费者，C是服务提供者

因此，服务B既可以是服务提供者，也可以是服务消费者。

---
# 3. Eureka注册中心
假如我们的服务提供者user-service部署了多个实例，如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042248556.png)
大家思考几个问题：
- order-service在发起远程调用的时候，该如何得知user-service实例的ip地址和端口？
- **有多个user-service实例地址，order-service调用时该如何选择**？
- order-service如何得知某个user-service实例是否依然健康，是不是已经宕机？

## 3.1 Eureka的结构和作用
**这些问题都需要利用SpringCloud中的注册中心来解决**，其中最广为人知的注册中心就是Eureka，其结构如下。在Eureka架构中，微服务角色有两类：
- EurekaServer：服务端，注册中心
	- 记录服务信息
	- 心跳监控
- EurekaClient：客户端
	- Provider：服务提供者，例如案例中的user-service
		- 注册自己的信息到EurekaServer
		- 每隔30秒向EurekaServer发送心跳
	- consumer：服务消费者，例如案例中的 order-service
		- 根据服务名称从EurekaServer拉取服务列表
		- **基于服务列表做负载均衡**，选中一个微服务后发起远程调用
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042248954.png)

回答之前的各个问题。

问题1：order-service如何得知user-service实例地址？
答：获取地址信息的流程如下：
- 每个服务包括user-service服务实例启动后，将自己的信息注册到eureka-server（Eureka服务端）。这个叫**服务注册**
- eureka-server保存**服务名称到服务实例地址列表**的映射关系
- order-service根据服务名称，拉取实例地址列表。这个叫**服务发现**或服务拉取

问题2：order-service如何从多个user-service实例中选择具体的实例？
- order-service从实例列表中利用负载均衡算法选中一个实例地址
- 向该实例地址发起远程调用

问题3：order-service如何得知某个user-service实例是否依然健康，是不是已经宕机？
- user-service会每隔一段时间（默认30秒）向eureka-server发起请求，报告自己状态，称为**心跳**
- 当超过一定时间没有发送心跳时，eureka-server会认为**微服务实例故障**，将该实例从服务列表中剔除
- order-service拉取服务时，就能将故障实例排除了

> 注意：==一个微服务，既可以是服务提供者，又可以是服务消费者，因此eureka将服务注册、服务发现等功能统一封装到了eureka-client端==

eureka的作用
- 消费者该如何获取服务提供者具体信息？
	- 服务提供者启动时向eureka注册自己的信息
	- eureka保存这些信息
	- 消费者根据服务名称向eureka拉取提供者信息
- 如果有多个服务提供者，消费者该如何选择？
	- 服务消费者利用负载均衡算法，从服务列表中挑选一个
- 消费者如何感知服务提供者健康状态？
	- 服务提供者会每隔30秒向EurekaServer发送心跳请求，报告健康状态
	- eureka会更新记录服务列表信息，心跳不正常会被剔除
	- 消费者就可以拉取到最新的信息

因此，接下来动手实践的步骤包括：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042251233.png)
## 3.2 搭建eureka-server
首先大家注册中心服务端：eureka-server，这必须是一个**独立的微服务**。
### 3.2.1 创建eureka-server服务
在cloud-demo父工程下，创建一个子模块：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042255489.png)
填写模块信息： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042255464.png)
然后填写服务信息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042256273.png)
### 3.2.2 引入eureka依赖
引入SpringCloud为eureka提供的starter依赖：
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-server</artifactId>
</dependency>
```
### 3.2.3 编写启动类
给eureka-server服务编写一个启动类，一定要添加一个@EnableEurekaServer注解，开启eureka的注册中心功能：
```java
package cn.itcast.eureka;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.netflix.eureka.server.EnableEurekaServer;

@SpringBootApplication
@EnableEurekaServer
public class EurekaApplication {
    public static void main(String[] args) {
        SpringApplication.run(EurekaApplication.class, args);
    }
}
```
> 那个报错的兄弟，我一开始也是，你到评论区找一下那个demo那个能行；maven setting文件镜像使用阿里最新的地址；如果这里报错，需要在settings-build-build tools-maven-ignored files 把忽视的勾勾去掉，这样父依赖就可以匹配到这个工程生效。真坑啊 parent版本号必须是2.3.9才行，最新版一直报错

### 3.2.4 编写配置文件
编写一个application.yml文件，内容如下：
```yaml
server:
  port: 10086 # 服务端口
# 做服务注册,写下面的配置
spring:
  application:
    name: eureka-server # eureka的服务名词
eureka: 
  client:
    service-url: # eureka的地址信息
      defaultZone: http://127.0.0.1:10086/eureka
```
为什么这里eureka启动时也要把自己注册到eureka中？为了后面eureka集群，开多个eureka，然后交流信息，这里的 `defaultZone` 后面应写为eureka集群的地址，虽然现在是单机。
### 3.2.5 启动服务
启动微服务，然后在浏览器访问：http://127.0.0.1:10086。看到下面结果应该是成功了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042307023.png)
服务和服务实例，就像类与类对象。

## 3.3 服务注册
下面，我们将user-service注册到eureka-server中去。
### 1）引入依赖
在user-service的pom文件中，引入下面的eureka-client依赖：
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```
### 2）配置文件
在user-service中，修改application.yml文件，添加**服务名称、eureka地址**：
```yaml
spring:
  application:
    name: userservice
eureka:
  client:
    service-url:
      defaultZone: http://127.0.0.1:10086/eureka
```
### 3）启动多个user-service实例
为了演示一个服务有多个实例的场景，我们添加一个SpringBoot的启动配置，再启动一个user-service。

首先，复制原来的user-service启动配置：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042321712.png)
然后，在弹出的窗口中，填写信息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042321262.png)
现在，SpringBoot窗口会出现两个user-service启动配置：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303042321714.png)
不过，第一个是8081端口，第二个是8082端口。

启动两个user-service实例：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303050016032.png)

查看eureka-server管理页面：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303050016420.png)

## 3.4 服务发现
下面，我们将order-service的逻辑修改：**向eureka-server拉取user-service的信息，实现服务发现**。
### 1）引入依赖
之前说过，**服务发现、服务注册统一都封装在eureka-client依赖**，因此这一步与服务注册时一致。

在order-service的pom文件中，引入下面的eureka-client依赖：
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-netflix-eureka-client</artifactId>
</dependency>
```
### 2）配置文件
**服务发现也需要知道eureka地址**，因此第二步与服务注册一致，都是配置eureka信息。在order-service中，修改application.yml文件，添加服务名称、eureka地址：
```yaml
spring:
  application:
    name: orderservice
eureka:
  client:
    service-url:
      defaultZone: http://127.0.0.1:10086/eureka
```
### 3）服务拉取和负载均衡
最后，我们要**去eureka-server中拉取user-service服务的实例列表，并且实现负载均衡**。不过这些动作不用我们去做，只需要添加一些注解即可。

在order-service的OrderApplication中，给RestTemplate这个Bean添加一个@LoadBalanced注解：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303050927074.png)
修改order-service服务中的cn.itcast.order.service包下的OrderService类中的queryOrderById方法。**修改访问的url路径，用服务名代替ip、端口**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303050927498.png)
spring会自动帮助我们从eureka-server端，根据userservice这个服务名称，获取实例列表，而后完成负载均衡。

---
# 4. Ribbon负载均衡
上一节中，我们添加了@LoadBalanced注解，即可实现负载均衡功能，这是什么原理呢？
## 4.1 负载均衡原理
SpringCloud底层其实是利用了一个名为Ribbon的组件，来实现负载均衡功能的。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303050949183.png)

那么我们发出的请求明明是http://userservice/user/1，怎么变成了http://localhost:8080的呢？
## 4.2 源码跟踪
为什么我们只输入了service名称就可以访问了呢？之前还要获取ip和端口。显然**有人帮我们根据service名称，获取到了服务实例的ip和端口**。它就是`LoadBalancerInterceptor`，这个类会在对RestTemplate的请求进行拦截，然后从Eureka根据服务id获取服务列表，随后利用负载均衡算法得到真实的服务地址信息，替换服务id。

我们进行源码跟踪。
### 1）LoadBalancerIntercepor
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051031052.png)
可以看到这里的intercept方法，拦截了用户的HttpRequest请求，然后做了几件事：
- `request.getURI()`：获取请求uri，本例中就是 http://user-service/user/8
- `originalUri.getHost()`：获取uri路径的主机名，其实就是服务id，`user-service`
- `this.loadBalancer.execute()`：处理服务id，和用户请求。

这里的`this.loadBalancer`是`LoadBalancerClient`类型，我们继续跟入。
### 2）LoadBalancerClient
继续跟入execute方法：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051035387.png)
代码是这样的：
- getLoadBalancer(serviceId)：根据服务id获取ILoadBalancer，而ILoadBalancer会拿着服务id去eureka中获取服务列表并保存起来。
- getServer(loadBalancer)：利用内置的负载均衡算法，从服务列表中选择一个。本例中，可以看到获取了8082端口的服务

放行后，再次访问并跟踪，发现获取的是8081：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051036547.png)
果然实现了负载均衡。
### 3）负载均衡策略IRule
在刚才的代码中，可以看到获取服务使通过一个`getServer`方法来做负载均衡:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051036547.png)
我们继续跟入：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051038093.png)
继续跟踪源码chooseServer方法，发现这么一段代码：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051038900.png)
我们看看这个rule是谁：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051039941.png)
这里的rule默认值是一个`RoundRobinRule`，看类的介绍：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051040771.png)
这不就是轮询的意思嘛。到这里，整个负载均衡的流程我们就清楚了。
### 4）总结
SpringCloudRibbon的底层采用了一个拦截器，拦截了RestTemplate发出的请求，对地址做了修改。用一幅图来总结一下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051041791.png)
基本流程如下：
- 拦截我们的RestTemplate请求http://userservice/user/1
- RibbonLoadBalancerClient会从请求url中获取服务名称，也就是user-service
- DynamicServerListLoadBalancer根据user-service到eureka拉取服务列表
- eureka返回列表，localhost:8081、localhost:8082
- IRule利用内置负载均衡规则，从列表中选择一个，例如localhost:8081
- RibbonLoadBalancerClient修改请求地址，用localhost:8081替代userservice，得到http://localhost:8081/user/1，发起真实请求

## 4.3 负载均衡策略
### 4.3.1 负载均衡策略
负载均衡的规则都定义在IRule接口中，而IRule有很多不同的实现类：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051043460.png)
不同规则的含义如下：

| **内置负载均衡规则类**    | **规则描述**                                                 |
| ------------------------- | ------------------------------------------------------------ |
| RoundRobinRule            | 简单轮询服务列表来选择服务器。它是Ribbon默认的负载均衡规则。 |
| AvailabilityFilteringRule | 对以下两种服务器进行忽略：   （1）在默认情况下，这台服务器如果3次连接失败，这台服务器就会被设置为“短路”状态。
| | 短路状态将持续30秒，如果再次连接失败，短路的持续时间就会几何级地增加。  
| |（2）并发数过高的服务器。如果一个服务器的并发连接数过高，配置了AvailabilityFilteringRule规则的客户端也会将其忽略。
| | 并发连接数的上限，可以由客户端的 `<clientName>.<clientConfigNameSpace>.ActiveConnectionsLimit` 属性进行配置。 |
| WeightedResponseTimeRule  | 为每一个服务器赋予一个权重值。服务器响应时间越长，这个服务器的权重就越小。这个规则会随机选择服务器，这个权重值会影响服务器的选择。 |
| **ZoneAvoidanceRule**     | 以区域可用的服务器为基础进行服务器的选择。使用Zone对服务器进行分类，这个Zone可以理解为一个机房、一个机架等。而后再对Zone内的多个服务做轮询。 |
| BestAvailableRule         | 忽略那些短路的服务器，并选择并发数较低的服务器。             |
| RandomRule                | 随机选择一个可用的服务器。                                   |
| RetryRule                 | 重试机制的选择逻辑                                           |

默认的实现就是ZoneAvoidanceRule，是一种轮询方案，根据Zone选择服务列表然后轮询。
### 4.3.2 自定义负载均衡策略
**通过定义IRule实现可以修改负载均衡规则**，有两种方式：
1. 代码方式：在order-service中的OrderApplication类中，定义一个新的IRule：
	```java
	@Bean
	public IRule randomRule(){
	    return new RandomRule();
	}
	```
2. 配置文件方式：在order-service的application.yml文件中，添加新的配置也可以修改规则：
	```yaml
	userservice: # 给某个微服务配置负载均衡规则，这里是userservice服务
	  ribbon:
	    NFLoadBalancerRuleClassName: com.netflix.loadbalancer.RandomRule # 负载均衡规则 
	```

> **注意**，一般用默认的负载均衡规则，不做修改。

代码方式：配置灵活，但修改时需要重新发布。
配置方式：直观方便，无需重新打包发布，但无法做全局配置
## 4.4 饥饿加载
Ribbon默认是采用懒加载，即第一次访问时才会去创建LoadBalanceClient，请求时间会很长。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051115328.png)
而**饥饿加载则会在项目启动时创建，降低第一次访问的耗时**，通过下面配置开启饥饿加载：
```yaml
ribbon:
  eager-load:
    enabled: true #开启饥饿加载
    clients: userservice # 指定饥饿加载的服务名称
    # clients: # 多个时
    #   - userservice
    #   - xxxxservice
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051116424.png)

---
# 5. Nacos注册中心
国内公司一般都推崇阿里巴巴的技术，比如注册中心，SpringCloudAlibaba也推出了一个名为Nacos的注册中心。
## 5.1 认识和安装Nacos
[Nacos](https://nacos.io/)是阿里巴巴的产品，现在是[SpringCloud](https://spring.io/projects/spring-cloud)中的一个组件。相比[Eureka](https://github.com/Netflix/eureka)功能更加丰富，在国内受欢迎程度较高。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051118617.png)
安装方式可以参考[[Nacos/Nacos安装指南]]。先是单机启动。

## 5.2.服务注册到nacos
Nacos是SpringCloudAlibaba的组件，而SpringCloudAlibaba也遵循SpringCloud中定义的服务注册、服务发现规范。因此使用Nacos和使用Eureka对于微服务来说，并没有太大区别。主要差异在于：
- 依赖不同
- 服务地址不同

### 1）引入依赖
在cloud-demo父工程的pom文件中的 `<dependencyManagement>` 中引入SpringCloudAlibaba的依赖：
```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-alibaba-dependencies</artifactId>
    <version>2.2.6.RELEASE</version>
    <type>pom</type>
    <scope>import</scope>
</dependency>
```
然后**在user-service和order-service中的pom文件中引入nacos-discovery依赖**：
```xml
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
> **注意**：不要忘了注释掉eureka的依赖。
### 2）配置nacos地址
在user-service和order-service的application.yml中添加nacos地址：
```yaml
spring:
  cloud:
    nacos:
      server-addr: localhost:8848 # nacos服务地址
```
> **注意**：不要忘了注释掉eureka的地址。

### 3）重启
重启微服务后，登录nacos管理页面，可以看到微服务信息：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051227168.png)
## 5.3 服务分级存储模型
一个**服务**可以有多个**实例**，例如我们的user-service，可以有：
- 127.0.0.1:8081
- 127.0.0.1:8082
- 127.0.0.1:8083

假如这些实例分布于全国各地的不同机房，例如：
- 127.0.0.1:8081，在上海机房
- 127.0.0.1:8082，在上海机房
- 127.0.0.1:8083，在杭州机房

Nacos就将 **同一机房内的实例** 划分为一个**集群**。也就是说，user-service是服务，一个服务可以包含多个集群（一个服务放在多个集群，容灾），如杭州、上海，每个集群下可以有多个实例，形成分级模型，如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051232918.png)
==微服务互相访问时，应该尽可能访问同集群实例，因为本地访问速度更快。当本集群内不可用时，才访问其它集群==。例如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051233733.png)
杭州机房内的order-service应该优先访问同机房的user-service。
### 5.3.1 给user-service配置集群
修改user-service的application.yml文件，**添加集群配置**（同时写两个配置的快捷键）：
```yaml
spring:
  cloud:
    nacos:
      server-addr: localhost:8848
      discovery:
        cluster-name: HZ # 集群名称
```
重启两个user-service实例后，我们可以在nacos控制台看到下面结果：
> 修改北京集群后点小锤子重新编译一下再启动UserService3，才能到北京集群
> 修改配置文件启动之后会跑到同一个地址的检查下pom文件里是不是有spring-boot-devtools这个依赖
> 配了两个集群后面在一个集群的是缓存问题mavenclearn一下再启动

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051235463.png)
我们再次复制一个user-service启动配置，添加属性：
```sh
-Dserver.port=8083 -Dspring.cloud.nacos.discovery.cluster-name=SH
```
配置如图所示：![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051235795.png)

启动UserApplication3后再次查看nacos控制台：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051239704.png)

### 5.3.2.同集群优先的负载均衡
默认的`ZoneAvoidanceRule`并不能实现**根据同集群优先来实现负载均衡**。因此Nacos中提供了一个`NacosRule`的实现，可以优先从同集群中挑选实例。

1）给order-service配置集群信息。修改order-service的application.yml文件，添加集群配置：
```sh
spring:
  cloud:
    nacos:
      server-addr: localhost:8848 # nacos服务端地址
      discovery:
        cluster-name: HZ # 集群名称配置，也就是机房位置
```
2）修改负载均衡规则。修改order-service的application.yml文件，修改负载均衡规则：
```yaml
userservice: # 要做配置的微服务名称
  ribbon:
    NFLoadBalancerRuleClassName: com.alibaba.cloud.nacos.ribbon.NacosRule # 负载均衡规则 
```
> 使用配置类也行，Ban作用域不一样，配置类是全局覆盖了yml.
8083还有记录的，记得把OrderApplication里面@Bean那块的自定义负载均衡代码注释
新版nacos LoadBalancer默认提供了两种负载均衡策略 RandomLoadBalancer  随机分配策略 (默认) RoundRobinLoadBalancer 轮询分配策略
不生效就在启动类创建个NacosRule的Bean并返回NacosRule就行了

NacosRule负载均衡策略
1. 优先选择同集群服务实例列表
2. 本地集群找不到提供者，才去其它集群寻找，并且会报警告
3. 确定了可用实例列表后，再采用随机负载均衡挑选实例

## 5.4 权重配置
实际部署中会出现这样的场景：服务器设备性能有差异，部分实例所在机器性能较好，另一些较差，我们希望性能好的机器承担更多的用户请求。但默认情况下NacosRule是优先选择本地集群、同集群内随机挑选，不会考虑机器的性能问题。

因此，Nacos提供了权重配置来控制访问频率，权重越大则访问频率越高。

在nacos控制台，找到user-service的实例列表，点击编辑，即可修改权重：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051301978.png)
在弹出的编辑窗口，修改权重：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051301359.png)
实例的权重控制
1. Nacos控制台可以设置实例的权重值，0~1之间
2. 同集群内的多个实例，权重越高被访问的频率越高
3. 如果权重修改为0，则该实例永远不会被访问（灰度发布）
## 5.5 环境隔离
Nacos中服务存储和数据存储的最外层都是一个名为namespace的东西，用来做最外层隔离
。Nacos提供了namespace来实现环境隔离功能。
- nacos中可以有多个namespace
- namespace下可以有group、service等
- 不同namespace之间相互隔离，例如**不同namespace的服务互相不可见**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051302691.png)
### 5.5.1 创建namespace
**默认情况下，所有service、data、group都在同一个namespace，名为public**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051303742.png)
我们可以点击页面新增按钮，添加一个namespace：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051303229.png)
然后，填写表单：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051306069.png)
就能在页面看到一个新的namespace：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051305430.png)

Nacos环境隔离
- 每个namespace都有唯一id
- 服务设置namespace时要写id而不是名称
- 不同namespace下的服务互相不可见
### 5.5.2 给微服务配置namespace
**给微服务配置namespace只能通过修改配置来实现**。例如，修改order-service的application.yml文件：
```yaml
spring:
  cloud:
    nacos:
      server-addr: localhost:8848
      discovery:
        cluster-name: HZ
        namespace: 492a7d5d-237b-46a1-a99a-fa8e98e4b0f9 # 命名空间，填ID
```
重启order-service后，访问控制台，可以看到下面的结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051307959.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051307830.png)
此时访问order-service，因为namespace不同，会导致找不到userservice，控制台会报错：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051308653.png)
## 5.6 Nacos与Eureka的区别
nacos注册中心细节分析：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051311647.png)
Nacos的服务实例分为两种l类型：
- 临时实例：如果实例宕机超过一定时间，会从服务列表剔除，默认的类型。
- 非临时实例：如果实例宕机，不会从服务列表剔除，也可以叫永久实例。nacos注册中心会等待实例恢复。

配置一个服务实例为永久实例：
```yaml
spring:
  cloud:
    nacos:
      discovery:
        ephemeral: false # 设置为非临时实例
```
Nacos和Eureka整体结构类似，服务注册、服务拉取、心跳等待，但是也存在一些差异：
- Nacos与eureka的共同点
  - **都支持服务注册和服务拉取**
  - **都支持服务提供者心跳方式做健康检测**
- Nacos与Eureka的区别
  - Nacos支持**服务端主动检测提供者状态**（服务器压力大一些）：临时实例采用心跳模式，非临时实例采用主动检测模式。临时实例心跳不正常会被剔除，非临时实例则不会被剔除
  - Nacos支持**服务列表变更的消息推送模式**，服务列表更新更及时
  - Nacos集群默认采用AP方式，**当集群中存在非临时实例时，采用CP模式**；Eureka采用AP方式
	这就是分布式理论有CAP理论和BASE理论；C：Consistance：一致性；A：Availability：可用性；P：Partion Tolerance分区容错性

---
# 6.Nacos配置管理
Nacos除了可以做注册中心，同样可以做配置管理来使用。
## 6.1.统一配置管理
当微服务部署的实例越来越多，达到数十、数百时，逐个修改微服务配置就会让人抓狂，而且很容易出错。**我们需要一种统一配置管理方案，可以集中管理所有实例的配置**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051323064.png)
Nacos一方面可以将配置集中管理，另一方可以在配置变更时，及时通知微服务，实现配置的热更新。
### 6.1.1 在nacos中添加配置文件
如何在nacos中管理配置呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051513147.png)
然后在弹出的表单中，填写配置信息（Data ID是微服务名称-环境名称-后缀名yaml）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051514480.png)

> 注意：==项目的核心配置，需要热更新的配置才有放到nacos管理的必要==。基本不会变更的一些配置还是保存在微服务本地比较好。
### 6.1.2 从微服务拉取配置
微服务要拉取nacos中管理的配置，并且与本地的application.yml配置合并，才能完成项目启动。

但如果尚未读取application.yml，又如何得知nacos地址呢？因此spring引入了一种新的配置文件：bootstrap.yaml文件，会在application.yml之前被读取，流程如下：![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051516769.png)

1）引入nacos-config依赖
首先，在user-service服务中，引入nacos-config的客户端依赖：
```xml
<!--nacos配置管理依赖-->
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alibaba-nacos-config</artifactId>
</dependency>
```
2）添加bootstrap.yaml
然后，在user-service中添加一个bootstrap.yaml文件，内容如下：
> 这里需要config下加个命名空间的
> 报错的注意老师的命名空间变成了public，我们还是上个视频的dev，在public里面加一个配置就好了
> 给bootstrap的config设置一个namespace   就可以了
> 图标搜spring插件spring boot assistant
> @GetMapping下面加个@ResponseBody，不然返回值类型报错，1.4.1
```yaml
spring:
  application:
    name: userservice # 服务名称
  profiles:
    active: dev #开发环境，这里是dev 
  cloud:
    nacos:
      server-addr: localhost:8848 # Nacos地址
      config:
        file-extension: yaml # 文件后缀名
```
这里会根据 `spring.cloud.nacos.server-addr` 获取nacos地址，再根据 `${spring.application.name}-${spring.profiles.active}.${spring.cloud.nacos.config.file-extension}` 作为文件id，来读取配置。本例中，就是去读取 `userservice-dev.yaml`：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051529827.png)

> 配置名、代码都是对的，但还是报错的兄弟听我一句：一定要保证boot、cloud、alibaba三者版本匹配！

3）读取nacos配置
在user-service中的UserController中添加业务逻辑，读取pattern.dateformat配置：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051531043.png)

完整代码：
```java
package cn.itcast.user.web;

import cn.itcast.user.pojo.User;
import cn.itcast.user.service.UserService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

@Slf4j
@RestController
@RequestMapping("/user")
public class UserController {

    @Autowired
    private UserService userService;

    @Value("${pattern.dateformat}")
    private String dateformat;
    
    @GetMapping("now")
    public String now(){
        return LocalDateTime.now().format(DateTimeFormatter.ofPattern(dateformat));
    }
    // ...略
}
```
在页面访问，可以看到效果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051532455.png)

## 6.2 配置热更新
我们最终的目的，是修改nacos中的配置后，微服务中无需重启即可让配置生效，也就是**配置热更新**。要实现配置热更新，可以使用两种方式：
### 6.2.1 方式一
在@Value注入的变量所在类上添加注解@RefreshScope：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051533806.png)

> 在你的pom文件上加上bootstarp的启动依赖即可，因为你的cloud版本过高不会自动识别bootstarp文件
> 中文配置报错的，启动时改成utf8编码就好了
> 我来说一下 我 解决NullPointer这个异常的过程 我刚开始因为@Value不行后来使用的@NacosValue 删除dev命名空间 这就是学习cloud容易遇到的问题 中文文档少难以解决
> 应该是主动通知，点发布后服务有日志提示
> 如果有出现加上@RefreshScope注解后获取的值为空的情况的，可以把注解更改成@RefreshScope(proxyMode = ScopedProxyMode.DEFAULT)
> 原因可能是@RefreshScope使用默认代理规则CGLIB，与该类的代理规则重复了，导致二次代理属性丢失

### 6.2.2 方式二
使用@ConfigurationProperties注解代替@Value注解。在user-service服务中，添加一个类，读取patterrn.dateformat属性：
> @ConfigurationProperties是一个属性配置类，会在括号中声明prefix前缀，然后通过属性和prefix结合就可以在yml文件中配置属性的内容
```java
package cn.itcast.user.config;

import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@Component
@Data
@ConfigurationProperties(prefix = "pattern")
public class PatternProperties {
    private String dateformat;
}
```
在UserController中使用这个类代替 `@Value` （没有加 `refreshScope`）：
意思就是说，只需要@ConfigurationProperties这个注解就可以实现热更新，无需配合@RefreshScope，而@Value必须配合@RefreshScope
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051539691.png)
完整代码：
```java
package cn.itcast.user.web;

import cn.itcast.user.config.PatternProperties;
import cn.itcast.user.pojo.User;
import cn.itcast.user.service.UserService;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

@Slf4j
@RestController
@RequestMapping("/user")
public class UserController {
    @Autowired
    private UserService userService;

    @Autowired
    private PatternProperties patternProperties;

    @GetMapping("now")
    public String now(){
        return LocalDateTime.now().format(DateTimeFormatter.ofPattern(patternProperties.getDateformat()));
    }
    // 略
}
```
## 6.3 配置共享
**其实微服务启动时，会去nacos读取多个配置文件**，例如：
- `[spring.application.name]-[spring.profiles.active].yaml`，例如：userservice-dev.yaml
- `[spring.application.name].yaml`，例如：userservice.yaml

**而 `[spring.application.name].yaml` 不包含环境，因此可以被多个环境共享**。下面我们通过案例来测试配置共享。
### 1）添加一个环境共享配置
我们在nacos中添加一个userservice.yaml文件：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051543963.png)
### 2）在user-service中读取共享配置
在user-service服务中，修改PatternProperties类，读取新添加的属性：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051545594.png)
在user-service服务中，修改UserController，添加一个方法（SpringMVC转为JSON返回）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051545716.png)

### 3）运行两个UserApplication，使用不同的profile
修改UserApplication2这个启动项，改变其profile值（-Dspring.profiles.active=test）：
> 新版idea没有这个需要 在这图中下面那地方添加参数指定自己写。。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051548448.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051548672.png)
这样，UserApplication(8081)使用的profile是dev，UserApplication2(8082)使用的profile是test。

启动UserApplication和UserApplication2。访问http://localhost:8081/user/prop，结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051551462.png)

访问http://localhost:8082/user/prop，结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051552500.png)

可以看出来，不管是dev，还是test环境，都读取到了envSharedValue这个属性的值。
### 4）配置共享的优先级
当nacos、服务本地同时出现相同属性时，优先级有高低之分：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051552734.png)
## 6.4 搭建Nacos集群
Nacos生产环境下一定要部署为集群状态，部署方式参考文档：[[Nacos/Nacos安装指南]]。

集群搭建步骤：
- 搭建MySQL集群并初始化数据库表
- 下载解压nacos
- 修改集群配置（节点信息）、数据库配置
- 分别启动多个nacos节点
- nginx反向代理
