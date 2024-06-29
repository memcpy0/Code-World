第7章　高级AWT
▲绘图操作流程
▲绘图提示
▲形状
▲图像读取器和写入器
▲区域
▲图像处理
▲笔划
▲打印
▲着色
▲剪贴版
▲坐标变换
▲拖放操作
▲剪切
▲平台集成
▲透明与组合
Graphics类有多种方法可以用来创建简单的图形。这些方法对于简单的applet和应用来说已经绰绰有余了，但是当你创建复杂的图形或者需要全面控制图形的外观时，它们就显得力不从心了。Java 2D API是一个更加成熟的类库，可以用它产生高质量的图形。本章中，我们将概要地介绍一下该API。
然后我们将要讲述关于打印方面的问题，说明如何将打印功能纳入到程序之中。
最后，我们将介绍在程序间传递数据的两种方法：系统剪切板和拖放机制。可以使用这些技术在两个Java应用之间，或者在Java应用和本机程序之间传递数据。最后，我们将讨论使Java应用可以像本地应用一样使用的技术，例如提供闪屏和在系统托盘中的图标。7.1　绘图操作流程
在最初的JDK 1.0中，用来绘制形状的是一种非常简单的机制，即选择颜色和画图的模式，并调用Graphics类的各种方法，比如drawRect或者fillOval。而Java 2D API支持更多的功能：
·可以很容易地绘制各式各样的形状。
·可以控制绘制形状的笔划，即控制跟踪形状边界的绘图笔。
·可以用单色、变化的色调和重复的模式来填充各种形状。
·可以使用变换法，对各种形状进行移动、缩放、旋转和拉伸。
·可以对形状进行剪切，将其限制在任意的区域内。
·可以选择各种组合规则，来描述如何将新形状的像素与现有的像素组合起来。
·可以提供绘制图形提示，以便在速度与绘图质量之间实现平衡。
如果要绘制一个形状，可以按照如下步骤操作：
1）获得一个Graphics2D类的对象，该类是Graphics类的子类。自Java SE 1.2以来，paint和paintComponent等方法就能够自动地接收一个Graphics2D类的对象，这时可以直接使用如下的转型：
2）使用setRenderingHints方法来设置绘图提示，它提供了速度与绘图质量之间的一种平衡。
3）使用setStroke方法来设置笔划，笔划用于绘制形状的边框。可以选择边框的粗细和线段的虚实。
4）使用setPaint方法来设置着色法，着色法用于填充诸如笔划路径或者形状内部等区域的颜色。可以创建单色、渐变色或者平铺的填充模式。
5）使用clip方法来设置剪切区域。
6）使用transform方法设置一个从用户空间到设备空间的变换方式。如果使用变换方式比使用像素坐标更容易定义在定制坐标系统中的形状，那么就可以使用变换方式。
7）使用setComposite方法设置一个组合规则，用来描述如何将新像素与现有的像素组合起来。
8）建立一个形状，Java 2D API提供了用来组合各种形状的许多形状对象和方法。
9）绘制或者填充该形状。如果要绘制该形状，那么它的边框就会用笔划画出来。如果要填充该形状，那么它的内部就会被着色。
当然，在许多实际的环境中，并不需要采用所有这些操作步骤。Java 2D图形上下文中有合理的默认设置。只有当你确实想要改变设置时，再去修改这些默认设置。
在下面的几节中，我们将要介绍如何描绘形状、笔划、着色、变换及组合的规则。
各种不同的set方法只是用于设置2D图形上下文的状态，它们并不进行任何实际的绘图操作。同样，在构建shape对象时，也不进行任何绘图操作。只有在调用draw或者fill方法时，才会绘制出图形的形状，而就在此刻，这个新的图形由绘图操作流程计算出来（参见图7-1）。
图7-1　绘图操作流程
在绘图流程中，需要以下这些操作步骤来绘制一个形状：
1）用笔划画出形状的线条；
2）对形状进行变换操作；
3）对形状进行剪切。如果形状与剪切区域之间没有任何相交的地方，那么就停止本次操作过程；
4）对剪切后的形状进行填充；
5）把填充后的形状与已有的形状进行组合（在图7-1中，圆形是已有像素部分，杯子的形状加在了它的上面）。
在下一节中，将会讲述如何对形状进行定义。然后，我们将转而对2D图形上下文设置进行介绍。
java.awt.Graphics2D 1.2
·void draw（Shape s）
用当前的笔划来绘制给定形状的边框。
·void fill（Shape s）
用当前的着色方案来填充给定形状的内部。7.2　形状
Graphics类中绘制形状的若干方法：
它们还有对应的fill方法，这些方法从JDK 1.0起就被纳入到Graphics类中了。Java 2D API使用了一套完全不同的面向对象的处理方法，即不再使用方法，而是使用下面的这些类：
这些类全部都实现了Shape接口。
最后，还有一个Point2D类，它用x和y坐标来描述一个点。点对于定义形状非常有用，不过它们本身并不是形状。
如果要绘制一个形状，首先要创建一个实现了Shape接口的类的对象，然后调用Graphics2D类的draw方法。
Line2D、Rectangle2D、RoundRectangle2D、Ellipse2D和Arc2D等这些类对应于drawLine、drawRectangle、drawRoundRect、drawOval和drawArc等方法。（“3D矩形”的概念已经理所当然地过时了，因没有与draw3DRect方法相对应的类。）Java 2D API提供了两个补充类，即二次曲线类和三次曲线类。我们将在本节的后面部分阐释这些形状。Java 2D API中没有任何Polygon2D类。相反，它用GeneralPath类来描述由线条、二次曲线、三次曲线构成的线条路径。可以使用GeneralPath来描述一个多边形；我们将在本节的后面部分对它进行介绍。
下面这些类：
都是从一个公共超类RectangularShape继承而来的。诚然，椭圆形和弧形都不是矩形，但是它们都有一个矩形的边界框（参见图7-2）。
图7-2　椭圆形和弧形的矩形边界框
名字以“2D”结尾的每个类都有两个子类，用于指定坐标是float类型的还是double类型的。在本书的第I卷中，我们已经介绍了Rectangle2D.Float和Rectangle2D.Double。
其他类也使用了相同的模式，比如Arc2D.Float和Arc2D.Double。
从内部来讲，所有的图形类使用的都是float类型的坐标，因为float类型的数占用较少的存储空间，而且它们有足够高的几何计算精度。然而，Java编程语言使得对float类型的数的操作要稍微复杂些。由于这个原因，图形类的大多数方法使用的都是double类型的参数和返回值。只有在创建一个2D对象的时候，才需要选择究竟是使用带有float类型坐标的构造器，还是使用带有double类型坐标的构造器。例如：
Xxx2D.Float和Xxx2D.Double两个类都是Xxx2D类的子类，在对象被构建之后，再记住其确切的子类型实质上已经没有任何额外的好处了，因此可以将刚被构建的对象存储为一个超类变量，正如上面代码示例中所阐释的那样。
从这些类古怪的名字中就可以判断出，Xxx2D.Float和Xxx2D.Double两个类同时也是Xxx2D类的内部类。这只是为了在语法上比较方便，以避免外部类的名字变得太长。
图7-3显示了各个形状类之间的关系。不过图中省略了Double和Float子类，并且来自以前的2D类库的遗留类用灰色的填充色标识。
图7-3　形状类之间的关系
使用形状类
我们在本书的第I卷第7章中介绍了如何使用Rectangle2D、Ellipse2D和Line2D类的方法。本节将介绍如何建立其他的2D形状。
如果要建立一个RoundRectangle2D形状，应该设定左上角、宽度、高度及应该变成圆角的边角区的x和y的坐标尺寸（参见图7-4）。例如，调用下面的方法：
便产生了一个带圆角的矩形，每个角的圆半径为20。
图7-4　构建一个RoundRectangle2D
如果要建立一个弧形，首先应该设定边界框，接着设定它的起始角度和弧形跨越的角度（见图7-5），并且设定弧形闭合的类型，即Arc2D.OPEN、Arc2D.PIE或者Arc2D.CHORD这几种类型中的一个。
图7-5　构建一个椭圆弧形
图7-6显示了几种弧形的类型。
警告：如果弧形是椭圆的，那么弧形角的计算就不是很直接了。API文档中描述到：“角是相对于非正方形的矩形边框指定的，以使得45度总是落到了从椭圆中心指向矩形边框右上角的方向上。因此，如果矩形边框的一条轴比另一条轴明显长许多，那么弧形段的起始点和终止点就会与边框中的长轴斜交。”但是，文档中并没有说明如何计算这种“斜交”。下面是其细节：
假设弧形的中心是原点，而且点（x，y）在弧形上。那么我们可以用下面的公式来获得这个斜交角：
这个值介于–180到180之间。按照这种方式计算斜交的起始角和终止角，然后，计算两个斜交角之间的差，如果起始角或角的差是负数，则加上360。之后，将起始角和角的差提供给弧形的构造器。如果运行本节末尾的程序，你用肉眼就能观察到这种计算所产生的用于弧形构造器的值是正确的。可参见本章图7-9。
图7-6　弧形的类型
Java 2D API提供了对二次曲线和三次曲线的支持。在本章中，我们并不会深入介绍这些曲线的数学特征。我们建议你通过运行程序清单7-1的代码，对曲线的形状有一个感性的认识。正如在图7-7和图7-8中看到的那样，二次曲线和三次曲线是由两个端点和一个或两个控制点来设定的。移动控制点，曲线的形状就会改变。
如果要构建二次曲线和三次曲线，需要给出两个端点和控制点的坐标。例如，
二次曲线不是非常灵活，所以实际上它并不常用。三次曲线（比如用CubicCurve2D类绘制的贝济埃（Bézier）曲线）却是非常常用的。通过将三次曲线组合起来，使得连接点的各个斜率互相匹配，就能够创建复杂的、外观平滑的曲线形状。如果要了解这方面的详细信息，请参阅James D.Foley、Andries van Dam和Steven K.Feiner等人合作撰写的《Computer Graphics：Principles and Practice》[1]，Addison Wesley出版社1995年出版。
图7-7　二次曲线
图7-8　三次曲线
可以建立线段、二次曲线和三次曲线的任意序列，并把它们存放到一个GeneralPath对象中去。可以用moveTo方法来指定路径的第一个坐标，例如，
然后，可以通过调用lineTo、quadTo或curveTo三种方法之一来扩展路径，这些方法分别用线条、二次曲线或者三次曲线来扩展路径。如果要调用lineTo方法，需要提供它的端点。而对两个曲线方法的调用，需要先提供控制点，然后提供端点。例如，
可以调用closePath方法来闭合路径，它能够绘制一条回到路径起始点的线条。
如果要绘制一个多边形，只需调用moveTo方法，以到达第一个拐角点，然后反复调用lineTo方法，以便到达其他的拐角点。最后调用closePath方法来闭合多边形。程序清单7-1更加详细地展示了构建多边形的方法。
普通路径没有必要一定连接在一起，随时可以调用moveTo方法来建立一个新的路径段。
最后，可以使用append方法，向普通路径添加任意个Shape对象。如果新建的形状应该连接到路径的最后一个端点，那么append方法的第二个参数值就是true，如果不应该连接，那么该参数值就是false。例如，调用下面的方法：
可以把矩形的边框添加到该路径中，但并不与现有的路径连接在一起。而下面的方法调用：
则是在路径的终点和矩形的起点之间添加了一条直线，然后将矩形的边框添加到该路径中。
程序清单7-1中的程序使你能够构建许多示例路径。图7-7和图7-8显示了运行该程序的示例结果。你可以从组合框中选择一个形状绘制器，该程序包含的形状绘制器可以用来绘制：
·直线；
·矩形、圆角矩形和椭圆形；
·弧形（除了显示弧形本身外，还可以显示矩形边框的线条和起始角度及结束角度）；
·多边形（使用GeneralPath方法）；
·二次曲线和三次曲线。
可以用鼠标来调整控制点。当你移动控制点时，形状会连续地重绘。
该程序有些复杂，因为它可以用来处理多种不同的形状，并且支持对控制点的拖拽操作。
抽象超类ShapeMaker封装了形状绘制器类的共性特征。每个形状都拥有固定数量的控制点，用户可以在控制点周围随意移动，而getPointCount方法用于返回控制点数量的值。下面这个抽象方法：
将在给定控制点的当前位置的情况下，计算实际的形状。toString方法用于返回类的名字，这样，ShapeMaker对象就能够放置到一个JComboBox中。
为了激活控制点的拖拽特征，ShapePanel类要同时处理鼠标事件和鼠标移动事件。当鼠标在一个矩形上面被按下时，那么拖拽鼠标就可以移动该矩形了。
大部分形状绘制器类都很简单，它们的makeShape方法只是用于构建和返回需要的形状。然而，当使用ArcMaker类的时候，需要计算弧形的变形起始角度和结束角度。此外，为了说明这些计算确实是正确的，返回的形状应该是包含该弧本身、矩形边框和从弧形中心到角度控制点之间的线条等的GeneralPath（参见图7-9）。
图7-9　ShapeTest程序的运行结果
程序清单7-1　shape/ShapeTest.java
java.awt.geom.RoundRectangle2D.Double 1.2
·RoundRectangle2D.Double（double x，double y，double width，double heigth，double arcWidth，double arcHeight）
用给定的矩形边框和弧形尺寸构建一个圆角矩形。参见图7-4有关arcWidth和arcHeight参数的解释。
java.awt.geom.Arc2D.Double 1.2
·Arc2D.Double（double x，double y，double w，double h，double startAngle，double arcAngle，int type）
用给定的矩形边框、起始角度、弧形角度和弧形类型构建一个弧形。startAngle和arcAngle在图7-5中已做介绍，type是Arc2D.OPEN、Arc2D.PIE和Arc2D.CHORD之一。
java.awt.geom.QuadCurve2D.Double 1.2
·QuadCurve2D.Double（double x1，double y1，double ctrlx，double ctrly，double x2，double y2）
用起始点、控制点和结束点构建一条二次曲线。
java.awt.geom.CubicCurve2D.Double 1.2
·CubicCurve2D.Double（double x1，double y1，double ctrlx1，double ctrly1，double ctrlx2，double ctrly2，double x2，double y2）
用起始点、两个控制点和结束点构建一条三次曲线。
java.awt.geom.GeneralPath 1.2
·GeneralPath（）
构建一条空的普通路径。
java.awt.geom.Path2D.Float 6
·void moveTo（float x，float y）
使（x，y）成为当前点，也就是下一个线段的起始点。
·void lineTo（float x，float y）
·void quadTo（float ctrlx，float ctrly，float x，float y）
·void curveTo（float ctrl1x，float ctrl1y，float ctrl2x，float ctrl2y，float x，float y）
从当前点绘制一个线条、二次曲线或者三次曲线到达结束点（x，y），并且使该结束点成为当前点。
java.awt.geom.Path2D 6
·void append（Shape s，boolean connect）
将给定形状的边框添加到普通路径中去。如果布尔型变量connect的值是true的话，那么该普通路径的当前点与添加进来的形状的起始点之间用一条直线连接起来。
·void closePath（）
从当前点到路径的第一点之间绘制一条直线，从而使路径闭合。
[1]本书的中文版以及英文影印版已由机械工业出版社出版。中文版书名为《计算机图形学原理及实践》书号为：7-111-13026-x，英文影印版书号为：7-111-10343-2。——编辑注7.3　区域
在上一节中，我们介绍了如何通过建立由线条和曲线构成的普通路径来绘制复杂的形状。通过使用足够数量的线条和曲线可以绘制出任何一种形状，例如，在屏幕上和打印文件上看到的字符的各种字体形状，都是由线条和三次曲线构成的。
有时候，使用各种不同形状的区域，比如矩形、多边形和椭圆形来建立形状，可能会更加容易描述。Java 2D API支持四种区域几何作图（constructive area geometry）操作，用于将两个区域组合成一个区域。
·add：组合区域包含了所有位于第一个区域或第二个区域内的点。
·subtract：组合区域包含了所有位于第一个区域内的点，但是不包括任何位于第二个区域内的点。
·intersect：组合区域包含了所有既位于第一个区域内，又位于第二个区域内的点。
·exclusiveOr：组合区域包含了所有位于第一个区域内，或者是位于第二个区域内的所有点，但是这些点不能同时位于两个区域内。
图7-10显示了这些操作的结果。
图7-10　区域几何作图操作
如果要构建一个复杂的区域，可以使用下面的方法先创建一个默认的区域对象。
然后，将该区域和其他的形状组合起来：
Area类实现了Shape接口。可以用draw方法勾勒出该区域的边界，或者使用Graphics2D类的fill方法给区域的内部着色。
java.awt.geom.Area
·void add（Area other）
·void subtract（Area other）
·void intersect（Area other）
·void exclusiveOr（Area other）
对该区域和other所代表的另一个区域执行区域几何作图操作，并且将该区域设置为执行后的结果。7.4　笔划
Graphics2D类的draw操作通过使用当前选定的笔划来绘制一个形状的边界。在默认的情况下，笔划是一条宽度为一个像素的实线。可以通过调用setStroke方法来选定不同的笔划，此时要提供一个实现了Stroke接口的类的对象。Java 2D API只定义了一个这样的类，即BasicStroke类。在本节中，我们将介绍BasicStroke类的功能。
你可以构建任意粗细的笔划。例如，下面的方法就绘制了一条粗细为10个像素的线条。
当一个笔划的粗细大于一个像素的宽度时，笔划的末端可采用不同的样式。图7-11显示了这些所谓的端头样式。端头样式有下面三种：
·平头样式（butt cap）在笔划的末端处就结束了；
·圆头样式（round cap）在笔划的末端处加了一个半圆；
·方头样式（square cap）在笔划的末端处加了半个方块。
当两个较粗的笔划相遇时，有三种笔划的连接样式可供选择（参见图7-12）：
图7-11　笔划的端头样式
图7-12　笔划的连接样式
·斜连接（bevel join），用一条直线将两个笔划连接起来，该直线与两个笔划之间的夹角的平分线相垂直。
·圆连结（round join），延长了每个笔划，并使其带有一个圆头。
·斜尖连接（miter join），通过增加一个尖峰，从而同时延长了两个笔划。
斜尖连接不适合小角度连接的线条。如果两条线连接后的角度小于斜尖连接的最小角度，那么应该使用斜连接。斜连接的使用，可以防止出现太长的尖峰。默认情况下，斜尖连接的最小角度是10度。
可以在BasicStroke构造器中设定这些选择，例如：
最后，通过设置一个虚线模式，可以指定需要使用的虚线。在程序清单7-2的程序中，可以选择一个虚线模式，拼出摩斯电码中的SOS代码。虚线模式是一个float[]类型的数组，它包含了笔划中“连接（on）”和“断开（off）”的长度（见图7-13）。
图7-13　一种虚线图案
当构建BasicStroke时，可以指定虚线模式和虚线相位（dash phase）。虚线相位用来表示每条线应该从虚线模式的何处开始。通常情况下，应该把它的值设置为0。
注意：在虚线模式中，每一条虚线的末端都可以应用端头样式。
程序清单7-2中的程序可以设定端头样式、连接样式和虚线（见图7-14）。可以移动线段的端头，用以测试斜尖连接的最小角度：首先选定斜尖连接；然后，移动线段末端形成一个非常尖的锐角。可以看到斜尖连接变成了一个斜连接。
图7-14　StrokeTest程序
这个程序类似于程序清单7-1的程序。当点击一个线段的末端时，鼠标监听器就会记下操作，而鼠标动作监听器则监听对端点的拖曳操作。一组单选按钮用以表示用户选择的端头样式、连接样式以及实线或虚线。StrokePanel类的paintComponent方法构建了一个GeneralPath，它由连接着用户可以用鼠标移动的三个点的两条线段构成。然后，它根据用户的选择构建一个BasicStroke，最后绘制出这个路径。
程序清单7-2　stroke/StrokeTest.java
java.awt.Graphics2D 1.2
·void setStroke（Stroke s）
将该图形上下文的笔划设置为实现了Stroke接口的给定对象。
java.awt.BasicStroke 1.2
·BasicStroke（float width）
·BasicStroke（float width，int cap，int join）
·BasicStroke（float width，int cap，int join，float miterlimit）
·BasicStroke（float width，int cap，int join，float miterlimit，float[]dash，float dashPhase）
用给定的属性构建一个笔划对象。
参数：width　画笔的宽度
cap　端头样式，它是CAP_BUTT、CAP_ROUND和CAP_SQUARE三种样式中的一个
join　连接样式，它是JOIN_BEVEL、JOIN_MITER和JOIN_ROUND三种样式中的一个
miterlimit　用度数表示的角度，如果小于这个角度，斜尖连接将呈现为斜连接
dash　虚线笔划的填充部分和空白部分交替出现的一组长度
dashPhase　虚线模式的“相位”；位于笔划起始点前面的这段长度被假设为已经应用了该虚线模式7.5　着色
当填充一个形状的时候，该形状的内部就上了颜色。使用setPaint方法，可以把颜色的样式设定为一个实现了Paint接口的类的对象。Java 2D API提供了三个这样的类：
·Color类实现了Paint接口。如果要用单色填充形状，只需要用Color对象调用setPaint方法即可，例如：
·GradientPaint类通过在两个给定的颜色值之间进行渐变，从而改变使用的颜色（参见图7-15）。
·TexturePaint类用一个图像重复地对一个区域进行着色（见图7-16）。
图7-15　渐变着色
图7-16　纹理着色
可以通过指定两个点以及在这两个点上想使用的颜色来构建一个GradientPaint对象，即：
上面语句将沿着连接两个点之间的直线的方向对颜色进行渐变，而沿着与该连接线垂直方向上的线条颜色则是不变的。超过线条端点的各个点被赋予端点上的颜色。
另外，如果调用GradientPaint构造器时cyclic参数的值为true，即：
那么颜色将循环变换，并且在端点之外仍然保持这种变换。
如果要构建一个TexturePaint对象，需要指定一个BufferedImage和一个锚位矩形。
在本章后面部分详细讨论图像时，我们再介绍BufferedImage类。获取缓冲图像最简单的方式就是读入图像文件：
锚位矩形在x和y方向上将不断地重复延伸，使之平铺到整个坐标平面。图像可以伸缩，以便纳入该锚位，然后复制到每一个平铺显示区中。
java.awt.Graphics2D 1.2
·void setPaint（Paint s）
将图形上下文的着色设置为实现了Paint接口的给定对象。
java.awt.GradientPaint 1.2
·GradientPaint（float x1，float y1，Color color1，float x2，float y2，Color color2）
·GradientPaint（float x1，float y1，Color color1，float x2，float y2，Color color2，boolean cyclic）
·GradientPaint（Point2D p1，Color color1，Point2D p2，Color color2）
·GradientPaint（Point2D p1，Color color1，Point2D p2，Color color2，boolean cyclic）
构建一个渐变着色的对象，以便用颜色来填充各个形状，其中，起始点的颜色为color1，结束点的颜色为color2，而两个点之间的颜色则是以线性的方式渐变。沿着连接起始点和结束点之间的线条相垂直的方向上的线条颜色是恒定不变的。在默认的情况下，渐变着色不是循环变换的。也就是说，起始点和结束点之外的各个点的颜色是分别与起始点和结束点的颜色相同的。如果渐变着色是循环的，那么颜色是连续变换的，首先返回到起始点的颜色，然后在两个方向上无限地重复。
java.awt.TexturePaint 1.2
·TexturePaint（BufferedImage texture，Rectangle2D anchor）
建立纹理着色对象。锚位矩形定义了色的平铺空间，该矩形在x和y方向上不断地重复延伸，纹理图像则被缩放，以便填充每个平铺空间。7.6　坐标变换
假设我们要绘制一个对象，比如汽车。从制造商的规格说明书中可以了解到汽车的高度、轴距和整个车身的长度。如果设定了每米的像素个数，当然就可以计算出所有像素的位置。但是，可以使用更加容易的方法：让图形上下文来执行这种转换。
Graphics2D类的scale方法可以将图形上下文中的坐标变换设置为一个比例变换。这种变换能够将用户坐标（用户设定的单元）转换成设备坐标（pixel，即像素）。图7-17显示了如何进行这种变换的方法。
图7-17　用户坐标与设备坐标
坐标变换在实际应用中非常有用，程序点可以使用方便的坐标值进行各种操作，图形上下文则负责执行将坐标值变换成像素的复杂工作。
这里有四种基本的变换：
·比例缩放：放大和缩小从一个固定点出发的所有距离。
·旋转：环绕着一个固定中心旋转所有点。
·平移：将所有的点移动一个固定量。
·切变：使一个线条固定不变，再按照与该固定线条之间的距离，成比例地将与该线条平行的各个线条“滑动”一个距离量。
图7-18显示了对一个单位的正方形进行这四种基本变换操作的效果。
图7-18　基本的变换
Graphics2D类的scale、rotate、translate和shear等方法用以将图形上下文中的坐标变换设置成为以上这些基本变换中的一种。
可以组合不同的变换操作。例如，你可能想对图形进行旋转和两倍尺寸放大的操作，这时，可以同时提供旋转和比例缩放的变换：
在这种情况下，变换方法的顺序是无关紧要的。然而，在大多数变换操作中，顺序却是很重要的。例如，如果想对形状进行旋转和切变操作，那么两种变换操作的不同执行序列，将会产生不同的图形。你必须明确想要得到的是什么样的图形，图形上下文将按照你所提供的相反顺序来应用这些变换操作。也就是说，你最后提供的方法会被最先使用。
可以根据你的需要提供任意多的变换操作。例如，假设你提供了下面这个变换操作序列：
最后一个变换操作（它是第一个被应用的）将把某个形状从点（x，y）移动到原点，第二个变换将使该形状围绕着原点旋转一个角度a，最后一个变换方法又重新把该形状从原点移动到点（x，y）处。总体效果就是该形状围绕着中心点（x，y）进行了一次旋转（参见图7-19）。围绕着原点之外的任意点进行旋转是一个很常见的操作，所以我们采用下面的快捷方法：
如果对矩阵理论有所了解，那么就会知道所有操作（诸如旋转、平移、缩放、切变）和由这些操作组合起来的操作都能够以如下矩阵变换的形式表示出来：
这种变换称为仿射变换（affine transformation）。Java 2D API中的AffineTransform类就是用于描述这种变换的。如果你知道某个特定变换矩阵的组成元素，就可以用下面的方法直接构造它：
另外，工厂方法getRotateInstance、getScaleInstance、getTranslateInstance和getShearInstance能够构建出表示相应变换类型的矩阵。例如，调用下面的方法：
将返回一个与下面这个矩阵相一致的变换。
图7-19　组合变换操作的应用
最后，实例方法setToRotation、setToScale、setToTranslation和setToShear用于将变换对象设置为一个新的类型。下面是一个例子：
可以把图形上下文的坐标变换设置为一个AffineTransform对象：
不过，在实际运用中，不要调用setTransform操作，因为它会取代图形上下文中可能存在的任何现有的变换。例如，一个用以横向打印的图形上下文已经有了一个90°的旋转变换，如果调用方法setTransfrom，就会删除这样的旋转操作。可以调用transform方法作为替代方案：
它会把现有的变换操作和新的AffineTransform对象组合起来。
如果只想临时应用某个变换操作，那么应该首先获得旧的变换操作，然后和新的变换操作组合起来，最后当你完成操作时，再还原旧的变换操作：
java.awt.geom.AffineTransform 1.2
·AffineTransform（double a，double b，double c，double d，double e，double f）
·AffineTransform（float a，float b，float c，float d，float e，float f）
用下面的矩阵构建该仿射变换。
·AffineTransform（double[]m）
·AffineTransform（float[]m）
用下面的矩阵构建该仿射变换。
·static AffineTransform getRotateInstance（double a）
创建一个围绕原点、旋转角度为a（弧度）的旋转变换。其变换矩阵是：
如果a在0到/2之间，那么图形将沿着x轴正半轴向y轴正半轴的方向旋转。
·static AffineTransform getRotateInstance（double a，double x，double y）
创建一个围绕点（x，y）、旋转角度为a（弧度）的旋转变换。
·static AffineTransform getScaleInstance（double sx，double sy）
创建一个比例缩放变换。x轴缩放幅度为sx；y轴缩放幅度为sy。其变换矩阵是：
·static AffineTransform getShearInstance（double shx，double shy）
创建一个切变变换。x轴切变shx；y轴切变shy。其变换矩阵是：
·static AffineTransform getTranslateInstance（double tx，double ty）
创建一个平移变换。x轴平移tx；y轴平移ty。其变换矩阵是：
·void setToRotation（double a）
·void setToRotation（double a，double x，double y）
·void setToScale（double sx，double sy）
·void setToShear（double sx，double sy）
·void setToTranslation（double tx，double ty）
用给定的参数将该变换设置为一个的基本变换。如果要了解基本变换和它们的参数说明，请参见getXxxInstance方法。
java.awt.Graphics2D 1.2
·void setTransform（AffineTransform t）
以t来取代该图形上下文中现有的坐标变换。
·void transform（AffineTransform t）
将该图形上下文的现有坐标变换和t组合起来。
·void rotate（double a）
·void rotate（double a，double x，double y）
·void scale（double sx，double sy）
·void shear（double sx，double sy）
·void translate（double tx，double ty）
将该图形上下文中现有的坐标变换和一个带有给定参数的基本变换组合起来。如果要了解基本变换和它们的参数说明，请参见AffineTransform.getXxxInstance方法。7.7　剪切
通过在图形上下文中设置一个剪切形状，就可以将所有的绘图操作限制在该剪切形状内部来进行。
但是，在实际应用中，不应该调用这个setClip操作，因为它会取代图形上下文中可能存在的任何剪切形状。例如，正如在本章的后面部分所看到的那样，用于打印操作的图形上下文就具有一个剪切矩形，以确保你不会在页边距上绘图。相反，你应该调用clip方法。
clip方法将你所提供的新的剪切形状同现有的剪切形状相交。
如果只想临时地使用一个剪切区域的话，那么应该首先获得旧的剪切形状，然后添加新的剪切形状，最后，在完成操作时，再还原旧的剪切形状：
在图7-20的例子中，我们炫耀了一下剪切的功能，它绘制了一个按照复杂形状进行剪切的相当出色的线条图案，即一组字符的轮廓。
图7-20　按照字母形状剪切出的线条图案
如果要获得字符的外形，需要一个字体渲染上下文（font render context）。请使用Graphics2D类的getFontRenderContext方法：
接着，使用某个字符串、某种字体和字体渲染上下文来创建一个TextLayout对象：
这个文本布局对象用于描述由特定字体渲染上下文所渲染的一个字符序列的布局。这种布局依赖于字体渲染上下文，相同的字符在屏幕上或者打印机上看起来会有不同的显示。
对我们当前的应用来说，更重要的是，getOutline方法将会返回一个Shape对象，这个Shape对象用以描述在文本布局中的各个字符轮廓的形状。字符轮廓的形状从原点（0，0）开始，这并不适合大多数的绘图操作。因此，必须为getOutline操作提供一个仿射变换操作，以便设定想要的字体轮廓所显示的位置：
接着，我们把字体的轮廓附加给剪切的形状：
最后，我们设置剪切形状，并且绘制一组线条。线条仅仅在字符边界的内部显示：
java.awt.Graphics 1.0
·void setClip（Shape s）1.2
将当前的剪切形状设置为形状s。
·Shape getClip（）1.2
返回当前的剪切形状。
java.awt.Graphics2D 1.2
·void clip（Shape s）
将当前的剪切形状和形状s相交。
·FontRenderContext getFontRenderContext（）
返回一个构建TextLayout对象所必需的字体渲染上下文。
java.awt.font.TextLayout 1.2
·TextLayout（String s，Font f，FontRenderContext context）
根据给定的字符串和字体来构建文本布局对象。方法中使用字体渲染上下文来获取特定设备的字体属性。
·float getAdvance（）
返回该文本布局的宽度。
·float getAscent（）
·float getDescent（）
返回基准线上方和下方该文本布局的高度。
·float getLeading（）
返回该文本布局使用的字体中相邻两行之间的距离。7.8　透明与组合
在标准的RGB颜色模型中，每种颜色都是由它的红、绿和蓝这三种成分来描述的。但是，用它来描述透明或者部分透明的图像区域也是非常方便的。当你将一个图像置于现有图像的上面时，透明的像素完全不会遮挡它们下面的像素，而部分透明的像素则与它们下面的像素相混合。图7-21显示了一个部分透明的矩形和一个图像相重叠时所产生的效果，我们仍然可以透过矩形看到该图像的细节。
图7-21　一个部分透明的矩形和一个图像相重叠时所显示的效果
在Java 2D API中，透明是由一个透明度通道（alpha channel）来描述的。每个像素，除了它的红、绿和蓝色部分外，还有一个介于0（完全透明）和1（部分透明）之间的透明度（alpha）值。例如，图7-21中的矩形填充了一种淡黄色，透明度为50%：
现在让我们看一看如果将两个形状重叠在一起时将会出现什么情况。必须把源像素和目标像素的颜色和透明度值混合或者组合起来。从事计算机图形学研究的Porter和Duff已经阐明了在这个混合过程中的12种可能的组合原则，Java 2D API实现了所有的这些原则。在继续介绍这个问题之前，需要指出的是，这些原则中只有两个原则有实际的意义。如果你发现这些原则晦涩难懂或者难以搞清楚，那么只使用SRC_OVER原则就可以了。它是Graphics2D对象的默认原则，并且它产生的结果最直接。
下面是这些规则的原理。假设你有了一个透明度值为aS的源像素，在该图像中，已经存在了一个透明度值为aD的目标像素，你想把两个像素组合起来。图7-22的示意图显示了如何设计一个像素的组合原则。
图7-22　设计一个像素组合的原则
Porter和Duff将透明度值作为像素颜色将被使用的概率。从源像素的角度来看，存在一个概率aS，它是源像素颜色被使用的概率；还存在一个概率1-aS，它是不使用该像素颜色的概率。同样的原则也适用于目标像素。当组合颜色时，我们假设源像素的概率和目标像素的概率是不相关的。那么正如图7-22所示，有四种组合情况。如果源像素想要使用它的颜色，而目标像素也不介意，那么很自然的，我们就只使用源像素的颜色。这也是为什么右上角的矩形框用“S”来标志的原因了，这种情况的概率为aS·（1-aD）。同理，左下角的矩形框用“D”来标志。如果源像素和目标像素都想选择自己的颜色，那该怎么办才好呢？这里就可以应用Porter-Duff原则了。如果我们认为源像素比较重要，那么我们在右下角的矩形框内也标志上一个“S”。这个规则被称为SRC_OVER。在这个规则中，我们赋予源像素颜色的权值aS，目标像素颜色的权值为然后将它们组合起来。
这样产生的视觉效果是源像素与目标像素相混合的结果，并且优先选择给定的源像素的颜色。特别是，如果aS为1，那么根本就不用考虑目标像素的颜色。如果aS为0，那么源像素将是完全透明的，而目标像素颜色则是不变的。
还有其他的规则，可以根据置于概率示意图各个框中的字母来理解这些规则的概念。表7-1和图7-23显示了Java 2D API支持的所有这些规则。图7-23中的各个图像显示了当你使用透明度值为0.75的矩形源区域和透明度值为1.0的椭圆目标区域组合时，所显示的各种组合效果。
图7-23　Porter-Duff组合规则
表7-1　Porter-Duff组合规则
如你所见，大多数规则并不是非常有用。例如，DST_IN规则就是一个极端的例子。它根本不考虑源像素颜色，但是却使用了源像素的透明度值来影响目标像素。SRC规则可能是有用的，它强制使用源像素颜色，而且关闭了与目标像素相混和的特性。
如果要了解更多的关于Porter-Duff规则的信息，请参阅Foley、Dam和Feiner等撰写的《Computer Graphics：Principles and Practice，Second Edition》（计算机图形学：原理语应用，第2版）。
你可以使用Graphics2D类的setComposite方法安装一个实现了Composite接口的类的对象。Java 2D API提供了这样的一个类，即AlphaComposite它实现了图7-23中的所有的Porter-Duff规则。
AlphaComposite类的工厂方法getInstance用来产生AlphaComposite对象，此时需要提供用于源像素的规则和透明度值。例如，可以考虑使用下面的代码：
这时，矩形将使用蓝色和值为0.5的透明度进行着色。因为该组合规则是SRC_OVER，所以它透明地置于现有图像的上面。
程序清单7-3中的程序深入地研究了这些组合规则。可以从组合框中选择一个规则，调节滑动条来设置AlphaComposite对象的透明度值。
此外，对每一条规则该程序都显示了一条文字描述。请注意，描述是根据组合规则表计算而来的。例如，第二行中的“DS”表示的就是“与目标像素相混合”。
该程序有一个重要的缺陷：它不能保证和屏幕相对应的图形上下文一定具有透明通道。（实际上，它通常没有这个透明通道）。当像素被放到没有透明通道的目标像素之上的时候，这些像素的颜色会与目标像素的透明度值相乘，而其透明度值却被弃用了。因为许多Porter-Duff规则都使用目标像素的透明度值，因此目标像素的透明通道是很重要的。由于这个原因，我们使用了一个采用ARGB颜色模型的缓存图像来组合各种形状。在图像被组合后，我们就将产生的图像在屏幕上绘制出来：
程序清单7-3和7-4展示了框体和构件类，程序清单7-5中的Rule类提供了对每条规则的简要解释，如图7-24所示。在运行这个程序的时候，从左到右地移动Alpha滑动条，就可以观察到所产生的组合形状的效果。特别是，请注意DST_IN与DST_OUT规则之间惟一的差别，那就是，当你改变源像素的透明度值时，目标（！）颜色将会发生什么样的变化。
图7-24　CompositeTest程序运行的结果
程序清单7-3　composite/CompositeTestFrame.java
程序清单7-4　composite/CompositeComponent.java
程序清单7-5　composite/Rule.java
java.awt.Graphics2D 1.2
·void setComposite（Composite s）
把图形上下文的组合方式设置为实现了Composite接口的给定对象。
java.awt.AlphaComposite 1.2
·static AlphaComposite getInstance（int rule）
·static AlphaComposite getInstance（int rule，float Source Alpha）
构建一个透明度（alpha）值的组合对象。规则是CLEAR，SRC，SRC_OVER，DST_OVER，SRC_IN，SRC_OUT，DST_IN，DST_OUT，DST，DST_ATOP，SRC_ATOP，XOR等值之一。7.9　绘图提示
在前面的小节中，已经看到了绘图过程是非常复杂的。虽然在大多数情况下Java 2D API的运行速度奇快，但是在某些情况下，你可能希望控制绘图的速度和质量之间的平衡关系。可以通过设置绘图提示来达到此目的。使用Graphics2D类的setRenderingHint方法，可以设置一条单一的绘图提示，提示的键和值是在RenderingHints类中声明的。表7-2汇总了可以使用的选项。以_DEFAULT结尾的值表示某种特定实现将其作为性能与质量之间的良好平衡而所选择的默认值。
这些设置中最有用的是消除图形锯齿现象的技术，这种技术消除了斜线和曲线中的“锯齿”（jaggies）。正如在图7-25所见的那样，斜线必须被绘制成为一个像素的“阶梯”。特别是在低分辨率的显示屏上，你所画的线条将非常难看。但是，如果不是完整地绘制或排除每一个像素，而是在线条所覆盖的像素中，用与被覆盖区域成比例的颜色，来着色被部分覆盖的元素，那么所产生的线条看上去就要平滑得多。这种技术被称为“消除图形锯齿状”技术。当然，使用这种技术所花费的时间要长一些，因为它需要花一定的时间去计算所有这些颜色的值。
表7-2　绘图提示
图7-25　消除图形锯齿现象的示意图
例如，下面的代码说明了应该如何请求使用消除图形锯齿状功能。
使用消除图形锯齿状技术对字体的绘制同样是有意义的。
和上面的这些应用相比较，其他的绘图提示并不是很常用。
也可以把一组键/值提示信息对放入映射表中，并且通过调用setRenderingHints方法一次性的将它们全部设置好。任何实现了映射表接口的集合类都可以被使用，当然也可以使用RenderingHints类本身。它实现了Map接口，并且在用无参数的构造器来创建对象时，会提供一个默认的映射表实现。例如，
这就是我们在程序清单7-6中使用的技术。该程序展示了几种我们认为会提供帮助的绘图提示。注意下面几点：
·消除锯齿功能使椭圆变得平滑。
·文本的消除锯齿功能使文本变得平滑。
·在某些平台上，值为小数的文本距离会使字母之间彼此靠得更近一些。
·选择VALUE_RENDER_QUALITY来平滑缩放的图像。（通过将KEY_INTERPOLATION设置为VALUE_INTERPOLATION_BICUBIC可以达到相同的效果）。
·当消除锯齿功能关闭时，选择VALUE_STROKE_NORMALIZE会改变椭圆的外观和正方形对角线的位置。
图7-26显示了运行该程序时所截取的一个屏幕。
图7-26　绘图提示测试程序的效果
程序清单7-6　renderQuality/RenderQualityTestFrame.java
java.awt.Graphics2D 1.2
·void setRenderingHint（RenderingHints.Key key，Object value）
为该图形上下文设置绘图提示。
·void setRenderingHints（Map m）
设置绘图提示，它们的键/值对存储在映射表中。
java.awt.RenderingHints 1.2
·RenderingHints（Map<RenderingHints.Key，？>m）
构建一个存放绘图提示的绘图提示映射表。如果m值为null，那么将会提供一个默认的绘图提示映射表。7.10　图像的读取器和写入器
Java SE在其1.4版本出现以前，只具备非常有限的对图像文件的读取和写入的功能。它可以读取GIF和JPEG格式的图像，但是对于写入图像的功能，它没有任何官方的支持。
现在这种情况已经得到了很大的改进。javax.imageio包包含了对读取和写入数种常用文件格式进行支持的“附加”特性。同时还包含了一个框架，使得第三方能够为其他图像格式的文件添加读取器和写入器。从Java SE 6开始，GIF、JPEG、PNG、BMP（Windows位图）和WBMP（无线位图）等文件格式都得到了支持。在较早的版本中，由于专利方面的原因，使得写入GIF格式文件不被支持。
该类库的基本应用是极其直接的。要想装载一个图像，可以使用ImageIO类的静态read方法。
ImageIO类根据文件的类型，选择一个合适的读取器。它可以参考文件的扩展名和文件开头的“幻数”（magic number）来选择读取器。如果没有找到合适的读取器或者读取器不能解码文件的内容，那么read方法将返回null。
把图像写入到文件中也是一样地简单。
这里，format字符串用来标识图像的格式，比如“JPEG”或者“PNG”。ImageIO类将选择一个合适的写入器以存储文件。7.10.1　获得图像文件类型的读取器和写入器
对于那些超出ImageIO类的静态read和write方法能力范围的高级图像读取和写入操作来说，首先需要获得合适的ImageReader和ImageWriter对象。ImageIO类列举了匹配下列条件之一的读取器和写入器。
·图像格式（比如“JPEG”）
·文件后缀（比如“jpg”）
·MIME类型（比如“image/jpeg”）
注意：MIME（Multipurpose Internet Mail Extensions standard）是“多用途因特网邮件扩展标准”的英文缩写。MIME标准定义了常用的数据格式，比如“image/jpeg”和“application/pdf”等。如果要了解定义MIME格式的RFC（征求意见的文件）的HTML版本，请访问http://www.oac.uci.edu/indiv/ehood/MIME。
例如，可以用下面的代码来获取一个JPEG格式文件的读取器。
getImageReadersBySuffix和getImageReadersByMIMEType这两个方法用于枚举与文件扩展名或MIME类型相匹配的读取器。
ImageIO类可能找到了多个读取器，而它们都能够读取某一特殊类型的图像文件。在这种情况下，必须从中选择一个，但是也许你不清楚怎么样才能选择一个最好的。如果要了解更多的关于读取器的信息，就要获取它的服务提供者接口：
然后，可以获得供应商的名字和版本号：
也许该信息能够帮助你决定选择哪一种读取器，或者你可以为你的程序用户提供一个读取器的列表，让他们做出选择。然而，目前来说，我们假定第一个列出来的读取器就能够满足用户的需求。
在程序清单7-7中，我们想查找所有可获得的读取器能够处理的文件的所有后缀，这样我们就可以在文件过滤器中使用它们。我们可以使用静态的ImageIO.getReader-FileSuffixes方法来达到此目的：
对于保存文件，相对来说更麻烦一些：我们希望为用户展示一个支持所有图像类型的菜单。可惜，IOImage类的getWriterFormateNames方法返回了一个相当奇怪的列表，里边包含了许多冗余的名字，比如：
这些并不是人们想要在菜单中显示的东西，我们所需要的是“首选”格式名列表。我们提供了一个用于此目的的助手方法getWriterFormats（参见程序清单7-7）。我们查找与每一种格式名相关的第一个写入器，然后，询问写入器它支持的格式名是什么，从而希望它能够将最流行的一个格式名列在首位。实际上，对JPEG写入器来说，这种方法确实很有效：它将“JPEG”列在其他选项的前面。（另一方面，PNG写入器把小写字母的“png”列在“PNG”的前面。我们希望这种行为能够在将来的某个时候得以解决。同时，我们强制将全小写名字转换为大写）。一旦挑选了首选名，我们就会将所有其他的候选名从最初的名字集中移除。之后，我们会继续执行直至所有的格式名都得到处理。7.10.2　读取和写入带有多个图像的文件
有些文件，特别是GIF动画文件，都包含了多个图像。ImageIO类的read方法只能够读取单一的图像文件。为了读取多个图像，应该将输入源（例如，输入流或者输入文件）转换成一个ImageInputStream。
接着把图像输入流作为参数传递给读取器的setInput方法：
方法中的第二个参数值表示输入的方式是“只向前搜索”，否则，就采用随机访问的方式，要么是在读取时将缓冲输入流，要么是使用随机文件访问。对于某些操作来说，必须使用随机访问的方法。例如，为了在一个GIF文件中查寻图像的个数，就需要读入整个文件。这时，如果想获取某一图像的话，必须再次读入该输入文件。
只有当从一个流中读取图像，并且输入流中包含多个图像，而且在文件头中的图像格式部分没有所需要的信息（比如图像的个数）时，考虑使用上面的方法才是合适的。如果要从一个文件中读取图像信息的话，可直接使用下面的方法：
一旦拥有了一个读取器后，就可以通过调用下面的方法来读取输入流中的图像。
其中index是图像的索引，其值从0开始。
如果输入流采用“只向前搜索”的方式，那么应该持续不断地读取图像，直到read方法抛出一个IndexOutOfBoundsException为止。否则，可以调用getNumImages方法：
在该方法中，它的参数表示允许搜索输入流来确定图像的数目。如果输入流采用“只向前搜索”的方式，那么该方法将抛出一个IllegalStateException异常。要不然，可以把是否“允许搜索”参数设置为false。如果getNumImages方法在不搜索输入流的情况下无法确定图像的数目，那么它将返回-1。在这种情况下，必须转换到B方案，那就是持续不断地读取图像，直到获得一个IndexOutOfBoundsException异常为止。
有些文件包含一些缩略图，也就是图像用来预览的小版本。可以通过调用下面的方法来获得某个图像的缩略图数量。
然后可以按如下方式得到一个特定索引：
另一个问题是，有时你想在实际获得图像之前，了解该图像的大小。特别是，当图像很大，或者是从一个较慢的网络连接中获取的时候，你更加希望能够事先了解到该图像的大小。那么请使用下面的方法：
通过上面两个方法可以获得具有给定索引的图像的大小。
如果要将多个图像写入到一个文件中，首先需要一个ImageWriter。ImageIO类能够枚举可以写入某种特定图像格式的所有写入器。
接着，将一个输出流或者输出文件转换成ImageOutputStream，并且将其作为参数传给写入器。例如，
必须将每一个图像都包装到IIOImage对象中。可以根据情况提供一个缩略图和图像元数据（比如，图像的压缩算法和颜色信息）的列表。在本例中，我们把两者都设置为null；如果要了解详细信息，请参阅API文档。
使用write方法，可以写出第一个图像：
对于后续的图像，使用下面的方法：
上面方法中的第三个参数可以包含一个ImageWriteParam对象，用以设置图像写入的详细信息，比如是平铺还是压缩；可以用null作为其默认值。
并不是所有的图像格式都能够处理多个图像的。在这种情况下，如果i>0，canInsertImage方法将返回false值，而且只保存单一图像。
程序清单7-7中的程序使用Java类库所提供的读取器和写入器支持的格式来加载和保持文件。该程序显示了多个图像（见图7-27），但是没有缩略图。
图7-27　一个GIF动画图像
程序清单7-7　imageIO/ImageIOFrame.java
javax.imageio.ImageIO 1.4
·static BufferedImage read（File input）
·static BufferedImage read（InputStream input）
·static BufferedImage read（URL input）
从input中读取一个图像。
·static boolean write（RenderedImage image，String formatName，File output）
·static boolean write（RenderedImage image，String formatName，OutputStream output）
将给定格式的图像写入output中。如果没有找到合适的写入器，则返回false。
·static Iterator<ImageReader>getImageReadersByFormatName（String formatName）
·static Iterator<ImageReader>getImageReadersBySuffix（String fileSuffix）
·static Iterator<ImageReader>getImageReadersByMIMEType（String mimeType）
·static Iterator<ImageWriter>getImageWritersByFormatName（String formatName）
·static Iterator<ImageWriter>getImageWritersBySuffix（String fileSuffix）
·static Iterator<ImageWriter>getImageWritersByMIMEType（String mimeType）
获得能够处理给定格式（例如“JPEG”）、文件后缀（例如“jpg”）或者MIME类型（例如“image/jpeg”）的所有读取器和写入器。
·static String[]getReaderFormatNames（）
·static String[]getReaderMIMETypes（）
·static String[]getWriterFormatNames（）
·static String[]getWriterMIMETypes（）
·static String[]getReaderFileSuffixes（）6
·static String[]getWriterFileSuffixes（）6
获取读取器和写入器所支持的所有格式名、MIME类型名和文件后缀。
·ImageInputStream createImageInputStream（Object input）
·ImageOutputStream createImageOutputStream（Object output）
根据给定的对象来创建一个图像输入流或者图像输出流。该对象可能是一个文件、一个流、一个RandomAccessFile或者某个服务提供商能够处理的其他类型的对象。如果没有任何注册过的服务提供器能够处理这个对象，那么返回null值。
javax.imageio.ImageReader 1.4
·void setInput（Object input）
·void setInput（Object input，boolean seekForwardOnly）
设置读取器的输入源。
参数：input　一个ImageInputStream对象或者是这个读取器能够接受的其他对象
seekForwardOnly　如果读取器只应该向前读取，则返回ture。默认地，读取器采用随机访问的方式，如果有必要，将会缓存图像数据
·BufferedImage read（int index）
读取给定图像索引的图像（索引从0开始）。如果没有这个图像，则抛出一个IndexOutOfBoundsException异常。
·int getNumImages（boolean allowSearch）
获取读取器中图像的数目。如果allowSearch值为false，并且不向前阅读就无法确定图像的数目，那么它将返回-1。如果allowSearch值是true，并且读取器采用了“只向前搜索”方式，那么就会抛出IllegalStateException异常。
·int getNumThumbnails（int index）
获取给定索引的图像的缩略图的数量。
·BufferedImage readThumbnail（int index，int thumbnailIndex）
获取给定索引的图像的索引号为thumbnailIndex的缩略图。
·int getWidth（int index）
·int getHeight（int index）
获取图像的宽度和高度。如果没有这样的图像，就抛出一个IndexOutOfBounds-Exception异常。
·ImageReaderSpi getOriginatingProvider（）
获取构建该读取器的服务提供者。
javax.imageio.spi.IIOServiceProvider 1.4
·String getVendorName（）
·String getVersion（）
获取该服务提供者的提供商的名字和版本。
javax.imageio.spi.ImageReaderWriterSpi 1.4
·String[]getFormatNames（）
·String[]getFileSuffixes（）
·String[]getMIMETypes（）
获取由该服务提供者创建的读取器或者写入器所支持的图像格式名、文件的后缀和MIME类型。
javax.imageio.ImageWriter 1.4
·void setOutput（Object output）
设置该写入器的输出目标。
参数：output　一个ImageOutputSteam对象或者这个写入器能够接受的其他对象。
·void write（IIOImage image）
·void write（RenderedImage image）
把单一的图像写入到输出流中。
·void writeInsert（int index，IIOImage image，ImageWriteParam param）
把一个图像写入到一个包含多个图像的文件中。
·boolean canInsertImage（int index）
如果在给定的索引处可以插入一个图像的话，则返回true值。
·ImageWriterSpi getOriginatingProvider（）
获取构建该写入器的服务提供者。
javax.imageio.IIOImage 1.4
·IIOImage（RenderedImage image，List thumbnails，IIOMetadata metadata）
根据一个图像、可选的缩略图和可选的元数据来构建一个IIOImage对象。7.11　图像处理
假设你有一个图像，并且希望改善图像的外观。这时需要访问该图像的每一个像素，并用其他的像素来取代这些像素。或者，你也许想要从头计算某个图像的像素，例如，你想显示一下物理测量或者数学计算的结果。BufferedImage类提供了对图像中像素的控制能力，而实现了BufferedImageOP接口的类都可以对图像进行变换操作。
注意：JDK1.0有一个完全不同且复杂得多的图像框架，它得到了优化，以支持对从Web下载的图像进行增量渲染（incremental rendering），即一次绘制一个扫描行。但是，操作这些图像很困难。我们在本书中不讨论这个框架。7.11.1　构建光栅图像
你处理的大多数图像都是直接从图像文件中读入的。这些图像有的可能是数码相机产生的，有的是扫描仪扫描而产生的，还有的一些图像是绘图程序产生的。在本节中，我们将介绍一种不同的构建图像技术，也就是一次为图像增加一个像素。
为了创建一个图像，需要以通常的方法构建一个BufferedImage对象：
现在，调用getRaster方法来获得一个类型为WritableRaster的对象，后面将使用这个对象来访问和修改该图像的各个像素：
使用setPixel方法可以设置一个单独的像素。这项操作的复杂性在于不能只是为该像素设置一个Color值，还必须知道存放在缓冲中的图像是如何设定颜色的，这依赖于图像的类型。如果图像有一个TYPE_INT_ARGB类型，那么每一个像素都用四个值来描述，即：红、绿、蓝和透明度（alpha），每个值的取值范围都介于0和255之间，这需要以包含四个整数值的一个数组的形式给出：
用Java 2D API的行话来说，这些值被称为像素的样本值。
警告：还有一些参数值是float[]和double[]类型的setPixel方法。然而，需要在这些数组中放置的值并不是介于0.0和1.0之间的规格化的颜色值：
无论数组属于什么类型，都必须提供介于0和255之间的某个值。
可以使用setPixels方法提供批量的像素。需要设置矩形的起始像素的位置和矩形的宽度和高度。接着，提供一个包含所有像素的样本值的一个数组。例如，如果你缓冲的图像有一个TYPE_INT_ARGB类型，那么就应该提供第一个像素的红、绿、蓝和透明度的值（alpha），然后，提供第二个像素的红、绿、蓝和透明度的值，以此类推：
反过来，如果要读入一个像素，可以使用getPixel方法。这需要提供一个含有四个整数的数组，用以存放各个样本值：
可以使用getPixels方法来读取多个像素：
如果使用的图像类型不是TYPE_INT_ARGB，并且已知该类型是如何表示像素值的，那么仍旧可以使用getPixel/setPixel方法。不过，必须要知道该特定图像类型的样本值是如何进行编码的。
如果需要对任意未知类型的图像进行处理，那么你就要费神了。每一个图像类型都有一个颜色模型，它能够在样本值数组和标准的RGB颜色模型之间进行转换。
注意：RGB颜色模型并不像你想象中的那么标准。颜色值的确切样子依赖于成像设备的特性。数码相机、扫描仪、控制器和LCD显示器等都有它们独有的特性。结果是，同样的RGB值在不同的设备上看上去就存在很大的差别。国际配色联盟（http://www.color.org）推荐，所有的颜色数据都应该配有一个ICC配置特性，它用以设定各种颜色是如何映射到标准格式的，比如1931 CIE XYZ颜色技术规范。该规范是由国际照明委员会即CIE（Commission Internationale de I’Eclairage，其网址为：http://www.cie.co.at/cie）制定的。该委员会是负责提供涉及照明和颜色等相关领域事务的技术指导的国际性机构。该规范是显示肉眼能够察觉到的所有颜色的一个标准化方法。它采用称为X、Y、Z三元组坐标的方式来显示颜色。（关于1931 CIE XYZ规范的详尽信息，可以参阅Foley、van Dam和Feiner等人所撰写的《Computer Graphics：Principles and Practice》一书的第13章。）
ICC配置特性非常复杂。然而，我们建议使用一个相对简单的标准，称为sRGB（请访问其网址http://www.w3.org/Graphics/Color/sRGB.html）。它设定了RGB值与1931 CIE XYZ值之间的具体转换方法，它可以非常出色地在通用的彩色监视器上应用。当需要在RGB与其他颜色空间之间进行转换的时候，Java 2D API就使用这种转换方式。
getColorModel方法返回一个颜色模型：
为了了解一个像素的颜色值，可以调用Raster类的getDataElements方法。这个方法返回了一个Object，它包含了有关该颜色值的与特定颜色模型相关的描述：
注意：getDataElements方法返回的对象实际上是一个样本值的数组。在处理这个对象时，不必要了解到这些。但是，它却解释了为什么这个方法名叫做getDataElements的原因。
颜色模型能够将该对象转换成标准的ARGB的值。getRGB方法返回一个int类型的值，它把透明度（alpha）、红、绿和蓝的值打包成四个块，每块包含8位。也可以使用Color（int argb，boolean hasAlpha）构造器来构建一个颜色的值：
如果要把一个像素设置为某个特定的颜色值，需要按与上述相反的步骤进行操作。Color类的getRGB方法会产生一个包含透明度、红、绿和蓝值的int型值。把这个值提供给ColorModel类的getDataElements方法，其返回值是一个包含了该颜色值的特定颜色模型描述的Object。再将这个对象传递给WritableRaster类的setDataElements方法：
为了阐明如何使用这些方法来用各个像素构建图像，我们按照传统，绘制了一个Mandelbrot集，如图7-28所示。
图7-28　Mandelbrot集
Mandelbrot集的思想就是把平面上的每一点和一个数字序列关联在一起。如果数字序列是收敛的，该点就被着色。如果数字序列是发散的，该点就处于透明状态。
下面就是构建简单Manderbrot集的方法。对于每一个点（a，b），你都能按照如下的公式得到一个点集序列，其开始于点（x，y）=（0，0），反复进行迭代：
结果证明，如果x或者y的值大于2，那么序列就是发散的。仅有那些与导致数字序列收敛的点（a，b）相对应的像素才会被着色。（该数字序列的计算公式基本上是从复杂的数学概念中推导出来的。我们只使用现成的公式，如果要了解更多的分形数学概念的详细说明，请查看http://classes.yale.edu/fractals/）
程序清单7-8显示了该代码。在此程序中，我们展示了如何使用ColorModel类将Color值转换成像素数据。这个过程和图像的类型是不相关的。为了增加些趣味，你可以把缓冲图像的颜色类型改变为TYPE_BYTE_GRAY。不必改变程序中的任何代码，该图像的颜色模型会自动地负责把颜色转换为样本值。
程序清单7-8　rasterImage/RasterImageFrame.java
java.awt.image.BufferedImage 1.2
·BufferedImage（int width，int height，int imageType）
构建一个被缓存的图像对象。
参数：width，height　图像的尺寸
imageType　图像的类型，最常用的类型是TYPE_INT_RGB、TYPE_INT_ARGB、TYPE_BYTE_GRAY和TYPE_BYTE_INDEXED
·ColorModel getColorModel（）
返回被缓存图像的颜色模型。
·WritableRaster getRaster（）
获得访问和修改该缓存图像像素的光栅。
java.awt.image.Raster 1.2
·Object getDataElements（int x，int y，Object data）
返回某个光栅点的样本数据，该数据位于一个数组中，而该数组的长度和类型依赖于颜色模型。如果data不为null，那么它将被视为是适合于存放样本数据的数组，从而被充填。如果data为null，那么将分配一个新的数组，其元素的类型和长度依赖于颜色模型。
·int[]getPixel（int x，int y，int[]sampleValues）
·float[]getPixel（int x，int y，float[]sampleValues）
·double[]getPixel（int x，int y，double[]sampleValues）
·int[]getPixels（int x，int y，int width，int heigth，int[]sampleValues）
·float[]getPixels（int x，int y，int width，int height，float[]sampleValues）
·double[]getPixels（int x，int y，int width，int height，double[]sampleValues）
返回某个光栅点或者是由光栅点组成的某个矩形的样本值，该数据位于一个数组中，数组的长度依赖于颜色模型。如果sampleValues不为null，那么该数组被视为长度足够存放样本值，从而该数组被填充。如果sampleValues为null，就要分配一个新数组。仅当你知道某一颜色模型的样本值的具体含义的时候，这些方法才会有用。
java.awt.image.WritableRaster 1.2
·void setDataElements（int x，int y，Object data）
设置光栅点的样本数据。data是一个已经填入了某一像素样本值的数组。数组元素的类型和长度依赖于颜色模型。
·void setPixel（int x，int y，int[]sampleValues）
·void setPixel（int x，int y，float[]sampleValues）
·void setPixel（int x，int y，double[]sampleValues）
·void setPixels（int x，int y，int width，int height，int[]sampleValues）
·void setPixels（int x，int y，int width，int height，float[]sampleValues）
·void setPixels（int x，int y，int width，int height，double[]sampleValues）
设置某个光栅点或由多个光栅点组成的矩形的样本值。只有当你知道颜色模型样本值的编码规则时，这些方法才会有用。
java.awt.image.ColorModel 1.2
·int getRGB（Object data）
返回对应于data数组中传递的样本数据的ARGB值。其元素的类型和长度依赖于颜色模型。
·Object getDataElements（int argb，Object data）；
返回某个颜色值的样本数据。如果data不为null，那么该数组被视为非常适合于存放样本值，进而该数组被填充。如果data为null，那么将分配一个新的数组。data是一个填充了用于某个像素的样本数据的数组，其元素的类型和长度依赖于该颜色模型。
java.awt.Color 1.0
·Color（int argb，boolean hasAlpha）1.2
如果hasAlpha的值是true，则用指定的ARGB组合值创建一种颜色。如果hasAlpha的值是false，则用指定的RGB值创建一种颜色。
·int getRGB（）
返回和该颜色相对应的ARGB颜色值。7.11.2　图像过滤
在前面的章节中，我们介绍了从头开始构建图像的方法。然而，你常常是因为另一个原因去访问图像数据的：你已经拥有了一个图像，并且想从某些方面对图像进行改进。
当然，可以使用前一节中的getPixel/getDataElements方法来读取和处理图像数据，然后把图像数据写回到文件中。不过，幸运的是，Java 2D API已经提供了许多过滤器，它们能够执行常用的图像处理操作。
图像处理都实现了BufferedImageOp接口。构建了图像处理的操作之后，只需调用filter方法，就可以把该图像转换成另一个图像。
有些图像操作可以恰当地（通过op.filter（image，image）方法）转换一个图像，但是大多数的图像操作都做不到这一点。
以下五个类实现了BufferedImageOp接口。
AffineTransformOp类用于对各个像素执行仿射变换。例如，下面的代码就说明了如何使一个图像围绕着它的中心旋转。
AffineTransformOp构造器需要一个仿射变换和一个渐变变换策略。如果源像素在目标像素之间的某处会发生变换的话，那么就必须使用渐变变换策略来确定目标图像的像素。例如，如果旋转源像素，那么通常它们不会精确地落在目标像素上。有两种渐变变换策略：AffineTransformOp.TYPE_BILINEAR和AffineTransformOp.TYPE_NEAREST_NEIGHBOR。双线性（Bilinear）渐变变换需要的时间较长，但是变换的效果却更好。
使用程序清单7-9的程序，可以把一个图像旋转5度（参见图7-29）。
图7-29　一个旋转50°的图像
RescaleOp用于为图像中的所有的颜色构件执行一个调整其大小的变换操作（透明度构件不受影响）：
用a>1进行调整，那么调整后的效果是使图像变亮。可以通过设定调整大小的参数和可选的绘图提示来构建RescaleOp。在程序清单7-9中，我们使用下面的设置：
也可以为每个颜色构件提供单独的缩放值，参见API说明。
使用LookupOp操作，可以为样本值设定任意的映射操作。你提供一张表格，用于设定每一个样本值应该如何进行映射操作。在示例程序中，我们计算了所有颜色的反，即将颜色c变成255-c。
LookupOp构造器需要一个类型是LookupTable的对象和一个选项提示映射表。LookupTable是抽象类，其有两个实体子类：ByteLookupTable和ShortLookupTable。因为RGB颜色值是由字节组成的，所以ByteLookupTable类应该就够用了。但是，考虑到在http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6183251中描述的缺陷，我们将使用ShortLookupTable。下面的代码说明了我们在程序清单中是如何构建一个LookupOp类的：
此项操作可以分别应用于每个颜色构件，但是不能应用于透明度值。也可以为每个颜色构件提供单独的查找表，参见API说明。
注意：不能将LookupOp用于带有索引颜色模型的图像。（在这些图像中，每个样本值都是调色板中的一个偏移量。）
ColorConvertOp对于颜色空间的转换非常有用。我们不准备在这里讨论这个问题了。
ConvolveOp是功能最强大的转换操作，它用于执行卷积变换。我们不想过分深入地介绍卷积变换的详尽细节。不过，其基本概念还是比较简单的。我们不妨看一下模糊过滤器的例子（见图7-30）。
这种模糊的效果是通过用像素和该像素临近的8个像素的平均值来取代每一个像素值而达到的。凭借直观感觉，就可以知道为什么这种变换操作能使得图像变模糊了。从数学理论上来说，这种平均法可以表示为一个以下面这个矩阵为内核的卷积变换操作：
卷积变换操作的内核是一个矩阵，用以说明在临近的像素点上应用的加权值。应用上面的内核进行卷积变换，就会产生一个模糊图像。下面这个不同的内核用以进行图像的边缘检测，查找图像颜色变化的区域：
边缘检测是在分析摄影图片时使用的一项非常重要的技术（参见图7-31）。
图7-30　对图像进行模糊处理
图7-31　边缘检测
如果要构建一个卷积变换操作，首先应为矩阵内核建立一个含有内核值的数组，并且构建一个Kernel对象。接着，根据内核对象建立一个ConvolveOp对象，进而执行过滤操作。
使用程序清单7-9的程序，用户可以装载一个GIF或者JPEG图像，并且执行我们已经介绍过的各种图像处理的操作。由于Java 2D API的图像处理的功能很强大，下面的程序非常简单。
程序清单7-9　imageProcessing/ImageProcessingFrame.java
java.awt.image.BufferedImageOp 1.2
·BufferedImage filter（BufferedImage source，BufferedImage dest）
将图像操作应用于源图像，并且将操作的结果存放在目标图像中。如果dest为null，一个新的目标图像将被创建。该目标图像将被返回。
java.awt.image.AffineTransformOp 1.2
·AffineTransformOp（AffineTransform t，int interpolationType）
构建一个仿射变换操作符。渐变变换的类型是TYPE_BILINEAR、TYPE_BICUBIC或者TYPE_NEAREST_NEIGHBOR中的一个。
java.awt.image.RescaleOp 1.2
·RescaleOp（float a，float b，RenderingHints hints）
·RescaleOp（float[]as，float[]bs，RenderingHints hints）
构建一个尺寸调整的操作符，它会执行缩放操作xnew=a·x+b。当使用第一个构造器时，所有的颜色构件（但不包括透明度构件）都将按照相同的系数进行缩放。当使用第二个构造器时，可以为每个颜色构件提供单独的值，在这种情况下，透明度构件不受影响，或者为每个颜色构件和透明度构件都提供单独的值。
java.awt.image.LookupOp 1.2
·LookupOp（LookupTable table，RenderingHints hints）
为给定的查找表构建一个查找操作符。
java.awt.image.ByteLookupTable 1.2
·ByteLookupTable（int offset，byte[]data）
·ByteLookupTable（int offset，byte[][]data）
为转化byte值构建一个字节查找表。在查找之前，从输入中减去偏移量。在第一个构造器中的值将提供给所有的颜色构件，但不包括透明度构件。当使用第二个构造器时，可以为每个颜色构件提供单独的值，在这种情况下，透明度构件不受影响，或者为每个颜色构件和透明度构件都提供单独的值。
java.awt.image.ShortLookupTable 1.2
·ShortLookupTable（int offset，short[]data）
·ShortLookupTable（int offset，short[][]data）
为转化short值构建一个字节查找表。在查找之前，从输入中减去偏移量。在第一个构造器中的值将提供给所有的颜色构件，但不包括透明度构件。当使用第二个构造器时，可以为每个颜色构件提供单独的值，在这种情况下，透明度构件不受影响，或者为每个颜色构件和透明度构件都提供单独的值。
java.awt.image.ConvolveOp 1.2
·ConvolveOp（Kernel kernel）
·ConvolveOp（Kernel kernel，int edgeCondition，RenderingHints hints）
构建一个卷积变换操作符。边界条件是EDGE_NO_OP和EDGE_ZERO_FILL两种方式之一。由于边界值没有足够的临近值来进行卷积变换的计算，所以边界值必须被特殊处理，其默认值是EDGE_ZERO_FILL。
java.awt.image.Kernel 1.2
·Kernel（int width，int height，float[]matrixElements）
为指定的矩阵构建一个内核。7.12　打印
最初的JDK根本不支持打印操作。它不可能从applets中进行打印操作，如果想在应用中使用打印操作，必须获得第三方的类库。JDK1.1提供了非常轻量级的打印支持，仅仅能够产生简单的打印输出，不过只要你对打印的质量没有太高的要求也就够用了。JDK 1.1的打印模型被设计为允许浏览器供应商打印出现在网页中的applet外观（然而，浏览器供应商对此并不感兴趣）。
Java SE 1.2开始推出了一种强大的打印模型，它和Java 2D图形实现了完全的集成。JDK1.4增加了许多重要的增强特性，比如，查找打印机的特性和用于服务器端打印管理的流式打印作业等。
在本节中，我们将介绍如何在单页纸上轻松地打印出一幅图画，如何来管理多页打印输出，还有如何利用Java 2D图像模型的出色特性，以及如何方便地产生一个打印预览对话框。7.12.1　图形打印
在本节中，我们将处理最常用的打印情景，即打印一个2D图形，当然该图形可以含有不同字体组成的文本，甚至可能完全由文本构成。
如果要生成打印输出，必须完成下面这两个任务：
·提供一个实现了Printable接口的对象。
·启动一个打印作业。
Printable接口只有下面一个方法：
每当打印引擎需要对某一页面进行排版以便打印时，都要调用这个方法。你的代码绘制了准备在图形上下文上打印的文本和图像，页面排版显示了纸张的大小和页边距，页号显示了将要打印的页。
如果要启动一个打印作业，需要使用PrinterJob类。首先，应该调用静态方法getPrinterJob来获取一个打印作业对象。然后，设置要打印的Printable对象。
警告：PrintJob这个类处理的是JDK1.1风格的打印操作，这个类已经被弃用了。请不要把PrinterJob类同其混淆在一起。
在开始打印作业之前，应该调用printDialog方法来显示一个打印对话框（见图7-32）。这个对话框为用户提供了机会去选择要使用的打印机（在多打印机可被利用的情况下），选择将要打印的页的范围，以及选择打印机的各种设置。
图7-32　一个跨平台的打印对话框
可以在一个实现了PrintRequestAttributeSet接口的类的对象中收集到各种打印机的设置，例如HashPrintRequestAttributeSet类：
你可以添加属性设置，并且把attributes对象传递给printDialog方法。
如果用户点击OK，那么printDialog方法将返回true；如果用户关掉对话框，那么该方法将返回false。如果用户接受了设置，那么就可以调用PrinterJob类的print方法来启动打印进程。print方法可能会抛出一个PrinterException异常。下面是打印代码的基本框架：
注意：在JDK1.4之前，打印系统使用的都是宿主平台本地的打印和页面设置对话框。要展示本地打印对话框，可以调用没有任何参数的printDialog方法。（不存在任何方式可以用来将用户的设置收集到一个属性集中。）
在执行打印操作时，PrinterJob类的print方法不断地调用和此项打印作业相关的Printable对象的print方法。
由于打印作业不知道用户想要打印的页数，所以它只是不断地调用print方法。只要该print方法的返回值是Printable.PAGE_EXISTS，打印作业就不断地产生输出页。当print方法返回Pringtable.NO_SUCH_PAGE时，打印作业就停止。
警告：打印作业传递到print方法的打印页号是从0开始的。
因此，在打印操作完成之前，打印作业并不知道准确的打印页数。为此，打印对话框无法显示正确的页码范围，而只能显示“Pages 1 to 1”（从第一页到第一页）。在下一节中，我们将介绍如何通过为打印作业提供一个Book对象来避免这个缺陷。
在打印的过程中，打印作业反复地调用Printable对象的print方法。打印作业可以对同一页面多次调用print方法，因此不应该在print方法内对页进行计数，而是应始终依赖于页码参数来进行计数操作。打印作业之所以能够对某一页反复地调用print方法是有一定道理的：一些打印机，尤其是点阵式打印机和喷墨式打印机，都使用条带打印技术，它们在打印纸上一条接着一条地打印。即使是每次打印一整页的激光打印机，打印作业都有可能使用条带打印技术。这为打印作业提供了一种对假脱机文件的大小进行管理的方法。
如果打印作业需要printable对象打印一个条带，那么它可以将图形上下文的剪切区域设置为所需要的条带，并且调用print方法。它的绘图操作将按照条带矩形区域进行剪切，同时，只有在条带中显示的那些图形元素才会被绘制出来。你的print方法不必晓得该过程，但是请注意：它不应该对剪切区域产生任何干扰。
警告：你的print方法获得的Graphics对象也是按照页边距进行剪切的。如果替换了剪切区域，那么就可以在边距外面进行绘图操作。尤其是在打印机的绘图上下文中，剪切区域是被严格遵守的。如果想进一步地限制剪切区域，可以调用clip方法，而不是setClip方法。如果必须要移除一个剪切区域，那么请务必在你的print方法开始处调用getClip方法，并还原该剪切区域。
print方法的PageFormat参数包含有关被打印页的信息。getWidth方法和getHeight方法返回该纸张的大小，它以磅为计量单位。1磅等于1/72英吋[1]。例如，A4纸的大小大约是595×842磅，美国人使用的信纸大小为612×792磅。
磅是美国印刷业中通用的计量单位，让世界上其他地方的人感到苦恼的是，打印软件包使用的是磅这种计量单位。使用磅有两个原因，即纸张的大小和纸张的页边距都是用磅来计量的。对所有的图形上下文来说，默认的计量单位就是1磅。你可以在本节后面的示例程序中证明这一点。该程序打印了两行文本，这两行文本之间的距离为72磅。运行一下示例程序，并且测量一下基准线之间的距离。它们之间的距离恰好是1英吋或是25.4毫米。
PageFormat类的getWidth和getHeight方法给你的信息是完整的页面大小，但并不是所有的纸张区域都会被用来打印。通常的情况是，用户会选择页边距，即使他们没有选择页边距，打印机也需要用某种方法来夹住纸张，因此在纸张的周围就出现了一个不能打印的区域。
getImageableWidth和getImageableHeight方法可以告诉你能够真正用来打印的区域的大小。然而，页边距没有必要是对称的，所以还必须知道可打印区域的左上角，见图7-33，它们可以通过调用getImageableX和getImageableY方法来获得。
提示：在print方法中接收到的图形上下文是经过剪切后的图形上下文，它不包括页边距。但是，坐标系统的原点仍然是纸张的左上角。应该将该坐标系统转换成可打印区域的左上角，并以其为起点。这只需让print方法以下面的代码开始即可：
如果想让用户来设定页边距，或者让用户在纵向和横向打印方式之间切换，同时并不涉及设置其他打印属性，那么就应该调用PrinterJob类的pageDialog方法。
注意：打印对话框中有一个选项卡包含了页面设置对话框（参见图7-34）。在打印前，你仍然可以为用户提供选项来设置页面格式。特别是，如果你的程序给出了一个待打印页面的“所见即所得”的显示屏幕，那么就更应该提供这样的选项。pageDialog方法返回了一个含有用户设置的PageFormat对象。
图7-33　页面格式计量
图7-34　一个跨平台的页面设置对话框
程序清单7-10和7-11显示了如何在屏幕和打印页面上绘制相同的一组形状的方法。Jpanel类的一个子类实现了Printable接口，该类中的paintComponent和print方法都调用了相同的方法来执行实际的绘图操作。
该示例代码显示并且打印了图7-20，即被用作线条模式的剪切区域的消息“Hello，World”的边框。
可以点击Print按钮来启动打印，或者点击页面设置按钮来打开页面设置对话框。程序清单7-10显示了它的代码。
注意：为了显示本地页面设置对话框，需要将默认的PageFormat对象传递给pageDialog方法。该方法会克隆这个对象，并根据用户在对话框中的选择来修改它，然后返回这个克隆的对象。
程序清单7-10　print/PrintTestFrame.java
程序清单7-11　print/PrintComponent.java
java.awt.print.Printable 1.2
·int print（Graphics g，PageFormat format，int pageNumber）
绘制一个页面，并且返回PAGE_EXISTS，或者返回NO_SUCH_PAGE。
参数：g　在上面绘制页面的图形上下文
format　要绘制的页面的格式
pageNumber　所请求页面的页码
java.awt.print.PrinterJob 1.2
·static PrinterJob getPrinterJob（）
返回一个打印机作业对象。
·PageFormat defaultPage（）
为该打印机返回默认的页面格式。
·boolean printDialog（PrintRequestAttributeSet attributes）
·boolean printDialog（）
打开打印对话框，允许用户选择将要打印的页面，并且改变打印设置。第一个方法将显示一个跨平台的打印对话框，第二个方法将显示一个本地的打印对话框。第一个方法修改了attributes对象来反映用户的设置。如果用户接受默认的设置，两种方法都返回true。
·PageFormat pageDialog（PrintRequestAttributeSet attributes）
·PageFormat pageDialog（PageFormat defaults）
显示页面设置对话框。第一个方法将显示一个跨平台的对话框，第二个方法将显示一个本地的页面设置对话框。两种方法都返回了一个PageFormat对象，对象的格式是用户在对话框中所请求的格式。第一个方法修改了attributes对象以反映用户的设置。第二个对象不修改defaults对象。
·void setPrintable（Printable p）
·void setPrintable（Printable p，PageFormat format）
设置该打印作业的Printable和可选的页面格式。
·void print（）
·void print（PrintRequestAttributeSet attributes）
反复地调用print方法，以打印当前的Printable，并将绘制的页面发送给打印机，直到没有更多的页面需要打印为止。
java.awt.print.PageFormat 1.2
·double getWidth（）
·double getHeight（）
返回页面的宽度和高度。
·double getImageableWidth（）
·double getImageableHeight（）
返回可打印区域的页面宽度和高度。
·double getImageableX（）
·double getImageableY（）
返回可打印区域的左上角的位置。
·int getOrientation（）
返回PORTARIT、LANDSCAPE和REVERSE_LANDSCAPE三者之一。页面打印的方向对程序员来说是透明的，因为打印格式和图形上下文自动地反映了页面的打印方向。
[1]1英吋=0.0254米。7.12.2　打印多页文件
在实际的打印操作中，通常不应该将原生的Printable对象传递给打印作业。相反，应该获取一个实现了Pageable接口的类的对象。Java平台提供了这样的一个被称为Book的类。一本书是由很多章节组成的，而每个章节都是一个Printable对象。可以通过添加Printable对象和相应的页数来构建一个Book对象。
然后，可以使用setPageable方法把Book对象传递给打印作业。
现在，打印作业就知道将要打印的确切页数了。然后，打印对话框显示一个准确的页面范围，用户可以选择整个页面范围或可选择它的一个子范围。
警告：当打印作业调用Printable章节的print方法时，它传递的是该书的当前页码，而不是每个章节的页码。这让人非常痛苦，因为每个章节必须知道它之前所有章节的页数，这样才能使得页码参数有意义。
从程序员的视角来看，使用Book类最大的挑战就是，当你打印它时，必须知道每一个章节究竟有多少页。你的Printable类需要一个布局算法，以便用来计算在打印页面上的素材布局。在打印开始前，要调用这个算法来计算出分页符的位置和页数。可以保留布局信息，从而可以在打印的过程中方便地使用它。
必须警惕“用户已经修改过页面格式”这种情况的发生。如果用户修改了页面格式，即使是所打印的信息没有发生任何改变，也必须要重新计算布局。
程序清单7-13中显示了如何产生一个多页打印输出。该程序用很大的字符在多个页面上打印了一条消息（见图7-35）。然后，可以剪裁掉页边缘，并将这些页面粘连起来，形成一个标语。
图7-35　一幅标语
Banner类的layoutPages方法用以计算页面的布局。我们首先展示了一个字体为72磅的消息字符串。然后，我们计算产生的字符串的高度，并且将其与该页面的可打印高度进行比较。我们根据这两个高度值得出一个比例因子，当打印该字符串时，我们按照比例因子来放大此字符串。
警告：如果要准确地布局打印信息，通常需要访问打印机的图形上下文。遗憾的是，只有当打印真正开始时，才能获得打印机的图形上下文。在我们的示例程序中使用的是屏幕的图形上下文，并且希望屏幕的字体度量单位与打印机的相匹配。
Banner类的getPageCount方法首先调用布局方法。然后，扩展字符串的宽度，并且将该宽度除以每一页的可打印宽度。得到的商向上取整，就是要打印的页数。
由于字符可以断开分布到多个页面上，所以上面打印标语的操作好像会有困难。然而，感谢Java 2D API提供的强大功能，这个问题现在不过是小菜一碟。当需要打印某一页时，我们只需要调用Graphics2D类的translate方法，将字符串的左上角向左平移。接着，设置一个大小是当前页面的剪切矩形（参见图7-36）。最后，我们用布局方法计算出的比例因子来扩展该图形上下文。
图7-36　打印一个标语页面
这个例子显示了图形变换操作的强大功能。绘图代码很简单，而图形变换操作负责执行将图形放到恰当位置上的所有操作。最后，剪切操作负责将落在页面外面的图像剪切掉。在下一节中，你将看到另一种必须使用变换操作的情况，即显示页面的打印预览。7.12.3　打印预览
大多数专业的程序都有一个打印预览机制，使用户能够在显示屏幕上看到要打印的页面，这样就不必为不满意的打印输出而浪费纸张了。Java平台的打印类并没有提供一个标准的“打印预览”对话框，但是可以非常容易地设计出自己的打印预览对话框（参见图7-37）。在本节中，我们将要介绍如何来设计自己的打印预览对话框。程序清单7-14中的PrintPreviewDialog类是一个完全泛化的类，你可以复用它来预览任何种类的打印输出。
图7-37　打印预览对话框
如果要构建一个PrintPreviewDialog类，必须提供一个Printable或Book，并且还要提供一个PageFormat对象。对话框包含一个PrintPreviewCanvas（参见程序清单7-15）。当使用Next和Previous按钮来浏览页面时，paintComponent方法将为你所请求预览的页面调用Printable对象的print方法。
通常，print方法在一个打印机的图形上下文上绘制页面上下文。但是，我们提供了屏幕的图形上下文，并进行了合适的缩放，这样，打印的整个页面就可以被纳入到较小的屏幕矩形中。
该print方法从来都不知道它实际上并不产生打印页面。它只是负责在图形上下文上进行绘制操作，从而在屏幕上产生一个微观的打印预览。这非常清楚地说明了Java 2D图像模型的强大功能。
程序清单7-12中包括了打印标语的程序代码。请将“Hello，World！”输入到文本框中，并且观察打印预览，然后把标语打印输出。
程序清单7-12　book/BookTestFrame.java
程序清单7-13　book/Banner.java
程序清单7-14　book/PrintPreviewDialog.java
程序清单7-15　book/PrintPreviewCanvas.java
java.awt.print.PrinterJob 1.2
·void setPageable（Pageable p）
设置一个要打印的Pageable（比如，一个Book）。
java.awt.print.Book 1.2
·void append（Printable p，PageFormat format）
·void append（Printable p，PageFormat format，int pageCount）
为该书添加一个章节。如果页数未指定，那么就添加第一页。
·Printable getPrintable（int page）
获取指定页面的可打印特性。7.12.4　打印服务程序
到目前为止，我们已经介绍了如何打印2D图形。然而，Java SE1.4中的打印API提供了更大的灵活性。该API定义了大量的数据类型，并且可以让你找到能够打印这些数据类型的打印服务程序。这些类型有：
·GIF、JPEG或者PNG格式的图像。
·纯文本、HTML、PostScript或者PDF格式的文档。
·原始的打印机代码数据。
·实现了Printable、Pageable或RenderableImage的某个类的对象。
数据本身可以存放在一个字节源或字符源中，比如一个输入流、一个URL或者一个数组中。文档风格（document flavor）描述了一个数据源和一个数据类型的组合。DocFlavor类为不同的数据源定义了许多内部类，每一个内部类都定义了指定风格的常量。例如，常量
描述了从输入流中读入一个GIF格式的图像。表7-3中列出了数据源和数据类型的各种组合。
假设我们想打印一个位于文件中的GIF格式的图像。首先，确认是否有能够处理该打印任务的打印服务程序。PrintServiceLookup类的静态lookupPrintServices方法返回一个能够处理给定文档风格的PrintService对象的数组。
表7-3　打印服务的文档风格
lookupPrintServices方法的第二个参数值为null，表示我们不想通过设定打印机属性来限制对文档的搜索。我们在下一节中介绍打印机的属性。
如果对打印服务程序的查找返回的数组带有多个元素的话，那就需要从打印服务程序列表中选择所需的打印服务程序。通过调用PrintService类的getName方法，可以获得打印机的名称，然后让用户进行选择。
接着，从该打印服务获取一个文档打印作业：
如果要执行打印操作，需要一个实现了Doc接口的类的对象。Java为此提供了一个SimpleDoc类。SimpleDoc类的构造器必须包含数据源对象、文档风格和一个可选的属性集。例如，
最后，就可以执行打印输出了。
与前面一样，null参数可以被一个属性集取代。
请注意，这个打印进程和上一节的打印进程之间有很大的差异。这里不需要用户通过打印对话框来进行交互式地操作。例如，可以实现一个服务器端的打印机制，这样，用户就可以通过Web表单提交打印作业了。
程序清单7-16中的程序展示了如何使用打印服务程序来打印一个图像文件。
程序清单7-16　printService/PrintServiceTest.java
javax.print.PrintServiceLookup 1.4
·PrintService[]lookupPrintServices（DocFlavor flavor，AttributeSet attributes）
查找能够处理给定文档风格和属性的打印服务程序。
参数：flavor　文档风格
attributes　需要的打印属性，如果不考虑打印属性的话，其值应该为null
javax.print.PrintService 1.4
·DocPrintJob createPrintJob（）
为了打印实现了Doc接口（如SimpleDoc）的对象而创建一个打印作业。
javax.print.DocPrintJob 1.4
·void print（Doc doc，PrintRequestAttributeSet attributes）
打印带有给定属性的给定文档。
参数：doc　要打印的Doc
attributes　需要的打印属性，如果不需要任何打印属性的话，其值为null
javax.print.SimpleDoc 1.4
·SimpleDoc（Object data，DocFlavor flavor，DocAttributeSet attributes）
构建一个能够用DocPrintJob打印的SimpleDoc对象。
参数：data　带有打印数据的对象，比如一个输入流或者一个Printable
flavor　打印数据的文档风格
attributes　文档属性，如果不需要文档属性，其值为null7.12.5　流打印服务程序
打印服务程序将打印数据发送给打印机。流打印服务程序产生同样的打印数据，但是并不把数据发送给打印机，而是发给流。这么做的目的也许是为了延迟打印或者因为打印数据格式可以由其他程序来进行解释。尤其是，如果打印数据格式是PostScript时，那么可将打印数据保存到一个文件中，因为有许多程序都能够处理PostScript文件。Java平台引入了一个流打印服务程序，它能够从图像和2D图形中产生PostScript输出。可以在任何系统中使用这种服务程序，即使这些系统中没有本地打印机，也可以使用该服务程序。
枚举流打印服务程序要比定位普通的打印服务程序复杂一些。既需要打印对象的DocFlavor又需要流输出的MIME类型，接着获得一个StreamPrintServiceFactory类型的数组，如下所示：
StreamPrintServiceFactory类没有任何方法能够帮助我们区分不同的factory，所以我们只提取factories[0]。我们调用带有输出流参数的getPrintService方法来获得一个StreamPrintService对象。
StreamPrintService类是PrintService的子类。如果要产生一个打印输出，只要按照上一节介绍的步骤进行操作即可。
javax.print.StreamPrintServiceFactory 1.4
·StreamPrintServiceFactory[]lookupStreamPrintServiceFactories（DocFlavor flavor，String mimeType）
查找所需的流打印服务程序工厂，它能够打印给定文档风格，并且产生一个给定MIME类型的输出流。
·StreamPrintService getPrintService（OutputStream out）
获得一个打印服务程序，以便将打印输出发送到指定的输出流中。7.12.6　打印属性
打印服务程序API包含了一组复杂的接口和类，用以设定不同种类的属性。重要的属性共有四组，前两组属性用于设定对打印机的访问请求。
·打印请求属性（Print request attributes）为一个打印作业中的所有doc对象请求特定的打印属性，例如，双面打印或者纸张的大小。
·Doc属性（Doc attributes）是仅作用在单个doc对象上的请求属性。
另外两组属性包含关于打印机和作业状态的信息。
·打印服务属性（Print service attributes）提供了关于打印服务程序的信息，比如打印机的种类和型号，或者打印机当前是否接受打印作业。
·打印作业属性（Print job attributes）提供了关于某个特定打印作业状态的信息，比如该打印作业是否已经完成。
如果要描述各种不同的打印属性，可以使用带有如下子接口的Attribute接口。
各个属性类都实现了上面的一个或几个接口。例如，Copies类的对象描述了一个打印输出的拷贝数量，该类就实现了PrintRequestAttribute和PrintJobAttribute两个接口。显然，一个打印请求可以包含一个对多个拷贝的请求。反过来，打印作业的某个属性可能表示的是实际上打印出来的拷贝数量。这个拷贝数量可能很小，也许是因为打印机的限制或者是因为打印机的纸张已经用完了。
SupportedValuesAttribute接口表示某个属性值反映的不是实际的打印请求或状态数据，而是某个服务程序的能力。例如，实现了SupportedValuesAttribute接口的CopiesSupported类，该类的对象可以用来描述某个打印机能够支持1~99份拷贝的打印输出。
图7-38显示了属性分层结构的类图。
图7-38　显示了一个属性层次结构的类图
除了为各个属性定义的接口和类以外，打印服务程序API还为属性集定义了接口和类。父接口AttributeSet有四个子接口：
对于每个这样的接口，都有一个实现类，因此会产生下面5个类：
图7-39显示了属性集分层结构的类图。
图7-39　属性集的分层结构
例如，可以用如下的方式构建一个打印请求属性集。
当构建完属性集后，就不用担心使用Hash前缀的问题了。
为什么要配有所有这些接口呢？因为，它们使得“检查属性是否被正确使用”成为了可能。例如，DocAttributeSet只接受实现了DocAttribute接口的对象，添加其他属性的任何尝试都会导致运行期错误的产生。
属性集是一个特殊的映射表，其键是Class类型的，而值是一个实现了Attribute接口的类。例如，如果要插入一个对象
到属性集中，那么它的键就是Class对象Copies.class。该键被称为属性的类别。Attribute接口声明了下面这样一个方法：
该方法就可以返回属性的类别。Copies类定义了用以返回Copies.class对象的方法。但是，属性的类别和属性的类没有必要是相同的。
当将一个属性添加到属性集中时，属性的类别就会被自动地获取。你只需添加该属性的值：
如果后来添加了一个具有相同类别的另一个属性，那么新属性就会覆盖第一个属性。如果要检索一个属性，需要使用它的类别作为键，例如，
最后，属性是按照它们拥有的值来进行组织的。Copies属性能够拥有任何整数值。Copies类继承了IntegerSyntax类，该类负责处理所有带有整数值的属性。getValue方法将返回属性的整数值，例如，
下面这些类：
用于封装一个字符串、日期与时间，或者URI（通用资源标识符）。
最后要说明的是，许多属性都能够接受数量有限的值。例如，PrintQuality属性有三个设置值：draft（草稿质量），normal（正常质量）和high（高质量），它们用三个常量来表示：
拥有有限数量值的属性类继承了EnumSyntax类，该类提供了许多便利的方法，用来以类型安全的方式设置这些枚举。当使用这样的属性时，不必担心该机制，只需要将带有名字的值添加给属性集即可：
下面的代码说明了如何来检查一个属性的值：
表7-4列出了各个打印属性。表中的第二列列出了属性类的超类（例如，Copies属性的IntegerSyntax类）或者是具有一组有限值属性的枚举值。最后四列表示该属性是否实现了DocAttribute（DA）、PrintJobAttribute（PJA）、PrintRequestAttribute（PRA）和PrintServiceAttribute（PSA）几个接口。
表7-4　打印属性一览表
注意：可以看到，属性的数量很多，其中许多属性都是专用的。大多数属性都来源于因特网打印协议1.1版（RFC 2911）。
注意：打印API的早期版本引入了JobAttributes和PageAttributes类，其目的与本节所介绍的打印属性类似。这些类现在已经弃用了。
javax.print.attribute.Attribute 1.4
·Class getCategory（）
获取该属性的类别。
·String getName（）
获取该属性的名字。
javax.print.attribute.AttributeSet 1.4
·boolean add（Attribute attr）
向属性集中添加一个属性。如果集中有另一个属性和此属性有相同的类别，那么集中的属性被新添加的属性所取代。如果由于添加属性的操作改变了属性集，则返回true。
·Attribute get（Class category）
检索带有指定属性类别键的属性，如果该属性不存在，则返回null。
·boolean remove（Attribute attr）
·boolean remove（Class category）
从属性集中删除给定属性，或者删除具有指定类别的属性。如果由于这个操作改变了属性集，则返回true。
·Attribute[]toArray（）
返回一个带有该属性集中所有属性的数组。
javax.print.PrintService 1.4
·PrintServiceAttributeSet getAttributes（）
获取打印服务程序的属性。
javax.print.DocPrintJob 1.4
·PrintJobAttributeSet getAttributes（）
获取打印作业的属性。
我们就要结束关于打印的讨论了。现在，读者已经知道应该如何打印2D图形和其他文档类型，怎样枚举各种打印机和流打印服务程序，以及如何设置和获取打印属性。接下来，我们将介绍两个重要的用户接口问题：剪贴板和拖放机制。7.13　剪贴板
在图形用户界面环境（比如Windows和X Window系统）中，剪切和拷贝是最有用和最方便的用户接口机制之一。你可以在某个程序中选择一些数据，将它们剪切或者拷贝到剪贴板上。然后选择另一个程序，将剪切板中的内容粘贴到该应用中去。使用剪贴板，可以把文本、图像或者别的数据从一个文档移动到另一个文档中，当然也可以从文档的一个地方移动到该文档的另一个地方。剪切和粘贴操作是如此普通，以至于大多数计算机的用户从来都没有考虑过它究竟是如何实现的。
尽管剪贴板从概念上来说是非常简单的，但是实现剪贴板服务却比想象中的要困难得多。假设你将文本从字处理程序拷贝到了剪贴板，如果你要将该文本粘贴到另一个字处理程序中，那么肯定希望该文本的字体和格式保持原样。也就是说，剪贴板中的文本必须保留原来的格式信息。但是如果要将该文本信息粘贴到一个纯文本域中，那么你希望只粘贴文本字符，而不包括附加的格式代码。为了支持这种灵活性，数据提供者可以提供多种格式的剪贴板数据，而数据使用者可以从多种格式中选择所需要的格式。
微软公司的Windows和苹果公司的Macintosh等操作系统的剪贴板实现方法是类似的。当然，它们之间会有略微的不同。然而，X Window系统的剪贴板机制的功能是非常有限的。它只支持纯文本的剪切和粘贴。当你试图运行本节中的程序时，应该考虑它的这些局限性。
注意：请查看你的平台上的jre/lib/flavormap.properties文件，以便得知关于哪些类型的对象能够在Java程序和系统剪贴板之间进行传递。
通常，程序应该支持对系统剪贴板不能处理的那些数据类型的剪切和粘贴。数据传递API支持对同一个虚拟机中任何本地对象引用的传递，而在不同的虚拟机之间，可以将序列化对象和对象的引用传递给远程对象。
表7-5汇总了剪贴板机制的数据传递能力。
表7-5　Java数据传递机制的能力
7.13.1　用于数据传递的类和接口
在Java技术中，java.awt.datatransfer包实现了数据传递的功能。下面就是该包中最重要的类和接口的概述。
·能够通过剪贴板来传递数据的对象必须实现Transferable接口。
·Clipboard类描述了一个剪贴板。可传递的对象是惟一可以置于剪贴板之上或者从剪贴板上取走的项。系统剪贴板是Clipboard类的一个具体实例。
·DataFlavor类用来描述存放到剪贴板中的数据风格。
·StringSelection类是一个实现了transferable接口的实体类。它用于传递文本字符串。
·当剪贴板的内容被别人改写时，如果一个类想得到这种情况的通知，那么就必须实现ClipboardOwner接口。剪贴板的所有权实现了复杂数据的“延迟格式化”。如果一个程序传递的是一个简单数据（比如一个字符串），那么它只需要设置剪贴板的内容，然后就可以继续进行接下来的操作了。但是，如果一个程序想把能够用多种风格来格式化的复杂数据放到剪贴板上，那么它实际上并不需要为此准备所有的风格，因为大多数的风格是从来不会被用到的。不过，这时必须保存剪贴板中的数据，这样就能在以后被请求的时候，建立所需的风格。当剪贴板的内容被更改时，剪贴板的所有者必须得到通知（通过调用lostOwnership方法）。这样可以告诉它，这些信息已经不再需要了。在我们的示例程序中，并不用担心剪贴板的所有权问题。7.13.2　传递文本
如果要了解数据传递类，最好的方法就是从最简单的情况开始：即在系统剪贴板上传递和获取文本信息。首先，获取一个系统剪贴板的引用：
传递给剪贴板的字符串，必须被包装在StringSelection对象中。
实际的传递操作是通过调用setContents方法来实现的，该方法将一个String-Selection对象和一个ClipBoardOwner作为参数。如果不想指定剪贴板所有者的话，可以把第二个参数设置为null。
下面是反过来的操作：从剪贴板中读取一个字符串：
程序清单7-17展示了如何在一个java应用和系统剪贴板之间进行剪切和粘贴操作。如果你选择文本区域中的一块文本区域，并且点击Copy，那么选中的文本就会被拷贝到系统剪贴板中，然后可以将其粘贴到任何文本编辑器中（参见图7-40）。反之，当从文本编辑器中拷贝文本时，也可以将其粘贴到我们的示例程序中。
图7-40　TextTransferTest程序的运行情况
程序清单7-17　transferText/TextTransferFrame.java
java.awt.Toolkit 1.0
·Clipboard getSystemClipboard（）1.1
获取系统剪贴板。
java.awt.datatransfer.Clipboard 1.1
·TRansferable getContents（Object requester）
获取剪贴板中的内容。
参数：requester　请求剪贴板内容的对象；该值实际上并不使用
·void setContents（Transferable contents，ClipboardOwner owner）
将内容放入剪贴板中。
参数：contents　封装了内容的Transferable
owner　当新的信息被放入剪贴板上时，要通知的对象（通过调用lostOwnership方法）。如果不需要通知，则值为null
·boolean isDataFlavorAvailable（DataFlavor flavor）5.0
如果该剪贴板中有给定风格的数据，那么返回true。
·Object getData（DataFlavor flavor）5.0
获取给定风格的数据，如果给定风格的数据不存在，则抛出UnsupportedFlavor-Exception异常。
java.awt.datatransfer.ClipboardOwner 1.1
·void lostOwnership（Clipboard clipboard，Transferable contents）
通知该对象，它已经不再是该剪贴板内容的所有者。
参数：clipboard　已放置内容的剪贴板
contents　该所有者放入剪贴板上的内容
java.awt.datatransfer.Transferable 1.1
·boolean isDataFlavorSupported（DataFlavor flavor）
如果给定的风格是所支持的数据风格中的一种，则返回true，否则返回false。
·Object getTransferData（DataFlavor flavor）
返回用所请求风格格式化的数据。如果不支持所请求的风格，则抛出一个Unsupported-FlavorException异常。7.13.3　Transferable接口和数据风格
DataFlavor是由下面两个特性来定义的：
·MIME类型的名字（比如"image/gif"）。
·用于访问数据的表示类（比如java.awt.Image）。
此外，每一种数据风格都有一个适合人类阅读的名字（比如"GIF Image"）。
表示类可以用MIME类型的class参数设定，例如，
注意：这只是一个说明其语法的例子。对于传递GIF图像数据，并没有一个标准的数据风格。
如果没有给定任何class参数，那么表示类就是InputStream。
为了传递本地的、序列化的和远程的Java对象，人们定义了如下三个MIME类型：
注意：x-前缀表示这是一个试用名，并不是IANA批准的名字，IANA是负责分配标准的MIME类型名的机构。
例如，标准的stringFlavor数据风格是由下面这个MIME类型描述的：
可以让剪贴板列出所有可用的风格：
也可以在剪贴板上安装一个FlavorListener，当剪贴板上的数据风格集合产生变化时，可以通知风格监听器。细节请参阅API注释。
java.awt.datatransfer.DataFlavor 1.1
·DataFlavor（String mimeType，String humanPresentableName）
创建一个数据风格，它描述了一个流数据，该流数据的格式是由一个MIME类型所描述的。
参数：mimeType　一个MIME类型字符串
humanPresentableName　一个更易于阅读的名字版本
·DataFlavor（Class class，String humanPresentableName）
创建一个用来描述Java平台类的数据风格。它的MIME类型是application/x-java-serialized-object；class=className。
参数：class　从Transferable检索到的类
humanPresentableName　一个可阅读的名字版本
·String getMimeType（）
返回该数据风格的MIME类型字符串。
·boolean isMimeTypeEqual（String mimeType）
测试该数据风格是否有给定的MIME类型。
·String getHumanPresentableName（）
为该数据风格的数据格式返回人们容易阅读的名字。
·Class getRepresentationClass（）
返回一个Class对象，它代表用该数据风格调用Transferable时返回的对象的类。它可以是MIME类型的class参数，也可以是InputStream。
java.awt.datatransfer.Clipboard 1.1
·DataFlavor[]getAvailableDataFlavors（）5.0
返回一个可用风格的数组。
·void addFlavorListener（FlavorListener listener）5.0
添加一个监听器，当可用的风格发生改变时，会通知该监听器。
java.awt.datatransfer.Transferable 1.1
·DataFlavor[]getTransferDataFlavors（）
返回一个所支持风格的数组。
java.awt.datatransfer.FlavorListener 5.0
·void flavorsChanged（FlavorEvent event）
当一个剪贴板中可用的风格集发生变化时，就调用该方法。7.13.4　构建一个可传递的图像
想要通过剪贴板传递对象就必须实现Transferable接口。StringSelection类是目前Java标准库中惟一一个实现了Transferable接口的公有类。在这一节中，我们将介绍如何通过剪贴板来传递图像。因为Java并没有提供传递图像的类，所以读者必须自己去实现它。
这个类其实只是一个非常普通的类。它只需告知惟一可用的数据格式是DataFlavor.image Flavor，并且它持有一个image对象即可。
注意：Java SE提供了DataFlavor.imageFlavor常量，并且负责执行所有复杂的操作，以便进行Java图像与本机剪贴板图像的转换。但是，奇怪的是，它并没有提供将图像放入剪贴板时所必需的包装类。
程序清单7-18中的程序展示了如何在Java应用程序和系统剪贴板之间进行图像传递。当程序开始运行时，它产生了一个包含红圈的图像。点击Copy按钮把图像拷贝到剪贴板上，然后将它粘贴到另一个应用中（参见图7-41）。之后再从另一个应用拷贝一个图像到系统剪贴板中，然后点击Paste按钮，就可以看到该图像被粘贴到示例程序中了（参见图7-42）。
图7-41　将图像从一个Java程序拷贝到本机程序中
图7-42　将图像从本机程序拷贝到一个Java程序中
该程序是直接在文本传递程序基础上进行修改而得到的。现在的数据风格是DataFlavor.imageFlavor，并且我们使用的是ImageSelection类来将图像传递给系统剪贴板。
程序清单7-18　imageTransfer/ImageTransferFrame.java
7.13.5　通过系统剪贴板传递Java对象
假设你想从一个Java应用拷贝和粘贴对象到另一个Java应用中，此时，你可以通过在系统剪贴板中放置序列化的Java对象来实现此任务。
程序清单7-19中的程序展示了这种能力。该程序显示了一个颜色选择器；Copy按钮将使当前的颜色以序列化Color对象的方式拷贝到系统剪贴板上；Paste按钮可以用来检查系统剪贴板中是否包含了一个序列化的Color对象，如果包含的话，它将提取该颜色，并且将它设置为颜色选择器的当前选择。
可以在两个Java应用程序之间传递被序列化的对象（参见图7-43）。运行两个SerialTransferTest程序，在第一个程序上点击Copy按钮，然后，在第二个程序上点击Paste按钮。这时，颜色对象便会从一个虚拟机传递到另一个虚拟机。
为了启用数据传递，Java平台将二进制数据放置到包含了被序列化对象的系统剪贴板上。这样，另一个Java程序就能够获取剪贴板中的数据，并且反序列化该对象，该Java程序没有必要与产生剪贴板数据的程序属相同类型的程序。
当然，一个非Java的应用将不知道如何处理剪贴板中的数据。出于这个原因，该示例程序提供了采用第二种风格的剪贴板数据，即文本数据。该文本是对被传递对象调用toString方法得到的结果。如果要查看第二种剪贴板的数据风格，则运行该程序，点击一种颜色，然后在你的文本编辑器中选中Paste命令之后类似于下面这样的一个字符串：
就会被插入到你的文档中。
图7-43　数据在一个Java应用的两个实例之间进行拷贝
实际上并不需要进行额外的编程就可以传递一个被序列化的对象，我们可以使用MIME类型：
与前面一样，你必须构建自己的传递包装器，细节请参见示例代码。
程序清单7-19　serialTransfer/SerialTransferFrame.Java
7.13.6　使用本地剪贴板来传递对象引用
有时，你需要拷贝和粘贴一个数据类型，但是该数据类型并不是系统剪贴板所支持的数据类型，且该数据类型是不可序列化的。如果要在同一个虚拟机内传递一个任意的Java对象引用，可以使用MIME类型。
这时需要为这种类型定义一个Transferable包装器，其过程与前面示例中介绍的SerialTransferable包装器完全相似。
对象的引用只有在单个虚拟机中才有意义。出于这个原因，不能将形状对象拷贝到系统剪贴板中。相反，要使用本地剪贴板：
构造器的参数是剪贴板的名字。
不过，使用本地剪贴板有一个重要的缺点：你必须使本地剪贴板和系统剪贴板同步，这样用户才不会将两者混淆。目前，Java平台并没有执行这个同步的功能。
java.awt.datatransfer.Clipboard 1.1
·Clipboard（String name）
构建一个带有指定名字的本地剪贴板。7.14　拖放操作
使用剪切和粘贴操作在两个程序之间传递信息时，剪贴板起到了一个中介的作用。拖放操作，打个比方来说就是去掉中间人，让两个程序之间直接通信。Java平台为拖放操作提供了基本的支持。我们还可以在Java程序和本地程序之间进行拖放操作。本节将要介绍如何编写作为放置目标的Java应用，以及如何编写作为拖曳源的应用。
在深入介绍Java平台的拖放操作支持特性之前，让我们快速地浏览一些拖放操作的用户界面。我们使用Windows Explorer和WordPad程序作为示例。在其他平台上，读者可以使用本机可用的带有拖放操作的程序来做试验。
可以使用拖曳源中的一个示意动作来初始化一个拖曳操作，通常需要选定一个或者多个元素，然后将选定的目标拖离它的初始位置。当你在接收放置操作的放置目标上释放鼠标按键时，放置目标将查询拖曳源，进而了解关于放置元素的信息，并且启动某个恰当的操作。例如，如果将一个文件图标从文件管理器中拖放到某个目录图标的上面，那么这个文件就会被移动到这个目录中。但是，如果将它拖放到一个文本编辑器中，那么文本编辑器就会打开这个文件。（当然，这要求我们所使用的文件管理器和文本编辑器支持拖放操作，例如Windows中的Explore与WordPad，以及Gnome中的Nautilus与gedit）。
如果在拖曳的时候按住CTRL键，那么放置操作的类型将从移动操作变为拷贝操作，该文件的一份拷贝被放入此目录中。如果同时按住了SHIFT和CTRL键，那么该文件的一个链接将被放入到此目录中。（其他平台可能使用别的按键组合来执行这些操作）
因此，有三种带有不同示意动作的放置操作：
·移动；
·拷贝；
·链接。
链接操作的目的是建立一个对被放置元素的引用。这种链接通常需要得到本机操作系统的支持（比如用于文件的符号链接，或者用于文档构件的对象链接），并且它通常在跨平台的程序中没有太大的意义。在本节中，我们将着重介绍如何使用拖放操作来进行拷贝和移动。
拖曳操作通常能够产生某种直观的反馈信息，至少光标的形状会发生改变。当你把光标移动到可能的放置目标上时，光标的形状将会表示出放置操作是否可行。如果放置操作可行的话，光标的形状也会表示出放置动作的类型。表7-6显示了光标在放置目标上所显示的几种形状。
表7-6　放置光标的形状
除了文件图标外，也可以拖曳别的元素。例如，可以在WordPad中选择文本，然后拖曳之。请试着将文本字段放到你希望放置的对象中，并且观察它们做何反应。
注意：这个试验显示了作为用户界面机制的拖放操作的一个缺点。用户很难预计究竟能拖曳什么，可以将它们放置到何处，以及当实施拖放操作时会出现什么情况。由于默认的移动操作能够删除原始的元素，因此用户在使用拖放操作时比较谨慎，这是可以理解的。7.14.1　Swing对数据传递的支持
从Java SE1.4开始，多种Swing构件就已经内置了对数据传递的支持（参见表7-7）。我们可以从大量的构件中拖曳选中的文本，也可以将文本放置到文本构件中。为了向后兼容性，我们必须使用setDragEnabled方法来激活拖曳功能，而放置功能总是会得到支持的。
表7-7　Swing中支持数据传递的构件
注意：java.awt.dnd包提供了一个低层的拖放API，它形成了Swing拖放的基础。我们在本书中不讨论这个API。
程序清单7-20中的程序演示了这种行为。在运行该程序时，应该注意下面几点：
·你可以选择列表、表格或树中的多个项（见程序清单7-21），并拖曳它们。
·从表格中拖曳项有些尴尬，你需要先用鼠标选择，然后移走鼠标，之后再次点击它，这之后才能拖曳它。
·当你在文本域中放置项时，可以看到被拖曳的信息是如何被格式化的：表格中的表元由制表符隔开，而每个选中的行都占据单独的一行（参见图7-44）。
图7-44　Swing的拖放测试程序
·你只能拷贝而不能移动列表、表格、树、文件选择器或颜色选择器中的项。对于所有的数据模型来说，从列表、表格或树中移除项都是不可能的。在下一节中你将会看到当数据模型可编辑时，可以如何实现这种移除能力。
·你不能在列表、表格、树或文件选择器中拖曳。
·如果你运行该程序的两个副本，就可以将颜色从一个颜色选择器中拖曳到另一个中。
·你不能将文本从文本域中拖出，因为我们没有在文本域上调用setDragEnabled。
Swing包提供了一个潜在的非常有用的机制，可以迅速地将一个构件转换成一个放置目标。我们可以为给定的属性安装一个传递处理器，例如，在示例程序中，我们调用了：
现在，可以在文本框中拖曳颜色，而其背景色也会随之改变。
当发生了一个放置操作时，传递处理器将检查是否有一种数据风格的表示类为Color，如果确实如此，那么它便调用setBackground方法。
通过把传递处理器安装在文本区域中，就可以禁用标准的传递处理器。你再也不能在此文本域中进行剪切、拷贝、粘贴、拖曳或者放置操作了。但是，你现在可以把颜色从该文本域中拖出来了。你仍旧需要选中文本以激活拖曳状态。当拖曳文本时，你会发现你可以将其放置到颜色选择器中，并将其颜色值改变成文本域的背景色。但是，你不能在文本域中放置文本。
程序清单7-20　dnd/SwingDnDTest.java
程序清单7-21　dnd/SampleComponents.java
javax.swing.JComponent 1.2
·void setTransferHandler（TransferHandler handler）1.4
设置一个用来处理数据传递操作（剪切、拷贝、粘贴、拖曳、放置）的传递处理器。
javax.swing.TransferHandler 1.4
·TransferHandler（String propertyName）
构建一个传递处理器，它可以在执行数据传递操作时读取或者写入带有给定名称的JavaBeans构件的属性。
javx.swing.JFileChooser 1.2
javax.swing.JColorChooser 1.2
javax.swing.text.JTextComponent 1.2
javax.swing.JList1.2
javax.swing.JTable 1.2
javax.swing.JTree 1.2
·void setDragEnabled（boolean b）1.4
使将数据从该构件中拖曳出去的操作可用或者禁用。7.14.2　拖曳源
在前一节中，你已经看到了如何利用Swing中基本的拖放支持。在本节中，我们将向你展示如何将任意的构件配置成拖曳源。在下一节中，我们将讨论放置目标，并将一个示例构件同时设置为图像的拖曳源和放置目标。
为了定制Swing构件的拖放行为，必须子类化TransferHandler类。首先，覆写getSourceActions方法，以表明该构件支持什么样的行为（拷贝、移动、链接）。接下来，覆写getTransferable方法，以产生Transferable对象，其过程遵循向剪贴板拷贝对象的过程。
在示例程序中，我们从一个JList中拖曳图像，这个JList填充了若干图像的图标（参见图7-45）。下面是createTransferable方法的实现，被选中的图像直接放置到一个ImageTransferable包装器中。
本例中，我们庆幸的是JList已经具备了启动拖曳姿态的机制，你只需通过调用setDragEnabled方法来激活这种机制。如果你希望向不识别拖曳姿态的构件中添加对拖曳操作的支持，则需要由你自己来启动这种传递。例如，下面的代码展示了如何在JLabel上启动拖曳：
图7-45　ImageList的拖放应用
这里，我们只是在用户在标签上点击时启动传递。更复杂的实现还可以观察引起鼠标微量拖曳的鼠标移动。
当用户完成放置行为后，拖曳源传递处理器的exportDone方法就会被调用，在这个方法中，如果用户执行了移动动作，则应该移除被传递的对象。下面是图像列表的相关实现：
总结一下，为了使一个构件成为拖曳源，需要添加一个指定了下列内容的传递处理器：
·可以支持哪些行为。
·可以传输哪些数据。
·在执行移动动作之后，如何移除原来的数据。
此外，如果拖曳源是表7-7中所列构件之外的构件，则还需要观察鼠标姿态和启动传递。
javax.swing.TransferHandler 1.4
·int getSourceActions（JComponent c）
覆写该方法，让其返回在给定的构件上进行拖曳时，允许对拖曳源执行的动作（COPY、MOVE和LINK的位组合）。
·protected Transferable createTransferable（JComponent source）
覆写该方法，让其为被拖曳的数据创建一个Transferable。
·void exportAsDrag（JComponent comp，InputEvent e，int action）
从给定的构件中启动一个拖曳姿态，action参数的值是COPY、MOVE或LINK。
·protected void exportDone（JComponent source，Transferable data，int action）
覆写该方法，让其在传递成功之后调整拖曳源。7.14.3　放置目标
在本节中，我们将展示如何实现放置目标。我们用到的示例还是填充了图像的图标的JList，在其中我们添加了对放置的支持，以便用户可以将图像放置到列表中。
要使一个构件成为放置目标，需要设置一个TransferHandler，并实现canImport和importData方法。
注意：我们可以向JFrame中添加传递处理器。其最常用到的地方就是在应用中放置文件，有效的放置位置包括窗体的装饰和菜单条，但是不包括窗体中包含的构件（它们有自己的传递处理器）。
当用户在放置的目标构件上移动鼠标时，canImport方法会被连续调用，如果放置是允许的，则返回true。这个信息对光标的图标产生影响，因为这个图标要对是否允许放置给出可视的反馈。
canImport方法有了一个TransferHandler.TransferSupport类型的参数，通过这个参数，可以获取用户选择的放置动作、放置位置以及要传输的数据。（在Java SE6之前，调用的是与此不同的一个canImport方法，它只提供数据风格的列表。）
在canImport方法中，还可以覆写用户的放置动作。例如，如果用户选择了移动动作，但是移除原有项是不恰当的，那么就可以强制传递处理器使用拷贝动作取而代之。
下面是一个典型的示例：假设图像列表构件将接受放置文件列表和图像的请求，但是，如果一个文件列表被拖曳到了这个构件中，那么用户选择的MOVE动作就会改为COPY动作，这样图像文件就不会被删除。
更复杂的实现可以检查拖曳的文件中是否确实包含图像。
当鼠标在放置目标上移动时，Swing构件JList、JTable、JTree和JTextComponent会给出有关插入位置的可视反馈。默认情况下，选中（对于JList、JTable和JTree而言）或脱字符（对于JTextComponent而言）被用来表示放置位置。这种方法对用户来说显得很不友好，而且也不灵活，它被设置成默认值只是为了向后兼容。应该调用setDropMode方法来选择更恰当的可视反馈。
可以控制被放置的数据是应该覆盖已有项，还是应该插入到已有项的中间。例如，在示例程序中，我们调用了
以允许用户在某一项上放置（因此也就覆盖了这一项），或者在两项之间插入（参见图7-46）。表7-8给出了Swing构件支持的放置模式。
图7-46　在某一项上放置的可视指示器和在两项之间放置的可视指示器
表7-8　放置模式
一旦用户结束了放置姿态，importData方法就会被调用。此时需要从拖曳源获得数据，在TransferSupport参数上调用getTransferable方法就可以获得一个对Transferable对象的引用。这与拷贝和粘贴时使用的接口相同。
拖放最常用的一种数据类型是DataFlavor.javaFileListFlavor。文件列表描述了要放置到目标上的文件集合，而传递数据就是List<File>类型的一个对象。下面的代码可以获取这些文件：
在放置表7-8中列出的构件时，需要知道放置数据的精确位置。在TransferSupport参数上调用getDropLocation方法可以发现产生放置动作的位置，这个方法将返回一个TransferHandler.DropLocation的某个子类的对象。JList、JTable、JTree和JTextComponent类都定义了在特定的数据模型中指定位置的子类。例如，在列表中的位置可以是一个整数，但是树中的位置就必须是一个树的路径。下面的代码展示了如何在我们的图像列表中获取放置位置：
JList.DropLocation子类有一个getIndex方法，该方法返回放置位置的索引。（JTree.DropLocation子类有一个类似的getPath方法。）
在数据通过CTRL+V组合键粘贴到构件中时，importData方法也会被调用。在这种情况下，getDropLocation方法将抛出IllegalStateException。因此，如果isDrop方法返回false，我们就只是将粘贴的数据追加到列表的尾部。
在向列表、表格或树中插入时，还需要检查数据是要插入到项之间，还是应该替换插入位置的项。对于列表，可以调用JList.DropLocation的isInsert方法，对于其他的构件，请查看本节末尾关于它们的放置位置类的API说明。
总结一下，为了使一个构件成为放置目标，需要添加一个指定了下列内容的传递处理器：
·何时可以接受被拖曳的项。
·如何导入被放置的数据。
此外，如果要向JList、JTable、JTree和JTextComponent添加对放置的支持，还应该设置放置模式。
程序清单7-22展示了完整的程序。注意，ImageList类既是拖曳源，又是放置目标。请尝试在两个列表之间拖曳图像，也可以从其他程序的文件选择器中拖曳图像文件到这些列表中。
程序清单7-22　dndImage/imageListDnDFrame.java
javax.swing.TransferHandler 1.4
·boolean canImport（TransferSupport support）6
覆写该方法，让其表示目标构件是否能够接受TransferSupport参数所描述的拖曳。
·boolean importData（TransferSupport support）6
覆写该方法，让其实现由TransferSupport参数描述的放置或粘贴姿态，并且在导入成功时返回true。
javax.swing.JFrame 1.2
·void setTransferHandler（TransferHandler handler）6
将传递处理器设置成为只处理放置和粘贴操作。
javax.swing.JList 1.2
javax.swing.JTable 1.2
javax.swing.JTree 1.2
javax.swing.text.JTextComponent 1.2
·void setDropMode（DropMode mode）6
将这个构件的放置模式设置成为表7-8中指定的值之一。
javax.swing.TransferHandler.TransferSupport 6
·Component getComponent（）
获取这个传递的目标构件。
·DataFlavor[]getDataFlavors（）
获取被传递数据的数据风格。
·boolean isDrop（）
如果这个传递是放置，则返回true，如果是粘贴则返回false。
·int getUserDropAction（）
获取由用户选择的放置动作（MOVE、COPY或LINK）。
·getSourceDropActions（）
获取拖曳源允许执行的放置动作。
·getDropAction（）
·setDropAction（）
获取和设置这个传递的放置动作。最初，这是一个用户的放置动作，但是它可以被传递处理器所覆盖。
·DropLocation getDropLocation（）
获取放置的鼠标位置，如果该传递不是一个放置动作，则抛出IllegalStateException。
javax.swing.TransferHandler.DropLocation 6
·Point getDropPoint（）
获取在目标构件中放置的鼠标位置。
javax.swing.JList.DropLocation 6
·boolean isInsert（）
如果数据被插入到给定位置之前，则返回true，如果它们替换了已有数据，则返回false。
·int getIndex（）
获取模型中用于插入或替换的索引。
javax.swing.JTable.DropLocation 6
·boolean isInsertRow（）
·boolean isInsertColumn（）
如果输入被插入到某行或某列之前，则返回true。
·int getRow（）
·int getColumn（）
获取模型中用于插入或替换的行或列索引，如果放置发生在空区域，则返回–1。
javax.swing.JTree.DropLocation 6
·TreePath getPath（）
·int getChildIndex（）
返回树的路径和孩子，以及目标构件的放置模式，该模式定义了拖放的位置，如下表所示。
放置模式树编辑动作
INSERT　作为该路径的一个孩子插入，插入到获得的孩子索引之前
ON or USE_SELECTION　替换该路径中的数据（没用到孩子索引）
INSERT_OR_ON　如果获得的孩子索引为-1，则以ON模式执行，否则，以INSERT模式执行
javax.swing.text.JTextComponent.DropLocation 6
·int getIndex（）
插入数据处的索引。7.15　平台集成
我们用几个特性来结束本章，这些特性使得Java应用看起来更像是本地应用。闪屏特性使得应用在虚拟机启动时可以显示一个闪屏；java.awt.Desktop类使我们可以启动本地应用，例如默认的浏览器和e-mail程序；最后，可以像许多本地应用一样，对系统托盘进行访问，并可以用图标来塞满它。7.15.1　闪屏
对Java应用最常见的抱怨就是启动时间太长。这是因为Java虚拟机花费了一段时间去加载所有必需的类，特别是对Swing应用，它们需要从Swing和AWT类库代码中抽取大量的内容。用户并不喜欢应用程序花费大量的时间去产生初始屏幕，他们甚至可能在不知道首次启动是否成功的情况下尝试着多次启动该应用程序。此问题的解决之道是采用闪屏，即迅速出现的小窗体，它可以告诉用户该应用程序已经成功启动了。
当然，我们可以在main方法开始之后立即呈现一个窗体，但是，main方法只有在类加载器加载了所有需要依赖的类之后才会被启动，而这一过程可能要等上一段时间。
可以让虚拟机在启动时立即显示一幅图像来解决这个问题。有两种机制可以指定这幅图像，一种是使用命令行参数-splash：
另一种是在JAR文件的清单中指定：
这幅图像会立即出现，并会在第一个AWT窗体可视时立即自动消失。我们可以使用任何GIF、JPEG或PNG图像、动画（GIF）和透明（GIF和PNG）都可以得到支持。
如果你的应用程序在达到main之后立即就可以执行，那么你就可以略过本节余下的内容。但是，许多应用使用了插件架构，其中有一个小内核，它将在启动时加载插件集。Eclipse和NetBeans就是典型的实例。在这种情况下，可以用闪屏来表示加载进度。
有两种方式来实现上述功能，即可以直接在闪屏上绘制，或者用含有相同内容的无边界窗体来替换初始图像，然后在该窗体的内部绘制。我们的示例程序同时展示了这两种技术。
为了直接在闪屏上绘制，需要获取一个对闪屏的引用，以及它的图形上下文与尺寸：
现在可以按照常规的方式来绘制了。当绘制完成后，调用update来确保绘制的图画被刷新。我们的示例程序绘制了一个简单的进度条，就像在图7-47中左边一幅图中看到的那样。
图7-47　初始的闪屏和无边界的后续视窗
注意：闪屏是单例对象，因此你不能创建自己的闪屏对象。如果在命令行或清单中没有设置任何闪屏，getSplashScreen方法将返回null。
直接在闪屏上绘制有一个缺陷，即计算所有的像素位置会显得很冗长，而且进度指示器不会去观察本地进度条。为了避免这些问题，可以在main方法启动后立即将初始闪屏用具有相同尺寸和内容的后续视窗替换。这个视窗可以包含任意的Swing构件。
程序清单7-23中的示例程序展示了这种技术。图7-47右边的那幅图展示了一个无边界的窗体，它有一个面板，绘制了闪屏并包含一个JProgressBar。现在我们对Swing API有了完整的访问能力，可以很轻松地添加消息字符串而不用受像素位置的困扰了。
请注意，我们不需要移除初始闪屏，它会在后续视窗可视之后被自动移除掉。
警告：遗憾的是，在闪屏被后续视窗替代时，会有明显的闪烁。
程序清单7-23　splashScreen/SplashScreenTest.java
java.awt.SplashScreen 6
·static SplashScreen getSplashScreen（）
获取一个对闪屏的引用，如果目前没有任何闪屏，则返回null。
·URL getImageURL（）
·void setImageURL（URL imageURL）
获取或设置闪屏图像的URL。设置该图像会更新闪屏。
·Rectangle getBounds（）
获取闪屏的边界。
·Graphics2D createGraphics（）
获取用于在闪屏上绘制的图形上下文。
·void update（）
更新闪屏的显示。
·void close（）
关闭闪屏。闪屏在第一个AWT视窗可视时会自动关闭。7.15.2　启动桌面应用程序
java.awt.Desktop类使我们可以启动默认的浏览器和e-mail程序，我们还可以用注册为用于某类文件类型的应用程序来打开、编辑和打印这类文件。
其API是很直观的。首先，调用静态的isDesktopSupported方法，如果它返回true，则当前平台支持启动桌面应用程序。然后调用静态的getDesktop方法来获取一个Desktop实例。
并非所有桌面环境都支持所有的API操作。例如，在Linux上的Gnome桌面中，有可能可以打开文件，但是不能打印它们。（目前没有对文件关联中的“动词”进行支持。）要查明平台所支持的操作，可以调用isSupported方法，并将Desktop.Action枚举中的某个值传给它。我们的示例程序中包含了下面这样的测试：
为了打开、编辑和打印文件，首先要检查这个动作是否得到了支持，然后再调用open、edit和print方法。为了启动浏览器，需要传递一个URI。（有关URI的更多信息可参见第3章。）可以直接用包含一个http或https的URL的字符串来调用URI构造器。
为了启动默认的e-mail程序，需要构造一个具有特定格式的URI，即：
这里recipient是接收者的e-mail地址，例如president@whitehouse.gov，而query包含了用&分隔的name=value对，其中值是用百分号编码的。（百分号编码机制实质与第3章所描述的URL编码机制算法相同，但是空格被编码为%20，而不是+）。subject=dinner%20RSVP&bcc=putin%40kremvax.ru是一个实例，这种格式归档在RFC2368中（http://www.ietf.org/rfc/rfc2368.txt）。但是，URI类不了解有关mailto这类URI的任何信息，因此我们必须组装和编码自己的URI。
程序清单7-24的示例程序使你可以打开、编辑或打印你选择的文件，可以浏览一个URL，或者启动e-mail程序（参见图7-48）。
图7-48　启动一个桌面应用程序
程序清单7-24　desktopApp/DesktopAppFrame.java
java.awt.Desktop 6
·static boolean isDesktopSupported（）
如果该平台支持启动桌面应用程序，则返回true。
·static Desktop getDesktop（）
返回用于启动桌面应用程序的Desktop对象。如果该平台不支持启动桌面操作，则抛出UnsupportedOperationException异常。
·boolean isSupported（Desktop.Action action）
如果支持给定的动作，则返回true。action是OPEN、EDIT、PRINT、BROWSE或MAIL之一。
·void open（File file）
启动注册为浏览给定文件的应用程序。
·void edit（File file）
启动注册为编辑给定文件的应用程序。
·void print（File file）
打印给定文件。
·void browse（URI uri）
用给定的URI启动默认浏览器。
·void mail（）
·void mail（URI uri）
启动默认邮件程序。第二个版本可以用来填充e-mail消息的部分内容。7.15.3　系统托盘
许多桌面环境都有一个区域用于放置在后台运行的程序的图标，这些程序偶尔会将某些事件通知给用户。在Windows中，这个区域称为系统托盘，而这些图标称为托盘图标。Java API采纳了相同的术语命名规则。有一个这种程序的典型实例，那就是检查软件更新的监视器。如果有新的更新，监视器程序可以改变其图标的外观，并在图表附近显示一条消息。
坦白地讲，系统托盘有些被滥用，当计算机用户发现又添加了新的托盘图标时，通常都会感到不痛快。我们的系统托盘应用程序示例也逃脱不了这条规则，这个程序可以分发虚拟的“签饼”。
java.awt.SystemTray类是跨平台的通向系统托盘的渠道，与前面讨论过的Desktop类相类似，首先要调用静态的isSupported方法来检查本地Java平台是否支持系统托盘。如果支持，则通过调用静态的getSystemTray方法来获取SystemTray的单例。
SystemTray类最重要的方法是add方法，它使得我们可以添加一个TrayIcon实例。托盘图标有三个主要的属性：
·图标的图像。
·当鼠标滑过图标时显示的工具提示。
·当用户用鼠标右键点击图标时显示的弹出式菜单。
弹出式菜单是AWT类库中的PopupMenu类的一个实例，表示本地的弹出式菜单，而不是Swing菜单。可以在其中添加AWT的MenuItem实例，而这些实例每个都有一个动作监听器，就像Swing中的菜单项一样。
最后，托盘图标可以向用户显示通知信息（参见图7-49），这需要调用TrayIcon类的displayMessage方法，并指定标题、消息和消息类型。
图7-49　从托盘图标中发出的通知
程序清单7-25展示了将“签饼”图标置于系统托盘中的应用程序。这个程序将读取一个签饼文件（从UNIX的fortune程序中读取），其中每个签都包含一段文本，这段文本的最后一行包含一个%字符。这个程序每秒显示一条消息。幸运的是，有一个弹出菜单可以用来退出该应用程序。如果所有的系统托盘图标都这么贴心就好了！
程序清单7-25　systemTray/SystemTrayTest.java
java.awt.SystemTray 6
·static boolean isSupported（）
如果这个平台支持对系统托盘的访问，则返回true。
·static SystemTray getSystemTray（）
返回用于访问系统托盘的SystemTray对象。如果这个平台不支持对系统托盘的访问，则抛出UnsupportedOperationException异常。
·Dimension getTrayIconSize（）
获取系统托盘中的图标的尺寸。
·void add（TrayIcon trayIcon）
·void remove（TrayIcon trayIcon）
添加或移除一个系统托盘图标。
java.awt.TrayIcon 6
·TrayIcon（Image image）
·TrayIcon（Image image，String tooltip）
·TrayIcon（Image image，String tooltip，PopupMenu popupMenu）
用给定的图像、工具提示和弹出式菜单构建一个托盘图标。
·Image getImage（）
·void setImage（Image image）
·String getTooltip（）
·void setTooltip（String tooltip）
·PopupMenu getPopupMenu（）
·void setPopupMenu（PopupMenu popupMenu）
获取或设置图像、工具提示，或该工具提示的弹出式菜单
·boolean isImageAutoSize（）
·void setImageAutoSize（boolean autosize）
获取或设置imageAutoSize属性，如果设置了，那么图像就会缩放到适合工具提示图标区的大小。如果没有设置（默认值），那么图像就会被截除（如果图像太大），或者居中（如果图像太小）。
·void displayMessage（String caption，String text，TrayIcon.MessageType messageType）
在托盘图标附近显示消息。消息的类型为INFO、WARNING、ERROR或NONE
·public void addActionListener（ActionListener listener）
·public void removeActionListener（ActionListener listener）
如果被调用的监听器是平台依赖的，则添加和移除动作监听器。典型情况是在通知信息上点击或在托盘图标上双击。
现在，我们来到了本章的尾声，这长长的一章涵盖了高级AWT特性。在下一章，我们将讨论JavaBeans规范和它在GUI构建器中的用处。