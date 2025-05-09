​怀着忐忑的心情，开始了开发实用篇文档的编写。为什么忐忑？特喵的债欠的太多，不知道从何写起。哎，不煽情了，开工。

​运维实用篇完结以后，开发实用篇采用日更新的形式发布给各位小伙伴，基本上是每天一集，目前已经发布完毕。

​开发实用篇中因为牵扯到SpringBoot整合各种各样的技术，由于不是每个小伙伴对各种技术都有所掌握，所以在整合每一个技术之前，都会做一个快速的普及，这样的话内容整个开发实用篇所包含的内容就会比较多。各位小伙伴在学习的时候，如果对某一个技术不是很清楚，可以先跳过对应章节，或者先补充一下技术知识，然后再来看对应的课程。开发实用篇具体包含的内容如下：
- 热部署
- 配置高级
- 测试
- 数据层解决方案
- 整合第三方技术
- 监控

​看目录感觉内容量并不是很大，但是在数据层解决方案和整合第三方技术中包含了大量的知识，一点一点慢慢学吧。下面开启第一部分热部署相关知识的学习

## KF-1.热部署
​什么是热部署？简单说就是==你程序改了，现在要重新启动服务器==，嫌麻烦？不用重启，==服务器会自己悄悄的把更新后的程序给重新加载一遍==，这就是热部署。

​热部署的功能是如何实现的呢？这就要分两种情况来说了，非springboot工程和springboot工程的热部署实现方式完全不一样。先说一下原始的非springboot项目是如何实现热部署的。
**非springboot项目热部署实现原理**
​开发非springboot项目时，我们要制作一个web工程并通过tomcat启动，通常需要先安装tomcat服务器到磁盘中，开发的程序配置发布到安装的tomcat服务器上。

如果想实现热部署的效果，这种情况其实有两种做法，
- 一种是在tomcat服务器的配置文件中进行配置，这种做法与你使用什么IDE工具无关，不管你使用eclipse还是idea都行。
- 还有一种做法是通过IDE工具进行配置，比如在idea工具中进行设置，这种形式需要依赖IDE工具，每款IDE工具不同，对应的配置也不太一样。但是核心思想是一样的，就是**使用服务器去监控其中加载的应用，发现产生了变化就重新加载一次**。

​上面所说的非springboot项目实现热部署看上去是一个非常简单的过程，几乎每个小伙伴都能自己写出来。如果你不会写，我给你个最简单的思路，但实际设计要比这复杂一些。例如启动一个定时任务，任务启动时记录每个文件的大小，以后每5秒比对一下每个文件的大小是否有改变，或者是否有新文件。如果没有改变，放行，如果有改变，刷新当前记录的文件信息，然后重新启动服务器，这就可以实现热部署了。

当然，这个过程肯定不能这么做，比如我把一个打印输出的字符串"abc"改成"cba"，比对大小是没有变化的，但是内容缺实变了，所以这么做肯定不行，只是给大家打个比方，而且重启服务器这就是冷启动了，不能算热部署，领会精神吧。

​看上去这个过程也没多复杂，在springboot项目中难道还有其他的弯弯绕吗？还真有。
**springboot项目热部署实现原理**
​基于springboot开发的web工程其实有一个显著的特征，就是tomcat服务器内置了，还记得内嵌服务器吗？**服务器是以一个对象的形式在spring容器中运行的**。本来我们期望于==tomcat服务器加载程序后由tomcat服务器盯着程序，你变化后我就重新启动重新加载==，但是现在tomcat和我们的程序是平级的了，都是spring容器中的组件，这下就麻烦了，缺乏了一个直接的管理权，那该怎么做呢？

简单，再搞一个程序X在spring容器中盯着你原始开发的程序A不就行了吗？确实，搞一个盯着程序A的程序X就行了，==如果你自己开发的程序A变化了，那么程序X就命令tomcat容器重新加载程序A就OK了==。并且这样做有一个好处，==spring容器中东西不用全部重新加载一遍，只需要重新加载你开发的程序那一部分就可以了==，这下效率又高了，挺好。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303132241711.png)

​下面就说说，怎么搞出来这么一个程序X，肯定不是我们自己手写了，springboot早就做好了，搞一个坐标导入进去就行了。
### KF-1-1.手动启动热部署
**步骤①**：导入开发者工具对应的坐标
```XML
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-devtools</artifactId>
    <optional>true</optional>
</dependency>
```
**步骤②**：构建项目，可以使用快捷键激活此功能
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251108886.png)
​对应的快捷键一定要记得
```CMD
<CTR>L+<F9>
```
​以上过程就实现了springboot工程的热部署，是不是挺简单的。不过这里需要把底层的工作工程给普及一下。

**重启与重载**
​**一个springboot项目在运行时实际上是分两个过程进行的**，根据加载的东西不同，划分成base类加载器与restart类加载器。
- 重载：base类加载器：用来加载jar包中的类，jar包中的类和配置文件由于不会发生变化，因此不管加载多少次，加载的内容不会发生变化
- 重启：restart类加载器：**用来加载开发者自己开发的类、配置文件、页面等信息**，这一类文件受开发者影响

> 热部署：只有重启,只加载资源文件等，耗时更短；重启：重启+重载，加载资源文件和jar包，耗时更长

​当springboot项目启动时，**base类加载器执行，加载jar包中的信息**后，**restart类加载器执行，加载开发者制作的内容**。当执行构建项目后，由于jar中的信息不会变化，因此base类加载器无需再次执行，所以**仅仅运行restart类加载**即可，也就是**将开发者自己制作的内容重新加载**就行了，这就完成了一次热部署的过程，也可以说==热部署的过程实际上是重新加载restart类加载器中的信息==。

**总结**
1. 使用开发者工具可以为当前项目开启热部署功能
2. 使用构建项目操作对工程进行热部署

**思考**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303132245481.png)
​上述过程每次进行热部署都需要开发者手工操作，不管是点击按钮还是快捷键都需要开发者手工执行。这种操作的应用场景主要是在开发调试期，并且调试的代码处于不同的文件中，比如服务器启动了，我需要改4个文件中的内容，然后重启，等4个文件都改完了再执行热部署，使用一个快捷键就OK了。但是如果现在开发者要修改的内容就只有一个文件中的少量代码，这个时候代码修改完毕如果能够让程序自己执行热部署功能，就可以减少开发者的操作，也就是自动进行热部署，能这么做吗？是可以的。咱们下一节再说。

### KF-1-2.自动启动热部署
​自动热部署其实就是设计一个开关，**打开这个开关后，IDE工具就可以自动热部署**。因此这个操作和IDE工具有关，以下以idea为例，设置idea中启动热部署

**步骤①**：设置自动构建项目
​打开【File】，选择【settings...】,在面板左侧的菜单中找到【Compile】选项，然后勾选【Build project automatically】，意思是自动构建项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251110154.png)
​自动构建项目选项勾选后

**步骤②**：允许在程序运行时进行自动构建
​使用快捷键【Ctrl】+【Alt】+【Shit】+【/】打开维护面板，选择第1项【Registry...】
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251111623.png)
​在选项中搜索comple，然后勾选对应项即可
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251112555.png)
​这样程序在运行的时候就可以进行自动构建了，实现了热部署的效果。

<font color="#ff0000"><b>关注</b></font>：如果你每敲一个字母，服务器就重新构建一次，这未免有点太频繁了，所以idea设置当idea工具失去焦点5秒后进行热部署。其实就是你从idea工具中切换到其他工具时进行热部署，比如改完程序需要到浏览器上去调试，这个时候idea就自动进行热部署操作。
> 新版本idea，这个设置在Settings中的Advanced Settings中，勾选Allow auto-make to...即可

**总结**
1. 自动热部署要开启自动构建项目
2. 自动热部署要开启**在程序运行时自动构建项目**

**思考**
​现在已经实现了热部署了，但是到企业开发的时候你会发现，为了便于管理，在你的程序目录中除了有代码，还有可能有文档，如果你修改了一下文档，这个时候会进行热部署吗？不管是否进行热部署，这个过程我们需要自己控制才比较合理，那这个东西能控制吗？咱们下一节再说。

### KF-1-3.参与热部署监控的文件范围配置
​通过修改项目中的文件，你可以发现其实并不是所有的文件修改都会激活热部署的，原因在于==在开发者工具中有一组配置，当满足了配置中的条件后，才会启动热部署==，配置中默认不参与热部署的目录信息如下
- /META-INF/maven
- /META-INF/resources
- /resources
- /static
- /public
- /templates

​以上目录中的文件如果发生变化，是不参与热部署的。如果想修改配置，可以通过application.yml文件进行设定哪些文件不参与热部署操作
```yaml
spring:
  devtools:
    restart:
      # 设置不参与热部署的文件或文件夹
      exclude: static/**,public/**,config/application.yml
```
**总结**
1. 通过配置可以修改不参与热部署的文件或目录

**思考**
​==热部署功能是一个典型的开发阶段使用的功能，到了线上环境运行程序时，这个功能就没有意义了==。能否关闭热部署功能呢？咱们下一节再说。

---
### KF-1-4.关闭热部署
​线上环境运行时是不可能使用热部署功能的，所以需要强制关闭此功能，通过配置可以关闭此功能。
```yaml
spring:
  devtools:
    restart:
      enabled: false
```
​如果当心配置文件层级过多、导致相符覆盖最终引起配置失效，可以提高配置的层级，在更高层级中配置关闭热部署。例如**在启动容器前通过系统属性设置关闭热部署功能**。
```JAVA
@SpringBootApplication
public class SSMPApplication {
    public static void main(String[] args) {
        System.setProperty("spring.devtools.restart.enabled","false");
        SpringApplication.run(SSMPApplication.class);
    }
}
```

​其实上述担心略微有点多余，因为**线上环境的维护是不可能出现修改代码的操作的**，这么做唯一的作用是降低资源消耗，毕竟那双盯着你项目是不是产生变化的眼睛只要闭上了，就不具有热部署功能了，这个开关的作用就是禁用对应功能。

**总结**
1. 通过配置可以关闭热部署功能降低线上程序的资源消耗

---
## KF-2.配置高级
​进入开发实用篇第二章内容，配置高级，其实配置在基础篇讲了一部分，在运维实用篇讲了一部分，这里还要讲，讲的东西有什么区别呢？距离开发过程越来越接近，解决的问题也越来越靠近线上环境，下面就开启本章的学习。

### KF-2-1.@ConfigurationProperties
​在基础篇学习了@ConfigurationProperties注解，此注解的作用是用来**为bean绑定属性的**。开发者可以在yml配置文件中以对象的格式添加若干属性
```YML
servers:
  ip-address: 192.168.0.1 
  port: 2345
  timeout: -1
```
​然后再开发一个用来封装数据的实体类，注意要提供属性对应的setter方法
```JAVA
@Component
@Data
public class ServerConfig {
    private String ipAddress;
    private int port;
    private long timeout;
}
```
​使用@ConfigurationProperties注解，就可以将配置中的属性值关联到开发的模型类上
```JAVA
@Component // 加载为Bean
@Data
@ConfigurationProperties(prefix = "servers") // 加载属性
public class ServerConfig {
    private String ipAddress;
    private int port;
    private long timeout;
}
```
​这样加载对应bean的时候，就可以直接加载配置属性值了。但是目前我们学的都是给自定义的bean使用这种形式加载属性值，如果是第三方的bean呢？能不能用这种形式加载属性值呢？为什么会提出这个疑问？原因就在于**当前@ConfigurationProperties注解是写在类定义的上方**，而第三方开发的bean源代码不是你自己书写的，你也不可能到源代码中去添加@ConfigurationProperties注解，这种问题该怎么解决呢？下面就来说说这个问题。

​使用@ConfigurationProperties注解，其实可以为第三方bean加载属性，格式特殊一点而已。

**步骤①**：使用@Bean注解定义第三方bean
```JAVA
@Bean
public DruidDataSource datasource(){
    DruidDataSource ds = new DruidDataSource();
    return ds;
}
```
**步骤②**：在yml中定义要绑定的属性，注意datasource此时全小写
```YAML
datasource:
  driverClassName: com.mysql.jdbc.Driver
```
**步骤③**：使用@ConfigurationProperties注解为第三方bean进行属性绑定，注意前缀是全小写的datasource
```JAVA
@Bean // 注册为Bean
@ConfigurationProperties(prefix = "datasource") // 加载属性
public DruidDataSource datasource(){
    DruidDataSource ds = new DruidDataSource();
    return ds;
}
```
​操作方式完全一样，只不过 **@ConfigurationProperties注解不仅能添加到类上，还可以添加到方法上**，添加到类上是为spring容器管理的当前类的对象绑定属性，==添加到方法上是为spring容器管理的当前方法的返回值对象绑定属性，其实本质上都一样==。

​做到这其实就出现了一个新的问题，目前我们**定义bean不是通过类注解@component定义**、就是**通过@Bean定义**，使用@ConfigurationProperties注解可以为bean进行属性绑定，那在一个业务系统中，==哪些bean通过注解@ConfigurationProperties去绑定属性了呢==？因为这个注解不仅可以写在类上，还可以写在方法上，所以找起来就比较麻烦了——为了解决这个问题，spring给我们提供了一个全新的注解，==专门标注「使用@ConfigurationProperties注解绑定属性的bean」是哪些==。这个注解叫做@EnableConfigurationProperties。具体如何使用呢？

> enable的注解相当于指定某个类为bean，而comp的注解也是把类加载为bean，所以有两个
> @Component是标注类，注册为bean；@Bean是标注方法返回的对象，将对象注册为bean

**步骤①**：**在配置类上开启**@EnableConfigurationProperties注解，并用一个数组、==标注要使用@ConfigurationProperties注解绑定属性的类==
```java
@SpringBootApplication
@EnableConfigurationProperties(ServerConfig.class)
public class Springboot13ConfigurationApplication {
}
```
**步骤②**：在对应的类上直接使用@ConfigurationProperties进行属性绑定
```JAVA
@Data
@ConfigurationProperties(prefix = "servers")
public class ServerConfig {
    private String ipAddress;
    private int port;
    private long timeout;
}
```
​有人感觉这没区别啊？注意观察，现在绑定属性的ServerConfig类并没有声明@Component注解。==当使用@EnableConfigurationProperties注解时，spring会默认将其标注的类定义为bean，因此无需再次声明@Component注解了==——@EnableConfigurationProperties注解可以将使用@ConfigurationProperties注解对应的类加入Spring容器。
> @EnableConfigurationProperties与@Component不能同时使用

​最后再说一个小技巧，使用@ConfigurationProperties注解时，会出现一个提示信息
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251114883.png)
​出现这个提示后只需要添加一个坐标，此提醒就消失了
```XML
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-configuration-processor</artifactId>
</dependency>
```
**总结**
1. 使用@ConfigurationProperties可以为使用@Bean声明的第三方bean绑定属性
2. 当使用@EnableConfigurationProperties声明要进行属性绑定的bean后，无需使用@Component注解再次进行bean声明

### KF-2-2.宽松绑定/松散绑定
​在进行属性绑定时，可能会遇到如下情况，为了进行标准命名，开发者会将属性名严格按照驼峰命名法书写，在yml配置文件中将datasource修改为dataSource，如下：
```YAML
dataSource:
  driverClassName: com.mysql.jdbc.Driver
```
​此时**程序可以正常运行**，然后又将代码中的前缀datasource修改为dataSource，如下：
```JAVA
@Bean
@ConfigurationProperties(prefix = "dataSource")
public DruidDataSource datasource(){
    DruidDataSource ds = new DruidDataSource();
    return ds;
}
```
​此时就发生了编译错误，而且并不是idea工具导致的，运行后依然会出现问题，**配置属性名dataSource是无效的**
```CMD
Configuration property name 'dataSource' is not valid:

    Invalid characters: 'S'
    Bean: datasource
    Reason: Canonical names should be kebab-case ('-' separated), lowercase alpha-numeric characters and must start with a letter

Action:
Modify 'dataSource' so that it conforms to the canonical names requirements.
```
​为什么会出现这种问题，这就要来说一说springboot进行属性绑定时的一个重要知识点了，**有关属性名称的宽松绑定**，也可以称为宽松绑定。

​什么是宽松绑定？实际上是springboot进行编程时人性化设计的一种体现，即==配置文件中的命名格式与变量名的命名格式可以进行格式上的最大化兼容==。兼容到什么程度呢？几乎主流的命名格式都支持，例如：​在ServerConfig中的ipAddress属性名
```JAVA
@Component
@Data
@ConfigurationProperties(prefix = "servers")
public class ServerConfig {
    private String ipAddress;
}
```
​可以与下面的配置属性名规则全兼容
```YML
servers:
  ipAddress: 192.168.0.2       # 驼峰模式
  ip_address: 192.168.0.2      # 下划线模式
  ip-address: 192.168.0.2      # 烤肉串模式
  IP_ADDRESS: 192.168.0.2      # 常量模式
```
​也可以说，以上4种模式最终都可以匹配到ipAddress这个属性名。为什么这样呢？原因就是在进行匹配时，==配置中的名称要去掉中划线和下划线后，忽略大小写的情况下，去与java代码中的属性名进行忽略大小写的等值匹配==，以上4种命名去掉下划线中划线忽略大小写后都是一个词ipaddress，java代码中的属性名忽略大小写后也是ipaddress，这样就可以进行等值匹配了，这就是为什么这4种格式都能匹配成功的原因。不过**springboot官方推荐使用烤肉串模式**，也就是中划线模式。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303140035302.png)

​到这里我们掌握了一个知识点，就是命名的规范问题。再来看开始出现的编程错误信息
```CMD
Configuration property name 'dataSource' is not valid:

    Invalid characters: 'S'
    Bean: datasource
    Reason: Canonical names should be kebab-case ('-' separated), lowercase alpha-numeric characters and must start with a letter

Action:
Modify 'dataSource' so that it conforms to the canonical names requirements.
```
​其中Reason描述了报错的原因，规范的名称应该是烤肉串(kebab)模式(case)，即==使用-分隔，使用小写字母、数字作为标准字符，且必须以字母开头==。然后再看我们写的名称dataSource，就不满足上述要求。闹了半天，在书写前缀时，这个词不是随意支持的，必须使用上述标准。编程写了这么久，基本上编程习惯都养成了，到这里又被springboot教育了，没辙，谁让人家东西好用呢，按照人家的要求写吧。

​最后说一句，==以上规则仅针对springboot中@ConfigurationProperties注解进行属性绑定时有效==，对@Value注解进行属性映射无效。有人就说，那我不用你不就行了？不用，你小看springboot的推广能力了，到原理篇我们看源码时，你会发现内部全是这玩意儿，算了，拿人手短吃人嘴短，认怂吧。

**总结**
1. @ConfigurationProperties绑定属性时支持属性名宽松绑定，这个宽松体现在属性名的命名规则上
2. @Value注解不支持松散绑定规则
3. ==绑定前缀名prefix推荐采用烤肉串命名规则，即使用中划线做分隔符，仅能使用纯小写字母、数字作为合法的字符==

### KF-2-3.常用计量单位绑定
​在前面的配置中，我们书写了如下配置值，其中第三项超时时间timeout描述了服务器操作超时时间，当前值是-1表示永不超时。
```YML
servers:
  ip-address: 192.168.0.1 
  port: 2345
  timeout: -1
```
​但每个人对这个值的理解会产生不同，比如线上服务器完成一次主从备份，配置超时时间240，这个240如果单位是秒就是超时时间4分钟，如果单位是分钟就是超时时间4小时。面对一次线上服务器的主从备份，设置4分钟，简直是开玩笑，别说拷贝过程，备份之前的压缩过程4分钟也搞不定，这个时候问题就来了，怎么解决这个误会？

​除了加强约定之外，springboot充分利用了JDK8中提供的全新的用来表示计量单位的新数据类型，从根本上解决这个问题。以下模型类中添加了两个JDK8中新增的类，分别是Duration和DataSize
```JAVA
@Component
@Data
@ConfigurationProperties(prefix = "servers")
public class ServerConfig {
	private String ipAddress;
	private int port;
	private long timeout;
	
	@DurationUnit(ChronoUnit.MINUTES)
	private Duration serverTimeOut;
	@DataSizeUnit(DataUnit.MEGABYTES)
	private DataSize dataSize;
}
```
- **Duration**：表示时间间隔，可以通过@DurationUnit注解描述时间单位，例如上例中描述的单位为小时（ChronoUnit.HOURS）
- **DataSize**：表示存储空间，可以通过@DataSizeUnit注解描述存储空间单位，例如上例中描述的单位为MB（DataUnit.MEGABYTES）；还可不加注解配置，在配置文档中直接写10MB之类的

​使用上述两个单位，就可以有效避免因沟通不同步、或文档不健全导致的信息不对称问题，从根本上解决了问题，避免产生误读。

Duration常用单位如下：
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251115032.png)
DataSize常用单位如下：
![300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251115080.png)

### KF-2-4.校验
​目前我们在进行属性绑定时，可以通过松散绑定规则在书写时放飞自我了，但**在书写时由于无法感知模型类中的数据类型，就会出现类型不匹配的问题**，比如代码中需要int类型，配置中给了非法的数值，例如写一个“a"，这种数据肯定无法有效的绑定，还会引发错误。

SpringBoot给出了强大的数据校验功能，可以有效的避免此类问题的发生。在JAVAEE的JSR303规范中给出了具体的数据校验标准，开发者可以根据自己的需要，选择对应的校验框架，此处使用Hibernate提供的校验框架、来作为实现进行数据校验。书写应用格式非常固定，话不多说，直接上步骤

**步骤①**：开启校验框架
```xml
<!--1.导入JSR303规范-->
<dependency>
    <groupId>javax.validation</groupId>
    <artifactId>validation-api</artifactId>
</dependency>
<!--使用hibernate框架提供的校验器做实现-->
<dependency>
    <groupId>org.hibernate.validator</groupId>
    <artifactId>hibernate-validator</artifactId>
</dependency>
```
**步骤②**：在需要开启校验功能的类上使用注解@Validated开启校验功能
```java
@Component
@Data
@ConfigurationProperties(prefix = "servers")
//开启对当前bean的属性注入校验
@Validated
public class ServerConfig {
}
```
**步骤③**：对具体的字段设置校验规则
```JAVA
@Component
@Data
@ConfigurationProperties(prefix = "servers")
//开启对当前bean的属性注入校验
@Validated
public class ServerConfig {
    //设置具体的规则
    @Max(value = 8888,message = "最大值不能超过8888")
    @Min(value = 202,message = "最小值不能低于202")
    private int port;
}
```
​通过设置数据格式校验，就可以有效避免非法数据加载，其实使用起来还是挺轻松的，基本上就是一个格式。

**总结**
1. 开启Bean属性校验功能一共3步：导入JSR303与Hibernate校验框架坐标、**使用@Validated注解启用校验功能、使用具体校验规则规范数据校验格式**

### KF-2-5.数据类型转换
​有关spring属性注入的问题到这里**基本上就讲完了**，但是最近一名开发者向我咨询了一个问题，我觉得需要给各位学习者分享一下。在学习阶段其实我们遇到的问题往往复杂度比较低，单一性比较强，但到了线上开发时，都是综合性的问题，而这个开发者遇到的问题就是由于bean的属性注入引发的灾难。

​先把问题描述一下，这位开发者连接数据库正常操作，但是运行程序后显示的信息是密码错误。
```CMD
java.sql.SQLException: Access denied for user 'root'@'localhost' (using password: YES)
```
​其实看到这个报错，几乎所有的学习者都能分辨出来，这是用户名和密码不匹配，就就是密码输入错了，但问题就在于密码并没有输入错误，这就比较讨厌了。给的报错信息无法帮助你有效的分析问题，甚至会给你带到沟里。如果是初学者，估计这会心态就崩了，我密码没错啊，你怎么能说我有错误呢？来看看用户名密码的配置是如何写的：
```YAML
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://localhost:3306/ssm_db?serverTimezone=UTC
    username: root
    password: 0127
```
​这名开发者的生日是1月27日，所以密码就使用了0127，其实问题就出在这里了。

​之前在基础篇讲属性注入时，提到过类型相关的知识，在整数相关知识中有这么一句话，**支持二进制，八进制，十六进制**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251116799.png)
​这个问题就在这里了，因为0127在开发者眼中是一个字符串“0127”，但是在springboot看来，这就是一个数字，而且是一个八进制的数字。当后台使用String类型接收数据时，如果配置文件中配置了一个整数值，他是先安装整数进行处理，读取后再转换成字符串。巧了，0127撞上了八进制的格式，所以最终以十进制数字87的结果存在了。

​这里提两个注意点，第一，字符串标准书写加上引号包裹，养成习惯，第二，遇到0开头的数据多注意吧。

**总结**
1. yaml文件中对于数字的定义支持进制书写格式，如需使用字符串请使用引号明确标注

## KF-3.测试
​说完bean配置相关的内容，下面要对前面讲过的一个知识做加强了，测试。测试是保障程序正确性的唯一屏障，在企业级开发中更是不可缺少，但由于测试代码往往不产生实际效益，所以一些小型公司并不是很关注，导致一些开发者从小型公司进入中大型公司后，往往这一块比较短板，所以还是要拿出来把这一块知识好好说说，做一名专业的开发人员。
### KF-3-1.加载测试专用属性
​测试过程本身并不是一个复杂的过程，但是很多情况下测试时需要模拟一些线上情况，或者模拟一些特殊情况。如果当前环境按照线上环境已经设定好了，例如是下面的配置
```YAML
env:
  maxMemory: 32GB
  minMemory: 16GB
```
​但你现在想测试对应的兼容性，需要测试如下配置
```YAML
env:
  maxMemory: 16GB
  minMemory: 8GB
```
​这个时候我们能不能每次测试时都去修改源码application.yml中的配置进行测试呢？显然是不行的。每次测试前改过来，每次测试后改回去，这太麻烦了。于是我们就想，==需要在测试环境中创建一组临时属性，去覆盖我们源码中设定的属性，这样测试用例就相当于是一个独立的环境，能够独立测试==，这样就方便多了。

**临时属性**
​springboot已经为我们开发者早就想好了，这种问题该如何解决，并且提供了对应的功能入口。在测试用例程序中，可以通过==对注解@SpringBootTest添加属性来模拟临时属性==，具体如下：
```JAVA
//properties属性可以为当前测试用例添加临时的属性配置
@SpringBootTest(properties = {"test.prop=testValue1"})
public class PropertiesAndArgsTest {

    @Value("${test.prop}")
    private String msg;
    
    @Test
    void testProperties(){
        System.out.println(msg);
    }
}
```
​使用注解@SpringBootTest的properties属性，就可以为当前测试用例添加临时的属性，覆盖源码配置文件中对应的属性值进行测试。优势：比多环境开发中的测试环境影响范围更小，仅对当前测试类有效。

**临时参数**
​除了上述这种情况，在前面讲解使用命令行启动springboot程序时讲过，==通过命令行参数也可以设置属性值。而且线上启动程序时，通常都会添加一些专用的配置信息==。

作为运维人员他们才不懂java，更不懂这些配置的信息具体格式该怎么写，那如果我们作为开发者提供了对应的书写内容后，能否提前测试一下这些配置信息是否有效呢？当时是可以的，还是通过注解@SpringBootTest的另一个属性来进行设定。
```JAVA
//args属性可以为当前测试用例添加临时的命令行参数
@SpringBootTest(args={"--test.prop=testValue2"})
public class PropertiesAndArgsTest {
    @Value("${test.prop}")
    private String msg;
    
    @Test
    void testProperties(){
        System.out.println(msg);
    }
}
```
​使用注解@SpringBootTest的args属性，就可以为当前测试用例模拟命令行参数并进行测试。

​说到这里，好奇宝宝们肯定就有新问题了，如果两者共存呢？其实如果思考一下配置属性与命令行参数的加载优先级，这个结果就不言而喻了。在属性加载的优先级设定中，有明确的优先级设定顺序，还记得下面这个顺序吗？

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251117200.png)
​在这个属性加载优先级的顺序中，明确规定了命令行参数的优先级排序是11，而配置属性的优先级是3，结果不言而喻了，args属性配置优先于properties属性配置加载。

​到这里我们就掌握了如果在测试用例中去模拟临时属性的设定。

**总结**
1. 加载测试临时属性可以通过注解@SpringBootTest的properties和args属性进行设定，**此设定应用范围仅适用于当前测试用例**

