-   分支：[210922_xfg_dbRouter](https://gitcode.net/KnowledgePlanet/Lottery/-/tree/210922_xfg_dbRouter)
-   组件：[db-router-spring-boot-starter](https://gitcode.net/KnowledgePlanet/db-router-spring-boot-starter)
-   描述：开发一个基于 HashMap 核心设计原理，使用哈希散列+扰动函数的方式，把数据散列到多个库表中的组件，并验证使用。
- https://www.bantanger.eu.org/pages/1164c2/#%E6%B5%81%E7%A8%8B%E5%88%86%E6%9E%90
- https://wx.zsxq.com/dweb2/index/topic_detail/814514158582822
- 一般像我们做营销活动，有时候需要一个字段分库分表，有时候需要一个字段分库，另外一个字段分表。那么怎么在这个路由组件扩展？

通常分库分表的几个常见方面； 
1. 访问频率：**对于高频访问的数据**，可以将其存储在单独的数据库或表中，以提高读写性能。 
2. 数据大小：**对于大量的数据**，可以将其拆分到多个表中，以减少单表的数据量，降低存储开销。 
3. 数据类型：**对于不同类型的数据**，可以将其拆分到不同的数据库或表中，便于管理和查询。 
4. 数据范围：**对于不同范围的数据**，可以将其拆分到不同的数据库或表中，便于数据的管理和查询。 

分库分表的主要目的在于；**数据分摊、提高QPS/TPS**、分摊压力、**提高可扩展性**。比如；比如数据库的读写性能下降，或者单表数据量过大，这时候您就需要考虑进行分库分表操作了。通过拆分数据库，可以将单个数据库的压力分摊到多个数据库上，从而避免单个数据库的性能瓶颈，提高系统的性能和可扩展性。此外，分库分表还可以解决数据库存储容量的限制，提高数据库的存储能力。 另外在分库分表之后，数据的一致性会受到影响，数据库的管理和维护成本也会增加。因此，在考虑分库分表时，需要仔细权衡利弊，确定是否真的需要进行分库分表操作。也就是你的开发成本问题。因为有分库分表就会相应的引入 canal binlog同步、es、mq、xxl-job等分布式技术栈。
## 一、开发日志
-   9月22日，新增数据库路由组件开发工程 db-router-spring-boot-starter 这是一个自研的分库分表组件。主要用到的技术点包括：散列算法、==数据源切换、AOP切面==、**SpringBoot Starter 开发**等
-   9月22日，完善分库中表信息，user_take_activity、user_take_activity_count、user_strategy_export_001~004，用于测试验证数据库路由组件
-   9月30日，基于Mybatis拦截器，对数据库**路由分表使用方式**进行优化，减少用户在使用过程中需要对数据库语句进行硬编码处理

## 什么时候考虑分库分表？
### 什么时候分表？
如果你的系统处于快速发展时期，如果每天的订单流水都新增几十万，并且，订单表的查询效率明变慢时，就需要规划分库分表了。一般B+树索引高度是2~3层最佳，如果数据量千万级别，可能高度就变4层了，数据量就会明显变慢了。不过业界流传，一般500万数据就要**考虑分表**了。

**当单表存储的数据量非常大的情况下，并且并发量也不高，数据库的连接也还够用**。但是**数据写入和查询的性能出现了瓶颈**，这个时候就需要考虑分表（只分表不分库）了。**将数据拆分到多张表中来减少单表存储的数据量**，从而提升读写的效率。

### 什么时候分库
业务发展很快，还是**多个服务共享一个单体数据库，数据库成为了性能瓶颈，就需要考虑分库了**。比如订单、用户等，都可以抽取出来，新搞个应用（其实就是微服务思想），并且**拆分数据库**（订单库、用户库）。

在什么时候选择分库（只分库不分表）呢？**当数据库读写访问量过高时，有可能会出现数据库连接不够用的问题**。在这种情况下，就需要考虑分库了。**通过增加数据库实例的方式来获取更多的数据库连接，提升整体系统的并发量**

### [#](https://www.bantanger.eu.org/pages/1164c2/#%E6%97%A2%E5%88%86%E5%BA%93%E5%8F%88%E5%88%86%E8%A1%A8)既分库又分表
那什么时候既要分库又要分表呢？结合前面的两种情况，如果同时满足前面的两个条件，也就是**数据连接也不够用**，并且**单表的数据量也很大**，从而导致数据库读写速度变慢的情况，这个时候就要考虑既分库又分表。

## [#](https://www.bantanger.eu.org/pages/1164c2/#%E5%88%86%E5%BA%93%E5%88%86%E8%A1%A8%E4%BC%9A%E5%AF%BC%E8%87%B4%E7%9A%84%E9%97%AE%E9%A2%98)分库分表会导致的问题？

### [#](https://www.bantanger.eu.org/pages/1164c2/#%E4%BA%8B%E5%8A%A1%E9%97%AE%E9%A2%98)事务问题
因为分库分表后，两个相关联的表处于不同的数据库中，本地事务就不能保证原子性，因此**需要使用分布式事务**

### [#](https://www.bantanger.eu.org/pages/1164c2/#%E8%B7%A8%E5%BA%93%E5%85%B3%E8%81%94)跨库关联
跨节点怎么Join？
-   FEDERATED引擎：[数据库分库后跨库join解决方案(opens new window)](https://blog.csdn.net/kisscatforever/article/details/78974576)
-   两次查询

![image-20221116152319505](https://cdn.staticaly.com/gh/BanTanger/image-hosting@master/01.%E6%95%B0%E6%8D%AE%E5%BA%93%E8%B7%AF%E7%94%B1%E9%80%89%E6%8B%A9%E4%B8%AD%E9%97%B4%E4%BB%B6%E6%90%AD%E5%BB%BA-assets202212231151785.webp)

### [#](https://www.bantanger.eu.org/pages/1164c2/#%E6%8E%92%E5%BA%8F%E9%97%AE%E9%A2%98)排序问题
跨节点的count,order by,group by以及聚合函数等问题

可以分别在各个节点上得到结果后在应用程序端进行合并。
### [#](https://www.bantanger.eu.org/pages/1164c2/#%E5%88%86%E9%A1%B5%E9%97%AE%E9%A2%98)分页问题
-   方案1：在个节点查到对应结果后，在代码端汇聚再分页。
-   方案2：把分页交给前端，前端传来pageSize和pageNo，在各个数据库节点都执行分页，然后汇聚总数量前端。这样缺点就是**会造成空查**，如果**分页需要排序，也不好搞**。

### [#](https://www.bantanger.eu.org/pages/1164c2/#%E5%88%86%E5%B8%83%E5%BC%8Fid)分布式ID
**据库被切分后，不能再依赖数据库自身的主键生成机制**

最简单可以考虑UUID，或者使用雪花算法生成分布式ID。

---
## 二、需求分析
`如果要做一个数据库路由，都需要做什么技术点？`

首先我们要知道**为什么要用分库分表**，其实就是==由于业务体量较大，数据增长较快，所以需要把用户数据拆分到不同的库表中去，减轻数据库压力==。

分库分表操作主要有垂直拆分和水平拆分：
-   **垂直拆分**：==指按照业务将表进行分类，分布到不同的数据库上，这样也就将数据的压力分担到不同的库上面==。最终一个数据库由很多表的构成，每个表对应着不同的业务，也就是专库专用。
-   **水平拆分**：**如果垂直拆分后遇到单机瓶颈，可以使用水平拆分**。相对于垂直拆分的区别是：垂直拆分是把不同的表拆到不同的数据库中，而本章节需要实现的水平拆分，是**把同一个表拆到不同的数据库中**。如：user_001、user_002

而本节要实现的也是**水平拆分的路由设计**，如图 1-1
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305262316546.png)

那么，这样的一个数据库路由设计要包括哪些技术知识点呢？
-   是**关于 AOP 切面拦截的使用**，这是==因为需要给**使用数据库路由的方法**做上标记，便于处理分库分表逻辑==。
-   数据源的切换操作，既然有分库，那么就会涉及**在多个数据源间进行链接切换，以便把数据分配给不同的数据库**。
-   数据**库、表寻址**操作，一条数据分配到哪个数据库，哪张表，都需要进行**索引计算**。在方法调用的过程中**最终通过 ThreadLocal 记录**。
-   ==为了能让数据均匀的分配到不同的库表中去，还需要考虑如何进行数据散列的操作==，不能分库分表后，让数据都集中在某个库的某个表，这样就失去了分库分表的意义。

综上，可以看到在数据库和表的数据结构下完成数据存放，我需要用到的技术包括：`AOP`、`数据源切换`、`散列算法`、`哈希寻址`、`ThreadLoca`l以及`SpringBoot的Starter开发方式`等技术。而像`哈希散列`、`寻址`、`数据存放`，其实这样的技术与 HashMap 有太多相似之处，**那么学完源码造火箭的机会来了**。如果你有过深入分析和学习过 HashMap 源码、Spring 源码、中间件开发，那么在设计这样的数据库路由组件时一定会有很多思路的出来。_接下来我们一起尝试下从源码学习到造火箭！_

## 三、技术调研
在 JDK 源码中，包含的数据结构设计有：数组、链表、队列、栈、红黑树，具体的实现有 ArrayList、LinkedList、Queue、Stack，而这些在数据存放都是顺序存储，并没有用到哈希索引的方式进行处理。而 **HashMap、ThreadLocal，两个功能则用了哈希索引、散列算法**以及在数据膨胀时的**拉链寻址和开放寻址**，所以我们要分析和借鉴的也会集中在这两个功能上。

### 1. ThreadLocal
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305262318426.png)
```java
@Test
public void test_idx() {
    int hashCode = 0;
    for (int i = 0; i < 16; i++) {
        hashCode = i * 0x61c88647 + 0x61c88647;
        int idx = hashCode & 15;
        System.out.println("斐波那契散列：" + idx + " 普通散列：" + (String.valueOf(i).hashCode() & 15));
    }
} 

斐波那契散列：7 普通散列：0
斐波那契散列：14 普通散列：1
斐波那契散列：5 普通散列：2
斐波那契散列：12 普通散列：3
斐波那契散列：3 普通散列：4
斐波那契散列：10 普通散列：5
斐波那契散列：1 普通散列：6
斐波那契散列：8 普通散列：7
斐波那契散列：15 普通散列：8
斐波那契散列：6 普通散列：9
斐波那契散列：13 普通散列：15
斐波那契散列：4 普通散列：0
斐波那契散列：11 普通散列：1
斐波那契散列：2 普通散列：2
斐波那契散列：9 普通散列：3
斐波那契散列：0 普通散列：4
```
-   **数据结构**：散列表的数组结构
-   **散列算法**：斐波那契（Fibonacci）散列法
-   **寻址方式**：**Fibonacci 散列法可以让数据更加分散**，在发生数据碰撞时进行开放寻址，从碰撞节点向后寻找位置进行存放元素。公式：`f(k) = ((k * 2654435769) >> X) << Y` 对于常见的32位整数而言，也就是 `f(k) = (k * 2654435769) >> 28` ，黄金分割点：`(√5 - 1) / 2 = 0.6180339887` `1.618:1 == 1:0.618`
-   **学到什么**：可以参考寻址方式和散列算法，但这种数据结构与要设计实现作用到数据库上的结构相差较大，不过 **ThreadLocal 可以用于存放和传递数据索引信息**。

### 2. HashMap
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305262332648.png)
```
public static int disturbHashIdx(String key, int size) {
    return (size - 1) & (key.hashCode() ^ (key.hashCode() >>> 16));
}
```
-   **数据结构**：哈希桶数组 + 链表 + 红黑树
-   **散列算法**：扰动函数、哈希索引，可以让数据更加散列的分布
-   **寻址方式**：通过拉链寻址的方式解决数据碰撞，数据存放时会进行索引地址，遇到碰撞产生数据链表，在一定容量超过8个元素进行扩容或者树化。
-   **学到什么**：==可以把散列算法、寻址方式都运用到数据库路由的设计实现中，还有整个数组+链表的方式其实库+表的方式也有类似之处==。

## 四、设计实现
### 1. 定义路由注解
**定义**
```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE, ElementType.METHOD})
public @interface DBRouter {
    String key() default "";
}
```
**使用**
```java
@Mapper
public interface IUserDao {
     @DBRouter(key = "userId")
     User queryUserInfoByUserId(User req);

     @DBRouter(key = "userId")
     void insertUser(User req);
}
```
-   首先我们需要自定义一个注解，用于放置在**需要被数据库路由的方法上**。
-   它的使用方式是**通过方法配置注解**，就可以**被我们指定的 AOP 切面进行拦截**，==拦截后进行相应的数据库路由计算和判断，并切换到相应的操作数据源上==。

### 2. 解析路由配置
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305262334804.png)
-   以上就是我们实现完数据库路由组件后的一个数据源配置，在分库分表下的数据源使用中，都需要**支持多数据源的信息配置**，这样才能满足**不同需求的扩展**。
-   对于这种自定义较大的信息配置，就需要使用到 `org.springframework.context.EnvironmentAware` 接口，来**获取配置文件**并**提取需要的配置信息**。

