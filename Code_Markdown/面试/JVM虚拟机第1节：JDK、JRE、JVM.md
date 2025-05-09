想问一些面试官，是因为大家都在问所以你问，还是你想从这里问出什么？ 其
实可能很多面试官如果不了解这些技术，往往会被求职者的答案击碎内心，哈哈哈哈哈哈。比如：梅森旋转算法、开放寻址、斐波那契散列、启发式清理、Javassist代理方式、扰动函数、哈希一致等等。
记住，让懂了就是真的懂，比看水文、背答案要爽的多！嗯，就是有时候烧脑！
# 一、面试题
谢飞机，小记！，也不知道咋了，总感觉有些面试攻击性不大，但侮辱性极强！
面试官：谢飞机写过 Java 吗？
谢飞机：那当然写过，写了 3 年多了！
面试官：那，JDK、JRE、JVM 之间是什么关系？
谢飞机：嗯 J J J，JDK 里面有 JRE，JVM 好像在 JRE 里！？
面试官：那，Client 模式、Server 模式是啥？
谢飞机：嗯！？啥？
面试官：好吧，问个简单的。JVM 是如何工作的？背答案了吗？
谢飞机：再见，面试官！
# 二、Java 平台标准(JDK 8)
Oracle has two products that implement Java Platform Standard Edition
(Java SE) 8: Java SE Development Kit (JDK) 8 and Java SE Runtime
Environment (JRE) 8. JDK 8 is a superset of JRE 8, and contains everything that is in JRE 8, plus tools such as the compilers and debuggers necessary for developing applets and applications. JRE 8 provides the libraries, the Java Virtual Machine (JVM), and other components to run applets and applications written in the Java programming language. Note that the JRE includes components not required by the Java SE specification, including both standard and non-standard Java components.

The following conceptual diagram illustrates the components of Oracle's Java SE products: Description of Java Conceptual Diagram
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305230016533.png)

关于 JDK、JRE、JVM 之间是什么关系，在 Java 平台标准中已经明确定义了。也就是上面的英文介绍部分。
- Oracle 有两个 Java 平台标准的产品，Java SE 开发工具包(JDK) 和 Java SE 运行时环境(JRE)。
- JDK(Java Development Kit Java 开发工具包)，JDK 是提供给 Java 开发人员使用的，其中包含了 java 的开发工具，也包括了 JRE。所以安装了 JDK，就不用单独安装JRE 了。其中的开发工具包括编译工具(javac.exe) 打包工具(jar.exe)等。
- JRE(Java Runtime Environment Java 运行环境) 是 JDK 的子集，也就是包括 JRE 所有内容，以及开发应用程序所需的编译器和调试器等工具。JRE 提供了**库、Java 虚拟机（JVM）和其他组件**，用于运行 Java 编程语言、小程序、应用程序。
- JVM(Java Virtual Machine Java 虚拟机)，JVM 可以理解为是**一个虚拟出来的计算机，具备着计算机的基本运算方式**，它主要负责把 Java 程序生成的字节码文件，解释成具体系统平台上的机器指令，让其在各个平台运行。

综上，从这段官网的平台标准介绍和概念图可以看出，我们运行程序的 JVM 是已经安装到 JDK 中，只不过可能你开发了很久的代码，也没有注意过。没有注意过的最大原因是，没有开发过一些和 JVM 相关的组件代码。关于，各 JDK 版本的平台标准，可以自行比对学习，如下：
- Java SE 6 Documentation：https://docs.oracle.com/javase/6/docs/
- Java Platform Standard Edition 7 Documentation：https://docs.oracle.com/javase/7/docs/
- Java Platform Standard Edition 8 Documentation：https://docs.oracle.com/javase/8/docs/

# 三、JDK 目录结构和作用
我们默认安装完 JDK 会有 jdk1.8.0_45、jre1.8.0_45，两个文件夹。其实在 JDK的文件中还会有 JRE 的文件夹，他们两个 JRE 文件夹的结构是一样的。
- bin：一堆 EXE 可执行文件，java.exe、javac.exe、javadoc.exe，以及密钥管理工具等。
- db：内置了 Derby 数据库，体积小，免安装。
- include：Java 和 JVM 交互的头文件，例如我们 JVMTI 写的 C++ 工程时，就需要把这个 include 包引入进去 jvmti.h。例如：**基于 jvmti 设计非入侵监控**
- jre：Java 运行环境，包含了运行时需要的可执行文件，以及运行时需要依赖的Java 类库和动态链接库.so .dll .dylib
- lib：Java 类库，例如 dt.jar、tools.jar
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250754142.png)
那么 jvm 在哪个文件夹呢？可 能 你 之 前 并 没 有 注 意 过 jvm 原 来 在 这 里 ： `C:\ProgramFiles\Java\jdk1.8.0_45\jre\bin\server`
- 这部分是整个 Java 实现跨平台的最核心内容，由 Java 程序编译成的 .class 文件会在虚拟机上执行。
- 另外在 JVM 解释 class 文件时需要调用类库 lib。在 JRE 目录下有两个文件夹lib、bin，而 lib 就是 JVM 执行所需要的类库。
- **jvm.dll 并不能独立工作，当 jvm.dll 启动后，会使用 explicit 方法来载入辅助动态链接库一起执行**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250755712.png)