**思考**
​应用于测试环境的临时属性解决了，如果想在测试的时候临时加载一些bean能不做呢？也就是说我测试时，想搞一些独立的bean出来，专门应用于测试环境，能否实现呢？咱们下一节再讲。

### KF-3-2.加载测试专用配置
​上一节提出了临时配置一些专用于测试环境的bean的需求，这一节我们就来解决这个问题。

​学习过Spring的知识，我们都知道，其实==一个spring环境中可以设置若干个配置文件或配置类，若干个配置信息可以同时生效==。现在我们的需求就是，在测试环境中再添加一个配置类，然后**启动测试环境时，生效此配置就行了**。其实做法和spring环境中**加载多个配置信息**的方式完全一样。具体操作步骤如下：

**步骤①**：在测试包test中创建专用的测试环境配置类。
```java
@Configuration
public class MsgConfig {
    @Bean
    public String msg(){
        return "bean msg";
    }
}
```
​上述配置仅用于演示当前实验效果，实际开发可不能这么注入String类型的数据

**步骤②**：在启动测试环境时，导入测试环境专用的配置类，使用@Import注解即可实现
```java
@SpringBootTest
@Import({MsgConfig.class})
public class ConfigurationTest {
    @Autowired
    private String msg;

    @Test
    void testConfiguration(){
        System.out.println(msg);
    }
}
```
​到这里就通过@Import属性，实现了==基于开发环境的配置基础上，对配置进行测试环境的追加操作，实现了1+1的配置环境效果==。这样我们就可以实现**每一个不同的测试用例加载不同的bean的效果，丰富测试用例的编写，同时不影响开发环境的配置**。
> 加上@Configuration注解 SpringBoot会自动扫描到的啊
> 这个追加配置貌似ssm那会儿不太好用，换configuration反而可以
> 在测试目录下,如果配置类和application启动类一个目录级别下,可以不用写Import注解
> 如果配置类在标准的SpringBoot包结构下(SpringBootApplication启动类包的根目录下)。是不需要@Import导入配置类的，SpringBoot自动帮做了
> 那些不加import也能跑的哪些人，import是加载配置类的，不用是加载其他类的
> 前面那位兄弟,可以去了解一下 import可以帮助我们解决很多代码,所以可以使用import,包括自动注入也是有一部分这个原理
> @configuration会被spring容器扫到并加入，@import单独导入使用

**总结**
1. 定义测试环境专用的配置类，然后通过@Import注解在具体的测试中导入临时的配置，例如测试用例，方便测试过程，且上述配置不影响其他的测试类环境

**思考**
​当前我们已经可以实现业务层和数据层的测试，并且==通过临时配置，控制每个测试用例加载不同的测试数据==。但实际企业开发不仅要保障业务层与数据层的功能安全有效，也要保障表现层的功能正常。

但我们目的对表现层的测试都是通过postman手工测试的，并没有在打包过程中体现表现层功能被测试通过。能否在测试用例中对表现层进行功能测试呢？还真可以，咱们下一节再讲。
### KF-3-3.Web环境模拟测试
​在测试中对表现层功能进行测试需要一个基础和一个功能。所谓的==一个基础是运行测试程序时，必须启动web环境，不然没法测试web功能==。==一个功能是必须在测试程序中具备发送web请求的能力，不然无法实现web功能的测试==。

所以在测试用例中测试表现层接口这项工作就转换成了两件事，一，如何在测试类中启动web测试，二，如何在测试类中发送web请求。下面一件事一件事进行，先说第一个

**测试类中启动web环境**
​每一个springboot的测试类上方，都会标准@SpringBootTest注解，而注解带有一个属性，叫做webEnvironment。通过该属性、就可以**设置在测试用例中启动web环境**，具体如下：
```JAVA
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class WebTest {	
}
```
​测试类中启动web环境时，可以指定启动的Web环境对应的端口，springboot提供了4种设置值，分别如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251117167.png)
- MOCK：根据当前设置确认是否启动web环境，例如使用了Servlet的API就启动web环境，属于适配性的配置
- DEFINED_PORT：使用自定义的端口作为web服务器端口
- RANDOM_PORT：使用随机端口作为web服务器端口
- NONE：不启动web环境

​通过上述配置，现在启动测试程序时、就可以正常启用web环境了，建议测试时使用RANDOM_PORT，避免代码中因为写死设定、引发线上功能打包测试时、由于端口冲突导致意外现象的出现。就是说你程序中写了用8080端口，结果线上环境8080端口被占用了，结果你代码中所有写的东西都要改，这就是写死代码的代价。现在你用随机端口就可以测试出来你有没有这种问题的隐患了。

​测试环境中的web环境已经搭建好了，下面就可以来解决第二个问题了，如何在程序代码中发送web请求。

**测试类中发送请求**
​对于测试类中发送请求，其实java的API就提供对应的功能，只不过平时各位接触的比较少，所以较为陌生。springboot为了便于开发者进行对应的功能开发，对其又进行了包装，简化了开发步骤，具体操作如下：

**步骤①**：在测试类中开启**web虚拟调用功能**，通过注解@AutoConfigureMockMvc实现此功能的开启
```java
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
//开启虚拟MVC调用
@AutoConfigureMockMvc
public class WebTest {
}
```
**步骤②**：定义发起虚拟调用的对象MockMVC，通过自动装配的形式初始化对象
```java
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
//开启虚拟MVC调用
@AutoConfigureMockMvc
public class WebTest {
    @Test
    void testWeb(@Autowired MockMvc mvc) {
    }
}
```
**步骤③**：==创建一个虚拟请求对象，封装请求的路径，并使用MockMVC对象发送对应请求==
```java
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
//开启虚拟MVC调用
@AutoConfigureMockMvc
public class WebTest {

    @Test
    void testWeb(@Autowired MockMvc mvc) throws Exception {
        //http://localhost:8080/books
        //创建虚拟请求，当前访问/books
        MockHttpServletRequestBuilder builder = MockMvcRequestBuilders.get("/books");
        //执行对应的请求
        mvc.perform(builder);
    }
}
```
​执行测试程序，现在就可以正常的发送/books对应的请求了，注意访问路径不要写http://localhost:8080/books，==因为前面的服务器IP地址和端口使用的是当前虚拟的web环境==，无需指定，仅指定请求的具体路径即可。

**总结**
1. 在测试类中测试web层接口，要保障测试类启动时启动web容器，使用@SpringBootTest注解的webEnvironment属性可以虚拟web环境用于测试
2. 为测试方法注入MockMvc对象，通过MockMvc对象可以发送虚拟请求，模拟web请求调用过程

**思考**
​目前已经成功的发送了请求，但还没有起到测试的效果，测试过程必须出现预计值与真实值的比对结果，才能确认测试结果是否通过，虚拟请求中能对哪些请求结果进行比对呢？咱们下一节再讲。

**web环境请求结果比对**
​上一节已经在测试用例中成功的模拟出了web环境，并成功的发送了web请求，本节就来解决发送请求后如何比对发送结果的问题。其实发完请求得到的信息只有一种，就是响应对象。至于响应对象中包含什么，就可以比对什么。常见的比对内容如下：
- 响应状态匹配
  ```JAVA
  @Test
  void testStatus(@Autowired MockMvc mvc) throws Exception {
      MockHttpServletRequestBuilder builder = MockMvcRequestBuilders.get("/books");
      ResultActions action = mvc.perform(builder);
      //设定预期值 与真实值进行比较，成功测试通过，失败测试失败
      //定义本次调用的预期值
      StatusResultMatchers status = MockMvcResultMatchers.status();
      //预计本次调用时成功的：状态200
      ResultMatcher ok = status.isOk();
      //添加预计值到本次调用过程中进行匹配
      action.andExpect(ok);
  }
  ```
- 响应体匹配（非json数据格式）
  ```JAVA
  @Test
  void testBody(@Autowired MockMvc mvc) throws Exception {
      MockHttpServletRequestBuilder builder = MockMvcRequestBuilders.get("/books");
      ResultActions action = mvc.perform(builder);
      //设定预期值 与真实值进行比较，成功测试通过，失败测试失败
      //定义本次调用的预期值
      ContentResultMatchers content = MockMvcResultMatchers.content();
      ResultMatcher result = content.string("springboot2");
      //添加预计值到本次调用过程中进行匹配
      action.andExpect(result);
  }
  ```
  ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303140125185.png)
- 响应体匹配（json数据格式，**开发中的主流使用方式**）
  ```JAVA
  @Test
  void testJson(@Autowired MockMvc mvc) throws Exception {
      MockHttpServletRequestBuilder builder = MockMvcRequestBuilders.get("/books");
      ResultActions action = mvc.perform(builder);
      //设定预期值 与真实值进行比较，成功测试通过，失败测试失败
      //定义本次调用的预期值
      ContentResultMatchers content = MockMvcResultMatchers.content();
      ResultMatcher result = content.json("{\"id\":1,\"name\":\"springboot2\",\"type\":\"springboot\"}");
      //添加预计值到本次调用过程中进行匹配
      action.andExpect(result);
  }
  ```
- 响应头信息匹配
  ```JAVA
  @Test
  void testContentType(@Autowired MockMvc mvc) throws Exception {
      MockHttpServletRequestBuilder builder = MockMvcRequestBuilders.get("/books");
      ResultActions action = mvc.perform(builder);
      //设定预期值 与真实值进行比较，成功测试通过，失败测试失败
      //定义执行结果匹配器
      HeaderResultMatchers header = MockMvcResultMatchers.header();
      //定义预期执行结果
      ResultMatcher contentType = header.string("Content-Type", "application/json");
      //使用本次真实执行结果与预期结果进行比对
      action.andExpect(contentType);
  }
  ```

​基本上齐了，头信息，正文信息，状态信息都有了，就可以组合出一个完美的响应结果比对结果了。以下范例就是三种信息同时进行匹配校验，也是==一个完整的信息匹配过程==。
```JAVA
@Test
void testGetById(@Autowired MockMvc mvc) throws Exception {
    MockHttpServletRequestBuilder builder = MockMvcRequestBuilders.get("/books");
    ResultActions action = mvc.perform(builder);

    StatusResultMatchers status = MockMvcResultMatchers.status();
    ResultMatcher ok = status.isOk();
    action.andExpect(ok);

    HeaderResultMatchers header = MockMvcResultMatchers.header();
    ResultMatcher contentType = header.string("Content-Type", "application/json");
    action.andExpect(contentType);

    ContentResultMatchers content = MockMvcResultMatchers.content();
    ResultMatcher result = content.json("{\"id\":1,\"name\":\"springboot\",\"type\":\"springboot\"}");
    action.andExpect(result);
}
```
**总结**
1. web虚拟调用可以对本地虚拟请求的返回响应信息进行比对，分为响应头信息比对、响应体信息比对、响应状态信息比对

### KF-3-4.数据层测试回滚
​当前我们的测试程序可以完美的进行表现层、业务层、数据层接口对应的功能测试了，但==测试用例开发完成后，在打包的阶段，由于test生命周期属于必须被运行的生命周期，如果跳过会给系统带来极高的安全隐患，所以测试用例必须执行==。但新的问题就呈现了，==测试用例如果测试时产生了事务提交，就会在测试过程中对数据库数据产生影响，进而产生垃圾数据==。这个过程不是我们希望发生的，作为开发者测试用例该运行运行，但过程中产生的数据不要在我的系统中留痕，这样该如何处理呢？

​springboot早就为开发者想到了这个问题，并且针对此问题给出了最简解决方案，在原始测试用例中添加注解@Transactional，即可实现当前测试用例的事务不提交。

==当程序运行后，只要注解@Transactional出现的位置存在注解@SpringBootTest，springboot就会认为这是一个测试程序，无需提交事务，所以也就可以避免事务的提交==。
```JAVA
@SpringBootTest
@Transactional
@Rollback(false)
public class DaoTest {
    @Autowired
    private BookService bookService;

    @Test
    void testSave(){
        Book book = new Book();
        book.setName("springboot3");
        book.setType("springboot3");
        book.setDescription("springboot3");

        bookService.save(book);
    }
}
```
​如果开发者想提交事务，也可以，**再添加一个@RollBack的注解，设置回滚状态为false即可正常提交事务，是不是很方便**？springboot在辅助开发者日常工作这一块展现出了惊人的能力，实在太贴心了。
> 默认情况下mysql数据库使用的是Innodb存储引擎（5.5版本之后），它是支持事务的，但是如果你的表的存储引擎是MyISAM，MyISAM是不支持事务的。这样就会出现“事务失效”的问题了。

**总结**
1. 在springboot的测试类中通过添加注解@Transactional来阻止测试用例提交事务
2. 通过注解@Rollback控制springboot测试类执行结果是否提交事务，需要配合注解@Transactional使用

**思考**
​当前测试程序已经近乎完美了，但由于测试用例中书写的测试数据属于固定数据，往往失去了测试的意义，开发者可以针对测试用例进行针对性开发，这样就有可能出现测试用例不能完美呈现「业务逻辑代码是否真实有效的达成业务目标」的现象，解决方案其实很容易想，测试用例的数据只要随机产生就可以了，能实现吗？咱们下一节再讲。

### KF-3-5.测试用例数据设定
​对于测试用例的数据固定书写肯定是不合理的，springboot提供了在配置中使用随机值的机制，确保每次运行程序加载的数据都是随机的。具体如下：
```yaml
testcase:
  book:
    id: ${random.int}
    id2: ${random.int(10)}
    type: ${random.int!5,10!}
    name: ${random.value}
    uuid: ${random.uuid}
    publishTime: ${random.long}
```
​当前配置就可以在每次运行程序时创建一组随机数据，避免每次运行时数据都是固定值的尴尬现象发生，有助于测试功能的进行。数据的加载按照之前加载数据的形式，使用@ConfigurationProperties注解即可（或者用@EnableConfigurationProperties）：
```JAVA
@Component
@Data
@ConfigurationProperties(prefix = "testcase.book")
public class BookCase {
    private int id;
    private int id2;
    private int type;
    private String name;
    private String uuid;
    private long publishTime;
}
```
​对于随机值的产生，还有一些小的限定规则，比如产生的数值性数据可以设置范围等，具体如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251121354.png)
- `${random.int}` 表示随机整数
- `${random.int(10)}` 表示10以内的随机数
- `${random.int(10,20)}` 表示10到20的随机数
- 其中()可以是任意字符，例如[]，!!均可

**总结**
1. 使用随机数据可以替换测试用例中书写的固定数据，提高测试用例中的测试数据有效性

## KF-4.数据层解决方案
​开发实用篇前三章基本上是开胃菜，从第四章开始，开发实用篇进入到了噩梦难度了，从这里开始，不再是单纯的在springboot内部搞事情了，要涉及到很多相关知识。本章节主要内容都是和数据存储与读取相关，前期学习的知识与数据层有关的技术基本上都围绕在数据库这个层面上，所以本章要讲的第一个大的分支，就是**SQL解决方案**相关的内容，除此之外，数据的来源还可以是非SQL技术相关的数据操作，因此第二部分围绕着**NOSQL解决方案**讲解。至于什么是NOSQL解决方案，讲到了再说吧。下面就从SQL解决方案说起。
### KF-4-1.SQL
​回忆一下之前做SSMP整合时，数据层解决方案涉及到了哪些技术？MySQL数据库与MyBatisPlus框架，后面又学了Druid数据源的配置，所以现在数据层解决方案可以说是Mysql+Druid+MyBatisPlus。而三个技术分别对应了数据层操作的三个层面：
- 数据源技术：Druid
- 持久化技术：MyBatisPlus
- 数据库技术：MySQL

​下面的研究就分为三个层面进行研究，对应上面列出的三个方面，咱们就从第一个数据源技术开始说起。
#### 数据源技术
​目前我们使用的数据源技术是Druid（导入了starter就用了Druid数据源），运行时可以在日志中看到对应的数据源初始化信息，具体如下：
```CMD
INFO 28600 --- [           main] c.a.d.s.b.a.DruidDataSourceAutoConfigure : Init DruidDataSource
INFO 28600 --- [           main] com.alibaba.druid.pool.DruidDataSource   : {dataSource-1} inited
```
​如果不使用Druid数据源，程序运行后是什么样子呢？是独立的数据库连接对象还是有其他的连接池技术支持呢？将Druid技术对应的starter去掉，再次运行程序可以在日志中找到如下初始化信息：
```CMD
INFO 31820 --- [           main] com.zaxxer.hikari.HikariDataSource       : HikariPool-1 - Starting...
INFO 31820 --- [           main] com.zaxxer.hikari.HikariDataSource       : HikariPool-1 - Start completed.
```
​虽然没有DruidDataSource相关的信息了，但是我们发现日志中有HikariDataSource这个信息，就算不懂这是个什么技术，看名字也能看出来，以DataSource结尾的名称，这一定是一个数据源技术。我们又没有手工添加这个技术，这个技术哪里来的呢？这就是这一节要讲的知识，springboot内嵌数据源。

==​数据层技术是每一个企业级应用程序都会用到的，而其中必定会进行数据库连接的管理==。springboot根据开发者的习惯出发，开发者提供了数据源技术，就用你提供的，==开发者没有提供，那总不能手工管理一个一个的数据库连接对象啊==，怎么办？我给你一个默认的就好了，这样省心又省事，大家都方便。

​springboot提供了3款内嵌数据源技术，分别如下：
- HikariCP：HikartCP，这是springboot官方推荐的数据源技术，作为默认内置数据源使用。啥意思？你不配置数据源，那就用这个。
- Tomcat提供DataSource：Tomcat提供的DataSource，如果不想用HikartCP，并且使用tomcat作为web服务器进行web程序的开发，使用这个。为什么是Tomcat，不是其他web服务器呢？==因为web技术导入starter后，默认使用内嵌tomcat，既然都是默认使用的技术了，那就一用到底，数据源也用它的。有人就提出怎么才能不使用HikartCP用tomcat提供的默认数据源对象呢？把HikartCP技术的坐标排除掉就OK了==。
- Commons DBCP：DBCP，这个使用的条件就更苛刻了，既不使用HikartCP也不使用tomcat的DataSource时，默认给你用这个。

​springboot这心操的，也是稀碎啊，就怕你自己管不好连接对象，给你一顿推荐，真是开发界的最强辅助。既然都给你奶上了，那就受用吧，怎么配置使用这些东西呢？之前我们配置druid时使用druid的starter对应的配置如下：
```YAML
spring:
  datasource:
    druid:	
   	  url: jdbc:mysql://localhost:3306/ssm_db?serverTimezone=UTC
      driver-class-name: com.mysql.cj.jdbc.Driver
      username: root
      password: root
```
​换成是默认的数据源HikariCP后，直接把druid删掉就行了，如下：
```YAML
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/ssm_db?serverTimezone=UTC
    driver-class-name: com.mysql.cj.jdbc.Driver
    username: root
    password: root
```
​通用配置无法设置具体的数据源配置信息，仅提供基本的连接相关配置，如需配置，在下一级配置中设置具体设定：
```yaml
spring:
  datasource:
    driver-class-name: com.mysql.cj.jdbc.Driver
    url: jdbc:mysql://localhost:3306/ssm_db
    username: root
    password: root
    hikari:
      maximum-pool-size: 50
```
当然，也可以写上是对hikari做的配置，但**url地址要单独配置**（不要用jdbc-url在hikari里面配置数据库URL），如下：
```YAML
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/ssm_db?serverTimezone=UTC
    hikari:
      driver-class-name: com.mysql.cj.jdbc.Driver
      username: root
      password: root
```
​这就是配置hikari数据源的方式。如果想对hikari做进一步的配置，可以继续配置其独立的属性。例如：
```yaml
spring:
  datasource:
    url: jdbc:mysql://localhost:3306/ssm_db?serverTimezone=UTC
    hikari:
      driver-class-name: com.mysql.cj.jdbc.Driver
      username: root
      password: root
      maximum-pool-size: 50
```
如果不想使用hikari数据源，使用tomcat的数据源或者DBCP配置格式也是一样的。学习到这里，==以后我们做数据层时，数据源对象的选择就不再是单一的使用druid数据源技术了，可以根据需要自行选择==。

**总结**
1. springboot技术提供了3种内置的数据源技术，分别是Hikari、tomcat内置数据源、DBCP

#### 持久化技术
​说完数据源解决方案，再来说一下持久化解决方案。springboot充分发挥其最强辅助的特征，给开发者提供了一套现成的数据层技术，叫做JdbcTemplate。其实这个技术不能说是springboot提供的，因为不使用springboot技术，一样能使用它，谁提供的呢？spring技术提供的，所以在springboot技术范畴中，这个技术也是存在的，毕竟springboot技术是加速spring程序开发而创建的。

​这个技术其实就是==回归到jdbc最原始的编程形式来进行数据层的开发==，下面直接上操作步骤：
**步骤①**：导入jdbc对应的坐标，记得是starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jdbc</artifactId>
</dependency
```
**步骤②**：自动装配JdbcTemplate对象
```java
@SpringBootTest
class Springboot15SqlApplicationTests {
    @Test
    void testJdbcTemplate(@Autowired JdbcTemplate jdbcTemplate){
    }
}
```
**步骤③**：使用JdbcTemplate实现查询操作（非实体类封装数据的查询操作）
```java
@Test
void testJdbcTemplate(@Autowired JdbcTemplate jdbcTemplate){
    String sql = "select * from tbl_book";
    List<Map<String, Object>> maps = jdbcTemplate.queryForList(sql);
    System.out.println(maps);
}
```
**步骤④**：使用JdbcTemplate实现查询操作（实体类封装数据的查询操作）
```java
@Test
void testJdbcTemplate(@Autowired JdbcTemplate jdbcTemplate){

    String sql = "select * from tbl_book";
    RowMapper<Book> rm = new RowMapper<Book>() {
        @Override
        public Book mapRow(ResultSet rs, int rowNum) throws SQLException {
            Book temp = new Book();
            temp.setId(rs.getInt("id"));
            temp.setName(rs.getString("name"));
            temp.setType(rs.getString("type"));
            temp.setDescription(rs.getString("description"));
            return temp;
        }
    };
    List<Book> list = jdbcTemplate.query(sql, rm);
    System.out.println(list);
}
```
**步骤⑤**：使用JdbcTemplate实现增删改操作
```java
@Test
void testJdbcTemplateSave(@Autowired JdbcTemplate jdbcTemplate){
    String sql = "insert into tbl_book values(3,'springboot1','springboot2','springboot3')";
    jdbcTemplate.update(sql);
}
```
​如果想对JdbcTemplate对象进行相关配置，可以在yml文件中进行设定，具体如下：
```yaml
spring:
  jdbc:
    template:
      query-timeout: -1   # 查询超时时间
      max-rows: 500       # 最大行数
      fetch-size: -1      # 缓存行数
```
**总结**
1. SpringBoot内置JdbcTemplate持久化解决方案
2. 使用JdbcTemplate需要导入spring-boot-starter-jdbc的坐标

#### 数据库技术
​截止到目前，springboot给开发者提供了内置的数据源解决方案和持久化解决方案，在数据层解决方案三件套中还剩下一个数据库，莫非springboot也提供有内置的解决方案？还真有，还不是一个，三个，这一节就来说说内置的数据库解决方案。

​springboot提供了3款内置的数据库，分别是
- H2
- HSQL
- Derby

​以上三款数据库除了可以独立安装之外，还可以==像是tomcat服务器一样，采用内嵌的形式运行在spirngboot容器中==。内嵌在容器中运行，那必须是java对象啊，对，这三款数据库底层都是使用java语言开发的。

​我们一直使用MySQL数据库就挺好的，为什么有需求用这个呢？原因就在于==这三个数据库都可以采用内嵌容器的形式运行==，在应用程序运行后，如果我们进行测试工作，此时测试的数据无需存储在磁盘上，但又要测试使用，内嵌数据库就方便了，运行在内存中，该测试测试，该运行运行，等服务器关闭后，一切烟消云散，多好，省得你维护外部数据库了。这也是**内嵌数据库的最大优点，方便进行功能测试**。

​下面以H2数据库为例讲解如何使用这些内嵌数据库，操作步骤也非常简单，简单才好用嘛。
**步骤①**：导入H2数据库对应的坐标，一共2个
```xml
<dependency>
    <groupId>com.h2database</groupId>
    <artifactId>h2</artifactId>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-jpa</artifactId>
</dependency>
```
**步骤②**：将工程设置为web工程，启动工程时启动H2数据库
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
**步骤③**：（把其他的配置都注释掉）通过配置开启H2数据库控制台访问程序，也可以使用其他的数据库连接软件操作
```yaml
server:
  port: 80
spring:
  h2:
    console:
      enabled: true
      path: /h2
```
​web端访问路径/h2，访问密码123456。如果访问失败，先配置下列数据源，启动程序运行后，再次访问/h2路径就可以正常访问了
```yaml
datasource:
  url: jdbc:h2:~/test
  hikari:
    driver-class-name: org.h2.Driver
    username: sa
    password: 123456
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303141538228.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303141541155.png)

**步骤④**：使用JdbcTemplate或MyBatisPlus技术操作数据库（略）

​其实我们只是换了一个数据库而已，其他的东西都不受影响。一个重要提醒，别忘了，==上线时，把内存级数据库关闭，采用MySQL数据库作为数据持久化方案，关闭方式就是设置enabled属性为false即可==。
> h2只能连接一个，把springboot程序关了再来测试

**总结**
1. H2内嵌式数据库启动方式，添加坐标，添加配置
2. H2数据库线上运行时请务必关闭

​到这里SQL相关的数据层解决方案就讲完了，现在的可选技术就丰富的多了。
- 数据源技术：Druid、Hikari、tomcat DataSource、DBCP
- 持久化技术：MyBatisPlus、MyBatis、JdbcTemplate
- 数据库技术：MySQL、H2、HSQL、Derby

​现在开发程序时，就可以在以上技术中任选一种组织成一套数据库解决方案了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303141548651.png)

### KF-4-2.NoSQL
​SQL数据层解决方案说完了，下面说NoSQL数据层解决方案。这个NoSQL是什么意思呢？从字面来看，No表示否定，NoSQL就是非关系型数据库解决方案，意思就是数据该存存该取取，只是这些数据不放在关系型数据库中了，那放在哪里？自然是一些能够存储数据的其他相关技术中了，比如Redis等。

本节讲解的内容就是springboot如何整合这些技术，在springboot官方文档中提供了10种相关技术的整合方案，我们将讲解国内市场上最流行的几款NoSQL数据库整合方案，分别是Redis、MongoDB、ES。

​因为每个小伙伴学习这门课程的时候起点不同，为了便于各位学习者更好的学习，每种技术在讲解整合前都会先讲一下安装和基本使用，然后再讲整合。如果对某个技术比较熟悉的小伙伴可以直接跳过安装的学习过程，直接看整合方案即可。

此外上述这些技术最佳使用方案都是在Linux服务器上部署，但是考虑到各位小伙伴的学习起点差异过大，所以下面都以Windows平台作为安装基础讲解，如果想看Linux版软件安装，可以再找到对应技术的学习文档查阅学习。
#### SpringBoot整合Redis
​Redis是一款采用key-value数据存储格式的内存级NoSQL数据库，重点关注数据存储格式，是key-value格式，也就是键值对的存储形式。与MySQL数据库不同，MySQL数据库有表、有字段、有记录。Redis没有这些东西，就是一个名称对应一个值，并且数据以存储在内存中使用为主。什么叫**以存储在内存中为主**？其实Redis有它的数据持久化方案，分别是RDB和AOF，但==Redis自身并不是为了数据持久化而生的，主要是在内存中保存数据，加速数据访问的，所以说是一款内存级数据库==。

​Redis支持多种数据存储格式，比如可以直接存字符串，也可以存一个map集合，list集合，后面会涉及到一些不同格式的数据操作，这个需要先学习一下才能进行整合，所以在基本操作中会介绍一些相关操作。下面就先安装，再操作，最后说整合

##### 安装
​windows版安装包下载地址：https://github.com/tporadowski/redis/releases

​下载的安装包有两种形式，一种是一键安装的msi文件，还有一种是解压缩就能使用的zip文件，哪种形式都行，这里不介绍安装过程了，采用的是msi一键安装的msi文件进行安装的。

​啥是msi，其实就是一个文件安装包，不仅安装软件，还帮你把安装软件时需要的功能关联在一起，打包操作。比如如安装序列、创建和设置安装路径、设置系统依赖项、默认设定安装选项和控制安装过程的属性。说简单点就是一站式服务，安装过程一条龙操作一气呵成，就是为小白用户提供的软件安装程序。

