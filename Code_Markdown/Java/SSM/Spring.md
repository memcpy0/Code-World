经常进去[https://spring.io/](https://spring.io/)来进行技术选型。SpringMVC从属于Spring，Maven高级是学习SpringBoot的基础。==基于SpringBoot实现基础SSM框架整合，掌握第三方技术与SpringBoot整合思想==。框架整合：
- MyBatis
- MyBatis-plus
- Struts
- Struts2
- Hibernate
- ...

---
# 1. Spring简介
## 1.1 Spring是什么
Spring是分层的Java SE/EE应用 ***full-stack* 轻量级开源框架**，==以反转控制 *IoC, Inverse Of Control* 和面向切面编程 *AOP, Aspect Oriented Programming* 为内核==。提供了**展现层** SpringMVC 和**持久层** Spring JDBCTemplate 以及**业务层事务管理**等众多的企业级应用技术，==还能整合开源世界众多著名的第三方框架和类库，逐渐成为使用最多的Java EE 企业应用开源框架==。Spring发展到今天已经形成了一种开发的生态圈，它提供了若干个项目，每个项目用于完成特定的功能——Spring全家桶。我们要学习的包括Spring Framework（**Spring生态圈中最基础的项目，是其他项目的根基**）、Spring Boot（简化Spring开发，提高效率）、Spring Cloud（分布式开发），基本可以应对企业级开发的所有主流开发需求。

## 1.2 Spring发展历程
1997 年，IBM提出了EJB的思想，引领了当时的Java EE开发，早期的JAVAEE开发大都基于该思想。
1998 年，SUN制定开发标准规范EJB1.0
1999 年，EJB 1.1发布
2001 年，EJB 2.0发布
2003 年，EJB 2.1发布
2006 年，EJB 3.0发布
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171500871.png)
Rod Johnson（ Spring 之父） Expert One-to-One J2EE Design and Development(2002)
- 阐述了J2EE使用EJB开发设计的优点及解决方案，Expert One-to-One J2EE Development without EJB(2004) ，
- 阐述了J2EE开发不使用 EJB 的解决方式（Spring 雏形），
-  Spring1.0是纯配置文件开发
* Spring2.0为了简化开发引入了注解开发，此时是配置文件加注解的开发方式
* Spring3.0已经可以进行**纯注解开发**，使开发效率大幅提升，这里会以注解开发为主
* Spring4.0根据JDK的版本升级对个别API进行了调整
* Spring5.0已经全面支持JDK8，现在Spring最新的是5系列，所以建议大家把JDK安装成1.8版
- 2017年9月份发布了Spring的最新版本Spring5.0通用版（GA）。

Spring Framework是Spring生态圈中最基础的项目，是其他项目的根基。Spring Framework的发展也经历了很多版本的变更，每个版本都有相应的调整：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171632011.png)
Spring Framework的5版本目前没有最新的架构图，而最新的是4版本，所以之后主要研究的是4的架构图。
## 1.3 Spring的优势
Spring有如下优势：
- **方便解耦，简化开发**：通过Spring提供的IoC容器，可以==将对象间的依赖关系交由Spring 进行控制，避免硬编码所造成的过度耦合==。用户也不必再为单例模式类、属性文件解析等这些很底层的需求编写代码，可以更专注于上层的应用。
- **AOP编程的支持**：通过Spring的AOP功能，方便进行面向切面编程，==许多「不容易用传统OOP实现的功能」可以通过AOP轻松实现==。
- **声明式事务的支持**：可以将我们从单调烦闷的事务管理代码中解脱出来，==通过声明式方式灵活的进行事务管理，提高开发效率和质量==。
- 方便程序的测试：==可以用「非容器依赖的编程方式」进行几乎所有的测试工作==，测试不再是昂贵的操作，而是随手可做的事情。
- 方便集成各种优秀框架：Spring对各种优秀框架（Struts、Hibernate、Hessian、Quartz等）的支持。
- 降低JavaEE API的使用难度：Spring对 JavaEE API（如 JDBC、JavaMail、远程调用等）**进行了薄薄的封装层**，使这些API的使用难度大为降低。
- Java源码是经典学习范例：Spring的源代码设计精妙、结构清晰、匠心独用，处处体现着大师对Java设计模式灵活运用、以及对Java技术的高深造诣。==它的源代码无意是 Java技术的最佳实践的范例==。

## 1.4 Spring的体系结构
![img|500x400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171505923.png)
学习过程：先核心容器，再数据访问，然后AOP/Aspects，接着是事务，Web在后面学习
1. Core Container：核心容器，装对象，这个模块是Spring最核心的模块，其他的都需要依赖该模块，核心概念是IoC/DI
2. AOP：面向切面编程，依赖核心容器（上层依赖下层），目的是==在不改变原有代码的前提下对其进行功能增强==
	Aspects：AOP思想的具体实现，建议与AOP一并使用
3. Data Access/Integration：Spring全家桶中对数据访问与集成（Spring支持整合其他的数据层解决方案，比如Mybatis）的技术，Transactions是其中重点学习
4. Web：Web开发，在SpringMVC中进一步学习
5. Test：单元测试与集成测试

## 1.5 Spring核心概念
在Spring核心中，主要包含IoC/DI、IoC容器和Bean，那么问题就来了，这些都是什么呢？要想解答这个问题，就需要先分析下目前咱们在编写代码过程中遇到的问题：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171534666.png)
(1) 业务层需要调用数据层的方法，就需要在业务层 `new` 数据层的对象
(2) 如果数据层的实现类发生变化，那么业务层的代码也需要跟着改变，发生变更后，都需要进行编译打包和重部署
(3) 所以，现在代码在编写的过程中存在的问题是：==耦合度偏高==

针对这个问题，该如何解决呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171557060.png)
我们就想，如果能把框中的内容给去掉，不就可以降低依赖了么，但是又会引入新的问题，去掉以后程序能运行么？答案肯定是不行，因为 `bookDao` 没有赋值为 `Null` ，强行运行就会出空指针异常。所以现在的问题就是，业务层不想 `new` 对象，运行时又需要这个对象，该咋办呢？

针对这个问题，Spring就提出了一个解决方案：**使用对象时，在程序中不要主动使用 `new` 产生对象，转换为由==外部==提供对象**。这种实现思想就是Spring的一个核心概念。

## 1.6 IoC、IoC容器、Bean、DI
### 1.6.1 *IoC, Inversion of Control* 控制反转
什么是控制反转呢？——**使用对象时，由主动 `new` 产生对象转换为由==外部==提供对象**，此过程中==对象创建控制权由程序转移到外部，此思想称为控制反转==。
- 业务层要用数据层的类对象，以前是自己 `new` 的
- 现在自己不 `new` 了，交给 `别人[外部]` 来创建对象
* `别人[外部]` 就反转控制了数据层对象的创建权
* 这种思想就是控制反转
* 别人[外部]指定是什么呢？继续往下

Spring和IoC之间的关系是什么呢？
* **Spring技术对IoC思想进行了实现**
* **Spring提供了一个容器**，称为==IoC容器==（即核心容器），用来**充当IoC思想中的"外部"**
* IoC思想中的 `别人[外部]` 指的就是Spring的IoC容器

IoC容器的作用以及内部存放的是什么？
* IoC容器负责**对象的创建、初始化**等一系列工作，其中包含了**数据层和业务层的类对象**
* **被创建或被管理的对象在IoC容器中统称为==Bean==**
* IoC容器中放的就是一个个的 `Bean` 对象
### 1.6.2 依赖注入
当IoC容器中创建好 `service` 和 `dao` 对象后，程序能正确执行么？
* 不行，因为 `service` 运行需要依赖 `dao` 对象
* IoC容器中虽然有 `service` 和 `dao` 对象，但 `service` 对象和 `dao` 对象没有任何关系
* 需要把 `dao` 对象交给 `service` ，也就是说**要绑定 `service` 和 `dao` 对象之间的关系**

像这种「在容器中建立对象与对象之间的绑定关系」就要用到 ==*DI, Dependency Injection* 依赖注入==：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171811880.png)

什么是依赖注入呢？（有关系才能绑定）
* **在容器中建立「 `bean` 与 `bean` 之间的依赖关系」的整个过程，称为依赖注入**
  * 业务层要用数据层的类对象，以前是自己 `new` 的
  * 现在自己不 `new` 了，靠 `别人[外部其实指的就是IoC容器]` 来给注入进来
  * 这种思想就是控制反转+依赖注入

IoC容器中哪些 `bean` 之间要建立依赖关系呢?
* 这个**需要程序员根据业务需求提前建立好关系**，如业务层需要依赖数据层，`service` 就要和 `dao` 建立依赖关系

介绍完Spring的IoC和DI的概念后，我们会发现这两个概念的最终目标就是==充分解耦==，具体实现靠：
* 使用IoC容器管理 `bean`（IoC)
* 在IoC容器内将有依赖关系的 `bean` 进行关系绑定（DI）
* 最终结果为，**使用对象时不仅可以直接从IoC容器中获取，并且获取到的 `bean` 已经绑定了所有的依赖关系**。

### 1.6.3 核心概念小结
这节比较重要，重点要理解 `什么是IoC/DI思想` 、`什么是IoC容器` 和 `什么是Bean` ：
(1) 什么IoC/DI思想？
* IoC：控制反转，控制反转的是**对象的创建权**
* DI：依赖注入，绑定对象与对象之间的依赖关系

(2) 什么是IoC容器？——Spring创建了一个容器用来存放所创建的对象，这个容器就叫IoC容器。

(3) 什么是Bean？——容器中所存放的一个个对象就叫 `Bean` 或 `Bean` 对象

## 1.7 入门案例
介绍完Spring的核心概念后，接下来我们思考一个问题就是，**Spring到底是如何来实现IoC和DI的**，接下来就通过一些简单入门案例，来演示具体实现过程。
### 1.7.1 IoC入门案例
对于入门案例，我们得先 `分析思路` 然后再 `代码实现` 。
#### 1. 入门案例思路分析
Spring是使用容器来管理 `bean` 对象的，那么管什么？——主要管理项目中所使用到的类对象，比如 `Service` 和 `Dao` 。

如何将被管理的对象告知IoC容器？——**使用配置文件**。

被管理的对象交给IoC容器，要想从容器中获取对象，就先得思考如何获取到IoC容器？——**Spring框架提供相应的接口**。

IoC容器得到后，如何从容器中获取bean？——**调用Spring框架提供对应接口中的方法**。

使用Spring导入哪些坐标？用别人的东西，就需要在 `pom.xml` 添加对应的依赖。

#### 2. 入门案例代码实现
> 需求分析：将 `BookServiceImpl` 和 `BookDaoImpl` 交给Spring管理，并从容器中获取对应的 `bean` 对象进行方法调用。
> 1. 创建Maven的Java项目
> 2. `pom.xml` 添加Spring的依赖JAR包
> 3. 创建 `BookService,BookServiceImpl, BookDao, BookDaoImpl` 四个类
> 4. `resources` 下**添加 `spring` 配置文件**，并完成 `bean` 的配置
> 5. **使用Spring提供的接口，完成IoC容器的创建**
> 6. **从容器中获取对象进行方法调用**

步骤1：创建Maven项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171823638.png)

步骤2：添加Spring的依赖JAR包：
```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>5.2.10.RELEASE</version>
    </dependency>
    <dependency>
        <groupId>junit</groupId>
        <artifactId>junit</artifactId>
        <version>4.12</version>
        <scope>test</scope>
    </dependency>
</dependencies>
```
步骤3：添加案例中需要的类。创建 `BookService, BookServiceImpl, BookDao, BookDaoImpl` 四个类：
```java
public interface BookDao {
    public void save();
}
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
}
public interface BookService {
    public void save();
}
public class BookServiceImpl implements BookService {
    private BookDao bookDao = new BookDaoImpl();
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
步骤4：添加Spring配置文件。`resources` 下添加Spring配置文件 `applicationContext.xml` ，并完成 `bean` 的配置。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171825744.png)
步骤5：在配置文件中完成 `bean` 的配置。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
 
    <!--bean标签标示配置bean
    	id属性标示给bean起名字
    	class属性表示给bean定义类型
	-->
	<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl"/>
</beans>
```
**==注意事项==： 定义 `bean` 时 `id` 属性在同一个上下文中（配置文件）不能重复**。

步骤6：获取IoC容器。使用Spring提供的接口 `ApplicationContext` 完成IoC容器的创建，创建 `App` 类，编写 `main` 方法：
```java
public class App {
    public static void main(String[] args) {
        //获取IoC容器
		ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml"); 
    }
}
```
步骤7：容器中获取对象进行方法调用
```java
public class App {
    public static void main(String[] args) {
        //获取IoC容器
		ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml"); 
//        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
//        bookDao.save();
        BookService bookService = (BookService) ctx.getBean("bookService");
        bookService.save();
    }
}
```
步骤8：运行程序。测试结果为：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171829365.png)
 
Spring的IoC入门案例已经完成，但**在 `BookServiceImpl` 的类中依然存在 `BookDaoImpl` 对象的 `new` 操作，它们之间的耦合度还是比较高**，这块该如何解决，就需要用到下面的 `DI依赖注入` 。

### 1.7.2 DI入门案例
对于DI的入门案例，我们依然先 `分析思路` 然后再 `代码实现` 。
#### 1. 入门案例思路分析
(1) 要想实现依赖注入，必须要基于IoC管理Bean。DI的入门案例要依赖于前面IoC的入门案例。

(2) `Service` 中使用 `new` 形式创建的 `Dao` 对象是否保留？——需要删除掉，最终要使用IoC容器中的 `bean` 对象

(3) `Service` 中需要的 `Dao` 对象如何进入到 `Service` 中？——**在 `Service` 中提供 `set` 方法，让Spring的IoC容器可以通过该方法传入 `bean` 对象**

(4) **`Service` 与 `Dao` 间的关系如何描述**？——使用配置文件

#### 2. 入门案例代码实现
> 需求：基于IoC入门案例，在 `BookServiceImpl` 类中删除 `new` 对象的方式，使用Spring的DI完成Dao层的注入 
> 1. 删除业务层中使用 `new` 的方式创建的 `dao` 对象 
> 2. **在业务层提供 `BookDao` 的 `setter` 方法**
> 3. **在配置文件中添加依赖注入的配置**
> 4. 运行程序调用方法

