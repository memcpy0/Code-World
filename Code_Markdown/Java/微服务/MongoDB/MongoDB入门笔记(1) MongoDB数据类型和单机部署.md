# 课程目标
理解MongoDB的**业务场景**、熟悉MongoDB的简介、特点和**体系结构**、**数据类型等**。
掌握MongoDB基本**常用命令**实现数据的CRUD
掌握MongoDB的基础操作和聚合操作
==掌握MongoDB的**索引类型**、索引管理、**执行计划**==。
使用**Spring Data MongoDB**完成文章评论业务的开发

---
# 1. MongoDB相关概念
## 1.1 业务应用场景
 传统的关系型数据库（如MySQL），在数据操作的**“三高”需求**以及应对Web2.0的网站需求面前，显得力不从心。解释：“三高”需求：
- High performance - 对数据库**高并发 读写**的需求。
- Huge Storage - 对海量数据的***高效率* 存储和访问**的需求。
- ==High Scalability && High Availability- 对数据库的**高可扩展性**和**高可用性**的需求==。

而MongoDB可应对“三高”需求。具体的应用场景如：
 1. 社交场景，使用MongoDB存储存储用户信息，以及用户发表的朋友圈信息，通过**地理位置索引**实现附近的人、地点等功能。
 2. 游戏场景，==使用MongoDB存储游戏用户信息，用户的装备、积分等直接以**内嵌文档**的形式存储==，方便查询、高效率存储和访问。
 3. 物流场景，使用MongoDB存储订单信息，==订单状态在运送过程中会不断更新，以**MongoDB内嵌数组**的形式来存储，**一次查询就能将订单所有的变更读取出来**==。
 4. 物联网场景，使用MongoDB存储所有接入的智能设备信息，以及设备汇报的日志信息，并对这些信息进行**多维度的分析**。
 5. 视频直播，使用MongoDB存储用户信息、点赞互动信息等。

使用场景
大数据量存储场景
MongoDB自带副本集和分片，天生就适用于大数量场景，无需开发人员通过中间件去分库分表，非常方便。

操作日志存储
很多时候，我们需要存储一些操作日志，可能只需要存储比如最近一个月的，一般的做法是定期去清理，在MongoDB中有固定集合的概念，我们在创建集合的时候可以指定大小，当数据量超过大小的时候会自动移除掉老数据。

爬虫数据存储
爬下来的数据有网页，也有Json格式的数据，一般都会按照表的格式去存储，如果我们用了MongoDB就可以将抓下来的Json数据直接存入集合中，无格式限制。

社交数据存储
在社交场景中使用 MongoDB 存储存储用户地址位置信息，通过地理位置索引实现附近的人，附近的地点等。

电商商品存储
不同的商品有不同的属性，常见的做法是抽出公共的属性表，然后和SPU进行关联，如果用MongoDB的话那么SPU中直接就可以内嵌属性。 

这些应用场景中，数据操作方面的共同特点是：
1. **数据量大**
2. 写入操作频繁（**读写都很频繁**）
3. **价值较低的数据**，对事务性要求不高

对于这样的数据，我们更适合使用MongoDB来实现数据的存储。
## 1.2 什么时候选择MongoDB
在架构选型上，除了上述的三个特点外，如果你还犹豫是否要选择它？可以考虑以下的一些问题：
- **应用不需要事务及复杂JOIN支持**
- 新应用，需求会变，**数据模型无法确定**，想**快速迭代开发**
- **应用需要2000-3000以上的读写QPS**（更高也可以）
- **应用需要TB甚至PB级别数据存储**
- 应用发展迅速，需要**能快速水平扩展**
- 应用要求存储的数据不丢失
- 应用**需要99.999%高可用**
- 应用**需要大量的地理位置查询、文本查询**

如果上述有1个符合，可以考虑MongoDB， 2 个及以上的符合，选择MongoDB绝不会后悔。

思考：如果用MySQL呢？
答：相对MySQL，可以以更低的成本解决问题（包括学习、开发、运维等成本）

## 1.3 MongoDB简介
MongoDB是一个开源、**高性能、无模式的文档型数据库**，（以 JSON 为数据模型），由C++语言编写，当初的设计就是用于简化开发和方便扩展，是NoSQL数据库产品中的一种。是最像关系型数据库（MySQL）的非关系型数据库。旨在为WEB应用提供可扩展的 高性能数据存储解决方案。 