​安装完毕后会得到如下文件，其中有两个文件对应两个命令，是启动Redis的核心命令，需要再CMD命令行模式执行。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303141528773.png)
**启动服务器**
```CMD
redis-server.exe redis.windows.conf
```
​初学者无需调整服务器对外服务端口，默认6379。
**启动客户端**
```CMD
redis-cli.exe
```
​如果启动redis服务器失败，可以先启动客户端，然后执行shutdown操作后退出，此时redis服务器就可以正常执行了。
##### 基本操作
​服务器启动后，使用客户端就可以连接服务器，类似于启动完MySQL数据库，然后启动SQL命令行操作数据库。

​放置一个字符串数据到redis中，先为数据定义一个名称，比如name,age等，然后使用命令set设置数据到redis服务器中即可
```CMD
set name itheima
set age 12
```
​从redis中取出已经放入的数据，根据名称取，就可以得到对应数据。如果没有对应数据就会得到(nil)
```CMD
get name
get age
```
​以上使用的数据存储是一个名称对应一个值，如果要维护的数据过多，可以使用别的数据存储结构。例如hash，它是一种一个名称下可以存储多个数据的存储模型，并且每个数据也可以有自己的二级存储名称。向hash结构中存储数据格式如下：
```
hset a a1 aa1#对外key名称是a，在名称为a的存储模型中，a1这个key中保存了数据aa1
hset a a2 aa2
```
​获取hash结构中的数据命令如下
```CMD
hget a a1	#得到aa1
hget a a2	#得到aa2
```
​有关redis的基础操作就普及到这里，需要全面掌握redis技术，请参看相关教程学习。

##### 整合
​在进行整合之前先梳理一下整合的思想，springboot整合任何技术其实就是在springboot中使用对应技术的API。如果两个技术没有交集，就不存在整合的概念了。所谓整合其实就是使用springboot技术去管理其他技术，几个问题是躲不掉的。
- ​第一，需要先导入对应技术的坐标，而整合之后，这些坐标都有了一些变化
- ​第二，任何技术通常都会有一些相关的设置信息，整合之后，这些信息如何写，写在哪是一个问题
- ​第三，==没有整合之前操作如果是模式A的话，整合之后如果没有给开发者带来一些便捷操作，那整合将毫无意义==，所以整合后操作肯定要简化一些，那对应的操作方式自然也有所不同

​按照上面的三个问题，去思考springboot整合所有技术，是一种通用思想，在整合的过程中会逐步摸索出整合的套路，而且适用性非常强，经过若干种技术的整合后，基本上可以总结出一套固定思维。

​下面就开始springboot整合redis，操作步骤如下：
**步骤①**：导入springboot整合redis的starter坐标
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
​上述坐标可以在创建模块的时候通过勾选的形式进行选择，归属NoSQL分类中
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303141653182.png)
**步骤②**：进行基础配置
```yaml
spring:
  redis:
    host: localhost
    port: 6379
```
​==操作redis，最基本的信息就是操作哪一台redis服务器，所以服务器地址属于基础配置信息，不可缺少==。但即便你不配置，目前也是可以用的。因为以上两组信息都有默认配置，刚好就是上述配置值。

**步骤③**：使用springboot整合redis的专用客户端接口操作，此处使用的是RedisTemplate：
```java
@SpringBootTest
class Springboot16RedisApplicationTests {
    @Autowired
    private RedisTemplate redisTemplate;
    @Test
    void set() {
        ValueOperations ops = redisTemplate.opsForValue();
        ops.set("age",41);
    }
    @Test
    void get() {
        ValueOperations ops = redisTemplate.opsForValue();
        Object age = ops.get("name");
        System.out.println(age);
    }
    @Test
    void hset() {
        HashOperations ops = redisTemplate.opsForHash();
        ops.put("info","b","bb");
    }
    @Test
    void hget() {
        HashOperations ops = redisTemplate.opsForHash();
        Object val = ops.get("info", "b");
        System.out.println(val);
    }
}
```
> redis装在虚拟机的，记得开放对应的端口或者直接把防火墙打开。同时redis需要配置远程连接，否则无法在外部连接，只能支持localhost
> 如果运行为空，那就是你没有运行set方法，没有设置值怎么得到值嘞

​在操作redis时，需要先确认操作何种数据，根据数据种类得到操作接口。例如使用opsForValue()获取string类型的数据操作接口，使用opsForHash()获取hash类型的数据操作接口，剩下的就是调用对应api操作了。各种类型的数据操作接口如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251122366.png)
**总结**
1. springboot整合redis步骤
   1. 导入springboot整合redis的starter坐标
   2. 进行基础配置
   3. 使用springboot整合redis的专用客户端接口RedisTemplate操作，RedisTemplate以对象作为key和value，内部对数据进行序列化

**StringRedisTemplate**
​==由于redis内部不提供java对象的存储格式，因此当操作的数据以对象的形式存在时，会进行转码，转换成字符串格式后进行操作==。为了方便开发者使用基于字符串为数据的操作，springboot整合redis时提供了专用的API接口StringRedisTemplate，你可以理解为这是RedisTemplate的一种指定数据泛型的操作API。StringRedisTemplate以字符串作为key和value，与Redis（命令行）客户端操作等效：
```JAVA
@SpringBootTest
public class StringRedisTemplateTest {
    @Autowired
    private StringRedisTemplate stringRedisTemplate;
    @Test
    void get(){
        ValueOperations<String, String> ops = stringRedisTemplate.opsForValue();
        String name = ops.get("name");
        System.out.println(name);
    }
}
```
**redis客户端选择**
springboot整合redis技术提供了多种客户端兼容模式，默认提供的是lettucs客户端技术，也可以根据需要切换成指定客户端技术，例如jedis客户端技术，切换成jedis客户端技术操作步骤如下：

**步骤①**：导入jedis坐标
```xml
<dependency>
    <groupId>redis.clients</groupId>
    <artifactId>jedis</artifactId>
</dependency>
```
​jedis坐标受springboot管理，无需提供版本号
**步骤②**：配置客户端技术类型，设置为jedis
```yaml
spring:
  redis:
    host: localhost
    port: 6379
    client-type: jedis
```
**步骤③**：根据需要设置对应的配置
```yaml
spring:
  redis:
    host: localhost
    port: 6379
    client-type: jedis
    lettuce:
      pool:
        max-active: 16
    jedis:
      pool:
        max-active: 16
```
**lettcus与jedis区别**
- jedis连接Redis服务器是直连模式，当多线程模式下使用jedis会存在线程安全问题，解决方案**可以通过配置连接池使每个连接专用**，这样整体性能就大受影响
- lettcus基于Netty框架进行与Redis服务器连接，底层设计中采用StatefulRedisConnection。 StatefulRedisConnection自身是线程安全的，可以保障并发访问安全问题，所以一个连接可以被多线程复用。当然lettcus也支持多连接实例一起工作

**总结**
1. springboot整合redis提供了StringRedisTemplate对象，以字符串的数据格式操作redis
2. 如果需要切换redis客户端实现技术，可以通过配置的形式进行

#### SpringBoot整合MongoDB
​使用Redis技术可以有效的提高数据访问速度，但==由于Redis的数据格式单一性，无法操作结构化数据，当操作对象型的数据时，Redis就显得捉襟见肘==。在**保障访问速度**的情况下，如果想操作结构化数据，看来Redis无法满足要求了，此时需要使用全新的数据存储结束来解决此问题，本节讲解springboot如何整合MongoDB技术。

​MongoDB是一个开源、高性能、**无模式的文档型数据库**，它是NoSQL数据库产品中的一种，是==最像关系型数据库的非关系型数据库==。

​上述描述中几个词，其中对于我们最陌生的词是无模式的。什么叫无模式呢？**简单说就是作为一款数据库，没有固定的数据存储结构**，第一条数据可能有A、B、C一共3个字段，第二条数据可能有D、E、F也是3个字段，第三条数据可能是A、C、E3个字段，也就是说数据的结构不固定，这就是无模式。有人会说这有什么用啊？**灵活，随时变更，不受约束**。基于上述特点，MongoDB的应用面也会产生一些变化。以下列出了一些可以使用MongoDB作为数据存储的场景，但是并不是必须使用MongoDB的场景：
- 淘宝用户数据
	- 存储位置：数据库
	- 特征：永久性存储，修改频度极低
- 游戏装备数据、游戏道具数据
	- 存储位置：数据库、Mongodb
	- 特征：永久性存储与临时存储相结合、修改频度较高
- 直播数据、打赏数据、粉丝数据
	- 存储位置：数据库、Mongodb
	- 特征：永久性存储与临时存储相结合，修改频度极高
- 物联网数据
	- 存储位置：Mongodb
	- 特征：临时存储，修改频度飞速

​快速了解一下MongoDB，下面直接开始我们的学习，老规矩，先安装，再操作，最后说整合

##### 安装
​windows版安装包下载地址：https://www.mongodb.com/try/download

​下载的安装包也有两种形式，一种是一键安装的msi文件，还有一种是解压缩就能使用的zip文件，哪种形式都行，本课程采用解压缩zip文件进行安装。

​解压缩完毕后会得到如下文件，其中bin目录包含了所有mongodb的可执行命令
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251122968.png)
==​mongodb在运行时需要指定一个数据存储的目录，所以创建一个数据存储目录，通常放置在安装目录中==，此处创建data的目录用来存储数据，具体如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251123346.png)
​如果在安装的过程中出现了如下警告信息，就是告诉你，你当前的操作系统缺少了一些系统文件，这个不用担心。
![400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251123364.png)
​根据下列方案即可解决，在浏览器中搜索提示缺少的名称对应的文件，并下载，将下载的文件拷贝到windows安装目录的system32目录下，然后在命令行中执行regsvr32命令注册此文件。根据下载的文件名不同，执行命令前更改对应名称。
```CMD
regsvr32 vcruntime140_1.dll
```
**启动服务器**
```CMD
mongod --dbpath=..\data\db
```
​启动服务器时需要指定数据存储位置，通过参数--dbpath进行设置，可以根据需要自行设置数据存储路径。默认服务端口27017。
**启动客户端**
```CMD
mongo --host=127.0.0.1 --port=27017
```
##### 基本操作
​MongoDB虽然是一款数据库，但它的操作并不是使用SQL语句进行的，因此操作方式各位小伙伴可能比较陌生，好在有一些类似于Navicat的数据库客户端软件，能够便捷的操作MongoDB，先安装一个客户端，再来操作MongoDB。

​同类型的软件较多，本次安装软件Robo3t，Robot3t是一款绿色软件，无需安装，解压缩即可。解压缩完毕后进入安装目录双击robot3t.exe即可使用。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251123471.png)
​打开软件首先要连接MongoDB服务器，选择【File】菜单，选择【Connect...】
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251123192.png)
​进入连接管理界面后，选择左上角的【Create】链接，创建新的连接设置
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251124609.png)
​如果输入设置值即可连接（默认不修改即可连接本机27017端口）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251124561.png)
​连接成功后，在命令输入区域输入命令即可操作MongoDB。
- ​创建数据库：在左侧菜单中使用右键创建，输入数据库名称即可
- ​创建集合：在Collections上使用右键创建，输入集合名称即可，集合等同于数据库中的表的作用

新增文档：（文档是一种类似json格式的数据，初学者可以先把数据理解为就是json数据）	
```CMD
db.集合名称.insert/save/insertOne(文档)
```
​删除文档：
```CMD
db.集合名称.remove(条件)
```
​修改文档：
```cmd
db.集合名称.update(条件，{操作种类:{文档}})
```
​查询文档：
```CMD
基础查询
查询全部：   db.集合.find();
查第一条：   db.集合.findOne()
查询指定数量文档：	db.集合.find().limit(10)	//查10条文档
跳过指定数量文档：	db.集合.find().skip(20)	//跳过20条文档
统计：	  	db.集合.count()
排序：db.集合.sort({age:1})//按age升序排序
投影：db.集合名称.find(条件,{name:1,age:1}) //仅保留name与age域

条件查询
基本格式：	db.集合.find({条件})
模糊查询：	db.集合.find({域名:/正则表达式/})  //等同SQL中的like，比like强大，可以执行正则所有规则
条件比较运算：   db.集合.find({域名:{$gt:值}})//等同SQL中的数值比较操作，例如：name>18
包含查询：	db.集合.find({域名:{$in:[值1，值2]}})//等同于SQL中的in
条件连接查询：   db.集合.find({$and:[{条件1},{条件2}]})	   //等同于SQL中的and、or
```
​有关MongoDB的基础操作就普及到这里，需要全面掌握MongoDB技术，请参看相关教程学习。

##### 整合
​使用springboot整合MongDB该如何进行呢？其实springboot为什么使用的开发者这么多，就是因为他的套路几乎完全一样。导入坐标，做配置，使用API接口操作。整合Redis如此，整合MongoDB同样如此。
- ​第一，先导入对应技术的整合starter坐标
- ​第二，配置必要信息
- ​第三，使用提供的API操作即可

​下面就开始springboot整合MongoDB，操作步骤如下：
**步骤①**：导入springboot整合MongoDB的starter坐标。
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-mongodb</artifactId>
</dependency>
```
​上述坐标也可在创建模块时通过勾选的形式进行选择，同样归属NoSQL分类中
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251124438.png)
**步骤②**：进行基础配置
```yaml
spring:
  data:
    mongodb:
      uri: mongodb://localhost/itheima
```
​==操作MongoDB需要的配置与操作redis一样，最基本的信息都是操作哪一台服务器==，区别就是连接的服务器IP地址和端口不同，书写格式不同而已。

**步骤③**：使用springboot整合MongoDB的专用客户端接口MongoTemplate来进行操作
```java
@SpringBootTest
class Springboot17MongodbApplicationTests {
    @Autowired
    private MongoTemplate mongoTemplate;
    @Test
    void contextLoads() {
        Book book = new Book();
        book.setId(2);
        book.setName("springboot2");
        book.setType("springboot2");
        book.setDescription("springboot2");
        mongoTemplate.save(book);
    }
    @Test
    void find(){
        List<Book> all = mongoTemplate.findAll(Book.class);
        System.out.println(all);
    }
}
```
​整合工作到这里就做完了，感觉既熟悉也陌生。熟悉的是这个套路，三板斧，就这三招，导坐标做配置用API操作，陌生的是这个技术，里面具体的操作API可能会不熟悉，有关springboot整合MongoDB我们就讲到这里。有兴趣可以继续学习MongoDB的操作，然后再来这里通过编程的形式操作MongoDB。

**总结**：springboot整合MongoDB步骤
1. 导入springboot整合MongoDB的starter坐标
2. 进行基础配置
3. 使用springboot整合MongoDB的专用客户端接口MongoTemplate操作

#### SpringBoot整合ES
​NoSQL解决方案已经讲完了两种技术的整合了，**Redis可以使用内存加载数据并实现数据快速访问**，**MongoDB可以在内存中存储类似对象的数据并实现数据的快速访问**，在企业级开发中对于速度的追求是永无止境的。下面要讲的内容也是一款NoSQL解决方案，只不过**他的作用不是为了直接加速数据的读写，而是加速数据的查询的**，叫做ES技术。

​ES（Elasticsearch）是一个分布式全文搜索引擎，重点是全文搜索。

​那什么是全文搜索呢？比如用户要买一本书，以Java为关键字进行搜索，不管是书名中还是书的介绍中，甚至是书的作者名字，只要包含java就作为查询结果返回给用户查看，上述过程就使用了全文搜索技术。搜索的条件不再是仅用于对某一个字段进行比对，而是在一条数据中使用搜索条件去比对更多的字段，只要能匹配上就列入查询结果，这就是全文搜索的目的。而ES技术就是一种可以实现上述效果的技术。

​要实现全文搜索的效果，不可能使用数据库中like操作去进行比对，这种效率太低了。ES设计了一种全新的思想，来实现全文搜索。具体操作过程如下：
1. 将被查询的字段的数据全部文本信息进行查分，分成若干个词
   - 例如“中华人民共和国”就会被拆分成三个词，分别是“中华”、“人民”、“共和国”，此过程有专业术语叫做分词。分词的策略不同，分出的效果不一样，不同的分词策略称为分词器。
2. 将分词得到的结果存储起来，对应每条数据的id
   - 例如id为1的数据中名称这一项的值是“中华人民共和国”，那么分词结束后，就会出现“中华”对应id为1，“人民”对应id为1，“共和国”对应id为1
   - 例如id为2的数据中名称这一项的值是“人民代表大会“，那么分词结束后，就会出现“人民”对应id为2，“代表”对应id为2，“大会”对应id为2
   - 此时就会出现如下对应结果，按照上述形式可以对所有文档进行分词。需要注意分词的过程不是仅对一个字段进行，而是对每一个参与查询的字段都执行，最终结果汇总到一个表格中

     | 分词结果关键字 | 对应id |
     | -------------- | ------ |
     | 中华           | 1      |
     | 人民           | 1,2    |
     | 共和国         | 1      |
     | 代表           | 2      |
     | 大会           | 2      |

3. 当进行查询时，如果输入“人民”作为查询条件，可以通过上述表格数据进行比对，得到id值1,2，然后根据id值就可以得到查询的结果数据了。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303142046948.png)

​上述过程中分词结果关键字内容每一个都不相同，作用有点类似于数据库中的索引，是用来加速数据查询的。但数据库中的索引是对某一个字段进行添加索引，而==这里的分词结果关键字不是一个完整的字段值，只是一个字段中的其中的一部分内容==。并且索引使用时是根据索引内容查找整条数据，==全文搜索中的分词结果关键字查询后得到的并不是整条的数据，而是数据的id==，要想获得具体数据还要再次查询，因此这里为这种分词结果关键字起了一个全新的名称，叫做**倒排索引**。

​通过上述内容的学习，发现使用ES其实准备工作还是挺多的，必须先建立文档的倒排索引，然后才能继续使用。快速了解一下ES的工作原理，下面直接开始我们的学习，老规矩，先安装，再操作，最后说整合。

##### 安装
​windows版安装包下载地址：[https://](https://www.elastic.co/cn/downloads/elasticsearch)[www.elastic.co/cn/downloads/elasticsearch](https://www.elastic.co/cn/downloads/elasticsearch)

​下载的安装包是解压缩就能使用的zip文件，解压缩完毕后会得到如下文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251124476.png)
- bin目录：包含所有的可执行命令
- config目录：包含ES服务器使用的配置文件
- jdk目录：此目录中包含了一个完整的jdk工具包，版本17，当ES升级时，使用最新版本的jdk确保不会出现版本支持性不足的问题
- lib目录：包含ES运行的依赖jar文件
- logs目录：包含ES运行后产生的所有日志文件
- modules目录：包含ES软件中所有的功能模块，也是一个一个的jar包。和jar目录不同，jar目录是ES运行期间依赖的jar包，modules是ES软件自己的功能jar包
- plugins目录：包含ES软件安装的插件，默认为空

**启动服务器**
```CMD
elasticsearch.bat
```
​双击elasticsearch.bat文件即可启动ES服务器，默认服务端口9200。通过浏览器访问http://localhost:9200看到如下信息视为ES服务器正常启动
```CMD
{
  "name" : "CZBK-**********",
  "cluster_name" : "elasticsearch",
  "cluster_uuid" : "j137DSswTPG8U4Yb-0T1Mg",
  "version" : {
    "number" : "7.16.2",
    "build_flavor" : "default",
    "build_type" : "zip",
    "build_hash" : "2b937c44140b6559905130a8650c64dbd0879cfb",
    "build_date" : "2021-12-18T19:42:46.604893745Z",
    "build_snapshot" : false,
    "lucene_version" : "8.10.1",
    "minimum_wire_compatibility_version" : "6.8.0",
    "minimum_index_compatibility_version" : "6.0.0-beta1"
  },
  "tagline" : "You Know, for Search"
}
```
##### 基本操作
​ES中保存有我们要查询的数据，只不过格式和数据库存储数据格式不同而已。==在ES中我们要先创建倒排索引，这个索引的功能又点类似于数据库的表，然后将数据添加到倒排索引中，添加的数据称为文档==。所以要进行ES的操作要**先创建索引，再添加文档，这样才能进行后续的查询操作**。

​要操作ES可以通过Rest风格的请求来进行，也就是说==发送一个请求就可以执行一个操作==。比如新建索引，删除索引这些操作都可以使用发送请求的形式来进行。
- 创建索引，books是索引名称，下同
  ```CMD
  PUT请求http://localhost:9200/books
  ```
  发送请求后，看到如下信息即索引创建成功
  ```json
  {
      "acknowledged": true,
      "shards_acknowledged": true,
      "index": "books"
  }
  ```
  重复创建已经存在的索引会出现错误信息，reason属性中描述错误原因
  ```json
  {
      "error": {
          "root_cause": [
              {
                  "type": "resource_already_exists_exception",
                  "reason": "index [books/VgC_XMVAQmedaiBNSgO2-w] already exists",
                  "index_uuid": "VgC_XMVAQmedaiBNSgO2-w",
                  "index": "books"
              }
          ],
          "type": "resource_already_exists_exception",
          "reason": "index [books/VgC_XMVAQmedaiBNSgO2-w] already exists",	# books索引已经存在
          "index_uuid": "VgC_XMVAQmedaiBNSgO2-w",
          "index": "book"
      },
      "status": 400
  }
  ```
- 查询索引
  ```CMD
  GET请求http://localhost:9200/books
  ```
  查询索引得到索引相关信息，如下
  ```json
  {
      "book": {
          "aliases": {},
          "mappings": {},
          "settings": {
              "index": {
                  "routing": {
                      "allocation": {
                          "include": {
                              "_tier_preference": "data_content"
                          }
                      }
                  },
                  "number_of_shards": "1",
                  "provided_name": "books",
                  "creation_date": "1645768584849",
                  "number_of_replicas": "1",
                  "uuid": "VgC_XMVAQmedaiBNSgO2-w",
                  "version": {
                      "created": "7160299"
                  }
              }
          }
      }
  }
  ```
  如果查询了不存在的索引，会返回错误信息，例如查询名称为book的索引后信息如下
  ```json
  {
      "error": {
          "root_cause": [
              {
                  "type": "index_not_found_exception",
                  "reason": "no such index [book]",
                  "resource.type": "index_or_alias",
                  "resource.id": "book",
                  "index_uuid": "_na_",
                  "index": "book"
              }
          ],
          "type": "index_not_found_exception",
          "reason": "no such index [book]",# 没有book索引
          "resource.type": "index_or_alias",
          "resource.id": "book",
          "index_uuid": "_na_",
          "index": "book"
      },
      "status": 404
  }
  ```
- 删除索引
  ```CMD
  DELETE请求	http://localhost:9200/books
  ```
  删除所有后，给出删除结果
  ```json
  {
      "acknowledged": true
  }
  ```
  如果重复删除，会给出错误信息，同样在reason属性中描述具体的错误原因
  ```JSON
  {
      "error": {
          "root_cause": [
              {
                  "type": "index_not_found_exception",
                  "reason": "no such index [books]",
                  "resource.type": "index_or_alias",
                  "resource.id": "book",
                  "index_uuid": "_na_",
                  "index": "book"
              }
          ],
          "type": "index_not_found_exception",
          "reason": "no such index [books]",# 没有books索引
          "resource.type": "index_or_alias",
          "resource.id": "book",
          "index_uuid": "_na_",
          "index": "book"
      },
      "status": 404
  }
  ```
- 创建索引并指定分词器
  **​前面创建的索引是未指定分词器的，可以在创建索引时添加请求参数，设置分词器**。目前国内较为流行的分词器是IK分词器，使用前先在下对应的分词器，然后使用。IK分词器下载地址：https://github.com/medcl/elasticsearch-analysis-ik/releases
  ​分词器下载后解压到ES安装目录的plugins目录中即可，安装分词器后需要重新启动ES服务器。使用IK分词器创建索引格式：
  ```json
  PUT请求http://localhost:9200/books
  
  请求参数如下（注意是json格式的参数）
  {
      "mappings":{	#定义mappings属性，替换创建索引时对应的mappings属性
          "properties":{#定义索引中包含的属性设置
              "id":{	#设置索引中包含id属性
                  "type":"keyword"	#当前属性可以被直接搜索
              },
              "name":{#设置索引中包含name属性
                  "type":"text",              #当前属性是文本信息，参与分词  
                  "analyzer":"ik_max_word",   #使用IK分词器进行分词             
                  "copy_to":"all"#分词结果拷贝到all属性中
              },
              "type":{
                  "type":"keyword"
              },
              "description":{
                  "type":"text",	                
                  "analyzer":"ik_max_word",                
                  "copy_to":"all"
              },
              "all":{	#定义属性，用来描述多个字段的分词结果集合，当前属性可以参与查询
                  "type":"text",	                
                  "analyzer":"ik_max_word"
              }
          }
      }
  }
  ```
  ​创建完毕后返回结果和不使用分词器创建索引的结果是一样的，此时可以通过查看索引信息，观察到添加的请求参数mappings已经进入到了索引属性中
  ```json
  {
      "books": {
          "aliases": {},
          "mappings": {#mappings属性已经被替换
              "properties": {
                  "all": {
                      "type": "text",
                      "analyzer": "ik_max_word"
                  },
                  "description": {
                      "type": "text",
                      "copy_to": [
                          "all"
                      ],
                      "analyzer": "ik_max_word"
                  },
                  "id": {
                      "type": "keyword"
                  },
                  "name": {
                      "type": "text",
                      "copy_to": [
                          "all"
                      ],
                      "analyzer": "ik_max_word"
                  },
                  "type": {
                      "type": "keyword"
                  }
              }
          },
          "settings": {
              "index": {
                  "routing": {
                      "allocation": {
                          "include": {
                              "_tier_preference": "data_content"
                          }
                      }
                  },
                  "number_of_shards": "1",
                  "provided_name": "books",
                  "creation_date": "1645769809521",
                  "number_of_replicas": "1",
                  "uuid": "DohYKvr_SZO4KRGmbZYmTQ",
                  "version": {
                      "created": "7160299"
                  }
              }
          }
      }
  }
  ```

目前我们已经有了索引了，但是索引中还没有数据，所以要先添加数据，ES中称数据为文档，下面进行文档操作。
- 添加文档，有三种方式
  ```json
  POST请求	http://localhost:9200/books/_doc#使用系统生成id
  POST请求	http://localhost:9200/books/_create/1	#使用指定id
  POST请求	http://localhost:9200/books/_doc/1#使用指定id，不存在创建，存在更新（版本递增）
  
  文档通过请求参数传递，数据格式json
  {
      "name":"springboot",
      "type":"springboot",
      "description":"springboot"
  }  
  ```
- 查询文档
  ```json
  GET请求	http://localhost:9200/books/_doc/1 #查询单个文档 
  GET请求	http://localhost:9200/books/_search #查询全部文档
  ```
- 条件查询
  ```json
  GET请求	http://localhost:9200/books/_search?q=name:springboot	# q=查询属性名:查询属性值
  ```
- 删除文档
  ```json
  DELETE请求	http://localhost:9200/books/_doc/1
  ```
- 修改文档（全量更新）
  ```json
  PUT请求	http://localhost:9200/books/_doc/1
  
  文档通过请求参数传递，数据格式json
  {
      "name":"springboot",
      "type":"springboot",
      "description":"springboot"
  }
  ```
- 修改文档（部分更新）
  ```json
  POST请求	http://localhost:9200/books/_update/1
  
  文档通过请求参数传递，数据格式json
  {	
      "doc":{#部分更新并不是对原始文档进行更新，而是对原始文档对象中的doc属性中的指定属性更新
          "name":"springboot"#仅更新提供的属性值，未提供的属性值不参与更新操作
      }
  }
  ```

##### 整合
​使用springboot整合ES该如何进行呢？老规矩，导入坐标，做配置，使用API接口操作。整合Redis如此，整合MongoDB如此，整合ES依然如此。太没有新意了，其实不是没有新意，这就是springboot的强大之处，所有东西都做成相同规则，对开发者来说非常友好。

​下面就开始springboot整合ES，操作步骤如下：
**步骤①**：导入springboot整合ES的starter坐标
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-elasticsearch</artifactId>
</dependency>
```
**步骤②**：进行基础配置
```yaml
spring:
  elasticsearch:
    rest:
      uris: http://localhost:9200
```
​配置ES服务器地址，端口9200

