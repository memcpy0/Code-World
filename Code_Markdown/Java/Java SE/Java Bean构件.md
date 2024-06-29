第8章　JavaBean构件
▲为何使用Bean
▲Beaninfo类
▲编写Bean的过程
▲属性编辑器
▲使用Bean来构造一个应用
▲定制器
▲Bean属性与事件的命名模式
▲Javabean持久化
▲Bean属性的类型
在JavaBean规范中有bean的官方定义：“根据Sun的JavaBean规范，一个bean就是一个可重用的软件构件，并且能够在开发工具中可视化地操作。”
一旦实现了一个bean，其他人就能够在开发环境中使用它（例如NetBeans）。与必须编写冗长的代码不同，开发人员可以直接将bean拖曳到GUI表单中，并用对话框来定制它。
本章将解释如何实现bean，以便让其他的开发人员可以方便地使用它们。
注意：在进入正题之前，我们先要指出一个常见的误解：本章讨论的JavaBean与Enterprise JavaBean（EJB）没什么关系。企业JavaBeans是服务器端的构件，提供对事务、持久化、复制以及安全问题的支持。从最基本的角度来看，它们都是能够在开发环境中使用的构件。然而，企业JavaBeans技术比“标准版”JavaBean技术要复杂得多。这并不意味着标准的JavaBean构件就仅限于客户端编程。Web技术就很依赖于JavaBean构件模型，例如JavaServer Faces（JSF）和JavaServer Pages（JSP）。8.1　为何使用Bean
有Visual Basic经验的程序员立刻就会明白，为什么bean如此重要。而那些来自“凡事都要亲自动手”的环境中的程序员通常很难相信Visual Basic是可重用对象技术最成功的例子之一。如果回想一下你是如何开发一个Visual Basic应用的，那么Visual Basic如此受欢迎的原因就会很清楚了。对于那些从未使用过Visual Basic的人，下面是一个简单的指导，教你如何构建Visual Basic应用：
1）通过将构件（在Visual Basic中称为控件）拖拽到表单窗口上来开发界面。
2）通过属性表单设置构件的属性，例如高度、颜色或其他行为。
3）属性监视器还列出了构件能够应对的事件。某些事件可以通过对话框捕获处理，而对于其他事件，你需要编写简短的事件处理代码。
举例来说，在本书卷I第2章中，我们编写了一个程序，它在一个窗体中显示了幅图像。那需要超过一页的代码，而在Visual Basic中创建类似功能的程序，只需：
1）添加两个控件到一个窗体中：一个Image控件用来显示图片，以及一个Common Dialog控件用来选择文件。
2）设置CommonDialog控件的Filter属性，使它只显示图像控件能够处理的图像文件，如图8-1所示。
图8-1　Visual Basic中一个图像应用程序的属性窗口
3）编写四行Visual Basic代码，它将在该工程首次运行的时候被激活。所需的全部代码如下所示：
上面的代码会弹出一个文件对话框，而且只有具备正确扩展名的文件才会显示出来，因为我们设置了过滤器属性。在用户选择了一个图像文件之后，该代码会告诉图像控件把选择的图像显示出来。
仅此而已。进行一些布局操作，再添加这些语句，就完成了整整一页Java代码才能完成的功能。很明显，比起编写整整一页的代码，学习如何拖拽构件与设置属性可简单多了。
我们并不认为Visual Basic对于所有的问题都是最好的解决办法。但是对于UI驱动的那一类Windows问题，它确实是最佳选择。JavaBean技术就是为了使Java技术在该领域也具备竞争力才被开发出来的，它使得供应商可以提供Visual Basic风格的开发环境。这些环境使得以最小的编程量来开发Java用户界面成为可能。8.2　编写Bean的过程
编写bean并不是什么困难的技术，只不过要掌握一些新的类与接口。特别是，最简单的bean与一个Java类没任何区别，只不过其方法采用了某种严格的命名惯例罢了。
注意：有些作者说bean必须有一个默认的无参构造器，但是JavaBean规范并没有提到这一点。但是，大多数开发工具还是要求每一个bean都具备一个无参构造器，这样他们才能在没有构造参数的情况下初始化bean。
本节末尾的程序清单8-1展示了一个ImageViewer bean的代码，它给予Java开发环境的功能与前一节中提到的Visual Basic中的图像控件相同。在看这些代码的时候，请注意ImageViewerBean类，它看起来与别的类没什么区别。例如所有的访问器方法都以get开头，所有修改器方法都以set开头。很快你就会看到，开发工具就是使用这种标准命名惯例来发现属性的。举个例子，fileName是这个bean的一个属性，因为它有get和set方法。
请注意，属性与实例的域并不完全相同。在这个例子中，fileName属性是从file实例域计算得来的。属性在概念上比实例域更高一层，它们代表接口的特征，而实例域属于类的实现。
当你阅读这一章的示例时，需要牢记的是，比起我们这些简洁的例子来，真正的bean要精巧且冗长得多，这有两个原因。
1）对于非专家级的程序员，bean必须便于使用。因此，你需要暴露出很多属性，以便用户无需编程，通过可视化的设计工具即可访问bean的大多数功能。
2）同一个bean必须能在多种环境中使用。bean的行为与外部特征都必须可订制，这又要求暴露大量的属性。
有一个非常好的bean的例子：CalendarBean，它具有丰富的行为，由Kai Tdter开发（参见图8-2）。这个bean与其源代码都是可以从http://www.toedter.com/en/jcalendar免费得到的。这个bean使用户能够方便地输入日期，只需直接从显示的日历中选择日期即可。这项工作显然相当复杂，并不是人人都想从头开始编程实现它。而使用这样的一个bean，直接拖拽该bean到开发工具中，就能够利用其他人的成果了。
图8-2　一个日历bean
幸运的是，要编写一个功能丰富的bean，你必须掌握的概念并不多。这一章的示例，虽然并不简单，但是我们已经尽量保持其简单，使其能够说明必要的概念即可。
程序清单8-1　imageViewer/ImageViewerBean.java
8.3　使用Bean构造应用程序
在我们深入到编写bean的机制之前，我希望读者先看一看如何使用或者测试bean。ImageViewerBean是一个非常实用的bean，但是在开发环境之外，它无法展示出自己独特的特点。
每个开发环境都有自己的一套策略，使程序员的生活更轻松。我们用到的环境是NetBeans集成开发环境，可从http://netbeans.org获得。
在这个例子中，我们用到了两个bean，ImageViewerBean和FileNameBean。你已经看到了ImageViewerBean的代码，本章稍后我们将分析FileNameBean的代码。现在，你只需知道点击具有“...”标签的按钮就会打开一个文件选择器。8.3.1　将Bean打包成JAR文件
为了使bean能够在开发工具中使用，需要将bean以及它使用到的所有类文件都打包进一个JAR文件。与apple的JAR文件不同，bean的JAR文件需要一个manifest清单文件，用以详细说明档案中的哪些类文件是bean，需要被包括到工具箱中。例如，下面是一个针对ImageViewerBean的清单文件ImageViewerBean.mf。
注意，清单版本与bean名字之间有一个空行。
注意：我们之所以将bean全放入一个包中，是因为某些开发环境从默认包加载bean时会有问题。
如果你的bean包含多个类文件，只需在清单中指出哪些类文件是bean并且希望显示在工具箱中。例如，可以将ImageViewerBean和FileNameBean放入同一个JAR文件中，然后使用如下清单：
警告：某些开发工具对于清单很挑剔。必须确保每一行后面都没有空格，版本与bean条目之间要有空行，而且最后以一个新行结束。
制作JAR文件，可按以下步骤进行：
1）编辑清单文件。
2）在一个目录中聚齐所有需要的类文件。
3）如下运行jar工具：
例如
还可以向JAR文件中添加其他东西，例如图标的图像。我们稍后会讨论bean的图标问题。
警告：要确保你的bean所需的所有文件都在JAR文件中。特别要注意内部类，例如FileNameBean$1.class。
开发环境具有添加新bean的机制，通常是通过加载JAR文件实现的。下面是向NetBeans（版本7）导入bean的过程。
编译ImageViewerBean和FileNameBean类，将它们打包成JAR文件。然后启动NetBeans并按以下步骤进行：
1）从菜单中选择Tools→Palette→Swing/AWT Component。
2）点击Add from JAR按钮。
3）在文件对话框中，转到ImageViewerBean目录并选择ImageViewerBean.jar。
4）这时会弹出一个对话框，列出了在JAR文件中发现的所有bean。选择ImageViewerBean。
5）最后，它会问你想把这个bean放到哪一个选项板中。请选择Bean。（还有其他一些用于Swing构件、AWT构件以及其他构件的选项板。）
6）检查一下Bean选项板，它现在包含了一个用图标表示的新bean。但是，该图标只是默认图标，不过稍后你会看到如何为bean添加图标。
对FileNameBean重复以上步骤。现在你就可以将这些bean组合到应用程序中了。8.3.2　在开发环境中组合Bean
基于构件开发的方法允许我们用预制的构件经过最少量的编程来组成应用程序。在本节中，你将看见如何用ImageViewerBean和FileNameBean构件来组成应用程序。
在NetBeans中，请从菜单中选择File→New Project，这时会弹出一个对话框。然后选择Java，之后是Java Application（参见图8-3）。
图8-3　创建一个新工程
点击Next按钮。在下面的屏幕中，为你的应用程序设置一个名字（例如ImageViewer），然后点击Finish按钮。现在，可以看到左边是一个工程视图，中间是源代码编辑器。
鼠标右击工程视图中的工程名，从菜单中选择New→JFrame Form（参见图8-4）。
图8-4　创建一个表单视图
这时会弹出一个对话框。键入frame类的名字（例如ImageViewerFrame），然后点击Finish按钮。然后你就得到了一个表单编辑器，它带有一个空白的窗体。向要想在该表单中添加一个bean，可以从位于表单编辑器右侧的选项板中选择bean，然后点击框架。
图8-5展示了将一个ImageViewerBean添加到窗体上的效果。
图8-5　添加一个bean
如果看一看代码编辑器窗口，就会发现源代码中包括了将bean对象添加到窗体中的Java指令（参见图8-6）。源代码被可怕的警告括了起来，说明你不应该编辑它。在你修改表单时，如果开发环境更新了代码，那么任何的编辑工作都可能会丢失。
图8-6　为添加bean而生成的源代码
注意：在构建应用程序的时候，并不要求开发环境去更新源代码。开发环境可以在你进行编辑的时候生成源代码，将你定制的bean序列化，或者可能产生一个与你的构建活动完全不同的描述。
例如，http://java.net/projects/bean-builder上实验性的Bean Builder就可以让你在设计GUI应用程序时无需编写任何代码。
JavaBean机制并没有对开发工具强加什么实现策略。它旨在向开发工具提供bean的信息，开发工具可以选择按自己的方式使用这些信息。
现在让我们回到表单视图，点击表单中的ImageViewerBean。右手边是一个属性检查器，列出了bean各项属性的名字与当前值。这是基于构件的开发工具的核心部分，因为在设计阶段设置属性就是在设置一个构件的初始状态。
例如，可以修改图像bean所使用的标签的text属性，而这只需在属性检查器中键入一个新的名字即可。改变text属性很简单，只需在文本区中键入一个字符串。试着将标签文本改为“Hello”，以验证一下表单是否会立刻更新以反映你的修改（参见图8-7）。
图8-7　在属性检查器中改变一个属性
注意：当改变某个属性的设置时，NetBeans环境会更新源码以反映你的动作。举例来说，如果将text字段设置为Hello，那么
这条语句就会被添加到initComponents方法中。我们曾经提到过了，其他的开发工具可能采用不同的策略来记录属性的设置。
属性不一定非要是字符串，它可以是任何Java类型的值。为了使设置任何类型的属性成为可能，开发工具采用了专门的属性编辑器。（属性编辑器或者来自于开发工具，或者由bean的开发者提供。在本章中你会看到如何开发自己的属性编辑器。）
下面我们以foreground属性为例，来看看一个简单的属性编辑器是如何工作的。该属性的类型为Color，你可以看到一个颜色编辑器，它带有一个包含[0，0，0]字符串的文本域，以及一个标记为“...”的按钮，这个按钮可以启动一个颜色选择器。接下来试一下改变前景颜色，注意，你立刻就能看到属性值的变化：标签的文本颜色改变了。
还有更有趣的，试一试在属性检查器中为图像文件这个属性选择一个文件名，一旦这么做了，ImageViewerBean就会自动显示该图像。
注意：如果仔细看一看NetBeans中的属性检查器，就会发现大量奇怪的属性，例如focusCycleRoot和iconTextGap。它们继承自父类JLabel，后面将会看到如何在属性检查器中隐藏它们。
接下来继续开发我们的应用程序，将一个FileNameBean对象放在窗体中。现在，我们希望当FileNameBean的fileName属性发生变化时，图像即被加载。这可以通过一个PropertyChange事件来实现，我们稍候会讨论此类事件。
想要针对该事件做出反应，请选择FileNameBean，然后在属性检查器中选择Events标签。然后点击propertyChange旁边的“...”按钮，这时会出现一个对话框，说明该事件还没有相关联的处理操作。点击该对话框的Add按钮，要求填入一个方法名（参见图8-8），输入loadImage。
图8-8　向bean中添加一个事件
现在来看看代码编辑器，事件处理代码已经被添加了进来，而且生成了一个新方法：
将如下代码添加到该方法中：
然后编译并执行这个窗体类。现在，你就有了一个完整的图像浏览器应用程序。点击“...”按钮，选择一个图像文件，该图像就会显示在图像浏览器中。
图8-9　图像浏览器应用
这个过程说明，你可以通过bean来创建Java应用程序，而这只需设置一些属性，并为事件处理提供很少量的代码。8.4　Bean属性与事件的命名模式
这一节将介绍设计bean的一些基本规则。首先，需要强调一下，并没有什么通用的bean类能够供你继承。可视化的bean直接或间接地继承自Component类，但是非可视化的bean并不必继承任何特别的类。请记住，bean就是一个在开发工具中可操作的类。开发工具并不会根据某个类的父类来决定它是否是一个bean类，开发工具要做的只是分析bean中方法的名字。要想顺利通过此项分析，bean方法的命名就必须遵守某些模式。
注意：确实有一个java.beans.Beans类，但是它所有的方法都是静态的。继承它没有什么意义，即便如此，你会发现它偶尔也会被用到，估计是为了“清晰度”更好。很明显，因为一个bean不能同时继承Beans和Component，因此这个方法对可视化bean不起作用。实际上，Beans类包含的方法被设计为由开发工具调用，举例来说，在设计期间或运行期间，用来检查工具是否正在运行中。
其他用于可视化设计环境的语言，例如Visual Basic和C#，都有诸如“Property”和“Event”之类的专有关键字，用以直接表达对应的概念。对于支持可视化编程，Java规范的设计者决定不增加关键字。因此，他们需要一点变通，以使开发工具能够分析bean，了解bean的属性或事件。事实上，我们有两种变通的机制。如果bean的开发者为属性与事件采用了标准的命名模式，那么开发工具就可以使用反射机制，从而了解bean打算暴露出来的属性和事件。还有一种方法，bean的开发者可以提供一个bean的信息类，由它告诉开发工具bean的属性和事件。我们决定采用命名模式，因为它们更易于上手。你将会看到，本章稍后还介绍了如何提供bean的信息类。
注意：虽然相关文档称这些标准的命名模式为“设计模式”，但这些其实只是命名习惯罢了，与面向对象编程中用到的设计模式没有什么关系。
属性的命名模式非常简单：
像上面这样成对的方法就对应于一个读/写属性。
例如我们的ImageViewerBean，它只有一个读/写属性（表示要浏览的文件名），对应于以下这对方法：
如果有一个get方法，但是没有相应的set方法，说明你定义了一个只读的属性。与此对应，只有set方法而没有相应的get方法，即是定义了一个只写的属性。
注意：你创建的get和set方法能够做的可不仅仅局限于获取或设置一个私有数据域。与其他Java方法一样，它们可以执行任意的动作。举例来说，ImageViewerBean类的setFileName方法不仅设置了fileName数据域的值，而且还打开了此文件并加载了图像。
注意：在Visual Basic和C#中，属性也伴随着get和set方法。然而，在这两种语言中，你需要显式地定义属性，而不是让开发工具通过分析方法的名字来猜测程序员的意图。在这些语言中，属性还有一个优点：在赋值操作的左边使用属性的名字，就会自动调用set方法。在表达式中使用属性的名字，会自动调用get方法。例如，在Visual Basic中你可以这样写
以代替
而这正是Java所采用的语法，不过Java语言设计者认为，用看起来是在访问域的语法来隐藏方法调用的主意并不高明。
get/set命名模式有一个例外，就是布尔类型的属性应该使用is/set命名模式，如下所示：
例如，一个动画可能有一个running属性，它有两个方法：
setRunning方法可以启动和停止动画，isRunning方法用来报告它当前的状态。
注意：布尔属性的访问器使用get前缀也是合法的（例如getRunning），不过最好使用is前缀。
请当心方法名中的大写字母，JavaBean规范的设计者决定，虽然我们例子中的get和set方法包含的是大写F（getFileName，setFileName），但属性名应该是fileName，使用小写的f。bean分析器会执行一个称为字母小写化（decapitalization）的步骤，推导出属性的名字。（也就是get或set之后的第一个字符被转为小写字母。）这个过程的合理之处在于，它使得方法和属性的名字对于程序员来说更为自然。
然而，如果开头两个字母都是大写字母（例如getURL），那么属性的第一个字母就不会被改成小写字母。毕竟，如果属性名为uRL会显得很可笑。
注意：如果你的类有一对get和set方法，但是它们与你希望用户在属性检查器中操作的属性并不对应，那你该怎么办呢？如果是你自己的类，你当然可以重命名你的属性以避免这种情况。然而，如果你是继承自其他类，那么你就继承了父类中的方法名。举例来说，如果你的bean继承自JPanel或JLabel，这种情况就有可能会发生，因为属性检查器会列出大量你不感兴趣的属性。在本章稍后你会看到，如何通过提供bean信息来改写自动发现属性的过程。在bean的信息类中，可以为你的bean专门指定应该暴露的属性。
对于事件，命名模式也同样简单。当你提供用来添加或移除事件监听器的方法时，bean的开发环境能够推导出你的bean将要生成的事件。所有事件类的名字必须以Event结尾，且必须继承自EventObject类。
假设你的bean会生成EventNameEvent类型的事件，那么监听器接口必须命名为EventNameListener，同时，添加和删除监听器的方法必须命名为：
如果你浏览一下ImageViewerBean的代码，就会看到它没有要暴露的事件。然而，很多Swing构件都会产生事件，而它们都遵循此命名模式。例如，AbstractButton类会产生ActionEvent对象，而且它具有以下方法，用来管理ActionListener对象：
警告：如果你的事件类不继承EventObject的话，你的代码也可能可以通过编译，因为你的代码可能没有用到EventObject类中的方法。然而，你的bean将会神秘地出现故障，因为自省机制（introspection mechanism）无法识别出你的事件。8.5　Bean属性的类型
一个复杂的bean会具有多种不同种类的属性和事件。属性可以很简单，就像你在ImageViewerBean和FileNameBean中看到的fileName那样；也可以很精巧复杂，例如我们后面的例子中会有颜色值，甚至数据点的数组。JavaBean规范允许四种类型的属性，我们将用不同的示例来分别说明它们。8.5.1　简单属性
简单属性只具有一个单独的值，例如一个字符串或者一个数字。ImageViewer的fileName属性就是一个简单属性的例子。编写简单属性非常容易：就使用我们前面讲过的set/get命名惯例即可。例如，可以参看程序清单8-1的代码，它实现了一个简单的字符串属性：
8.5.2　索引属性
索引属性指定了一个数组。使用索引属性，需要提供两对get和set方法：一对用于数组，一对用于数组内的单个元素。它们的命名必须遵守下面的模式：
例如，FileNameBean对于文件扩展名使用了索引属性。它提供了四个方法：
setPropertyName（int i，Type x）方法不能用来扩充数组。要想扩充数组的容量，必须手动创建一个新的数组，然后将它传递给setPropertyName（Type[]x）方法。8.5.3　绑定属性
绑定属性会在属性值发生变化时，通知所有相关的监听器。举例来说，FileNameBean中的fileName属性就是一个绑定属性。一旦文件名发生变化，就会自动通知ImageViewerBean，然后由它加载新的文件。
为了实现一个绑定属性，必须实现两个机制。
1）无论何时，只要属性的值发生了变化，该bean必须发送一个PropertyChange事件给所有已注册的监听器。该变化可能发生在调用set方法时，或者某个其他方法（诸如“...”按钮的动作监听器）改变属性值时。
2）为了使感兴趣的监听器能够进行注册，bean必须实现以下两个方法：
我们还推荐（但不是必需的）提供下面的方法。
java.beans包有一个很实用的类，叫做PropertyChangeSupport，它能够为你管理监听器。要使用这个类，需要添加一个类型为这个类的实例域：
这样就可以将添加和移除属性改变监听器的任务交给这个对象了。
无论何时，只要属性的值发生了变化，就应该使用PropertyChangeSupport对象的firePropertyChange方法，它会将一个事件发送给所有已注册的监听器。该方法有三个参数：属性的名字、旧的值以及新的值。下面是典型的用于绑定属性的设置器的样本代码：
要激发一个对索引属性的修改，可以调用：
提示：如果你的bean继承自某个类，而这个类最终继承自Component类，那么就不必实现addPropertyChangeListener、removePropertyChangeListener和getProperty-ChangeListeners方法了。因为这些方法在超类Component中已经实现了。在属性变化时，要想通知监听器，只需调用超类Component中的firePropertyChange方法。但是，它不支持对索引属性修改的激发。
当属性的值发生变化时，如果其他的bean也想收到通知，那么它们必须实现PropertyChange Listener接口。该接口只有一个方法：
PropertyChangeEvent对象持有属性的名字，以及旧值与新值，这些内容可以用getPropertyName、getOldValue和getNewValue方法获得。
如果属性的类型不是类类型，那么属性值对象就是常用的包装器类的实例。8.5.4　约束属性
一个约束属性是指它受到某种约束，即任何监听器都可以“否决”它提出的改变，强迫其还原旧的设置。Java库中只有很少的一些约束属性的例子，其中一个是JInternalFrame类的closed属性。如果某人在内部窗体上调用了setClosed（true）方法，那么它所有的VetoableChangeListeners都会收到通知。如果它们中的任何一个抛出PropertyVeto-Exception异常，那么closed属性就不会改变，而且setClosed方法也会抛出同样的异常。特别是，如果某个VetoableChangeListener中的内容还没有保存，它就可以否决关闭窗体的提议。
要构建一个约束属性，你的bean必须包含下面两个方法，以管理VetoableChange-Listener对象：
它还应该有一个方法用来获取所有的监听器：
为了管理属性改变监听器，我们有一个很方便实用的类，同样，为了管理那些可否决修改的监听器，我们也有一个非常方便实用的类，叫做VetoableChangeSupport。你的bean应该包含该类的一个对象。
于是，增加和移除监听器的工作就全都可以交给这个对象代理了。例如：
为了更新一个约束属性，bean要经历以下三个步骤：
1）将修改属性值的意图通知所有可否决修改的监听器。（使用VetoableChangeSupport类的fireVetoableChange方法。）
2）如果所有可否决修改的监听器都没有抛出PropertyVetoException，就更新属性的值。
3）通知监听属性改变的所有监听器，确认有一个改变发生。
例如，
直到所有已注册的可否决修改的监听器都同意你提议的修改之后，属性的值才会真正改变，这一点很重要。相反地，可否决修改的监听器决不应该假设它同意的修改确实会发生。当某个修改确实发生的时候，惟一可靠的通知方式就是通过属性改变监听器。
注意：如果你的bean扩展自JComponent类，就不需要一个单独的VetoableChange Support对象，直接调用JComponent超类的fireVetoableChange方法即可。注意，你不能对JComponent中的某个具体的属性安装可否决修改监听器，你需要监听所有的可否决修改。
我们就要结束关于JavaBean属性的讨论了，接下来是FileNameBean的完整代码（见程序清单8-2）。FileNameBean有一个索引属性extensions，和一个受约束的filename属性。因为FileNameBean继承自JPanel类，我们就不必显式地使用PropertyChangeSupport对象了，可以直接依靠JPanel类的能力来管理属性改变监听器。
程序清单8-2　filePicker/FilePickerBean.java
java.beans.PropertyChangeListener 1.1
·void propertyChange（PropertyChangeEvent event）
当发生了一个属性改变事件时该方法会被调用。
java.beans.PropertyChangeSupport 1.1
·PropertyChangeSupport（Object sourceBean）
构建一个PropertyChangeSupport对象，用来管理对给定bean的绑定属性的变化感兴趣的监听器。
·void addPropertyChangeListener（PropertyChangeListener listener）
·void addPropertyChangeListener（String propertyName，Property-ChangeListener listener）1.2
注册一个监听器，它对所有的绑定属性发生的改变都感兴趣，或者只是为指定名字的绑定属性注册一个监听器。
·void removePropertyChangeListener（PropertyChangeListener listener）
·void removePropertyChangeListener（String propertyName，Property-ChangeListener listener）1.2
移除一个之前注册过的属性变化监听器。
·void firePropertyChange（String propertyName，Object oldValue，Object newValue）
·void firePropertyChange（String propertyName，int oldValue，int newValue）1.2
·void firePropertyChange（String propertyName，boolean oldValue，boolean newValue）1.2
向已注册的监听器发送一个PropertyChangeEvent。
·void fireIndexedPropertyChange（String propertyName，int index，Object oldValue，Object newValue）5.0
·void fireIndexedPropertyChange（String propertyName，int index，int oldValue，int newValue）5.0
·void fireIndexedPropertyChange（String propertyName，int index，boolean oldValue，boolean newValue）5.0
向已注册的监听器发送一个IndexedPropertyChangeEvent。
·PropertyChangeListener[]getPropertyChangeListeners（）1.4
·PropertyChangeListener[]getPropertyChangeListeners（String propertyName）1.4
获取对所有绑定属性的改变都感兴趣的监听器，或者只是获取指定名字属性的监听器。
java.beans.PropertyChangeEvent 1.1
·PropertyChangeEvent（Object sourceBean，String propertyName，Object oldValue，Object newValue）
构建一个新的PropertyChangeEvent对象，用来描述给定属性已经由oldValue变为newValue。
·String getPropertyName（）
返回属性的名字。
·Object getNewValue（）
返回该属性的新值。
·Object getOldValue（）；
返回该属性的旧值。
java.beans.IndexedPropertyChangeEvent 5.0
·IndexedPropertyChangeEvent（Object sourceBean，String propertyName，Object oldValue，Object newValue，int index）
构建一个新的IndexedPropertyChangeEvent对象，用来描述给定属性在指定索引位置的元素对象由oldValue变为newValue。
·int getIndex（）
返回发生变化的元素的索引号。
java.beans.VetoableChangeListener 1.1
·void vetoableChange（PropertyChangeEvent event）
当属性将发生变化时调用此方法。如果该变化不可接受，会抛出PropertyVeto-Exception异常。
java.beans.VetoableChangeSupport 1.1
·VetoableChangeSupport（Object sourceBean）
构建一个PropertyChangeSupport对象，用来管理参数bean上的约束属性的监听器。
·void addVetoableChangeListener（VetoableChangeListener listener）
·void addVetoableChangeListener（String propertyName，Vetoable-ChangeListener listener）1.2
注册一个监听器，它对所有约束属性的变化都感兴趣，或者只为给定名字的约束属性注册一个监听器。
·void removeVetoableChangeListener（VetoableChangeListener listener）
·void removeVetoableChangeListener（String propertyName，VetoableChangeListener listener）1.2
移除之前注册的一个可否决修改的监听器。
·void fireVetoableChange（String propertyName，Object oldValue，Object newValue）
·void fireVetoableChange（String propertyName，int oldValue，int newValue）1.2
·void fireVetoableChange（String propertyName，boolean oldValue，boolean newValue）1.2
向已注册的监听器发送一个VetoableChangeEvent事件。
·VetoableChangeListener[]getVetoableChangeListeners（）1.4
·VetoableChangeListener[]getVetoableChangeListeners（String propertyName）1.4
获取对所有约束属性的变化都感兴趣的监听器，或者返回指定名字的绑定属性的监听器。
java.awt.Component 1.0
·void addPropertyChangeListener（PropertyChangeListener listener）1.2
·void addPropertyChangeListener（String propertyName，Property-ChangeListener listener）1.2
注册一个监听器，它对所有绑定属性的变化都感兴趣，或注册一个只对指定名字的绑定属性感兴趣的监听器。
·void removePropertyChangeListener（PropertyChangeListener listener）1.2
·void removePropertyChangeListener（String propertyName，Property-ChangeListener listener）1.2
移除之前注册的一个属性改变监听器。
·void firePropertyChange（String propertyName，Object oldValue，Object newValue）1.2
向已注册监听器发送一个PropertyChangeEvent事件。
javax.swing.JComponent 1.2
·void addVetoableChangeListener（VetoableChangeListener listener）
注册一个监听器，对所有约束属性的变化感兴趣，或者注册一个只对指定名字的约束属性感兴趣的监听器。
·void removeVetoableChangeListener（VetoableChangeListener listener）
移除之前注册的一个可否决修改的监听器。
·void fireVetoableChange（String propertyName，Object oldValue，Object newValue）
向已注册监听器发送一个VetoableChangeEvent事件。
java.beans.PropertyVetoException 1.1
·PropertyVetoException（String message，PropertyChangeEvent event）
生成一个新的PropertyVetoException。
·PropertyChangeEvent getPropertyChangeEvent（）
返回PropertyChangeEvent，用来构建PropertyVetoException异常。8.6　BeanInfo类
如果你的bean中的方法使用了标准的命名模式，那么开发工具就可以使用反射机制来确定bean的特征，例如属性以及事件。这个过程使得bean的编程非常容易，但是，命名模式最终也有其局限。当你的bean变得复杂的时候，bean就会有一些特征无法通过命名模式来揭示。还有我们已经提到过的，很多bean都具有不应该对应到bean属性的get/set方法对。
如果你需要更灵活的方式来描述有关bean的信息，可以定义一个实现了BeanInfo接口的对象。只要你提供了这样的对象，开发工具就会通过询问它来识别你的bean具有的特性。
bean信息类的名字必须是通过将BeanInfo添加到bean的名字后面而构成的。举例来说，与ImageViewerBean类关联的bean信息类必须命名为ImageViewerBeanBeanInfo。bean信息类还必须与bean在同一个包中。
一般情况下，你的类并不需要实现BeanInfo接口中的所有方法。但是，你应该继承SimpleBeanInfo类，它很实用，提供了BeanInfo接口中所有方法的默认实现。
使用BeanInfo类最常见的原因是为了获取对bean属性的控制权。这样的话，你只需提供属性名和所属的bean类，就可以为每个属性构建一个PropertyDescriptor。
然后实现BeanInfo类中的getPropertyDescriptors方法，使它返回一个数组，其中包含了所有的属性描述符。
例如，假设ImageViewerBean想隐藏它由超类JLabel继承而来的所有属性，只暴露出fileName属性，那么下面的BeanInfo类就可以做到这一点：
其他方法还可以返回EventSetDescriptor和MethodDescriptor数组，但是它们不常用。如果其中一个方法返回null（SimpleBeanInfo中的方法就是如此），那么标准命名模式就派上用场了。无论如何，如果你覆盖了其中一个方法，使它返回一个非null数组，那么该数组必须包含所有的属性、事件、或者方法。
注意：有的时候，读者可能希望编写具有通用性的代码，用来发现任意bean的属性或事件。这时可以调用Introspector类中的静态方法getBeanInfo。Introspector会构建出一个BeanInfo类，而它完整地描述了bean，而你则可以参考这个BeanInfo类中携带的信息。
BeanInfo接口中另一个有用的方法是getIcon，可以利用它给你的bean自定义一个图标，开发工具将在选项板中显示该图标。实际上，你可以指定四个独立的图标位图。BeanInfo接口有四个常量，涵盖了各种标准尺寸：
在下面的类中，我们使用了SimpleBeanInfo类中的便利方法loadImage来加载一个图标的图像：
java.beans.Introspector 1.1
·static BeanInfo getBeanInfo（Class<？>beanClass）
获取指定类的bean信息。
java.beans.BeanInfo 1.1
·PropertyDescriptor[]getPropertyDescriptors（）
返回bean属性的描述符。如果返回null，则表示应该用命名规则来查找属性。
·Image getIcon（int iconType）
返回一个图像对象，它在工具箱、工具条，或类似的地方表示该bean。如前所述，有四个常量表示四种标准类型的图标。
java.beans.SimpleBeanInfo 1.1
·Image loadImage（String resourceName）
返回与资源相关联的一个图像对象文件。资源名是一个路径名，该路径是相对于包含bean信息类的目录的路径。
java.beans.FeatureDescriptor 1.1
·String getName（）
·void setName（String name）
获得或设置该特性在程序中的名字。
·String getDisplayName（）
·void setDisplayName（String displayName）
获取或设置该特性用以显示的名字。默认值是getName的返回值。然而，当前还没有显式地支持以多种本地语言提供属性的名字。
·String getShortDescription（）
·void setShortDescription（String text）
获取或设置一个字符串，开发工具使用它为该特性提供一个简短的描述。默认值是getDisplayName的返回值。
·boolean isExpert（）
·void setExpert（boolean b）
获取或设置一个专家标记，开发工具可用它来决定是否对普通用户隐藏某个特性。
·boolean isHidden（）
·void setHidden（boolean b）
获取或设置一个标记，开发工具根据它决定是否应该隐藏此特性。
java.beans.PropertyDescriptor 1.1
·PropertyDescriptor（String propertyName，Class<？>beanClass）
·PropertyDescriptor（String propertyName，Class<？>beanClass，String getMethod，String setMethod）
构建一个PropertyDescriptor对象。在自省时如果发生错误，该方法会抛出IntrospectionException异常。第一个构造器假设你遵守了get和set方法的标准命名惯例。
·Class<？>getPropertyType（）
针对属性的类型返回一个Class对象。
·Method getReadMethod（）
·Method getWriteMethod（）
返回获取和设置属性的方法。
java.beans.IndexedPropertyDescriptor 1.1
·IndexedPropertyDescriptor（String propertyName，Class<？>beanClass）
·IndexedPropertyDescriptor（String propertyName，Class<？>beanClass，String getMethod，String setMethod，String indexedGetMethod，String indexedSetMethod）
为带索引的属性构造一个IndexedPropertyDescriptor。第一个构造器假设你遵守了get和set方法标准的命名惯例。
·Method getIndexedReadMethod（）
·Method getIndexedWriteMethod（）
返回获取和设置属性中的索引值的方法。8.7　属性编辑器
如果向bean添加一个整数或字符串的属性，它会自动在bean的属性检查器中显示出来。但是，如果你添加的属性的值不能在文本域中方便地编辑，例如日期或Color，那会怎么样呢？这就需要你提供一个单独的构件，用户可以通过它来设定属性值。这样的构件就称为属性编辑器。例如，一个日期对象的属性编辑器，它可以是一个日历，用户可以滚动月份选择一个日期。而Color对象的属性编辑器则允许用户选择所需颜色的红、绿、蓝成分。
实际上，NetBeans已经提供了颜色属性编辑器。当然，还有诸如String（一个文本域）和boolean（一个检验栏）这样的基本类型的属性编辑器。
我们简要介绍一下提供一个全新的属性编辑器的过程。首先，为你的bean创建一个bean信息类，并覆盖getPropertyDescriptors方法。这个方法返回一个Property-Descriptor对象数组。为需要在属性编辑器中显示的每一个属性都创建一个对象，即使是只需要使用默认属性编辑器的那些属性也是如此。
用属性名和包含该属性的bean类，来构建一个PropertyDescriptor。
然后调用PropertyDescriptor类上的setPropertyEditorClass方法。
接下来，为你的bean的属性构建一个描述符数组。例如，我们讨论过的chart bean，它有五个属性：
·一个Color属性，graphColor。
·一个String属性，title。
·一个int属性，titlePosition。
·一个double[]属性，values。
·一个boolean属性，inverse。
程序清单8-3就是ChartBeanBeanInfo类的代码，它为这些属性指定了属性编辑器，实现了以下两个目标：
1）getPropertyDescriptors方法为每个属性返回一个描述符。title和graphColor属性使用默认的编辑器，也就是开发工具自带的字符串编辑器和颜色编辑器。
2）titlePosition、values和inverse属性使用专门的编辑器，分别是TitlePosition Editor、DoubleArrayEditor和InverseEditor。
图8-10展示了chart bean。可以看到title在顶部，它的位置也可以设置为左、中、右。values属性指明了图的值。如果inverse属性为真，那么背景就会有颜色，而图表中的柱图就为白色。你可以在本书附带的代码中找到chart bean的代码，它修改自第1卷第10章的chart applet。
图8-10　chart bean
程序清单8-3　chart/ChartBeanBeanInfo.java
java.beans.PropertyDescriptor 1.1
·PropertyDescriptor（String name，Class<？>beanClass）
构建一个PropertyDescriptor对象。
参数：name　属性的名字
beanClass　该属性所属的bean的类
·void setPropertyEditorClass（Class<？>editorClass）
设置该属性将使用的属性编辑器的类。
java.beans.BeanInfo 1.1
·PropertyDescriptor[]getPropertyDescriptors（）
返回每一个属性所对应的描述符，这些属性都是会在bean的属性检查器中显示的属性。
编写属性编辑器
在深入编写属性编辑器的机制之前，我们还应该指出一点，编辑器受开发工具的控制，而不是受bean的控制。当开发工具显示属性检查器时，它会对每个bean属性执行下面的步骤：
1）实例化一个属性编辑器。
2）向bean询问该属性的当前值。
3）然后要求属性编辑器显示该值。
属性编辑器必须提供默认的无参构造器，并实现PropertyEditor接口。通常我们都会去扩展便利的PropertyEditorSupport类，它提供了该接口中的方法的默认实现版本。
对于你所写的每个属性编辑器，都需要在三种显示和编辑属性值的方式中选择一种：
·作为文本字符串处理（定义getAsText和setAsText）。
·作为选择域处理（定义getAsText、setAsText和getTags）。
·通过绘制按照图形化的方式处理（定义isPaintable、paintValue、supports-CustomEditor和getCustomEditor）。
下面我们将进一步讨论这些不同的选择。
1.简单属性编辑器
简单属性编辑器使用的是文本字符串，需要覆盖setAsText和getAsText方法。例如对于我们的图表bean，可以设置它的一个属性来决定标题显示的位置：居左、居中或居右。这些选择其实是作为枚举来实现的。
当然，我们不会让它们就以大写字符串LEFT、CENTER、RIGHT的形式显示在文本域中，除非我们想进入“用户接口恐怖名人堂”。因此，我们定义了一个属性编辑器，它的getAsText方法会把让开发者喜欢的字符串取出来。
理想情况下，这些字符串应该按照当前的Locale进行显示，而不一定非要是英语的，但是我们将这个任务留给读者作为一个练习去完成。
反过来，我们需要提供一个方法将文本字符串转换回属性值：
如果我们只提供这两个方法，属性检查器就会提供一个文本框，它是通过对getAsText进行调用而初始化的，而在我们完成编辑时，setAsText会被调用。当然，在上述情况中，这个属性编辑器对于titlePosition属性来说并不是一个很好的选择，当然，除非我们正打算争夺进入“用户接口羞耻名人堂”的入场券。更好的办法是在组合框中将所有可能的设置都显示出来（参见图8-11）。PropertyEditorSupport类给出了一个简单的机制，用于表明组合框才是恰当的显示方法。我们只需编写一个getTags方法，它返回一个字符串数组。
getTags方法默认返回null，表示文本框才是适合用来编辑该属性值的方式。即便提供了getTags方式，我们仍需提供getAsText和setAsText方法。getTags方法直接指出哪些字符串应该呈现给用户，而getAsText和setAsText方法负责在字符串与该属性的数据类型（可以是字符串、整数，或某种完全不同的类型）之间的转换工作。
图8-11　自定义的属性编辑器
最后，属性编辑器应该实现getJavaInitializationString方法。通过使用这个方法，可以向开发工具提供将一个属性设置为其当前值的Java代码，开发工具将由用这些代码构成的字符串进行自动代码生成。下面是用于TitlePositionEditor的方法：
这个方法将返回诸如"chart.ChartBean.Position.LEFT"这样的字符串。试着在NetBeans中使用它：如果你编辑titlePosition属性，那么NetBeans就会插入像下面这样的代码：
本例中，这个代码有些麻烦，因为ChartBean.Position.class.getName（）的返回值是字符串"chart.ChartBean$Position"，我们需要用句点来替换$，并追加在枚举值上调用toString方法得到的结果。
注意：如果一个属性有自定义的编辑器，而这个编辑器没有实现getJavaInitialization String方法，那么NetBeans就不了解应该如何生成代码，于是就产生了具有？？？参数的设置方法。
程序清单8-4包含了此属性编辑器的完整代码。
程序清单8-4　chart/TitlePositionEditor.java
2.基于GUI的属性编辑器
更复杂的属性类型则不应该作为文本来编辑，而是应该在属性检查器中显示图形化的表示，而属性检查器的这个小区域在其他情况下持有的是一个文本框或者组合框。当用户点击该区域时，会弹出一个自定义的编辑器对话框（参见图8-12）。这个对话框包含一个由属性编辑器提供的用来编辑属性值的构件，还有由开发工具提供的各种按钮。在本例中，定制的编辑器非常简约，只包含一个按钮。本书附带的代码包含一个用于编辑图表值的更精致的编辑器。
要开发一个基于GUI的属性编辑器，首先要告诉属性检查器你将画出属性的值，而不是使用字符串。然后覆盖PropertyEditor接口中的getAsText方法，使之返回null，并且令isPaintable方法返回true。
然后，要实现paintValue方法。它接收一个Graphics上下文和相应的矩形区域作为参数，你将在该矩形区域内画图。请注意，该矩形通常比较小，因此你不能使用很复杂的表达方式。我们只是画了两个图标中的一个（参见图8-11）。
图8-12　一个自定义的编辑器对话框
当然，这个图形化的表示是不可编辑的。用户必须点击它，才能弹出一个自定义的编辑器。
通过覆写PropertyEditor接口的supportsCustomEditor方法，使之返回true，就可以表示你将使用一个自定义的编辑器。
接下来，由PropertyEditor接口的getCustomEditor方法构造并返回一个自定义编辑器类的对象。
程序清单8-5展示了在属性检查器中显示当前属性值的InverseEditor的代码。
程序清单8-6展示了用于修改这个值的自定义的编辑器面板的代码。
程序清单8-5　chart/InverseEditor.java
程序清单8-6　chart/InverseEditorPanel.java
java.beans.PropertyEditor 1.1
·Object getValue（）
返回属性的当前值，基本类型会被封装到对象包装器中。
·void setValue（Object newValue）
设置属性为新值，基本类型必须封装到对象包装器中。
参数：newValue　对象的新值，应该是一个该属性可以拥有的新创建的对象。
·String getAsText（）
覆盖该方法，使其返回属性当前值的一个字符串表示。默认情况下返回null，表示该属性不能表示成字符串。
·void setAsText（String text）
覆盖该方法，让其用通过解析文本获得的新值来设置属性。如果这个文本表示的不是合法值，或者这个属性不能用字符串表示，则有可能会抛出IllegalArgumentException。
·String[]getTags（）
覆盖该方法，让其返回属性值所有可能的字符串表示，以使得它们可以显示在选择框中。默认情况下返回null，表示不存在字符串值的有限集。
·boolean isPaintable（）
如果该类使用paintValue方法来显示属性，就应该覆写此方法，使之返回true。
·void paintValue（Graphics g，Rectangle bands）
应该覆写该方法，使其通过将属性值画入一个图形上下文环境中来表示该属性的值，该图形上下文环境位于属性检查器所用构件上指定的位置。
·boolean supportsCustomEditor（）
如果该属性编辑器有一个自定义编辑器，那么就应该覆写该方法，使其返回true。
·Component getCustomEditor（）
应该覆写该方法，使其返回一个构件，该构件包含了用来编辑属性值的自定义的GUI。
·String getJavaInitializationString（）
应该覆写该方法，返回一个Java代码字符串，它将用来生成初始化属性值的代码。例如"0"，"new Color（64，64，64）"。8.8　定制器
属性编辑器是为了让用户一次设置一个属性。但是，如果某个bean的特定属性与其他属性有关联时，它应该向用户提供更友好的方法，以便用户在同一时刻能够编辑多个属性。要使用这种特性，你需要提供一个定制器来取代多个属性编辑器，或者在多个属性编辑器的基础上增加一个定制器。
此外，某些bean可能会有一些特性并不希望作为属性暴露出来，因此也就不能通过属性检查器来编辑。对于这样的bean，定制器就很关键了。
在这一节的程序清单中，我们将为图表bean开发一个定制器。这个定制器允许你在一个对话框中设置多个属性，如图8-13所示。
要想为你的bean添加一个定制器，必须提供一个BeanInfo类，并且覆盖其getBeanDescriptor方法，例如：
注意，定制器不需要任何命名模式。不过，通常习惯于将定制器命名为BeanName-Customizer。
图8-13　ChartBean的定制器
在下一节，你会看到如何实现一个定制器。
java.beans.BeanInfo 1.1
·BeanDescriptor getBeanDescriptor（）
返回一个BeanDescriptor对象，它描述了bean的特性。
java.beans.BeanDescriptor 1.1
·BeanDescriptor（Class<？>beanClass，Class<？>customizerClass）
为具有定制器的bean构建一个BeanDescriptor对象。
参数：beanClass　bean对应的Class对象
customizerClass　bean的定制器对应的Class对象
编写一个定制器类
任何定制器类都必须有一个无参构造器、扩展Component类，并实现Customizer接口。该接口只有三个方法：
·setObject方法，包含一个参数用于指定要被定制的bean。
·addPropertyChangeListener和removePropertyChangeListener方法，用来管理监听器集合，当定制器中的属性发生变化时就会通知这些监听器。
只要是用户改变了某个属性，就应该通过广播一个PropertyChangeEvent事件来更新目标bean的可视化外观，而不要仅仅只在用户结束定制过程的时候才更新bean。这是个不错的主意。
与属性编辑器不同，定制器不会自动显示出来。在NetBeans中，必须右键点击bean，并且选择Customize菜单选项才会弹出定制器。在这一时刻，开发工具会调用定制器上的setObject方法。请注意，你的定制器就是这样被创建出来的，先创建定制器，然后再与你的bean建立实际的关联。因此，在定制器中，你不能假设拥有与bean状态有关的任何信息。
因为定制器通常用来向用户显现许多选项，所以由标签面板构成的用户界面一般比较便于使用。我们就是采用这样方法，使定制器继承自JTabbedPane类。
我们的定制器在三个面板中收集了以下信息：
·图形颜色和反转模式；
·标题与标题的位置；
·数据点。
当然，开发这一类的用户界面可能很乏味，我们的例子用了100多行的代码，只是为了在构造器中装配界面。然而，这项工作只需要普通的Swing编程技术就够了，而我们也不想在这里多谈其中的细节。
有一个技巧很值得学习：你经常需要在一个定制器中编辑属性的值，与其为了设置特定类的某个属性而实现一个新的接口，不如直接找到一个已经存在的属性编辑器，然后把它添加到你的用户界面中。以我们的ChartBean2定制器为例，我们需要设置图形颜色。由于我们知道BeanBox已经有一个很好的颜色属性编辑器，于是我们可以找到并复用它：
一旦我们排列好所有的构件，就可以在setObject方法中初始化它们的值。当定制器显示出来的时候，会调用setObject方法。它的参数是将要被定制的bean。接下来，我们保存了该bean的引用，在稍后通知bean属性有所改变时需要用到它。然后，我们初始化每一个用户界面构件。下面是图表bean的定制器的setObject方法的片段，由它完成初始化的工作。
最后，我们要挂钩事件处理器，以跟踪用户的行动。无论何时，只要用户改变了某个构件的值，该构件就发出一个必须由我们的定制器来处理的事件。事件处理器必须更新属性的值，而且必须也发出一个PropertyChangeEvent事件，这样其他监听器（例如属性检查器）才能收到更新。接下来我们就按照这个过程，来完成图表bean定制器中的两个用户界面元素。
当用户输入一个新的标题时，我们希望更新title属性。于是我们给用户输入标题的文本域添加一个DocumentListener。
这个监听器的三个方法都调用定制器的setTitle方法，而setTitle方法要求bean更新属性的值，然后发出一个属性改变事件。（该更新仅对那些没有绑定的属性是必需的。）下面就是setTitle方法的代码。
当颜色属性编辑器中的值发生变化时，我们希望更新bean的图形颜色。通过将一个监听器附着到属性编辑器上，我们就可以跟踪颜色的变化。容易令人混淆的是，编辑器本身也会发出属性改变的事件。
程序清单8-7提供了图表bean定制器完整的代码。
程序清单8-7　chart2/ChartBeanCustomizer.java
java.beans.Customizer 1.1
·void setObject（Object bean）
指定要定制的bean。8.9　JavaBean持久化
JavaBean持久化是指，用JavaBean的属性来将bean存储到流中，并在之后的某个时刻，或者在另一个虚拟机中再将它们读回来。在这一点上，JavaBean的持久化与对象序列化相似。（有关序列化的更多内容参见本书第1章。）然而，它们有一个非常重要的区别：JavaBean持久化适合于长期存储。
当一个对象被序列化后，它的实例域就都被写到了一个流中。如果类的实现发生了变化，它的实例域也可能会产生变化，这样就无法直接读取包含了旧版本被序列化对象的文件。探测到版本的不同，然后在新旧数据表现之间进行转换是可以实现的。然而，这个过程非常令人厌烦，而且应该只有在无路可走的情况下才使用这种手段。简单地说，序列化不适合长期存储。基于此原因，所有的Swing构件在其文档中都有如下的信息：“警告：该类的序列化对象与未来版本的Swing不兼容。当前对序列化的支持仅适合于短期存储或者应用程序之间的RMI。”
为了解决此问题，人们发明了长期存储机制。不过最初是为了用于拖拽GUI设计工具。设计工具为了保存鼠标点击窗体、面板、按钮以及其他Swing构件的结果，于是将此结果以长期持久化的形式保存在一个文件中，而运行中的程序可以直接打开该文件。这个方法减少了令人厌烦的布局、装配Swing构件的代码。可惜的是，它并没有被广泛地实现。
JavaBean持久化背后的想法非常简单。假设你想将一个JFrame对象保存到一个文件中，以便以后可以将它取回。如果看一看JFrame类及其父类的源代码，你会看到几十个实例域。如果窗体被序列化，那么所有这些域的值都需要写下来。但是想一想窗体是如何构造的吧：
默认的无参构造器会初始化所有的实例域，然后由你设置一两个属性。如果你保存这个frame对象，JavaBean持久化机制会精确地以XML形式保存这些语句：
当该对象被读回的时候，下面这些语句会被执行：构造一个JFrame对象，将它的title和visible设置为指定值。它完全不管JFrame的内部表示是否发生了变化，它只关心你可以通过设置属性来恢复该对象。
请注意，只有那些与默认值不同的属性会保存下来。XMLEncoder制作了一个默认的JFrame，并将它的属性与被处理的窗体做比较。只有那些与默认值不同的属性，才会生成属性设置语句。这个过程称为消除冗余（redundancy elimination）。结果是，存储下来的东西通常比序列化的结果要小。（尤其是序列化Swing构件时，区别更是非常明显，因为Swing对象有很多状态，大多数从生成默认值的一刻起就从未改变过。）
当然，此方法还有一些小的技术障碍。例如，
这并不是属性设置方法，然而XMLEncoder能够处理它，它会编写下面的语句：
要将一个对象保存为流，需要使用XMLEncoder：
要将它读回来，需要使用XMLDecoder：
程序清单8-8中的程序演示了一个窗体是如何读取以及保存它自己的（参见图8-14）。当你运行该程序时，首先点击Save按钮，将此窗体保存为一个文件。然后移动原始的窗体到另一个位置，再点击Load按钮，你会看到在原来的位置弹出了另一个窗体。我们再来看一看该程序生成的XML文件。
如果仔细观察输出的XML文件，你会发现在保存窗体时，XMLEncoder完成了大量的工作。XMLEncoder生成的语句完成了以下操作：
图8-14　PersistentFrameTest程序
·设置各种不同的窗体属性：size、layout、defaultCloseOperation、title等等。
·向窗体添加按钮。
·为按钮添加动作监听器。
在这里，我们必须使用EventHandler类构建动作监听器。因为XMLEncoder不能保存任何内部类，但是它知道如何处理EventHandler对象。
程序清单8-8　persistentFrame/PersistentFrameTest.java
8.9.1　JavaBean持久化可用于任何数据
JavaBean持久化并不局限于Swing构件的存储，可以使用该机制存储任意对象的集合，只要遵守一些简单的规则即可。在下一节中，将学习如何将JavaBean持久化作为一种长期的存储形式来保存你自己的数据。
XMLEncoder内置了对下列类型的支持：
·null
·所有基本类型及其包装器类型
·枚举（从Java SE 6开始）
·String
·数组
·集合与映射表
·反射类型Class、Field、Method和Proxy
·AWT类型Color、Cursor、Dimension、Font、Insets、Point、Rectangle和ImageIcon
·AWT和Swing构件、边界、布局管理器和模型
·事件处理器
1.编写一个持久化代理（Persistence Delegate）来构造对象
如果可以通过设置属性得到每个对象的状态，那么使用JavaBean持久化就是小菜一碟。但是，在真实的程序中，总有些类无法这样工作。例如，思考一下卷I第4章中的Employee类。Employee并不是一个循规蹈矩的bean。它没有默认的无参构造器，而且它也没有setName、setSalary、setHireDay方法。要克服这个问题，可以定义一个持久化代理（persistence delegate），该代理负责生成对象的XML编码机制。
Employee类的持久化代理覆写了instantiate方法，以生成一个构造对象的表达式（expression）。
这意味着：“要重新生成oldInstance，可以调用Employee.class对象上的new方法（也就是构造器），并提供指定的参数。”参数名oldInstance容易令人误解——它就是正在被保存的实例。
安装持久性代理有两种选择：一种是将其与某个具体的XMLWriter相关联：
或者，可以设置Beaninfo的bean描述符的persistenceDelegate属性：
一旦安装了代理，就可以保存Employee对象了。例如：
该语句会生成如下的输出：
注意：你只需要控制编码的过程，但并不需要任何专门的解码方法。解码器将直接执行在输入的XML中所找到的语句和表达式。
2.由属性构造对象
如果构造器所需的参数都可以通过访问oldInstance的属性获得，那你就不必自己编写instantiate方法了。你可以直接构造一个DefaultPersistenceDelegate，然后提供属性的名字即可。
例如，下面的语句为Rectangle2D.Double类设置了持久化代理：
它告诉编码器：“要编码一个Rectangle2D.Double对象，就要取得它的x、y、width和height属性，并且用它们来调用构造器。”其结果会产生包含如下元素的输出：
如果你是类的作者，那么你甚至可以做得更好，即用@ConstructorProperties注解构造器。例如，假设Employee类有一个接受三个参数的构造器（名字、薪水和受雇日），然后我们对其进行注解：
这可以告诉编码器去调用getName、getSalary和getHireDay这三个属性获取方法，并将得到的值写入object表达式。
@ConstructorProperties注解是在Java SE 6中引入的，因此只在Java管理扩展（JMX）API中得到了应用。
3.使用工厂方法构造对象
有时，你需要保存那些通过工厂方法获得的对象，而不是通过构造器产生的对象。举例来说，思考一下你是如何得到一个InetAddress对象的：
PersistenceDelegate的instantiate方法会产生对工厂方法的一次调用。
下面是一个输出样例：
警告：安装这样的代理必须是针对具体子类，例如Inet4Address，而不能是抽象的InetAddress类！
4.构造后的工作
某些类的状态是通过调用非属性设置方法[1]来建立的，你可以通过覆写DefaultPersistence Delegate的initialize方法来处理这种状况。initialize方法会在instantiate方法之后调用，在其中你可以生成将被录在bean存档文件中的语句序列。
例如，思考一下BitSet类。为了重新生成一个BitSet对象，你要设置原对象中存在的所有位。下面的initialize方法生成了必要的语句：
输出样例如下所示：
注意：编写new Statement（bs，"set"，new Object[]{i}）会更有意义，不过，如果那样的话，XMLWriter会使用一个空的名字来设置属性，而这样的语句很不直观。
5.瞬态属性
偶尔的，类中会包含能够被XMLDecoder发现的带有getXxx和setXxx的属性，但是你不希望在存档文件中包含该属性的值。为了禁止某个属性被存档，可以在属性描述符中将它标记为transient。例如，下面的语句把DamageReporter类（在下一节你将会看到这个类的细节）的removeMode属性标记成瞬时的。
程序清单8-9中的程序演示了各种持久化代理。请记住，这个程序实际上是一个应用程序的反面教材，其中的很多类其实无需使用代理就可以存档保存。
程序清单8-9　persistenceDelegate/PersistenceDelegateTest.java
[1]即非Set Xxx方法。——编辑注8.9.2　一个JavaBean持久化的完整示例
我们将以一个完整的示例来结束有关JavaBean持久化的内容（参见图8-15）。这个应用程序为租赁车编写受损报告。租赁车的代理商负责填写出租记录，选择车的类型，用鼠标来点击轿车的受损区域，然后保存该报告。这个应用程序还可以读取已经保存的受损报告。程序清单8-10是该程序的代码。
图8-15　受损报告应用程序
这个应用程序使用了JavaBean持久化机制来保存和读取DamageReport对象（见程序清单8-11）。它演示了持久化机制的几个方面：
·属性是自动保存并恢复的。对于rental Record和carType属性而言，不需要再做任何事情。
·构造后的工作是恢复受损的位置。此持久化代理生成了调用click方法的语句。
·Point2D.Double类需要一个Default PersistenceDelegate，它负责通过x和y属性来构造一个点。
·removeMode属性（它指明了鼠标点击是在增加还是在移除受损标志）是瞬态的，因此它不应该被保存在受损报告中。
这里有一份受损报告的例子：
注意：这个示例程序并没有使用JavaBean持久化来保存应用程序的GUI。开发工具的创造者也许会对它更感兴趣，不过我们在这里讨论的是如何使用持久化机制来存储应用程序的数据。
我们以这个例子来结束JavaBean持久化机制的讨论。下面总结一下，JavaBean持久化的存档是：
·适合长期存储的。
·小而且快的。
·易于生成的。
·易于人们进行编辑的。
·是标准Java的一部分。
程序清单8-10　damageReporter/DamageReporterFrame.java
程序清单8-11　damageReporter/DamageReport.java
java.beans.XMLEncoder 1.4
·XMLEncoder（OutputStream out）
构造一个XMLEncoder，它将其输出发送到指定的流。
·void writeObject（Object obj）
存档给定的对象。
·void writeStatement（Statement stat）
向存档文件中写入给定的语句。该方法应该只在持久化代理中调用。
java.beans.Encoder 1.4
·void setPersistenceDelegate（Class<？>type，PersistenceDelegate delegate）
·PersistenceDelegate getPersistenceDelegate（Class<？>type）
设置或获取用于指定类型存档对象的代理。
·void setExceptionListener（ExceptionListener listener）
·ExceptionListener getExceptionListener（）
设置或获取异常监听器，该监听器在编码过程中发生异常时会收到通知。
java.beans.ExceptionListener 1.4
·void exceptionThrown（Exception e）
在编码或解码期间，当异常被抛出时调用该方法。
java.beans.XMLDecoder 1.4
·XMLDecoder（InputStream in）
构造一个XMLDecoder，它从给定的输入流中读取一个存档。
·Object readObject（）
从存档中读取下一个对象。
·void setExceptionListener（ExceptionListener listener）
·ExceptionListener getExceptionListener（）
设置或获取异常监听器，如果在编码过程中发生异常，该监听器会收到通知。
java.beans.PersistenceDelegate 1.4
·protected abstract Expression instantiate（Object oldInstance，Encoder out）
返回一个用于实例化对象的表达式，该对象等价于oldInstance。
·protected void initialize（Class<？>type，Object oldInstance，Object newInstance，Encoder out）
向out写入语句，将newInstance转换成一个等价于oldInstance的对象。
java.beans.DefaultPersistenceDelegate 1.4
·DefaultPersistenceDelegate（）
为带有零参数构造器的类构造一个持久化代理。
·DefaultPersistenceDelegate（String[]propertyNames）
为一个类构造一个持久化代理，这个类的构造器参数正是给定的属性的值。
·protected Expression instantiate（Object oldInstance，Encoder out）
返回一个用来调用构造器的表达式，该构造器要么不含参数，要么使用构造器中指定的属性值作为参数。
·protected void initialize（Class<？>type，Object oldInstance，Object newInstance，Encoder out）
向out写入语句，它提供了newInstance的属性设置器，并试图将其转换为等价于oldInstance的对象。
java.beans.Expression 1.4
·Expression（Object value，Object target，String methodName，Object[]parameters）
构造一个表达式，它使用给定的参数来调用target上的指定方法。表达式的结果将赋给value。如果想要调用一个构造器，那么target应该是一个Class对象，而且methodName应该是“new”。
java.beans.Statement 1.4
·Statement（Object target，String methodName，Object[]parameters）
构造一个语句，它会使用给定的参数调用target上指定的方法。
你现在已经通读了篇幅很长的有关运用Swing、AWT和JavaBeans进行GUI编程的三章。在下一章，我们将转向完全不同的主题：安全。安全已经成为Java平台的核心特征之一。由于我们生存和计算的世界变得越来越危险，因此对Java安全地透彻理解对于许多开发者来说就显得尤为重要。