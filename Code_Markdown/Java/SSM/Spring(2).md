# 9. Spring整合
学习到这里，已经对Spring有一个简单的认识了，**Spring有一个容器，叫做IoC容器，里面保存 `bean`** 。在进行企业级开发时，其实**除了将自己写的类让Spring管理之外**，还有一部分重要的工作就是使用第三方的技术。前面已经讲了如何**管理第三方 `bean` 了**。下面结合IoC和DI，整合2个常用技术，进一步加深对Spring的使用理解。
## 9.1 Spring整合Mybatis思路分析
### 9.1.1 环境准备
在准备环境的过程中，我们也来回顾下Mybatis开发的相关内容。

步骤1：准备数据库表。Mybatis是来操作数据库表，所以先创建一个数据库及表
```sql
create database spring_db character set utf8;
use spring_db;
create table tbl_account(
    id int primary key auto_increment,
    name varchar(35),
    money double
);
```
步骤2：创建项目导入JAR包。项目的 `pom.xml` 添加相关依赖：
```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>5.2.10.RELEASE</version>
    </dependency>
    <dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>druid</artifactId>
        <version>1.1.16</version>
    </dependency>
    <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis</artifactId>
        <version>3.5.6</version>
    </dependency>
    <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <version>5.1.47</version>
    </dependency>
</dependencies>
```
步骤3：根据表创建模型类（`domain` 包中；或者是 `pojo` 包）。
```java
public class Account implements Serializable {

    private Integer id;
    private String name;
    private Double money;
	//setter...getter...toString...方法略    
}
```
步骤4：创建 `Dao` 接口，注解形式配置映射关系，没有映射配置文件；没有 `mapper` 文件夹（注解开发，就不要 `mapper` 映射文件了）；下面的接口没有实现类，说明是自动代理完成的，就是用 `mapper` 代理完成的。
```java
public interface AccountDao {
    @Insert("insert into tbl_account(name,money)values(#{name},#{money})")
    void save(Account account);

    @Delete("delete from tbl_account where id = #{id} ")
    void delete(Integer id);

    @Update("update tbl_account set name = #{name} , money = #{money} where id = #{id} ")
    void update(Account account);

    @Select("select * from tbl_account")
    List<Account> findAll();

    @Select("select * from tbl_account where id = #{id} ")
    Account findById(Integer id);
}
```
步骤5：创建 `Service` 接口和实现类。
```java
public interface AccountService {
    void save(Account account);
    void delete(Integer id);
    void update(Account account);
    List<Account> findAll();
    Account findById(Integer id);
}
@Service
public class AccountServiceImpl implements AccountService {
    @Autowired
    private AccountDao accountDao;

    public void save(Account account) {
        accountDao.save(account);
    }

    public void update(Account account){
        accountDao.update(account);
    }

    public void delete(Integer id) {
        accountDao.delete(id);
    }

    public Account findById(Integer id) {
        return accountDao.findById(id);
    }

    public List<Account> findAll() {
        return accountDao.findAll();
    }
}
```
步骤6：添加 `jdbc.properties` 文件。`resources` 目录下添加，用于配置数据库连接四要素。
```java
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost:3306/spring_db?useSSL=false
jdbc.username=root
jdbc.password=root
```
`useSSL` ：关闭MySQL的SSL连接

步骤7：添加Mybatis核心配置文件。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration
        PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <!--读取外部properties配置文件-->
    <properties resource="jdbc.properties"></properties>
    <!--别名扫描的包路径-->
    <typeAliases>
        <package name="com.itheima.domain"/>
    </typeAliases>
    <!--数据源-->
    <environments default="mysql">
        <environment id="mysql">
            <transactionManager type="JDBC"></transactionManager>
            <dataSource type="POOLED">
                <property name="driver" value="${jdbc.driver}"></property>
                <property name="url" value="${jdbc.url}"></property>
                <property name="username" value="${jdbc.username}"></property>
                <property name="password" value="${jdbc.password}"></property>
            </dataSource>
        </environment>
    </environments>
    <!--映射文件扫描包路径-->
    <mappers>
        <package name="com.itheima.dao"></package>
    </mappers>
</configuration>
```
步骤8：编写应用程序。
```java
public class App {
    public static void main(String[] args) throws IOException {
        // 1. 创建SqlSessionFactoryBuilder对象
        SqlSessionFactoryBuilder sqlSessionFactoryBuilder = new SqlSessionFactoryBuilder();
        // 2. 加载SqlMapConfig.xml配置文件
        InputStream inputStream = Resources.getResourceAsStream("SqlMapConfig.xml.bak");
        // 3. 创建SqlSessionFactory对象
        SqlSessionFactory sqlSessionFactory = sqlSessionFactoryBuilder.build(inputStream);
        // 4. 获取SqlSession
        SqlSession sqlSession = sqlSessionFactory.openSession();
        // 5. 执行SqlSession对象执行查询，获取结果User
        AccountDao accountDao = sqlSession.getMapper(AccountDao.class);

        Account ac = accountDao.findById(1);
        System.out.println(ac);
        // 6. 释放资源
        sqlSession.close();
    }
}
```
步骤9：运行程序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302192318467.png)

### 9.1.2 整合思路分析
Mybatis的基础环境我们已经准备好了，接下来就得分析下在上述的内容中，哪些对象可以交给Spring来管理？
* Mybatis程序核心对象分析
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302192320818.png)
  从图中可以获取到，真正需要交给Spring管理的是 `SqlSessionFactory` 。`SqlSession` 有点像连接池，已经有了、直接获得，而非自己创建。
* 整合Mybatis，就是将Mybatis用到的内容交给Spring管理，分析下配置文件
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302192322476.png)
  **说明**
	* 第一行读取外部 `properties` 配置文件，Spring有提供具体的解决方案 `@PropertySource` ，需要交给Spring
	* 第二行起别名包扫描，为 `SqlSessionFactory` 服务的，需要交给Spring
	* **第三行主要用于做连接池**，**Spring之前我们已经整合了 `Druid` 连接池，这块也需要交给Spring**
	* 前面三行一起都是为了创建 `SqlSession` 对象用的，那么用Spring管理 `SqlSession` 对象吗？回忆下 `SqlSession` 是由 `SqlSessionFactory` 创建出来的，所以只需要将 `SqlSessionFactory` 交给Spring管理即可。
	* 第四行是 `Mapper` 接口和映射文件[如果使用注解就没有该映射文件]，这个是**在获取到 `SqlSession` 以后、执行具体操作时用**，所以它和 `SqlSessionFactory` 创建的时机都不在同一个时间，可能需要单独管理。

### 6.2 Spring整合Mybatis
前面我们已经分析了Spring与Mybatis的整合，大体需要做两件事，第一件事是：Spring要管理MyBatis中的 `SqlSessionFactory` ，第二件事是Spring要管理 `Mapper` 接口的扫描。具体该如何实现，具体的步骤如下。

步骤1：项目中导入整合需要的JAR包。
```xml
<dependency>
    <!--Spring操作数据库都需要该jar包-->
    <groupId>org.springframework</groupId>
    <artifactId>spring-jdbc</artifactId>
    <version>5.2.10.RELEASE</version>
</dependency>
<dependency>
    <!--
		Spring整合Mybatis要用的jar包
		这个jar包mybatis在前面,是Mybatis提供的(被整合的做)
	-->
    <groupId>org.mybatis</groupId>
    <artifactId>mybatis-spring</artifactId>
    <version>1.3.0</version>
</dependency>
```
步骤2：创建Spring的主配置类。
```java
//配置类注解
@Configuration
//包扫描,主要扫描的是项目中的AccountServiceImpl类
@ComponentScan("com.itheima")
public class SpringConfig {
}
```
步骤3：**创建数据源的配置类**，在配置类中完成数据源的创建。
```java
public class JdbcConfig {
    @Value("${jdbc.driver}")
    private String driver;
    @Value("${jdbc.url}")
    private String url;
    @Value("${jdbc.username}")
    private String userName;
    @Value("${jdbc.password}")
    private String password;

    @Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName(driver);
        ds.setUrl(url);
        ds.setUsername(userName);
        ds.setPassword(password);
        return ds;
    }
}
```
步骤4：**主配置类中读 `properties` 并引入数据源配置类**。
```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("classpath:jdbc.properties")
@Import(JdbcConfig.class)
public class SpringConfig {
}

```
步骤5：创建Mybatis配置类并配置 `SqlSessionFactory` ：
```java
public class MybatisConfig {
    //定义bean,SqlSessionFactoryBean用于产生SqlSessionFactory对象
    @Bean
    public SqlSessionFactoryBean sqlSessionFactory(DataSource dataSource){
        SqlSessionFactoryBean ssfb = new SqlSessionFactoryBean();
        //设置模型类的别名扫描
        ssfb.setTypeAliasesPackage("com.itheima.domain");
        //设置数据源
        ssfb.setDataSource(dataSource);
        return ssfb;
    }
    //定义bean,返回MapperScannerConfigurer对象
    @Bean
    public MapperScannerConfigurer mapperScannerConfigurer(){
        MapperScannerConfigurer msc = new MapperScannerConfigurer();
        msc.setBasePackage("com.itheima.dao");
        return msc;
    }
}
```
**说明**
* 使用 `SqlSessionFactoryBean` 封装 `SqlSessionFactory` 需要的环境信息
	* `SqlSessionFactoryBean` 是前面我们讲解 `FactoryBean` 的一个子类，在该类中将 `SqlSessionFactory` 的创建进行了封装，简化对象的创建，我们只需要将其需要的内容设置即可。
	* 方法中有一个参数为 `dataSource` ，当前Spring容器中已经创建了 `Druid` 数据源，类型刚好是 `DataSource` 类型，**此时在初始化 `SqlSessionFactoryBean` 这个对象时，发现需要使用 `DataSource` 对象，而容器中刚好有这么一个对象，就自动加载了 `DruidDataSource` 对象**。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200012535.png)
* 使用 `MapperScannerConfigurer` 加载 `Dao` 接口，创建代理对象保存到IoC容器中
	* **这个 `MapperScannerConfigurer` 对象也是MyBatis提供的、专用于整合的JAR包中的类**，用来处理原始配置文件中的 `mappers` 相关配置，加载数据层的 `Mapper` 接口类
	* `MapperScannerConfigurer` 有一个核心属性 `basePackage` ，就是**用来设置所扫描的包路径**
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200012072.png)

步骤6：主配置类中引入Mybatis配置类。
```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("classpath:jdbc.properties")
@Import({JdbcConfig.class, MybatisConfig.class})
public class SpringConfig {
}
```

步骤7：编写运行类。在运行类中，从IoC容器中获取 `Service` 对象，调用方法获取结果（这里不用获取DAO对象，因为DAO已经注入到 `Service` 对象中——在 `ServiceImpl` 里通过注解获得了 `dao` 的对象，所以可以直接调用 `dao` 对象里的SQL方法，然后在 `app2` 里也就可以通过 `ServiceImpl` 的方法获取SQL执行结果）。
```java
public class App2 {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);

        AccountService accountService = ctx.getBean(AccountService.class);

        Account ac = accountService.findById(1);
        System.out.println(ac);
    }
}

```
步骤8：运行程序。 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200013136.png)

支持Spring与Mybatis的整合就已经完成了，其中主要用到的两个类分别是：
* ==SqlSessionFactoryBean==
* ==MapperScannerConfigurer==

## 9.3 Spring整Junit
整合Junit与整合Druid和MyBatis差异比较大，为什么呢？Junit是一个搞单元测试用的工具，它不是我们程序的主体，也不会参加最终程序的运行，从作用上来说就和之前的东西不一样，它不是做功能的，看做是一个辅助工具就可以了。
### 9.3.1 环境准备
这块环境，大家可以直接使用Spring与Mybatis整合的环境即可。当然也可以重新创建一个，因为内容是一模一样，所以我们直接来看下项目结构即可：
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200026033.png)

### 9.3.2 整合Junit步骤
在上述环境的基础上，我们来对Junit进行整合。步骤1：引入依赖，Junit版本要在4.12以上。
```xml
<dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <version>4.12</version>
    <scope>test</scope>
