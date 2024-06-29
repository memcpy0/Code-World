# 1. DML编程控制
查询相关的操作我们已经介绍完了，紧接着我们需要对另外三个，增删改进行内容的讲解。挨个来说明下，首先是新增 `insert` 中的内容。
## 1.1 `id` 生成策略控制
前面我们在新增的时候留了一个问题，就是新增成功后，主键ID是一个很长串的内容，**我们更想要的是按照数据库表字段进行自增长**，在解决这个问题之前，我们先来分析下ID该如何选择：
* 不同的表应用**不同的 `id` 生成策略**
	* 日志：自增（1,2,3,4，……）
	* 购物订单：**特殊规则**（FQ23948AK3843）
	* 外卖单：关联地区日期等信息（10 04 20200314 34 91）
	* 关系表：可省略 `id`
	* ……

**不同的业务采用的ID生成方式应该是不一样的**，那么在MP中都提供了哪些主键生成策略，以及我们该如何进行选择？在这里，我们又需要用到MP的一个注解叫 `@TableId` 。
#### 知识点1：`@TableId`
| 名称     | `@TableId`                                                     |
| -------- | ------------------------------- |
| 类型     | ==属性注解==                                      |
| 位置     | 模型类中用于表示主键的属性定义上方             |
| 作用     | 设置当前类中主键属性的生成策略            |
| 相关属性 | value(默认)：设置数据库表主键名称<br/>type:设置主键属性的生成策略，值查照IdType的枚举值 |

### 1.1.1 环境构建
在构建条件查询之前，我们先来准备下环境
- 创建一个SpringBoot项目
- `pom.xml` 中添加对应的依赖
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <project xmlns="http://maven.apache.org/POM/1.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
           xsi:schemaLocation="http://maven.apache.org/POM/1.0.0 https://maven.apache.org/xsd/maven-1.0.0.xsd">
      <modelVersion>1.0.0</modelVersion>
      <parent>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-parent</artifactId>
          <version>2.5.0</version>
          <relativePath/> <!-- lookup parent from repository -->
      </parent>
      <groupId>com.itheima</groupId>
      <artifactId>mybatisplus_03_dml</artifactId>
      <version>0.0.1-SNAPSHOT</version>
      <properties>
          <java.version>1.8</java.version>
      </properties>
      <dependencies>
  
          <dependency>
              <groupId>com.baomidou</groupId>
              <artifactId>mybatis-plus-boot-starter</artifactId>
              <version>3.1.1</version>
          </dependency>
  
          <dependency>
              <groupId>org.springframework.boot</groupId>
              <artifactId>spring-boot-starter</artifactId>
          </dependency>
  
          <dependency>
              <groupId>com.alibaba</groupId>
              <artifactId>druid</artifactId>
              <version>1.1.16</version>
          </dependency>
  
          <dependency>
              <groupId>mysql</groupId>
              <artifactId>mysql-connector-java</artifactId>
              <scope>runtime</scope>
          </dependency>
  
          <dependency>
              <groupId>org.springframework.boot</groupId>
              <artifactId>spring-boot-starter-test</artifactId>
              <scope>test</scope>
          </dependency>
  
          <dependency>
              <groupId>org.projectlombok</groupId>
              <artifactId>lombok</artifactId>
              <version>1.18.12</version>
          </dependency>
      </dependencies>
  
      <build>
          <plugins>
              <plugin>
                  <groupId>org.springframework.boot</groupId>
                  <artifactId>spring-boot-maven-plugin</artifactId>
              </plugin>
          </plugins>
      </build>
  
  </project>
  
  ```
- 编写 `UserDao` 接口
  ```java
  @Mapper
  public interface UserDao extends BaseMapper<User> {
  }
  ```
- 编写模型类
  ```java
  @Data
  @TableName("tbl_user")
  public class User {
      private Long id;
      private String name;
      @TableField(value="pwd",select=false)
      private String password;
      private Integer age;
      private String tel;
      @TableField(exist=false)
      private Integer online;
  }
  ```
- 编写引导类
  ```java
  @SpringBootApplication
  public class Mybatisplus03DqlApplication {
      public static void main(String[] args) {
          SpringApplication.run(Mybatisplus03DqlApplication.class, args);
      }
  }
  ```
- 编写配置文件
  ```yml
  # dataSource
  spring:
    datasource:
      type: com.alibaba.druid.pool.DruidDataSource
      driver-class-name: com.mysql.cj.jdbc.Driver
      url: jdbc:mysql://localhost:3306/mybatisplus_db?serverTimezone=UTC
      username: root
      password: root
  # mp日志
  mybatis-plus:
    configuration:
      log-impl: org.apache.ibatis.logging.stdout.StdOutImpl
  ```
- 编写测试类
  ```java
  @SpringBootTest
  class Mybatisplus02DqlApplicationTests {
      @Autowired
      private UserDao userDao;
      @Test
      void testGetAll(){
          List<User> userList = userDao.selectList(null);
          System.out.println(userList);
      }
  }
  ```
- 测试
  ```java
  @SpringBootTest
  class Mybatisplus03DqlApplicationTests {
      @Autowired
      private UserDao userDao;
      @Test
      void testSave(){
          User user = new User();
          user.setName("黑马程序员");
          user.setPassword("itheima");
          user.setAge(12);
          user.setTel("4006184000");
          userDao.insert(user);
      }
      @Test
      void testDelete(){
          userDao.deleteById(1401856123925713409L)
      }
      @Test
      void testUpdate(){
          User user = new User();
          user.setId(3L);
          user.setName("Jock666");
          user.setVersion(1);
          userDao.updateById(user);
      }
  }
  ```

最终创建的项目结构为：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231947365.png)
### 1.1.2 代码演示
#### 1. AUTO策略
步骤1：设置生成策略为AUTO。
```java
@Data
@TableName("tbl_user")
public class User {
    @TableId(type = IdType.AUTO)
    private Long id;
    private String name;
    @TableField(value="pwd",select=false)
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
}
```
步骤2：删除测试数据并修改自增值
* 删除测试数据
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231948971.png)
* 因为之前生成主键ID的值比较长，会把MySQL的自动增长的值变的很大，所以需要将其调整为目前最新的id值。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231949643.png)

步骤3：运行新增方法  
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231949896.png)
会发现，新增成功，并且主键ID也是从5开始：

经过这三步的演示，会发现 `AUTO` 的作用是==使用数据库ID自增==，在使用该策略的时候**一定要确保对应的数据库表设置了ID主键自增**，否则无效。

接下来，我们可以进入源码查看下ID的生成策略有哪些？打开源码后，你会发现并没有看到中文注释，这就需要我们点击右上角的 `Download Sources` ，会自动帮你把这个类的Java文件下载下来，我们就能看到具体的注释内容。因为这个技术是国人制作的，所以他代码中的注释还是比较容易看懂的。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231950650.png)
当把源码下载完后，就可以看到如下内容：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232003673.png)
从源码中可以看到，除了AUTO这个策略以外，还有如下几种生成策略：
* `NONE` ： 不设置ID生成策略
* `INPUT` ：用户手工输入ID
* `ASSIGN_ID` ：雪花算法生成ID（可兼容数值型与字符串型）
* `ASSIGN_UUID` ：以UUID生成算法作为ID生成策略
* **其他的几个策略均已过时，都将被 `ASSIGN_ID` 和 `ASSIGN_UUID` 代替掉**。

**拓展**：分布式ID是什么？
* 当数据量足够大的时候，一台数据库服务器存储不下，这个时候就需要多台数据库服务器进行存储
* 比如订单表就有可能被存储在不同的服务器上
* 如果用数据库表的自增主键，因为在两台服务器上所以会出现冲突
* 这个时候就需要一个全局唯一ID，这个ID就是分布式ID。

#### 2. INPUT策略
步骤1：设置生成策略为INPUT。
```java
@Data
@TableName("tbl_user")
public class User {
    @TableId(type = IdType.INPUT)
    private Long id;
    private String name;
    @TableField(value="pwd",select=false)
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
}
```
**注意**：这种ID生成策略，需要将表的自增策略删除掉。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232004497.png)
步骤2：添加数据手动设置ID。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
	
    @Test
    void testSave(){
        User user = new User();
        //设置主键ID的值
        user.setId(666L);
        user.setName("黑马程序员");
        user.setPassword("itheima");
        user.setAge(12);
        user.setTel("4006184000");
        userDao.insert(user);
    }
}
```
步骤3：运行新增方法。如果没有设置主键ID的值，则会报错，错误提示就是主键ID没有给值： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232004979.png)
如果设置了主键ID，则数据添加成功，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232004471.png)
#### ASSIGN_ID策略
步骤1：设置生成策略为 `ASSIGN_ID` 。
```java
@Data
@TableName("tbl_user")
public class User {
    @TableId(type = IdType.ASSIGN_ID)
    private Long id;
    private String name;
    @TableField(value="pwd",select=false)
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
}
```
步骤2：添加数据不设置ID。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
	
    @Test
    void testSave(){
        User user = new User();
        user.setName("黑马程序员");
        user.setPassword("itheima");
        user.setAge(12);
        user.setTel("4006184000");
        userDao.insert(user);
    }
}
```
**注意**：==这种生成策略，不需要手动设置ID，如果手动设置ID，则会使用自己设置的值==。

步骤3：运行新增方法。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232006192.png)
生成的ID就是一个 `Long` 类型的数据。

#### ASSIGN_UUID策略
步骤1：设置生成策略为 `ASSIGN_UUID` 。使用 `uuid` 需要注意的是，**主键的类型不能是 `Long` ，而应该改成 `String` 类型**：
```java
@Data
@TableName("tbl_user")
public class User {
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;
    private String name;
    @TableField(value="pwd",select=false)
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
}
```
步骤2：修改表的主键类型。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232007408.png)
**主键类型设置为 `varchar` ，长度要大于32**，因为UUID生成的主键为32位，如果长度小的话就会导致插入失败。

步骤3：添加数据不设置ID。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
	
    @Test
    void testSave(){
        User user = new User();
        user.setName("黑马程序员");
        user.setPassword("itheima");
        user.setAge(12);
        user.setTel("4006184000");
        userDao.insert(user);
    }
}
```
步骤4：运行新增方法。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232008915.png)
接下来我们来聊一聊<font color="red"><b>雪花算法</b></font>  `Snow Flake` ，是Twitter官方给出的算法实现，是用Scala写的。其生成的结果是一个64bit大小整数，它的结构如下图:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232009169.png)
1. 1bit,不用，因为二进制中最高位是符号位，1表示负数，0表示正数。生成的id一般都是用整数，所以最高位固定为0。
2. **41bit-时间戳**，用来记录时间戳，毫秒级
3. **10bit-工作机器id**，用来记录工作机器id，其中高位5bit是数据中心ID，其取值范围0-31，低位5bit是工作节点ID，其取值范围0-31。两个组合起来最多可以容纳1024个节点
1. 序列号占用12bit，每个节点每毫秒0开始不断累加，最多可以累加到4095，一共可以产生4096个ID

