### 1. 任务执行和调度
JDK线程池：
- ExecutorService
- ScheduledExecutorService：执行间隔的任务

Spring线程池：
- ThreadPoolTaskExecutor
- ThreadPoolTaskScheduler

分布式定时任务：Spring Quartz http://www.quartz-scheduler.org

普通的定时任务的线程池，在分布式系统中会有问题：假设有两台服务器，服务器里面有两类程序，分别是普通的程序（controller）解决普通的请求，还有Scheduler解决定时任务。分布式环境下，**浏览器不会直接访问服务器，服务器前面通常还有Nginx来做负载均衡**，Nginx根据某些策略分发请求给某台服务器上的Controller执行。对于一个请求来说，只有一个服务器上的一个程序来处理，对于普通的请求来说没有影响。

但对于定时任务来说，服务器启动就会开始执行，两台服务器上的两个相同定时任务会在启动时执行，就会重复了。比如每隔10分钟清理临时文件，就可能出现冲突。JDK的SchedulerExecutorService和Spring的ThreadPoolTaskScheduler都有这种问题。

JDK和Spring的定时器组件都基于内存，配置参数都在内存中，服务器内存不共享，不知道彼此干了啥。Quartz定时任务的参数存在数据库中，而我们的数据库只有一个，就可以共享。不过，访问时需要加锁，看到别的服务器上的定时任务正在运行，就等待不执行。

---
导入包：
```xml
<dependency>  
   <groupId>org.springframework.boot</groupId>  
   <artifactId>spring-boot-starter-quartz</artifactId>  
</dependency>
```
Quartz有几个核心组件：
- Scheduler：所有任务都是通过它调度的。
- Job：通过这个接口定义任务，里面只有一个execute方法。
- JobDetail：用于配置Job的详情。
- Trigger：用于定时、以什么频率来运行。

