
# 1. Maven基础
## 1.1 Maven简介
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160029715.png)

传统项目管理状态分析：组件升级或变动（==要把源代码放到服务器上，重新编译，重新运行、测试、打包、部署==）。
- JAR包不统一，JAR包不兼容
- 工程升级维护过程操作繁琐（本地机和服务器环境不一致，虚拟机镜像不是很合适，虚拟机内存小……）
- ...

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160032986.png)
==Apache Maven的本质是一个项目管理和构建工具，是专门用于管理和构建Java项目的工具（**蓝色方框中的东西**），它将项目开发和管理过程抽象成一个项目对象模型== *Project Object Model, POM* ，<font color="red"><b>通过一小段描述信息来管理项目的构建、报告和文档</b></font>，对应的 `pom.xml` 配置文件代表一个项目：
pom.xml -> **项目对象模型POM** <--> **依赖管理** Dependency, requires and exports -> 本地仓库 -> 私服仓库 -> 中央仓库 依赖管理的东西是从中央仓库拿来的。
> 项目对象模型和依赖管理的双向箭头说明，我们不仅能用依赖管理来管理自己的项目，自己的项目以后还能作为其他项目的依赖来被Maven管理。

除了依赖管理，还能做项目构建和管理，这个依赖于"**构建生命周期/阶段** " --> 要用到一些**插件**（Maven做好了无数个插件，帮你完成这个部分），插件会产生一些结果如JAR包、源代码、XML等等。

Maven的主要功能如下：
- **依赖管理**：提供了一套依赖管理机制，能方便快捷地管理项目依赖的资源（管JAR包），避免资源间的版本冲突问题
- **项目构建**：提供了一套标准化的、跨平台的、自动化的项目构建流程（编译、测试、打包、发布）
- **统一开发结构**：提供了一套标准化的、统一的项目结构，无论是IDEA、还是Eclipse、NetBeans、Web项目，都能用这个结构

### 标准化的项目结构
项目结构我们都知道，每一个开发工具（IDE）都有自己不同的项目结构，它们互相之间不通用。在Eclipse中创建的目录，无法在IDEA中进行使用，这就造成了很大的不方便。 

而Maven提供了一套标准化的项目结构，**所有的IDE使用Maven构建的项目完全一样**，所以IDE创建的Maven项目可以通用。如下图就是Maven构建的项目结构。如果是Java项目，则 `webapp` 目录可以不要：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221148754.png)
### 标准化的构建流程 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221154288.png)

如上图所示我们开发了一套系统，代码需要进行编译、测试（`test` 目录）、打包、发布，这些操作如果需要反复进行（对于大型项目）就显得特别麻烦，而Maven提供了一套简单的命令来完成项目构建——在IDE中，可能需要插件或专业版。
### 依赖管理
**依赖管理其实就是管理你项目所依赖的第三方资源（JAR包、插件）**。如之前我们项目中需要使用JDBC和Druid的话，就需要去网上下载对应的依赖包（之前是已经下载好提供给大家了）、复制到项目中，还要将 `JAR` 包加入工作环境这一系列的操作。如下图所示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221157074.png)

而Maven使用标准的 **坐标** 配置来管理各种依赖，只需要简单的配置就可以完成依赖管理。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221159456.png)

如上图右边所示就是mysql驱动包的坐标，在项目中只需要写这段配置，其他都不需要我们担心，Maven都帮我们进行下载/管理/删除第三方资源。

市面上有很多构建工具，而Maven依旧还是主流构建工具，如下图是常用构建工具的使用占比——Ant已经被Maven与Gradle取代了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221201219.png)

---
## 1.2 下载安装与IDEA使用
### 1.2.1 下载安装
官网：http://maven.apache.org/
下载地址：http://maven.apache.org/download.cgi
https://mvnrepository.com/ 这个网站只提供maven仓库包的索引与查询并不提供下载，从这个网站添加的依赖包还是需要到原网站下载

