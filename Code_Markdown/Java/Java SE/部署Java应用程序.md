第 1 3 章 部 署 Java 应用程序
▲ JAR 文件
▲ applet
▲ 应用首选项的存储
▲ Java Web Start
▲ 服务加载器
到目前为止， 我们已经能够熟练地使用 Java 程序语言的大部分特性，并且对 Java 图形
编程的基本知识也有所了解。 现在准备创建提交给用户的应用程序， 至此需要知道如何将这
些应用程序进行打包， 以便部署到用户的计算机上。传统的部署方式是使用 applet, 这应该
归功于在 Java 出现的最初几年中对其给予的大肆吹捧。applet 是一种特殊的 Java 程序， 它允
许通过网络下载，并可以在浏览器中运行。其目的在于让用户不再为安装软件烦恼，并且可
以通过支持 Java 的计算机或者其他具有 Internet 连接的设备使用这些软件。
由于种种原因，applet 并没有实现上述目标。因此， 本章首先介绍打包应用的指令。然
后展示应用如何存储配置信息和用户首选项。 另外还会学习如何使用 ServiceLoader 类在应用
中加载插件。
接下来， 我们再来讨论 applet, 介绍创建或维护 applet 时需要了解的有关知识。我们还
会讨论 Java
Start 机制
这是一种基于 Internet 的应用发布方法，很多方面都与 applet
很类似，不过更适合不在 Web 页面中的程序。
13.1
JAR 文件
在将应用程序进行打包时， 使用者一定希望仅提供给其一个单独的文件， 而不是一个含
有大量类文件的目录，Java 归档（JAR) 文件就是为此目的而设计的。一个 JAR 文件既可以
包含类文件，也可以包含诸如图像和声音这些其他类型的文件。此外， JAR 文件是压缩的，
它使用了大家熟悉的 ZIP 压缩格式。
提示：pack200 是一种较通常的 ZIP 压缩算法更加有效的压缩类文件的方式。Oracle 声
称， 对类文件的压缩率接近 90%。 有关更加详细的信息请参看网站 http://doc.oracle.com/
javase/1.5.0/docs/guide/deployment/deployment-guide/pack200.htmlo
13.1.1
创建 JAR 文件
可以使用 jar 工具制作 JAR 文件（在默认的 JDK 安装中， 位于 jdk/bin 目录下)。创建一
个新的 JAR 文件应该使用的常见命令格式为：
$ 13t
部署 Java&用程序
581
jar cvf JARFileNameFile：File2 . . .
例如：
jar cvf CalculatorClasses.jar'
class icon.gif
通常，jar 命令的格式如下：
jar options File File2 . . .
表 13-1 列出了所有 jar 程序的可选项。它们类似于 UNIX tar 命令的选项。
表 13-1
jai•程序选项
选项
说
明
~
c
创建一个新的或者空的存档文件并加人文件。 如果指定的文件名是目录，jai•程序将会对它们
进行递归处理
C
暂时改变 H 录，例如：
jar cvf JARFileName.jar -C cl asses *.class
改变 classes 子目录， 以便增加这些类文件
e
在清单文件中创建一个条目（请参看 13.1.3 节）
f
将 JAR 文件名指定为第二个命令行参数。 如果没有这个参数，jar 命令会将结果写到标准输出
上（在创建 JAR 文件时）或者从标准输入中读取它 （在解压或者列出 JAR 文件内容时）
i
建立索引文件（用于加快对大型归档的查找）
m
将一个清单文件 （ manifest ) 添加到 JAR 文件中。清单是对存杓内容和来源的说明每个归档
有一个默认的清单文件。但是， 如果想验证归杓文件的内容， 可以提供自己的清单文件
M
不为条0创建清笮文件
t
显示内容表
u
更新一个已有的 JAR 文件
v
生成详细的输出结果
x
解作:文件。 如果提供一个或多个文件名， 只解压这些文件； 否则， 解压所有文件
o
存储’ 不进行 ZIP m缩
可以将应用程序、 程序组件 （有时称为“
beans”—参见卷 II 第 11 章） 以及代码库打
包在 JAR 文件中。 例如， JDK 的运行时库包含在一个非常庞大的文件 rt.jar 中。
1 3 . 1.2
清 单 文 件
除了类文件、 图像和其他资源外， 每个 JAR 文件还包含一个用于描述归档特征的清单文
件 （manifest )。
清单文件被命名为 MANIFEST.MF, 它位于 JAR 文件的一个特殊 META-INF 子目录中。
最小的符合标准的清单文件是很简单的：
Manifest-Version: 1.0
复杂的清单文件可能包含更多条目。 这些清单条目被分成多个节。 第一节被称为主节
( main section ) 0 它作用于整个 JAR 文件。随后的条目用来指定已命名条目的属性，这些已命
名的条目可以是某个文件、 包或者 URL。它们都必须起始于名为 Name 的条目。 节与节之间
582
Java 核心技?
用空行分开。例如：
Manifest-Version: 1.0
描述这个归档文件的行
Name: Woozle.class
描述这个文件的行
Name: cora/mycompany/mypkg/
描述这个包的行
要想编辑清单文件， 需要将希望添加到清单文件中的行放到文本文件中，然后运行：
jar cfm JARFileNameManifestFileName . . .
例如， 要创建一个包含清单的 JAR 文件，应该运行：
jar cfm MyArchive.jar manifest.mf com/mycompany/mypkg/*.class
要想更新一个已有的 JAR 文件的清单，则需要将增加的部分放置到一个文本文件中，然
后执行下列命令：
jar ufin MyArchive.jar manifest-additions.mf
0 注释： 请参看 http://docs.orade.com/javase/8/docs/technotes/guides/jar 获得有关 JAR 文件
和清单文件格式的更多信息。
13.1.3
可执行 JAR 文件
可以使用jar 命令中的 e 选项指定程序的人口点， 即通常需要在调用java 程序加载器时
指定的类：
jar cvfe HyPrograni.jar com.myconipany.iiiypkg.MainAppClass files to add
或者， 可以在清单中指定应用程序的主类， 包括以下形式的语句：
Main-C1ass: com.nycompany.mypkg.MainAppClass
不要将扩展名 .class 添加到主类名中。
警告： 清单文件的最后一行必须以换行符结束。 否则， 清单文件将无法被正确地读取。
常见的错误是创建了一个只包含 Main-Class 而没有行结束符的文本文件。
不论哪一种方法，用户可以简单地通过下面命令来启动应用程序：
java -jar MyProgram.jar
根据操作系统的配置， 用户甚至可以通过双击 JAR 文件图标来启动应用程序。下面是各
种操作系统的操作方式：
•在 Windows 平台中，Java 运行时安装器将建立一个扩展名为 jar 的文件与 javaw -jar
命令相关联来启动文件（与java 命令不同，javaw 命令不打开 shell 窗口）。
•在 Solaris 平台中， 操作系统能够识别 JAR 文件的“ 魔法数” 格式， 并用java -jar 命
令启动它。
$13
部署 Java ?用程序
583
•在 Mac OS X 平台中， 操作系统能够识别 .jar 扩展名文件。 当双击 JAR 文件时就会执
行 Java 程序可以运行。
无论怎样， 人们对 JAR 文件中的 Java 程序与本地文件有着不同的感觉。在 Windows 平
台中， 可以使用第三方的包装器工具将 JAR 文件转换成 Windows 可执行文件。包装器是一
个大家熟知的扩展名为 .exe 的 Windows 程序，它可以查找和加载 Java 虚拟机（JVM)， 或者
在没有找到 JVM 时告诉用户应该做些什么。有许多商业的和开源的产品， 例如， LaunCh4J
(http://launch4j.sourceforge.net) 和 IzPack (http://izpack.org)。
在 Macintosh 平台中，这种情形处理起来会容易一些。Jar Bundler 工具是 XCode 的一部
分，可以将 JAR 文件转换为一个一流的 Mac 应用。
13.1.4
资源
在 applet 和应用程序中使用的类通常需要使用一些相关的数据文件， 例如:
•图像和声音文件。
•带有消息字符串和按钮标签的文本文件。
•二进制数据文件， 例如， 描述地图布局的文件。
在 Java 中，这些关联的文件被称为资源 （resource)。
K1 注释： 在 Windows 中， 术语“ 资源” 有着更加特殊的含义。Windows 资源也是由图像、
按钮标签等组成， 但是它们都附属于可执行文件， 并通过标准的程序设计访问。相比之
下，Java 资源作为单独的文件存储， 并不是作为类文件的一部分存储。 对资源的访问和
解释由每个程序自己完成。
例如， AboutPanel 类显示了一条信息， 如图 13-1 所示。
当然，在面板中的书名和版权年限将会在出版下一版
图书时发生变化。为了易于追踪这个变化， 希望将文本放
置在一个文件中，而不是以字符串的形式硬写到代码中。
但是应该将 about.txt 这样的文件放在哪儿呢？ 显然，
将它与其他程序文件一起放在 JAR 文件中是最方便的。
类加载器知道如何搜索类文件，直到在类路径、 存档
文件或 web 服务器上找到为止。利用资源机制， 对于非类
文件也可以同样方便地进行操作。下面是必要的步骤：
.ore java
LOth Edit
图 13-丨显示来自 JAR 文件的资源
1 ) 获得具有资源的 Class 对象， 例如， AboutPanel.class。
2 ) 如果资源是一个图像或声音文件， 那么就需要调用 getresource (filename) 获得作为
URL 的资源位置，然后利用 getlmage 或 getAudioClip 方法进行读取。
3 ) 与图像或声音文件不同，其他资源可以使用 getResourceAsStream 方法读取文件中的
数据。
重点在于类加载器可以记住如何定位类，然后在同一位置査找关联的资源。
584
Java 核心技?
例如，要想利用 about.gif 图像文件制作图标，可以使用下列代码：
URL url = ResourceTest.class.getResource("about.gif");
Image img = new Imagelcon(url).getlmage():
这段代码的含义是“ 在找到 ResourceTest 类的地方查找 about.gif 文件”。
要想读取 about.txt 文件，可以使用下列命令：
I叩utStrearo stream = ResourceTest.class.getResourceAsStreamrabout.txt");
Scanner in = new Scanner(streara，"UTF-8n):
除了可以将资源文件与类文件放在同一个目录中外，还可以将它放在子目录中。可以使
用下面所示的层级资源名：
data/text/about.txt
这是一个相对的资源名， 它会被解释为相对于加载这个资源的类所在的包。注意， 必须
使用“ /” 作为分隔符，而不要理睬存储资源文件的系统实际使用哪种目录分隔符。 例如， 在
Windows 文件系统中， 资源加载器会自动地将“ /” 转换成
一个以“
/” 开头的资源名被称为绝对资源名。它的定位方式与类在包中的定位方式一
样。 例如，资源
/corejava/title.txt
定位于 corejava 目录下（它可能是类路径的一个子目录，也可能位于 JAR 文件中， 对 applet
来说在 web 服务器上)。
文件的自动装载是利用资源加载特性完成的。 没有标准的方法来解释资源文件的内容。
每个程序必须拥有解释资源文件的方法。
另一个经常使用资源的地方是程序的国际化。 与语言相关的字符串， 如消息和用户界面
标签都存放在资源文件中， 每种语言对应一个文件。 国际化 API ( internationalizationAPI ) 将
在卷 II 的第 5 章中进行讨论。这些 API 提供了组织和访问本地化文件的标准方法。
程序清单 13-1 显示了这个程序的源代码。 这个程序演示了资源加载。 编译、 创建 JAR
文件和执行这个程序的命令是：
javac resource/ResourceTest.java
jar cvfm ResourceTest.jar resource/ResourceTest.mf resource/*.class resource/*.gif resource/*.txt
java -jar ResourceTest.jar
将 JAR 文件移到另外一个不同的目录中，再运行它， 以便确认程序是从 JAR 文件中而
不是从当前目录中读取的资源。
程序清单 13-1
resource/ResourceTest.java
1 package resource;
2
3 import java.awt.*;
4 import java.io.*;
5 import java.net.*;
6 import java.util.*;
7 import javax.swing.*;
第 /J 章 部 署 Java ? 用 程 序
5 8 5
9 /**
10
* ©version 1.41 2015-06-12
11
*
author Cay Horstmann
12
V
13 public class ResourceTest
w {
is
public static void main(String ] args)
EventQueue.invokeLater(() -> {
]Frame frame = new ResourceTestFrameO;
frame.setTitl e("ResourceTest");
frame.setDefaultCloseOperati on(]Frame.EXIT_0N_CL0SE);
frame _setVisi ble(true);
})；
25
26 /**
27
* A frame that loads image and text resources.
28
V
29 cl ass ResourceTestFrame extends ]Frame
30 {
31
private static final int DEFAULT_WIDTH = 300;
32
private static final int DEFAULTJEIGHT = 300;
18
19
20
21
34
public ResourceTestFrame()
35
{
36
setSize(DEFAULT_WIDTH, DEFAULTJEIGHT) ;
37
URL aboutURL = getClassO.getResource("about.gif’）；
38
Image img = new Imagelcon(aboutURL).getlmageO:
39
setlconlmage(img) ;
40
41
JTextArea textArea = new ]TextA「ea();
42
InputStream stream = getClassO.getResourceAsStreamCabout.txt"):
43
try (Scanner in = new Scanner(stream, "UTF-8"))
44
{
4s
while (in.hasNextO)
46
textArea.append(in.nextLine() + "\n");
<7
}
48
add(textArea) ;
49
}
so }
爲PI| java.iang.Class 1.0
•
URL getResource(String name) 1.1
• InputStream getResourceAsStream(String name) 1.1
找到与类位于同一位置的资源， 返回一个可以加载资源的 URL 或者输人流。 如果没
有找到资源， 则返回 null, 而且不会抛出异常或者发生 I/O 错误。
13.1.5
密封
在第 4 章曾经提到过， 可以将 Java 包密封 （ seal ) 以保证不会有其他的类加人到其中。
586
Java 核心技?
如果在代码中使用了包可见的类、方法和域，就可能希望密封包。如果不密封， 其他类就有
可能放在这个包中，进而访问包可见的特性。
例如， 如果密封了 com.mycompany.util 包， 就不能用下面的语句顶替密封包之外的类：
package com.mycompany.util ;
要想密封一个包，需要将包中的所有类放到一个 JAR 文件中。在默认情况下，JAR 文件
中的包是没有密封的。可以在清单文件的主节中加人下面一行：
Sealed: true
来改变全局的默认设定。对于每个单独的包，可以通过在 JAR 文件的清单中增加一节， 来指
定是否想要密封这个包。例如：
Name: com/mycoinpany/util/
Sealed: true
Name: com/myconpany/misc/
Sealed: false
要想密封一个包，需要创建一个包含清单指令的文本文件。然后用常规的方式运行 jar
命令：
jar cvfw MyArchive.jar manifest.mf files to add
13.2
应用首选项的存储
应用用户通常希望能保存他们的首选项和定制信息， 以后再次启动应用时再恢复这些配
置。首先我们来介绍 Java 应用的传统做法， 这是一种简单的方法，将配置信息保存在属性文
件中。然后我们会介绍首选项 API, 它提供了一个更加健壮的解决方案。
13.2.1
属性映射
属性映射 （ property map) 是一种存储键 / 值对的数据结构。属性映射通常用来存储配置
信息，它有 3 个特性：
•键和值是字符串。
•映射可以很容易地存人文件以及从文件加载。
•有一个二级表保存默认值。
实现属性映射的 Java 类名为 Properties。
属性映射对于指定程序的配置选项很有用。 例如：
Properties settings - new Properties。
;
settings.setProperty("width", "200");
settings.setProperty("title", "Hello, World!");
可以使用 store 方法将属性映射列表保存到一个文件中。 在这里， 我们将属性映射保存
在文件 program.properties 中。 第二个参数是包含在这个文件中的注释。
第 1 3 章 部 署 Java ? 用 程 序
5 8 7
OutputStream out = new FileOutputStream("program.properties");
settings.store(out, "Program Properties");
这个示例会给出以下输出：
IProgran Properties
#Mon Apr 30 07:22:52
2007
width=200
title=Hello, World!
要从文件加载属性，可以使用以下调用：
I叩utStrean in = new Filei叩utStream("prograni.properties");
settings,load(in);
习惯上， 会把程序属性存储在用户主目录的一个子目录中。目录名通常以一个点号开头
(在 UNIX 系统中)， 这个约定说明这是一个对用户隐藏的系统目录。我们的示例程序就遵循
这个约定。
要找出用户的主目录，可以调用 System.getProperties 方法，它恰好也使用一个 Properties
对象描述系统信息。主目录包含键 "usenhome"。还有一个便利方法可以读取单个键：
String userDir = System.getPropertyC'user.home")；
可以为程序属性提供默认值， 这是一个很好的想法， 因为用户有可能手动编辑这个文
件。Properties 类有两种提供默认值的机制。 第一种方法是， 查找一个字符串的值时可以指定
一个默认值，这样当键不存在时就会自动使用这个默认值。
String title = settings.getProperty("title", "Default title");
如果属性映射中有一个 "title" 属性， title 就会设置为相应的字符串。否则，title 会设置
为 "Default title"o
如果觉得在每个 getProperty 调用中指定默认值太过麻烦， 可以把所有默认值都放在一个
二级属性映射中，并在主属性映射的构造器中提供这个二级映射。
Properties defaultSettings = new PropertiesO；
defaultSettings.setPropertyC'ividth", "300");
defaultSettings.setProperty("height", "200");
defaultSettings.setPropertyftitie", "Default title");
Properties settings = new Properties(defaultSettings);
没错， 如果为 defaultSettings 构造器提供另一个属性映射参数， 甚至可以为默认值指定
默认值， 不过一般不会这么做。
程序清单 13-2 显示了如何使用属性来存储和加载程序状态。程序会记住框架位置、 大小
和标题。也可以手动编辑主目录中的文件 .corejava/program.properties 把程序的外观改成你希
望的样子。
令警告： 出于历史上的原因， Properties 类实现了 Map<Object,Object〉
。 因此， 可以使用
Map 接口的 get 和 put 方法。 不过，get 方法会返回类型 Object,
而 put 方法允许插入任何对
象。 最好坚持使用 getProperty 和 setProperty 方法，这些方法会处理字符串， 而不是对象。
588
Java 核心技?
El 注释： 属性映射是没有层次结构的简单表。 通常会用类似 window.main.color、 window,
main.title 等键名引入一个伪层次结构 Q 不过 Properties 类没有提供方法来组织这样一个
层次结构。 如果存储复杂的配置信息， 就应当使用 Preferences 类， 请看下一节。
程序清单 13-2
properties/PropertiesTest.java
1 package properties;
2
3 import java.awt.EventQueue;
4 import java.awt.event.*;
5 import java.io.*;
6 import java.util.Properties;
7
8 import javax.swing.*;
9
10 /**
* A program to test properties. The program remembers the frame position, size,
i2
* and title.
is
* Aversion 1.01 2015-06-16
14
*
author Cay Horstmann
15
*/
16 public class'PropertiesTest
17 {
18
public static void main(String[] args)
EventQueue.invokeLater(() -> {
PropertiesFrame frame = new PropertiesFrame();
frame.setVisibleftrue);
})；
27
/**
28
* A frame that restores position and size from a properties file and updates
29
* the properties upon exit.
30
*/
31 class PropertiesFrame extends ]Frame
32
{
33
private static final int DEFAULT.WIDTH = 300;
34
private static final int DEFAULT.HEICHT = 200;
35
36
private File propertiesFile;
37
private Properties settings;
38
39
public PropertiesFrame()
40
{
41
// get position, size, title from properties
42
43
String userOir = System,getProperty("user,home")；
44
File propertiesDir = new File(userDir, ".corejava");
4s
if (!propertiesDir.exists0) propertiesDir.mkdirO;
46
propertiesFile = new File(propertiesDir, "program,properties");
20
21
22
Properties defaultSettings = new Properties();
defaultSettings.setPropertyC'left", "0")；
defaultSettings.setPropertyC'top", "0")；
M 13t
部署 Java ?用程序
589
100
101
102 }
defaultSettings.setPropertyC'width", "" + DEFAULTJIDTH);
defaultSettings.setProperty("height", "" + DEFAULTJEIGHT);
defaultSettings.setPropertyC'title", ""):
settings = new Properties(defaultSettings);
if (propertiesFile.exists())
try (InputStream in = new Fi1eInputStream(propertiesFi1e))
{
settings,load(in);
}
catch (IOException ex)
{
ex.printStackTrace();
int left = Integer.parselnt(settings.getProperty("1eft"));
int top = Integer.parselnt(settings.getPropertyC'top"))；
int width = Integer,parselnt(settings.getPropertyC'width"))；
int height = Integer.parselnt(settings.getProperty("height"))；
setBounds(left, top, width, height);
// if no title given, ask user
String title = settings.getProperty("title");
if (title.equalsC"'))
'
title = ]0ptionPane.showInputDialog("PIease supply a frame title:");
if (title == null) title =
setTitie(title);
addWindowLi stener(new WindowAdapterO
{
public void windowClosing(WindowEvent event)
{
settings.setPropertyC'left", "" + getXQ)；
settings.setProperty("top", "" + getYO);
settings.setProperty("width", "" + getWidthO);
settings.setPropertyC'height", "" + getHeightO);
settings.setPropertyC'titie", getTitle());
try (OutputStream out = new Fi1eOutputStream(propertiesFi1e))
{
settings.store(out, "Program Properties");
}
catch (IOException ex)
{
ex.printStackTraceO;
}
System.exit(0);
}
})；
[AW] java.util.Properties 1.0
• P r o p e r t i e s( )
590
Java 核心技?
创建一个空属性映射。
•P r o p e r t i e s( P r o p e r t i e s d e f a u l t s )
用一组默认值创建一个空属性映射。
参数：defaults
用于查找的默认值。
•S t r i n g g e t P r o p e r t y( S t r i n g k e y)
获得一个属性。返回与键（ key) 关联的值， 或者如果这个键未在表中出现， 则返回默
认值表中与这个键关联的值， 或者如果键在默认值表中也未出现， 则返回 null。
参数：key
要获得相关字符串的键。
•S t r i n g g e t P r o p e r t y( S t r i n g key, S t r i n g d e f a u l t V a l u e )
如果键未找到， 获得有默认值的属性。返回与键关联的字符串， 或者如果键在表中未
出现， 则返回默认字符串。
参数： key
要获得相关字符串的键
defaultValue
键未找到时返回的字符串
•O b j e c t s e t P r o p e r t y( S t r i n g key, S t r i n g v a l u e )
设置一个属性。返回给定键之前设置的值。
参数： key
要设置相关字符串的键
value
与键关联的值
•v o i d 1 o a d( I n p u t S t r e a m i n ) t h r o w s IOException
从一个输入流加载一个属性映射。
参数： in
输入流
•v o i d s t o r e( O u t p u t S t r e a m out , S t r i ng h e a d e r ) 1.2
将一个属性映射保存到一个输出流。
参数： out
输出流
header
存储文件第一行的标题
[API|java.lang.System 1.0
•P r o p e r t i e s g e t P r o p e r t i e s( )
获取所有系统属性。应用必须有权限获取所有属性， 否则会拋出一个安全异常。
•S t r i n g g e t P r o p e r t y( S t r i n g key)
获取给定键名对应的系统属性。应用必须有权限获取这个属性， 否则会抛出一个安全
异常。 以下属性总是允许获取：
java.class.version
os.name
os.version
os.arch
file,separator
java,version
java.vendor
java.vendor.
0 13t
部署 Java ?用程序
591
path.separator
line.separator
java•spedfi cation,version
java.vm.spedfi cation,version
java.vm.specifi cation,vendor
java.vm.spedfi cation.name
java.vm.vendor
El 注释： 可以在 Java 运行时目录的 security/java.policy 文件中找到可以自由访问的系统属
性名。
13.2.2
首选项 API
我们已经看到，利用 Properties 类可以很容易地加载和保存配置信息。不过， 使用属性
文件有以下缺点：
•有些操作系统没有主目录的概念， 所以很难找到一个统一的配置文件位置。
•关于配置文件的命名没有标准约定， 用户安装多个 Java 应用时，就更容易发生命名冲
突。
有些操作系统有一个存储配置信息的中心存储库。最著名的例子就是 Microsoft Windows
中的注册表。Preferences 类以一种平台无关的方式提供了这样一个中心存储库。在 Windows
中， Preferences 类使用注册表来存储信息；在 Linux 上， 信息则存储在本地文件系统中。当
然，存储库实现对使用 Preferences 类的程序员是透明的。
Preferences 存储库有一个树状结构， 节点路径名类似于 /com/mycompany/myapp。 类似
于包名， 只要程序员用逆置的域名作为路径的开头， 就可以避免命名冲突。实际上， API 的
设计者就建议配置节点路径要与程序中的包名一致。
存储库的各个节点分别有一个单独的键 / 值对表， 可以用来存储数值、字符串或字节数
组，但不能存储可串行化的对象 = API 设计者认为对于长期存储来说， 串行化格式过于脆弱，
并不合适。当然， 如果你不同意这种看法，也可以用字节数组保存串行化对象。
为了增加灵活性，可以有多个并行的树。每个程序用户分别有一棵树；另外还有一棵系
统树， 可以用于存放所有用户的公共信息。 Preferences 类使用操作系统的“ 当前用户” 概念
来访问适当的用户树。
若要访问树中的一个节点，需要从用户或系统根开始：
Preferences root = Preferences.userRootQ;
或
Preferences root = Preferences.systemRootO:
然后访问节点。可以直接提供一个节点路径名:
Preferences node = root.node("/com/mycompany/myapp"):
如果节点的路径名等于类的包名，还有一种便捷方式来获得这个节点。 只需要得到这个
592
Java 核心技?
类的一个对象，然后调用：
Preferences node = Preferences.userNodeForPackage(obj.getClassO);
或
Preferences node = Preferences.systemNodeForPackage(obj .getClassO):
一般来说， Obj 往往是 this 引用。
一旦得到了节点， 可以用以下方法访问键 / 值表：
String get(String key, String defval)
int getInt(String key, int defval)
long getLong(String key, long defval)
float getFIoat(String key, float defval)
double getDouble(String key, double defval)
boolean getBoolean(String key, boolean defval)
byte[] getByteArray(String key, byte[] defval)
需要说明的是， 读取信息时必须指定一个默认值， 以防止没有可用的存储库数据。 之所
以必须有默认值， 有很多原因。 可能由于用户从未指定过首选项， 所以没有相应的数据。某
些资源受限的平台可能没有存储库， 移动设备有可能与存储库暂时断开了连接。
相对应地， 可以用如下的 put 方法向存储库写数据：
put(String key, String value)
putInt(String key, int value)
等等。
可以用以下方法枚举一个节点中存储的所有键：
String[] keys()
目前没有办法找出一个特定键对应的值的类型。
类似 Windows 注册表这样的中心存储库通常都存在两个问题：
•它们会变成充斥着过期信息的“ 垃圾场”。
•配置数据与存储库纠缠在一起， 以至于很难把首选项迁移到新平台。
Preferences 类为第二个问题提供了一个解决方案。可以通过调用以下方法导出一个子树
( 或者比较少见的，也可以是一个节点）的首选项：
void exportSubtree(OutputStream out)
void exportNode(OutputStream out)
数据用 XML 格式保存。 可以通过调用以下方法将数据导人到另一个存储库：
void importPreferences(InputStreain in)
下面是一个示例文件：
<?xml version="l.0" encoding="UTF-8"?>
<!D0CTYPE preferences SYSTEM "http://java.sun.com/dtd/preferences.dtd">
《preferences EXTERNAL_XML_VERSI0N="1.0">
<root type="user">
<map/>
<node name="com">
第n 章 部 署 Java ? 用 程 序
5 9 3
<map/>
<node name="horstmann">
<map/>
<node name="corejava">
<map>
〈entry key="left" value="117>
<entry key="top" value="9"/>
〈entry key="width" value="453"/>
〈entry key="height" value="365"/>
〈entry key="title" value="Hello, World!"/〉
</map>
</node>
</node>
</node>
</root>
</preferences〉
如果你的程序使用首选项， 要让用户有机会导出和导人首选项， 从而可以很容易地将设
置从一台计算机迁移到另一台计算机。程序清单 13-3 中的程序展示了这种技术。这个程序只
保存了主窗口的位置、 大小和标题。试着调整窗口的大小， 然后退出并重启应用。 窗口的状
态与之前退出时是一样的。
程序清单 13-3
preferences/PreferencesTest.java
1 package preferences;
2
3 import java.awt.*;
4 import java.io.*;
s import java.util.prefs.*;
6
7 import javax.swing.*;
8 import javax.swing.fi1echooser.*;
9
10 /**
11
* A program to test preference settings. The program remembers the frame
12
* position, size, and title,
is
* ©version 1.03 2015-06-12
i4
*
author Cay Horstmann
is
V
16 public class PreferencesTest
17 {
18
public static void main(String[] args)
EventQueue.invokeLater(() -> {
PreferencesFrame frame = new PreferencesFrameO;
frame.setDefaultCloseOperation(]Frame.EXIT_0N_CL0SE);
frame.setVisible(true);
})；
27
28 /**
29
* A frame that restores position and size from user preferences and updates the
30
* preferences upon exit.
31
V
20
594
Java 核心技?
32
36
class PreferencesFrame extends ]Frame
{
private static final int DEFAULT_WIDTH = 300;
.private static final int DEFAULT_HEIGHT = 200;
private Preferences root = Preferences.userRootO;
private Preferences node = root.node("/com/horstmann/corejava");
public PreferencesFrameO
{
// get position, size, title from preferences
int left = node.getlnt("left", 0);
int top = node.getlnt("top", 0);
int width = node.getlnt("width", DEFAULT_WIDTH);
int height = node.getlnt("height", DEFAULT.HEICHT);
setBounds(left, top, width, height);
// if no title given, ask user
String title = node,get("title",
if (title.equalsD)
title = 30ptionPane.showInputDialog("PIease supply a frame title:")；
if (title == null) title =
setTitie(title);
// set up file chooser that shows XML files
final JFileChooser chooser = new ]Fi1eChooser();
chooser.setCurrentDirectory(new File("."))；
chooser.setFi1eFi1ter(new Fi1eNameExtensionFi1ter("XML files", "xml"));
// set up menus
JMenuBar menuBar = new JMenuBarO；
set]MenuBar(menuBar);
DMenu menu = new ]Menu("File")；
menuBar.add(menu);
JMenuItem exportltern = new ]MenuItem("Export preferences");
menu.add(exportltem);
exportltem
.addActionListener(event -> {
if (chooser.showSaveDialog(PreferencesFrame ■this) == ]Fi1eChooser.APPR0VE_0PTI0N)
{
try
{
savePreferencesO;
OutputStream out = new Fi1eOutputStream(chooser
.getSelectedFileO);
node.expo「tSubtree(out);
out.closeO;
}
catch (Exception e)
e.printStackTraceO；
1 3t
部 署 Java ? 用 程 序
5 9 5
103
104
105
106
107
108
109
111
112
in
ns
116
117
118
119
120
121
122
123
124
12S
126
127 }
});
JMenuItem importltem = new ]MenuItem("Import preferences")；
menu,add(importltem);
importltem
.addActionListener(event -> {
if (chooser.showOpenDialog(PreferencesFrame.this)
{
try
{ I叩utStream in = new Fi1elnputStream(chooser
.getSelectedFileO);
Preferences.importPreferences(in);
in.closeO;
}
catch (Exception e)
{
e.printStackTrace();
})；
JMenuItem exitltem = new ]MenuItem("Exit")
menu.add(exitltem);
exitltem.addActionListener(event -> {
savePreferencesO;
System,exit⑼；
})；
]FileChooser.APPR0VE_0PTI0N)
}
public void savePreferencesO
{
node.putlnt("left", getXO);
node.putlnt("top", getYO);
node.putlnt("width", getWidth());
node,putlnt("height", getHeightO);
node,put("title", getTitle());
}
[ AW] javautil.prefs.Preferences 1.4
•Preferences userRoot( )
返回调用程序的用户的首选项根节点。
•Preferences systemRoot( )
返回系统范围的首选项根节点。
•Preferences node(String path )
返回从当前节点由给定路径可以到达的节点。 如果 path 是绝对路径 （也就是说， 以一
个 / 开头 )，则从包含这个首选项节点的树的根节点开始查找。 如果给定路径不存在相
应的节点， 则创建这样一个节点。
參 Preferences userNodeForPackage(Class cl )
596
Java 核心技?
• Preferences systemNodeForPackage(Class cl )
返回当前用户树或系统树中的一个节点， 其绝对节点路径对应类 d 的包名。
• StringC ] keys( )
返冋属于这个节点的所有键。
• String get(String key, String defval )
• int getInt( String key, int defval )
• long getLong(String key, long defval )
• float getFloat( String key, float defval )
• double getDouble(String key, double defval )
• boolean getBoolean( String key, boolean defval )
• byte[ 3 getByteArray( String key, byte[ ] defval )
返回与给定键关联的值， 或者如果没有值与这个键关联、 关联的值类型不正确或首选
项存储库不可用， 则返回所提供的默认值。
• void put( String key, String value)
• void putlnt(String key, int value)
• void putLong(String key, long value)
• void putFloat( String key, float value)
• void putDouble( String key, double value)
• void putBoolean( String key, boolean value)
• void putByteArray(String key, byte[ ] value)
在这个节点存储一个键 / 值对。
• void exportSubtree(OutputStream out )
将这个节点及其子节点的首选项写至指定的流。
• void exportNode(OutputStream out )
将这个节点 （但不包括其子节点） 的首选项写至指定的流。
• void importPreferences( InputStream in)
导人指定流中包含的首选项。
13.3
服务加载器
有时会开发采用插件体系结构的应用。 有些平台支持这种方法， 如 OSGi (http://0Sgi.
org), 并用于开发环境、应用服务器和其他复杂的应用中。这些平台超出了本书讨论的范畴，
不过 JDK 还提供了一个加载插件的简单机制， 这里就来介绍这个内容。
通常， 提供一个插件时， 程序希望插件设计者能有一些自由来确定如何实现插件的特
性。 另外还可以有多个实现以供选择。 利用 ServiceLoader 类可以很容易地加载符合一个公共
接口的插件。
第 1 3 章 部 署 Java ? 用 程 序
5 9 7
定义一个接口 （或者， 如果愿意也可以定义一个超类 )， 其中包含服务的各个实例应当提
供的方法。 例如， 假设你的服务要提供加密。
package serviceLoader;
public interface Cipher
{
byte口 encrypt(byte[] source, byte口 key) ;
byte[] decrypt(byte[] source, byte[] key);
int strength。;
}
服务提供者可以提供一个或多个实现这个服务的类， 例如：
package serviceLoader.impl ;
public class Caesa「Cipher implements Cipher
{
public byte口 encrypt(byte[] source, byte口 key)
{
byte口 result = new byte[source.length] ;
for (int i = 0; i < source.length; i++)
result[i] = (byte)(source[i] + key[0]);
return result;
}
public byte[] decrypt(byte[] source, byte[] key)
{
return encrypt(source, new byte[] { (byte) -key[0] });
}
public int strength0 { return 1; }
}
实现类可以放在任意包中， 而不一定是服务接口所在的包。每个实现类必须有一个无参
数构造器。
现在把这些类的类名增加到 META-INF/services 目录下的一个 UTF-8 编码文本文件中，
文件名必须与完全限定类名一致。 在我们的例子中， 文件 META-INF/services/serviceLoader.
Cipher 必须包含这样一行：
serviceLoader.impl .CaesarCipher
在这个例子中， 我们提供了一个实现类。还可以提供多个类， 以后可以从中选择。
完成这个准备工作之后， 程序可以如下初始化一个服务加载器：
public static ServiceLoader<Cipher> cipherLoader = ServiceLoader.1oad(Cipher.class);
这个初始化工作只在程序中完成一次。
服务加载器的 iterator 方法会对服务提供的所有实现返冋一个迭代器。（有关迭代器的更
多信息参见第 9 章。）最容易的做法是使用一个增强的 for 循环进行遍历。 在循环中， 选择一
个适当的对象来完成服务。
public static Cipher getCipher(int minStrength)
598
Java 核心技?
for (Cipher cipher : cipherLoader) // Implicitly calls cipherLoader.iteratorO
{
if (cipher.strength0 >= minStrength) return cipher;
}
return null ;
}
jw] java.util.ServiceLoader<S> 1.6
•static <S> ServiceLoader<S> 1oad( C1ass<S> service)
创建一个服务加载器来加载实现给定服务接口的类。
•Iterator<S> iterator
)
生成一个以“ 懒” 方式加载服务类的迭代器。也就是说，迭代器推进时类才会加载。
13.4
applet
applet 是包含在 HTML 页面中的 Java 程序。HTML 页面必须告诉浏览器要加载哪些
applet, 另 外 每 个 applet 要放在 Web 页面的什么位置。 可以想见， 需要使用 applet 的标记
( tag) 必须告诉浏览器从哪里得到类文件， 以及这个 applet 在 Web 页面上如何定位（大小、
位置等)。然后浏览器再从 Internet ( 或者从用户机器上的某个目录）获取类文件， 并自动运
行 applet。
最初开发 applet 时， 必须使用 Sun 的 RotJava 浏览器才能査看包含 applet 的 Web 页面。
很自然地， 很少有用户愿意只是为了尝试一个新的 Web 特性去使用另一个浏览器。后来
Netscape 在它的 Navigator 浏览器中包含了一个 Java 虚拟机， 那时 Java applet 才开始流行。
Microsoft 的 Internet Explorer 紧随其后。遗憾的是， Internet Explorer 中的 Java 支持很快就落
伍了，只能用于一些过期的 Java 版本，后来干脆取消了。
为了解决这个问题，Sun Microsystems 开发了“ Java Plug-in”。通过使用浏览器扩展机制，
可以把插件插人不同的浏览器， 允许这些浏览器使用外部 Java 运行时环境执行 Java applet。
多年来， 这个解决方案都能满足要求， applet 常用于教学工具、企业应用和一些游戏。
遗憾的是，尽管不时会发现 Java 虚拟机的安全漏洞并被恶意利用， 但 Sun Microsystems 和后
来的 Oracle 在修补这些漏洞方面却动作迟缓， 举措不力。由于不安全的 JVM 会置用户于危
险当中， 所以浏览器制造商让 Java 的使用更有难度。 有些浏览器除了最新版本的 Java 插件
外甚至完全禁用其他 Java 插件，另外一些浏览器则不再支持插件体系结构。Oracle 的反应也
很让人失望。它开始要求所有 applet 都有数字签名（有关内容见 13.4.9 节 )。
如今，开发人员要部署 Java applet 会很困难， 用户运行 applet 也不容易。因此， 我们相
信， 只有那些需要维护遗留 applet 的读者才会对下面的小节感兴趣。
0 注释： 要在浏览器中运行这一章中的 applet, 需要安装当前版本的 Java Plug-in, 并 确 保
浏览器与插件连接。 要测试 applet, 还需要适当地配置插件， 让它信任本地文件。 有关
指令参见 2.5 节。
第 U 章 部 署 Java ? 用 程 序
5 9 9
13.4.1 —
个简单的 applet
按传统， 我们首先编写一个 NotHelloWorld 程序，这里把它写为一个 applet。applet 就是
一个扩展了 java.applet.Applet 类的 Java 类。在本书中， 我们将使用 Swing 来实现 applet。这
里的所有 applet 都将扩展 JApplet 类，它是 Swing applet 的超类。如图 13-2 所示， JApplet 是
Applet 类的一个直接子类。
E1 注释： 如果你的 applet 包含 Swing 组件， 必须扩展 JApplet 类。 普通 Applet 中的 Swing
组件不能正确绘制。
-
I
JFrame
K
UZJ
图 13-2
Applet 继承图
程序清单 13«4 显示了 applet 版本“
Not Hello World” 的代码。注意这个代码与第 10 章中
的相应程序很类似。不过， 由于 applet 在 web 页面中， 所以没有必要指定退出 applet 的方法。
程序清单 13-4
applet/NotHelloWorld.java
1 package applet;
2
3 import java.awt.*;
4 import javax.swing.*;
600
Java 核心技?
6 /**
7
* Version 1.24 2015-06-12
8
* ©author Cay Horstmann
9
*/
10 public class NotHelloWorld extends ]Applet
11 1
12
public void initO
u
{
i4
EventQueue.invokeLater(0 -> {
is
]Label label = new ]Label("Not a Hello, World applet",
16
SwingConstants.CENTER);
17
add(label);
i«
})；
要执行 applet, 需要完成以下 3 个步骤：
1 ) 将 Java 源文件编译为类文件。
2 ) 将类打包到一个 JAR 文件中（参见 13.1.1 节)。
3 ) 创建一个 HTML 文件， 告诉浏览器首先加载哪个类文件， 以及如何设定 applet 的
大小。
下面给出这个文件的内容：
<applet dass="applet/NotHel1olilorld.class" archive="NotHel1oWorld.jar" width="300i, height="300">
</applet>
在浏览器中査看 applet 之前， 最好先在 JDK 自带的 applet viewer (applet 查看器）程序中
进行测试。要使用 applet 查看器测试我们的示例
applet, 可以在命令行输入：
appletviewer NotHelloWorldApplet.html
applet 查看器程序的命令行参数是 HTML 文件
名， 而不是类文件。 图 13-3 给出了显示这个 applet
的 applet 查看器。
applet 查看器很适用于测试的第一阶段， 不
过总会有某个时候需要在真正的浏览器中运行
applet , 来查看用户实际看到的样子。具体来说，
applet 査看器程序只会显示 applet, 而 不 会 显 示
周 围 的 HTML 文本。 如果你的 HTML 文件包含
多个 applet 标记， applet 查 看 器 就 会 弹 出 多 个
窗口。
要正确地査看 applet, 只 需 要 把 HTML 文件加
载到浏览器（如图 13M 所示）。 如果 applet 没有出
现， 则需要安装 JavaPlug-in， 并允许它加载无签
Applet
Not aHello,
Applet started.
World applet
图 13-3
在 applet 査看器中査看 applet
•'
• - / •泛
LS 1Q file:///home/cay/books/ci [ ]»
>]
Applet NotHelloWorldApplet started
图 134
在浏览器中查看 applet
第 1 3 章 部 署 Java ?用程序
601
名的本地 applet，参见 2.5 节的介绍。
提示： 如果修改了 applet 并重新编译， 就需要重新启动浏览器， 这样才会加栽新的类文
件。 只是刷新 HTML 页面并不会加载新代码。 调试 applet 时这会很麻烦。 可以利用 Java
控制台避免烦人的浏览器重启。 启动 Java 控制台， 并执行 x 命令， 这会清空类加栽器缓
存。然后可以重新加载 HTML 页面， 这样就会使用新的 applet 代码。在 Windows 上， 可
以在 Windows 控制面板打开 Java Plug-in 控制。在 Linux 上， 可以运行 jcontrol, 请求显
示 Java 控制台。 加栽 applet 时控制台就会弹出。
很容易把一个图形化 Java 应用转换为可以嵌入在 Web 页面中的 applet。基本上来说， 所
有用户界面代码都可以保持不变。下面给出具体的步骤：
1 ) 建立4HTML 页面， 其中包含加载 applet 代码的适当标记。
2 ) 提供 JApplet 类的一个子类。 将这个类标记为 public。否则 applet 将无法加载。
3 ) 删去应用中的 main 方法。不要为应用构造框架窗口。你的应用将在浏览器中显示。
4 ) 把所有初始化代码从框架窗口移至 applet 的 init 方法。 不需要明确构造 applet 对象，
浏览器会实例化 applet 对象并调用 init 方法。
5 ) 删除 setSize 调用；对 applet 来说， 用 HTML 文件中的 width 和 height 参数就可以指
定大小。
6 ) 删除 setDefaultCloseOperation 调用。 applet 不能关闭； 浏览器退出时 applet 就会终止
运行。
7 ) 如果应用调用 setTitle, 则删除这个方法调用。applet 没有标题栏。（当然， 可以用
HTML title 标记为 Web 页面本身指定标题。）
8 ) 不要调用 setVisible(true)。 applet 会自动显示。
[iiw] java.applet.Applet 1.0
• v o i d i n i t( )
首次加载 applet 时会调用这个方法。 覆盖这个方法， 把所有初始化代码放在这里。
• v o i d s t a r t( )
覆盖这个方法，用户每次访问包含这个 applet 的浏览器页面时需要执行的代码都放在
这个方法中。典型的动作是重新激活一个线程。
• v o i d s t o p( )
覆盖这个方法，用户每次离开包含这个 applet 的浏览器页面时需要执行的代码都放在
这个方法中。典型的动作是撤销一个线程。
• v o i d d e s t r o y( )
覆盖这个方法，用户退出浏览器时需要执行的代码都放在这个方法中。
• v o i d r e s i z e( i n t w i d t h , i n t h e i g h t )
请求调整 applet 的大小。 如果能用在 Web 页面上这会是一个很不错的方法；遗憾的
是，在当前浏览器中并不可用， 因为这与当前浏览器的页面布局机制有冲突。
602
Java 核心技?
13.4.2
applet HTML 标记和属性
下面是一个最简形式的 applet 标记示例：
<applet code="applet/NotHelloWorld.class" archive="NotHelloWorld. jar"
width="300" height="100"x/applet>
可以在 applet 标记中使用以下属性：
•width, height
这些属性是必要的， 指定了 applet 的宽度和高度（单位为像素）。在 applet 查看器
中， 这是 applet 的初始状态。可以调整 applet 查看器创建的任何窗口的大小。但在浏
览器中不能调整 applet 的大小。需要适当地猜测你的 applet 需要占据多大的空间，才
能够让所有用户看到好的效果。
•align
这个属性指定了 applet 的对齐方式。 属性值与 HTML img 标记的 align 属性值
相同。
•vspace, hspace
这些属性是可选的， 指定了 applet 上下的像素数（ vspace) 以及左右两边的像素
数（hspace) 0
•code
这个属性指定了 applet 的类文件名。
路径名必须与 applet 类的包一致。 例如， 如果 applet 类在包 com.mycompany 中，
那 么 这 个 属 性 就 是 code="com/mycompany/MyApplet.class"， 也 可 以 是 code="com.
mycompany.MyApplet.class” 0
code 属性只指定包含 applet 类的类的名字。当然，你的 applet 可能包含其他类文
件。一旦浏览器的类加载器加载了包含 applet 的类， 它就会发现还需要更多类文件，
并加载这些文件。
•archive
这个属性会列出包含 applet 的类以及其他资源的 JAR 文件（可能有多个 JAR 文
件)。这些文件会在加载 applet 之前从 Web 服务器获取。JAR 文件用逗号分隔。 例如：
<applet code="MyApplet.class"
archive="MyClasses.jar,corejava/Core]avaClasses.jar"
height，
齡
•codebase
这个属性是加载 JAR 文件（早期还可以加载类文件）的 URL。
•object
这个属性已经过时， 可以指定包含串行化 applet 对象的文件的文件名，这个文件
用于持久存储 applet 状态。由于没有办法对一个串行化文件签名， 所以这个特性已经
没有用了。
013t
部署 Java ?用程序
603
•alt
Java 禁用时，可以使用 alt 属性来显示一个消息。
如果一个浏览器根本无法处理 applet, 它会忽略未知的 applet 和 param 标记。浏
览 器 会 显 示 记 之 间 的 所 有 文 本。与 之 相 反， 支 持 Java 的浏览
器不会显示 <applet> 和 </applet> #记之间的任何文本。对于使用这些浏览器的人，
可以在这些标记之间显示提示消息。例如：
<applet . . . alt="If you activated Java, you would see my applet here"〉
If your browser could show 3ava, you would see my applet here.
</applet>
•name
编写脚本的人可以为 applet 指定一 个 name 属性， 用来指本所编写的 applet。
Netscape 和 Internet Explorer 都允许通过 JavaScript 调用页面上的 applet 的方法。
要从 JavaScript 访问一个 applet, 首先要指定一个名字。
<applet ... na«e=Miinenx/applet>
然后可以用 document.applets.ap/7/e/wfl/we 表示这个对象。 例如：
var rayApplet = document.applets.line;
接下来就可以调用 applet 方法了：
myApplet.initO;
希望同一个页面上的两个 applet 相互直接通信时 name 属性也很重要。为每个当
前 applet 实例指定一个名字，将这个字符串传递到 AppletContext 接口的 getApplet 方
法。本章后面还会讨论这种 applet 间通信 ( inter-applet communication ) 机制。
El 注释： 在 www.javaworld.com/javatips/jw-javatip80.html 中， Francis Lu 使用 JavaScript-
Java 通信解决了一个年代久远的问题： 如何调整 applet 的大小， 而不限于只能通过硬编
码 width 和 height 属性来指定。这是 Java 与 JavaScript 集成的一个很好的例子。
13.4.3
使用参数向 applet 传递信息
与应用可以使用命令行信息一样，applet 可以使用内嵌在 HTML 文件中的参数。这是利
用 HTML param 标记以及所定义的属性来完成的。例如， 假设想让 Web 页面确定 applet 中使
用的字体样式。可以使用以下 HTML 标记：
〈applet code="FontParamApplet.class" ...>
<param name="font" value="Helvetica"/>
</applet>
然后使用 Applet 类的 getParameter 方法得到参数的值：
public class FontParamApplet extends 3Applet
public void initO
604
Java 核心技?
String fontName = getParameterffont");
El 注释： 只能在 applet 的 init 方法中调用 getParameter 方法， 而不能在构造器中调用。执行
applet 构造器时， 参数还没有准备好。 由于大多数重要 applet 的布局都由参数确定， 所
以建议不要为 applet 提供构造器，要把所有初始化代码放在 init 方法中。
参数总是作为字符串返回。 如果需要数值类型， 则需要将字符串转换为数值。 可以调用
适当的方法采用标准方式进行转换， 如 Integer 类的 parselnt。
例如， 如果想为字体增加一个 size 参数， HTML 代码可能如下所示：
<applet code="PontParamApplet.class" ...>
<param name="font" value="Helvetica7>
<param name="size" value="247>
</applet>
下面的源代码显示了如何读取这个整数参数：
public class FontParamApplet extends JApplet
{
public void init()
{
String fontName = getParameter("font");
int fontSize = Integer.parseInt(getParameter("size"));
El 注释： param 标记中的 name 属性值与 getParameter 方法的参数匹配时， 会使用不区分大
小写的比较。
除了要确保代码中的参数匹配之外， 还要检
查是否缺少 size 参数。可以简单地测试是否为 null
来达到目的。例如：
int fontsize;
String sizeString = getParameter("size");
if (sizeString = null) fontSize = 12;
else fontSize = Integer.parseint(sizeString):
下面给出一个经典的 applet, 它会使用参数绘
制一个直方图， 如图 13-5 所示。
这个 applet 从 HTML 文件中的 param 值得到
直方柱的标签和高度。 下面给出图 13-5 相应的
HTML 文件：
〈applet code="Chart.class" width="400" height="300">
<param name="title" value=MDiameters of the Planets"/〉
图 13-5
— 个直方图 applet
Eile
£dit
ienK
Hijtory 0oolOT>arks Iools t lp
.:
命，參，營 W 傷 |D file:///home/cay/bookib|g)fcj
Diameters of the Planets
JL
Mercury Venus
Earth
M&rs
Jupiter
Saturn
Ur&nu! Neptune
Pluto
Applet Chart started
第 /3 章 部 署 Java ?用程序
605
<param name="values" value="9"/>
<pa「am name="naine.r value="Mercury"/>
<param name="name.2" value="Venus"/>
<param name="name.3" value="Earth'7>
<param name="name.4" value="Mars"/>
<param name="name.5" value="]upiter'7>
<param name="name.6" value="Saturn"/>
<param name="name.7" value="Uranus"/>
<param name="name.8" value="Neptune"/>
<param name="name.9" value="Pluto"/>
<param name="value.r value="3100"/>
<param name="value.2" value="7500'7>
<param name="value.3
H value="8000"/>
<param name="va1ue.4" value="4200"/>
<param name="value.5" value:"88000"/〉
<param name="value.6" value="71000"/>
<param name="value.7" value="32000"/>
<param name="value.8" value:"306007〉
<param name="value.9" value="14307>
</applet>
也可以在 applet 中建立一个字符串数组和一个数值数组， 不过使用参数机制有两个好
处。 Web 页面上的 N—
个 applet 可以有多个副本， 分别显示不同的图表： 只需要在页面上
放置两个 applet 标记， 分别用一组不同的参数。 另外， 可以改变要绘制图表的数据。必须承
认， 行星直径在相当长的时间内都不会改变， 不过假设你的 Web 页面包含每周销量数据的图
表呢？ 由于使用纯文本， 所以更新 Web 页面很容易。 而每周编辑和重新编译一个 Java 文件
就要麻烦多了。
实际上， 有很多商业 JavaBeans 组件 （ beans) 可以提供比这个 applet 更精美的图表。 如
果购买这样一个 bean, 可以把它放在你的 Web 页面中， 为它提供参数， 而无须知道这个
applet 是如何呈现图表的。
程序清单 13-5 是这个图表 applet 的源代码。 需要说明， init 方法读取了参数， 并由
paintComponent 方法绘制图表。
代码清单 13-5
chart/Chart.java
、
1 package chart;
2
3 import java.awt.*;
4 import java.awt.font.*;
5 import java.awt.geom.*;
6 import javax.swing.*;
7
8
/**
9
* ©version 1.34 2015-06-12
10
* author Cay Horstmann
V
12 public class Chart extends ]Applet
13 {
14
public void initQ
EventQueue.invokeLater(() -> {
606
Java 核心技?
17
String v = getParameter("values");
18
if (v == null) return;
19
int n = Integer.parselnt(v);
20
double[] values = new double[n];
21
String[] names = new String[n];
22
for (int i = 0;i < n; i++)
23
{
24
values[i] = Double.parseDouble(getParameter("value." + (i + 1)));
25
names[i] = getParameter("name." + (i + 1));
26
}
27
28
add(new ChartComponent(values, names, getParameter("title”）））；
29
})；
30
}
31 }
J2
33 /**
34
* A component that draws a bar chart.
35
V
36
class ChartComponent extends ]Component
37
{
38
private double[] values;
39
private String[] names;
40
private String title;
41
42
/**
43
* Constructs a ChartComponent.
44
* @param v the array of values for the chart
45
* @param n the array of names for the values
46
* @param t the title of the chart
47
*/
48
public ChartComponent(double[] v, String[] n, String t)
49
{
so
values = v;
si
names = n;
52
title = t;
53
}
54
ss
public void paintComponent(Graphics g)
56
{
57
Craphics2D g2 = (Craphics2D) g;
58
59
// compute the minimum and maximum values
60
if (values == null) return;
61
double minValue = 0;
62
double maxValue = 0;
63
for (double v : values)
64
{
65
if (minValue > v) minValue = v;
66
if (maxValue < v) maxValue = v;
67
}
68
if (maxValue == minValue) return;
69
70
int panelWidth = getWidthO;
71
int panelHeight = getHeightO;
72
73
Font titleFont = new Font("SansSerif", Font.BOLD, 20);
第 / ■? 章 部 署 JavaM 用 程 序
6 0 7
102
103
104
105
106
107
115
119
120
121
123
124
125
129
Font labelFont = new FontC'SansSerif", Font.PLAIN, 10);
// compute the extent of the title
FontRenderContext context = g2.getFontRenderContext():
Rectangle2D titleBounds = titleFont.getStringBounds(title, context);
double titleWidth = titleBounds.getWidth():
double top = titleBounds.getHeightO;
// draw the title
double y = -titleBounds.getYO; // ascent
double x = (panelWidth - titleWidth) / 2;
g2.setFont(titleFont);
g2.drawString(title, (float) x, (float) y);
// compute the extent of the bar labels
LineMetrics labelMetrics = label Font.getLineMetrics("", context);
double bottom = 1abelMetrics.getHeight();
y = panelHeight - 1abelMetrics.getDescent();
g2.setFont(labelFont):
// get the scale factor and width for the bars
double scale = (panelHeight - top - bottom) / (maxValue - minValue);
int barWidth = panelWidth / values.length;
// draw the bars
for (inti = 0; i < values.length; i++)
{
// get the coordinates of the bar rectangle
double xl =i * barWidth + 1;
double yl = top;
double height = values[i] * scale;
if (values[i] >= 0)
yl += (maxValue - values[i]) * scale;
else
{
yl += maxValue * scale;
height = -height;
// fill the bar and draw the bar outline
Rectangle2D rect = new Rectangle2D.Double(xl, yl, barWidth - 2, height);
g2.setPaint(Color.RED);
g2.fi11(rect);
g2.setPaint(Color.BLACK);
g2.draw(rect);
// draw the centered label below the bar
Rectang1e2D labelBounds = 1abelFont.getStringBounds(names[i], context);
double labelWidth = 1abelBounds.getWidth()；
x = xl + (barWidth - labelWidth) / 2;
g2.drawString(names[i], (float) x, (float) y);
60S
Java 核心技?
java.applet.Applet 1.0
•public String getParameter( String name )
得到加载 applet 的 Web 页面中用 param 标记定义的一个参数值。 字符串 name 是区分
大小写的。
•public String getAppletInfo( )
很多 applet 作者都会覆盖这个方法来返回关于当前 applet 作者、 版本和版权信息的字
符串。
•public String[][] getParameterInfo( )
可以覆盖这个方法来返回 applet 支持的一个 param 标记选项数组。每一行包含 3 项：
参数名、类型和参数的描述。下面给出一个例子：
"fps", "1-10", "frames per second"
"repeat", "boolean", "repeat image loop?"
"images", "url", "directory containing images"
13.4.4
访问图像和音频文件
applet 可以处理图像和音频。 写作这本书时， 图像必须是 GIF、 PNG 或 JPEG 格式， 音
频文件必须是 AU、AIFF、 WAV 或 MIDI。另外也支持动画 GIF, 可以显示动画。
要用相对 URL 指定图像和音频文件的位置。通常可以通过调用 getDocumentBase 或
getCodeBase 方法得到基 URL , 前一个方法会得到包含这个 applet 的 HTML 页面的 URL, 后
者会得到 applet 的 codebase 属性指定的 URL。
可以为 getlmage 或 getAudioClip 方法提供基 URL 和文件位置。 例如：
Image cat = getlmage(getDocumentBase(), "images/cat.gif");
AudioGip meow = getAudioClip(getDocumentBase0, "audio/meow.au");
第 10 章中已经见过如何显示一个图像。 要播放一段音频， 只需要调用它的 play 方法。
还可以调用 Applet 类的 play 方法而无须先加载这段音频。
piay(getDocumentBase(), "audio/raeow.au")；
[AW|java.applet.Applet 1.0
•URL getDocumentBase( )
得到包含这个 applet 的 Web 页面的 URL。
•URL getCodeBase( )
得到加载这个 appiet 的代码基目录的 URL。 这可以是 codebase 属性引用的目录的绝
对 URL, 如果没有指定， 则是 HTML 文件所在目录的 URL。
•void play( URL url )
•void play( URL url , String name)
前一种形式会播放 URL 指定的一个音频文件。第二种形式使用字符串来提供相对于
第一个参数中 URL 的一个路径。 如果未找到任何音频剪辑，那么什么也不会发生。
$13t
部署 Java ?用程序
609
•
AudioClip getAudioClip(URL url)
•
AudioClip getAudioClip(URL url, String name)
第一种形式从给定 URL 得到一个音频剪辑。 第二种形式使用字符串提供相对于第一
个参数中 URL 的一个路径。 如果无法找到音频剪辑， 这两个方法会返回 null。
•
Image getImage(URL url)
• Image getImage(URL url, String name)
返回封装了 URL 所指定图像的一个图像对象。 如果图像不存在， 贝立即返回 null。否
则， 会启动一个单独的线程来加载这个图像。
13.4.5
applet 上下文
applet 在浏览器或 applet 查看器中运行。applet 可以要求浏览器为它做些事情， 例如， 获
取一个音频剪辑，在状态栏中显示一个简短的消息， 或者显示一个不同的 Web 页面。浏览器
可能会执行这些请求，也可能将其忽略。 例如， 如果一个在 applet 查看器中运行的 applet 要
求 applet 査看器程序显示一个 Web 页面， 那么什么也不会发生。
要与浏览器通信，applet 可以调用 getAppletContext 方法。 这个方法会返回一个实现
AppletContext 接口的对象可以认为 AppletContext 接口的具体实现是 applet 与外围浏览器
之间的一个通信渠道。 除了 getAudioClip 和 getlmage 之外，AppletContext 接口还包含很多有
用的方法，将在接下来的几节中讨论。
13.4.6
applet 间通信
一个 Web 页面可以包含多个 applet。 如果一个 Web 页面包含来自同一个 codebase 的多
个 applet，它们可以相互通信。 很自然地， 这是一项高级技术，你可能很少会用到。
如果为 HTML 文件中的各个 applet 指定 name 属性，可以使用 AppletContext 接口
的 getApplet 方法来得到这个 applet 的一个引用。 例如， 如果 HTML 文件中包含以下
标记：
<app1et code="Chart.class" width="100" height="100" name="Chartr>
则以下调用
Applet chartl = getAppletContext().getAppletC'Chartl");
会提供这个 applet 的一个引用。 如何使用这个引用呢？ 假设 Chart 类中有一个方法可以接受
新数据并重绘图表，可以通过适当的类型转换来调用这个方法：
((Chart) chartl).setData(3, "Earth", 9000);
还可以列出一个 Web 页面上的所有 applet, 不论它们是否有 name 属性。getApplets 方法
会返回一个枚举对象。 下面给出一个循环，它会打印当前页面上所有 applet 的类名：
Enumeration<Applet> e = getAppletContext() •getApplets();
while (e.hasMoreElementsO)
610
Java 核心技?
Applet a = e.nextElementO;
System•out.printl n(a.getClass().getName());
}
applet 不能与不同 Web 页面上的其他 applet 通信。
13.4.7
在浏览器中显示信息项
可以访问外围浏览器的两个区域：状态栏和 Web 页面显示区， 这都要使用 AppletContext
接口的方法。
可以用 showStatus 方法在浏览器底部的状态栏中显示一个字符串。例如：
showStatus("Loading data . . . please wait");
提示：从我们的经验来看， showStatus 的作用有限。浏览器也会经常使用状态栏， 它会
用类似“
Applet running” 之类的话菝盖原先的消息。 可以使用状态栏显示不太重要的消
息， 如 “Loading data . . . please wait”， 而不能显示用户不能遗漏的消息。
可以用 showDocument 方法告诉浏览器显示一个不同的 Web 页面。 有很多方法可以达到
这个目的。最简单的办法是调用 showDocument 并提供一个参数， 即你想要显示的 URL:
URL u = new URLC'http://horstmann.com/i ndex.html"):
getAppletContextO.showDocument(u):
这个调用的问题在于，它会在当前页面所在的同一个窗口中打开新 Web 页面， 因此会替
换你的 applet。要返回原来的 applet, 用户必须点击浏览器的后退按钮。
可以在 showDocument 调用中提供第二个参数告诉浏览器在另一个窗口中显示文档（见
表 13-2 )。如果提供了特殊字符串 "_blank"， 浏览器会用这个文档打开一个新窗口，而不是
替换当前文档。更重要的是， 如果利用 HTML 中的框架特性，可以把一个浏览器窗口分割为
多个框架， 每个框架都有自己的名字。可以把 applet 放在一个框架中，让它在其他框架中显
示文档。下一节会给出这样的一个例子。
E1 注释：applet 查看器不显示 Web 页面。showDocument 方法在 applet 查看器中会被忽略。
表 13-2
showDocument 方法
目标参数
位
置
"_self
_
或无
在当前框架中显示文档
"_parent”
在父框架中显示文档
"_top"
在最顶层框架中显示文档
"_blank"
在新的未命名顶层窗口中显示文档
其他字符串
在指定框架中显示。如果不存在指定名字的框架， 则打开一个新窗口， 并指定为这个窗口的名字
|*w|java.applet.Applet 1.2
• public AppletContext getApp etContext( )
$13t
部署 Java ?用程序
611
提供 applet 的浏览器环境的一个句柄。在大多数浏览器中，可以使用这个信息控制运
行这个 applet 的浏览器。
• void showStatus(String msg)
在浏览器的状态栏中显示指定字符串。
j w| java.applet.AppletContext 1.0
• Enumeration<Applet> getApplets()
返回相同上下文（也就是相同的 Web 页面）中所有 applet 的一个枚举（见第 9 章)。
• Applet getApplet(String name)
返回当前上下文中有指定名的 applet; 如果不存在则返回 null。只会搜索当前 Web 页面。
• void showDocument(URL url)
• void showDocument(URL url , String target)
在浏览器的一个框架中显示一个新 Web 页面。第一种形式中， 新页面会替换当前页
面。第二种形式使用 target 参数标识目标框架（见表 13-2 )。
13.4.8
沙箱
从一个远程站点加载代码再在本地执行时，安全会变得非常重要。访问一个 Web 页面会
自动启动这个页面上的所有 applet。
点击一个链接会启动 Java Web Start 应用。如果访问一个 Web 页面或点击一个链接会执
行用户计算机上的任意代码，有恶意的人就能轻松地窃取机密信息、 访问金融数据，或者利
用用户的机器发送垃圾邮件。
为了确保 Java 技术不会被恶意利用，Java 提供了一个精巧的安全模型， 我们将在卷 II 中
详 细 讨 论。安 全 管 理 器 会 检 査 对 所 有 系 统 资 源 的 访 问。默 认 情 况 下，它
只允许无害的操作。要允许其他操作， 用户必须明确地批准 applet 或应用运行。
远程代码在所有平台上能做什么？ 如果只是显示图像和播放声音， 获得用户的按键和鼠
标点击，将用户输入发回给加载代码的主机，这些通常都没有问题。这对于显示资料数据或
者与教育程序或游戏交互来说够用了。这个受限的执行环境通常称为“ 沙箱”。沙箱中运行
的代码不能改变或刺探用户的系统。
具体来说， 沙箱中的程序有以下限制：
•它们绝对不能运行任何本地可执行程序。
•它们不能读写本地计算机的文件系统。
•它们不能査找有关本地计算机的信息， 不过所用的 Java 版本和一些无害的操作系统细
节除外。特别是， 沙箱中的代码不能査找用户的名字、e-mail 地址， 等等。
•远程加载的程序需要用户同意与下载这个程序的服务器以外的主机通信；这个服务器
??源主机（originating ftost)。这个规则通常称为“ 远程代码只能与家人通话”。这
个规则可以保护用户防止代码刺探内部网资源。
612
Java 核心技?
•所有弹出窗口都带有一个警告消息。这个消息是一个安全特性， 确保用户不会把这个
窗口误认为一个本地应用。 这是因为会担心不设防的用户可能访问一个 Web 页面， 被
诱骗运行远程代码， 然后键人密码或信用卡号， 这可能会发回给 Web 服务器。 在早
期版本的 JDK 中，这个消息看上去很严重：“ 不能信任的 Java Applet 窗口”。后来的
版本把这个警告稍稍放松了一点：“ 未授权的 Java Applet 窗口”，后来又改为“ 警告：
Java Applet 窗口”。 现在是一个警告性的小三角， 只有最敏锐的用户才会注意到。
沙箱概念没有原先那么有意义。过去， 任何人都可能部署沙箱代码， 只有需要得到许可
在沙箱外执行的代码才需要有数字签名。 如今， 不论是否在沙箱中运行， 通过 Java Plug-in
执行的所有代码都必须有数字签名。
13.4.9
签名代码
applet 或 Java Web Start 应用的 JAR 文件必须有数字签名。签名的 JAR 文件带有一个证
书， 指示签名者的身份。加密技术可以确保这个证书不可伪造，而且能检测出对签名文件的
任何篡改。
例如， 假设你收到由 yWorlcs GmbH 开发并签名的一个应用，它使用 Thawte 签发的一个
证书来签名（如图 13-6 所示）。接收到应用时，可以确保以下几个方面：
1 ) 代码与签名时是一样的；没有第三方篡改这个代码。
2 ) 签名确实来自 yWorks。
3 ) 证书确实由 Thawte 签发。（ Java Plug-in 知道如何检查 Thawte 和很多其他证书发行商
的证书， 另外也可以安装候选的“ 根证书”。）
0 二
91
图 13-6 —
个安全证书
$ 13t
部署 Java ?用程序
613
如果点击“ 更多信息” 链接， 会告诉你这个应用运行时不会有 Java 通常的安全限制。要
安装和运行这个应用吗？ 实际上这取决于你对 yWorks GmbH 的信任。
要从某个支持的发行商得到一个证书，可能每年要付数百美元， 一些证书发行商还要求
提供公司或企业的执照证明。过去，一些 Java 开发人员会生成他们自己的证书用来对代码签
名。当然，Java Plug-in 没有办法检査这些证书的准确性。 不过，过去 Java Plug-in 会为用户
提供证书。这没有太大意义， 因为很少有用户了解安全与不安全的证书之间的差别。现在已
经不再支持不安全的证书。
如果想要发布一个 Java applet 或 Web Start 应用， 你将别无选择。 必须从 Java Plug-in 支
持的一个证书发行商得到一个证书， 用它对你的 JAR 文件签名。
如果你在一家公司任职， 你的公司很有可能已经与一个证书发行商建立了关系，可以直
接订购一个代码签名证书。如果没有，很有必要货比三家，因为具体的价格有很大差异， 有
些发行商对于向个人签发证书要求比较宽松。
你的证书会提供相关的安装说明， 指出如何把它安装在一个 Java 密钥库中， 这是一个受
密码保护的文件， 可以在签名过程中从这个密钥库获取证书。 要保证这个密钥库文件和密码
的安全。
接下来，需要确定希望得到什么权限。 你可以选择沙箱权限和完全权限。 建立一个清单
文件（参见 13.1.2 节)。
包含 Permissions: sandbox 或 Permissions: all-permissions, 例$0:
Manifest-Version: 1.0
Permissions: all-permissions
接下来运行 jai•工具：
jar cvfR MyApplet.jar Manifest.af mypackage/*.c1ass
HTML 文件的 applet 元素应当有属性 archive="MyAppletjarM。
最后，对 JAR 文件签名。命令如下:
jarsigner -keystore keystorefile -tsa timestampURL MyApplet.jar keyalias
需要向证书发行商询问时间戳 URL。 密钥别名由证书发行商签名。运行以下命令:
keytool -keystore keystorefile -list
来得到密钥别名。还可以用 keytool 命令的 -changealias 选项修改别名。（关于 keytool 的更多
信息， 参见卷 n 的第 9 章。）
现在把签名的 JAR 文件和包含 applet 元素的 HTML 文件放到你的 Web 服务器上。
0 注释： 可以详细地控制为 Java 应用授予的权限， 有关内容将在卷 II 的第 12 章讨论。 不
过， 这个体系没有以对消费者来说有意义的方式投入使用。Java Plug-in 只提供了两个安
全级别：沙箱和完全权限 _
614
Java 核心技?
13.5
Java Web Start
Java Web Start 是一项在 Internet 上发布应用程序的技术。Java Web Start 应用程序包含下
列主要特性：
•Java Web Start 应用程序一般通过浏览器发布。只要 Java Web Start 应用程序下载到本
地就可以启动它， 而不需要浏览器。
•Java Web Start 应用程序并不在浏览器窗口内。它将显示在浏览器外的一个属于自己的
框架中。
•Java Web Start 应用程序不使用浏览器的 Java 实现。浏览器只是在加载 Java Web Start
应用程序描述符时启动一个外部应用程序。这与启动诸如 Adobe Acrobat 或 RealAudio
这样的辅助应用程序所使用的机制一样。
•数字签名应用程序可以被赋予访问本地机器的任意权限。未签名的应用程序只能运行
在“ 沙箱” 中，它可以阻止具有潜在危险的操作。
13.5.1
发布 Java Web Start 应用
要想准备一个通过 Java Web Start 发布的应用程序， 应该将其打包到一个或多个 JAR 文
件中。然后创建一个 Java Network Launch Protocol ( JNLP) 格式的描述符文件。将这些文件
放置在 Web 服务器上。
还需要确保 Web 服务器对扩展名为.jnlp 的文件报告一个 application/x-java-jnlp-file 的
MIME 类型（浏览器利用 MIME 类型确定启动哪一种辅助应用程序)。更详细的有关信息请
参看 Web 服务器文档。
提示：要想体验 Java Web Start, 需 要 从 jakarta.apache.org/tomcat 上安装 Tomcat。Tomcat
是一个 servlet 和 JSP 页面的容器， 也提供网页服务。 它被预配置为服务于 JNLP 文件所
对应的 MIME 类型。
试着用 Java Web Start 发布第 12 章中开发的计算器应用程序。步骤如下:
1 ) 编译程序。
javac -classpath .:；
rfjt/jre/lib/javaws.jar webstart/*.java
2 ) 使用下列命令创建一个 JAR 文件：
jar cvfe Calculator,jar webstart.Calculator webstart/*.class
3 ) 使用下列内容准备启动文件 Calculatorjnlp:
<?x»l version="1.0" encoding="utf-8"?>
<jn1p spec="1.0+" codebase="http://1ocalhost:8080/calculator/" href Calculator.jnlp">
<information>
<title>Cal cul ator Demo Application</title>
<vendor>Cay S. Horstmann</vendor>
<descri pti on>A Cal culator</description>
<offline-allowed/>
第 A? 章 部 署 JavaM 用 程 序
6 1 5
《/information〉
<resources〉
<java version="l.6.0+7>
<jar href=MCa1culator.jar"/>
</resources>
<application-desc/>
</jnlp>
注意， 版本号必须是 1.6.0, 而不是 6.0。
启动文件格式很容易理解， 无须说明。要想了解全部的规范， 请参看 www.orade.com/
technetwork/java/javase/javawebstarto
4 ) 如 果 使 用 Tomcat， 则 在 Tomcat 安 装 的 根 目 录 上 创 建 一 个 目 录 tomcat/webapps/
calculator。创建子目录 tomcat/webapps/calculator/WEB-INF， 并且将最小的 web.xml 文件放
置在 WEB-INF 子目录下：
<?xml version=n1.0" encoding="utf-8"?>
<web-app version="2.5" xmlns="http://java.sun.con/xnl/ns/j2ee"
xml ns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee
http://java.sun.com/xml/ns/j2ee/web-app_2_5.xsd">
</web-app>
5 ) 将 JAR 文件和启动文件放入 towcaZ/webapps/calculator 目录。
6 ) 按照 2.5 节描述的过程， 在 Java 控制面板中将 URLhttp://localhost:8080 增加到可信
站点列表。 或者， 可以按 13.4.9 介绍的过程为 JAR 文件签名。
7 ) 在 /owcaf/bin 目录执行启动脚本来启动 Tomcat。
8 ) 将浏览器指向 JNLP 文件。 例如， 如果使用 Tomcat, 则 访 问 http://localhost:8080/
calculator/Calculator.jnlpo 如果已经对浏览器完成了 Java Web Start 的有关配置，应该能看到
Java Web Start 的启动窗口（见图 13-7 )。
如果你的浏览器不知道如何处理 JNLP 文件，可能会提供一个选项将它们与一个应用关
联。 如果是这样， 请选择_/ /M)in/javaws。 否则， 明确如何将 MIME 类型 application/x-java-
jnlp-file 与 javaws 应用关联。还可以试着重新安装可以做到这一点的 JDK。
9 ) 稍后，计算器就会出现， 所带的边框表明这是一个 Java 应用程序， 如图 13-8 所示。
Downloading application.
Publisher CayS. Horstmann
From:
http /tocalhost:8080
|Cancel|
Java Application Window
( )
'1
«
1
9
/
4
，… 5.. |
6
1
.
0
=
+
图 13-7
启动 Java Web Start
图 13-8
Java Web Start 发布的计算器
10 ) 当再次访问 JNLP 文件时， 应用程序将从缓存中取出。可以利用 Java 插件控制面板
616
Java 核心技?
查看缓存内容， 如图 13-9 所示。 在 Windows 系统的 Windows 控制面板中可以看到 Java 插件
控制面板。 在 Linux 下， 可以运行 jdk/jre/bin/ControlPanel。
Show:lApplicati
13 0.1®
A
Cache Size：17775 KB
Application
yEd Graph Editor
Vendor
1
Type
j
Date |
Size |
Status
yWorks GmbH
lowagie.com
Cay S.Horstmann
Application
7070 KB
Application
3282 KB
Application
Apr 30. 20...
4638 KB
j Animal Algorithm Anima... AnimalDeveloper T...
Application
Apr 30. 20...
2768
Cafculator Demo Applic.,, i Cay S,Horstmann
Application
图 13-9
应用程序缓存
<5
提示： 如果在测试 JNLP 配置期间不想运行 web 服务器， 可以通过执行下列命令临时性
地覆盖启动文件中的 URL:
]avaws -codebase fi1e://programDirectoryJNLPfile
例如， 在 UND( 中， 可以从包含 JNLP 文件的目录中直接地发出下列命令：
javaws -codebase file:// 'pwd ' Calculator.]nip
当然， 没有告诉用户再次运行应用程序时要启动缓存视图。 可以利用安装器安装桌面和
菜单的快捷键。 将下列代码添加到 JNLP 文件中：
<shortcut>
<desktop/>
<menu submenu="Accessories"/>
々shortcut〉
当用户首次下载应用程序时， 将会显示“ desktop integration warning”， 如图 13-10 所示。
还应该为菜单快捷键和启动屏幕提供一个图标。Oracle 建议提供 32 x 32 和 64 x 64 的图
标。 把这些图标文件与 JNLP 和 JAR 文件一起
放在 Web 服务器上。 将下列代码添加到 JNLP
文件的 information 节中：
〈icon href="calcJcon32.png" width="32" height="32" />
〈icon href="calc_icon64.png" width="64" height="64" />
注意， 这些图标与应用程序图标无关。 如
果想让应用程序也有图标，需要将一个独立的
图标图像添加到 JAR 文件中， 并在框架类中
调用 setlconlmage 方法。请看程序清单 13-1。
The application would like to create
shortcuts. Do you want to continue?
Calculator Demo Application
Publisher:
Cay S. Horstmann
From:
http://localhost:8080
EJ
t
Q<
' 1 I skip |
9
application will add
图 13- 10
桌面集成警告
第 A3 章 部 署 Java ? 用 程 序
6 1 7
13.5.2
JNLPAPI
JNLP API 允许未签名的应用程序在沙箱中运行， 同时通过一种安全的途径访问本地资
源。 例如， 有一些加载和保存文件的服务。应用程序不能査看系统文件， 也不能指定文件
名。然而， 可以弹出一个文件对话框， 程序用户可以从中选择文件。 在文件对话框弹出之
前， 应用程序不能浏览文件系统， 也不能指定文件名。取而代之的是， 系统将弹出文件对话
框， 由程序的用户从中选择文件。在文件对话框弹出之前， 程序的用户会得到警告并且必须
同意继续处理， 如图 13-11 所示。 而且，API 并不给予程序访问 File 对象的权限。特别是，
应用程序没有办法找到文件的位置。因此， 程序员需要通过工具实现“ 打开文件” 和“ 保存
文件” 的操作，但是对于不信任的应用程序，
系统应尽可能地将信息隐藏起来。
API 提供了下面的服务：
•加载和保存文件
•访问剪贴板
•打印
•下载文件
•在默认的浏览器中显示一个文档
•保存和获取持久性配置信息
•确信只运行一个应用程序的实例
要访问服务， 需要使用 ServiceManager, 如下所示：
FileSaveService service = (FileSaveService) ServiceManager.1ookupC'javax.jnlp.Fi1eSaveService"):
如果服务不可用， 调用将拋出 UnavailableServiceException。
0 注释： 如果想要编译使用了 JNLPAPI 的程序， 那就必须在类路径中包含 javaws.jar 文件。
这个文件在 JDK 的 jre/lib 子目录下。
The application has requested read access
to a file on the machine. Do you want to
i
■ I
http7/localhost:8080
□g
5toi
'S,sact；
C
J c
cel
图 13-11
Java Web Start 安全警告
现在， 讨论一些最常用的 JNLP 服务。要保存文件，需要为文件对话框提供文件的初始
路径名和文件扩展类型、要保存的数据和建议的文件名。 例如：
service.saveFi1eOial og
, net* String□ { "txt" }, data,
__
calc.txt");
数据必须由 InputStream 传递。这可能有些困难。在程序清单 13-6 中， 程序使用下面的
策略：
1 ) 创建 ByteArrayOutputStream 用于存放需要保存的字节。
2 ) 创建 PrintStream 用于将数据传递给 ByteArrayOutputStream。
3 ) 将要保存的数据打印到 PrintStream。
4 ) 建立 ByteArraylnputStream 用于读取保存的学节。
5 ) 将上面的流传递给 saveFileDialog 方法。
卷 n 的第 1 章将阐述更多有关流的知识。现在，可以忽略示例中关于流的细节。
618
Java 核心技?
要想从文件中读取数据， 需要使用 FileOpenService。 它的 openFileDialog 对话框
接收应用程序提供的初始路径名和文件扩展名， 并返回一个 FileCcmtents 对象。 然后
调用 getlnputStream 和 getOuputStream 方法来读写文件数据。 如果用户没有选择文件，
openFileDialog 方法将返回 null。
FileOpenService service = (FileOpenService) ServiceManager.1ookup(njavax.jnlp.Fi1eOpenService");
FileContents contents = service.openFileDialogC'.", new String[] { "txt" });
if (contents !* null)
{ I叩utStream in = contents.getl叩utStream();
注意， 应用程序不知道文件名或文件所放置的位置。相反地， 如果想要打开一个特定的
文件， 需要使用 ExtendedService:
ExtendedService service = (ExtendedService) ServiceManager.1ookup("javax.jnlp.ExtendedService");
FileContents contents = service.openFile(new Fi1e("c:\\autoexec.bat"));
if (contents != null)
{
OutputStream out = contents.getOutputStreamQ;
程序的用户必须同意文件访问。如图 13-12 所示。
要想在默认浏览器中显示一个文档， 就需要使用 BasicService 接口。注意， 有些系统可
能没有默认的浏览器。
BasicService service = (BasicService) ServiceManager.1ookup("javax.jnl p.BasicService");
if (service.isWebBrowserSupported())
service.showDocument(url);
else . . .
处于起步阶段的 PersistenceService 允
许应用程序保存少量的配置信息， 并且在
应用程序下次运行时恢复。 这种机制类似
于 HTTP 的 cookie, 使用 URL 键 进 行 持
久性存储。URL 并不需要指向一个真正的
web 资源， 服务仅仅使用它们来构造一个
方便的具有层次结构的命名机制。 对于任
何给定的 URL 键，应用程序可以存储任意
的二进制数据（存储可能会限制数据块的
大小)。
因为应用程序是彼此分离的， 一个特定的应用程序只能使用从 codebase 开始的 URL 键
值（codebase 在 JNLP 文件中指定）。例如， 如果一
应用程序是从 http://myserver.com/apps
下载的， 那么它只能使用 http://myserver.com/apps/subkeyl/subkey2/... 形式的键。访问其他键
值的企图终将会失败。
The application has requested read-write
access to the listed files. Do you want to
allow this action?
LJ
] C«n«l
图 1 3-1 2
文 件 访 问 警 告
$ 13t
部署 Java ?用程序
619
应用程序可以调用 BasicService 类的 getCodeBase 方法查看它的 codebase。
可以调用 PersistenceService 中的 create 方法建立一个新的键：
URL url = new URL(CodeBase, "mykey");
service.create(url, maxSize);
要想访问与某个特定键关联的信息， 需要调用 get 方法。 这个方法将返回一个
FileContents 对象， 通过这个对象可以对键数据进行读写。例如：
FileContents contents = service,get(url);
InputStream in - contents.getlnputStream();
OutputStream out = contents.getOutputStream(true); // true = overwrite
遗憾的是， 无法轻而易举地知道这个键是已经存在还是需要创建。可以假定这个键已经
存在， 并调用 get 方法。如果抛出 FileNotFoundException， 就说明需要创建。
B1 注释： Java Web Start 应用程序和 applet 都可以使用
常规的打印 API 进行打印。 这时会弹出一个安全对
话框来询问用户是否允许访问打印机。 有关更多的
打印 API 的信息，请参看卷 n 的第 7 章。
程序清单 13-6 对计算器应用程序的功能做了一点
改进。这个计算器有一个虚拟的纸带来记录所有的计算
过程。用户可以保存或读取计算历史信息。为了演示持
久性存储功能， 应用程序允许设置框架的标题。 如果再
次运行程序， 应用程序会从持久性存储中得到标题（如
H 13-13 所示）。
图 13-13
WebStartCalculator 应用程序
程序清单 13-6
webstart/WebStartCalculatorFrame.java
1 package webstart;
2
3 import java.io.BufferedReader;
4 import java.io.ByteArraylnputStream:
s import java.io.ByteArrayOutputStream:
6 import java.io.Fi1eNotFoundException:
7 import java.io.IOException;
8 import java.io.I叩utStream;
9 import java.io.InputStreainReader:
10 import java.io.OutputStream;
11 import java.io.PrintStream;
12 import java.net.MaiformedURLException:
13 import java.net.URL;
14
is import javax.jnip.BasicService;
16 import javax.jnip.FileContents;
17 import :javax.jnip.FileOpenSe「vice;
18
import javax.jnlp.Fi1eSaveService:
19 import javax.jnip.PersistenceService;
20 import javax.jnlp.Servi ceManager:
21 import javax.jnlp.Unavai1ableServiceException:
favorite title
java Appl丨catlort Window
Hie
1000 -
1729 0
12，
二
....fei .
620
Java 核心技?
22 import javax.swing.]Frame;
23 import javax.swing.]Menu;
24 import javax.swingJMenuBar;
25 import javax.swingJMenuItem;
26 import javax.swing.]OptionPane;
27
28 /**
29
* A frame with a calculator panel and a menu to load and save the calculator history.
30
*/
31 public class CalculatorFrame extends 3Frame
32 {
33
private Cakulato「Panel panel;
34
35
public Calculato「Frame()
36
{
37
setTitle();
38
panel = new Calculato「Panel();
39
add(panel);
40
41
JMenu fileMenu = new ]Menu("File")；
42
JMenuBar menuBar = new ]MenuBar();
43
menuBar.add(fileMenu);
44
set]MenuBar(menuBar);
45
46
. JMenuItem openltem = fileMenu.add("Open");
47
openltem.addActionlistener(event -> open());
48
JMenuItem saveltem = fileMenu.add("Save");
49
saveltem.addActionlistener(event -> save());
so
si
pack();
52
}
53
54
/**
ss
* Gets the title from the persistent store or asks the user for the title if there is no prior
56
* entry.
57
*/
ss
public void setTitleO
59
{
60
try
61
{
62
String title = null;
63
64
BasicService basic = (Basi〔Service) ServiceManager.1ookup("javax.jnlp.BasicService")；
6s
URL CodeBase = basic.getCodeBaseO;
66
67
PersistenceService service = (PersistenceService) ServiceManager
68
.1ookup("javax.jnlp.PersistenceService");
69
URL key = new URL(CodeBase, "title");
70
71
try
72
{
73
FileContents contents = service,get(key);
74
I叩utStream in = contents.getl叩utStream();
7s
BufferedReader reader = new BufferedReader(new InputStreamReader(in));
76
title = reader.readLineO;
77
}
catch (FileNotFoundException e)
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
11S
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
13S
第
章
部
署 Java ? 用程序
621
title = JOpti onPane.showInputDial og("PIease supply a frame title:");
if (title == null) return;
service.create(key, 100) ;
FileContents contents = service.get(key);
OutputStream out = contents.getOutputSt ream(t rue);
PrintStream printout = new PrintSt ream(out);
printout ,print(title);
}
setTitie(title) ;
}
catch (UnavailableServiceException | IOException e)
{
JOptionPane.showMessageDialog(this, e);
/**
* Opens a history file and updates the display.
V
public void open()
{
try
{
FileOpenService service = (FileOpenService) ServiceManager
•1ookupC'javax.jnlp.Fil eOpenServi ce") ;
FileContents contents = service.openFileDialog("_"， new String[] { "txt" }) ;
30ptionPane.showMessageDialog(this, contents.getName());
if (contents != null)
{
InputStream in = contents.getlnputStreamO;
BufferedReader reader = new BufferedReader(new InputStreamReader(in)) ;
String line;
while ((line = reader. readLineO) != null)
{
panel .append(line) ;
panel .append("\n")；
catch (UnavailableServiceException e)
{
JOptionPane.showMessageDialog(this，e) ;
}
catch (IOException e)
{
JOpti onPane.showMessageDialog(this， e);
}
}
* Saves the calculator history to a file.
V
public void save()
622
Java 核心技?
136
try
137
{
138
ByteArrayOutputStream out = new ByteArrayOutputSt「eam() ;
139
PrintStream printout = new PrintStream(out);
140
printout,print(panel.getTextO);
141
I叩utStream data = new ByteArraylnputStream(out.toByteArray());
142
FileSaveService service = (FileSaveService) ServiceManager
143
.1ookupfjavax.jnlp.FileSaveService");
144
service.saveFileDialog
new String[] { "txt" }, data, "calc.txt")；
M5
}
146
catch (Unavai1ableServiceException e)
147
{
148
30ptionPane. showMessageDialog(thi s, e) ;
149
}
iso
catch (IOException e)
m
{
152
]0ptionPane.showMessageDialog(thi s, e);
153
}
154
}
155 }
[AW] javax.jnlp.ServiceManager
• static String[ ] getServiceNames ( )
返回所有可用的服务名称。
• static Object
1 ookup( String name )
返回给定名称的服务。
IAW|javax.jnlp.BasicService
• URL getCodeBase( )
返回应用程序的 codebase。
• boolean isWebBrowserSupported( )
如果 Web Start 环境可以启动浏览器， 返回 true。
• boolean showDocument( URL url )
尝试在浏览器中显示一个给定的 URL。 如果请求成功， 返回 true。
• InputStream getInputStream( )
返回一个读取文件内容的输入流。
• OutputStream getOutputStream(boolean overwrite )
返回一个对文件进行写操作的输出流。 如果 overwrite 为 true， 文件中已有的内容将被
覆盖。
• String getName( )
返回文件名 （但不是完整的目录路径)。
• boolean canRead( )
第 J3 章 部 署 JavaM 用 程 序
6 2 3
• b o o l e a n c a n W r i t e( )
如果后台文件可以读写， 返回 true。
A«J javax.jnlp.FileOpenService
• F i 1e C o n t e n t s
o p e n F i 1e D i a l o g( S t r i n g p a t h H i n t, S t r i n g[ ] e x t e n s i o n s )
• F i l e C o n t e n t s[ ]
o p e n M u l t i F i l e D i a l o g( S t r i n g
p a t h H i n t,
S t r i n g[ ]
e x t e n s i o n s )
显示警告信息并打开文件选择器。返回文件的内容描述符或者用户选择的文件。 如果
用户没有选择文件， 返回 mill。
AW|javax.jnlp.FileSaveService
• F i 1e C o n t e n t s
s a v e F i l e D i a l o g( S t r i n g
p a t h H i n t,
S t r i n g[ ]
e x t e n s i o n s,
I n p u t S t r e a m d a t a, S t r i n g n a m e H i n t )
參 F i l e C o n t e n t s
s a v e A s F i l e D i a l o g ( S t r i n g
p a t h H i n t ,
S t r i n g[ ]
e x t e n s i o n s , F i l e C o n t e n t s d a t a )
显示警告信息并打开文件选择器。 写人数据， 并返回文件的内容描述符或者用户选择
的文件。如果用户没有选择文件， 返回 null。
[*w] javax.jnlp.PersistenceService
• l o n g c r e a t e( URL k e y, l o n g m a x s i z e)
对于给定的键创建一个持久性存储条目。返回由持久性存储授予这个条目的最大存储
空间。
• v o i d d e l e t e( URL k e y )
删除给定键的条目。
• S t r i n g[ ] getNames( URL u r l )
返回由给定的 URL 开始的全部键的相对键名。
• F i 1e C o n t e n t s g e t(URL k e y)
返回用来修改与给定键相关的数据的内容描述符。 如果没有与键相关的条目， 抛出
FileNotFoundException。
至此， 我们结束了对 Java 软件部署的讨论。这一卷最后一章将介绍一个重要内容： 并发
编程。