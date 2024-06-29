> 基于MyBatisPlus完成标准Dao的增删改查功能
> 掌握MyBatisPlus中的**分页及条件查询构建**
> 掌握主键ID的生成策略
> 了解MyBatisPlus的代码生成器

---
# 1. MyBatisPlus入门案例与简介
这一节学习MyBatisPlus的入门案例与简介。**MybatisPlus（简称MP）是基于MyBatis框架基础上开发的增强型工具**，旨在简化开发（**对MyBatis的简化**）、提供效率。对于MyBatisPlus的学习，我们先体会下它简化在哪，然后再学习它是什么，以及它帮我们都做哪些事。MyBatisPlus的开发方式：
* 基于MyBatis使用MyBatisPlus
* 基于Spring使用MyBatisPlus+MyBatis
* ==基于SpringBoot使用MyBatisPlus==+MyBatis
## 1.1 MybatisPlus简介
MyBatisPlus（简称MP）是基于MyBatis框架基础上开发的增强型工具，旨在==简化开发、提高效率==。MyBatisPlus的官网为https://mp.baomidou.com/或https://mp.baomidou.com/。
![img|680x110](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231557619.png)
现在的页面中，这一行已经被删除，现在再去访问https://mybatis.plus会发现访问不到，这个就有很多可能性供我们猜想了，所以大家使用baomidou的网址进行访问即可。

官方文档中有一张很多小伙伴比较熟悉的图片：
![img|600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231558915.png)
从这张图中我们可以看出MP旨在成为MyBatis的最好搭档，而不是替换MyBatis，所以可以理解为MP是MyBatis的一套增强工具，它是在MyBatis的基础上进行开发的。==我们虽然使用MP，但底层依然是MyBatis的东西，也就是说**我们也可以在MP中写MyBatis的内容**==。

对于MP的学习，大家可以参考着官方文档来进行学习，里面都有详细的代码案例。
- 文档地址：https://mybatis.plus/guide/
- 源码地址：https://github.com/baomidou/mybatis-plus

MP的特性：
- **无侵入**：只做增强不做改变，引入它不会对现有工程产生影响，如丝般顺滑
- **损耗小**：启动即会**自动注入基本CURD**，性能基本无损耗，直接面向对象操作
- **强大的CRUD操作**：内置**通用Mapper**、**通用Service**，仅仅通过少量配置即可实现单表大部分CRUD操作，更有强大的条件构造器，满足各类使用需求
- **支持Lambda形式调用**：通过Lambda表达式，方便编写各类查询条件，无需再担心字段写错
- 支持多种数据库：支持MySQL、MariaDB、Oracle、DB2、H2、HSQL、SQLite、Postgre、
SQLServer2005、SQLServer等多种数据库
- **支持主键自动生成**：支持多达4种主键策略（内含分布式唯一 ID 生成器-Sequence），可自由配置，完美解决主键问题
- **支持XML热加载**：Mapper对应的XML支持热加载，对于简单的CRUD操作，甚至可以无XML启动
- 支持 `ActiveRecord` 模式：支持 `ActiveRecord` 形式调用，**实体类只需继承 `Model` 类即可进行强大的CRUD操作**
- **支持自定义全局通用操作**：支持全局通用方法注入（ Write once, use anywhere ）
- 支持关键词自动转义：支持数据库关键词（order、key......）自动转义，还可自定义关键词
- **内置代码生成器**：采用代码或者Maven插件，可快速生成Mapper、Model、Service、Controller层代码，
- **支持模板引擎**，更有超多自定义配置等您来使用
- **内置分页插件**：基于 MyBatis 物理分页，开发者无需关心具体操作，配置好插件之后，写分页等同于普通 List查询
- **内置性能分析插件**：可输出SQL语句以及其执行时间，建议开发测试时启用该功能，能快速揪出慢查询
- **内置全局拦截插件**：提供全表 `delete, update` 操作智能分析阻断，也可自定义拦截规则，预防误操作
- **内置SQL注入剥离器**：支持SQL注入剥离，有效预防SQL注入攻击

## 1.2 架构
`mybatis-plus-boot-start` 是为Spring Boot整合提供的模块，然后是注解模块、扩展模块、核心模块和生成代码模块来组成框架。执行逻辑是：==扫描实体，通过反射抽取表以及字段，生成SQL语句，并注入MyBatis容器中，通过MyBatis执行==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303012151299.png)

## 1.3 入门案例：Mybatis + MP
下面演示，通过纯Mybatis与Mybatis-Plus整合。

创建数据库 `mp` ，字符集 `utf8` ：
```sql
-- 创建测试表
CREATE TABLE `tb_user` (
`id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '主键ID',
`user_name` varchar(20) NOT NULL COMMENT '用户名',
`password` varchar(20) NOT NULL COMMENT '密码',
`name` varchar(30) DEFAULT NULL COMMENT '姓名',
`age` int(11) DEFAULT NULL COMMENT '年龄',
`email` varchar(50) DEFAULT NULL COMMENT '邮箱',
PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
-- 插入测试数据
INSERT INTO `tb_user` (`id`, `user_name`, `password`, `name`, `age`, `email`) VALUES
('1', 'zhangsan', '123456', '张三', '18', 'test1@itcast.cn');
INSERT INTO `tb_user` (`id`, `user_name`, `password`, `name`, `age`, `email`) VALUES
('2', 'lisi', '123456', '李四', '20', 'test2@itcast.cn');
INSERT INTO `tb_user` (`id`, `user_name`, `password`, `name`, `age`, `email`) VALUES
('3', 'wangwu', '123456', '王五', '28', 'test3@itcast.cn');
INSERT INTO `tb_user` (`id`, `user_name`, `password`, `name`, `age`, `email`) VALUES
('4', 'zhaoliu', '123456', '赵六', '21', 'test4@itcast.cn');
INSERT INTO `tb_user` (`id`, `user_name`, `password`, `name`, `age`, `email`) VALUES
('5', 'sunqi', '123456', '孙七', '24', 'test5@itcast.cn');
```
创建工程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303012217370.png)
导入依赖：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>cn.itcast.mp</groupId>
	<artifactId>itcast-mybatis-plus</artifactId>
	<version>1.0-SNAPSHOT</version>
	<modules>
		<module>itcast-mybatis-plus-simple</module>
	</modules>
	<packaging>pom</packaging>
	
	<dependencies>
		<!-- mybatis-plus插件依赖 -->
		<dependency>
			<groupId>com.baomidou</groupId>
			<artifactId>mybatis-plus</artifactId>
			<version>3.1.1</version>
		</dependency>
		<!-- MySql -->
		<dependency>
			<groupId>mysql</groupId>
			<artifactId>mysql-connector-java</artifactId>
			<version>5.1.47</version>
		</dependency>
		<!-- 连接池 -->
		<dependency>
			<groupId>com.alibaba</groupId>
			<artifactId>druid</artifactId>
			<version>1.0.11</version>
		</dependency>
		<!--简化bean代码的工具包-->
		<dependency>
			<groupId>org.projectlombok</groupId>
			<artifactId>lombok</artifactId>
			<optional>true</optional>
			<version>1.18.4</version>
		</dependency>
			<dependency>
			<groupId>junit</groupId>
			<artifactId>junit</artifactId>
			<version>4.12</version>
		</dependency>
		<dependency>
			<groupId>org.slf4j</groupId>
			<artifactId>slf4j-log4j12</artifactId>
			<version>1.6.4</version>
		</dependency>
	</dependencies>
	<build>
		<plugins>
			<plugin>
				<groupId>org.apache.maven.plugins</groupId>
				<artifactId>maven-compiler-plugin</artifactId>
				<configuration>
					<source>1.8</source>
					<target>1.8</target>
				</configuration>
			</plugin>
		</plugins>
	</build>
</project>
```
接着创建子模块：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" 
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<parent>
		<artifactId>itcast-mybatis-plus</artifactId>
		<groupId>cn.itcast.mp</groupId>
		<version>1.0-SNAPSHOT</version>
	</parent>
	<modelVersion>4.0.0</modelVersion>
	<packaging>jar</packaging>
	<artifactId>itcast-mybatis-plus-simple</artifactId>
</project>
```
`log4j.properties` ：
```JAVA
log4j.rootLogger=DEBUG,A1
log4j.appender.A1=org.apache.log4j.ConsoleAppender
log4j.appender.A1.layout=org.apache.log4j.PatternLayout
log4j.appender.A1.layout.ConversionPattern=[%t] [%c]-[%p] %m%n
```
### 1.3.1 Mybatis实现查询 `User` 
Mybatis实现查询 `User` 第一步，编写 `mybatis-config.xml` 文件：
```XML
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration
PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
	<environments default="development">
		<environment id="development">
			<transactionManager type="JDBC"/>
			<dataSource type="POOLED">
				<property name="driver" value="com.mysql.jdbc.Driver"/>
				<property name="url" value="jdbc:mysql://127.0.0.1:3306/mp?
				useUnicode=true&amp;characterEncoding=utf8&amp;autoReconnect=true&amp;allowMultiQuerie
				s=true&amp;useSSL=false"/>
				<property name="username" value="root"/>
				<property name="password" value="root"/>
			</dataSource>
		</environment>
	</environments>
	
	<mappers>
		<mapper resource="UserMapper.xml"/>
	</mappers>