MongoDB是一个介于关系数据库和非关系数据库之间的产品，是非关系数据库当中功能最丰富，最像关系数据库的。它支持的数据结构非常松散，是一种**类似于JSON的格式叫BSON**，一种类似JSON的二进制形式的存储格式，简称BinaryJSON，**和JSON一样支持内嵌的文档对象和数组对象**，因此可以存储比较复杂的数据类型，又相当的灵活。

MongoDB最大的特点是它支持的查询语言非常强大，其语法有点类似于面向对象的查询语言，几乎可以实现类似关系数据库**单表查询的绝大部分功能**，而且**还支持对数据建立索引**。

原则上Oracle和MySQL能做的事情，MongoDB都能做（包括ACID事务）。MongoDB在数据库总排名第5，仅次于Oracle、MySQL等RDBMS，在NoSQL数据库排名首位。从诞生以来，其项目应用广度、社区活跃指数持续上升。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307100952335.png)

**MongoDB中的记录是一个文档**，它是一个由字段和值对 `field : value` 组成的数据结构。==MongoDB文档类似于JSON对象，即**一个文档认为就是一个对象**==。字段的数据类型是字符型，它的值除了使用基本的一些类型外，还可以包括其他文档、普通数组和文档数组。
## 1.4 体系结构
MySQL和MongoDB对比
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303090204184.png)


| SQL术语/概念 | MongoDB术语/概念|  解释/说明 |
|:---|:---|:---|
| database | database | 数据库
| table | collection| 数据库表/集合
| row | document | 数据记录行/文档
| column | field | 数据字段/域
| index | index | 索引
| table joins|  | 表连接，MongoDB不支持|   
|  |  嵌入文档 | MongoDB通过嵌入式文档来替代多表连接
| primary key| primary key  | 主键，MongoDB自动将 `_id` 字段设置为主键

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202307100953337.png)

MongoDB概念与关系型数据库（RDBMS）非常类似：
- 数据库（database）：最外层的概念，可以理解为逻辑上的名称空间，一个数据库包含多个不同名称的集合。
- 集合（collection）：相当于SQL中的表，一个集合可以存放多个不同的文档。
- **文档**（document）：一个文档相当于数据表中的一行，由多个不同的字段组成。
- 字段（field）：文档中的一个属性，等同于列（column）。
- 索引（index）：独立的检索式数据结构，与SQL概念一致。
- **id：每个文档中都拥有一个唯一的id字段**，相当于SQL中的主键（primarykey）。
- 视图（view）：可以看作一种虚拟的（非真实存在的）集合，与SQL中的视图类似。从MongoDB3.4版本开始提供了视图功能，其通过聚合管道技术实现。
- 聚合操作 `$lookup` ：MongoDB用于实现“类似”表连接（tablejoin）的聚合操作符。

尽管这些概念大多与SQL标准定义类似，但MongoDB与传统RDBMS仍然存在不少差异，包括：
- 半结构化，**在一个*集合*中，*文档所拥有的字段*并不需要是相同的，而且也不需要对所用的字段进行声明**。因此，MongoDB具有很明显的半结构化特点。除了松散的表结构，文档还可以支持多级的嵌套、数组等灵活的数据类型，非常契合面向对象的编程模型。
- 弱关系，MongoDB没有外键的约束，也没有非常强大的表连接能力。类似的功能需要使用聚合管道技术来弥补。

```mongos
test> db.createCollection("test1")
{ ok: 1 }
test> db.test1.insertOne({ x : 2 })
{
  acknowledged: true,
  insertedId: ObjectId("64ab6ca55007c73001a222bc")
}
test> db.test1.insert( { x:2 } )
DeprecationWarning: Collection.insert() is deprecated. Use insertOne, insertMany, or bulkWrite.
{
  acknowledged: true,
  insertedIds: { '0': ObjectId("64ab6cb05007c73001a222bd") }
}
```
## 1.5 数据模型
**MongoDB的最小存储单位就是文档 *document* 对象**。文档 *document* 对象对应于关系型数据库的**行**。数据在MongoDB中以BSON（Binary-JSON）文档的格式存储在磁盘上。

