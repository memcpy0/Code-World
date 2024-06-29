用都用不到怎么学？
没有场景、没有诉求，怎么学习这些似乎用不上知识点。
其实最好的方式就是归纳、整理、实践、输出，一套组合拳下来，你就掌握了这个系列的知识了。
但在当前阶段可能真的用不上，JVM 是一个稳定服务，哪能天天出问题，哪需要你老排查。又不是像你写的代码那样！，可是知识的学习就是**把你垫基到更高层次后，才有机会接触更有意思的工作和技术创新**。如果只是单纯的学几个指令，其实并没有多有意思。但**让你完成一套全链路监控，里面需要含有一次方法调用的整体耗时、执行路径、参数信息、异常结果、GC 次数、堆栈数据、分代内容**等等的时候，那么你的知识储备够开发一个这样的系统吗？

好，先上图看看本文要讲啥，再跟着小傅哥的步伐往下走。

JVM 故障处理工具
# 一、面试题
谢飞机，小记！，周末休息在家无聊，把已经上灰了的 JVM 虚拟机学习翻出来。
谢飞机：呱...呱...，喂大哥，这个，这个 JVM 虚拟机看啥呀。
面试官：看啥？不知道从哪开始？嗯，那你从问题点下手！
谢飞机：啥问题点呢，我就是不知道自己不会啥，也不知道问你啥。
面试官：啊！那我问你个，怎么通过 JVM 故障处理工具，查看 JVM 启动时参数都配置了什么呢？
谢飞机：这个！？不道呀！
面试官：那你熟悉的监控指令都有啥，如果问你堆内存统计如何统计，你可知晓！？
谢飞机：也不知道，哈哈哈，好像知道要去看啥了！
面试官：去吧，带着问题看，看完整理出来！
## 二、基础故障处理工具
### 1. jps 虚拟机进程状况
jps（JVM Process Status Tool），它的功能与 ps 命令类似，可以列出正在运行的虚拟机进程，并显示虚拟机执行主类（Main Class，main()函数所在的类）名称以及这些进程的本地虚拟机唯一ID（ Local Virtual Machine Identifier, LVMID），类似于 ps -ef | grep java 的功能。
**这小家伙虽然不大，功能又单一，但可以说基本你用其他命令都得先用它**，来查询到 LVMID 来确定要监控的是哪个虚拟机进程。
命令格式
jps [ options ] [ hostid ]
- options：选项、参数，不同的参数可以输出需要的信息
- hostid：远程查看

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292014398.png)
## 1.1 jps -q，只列出进程 ID
```bash
E:\itstack\git\github.com\interview>jps -q
104928
111552
26852
96276
59000
8460
76188
```
## 1.2 jps -l，输出当前运行类全称
```bash
E:\itstack\git\github.com\interview>jps -l
111552 org/netbeans/Main
26852
96276 org.jetbrains.jps.cmdline.Launcher
59000
62184 sun.tools.jps.Jps
8460 org/netbeans/Main
76188 sun.tools.jstatd.Jstatd
```
用这个命令输出的内容就清晰多了，-l 也是非常常用的一个参数选项。
## 1.3 jps -m，列出传给 main()函数的参数
```
E:\itstack\git\github.com\interview>jps -m
111552 Main --branding visualvm --cachedir C:\Users\xiaofuge\AppData\Local\VisualVM\Cache/8u131 --
openid 3041391569375200
26852
96276 Launcher C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/plugins/java/lib/javac2.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/plugins/java/lib/aether-api-1.1.0.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/lib/jna-platform.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/lib/guava-27.1-jre.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/lib/httpclient-4.5.10.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/lib/forms-1.1-preview.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/plugins/java/lib/aether-connector-basic-1.1.0.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/plugins/java/lib/maven-model-builder-3.3.9.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/lib/jps-model.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/plugins/java/lib/maven-model-3.3.9.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/plugins/java/lib/aether-impl-1.1.0.jar;C:/Program Files/JetBrains/IntelliJ IDEA 2019.3.1/lib/gson-2.8.5.jar;C:/Program File
59000
16844 Jps -m
8460 Main --branding visualvm --cachedir C:\Users\xiaofuge\AppData\Local\VisualVM\Cache/8u131 --openid 3041414336579200
76188 Jstatd
```
## 1.4 jps -v，输出虚拟机进程启动时 JVM 参数[-Xms24m -Xmx256m]
```bash
E:\itstack\git\github.com\interview>jps -v
111552 Main -Xms24m -Xmx256m -Dsun.jvmstat.perdata.syncWaitMs=10000 -
Dsun.java2d.noddraw=true -Dsun.java2d.d3d=false -Dnetbeans.keyring.no.master=true -
Dplugin.manager.install.global=false --add-exports=java.desktop/sun.awt=ALL-
UNNAMED --add-exports=jdk.jvmstat/sun
.jvmstat.monitor.event=ALL-UNNAMED --add-
exports=jdk.jvmstat/sun.jvmstat.monitor=ALL-UNNAMED --add-
exports=java.desktop/sun.swing=ALL-UNNAMED --add-
exports=jdk.attach/sun.tools.attach=ALL-UNNAMED --add-modules=java.activation -
XX:+IgnoreUnrecognizedVMOptions -Djdk.
home=C:/Program Files/Java/jdk1.8.0_161 -
Dnetbeans.home=C:\Program Files\Java\jdk1.8.0_161\lib\visualvm\platform -
Dnetbeans.user=C:\Users\xiaofuge1\AppData\Roaming\VisualVM\8u131 -
Dnetbeans.default_userdir_root=C:\Users\xiaofuge1\AppData\Roaming\VisualVM -XX:+H
eapDumpOnOutOfMemoryError -
XX:HeapDumpPath=C:\Users\xiaofuge1\AppData\Roaming\VisualVM\8u131\var\log\heapdump.
hprof -Dsun.awt.keepWorkingSetOnMinimize=true -
Dnetbeans.dirs=C:\Program Files\Java\jdk1.8.0_161\lib\visualvm\visualvm;C:\Program
59000 -Dfile.encoding=UTF-8 -Xms128m -Xmx1024m -XX:MaxPermSize=256m
76188 Jstatd -
Denv.class.path=.;C:\Program Files\Java\jre1.8.0_161\lib;C:\Program Files\Java\jre1
.8.0_161\lib\tool.jar; -Dapplication.home=C:\Program Files\Java\jdk1.8.0_161 -
Xms8m -Djava.security.policy=jstatd.all.policy
```
## 1.5 jps -lv 127.0.0.1，输出远程机器信息
jps 链接远程输出 JVM 信息，需要注册 RMI，否则会报错 RMI Registry not available at 127.0.0.1。
注册 RMI 开启 jstatd 在你的 C:\Program Files\Java\jdk1.8.0_161\bin 目录
下添加名称为 jstatd.all.policy 的文件。无其他后缀