</configuration>
```
第二步，编写 `User` 实体对象：（这里使用lombok进行了 `bean` 操作）
```java
package cn.itcast.mp.simple.pojo;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
@Data
@NoArgsConstructor
@AllArgsConstructor
public class User {
	private Long id;
	private String userName;
	private String password;
	private String name;
	private Integer age;
	private String email;
}
```
第三步，编写 `UserMapper` 接口：
```java
package cn.itcast.mp.simple.mapper;
import cn.itcast.mp.simple.pojo.User;
import java.util.List;
public interface UserMapper {
	List<User> findAll();
}
```
第四步，编写 `UserMapper.xml` 文件：
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="cn.itcast.mp.simple.mapper.UserMapper">
	<select id="findAll" resultType="cn.itcast.mp.simple.pojo.User">
		select * from tb_user
	</select>
</mapper>
```
第五步，编写TestMybatis测试用例：
```java
package cn.itcast.mp.simple;
import cn.itcast.mp.simple.mapper.UserMapper;
import cn.itcast.mp.simple.pojo.User;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Test;
import java.io.InputStream;
import java.util.List;

public class TestMybatis {
	@Test
	public void testUserList() throws Exception{
		String resource = "mybatis-config.xml";
		InputStream inputStream = Resources.getResourceAsStream(resource);
		SqlSessionFactory sqlSessionFactory = new
		SqlSessionFactoryBuilder().build(inputStream);
		SqlSession sqlSession = sqlSessionFactory.openSession();
		UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
		List<User> list = userMapper.findAll();
		for (User user : list) {
			System.out.println(user);
		}
	}
}
```
测试结果：
```java
[main] [cn.itcast.mp.simple.mapper.UserMapper.findAll]-[DEBUG] ==> Parameters:
[main] [cn.itcast.mp.simple.mapper.UserMapper.findAll]-[DEBUG] <== Total: 5
User(id=1, userName=null, password=123456, name=张三, age=18, email=test1@itcast.cn)
User(id=2, userName=null, password=123456, name=李四, age=20, email=test2@itcast.cn)
User(id=3, userName=null, password=123456, name=王五, age=28, email=test3@itcast.cn)
User(id=4, userName=null, password=123456, name=赵六, age=21, email=test4@itcast.cn)
User(id=5, userName=null, password=123456, name=孙七, age=24, email=test5@itcast.cn)
```
### 1.3.2 Mybatis+MP实现查询 `User` 
第一步，`UserMapper` 继承 `BaseMapper` ，将拥有了 `BaseMapper` 中的所有方法：
```java
package cn.itcast.mp.simple.mapper;
import cn.itcast.mp.simple.pojo.User;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
import java.util.List;
public interface UserMapper extends BaseMapper<User> {
	List<User> findAll();
}
```
第二步，使用MP中的 `MybatisSqlSessionFactoryBuilder` 进程构建：
```java
package cn.itcast.mp.simple;
import cn.itcast.mp.simple.mapper.UserMapper;
import cn.itcast.mp.simple.pojo.User;
import com.baomidou.mybatisplus.core.MybatisSqlSessionFactoryBuilder;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.junit.Test;
import java.io.InputStream;
import java.util.List;
public class TestMybatisPlus {
	@Test
	public void testUserList() throws Exception{
		String resource = "mybatis-config.xml";
		InputStream inputStream = Resources.getResourceAsStream(resource);
		//这里使用的是MP中的MybatisSqlSessionFactoryBuilder
		SqlSessionFactory sqlSessionFactory = new
			MybatisSqlSessionFactoryBuilder().build(inputStream); // 完成了MyBatis与MP的整合
		SqlSession sqlSession = sqlSessionFactory.openSession();
		UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
		// 可以调用BaseMapper中定义的方法
		List<User> list = userMapper.selectList(null); // 或者使用自己定义的userMapper.findAll()方法
		for (User user : list) {
			System.out.println(user);
		}
	}
}
```
运行报错：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303012238413.png)
解决：在 `User` 对象中添加 `@TableName` ，指定数据库表名。之后测试，结果和之前一样成功了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303012239147.png)
简单说明：由于使用了 `MybatisSqlSessionFactoryBuilder` 进行了构建，**继承的 `BaseMapper` 中的方法就载入到了 `SqlSession` 中**，所以就可以直接使用相关的方法；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303012240209.png)
## 1.4 入门案例：Spring + MyBatis + MP
引入了Spring框架，**数据源、构建等工作就交给了Spring管理**。创建子Module：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<parent>
		<artifactId>itcast-mybatis-plus</artifactId>
		<groupId>cn.itcast.mp</groupId>
		<version>1.0-SNAPSHOT</version>
	</parent>
	<modelVersion>4.0.0</modelVersion>
	<artifactId>itcast-mybatis-plus-spring</artifactId>
	<properties>
		<spring.version>5.1.6.RELEASE</spring.version>
	</properties>
	
	<dependencies>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>${spring.version}</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-jdbc</artifactId>
			<version>${spring.version}</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-test</artifactId>
			<version>${spring.version}</version>
		</dependency>
	</dependencies>
</project>
```
### 1.4.1 实现查询 `User`
第一步，编写 `jdbc.properties` 。
```java
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://127.0.0.1:3306/mp?
useUnicode=true&characterEncoding=utf8&autoReconnect=true&allowMultiQueries=true&useSSL
=false
jdbc.username=root
jdbc.password=root
```
第二步，编写 `applicationContext.xml` 。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:context="http://www.springframework.org/schema/context"
xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context.xsd">
<context:property-placeholder location="classpath:*.properties"/>
	<!-- 定义数据源 -->
	<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource"
	destroy-method="close">
		<property name="url" value="${jdbc.url}"/>
		<property name="username" value="${jdbc.username}"/>
		<property name="password" value="${jdbc.password}"/>
		<property name="driverClassName" value="${jdbc.driver}"/>
		<property name="maxActive" value="10"/>
		<property name="minIdle" value="5"/>
	</bean>
	<!--这里使用MP提供的sqlSessionFactory，完成了Spring与MP的整合-->
	<bean id="sqlSessionFactory"
		class="com.baomidou.mybatisplus.extension.spring.MybatisSqlSessionFactoryBean">
		<property name="dataSource" ref="dataSource"/>
	</bean>
	<!--扫描mapper接口，使用的依然是Mybatis原生的扫描器-->
	<bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
		<property name="basePackage" value="cn.itcast.mp.simple.mapper"/>
	</bean>
</beans>
```
第三步，编写 `User` 对象以及 `UserMapper` 接口：
```java
package cn.itcast.mp.simple.pojo;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
@Data
@NoArgsConstructor
@AllArgsConstructor
@TableName("tb_user")
public class User {
	private Long id;
	private String userName;
	private String password;
	private String name;
	private Integer age;
	private String email;
}

package cn.itcast.mp.simple.mapper;
import cn.itcast.mp.simple.pojo.User;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;
public interface UserMapper extends BaseMapper<User> {

}
```
第四步，编写测试用例：
```java
package cn.itcast.mp.simple;
import cn.itcast.mp.simple.mapper.UserMapper;
import cn.itcast.mp.simple.pojo.User;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import java.util.List;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class TestSpringMP {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testSelectList(){
		List<User> users = this.userMapper.selectList(null);
		for (User user : users) {
		System.out.println(user);
		}
	}
}
```
测试：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303020021067.png)

## 1.5 入门案例：SpringBoot整合MyBatis、MyBatisPlus
SpringBoot我们刚学习完成，它能**快速构建Spring开发环境**、用以整合其他技术，使用起来是非常简单，**对于MP的学习，我们也基于SpringBoot来构建学习**。学习之前，我们先来回顾下，SpringBoot整合Mybatis的开发过程：
* 创建SpringBoot工程
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231524259.png)
* 勾选配置使用的技术，能够实现自动添加起步依赖包
	![img|650](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231525363.png)
* 设置 `dataSource` 相关属性（JDBC参数）
	![img|550](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231525333.png)
* 定义数据层接口映射配置
	![img|500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231550193.png)

我们可以参考着上面实现步骤，**把SpringBoot整合MyBatisPlus**来快速实现下，具体的实现步骤如下。

步骤1：创建数据库及表。
```sql
create database if not exists mybatisplus_db character set utf8;
use mybatisplus_db;
CREATE TABLE user (
    id bigint(20) primary key auto_increment,
    name varchar(32) not null,
    password  varchar(32) not null,
    age int(3) not null ,
    tel varchar(32) not null
);
insert into user values(1,'Tom','tom',3,'18866668888');
insert into user values(2,'Jerry','jerry',4,'16688886666');
insert into user values(3,'Jock','123456',41,'18812345678');
insert into user values(4,'传智播客','itcast',15,'4006184000');
```
步骤2：创建SpringBoot工程。
![img|900](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231550591.png) 
步骤3：勾选配置使用技术。
![img|700](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231551608.png)
**说明**：**由于MP并未被收录到IDEA的系统内置配置，无法直接选择加入**，需要手动在 `pom.xml` 中配置添加。