步骤1：去除代码中的 `new` 。在 `BookServiceImpl` 类中，删除业务层中使用 `new` 的方式创建的 `dao` 对象。
```java
public class BookServiceImpl implements BookService {
    //删除业务层中使用new的方式创建的dao对象
    private BookDao bookDao; 
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
步骤2：为属性提供 `setter` 方法。在 `BookServiceImpl` 类中，为 `BookDao` 提供 `setter` 方法。
```java
public class BookServiceImpl implements BookService {
    //删除业务层中使用new的方式创建的dao对象
    private BookDao bookDao;

    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
    //提供对应的set方法
    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }
}

```
步骤3：修改配置完成注入。在配置文件中添加依赖注入的配置：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <!--bean标签标示配置bean
    	id属性标示给bean起名字
    	class属性表示给bean定义类型
	-->
    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>

    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <!--配置server与dao的关系-->
        <!--property标签表示配置当前bean的属性
        		name属性表示配置哪一个具体的属性
        		ref属性表示参照哪一个bean,这里是上面的bead id="bookDao"
		-->
        <property name="bookDao" ref="bookDao"/>
    </bean>

</beans>
```
==注意：配置中的两个 `bookDao` 的含义是不一样的==。
* `name="bookDao"` 中 `bookDao` 的作用是，**让Spring的IoC容器在获取到名称后，将首字母大写，前面加 `set` 找对应的 `setBookDao()` 方法进行对象注入**
* `ref="bookDao"` 中 `bookDao` 的作用是，**让Spring能在IoC容器中找到 `id` 为 `bookDao` 的 `Bean` 对象、给`bookService`进行注入**
* 综上所述，对应关系如下：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171842735.png)

步骤4：运行程序。运行，测试结果为：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171829365.png)

---
# 2. IoC相关内容
通过前面两个案例，我们已经学习了 `bean如何定义配置` 、`DI如何定义配置`以及 `容器对象如何获取` 的内容，接下来把这三块内容进行详细讲解，深入学习这三部分，首先是 `bean` 基础配置。
## 2.1 `bean` 基础配置
对于 `bean` 的配置中，主要会讲解 `bean基础配置` 、`bean的别名配置` 、`bean的作用范围配置`（==重点==）这三部分内容。
### 2.1.1 `bean` 基础配置（`id` 与 `class` ）
对于 `bean` 的基础配置，在前面的案例中已经使用过：
```xml
<bean id="" class=""/>
```
其中，`bean` 标签的功能、使用方式以及 `id` 和 `class` 属性的作用，通过一张图来描述：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171848571.png)
其中需要重点掌握的是：`bean` 标签的 `id` 和 `class` 属性的使用。

**思考**：
- `class` 属性能不能写接口如 `BookDao` 的类全名呢？——答案肯定是不行，**因为接口是没办法创建对象的**。
- 前面提过，为 `bean` 设置 `id` 时，`id` 必须唯一，但如果由于命名习惯而产生了分歧后，该如何解决？

在解决这个问题之前，我们需要准备下开发环境，对于开发环境我们可以有两种解决方案：
* 使用前面IoC和DI的案例
* 重新搭建一个新的案例环境，目的是方便查阅代码

搭建的内容和前面的案例是一样的，内容如下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171850821.png)
### 2.1.2 `bean` 的 `name` 属性
环境准备好后，接下来就可以在这个基础上学习 `bean` 的别名配置，首先来看下别名的配置说明：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171851654.png)
步骤1：配置别名。打开Spring的配置文件 `applicationContext.xml` 。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <!--name: 为bean指定别名,别名可以有多个,使用逗号,分号,空格进行分隔-->
    <bean id="bookService" name="service service4 bookEbi" class="com.itheima.service.impl.BookServiceImpl">
        <property name="bookDao" ref="bookDao"/>
    </bean>

    <!--scope: 为bean设置作用范围,可选值为单例singloton,非单例prototype-->
    <bean id="bookDao" name="dao" class="com.itheima.dao.impl.BookDaoImpl"/>
</beans>
```
> 说明：Ebi全称 *Enterprise Business Interface* ，翻译为企业业务接口

步骤2：根据名称容器中获取 `bean` 对象：
```java
public class AppForName {
    public static void main(String[] args) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        //此处根据bean标签的id属性和name属性的任意一个值来获取bean对象
        BookService bookService = (BookService) ctx.getBean("service4");
        bookService.save();
    }
}
```
步骤3：运行程序。测试结果为： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171829365.png)

==注意事项==：`bean` 依赖注入的 `ref` 属性指定 `bean` ，必须在容器中存在
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171915270.png)
如果不存在，则会报错，如下：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171915504.png)
这个错误大家需要特别关注下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171916950.png)
获取 `bean` 无论是通过 `id` 还是 `name` 获取，如果无法获取到，将抛出异常`NoSuchBeanDefinitionException` 。
### 2.1.3 `bean` 作用范围 `scope` 配置
关于 `bean` 的作用范围是 `bean` 属性配置的一个==重点==内容。看到这个作用范围，就得思考 `bean` 的作用范围是来控制 `bean` 哪块内容的？先来看下`bean作用范围的配置属性` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171917386.png)
#### 1. 验证IoC容器中对象是否为单例
验证思路：同一个 `bean` 获取两次，将对象打印到控制台，看打印出的地址值是否一致。

具体实现：创建一个 `AppForScope` 的类，在其 `main` 方法中来验证
```java
public class AppForScope {
  public static void main(String[] args) {
	  ApplicationContext ctx = new 
		  ClassPathXmlApplicationContext("applicationContext.xml");

	  BookDao bookDao1 = (BookDao) ctx.getBean("bookDao");
	  BookDao bookDao2 = (BookDao) ctx.getBean("bookDao");
	  System.out.println(bookDao1);
	  System.out.println(bookDao2);
  }
}
```
打印，观察控制台的打印结果： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171918731.png)
结论：**默认情况下，Spring创建的 `bean` 对象都是单例的**。

获取到结论后，问题就来了，那如果我想创建出来非单例的 `bean` 对象，该如何实现呢？
#### 2. 配置 `bean` 为非单例
在Spring配置文件中，配置 `scope` 属性来实现 `bean` 的非单例创建，在Spring的配置文件中，修改 `<bean>` 的 `scope` 属性：
```xml
<bean id="bookDao" name="dao" class="com.itheima.dao.impl.BookDaoImpl" scope=""/>
```
将 `scope` 设置为 `singleton` ：
```xml
<bean id="bookDao" name="dao" class="com.itheima.dao.impl.BookDaoImpl" scope="singleton"/>
```
运行 `AppForScope` ，打印看结果 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171918731.png)
将 `scope` 设置为`prototype` ：
```xml
<bean id="bookDao" name="dao" class="com.itheima.dao.impl.BookDaoImpl" scope="prototype"/>
```
运行 `AppForScope` ，打印看结果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171926062.png)
结论，使用 `bean` 的 `scope` 属性可以控制 `bean` 的创建是否为单例：
* `singleton` 默认为单例
* `prototype` 为非单例

#### 3. `scope` 使用后续思考
介绍完 `scope` 属性以后，我们来思考几个问题：为什么 `bean` 默认为单例？
* `bean` 为单例的意思是，**在Spring的IoC容器中只会有该类的一个对象**
* `bean` 对象只有一个就**避免了对象的频繁创建与销毁**，达到了 `bean` 对象的复用，性能高

`bean` 在容器中是单例的，会不会产生线程安全问题？
* 如果对象是有状态对象，即该对象有成员变量可以用来存储数据的，因为所有请求线程共用一个 `bean` 对象，所以会存在线程安全问题。
* 如果对象是**无状态对象**，即该对象没有成员变量没有进行数据存储的，因方法中的局部变量在方法调用完成后会被销毁，所以不会存在线程安全问题。

哪些 `bean` 对象适合交给容器进行管理？
* **表现层对象**
* **业务层对象**
* **数据层对象**
* **工具对象**

哪些 `bean` 对象不适合交给容器进行管理？——封装实例的域对象，因为会引发线程安全问题，所以不适合。

## 2.1.4 bean基础配置小结
关于 `bean` 的基础配置中，需要大家掌握以下属性：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171930768.png)

## 2.2 bean实例化
对象已经能交给Spring的IoC容器来创建了，但是容器是如何来创建对象的呢？这就需要研究下 `bean的实例化过程` ，在这块内容中主要解决两部分内容，分别是：
* `bean` 是如何创建的
* **实例化 `bean` 的三种方式，`构造方法` 、`静态工厂` 、`实例工厂`**

在讲解这三种创建方式之前，需要先确认一件事：**`bean` 本质上就是对象，对象在 `new` 时会使用构造方法完成，那创建 `bean` 也是使用构造方法完成的**。基于这个知识点出发，我们来验证Spring中 `bean` 的三种创建方式，
### 2.2.1 环境准备
为了方便阅读代码，重新准备个开发环境，
* 创建一个Maven项目
* `pom.xml` 添加依赖
* `resources` 下添加Spring的配置文件 `applicationContext.xml`

这些步骤和前面的都一致，快速拷贝即可，最终项目的结构如下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302171935420.png)
### 2.2.2 构造方法实例化
在上述的环境下，我们来研究下Spring中的第一种 `bean` 的创建方式——`构造方法实例化` 。

步骤1：准备需要被创建的类。准备 `BookDao, BookDaoImpl` 类。
```java
public interface BookDao {
    public void save();
}
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
}
```
步骤2：将类配置到Spring容器。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

	<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
</beans>
```
步骤3：编写运行程序。
```java
public class AppForInstanceBook {
    public static void main(String[] args) {
        ApplicationContext ctx = new 
            ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        bookDao.save();

    }
}
```
步骤4：类中提供构造函数测试。在 `BookDaoImpl` 类中添加一个无参构造函数，并打印一句话，方便观察结果。
```java
public class BookDaoImpl implements BookDao {
    public BookDaoImpl() {
        System.out.println("book dao constructor is running ....");
    }
    public void save() {
        System.out.println("book dao save ...");
    }
}
```
运行程序，如果控制台有打印构造函数中的输出，说明**Spring容器在创建对象时也走的是构造函数**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172016473.png)
步骤5：将构造函数改成 `private` 测试。
```java
public class BookDaoImpl implements BookDao {
    private BookDaoImpl() {
        System.out.println("book dao constructor is running ....");
    }
    public void save() {
        System.out.println("book dao save ...");
    }
}
```
运行程序，能执行成功，说明内部走的依然是构造函数，**能访问到类中的私有构造方法，显而易见Spring底层用的是反射** 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172016473.png)
步骤6：构造函数中添加一个参数测试。
```java
public class BookDaoImpl implements BookDao {
    private BookDaoImpl(int i) {
        System.out.println("book dao constructor is running ....");
    }
    public void save() {
        System.out.println("book dao save ...");
    }
}
```
运行程序，程序会报错，说明**Spring底层使用的是类的无参构造方法**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172017047.png)

### 2.2.3 分析Spring的错误信息
接下来，我们主要研究下Spring的报错信息来学一学。**错误信息从下往上依次查看**，因为上面的错误大都是对下面错误的一个包装，最核心错误是在最下面。
* Caused by: java.lang.NoSuchMethodException: com.itheima.dao.impl.BookDaoImpl.`<init>`()
	* Caused by 翻译为 `引起` ，即出现错误的原因
	* java.lang.NoSuchMethodException ：抛出的异常为 `没有这样的方法异常`
	* com.itheima.dao.impl.BookDaoImpl.`<init>`() ：哪个类的哪个方法没有被找到导致的异常，`<init>`() 指定是类的构造方法，即该类的无参构造方法

如果最后一行错误获取不到错误信息，接下来查看第二层：
Caused by: org.springframework.beans.BeanInstantiationException: Failed to instantiate [com.itheima.dao.impl.BookDaoImpl]: No default constructor found; nested exception is java.lang.NoSuchMethodException: com.itheima.dao.impl.BookDaoImpl.`<init>`()
* nested是嵌套的意思，后面的异常内容和最底层的异常是一致的
* Caused by: org.springframework.beans.BeanInstantiationException: Failed to instantiate [com.itheima.dao.impl.BookDaoImpl]: No default constructor found; 
	* Caused by：`引发`
	* BeanInstantiationException：翻译为 `bean实例化异常`
	* No default constructor found：没有一个默认的构造函数被发现

看到这其实错误已经比较明显，把倒数第三层的错误分析下：
Exception in thread "main" org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'bookDao' defined in class path resource [applicationContext.xml]: Instantiation of bean failed; nested exception is org.springframework.beans.BeanInstantiationException: Failed to instantiate [com.itheima.dao.impl.BookDaoImpl]: No default constructor found; nested exception is java.lang.NoSuchMethodException: com.itheima.dao.impl.BookDaoImpl.`<init>`()。

至此，关于Spring的构造方法实例化就已经学习完了，==因为每一个类默认都会提供一个无参构造函数，所以其实真正在使用这种方式时，我们什么也不需要做==。这也是以后比较常用的一种方式。

### 2.2.4 静态工厂实例化
接下来研究Spring中的第二种 `bean` 的创建方式 `静态工厂实例化` 。
#### 1. 工厂方式创建 `bean`
在讲这种方式之前，需要先回顾一个知识点——使用工厂来创建对象的方式：
(1) 准备一个 `OrderDao` 和 `OrderDaoImpl` 类：
```java
public interface OrderDao {
    public void save();
}
public class OrderDaoImpl implements OrderDao {
    public void save() {
        System.out.println("order dao save ...");
    }
}
```
(2) 创建一个工厂类 `OrderDaoFactory` 并提供一个==静态方法==：
```java
//静态工厂创建对象
public class OrderDaoFactory {
    public static OrderDao getOrderDao(){
        return new OrderDaoImpl();
    }
}
```
(3) 编写 `AppForInstanceOrder` 运行类，在类中通过工厂获取对象：
```java
public class AppForInstanceOrder {
    public static void main(String[] args) {
        //通过静态工厂创建对象
        OrderDao orderDao = OrderDaoFactory.getOrderDao();
        orderDao.save();
    }
}
```
(4) 运行后，可以查看到结果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172021909.png)
如果代码中对象是通过上面的这种方式来创建的，如何将其交给Spring来管理呢？
#### 2. 静态工厂实例化
这就要用到Spring中的静态工厂实例化的知识了（兼容早期的遗留代码），具体实现步骤如下。
(1) 在Spring的配置文件 `applicationContext.xml` 中添加以下内容：
```xml
<bean id="orderDao" class="com.itheima.factory.OrderDaoFactory" factory-method="getOrderDao"/>
```
`class` ：工厂类的类全名
`factory-mehod` ：具体工厂类中创建对象的方法名。对应关系如下图： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172024566.png)