jstatd.all.policy 文件内容如下：
```java
grant codebase "file:${java.home}/../lib/tools.jar" {
permission java.security.AllPermission;
};
```
添加好配置文件后，在 bin 目录下注册添加的 jstatd.all.policy 文件：
```java
C:\Program Files\Java\jdk1.8.0_161\bin>jstatd -J -Djava.security.policy=jstatd.all.policy
```
顺利的话现在就可以查看原创机器 JVM 信息了，如下：
```java
E:\itstack\git\github.com\interview>jps -l 127.0.0.1
111552 org/netbeans/Main
26852
96276 org.jetbrains.jps.cmdline.Launcher
36056 sun.tools.jps.Jps
59000
8460 org/netbeans/Main
76188 sun.tools.jstatd.Jstatd
```

也可以组合使用 jps 的选项参数，比如：jps -lm 127.0.0.1
## 2. jcmd 虚拟机诊断命令
jcmd，是从 jdk1.7 开始新发布的 JVM 相关信息诊断工具，可以用它来导出堆和线程信息、查看 Java 进程、执行 GC、还可以进行采样分析（jmc 工具的飞行记录器）。注意其使用条件是**只能在被诊断的 JVM 同台 sever 上，并且具有相同的用户和组(user and group)**.

命令格式
```java
jcmd <pid | main class> <command ...|PerfCounter.print|-f file>
```
- pid，接收诊断命令请求的进程 ID
- main class，接收诊断命令请求的进程 main 类。
- command，接收诊断命令请求的进程 main 类。
- PerfCounter.print，打印目标 Java 进程上可用的性能计数器。
- -f file，从文件 file 中读取命令，然后在目标 Java 进程上调用这些命令。
- -l，查看所有进程列表信息。
- -h、-help，查看帮助信息。