步骤4：`pom.xml` 补全依赖。
```xml
<!--mybatis-plus的springboot支持-->
<dependency>
    <groupId>com.baomidou</groupId>
    <artifactId>mybatis-plus-boot-starter</artifactId>
    <version>3.4.1</version>
</dependency>
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.1.16</version>
</dependency>
-------------------------------------------------------
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">  
    <modelVersion>4.0.0</modelVersion>  
    <parent>  
        <groupId>org.springframework.boot</groupId>  
        <artifactId>spring-boot-starter-parent</artifactId>  
        <version>2.4.5</version>  
        <relativePath/> <!-- lookup parent from repository -->  
    </parent>  
  
    <groupId>com.example</groupId>  
    <artifactId>demo</artifactId>  
    <version>0.0.1-SNAPSHOT</version>  
    <name>demo</name>  
    <description>demo</description>  
  
    <properties>  
        <java.version>1.8</java.version>  
    </properties>  
  
    <dependencies>  
        <dependency>  
            <groupId>org.springframework.boot</groupId>  
            <artifactId>spring-boot-starter</artifactId>  
        </dependency>  
  
        <dependency>  
            <groupId>org.springframework.boot</groupId>  
            <artifactId>spring-boot-starter-test</artifactId>  
            <scope>test</scope>  
        </dependency>  
  
        <dependency>  
            <groupId>org.springframework.boot</groupId>  
            <artifactId>spring-boot-starter-web</artifactId>  
            <scope>compile</scope>  
        </dependency>  
  
        <dependency>  
            <groupId>org.springframework.boot</groupId>  
            <artifactId>spring-boot-devtools</artifactId>  
            <optional>true</optional>  
        </dependency>  
  
        <dependency>  
            <groupId>com.baomidou</groupId>  
            <artifactId>mybatis-plus-boot-starter</artifactId>  
            <version>3.4.2</version>  
        </dependency>  
  
        <dependency>  
            <groupId>org.projectlombok</groupId>  
            <artifactId>lombok</artifactId>  
            <version>1.18.20</version>  
        </dependency>  
        <dependency>  
            <groupId>com.alibaba</groupId>  
            <artifactId>fastjson</artifactId>  
            <version>1.2.76</version>  
        </dependency>  
        <dependency> <!-- 通用语言包 -->  
            <groupId>commons-lang</groupId>  
            <artifactId>commons-lang</artifactId>  
            <version>2.6</version>  
        </dependency>  
  
        <dependency>  
            <groupId>mysql</groupId>  
            <artifactId>mysql-connector-java</artifactId>  
            <scope>runtime</scope>  
        </dependency>  
        <dependency>  
            <groupId>com.alibaba</groupId>  
            <artifactId>druid-spring-boot-starter</artifactId>  
            <version>1.1.23</version>  
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
**说明**：
* `Druid` 数据源可以加也可以不加，SpringBoot有内置的数据源，可以配置成使用 `Druid` 数据源
* ==从MP的依赖关系可以看出，通过依赖传递已经将MyBatis与MyBatis整合Spring的JAR包导入，**我们不需要额外添加MyBatis的相关JAR包**==。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231552937.png)

步骤5：添加MP的相关配置信息。`resources` 默认生成的是 `properties` 配置文件，可以将其替换成 `yml` 文件，并在文件中配置数据库连接的相关信息 `application.yml` 。
```yml
spring:
  datasource:
    type: com.alibaba.druid.pool.DruidDataSource
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://localhost:3306/mybatisplus_db?serverTimezone=UTC 
    username: root
    password: root
```
**说明**：**`serverTimezone` 是用来设置时区**，UTC是标准时区，和咱们的时间差8小时，所以可以将其修改为 `Asia/Shanghai` 。

步骤6：根据数据库表创建实体类。
```java
@Data
@NoArgsConstructor
@AllArgsConstructor
@TableName("user")
public class User {   
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;
    //或 setter...getter...toString方法略
}
```
步骤7：创建 `Dao` 数据接口，继承 `BaseMapper<User>` （相当于之前的 `UserMapper` ，不用编写方法和SQL语句了）。`UserDao` 继承 `BaseMapper` ，将拥有了 `BaseMapper` 中的所有方法。
```java
@Mapper
public interface UserDao extends BaseMapper<User> {
}
```
步骤8：编写引导启动类。
```java
@SpringBootApplication
//@MapperScan("com.itheima.dao") //设置mapper接口的扫描包
public class Mybatisplus01QuickstartApplication {
    public static void main(String[] args) {
        SpringApplication.run(Mybatisplus01QuickstartApplication.class, args);
    }
}
```
**说明**：**`Dao` 接口要想被容器扫描到**，有两种解决方案：
* 方案一：**在 `Dao` 接口上添加 `@Mapper` 注解**，并且==确保 `Dao` 处在引导类所在包或其子包中==。**该方案的缺点是需要在每一 `Dao` 接口中添加注解**。
* 方案二：**在引导类上添加 `@MapperScan` 注解，其属性为所要扫描的 `Dao` 所在包**
	* 该方案的好处是只需要写一次，则指定包下的所有 `Dao` 接口都能被扫描到，`@Mapper` 就可以不写。

步骤9：编写测试类。
```java
//@RunWith(SpringRunner.class)
@SpringBootTest
class MpDemoApplicationTests {
	@Autowired
	private UserDao userDao;
	@Test
	public void testGetAll() {
		List<User> userList = userDao.selectList(null);
		System.out.println(userList);
	}
}
```
**说明**：`userDao` 注入的时候下面有红线提示的原因是什么？
* `UserDao` 是一个接口，不能实例化对象
* **只有在服务器启动IOC容器初始化后，由框架创建DAO接口的代理对象来注入**
* 现在服务器并未启动，所以代理对象也未创建，IDEA查找不到对应的对象注入，所以提示报红
* 一旦服务启动，就能注入其代理对象，所以该错误提示不影响正常运行。

查看运行结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231556568.png)
==跟之前整合MyBatis相比，你会发现我们**不需要在DAO接口中编写方法和SQL语句了**，只需要继承 `BaseMapper` 接口即可==。整体来说简化很多。

---
# 2. 标准数据层开发
通过前面的学习，我们了解到：通过继承 `BaseMapper` 就可以获取到各种各样的单表操作。
![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303020233228.png)
这一节中重点学习的是**数据层标准的CRUD（增删改查）的实现**与**分页功能**。代码比较多，我们一个个来学习。
## 2.1 标准CRUD使用
标准的CRUD功能都有哪些，以及MP都提供了哪些方法可以使用呢？我们先来看张图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231559275.png)
对于这张图的方法，我们挨个演示。首先说下，案例中的环境就是入门案例的内容，第一个先完成 `新增` 功能。
## 2.2 新增
在进行新增之前，我们可以分析下新增的方法：
```java
/**
* 插入一条记录
*
* @param entity 实体对象
*/
int insert(T entity);
```
* `T` ：泛型，新增用来保存新增数据
* `int` ：返回值，新增成功后返回1，没有新增成功返回的是0

在测试类中进行新增操作：
```java
@SpringBootTest
class Mybatisplus01QuickstartApplicationTests {
    @Autowired
    private UserDao userDao;

    @Test
    void testSave() {
        User user = new User();
        user.setName("黑马程序员");
        user.setPassword("itheima");
        user.setAge(12);
        user.setTel("4006184000");
        userDao.insert(user);
        int result = this.userMapper.insert(user); //返回的result是受影响的行数,并不是自增后的id
		System.out.println("result = " + result);
		System.out.println(user.getId()); //自增后的id会回填到对象中
    }
}
```
执行测试后，数据库表中就会添加一条数据。 但ID的值不正确，我们期望的是数据库自增长，应该是5才对，实际却是MP自动生成了ID的值并写入到了数据库。
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231618894.png)
这是后面要学习的主键ID生成策略，这块的这个问题，暂时先放放。
## 2.3 删除
在进行删除之前，我们可以分析下删除的方法：
```java
/**
* 根据 ID 删除
*
* @param id 主键ID
*/
int deleteById(Serializable id);
```
* `Serializable` ：参数类型
* 思考：参数类型为什么是一个序列化类？ 从这张图可以看出，
	![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231619337.png)
    * `String` 和 `Number` 是 `Serializable` 的子类，
    * `Number` 又是 `Float,Double,Integer` 等类的父类，
    * 能作为主键的数据类型都已经是 `Serializable` 的子类，
    * **MP使用 `Serializable` 作为参数类型，就好比我们可以用 `Object` 接收任何数据类型一样**。
* `int` ：返回值类型，数据删除成功返回1，未删除数据返回0。

在测试类中进行删除操作：
```java
 @SpringBootTest