</dependency>

<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-test</artifactId>
    <version>5.2.10.RELEASE</version>
</dependency>
```
步骤2：编写测试类。在 `test\java` 下创建一个 `AccountServiceTest` ，这个名字任意（Junit就相当于不用走 `main` 方法直接执行）。
```java
//设置类运行器
@RunWith(SpringJUnit4ClassRunner.class)
//设置Spring环境对应的配置类
@ContextConfiguration(classes = {SpringConfig.class}) //加载配置类
//@ContextConfiguration(locations={"classpath:applicationContext.xml"})//加载配置文件
public class AccountServiceTest {
    //支持自动装配注入bean
    @Autowired
    private AccountService accountService;
    @Test
    public void testFindById() {
        System.out.println(accountService.findById(1));
    }
    @Test
    public void testFindAll() {
        System.out.println(accountService.findAll());
    }
}
```
**注意**：
* **单元测试，如果测试的是注解配置类，则使用 `@ContextConfiguration(classes = 配置类.class)`**
* 单元测试，如果测试的是配置文件，则使用 `@ContextConfiguration(locations={配置文件名,...})`
* ==Junit运行后是基于Spring环境运行的，所以Spring提供了一个专用的类运行器，这个务必要设置==，这个类运行器是在Spring的测试专用包中提供的，导入的坐标就是这个东西 `SpringJUnit4ClassRunner`
* 上面两个配置都是固定格式，==当需要测试哪个 `bean` 时，使用自动装配加载对应的对象，下面的工作就和以前做Junit单元测试完全一样了==

#### 知识点1：`@RunWith`

| 名称 | `@RunWith`                          |
| ---- | --------------------------------- |
| 类型 | 测试类注解                        |
| 位置 | 测试类定义上方                    |
| 作用 | 设置JUnit运行器                   |
| 属性 | value（默认）: 运行所使用的运行期 |

#### 知识点2：`@ContextConfiguration`

| 名称 | `@ContextConfiguration`                                        |
| ---- | ------------------------------------------------------------ |
| 类型 | 测试类注解                                                   |
| 位置 | 测试类定义上方                                               |
| 作用 | 设置JUnit加载的Spring核心配置                                |
| 属性 | classes: 核心配置类，可以使用数组的格式设定加载多个配置类<br/> locations: 配置文件，可以使用数组的格式设定加载多个配置文件名称 |

---
> * 理解并掌握AOP相关概念
> * 能够说出**AOP的工作流程**
> * 能**运用AOP相关知识**完成对应的案例编写
> * 重点掌握**Spring的声明式事务管理**

# 10. AOP简介
前面在介绍Spring时说过，Spring有两个核心的概念，一个是 `IoC/DI` ，一个是 `AOP` 。前面已经对 `IoC/DI` 进行了系统的学习，接下来要学习它的另一个核心内容，就是==AOP==。对于AOP，我们前面提过一句话是：==AOP是在不改原有代码的前提下对其进行增强==。下面的内容，主要就是围绕着这一句话展开学习，主要学习两方面内容 `AOP核心概念` 和 `AOP作用` 。

## 10.1 什么是AOP？
面向切面编程 *AOP, Aspect Oriented Programming* ，一种编程范式，指导开发者如何组织程序结构。

我们都知道面向对象编程 *OOP, Object Oriented Programming* 是一种编程思想，那么AOP也是一种编程思想，==编程思想主要的内容，就是指导程序员该如何编写程序，所以它们两个是不同的编程范式==。

## 10.2 AOP作用
作用：**在不惊动原始设计的基础上为其进行功能增强**，前面咱们有技术就可以实现这样的功能即代理模式。

## 10.3 AOP核心概念
为了能更好的理解AOP的相关概念，准备了一个环境，整个环境的内容暂时不用关注，最主要的类为 `BookDaoImpl` 。
```java
@Repository
public class BookDaoImpl implements BookDao {
    public void save() {
        //记录程序当前执行执行（开始时间）
        Long startTime = System.currentTimeMillis();
        //业务执行万次
        for (int i = 0; i<10000; i++) {
            System.out.println("book dao save ...");
        }
        //记录程序当前执行时间（结束时间）
        Long endTime = System.currentTimeMillis();
        //计算时间差
        Long totalTime = endTime - startTime;
        //输出信息
        System.out.println("执行万次消耗时间：" + totalTime + "ms");
    }
    public void update(){
        System.out.println("book dao update ...");
    }
    public void delete(){
        System.out.println("book dao delete ...");
    }
    public void select(){
        System.out.println("book dao select ...");
    }
}
```
代码的内容相信大家都能够读懂，`save` 方法中有计算万次执行消耗的时间。当在 `App` 类中从容器中获取 `bookDao` 对象后，分别执行其 `save, delete, update` 和 `select` 方法后会有如下的打印结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200039165.png)
这个时候，我们就应该有些疑问？
* 对于计算万次执行消耗的时间只有 `save` 方法有，为什么 `delete` 和 `update` 方法也会有呢？
* `delete` 和 `update` 方法有，那什么 `select` 方法为什么又没有呢？

这个案例中其实就使用了Spring的AOP，**在不惊动(改动)原有设计(代码)的前提下，想给谁添加功能就给谁添加**。这个也就是Spring的理念：**无入侵式/无侵入式**。

说了这么多，Spring到底是如何实现的呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200117598.png)
1. 前面一直在强调，Spring的AOP是「对一个类的方法，在不进行任何修改的前提下实现增强」。上面案例中 `BookServiceImpl` 中有 `save, update, delete, select` 方法，这些方法我们给起了一个名字叫==连接点==。
2. 在 `BookServiceImpl` 的四个方法中，`update` 和 `delete` 只有打印没有计算万次执行消耗时间，但在运行时已经有该功能，那也就是说 `update` 和 `delete` 方法都已经被增强，所以对于**需要增强的方法**我们给起了一个名字叫==切入点==。
3. 执行 `BookServiceImpl` 的 `update` 和 `delete` 方法时，都被添加了一个计算万次执行消耗时间的功能，将这个功能（<font color="blue"><b>左右两边的蓝色块</b></font>）抽取到一个方法中，换句话说就是**存放共性功能的方法**，我们给起了个名字叫==通知==
4. ==通知是要增强的内容，会有多个，切入点是需要被增强的方法，也会有多个==，那哪个切入点需要添加哪个通知，就需要提前将它们之间的关系描述清楚，那么对于**通知和切入点之间的关系描述**，我们给起了个名字叫==切面==
5. **通知是一个方法，方法不能独立存在、需要被写在一个类中**，这个类我们也给起了个名字叫==通知类==

至此，AOP中的核心概念就已经介绍完了（不光Spring有AOP，AOPS一个大的概念），总结下：
* **连接点** *Join Point* ：**程序执行过程中的任意位置**，粒度为执行方法、抛出异常、设置变量等
	* ==在Spring AOP中，理解为方法的执行==
* **切入点** *Point cut* ：匹配连接点的式子
	* ==在Spring AOP中，**一个切入点可以描述一个具体方法，也可也匹配多个方法**==
		* 一个具体的方法，如 `com.itheima.dao` 包下的 `BookDao` 接口中的无形参无返回值的 `save` 方法
		* 匹配多个方法：所有的 `save` 方法，所有的 `get` 开头的方法，所有以 `Dao` 结尾的接口中的任意方法，所有带有一个参数的方法
	* ==连接点范围要比切入点范围大，是切入点的方法也一定是连接点==，但是是连接点的方法就不一定要被增强，所以可能不是切入点。
* **通知** *Advice* ：**在切入点处执行的操作**，也就是共性功能
	* 在SpringAOP中，**功能最终以方法（类）的形式呈现**
* **通知类**：定义通知的类
* **切面** *Aspect* ：描述通知与切入点的对应关系。

**小结**——这一节中主要讲解了AOP的概念与作用，以及AOP中的核心概念，学完以后大家需要能说出：
* 什么是AOP？
* AOP的作用是什么？
* AOP中核心概念分别指的是什么？
	* 连接点
	* 切入点
	* 通知
	* 通知类
	* 切面

---
# 11. AOP入门案例
## 11.1 需求分析
案例设定：测算接口执行效率，但这个案例稍微复杂了点，我们对其进行简化。简化设定：**在方法执行前输出当前系统时间**。
对于SpringAOP的开发有两种方式，XML和==注解==，我们使用哪个呢？因为现在注解使用的比较多，所以本次采用注解完成AOP的开发。

总结需求为：使用Spring AOP的注解方式，完成在方法执行的前打印出当前系统时间。
## 11.2 思路分析
需求明确后，具体该如何实现，都有哪些步骤，我们先来分析：
> 1. 导入坐标 `pom.xml`
> 2. 制作连接点（原始操作，`Dao` 接口与实现类）
> 3. 制作共性功能（通知类与通知）
> 4. 定义切入点
> 5. 绑定切入点与通知关系（切面）
## 11.3 环境准备
* 创建一个Maven项目
* `pom.xml` 添加Spring依赖
  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```
* 添加 `BookDao` 和 `BookDaoImpl` 类
  ```java
  public interface BookDao {
      public void save();
      public void update();
  }
  
  @Repository
  public class BookDaoImpl implements BookDao {
  
      public void save() {
          System.out.println(System.currentTimeMillis());
          System.out.println("book dao save ...");
      }
  
      public void update(){
          System.out.println("book dao update ...");
      }
  }
  ```
* 创建Spring的配置类
  ```java
  @Configuration
  @ComponentScan("com.itheima")
  public class SpringConfig {
  }
  ```
* 编写App运行类
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookDao bookDao = ctx.getBean(BookDao.class);
          bookDao.save();
      }
  }
  ```
  
最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200245145.png)
**说明**：
* 目前打印 `save` 方法时，因为方法中有打印系统时间，所以运行时是可以看到系统时间
* 对于 `update` 方法来说，就没有该功能
* 我们要使用SpringAOP的方式，**在不改变 `update` 方法的前提下、让其具有打印系统时间的功能**。

## 11.4 AOP实现步骤
步骤1：添加依赖 `pom.xml` ：
```xml
<dependency>
    <groupId>org.aspectj</groupId>
    <artifactId>aspectjweaver</artifactId>
    <version>1.9.4</version>
</dependency>
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200251495.png)
* 因为 **`spring-context` 中已经导入了 `spring-aop` ，所以不需要再单独导入`spring-aop`**
* 导入 `AspectJ` 的JAR包，`AspectJ` 是AOP思想的一个具体实现，Spring有自己的AOP实现，但相比于 `AspectJ` 来说比较麻烦，所以**我们直接采用Spring整合 `ApsectJ` 的方式进行AOP开发**。

步骤2：定义接口与实现类。
```
环境准备时，BookDaoImpl已经准备好，不需要做任何修改
```
步骤3：定义通知类和通知。**通知就是将共性功能抽取出来后形成的方法**，共性功能指的就是当前系统时间的打印。类名和方法名没有要求，可以任意。
```java
public class MyAdvice {
    public void method() {
        System.out.println(System.currentTimeMillis());
    }
}
```
步骤4：**定义切入点**。`BookDaoImpl` 中有两个方法，分别是 `save` 和 `update` ，我们要增强的是 `update` 方法，该如何定义呢？
```java
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt() {}
    public void method() {
        System.out.println(System.currentTimeMillis());
    }
}
```
**说明**：
* ==切入点定义依托一个不具有实际意义的方法进行，即无参数、无返回值、方法体无实际逻辑==。
* `execution` 及后面编写的内容，后面会专门学习。

