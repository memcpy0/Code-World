# 1. 分布式系统中的相关概念
## 1.1 大型互联网项目**架构**目标
传统项目和互联网项目有很多不同：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081809371.png)

- 用户群体不同：一个是企业员工；一个是广大网民，形形色色
- 数量级不同：企业员工有限，比如几千人、几万人。网民人数众多。
- 用户忍耐力不同：传统项目可以忍受；互联网项目零忍耐。

（互联网项目的）用户体验：美观、功能、速度、稳定性

衡量一个网站速度是否快：打开一个新页面一瞬间完成；页面内跳转，一刹那间完成。
> 根据佛经《僧祇律》记载：一刹那者为一念，二十念为一瞬，二十瞬为一弹指，二十弹指为一罗预，二十罗预为一须臾，一日一夜有三十须臾。 
> 经过周密的计算，一瞬间为0.36秒,一刹那有0.018秒 

互联网项目特点：
- 用户多
- 流量大，并发高
- 海量数据
- 易受攻击
- 功能繁琐
- 变更快

**衡量网站的性能指标**：
- 响应时间：指执行一个请求从开始到最后收到响应数据所花费的总体时间。
- 并发数：指**系统同时能处理的请求数量**。
	- **并发连接数**：指的是客户端向服务器发起请求，并建立了TCP连接。每秒钟服务器连接的总TCP数量（一个连接可以发出多个请求）
	- **请求数**：也称为QPS(Query Per Second) 指每秒多少请求.
	- **并发用户数**：单位时间内有多少用户
- 吞吐量：指单位时间内系统能处理的请求数量。
	- QPS：Query Per Second 每秒查询数。 
	- TPS：Transactions Per Second 每秒事务数。 ==一个事务（刷新一次网页页面）是指一个客户机向服务器发送请求、然后服务器做出反应的过程==。客户机在发送请求时开始计时，收到服务器响应后结束计时，以此来计算使用的时间和完成的事务个数。

一个页面的一次访问，只会形成一个TPS；但一次页面访问，可能产生多次对服务器的请求，就会有多个QPS。QPS >= 并发连接数 >= TPS

大型互联网项目**架构**目标：
- 高性能：提供快速的访问体验。
- 高可用：网站服务**一直可以正常访问**。
- 可伸缩：通过硬件增加/减少，**提高/降低处理能力**。
- 高可扩展：**系统间耦合低**，方便的通过新增/移除方式，**增加/减少新的功能/模块**。 
- 安全性：提供网站安全访问和数据加密，安全存储等策略。
- 敏捷性：随需应变，快速响应。

## 1.2 集群和分布式
集群：很多“人”（机器）一起 ，**干一样的事**。 -- 高性能 -- 高可用
- 一个业务模块，部署在多台服务器上。 

分布式：很多“人”一起，**干不一样的事**。这些不一样的事，**合起来是一件大事**。 -- 可伸缩，高可扩展
- 一个大的业务系统，拆分为小的业务模块，分别部署在不同的机器上。 

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081812413.png)

早期架构：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081813162.png)
集群架构优化：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081832699.png)
分布式架构优化：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081833203.png)
## 1.3 架构演进
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081834684.png)
### 1.3.1 单体架构
**部署在单机就是单机单体，部署在多机就是集群单体项目。**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081835327.png)
优点：
- 简单：开发部署都很方便，小型项目首选

缺点：时间推移，业务增长、访问用户变多
- 项目启动慢
- 可靠性差
- 可伸缩性差
- 扩展性和可维护性差
- 性能低
### 1.3.2 垂直架构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081839644.png)
垂直架构是指将**单体架构中的多个模块**拆分为多个独立的项目。形成**多个独立的单体架构**。

垂直架构存在的问题：**重复功能太多。有些公共组件会在多个独立项目中重复存在**。
### 1.3.3 分布式架构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081841377.png)

分布式架构是指==在垂直架构的基础上，将公共业务模块抽取出来，作为独立的服务，供其他调用者消费，以实现服务的共享和重用==。

RPC： Remote Procedure Call 远程过程调用。有非常多的协议和技术来都实现了RPC的过程。比如：**HTTP REST风格**，Java RMI规范、WebService SOAP协议、Hession等等。

垂直架构存在的问题：重复功能太多

