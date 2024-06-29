# 1. Feign远程调用
先来看我们以前利用RestTemplate发起远程调用的代码：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051625290.png)
存在下面的问题：
- 代码可读性差，编程体验不统一
- 参数复杂URL难以维护

Feign是一个声明式（回想声明式事务）的http客户端，官方地址：https://github.com/OpenFeign/feign。其作用就是帮助我们优雅的实现http请求的发送，解决上面提到的问题。![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051626014.png)

## 1.1 Feign替代RestTemplate
Fegin的使用步骤如下：
### 1）引入依赖
我们在order-service服务的pom文件中引入feign的依赖：
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```
> 前面的，你没有用springboot开发，没有parent肯定要写版本号啊。这是因为老师用的boot版本管理在parent里面有
### 2）添加注解
在order-service的启动类添加注解开启Feign的功能：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051629167.png)
### 3）编写Feign的客户端
在order-service中新建一个接口，内容如下：
```java
package cn.itcast.order.client;

import cn.itcast.order.pojo.User;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;

@FeignClient("userservice")
public interface UserClient {
    @GetMapping("/user/{id}")
    User findById(@PathVariable("id") Long id);
}
```
这个客户端主要是**基于SpringMVC的注解**来声明远程调用的信息，比如：
- 服务名称：`userservice`
- 请求方式：`GET`
- 请求路径：`/user/{id}`
- 请求参数：`Long id`
- 返回值类型：`User`

这样，Feign就可以帮助我们发送http请求，无需自己使用RestTemplate来发送了。
### 4）测试
修改order-service中的OrderService类中的queryOrderById方法，使用Feign客户端代替RestTemplate：
> @Autowired SpringBoot官方不建议在声明变量时使用，建议用@Resource注入
> 为什么没有写注入bean的注解能直接注入——因为@FeignClient注解的类会装在到容器
> 实际放在容器中的是动态代理类
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051632926.png)

是不是看起来优雅多了。
> 基于fegin返回的是json格式  这里注意 不然报错
> 这里启动 报OrderApplication错误 去yml中把临时实例（ephemeral）关掉就可以了
> 无法启动 删除pom中的spring-cloud-starter-alibaba-nacos-config试试
> 目前这个项目nginx负责nacos集群的负载，而Ribbon则负责服务的负载，懂了不

### 5）总结
使用Feign的步骤：
① 引入依赖
② 添加@EnableFeignClients注解
③ 编写FeignClient接口
④ **使用FeignClient中定义的方法代替RestTemplate**

## 1.2 自定义配置
Feign可以支持很多的自定义配置，如下表所示：

| 类型                   | 作用             | 说明                                                   |
| ---------------------- | ---------------- | ------------------------------------------------------ |
| **feign.Logger.Level** | 修改日志级别     | 包含四种不同的级别：NONE、BASIC、HEADERS、FULL         |
| feign.codec.Decoder    | 响应结果的解析器 | http远程调用的结果做解析，例如解析json字符串为java对象 |
| feign.codec.Encoder    | 请求参数编码     | 将请求参数编码，便于通过http请求发送                   |
| feign. Contract        | 支持的注解格式   | 默认是SpringMVC的注解                                  |
| feign. Retryer         | 失败重试机制     | 请求失败的重试机制，默认是没有，不过会使用Ribbon的重试 |

一般情况下，默认值就能满足我们使用，如果要自定义时，只需要创建自定义的 `@Bean` 覆盖默认 `Bean` 即可。下面以日志为例来演示如何自定义配置。
### 1.2.1 配置文件方式
基于配置文件 `application.yml` 修改feign的日志级别，**可以针对单个服务**：
```yaml
feign:  
  client:
    config: 
      userservice: # 针对某个微服务的配置
        loggerLevel: FULL #  日志级别 
```
**也可以针对所有服务**：
```yaml
feign:  
  client:
    config: 
      default: # 这里用default就是全局配置，如果是写服务名称，则是针对某个微服务的配置
        loggerLevel: FULL #  日志级别 
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051642842.png)
而日志的级别分为四种：
- NONE：不记录任何日志信息，这是默认值。
- BASIC：仅记录**请求的方法，URL以及响应状态码和执行时间**
- HEADERS：在BASIC的基础上，额外记录了**请求和响应的头信息**
- FULL：记录**所有请求和响应的明细**，包括头信息、请求体、元数据。