### 2.1 jcmd pid VM.flags，查看 JVM 启动参数
```bash
E:\itstack\git\github.com\interview>jcmd 111552 VM.flags
111552:
-XX:CICompilerCount=4 -XX:+HeapDumpOnOutOfMemoryError -
XX:HeapDumpPath=C:\Users\xiaofuge1\AppData\Roaming\VisualVM\8u131\var\log\heapdump.
hprof -XX:+IgnoreUnrecognizedVMOptions -XX:InitialHeapSize=25165824 -
XX:MaxHeapSize=268435456 -XX:MaxNewSize=89128960 -XX:Min
HeapDeltaBytes=524288 -XX:NewSize=8388608 -XX:OldSize=16777216 -
XX:+UseCompressedClassPointers -XX:+UseCompressedOops -
XX:+UseFastUnorderedTimeStamps -XX:-UseLargePagesIndividualAllocation -
XX:+UseParallelGC
```
### 2.2 jcmd pid VM.uptime，查看 JVM 运行时长
```bash
E:\itstack\git\github.com\interview>jcmd 111552 VM.uptime
111552:
583248.912 s
```
### 2.3 jcmd pid PerfCounter.print，查看 JVM 性能相关参数
```bash
E:\itstack\git\github.com\interview>jcmd 111552 PerfCounter.print
111552:
java.ci.totalTime=56082522
java.cls.loadedClasses=5835
java.cls.sharedLoadedClasses=0
java.cls.sharedUnloadedClasses=0
java.cls.unloadedClasses=37
...
```
## 2.4 jcmd pid GC.class_histogram，查看系统中类的统计信息
```java
E:\itstack\git\github.com\interview>jcmd 111552 GC.class_histogram
111552:
num #instances #bytes class name
----------------------------------------------
1: 50543 3775720 [C
2: 3443 2428248 [I
3: 50138 1203312 java.lang.String
4: 25351 811232 java.util.HashMap$Node
5: 6263 712208 java.lang.Class
6: 3134 674896 [B
7: 6687 401056 [Ljava.lang.Object;
8: 2468 335832 [Ljava.util.HashMap$Node;
```