分布式架构存在的问题：**服务提供方一旦产生变更，所有消费方都需要变更**。
### 1.3.4 SOA架构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081843124.png)
*SOA, Service-Oriented Architecture* 面向服务的架构是一个组件模型，它将应用程序的不同功能单元（称为服务）进行拆分，并通过这些服务之间定义**良好的接口和契约**联系起来。

*ESB, Enterparise Service Bus* 企业服务总线，服务中介。主要是**提供了一个服务于服务之间的交互**（核心！！）。ESB包含的功能如：负载均衡，流量控制，加密处理，服务的监控，异常处理，监控告急等等。

分布式架构存在的问题（服务提供方一旦产生变更，所有消费方都需要变更）得到解决，因**为不直接调用，而是通过中介调用**。

### 1.3.5 微服务架构
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081846555.png)
**微服务架构是在 SOA 上做的升华**，微服务架构强调的一个重点是“**业务需要彻底的组件化和服务化**”，==原有的单个业务系统会拆分为多个可以独立开发、设计、运行的小应用==。这些小应用之间**通过服务完成交互和集成**。

微服务架构 = 80%的SOA服务架构思想 + 100%的组件化架构思想 + 80%的领域建模思想
特点：
- 服务实现组件化：开发者可以自由选择开发技术。也不需要协调其他团队
- 服务之间交互一般使用REST API
- 去中心化：**每个微服务有自己私有的数据库、持久化业务数据**
- 自动化部署：把应用拆分成为一个一个独立的单个服务，方便自动化部署、测试、运维

**Dubbo是SOA时代的产物，SpringCloud是微服务时代的产物**

---
# 2. Dubbo概述
## 2.1 Dubbo概念
Dubbo是阿里巴巴公司开源的一个高性能、**轻量级的Java RPC框架**。致力于提供高性能和透明化的RPC远程服务调用方案，以及SOA服务治理方案。官网：http://dubbo.apache.org。
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081849514.png)
节点角色说明：
- Provider：暴露服务的服务提供方，服务要运行在一个Container中
- Container：服务运行容器
- Consumer：调用远程服务的服务消费方
- Registry：服务注册与发现的注册中心
- Monitor：**统计服务的调用次数和调用时间的监控中心**

## 2.2 Dubbo快速入门
### 2.2.1 Zookeeper安装
Dubbo官方推荐使用Zookeeper作为注册中心。
### 2.2.2 Dubbo入门案例
实现步骤：两个模块，是两个项目。先是Spring和SpringMVC整合
1. 创建服务提供者Provider模块
2. 创建服务消费者Consumer模块
3. 在服务提供者模块编写 `UserServiceImpl` 提供服务
4. 在服务消费者中的 `UserController` 远程调用 `UserServiceImpl` 提供的服务
5. 分别启动两个服务，测试
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081904627.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081906318.png)

> 不直接创建maven项目是为了管理好包及其子包

因为web.xml里 `<servlet-mapping>` 标签配置的映射是*.do，所以访问在结尾要加.do。

一开始web模块直接调用service模块，不是分布式，还是单机架构，因为分布式的模块要能独立启动并向外提供服务。现在通过Dubbo完成web模块和service模块的远程交互。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081905618.png)
dubbo-service（spring）和dubbo-web（springmvc）的pom.xml基本类似，都使用war打包方式，以对外提供服务：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.itheima</groupId>
    <artifactId>dubbo-service</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>war</packaging>
 
    <properties>
        <spring.version>5.1.9.RELEASE</spring.version>
        <dubbo.version>2.7.4.1</dubbo.version>
        <zookeeper.version>4.0.0</zookeeper.version>

    </properties>

    <dependencies>
        <!-- servlet3.0规范的坐标 -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
            <scope>provided</scope>
        </dependency>
        <!--spring的坐标-->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <!--springmvc的坐标-->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
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

        <!--Dubbo的起步依赖，版本2.7之后统一为rg.apache.dubb -->
        <dependency>
            <groupId>org.apache.dubbo</groupId>
            <artifactId>dubbo</artifactId>
            <version>${dubbo.version}</version>
        </dependency>
        <!--ZooKeeper客户端实现 -->
        <dependency>
            <groupId>org.apache.curator</groupId>
            <artifactId>curator-framework</artifactId>
            <version>${zookeeper.version}</version>
        </dependency>
        <!--ZooKeeper客户端实现 -->
        <dependency>
            <groupId>org.apache.curator</groupId>
            <artifactId>curator-recipes</artifactId>
            <version>${zookeeper.version}</version>
        </dependency>

        <!--依赖公共的接口模块-->
        <dependency>
            <groupId>com.itheima</groupId>
            <artifactId>dubbo-interface</artifactId>
            <version>1.0-SNAPSHOT</version>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <!--tomcat插件-->
            <plugin>
                <groupId>org.apache.tomcat.maven</groupId>
                <artifactId>tomcat7-maven-plugin</artifactId>
                <version>2.1</version>
                <configuration>
                    <port>9000</port>
                    <path>/</path>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```
dubbo-service的com.itheima.service.impl中的类 `UserServiceImpl`（查了一下，**Dubbo2.7以后，springboot中@Service被DubboService替代**）：
```java
package com.itheima.service.impl;