==*BSON, Binary Serialized Document Format* 是一种类JSON的、二进制形式的存储格式，简称Binary JSON==。BSON和JSON一样，支持**内嵌的文档对象和数组对象**，但是BSON有JSON没有的一些数据类型，如Date和BinData类型。

BSON采用了类似于C语言结构体的名称、对表示方法，支持内嵌的文档对象和数组对象，具有轻量性、可遍历性、高效性的三个特点，可以有效描述非结构化数据和结构化数据。这种格式的优点是灵活性高，但它的缺点是空间利用率不是很理想。

Bson中除了基本的JSON类型：`string, integer, boolean, double, null, array, object` ，MongoDB还使用了特殊的数据类型。这些类型包括 `date, object id, binary data, regular expression, code` 。每一个驱动都以特定语言的方式实现了这些类型，查看你的驱动的文档来获取详细信息。

BSON数据类型参考列表：

| 数据类型 | 描述 | 举例 |
|:---|:----|:---|
| 字符串 | UTF-8字符串都可表示为字符串类型的数据 | `{"x": "foobar"}`
| 对象id | 对象id是文档的**12 字节的唯一 ID** | `{"X": ObjectId() }`
| 布尔值 | 真或者假：true或者false | `{"x": true}+`
| 数组 | 值的集合或者列表可以表示成数组 | `{"x": ["a", "b", "c"]}`
| 32位整数 | 类型不可用。JavaScript仅支持64位浮点数，所以32位整数会被自动转换。| shell是不支持该类型的，**shell中默认会转换成64位浮点数**
| 64 位整数 | 不支持这个类型。**shell会使用一个特殊的内嵌文档来显示64位整数** | shell是不支持该类型的，shell中默认会转换成64位浮点数
| 64 位浮点数 | shell中的数字就是这一种类型 | `{"x": 3.14159, "y": 3}`
| null | 表示空值或者未定义的对象 | `{"x": null}`
| undefined | 文档中也可以使用未定义类型 | `{"x": undefined}`
| 符号 | shell不支持，**shell会将数据库中的符号类型的数据自动转换成字符串** |
| 正则表达式 | **文档中可以包含正则表达式**，采用JavaScript的正则表达式语法 |  `{"x": /foobar/i}`
| 代码 | **文档中还可以包含JavaScript代码** | `{"x": function() { /* ...... */ }}`
| 二进制数据  |二进制数据可以由任意字节的串组成，不过shell中无法使用 | 
| 最大值/最小值 | BSON包括一个特殊类型，表示可能的最大值。shell中没有这个类型 |

提示：**shell默认使用64位浮点型数值**。`{"x": 3.14}`或 `{"x": 3}` 。对于整型值，可以使用 `NumberInt`（4字节符号整数）或 `NumberLong`（8字节符号整数），`{"x": NumberInt("3")} {"x": NumberLong("3")}` 。

## 1.5 MongoDB的特点
MongoDB主要有如下特点：
1. 高性能：MongoDB提供高性能的数据持久性。特别是，对嵌入式数据模型的支持减少了数据库系统上的I/O活动。
	索引支持更快的查询，并且可以包含来自嵌入式文档和数组的键（**文本索引解决搜索的需求**、TTL索引解决**历史数据自动过期**的需求、地理位置索引可用于构建各种O2O应用），mmapv1、wiredtiger、mongorocks（rocksdb）、in-memory 等多引擎支持满足各种场景需求。Gridfs解决文件存储的需求。
2. 高可用性：==MongoDB的复制工具称为副本集==（replica set），它可提供自动故障转移和数据冗余。
3. 高扩展性：==MongoDB提供了水平可扩展性作为其核心功能的一部分==。分片将数据分布在一组集群的机器上（海量数据存储，服务能力水平扩展）。
	从3.4开始，MongoDB支持基于片键创建数据区域。在一个平衡的集群中，MongoDB将一个区域所覆盖的读写、只定向到该区域内的那些片。
4. 丰富的查询支持：==MongoDB支持丰富的查询语言==，支持读和写操作(CRUD)，比如数据聚合、文本搜索和地理空间查询等。
5. 其他特点：如无模式（动态模式）、灵活的文档模型、