## 2.5 jcmd pid Thread.print，查看线程堆栈信息
```java
E:\itstack\git\github.com\interview>jcmd 111552 Thread.print
111552:
2021-01-10 23:31:13
Full thread dump Java HotSpot(TM) 64-Bit Server VM (25.161-b12 mixed mode):
"Computes values in handlers" #52 daemon prio=5 os_prio=0 tid=0x0000000019839000 ni
d=0x16014 waiting for monitor entry [0x0000000026bce000]
java.lang.Thread.State: BLOCKED (on object monitor)
at com.sun.tools.visualvm.core.model.ModelFactory.getModel(ModelFactory.jav
a:76)
- waiting to lock <0x00000000f095bcf8> (a com.sun.tools.visualvm.jvmstat.ap
plication.JvmstatApplication)
at com.sun.tools.visualvm.application.jvm.JvmFactory.getJVMFor(JvmFactory.j
ava:45)
at com.sun.tools.visualvm.application.options.Open.openApplication(Open.jav
a:108)
at com.sun.tools.visualvm.application.options.Open.process(Open.java:93)
at org.netbeans.spi.sendopts.Option$1.process(Option.java:348)
at org.netbeans.api.sendopts.CommandLine.process(CommandLine.java:278)
at org.netbeans.modules.sendopts.HandlerImpl.execute(HandlerImpl.java:23)
at org.netbeans.modules.sendopts.Handler.cli(Handler.java:30)
at org.netbeans.CLIHandler.notifyHandlers(CLIHandler.java:195)
at org.netbeans.core.startup.CLICoreBridge.cli(CLICoreBridge.java:43)
at org.netbeans.CLIHandler.notifyHandlers(CLIHandler.java:195)
at org.netbeans.CLIHandler$Server$1ComputingAndNotifying.run(CLIHandler.jav
a:1176)
```
## 2.6 jcmd pid VM.system_properties，查看 JVM 系统参数
```java
E:\itstack\git\github.com\interview>jcmd 111552 VM.system_properties
111552:
#Sun Jan 13 23:33:19 CST 2021
java.vendor=Oracle Corporation
netbeans.user=C\:\\Users\\xiaofuge1\\AppData\\Roaming\\VisualVM\\8u131
sun.java.launcher=SUN_STANDARD
sun.management.compiler=HotSpot 64-Bit Tiered Compilers
netbeans.autoupdate.version=1.23
os.name=Windows 10
```
### 2.7 jcmd pid GC.heap_dump 路径，导出 heap dump 文件
```
E:\itstack\git\github.com\interview>jcmd 111552 GC.heap_dump C:\Users\xiaofuge1\Des
ktop\_dump_0110
111552:
Heap dump file created
```
导出的文件需要配合 jvisualvm 查看
### 2.8 jcmd pid help，列出可执行操作
```
E:\itstack\git\github.com\interview>jcmd 111552 help
111552:
The following commands are available:
JFR.stop
JFR.start
JFR.dump
JFR.check
```
### 2.9 jcmd pid help JFR.stop，查看命令使用
```bash
E:\itstack\git\github.com\interview>jcmd 111552 help JFR.stop
111552:
JFR.stop
Stops a JFR recording
Impact: Low
Permission: java.lang.management.ManagementPermission(monitor)
Syntax : JFR.stop [options]
Options: (options must be specified using the <key> or <key>=<value> syntax)
name : [optional] Recording name,.e.g \"My Recording\" (STRING, no default
value)
recording : [optional] Recording number, see JFR.check for a list of availa
ble recordings (JLONG, -1)
discard : [optional] Skip writing data to previously specified file (if any
) (BOOLEAN, false)
filename : [optional] Copy recording data to file, e.g. \"C:\Users\user\My
Recording.jfr\" (STRING, no default value)
compress : [optional] GZip-compress "filename" destination (BOOLEAN, false)
```
## 3. jinfo Java 配置信息工具
jinfo（Configuration Info for Java），实时查看和调整 JVM 的各项参数。

在上面讲到 jps -v 指令时，可以看到它把虚拟机启动时显式的参数列表都打印出来了，但如果想**更加清晰的看具体的一个参数**或者想知道未被显式指定的参数时，就可以通过 jinfo -flag 来查询了。
命令格式
jinfo [ option ] pid
使用方式
```bash
E:\itstack\git\github.com\interview>jinfo -flag MetaspaceSize 111552
-XX:MetaspaceSize=21807104
E:\itstack\git\github.com\interview>jinfo -flag MaxMetaspaceSize 111552
-XX:MaxMetaspaceSize=18446744073709486080
E:\itstack\git\github.com\interview>jinfo -flag HeapDumpPath 111552
-XX:HeapDumpPath=C:\Users\xiaofuge\AppData\Roaming\VisualVM\8u131\var\log\heapdump.h
prof
```
各种 JVM 参数你都可以去查询，这样更加方便的只把你要的显示出来。

## 4. jstat 收集虚拟机运行数据
jstat（JVM Statistics Monitoring Tool），用于监视虚拟机各种运行状态信息。它可以查看本地或者远程虚拟机进程中，类加载、内存、垃圾收集、即时编译等运行时数据。

命令格式
```bash
jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]
```
- vmid：如果是查看远程机器，需要按照此格式：
    [protocol:][//]lvmid[@hostname[:port]/servername]
- interval 和 count，表示查询间隔和次数，比如每隔 1000 毫秒查询一次进程 ID 的gc 收集情况，每次查询 5 次。jstat -gc 111552 1000 5
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292155244.png)