import com.itheima.service.UserService;
import org.apache.dubbo.config.annotation.Service;


//@Service//将该类的对象创建出来，放到Spring的IOC容器中  bean定义

@Service//将这个类提供的方法（服务）对外发布。将访问的地址 ip，端口，路径注册到注册中心中
public class UserServiceImpl implements UserService {

    public String sayHello() {
        return "hello dubbo hello!~";
    }
}
```
`dubbo-service/src/main/resources/spring/applicationContext.xml` 中：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	   xmlns:dubbo="http://dubbo.apache.org/schema/dubbo" xmlns:context="http://www.springframework.org/schema/context"
	   xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
        http://dubbo.apache.org/schema/dubbo http://dubbo.apache.org/schema/dubbo/dubbo.xsd http://www.springframework.org/schema/context https://www.springframework.org/schema/context/spring-context.xsd">

	<!--<context:component-scan base-package="com.itheima.service" />-->
	<!--dubbo的配置-->
	<!--1.配置项目的名称,唯一-->
	<dubbo:application name="dubbo-service"/>
	<!--2.配置注册中心的地址-->
	<dubbo:registry address="zookeeper://192.168.149.135:2181"/>
	<!--3.配置dubbo包扫描-->
	<dubbo:annotation package="com.itheima.service.impl" />
</beans>
```
`dubbo-service/.../webapp/WEB-INF/web.xml` ：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns="http://java.sun.com/xml/ns/javaee"
         xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
         version="2.5">

		 
	<!-- spring -->
    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath*:spring/applicationContext*.xml</param-value>
    </context-param>
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>


</web-app>
```

dubbo-web的 `java/com/itheima/controller/UserController.java` ：
```java
package com.itheima.controller;

import com.itheima.service.UserService;
import org.apache.dubbo.config.annotation.Reference;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/user")
public class UserController {
    //注入Service
    //@Autowired//本地注入

    /*
	1. 从zookeeper注册中心获取userService的访问url
	2. 进行远程调用RPC
	3. 将结果封装为一个代理对象。给变量赋值
     */
    @Reference//远程注入
    private UserService userService;

    @RequestMapping("/sayHello")
    public String sayHello(){
        return userService.sayHello();
    }
}
```
dubbo 2.7.0版本后 @Reference已弃用，建议使用@DubboReference 。
> 上个视频我的报错原因是：我安装的zookeeper版本和pom文件中的zk版本不匹配，我把pom中的zk版本调低就不报错了。

dubbo-web的 `src/main/resources/spring/springmvc.xml` ：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:dubbo="http://dubbo.apache.org/schema/dubbo"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd
         http://dubbo.apache.org/schema/dubbo http://dubbo.apache.org/schema/dubbo/dubbo.xsd http://www.springframework.org/schema/context https://www.springframework.org/schema/context/spring-context.xsd">

    <mvc:annotation-driven/>
    <context:component-scan base-package="com.itheima.controller"/>

    <!--dubbo的配置-->
    <!--1.配置项目的名称,唯一-->
    <dubbo:application name="dubbo-web" >
        <dubbo:parameter key="qos.port" value="33333"/>
    </dubbo:application>
    <!--2.配置注册中心的地址-->
    <dubbo:registry address="zookeeper://192.168.149.135:2181"/>
    <!--3.配置dubbo包扫描-->
    <dubbo:annotation package="com.itheima.controller" />

</beans>
```
然后  `dubbo-web/src/.../webapp/WEB-INF/web.xml` ：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns="http://java.sun.com/xml/ns/javaee"
         xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
         version="2.5">

	<!-- Springmvc -->	 
    <servlet>
        <servlet-name>springmvc</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <!-- 指定加载的配置文件 ，通过参数contextConfigLocation加载-->
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring/springmvc.xml</param-value>
        </init-param>
    </servlet>

    <servlet-mapping>
        <servlet-name>springmvc</servlet-name>
        <url-pattern>*.do</url-pattern>
    </servlet-mapping>