**步骤③**：使用springboot整合ES的专用客户端接口ElasticsearchRestTemplate来进行操作
```java
@SpringBootTest
class Springboot18EsApplicationTests {
    @Autowired
    private ElasticsearchRestTemplate template;
}
```
​上述操作形式是ES早期的操作方式，使用的客户端被称为Low Level Client，这种客户端操作方式性能方面略显不足，于是==ES开发了全新的客户端操作方式，称为High Level Client。高级别客户端与ES版本同步更新，但springboot最初整合ES的时候使用的是低级别客户端==，所以企业开发需要更换成高级别的客户端模式。

​下面使用高级别客户端方式进行springboot整合ES，操作步骤如下：
**步骤①**：导入springboot整合ES高级别客户端的坐标，此种形式目前没有对应的starter
```xml
<dependency>
    <groupId>org.elasticsearch.client</groupId>
    <artifactId>elasticsearch-rest-high-level-client</artifactId>
</dependency>
```
**步骤②**：使用编程形式设置连接的ES服务器，并获取客户端对象
```java
@SpringBootTest
class Springboot18EsApplicationTests {
    private RestHighLevelClient client;
      @Test
      void testCreateClient() throws IOException {
          HttpHost host = HttpHost.create("http://localhost:9200");
          RestClientBuilder builder = RestClient.builder(host);
          client = new RestHighLevelClient(builder);
  
          client.close();
      }
}
```
​配置ES服务器地址与端口9200，记得客户端使用完毕需要手工关闭。==由于当前客户端是手工维护的，因此不能通过自动装配的形式加载对象==。

**步骤③**：使用客户端对象操作ES，例如创建索引
```java
@SpringBootTest
class Springboot18EsApplicationTests {
    private RestHighLevelClient client;
      @Test
      void testCreateIndex() throws IOException {
          HttpHost host = HttpHost.create("http://localhost:9200");
          RestClientBuilder builder = RestClient.builder(host);
          client = new RestHighLevelClient(builder);
          // 客户端操作
          CreateIndexRequest request = new CreateIndexRequest("books");
          // 获取操作索引的客户端对象，调用创建索引操作
          client.indices().create(request, RequestOptions.DEFAULT); 
          
          client.close();
      }
}
```
**​高级别客户端操作是通过发送请求的方式完成所有操作的**，ES针对各种不同的操作，设定了各式各样的请求对象，上例中创建索引的对象是CreateIndexRequest，其他操作也会有自己专用的Request对象。

​当前操作我们发现，无论进行ES何种操作，第一步永远是获取RestHighLevelClient对象，最后一步永远是关闭该对象的连接。在测试中，可以使用测试类的特性去帮助开发者一次性完成上述操作，但在业务书写时，还需要自行管理。==将上述代码格式转换成使用测试类的初始化方法和销毁方法，进行客户端对象的维护==。
```JAVA
@SpringBootTest
class Springboot18EsApplicationTests {
    @BeforeEach//在测试类中每个操作运行前运行的方法
    void setUp() {
        HttpHost host = HttpHost.create("http://localhost:9200");
        RestClientBuilder builder = RestClient.builder(host);
        client = new RestHighLevelClient(builder);
    }

    @AfterEach//在测试类中每个操作运行后运行的方法
    void tearDown() throws IOException {
        client.close();
    }

    private RestHighLevelClient client;

    @Test
    void testCreateIndex() throws IOException {
        CreateIndexRequest request = new CreateIndexRequest("books");
        client.indices().create(request, RequestOptions.DEFAULT);
    }
}
```
​现在的书写简化了很多，也更合理。下面使用上述模式，将所有的ES操作执行一遍，测试结果。
> RestHightLevelClient 在 ES8.x 已被弃用
> 高版本的springboot已经整合了这个了 可以用autowired注入

**创建索引（IK分词器）**：
```java
@Test
void testCreateIndexByIK() throws IOException {
    CreateIndexRequest request = new CreateIndexRequest("books");
    String json = "{\n" +
            "    \"mappings\":{\n" +
            "        \"properties\":{\n" +
            "            \"id\":{\n" +
            "                \"type\":\"keyword\"\n" +
            "            },\n" +
            "            \"name\":{\n" +
            "                \"type\":\"text\",\n" +
            "                \"analyzer\":\"ik_max_word\",\n" +
            "                \"copy_to\":\"all\"\n" +
            "            },\n" +
            "            \"type\":{\n" +
            "                \"type\":\"keyword\"\n" +
            "            },\n" +
            "            \"description\":{\n" +
            "                \"type\":\"text\",\n" +
            "                \"analyzer\":\"ik_max_word\",\n" +
            "                \"copy_to\":\"all\"\n" +
            "            },\n" +
            "            \"all\":{\n" +
            "                \"type\":\"text\",\n" +
            "                \"analyzer\":\"ik_max_word\"\n" +
            "            }\n" +
            "        }\n" +
            "    }\n" +
            "}";
    //设置请求中的参数
    request.source(json, XContentType.JSON);
    client.indices().create(request, RequestOptions.DEFAULT);
}
```
​IK分词器是通过请求参数的形式进行设置的，设置请求参数使用request对象中的source方法进行设置，至于参数是什么，取决于你的操作种类。当请求中需要参数时，均可使用当前形式进行参数设置。	

**添加文档**：
```java
@Test
//添加文档
void testCreateDoc() throws IOException {
    Book book = bookDao.selectById(1);
    IndexRequest request = new IndexRequest("books").id(book.getId().toString());
    String json = JSON.toJSONString(book);
    request.source(json,XContentType.JSON);
    client.index(request,RequestOptions.DEFAULT);
}
```
​添加文档使用的请求对象是IndexRequest，与创建索引使用的请求对象不同。	

**批量添加文档**：
```java
@Test
//批量添加文档
void testCreateDocAll() throws IOException {
    List<Book> bookList = bookDao.selectList(null);
    BulkRequest bulk = new BulkRequest();
    for (Book book : bookList) {
        IndexRequest request = new IndexRequest("books").id(book.getId().toString());
        String json = JSON.toJSONString(book);
        request.source(json,XContentType.JSON);
        bulk.add(request);
    }
    client.bulk(bulk,RequestOptions.DEFAULT);
}
```

​批量做时，先创建一个BulkRequest的对象，可以将该对象理解为是一个保存request对象的容器，将所有的请求都初始化好后，添加到BulkRequest对象中，再使用BulkRequest对象的bulk方法，一次性执行完毕。

**按id查询文档**：

```java
@Test
//按id查询
void testGet() throws IOException {
    GetRequest request = new GetRequest("books","1");
    GetResponse response = client.get(request, RequestOptions.DEFAULT);
    String json = response.getSourceAsString();
    System.out.println(json);
}
```
​根据id查询文档使用的请求对象是GetRequest。

**按条件查询文档**：
```java
@Test
//按条件查询
void testSearch() throws IOException {
    SearchRequest request = new SearchRequest("books");

    SearchSourceBuilder builder = new SearchSourceBuilder();
    builder.query(QueryBuilders.termQuery("all","spring"));
    request.source(builder);

    SearchResponse response = client.search(request, RequestOptions.DEFAULT);
    SearchHits hits = response.getHits();
    for (SearchHit hit : hits) {
        String source = hit.getSourceAsString();
        //System.out.println(source);
        Book book = JSON.parseObject(source, Book.class);
        System.out.println(book);
    }
}
```
​按条件查询文档使用的请求对象是SearchRequest，查询时调用SearchRequest对象的termQuery方法，需要给出查询属性名，此处支持使用合并字段，也就是前面定义索引属性时添加的all属性。

​springboot整合ES的操作到这里就说完了，与前期进行springboot整合redis和mongodb的差别还是蛮大的，主要原始就是我们没有使用springboot整合ES的客户端对象。至于操作，由于ES操作种类过多，所以显得操作略微有点复杂。有关springboot整合ES就先学习到这里吧。

**总结**
1. springboot整合ES步骤
   1. 导入springboot整合ES的High Level Client坐标
   2. 手工管理客户端对象，包括初始化和关闭操作
   3. 使用High Level Client根据操作的种类不同，选择不同的Request对象完成对应操作

---
## KF-5.整合第三方技术
​通过第四章的学习，我们领略到了springboot在整合第三方技术时强大的一致性，在第五章中我们要使用springboot继续整合各种各样的第三方技术，通过本章的学习，可以将之前学习的springboot整合第三方技术的思想贯彻到底，还是那三板斧。导坐标、做配置、调API。

​springboot能够整合的技术实在是太多了，可以说是万物皆可整。本章将从企业级开发中常用的一些技术作为出发点，对各种各样的技术进行整合。
### KF-5-1.缓存
​企业级应用主要作用是信息处理，当需要读取数据时，由于受限于数据库的访问效率，导致整体系统性能偏低。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251125958.png)
应用程序直接与数据库打交道，访问效率低

​为了改善上述现象，开发者通常会在应用程序与数据库之间、建立一种临时的数据存储机制，该区域中的数据在内存中保存，读写速度较快，可以有效解决数据库访问效率低下的问题。这一块临时存储数据的区域就是缓存。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251125726.png)

使用缓存后，应用程序与缓存打交道，缓存与数据库打交道，数据访问效率提高

​缓存是什么？缓存是一种介于数据永久存储介质与应用程序之间的数据临时存储介质，使用缓存可以有效的减少低速数据读取过程的次数（例如磁盘IO），提高系统性能。此外，==缓存不仅可以用于提高永久性存储介质的数据读取效率，还可以提供临时的数据（比如验证码）存储空间==。而springboot提供了对市面上几乎所有的缓存技术进行整合的方案，下面就一起开启springboot整合缓存之旅。
#### SpringBoot内置缓存解决方案
​springboot技术提供有内置的缓存解决方案，可以帮助开发者快速开启缓存技术，并使用缓存技术进行数据的快速操作，例如读取缓存数据和写入数据到缓存。缓存使用
- 启用缓存
- 设置进入缓存的数据
- 设置读取缓存的数据

**步骤①**：导入springboot提供的缓存技术对应的starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-cache</artifactId>
</dependency>
```
**步骤②**：启用缓存，在引导类上方标注注解@EnableCaching，配置springboot程序中可以使用缓存
```java
@SpringBootApplication
//开启缓存功能
@EnableCaching
public class Springboot19CacheApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot19CacheApplication.class, args);
    }
}
```
**步骤③**：设置操作的数据是否使用缓存
> Book要实现Serializable接口
> 不是bug，这个修改是要在你的执行修改的地方加注解、来改缓存中的数据
```java
@Service
public class BookServiceImpl implements BookService {
    @Autowired
    private BookDao bookDao;

    @Cacheable(value="cacheSpace", key="#id")
    public Book getById(Integer id) {
        return bookDao.selectById(id);
    }
}
```
​在业务方法上面使用注解@Cacheable，声明当前方法的返回值放入缓存中，其中要==指定缓存的存储位置==，以及==缓存中保存当前方法返回值对应的名称==。上例中value属性描述缓存的存储位置，可以理解为是一个存储空间名，key属性描述了缓存中保存数据的名称，使用#id读取形参中的id值作为缓存名称。

​使用@Cacheable注解后，==执行当前操作，如果发现对应名称在缓存中没有数据，就正常读取数据，然后放入缓存==；如果对应名称在缓存中有数据，就终止当前业务方法执行，直接返回缓存中的数据。

SpringBoot提供的缓存技术除了提供默认的缓存方案，还可以**对其他缓存技术进行整合，统一接口**，方便缓存技术的开发与管理
- Generic
- JCache
- Ehcache
- Hazelcast
- Infinispan
- Couchbase
- Redis
- Caffeine
- Simple（默认）
- memcached

#### 手机验证码案例
​为了便于下面演示各种各样的缓存技术，我们创建一个手机验证码的案例环境，模拟使用缓存保存手机验证码的过程。​手机验证码案例需求如下：
- 输入手机号获取验证码，组织文档以短信形式发送给用户（页面模拟）
- 输入手机号和验证码验证结果

​为了描述上述操作，我们制作两个表现层接口，一个用来**模拟发送短信的过程**，其实就是根据用户提供的手机号生成一个验证码，然后**放入缓存**，另一个用来模拟验证码校验的过程，其实就是**使用传入的手机号和验证码进行匹配，并返回最终匹配结果**。需求分析
- 提供controller，传入手机号，业务层通过手机号计算出独有的6位验证码数据，存入缓存后返回此数据
- 提供controller，传入手机号与验证码，业务层通过手机号从缓存中读取验证码与输入验证码进行比对，返回比对结果

下面直接制作本案例的模拟代码，先以上例中springboot提供的内置缓存技术来完成当前案例的制作。
> 过程没有数据层参与

**步骤①**：导入springboot提供的缓存技术对应的starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-cache</artifactId>
</dependency>
```
**步骤②**：启用缓存，在引导类上方标注注解@EnableCaching配置springboot程序中可以使用缓存
```java
@SpringBootApplication
//开启缓存功能
@EnableCaching
public class Springboot19CacheApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot19CacheApplication.class, args);
    }
}
```
**步骤③**：定义验证码对应的实体类，封装手机号与验证码两个属性
```java
@Data
public class SMSCode {
    private String tele;
    private String code;
}
```
**步骤④**：定义验证码功能的业务层接口与实现类
```java
public interface SMSCodeService {
	/**
	 * 传入手机号获取验证码，存入缓存
	 * @param tele
	 * @return
	 */
    public String sendCodeToSMS(String tele);
    /**
	 * 传入手机号与验证码，校验匹配是否成功
	 * @param smsCode
	 * @return
	 */
    public boolean checkCode(SMSCode smsCode);
}

@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @Autowired
    private CodeUtils codeUtils;

    @CachePut(value = "smsCode", key = "#tele")
    public String sendCodeToSMS(String tele) {
        String code = codeUtils.generator(tele);
        return code;
    }

    public boolean checkCode(SMSCode smsCode) {
        //取出内存中的验证码与传递过来的验证码比对，如果相同，返回true
        String code = smsCode.getCode();
        String cacheCode = codeUtils.get(smsCode.getTele()); // 这里的get方法是自己写的,取缓存中的值,用传过来的电话作为键
        return code.equals(cacheCode);
    }
}
```
==​获取验证码后，当验证码失效时必须重新获取验证码，因此在获取验证码的功能上不能使用@Cacheable注解==，@Cacheable注解是**缓存中没有值则放入值，缓存中有值则取值**。此处的功能仅仅是生成验证码并放入缓存，并不具有从缓存中取值的功能，因此不能使用@Cacheable注解，应该使用**仅具有向缓存中保存数据的功能**，使用@CachePut注解即可。

​对于校验验证码（应该是取出验证码）的功能，建议放入工具类中进行。
**步骤⑤**：定义验证码的生成策略与根据手机号读取验证码的功能
```java
@Component
public class CodeUtils {
    private String [] patch = {"000000","00000","0000","000","00","0",""};

    public String generator(String tele){
        int hash = tele.hashCode();
        int encryption = 20206666;
        long result = hash ^ encryption;
        long nowTime = System.currentTimeMillis();
        result = result ^ nowTime;
        long code = result % 1000000;
        code = code < 0 ? -code : code;
        String codeStr = code + "";
        int len = codeStr.length();
        return patch[len] + codeStr;
    }

    @Cacheable(value = "smsCode",key="#tele")
    public String get(String tele) {
        return null;
    }
}
```
> @Cacheable标记的方法，若第二次以相同参数（key）调用该方法，会直接返回缓存的value而不会执行该方法，哪怕该方法返回的是null也一样，不会影响结果的返回。 Cacheable的功能是查询是否存在缓存 若存在则直接返回缓存值 不存在就返回方法结果值 所以return的null是正确的
> 
> 好像在@Cacheable标注的方法的同一个类里面，调用取缓存值的方法不会触发缓存，在外部调用就可以，比如直接在Controller里面调用就可以触发缓存
> **@Cacheable等方法是基于Spring AOP代理类，内部方法调用是不走代理的**（代理对象自身不做增强），假设CodeUtil的get方法被放到SMSCodeServiceImpl中，则get方法的@Cacheable是不起作用的
> 同一个类中checkCode调用同类中的get方法，没有使用ioc容器中的对象调用方法，注解没有被扫描，**使用ioc容器的方法，注解才会被扫描**
> 经过spring管理的应该是autowire注入才是创建的spring管理的类，在方法内部调用并没有用容器new一个对象，只是方法内部调用

**步骤⑥**：定义验证码功能的web层接口，一个方法用于**提供手机号获取验证码**，一个方法用于**提供手机号和验证码进行校验**
```java
@RestController
@RequestMapping("/sms")
public class SMSCodeController {
    @Autowired
    private SMSCodeService smsCodeService;
    
    @GetMapping
    public String getCode(String tele){
        String code = smsCodeService.sendCodeToSMS(tele);
        return code;
    }
    
    @PostMapping
    public boolean checkCode(SMSCode smsCode){
        return smsCodeService.checkCode(smsCode);
    }
}
```
### SpringBoot整合Ehcache缓存
​手机验证码的案例已经完成了，下面就开始springboot整合各种各样的缓存技术，第一个整合Ehcache技术。Ehcache是一种缓存技术，使用springboot整合Ehcache其实就是变更一下缓存技术的实现方式，话不多说，直接开整

**步骤①**：导入Ehcache的坐标
```xml
<dependency>
    <groupId>net.sf.ehcache</groupId>
    <artifactId>ehcache</artifactId>
</dependency>
```
​此处为什么不是导入Ehcache的starter，而是导入技术坐标呢？其实==springboot整合缓存技术做的是**通用格式**，不管你整合哪种缓存技术，只是实现变化了，操作方式一样==。这也体现出springboot技术的优点，统一同类技术的整合方式。

**步骤②**：配置缓存技术实现使用Ehcache
```yaml
spring:
  cache:
    type: ehcache
    ehcache:
      config: ehcache.xml
```
> 如果指定了config还是报错->找不到ehcache的配置，在文件名前面加classpath:

​配置缓存的类型type为ehcache，此处需要说明一下，当前springboot可以整合的缓存技术中包含有ehcache，所以可以这样书写。其实这个type不可以随便写的，不是随便写一个名称就可以整合的。

​由于ehcache的配置有独立的配置文件格式，因此还需要指定ehcache的配置文件，以便于读取相应配置：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<ehcache xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:noNamespaceSchemaLocation="http://ehcache.org/ehcache.xsd"
         updateCheck="false">
    <diskStore path="D:\ehcache" />

    <!--默认缓存策略 -->
    <!-- external：是否永久存在，设置为true则不会被清除，此时与timeout冲突，通常设置为false-->
    <!-- diskPersistent：是否启用磁盘持久化-->
    <!-- maxElementsInMemory：最大缓存数量-->
    <!-- overflowToDisk：超过最大缓存数量是否持久化到磁盘-->
    <!-- timeToIdleSeconds：最大不活动间隔，设置过长缓存容易溢出，设置过短无效果，可用于记录时效性数据，例如验证码-->
    <!-- timeToLiveSeconds：最大存活时间-->
    <!-- memoryStoreEvictionPolicy：缓存清除策略-->
    <defaultCache
        eternal="false"
        diskPersistent="false"
        maxElementsInMemory="1000"
        overflowToDisk="false"
        timeToIdleSeconds="60"
        timeToLiveSeconds="60"
        memoryStoreEvictionPolicy="LRU" />

    <cache
        name="smsCode"
        eternal="false"
        diskPersistent="false"
        maxElementsInMemory="1000"
        overflowToDisk="false"
        timeToIdleSeconds="10"
        timeToLiveSeconds="10"
        memoryStoreEvictionPolicy="LRU" />
</ehcache>
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303150055015.png)

​注意前面的案例中，设置了数据保存的位置是smsCode（没有这个 `value = "smsCode"` ，就是默认的cache）：
```java
@CachePut(value = "smsCode", key = "#tele")
public String sendCodeToSMS(String tele) {
    String code = codeUtils.generator(tele);
    return code;
}	
```
​这个设定需要保障、ehcache中有一个缓存空间名称叫做smsCode的配置，前后要统一。==在企业开发过程中，通过设置不同名称的cache、来设定不同的缓存策略，应用于不同的缓存数据==。

​到这里springboot整合Ehcache就做完了，可以发现一点，原始代码没有任何修改，仅仅是加了一组配置就可以变更缓存供应商了，这也是springboot提供了统一的缓存操作接口的优势，变更实现并不影响原始代码的书写。

**总结**
1. springboot使用Ehcache作为缓存实现需要导入Ehcache的坐标
2. 修改设置，配置缓存供应商为ehcache，并提供对应的缓存配置文件
#### SpringBoot整合Redis缓存
​上节使用Ehcache替换了springboot内置的缓存技术，其实springboot支持的缓存技术还很多，下面使用redis技术作为缓存解决方案来实现手机验证码案例。

​比对使用Ehcache的过程，加坐标，改缓存实现类型为ehcache，做Ehcache的配置。如果还成redis做缓存呢？一模一样，加坐标，改缓存实现类型为redis，做redis的配置。差别之处只有一点，**redis的配置可以在yml文件中直接进行配置，无需制作独立的配置文件**。

**步骤①**：导入redis的坐标
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```
**步骤②**：配置缓存技术实现使用redis（要启动这个服务）
```yaml
spring:
  redis:
    host: localhost
    port: 6379
  cache:
    type: redis
```
​如果需要对redis作为缓存进行配置，注意：==不是对原始的redis进行配置，而是配置redis作为缓存使用相关的配置==，隶属于spring.cache.redis节点下，注意不要写错位置了。
> 前面整合redis原来是直接用这个技术，现在是想把spring默认的缓存技术换成redis支持。
```yaml
spring:
  redis:
    host: localhost
    port: 6379
  cache:
    type: redis
    redis:
      use-key-prefix: false # 是否使用前缀
      key-prefix: sms_  # 为true时使用这个前缀
      cache-null-values: false # 是否缓存空值
      time-to-live: 10s # 生命周期
```
> 缓存空值是为了防止缓存穿透吧
> 带前缀最大的作用是提高可读性

**总结**
1. springboot使用redis作为缓存实现需要导入redis的坐标
2. 修改设置，配置缓存供应商为redis，并提供对应的缓存配置

#### SpringBoot整合Memcached缓存
​目前我们已经掌握了3种缓存解决方案的配置形式，分别是springboot内置缓存，ehcache和redis，本节研究一下国内比较流行的一款缓存memcached。

​按照之前的套路，其实变更缓存并不繁琐，但springboot并没有支持使用memcached作为其缓存解决方案，也就是说在type属性中没有memcached的配置选项，这里就需要更变一下处理方式了。在整合之前先安装memcached。

**安装**
​windows版安装包下载地址：https://www.runoob.com/memcached/window-install-memcached.html。​下载的安装包是解压缩就能使用的zip文件，解压缩完毕后会得到如下文件：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251126114.png)
​可执行文件只有一个memcached.exe，使用该文件可以将memcached作为系统服务启动，执行此文件时会出现报错信息，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251126079.png)
​此处出现问题的原因是，注册系统服务时需要使用管理员权限，当前账号权限不足导致安装服务失败，切换管理员账号权限启动命令行
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251126322.png)
​然后再次执行安装服务的命令即可，如下：
```CMD
memcached.exe -d install
```
​服务安装完毕后可以使用命令启动和停止服务，如下：
```cmd
memcached.exe -d start# 启动服务
memcached.exe -d stop# 停止服务
```
​也可以在任务管理器中进行服务状态的切换
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251126466.png)
**变更缓存为Memcached**
​由于memcached未被springboot收录为缓存解决方案，因此使用memcached需要通过手工硬编码的方式来使用，于是前面的套路都不适用了，需要自己写了。

​memcached目前提供有三种客户端技术，分别是Memcached Client for Java、SpyMemcached和Xmemcached，其中性能指标各方面最好的客户端是Xmemcached，本次整合就使用这个作为客户端实现技术了。下面开始使用Xmemcached

**步骤①**：导入xmemcached的坐标
```xml
<dependency>
    <groupId>com.googlecode.xmemcached</groupId>
    <artifactId>xmemcached</artifactId>
    <version>2.4.7</version>
</dependency>
```
**步骤②**：配置memcached，制作memcached的配置类
```java
@Configuration
public class XMemcachedConfig {
    @Bean
    public MemcachedClient getMemcachedClient() throws IOException {
        MemcachedClientBuilder memcachedClientBuilder = new XMemcachedClientBuilder("localhost:11211");
        MemcachedClient memcachedClient = memcachedClientBuilder.build();
        return memcachedClient;
    }
}
```
​memcached默认对外服务端口11211。
**步骤③**：使用xmemcached客户端操作缓存，注入MemcachedClient对象
```java
@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @Autowired
    private CodeUtils codeUtils;
    @Autowired
    private MemcachedClient memcachedClient;

    public String sendCodeToSMS(String tele) {
        String code = codeUtils.generator(tele);
        try {
            memcachedClient.set(tele,10,code);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return code;
    }

    public boolean checkCode(SMSCode smsCode) {
        String code = null;
        try {
            code = memcachedClient.get(smsCode.getTele()).toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return smsCode.getCode().equals(code);
    }
}
```
​设置值到缓存中使用set操作，取值使用get操作，其实更符合我们开发者的习惯。

​上述代码中对于服务器的配置，使用硬编码写死到了代码中，将此数据提取出来，做成独立的配置属性。

**定义配置属性**
​以下过程采用前期学习的属性配置方式进行，当前操作有助于理解原理篇中的很多知识。
- 定义配置类，加载必要的配置属性，读取配置文件中memcached节点信息
  ```java
  @Component
  @ConfigurationProperties(prefix = "memcached")
  @Data
  public class XMemcachedProperties {
      private String servers;
      private int poolSize;
      private long opTimeout;
  }
  ```
- 定义memcached节点信息
  ```yaml
  memcached:
    servers: localhost:11211
    poolSize: 10
    opTimeout: 3000
  ```
- 在memcached配置类中加载信息
```java
@Configuration
public class XMemcachedConfig {
    @Autowired
    private XMemcachedProperties props;
    @Bean
    public MemcachedClient getMemcachedClient() throws IOException {
        MemcachedClientBuilder memcachedClientBuilder = new XMemcachedClientBuilder(props.getServers());
        memcachedClientBuilder.setConnectionPoolSize(props.getPoolSize());
        memcachedClientBuilder.setOpTimeout(props.getOpTimeout());
        MemcachedClient memcachedClient = memcachedClientBuilder.build();
        return memcachedClient;
    }
}
```
**总结**
1. memcached安装后，需要启动对应服务才可以对外提供缓存功能，安装memcached服务需要基于windows系统管理员权限
2. 由于springboot没有提供对memcached的缓存整合方案，需要采用手工编码的形式创建xmemcached客户端操作缓存
3. 导入xmemcached坐标后，**创建memcached配置类**，注册MemcachedClient对应的bean，用于操作缓存
4. 初始化MemcachedClient对象所需要使用的属性，**可以通过自定义配置属性类的形式加载**

**思考**
​到这里已经完成了三种缓存的整合，其中redis和mongodb需要安装独立的服务器，连接时需要输入对应的服务器地址，这种是远程缓存，**Ehcache是一个典型的内存级缓存，因为它什么也不用安装**，启动后导入jar包就有缓存功能了。这个时候就要问了，能不能这两种缓存一起用呢？咱们下节再说。

#### SpringBoot整合jetcache缓存
​目前我们使用的缓存都是要么A要么B，能不能AB一起用呢？这一节就解决这个问题。springboot针对缓存的整合仅仅停留在用缓存上面，如果缓存自身不支持同时支持AB一起用，springboot也没办法，所以要想解决AB缓存一起用的问题，就必须找一款缓存能够支持AB两种缓存一起用，有这种缓存吗？还真有，阿里出品，jetcache。

**​jetcache严格意义上来说，并不是一个缓存解决方案，只能说他算是一个缓存框架**，然后把别的缓存放到jetcache中管理，这样就可以支持AB缓存一起用了。并且jetcache参考了springboot整合缓存的思想，整体技术使用方式和springboot的缓存解决方案思想非常类似。下面咱们就先把jetcache用起来，然后再说它里面的一些小的功能。

​做之前要先明确一下，jetcache并不是随便拿两个缓存都能拼到一起去的。目前jetcache支持的缓存方案本地缓存支持两种，远程缓存支持两种，分别如下：
- 本地缓存（Local）
  - LinkedHashMap
  - Caffeine
- 远程缓存（Remote）
  - Redis
  - Tair

​其实也有人问我，为什么jetcache只支持2+2这么4款缓存呢？阿里研发这个技术其实主要是为了满足自身的使用需要。最初肯定只有1+1种，逐步变化成2+2种。下面就以LinkedHashMap+Redis的方案实现本地与远程缓存方案同时使用。

##### 纯远程方案
**步骤①**：导入springboot整合jetcache对应的坐标starter，当前坐标默认使用的远程方案是redis
```xml
<dependency>
    <groupId>com.alicp.jetcache</groupId>
    <artifactId>jetcache-starter-redis</artifactId>
    <version>2.6.2</version>
</dependency>
```
> 引入jetcache依赖启动应用报错的可以换个版本2.4.4的

**步骤②**：**远程**方案基本配置，其中poolConfig是必配项，否则会报错
```yaml
jetcache:
  remote:
    default:
      type: redis
      host: localhost
      port: 6379
      poolConfig:
        maxTotal: 50
```
​**步骤③**：启用缓存，在引导类上方标注注解@EnableCreateCacheAnnotation配置，springboot程序中可以使用注解的形式创建缓存
```java
@SpringBootApplication
//jetcache启用缓存的主开关
@EnableCreateCacheAnnotation
public class Springboot20JetCacheApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot20JetCacheApplication.class, args);
    }
}
```
**步骤④**：创建缓存对象Cache，并**使用注解@CreateCache标记当前缓存的信息**，然后使用Cache对象的API操作缓存，put写缓存，get读缓存。
```java
@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @Autowired
    private CodeUtils codeUtils;
    
    @CreateCache(name="jetCache_",expire = 10,timeUnit = TimeUnit.SECONDS)
    private Cache<String ,String> jetCache;

    public String sendCodeToSMS(String tele) {
        String code = codeUtils.generator(tele);
        jetCache.put(tele,code);
        return code;
    }

    public boolean checkCode(SMSCode smsCode) {
        String code = jetCache.get(smsCode.getTele());
        return smsCode.getCode().equals(code);
    }
}
```
​通过上述jetcache使用远程方案连接redis可以看出，==jetcache操作缓存时的接口操作更符合开发者习惯，使用缓存就先获取缓存对象Cache，放数据进去就是put，取数据出来就是get==，更加简单易懂。并且jetcache操作缓存时，可以为某个缓存对象设置过期时间，将同类型的数据放入缓存中，方便有效周期的管理。

​上述方案中使用的是**配置中定义的default缓存**，其实这个default是个名字，可以随便写，也可以随便加。例如再添加一种缓存解决方案，参照如下配置进行：
```yaml
jetcache:
  remote:
    default:
      type: redis
      host: localhost
      port: 6379
      poolConfig:
        maxTotal: 50
    sms:
      type: redis
      host: localhost
      port: 6379
      poolConfig:
        maxTotal: 50
