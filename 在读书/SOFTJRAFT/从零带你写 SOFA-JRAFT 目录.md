

1 手写SOFA-JRAFT：单主复制在raft共识算法中的实现(一)

2 手写SOFA-JRAFT：单主复制在raft共识算法中的实现(二)

3 手写SOFA-JRAFT：单主复制在raft共识算法中的实现(三)

4 手写SOFA-JRAFT：分析节点启动过程，初步实现 NodeImpl(一)

5 手写SOFA-JRAFT：分析 NodeImpl 初始化流程，实现节点预投票功能(二)

6 手写SOFA-JRAFT：分析 NodeImpl 初始化流程，实现节点预投票功能(三)

7 手写SOFA-JRAFT：分析选举超时定时器，重构节点选举流程，实现领导者选举(四)

8 手写SOFA-JRAFT：引入 Replicator 复制器，完善领导者选举(五)

9 手写SOFA-JRAFT：重构 Replicator，实现心跳检测，分析节点内部线程的职责(六)

10 手写SOFA-JRAFT：引入 LogManager 日志管理体系，实现日志组件初始化(一)

11 手写SOFA-JRAFT：引入 LogManager 日志管理体系，实现日志组件初始化(二)

12 手写SOFA-JRAFT：引入 Disruptor 框架，运用回调实现 JRAFT 领导者的日志持久化功能(一)

13 手写SOFA-JRAFT：引入 Disruptor 框架，运用回调实现 JRAFT 领导者的日志持久化功能(二)

14 手写SOFA-JRAFT：剖析日志传输模式，初步实现领导者向跟随者传输日志功能(一)

15 手写SOFA-JRAFT：剖析日志传输模式，初步实现领导者向跟随者传输日志功能(二)

16 手写SOFA-JRAFT：实现跟随者接收日志功能，解决跟随者和领导者日志冲突问题

17 手写SOFA-JRAFT：引入 Pipeline 日志传输模式，再次重构 Replicator 复制器对象

18 手写SOFA-JRAFT：引入 AppendEntriesRequestProcessor，完善 Pipeline 模式

19 手写SOFA-JRAFT：引入 FSMCallerImpl 状态机组件，剖析领导者日志提交全流程(一)

20 手写SOFA-JRAFT：再次使用 Disruptor 框架，在异步回调中实现日志的提交和应用 (二)

21 手写SOFA-JRAFT：再次使用 Disruptor 框架，在异步回调中实现日志的提交和应用 (三)

22 手写SOFA-JRAFT：实现线性一致读功能

23 手写SOFA-JRAFT：引入日志快照组件，创建 snapshotTimer 快照定时生成器，实现快照定时生成(一)

24 手写SOFA-JRAFT：引入日志快照组件，创建 snapshotTimer 快照定时生成器，实现快照定时生成(二)

25 手写SOFA-JRAFT：引入日志快照组件，创建 snapshotTimer 快照定时生成器，实现快照定时生成(三)

26 手写SOFA-JRAFT：引入日志快照组件，创建 snapshotTimer 快照定时生成器，实现快照定时生成(四)

27 手写SOFA-JRAFT：引入日志快照组件，创建 snapshotTimer 快照定时生成器，实现快照定时生成(五)

28 手写SOFA-JRAFT：完善日志快照组件，剖析领导者向跟随者节点安装快照流程(一)

29 手写SOFA-JRAFT：引入快照文件远程传输组件，实现跟随者节点的快照安装(二)

30 手写SOFA-JRAFT：完善 ConfigurationCtx 配置内部类功能，实现集群配置顺利变更(一)

**31 手写SOFA-JRAFT：完善 ConfigurationCtx 配置内部类功能，实现集群配置顺利变更(二)**

**32 手写SOFA-JRAFT：实现 CliServiceImpl 集群客户端功能**

33 手写SOFA-JRAFT：实现优雅停机功能(一)

34 手写SOFA-JRAFT：实现优雅停机功能(二)

35 手写SOFA-JRAFT：实现优雅停机功能(三)