> * 理解并实现**分模块开发**
> * 能够使用**聚合工程**快速构建项目
> * 能够使用**继承**简化项目配置
> * 能够根据需求**配置生成、开发、测试环境**，并在各个环境间切换运行
> * 了解Maven的私服

---
# 1. 分模块开发
## 1.1 分模块开发设计
(1) 按照功能拆分
我们现在的项目都是在一个模块中，比如前面的SSM整合开发。虽然这样做功能也都实现了，但也存在了一些问题，拿银行的项目为例来聊聊这个事。
* 网络没有那么发达时，我们需要到银行柜台或者取款机进行业务操作
* 随着互联网的发展，我们有了电脑以后，就可以在网页上登录银行网站使用U盾进行业务操作
* 再来就是随着智能手机的普及，只需用手机登录APP就可以进行业务操作

上面三个场景出现的时间是不相同的，如果非要把三个场景的模块代码放入到一个项目，那么当其中某一个模块代码出现问题，就会导致整个项目无法正常启动，从而导致银行的多个业务都无法正常办理。所以我们会==按照功能==将项目进行拆分。

(2) 按照模块拆分
比如电商的项目中，有订单和商品两个模块，订单中需要包含商品的详细信息，所以需要商品的模型类，商品模块也会用到商品的模型类，这个时候如果两个模块中都写模型类，就会出现重复代码，后期的维护成本就比较高。我们就想==能不能将它们公共的部分抽取成一个独立的模块，其他模块要想使用，可以像添加第三方JAR包依赖一样、来使用我们自己抽取的模块==，这样就解决了代码重复的问题,这种拆分方式就说我们所说的==按照模块==拆分。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221928865.png)
经过两个案例的分析，我们就知道：分模块开发就是，将原始模块按照功能拆分成若干个子模块，能方便模块间的相互调用，接口共享。

刚刚我们说了可以将 `domain` 层进行拆分，除了 `domain` 层，我们也可以将其他的层也拆成一个个对立的模块，如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221928241.png)
这样的话，**项目中的每一层都可以单独维护，也可以很方便的被别人使用**。关于分模块开发的意义，我们就说完了，说了这么多好处，那么该如何实现呢？

## 1.2 分模块开发实现
前面我们已经完成了SSM整合，接下来，咱们就基于SSM整合的项目来实现对项目的拆分。
### 1.2.1 环境准备
将 `\maven_02_ssm` 部署到IDEA中，将环境快速准备好，部署成功后，项目的格式如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221935688.png)
### 1.2.2 抽取 `domain` 层
步骤1：创建新模块。创建一个名称为 `maven_03_pojo` 的JAR项目,为什么项目名是从02到03这样创建，原因后面我们会提到，这块的名称可以任意。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221936158.png)

步骤2：项目中创建 `domain` 包。在 `maven_03_pojo` 项目中创建 `com.itheima.domain` 包，并将` maven_02_ssm` 中 `Book` 类拷贝到该包中。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221937843.png)

步骤3：删除原项目中的 `domain` 包。删除后，`maven_02_ssm` 项目中用到 `Book` 的类中都会有红色提示，如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221938009.png)
**说明**：出错的原因是 `maven_02_ssm` 中已经将 `Book` 类删除，所以该项目找不到 `Book` 类，所以报错。要想解决上述问题，我们需要在 `maven_02_ssm` 中添加 `maven_03_pojo` 的依赖。

步骤4：**建立依赖关系**。在 `maven_02_ssm` 项目的 `pom.xml` 添加 `maven_03_pojo` 的依赖
```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>maven_03_pojo</artifactId>
    <version>1.0-SNAPSHOT</version>
</dependency>
```
因为添加了依赖，所以在 `maven_02_ssm` 中就已经能找到 `Book` 类，所以刚才的报红提示就会消失。

步骤5：编译 `maven_02_ssm` 项目。编译 `maven_02_ssm` 你会在控制台看到如下错误。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221942597.png)
错误信息为：不能解决 `maven_02_ssm` 项目的依赖问题，找不到 `maven_03_pojo` 这个JAR包。为什么找不到呢？==原因是Maven会从本地仓库找对应的包，但是本地仓库又不存在该JAR包所以会报错==。

在IDEA中是有 `maven_03_pojo` 这个项目，所以**我们只需要将 `maven_03_pojo` 项目安装到本地仓库即可**。

步骤6：将项目安装本地仓库。将需要被依赖的项目 `maven_03_pojo` ，使用maven的 `install` 命令，把其安装到Maven的本地仓库中。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221943154.png)
安装成功后，在对应的路径下就看到安装好的JAR包。

**说明**：具体安装在哪里，和你们自己电脑上Maven的本地仓库配置的位置有关。当再次执行 `maven_02_ssm` 的 `compile` 的命令后，就已经能够成功编译。
### 1.2.3 抽取 `Dao` 层
步骤1：创建新模块。创建一个名称为`maven_04_dao`的JAR项目。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221948820.png)
步骤2：项目中创建 `dao` 包。在 `maven_04_dao` 项目中创建 `com.itheima.dao` 包，并将 `maven_02_ssm` 中 `BookDao` 类拷贝到该包中。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221949478.png)
在 `maven_04_dao` 中会有如下几个问题需要解决下： 
* 项目 `maven_04_dao` 的 `BookDao` 接口中 `Book` 类找不到报错
	* 解决方案在 `maven_04_dao` 项目的 `pom.xml` 中添加 `maven_03_pojo` 项目
    ```xml
    <dependencies>
        <dependency>
            <groupId>com.itheima</groupId>
            <artifactId>maven_03_pojo</artifactId>
            <version>1.0-SNAPSHOT</version>
        </dependency>
    </dependencies>
    ```
* 项目 `maven_04_dao` 的 `BookDao` 接口中，Mybatis的增删改查注解报错
	- 解决方案在 `maven_04_dao` 项目的 `pom.xml` 中添加 `mybatis` 的相关依赖
    ```xml
    <dependencies>
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

步骤3：删除原项目中的 `dao` 包。删除 `Dao` 包以后，因为 `maven_02_ssm` 中的 `BookServiceImpl` 类中有使用到 `Dao` 的内容，所以需要在 `maven_02_ssm` 的 `pom.xml` 添加 `maven_04_dao` 的依赖。
```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>maven_04_dao</artifactId>
    <version>1.0-SNAPSHOT</version>