</web-app>
```

---
# 3. Dubbo高级特性
## 3.1 dubbo-admin管理平台
dubbo-admin比Monitor更强大的Monitor。

dubbo-admin管理平台，是图形化的服务管理页面，从注册中心中获取到所有的提供者/消费者进行配置管理。路由规则、动态配置、服务降级、访问控制、权重调整、负载均衡等管理功能。dubbo-admin是一个前后端分离的项目。前端使用vue，后端使用springboot。**安装dubbo-admin其实就是部署该项目**。
## 3.2 dubbo-admin安装
**1、环境准备**
我们将dubbo-admin安装到开发环境上。要保证开发环境有jdk，maven，nodejs

安装node(如果当前机器已经安装请忽略)。因为前端工程是用vue开发的，所以需要安装node.js，node.js中自带了npm，后面我们会通过npm启动

下载地址 https://nodejs.org/en/
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082059666.png)

**2、下载 Dubbo-Admin**
进入github，搜索dubbo-admin，https://github.com/apache/dubbo-admin。下载：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082100869.png)
**3、把下载的zip包解压到指定文件夹(解压到那个文件夹随意)**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082100868.png)

**4、修改配置文件**
解压后我们进入 `…\dubbo-admin-develop\dubbo-admin-server\src\main\resources` 目录，找到 **application.properties** 配置文件 进行配置修改
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082101382.png)
修改zookeeper地址为虚拟机的IP，而非本机的IP：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082101920.png)
```shell
# centers in dubbo2.7
admin.registry.address=zookeeper://192.168.149.135:2181
admin.config-center=zookeeper://192.168.149.135:2181
admin.metadata-report.address=zookeeper://192.168.149.135:2181
```
admin.registry.address注册中心
admin.config-center 配置中心
admin.metadata-report.address元数据中心
**5、打包项目**
在 dubbo-admin-develop 目录执行打包命令
```shell
mvn  clean package
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082102835.png)
打包失败的，单独去server里面打包一次，再出来打包，记得是用mvn clean package -Dmaven.test.skip=true
**6、启动后端**
切换到目录
```shell
dubbo-Admin-develop\dubbo-admin-distribution\target>
```
执行下面的命令启动 dubbo-admin，dubbo-admin后台由SpringBoot构建。
```shell
java -jar .\dubbo-admin-0.1.jar
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082103985.png)

**7、前台后端**
dubbo-admin-ui 目录下执行命令
```shell
npm run dev
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082103319.png)
**8、访问**
浏览器输入。用户名密码都是root
```
http://localhost:8081/
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082104733.png)
## 二、dubbo-admin简单使用 
注意:Dubbo Admin【服务Mock】【服务统计】将在后续版本发布....

在上面的步骤中，我们已经进入了Dubbo-Admin的主界面，在【快速入门】章节中，我们定义了服务生产者、和服务消费者，下面我们从Dubbo-Admin管理界面找到这个两个服务
**1、点击服务查询**
**2、查询结果**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082106686.png)
A:输入的查询条件com.itheima.service.UserService
B:搜索类型，主要分为【按服务名】【按IP地址】【按应用】三种类型查询
C:搜索结果

**dubo-admin查看详情**
我们查看com.itheima.service.UserService （服务提供者）的具体详细信息，包含【元数据信息】
**1）点击详情**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082107944.png)
从【详情】界面查看，主要分为3个区域
- A区域：主要包含服务端 基础信息比如服务名称、应用名称等
- B区域：主要包含了生产者、消费者一些基本信息
- **C区域：是元数据信息，注意看上面的图，元数据信息是空的**

我们需要打开我们的生产者配置文件，加入下面配置
```xml
    <!-- 元数据配置 -->
    <dubbo:metadata-report address="zookeeper://192.168.149.135:2181" />
