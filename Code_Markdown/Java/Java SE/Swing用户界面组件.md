▲ Swing 与模型- 视图 -控制器设计模式
▲ 布局管理概述
▲ 文本输人
▲ 选择组件
▲ 菜单
▲ 复杂的布局管理
▲ 对话框
▲ GUI 程序排错
上一章主要介绍了如何使用 Java 中的事件模式。通过学习读者已经初步知道了构造图形
用户界面的基本方法。本章将介绍构造功能更加齐全的图形用户界面（ GUI ) 所需要的一些
重要工具。
下面， 首先介绍 Swing 的基本体系结构。要想弄清如何有效地使用一些更高级的组件，
必须了解底层的东西。然后， 再讲述 Swing 中各种常用的用户界面组件， 如文本框、 单选按
钮以及菜单等。接下来，介绍在不考虑特定的用户界面观感时， 如何使用 Java 中的布局管理
器排列在窗口中的这些组件。最后，介绍如何在 Swing 中实现对话框。
本章囊括了基本的 Swing 组件， 如文本组件、 按钮和滑块等， 这些都是基本的用户界面
组件， 使用十分频繁。Swing 中的高级组件将在卷 n 中讨论。
12.1
Swing 和模型-视图- 控制器设计模式
前面说过， 本章将从 Swing组件的体系结构开始。首先， 我们讨论设计模式的概念， 然
后再看一下 Swing 框架中最具影响力的“ 模型- 视图-控制器” 模式。
12.1.1
设计模式
在解决一个问题时， 不需要从头做起， 而是借鉴过去的经验， 或者向做过相关工作的专
家请教。设计模式就是一种方法，这种方法以一种结构化的方式展示专家们的心血。
近几年来， 软件工程师们开始对这些模式进行汇总分类。这个领域的先驱者的灵感来源
于建筑师 Christopher Alexander 的设计模式。 他在《The Timeless Way of Building》
（ 1979 年，
牛津大学出版）一书中， 为公共和私人居住空间的建筑设计模式进行了分类。下面是一个典
型的例子：
窗 户 位 置
每个人都喜欢靠窗户的座位，可以画上凸出去的窗户、 低窗台的大窗户以及放在这里的
舒适椅子。 如果一个房间中没有这样一个地方，很少有人会感到舒服和安逸。
如果房间中没有像这样“ 位置” 的一个窗户，房间里的人就有可能要做出下列抉择：（ 1 ) 舒
470
Java 核心技?
适地坐下；（2 ) 要充足的阳光。
显然， 舒适的地方是房间中最想坐的地方, 但它们远离窗户，这种冲突不可避免。
因此， 对于白天长时间逗留的房间， 至少要将一个窗
户开在“ 窗户位置” 处（见图 12-1 )。
在 Alexander 的模式分类和软件模式的分类中， 每种模
式都遵循一种特定的格式。 这些模式首先描述背景， 即引
发设计问题的情形；接着解释问题， 通常这里会有几个冲
突的因素；最终， 权衡这些冲突， 给出问题的解决方案。
在“ 窗户位置” 模式中， 背景是在白天逗留时间较长
I
細11圆丨丄_
脚_
丨
的房间。 冲突因素就是既想舒适地坐下，又想拥有充足的
窗?位置
光线。解决方案是找到一个“ 窗户位置'
在“ 模型-视图-控制器” 模式中， 背景是显示信息和接收用户输人的用户界面系统。
有关“ 模型-视图-控制器” 模式将在接下来的章节中讲述。这里有几个冲突因素。对于同
一数据来说， 可能需要同时更新多个可视化表示。 例如，为了适应各种观感标准， 可能需要
改变可视化表示形式；又例如， 为了支持语音命令， 可能需要改变交互机制。解决方案是将
这些功能分布到三个独立的交互组件： 模型、视图和控制器。
模型-视图-控制器模式并不是 AWT 和 Swing 设计中使用的唯一模式。下列是应用的
另外几种模式：
•容器和组件是“ 组合 （composite)” 模式
•带滚动条的面板是“ 装饰器（decorator)” 模式
•布局管理器是“ 策略（strategy)” 模式
设计模式的另外一个最重要的特点是它们已经成为文化的一部分。只要谈论起模型-视
图-控制器或“ 装饰器” 模式，遍及世界各地的程序员就会明白。 因此，模式已经成为探讨
设计方案的一种有效方法。
读 者 可 以 在 Erich Gamma 等 编 著 的《 Design Patterns
Elements of Reusable Object-
Oriented Software》（Addison-Wesley 出版社， 1995 年出版e) —
■书中找到大量的实用软件模式
的规范描述，这是一本研究模式运动的书籍。这里再强烈地推荐一本由 FrankBuschmann 等
编著的《A System of Patterns》，John Wiley & Sons 出版社于 1996 出版。这是一本不错的书籍，
相对前一本， 这本书更容易读懂。
12.1.2
模 型- 视 图-控 制 器 模 式
让我们稍稍停顿一会儿， 回想一下构成用户界面组件的各个组成部分， 例如， 按钮、 复
选框、 文本框或者复杂的树形组件等。 每个组件都有三个要素：
•内容， 如： 按钮的状态 （是否按下 )， 或者文本框的文本。
©
本书中文版《设计模式—可复用面向对象软件的基础》已由机械工业出版社出版。一编辑注
$ 12
Swing 用?界面?件
471
•外观（颜色，大小等)。
•行为 （对事件的反应)。
这三个要素之间的关系是相当复杂的， 即使对于最简单的组件（如： 按钮）来说也是如
此。很明显，按钮的外观显示取决于它的观感。Metal 按钮的外观与 Windows 按钮或者 Motif
按钮的外观就不一样。 另外， 外观显示还要取决于按钮的状态：当按钮被按下时， 按钮需要
被重新绘制成另一种不同的外观。 而状态取决于按钮接收到的事件。当用户在按钮上点击
时，按钮就被按下。
当然， 在程序中使用按钮时， 只需要简单地把它看成是一个按钮，而不需要考虑它的内
部工作和特性。?竟，这些是实现按钮的程序员的工作。无论怎样， 实现按钮的程序员就要
对这些按钮考虑得细致一些了。毕竟，无论观感如何， 他们必须实现这些按钮和其他用户界
面组件， 以便让这些组件正常地工作。
为了实现这样的需求， Swing 设计者采用了一种很有名的设计模式（design pattern ) : 模
型 - 视图 - 控制器 （ model-view-controller) 模式。 这种设计模式同其他许多设计模式一样，
都遵循第 5 章介绍过的面向对象设计中的一个基本原则： 限制一个对象拥有的功能数量。 不
要用一个按钮类完成所有的事情， 而是应该让一个对象负责组件的观感， 另一个对象负责存
储内容。模型- 视图- 控制器（MVC) 模式告诉我们如何实现这种设计，实现三个独立的类：
•模型 （model): 存储内容。
•视图 （view): 显示内容。
•控制器 （controller): 处理用户输入。
这个模式明确地规定了三个对象如何进行交互。模型存储内容， 它没有用户界面。按钮的
内容非常简单， 只有几个用来表示当前按钮是否按下， 是否处于活动状态的标志等。文本框内
容稍稍复杂一些，它是保存当前文本的字符串对象。这与视图显示的内容并不一致—如果内容
的长度大于文本框的显示长度，用户就只能看到文本框可以显示的那一部分， 如图 12-2 所示。
jjumps:
/er the lazy flog" B
|
i 1
|broiun |foK jump
图 12-2
文本框的模型和视图
模型必须实现改变内容和查找内容的方法。例如，一个文本模型中的方法有：在当前文
本中添加或者删除字符以及把当前文本作为一个字符串返回等。记住：模型是完全不可见的。
显示存储在模型中的数据是视图的工作。
0 注释：“ 模式” 这个术语可能不太贴切， 因为人们通常把模式视为一个抽象概念的具体表
示。 汽车和飞机的设计者构造模式来模拟真实的汽车和飞机。但这种类比可能会使你对模
型 -视图-控制器模式产生错误的理解。在设计模式中， 模型存储完整的内容， 视图给出
了内容的可视化显示（完整或者不完整）。一个更恰当的比喻应当是模特为画家摆好姿势。
472
Java 核心技?
此时， 就要看画家如何看待模特， 并由此来画一张画了。 那张画是一幅规矩的肖像画， 或
是一幅印象派作品， 还是一幅立体派作品（以古怪的曲线来描绘四肢）完全取决于画家。
模型- 视图-控制器模式的一个优点是一个模型可以有多个视图， 其中每个视图可以显
示全部内容的不同部分或不同形式。 例如， 一个 HTML 编辑器常常为同一内容在同一时刻
提供两个视图： 一个 WYSIWYG (所见即所得）视图和一个“ 原始标记” 视图（见图 12-3 )。
当通过某一个视图的控制器对模型进行更新时， 模式会把这种改变通知给两个视图。视图得
到通知以后就会自动地刷新。 当然，对于一个简单的用户界面组件来说， 如按钮， 不需要为
同一模型提供多个视图。
图 12-3
同一模型的两个不同视图
控制器负责处理用户输入事件， 如点击鼠标和敲击键盘。然后决定是否把这些事件转化
成对模型或视图的改变。例如， 如果用户在一个文本框中按下了一个字符键， 控制器调用模
型中的“ 插入字符” 命令，然后模型告诉视图进行更新，而视图永远不会知道文本为什么改
变了。但是如果用户按下了一个光标键， 那么控制器会通知视图进行卷屏。卷动视图对实际
文本不会有任何影响， 因此模型永远不会知道这个事件的发生。
图 12-4 给出了模型、 视图和控制器对象之间的交互。
在程序员使用 Swing 组件时， 通常不需要考虑模型 - 视图-控制器体系结构。 每个用户
界面元素都有一个包装器类（如 JButton 或 JTextField) 来保存模型和视图。当需要查询内容
(如文本域中的文本）时， 包装器类会向模型询问并且返回所要的结果。 当想改变视图时（例
如， 在一个文本域中移动光标位置)， 包装器类会把此请求转发给视图。然而，有时候包装器
转发命令并不得力。在这种情况下， 就必须直接地与模型打交道（不必直接操作视图—这
是观感代码的任务)。
第 /2 聿
Swing 用?界面?件
473
1
'
I
d
给制视图
读取内晗
I更新内容 5
内容改变
_
新视图 p
图丨2*4
模型、 视图、 控制器对象之间的交互
除了“ 本职工作” 外，模型-视图- 控制器模式吸引 Swing 设计者的主要原因是这种模
式允许实现可插观感。 每个按钮或者文本域的模型是独立于观感的。当然可视化表示完全依
赖于特殊观感的用户界面设计，且控制器可以改变它。 例如， 在一个语音控制设备中，控制
器需要处理的各种事件与使用键盘和鼠标的标准计算机完全不同。通过把底层模型与用户界
面分离开， Swing 设计者就能够重用模型的代码，甚至在程序运行时对观感进行切换。
当然，模式只能作为一种指导性的建议而并没有严格的戒律。没有一种模式能够适用于
所有情况。例如， 使用“ 窗户位置” 模式 （设计模式中并非主要成分）来安排小卧室就不太
合适。同样地， Swing 设计者发现对于可插观感实现来说， 使用模型-视图-控制器模式并
非都是完美的。模型容易分离开， 每个用户界面组件都有一个模型类。但是， 视图和控制器
的职责分工有时就不很明显， 这样将会导致产生很多不同的类。当然， 作为这些类的使用者
来说， 不必为这些细节费心。前面已经说过，这些类的使用者根本无需为模型操心， 仅使用
组件包装器类即可。
12.1.3
Swing 按钮的模型- 视图- 控制器分析
前一章已经介绍了如何使用按钮，当时没有考虑模型、 视图和控制器。 按钮是最简单的
用户界面元素， 所以我们从按钮开始学习模型-视图-控制器模式会感觉容易些。 对于更复
474
Java 核心技?
杂的 Swing 组件来说，所遇到的类和接口都是类似的。
对于大多数组件来说， 模型类将实现一个名字以 Model 结尾的接口， 例如， 按钮就实现
了 ButtonModel 接口。实现了此接口的类可以定义各种按钮的状态。实际上，按钮并不复杂，
在 Swing 库中有一个名为 DefaultButtonModel 的类就实现了这个接口。
读者可以通过查看 ButtonModd 接口中的特征来了解按钮模型所维护的数据类别。表 12-1
列出了这些特征。
表 12-1
ButtonModel 接口的属性
属性名
值
与按钮关联的动作命令字符串
按钮的快捷键
如果按钮被按下且鼠标仍在按钮上则为 true
如果按钮是可选择的则为 true
如果按钮被按下且鼠标按键没有释放则为 true
如果鼠标在按钮之上则为 true
如果按钮已经被选择（用于复选框和单选钮) 则为 true
actionCommand
enabled
pressed
rollover
se ected
每个 JButton 对象都存储了一个按钮模型对象， 可以用下列方式得到它的引用。
]Button button = new JButton("Blue");
ButtonModel model = button.getModel () i
实际上， 不必关注按钮状态的零散信息， 只有绘制它的视图才对此感兴趣。诸如按钮是
否可用这样的重要信息完全可以通过 JButton 类得到（当然， JButton 类也通过向它的模型询
问来获得这些信息 )。
下面查看 ButtonModel 接口中不包含的信息。模型不存储按钮标签或者图标。对于一个
按钮来说， 仅凭模型无法知道它的外观 （实际上， 在有关单选钮的 12.4.2 节中将会看到，这
种纯粹的设计会给程序员带来一些麻烦)。
需要注意的是， 同样的模型（即 DefaultButtonModel ) 可用于下压按钮、 单选按钮、复选
框、 甚至是菜单项。 当然， 这些按钮都有各自不同的视图和控制器。 当使用 Metal 观感时，
JButton 类用 BasicButtonUI 类作为其视图； 用 ButtonUIListener 类作为其控制器。通常， 每
个 Swing 组件都有一个相关的后缀为 UI 的视图对象， 但并不是所有的 Swing 组件都有专门
的控制器对象。
在阅读 JButton 底层工作的简介之后可能会想到： JButton 究竟是什么？ 事实上， 它仅仅
是一个继承了 JComponent 的包装器类， JComponent 包含了一个 DefauUButtonModel 对象，
一些视图数据（例如按钮标签和图标）和一个负责按钮视图的 BasicButtonUI 对象。
12.2
布局管理概述
在讨论每个 Swing 组件（例如： 文本域和单选按钮）之前，首先介绍一下如何把这些组
第 章
Swing 用 ? 界 面 ? 件
4 7 5
件排列在一个框架内。与 Visual Basic 不同， 由于在 JDK 中没有表单设计器， 所以需要通过
编写代码来定制（布局）用户界面组件所在的位置。
当然， 如果有支持 Java 的开发环境， 就可能有某种布局工具来部分自动地或全部自动地
完成这些布局任务。然而， 弄清底层的实现方式是非常重要的， 因为即使最好的工具有时也
需要手工编码。
回顾上一章的程序， 我们设计了几个按钮， 点击这些按钮可以改变框架的背景颜色。 如
图 12-5 所示。
这几个按钮被放置在一个 JPane〖 对象中， 且用流布局管理器（ flow layout manager) 管
理， 这是面板的默认布局管理器。图 12-6 展示了向面板中添加多个按钮后的效果。正如读者
所看到的，当一行的空间不够时，会将显示在新的一行上。
1-
ICiffDLifa .1-
图 12-5
包含三个按钮的面板
1 tfe.iaW . ll
图 12-6
用流布局管理六个按钮的面板
另外， 按钮总是位于面板的中央， 即使用户对
框架进行缩放也是如此。 如图 12-7 所示。
通常， 组件放置在容器中， 布局管理器决定容
器中的组件具体放置的位置和大小。
按钮、文本域和其他的用户界面元素都继承于
Component 类， 组件可以放置在面板这样的容器
中。由于 Container 类继承于 Component 类， 所以
容器也可以放置在另一个容器中。 图 12-8 给出了
Component 的类层次结构。
I ] Jhjt
1
「
If
1
图 12-7
改变面板尺寸后自动重新安排按钮
0 注释： 可惜的是， 继承层次有两点显得有点混乱。 首先， 像 JFrame 这样的顶层窗口
是 Container 的子类， 所以也是 Component 的子类， 但却不能放在其他容器内。 另外，
JComponent 是 Container 的子类， 但不直接继承 Component, 因此， 可以将其他组件添
置到 JButton 中。（但无论如何， 这些组件无法显示出来）。
每个容器都有一个默认的布局管理器，但可以重新进行设置。 例如， 使用下列语句：
panel .setLayout(new CridLayout(4, 4));
这个面板将用 GridLayout 类布局组件。可以往容器中添加组件。容器的 add 方法将把组
件和放置的方位传递给布局管理器。
476
Java 核心技?
I
Object
1
I Component ■
、 a111
f— :(rzi
i—
.
. ! ■，.
,
r J
1- uL
,
-L
,
J
Window I
J JComponen
i
jy
. 1— 4-1
(i
r-J~~i~~44-
Frame| Diaiog|
JPa„el|c::
t|A-|
¥
’ 宁
’
_
"
1
•’平
• 广
"J I
— n
,
,
|jjComboBoxJ |JMenuBar|
T
r T
i
T
： i
JFrame
JDialog |
JTextField jj|
JTextArea|j|
1
1
JButton ■
■
JMenultem V
阁 12-8
Component 的类层次结构
Awj java.awt.Container 1.0
• Void SetLayout(LayoutManager m)
为容器设置布局管理器
• Component add(Component c)
• Component add(Component c, Object constraints) 1.1
将组件添加到容器中， 并返回组件的引用。
参数： c
要添加的组件
constraints
布局管理器理解的标识符
AW|java.awt.FlowLayout 1.0
• FIowLayout ()
• FIowLayout(int align)
• FIowLayout (int align, int hgap , int vgap)
构造一个新的 FlowLayout 对象。
参数： align
LEFT、 CENTER 或者 RIGHT
第 1 2 章
Swing 用 ? 界 面 ? 件
4 7 7
hgap
以像素为单位的水平间距（如果为负值， 则强行重叠）
vgap
以像素为单位的垂直间距（如果为负值，则强行重叠）
12.2.1
边框布局
J
「
一
.—丁：
—
'
边框布局管理器（border layout manager) 是每个 IFrame
I
北
的内容窗格的默认布局管理器。 流布局管理器完全控制每个
I
组件的放置位置，边框布局管理器则不然，它允许为每个组
一和
F|
|
西
中
东
；_
件选择一个放置位置。 可以选择把组件放在内容窗格的中
部、 北部、南部、 东部或者西部。如图 12-9 所示。
j —
—
I
例如：
南
!
frame,add(component, Bo「de「Layout.SOUTH);
i
P
先放置边缘组件，剩余的可用空间由中间组件占据。当
图 12-9
边框布局
容器被缩放时，边缘组件的尺寸不会改变， 而中部组件的大
小会发生变化。在添加组件时可以指定 BorderLayout 类中的 CENTER、 NORTH、 SOUTH、EAST
和 WEST 常量。并非需要占用所有的位置，如果没有提供任何值， 系统默认为 CENTER。
0 注 释： BorderLayout 常 量 定 义 为 字 符 串。 例 如： BorderLayout.SOUTH 定 义 为 字 符 串
“
SOUTH”。很多程序员喜欢直接使用字符串， 因为这些字符串比较简短， 例如， frame,
add (component,“
SOUTH” ）。 然而， 如果字符串拼写有误， 编译器不会捕获错误。
与流布局不同，边框布局会扩展所有组件的尺寸以便填满可用空间（流布局将维持每个
组件的最佳尺寸)。当将一个按钮添加到容器中时会出现问题：
frame.add(yellowButton, BorderLayout.SOUTH); // don't
图 12-10 给出了执行上述语句的显示效果。 按钮扩展至填满框架的整个南部区域。而且，
如果再将另外一个按钮添加到南部区域， 就会取代第一个按钮。
解决这个问题的常见方法是使用另外一个面板（panel)。 例如， 如图 12-11 所示。屏幕底
部的三个按钮全部包含在一个面板中。这个面板被放置在内容窗格的南部。
北
西
中
东
南
图 12-9
边框布局
1, '_
蘇」
1
B|u* II
R*d
1
图 12-10
边框布局管理一个按钮
图 12-11
面板放置在框架的南部区域
要想得到这种配置效果， 首先需要创建一个新的 JPanel 对象，然后逐一将按钮添加到面
478
Java 核心技?
板中。面板的默认布局管理器是 HowLayout，这恰好符合我们的需求。随后使用在前面已经
看到的 add 方法将每个按钮添加到面板中。每个按钮的放置位置和尺寸完全处于 FlowLayout
布局管理器的控制之下。这意味着这些按钮将置于面板的中央，并且不会扩展至填满整个面
板区域。最后， 将这个面板添加到框架的内容窗格中。
3Panel panel = new ]Panel();
panel .add(yellowButton) ;
panel.add(blueButton);
panel.add(redButton);
frame.add(panel , BorderLayout.SOUTH);
边框布局管理器将会扩展面板大小， 直至填满整个南部区域。
[ >w|java.awt.BorderLayout 1.0
• BorderLayout()
• BorderLayout( int hgap, int vgap)
构造一个新的 BorderLayout 对象。
参数：hgap
以像素为单位的水平间距（如果为负值， 则强行重叠）
vgap
以像素为单位的垂直间距（如果为负值， 则强行重叠）
12.2.2
网格布局
网格布局像电子数据表一样， 按行列排列所有的组件。不过，它
的每个单元大小都是一样的。 图 12-12 显示的计算器程序就使用了网
格布局来排列计算器按钮。 当缩放窗口时， 计算器按钮将随之变大或
变小，但所有的按钮尺寸始终保持一致。
在网格布局对象的构造器中，需要指定行数和列数：
panel .setLayout(new CridLayout(4, 4)) ;
添加组件， 从第一行的第一列开始， 然后是第一行的第二列， 以此类推。
panel .add(new ]Button("l"));
panel.add(new ]Button("2"))；
程序清单 12-1 是计算器程序的源代码。这是一个常规的计算器， 而不像 Java 指南中所
提到的“ 逆波兰” 那样古怪。 在这个程序中， 在将组件添加到框架之后， 调用了 pack 方法。
这个方法使用所有组件的最佳大小来计算框架的高度和宽度。
当然， 极少有像计算器这样整齐的布局。 实际上， 在组织窗口的布局时小网格（通常只
有一行或者一列）比较有用。 例如， 如果想放置一行尺寸都一样的按钮，就可以将这些按钮
放置在一个面板里， 这个面板使用只有一行的网格布局进行管理。
程序清单 12-1
caiculator/CalculatorPanel.java
1 package calculator;
2
3 import java.awt.*;
1729.0
7
8
一
，'晃
■
4
'
6
1
2
一
,,
.i.
-
+
图 1 2-1 2
计 算 器
第 /2 章
Swing 用? 界面? 件
479
4 import java.awt.event.*;
5 import javax.swing.*;
6
7 /**
8
* A panel with calculator buttons and a result display.
9
*/
10 public class CalculatorPanel extends ]Panel
11 {
12
private ]Button display;
13
private ]Panel panel;
14
private double result;
is
private String lastCommand;
16
private boolean start;
17
18
public CalculatorPanel ()
19
{
20
setLayout(new BorderLayoutO) ;
21
n
result = 0;
23
lastCo_
and =
24
start = true;
2S
S 7
// add the display
display = new ]Button("0");
display.setEnabl ed(false);
add(display, Borde「Layout.NORTH) ;
ActionListener insert = new InsertActionO;
ActionListener command = new Co_
andAction();
// add the buttons in a 4 x 4 grid
panel = new ]Panel ()；
panel.setLayout(new GridLayout(4，4));
addButton(n7", insert);
addButton("8", insert);
addButton("9"，insert) ;
addButton(7", command);
addButton("4", insert);
addButton("5", insert);
addButton("6", insert);
addButton("*", command);
addButton('T, insert);
addButton("2", insert);
addButton("3", insert);
addButton("-
M , command):
addButtonfO", insert) ;
addButton(".", insert);
addButton("=", command);
addButton("+", command);
S8
add(panel , Borde「Layout.CENTER);
480
Java 核心技?
80
100
113
114
117
/**
* Adds a button to the center panel .
* @param label the button label
* @param listener the button listener
V
private void addButton(String label , ActionListener listener)
{
]Button button = new ]Button(1abel ) ;
button.addActi onLi stener(1istener);
panel .add(button) ;
Jirk
* This action inserts the button action string to the end of the display text.
V
private class InsertAction implements ActionListener
{
public void actionPerformed(ActionEvent event)
{
String input = event.getActionCommand() ;
if (start)
{
displ ay.setText(
_
•") ;
start = false;
}
display.setText(display_getText() + input);
* This action executes the command that the button action string denotes.
V
private class CommandAction implements ActionListener
{
public void actionPerformed(ActionEvent event)
{
String command = event.getActionCommand() ;
if (start)
{
if (command.equals("-"))
{
display.setText(command):
start = false;
}
else lastCo晒and = command;
}
else
{
calculate(Double.parseDouble(displ ay.getText()))；
1astCommand = command;
start = true;
}
第！
2 章
Swing 用?界面?件
481
118
/**
9
* Carries out the pending calculation.
i2o
* @param x the value to be accumulated with the prior result.
m
*/
122
public void calculate(double x)
123
{
124
if CastCommand.equals("+")) result += x;
us
else if (lastCommand.equals("-")) result -= x;
126
else if (lastCommand.equals("*")) result *= x;
127
else if (lastCommand.equals("/")5 result /= x;
us
else if (1astCommand.equals("=")) result = x;
129
display.setTextf" + result) ;
130
}
[AW] java.awt.GridLayout 1.0
• GridLayout(int rows, int columns)
• GridLayout(int rows, int columns, int hgap, int vgap)
构造一个新的 GridLayout 对象。rows 或者 columns 可以为零， 但不能同时为零， 指定
的每行或每列的组件数量可以任意的。
参数：rows
网格的行数
columns
网格的列数
hgap
以像素为单位的水平间距（如果为负值， 则强行重叠）
vgap
以像素为单位的垂直间距 （如果为负值，则强行重叠）
12.3
文本输入
现在终于可以开始介绍 Swing 用户界面组件了o 首先， 介绍具有用户输入和编辑文本功
能的组件。文本域（JTextField) 和文本区 （ JTextArea) 组件用于获取文本输人。文本域只能
接收单行文本的输人， 而文本区能够接收多行文本的输人。JPassword 也只能接收单行文本
的输人，但不会将输入的内容显示出来。
这三个类都继承于 JTextComponent 类。由于 JTextComponent 是一个抽象类，所以不能
够构造这个类的对象。另外，在 Java 中常会看到这种情况。在査看 API 文档时，发现自己正
在寻找的方法实际上来自父类 JTextComponent, 而不是来自派生类自身。例如，在一个文本
域和文本区内获取（get)、 设置（set) 文本的方法实际上都是 JTextComponent 类中的方法。
[»n] javax.swing.text.JTextComponent 1.2
• String getText()
• void setText(String text)
获取或设置文本组件中的文本。
• boolean isEditable()
482
Java 核心技?
•v o i d s e t E d i t a b e( b o o l e a n b)
获取或设置 editable 特性， 这个特性决定了用户是否可以编辑文本组件中的内容。
12.3.1
文本域
把文本域添加到窗口的常用办法是将它添加到面板或者其他容器中，这与添加按钮完全一样：
]Panel panel = new ]Panel();
JTextField textField = new JTextField("Default input", 20);
panel,add(textField);
这段代码将添加一个文本域， 同时通过传递字符串“
Default input” 进行初始化。构造
器的第二个参数设置了文本域的宽度。在这个示例中， 宽度值为 20“ 列”。但是，这里所说
的列不是一个精确的测量单位。一列就是在当前使用的字体下一个字符的宽度。 如果希望文
本域最多能够输人 n 个字符， 就应该把宽度设置为 n 列。在实际中， 这样做效果并不理想，
应该将最大输人长度再多设 1 ~ 2 个字符。列数只是给 AWT 设定首选（ preferred) 大小的一
个提示。如果布局管理器需要缩放这个文本域， 它会调整文本域的大小。在 JTextField 的构
造器中设定的宽度并不是用户能输人的字符个数的上限。 用户可以输入一个更长的字符串，
但是当文本长度超过文本域长度时输人就会滚动。用户通常不喜欢滚动文本域， 因此应该尽
量把文本域设置的宽一些。如果需要在运行时重新设置列数，可以使用 setColumns 方法。
提示： 使用 setColumns 方法改变了一个文本域的大小之后， 需要调用包含这个文本框的
容器的 revalidate 方法。
textField.setColumns(10);
panel,revalidate0;
revalidate 方法会重新计算容器内所有组件的大小， 并且对它们重新进行布局。 调用
revalidate 方法以后， 布局管理器会重新设置容器的大小， 然后就可以看到改变尺寸后的
文本域了。
revalidate 方法是 JComponent 类中的方法。 它并不是马上就改变组件大小， 而是给
这个组件加一个需要改变大小的标记。这样就避免了多个组件改变大小时带来的重复计
算。但是， 如果想重新计算一个 JFrame 中的所有组件， 就必须调用 validate 方法
JFrame 没有扩展 JComponent。
通常情况下， 希望用户在文本域中键入文本（或者编辑已经存在的文本)。文本域一般初
始为空白。只要不为 JTextField 构造器提供字符串参数，就可以构造一个空白文本域：
JTextField textField = new JTextField(20);
可以在任何时候调用 setText 方法改变文本域中的内容。 这个方法是从前面提到的
JTextComponent 中继承而来的。 例如：
textField.setTextCHello!");
并且， 在前面已经提到， 可以调用 getText 方法来获取用户键人的文本。这个方法返回
$12$
Swing 用?界面?件
483
用户输人的文本。 如果想要将 getText 方法返回的文本域中的内容的前后空格去掉， 就应该
调用 trim 方法：
String text = textField.getText().tri»0；
如果想要改变显示文本的字体， 就调用 setFont 方法。
IAW] javax.swing.JTextField 1.2
•JTextF1eld(1nt cols)
构造一个给定列数的空 JTextField 对象。
•JTextField(String text , int cols)
构造一个给定列数、 给定初始字符串的 JTextField 对象。
•int getColumns( )
•void setColumns( int cols )
获取或设置文本域使用的列数。
[API) javax.swing.JComponent 1.2
•void revalidate( )
重新计算组件的位置和大小。
•void setFont( Font f )
设置组件的字体。
[w]java.awt.Component 1.0
•void validate( )
重新计算组件的位置和大小。如果组件是容器， 容器中包含的所有组件的位置和大小
也被重新计算。
•Font getFont( )
获取组件的字体。
12.3.2
标签和标签组件
标签是容纳文本的组件， 它们没有任何的修饰（例如没有边缘 )， 也不能响应用户输入。
可以利用标签标识组件。 例如： 与按钮不同， 文本域没有标识它们的标签
要想用标识符标
识这种不带标签的组件， 应该
1 ) 用相应的文本构造一个 JLabel 组件。
2 ) 将标签组件放置在距离需要标识的组件足够近的地方， 以便用户可以知道标签所标
识的组件。
儿abel 的构造器允许指定初始文本和图标， 也可以选择内容的排列方式。可以用 Swing
Constants 接口中的常量来指定排列方式。在这个接口中定义了几个很有用的常量， 如 LEFT、
RIGHT、CENTER、NORTH、 EAST 等。JLabel 是实现这个接口的一个 Swing 类。因此， 可
484
Java 核心技?
以指定右对齐标签：
]Label label = new ]Label("User name: ", SwingConstants.RIGHT);
或者
3Label label = new 3Label("User name: ", 3Label.RIGHT);
利用 setText 和 setlcon 方法可以在运行期间设置标签的文本和图标。
提示： 可以在按钮、 标签和菜单项上使用无格式文本或 HTML 文本。 我们不推荐在按钮
上使用 HTML 文本—这样会影响观感。 但是 HTML 文本在标签中是非常有效的。 只要
简单地将标签字符串放置在 <html>...</html> 中即可：
label = new ]Label("<htmlxb>Required</b> entry:</html>°);
需要说明的是包含 HTML 标签的第一个组件需要延迟一段时间才能显示出来， 这是
因为需要加载相当复杂的 HTML 显示代码。
与其他组件一样，标签也可以放置在容器中。这就是说，可以利用前面介绍的技巧将标
签放置在任何需要的地方。
[«H|javax.swing.JLabel 1.2
•JLabel (String text )
•JLabel (Icon icon)
•JLabel (String text, int align)
•JLabel (String text, Icon Icon, int align)
构造一个标签。
参数： text
标签中的文本
icon
标签中的图标
align
一个 SwingConstants 的常量 LEFT (默认)、CENTER 或者 WGHT
•String getText( )
•void setText(String text )
获取或设置标签的文本。
•Icon getIcon( )
•void setIcon(Icon Icon)
获取或设置标签的图标。
12.3.3
密码域
密码域是一种特殊类型的文本域。为了避免有不良企图的人看到密码， 用户输入的字符
不显示出来。 每个输人的字符都用回显字符 （ echo character) 表示， 典型的回显字符是星号
(*)。Swing 提供了 JPasswordField 类来实现这样的文本域。
密码域是另一个应用模型 - 视图 - 控制器体系模式的例子。密码域采用与常规的文本域
第 12 章
Swing 用?界面?件
485
相同的模型来存储数据， 但是，它的视图却改为显示回显字符，而不是实际的字符。
!«w| javax.swing.JPasswordField 1.2
•
JPasswordField(String text, int columns)
构造一个新的密码域对象。
•
void setEchoChar(char echo)
为密码域设置回显字符。注意： 独特的观感可以选择自己的回显字符。0 表示重新设
置为默认的回显字符。
• char[ ] getPassworci()
返回密码域中的文本。 为了安全起见， 在使用之后应该覆写返回的数组内容 （密码并
不是以 String 的形式返回，这是因为字符串在被垃圾回收器回收之前会一直驻留在虚
拟机中）。
12.3.4
文本区
有时， 用户的输人超过一行。 正像前面提到的， 需要使用 JTextArea 组件来接收这样的
输入。当在程序中放置一个文本区组件时， 用户就可以输
人多行文本，并用 ENTER 键换行。每行都以一个“
\n” 结
尾。图 12-13 显示了一个工作的文本区。
在 JTextArea 组件的构造器中，可以指定文本区的行数
和列数。 例如：
textArea = new JTextArea(8, 40);// 8 lines of 40 columns each
与文本域一样。 出于稳妥的考虑， 参数 columns 应该设置
得大一些。另外，用户并不受限于输人指定的行数和列数。
当输人过长时，文本会滚动。还可以用 setColumns 方法改
变列数， 用 setRows 方法改变行数。这些数值只是首选大
小一布局管理器可能会对文本区进行缩放。
如果文本区的文本超出显示的范围， 那么剩下的文本就会被剪裁掉。可以通过开启换行
特性来避免裁剪过长的行：
textArea.setLineWrap(true):// long lines are wrapped
换行只是视觉效果；文档中的文本没有改变，在文本中并没有插入“ \n” 字符。
12.3.5
滚动窗格
在 Swing 中， 文本区没有滚动条。 如果需要滚动条， 可以将文本区插人到滚动窗格
(scroll pane) 中。
textArea = new JTextArea(8, 40);
JScrollPane scrollPane = new JScratlPane(textArea):
User name:|
Password: [
User name: troosevelt Pas
troosevett
••••••••••
sword jabberwock
1
图 1 2-1 3
文 本 组 件
486
Java 核心技?
现在滚动窗格管理文本区的视图。 如果文本超出了文本区可以显示的范围， 滚动条就会
自动地出现， 并且在删除部分文本后， 当文本能够显示在文本区范围内时， 滚动条会再次自
动地消失。滚动是由滚动窗格内部处理的， 编写程序时无需处理滚动事件。
这是一种为任意组件添加滚动功能的通用机制， 而不是文本区特有的。 也就是说， 要想
为组件添加滚动条， 只需将它们放人一个滚动窗格中即可。
程序清单 12-2 展示了各种文本组件。这个程序只是简单地显示了一个文本域、 一个密码
域和一个带滚动条的文本区。 文本域和密码域都使用了标签。点击“
Insert” 会将组件中的
内容插入到文本区中。
0 注释： JTextArea 组件只显示无格式的文本， 没有特殊字体或者格式设置。 如果想要显示
格式化文本（如 HTML ), 就需要使用 JEditorPane 类。在卷 II 将详细讨论。
程序清单 12-2
text/TextComponentFrame.java
1 package text;
2
3 import java.awt.Borde「Layout;
4 import java.awt.GridLayout;
5
6 import javax.swing.]Button;
7 import javax.swing.]Frame;
8 import javax.swing.]Label;
9 import javax.swing.]Panel;
10 import javax.swing.]PasswordField;
11 import javax.swing.]ScrollPane;
12 import javax.swingJTextArea;
13 import javax.swingJTextField;
14 import :javax.swing.SwingConstants;
is
16 /**
17
* A frame with sample text components.
18
*/
19 public class TextComponentFrame extends ]Frame
20 {
21
public static final int TEXTAREA.ROWS = 8;
22
public static final int TEXTAREA.COLUMNS = 20;
23
24
public TextComponentFrame()
25
{
26
]TextField textField = new JTextField();
27
]PasswordField passwordField = new ]Passwordseld();
28
29
3Panel northPanel = new ]Panel();
30
northPanel.setLayout(new CridLayout(2, 2));
31
northPanel.add(new ]Label("User name: ", SwingConstants.RIGHT));
32
northPanel.add(textField);
33
northPanel _ add(new ]LabelfPassword:
SwingConstants.RIGHT));
34
northPanel.add(passwordField);
add(northPanel, BorderLayout.NORTH);
]TextA「ea textArea = new 丌extA「ea(TEXTAREA_ROWS, TEXTAREA_COLUMNS);
第 A2 章
Swing 用 ? 界 面 ? 件
4 8 7
JScrollPane scrollPane = new JScrollPane(textArea);
add(scrollPane, BorderLayout.CENTER);
// add button to append text into the text
' area
]Panel southPanel = new JPanel();
]Button insertButton = new ]Button("Insert");
southPanel.add(insertButton);
insertButton.addActionListener(event ->
textArea.append("User name:
11 + textField.getText() + " Password:
+ new String(passwordField.getPassword()) + "\n"))；
add(southPanel, BorderLayout.SOUTH);
pack();
| API] javax.swing.JTextArea 1.2
•JTextArea( )
•JTextArea(int rows, int cols )
•JTextArea(String text, int rows, int cols )
构造一个新的文本区对象。
•void setColumns(int cols )
设置文本区应该使用的首选列数。
•void setRows(int rows )
设置文本区应该使用的首选行数。
•void append( String newText )
将给定的文本追加到文本区中已有文本的尾部。
•void setLineWrap(boolean wrap )
打开或关闭换行。
•void setWrapStyleWord(boolean word)
如果 word 是 true, 超长的行会在字边框处换行。 如果为 false, 超长的行被截断而不
考虑字边框。
•void SETTABSIZG(int c )
将制表符 （tab stop ) 设置为 c 列。 注意， 制表符不会被转化为空格， 但可以让文本对
齐到下一个制表符处。
Unj javax.swing.JScroliPane 1.2
•JScrol 1Pane( Component c )
创建一个滚动窗格， 用来显示指定组件的内容。 当组件内容超过显示范围时， 滚动条
会自动地出现。
488
Java 核心技?
12.4
选择组件
前面已经讲述了如何获取用户输入的文本。然而， 在很多情况下，可能更加愿意给用户
几种选项，而不让用户在文本组件中输人数据。使用一组按钮或者选项列表让用户做出选择
(这样也免去了检查错误的麻烦 )。在本节中，将介绍如何编写程序来实现复选框、 单选按钮、
选项列表以及滑块。
12.4.1
复选框
如果想要接收的输人只是“ 是” 或“ 非”， 就可以使用复选框组件。复选框自动地带有标
识标签。用户通过点击某个复选框来选择相应的选项， 再点击则取消选取。 当复选框获得焦
点时， 用户也可以通过按空格键来切换选择。
图 12-14 所示的程序中有两个复选框， 其中一个用于打
开或关闭字体倾斜属性， 而另一个用于控制加粗属性。注
意， 第二个复选框有焦点， 这一点可以由它周围的矩形框
看出。只要用户点击某个复选框， 程序就会刷新屏幕以便
应用新的字体属性。
复选框需要一个紧邻它的标签来说明其用途。 在构造
器中指定标签文本。
bold = new ]CheckBox("Bold");
可以使用 setSelected 方法来选定或取消选定复选框。 例如：
□Bold EjiuH
图 1 2-1 4
复 选 框
bold,setSelected(true);
isSelected 方法将返回每个复选框的当前状态。 如果没有选取则为 false, 否则为 true。
当用户点击复选框时将触发一个动作事件。通常， 可以为复选框设置一个动作监听器。
在下面程序中， 两个复选框使用了同一个动作监听器。
ActionListener listener = . . .
bold.addActionListenerflistener);
italic.addActionListener(1istener);
actionPerformed 方法查询 bold 和 italic 两个复选框的状态，并且把面板中的字体设置为
常规、 加粗、 倾斜或者粗斜体。
ActionListener listener = event -> {
int mode = 0;
if (bold.isSelectedO) mode += Font.BOLD;
if (italic.isSelectedO) mode += Font.ITALIC;
label.setFont(new Font(Font.SERIF, mode, FONTSIZE));
}；
程序清单 12-3 给出了复选框例子的全部代码。
第 /2 章
Swing 用?界面?件
489
程序清单 12-3
checkBox/CheckBoxTest.java
1 package checkBox;
2
3 import java.awt.*;
4 import java.awt.event.*;
5 import javax.swing.*;
7 /**
8
* A frame with a sample text label and check boxes for selecting font
9
* attributes.
10
*/
11 public class CheckBoxFrame extends ]Frame
12
{
13
private 3Label label;
14
private JCheckBox bold;
is
private ]CheckBox italic;
16
private static final int FONTSIZE = 24;
17
is
public CheckBoxFrameO
19
{
20
// add the sample text label
21
22
label = new ]Label("The quick brown fox jumps over the lazy dog.")；
23
label.setFont(new Font("Serif", Font.BOLD, FONTSIZE));
24
add(label, Bo「de「Layout.CENTER);
25
26
// this listener sets the font attribute of
27
// the label to the check box state
29
ActionListener listener = event -> {
30
int mode = 0;
31
if (bold.isSelectedO) mode += Font.BOLD;
32
if (italic.isSelectedO) mode += Font.ITALIC;
33
label.setFont(new Font("Serif", mode, FONTSIZE));
34
}；
// add the check boxes
]Panel buttonPanel = new ]Panel ();
bold = new ]CheckBox("Bo1d");
bold.addActionLi stener(listener);
bold.setSelected(true);
buttonPanel .add(bold);
45
italic = new ]CheckBox("Italic");
46
italic.addActionListene「(1istener);
47
buttonPanel.add(italic);
48
49
add(buttonPanel, Borde「Layout.SOUTH);
so
pack();
490
Java 核心技?
|API] javax.swing.JCheckBox 1.2
•JCheckBox(String label )
•JCheckBox( String label , Icon icon)
构造一个复选框， 初始没有被选择。
•JCheckBox(String label , boolean state)
用给定的标签和初始化状态构造一个复选框。
•boolean isSelected ( )
•void setSelected(boolean state)
获取或设置复选框的选择状态。
12.4.2
单选钮
在前一个例子中， 对于两个复选框， 用户既可以选择一个、 两个，也可以两个都不选。
在很多情况下，我们需要用户只选择几个选项当中的一个。当用户选择另一项的时候， 前一
项就自动地取消选择。这样一组选框通常称为单选钮组（Radio Button Group), 这是因为这些
按钮的工作很像收音机上的电台选择按钮。当按下一个按钮时，前一个按下的按钮就会自动
弹起。图 12-15 给出了一个典型的例子。这里允许用户在多个选择中选择字体的大小，即小、
中、 大和超大，但是，每次用户只能选择一个。
在 Swing 中，实现单选钮组非常简单。为单选钮组构造一个 ButtonGroup 的对象。然后，
再将 JRadioButton 类型的对象添加到按钮组中。按钮组负责在新按钮被按下时，取消前一个
被按下的按钮的选择状态。
ButtonGroup group = new ButtonGroupO;
JRadioButton smallButton = new ]RadioButton("Small", false);
group.add(smallButton);
JRadioButton mediumButton = new ]RadioButton("Medium", true);
group,add(mediumButton);
图 12-15
单选钮组
The quick brown fox...
CSman C Medium O Large
构造器的第二个参数为 true 表明这个按钮初始状态是被选择，其他按钮构造器的这个参
数为 false。注意， 按钮组仅仅控制按钮的行为， 如果想把这些按钮组织在一起布局， 需要把
它们添加到容器中， 如 JPanel。
如果再看一下图 12-14 和图 12-15 则会发现，单选钮与复选框的外观是不一样的。复选
框为正方形， 并且如果被选择， 这个正方形中会出现一个对钩的符号。单选钮是圆形，选择
以后圈内出现一个圆点。
单选钮的事件通知机制与其他按钮一样。当用户点击一个单选钮时， 这个按钮将产生一
个动作事件。在示例中，定义了一个动作监听器用来把字体大小设置为特定值：
ActionListener listener = event ->
label.setFont(new FontC'Serif", Font.PLAIN, size));
襄 /2 聿
Swing 用?界面?件
491
用这个监听器与复选框中的监听器做一个对比。每个单选钮都对应一个不同的监听器对
象。 每个监听器都非常清楚所要做的事情—把字体尺寸设置为一个特定值。在复选框示例
中， 使用的是一种不同的方法， 两个复选框共享一个动作监听器。这个监听器调用一个方法
来检查两个复选框的当前状态。
对于单选钮可以使用同一个方法吗？ 可以试一下使用一个监听器来计算尺寸， 如：
if (smallButton.isSelectedO) size = 8;
else if (mediumButton.isSelected()) size = 12;
然而， 更愿意使用各自独立的动作监听器， 因为这样可以将尺寸值与按钮紧密地绑定在
一起。
0 注释： 如果有一组单选钮， 并知道它们之中只选择了一个。 要是能够不查询组内所有
的按钮就可以很快地知道哪个按钮被选择的话就好了。 由 于 ButtonGroup 对象控制着
所有的按钮， 所以如果这个对象能够给出被选择的按钮的引用就方便多了。 事实上，
ButtonGroup 类中有一个 getSelection 方法， 但是这个方法并不返回被选择的单选钮， 而
是返回附加在那个按钮上的模型 ButtonModel 的引用。 对于我们来说，ButtonModel 中
的方法没有什么实际的应用价值。ButtonModel 接 口 从 ItemSelectable 接口继承了一个
getSelectedObject 方法， 但是这个方法没有用， 它返回 null。getActionCommand 方法看
起来似乎可用， 这是因为一个单选钮的“ 动作命令” 是它的文本标签， 但是它的模型的
动作命令是 null。只有在通过 setActionCommand 命令明确地为所有单选钮设定动作命令
后， 才能够通过调用方法 buttonGroup.getSelection().getActionCommand() 获得当前选择
的按钮的动作命令。
程序清单 12*4 是一个用于选择字体大小的完整程序， 它演示了单选钮的工作过程。
程序清单 12-4
radioButton/RadioButtonFrame.java
1 package radioButton;
2
3 import java.awt.*;
4 import java.awt.event.*;
5 import javax.swing.*;
6
7 /**
8
* A frame with a sample text label and radio buttons for selecting font sizes.
«
*/
IO public class RadioButtonPrame extends ]Frame
n {
12
private ]Panel buttonPanel:
13
private ButtonGro叩 group;
14
private ]Label label;
is
private static final int DEFAULT.SIZE = 36;
16
17
public RadioButtonFrame()
// add the sample text label
492
Java 核心技?
20
44
57
label = new]Label("The quick brown fox jumps over the lazy dog.");
label.setFont(new Font("Serif", Font.PLAIN, DEFAULT_SIZE));
add(label , BorderLayout.CENTER);
// add the radio buttons
buttonPanel = new]Panel();
group = new BtittonGroupO；
addRadioButton("Small", 8);
addRadioButton("Medium", 12);
addRadioButton("Large", 18);
addRadioButton("Extra large", 36);
add(buttonPanel , BorderLayout.SOUTH);
pack();
* Adds a radio button that sets the font size of the sample text.
* @param name the string to appear on the button
* @param size the font size that this button sets
V
public void addRadioButton(String name, int size)
{
boolean selected = size == DEFAULT_SIZE;
JRadioButton button = new ]RadioButton(name, selected);
group.add(button);
buttonPanel .add(button);
// this listener sets the label font size
ActionListener listener = event -> label.setFont(new FontC'Serif", Font.PLAIN, size));
button.addActionListener(listener);
| AW] javax.swing.JRadioButton 1.2
• JRadioButton(String label , Icon icon)
构造一个单选钮， 初始没有被选择。
• JRadioButton(String label , boolean state)
用给定的标签和初始状态构造一个单选钮。
iAPi} javax.swing.ButtonGroup 1.2
• void add(AbstractButton b)
将按钮添加到组中。
• ButtonModel getSelection()
返回被选择的按钮的按钮模型。
第 J2 章
Swing 用?界面?件
493
API|javax.swing.ButtonModel 1.2
• String getActionCommand()
返回按钮模型的动作命令。
[*H] javax.swing.AbstractButton 1.2
• void setActionCommand(String s)
设置按钮及其模型的动作命令。
12.4.3
边框
如果在一个窗口中有多组单选按钮， 就需要用可视化的形式指明哪些按钮属于同一组。
Swing 提供了一组很有用的边框（ borders) 来解决这个问题。可以在任何继承了 JComponent
的组件上应用边框。 最常用的用途是在一个面板周围放置一个边框， 然后用其他用户界面元
素 （如单选钮）填充面板。
有几种不同的边框可供选择，但是使用它们的步骤完全一样。
1 ) 调用 BorderFactory 的静态方法创建边框。下面是几种可选的风格（如图 12-16 所示):
•凹斜面
•凸斜面
•蚀刻
參 直 线
•蒙版
•空（只是在组件外围创建一些空白空间）
O Lowered bevel O Raised bevtl O Eldictf O Lbic
O Empty
图 12-16
测试边框类型
2 ) 如果愿意的话， 可以给边框添加标题， 具体的实现方法是将边框传递给
BroderFactory.createTitledBorder。
3 ) 如果确实想把一切凸显出来， 可以调用下列方法将几种边框组合起来使用：
BorderFactory.createCompoundBorder。
4 ) 调用 JComponent 类中 setBorder 方法将结果边框添加到组件中。
例如，下面代码说明了如何把一个带有标题的蚀刻边框添加到一个面板上：
Border etched = BorderFactory.createEtchedBorder() ;
Border titled = BorderFactory.createTitledBorder(etched, "A Title")；
panel.setBorder(titled);
494
Java 核心技?
运行程序清单 12-5 中的程序可以看到各种边框的外观。
不同的边框有不同的用于设置边框的宽度和颜色的选项。 详情请参看 API 注释。 偏
爱使用边框的人都很欣赏这一点， SoftBevelBorder 类用于构造具有柔和拐角的斜面边
框， LineBorder 类也能够构造圆拐角。 这些边框只能通过类中的某个构造器构造， 而没有
BorderFactory 方法。
程序清单 12-5
border/BorderFrame.java
1 package border;
2
3 import java.awt.*;
4 import javax.swing.*;
5 import javax.swing.border.*;
7
/**
8
* A frame with radio buttons to pick a border style.
9
*/
io public class BorderFrame extends ]Frame
{
12
private ]Panel demoPanel;
13
private ]Panel buttonPanel;
14
private ButtonCroup group;
16
public BorderFrameO
{
demoPanel = new ]Panel();
buttonPanel = new ]Panel ()；
group = new ButtonCroupO;
addRadioButton("Lowered bevel", BorderFactory.createLoweredBevelBorde「()）；
addRadioButton("Raised bevel", BorderFactory.createRaisedBevelBorderO);
addRadioButton("Etched", BorderFactory.createEtchedBorderO);
addRadioButton("Line", BorderFactory.createLineBorder(Color.BLUE));
addRadioButton("Matte", BorderFactory.createMatteBorder(10, 10, 10, 10, Color.BLUE)) ;
addRadioButton("Empty", BorderFactory.createEmptyBorderO);
Border etched = BorderFactory.createEtchedBorder();
Border titled = BorderFactory.createTitledBorder(etched, "Border types")；
buttonPanel.setBorder(titled);
setLayout(new GridLayout(2, 1));
add(buttonPanel);
add(demoPanel);
packQ;
39
public void addRadioButton(String buttonName, Border b)
40
{
41
3RadioButton button = new JRadioButton(buttonName);
42
button.addActionListener(event -> demoPanel.setBorder(b));
43
group.add(button);
44
buttonPanel.add(button);
第 J2 章
Swing 用?界面?件
495
AW] javax.swing.BorderFactory 1.2
• s tatic Border createLineBorder( Co1or color )
參 s tatic Border createLineBorder(Color col or, i n t thickness )
创建一个简单的直线边框。
• s t a t i c
MatteBorder
createMatteBorder ( i n t
top, in t
l e f t, in t
bottom, i n t r i g h t, Col or color )
參 s t a t i c
MatteBorder
createMatteBorder ( i n t
top, in t
l e f t, in t
bottom, i n t r i g h t, Icon tilelcon)
创建一个用 color 颜色或一个重复 （repeating ) 图标填充的粗的边框。
• static Border createEmptyBorder( )
• static Border createEmptyBorder( i n t top, i n t l e f t, i n t bottom, i n t
r i g h t )
创建一个空边框。
參
static Border
static Border
static Border
static
Border
shadow)
createEtchedBorder( )
createEtchedBorder( Col or highlight,
createEtchedBorder( i n t type)
createEtchedBorder( i n t
type,
Col or
Col or
shadow)
highlight,
Col or
创建一个具有 3D 效果的直线边框。
参数： highlight, shadow
用于 3D 效果的颜色
type
EtchedBorder.RAISED 和 EtchedBorder.LOWERED 之一
參 s ta ti c Border createBevelBorder ( i n t type)
參 s t a t i c
Border
createBevelBorder( i n t
type,
Col or
highlight,
Col or
shadow)
• static Border createLoweredBevelBorder( )
• static Border createRaisedBevelBorder( )
创建一个具有凹面或凸面效果的边框。
参数：type
BevelBorder.LOWERED 和 BevelBorder.RAISED 之一*
highlight, shadow
用于 3D 效果的颜色
參 static TitledBorder createTitledBorder( String t i t l e)
• static TitledBorder createTitledBorder( Border border )
• static TitledBorder createTitledBorder( Border border, String t i t l e)
• s t a t i c
TitledBorder
createTitledBorder ( Border
border,
S t r i n g
t i t l e, i n t j u s t i f i c a t i o n, I n t position)
• s t a t i c
TitledBorder
createTitledBorder ( Border
border,
S t r i n g
t i t l e, i n t j u s t i f i c a t i o n, i n t position, Font font )
496
Java 核心技?
static Tit edBorder
createTitledBorder(Border
border ,
String
title, int justification, int position, Font font, Color color)
创建一个具有给定特性的带标题的边框。
参数: title
border
justification
标题字符串
用标题装饰的边框
TitledBorder 常量 LEFT、 CENTER、 RIGHT、 LEADING、
TRAILING 或 DEFAULT_JUSTIFICATION ( left) 之一
TitledBorder 常量 ABOVE—TOP、TOP、BELOW—TOP、 ABOVE_
BOTTOM、 BOTTOM、 BELOW_BOTTOM 或 DEFAULT—
POSITION (top) 之一
标题的字体
标题的颜色
• static
CompoundBorder createCompoundBorder(Border
outsideBorder,
Border insideBorder)
将两个边框组合成一个新的边框。
position
font
color
[API|javax.swing,border.SoftBevelBorder 1.2
• SoftBevelBorder(int type)
參 SoftBevelBorder(int type, Color highlight, Color shadow)
创建一个带有柔和边角的斜面边框。
参数：type
BevelBorder.LOWERED 和 BevelBorder.RAISED 之一
highlight, shadow
用于 3D 效果的颜色
APij javax.swing.border.LineBorder 1.2
• public LineBorder(Co1or color, int thickness, boolean roundedCorners)
用指定的颜色和粗细创建一个直线边框。如果 roundedCorners 为 true， 则边框有圆角。
Uw|javax.swing.JComponent 1.2
• void setBorder(Border border)
设置这个组件的边框。
12.4.4
组合框
如果有多个选择项， 使用单选按钮就不太适宜了， 其
原因是占据的屏幕空间太大。 这时就可以选择组合框。 当
用户点击这个组件时， 选择列表就会下拉出来， 用户可以
从中选择一项（见图 12-17 )。
The quick brown fox jumps over the lazy dog.
SansSerif
■■■■■■■
Monospaced
Dialog
Pialoglnput
如果下拉列表框被设置成可编辑 （ editable ), 就可以
图 12-17
组合框
襄 A2 章
Swing 用 ? 界 面 ? 件
4 9 7
像编辑文本一样编辑当前的选项内容。鉴于这个原因， 这种组件被称为组合框（ combo box),
它将文本域的灵活性与一组预定义的选项组合起来。JComboBox 类提供了组合框的组件。
在 Java SE 7 中，JComboBox 类是一个泛型类。 例如，JComboBox<String> 包含 String
类型的对象， JComboBox<Integer> 包含整数。
调用 setEditable 方法可以让组合框可编辑。 注意， 编辑只会影响当前项， 而不会改变列
表内容。
可以调用 getSelectedltem 方法获取当前的选项， 如果组合框是可编辑的， 当前选项则是
可以编辑的。不过，对于可编辑组合框， 其中的选项可以是任何类型，这取决于编辑器（即
由编辑器获取用户输人并将结果转换为一个对象 )。关于编辑器的讨论请参见卷 n 中的第 6
章。 如果你的组合框不是可编辑的， 最好调用
combo.getltemAt(combo.getSelectedlndex())
这会为所选选项提供正确的类型。
在示例程序中， 用户可以从字体歹!J表 （Serif, SansSerif，Monospaced 等）中选择一种字体，
用户也可以键人其他的字体。
可以调用 addltem 方法增加选项。在示例程序中， 只在构造器中调用了 addltem 方法，
实际上，可以在任何地方调用它。
]ComboBox<String> faceCombo = new JConboBoxoO；
faceCombo.addItem(.
_
Serif");
faceCombo.addltemC'SansSerif");
这个方法将字符串添加到列表的尾部。可以利用 insertltemAt 方法在列表的任何位置插
人一个新选项：
faceCombo.insertltemAt("Monospaced", 0); // add at the beginning
可以增加任何类型的选项，组合框可以调用每个选项的 toString 方法显示其内容。
如果需要在运行时删除某些选项，可以使用 removeltem 或者 removeltemAt 方法，使用
哪个方法将取决于参数提供的是想要删除的选项内容， 还是选项位置。
faceCombo.removeltem("Monospaced");
faceCombo.removeltemAt(0): // remove first item
调用 removeAllltems 方法将立即移除所有的选项。
提示： 如果需要往组合框中添加大量的选项，addltem 方法的性能就显得很差了 n 取 而 代
之的是构造一个 DefaultComboBoxModel, 并调用 addElement 方法进行加载， 然后再调
用 JComboBox 中的 setModel 方法。
当用户从组合框中选择一个选项时，组合框就将产生一个动作事件。为了判断哪个选
项被选择， 可以通过事件参数调用 getSource 方法来得到发送事件的组合框引用， 接着调用
getSelectedltem 方法获取当前选择的选项。需要把这个方法的返回值转化为相应的类型，通
常是 String 型。
498
Java 核心技?
ActionListener listener = event ->
label .setFont(new Font(
faceCoibo.getltenAt(faceConbo.setSelectedlndex()),
Font.PLAIN,
DEFAULT_SIZE));
程序清单 12-6 给出了完整的代码。
0 注释： 如果希望持久地显示列表， 而不是下拉列表， 就应该使用 _iList 组件。 在卷 n 的第
6 章中将介绍 JList。
程序清单 12-6
comboBox/ComboBoxFrame.java
1 package comboBox;
2
3 import :java.awt.BorderLayout;
4
import java.awt.Font;
5
6 import javax.swing.]ComboBox;
7
import javax. swing.]Frame;
8
import javax. swing.]Label ;
9
import javax.swing.]Panel ;
/**
12
* A frame with a sample text label and a combo box for selecting font faces.
13
V
14
public class ComboBoxFrame extends 3Frame
is {
16
private JComboBox<String> faceCombo;
17
private 3Label label;
18
private static final int DEFAULT.SIZE = 24;
public ComboBoxFrameO
{
// add the sample text label
label = new ]Label ("The quick brown fox jumps over the lazy dog.")；
label .setPont(new FontCSerif , Font.PLAIN, DEFAULT.SEE));
add(1abel， Borde「Layout.CENTER) ;
// make a combo box and add face names
faceCombo = new 3ComboBox<>();
faceCombo.addltern("Serif") ;
faceCombo.addItem("SansSerif")；
faceCombo.addItem("Monospaced");
faceCombo.addltem("Dialog") ;
faceCombo.addltem("Dialoglnput");
// the combo box listener changes the label font to the selected face name
faceCombo.addActionListener(event ->
label .setFont(
new Font(faceCombo.getltemAt(faceCombo.getSelectedlndex()) ,
Font.PLAIN, DEFAULT.SIZE)))；
第 口 章
Swing 用 ? 界 面 ? 件
4 9 9
II add combo box to a panel at the frame' s southern border
]Panel comboPanel = new ]Panel();
comboPanel.add(faceCombo);
add(comboPanel, BorderLayout.SOUTH);
pack();
[API) javax.swing.JComboBox 1.2
• boolean isEditable( )
• void setEditable( boo1ean b )
获取或设置组合框的可编辑特性。
• void addItem( Object item)
把一个选项添加到选项列表中。
• void insertltemAtCObject item, int index )
将一个选项添加到选项列表的指定位置。
• void removeItem( Object item)
从选项列表中删除一个选项。
• void removeItemAt( int index )
删除指定位置的选项。
• void removeAl 1 Items( )
从选项列表中删除所有选项。
• Object getSelectedltem( )
返回当前选择的选项。
12.4.5
滑动条
组合框可以让用户从一组离散值中进行选择。 滑动条允
许进行连续值的选择， 例如， 从 1 ~ 100 之间选择任意数值。
通常， 可以使用下列方式构造滑动条：
JSlider slider = new ]Slide「(min, max, initialValue) ;
如果省略最小值、 最大值和初始值， 其默认值分别为 0、
100 和 50。
或者如果需要垂直滑动条， 可以按照下列方式调用构造器：
]Slider slider = new ]SIider(SwingConstants.VERTICAL, min, max, initialValue);
这些构造器构造了一个无格式的滑动条， 如图 12-18 最
上面的滑动条所示。下面看一下如何为滑动条添加装饰。
当用户滑动滑动条时， 滑动条的值就会在最小值和最大
= =
40
6丨
Custom labels
_
0圃園圔□
50
图 12-18
滑动条
500
Java 核心技?
值之间变化。 当值发生变化时， ChangeEvent 就会发送给所有变化的监听器。 为了得到这些
改变的通知，需要调用 addChangeListener 方法并且安装一个实现了 ChangeListener 接口的对
象。这个接口只有一个方法 StateChanged。在这个方法中，可以获取滑动条的当前值：
ChangeListener listener = event -> {
]Slider slider = (]Slider) event.getSource()；
int value - slider.getValueO;
}; …
可以通过显示标尺（tick ) 对滑动条进行修饰。 例如，在示例程序中， 第二个滑动条使用
了下面的设置：
slider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
上述滑动条在每 20 个单位的位置显示一个大标尺标记， 每 5 个单位的位置显示一个小
标尺标记。所谓单位是指滑动条值， 而不是像素。
这些代码只设置了标尺标记， 要想将它们显示出来， 还需要调用：
slider.setPaintTicks(true);
大标尺和小标尺标记是相互独立的。 例如， 可以每 20 个单位设置一个大标尺标记， 同
时每 7 个单位设置一个小标尺标记， 但是这样设置， 滑动条看起来会显得非常凌乱。
可以强制滑动条对齐标尺。这样一来， 只要用户完成拖放滑动条的操作， 滑动条就会立
即自动地移到最接近的标尺处。 激活这种操作方式需要调用：
slider.set5napToTicks(true);
0 注释：“ 对齐标尺” 的行为与想象的工作过程并不太一样。在滑动条真正对齐之前， 改变
监听器报告的滑动条值并不是对应的标尺值。 如果点击了滑动条附近， 滑动条将会向点
击的方向移动一小段距离，“ 对夺标尺” 的滑块并不移动到下一个标尺处。
可以调用下列方法为大标尺添加标尺标记标签（tick mark labels)：
slider.setPaintLabels(true);
例如， 对于一个范围为 0 到 100 的滑动条， 如果大标尺的间距是 20, 每个大标尺的标签
就应该分别是 0、20、40、 60、 80 和 100。
还可以提供其他形式的标尺标记， 如字符串或者图标（见图 12-18 )。 这样做有些烦
琐。 首先需要填充一个键为 Integer 类型且值为 Component 类型的散列表。 然后再调用
setLabelTable 方法，组件就会放置在标尺标记处。通常组件使用的是几此以对象。 下面代码
说明了如何将标尺标签设置为 A、 B、 C、D、 E 和 F。
Hashtab1e<Integer, Component〉labelTable = new Hashtab1e<Integer, Components);
，
labelTable.put(0, new ]Label("A"));
labelTable.put(20, new 儿abel("B"));
labelTable.putdOO, new 儿abel("F”)）；
slider,setLabelTable(labelTable):
第！
2 章
Swing 用 ? 界 面 ? 件
5 0 1
关于散列表的详细介绍， 参考第 9 章。
程序清单 12-7 显示了如何创建用图标作为标尺标签的滑动条。
提示： 如果标尺的标记或者标签不显示， 请检查一下是否调用了 setPaintTicks(true) 和
setPaintLabels(true)。
在图 12-18 中， 第 4 个滑动条没有轨迹。要想隐藏滑动条移动的轨迹， 可以调用：
slider.setPaintTrack(false);
图 12-18 中第 5 个滑动条是逆向的， 调用下列方法可以实现这个效果：
slider.setlnverted(true);
示例程序演示了所有不同视觉效果的滑动条。 每个滑动条都安装了一个改变事件监听
器， 它负责把当前的滑动条值显示到框架底部的文本域中。
程序清单 12-7
slider/SliderFrame.java
1 package slider;
2
3 import java.awt.*;
4
import java.util.*;
5 import javax.swing.*;
6 import javax.swing.event.*;
7
8 /**
9
* A frame with many sliders and a text field to show slider values.
10
V
public class SliderFrame extends 3Frame
12 {
13
private ]Panel sliderPanel;
14
private JTextField textField;
is
private ChangeListener listener;
16
17
public Slide「Frame()
18
{
19
sliderPanel = new ]Panel0；
20
sliderPanel.setLayout(new CridBagLayoutO);
21
22
// common listener for all sliders
23
listener = event -> {
24
// update text field when the slider value changes
25
JSlider source = 0SIider) event.getSourceO；
26
textField.setText("" + source.getValueO);
27
}；
28
29
// add a plain slider
30
31
JSlider slider = new ]Slider();
32
addSlider(slider, "Plain");
33
34
// add a slider with major and minor ticks
35
slider = new ]Slider();
slider.setPaintTicks(true);
502
Java 核心技?
slider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
addSlider(slider, "Ticks");
// add a slider that snaps to ticks
slider = new DSliderQ；
slider.setPaintTicks(true);
slider.setSnapToTicks(true);
slider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
addSlider(slider, "Snap to ticks");
// add a slider with no track
slider = new ]Slide「(）；
slider.setPaintTicks(true);
slider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
slider.setPaintTrack(false);
addSlider(slider, "No track")；
// add an inverted slider
slider = new ]Slider();
slider.setPaintTicks(true);
slider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
slider.setlnverted(true):
addSlider(slider, "Inverted");
// add a slider with numeric labels
slider = new ]Slider();
slider.setPaintTicks(true);
slider.setPaintLabels(true);
siider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
addSlider(slider, "Labels");
// add a slider with alphabetic labels
slider = new ]Slider();
siider _ setPaintLabels(true);
slider.setPaintTicks(true);
slider.setMajorTickSpacing(20);
slider.setMinorTickSpacing(5);
Dictionaryclnteger, Component〉labelTable = new Hashtable<>();
1abelTable.put(0，new ]Label("A"));
1abelTable.put(20, new ]Label ("B"));
labelTable,put(40, new ]Label("C"));
labelTable.put(60, new ]Label ("D"));
labelTable,put(80, new ]Label("E"));
labelTable.put(100, new ]Label("F"))；
slider.setLabelTable(labelTabl e);
第 12 章
Swing 用 ? 界 面 ? 件
503
95
addSlider(slider, "Custom labels")；
96
97
// add a slider with icon labels
98
99
slider = new JSliderO；
100
slider.setPaintTicks(true);
101
slider,setPaintLabel s(true);
102
siider.setSnapToTicks(true):
103
slider.setMajorTickSpacing(20);
104
slider.setMinorTickSpacing(20);
10$
106
labelTable = new Hashtable<Integer, Component〉(）；
107
108
// add card images
109
no
1abelTable.put(0, new ]Label(new Imagelcon("nine.gif')));
in
labelTable,put(20, new ]Label (new ImagelconC'ten.gif")));
112
labelTable,put(40, new ]Label(new Imagelcon("jack,gif')));
113
labelTable,put(60, new ]Label(new Imagelcon("queen,gif')));
114
labelTable,put(80, new ]Label(new Imagelcon("king.gif")))；
us
labelTable,put(100, new ]Label(new Imagelcon("ace.gif••)));
116
117
slider,setLabelTable(1abelTable);
118
addSlider(slider, "Icon labels");
119
120
// add the text field that displays the slider value
121
122
textField = new ]TextField();
123
add(s1iderPane1, Borde「Layout.CENTER);
124
add(textField, BorderLayout.SOUTH);
12s
pack();
126
}
127
128
/**
129
* Adds a slider to the slider panel and hooks up the listener
130
* @param s the slider
131
*
@param description the slider description
132
*/
133
public void addSliderQSlider s, String description)
134
{
us
s.addChangeListenerflistener);
136
]Panel panel = new 3Panel();
137
panel.add(s);
138
panel.add(new ]Label(description));
139
panel.setAlignmentX(Component.LEFT_ALIGNMENT);
MO
CridBagConstraints gbc = new CridBagConstraintsO;
MI
gbc.gridy = sliderPanel.getComponentCountO；
142
gbc.anchor = CridBagConstraints.WEST;
MB
slide「Panel ,add(panel, gbc);
144
}
145 }
[AH} javax.swing.JSlider 1.2
• JSliderO
• JS1ider(int direction)
504
Java 核心技?
• JS1ider(int min, int max )
• JS1ider( int min, int max, int initialValue)
• JS1ider(int direction, int min, int max, int initialValue)
用给定的方向、最大值、 最小值和初始化值构造一个水平滑动条。
参数： direction
SwingConstants.HORIZONTAL 或 SwingConstants.
VERTICAL 之一。默认为水平。
min, max
滑动条的最大值、最小值。 默认值为 0 到 100。
initialValue
滑动条的初始化值。 默认值为 50。
• void setPaintTicks(boolean b)
如果 b 为 true, 显示标尺。
• void setMajorTickSpacing(int units)
• void setMinorTickSpacing(int units )
用给定的滑动条单位的倍数设置大标尺和小标尺。
• void setPaintLabels(boolean b)
如果 b 是 tme， 显示标尺标签。
• void setLabel Tab e(Dictionary table )
设置用于作为标尺标签的组件。 表中的每一个键 / 值对都采用 new Integer(value)/
component 的格式 0
• void setSnapToTicks(boolean b)
如果 b 是 true， 每一次调整后滑块都要对齐到最接近的标尺处。
• void setPaintTrack( boolean b)
如果 b 是 tme， 显示滑动条滑动的轨迹。
1 2 . 5
菜 单
前面介绍了几种最常用的可以放到窗口内的组件， 如： 各种按钮、 文本域以及组合框等。
Swing 还提供了一些其他种类的用户界面兀素’ 下拉式菜单就是 GUI 应用程序中很常见的一种。
位于窗口顶部的菜单栏 （menubar ) 包括了下拉菜单的名字。点击一个名字就可以打开包
含菜单项 （ menu items ) 和子菜单 （ submenus) 的菜单。 当
用户点击菜单项时， 所有的菜单都会被关闭并且将一条消
息发送给程序。 图 12-19 显示了一个带子菜单的典型菜单。
12.5.1
菜单创建
创建菜单是一#非常容易的事情。首先要创建一个菜单栏：
JMenuBar menuBar = new ]MenuBar();
菜单栏是一个可以添加到任何位置的组件。 通常放置
图 12-19
带有子菜单的菜单
File WSM Help
cut
电 Copy
岛 Paste
Read- only
Insert
Overtype
第！
2 章
Swing 用?界面?件
505
在框架的顶部。可以调用 setJMenuBar 方法将菜单栏添加到框架上：
frame.set3MenuBar(menuBar):
需要为每个菜单建立一个菜单对象：
JMenu editMenu = new 3Menu("Edit");
然后将顶层菜单添加到菜单栏中：
menuBar.add(editMenu);
向菜单对象中添加菜单项、分隔符和子菜单：
JMenuItem pasteltem = new ]MenuItem("Paste")；
editMenu.add(pasteltem);
edi tMenu.addSeparatorO;
JMenu optionsMenu = . . .; // a submenu
editMenu.add(optionsMenu);
可以看到图 12-19 中分隔符位于 Paste 和 Read-only 菜单项之间。
当用户选择菜单时，将触发一个动作事件。这里需要为每个菜单项安装一个动作监听器。
ActionListener listener = . .
pastelteiaddActi onListenerQistener) ;
可以使用 JMenu.add(Striiig s) 方法将菜单项插入到菜单的尾部， 例如：
edi tMenu.addCPaste") ;
Add 方法返回创建的子菜单项。可以采用下列方式获取它， 并添加监听器：
JMenuItem pasteltem = editMenu.add("Paste")；
pasteltem.addActionListener(listener);
在通常情况下， 菜单项触发的命令也可以通过其他用户界面元素（如工具栏上的按钮)
激活。在第 11 章中， 已经看到了如何通过 Action 对象来指定命令。通常， 采用扩展抽象类
AbstractAction 来定义一个实现 Action 接口的类。这里需要在 AbstractAction 对象的构造器中
指定菜单项标签并且覆盖 actionPerformed 方法来获得菜单动作处理器。例如：
Action exitAction = new AbstractAction("Exit") // menu item text goes here
{
public void actionPerformed(ActionEvent event)
{
// action code goes here
System.exit(0);
}
}；
然后将动作添加到菜单中：
]MenuItem exitltem = fileMenu.add(exitAction) ;
这个命令利用动作名将一个菜单项添加到菜单中。这个动作对象将作为它的监听器。上
面这条语句是下面两条语句的快捷形式：
JMenuItem exitltem = new JMenuItem(exitAction) ;
fileMenu.add(exitltem) ;
506
Java 核心技?
Q javax.swing.JMenu 1.2
• JMenu(String label)
用给定标签构造一个菜单。
• JMenuItem add(JMenuItem item)
添加一个菜单项 （或一个菜单)。
• JMenuItem add(String label)
用给定标签将一个菜单项添加到菜单中， 并返回这个菜单项。
• JMenuItem add(Action a)
用给定动作将一个菜单项添加到菜单中， 并返回这个菜单项。
• void addSeparator()
将一个分隔符行 （separatorline)添加到菜单中。
• JMenuItem insert(JMenuItem menu, int index)
将一个新菜单项（或子菜单）添加到菜单的指定位置。
• JMenuItem insert(Action a, int index)
用给定动作在菜单的指定位置添加一个新菜单项。
• void insertSeparator(int index)
将一个分隔符添加到菜单中。
参数:
• index
添加分隔符的位置
• void remove(int index)
• void remove(JMenuItem item)
从菜单中删除指定的菜单项。
AW] javax.swing.JMenuItem 1.2
• JMenuItem(String label)
用给定标签构造一个菜单项。
• JMenuItemCAction a) 1.3
为给定动作构造一个菜单项。
[iwj javax.swing.AbstractButton 1.2
• void setAction(Action a) 1.3
为这个按钮或菜单项设置动作。
[API|javax.swing.JFrame 1.2
• void setJMenuBar(JMenuBar menubar)
为这个框架设置菜单栏。
$ 12t
Swing用?界面?件
507
12.5.2
菜单项中的图标
菜单项与按钮很相似。 实际上， JMenuItem 类扩展了 AbstractButton 类。与按钮一样，
菜单可以包含文本标签、 图标， 也可以两者都包含。既可以利用 JMenuItem(Striiig，Icon) 或
者 JMenuItem(Icon) 构造器为菜单指定一个图标，也可以利用 JMenuItem 类中的 setlcon 方法
(继承自 AbstractButton 类）指定一个图标。例如：
JMenuItem cutltem = new 3MenuItem("Cut", new ImagelconC'cut.gif"))；
图 12-19 展示了具有图标的菜单项。正如所看到的，在默认情况下，菜单项的文本被放置在
图标的右侧。如果喜欢将文本放置在左侧， 可以调用 JMenuItem 类中的 setHorizontalTextPosition
方法（继承自 AbstractButton 类）设置。例如：
cutltem.setHorizontalTextPosition(SwingConstants.LEFT);
这个调用把菜单项文本移动到图标的左侧。
也可以将一个图标添加到一个动作上：
cutAction.putValue(Action.SMALL.ICON, new ImagelconC'cut.gif'))；
当使用动作构造菜单项时，Action.NAME 值将会作为菜单项的文本，而 Action.SMALL_
ICON 将会作为图标。
另外， 可以利用 AbstractAction 构造器设置图标：
cutAction = new
AbstractAction("Cut", new ImagelconC'cut.gif"))
{
public void actionPerformed(ActionEvent event)
{
}
}；
fw] javax.swing.JMenuItem 1.2
• JMenuItem(String label , Icon icon)
用给定的标签和图标构造一个菜单项。
[AH} javax.swing.AbstractButton 1.2
• void setHorizontalTextPosition(int pos)
设置文本对应图标的水平位置。
参数： pos
SwingConstants.RIGHT (文本在图标的右侧）或 SwingConstants.LEFT
因 javax.swing.AbstractAction 1.2
• AbstractAction(String name, Icon smallIcon)
用给定的名字和图标构造一个抽象的动作。
508
Java 核心技?
12.5.3
复选框和单选钮菜单项
复选框和单选钮菜单项在文本旁边显示了一个复选框或一个单选钮（参见图 12-19 )。当
用户选择一个菜单项时，菜单项就会自动地在选择和未选择间进行切换。
除了按钮装饰外，同其他菜单项的处理一样。 例如， 下面是创建复选框菜单项的代码：
]CheckBoxHenuItem readonlyltem = new ]CheckBoxMenuItem("Read-only");
optionsMenu.add(readonlyltem);
单选钮菜单项与普通单选钮的工作方式一样， 必须将它们加人到按钮组中。 当按钮组中
的一个按钮被选中时， 其他按钮都自动地变为未选择项。
ButtonGroup group = new ButtonGroupO;
JRadioButtonMenuItem insertltem = new]RadioButtonMenuItem("Insert");
insertltem.setSelected(true);
]RadioButtonMenuItem overtypeltem = new]RadioButtonMenuItem("Overtype")；
group,add(insertltem);
group.add(overtypeltem);
optionsMenu.add(insertltem);
optionsMenu.add(overtypeltem);
使用这些菜单项，不需要立刻得到用户选择菜单项的通知。而是使用 isSelected 方法来
测试菜单项的当前状态（当然， 这意味着应该保留一个实例域保存这个菜单项的引用）。使用
setSelected 方法设置状态。
[iw|javax.swing.JCheckBoxMenultem 1.2
• JCheckBoxMenuItem(String label)
用给定的标签构造一个复选框菜单项。
• JCheckBoxMenuItem(String label , boolean state)
用给定的标签和给定的初始状态（true 为选定）构造一个复选框菜单。
因 javax.swing.JRadioButtonMenultem 1.2
• JRadioButtonMenuItem(String label)
用给定的标签构造一个单选钮菜单项。
• JRadioButtonMenuItemCString label , boolean state)
用给定的标签和给定的初始状态（true 为选定）构造一个单选钮菜单项。
[w|javax.swing.AbstractButton 1.2
• boolean isSelected()
• void setSelected(boo1ean state)
获取或设置这个菜单项的选择状态（true 为选定)。
12.5.4
弹出菜单
弹出菜单（pop-up menu) 是不固定在菜单栏中随处浮动的菜单（参见图 12-20 )。
第 J2 章
Swing 用?界面?件
509
创建一个弹出菜单与创建一个常规菜单的方法类似，但是弹出菜单没有标题。
JPopupMenu popup = new JPopupMenuO;
然后用常规的方法添加菜单项：
JMenuItem item = new ]MenuItem("Cut");
item.addActi onListener(1istener);
popup,add(item);
弹出菜单并不像常规菜单栏那样总是显示在框架的顶
部，必须调用 show 方法菜单才能显示出来。 调用时需要给
出父组件以及相对父组件坐标的显示位置。 例如：
popup.show(panel , x, y);
通常， 当用户点击某个鼠标键时弹出菜单。 这就是所谓的弹出式触发器（pop-up
trigger) o 在 Windows 或者 Linux 中， 弹出式触发器是鼠标右键。要想在用户点击某一个组件
时弹出菜单， 需要按照下列方式调用方法：
component.setComponentPopupMenu(popup) ;
偶尔会遇到在一个含有弹出菜单的组件中放置一个组件的情况。这个子组件可以调用下
列方法继承父组件的弹出菜单。调用：
child.setlnheritsPop叩Menu(true);
ITcut
—
电 Copy
图 12-20
弹出菜单
[AW|javax.swing.JPopupMenu 1.2
•void show( Component c, int x, int y)
显示一个弹出菜单。
参数： c
显示弹出菜单的组件
X，y
弹出菜单左上角的坐标（C 的坐标空间内）
•boolean isPopupTrigger( MouseEvent event ) 1.3
如果鼠标事件是弹出菜单触发器， 则返回 true。
[iiw|java.awt.event.MouseEvent 1.1
•boolean isPopupTrigger( )
如果鼠标事件是弹出菜单触发器， 则返回 true。
[AW|javax.swing.JComponent 1.2
•JPopupMenu getComponentPopupMenu( ) 5.0
•void setComponentPopupMenu( JPopupMenu popup) 5.0
获取或设置用于这个组件的弹出菜单。
•boolean getlnheritsPopupMenu( ) 5.0
•void setInheritsPopupMenu( boolean b) 5.0
获取或设置 inheritsPopupMenu 特性。 如果这个特性被设置或这个组件的弹出菜单为
510
Java 核心技?
null, 则应用上一级弹出菜单。
12.5.5
快捷键和加速器
对于有经验的用户来说， 通过快捷键来选择菜单项会感觉更加便捷。 可以通过在菜单项
的构造器中指定一个快捷字母来为菜单项设置快捷键：
JMenuItem aboutltem = new ]HenuItem("About", 'A');
快捷键会自动地显示在菜单项中， 并带有一条下划线
(如图 12-21 所示)。 例如，在上面的例子中， 菜单项中的标
签为“
About”， 字母 A 带有一个下划线。当显示菜单时，
用户只需要按下“ A” 键就可以这个选择菜单项（如果快捷
字母没有出现在菜单项标签字符串中， 同样可以按下快捷
键选择菜单项， 只是快捷键没有显示出来。很自然，这种
不可见的快捷键没有提示效果)。
有时候不希望在菜单项的第一个快捷键字母下面加下划线。 例如， 如果在菜单项“
Save
As” 中使用快捷键“
A”， 则在第二个“
A” （ Save As) 下面加下划线更为合理。 可以调用
setDisplayedMnemonicIndex 方法指定希望加下划线的字符。
如果有一个 Action 对象， 就可以把快捷键作为 Action. MNEMONIC_KEY 的键值添加到
对象中。 如：
cutAction.putValue(Action,MNEMONIC_KEY, new IntegerCA'))；
只能在菜单项的构造器中设定快捷键字母， 而不是在菜单构造器中。 如果想为菜单设置
快捷键， 需要调用 setMnemonic 方法：
]Menu helpMenu = new ]Menu("Help");
helpMenu. setMnemonic('H');
可以同时按下 ALT 键和菜单的快捷键来实现在菜单栏中选择一个顶层菜单的操作。 例
如： 按下 ALT+H 可以从菜单中选择 Help 菜单项。
可以使用快捷键从当前打开的菜单中选择一个子菜单或者菜单项。而加速器是在不打开
菜单的情况下选择菜单项的快捷键。 例如： 很多程序把加速器 CTRL+O 和 CTRL+S 关联到
File 菜单中的 Open 和 Save 菜单项。 可以使用 setAccelerator 将加速器键关联到一个菜单项
上。这个方法使用 Keystroke 类型的对象作为参数。 例如：下面的调用将加速器 CTRL+O 关
联到 Openltem 菜单项。
openItem.setAccelerator(KeyStroke.getKeyStroke("ctrl 0"));
当用户按下加速器组合键时， 就会自动地选择相应的菜单项， 同时激活一个动作事件，
这与手工地选择这个菜单项一样。
加速器只能关联到菜单项上， 不能关联到菜单上。加速器键并不实际打开菜单。它将直
接地激活菜单关联的动作事件。
indtx I
1 About I
图 1 2-2 1
键 盘 快 捷 键
第 12 章
Swing 用?界面?件
511
从概念上讲， 把加速器添加到菜单项与把加速器添加到 Swing 组件上所使用的技术十分
类似（在第 11 章中讨论了这个技术 )。但是， 当加速器添加
到菜单项时， 对应的组合键就会自动地显示在相应的菜单
上（见图 12-22 )。
0 注释： 在 Windows 下， ALT+F4 用于关闭窗口。 但这
不是 Java 程序设定的加速键 .
这是操作系统定义的快
捷键。 这个组合键总会触发活动窗口的 WindowClosing
事件， 而不管菜单上是否有 Close 菜单项。
[wi] javax.swing.JMenultem 1.2
•JMenuItemCString label， int mnemonic )
用给定的标签和快捷键字符构造一个菜单项。
参数：label
菜单项的标签
mnemonic 菜单项的快捷键字符， 在标签中这个字符下面会有一个下划线。
•void setAccelerator(Keystroke k )
将 k 设置为这个菜单项的加速器。 加速器显示在标签的旁边。
[«H|javax.swing.AbstractButton 1.2
•void setMnemonicCint mnemonic )
设置按钮的快捷字符。该字符会在标签中以下划线的形式显示。
•void setDisplayedMnemoniclndex(int index ) 1.4
将按钮文本中的 index 字符设定为带下划线。 如果不希望第一个出现的快捷键字符带
下划线， 就可以使用这个方法。
12.5.6
启用和禁用菜单项
在有些时候， 某个特定的菜单项可能只能够在某种特定的环境下才可用。 例如， 当文档
以只读方式打开时， Save 菜单项就没有意义s 当然，可以使用 JMemremove 方法将这个菜
单项从菜单中删掉， 但用户会对菜单内容的不断变化感到奇怪。 然而， 可以将这个菜单项设
为禁用状态， 以便屏蔽掉这些暂时不适用的命令。被禁用
的菜单项被显示为灰色，不能被选择它（见图 12-23 )。
启用或禁用菜单项需要调用 setEnabled 方法：
saveltem.setEnabled(false):
启用和禁用菜单项有两种策略。 每次环境发生变化就
对相关的菜单项或动作调用 setEnabled。 例如： 只要当文档
以只读方式打开， 就禁用 Save 和 Save As 菜单项。 另一种
方法是在显示菜单之前禁用这些菜单项。这里必须为“ 菜
图 12-23
禁用菜单项
图 1 2-2 2
加 速 键
512
Java #心技?
单选中” 事件注册监听器。javax.swing.event 包定义了 MenuListener 接口， 它包含三个方法：
void menuSelected(MenuEvent event)
void menuDeselected(MenuEvent event)
void menuCanceled(MenuEvent event)
由于在菜单显示之前调用 menuSelected 方法， 所以可以在这个方法中禁用或启用菜单
项。 下面代码显示了只读复选框菜单项被选择以后， 如何禁用 Save 和 Save As 动作。
public void menuSelected(MenuEvent event)
{
saveAction.setEnabled(!readonlyltem.isSelected());
saveAsAction.setEnabled(!readonlyltem.isSelected());
}
警告： 在显示菜单之前禁用菜单项是一种明智的选择， 但这种方式不适用于带有加速键
的菜单项。 这是因为在按下加速键时并没有打开菜单， 因此动作没有被禁用， 致使加速
键还会触发这个行为。
javax.swing.JMenultem 1.2
• void setEnabled(boolean b )
启用或禁用菜单项。
jAPt) javax.swing.event.MenuListener 1.2
• void menuSelected(MenuEvent e)
在菜单被选择但尚未打开之前调用。
• void menuDeselected(MenuEvent e)
在菜单被取消选择并且已经关闭之后被调用。
• void menuCanceled(MenuEvent e)
当菜单被取消时被调用。 例如， 用户点击菜单以外的区域。
程序清单 12-8 是创建一组菜单的示例程序。这个程序演示了本节介绍的所有特性， 包
括： 嵌套菜单、 禁用菜单项、 复选框和单选钮菜单项、 弹出菜单以及快捷键和加速器。
程序清单 12-8
menu/MenuFrame.java
1 package menu;
2
3 import java.awt.event.*;
4 import javax.swing.*;
6
/**
7
* A frame with a sample menu bar.
8
V
9 public class MenuFrame extends ]Frame
10 {
11
private static final int DEFAULT_WIDTH = 300;
12
private static final int DEFAULTJEICHT = 200;
13
private Action saveAction;
14
private Action saveAsAction;
IS
16
17
18
19
20
21
22
23
24
2S
26
27
28
29
30
31
32
33
34
3S
36
37
38
39
40
41
42
43
44
45
46
47
48
49
SO
51
52
53
S4
55
S6
S7
58
S9
60
61
62
63
64
6S
66
67
68
69
70
71
第 口 章
Swing 用 ? 界 面 ? 件
5 1 3
private JCheckBoxMenuItem readonlyltem;
private JPopupMenu popup;
/**
* A sample action that prints the action name to System.out
V
class TestAction extends AbstractAction
{
public TestAction(String name)
{
super(name);
}
public void actionPerformed(ActionEvent event)
{
System.out.println(getValue(Action.NAME) + " selected.");
}
}
public MenuFrame()
{
setSize(DEFAULT.WIDTH, DEFAULTJEIGHT);
JMenu fileMenu = new ]Menu("FileH)；
fileMenu.add(new TestAction("New"));
// demonstrate accelerators
JMenuItem openltem = fileMenu.add(new TestAction("0penM));
openltem.setAccelerator(KeyStroke.getKeyStroke("ctrl 0"));
fileMenu.addSeparator();
saveAction = new TestAction("Save");
JMenuItem saveltem = fileMenu.add(saveAction);
saveltem.setAccelerator(KeyStroke.getKeyStrokefctrl S"));
saveAsAction = new TestAction("Save As")；
fileMenu.add(saveAsAction);
fileMenu.addSeparator();
fileMenu.add(new AbstractAction("Exit")
{
public void actionPerformed(ActionEvent event)
{
System,exit⑼；
}
})；
// demonstrate checkbox and radio button menus
readonlyltem = new ]CheckBoxMenuItem("Read-only")；
readonlyltem.addActionListene「(new ActionListenerO
{
public void actionPe「formed(ActionEvent event)
{
boolean saveOk = !readonlyltem.isSelected();
514
Java 核心技?
72
saveAction.setEnabled(saveOk);
73
saveAsAction.setEnabled(saveOk) ;
74
}
75
})；
76
77
ButtonCroup group = new ButtonCroupO ;
78
79
JRadioButtonMenuIteni insertltem = new ]RadioButtonMenuItem("Inse「t");
so
insertltem.setSelected(true);
si
JRadioButtonMenuItem overtypeltem = new ]RadioButtonMenuItem("Overtype");
82
83
gro叩.add(insertltem);
84
group.add(overtypeltem);
85
86
// demonstrate icons
87
88
Action cutAction = new TestAction("Cut")；
89
cutAction.putValue(Action. SMALL_IC0N, new Imagelconfcut.gif'));
90
Action copyAction = new TestAction("Copy")；
91
copyAction.putValue(Action. SMALL_IC0N, new Imagelcon("copy.gif"));
92
Action pasteAction = new TestAction("Paste");
93
pasteAction.putValue(Action. SMALL一ICON, new ImageIcon("paste.gif"));
94
95
JMenu editMenu = new ]Menu("Edit");
96
editMenu.add(cutActi on);
97
editMenu. add(copyActi on);
98
editMenu.add(pasteAction);
99
100
// demonstrate nested menus
101
102
JMenu optionMenu = new ]Menu("Options");
103
104
optionMenu • add(readonlyltem) ;
105
optionMenu.addSeparatorO；
106
opti onMenu.add(insertltem);
107
optionMenu.add(overtypeltem) ;
108
io9
editMenu.addSeparatorO;
no
editMenu.add(optionMenu):
m
112
// demonstrate mnemonics
113
114
JMenu helpMenu = new 3Menu("Help")；
us
helpMenu. setMnemonic('H');
116
117
JMenuItem indexltem = new ]MenuItem("Index") ;
118
indexltem. setMnemoni c(T);
119
helpMenu . add(indexltem);
120
121
// you can also add the mnemonic key to an action
122
Action aboutAction = new TestAction("About");
123
aboutAction.putValue(Action.MNEM0NIC_KEY, new Integer ' A’））；
124
helpMenu.add(aboutAction):
125
126
// add all top-level menus to menu bar
127
us
3MenuBar menuBar = new ]MenuBar();
129
set3MenuBar(menuBar);
第 /2 章
Swing 用 ? 界 面 ? 件
5 1 5
12.5.7
工具栏
工具栏是在程序中提供的快速访问常用命令的按钮栏， 如图 12-24 所示。
工具栏的特殊之处在于可以将它随处移动。 可以将它拖拽到框架的四个边框上， 如
图 12-25 所示。 释放鼠标按钮后， 工具栏将会停靠在新的位置上， 如图 12-26 所示。
图 12-24
工具栏
图 12-25
拖拽工具栏
Kl 注释： 工具栏只有位于采用边框布局或者任何支持 North、 East、 South 和 West 约束布局
管理器的容器内才能够被拖拽。
工具栏可以完全脱离框架。 这样的工具栏将包含在自己的框架中， 如图 12-27 所示。 当
关闭包含工具栏的框架时， 它会冋到原始的框架中。
图 12-26
将工具栏拖拽到另一边框
图 12-27
脱离的工具栏
MM ®
:
匕
Color
BEIJL丨 i7< !
130
131
132
134
135
136
137
138
139
menuBar.add(fileMenu);
menuBar.add(editMenu);
menuBar.add(helpMenu);
// demonstrate pop-ups
popup = new ]Pop叩Menu();
popup.add(cutAction);
popup.add(copyAction);
popup.add(pasteAction);
]Panel panel = new ]Panel。
;
panel.setComponentPopupMenu(popup);
add(panel);
ToolBarTest
Color
lAjjToo丨BarTest
UlnlX
Color
m
1
516
Java 核心技?
编写创建工具栏的代码非常容易，并且可以将组件添加到工具栏中：
JToolBar bar = new ]ToolBar();
bar.add(blueButton);
JToolBar 类还有一个用来添加 Action 对象的方法，可以用 Action 对象填充工具栏：
bar.add(blueAction);
这个动作的小图标将会出现在工具栏中。
可以用分隔符将按钮分组：
bar.addSeparatorO；
例如， 图 12-24 中的工具栏有一个分隔符， 它位于第三个按钮和第四个按钮之间。
然后，将工具栏添加到框架中：
add(bar, BorderLayout.NORTH):
当工具栏没有停靠时，可以指定工具栏的标题：
bar = new ]ToolBar(titleString) ;
在默认情况下， 工具栏最初为水平的。如果想要将工具栏垂直放置， 可以使用下列代码：
bar = new 3ToolBar(SwingConstants.VERTICAL)
或者
bar = new ]ToolBar(titleString, Swi ngConstants.VERTICAL)
按钮是工具栏中最常见的组件类型。然而工具栏中的组件并不仅限如此。例如，可以往
工具栏中加人组合框。
12.5.8
工具提示
工具栏有一个缺点，这就是用户常常需要猜测按钮上小图标按钮的含义。为了解决这个
问题， 用户界面设计者发明了工具提示（tooltips)。当光标停留在某个按钮上片刻时，工具提
示就会被激活。 工具提示文本显示在一个有颜色的矩形里。
当用户移开鼠标时，工具提示就会自动地消失。如图 12-28
所示。
在 Swing 中，可以调用 setToolText 方法将工具提不添
加到 Component 上：
exitButton.setToolTipText("Exit");
还有一种方法是， 如果使用 Action 对象， 就可以用
SHORT_DESCRIPTION 关联工具提示：
exitAction.putValue(Action.SHORTJESCRIPnON, "Exit");
程序清单 12-9 说明了如何将一个 Action 对象添加到菜单和工具栏中。 注意， 动作名在
菜单中就是菜单项名，而在工具栏中就是简短的说明。
图 12-28
工具提示
$ 12t
Swing 用?界面?件
517
程序清单 12-9
too旧ar/Too旧arFrame.java
1 package toolBar;
2
3 import java.awt.*;
4 import :java.awt.event.*;
s
import javax.swing.*;
6
7 /**
8
* A frame with a toolbar and menu for color changes.
9
V
10
public class ToolBarFrame extends ]Frame
11 {
12
private static final int DEFAULT_WIDTH = 300;
13
private static final int DEFAULT一HEIGHT = 200;
14
private ]Panel panel ;
is
16
public ToolBarFrame()
17
{
is
setSize(DEFAULT_WIDTH, DEFAULTJEICHT);
19
20
// add a panel for color change
21
22
panel = new ]Panel ();
23
add(panel , BorderLayout.CENTER);
24
2$
// set up actions
26
27
Action blueAction = new ColorActionC'Blue" , new Imagelcon("blue-ball .gif") , Color . BLUE) ;
28
Action yellowAction = new ColorAction("Yellow", new Imagelcon("yel1ow-bal1.gif"),
29
Color.YELLOW);
BO
Action redAction = new ColorAction("Red" , new Imagelcon("red-ball ,gif") , Color.RED);
31
32
Action exitAction = new AbstractAction("Exit", new Imagelcon("exit ,gif"))
33
{
34
public void actionPerformed(ActionEvent event)
35
{
36
System.exit(O);
37
}
38
}；
39
exitAction.putValue(Action.SH0RTJESCRIPTI0N, "Exit");
40
41
// populate toolbar
42
43
JToolBar bar = new ]ToolBar();
44
bar.add(blueAction);
4s
bar.add(yellowAction);
46
bar.add(redAction);
47
bar.addSeparatorO;
48
bar.add(exitAction) ;
49
add(bar , BorderLayout.NORTH) ;
so
si
// populate menu
52
53
]Menu menu = new ]Menu("Color");
54
menu.add(yellowAction) ;
ss
menu.add(blueAction);
518
Java 核心技?
64
82
menu.add(redAction);
menu.add(exitAction);
JMenuBar menuBar = new ]MenuBa「(）；
menuBar.add(menu);
setjMenuBar(menuBar);
/**
* The color action sets the background of the frame to a given color.
V
class ColorAction extends AbstractAction
{
public ColorAction(String name, Icon icon, Color c)
{
putValue(Action.NAME, name);
putValue(Action.SMALL_ICON, icon);
putValue(Action.SHORTJESCRIPTION, name + " background")；
putValue("Color", c);
public void actionPerformed(ActionEvent event)
{
Color c = (Color) getValue("Color");
panel.setBackground(c);
API| javax.swing.JToolBar 1.2
• JToolBar( )
• JToolBar( String titlestring )
• JToolBar( int orientation )
• JToolBar( String tit eString , int orientation )
用给定的标题字符串和方向构造一个工具栏。Orientation 可以是 SwingConstants.
HORIZONTAL ( 默认）或 SwingConstants.VERTICAL。
• JButton add( Action a )
用给定的动作名、 图标、 简要的说明和动作回调构造一个工具栏中的新按钮。
• void addSeparator( )
将一个分隔符添加到工具栏的尾部。
AW] javax.swing.JComponent 1.2
• void setToolTipText( String text )
设置当鼠标停留在组件 h时显示在工具提示中的文本。
12.6
复杂的布局管理
迄今为止， 在前面的示例应用程序所使用的用户界面组件中， 只使用了边框布局、 流布
0 12t
Swing 用?界面?件
519
局和网格布局。对于复杂的问题而言， 只使用这四种布局显然不够。 本节将详细地讨论高级
布局管理器。
Windows 程序员可能会为 Java 对布局管理器如此兴师动众而感到奇怪。 毕竟， 在
Windows 中， 布局管理不是一个太大的问题： 首先， 可以用对话框编辑器将组件拖放到对话
柜的表面上。然后，再使用编辑器工具完成组件对齐、 均衡间隔、 中心定位等工作。如果正
在开发的是一个大型项目，可能根本就不必担心组件如何布局， 技术娴熟的用户界面设计师
会完成所有这些任务。
使用这种方法布局会出现这样的问题： 如果组件的大小发生改变， 必须手工进行更新。
为什么组件的大小会发生改变呢？ 通常有两种可能。第一种可能是为按钮标签和其他对话框
文本选择了一种较大的字体。 如果在 Windows 里试验一下就会发现很多应用程序没有解决好
这个问题。按钮的尺寸不增大，大字体被紧缩在原来的空间里。当应用程序中的字符串翻译
成其他语言时也有可能出现同样的问题， 例如，“
Cancel” 在德语中为“
Abbrechen”。如果
一个按钮的大小被设计成刚好能够显示字符串“
Cancel”，那么德语版显示就会出现问题了，
字符串将会被剪掉一部分。
为什么 Windows 中的按钮不能动态地增大以适应标签呢？ 这是因为用户界面设计师没有
给出应该在哪个方向增大的指令。 每个组件拖放或排列之后， 对话框编辑器只保存其像素位
置和尺寸大小。至于组件为什么以这种方式排列并没有记录下来。
Java 布局管理器是一种用于组件布局的好方法。应用布局管理器， 布局就可以使用组件
间关系的指令来完成布局操作。对于最初的 AWT 来说， 这一点特别重要， 这是因为 AWT 使
用的是本地用户界面元素。在 Motif、Windows 和 Macintosh 中，按钮和列表框的大小各不相
同， 而且应用程序或 applet 不会预先知道它们将在哪个平台上显示。在某种程度上， 可变性
在 Swing 中就没有那么重要。 如果应用程序强制使用特定的观感， 如 Metal 观感， 那么这个
程序在所有平台上显示的结果都一样。但是， 如果允许应用程序的用户随意地选择观感， 则
需要依据布局管理器的灵活性调整组件的排列了。
自从 Java 1.0 以来，AWT 就含有网格组布局（grid bag layout), 这种布局将组件按行和
列排列。行和列的大小可以灵活改变， 并且组件可以横跨多行多列。这种布局管理器非常灵
活，但也非常复杂。仅仅提及“ 网格组布局” 一词就会吓住一些 Java 程序员。
Swing 设计者有一个失败的尝试：为了能够将程序员从使用网格组布局的困难中解脱出
来， 提出了一种被称为箱式布局（box layout) 的布局管理器。在 BoxLayout 类的 JDK 文档中
写道：“ 采用水平和垂直 [sic] 的不同组合内嵌多个面板将可以获得与 GridBagLayout 类似的
效果， 而且降低了复杂度。” 然而， 由于每个箱子是独立布局的， 所以不能使用箱式布局排
列水平和垂直方向都相邻的组件。
Java SE 1.4 还做了一个尝试：设计一•种代替网格组组件的布局
（ spring layout)。这
种布局使用一个虚构的弹簧将同一个容器中的所有组件连接起来。当容器改变大小时， 弹簧
可以伸展或收缩， 以便调节组件的位置。这听起来似乎感觉枯燥且不可思议，其实也确实如
此。 弹簧布局很快就会陷人含糊不清的境地。
520
Java 核心技?
在 2005 年， NetBeans 开发队伍发明了 Matisse 技术， 这种技术将布局工具与布局管理器
结合起来。用户界面设计者可以使用工具将组件拖拽到容器中， 并指出组件的排列方式。工
具将设计者的意图转换成组布局管理器的可以理解的指令，与手工地编写布局管理的代码相
比，这样做要便捷得多。组布局管理器现在是 Java SE 6 中的一部分。 即使没有用 NetBeans
作为 IDE, 也应该考虑使用它的 GUI 生成工具。 可以用 NetBeans 设计 GUI, 然后再将得到
的代码粘贴到所选择的 IDE 中。
接下来，将讲述网格组布局。这是因为这种布局在早期的 Java 版本中， 使用的最普遍，
且也是产生布局代码的最简单方式。下面的策略可以让网格组布局的使用相对简单些。
随后， 介绍 Matisse 工具和组布局管理器。我们需要了解组布局管理器的工作过程， 以
便能够在用可视化方式将组件放置在某个位置时能够查看 Matisse 记录的指令是否正确。
最后， 将演示如何完全绕开布局管理，手工地放置组件， 以及如何编写自己的布局管
理器。
12.6.1
网格组布局
网格组布局是所有布局管理器之母。可以将网格组布局看成是没有任何限制的网格布
局。在网格组布局中， 行和列的尺寸可以改变。 可以将相邻的单元合并以适应较大的组件
(很多字处理器以及 HTML 都利用这个功能编辑表格： 一旦需要就合并相邻的单元格 )。组件
不需要填充整个单元格区域， 并可以指定它们在单元格内的对齐方式。
请看图 12-29 中所示的字体选择对话框， 其中包含下面的组件：
•两个用于指定字体外观和大小的组合框
•两个组合框的标签
•两个用于选择粗体和斜体的复选框
•一个用于显示示例字符串的文本区
现在，将容器分解为网格单元， 如图 12-30 所示（行和列的尺寸不需要相同）。每个复选
框横跨两列，文本区跨四行。
ps over the lazy dog
Size:|«~
五
□Italic
□
□
The quick brown fox jum
ps uvenheWazv*de®
J
图 12-29
字体选择器
图 12-30
设计中使用的对话框网格
第！
2 章
Swing 用?界面?件
521
要想使用网格组管理器进行布局， 必须经过下列过程：
1 ) 建立一个 GridBagLayout 的对象。不需要指定网格的行数和列数。 布局管理器会根据
后面所给的信息猜测出来。
2 ) 将 GridBagLayout 对象设置成组件的布局管理器。
3 ) 为每个组件建立一个 GridBagConstraints 对象。 设置 GridBagConstraints 对象的域以
便指出组件在网格组中的布局方案。
4 ) 最后，通过下面的调用添加组件的约束：
add(component, constraints )；
下面给出了相应的代码（稍后将更加详细地介绍各种约束。如果现在不明白约束的作用，
不必担心)。
GridBagLayout layout = new GridBagLayoutO ;
panel .setLayout(layout);
GridBagConstraints constraints = new GridBagConstraintsO ;
constraints.weightx = 100;
constraints.weighty = 100;
constraints.gridx = 0;
constraints.gridy = 2 ;
constrai nts.gridwidth = 2;
constraints.gridheight = 1;
panel .add(component , constraints);
知道如何设置 GridBagConstraints 对象的状态是非常困难的。下面将详细地介绍几个最
重要的约束。
12.6.1.1
gridx、 gridy、gridwidth 和 gridheight 参数
这些约束定义了组件在网格中的位置。gridx 和 gridy 指定了被添加组件左上角的行、 列
位置。gridwidth 和 gridheight 指定了组件占据的行数和列数。
网格的坐标从 0 开始。gridx=0 和 gridy=0 代表最左上角。 例如， 示例程序中， 文本
区的 gridx=2, gridy=0。 这是因为这个文本区起始于 0 行 2 列（即第 3 列 )， girdwidth=l ,
gridheight=4 因为它横跨了 4 行 1 列。
12.6.1.2
增量域
在网格布局中， 需要为每个区域设置增量域（weightx 和 weighty)。 如果将增量设置为
0, 则这个区域将永远为初始尺寸。在如图 12-29 所示的网格布局中， 由于将标签的 weightx
设置为 0, 所以在窗口缩放时， 标签大小始终保持不变。另一方面， 如果将所有区域的增量
都设置为 0, 容器就会集聚在为它分配的区域中间， 而不是通过拉伸来填充它。
从概念上讲，增量参数属于行和列的属性， 而不属于某个单独的单元格。但却需要在单
元格上指定它们，这是因为网格组布局并不暴露行和列。行和列的增量等于每行或每列单元
格的增量最大值。因此， 如果想让一行或一列的大小保持不变， 就需要将这行、这列的所有
组件的增量都设置为 0。
注意，增量并不实际给出列的相对大小。当容器超过首选大小时， 增量表示分配给每个
区域的扩展比例值。这么说并不太直观。这里建议将所有的增量设置为 100, 运行程序，查
522
Java 核心技?
看一下布局情况。缩放对话框，查看一下行和列是如何调整的。如果发现某行或某列不应该
扩大， 就将那行或那列中的所有组件的增量设置为 0。 也可以使用其他的增量值进行修补，
但是这么做的意义并不大。
12.6.1.3
fill 和 anchor 参数
如果不希望组件拉伸至整个区域， 就需要设置 fill 约束。它有四个有效值：GridBag
Constraints.NONE、 GridBagConstraints.HORIZONTAL、 GridBagConstraints. VERTICAL 和
GridBagConstraints.BOTH.0
如 果 组 件 没 有 填 充 整 个 区 域， 可 以 通 过 设 置 anchor 域 指 定 其 位 置。 有 效 值 为
GridBag Constraints.CENTER ( 默 认 值 ）、GridBagConstraints.NORTH、 GridBagConstraints.
NORTHEAST 和 GridBagConstraints.EAST 等。
12.6.1.4
填充
可以通过设置 GridBagLayout 的 insets 域在组件周围增加附加的空白区域。 通过设置
Insets 对象的 left、top、right 和 bottom 指定组件周围的空间量。这被称作外部填充（或外边距 )
(external padding )。
通过设置 ipadx 和 ipady 指定内部填充（或内外距 )( internal padding)。这两个值被加到组
件的最小宽度和最小高度上。这样可以保证组件不会收缩至最小尺寸之下。
12.6.1.5
指定 gridx, gridy, gridwidth 和 gridheight 参數的另一种方法
AWT 文档建议不要将 gridx 和 gridy 设置为绝对位置， 应该将它们设置为常量 GridBag
Constraints.RELATIVEo 然后，按照标准的顺序，将组件添加到网格组布局中。即第一行从
左向右，然后再开始新的一行， 以此类推。
还需要通过为 gridheight 和 gridwidth 域指定一个适当的值来设置组件横跨的行数和列
数。除此之外， 如果组件扩展至最后一行或最后一列， 则不要给出一个实际的数值， 而是用
常量 GridBagConstraints.REMAINDER 替代， 这样会告诉布局管理器这个组件是本行上的最
后一
■个组件。
这种方案看起来能起作用，但似乎显得有点笨拙。这是因为这样做会将实际位置信息对
布局管理器隐藏起来，而日后又希望它能够重新发现这些信息。
这些事情看起来都很麻烦和复杂， 但实际上， 下面的策略可以让网格组布局的使用相对
简单一些：
1 ) 在纸上画出组件布局草图。
2 ) 找出一种网格， 小组件被放置在一个单元格内， 大组件将横跨多个单元格。
3 ) 用 0, 1, 2
标识网格的行和列。现在可以读取 gridx, gridy, gridwidth 和 gridheight
的值。
4 ) 对于每个组件，需要考虑下列问题： 是否需要水平或者垂直填充它所在的单元格？ 如
果不需要， 希望如何排列？ 这些就是 fill 和 anchor 参数的设置。
5 ) 将所有的增量设置为 100。 如果需要某行或某列始终保持默认的大小， 就将这行或这
列中所有组件的 weightx 和 weighty 设置为 0。
第 12 章
Swing 用?界面?件
523
6 ) 编写代码。仔细地检查 GridBagConstraints 的设置。错误的约束可能会破坏整个布局。
7 ) 编译、运行。
有些 GUI 构造者使用工具来可视化地指定约束。 如图 12-31 就是 NetBeans 中的配置对
话框。
Internal Padi
Internal Padi
TT 7
MU
1
l-Paddkig—
w 1-
i
-»l
㈠
l +
: EE
|
i
|
|j
el2j
|jCoiwbo»o...|
|jComboBo...|]
Kanawa
图 1 2-3 1
在 Neffieans 中指定网格组的约束
12.6.1.6
使用帮助类来管理网格组约束
网格组布局最乏味的工作就是为设置约束编写代码。为此， 很多程序员编写帮助函数或
者帮助类来满足上面的目的。下面是为字体对话框示例编写的帮助类。这个类有下列特性：
•名字简短：GBC 代替 GridBagConstraints。
•扩展于 GridBagConstraints, 因此可以使用约束的缩写， 如 GBC.EAST。
•当添加组件时，使用 GBC 对象， 如：
add(component, new CBC(lf 2));
•有 两 个 构 造 器 可 以 用 来 设 置 最 常 用 的 参 数：gridx 和 gddy， 或 者 gridx、gridy、
gridwidth 和 gridheighto
add(component, new CBC(1, 2, 1, 4));
•域有很便捷的设置方法，采用 x/y 值对形式：
add(component, new GBC(1，2).setWeight(100, 100));
•设置方法将返回 this, 所以可以链接它们：
524
Java 核心技?
add(component, new GBC(1, 2).setAnchor(GBC.EAST).setWeight(100, 100));
•setlnsets 方法将构造 Inset 对象。 要想获取 1 个像素的 insets, 可以调用：
add(component , new CBC(1, 2).setAnchor(CBC.EAST).setlnsets(1));
程序清单 12-10 显示了字体对话框示例的框架类。GBC 帮助类见程序清单 12-11。 下面
是将组件添加到网格组中的代码：
add(faceLabel , new CBC(0, 0).setAnchor(CBC.EAST)) ;
add(face, new CBC(1, 0).setFill (CBC.HORIZONTAL).setWeight(100, 0).setlnsets⑴）;
add(sizeLabel , new CBC(0, 1).setAnchor(CBC.EAST)):
add(size, new CBC(1, 1).setFi11(CBC.HORIZONTAL).setWeight(100, 0).setlnsets(1));
add(bold, new CBC(0, 2, 2, 1).setAnchor(CBC.CENTER).setWeight(100, 100));
add(italic, new CBC(0, 3, 2, 1).setAnchor(CBC.CENTER).setWeight(100, 100));
add(sample, new CBC(2, 0, 1, 4).setFi11(CBC.BOTH).setWeight(100, 100));
一旦理解了网格组约束， 就会觉得这些代码十分易于阅读且便于调试。
E1 注释： http://docs.orade.com/javase/tutorial/uiswing/layout/gridbag.html 的教程中建议： 对于所
有的组件重用同一个 GridBagConstraints 对象。我们发现这样做将会使代码难于阅读并易
于发生错误。 例雀口， 请看 http://docs.oracle.com/javase/tutorial/uiswing/events/containerlistener.
html 的演示。按钮真的被水平拉伸吗？ 还是程序员忘记了关闭 fill 约束？
程序清单 12-10
gridbag/FontFrame.java
1 package gridbag;
2
j import java.awt.Font;
4 import java.awt.CridBagLayout;
5 import java.awt.event.ActionListener;
6
7 import :javax.swing.BorderFactory;
8 import javax.swingJCheckBox;
9 import javax.swingJComboBox;
10 import javax.swingJFrame;
11 import javax.swing.]Label ;
12
import javax.swingJTextArea;
13
14 /**
is
* A frame that uses a grid bag layout to arrange font selection components.
16
*/
17 public class FontFrame extends ]Frame
18 {
19
public static final int TEXT一ROWS = 10;
20
public static final int TEXT.COLUMNS = 20;
21
22
private ]ComboBox<String> face;
2j
private ]ComboBox<Integer> size;
24
private ]CheckBox bold;
25
private ]CheckBox italic;
26
private 丌extArea sample;
27
28
public FontFrame()
CridBagLayout layout = new CridBagLayoutQ;
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
SI
S2
53
54
SS
56
S7
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
第 12 章
Swing 用? 界面? 件
525
setLayout(1ayout);
ActionListener listener = event -> updateSampleO;
II construct components
]Label faceLabel = new ]Label("Face: ")；
face = new 3ComboBoxo(new String[] { "Serif", "SansSerif", "Monospaced",
"Dialog", "Dialoglnput" });
face.addActionListene「(1istener);
]Label sizeLabel = new ]Label ("Size: ");
size = new ]ComboBox<>(new Integer[] { 8, 10, 12, 15, 18, 24, 36, 48 });
size.addActi onListener(listener);
bold = new ]CheckBox("Bold");
bold.addActi onListener(1istener);
italic = new JCheckBox("Italic")；
italic.addActi onListener(listener);
sample = new ]TextArea(TEXT_R0WS, TEXT.COLUMNS);
sample.setText("The quick brown fox jumps over the lazy dog")；
sample.setEditable(false);
sample,setLineWrap(true);
sample.setBorder(BorderFactory.createEtchedBorderO)；
// add components to grid, using CBC convenience class
add(faceLabel, new GBC(0, 0).setAnchor(CBC.EAST));
add(face, new CBC(1, 0).setFi11(CBC.HORIZONTAL).setWeight(100, 0)
.setlnsets(l));
add(sizeLabel, new CBC(0,l) .setAnchor(CBC.EAST));
add(size, new CBC(1, 1).setFi11(CBC.HORIZONTAL).setWeight(100, 0)
.setlnsets(l));
add(bold, new CBC(0, 2, 2, 1) • setAnchor(GBC _ CENTER).setWeight(100, 100));
add(italic， new GBC(0, 3, 2, 1).setAnchor(CBC.CENTER).setWeight(100, 100));
add(sample, new CBC(2, 0, 1, 4).setFi11(CBC.BOTH).setWeight(100, 100)) ;
pack();
updateSampleO;
public void updateSampleO
{
String fontFace = (String) face.getSelectedltemO；
int fontStyle = (bold.isSelected() ? Font.BOLD : 0)
+ (italic.isSelectedO ? Font.ITALIC : 0);
int fontSize = size.getltemAt(size.getSelectedlndex());
Font font = new Font(fontFace, fontStyle, fontSize);
sample,setFont(font);
sample,repaint();
526
Java 核心技?
程序清单 12-11
gridbag/GBC.java
1 package gridbag;
2
3
import java.awt.*;
s /**
6
* This class simplifies the use of the GridBagConstraints class.
7
* ©version 1.01 2004-05-06
s
* ©author Cay Horstnann
9
V
10 public class GBC extends GridBagConstraints
{
12
/**
13
* Constructs a GBC with a given gridx and gridy position and all other grid
14
* bag constraint values set to the default,
is
* @param gridx the gridx position
16
* @param gridy the gridy position
17
V
is
public CBC(int gridx, int gridy)
19
{
20
this.gridx = gridx;
21
this.gridy = gridy;
23
24
/**
2s
* Constructs a GBC with given gridx, gridy, gridwidth, gridheight and all
26
* other grid bag constraint values set to the default.
27
* @param gridx the gridx position
28
* @param gridy the gridy position
29
* @param gridwidth the cell span in x-direction
BO
* @param gridheight the cell span in y-direction
31
V
32
public GBC(int gridx, int gridy, int gridwidth, int gridheight)
33
{
34
this,gridx = gridx;
3s
this,gridy = gridy;
36
this.gridwidth =：gridwidth;
37
this,gridheight = gridheight;
39
40
41
42
43
49
50
53
54
55
/**
* Sets the anchor.
* @param anchor the anchor value
* ©return this object for further modification
V
public GBC setAnchor(int anchor)
{
this.anchor = anchor;
return this;
/**
* Sets the fill direction.
* @param fill the fill direction
* ©return this object for further modification
第 章
Swing 用 ? 界 面 ? 件
5 2 7
s6
public GBC setFill(int fill)
57
{
58
this.fill = fill;
59
return this;
60
}
61
62
/**
63
* Sets the cell weights.
64
* @param weightx the cell weight in x-direction
65
*
@param weighty the cell weight in y-direction
66
* ©return this object for further modification
67
*/
68
public GBC setWeight(double weightx, double weighty)
69
{
70
this,weightx = weightx;
71
this,weighty = weighty;
72
return this;
73
}
74
75
/**
76
* Sets the insets of this cell.
77
* @param distance the spacing to use in all directions
78
* ©return this object for further modification
79
*/
so
public GBC setlnsets(int distance)
81
{
82
this.insets = new Insets(distance, distance, distance, distance);
83
return this;
84
}
85
86
/**
87
* Sets the insets of this cell.
88
* @param top the spacing to use on top
89
*
@param left the spacing to use to the left
go
* @param bottom the spacing to use on the bottom
91
* @param right the spacing to use to the right
92
* @return this object for further modification
93
V
94
public GBC setlnsets(int top, int left, int bottom, int right)
95
{
96
this.insets = new Insets(top, left, bottom, right);
97
return this;
98
}
99
100
/**
101
* Sets the internal padding
102
*
@param ipadx the internal padding in x-direction
103
*
@param ipady the internal padding in y-direction
104
*
return this object for further modification
105
*/
106
public GBC setlpad(int ipadx, int ipady)
107
{
108
this.ipadx - ipadx;
109
this,ipady = ipady;
no
return this;
m
}
112 }
528
Java 核心技?
[ wj java.awt.GridBagConstraints 1.0
•i n t g r i d x, g r i d y
指定单元格的起始行和列。 默认值为 0。
•i n t g r i d w i d t h, g r i d h e i g h t
指定单元格的行和列的范围。默认值为 1。
•d o u b l e weightx, weighty
指定单元格扩大时的容量 3 默认值为 0。
•i n t a n c h o r
表示组件在单元格内的对齐方式。可以选择的绝对位置有:
NORTHWEST
WEST
SOUTHWEST
NORTH
NORTHEAST
CENTER
EAST
SOUTH
SOUTHEAST
或者各个方向上的相对位置：
FIRST_LINE_START
LINE_START
PAGE_START
CENTER
LAST一LINE一START
LINE
—
END
FIRST_LINE_ENO
PAGE一END
LAST一LINE_END
如果应用程序有可能从右向左， 或者自顶至底排列文本， 就应该使用后者。 默认值为
CENTERo
•i n t f i l l
指定组件在单元格内的填充行为， 取值为 NONE、 BOTH、 HORIZONTAL, 或者
VERTICAL。默认值为 NONE。
•i n t ipadx, i p a d y
指定组件周围的“ 内部” 填充。默认值为 0。
•I n s e t s i n s e t s
指定组件边框周围的“ 外部” 填充。默认为不填充。
•G r i d B a g C o n s t r a i n t s( i n t
g r i d x ,
i n t
g r i d y,
i n t
g r i d w i d t h ,
i n t
g r i d h e i g h t ,
d o u b l e weightx,
d o u b l e w e i g h t y,
i n t
a n c h o r,
i n t f i l l ,
I n s e t s i n s e t s, i n t ipadx, i n t i p a d y) 1 . 2
用参数中给定的所有域值构造 GridBagConstraints。Sun 建议这个构造器只用在自动代
码生成器中，因为它会使得代码非常难于阅读。
12.6.2
组 布 局
在讨论 GroupLayout 类的 API 之前， 先快速地浏览一下 NetBeans 中的 Matisse GUI 构
造器。 这里并没有讲述 Matisse 全部的使用方法， 有关更加详细的信息请参看网站 http://
netbeans.org/ kb/docs/java/quickstart-gui.html 0
第 章
Swing 用 ? 界 面 ? 件
5 2 9
下面是布局如图 12-13 所示的对话框的工作流程。 创建一个新项 0 , 并添加一个新的
JFrame 表单。拖拽一个标签， 直到出现了两条分离于容器边框的引导线：
卜
111
在第一行下面放置另一个标签：
I
iLabell
|Labe[2[
拖拽一个文木域， 让文本域的基线与第一个标签的基线对齐。再次注意引导线
[ilfixiELfiidll-
jLabel2
最后， 放置一个密码域，它的左侧是标签， 上方是文本域。
jLabell IjTextFieldTI
Matisse 将这些操作转换成下列 Java 代码：
1ayout ■setHorizontalCroup(
1ayout.createParallelCroup(GroupLayout.A1ignment.LEADING)
.addGro叩(1ayout.createSequentialCroupO
.addContainerCapO
.addGro叩(1ayout • createParal1elGroup(Gro叩Layout.A1ignment.LEADING)
•addGro叩(1ayout.createSequentialGroup0
.addComponent(jLabel1)
.addPreferredCap(LayoutStyle.ComponentPlacement.RELATED)
.addComponent(jTextFieldl))
.addCroupO ayout.createSequentialGroupO
.addComponent(jLabel 2)
.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
.addComponent(jPasswordFieldl)))
.addContainerGap(222, Short.MAX_VALUE)));
530
Java 核心技?
1ayout.setVerticalGroup(
1ayout.createParallelGro叩(Gro叩Layout.A1ignment.LEADING)
.addGroup(1ayout.createSequentialCroup()
.addContainerCapO
.addGro叩(1ayout.createParallelCroup(CroupLayout.Alignment.BASELINE)
.addComponent(jLabel1)
.addComponent(jTextFieldl))
.addPreferredCap(LayoutStyle.ComponentPIacement.RELATED)
.addGroup(1ayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
.addComponent(jLabel 2)
.addComponent(jPasswordFieldl))
.addContainerCap(244, Short.MAX_VALUE)));
看起来有点让人感觉惊恐， 庆幸的是不需要编写代码。阅读一下这段代码会有助于理解
布局行为的操作过程， 以便能够发现程序中的错误。 下面分析一下这段代码的基本结构。 在
本节后面有关 API 注解中将更加详却地解释每个类和方法。
可以通过将组件放人 GroupLyout.SequentialGroup 或者 GroupLayout.ParallelGroup 对象
中将它们组织起来。这些类是 GroupLayout.Group 的子类。 在组中可以包含组件、 间距和内
嵌的组。 由于组类中的各种 add 方法都返回组对象， 因此可以像下面这样将方法调用串联在
一起：
gro叩.addComponent(. . ,).addPreferredGap(. . .).addComponent(. . .)；
正像从示例代码屮所看到的， 组布局分别对水平和垂直布局进行计算。
为了能够看到水平计算的效果， 假设组件都被压平了， 因此高度为 0, 如下所示：
Container
border
jLabell
jTextFieldl
Gap
jLabel2
Gap
Gap
jPasswordField]
有两个平行的组件序列， 对应的代码 （略有简化）是：
.addContaine「Gap()
• addGroup(1ayout.createParallelCroupO
•addGroup0ayout.createSequentialGroup0
.addComponent(jLabel1)
.addPreferredGap(LayoutStyle.ComponentPIacement.RELATED)
.addComponent(jTextFieldl))
.addGroup(1ayout ■ createSequentialGroup()
.addComponent(jLabel 2)
•addPreferredCap(LayoutStyle.ComponentPlacement.RELATED)
•addComponent(jPasswordFieldl)))
但是， 请稍等， 上面这段代码有问题。 如果两个标签的长度不一样， 那么文本域和密码域就
无法对齐。
第 章
Swing 用 ? 界 面 ? 件
5 3 1
必须通知 Matisse， 这里希望将组件对齐。选择这两个域， 然后点击鼠标右键， 并从菜单
中选择 Align
—
Left to Column。 同样可以对齐标签。 如图 12-32 所示。
extFi£ldllL
Edh Text
Change Variable Name
Brents
Anchor
►
Right to Column
Auto Resizing
>
Top to Row
Same Size
►
Bottom
Set Default Size
Sp&c« Around Component
Move Up
Move Down
Right
Top
Bottom
Cut
£opy
delete
Ctri-X
Ctrl-C
Delete
图 12-32
在 Matisse
T
8Lah>li| lifexiFieldlj
Edit Text
Change Variable Name
Events
►
Left to Column
Anchor
►
Auto Resizing
¥
Same Size
¥
Top to Row
Bottom to Row
Set Default Size
Space Around Component .
Left
Right
Move Up
Move Down
Top
Bottom
Cut
Ctrl-X
Copy
Ctrl-C 厂
中对齐标签和文本域
这样一来显著地改变了布局代码：
.addCroup(layout.createSequentialCroupO
•addContainerCapO
. addCroupOayout _ createParallelCroup(GroupLayout.Alignment.LEADING)
.addComponent(]
• Label1, GroupLayout.Ali gnment .TRAILING)
.addComponent(jLabel 2, Gro叩Layout.A1ignment.TRAILING))
.addPreferredCap(LayoutStyle.ComponentPlacement.RELATED)
.addCroupOayout.createParallelCroup(CroupLayout.A1ignment.LEADING)
.addComponent(jTextFieldl)
.addComponent(jPasswordFieldl))
现在， 标签和域分别置于两个平行组中。 第一组的对齐方式是 TRAILING (这意味着当
文本方向是自左向右时， 应该右对齐 )：
Contain©
i
border
jUbell
jTextfieldl
|
Ga :、
jlabeU
GaP jPass rdfieldl
简直太奇妙了！ Matisse 居然可以将设计者的指令转换成嵌套的组。 其实， 正如 Arthur
C.Clarke 所说： 任何高级技术都源于奇特的想法。
鉴于完整性的考虑， 下面看一下垂直计算。此时， 应该将组件看作没有宽度。 这里有一
个顺序排列的组， 其中包含了两个用间距分隔的平行组：
532
Java 核心技?
一—
r
C
Itanin
卜卜
‘
,
卜一~4
一i
-
一
对应的代码是:
1ayout.createSequentialGroupO
.addContainerCapO
.addGroup0ayout.createParailelCroup(CroupLayout.Alignment.BASELINE)
.addComponentCjLabel1)
.addComponent(jTextFieldl))
.addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
.addGro叩(1ayout.createParallelGro叩(Gro叩Layout.A1ignment.BASELINE)
.addComponentCjLabel 2)
.addComponentCjPasswordFieldl))
从这段代码可以看到： 组件依据基线对齐（基线是组件文本对齐的直线）。
可以将一组组件的大小强制为相等。 例如， 可能想确保文本域和密码域的宽度相等。在
Matisse 中， 选择这两个组件， 然后点击鼠标右键， 并从菜单中 Same Size—
► Same Width。 如
图 12-33 所示。
.
鲜
3.
Change Variable
Size
id Component.
Move Up
Move Down
Copy
Ctrl-X
CtrJ-C
图 12-33
强制两个组件的宽度相等
Matisse 将下面这条语句添加到布局代码中：
1ayout.1inkSize(SwingConstants.HORIZONTAL, new Component ] {jPasswordFieldl, jTextFieldl});
在程序清单 12-12 的代码中显示了如何用 GroupLayout 替代 GridBagLayout 来布局前面
讲述的字体选择器。这里的代码看起来可能比程序清单 12-10 要复杂些， 但不必编写。 可以
12 jr
Swing 用?界面?件
533
使用 Matisse 进行布局， 然后， 对少量的代码整理一下即可。
程序清单 12-12
groupLayout/FontFrame.java
1 package gro叩Layout;
2
3 import java.awt.Font;
4 import java.awt.event.ActionListener;
s
6 import :javax.swing.Bo「de「Factory;
7 import javax.swing.GroupLayout;
s import javax.swingJCheckBox;
9 import javax.swingJComboBox;
10 import javax.swing.]Frame;
u import javax.swing.]Label;
12 import javax.swing.]ScrollPane;
13 import javax.swingJTextArea;
14 import javax.swing.LayoutStyle;
is import javax.swing.SwingConstants:
16
17 /**
is
* A frame that uses a group layout to arrange font selection components.
19
V
20 public class FontFrame extends ]Frame
21 {
22
public static final int TEXT_R0WS = 10;
23
public static final int TEXT_C0LUMNS = 20;
25
private ]ComboBox<String> face;
26
private 3ComboBox<Integer> size;
27
private ]CheckBox bold;
28
private JCheckBox italic;
29
private 3ScrollPane pane;
private JTextArea sample;
30
31
33
34
37
38
39
40
public FontFrame()
{
ActionListener listener = event -> updateSample();
// construct components
]Label faceLabel = new ]Label ("Face: ")；
face = new ]ComboBoxo(new String ] { "Serif", "SansSerif", "Monospaced",
"Oialoglnput" });
face.addActionListener(1istener);
]Label sizeLabel = new ]Label ("Size: ")；
size = new JComboBox<>(new Integers { 8，10， 12，15， 18， 24, 36, 48 });
size.addActionListener(1istener);
"Dialog",
bold = new JCheckBox("Bold");
bold.addActionListenerflistener);
534
Java 核心技?
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
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
italic = new ]CheckBox("Italic");
italic.addActionlistener(1istener);
sample = new 3TextArea(TEXT_R0WS, TEXT.COLUMNS);
sample.setText("The quick brown fox jumps over the lazy dog")；
sample.setEditable(false);
sample,setLineWrap(true);
sample.setBorder(BorderFactory.createEtchedBorderO);
pane = new ]ScrollPane(sample);
Gro叩Layout layout = new CroupLayout(getContentPaneO)；
setLayout(layout);
1ayout.setHorizontalCroup(1ayout.createParallelCroup(CroupLayout.Alignment.LEADING)
•addGroup(
1ayout.createSequentialGroup().addContaine「Gap().addGroup(
1ayout.createParallelGroup(Gro叩Layout.Alignment.LEADING).addGroup(
Gro叩Layout _ Alignment.TRAILING,
1ayout.createSequentialGroup().addCroup(
1ayout.createParallelGroup(GroupLayout.Alignment.TRAILING)
.addComponent(faceLabel).addComponent(sizeLabel))
.addPreferredCap(LayoutStyle.ComponentPlacement.RELATED)
.addGroup(
1ayout.createParallelCroup(
Gro叩Layout _ Alignment.LEADING, false)
■ addComponent(size).addComponent(face)))
.addComponent(italic).addComponent(bold)).addPreferredCapC
LayoutStyle.ComponentPlacement.RELATED).addComponent(pane)
.addContainerCapO));
1ayout.1inkSize(SwingConstants.HORIZONTAL, new java.awt.Component[] { face, size });
1ayout.setVerticalCroup(1ayout.createParallelCro叩(GroupLayout.Alignment.LEADING)
.addGroupC
layout.createSequentialGroup().addContainerGapO.addGroup(
1ayout.createParallelGro叩(GroupLayout.Alignment.LEADING).addComponent(
pane, CroupLayout.Alignment.TRAILING).addCroup(
1ayout.createSequentialGroupO •addGr( p(
1ayout.createParallelGroup(GroupLayout.Alignment.BASELINE)
■ addComponent(face).addComponent(faceLabel))
•addPreferredCap(LayoutStyle.ComponentPlacement.RELATED)
.addGroupC
1ayout.createParallelCroup(
GroupLayout.Alignment.BASELINE).addComponent(size)
.addComponent(sizeLabel)).addPreferredCapC
LayoutStyle.ComponentPlacement.RELATED).addComponent(
italic, CroupLayout.DEFAULT.SIZE,
GroupLayout.DEFAULT.SIZE, Short.MAX.VALUE)
.addPreferredCap(LayoutStyle.ComponentPIacement.RELATED)
.addComponent(bold, CroupLayout.DEFAULT.SIZE,
CroupLayout.DEFAULT.SIZE, Short.MAX.VALUE)))
.addContainerGapO))；
pack();
public void updateSampleQ
第 12 章
Swing 用?界面?件
535
112
115
116
117
String fontFace = (String) face.getSelectedltemO ;
int fontStyle = (bold.isSelectedO ? Font.BOLD : 0)
+ (italic.isSelectedO ? Font.ITALIC : 0) ;
int fontSize = size.getltemAt(size.getSelectedlndex()) ;
Font font = new Font(fontFace, fontStyle, fontSize) ;
sample.setFont(font);
sample, repaint();
}
javax.swing.GroupLayout 6
•GroupLayout( C o n t a i n e r h o s t )
构造一个 GroupLayout 对象， 用于布局 host 容器中的组件（注意：host 容器仍然需要
调用 setLayout )。
•v o i d s e t H o r i z o n t a l G r o u p( GroupLayout .Group g )
•v o i d s e t V e r t i c a l Group( GroupLayout . Group g )
设置用于控制水平或垂直容器的组。
•v o i d 1 i n k S i z e( Component . .. components )
•v o i d l i n k S i z e( i n t a x i s,
Component ... component )
强制给定的几个组件具有相同的尺寸， 或者在指定的坐标轴上有相同的尺寸 （ Swing
Constants.HORIZONTAL 或者 SwingContants.VERTICAL )。
•GroupLayout .SequentialGroup c r e a t e S e q u e n t i a l G r o u p( )
创建一个组， 用于顺序地布局子组件。
•GroupLayout . P a r a l 1e l Group c r e a t e P a r a l l e1Group( )
•G r o u p L a y o u t . P a r a l l e i G r o u p
c r e a t e P a r a 1 1 e l G r o u p( G r o u p L a y o u t .
Alignment a l i g n )
•G r o u p L a y o u t . P a r a l l e i G r o u p
c r e a t e P a r a l 1 e l G r o u p( G r o u p L a y o u t .
Alignment a l i g n , b o o l e a n r e s i z a b l e )
创建一个组， 用于并行地布局子组件。
参数： align
BASELINE、 LEADING ( 默认值)、 TRAILING 或 CENTER
resizable
如果组可以调整大小， 这个值为 true ( 默认值）；如果首选的尺寸是
最小尺寸或最大尺寸，这个值为 false
•b o o l e a n g e t H o n o r s v i s i b i l i t y( )
•v o i d s e t H o n o r s v i s i b i l i t y( b o o l e a n b )
获取或设置 honorsVisibility 特性。 当这个值为 true ( 默认值）时， 不可见的组件不参
与布局。当这个值为 false 时， 好像可见的组件一样， 这些组件也参与布局。 这个特
性主要用于想要临时隐藏一些组件， 而又不希望改变布局的情况。
•b o o l e a n getAutoCreateCaps( )
•v o i d s e t A u t o C r e a t e C a p s( b o o l e a n b )
536
Java 核心技?
•boolean getAutoCreateContainerCaps( )
•void setAutoCreateContainerCaps(boolean b )
获取或设置 autoCreateCaps 和 autoCreateContainerCaps 特性。 当这个值为 true 时， 将
自动地在组件或容器边框之间添加空隙。 默认值是 false。 在手工地构造 GmupLayout
曰寸， true 值很有用。
APij javax.swing.GroupLayout.Group
•GroupLayout .Group addComponent(Component c )
•GroupLayout.Group
addComponent( Component
c, int
minimumSize, int
preferredSize, int maximumSize )
添加一个组件至本组中。尺寸参数可以是一个实际值 （非负值 )， 或者是一个特定
的常量 GroupLayout.DEFAULT_SIZE 或 GroupLayout.PREFERRED_SIZE。 当 使 用
DEFAULT_SIZE， 将调用组件的 getMinimumSize 、 getPreferredSize 或 getMaximumSize。
当使用 PREFERRED_SIZE 时， 将调用组件的 getPreferredSize 方法。
•GroupLayout.Group addCap(int size)
•GroupLayout.Group
addCap(int
minimumSize, int preferredSize, int
maximumSize )
添加一个间定的或可调节的间隙。
參 GroupLayout.Group addGroup(GroupLayout.Group g)
将一个给定的组添加到本组中。
[ API|javax.swing.GroupLayout.ParallelGroup
•GroupLayout.Para 1el Group
addComponent(Component
c ,
GroupLayout.
Alignment align)
•GroupLayout.Paral 1el Group
addComponent( Component
c,
GroupLayout.
Alignment align, int minimumSize,
int preferredSize,
int maximumSize)
•G r o u p L a y o u t . P a r a l l e i G r o u p
a d d G r o u p ( G r o u p L a y o u t . G r o u p
g ,
GroupLayout. A1ignment a l i g n)
利用给定的对齐方式（ BASELINE、 LEADING、 TRA1L1NC 或 CENTER ) 添加一个组
件或组至本组中。
*n|javax.swing.GroupLayout.SequentialGroup
•GroupLayout .SequentialGroup addContainerCap( )
•GroupLayout.Sequential Group addContainerCap(int preferredSize, int
maximumSize )
为分隔组件和容器的边缘添加一个间隙。
•G r o u p L a y o u t . S e q u e n t i a l G r o u p
a d d P r e f e r r e d C a p ( L a y o u t S t y1 e .
第 J2 章
Swing 用 ? 界 面 ? 件
5 3 7
ComponentPlacement type)
为分隔组件添加一个间隙。间隙的类型是 LayoutStyle.ComponentPlacement.RELATED
或 LayoutStyle.ComponentPlacement。
12.6.3
不使用布局管理器
有时候用户可能不想使用任何布局管理器， 而只想把组件放在一个固定的位置上（通常
称为绝对定位)。这对于与平台无关的应用程序来说并不是一个好主意，但可用来快速地构
造原型。
下面是将一个组件定位到某个绝对定位的步骤：
1 ) 将布局管理器设置为 null。
2 ) 将组件添加到容器中。
3 ) 指定想要放置的位置和大小。
frame.setLayout(null);
]Button ok = new ]Button("OK")；
frame.add(ok);
ok.setBoundsdO, 10, 30, 15);
j*
pi|java.awt.Component 1.0
• void setBounds(int x, int y, int width, int height)
移动并调节组件的尺寸
参数： x, y
组件新的左上角位置
width, height
组件新的尺寸
12.6.4
定制布局管理器
原则上， 可以通过自己设计 LayoutManager 类来实现特殊的布局方式。 例如， 可以将容
器中的组件排列成一个圆形。 如图 12-34
所示。
定制布局管理器必须实现 LayoutManager
接口， 并且需要覆盖下面 5个施
void addLayoutComponent(String s, Component c);
void removeLayoutComponent(Component c);
Dimension preferredLayoutSize(Container parent);
Dimension minimumLayoutSize(Container parent);
void 1ayoutContainer(Container parent);
在添加或删除一个组件时会调用前面两个方法。如果不需要保存组件的任何附加信息，
那么可以让这两个方法什么都不做。接下来的两个方法计算组件的最小布局和首选布局所需
要的空间。两者通常相等。第 5 个方法真正地实施操作， 它调用所有组件的 setBounds 方法。
Q 注释： AWT 还有第二个接口 LayoutManager2, 其中包含 10 个需要实现的方法， 而不是
m
图 12-34
圆形布局
538
Java 核心技?
5 个。 这个接口的主要特点是允许用户使用带有约束的 add 方法。例如， BorderLayout 和
GridBagLayout 都实现了 LayoutManager2 接口 0
程序清单 12-13 简单实现了 CirdeLayout 管理器的代码， 在父组件中沿着圆形排列组件。
这个管理器很有趣， 但是没有什么实际的应用价值。示例程序的框架类见程序清单 12-14。
程序清单 12-13
circleLayout/CircleLayout.java
1
package circleLayout;
2
3 import java.awt.*;
4
5 /**
6
* A layout manager that lays out components along a circle.
7
V
8 public class CircleLayout implements LayoutManager
9 {
10
private int minWidth = 0;
11
private int minHeight = 0;
12
private int preferredWidth = 0;
13
private int preferredHeight = 0;
14
private boolean sizesSet = false;
is
private int maxComponentWidth = 0;
16
private int maxComponentHeight = 0;
17
is
public void addLayoutCofnponent(String name, Component comp)
21
22
23
public void removeLayoutComponent(Component comp)
public void setSizes(Container parent)
{
if (sizesSet) return;
int n = parent.getComponentCountO；
preferredWidth = 0;
preferredHeight = 0;
minWidth = 0;
minHeight = 0;
maxComponentWidth = 0;
maxComponentHeight = 0;
// compute the maximum component widths and heights
// and set the preferred size to the sum of the component sizes.
for (int i = 0; i < n; i++)
{
Component c = parent.getComponent(i);
if (c.isVisibleO)
{
Dimension d = c.getPreferredSizeO;
maxComponentWidth = Math.max(maxComponentWidth, d.width);
maxComponentHeight = Math.max(maxComponentHeight, d.height) ;
preferredWidth += d.width ;
49
SO
51
52
S3
S4
ss
S6
S7
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
8S
86
87
88
89
90
91
92
93
94
9S
96
97
98
99
100
101
102
103
104
10S
第 1 2 章
Swing 用 ? 界 面 ? 件
5 3 9
preferredHeight += d.height;
minWidth = preferredWidth / 2;
minHeight = preferredHeight / 2;
sizesSet = true;
public Dimension preferredlayoutSize(Container parent)
{
setSizes(parent);
Insets insets = parent.getlnsetsO；
int width = preferredWidth + insets.left + insets.right;
int height = preferredHeight + insets.top + insets.bottom;
return new Dimension(width, height);
public Dimension minimumLayoutSize(Container parent)
{
setSizes(parent);
Insets insets = parent.getlnsetsO;
int width = minWidth + insets.left + insets.right;
int height = minHeight + insets.top + insets.bottom;
return new Dimension(width, height);
public void layoutContainer(Container parent)
{
setSizes(parent);
// compute center of the circle
Insets insets = parent.getlnsetsO;
int containerWidth = parent.getSizeO.width - insets.left - insets.right;
int containerHeight = parent.getSizeO.height - insets.top - insets.bottom;
int xcenter = insets.left + containerWidth / 2;
int ycenter = insets.top + containerHeight / 2;
// compute radius of the circle
int xradius = (containerWidth - maxComponentWidth) / 2;
int yradius = (containerHeight - maxComponentHeight) / 2;
int radius = Math,min(xradius, yradius);
// lay out components along the circle
int n = parent.getComponentCountO；
for (int i = 0; i < n;i++)
{
Component c = parent.getComponent(i);
if (c.isVisibleO)
{
double angle = 2 * Math.PI * i / n;
// center point of component
int x = xcenter + (int) (Math,cos(angle) * radius);
540
Java 核心技?
106
int y = ycenter + (int) (Math,sin(angle) * radius);
107
108
// move component so that its center is (x, y)
109
// and its size is its preferred size
no
Dimension d = c.getPreferredSizeO；
in
c.setBounds(x - d.width / 2, y - d.height / 2, d.width, d.height);
112
}
113
}
114
}
115 }
程序清单 12-14
circleLayout/CircleLayoutFrame.java
1 package cirdeLayout;
2
3 import javax.swing.*;
4
5
/**
6
* A frame that shows buttons arranged along a circle.
7
V
8 public class CircleLayoutFrame extends ]Frame
9
{
.
10
public CirdeLayoutFrame()
11
{
12
setLayout(new CircleLayoutO);
13
add(new ]Button("Yellow"))；
14
add(new ]Button("Blue"))；
is
add(new ]Button("Red"))；
16
add(new ]Button("Green"));
17
add(new ]Button("0range"));
18
add(new ]Button("Fuchsia"));
19
add(new ]Button("Indigo"))；
20
pack();
21
}
AW| java.awt.LayoutManager 1.0
•
void addLayoutComponent(String name, Component comp)
将组件添加到布局中。
参数： name
组件位置的标识符
comp
被添加的组件
•
void removeLayoutComponent(Component comp)
从本布局中删除一个组件。
•
Dimension preferredLayoutSize(Container cont)
返回本布局下的容器的首选尺寸。
•
Dimension minimumLayoutSize(Container cont)
返回本布局中下容器的最小尺寸。
12t
Swing 用?界面?件
541
•void 1ayoutContainer(Container cont )
摆放容器内的组件。
12.6.5
遍历顺序
当把很多组件添加到窗口中时， 需要考虑遍历顺序（traversal order) 的问题。 窗口被初
次显示时，遍历序列的第一个组件会有键盘焦点。 每次用户按下 TAB 键， 下一个组件就会获
得焦点（回忆一下，具有键盘焦点的组件可以用键盘进行操作。例如， 如果按钮具有焦点，
按下空格键就相当于“ 点击” 它)。我们可能并不习惯使用 TAB 键遍历一组控件， 但是也有
很多用户喜欢这样做。这些人有可能厌恶鼠标， 也有可能由于残疾而无法使用鼠标， 或者采
用语言方式进行交互，所以应该知道 Swing 是如何设置遍历顺序的。
遍历顺序很直观， 它的顺序是从左至右， 从上至下。
例如， 在字体对话框例子中， 组件按照下面顺序进行遍历
(见图 12-35 ):
①外观组合框
②示例文本区（按下 CTRL+TAB 键移动到下一个文本
域， TAB 字符被认为是文本输入)。
③尺寸组合框
④加粗复选框
⑤斜体复选框
如果容器还包含其他的容器， 情况就更加复杂了。当焦点给予另外一个容器时，那个容
器左上角的组件就会自动地获得焦点，然后再遍历那个容器中的所有组件。最后，将焦点移
交给紧跟着那个容器的组件。
利用这一点，可以将相关元素组织在一起并放置在一个容器中。例如’
放置在一个面板中。
UIE
SHlfO
:
-w The quick Drown fox
|ps over the lazydog
O口
©□
图丨2-35
遍历顺序
El 注释： 调用
component.setFocusable(false);
可以从焦点遍历中删除一个组件。 这对于不接受键盘输入、 自行绘制的组件很有用。
1 2 . 7
对 话 框
到目前为止， 所有的用户界面组件都显示在应用程序创建的框架窗口中。这对于编写运
行在 Web 浏览器中的 applets 来说是十分常见的情况。但是， 如果编写应用程序，通常就需
要弹出独立的对话框来显示信息或者获取用户信息。
与大多数的窗口系统一样， AWT 也分为模式对话框和无模式对话框。 所谓模式对话框是
指在结束对它的处理之前， 不允许用户与应用程序的其余窗口进行交互。模式对话框主要用
于在程序继续运行之前获取用户提供的信息。例如， 当用户想要读取文件时，就会弹出一个
542
Java 核心技?
模式对话框。用户必须给定一个文件名， 然后程序才能够开始读操作。只有用户关闭（模式)
对话框之后， 应用程序才能够继续执行。
所谓无模式对话框是指允许用户同时在对话框和应用程序的其他窗口中输入信息。使用
无模式对话框的最好例子就是工具栏。工具栏可以停靠在任何地方，并且用户可以在需要的
时候，同时与应用程序窗口和工具栏进行交互。
本节从最简单的对话框开始
个简单信息的模式对话框。Swing 有一个很容易使用
的类 JOptionPane， 它可以弹出一个简单的对话框， 而不必编写任何对话框的相关代码。 随
后， 将看到如何通过实现自己的对话框窗口来编写一个复杂的对话框。最后， 介绍在应用程
序与对话框之间如何传递数据。
本节用两个标准的对话框结束： 文件对话框和颜色对话框。文件对话框比较复杂， 为
此需要熟悉 Swing 中的 JFileChooser
自已编写文件对话框是一项颇有挑战性的任务。
JColorChooser 对话框可用来让用户选取颜色。
12.7.1
选项对话框
Swing 有一套简单的对话框， 用于获取用户的一些简单信息。JOptionPane 有 4 个用于显
示这些对话框的静态方法：
showMessageDialog:
showConfirmDialog:
showOptionDialog:
showInputDialog:
显示一条消息并等待用户点击 OK
显示一条消息并等待用户确认（与 OK/Cancel 类似）
显示一条消息并获得用户在一组选项中的选择
显示一条消息并获得用户输人的一行文本
匾
图 12-36 显示了一个典型的对话框。可以看到， 对话框有下列组件：
•一个图标
•一条消息
•一个或多个按钮
输人对话框有一个用于接收用户输入的额外组件。它既可
能是用于输入任何字符串的文本域， 也可能是允许用户从中选
择的组合框。
这些对话框的确切布局和为标准消息类型选择的图标都取决于具体的观感。
左侧的图标将由下面 5 种消息类型决定：
wsaJ.J
图 12-36
选项对话框
ERROR.MESSACE
INFORMATIONJESSACE
WARNINCJESSACE
QUESTIONJESSACE
PLAINJESSACE
PLAIN_MESSAGE 类型没有图标。每个对话框类型都有一个方法，可以用来提供自己的
图标， 以替代原来的图标。
可以为每个对话框类型指定一条消息。这里的消息既可以是字符串、 图标、用户界面组
第 口 章
Swing 用 ? 界 面 ? 件
5 4 3
件， 也可以是其他类型的对象。 下面是显示消息对象的基本方式:
String:
Icon：
Component
Object□:
绘制字符串
显示图标
显示组件
显示数组中的所有对象， 依次叠加
任何其他对象：
调用 toString 方法来显示结果字符串
可以运行程序清单 12-15 中的程序， 查看一下这些选项。
当然， 提供字符串消息是最常见的情况，而提供一个 Component 会带来更大的灵活性。
这是因为通过调用 paintComponent 方法可以绘制自己想要的任何内容。
位 于 底 部 的 按 钮 取 决 于 对 话 框 类 型 和 选 项 类 型。 当 调 用 showMessageDialog 和
showInputDialog 时， 只能看到一组标准按钮（分别是 OK/Cancel)。 当调用 showConfirmDialog
时， 可以选择下面四种选项类型之一：
DEFAULL0PTI0N
YES_NO_OPTION
YES_N0_CANCEL_0PTI0N
OK_CANCELJPTION
使用 showOptionDialog 可以指定任意的选项。这里需要为选项提供一个对象数组。每个
数组元素可以是下列类型之一：
String：
使用字符串标签创建一个按钮
Icon：
使用图标创建一个按钮
Component：
显示这个组件
其他类型的对象： 使用 toString 方法，然后用结果字符串作为标签创建按钮
下面是这些方法的返回值：
showMessageDialog 无
showConfirmDialog 表示被选项的一个整数
showOptionDialog
表示被选项的一个整数
showInputDialog
用户选择或输入的字符串
showConfirmDialog 和 showOptionDialog 返回一个整数用来表示用户选择了哪个按钮。
对于选项对话框来说， 这个值就是被选的选项的索引值或者是 CLOSED_OPTION (此时用户
没有选择可选项， 而是关闭了对话框)。对于确认对话框， 返回值可以是下列值之一：
0KJPTI0N
CANCEL0PTI0N
VESJPTION
N0_0PTI0N
CLOSED.OPTION
这些选项似乎令人感到迷惑不解， 实际上非常简单步骤如下:
1 ) 选择对话框的类型（消息、 确认、选项或者输人 )。
2 ) 选择图标（错误、信息、 警告、 问题、 无或者自定义)。
544
Java 核心技?
3 ) 选择消息（字符串、 图表、 自定义组件或者它们的集合 )。
4 ) 对于确认对话框， 选择选项类型 （默认、 Yes/No、 Yes/No/Cancel 或者 Ok/Cancel )。
5 ) 对于选项对话框， 选择选项 （字符串、 图表或者自定义组件） 和默认选项。
6 ) 对于输人对话框， 选择文本框或者组合框。
7 ) 调用 JOptionPane API 中的相应方法。
例如， 假设需要显示一个图 12-36 的对话框。 这个对话框显示了一条消息， 并请求用户
确认或者取消。 这是一个确认对话框。图标是警告图标， 消息是字符串， 选项类型是 OK_
CANCEL_OPTION。调用如下：
int selection = JOptionPane.showConfirfnDialog(parent,
"Message", "Title",
]0ptionPane.0K_CANCEL_0PTI0N,
]0ptionPane.QUESTION_MESSACE);
if (selection == ]0ptionPane.0K_0PTI0N) . . .
提示： 消息字符串中可以包含换行符 （'\n')。 这样就可以让字符串多行显示;，
这个程序显示了 6 个按钮面板 （见图 12-37 ), 其框架类在程序清单 12-15 中给出。 程序
清单 12-16 显示了这些面板的类。 点击 Show 按钮时， 会显示所选的对话框。
f Type
Message Type
—
—
Message
O Message
O ERROK.MESSACE
⑱ Suing
<S> Confirm
O 1NF0RMATI0N_MESSAGE
O Icon
O Option
O WARNINC.MESSAGE
O Component
O Input
® QUESTION.MESSACE
O Other
, 1
O PLAIN_MESSACE
O Objettl)
! O DEFAULT-OPTION
® Stringf)
# Texi field
| O YCS.NO.OPTION
O lcon()
O Combo box
| Q YES_NO_CANCELOPTION
Q Objean
；# OICCANCELOPTION
1
1
图 12-37
OptionDialogTest 程序
程序清单 12-15
optionDialog/OptionDialogFrame.java
1 package optionDialog;
2
3 import java.awt.*;
4 import java.awt
_ event.*;
5 import java.awt.geom.*;
6 import java.util.*;
7 import javax.swing.*;
9 /**
10
11
12
13
14
IS
16
17
18
19
20
21
22
23
24
2S
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
4S
46
47
48
49
50
SI
S2
SB
S4
55
S6
S7
S8
S9
60
61
62
63
64
6S
66
第 J2 章
Swing 用?界面?件
545
* A frame that contains settings for selecting various option dialogs.
V
public class OptionDialogFrame extends ]Frame
{
private ButtonPanel typePanel :
private ButtonPanel messagePanel ;
private ButtonPanel messageTypePanel ;
private ButtonPanel optionTypePanel ;
private ButtonPanel optionsPanel ;
private ButtonPanel inputPanel ;
private String messagestring = "Message";
private Icon messagelcon = new Imagelcon("blue-ball .gif');
private Object messageObject = new Date。
;
private Component messageComponent = new SampleComponentO ;
public OptionDialogFrame()
{
]Panel gridPanel = new ]Panel O；
gndPanel.setLayout(new CridLayout(2, 3));
typePanel = new ButtonPanel ("Type", "Message" , "Confirm", "Option", "Input"):
messageTypePanel = new ButtonPanel ("Message Type", "ERROR.MESSAGE", "INFORMATION.MESSACE",
"WARNINCJESSAGE", "QUESTION.MESSAGE", "PLAINJESSAGE");
messagePanel = new ButtonPanel ("Message" , "String", "Icon", "Component", "Other",
"ObjectD");
optionTypePanel = new ButtonPanel ("Confirm", "DEFAULT_0PTI0N", HYES_N0_0PTI0N",
"YESJO.CANCEL.OPnON", "0K_CANCEL_0PTI0N");
optionsPanel = new ButtonPanel ("Option", "String[]", "Icon口"，"Object[]")；
inputPanel = new ButtonPanel ("Input", "Text field", "Combo box");
gridPanel .add(typePanel );
gridPanel .add(messageTypePanel);
gridPanel.add(messagePanel) ;
gridPanel .add(optionTypePanel );
gridPanel .add(optionsPanel);
gri dPanel .add(i nputPanel);
// add a panel with a Show button
]Panel showPanel = new ]Panel ();
]Button showButton = new ]Button("Show") ;
showButton.addActionListener(new ShowAction()) ;
showPanel .add(showButton) ;
add(gridPanel , Borde「Layout.CENTER);
add(showPanel , Borde「Layout.SOUTH) ;
packQ:
/**
* Gets the currently selected message.
©return a string, icon, component, or object array, depending on the Message panel selection
V
public Object getMessageO
{
String s = messagePanel.getSelection():
if (s.equals("String")) return messageString;
546
Java 核心技?
67
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
114
118
119
120
121
122
123
else if (s.equals("Icon")) return messagelcon;
else if (s.equals("Component")) return messageComponent;
else if (s.equals("0bject[]")) return new Object[] { messagestring, messagelcon,
messageComponent, messageObject };
else if (s.equals("0ther")) return messageObject;
else return null ;
}
/**
* Gets the currently selected options.
* ©return an array of strings, icons, or objects, depending on the Option panel selection
V
public Object[] getOptionsO
{
String s = optionsPanel .getSelection();
if (s.equalsCStringD")) return new String[] { "Yellow", "Blue", "Red" };
else if (s.equals("Icon[]")) return new Icon[] { new Imagelcon("yellow-ball .gif") ,
new ImageIconrblue-ball .gif"), new Imagelcon("red-bal1.gi f") };
else if (s.equals("0bject[]")) return new Object[] { messageString, messagelcon,
messageComponent, messageObject };
else return null ;
/**
* Gets the selected message or option type
* @param panel the Message Type or Confirm panel
* @return the selected XXX MESSAGE or XXX.OPTION constant from the JOptionPane cl ass
V
public int getType(ButtonPanel panel)
{
String s = panel .getSel ecti on();
try
{
return ]0ptionPane.class.getField(s).getlnt(null) ;
}
catch (Exception e)
{
return -1;
r
* The action listener for the Show button shows a Confirm, Input, Message, or Option dialog
* depending on the Type panel selection.
V
private class ShowAction implements ActionListener
{
public void actionPerformed(ActionEvent event)
{
if (typePanel .getSel ecti on()•equals("Confirm")) JOpti onPane.showConfi rmDi al og(
OptionDialogFrame.this, getMessageO , "Title", getType(optionTypePanel) ,
getType(messageTypePanel));
else if (typePanel .getSel ecti on().equals("Input"))
{
if (inputPanel .getSelecti on().equals("Text field")) ]0pti onPane.showInputDialog(
OptionDialogFrame.this, getMessage() , "Title", getType(messageTypePanel ));
else JOpti onPane.showInputDi al og(Opti onDi al ogFrame.this, getMessageQ , "Title",
第 12 章
Swing 用 ? 界 面 ? 件
5 4 7
124
125
127
128
129
getType(messageTypePanel), null, new String[] { "Yellow", "Blue", "Red" },
"Blue");
}
else if (typePanel.getSelection().equals("Message")) JOptionPane.showMessageDialog(
OptionDialogFrame.this, getMessageO, "Title", getType(messageTypePanel)):
else if (typePanel •getSelection() • equals("Option")) ]0ptionPane.showOptionDialog(
OptionDialogFrame,this, getMessageO, "Title", getType(optionTypePanel),
131
getType(messageTypePanel), null, getOptionsO, getOptions() [0]);
132
}
133
}
134 }
135
136 /**
137 * A component with a painted surface
138
*/
139
HO class SampleComponent extends JComponent
{
public void paintComponent(Graphics g)
{
Craphics2D g2 = (Graphics2D) g;
Rectangle2D rect = new Rectangle2D.Double(0, 0, getWidth() - 1, getHeightO - 1);
g2.setPaint(Color.YELLOW);
g2.fi11(rect);
g2.setPaint(Color.BLUE);
g2.draw(rect);
144
14 S
146
148
151
152
153
154
155
156 }
}
public Dimension getPreferredSizeO
{
return new Dimension(10, 10);
}
程序清单 12-16
optionDialog/ButtonPanel.java
i package optionDialog;
3
import javax.swing.*;
5 /**
6
* A panel with radio buttons inside a titled border.
7
V
8 public class ButtonPanel extends ]Panel
9
{
10
private ButtonCroup gro叩；
u
12
/**
13
* Constructs a button panel.
14
*
@param title the title shown in the border
is
*
@param options an array of radio button labels
16
*/
17
public ButtonPanel(String title, String... options)
18
{
19
setBorder(BorderFactory.createTitledBorder(BorderFactory.createEtchedBorder(), title));
20
setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
548
Java 核心技?
group > new ButtonCroupO:
// make one radio button for each option
for (String option : options)
{
]RadioButton b = new ]RadioButton(option) ;
b•setActionCommand(option) ;
add(b);
group,add(b);
b.setSelected(option == options[0]);
/**
* Gets the currently selected option.
* ©return the label of the currently selected radio button.
V
public String getSelection()
{
return group.getSelection().getActionCommand();
[ APij javax.swing.JOptionPane 1.2
• static
void
showMessageDialog(Component
parent,
Object
message,
String title, int messageType, Icon icon)
• static
void
showMessageDialog(Component
parent,
Object
message,
String title, int messageType)
• static void showMessageDialog(Component parent , Object message)
• static
void
showlnternalMessageDialog(Component
parent,
Object
message, String title, int messageType, Icon icon)
• static
void
showlnternalMessageDialog(Component
parent,
Object
message, String title, int messageType)
• static
void
showlnternalMessageDialog(Component
parent,
Object
message)
显示-
参数:
个消息对话框或者一个内部消息对话框（内部对话框完全显示在所在的框架内)。
parent
父组件 （可以为 null )。
message
显示在对话框中的消息（可以是字符串、图标、 组件或者一
个这些类型的数组 )。
title
对话框标题栏中的字符串。
messageType
取值为 ERROR_MESSAGE、 INFORMATION
—
MESSAGE、
WARNING_ MESSAGE、 QUESTION_MESSAGE、 PLAIN
MESSAGE 之一。
icon
用于替代标准图标的图标。
第 /2 章
Swing 用 ? 界 面 ? 件
5 4 9
• static
int showConfirmDialog(Component
parent,
Object
message,
String title, int optionType, int messageType, Icon Icon)
• static
int
showConfirmDialog(Component
parent ,
Object
message,
String title, int optionType, int messageType)
• static
int
showConfirmDialog(Component
parent,
Object
message,
String title, int optionType)
• static int showConfirmDialog(Component parent, Object message)
• static
int
showInternalConfirmDialog(Component
parent ,
Object
message, String title, int optionType, int messageType, Icon icon)
• static
int
showInternalConfirmDialog(Component
parent ,
Object
message, String title, int optionType, int messageType)
• static
int
showInternalConfirmDialog(Component
parent,
Object
message, String title, int optionType)
• static
int
showlnternalConfirmDialog(Component
parent,
Object
message)
显示一个确认对话框或者内部确认对话框 （内部对话框完全显示在所在的框架内）。
返冋用户选择的选项 （取值为 OK_OPTION, CANCEL OPTION, YES_OPTION, NO_
OPTION )；如果用户关闭对话框将返回 CLOSED_OPTION。
参数：parent
message
title
messageType
父组件 （可以为 null )。
显示在对话框中的消息 （可以是字符串、 图标、 组件或者一
个这些类型的数组）。
对话框标题栏中的字符串。
取值为 ERROR_MESSAGE、 INFORMAT10N_MESSAGE、
WARNING_MESSAGE 、 QUESTION MESSAGE > PLAIN
MESSAGE 之一 0
optionType
取值为 DEFAULT_OPTION、 YES—N0_0PT10N、 YES_NO
CANCEL— OPTION、OK_CANCEL_OPTION 之一。
icon
用于替代标准图标的图标。
• static
int showOptionDialog(Component
parent, Object
message,
String title, int optionType, int messageType, Icon icon, Object[]
options, Object default)
參 static
int
showlnternalOptionDialog(Component
parent ,
Object
message, String title, int optionType, int messageType, Icon icon ,
Object[] options, Object default)
显示一个选项对话框或者内部选项对话框（内部对话框完全显示在所在的框架内）。返
回用户选择的选项索引； 如果用户取消对话框返冋 CLOSED_OPT丨ON。
550
Java 核心技?
参数： parent
父组件（可以为 null )。
message
显示在对话框中的消息 （可以是字符串，图标，组件或者一
个这些类型的数组）。
title
对话框标题栏中的字符串。
messageType
取值为 ERROR_MESSAGE、 INFORMATION MESSAGE <
WARNING_MESSAGE、 QUESTION_MESSAGE、 PLAIN_
MESSAGE 之一 0
optionType
取值为 DEFAULT_0PT10N、YES_NO_OPTION、 YES_NO_
CANCEL_OPTION、 OK_CANCEL_OPTION 之一。
icon
用于替代标准图标的图标。
options
一组选项（可以是字符串、 图标或者组件 )。
default
呈现给用户的默认值。
• static Object showInputDialog(Component parent, Object message, String
title, int messageType, Icon icon, Object[] values, Object default)
• static
String
showInputDialog( Component
parent,
Object
message,
String title, int messageType )
• static String showInputDialog( Component parent, Object message)
• static String showInputDialog( Object message)
• static
String
showInputDialog( Component
parent,
Object
message,
Object default ) 1.4
• static String showInputDialog(Object message, Object default ) 1.4
• static
Object
showinternalInputDialog( Component
parent,
Object
message,
String title,
int
messageType,
Icon
icon,
Object[ ]
values, Object default )
• static
String
showinternalInputDialog( Component
parent ,
Object
message, String title, int messageType)
• static String showinternalInputDialog(Component parent, Object message)
显示一个输入对话框或者内部输人对话框 （内部对话框完全显示在所在的框架内）。返
回用户输人的字符串； 如果用户取消对话框返回 null。
参数： parent
父组件 （可以为 null )。
message
显示在对话框中的消息 （可以是字符串、 图标、 组件或者一
个这些类型的数组）。
title
对话框标题栏中的字符串。
messageType
取值为 ERROR_MESSAGE、 INFORMATION_MESSAGE ,
WARNING—
MESSAGE、 QUESTION_MESSAGE、 PLAIN_
MESSAGE 之一。
第 12 章
Swing 用?界面?件
551
icon
用于替代标准图标的图标。
values
在组合框中显示的一组值。
default
呈现给用户的默认值。
12.7.2
创建对话框
在上一节中，介绍了如何使用 JOptionPane 来显示一个简单的对话框。 本节将讲述如何
手工地创建这样一个对话框。
图 12-38 显示了一个典型的模式对话框。当用户点击 About 按钮时就会显示这样一个程
序信息对话框。
要想实现一个对话框，需要从 JDialog 派生一个类。这与应用
程序窗口派生于 JFrame 的过程完全一样。具体过程如下：
1 ) 在对话框构造器中，调用超类 JDialog 的构造器。
2 ) 添加对话框的用户界面组件。
3 ) 添加事件处理器。
4 ) 设置对话框的大小。
在调用超类构造器时，需要提供拥有者框架（ownerframe)、 对话框标题及模式特征。
拥有者框架控制对话框的显示位置， 如果将拥有者标识为 null, 那么对话框将由一个隐
藏框架所拥有。
模式特征将指定对话框处于显示状态时，应用程序中其他窗口是否被锁住。无模式对话
框不会锁住其他窗口，而有模式对话框将锁住应用程序中的所有其他窗口（除对话框的子窗
口外)。用户经常使用的工具栏就是无模式对话框， 另一方面， 如果想强迫用户在继续操作
之前提供一些必要的信息就应该使用模式对话框。
0 注释：在 Java SE 6 中， 有两个额外的模式特征类型。文档-模式对话框将阻塞所有属于
相同“
文档” 的窗口。 更准确地说， 是所有作为对话框的具有相同无父根窗口的窗口。
这样解决了帮助系统的问题。在早期的版本中， 当弹出一个模式对话框时， 用户不可能
与帮助窗口交互。 工具箱对话框阻塞了所有来自相同“
工具箱” 的窗口。 工具箱是一个
运行于多个应用的 Java 程序， 例如， 浏览器中的 applet 引擎。有关更加详细的内容请参
看网站：www.oracle.com/technetwork/artides/javase/modality-137604.html。
下面是一个对话框的例子：
public AboutDialog extends JDialog
{
public AboutDialog(JFrame owner)
{
super(owner, "About DialogTest", true);
add(new ]Label(
"<htmlxhlxi>Core ]ava</ix/hlxhr>By Cay Horstmann</htnn>"),
BorderLayout.CENTER);
[hi Dulog
- O X
1■刪-IJ
CoreJava
By Cay Horstmann
L遍」
图 12-38
About 对话框
]Panel panel = new JFanelO；
552
Java 核心技?
]Button ok = new ]Button("OK") ;
ok.addActionListener(event -> setVisible(false));
panel . add(ok) ;
add(panel , BorderLayout.SOUTH);
setSize(250, 150) ;
正如看到的， 构造器添加了用户界面组件， 在本例中添加的是标签和按钮， 并且为按钮
设置了处理器，然后还设置了对话框的大小。
要想显示对话框， 需要建立一个新的对话框对象， 并让它可见：
]Dialog dialog = new AboutDialog(this) ;
dialog.setVisible(true);
实际上， 在下面的示例代码中， 只建立了一次对话框， 无论何时用户点击 About 按钮，
都可以重复使用它。
if (dialog == null) // first time
dialog = new AboutDialog(this) ;
dialog.setVisible(true) ;
当用户点击 OK 按钮时， 该对话框将被关闭。 下面是在 OK 按钮的事件处理器中的处理
代码：
ok.addActionListener(event -> setVisible(false)) ;
当用户点击 Close 按钮关闭对话框时， 对话框就被隐藏起来。与 JFrame—样， 可以覆盖
setDefaultCloseOperation 方法来改变这个行为。
程序清单 12-17 是测试程序框架类的代码。 程序清单 12-18 显示了对话框类。
程序清单 12-17
dialog/DialogFrame.java
1 package dialog;
2
3
import javax. swing.]Frame;
4
import javax.swingJMenu;
5 import javax . swingJMenuBar;
6 import javax.swingJMenuItem;
7
8 /**
9
* A frame with a menu whose File->About action shows a dialog.
10
*/
11 public class DialogFrame extends ]Frame
12 {
13
private static final int DEFAULT_WIDTH = 300;
14
private static final int DEFAULT_HEICHT = 200;
is
private AboutDialog di alog;
16
17
public DialogFrameO
setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT) ;
// Construct a File menu.
第 J2 章
Swing 用 ? 界 面 ? 件
5 5 3
22
JMenuBar menuBar = new ]MenuBar()；
setDMenuBar(menuBar);
JMenu fileMenu = new ]Menu("File");
menuBar . add(fileMenu);
// Add About and Exit menu items.
// The About item shows the About dialog.
JMenuItem aboutltem = new ]MenuItem("About")；
aboutItem.addActionListener(event -> {
if (dialog = null) // first time
dialog = new AboutDialog(DialogFrame.this) ;
dialog.setVisible(true); // pop up dialog
})；
fileMenu.add(aboutltem) ;
// The Exit item exits the program.
JMenuItem exitltem = new ]MenuItem("ExitM)；
exitltem.addActionListener(event -> System.exit(O));
fileMenu.add(exitltem) ;
程序清单 12-18
dialog/AboutDialog.java
1 package dialog;
2
3 import java.awt.BorderLayout;
4
s
import javax. swing. ]Button;
6 import javax. swing.]Dialog;
7
import javax.swing.]Frame;
8
import javax.swing.]Label;
9 import javax.swing.]Panel;
10
11 /**
12
* A sample modal dialog that displays a message and waits for the user to click the OK button.
13
*/
14
public class AboutDialog extends ]Dialog
is {
16
public AboutDialogOFrame owner)
supe「(owne「， "About DialogTest" , true);
// add HTML label to center
add(
new ]Label (
"<htmlxhlxi>Core ]ava</ix/hlxh「>By Cay Ho「stmann</htnTI >")，.
Borde
「Layout.CENTER);
22
23
26
// OK button closes the dialog
554
Java 核心技?
]Button ok = new 3Button("OK")；
ok.addActionListener(event -> setVisible(false));
// add OK button to southern border
]Panel panel = new 3Panel();
panel,add(ok);
add(panel, BorderLayout.SOUTH);
pack();
javax.swing.JDialog 1.2
• public JDialog( Frame parent , String title, boolean modal )
构造一个对话框。在没有明确地让对话框显示之前， 它是不可见的。
参数：parent
对话框拥有者的框架
title
对话框的标题
modal
trae 代表模式对话框（模式对话框阻塞其他窗口的输入）
12.7.3
数据交换
使用对话框最通常的目的是获取用户的输入信息。 在前面已经看到， 构造对话框对象非
常简单： 首先初始化数据， 然后调用 setViSible(tme) 就会在屏幕上显示对话框。现在，看看
如何将数据传人传出对话框。
看一下如图 12-39 所示的对话框， 可以用来获得用户名
和用户密码以便连接某些在线服务。
对话框应该提供设置默认数据的方法。 例如， 示例程
序中的 PasswordChooser 类提供了一个 setUser 方法， 用来
将默认值放到下面的字段中：
public void setUser(User u)
{
username.setText(u _getName());
}
一旦设置了默认值（如果需要 )， 就可以调用 setViSible(true) 让对话框显示在屏幕上。
然后用户输人信息， 点击 0K 或者 Cancel 按钮。 这两个按钮的事件处理器都会调用
SetVisible(false) 终止对 setVisible(true) 的调用。 另外， 用户也可以选择关闭对话框。 如果没
有为对话框安装窗口监听器， 就会执行默认的窗口结束操作，即对话框变为不可见，这也中
止了对 setVisible(true) 的调用。
重要的问题是在用户解除这个对话框之前， 一直调用 setVisible(true) 阻塞。 这样易于实
现模式对话框。
希望知道用户是接收对话框， 还是取消对话框。在示例代码中设置了 OK 标志， 在对话
Ufl]DataExchangeTest
|M|DlX
File
User namelyoumame
[
Ok ||
Cancel |
图丨2-39
密码对话框
第 U 章
Swing 用?界面?件
555
框显示之前是 false。 只有 OK 按钮的事件处理器可以将它设置为 true。这样， 就可以获得对
话框中的用户输入。
0 注释： 无模式对话框数据传输就没有那么简单了。 当无模式对话框显示时， 调用
setVisible(true) 并不阻塞， 在对话框显示时， 其他程序仍继续运行。 如果用户选择了无模
式对话框中的一项， 并点击 OK , 对话框就会将一个事件发送给程序中的某个监听器。
示例程序中还包含另外一个很有用的改进。 在构造一个 JDialog 对象时，需要指定拥有
者框架。但是， 在很多情况下，一个对话框可能会有多个拥有者框架，所以最好在准备显示
对话框时再确定拥有者框架，而不是在构造 PasswordChooser 对象时。
有一个技巧是让 PasswordChooser 扩展 JPanel, 而不是扩展 JDialog, 在 showDialog 方法
中动态建立 JDialog 对象：
public boolean showDia1og(Frane owner, String title)
{
ok = false;
if (dialog = null || dialog.getOwnerO != owner)
{
dialog = new 3Dialog(owner, true);
dialog.add(this);
dialog.packO；
}
dialog.setTitie(title) ;
dialog.setVlsible(true);
return ok;
}
注意， 让 owner 等于 null 是安全的。
可以再做进一步的改进。有时， 拥有者框架并不总是可用的。利用任意的 parent 组件可
以很容易地得到它。 如下所示：
Frame owner;
if (parent instanceof Frame)
owner = (Frame) parent;
else
owner = (Frame) SwingUti1ities.getAncestorOfClass(Frame.class, parent) ;
在示例程序中使用了改进的代码。JOptionPane 类也使用了上面的机制。
很多对话框都有默认按钮。 如果用户按下一个触发器键（在大多数“ 观感” 实现中是
ENTER) 就自动地选择了它。默认按钮通常用加粗的轮廓给予特别标识。
可以在对话框的根窗格（root pane) 中设置默认按钮：
dialog.getRootPane()•setDefaultButton(okButton);
如果按照前面的建议， 在一个面板中布局对话框就必须特别小心。在包装面板进人对话
框后再设置默认按钮。面板本身没有根窗格。
程序清单 12-19 是程序的框架类， 这个程序展示了进出对话框的数据流。程序清单 12-20
556
Java 核心技?
?出了??框?。
程序清? 12-19
dataExchange/DataExchangeFrame.java
1 package dataExchange;
2
3 import java.awt.*;
4 import java.awt.event.*;
s import javax.swing.*;
6
7 /**
8
* A frame with a menu whose File->Connect action shows a password dialog.
9
*/
10 public cl ass DataExchangeFrame extends ]Frame
11 {
12
public static final int TEXT一ROWS = 20;
13
public static final int TEXT_C0LUMNS = 40;
14
private PasswordChooser dialog = null:
is
private ]TextA「ea textArea;
16
17
public DataExchangeFrameO
18
{
19
// construct a File menu
20
21
JMenuBar mbar =•new ]MenuBar();
22
set]MenuBar(mbar);
23
JMenu fileMenu = new 3Menu("File");
24
mbar.add(fileMenu);
25
26
// add Connect and Exit menu items
27
28
JMenuItem connectltem = new ]MenuItem("Connect");
29
connectltem.addActionListener(new ConnectActionO);
30
fileMenu.add(connectltem);
31
32
// The Exit item exits the program
33
34
JMenuItem exitltem = new JMenuItem("Exit");
35
exitltem.addActionListener(event -> System,exit(0));
36
fileMenu.add(exitltem);
37
38
textArea = new ]TextArea(TEXT_R0WS, TEXT_C0LUMNS);
39
add(new ]ScrollPane(textArea), BorderLayout.CENTER);
40
pack();
«
}
42
43
/**
44
* The Connect action pops up the password dialog.
45
V
46
private class ConnectAction implements ActionListener
47
{
48
public void actionPerformed(ActionEvent event)
49
{
so
// i f first time, construct dialog
si
52
if (dialog == null) dialog = new PasswordChooserO;
53
第 J2 聿
Swing 用 ? 界 面 ? 件
5 5 7
67
// set default values
dialog.setUser(new UserCyourname", null));
// pop up dialog
if (dialog.showDialog(DataExchangeFrame.this, "Connect"))
{
// i f accepted, retrieve user input
User u = dialog.getUserO;
textArea.appendfuser name = " + u.getName() + ", password =
+ (new String(u.getPassword())) + "\n");
程序清单 12-20
dataExchange/PasswordChooser.java
1 package dataExchange;
2
3 import java.awt.Bo「de「Layout;
4 import java.awt.Component;
s import java.awt.Frame;
6 import java.awt.GridLayout;
7
s import javax.swing.]Button;
9 import javax.swing.]Dialog;
10 import javax.swing.]Label;
11 import javax.swing.]Panel;
12 import :javax.swing.]PasswordField;
13 import javax.swingJTextField;
14 import javax.swing.SwingUti1ities:
is
16 /**
17
* A password chooser that is shown inside a dialog
18 v
19 public class PasswordChooser extends JPanel
2。 {
21
private JTextField username;
22
private ]PasswordField password;
23
private JButton okButton;
24
private boolean ok;
25
private ]Dialog dialog;
26
27
public PasswordChooser()
28
{
29
setLayout(new Bo「de「Layout());
30
si
// construct a panel with user name and password fields
32
33
3Panel panel = new 3Panel();
34
panel.setLayout(new CridLayout(2, 2));
3s
panel.add(new ]Label ("User name:"));
36
panel.add(username = new JTextField(""))；
37
panel.add(new JLabel("Password:"));
38
panel.add(password = new ]PasswordField(""));
39
add(panel, Bo「de「Layout.CENTER);
558
Java 核心技?
40
41
// create Ok and Cancel buttons that terminate the dialog
42
43
okButton = new 3Button("Ok")；
44
okButton.addActionListener(event -> {
45
ok = true;
46
dialog.setVisible(false);
47
})；
48
49
]Button cancelButton = new ]Button("Cancel");
so
cancelButton.addActionLi stener(event -> dialog.setVisible(false));
si
52
// add buttons to southern border
53
54
]Panel buttonPanel = new JPanel();
55
buttonPanel.add(okButton);
56
buttonPanel.add(cancelButton);
57
add(buttonPanel, Bo「de「Layout.SOUTH);
58
}
59
60
/**
61
* Sets the dialog defaults.
62
* @param u the default user information
63
*/
64
public void setUser(User u)
65
{
66
username.setText(u.getNameO);
67
}
68
69
/**
70
* Gets the dialog entries.
71
* ©return a User object whose state represents the dialog entries
72
*/
73
public User getUser()
74
{
7s
return new User(username.getTextO, password.getPasswordO):
76
}
77
78
/**
79
* Show the chooser panel in a dialog
so
*
@param parent a component in the owner frame or null
si
* @param title the dialog window title
82
*/
83
public boolean showDialog(Component parent, String title)
84
{
85
ok = false;
86
87
// locate the owner frame
88
89
Frame owner = null;
90
if (parent instanceof Frame)
91
owner = (Frame) parent;
92
else
93
owner = (Frame) SwingUti1ities.getAncestorOfClass(Frame.class, parent):
94
95
// if first time, or if owner has changed, make new dialog
96
第 12 章
Swing 用?界面?件
559
if (dialog = null 11 dialog.getOwnerO != owner)
{
dialog = new Dialog(owner, true);
dialog.add(this);
dialog.getRootPaneO.setDefaultButton(okButton);
dialog.packQ;
// set title and show dialog
dialog.setTitie(title);
dialog.setVisible(true);
return ok;
[i>w|javax.swing.SwingUti丨丨ties 1.2
•
Container getAncestorOfClass(C1ass c, Component comp)
返回给定组件的最先的父容器。 这个组件属于给定的类或者其子类之一。
[«w|javax.swing.JComponent 1.2
•
JRootPane getRootPane()
获得最靠近这个组件的根窗格， 如果这个组件的祖先没有根窗格返回 mill。
[wj javax.swing.JRootPane 1.2
•
void setDefaultButton(JButton button)
设置根窗格的默认按钮。 要想禁用默认按钮， 需要将参数设置为 null。
[wj] javax.swing.JButton 1.2
•
boolean isDefaultButton()
如果这个按钮是它的根窗格的默认按钮， 返回 true。
12.7.4
文件对话框
当编写应用程序时，通常希望可以打开和保存文件。一个好的文件对话框应该可以显示
文件和目录， 可以让用户浏览文件系统， 这是很难编写的。人们肯定不愿意从头做起。 很幸
运，Swing 中提供了 JFileChooser 类， 它可以显示一个文件对话框， 其外观与本地应用程序
中使用的文件对话框基本一样。JFileChooser 是一个模式对话框。 注意， JFileChooser 类并不
是 JDialog 类的子类。 需要调用 showOpenDialog， 而不是调用 SetVisible(true) 显示打开文件
的对话框， 或者调用 showSaveDialog 显示保存文件的对话框。接收文件的按钮被自动地标签
为 Open 或者 Save。也可以调用 showDialog 方法为按钮设定标签。 图 12*40 是文件选择对话
框的样例。
560
Java 核心技?
m
a tod» i
a
D
|AH
图 12-40
文件选择对话框
下面是建立文件对话框并且获取用户选择信息的步骤：
1 ) 建立一个 JFileChooser 对象。与 JDialog 类的构造器不同，它不需要指定父组件。允
许在多个框架中重用一个文件选择器。 例如：
JFileChooser chooser = new JFileChooserO；
提示： 重用一个文件选择器对象是一个很好的想法， 其原因是 JFileChooser 的构造器相
当耗费时间。特别是在 Windows 上， 用户映射了很多网络驱动器的情况下。
2 ) 调用 setCurrentDirectory 方法设置当前目录。例如， 使用当前的工作目录：
chooser.setCurrentDirectory(new File
需要提供一个 File 对象。File 对象将在卷 II 的第 2 章中详细地介绍。 这里只需要知道构
造器 File (String fileName ) 能够将一个文件或目录名转化为一个 File 对象即可。
3 ) 如果有一个想要作为用户选择的默认文件名， 可以使用 setSelectedFile 方法进行指定：
chooser.setSelectedFi1e(new File(filename)) ;
4 ) 如果允许用户在对话框中选择多个文件， 需要调用 setMultiSelectionEnabled 方法。
当然，这是可选的。
chooser.setMultiSelectionEnabled(true) ;
5 ) 如果想让对话框仅显示某一种类型的文件（如， 所有扩展名为 .gif 的文件)， 需要设
置文件过滤器， 稍后将会进行讨论。
6 ) 在默认情况下，用户在文件选择器中只能选择文件。 如果希望选择目录，需要调用
setFileSelectionMode 方法。 参数值为：JFileChooser.FILES_ONLY ( 默认值）， JFileChooser.
DIRECTORIES_ONLY 或者 JFileChooser.FILES_AND_DIRECTORIES0
7 ) 调用 showOpenDialog 或者 showSaveDialog 方法显7TC对话框。必须为这些调用提供父
组件：
第 J2 章
Swing 用 ? 界 面 ? 件
5 6 1
int result = chooser.showOpenDialog(parent):
或者
int result = chooser.showSaveDialog(parent);
这些调用的区别是“ 确认按钮” 的标签不同。点击“ 确认按钮” 将完成文件选择。也可
以调用 showDialog 方法， 并将一个显式的文本传递给确认按钮：
int result = chooser.showDialog(parent, "Select");
仅当用户确认、 取消或者离开对话框时才返回调用。 返回值可以是 JFileChooser.
APPROVE
—
OPTION、 JFileChooser.CANCEL_OPTION 或者 JFileChooser.ERROR—
OPTION。
8 ) 调 用 getSelectedFile() 或者 getSelectedFiles() 方法获取用户选择的一个或多个文件。
这些方法将返回一个文件对象或者一组文件对象。 如果需要知道文件对象名时， 可以调用
getPath 方法。 例如：
String filename = chooser.getSelectedFileO.getPath();
在大多数情况下， 这些过程比较简单。使用文件对话框的主要困难在于指定用户需要
选择的文件子集。 例如， 假定用户应该选择 GIF 图像文件。后面的文件选择器就应该只显
示扩展名为 .gif 的文件，并且， 还应该为用户提供反馈信息来说明显示的特定文件类别， 如
“
GIF 图像”。然而， 情况有可能会更加复杂。 如果用户应该选择 JPFG 图像文件， 扩展名就
可以是 .jpg 或者 jpeg。与重新编码实现这种复杂情况相比， 文件选择器的设计者提供了一
种更好的机制： 若想限制显示的文件， 需要创建一个实现了抽象类 jaVax.swing.filechooser.
FileFilter 的对象。文件选择器将每个文件传递给文件过滤器， 只有文件过滤器接受的文件才
被最终显示出来。
在编写本书的时候，有两个子类可用：可以接受所有文件的默认过滤器和可以接受给定
扩展名的所有文件的过滤器。其实，设计专用文件过滤器非常简单， 只要实现 FileFilter 超类
中的两个方法即可：
public boolean accept(File f);
public String getDescriptionO;
第一个方法检测是否应该接受一个文件， 第二个方法返回显示在文件选择器对话框中显
示的文件类型的描述信息。
0 注 释： 在 java.io 包中有一个无关的 FileFilter 接口， 其中只包含一个方法：boolean
accept(File f)。File 类中的 listFiles 方法利用它显示目录中的文件。 我们不知道 Swing 的
设计者为什么不扩展这个接口， 可能是因为 Java 类库过于复杂， 致使 Sun 程序员也不太
熟悉所有的标准类和接口了。
需要解决同时导入 javax.io 包和 javax.swing.filechooser 包带来的名称冲突问题。 最简
单的方法是导入 javax.swing.filechooser.FileFilter, 而不要导入 javax.swing.filechooser.*0
一旦有了文件过滤器对象， 就可以调用 JFileChooser 类中的 setFileFilter 方法，将这个对
562
Java 核心技?
象安装到文件选择器对象中：
chooser.setFileFilter(new FileNameExtensionFi1ter("Image files", "gif", "jpg"));
.可以为一个文件选择器安装多个过滤器：
chooser.addChoosabl eFi1eFi1ter(filterl):
chooser.addChoosabl eFi1eFi1ter(filter2);
用户可以从文件对话框底部的组合框中选择过滤器。在默认情况下， All files 过滤器总是
显示在组合框中。这是一个很好的主意， 特别是在使用这个程序的用户需要选择一个具有非
标准扩展名的文件时。然而， 如果你想禁用 All files 过滤器，需要调用：
chooser.setAcceptAUFileFilterUsed(false)
警告： 如果为加载和保存不同类型的文件重用一个文件选择器， 就需要调用：
chooser.resetChoosableFi1ters0
这样可以在添加新文件过滤器之前清除旧文件过滤器。
最后，可以通过为文件选择器显示的每个文件提供特定的图标和文件描述来定制文件选
择器。这需要应用一个扩展于 javax.swing.filechooser 包中的 FileView 类的对象。这是一个高
级技巧。在通常情况下，不需要提供文件视图—可插观感会提供。然而， 如果想让某种特
定的文件类型显示不同的图标， 就需要安装自己的文件视图。这要扩展 FileView 并实现下面
5 个方法：
Icon getIcon(File f);
String getName(File f);
String getDescription(File f);
String getTypeDescription(Fi1e f);
Boolean isTraversable(File f);
然后，调用 setFileView 方法将文件视图安装到文件选择器中。
文件选择器为每个希望显示的文件或目录调用这些方法。如果方法返回的图标、名字或
描述信息为 null, 那么文件选择器将会构造当前观感的默认文件视图。这样处理很好，其原
因是这样只需处理具有不同显示的文件类型。
文件选择器调用 isTraversable 方法来决定是否在用户点击一个目录的时候打开这个目录。
请注意， 这个方法返回一个 Boolean 对象， 而不是 boolean 值。看起来似乎有点怪， 但实际
上很方便—如果需要使用默认的视图， 则返回 null。文件选择器将会使用默认的文件视
图。换句话说，这个方法返回的 Boolean 对象能给出下面三种选择： 真（Boolean.TRUE), 假
(Boolean.FALSE) 和不关心（null)。
在示例中包含了一个简单的文件视图类。当文件匹配文件过滤器时， 这个类将会显示一
个特定的图标。可以利用这个类为所有的图像文件显示一个调色板图标。
class FilelconView extends FileView
{
private FileFilter filter;
private Icon icon;
第 1 2 章
Swing 用 ? 界 面 ? 件
5 6 3
public FileIconView(FileFilter aFilter, Icon anlcon)
{
filter = aFilter;
icon = anlcon;
}
public Icon getIcon(File f)
{
if (If.isDirectoryO && fiIter.accept(f))
return icon;
else return null ;
}
}
可以调用 setFileView 方法将这个文件视图安装到文件选择器中：
chooser.setFileView(new Fil elconVi ew(filter,
new IaagelconC'palette.gif")));
文件选择器会在通过 filter 的所有文件旁边显示调色板图标，并且使用默认的文件视图来
显示所有其他的文件。很自然， 我们可以使用与文件选择器设定的一样的过滤器。
提示： 可以在 JDK 的 demo/jfc/FileChooserDemo 目录下找到更实用的 ExampleFileView
类。它可以将图标和描述信息与任意扩展名关联起来。
最后， 可以通过添加一个附件组件来定制文件对话框。例如， 图 12*41 在文件列表旁边
显示了一个预览附件。这个附件显示了当前选择文件的缩略视图。
Look in: C3ImageVicwer
hi Nleibitilcii
gCay.glf
%Towcr.gilj
Hit dune
Imwtrilif
[
Flits of Xypc: [imagt flits
| ]
|
Optn ||
Cancel |
图 12>41
带预览附件的文件对话框
附件可以是任何 Swing组件。在这个示例中， 扩展 JLabel 类， 并将图标设置为所选的图
像文件的压缩拷贝。
class ImagePreviewer extends JLabel
{
public ImagePreviewerQFi1eChooser chooser)
{
setPreferredSize(new Dimension(100, 100));
564
Java 核心技?
setBorder(BorderFactory.createEtchedBorder());
public void loadImage(File f)
{
Imagelcon icon = new Imagelcon(f.getPath());
if(icon.getIconWidth() > getWidthO)
icon = new Imagelcon(icon.getlmage().getScaledlnstance(
getWidthO, -1, Image.SCALEJEFAULT));
setIcon(icon);
repaint。
;
}
}
这里还有一个挑战， 即需要在用户选择不同的文件时更新预览图像。 文件选择器使用了
JavaBeans 机制。 当它的属性发生变化时， 文件选择器就会通知相关的监听器。 被选择文件
是一个属性， 可以通过安装 PropertyChangeListener 监听它。本书将在卷 II 第 8 章中讨论这个
机制。下面这段代码可以用来捕捉通知：
chooser.addPropertyChangeListener(event -> {
if (event.getPropertyNameO == 3Fi1eChooser.SELECTED_FILE_CHANCED_PROPERTY)
{
File newFile = (File) event.getNewValueO;
// update the accessory
} …
})；
在这个示例中，将这段代码添加到 ImagePreviewer 构造器中。
程序清单 12-21 〜 程序清单 12-23 对第 2 章中的 ImageViewer 程序做了一定的修改。通
过自定义的文件视图和预览附件文件增强了文件选择器的功能。
程序清单 12-21
fileChooser/lmageViewerFrame.java
1 package fileChooser;
2
3 import java.io.*;
4
5 import javax.swing.*;
6 import javax.swing.fi1echooser.*;
7 import javax.swing.fi1echooser.Fi1eFi1ter;
8
9 /**
10
* A frame that has a menu for loading an image and a display area for the
11
* loaded image.
12
*/
13 public class ImageViewe「Frame extends 3Frame
i< {
is
private static final int DEFAULTJUDTH:300;
16
private static final int DEFAULTJEICHT = 400;
17
private ]Label label;
18
private 3FileChooser chooser;
public ImageViewe「Frame()
第 章
Swing 用 ? 界 面 ? 件
5 6 5
22
setSize(DEFAULT_WIDTH, DEFAULTJEIGHT);
23
24
I/ set up menu bar
2s
JNenuBar menuBar = new DMenuBarO:
26
set]MenuBar(menuBar);
27
28
JMenu menu = new JMenuC'File");
29
menuBar.add(menu);
30
31
JMenuItem openltem = new JMenuItemCOpen")；
32
menu,add(openltem);
33
openltem.addActionListener(event -> {
34
chooser.setCurrentDirectory(new File("."))；
35
36
// show file chooser dialog
37
int result = chooser.showOpenDialog(ImageViewerFrame.this);
38
39
// if image file accepted, set it as icon of the label
40
if (result == ]FileChooser.APPR0VE_0PTI0N)
«
{
42
String name = chooser.getSelectedPile().getPath();
43
label.setIcon(new Imagelcon(name));
44
pack();
«
}
«
})；
47
48
JMenuItem exitltem = new ]MenuItem("Exit")；
49
menu.add(exitltem);
so
exitltem.addActionListener(event -> System,exit(0));
51
52
// use a label to display the images
53
label = new ]Label0;
54
add(label);
55
56
// set up file chooser
57
chooser = new ]FileChooser();
58
59
// accept all image files ending with .jpg, .jpeg, .gif
60
FileFilter filter = new Fi1eNameExtensionFi1ter(
61
"Image files", "jpg", "jpeg", "gif)；
62
chooser,setFileFilter(fiIter);
63
64
chooser.setAccessory(new ImagePreviewer(chooser));
65
66
chooser.setFileView(new FileIconView(filter, new Imagelcon("palette.gif"))):
67
}
68 }
程序清单 12-22
fileChooser/lmagePreviewer.java
1 package fileChooser;
2
3 import java.awt.*;
« import java.io.*;
6 import javax.swing.*;
566
Java 核心技?
10
14
15
18
19
22
23
24
25
29
30
31
36
7
8
9
0
1
2
3
4
5
6
/**
* A file chooser accessory that previews images.
V
public class ImagePreviewer extends ]Label
{
/**
* Constructs an ImagePreviewer.
* Qparam chooser the file chooser whose property changes trigger an image
*
change in this previewer
V
public ImagePreviewer(]Fi1eChooser chooser)
{
setPreferredSize(new Dimension(100, 100));
setBo「de「(BorderFactory.createEtchedBorder());
chooser.addPropertyChangeListener(event -> {
if (event.getPropertyNameO == ]Fi1eChooser.SELECTED_FILE_CHANGED_PROPERTY)
{
// the user has selected a new file
File f = (File) event.getNewValueO;
if (f == null)
{
setlcon(null);
return;
// read the image into an icon
Imagelcon icon = new Imagelcon(f.getPathO)；
// if the icon is too large to fit, scale it
if (icon.getlconWidthO > getWidthO)
icon = new Imagelcon(icon.getlmage().getScaledlnstance(
getWidthO, -1, Image.SCALEJEFAULT));
setIcon(icon);
}
})；
程序清? 12-23
fileChooser/FilelconView.java
1 package fileChooser;
2
3 import java.io.*;
4 import javax.swing.*;
s import javax.swing.filechooser.*;
6 import javax.swi ng.fi1echooser.Fi1eFiIter;
7
8 /**
9
* A file view that displays an icon for all files that match a file filter.
10
*/
•
11 public class FilelconView extends FileView
12 {
private FileFilter filter;
第 /2 章
Swing 用?界面?件
567
i4
private Icon icon;
is
16
/**
17
* Constructs a FilelconView.
18
* @param aFilter a file filter all files that this filter accepts will be shown
19
* with the icon.
20
* @param anlcon
the icon shown with all accepted files.
21
V
22
public Fi1elconView(FileFi1ter aFilter, Icon anlcon)
23
{
24
filter = aFilter;
2s
icon = anlcon;
26
}
27
28
public Icon getIcon(File f)
29
{
30
if (Sf.isDirectory() && filter.accept(f)) return icon;
31
else return null;
| API] javax.swing.JFileChooser 1.2
• JFi 1eChooser( )
创建一个可用于多框架的文件选择器对话框。
• void setCurrentDirectory(Fi 1e dir )
设置文件对话框的初始目录。
• void setSelectedFi 1e(Fi 1e file )
• void setSelectedFiles(Fi 1e[ ] file )
设置文件对话框的默认文件选择。
參 void setMultiSelectionEnabled(boolean b )
设置或清除多选模式。
• void setFi 1eSelectionMode( int mode )
设置用户选择模式， 只可以选择文件（默认)， 只可以选择目录， 或者文件和目录均可以
选择。mode 参数的取值可以是 JFileChooser.FILES_ONLY、JFileChooser. DIRECTORIES_
ONLY 和 JFileChooser.FILES_AND_DIRECTORIES 之一。
• int
showOpenDialog( Component parent )
• int
showSaveDialog( Component parent )
• int
showDialog( Component parent , String approveButtonText )
显 示 按 钮 标 签 为 Open， Save 或 者 approveButtonText 字 符 串 的 对 话 框， 并 返 回
APPROVE_ OPTION、 CANCEL_OPTION ( 如果用户选择取消按钮或者离开了对话框)
或者 ERROR_OPTION ( 如果发生错误)。
• File getSelectedFi 1e( )
• Fi 1e[ ] getSelectedFi 1es( )
568
Java 核心技?
获取用户选择的一个文件或多个文件（如果用户没有选择文件，返回 null)。
• void setmeF1lter(Fi leFilter filter )
设置文件对话框的文件过滤器。 所有让 filteraccqrt 返回 true 的文件都会被显示，并且
将过滤器添加到可选过滤器列表中。
• void addChoosableFi 1eFi 1ter(Fi 1eFilter filter )
将文件过滤器添加到可选过滤器列表中。
• void setAcceptAl 1Fi 1eFi 1terUsed(boo1ean b)
在过滤器组合框中包括或者取消 All files 过滤器。
• void resetChoosableFi 1eFi 1ters( )
清除可选过滤器列表。除非 All files 过滤器被显式地清除，否则它仍然会存在。
• void setFileView(FileView view )
设置一个文件视图来提供文件选择器显示信息。
• void setAccessory( JComponent component )
设置一个附件组件。
[A«|javax.swing.filechooser.FileFilter 1.2
• boolean accept(File f )
如果文件选择器可以显示这个文件，返回 true。
• String getDescription( )
返回这个文件过滤器的说明信息， 例如， Image files (*.gif，*jpeg)。
[AW|javax.swing.filechooser.FileNameExtensionFiler 6
• Fi 1 eNameExtensionFi 1ter( String description, String . . . extensions )
利用给定的描述构造一个文件过滤器。这些描述限定了被接受的所有目录和文件其名
称结尾的句点之后所包含的扩展字符串。
j |javax.swing.filechooser.FileView 1.2
• String getName(File f )
返回文件 f 的文件名， 或者 null。通常这个方法返回 f.getName()。
• String getDescription(File f )
返回文件 f 的可读性描述， 或者 null。例如， 如果 f 是 HTML 文档， 那么这个方法有
可能返回它的标题。
• String getTypeDescription(Fi 1e f )
返回文件 f 的类型的可读性描述。 例如， 如果 f 是 HTML 文档， 那么这个方法有可能
返回 Hypertext document 字符串。
• Icon getIcon(File f )
返回文件 f 的图标， 或者 null。 例如， 如果 f 是 JPEG 文件， 那么这个方法有可能返回
第 n 章
Swing 用 ? 界 面 ? 件
5 6 9
简略的图标。
•B o o l e a n i s T r a v e r s a b l e( F i 1e f )
如果 f 是用户可以打开的目录， 返回 Boolean.TRUE
如果目录在概念上是复合文档，
那么这个方法有可能返冋 false。与所有的 Fi丨eView 方法一样， 这个方法有可能返回
null, 用于表示文件选择器应该使用默认视图。
12.7.5
颜色选择器
前面曾经说过， 一个高质量的文件选择器是
一个很复杂的用户界面组件， 人们肯定不愿意自
己去编写它。许多用户界面工具包还提供了另外
一些常用的对话框： 选择日期 / 时间、 货币值、
字体以及颜色等。 这将会带来两个方面的好处：
程序员可以直接地使用这些高质量的代码而不用
从头做起， 并且用户可以得到一致的组件使用
体验。
除了文件选择器外，Swing还提供了一种选择
器—JColorChooser (如图 12M2 ~ 图 12 44 )。可
以利用这个选择器选取颜色。 与 JFileChooser -
样， 颜色选择器也是一个组件， 而不是一个对话
框， 但是它包含了用于创建包含颜色选择器组件的对话框方法。
a D ■
■■
图 12-42
颜色选择器的 swatches 窗格
& 麵 G 204
□
图 12-43
颜色选择器的 HSB 窗格
口
□
US
OK
图 12-44
颜色选择器的 RGB 窗格
下面这段代码说明了如何利用颜色选择器显示模式对话框:
Color selectedColor = JColorChooser.showDial og(parent,titl e, initialColor);
570
Java 核心技?
另外， 也可以显示无模式颜色选择器对话框， 需要提供：
•一个父组件。
•对话框的标题。
•选择模式 / 无模式对话框的标志。
•颜色选择器。
•0K 和 Cancel 按钮的监听器 （如果不需要监听器可以设置为 null )。
下面这段代码将会创建一个无模式对话框。当用户按下 OK 键时， 对话框的背景颜色就
会被设成所选择的颜色。
chooser = new ]ColorChooser() ;
dialog = ]ColorChooser.createDialog(
parent,
"Background Color",
false /* not modal */ »
chooser,
event -> setBackground(chooser.getColor()) ,
null /* no Cancel button listener */)；
读者还可以做进一步的改进， 将颜色选择立即反馈给用户。 如果想要监视颜色的选择，
那就需要获得选择器的选择模型并添加改变监听器：
chooser.getSelectionModel () .addChangeListener(event -> {
do something with chooser.getColor();
})；
在这种情况下， 颜色选择器对话框提供的 OK 和 Cancer没有什么用途。可以将颜色选择
器组件直接添加到一个无模式对话框中：
dialog = new 3Dialog(parent, false /* not modal */)；
dialog.add(chooser);
dialog.pack() ;
程序清单 12-24 的程序展示了三种对话框类型。如果点击 Model ( 模式）按钮， 则需要选
择一个颜色才能够继续后面的操作。 如果点击 Modaless ( 无模式）按钮， 则会得到一个无模
式对话框。 这时只有点击对话框的 OK 按钮时， 颜色才会发生改变。 如果点击 Immediate 按
钮， 那将会得到一个没有按钮的无模式对话框。 只要选择了对话框中的一个颜色， 面板的背
景就会立即更新。
程序清单 12-24
colorChooser/ColorChooserPanel.java
1 package colo「Chooser;
2
3
import java.awt.Color;
4 import java.awt.Frame;
s
import java. awt.event.ActionEvent;
6 import java.awt.event.ActionListener;
7
8
import javax.swing.]Button;
9 import javax. swing.]Colo「Chooser；
10 import javax.swingJDialog;
import javax. swing.]Panel ;
第 1 2 章
Swing 用 ? 界 面 ? 件
571
13 /**
14
* A panel with buttons to pop up three types of color choosers
is
*/
i6 public class Colo「Choose「Panel extends ]Panel
18
public ColorChooserPanelO
19
{
20
JButton modalButton = new ]Button("Modal")；
21
modalButton.addActionListener(new ModalListenerO);
22
add(modalButton);
23
24
]Button modelessButton = new ]Button("Modeless")；
25
modelessButton.addActionListener(new ModelessListenerO);
26
add(modelessButton);
27
28
JButton inmediateButton = new ]Button("Immediate");
29
immediateButton.addActionListene「(new ImmediatelistenerO);
30
add(immediateButton);
43
44
45
54
55
56
57
60
61
This listener pops up a modal color chooser
/**
* T
V
private class ModalListener implements ActionListener
{
public void actionPerformed(ActionEvent event)
{
Color defaultedor = getBackground();
Color selected = ]ColorChooser.showDialog(ColorChooserPanel.this, "Set background",
defaultedor);
if (selected != null) setBackground(selected);
47
/**
48
* This listener pops up a modeless color chooser. The panel color is changed when the user
49
* clicks the OK button,
so
*/
si
private class ModelessLi stene「 implements ActionListene「
52
{
53
private ]Dialog dialog;
private ]ColorChooser chooser;
public ModelessListenerO
{
chooser = new JColorChooserO;
dialog - JColorChooser.createDialog(ColorChooserPanel.this, "Background Color",
false /* not modal */. chooser,
event -> setBackground(chooser.getColor()),
null /* no Cancel button listener */)；
public void actionPerformed(ActionEvent event)
{
chooser.setColor(getBackgroundO);
dialog.setVisible(true);
572
Java 核心技?
70
71
72
73
U
77
78
79
81
82
86
87
88
89
91
97
/**
* This listener pops up a modeless color chooser. The panel color is changed immediately when
* the user picks a new color.
V
private class ImmediateListener implements ActionListener
{
private ]Dialog dialog;
private ]ColorChooser chooser;
public ImediateListenerO
{
chooser = new ]ColorChooser();
chooser.getSelectionModel().addChangeListener(
event -> setBackground(chooser.getColor()));
dialog = new ]Dialog((Frame) null, false /* not modal */);
dialog.add(chooser);
dialog.packQ;
public void actionPerformed(ActionEvent event)
{
chooser.setColor(getBackground());
dialog.setVisible(true);
| AW| javax.swing.JColorChooser 1.2
• JColorChooser( )
构造一个初始颜色为白色的颜色选择器。
• Color getColor( )
• void setColor( Color c )
获取和设置颜色选择器的当前颜色。
• s t a t i c
Col or
showDialog( Component
parent ,
S t r i n g
t i t l e ,
Col or
i n i t i a l Col or )
显示包含颜色选择器的模式对话框。
参数： parent
对话框显7K在上面的组件
title
对话框框架的标题
initialColor
颜色选择器的初始颜色
• s t a t i c
JD i a 1 og
createDialogC Component
p a r e n t ,
S t r i n g
t i t l e ,
boolean
modal ,
JColorChooser
chooser ,
ActionListener
okListener ,
ActionListener cancel Listener )
创建一个包含颜色选择器的对话框。
第 1 2 章
Swing 用 ? 界 面 ? 件
5 7 3
参数： parent
title
modal
chooser
okListener, cancelListener
对话框显示在上面的组件
对话框框架的标题
如果直到对话框关闭， 这个调用都被阻塞， 则返回 tme
添加到对话框中的颜色选择器
OK 和 Cancel 按钮的监听器
12.8
GU丨程序排错
本节会给出 GUI 编程的一些调试技巧， 然后介绍如何使用 AWT 机器人 （ AWT robot ) 自
动完成 GUI 测试。
12.8.1
调试技巧
如果你看过 Swing 窗口， 肯定想知道它的设计者如何把组件摆放得如此恰到好处， 可以
查看它的内容。 按下 Ctrl+Shift+Fl 得到所有组件的层次结构输出：
FontDialog[frameO,0,0, 300x200,1ayout=java.awt.Borde「L a y o u t
■
javax.swing.]RootPane[,4,23,292x173,1ayout=javax.swing. jRootPane$RootLayout
javax.swing.]Panel [null.glassPane,0,0,292x173,hidden，1ayout=java.awt.FlowLayout,
javax.swingJLayeredPane[null .1ayeredPane,0,0,292x173....
javax.swing.]Panel [null .contentPane,0,0,292x173,1ayout=java.awt.GridBagLayout....
javax.swing.JList[,0,0,73x152,alignmentX=null ,alignmentY=nul1,
javax.swing.CellRendere「Pane[,0,0,0x0，hidden]
javax.swing.DefaultListCel1RendereriUIResource[，-73，-19，0x0,
javax.swing.JCheckBox[,157,13, 50x25,1ayout=javax.swing.OverlayLayout....
javax.swing.JCheckBox[,156,65, 52x25,1ayout=javax.swing.OverlayLayout....
javax.swing.]Label [,114,119,30x17,alignmentX=0.0，alignmentY=nul1....
javax.swing.]TextFi eld[,186，117，105x21,alignmentX=nul1,alignmentY=nul1，...
javax.swingJTextField[,0,152,291x21,ali gnmentX=nul1,ali gnmentY=nul1,…
如果设计你自己的定制 Swing 组件， 但组件没能正确显示， 你肯定很高兴有一个 Svv/«g
图形化调试器（
grap/n'cs 也/wgger)。 即使你没有写自己的组件类， 能直观地看到如何
绘制组件内容也是很有趣的。 可以使用 JComponent 类的 setDebugGraphicsOptions 方法打开
对一个 Swing 组件的调试。有以下几个选项。
DebugGraphics.FLASH_OPTION
绘制前用红色闪烁地显示各条线、 矩形和文本
DebugGraphics.LOGOPTION
为每个绘制操作打印一个消息
DebugGraphics.BUFFERED_OPTION
显示在离屏缓冲区完成的操作
DebugGraphics.NONEOPTION
关闭图形调试
我们发现， 要让闪烁选项起作用， 必须禁用“ 双缓冲”—这是 Swing 更新窗口时为减
少闪烁所用的策略。 打开闪烁选项的魔咒是：
RepaintManager.currentManager(getRootPane()).setDoubleBufferingEnabled(false);
((]Component) getContentPaneQ)•setDebugCraphicsOptions(DebugCraphics.FLASH.OPTION);
574
Java 核心技?
只需要把这些代码行放在 feme 窗口构造器的末尾。程序运行时， 你将看到会用慢动作填充
内容窗格。或者， 对于更本地化的调试， 只需要为组件调用 setDebugGraphicsOpticms。“ 控制狂
人” 可能还会设置闪烁的时间段、 次数和颜色—详细信息参见 DebugGmphics 类的在线文档。
如果希望得到 GUI 应用中生成的每一个 AWT 事件的记录， 可以在发出事件的每一个组
件中安装一个监听器。 利用反射， 可以很容易地自动完成这个工作。 程序清单 12-25 给出了
EventTracer?。
程序清单 12-25
eventTracer/EventTracer.java
1 package eventTracer;
2
3 import java.awt.*;
4
import java.beans.*;
s
import java.lang.reflect.*;
7 /**
8
* ©version 1.31 2004-05-10
9
* ©author Cay Horstmann
10
*/
11 public class EventTracer
12 {
13
private InvocationHandler handler;
14
is
public EventTracer()
// the handler for all event proxies
handler = new InvocationHandlerQ
19
20
J3
34
37
43
public Object invoke(Object proxy , Method method, Object[] args)
System.out.println(method
+ args[0]);
return null;
}；
r
Adds event tracers for all events to which this component and its children can listen
* @paran c a component
*/
public void add(Component c)
{
try
{
// get all events to which this component can listen
Beanlnfo info = Introspector.getBeanlnfo(c.getClassO)；
EventSetDescriptor[] eventSets = info.getEventSetDescriptorsO；
for (EventSetDescriptor eventSet : eventSets)
addListener(c, eventSet);
catch (Introspect onException e)
第 1 2 章
Swing 用 ? 界 面 ? 件
5 7 5
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
// ok not to add listeners if exception is thrown
if (c instanceof Container)
{
// get all children and call add recursively
for (Component comp : ((Container) c)
_ getComponents())
add(comp);
* Add a listener to the given event set
* @param c a component
* Qparam eventSet a descriptor of a listener interface
V _
public void addListener(Component c, EventSetDescriptor eventSet)
{
// make proxy object for this listener type and route all calls to the handler
Object proxy = Proxy.newProxyInstance(null , new Class[] { eventSet.getListenerTypeO },
handler);
// add the proxy as a listener to the component
Method addListenerMethod = eventSet.getAddListenerMethod();
try
{
addListenerMethod.invoke(c , proxy);
}
catch (ReflectiveOperati onExcepti on e)
// ok not to add listener if exception is thrown
要查看消息， 可以把希望跟踪事件的组件增加到一个事件跟踪器:
EventTracer tracer = new EventTracerO;
tracer.add(frame);
这样一来， 就可以得到所有事件的一个文本描述， 如图 12-45 所示。
，
物鄭獅丨她_
MouseEvent):
. MouseEvent[MOUSE.MOVED ,(305.28),absolute(380.232).b£
utton=0,clickCount=O] on null.contentPane
public abstract void java.awt .event.MouseMotionListener.mouseMoved(java.awt.evenj
t.MouseEvent):java.awt.event . MouseEvent[MOUSE,MOVED ,(341,46).absolute(416.250),b
utton=0,clickCount=0】 on null.contentPane
public abstract void java.awt .event.MouseMotionListener.mouseMoved(java.awt.even
t . MouseEvent):java.awt .event.MouseEvent[MOUSE_MOVED ,(389,68).absolute(464,272),b
utton*0,clickCount=G] on null.contentPane
public abstract void java.awt event . MouseListener.mouseExited(java awt.event Mou
aeEvent):java.awt.event.MouseEvent MOUSE_EXITED,(441,90),absolute(516 ,294).butto
n=Q,c1ickCount=0】 on null.contentPane
public abstract void Java.awt.event.FocusListener.focusLost(java.awt.event.Focus
Event):java.awt.FocusEvent[F0CUS_L0ST,temporary ,opposite=null,cause=ACTIVATION]
.swing.JSlider[ ,0,0,398x16 ,alignmentX=G.0,alignmentY=0.G,border=,flags=2
imumSize= ,minimumSize=,preferredSize=,ialnverted=false ,majorTickSpacing=0,
minorTickSpacing=0,orientation=HORIZONTAL,paintLabe\s=falsa,paintTicks=false ,pai
ntlrack=true ,snapToTicka=false.anapToValue=true)
public abstract void java .awt.event.WindowFocusListener .windowLoatFocus(java awt
.event.WindowEvent):java.awt.event.WindowEvent[WIND0W_
L0ST_F0CUS ,oppoaite=null,o
ldState=0,newState=0] on frameG
ipublic abstract void java.awt.event.WindowListener.windowOeactivated(java.
ent.WindowEvent):java.awt .event.WindowEvent[WINDOW DEACTIVATED #opposite=null.old
State=0,newState=0] on frameG
tr
.on javax
96,maxim
a
图 12-45
EventTracer 类的实际工作
576
Java 核心技?
12.8.2
让 AWT 机器人完成工作
Robot 类可以向任何 AWT 程序发送按键和鼠标点击事件。这个类就是用来自动测试用户
界面的。
要得到一个机器人， 首先需要得到一个 GraphicsDevice 对象。可以通过以下调用序列得
到默认的屏幕设备：
CraphicsEnvironment environment = CraphicsEnvironment.getlocalCraphicsEnvironmentO;
CraphicsDevice screen = envi roninent.getDefaultScreenDevice():
然后构造一个机器人：
Robot robot = new Robot(screen);
若要发送一个按键事件， 需告知机器人模拟按下和松开按键:
robot.keyPress(KeyEvent.VK_TAB):
robot.keyRelease(KeyEvent.VK.TAB);
对于鼠标点击事件， 首先需要移动鼠标， 然后按下再释放鼠标按钮:
robot.mouseMove(x• y); // x and y are absolute screen pixel coordinates,
robot.nousePress(I叩utEvent.BUTTONl.MASK);
robot.mouseRelease(InputEvent.BUTTON1_MASK);
我们的思路是首先模拟按键和鼠标输入， 然后截屏来查看应用是否完成了它该完成的工
作。截屏需要使用 createScreenCapture 方法：
Rectangle rect = new Rectangle(x, y, width, height);
Bufferedlmage image = robot.createScreenCapture(rect):
矩阵坐标也指示绝对屏幕像素。
最后， 通常我们都希望在机器人指令之间增加一个很小的延迟， 使应用能跟得上。可以
使用 delay 方法并提供延迟时间（毫秒数)。例如：
robot.delay(1000); // delay by 1000 milliseconds
程序清单 12-26 显示了如何使用机器人。这个机器人会检查第 11 章中我们见过的按钮测
试程序。 首先， 按下空格键激活最左边的按钮。
然后机器人会等待两秒， 以便你能看到它做了什
么。这个延迟之后，机器人会模拟按下 Tab 键和
再按一次空格键， 来点击下一个按钮。 最后， 它
会模拟用鼠标点击第 3 个按钮。（要按下按钮， 可
能需要调整程序的 x 和 y 坐标。）程序的最后会截
屏， 并在另一个 frame 窗口中显示（见图 12*46 )。
0 注释： 需要在一个单独的线程中运行机器人，
如示例代码中所示。 关于线程的更多信息参
见第 14 章。
m
<))
> Uilvlch09-GroupLayoutTest
>
vlch09-MenuTest
> W vlch09-OptionDialogTest
public
图 12*46
用 AWT 机器人截屏
第 A? 章
Swing 用?界面?件
577
从这个例子可以看到， 单凭 Robot 类本身并不能方便地测试用户界面。 实际上， 这只是
一个基本构建模块，可以作为测试工具的一个基础部分。专业的测试工具可以捕获、 存储和
重放用户交互场景， 并得到组件在屏幕上的位置， 这样就不用猜测鼠标点击的位置了。
程序清单 12-26
robot/RobotTest.java
1 package robot;
2
3 import java.awt.*;
4 import java.awt.event.*;
5 import java.awt.image.*;
6 import javax.swing.*;
48
/**
* ©version 1.05 2015-08-20
* ©author Cay Horstmann
V
public class RobotTest
{
public static void main(String ] args)
{
EventQueue.invokeLater(() ->
{
// make frame with a button panel
ButtonFrame frame = new ButtonFrame();
frame.setTitlefButtonTest");
frame.setDefaultCloseOperation(]Frame.EXIT_0N_CL0SE);
frame •setVisible(true);
})；
// attach a robot to the screen device
CraphicsEnvironment environment = GraphicsEnvironinent.getLocalCraphicsEnvironment();
CraphicsDevice screen = environment.getDefaultScreenDevice():
try
{
final Robot robot = new Robot(screen);
robot.waitForldleO；
new Thread0
{
public void run()
{
runTest(robot);
}；
}.start0;
}
catch (AlufTException e)
{
e.printStackTrace();
49
/**
so
* Runs a sample test procedure
578
Java 核心技?
* @param robot the robot attached to the screen device
*/
public static void runTest(Robot robot)
{
// simulate a space bar press
robot.keyPressC ');
robot.keyRelease(' ');
// simulate a tab key followed by a space
robot.delay(2000) ;
robot. keyPress(KeyEvent.VK_TAB);
robot.keyRelease(KeyEvent.VK_TAB) ;
robot.keyPressC ') ;
robot . keyRelease(' ');
// simulate a mouse click over the rightmost button
robot.del ay(2000) ;
robot.mouseMove(220， 40) ;
robot.mousePress(InputEvent.BUTT0N1_MASK);
robot.mouseRelease(InputEvent.BUTTON1_MASK);
// capture the screen and show the resulting image
robot.delay(2000) ;
Bufferedlmage image = robot.createScreenCapture(new Rectangle(0, 0, 400, 300));
ImageFrame frame = new ImageFrame(image) ;
frame.setVisible(true);
81 /**
82
* A frame to display a captured image
83
*/
84
class ImageFrame extends ]Frame
85 {
86
private static final int DEFAULT_WIDTH = 450;
87
private static final int DEFAULTJEICHT = 350;
88
89
/**
90
*
@param image the image to display
91
V
92
public ImageFrame(Image image)
93
{
94
setTitle("Capture")；
95
setSize(DEFAULT_WIDTH, DEFAULTJEICHT);
96
97
]Label label = new ]Label (new Imagelcon(image));
98
add(label);
99
}
100 }
APIJ java.awt.GraphjcsEnvironment 1.2
• static GraphicsEnvironment getLocalGraphicsEnvironment()
返冋本地图形环境。
襄 A2 章
Swing 用 ? 界 面 ? 件
5 7 9
• GraphicsDevice getDefaultScreenDevice()
返回默认屏幕设备。 需要注意的是， 如果一个计算机有多个显示器， 每一个屏幕会有一
个图形设备—
可以使用 getScreenDevices 方法来得到包括所有屏幕设备的一个数组。
|AFI{ java.awt.Robot 1.3
參 Robot(GraphicsDevice device)
构造一个可以与给定设备交互的机器人。
• void keyPress(int key)
• void keyRelease(int key)
模拟按下或松开按键。
参数：key
按键码。关于按键码的更多信息参见 Keystroke 类。
• void mouseMove(int x , int y)
模拟鼠标移动。
参数： x，y
鼠标位置 （绝对像素坐标）
• void mousePress(int eventMask)
• void mouseRelease(int eventMask)
模拟按下或松开一个鼠标按钮。
参数：eventMask
描述鼠标按钮的事件掩码。 关于事件掩码的更多信息参见
InputEvent 类。
• void delay(int milliseconds)
将机器人延迟指定的时间 （毫秒数）。
• Bufferedlmage createScreenCapture(Rectangle rect)
捕获屏幕的一部分。
参数：rect
要捕获的矩形（绝对像素坐标)。
至此将结束用户界面组件的讨论。 第 10 章 〜 第 12 章讲述了如何用 Swing 实现简单的图
形用户界面。卷 n 将讨论更加高级的 Swing 组件和更加复杂的图形技术。