</dependency>
```
此时在 `maven_02_ssm` 项目中就已经添加了 `maven_03_pojo` 和 `maven_04_dao` 包。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221953766.png)
再次对 `maven_02_ssm` 项目进行编译，又会报错，如下:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221954627.png)
和刚才的错误原因是一样的，Maven在仓库中没有找到 `maven_04_dao` ，所以此时我们只需要将 `maven_04_dao` 安装到Maven的本地仓库即可。

步骤4：将项目安装到本地仓库。将需要被依赖的项目 `maven_04_dao` ，使用Maven的 `install` 命令，把其安装到Maven的本地仓库中。
 ![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221955196.png)
安装成功后，在对应的路径下就看到了安装好对应的JAR包：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302221955310.png)
当再次执行 `maven_02_ssm` 的 `compile` 的指令后，就已经能够成功编译。
### 1.2.4 运行测试并总结
将抽取后的项目进行运行，测试之前的增删改查功能依然能够使用。所以对于项目的拆分，大致会有如下几个步骤：
(1) 创建Maven模块
(2) 书写模块代码：分模块开发需要先针对模块功能进行设计，再进行编码。不会先将工程开发完毕，然后进行拆分。**拆分方式可以按照功能拆也可以按照模块拆**。
(3) 通过Maven指令安装模块到本地仓库（`install` 指令），团队内部开发需要发布模块功能到团队内部可共享的仓库中（私服），私服我们后面会讲解。

---
# 2. 依赖管理
我们现在已经能把项目拆分成一个个独立的模块，当在其他项目中想要使用独立出来的这些模块，只需要在其 `pom.xml` 使用 `<dependency>` 标签来进行JAR包的引入即可。

`<dependency>` 其实就是依赖，关于依赖管理里面都涉及哪些内容，我们就一个个来学习下：
* 依赖传递
* 可选依赖
* 排除依赖

我们先来说说什么是依赖——**依赖指当前项目运行所需的JAR**，一个项目可以设置多个依赖。格式为：
```xml
<!--设置当前项目所依赖的所有jar-->
<dependencies>
    <!--设置具体的依赖-->
    <dependency>
        <!--依赖所属群组id-->
        <groupId>org.springframework</groupId>
        <!--依赖所属项目id-->
        <artifactId>spring-webmvc</artifactId>
        <!--依赖版本号-->
        <version>5.2.10.RELEASE</version>
    </dependency>
</dependencies>
```
## 2.1 依赖传递与冲突问题
回到我们刚才的项目案例中，打开Maven的面板，你会发现：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222000259.png)
在项目所依赖的这些JAR包中，有一个比较大的区别就是**有的依赖前面有箭头`>` ，有的依赖前面没有**。那么这个箭头所代表的含义是什么？打开前面的箭头，你会发现这个JAR包下面还包含有其他的JAR包。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222000707.png)
会发现有两个 `maven_03_pojo` 的依赖被加载到 `Dependencies` 中，那么 `maven_04_dao` 中的 `maven_03_pojo` 能不能使用呢？要想验证非常简单，只需要把 `maven_02_ssm` 项目中 `pom.xml` 关于 `maven_03_pojo` 的依赖注释或删除掉：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222000063.png)
在 `Dependencies` 中移除自己所添加 `maven_03_pojo` 依赖后，打开 `BookServiceImpl` 的类，你会发现 `Book` 类依然存在，可以被正常使用
![img|300x400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222000383.png)

这个特性其实就是我们要讲解的==依赖传递==。依赖是具有传递性的：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222001899.png)

**说明**：A代表自己的项目；B,C,D,E,F,G代表的是项目所依赖的JAR包；D1和D2 E1和E2代表是相同JAR的不同版本。
(1) A依赖了B和C，B和C又分别依赖了其他JAR包，所以在A项目中就可以使用上面所有JAR包，这就是所说的依赖传递
(2) 依赖传递有直接依赖和间接依赖
* 相对于A来说，A直接依赖B和C，间接依赖了D1,E1,G, F,D2和E2
* 相对于B来说，B直接依赖了D1和E1，间接依赖了G
* 直接依赖和间接依赖是一个相对的概念

(3) 因为有依赖传递的存在，就会导致JAR包在依赖的过程中出现冲突问题，具体什么是冲突？Maven是如何解决冲突的？

这里所说的==依赖冲突==是指**项目依赖的某一个JAR包，有多个不同的版本**，因而造成类包版本冲突。

情况一：在 `maven_02_ssm` 的 `pom.xml` 中添加两个不同版本的Junit依赖。
```xml
<dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.12</version>
      <scope>test</scope>
    </dependency>

    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.11</version>
      <scope>test</scope>
    </dependency>
</dependencies>
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222001200.png)
通过对比，会发现一个结论
* 特殊优先：==当同级配置了相同资源的不同版本，后配置的覆盖先配置的==。

情况二：路径优先，==当依赖中出现相同的资源时，层级越深，优先级越低，层级越浅，优先级越高==
* A通过B间接依赖到E1
* A通过C间接依赖到E2
* A就会间接依赖到E1和E2，Maven会按照层级来选择，E1是2度，E2是3度，所以最终会选择E1

情况三：声明优先，==当资源在相同层级被依赖时，配置顺序靠前的覆盖配置顺序靠后的==
* A通过B间接依赖到D1
* A通过C间接依赖到D2
* D1和D2都是两度，这个时候就不能按照层级来选择，需要按照声明来，谁先声明用谁，也就是说**B在C之前声明，这个时候使用的是D1，反之则为D2**

但对应上面这些结果，大家不需要刻意去记它。因为**不管Maven怎么选，最终的结果都会在Maven的 `Dependencies` 面板中展示出来**，展示的是哪个版本，也就是说它选择的就是哪个版本，如：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222001786.png)

如果想更全面的查看Maven中各个坐标的依赖关系，可以点击Maven面板中的 `Show Dependencies` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222001207.png)

在这个视图中，就能很明显的展示出JAR包之间的相互依赖关系。
### 2.2 可选依赖和排除依赖
依赖传递介绍完以后，我们来思考一个问题，
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222002791.png)
* `maven_02_ssm` 依赖了 `maven_04_dao`
* `maven_04_dao` 依赖了 `maven_03_pojo`
* 因为现在有依赖传递，所以 `maven_02_ssm` 能够使用到 `maven_03_pojo` 的内容
* 如果说现在不想让 `maven_02_ssm` 依赖到 `maven_03_pojo` ，有哪些解决方案?

**说明**：在真实使用的过程中，`maven_02_ssm` 中是需要用到 `maven_03_pojo` 的，我们这里只是用这个例子描述我们的需求。因为**有时候 `maven_04_dao` 出于某些因素的考虑，就是不想让别人使用自己所依赖的 `maven_03_pojo`** 。

### 2.2.1 方案一：可选依赖
可选依赖指对外隐藏当前所依赖的资源——不透明。在`maven_04_dao`的 `pom.xml` ，在引入 `maven_03_pojo` 时，添加 `optional` ：
```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>maven_03_pojo</artifactId>
    <version>1.0-SNAPSHOT</version>
    <!--可选依赖是隐藏当前工程所依赖的资源，隐藏后对应资源将不具有依赖传递-->
    <optional>true</optional>
</dependency>
```
此时 `BookServiceImpl` 就已经报错了，说明由于 `maven_04_dao` 将 `maven_03_pojo` 设置成可选依赖，导致 `maven_02_ssm` 无法引用到 `maven_03_pojo` 中的内容，导致 `Book` 类找不到。
![300x430](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222002819.png)