---
# 2. 单机部署
## 2.1 Windows系统中的安装启动
第一步：下载安装包。MongoDB提供了可用于 32 位和 64 位系统的预编译二进制包，你可以从MongoDB官网下载安装，MongoDB 预编译二进制包下载地址：
https://www.mongodb.com/download-center#community
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303090217170.png)
根据上图所示下载zip包。

提示：版本的选择，MongoDB的版本命名规范如：x.y.z；
- y为奇数时表示当前版本为开发版，如：1.5.2、4.1.13；
- y为偶数时表示当前版本为稳定版，如：1.6.3、4.0.10；
- z是修正版本号，数字越大越好。

详情：http://docs.mongodb.org/manual/release-notes/#release-version-numbers

第二步：解压安装启动。将压缩包解压到一个目录中。在解压目录中，手动建立一个目录用于存放数据文件，如data/db。
方式 1 ：命令行参数方式启动服务。在bin目录中打开命令行提示符，输入如下命令：
```bash
$ mongod --dbpath=..\data\db
```
我们在启动信息中可以看到，mongoDB的默认端口是27017 ，如果我们想改变默认的启动端口，可以通过 `--port` 来指定端口。

**为了方便我们每次启动，可以将安装目录的bin目录设置到环境变量的path中**，bin目录下是一些常用命令，比如**mongod启动服务用的，mongo客户端连接服务用的**。

方式2：配置文件方式启动服务。在解压目录中新建config文件夹，该文件夹中新建配置文件mongod.conf，内如参考如下：
```java
storage:
	#The directory where the mongod instance stores its data.Default Value is "\data\db" on Windows.
	dbPath: D:\02_Server\DBServer\mongodb-win32-x86_64-2008plus-ssl-4.0.1\data
```
详细配置项内容可以参考官方文档：https://docs.mongodb.com/manual/reference/configuration-options/

【注意】
(1) 配置文件中如果使用双引号，比如路径地址，自动会将双引号的内容转义。如果不转义，则会报错：
```cpp
error-parsing-yaml-config-file-yaml-cpp-error-at-line-3-column-15-unknown-escape-character-d
```
解决：
a. 对 `\` 换成 `/` 或 `\\`
b. 如果路径中没有空格，则无需加引号。

(2) 配置文件中不能以Tab分割字段
解决：将其转换成空格。

启动方式：
```bash
mongod -f ../config/mongod.conf
或
mongod --config ../config/mongod.conf
```
更多参数配置：
```bash
systemLog:
	destination: file
	#The path of the log file to which mongod or mongos should send all diagnostic logging information
	path: "D:/02_Server/DBServer/mongodb-win32-x86_64-2008plus-ssl-4.0.1/log/mongod.log"
	logAppend: true
storage:
	journal:
		enabled: true
	#The directory where the mongod instance stores its data.Default Value is "/data/db".
	dbPath: "D:/02_Server/DBServer/mongodb-win32-x86_64-2008plus-ssl-4.0.1/data"
net:
	#bindIp: 127.0.0.1
	port: 27017
setParameter:
	enableLocalhostAuthBypass: false
```
## 2.2 Shell连接（mongo命令）
在命令提示符输入以下shell命令即可完成登陆
```bash
mongo
或
mongo --host=127.0.0.1 --port=27017
```
查看已经有的数据库
```bash
>show databases
```
退出mongodb
```bash
exit
```
更多参数可以通过帮助查看：
```java
mongo --help
```
提示：MongoDB javascript shell是一个基于Javascript的解释器，故是支持JS程序的。

## 2.3 Compass图形化界面客户端
到MongoDB官网下载MongoDB Compass，地址：https://www.mongodb.com/download-center/v2/compass?initial=true

如果是下载安装版，则按照步骤安装；如果是下载压缩版，直接解压，执行里面的 `MongoDBCompassCommunity.exe` 文件即可。

在打开的界面中，输入主机地址、端口等相关信息，点击连接：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303090227551.png)
## 2.4 Linux系统中的安装启动和连接
目标：在Linux中部署一个单机的MongoDB，作为生产环境下使用。
提示：和Windows下操作差不多。
步骤如下：
1.先到官网下载压缩包 `mongod-linux-x86_64-4.0.10.tgz`。
2.上传压缩包到Linux中，解压到当前目录：
```bash
tar -xvf mongodb-linux-x86_64-4.0.10.tgz
```
3.移动解压后的文件夹到指定的目录中：
```bash
mv mongodb-linux-x86_64-4.0.10 /usr/local/mongodb
```
4.新建几个目录，分别用来存储数据和日志：
```bash
#数据存储目录
mkdir -p /mongodb/single/data/db
#日志存储目录
mkdir -p /mongodb/single/log
```
5.新建并修改配置文件
```bash
vi /mongodb/single/mongod.conf
```
配置文件的内容如下：
```bash
systemLog:
	#MongoDB发送所有日志输出的目标指定为文件
	# #The path of the log file to which mongod or mongos should send all diagnostic logging information
	destination: file
	#mongod或mongos应向其发送所有诊断日志记录信息的日志文件的路径
	path: "/mongodb/single/log/mongod.log"
	#当mongos或mongod实例重新启动时，mongos或mongod会将新条目附加到现有日志文件的末尾。
	logAppend: true