### 4.01 jstat -class，类加载统计
```
E:\itstack\git\github.com\interview>jstat -class 111552
Loaded Bytes Unloaded Bytes Time
5835 12059.6 37 53.5 3.88
```
- Loaded，加载 class 的数量
- Bytes：所占用空间大小
- Unloaded：未加载数量
- Bytes：未加载占用空间
- Time：时间
### 4.02 jstat -compiler，编译统计
```bash
E:\itstack\git\github.com\interview>jstat -compiler 111552
Compiled Failed Invalid Time FailedType FailedMethod
3642 0 0 5.61 0
```
- Compiled：编译数量
- Failed：失败数量
- Invalid：不可用数量
- Time：时间
- FailedType：失败类型
- FailedMethod：失败方法
### 4.03 jstat -gc，垃圾回收统计
```java
E:\itstack\git\github.com\interview>jstat -gc 111552
S0C S1C S0U S1U EC EU OC OU MC MU CCSC CCSU YGC YGCT FGC FGCT GCT
1024.0 512.0 0.0 0.0 77312.0 35.1 39424.0 13622.9 37120.0 34423.3 5376.0 4579.4 60 0.649 52 3.130 3.779
```
- 各种 JVM 参数你都可以去查询，这样更加方便的只把你要的显示出来。
- S0C、S1C，第一个和第二个幸存区大小
- S0U、S1U，第一个和第二个幸存区使用大小
- EC、EU，伊甸园的大小和使用
- OC、OU，老年代的大小和使用
- MC、MU，方法区的大小和使用
- CCSC、CCSU，压缩类空间大小和使用
- YGC、YGCT，年轻代垃圾回收次数和耗时
- FGC、FGCT，老年代垃圾回收次数和耗时
- GCT，垃圾回收总耗时

## 4.04 jstat -gccapacity，堆内存统计
```bash
E:\itstack\git\github.com\interview>jstat -gccapacity 111552
NGCMN NGCMX NGC S0C S1C EC OGCMN OGCMX OGC
OC MCMN MCMX MC CCSMN CCSMX CCSC YGC FGC
8192.0 87040.0 80384.0 1024.0 512.0 77312.0 16384.0 175104.0 39424.0
39424.0 0.0 1081344.0 37120.0 0.0 1048576.0 5376.0 60 52
```
- NGCMN、NGCMX，新生代最小和最大容量
- NGC，当前新生代容量
- S0C、S1C，第一和第二幸存区大小
- EC，伊甸园区的大小
- OGCMN、OGCMX，老年代最小和最大容量
- OGC、OC，当前老年代大小
- MCMN、MCMX，元数据空间最小和最大容量
- MC，当前元空间大小
- CCSMN、CCSMX，压缩类最小和最大空间
- YGC，年轻代 GC 次数
- FGC，老年代 GC 次数

## 4.05 jstat -gcnewcapacity，新生代内存统计
```bash
E:\itstack\git\github.com\interview>jstat -gcnewcapacity 111552
NGCMN NGCMX NGC S0CMX S0C S1CMX S1C ECMX
EC YGC FGC
8192.0 87040.0 80384.0 28672.0 1024.0 28672.0 512.0 86016.0
77312.0 60 52
```
- NGCMN、NGCMX，新生代最小和最大容量
- NGC，当前新生代容量
- S0CMX，最大幸存 0 区大小
- S0C，当前幸存 0 区大小
- S1CMX，最大幸存 1 区大小
- S1C，当前幸存 1 区大小
- ECMX，最大伊甸园区大小
- EC，当前伊甸园区大小
- YGC，年轻代垃圾回收次数
- FGC，老年代回收次数