class Mybatisplus01QuickstartApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testDelete() {
        int result = userDao.deleteById(1401856123725713409L);
        System.out.println("result = " + result);
    }
}
```
还有删除方法：
```java
/**
* 根据 columnMap 条件，删除记录
*
* @param columnMap 表字段 map 对象
*/
int deleteByMap(@Param(Constants.COLUMN_MAP) Map<String, Object> columnMap);
```
测试用例：
```java
package cn.itcast.mp;
import cn.itcast.mp.mapper.UserMapper;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
import java.util.HashMap;
import java.util.Map;
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testDeleteByMap() {
		Map<String, Object> columnMap = new HashMap<>();
		columnMap.put("age",20);
		columnMap.put("name","张三");
		//将columnMap中的元素设置为删除的条件，多个之间为and关系
		int result = this.userMapper.deleteByMap(columnMap);
		System.out.println("result = " + result);
	}
}
```
结果：
```java
[main] [cn.itcast.mp.mapper.UserMapper.deleteByMap]-[DEBUG] ==> Preparing: DELETE FROM
tb_user WHERE name = ? AND age = ?
[main] [cn.itcast.mp.mapper.UserMapper.deleteByMap]-[DEBUG] ==> Parameters: 张三
(String), 20(Integer)
[main] [cn.itcast.mp.mapper.UserMapper.deleteByMap]-[DEBUG] <== Updates: 0
```
`delete` 方法定义：
```java
/**
* 根据 entity 条件，删除记录
*
* @param wrapper 实体对象封装操作类（可以为 null）
*/
int delete(@Param(Constants.WRAPPER) Wrapper<T> wrapper);
```
测试用例：
```java
package cn.itcast.mp;
import cn.itcast.mp.mapper.UserMapper;
import cn.itcast.mp.pojo.User;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
import java.util.HashMap;
import java.util.Map;
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testDeleteByMap() {
		User user = new User();
		user.setAge(20);
		user.setName("张三");
		//将实体对象进行包装，包装为操作条件
		QueryWrapper<User> wrapper = new QueryWrapper<>(user);
		int result = this.userMapper.delete(wrapper);
		System.out.println("result = " + result);
	}
}
```
`deleteBatchIds` 方法定义：
```java
/**
* 删除（根据ID 批量删除）
*
* @param idList 主键ID列表(不能为 null 以及 empty)
*/
int deleteBatchIds(@Param(Constants.COLLECTION) Collection<? extends Serializable>
idList);
```
测试用例：
```java
...
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testDeleteByMap() {
		//根据id集合批量删除
		int result = this.userMapper.deleteBatchIds(Arrays.asList(1L,10L,20L));
		System.out.println("result = " + result);
	}
}
```
## 2.4 修改
在MP中，修改更新操作有2种，一种是根据ID更新，另一种是根据条件更新。在进行修改之前，我们可以分析修改的方法：
```java
/**
* 根据 ID 修改
*
* @param entity 实体对象
*/
int updateById(@Param(Constants.ENTITY) T entity);
```
- `T` ：泛型，需要修改的数据内容，注意==因为是根据ID进行修改，所以传入的对象中需要有ID属性值==
- `int` ：返回值，修改成功后返回1，未修改数据返回0

在测试类中进行新增操作：
```java
@SpringBootTest
class Mybatisplus01QuickstartApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testUpdate() {
        User user = new User();
        user.setId(1L); //主键
        user.setName("Tom888");
        user.setPassword("tom888");
        userDao.updateById(user); // 根据id更新,更新不为null的字段
    }
}
```
**说明**：==修改的时候，只修改实体对象中有值的字段==。选择性修改，这其实是利用Mybatis的逆向工程。

根据条件更新的方法定义如下：
```java
/**
* 根据whereEntity条件,更新记录
*
* @param entity 实体对象 (set条件值,可以为null)
* @param updateWrapper 实体对象封装操作类（可以为null,里面的entity用于生成where语句）
*/
int update(@Param(Constants.ENTITY) T entity, @Param(Constants.WRAPPER) Wrapper<T>
updateWrapper);
```
测试用例：
```java
package cn.itcast.mp;
import cn.itcast.mp.mapper.UserMapper;
import cn.itcast.mp.pojo.User;
import com.baomidou.mybatisplus.core.conditions.Wrapper;
import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.baomidou.mybatisplus.core.conditions.update.UpdateWrapper;
import net.minidev.json.writer.UpdaterMapper;
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
	public void testUpdate() {
		User user = new User();
		user.setAge(22); //更新的字段
		//更新的条件
		QueryWrapper<User> wrapper = new QueryWrapper<>();
		wrapper.eq("id", 6);
		//执行更新操作
		int result = this.userMapper.update(user, wrapper);
		System.out.println("result = " + result);
	}
}
```
或者，通过 `UpdateWrapper` 进行更新：
```java
@Test
public void testUpdate() {
	//更新的条件以及字段
	UpdateWrapper<User> wrapper = new UpdateWrapper<>();
	wrapper.eq("id", 6).set("age", 23);
	//执行更新操作
	int result = this.userMapper.update(null, wrapper);
	System.out.println("result = " + result);
}
```
均可达到更新的效果。
## 2.5 根据ID查询
在进行根据ID查询之前，我们可以分析下根据ID查询的方法：
```java
/**
* 根据 ID 查询
*
* @param id 主键ID
*/
T selectById(Serializable id);
```
- `Serializable` ：参数类型，主键ID的值
- `T` ：根据ID查询只会返回一条数据

在测试类中进行查询操作：
```java
@SpringBootTest
class Mybatisplus01QuickstartApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetById() {
        User user = userDao.selectById(2L);
        System.out.println(user);
    }
}
```
## 2.6 查询所有
在进行查询所有之前，我们可以分析下查询所有的方法：
```java
/**
* 根据 entity 条件，查询全部记录
*
* @param queryWrapper 实体对象封装操作类（可以为 null）
*/
List<T> selectList(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);
```
- `Wrapper` ：用来构建条件查询的条件，目前我们没有可直接传为Null
- `List<T>` ：因为查询的是所有，所以返回的数据是一个集合

在测试类中进行查询操作：
```java
@SpringBootTest
class Mybatisplus01QuickstartApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll() {
        List<User> userList = userDao.selectList(null);
        System.out.println(userList);
    }
}
```
## 2.7 其他查询操作
`selectBatchIds` 方法定义：
```java
/**
* 查询（根据ID 批量查询）
*
* @param idList 主键ID列表(不能为 null 以及 empty)
*/
List<T> selectBatchIds(@Param(Constants.COLLECTION) Collection<? extends Serializable>
idList);
```
测试用例：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testSelectBatchIds() {
		//根据id集合批量查询
		List<User> users = this.userMapper.selectBatchIds(Arrays.asList(2L, 3L, 10L));
		for (User user : users) {
			System.out.println(user);
		}
	}
}
```
`selectOne` 方法定义：
```java
/**
* 根据 entity 条件，查询一条记录
*
* @param queryWrapper 实体对象封装操作类（可以为 null）
*/
T selectOne(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);
```
测试用例：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testSelectOne() {
		QueryWrapper<User> wrapper = new QueryWrapper<User>();
		wrapper.eq("name", "李四");
		//根据条件查询一条数据，如果结果超过一条会报错
		User user = this.userMapper.selectOne(wrapper);
		System.out.println(user);
	}
}
```
`selectCount` 方法定义：
```java
/**
* 根据 Wrapper 条件，查询总记录数
*
* @param queryWrapper 实体对象封装操作类（可以为 null）
*/
Integer selectCount(@Param(Constants.WRAPPER) Wrapper<T> queryWrapper);
```
测试样例：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testSelectCount() {
		QueryWrapper<User> wrapper = new QueryWrapper<User>();
		wrapper.gt("age", 23); //年龄大于23岁
		//根据条件查询数据条数
		Integer count = this.userMapper.selectCount(wrapper);
		System.out.println("count = " + count);
	}
}
```
我们所调用的方法==都是==来自于**DAO接口继承的 `BaseMapper` 类**中。里面的方法有很多，后面慢慢去学习里面的内容。

## 2.8 Lombok
代码写到这，我们会发现**DAO接口类的编写现在变成最简单的了，里面什么都不用写**。反过来看看模型类的编写都需要哪些内容：
* 私有属性
* `setter...getter...`方法
* `toString` 方法
* 构造函数

虽然这些内容不难，同时也都是通过IDEA工具生成的，但是过程还是必须得走一遍，那么**对于模型类的编写有没有什么优化方法**？就是我们接下来要学习的Lombok。
### 2.8.1 概念
Lombok，一个Java类库，**提供了一组注解，简化POJO实体类开发**。
### 2.8.2 使用步骤
步骤1：添加lombok依赖。
```xml
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <!--<version>1.18.12</version>-->
</dependency>
```
**注意**：版本可以不用写，因为**SpringBoot中已经管理了lombok的版本**。

步骤2：安装Lombok的插件。==新版本IDEA已经内置了该插件，如果删除setter和getter方法程序有报红，则需要安装插件==：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231644928.png)
如果在IDEA中找不到lombok插件，可以访问如下网站
`https://plugins.jetbrains.com/plugin/6317-lombok/versions` ，根据自己IDEA的版本下载对应的lombok插件，下载成功后，在IDEA中采用离线安装的方式进行安装。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231649929.png)
步骤3：模型类上添加注解。Lombok常见的注解有：
* `@Setter` ：为模型类的属性提供 `setter` 方法
* `@Getter` ：为模型类的属性提供 `getter` 方法
* `@ToString` ：为模型类的属性提供 `toString` 方法
* `@EqualsAndHashCode` ：为模型类的属性提供 `equals` 和 `hashcode` 方法
* `@Data` ：**是个组合注解**，包含上面的注解的功能
* `@NoArgsConstructor` ：提供一个无参构造函数
* `@AllArgsConstructor` ：提供一个包含所有参数的构造函数

Lombok的注解还有很多，后三个是比较常用的，其他的大家后期用到了，再去补充学习。
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class User {
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;
}
```
**说明**：Lombok只是简化模型类的编写，我们之前的方法也能用，比如有人会问：我如果只想要有 `name` 和 `password` 的构造函数，该如何编写？
```java
@Data
@AllArgsConstructor
@NoArgsConstructor
public class User {
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;