storage:
	#mongod实例存储其数据的目录。storage.dbPath设置仅适用于mongod。
	##The directory where the mongod instance stores its data.Default Value is "/data/db".
	dbPath: "/mongodb/single/data/db"
	journal:
		#启用或禁用持久性日志以确保数据文件保持有效和可恢复。
		enabled: true
processManagement:
	#启用在后台运行mongos或mongod进程的守护进程模式。
	fork: true
net:
	#服务实例绑定的IP，默认是localhost
	bindIp: localhost,192.168.0.2
	#bindIp
	#绑定的端口,默认是27017
	port: 27017
```
6.启动MongoDB服务
```bash
[root@bobohost single]# /usr/local/mongodb/bin/mongod -f /mongodb/single/mongod.conf
about to fork child process, waiting until server is ready for connections.
forked process: 90384
child process started successfully, parent exiting
```
注意：如果启动后不是successfully，则是启动失败了。原因基本上就是配置文件有问题。

通过进程来查看服务是否启动了：
```bash
[root@bobohost single]# ps -ef |grep mongod
root 90384 1 0 8月26 ? 00:02:13 /usr/local/mongdb/bin/mongod -f /mongodb/single/mongod.conf
```
 7.分别使用mongo命令和compass工具来连接测试。提示：如果远程连接不上，需要配置防火墙放行，或直接关闭Linux防火墙。
```bash
#查看防火墙状态
systemctl status firewalld
#临时关闭防火墙
systemctl stop firewalld
#开机禁止启动防火墙
systemctl disable firewalld
```
8.停止关闭服务。停止服务的方式有两种：快速关闭和标准关闭，下面依次说明：
(1) 快速关闭方法（快速，简单，数据可能会出错）。目标：通过系统的kill命令直接杀死进程：
```bash
#通过进程编号关闭节点
kill -2 54410
```
杀完要检查一下，避免有的没有杀掉。

【补充】如果一旦是因为数据损坏，则需要进行如下操作（了解）：
1）删除lock文件：
```bash
rm -f /mongodb/single/data/db/*.lock
```
2）修复数据：
```bash
/usr/local/mongdb/bin/mongod --repair --dbpath=/mongodb/single/data/db
```
(2) 标准的关闭方法（数据不容易出错，但麻烦）。目标：通过mongo客户端中的shutdownServer命令来关闭服务。主要的操作步骤参考如下：
```bash
//客户端登录服务，注意，这里通过localhost登录，如果需要远程登录，必须先登录认证才行。
mongo --port 27017
//#切换到admin库
use admin
//关闭服务
db.shutdownServer()
```

---


# 5. 文章评论
## 5.1 需求分析
某头条的文章评论业务如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091143794.png)
文章示例参考：早晨空腹喝水，是对还是错？https://www.toutiao.com/a6721476546088927748/
需要实现以下功能：
1）基本增删改查API
2）根据文章id查询评论
3）评论点赞

## 5.2 表结构分析
数据库：articledb
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091144330.png)

## 5.3 技术选型
### 5.3.1 mongodb-driver（了解）
mongodb-driver是mongo官方推出的java连接mongoDB的驱动包，相当于JDBC驱动。我们通过一个入门的案例来了解mongodb-driver的基本使用。

官方驱动说明和下载：http://mongodb.github.io/mongo-java-driver/
官方驱动示例文档：http://mongodb.github.io/mongo-java-driver/3.8/driver/getting-started/quick-start/

### 5.3.2 SpringDataMongoDB
**SpringData家族成员之一，用于操作MongoDB的持久层框架**，封装了底层的mongodb-driver。

官网主页： https://projects.spring.io/spring-data-mongodb/ 。我们十次方项目的吐槽微服务就采用SpringDataMongoDB框架。

## 5.4 文章微服务模块搭建
（1）搭建项目工程article，pom.xml引入依赖：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-parent</artifactId>
		<version>2.1.6.RELEASE</version>
		<relativePath/> <!-- lookup parent from repository -->
	</parent>
	<groupId>cn.itcast</groupId>
	<artifactId>article</artifactId>
	<version>1.0-SNAPSHOT</version>
	<dependencies>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-test</artifactId>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-data-mongodb</artifactId>
		</dependency>
	</dependencies>
</project>
```
（2）创建application.yml（Tab还是空格）
```yaml
spring:
	#数据源配置
	data:
		mongodb:
			# 主机地址
			host: 192.168.40.141
			# 数据库
			database: articledb
			# 默认端口是27017
			port: 27017
			#也可以使用uri连接
			#uri: mongodb://192.168.40.134:27017/articledb
```
（3）创建启动类 `cn.itcast.article.ArticleApplication` 。
```java
package cn.itcast.article;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
@SpringBootApplication
public class ArticleApplication {
	public static void main(String[] args) {
		SpringApplication.run(ArticleApplication.class, args);
	}
}
```
（4）启动项目，看是否能正常启动，控制台没有错误。
## 5.5 文章评论实体类的编写
创建实体类。创建包cn.itcast.article，包下建包po用于存放实体类，创建实体类 `cn.itcast.article.po.Comment` 。
```java
package cn.itcast.article.po;
import org.springframework.data.annotation.Id;
import org.springframework.data.mongodb.core.index.Indexed;
import org.springframework.data.mongodb.core.mapping.Document;
import org.springframework.data.mongodb.core.mapping.Field;
import java.io.Serializable;
import java.time.LocalDateTime;
import java.util.Date;
/**
 * 文章评论实体类
 */