**数据源配置提取**
```java
@Override
public void setEnvironment(Environment environment) {
    String prefix = "router.jdbc.datasource.";    

    dbCount = Integer.valueOf(environment.getProperty(prefix + "dbCount"));
    tbCount = Integer.valueOf(environment.getProperty(prefix + "tbCount"));    

    String dataSources = environment.getProperty(prefix + "list");
    for (String dbInfo : dataSources.split(",")) {
        Map<String, Object> dataSourceProps = PropertyUtil.handle(environment, prefix + dbInfo, Map.class);
        dataSourceMap.put(dbInfo, dataSourceProps);
    }
}
```
-   prefix，是**数据源配置的开头信息**，你可以自定义需要的开头内容。
-   dbCount、tbCount、dataSources、dataSourceProps，都是对配置信息的提取，并**存放到 dataSourceMap 中便于后续使用**。

### 3. 数据源切换
**在结合 SpringBoot 开发的 Starter 中，需要提供一个 DataSource 的实例化对象**，那么这个对象我们就**放在 DataSourceAutoConfig 来实现**，并且这里提供的数据源是可以动态变换的，也就是**支持动态切换数据源**。

**创建数据源**
```java
@Bean
public DataSource dataSource() {
    // 创建数据源
    Map<Object, Object> targetDataSources = new HashMap<>();
    for (String dbInfo : dataSourceMap.keySet()) {
        Map<String, Object> objMap = dataSourceMap.get(dbInfo);
        targetDataSources.put(dbInfo, new DriverManagerDataSource(objMap.get("url").toString(), objMap.get("username").toString(), objMap.get("password").toString()));
    }
    // 设置数据源
    DynamicDataSource dynamicDataSource = new DynamicDataSource();
    dynamicDataSource.setTargetDataSources(targetDataSources);
    dynamicDataSource.setDefaultTargetDataSource(new DriverManagerDataSource(defaultDataSourceConfig.get("url").toString(), defaultDataSourceConfig.get("username").toString(), defaultDataSourceConfig.get("password").toString()));

    return dynamicDataSource;
}
```
-   这里是一个简化的创建案例，**把基于从配置信息中读取到的数据源信息，进行实例化创建**。
-   数据源创建完成后存放到 `DynamicDataSource` 中，它是一个继承了 AbstractRoutingDataSource 的实现类，这个类里可以存放和读取**相应的具体调用的数据源信息**。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305270020370.png)
讲讲流程分析（参考图一）
1. 首先通过SPI机制找到DataSourceAutoConfig 这个Config类是整个中间件的核心。它继承EnvironmentAware接口，这个接口适用于自定义配置很多的情况 
2. 我们自定义配置的选项，设置在DBRouterConfig上，需要注意的是 **数据源的配置不需要我们设置，spring内置了数据源配置属性，能通过 EnvironmentAware 找到** 
3. 在DataSourceAutoConfig 中，通过setEnvironment方法将自定义配置yaml文件读入，并将这些数据源先存放在本地缓存（Map）里 
4. DataSource（Spring内置）通过读取本地缓存的数据源，进而创建数据源。 
5. 注入Bean，将DynamicMybatisPlugin，DBRouterJoinPoint，DBRouterConfig, DBRouterStrategy注入，以及编程式事务TransactionTemplate 保证事务的原子性。
6. **切点注册后，通过寻找切点定位到具体的方法上**，首先获取需要进行分库分表逻辑的路由值 routerKey，然后**这个路由值需要执行路由计算策略 doRouter来确定这个值具体位置**（存放 和 查找） 
7. 路由计算策略位于 DBRouterStrategyHashCode 它通过拟HashMap的路由计算方式计算出当前数据应当往哪个数据源进行存储。然后存储的策略需要放在ThreadLocal中进行数据源上下文的保存，上下文可用在分库分表具体数据源切换的逻辑中，并且使用ThreadLocal一方面避免了多线程产生的冲突，另一方面ThreadLocal内部使用的斐波那契散列也保证了数据存储不容易发生散列冲突。 
8. 分库DynamicDataSource：通过数据源上下文 DBContextHolder 获取到具体的分库数据 DBKey ，将其拼接到 DB 上，分库实现了Spring内置的接口AbstractRoutingDataSource，内置了方法determineCurrentLookupKey 
9. 分表DynamicMybatisPlugin：通过 @Intercepts 将 Mybatis的语句在执行前拦截。反射机制获取到需要进行分表的方法（使用注解@DBStrategy），正则提取出From后的表名。通过上下文获取到路由后的表位置，并将其拼接替换掉原表名。因为sql是private属性，需要通过反射机制对权限进行修改。这整个过程通过编程式事务实现了事务的原子性，如果执行失败，这条语句将不会执行。
### 4. 切面拦截
在 AOP 的切面拦截中需要完成；**数据库路由计算**、扰动函数加强散列、计算库**表索引**、设置到 ThreadLocal 传递数据源，整体案例代码如下：
```java
@Around("aopPoint() && @annotation(dbRouter)")
public Object doRouter(ProceedingJoinPoint jp, DBRouter dbRouter) throws Throwable {
    String dbKey = dbRouter.key();
    if (StringUtils.isBlank(dbKey)) throw new RuntimeException("annotation DBRouter key is null！");

    // 计算路由
    String dbKeyAttr = getAttrValue(dbKey, jp.getArgs());
    int size = dbRouterConfig.getDbCount() * dbRouterConfig.getTbCount(); // 库数量*表数量

    // 扰动函数
    int idx = (size - 1) & (dbKeyAttr.hashCode() ^ (dbKeyAttr.hashCode() >>> 16));

    // 库表索引
    int dbIdx = idx / dbRouterConfig.getTbCount() + 1; // 从库1开始
    int tbIdx = idx - dbRouterConfig.getTbCount() * (dbIdx - 1);   

    // 设置到 ThreadLocal
    DBContextHolder.setDBKey(String.format("%02d", dbIdx));
    DBContextHolder.setTBKey(String.format("%02d", tbIdx));
    logger.info("数据库路由 method：{} dbIdx：{} tbIdx：{}", getMethod(jp).getName(), dbIdx, tbIdx);
   
    // 返回结果
    try {
        return jp.proceed();
    } finally {
        DBContextHolder.clearDBKey();
        DBContextHolder.clearTBKey();
    }
}
```
-   简化的核心逻辑实现代码如上，**首先我们提取了库表乘积的数量，把它当成 HashMap 一样的长度进行使用**。
-   接下来使用和 HashMap 一样的扰动函数逻辑，**让数据分散的更加散列**。
-   当计算完总长度上的一个索引位置后，还需要把这个位置折算到库表中，看看**总体长度的索引因为落到哪个库哪个表**。
-   最后是把这个计算的索引信息存放到 ThreadLocal 中，用于传递**在方法调用过程中可以提取到索引信息**。