我们运行Maven的时候，实际上是在 `bin` 目录下运行 `mvn.cmd` ，其中的代码说明：需要JAVA_HOME和MAVEN_HOME。解压 `apache-maven-3.6.1.rar` 就安装完成了。解压缩后的目录结构如下：
![图|320x170](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221355287.png)
- `bin` 目录 ： 存放的是可执行命令。`mvn` 命令重点关注。
- `conf` 目录 ：存放Maven的配置文件。`settings.xml` 配置文件后期需要修改。
- `lib` 目录 ：存放Maven依赖的JAR包。Maven也是使用Java开发的，所以它也依赖其他的JAR包。
- `boot` 目录是类加载器，因为Maven是Java写的，它定义了自己的类加载器（？）。

依赖Java，需要配置 `JAVA_HOME` 。设置Maven自己的运行环境，配置 `MAVEN_HOME` 为安装路径，然后在Path变量中输入 `%MAVEN_HOME%\bin`。打开命令提示符进行验证，出现如图所示表示安装成功：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221356176.png)
配置本地仓库——修改 `conf/settings.xml` 中的内容，指定一个目录作为本地仓库，用来存储JAR包。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221357321.png)
配置阿里云私服——中央仓库在国外，所以下载JAR包速度可能比较慢。Maven默认连接的仓库位置如下：
```xml
<repositories>
	<repository>
		<id>central</id> <!-- 中央仓库 -->
		<name>Central Repository</name>
		<url>https://repo.maven.apache.org/maven2</url>
		<layout>default</layout>
		<snapshots>
			<enabled>false</enabled>
		</snapshots>
	</repository>
</repositories>
```
而阿里公司提供了一个远程仓库，里面基本也都有开源项目的JAR包。修改 `conf/settings.xml` 中的标签，为其添加如下子标签，配置阿里云镜像仓库：
```xml
<mirrors>
	<!-- 配置具体的仓库的下载镜像  -->
	<mirror>
		<!-- 此镜像的唯一标识符，用来区分不同的mirror  -->
		<id>alimaven</id>
		<!-- 对哪种仓库镜像，简单说就是替代哪个仓库  -->
		<mirrorOf>central</mirrorOf>
		<!-- 镜像名称 -->
		<name>aliyun maven</name>
		<!-- 镜像URL -->
		<url>http://maven.aliyun.com/nexus/content/groups/public/</url>
	</mirror>
	
</mirrors>
```
> 注意：全局Settings和用户Settings的区别：前者定义了当前计算机中Maven的公共配置；后者定义了当前用户的配置。

```xml
<mirror>
  <id>aliyunmaven</id>
  <mirrorOf>*</mirrorOf>
  <name>阿里云公共仓库</name>
  <url>https://maven.aliyun.com/repository/public</url>
</mirror>

<mirror>
		<id>jboss-public-repository-group</id>
		<mirrorOf>central</mirrorOf>
		<name>JBoss Public Repository Group</name>
		<url>http://repository.jboss.org/nexus/content/groups/public</url>
	</mirror>
```
### 1.2.2 IDEA中使用Maven
以后开发中肯定会在高级开发工具中使用Maven管理项目，而常用的高级开发工具是IDEA，所以接下来讲解Maven在IDEA中的使用。我们需要先在IDEA中配置Maven环境：
- 选择IDEA中File --> Settings：
	![图|300x450](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301222356571.png)
- 搜索Maven，设置IDEA使用本地安装的Maven，并修改配置文件路径：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301222358984.png)

IDEA创建Maven项目也很简单：创建模块，选择Maven，点击Next
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230008055.png)
填写模块名称，坐标信息，点击finish，创建完成：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230009098.png)
创建好的项目目录结构如下：
![图|360x220](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230010981.png)

在IDEA中导入其他人写的Maven项目，也很简单：选择右侧Maven面板，点击+号
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230011118.png)
再选中对应项目的 `pom.xml` 文件，双击即可：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230012991.png)
如果没有Maven面板，选择View --> Appearance --> Tool Window Bars：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230012960.png)
可以通过下图所示进行命令的操作：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230012985.png)
### 1.2.3 配置Maven-Helper插件
选择IDEA中File --> Settings
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230014694.png)
选择Plugins：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230017945.png)
搜索Maven，选择第一个Maven Helper，点击Install安装，弹出面板中点击Accept。最后重启IDEA：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230018452.png)

安装完该插件后可以通过选中项目右键进行相关命令操作，如下图所示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230020426.png)

