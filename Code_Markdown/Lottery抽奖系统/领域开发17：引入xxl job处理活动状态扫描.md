
-   分支：[211106_xfg_xxl-job](https://gitcode.net/KnowledgePlanet/Lottery/-/tree/211106_xfg_xxl-job)
-   描述：引入XXL-JOB，分布式任务调度平台，处理需要使用定时任务解决的场景。

## [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#%E4%B8%80%E5%BC%80%E5%8F%91%E6%97%A5%E5%BF%97)一、开发日志

-   **搭建 XXL-JOB 分布式任务调度环境**，这里需要在官网：[https://github.com/xuxueli/xxl-job/](https://github.com/xuxueli/xxl-job/) 下载运行包，按照 Java SpringBoot 修改一些基本配置，项目启动即可。
-   配置 XXL-JOB 的基础使用环境，导入库表、配置文件、验证官网管理，测试任务启动运行
-   **解决第一个分布式任务场景问题**，扫描抽奖活动状态，把审核通过的活动扫描为活动中，把已过期活动中的状态扫描为关闭。后续章节我们还会使用分布式任务调度系统解决其他场景问题。

## [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#%E4%BA%8C%E6%90%AD%E5%BB%BA%E5%88%86%E5%B8%83%E5%BC%8F%E4%BB%BB%E5%8A%A1%E8%B0%83%E5%BA%A6%E7%8E%AF%E5%A2%83xxl-job)二、搭建分布式任务调度环境(XXL-JOB)

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#1-xxl-job-%E7%B3%BB%E7%BB%9F%E7%AE%80%E4%BB%8B)1. XXL-JOB 系统简介

XXL-JOB是一个分布式任务调度平台，其核心设计目标是开发迅速、学习简单、轻量级、易扩展。现已开放源代码并接入多家公司线上产品线，开箱即用。

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#2-%E5%8A%9F%E8%83%BD)2. 功能

-   1、简单：支持通过Web页面对任务进行CRUD操作，操作简单，一分钟上手；
-   2、动态：支持动态修改任务状态、启动/停止任务，以及终止运行中任务，即时生效；
-   3、**调度中心HA**（中心式）：调度采用中心式设计，“调度中心”自研调度组件并支持集群部署，可保证调度中心HA；
-   4、**执行器HA**（分布式）：**任务分布式执行**，任务"执行器"支持集群部署，可保证任务执行HA；
-   5、注册中心: **执行器会周期性自动注册任务**, 调度中心将会自动发现注册的任务并触发执行。同时，也支持手动录入执行器地址；
-   6、弹性扩容缩容：一旦有新执行器机器上线或者下线，下次调度时将会重新分配任务；
-   7、触发策略：提供丰富的任务触发策略，包括：Cron触发、固定间隔触发、固定延时触发、API（事件）触发、人工触发、父子任务触发；
-   8、调度过期策略：调度中心错过调度时间的补偿处理策略，包括：忽略、立即补偿触发一次等；
-   9、阻塞处理策略：调度过于密集执行器来不及处理时的处理策略，策略包括：单机串行（默认）、丢弃后续调度、覆盖之前调度；
-   10、**任务超时控制**：支持自定义任务超时时间，任务运行超时将会主动中断任务；
-   11、任务失败重试：支持自定义任务失败重试次数，当任务失败时将会按照预设的失败重试次数主动进行重试；其中分片任务支持分片粒度的失败重试；
-   12、任务失败告警；默认提供邮件方式失败告警，同时预留扩展接口，可方便的扩展短信、钉钉等告警方式；
-   13、**路由策略**：执行器集群部署时提供丰富的路由策略，包括：第一个、最后一个、轮询、随机、一致性HASH、最不经常使用、最近最久未使用、故障转移、忙碌转移等；
-   14、分片广播任务：执行器集群部署时，任务路由策略选择"分片广播"情况下，一次任务调度将会广播触发集群中所有执行器执行一次任务，可根据分片参数开发分片任务；
-   15、动态分片：分片广播任务以执行器为维度进行分片，支持动态扩容执行器集群从而动态增加分片数量，协同进行业务处理；在进行大数据量业务操作时可显著提升任务处理能力和速度。
-   16、故障转移：任务路由策略选择"故障转移"情况下，如果执行器集群中某一台机器故障，将会自动Failover切换到一台正常的执行器发送调度请求。
-   17、任务进度监控：支持实时监控任务进度；
-   18、Rolling实时日志：支持在线查看调度结果，并且支持以Rolling方式实时查看执行器输出的完整的执行日志；
-   19、GLUE：提供Web IDE，支持在线开发任务逻辑代码，动态发布，实时编译生效，省略部署上线的过程。支持30个版本的历史版本回溯。
-   20、脚本任务：支持以GLUE模式开发和运行脚本任务，包括Shell、Python、NodeJS、PHP、PowerShell等类型脚本;
-   21、命令行任务：原生提供通用命令行任务Handler（Bean任务，"CommandJobHandler"）；业务方只需要提供命令行即可；
-   22、任务依赖：支持配置子任务依赖，当父任务执行结束且执行成功后将会主动触发一次子任务的执行, 多个子任务用逗号分隔；
-   23、一致性：“调度中心”通过DB锁保证集群分布式调度的一致性, 一次任务调度只会触发一次执行；
-   24、自定义任务参数：支持在线配置调度任务入参，即时生效；
-   25、调度线程池：调度系统多线程触发调度运行，确保调度精确执行，不被堵塞；
-   26、数据加密：调度中心和执行器之间的通讯进行数据加密，提升调度信息安全性；
-   27、邮件报警：任务失败时支持邮件报警，支持配置多邮件地址群发报警邮件；
-   28、推送maven中央仓库: 将会把最新稳定版推送到maven中央仓库, 方便用户接入和使用;
-   29、运行报表：支持实时查看运行数据，如任务数量、调度次数、执行器数量等；以及调度报表，如调度日期分布图，调度成功分布图等；
-   30、全异步：任务调度流程全异步化设计实现，如异步调度、异步运行、异步回调等，有效对密集调度进行流量削峰，理论上支持任意时长任务的运行；
-   31、跨语言：调度中心与执行器提供语言无关的 RESTful API 服务，第三方任意语言可据此对接调度中心或者实现执行器。除此之外，还提供了 “多任务模式”和“httpJobHandler”等其他跨语言方案；
-   32、国际化：调度中心支持国际化设置，提供中文、英文两种可选语言，默认为中文；
-   33、容器化：提供官方docker镜像，并实时更新推送dockerhub，进一步实现产品开箱即用；
-   34、线程池隔离：调度线程池进行隔离拆分，慢任务自动降级进入"Slow"线程池，避免耗尽调度线程，提高系统稳定性；
-   35、用户管理：支持在线管理系统用户，存在管理员、普通用户两种角色；
-   36、权限控制：执行器维度进行权限控制，管理员拥有全量权限，普通用户需要分配执行器权限后才允许相关操作；

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#3-%E5%9F%BA%E7%A1%80%E9%85%8D%E7%BD%AE)3. 基础配置

1.  下载：[2.3.0](https://github.com/xuxueli/xxl-job/releases/tag/2.3.0)
2.  打开：使用 IDEA 打开下载的 xxl-job
3.  导表：把 xxl-job 中的 `doc\db\tables_xxl_job.sql` 导入到自己的数据库中
4.  启动：xxl-job-admin 是用于管理分布式任务调度的后台，一切配置完后，启动 xxl-job-admin 另外你需要配置 application.properties 修改数据库链接参数和日志文件夹
5.  案例：xxl-job-executor-samples 是一组job任务案例，运行后可以在分布式任务调度后台管理任务，配置、启动、关闭
6.  核心：xxl-job-core

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#4-%E5%90%AF%E5%8A%A8%E6%9C%8D%E5%8A%A1xxl-job-admin)4. 启动服务(xxl-job-admin)

1.  启动前检查好 application.properties 中的端口号
2.  确保数据库表已经初始化完成，并修改 application.properties 中数据库链接信息
3.  修改 logback.xml 日志打印目录，否则日志找不到会报错

**执行启动**

```
@SpringBootApplication
public class XxlJobAdminApplication {

	public static void main(String[] args) {
        SpringApplication.run(XxlJobAdminApplication.class, args);
	}

}
```

-   像启动 SpringBoot 程序一样，启动这个 xxl-job 任务调度后台，如果你是部署到云服务器，则需要打包构架部署

**访问页面**

-   地址：http://localhost:`7397`/xxl-job-admin `我修改端口为7397了，你按照自己的端口就可以`
-   账号：admin
-   密码：123456

**页面效果**

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-01.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-01.png)

-   这个就是 xxl-job 的任务调度后台，你可以在这里查看和配置任务。_如果有一些特殊的功能需要使用，可以查看官网文档_

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#5-%E5%90%AF%E5%8A%A8%E4%BB%BB%E5%8A%A1)5. 启动任务

-   当任务调度后台启动后，则可以启动测试任务，这样就可以通过任务调度后来管理任务了。

**任务类 com.xxl.job.executor.service.jobhandler.SampleXxlJob**

```
@Component
public class SampleXxlJob {

    private Logger logger = LoggerFactory.getLogger(SampleXxlJob.class);

    /**
     * 1、简单任务示例（Bean模式）
     */
    @XxlJob("demoJobHandler")
    public void demoJobHandler() throws Exception {
        logger.info("XXL-JOB, Hello World.");

        for (int i = 0; i < 5; i++) {
            XxlJobHelper.log("beat at:" + i);
            TimeUnit.SECONDS.sleep(2);
        }
        // default success
    }

}
```

-   这里面使用任务的方式也比较多，我们目前先看这个最简单的方式，因为这个基本就够我们使用了。

**xxl-job-executor-samples#xxl-job-executor-sample-springboot**

```
@SpringBootApplication
public class XxlJobExecutorApplication {

	public static void main(String[] args) {
        SpringApplication.run(XxlJobExecutorApplication.class, args);
	}

}
```

-   正常启动即可，运行后就可以在任务调度后台管理任务了。

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#6-%E6%9F%A5%E7%9C%8B%E4%BB%BB%E5%8A%A1)6. 查看任务

**地址**：[http://localhost:7397/xxl-job-admin/jobinfo?jobGroup=1](http://localhost:7397/xxl-job-admin/jobinfo?jobGroup=1)

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-02.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-02.png)

-   在任务管理中，可以新增任务，并配置任务的执行时间、以及测试等功能。

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#7-%E6%89%A7%E8%A1%8C%E5%99%A8%E7%AE%A1%E7%90%86)7. 执行器管理

-   所有的任务配置，都需要先配置执行器，也就是你哪个应用实例执行任务。

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-03.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-03.png)

-   每一个执行器下就会对应一组带执行的 job 任务，来完成我们自己的任务调度。

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#8-%E6%B5%8B%E8%AF%95%E6%95%88%E6%9E%9C)8. 测试效果

**启动任务**

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-05.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-05.png)