### 2.2.2 方案二：排除依赖
排除依赖指主动断开「依赖中包含的资源」，被排除的资源无需指定版本——不需要。

前面我们已经通过可选依赖实现了阻断 `maven_03_pojo` 的依赖传递，对于排除依赖，则指的是**已经有依赖的事实**，也就是说 **`maven_02_ssm` 项目中已经通过依赖传递用到了 `maven_03_pojo` ，此时我们需要做的是将其进行排除**，所以接下来需要修改 `maven_02_ssm` 的 `pom.xml` 。
```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>maven_04_dao</artifactId>
    <version>1.0-SNAPSHOT</version>
    <!--排除依赖是隐藏当前资源对应的依赖关系-->
    <exclusions>
        <exclusion>
            <groupId>com.itheima</groupId>
            <artifactId>maven_03_pojo</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```
这样操作后，`BookServiceImpl` 中的 `Book` 类一样也会报错。当然 `exclusions` 标签带 `s` 说明我们是可以依次排除多个依赖到的JAR包，比如 `maven_04_dao` 中有依赖 Junit和Mybatis，我们也可以一并将其排除。
```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>maven_04_dao</artifactId>
    <version>1.0-SNAPSHOT</version>
    <!--排除依赖是隐藏当前资源对应的依赖关系-->
    <exclusions>
        <exclusion>
            <groupId>com.itheima</groupId>
            <artifactId>maven_03_pojo</artifactId>
        </exclusion>
        <exclusion>
            <groupId>log4j</groupId>
            <artifactId>log4j</artifactId>
        </exclusion>
        <exclusion>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```
介绍我这两种方式后，简单来梳理下，就是
* A依赖B，B依赖C，`C`通过依赖传递会被A使用到，现在要想办法**让A不去依赖C**
* 可选依赖是在B上设置 `<optional>` ，**A不知道有C的存在**（给别人用）
* 排除依赖是在A上设置 `<exclusions>` ，**A知道有C的存在，主动将其排除掉**（用别人的东西）。

---
# 3. 聚合和继承
我们的项目已经从以前的单模块，变成了现在的多模块开发。项目一旦变成了多模块开发以后，就会引发一些问题，在这一节中我们主要会学习两个内容 `聚合` 和 `继承` ，用这两个知识来解决下分模块后的一些问题。
## 3.1 聚合
![img|580x300](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222003658.png)
* 分模块开发后，需要将这四个项目都安装到本地仓库，目前我们只能通过项目Maven面板的 `install` 来安装，并且需要安装四个，如果我们的项目足够多，那么一个个安装起来还是比较麻烦的
* 如果四个项目都已经安装成功，当 `ssm_pojo` 发生变化后，我们就得将 `ssm_pojo` 重新安装到Maven仓库，但**为了确保我们对 `ssm_pojo` 的修改不会影响到其他项目模块，我们需要对所有的模块进行重新编译，那又需要将所有的模块再来一遍**

项目少的话还好，但如果项目多的话，一个个操作项目就容易出现漏掉或重复操作的问题，所以我们就想==能不能抽取一个项目，把所有的项目管理起来，以后我们要想操作这些项目，只需要操作这一个项目，其他所有的项目都走一样的流程==，这个不就很省事省力。

这就用到了我们接下来要讲解的==聚合==：
* 所谓聚合：**将多个模块组织成一个整体，同时进行项目构建的过程**称为聚合
* 聚合工程：通常是一个不具有业务功能的"空"工程（有且仅有一个 `pom` 文件）
* 作用：使用聚合工程可以将多个工程编组，通过对聚合工程进行构建，**实现对所包含的模块进行同步构建**
	* 当工程中某个模块发生更新（变更）时，必须保障工程中与已更新模块关联的模块同步更新，此时可以**使用聚合工程来解决批量模块同步构建的问题**。

关于聚合具体的实现步骤如下。
步骤1:创建一个空的maven项目
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222003451.png)
步骤2：将项目的打包方式改为 `pom` 。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.itheima</groupId>
    <artifactId>maven_01_parent</artifactId>
    <version>1.0-RELEASE</version>
    <packaging>pom</packaging>
</project>
```
**说明**：项目的打包方式，我们接触到的有三种，分别是
* `jar` ：默认情况，说明该项目为Java项目
* `war` ：说明该项目为Web项目
* `pom` ：说明该项目为聚合或继承（后面会讲）项目

步骤3：`pom.xml` 添加所要管理的项目。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.itheima</groupId>
    <artifactId>maven_01_parent</artifactId>
    <version>1.0-RELEASE</version>
    <packaging>pom</packaging>
    
    <!--设置管理的模块名称-->
    <modules>
        <module>../maven_02_ssm</module>
        <module>../maven_03_pojo</module>
        <module>../maven_04_dao</module>
    </modules>
</project>
```
步骤4：使用聚合统一管理项目。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222004654.png)

测试发现，当 `maven_01_parent` 的 `compile` 被点击后，所有被其管理的项目都会被执行编译操作。这就是聚合工程的作用。

**说明**：==聚合工程管理的项目在运行时，会按照项目与项目之间的依赖关系来自动决定执行的顺序==，和配置的顺序无关。

聚合的知识我们就讲解完了，最后总结一句话就是，**聚合工程主要是用来管理项目**。
## 3.2 继承
我们已经使用聚合工程去管理项目，==聚合工程进行某一个构建操作，其他被其管理的项目也会执行相同的构建操作==。那么接下来，我们再来分析下，多模块开发存在的另外一个问题，`重复配置` 的问题，先来看张图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222004517.png)
* `spring-webmvc`、`spring-jdbc` 在三个项目模块中都有出现，这样就出现了重复的内容
* `spring-test` 只在 `ssm_crm` 和 `ssm_goods` 中出现，而在 `ssm_order` 中没有，这里是部分重复的内容
* 我们使用的Sspring版本目前是 `5.2.10.RELEASE` ，假如后期要想升级Spring版本，所有跟Spring相关JAR包都得被修改，涉及到的项目越多，维护成本越高

面对上面的这些问题，我们就得用到接下来要学习的==继承==
* 所谓继承：描述的是两个工程间的关系，与Java中的继承相似，**子工程可以继承父工程中的配置信息，常见于依赖关系的继承**。
* 作用：
	- 简化配置
	- 减少版本冲突

接下来，我们到程序中去看看继承该如何实现？

步骤1：**创建一个空的Maven项目、并将其打包方式设置为 `pom`** 。因为这一步和前面Maven创建聚合工程的方式一样，所以可以单独创建一个新的工程，也可以直接和聚合公用一个工程。==实际开发中，聚合和继承一般也都放在同一个项目中，但这两个的功能不一样==。

