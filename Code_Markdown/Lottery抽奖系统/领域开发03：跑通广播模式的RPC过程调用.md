-   分支：[210804_xfg_buildFramework](https://gitcode.net/KnowledgePlanet/Lottery/-/tree/210804_xfg_buildFramework)
-   描述：**构建工程完成RPC接口的实现和调用**

当基础的工程模块创建完成以后，还需要给整个工程注入`灵魂`，就是**让它可以跑通**。这个过程包括**一个简单的 RPC 接口功能实现**和**测试调用**，那么这里为了让功能体现出一个完整度，还会**创建出一个库表**，在 RPC 调用时查询出库表中的数据、并🔙返回结果。那么在这个分支上，我们就先来完成这样一个内容的实现。

-   常见错误：
    -   Dubbo 接口无论是入参、出参，以及参数中的包括的对象，都必须 implements Serializable
    -   Dubbo 接口实现需要使用 `@org.apache.dubbo.config.annotation.Service` 不要用 Spring 的 Service
    -   关于链接错误：[Dubbo 广播模式失败问题](https://gitcode.net/KnowledgePlanet/Lottery/-/issues/1 "关于第三节:跑通广播模式RPC过程调用的Dubbo的广播模式调用失败问题。")
    -   检查数据库配置、对象注入、工程启动是否成功

> Dubbo2.7以后，springboot中@Service被DubboService替代
> @Service ：**将该类的对象创建出来**，放到Spring的IOC容器中  bean定义
> @Service ：**将这个类提供的方法（服务）对外发布**。将访问的地址 ip，端口，路径注册到注册中心中

---
⛳ **目录**
-   [一、创建抽奖活动表](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E4%B8%80%E5%88%9B%E5%BB%BA%E6%8A%BD%E5%A5%96%E6%B4%BB%E5%8A%A8%E8%A1%A8)
-   [二、POM 文件配置](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E4%BA%8Cpom-%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE)
-   [三、配置Mybatis](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E4%B8%89%E9%85%8D%E7%BD%AEmybatis)
-   [四、配置广播模式 Dubbo](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E5%9B%9B%E9%85%8D%E7%BD%AE%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8F-dubbo)
-   [五、定义和开发 RPC 接口](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E4%BA%94%E5%AE%9A%E4%B9%89%E5%92%8C%E5%BC%80%E5%8F%91-rpc-%E6%8E%A5%E5%8F%A3)
-   [六、搭建测试工程调用 RPC](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E5%85%AD%E6%90%AD%E5%BB%BA%E6%B5%8B%E8%AF%95%E5%B7%A5%E7%A8%8B%E8%B0%83%E7%94%A8-rpc)
-   [七、本章知识点](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-03-%E8%8A%82%EF%BC%9A%E8%B7%91%E9%80%9A%E5%B9%BF%E6%92%AD%E6%A8%A1%E5%BC%8FRPC%E8%BF%87%E7%A8%8B%E8%B0%83%E7%94%A8#%E4%B8%83%E6%9C%AC%E7%AB%A0%E7%9F%A5%E8%AF%86%E7%82%B9)

## 一、创建抽奖活动表
在抽奖活动的设计和开发过程中，会涉及到的表信息包括：活动表、奖品表、策略表、规则表、用户参与表、中奖信息表等，这些都会在我们随着开发抽奖的过程中不断的添加出来这些表的创建。

那么目前我们为了先把程序跑通，可以**先简单创建出一个活动表，用于实现系统对数据库的CRUD操作**，也就可以被RPC接口调用。在后面陆续实现的过程中，可能会有一些不断优化和调整的点，用于满足系统对需求功能的实现。

**活动表(activity)**：编码是utf8mb4
```sql
CREATE TABLE `activity` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增ID',
  `activity_id` bigint(20) NOT NULL COMMENT '活动ID',
  `activity_name` varchar(64) CHARACTER SET utf8mb4 DEFAULT NULL COMMENT '活动名称',
  `activity_desc` varchar(128) CHARACTER SET utf8mb4 DEFAULT NULL COMMENT '活动描述',
  `begin_date_time` datetime DEFAULT NULL COMMENT '开始时间',
  `end_date_time` datetime DEFAULT NULL COMMENT '结束时间',
  `stock_count` int(11) DEFAULT NULL COMMENT '库存',
  `take_count` int(11) DEFAULT NULL COMMENT '每人可参与次数',
  `state` tinyint(2) DEFAULT NULL COMMENT '活动状态：1编辑、2提审、3撤审、4通过、5运行(审核通过后worker扫描状态)、6拒绝、7关闭、8开启',
  `creator` varchar(64) CHARACTER SET utf8mb4 DEFAULT NULL COMMENT '创建人',
  `create_time` datetime DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间',
  `update_time` datetime DEFAULT CURRENT_TIMESTAMP COMMENT '修改时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `unique_activity_id` (`activity_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin COMMENT='活动配置';
```

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="cn.itedus.lottery.infrastructure.dao.IActivityDao">
    <resultMap id="activityMap" type="cn.itedus.lottery.infrastructure.po.Activity">
        <id column="id" property="id"/>
        <result column="activity_id" property="activityId"/>
        <result column="activity_name" property="activityName"/>
        <result column="activity_desc" property="activityDesc"/>
        <result column="begin_date_time" property="beginDateTime"/>
        <result column="end_date_time" property="endDateTime"/>
        <result column="stock_count" property="stockCount"/>
        <result column="take_count" property="takeCount"/>
        <result column="state" property="state"/>
        <result column="creator" property="creator"/>
        <result column="create_time" property="createTime"/>
        <result column="update_time" property="updateTime"/>
    </resultMap>
    <insert id="insert" parameterType="cn.itedus.lottery.infrastructure.po.Activity">
        INSERT INTO activity
        (activity_id, activity_name, activity_desc, begin_date_time, end_date_time,
        stock_count, take_count, state, creator, create_time, update_time)
        VALUES
        (#{activityId}, #{activityName}, #{activityDesc}, #{beginDateTime}, #{endDateTime},
         #{stockCount}, #{takeCount}, #{state}, #{creator}, now(), now())
    </insert>
    <select id="queryActivityById" parameterType="java.lang.Long" resultMap="activityMap">
        SELECT activity_id, activity_name, activity_desc, begin_date_time, end_date_time,
        stock_count, take_count, state, creator, create_time, update_time
        FROM activity
        WHERE activity_id = #{activityId}
    </select>
</mapper>
```
-   活动表：**是一个用于配置抽奖活动的总表**，用于存放活动信息，包括：ID、名称、描述、时间、库存、参与次数等。有两个键：id和activity_id

## 二、POM 文件配置
按照现有工程的结构模块分层，包括：
-   lottery-application，应用层，引用：`domain`
-   lottery-common，通用包，引用：`无`
-   lottery-domain，领域层，引用：`infrastructure`
-   lottery-infrastructure，基础层，引用：`无`
-   lottery-interfaces，接口层，引用：`application`、`rpc`
-   lottery-rpc，**RPC接口定义层**，引用：`common`

**在此分层结构和依赖引用下，各层级模块不能循环依赖**，同时 **`lottery-interfaces` 作为系统的 war 包工程**，在构建工程时需要依赖于 POM 中配置的相关信息。

那这里就需要注意，作为 Lottery 工程下的主 pom.xml 需要完成对 SpringBoot 父文件的依赖，此外还需要定义一些用于其他模块可以引入的配置信息，比如：jdk版本、编码方式等。而其他层在依赖于工程总 pom.xml 后，还需要配置自己的信息。
```xml
<?xml version="1.0" encoding="UTF-8"?>  
<project xmlns="http://maven.apache.org/POM/4.0.0"  
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
    <parent>        <artifactId>Lottery</artifactId>  
        <groupId>com.memcpy0.lottery</groupId>  
        <version>1.0-SNAPSHOT</version>  
    </parent>    <modelVersion>4.0.0</modelVersion>  
  
    <artifactId>lottery-interfaces</artifactId>  
  
    <packaging>war</packaging>  
  
</project>
```
### 1. 总工程下POM配置
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>cn.itedus.lottery</groupId>
    <artifactId>Lottery</artifactId>
    <packaging>pom</packaging>
    <version>1.0-SNAPSHOT</version>
    <modules>
        <module>lottery-application</module>
        <module>lottery-domain</module>
        <module>lottery-infrastructure</module>
        <module>lottery-interfaces</module>
        <module>lottery-rpc</module>
        <module>lottery-common</module>
    </modules>
    <properties>
        <!-- Base -->
        <jdk.version>1.8</jdk.version>
        <sourceEncoding>UTF-8</sourceEncoding>
    </properties>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.3.5.RELEASE</version>
        <relativePath/> <!-- lookup parent from repository -->
    </parent>
    <dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>org.mybatis.spring.boot</groupId>
            <artifactId>mybatis-spring-boot-starter</artifactId>
            <version>2.1.4</version>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.34</version>
        </dependency>
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>1.2.58</version>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-lang3</artifactId>
            <version>3.8</version>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <scope>test</scope>
        </dependency>
    </dependencies>
</project>
```
相对于工程下其他的模块来说，**总POM就是这些模块的父类模块**，在父类模块中一般只提供基础的定义，**不提供各个Jar包的引入配置**。==如果在父类 POM 中引入了所有的 Jar，那么各个模块无论是否需要这个 Jar 都会被自动引入进去，造成没必要的配置==，也会影响对核心Jar的扰乱，让你分不清自己需要的是否就在眼前。

上面的一些Jar的引用后面会被去掉。
### 2. 模块类POM配置
lottery_infrastructure：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <parent>
        <artifactId>Lottery</artifactId>
        <groupId>cn.itedus.lottery</groupId>
        <version>1.0-SNAPSHOT</version>
    </parent>
    <modelVersion>4.0.0</modelVersion>
    <artifactId>lottery-infrastructure</artifactId>
    <build>
        <finalName>lottery-infrastructure</finalName>
        <plugins>
            <!-- 编译plugin -->
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>${jdk.version}</source>
                    <target>${jdk.version}</target>
                    <compilerVersion>1.8</compilerVersion>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```
lottery-rpc：
```xml
<parent>
    <artifactId>Lottery</artifactId>
    <groupId>cn.itedus.lottery</groupId>
    <version>1.0-SNAPSHOT</version>
</parent>
<modelVersion>4.0.0</modelVersion>
<artifactId>lottery-rpc</artifactId>

<packaging>jar</packaging>

<dependencies>
    <dependency>
        <groupId>cn.itedus.lottery</groupId>
        <artifactId>lottery-common</artifactId>
        <version>1.0-SNAPSHOT</version>
    </dependency>
</dependencies>

<build>
    <finalName>lottery-rpc</finalName>
    <plugins>
        <!-- 编译plugin -->
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-compiler-plugin</artifactId>
            <configuration>
                <source>${jdk.version}</source>
                <target>${jdk.version}</target>
                <compilerVersion>1.8</compilerVersion>
            </configuration>
        </plugin>
    </plugins>
</build>
```
在各个模块配置中需要关注点包括：依赖父POM配置 `parent` 、构建包类型 `packaging`、需要引入的包 `dependencies`、构建信息 `build` ，之所以要配置这个是有些时候，在这个模块工程下还可能会有一些差异化信息的引入。

### 3. War包pom配置
```xml
<artifactId>lottery-interfaces</artifactId>

<packaging>war</packaging>

<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    ...
</dependencies>

<build>
    <finalName>Lottery</finalName>
    <resources>
        <resource>
            <directory>src/main/resources</directory>
            <filtering>true</filtering>
            <includes>
                <include>**/**</include>
            </includes>
        </resource>
    </resources>
    <testResources>
        <testResource>
            <directory>src/test/resources</directory>
            <filtering>true</filtering>
            <includes>
                <include>**/**</include>
            </includes>
        </testResource>
    </testResources>
    <plugins>
        <plugin>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-maven-plugin</artifactId>
        </plugin>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-compiler-plugin</artifactId>
            <configuration>
                <source>8</source>
                <target>8</target>
            </configuration>
        </plugin>
    </plugins>
</build>
```
-  lottery-interfaces 是整个程序的出口，也是用于构建 War 包的工程模块，所以你会看到一个 `<packaging>war</packaging>` 的配置。
-   **在 dependencies 会包含所有需要用到的 SpringBoot 配置，也会包括对其他各个模块的引入**。
-   在 build 构建配置上还会看到一些关于测试包的处理，比如这里包括了资源的引入，也可以包括构建时跳过测试包的配置。

## 三、配置Mybatis
SpringBoot 与 Mybatis 的结合使用还是非常方便的，暂时我们也还没有引入分库分表路由的组件，所以只是一个引入操作数据库的简单配置。
**引入 starter**
```xml
<dependency>
    <groupId>org.mybatis.spring.boot</groupId>
    <artifactId>mybatis-spring-boot-starter</artifactId>
    <version>2.1.4</version>
</dependency>
```
在 SpringBoot 的使用中，你会看到各种 xxx-starter，它们这些组件的包装都是用于完成桥梁的作用，把一些服务交给 SpringBoot 启动时初始化或者加载配置等操作。
**配置 yml**：
```yml
spring:
  datasource:
    username: root
    password: 1234
    url: jdbc:mysql://127.0.0.1:3306/lottery?useUnicode=true
    driver-class-name: com.mysql.jdbc.Driver

mybatis:
  mapper-locations: classpath:/mybatis/mapper/*.xml
  config-location:  classpath:/mybatis/config/mybatis-config.xml
```
-   **配置 Spring.datasource 数据源和mybatis文件的引入配置**，这些mybatis文件主要包括了各个 mapper 的处理。_当然你也可以习惯于使用注解方式使用数据库_
-   ==如果是引入了分库分表组件，那么这里就需要配置多个数据源信息，在后面的开发过程中会有所体现==。

mybatis-config.xml：在interfaces/src/main/resources/mybatis/config下，其实也可以放在infrastructure中，当这里是为了统一管理配置信息。
```xml
<?xml version="1.0" encoding="UTF-8"?>  
<!DOCTYPE configuration  
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"  
        "http://mybatis.org/dtd/mybatis-3-config.dtd">  
<configuration>  
    <typeAliases>        <package name="com.memcpy0.lottery.infrastructure.po.Activity"/>  
    </typeAliases></configuration>
```
Activity_Mapper.xml：
```xml
<?xml version="1.0" encoding="UTF-8"?>  
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">  
<mapper namespace="com.memcpy0.lottery.infrastructure.dao.IActivityDao">  
  
    <insert id="insert" parameterType="com.memcpy0.lottery.infrastructure.po.Activity">  
        INSERT INTO activity  
        (activityId, activityName, activityDesc,beginDateTime, endDateTime,        stockAllTotal, stockDayTotal, takeAllCount, takeDayCount, state,        creator, createTime, updateTime)        VALUES        (#{activityId}, #{activityName}, #{activityDesc},#{beginDateTime}, #{endDateTime},         #{stockAllTotal}, #{stockDayTotal}, #{takeAllCount}, #{takeDayCount}, #{state},         #{creator}, now(), now())    </insert>  
  
    <select id="queryActivityById" parameterType="java.lang.Long" resultType="com.memcpy0.lottery.infrastructure.po.Activity">  
        SELECT activityId, activityName, activityDesc,beginDateTime, endDateTime,  
        stockAllTotal, stockDayTotal, takeAllCount, takeDayCount, state,        creator, createTime, updateTime        FROM activity        WHERE activityId = #{activityId}    </select>  
  
</mapper>
```

在 lottery-interfaces的pom依赖中引入infrastructure是为了做测试，引入rpc包是为了直接使用或实现其中的接口：
```xml
<dependency>  
    <groupId>org.apache.dubbo</groupId>  
    <artifactId>dubbo</artifactId>  
    <version>2.7.1</version>  
</dependency>  
<dependency>  
    <groupId>org.apache.dubbo</groupId>  
    <artifactId>dubbo-spring-boot-starter</artifactId>  
    <version>2.7.1</version>  
</dependency>
<dependency>  
    <groupId>com.memcpy0.lottery</groupId>  
    <artifactId>lottery-infrastructure</artifactId>  
    <version>1.0-SNAPSHOT</version>  
</dependency>    <dependency>        
<groupId>com.memcpy0.lottery</groupId>  
    <artifactId>lottery-rpc</artifactId>  
    <version>1.0-SNAPSHOT</version>  
</dependency>  
</dependencies>
```
## 四、配置广播模式 Dubbo
首先说说为什么要配置广播模式的 Dubbo，其实**最早 RPC 的设计和使用都是依赖于注册中心**，就是==需要把服务接口信息在程序启动时，推送到一个统一的注册中心，在其他需要调用 RPC 接口的服务上，再通过注册中心的均衡算法来匹配可以连接的接口、落到本地保存和更新==。那么这样的标准使用方式，可以提供更大的连接数和更强的负载均衡作用。

但目前这个以学习实践为目的的工程开发则需要尽可能减少学习成本，也就需要**在开发阶段少一些引入一些额外的配置**，那么目前使用广播模式就非常适合，以后也可以直接把 Dubbo 配置成注册中心模式。官网：[https://dubbo.apache.org](https://dubbo.apache.org/zh/docs/quick-start/)
**配置 yml**
```
# Dubbo 广播方式配置
dubbo:
  application:
    name: Lottery
    version: 1.0.0
  registry:
    address: N/A # multicast://224.5.6.7:1234 
  protocol:
    name: dubbo
    port: 20880
  scan:
    base-packages: cn.itedus.lottery.rpc
```
-   广播模式的配置唯一区别在于注册地址，`registry.address = multicast://224.5.6.7:1234`，**服务提供者和服务调用者都需要配置相同的📢广播地址**。或者配置为 N/A 用于直连模式使用
-   application，配置应用名称和版本
-   protocol，配置的通信协议和端口
-   scan，**相当于 Spring 中自动扫描包的地址，可以把此包下的所有 rpc 接口都注册到服务中**

## 五、定义和开发 RPC 接口
由于 **RPC 接口在通信的过程中，需要提供接口的描述文件，也就是接口的定义信息**。所以这里会看到，**我们会把所有的 RPC 接口定义都放到 `lottery-rpc` 模块下**，这种方式的使用让外部就只依赖这样一个 pom 配置引入的 Jar 包即可。

**定义接口**
```java
public interface IActivityBooth {
    ActivityRes queryActivityById(ActivityReq req);
}
```
-   这里先来定义一个`活动展台`的接口类，用于包装活动的创建、查询、修改、审核的接口。

此外还在rpc包下有三个子包：dto/ActivityDto，res/ActivityRes，req/ActivityReq，都实现了Serializable接口，用于Dubbo通信。

**实现接口**：在interfaces包下实现：
```java
@Service // dubbo的Service
public class ActivityBooth implements IActivityBooth {

    @Resource
    private IActivityDao activityDao;

    @Override
    public ActivityRes queryActivityById(ActivityReq req) {

        Activity activity = activityDao.queryActivityById(req.getActivityId());

        ActivityDto activityDto = new ActivityDto();
        activityDto.setActivityId(activity.getActivityId());
        activityDto.setActivityName(activity.getActivityName());
        activityDto.setActivityDesc(activity.getActivityDesc());
        activityDto.setBeginDateTime(activity.getBeginDateTime());
        activityDto.setEndDateTime(activity.getEndDateTime());
        activityDto.setStockAllTotal(activity.getStockAllTotal());
        activityDto.setStockDayTotal(activity.getStockDayTotal());
        activityDto.setTakeAllCount(activity.getStockAllTotal());
        activityDto.setTakeDayCount(activity.getStockDayTotal());

        return new ActivityRes(new Result(Constants.ResponseCode.SUCCESS.getCode(), Constants.ResponseCode.SUCCESS.getInfo()), activityDto);
    }
}
```
-   用于实现 RPC 接口的实现类 ActivityBooth 上有一个注解 @Service，这个注解是来自于 Dubbo 的 `org.apache.dubbo.config.annotation.Service`，也就是**这个包下含有此注解配置的类可以被 Dubbo 管理**。
-   在 queryActivityById 功能实现中目前还比较粗糙，但大体可以看出**这是对数据库的操作以及对结果的封装**，提供 DTO 的对象并返回 Res 结果。_目前dto的创建，后续可以使用门面模式和工具类进行处理_

调整活动库表，删除活动日库存、参与日库存，在抽奖系统中一般都是即时的，不做过渡设计

**以上就是当前工程对 RPC 接口的一个使用流程的实现，后续会在此基础上添加各个模块的功能。**

---
## 六、搭建测试工程调用 RPC
为了测试 RPC 接口的调用以及后续其他逻辑的验证，这里需要创建一个测试工程：[Lottery-Test](https://gitcode.net/KnowledgePlanet/Lottery-Test) 这个工程中用于引入 RPC 接口的配置和同样广播模式的调用。
### 1. 配置 POM
```xml
<dependency>
    <groupId>cn.itedus.lottery</groupId>
    <artifactId>lottery-rpc</artifactId>
    <version>1.0-SNAPSHOT</version>
</dependency>
```
-   测试之前需要把 Lottery 工程中的 lottery-rpc 进行构建打包，便于测试工程引入
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305231955845.png)

### 2. Consumer端配置广播模式 Dubbo 
```yml
server:
  port: 8081
# Dubbo 广播方式配置
dubbo:
  application:
    name: Lottery
    version: 1.0.0
  registry:
    address: multicast://224.5.6.7:1234 # N/A
  protocol:
    name: dubbo
    port: 20880
```
-   这里的配置与 Dubbo 接口提供者是一样的，都需要在一个广播地址下使用。
### 3. 单元测试类 
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class ApiTest {
    private Logger logger = LoggerFactory.getLogger(ApiTest.class);

    @Reference(interfaceClass = IActivityBooth.class, url = "dubbo://127.0.0.1:20880")
    private IActivityBooth activityBooth;

    @Test
    public void test_rpc() {
        ActivityReq req = new ActivityReq();
        req.setActivityId(100001L);
        ActivityRes result = activityBooth.queryActivityById(req);
        logger.info("测试结果：{}", JSON.toJSONString(result));
    }
}
```
-   如果广播模式不太稳定，**可以在 @Reference 注解的 url 属性指定直连模式**。_后续真正的业务使用**一般都是使用注册中心模式**，此案例中的模式主要是为了更简单的方式学习验证_
-   **IActivityBooth 是 RPC 接口，通过 Dubbo 的注解 `@Reference` 进行注入配置**。有了这个注解的配置，其实调用上就没有太多不同了。

**测试结果 
```
2021-08-08 12:07:34.898  INFO 9474 --- [           main] cn.itedus.lottery.test.SpringRunnerTest           : 测试结果：{"activity":{"activityDesc":"傅哥的活动","activityId":100001,"activityName":"测试活动","beginDateTime":1628061494000,"endDateTime":1628061494000,"stockCount":100,"takeCount":10},"result":{"code":"0000","info":"成功"}}
2021-08-08 12:07:34.915  INFO 9474 --- [extShutdownHook] .b.c.e.AwaitingNonWebApplicationListener :  [Dubbo] Current Spring Boot Application is about to shutdown...
```
-   通过测试结果可以看到，目前通过 RPC 调用已经可以查询到分布式系统提供的数据库操作功能。 

---
## 七、本章知识点 
-   **DDD + RPC 各个分层模块的 POM 配置和依赖关系**
-   Mybatis 的配置和使用
-   **Dubbo 中广播模式的配置**，实际使用的过程中一般都是使用注册中心模式

---
# 八、要想成功
## 修改代码和配置文件，把广播模式改为直连模式，运行正确
服务端要先启动，确保启动时候的日志是成功的。另外如果感觉折腾的费劲，可以先做一个dubbo的案例，按照dubbo官网。
## 1、配置文件修改
### 1.1、provider端配置修改
```
# Dubbo 广播方式配置（把address改为: N/A）
dubbo:
  application:
    name: Lottery
    version: 1.0.0
  registry:
    address: N/A
  protocol:
    name: dubbo
    port: 20880
  scan:
    base-packages: cn.itedus.lottery.rpc
```
### 1.2、consumer端配置
```
# Dubbo 广播方式配置(去掉registry:address: multicast://224.5.6.7:1234)
dubbo:
  application:
    name: Lottery
    version: 1.0.0
  protocol:
    name: dubbo
    port: 20880
```

```java
 @Reference(interfaceClass = IActivityBooth.class,url="dubbo://127.0.0.1:20880")
 private IActivityBooth activityBooth;
```
在测试类中注入远程接口的时候加上调用服务的地址和端口，就是消费者端的dubbo开启的地址和端口。直接连接调用。测试结果正确。 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305232029520.png)

---
## 我的做法
遇到Error creating bean with name 'cn.itedus.lottery.test.ApiTest': Injection of @org.apache.dubbo.config.annotation.Reference dependencies is failed; nested exception is java.lang.ExceptionInInitializerError问题

改用直连模式，且都用JDK 1.8编译，Lottery-Test运行成功：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305232044224.png)

又改用广播模式，好像也能行？所以是编译版本的问题？

> 如果使用3.x版本的dubbo，本地直连测试中，不仅需要跟换dubbo.protocol.port（默认20880）,还需要更换dubbo.application.qos-port（默认22222）,不然会存在端口冲突。

> 嗯 你没仔细看官网文档，我也遇到这个问题后去查官网，广播模式如果生产者和消费者在同一台机器上，消费者需要把默认单播配置进行修改---》unicast=false
```yml
# Dubbo 广播方式配置
dubbo:
  application:
    name: Lottery
    version: 1.0.0
    parameters:
      unicast: false
  registry:
    address: multicast://224.5.6.7:1234
  protocol:
    name: dubbo
    port: 20880
```