-   你可以在任务管理中，操作任务的执行。除了配置好多久执行一次的 CRON 表达式外，还可以点击执行1次，这样会立即执行，方便测试。

**运行效果**

```
14:54:40.141 logback [xxl-rpc, EmbedServer bizThreadPool-979195415] INFO  c.x.job.core.executor.XxlJobExecutor - >>>>>>>>>>> xxl-job regist JobThread success, jobId:1, handler:com.xxl.job.core.handler.impl.MethodJobHandler@5327a06e[class com.xxl.job.executor.service.jobhandler.SampleXxlJob#demoJobHandler]
14:54:40.145 logback [Thread-15] INFO  c.x.j.e.s.jobhandler.SampleXxlJob - XXL-JOB, Hello World.
14:54:50.170 logback [Thread-15] INFO  c.x.j.e.s.jobhandler.SampleXxlJob - XXL-JOB, Hello World.
14:55:00.201 logback [Thread-15] INFO  c.x.j.e.s.jobhandler.SampleXxlJob - XXL-JOB, Hello World.
14:55:10.222 logback [Thread-15] INFO  c.x.j.e.s.jobhandler.SampleXxlJob - XXL-JOB, Hello World.
14:55:20.244 logback [Thread-15] INFO  c.x.j.e.s.jobhandler.SampleXxlJob - XXL-JOB, Hello World.
14:55:30.266 logback [Thread-15] INFO  c.x.j.e.s.jobhandler.SampleXxlJob - XXL-JOB, Hello World.
```

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-04.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-04.png)