### 1.2.2 Java代码方式
也可以基于Java代码来修改日志级别，先声明一个类，然后声明一个 `Logger.Level` 的对象：
```java
public class DefaultFeignConfiguration  {
    @Bean
    public Logger.Level feignLogLevel(){
        return Logger.Level.BASIC; // 日志级别为BASIC
    }
}
```
如果要**全局生效**，将其放到启动类的 `@EnableFeignClients` 这个注解中：
```java
@EnableFeignClients(defaultConfiguration = DefaultFeignConfiguration .class) 
```
如果是**局部生效**，则把它放到对应的 `@FeignClient` 这个注解中（Ctrl+P快捷键）：
```java
@FeignClient(value = "userservice", configuration = DefaultFeignConfiguration .class) 
```
## 2.3 Feign使用优化
Feign底层发起HTTP请求，依赖于其它的框架。其底层客户端实现包括：
•URLConnection：默认实现，不支持连接池
•Apache HttpClient ：支持连接池
•OKHttp：支持连接池

因此提高Feign的性能主要手段，==就是使用**连接池**代替默认的URLConnection==。这里我们用Apache的HttpClient来演示。

1）引入依赖
在order-service的pom文件中引入Apache的HttpClient依赖：
```xml
<!--httpClient的依赖 -->
<dependency>
    <groupId>io.github.openfeign</groupId>
    <artifactId>feign-httpclient</artifactId>
</dependency>
```
2）配置连接池
在order-service的application.yml中添加配置（JMeter测试，看多少连接合适）：
```yaml
feign:
  client:
    config:
      default: # default全局的配置
        loggerLevel: BASIC # 日志级别，BASIC就是基本的请求和响应信息
  httpclient:
    enabled: true # 开启feign对HttpClient的支持
    max-connections: 200 # 最大的连接数
    max-connections-per-route: 50 # 每个路径的最大连接数
```
接下来，在FeignClientFactoryBean中的loadBalance方法中打断点：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051649226.png)
Debug方式启动order-service服务，可以看到这里的client，底层就是Apache HttpClient：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051651844.png)
总结，Feign的优化：
1.日志级别尽量用basic
2.使用HttpClient或OKHttp代替URLConnection
①  引入feign-httpClient依赖
②  配置文件开启httpClient功能，设置连接池参数

## 1.4 最佳实践
所谓最近实践，就是使用过程中总结的经验，最好的一种使用方式。自习观察可以发现，Feign的客户端与服务提供者的controller代码非常相似：

feign客户端：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051653597.png)
UserController：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051653143.png)

有没有一种办法简化这种重复的代码编写呢？
### 1.4.1 继承方式
一样的代码可以通过继承来共享：
1）定义一个API接口，利用定义方法，并**基于SpringMVC注解做声明**。
2）Feign客户端和Controller都集成改接口
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051655890.png)
优点：
- 简单
- 实现了代码共享

缺点：
- 服务提供方、服务消费方紧耦合
- **参数列表中的注解映射 `@PathVariable` 并不会继承**，因此Controller中必须再次声明方法、参数列表、注解
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051657206.png)

### 1.4.2 抽取方式
==将Feign的Client抽取为独立模块，并且把接口有关的POJO、默认的Feign配置都放到这个模块中，提供给所有消费者使用==。
> 这不是设计原则嘛：多用组合，少用继承

例如，将UserClient、User、Feign的默认配置都抽取到一个 `feign-api` 包中，所有微服务引用该依赖包，即可直接使用。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051659236.png)