## 4.06 jstat -gcnew，新生代垃圾回收统计
```bash
E:\itstack\git\github.com\interview>jstat -gcnew 111552
S0C S1C S0U S1U TT MTT DSS EC EU YGC YGCT
1024.0 512.0 0.0 0.0 3 15 512.0 77312.0 70.2 60 0.649
```
- S0C、S1C，第一和第二幸存区大小
- S0U、S1U，第一和第二幸存区使用
- TT，对象在新生代存活的次数
- MTT，对象在新生代存活的最大次数
- DSS：期望的幸存区大小
- EC，伊甸园区的大小
- EU，伊甸园区的使用
- YGC，年轻代垃圾回收次数
- YGCT，年轻代垃圾回收消耗时间
## 4.07 jstat -gcold，老年代垃圾回收统计
```bash
E:\itstack\git\github.com\interview>jstat -gcold 111552
MC MU CCSC CCSU OC OU YGC FGC FGCT GCT
37120.0 34423.3 5376.0 4579.4 39424.0 13622.9 60 52 3.130
3.779
```
- MC、MU，方法区的大小和使用
- CCSC、CCSU，压缩类空间大小和使用
- OC、OU，老年代大小和使用
- YGC，年轻代垃圾回收次数
- FGC，老年代垃圾回收次数
- FGCT，老年代垃圾回收耗时
- GCT，垃圾回收总耗时

## 4.08 jstat -gcoldcapacity，老年代内存统计
```bash
E:\itstack\git\github.com\interview>jstat -gcoldcapacity 111552
OGCMN OGCMX OGC OC YGC FGC FGCT GCT
16384.0 175104.0 39424.0 39424.0 60 52 3.130 3.779
```
- OGCMN、OGCMX，老年代最小和最大容量
- OGC，当前老年代大小
- OC，老年代大小
- YGC，年轻代垃圾回收次数
- FGC，老年代垃圾回收次数
- FGCT，老年代垃圾回收耗时
- GCT，垃圾回收消耗总耗时
## 4.09 jstat -gcmetacapacity，元空间统计
```bash
E:\itstack\git\github.com\interview>jstat -gcmetacapacity 111552
MCMN MCMX MC CCSMN CCSMX CCSC YGC FGC FGC T GCT
0.0 1081344.0 37120.0 0.0 1048576.0 5376.0 60 52 3.
130 3.779
```
- MCMN、MCMX，元空间最小和最大容量
- MC，当前元数据空间大小
- CCSMN、CCSMX，压缩类最小和最大空间
- CCSC，压缩类空间大小
- YGC，年轻代垃圾回收次数
- FGC，老年代垃圾回收次数
- FGCT，老年代垃圾回收耗时
- GCT，垃圾回收消耗总耗时

## 4.10 jstat -gcutil，垃圾回收统计
```bash
E:\itstack\git\github.com\interview>jstat -gcutil 111552
S0 S1 E O M CCS YGC YGCT FGC FGCT GCT
0.00 0.00 0.09 34.55 92.74 85.18 60 0.649 52 3.130 3.779
```
- S0、S1、幸存 1 区和 2 区，当前使用占比
- E，伊甸园区使用占比
- O，老年代区使用占比
- M，元数据区使用占比
- CCS，压缩类使用占比
- YGC，年轻代垃圾回收次数
- FGC，老年代垃圾回收次数
- FGCT，老年代垃圾回收耗时
- GCT，垃圾回收消耗总耗时

### 4.11 jstat -printcompilation，JVM 编译方法统计
```bash
E:\itstack\git\github.com\interview>jstat -printcompilation 111552
Compiled Size Type Method
3642 9 1 java/io/BufferedWriter min
```
- Compiled：最近编译方法的数量
- Size：最近编译方法的字节码数量
- Type：最近编译方法的编译类型
- Method：方法名标识