(2) 在 `AppForInstanceOrder` 运行类，使用从IoC容器中获取 `bean` 的方法进行运行测试：
```java
public class AppForInstanceOrder {
    public static void main(String[] args) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        OrderDao orderDao = (OrderDao) ctx.getBean("orderDao");
        orderDao.save();
    }
}
```
(3) 运行后，可以查看到结果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172021909.png)

看到这，可能有人会问了，**你这种方式在工厂类中不也是直接 `new` 对象的，和我自己直接 `new` 没什么太大的区别**，而且静态工厂的方式反而更复杂，这种方式的意义是什么？主要的原因是——在工厂的静态方法中，**我们除了 `new` 对象还可以做其他的一些业务操作**，这些操作必不可少，如：
```java
public class OrderDaoFactory {
    public static OrderDao getOrderDao(){
        System.out.println("factory setup....");//模拟必要的业务操作
        return new OrderDaoImpl();
    }
}
```
之前 `new` 对象的方式就无法添加其他的业务内容，重新运行，查看结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172030519.png)
介绍完静态工厂实例化后，这种方式一般是用来兼容早期的一些老系统，所以==了解为主==。
### 2.2.5 实例工厂与 `FactoryBean`
接下来继续来研究，Spring的第三种 `bean` 的创建方式 `实例工厂实例化` 。
#### 1. 环境准备
(1) 准备一个 `UserDao, UserDaoImpl` 类
```java
public interface UserDao {
    public void save();
}
public class UserDaoImpl implements UserDao {
    public void save() {
        System.out.println("user dao save ...");
    }
}
```
(2) 创建一个工厂类 `OrderDaoFactory` 并提供一个普通方法，注意此处和静态工厂的工厂类不一样的地方是方法不是静态方法：
```java
public class UserDaoFactory {
    public UserDao getUserDao(){
        return new UserDaoImpl();
    }
}
```
(3) 编写 `AppForInstanceUser` 运行类，在类中通过工厂获取对象：
```java
public class AppForInstanceUser {
    public static void main(String[] args) {
        //创建实例工厂对象
        UserDaoFactory userDaoFactory = new UserDaoFactory();
        //通过实例工厂对象创建对象
        UserDao userDao = userDaoFactory.getUserDao();
        userDao.save();
}
```
(4) 运行后，可以查看到结果
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172038488.png)
对于上面这种实例工厂的方式如何交给Spring管理呢？
#### 2. 实例工厂实例化
具体实现步骤为：
(1) 在Spring的配置文件中添加以下内容：
```xml
<bean id="userFactory" class="com.itheima.factory.UserDaoFactory"/>
<bean id="userDao" factory-method="getUserDao" factory-bean="userFactory"/>
```
实例化工厂运行的顺序是：
* 创建实例化工厂对象，对应的是第一行配置
* 调用对象中的方法来创建 `bean` ，对应的是第二行配置
	* `factory-bean` ：工厂的实例对象
	* `factory-method` ：工厂对象中具体创建对象的方法名，对应关系如下：  
		![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172044343.png)
 
(2) 在 `AppForInstanceUser` 运行类，使用从IoC容器中获取 `bean` 的方法进行运行测试：
```java
public class AppForInstanceUser {
    public static void main(String[] args) {
        ApplicationContext ctx = new 
            ClassPathXmlApplicationContext("applicationContext.xml");
        UserDao userDao = (UserDao) ctx.getBean("userDao");
        userDao.save();
    }
}
```
(3) 运行后，可以查看到结果： 
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172038488.png)
实例工厂实例化的方式就已经介绍完了，配置的过程还是比较复杂，所以Spring为了简化这种配置方式，就提供了一种叫 `FactoryBean` 的方式来简化开发（**后面很实用**）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172049253.png)
#### 3. `FactoryBean` 的使用
具体的使用步骤为：
(1) 创建一个 `UserDaoFactoryBean` 的类，**实现 `FactoryBean` 接口，重写接口的方法**：
```java
public class UserDaoFactoryBean implements FactoryBean<UserDao> {
    // 代替原始实例工厂中创建对象的方法
    // 以后创建UserDao对象时统一使用这个工厂Bean
    public UserDao getObject() throws Exception {
        return new UserDaoImpl();
    }
    // 返回所创建类的Class对象
    public Class<?> getObjectType() {
        return UserDao.class;
    }
}
```
(2) 在Spring的配置文件中进行配置：
```xml
<bean id="userDao" class="com.itheima.factory.UserDaoFactoryBean"/>
```
(3) `AppForInstanceUser` 运行类不用做任何修改，直接运行：
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172038488.png)
==这种方式在Spring去整合其他框架时会被用到，所以这种方式需要大家理解掌握==。

查看源码会发现，`FactoryBean` 接口其实会有三个方法，分别是：
```java
T getObject() throws Exception;
Class<?> getObjectType();
default boolean isSingleton() {
		return true;
}
```
- 方法一 `getObject()` 被重写后，**在方法中进行对象的创建并返回**
- 方法二 `getObjectType()` 被重写后，主要**返回的是被创建类的 `Class` 对象**
- 方法三:没有被重写，因为它已经给了默认值，从方法名中可以看出其作用是设置对象是否为单例，默认 `true` ，从意思上来看，我们猜想默认应该是单例，如何来验证呢？

思路很简单，就是从容器中获取该对象的多个值，打印到控制台，查看是否为同一个对象。
```java
public class AppForInstanceUser {
    public static void main(String[] args) {
        ApplicationContext ctx = new 
            ClassPathXmlApplicationContext("applicationContext.xml");
        UserDao userDao1 = (UserDao) ctx.getBean("userDao");
        UserDao userDao2 = (UserDao) ctx.getBean("userDao");
        System.out.println(userDao1);
        System.out.println(userDao2);
    }
}
```
打印结果，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172057919.png)
通过验证，会发现默认是单例，那如果想改成单例具体如何实现？只需要将 `isSingleton()` 方法进行重写，修改返回为 `false` ，即可
```java
//FactoryBean创建对象
public class UserDaoFactoryBean implements FactoryBean<UserDao> {
    //代替原始实例工厂中创建对象的方法
    public UserDao getObject() throws Exception {
        return new UserDaoImpl();
    }
    public Class<?> getObjectType() {
        return UserDao.class;
    }
    public boolean isSingleton() {
        return false;
    }
}
```
重新运行 `AppForInstanceUser` ，查看结果： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172058918.png)
从结果中可以看出现在已经是非单例了，但是一般情况下我们都会采用单例，也就是采用默认即可。所以 `isSingleton()` 方法一般不需要进行重写。
### 2.2.6 `bean` 实例化小结
通过这一节的学习，需要掌握：
(1) `bean` 是如何创建的呢？——构造方法

(2) Spring的IoC实例化对象的三种方式分别是：
* 构造方法(常用)
* 静态工厂(了解)
* 实例工厂(了解)
	* `FactoryBean` (实用)

这些方式中，重点掌握 `构造方法` 和 `FactoryBean` 即可。

需要注意的一点是，==构造方法在类中默认会提供，但如果重写了构造方法，默认的就会消失==，在使用的过程中需要注意，==如果需要重写构造方法，最好把默认的构造方法也重写下==。

## 2.3 `bean` 的生命周期
关于 `bean` 的相关知识还有最后一个，是 `bean的生命周期` 。对于生命周期，我们主要围绕 `bean生命周期控制` 来讲解。首先理解下什么是生命周期？从创建到消亡的完整过程，例如人从出生到死亡的整个过程就是一个生命周期。

`bean` 生命周期是什么？`bean` 对象从创建到销毁的整体过程。`bean` 生命周期控制是什么？在 `bean` 创建后到销毁前做一些事情。

现在我们面临的问题是，如何在 `bean` 的创建之后和销毁之前、把我们需要添加的内容添加进去。
### 2.3.1 环境准备
还是老规矩，为了方便后期代码的阅读，我们重新搭建下环境：
- 创建一个Maven项目
- `pom.xml` 添加依赖
- `resources` 下添加Spring的配置文件 `applicationContext.xml`

这些步骤和前面的都一致，大家可以快速的拷贝即可，最终项目的结构如下： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172115320.png)
(1) 项目中添加 `BookDao, BookDaoImpl, BookService, BookServiceImpl` 类：
```java
public interface BookDao {
    public void save();
}
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
}
public interface BookService {
    public void save();
}
public class BookServiceImpl implements BookService {
    private BookDao bookDao;

    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }

    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
(2) `resources` 下提供Spring的配置文件：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
</beans>
```
(3) 编写 `AppForLifeCycle` 运行类，加载Spring的IoC容器，并从中获取对应的 `bean` 对象：
```java
public class AppForLifeCycle {
    public static void main( String[] args ) {
        ApplicationContext ctx = new 
        	ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        bookDao.save();
    }
}
```
### 2.3.2 生命周期设置
接下来，在上面这个环境中为 `BookDao` 添加生命周期的控制方法，具体的控制有两个阶段：
* `bean` 创建之后，想要添加内容，比如用来初始化需要用到资源
* `bean` 销毁之前，想要添加内容，比如用来释放用到的资源

步骤1：添加初始化和销毁方法。针对这两个阶段，我们在 `BooDaoImpl` 类中分别添加两个方法，==方法名任意==：
```java
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
    //表示bean初始化对应的操作
    public void init() {
        System.out.println("init...");
    }
    //表示bean销毁前对应的操作
    public void destory() {
        System.out.println("destory...");
    }
}
```
步骤2：**配置生命周期**。在配置文件添加配置，如下：
```xml
<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl" init-method="init" destroy-method="destory"/>
```
步骤3：运行程序，运行 `AppForLifeCycle` 打印结果为： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172123668.png)
从结果中可以看出，`init` 方法执行了，但 `destroy` 方法却未执行，这是为什么呢？
* **Spring的IoC容器是运行在JVM中**
* 运行 `main` 方法后，JVM启动，Spring加载配置文件生成IoC容器，从容器获取 `bean` 对象，然后调方法执行
* `main` 方法执行完后，**JVM退出**，这个时候IoC容器中的 `bean` 还没有来得及销毁就已经结束了
* 所以没有调用对应的 `destroy` 方法

知道了出现问题的原因，具体该如何解决呢？
### 2.3.3 `close` 关闭容器
`ApplicationContext` 中没有 `close` 方法，需要将 `ApplicationContext` 更换成 `ClassPathXmlApplicationContext` ：
```java
ClassPathXmlApplicationContext ctx = new 
  ClassPathXmlApplicationContext("applicationContext.xml");
```
调用 `ctx` 的 `close()` 方法：
```java
ctx.close();
```
运行程序，就能执行 `destroy` 方法的内容：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302172125852.png)
### 2.3.4 注册关闭钩子关闭容器
==在容器未关闭之前，提前设置好回调函数，**让JVM在退出之前回调此函数来关闭容器**==。调用 `ctx` 的 `registerShutdownHook()` 方法：
```java
ctx.registerShutdownHook();
```
**注意**：`registerShutdownHook` 在 `ApplicationContext` 中也没有。

运行后，查询打印结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180059875.png)
两种方式介绍完后，`close` 和 `registerShutdownHook` 选哪个？相同点：这两种都能用来关闭容器。不同点：`close()` 是在调用时关闭容器， `registerShutdownHook()` 是在JVM退出前调用关闭。

分析上面的实现过程，会发现==添加初始化和销毁方法，即需要编码也需要配置，实现起来步骤比较多也比较乱==。

Spring提供了两个接口来完成生命周期的控制，好处是**可以不用再进行配置 `init-method` 和 `destroy-method`** 。接下来在 `BookServiceImpl` 完成这两个接口的使用:

修改 `BookServiceImpl` 类，添加两个接口 `InitializingBean`， `DisposableBean` ，并实现接口中的两个方法 `afterPropertiesSet` 和 `destroy` ：
```java
public class BookServiceImpl implements BookService, InitializingBean, DisposableBean {
    private BookDao bookDao;
    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }
    public void save() {
        System.out.println("book service save ...");
        bookDao.save(); 
    }
    public void destroy() throws Exception {
        System.out.println("service destroy");
    }
    public void afterPropertiesSet() throws Exception {
        System.out.println("service init");
    }
}
```
重新运行 `AppForLifeCycle` 类， ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180103417.png)
那第二种方式的实现，我们也介绍完了。

**小细节**
对于 `InitializingBean` 接口中的 `afterPropertiesSet` 方法，翻译过来为`属性设置之后` 。对于 `BookServiceImpl` 来说，`bookDao` 是它的一个属性，而 `setBookDao` 方法是**Spring的IoC容器为其注入属性的方法**。思考：`afterPropertiesSet` 和 `setBookDao` 谁先执行？

从方法名分析，猜想应该是 `setBookDao` 方法先执行。验证思路，在 `setBookDao` 方法中添加一句话：
```java
public void setBookDao(BookDao bookDao) {
	System.out.println("set .....");
	this.bookDao = bookDao;
}
```
重新运行 `AppForLifeCycle` ，打印结果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180105533.png)
验证的结果和我们猜想的结果是一致的，所以初始化方法会在类中属性设置之后执行。

### 2.3.5 `bean` 生命周期小结
(1) 关于Spring中对 `bean` 生命周期控制提供了两种方式:
* 在配置文件中的 `bean` 标签中添加 `init-method` 和 `destroy-method` 属性
* 类实现 `InitializingBean` 与 `DisposableBean` 接口，这种方式了解下即可。

