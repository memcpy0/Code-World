-   分支：[210920_xfg_IdGenerator](https://gitcode.net/KnowledgePlanet/Lottery/-/tree/210920_xfg_IdGenerator)
-   描述：使用**雪花算法**、**阿帕奇工具包** RandomStringUtils、**日期拼接**，三种方式生成ID，分别==用在订单号、策略ID、活动号的生成上==。

## 一、开发日志
-   【说明】从本章节开始，我们会陆续的引入**一些基础内容的搭建**，包括本章节关于ID的生成、以及后续章节需要引入分库分表、vo2dto方法、Redis等，这些会支撑我们继续**开发业务领域中一些需要用到的订单号、活动号生成以及个人用户参与到的抽奖信息落库**。
-   ==使用策略模式把三种生成ID的算法进行统一包装，由调用方决定使用哪种生成ID的策略==。_策略模式属于行为模式的一种，一个类的行为或算法可以在运行时进行更改_
-   雪花算法本章节使用的是工具包 hutool 包装好的工具类，一般在实际使用雪花算法时需要做一些优化处理，比如**支持时间回拨、支持手工插入、简短生成长度、提升生成速度**等。
-   而**日期拼接和随机数工具包生成方式，都需要自己保证唯一性**（生成的ID数量较少，用在自己的系统中），**一般使用此方式生成的ID，都用在单表中**，本身可以在数据库配置唯一ID。==_那为什么不用自增ID，因为自增ID通常容易被外界知晓你的运营数据，以及后续需要做数据迁移到分库分表中都会有些麻烦_==

## 二、支撑领域
在 domain 领域包下新增支撑领域，ID 的生成服务就放到这个领域下实现。关于 ID 的生成因为有三种不同 ID 用于在不同的场景下；
-   订单号：唯一、大量、订单创建时使用、分库分表
-   活动号：**唯一、少量**、活动创建时使用、**单库单表**
-   策略号：**唯一、少量**、活动创建时使用、**单库单表**

## 三、策略模式
通过策略模式的使用，来开发策略ID的服务提供。之所以使用策略模式，是因为外部的调用方会需要根据不同的场景来选择出适合的ID生成策略，而策略模式就非常适合这一场景的使用。案例：[https://mp.weixin.qq.com/s/zOFLtSFVrYEyTuihzwgKYw](https://mp.weixin.qq.com/s/zOFLtSFVrYEyTuihzwgKYw)

### 1. 工程结构
```java
lottery-domain
└── src
    └── main
        └── java
            └── cn.itedus.lottery.domain.support.ids
                ├── policy
                │   ├── RandomNumeric.java
                │   ├── ShortCode.java
                │   └── SnowFlake.java
                ├── IdContext.java
                └── IIdGenerator.java
```

-   IIdGenerator，定义生成ID的策略接口。RandomNumeric、ShortCode、SnowFlake，是三种生成ID的策略。
-   IdContext，ID生成上下文，也就是从这里提供策略配置服务。

### 2. IIdGenerator 策略接口
```java
public interface IIdGenerator {

    /**
     * 获取ID，目前有两种实现方式
     * 1. 雪花算法，用于生成单号
     * 2. 日期算法，用于生成活动编号类，特性是生成数字串较短，但指定时间内不能生成太多
     * 3. 随机算法，用于生成策略ID 11位
     *
     * @return ID
     */
    long nextId();

}
```
### 3. 策略ID实现
```java
@Component
public class SnowFlake implements IIdGenerator {
    private Snowflake snowflake;
    @PostConstruct
    public void init() {
        // 0 ~ 31 位，可以采用配置的方式使用
        long workerId;
        try {
            workerId = NetUtil.ipv4ToLong(NetUtil.getLocalhostStr());
        } catch (Exception e) {
            workerId = NetUtil.getLocalhostStr().hashCode();
        }

        workerId = workerId >> 16 & 31;

        long dataCenterId = 1L;
        snowflake = IdUtil.createSnowflake(workerId, dataCenterId);
    }

    @Override
    public synchronized long nextId() {
        return snowflake.nextId();
    }
}
```
-   使用 hutool 工具类提供的雪花算法（源码中要求的workId不能超过范围31），提供生成ID服务
-   其他方式的 ID 生成可以直接参考源码

业务使用中，需要提供一个完整的系统来生成ID，才能提供分布式的可靠性的保证，所有其他系统调用同一个系统生成的ID，而不是仅使用工具类的雪花算法，
### 4. 策略服务上下文
```java
@Configuration
public class IdContext {
    /**
     * 创建 ID 生成策略对象，属于策略设计模式的使用方式
     *
     * @param snowFlake 雪花算法，长码，大量
     * @param shortCode 日期算法，短码，少量，全局唯一需要自己保证
     * @param randomNumeric 随机算法，短码，大量，全局唯一需要自己保证
     * @return IIdGenerator 实现类
     */
    @Bean
    public Map<Constants.Ids, IIdGenerator> idGenerator(SnowFlake snowFlake, ShortCode shortCode, RandomNumeric randomNumeric) {
        Map<Constants.Ids, IIdGenerator> idGeneratorMap = new HashMap<>(8);
        idGeneratorMap.put(Constants.Ids.SnowFlake, snowFlake);
        idGeneratorMap.put(Constants.Ids.ShortCode, shortCode);
        idGeneratorMap.put(Constants.Ids.RandomNumeric, randomNumeric);
        return idGeneratorMap;
    }

}
```
-   通过配置注解 `@Configuration` 和 Bean 对象的生成 `@Bean`，来把策略生成ID服务包装到 `Map<Constants.Ids, IIdGenerator>` 对象中。

## 四、测试验证
**cn.itedus.lottery.test.domain.SupportTest**
```java
@RunWith(SpringRunner.class)
@SpringBootTest
public class SupportTest {
    private Logger logger = LoggerFactory.getLogger(SupportTest.class);
    @Resource
    private Map<Constants.Ids, IIdGenerator> idGeneratorMap;
    @Test
    public void test_ids() {
        logger.info("雪花算法策略，生成ID：{}", idGeneratorMap.get(Constants.Ids.SnowFlake).nextId());
        logger.info("日期算法策略，生成ID：{}", idGeneratorMap.get(Constants.Ids.ShortCode).nextId());
        logger.info("随机算法策略，生成ID：{}", idGeneratorMap.get(Constants.Ids.RandomNumeric).nextId());
    }
}
```
**测试结果**
```
20:19:19.364  INFO 5978 --- [           main] c.i.lottery.test.domain.SupportTest      : 雪花算法策略，生成ID：1439927148298272768
20:19:19.364  INFO 5978 --- [           main] c.i.lottery.test.domain.SupportTest      : 日期算法策略，生成ID：120392968
20:19:19.365  INFO 5978 --- [           main] c.i.lottery.test.domain.SupportTest      : 随机算法策略，生成ID：67381317002
```
-   每种ID策略获取到的ID结果都有所不同，这也是为了**适合不同类型的ID使用**，避免同样长度的ID造成混乱。_比如订单ID用在活动ID生成上，就会显得有些混乱_
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305262152837.png)

---
1.  学习策略模式的注解配置方式使用 @Configuration、@Bean
2.  了解雪花算法的使用，==通常目前并发场景中用于生成ID的算法，基本都是在雪花算法的基础上进行设计和优化==。

```java
/**  
 * 获取单例的Twitter的Snowflake 算法生成器对象<br>  
 * 分布式系统中，有一些需要使用全局唯一ID的场景，有些时候我们希望能使用一种简单一些的ID，并且希望ID能够按照时间有序生成。  
 *  
 * <p>  
 * snowflake的结构如下(每部分用-分开):<br>  
 *  
 * <pre>  
 * 0 - 0000000000 0000000000 0000000000 0000000000 0 - 00000 - 00000 - 000000000000  
 * </pre>  
 * <p>  
 * 第一位为未使用，接下来的41位为毫秒级时间(41位的长度可以使用69年)<br>  
 * 然后是5位datacenterId和5位workerId(10位的长度最多支持部署1024个节点）<br>  
 * 最后12位是毫秒内的计数（12位的计数顺序号支持每个节点每毫秒产生4096个ID序号）  
 *  
 * <p>  
 * 参考：http://www.cnblogs.com/relucent/p/4955340.html  
 * * @param workerId     终端ID  
 * @param datacenterId 数据中心ID  
 * @return {@link Snowflake}  
 * @since 4.5.9  
 */
public static Snowflake getSnowflake(long workerId, long datacenterId) {  
   return Singleton.get(Snowflake.class, workerId, datacenterId);  
}
```

ID生成器工具类，此工具类中主要封装：
  1. 唯一性ID生成器：UUID、ObjectId（MongoDB）、Snowflake
  2. ID相关文章见：http://calvin1978.blogcn.com/articles/uuid.html

分布式全局ID生成策略
## 一、需求缘起
几乎所有的业务系统，都有生成一个记录标识的需求，例如：
（1）消息标识：message-id
（2）**订单标识**：order-id
（3）帖子标识：tiezi-id

这个记录标识往往就是数据库中的唯一主键，数据库上会建立**聚集索引**（cluster index），即在物理存储上以这个字段排序。

这个记录标识上的查询，往往又有分页或者排序的业务需求，例如：
（1）拉取最新的一页消息：`selectmessage-id/ order by time/ limit 100`
（2）拉取最新的一页订单：`selectorder-id/ order by time/ limit 100`
（3）拉取最新的一页帖子：`selecttiezi-id/ order by time/ limit 100`

所以==往往要有一个time字段，并且在time字段上建立普通索引（non-cluster index）==。

我们都知道==普通索引存储的是实际记录的指针，其访问效率会比聚集索引慢，如果记录标识在生成时能够基本按照时间有序==，则可以省去这个time字段的索引查询：
select message-id/ (order by message-id)/limit 100

再次强调，能这么做的前提是，**message-id的生成基本是趋势时间递增的**。这就引出了**记录标识生成**（也就是上文提到的三个XXX-id）的两大核心需求：
（1）全局唯一
（2）趋势有序

这也是本文要讨论的核心问题：如何高效生成趋势有序的全局唯一ID。
## 二、常见方法、不足与优化
#### 【常见方法一：使用数据库的 auto_increment 来生成全局唯一递增ID】
优点：
（1）简单，使用数据库已有的功能
（2）能够保证单机唯一性
（3）能够保证递增性
（4）步长固定

缺点：
（1）**可用性难以保证**：数据库常见架构是一主多从+读写分离，生成自增ID是写请求，主库挂了就玩不转了
（2）**扩展性差**，性能有上限：==因为写入是单点，数据库主库的写性能决定**ID的生成性能上限**，并且难以扩展==

改进方法：
（1）**增加主库**，避免写入单点
（2）**数据水平切分，保证各主库生成的ID不重复**

![400](https://image.dandelioncloud.cn/images/20220531/7569fbbd8ab44159b25d4cf0f10e7699.png)

如上图所述，由 1 个写库变成 3 个写库， **每个写库设置不同的auto_increment初始值，以及相同的增长步长 ，以保证每个数据库生成的 ID 是不同的**（上图中库 0 生成 0,3,6,9… ，库 1 生成 1,4,7,10 ，库 2 生成 2,5,8,11… ）

改进后的架构保证了可用性，但缺点是：
（1）丧失了ID生成的“绝对递增性”：先访问库0生成0,3，再访问库1生成1，可能导致在非常短的时间内，ID生成不是绝对递增的（这个问题不大，我们的目标是**趋势递增，不是绝对递增**）
（2）**数据库的写压力依然很大，每次生成ID都要访问数据库**

为了解决上述两个问题，引出了第二个常见的方案
### 【常见方法二：单点批量ID生成服务】
分布式系统之所以难，很重要的原因之一是“没有一个全局时钟，难以保证绝对的时序”，**要想保证绝对的时序，还是只能使用单点服务，用本地时钟保证“绝对时序”**。数据库写压力大，是因为每次生成ID都访问了数据库，可以使用批量的方式降低数据库写压力。

![70 1](https://image.dandelioncloud.cn/images/20220531/6e766ee44de348c48816829bed790f9e.png)  
如上图所述，数据库使用双master保证可用性，数据库中只存储当前ID的最大值，例如0。ID生成服务假设每次批量拉取6个ID，服务访问数据库，将当前ID的最大值修改为5，这样应用访问ID生成服务索要ID，ID生成服务不需要每次访问数据库，就能依次派发0,1,2,3,4,5这些ID了，当ID发完后，再将ID的最大值修改为11，就能再次派发6,7,8,9,10,11这些ID了，于是数据库的压力就降低到原来的1/6了。

优点：
（1）保证了ID生成的绝对递增有序
（2）大大的降低了数据库的压力，ID生成可以做到每秒生成几万几十万个

缺点：
（1）服务仍然是单点
（2）如果服务挂了，服务重启起来之后，继续生成ID可能会不连续，中间出现空洞（服务内存是保存着0,1,2,3,4,5，数据库中max-id是5，分配到3时，服务重启了，下次会从6开始分配，4和5就成了空洞，不过这个问题也不大）
（3）虽然每秒可以生成几万几十万个ID，但毕竟还是有性能上限，无法进行水平扩展

改进方法：**单点服务的常用高可用优化方案是“备用服务”，也叫“影子服务”**，所以我们能用以下方法优化上述缺点（1）：

![70 2](https://image.dandelioncloud.cn/images/20220531/0af2c8e2fa14497297a8731f70d1e38f.png)  
如上图，对外提供的服务是主服务，有一个影子服务时刻处于备用状态，当主服务挂了的时候影子服务顶上。这个切换的过程对调用方是透明的，可以自动完成，常用的技术是vip+keepalived，具体就不在这里展开。

#### 【常见方法三：uuid】
上述方案来生成ID，==虽然性能大增，但由于是单点系统，总还是存在性能上限的==。同时，上述两种方案，**不管是数据库还是服务来生成ID，业务方Application都需要进行一次远程调用**，比较耗时。有没有一种**本地生成ID的方法**，即高性能，又时延低呢？

uuid是一种常见的方案：string ID =GenUUID();

优点：
（1）**本地生成ID，不需要进行远程调用**，时延低
（2）扩展性好，**基本可以认为没有性能上限**

缺点：
（1）**无法保证趋势递增**
（2）**uuid过长，往往用字符串表示**，**作为主键建立索引查询效率低**，==常见优化方案为“转化为两个uint64整数存储”或者“折半存储”==（折半后不能保证唯一性）
### 【常见方法四：取当前毫秒数】
uuid是一个本地算法，生成性能高，但无法保证趋势递增，且作为字符串ID检索效率低，有没有一种**能保证递增的本地算法**呢？取当前毫秒数是一种常见方案：uint64 ID = GenTimeMS();
优点：
（1）本地生成ID，不需要进行远程调用，时延低
（2）生成的ID趋势递增
（3）生成的ID是整数，建立索引后查询效率高

缺点：
（1）**如果并发量超过1000，会生成重复的ID**

我去，这个缺点要了命了，不能保证ID的唯一性。当然，使用微秒可以降低冲突概率，但每秒最多只能生成1000000个ID，再多的话就一定会冲突了，所以使用微秒并不从根本上解决问题。

### 【常见方法五：类snowflake算法】
**snowflake是twitter开源的分布式ID生成算法**，其核心思想是：==一个long型的ID，使用其中41bit作为毫秒数，10bit作为机器编号，12bit作为毫秒内序列号==。这个算法单机每秒内理论上最多可以生成1000*(2^12)，也就是400W的ID，完全能满足业务的需求。

==借鉴snowflake的思想，结合各公司的业务逻辑和并发量，可以实现自己的分布式ID生成算法==。

举例，假设某公司ID生成器服务的需求如下：
（1）单机高峰并发量小于1W，预计未来5年单机高峰并发量小于10W
（2）有2个机房，预计未来5年机房数量小于4个
（3）每个机房机器数小于100台
（4）目前有5个业务线有ID生成需求，预计未来业务线数量小于10个
（5）…

分析过程如下：
（1）高位取从2016年1月1日到现在的毫秒数（假设系统ID生成器服务在这个时间之后上线），假设系统至少运行10年，那至少需要10年*365天*24小时*3600秒*1000毫秒=320*10^9，差不多预留39bit给毫秒数
（2）每秒的单机高峰并发量小于10W，即平均每毫秒的单机高峰并发量小于100，差不多预留7bit给每毫秒内序列号
（3）5年内机房数小于4个，预留2bit给机房标识
（4）每个机房小于100台机器，预留7bit给每个机房内的服务器标识
（5）业务线小于10个，预留4bit给业务线标识

![70 3](https://image.dandelioncloud.cn/images/20220531/e4e55b0ef7ea488c8c9133f5a74c07b5.png)  
这样设计的64bit标识，可以保证：
（1）每个业务线、每个机房、每个机器生成的ID都是不同的
（2）同一个机器，每个毫秒内生成的ID都是不同的
（3）同一个机器，同一个毫秒内，以序列号区区分保证生成的ID是不同的
（4）**将毫秒数放在最高位，保证生成的ID是趋势递增的**

缺点：
（1）由于“没有一个全局时钟”，==每台服务器分配的ID是绝对递增的，但从全局看，生成的ID只是趋势递增的==（有些服务器的时间早，有些服务器的时间晚）

最后一个容易忽略的问题：**生成的ID**，例如message-id/ order-id/ tiezi-id，**在数据量大时往往需要分库分表，这些ID经常作为取模分库分表的依据**，为了分库分表后数据均匀，ID生成往往有“取模随机性”的需求，所以**我们通常把每秒内的序列号放在ID的最末位，保证生成的ID是随机的**。

又如果，我们在跨毫秒时，序列号总是归0，会使得序列号为0的ID比较多，导致生成的ID取模后不均匀。解决方法是，序列号不是每次都归0，而是归一个0到9的随机数，这个地方。
## 三. 成熟产品
1.  小米开源了一个号称 高可用、高性能、提供全局唯一而且严格单调递增timestamp 服务的服务https://github.com/XiaoMi/chronos

## 四. 应用
1.  系统幂等
如：下单，支付—订单编号，支付编号
1.  分布式调用链的TraceId

另一个思考：**分布式的Unique ID的用途如此广泛**，从业务对象Id到服务化体系里分布式调用链的TraceId，本文总结了林林总总的各种生成算法。

对于UID的要求，一乐那篇《业务系统需要什么样的ID生成器》的提法很好： ==唯一性，时间相关，粗略有序，可反解==，可制造。

### 1. 发号器
我接触的最早的Unique ID，就是Oracle的Sequence。

特点是准连续的自增数字，为什么说是准连续？因为性能考虑，每个Client一次会领20个ID回去慢慢用，用完了再来拿。另一个Client过来，拿的就是另外20个ID了。

新浪微博里，Tim用Redis做相同的事情，Incr一下拿一批ID回去。如果有多个数据中心，那就拿高位的几个bit来区分。

只要舍得在总架构里增加额外Redis带来的复杂度，一个64bit的long就够表达了，而且不可能有重复ID。

批量是关键，否则每个ID都远程调用一次谁也吃不消。

### 2. UUID
#### 2.1 概述
Universally Unique IDentifier(UUID)，有着正儿八经的RFC规范，是一个128bit的数字，也可以表现为32个16进制的字符（每个字符0-F的字符代表4bit），中间用”-“分割。
-   时间戳＋UUID版本号： 分三段占16个字符(60bit+4bit)，
-   Clock Sequence号与保留字段：占4个字符(13bit＋3bit)，
-   节点标识：占12个字符(48bit)，

比如：
> f81d4fae-7dec-11d0-a765-00a0c91e6bf6

实际上，**UUID一共有多种算法，能用于TraceId的是**：
-   version1: 基于时间的算法
-   version4: 基于随机数的算法
#### 2.2 version 4 基于随机数的算法
先说Version4，==这是最暴力的做法，也是JDK里的算法，不管原来各个位的含义了，除了少数几个位必须按规范填，其余全部用随机数表达==。

JDK里的实现，用 SecureRandom生成了16个随机的Byte，用2个long来存储。记得加 `-Djava.security.egd=file:/dev/./urandom` ， 详见SecureRandom的江湖偏方与真实效果
#### 2.3 version 1 基于时间的算法
然后是Version1，严格守着原来各个位的规矩：
- 时间戳：有满满的60bit，所以可以尽情花，以100纳秒为1，从1582年10月15日算起(能撑3655年，真是位数多给烧的，1582年起头有意思么)
- 顺序号： 这16bit则仅用于避免前面的节点标示改变（如网卡改了），时钟系统出问题（如重启后时钟快了慢了），让它随机一下避免重复。
- 节点标识：48bit，一般用MAC地址表达，如果有多块网卡就随便用一块。如果没网卡，就用随机数凑数，或者拿一堆尽量多的其他的信息，比如主机名什么的，拼在一起再hash一把。

但这里的顺序号并不是我们想象的自增顺序号，而是一个一次性的随机数，所以如果两个请求在同100个纳秒发生时。。。。。

还有这里的节点标识只在机器级别，没考虑过一台机器上起了两个进程。

所以严格的Version1没人实现，接着往下看各个变种吧。
#### 2.4 version 1 vs version4
version4随机数的做法简单直接，但以唯一性，时间相关，粗略有序，可反解，可制造做要求的话，则不如version4，比如==唯一性，因为只是两个随机long，并没有从理论上保证不会重复==。

### 3. Version1变种 - Hibernate
Hibernate的CustomVersionOneStrategy.java，解决了之前version 1的两个问题
-   时间戳(6bytes, 48bit)：毫秒级别的，从1970年算起，能撑8925年….
-   顺序号(2bytes, 16bit, 最大值65535): 没有时间戳过了一毫秒要归零的事，各搞各的，short溢出到了负数就归0。
-   机器标识(4bytes 32bit): 拿localHost的IP地址，IPV4呢正好4个byte，但如果是IPV6要16个bytes，就只拿前4个byte。
-   进程标识(4bytes 32bit)： 用当前时间戳右移8位再取整数应付，不信两条线程会同时启动。

顺序号也不再是一次性的随机数而是自增序列了。

节点标识拆成机器和进程两个字段，增加了从时间戳那边改变精度节约下来的16bit。另外节点标识这64bit（1个Long）总是不变，节约了生成UID的时间。

### 4. Version1变种 - MongoDB
MongoDB的ObjectId.java
-   时间戳(4 bytes 32bit)：是秒级别的，从1970年算起，能撑136年。
-   自增序列(3bytes 24bit, 最大值一千六百万)： 是一个从随机数开始（机智）的Int不断加一，也没有时间戳过了一秒要归零的事，各搞各的。因为只有3bytes，所以一个4bytes的Int还要截一下后3bytes。
-   机器标识(3bytes 24bit)： 将所有网卡的Mac地址拼在一起做个HashCode，同样一个int还要截一下后3bytes。搞不到网卡就用随机数混过去。
-   进程标识(2bytes 16bits)：从JMX里搞回来到进程号，搞不到就用进程名的hash或者随机数混过去。

可见，MongoDB的每一个字段设计都比Hibernate的更合理一点，时间戳是秒级别的，自增序列变长了，进程标识变短了。总长度也降到了12 bytes 96bit。

但如果果用64bit长的Long来保存有点不上不下的，只能表达成byte数组或16进制字符串。
### 5. Twitter的snowflake派号器
snowflake也是一个派号器，基于Thrift的服务，不过不是用redis简单自增，而是类似UUID version1，只有一个Long 64bit的长度，所以IdWorker紧巴巴的分配成：
-   时间戳(42bit) ：自从2012年以来(比那些从1970年算起的会过日子)的毫秒数，能撑139年。
-   自增序列(12bit，最大值4096)：毫秒之内的自增，过了一毫秒会重新置0。
-   DataCenter ID (5 bit, 最大值32）：配置值，支持多机房。
-   Worker ID ( 5 bit, 最大值32)，配置值，因为是派号器的id，一个机房里最多32个派号器就够了，还会在ZK里做下注册。

可见，因为是中央派号器，把至少40bit的节点标识都省出来了，换成10bit的派号器标识。所以整个UID能够只用一个Long表达。

另外，这种派号器，client每次只能一个ID，不能批量取，所以额外增加的延时是问题。
### 6. 扩展阅读
《细聊分布式ID生成方法》
《生成全局唯一ID的3个思路，来自一个资深架构师的总结》
### 7. 扩展问题，能不能不用派号器，又一个Long搞定UUID??
这是我们服务化框架一开始的选择，TraceID设为了一个Long，而不是String，又不想用派号器的话，怎么办？

从UUID的128位压缩到Long的64位，又不用中央派号器而是本地生成，最难还是怎么来区分本地的机器＋进程号。
#### 思路一，压缩其他字段，留足够多的长度来做机器＋进程号标识
时间戳是秒级别，1年要24位，两年要25位…..  
自增序列，6万QPS要16位，10万要17位…  
剩下20－24位，百万分之一到一千六百万分之一的重复率，然后把网卡Mac＋进程号拼在一起再hash，取结果32个bit的后面20或24个bit。但假如这个标识字段重复了，后面时间戳和自增序列也很容易重复，不停的重复。
#### 思路二，使用ZK 或 mysql 或 redis来自增管理标识号
如果workder字段只留了12位（4096），就要用ZK或etcd，当进程关闭了要回收这个号。  
如果workder字段的位数留得够多，比如有20位（一百万），那用redis或mysql来自增最简单，每个进程启动时拿一个worker id。
#### 思路三，继续Random
继续拼了，因为traceId的唯一性要求不高，偶然重复也没大问题，所以直接拿JDK UUID.randomUUID()的低位long（按UUID规范，高位的long被置了4个默认值的bit，低位只被设置3个bit），或者不用UUID.randomUUID()了，直接SecureRandom.nextLong()，不浪费了那3个bit。


**文章之3：**

不仅仅局限于数据库中的ID主键生产，也可以适用于其他分布式环境中的唯一标示，比如全局唯一事务ID，日志追踪时的唯一标示等。

> 先列出笔者最喜欢的一种全局唯一ID的生成方式，注意：没有完美的方案，只有适合自己的方案，还请读者根据具体的业务进行取舍，而且可以放到客户端进行ID 的生成，没有单点故障，性能也有一定保证，而且不需要独立的服务器。

# 全数字全局唯一标识（来自于mongodb）

其实现在有很多种生成策略，也各有优缺点，使用场景不同。这里说的是一种全数字的全局唯一ID，为什么我比较喜欢呢，首先它是全数字，保存和计算都比较简单(想一下MySQL数据库中对数字和字符串的处理效率)，而且从这个ID中可以得到一些额外的信息，不想一些UUID、sha等字符串对我们几乎没有太大帮助。好了下面就说一下具体实现过程。

> 本算法来自于mongodb

ObjectId使用12字节的存储空间，每个字节存两位16进制数字，是一个24位的字符串。其生成方式如下：

12位生成规则：  
[0,1,2,3] [4,5,6] [7,8] [9,10,11]  
时间戳 |机器码 |PID |计数器

1.  前四个字节时间戳是从标准纪元开始的时间戳，单位为秒，有如下特性：
    
    -   时间戳与后边5个字节一块，保证秒级别的唯一性；
    -   保证插入顺序大致按时间排序；
    -   隐含了文档创建时间；
    -   时间戳的实际值并不重要，不需要对服务器之间的时间进行同步（因为加上机器ID和进程ID已保证此值唯一，唯一性是ObjectId的最终诉求）。

> 上面牵扯到两个分布式系统中的概念：分布式系统中全局时钟同步很难，基本不可能实现，也没必要；时序一致性（顺序性）无法保证。这不属于本文范畴，感兴趣读者请自行搜索。

1.  机器ID是服务器主机标识，通常是机器主机名的hash散列值。
2.  同一台机器上可以运行多个mongod实例，因此也需要加入进程标识符PID。
3.  前9个字节保证了同一秒钟不同机器不同进程产生的ObjectId的唯一性。后三个字节是一个自动增加的计数器（一个mongod进程需要一个全局的计数器），保证同一秒的ObjectId是唯一的。同一秒钟最多允许每个进程拥有（256^3 = 16777216）个不同的ObjectId。

总结一下：时间戳保证秒级唯一，机器ID保证设计时考虑分布式，避免时钟同步，PID保证同一台服务器运行多个mongod实例时的唯一性，最后的计数器保证同一秒内的唯一性（选用几个字节既要考虑存储的经济性，也要考虑并发性能的上限）。

## 改为全数字

上面mongodb中保存的是16进制，如果不想用16进制的话，可以修改为10进制保存，只不过占用空间会大一些。

后面的计数器留几位，具体就看你们的业务量了，设计的时候要预留出以后的业务增长量。单进程内的计数器可以使用atomicInteger。

具体代码请参考我写的另一篇文章Twitter的分布式自增ID算法snowflake（有改动Java版）http://blog.csdn.net/liubenlong007/article/details/74354713

# UUID
1.  `UUID生成的是length=32的16进制格式的字符串，如果回退为byte数组共16个byte元素，即UUID是一个128bit长的数字，`
2.    `一般用16进制表示。`
3.    `算法的核心思想是结合机器的网卡、当地时间、一个随即数来生成UUID。`
4.    `从理论上讲，如果一台机器每秒产生10000000个GUID，则可以保证（概率意义上）3240年不重复`
5.    `优点：`
6.    `（1）本地生成ID，不需要进行远程调用，时延低`
7.    `（2）扩展性好，基本可以认为没有性能上限`
8.    `缺点：`
9.    `（1）无法保证趋势递增`
10.    `（2）uuid过长，往往用字符串表示，作为主键建立索引查询效率低，常见优化方案为“转化为两个uint64整数存储”或者“折半存储”（折半后不能保证唯一性）`

> 注：以下这几种需要独立的服务器

# 来自Flicker的解决方案（依赖数据库）

因为MySQL本身支持auto_increment操作，很自然地，我们会想到借助这个特性来实现这个功能。  
Flicker在解决全局ID生成方案里就采用了MySQL自增长ID的机制（auto_increment + replace into + MyISAM）。一个生成64位ID方案具体就是这样的：  
先创建单独的数据库(eg:ticket)，然后创建一个表：

1.  `CREATE TABLE Tickets64 ( id bigint(20) unsigned NOT NULL auto_increment, stub char(1) NOT NULL default '', PRIMARY KEY (id), UNIQUE KEY stub (stub) ) ENGINE=MyISAM`
    
    当我们插入记录后，执行SELECT * from Tickets64，查询结果就是这样的：
    
    +—————————-+———+  
    | id | stub |  
    +—————————-+———+  
    | 72157623227190423 | a |  
    +—————————-+———+  
    在我们的应用端需要做下面这两个操作，在一个事务会话里提交：
    REPLACEINTOTickets64 (stub)VALUES(‘a’);  
    SELECTLAST_INSERT_ID();
    
 
    
    这样我们就能拿到不断增长且不重复的ID了。  
    到上面为止，我们只是在单台数据库上生成ID，从高可用角度考虑，接下来就要解决单点故障问题：Flicker启用了两台数据库服务器来生成ID，通过区分auto_increment的起始值和步长来生成奇偶数的ID。
    
    TicketServer1:  
    auto-increment-increment = 2  
    auto-increment-offset = 1
    
    TicketServer2:  
    auto-increment-increment = 2  
    auto-increment-offset = 2
 
    
    最后，在客户端只需要通过轮询方式取ID就可以了。
    
    优点：充分借助数据库的自增ID机制，提供高可靠性，生成的ID有序。  
    缺点：占用两个独立的MySQL实例，有些浪费资源，成本较高。在服务器变更的时候要修改步长，比较麻烦。
    
# 基于redis的分布式ID生成器
首先，要知道redis的EVAL，EVALSHA命令：  

### 原理  
利用redis的lua脚本执行功能，在每个节点上通过lua脚本生成唯一ID。  生成的ID是64位的：
-   使用41 bit来存放时间，精确到毫秒，可以使用41年。
-   使用12 bit来存放逻辑分片ID，最大分片ID是4095
-   使用10 bit来存放自增长ID，意味着每个节点，每毫秒最多可以生成1024个ID  
    比如GTM时间 Fri Mar 13 10:00:00 CST 2015 ，它的距1970年的毫秒数是 1426212000000，假定分片ID是53，自增长序列是4，则生成的ID是：
    
    5981966696448054276 = 1426212000000 << 22 + 53 << 10 + 41  
    redis提供了TIME命令，可以取得redis服务器上的秒数和微秒数。因些lua脚本返回的是一个四元组。
    
    second, microSecond, partition, seq  
    客户端要自己处理，生成最终ID。
    
    ((second * 1000 + microSecond / 1000) << (12 + 10)) + (shardId << 10) + seq;
    

## **参考**：
1.  沈剑架构师之路-《分布式ID生成器》
2.  江南白衣-《服务化框架－分布式Unique ID的生成方法一览》：http://calvin1978.blogcn.com/articles/uuid.html
3.  《高并发分布式环境中获取全局唯一ID[分布式数据库全局唯一主键生成]》：http://blog.csdn.net/liubenlong007/article/details/53884447