### 1.4.3 实现基于抽取的最佳实践
实现最佳实践方式二的步骤如下：
1. 首先创建一个module，命名为feign-api，然后引入feign的starter依赖
2. 将order-service中编写的UserClient、User、DefaultFeignConfiguration都复制到feign-api项目中
3. 在order-service中引入feign-api的依赖
4. 修改order-service中的所有与上述三个组件有关的import部分，改成导入feign-api中的包
5. 重启测试
#### 1）抽取
首先创建一个module，命名为feign-api：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051702269.png)
项目结构：
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051702659.png)
在feign-api中然后引入feign的starter依赖：
```xml
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-openfeign</artifactId>
</dependency>
```
然后，order-service中编写的UserClient、User、DefaultFeignConfiguration都复制到feign-api项目中
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051702659.png)
#### 2）在order-service中使用feign-api
首先，删除order-service中的UserClient、User、DefaultFeignConfiguration等类或接口。**在order-service的pom文件中中引入feign-api的依赖**：
```xml
<dependency>
    <groupId>cn.itcast.demo</groupId>
    <artifactId>feign-api</artifactId>
    <version>1.0</version>
</dependency>
```
修改order-service中的所有与上述三个组件有关的导包部分，改成导入feign-api中的包
#### 3）重启测试
重启后，发现服务报错了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051713991.png)
这是因为UserClient现在在cn.itcast.feign.clients包下，而order-service的@EnableFeignClients注解是在cn.itcast.order包下，不在同一个包，`SpringBootApplication` 无法扫描到UserClient。编译成功证明有这个类，但没有创建对象说明没有被Spring扫描注册成Bean。
#### 4）解决扫描包问题
方式一：在@EnableFeignClients注解中添加basePackages，指定FeignClient所在的包。
```java
@EnableFeignClients(basePackages = "cn.itcast.feign.clients", defaultConfiguration = ...)
```
方式二：在@EnableFeignClients注解中添加clients，指定具体FeignClient的字节码
。**推荐**，用哪个就指定哪个。
```java
@EnableFeignClients(clients = {UserClient.class}, defaultConfiguration = ...)
```

---
# 2. Gateway服务网关
Spring Cloud Gateway是Spring Cloud的一个全新项目，该项目是基于Spring 5.0，Spring Boot 2.0 和 Project Reactor 等**响应式编程和事件流技术**开发的网关，它旨在==为微服务架构提供一种简单有效的统一的API路由管理方式==。
## 2.1 为什么需要网关
Gateway网关是我们服务的守门神，所有微服务的统一入口。网关的**核心功能特性**：
- 请求路由
- 权限控制
- 限流

架构图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051730326.png)
- **权限控制**：网关作为微服务入口，需要校验用户是是否有请求资格，如果没有则进行拦截。
- **路由和负载均衡**：一切请求都必须先经过Gateway，但网关不处理业务，而是根据某种规则，把请求转发到某个微服务，这个过程叫做路由。当然**路由的目标服务有多个时，还需要做负载均衡**。
- **限流**：当请求流量过高时，在网关中按照下流的微服务能够接受的速度来放行请求，避免服务压力过大。

在SpringCloud中网关的实现包括两种：
- zuul：Zuul是基于Servlet的实现，属于阻塞式编程。
- gateway：SpringCloudGateway则是基于Spring5中提供的WebFlux，属于响应式编程的实现，具备更好的性能。

## 2.2 gateway快速入门
下面，我们就演示下网关的基本路由功能。基本步骤如下：
1. 创建SpringBoot工程gateway，引入网关依赖
2. 编写启动类
3. 编写基础配置和路由规则
4. 启动网关服务进行测试