-   这是任务执行后，在后台输出的结果。

## [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#%E4%B8%89%E4%BB%BB%E5%8A%A1%E6%89%AB%E6%8F%8F%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81)三、任务扫描活动状态

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#1-%E5%BC%95%E5%85%A5pom)1. 引入POM

```
<!-- xxl-job-core https://github.com/xuxueli/xxl-job/-->
<dependency>
    <groupId>com.xuxueli</groupId>
    <artifactId>xxl-job-core</artifactId>
    <version>2.3.0</version>
</dependency>
```

-   把需要使用 xxl-job 的包，引入对应的 POM 配置

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#2-%E9%85%8D%E7%BD%AE-applicationyml)2. 配置 application.yml

```
# xxl-job
# 官网：https://github.com/xuxueli/xxl-job/
# 地址：http://localhost:7397/xxl-job-admin 【需要先启动 xxl-job】
# 账号：admin
# 密码：123456
xxl:
  job:
    admin:
      addresses: http://127.0.0.1:7397/xxl-job-admin
    executor:
      address:
      appname: lottery-job
      ip:
      port: 9999
      logpath: /Users/fuzhengwei/itstack/data/applogs/xxl-job/jobhandler
      logretentiondays: 50
    accessToken:
```

-   配置中主要包括你的服务地址，应用名称、日志路径等

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#3-%E4%BB%BB%E5%8A%A1%E5%88%9D%E5%A7%8B%E7%B1%BB)3. 任务初始类