    public User(String name, String password) {
        this.name = name;
        this.password = password;
    }
}
```
**这种方式是被允许的**。
## 2.9 分页功能
基础的增删改查就已经学习完了，刚才我们在分析基础开发的时候，有一个分页功能还没有实现，**在MP中如何实现分页功能**，就是咱们接下来要学习的内容。分页查询使用的方法是：
```java
/**
* 根据 entity 条件，查询全部记录（并翻页）
*
* @param page 分页查询条件（可以为 RowBounds.DEFAULT）
* @param queryWrapper 实体对象封装操作类（可以为 null）
*/
IPage<T> selectPage(IPage<T> page, @Param(Constants.WRAPPER) Wrapper<T> queryWrapper);
```
- `IPage` ：用来构建**分页查询条件**
- `Wrapper` ：用来构建**条件查询的条件**，目前我们没有可直接传为 `Null`
- `IPage` ：返回值，你会发现**构建分页条件和方法的返回值**都是 `IPage`

**`IPage` 是一个接口，我们需要找到它的实现类来构建它**，具体的实现类，可以进入到 `IPage` 类中按Ctrl+H，会找到其只有一个实现类为 `Page` 。具体使用步骤如下。

步骤1：调用方法传入参数获取返回值。
```java
@SpringBootTest
class Mybatisplus01QuickstartApplicationTests {
    @Autowired
    private UserDao userDao;
    //分页查询
    @Test
    void testSelectPage() {
        //1 创建IPage分页对象,设置分页参数,1为当前页码，3为每页显示的记录数
        IPage<User> page = new Page<>(1,3);
        //2 执行分页查询
        userDao.selectPage(page,null);
        //3 获取分页结果
        System.out.println("当前页码值："+page.getCurrent());
        System.out.println("每页显示数："+page.getSize());
        System.out.println("一共多少页："+page.getPages());
        System.out.println("一共多少条数据："+page.getTotal());
        System.out.println("数据："+page.getRecords());
    }
}
```
步骤2：设置分页拦截器（拦截并增强）。这个拦截器MP已经为我们提供好了，**我们只需要将其配置成Spring管理的 `bean` 对象即可**。
```java
@Configuration
public class MybatisPlusConfig {
	/**
	 * 分页插件
	 */
    @Bean
    public MybatisPlusInterceptor mybatisPlusInterceptor(){
        //1 创建MybatisPlusInterceptor拦截器对象
        MybatisPlusInterceptor mpInterceptor = new MybatisPlusInterceptor();
        //2 添加分页拦截器(可以有多个)
        mpInterceptor.addInnerInterceptor(new PaginationInnerInterceptor());
        return mpInterceptor;
    }
}
// 集成spring的可以在spring配置文件,往MybatisSqlSessionFactoryBean中的参数plugins中注入MybatisPlusInterceptor
// @Configuration
// @MapperScan("cn.itcast.mp.mapper") //设置mapper接口的扫描包
// public class MybatisPlusConfig {
// 	/**
// 	 * 分页插件
// 	 */
// 	@Bean
// 	public PaginationInterceptor paginationInterceptor() {
// 		return new PaginationInterceptor();
// 	}
// }
```
**说明**：上面的代码记不住咋办呢？这些内容在MP的官方文档中有详细的说明，我们可以查看官方文档类配置。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231747625.png)
步骤3：运行测试程序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231747241.png)
如果想查看MP执行的SQL语句（**用于调试程序**），可以修改 `application.yml` 配置文件，
```yml
mybatis-plus:
  configuration:
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl #打印SQL日志到控制台
```
打开日志后，就可以在控制台打印出对应的SQL语句，开启日志功能性能就会受到影响，调试完后记得关闭。
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231815138.png)


## 2.10 SQL注入的原理
前面我们已经知道，**MP在启动后会将 `BaseMapper` 中的一系列的方法注册到 `mappedStatements` 中**，我们就可以使用它了。那么究竟是如何注入的呢？流程又是怎么样的？下面我们将一起来分析下。

在MP中，`ISqlInjector` 负责SQL的注入工作，它是一个接口，`AbstractSqlInjector` 是它的实现类，实现关系如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021310400.png)
在 `AbstractSqlInjector` 中，主要是由 `inspectInject() `方法进行注入的，如下：
```java
@Override
public void inspectInject(MapperBuilderAssistant builderAssistant, Class<?> mapperClass) {
	Class<?> modelClass = extractModelClass(mapperClass);
	if (modelClass != null) {
		String className = mapperClass.toString();
		Set<String> mapperRegistryCache = GlobalConfigUtils.getMapperRegistryCache(builderAssistant.getConfiguration());
		if (!mapperRegistryCache.contains(className)) {
			List<AbstractMethod> methodList = this.getMethodList();
			if (CollectionUtils.isNotEmpty(methodList)) {
				TableInfo tableInfo = TableInfoHelper.initTableInfo(builderAssistant, modelClass);
				// 循环注入自定义方法
				methodList.forEach(m -> m.inject(builderAssistant, mapperClass, modelClass, tableInfo));
			} else {
				logger.debug(mapperClass.toString() + ", No effective injection method was found.");
			}
			mapperRegistryCache.add(className);
		}
	}
}
```
在实现方法中， `methodList.forEach(m -> m.inject(builderAssistant, mapperClass, modelClass, tableInfo));` 是关键，循环遍历方法，进行注入。
最终会调用抽象方法 `injectMappedStatement` 进行真正的注入：
```java
/**
* 注入自定义 MappedStatement
*
* @param mapperClass mapper 接口
* @param modelClass mapper 泛型
* @param tableInfo 数据库表反射信息
* @return MappedStatement
*/
public abstract MappedStatement injectMappedStatement(Class<?> mapperClass, Class<?> modelClass, TableInfo tableInfo);
```
Ctrl+H查看哪些类实现了该方法，实现类很多：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021314073.png)
以 `SelectById` 为例查看：
```java
public class SelectById extends AbstractMethod {
	@Override
	public MappedStatement injectMappedStatement(Class<?> mapperClass, Class<?> modelClass, TableInfo tableInfo) {
		SqlMethod sqlMethod = SqlMethod.LOGIC_SELECT_BY_ID;
		SqlSource sqlSource = new RawSqlSource(configuration,
			String.format(sqlMethod.getSql(),
				sqlSelectColumns(tableInfo, false),
				tableInfo.getTableName(), tableInfo.getKeyColumn(),
				tableInfo.getKeyProperty(),
				tableInfo.getLogicDeleteSql(true, false)
			), Object.class
		);
		return this.addSelectMappedStatement(mapperClass, sqlMethod.getMethod(), sqlSource, modelClass, tableInfo);
	}
}
```
在方法中打上几个断点，Debug运行，可以看到，生成了 `SqlSource` 对象，再将SQL通过 `addSelectMappedStatement` 方法添加到 `mappedStatements` 中。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021318287.png)

---
# 3. DQL编程控制
增删改查四个操作中，**查询是非常重要的也是非常复杂的操作**，这块需要我们重点学习下，这节主要学习的内容有：
* 条件查询方式
* 查询投影
* 查询条件设定
* 字段映射与表名映射

## 3.1 条件查询
### 3.1.1 条件查询的类
==MyBatisPlus将书写复杂的SQL查询条件进行了封装，使用编程形式完成查询条件的组合==。

这个我们在前面都有见过，比如查询所有和分页查询的时候，都有看到过一个`Wrapper` 类，这个类就是用来构建查询条件的，如下图所示：
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231816677.png)

那么条件查询如何使用 `Wrapper` 来构建呢？
### 3.1.2 环境构建
在构建条件查询之前，我们先来准备下环境
* 创建一个SpringBoot项目
* `pom.xml` 中添加对应的依赖
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
           xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 https://maven.apache.org/xsd/maven-4.0.0.xsd">
      <modelVersion>4.0.0</modelVersion>
      <parent>
          <groupId>org.springframework.boot</groupId>
          <artifactId>spring-boot-starter-parent</artifactId>
          <version>2.5.0</version>
      </parent>
      <groupId>com.itheima</groupId>
      <artifactId>mybatisplus_02_dql</artifactId>
      <version>0.0.1-SNAPSHOT</version>
      <properties>
          <java.version>1.8</java.version>
      </properties>
      <dependencies>
          <dependency>
              <groupId>com.baomidou</groupId>
              <artifactId>mybatis-plus-boot-starter</artifactId>
              <version>3.4.1</version>
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
* 编写 `UserDao` 接口（感觉很鸡肋，Dao层业务写到了Service里，重用性就没了，维护成本也增加了）
  ```java
  @Mapper
  public interface UserDao extends BaseMapper<User> {
  }
  ```
* 编写模型类
  ```java
  @Data
  public class User {
      private Long id;
      private String name;
      private String password;
      private Integer age;
      private String tel;
  }
  ```
* 编写引导类
  ```java
  @SpringBootApplication
  public class Mybatisplus02DqlApplication {
      public static void main(String[] args) {
          SpringApplication.run(Mybatisplus02DqlApplication.class, args);
      }
  }
  ```
* 编写配置文件
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
* 编写测试类
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

最终创建的项目结构为：
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231818140.png)
测试时，控制台打印的日志比较多，速度有点慢而且不利于查看运行结果，所以接下来我们把这个日志处理下：
* **取消初始化Spring日志打印**，`resources` 目录下添加 `logback.xml` ，名称固定，内容如下（什么都不写）：
    ```xml
    <?xml version="1.0" encoding="UTF-8"?>
    <configuration>
    </configuration>
    ```
	**说明**：`logback.xml` 的配置内容，不是我们学习的重点，如果有兴趣可以自行百度查询。
- **取消MybatisPlus启动banner图标**。`application.yml` 添加如下内容:
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231819839.png)
    ```yml
    # mybatis-plus日志控制台输出
    mybatis-plus:
      configuration:
        log-impl: org.apache.ibatis.logging.stdout.StdOutImpl
      global-config:
        banner: off # 关闭mybatisplus启动图标
    ```
* **取消SpringBoot的log打印**。`application.yml` 添加如下内容：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231820288.png)
    ```yml
    spring:
      main:
        banner-mode: off # 关闭SpringBoot启动图标(banner)
    ```

解决控制台打印日志过多的相关操作可以不用去做，一般会被用来方便我们查看程序运行的结果。
### 3.1.3 构建条件查询：条件构造器
在进行查询的时候，我们的入口是在 `Wrapper` 这个类上，因为它是一个接口，所以我们需要去找它对应的实现类，关于实现类也有很多，说明我们有多种构建查询条件对象的方式。可以看到，`AbstractWrapper` 和 `AbstractChainWrapper` 是重点实现，接下来我们重点学习 `AbstractWrapper` 以及其子类。官网文档地址：https://mybatis.plus/guide/wrapper.html
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231821840.png)
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021603991.png)

> 说明：`QueryWrapper(LambdaQueryWrapper)` 和 `UpdateWrapper(LambdaUpdateWrapper)` 的父类用于生成SQL的 `where` 条件，`entity` 属性也用于生成SQL的 `where` 条件。注意：`entity` 生成的 `where` 条件与使用各个API生成的 `where` 条件没有任何关联行为。

先来看第一种：`QueryWrapper` 。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll(){
        QueryWrapper qw = new QueryWrapper();
        qw.lt("age",18);
        List<User> userList = userDao.selectList(qw);
        System.out.println(userList);
    }
}
```
`lt` ：小于 `<` ，最终的SQL语句为
```sql
SELECT id,name,password,age,tel FROM user WHERE (age < ?)
```
第一种方式介绍完后，有个小问题就是在写字符串条件时，容易出错，比如 `age` 写错，就会导致查询不成功。