步骤5：制作切面。**切面是用来描述通知和切入点之间的关系**，如何进行关系的绑定？
```java
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())") // 原始连接点 -> 切入点
    private void pt(){} 
    
    @Before("pt()")
    public void method() { // 通知
        System.out.println(System.currentTimeMillis());
    }
}
```
**绑定切入点与通知关系**（切入点一般和通知在同一个类中），并指定「通知添加到原始连接点」的具体执行==位置==：
> **说明**：`@Before` 翻译过来是之前，也就是说==通知会在切入点方法执行之前执行==，除此之前还有其他四种类型，后面会讲。
> ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200252782.png)

步骤6：**将通知类配给容器，并标识其为切面类**。
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Before("pt()")
    public void method(){
        System.out.println(System.currentTimeMillis());
    }
}
```
步骤7：**开启注解格式AOP功能**。
```java
@Configuration
@ComponentScan("com.itheima")
@EnableAspectJAutoProxy
public class SpringConfig {
}
```
步骤8：运行程序。
```java
public class App {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao = ctx.getBean(BookDao.class);
        bookDao.update();
    }
}
```
看到在执行 `update` 方法之前、打印了系统时间戳，说明对原始方法进行了增强，AOP编程成功。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200252948.png)
****
### 知识点1：`@EnableAspectJAutoProxy`
| 名称 | `@EnableAspectJAutoProxy` |
| ---- | ----------------------- |
| 类型 | 配置类注解              |
| 位置 | **配置类定义上方**          |
| 作用 | 开启注解格式AOP功能     |

### 知识点2：`@Aspect`
| 名称 | `@Aspect`               |
| ---- | --------------------- |
| 类型 | 类注解                |
| 位置 | **切面类定义上方**        |
| 作用 | 设置当前类为AOP切面类 |

### 知识点3：`@Pointcut`
| 名称 | `@Pointcut`                   |
| ---- | --------------------------- |
| 类型 | 方法注解                    |
| 位置 | **切入点方法定义上方**         |
| 作用 | 设置切入点方法              |
| 属性 | value（默认）：切入点表达式 |

### 知识点4：`@Before`
| 名称 | `@Before`             |
| ---- | -------------  |
| 类型 | 方法注解           |
| 位置 | 通知方法定义上方        |
| 作用 | 设置当前通知方法与切入点之间的绑定关系，当前通知方法在原始切入点方法前运行 |

----
# 12. AOP工作流程
AOP的入门案例已经完成，对于刚才案例的执行过程，我们就得来分析分析，这一节主要讲解两个知识点 `AOP工作流程` 和 `AOP核心概念` 。其中核心概念是对前面核心概念的补充。
## 12.1 AOP工作流程
由于**AOP是基于Spring容器管理的 `bean` 做的增强**，所以整个工作过程需要从Spring加载 `bean` 说起。

**流程1：Spring容器启动**，**容器启动就需要去加载 `bean`** ，哪些类需要被加载呢？
* 需要被增强的类，如 `BookServiceImpl`
* 通知类，如 `MyAdvice`
* 注意此时 `bean` 对象还没有创建成功

**流程2：读取所有切面配置中的切入点**，即已经使用的切入点
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200253917.png)
* 上面这个例子中有两个切入点的配置，但是第一个 `ptx()` 并没有被使用，所以不会被读取。

流程3：初始化 `bean` ，**判定「 `bean` 对应的类中的方法」是否匹配到任意切入点**
* 注意第1步在容器启动时，`bean` 对象还没有被创建成功。
* 要让实例化 `bean` 对象的类中的方法和切入点进行匹配
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200255139.png)
	* 匹配失败，创建原始对象，如` UserDao` 。匹配失败说明不需要增强，直接调用原始对象的方法即可。
	* 匹配成功，创建原始对象（==目标对象==）的==代理==对象，如 `BookDao` 。**匹配成功说明需要对其进行增强**
* ==对哪个类做增强，这个类对应的对象就叫做目标对象==
* 因为要对目标对象进行功能增强，而==采用的技术是动态代理，所以会为其创建一个代理对象==
* ===最终运行的是代理对象的方法，在该方法中会对原始方法进行功能增强==

流程4：**获取 `bean` 来执行方法**
* （匹配失败时）获取的 `bean` 是原始对象时，调用方法并执行，完成操作
* **获取的 `bean` 是代理对象时，根据代理对象的运行模式，运行原始方法与增强的内容，完成操作**

## 12.2 验证容器中是否为代理对象
为了验证「IoC容器中创建的对象」和「我们刚才所说的结论」是否一致，首先先把结论理出来：
* 如果目标对象中的方法会被增强，那么容器中将存入的是**目标对象的代理对象**
* 如果目标对象中的方法不被增强，那么容器中将存入的是目标对象本身。

验证思路：
1. 要执行的方法，不被定义的切入点包含，即不要增强，打印当前类的 `getClass()` 方法
2. 要执行的方法，被定义的切入点包含，即要增强，打印出当前类的 `getClass()` 方法
3. 观察两次打印的结果

步骤1：修改 `App` 类，获取类的类型。
```java
public class App {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao = ctx.getBean(BookDao.class);
        System.out.println(bookDao);
        System.out.println(bookDao.getClass());
    }
}
```
步骤2：修改 `MyAdvice` 类，不增强。因为定义的切入点中被修改成 `update1` ，所以 `BookDao` 中的 `update` 方法在执行时、就不会被增强，所以**容器中的对象应该是目标对象本身**。
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update1())")
    private void pt(){}
    
    @Before("pt()")
    public void method(){
        System.out.println(System.currentTimeMillis());
    }
}
```
步骤3：运行程序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200310505.png)
步骤4：修改 `MyAdvice` 类，增强。因为定义的切入点中被修改成 `update` ，所以 `BookDao` 中的 `update` 方法在执行时、就会被增强，所以容器中的对象应该是目标对象的代理对象：
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Before("pt()")
    public void method(){
        System.out.println(System.currentTimeMillis());
    }
}
```
步骤5：运行程序
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200311060.png)
至此对于刚才的结论，我们就得到了验证，这块大家需要注意的是：==不能直接打印对象，从上面两次结果中可以看出，直接打印对象走的是对象的 `toString` 方法，不管是不是代理对象打印的结果都是一样的==，原因是**内部对 `toString` 方法进行了重写**。

## 12.3 AOP核心概念
在上面介绍AOP的工作流程中，我们提到了两个核心概念，分别是：
* 目标对象 `Target` ：「原始功能去掉共性功能对应的类」产生的对象，这种对象是无法直接完成最终工作的
* 代理 `Proxy` ：目标对象无法直接完成工作，需要对其进行功能回填，**通过原始对象的代理对象实现**

上面这两个概念比较抽象，简单来说，
- 目标对象就是要增强的类（如 `BookServiceImpl` 类）对应的对象，也叫原始对象，不能说它不能运行，只能说它在运行的过程中对于要增强的内容是缺失的。
- SpringAOP是在不改变原有设计（代码）的前提下、对其进行增强的，==它的底层采用的是代理模式实现的，所以要对原始对象进行增强，就需要对原始对象创建代理对象==，在代理对象中的方法把通知（如 `MyAdvice` 中的 `method` 方法）内容加进去，就实现了增强，这就是我们所说的代理 `Proxy` 。

**小结**：通过这一节中，我们需要掌握的内容有
* 能说出AOP的工作流程
* AOP的核心概念
	* 目标对象、连接点、切入点
	* 通知类、通知
	* 切面
	* 代理
* **Spring AOP的本质或者可以说底层实现是通过代理模式**。

---
# 13. AOP配置管理
## 13.1 AOP切入点表达式
前面的案例中，有涉及到如下内容：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200315741.png)
对于AOP中切入点表达式，我们总共会学习三个内容，分别是 `语法格式` 、 `通配符` 和 `书写技巧` 。
### 13.1.1 语法格式
首先我们先要明确两个概念：
* 切入点：要进行增强的方法
* 切入点表达式：要进行增强的方法的**描述方式**

对于切入点的描述，我们其实是有两种方式的，先来看下前面的例子
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200316506.png)
描述方式一：执行 `com.itheima.dao` 包下的 `BookDao` 接口中的无参数 `update` 方法：
```java
execution(void com.itheima.dao.BookDao.update())
```
描述方式二：执行 `com.itheima.dao.impl` 包下的 `BookDaoImpl` 类中的无参数 `update` 方法：
```java
execution(void com.itheima.dao.impl.BookDaoImpl.update())
```
因为**调用接口方法时最终运行的还是其实现类的方法**，所以上面两种描述方式都是可以的。

对于切入点表达式的语法为——==切入点表达式标准格式：动作关键字(访问修饰符  返回值  包名.类/接口名.方法名(参数) 异常名）==。对于这个格式，我们不需要硬记，通过一个例子，理解它：
```java
execution(public User com.itheima.service.UserService.findById(int))
```
* `execution` ：**动作关键字**，描述切入点的行为动作，例如 `execution` 表示执行到指定切入点（几乎不变）
* `public` ：访问修饰符，还可以是 `protected, private` 等，**可以省略**
* `User` ：返回值，写返回值类型
* `com.itheima.service` ：包名，多级包使用点连接
* `UserService` ：类/接口名称
* `findById` ：方法名
* `int` ：参数，直接写参数的类型，多个类型用逗号隔开
* 异常名：**方法定义中抛出指定异常**，可以省略

切入点表达式就是要找到需要增强的方法，所以**它就是对一个具体方法的描述**，但是方法的定义会有很多，所以如果每一个方法对应一个切入点表达式，想想这块就会觉得将来编写起来会比较麻烦，有没有更简单的方式呢？就需要用到下面所学习的通配符。
### 13.1.2 通配符
我们==使用通配符描述切入点，主要的目的就是简化之前的配置==，具体都有哪些通配符可以使用？
* `*` ：**单个**独立的任意符号，可以独立出现，也可以**作为前缀或者后缀的匹配符**出现
  ```java
  execution(public * com.itheima.*.UserService.find*(*))
  ```
  匹配 `com.itheima` 包下的任意包中的、`UserService` 类或接口中所有 **`find` 开头的**、带有**一个任意参数**的方法
* `..` ：**多个**连续的任意符号，可以独立出现，常用于**简化包名与参数**的书写
  ```java
  execution(public User com..UserService.findById(..))
  ```
  匹配 `com` 包下的任意包中的、`UserService` 类或接口中所有名称为 `findById` 的方法
* `+` ：专用于**匹配子类类型**
  ```java
  execution(* *..*Service+.*(..))
  ```
  这个使用率较低，描述子类的，咱们做JavaEE开发，**继承机会就一次**，使用都很慎重，所以很少用它。`*Service+` ，表示「所有以 `Service` 结尾（在我们的代码中是接口）的接口」的子类。这一式子表示匹配业务层的所有方法。

接下来，我们把案例中使用到的切入点表达式来分析下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302200324520.png)
```java
execution(void com.itheima.dao.BookDao.update())
匹配接口，能匹配到
execution(void com.itheima.dao.impl.BookDaoImpl.update())
匹配实现类，能匹配到
execution(* com.itheima.dao.impl.BookDaoImpl.update())
返回值任意，能匹配到
execution(* com.itheima.dao.impl.BookDaoImpl.update(*))
返回值任意，但是update方法必须要有一个参数，无法匹配，要想匹配需要在update接口和实现类添加参数