Quartz需要提前创建表，先将配置数据初始化到表中，然后读取表来执行任务。看下：
- QRTZ_JOB_DETAILS：JOB的相关信息
- QRTZ_SIMPLE_TRIGGERS，QRTZ_TRIGGERS：执行时间频率等
- QRTZ_SCHEDULER_STATE：执行状态等
- QRTZ_LOCKS：多个quartz访问表时，以其中的LOCK_NAME来加锁。
```sql
#
# In your Quartz properties file, you'll need to set
# org.quartz.jobStore.driverDelegateClass = org.quartz.impl.jdbcjobstore.StdJDBCDelegate
#
#
# By: Ron Cordell - roncordell
#  I didn't see this anywhere, so I thought I'd post it here. This is the script from Quartz to create the tables in a MySQL database, modified to use INNODB instead of MYISAM.

DROP TABLE IF EXISTS QRTZ_FIRED_TRIGGERS;
DROP TABLE IF EXISTS QRTZ_PAUSED_TRIGGER_GRPS;
DROP TABLE IF EXISTS QRTZ_SCHEDULER_STATE;
DROP TABLE IF EXISTS QRTZ_LOCKS;
DROP TABLE IF EXISTS QRTZ_SIMPLE_TRIGGERS;
DROP TABLE IF EXISTS QRTZ_SIMPROP_TRIGGERS;
DROP TABLE IF EXISTS QRTZ_CRON_TRIGGERS;
DROP TABLE IF EXISTS QRTZ_BLOB_TRIGGERS;
DROP TABLE IF EXISTS QRTZ_TRIGGERS;
DROP TABLE IF EXISTS QRTZ_JOB_DETAILS;
DROP TABLE IF EXISTS QRTZ_CALENDARS;

CREATE TABLE QRTZ_JOB_DETAILS(
SCHED_NAME VARCHAR(120) NOT NULL,
JOB_NAME VARCHAR(190) NOT NULL,
JOB_GROUP VARCHAR(190) NOT NULL,
DESCRIPTION VARCHAR(250) NULL,
JOB_CLASS_NAME VARCHAR(250) NOT NULL,
IS_DURABLE VARCHAR(1) NOT NULL,
IS_NONCONCURRENT VARCHAR(1) NOT NULL,
IS_UPDATE_DATA VARCHAR(1) NOT NULL,
REQUESTS_RECOVERY VARCHAR(1) NOT NULL,
JOB_DATA BLOB NULL,
PRIMARY KEY (SCHED_NAME,JOB_NAME,JOB_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_TRIGGERS (
SCHED_NAME VARCHAR(120) NOT NULL,
TRIGGER_NAME VARCHAR(190) NOT NULL,
TRIGGER_GROUP VARCHAR(190) NOT NULL,
JOB_NAME VARCHAR(190) NOT NULL,
JOB_GROUP VARCHAR(190) NOT NULL,
DESCRIPTION VARCHAR(250) NULL,
NEXT_FIRE_TIME BIGINT(13) NULL,
PREV_FIRE_TIME BIGINT(13) NULL,
PRIORITY INTEGER NULL,
TRIGGER_STATE VARCHAR(16) NOT NULL,
TRIGGER_TYPE VARCHAR(8) NOT NULL,
START_TIME BIGINT(13) NOT NULL,
END_TIME BIGINT(13) NULL,
CALENDAR_NAME VARCHAR(190) NULL,
MISFIRE_INSTR SMALLINT(2) NULL,
JOB_DATA BLOB NULL,
PRIMARY KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP),
FOREIGN KEY (SCHED_NAME,JOB_NAME,JOB_GROUP)
REFERENCES QRTZ_JOB_DETAILS(SCHED_NAME,JOB_NAME,JOB_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_SIMPLE_TRIGGERS (
SCHED_NAME VARCHAR(120) NOT NULL,
TRIGGER_NAME VARCHAR(190) NOT NULL,
TRIGGER_GROUP VARCHAR(190) NOT NULL,
REPEAT_COUNT BIGINT(7) NOT NULL,
REPEAT_INTERVAL BIGINT(12) NOT NULL,
TIMES_TRIGGERED BIGINT(10) NOT NULL,
PRIMARY KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP),
FOREIGN KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP)
REFERENCES QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_CRON_TRIGGERS (
SCHED_NAME VARCHAR(120) NOT NULL,
TRIGGER_NAME VARCHAR(190) NOT NULL,
TRIGGER_GROUP VARCHAR(190) NOT NULL,
CRON_EXPRESSION VARCHAR(120) NOT NULL,
TIME_ZONE_ID VARCHAR(80),
PRIMARY KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP),
FOREIGN KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP)
REFERENCES QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_SIMPROP_TRIGGERS
  (
    SCHED_NAME VARCHAR(120) NOT NULL,
    TRIGGER_NAME VARCHAR(190) NOT NULL,
    TRIGGER_GROUP VARCHAR(190) NOT NULL,
    STR_PROP_1 VARCHAR(512) NULL,
    STR_PROP_2 VARCHAR(512) NULL,
    STR_PROP_3 VARCHAR(512) NULL,
    INT_PROP_1 INT NULL,
    INT_PROP_2 INT NULL,
    LONG_PROP_1 BIGINT NULL,
    LONG_PROP_2 BIGINT NULL,
    DEC_PROP_1 NUMERIC(13,4) NULL,
    DEC_PROP_2 NUMERIC(13,4) NULL,
    BOOL_PROP_1 VARCHAR(1) NULL,
    BOOL_PROP_2 VARCHAR(1) NULL,
    PRIMARY KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP),
    FOREIGN KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP)
    REFERENCES QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_BLOB_TRIGGERS (
SCHED_NAME VARCHAR(120) NOT NULL,
TRIGGER_NAME VARCHAR(190) NOT NULL,
TRIGGER_GROUP VARCHAR(190) NOT NULL,
BLOB_DATA BLOB NULL,
PRIMARY KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP),
INDEX (SCHED_NAME,TRIGGER_NAME, TRIGGER_GROUP),
FOREIGN KEY (SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP)
REFERENCES QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_CALENDARS (
SCHED_NAME VARCHAR(120) NOT NULL,
CALENDAR_NAME VARCHAR(190) NOT NULL,
CALENDAR BLOB NOT NULL,
PRIMARY KEY (SCHED_NAME,CALENDAR_NAME))
ENGINE=InnoDB;

CREATE TABLE QRTZ_PAUSED_TRIGGER_GRPS (
SCHED_NAME VARCHAR(120) NOT NULL,
TRIGGER_GROUP VARCHAR(190) NOT NULL,
PRIMARY KEY (SCHED_NAME,TRIGGER_GROUP))
ENGINE=InnoDB;

CREATE TABLE QRTZ_FIRED_TRIGGERS (
SCHED_NAME VARCHAR(120) NOT NULL,
ENTRY_ID VARCHAR(95) NOT NULL,
TRIGGER_NAME VARCHAR(190) NOT NULL,
TRIGGER_GROUP VARCHAR(190) NOT NULL,
INSTANCE_NAME VARCHAR(190) NOT NULL,
FIRED_TIME BIGINT(13) NOT NULL,
SCHED_TIME BIGINT(13) NOT NULL,
PRIORITY INTEGER NOT NULL,
STATE VARCHAR(16) NOT NULL,
JOB_NAME VARCHAR(190) NULL,
JOB_GROUP VARCHAR(190) NULL,
IS_NONCONCURRENT VARCHAR(1) NULL,
REQUESTS_RECOVERY VARCHAR(1) NULL,
PRIMARY KEY (SCHED_NAME,ENTRY_ID))
ENGINE=InnoDB;

CREATE TABLE QRTZ_SCHEDULER_STATE (
SCHED_NAME VARCHAR(120) NOT NULL,
INSTANCE_NAME VARCHAR(190) NOT NULL,
LAST_CHECKIN_TIME BIGINT(13) NOT NULL,
CHECKIN_INTERVAL BIGINT(13) NOT NULL,
PRIMARY KEY (SCHED_NAME,INSTANCE_NAME))
ENGINE=InnoDB;

CREATE TABLE QRTZ_LOCKS (
SCHED_NAME VARCHAR(120) NOT NULL,
LOCK_NAME VARCHAR(40) NOT NULL,
PRIMARY KEY (SCHED_NAME,LOCK_NAME))
ENGINE=InnoDB;

CREATE INDEX IDX_QRTZ_J_REQ_RECOVERY ON QRTZ_JOB_DETAILS(SCHED_NAME,REQUESTS_RECOVERY);
CREATE INDEX IDX_QRTZ_J_GRP ON QRTZ_JOB_DETAILS(SCHED_NAME,JOB_GROUP);

CREATE INDEX IDX_QRTZ_T_J ON QRTZ_TRIGGERS(SCHED_NAME,JOB_NAME,JOB_GROUP);
CREATE INDEX IDX_QRTZ_T_JG ON QRTZ_TRIGGERS(SCHED_NAME,JOB_GROUP);
CREATE INDEX IDX_QRTZ_T_C ON QRTZ_TRIGGERS(SCHED_NAME,CALENDAR_NAME);
CREATE INDEX IDX_QRTZ_T_G ON QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_GROUP);
CREATE INDEX IDX_QRTZ_T_STATE ON QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_STATE);
CREATE INDEX IDX_QRTZ_T_N_STATE ON QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP,TRIGGER_STATE);
CREATE INDEX IDX_QRTZ_T_N_G_STATE ON QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_GROUP,TRIGGER_STATE);
CREATE INDEX IDX_QRTZ_T_NEXT_FIRE_TIME ON QRTZ_TRIGGERS(SCHED_NAME,NEXT_FIRE_TIME);
CREATE INDEX IDX_QRTZ_T_NFT_ST ON QRTZ_TRIGGERS(SCHED_NAME,TRIGGER_STATE,NEXT_FIRE_TIME);
CREATE INDEX IDX_QRTZ_T_NFT_MISFIRE ON QRTZ_TRIGGERS(SCHED_NAME,MISFIRE_INSTR,NEXT_FIRE_TIME);
CREATE INDEX IDX_QRTZ_T_NFT_ST_MISFIRE ON QRTZ_TRIGGERS(SCHED_NAME,MISFIRE_INSTR,NEXT_FIRE_TIME,TRIGGER_STATE);
CREATE INDEX IDX_QRTZ_T_NFT_ST_MISFIRE_GRP ON QRTZ_TRIGGERS(SCHED_NAME,MISFIRE_INSTR,NEXT_FIRE_TIME,TRIGGER_GROUP,TRIGGER_STATE);

CREATE INDEX IDX_QRTZ_FT_TRIG_INST_NAME ON QRTZ_FIRED_TRIGGERS(SCHED_NAME,INSTANCE_NAME);
CREATE INDEX IDX_QRTZ_FT_INST_JOB_REQ_RCVRY ON QRTZ_FIRED_TRIGGERS(SCHED_NAME,INSTANCE_NAME,REQUESTS_RECOVERY);
CREATE INDEX IDX_QRTZ_FT_J_G ON QRTZ_FIRED_TRIGGERS(SCHED_NAME,JOB_NAME,JOB_GROUP);
CREATE INDEX IDX_QRTZ_FT_JG ON QRTZ_FIRED_TRIGGERS(SCHED_NAME,JOB_GROUP);
CREATE INDEX IDX_QRTZ_FT_T_G ON QRTZ_FIRED_TRIGGERS(SCHED_NAME,TRIGGER_NAME,TRIGGER_GROUP);
CREATE INDEX IDX_QRTZ_FT_TG ON QRTZ_FIRED_TRIGGERS(SCHED_NAME,TRIGGER_GROUP);

commit;
```
### 2. Job、JobDetail、Trigger
```java
package com.nowcoder.community.quartz;  
  
import org.quartz.Job;  
import org.quartz.JobExecutionContext;  
import org.quartz.JobExecutionException;  
  
public class AlphaJob implements Job {  
    @Override  
    public void execute(JobExecutionContext jobExecutionContext) throws JobExecutionException {  
        System.out.println(Thread.currentThread().getName() + ": execute a quartz job."); // 当前线程的信息  
    }  
}
```
```java
package com.nowcoder.community.config;  
  
import com.nowcoder.community.quartz.AlphaJob;  
import org.quartz.JobDataMap;  
import org.quartz.JobDetail;  
import org.springframework.context.annotation.Bean;  
import org.springframework.context.annotation.Configuration;  
import org.springframework.scheduling.quartz.JobDetailFactoryBean;  
import org.springframework.scheduling.quartz.SimpleTriggerFactoryBean;  
  
// 配置 -> 第一次加载时写到数据库中 -> 之后调用时从数据库查询  
@Configuration  
public class QuartzConfig {  
    // 实现FactoryBean接口,目的在于简化Bean的实例化过程  
    // 1. 通过FactoryBean封装了Bean的实例化过程  
    // 2. 将FactoryBean装配到Spring容器中  
    // 3. 通过FactoryBean注入给其他Bean  
    // 4. 该Bean得到的是FactoryBean管理的对象实例  
    @Bean  
    public JobDetailFactoryBean alphaJobDetail() { // 配JobDetail  
        // JobDetailFactoryBean底层封装了JobDetail详细实例化的过程  
        JobDetailFactoryBean factoryBean = new JobDetailFactoryBean();  
        factoryBean.setJobClass(AlphaJob.class);  
        factoryBean.setName("alphaJob");  
        factoryBean.setGroup("alphaJobGroup"); // 多个任务可以同属于一组  
        factoryBean.setDurability(true); // 任务是否持久保存，不用删除  
        factoryBean.setRequestsRecovery(true); // 任务是否可恢复，应用恢复后是否回复任务  
        return factoryBean;  
    }  
    // 配置Trigger: SimpleTriggerFactoryBean, CronTriggerFactoryBean  
    @Bean  
    public SimpleTriggerFactoryBean alphaTrigger(JobDetail alphaJobDetail) { // 配置Trigger,依赖于JobDetail  
        SimpleTriggerFactoryBean factoryBean = new SimpleTriggerFactoryBean();  
        factoryBean.setJobDetail(alphaJobDetail);  
        factoryBean.setName("alphaTrigger");  
        factoryBean.setGroup("alphaTriggerGroup");  
        factoryBean.setRepeatInterval(3000); // 频率，多次时间执行任务  
        factoryBean.setJobDataMap(new JobDataMap()); // 存Job的状态  
        return factoryBean;  
    }  
}
```
这时启动时，我没有配置Quartz对应的数据表，但还是能运行，因为读的是内存中的配置数据：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304171837163.png)
Quartz的底层也是用线程池实现的，可以进行配置。进行YML配置后：==一旦服务被启动，QuartzConfig配置类就被加载为Bean，然后Quartz根据这两项配置向数据库写入数据==（**要在配置中要求**，不然是根据内存中的数据来调度！），有了数据Quartz底层的Scheduler根据数据来调度任务。
```yml
spring:
	quartz:  
  job-store-type: jdbc # 底层用的JDBC存储  
  scheduler-name: CommunityScheduler # 调度器名字  
  wait-for-jobs-to-complete-on-shutdown: false # 默认为false,用于设定是否等待任务执行完毕后容器才会关闭  
  overwrite-existing-jobs: false # 默认为false,配置的JOB是否覆盖已经存在的JOB信息  
  properties:  
    org:  
      quartz:  
        scheduler: # 集群配置  
          # 集群名，区分同一系统的不同实例，若使用集群功能，则每一个实例都要使用相同的名字  
          instanceName: COMMUNITY-SCHEDULER-INSTANCE  
          # 若是集群下，每个instanceId必须唯一  
          instanceId: AUTO # 自动生成  
        jobStore: #选择JDBC的存储方式  
          class: org.quartz.impl.jdbcjobstore.JobStoreTX  
          driverDelegateClass: org.quartz.impl.jdbcjobstore.PostgreSQLDelegate  
          tablePrefix: QRTZ_  
          useProperties: false  
          isClustered: true  
          clusterCheckinInterval: 15000  
        threadPool:  
          class: org.quartz.simpl.SimpleThreadPool #一般使用这个便可  
          threadCount: 5  
          threadPriority: 5 #线程数量，不会动态增加  
          threadsInheritContextClassLoaderOfInitializingThread: true
```

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304171858199.png)
表中出现了数据：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202304171859266.png)

我把QuartzConfig类都注释掉，再启动服务，发现还是可以运行任务。不让他运行任务，可以删除相关表，或者编程控制。
```java
package com.nowcoder.community;  
  
import org.junit.jupiter.api.Test;  
import org.quartz.JobKey;  
import org.quartz.Scheduler;  
import org.springframework.beans.factory.annotation.Autowired;  
import org.springframework.boot.test.context.SpringBootTest;  
import org.springframework.test.context.ContextConfiguration;  
  
@SpringBootTest  
@ContextConfiguration(classes = CommunityApplication.class)  
public class QuartzTests {  
    @Autowired  
    private Scheduler scheduler;  
    @Test  
    public void testDeleteJob() {  
        try {  
            boolean ans = scheduler.deleteJob(new JobKey("alphaJob", "alphaJobGroup"));  
            System.out.println(ans);  
        } catch (Exception ex) {  
            ex.printStackTrace();  
        }  
    }  
}
```