### 1.1.3 ID生成策略对比
介绍了这些主键ID的生成策略，我们以后该用哪个呢？
* `NONE` ：不设置id生成策略，**MP不自动生成，约等于INPUT**。所以这两种方式都需要用户手动设置，但手动设置第一个问题是容易出现相同的ID、造成主键冲突，为了保证主键不冲突就需要做很多判定，实现起来比较复杂
* `AUTO` ：**数据库ID自增，这种策略适合在数据库服务器只有1台的情况下使用**，不可作为分布式ID使用
* `ASSIGN_UUID` ：**可以在分布式的情况下使用，而且能够保证唯一**，但生成的主键是32位的字符串，长度过长占用空间而且还不能排序，查询性能也慢
* `ASSIGN_ID` ：可以在分布式的情况下使用，生成的是 `Long` 类型的数字，可以排序性能也高，但生成的策略和服务器时间有关，如果修改了系统时间就有可能导致出现重复主键
* 综上所述，每一种主键策略都有自己的优缺点，**根据自己项目业务的实际情况来选择使用才是最明智的选择**。

### 1.1.4 简化配置
前面我们已经完成了**表关系映射、数据库主键策略的设置**，接下来对于这两个内容的使用，我们再讲下他们的简化配置。
#### 1. 模型类主键策略设置
对于主键ID的策略已经介绍完，但如果要在项目中的每一个模型类上都需要使用相同的生成策略，如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232011250.png)
确实是稍微有点繁琐，我们能不能在某一处进行配置，就能让所有的模型类都可以使用该主键ID策略呢？答案是肯定有，我们只需要在配置文件中添加如下内容
```yml
mybatis-plus:
  global-config:
    db-config:
    	id-type: assign_id
```
配置完成后，每个模型类的主键ID策略都将成为 `assign_id` 。
#### 2. 数据库表与模型类的映射关系
MP会默认将模型类的类名名首字母小写作为表名使用，假如数据库表的名称都以 `tbl_` 开头，那么**我们就需要将所有的模型类上添加 `@TableName`** ，如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232013781.png)
配置起来还是比较繁琐，简化方式为在配置文件中配置如下内容：
```yml
mybatis-plus:
  global-config:
    db-config:
    	table-prefix: tbl_
```
设置表的前缀内容，这样MP就会拿 `tbl_` 加上模型类的首字母小写，就刚好组装成数据库的表名。

---
# 2. Oracle主键Sequence
在MySQL中，主键往往是自增长的，这样使用起来是比较方便的，如果使用的是Oracle数据库，那么就不能使用自增长了，就得使用Sequence序列生成ID值了。
## 2.1 部署Oracle环境
为了简化环境部署，这里使用Docker环境进行部署安装Oracle。
```bash
#拉取镜像
$ docker pull sath89/oracle-12c
#创建容器
$ docker create --name oracle -p 1521:1521 sath89/oracle-12c
#启动
$ docker start oracle && docker logs -f oracle
#下面是启动过程
Database not initialized. Initializing database.
Starting tnslsnr
Copying database files
1% complete
3% complete
11% complete
18% complete
26% complete
37% complete
Creating and starting Oracle instance
40% complete
45% complete
50% complete
55% complete
56% complete
60% complete
62% complete
Completing Database Creation
66% complete
70% complete
73% complete
85% complete
96% complete
100% complete
Look at the log file "/u01/app/oracle/cfgtoollogs/dbca/xe/xe.log" for further details.
Configuring Apex console
Database initialized. Please visit http://#containeer:8080/em
http://#containeer:8080/apex for extra configuration if needed
Starting web management console
PL/SQL procedure successfully completed.
Starting import from '/docker-entrypoint-initdb.d':
ls: cannot access /docker-entrypoint-initdb.d/*: No such file or directory
Import finished
Database ready to use. Enjoy! ;)
#通过用户名密码即可登录
用户名和密码为： system/oracle
```
下面使用Navicat12进行连接并操作Oracle，使用资料中提供的安装包，可以试用14天。需要注意的是：由于安装的Oracle是64位版本，所以Navicat也是需要使用64为版本，否则连接不成功。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021850473.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021851641.png)
## 2.2 创建表以及序列
```sql
--创建表，表名以及字段名都要大写
CREATE TABLE "TB_USER" (
"ID" NUMBER(20) VISIBLE NOT NULL ,
"USER_NAME" VARCHAR2(255 BYTE) VISIBLE ,
"PASSWORD" VARCHAR2(255 BYTE) VISIBLE ,
"NAME" VARCHAR2(255 BYTE) VISIBLE ,
"AGE" NUMBER(10) VISIBLE ,
"EMAIL" VARCHAR2(255 BYTE) VISIBLE
)
--创建序列
CREATE SEQUENCE SEQ_USER START WITH 1 INCREMENT BY 1
```
## 2.3 JDBC驱动包
由于版权原因，我们不能直接通过Maven的中央仓库下载Oracle数据库的JDBC驱动包，所以我们需要将驱动包安装到本地仓库。
```bash
# ojdbc8.jar文件在资料中可以找到
$ mvn install:install-file -DgroupId=com.oracle -DartifactId=ojdbc8 -Dversion=12.1.0.1 -Dpackaging=jar -Dfile=ojdbc8.jar
```
安装完成后的坐标：
```xml
<dependency>
	<groupId>com.oracle</groupId>
	<artifactId>ojdbc8</artifactId>
	<version>12.1.0.1</version>
</dependency>
```
## 2.4 修改 `application.properties`
对于 `application.properties` 的修改，需要修改2个位置，分别是：
```yaml
#数据库连接配置
spring.datasource.driver-class-name=oracle.jdbc.OracleDriver
spring.datasource.url=jdbc:oracle:thin:@192.168.31.81:1521:xe
spring.datasource.username=system
spring.datasource.password=oracle
#id生成策略
mybatis-plus.global-config.db-config.id-type=input
```