(2) 对于 `bean` 的生命周期控制在「 `bean` 的整个生命周期」中所处的位置如下：
* 初始化容器
	1. 创建对象（内存分配）
	2. 执行构造方法
	3. 执行属性注入（`set` 操作）
	4. **执行bean初始化方法**
* 使用 `bean`
	1. 执行业务操作
* 关闭/销毁容器
	1. **执行 `bean` 销毁方法**

(3) 关闭容器的两种方式:
* `ConfigurableApplicationContext` 是 `ApplicationContext` 的子类
	* `close()` 方法
	* `registerShutdownHook()` 方法

---
# 3. DI相关内容
前面已经完成了 `bean` 相关操作的讲解，接下来就进入第二个大的模块 `DI依赖注入` ，首先介绍Spring中有哪些注入方式？我们先来思考，**向一个类中传递数据的方式有几种**？
* 普通方法（`set` 方法）
* 构造方法

依赖注入描述了**在容器中建立 `bean` 与 `bean` 之间的依赖关系的过程**，如果 `bean` 运行需要的是数字或字符串呢?
* 引用类型
* 简单类型（基本数据类型与String）

Spring就是基于上面这些知识点，为我们提供了两种注入方式，分别是：
* `setter` 注入
	* 简单类型
	* 引用类型
* 构造器注入
	* 简单类型
	* 引用类型

依赖注入的方式已经介绍完，接下来挨个学习下。
## 3.1 `setter` 注入
`setter` 方式注入引用类型的方式之前学习过，快速回顾：在 `bean` 中定义引用类型属性，并提供可访问的 `set` 方法。
```java
public class BookServiceImpl implements BookService {
    private BookDao bookDao;
    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }
}
```
**配置中使用 `property` 标签 `ref` 属性注入引用类型对象**：
```xml
<bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
	<property name="bookDao" ref="bookDao"/>
</bean>

<bean id="bookDao" class="com.itheima.dao.imipl.BookDaoImpl"/>
```
### 3.1.1 环境准备
为了更好的学习下面内容，我们依旧准备一个新环境：
- 创建一个Maven项目
- `pom.xml` 添加依赖
- `resources` 下添加Spring的配置文件

这些步骤和前面的都一致，大家快速拷贝即可，最终项目的结构如下：
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180114590.png)
(1) 项目中添加 `BookDao, BookDaoImpl, UserDao, UserDaoImpl, BookService, BookServiceImpl` 类：
```java
public interface BookDao {
    public void save();
}
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
}
public interface UserDao {
    public void save();
}
public class UserDaoImpl implements UserDao {
    public void save() {
        System.out.println("user dao save ...");
    }
}
public interface BookService {
    public void save();
}
public class BookServiceImpl implements BookService{
    private BookDao bookDao;

    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }

    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
(2) `resources` 下提供Spring的配置文件：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <property name="bookDao" ref="bookDao"/>
    </bean>
</beans>
```
(3) 编写 `AppForDISet` 运行类，加载Spring的IoC容器，并从中获取对应的 `bean` 对象：
```java
public class AppForDISet {
    public static void main( String[] args ) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        BookService bookService = (BookService) ctx.getBean("bookService");
        bookService.save();
    }
}
```
接下来，在上面这个环境中来完成 `setter` 注入的学习。
### 3.1.2 注入引用数据类型
> 需求：在 `bookServiceImpl` 对象中注入 `userDao` 
> 1. 在 `BookServiceImpl` 中声明 `userDao` 属性
> 2. 为 `userDao` 属性提供 `setter` 方法
> 3. 在配置文件中使用 `property` 标签注入

步骤1：声明属性并提供 `setter` 方法。在 `BookServiceImpl` 中声明 `userDao` 属性，并提供 `setter` 方法：
```java
public class BookServiceImpl implements BookService{
    private BookDao bookDao;
    private UserDao userDao;
    
    public void setUserDao(UserDao userDao) {
        this.userDao = userDao;
    }
    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }

    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
        userDao.save();
    }
}
```
步骤2：配置文件中进行注入配置。在 `applicationContext.xml` 配置文件中使用 `property` 标签注入。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
    <bean id="userDao" class="com.itheima.dao.impl.UserDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <property name="bookDao" ref="bookDao"/>
        <property name="userDao" ref="userDao"/>
    </bean>
</beans>
```
步骤3：运行程序。运行 `AppForDISet` 类，查看结果，说明 `userDao` 已经成功注入。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180118756.png)
### 3.1.3 注入简单数据类型
> 需求：给 `BookDaoImpl` 注入一些简单数据类型的数据。参考引用数据类型的注入，我们可以推出具体的步骤为：
> 1. 在 `BookDaoImpl` 类中声明对应的简单数据类型的属性
> 2. 为这些属性提供对应的 `setter`方法
> 3. 在 `applicationContext.xml` 中配置

**思考**：**引用类型使用的是 `<property name="" ref=""/>`** ，简单数据类型还是使用 `ref` 吗？`ref` 是指向Spring的IoC容器中的另一个 `bean` 对象的，对于简单数据类型，没有对应的 `bean` 对象，该如何配置？

步骤1：声明属性并提供 `setter` 方法。在 `BookDaoImpl` 类中声明对应的简单数据类型的属性，并提供对应的 `setter` 方法：
```java
public class BookDaoImpl implements BookDao {
    private String databaseName;
    private int connectionNum;
    public void setConnectionNum(int connectionNum) {
        this.connectionNum = connectionNum;
    }
    public void setDatabaseName(String databaseName) {
        this.databaseName = databaseName;
    }
    public void save() {
        System.out.println("book dao save ..."+databaseName+","+connectionNum);
    }
}
```
步骤2：配置文件中进行注入配置。在 `applicationContext.xml` 配置文件中使用 `property` 标签注入。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
        <property name="databaseName" value="mysql"/>
     	<property name="connectionNum" value="10"/>
    </bean>
    <bean id="userDao" class="com.itheima.dao.impl.UserDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <property name="bookDao" ref="bookDao"/>
        <property name="userDao" ref="userDao"/>
    </bean>
</beans>
```
**说明**：**`value` 后面跟的是简单数据类型**。对于参数类型，Spring在注入时会自动转换，但是不能写成：
```xml
<property name="connectionNum" value="abc"/>
```
这样的话，Spring在将 `abc` 转换成 `int` 类型时就会报错。

步骤3：运行程序。运行 `AppForDISet` 类，查看结果，说明 `userDao` 已经成功注入。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180118756.png)**注意**：两个 `property` 注入标签的顺序可以任意。

对于 `setter` 注入方式的基本使用就已经介绍完了，
* 对于引用数据类型使用的是 `<property name="" ref=""/>`
* 对于简单数据类型使用的是 `<property name="" value=""/>`

## 3.2 构造器注入
### 3.2.1 环境准备
构造器注入也就是构造方法注入，学习之前，还是准备下环境，和之前一样，只是没使用 `UserDao` 类。
### 3.2.2 构造器注入引用数据类型
接下来，在上面环境中完成构造器注入的学习：
> 需求：将 `BookServiceImpl` 类中的 `bookDao` 修改成使用构造器的方式注入。
> 1. 将 `bookDao` 的 `setter` 方法删除掉
> 2. 添加带有 `bookDao` 参数的构造方法
> 3. 在 `applicationContext.xml` 中配置

步骤1：删除 `setter` 方法并提供构造方法。在 `BookServiceImpl` 类中将 `bookDao` 的 `setter` 方法删除掉，并添加带有 `bookDao` 参数的构造方法。
```java
public class BookServiceImpl implements BookService{
    private BookDao bookDao;

    public BookServiceImpl(BookDao bookDao) {
        this.bookDao = bookDao;
    }
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
步骤2：配置文件中进行配置构造方式注入。在 `applicationContext.xml` 中配置。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <constructor-arg name="bookDao" ref="bookDao"/>
    </bean>
</beans>
```
==**说明**==：标签 `<constructor-arg>` 中
* **`name` 属性对应的值为「构造函数中方法形参的参数名」，必须要保持一致**。
* `ref` 属性指向的是Spring的IoC容器中其他 `bean` 对象。

步骤3：运行程序。运行 `AppForDIConstructor` 类，查看结果，说明 `bookDao` 已经成功注入。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180202519.png)
### 3.2.3 构造器注入多个引用数据类型
> 需求：在 `BookServiceImpl` 使用构造函数注入多个引用数据类型，比如 `IserDao` 。
> 1. 声明 `userDao` 属性
> 2. 生成一个带有 `bookDao` 和 `userDao`  `参数的构造函数
> 3. 在 `applicationContext.xml` 中配置注入

步骤1：提供多个属性的构造函数。在 `BookServiceImpl` 声明 `userDao` 并提供多个参数的构造函数。
```java
public class BookServiceImpl implements BookService{
    private BookDao bookDao;
    private UserDao userDao;
    public BookServiceImpl(BookDao bookDao,UserDao userDao) {
        this.bookDao = bookDao;
        this.userDao = userDao;
    }
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
        userDao.save();
    }
}
```
步骤2：配置文件中配置多参数注入。在 `applicationContext.xml` 中配置注入。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
    <bean id="userDao" class="com.itheima.dao.impl.UserDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <constructor-arg name="bookDao" ref="bookDao"/>
        <constructor-arg name="userDao" ref="userDao"/>
    </bean>
</beans>
```
**说明**：这两个 `<contructor-arg>` 的配置顺序可以任意。

步骤3：运行程序。运行 `AppForDIConstructor` 类，查看结果，说明 `userDao` 已经成功注入。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180212287.png)
### 3.2.4 构造器注入多个简单数据类型
> 需求：在 `BookDaoImpl` 中，使用构造函数注入 `databaseName` 和 `connectionNum` 两个参数。
> 参考引用数据类型的注入，我们可以推出具体的步骤为:
> 1. 提供一个包含这两个参数的构造方法
> 2. 在 `applicationContext.xml` 中进行注入配置

步骤1：添加多个简单属性并提供构造方法。修改 `BookDaoImpl` 类，添加构造方法。
```java
public class BookDaoImpl implements BookDao {
    private String databaseName;
    private int connectionNum;
    public BookDaoImpl(String databaseName, int connectionNum) {
        this.databaseName = databaseName;
        this.connectionNum = connectionNum;
    }
    public void save() {
        System.out.println("book dao save ..."+databaseName+","+connectionNum);
    }
}
```
步骤2：配置完成多个属性构造器注入。在 `applicationContext.xml` 中进行注入配置。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
        <constructor-arg name="databaseName" value="mysql"/>
        <constructor-arg name="connectionNum" value="666"/>
    </bean>
    <bean id="userDao" class="com.itheima.dao.impl.UserDaoImpl"/>
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl">
        <constructor-arg name="bookDao" ref="bookDao"/>
        <constructor-arg name="userDao" ref="userDao"/>
    </bean>
</beans>
```
步骤3：运行程序。运行 `AppForDIConstructor` 类，查看结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180214708.png)

上面已经完成了构造函数注入的基本使用，但会存在一些问题：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180214514.png)
* 当构造函数中方法的参数名发生变化后，配置文件中的 `name` 属性也需要跟着变
* **这两块存在紧耦合**，具体该如何解决?

在解决这个问题之前，需要提前说明的是，==这个参数名发生变化的情况并不多，所以上面的还是比较主流的配置方式==，下面介绍的，大家都以了解为主。

方式一：删除 `name` 属性，添加 `type` 属性，按照类型注入。
```xml
<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
    <constructor-arg type="int" value="10"/>
    <constructor-arg type="java.lang.String" value="mysql"/>
</bean>
```
* 这种方式可以解决「构造函数形参名发生变化」带来的耦合问题
* ==但如果构造方法参数中有类型相同的参数，这种方式就不太好实现了==

方式二：删除 `type` 属性，添加 `index` 属性，按照索引下标注入，下标从0开始：
```xml
<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
    <constructor-arg index="1" value="100"/>
    <constructor-arg index="0" value="mysql"/>
</bean>
```
* 这种方式可以解决参数类型重复问题
* ==但如果构造方法参数顺序发生变化后，这种方式又带来了耦合问题==

介绍完两种参数的注入方式，具体我们该如何选择呢？
1. **强制依赖（指对象在创建的过程中必须要注入指定的参数）使用构造器进行**，使用 `setter` 注入有概率不进行注入、导致 `null` 对象出现
2. **可选依赖（对象在创建过程中注入的参数可有可无）使用 `setter` 注入进行**，灵活性强
3. **Spring框架倡导使用构造器**，第三方框架内部大多数采用构造器注入的形式进行数据初始化，相对严谨
4. 如果有必要可以两者同时使用，使用构造器注入完成强制依赖的注入，使用 `setter` 注入完成可选依赖的注入
5. 实际开发过程中还要根据实际情况分析，**如果受控对象没有提供 `setter` 方法就必须使用构造器注入**
6. **==自己开发的模块推荐使用 `setter` 注入==**

这节中主要讲解的是「Spring依赖注入」的实现方式：
* `setter` 注入
	* 简单数据类型
    ```xml
    <bean ...>
    	<property name="" value=""/>
    </bean>
    ```
	* 引用数据类型
    ```xml
    <bean ...>
    	<property name="" ref=""/>
    </bean>
    ```
* 构造器注入
	* 简单数据类型
    ```xml
    <bean ...>
    	<constructor-arg name="" index="" type="" value=""/>
    </bean>
    ```
	* 引用数据类型
    ```xml
    <bean ...>
    	<constructor-arg name="" index="" type="" ref=""/>
    </bean>
    ```
* 依赖注入的方式选择上
	* 建议使用 `setter` 注入
	* 第三方技术根据情况选择

## 3.3 自动配置
前面花了大量的时间把Spring的注入去学习了下，总结起来就一个字==麻烦==。
> 问：麻烦在哪？
> 答：配置文件的编写配置上。
> 问：有更简单方式么？
> 答：有，自动配置。

什么是自动配置以及如何实现自动配置，就是接下来要学习的内容。

**IoC容器根据 `bean` 所依赖的资源，在容器中自动查找、并注入到 `bean` 中的过程**，称为自动装配。自动装配方式有哪些？
* ==按类型（常用）==
* 按名称
* 按构造方法
* 不启用自动装配准备案例环境同上。
### 3.3.1 完成自动装配的配置
接下来，在上面环境完成 `自动装配` 的学习。自动装配只需要修改 `applicationContext.xml` 配置文件即可：
(1) 将 `<property>` 标签删除
(2) 在 `<bean>` 标签中添加 `autowire` 属性

首先实现按照类型注入的配置：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean class="com.itheima.dao.impl.BookDaoImpl"/>
    <!--autowire属性：开启自动装配，通常使用按类型装配-->
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl" autowire="byType"/>
</beans>
```
==注意事项==：
* 需要注入属性的类中，**对应属性的 `setter` 方法不能省略**
* **被注入的对象必须要被Spring的IoC容器管理**
* 按照类型在Spring的IoC容器中，**如果找到多个对象，会报`NoUniqueBeanDefinitionException`**