//用@Document把一个java类声明为mongodb的文档，可以通过collection参数指定这个类对应的文档。
//@Document(collection="mongodb 对应 collection 名")
// 若未加 @Document ，该 bean save 到 mongo 的 comment collection
// 若添加 @Document ，则 save 到 comment collection
@Document(collection="comment") //可以省略，如果省略，则默认使用类名小写映射集合
//复合索引
// @CompoundIndex( def = "{'userid': 1, 'nickname': -1}")
public class Comment implements Serializable {
	//主键标识，该属性的值会自动对应mongodb的主键字段"_id"，如果该属性名就叫“id”,则该注解可以省略，否则必须写
	@Id
	private String id;//主键
	//该属性对应mongodb的字段的名字，如果一致，则无需该注解
	@Field("content")
	private String content; //吐槽内容
	private Date publishtime; //发布日期
	//添加了一个单字段的索引
	@Indexed
	private String userid; //发布人ID
	private String nickname; //昵称
	private LocalDateTime createdatetime ;//评论的日期时间
	private Integer likenum; //点赞数
	private Integer replynum; //回复数
	private String state; //状态
	private String parentid; //上级ID
	private String articleid;
	
	//getter and setter.....
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getContent() {
		return content;
	}
	public void setContent(String content) {
		this.content = content;
	}
	public Date getPublishtime() {
		return publishtime;
	}
	public void setPublishtime(Date publishtime) {
		this.publishtime = publishtime;
	}
	public String getUserid() {
		return userid;
	}
	public void setUserid(String userid) {
		this.userid = userid;
	}
	public String getNickname() {
		return nickname;
	}
	public void setNickname(String nickname) {
		this.nickname = nickname;
	}
	public LocalDateTime getCreatedatetime() {
		return createdatetime;
	}
	public void setCreatedatetime(LocalDateTime createdatetime) {
		this.createdatetime = createdatetime;
	}
	public Integer getLikenum() {
		return likenum;
	}
	public void setLikenum(Integer likenum) {
		this.likenum = likenum;
	}
	public Integer getReplynum() {
		return replynum;
	}
	public void setReplynum(Integer replynum) {
		this.replynum = replynum;
	}
	public String getState() {
		return state;
	}
	public void setState(String state) {
		this.state = state;
	}
	public String getParentid() {
		return parentid;
	}
	public void setParentid(String parentid) {
		this.parentid = parentid;
	}
	public String getArticleid() {
		return articleid;
	}
	public void setArticleid(String articleid) {
		this.articleid = articleid;
	}
	@Override
	public String toString() {
		return "Comment{" +
			"id='" + id + '\'' +
			", content='" + content + '\'' +
			", publishtime=" + publishtime +
			", userid='" + userid + '\'' +
			", nickname='" + nickname + '\'' +
			", createdatetime=" + createdatetime +
			", likenum=" + likenum +
			", replynum=" + replynum +
			", state='" + state + '\'' +
			", parentid='" + parentid + '\'' +
			", articleid='" + articleid + '\'' +
			'}';
	}
}
```
说明：索引可以大大提升查询效率，一般在查询字段上添加索引，索引的添加可以通过Mongo的命令来添加，也可以**在Java的实体类中通过注解添加**。

（1）单字段索引注解 `@Indexed` 。`org.springframework.data.mongodb.core.index.Indexed.class` 。
声明该字段需要索引，建索引可以大大的提高查询效率。Mongo命令参考：
```js
db.comment.createIndex({"userid":1})
```
（2）复合索引注解 `@CompoundIndex` 。`org.springframework.data.mongodb.core.index.CompoundIndex.class` 。复合索引的声明，建复合索引可以有效地提高多字段的查询效率。Mongo命令参考：
```js
db.comment.createIndex({"userid":1,"nickname":-1})
```
## 5.6 文章评论的基本增删改查
（1）创建数据访问接口 `cn.itcast.article` 包下创建dao包，包下创建接口 `cn.itcast.article.dao.CommentRepository` 。在service中使用这个接口。
```java
package cn.itcast.article.dao;
import cn.itcast.article.po.Comment;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.mongodb.repository.MongoRepository;
//评论的持久层接口,泛型是实体类和ID类型
public interface CommentRepository extends MongoRepository<Comment,String> {

}
```
（2）创建业务逻辑类 cn.itcast.article包下创建service包，包下创建类 `cn.itcast.article.service.CommentService` 。Ctrl+D查看注入 `commentRepository` 动态代理的对象——实际上是 `SimpleMongoRepository` 类的对象。
```java
package cn.itcast.article.service;
import cn.itcast.article.dao.CommentRepository;