## 2.5 配置序列
使用Oracle的序列需要做2件事情：第一，**需要配置MP的序列生成器到Spring容器**。
```java
package cn.itcast.mp;
import com.baomidou.mybatisplus.extension.incrementer.OracleKeyGenerator;
import com.baomidou.mybatisplus.extension.plugins.PaginationInterceptor;
import org.mybatis.spring.annotation.MapperScan;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
@Configuration
@MapperScan("cn.itcast.mp.mapper") //设置mapper接口的扫描包
public class MybatisPlusConfig {
	/**
	 * 分页插件
	 */
	@Bean
	public PaginationInterceptor paginationInterceptor() {
		return new PaginationInterceptor();
	}
	/**
	 * 序列生成器
	 */
	@Bean
	public OracleKeyGenerator oracleKeyGenerator(){
		return new OracleKeyGenerator();
	}
}
```
第二，在实体对象中指定序列的名称：
```java
@KeySequence(value = "SEQ_USER", clazz = Long.class)
public class User{
	......
}
```
## 2.6 测试
```java
package cn.itcast.mp;
import cn.itcast.mp.mapper.UserMapper;
import cn.itcast.mp.pojo.User;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
import java.util.List;
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testInsert() {
		User user = new User();
		user.setAge(20);
		user.setEmail("test@itcast.cn");
		user.setName("曹操");
		user.setUserName("caocao");
		user.setPassword("123456");
		int result = this.userMapper.insert(user); //返回的result是受影响的行数，并不是自增后的id
		System.out.println("result = " + result);
		System.out.println(user.getId()); //自增后的id会回填到对象中
	}
	@Test
	public void testSelectById() {
		User user = this.userMapper.selectById(8L);
		System.out.println(user);
	}
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021910458.png)


---
# 3. `ActiveRecord`
`ActiveRecord`（简称AR）一直广受动态语言（ PHP 、 Ruby 等）的喜爱，而 Java 作为准静态语言，对于 `ActiveRecord` 往往只能感叹其优雅，所以我们也在AR道路上进行了一定的探索。

什么是 `ActiveRecord` ？`ActiveRecord` 也属于ORM（对象关系映射）层，由Rails最早提出，遵循标准的ORM模型：==表映射到类，记录映射到对象，字段映射到对象属性==。配合遵循的命名和配置惯例，能够很大程度的快速实现模型的操作，而且简洁易懂。

`ActiveRecord` 的主要思想是：
- ==每一个数据库表对应创建一个类，通常表的每个字段在类中都有相应的 `Field` ；类的每一个对象实例对应于数据库中表的一行记录==；
- **`ActiveRecord` 同时负责把自己持久化**，**在 `ActiveRecord` 中封装了对数据库的访问**（即不用显式注入 `UserMapper` 这种类），即CURD。
- `ActiveRecord` 是一种领域模型（Domain Model），封装了部分业务逻辑。
 
在MP中，开启AR非常简单，只需要将实体对象继承 `Model` 即可。
```java
package cn.itcast.mp.pojo;
import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import com.baomidou.mybatisplus.extension.activerecord.Model;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
@Data
@NoArgsConstructor
@AllArgsConstructor
public class User extends Model<User> {
	private Long id;
	private String userName;
	private String password;
	private String name;
	private Integer age;
	private String email;
}
```
测试用例：根据主键查询。
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Test
	public void testAR() {
		User user = new User();
		user.setId(2L);
		User user2 = user.selectById();
		System.out.println(user2);
	}
}
```
运行结果如下，**不用注入 `UserMapper`** ，但底层的使用中还是要用继承了 `BaseMapper<User>` 的 `UserMapper` ，所以不能省略：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021720142.png)
更新操作：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Test
	public void testAR() {
		User user = new User();
		user.setId(8L);
		user.setAge(35);
		boolean update = user.updateById();
		System.out.println(update);
	}
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021728759.png)

删除操作：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest { 
	@Test
	public void testAR() {
		User user = new User();
		user.setId(7L);
		boolean delete = user.deleteById();
		System.out.println(delete);
	}
}
```
根据条件查询：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Test
	public void testAR() {
		User user = new User();
		QueryWrapper<User> userQueryWrapper = new QueryWrapper<>();
		userQueryWrapper.le("age","20");
		List<User> users = user.selectList(userQueryWrapper);
		for (User user1 : users) {
			System.out.println(user1);
		}
	}
}
```
---
# 4. 多记录操作
先来看下问题：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232013678.png)
之前添加了很多商品到购物车，过了几天发现这些东西又不想要了，该怎么办呢？很简单删除掉，但是一个个删除的话还是比较慢和费事的，所以一般会给用户一个批量操作，也就是前面有一个复选框，用户一次可以勾选多个也可以进行全选，然后删一次就可以将购物车清空，这个就需要用到 `批量删除` 的操作了。

具体该如何实现多条删除，我们找找对应的API方法：
```java
int deleteBatchIds(@Param(Constants.COLLECTION) Collection<? extends Serializable> idList);
```
翻译方法的字面意思为：删除（根据ID批量删除），参数是一个集合，可以存放多个 `id` 值。

需求：根据传入的 `id` 集合将数据库表中的数据删除掉。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
	
    @Test
    void testDelete(){
        //删除指定多条数据
        List<Long> list = new ArrayList<>();
        list.add(1402551342481838081L);
        list.add(1402553134049501186L);
        list.add(1402553619611430913L);
        userDao.deleteBatchIds(list);
    }
}
```
执行成功后，数据库表中的数据就会按照指定的ID进行删除。除了按照ID集合进行批量删除，也可以按照ID集合进行批量查询，还是先来看下API：
```java
List<T> selectBatchIds(@Param(Constants.COLLECTION) Collection<? extends Serializable> idList);
```
方法名称翻译为：查询（根据ID批量查询），参数是一个集合，可以存放多个ID值。

需求：根据传入的ID集合查询用户信息。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
	
    @Test
    void testGetByIds(){
        //查询指定多条数据
        List<Long> list = new ArrayList<>();
        list.add(1L);
        list.add(3L);
        list.add(4L);
        userDao.selectBatchIds(list);
    }
}
```
查询结果就会按照指定传入的ID值进行查询
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232015400.png)
## 逻辑删除
接下来要讲解是删除中比较重要的一个操作，逻辑删除。==开发系统时，有时候在实现功能时，删除操作需要实现逻辑删除==，所谓逻辑删除==就是将数据标记为删除，而并非真正的物理删除==（非DELETE操作）。**查询时需要携带状态条件，确保被标记的数据不被查询到**。这样做的目的就是避免数据被真正的删除。

先来分析下问题：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232015743.png)
* 这是一个员工和其所签的合同表，关系是一个员工可以签多个合同，是一个一（员工）对多（合同）的表
* 员工ID为1的张业绩，总共签了三个合同，如果此时他离职了，我们需要将员工表中的数据进行删除，会执行 `delete` 操作
* 如果表在设计的时候有主外键关系，那么同时也得将合同表中的前三条数据也删除掉
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232017582.png)
* 后期要统计所签合同的总金额，就会发现对不上，原因是已经将员工1签的合同信息删除掉了
* 如果只删除员工不删除合同表数据，那么合同的员工编号对应的员工信息不存在，那么就会出现垃圾数据，就会出现无主合同，根本不知道有张业绩这个人的存在
* 所以经过分析，我们不应该将表中的数据删除掉，而是需要进行保留，但是又得把离职的人和在职的人进行区分，这样就解决了上述问题，如：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232018909.png)
* 区分的方式，就是在员工表中添加一列数据 `deleted` ，如果为0说明在职员工，如果离职则将其改完1，（0和1所代表的含义是可以自定义的）