==一个类型在IoC中有多个对象，还想要注入成功，这个时候就需要按照名称注入==，配置方式为：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean class="com.itheima.dao.impl.BookDaoImpl"/>
    <!--autowire属性：开启自动装配，通常使用按类型装配-->
    <bean id="bookService" class="com.itheima.service.impl.BookServiceImpl" autowire="byName"/>

</beans>
```
==注意事项==：按照名称注入中的名称指的是什么？指的是属性名（其实是把 `set` 后首字母小写的名字），要与某个 `bean` 中的 `id` 匹配。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180230043.png)
* `bookDao` 是 `private` 修饰的，外部类无法直接方法，外部类只能通过属性的 `set` 方法进行访问
* 对外部类来说，`setBookDao` 方法名，**去掉 `set` 后首字母小写**是其属性名
* 为什么是去掉 `set` 首字母小写？这个规则是 `set` 方法生成的默认规则，**`set` 方法的生成是把属性名首字母大写前面加 `set` 形成的方法名**
* 所以**按照名称注入，其实是和对应的 `set` 方法有关**，但如果按照标准起名称，属性名和 `set` 方法对应的名是一致的

如果**按照名称去找对应的 `bean` 对象**（要保证至少有一个 `bean` 的 `id` 与其名称匹配），找不到则注入 `Null` ；**当某一个类型在IoC容器中有多个对象，按照名称注入只找其指定名称对应的 `bean` 对象，不会报错**。

两种方式介绍完后，以后用的更多的是==按照类型==注入。最后对于依赖注入，需要注意一些其他的配置特征：
1. ==自动装配用于引用类型依赖注入，不能对简单类型进行操作==
2. 使用按类型装配时 `byType` ，**必须保障容器中相同类型的 `bean` 唯一**，推荐使用
3. 使用按名称装配时 `byName` ，**必须保障容器中具有指定名称的 `bean`** ，因变量名与配置耦合，**不推荐使用**
4. **自动装配优先级低于 `setter` 注入与构造器注入**，同时出现时自动装配配置失效

## 3.4 集合注入
前面已经能完成引入数据类型和简单数据类型的注入，但还有一种数据类型——==集合==，集合中既可以装简单数据类型也可以装引用数据类型，对于集合，在Spring中该如何注入呢？先来回顾下，常见的集合类型有哪些？
* 数组
* List
* Set
* Map
* Properties

针对不同的集合类型，该如何实现注入呢？
### 3.4.1 环境准备
- 创建一个Maven项目
- `pom.xml` 添加依赖
- `resources` 下添加Spring的配置文件 `applicationContext.xml`

这些步骤和前面的都一致，快速拷贝即可。

(1) 项目中添加 `BookDao, BookDaoImpl` 类：
```java
public interface BookDao {
    public void save();
}
public class BookDaoImpl implements BookDao {
    private int[] array;
    private List<String> list;
    private Set<String> set;
    private Map<String,String> map;
    private Properties properties;

     public void save() {
        System.out.println("book dao save ...");
        System.out.println("遍历数组:" + Arrays.toString(array));
        System.out.println("遍历List" + list);
        System.out.println("遍历Set" + set);
        System.out.println("遍历Map" + map);
        System.out.println("遍历Properties" + properties);
    }
	//setter....方法省略，自己使用工具生成
}
```
(2) `resources` 下提供Spring的配置文件，`applicationContext.xml` ：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
</beans>
```
(3) 编写 `AppForDICollection` 运行类，加载Spring的IoC容器，并从中获取对应的 `bean` 对象：
```java
public class AppForDICollection {
    public static void main( String[] args ) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        bookDao.save();
    }
}
```
接下来，在上面环境中完成 `集合注入` 的学习。下面所有配置方式，**都是在 `bookDao` 的 `bean` 标签中使用 `<property>` 进行注入**。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">

    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
        
    </bean>
</beans>
```
### 3.4.2 注入数组类型数据
如果有多个数组呢？——看 `property` 的 `name` 属性。数组中不是基本类型呢？
```xml
<property name="array">
    <array>
        <value>100</value>
        <value>200</value>
        <value>300</value>
    </array>
</property>
```
### 3.4.3 注入 `List` 类型数据
```xml
<property name="list">
    <list>
        <value>itcast</value>
        <value>itheima</value>
        <value>boxuegu</value>
        <value>chuanzhihui</value>
    </list>
</property>
```
### 3.4.4 注入 `Set` 类型数据
```xml
<property name="set">
    <set>
        <value>itcast</value>
        <value>itheima</value>
        <value>boxuegu</value>
        <value>boxuegu</value>
    </set>
</property>
```
### 3.4.5 注入 `Map` 类型数据
```xml
<property name="map">
    <map>
        <entry key="country" value="china"/>
        <entry key="province" value="henan"/>
        <entry key="city" value="kaifeng"/>
    </map>
</property>
```
### 3.4.6 注入 `Properties` 类型数据
```xml
<property name="properties">
    <props>
        <prop key="country">china</prop>
        <prop key="province">henan</prop>
        <prop key="city">kaifeng</prop>
    </props>
</property>
```
配置完成后，运行下看结果： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180251332.png)

**说明**：
* **`property` 标签表示 `setter` 方式注入**，构造方式注入 `constructor-arg` 标签内部也可以写 `<array>, <list>, <set>, <map>, <props>` 标签
* `List` 的底层也是通过数组实现的，所以 `<list>` 和` <array>` 标签是可以混用
* 集合中要添加引用类型，只需要把 `<value>` 标签改成` <ref bean="beanId">` 标签，**这种方式用的比较少**

---
 > * 掌握IoC/DI**配置管理**第三方bean
> * 掌握IoC/DI的**注解开发**
> * 掌握IoC/DI**注解管理**第三方bean
> * 完成Spring与Mybatis及Junit的**整合开发**

# 4. IoC/DI配置管理第三方 `bean`
前面所讲知识点都基于我们自己写的类，现在如果有需求让我们去管理第三方JAR中的类，该如何管理？
## 4.1 案例：数据源对象管理
在这一节中，通过一个案例来学习，对于第三方 `bean` 该如何进行配置管理。以后我们会用到很多第三方 `bean` ，本次案例将使用前面提到过的数据源 `Druid(德鲁伊)` 和 `C3P0` 来配置学习。
### 4.1.1 环境准备
学习之前，先来准备下案例环境：
* 创建一个简单的Maven项目
* `pom.xml` 添加依赖：
  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-context</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
  </dependencies>
  ```
* `resources` 下添加Spring的配置文件 `applicationContext.xml`
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd">
  
  </beans>
  ```
* 编写一个运行类App
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
      }
  }
  ```
### 4.1.2 思路分析
在上述环境下，我们来对数据源进行配置管理，先分析思路：
> 需求：使用Spring的IoC容器来管理Druid连接池对象
> 1. 使用第三方的技术，需要在 `pom.xml` 添加依赖
> 2. **在配置文件中将【第三方的类】制作成一个 `bean` ，让IoC容器进行管理**
> 3. 数据库连接需要基础的四要素  `驱动`、`连接`、`用户名`和`密码`，**【如何注入】到对应的 `bean` 中**
> 4. 从IoC容器中获取对应的 `bean` 对象，将其打印到控制台查看结果

**思考**：带着这两个问题，把下面的案例实现下。
* 第三方的类指的是什么？
* 如何注入数据库连接四要素？

#### 1. 实现Druid管理
步骤1：导入 `druid` 的依赖。`pom.xml` 中添加依赖：
```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.1.16</version>
</dependency>
```
步骤2：配置第三方 `bean` 。在 `applicationContext.xml` 配置文件中添加 `DruidDataSource` 的配置（发现 `DruidDataSource` 类中没有合适的构造器，只能用 `setter` 注入）：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd">
	<!--管理DruidDataSource对象,setter注入-->
    <bean class="com.alibaba.druid.pool.DruidDataSource">
        <property name="driverClassName" value="com.mysql.jdbc.Driver"/>
        <property name="url" value="jdbc:mysql://localhost:3306/spring_db"/>
        <property name="username" value="root"/>
        <property name="password" value="root"/>
    </bean>
</beans>
```
**说明**：
* `driverClassName` ：数据库驱动
* `url` ：数据库连接地址
* `username` ：数据库连接用户名
* `password` ：数据库连接密码
* 数据库连接的四要素要和自己使用的数据库信息一致。

步骤3：从IoC容器中获取对应的 `bean` 对象。
```java
public class App {
    public static void main(String[] args) {
       ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
       DataSource dataSource = (DataSource) ctx.getBean("dataSource");
       System.out.println(dataSource);
    }
}
```
步骤4：运行程序。打印如下结果：说明第三方 `bean` 对象已经被Spring的IoC容器进行管理。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180324390.png)

做完案例后，我们可以将刚才思考的两个问题答案说下：
- 第三方的类指的是什么？—— `DruidDataSource` 。
- 如何注入数据库连接四要素？—— `setter` 注入。

### 4.1.4 实现C3P0管理
完成了 `DruidDataSource` 的管理，接下来我们加深练习，这次管理 `C3P0` 数据源，具体的实现步骤是什么？
>需求：使用Spring的IoC容器来管理C3P0连接池对象
>实现方案和上面基本一致，**重点要关注管理的是哪个 `bean` 对象？**

步骤1：导入`C3P0`的依赖。`pom.xml` 中添加依赖。
```xml
<dependency>
    <groupId>c3p0</groupId>
    <artifactId>c3p0</artifactId>
    <version>0.9.1.2</version>
</dependency>
```

**对于新的技术，不知道具体的坐标该如何查找**？
* 直接百度搜索
* 从MVN的仓库 `https://mvnrepository.com/` 中进行搜索
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180330932.png)

步骤2：配置第三方 `bean` 。在 `applicationContext.xml` 配置文件中添加配置。
```xml
<bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
    <property name="driverClass" value="com.mysql.jdbc.Driver"/>
    <property name="jdbcUrl" value="jdbc:mysql://localhost:3306/spring_db"/>
    <property name="user" value="root"/>
    <property name="password" value="root"/>
    <property name="maxPoolSize" value="1000"/>
</bean>
```
**==注意==**：
* `ComboPooledDataSource` 的属性是通过 `setter` 方式进行注入
* 想注入属性，就需要在 `ComboPooledDataSource` 类或其上层类中、有提供属性对应的 `setter` 方法
* C3P0的四个属性和Druid的四个属性是不一样的

步骤3：运行程序。程序会报错，错误如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180332323.png)
报的错为 `ClassNotFoundException` ，翻译出来是`类没有发现的异常`，具体的类为 `com.mysql.jdbc.Driver` 。错误的原因是缺少MySQL的驱动包。分析出错误的原因，具体的解决方案就比较简单，只需要在 `pom.xml` 把驱动包引入即可。
```xml
<dependency>
    <groupId>mysql</groupId>
    <artifactId>mysql-connector-java</artifactId>
    <version>5.1.47</version>
</dependency>
```
添加完MySQL的驱动包以后，再次运行App，就可以打印出结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302180333830.png)

**注意**：
* ==数据连接池在配置属性时，除了可以注入数据库连接四要素外，还可以配置很多其他的属性==，具体都有哪些属性用到时再去查，一般配置基础的四个，其他都有自己的默认值
* `Druid` 和 `C3P0在` 没有导入MySQL驱动包的前提下，一个没报错一个报错，说明 `Druid` 在初始化时没有去加载驱动，而 `C3P0` 刚好相反
* `Druid` 程序运行虽然没有报错，但当调用 `DruidDataSource` 的 `getConnection()` 方法获取连接时，也会报找不到驱动类的错误

## 4.2 加载 `properties` 文件
上节中已完成两个数据源 `Druid` 和 `C3P0` 的配置，但其中包含了一些问题，我们来分析下：
* 这两个数据源中都使用到了一些固定的常量，如数据库连接四要素，**把这些值写在Spring的配置文件中不利于后期维护**
* 需要将这些值提取到一个外部的 `properties` 配置文件中
* **Spring框架如何从配置文件中读取属性值来配置**，就是接下来要解决的问题。

问题提出来后，具体该如何实现？
### 4.2.1 第三方 `bean` 属性优化
#### 1. 实现思路
> 需求：将数据库连接四要素提取到 `properties` 配置文件，Spring来加载配置信息，并使用这些信息来完成属性注入。
> 1. 在 `resources` 下创建一个 `jdbc.properties`（文件的名称可以任意）
> 2. 将数据库连接四要素配置到配置文件中
> 3. **在Spring的配置文件中加载 `properties` 文件**
> 4. 使用加载到的值实现属性注入
> 其中第3, 4步骤是需要大家重点关注，具体是如何实现。