```
**​如果想使用名称是sms的缓存，需要再创建缓存时指定参数area**，声明使用对应缓存即可（name后一般要加上下划线）
```JAVA
@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @Autowired
    private CodeUtils codeUtils;
    
    @CreateCache(area="sms",name="jetCache_",expire = 10,timeUnit = TimeUnit.SECONDS)
    private Cache<String ,String> jetCache;

    public String sendCodeToSMS(String tele) {
        String code = codeUtils.generator(tele);
        jetCache.put(tele,code);
        return code;
    }

    public boolean checkCode(SMSCode smsCode) {
        String code = jetCache.get(smsCode.getTele());
        return smsCode.getCode().equals(code);
    }
}
```
##### 纯本地方案
​远程方案中，配置中使用remote表示远程，换成local就是本地，只不过类型不一样而已。
> jetcache的版本用2.6.2的包循环依赖，升成2.6.3就正常了，为什么你们是降版本

**步骤①**：导入springboot整合jetcache对应的坐标starter
```xml
<dependency>
    <groupId>com.alicp.jetcache</groupId>
    <artifactId>jetcache-starter-redis</artifactId>
    <version>2.6.2</version>
</dependency>
```
**步骤②**：本地缓存基本配置
```yaml
jetcache:
  local:
    default:
      type: linkedhashmap
      keyConvertor: fastjson
```
​为了加速数据获取时key的匹配速度，**jetcache要求指定key的类型转换器**。简单说就是，如果你给了一个Object作为key的话，我先用key的类型转换器给转换成字符串，然后再保存。等到获取数据时，仍然是先使用给定的Object转换成字符串，然后根据字符串匹配。由于jetcache是阿里的技术，这里推荐key的类型转换器使用阿里的fastjson。

**步骤③**：启用缓存
```java
@SpringBootApplication
//jetcache启用缓存的主开关
@EnableCreateCacheAnnotation
public class Springboot20JetCacheApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot20JetCacheApplication.class, args);
    }
}
```
**步骤④**：创建缓存对象Cache时，**标注当前使用本地缓存**
```java
@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @CreateCache(name="jetCache_",expire = 1000,timeUnit = TimeUnit.SECONDS,cacheType = CacheType.LOCAL)
    private Cache<String ,String> jetCache;

    public String sendCodeToSMS(String tele) {
        String code = codeUtils.generator(tele);
        jetCache.put(tele,code);
        return code;
    }

    public boolean checkCode(SMSCode smsCode) {
        String code = jetCache.get(smsCode.getTele());
        return smsCode.getCode().equals(code);
    }
}
```
​cacheType控制当前缓存使用本地缓存还是远程缓存，配置cacheType=CacheType.LOCAL即使用本地缓存（或者BOTH）。

##### 本地+远程方案
​本地和远程方法都有了，两种方案一起使用如何配置呢？其实就是将两种配置合并到一起就可以了。
```YAML
jetcache:
  local:
    default:
      type: linkedhashmap
      keyConvertor: fastjson
  remote:
    default:
      type: redis
      host: localhost
      port: 6379
      poolConfig:
        maxTotal: 50
    sms:
      type: redis
      host: localhost
      port: 6379
      poolConfig:
        maxTotal: 50
```
​在创建缓存的时候，配置cacheType为BOTH即则本地缓存与远程缓存同时使用。
```java
@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @CreateCache(name="jetCache_",expire = 1000,timeUnit = TimeUnit.SECONDS,cacheType = CacheType.BOTH)
    private Cache<String ,String> jetCache;
}
```
​cacheType如果不进行配置，默认值是REMOTE，即仅使用远程缓存方案。关于jetcache的配置，参考以下信息
| 属性                                                      | 默认值 | 说明                                                         |
| --------------------------------------------------------- | ------ | ------------------------------------------------------------ |
| jetcache.statIntervalMinutes                              | 0      | 统计间隔，0表示不统计                                        |
| jetcache.hiddenPackages                                   | 无     | 自动生成name时，隐藏指定的包名前缀                           |
| jetcache.[local\|remote].${area}.type                     | 无     | 缓存类型，本地支持linkedhashmap、caffeine，远程支持redis、tair |
| jetcache.[local\|remote].${area}.keyConvertor             | 无     | key转换器，当前仅支持fastjson                                |
| jetcache.[local\|remote].${area}.valueEncoder             | java   | 仅remote类型的缓存需要指定，可选java和kryo                   |
| jetcache.[local\|remote].${area}.valueDecoder             | java   | 仅remote类型的缓存需要指定，可选java和kryo                   |
| jetcache.[local\|remote].${area}.limit                    | 100    | 仅local类型的缓存需要指定，缓存实例最大元素数                |
| jetcache.[local\|remote].${area}.expireAfterWriteInMillis | 无穷大 | 默认过期时间，毫秒单位                                       |
| jetcache.local.${area}.expireAfterAccessInMillis          | 0      | 仅local类型的缓存有效，毫秒单位，最大不活动间隔              |
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303151236847.png)

​以上方案仅支持手工控制缓存，但是springcache方案中的方法缓存特别好用，给一个方法添加一个注解，方法就会自动使用缓存。jetcache也提供了对应的功能，即方法缓存。

**方法缓存**
​jetcache提供了方法缓存方案，只不过名称变更了而已。在对应的操作接口上方使用注解@Cached即可

**步骤①**：导入springboot整合jetcache对应的坐标starter
```xml
<dependency>
    <groupId>com.alicp.jetcache</groupId>
    <artifactId>jetcache-starter-redis</artifactId>
    <version>2.6.2</version>
</dependency>
```
**步骤②**：配置缓存
```yaml
jetcache:
  local:
    default:
      type: linkedhashmap
      keyConvertor: fastjson
  remote:
    default:
      type: redis
      host: localhost
      port: 6379
      keyConvertor: fastjson
      valueEncode: java
      valueDecode: java
      poolConfig:
        maxTotal: 50
    sms:
      type: redis
      host: localhost
      port: 6379
      poolConfig:
        maxTotal: 50
```
​由于redis缓存中不支持保存对象，因此需要对redis设置当Object类型数据进入到redis中时如何进行类型转换。需要配置keyConvertor表示key的类型转换方式，同时标注value的转换类型方式，值进入redis时是java类型，标注valueEncode为java，值从redis中读取时转换成java，标注valueDecode为java。

​注意，为了实现Object类型的值进出redis，需要保障进出redis的Object类型的数据必须实现序列化接口。
```JAVA
@Data
public class Book implements Serializable {
    private Integer id;
    private String type;
    private String name;
    private String description;
}
```
**步骤③**：启用缓存时**开启方法缓存功能**，并配置basePackages，说明**在哪些包中开启方法缓存**
```java
@SpringBootApplication
//jetcache启用缓存的主开关
@EnableCreateCacheAnnotation
//开启方法注解缓存
@EnableMethodCache(basePackages = "com.itheima")
public class Springboot20JetCacheApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot20JetCacheApplication.class, args);
    }
}
```
**步骤④**：使用注解@Cached标注当前方法使用缓存
```java
@Service
public class BookServiceImpl implements BookService {
    @Autowired
    private BookDao bookDao;
    
    @Override
    @Cached(name="book_",key="#id",expire = 3600,cacheType = CacheType.REMOTE)
    public Book getById(Integer id) {
        return bookDao.selectById(id);
    }
}
```

##### 远程方案的数据同步
​==由于远程方案中redis保存的数据可以被多个客户端共享，这就存在了数据同步问题==。jetcache提供了3个注解解决此问题，分别在更新、删除操作时同步缓存数据，和读取缓存时定时刷新数据

**更新缓存**
```JAVA
@CacheUpdate(name="book_",key="#book.id",value="#book")
public boolean update(Book book) {
    return bookDao.updateById(book) > 0;
}
```
**删除缓存**
```JAVA
@CacheInvalidate(name="book_",key = "#id")
public boolean delete(Integer id) {
    return bookDao.deleteById(id) > 0;
}
```
**定时刷新缓存**
```JAVA
@Cached(name="book_",key="#id",expire = 3600,cacheType = CacheType.REMOTE)
@CacheRefresh(refresh = 5)
public Book getById(Integer id) {
    return bookDao.selectById(id);
}
```
##### 数据报表
​jetcache还提供有简单的数据报表功能，帮助开发者快速查看缓存命中信息，只需要添加一个配置即可
```yaml
jetcache:
  statIntervalMinutes: 1
```
​设置后，**每1分钟在控制台输出缓存数据命中信息**
```CMD
[DefaultExecutor] c.alicp.jetcache.support.StatInfoLogger  : jetcache stat from 2022-02-28 09:32:15,892 to 2022-02-28 09:33:00,003
cache    |    qps|   rate|   get|    hit|   fail|   expire|   avgLoadTime|   maxLoadTime
---------+-------+-------+------+-------+-------+---------+--------------+--------------
book_    |   0.66| 75.86%|    29|     22|      0|        0|          28.0|           188
---------+-------+-------+------+-------+-------+---------+--------------+--------------
```
**总结**
1. jetcache是一个类似于springcache的缓存解决方案，自身不具有缓存功能，它提供有本地缓存与远程缓存多级共同使用的缓存解决方案
2. jetcache提供的缓存解决方案受限于目前支持的方案，本地缓存支持两种，远程缓存支持两种
3. 注意数据进入远程缓存时的类型转换问题
4. jetcache提供方法缓存，并提供了对应的缓存更新与刷新功能
5. jetcache提供有简单的缓存信息命中报表方便开发者即时监控缓存数据命中情况

**思考**
==​jetcache解决了前期使用缓存方案单一的问题，但仍然不能灵活的选择缓存进行搭配使用==，是否存在一种技术可以灵活的搭配各种各样的缓存使用呢？有，咱们下一节再讲。

#### SpringBoot整合j2cache缓存
​jetcache可以在限定范围内构建多级缓存，但灵活性不足，不能随意搭配缓存，本节介绍一种**可以随意搭配缓存解决方案的缓存整合框架**，j2cache。下面就来讲解如何使用这种缓存框架，以Ehcache与redis整合为例：

**步骤①**：导入j2cache、redis、ehcache坐标
```xml
<dependency>
    <groupId>net.oschina.j2cache</groupId>
    <artifactId>j2cache-core</artifactId>
    <version>2.8.4-release</version>
</dependency>
<dependency>
    <groupId>net.oschina.j2cache</groupId>
    <artifactId>j2cache-spring-boot2-starter</artifactId>
    <version>2.8.0-release</version>
</dependency>
<dependency>
    <groupId>net.sf.ehcache</groupId>
    <artifactId>ehcache</artifactId>
</dependency>
```
​j2cache的starter中默认包含了redis坐标，**官方推荐使用redis作为二级缓存**，因此此处无需导入redis坐标。下面是application.xml：
```yaml
server:
	port: 80
j2cache:
	config-location: j2cache.properties
```
**步骤②**：==配置一级与二级缓存，并配置一二级缓存间数据传递方式==，配置书写在名称为j2cache.properties的文件中。如果使用ehcache还需要单独添加ehcache的配置文件
```yaml
# 1级缓存
j2cache.L1.provider_class = ehcache
ehcache.configXml = ehcache.xml

# 2级缓存
j2cache.L2.provider_class = net.oschina.j2cache.cache.support.redis.SpringRedisProvider
j2cache.L2.config_section = redis # 相当于一个前缀名
redis.hosts = localhost:6379

# 配置1级缓存数据到2级缓存的广播方式：可以使用redis提供的消息订阅模式，也可以使用jgroups多播实现
j2cache.broadcast = net.oschina.j2cache.cache.support.redis.SpringRedisPubSubPolicy

redis.mode = single # 单机缓存
```
此处配置不能乱配置，需要参照官方给出的配置说明进行。例如1级供应商选择ehcache，供应商名称仅仅是一个ehcache，但是==2级供应商选择redis时、要写专用的「Spring整合Redis的供应商类名」SpringRedisProvider==，而且这个名称并不是所有的redis包中能提供的，也不是spring包中提供的。因此==配置j2cache必须参照官方文档配置，而且还要去找专用的整合包，导入对应坐标才可以使用==。

​一级与二级缓存最重要的一个配置就是两者之间的数据沟通方式，此类配置也不是随意配置的，并且**不同的缓存解决方案提供的数据沟通方式差异化很大**，需要查询官方文档进行设置。

**步骤③**：设置使用缓存
```java
@Service
public class SMSCodeServiceImpl implements SMSCodeService {
    @Autowired
    private CodeUtils codeUtils;

    @Autowired
    private CacheChannel cacheChannel;

    public String sendCodeToSMS(String tele) {
        String code = codeUtils.generator(tele);
        cacheChannel.set("sms",tele,code);
        return code;
    }

    public boolean checkCode(SMSCode smsCode) {
        String code = cacheChannel.get("sms",smsCode.getTele()).asString();
        return smsCode.getCode().equals(code);
    }
}
```
​j2cache的使用和jetcache比较类似，但无需开启使用的开关，直接定义缓存对象即可使用，缓存对象名CacheChannel。

​j2cache的使用不复杂，**配置是j2cache的核心，毕竟是一个整合型的缓存框架。缓存相关的配置过多**，可以查阅j2cache-core核心包中的j2cache.properties文件中的说明。如下：
```properties
#J2Cache configuration
#########################################
# Cache Broadcast Method
# values:
# jgroups -> use jgroups's multicast
# redis -> use redis publish/subscribe mechanism (using jedis)
# lettuce -> use redis publish/subscribe mechanism (using lettuce, Recommend)
# rabbitmq -> use RabbitMQ publisher/consumer mechanism
# rocketmq -> use RocketMQ publisher/consumer mechanism
# none -> don't notify the other nodes in cluster
# xx.xxxx.xxxx.Xxxxx your own cache broadcast policy classname that implement net.oschina.j2cache.cluster.ClusterPolicy
#########################################
j2cache.broadcast = redis

# jgroups properties
jgroups.channel.name = j2cache
jgroups.configXml = /network.xml

# RabbitMQ properties
rabbitmq.exchange = j2cache
rabbitmq.host = localhost
rabbitmq.port = 5672
rabbitmq.username = guest
rabbitmq.password = guest

# RocketMQ properties
rocketmq.name = j2cache
rocketmq.topic = j2cache
# use ; to split multi hosts
rocketmq.hosts = 127.0.0.1:9876

#########################################
# Level 1&2 provider
# values:
# none -> disable this level cache
# ehcache -> use ehcache2 as level 1 cache
# ehcache3 -> use ehcache3 as level 1 cache
# caffeine -> use caffeine as level 1 cache(only in memory)
# redis -> use redis as level 2 cache (using jedis)
# lettuce -> use redis as level 2 cache (using lettuce)
# readonly-redis -> use redis as level 2 cache ,but never write data to it. if use this provider, you must uncomment `j2cache.L2.config_section` to make the redis configurations available.
# memcached -> use memcached as level 2 cache (xmemcached),
# [classname] -> use custom provider
#########################################

j2cache.L1.provider_class = caffeine
j2cache.L2.provider_class = redis

# When L2 provider isn't `redis`, using `L2.config_section = redis` to read redis configurations
# j2cache.L2.config_section = redis

# Enable/Disable ttl in redis cache data (if disabled, the object in redis will never expire, default:true)
# NOTICE: redis hash mode (redis.storage = hash) do not support this feature)
j2cache.sync_ttl_to_redis = true

# Whether to cache null objects by default (default false)
j2cache.default_cache_null_object = true

#########################################
# Cache Serialization Provider
# values:
# fst -> using fast-serialization (recommend)
# kryo -> using kryo serialization
# json -> using fst's json serialization (testing)
# fastjson -> using fastjson serialization (embed non-static class not support)
# java -> java standard
# fse -> using fse serialization
# [classname implements Serializer]
#########################################

j2cache.serialization = json
#json.map.person = net.oschina.j2cache.demo.Person

#########################################
# Ehcache configuration
#########################################

# ehcache.configXml = /ehcache.xml

# ehcache3.configXml = /ehcache3.xml
# ehcache3.defaultHeapSize = 1000

#########################################
# Caffeine configuration
# caffeine.region.[name] = size, xxxx[s|m|h|d]
#
#########################################
caffeine.properties = /caffeine.properties

#########################################
# Redis connection configuration
#########################################

#########################################
# Redis Cluster Mode
#
# single -> single redis server
# sentinel -> master-slaves servers
# cluster -> cluster servers (数据库配置无效，使用 database = 0）
# sharded -> sharded servers  (密码、数据库必须在 hosts 中指定，且连接池配置无效 ; redis://user:password@127.0.0.1:6379/0）
#
#########################################

redis.mode = single

#redis storage mode (generic|hash)
redis.storage = generic

## redis pub/sub channel name
redis.channel = j2cache
## redis pub/sub server (using redis.hosts when empty)
redis.channel.host =

#cluster name just for sharded
redis.cluster_name = j2cache

## redis cache namespace optional, default[empty]
redis.namespace =

## redis command scan parameter count, default[1000]
#redis.scanCount = 1000

## connection
# Separate multiple redis nodes with commas, such as 192.168.0.10:6379,192.168.0.11:6379,192.168.0.12:6379

redis.hosts = 127.0.0.1:6379
redis.timeout = 2000
redis.password =
redis.database = 0
redis.ssl = false

## redis pool properties
redis.maxTotal = 100
redis.maxIdle = 10
redis.maxWaitMillis = 5000
redis.minEvictableIdleTimeMillis = 60000
redis.minIdle = 1
redis.numTestsPerEvictionRun = 10
redis.lifo = false
redis.softMinEvictableIdleTimeMillis = 10
redis.testOnBorrow = true
redis.testOnReturn = false
redis.testWhileIdle = true
redis.timeBetweenEvictionRunsMillis = 300000
redis.blockWhenExhausted = false
redis.jmxEnabled = false

#########################################
# Lettuce scheme
#
# redis -> single redis server
# rediss -> single redis server with ssl
# redis-sentinel -> redis sentinel
# redis-cluster -> cluster servers
#
#########################################

#########################################
# Lettuce Mode
#
# single -> single redis server
# sentinel -> master-slaves servers
# cluster -> cluster servers (数据库配置无效，使用 database = 0）
# sharded -> sharded servers  (密码、数据库必须在 hosts 中指定，且连接池配置无效 ; redis://user:password@127.0.0.1:6379/0）
#
#########################################

## redis command scan parameter count, default[1000]
#lettuce.scanCount = 1000
lettuce.mode = single
lettuce.namespace =
lettuce.storage = hash
lettuce.channel = j2cache
lettuce.scheme = redis
lettuce.hosts = 127.0.0.1:6379
lettuce.password =
lettuce.database = 0
lettuce.sentinelMasterId =
lettuce.maxTotal = 100
lettuce.maxIdle = 10
lettuce.minIdle = 10
# timeout in milliseconds
lettuce.timeout = 10000
# redis cluster topology refresh interval in milliseconds
lettuce.clusterTopologyRefresh = 3000

#########################################
# memcached server configurations
# refer to https://gitee.com/mirrors/XMemcached
#########################################

memcached.servers = 127.0.0.1:11211
memcached.username =
memcached.password =
memcached.connectionPoolSize = 10
memcached.connectTimeout = 1000
memcached.failureMode = false
memcached.healSessionInterval = 1000
memcached.maxQueuedNoReplyOperations = 100
memcached.opTimeout = 100
memcached.sanitizeKeys = false
```
**总结**
1. j2cache是一个缓存框架，自身不具有缓存功能，它提供多种缓存整合在一起使用的方案
2. j2cache需要通过复杂的配置设置各级缓存，以及缓存之间数据交换的方式
3. j2cache操作接口通过CacheChannel实现

### KF-5-2.任务
​springboot整合第三方技术第二部分来说说任务系统，其实这里说的==任务系统指的是定时任务==。定时任务是企业级开发中必不可少的组成部分，诸如长周期业务数据的计算，例如年度报表，诸如系统脏数据的处理，再比如系统性能监控报告，还有==抢购类活动的商品上架==，这些都离不开定时任务。本节将介绍两种不同的定时任务技术。
#### Quartz
​Quartz技术是一个比较成熟的定时任务框架，怎么说呢？有点繁琐，用过的都知道，配置略微复杂。==springboot对其进行整合后，简化了一系列的配置，将很多配置采用默认设置==，这样开发阶段就简化了很多。再学习springboot整合Quartz前先普及几个Quartz的概念。
- 工作（Job）：用于定义具体执行的工作
- 工作明细（JobDetail）：用于描述**定时工作相关的信息**
- 触发器（Trigger）：描述了**工作明细与调度器的对应关系**
- 调度器（Scheduler）：用于描述**触发工作的执行规则**，通常使用cron表达式定义规则

​简单说就是你定时干什么事情，这就是工作，工作不可能就是一个简单的方法，还要设置一些明细信息。工作啥时候执行，设置一个调度器，可以简单理解成设置一个工作执行的时间。工作和调度都是独立定义的，它们两个怎么配合到一起呢？用触发器。完了，就这么多。下面开始springboot整合Quartz。

**步骤①**：导入springboot整合Quartz的starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-quartz</artifactId>
</dependency>
```
**步骤②**：定义具体要执行的任务Bean，按照Quartz的开发规范制作，继承QuartzJobBean
```java
public class MyQuartz extends QuartzJobBean {
    @Override
    protected void executeInternal(JobExecutionContext context) throws JobExecutionException {
        System.out.println("quartz task run...");
    }
}
```
**步骤③**：创建Quartz配置类，定义工作明细（JobDetail）与触发器的（Trigger）bean
```java
@Configuration
public class QuartzConfig {
    @Bean
    public JobDetail printJobDetail(){
        //绑定具体的工作
        return JobBuilder.newJob(MyQuartz.class).storeDurably().build();
    }
    @Bean
    public Trigger printJobTrigger(){
        ScheduleBuilder schedBuilder = CronScheduleBuilder.cronSchedule("0/5 * * * * ?"); // 日期和星期不同时规定cron表达式
        //绑定对应的工作明细
        return TriggerBuilder.newTrigger().forJob(printJobDetail()).withSchedule(schedBuilder).build(); 
    }
}
```
​**工作明细中要设置对应的具体工作**，使用newJob()操作传入对应的工作任务类型即可。

​**触发器需要绑定任务**，**使用forJob()操作传入绑定的工作明细对象**。此处可以为工作明细设置名称然后使用名称绑定，也可以直接调用对应方法绑定。==触发器中最核心的规则是执行时间，此处使用调度器定义执行时间，执行时间描述方式使用的是cron表达式==。有关cron表达式的规则，各位小伙伴可以去参看相关课程学习，略微复杂，而且格式不能乱设置，不是写个格式就能用的，写不好就会出现冲突问题。

**总结**
1. springboot整合Quartz就是将Quartz对应的核心对象交给spring容器管理，包含两个对象，JobDetail和Trigger对象
2. JobDetail对象描述的是工作的执行信息，需要绑定一个QuartzJobBean类型的对象
3. Trigger对象定义了一个触发器，需要为其指定绑定的JobDetail是哪个，同时要设置执行周期调度器

**思考**
​上面的操作看上去不多，但**Quartz将其中的对象划分粒度过细，导致开发的时候有点繁琐**，spring针对上述规则进行了简化，开发了自己的任务管理组件——Task，如何用呢？咱们下节再说。

#### Task
​spring根据定时任务的特征，将定时任务的开发简化到了极致。怎么说呢？要做定时任务**总要告诉容器有这功能吧**，然后**定时执行什么任务**直接告诉对应的bean什么时间执行就行了，就这么简单，一起来看怎么做

**步骤①**：开启定时任务功能，在引导类上开启定时任务功能的开关，使用注解@EnableScheduling
```java
@SpringBootApplication
//开启定时任务功能
@EnableScheduling
public class Springboot22TaskApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot22TaskApplication.class, args);
    }
}
```
**步骤②**：定义Bean，在对应要定时执行的操作上方，使用注解@Scheduled定义执行的时间，执行时间的描述方式还是cron表达式
```java
@Component
public class MyBean {
    @Scheduled(cron = "0/1 * * * * ?")
    public void print(){
        System.out.println(Thread.currentThread().getName()+" :spring task run...");
    }
}
```
​完事，这就完成了定时任务的配置。总体感觉其实什么东西都没少，==只不过没有将所有的信息都抽取成bean，而是直接使用注解绑定定时执行任务的事情而已==。

​如何想对定时任务进行相关配置，可以通过配置文件进行
```yaml
spring:
  task:
   	scheduling:
      pool:
       	size: 1	# 任务调度线程池大小 默认 1
      thread-name-prefix: ssm_      	# 调度线程名称前缀 默认 scheduling-      
        shutdown:
          await-termination: false # 线程池关闭时等待所有任务完成
          await-termination-period: 10s	# 调度线程关闭前最大等待时间，确保最后一定关闭
```
**总结**
1. spring task需要使用注解@EnableScheduling开启定时任务功能
2. 为定时执行的的任务设置执行周期，描述方式cron表达式

### KF-5-3.邮件
​springboot整合第三方技术第三部分说说邮件系统，发邮件是java程序的基本操作，springboot整合javamail其实就是简化开发。不熟悉邮件的小伙伴可以先学习完javamail的基础操作，再来看这一部分内容才能感触到springboot整合javamail究竟简化了哪些操作。简化的多吗？其实不多，差别不大，只是改个格式而已。

​学习邮件发送之前先了解3个概念，这些概念规范了邮件操作过程中的标准。
- SMTP（Simple Mail Transfer Protocol）：简单邮件传输协议，用于**发送**电子邮件的传输协议
- POP3（Post Office Protocol - Version 3）：用于**接收**电子邮件的标准协议
- IMAP（Internet Mail Access Protocol）：互联网消息协议，是POP3的替代协议