### 5. Mybatis 拦截器处理分表
-   最开始考虑直接在Mybatis对应的表 `INSERT INTO user_strategy_export`**_${tbIdx}** 添加字段的方式处理分表。**但这样看上去并不优雅**，不过也并不排除这种使用方式，仍然是可以使用的。
-   那么我们可以**基于 Mybatis 拦截器** Interceptor 进行处理，通过==拦截 SQL 语句动态修改添加分表信息，再设置回 Mybatis 执行 SQL 中==。
-   此外再完善一些分库分表路由的操作，比如**配置默认的分库分表字段**以及**单字段入参时默认取此字段作为路由字段**。

HandlerInterceptor是Spring的拦截器。
**cn.bugstack.middleware.db.router.dynamic.DynamicMybatisPlugin**
```java
@Intercepts({@Signature(type = StatementHandler.class, method = "prepare", args = {Connection.class, Integer.class})})
public class DynamicMybatisPlugin implements Interceptor {
    private Pattern pattern = Pattern.compile("(from|into|update)[\\s]{1,}(\\w{1,})", Pattern.CASE_INSENSITIVE);

    @Override
    public Object intercept(Invocation invocation) throws Throwable {
        // 获取StatementHandler
        StatementHandler statementHandler = (StatementHandler) invocation.getTarget();
        MetaObject metaObject = MetaObject.forObject(statementHandler, SystemMetaObject.DEFAULT_OBJECT_FACTORY, SystemMetaObject.DEFAULT_OBJECT_WRAPPER_FACTORY, new DefaultReflectorFactory());
        MappedStatement mappedStatement = (MappedStatement) metaObject.getValue("delegate.mappedStatement");

        // 获取自定义注解判断是否进行分表操作
        String id = mappedStatement.getId();
        String className = id.substring(0, id.lastIndexOf("."));
        Class<?> clazz = Class.forName(className);
        DBRouterStrategy dbRouterStrategy = clazz.getAnnotation(DBRouterStrategy.class);
        if (null == dbRouterStrategy || !dbRouterStrategy.splitTable()) {
            return invocation.proceed();
        }

        // 获取SQL
        BoundSql boundSql = statementHandler.getBoundSql();
        String sql = boundSql.getSql();

        // 替换SQL表名 USER 为 USER_03
        Matcher matcher = pattern.matcher(sql);
        String tableName = null;
        if (matcher.find()) {
            tableName = matcher.group().trim();
        }
        assert null != tableName;
        String replaceSql = matcher.replaceAll(tableName + "_" + DBContextHolder.getTBKey());

        // 通过反射修改SQL语句
        Field field = boundSql.getClass().getDeclaredField("sql");
        field.setAccessible(true);
        field.set(boundSql, replaceSql);

        return invocation.proceed();
    }
}
```
-   实现 Interceptor 接口的 intercept 方法，获取StatementHandler、**通过自定义注解判断是否进行分表操作**、获取SQL并替换SQL表名 USER 为 USER_03、最后**通过反射修改SQL语句**
-   此处会用到正则表达式拦截出匹配的sql，`(from|into|update)[\\s]{1,}(\\w{1,})`