步骤1：准备 `properties` 配置文件。`resources` 下创建一个 `jdbc.properties` 文件，并添加对应的属性键值对：
```properties
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://127.0.0.1:3306/spring_db
jdbc.username=root
jdbc.password=root
```
步骤2：开启 `context` 命名空间。在 `applicationContext.xml` 中开 `context` 命名空间。复制 `<beans` 后面那行，改 `xmlns` 为 `xmlns:context` ，改 `/beans` 为 `/context` ；同时复制 `xsi:schemaLocation` 的前两条URL，将其中的 `beans` 都改为 `context` ，粘贴在下面。 
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-context.xsd">
</beans>
```
步骤3：加载 `properties` 配置文件。在配置文件中使用「`context` 命名空间下的标签」来加载 `properties` 配置文件。
```xml
<context:property-placeholder location="jdbc.properties"/>
```
步骤4：完成属性注入。使用 `${key}` 来读取 `properties` 配置文件中的内容，并完成属性注入。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-context.xsd">
    
    <context:property-placeholder location="jdbc.properties"/>
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource">
        <property name="driverClassName" value="${jdbc.driver}"/>
        <property name="url" value="${jdbc.url}"/>
        <property name="username" value="${jdbc.username}"/>
        <property name="password" value="${jdbc.password}"/>
    </bean>
</beans>
```
至此，读取外部 `properties` 配置文件中的内容就已经完成。
### 4.2.2 读取单个属性
#### 1. 实现思路
对于上面的案例，效果不是很明显，我们可以换个案例来演示:
> 需求：从 `properties` 配置文件中读取 `key` 为 `name` 的值，并将其注入到 `BookDao` 中并在 `save` 方法中进行打印。
> 1. 在项目中添加 `BookDao, BookDaoImpl` 类
> 2. 为 `BookDaoImpl` 添加一个 `name` 属性并提供 `setter` 方法
> 3. 在 `jdbc.properties` 中添加数据、注入到 `bookDao` 中，并打印查询结果
> 4. 在 `applicationContext.xml` 添加配置完成配置文件、属性注入( `${key}` )

步骤1：在项目中添对应的类。`BookDao, BookDaoImpl` 类，并在 `BookDaoImpl` 类中添加 `name` 属性与 `setter` 方法。
```java
public interface BookDao {
    public void save();
}
public class BookDaoImpl implements BookDao {
    private String name;
    public void setName(String name) {
        this.name = name;
    }
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```
步骤2：完成配置文件的读取与注入。在 `applicationContext.xml` 添加配置，`bean` 的配置管理`、`读取外部 `properties` 、**依赖注入**:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans
            http://www.springframework.org/schema/beans/spring-beans.xsd
            http://www.springframework.org/schema/context
            http://www.springframework.org/schema/context/spring-context.xsd">
    
    <context:property-placeholder location="jdbc.properties"/>
    
    <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
        <property name="name" value="${jdbc.driver}"/>
    </bean>
</beans>
```
步骤3：运行程序。在 `App` 类中，从IoC容器中获取 `bookDao` 对象，调用方法，查看值是否已经被获取到并打印控制台。
```java
public class App {
    public static void main(String[] args) throws Exception{
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        bookDao.save();
    }
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302181344928.png)
#### 2. 注意事项
至此，读取 `properties` 配置文件中的内容就已经完成，但在使用时，有些注意事项。
* 问题一：键值对的 `key` 为 `username` 引发的问题
  1.在 `properties` 中配置键值对时，如果 `key` 设置为 `username`
  ```
  username=root666
  ```
  2.在 `applicationContext.xml` 注入该属性
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:context="http://www.springframework.org/schema/context"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd
              http://www.springframework.org/schema/context
              http://www.springframework.org/schema/context/spring-context.xsd">
      
      <context:property-placeholder location="jdbc.properties"/>
      
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl">
          <property name="name" value="${username}"/>
      </bean>
  </beans>
  ```
  3.运行后，在控制台打印的却不是 `root666`，而是自己电脑的用户名
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302181834272.png)
  4.出现问题的原因是 **`<context:property-placeholder/>` 标签会加载系统的环境变量，而且环境变量的值会被优先加载**，如何查看系统的环境变量？
  ```java
  public static void main(String[] args) throws Exception{
      Map<String, String> env = System.getenv();
      System.out.println(env);
  }
  ```
  大家可以自行运行，在打印出来的结果中会有一个 `USERNAME=XXX[自己电脑的用户名称]` 。
  5.解决方案：
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:context="http://www.springframework.org/schema/context"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd
              http://www.springframework.org/schema/context
              http://www.springframework.org/schema/context/spring-context.xsd">
      
      <context:property-placeholder location="jdbc.properties" system-properties-mode="NEVER"/>
  </beans>
  ```
  `system-properties-mode` ：设置为 `NEVER` ，表示不加载系统属性，就可以解决上述问题。当然，还有一个解决方案就是**避免使用`username`作为属性的 `key`**。

* 问题二：当有多个 `properties` 配置文件需要被加载，该如何配置?
  1.调整下配置文件的内容，在 `resources` 下添加 `jdbc.properties, jdbc2.properties` ，内容如下：
  ```java
  jdbc.driver=com.mysql.jdbc.Driver
  jdbc.url=jdbc:mysql://127.0.0.1:3306/spring_db
  jdbc.username=root
  jdbc.password=root
  ```
  ```java
  username=root666
  ```
  2.修改 `applicationContext.xml` ：
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xmlns:context="http://www.springframework.org/schema/context"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans
              http://www.springframework.org/schema/beans/spring-beans.xsd
              http://www.springframework.org/schema/context
              http://www.springframework.org/schema/context/spring-context.xsd">
      <!--方式一 -->
      <context:property-placeholder location="jdbc.properties,jdbc2.properties" system-properties-mode="NEVER"/>
      <!--方式二-->
      <context:property-placeholder location="*.properties" system-properties-mode="NEVER"/>
      <!--方式三 -->
      <context:property-placeholder location="classpath:*.properties" system-properties-mode="NEVER"/>
      <!--方式四-->
      <context:property-placeholder location="classpath*:*.properties" system-properties-mode="NEVER"/>
  </beans>	
  ```
  **说明:**
  * 方式一：可以实现，如果配置文件多的话，每个都需要配置
  * 方式二：`*.properties` 代表所有以 `properties` 结尾的文件都会被加载，可以解决方式一的问题，但是不标准
  * 方式三：**标准的写法**，`classpath:` 代表的是从根路径下开始查找，但是**只能查询当前项目的根路径**（必须写成这种格式）
  * 方式四：不仅可以加载当前项目，还可以加载「当前项目所依赖的所有项目」的根路径下的 `properties` 配置文件

### 4.2.3 加载 `properties` 文件小结
本节主要讲解的是 `properties` 配置文件的加载，需要掌握的内容有：
* 如何开启 `context` 命名空间 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302181849973.png)
* 如何加载 `properties` 配置文件
    ```xml
    <context:property-placeholder location="" system-properties-mode="NEVER"/>
    ```
* 如何在 `applicationContext.xml` 引入 `properties` 配置文件中的值
    ```java
    ${key}
    ```

---
# 5. 核心容器
前面已经完成 `bean` 与依赖注入的相关知识学习，接下来我们主要学习的是IoC容器中的==核心容器==。

**这里所说的核心容器，大家可以把它简单的理解为 `ApplicationContext`** ，前面虽已用过，但并没有系统学习，接下来咱们从以下几个问题入手来学习下容器的相关知识：
* 如何创建容器？
* 创建好容器后，如何从容器中获取 `bean` 对象？
* 容器类的层次结构是什么？
* `BeanFactory` 是什么？

## 5.1 环境准备
在学习和解决上述问题之前，先来准备下案例环境：
* 创建一个Maven项目
* `pom.xml` 添加Spring的依赖
* `resources` 下添加 `applicationContext.xml` 
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
  </beans>
  ```
* 添加 `BookDao` 和 `BookDaoImpl` 类
  ```java
  public interface BookDao {
      public void save();
  }
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  ```
* 创建运行类App
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
          BookDao bookDao = (BookDao) ctx.getBean("bookDao");
          bookDao.save();
      }
  }
  ```
最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302181854228.png)
## 5.2 容器
### 5.2.1 容器的创建方式
案例中创建 `ApplicationContext` 的方式为：
```java
ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
```
这种方式翻译为==类路径下的XML配置文件==。除了上面这种方式，Spring还提供了另外一种创建方式为：
```java
ApplicationContext ctx = new FileSystemXmlApplicationContext("applicationContext.xml");
```
这种方式翻译为==文件系统下的XML配置文件==。使用这种方式运行，会出现如下错误： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182321226.png)
从错误信息中能发现，这种方式是从项目路径下开始查找 `applicationContext.xml` 配置文件的，所以需要将其修改为：
```java
ApplicationContext ctx = new FileSystemXmlApplicationContext("D:\\workspace\\spring\\spring_10_container\\src\\main\\resources\\applicationContext.xml"); 
```
**说明**：大家练习时，写自己的具体路径。这种方式虽能实现，==但当项目的位置发生变化后，代码也需要跟着改，耦合度较高，不推荐使用==。
### 5.2.2 `Bean` 的三种获取方式
方式一，就是目前案例中获取的方式：
```java
BookDao bookDao = (BookDao) ctx.getBean("bookDao");
```
这种方式存在的问题是**每次获取时都需要进行类型转换**，有没有更简单的方式呢？

方式二：
```java
BookDao bookDao = ctx.getBean("bookDao"，BookDao.class);
```
这种方式可以解决类型强转问题，但参数又多加了一个，相对来说没有简化多少。

方式三：
```java
BookDao bookDao = ctx.getBean(BookDao.class);
```
这种方式就==类似我们之前所学习依赖注入中的按类型注入==。必须要**确保IoC容器中该类型对应的 `bean` 对象只能有一个**。
### 5.2.3 容器类层次结构
在IDEA中双击 `shift` ，输入 `BeanFactory` ： 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182323662.png)
点击进入 `BeanFactory` 类，`ctrl+h` 就能查看到如下结构的层次关系：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182324030.png)
 从图中可以看出，==容器类也是从无到有根据需要一层层叠加上来的，大家重点理解下这种设计思想==。

### 5.2.4 `BeanFactory` 的使用
使用 `BeanFactory` 来创建IoC容器的具体实现方式为：
```java
public class AppForBeanFactory {
    public static void main(String[] args) {
        Resource resources = new ClassPathResource("applicationContext.xml");
        BeanFactory bf = new XmlBeanFactory(resources);
        BookDao bookDao = bf.getBean(BookDao.class);
        bookDao.save();
    }
}
```
为了更好的看出 `BeanFactory` 和 `ApplicationContext` 之间的区别，在 `BookDaoImpl` 添加如下构造函数：
```java
public class BookDaoImpl implements BookDao {
    public BookDaoImpl() {
        System.out.println("constructor");
    }
    public void save() {
        System.out.println("book dao save ..." );
    }
}
```
如果去获取 `bean` 对象，打印会发现：
* **`BeanFactory` 是延迟加载，只有在获取 `bean` 对象时才会去创建**
* `ApplicationContext` 是立即加载，容器加载时就会创建 `bean` 对象
* `ApplicationContext` 要想延迟加载，只需要按照如下方式进行配置
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"  lazy-init="true"/>
  </beans>
  ```
**小结**
这一节中所讲的知识点包括：
* 容器创建的两种方式
	* `ClassPathXmlApplicationContext` [掌握]
	* `FileSystemXmlApplicationContext` [知道即可]
* 获取 `Bean` 的三种方式
	* `getBean("名称")` ：需要类型转换
	* `getBean("名称",类型.class)` ：多了一个参数
	* `getBean(类型.class)` ：容器中不能有多个该类的 `bean` 对象
	上述三种方式，各有各的优缺点，用哪个都可以。
* 容器类层次结构：只需要知晓容器的最上级的父接口为 `BeanFactory` 即可
* `BeanFactory`
	* 使用 `BeanFactory` 创建的容器是延迟加载
	* 使用 `ApplicationContext` 创建的容器是立即加载
	* 具体 `BeanFactory` 如何创建只需了解即可。

## 5.5 核心容器总结
这节中没有新的知识点，只是对前面知识的一个大总结，共包含如下内容。

**容器相关**：
- `BeanFactory` 是IoC容器的顶层接口，初始化 `BeanFactory` 对象时，加载的 `bean` 延迟加载
- `ApplicationContext` 接口是Spring容器的核心接口，初始化时 `bean` 立即加载
- `ApplicationContext` 接口提供基础的 `bean` 操作相关方法，**通过其他接口扩展其功能**
- `ApplicationContext` 接口常用初始化类
	- `ClassPathXmlApplicationContext` (常用)
	- `FileSystemXmlApplicationContext`

**`bean` 相关**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182329683.png)
其实整个配置中最常用的就两个属性 `id` 和 `class` 。把 `scope, init-method, destroy-method` 框起来的原因是，后面讲解「注解」时还会用到，所以对这三个属性关注。

依赖注入相关：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182330098.png)

# 6. IoC/DI注解开发
Spring的IoC/DI对应的配置开发已经讲解完成，但使用起来相对来说还是比较复杂的，复杂的地方在==配置文件==。前面聊Spring时说过，Spring可以简化代码的开发，到现在并没有体会到。所以==Spring到底是如何简化代码开发的==？

<b><font color="red">要想真正简化开发，就需要用到Spring的注解开发</font></b>，Spring对注解支持的版本历程：
* 2.0版开始支持注解
* 2.5版注解功能趋于完善
* **3.0版支持纯注解开发**

关于注解开发，讲解两块内容 `注解开发定义bean` 和 `纯注解开发` 。注解开发定义 `bean` 用的是2.5版提供的注解，**纯注解开发用的是3.0版提供的注解**。

## 6.1 环境准备
在学习注解开发之前，先准备案例环境：
- 创建一个Maven项目
- `pom.xml` 添加Spring的依赖 
- `resources` 下添加 `applicationContext.xml`
  ```xml
  <?xml version="1.0" encoding="UTF-8"?>
  <beans xmlns="http://www.springframework.org/schema/beans"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="
              http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
      <bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
  </beans>
  ```
- 添加 `BookDao, BookDaoImpl, BookService, BookServiceImpl` 类
- 创建运行类App
  ```java
  public class App {
      public static void main(String[] args) {
          ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
          BookDao bookDao = (BookDao) ctx.getBean("bookDao");
          bookDao.save();
      }
  }
  ```

最终创建好的项目结构如下：
![img|330x200](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182333738.png)

## 6.2 注解开发定义 `bean`
在上述环境的基础上，我们学Spring是如何通过注解实现 `bean` 的定义开发。