execution(void com.*.*.*.*.update())
返回值为void,com包下的任意包三层包下的任意类的update方法，匹配到的是实现类，能匹配
execution(void com.*.*.*.update())
返回值为void,com包下的任意两层包下的任意类的update方法，匹配到的是接口，能匹配

execution(void *..update())
返回值为void，方法名是update的任意包下的任意类，能匹配
execution(* *..*(..))
匹配项目中任意类的任意方法，能匹配，但是不建议使用这种方式，影响范围广

execution(* *..u*(..))
匹配项目中任意包任意类下只要以u开头的方法，update方法能满足，能匹配
execution(* *..*e(..))
匹配项目中任意包任意类下只要以e结尾的方法，update和save方法能满足，能匹配

execution(void com..*())
返回值为void，com包下的任意包任意类任意方法，能匹配，*代表的是方法

execution(* com.itheima.*.*Service.find*(..))
将项目中所有业务层方法的以find开头的方法匹配
execution(* com.itheima.*.*Service.save*(..))
将项目中所有业务层方法的以save开头的方法匹配
```
后面两种更符合我们平常切入点表达式的编写规则，反着识别。
### 13.1.3 书写技巧
对于切入点表达式的编写其实是很灵活的，那么在编写时，有没有什么好的技巧让我们用用：
- 所有代码按照标准规范开发，否则以下技巧全部失效
- 描述切入点==通常描述接口==，而不描述实现类，如果描述到实现类，就出现紧耦合了
- 访问控制修饰符针对接口开发均采用 `public` 描述（**==可省略访问控制修饰符描述==**）
- 返回值类型对于增删改类使用==精准类型==加速匹配，对于查询类使用 `*` 通配快速描述
- ==包名==书写尽量不使用 `..` 匹配，效率过低，常用 `*` 做单个包描述匹配，或精准匹配
- ==接口名/类名书写名称与模块相关的==采用 `*` 匹配，例如 `UserService` 书写成 `*Service` ，绑定业务层接口名
- ==方法名==书写以==动词==进行==精准匹配==，名词采用 `*` 匹配，例如 `getById` 书写成 `getBy*` ，`selectAll` 书写成 `selectAll`
- 参数规则较为复杂，根据业务方法灵活调整
- 通常==不使用异常==作为匹配规则

## 13.2 AOP通知类型
前面的案例中，有涉及到如下内容：
```java
@Before("pt()")
```
它所代表的含义是将 `通知` 添加到 `切入点` 方法执行的==前面==。

除了这个注解外，还有没有其他的注解，换个问题就是除了可以在前面加，能不能在其他的地方加？
### 13.2.1 类型介绍
我们先来回顾下AOP通知：
* AOP通知描述了抽取的共性功能，**根据共性功能抽取的位置不同，最终运行代码时要将其加入到合理的位置**

通知具体要添加到切入点的哪里？共提供了5种通知类型：
- 前置通知
- 后置通知
- **==环绕通知（重点）==**
- 返回后通知（了解）
- 抛出异常后通知（了解）

为了更好的理解这几种通知类型，我们来看一张图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201524759.png)
1. 前置通知，追加功能到方法执行前，类似于在代码1或者代码2添加内容
2. 后置通知，追加功能到方法执行后，**不管方法执行的过程中、有没有抛出异常都会执行**，类似于在代码5添加内容
3. 返回后通知，追加功能到方法执行后，**只有方法正常执行结束后才进行**，类似于在代码3添加内容，如果方法执行抛出异常，返回后通知将不会被添加
4. 抛出异常后通知，追加功能到方法抛出异常后，**只有方法执行出异常才进行**，类似于在代码4添加内容，只有方法抛出异常后才会被添加
5. 环绕通知，==环绕通知功能比较强大，它可以追加功能到方法执行的前后，这也是比较常用的方式，它可以实现其他四种通知类型的功能==，具体是如何实现的，需要我们往下学习。

### 13.2.2 环境准备
- 创建一个Maven项目
- `pom.xml` 添加Spring依赖
  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.aspectj</groupId>
        <artifactId>aspectjweaver</artifactId>
        <version>1.9.4</version>
      </dependency>
  </dependencies>
  ```
- 添加 `BookDao` 和 `BookDaoImpl` 类
  ```java
  public interface BookDao {
      public void update();
      public int select();
  }
  
  @Repository
  public class BookDaoImpl implements BookDao {
      public void update(){
          System.out.println("book dao update ...");
      }
      public int select() {
          System.out.println("book dao select is running ...");
          return 100;
      }
  }
  ```
- 创建Spring的配置类
  ```java
  @Configuration
  @ComponentScan("com.itheima")
  @EnableAspectJAutoProxy
  public class SpringConfig {
  }
  ```
- 创建通知类
  ```java
  @Component
  @Aspect
  public class MyAdvice {
      @Pointcut("execution(void com.itheima.dao.BookDao.update())")
      private void pt(){}
  
      public void before() {
          System.out.println("before advice ...");
      }
  
      public void after() {
          System.out.println("after advice ...");
      }
  
      public void around(){
          System.out.println("around before advice ...");
          System.out.println("around after advice ...");
      }
  
      public void afterReturning() {
          System.out.println("afterReturning advice ...");
      }
      
      public void afterThrowing() {
          System.out.println("afterThrowing advice ...");
      }
  }
  ```
- 编写App运行类
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookDao bookDao = ctx.getBean(BookDao.class);
          bookDao.update();
      }
  }
  ```

最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201537956.png)

### 13.2.3 通知类型的使用
#### 1. 前置通知
修改 `MyAdvice` ，在 `before` 方法上添加 `@Before` 注解。
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Before("pt()")
    //此处也可以写成 @Before("MyAdvice.pt()"),不建议
    public void before() {
        System.out.println("before advice ...");
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201539248.png)
#### 2. 后置通知
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Before("pt()")
    public void before() {
        System.out.println("before advice ...");
    }
    @After("pt()")
    public void after() {
        System.out.println("after advice ...");
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201540945.png)
#### 3. 环绕通知
##### 基本使用
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Around("pt()")
    public void around(){
        System.out.println("around before advice ...");
        System.out.println("around after advice ...");
    }
}
```
运行结果中，通知的内容打印出来，但是原始方法的内容却没有被执行。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201541440.png)
因为**环绕通知需要在原始方法的前后进行增强**，所以**环绕通知就必须要能对原始操作进行调用**，具体如何实现？
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Around("pt()")
    public void around(ProceedingJoinPoint pjp) throws Throwable{
        System.out.println("around before advice ...");
        //表示对原始操作的调用
        pjp.proceed();
        System.out.println("around after advice ...");
    }
}
```
**说明**：`proceed()` 为什么要抛出异常？原因很简单，看下源码就知道了。AOP只负责增强功能，不负责帮原始对象的方法处理异常，强制抛出一个 `Throwable` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201544924.png)
再次运行，程序可以看到原始方法已经被执行了。

##### 注意事项
(1) **原始方法有返回值的处理**
修改 `MyAdvice` ，对 `BookDao` 中的 `select` 方法添加环绕通知：
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Pointcut("execution(int com.itheima.dao.BookDao.select())")
    private void pt2(){}
    
    @Around("pt2()")
    public void aroundSelect(ProceedingJoinPoint pjp) throws Throwable {
        System.out.println("around before advice ...");
        //表示对原始操作的调用
        pjp.proceed();
        System.out.println("around after advice ...");
    }
}
```
再修改 `App` 类，调用 `select` 方法：
```java
public class App {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao = ctx.getBean(BookDao.class);
        int num = bookDao.select();
        System.out.println(num);
    }
}
```
运行后会报错，错误内容为：
```
Exception in thread "main" org.springframework.aop.AopInvocationException: Null return value from advice does not match primitive return type for: public abstract int com.itheima.dao.BookDao.select()
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:226)
	at com.sun.proxy.$Proxy19.select(Unknown Source)
	at com.itheima.App.main(App.java:12)
```
错误大概的意思是：`空的返回不匹配原始方法的int返回`
* `void` 就是返回 `Null`
* 原始方法就是 `BookDao` 下的 `select` 方法