所以对于删除操作业务问题来说有：
* 物理删除：业务数据从数据库中丢弃，执行的是 `delete` 操作
* 逻辑删除：==为数据设置是否可用状态字段，删除时设置状态字段为不可用状态，数据保留在数据库中==，执行的是 `update` 操作

MP中逻辑删除具体该如何实现？

步骤1：**修改数据库表添加 `deleted` 列**。字段名可以任意，内容也可以自定义，比如0代表未删除，1代表删除，可以在添加列的同时设置其默认值为0。
![660](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232020448.png)
```sql
ALTER TABLE `tb_user`
ADD COLUMN `deleted` int(1) NULL DEFAULT 0 
	COMMENT '1代表删除，0代表未删除' AFTER `version`;
```
步骤2：实体类添加属性
(1) 添加与数据库表的列对应的一个属性名，名称可以任意，如果和数据表列名对不上，可以使用 `@TableField` 进行关系映射，如果一致，则会自动对应。

(2) **标识新增的字段为逻辑删除字段**，使用 `@TableLogic` 。
```java
@Data
//@TableName("tbl_user") 可以不写是因为配置了全局配置
public class User {
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;
    private String name;
    @TableField(value="pwd",select=false)
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
    @TableLogic(value="0",delval="1")
    //value为正常数据的值，delval为删除数据的值
    private Integer deleted;
}
```
步骤3：运行删除方法。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testDelete(){
       userDao.deleteById(1L);
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232021807.png)
从测试结果来看，**逻辑删除最后走的是 `update` 操作，会将指定的字段修改成删除状态对应的值**。

**思考**：逻辑删除，对查询有没有影响呢？
* 执行查询操作
  ```java
  @SpringBootTest
  class Mybatisplus03DqlApplicationTests {
      @Autowired
      private UserDao userDao;
  	
      @Test
      void testFind(){
         System.out.println(userDao.selectList(null));
      }
  }
  ```
  运行测试，会发现打印出来的SQL语句中会多一个查询条件，如：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232022332.png)
  可想而知，**MP的逻辑删除会将所有的查询都添加一个未被删除的条件**，也就是已经被删除的数据是不应该被查询出来的。

* 如果还是想把已经删除的数据都查询出来该如何实现呢？
  ```java
  @Mapper
  public interface UserDao extends BaseMapper<User> {
      //查询所有数据包含已经被删除的数据
      @Select("select * from tbl_user")
      public List<User> selectAll();
  }
  ```
* **如果每个表都要有逻辑删除，那么就需要在每个模型类的属性上添加 `@TableLogic` 注解**，如何优化？在配置文件中添加全局配置，如下：
  ```yml
  mybatis-plus:
    global-config:
      db-config:
        # 逻辑删除字段名
        logic-delete-field: deleted
        # 逻辑删除字面值：未删除为0
        logic-not-delete-value: 0
        # 逻辑删除字面值：删除为1
        logic-delete-value: 1
  ```

介绍完逻辑删除，逻辑删除的本质为：==**逻辑删除的本质其实是修改操作。如果加了逻辑删除字段，查询数据时也会自动带上逻辑删除字段**==。

执行的SQL语句为：`UPDATE tbl_user SET deleted=1 where id = ? AND deleted=0` 。执行数据结果为：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232026551.png)

#### 知识点1：`@TableLogic`
| 名称     | `@TableLogic`                               |
| -------- | ----------------------------------------- |
| 类型     | ==属性注解==                              |
| 位置     | 模型类中用于表示删除字段的属性定义上方    |
| 作用     | 标识该字段为进行逻辑删除的字段            |
| 相关属性 | value: 逻辑未删除值<br/>delval: 逻辑删除值 |

---
# 5. 插件
## 5.1 Mybatis的插件机制
MyBatis的插件又称为拦截器。**MyBatis允许你在「已映射语句执行过程中的某一点」进行拦截调用**。默认情况下，MyBatis允许**使用插件来拦截的方法调用**包括：
1. `Executor (update, query, flushStatements, commit, rollback, getTransaction, close, isClosed)`
2. `ParameterHandler (getParameterObject, setParameters)`
3. `ResultSetHandler (handleResultSets, handleOutputParameters)`
4. `StatementHandler (prepare, parameterize, batch, update, query)`

我们看到了，可以拦截 `Executor` 接口的部分方法，比如 `update, query, commit, rollback` 等方法，还有其他接口的一些方法等。

总体概括为：
1. 拦截**执行器的方法**
2. 拦截**参数的处理**
3. 拦截**结果集的处理**
4. 拦截**SQL语法构建的处理**

拦截器示例：
```java
package cn.itcast.mp.plugins;
import org.apache.ibatis.executor.Executor;
import org.apache.ibatis.mapping.MappedStatement;
import org.apache.ibatis.plugin.*;

import java.util.Properties;

@Intercepts({@Signature(
type = Executor.class,
method = "update",
args = {MappedStatement.class,Object.class})})
public class MyInterceptor implements Interceptor {
	@Override
	public Object intercept(Invocation invocation) throws Throwable {
		//拦截方法，具体业务逻辑编写的位置
		return invocation.proceed();
	}
	@Override
	public Object plugin(Object target) {
		//通过Plugin.wrap进行包装,创建target对象的代理对象,目的是将当前拦截器加入到该对象中
		return Plugin.wrap(target, this);
	}
	@Override
	public void setProperties(Properties properties) {
		//属性设置
	}
}
```
注入到Spring容器：
```java
/**
 * 自定义拦截器
 */
@Bean
public MyInterceptor myInterceptor() {
	return new MyInterceptor();
}
```
或者通过XML配置，`mybatis-config.xml` ：
```java
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
	PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
	"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
	<plugins>
		<plugin interceptor="cn.itcast.mp.plugins.MyInterceptor"></plugin>
	</plugins>
</configuration>
```
## 5.2 执行分析插件
在MP中提供了对SQL执行的分析插件，可用作阻断全表更新、删除的操作，注意：**该插件仅适用于开发环境，不适用于生产环境**。

SpringBoot配置：
```java
@Bean
public SqlExplainInterceptor sqlExplainInterceptor(){
	SqlExplainInterceptor sqlExplainInterceptor = new SqlExplainInterceptor();
	List<ISqlParser> sqlParserList = new ArrayList<>();
	// 攻击 SQL 阻断解析器、加入解析链
	sqlParserList.add(new BlockAttackSqlParser());
	sqlExplainInterceptor.setSqlParserList(sqlParserList);
	return sqlExplainInterceptor;
}
```
测试：
```java
@Test
public void testUpdate(){
	User user = new User();
	user.setAge(20);
	int result = this.userMapper.update(user, null);
	System.out.println("result = " + result);
}
```
结果如下：
```java
Caused by: com.baomidou.mybatisplus.core.exceptions.MybatisPlusException: Prohibition
of table update operation
	at com.baomidou.mybatisplus.core.toolkit.ExceptionUtils.mpe(ExceptionUtils.java:49)
	at com.baomidou.mybatisplus.core.toolkit.Assert.isTrue(Assert.java:38)
	at com.baomidou.mybatisplus.core.toolkit.Assert.notNull(Assert.java:72)
	at com.baomidou.mybatisplus.extension.parsers.BlockAttackSqlParser.processUpdate(BlockAttackSqlParser.java:45)
	at com.baomidou.mybatisplus.core.parser.AbstractJsqlParser.processParser(AbstractJsqlParser.java:92)
	at com.baomidou.mybatisplus.core.parser.AbstractJsqlParser.parser(AbstractJsqlParser.java:67)
	at com.baomidou.mybatisplus.extension.handlers.AbstractSqlParserHandler.sqlParser(AbstractSqlParserHandler.java:76)
	at com.baomidou.mybatisplus.extension.plugins.SqlExplainInterceptor.intercept(SqlExplainInterceptor.java:63)
	at org.apache.ibatis.plugin.Plugin.invoke(Plugin.java:61)
	at com.sun.proxy.$Proxy70.update(Unknown Source)
	at org.apache.ibatis.session.defaults.DefaultSqlSession.update(DefaultSqlSession.java:197
)
... 41 more
```
可以看到，当执行全表更新时，会抛出异常，这样有效防止了一些误操作。
## 5.3 性能分析插件
性能分析拦截器，用于输出每条SQL语句及其执行时间，可以设置最大执行时间，超过时间会抛出异常。该插件只用于开发环境，不建议生产环境使用。