### 1）创建gateway服务，引入依赖
搭建网关服务的步骤：创建服务模块：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051734652.png)
引入SpringCloudGateway的依赖和nacos的服务发现依赖（网关本身也是个微服务，因此也需要服务发现依赖）：
```xml
<!--网关-->
<dependency>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-gateway</artifactId>
</dependency>
<!--nacos服务发现依赖-->
<dependency>
    <groupId>com.alibaba.cloud</groupId>
    <artifactId>spring-cloud-starter-alibaba-nacos-discovery</artifactId>
</dependency>
```
网关会拉取nacos的服务列表，之后就转发到相对应的服务，你网关不和nacos拉取列表怎么知道转发到哪个服务。
### 2）编写启动类
```java
package cn.itcast.gateway;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class GatewayApplication {
	public static void main(String[] args) {
		SpringApplication.run(GatewayApplication.class, args);
	}
}
```
### 3）编写基础配置和路由规则
创建application.yml文件，编写路由配置及nacos地址，内容如下：
```yaml
server:
  port: 10010 # 网关端口
spring:
  application:
    name: gateway # 服务名称
  cloud:
    nacos:
      server-addr: localhost:8848 # nacos地址
    gateway:
      routes: # 网关路由配置(多个)
        - id: user-service # 路由id，自定义，只要唯一即可
          # uri: http://127.0.0.1:8081 # 路由的目标地址 http就是固定地址
          uri: lb://userservice # 路由的目标地址 lb就是负载均衡，后面跟服务名称
          predicates: # 路由断言，也就是判断请求是否符合路由规则的条件
            - Path=/user/** # 这个是按照路径匹配，只要以/user/开头就符合要求
        - id: ...
	      uri: ...
          predicates: ...
```
我们将「符合 `Path` 规则的一切请求」都代理到 `uri` 参数指定的地址。本例中，我们**将 `/user/**`开头的请求，代理到 `lb://userservice`，lb是负载均衡，根据服务名拉取服务列表，实现负载均衡**。
> gateway配dev集群里面即可，前几集老师把user和order配到dev了，亲测有效，不用引入其他依赖
> 要加loadbalance依赖，新版cloud已经没有ribbon了
> nginx是用户到前端的网关，gateway是前端到后端服务的网关

### 4）重启测试
重启网关，访问http://localhost:10010/user/1时，符合`/user/**`规则，请求转发到uri：http://userservice/user/1，得到了结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051814264.png)
### 5）网关路由的流程图
整个访问的流程如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051811809.png)
总结：网关搭建步骤：
1. 创建项目，引入nacos服务发现和gateway依赖
2. 配置application.yml，包括服务基本信息、nacos地址、路由

路由配置包括：
1. 路由id：路由的唯一标示
2. 路由目标（uri）：路由的目标地址，http代表固定地址，lb代表根据服务名负载均衡
3. 路由断言（predicates）：路由断言，判断请求是否符合要求，符合则转发到路由目的地
4. 路由过滤器（filters）：路由过滤器，处理请求或响应

接下来，就重点来学习路由断言和路由过滤器的详细知识
## 2.3 路由断言工厂Route Predicate Factory
我们在配置文件中写的断言规则只是字符串，这些字符串会被Predicate Factory读取并处理，转变为路由判断的条件。