步骤2：**在子项目中设置其父工程**。分别在 `maven_02_ssm, maven_03_pojo, maven_04_dao` 的 `pom.xml` 中添加其父项目为 `maven_01_parent` 。
```xml
<!--配置当前工程继承自parent工程-->
<parent>
    <groupId>com.itheima</groupId>
    <artifactId>maven_01_parent</artifactId>
    <version>1.0-RELEASE</version>
    <!--设置父项目pom.xml位置路径-->
    <relativePath>../maven_01_parent/pom.xml</relativePath>
</parent>
```
步骤3：**优化子项目共有依赖导入问题**。将子项目共同使用的JAR包都抽取出来，维护在父项目的 `pom.xml` 中。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.itheima</groupId>
    <artifactId>maven_01_parent</artifactId>
    <version>1.0-RELEASE</version>
    <packaging>pom</packaging>
    
    <!--设置管理的模块名称-->
    <modules>
        <module>../maven_02_ssm</module>
        <module>../maven_03_pojo</module>
        <module>../maven_04_dao</module>
    </modules>
    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>5.2.10.RELEASE</version>
        </dependency>

        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
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
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
            <scope>provided</scope>
        </dependency>

        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.9.0</version>
        </dependency>
    </dependencies>
</project>
```
删除子项目中已经被抽取到父项目的 `pom.xml` 中的JAR包，如在 `maven_02_ssm` 的 `pom.xml` 中将已经出现在父项目的JAR包删除掉。
```xml
<?xml version="1.0" encoding="UTF-8"?>

<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.itheima</groupId>
  <artifactId>maven_02_ssm</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>war</packaging>

  <!--配置当前工程继承自parent工程-->
  <parent>
    <groupId>com.itheima</groupId>
    <artifactId>maven_01_parent</artifactId>
    <version>1.0-RELEASE</version>
    <relativePath>../maven_01_parent/pom.xml</relativePath>
  </parent>
  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.12</version>
      <scope>test</scope>
    </dependency>

    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.11</version>
      <scope>test</scope>
    </dependency>

    <dependency>
      <groupId>com.itheima</groupId>
      <artifactId>maven_04_dao</artifactId>
      <version>1.0-SNAPSHOT</version>
      <!--排除依赖是隐藏当前资源对应的依赖关系-->
      <exclusions>
        <exclusion>
          <groupId>log4j</groupId>
          <artifactId>log4j</artifactId>
        </exclusion>
        <exclusion>
          <groupId>org.mybatis</groupId>
          <artifactId>mybatis</artifactId>
        </exclusion>
      </exclusions>
    </dependency>

  </dependencies>

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
</project>
```
删除完后，你会发现父项目中有依赖对应的JAR包，子项目虽然已经将重复的依赖删除掉了，但刷新时，子项目中所需要的JAR包依然存在。

~~当项目的 `<parent>` 标签被移除掉，会发现多出来的JAR包依赖也会随之消失~~。将 `maven_04_dao` 项目的 `pom.xml` 中的所有依赖删除，然后添加上 `maven_01_parent` 的父项目坐标：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.itheima</groupId>
    <artifactId>maven_04_dao</artifactId>
    <version>1.0-SNAPSHOT</version>

    <!--配置当前工程继承自parent工程-->
    <parent>
        <groupId>com.itheima</groupId>
        <artifactId>maven_01_parent</artifactId>
        <version>1.0-RELEASE</version>
        <relativePath>../maven_01_parent/pom.xml</relativePath>
    </parent>
</project>
```
刷新并查看Maven的面板，会发现 `maven_04_dao` 同样引入了父项目中的所有依赖。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222005385.png)
这样我们就可以解决刚才提到的第一个问题，将子项目中的公共JAR包抽取到父工程中进行统一添加依赖，这样做可以简化配置，并且当父工程中所依赖的JAR包版本发生变化，所有子项目中对应的JAR包版本也会跟着更新。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222005429.png)

步骤4：**优化子项目依赖版本问题**。如果把所有用到的JAR包都管理在父项目的 `pom.xml` ，看上去更简单些，但是这样就会导致有很多项目引入了过多自己不需要的JAR包。如上面看到的这张图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222006906.png)
如果把所有的依赖都放在了父工程中进行统一维护，就会导致 `ssm_order` 项目中多引入了 `spring-test` 的JAR包，如果这样的JAR包过多的话，对于 `ssm_order` 来说也是一种"负担"。

那针对于这种部分项目有的JAR包，我们该如何管理优化呢？
(1) 在父工程 `maven_01_parent` 的 `pom.xml` 来定义**依赖管理**。
```xml
<!--定义依赖管理-->
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <scope>test</scope>
        </dependency>
    </dependencies>
</dependencyManagement>
```
(2) 将 `maven_02_ssm` 的 `pom.xml` 中的 `junit` 依赖删除掉，刷新Maven：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222006226.png)

刷新完会发现，在 `maven_02_ssm` 项目中的Junit依赖并没有出现，所以我们得到一个结论：`<dependencyManagement>` 标签==不真正引入JAR包，而是配置可供子项目选择的JAR包依赖==。**子项目要想使用它所提供的这些JAR包，需要自己添加依赖，并且不需要指定 `<version>`** 。

(3) 在 `maven_02_ssm` 的 `pom.xml` 添加Junit的依赖
```xml
<dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <scope>test</scope>
</dependency>
```
**注意：这里就不需要添加版本了**，这样做的好处是，当父工程 `dependencyManagement` 标签中的版本发生变化后，子项目中的依赖版本也会跟着发生变化。

(4) `maven_04_dao` 的 `pom.xml` 添加Junit的依赖。
```xml
<dependency>
    <groupId>junit</groupId>
    <artifactId>junit</artifactId>
    <scope>test</scope>
</dependency>
```
这个时候，`maven_02_ssm` 和 `maven_04_dao` 这两个项目中的Junit版本，会跟随着父项目中的标签 `dependencyManagement` 中Junit的版本发生变化而变化。不需要Junit的项目就不需要添加对应的依赖即可。

至此继承就已经学习完了，总结来说，继承可以帮助做两件事
* 将所有项目公共的JAR包依赖提取到父工程的 `pom.xml` 中，子项目就可以不用重复编写，简化开发
* 将所有项目的JAR包配置到父工程的 `dependencyManagement` 标签下，实现版本管理，方便维护
	* **`dependencyManagement` 标签不真正引入JAR包，只是管理JAR包的版本**
	* 子项目在引入时，只需要指定 `groupId` 和 `artifactId` ，不需要加 `version`
	* 当 `dependencyManagement` 标签中JAR包版本发生变化，所有子项目中有用到该JAR包的地方、对应的版本会自动随之更新

最后总结一句话就是，**父工程主要是用来快速配置依赖JAR包和管理项目中所使用的资源**。继承的资源如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302230022838.png)