所以如果我们==使用环绕通知的话，要根据原始方法的返回值来设置环绕通知的返回值==，具体解决方案为：
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Pointcut("execution(int com.itheima.dao.BookDao.select())")
    private void pt2(){}
    
    @Around("pt2()")
    public Object aroundSelect(ProceedingJoinPoint pjp) throws Throwable {
        System.out.println("around before advice ...");
        //表示对原始操作的调用
        Object ret = pjp.proceed();
        System.out.println("around after advice ...");
        return ret;
    }
}
```
**说明**：
- 为什么返回的是 `Object` 而不是 `int` 的主要原因是 `Object` 类型更通用。
- **在环绕通知中是可以对原始方法返回值就行修改的**。

#### 4. 返回后通知
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Pointcut("execution(int com.itheima.dao.BookDao.select())")
    private void pt2(){}
    
    @AfterReturning("pt2()")
    public void afterReturning() {
        System.out.println("afterReturning advice ...");
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201557792.png)
**注意**：返回后通知是需要在原始方法 `select` 正常执行后才会被执行，如果 `select()` 方法执行的过程中出现了异常，那么返回后通知不会被执行。==后置通知是不管原始方法有没有抛出异常都会被执行==。这个案例可以自己验证下。
#### 5. 异常后通知
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(void com.itheima.dao.BookDao.update())")
    private void pt(){}
    
    @Pointcut("execution(int com.itheima.dao.BookDao.select())")
    private void pt2(){}
    
    @AfterReturning("pt2()")
    public void afterThrowing() {
        System.out.println("afterThrowing advice ...");
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201601135.png)
**注意**：异常后通知是需要原始方法抛出异常，可以在 `select()` 方法中添加一行代码 `int i = 1/0` 即可。如果没有抛异常，异常后通知将不会被执行。

学习完这5种通知类型，思考下环绕通知是如何实现其他通知类型的功能的？==因为环绕通知是可以控制原始方法执行的，所以把增强的代码写在调用原始方法的不同位置，就可以实现不同的通知类型的功能==，如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201606919.png)

#### 6. 通知类型总结
##### 知识点1：`@After`
| 名称 | `@After`                                                       |
| ---- | --------------------------- |
| 类型 | 方法注解                                                     |
| 位置 | 通知方法定义上方                                             |
| 作用 | 设置当前通知方法与切入点之间的绑定关系，当前通知方法在原始切入点方法后运行 |

##### 知识点2：`@AfterReturning`
| 名称 | @AfterReturning                                              |
| ---- | ------------------------------------------------------------ |
| 类型 | 方法注解                                                     |
| 位置 | 通知方法定义上方                                             |
| 作用 | 设置当前通知方法与切入点之间的绑定关系，当前通知方法在原始切入点方法正常执行完毕后执行 |

##### 知识点3：`@AfterThrowing`  
| 名称 | `@AfterThrowing`                                               |
| ---- | ------------------------------------- |
| 类型 | 方法注解                                                     |
| 位置 | 通知方法定义上方                                             |
| 作用 | 设置当前通知方法与切入点之间的绑定关系，当前通知方法在原始切入点方法运行抛出异常后执行 |

##### 知识点4：`@Around`
| 名称 | @Around                                                      |
| ---- | ------------------------------------------------------------ |
| 类型 | 方法注解                                                     |
| 位置 | 通知方法定义上方                                             |
| 作用 | 设置当前通知方法与切入点之间的绑定关系，当前通知方法在原始切入点方法前后运行 |

==**环绕通知注意事项**==
1. 环绕通知必须依赖形参 `ProceedingJoinPoint` ，才能实现对原始方法的调用，进而实现原始方法调用前后同时添加通知
2. 通知中如果未使用 `ProceedingJoinPoint` 对原始方法进行调用，将跳过原始方法的执行（可以用来做权限隔离）
3. **对原始方法的调用可以不接收返回值**，通知方法设置成 `void` 即可，如果接收返回值，最好设定为 `Object` 类型
4. 原始方法的返回值如果是 `void` 类型，通知方法的返回值类型可以设置成 `void` ，也可以设置成 `Object`
5. **由于无法预知原始方法运行后是否会抛出异常，因此环绕通知方法必须要处理 `Throwable` 异常**

介绍完这么多种通知类型，具体该选哪一种呢？我们可以通过一些案例加深对通知类型的学习。
## 13.3 业务层接口执行效率
### 13.3.1 需求分析
这个需求也比较简单，前面我们在介绍AOP时已经演示过：
- 需求：任意业务层接口执行，均可显示其执行效率（执行时长）

这个案例的目的是==查看每个业务层执行的时间，这样就可以监控出哪个业务比较耗时，将其查找出来方便优化==。具体实现的思路：
1. 开始执行方法之前记录一个时间
2. 执行方法
3. 执行完方法之后记录一个时间
4. 用后一个时间减去前一个时间的差值，就是我们需要的结果。

所以要在方法执行的前后添加业务，经过分析我们将采用 `环绕通知` 。

**说明**：原始方法如果只执行一次，时间太快，两个时间差可能为0，所以我们要执行万次来计算时间差。
### 13.3.2 环境准备
- 创建一个Maven项目
- `pom.xml` 添加Spring依赖
  ```xml
  <dependencies>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>5.2.10.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-jdbc</artifactId>
        <version>5.2.10.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-test</artifactId>
        <version>5.2.10.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.aspectj</groupId>
        <artifactId>aspectjweaver</artifactId>
        <version>1.9.4</version>
      </dependency>
      <dependency>
        <groupId>mysql</groupId>
        <artifactId>mysql-connector-java</artifactId>
        <version>5.1.47</version>
      </dependency>
      <dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>druid</artifactId>
        <version>1.1.16</version>
      </dependency>
      <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis</artifactId>
        <version>3.5.6</version>
      </dependency>
      <dependency>
        <groupId>org.mybatis</groupId>
        <artifactId>mybatis-spring</artifactId>
        <version>1.3.0</version>
      </dependency>
      <dependency>
        <groupId>junit</groupId>
        <artifactId>junit</artifactId>
        <version>4.12</version>
        <scope>test</scope>
      </dependency>
    </dependencies>
  ```
- 添加 `AccountService, AccountServiceImpl, AccountDao, Account` 类
  ```java
  public interface AccountService {
      void save(Account account);
      void delete(Integer id);
      void update(Account account);
      List<Account> findAll();
      Account findById(Integer id);
  }
  @Service
  public class AccountServiceImpl implements AccountService {
      @Autowired
      private AccountDao accountDao;
  
      public void save(Account account) {
          accountDao.save(account);
      }

      public void update(Account account){
          accountDao.update(account);
      }
      public void delete(Integer id) {
          accountDao.delete(id);
      }
      public Account findById(Integer id) {
          return accountDao.findById(id);
      }
      public List<Account> findAll() {
          return accountDao.findAll();
      }
  }
  public interface AccountDao { 
      @Insert("insert into tbl_account(name,money)values(#{name},#{money})")
      void save(Account account);
  
      @Delete("delete from tbl_account where id = #{id} ")
      void delete(Integer id);
  
      @Update("update tbl_account set name = #{name} , money = #{money} where id = #{id} ")
      void update(Account account);
  
      @Select("select * from tbl_account")
      List<Account> findAll();
  
      @Select("select * from tbl_account where id = #{id} ")
      Account findById(Integer id);
  }
  
  public class Account implements Serializable {
      private Integer id;
      private String name;
      private Double money;
      //setter..getter..toString方法省略
  }
  ```
	`AccountDao` 的 `bean` 是在 `MyBaitsConfig` 中进行包扫描中返回的。
- `resources` 下提供一个 `jdbc.properties`
  ```java
  jdbc.driver=com.mysql.jdbc.Driver
  jdbc.url=jdbc:mysql://localhost:3306/spring_db?useSSL=false
  jdbc.username=root
  jdbc.password=root
  ```
- 创建相关配置类
  ```java
  //Spring配置类:SpringConfig
  @Configuration
  @ComponentScan("com.itheima")
  @PropertySource("classpath:jdbc.properties")
  @Import({JdbcConfig.class,MybatisConfig.class})
  public class SpringConfig {
  }
  //JdbcConfig配置类
  public class JdbcConfig {
      @Value("${jdbc.driver}")
      private String driver;
      @Value("${jdbc.url}")
      private String url;
      @Value("${jdbc.username}")
      private String userName;
      @Value("${jdbc.password}")
      private String password;
  
      @Bean
      public DataSource dataSource(){
          DruidDataSource ds = new DruidDataSource();
          ds.setDriverClassName(driver);
          ds.setUrl(url);
          ds.setUsername(userName);
          ds.setPassword(password);
          return ds;
      }
  }
  //MybatisConfig配置类
  public class MybatisConfig {
      @Bean
      public SqlSessionFactoryBean sqlSessionFactory(DataSource dataSource){
          SqlSessionFactoryBean ssfb = new SqlSessionFactoryBean();
          ssfb.setTypeAliasesPackage("com.itheima.domain");
          ssfb.setDataSource(dataSource);
          return ssfb;
      }
      @Bean
      public MapperScannerConfigurer mapperScannerConfigurer(){
          MapperScannerConfigurer msc = new MapperScannerConfigurer();
          msc.setBasePackage("com.itheima.dao");
          return msc;
      }
  }
  ```
- **编写Spring整合Junit的测试类**
  ```java
  @RunWith(SpringJUnit4ClassRunner.class)
  @ContextConfiguration(classes = SpringConfig.class)
  public class AccountServiceTestCase {
      @Autowired
      private AccountService accountService;
      @Test
      public void testFindById(){
          Account ac = accountService.findById(2);
      }
      @Test
      public void testFindAll(){
          List<Account> all = accountService.findAll();
      }
  }
  ```

最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201621251.png)

### 13.3.3 功能开发
步骤1：开启SpringAOP的注解功能。在Spring的主配置文件 `SpringConfig` 类中添加注解。
```java
@EnableAspectJAutoProxy
```
步骤2：创建AOP的通知类
* 该类要被Spring管理，需要添加 `@Component`
* 要标识该类是一个AOP的切面类，需要添加 `@Aspect`
* 配置切入点表达式，需要添加一个方法，并添加 `@Pointcut`

```java
@Component
@Aspect
public class ProjectAdvice {
    //配置业务层的所有方法
    @Pointcut("execution(* com.itheima.service.*Service.*(..))")
    private void servicePt(){}
    public void runSpeed(){
    } 
}
```

步骤3：添加环绕通知。在 `runSpeed()` 方法上添加 `@Around` 。
```java
@Component
@Aspect
public class ProjectAdvice {
    //配置业务层的所有方法
    @Pointcut("execution(* com.itheima.service.*Service.*(..))")
    private void servicePt(){}
    
    //@Around("ProjectAdvice.servicePt()") 可以简写为下面的方式
    @Around("servicePt()")
    public Object runSpeed(ProceedingJoinPoint pjp){
        Object ret = pjp.proceed();
        return ret;
    } 
}
```
**注意**：目前并没有做任何增强。

步骤4：完成核心业务，记录万次执行的时间。
```java
@Component
@Aspect
public class ProjectAdvice {
    //配置业务层的所有方法
    @Pointcut("execution(* com.itheima.service.*Service.*(..))")
    private void servicePt(){}
    //@Around("ProjectAdvice.servicePt()") 可以简写为下面的方式
    @Around("servicePt()")
    public void runSpeed(ProceedingJoinPoint pjp){
        
        long start = System.currentTimeMillis();
        for (int i = 0; i < 10000; i++) {
           pjp.proceed();
        }
        long end = System.currentTimeMillis();
        System.out.println("业务层接口万次执行时间: "+(end-start)+"ms");
    } 
}
```
步骤5：运行单元测试类
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201646565.png)

**注意**：因为程序每次执行的时长是不一样的，所以运行多次最终的结果是不一样的。

步骤6：程序优化。目前程序所面临的问题是，多个方法一起执行测试时，控制台都打印的是：`业务层接口万次执行时间:xxxms` 。我们没有办法区分到底是哪个接口的哪个方法执行的具体时间，具体如何优化？
```java
@Component
@Aspect
public class ProjectAdvice {
    //配置业务层的所有方法
    @Pointcut("execution(* com.itheima.service.*Service.*(..))")
    private void servicePt(){}
    //@Around("ProjectAdvice.servicePt()") 可以简写为下面的方式
    @Around("servicePt()")
    public void runSpeed(ProceedingJoinPoint pjp){
        //获取执行签名信息
        Signature signature = pjp.getSignature();
        //通过签名获取执行操作名称(接口名)
        String className = signature.getDeclaringTypeName();
        //通过签名获取执行操作名称(方法名)
        String methodName = signature.getName();
        
        long start = System.currentTimeMillis();
        for (int i = 0; i < 10000; i++) {
           pjp.proceed();
        }
        long end = System.currentTimeMillis();
        System.out.println("万次执行："+ className+"."+methodName+"---->" +(end-start) + "ms");
    } 
}
```
步骤7：运行单元测试类。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201649609.png)

==补充说明==
当前测试的接口执行效率仅仅是一个理论值，并不是一次完整的执行过程。

这块只是通过该案例把AOP的使用进行了学习，具体的实际值是有很多因素共同决定的。

## 13.4 AOP通知获取数据
目前我们写AOP、仅仅是在原始方法前后追加一些操作，接下来要说说AOP中数据相关的内容，从 `获取参数`、`获取返回值` 和 `获取异常` 三个方面来研究切入点的相关信息。

前面我们介绍通知类型时总共讲了五种，那么对于这五种类型都会有参数，返回值和异常吗？我们先来一个个分析：
* 获取切入点方法的参数，所有的通知类型都可以获取参数
	* `JoinPoint` ：适用于前置、后置、返回后、抛出异常后通知
	* `ProceedingJoinPoint` ：适用于环绕通知
* 获取切入点方法返回值，**前置和抛出异常后通知是没有返回值**，后置通知可有可无，所以不做研究
	* 返回后通知
	* 环绕通知
* 获取切入点方法运行异常信息，**前置和返回后通知是不会有**，后置通知可有可无，所以不做研究
	* 抛出异常后通知
	* 环绕通知

### 13.4.1 环境准备
- 创建一个Maven项目
- `pom.xml` 添加Spring依赖
  ```xml
  <dependencies>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>5.2.10.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.aspectj</groupId>
        <artifactId>aspectjweaver</artifactId>
        <version>1.9.4</version>
      </dependency>
    </dependencies>
  ```
- 添加 `BookDao` 和 `BookDaoImpl` 类
  ```java
  public interface BookDao {
      public String findName(int id);
  }
  @Repository
  public class BookDaoImpl implements BookDao {
      public String findName(int id) {
          System.out.println("id:"+id);
          return "itcast";
      }
  }
  ```
- 创建Spring的配置类
  ```java
  @Configuration
  @ComponentScan("com.itheima")
  @EnableAspectJAutoProxy
  public class SpringConfig {
  }
  ```
- 编写通知类
  ```java
  @Component
  @Aspect
  public class MyAdvice {
      @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
      private void pt(){}
  
      @Before("pt()")
      public void before() {
          System.out.println("before advice ..." );
      }
  
      @After("pt()")
      public void after() {
          System.out.println("after advice ...");
      }
  
      @Around("pt()")
      public Object around() throws Throwable{
          Object ret = pjp.proceed();
          return ret;
      }
      @AfterReturning("pt()")
      public void afterReturning() {
          System.out.println("afterReturning advice ...");
      }
  
      @AfterThrowing("pt()")
      public void afterThrowing() {
          System.out.println("afterThrowing advice ...");
      }
  }
  ```
- 编写App运行类
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookDao bookDao = ctx.getBean(BookDao.class);
          String name = bookDao.findName(100);
          System.out.println(name);
      }
  }
  ```