接着来看第二种：**`QueryWrapper` 的基础上使用lambda**。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        QueryWrapper<User> qw = new QueryWrapper<User>();
        qw.lambda().lt(User::getAge, 10);//添加条件
        List<User> userList = userDao.selectList(qw);
        System.out.println(userList);
    }
}
```
`User::getAget` ，为lambda表达式中的类名::方法名，最终的SQL语句为：
```sql
SELECT id,name,password,age,tel FROM user WHERE (age < ?)
```
**注意**：**构建 `LambdaQueryWrapper` 的时候泛型不能省**。此时我们再次编写条件的时候，就不会存在写错名称的情况，但 `qw` 后面多了一层 `lambda()` 调用。

接着来看第三种，`LambdaQueryWrapper` 。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.lt(User::getAge, 10);
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
这种方式就解决了上一种方式所存在的问题。
### 3.1.4 多条件构建
学完了三种构建查询对象的方式，每一种都有自己的特点，所以用哪一种都行，刚才都是一个条件，**那如果有多个条件该如何构建呢**？

需求：查询数据库表中，年龄在10岁到30岁之间的用户信息。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.lt(User::getAge, 30);
        lqw.gt(User::getAge, 10);
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
`gt` ：大于，最终的SQL语句为：
```sql
SELECT id,name,password,age,tel FROM user WHERE (age < ? AND age > ?)
```
**构建多条件的时候，可以支持链式编程**：
```java
LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
lqw.lt(User::getAge, 30).gt(User::getAge, 10);
List<User> userList = userDao.selectList(lqw);
System.out.println(userList);
```
需求：查询数据库表中，年龄小于10或年龄大于30的数据。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll() {
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.lt(User::getAge, 10).or().gt(User::getAge, 30);
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
`or()` 就相当于我们SQL语句中的 `or` 关键字，不加默认是 `and` ，最终的SQL语句为:
```sql
SELECT id,name,password,age,tel FROM user WHERE (age < ? OR age > ?)
```
### 3.1.5 `null` 判定
先来看一张图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231824107.png)
* 我们在做条件查询的时候，一般会有很多条件可以供用户进行选择查询。
* 这些条件用户可以选择使用也可以选择不使用，比如我要查询价格在8000以上的手机
* 在输入条件的时候，价格有一个区间范围，按照需求只需要在第一个价格输入框中输入8000
* 后台在做价格查询的时候，一般会让 price>值1 and price <值2
* 因为前端没有输入值2，所以如果不处理的话，就会出现 price>8000 and price < null问题
* 这个时候查询的结果就会出问题，具体该如何解决？
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231825921.png)

需求：查询数据库表中，根据输入年龄范围来查询符合条件的记录。用户在输入值的时候，如果只输入第一个框，说明要**查询大于该年龄的用户**；如果只输入第二个框，说明要**查询小于该年龄的用户**；如果两个框都输入了，说明要查询年龄在两个范围之间的用户。

思考第一个问题：后台如果想接收前端的两个数据，该如何接收？我们可以使用两个简单数据类型，也可以使用一个模型类，但 `User` 类中目前只有一个 `age` 属性，如：
```java
@Data
public class User {
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;
}
```
使用一个 `age` 属性，如何去接收页面上的两个值呢？这个时候我们有两个解决方案。

方案一：添加属性 `age2` ，这种做法可以但是会影响到原模型类的属性内容。
```java
@Data
public class User {
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;
    private Integer age2;
}
```
方案二：新建一个模型类，让其继承 `User` 类，并在其中添加 `age2` 属性， `UserQuery` 在拥有 `User` 属性后同时添加了 `age2` 属性。
```java
@Data
public class User {
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;
}
@Data
public class UserQuery extends User {
    private Integer age2;
}
```
环境准备好后，我们来实现下刚才的需求：
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll(){
        //模拟页面传递过来的查询数据
        UserQuery uq = new UserQuery();
        uq.setAge(10);
        uq.setAge2(30);
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        if(null != uq.getAge2()){
            lqw.lt(User::getAge, uq.getAge2());
        }
        if(null != uq.getAge()) {
            lqw.gt(User::getAge, uq.getAge());
        }
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
上面的写法可以完成条件为非空的判断，但问题很明显，如果条件多的话，每个条件都需要判断，代码量就比较大，来看MP给我们提供的简化方式：
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll(){
        //模拟页面传递过来的查询数据
        UserQuery uq = new UserQuery();
        uq.setAge(10);
        uq.setAge2(30);
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        // 先判定第一个参数是否为true,是true则连接当前条件
        lqw.lt(null != uq.getAge2(), User::getAge, uq.getAge2());
        lqw.gt(null != uq.getAge(), User::getAge, uq.getAge());
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
`lt()` 方法：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231831151.png)
`condition` 为 `boolean` 类型，返回 `true` ，则添加条件，返回 `false` 则不添加条件。

## 3.2 查询投影（设置查询什么字段）
### 3.2.1 查询指定字段
目前我们在查询数据的时候，**什么都没有做默认就是查询表中所有字段的内容**，我们所说的**查询投影**即不查询所有字段，只查询出指定内容的数据。具体如何来实现？
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {

    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.select(User::getId, User::getName, User::getAge);
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
**`select(...)` 方法用来设置查询的字段列**，可以设置多个，最终的SQL语句为：
  ```sql
  SELECT id,name,age FROM user
  ```
如果使用的不是lambda，就需要手动指定字段：
  ```java
  @SpringBootTest
  class Mybatisplus02DqlApplicationTests {
  
      @Autowired
      private UserDao userDao;
      
      @Test
      void testGetAll(){
          QueryWrapper<User> lqw = new QueryWrapper<User>();
          lqw.select("id", "name", "age", "tel");
          List<User> userList = userDao.selectList(lqw);
          System.out.println(userList);
      }
  }
  ```
最终的SQL语句为：`SELECT id,name,age,tel FROM user` 。
### 3.2.2 聚合查询
需求：聚合函数查询，完成 `count, max, min, avg, sum` 的使用
- `count` ：总记录数
- `max` ：最大值
- `min` ：最小值
- `avg` ：平均值
- `sum` ：求和

```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    @Test
    void testGetAll(){
        QueryWrapper<User> lqw = new QueryWrapper<User>();
        //lqw.select("count(*) as count");
        //SELECT count(*) as count FROM user
        //lqw.select("max(age) as maxAge");
        //SELECT max(age) as maxAge FROM user
        //lqw.select("min(age) as minAge");
        //SELECT min(age) as minAge FROM user
        //lqw.select("sum(age) as sumAge");
        //SELECT sum(age) as sumAge FROM user
        lqw.select("avg(age) as avgAge");
        //SELECT avg(age) as avgAge FROM user
        List<Map<String, Object>> userList = userDao.selectMaps(lqw);
        System.out.println(userList);
    }
}
```
为了在做结果封装的时候能够更简单，我们将上面的聚合函数都起了个名称，方面后期来获取这些数据。
### 3.2.3 分组查询
需求：分组查询，完成 `group by` 的查询使用。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        QueryWrapper<User> lqw = new QueryWrapper<User>();
        lqw.select("count(*) as count, tel");
        lqw.groupBy("tel");
        List<Map<String, Object>> list = userDao.selectMaps(lqw);
        System.out.println(list);
    }
}
```
`groupBy` 为分组，最终的SQL语句为：
```sql
SELECT count(*) as count,tel FROM user GROUP BY tel
```
**注意**：
* **聚合与分组查询，无法使用lambda表达式来完成**
* MP只是对MyBatis的增强，==如果MP实现不了，我们可以直接在DAO接口中使用MyBatis的方式实现==。