步骤1：删除原XML配置。将配置文件中的 `<bean>` 标签删除掉：
```xml
<bean id="bookDao" class="com.itheima.dao.impl.BookDaoImpl"/>
```
步骤2：`Dao` 上添加注解。在 `BookDaoImpl` 类上添加 `@Component` 注解。
```java
@Component("bookDao")
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ..." );
    }
}
```
==注意==：`@Component` 注解不可以添加在接口上，因为接口是无法创建对象的。`@Component` 等
| 名称 | `@Component/@Controller/@Service/@Repository` |
| ---- | ------------------------------------------- |
| 类型 | 类注解                                      |
| 位置 | 类定义上方                                  |
| 作用 | 设置该类为Spring管理的 `bean`                  |
| 属性 | `value`（默认）：定义 `bean` 的 `id`                |

对于 `@Component` 注解，还衍生出了其他三个注解 `@Controller, @Service, @Repository` 。通过查看源码会发现：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182339176.png)
这三个注解和 `@Component` 注解的作用是一样的，为什么要衍生出这三个呢？——==方便我们后期在编写类时，能很好区分出这个类是属于表现层、业务层还是数据层的类==。

XML与注解配置的对应关系：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182335035.png)

步骤3：**配置Spring的注解包扫描**。为了让Spring框架能够扫描到写在类上的注解，需要在配置文件上进行包扫描：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="
            http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd">
    <context:component-scan base-package="com.itheima"/>
</beans>
```
**说明**：`component-scan`（`component` 组件，**Spring将管理的 `bean` 视作自己的一个组件**；`scan` ：扫描）
- **`base-package` 指定Spring框架扫描的包路径**，它会扫描指定包及其子包中的所有类上的注解。
* 包路径越多[如 `com.itheima.dao.impl` ]，扫描的范围越小速度越快
* 包路径越少[如 `com.itheima` ]，扫描的范围越大速度越慢
* 一般扫描到项目的组织名称即Maven的 `groupId` 下[如 `com.itheima` ]即可。

步骤4：运行程序。运行 `App` 类查看打印结果
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182337109.png)
步骤5：`Service` 上添加注解。**在 `BookServiceImpl` 类（而非接口）上也添加 `@Component` 交给Spring框架管理**：
```java
@Component
public class BookServiceImpl implements BookService {
    private BookDao bookDao;

    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }

    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
步骤6：运行程序。在 `App` 类中，从IoC容器中获取 `BookServiceImpl` 对应的 `bean` 对象，打印。
```java
public class App {
    public static void main(String[] args) {
        ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        System.out.println(bookDao);
        //按类型获取bean
        BookService bookService = ctx.getBean(BookService.class);
        System.out.println(bookService);
    }
}
```
打印观察结果，两个 `bean` 对象都已经打印到控制台：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182338723.png)
**说明**：
* `BookServiceImpl` 类没有起名称，所以在 `App` 中是**按照类型来获取 `bean` 对象**
* **`@Component` 注解如果不起名称，会有一个默认值就是 `当前类名首字母小写`** ，所以也可以按照名称获取，如
  ```java
  BookService bookService = (BookService)ctx.getBean("bookServiceImpl");
  System.out.println(bookService);
  ```

## 6.3 纯注解开发模式
上面已经可以使用注解来配置 `bean` ，但是依然有用到配置文件，在配置文件中对包进行了扫描，Spring在3.0版已经支持纯注解开发，即**使用Java类替代配置文件，开启了Spring快速开发赛道**。具体如何实现？
### 6.3.1 思路分析
实现思路为：将配置文件 `applicationContext.xml` 删除掉，使用类来替换。
### 6.3.2 实现步骤
步骤1：创建配置类。创建一个配置类 `SpringConfig` 
```java
public class SpringConfig {
}
```
步骤2：**标识该类为配置类**。在配置类上添加 `@Configuration` 注解，将其标识为一个配置类，替换 `applicationContext.xml` 。
```java
@Configuration
public class SpringConfig {
}
```
步骤3：**用注解替换包扫描配置**。在配置类上添加包扫描注解 `@ComponentScan` 替换 `<context:component-scan base-package=""/>` 。
```java
@Configuration
@ComponentScan("com.itheima")
public class SpringConfig {
}
```
步骤4：创建运行类并执行。创建一个新的运行类 `AppForAnnotation` 。
```java
public class AppForAnnotation {
    public static void main(String[] args) {
        ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao = (BookDao) ctx.getBean("bookDao");
        System.out.println(bookDao);
        BookService bookService = ctx.getBean(BookService.class);
        System.out.println(bookService);
    }
}
```
运行 `AppForAnnotation` ，可以看到两个对象依然被获取成功
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182345293.png)

至此，纯注解开发的方式就已经完成了，主要内容包括：
* Java类替换Spring核心配置文件
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302182345837.png)
* `@Configuration` 注解用于设定当前类为配置类
* `@ComponentScan` 注解用于设定扫描路径，此注解只能添加一次，多个数据请用数组格式
  ```java
  @ComponentScan({com.itheima.service", "com.itheima.dao"})
  ```
* 读取Spring核心配置文件、初始化容器对象，切换为「读取Java配置类、初始化容器对象」：
  ```java
  //加载配置文件初始化容器
  ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext.xml");
  //加载配置类初始化容器
  ApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
  ```

### 知识点1：`@Configuration`
| 名称 | `@Configuration`             |
| ---- | --------------------------- |
| 类型 | 类注解                      |
| 位置 | 类定义上方                  |
| 作用 | 设置该类为Spring配置类      |
| 属性 | value（默认）：定义 `bean` 的 `id` |

### 知识点2：`@ComponentScan`

| 名称 | `@ComponentScan`                                           |
| ---- | -------------------------------------------------------- |
| 类型 | 类注解                                                   |
| 位置 | 类定义上方                                               |
| 作用 | 设置Spring配置类扫描路径，用于加载使用注解格式定义的 `bean` |
| 属性 | value（默认）：扫描路径，此路径可以逐层向下扫描          |

**小结**：这一节重点掌握的是**使用注解完成Spring的 `bean` 管理**，需要掌握的内容为
* 记住 `@Component, @Controller, @Service, @Repository` 这四个注解
* `applicationContext.xml` 中 `<context:component-san/>` 的作用，是指定扫描包路径，注解为 `@ComponentScan`
* `@Configuration` 标识该类为配置类，使用类替换 `applicationContext.xml` 文件
* `ClassPathXmlApplicationContext` 是加载XML配置文件
* `AnnotationConfigApplicationContext` 是加载配置类

## 6.3 注解开发 `bean` 作用范围与生命周期管理
使用注解已经完成了 `bean` 的管理，接下来按照前面所学习的内容，==将通过配置实现的内容都换成对应的注解实现==，包含两部分内容 `bean作用范围` 和 `bean生命周期` 。
### 6.3.1 环境准备
老规矩，学习之前先来准备环境：
- 创建一个Maven项目
- `pom.xml` 添加Spring的依赖 
- 添加一个配置类 `SpringConfig`
  ```java
  @Configuration
  @ComponentScan("com.itheima")
  public class SpringConfig {
  }
  ```
- 添加 `BookDao, BookDaoImpl` 类
- 创建运行类 `App`
  ```java
  public class App {
      public static void main(String[] args) {
          AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookDao bookDao1 = ctx.getBean(BookDao.class);
          BookDao bookDao2 = ctx.getBean(BookDao.class);
          System.out.println(bookDao1);
          System.out.println(bookDao2);
      }
  }
  ```
最终创建好的项目结构如下:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191001311.png)
### 6.3.2 `Bean` 的作用范围
(1) 先运行 `App` 类，在控制台打印两个一模一样的地址，说明默认情况下 `bean` 是单例
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191004137.png)
(2) 要想将 `BookDaoImpl` 变成非单例，只需要在其类上添加 `@scope` 注解
```java
@Repository
//@Scope设置bean的作用范围
@Scope("prototype")
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
}
```
再次执行 `App` 类，打印结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191005847.png)

### 知识点1：`@Scope`
| 名称 | @Scope                                                       |
| ---- | ------------------------------------------------------------ |
| 类型 | 类注解                                                       |
| 位置 | 类定义上方                                                   |
| 作用 | 设置该类创建对象的作用范围<br/>可用于设置创建出的 `bean` 是否为单例对象 |
| 属性 | value（默认）：定义 `bean` 作用范围，<br/>==默认值singleton（单例），可选值prototype（非单例）== |

### 6.3.3 `Bean` 的生命周期
(1) 在 `BookDaoImpl` 中添加两个方法，`init` 和 `destroy` ，方法名可以任意：
```java
@Repository
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
    public void init() {
        System.out.println("init ...");
    }
    public void destroy() {
        System.out.println("destroy ...");
    }
}
```
(2) 如何对方法进行标识，哪个是初始化方法，哪个是销毁方法？**只需要在对应的方法上添加 `@PostConstruct` 和 `@PreDestroy` 注解即可**。
```java
@Repository
public class BookDaoImpl implements BookDao {
    public void save() {
        System.out.println("book dao save ...");
    }
    @PostConstruct //在构造方法之后执行，替换 init-method
    public void init() {
        System.out.println("init ...");
    }
    @PreDestroy //在销毁方法之前执行,替换 destroy-method
    public void destroy() {
        System.out.println("destroy ...");
    }
}
```
(3) 要想看到两个方法执行，需要注意的是 `destroy` 只有在容器关闭时，才会执行，所以需要修改 `App` 的类：
```java
public class App {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        BookDao bookDao1 = ctx.getBean(BookDao.class);
        BookDao bookDao2 = ctx.getBean(BookDao.class);
        System.out.println(bookDao1);
        System.out.println(bookDao2);
        ctx.close(); //关闭容器
    }
}
```
(4) 运行 `App` 类查看打印结果，证明 `init` 和 `destroy` 方法都被执行了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191014035.png)
==**注意**==：`@PostConstruct` 和 `@PreDestroy` 注解如果找不到，需要导入下面的 `jar` 包。
```java
<dependency>
  <groupId>javax.annotation</groupId>
  <artifactId>javax.annotation-api</artifactId>
  <version>1.3.2</version>
</dependency>
```
找不到的原因是，从JDK9以后 `jdk` 中的 `javax.annotation` 包被移除了，这两个注解刚好就在这个包中。

### 知识点1：`@PostConstruct`
| 名称 | `@PostConstruct`        |
| ---- | ---------------------- |
| 类型 | 方法注解               |
| 位置 | 方法上                 |
| 作用 | 设置该方法为初始化方法 |
| 属性 | 无                     |

### 知识点2：`@PreDestroy`
| 名称 | `@PreDestroy`          |
| ---- | -------------------- |
| 类型 | 方法注解             |
| 位置 | 方法上               |
| 作用 | 设置该方法为销毁方法 |
| 属性 | 无                   |

**小结**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302192247006.png)


## 6.4 注解开发依赖注入
Spring为了使用注解简化开发，并没有提供 `构造函数注入` 、`setter注入` 对应的注解，**只提供了自动装配的注解实现**。
### 6.4.1 环境准备
在学习之前，把案例环境介绍下：
- 创建一个Maven项目
- `pom.xml` 添加Spring的依赖
- 添加一个配置类`SpringConfig`
  ```java
  @Configuration
  @ComponentScan("com.itheima")
  public class SpringConfig {
  }
  ```
- 添加 `BookDao, BookDaoImpl, BookService, BookServiceImpl` 类
  ```java
  public interface BookDao {
      public void save();
  }
  @Repository
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  public interface BookService {
      public void save();
  }
  @Service
  public class BookServiceImpl implements BookService {
      private BookDao bookDao;
  	public void setBookDao(BookDao bookDao) {
          this.bookDao = bookDao;
      }
      public void save() {
          System.out.println("book service save ...");
          bookDao.save();
      }
  }
  ```
- 创建运行类 `App`
  ```java
  public class App {
      public static void main(String[] args) {
          AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
          BookService bookService = ctx.getBean(BookService.class);
          bookService.save();
      }
  }
  ```

最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191021355.png)
环境准备好后，运行后会发现有问题
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302192302186.png)
出现问题的原因是，在 `BookServiceImpl` 类中添加了 `BookDao` 的属性，并提供了 `setter` 方法，但目前是没有提供配置注入 `BookDao` 的，所以 `bookDao` 对象为 `Null` ，调用其 `save` 方法就会报 `空指针异常` 。
### 6.4.2 注解实现按照类型注入
对于这个问题使用注解该如何解决？
(1) 在 `BookServiceImpl` 类的 `bookDao` 属性上添加 `@Autowired` 注解：
```java
@Service
public class BookServiceImpl implements BookService {
    @Autowired
    private BookDao bookDao;
    
//	  public void setBookDao(BookDao bookDao) {
//        this.bookDao = bookDao;
//    }
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
**注意**：
* `@Autowired` 可以写在属性上，也可也写在 `setter` 方法上，**最简单的处理方式是 `写在属性上并将setter方法删除掉`**
* 为什么 `setter` 方法可以删除呢？
  * 自动装配基于反射设计、创建对象并通过暴力反射为私有属性进行设值
  * 普通反射只能获取 `public` 修饰的内容
  * 暴力反射除了获取 `public` 修饰的内容，还可以获取 `private` 修改的内容
  * 所以**此处无需提供 `setter` 方法**

(2) **`@Autowired` 是按照类型注入**，那么对应 `BookDao` 接口如果有多个实现类，比如添加 `BookDaoImpl2` ：
```java
@Repository
public class BookDaoImpl2 implements BookDao {
    public void save() {
        System.out.println("book dao save ...2");
    }
}
```
这个时候再次运行App，就会报错：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191026414.png)
此时，按照类型注入就无法区分到底注入哪个对象，解决方案是 `按照名称注入` 。先给两个 `Dao` 类分别起个名称：
```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
  public void save() {
	  System.out.println("book dao save ..." );
  }
}
@Repository("bookDao2")
public class BookDaoImpl2 implements BookDao {
  public void save() {
	  System.out.println("book dao save ...2" );
  }
}
```
此时就可以注入成功，但是得思考个问题：
* `@Autowired` 是按照类型注入的，给 `BookDao` 的两个实现起了名称，它还是有两个 `bean` 对象，**为什么不报错**？
* `@Autowired` 默认按照类型自动装配，**如果IoC容器中同类的 `Bean` 找到多个，就按照变量名和 `Bean` 的名称匹配**。因为变量名叫 `bookDao` 而容器中也有一个 `booDao` ，所以可以成功注入。
* 分析下面这种情况是否能完成注入呢？
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191028818.png)
* 不行，因为按照类型会找到多个 `bean` 对象，此时会按照 `bookDao` 名称去找，因为IoC容器只有名称叫 `bookDao1` 和 `bookDao2` ，所以找不到，会报 `NoUniqueBeanDefinitionException`

### 6.4.3 注解实现按照名称注入
**当根据类型在容器中找到多个bean，注入参数的属性名又和容器中 `bean` 的名称不一致**，这个时候该如何解决，就需要使用到 `@Qualifier` 来指定注入哪个名称的 `bean` 对象。
```java
@Service
public class BookServiceImpl implements BookService {
    @Autowired
    @Qualifier("bookDao1")
    private BookDao bookDao;
    