**小结**。继承的实现步骤：
* 创建Maven模块，设置打包类型为 `pom`
  ```xml
  <packaging>pom</packaging>
  ```
* 在父工程的 `pom` 文件中配置依赖关系（子工程将沿用父工程中的依赖关系），**一般只抽取子项目中公有的JAR包**
  ```xml
  <dependencies>
      <dependency>
          <groupId>org.springframework</groupId>
          <artifactId>spring-webmvc</artifactId>
          <version>5.2.10.RELEASE</version>
      </dependency>
      ...
  </dependencies>
  ```
* 在父工程中配置子工程中可选的依赖关系
  ```xml
  <dependencyManagement>
      <dependencies>
          <dependency>
              <groupId>com.alibaba</groupId>
              <artifactId>druid</artifactId>
              <version>1.1.16</version>
          </dependency>
      </dependencies>
      ...
  </dependencyManagement>
  ```
* **在子工程中配置当前工程所继承的父工程**
  ```xml
  <!--定义该工程的父工程-->
  <parent>
      <groupId>com.itheima</groupId>
      <artifactId>maven_01_parent</artifactId>
      <version>1.0-RELEASE</version>
      <!--填写父工程的pom文件,可以不写-->
      <relativePath>../maven_01_parent/pom.xml</relativePath>
  </parent>
  ```
* 在子工程中配置使用父工程中可选依赖的坐标
  ```xml
  <dependencies>
      <dependency>
          <groupId>com.alibaba</groupId>
          <artifactId>druid</artifactId>
      </dependency>
  </dependencies>
  ```

注意事项：
1. 子工程中使用父工程中的可选依赖时，仅需要提供群组 `id` 和项目 `id` ，无需提供版本，**版本由父工程统一提供，避免版本冲突**
2. 子工程中还可以定义父工程中没有定义的依赖关系，**只不过不能被父工程进行版本统一管理**。

## 3.3 聚合与继承的区别
### 3.3.1 聚合与继承的区别
两种之间的作用：
* 聚合用于**快速构建项目**，对项目进行管理
* 继承用于**快速配置和管理子项目中所使用JAR包的版本**

聚合和继承的相同点：
* **聚合与继承的 `pom.xml` 文件打包方式均为 `pom`** ，可以将两种关系制作到同一个 `pom` 文件中
* **聚合与继承均属于设计型模块**，并无实际的模块内容

聚合和继承的不同点：
* **聚合是在当前模块中配置关系**，聚合可以感知到参与聚合的模块有哪些
* **继承是在子模块中配置关系**（Maven中的继承与Java中的继承相似），父模块无法感知哪些子模块继承了自己

相信到这里，大家已经能区分开什么是聚合和继承，但有一个稍微麻烦的地方就是聚合和继承的工程构建，需要在聚合项目中手动添加 `modules` 标签，需要在所有的子项目中添加 `parent` 标签，万一写错了咋办？
### 3.3.2 IDEA构建聚合与继承工程
其实对于聚合和继承工程的创建，IDEA已经能帮助我们快速构建，具体的实现步骤为。

步骤1：创建一个Maven项目。创建一个空的Maven项目，可以将项目中的 `src` 目录删除掉，这个项目作为聚合工程和父工程。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222007634.png)
步骤2：创建子项目。**该项目可以被聚合工程管理，同时会继承父工程**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222008635.png)
创建成功后，`maven_parent` 即是聚合工程又是父工程，`maven_web` 中也有 `parent` 标签，继承的就是 `maven_parent` ，对于难以配置的内容都自动生成。

按照上面这种方式，大家就可以根据自己的需要、来构建分模块项目。

---
# 4. 属性
在这一节学习两个内容，分别是
* 属性
* 版本管理

**属性中会继续解决分模块开发项目存在的问题**，版本管理主要是**认识下当前主流的版本定义方式**。
## 4.1 属性
### 4.1.1 问题分析
讲解内容之前，我们还是先来分析问题。前面我们在父工程中的 `dependencyManagement` 标签中，对项目中所使用的JAR包版本进行了统一的管理，但如果在标签中有如下的内容：
![img|350x400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222008049.png)
你会发现，==如果我们现在想更新Spring的版本，你会发现我们依然需要更新多个JAR包的版本==，这样的话还是有可能出现漏改、导致程序出问题，而且改起来也是比较麻烦。

问题清楚后，我们需要解决的话，就可以参考咱们JAR基础所学习的变量，声明一个变量，在其他地方使用该变量，当变量的值发生变化后，所有使用变量的地方，就会跟着修改，即：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222009210.png)
属性有好几种类别：
1. 自定义属性。**等同于定义变量，方便统一维护**。
	定义格式：
	```xml
	<!-- 定义自定义属性 -->
	<properties>
		<spring.version>5.1.9.RELEASE</spring.version>
		<junit.version>4.12</junit.version>
	</properties>
	```
	调用格式：
	```xml
	<dependency>
		<groupId>org.springframework</groupId>
		<artifactId>spring-context</artifactId>
		<version>${spring.version}</version>
	</dependency>
	```
2. 内置属性。**使用Maven内置属性，快速配置**。调用格式：
	```xml
	${basedir}
	${version}
	```
3. `Setting` 属性。**使用Maven配置文件 `setting.xml` 中的标签属性，用于动态配置**。调用格式：
	```xml
	${settings.localRepository}
	```
4. Java系统属性。**读取Java系统属性**。调用格式：
	```xml
	${user.home}
	```
	系统属性查询方式：
	```java
	mvn help:system
	```
5. 环境变量属性。调用格式：
	```xml
	${env.JAVA_HOME}
	```
	环境变量属性查询方式：
	```java
	mvn help:system
	```
 
### 4.1.2 解决步骤
步骤1：父工程中定义属性。
```xml
<properties>
    <spring.version>5.2.10.RELEASE</spring.version>
    <junit.version>4.12</junit.version>
    <mybatis-spring.version>1.3.0</mybatis-spring.version>
</properties>
```
步骤2：修改依赖的 `version` 。
```xml
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-core</artifactId>
    <version>${spring.version}</version>
</dependency>
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
```
此时，我们只需要更新父工程中 `properties` 标签中所维护的JAR包版本，所有子项目中的版本也就跟着更新。当然除了将Spring相关版本进行维护，我们可以将其他的JAR包版本也进行抽取，这样就可以**对项目中所有JAR包的版本进行统一维护**，如：
```xml
<!--定义属性-->
<properties>
    <spring.version>5.2.10.RELEASE</spring.version>
    <junit.version>4.12</junit.version>
    <mybatis-spring.version>1.3.0</mybatis-spring.version>
</properties>
```

## 4.2 配置文件加载属性
Maven中的属性我们已经介绍过了，现在也已经能够通过Maven来集中管理Maven中依赖jar包的版本。但又有新的需求，就是**想让Maven对于属性的管理范围能更大些**，比如我们之前项目中的 `jdbc.properties`，这个配置文件中的属性，能不能也来让Maven进行管理呢？答案是肯定的，具体的实现步骤如下。