最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201712137.png)
### 13.4.2 获取参数
#### 1. 非环绕通知获取方式
在方法上添加 `JoinPoint` ，通过 `JoinPoint` 来获取参数。
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
    private void pt(){}

    @Before("pt()")
    public void before(JoinPoint jp) 
        Object[] args = jp.getArgs();
        System.out.println(Arrays.toString(args));
        System.out.println("before advice ..." );
    }
	//...其他的略
}
```
运行 `App` 类，可以获取如下内容，说明参数100已经被获取：
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201714002.png)

**思考**：方法的参数只有一个，为什么获取的是一个数组？——因为参数的个数是不固定的，所以使用数组更通配些。如果将参数改成两个会是什么效果呢？

(1) 修改 `BookDao` 接口和 `BookDaoImpl` 实现类
```java
public interface BookDao {
    public String findName(int id,String password);
}
@Repository
public class BookDaoImpl implements BookDao {

    public String findName(int id,String password) {
        System.out.println("id:"+id);
        return "itcast";
    }
}
```
(2) 修改 `App` 类，调用方法传入多个参数
```java
public class App {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao = ctx.getBean(BookDao.class);
        String name = bookDao.findName(100,"itheima");
        System.out.println(name);
    }
}
```
(3) 运行 `App` ，查看结果,说明两个参数都已经被获取到
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201718872.png)

**说明**：使用 `JoinPoint` 的方式获取参数，适用于 `前置`、`后置`、`返回后`、`抛出异常后`通知。剩下的大家自行去验证。
#### 2. 环绕通知获取方式
环绕通知使用的是 `ProceedingJoinPoint` ，因为 `ProceedingJoinPoint` 是 `JoinPoint` 类的子类，所以对于 `ProceedingJoinPoint` 类中应该也会有对应的 `getArgs()` 方法，我们去验证下：
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
    private void pt(){}

    @Around("pt()")
    public Object around(ProceedingJoinPoint pjp)throws Throwable {
        Object[] args = pjp.getArgs();
        System.out.println(Arrays.toString(args));
        Object ret = pjp.proceed();
        return ret;
    }
	//其他的略
}
```
运行 `App` 后查看运行结果，说明 `ProceedingJoinPoint` 也是可以通过 `getArgs()` 获取参数：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201729540.png)
**注意**：`pjp.proceed()` 方法是有两个构造方法，分别是
* 调用无参数的 `proceed` ，当原始方法有参数，会在调用的过程中自动传入参数
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201730811.png)
* 所以调用这两个方法的任意一个都可以完成功能
* 但当需要修改原始方法的参数时，就只能采用带有参数的方法，如下：
    ```java
    @Component
    @Aspect
    public class MyAdvice {
        @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
        private void pt(){}
    
        @Around("pt()")
        public Object around(ProceedingJoinPoint pjp) throws Throwable{
            Object[] args = pjp.getArgs();
            System.out.println(Arrays.toString(args));
            args[0] = 666;
            Object ret = pjp.proceed(args);
            return ret;
        }
    	//其他的略
    }
    ```
    有了这个特性后，我们就可以==在环绕通知中对原始方法的参数进行拦截过滤==，避免由于参数的问题导致程序无法正确运行，保证代码的健壮性。

### 13.4.3 获取返回值
**对于返回值，只有返回后 `AfterReturing` 和环绕 `Around` 这两个通知类型可以获取**，具体如何获取？
#### 1. 环绕通知获取返回值
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
    private void pt(){}

    @Around("pt()")
    public Object around(ProceedingJoinPoint pjp) throws Throwable{
        Object[] args = pjp.getArgs();
        System.out.println(Arrays.toString(args));
        args[0] = 666;
        Object ret = pjp.proceed(args);
        return ret;
    }
	//其他的略
}
```
上述代码中，`ret` 就是方法的返回值，**我们是可以直接获取，不但可以获取，如果需要还可以进行修改**。
#### 2. 返回后通知获取返回值
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
    private void pt(){}

    @AfterReturning(value = "pt()",returning = "ret")
    public void afterReturning(Object ret) {
        System.out.println("afterReturning advice ..."+ret);
    }
	//其他的略
}
```

==注意==：
(1) 参数名的问题
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201748742.png)
(2) `afterReturning` 方法参数类型的问题——参数类型可以写成 `String` ，但是**为了能匹配更多的参数类型，建议写成 `Object` 类型**。

(3) `afterReturning` 方法参数的顺序问题
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201749931.png)

运行 `App` 后查看运行结果，说明返回值已经被获取到：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201749150.png)

### 13.4.4 获取异常
对于获取抛出的异常，只有抛出异常后 `AfterThrowing` 和环绕 `Around` 这两个通知类型可以获取，具体如何获取？
#### 1. 环绕通知获取异常
这块比较简单，以前我们是抛出异常，**现在只需要将异常捕获**，就可以获取到原始方法的异常信息了：
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
    private void pt(){}

    @Around("pt()")
    public Object around(ProceedingJoinPoint pjp){
        Object[] args = pjp.getArgs();
        System.out.println(Arrays.toString(args));
        args[0] = 666;
        Object ret = null;
        try{
            ret = pjp.proceed(args);
        }catch(Throwable throwable){
            t.printStackTrace();
        }
        return ret;
    }
	//其他的略
}
```
在 `catch` 方法中就可以获取到异常，至于获取到异常以后该如何处理，这个就和你的业务需求有关了。
#### 2. 抛出异常后通知获取异常
```java
@Component
@Aspect
public class MyAdvice {
    @Pointcut("execution(* com.itheima.dao.BookDao.findName(..))")
    private void pt(){}

    @AfterThrowing(value = "pt()",throwing = "t")
    public void afterThrowing(Throwable t) {
        System.out.println("afterThrowing advice ..."+t);
    }
	//其他的略
}
```
如何让原始方法抛出异常，方式有很多：
```java
@Repository
public class BookDaoImpl implements BookDao {

    public String findName(int id,String password) {
        System.out.println("id:"+id);
        if(true){ // 蒙过去让它执行
            throw new NullPointerException();
        }
        return "itcast";
    }
}
```
==注意==
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201753100.png)

运行 `App` 后，查看控制台，就能看的异常信息被打印到控制台
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201753117.png)
至此，AOP通知如何获取数据就已经讲解完了，数据中包含 `参数` 、`返回值` 、`异常(了解)`。

## 13.5 百度网盘密码数据兼容处理
### 13.5.1 需求分析
需求： 对百度网盘分享链接输入密码时尾部多输入的空格做兼容处理。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201754239.png)
问题描述：
* 点击链接，会提示，请输入提取码，如下图所示
		![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201754512.png)
* 当我们从别人发给我们的内容中复制提取码时，有时候会多复制到一些空格，直接粘贴到百度的提取码输入框
* 但百度那边记录的提取码是没有空格的
* 这个时候如果不做处理，直接对比的话，就会引发提取码不一致，导致无法访问百度盘上的内容
* 所以多输入一个空格可能会导致项目的功能无法正常使用。
* 此时我们就想，能不能将输入的参数先帮用户去掉空格再操作呢？

答案是可以的，**我们只需要在业务方法执行之前，对所有的输入参数进行格式处理—— `trim()`** 。


是对所有的参数都需要去除空格么？也没有必要，一般只需要针对字符串处理即可。以后涉及到「需要去除前后空格」的业务可能会有很多，这个去空格的代码是每个业务都写么？可以考虑使用AOP来统一处理。
* AOP有五种通知类型，该使用哪种呢？

我们的需求是：==将原始方法的参数处理后在参与原始方法的调用，能做这件事的就只有环绕通知==。综上所述，我们需要考虑两件事:
1. 在业务方法执行之前对所有的输入参数进行格式处理—— `trim()`
2. 使用处理后的参数调用原始方法——环绕通知中存在对原始方法的调用
### 13.5.2 环境准备
- 创建一个Maven项目
- `pom.xml` 添加Spring依赖
  ```xml
  <dependencies>
      <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>5.2.10.RELEASE</version>
      </dependency>
      <dependency>
        <groupId>org.aspectj</groupId>
        <artifactId>aspectjweaver</artifactId>
        <version>1.9.4</version>
      </dependency>
    </dependencies>
  ```
- 添加 `ResourcesService，ResourcesServiceImpl,ResourcesDao, ResourcesDaoImpl` 类
  ```java
  public interface ResourcesDao {
      boolean readResources(String url, String password);
  }
  @Repository
  public class ResourcesDaoImpl implements ResourcesDao {
      public boolean readResources(String url, String password) {
          //模拟校验
          return password.equals("root");
      }
  }
  public interface ResourcesService {
      public boolean openURL(String url ,String password);
  }
  @Service
  public class ResourcesServiceImpl implements ResourcesService {
      @Autowired
      private ResourcesDao resourcesDao;
  
      public boolean openURL(String url, String password) {
          return resourcesDao.readResources(url,password);
      }
  }
  
  ```
- 创建Spring的配置类
  ```java
  @Configuration
  @ComponentScan("com.itheima")
  public class SpringConfig {
  }
  ```
- 编写 `App` 运行类
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          ResourcesService resourcesService = ctx.getBean(ResourcesService.class);
          boolean flag = resourcesService.openURL("http://pan.baidu.com/haha", "root");
          System.out.println(flag);
      }
  }
  ```

最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302201756574.png)

现在项目的效果是，当输入密码为 `"root"` 控制台打印为 `true` ，如果密码改为 `"root "` 控制台打印的是 `false` 。

需求是使用AOP将参数进行统一处理，不管输入的密码 `root` 前后包含多少个空格，最终控制台打印的都是 `true` 。
### 13.5.3 具体实现
步骤1：开启Spring AOP的注解功能。
```java
@Configuration
@ComponentScan("com.itheima")
@EnableAspectJAutoProxy
public class SpringConfig {
}
```
步骤2：编写通知类
```java
@Component
@Aspect
public class DataAdvice {
    @Pointcut("execution(boolean com.itheima.service.*Service.*(*,*))")
    private void servicePt(){}
    
}
```
步骤3：添加环绕通知
```java
@Component
@Aspect
public class DataAdvice {
    @Pointcut("execution(boolean com.itheima.service.*Service.*(*,*))")
    private void servicePt(){}
    
    @Around("DataAdvice.servicePt()")
    // @Around("servicePt()")这两种写法都对
    public Object trimStr(ProceedingJoinPoint pjp) throws Throwable {
        Object ret = pjp.proceed();
        return ret;
    }
    
}
```
步骤4：完成核心业务，处理参数中的空格
```java
@Component
@Aspect
public class DataAdvice {
    @Pointcut("execution(boolean com.itheima.service.*Service.*(*,*))")
    private void servicePt(){}
    
    @Around("DataAdvice.servicePt()")
    // @Around("servicePt()")这两种写法都对
    public Object trimStr(ProceedingJoinPoint pjp) throws Throwable {
        //获取原始方法的参数
        Object[] args = pjp.getArgs();
        for (int i = 0; i < args.length; i++) { // foreach得到的对象只读
            //判断参数是不是字符串
            if(args[i].getClass().equals(String.class)){
                args[i] = args[i].toString().trim();
            }
        }
        //将修改后的参数传入到原始方法的执行中
        Object ret = pjp.proceed(args);
        return ret;
    }
    
}
```
步骤5：运行程序。不管密码 `root` 前后是否加空格，最终控制台打印的都是 `true` 。