import cn.itcast.article.po.Comment;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import java.util.List;
//评论的业务层
@Service
public class CommentService {
	//注入dao
	@Autowired
	private CommentRepository commentRepository;
	/**
	 * 保存一个评论
	 * @param comment
	 */
	public void saveComment(Comment comment){
		//如果需要自定义主键，可以在这里指定主键；如果不指定主键，MongoDB会自动生成主键
		//设置一些默认初始值
		//调用dao
		commentRepository.save(comment);
	}
	/**
	 * 更新评论
	 * @param comment
	 */
	public void updateComment(Comment comment){
		//调用dao
		commentRepository.save(comment);
	}
	/**
	 * 根据id删除评论
	 * @param id
	 */
	public void deleteCommentById(String id){
		//调用dao
		commentRepository.deleteById(id);
	}
	/**
	 * 查询所有评论
	 * @return
	 */
	public List<Comment> findCommentList(){
		//调用dao
		return commentRepository.findAll();
	}
	/**
	 * 根据id查询评论
	 * @param id
	 * @return
	 */
	public Comment findCommentById(String id){
		//调用dao
		return commentRepository.findById(id).get();
	}
}
```
（3）新建Junit测试类，测试保存和查询所有：`cn.itcast.article.service.CommentServiceTest` 。
```java
package cn.itcast.article.service;
import cn.itcast.article.ArticleApplication;
import cn.itcast.article.po.Comment;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.data.domain.Page;
import org.springframework.test.context.junit4.SpringRunner;
import java.time.LocalDateTime;
import java.util.List;