## 5. jmap 内存映射工具
jmap（Memory Map for Java），用于生成堆转储快照（heapdump 文件）。jmap 的作用除了获取堆转储快照，还可以查询 finalize 执行队列、Java 堆和方法区的详细信息。
命令格式
```
jmap [ option ] pid
```
- option：选项参数
- pid：需要打印配置信息的进程 ID
- executable：产生核心 dump 的 Java 可执行文件
- core：需要打印配置信息的核心文件
- server-id：可选的唯一 id，如果相同的远程主机上运行了多台调试服务器，用此选项参数标识服务器
- remote server IP or hostname： 远程调试服务器的 IP 地址或主机名

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292221262.png)
### 5.1 jmap，打印共享对象映射
```bash
E:\itstack\git\github.com\interview>jmap 111552
Attaching to process ID 111552, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.161-b12
0x000000005b4a0000 1632K C:\Program Files\Java\jdk1.8.0_161\jre\bin\awt.dll
0x000000005b8c0000 264K C:\Program Files\Java\jdk1.8.0_161\jre\bin\t2k.dll
0x000000005b910000 284K C:\Program Files\Java\jdk1.8.0_161\jre\bin\fontmana
ger.dll
0x000000005b960000 224K C:\Program Files\Java\jdk1.8.0_161\jre\bin\splashsc
reen.dll
0x000000005b9a0000 68K C:\Program Files\Java\jdk1.8.0_161\jre\bin\nio.dll
```
### 5.2 jmap -heap，堆详细信息
```bash
E:\itstack\git\github.com\interview>jmap -heap 111552
Attaching to process ID 111552, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.161-b12
using thread-local object allocation.

Parallel GC with 8 thread(s)
Heap Configuration:
    MinHeapFreeRatio = 0
    MaxHeapFreeRatio = 100
    MaxHeapSize = 268435456 (256.0MB)
    NewSize = 8388608 (8.0MB)
    MaxNewSize = 89128960 (85.0MB)
    OldSize = 16777216 (16.0MB)
    NewRatio = 2
    SurvivorRatio = 8
    MetaspaceSize = 21807104 (20.796875MB)
    CompressedClassSpaceSize = 1073741824 (1024.0MB)
    MaxMetaspaceSize = 17592186044415 MB
    G1HeapRegionSize = 0 (0.0MB)
```
### 5.3 jmap -clstats，打印加载类
```bash
E:\itstack\git\github.com\interview> jmap -clstats 111552
Attaching to process ID 111552, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.161-b12
finding class loader instances ..done.
computing per loader stat ..done.
please wait.. computing liveness...................................................
..............liveness analysis may be inaccurate ...
class_loader classes bytes parent_loader alive? type
<bootstrap> 3779 6880779 null live <internal>
0x00000000f03853b8 57 132574 0x00000000f031aac8 live org/netbean
s/StandardModule$OneModuleClassLoader@0x00000001001684f0
0x00000000f01b9b98 0 0 0x00000000f031aac8 live org/netbean
s/StandardModule$OneModuleClassLoader@0x00000001001684f0
0x00000000f005b280 0 0 0x00000000f031aac8 live java/util/R
esourceBundle$RBClassLoader@0x00000001000c6ae0
0x00000000f01dfa98 0 0 0x00000000f031aac8 live org/netbean
s/StandardModule$OneModuleClassLoader@0x00000001001684f0
0x00000000f01ec518 79 252894 0x00000000f031aac8 live org/netbean
s/StandardModule$OneModuleClassLoader@0x00000001001684f0
```
### 5.4 jmap -dump，堆转储文件
```bash
E:\itstack\git\github.com\interview>jmap -
dump:live,format=b,file=C:/Users/xiaofuge/Desktop/heap.bin 111552
Dumping heap to C:\Users\xiaofuge\Desktop\heap.bin ...
Heap dump file created
```
## 6. jhat 堆转储快照分析工具
jhat（JVM Heap Analysis Tool），与 jmap 配合使用，用于分析 jmap 生成的堆转储快照。
**jhat 内置了一个小型的 http/web 服务器，可以把堆转储快照分析的结果，展示在浏览器中查看**。不过用途不大，基本大家都会使用其他第三方工具。
命令格式
```
jhat [-stack <bool>] [-refs <bool>] [-port <port>] [-baseline <file>] [-debug <int>] [-version] [-h|-help] <file>
```
命令使用
```
E:\itstack\git\github.com\interview>jhat -
port 8090 C:/Users/xiaofuge1/Desktop/heap.bin
Reading from C:/Users/xiaofuge1/Desktop/heap.bin...
Dump file created Wed Jan 13 16:53:47 CST 2021
Snapshot read, resolving...
Resolving 246455 objects...
Chasing references, expect 49 dots.................................................
Eliminating duplicate references.................................................
Snapshot resolved.
Started HTTP server on port 8090
Server is ready.
http://localhost:8090/
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292224511.png)
## 7. jstack Java 堆栈跟踪工具
jstack（Stack Trace for Java），用于**生成虚拟机当前时刻的线程快照**（threaddump、javacore）。

线程快照就是**当前虚拟机内每一条线程正在执行的方法堆栈**的集合，生成线程快照的目的通常是**定位线程出现长时间停顿的原因**，如：线程死锁、死循环、请求外部资源耗时较长导致挂起等。

线程出现听顿时通过 jstack 来查看各个线程的调用堆栈，就可以获得没有响应的线程在搞什么鬼。
命令格式
```bash
jstack [ option ] vmid
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292225144.png)
命令使用
```bash
E:\itstack\git\github.com\interview>jstack 111552
2021-01-10 23:15:03
Full thread dump Java HotSpot(TM) 64-Bit Server VM (25.161-b12 mixed mode):
"Inactive RequestProcessor thread [Was:StdErr Flush/org.netbeans.core.startup.loggi
ng.PrintStreamLogger]" #59 daemon prio=1 os_prio=-
2 tid=0x000000001983a800 nid=0x688 in Object.wait() [0x0000000017fbf000]
java.lang.Thread.State: TIMED_WAITING (on object monitor)
at java.lang.Object.wait(Native Method)
at org.openide.util.RequestProcessor$Processor.run(RequestProcessor.java:19
39)
- locked <0x00000000fab31d88> (a java.lang.Object)
```
在验证使用的过程中，可以尝试写一个死循环的线程，之后通过 jstack 查看线程信息。