例如Path=/user/**是按照路径匹配**，这个规则是由 `org.springframework.cloud.gateway.handler.predicate.PathRoutePredicateFactory`类来处理的，像这样的断言工厂在SpringCloudGateway还有十几个:

| **名称**   | **说明**                       | **示例**                                                     |
| ---------- | ------------------------------ | ------------------------------------------------------------ |
| After      | 是某个时间点后的请求           | -  After=2037-01-20T17:42:47.789-07:00[America/Denver]       |
| Before     | 是某个时间点之前的请求         | -  Before=2031-04-13T15:14:47.433+08:00[Asia/Shanghai]       |
| Between    | 是某两个时间点之前的请求       | -  Between=2037-01-20T17:42:47.789-07:00[America/Denver],  2037-01-21T17:42:47.789-07:00[America/Denver] |
| Cookie     | 请求必须包含某些cookie         | - Cookie=chocolate, ch.p                                     |
| Header     | 请求必须包含某些header         | - Header=X-Request-Id, \d+                                   |
| Host       | 请求必须是访问某个host（域名） | -  Host=**.somehost.org,**.anotherhost.org                   |
| Method     | 请求方式必须是指定方式         | - Method=GET,POST                                            |
| Path       | 请求路径必须符合指定规则       | - Path=/red/{segment},/blue/**                               |
| Query      | 请求参数必须包含指定参数       | - Query=name, Jack或者-  Query=name                          |
| RemoteAddr | 请求者的ip必须是指定范围       | - RemoteAddr=192.168.1.1/24                                  |
| Weight     | 权重处理                       |                                                              |

我们只需要掌握Path这种路由工程就可以了。
## 2.4 过滤器工厂
GatewayFilter是网关中提供的一种过滤器，可以对**进入网关的请求和微服务返回的响应**做处理：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051853900.png)
### 2.4.1.路由过滤器的种类
Spring提供了31种不同的路由过滤器工厂GateWayFilter Factory。例如：

| **名称**             | **说明**                     |
| -------------------- | ---------------------------- |
| AddRequestHeader     | 给当前请求添加一个请求头     |
| RemoveRequestHeader  | 移除请求中的一个请求头       |
| AddResponseHeader    | 给响应结果中添加一个响应头   |
| RemoveResponseHeader | 从响应结果中移除有一个响应头 |
| RequestRateLimiter   | 限制请求的流量               |

### 2.4.2 请求头过滤器
下面我们以 `AddRequestHeader` 为例来讲解。
> **需求**：给所有进入 `userservice` 的请求添加一个请求头：`Truth=itcast is freaking awesome!`

只需要修改 `gateway` 服务的 `application.yml` 文件，添加路由过滤即可：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: user-service 
        uri: lb://userservice 
        predicates: 
        - Path=/user/** 
        filters: # 过滤器
        - AddRequestHeader=Truth, Itcast is freaking awesome! # 添加请求头
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303061558375.png)
当前过滤器写在 `userservice` 路由下，因此**仅仅对访问 `userservice` 的请求有效**。
### 2.4.3 默认过滤器
如果要对所有的路由都生效，则可以将过滤器工厂写到 `default` 下。格式如下：
```yaml
spring:
  cloud:
    gateway:
      routes:
      - id: user-service 
        uri: lb://userservice 
        predicates: 
        - Path=/user/**
      default-filters: # 默认过滤项
      - AddRequestHeader=Truth, Itcast is freaking awesome! 
```
### 2.4.4 总结
过滤器的作用是什么？
① 对路由的请求或响应做加工处理，比如添加请求头
② **配置在路由下的过滤器只对当前路由的请求生效**

`defaultFilters` 的作用是什么？
① 对所有路由都生效的过滤器

## 2.5 全局过滤器
上一节学习的过滤器，网关提供了31种，但每一种过滤器的作用都是固定的。如果我们希望拦截请求，做自己的业务逻辑则没办法实现。
### 2.5.1 全局过滤器作用
全局过滤器的作用也是**处理一切进入网关的请求和微服务响应**，与 `GatewayFilter` 的作用一样。区别在于 `GatewayFilter` 通过配置定义，处理逻辑是固定的；而 `GlobalFilter` 的逻辑需要自己写代码实现。定义方式是实现 `GlobalFilter` 接口。
```java
public interface GlobalFilter {
    /**
     *  处理当前请求，有必要的话通过{@link GatewayFilterChain}将请求交给下一个过滤器处理
     *
     * @param exchange 请求上下文，里面可以获取Request、Response等信息
     * @param chain 用来把请求委托给下一个过滤器 
     * @return {@code Mono<Void>} 返回标示当前过滤器业务结束
     */
    Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain);
}
```
在 `filter` 中编写自定义逻辑，可以实现下列功能：
- 登录状态判断
- 权限校验
- 请求限流等

### 2.5.2 自定义全局过滤器
需求：定义全局过滤器，拦截请求，判断请求的参数是否满足下面条件：
- 参数中是否有authorization，
- authorization参数值是否为admin

如果同时满足则放行，否则拦截。实现：在gateway中定义一个过滤器：
```java
package cn.itcast.gateway.filters;

import org.springframework.cloud.gateway.filter.GatewayFilterChain;
import org.springframework.cloud.gateway.filter.GlobalFilter;
import org.springframework.core.annotation.Order;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Component;
import org.springframework.web.server.ServerWebExchange;
import reactor.core.publisher.Mono;