步骤1：父工程定义属性。
```xml
<properties>
   <jdbc.url>jdbc:mysql://127.1.1.1:3306/ssm_db</jdbc.url>
</properties>
```
步骤2：`jdbc.properties` 文件中引用属性。在 `jdbc.properties` ，将 `jdbc.url` 的值直接获取Maven配置的属性。
```properties
jdbc.driver=com.mysql.jdbc.Driver
jdbc.url=${jdbc.url}
jdbc.username=root
jdbc.password=root
```
步骤3：**设置Maven过滤文件范围**。Maven在默认情况下是**从当前项目的 `src\main\resources` 下读取文件进行打包**。现在我们需要打包的资源文件是在 `maven_02_ssm` 下，需要我们**通过配置来指定具体的资源目录**。
```xml
<build>
    <resources>
        <!--设置资源目录-->
        <resource>
            <directory>../maven_02_ssm/src/main/resources</directory>
            <!--开启对配置文件的资源加载过滤-->
            <!--开启过滤规则,设置能够解析${}格式,默认是false -->
            <filtering>true</filtering>
        </resource>
    </resources>
</build>
```
**说明**：`directory` 路径前要添加 `../` 的原因是，`maven_02_ssm` 相对于父工程的 `pom.xml` 路径是在其上一层的目录中，所以需要添加。

修改完后，注意 `maven_02_ssm` 项目的 `resources` 目录就多了些东西，如下：
![img|400](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222009403.png)

步骤4：测试是否生效。测试时，只需要将 `maven_02_ssm` 项目进行打包（出现需要 `webxml` 文件时可以简单给一个，虽然已经写成配置类了），然后观察打包结果中最终生成的内容是否为Maven中配置的内容。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222010603.png)
**说明**：打包的过程中如果报如下错误
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222010766.png)
原因就是Maven发现你的项目为Web项目，就会去找Web项目的入口 `web.xml` [配置文件配置的方式]，发现没有找到，就会报错。

解决方案1：在 `maven_02_ssm` 项目的 `src\main\webapp\WEB-INF\` 添加一个 `web.xml` 文件。
```xml
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
         version="3.1">
</web-app>
```
解决方案2：配置Maven打包 `war` 时，忽略 `web.xml` 检查
```xml
<build>
    <plugins>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-war-plugin</artifactId>
            <version>3.2.3</version>
            <configuration>
                <failOnMissingWebXml>false</failOnMissingWebXml>
            </configuration>
        </plugin>
    </plugins>
</build>
```

上面的属性管理就已经完成，但有一个问题没有解决，因为不只是 `maven_02_ssm` 项目需要有属性被父工程管理，**如果有多个项目需要配置，该如何实现呢**？

方式一：可以配，但是如果项目够多的话，这个配置也是比较繁琐。
```xml
<build>
    <resources>
        <!--设置资源目录，并设置能够解析${}-->
        <resource>
            <directory>../maven_02_ssm/src/main/resources</directory>
            <!--开启对配置文件的资源加载过滤-->
            <filtering>true</filtering>
        </resource>
        <resource>
            <directory>../maven_03_pojo/src/main/resources</directory>
            <filtering>true</filtering>
        </resource>
        ...
    </resources>
</build>
```
方式二：
```xml
<build>
    <resources>
		<!-- 设定配置文件对应的位置目录,支持使用属性动态设定路径 -->
        <!--
			${project.basedir}:当前项目所在目录,子项目继承了父项目,
			相当于所有的子项目都添加了资源目录的过滤
		-->
        <resource>
            <directory>${project.basedir}/src/main/resources</directory>
            <filtering>true</filtering>
        </resource>
    </resources>
</build>
```
上面我们所使用的都是Maven的自定义属性，除了 `${project.basedir}` ，它属于Maven的内置系统属性。在Maven中的属性分为：
- 自定义属性（常用）
- 内置属性
- Setting属性
- Java系统属性
- 环境变量属性
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222012304.png)

具体如何查看这些属性：命令行中输入 `mvn help:system` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222012790.png)
具体使用，就是使用 `${key}` 来获取，`key` 为等号左边的，值为等号右边的，比如获取红线的值，对应的写法为 `${java.runtime.name}`。

## 4.3 版本管理
关于这个版本管理解决的问题是，在Maven创建项目和引用别人项目时，我们都看到过如下内容:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222012713.png)

这里面有两个单词，SNAPSHOT和RELEASE，它们所代表的含义是什么呢？我们打开Maven仓库地址 `https://mvnrepository.com/` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222013987.png)
在我们JAR包的版本定义中，有两个工程版本用的比较多：
- SNAPSHOT（快照版本）
	- 项目开发过程中，为方便团队成员合作，解决模块间相互依赖和时时更新的问题，开发者对每个模块进行构建时，输出的临时性版本，称为快照版本
	- 快照版本会随着开发的进展不断更新
- RELEASE（发布版本）
	- 项目开发到一定阶段里程碑后，向团队外部发布较为稳定的版本，这种版本所对应的构件文件是稳定的
	- **即便进行功能的后续开发，也不会改变当前发布版本内容**，这种版本称为发布版本

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302230055804.png)
除了上面的工程版本，我们还经常能看到一些发布版本：
* alpha版：内测版，bug多不稳定内部版本不断添加新功能
* beta版：公测版，不稳定（比alpha稳定些），bug相对较多不断添加新功能
* 纯数字版

对于这些版本，大家只需要简单认识下即可。

---
# 5. 多环境配置与应用
这一节中，我们会讲两个内容，分别是 `多环境开发` 和打包过程中 `跳过测试` 。
### 5.1 多环境开发
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222013666.png)
- 我们平常都是在自己的开发环境进行开发
* 当开发完成后，需要把开发的功能部署到测试环境、供测试人员进行测试使用，
* 等测试人员测试通过后，我们会将项目部署到生成环境上线使用。
* 这时就有一个问题是，不同环境的配置是不相同的，如不可能让三个环境都用一个数据库，所以就会有三个数据库的URL配置，
* 我们在项目中如何配置？
* 要想**实现不同环境之间的配置切换**，又该如何实现呢？

Maven提供配置多种环境的设定，帮助开发者在使用过程中快速切换环境。具体实现步骤如下。