---
# 1.3 第一个Maven项目
## 1.3.1 手工制作
Maven工程目录结构如下（`resources` 中写配置文件；测试程序也有自己专用的配置文件，要单独地创建和存储）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160130425.png)
在 `src` 同层目录下创建 `pom.xml` ，自己写是不太现实的，可以打开Maven安装目录/lib，随便打开一个JAR包，查找POM文件，复制一个简单的然后改一改：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project
	xmlns="http://maven.apache.org/POM/4.0.0"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
	<modelVersion>4.0.0</modelVersion>  <!-- Maven对象模型的版本号,与Maven版本无关 -->
	<groupId>com.itheima</groupId>
	<artifactId>project-java</artifactId>
	<version>1.0</version>
	<packaging>jar</packaging>
	
	<dependencies>
		<dependency>
			<groupId>junit</groupId>
			<artifactId>junit</artifactId>
			<version>4.12</version>
		</dependency>
	</dependencies>
</project>
```
在 `src/main/java/com/itheima` 中创建如下 `Demo.java` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160106786.png)
接着在 `src/test/java/com/itheima` 中创建 `DemoTest.java` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160109154.png)
为了让它们运行，在 `pom.xml` 文件夹中进入CMD，运行 `mvn compile` 开始编译（先下载依赖和插件，然后开始编译）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160121566.png)
显示 `BUILD SUCCESS` 说明编译成功，并且生成一个 `target` 目录，放编译完成后的生成物：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160123676.png)
运行 `mvn test` 结果如下，还会生成详细报告（TXT和XML文件），说明运行测试环境等：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160124878.png)
运行 `mvn package` 可以打包项目，里面先编译、测试、再打包。运行 `mvn install` 会将打包的项目放在仓库中，`groupid` 和项目名、版本号决定了其放置的目录层级。
## 1.3.2 插件创建工程
创建工程，使用模板生成：
```bash
mvn archetype:generate
	-DgroupId={project-packaging}
	-DartifactId={project-name}
	-DarchetypeArtifactId=maven-archetype-quickstart # 说明使用哪个模板
	-DinteractiveMode=false
```
创建Java工程：
```bash
mvn archetype:generate 
	-DgroupId=com.itheima 
	-DartifactId=java-project 
	-DarchetypeArtifactId=maven-archetype-quickstart 
	-Dversion=0.0.1-snapshot 
	-DinteractiveMode=false
```
到一个空目录中，创建时要求里面不是一个Maven项目结构，运行如上命令后结果如下，多了一个 `java-project` 的Maven工程：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160143573.png)
创建Web工程：
```bash
mvn archetype:generate 
	-DgroupId=com.itheima 
	-DartifactId=web-project 
	-DarchetypeArtifactId=maven-archetype-webapp 
	-Dversion=0.0.1-snapshot 
	-DinteractiveMode=false
```

## 1.3.3 IDEA生成和使用骨架
IDEA对Maven 3.6.2及以上版本存在兼容性问题，为避免冲突，IDEA中安装使用3.6.1版本（？现在存疑）。

配置Maven：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160150004.png)
进入Project Structure-Modules看到如下界面，手工创建Java项目：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160151716.png)
要修改项目结构，注意颜色（或者使用Mark Directory as）：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160155476.png)
可以在项目中运行：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161236194.png)
还可以配置Maven运行环境：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161237240.png)

如果用骨架创建Java工程，则使用 `quick-start` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161256810.png)
区别在于，`main, test` 下都没有 `resources` 目录、且没有颜色，可以手工添加：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161257360.png)

如果用骨架创建Web工程，使用 `webapp` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161300634.png)
区别在于，`main` 下没有 `java` 目录，而且没有 `test` 目录、且没有颜色，可以手工添加。

## 1.3.4 POM中安装Tomcat Maven插件
[[../Java Web/Web核心：Tomcat+Servlet+HTTP+Request+Response]]中应该也说过这一点。
```xml
<build>
	<plugins>
		<plugin>
			<groupId>org.apache.tomcat.maven</groupId>
			<artifactId>tomcat7-maven-plugin</artifactId>
			<version>2.1</version>
			<configuration>
				<port>80</port>
				<path>/</path>
			</configuration>
		</plugin>
	</plugins>