```
重新启动生产者，再次打开Dubbo-Admin。这样我们的元数据信息就出来了
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082108516.png)

## 3.3 dubbo常用高级配置
### 3.3.1 序列化
两个机器传输数据，如何传输Java对象？
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081908622.png)

dubbo内部已经将序列化和反序列化的过程内部封装了。我们只需要在定义pojo类（如class User）时实现Serializable接口即可。一般会定义一个公共的pojo模块（比如将User类放到这个公共模块中），让生产者和消费者都通过Maven的依赖来依赖该模块。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082115319.png)
### 3.3.2 地址缓存
注册中心挂了，服务是否可以正常访问？——可以，因为dubbo服务消费者在第一次调用时，会将服务提供方地址缓存到本地，以后调用则不会访问注册中心。**当服务提供者地址发生变化时，注册中心会通知服务消费者**。
### 3.3.3 超时与重试
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081909429.png)

服务消费者在调用服务提供者时发生了阻塞、等待的情形，这个时候，服务消费者会一直等待下去。在某个峰值时刻，大量的请求都在同时请求服务消费者，会造成线程的大量堆积，势必会造成雪崩。

dubbo利用超时机制来解决这个问题，设置一个超时时间，在这个时间段内，无法完成服务访问，则自动断开连接。使用timeout属性配置超时时间，默认值1000，单位毫秒。（服务的提供方）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082120022.png)
（服务的调用者）：1秒生效还是3秒生效？1秒生效！
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082123933.png)
超时时间应该是服务的提供者给出，而不要配置在服务的调用者一方！

如果出现网络**抖动**，则这一次请求就会失败。Dubbo提供（超时后）重试机制来避免类似问题的发生。通过 retries  属性来设置重试次数。默认为2次。重试完成后还没成功，就认为断了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082125682.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082126732.png)

### 3.3.4 多版本
灰度发布：当出现新功能时，会让一部分用户先使用新功能，用户反馈没问题时，再将所有用户迁移到新功能。dubbo中使用 `version` 属性来设置和调用同一个接口的不同版本。
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081910354.png)
下面启用两个版本：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082156720.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082157963.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082158722.png)
调用改为 `"2.0"` 之后，就会调用新版本。
### 3.3.5 负载均衡策略
> 不用，最小活跃数量，访问加一，完成减一，本地记录就行，老师讲错了

4种 ：
- Random ：按权重随机，默认值。**按权重设置随机概率**。
- RoundRobin ：按权重轮询
	![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081911612.png)
- LeastActive：最少活跃调用数，相同活跃数的随机。
	![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081911954.png)
- ConsistentHash：一致性 Hash，相同参数的请求总是发到同一提供者。
	![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303081912594.png)
在服务提供者那边配置权重：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082207188.png)
第一个服务启动没有问题，但是其他服务的端口却会冲突，需要改变dubbo:protocol port和Tomcat启动的端口……
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082202668.png)
配置均衡策略：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082212767.png)
下面看到不同的权重：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082213749.png)



### 3.3.6 集群容错
集群容错模式：
- Failover Cluster ：失败重试。默认值。当出现失败，重试其它服务器 ，默认重试2次，使用 `retries` 配置。一般用于读操作
- Failfast Cluster ：快速失败，**只发起一次调用，失败立即报错**。通常用于写操作。
- Failsafe Cluster ：失败安全，出现异常时，直接忽略。返回一个空结果。
- Failback Cluster ：失败自动恢复，**后台记录失败请求，定时重发**。通常用于消息通知操作。
- Forking Cluster ：并行调用多个服务器，只要一个成功即返回。
- Broadcast  Cluster ：广播调用所有提供者，逐个调用，任意一台报错则报错。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082218408.png)

### 3.3.7 服务降级
Random LoadBalance
内存要崩溃等威胁情况下，将边缘不重要的服务如广告、日志关闭，保证核心服务的正常进行。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303082222736.png)

服务降级方式：
- `mock = fore:return null` 表示：（注解）消费方对该服务的方法调用，都直接返回null值，不发起远程调用。用来屏蔽不重要服务不可用时、对调用方的影响。
- `mock = fail:return null` 表示：消费方对该服务的方法调用，在失败后再返回 `null` 值，不抛异常。用来容忍不重要服务不稳定时、对调用方的影响。