步骤1：**父工程配置多个环境，并指定默认激活环境**。
```xml
<profiles>
    <!--开发环境-->
    <profile>
        <id>env_dep</id>
        <properties>
            <jdbc.url>jdbc:mysql://127.1.1.1:3306/ssm_db</jdbc.url>
        </properties>
        <!--设定是否为默认启动环境-->
        <activation>
            <activeByDefault>true</activeByDefault>
        </activation>
    </profile>
    <!--生产环境-->
    <profile>
        <id>env_pro</id>
        <properties>
            <jdbc.url>jdbc:mysql://127.2.2.2:3306/ssm_db</jdbc.url>
        </properties>
    </profile>
    <!--测试环境-->
    <profile>
        <id>env_test</id>
        <properties>
            <jdbc.url>jdbc:mysql://127.3.3.3:3306/ssm_db</jdbc.url>
        </properties>
    </profile>
</profiles>
```
步骤2：执行安装查看 `env_dep` 环境是否生效。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222013433.png)

查看到的结果为:
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222014212.png)

步骤3：切换默认环境为生产环境。
```xml
<profiles>
    <!--开发环境-->
    <profile>
        <id>env_dep</id>
        <properties>
            <jdbc.url>jdbc:mysql://127.1.1.1:3306/ssm_db</jdbc.url>
        </properties>
    </profile>
    <!--生产环境-->
    <profile>
        <id>env_pro</id>
        <properties>
            <jdbc.url>jdbc:mysql://127.2.2.2:3306/ssm_db</jdbc.url>
        </properties>
        <!--设定是否为默认启动环境-->
        <activation>
            <activeByDefault>true</activeByDefault>
        </activation>
    </profile>
    <!--测试环境-->
    <profile>
        <id>env_test</id>
        <properties>
            <jdbc.url>jdbc:mysql://127.3.3.3:3306/ssm_db</jdbc.url>
        </properties>
    </profile>
</profiles>
```
步骤4：执行安装并查看 `env_pro` 环境是否生效。查看到的结果为 `jdbc:mysql://127.2.2.2:3306/ssm_db` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222014241.png)
虽然已经能够实现不同环境的切换，但是每次切换都是需要手动修改，如何实现在不改变代码的前提下完成环境的切换呢？

步骤5：**命令行实现环境切换**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222015326.png)

步骤6：执行安装并查看 `env_test` 环境是否生效。查看到的结果为 `jdbc:mysql://127.3.3.3:3306/ssm_db` ：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222015388.png)

所以总结来说，对于多环境切换只需要两步即可：
* 父工程中定义多环境
  ```xml
  <!-- 创建多环境 -->
  <profiles>
	<!-- 定义具体的环境 -->
	  <profile>
		  <!-- 定义环境对应的唯一名称 -->
	      <id>环境名称</id>
		  <!-- 定义环境中专用的属性值 -->
          <properties>
	          <!-- 设置默认启动 -->
	          <key>value</key>
          </properties>
          <activation>
          	<activeByDefault>true</activeByDefault>
          </activation>
      </profile>
      ...
  </profiles>
  ```
* 使用多环境（构建过程）
  ```bash
  mvn 指令 -P 环境定义ID[环境定义中获取]
  ```

## 5.2 跳过测试
前面在执行 `install` 指令时，Maven都会按照顺序从上往下依次执行，每次都会执行 `test` ，对于 `test` 来说有它存在的意义，可以确保每次打包或者安装时程序的正确性：
- 假如测试已经通过，在我们没有修改程序的前提下，再次执行打包或安装命令，由于顺序执行，测试会被再次执行，就有点耗费时间了。
* ==功能开发过程中有部分模块还没有开发完毕，测试无法通过，但想要把其中某一部分进行快速打包，此时由于测试环境失败，就会导致打包失败==。

遇到上面这些情况时，我们就想跳过测试执行下面的构建命令，具体实现方式有很多。

### 5.2.1 方式一：IDEA工具实现跳过测试
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222015842.png)
图中的按钮为 `Toggle 'Skip Tests' Mode` ，Toggle翻译为切换的意思，也就是说在测试与不测试之间进行切换。点击一下，出现测试画横线的图片，如下：![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222016235.png)
说明测试已经被关闭，再次点击就会恢复。==这种方式最简单，但有点"暴力"，会把所有的测试都跳过==，如果我们想更精细的控制哪些跳过哪些不跳过，就需要使用配置插件的方式。

### 5.2.2 方式二：配置插件实现跳过测试
在父工程中的 `pom.xml` 中添加测试插件配置：
```xml
<build>
    <plugins>
        <plugin>
            <artifactId>maven-surefire-plugin</artifactId>
            <version>2.12.4</version>
            <configuration>
                <skipTests>false</skipTests>
                <!--排除掉不参与测试的内容-->
                <excludes>
                    <exclude>**/BookServiceTest.java</exclude>
                </excludes>
            </configuration>
        </plugin>
    </plugins>
</build>
```
`skipTests` ：如果为 `true` ，则跳过所有测试，如果为 `false` ，则不跳过测试
`excludes` ：哪些测试类不参与测试，即排除，针对 `skipTests` 为 `false` 来设置的
`includes` ：哪些测试类要参与测试，即包含，针对 `skipTests` 为 `true` 来设置的

### 5.2.3 方式三：命令行跳过测试
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222016335.png)
使用Maven的命令行，`mvn 指令 -D skipTests`

注意事项：
* 执行的项目构建指令必须包含测试生命周期，否则无效果。例如执行 `compile` 生命周期，不经过 `test` 生命周期。
* 该命令可以不借助IDEA，直接使用命令行进行跳过测试，**需要注意的是CMD要在 `pom.xml` 所在目录下进行执行**。

---
# 6. 私服
这一节，我们主要学习的内容是：
* 私服简介
* 私服仓库分类
* 资源上传与下载

首先来说一说什么是私服？
## 6.1 私服简介
团队开发现状分析：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222016134.png)

(1) 张三负责 `ssm_crm` 的开发，自己写了一个 `ssm_pojo` 模块，要想使用直接将 ssm_pojo 安装到本地仓库即可
(2) 李四负责 `ssm_order` 的开发，需要用到张三所写的 `ssm_pojo` 模块，这个时候如何将张三写的 `ssm_pojo` 模块交给李四呢？
(3) 如果直接拷贝，那么团队之间的jar包管理会非常混乱而且容器出错，这时我们就想==能不能将写好的项目上传到中央仓库，谁想用就直接联网下载即可==
(4) Maven的中央仓库不允许私人上传自己的JAR包，那么我们就得换种思路，自己搭建一个类似于中央仓库的东西，把自己的内容上传上去，其他人就可以从上面下载JAR包使用
(5) 这个类似于中央仓库的东西，就是我们接下来要学习的==私服==。

所以到这就有两个概念，一个是私服，一个是中央仓库：
- 私服：公司内部搭建的用于存储Maven资源的服务器，是一台独立的服务器，**用于解决团队内部的资源共享与资源同步问题**
- 远程仓库：Maven开发团队维护的用于存储Maven资源的服务器

搭建Maven私服的方式有很多，我们来介绍其中一种使用量比较大的实现方式——Nexus
* Sonatype公司的一款Maven私服产品
* 下载地址：https://help.sonatype.com/repomanager3/download

