第 2 章
Java 程序设计环境
▲ 安装 Java 开发T具包
▲ 运行图形化应用程序
▲ 使用命令行工具
▲ 构建并运行 applet
▲ 使用集成开发环境
本章主要介绍如何安装 Java 开发工具包（ JDK ) 以及如何编译和运行不同类型的程序：
控制台程序、 图形化应用程序以及 applet。运行 JDK 工具的方法是在终端窗口中键人命令。
然而, 很多程序员更喜欢使用集成开发环境。 为此，将在稍后介绍如何使用免费的开发环境
编译和运行 Java 程序。 尽管学起来很容易， 但集成开发环境需要吞噬大量资源， 编写小型程
序时也比较烦琐。 一旦掌握了本章的技术， 并选定了自己的开发工具， 就可以学习第 3 章，
开始研究 Java 程序设计语言:.
2 . 1
安 装 Java 幵发工具包
Oracle 公司为 Linux 、 Mac OS X 、Solaris 和 Windows 提供了 Java 开发工具包（ JDK ) 的
最新、 最完整的版本。 用于很多其他平台的版本仍处于多种不同的开发状态中， 不过，这些
版本都由相应平台的开发商授权并分发。
2.1 .1
下载 JDK
要想下载 Java 开发1:具包， 可以访问 Oracle 网站：www.oracle.com/technetwork/java/javase/
downloads, 在得到所需的软件之前必须弄清楚大量专业术语。请看表 2-1 的总结„
表 2-1
Java 术语
术 语 名
缩 写
解
释
Java Development Kit
JDK
编M Java 程序的程序员使用的软件
Java Runtime Environment
JRE
运行 Java 程序的用户使用的软件
Server JRE
fr:服务器丨•.运行 Java 程序的软件
Standard Edition
Sb：
川尸桌面或简单服务器应用的 Java 平台
Enterprise Edition
EE
川
•复杂服务器应用的 Java 平台
Micro Edition
ME
iiJ r
•手机和
•他小搜设备的 Java f-f?
Java FX
Ill丨•图形化用户界面的一个替代 T：
具包， 在 Oracle 的 Java SE 发布版本中提供
OpenJDK
JavaSE 的
• 个免费幵源实现， +包含浏览器集成或 JavaFX
Java 2
J2
- 个过时的术语，丨丨丨丨•描述丨998 年 〜 2006 年之间的 Java 版本
Software Development Kit
SDK
• 个过时的术语， 用于描述 1998 年 ~ 2006 年之间的 JDK
Update
u
Oracle 的术语， 表示 bug 修正版本
NetBeans
Oracle 的集成开发环境
Java 程序???境
13
你已经看到， JDK 是 Java Development Kit 的缩写。有点混乱的是： 这个工具包的版本
1.2 ~ 版本 1.4 被称为 Java SDK ( 软件开发包， Software Development Kit )。 在某些场合下，
还可以看到这个过时的术语。另外， 还有一个术语是 Java 运行时环境（JRE), 它包含虚拟机
但不包含编译器。这并不是开发者想要的环境， 而是专门为不需要编译器的用户而提供。
接下来，Java SE 会大量出现， 相对于 Java EE ( Enterprise Edition) 和 Java ME ( Micro
Edition), 它是 Java 的标准版。
Java 2 这种提法始于 1998 年。当时 Sim 公司的销售人员感觉增加小数点后面的数值改
变版本号并没有反映出 JDK 1.2 的重大改进。但是， 由于在发布之后才意识到这个问题， 所
以决定开发工具包的版本号仍然沿用 1.2, 接下来的版本是 1.3、 1.4 和 5.0, 但是，Java 平台
被重新命名为 Java 2。因此， 就有了 Java 2 Standard Edition Software Development Kit ( Java 2
标准版软件开发包）的 5.0 版， 即 J2SE SDK 5.0。
幸运的是，2006 年版本号得到简化。Java 标准版的下一个版本取名为 Java SE 6, 后来
又有了 Java SE 7 和 Java SE 8。不过，-“ 内部” 版本号分别是 1.6.0、 1.7.0 和 1.8.0。
当 Oracle 为解决一些紧急问题做出某些微小的版本改变时， 将其称为更新。 例如： Java
SE 8u31 是 JavaSE 8 的第 31 次更新， 它的内部版本号是 1.8.0_31。 更新不需要安装在前一
个版本上， 它会包含整个 JDK 的最新版本。 另外， 并不是所有更新都公开发布， 所以如果
“ 更新 31” 之后没有“ 更新 32”， 你也不用惊慌。
对于 Windows 或 Linux, 需要在 x86 ( 32 位） 和 x64 ( 64 位）版本之间做出选择。 应当
选择与你的操作系统体系结构匹配的版本。
对于 Linux, 还可以在 RPM 文件和 .tar.gz 文件之间做出选择。我们建议使用后者，可以
在你希望的任何位置直接解压缩这个压缩包。
现在你已经了解了如何选择适当的 JDK。下面做一个小结：
•你需要的是 JDK (Java SE 开发包)，而不是 JRE。
•Windows 或 Linux: 32 位选择 x86，64 位以 x64c
•Linux：选择 .tar.gz 版本。
接受许可协议，然后下载文件。
0 注释：Oracle 提供了 一个捆绑包， 其中包含 Java 开发包（JDK) 和 NetBeans 集成开发
环境。 建议现在不要安装任何捆绑包， 而只需安装 Java 开发包。 如果以后你打算使用
NetBeans, 可以再从 http://netbeans.org 下载。
2.1.2
设置 JDK
下载 JDK 之后，需要安装这个开发包并明确要在哪里安装，后面还会需要这个信息。
•在 Windows 上， 启动安装程序。 会询问你要在哪里安装 JDK。 最好不要接受路径名
中包含空格的默认位置， 如 c:\Program Files\Java\jdkl.8.0_verj/o«。 取出路径名中的
Program Files 部分就可以了。
14
Java 核心技?
•在 Mac 上，运行安装程序。 这会把软件安装到 /Libraiy/Java/JavaV_
lMachines/jdkl.8.0_
vers»
_
o«.jdk/Contents/Home。用 Finder 找到这个目录。
•在 Linux 上t 只需要把 .tar.gz 文件解压缩到你选择的某个位置， 如你的主目录， 或者
/opt。 如果从 RPM 文件安装， 则要反复检查是否安装在 /usr/java/jdkl.S.Ojersfofj。
在这本书中， 安装目录用)成表示。例如， 谈到如M)in 目录时， 是指 /opt/jdkl.8.0_31/
bin 或 c:\Java\jdkl _8_0_31\bin 目录。
在 Windows 或 Linux 上安装 JDK 时， 还需要另外完成一个步骤：将如;t/bin 目录增加到
执行路径中一执行路径是操作系统查找可执行文件时所遍历的目录列表。
•在 Linux 上，需要在 7.bashrc 或 7.bash_profile 文件的最后增加这样一行：
export PATH=/d/t/bin:$PATH
一定要使用 JDK 的正确路径， 如 /opt/jdkl.8.0_31。
•在 Windows 上， 启动控制面板， 选择“ 系统与安全’（System and Security)，再选择‘
系
统”（System)，选择高级系统设置 （Advanced System Settings)(参见图 2-1 )。 在系统属
性 （SystemProperties) 对话框中， 点击“ 高级”(Advanced) 标签页，然后点击“ 环境”
( Environment ) 按钮。
%
m
©0-
!
!
Central
、-广
, :
• 冲-一
w的
,„„„„,„1,,, mwnir iwniiBiiimniimiiwriiliwi
%
D
9 Orvxt*
Mao»(i Cocpwition. ABnghl-
«wtt»»n*we<«iaoofWind»irt7
图 2-1
Windows 7中设置系统属性
滚动“ 系统变量”（System Variables) 列表, 直到找到名为 Path 的变量。点击“ 编辑”（Edit)
襄 2 章
Java 程 序 ? ? ? 境
1 5
按钮（参见图 2-2 )。将yWhbin 目录增加到路径最前面， 并用一个分号分隔新增的这一项， 如
下所示：
jdk\bin\other stuff
Syst«n Properties
k aa：
CowPMfeBrWBBw i
j Advanced |Syitem Protection
an Admnstratorto make moat of 1
UserPrafies
DeAtop settings
Startup and Recovoy
Syatan 9tarti4).patent faiue.
1
如
j
! 1
i l
C:\lMndows，
FP_NO_HOSr_C... NO
3U3F_P... 1
□
Windows.
New…
J
Edt-
|
J ,
C-
ncd1
RES9P
Variable
Variable
图 2-2
Windows 7 中设置 Path 环境变量
注意要把_/成替换为具体的 Java 安装路径， 如 c:\Java\jdkL8.0_31。 如果忽视前面的建
议， 想要保留 Program Files 部分， 则要把整个路径用双引号引起来："c:\ProgramFiles\Java\
jdkl.8.0_31\bin"; 其他目录。
保存所做的设置。之后新打开的所有控制台窗口都会有正确的路径。
可以如下测试设置是否正确： 打开一个终端窗口， 键人：
javac -version
然后按回车键。 应该能看到显示以下信息：
javac 1.8.0J1
如果得到诸如“ javac: command not found”(javac:: 命令未找到）或 “The name specified
is not recognized as an internal or external command, operable program or batch file” (指定名不
是一个内部或外部命令、 可执行的程序或批文件 )，就需要退回去反复检査你的安装。
2.1.3
安装库源文件和文档
库源文件在 JDK 中以一个压缩文件 src.Zip 的形式发布， 必须将其解压缩后才能够访问
16
Java 核心技?
源代码。 建议按照下面所述的步骤进行操作。很简单：
1 ) 确保 JDK 已经安装， 并且 jdk/bin 目录在执行路径中。
2 ) 在主目录中建立一个目录 javasrc。 如果愿意，可以在一个终端窗口完成这个步骤。
mkdir javasrc
3 ) 在 jdk 目录下找到文件 src.zip。
4 ) 将 src.zip 文件解压缩到 javasrc 目录。在一个终端窗口中，可以执行以下命令：
cd javasrc
jar xvf yrffc/src.zip
cd . .
Qf 提示：src.zip 文件中包含了所有公共类库的源代码。要想获得更多的源代码（例如： 编
译器、 虚拟机、本地方法以及私有辅助类)， 请访问网站：http://jdk8.java.net。
文档包含在一个压缩文件中，它是一个独立于JDK 的压缩文件。可以直接从网站 http://
www.oracle.com/technetwork/java/javase/downloads 下载这个文档。操作步骤如下：
1 ) 下载文档压缩文件。这个文件名为 jdk-ver5/cw-docs-all.zip, 其 中 的 表 示 版 本
号， 例如 8u31。
2 ) 解压缩这个文件，将 doc 目录重命名为一个更有描述性的名字， 如 javadoc。如果愿
意， 可以从命令行完成这个工作：
jar xvf Downloads/jdk-wrswn-docs-al 1 .zip
mv doc javadoc
这里 version 是相应的版本号。
3 ) 在浏览器中导航到 javadoc/api/index.html, 将这个页面增加到书签。
还要安装本书的程序示例。 可以从 http://horstmann.com/corejava 下载示例。 这些程序
打包在一个 zip 文件 corejava.zip*。可以将程序解压缩到你的主目录。 它们会放在目录
corejava 中。 如果愿意， 可以从命令行完成这个工作：
jar xvf Downloads/corejava.zip
2.2
使用命令行工具
如果在此之前有过使用 Microsoft Visual Studio 等开发环境编程的经验， 你可能会习惯于
有一个内置文本编辑器、 用于编译和启动程序的菜单以及调试工具的系统. JDK 完全没有这
些功能。所有工作都要在终端窗口中键入命令来完成。这听起来很麻烦， 不过确实是一个基
本技能。 第一次安装 Java 时， 你可能希望在安装开发环境之前先检査 Java 的安装是否正确。
另外，通过自己执行基本步骤， 你可以更好地理解开发环境的后台工作。
不过， 掌握了编译和运行 Java 程序的基本步骤之后， 你可能就会希望使用专业的开发环
境。下一节会介绍如何使用开发环境。
首先介绍较难的方法： 从命令行编译并运行 Java 程序。
第 2 章
Java 程 序 ? ? ? 境
1 7
1 ) 打?一
■??端窗口。
2 ) ?入 corejava/vlch02/Welcome目?（CoreJava 是安?本?示例源代?的目?，??
看 2.1.3?)。
3 ) ?人下面的命令：
javac Wei come.java
java Welcome
然后， ??在?端窗口中看到? 2-3 所示的?出。
-$ cd corejava/vlch02/Welcome
-/corejava/vlch02/Welcome$ javac Welcome.java
~/corejava/vlch02/Welcome$ java Welcome
Welcome to Core Java!
~/corejava/vlch02/Welcome$|
图 2-3
编译并运行 Welcome.java
祝?你！ 你已???并?行了第一? Java 程序。
那么， ?才都?行了哪些操作呢？ javac 程序是一? Java ??器。它?文件 Welcome,
java ??成 Welcome.class。java 程序?? Java ??机。??机?行??器放在 class 文件中
的字??。
Welcome 程序非常??。 它只是向控制台?出了一?消息。 你可能想查看程序清? 2-1
的程序代?。（在下一章中， ?解?它是如何工作的d
程序清单 2-1
Welcome/Welcome.java
1 /**
2
* This program displays a greeting for the reader.
3
* Aversion 1.30 2014-02-27
4
* ©author Cay Horstmann
5
V
6 public class Welcome
7 {
s
public static void main(String口 args)
9
{
10
String greeting = "Welcome to Core Java!";
n
System ,out.println(greeting) ;
12
for (int i = 0; i < greeting.lengthO; i++)
13
System.out.print("='')；
14
System.out.printlnO;
is
}
16 }
18
Java 核心技木
在使用可视化开发环境的年代， 许多程序员对于在终端窗口中运行程序已经很生疏了。
常常会出现很多错误，最终导致令人沮丧的结果。
一定要注意以下几点：
•如果手T_输人源程序， 一定要注意大小写。尤其是类名为 Welcome, 而不是 welcome
或 WELCOME。
•编译器需要一个文件名 （Welcomejava), 而运行程序时， 只需要指定类名 （Welcome),
不要带扩展名 .java 或 .class。
•如果看到诸如 Bad command or file name 或 javacxommand not found 这类消息， 就要
返回去反复检查安装是否有问题， 特别是执行路径的设置。
•如果 javac 报告了一个错误， 指出无法找到 Welcome.java， 就应该检查目录中是否存
在这个文件。
在 Linux 环境下， 检查 Welcome.java 是否以正确的大写字母开头。
在 Windows 环境下， 使用命令 dir, 而不要使用图形浏览器工具。有些文本编辑器（特
别是 Notepad) 在每个文件名后面要添加扩展名 .txt。 如果使用 Notepad 编辑 Welcome.java 就
会存为 Welcome.java.txt。 对于默认的 Windows 设置， 浏览器与 Notepad 都隐含 .txt 扩展名，
这是因为这个扩展名属于“ 已知的文件类型”。 此时， 需要重新命名这个文件， 使用命令
ren, 或是另存一次， 为文件名加一对双引号， 如：“ Welcome.java”。
•如果运行程序之后，收到关于 java.lang.NoClassDefFoundError 的错误消息， 就应该仔
细地检查出问题的类的名字。
如果收到关于 welcome ( w 为小写）的错误消息， 就应该重新执行命令：java
Welcome (W 为大写)。 记住， Java 区分大小写。
如果收到有关 Welcome/java 的错误信息， 这说明你错误地键人了 java Welcome,
java, 应该重新执行命令 java Welcome。
•如果键人 java Welcome, 而虚拟机没有找到 Welcome 类， 就应该检查一下是否有人设
置了系统的 CLASSPATH 环境变量（将这个变量设置为全局并不是一个提倡的做法，
然而， Windows 中有些比较差的软件安装程序就是这样做的）。 可以像设置 PATH 环
境变量一样设置 CLASSPATH, 不过这里将删除这个设置。
： 在 http://docs.oracle.com/javase/tutorial/getStarted/cupojava/ 上有一个很好的教程。
其中提到了初学者经常容易犯的一些错误
2.3
使用集成幵发环境
上一节中， 你已经了解了如何从命令行编译和运行一个 Java 程序。 这是一个很有用的技
能， 不过对于大多数日常T.作来说， 都应当使用集成开发环境。这些环境非常强大，也很方
便， 不使用这些环境有些不合情理。 我们可以免费得到一些很棒的开发环境， 如 Eclipse、
NetBeans 和丨ntellLMDEA 程序。 这一章中， 我们将学习如何从 Eclipse 起步。 肖然， 如果你
第 2 章
Java 程 序 ? ? ? 境
1 9
Select a wizard
Create a Java project
Wizards：
|type filter text
喜欢其他开发环境， 学习本书时也完全可以使用你喜欢的环境。
本节将介绍如何使用 Eclipse 编译一个程
序。 Eclipse 是 一 个 可 以 从 网 站 http://eclipse.
org/downloads 上免费下载得到的集成开发环
境。Eclipse 已 经 有 面 向 Linux 、 Mac OS X、
Solaris 和 Windows 的 版 本。 访 问 下 载 网 站
时， 选择“ Eclipse IDE for Java Developers”。
再根据你的操作系统选择 32 位或 64 位版本。
将 Eclipse 解压缩到你选择的位置， 执行
这个 zip 文件中的 eclipse 程序。
下面是用 Eclipse 编写程序的一•般步骤。
1 ) 启动 Eclipse 之后， 从菜单选择 File
—
New —
► Project。
2 ) 从向导对话框中选择 Java Project ( 如
图 2-4 所示）。
3 ) 点击 Next 按钮， 不选中“
Use default location
vlch02/Welcome 目 录 (见 图 2-5 )。
ED
~y\
,Java Project from Existing Ant Buildfile
•
Ptuq-in Project
>
General
> feCVS
> (c> java
! >
Java EE
|>
JavaScript
⑦
|
< Back
或
Next >
j j
Cancel | j
Brush
图 2-4
Eclipse 中的 “ New Project ” 对话框
复选框。点击 Browse 导航到 corejava/
Create a java Profect
Create a Java project in the workspace or in an external location.
project name：|welcomd
□ Use default location
Location：|/home/cay/cor javaA/Ich02/Welcome
B[owse...
rlRE
:
® Use an execution environment JRE：
IjavaSE-l.a
O U$e a project specific JRE-
|jdkl.8,0_25
O Use default JRE (currently 'idkj.8.0_25')
11
Configure IRES.,.
■Project layout—
O Use project folder as root for sources and dass files
® Create separate folders for sources and class files
Configure default...
-Working sets
□ Add project to working sets
Working sets：j
jOil
Setect...~i
① The wizard will automatically configure the jRE and the project layout
based on the existing s
⑦
|
< gack
|[
~~
Next >
J |
Cancel
[ |
finish
图 2-5
配 置 Eclipse 工 程
20
Java 核心技?
4 ) 点击 Finish 按钮。 这个工程已经创建完成了。
5 ) 点击工程窗口左边窗格中的三角，直到找到 Welcome.java 并双击 „ 现在应该看到带
有程序代码的窗口了（如图 2-6 所示）。
ED
Rle Edit Source
：o - ffi -
or Navigate Search Croject Bun Window «elp
eversion 1.39 2614- 02 27
author Cay Horstmann
6
public
■MRE System Library Ijdkl
8
public
main(String[ ]
ng greeting = 'Welcoae to
ea.out.Drintln(areetina )：
Syste*.oot.print("=_
}
System,out.printlnO;
=• n
图 2-6
使 用 Eclipse 编辑源文件
6 ) 用鼠标右键点击最左侧窗格中的工程名（ Welcome), 选 择 Run —>•Run As—»•Java
Application。 程序输出会显示在控制台窗格中。
可以假定，这个程序没有输入错误或 bug (毕竟， 这段代码只有几行 )。为了说明问题，
假定在代码中不小心出现了录人错误（或者甚至语法错误 )。试着将原来的程序修改一下，让
它包含一些录人错误， 例如，将 String 的大小写弄错：
string greeting = "Welcome to Core Java!";
注意 string 下面的波折线。 点击源代码下标签页中的 Problems, 展 开 小 角， 会 看 到 一
个错误消息， 指出有一个未知的 string 类型（见图 2-7 )。点击这个错误消息。 光标会移到编
辑窗口中相应的代码行，可以在这里纠正错误。利用这个特性可以快速地修正错误。
提示： 通常， Eclipse 错误报告会伴有一个灯泡图标。 点击这个图标可以得到一个建议解
决这个错误的方案列表 (
$
Java 程 序 ? ? ? 境
2 1
m
file Edit Source
Refactor fjavigate Search Eroject Bun Window Help
rt• ns•
»
lornck Access |； ©
4Jjava
\i Package Explorer S3
e %
▽ P Welcome
▽ iSB (default package)
A Weicome.java
□
£) Weicome.java J2
«/••
2 •This program displays a greeting for the reader.
3 • version 1.30 2014 92 27
4
* (dauthor Cay Horstmann
5
V
b public class Welcome
□
■
t> a)i|RE System Library [jdkl .l
8-
9
10
11
12
13
14
15
16
17
public static void main(String[ ] args)
<
striiio greeting = "Welcoiie to Core Java!-;
Systen.oi/t.println(greeting)；
for (int i = 0；i < greeting.length( ); i-M-)
Systew.out.print(-=
B );
Systew.oi/t.println( );
ia.Problems E2
A Javadoc
VV Declaration O Console
Description
Resource
▽
Errors (1item)
u string cannot be resolved to a type
Path
Weicome.java /Welcome
Location
1 1
a
B
Type
line 10
Java Pi
图 2-7
Eclipse 中的错误消息
2.4
运行图形化应用程序
Welcome 程序并不会引起人们的兴奋。接下来， 给出一个图形化应用程序。这个程序是
一个简单的图像文件查看器（ viewer )， 它可以加载并显示一个图像。 首先， 由命令行编译并
运行这个程序。
1 ) 打开一个终端窗口。
2 ) 进入 corejava/vlch02/ImageViewer。
3 ) 输入：
javac ImageViewer.java
java ImageViewer
将弹出一个标题栏为 ImageViewer 的新程序窗口 （如
图 2-8 所示）。
现在， 选择 File
—
Open， 然后找到一个图像文件并
打开它 （我们在同一个目录下提供了两个示例文件）。要
关闭这一程序， 只需要点击标题栏中的关闭按钮或者从
f i l e
菜单中选择 File
—
Exit。
阁 2-8
运行 ImageViewer 应用程序
22
Java 核心技?
下面快速地浏览一下源代码 （程序清单 2-2 )。这个程序比第一个程序要长很多， 但是
只要想一想用 C 或 C++ 编写同样功能的应用程序所需要的代码量， 就不会觉得它太复杂了。
木书将在第 10 章 〜 第 12 章介绍如何编写像这样的图形化应用程序。
程序清单 2-2
ImageViewer/lmageViewer.java
1 import java.awt.*;
2 import java.io.*;
3 import javax.swing.*;
4
5 /**
6
* A program for viewing images.
7
* ©version 1.30 2014-02-27
8
*
author Cay Horstmann
9
*/
IO public class ImageViewer
{
12
public static void main(String口 args)
13
{
14
EventQueue.invokeLater(() -> {
is
JFrame frame = new ImageViewerFrame();
16
frame.setTitie("ImageVi ewer");
17
frame.setDefaultCloseOperati on(]Frame.EXIT_0N_CL0SE);
18
frame.setVisible(true) ;
19
});
20
}
21 }
22
23 /**
24
* A frame with a label to show an image.
25
*/
26 class ImageViewerFrame extends ]Frame
27 {
28
private ]Label label ;
29
private JFileChooser chooser;
30
private static final int DEFAULT_WIDTH = 300;
31
private static final int DEFAULTJEIGHT = 400;
32
33
public ImageVi ewe「Frame()
34
{
35
setSize(DEFAULT_WIDTH, DEFAULT.HEIGHT) ;
36
37
// use a label to display the images
38
label = new ]Label ();
39
add(label);
40
41
// set up the file chooser
42
chooser = new ]FileChooser() ;
43
chooser.setCurrentDirectory(new File("."));
44
45
// set up the menu bar
46
]MenuBar menuBar = new ]MenuBar();
47
set]MenuBar(menuBar) ;
48
49
JMenu menu = new ]Menu("File");
so
menuBar.add(menu);
第
■? 章
Java 程序???境
23
JMenuItem openltem = new 3MenuItem("0pen");
menu.add(openltem);
openltem.addActionListener(event -> {
// show file chooser dialog
int result = chooser.shomOpenDIalog(null):
// if file selected, set it as icon of the label
if (result
JFileChooser.APPROVE OPTION)
{
String name = chooser.getSelectedFileO.getPathO:
label.setIcon(new Imagelcon(name));
}
})；
JMenuItem exitltem = new ]MenuItem("Exit")；
menu.add(exitltem);
exititem.addActionListener(event -> System.exit(0));
2.5
构建并运行 applet
本书给出的前两个程序是 Java 应用程序。它们与所有本地程序一样， 是独立的程序。 然
而， 正如第 1 章提到的， 有关 Java 的大量宣传都在炫耀 Java 在浏览器中运行 applet 的能力。
如果你对“ 过去的记忆” 感兴趣， 可以继续阅读下面的内容来了解如何构建和运行一个
applet, 以及如何在 Web 浏览器中显示；如果你不感兴趣， 完全可以跳过这个例子，直接转
到第 3 章。
首先， 打开终端窗口并转到 CoreJava/vlch02/RoadApplet，
然后，输入下面的命令：
javac RoadApplet.java
jar cvfm RoadApplet.jar RoadApplet.mf *.dass
appletviewer RoadApplet.htnil
图 2-9 显示了在 applet 查看器窗口中显示的内容。
这个 applet 图示显示了司机随意减速可能导致交通拥堵
的情况。1996 年，applet 是创建这种可视化显示的绝佳
工具。
第一条命令是大家已经非常熟悉的调用 Java 编译器
的命令。它将 RoadApplet.java 源文件编译成字节码文件
RoadApplet.classo
不过这一次不要运行 java 程序。 首先， 使用jar
工具将类文件打包到一个“
MR 文件”。 然后调用
appletviewer 程序， 这是 JDK 自带的一个工具， 可以用
Slowdown Q==>
Applet started.
图 2-9
在 applet 查看器窗口中显示的
RoadApplet
24
Java 核心技?
来快速测试 applet
需要为这个程序指定一个 HTML 文件名， 而不是一个 Java 类文件名。
RoadApplet.html 文件的内容如本节最后的程序清单 2-3 所示。
程序清单 2-3
RoadApplet/RoadApplet.html
1 <html xmlns="http://www.w3.org/1999/xhtml">
2
<headxtitle>A Traffic Simulator Applet</titlex/head>
3
<body>
4
<hl>Traffic Simulator Applet</hl>
5
6
<p>I wrote this traffic simulation, following the article &quot;Und nun die
7
Stauvorhersage&quot; of the German Magazine <i>Die Zeit</i>, June 7,
8
1996. The article describes the work of Professor Michael Schreckenberger
9
of the University of Duisburg and unnamed collaborators at the University
10
of Cologne and Los Alamos National Laboratory. These researchers model
11
traffic flow according to simple rules, such as the following: </p>
12
<ul>
13
<li>A freeway is modeled as a sequence of grid points. </li>
14
<li>Every car occupies one grid point. Each grid point occupies at most
is
one car. </li>
16
<li>A car can have a speed of 0 - 5 grid points per time interval. </li>
17
<li>A car with speed of less than 5 increases its speed by one unit in
18
each time interval , until it reaches the maximum speed. </li>
19
<li>If a car' s distance to the car in front is <i>d</i> grid points, its
20
speed is reduced to <i>d</i>-lif necessary to avoid crashing into it.
21
</li >
22
<li>With a certain probability, in each time interval some cars slow down
23
one unit for no good reason whatsoever. </li>
24
</ul>
25
26
<p>This applet models these rules. Each line shows an image of the same
27
stretch of road. Each square denotes one car. The first scrollbar lets you
28
adjust the probability that some cars slow down. If the slider is all the
29
way to the left, no car slows down. If it is all the way to the right,
30
every car slows down one unit. A typical setting is that 10% - 20% of the
31
cars slow down. The second slider controls the arrival rate of the cars.
32
When it is all the way to the left, no new cars enter the freeway. If it
33
is all the way to the right, a new car enters the freeway every time
34
interval , provided the freeway entrance is not blocked. </p>
35
36
<p>Try out the following experiments. Decrease the probability of slowdown
37
to 0. Crank up the arrival rate to 1. That means, every time unit, a new
38
car enters the road. Note how the road can carry this load. </p>
39
40
<p>Now increase the probability that some cars slow down. Note how traffic
41
jams occur almost immediately. </p>
42
43
<p>The moral is: If it wasn't for the rubberneckers, the cellular phone
44
users, and the makeup-appliers who can't keep up a constant speed, we'd all
45
get to work more quickly. </p>
46
47
<p>Notice how the traffic jam is stationary or even moves backwards, even
48
though the individual cars are still moving. In fact, the first car
49
causing the jam has long left the scene by the time the jam gets bad.
so
(To make it easier to track cars, every tenth vehicle is colored red.) </p>
si
•
2t
Java 程 序 ? ? ? 境
2 5
52
〈pxapplet code="RoadApplet.cl ass" archi ve="RoadApplet.jar"
53
width="400
M height="400" alt="Traffic jam visualization%
54
</appletx/p>
ss
56
<p>For more information about applets, graphics programming and
57
multithreading in Java, see
ss
<a href="http://horstmann.c0m/corejava%C0re ]ava</a>. </p>
ss
</body>
60
</html >
如果熟悉 HTML， 你会注意这里的标准 HTML 标记和 applet 标签，这会告诉 applet 查看
器加载 applet, 其代码存储在 RoadApplet.jai
D applet 会忽略除 applet 标签外的所有 HTML
标签。
当然， applet 要在浏览器中查看。遗憾的是， 现在很多浏览器并不提供 Java 支持， 或者
启用 Java 很困难。对此，最好使用 Firefox。
如果使用 Windows 或 Mac OS X, Firefox 会自动启用计算机上安装的 Java。在 Linux 上，
则需要用下面的命令启用这个插件：
mkdir -p /.mozi11a/plugins
cd ~/.mozilla/plugins
In -s jdk/]re/1ib/amd64/1ibnpjp2.so
作为检查，可以在地址栏键人 about:plugins， 查找 Java 插件。确保使用这个插件的 Java
SE 8 版本， 为此要查找 MIME 类型 application/x-java-applet;version=l.8。
接下来， 将浏览器导航到 http://horstmann.com/applets/RoadApplet/RoadApplet.html， 对
所有安全提示都选择接受， 保证最后会显示 applet。
遗憾的是， 只是测试刚刚编译的 applet 还不够。horstmann.com 服务器上的 applet 有数字
签名。还必须再花一些工夫，让 Java 虚拟机信任的一个证书发行者信任我， 为我提供一个证
书，我再用这个证书为 JAR 文件签名。浏览器插件不再运行不信任的 applet。与过去相比，
这是一个很大的变化， 原先在屏幕上绘制像素的简单 applet 会限制在“ 沙箱” 中， 即使没有
签名也可以工作。可惜，即使是 Oracle 也不再相信沙箱的安全性了。
为了解决这个问题， 可以临时将 Java 配置为信任本地文件系统的 applet。 首先， 打开
Java 控制面板。
•在 Windows 中，査看控制面板中的 Programs ( 程序）部分。
•在 Mac 上，打开 System Preferences ( 系统首选项）。
•在 Linux 上，运行 jcontrol。
然后点击 Security ( 安全）标签页和 Edit Site List ( 编辑网站列表）按钮。再点击 Add (增
加)，并键人 file:///。点击 OK， 接受下一个安全提示，然后再次点击 OK ( 见图 2-10 )。
现在应该可以在浏览器中加载文件 corejava/v1ch02/RoadApplet/RoadApplet.html, applet
将随周围的文本一同显示。结果如图 2-11 所示。
最后， 在程序清单 2-4 中给出了这个 applet 类的代码。现在， 只需要简单看一下。在第
26
Java 核心技?
13 章中， 还会再来介绍 applet 的编写。
norol
lava
Security
Advanced
iEl Enable j iva content in the browser
Security level for applications not on the Exception Site list
Very High- Only Java applications identified by a certificate from a
trusted authority are allowed to run, and onfy if the certificate can be
verified a& not revoked.
High-Java applications identified by a certificate from a trusted
ED
authority are allowed to run, even if the revocation status of the
certificate cannot be verified.
Exception Site List
ications laur
the appropriate security prompts.
Applications launched from thesites listed below will be allowed to
after
L-
tdrt %,teL,st M. _
I
i. aesto,e Securit» PromP,s a
Manage tertiticates... |
ED
Applications launched from the sites listed below will be allowed to run after the appropriate security
prompts.
Location
ile：///
gemovi* |
FILE and HTTP protocols arc considered a security risk. We recommend using HTT
sitesTwhere available.
OK
I i
i
Including a FILE Location on the Exception Site
List is considered a security risk
丄
Location: file：//
locations that use FILE protocol are a security risk and may
compromise the personal information on your computer. We
recommend including only HTTPS sites on the Exception Site Lis*.
Click Continue to accept this location or Cancel to abort this
change.
\
Continue
(
Cancel
[
图 2-10
配置 Java 信任本地 applet
: I I 4 f1le：///data/cay/books/cjl0/code/vlch02/RoadApplet/RoadApplet.html
v e :
SD
© =
Try out the following experiments. Decrease the probability of slowdown to 0. Crank up the
arrival rate to 1. That means, every time unit, a new car enters the road. Note how the road can
carry this load.
Now increase the probability that some cars slow down. Note how traffic jams occur almost
immediately.
The moral is：If it wasn't for the rubberneckers, the cellular phone users, and the makeup-appliers
who can't keep up a constant speed, we'd all get to work more quickly.
Notice how the traffic jam is stationary or even moves backwards, even though the individual cars
are still moving. In fact, the first car causing thejam has long left the scene by the time thejam
gets bad. (To make it easier to track cars, every tenth vehicle is colored red.)
Slowdown
C：x
P諮::淡雙編纖
For more information about applets, graphics programming and multithreading inJava, see Core
Java-
图 2-11
在浏览器中运彳』
•RoadApplet
第 2 章
Java 程 序 ? ? ? 境
2 7
程序清单 2-4
RoadApplet/RoadApp丨et.java
1 import java. awt .*;
2 import java.applet .*;
3
import javax. swing.*;
4
public class RoadApplet extends JApplet
{
private RoadComponent roadComponent;
private ]Slider slowdown;
private JSlider arrival ;
public void init()
{
EventQueue.invokeLater(() ->
{
roadComponent = new RoadComponent() ;
slowdown = new ]Slider(0, 100, 10);
arrival = new ]Slider(0, 100, 50);
]Panel p = new JPanel ();
p.setLayout(new GridLayout(l, 6)) ;
p _ add(new ]Label ("Slowdown"));
p. add(slowdown) ;
p.add(new JLabel ("")) ;
p.add(new ]Label ("Arrival"))；
p.add(arrival) ;
p.add(new JLabel (""))；
setLayout(new BorderLayoutO) ;
add(p, BorderLayout.NORTH);
add(roadComponent, BorderLayout.CENTER);
})；
public void start()
{
new Thread(() ->
{
for (;;)
{
roadComponent .update(
0.01 * slowdown.getValueO ,
0.01 * arrival .getValueO)；
try { Thread.sleep(50); } catch(InterruptedException e) {}
}
}) _ start();
在本章中， 我们学习了有关编译和运行 Java 程序的机制。现在可以转到第 3 章开始学习
Java 语言了。