## 3.3 查询条件
前面我们只使用了 `lt()` 和 `gt()` ，除了这两个方法外，MP还封装了很多条件对应的方法，这一节我们重点把MP提供的查询条件方法进行学习下。MP的查询条件有很多：
* 范围匹配 `>, =, between, notBetween`
* 模糊匹配 `like`
* 空判定 `null`
* 包含性匹配 `in`
* 分组 `group`
* 排序 `order`
* ……

### 3.3.1 等值查询
需求：根据用户名和密码查询用户信息。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.eq(User::getName, "Jerry").eq(User::getPassword, "jerry");
        User loginUser = userDao.selectOne(lqw);
        System.out.println(loginUser);
    }
}
```
- `selectList` ：查询结果为多个或者单个
- `selectOne` ：查询结果为单个

`eq()` 相当于 `=` ，对应的SQL语句为：
```sql
SELECT id,name,password,age,tel FROM user WHERE (name = ? AND password = ?)
```
 `allEq` ：全部 `eq`（或个别 `isNull` ）。
```java
allEq(Map<R, V> params)
allEq(Map<R, V> params, boolean null2IsNull)
allEq(boolean condition, Map<R, V> params, boolean null2IsNull)
```
个别参数说明：
- `params` : `key` 为数据库字段名，`value` 为字段值。`null2IsNull` 为 `true` 则在 `map` 的 `value` 为 `null` 时，调用 `isNull` 方法，为 `false` 时则忽略 `value` 为 `null` 的字段
- 例1：`allEq({id:1,name:"老王",age:null})` ---> `id = 1 and name = '老王' and age is null`
- 例2：`allEq({id:1,name:"老王",age:null}, false)` ---> `id = 1 and name = '老王'`

```java
allEq(BiPredicate<R, V> filter, Map<R, V> params)
allEq(BiPredicate<R, V> filter, Map<R, V> params, boolean null2IsNull)
allEq(boolean condition, BiPredicate<R, V> filter, Map<R, V> params, boolean
null2IsNull)
```
个别参数说明：
- `filter` ：过滤函数，是否允许字段传入，比对条件中 `params` 与 `null2IsNull` 
- 例1：`allEq((k,v) -> k.indexOf("a") > 0, {id:1,name:"老王",age:null})` ---> `name = '老王' and age is null`
- 例2：`allEq((k,v) -> k.indexOf("a") > 0, {id:1,name:"老王",age:null}, false)` ---> `name = '老王'`

测试用例：
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testWrapper() {
		QueryWrapper<User> wrapper = new QueryWrapper<>();
		//设置条件
		Map<String,Object> params = new HashMap<>();
		params.put("name", "曹操");
		params.put("age", "20");
		params.put("password", null);
		// wrapper.allEq(params);//SELECT * FROM tb_user WHERE password IS NULL AND name = ? AND age = ?
		// wrapper.allEq(params,false); //SELECT * FROM tb_user WHERE name = ? AND age = ?
		// wrapper.allEq((k, v) -> (k.equals("name") || k.equals("age")), params);//SELECT * FROM tb_user WHERE name = ? AND age = ?
		List<User> users = this.userMapper.selectList(wrapper);
		for (User user : users) {
			System.out.println(user);
		}
	}
}
```
### 3.3.2 范围查询
需求：对年龄进行范围查询，使用 `lt(), le(), gt(), ge(), between()` 进行范围查询。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.between(User::getAge, 10, 30);
        //SELECT id,name,password,age,tel FROM user WHERE (age BETWEEN ? AND ?)
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
* `gt()` 大于 `>`
* `ge()` 大于等于 `>=`
* `lt()` 小于 `<`
* `lte()` 小于等于 `<=`
* `between() : between ? and ?`

### 3.3.3 模糊查询
需求：查询表中 `name` 属性的值以 `J` 开头的用户信息，使用 `like` 进行模糊查询。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lqw = new LambdaQueryWrapper<User>();
        lqw.likeLeft(User::getName, "J");
        //SELECT id,name,password,age,tel FROM user WHERE (name LIKE ?)
        List<User> userList = userDao.selectList(lqw);
        System.out.println(userList);
    }
}
```
* `like()` ：前后加百分号，如 `%J%`
* `likeLeft()` ：前面加百分号，如 `%J`
* `likeRight()` ：后面加百分号，如 `J%`

### 3.3.4 排序查询
需求：查询所有数据，然后按照 `id` 降序。
```java
@SpringBootTest
class Mybatisplus02DqlApplicationTests {
    @Autowired
    private UserDao userDao;
    