​简单说就是SMPT是发邮件的标准，POP3是收邮件的标准，IMAP是对POP3的升级。==我们制作程序中操作邮件，通常是发邮件，所以SMTP是使用的重点，收邮件大部分都是通过邮件客户端完成==，所以**开发收邮件的代码极少**。除非你要读取邮件内容，然后解析，做邮件功能的统一处理。例如HR的邮箱收到求职者的简历，可以读取后统一处理。但为什么不制作独立的投递简历的系统呢？

所以说，好奇怪的需求，因为要想收邮件就要规范发邮件的人的书写格式，这个未免有点强人所难，并且极易收到外部攻击，你不可能使用白名单来收邮件。如果能使用白名单来收邮件然后解析邮件，==还不如开发个系统给白名单中的人专用呢，更安全，总之就是鸡肋了==。下面就开始学习springboot如何整合javamail发送邮件。
#### 发送简单邮件
**步骤①**：导入springboot整合javamail的starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-mail</artifactId>
</dependency>
```
**步骤②**：配置邮箱的登录信息
```yaml
spring:
  mail:
    host: smtp.126.com
    username: test@126.com
    password: test
```
​java程序仅用于发送邮件，邮件的功能还是邮件供应商提供的，所以**这里是用别人的邮件服务，要配置对应信息**。
- ​host配置的是**提供邮件服务的主机协议**，当前程序仅用于发送邮件，因此配置的是smtp的协议。
- ​password并不是邮箱账号的登录密码，是邮件供应商提供的一个加密后的密码，也是为了保障系统安全性。不然==外部人员通过地址访问下载了配置文件，直接获取到了邮件密码就会有极大的安全隐患==。有关该密码的获取，每个邮件供应商提供的方式都不一样，此处略过。可以到邮件供应商的设置页面找POP3或IMAP这些关键词找到对应的获取位置。下例仅供参考：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251128874.png)
**步骤③**：使用JavaMailSender接口发送邮件
```java
@Service
public class SendMailServiceImpl implements SendMailService {
    @Autowired
    private JavaMailSender javaMailSender;

    //发送人
    private String from = "test@qq.com";
    //接收人
    private String to = "test@126.com";
    //标题
    private String subject = "测试邮件";
    //正文
    private String context = "测试邮件正文内容";

    @Override
    public void sendMail() {
        SimpleMailMessage message = new SimpleMailMessage();
        message.setFrom(from+"(小甜甜)");
        message.setTo(to);
        message.setSubject(subject);
        message.setText(context);
        javaMailSender.send(message);
    }
}
```
> 已经解决了 老师将serviceimpl设置为bean了
> 亲测只能发几次，之后会报错，还不能再次发送（拿自己邮箱发给自己试了一下）
> 发太多就被qq邮箱阻止了，会报错的
> QQ邮件发送失败的话，需要开启ssl
> properties:mail:smtp:ssl:enable: true
> 这边from和to要填写两个账号

​将发送邮件的必要信息（发件人、收件人、标题、正文）封装到SimpleMailMessage对象中，可以根据规则设置发送人昵称等。

#### 发送多组件邮件（附件、复杂正文）
​发送简单邮件仅需要提供对应的4个基本信息就可以了，如果想发送复杂的邮件，需要更换邮件对象。使用MimeMessage可以发送特殊的邮件。
**发送网页正文邮件**
```JAVA
@Service
public class SendMailServiceImpl2 implements SendMailService {
    @Autowired
    private JavaMailSender javaMailSender;
    //发送人
    private String from = "test@qq.com";
    //接收人
    private String to = "test@126.com";
    //标题
    private String subject = "测试邮件";
    //正文
    private String context = "<img src='ABC.JPG'/><a href='https://www.itcast.cn'>点开有惊喜</a>";
    public void sendMail() {
        try {
            MimeMessage message = javaMailSender.createMimeMessage();
            MimeMessageHelper helper = new MimeMessageHelper(message);
            helper.setFrom(to+"(小甜甜)");
            helper.setTo(from);
            helper.setSubject(subject);
            helper.setText(context,true);//此处设置正文支持html解析
            javaMailSender.send(message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
**发送带有附件的邮件**
```JAVA
@Service
public class SendMailServiceImpl2 implements SendMailService {
    @Autowired
    private JavaMailSender javaMailSender;
    //发送人
    private String from = "test@qq.com";
    //接收人
    private String to = "test@126.com";
    //标题
    private String subject = "测试邮件";
    //正文
    private String context = "测试邮件正文";

    public void sendMail() {
        try {
            MimeMessage message = javaMailSender.createMimeMessage();
            MimeMessageHelper helper = new MimeMessageHelper(message,true);//此处设置支持附件
            helper.setFrom(to+"(小甜甜)");
            helper.setTo(from);
            helper.setSubject(subject);
            helper.setText(context);

            //添加附件
            File f1 = new File("springboot_23_mail-0.0.1-SNAPSHOT.jar");
            File f2 = new File("resources\\logo.png");

            helper.addAttachment(f1.getName(),f1);
            helper.addAttachment("最靠谱的培训结构.png",f2);

            javaMailSender.send(message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
**总结**
1. springboot整合javamail其实就是简化了发送邮件的客户端对象JavaMailSender的初始化过程，通过配置的形式加载信息简化开发过程

### KF-5-4.消息
​springboot整合第三方技术最后一部分说说消息中间件，首先先介绍一下消息的应用。
#### 消息的概念
​从广义角度来说，消息其实就是信息，但是和信息又有所不同。==信息通常被定义为一组数据，而消息除了具有数据的特征之外，还有消息的来源与接收的概念==。通常发送消息的一方称为消息的生产者，接收消息的一方称为消息的消费者。这样比较后，发现其实消息和信息差别还是很大的。

​为什么要设置生产者和消费者呢？这就是要说到消息的意义了。信息通常就是一组数据，但==消息由于有了生产者和消费者，就出现了消息中所包含的信息可以被二次解读==，生产者发送消息，可以理解为生产者发送了一个信息，也可以理解为生产者发送了一个命令；消费者接收消息，可以理解为消费者得到了一个信息，也可以理解为消费者得到了一个命令。

对比一下我们会发现信息是一个基本数据，而命令则可以关联下一个行为动作，这样就可以理解为==基于接收的消息相当于得到了一个行为动作，使用这些行为动作就可以组织成一个业务逻辑，进行进一步的操作==。总的来说，**消息其实也是一组信息，只是为其赋予了全新的含义，因为有了消息的流动，并且是有方向性的流动，带来了基于流动的行为产生的全新解读**。开发者就可以基于消息的这种特殊解，将其换成代码中的指令。

​对于消息的理解，初学者总认为消息内部的数据非常复杂，这是一个误区。比如我发送了一个消息，要求接受者翻译发送过去的内容。初学者会认为消息中会包含被翻译的文字，已经本次操作要执行翻译操作而不是打印操作。其实这种现象有点过度解读了，发送的消息中仅仅包含被翻译的文字，但可以通过控制不同的人接收此消息来确认要做的事情。例如发送被翻译的文字仅到A程序，而A程序只能进行翻译操作，这样就可以发送简单的信息、完成复杂的业务了，是通过接收消息的主体不同，进而执行不同的操作，而**不会在消息内部定义数据的操作行为**，当然如果开发者希望消息中包含操作种类信息也是可以的，只是提出消息的内容可以更简单，更单一。

​对于消息的生产者与消费者的工作模式，还可以将消息划分成两种模式，同步消费与异步消息。
- ​所谓同步消息就是生产者发送完消息，等待消费者处理，消费者处理完将结果告知生产者，然后生产者继续向下执行业务。这种模式过于卡生产者的业务执行连续性，**在现在的企业级开发中，上述这种业务场景通常不会采用消息的形式进行处理**。
- ​所谓异步消息就是==生产者发送完消息，无需等待消费者处理完毕，生产者继续向下执行其他动作==。比如生产者发送了一个日志信息给日志系统，发送过去以后生产者就向下做其他事情了，无需关注日志系统的执行结果。日志系统根据接收到的日志信息继续进行业务执行，是单纯的记录日志，还是记录日志并报警，这些和生产者无关，这样**生产者的业务执行效率就会大幅度提升**。并且可以通过添加多个消费者、来处理同一个生产者发送的消息来**提高系统的高并发性，改善系统工作效率，提高用户体验**。一旦某一个消费者由于各种问题宕机了，也不会对业务产生影响，**提高了系统的高可用性**。

​以上简单的介绍了一下消息这种工作模式存在的意义，希望对各位学习者有所帮助。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303151721546.png)

#### Java处理消息的标准规范
​目前企业级开发中广泛使用的消息处理技术共三大类，具体如下：
- JMS
- AMQP
- MQTT

​为什么是三大类，而不是三个技术呢？因为这些都是规范，就想JDBC技术，是个规范，开发针对规范开发，运行还要靠实现类，例如MySQL提供了JDBC的实现，最终运行靠的还是实现。并且这三类规范都是针对异步消息进行处理的，也符合消息的设计本质，处理异步的业务。对以上三种消息规范做一下普及

##### JMS
​JMS（Java Message Service）：这是一个规范，作用等同于JDBC规范，提供了与消息服务相关的API接口。实现由厂商进行。

**JMS消息模型**
​JMS规范中规范了消息有两种模型。分别是**点对点模型**和**发布订阅模型**。
- ​**点对点模型**：peer-2-peer，生产者会将消息发送到一个保存消息的容器中，通常使用队列模型，使用队列保存消息。==一个队列的消息只能被一个消费者消费，或未被及时消费导致超时==。这种模型下，生产者和消费者是一对一绑定的。
- ​**发布订阅模型**：publish-subscribe，生产者将消息发送到一个保存消息的容器中，也是使用队列模型来保存。但消息可以被多个消费者消费，生产者和消费者完全独立，相互不需要感知对方的存在。

​以上这种分类是从消息的生产和消费过程来进行区分，针对消息所包含的信息不同，还可以进行不同类别的划分。

**JMS消息种类**
​根据消息中包含的数据种类划分，可以将消息划分成6种消息。
- TextMessage
- MapMessage
- **BytesMessage**（最常用）
- StreamMessage
- ObjectMessage
- Message （只有消息头和属性）

​JMS主张不同种类的消息，消费方式不同，可以根据使用需要选择不同种类的消息。但这一点也成为其诟病之处，后面再说。整体上来说，JMS就是典型的保守派，什么都按照J2EE的规范来，做一套规范，定义若干个标准，每个标准下又提供一大批API。目前对JMS规范实现的消息中间件技术还是挺多的，毕竟是皇家御用，肯定有人舔，例如ActiveMQ、Redis、HornetMQ。但也有一些不太规范的实现，参考JMS的标准设计，但是又不完全满足其规范，例如：RabbitMQ、RocketMQ。

##### AMQP
​JMS的问世为消息中间件提供了很强大的规范性支撑，但使用的过程中就开始被人诟病，比如JMS设置的极其复杂的多种类消息处理机制。本来分门别类处理挺好的，为什么会被诟病呢？原因就在于JMS的设计是J2EE规范，站在Java开发的角度思考问题。但现实往往是复杂度很高的。比如我有一个.NET开发的系统A，有一个Java开发的系统B，现在要从A系统给B系统发业务消息，结果两边数据格式不统一，没法操作。JMS不是可以统一数据格式吗？提供了6种数据种类，总有一款适合你啊。NO，一个都不能用。因为A系统的底层语言不是Java语言开发的，根本不支持那些对象。这就意味着：如果想使用现有的业务系统A继续开发已经不可能了，必须推翻重新做使用Java语言开发的A系统。

​这时候有人就提出说，你搞那么复杂，整那么多种类干什么？找一种大家都支持的消息数据类型，不就解决这个跨平台的问题了吗？大家一想，对啊，于是AMQP孕育而生。

​单从上面的说明中，其实可以明确感知到，AMQP的出现解决的是：**消息传递时使用的消息种类**的问题，化繁为简，但其并没有完全推翻JMS的操作API，所以说**AMQP仅仅是一种协议，规范了数据传输的格式而已**。

​AMQP（advanced message queuing protocol）：一种协议（高级消息队列协议，也是消息代理规范），规范了网络交换的数据格式，兼容JMS操作。
**优点**：​具有跨平台性，服务器供应商，生产者，消费者可以使用不同的语言来实现

**JMS消息种类**
​AMQP消息种类：`byte[]`（一统天下）
​AMQP在JMS的消息模型基础上又进行了进一步的扩展，除了点对点和发布订阅的模型，开发了**几种全新的消息模型**，适应各种各样的消息发送。

**AMQP消息模型**
- direct exchange
- fanout exchange
- topic exchange
- headers exchange
- system exchange

​目前实现了AMQP协议的消息中间件技术也很多，而且都是较为流行的技术，例如：RabbitMQ、StormMQ、RocketMQ

##### MQTT
​MQTT（Message Queueing Telemetry Transport）消息队列遥测传输，专为小设备设计，是物联网（IOT）生态系统中主要成分之一。由于与JavaEE企业级开发没有交集，此处不作过多的说明。

​除了上述3种J2EE企业级应用中广泛使用的三种异步消息传递技术，还有一种技术也不能忽略，Kafka。
##### KafKa
​Kafka，一种高吞吐量的分布式发布订阅消息系统，提供实时消息功能。Kafka技术并不是作为消息中间件为主要功能的产品，但是其拥有发布订阅的工作模式，也可以充当消息中间件来使用，而且目前企业级开发中其身影也不少见。

​本节内容讲围绕着上述内容中的几种实现方案，讲解springboot整合各种各样的消息中间件。由于各种消息中间件必须先安装再使用，下面的内容采用Windows系统安装，降低各位学习者的学习难度，基本套路和之前学习NoSQL解决方案一样，先安装再整合。
#### 购物订单发送手机短信案例
购物订单业务
- 登录状态检测
- 生成主单
- 生成子单
- 库存检测与变更
- 积分变更
- 支付
- 短信通知（必须是异步的）
- 购物车维护
- 运单信息初始化
- 商品库存维护
- 会员维护
​
为了便于下面演示各种各样的消息中间件技术，我们创建一个**购物过程生成订单时**为用户发送短信的案例环境，模拟**使用消息中间件实现发送手机短信**的过程。​手机验证码案例需求如下：
- 执行下单业务时（模拟此过程），调用消息服务，将要发送短信的订单id传递给消息中间件
- 消息处理服务接收到要发送的订单id后，输出订单id（模拟发短信）

由于不涉及数据读写，仅开发业务层与表现层，其中短信处理的业务代码独立开发，代码如下：**订单业务**
​**业务层接口**
```JAVA
public interface OrderService {
    void order(String id);
}
```
​模拟传入订单id，执行下订单业务，参数为虚拟设定，实际应为订单对应的实体类

​**业务层实现**
```JAVA
@Service
public class OrderServiceImpl implements OrderService {
    @Autowired
    private MessageService messageService;
    
    @Override
    public void order(String id) {
        //一系列操作，包含各种服务调用，处理各种业务
        System.out.println("订单处理开始");
        //短信消息处理
        messageService.sendMessage(id);
        System.out.println("订单处理结束");
        System.out.println();
    }
}
```
​业务层转调短信处理的服务MessageService

​**表现层服务**
```JAVA
@RestController
@RequestMapping("/orders")
public class OrderController {

    @Autowired
    private OrderService orderService;

    @PostMapping("{id}")
    public void order(@PathVariable String id){
        orderService.order(id);
    }
}
```
​表现层对外开放接口，传入订单id即可（模拟）

**短信处理业务**
​**业务层接口**
```JAVA
public interface MessageService {
    void sendMessage(String id);
    String doMessage();
}
```
​短信处理业务层接口提供两个操作，发送要处理的订单id到消息中间件，另一个操作目前暂且设计成处理消息，实际消息的处理过程不应该是手动执行，应该是自动执行，到具体实现时再进行设计
​**业务层实现**
```JAVA
@Service
public class MessageServiceImpl implements MessageService {
    private ArrayList<String> msgList = new ArrayList<String>();

    @Override
    public void sendMessage(String id) {
        System.out.println("待发送短信的订单已纳入处理队列，id："+id);
        msgList.add(id);
    }

    @Override
    public String doMessage() {
        String id = msgList.remove(0);
        System.out.println("已完成短信发送业务，id："+id);
        return id;
    }
}
```
​短信处理业务层实现中使用集合先模拟消息队列，观察效果
​**表现层服务**
```JAVA
@RestController
@RequestMapping("/msgs")
public class MessageController {
    @Autowired
    private MessageService messageService;

    @GetMapping
    public String doMessage(){
        String id = messageService.doMessage();
        return id;
    }
}
```
​短信处理表现层接口暂且开放出一个处理消息的入口，但此业务是对应业务层中设计的模拟接口，实际业务不需要设计此接口。

​下面开启springboot整合各种各样的消息中间件，从严格满足JMS规范的ActiveMQ开始
#### SpringBoot整合ActiveMQ
​ActiveMQ是MQ产品中的元老级产品，早期标准MQ产品之一，在AMQP协议没有出现之前，占据了消息中间件市场的绝大部分份额，后期因为AMQP系列产品的出现，迅速走弱，目前仅在一些线上运行的产品中出现，新产品开发较少采用。

##### 安装
​windows版安装包下载地址：[https://activemq.apache.org/components/classic/download](https://activemq.apache.org/components/classic/download/)[/](https://activemq.apache.org/components/classic/download/)

​下载的安装包是解压缩就能使用的zip文件，解压缩完毕后会得到如下文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251128526.png)
**启动服务器**
```CMD
activemq.bat
```
​运行bin目录下的win32或win64目录下的activemq.bat命令即可，根据自己的操作系统选择即可，默认对外服务端口61616。

**访问web管理服务**
​ActiveMQ启动后会启动一个Web控制台服务，可以通过该服务管理ActiveMQ。
```CMD
http://127.0.0.1:8161/
```
​web管理服务默认端口8161，访问后可以打开ActiveMQ的管理界面，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251128826.png)
​首先输入访问用户名和密码，初始化用户名和密码相同，均为：admin，成功登录后进入管理后台界面，如下：

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251128663.png)
​看到上述界面视为启动ActiveMQ服务成功。

**启动失败**
​在ActiveMQ启动时要占用多个端口，以下为正常启动信息：」
```bash
wrapper  | --> Wrapper Started as Console
wrapper  | Launching a JVM...
jvm 1    | Wrapper (Version 3.2.3) http://wrapper.tanukisoftware.org
jvm 1    |   Copyright 1999-2006 Tanuki Software, Inc.  All Rights Reserved.
jvm 1    |
jvm 1    | Java Runtime: Oracle Corporation 1.8.0_172 D:\soft\jdk1.8.0_172\jre
jvm 1    |   Heap sizes: current=249344k  free=235037k  max=932352k
jvm 1    |     JVM args: -Dactivemq.home=../.. -Dactivemq.base=../.. -Djavax.net.ssl.keyStorePassword=password -Djavax.net.ssl.trustStorePassword=password -Djavax.net.ssl.keyStore=../../conf/broker.ks -Djavax.net.ssl.trustStore=../../conf/broker.ts -Dcom.sun.management.jmxremote -Dorg.apache.activemq.UseDedicatedTaskRunner=true -Djava.util.logging.config.file=logging.properties -Dactivemq.conf=../../conf -Dactivemq.data=../../data -Djava.security.auth.login.config=../../conf/login.config -Xmx1024m -Djava.library.path=../../bin/win64 -Dwrapper.key=7ySrCD75XhLCpLjd -Dwrapper.port=32000 -Dwrapper.jvm.port.min=31000 -Dwrapper.jvm.port.max=31999 -Dwrapper.pid=9364 -Dwrapper.version=3.2.3 -Dwrapper.native_library=wrapper -Dwrapper.cpu.timeout=10 -Dwrapper.jvmid=1
jvm 1    | Extensions classpath:
jvm 1    |   [..\..\lib,..\..\lib\camel,..\..\lib\optional,..\..\lib\web,..\..\lib\extra]
jvm 1    | ACTIVEMQ_HOME: ..\..
jvm 1    | ACTIVEMQ_BASE: ..\..
jvm 1    | ACTIVEMQ_CONF: ..\..\conf
jvm 1    | ACTIVEMQ_DATA: ..\..\data
jvm 1    | Loading message broker from: xbean:activemq.xml
jvm 1    |  INFO | Refreshing org.apache.activemq.xbean.XBeanBrokerFactory$1@5f3ebfe0: startup date [Mon Feb 28 16:07:48 CST 2022]; root of context hierarchy
jvm 1    |  INFO | Using Persistence Adapter: KahaDBPersistenceAdapter[D:\soft\activemq\bin\win64\..\..\data\kahadb]
jvm 1    |  INFO | KahaDB is version 7
jvm 1    |  INFO | PListStore:[D:\soft\activemq\bin\win64\..\..\data\localhost\tmp_storage] started
jvm 1    |  INFO | Apache ActiveMQ 5.16.3 (localhost, ID:CZBK-20210302VL-10434-1646035669595-0:1) is starting
jvm 1    |  INFO | Listening for connections at: tcp://CZBK-20210302VL:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600
jvm 1    |  INFO | Connector openwire started
jvm 1    |  INFO | Listening for connections at: amqp://CZBK-20210302VL:5672?maximumConnections=1000&wireFormat.maxFrameSize=104857600
jvm 1    |  INFO | Connector amqp started
jvm 1    |  INFO | Listening for connections at: stomp://CZBK-20210302VL:61613?maximumConnections=1000&wireFormat.maxFrameSize=104857600
jvm 1    |  INFO | Connector stomp started
jvm 1    |  INFO | Listening for connections at: mqtt://CZBK-20210302VL:1883?maximumConnections=1000&wireFormat.maxFrameSize=104857600
jvm 1    |  INFO | Connector mqtt started
jvm 1    |  INFO | Starting Jetty server
jvm 1    |  INFO | Creating Jetty connector
jvm 1    |  WARN | ServletContext@o.e.j.s.ServletContextHandler@7350746f{/,null,STARTING} has uncovered http methods for path: /
jvm 1    |  INFO | Listening for connections at ws://CZBK-20210302VL:61614?maximumConnections=1000&wireFormat.maxFrameSize=104857600
jvm 1    |  INFO | Connector ws started
jvm 1    |  INFO | Apache ActiveMQ 5.16.3 (localhost, ID:CZBK-20210302VL-10434-1646035669595-0:1) started
jvm 1    |  INFO | For help or more information please see: http://activemq.apache.org
jvm 1    |  WARN | Store limit is 102400 mb (current store usage is 0 mb). The data directory: D:\soft\activemq\bin\win64\..\..\data\kahadb only has 68936 mb of usable space. - resetting to maximum available disk space: 68936 mb
jvm 1    |  INFO | ActiveMQ WebConsole available at http://127.0.0.1:8161/
jvm 1    |  INFO | ActiveMQ Jolokia REST API available at http://127.0.0.1:8161/api/jolokia/
```
​其中占用的端口有：61616、5672、61613、1883、61614，如果启动失败，请先管理对应端口即可。以下就是某个端口占用的报错信息，可以从抛出异常的位置看出，启动5672端口时端口被占用，显示java.net.BindException: Address already in use: JVM_Bind。Windows系统中终止端口运行的操作参看[【命令行启动常见问题及解决方案】](#命令行启动常见问题及解决方案)
```bash
wrapper  | --> Wrapper Started as Console
wrapper  | Launching a JVM...
jvm 1    | Wrapper (Version 3.2.3) http://wrapper.tanukisoftware.org
jvm 1    |   Copyright 1999-2006 Tanuki Software, Inc.  All Rights Reserved.
jvm 1    |
jvm 1    | Java Runtime: Oracle Corporation 1.8.0_172 D:\soft\jdk1.8.0_172\jre
jvm 1    |   Heap sizes: current=249344k  free=235038k  max=932352k
jvm 1    |     JVM args: -Dactivemq.home=../.. -Dactivemq.base=../.. -Djavax.net.ssl.keyStorePassword=password -Djavax.net.ssl.trustStorePassword=password -Djavax.net.ssl.keyStore=../../conf/broker.ks -Djavax.net.ssl.trustStore=../../conf/broker.ts -Dcom.sun.management.jmxremote -Dorg.apache.activemq.UseDedicatedTaskRunner=true -Djava.util.logging.config.file=logging.properties -Dactivemq.conf=../../conf -Dactivemq.data=../../data -Djava.security.auth.login.config=../../conf/login.config -Xmx1024m -Djava.library.path=../../bin/win64 -Dwrapper.key=QPJoy9ZoXeWmmwTS -Dwrapper.port=32000 -Dwrapper.jvm.port.min=31000 -Dwrapper.jvm.port.max=31999 -Dwrapper.pid=14836 -Dwrapper.version=3.2.3 -Dwrapper.native_library=wrapper -Dwrapper.cpu.timeout=10 -Dwrapper.jvmid=1
jvm 1    | Extensions classpath:
jvm 1    |   [..\..\lib,..\..\lib\camel,..\..\lib\optional,..\..\lib\web,..\..\lib\extra]
jvm 1    | ACTIVEMQ_HOME: ..\..
jvm 1    | ACTIVEMQ_BASE: ..\..
jvm 1    | ACTIVEMQ_CONF: ..\..\conf
jvm 1    | ACTIVEMQ_DATA: ..\..\data
jvm 1    | Loading message broker from: xbean:activemq.xml
jvm 1    |  INFO | Refreshing org.apache.activemq.xbean.XBeanBrokerFactory$1@2c9392f5: startup date [Mon Feb 28 16:06:16 CST 2022]; root of context hierarchy
jvm 1    |  INFO | Using Persistence Adapter: KahaDBPersistenceAdapter[D:\soft\activemq\bin\win64\..\..\data\kahadb]
jvm 1    |  INFO | KahaDB is version 7
jvm 1    |  INFO | PListStore:[D:\soft\activemq\bin\win64\..\..\data\localhost\tmp_storage] started
jvm 1    |  INFO | Apache ActiveMQ 5.16.3 (localhost, ID:CZBK-20210302VL-10257-1646035577620-0:1) is starting
jvm 1    |  INFO | Listening for connections at: tcp://CZBK-20210302VL:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600
jvm 1    |  INFO | Connector openwire started
jvm 1    | ERROR | Failed to start Apache ActiveMQ (localhost, ID:CZBK-20210302VL-10257-1646035577620-0:1)
jvm 1    | java.io.IOException: Transport Connector could not be registered in JMX: java.io.IOException: Failed to bind to server socket: amqp://0.0.0.0:5672?maximumConnections=1000&wireFormat.maxFrameSize=104857600 due to: java.net.BindException: Address already in use: JVM_Bind
jvm 1    |      at org.apache.activemq.util.IOExceptionSupport.create(IOExceptionSupport.java:28)
jvm 1    |      at org.apache.activemq.broker.BrokerService.registerConnectorMBean(BrokerService.java:2288)
jvm 1    |      at org.apache.activemq.broker.BrokerService.startTransportConnector(BrokerService.java:2769)
jvm 1    |      at org.apache.activemq.broker.BrokerService.startAllConnectors(BrokerService.java:2665)
jvm 1    |      at org.apache.activemq.broker.BrokerService.doStartBroker(BrokerService.java:780)
jvm 1    |      at org.apache.activemq.broker.BrokerService.startBroker(BrokerService.java:742)
jvm 1    |      at org.apache.activemq.broker.BrokerService.start(BrokerService.java:645)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerService.afterPropertiesSet(XBeanBrokerService.java:73)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.invokeCustomInitMethod(AbstractAutowireCapableBeanFactory.java:1748)
jvm 1    |      at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.invokeInitMethods(AbstractAutowireCapableBeanFactory.java:1685)
jvm 1    |      at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.initializeBean(AbstractAutowireCapableBeanFactory.java:1615)
jvm 1    |      at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.doCreateBean(AbstractAutowireCapableBeanFactory.java:553)
jvm 1    |      at org.springframework.beans.factory.support.AbstractAutowireCapableBeanFactory.createBean(AbstractAutowireCapableBeanFactory.java:481)
jvm 1    |      at org.springframework.beans.factory.support.AbstractBeanFactory$1.getObject(AbstractBeanFactory.java:312)
jvm 1    |      at org.springframework.beans.factory.support.DefaultSingletonBeanRegistry.getSingleton(DefaultSingletonBeanRegistry.java:230)
jvm 1    |      at org.springframework.beans.factory.support.AbstractBeanFactory.doGetBean(AbstractBeanFactory.java:308)
jvm 1    |      at org.springframework.beans.factory.support.AbstractBeanFactory.getBean(AbstractBeanFactory.java:197)
jvm 1    |      at org.springframework.beans.factory.support.DefaultListableBeanFactory.preInstantiateSingletons(DefaultListableBeanFactory.java:756)
jvm 1    |      at org.springframework.context.support.AbstractApplicationContext.finishBeanFactoryInitialization(AbstractApplicationContext.java:867)
jvm 1    |      at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:542)
jvm 1    |      at org.apache.xbean.spring.context.ResourceXmlApplicationContext.<init>(ResourceXmlApplicationContext.java:64)
jvm 1    |      at org.apache.xbean.spring.context.ResourceXmlApplicationContext.<init>(ResourceXmlApplicationContext.java:52)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory$1.<init>(XBeanBrokerFactory.java:104)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory.createApplicationContext(XBeanBrokerFactory.java:104)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory.createBroker(XBeanBrokerFactory.java:67)
jvm 1    |      at org.apache.activemq.broker.BrokerFactory.createBroker(BrokerFactory.java:71)
jvm 1    |      at org.apache.activemq.broker.BrokerFactory.createBroker(BrokerFactory.java:54)
jvm 1    |      at org.apache.activemq.console.command.StartCommand.runTask(StartCommand.java:87)
jvm 1    |      at org.apache.activemq.console.command.AbstractCommand.execute(AbstractCommand.java:63)
jvm 1    |      at org.apache.activemq.console.command.ShellCommand.runTask(ShellCommand.java:154)
jvm 1    |      at org.apache.activemq.console.command.AbstractCommand.execute(AbstractCommand.java:63)
jvm 1    |      at org.apache.activemq.console.command.ShellCommand.main(ShellCommand.java:104)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.apache.activemq.console.Main.runTaskClass(Main.java:262)
jvm 1    |      at org.apache.activemq.console.Main.main(Main.java:115)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.tanukisoftware.wrapper.WrapperSimpleApp.run(WrapperSimpleApp.java:240)
jvm 1    |      at java.lang.Thread.run(Thread.java:748)
jvm 1    | Caused by: java.io.IOException: Failed to bind to server socket: amqp://0.0.0.0:5672?maximumConnections=1000&wireFormat.maxFrameSize=104857600 due to: java.net.BindException: Address already in use: JVM_Bind
jvm 1    |      at org.apache.activemq.util.IOExceptionSupport.create(IOExceptionSupport.java:34)
jvm 1    |      at org.apache.activemq.transport.tcp.TcpTransportServer.bind(TcpTransportServer.java:146)
jvm 1    |      at org.apache.activemq.transport.tcp.TcpTransportFactory.doBind(TcpTransportFactory.java:62)
jvm 1    |      at org.apache.activemq.transport.TransportFactorySupport.bind(TransportFactorySupport.java:40)
jvm 1    |      at org.apache.activemq.broker.TransportConnector.createTransportServer(TransportConnector.java:335)
jvm 1    |      at org.apache.activemq.broker.TransportConnector.getServer(TransportConnector.java:145)
jvm 1    |      at org.apache.activemq.broker.TransportConnector.asManagedConnector(TransportConnector.java:110)
jvm 1    |      at org.apache.activemq.broker.BrokerService.registerConnectorMBean(BrokerService.java:2283)
jvm 1    |      ... 46 more
jvm 1    | Caused by: java.net.BindException: Address already in use: JVM_Bind
jvm 1    |      at java.net.DualStackPlainSocketImpl.bind0(Native Method)
jvm 1    |      at java.net.DualStackPlainSocketImpl.socketBind(DualStackPlainSocketImpl.java:106)
jvm 1    |      at java.net.AbstractPlainSocketImpl.bind(AbstractPlainSocketImpl.java:387)
jvm 1    |      at java.net.PlainSocketImpl.bind(PlainSocketImpl.java:190)
jvm 1    |      at java.net.ServerSocket.bind(ServerSocket.java:375)
jvm 1    |      at java.net.ServerSocket.<init>(ServerSocket.java:237)
jvm 1    |      at javax.net.DefaultServerSocketFactory.createServerSocket(ServerSocketFactory.java:231)
jvm 1    |      at org.apache.activemq.transport.tcp.TcpTransportServer.bind(TcpTransportServer.java:143)
jvm 1    |      ... 52 more
jvm 1    |  INFO | Apache ActiveMQ 5.16.3 (localhost, ID:CZBK-20210302VL-10257-1646035577620-0:1) is shutting down
jvm 1    |  INFO | socketQueue interrupted - stopping
jvm 1    |  INFO | Connector openwire stopped
jvm 1    |  INFO | Could not accept connection during shutdown  : null (null)
jvm 1    |  INFO | Connector amqp stopped
jvm 1    |  INFO | Connector stomp stopped
jvm 1    |  INFO | Connector mqtt stopped
jvm 1    |  INFO | Connector ws stopped
jvm 1    |  INFO | PListStore:[D:\soft\activemq\bin\win64\..\..\data\localhost\tmp_storage] stopped
jvm 1    |  INFO | Stopping async queue tasks
jvm 1    |  INFO | Stopping async topic tasks
jvm 1    |  INFO | Stopped KahaDB
jvm 1    |  INFO | Apache ActiveMQ 5.16.3 (localhost, ID:CZBK-20210302VL-10257-1646035577620-0:1) uptime 0.426 seconds
jvm 1    |  INFO | Apache ActiveMQ 5.16.3 (localhost, ID:CZBK-20210302VL-10257-1646035577620-0:1) is shutdown
jvm 1    |  INFO | Closing org.apache.activemq.xbean.XBeanBrokerFactory$1@2c9392f5: startup date [Mon Feb 28 16:06:16 CST 2022]; root of context hierarchy
jvm 1    |  WARN | Exception encountered during context initialization - cancelling refresh attempt: org.springframework.beans.factory.BeanCreationException: Error creating bean with name 'org.apache.activemq.xbean.XBeanBrokerService#0' defined in class path resource [activemq.xml]: Invocation of init method failed; nested exception is java.io.IOException: Transport Connector could not be registered in JMX: java.io.IOException: Failed to bind to server socket: amqp://0.0.0.0:5672?maximumConnections=1000&wireFormat.maxFrameSize=104857600 due to: java.net.BindException: Address already in use: JVM_Bind
jvm 1    | ERROR: java.lang.RuntimeException: Failed to execute start task. Reason: java.lang.IllegalStateException: BeanFactory not initialized or already closed - call 'refresh' before accessing beans via the ApplicationContext
jvm 1    | java.lang.RuntimeException: Failed to execute start task. Reason: java.lang.IllegalStateException: BeanFactory not initialized or already closed - call 'refresh' before accessing beans via the ApplicationContext
jvm 1    |      at org.apache.activemq.console.command.StartCommand.runTask(StartCommand.java:91)
jvm 1    |      at org.apache.activemq.console.command.AbstractCommand.execute(AbstractCommand.java:63)
jvm 1    |      at org.apache.activemq.console.command.ShellCommand.runTask(ShellCommand.java:154)
jvm 1    |      at org.apache.activemq.console.command.AbstractCommand.execute(AbstractCommand.java:63)
jvm 1    |      at org.apache.activemq.console.command.ShellCommand.main(ShellCommand.java:104)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.apache.activemq.console.Main.runTaskClass(Main.java:262)
jvm 1    |      at org.apache.activemq.console.Main.main(Main.java:115)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.tanukisoftware.wrapper.WrapperSimpleApp.run(WrapperSimpleApp.java:240)
jvm 1    |      at java.lang.Thread.run(Thread.java:748)
jvm 1    | Caused by: java.lang.IllegalStateException: BeanFactory not initialized or already closed - call 'refresh' before accessing beans via the ApplicationContext
jvm 1    |      at org.springframework.context.support.AbstractRefreshableApplicationContext.getBeanFactory(AbstractRefreshableApplicationContext.java:164)
jvm 1    |      at org.springframework.context.support.AbstractApplicationContext.destroyBeans(AbstractApplicationContext.java:1034)
jvm 1    |      at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:555)
jvm 1    |      at org.apache.xbean.spring.context.ResourceXmlApplicationContext.<init>(ResourceXmlApplicationContext.java:64)
jvm 1    |      at org.apache.xbean.spring.context.ResourceXmlApplicationContext.<init>(ResourceXmlApplicationContext.java:52)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory$1.<init>(XBeanBrokerFactory.java:104)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory.createApplicationContext(XBeanBrokerFactory.java:104)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory.createBroker(XBeanBrokerFactory.java:67)
jvm 1    |      at org.apache.activemq.broker.BrokerFactory.createBroker(BrokerFactory.java:71)
jvm 1    |      at org.apache.activemq.broker.BrokerFactory.createBroker(BrokerFactory.java:54)
jvm 1    |      at org.apache.activemq.console.command.StartCommand.runTask(StartCommand.java:87)
jvm 1    |      ... 16 more
jvm 1    | ERROR: java.lang.IllegalStateException: BeanFactory not initialized or already closed - call 'refresh' before accessing beans via the ApplicationContext
jvm 1    | java.lang.IllegalStateException: BeanFactory not initialized or already closed - call 'refresh' before accessing beans via the ApplicationContext
jvm 1    |      at org.springframework.context.support.AbstractRefreshableApplicationContext.getBeanFactory(AbstractRefreshableApplicationContext.java:164)
jvm 1    |      at org.springframework.context.support.AbstractApplicationContext.destroyBeans(AbstractApplicationContext.java:1034)
jvm 1    |      at org.springframework.context.support.AbstractApplicationContext.refresh(AbstractApplicationContext.java:555)
jvm 1    |      at org.apache.xbean.spring.context.ResourceXmlApplicationContext.<init>(ResourceXmlApplicationContext.java:64)
jvm 1    |      at org.apache.xbean.spring.context.ResourceXmlApplicationContext.<init>(ResourceXmlApplicationContext.java:52)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory$1.<init>(XBeanBrokerFactory.java:104)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory.createApplicationContext(XBeanBrokerFactory.java:104)
jvm 1    |      at org.apache.activemq.xbean.XBeanBrokerFactory.createBroker(XBeanBrokerFactory.java:67)
jvm 1    |      at org.apache.activemq.broker.BrokerFactory.createBroker(BrokerFactory.java:71)
jvm 1    |      at org.apache.activemq.broker.BrokerFactory.createBroker(BrokerFactory.java:54)
jvm 1    |      at org.apache.activemq.console.command.StartCommand.runTask(StartCommand.java:87)
jvm 1    |      at org.apache.activemq.console.command.AbstractCommand.execute(AbstractCommand.java:63)
jvm 1    |      at org.apache.activemq.console.command.ShellCommand.runTask(ShellCommand.java:154)
jvm 1    |      at org.apache.activemq.console.command.AbstractCommand.execute(AbstractCommand.java:63)
jvm 1    |      at org.apache.activemq.console.command.ShellCommand.main(ShellCommand.java:104)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.apache.activemq.console.Main.runTaskClass(Main.java:262)
jvm 1    |      at org.apache.activemq.console.Main.main(Main.java:115)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
jvm 1    |      at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
jvm 1    |      at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
jvm 1    |      at java.lang.reflect.Method.invoke(Method.java:498)
jvm 1    |      at org.tanukisoftware.wrapper.WrapperSimpleApp.run(WrapperSimpleApp.java:240)
jvm 1    |      at java.lang.Thread.run(Thread.java:748)
wrapper  | <-- Wrapper Stopped
请按任意键继续. . .
```
##### 整合
​做了这么多springboot整合第三方技术，已经摸到门路了，加坐标，做配置，调接口，直接开工
**步骤①**：导入springboot整合ActiveMQ的starter
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-activemq</artifactId>
</dependency>
```
**步骤②**：配置ActiveMQ的服务器地址
```yaml
spring:
  activemq:
    broker-url: tcp://localhost:61616
  jms:
	pub-sub-domain: true
	template:
	  default-destination: itheima
```
**步骤③**：使用JmsMessagingTemplate操作ActiveMQ
```java
@Service
public class MessageServiceActivemqImpl implements MessageService {
    @Autowired
    private JmsMessagingTemplate messagingTemplate;

    @Override
    public void sendMessage(String id) {
        System.out.println("待发送短信的订单已纳入处理队列，id："+id);
        messagingTemplate.convertAndSend("order.queue.id",id);
    }

    @Override
    public String doMessage() {
        String id = messagingTemplate.receiveAndConvert("order.queue.id",String.class);
        System.out.println("已完成短信发送业务，id："+id);
        return id;
    }
}
```
​==发送消息需要先将消息的类型转换成字符串，然后再发送==，所以是convertAndSend，定义消息发送的位置，和具体的消息内容，此处使用id作为消息内容。

​==接收消息需要先将消息接收到，然后再转换成指定的数据类型==，所以是receiveAndConvert，接收消息除了提供读取的位置，还要给出**转换后的数据的具体类型**。

**步骤④**：使用消息监听器在服务器启动后，监听指定位置，当消息出现后，立即消费消息（这个是发布订阅模式，而非点对点）：
```JAVA
@Component
public class MessageListener {
    @JmsListener(destination = "order.queue.id")
    @SendTo("order.other.queue.id")
    public String receive(String id){
        System.out.println("已完成短信发送业务，id："+id);
        return "new:"+id;
    }
}
```
​使用注解@JmsListener，定义当前方法监听**ActiveMQ中指定名称的消息队列**。

​如果当前消息队列处理完，还需要继续向下传递当前消息（传递的是方法的返回值，这里是 `"new:"+id` ）到另一个队列中，使用注解@SendTo即可，这样即可构造**连续执行的顺序消息队列**。

**步骤⑤**：切换消息模型由点对点模型到发布订阅模型，修改jms配置即可
```yaml
spring:
  activemq:
    broker-url: tcp://localhost:61616
  jms:
    pub-sub-domain: true
```
​pub-sub-domain默认值为false，即点对点模型，修改为true后就是发布订阅模型。

**总结**
1. springboot整合ActiveMQ，**提供了JmsMessagingTemplate对象作为客户端操作消息队列**
2. 操作ActiveMQ需要配置ActiveMQ服务器地址，默认端口61616
3. 企业开发时，通常使用监听器来处理消息队列中的消息，设置监听器使用注解@JmsListener
4. 配置jms的pub-sub-domain属性，可以在点对点模型和发布订阅模型间切换消息模型

#### SpringBoot整合RabbitMQ
​RabbitMQ是MQ产品中的目前较为流行的产品之一，它遵从AMQP协议。RabbitMQ的底层实现语言使用的是Erlang，所以安装RabbitMQ需要先安装Erlang。

**Erlang安装**
​windows版安装包下载地址：[https](https://www.erlang.org/downloads)[://www.erlang.org/downloads](https://www.erlang.org/downloads)。​下载完毕后得到exe安装文件，一键傻瓜式安装，安装完毕需要重启，需要重启，需要重启。

​安装的过程中可能会出现依赖Windows组件的提示，根据提示下载安装即可，都是自动执行的，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251129585.png)
​Erlang安装后需要配置环境变量，否则RabbitMQ将无法找到安装的Erlang。需要配置项如下，作用等同JDK配置环境变量的作用。
- ERLANG_HOME
- PATH

##### 安装
​windows版安装包下载地址：https://rabbitmq.com/install-windows.html。​下载完毕后得到exe安装文件，一键傻瓜式安装，安装完毕后会得到如下文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251129718.png)

**启动服务器**
```CMD
rabbitmq-service.bat start# 启动服务
rabbitmq-service.bat stop# 停止服务
rabbitmqctl status# 查看服务状态
```
​运行sbin目录下的rabbitmq-service.bat命令即可，start参数表示启动，stop参数表示退出，默认对外服务端口5672。

​注意：启动rabbitmq的过程实际上是开启rabbitmq对应的系统服务，需要管理员权限方可执行。

​说明：有没有感觉5672的服务端口很熟悉？**activemq与rabbitmq有一个端口冲突问题，学习阶段无论操作哪一个，请确保另一个处于关闭状态**。

​说明：不喜欢命令行的小伙伴可以使用任务管理器中的服务页，找到RabbitMQ服务，使用鼠标右键菜单控制服务的启停。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251129669.png)
**访问web管理服务**
​RabbitMQ也提供有web控制台服务，但此功能是一个插件，需要先启用才可以使用。
```CMD
rabbitmq-plugins.bat list	# 查看当前所有插件的运行状态
rabbitmq-plugins.bat enable rabbitmq_management# 启动rabbitmq_management插件
```
​启动插件后，可以在插件运行状态中查看是否运行，运行后通过浏览器即可打开服务后台管理界面
```CMD
http://localhost:15672
```
​web管理服务默认端口15672，访问后可以打开RabbitMQ的管理界面，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251129201.png)
​首先输入访问用户名和密码，初始化用户名和密码相同，均为：guest，成功登录后进入管理后台界面，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251129846.png)