</build>
```

---
## 1.4 Maven模型
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221208579.png)
- **项目对象模型** `Project Object Model` ：通过一小段描述信息来管理项目的构建、报告和文档，写到 `pom.xml` 配置文件中，该配置文件中 `groupId-artifactId-version` 是该项目的唯一标识，即坐标。
- **依赖管理模型** `Dependency` ：通过不同项目的不同坐标，可以导入（从本地仓库或网络仓库）其他依赖，提供了方便的依赖管理功能。
- 插件 `Plugin` 和构建生命周期/阶段。

如上图所示就是Maven的模型，而我们先看紫色框框起来的部分，就是用来完成**标准化构建流程**。如我们需要编译，Maven提供了一个编译插件供我们使用，我们需要打包，Maven就提供了一个打包插件提供我们使用等。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221218320.png)
上图中紫色框起来的部分，**项目对象模型就是将我们自己抽象成一个对象模型，有自己专属的坐标**，如下图所示是一个Maven项目：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221218484.png)
**依赖管理模型则是使用坐标来描述当前项目依赖哪些第三方JAR包**，如下图所示：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221219479.png)
上述Maven模型图中还有一部分是仓库。如何理解仓库呢？

---
## 1.5 Maven仓库（存储资源，包含各种JAR包）与坐标
大家想想这样的场景，**我们创建Maven项目，在项目中使用坐标来指定项目的依赖**，那么依赖的JAR包到底存储在什么地方呢？其实依赖JAR包是存储在我们的本地仓库中。而项目运行时从本地仓库中拿需要的依赖JAR包。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302160047537.png)
仓库分类：
- 远程仓库：非本机电脑上的仓库，为本地仓库提供资源
	- **中央仓库**：由Maven团队维护的全球唯一的仓库，存储所有资源的仓库。地址： https://repo1.maven.org/maven2/
	- 私服：一般由公司团队搭建的私有仓库，在公司范围内存储资源，从中央仓库获取资源
	   作用：访问速度快，多个本地仓库访问下载中央仓库，不如中间添加一个私服仓库（基本上是局域网级别）；
		保存具有版权的资源，包含购买或自主研发的JAR；中央仓库中的JAR都是开源的，不能存储具有版权的资源；
		一定范围内共享资源，仅对内部开放，不对外共享。
- 本地仓库：自己电脑上存储资源的仓库，是自己计算机上的一个目录，连接远程仓库获取资源
> 只学习远程仓库的使用，并不会搭建。

当项目中使用坐标引入对应依赖JAR包后，首先会查找本地仓库中是否有对应的JAR包：如果有，则在项目直接引用；如果没有，则去中央仓库中下载对应的JAR包到本地仓库。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221333731.png)
如果还可以搭建远程仓库，将来JAR包的查找顺序则变为：本地仓库 --> 远程仓库（私服）--> 中央仓库。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221335357.png)

对于坐标，什么是坐标？==**Maven中的坐标是资源的唯一标识**，用于描述仓库中资源的位置。我们使用坐标来定义项目或引入项目中需要的依赖==。Maven坐标主要组成：
- `groupId` ：组织ID，定义当前Maven项目所属组织名称（通常是反向域名，如org.mybatis）
- `artifactId` ：项目ID，定义当前Maven项目名称（通常是模块名称，例如order-service、goods-service）
- `version` ：定义当前项目版本号
- `packaging` ：定义该项目的打包方式

如下图就是使用坐标表示一个项目：
![图|300x100](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230006136.png)
> 注意：上面所说的资源可以是插件、依赖、当前项目。我们的项目如果被其他的项目依赖时，也是需要坐标来引入的。

Maven坐标的作用：使用唯一标识，唯一性定位资源位置，通过该标识可以将资源的识别号与下载工作交由机器完成。

---
# 1.6 依赖管理
## 1.6.1 使用坐标引入JAR包
使用坐标引入JAR包的步骤：
- 在项目的 `pom.xml` 中编写标签
- 在标签 `<dependency></dependency>` 中引入坐标
- 定义坐标的 `groupId, artifactId, version` 
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230022145.png)
- 点击刷新按钮，使坐标生效
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230113221.png)

> 注意：具体的坐标我们可以到如下网站进行搜索：https://mvnrepository.com/。

**快捷方式导入JAR包的坐标**：每次需要引入JAR包，都去对应的网站进行搜索是比较麻烦的，接下来给大家介绍一种快捷引入坐标的方式：
- 在 `pom.xml` 中按Alt + Insert，选择 `Dependency` ：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230114778.png)

- 在弹出的面板中搜索对应坐标，然后双击选中对应坐标：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230117989.png)
- 点击刷新按钮，使坐标生效

**自动导入设置**：上面每次操作都需要点击刷新按钮，让引入的坐标生效。当然我们也可以通过设置让其自动完成：
- 选择IDEA中File --> Settings ：
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230118742.png)
- 在弹出的面板中找到Build Tools，选择Any changes，勾选即可生效
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230119891.png)
## 1.6.2 依赖范围
依赖的JAR默认情况下可在任何地方使用，但通过设置坐标的依赖范围 `scope` ，可以设置对应JAR包的作用范围：**编译环境、测试环境、运行环境**。==运行环境就是，将来这个Java/Web项目要被打包成JAR/WAR包来运行，JAR/War包中有无对应的依赖JAR包==。 作用范围：
- 主程序范围有效（ `main` 文件夹范围内）
- 测试程序范围有效（ `test` 文件夹范围内）
- 是否参与打包（`package` 指令范围内）

如下图所示，给 `junit` 依赖通过 `scope` 标签指定依赖的作用范围。 那么这个依赖就只能作用在测试环境，其他环境下不能使用：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230119085.png)
那么 `scope` 都可以有哪些取值呢？
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301230120348.png)
- `compile` ：作用于编译环境、测试环境、运行环境。
- `test` ： 作用于测试环境。典型的就是Junit坐标，**以后使用Junit时，都会将 `scope` 指定为该值**
- `provided` ：作用于编译环境、测试环境。我们后面会**学习 `servlet-api` ，在使用它时，必须将 `scope` 设置为该值，不然运行时就会报错**——打包进去会出现Servlet冲突。
- `runtime` ： 作用于~~测试环境、~~（这边好像写错了吗）运行环境。**JDBC驱动一般将 `scope` 设置为该值**，当然不设置也没有任何问题

> 注意：如果引入坐标不指定 `scope` 标签时，默认就是 `compile` 值。以后大部分JAR包都是使用默认值。

## 1.6.3 依赖传递
依赖具有传递性：
- 直接依赖：在当前项目中，通过依赖配置建立的依赖关系
- 间接依赖：依赖的资源如果依赖其他资源，则当前项目间接依赖其他资源
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161642714.png)

比如，对于 `project03` ，将它作为资源引入 `project02` ，就是将 `project03` 的坐标写入 `project02` 的 `pom.xml` 的 `<dependency>` 中，刷新以后在 `project02/Dependencies`中可以看得 `project03` 这个包：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161644480.png)
如果在 `project03` 的 `pom.xml` 中导入 `log4j, junit` ，会发现 `project02/Dependencies` 中、`project03` 下出现 `log4j, junit` ，这说明 `project03` 依赖这两个包，**于是 `project02` 间接依赖这两个包，也可以使用这两个包**：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161647141.png)
依赖传递出现冲突问题：
- 路径优先：当依赖中出现相同的资源时，**层级越深，优先级越低**，层级越浅，优先级越高；
- 声明优先：**当资源在相同层级被依赖时**（但不在同个配置文件中声明；大于1度），配置顺序靠前的覆盖配置顺序靠后的
- 特殊优先：当同个配置文件中配置了相同资源的不同版本，后配置的覆盖先配置的

依赖管理中**可选依赖**——指对外隐藏当前所依赖的资源（不透明）：
```xml
<dependency>
	<groupId>junit</groupId>
	<artifactId>junit</artifactId>
	<version>4.12</version>
	<optional>true</optional>