# 四、JDK 是什么？
综上通过 **Java 平台标准**和 **JDK 的目录结构**，JDK 是 JRE 的超集，JDK 包含了 JRE 所有的开发、调试以及监视应用程序的工具。以及如下重要的组件：
- java – 运行工具，运行 .class 的字节码
- javac– 编译器，将后缀名为.java 的源代码编译成后缀名为.class 的字节码
- javap – 反编译程序
- javadoc – 文档生成器，从源码注释中提取文档，注释需符合规范
- jar – 打包工具，将相关的类文件打包成一个文件
- jdb – debugger，调试工具
- jps – 显示当前 java 程序运行的进程状态
- appletviewer – 运行和调试 applet 程序的工具，不需要使用浏览器
- javah – 从 Java 类生成 C 头文件和 C 源文件。这些文件提供了连接胶合，使 Java和 C 代码可进行交互。
- javaws – 运行 JNLP 程序
- extcheck – 一个检测 jar 包冲突的工具
- apt – 注释处理工具
- jhat – java 堆分析工具
- jstack – 栈跟踪程序
- jstat – JVM 检测统计工具
- jstatd – jstat 守护进程
- jinfo – 获取正在运行或崩溃的 java 程序配置信息
- jmap – 获取 java 进程内存映射信息
- idlj – IDL-to-Java 编译器. 将 IDL 语言转化为 java 文件
- policytool – 一个 GUI 的策略文件创建和管理工具
- jrunscript – 命令行脚本运行
- appletviewer：小程序浏览器，一种执行 HTML 文件上的 Java 小程序的 Java 浏览器

# 五、JRE 是什么？
JRE 本身也是一个运行在 CPU 上的程序，用于解释执行 Java 代码。一般像是实施的工作，会在客户现场安装 JRE，因为这是运行 Java 程序的最低要求。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250801036.png)
JRE 目录结构 lib、bin

- bin：**有 java.exe 但没有 javac.exe**。也就是无法编译 Java 程序，但可以运行Java 程序，可以把这个 bin 目录理解成 JVM。
- lib：**Java 基础&核心类库**，包含 JVM 运行时需要的类库和 rt.jar。也包含用于安全管理的文件，这些文件包括安全策略(security policy)和安全属性(security properties)等。
# 六、JVM 是什么？
其实简单说 JVM 就是运行 Java 字节码的虚拟机，JVM 是一种规范，各个供应商都可以实现自己 JVM 虚拟机。就像小傅哥自己也按照虚拟机规范和手写 JVM的相关书籍实现了，基于 Java 实现的 JVM 虚拟机。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305250802645.png)

源码地址：https://github.com/fuzhengwei/itstack-demo-jvm

内容简介：本代码主要介绍如何通过 java 代码来实现 JVM 的基础功能（搜索解析 class 文件、字节码命令、运行时数据区等），从而让 java 程序员通过最熟知的 java 程序，学习 JVM 是如何将 java 程序一步步跑起来的。

当然，我们下载 Oracle 公司的 JVM 与自己实现的相比，要高级的多。他们的设计有不断优化的内存模型、GC 回收策略、自适应优化器等。

另外，JVM 之所以称为虚拟机，主要就是因为它为了实现 “write-once-run-anywhere”。提供了一个不依赖于底层操作系统和机器硬件结构的运行环境。
## 1. Client 模式、Server 模式
在 JVM 中有两种不同风格的启动模式， Client 模式、Server 模式。
- Client 模式：加载速度较快。可以用于运行 GUI 交互程序。
- Server 模式：加载速度较慢但运行起来较快。可以用于运行服务器后台程序。

修改配置模式文件：`C:\Program Files\Java\jre1.8.0_45\lib\amd64\jvm.cfg`
```
# List of JVMs that can be used as an option to java, javac, etc.
# Order is important -- first in this list is the default JVM.
# NOTE that this both this file and its format are UNSUPPORTED and
# WILL GO AWAY in a future release.
#
# You may also select a JVM in an arbitrary location with the
# "-XXaltjvm=<jvm_dir>" option, but that too is unsupported
# and may not be available in a future release.
#
-server KNOWN
-client IGNORE
```
如果需要调整，可以把 client 设置为 KNOWN，并调整到 server 前面。
- JVM 默认在 Server 模式下，-Xms128M、-Xmx1024M
- JVM 默认在 Client 模式下，-Xms1M、-Xmx64M

## 2. JVM 结构和执行器
这部分属于 JVM 的核心知识，但不是本篇重点，会在后续的章节中陆续讲到。本章只做一些介绍。
- Class Loader：类装载器是用于加载类文件的一个子系统，其主要功能有三个：loading(加载），linking（链接）,initialization（初始化）。
- JVM Memory Areas：方法区、堆区、栈区、程序计数器。
- Interpreter(解释器)：通过查找预定义的 JVM 指令到机器指令映射，JVM 解释器可以将每个字节码指令转换为相应的本地指令。它直接执行字节码，不执行任何优化。
- JIT Compiler(即时编译器)：为了提高效率，**JIT Compiler 在运行时与 JVM 交互，并适当将字节码序列编译为本地机器代码**。典型地，JIT Compiler 执行一段代码，不是每次一条语句。优化这块代码，并将其翻译为优化的机器代码。JIT Compiler是默认开启
# 七、总结
这篇的知识并不复杂，涉及的面试内容也较少，更多的是对接下来要讲到 JVM 相关面试内容的一个开篇介绍，为后续的要讲的内容做一个铺垫。

如果你在此之前没有关注过 JDK、JRE、JVM 的结构和相应的组件配置以及执行模式，那么可以在此基础上继续学习加深印象。另外想深入学习 JVM 并不太容易，既要学习 JVM 规范也要上手应用实践，所以很建议先手写 JVM，再实践验证JVM。

---