##### 整合(direct模型)
​RabbitMQ满足AMQP协议，因此**不同的消息模型对应的制作不同**，先使用最简单的direct模型开发。
**步骤①**：导入springboot整合amqp的starter，amqp协议默认实现为rabbitmq方案
```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-amqp</artifactId>
</dependency>
```
**步骤②**：配置RabbitMQ的服务器地址
```yaml
spring:
  rabbitmq:
    host: localhost
    port: 5672
```
**步骤③**：初始化直连模式系统设置

​由于RabbitMQ不同模型要使用不同的交换机，因此需要先初始化RabbitMQ相关的对象，例如队列，交换机等
```JAVA
@Configuration
public class RabbitConfigDirect {
	@Bean
	public Queue queue(){
		// durable:是否持久化,默认false
		// exclusive:是否当前连接专用，默认false，连接关闭后队列即被删除
		// autoDelete:是否自动删除，当生产者或消费者不再使用此队列，自动删除
		return new Queue("simple_queue",true,false,false);
	}
    @Bean
    public Queue directQueue(){
        return new Queue("direct_queue");
    }
    @Bean
    public Queue directQueue2(){
        return new Queue("direct_queue2");
    }
    @Bean
    public DirectExchange directExchange(){
        return new DirectExchange("directExchange");
    }
    @Bean
    public Binding bindingDirect(){
        return BindingBuilder.bind(directQueue()).to(directExchange()).with("direct");
    }
    @Bean
    public Binding bindingDirect2(){
        return BindingBuilder.bind(directQueue2()).to(directExchange()).with("direct2");
    }
}
```
​队列Queue与直连交换机DirectExchange创建后，还需要绑定他们之间的关系Binding，这样就可以通过交换机操作对应队列。

**步骤④**：使用AmqpTemplate操作RabbitMQ
```java
@Service
public class MessageServiceRabbitmqDirectImpl implements MessageService {
    @Autowired
    private AmqpTemplate amqpTemplate;

    @Override
    public void sendMessage(String id) {
        System.out.println("待发送短信的订单已纳入处理队列（rabbitmq direct），id："+id);
        amqpTemplate.convertAndSend("directExchange","direct",id);
    }
}
```
​amqp协议中的操作API接口名称看上去和jms规范的操作API接口很相似，但是传递参数差异很大。

**步骤⑤**：使用消息监听器在服务器启动后，监听指定位置，当消息出现后，立即消费消息
```JAVA
@Component
public class MessageListener {
    @RabbitListener(queues = "direct_queue")
    public void receive(String id){
        System.out.println("已完成短信发送业务(rabbitmq direct)，id："+id);
    }
}
```
​使用注解@RabbitListener，定义当前方法监听RabbitMQ中指定名称的消息队列。

##### 整合(topic模型)
**步骤①**：同上
**步骤②**：同上
**步骤③**：初始化主题模式系统设置
```JAVA
@Configuration
public class RabbitConfigTopic {
    @Bean
    public Queue topicQueue(){
        return new Queue("topic_queue");
    }
    @Bean
    public Queue topicQueue2(){
        return new Queue("topic_queue2");
    }
    @Bean
    public TopicExchange topicExchange(){
        return new TopicExchange("topicExchange");
    }
    @Bean
    public Binding bindingTopic(){
        return BindingBuilder.bind(topicQueue()).to(topicExchange()).with("topic.*.id");
    }
    @Bean
    public Binding bindingTopic2(){
        return BindingBuilder.bind(topicQueue2()).to(topicExchange()).with("topic.orders.*");
    }
}
```
​主题模式支持routingKey匹配模式，`*` 表示匹配一个单词，#表示匹配任意内容，这样就可以通过主题交换机将消息分发到不同的队列中，详细内容请参看RabbitMQ系列课程。	

| **匹配键**        | **topic.\*.\*** | **topic.#** |
| ----------------- | --------------- | ----------- |
| topic.order.id    | true            | true        |
| order.topic.id    | false           | false       |
| topic.sm.order.id | false           | true        |
| topic.sm.id       | false           | true        |
| topic.id.order    | true            | true        |
| topic.id          | false           | true        |
| topic.order       | false           | true        |

**步骤④**：使用AmqpTemplate操作RabbitMQ
```java
@Service
public class MessageServiceRabbitmqTopicImpl implements MessageService {
    @Autowired
    private AmqpTemplate amqpTemplate;

    @Override
    public void sendMessage(String id) {
        System.out.println("待发送短信的订单已纳入处理队列（rabbitmq topic），id："+id);
        amqpTemplate.convertAndSend("topicExchange","topic.orders.id",id);
    }
}
```
​发送消息后，根据当前提供的routingKey与绑定交换机时设定的routingKey进行匹配，规则匹配成功消息才会进入到对应的队列中。

**步骤⑤**：使用消息监听器在服务器启动后，监听指定队列
```JAVA
@Component
public class MessageListener {
    @RabbitListener(queues = "topic_queue")
    public void receive(String id){
        System.out.println("已完成短信发送业务(rabbitmq topic 1)，id："+id);
    }
    @RabbitListener(queues = "topic_queue2")
    public void receive2(String id){
        System.out.println("已完成短信发送业务(rabbitmq topic 22222222)，id："+id);
    }
}
```
​使用注解@RabbitListener，定义当前方法监听RabbitMQ中指定名称的消息队列。

**总结**
1. springboot整合RabbitMQ提供了AmqpTemplate对象作为客户端操作消息队列
2. 操作ActiveMQ需要配置ActiveMQ服务器地址，默认端口5672
3. 企业开发时通常使用监听器来处理消息队列中的消息，设置监听器使用注解@RabbitListener
4. RabbitMQ有5种消息模型，使用的队列相同，但是交换机不同。交换机不同，对应的消息进入的策略也不同

#### SpringBoot整合RocketMQ
​RocketMQ由阿里研发，后捐赠给apache基金会，目前是apache基金会顶级项目之一，也是目前市面上的MQ产品中较为流行的产品之一，它遵从AMQP协议。

##### 安装
​windows版安装包下载地址：[https://rocketmq.apache.org](https://rocketmq.apache.org/)[/](https://rocketmq.apache.org/)。​下载完毕后得到zip压缩文件，解压缩即可使用，解压后得到如下文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251130607.png)
 ​RocketMQ安装后需要配置环境变量，具体如下：
- ROCKETMQ_HOME
- PATH
- NAMESRV_ADDR （建议）： 127.0.0.1:9876

​关于NAMESRV_ADDR对于初学者来说建议配置此项，也可以通过命令设置对应值，操作略显繁琐，建议配置。系统学习RocketMQ知识后，即可灵活控制该项。
**RocketMQ工作模式**
​在RocketMQ中，处理业务的服务器称为broker，生**产者与消费者不是直接与broker联系的，而是通过命名服务器进行通信**。==broker启动后会通知命名服务器自己已经上线，这样命名服务器中就保存有所有的broker信息==。当生产者与消费者需要连接broker时，通过命名服务器找到对应的处理业务的broker，因此**命名服务器在整套结构中起到一个信息中心的作用**。并且**broker启动前必须保障命名服务器先启动**。
> 跟微服务注册中心概念有点像
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251130507.png)
**启动服务器**
```bash
mqnamesrv # 启动命名服务器
mqbroker # 启动broker
```
​运行bin目录下的mqnamesrv命令，即可启动命名服务器，默认对外服务端口9876。

​运行bin目录下的mqbroker命令，即可启动broker服务器，如果环境变量中没有设置NAMESRV_ADDR，则需要在运行mqbroker指令前通过set指令设置NAMESRV_ADDR的值，并且每次开启均需要设置此项。
**测试服务器启动状态**
​RocketMQ提供有一套测试服务器功能的测试程序，运行bin目录下的tools命令即可使用。
```bash
tools org.apache.rocketmq.example.quickstart.Producer # 生产消息
tools org.apache.rocketmq.example.quickstart.Consumer # 消费消息
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303151940242.png)

##### 整合（异步消息）
**步骤①**：导入springboot整合RocketMQ的starter，此坐标不由springboot维护版本
```xml
<dependency>
    <groupId>org.apache.rocketmq</groupId>
    <artifactId>rocketmq-spring-boot-starter</artifactId>
    <version>2.2.1</version>
</dependency>
```
**步骤②**：配置RocketMQ的服务器地址（不属于Spring下）
```yaml
rocketmq:
  name-server: localhost:9876
  producer:
    group: group_rocketmq
```
​设置默认的生产者消费者所属组group。

**步骤③**：使用RocketMQTemplate操作RocketMQ。
生产同步消息：
```java
@Service
public class MessageServiceRocketmqImpl implements MessageService {
	@Autowired
	private RocketMQTemplate rocketMQTemplate;
	@Override
	public void sendMessage(String id) {
		rocketMQTemplate.convertAndSend("order_sm_id",id);
		System.out.println("使用Rabbitmq将待发送短信的订单纳入处理队列，id："+id);
	}
}
```
生产异步消息：
```java
@Service
public class MessageServiceRocketmqImpl implements MessageService {
    @Autowired
    private RocketMQTemplate rocketMQTemplate;

    @Override
    public void sendMessage(String id) {
        System.out.println("待发送短信的订单已纳入处理队列（rocketmq），id："+id);
        SendCallback callback = new SendCallback() {
            @Override
            public void onSuccess(SendResult sendResult) {
                System.out.println("消息发送成功");
            }
            @Override
            public void onException(Throwable e) {
                System.out.println("消息发送失败！！！！！");
            }
        };
        rocketMQTemplate.asyncSend("order_id",id,callback);
    }
}

```
​使用asyncSend方法发送异步消息。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303151949714.png)

**步骤④**：使用消息监听器在服务器启动后，**监听指定位置，当消息出现后，立即消费消息**：
```JAVA
@Component
@RocketMQMessageListener(topic = "order_id",consumerGroup = "group_rocketmq")
public class MessageListener implements RocketMQListener<String> {
    @Override
    public void onMessage(String id) {
        System.out.println("已完成短信发送业务(rocketmq)，id："+id);
    }
}
```
​RocketMQ的监听器必须按照标准格式开发，**实现RocketMQListener接口，泛型为消息类型**。

​使用注解@RocketMQMessageListener定义当前类监听RabbitMQ中指定组、指定名称的消息队列。

**总结**
1. springboot整合RocketMQ使用RocketMQTemplate对象作为客户端操作消息队列
2. 操作RocketMQ需要配置RocketMQ服务器地址，默认端口9876
3. 企业开发时通常使用监听器来处理消息队列中的消息，设置监听器使用注解@RocketMQMessageListener

#### SpringBoot整合Kafka
##### 安装
​windows版安装包下载地址：[https://](https://kafka.apache.org/downloads)[kafka.apache.org/downloads](https://kafka.apache.org/downloads)。​下载完毕后得到tgz压缩文件，使用解压缩软件解压缩即可使用，解压后得到如下文件
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251130715.png)
windows系统下3.0.0版本存在BUG，建议使用2.X版本。​建议使用windows版2.8.1版本。

**启动服务器**
​kafka服务器的功能相当于RocketMQ中的broker，kafka运行还需要一个类似于命名服务器的服务。在kafka安装目录中自带一个类似于命名服务器的工具，叫做zookeeper，它的作用是注册中心，相关知识请到对应课程中学习。
```bash
zookeeper-server-start.bat ..\..\config\zookeeper.properties# 启动zookeeper
kafka-server-start.bat ..\..\config\server.properties# 启动kafka
```
​运行bin目录下的windows目录下的zookeeper-server-start命令，即可启动注册中心，默认对外服务端口2181。

​运行bin目录下的windows目录下的kafka-server-start命令，即可启动kafka服务器，默认对外服务端口9092。

**创建主题**
​和之前操作其他MQ产品相似，**kakfa也是基于主题操作**，操作之前需要先初始化topic。
```bash
# 创建topic 分区分片
kafka-topics.bat --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic itheima
# 查询topic
kafka-topics.bat --zookeeper 127.0.0.1:2181 --list	
# 删除topic
kafka-topics.bat --delete --zookeeper localhost:2181 --topic itheima
```
**测试服务器启动状态**
​Kafka提供有一套测试服务器功能的测试程序，运行bin目录下的windows目录下的命令即可使用。
```CMD
kafka-console-producer.bat --broker-list localhost:9092 --topic itheima	# 测试生产消息
kafka-console-consumer.bat --bootstrap-server localhost:9092 --topic itheima --from-beginning	# 测试消息消费
```
> 这里报错from is not a recognized option的小伙伴，把from前面的- - 去掉就可以了

##### 整合
**步骤①**：导入springboot整合Kafka的starter，此坐标由springboot维护版本
```xml
<dependency>
    <groupId>org.springframework.kafka</groupId>
    <artifactId>spring-kafka</artifactId>