配置：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
	PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
	"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
	<plugins>
		<!-- SQL 执行性能分析，开发环境使用，线上不推荐。 maxTime 指的是 sql 最大执行时长 -->
		<plugin
		interceptor="com.baomidou.mybatisplus.extension.plugins.PerformanceInterceptor">
			<!-- 最大执行时间,默认是毫秒 -->
			<property name="maxTime" value="100" />
			<!--SQL是否格式化 默认false-->
			<property name="format" value="true" />
		</plugin>
	</plugins>
</configuration>
```
执行结果：
```java
Time：11 ms - ID：cn.itcast.mp.mapper.UserMapper.selectById
Execute SQL：
	SELECT
		id,
		user_name,
		password,
		name,
		age,
		email
	FROM
		tb_user
	WHERE
		id=7
```
可以看到，执行时间为11ms。如果将 `maxTime` 设置为1，那么，该操作会抛出异常。
```java
Caused by: com.baomidou.mybatisplus.core.exceptions.MybatisPlusException: The SQL
execution time is too large, please optimize !
	at com.baomidou.mybatisplus.core.toolkit.ExceptionUtils.mpe(ExceptionUtils.java:49)
	at com.baomidou.mybatisplus.core.toolkit.Assert.isTrue(Assert.java:38)
	................
```
## 3.4 乐观锁插件
### 3.4.1 概念
在讲解乐观锁之前，我们还是先来分析下问题：业务并发现象带来的问题——==秒杀==。
* 假如有100个商品或者票在出售，==为了能保证每个商品或者票只能被一个人购买，如何保证不会出现超买或者重复卖==
* 对于这一类问题，其实有很多的解决方案可以使用
* 第一个最先想到的就是锁，==锁在一台服务器中是可以解决的，但如果在多台服务器下锁就没有办法控制==，比如12306有两台服务器在进行卖票，在两台服务器上都添加锁的话，那也有可能会导致在同一时刻有两个线程在进行卖票，还是会出现并发问题
* 我们接下来介绍的这种方式是==针对于小型企业的解决方案，因为数据库本身的性能就是个瓶颈==，如果对其并发量超过2000以上的、就需要考虑其他的解决方案了。

简单来说，**乐观锁主要解决的问题是当要更新一条记录的时候，希望这条记录没有在被别人更新**，只能有一个线程更新数据。
### 3.4.2 实现思路
乐观锁的实现方式：
> * 数据库表中添加 `version` 列，比如默认值给1
> * 第一个线程要修改数据之前，**取出记录时**，获取当前数据库中的 `version=1`
> * 第二个线程要修改数据之前，**取出记录时**，获取当前数据库中的 `version=1`
> * 第一个线程执行更新时，`set version = newVersion where version = oldVersion`
> 	 * `newVersion = version+1`  [2]
> 	 * `oldVersion = version`  [1]
> * 第二个线程执行更新时，`set version = newVersion where version = oldVersion`
> 	 * `newVersion = version+1`  [2]
> 	 * `oldVersion = version`  [1]
> * 假如这两个线程都来更新数据，**第一个和第二个线程都可能先执行**
> 	* 假如第一个线程先执行更新，会把 `version` 改为2，
> 		* 第二个线程再更新的时候，`set version = 2 where version = 1` ，此时数据库表的数据 `version` 已经为2，所以第二个线程会修改失败
> 	* 假如第二个线程先执行更新，会把 version 改为2，
> 		* 第一个线程再更新的时候，`set version = 2 where version = 1` ，此时数据库表的数据 `version` 已经为2，所以第一个线程会修改失败
> 	 * ==不管谁先执行都会确保**只能有一个线程更新数据**，这就是MP提供的乐观锁的实现原理分析==。

上面所说的步骤具体该如何实现呢？
### 3.4.3 实现步骤
分析完步骤后，具体的实现步骤如下。

步骤1：数据库表添加列。列名可以任意，比如使用 `version` ，并且设置初始值为1。
```sql
ALTER TABLE `tb_user`
ADD COLUMN `version` int(10) NULL AFTER `email`;
UPDATE `tb_user` SET `version`='1';
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232032761.png)
步骤2：**在模型类中添加对应的属性，需要为实体字段添加 `@Version` 注解**。根据添加的字段列名，在模型类中添加对应的属性值：
```java
@Data
//@TableName("tbl_user") 可以不写是因为配置了全局配置
public class User {
    @TableId(type = IdType.ASSIGN_UUID)
    private String id;
    private String name;
    @TableField(value="pwd",select=false)
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
    private Integer deleted;
    @Version
    private Integer version;
}
```
步骤3：**添加乐观锁的拦截器**（SpringBoot）。
```java
@Configuration
public class MpConfig {
    @Bean
    public MybatisPlusInterceptor mpInterceptor() {
        //1.定义Mp拦截器
        MybatisPlusInterceptor mpInterceptor = new MybatisPlusInterceptor();
        //2.添加乐观锁拦截器
        mpInterceptor.addInnerInterceptor(new OptimisticLockerInnerInterceptor());
        return mpInterceptor;
    }
}
```
> 如果是Spring的配置，`spring.xml` ：
> ```xml
>  <bean class="com.baomidou.mybatisplus.extension.plugins.OptimisticLockerInterceptor"/>
> ```
> 第三种是MyBatis全局配置文件。

步骤4：执行更新操作。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
	
    @Test
    void testUpdate(){
       User user = new User();
        user.setId(3L);
        user.setName("Jock666");
        userDao.updateById(user);
    }
}
```
![550](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232035998.png)
你会发现，**这次修改并没有更新 `version` 字段，原因是没有携带 `version` 数据**。添加 `version` 数据：
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testUpdate(){
        User user = new User();
        user.setId(3L);
        user.setName("Jock666");
        user.setVersion(1);
        userDao.updateById(user);
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232036167.png)
你会发现，我们传递的是1，MP会将1进行加1（`2(Integer)`），然后，更新回到数据库表中。

对另一测试用例：
```java
@Test
public void testUpdate(){
	User user = new User();
	user.setAge(30);
	user.setId(2L);
	user.setVersion(1); //获取到version为1
	int result = this.userMapper.updateById(user);
	System.out.println("result = " + result);
}
```
执行日志如下，可以看到，更新的条件中有 `version` 条件，并且更新的 `version` 为2。**如果再次执行，更新则不成功**。这样就**避免了多人同时更新时导致数据的不一致**。
```xml
main] [com.baomidou.mybatisplus.extension.parsers.BlockAttackSqlParser]-[DEBUG]
	Original SQL: UPDATE tb_user SET age=?, version=? WHERE id=? AND version=?
[main] [com.baomidou.mybatisplus.extension.parsers.BlockAttackSqlParser]-[DEBUG]
	parser sql: UPDATE tb_user SET age = ?, version = ? WHERE id = ? AND version = ?
[main] [org.springframework.jdbc.datasource.DataSourceUtils]-[DEBUG] Fetching JDBC
	Connection from DataSource
[main] [org.mybatis.spring.transaction.SpringManagedTransaction]-[DEBUG] JDBC
	Connection [HikariProxyConnection@540206885 wrapping
	com.mysql.jdbc.JDBC4Connection@27e0f2f5] will not be managed by Spring
[main] [cn.itcast.mp.mapper.UserMapper.updateById]-[DEBUG] ==> Preparing: UPDATE
	tb_user SET age=?, version=? WHERE id=? AND version=?
[main] [cn.itcast.mp.mapper.UserMapper.updateById]-[DEBUG] ==> Parameters:
	30(Integer), 2(Integer), 2(Long), 1(Integer)
[main] [cn.itcast.mp.mapper.UserMapper.updateById]-[DEBUG] <== Updates: 1
[main] [org.mybatis.spring.SqlSessionUtils]-[DEBUG] Closing non transactional
	SqlSession [org.apache.ibatis.session.defaults.DefaultSqlSession@30135202]
	result = 1