@Order(-1) //越小过滤器优先级越低,还可通过实现Ordered接口改变优先级
@Component //注入到Spring作为一个Bean
public class AuthorizeFilter implements GlobalFilter {
    @Override
    public Mono<Void> filter(ServerWebExchange exchange, GatewayFilterChain chain) {
        // 1.获取请求参数
        MultiValueMap<String, String> params = exchange.getRequest().getQueryParams();
        // 2.获取authorization参数
        String auth = params.getFirst("authorization");
        // 3.校验
        if ("admin".equals(auth)) {
            // 放行给下一个过滤器
            return chain.filter(exchange);
        }
        // 4.拦截
        // 4.1.禁止访问，设置状态码
        exchange.getResponse().setStatusCode(HttpStatus.FORBIDDEN);
        // 4.2.结束处理
        return exchange.getResponse().setComplete();
    }
}
```
### 2.5.3 过滤器执行顺序
请求进入网关会碰到三类过滤器：当前路由的过滤器、DefaultFilter、GlobalFilter。

**请求路由后，会将当前路由过滤器和DefaultFilter、GlobalFilter**（网关中的所有过滤器最终都是GateWayFilter；通过适配器适配），合并到一个过滤器链（集合）中，排序后依次执行每个过滤器：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051942127.png)
排序的规则是什么呢？
- 每一个过滤器都必须指定一个 `int` 类型的order值，**order值越小，优先级越高，执行顺序越靠前**。
- GlobalFilter通过实现Ordered接口，或者添加@Order注解来指定order值，由我们自己指定
- 路由过滤器和defaultFilter的order由Spring指定，**默认是按照声明顺序从1递增**。
- 当过滤器的order值一样时，会按照 defaultFilter > 路由过滤器 > GlobalFilter的顺序执行。

详细内容，可以查看源码： `org.springframework.cloud.gateway.route.RouteDefinitionRouteLocator#getFilters()`方法是先加载 `defaultFilters` ，然后再加载某个route的filters，然后合并。这里源代码用了一个 `List<GatewayFilter>` 保存加载的过滤器，先加载 `DefaultFilter` 过滤器，然后加载路由的过滤器，最后调用了一个方法进行排序。

`org.springframework.cloud.gateway.handler.FilteringWebHandler#handle()`方法会加载全局过滤器，与前面的过滤器合并后根据 `order` 排序，组织过滤器链。
## 2.6 跨域问题
### 2.6.1 什么是跨域问题
之前JavaWeb是Nginx反向代理解决跨域问题。

跨域：**域名不一致就是跨域**，主要包括：
- 域名不同： www.taobao.com 和 www.taobao.org 和 www.jd.com 和 miaosha.jd.com
- 域名相同，端口不同：localhost:8080和localhost8081

跨域问题：==**浏览器**禁止请求的发起者与服务端发生**跨域AJAX请求**，请求被浏览器拦截的问题==。跨域只出现在浏览器客户端和服务端的交互上，由于同源策略的安全限制，其他客户端并不会（这里的UserService到OrderService与浏览器无关，也不是AJAX请求）。

解决方案：CORS（浏览器询问服务器，是否允许跨域），这个以前应该学习过，这里不再赘述了。不知道的小伙伴可以查看https://www.ruanyifeng.com/blog/2016/04/cors.html
### 2.6.2 模拟跨域问题
找到资料的页面文件：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051943193.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303061627859.png)
放入Tomcat或者Nginx这样的Web服务器中，启动并访问。可以在浏览器控制台看到下面的错误：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303051944580.png)

从localhost:8090访问localhost:10010，端口不同，显然是跨域的请求。
### 2.6.3 解决跨域问题
网关处理跨域采用的同样是CORS方案，并且只需要简单配置即可实现.。在gateway服务的application.yml文件中，添加下面的配置：
```yaml
spring:
  cloud:
    gateway:
      globalcors: # 全局的跨域处理(网关特有;AJAX采用CORS,浏览器去问服务器,有一次询问,请问方式是Option,默认情况这种请求是会被网关拦截的)
        add-to-simple-url-handler-mapping: true # 解决options请求被拦截问题,CORS的请求会正常发出
        corsConfigurations:
          '[/**]':
            allowedOrigins: # 允许哪些网站的跨域请求 
              - "http://localhost:8090"
            allowedMethods: # 允许的跨域ajax的请求方式
              - "GET"
              - "POST"
              - "DELETE"
              - "PUT"
              - "OPTIONS"
            allowedHeaders: "*" # 允许在请求中携带的头信息吗
            allowCredentials: true # 是否允许携带cookie
            maxAge: 360000 # 这次跨域检测的有效期(减少CORS性能损耗),有效期内浏览器不在询问直接放行
```