</dependency>
```
**步骤②**：配置Kafka的服务器地址
```yaml
spring:
  kafka:
    bootstrap-servers: localhost:9092
    consumer:
      group-id: order
```
​设置默认的生产者消费者所属组id。

**步骤③**：使用KafkaTemplate操作Kafka
```java
@Service
public class MessageServiceKafkaImpl implements MessageService {
    @Autowired
    private KafkaTemplate<String,String> kafkaTemplate;

    @Override
    public void sendMessage(String id) {
        System.out.println("待发送短信的订单已纳入处理队列（kafka），id："+id);
        kafkaTemplate.send("itheima2022",id);
    }
}
```
​使用send方法发送消息，需要传入topic名称。

**步骤④**：使用消息监听器在服务器启动后，监听指定位置，当消息出现后，立即消费消息
```JAVA
@Component
public class MessageListener {
    @KafkaListener(topics = "itheima2022")
    public void onMessage(ConsumerRecord<String,String> record){
        System.out.println("已完成短信发送业务(kafka)，id："+record.value());
    }
}
```
​使用注解@KafkaListener，定义当前方法监听Kafka中指定topic的消息，接收到的消息封装在对象ConsumerRecord中，获取数据从ConsumerRecord对象中获取即可。

**总结**
1. springboot整合Kafka使用KafkaTemplate对象作为客户端操作消息队列
2. 操作Kafka需要配置Kafka服务器地址，默认端口9092
3. 企业开发时通常使用监听器来处理消息队列中的消息，设置监听器使用注解@KafkaListener。接收消息保存在形参ConsumerRecord对象中

## KF-6.监控
​在说监控之前，需要回顾一下软件业的发展史。最早的软件完成一些非常简单的功能，代码不多，错误也少。随着软件功能的逐步完善，软件的功能变得越来越复杂，功能不能得到有效的保障，这个阶段出现了针对软件功能的检测，也就是软件测试。伴随着计算机操作系统的逐步升级，软件的运行状态也变得开始让人捉摸不透，出现了不稳定的状况。

伴随着计算机网络的发展，程序也从单机状态切换成基于计算机网络的程序，应用于网络的程序开始出现，由于网络的不稳定性，程序的运行状态让使用者更加堪忧。互联网的出现彻底打破了软件的思维模式，随之而来的互联网软件就更加凸显出应对各种各样复杂的网络情况之下的弱小。计算机软件的运行状况已经成为了软件运行的一个大话题，针对软件的运行状况就出现了全新的思维，建立起了初代的软件运行状态监控。

​什么是监控？就是==通过软件的方式展示另一个软件的运行情况，运行的情况则通过各种各样的指标数据反馈给监控人员==。例如网络是否顺畅、服务器是否在运行、程序的功能是否能够整百分百运行成功，内存是否够用，等等等等。

​本章要讲解的监控就是对软件的运行情况进行监督，但springboot程序与非springboot程序的差异还是很大的，为了方便监控软件的开发，springboot提供了一套功能接口，为开发者加速开发过程。
### KF-6-1.监控的意义
​对于现代的互联网程序来说，规模越来越大，功能越来越复杂，还要追求更好的客户体验，因此要监控的信息量也就比较大了。由于现在的互联网程序大部分都是基于微服务的程序，一个程序的运行需要若干个服务来保障，因此第一个要监控的指标就是服务是否正常运行，也就是**监控服务状态是否宕机**。一旦发现某个服务宕机了，必须马上给出对应的解决方案，避免整体应用功能受影响。

其次，由于互联网程序服务的客户量是巨大的，当客户的请求在短时间内集中达到服务器后，就会出现各种程序运行指标的波动。比如内存占用严重，请求无法及时响应处理等，这就是第二个要监控的重要指标，**监控服务运行指标**。

虽然软件是对外提供用户的访问需求，完成对应功能的，但是后台的运行是否平稳，是否出现了不影响客户使用的功能隐患，这些也是要密切监控的，此时就需要在不停机的情况下，监控系统运行情况，日志是一个不错的手段。如果在众多日志中找到开发者或运维人员所关注的日志信息，简单快速有效的过滤出要看的日志也是监控系统需要考虑的问题，这就是第三个要监控的指标，**监控程序运行日志**。

虽然我们期望程序一直平稳运行，但是由于突发情况的出现，例如服务器被攻击、服务器内存溢出等情况造成了服务器宕机，此时当前服务不能满足使用需要，就要将其重启甚至关闭，如何快速控制服务器的启停，也是程序运行过程中不可回避的问题，这就是第四个监控项，**管理服务状态**。

以上这些仅仅是从大的方面来思考监控这个问题，还有很多的细节点，例如上线了一个新功能，定时提醒用户续费，这种功能不是上线后马上就运行的，但是当前功能是否真的启动，如果快速的查询到这个功能已经开启，这也是监控中要解决的问题，等等。看来监控真的是一项非常重要的工作。

​通过上述描述，可以看出监控很重要。那具体的监控要如何开展呢？还要从实际的程序运行角度出发。比如现在有3个服务支撑着一个程序的运行，每个服务都有自己的运行状态。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251131081.png)
​此时被监控的信息就要在三个不同的程序中去查询并展示，但三个服务是服务于一个程序的运行的，如果不能合并到一个平台上展示，监控工作量巨大，而且信息对称性差，要不停的在三个监控端查看数据。如果将业务放大成30个，300个，3000个呢？看来必须有一个单独的平台，将多个被监控的服务对应的监控指标信息汇总在一起，这样更利于监控工作的开展。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251131583.png)

​新的程序专门用来监控，新的问题就出现了，==是被监控程序主动上报信息还是监控程序主动获取信息==？如果监控程序不能主动获取信息，这就意味着监控程序有可能看到的是很久之前被监控程序上报的信息，万一被监控程序宕机了，监控程序就无法区分究竟是好久没法信息了，还是已经下线了。所以==监控程序必须具有主动发起请求获取被监控服务信息的能力==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251131717.png)
​如果监控程序要监控服务时，主动获取对方的信息。那监控程序如何知道哪些程序被自己监控呢？不可能在监控程序中设置我监控谁，这样互联网上的所有程序岂不是都可以被监控到，这样的话信息安全将无法得到保障。合理的做法只能是在被监控程序启动时上报监控程序，告诉监控程序你可以监控我了。看来==需要在被监控程序端做主动上报的操作，这就要求被监控程序中配置对应的监控程序是谁==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251131423.png)
​被监控程序可以提供各种各样的指标数据给监控程序看，但每一个指标都代表着公司的机密信息，并不是所有的指标都可以给任何人看的，乃至运维人员，所以对被监控指标的是否开放出来给监控系统看，也需要做详细的设定。

​以上描述的整个过程就是一个监控系统的基本流程。

**总结**
1. 监控是一个非常重要的工作，是保障程序正常运行的基础手段
2. 监控的过程通过一个监控程序进行，它汇总所有被监控的程序的信息集中统一展示
3. 被监控程序需要主动上报自己被监控，同时要设置哪些指标被监控

**思考**
​下面就要开始做监控了，新的问题就来了，监控程序怎么做呢？难道要自己写吗？肯定是不现实的，如何进行监控，咱们下节再讲。

### KF-6-2.可视化监控平台
​springboot抽取了大部分监控系统的常用指标，提出了监控的总思想。然后就有好心的同志根据监控的总思想，制作了一个通用性很强的监控系统，因为是基于springboot监控的核心思想制作的，所以这个程序被命名为**Spring Boot Admin**。

==​Spring Boot Admin，这是一个开源社区项目，用于管理和监控SpringBoot应用程序==。这个项目中包含有客户端和服务端两部分，而监控平台指的就是服务端。我们做的程序如果需要被监控，**将我们做的程序制作成客户端**，然后配置服务端地址后，**服务端就可以通过HTTP请求的方式从客户端获取对应的信息，并通过UI界面展示对应信息**。

​下面就开发这套监控程序，先制作服务端，其实服务端可以理解为是一个web程序，收到一些信息后展示这些信息。

**服务端开发**
**步骤①**：导入springboot admin对应的starter，版本与当前使用的springboot版本保持一致，并将其配置成web工程
```xml
<properties>
	<spring-boot-admin.version>2.5.4</spring-boot-admin.version>
</properties>
<dependencies>
	<dependency>
		<groupId>de.codecentric</groupId>
		<artifactId>spring-boot-admin-starter-server</artifactId>
	</dependency>
</dependencies>
<dependencyManagement>
	<dependencies>
		<dependency>
			<groupId>de.codecentric</groupId>
			<artifactId>spring-boot-admin-dependencies</artifactId>
			<version>${spring-boot-admin.version}</version>
			<type>pom</type>
			<scope>import</scope>
		</dependency>
	</dependencies>
</dependencyManagement>
或者
<dependency>
    <groupId>de.codecentric</groupId>
    <artifactId>spring-boot-admin-starter-server</artifactId>
    <version>2.5.4</version>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
​上述过程可以通过创建项目时使用勾选的形式完成。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251131995.png)
Admin服务端：
```java
server:
  port: 8080
```
**步骤②**：在引导类上添加注解@EnableAdminServer，声明当前应用启动后作为SpringBootAdmin的服务器使用
```java
@SpringBootApplication
@EnableAdminServer
public class Springboot25AdminServerApplication {
    public static void main(String[] args) {
        SpringApplication.run(Springboot25AdminServerApplication.class, args);
    }
}
```
​做到这里，这个服务器就开发好了，启动后就可以访问当前程序了，界面如下。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251132306.png)
​由于目前没有启动任何被监控的程序，所以里面什么信息都没有。下面制作一个被监控的客户端程序。

**客户端开发**
​客户端程序开发其实和服务端开发思路基本相似，多了一些配置而已。

**步骤①**：导入springboot admin对应的starter，版本与当前使用的springboot版本保持一致，并将其配置成web工程
```xml
<properties>
	<spring-boot-admin.version>2.5.4</spring-boot-admin.version>
</properties>
<dependencies>
	<dependency>
		<groupId>de.codecentric</groupId>
		<artifactId>spring-boot-admin-starter-client</artifactId>
	</dependency>
</dependencies>
<dependencyManagement>
	<dependencies>
		<dependency>
			<groupId>de.codecentric</groupId>
			<artifactId>spring-boot-admin-dependencies</artifactId>
			<version>${spring-boot-admin.version}</version>
			<type>pom</type>
			<scope>import</scope>
		</dependency>
	</dependencies>
</dependencyManagement>
或者
<dependency>
    <groupId>de.codecentric</groupId>
    <artifactId>spring-boot-admin-starter-client</artifactId>
    <version>2.5.4</version>
</dependency>

<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
​上述过程也可以通过创建项目时使用勾选的形式完成，不过一定要小心，端口配置成不一样的，否则会冲突。

**步骤②**：设置当前客户端将信息上传到哪个服务器上，通过yml文件配置
```yaml
spring:
  boot:
    admin:
      client:
        url: http://localhost:8080
```
​做到这里，这个客户端就可以启动了。启动后再次访问服务端程序，界面如下。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251132203.png)
​可以看到，当前监控了1个程序，点击进去查看详细信息。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251132268.png)
​由于当前**没有设置开放哪些信息给监控服务器**，所以目前看不到什么有效的信息。下面需要做两组配置就可以看到信息了。
1.开放指定信息给服务器看
2.允许服务器以HTTP请求的方式获取对应的信息
   配置如下：
```yaml
server:
  port: 80
spring:
  boot:
    admin:
      client:
        url: http://localhost:8080
management:
  endpoint:
    health:
      show-details: always
  endpoints:
    web:
      exposure:
        include: "*"
```
​上述配置对于初学者来说比较容易混淆。简单解释一下，到下一节再做具体的讲解。**springbootadmin的客户端默认开放了13组信息给服务器，但这些信息除了一个之外，其他的信息都不让通过HTTP请求查看**。所以你看到的信息基本上就没什么内容了，只能看到一个内容，就是下面的健康信息。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251132311.png)
​但即便如此我们看到健康信息中也没什么内容，原因在于健康信息中有一些信息描述了你当前应用使用了什么技术等信息，如果无脑的对外暴露功能会有安全隐患。通过配置就可以开放所有的健康信息明细查看了。
```yaml
management:
  endpoint:
    health:
      show-details: always
```
​健康明细信息如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251132534.png)
​目前除了健康信息，其他信息都查阅不了。原因在于其他12种信息是默认不提供给服务器通过HTTP请求查阅的，所以需要开启查阅的内容项，使用 `*` 表示查阅全部。记得带引号。
```yaml
endpoints:
  web:
    exposure:
      include: "*"
```
​配置后再刷新服务器页面，就可以看到所有的信息了。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251133105.png)​以上界面中展示的信息量就非常大了，包含了13组信息，有性能指标监控，加载的bean列表，加载的系统属性，日志的显示控制等等。

**配置多个客户端**
​可以通过配置客户端的方式，在其他的springboot程序中添加客户端坐标，这样当前服务器就可以监控多个客户端程序了。每个客户端展示不同的监控信息。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251133254.png)
​进入监控面板，如果你加载的应用具有功能，在监控面板中可以看到3组信息展示的，与之前加载的空工程不一样。
- 类加载面板中可以查阅到开发者自定义的类，如左图
	​ ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251133894.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251133101.png)
- 映射中可以查阅到当前应用配置的所有请求
    ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251134597.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251134261.png)
- 性能指标中可以查阅当前应用独有的请求路径统计数据
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251134953.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251134572.png)

**总结**
1. 开发监控服务端需要导入坐标，然后在引导类上添加注解@EnableAdminServer，并将其配置成web程序即可
2. 开发被监控的客户端需要导入坐标，然后配置服务端服务器地址，并做开放指标的设定即可
3. 在监控平台中可以查阅到各种各样被监控的指标，前提是客户端开放了被监控的指标
**思考**
​之前说过，服务端要想监控客户端，需要主动获取到对应信息并展示出来。但是目前我们并没有在客户端开发任何新的功能，但是服务端确可以获取监控信息，谁帮我们做的这些功能呢？咱们下一节再讲。
### KF-6-3.监控原理
**Actuator提供了SpringBoot生产就绪功能，通过端点的配置与访问，获取端点信息**。端点描述了一组监控信息，SpringBoot提供了多个内置端点，也可以根据需要自定义端点信息。​通过查阅监控中的映射指标，可以看到当前系统中可以运行的所有请求路径，其中大部分路径以/actuator开头
- 访问当前应用所有端点信息：/actuator
- 访问端点详细信息：/actuator/端点名称
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251135945.png)
​
首先这些请求路径不是开发者自己编写的，其次这个路径代表什么含义呢？既然这个路径可以访问，就可以通过浏览器发送该请求看看究竟可以得到什么信息。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251135671.png)
​通过发送请求，可以得到一组json信息，如下
```json
{
    "_links": {
        "self": {
            "href": "http://localhost:81/actuator",
            "templated": false
        },
        "beans": {
            "href": "http://localhost:81/actuator/beans",
            "templated": false
        },
        "caches-cache": {
            "href": "http://localhost:81/actuator/caches/{cache}",
            "templated": true
        },
        "caches": {
            "href": "http://localhost:81/actuator/caches",
            "templated": false
        },
        "health": {
            "href": "http://localhost:81/actuator/health",
            "templated": false
        },
        "health-path": {
            "href": "http://localhost:81/actuator/health/{*path}",
            "templated": true
        },
        "info": {
            "href": "http://localhost:81/actuator/info",
            "templated": false
        },
        "conditions": {
            "href": "http://localhost:81/actuator/conditions",
            "templated": false
        },
        "shutdown": {
            "href": "http://localhost:81/actuator/shutdown",
            "templated": false
        },
        "configprops": {
            "href": "http://localhost:81/actuator/configprops",
            "templated": false
        },
        "configprops-prefix": {
            "href": "http://localhost:81/actuator/configprops/{prefix}",
            "templated": true
        },
        "env": {
            "href": "http://localhost:81/actuator/env",
            "templated": false
        },
        "env-toMatch": {
            "href": "http://localhost:81/actuator/env/{toMatch}",
            "templated": true
        },
        "loggers": {
            "href": "http://localhost:81/actuator/loggers",
            "templated": false
        },
        "loggers-name": {
            "href": "http://localhost:81/actuator/loggers/{name}",
            "templated": true
        },
        "heapdump": {
            "href": "http://localhost:81/actuator/heapdump",
            "templated": false
        },
        "threaddump": {
            "href": "http://localhost:81/actuator/threaddump",
            "templated": false
        },
        "metrics-requiredMetricName": {
            "href": "http://localhost:81/actuator/metrics/{requiredMetricName}",
            "templated": true
        },
        "metrics": {
            "href": "http://localhost:81/actuator/metrics",
            "templated": false
        },
        "scheduledtasks": {
            "href": "http://localhost:81/actuator/scheduledtasks",
            "templated": false
        },
        "mappings": {
            "href": "http://localhost:81/actuator/mappings",
            "templated": false
        }
    }
}
```
​其中**每一组数据都有一个请求路径**，而在这里请求路径中有之前看到过的health，发送此请求又得到了一组信息
```JSON
{
    "status": "UP",
    "components": {
        "diskSpace": {
            "status": "UP",
            "details": {
                "total": 297042808832,
                "free": 72284409856,
                "threshold": 10485760,
                "exists": true
            }
        },
        "ping": {
            "status": "UP"
        }
    }
}
```
​当前信息与监控面板中的数据存在着对应关系：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251135405.png)
​原来，**监控中显示的信息实际上是通过发送请求后得到json数据，然后展示出来**。按照上述操作，可以发送更多的以 `/actuator` 开头的链接地址，获取更多的数据，这些数据汇总到一起、组成了监控平台显示的所有数据。

​到这里我们得到了一个核心信息，==监控平台中显示的信息实际上是通过对被监控的应用发送请求得到的==。那这些请求谁开发的呢？打开被监控应用的pom文件，其中导入了springboot admin的对应的client，在这个资源中导入了一个名称叫做actuator的包。**被监控的应用之所以可以对外提供上述请求路径，就是因为添加了这个包**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251135412.png)
​这个actuator是什么呢？这就是本节要讲的核心内容，监控的端点。

​Actuator，可以称为端点，描述了一组监控信息，SpringBootAdmin提供了多个内置端点，通过访问端点就可以获取对应的监控信息，也可以根据需要自定义端点信息。通过发送请求路径 `/actuator` 可以访问应用所有端点信息，如果端点中还有明细信息，可以发送请求 `/actuator/端点名称` 来获取详细信息。以下列出了所有端点信息说明：

| ID               | 描述                                                         | 默认启用 |
| ---------------- | ------------------------------------------------------------ | -------- |
| auditevents      | 暴露当前应用程序的审计事件信息。                             | 是       |
| beans            | 显示应用程序中所有 Spring bean 的完整列表。                  | 是       |
| caches           | 暴露可用的缓存。                                             | 是       |
| conditions       | 显示在配置和自动配置类上评估的条件以及它们匹配或不匹配的原因。 | 是       |
| configprops      | 显示所有 @ConfigurationProperties 的校对清单。               | 是       |
| env              | 暴露 Spring ConfigurableEnvironment 中的属性。               | 是       |
| flyway           | 显示已应用的 Flyway 数据库迁移。                             | 是       |
| health           | 显示应用程序健康信息                                         | 是       |
| httptrace        | 显示 HTTP 追踪信息（默认情况下，最后 100 个  HTTP 请求/响应交换）。 | 是       |
| info             | 显示应用程序信息。                                           | 是       |
| integrationgraph | 显示 Spring Integration 图。                                 | 是       |
| loggers          | 显示和修改应用程序中日志记录器的配置。                       | 是       |
| liquibase        | 显示已应用的 Liquibase 数据库迁移。                          | 是       |
| metrics          | 显示当前应用程序的指标度量信息。                             | 是       |
| mappings         | 显示所有 @RequestMapping 路径的整理清单。                    | 是       |
| scheduledtasks   | 显示应用程序中的调度任务。                                   | 是       |
| sessions         | 允许从 Spring Session 支持的会话存储中检索和删除用户会话。当使用 Spring Session 的响应式 Web 应用程序支持时不可用。 | 是       |
| shutdown         | 正常关闭应用程序。                                           | 否       |
| threaddump       | 执行线程 dump。                                              | 是       |
| heapdump         | 返回一个 hprof 堆 dump 文件。                                | 是       |
| jolokia          | 通过 HTTP 暴露 JMX bean（当  Jolokia 在 classpath 上时，不适用于 WebFlux）。 | 是       |
| logfile          | 返回日志文件的内容（如果已设置 logging.file 或 logging.path 属性）。支持使用 HTTP Range 头来检索部分日志文件的内容。 | 是       |
| prometheus       | 以可以由 Prometheus 服务器抓取的格式暴露指标。               | 是       |
​==上述端点每一项代表被监控的指标，如果对外开放，则监控平台可以查询到对应的端点信息，如果未开放则无法查询对应的端点信息==。通过配置，可以设置端点是否对外开放功能。使用enable属性控制端点是否对外开放。其中health端点为默认端点，不能关闭。
```yaml
management:
  endpoint:
    health: # 端点名称
      show-details: always
    info: # 端点名称
      enabled: true # 是否开放
```
​为了方便开发者快速配置端点，springboot admin设置了13个较为常用的端点作为默认开放的端点，如果需要控制**默认开放的端点**的开放状态，可以通过配置设置，如下：
```YAML
management:
  endpoints:
    enabled-by-default: true	# 是否开启默认端点，默认值true # 启用所有端点
```
​上述端点开启后，就可以通过端点对应的路径查看对应的信息了。但此时还不能通过HTTP请求查询此信息，还需要**开启通过HTTP请求查询的端点名称**，使用 `*` 可以简化配置成「开放**所有端点的WEB端HTTP请求权限**」。
```YAML
management:
  endpoints:
    web:
      exposure:
        include: "*"
```
​整体上来说，对于端点的配置有两组信息，一组是endpoints开头的，对所有端点进行配置；一组是endpoint开头的，对具体端点进行配置。
```YAML
management:
  endpoint: # 具体端点的配置
    health: # 端点名称
      show-details: always
    info:
      enabled: true
  endpoints:	# 全部端点的配置
    web:
      exposure:
        include: "*"
    enabled-by-default: true
```
**总结**
1. 被监控客户端通过**添加actuator的坐标**，可以**对外提供被访问的端点功能**
2. 端点功能的开放与关闭可以通过配置进行控制
3. web端默认无法获取所有端点信息，通过配置开放端点功能
4. 暴露端点功能：端点中包含的信息存在敏感信息，需要对外暴露端点功能时，手动设定指定端点信息
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303152250553.png)
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303152251390.png)

### KF-6-4.自定义监控指标
​端点描述了被监控的信息，除了系统默认的指标，还可以自行添加显示的指标，下面就通过**3种不同的端点指标自定义方式**，来学习端点信息的二次开发。

**INFO端点**
​info端点描述了当前应用的基本信息，可以通过两种形式快速配置info端点的信息
- 配置形式
  在yml文件中通过设置info节点的信息，就可以快速配置端点信息
  ```yaml
  info:
    appName: @project.artifactId@
    version: @project.version@
    company: 传智教育
    author: itheima
  ```
  配置完毕后，对应信息显示在监控平台上
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251135021.png)

  也可以通过请求端点信息路径，获取对应json信息
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251136412.png)

- 编程形式
  ==通过配置的形式只能添加固定的数据==，如果需要动态数据，还可以通过配置bean的方式为info端点添加信息，此信息与配置信息共存
  ```JAVA
  @Component
  public class InfoConfig implements InfoContributor {
      @Override
      public void contribute(Info.Builder builder) {
          builder.withDetail("runTime",System.currentTimeMillis());//添加单个信息
          Map infoMap = new HashMap();
          infoMap.put("buildTime","2006");
          builder.withDetails(infoMap);	//添加一组信息
      }
  }
  ```

**Health端点**
​health端点描述当前应用的运行健康指标，即应用的运行是否成功。通过编程的形式可以扩展指标信息。
```JAVA
@Component
public class HealthConfig extends AbstractHealthIndicator {
    @Override
    protected void doHealthCheck(Health.Builder builder) throws Exception {
        boolean condition = true;
        if(condition) { // 正常加载成功了就显示这组信息
            builder.status(Status.UP);	//设置运行状态为启动状态
            builder.withDetail("runTime", System.currentTimeMillis());
            Map infoMap = new HashMap();
            infoMap.put("buildTime", "2006");
            builder.withDetails(infoMap);
        }else{
            builder.status(Status.OUT_OF_SERVICE);//设置运行状态为不在服务状态
            builder.withDetail("上线了吗？","你做梦");
        }
    }
}
```
​==当任意一个组件状态不为UP时，整体应用对外服务状态为非UP状态==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251136479.png)

**Metrics端点**
​metrics端点描述了性能指标，除了系统自带的监控性能指标，还可以自定义性能指标。
```JAVA
@Service
public class BookServiceImpl extends ServiceImpl<BookDao, Book> implements IBookService {
    @Autowired
    private BookDao bookDao;
    private Counter counter;
    public BookServiceImpl(@Autowired MeterRegistry meterRegistry){
        counter = meterRegistry.counter("用户付费操作次数：");
    }
    @Override
    public boolean delete(Integer id) {
        //每次执行删除业务等同于执行了付费业务
        counter.increment();
        return bookDao.deleteById(id) > 0;
    }
}
```
​在性能指标中，就出现了自定义的性能指标监控项：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251136341.png)

**自定义端点**
​可以根据业务需要自定义端点，方便业务监控
```JAVA
@Component
@Endpoint(id="pay",enableByDefault = true) // 不在这写,就要在配置中写
public class PayEndpoint {
    @ReadOperation
    public Object getPay(){
        Map payMap = new HashMap();
        payMap.put("level 1","300");
        payMap.put("level 2","291");
        payMap.put("level 3","666");
        return payMap;
    }
}
```
​由于此端点数据spring boot admin无法预知该如何展示，所以通过界面无法看到此数据，==通过HTTP请求路径可以获取到当前端点的信息，但需要先开启当前端点对外功能，或者设置当前端点为默认开发的端点==。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302251136520.png)

**总结**
1. 端点的指标可以自定义，但是每种不同的指标根据其功能不同，自定义方式不同
2. info端点通过配置和编程的方式都可以添加端点指标
3. health端点通过编程的方式添加端点指标，需要注意要为对应指标添加启动状态的逻辑设定
4. metrics指标通过在业务中添加监控操作设置指标
5. 可以自定义端点添加更多的指标

---
## 开发实用篇完结
​开发实用篇到这里就暂时完结了，在开发实用篇中我们讲解了大量的第三方技术的整合方案，选择的方案都是市面上比较流行的常用方案，还有一些国内流行度较低的方案目前还没讲，留到番外篇中慢慢讲吧。

​整体开发实用篇中讲解的内容可以分为两大类知识：实用性知识与经验性知识。

​实用性知识就是新知识了，springboot整合各种技术，每种技术整合中都有一些特殊操作，整体来说其实就是三句话。加坐标做配置调接口。经验性知识是对前面两篇中出现的一些知识的补充，在学习基础篇时如果将精力放在这些东西上就有点学偏了，容易钻牛角尖，放到实用开发篇中结合实际开发说一些不常见的但是对系统功能又危害的操作解决方案，提升理解。

​开发实用篇做到这里就告一段落，下面就要着手准备原理篇了。市面上很多课程原理篇讲的过于高深莫测，在新手还没明白123的时候就开始讲微积分了，着实让人看了着急。至于原理篇我讲成什么样子？一起期待吧。