```
所以**要想使用乐观锁机制，在修改前，首先第一步应该是拿到表中对应数据的 `version` ，然后拿 `version` 当条件、再将 `version` 加1更新回到数据库表中**，所以我们在查询的时候，需要对其进行查询：
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testUpdate() {
        //1.先通过要修改的数据id将当前数据查询出来
        User user = userDao.selectById(3L);
        //2.将要修改的属性逐一设置进去
        user.setName("Jock888");
        userDao.updateById(user);
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232036998.png)

大概分析完乐观锁的实现步骤以后，我们来模拟一种加锁的情况，==看看能不能实现多个人修改同一个数据的时候，只能有一个人修改成功==。
```java
@SpringBootTest
class Mybatisplus03DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testUpdate(){
       //1.先通过要修改的数据id将当前数据查询出来
        User user = userDao.selectById(3L);     //version=3
        User user2 = userDao.selectById(3L);    //version=3
        user2.setName("Jock aaa");
        userDao.updateById(user2);              //version=>4
        user.setName("Jock bbb");
        userDao.updateById(user);               //verion=3?条件还成立吗？
    }
}
```
运行程序，分析结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232037403.png)
特别说明：
- 支持的数据类型只有 `int,Integer,long,Long,Date,Timestamp,LocalDateTime`  。
- 整数类型下 `newVersion = oldVersion + 1` ，`newVersion` 会回写到 `entity` 中。
- 仅支持 `updateById(id)` 与 `update(entity, wrapper)` 方法
- 在 `update(entity, wrapper)` 方法下，`wrapper` 不能复用！

乐观锁就已经实现完成了，如果对于上面的这些步骤记不住咋办呢？参考官方文档来实现：`https://mp.baomidou.com/guide/interceptor-optimistic-locker.html#optimisticlockerinnerinterceptor` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232037021.png)

---
# 4. SQL注入器
我们已经知道，**在MP中，通过 `AbstractSqlInjector` 将 `BaseMapper` 中的方法注入到了Mybatis容器，这样这些方法才可以正常执行**。那么，如果我们需要扩充 `BaseMapper` 中的方法，又该如何实现呢？

下面我们以扩展 `findAll` 方法为例进行学习。
## 4.1 编写 `MyBaseMapper`
其他的Mapper都可以继承该Mapper，这样实现了统一的扩展。
```java
package cn.itcast.mp.mapper;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import java.util.List;
public interface MyBaseMapper<T> extends BaseMapper<T> {
	List<T> findAll();
}
```
如：
```java
package cn.itcast.mp.mapper;
import cn.itcast.mp.pojo.User;
public interface UserMapper extends MyBaseMapper<User> {
	User findById(Long id);
}
```
## 4.2 编写 `MySqlInjector`
**如果直接继承 `AbstractSqlInjector` 的话，原有的 `BaseMapper` 中的方法将失效**。原因是，目前Spring容器中已经有了一个SQL注入器，之前默认的SQL注入器 `DefaultSqlInjector` 就不再注入Spring容器了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303030146207.png)
而这个类会注入 `BaseMapper` 中的一系列方法，这些方法现在都会失效：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303030147083.png)

所以我们不应该直接继承 `AbstractSqlInjector` ，而应选择继承 `DefaultSqlInjector` 进行扩展。
```java
package cn.itcast.mp.sqlInjector;
import com.baomidou.mybatisplus.core.injector.AbstractMethod;
import com.baomidou.mybatisplus.core.injector.DefaultSqlInjector;
import java.util.List;
public class MySqlInjector extends DefaultSqlInjector {
	@Override
	public List<AbstractMethod> getMethodList() {
		// 获取父类DefaultSqlInjector中的方法列表集合
		List<AbstractMethod> methodList = super.getMethodList();
		// 再扩充自定义的方法
		methodList.add(new FindAll());
		// list.add(new FindAll());
		return methodList;
	}
}
```
## 4.3 编写 `FindAll`
```java
package cn.itcast.mp.sqlInjector;
import com.baomidou.mybatisplus.core.enums.SqlMethod;
import com.baomidou.mybatisplus.core.injector.AbstractMethod;
import com.baomidou.mybatisplus.core.metadata.TableInfo;
import org.apache.ibatis.mapping.MappedStatement;
import org.apache.ibatis.mapping.SqlSource;
public class FindAll extends AbstractMethod {
	@Override
	public MappedStatement injectMappedStatement(Class<?> mapperClass, Class<?> modelClass, TableInfo tableInfo) {
		String sqlMethod = "findAll";
		String sql = "select * from " + tableInfo.getTableName();
		SqlSource sqlSource = languageDriver.createSqlSource(configuration, sql, modelClass);
		return this.addSelectMappedStatement(mapperClass, sqlMethod, sqlSource, modelClass, tableInfo);
	}
}
```
## 4.4 注册到Spring容器
```java
/**
* 自定义SQL注入器
*/
@Bean
public MySqlInjector mySqlInjector(){
	return new MySqlInjector();
}
```
## 4.5 测试
```java
@Test
public void testFindAll(){
	List<User> users = this.userMapper.findAll();
	for (User user : users) {
		System.out.println(user);
	}
}
```
输出的SQL：
```java
[main] [cn.itcast.mp.mapper.UserMapper.findAll]-[DEBUG] ==> Preparing: select * from tb_user
[main] [cn.itcast.mp.mapper.UserMapper.findAll]-[DEBUG] ==> Parameters:
[main] [cn.itcast.mp.mapper.UserMapper.findAll]-[DEBUG] <== Total: 10
```
至此，我们实现了全局扩展SQL注入器。

---
# 5. 自动填充功能
有些时候我们可能会有这样的需求，**插入或者更新数据时，希望有些字段可以自动填充数据**，比如密码、`version` 等。在MP中提供了这样的功能，可以实现自动填充。
## 5.1 添加 `@TableField` 注解
为 `password` 添加自动填充功能，在新增数据时有效。
```java
@TableField(fill = FieldFill.INSERT) //插入数据时进行填充
private String password;
```
`FieldFill` 提供了多种模式选择：
```java
public enum FieldFill {
	/**
	 * 默认不处理
	 */
	DEFAULT,
	/**
	 * 插入时填充字段
	 */
	INSERT,
	/**
	 * 更新时填充字段
	 */
	UPDATE,
	/**
	 * 插入和更新时填充字段
	 */
	INSERT_UPDATE
}
```
## 5.2 编写 `MyMetaObjectHandler`
```java
package cn.itcast.mp.handler;
import com.baomidou.mybatisplus.core.handlers.MetaObjectHandler;
import org.apache.ibatis.reflection.MetaObject;
import org.springframework.stereotype.Component;
@Component
public class MyMetaObjectHandler implements MetaObjectHandler {
	@Override
	public void insertFill(MetaObject metaObject) {
		Object password = getFieldValByName("password", metaObject);
		if(null == password){
			//字段为空，可以进行填充
			setFieldValByName("password", "123456", metaObject);
		}
	}
	@Override
	public void updateFill(MetaObject metaObject) {
	}
}
```
测试：
```java
@Test
public void testInsert(){
	User user = new User();
	user.setName("关羽");
	user.setUserName("guanyu");
	user.setAge(30);
	user.setEmail("guanyu@itast.cn");
	user.setVersion(1);
	int result = this.userMapper.insert(user);
	System.out.println("result = " + result);
}
```
结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303030138528.png)