</dependency>
```
还可**排除依赖**——指主动断开间接依赖的资源，被排除的资源无需指定版本——你传递过来，我不需要。
```xml
<dependency>
	<groupId>junit</groupId>
	<artifactId>junit</artifactId>
	<version>4.12</version>
	<exclusions>
		<exclusion> <!-- 要排除掉的junit依赖的资源 -->
			<groupId>org.hamcrest</groupId>
			<artifactId>hamcrest-core</artifactId>
		</exclusion>
	</exclusions>
</dependency>
```

## 1.6.4 依赖范围传递性
带有依赖范围的资源，在进行传递时作用范围将受到影响：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161707369.png)
~~如果1度项目使用 `runtime` 范围导入坐标，则0度项目中无论使用何种范围导入1度项目，其范围和1度项目导入范围取交集，有交集取交集，无交集取大的范围~~。因为1度项目中使用 `test, provided`（**不能进行依赖传递**），则没有将间接依赖打进包里。所以0度项目无论使用何种范围导入1度项目，都不能用1度项目的依赖。

---
# 1.7 Maven基本使用
## 1.7.1 Maven常用命令
Maven命名使用 `mvn` 开头，后面添加功能参数，可以一次执行多个命令，使用空格分隔：
- compile：编译
- clean：清理
- test：测试
- package：打包
- install：安装
 
如下所示，`maven-project` 提供了一个使用Maven构建的简单项目，项目结构如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221406835.png)
而我们使用上面命令需要在磁盘上进入到项目的 `pom.xml` 目录下，打开命令提示符：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221410018.png)

**编译命令演示**：执行下列命令可以看到，
- 从阿里云下载编译需要的插件的JAR包，在本地仓库也能看到下载好的插件
- 在项目下会生成一个 `target` 目录
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221415835.png)
同时在项目下会出现一个 `target` 目录，编译后的字节码文件就放在该目录下：
![图|450x150](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221420957.png)

**清理命令演示**：执行下列命令可以看到，
- 从阿里云下载清理需要的插件JAR包
- 删除项目下的 `target` 目录
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221422983.png)

**打包命令演示**：执行下列命令可以看到，
- 从阿里云下载打包需要的插件JAR包
- 在项目的 `target` 目录下有一个JAR包（将当前项目打成的JAR包）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221427198.png)

**测试命令演示**：该命令会执行所有的测试代码。执行上述命令效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221428647.png)

**安装命令演示**：该命令会将当前项目打成JAR包，并安装到本地仓库。执行完上述命令后到本地仓库查看结果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301221430962.png)
## 1.7.2 Maven生命周期与插件
==Maven项目构建生命周期描述的是**一次构建过程经历经历了多少个事件**==。Maven对项目构建的生命周期划分为3套：
- clean ：清理工作 pre-clent -> clean -> post-clean。
- default ：**核心工作，例如编译，测试，打包，安装等** compile -> test -> package -> install。
- site ： 产生报告，发布站点等。这套声明周期一般不会使用 pre-site -> site -> post->site。

==同一套生命周期内，执行后边的命令，前面的所有命令会自动执行==。例如默认（default）生命周期如下：
![图|320x80](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202301222348817.png)
- 当我们执行 `install`（安装）命令时，它会先执行 `compile` 命令，再执行 `test` 命令，再执行 `package` 命令，最后执行 `install` 命令。
- 当我们执行 `package`（打包）命令时，它会先执行 `compile` 命令，再执行 `test` 命令，最后执行 `package` 命令。

实际上，默认构建生命周期包含如下这些事件，要执行 `compile` ，就要从上到下 `validate -> initialize -> ... -> compile` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161736431.png)
`site` 构建生命周期包括如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302161736454.png)

Maven生命周期中的事件都由谁来执行呢？由插件。==插件与生命周期内的阶段绑定，在执行到对应生命周期时执行对应的插件功能==。默认Maven在各个生命周期上绑定有预设的功能，**通过插件可以自定义其他功能**。
```xml
<build>
	<plugins>
		<plugin>
			<groupId>org.apache.maven.plugins</groupId>
			<artifactId>maven-source-plugin</artifactId>
			<version>2.2.1</version>
			<executions>
				<execution>
					<goals>
						<goal>jar</goal> <!-- 要对什么操作,可以设置多个goal -->
					</goals>
					<phase>generate-test-resources</phase> <!-- 标记这个插件在什么时候执行 -->
				</execution>
			</executions>
		</plugin>
	</plugins>
</build>
```
每个插件的用法都不同，见Maven官网的Plugins。

--- 