    @Test
    void testGetAll(){
        LambdaQueryWrapper<User> lwq = new LambdaQueryWrapper<>();
        /**
         * condition: 条件，返回boolean，
         		当condition为true，进行排序，如果为false，则不排序
         * isAsc:是否为升序，true为升序，false为降序
         * columns：需要操作的列
         */
        lwq.orderBy(true,false, User::getId);
        userDao.selectList(lw
    }
}
```
除了上面演示的这种实现方式，还有很多其他的排序方法可以被调用，如图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231846789.png)
* `orderBy` 排序
	* `condition` 条件，`true` 则添加排序，`false` 则不添加排序
	* `isAsc` 是否为升序，`true` 升序，`false` 降序
	* `columns` 排序字段，可以有多个
* `orderByAsc/Desc(单个column)` 按照指定字段进行升序/降序
* `orderByAsc/Desc(多个column)` 按照多个字段进行升序/降序
* `orderByAsc/Desc`
	* `condition` ：条件，`true` 添加排序，`false` 不添加排序
	* 多个 `columns` ：按照多个字段进行排序

### 3.3.5 逻辑查询
拼接 `OR` ：主动调用 `or` ，表示紧接着下一个方法不是用 `and` 连接！不调用 `or` 则默认为使用 `and` 连接。

`AND` 嵌套。例：`and(i -> i.eq("name", "李白").ne("status", "活着")) ---> and (name = '李白' and status <> '活着')`

除了上面介绍的这几种查询条件构建方法以外，还会有很多其他的方法，比如 `isNull,isNotNull,in,notIn` 等等方法可供选择，具体参考官方文档的条件构造器来学习使用，具体的网址为https://mp.baomidou.com/guide/wrapper.html#abstractwrapper。

## 3.4 映射匹配兼容性
前面我们已经能从表中查询出数据，并将数据封装到模型类中，这整个过程涉及到一张表和一个模型类： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231849109.png)
之所以数据能够成功的从表中获取并封装到模型对象中，原因是表的字段列名和模型类的属性名一样。那么问题就来了。
### 问题1：表字段与编码属性设计不同步
==当表的列名和模型类的属性名发生不一致，就会导致数据封装不到模型对象==，这个时候就需要其中一方做出修改，那如果前提是==两边都不能改又该如何解决==？

MP给我们提供了一个注解 `@TableField` ，使用该注解可以实现**模型类属性名和表的列名**之间的映射关系。该注解常常解决的问题有2个：
1. 对象中的属性名和字段名不一致的问题（非驼峰）
2. 对象中的属性字段在表中不存在的问题
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231850072.png)
### 问题2：编码中添加了数据库中未定义的属性
当模型类中多了一个数据库表不存在的字段，就会导致生成的SQL语句中在 `select` 时查询了数据库不存在的字段，程序运行就会报错，错误信息为：
```java
Unknown column '多出来的字段名称' in 'field list'
```
具体的解决方案用到的**还是 `@TableField` 注解**，它有一个属性叫 `exist` ，设置该字段是否在数据库表中存在，如果设置为 `false` 则不存在，生成SQL语句查询的时候，就不会再查询该字段了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231851873.png)
### 问题3：采用默认查询开放了更多的字段查看权限
查询表中所有的列的数据，就可能把一些敏感数据查询到返回给前端，这个时候我们就需要**限制哪些字段默认不要进行查询**。解决方案是 `@TableField` 注解的一个属性叫 `select` ，该属性设置**默认是否需要查询该字段的值**，`true(默认值)` 表示默认查询该字段，`false` 表示默认不查询该字段。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231852070.png)
类似的查询结果图如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303020245708.png)

其他用法，如大字段不加入查询字段：
![450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303020244590.png)

### 知识点1：`@TableField`
| 名称     | `@TableField`                                                  |
| -------- | ------------------------- |
| 类型     | ==属性注解==                                            |
| 位置     | 模型类属性定义上方                 |
| 作用     | 设置当前属性对应的数据库表中的字段关系  |
| 相关属性 | value(默认): 设置数据库表字段名称<br/>exist: 设置属性在数据库表字段中是否存在，默认为true，此属性不能与value合并使用<br/>select: 设置属性是否参与查询，此属性与select()映射配置不冲突 |

### 问题4：表名与编码开发设计不同步
**该问题主要是表的名称和模型类的名称不一致**，导致查询失败，这个时候通常会报如下错误信息：
```java
Table 'databaseName.tableNaem' doesn't exist
```
翻译过来就是数据库中的表不存在。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231853276.png)
解决方案是使用MP提供的另外一个注解 `@TableName` 、来设置表与模型类之间的对应关系。
#### 知识点2：`@TableName`
| 名称     | `@TableName`                    |
| -------- | ----------------------------- |
| 类型     | ==类注解==                    |
| 位置     | 模型类定义上方                |
| 作用     | 设置当前类对应于数据库表关系  |
| 相关属性 | value(默认)：设置数据库表名称 |

### 代码演示
接下来我们使用案例的方式把刚才的知识演示下。
步骤1：修改数据库表 `user` 为 `tbl_user` 。直接查询会报错，原因是**MP默认情况下会使用模型类的类名首字母小写当表名使用**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231942681.png)
步骤2：模型类添加 `@TableName` 注解。
```java
@Data
@TableName("tbl_user")
public class User {
    private Long id;
    private String name;
    private String password;
    private Integer age;
    private String tel;
}
```
步骤3：将字段 `password` 修改成 `pwd` 。直接查询会报错，原因是**MP默认情况下会使用模型类的属性名当做表的列名使用**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231943998.png)
步骤4：使用 `@TableField` 映射关系。
```java
@Data
@TableName("tbl_user")
public class User {
    private Long id;
    private String name;
    @TableField(value="pwd")
    private String password;
    private Integer age;
    private String tel;
}
```
步骤5：添加一个数据库表不存在的字段。
```java
@Data
@TableName("tbl_user")
public class User {
    private Long id;
    private String name;
    @TableField(value="pwd")
    private String password;
    private Integer age;
    private String tel;
    private Integer online;
}
```
直接查询会报错，原因是MP默认情况下会查询「模型类的所有属性」对应的数据库表的列，而 `online` 不存在。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302231943483.png)
步骤6：使用 `@TableField` 排除字段
```java
@Data
@TableName("tbl_user")
public class User {
    private Long id;
    private String name;
    @TableField(value="pwd")
    private String password;
    private Integer age;
    private String tel;
    @TableField(exist=false)
    private Integer online;
}
```
步骤7：查询时将 `pwd` 隐藏。
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

---
# 4. 配置
在MP中有大量的配置，其中有一部分是Mybatis原生的配置，另一部分是MP的配置，详情：https://mybatis.plus/config/。

下面我们对常用的配置做讲解，有些是SpringBoot会帮忙做的。
## 4.1 基本配置
### 4.1.1 `configLocation` 指定全局配置文件
MyBatis配置文件位置，如果您有单独的MyBatis配置，请将其路径配置到 `configLocation` 中。 MyBatis Configuration的具体内容请参考MyBatis官方文档。

Spring Boot：
```yaml
mybatis-plus.config-location = classpath:mybatis-config.xml
```
Spring MVC：
```xml
<bean id="sqlSessionFactory" class="com.baomidou.mybatisplus.extension.spring.MybatisSqlSessionFactoryBean">
	<property name="configLocation" value="classpath:mybatis-config.xml"/>
</bean>
```
### 4.1.2 `mapperLocations` 指定Mapper.xml文件的路径
MyBatis Mapper所对应的XML文件位置，**如果您在 `Mapper` 中有自定义方法（XML中有自定义实现），需要进行该配置**（`BaseMapper` 提供的方法无法满足需求），告诉 `Mapper` 所对应的 XML 文件位置。

Spring Boot：
```yaml
mybatis-plus.mapper-locations = classpath*:mybatis/*.xml
```
Spring MVC：
```xml
<bean id="sqlSessionFactory"
class="com.baomidou.mybatisplus.extension.spring.MybatisSqlSessionFactoryBean">
	<property name="mapperLocations" value="classpath*:mybatis/*.xml"/>
</bean>
```
Maven 多模块项目的扫描路径需以 `classpath*:` 开头 （即加载多个JAR包下的 XML文件）。

测试：UserMapper.xml。
```xml
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper
PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="cn.itcast.mp.mapper.UserMapper">
	<select id="findById" resultType="cn.itcast.mp.pojo.User">
		select * from tb_user where id = #{id}
	</select>
</mapper>
```
```java
package cn.itcast.mp.mapper;
import cn.itcast.mp.pojo.User;
import com.baomidou.mybatisplus.core.mapper.BaseMapper;

public interface UserMapper extends BaseMapper<User> {
	User findById(Long id);
}
```
测试用例：
```java
package cn.itcast.mp;
import cn.itcast.mp.mapper.UserMapper;
import cn.itcast.mp.pojo.User;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
@RunWith(SpringRunner.class)
@SpringBootTest
public class UserMapperTest {
	@Autowired
	private UserMapper userMapper;
	@Test
	public void testSelectPage() {
		User user = this.userMapper.findById(2L);
		System.out.println(user);
	}
}
```
运行结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303021430083.png)

### 4.1.3 `typeAliasesPackage` 指定包的别名
MyBaits别名包扫描路径，通过该属性可以**给包中的类注册别名**，注册后在 **`Mapper` 对应的XML文件**中可以直接使用类名，而**不用使用全限定的类名**（即XML中调用时不用包含包名），简化XML文件的编写。

Spring Boot：
```yaml
mybatis-plus.type-aliases-package = cn.itcast.mp.pojo
```
Spring MVC：
```xml
<bean id="sqlSessionFactory"
class="com.baomidou.mybatisplus.extension.spring.MybatisSqlSessionFactoryBean">
	<property name="typeAliasesPackage"
value="com.baomidou.mybatisplus.samples.quickstart.entity"/>
</bean>
```

## 4.2 进阶配置
本部分（Configuration）的配置大都为MyBatis原生支持的配置，这意味着您可以通过MyBatis XML配置文件的形式进行配置，或者采用MyBatisPlus的方式配置。
### 4.2.1 `mapUnderscoreToCamelCase`
类型： `boolean`
默认值： `true`
是否开启自动驼峰命名规则（camel case）映射，即**从经典数据库列名 `A_COLUMN`（下划线命名） 到经典Java属性名 `aColumn`（驼峰命名） 的类似映射**。

> 注意：此属性在MyBatis中原默认值为 `false` ，在MyBatis-Plus中，此属性也将用于生成最终的SQL的 `select body` 。如果您的数据库命名符合规则，无需使用 `@TableField` 注解指定数据库字段名

示例（SpringBoot）：
```yaml
#关闭自动驼峰映射，该参数不能和mybatis-plus.config-location同时存在
mybatis-plus.configuration.map-underscore-to-camel-case=false
```
### 4.2.2 `cacheEnabled`
类型： `boolean`
默认值： `true`
全局地开启或关闭配置文件中的所有映射器已经配置的任何缓存，默认为 `true` 。
示例：
```java
mybatis-plus.configuration.cache-enabled=false
```
## 4.3 DB策略配置
### 4.3.1 `idType`
类型： `com.baomidou.mybatisplus.annotation.IdType`
默认值： `ID_WORKER`
**全局默认主键类型**，设置后，即可省略实体对象中的 `@TableId(type = IdType.AUTO)` 配置。

示例：SpringBoot：
```yaml
mybatis-plus.global-config.db-config.id-type=auto

mybatis-plus:  
  configuration:  
    #在映射实体或者属性时，将数据库中表名和字段名中的下划线去掉，按照驼峰命名法映射 (mybatis-plus)    #如表名address_book -> 类名AddressBook  
    #如字段user_name -> userName  
    map-underscore-to-camel-case: true  
    # 使用配置方式开启日志,设置日志输出方式为标准输出,查阅SQL执行日志  
    log-impl: org.apache.ibatis.logging.stdout.StdOutImpl  
  global-config:  
    db-config:  
      id-type: Assign_ID # 分配  
```
SpringMVC：
```xml
<!--这里使用MP提供的sqlSessionFactory，完成了Spring与MP的整合-->
<bean id="sqlSessionFactory"
class="com.baomidou.mybatisplus.extension.spring.MybatisSqlSessionFactoryBean">
	<property name="dataSource" ref="dataSource"/>
	<property name="globalConfig">
		<bean class="com.baomidou.mybatisplus.core.config.GlobalConfig">
			<property name="dbConfig">
				<bean
				class="com.baomidou.mybatisplus.core.config.GlobalConfig$DbConfig">
					<property name="idType" value="AUTO"/>
				</bean>
			</property>
		</bean>
	</property>
</bean>
```
### 4.3.2 `tablePrefix`
类型： `String`
默认值： `null`
表名前缀，**全局配置后可省略 `@TableName()` 配置**。

SpringBoot：
```java
mybatis-plus.global-config.db-config.table-prefix=tb_
```
SpringMVC：在 `MybatisSqlSessionFactoryBean` 中指定 `globalConfig` 。
```java
<bean id="sqlSessionFactory"
class="com.baomidou.mybatisplus.extension.spring.MybatisSqlSessionFactoryBean">
	<property name="dataSource" ref="dataSource"/>
	<property name="globalConfig">
		<bean class="com.baomidou.mybatisplus.core.config.GlobalConfig">
			<property name="dbConfig">
				<bean
				class="com.baomidou.mybatisplus.core.config.GlobalConfig$DbConfig">
					<property name="idType" value="AUTO"/>
					<property name="tablePrefix" value="tb_"/>
				</bean>
			</property>
		</bean>
	</property>
</bean>
```