---
# 6. 通用枚举
解决繁琐的配置，让Mybatis优雅的使用枚举属性！
## 6.1 修改表结构
```java
ALTER TABLE `tb_user`
ADD COLUMN `sex` int(1) NULL DEFAULT 1 COMMENT '1-男，2-女' AFTER `deleted`;
```
## 6.2 定义枚举
```java
package cn.itcast.mp.enums;
import com.baomidou.mybatisplus.core.enums.IEnum;
import com.fasterxml.jackson.annotation.JsonValue;

public enum SexEnum implements IEnum<Integer> {
	MAN(1,"男"),
	WOMAN(2,"女");
	private int value;
	private String desc;
	SexEnum(int value, String desc) {
		this.value = value;
		this.desc = desc;
	}
	@Override
	public Integer getValue() {
		return this.value;
	}
	@Override
	public String toString() {
		return this.desc;
	}
}
```
## 7.3 配置
在 `application.properties` 中配置枚举类型的包扫描：
```java
# 枚举包扫描
mybatis-plus.type-enums-package=cn.itcast.mp.enums
```
## 7.4 修改实体
```java
private SexEnum sex;
```
## 7.5 测试
测试插入数据：
```java
@Test
public void testInsert(){
	User user = new User();
	user.setName("貂蝉");
	user.setUserName("diaochan");
	user.setAge(20);
	user.setEmail("diaochan@itast.cn");
	user.setVersion(1);
	user.setSex(SexEnum.WOMAN);
	int result = this.userMapper.insert(user);
	System.out.println("result = " + result);
}
```
SQL：
```java
[main] [cn.itcast.mp.mapper.UserMapper.insert]-[DEBUG] ==> Preparing: INSERT INTO tb_user ( user_name, password, name, age, email, version, sex ) VALUES ( ?, ?, ?, ?, ?, ?, ? )
[main] [cn.itcast.mp.mapper.UserMapper.insert]-[DEBUG] ==> Parameters:
diaochan(String), 123456(String), 貂蝉(String), 20(Integer), diaochan@itast.cn(String), 
1(Integer), 2(Integer)
[main] [cn.itcast.mp.mapper.UserMapper.insert]-[DEBUG] <== Updates: 1
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303030203997.png)

查询：
```java
@Test
public void testSelectById(){
	User user = this.userMapper.selectById(2L);
	System.out.println(user);
}
```
结果：
```java
[main] [cn.itcast.mp.mapper.UserMapper.selectById]-[DEBUG] ==> Preparing: SELECT id,user_name,password,name,age,email,version,deleted,sex FROM tb_user WHERE id=? AND deleted=0
[main] [cn.itcast.mp.mapper.UserMapper.selectById]-[DEBUG] ==> Parameters: 2(Long)
[main] [cn.itcast.mp.mapper.UserMapper.selectById]-[DEBUG] <== Total: 1
User(id=2, userName=lisi, password=123456, name=李四, age=30, email=test2@itcast.cn, address=null, version=2, deleted=0, sex=女)
```
从测试可以看出，可以很方便的使用枚举了。

条件查询时也是有效的：
```java
@Test
public void testSelectBySex() {
	QueryWrapper<User> wrapper = new QueryWrapper<>();
	wrapper.eq("sex", SexEnum.WOMAN);
	List<User> users = this.userMapper.selectList(wrapper);
	for (User user : users) {
		System.out.println(user);
	}
}
```
SQL：
```java
[main] [cn.itcast.mp.mapper.UserMapper.selectList]-[DEBUG] ==> Preparing: SELECT id,user_name,password,name,age,email,version,deleted,sex FROM tb_user WHERE deleted=0 AND sex = ?
[main] [cn.itcast.mp.mapper.UserMapper.selectList]-[DEBUG] ==> Parameters: 2(Integer)
[main] [cn.itcast.mp.mapper.UserMapper.selectList]-[DEBUG] <== Total: 3
```

---
# 8. 快速开发
## 8.1 代码生成器原理分析
造句：`______正________`
我们可以往空白内容进行填词造句，比如 `警察正指挥交通` ，比如 `农民正忙着润到城市` .

观察我们之前写的代码，会发现其中也会有很多重复内容，比如： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232039681.png)
那我们就想，如果我想做一个 `Book` 模块的开发，是不是只需要将红色部分的内容全部更换成 `Book` 即可，如：
![550](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232040088.png)
所以我们会发现，做任何模块的开发，对于这段代码，基本上都是对红色部分的调整，所以我们把去掉红色内容的东西称之为==模板==，红色部分称之为==参数==，以后只需要传入不同的参数，就可以根据模板创建出不同模块的 `dao` 代码。

**除了 `Dao` 可以抽取模块，其实我们常见的类都可以进行抽取，只要他们有公共部分即可**。再来看下模型类的模板：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232040937.png)
* ① 可以根据数据库表的表名来填充
* ② 可以根据用户的配置来生成ID生成策略
* ③到⑨可以根据数据库表字段名称来填充

所以==只要我们知道是对哪张表进行代码生成，这些内容我们都可以进行填充==。

分析完后，我们会发现，要想完成代码自动生成，我们需要有以下内容：
* 模板：**MyBatisPlus提供**，可以自己提供，但是麻烦，不建议
* 数据库相关配置：读取数据库获取表和字段信息
* 开发者自定义配置：**手工配置**，比如ID生成策略

## 8.2 提供的代码生成器
`AutoGenerator `是MyBatis-Plus的代码生成器，通过 `AutoGenerator `可以快速生成Entity、Mapper、Mapper XML、Service、Controller等各个模块的代码，极大的提升了开发效率。效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303030216629.png)

## 8.3 代码生成器实现
步骤1：创建一个Maven项目
代码2：导入对应的JAR包
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/1.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/1.0.0 https://maven.apache.org/xsd/maven-1.0.0.xsd">
    <modelVersion>1.0.0</modelVersion>
    <parent>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-parent</artifactId>
        <version>2.5.1</version>
    </parent>
    
    <groupId>com.itheima</groupId>
    <artifactId>mybatisplus_04_generator</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <properties>
        <java.version>1.8</java.version>
    </properties>
    
    <dependencies>
        <!--spring webmvc-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>

        <!--mybatisplus-->
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus-boot-starter</artifactId>
            <version>3.1.1</version>
        </dependency>

        <!--druid-->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>druid</artifactId>
            <version>1.1.16</version>
        </dependency>

        <!--mysql-->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <scope>runtime</scope>
        </dependency>

        <!--test-->
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
            <scope>test</scope>
        </dependency>

        <!--lombok-->
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.18.12</version>
        </dependency>

        <!--代码生成器-->
        <dependency>
            <groupId>com.baomidou</groupId>
            <artifactId>mybatis-plus-generator</artifactId>
            <version>3.1.1</version>
        </dependency>

        <!--velocity模板引擎-->
        <dependency>
            <groupId>org.apache.velocity</groupId>
            <artifactId>velocity-engine-core</artifactId>
            <version>2.3</version>
        </dependency>
    </dependencies>

    <build>
        <plugins>
            <plugin>
                <groupId>org.springframework.boot</groupId>
                <artifactId>spring-boot-maven-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>
```
步骤3：编写引导类。
```java
@SpringBootApplication
public class Mybatisplus04GeneratorApplication {
    public static void main(String[] args) {
        SpringApplication.run(Mybatisplus04GeneratorApplication.class, args);
    }
}
```
步骤4：创建代码生成类（发现这个版本下没有ASSIGN_ID，更高版本的AutoGenerator()是私有，而且使用新的代码生成器，Builder模式）。
```java
public class CodeGenerator {
    public static void main(String[] args) {
        //1.获取代码生成器的对象
        AutoGenerator autoGenerator = new AutoGenerator();

        //设置数据库相关配置
        DataSourceConfig dataSource = new DataSourceConfig();
        dataSource.setDriverName("com.mysql.cj.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/mybatisplus_db?serverTimezone=UTC");
        dataSource.setUsername("root");
        dataSource.setPassword("root");
        autoGenerator.setDataSource(dataSource);

        //设置全局配置
        GlobalConfig globalConfig = new GlobalConfig();
        globalConfig.setOutputDir(System.getProperty("user.dir")+"/mybatisplus_04_generator/src/main/java");    //设置代码生成位置
        globalConfig.setOpen(false);    //设置生成完毕后是否打开生成代码所在的目录
        globalConfig.setAuthor("黑马程序员");    //设置作者
        globalConfig.setFileOverride(true);     //设置是否覆盖原始生成的文件
        globalConfig.setMapperName("%sDao");    //设置数据层接口名，%s为占位符，指代模块名称
        globalConfig.setIdType(IdType.ASSIGN_ID);   //设置Id生成策略
        autoGenerator.setGlobalConfig(globalConfig);

        //设置包名相关配置
        PackageConfig packageInfo = new PackageConfig();
        packageInfo.setParent("com.aaa");   //设置生成的包名，与代码所在位置不冲突，二者叠加组成完整路径
        packageInfo.setEntity("domain");    //设置实体类包名
        packageInfo.setMapper("dao");   //设置数据层包名
        autoGenerator.setPackageInfo(packageInfo);

        //策略设置
        StrategyConfig strategyConfig = new StrategyConfig();
        strategyConfig.setInclude("tbl_user");  //设置当前参与生成的表名，参数为可变参数
        strategyConfig.setTablePrefix("tbl_");  //设置数据库表的前缀名称，模块名 = 数据库表名 - 前缀名  例如： User = tbl_user - tbl_
        strategyConfig.setRestControllerStyle(true);    //设置是否启用Rest风格
        strategyConfig.setVersionFieldName("version");  //设置乐观锁字段名
        strategyConfig.setLogicDeleteFieldName("deleted");  //设置逻辑删除字段名
        strategyConfig.setEntityLombokModel(true);  //设置是否启用lombok
        autoGenerator.setStrategy(strategyConfig);
        //2.执行生成操作
        autoGenerator.execute();
    }
}
```
或者另一个代码：
```java
package cn.itcast.mp.generator;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import com.baomidou.mybatisplus.core.exceptions.MybatisPlusException;
import com.baomidou.mybatisplus.core.toolkit.StringPool;
import com.baomidou.mybatisplus.core.toolkit.StringUtils;
import com.baomidou.mybatisplus.generator.AutoGenerator;
import com.baomidou.mybatisplus.generator.InjectionConfig;
import com.baomidou.mybatisplus.generator.config.DataSourceConfig;
import com.baomidou.mybatisplus.generator.config.FileOutConfig;
import com.baomidou.mybatisplus.generator.config.GlobalConfig;
import com.baomidou.mybatisplus.generator.config.PackageConfig;
import com.baomidou.mybatisplus.generator.config.StrategyConfig;
import com.baomidou.mybatisplus.generator.config.TemplateConfig;
import com.baomidou.mybatisplus.generator.config.po.TableInfo;
import com.baomidou.mybatisplus.generator.config.rules.NamingStrategy;
import com.baomidou.mybatisplus.generator.engine.FreemarkerTemplateEngine;
/**
 * <p>
 * mysql 代码生成器演示例子
 * </p>
 */
public class MysqlGenerator {
	/**
	 * <p>
	 * 读取控制台内容
	 * </p>
	 */
	public static String scanner(String tip) {
		Scanner scanner = new Scanner(System.in);
		StringBuilder help = new StringBuilder();
		help.append("请输入" + tip + "：");
		System.out.println(help.toString());
		if (scanner.hasNext()) {
			String ipt = scanner.next();
			if (StringUtils.isNotEmpty(ipt)) {
				return ipt;
			}
		}
		throw new MybatisPlusException("请输入正确的" + tip + "！");
	}
	/**
	 * RUN THIS
	 */
	public static void main(String[] args) {
		// 代码生成器
		AutoGenerator mpg = new AutoGenerator();
		// 全局配置
		GlobalConfig gc = new GlobalConfig();
		String projectPath = System.getProperty("user.dir");
		gc.setOutputDir(projectPath + "/src/main/java");
		gc.setAuthor("itcast");
		gc.setOpen(false);
		mpg.setGlobalConfig(gc);
		// 数据源配置
		DataSourceConfig dsc = new DataSourceConfig();
		dsc.setUrl("jdbc:mysql://127.0.0.1:3306/mp?
		useUnicode=true&useSSL=false&characterEncoding=utf8");
		// dsc.setSchemaName("public");
		dsc.setDriverName("com.mysql.jdbc.Driver");
		dsc.setUsername("root");
		dsc.setPassword("root");
		mpg.setDataSource(dsc);
		// 包配置
		PackageConfig pc = new PackageConfig();
		pc.setModuleName(scanner("模块名"));
		pc.setParent("cn.itcast.mp.generator");
		mpg.setPackageInfo(pc);
		// 自定义配置
		InjectionConfig cfg = new InjectionConfig() {
			@Override
			public void initMap() {
				// to do nothing
			}
		};
		List<FileOutConfig> focList = new ArrayList<>();
		focList.add(new FileOutConfig("/templates/mapper.xml.ftl") {
			@Override
			public String outputFile(TableInfo tableInfo) {
				// 自定义输入文件名称
				return projectPath + "/itcast-mp-generator/src/main/resources/mapper/" + pc.getModuleName()
		+ "/" + tableInfo.getEntityName() + "Mapper" + StringPool.DOT_XML;
			}
		});
		cfg.setFileOutConfigList(focList);
		mpg.setCfg(cfg);
		mpg.setTemplate(new TemplateConfig().setXml(null));
		// 策略配置
		StrategyConfig strategy = new StrategyConfig();
		strategy.setNaming(NamingStrategy.underline_to_camel);
		strategy.setColumnNaming(NamingStrategy.underline_to_camel);
		//
		strategy.setSuperEntityClass("com.baomidou.mybatisplus.samples.generator.common.BaseEntity");
		strategy.setEntityLombokModel(true);
		strategy.setSuperControllerClass("com.baomidou.mybatisplus.samples.generator.common.BaseController");
		strategy.setInclude(scanner("表名"));
		strategy.setSuperEntityColumns("id");
		strategy.setControllerMappingHyphenStyle(true);
		strategy.setTablePrefix(pc.getModuleName() + "_");
		mpg.setStrategy(strategy);
		// 选择 freemarker 引擎需要指定如下加，注意 pom 依赖必须有！
		mpg.setTemplateEngine(new FreemarkerTemplateEngine());
		mpg.execute();
	}
}
```
对于代码生成器中的代码内容，我们可以直接从官方文档中获取代码进行修改，`https://mp.baomidou.com/guide/generator.html` 。