步骤6：优化测试。为了能更好的看出AOP已经生效，我们可以修改 `ResourcesImpl` 类，在方法中将密码的长度进行打印。
```java
@Repository
public class ResourcesDaoImpl implements ResourcesDao {
    public boolean readResources(String url, String password) {
        System.out.println(password.length());
        //模拟校验
        return password.equals("root");
    }
}
```
再次运行成功，就可以根据最终打印的长度来看看，字符串的空格有没有被去除掉。

**注意**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202149290.png)

---
# 14. AOP总结
AOP的知识就已经讲解完了，接下来对于AOP的知识进行一个总结。
## 14.1 AOP的核心概念
概念：面向切面编程 *AOP, Aspect Oriented Programming* ，一种编程范式
* 作用：在不惊动原始设计的基础上为方法进行功能==增强==
* 核心概念
	* 代理（Proxy）：**SpringAOP的核心本质是采用代理模式实现的**
	* 连接点（JoinPoint）：在SpringAOP中，理解为**任意方法的执行**
	* 切入点（Pointcut）：**匹配连接点的式子**，也是具有共性功能的方法描述
	* 通知（Advice）：若干个方法的共性功能，在切入点处执行，**最终体现为一个方法**
	* 切面（Aspect）：**描述通知与切入点的对应关系**
	* 目标对象（Target）：被代理的原始对象成为目标对象

## 14.2 切入点表达式
切入点表达式标准格式：动作关键字(访问修饰符  返回值  包名.类/接口名.方法名（参数）异常名)
```java
execution(* com.itheima.service.*Service.*(..))
```
切入点表达式描述通配符：
* 作用：用于快速描述，范围描述
* `*` ：**匹配任意符号**（常用）
* `..` ：**匹配多个连续的任意符号**（常用）
* `+` ：匹配子类类型

切入点表达式书写技巧
1. 按==标准规范==开发
2. 查询操作的返回值建议使用 `*` 匹配
3. **减少使用 `..` 的形式描述包**
4. ==对接口进行描述==，使用 `*` 表示模块名，例如 `UserService` 的匹配描述为 `*Service`
5. **方法名书写保留动词**，例如 `get` ，**使用 `*` 表示名词**，例如 `getById` 匹配描述为 `getBy*`
6. 参数根据实际情况灵活调整

## 14.3 五种通知类型
- 前置通知
- 后置通知
- 环绕通知（重点）
	- 环绕通知依赖形参 `ProceedingJoinPoint` 才能实现对原始方法的调用
	- **环绕通知可以隔离原始方法的调用执行**
	- **环绕通知返回值设置为 `Object` 类型**
	- 环绕通知中可以对「原始方法调用过程中出现的异常」进行处理
- 返回后通知
- 抛出异常后通知

## 14.4 通知中获取参数
- 获取切入点方法的参数，所有的通知类型都可以获取参数
	- `JoinPoint` ：适用于前置、后置、返回后、抛出异常后通知
	- `ProceedingJoinPoint` ：适用于环绕通知
- 获取切入点方法返回值，前置和抛出异常后通知是没有返回值，后置通知可有可无，所以不做研究
	- 返回后通知
	- 环绕通知
- 获取切入点方法运行异常信息，前置和返回后通知是不会有，后置通知可有可无，所以不做研究
	- 抛出异常后通知
	- 环绕通知

---
# 15. AOP事务管理
## 15.1 Spring事务简介
### 15.1.1 相关概念介绍
事务作用：在数据层保障一系列的数据库操作同成功同失败。Spring事务作用：在==数据层==或==业务层==**保障一系列的数据库操作同成功同失败

数据层有事务我们可以理解，为什么业务层也需要处理事务呢？举个简单的例子，
* 转账业务会有两次数据层的调用，一次是加钱一次是减钱
* 把事务放在数据层，加钱和减钱就有两个事务
* 没办法保证加钱和减钱同时成功或者同时失败
* 这个时候就需要将事务放在业务层进行处理。

Spring为了管理事务，提供了一个平台事务管理器 `PlatformTransactionManager`
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202158154.png)
`commit` 是用来提交事务，`rollback` 是用来回滚事务。

`PlatformTransactionManager` 只是一个接口，Spring还为其提供了一个具体的实现：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202158105.png)
从名称上可以看出，**我们只需要给它一个 `DataSource` 对象，它就可以帮你去在业务层管理事务**。其**内部采用的是JDBC的事务**。所以说如果你持久层采用的是JDBC相关的技术，就可以采用这个事务管理器来管理你的事务。而Mybatis内部采用的就是JDBC的事务，所以后期我们Spring整合Mybatis就采用的这个 `DataSourceTransactionManager` 事务管理器。

### 15.1.2 转账案例-需求分析
接下来通过一个案例来学习下Spring是如何来管理事务的。先分析下需求：实现任意两个账户间转账操作。需求微缩: A账户减钱，B账户加钱

为了实现上述的业务需求，我们可以按照下面步骤来实现下：
1. 数据层提供基础操作，指定账户减钱（outMoney），指定账户加钱（inMoney）
2. 业务层提供转账操作（transfer），调用减钱与加钱的操作
3. 提供2个账号和操作金额执行转账操作
4. 基于Spring整合MyBatis环境搭建上述操作

### 15.1.3 转账案例-环境搭建
步骤1：准备数据库表。之前我们在整合Mybatis时已经创建了这个表，可以直接使用
```sql
create database spring_db character set utf8;
use spring_db;
create table tbl_account(
    id int primary key auto_increment,
    name varchar(35),
    money double
);
insert into tbl_account values(1,'Tom',1000);
insert into tbl_account values(2,'Jerry',1000);
```
步骤2：创建项目导入JAR包。项目的 `pom.xml` 添加相关依赖
```xml
<dependencies>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-context</artifactId>
      <version>5.2.10.RELEASE</version>
    </dependency>
    <dependency>
      <groupId>com.alibaba</groupId>
      <artifactId>druid</artifactId>
      <version>1.1.16</version>
    </dependency>

    <dependency>
      <groupId>org.mybatis</groupId>
      <artifactId>mybatis</artifactId>
      <version>3.5.6</version>
    </dependency>

    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
      <version>5.1.47</version>
    </dependency>

    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-jdbc</artifactId>
      <version>5.2.10.RELEASE</version>
    </dependency>

    <dependency>
      <groupId>org.mybatis</groupId>
      <artifactId>mybatis-spring</artifactId>
      <version>1.3.0</version>
    </dependency>

    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.12</version>
      <scope>test</scope>
    </dependency>

    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-test</artifactId>
      <version>5.2.10.RELEASE</version>
    </dependency>

  </dependencies>
```
步骤3：根据表创建模型类。
```java
public class Account implements Serializable {

    private Integer id;
    private String name;
    private Double money;
	//setter...getter...toString...方法略    
}
```
步骤4：创建Dao接口。
```java
public interface AccountDao {

    @Update("update tbl_account set money = money + #{money} where name = #{name}")
    void inMoney(@Param("name") String name, @Param("money") Double money);

    @Update("update tbl_account set money = money - #{money} where name = #{name}")
    void outMoney(@Param("name") String name, @Param("money") Double money);
}
```
步骤5：创建Service接口和实现类。
```java
public interface AccountService {
    /**
     * 转账操作
     * @param out 传出方
     * @param in 转入方
     * @param money 金额
     */
    public void transfer(String out,String in ,Double money) ;
}
@Service
public class AccountServiceImpl implements AccountService {
    @Autowired
    private AccountDao accountDao;

    public void transfer(String out,String in ,Double money) {
        accountDao.outMoney(out,money);
        accountDao.inMoney(in,money);
    }
}
```
步骤6：添加 `jdbc.properties` 文件。
```java
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://localhost:3306/spring_db?useSSL=false
jdbc.username=root
jdbc.password=root
```
步骤7：创建 `JdbcConfig` 配置类。
```java
public class JdbcConfig {
    @Value("${jdbc.driver}")
    private String driver;
    @Value("${jdbc.url}")
    private String url;
    @Value("${jdbc.username}")
    private String userName;
    @Value("${jdbc.password}")
    private String password;

    @Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName(driver);
        ds.setUrl(url);
        ds.setUsername(userName);
        ds.setPassword(password);
        return ds;
    }
}
```
步骤8：创建 `MybatisConfig` 配置类。
```java
public class MybatisConfig {

    @Bean
    public SqlSessionFactoryBean sqlSessionFactory(DataSource dataSource){
        SqlSessionFactoryBean ssfb = new SqlSessionFactoryBean();
        ssfb.setTypeAliasesPackage("com.itheima.domain");
        ssfb.setDataSource(dataSource);
        return ssfb;
    }

    @Bean
    public MapperScannerConfigurer mapperScannerConfigurer(){
        MapperScannerConfigurer msc = new MapperScannerConfigurer();
        msc.setBasePackage("com.itheima.dao");
        return msc;
    }
}
```
步骤9：创建 `SpringConfig` 配置类。
```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("classpath:jdbc.properties")
@Import({JdbcConfig.class,MybatisConfig.class})
public class SpringConfig {
}
```
步骤10：编写测试类。
```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = SpringConfig.class)
public class AccountServiceTest {

    @Autowired
    private AccountService accountService;

    @Test
    public void testTransfer() throws IOException {
        accountService.transfer("Tom","Jerry",100D);
    }

}
```
最终创建好的项目结构如下：
![img|300x450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202210576.png)
### 15.1.4 事务管理
上述环境，运行单元测试类，会执行转账操作，`Tom` 的账户会减少100，`Jerry` 的账户会加100。这是正常情况下的运行结果，但如果在转账的过程中出现了异常，如：
```java
@Service
public class AccountServiceImpl implements AccountService {
    @Autowired
    private AccountDao accountDao;
    public void transfer(String out,String in ,Double money) {
        accountDao.outMoney(out,money);
        int i = 1/0;
        accountDao.inMoney(in,money);
    }
}
```
这个时候就模拟了转账过程中出现异常的情况，正确的操作应该是转账出问题了，Tom应该还是900，Jerry应该还是1100，但真正运行后会发现，并没有像我们想象的那样，Tom账户为800而Jerry还是1100，100块钱凭空消息了，银行乐疯了。如果把转账换个顺序，银行就该哭了。

不管哪种情况，都是不允许出现的，对刚才的结果我们做一个分析：
1. 程序正常执行时，账户金额A减B加，没有问题
2. 程序出现异常后，转账失败，但异常之前操作成功，异常之后操作失败，整体业务失败

当程序出问题后，我们需要让事务进行回滚，而且这个事务应该是加在业务层上，而Spring的事务管理就是用来解决这类问题的。Spring事务管理具体的实现步骤如下。

步骤1：在需要被事务管理的方法上添加注解。
```java
public interface AccountService {
    /**
     * 转账操作
     * @param out 传出方
     * @param in 转入方
     * @param money 金额
     */
    //配置当前接口方法具有事务
    public void transfer(String out,String in ,Double money) ;
}
@Service
public class AccountServiceImpl implements AccountService {
    @Autowired
    private AccountDao accountDao;
	@Transactional
    public void transfer(String out,String in ,Double money) {
        accountDao.outMoney(out,money);
        int i = 1/0;
        accountDao.inMoney(in,money);
    }

}
```
==注意==：`@Transactional` 可以写在**接口类、接口方法、实现类和实现类方法**上。
* 写在接口类上，该接口的所有实现类的所有方法都会有事务
* 写在接口方法上，该接口的所有实现类的该方法都会有事务
* 写在实现类上，该类中的所有方法都会有事务
* 写在实现类方法上，该方法上有事务
* ==建议写在实现类或实现类的方法上==