## 五、测试验证(分库)
-   打包 db-router-spring-boot-starter
-   引入 pom 文件
```xml
<dependency>
    <groupId>cn.bugstack.middleware</groupId>
    <artifactId>db-router-spring-boot-starter</artifactId>
    <version>1.0.0-SNAPSHOT</version>
</dependency>
```
### 1. 在需要使用数据库路由的DAO方法上加入注解
**cn.itedus.lottery.infrastructure.dao.IUserTakeActivityDao**
```java
@Mapper
public interface IUserTakeActivityDao {

    /**
     * 插入用户领取活动信息
     *
     * @param userTakeActivity 入参
     */
    @DBRouter(key = "uId")
    void insert(UserTakeActivity userTakeActivity);

}
```

-   @DBRouter(key = "uId") key 是入参对象中的属性，用于提取作为分库分表路由字段使用

### 2. 数据库语句
```xml
<insert id="insert" parameterType="cn.itedus.lottery.infrastructure.po.UserTakeActivity">
    INSERT INTO user_take_activity
    (u_id, take_id, activity_id, activity_name, take_date,
     take_count, uuid, create_time, update_time)
    VALUES
    (#{uId}, #{takeId}, #{activityId}, #{activityName}, #{takeDate},
     #{takeCount}, #{uuid}, now(), now())
</insert>
```
-   如果一个表**只分库不分表**，则它的 sql 语句并不会有什么差异
-   如果需要分表，那么则需要在表名后面加入 user_take_activity_${tbIdx} 同时入参对象需要继承 DBRouterBase 这样才可以拿到 tbIdx 分表信息 `这部分内容我们在后续开发中会有体现`