## 6.2 私服安装
步骤1：下载解压。将 `资料\latest-win64.zip` 解压到一个空目录下。
![img|200](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222016580.png)
步骤2：启动Nexus。
![img|200](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222018224.png)
使用CMD进入到解压目录下的 `nexus-3.30.1-01\bin` ，执行如下命令：
```bash
nexus.exe /run nexus
```
看到如下内容，说明启动成功（启动起来是一个Web服务器，我们可以访问）。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222017233.png)
步骤3：浏览器访问。访问地址为：
```
http://localhost:8081
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222017391.png)
步骤4：首次登录重置密码。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222018994.png)
输入用户名和密码进行登录，登录成功后，出现如下页面：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222018330.png)
点击下一步，需要重新输入新密码，为了和后面的保持一致，密码修改为 `admin` 。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222019781.png)
设置是否允许匿名访问：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222019289.png)
点击完成：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222019058.png)
至此私服就已经安装成功。如果要想修改一些基础配置信息，可以使用：
- 修改基础配置信息
	- 安装路径下 `etc` 目录中 `nexus-default.properties` 文件，保存有nexus基础配置信息，例如默认访问端口。
- 修改服务器运行配置信息
	- 安装路径下 `bin` 目录中 `nexus.vmoptions` 文件，保存有nexus服务器启动对应的配置信息，例如默认占用内存空间。

## 6.3 私服仓库分类
私服资源操作流程分析：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222019270.png)
(1) 在没有私服的情况下，我们自己创建的服务都是安装在Maven的本地仓库中
(2) 私服中也有仓库，我们要把自己的资源上传到私服，最终也是放在私服的仓库中
(3) 其他人要想使用你所上传的资源，就需要从私服的仓库中获取

(4) 当我们要使用的资源不是自己写的，是远程中央仓库有的第三方JAR包，这时就需要从远程中央仓库下载，每个开发者都去远程中央仓库下速度比较慢（中央仓库服务器在国外）
(5) **私服就再准备一个仓库，用来专门存储从远程中央仓库下载的第三方JAR包**，第一次访问没有就会去远程中央仓库下载，下次再访问就直接走私服下载

(6) 前面在介绍版本管理时提到过有 `SNAPSHOT` 和 `RELEASE`，如果把这两类的都放到同一个仓库，比较混乱，所以私服就把这两种JAR包放入不同的仓库
(7) 上面我们已经介绍了有三种仓库，一种是存放 `SNAPSHOT` 的，一种是存放 `RELEASE` ，还有一种是存放从远程仓库下载的第三方包，那么我们在获取资源时要从哪个仓库获取呢？
(8) 为了方便获取，==我们将所有的仓库编成一个组，我们**只需要访问仓库组**去获取资源==。

所有私服仓库总共分为三大类：
- 宿主仓库 `hosted` ：保存无法从中央仓库获取的资源
	- 自主研发
	- 第三方非开源项目，比如Oracle，因为是付费产品，所以中央仓库没有
- 代理仓库 `proxy` ：代理远程仓库，通过nexus访问其他公共仓库，例如中央仓库（所有项目组共用）
- 仓库组 `group` ：
	- 将若干个仓库组成一个群组，简化配置
	- 仓库组不能保存资源，属于设计型仓库
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222020671.png)

## 6.4 本地仓库访问私服配置
我们通过IDEA将开发的模块上传到私服，中间是要经过本地Maven的（先安装到本地仓库）。本地Maven需要知道私服的访问地址、以及私服访问的用户名和密码；私服中的仓库很多，Maven最终要把资源上传到哪个仓库？**Maven下载时，又需要携带用户名和密码到私服上找对应的仓库组进行下载**，然后再给IDEA。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222020522.png)
上面所说的这些内容，我们需要在本地Maven的配置文件 `settings.xml`中进行配置。

步骤1：私服上配置仓库。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222020346.png)
**说明**：第5、6步骤是创建 `itheima-snapshot` 仓库，第7、8步骤是创建 `itheima-release` 仓库

步骤2：配置本地Maven对私服的访问权限（本地仓库 `setting.xml`）。
```xml
<servers>
    <server>
        <id>itheima-snapshot</id>
        <username>admin</username>
        <password>admin</password>
    </server>
    <server>
        <id>itheima-release</id>
        <username>admin</username>
        <password>admin</password>
    </server>
</servers>
```
步骤3：配置私服的访问路径，即本地仓库的资源来源（`setting.xml`）。
```xml
<mirrors>
    <mirror>
        <!--配置仓库组的ID-->
        <id>maven-public</id>
        <!--*代表所有内容都从私服获取-->
        <mirrorOf>*</mirrorOf>
        <!--私服仓库组maven-public的访问路径-->
        <url>http://localhost:8081/repository/maven-public/</url>
    </mirror>
</mirrors>
```
为了避免阿里云Maven私服地址的影响，建议先将之前配置的阿里云Maven私服镜像地址注释掉，等练习完后，再将其恢复。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222021133.png)
至此本地仓库就能与私服进行交互了。

## 6.5 私服资源上传与下载
本地仓库与私服已经建立了连接，接下来我们就需要往私服上传资源和下载资源，具体的实现步骤如下。

步骤1：配置当前工程访问私服、上传资源的具体位置（`pom.xml`）：
```xml
 <!--配置当前工程保存在私服中的具体位置-->
<distributionManagement>
    <repository>
        <!--和maven/settings.xml中server中的id一致，表示使用该id对应的用户名和密码-->
        <id>itheima-release</id>
         <!--release版本上传仓库的具体地址-->
        <url>http://localhost:8081/repository/itheima-release/</url>
    </repository>
    <snapshotRepository>
        <!--和maven/settings.xml中server中的id一致，表示使用该id对应的用户名和密码-->
        <id>itheima-snapshot</id>
        <!--snapshot版本上传仓库的具体地址-->
        <url>http://localhost:8081/repository/itheima-snapshot/</url>
    </snapshotRepository>
</distributionManagement>
```
步骤2：发布资源到私服。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222021804.png)
或者执行Maven命令：
```
mvn deploy
```
**注意**：要发布的项目都需要配置 `distributionManagement` 标签，要么在自己的 `pom.xml` 中配置，要么在其父项目中配置，然后子项目中继承父****项目即可。

发布成功，在私服中就能看到：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222021335.png)
现在发布是在 `itheima-snapshot` 仓库中，如果想发布到 `itheima-release` 仓库中就需要将项目 `pom.xml` 中的 `version` 修改成 `RELEASE` 即可。

如果想删除已经上传的资源，可以在界面上进行删除操作：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222022203.png)
如果私服中没有对应的JAR，会去中央仓库下载，速度很慢。可以配置让私服去阿里云中下载依赖。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202302222022218.png)
至此私服的搭建就已经完成，相对来说有点麻烦，但步骤都比较固定，后期大家如果需要的话，就可以参考上面一步步完成搭建即可。