# 三、可视化故障处理工具
## 1. jconsole，Java 监视与管理控制台
JConsole（ Java Monitoring and Management Console），是一款基于 JMX（ Java Management Extensions） 的可视化监视管理工具。

它的功能主要是对系统进行收集和参数调整，不仅可以在虚拟机本身管理还可以开发在软件上，是开放的服务，有相应的代码 API 调用。
### JConsole 启动
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292236035.png)
## JConsole 使用
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292236438.png)
## 2. VisualVM，多合故障处理工具
VisualVM（ All-in-One Java Troubleshooting Tool），是功能最强大的运行监视和故障处理工具之一。
它除了常规的运行监视、故障处理外，还可以做性能分析等工作。因为它的通用性很强，对应用程序影响较小，所以可以直接接入到生产环境中。

### VisualVM IDEA 安装
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292237346.png)
```java
public static void main(String[] args) throws InterruptedException {
Thread.sleep(5000);
ClassLoadingMXBean loadingBean = ManagementFactory.getClassLoadingMXBean();
while (true) {
Enhancer enhancer = new Enhancer();
enhancer.setSuperclass(MetaSpaceOomMock.class);
enhancer.setCallbackTypes(new Class[]{Dispatcher.class, MethodInterceptor.c
lass});
enhancer.setCallbackFilter(new CallbackFilter() {
@Override
public int accept(Method method) {
return 1;
}
@Override
public boolean equals(Object obj) {
return super.equals(obj);
}
});
System.out.println(enhancer.createClass().getName() + loadingBean.getTotalL
oadedClassCount() + loadingBean.getLoadedClassCount() + loadingBean.getUnloadedClas
sCount());
}
}
```
记得调整元空间大小
```
-XX:MetaspaceSize=8m
-XX:MaxMetaspaceSize=80m
-Djava.rmi.server.hostname=127.0.0.1
-Dcom.sun.management.jmxremote
-Dcom.sun.management.jmxremote.port=7397
-Dcom.sun.management.jmxremote.ssl=false
-Dcom.sun.management.jmxremote.authenticate=false
```
我们就监测这段让元空间溢出的代码，java.lang.OutOfMemoryError:Metaspace
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305292239054.png)
这里包括了很多常用的指令，通过这些指令的学习我们也大概会知道 JVM 都给我们提供了什么样的监控信息。

其实实际的业务使用中很少通过指令去监控 JVM 而是有一整套的非入侵全链路监控，在监控服务里与之方法调用时的 JVM 一并监控，可以让研发人员更快速的排查问题。但这些工具的实现依然是需要这些基础，在有了基础的知识掌握后，可以更好多使用工具。