### 3. 单元测试
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserTakeActivityDaoTest {

    private Logger logger = LoggerFactory.getLogger(ActivityDaoTest.class);

    @Resource
    private IUserTakeActivityDao userTakeActivityDao;

    @Test
    public void test_insert() {
        UserTakeActivity userTakeActivity = new UserTakeActivity();
        userTakeActivity.setuId("Uhdgkw766120d"); // 1库：Ukdli109op89oi 2库：Ukdli109op811d
        userTakeActivity.setTakeId(121019889410L);
        userTakeActivity.setActivityId(100001L);
        userTakeActivity.setActivityName("测试活动");
        userTakeActivity.setTakeDate(new Date());
        userTakeActivity.setTakeCount(10);
        userTakeActivity.setUuid("Uhdgkw766120d");

        userTakeActivityDao.insert(userTakeActivity);
    }
}
```
-   测试中分别验证了不同的 uId 主要是为了解决数据散列到不同库表中去。
## 六、测试验证(分表)
### 1. 配置分表注解
```java
@Mapper
@DBRouterStrategy(splitTable = true)
public interface IUserStrategyExportDao {

    /**
     * 新增数据
     * @param userStrategyExport 用户策略
     */
    @DBRouter(key = "uId")
    void insert(UserStrategyExport userStrategyExport);