    public void save() {
        System.out.println("book service save ...");
        bookDao.save();
    }
}
```
`@Qualifier` 注解后的值就是需要注入的 `bean` 的名称。
> ==注意==：**`@Qualifier` 不能独立使用，必须和 `@Autowired` 一起使用**。

### 6.4.4 简单数据类型注入
引用类型看完，简单类型注入就比较容易懂了。简单类型注入的是基本数据类型或者字符串类型，下面在 `BookDaoImpl` 类中添加一个 `name` 属性，用其进行简单类型注入：
```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
    private String name;
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```
**数据类型换了，对应的注解也要跟着换**，这次使用 `@Value` 注解，**将值写入注解的参数中就行了**：
```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
    @Value("itheima")
    private String name;
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```
注意数据格式要匹配，如将 `"abc"` 注入给 `int` 值，这样程序就会报错。

介绍完后，会有一种感觉就是：==这个注解好像没什么用，跟直接赋值是一个效果，还没有直接赋值简单，所以这个注解存在的意义是什么==？
### 6.4.5 注解读取 `properties` 配置文件
**`@Value` 一般会被用在从 `properties` 配置文件中读取内容进行使用**，具体如何实现？

步骤1：`resource` 下准备 `properties` 文件 `jdbc.properties` ：
```java
name=itheima888
```
步骤2：使用注解**加载 `properties` 配置文件**。**在配置类上添加`@PropertySource`注解**：
```java
@Configuration
@ComponentScan("com.itheima")
@PropertySource("jdbc.properties")
public class SpringConfig {
}

```
步骤3：使用 `@Value` 读取配置文件中的内容：
```java
@Repository("bookDao")
public class BookDaoImpl implements BookDao {
    @Value("${name}")
    private String name;
    public void save() {
        System.out.println("book dao save ..." + name);
    }
}
```
步骤4：运行程序。运行 `App` 类，查看运行结果，说明配置文件中的内容已经被加载到
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191047663.png)
**注意**：
* 如果读取的 `properties` 配置文件有多个，可以使用 `@PropertySource` 的属性来指定多个
  ```java
  @PropertySource({"jdbc.properties","xxx.properties"})
  ```
* `@PropertySource` 注解属性中不支持使用通配符 `*` ，运行会报错
  ```java
  @PropertySource({"*.properties"})
  ```
* `@PropertySource`注解属性中可以把 `classpath:` 加上，**代表从当前项目的根路径找文件**：
  ```java
  @PropertySource({"classpath:jdbc.properties"})
  ```

### 知识点1：`@Autowired`
| 名称 | `@Autowired `                                                  |
| ---- | ------------------------------------------------------------ |
| 类型 | 属性注解 或 方法注解（了解）或 方法形参注解（了解）     |
| 位置 | 属性定义上方 或 标准 `set` 方法上方 或 类 `set` 方法上方 或 **方法形参前面** |
| 作用 | 为引用类型属性设置值                                         |
| 属性 | `required：true/false` ，定义该属性是否允许为null               |

### 知识点2：`@Qualifier`
| 名称 | `@Qualifier`                                           |
| ---- | ---------------------------------------------------- |
| 类型 | 属性注解 或 方法注解（了解）                       |
| 位置 | 属性定义上方 或 标准 `set` 方法上方 或 类 `set` 方法上方 |
| 作用 | 为引用类型属性指定注入的 `beanId`                       |
| 属性 | value（默认）：设置注入的 `beanId`                      |

### 知识点3：`@Value`
| 名称 | `@Value`                                               |
| ---- | ---------------------------------------------------- |
| 类型 | 属性注解 或 方法注解（了解）                       |
| 位置 | 属性定义上方 或 标准 `set` 方法上方 或 类 `set` 方法上方 |
| 作用 | 为 基本数据类型 或 字符串类型 属性设置值         |
| 属性 | value（默认）：要注入的属性值                        |

### 知识点4：`@PropertySource`
| 名称 | `@PropertySource`                                              |
| ---- | ------------------------------------------------------------ |
| 类型 | 类注解                                                       |
| 位置 | 类定义上方                                                   |
| 作用 | 加载 `properties` 文件中的属性值                                 |
| 属性 | value（默认）：设置加载的 `properties` 文件对应的文件名，或文件名组成的数组 |

---
# 7. IoC/DI注解开发管理第三方 `bean`
前面定义 `bean` 时，都是在自己开发的类上面写个注解就完成了，==但如果是第三方的类，这些类都是在JAR包中，我们没有办法在类上面添加注解==，这个时候该怎么办?

遇到上述问题，**我们就需要有一种更加灵活的方式来定义 `bean`** ，这种方式不能在原始代码上面书写注解，一样能定义 `bean` ，这就用到了一个全新的注解 `@Bean` 。这个注解该如何使用呢？

咱们把之前使用配置方式管理的数据源，使用注解再来一遍，通过这个案例来学习下 `@Bean` 的使用。
## 7.1 环境准备
学习 `@Bean` 注解之前先来准备环境:
- 创建一个Maven项目
- `pom.xml` 添加Spring的依赖 
- 添加一个配置类 `SpringConfig`
  ```java
  @Configuration
  public class SpringConfig {
  }
  ```
- 添加 `BookDao, BookDaoImpl` 类
  ```java
  public interface BookDao {
      public void save();
  }
  @Repository
  public class BookDaoImpl implements BookDao {
      public void save() {
          System.out.println("book dao save ..." );
      }
  }
  ```
- 创建运行类 `App`
  ```java
  public class App {
      public static void main(String[] args) {
          AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
      }
  }
  ```

最终创建好的项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302192227986.png)
## 7.2 注解开发管理第三方 `bean`
在上述环境中完成对 `Druid` 数据源的管理，具体的实现步骤如下。

步骤1：导入对应的JAR包。
```xml
<dependency>
    <groupId>com.alibaba</groupId>
    <artifactId>druid</artifactId>
    <version>1.1.16</version>
</dependency>
```
步骤2：在配置类中添加一个方法。注意，**该方法的返回值就是要创建的 `Bean` 对象类型**：
```java
@Configuration
public class SpringConfig {
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
步骤3：在方法上添加 `@Bean` 注解。**`@Bean` 注解的作用是将方法的返回值制作为Spring管理的一个 `bean` 对象**。
```java
@Configuration
public class SpringConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
**注意**：不能使用 `DataSource ds = new DruidDataSource()` ，**因为 `DataSource` 接口中没有对应的 `setter` 方法来设置属性**。

步骤4：从IoC容器中获取对象并打印。
```java
public class App {
    public static void main(String[] args) {
        AnnotationConfigApplicationContext ctx = new AnnotationConfigApplicationContext(SpringConfig.class);
        DataSource dataSource = ctx.getBean(DataSource.class);
        System.out.println(dataSource);
    }
}
```
至此使用 `@Bean` 来管理第三方 `bean` 的案例就已经完成。

**如果有多个 `bean` 要被Spring管理，直接在配置类中多些几个方法，方法上添加 `@Bean` 注解即可**。

## 7.3 引入外部配置类
如果把所有的第三方 `bean` 都配置到Spring的配置类 `SpringConfig` 中，虽然可以，但不利于代码阅读和分类管理，所以我们就想：能不能按照类别将这些 `bean` 配置到不同的配置类中？

对于数据源的 `bean` ，我们新建一个 `JdbcConfig` 配置类，并把数据源配置到该类下。
```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
现在的问题是，**这个配置类如何能被Spring配置类加载到，并创建 `DataSource` 对象在IoC容器中**？针对这个问题，有两个解决方案。
### 7.3.1 使用包扫描引入
步骤1：在Spring的配置类上添加包扫描。
```java
@Configuration
@ComponentScan("com.itheima.config")
public class SpringConfig {
	
}
```
步骤2：在 `JdbcConfig` 上添加配置注解。`JdbcConfig` 类要放入到 `com.itheima.config` 包下，需要被Spring的配置类扫描到即可：
```java
@Configuration
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
步骤3：运行程序。依然能获取到 `bean` 对象并打印控制台。这种方式虽然能够扫描到，==但不能很快的知晓都引入了哪些配置类，所以这种方式不推荐使用==。

### 7.3.2 使用 `@Import` 引入
方案一实现起来有点小复杂，Spring早就想到了这一点，于是又给我们提供了第二种方案。这种方案可以不用加 `@Configuration` 注解，**但必须在Spring配置类上使用 `@Import` 注解手动引入需要加载的配置类**。

步骤1：去除 `JdbcConfig` 类上的注解。
```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
步骤2：在Spring配置类中引入。
```java
@Configuration
//@ComponentScan("com.itheima.config")
@Import({JdbcConfig.class})
public class SpringConfig {
}
```
**注意**：
* **扫描注解可以移除**
* `@Import` 参数需要的是一个数组，**可以引入多个配置类**。
* **`@Import` 注解在配置类中只能写一次**，下面的方式是==不允许的==
  ```java
  @Configuration
  //@ComponentScan("com.itheima.config")
  @Import(JdbcConfig.class)
  @Import(Xxx.class)
  public class SpringConfig {
  	
  }
  ```

步骤3：运行程序。依然能获取到 `bean` 对象并打印控制台。
### 知识点1：`@Bean`
| 名称 | @Bean                                  |
| ---- | -------------------------------------- |
| 类型 | 方法注解                               |
| 位置 | 方法定义上方                           |
| 作用 | 设置该方法的返回值作为Spring管理的 `bean` |
| 属性 | value（默认）：定义 `bean` 的 `id`            |

### 知识点2：`@Import`
| 名称 | `@Import`   |
| ---- | -------- |
| 类型 | 类注解   |
| 位置 | 类定义上方 |
| 作用 | 导入配置类 |
| 属性 | value（默认）：定义导入的配置类类名，<br/>当配置类有多个时使用数组格式一次性导入多个配置类 |

## 7.4 注解开发实现为第三方 `bean` 注入资源
在使用 `@Bean` 创建 `bean` 对象时，**如果方法在创建的过程中需要其他资源该怎么办**？这些资源会有两大类，分别是 `简单数据类型` 和 `引用数据类型` 。
### 7.4.1 简单数据类型
#### 1. 需求分析
对于下面代码关于数据库的四要素不应该写死在代码中，应是从 `properties` 配置文件中读取。如何来优化下面的代码？
```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
#### 2. 注入简单数据类型步骤
步骤1：类中提供四个属性。
```java
public class JdbcConfig {
    private String driver;
    private String url;
    private String userName;
    private String password;
	@Bean
    public DataSource dataSource(){
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
步骤2：使用 `@Value` 注解引入值。
```java
public class JdbcConfig {
    @Value("com.mysql.jdbc.Driver")
    private String driver;
    @Value("jdbc:mysql://localhost:3306/spring_db")
    private String url;
    @Value("root")
    private String userName;
    @Value("password")
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
#### 扩展
现在的数据库连接四要素还是写在代码中，我们需要做的是，将这些内容提取到 `jdbc.properties` 配置文件，然后使用这个配置文件，大家思考下该如何实现？
> 1. `resources` 目录下添加 `jdbc.properties` 
> 2. 配置文件中提供四个键值对分别是数据库的四要素
> 3. 使用 `@PropertySource` 加载 `jdbc.properties` 配置文件
> 4. 修改 `@Value` 注解属性的值，将其修改为 `${key}` ，`key` 就是键值对中的键的值

具体的实现就交由大家自行实现下。

### 7.4.2 引用数据类型
#### 1. 需求分析 
假设在构建 `DataSource` 对象时，需要用到 `BookDao` 对象，该如何把 `BookDao` 对象注入进方法内让其使用呢？
```java
public class JdbcConfig {
	@Bean
    public DataSource dataSource() {
        DruidDataSource ds = new DruidDataSource();
        ds.setDriverClassName("com.mysql.jdbc.Driver");
        ds.setUrl("jdbc:mysql://localhost:3306/spring_db");
        ds.setUsername("root");
        ds.setPassword("root");
        return ds;
    }
}
```
#### 2. 注入引用数据类型步骤
步骤1：在 `SpringConfig` 中扫描 `BookDao` 。**扫描的目的是让Spring能管理到 `BookDao`** ，也就是说要让IoC容器中有一个 `bookDao` 对象：
```java
@Configuration
@ComponentScan("com.itheima.dao")
@Import({JdbcConfig.class})
public class SpringConfig {
}
```
步骤2：在 `JdbcConfig` 类的方法上添加参数
```java
@Bean
public DataSource dataSource(BookDao bookDao){
    System.out.println(bookDao);
    DruidDataSource ds = new DruidDataSource();
    ds.setDriverClassName(driver);
    ds.setUrl(url);
    ds.setUsername(userName);
    ds.setPassword(password);
    return ds;
}
```
==引用类型注入只需要为 `bean` 定义方法设置形参即可，容器会根据类型自动装配对象==。

步骤3：运行程序。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191645065.png)

---
# 8. 注解开发总结
前面我们已经完成了**XML配置**和**注解**的开发实现，至于两者之间的差异，咱们放在一块去对比回顾下（`@Autowired` 能解决90%开发需求）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302191647791.png)