**cn.itedus.lottery.application.worker.LotteryXxlJobConfig**

```
@Configuration
public class LotteryXxlJobConfig {

    private Logger logger = LoggerFactory.getLogger(LotteryXxlJobConfig.class);

    @Value("${xxl.job.admin.addresses}")
    private String adminAddresses;

    @Value("${xxl.job.accessToken}")
    private String accessToken;

    @Value("${xxl.job.executor.appname}")
    private String appname;

    @Value("${xxl.job.executor.address}")
    private String address;

    @Value("${xxl.job.executor.ip}")
    private String ip;

    @Value("${xxl.job.executor.port}")
    private int port;

    @Value("${xxl.job.executor.logpath}")
    private String logPath;

    @Value("${xxl.job.executor.logretentiondays}")
    private int logRetentionDays;

    @Bean
    public XxlJobSpringExecutor xxlJobExecutor() {
        logger.info(">>>>>>>>>>> xxl-job config init.");

        XxlJobSpringExecutor xxlJobSpringExecutor = new XxlJobSpringExecutor();
        xxlJobSpringExecutor.setAdminAddresses(adminAddresses);
        xxlJobSpringExecutor.setAppname(appname);
        xxlJobSpringExecutor.setAddress(address);
        xxlJobSpringExecutor.setIp(ip);
        xxlJobSpringExecutor.setPort(port);
        xxlJobSpringExecutor.setAccessToken(accessToken);
        xxlJobSpringExecutor.setLogPath(logPath);
        xxlJobSpringExecutor.setLogRetentionDays(logRetentionDays);

        return xxlJobSpringExecutor;
    }

    /**********************************************************************************************
     * 针对多网卡、容器内部署等情况，可借助 "spring-cloud-commons" 提供的 "InetUtils" 组件灵活定制注册IP；
     *
     *      1、引入依赖：
     *          <dependency>
     *             <groupId>org.springframework.cloud</groupId>
     *             <artifactId>spring-cloud-commons</artifactId>
     *             <version>${version}</version>
     *         </dependency>
     *
     *      2、配置文件，或者容器启动变量
     *          spring.cloud.inetutils.preferred-networks: 'xxx.xxx.xxx.'
     *
     *      3、获取IP
     *          String ip_ = inetUtils.findFirstNonLoopbackHostInfo().getIpAddress();
     **********************************************************************************************/

}
```

-这里需要启动一个任务执行器，通过配置 @Bean 对象的方式交给 Spring 进行管理。

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#4-%E5%BC%80%E5%8F%91%E6%B4%BB%E5%8A%A8%E6%89%AB%E6%8F%8F%E4%BB%BB%E5%8A%A1)4. 开发活动扫描任务

**cn.itedus.lottery.application.worker.LotteryXxlJob**