步骤2：在 `JdbcConfig` 类中**配置事务管理器**。
```java
public class JdbcConfig {
    @Value("${jdbc.driver}")
    private String driver;
    @Value("${jdbc.url}")
    private String url;
    @Value("${jdbc.username}")
    private String userName;
    @Value("${jdbc.password}")
    private String password;

    @Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName(driver);
        ds.setUrl(url);
        ds.setUsername(userName);
        ds.setPassword(password);
        return ds;
    }

    //配置事务管理器，mybatis使用的是jdbc事务
    @Bean
    public PlatformTransactionManager transactionManager(DataSource dataSource){ // 一个Bean需要外部资源,依赖注入
        DataSourceTransactionManager transactionManager = new DataSourceTransactionManager();
        transactionManager.setDataSource(dataSource);
        return transactionManager;
    }
}
```
**注意**：事务管理器要根据使用技术进行选择，Mybatis框架使用的是JDBC事务，可以直接使用 `DataSourceTransactionManager` 。

步骤3：开启事务注解。在 `SpringConfig` 的配置类中开启：
```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("classpath:jdbc.properties")
@Import({JdbcConfig.class,MybatisConfig.class
//开启注解式事务驱动
@EnableTransactionManagement
public class SpringConfig {
}

```

步骤4：运行测试类。会发现在转换的业务出现错误后，事务就可以控制回顾，保证数据的正确性。

### 知识点1：`@EnableTransactionManagement`

| 名称 | `@EnableTransactionManagement`           |
| ---- | -------------------------------------- |
| 类型 | 配置类注解                             |
| 位置 | 配置类定义上方                         |
| 作用 | 设置当前Spring环境中开启注解式事务支持 |

### 知识点2：`@Transactional`   

| 名称 | `@Transactional`                                               |
| ---- | ------------------------------------------------------------ |
| 类型 | 接口注解  类注解  方法注解                                   |
| 位置 | 业务层接口上方  业务层实现类上方  业务方法上方               |
| 作用 | 为当前业务层方法添加事务（如果设置在类或接口上方，则类或接口中所有方法均添加事务） |

## 15.2 Spring事务角色
这节中我们重点要理解两个概念，分别是 `事务管理员` 和 `事务协调员` 。

未开启Spring事务之前：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202218428.png)
* `AccountDao` 的 `outMoney` 因为是修改操作，会开启一个事务T1
* `AccountDao` 的 `inMoney` 因为是修改操作，会开启一个事务T2
* `AccountService` 的 `transfer` 没有事务，
	* 运行过程中如果没有抛出异常，则T1和T2都正常提交，数据正确
	* 如果在两个方法中间抛出异常，T1因为执行成功提交事务，T2因为抛异常不会被执行
	* 就会导致数据出现错误

开启Spring的事务管理后：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202220676.png)
* `transfer` 上添加了 `@Transactional` 注解，在该方法上就会有一个事务T
* `AccountDao` 的 `outMoney` 方法的事务T1加入到 `transfer` 的事务T中
* `AccountDao` 的 `inMoney` 方法的事务T2加入到 `transfer` 的事务T中
* 这样就保证他们在同一个事务中，==当业务层中出现异常，整个事务就会回滚，保证数据的准确性==。

通过上面例子的分析，我们就可以得到如下概念：
- 事务管理员：发起事务方，在Spring中通常指代==业务层开启事务的方法==
- 事务协调员：加入事务方，在Spring中通常指代==数据层方法，也可以是业务层方法==

==注意==：目前的事务管理是基于 `DataSourceTransactionManager` 和`SqlSessionFactoryBean` 使用的是同一个数据源，MyBatis和Druid、Spring都用同样的 `datasource` 。
## 15.3 Spring事务属性
上一节我们介绍了两个概念，事务的管理员和事务的协同员，对于这两个概念具体做什么的，待会通过案例来使用下。除了这两个概念，还有事务的其他相关配置，就是我们接下来要学习的内容。

在这一节中，主要学习三部分内容 `事务配置` 、`转账业务追加日志` 、`事务传播行为` 。
### 15.3.1 事务配置
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202225021.png)
上面这些属性都可以在 `@Transactional` 注解的参数上进行设置。
* `readOnly`：`true` 只读事务，`false` 读写事务，增删改要设为 `false` ，查询设为 `true` 。
* `timeout` ：设置超时时间单位秒，在多长时间之内事务没有提交成功就自动回滚，-1表示不设置超时时间。
* `rollbackFor` ：**当出现指定异常进行事务回滚**
* `noRollbackFor` ：**当出现指定异常不进行事务回滚**
* `rollbackForClassName` 等同于 `rollbackFor` ，只不过属性为异常的类全名字符串
* `noRollbackForClassName` 等同于 `noRollbackFor` ，只不过属性为异常的类全名字符串
* `isolation` 设置事务的隔离级别
	* `DEFAULT`  ：默认隔离级别, 会采用数据库的隔离级别
	* `READ_UNCOMMITTED` ：读未提交
	* `READ_COMMITTED` ：读已提交
	* `REPEATABLE_READ` ：重复读取
	* `SERIALIZABLE` ：串行化
  
思考：出现异常事务会自动回滚，这个是我们之前就已经知道的
* `noRollbackFor` 是设定对于指定的异常不回滚，这个好理解
* `rollbackFor` 是指定回滚异常，对于异常事务不应该都回滚么，为什么还要指定?
* 这块需要更正一个知识点，**并不是所有的异常都会回滚事务**，比如下面的代码就不会回滚
  ```java
  public interface AccountService {
	  /**
	   * 转账操作
	   * @param out 传出方
	   * @param in 转入方
	   * @param money 金额
	   */
	  //配置当前接口方法具有事务
	  public void transfer(String out,String in ,Double money) throws IOException;
  }
  
  @Service
  public class AccountServiceImpl implements AccountService {
	  @Autowired
	  private AccountDao accountDao;
	@Transactional
	  public void transfer(String out,String in ,Double money) throws IOException{
		  accountDao.outMoney(out,money);
		  //int i = 1/0; //这个异常事务会回滚
		  if(true){
			  throw new IOException(); //这个异常事务就不会回滚
		  }
		  accountDao.inMoney(in,money);
	  }
  
  }
  ```
	出现这个问题的原因是，**Spring的事务只会对 `Error异常` 和 `RuntimeException异常` 及其子类进行事务回顾，其他的异常类型是不会回滚的**，对应 `IOException` 不符合上述条件所以不回滚  
* 此时就可以使用 `rollbackFor` 属性来设置出现 `IOException` 异常不回滚
  ```java
  @Service
  public class AccountServiceImpl implements AccountService {
	  @Autowired
	  private AccountDao accountDao;
	 @Transactional(rollbackFor = {IOException.class})
	  public void transfer(String out,String in ,Double money) throws IOException{
		  accountDao.outMoney(out,money);
		  //int i = 1/0; //这个异常事务会回滚
		  if(true){
			  throw new IOException(); //这个异常事务就不会回滚
		  }
		  accountDao.inMoney(in,money);
	  }
  
  }
  ```
介绍完上述属性后，还有最后一个事务的传播行为，为了讲解该属性的设置，我们需要完成下面的案例。

### 15.3.2 转账业务追加日志案例
#### 1. 需求分析
在前面转账案例基础上添加新的需求，完成转账后记录日志。
- 需求：实现任意两个账户间转账操作，并对每次转账操作在数据库进行留痕
- 需求微缩：A账户减钱，B账户加钱，**数据库记录日志**

基于上述的业务需求，我们来分析下该如何实现：
1. 基于转账操作案例添加日志模块，实现数据库中记录日志
2. 业务层转账操作 `transfer` ，调用减钱、加钱与记录日志功能

需要注意一点就是，我们这个案例的预期效果为：==无论转账操作是否成功，均进行转账操作的日志留痕==。

#### 2. 环境准备
该环境是基于转账环境来完成的，所以环境的准备可以参考上述环境搭建步骤，在其基础上，我们继续往下写。步骤1：创建日志表
```sql
create table tbl_log(
   id int primary key auto_increment,
   info varchar(255),
   createDate datetime
)
```
步骤2：添加 `LogDao` 接口。
```java
public interface LogDao {
    @Insert("insert into tbl_log (info,createDate) values(#{info},now())")
    void log(String info);
}

```
步骤3：添加 `LogService` 接口与实现类。
```java
public interface LogService {
    void log(String out, String in, Double money);
}
@Service
public class LogServiceImpl implements LogService {
    @Autowired
    private LogDao logDao;
	@Transactional
    public void log(String out,String in,Double money ) {
        logDao.log("转账操作由"+out+"到"+in+",金额："+money);
    }
}
```
步骤4：在转账的业务中添加记录日志。
```java
public interface AccountService {
    /**
     * 转账操作
     * @param out 传出方
     * @param in 转入方
     * @param money 金额
     */
    //配置当前接口方法具有事务
    public void transfer(String out,String in ,Double money)throws IOException ;
}
@Service
public class AccountServiceImpl implements AccountService {
    @Autowired
    private AccountDao accountDao;
    @Autowired
    private LogService logService;
	@Transactional
    public void transfer(String out,String in ,Double money) {
        try{
            accountDao.outMoney(out,money);
            accountDao.inMoney(in,money);
        } finally {
            logService.log(out,in,money);
        }
    }

}
```
步骤5：运行程序
* 当程序正常运行，`tbl_account `表中转账成功，`tbl_log` 表中日志记录成功
* 当转账业务之间出现异常（`int i =1/0`），转账失败，`tbl_account `成功回滚，但 `tbl_log` 表未添加数据
* **这个结果和我们想要的不一样**，什么原因？该如何解决？
* 失败原因：日志的记录与转账操作隶属同一个事务，同成功同失败
* 最终效果：无论转账操作是否成功，日志必须保留

### 15.3.3 事务传播行为
对于上述案例的分析：
* `log` 方法、 `inMoney` 方法和 `outMoney` 方法都属于增删改，分别有事务T1,T2,T3
* `transfer` 因为加了 `@Transactional` 注解，也开启了事务T
* 前面我们讲过Spring事务会把T1,T2,T3都加入到事务T中
* 所以当转账失败后，所有的事务都回滚，导致日志没有记录下来
* 这和我们的需求不符，这个时候我们就想能不能**让 `log` 方法单独是一个事务**呢？
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202242033.png)

要想解决这个问题，就需要用到事务传播行为，所谓的事务传播行为指的是：==事务协调员对事务管理员所携带事务的处理态度==。具体如何解决，就需要用到之前我们没有说的 `propagation属性` 。
#### 1.修改 `logService` 改变事务的传播行为
```java
@Service
public class LogServiceImpl implements LogService {
    @Autowired
    private LogDao logDao;
	//propagation设置事务属性：传播行为设置为当前操作需要新事务
    @Transactional(propagation = Propagation.REQUIRES_NEW)
    public void log(String out,String in,Double money ) {
        logDao.log("转账操作由"+out+"到"+in+",金额："+money);
    }
}
```
运行后，就能实现我们想要的结果，不管转账是否成功，都会记录日志。
#### 2.事务传播行为的可选值
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302202309383.png)
对于我们开发实际中使用的话，因为默认值需要事务是常态的。根据开发过程选择其他的就可以了，例如**案例中需要新事务就需要手工配置**。其实入账和出账操作上也有事务，采用的就是默认值。