//测试评论的业务层
//SpringBoot的Junit集成测试
@RunWith(SpringRunner.class)
//SpringBoot的测试环境初始化，参数：启动类
@SpringBootTest(classes = ArticleApplication.class)
public class CommentServiceTest {
	//注入Service
	@Autowired
	private CommentService commentService;
	/**
	 * 保存一个评论
	 */
	@Test
	public void testSaveComment(){
		Comment comment=new Comment();
		comment.setArticleid("100000");
		comment.setContent("测试添加的数据");
		comment.setCreatedatetime(LocalDateTime.now());
		comment.setUserid("1003");
		comment.setNickname("凯撒大帝");
		comment.setState("1");
		comment.setLikenum(0);
		comment.setReplynum(0);
		commentService.saveComment(comment);
	}
	/**
	 * 查询所有数据
	 */
	@Test
	public void testFindAll(){
		List<Comment> list = commentService.findCommentList();
		System.out.println(list);
	}
	/**
	 * 测试根据id查询
	 */
	@Test
	public void testFindCommentById(){
		Comment comment = commentService.findCommentById("5d6a27b81b8d374798cf0b41");
		System.out.println(comment);
	}
}
```
添加结果：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091203982.png)

## 5.7 根据上级ID查询文章评论的分页列表
（1）CommentRepository新增方法定义
> Spring Data 规范就这样  JPA 和 Mongo 都是实现 当然一样
> 如果不用这种写法呢？
> 这个其实是，jpa自己去帮你实现了这个方法，如果你自己写的话，方法叫什么都行
```java
//根据父id，查询子评论的分页列表
Page<Comment> findByParentid(String parentid, Pageable pageable);
```
（2）CommentService新增方法
```java
/**
* 根据父id查询分页列表
* @param parentid
* @param page
* @param size
* @return
*/
public Page<Comment> findCommentListPageByParentid(String parentid, int page, int size){
	return commentRepository.findByParentid(parentid, PageRequest.of(page-1, size));
}
```
（3）junit测试用例：`cn.itcast.article.service.CommentServiceTest`
```java
/**
* 测试根据父id查询子评论的分页列表
*/
@Test
public void testFindCommentListPageByParentid(){
	Page<Comment> pageResponse = commentService.findCommentListPageByParentid("3", 1, 2);
	System.out.println("----总记录数："+pageResponse.getTotalElements());
	System.out.println("----当前页数据："+pageResponse.getContent());
}
```
（4）测试。使用compass快速插入一条测试数据，数据的内容是对 3 号评论内容进行评论。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091205416.png)

执行测试，结果：
```java
----总记录数：1
----当前页数据：[Comment{id='33', content='你年轻，火力大', publishtime=null, userid='1003', nickname='凯撒大帝',
createdatetime=null, likenum=null, replynum=null, state='null', parentid='3', articleid='100001'}]
```
## 5.8 MongoTemplate实现评论点赞
我们看一下以下点赞的临时示例代码： CommentService 新增updateThumbup方法
```java
/**
 * 点赞-效率低
 * @param id
 */
public void updateCommentThumbupToIncrementingOld(String id){
	Comment comment = CommentRepository.findById(id).get();
	comment.setLikenum(comment.getLikenum()+1);
	CommentRepository.save(comment);
}
```
以上方法虽然实现起来比较简单，但执行效率并不高，因为我只需要将点赞数加 1 就可以了，==没必要查询出所有字段修改后再更新所有字段==。(蝴蝶效应)

我们可以使用MongoTemplate类来实现对某列的操作。 
（1）修改CommentService
```java
//注入MongoTemplate
@Autowired
private MongoTemplate mongoTemplate;
/**
 * 点赞数+1
 * @param id
 */
public void updateCommentLikenum(String id){
	//查询对象
	Query query=Query.query(Criteria.where("_id").is(id));
	//更新对象
	Update update=new Update();
	//局部更新，相当于$set
	// update.set(key,value)
	//递增$inc
	// update.inc("likenum",1);
	update.inc("likenum");
	//参数1：查询对象
	//参数2：更新对象
	//参数3：集合的名字或实体类的类型Comment.class
	mongoTemplate.updateFirst(query,update,"comment");
}
```
（2）测试用例：`cn.itcast.article.service.CommentServiceTest`
```java
/**
 * 点赞数+1
 */
@Test
public void testUpdateCommentLikenum(){
	//对3号文档的点赞数+1
	commentService.updateCommentLikenum("3");
}
```
执行测试用例后，发现点赞数+1了：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202303091207004.png)