    /**
     * 查询数据
     * @param uId 用户ID
     * @return 用户策略
     */
    @DBRouter
    UserStrategyExport queryUserStrategyExportByUId(String uId);
}
```
-   @DBRouterStrategy(splitTable = true) 配置分表信息，**配置后会通过数据库路由组件把sql语句添加上分表字段**，比如表 user 修改为 user_003
-   @DBRouter(key = "uId") 设置路由字段
-   @DBRouter 未配置情况下走默认字段，routerKey: uId

这里好像需要添加一个：才能测试通过。
```java
/**  
 * 查询数据  
 * @param uId 用户ID  
 * @return 用户策略  
 */  
@DBRouter(key = "uId")  
UserStrategyExport queryUserStrategyExportByUId(String uId);
```
### 2. 数据库语句
```xml
<insert id="insert" parameterType="cn.itedus.lottery.infrastructure.po.UserStrategyExport">
    INSERT INTO user_strategy_export
    (u_id, activity_id, order_id, strategy_id, strategy_mode,
     grant_type, grant_date, grant_state, award_id, award_type,
     award_name, award_content, uuid, create_time, update_time)
    VALUES
    (#{uId},#{activityId},#{orderId},#{strategyId},#{strategyMode},
     #{grantType},#{grantDate},#{grantState},#{awardId},#{awardType},
     #{awardName},#{awardContent},#{uuid},now(),now())
</insert>
<select id="queryUserStrategyExportByUId" parameterType="java.lang.String" resultMap="userStrategyExportMap">
    SELECT id, u_id, activity_id, order_id, strategy_id, strategy_mode,
           grant_type, grant_date, grant_state, award_id, award_type,
           award_name, award_content, uuid, create_time, update_time
    FROM user_strategy_export
    WHERE u_id = #{uId}
</select>
```
-   正常写 SQL 语句即可，如果你不使用注解 @DBRouterStrategy(splitTable = true) 也可以使用 user_strategy_export`_003`
### 3. 单元测试
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserStrategyExportDaoTest {

    private Logger logger = LoggerFactory.getLogger(UserStrategyExportDaoTest.class);

    @Resource
    private IUserStrategyExportDao userStrategyExportDao;

    @Resource
    private Map<Constants.Ids, IIdGenerator> idGeneratorMap;

    @Test
    public void test_insert() {
        UserStrategyExport userStrategyExport = new UserStrategyExport();
        userStrategyExport.setuId("Uhdgkw766120d");
        userStrategyExport.setActivityId(idGeneratorMap.get(Constants.Ids.ShortCode).nextId());
        userStrategyExport.setOrderId(idGeneratorMap.get(Constants.Ids.SnowFlake).nextId());
        userStrategyExport.setStrategyId(idGeneratorMap.get(Constants.Ids.RandomNumeric).nextId());
        userStrategyExport.setStrategyMode(Constants.StrategyMode.SINGLE.getCode());
        userStrategyExport.setGrantType(1);
        userStrategyExport.setGrantDate(new Date());
        userStrategyExport.setGrantState(1);
        userStrategyExport.setAwardId("1");
        userStrategyExport.setAwardType(Constants.AwardType.DESC.getCode());
        userStrategyExport.setAwardName("IMac");
        userStrategyExport.setAwardContent("奖品描述");
        userStrategyExport.setUuid(String.valueOf(userStrategyExport.getOrderId()));

        userStrategyExportDao.insert(userStrategyExport);
    }

    @Test
    public void test_select() {
        UserStrategyExport userStrategyExport = userStrategyExportDao.queryUserStrategyExportByUId("Uhdgkw766120d");
        logger.info("测试结果：{}", JSON.toJSONString(userStrategyExport));
    }

}
```

```
21:05:52.050  INFO 86968 --- [           main] c.i.l.t.dao.UserStrategyExportDaoTest    : Started UserStrategyExportDaoTest in 6.103 seconds (JVM running for 7.51)
21:05:52.292  INFO 86968 --- [           main] c.b.m.db.router.DBRouterJoinPoint        : 数据库路由 method：queryUserStrategyExportByUId dbIdx：1 tbIdx：3
21:05:52.800  INFO 86968 --- [           main] c.i.l.t.dao.UserStrategyExportDaoTest    : 测试结果：{"activityId":120405215,"awardContent":"test","awardId":"1","awardName":"IMac","awardType":1,"createTime":1633006252000,"grantDate":1633006252000,"grantState":1,"grantType":1,"id":1,"orderId":1443558966104850432,"strategyId":42480428672,"strategyMode":1,"uId":"Uhdgkw766120d","updateTime":1633006252000,"uuid":"1443558966104850432"}
```

-   分别测试插入数据和查询数据，已经可以正常使用分库分表了

---

1.  学习SpringBoot组件开发，了解一个简单的数据库路由实现原理
2.  使用 Mybatis 拦截器处理分表 SQL
3.  学习使用数据库路由

在课程的学习中,我发现UserTakeActivityDaoTest的34行,uid=Uhdgkw766120d,也就是分库分表设计中,代表库ID和表ID的uid是如何生成的逻辑

打卡: 总结了一下路由组件 如何创建自己的路由分库分表组件? 
一.编写注解类 需要明确用户是否需要分库分表?分库分表的字段是什么? 所以定义的注解类中需声明分库分表字段. @Documented //可将java doc注释展示 @Retention(RetentionPolicy.RUNTIME) // 标注接口作用范围(运行时.编译时等) @Target({ElementType.TYPE, ElementType.METHOD}) //该接口可在类、接口、方法上声明 public @interface DBRouter { ​    /** 分库分表字段 */    String key() default ""; ​ } 明确用户是否需要分库分表,所以需要提供分表注解 public @interface DBRouterStrategy { /** * 标记是否需要分表,默认不分表 */    boolean splitTable() default false; ​ } 二.提供Spring Boot自动配置(获取配置信息) 1.在spring.factories文件中开启Spring Boot 自动配置 org.springframework.boot.autoconfigure.EnableAutoConfiguration=.....middleware.db.router.config.DataSourceAutoConfig 2.DataSourceAutoConfig 用户需要对数据源配置(数据源又分为默认数据源,和需要分库数据源) 有多少数据库 有多少表 路由字段 1.设置切入点 2.配置拦截器为mybatis拦截器 3.利用SpringBoot提供的determineCurrentLookupKey()动态数据库切换拓展方法实现数据库切换 4.配置路由执行算法 5.提供对事务管理 transactionTemplate 6.从配置文件中获取数据库数量,表数量,路由Key,需要分库分表的数据源 三.编写环绕通知(在执行sql进行分库) 1.将DBRouter作为切入点,进行环绕通知 2.dbRouter.key()确定根据哪个字段进行路由 getAttrValue根据数据库路由字段，从入参中读取出对应的值。比如路由key 是uId，那么就从入参对象 Obj 中获取到 uId 的值。 dbRouterStrategy.doRouter(dbKeyAttr)路由策略根据具体的路由值进行处理4.路由处理完成比，就是放行jp.proceed() 最后 dbRouterStrategy 需要执行 clear 因为这里用到了 ThreadLocal 需要手动清空。

编写mybatis拦截器(执行sql进行分表) 1.设置拦截器拦截sql语句预处理 @Intercepts({@Signature(type = StatementHandler.class, method = "prepare", args = {Connection.class, Integer.class})}) 2.拦截from、into、update这种后面跟表名的 private Pattern pattern = Pattern.compile("(from|into|update)[\\s]{1,}(\\w{1,})", Pattern.CASE_INSENSITIVE); 3.通过mybatis的反射工具类—MetaObject（反射对象类）获取sql object待处理的原始对象业务对象 objectFactory 创建对象的工厂 new DefaultObjectFactory() objectWrapperFactory object是否是wrapper new DefaultObjectWrapperFactory() reflectorFactory 反射的工厂类 new DefaultReflectorFactory() mapperStatement中包含sql中的所有参数 通过mapperStatement获取自定义注解,查看用户是否需要分库分表 5.通过反射获取sql,将sql中的表名替换

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305271009596.png)
