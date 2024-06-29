-   **加入项目**：[https://t.zsxq.com/jAi2nUf (opens new window)](https://t.zsxq.com/jAi2nUf)- 加入后在星球置顶消息可以申请加入项目组，`公众号：bugstack虫洞栈 回复：星球 可以获得加入优惠券`
-   **代码仓库**：[https://gitcode.net/KnowledgePlanet/Lottery (opens new window)](https://gitcode.net/KnowledgePlanet/Lottery)- `必须加入星球：码农会锁，在置顶🔝消息，申请授权后才可以访问。`
-   **小册地址**：[https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/home (opens new window)](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/home)- `小册地址会有专门的学习目录，授权后可访问`

---
写CRUD、堆API、改屎山⛰，熬多少个996也只是成为重复的螺丝钉。如果你希望捅破现有工作的瓶颈，拉高一下对技术的认知，那么就非常需要一个大项目来贯穿常用技术栈的知识体系。**_碎片化断层的背八股文，是解决不了这个事情_**

那怎么办？当然是要肝项目了，肝一个`有互联网技术体系`、`有分布式架构运用`、`有DDD思想和设计模式实践` 的真实场景项目，才能让一个尚未接触此类项目或是长期陷入CURD的熟练工，打开视野，快速成长起来。

**整整半年**，小傅哥都在做这件事情，直到今天才完成整个系统的第一期设计实现和落地。在这个[《分布式抽奖系统》](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#)项目中，我会带着大家以DDD架构和设计模式落地实战的方式，进行`代码开发`、`视频介绍`、`文档说明`的方式讲解和实现分布式抽奖系统，那么这里会涉及到很多DDD的设计思路和设计模式应用，以及互联网大厂开发中所应用到的技术，包括：SpringBoot、Mybatis、Dubbo、MQ、Redis、Mysql、ELK、分库分表、Otter 等。

## [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%E4%B8%80%E3%80%81%E5%92%8B-%E6%92%B8%E4%B8%AA%E9%A1%B9%E7%9B%AE)一、撸个项目？

**在这之前一直有粉丝伙伴问傅哥**，有没有能`上手练习技术的项目`，现在学了这么多技术知识、看了这么多设计模式、搜了这么多架构设计，但这些内容都是怎么结合在一起使用的呢？互联网中的项目架构设计是什么样的呢？我该怎么开始学到什么样才能进大厂呢？

`咋neng呢，撸个项目吧！` 想做个什么项目，如；积分商城、抽奖系统、活动系统、监控系统、技术组件，并且这些项目中用到了哪些技术栈。

最后在大家的意见反馈中，先以开发互联网中C端类项目 **抽奖系统** 开始，这样一个项目可以让大家在系统的`架构搭建`、`功能配置`、`服务开发`中学习到关于一些关于解决`高并发`、`高性能`、`高可用`场景时的技术实践运用。_放心，其他类的互联网项目，我们也会陆续的折腾起来！_

So！基于DDD领域驱动设计的四层架构**抽奖系统**，开始啦！有座，这趟车的你跟上！

## [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%E4%BA%8C%E3%80%81%E5%91%80-%E8%83%BD%E5%AD%A6%E4%B8%9C%E8%A5%BF)二、呀，能学东西！

![图 1-2](https://bugstack.cn/images/article/project/lottery/Part-2/1-02.png)

一个以真实场景 `实践技术栈整合` 开发实际需求的项目，势必会因为要完成需求而引入各项技术栈的使用，也会**由于要解决互联网中C端场景中的三高问题**，而使用相应的技术实现不同类别解决与方案，我们也可以把此类解决方案理解为DDD中的**业务领域模型开发**。在这个设计和开发的过程中会涉及到`架构设计`、`技术应用`、`场景实现`，每一块内容都会有非常多的实践知识，可以让读者学到东西。

### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#_1-%E6%B6%89%E5%8F%8A%E6%8A%80%E6%9C%AF)1. 涉及技术
在此项目中你会学习到互联网公司关于C端项目开发时候用到的一些，技术、架构、规范等内容。由于项目为实战类编程项目，在学习的过程中需要上手操作，小傅哥会把系统的搭建拉不同的分支列为每一个章节进行设计和实现并记录到开发日记中，读者在学习的过程中可以结合这部分内容边看文章边写代码实践。

![](https://bugstack.cn/images/article/about/about-220207-01.png)

-   技术：JDK 1.8、SpringBoot、Mybatis、Dubbo、MQ、Redis、Mysql、ELK、分库分表、Otter、vue、微信公众号、Docker
-   架构：DDD 领域驱动设计、充血模型、设计模式
-   规范：分支提交规范、代码编写规范

**其他所需环境如下(mysql\kafka\zk\redis\xxl-job)：**

![](https://bugstack.cn/images/article/project/lottery/introduce/portainer.png)

-   不只是 DDD 分布式项目开发，你还可以学习到关于 Docker 的实践使用

### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#_2-%E7%B3%BB%E7%BB%9F%E6%9E%B6%E6%9E%84)2. 系统架构
![](https://bugstack.cn/images/article/project/lottery/introduce/introduce-220101-01.png)
-   此系统架构为 DDD 领域驱动设计的四层架构实现方式，以重视代码实现落地的方式向读者介绍和展示如何开发这样的代码。
-   在 Domain 领域层逐步通过拆解系统流程设计，按照职责边界的领域模块进行设计和开发，最终在应用层进行逻辑功能编排。
-   这个系统中会体现出很多的设计模式思想和最终的实现，==只有把 DDD 和设计模式结合起来，才能开发出更加易于扩展和维护的代码结构==。

### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#_3-%E5%88%86%E5%B8%83%E5%BC%8F%E8%AE%BE%E8%AE%A1)3. 分布式设计

![](https://bugstack.cn/images/article/project/lottery/introduce/introduce-220101-02.png)

整体系统架构设计包含了6个工程：

1.  Lottery：**分布式部署的抽奖服务系统**，提供抽奖业务领域功能，**以分布式部署的方式提供 RPC 服务。可部署多个**！
2.  Lottery-API：网关API服务，提供；H5 页面抽奖、公众号开发回复消息抽奖。
3.  Lottery-Front：C端用户系统，vue H5 lucky-canvas 大转盘抽奖界面，讲解 vue 工程创建、引入模块、开发接口、跨域访问和功能实现
4.  Lottery-ERP：B端运营系统，满足运营人员对于活动的查询、配置、修改、审核等操作。
5.  DB-Router：**分库分表路由组件**，开发一个基于 HashMap 核心设计原理，==使用哈希散列+扰动函数的方式，把数据散列到多个库表中的组件，并验证使用==。
6.  Lottery-Test：测试验证系统，用于测试验证RPC服务、系统功能调用的测试系统。

📢 **注意**：db-router-spring-boot-starter 为自研数据库路由组件，**你需要下载代码到本地，自行构建到本地仓库进行使用**。_章节中也会介绍这个路由组件的设计和开发_
### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#_4-%E5%87%9D%E7%BB%83%E6%B5%81%E7%A8%8B%E9%A2%86%E5%9F%9F)4. 凝练流程领域

![](https://bugstack.cn/images/article/project/lottery/introduce/introduce-220101-03.png)

-   **拆解功能流程，提炼领域服务**，一步步教会你把一个业务功能流程如何拆解为**各个职责边界下的领域模块**，在通过把开发好的领域服务在应用层进行串联，提供整个服务链路。
-   通过这样的设计和落地思想，以及把流程化的功能按照面向对象的思路、使用设计模式进行设计，让每一步代码都变得清晰易懂，这样实现出来的代码也就更加易于维护和扩展了。
-   所以，你在这个过程中学会的不只是代码开发，还有更多的落地思想实践在这里面体现出来。也能为你以后开发这样的一个项目或者在面试过程中，一些实际复杂场景问题的设计思路，打下不错的基础。

## [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%E4%B8%89%E3%80%81%E5%91%90-%E6%80%8E%E4%B9%88%E5%BC%80%E5%A7%8B)三、呐，怎么开始？
### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%F0%9F%94%9C-%E5%BF%AB%E9%80%9F%E5%8A%A0%E5%85%A5)🔜 快速加入
为了能让读者伙伴快速🔜进入项目学习，可以按照下面的步骤开始：
2.  【入口】[Lottery (opens new window)](https://gitcode.net/KnowledgePlanet/Lottery)项目主入口中有一个 README.md 有关于项目的学习说明、开发规范、章节目录和问题交流提交issue说明以及群内交流，在学习的过程中可以参考使用。
3.  【文章】每一个章节内容中都会包括；**需求、实现、验证、细节**，四块内容的介绍，以及当前章节中对应的代码分支可以切换学习。
4.  【代码】在代码学习的过程中**可以克隆工程进行开发练习**，也可以给主工程小傅哥工程代码提交PR、ISSUE，我会去审核和合并以及不断的完善代码。

### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%F0%9F%93%9D-%E5%AD%A6%E4%B9%A0%E8%AF%B4%E6%98%8E-%E5%B0%8F%E5%86%8C%E3%80%81%E6%BA%90%E7%A0%81%E3%80%81%E8%A7%86%E9%A2%91)📝 学习说明(小册、源码、视频)
课程包括：`小册`、`视频`、`代码`、`作业`，四方面结合的方式进行学习，所以也能让即使缺少编码经验的在校学生、应届生或者是CRUD熟练工，都能快速加入项目进行学习。

![](https://bugstack.cn/images/article/project/lottery/introduce/introduce-220101-04.png)

-   课程分为：`大厂规范`、`领域开发`、`运营后台`、`应用场景`、`系统运维`，共5章34节来讲解DDD分布式系统的架构设计和实践落地。
- 只要你能认真跟着敲下来，22年的 Offer 不会便宜！_真的是研发能力有差异吗，其实不是，你差的只是一个有人带着你肝的大型系统而已！_

**Lottery 抽奖系统** - 基于领域驱动设计的四层架构实践 `只有加入知识星球：码农会锁 申请加入项目组以下链接才能访问`
1.  代码：[https://gitcode.net/KnowledgePlanet/Lottery(opens new window)](https://gitcode.net/KnowledgePlanet/Lottery)
2.  小册：[https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/home(opens new window)](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/home)
3.  提问：[https://gitcode.net/KnowledgePlanet/Lottery/-/issues(opens new window)](https://gitcode.net/KnowledgePlanet/Lottery/-/issues)
4.  介绍：[https://articles.zsxq.com/id_tz44w3oqjftv.html(opens new window)](https://articles.zsxq.com/id_tz44w3oqjftv.html)

注意：在项目学习的过程中，如果遇到问题可以先查看 issue，一般大家的共性问题都会在这里体现。如果你没有搜到与你相同的问题，也可以提一个新的 issue，可以包括：学习疑惑、Bug提醒、优化建议、技术分享等。
### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%F0%9F%8E%A8-%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE)🎨 环境配置
-   **技术栈项**：JDK1.8、Maven3.6.3、Mysql5.7(可升级配置)，SpringBoot、Mybatis、Dubbo 随POM版本
-   **初始打包**：你需要在 Lottery 工程的 Maven 根上，点击 Install 这样才能完整打包，否则 Lottery-Test 等，不能正常引入 Jar 包
-   **建表语句**：[doc/asserts/sql (opens new window)](https://gitcode.net/KnowledgePlanet/Lottery/-/blob/master/doc/assets/sql/lottery.sql)- `建议随非分支内sql版本走，因为需求不断迭代升级优化，直接使用最新的会遇到在各个分支下的代码运行问题`
-   **学习使用**：下载代码库后，切换本地分支到wiki中章节对应的分支，这样代码与章节内容是对应的，否则你在master看到的是全量代码。
-   **下载依赖**：[db-router-spring-boot-starter (opens new window)](https://gitcode.net/KnowledgePlanet/db-router-spring-boot-starter)本项目依赖自研分库分表组件，需要可以用IDEA像打开一个项目一样打开，之后点击 Maven Install 这样就把 Jar 打包到你本地仓库了，Lottery 就可以引入这个 Jar 了

### [#](https://bugstack.cn/md/project/lottery/introduce/Lottery%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F.html#%F0%9F%93%90-%E5%BC%80%E5%8F%91%E8%A7%84%E8%8C%83)📐 开发规范
**分支命名**：日期_姓名首字母缩写_功能单词，如：`210804_xfg_buildFramework`

**提交规范**：`作者，type: desc` 如：`小傅哥，fix：修复查询用户信息逻辑问题` _参考Commit message 规范_
```
# 主要type
feat:     增加新功能
fix:      修复bug

# 特殊type
docs:     只改动了文档相关的内容
style:    不影响代码含义的改动，例如去掉空格、改变缩进、增删分号
build:    构造工具的或者外部依赖的改动，例如webpack，npm
refactor: 代码重构时使用
revert:   执行git revert打印的message

# 暂不使用type
test:     添加测试或者修改现有测试
perf:     提高性能的改动
ci:       与CI（持续集成服务）有关的改动
chore:    不修改src或者test的其余修改，例如构建过程或辅助工具的变动
```


## 贫血模型 & 充血模型 设计对比
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305232104722.png)