```
@Component
public class LotteryXxlJob {

    private Logger logger = LoggerFactory.getLogger(LotteryXxlJob.class);

    @Resource
    private IActivityDeploy activityDeploy;

    @Resource
    private IStateHandler stateHandler;

    @XxlJob("lotteryActivityStateJobHandler")
    public void lotteryActivityStateJobHandler() throws Exception {
        logger.info("扫描活动状态 Begin");

        List<ActivityVO> activityVOList = activityDeploy.scanToDoActivityList(0L);
        if (activityVOList.isEmpty()){
            logger.info("扫描活动状态 End 暂无符合需要扫描的活动列表");
            return;
        }

        while (!activityVOList.isEmpty()) {
            for (ActivityVO activityVO : activityVOList) {
                Integer state = activityVO.getState();
                switch (state) {
                    // 活动状态为审核通过，在临近活动开启时间前，审核活动为活动中。在使用活动的时候，需要依照活动状态核时间两个字段进行判断和使用。
                    case 4:
                        Result state4Result = stateHandler.doing(activityVO.getActivityId(), Constants.ActivityState.PASS);
                        logger.info("扫描活动状态为活动中 结果：{} activityId：{} activityName：{} creator：{}", JSON.toJSONString(state4Result), activityVO.getActivityId(), activityVO.getActivityName(), activityVO.getCreator());
                        break;
                    // 扫描时间已过期的活动，从活动中状态变更为关闭状态
                    case 5:
                        if (activityVO.getEndDateTime().before(new Date())){
                            Result state5Result = stateHandler.close(activityVO.getActivityId(), Constants.ActivityState.DOING);
                            logger.info("扫描活动状态为关闭 结果：{} activityId：{} activityName：{} creator：{}", JSON.toJSONString(state5Result), activityVO.getActivityId(), activityVO.getActivityName(), activityVO.getCreator());
                        }
                        break;
                    default:
                        break;
                }
            }

            // 获取集合中最后一条记录，继续扫描后面10条记录
            ActivityVO activityVO = activityVOList.get(activityVOList.size() - 1);
            activityVOList = activityDeploy.scanToDoActivityList(activityVO.getId());
        }

        logger.info("扫描活动状态 End");

    }

}
```

在任务扫描中，主要把已经审核通过的活动和已过期的活动中状态进行变更操作；

-   审核通过 -> 扫描为活动中
-   活动中已过期时间 -> 扫描为活动关闭

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#5-%E9%85%8D%E7%BD%AE%E6%8A%BD%E5%A5%96%E7%B3%BB%E7%BB%9F%E4%BB%BB%E5%8A%A1%E8%B0%83%E5%BA%A6%E6%89%A7%E8%A1%8C%E5%99%A8)5. 配置抽奖系统任务调度执行器

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-06.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-06.png)

-   只有配置了任务执行器，才能执行当前这个实例中的任务。
-   另外在有些业务体量较大的场景中，需要把任务开发为新工程并单独部署。

### [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#6-%E9%85%8D%E7%BD%AE%E4%BB%BB%E5%8A%A1)6. 配置任务

-   这里我们把已经开发了的任务 `LotteryXxlJob#lotteryActivityStateJobHandler` 配置到任务调度中心，如下：

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-07.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-07.png)

-   配置完成后，就可以启动任务了。

## [](https://gitcode.net/KnowledgePlanet/Lottery/-/wikis/%E7%AC%AC-2-%E9%83%A8%E5%88%86-%E9%A2%86%E5%9F%9F%E5%BC%80%E5%8F%91//%E7%AC%AC17%E8%8A%82%EF%BC%9A%E5%BC%95%E5%85%A5xxl-job%E5%A4%84%E7%90%86%E6%B4%BB%E5%8A%A8%E7%8A%B6%E6%80%81%E6%89%AB%E6%8F%8F#%E5%9B%9B%E6%B5%8B%E8%AF%95%E9%AA%8C%E8%AF%81)四、测试验证

**准备数据**

-   确保数据库中有可以扫描的活动数据，比如可以把活动数据从活动中扫描为结束，也就是把状态5变更为7

**启动任务**

```
12:35:37.175  INFO 23141 --- [Pool-1090755084] c.xxl.job.core.executor.XxlJobExecutor   : >>>>>>>>>>> xxl-job regist JobThread success, jobId:2, handler:com.xxl.job.core.handler.impl.MethodJobHandler@19a20bb2[class cn.itedus.lottery.application.worker.LotteryXxlJob#lotteryActivityStateJobHandler]
12:35:37.180  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:38.013  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:39.012  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:40.013  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:41.009  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:42.012  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:43.014  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:44.011  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:45.016  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:46.012  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
12:35:47.008  INFO 23141 --- [      Thread-18] c.i.l.application.worker.LotteryXxlJob   : 扫描活动状态，把审核通过的活动，扫描成活动中
```

[![](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-07.png)](https://gitcode.net/KnowledgePlanet/Lottery/-/raw/master/doc/assets/img/Part-2/17-07.png)

-   此时就已经把活动状态为5的已过期的活动，扫描为关闭。
-   下一节我们会继续开发分布式任务调度，完成发奖数据MQ补偿处理。

---

1.  学习 xxl-job 的使用，如果你对这类技术的源码感兴趣，也可以阅读小傅哥关于 [SpringBoot中间件的设计和开发](https://juejin.cn/book/6940996508632219689) 这里就包括了分布式任务调度的设计和实现
2.  如果你之前还没有接触过类似分布式任务的内容，可以好好使用下，补全这部分内容。