步骤5：运行程序。运行成功后，会在当前项目中生成很多代码，代码包含`controller, service, mapper, entity` 。
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302232042045.png)
至此代码生成器就已经完成工作，我们能快速根据数据库表来创建对应的类，简化我们的代码开发。
## 8.3 MP中Service的CRUD
回顾我们之前业务层代码的编写，编写接口和对应的实现类：
```java
public interface UserService {

}
@Service
public class UserServiceImpl implements UserService{

}
```
接口和实现类有了以后，需要在接口和实现类中声明方法：
```java
public interface UserService {
	public List<User> findAll();
}

@Service
public class UserServiceImpl implements UserService{
    @Autowired
    private UserDao userDao;
    
	public List<User> findAll(){
        return userDao.selectList(null);
    }
}
```
MP看到上面的代码以后就说这些方法也是比较固定和通用的，那我来帮你抽取下，所以**MP提供了一个Service接口和实现类**，分别是 `IService` 和 `ServiceImpl` ，后者是对前者的一个具体实现。

以后我们自己写的Service就可以进行如下修改：
```java
public interface UserService extends IService<User>{
	
}

@Service
public class UserServiceImpl extends ServiceImpl<UserDao, User> implements UserService{

}
```
修改以后的好处是，==MP已经帮我们把业务层的一些基础的增删改查都已经实现了，可以直接进行使用==。虽然不怎么常用这一部分。

编写测试类进行测试：
```java
@SpringBootTest
class Mybatisplus04GeneratorApplicationTests {

    private IUserService userService;

    @Test
    void testFindAll() {
        List<User> list = userService.list();
        System.out.println(list);
    }

}
```
**注意**：**`mybatisplus_04_generator` 项目中对于MyBatis的环境没有进行配置**，如果想要运行，需要提取将配置文件中的内容进行完善后在运行。

思考：在MP封装的Service层都有哪些方法可以用？查看官方文档 `https://mp.baomidou.com/guide/crud-interface.html` ，这些提供的方法大家可以参考官方文档进行学习使用，方法的名称可能有些变化，但是方法对应的参数和返回值基本类似。

---
# 9. MybatisX快速开发插件
MybatisX是一款基于IDEA的快速开发插件，为效率而生。安装方法：打开 IDEA，进入File -> Settings -> Plugins -> Browse Repositories，输入 mybatisx 搜索并安装。

功能：
- Java与XML调回跳转
- Mapper方法自动生成XML
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303030224791.png)
