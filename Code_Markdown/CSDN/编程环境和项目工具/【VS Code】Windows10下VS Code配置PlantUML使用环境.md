

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。

@[toc]

在[获取了VS Code编辑器](https://memcpy0.blog.csdn.net/article/details/117640795)之后，可以尝试在VS Code中配置PlantUML的使用环境，以便学习**设计模式**和**面向对象设计**。

---
# 0. 开始学习PlantUML
## 0.1 PlantUML是什么
PlantUML是一个用来快速绘制UML图的Java类库，支持的UML图包括：时序图、用例图、**类图**、**对象图**、活动图、组件图、部署图、状态图、定时图，还支持这些非UML图：**实体联系图**、JSON数据、YAML数据、网络图、线框图形界面、架构图、规范和描述语言（DSL）、Ditaa图、甘特图、思维导图、WBS工作分解图等。

比起[Graphviz和DOT](https://memcpy0.blog.csdn.net/article/details/119459490)来说，**PlantUML可以通过更加简单直观的语言来定义这些示意图，是用于绘制UML图的、基于Java和Graphviz-Dot的特化工具**，而Graphviz和Dot则胜在通用性。

## 0.2 PlantUML官网简介
在[https://plantuml.com/zh/news](https://plantuml.com/zh/news)页面可以查看和PlantUML相关的最新消息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/419989d915f849419ca3c1e7781618ac.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
[https://plantuml.com/zh/faq](https://plantuml.com/zh/faq)中列出了许多常见问题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/701451c1701c40cfb7823eb554a06fcd.png)
可以在[https://plantuml.com/zh/download](https://plantuml.com/zh/download)下载页面下载不同版本的PlantUML：
![在这里插入图片描述](https://img-blog.csdnimg.cn/eb85b5a0375d4ee4b2d962e6fbfaa811.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

## 0.3 PlantUML学习指南:star:
[PDF形式的用户指南](http://plantuml.com/zh/guide)，囊括了大部分常用的内容，可能有些错别字或不全的地方，比如对预处理一节，个人建议看[对应网页](https://plantuml.com/zh/preprocessing#f8d8089a98ca8958)。

[PlantUML主题页面](https://plantuml.com/zh/theme)使我们可以尝试更多样式，让UML图变得更加美观：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ad770f9cec0d45a8b3b544339557e881.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


[标准库](https://plantuml.com/zh/stdlib)页面列出了PlantUML的标准库：
![在这里插入图片描述](https://img-blog.csdnimg.cn/eeeea37d39a7483887d3bcc0f51f4d92.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
更深入的对PlantUML Stdlib的学习，**一定不能错过[Hitchhiker's Guide](https://crashedmind.github.io/PlantUMLHitchhikersGuide/)**！！！
![在这里插入图片描述](https://img-blog.csdnimg.cn/f4b66ac1df0f44a283ee6a23263d6e8c.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16) 
## 0.4 PlantUML Server 
PlantUML还有自己的[在线版本](http://www.plantuml.com/plantuml/uml/SyfFKj2rKt3CoKnELR1Io4ZDoSa70000)，功能已经很齐全了：在文本框中输入定义，点击 `Submit` 生成图像，使用 `View as PNG, View as SVG, View as ASCII Art` 查看对应的PNG、SVG和字符图形，通过 `Decode URL` 来分享相应图像的URL。尝试PlantUML最简单快捷的方法就是使用这一在线编辑服务：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e71948ec05254c5eaf55a8778a6db28e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

 



---
# 1. 下载安装PlantUML
如前文所述，我们可以使用在线版本，不过使用本地环境还是更安心顺手一些。因此本节将下载安装PlantUML。

## 1.1 下载安装Java和Graphviz-Dot
Java是运行PlantUML的必需条件，在个人环境中安装Java见[这篇文章](https://memcpy0.blog.csdn.net/article/details/119280425)。Graphviz-Dot虽说是可选的，但是建议安装——如果你想绘制除时序图和活动图以外的图, 就需要安装Graphviz软件，具体过程见[这篇文章](https://memcpy0.blog.csdn.net/article/details/119459490)。
## 1.2 通过命令行使用PlantUML（可以和脚本和文档工具集成）
如果想要在命令行使用PlantUML，就要下载[plantuml.jar文件](https://sourceforge.net/projects/plantuml/files/plantuml.jar/download), 保存到本地安装目录。然后使用PlantUML的[命令行语法](https://plantuml.com/zh/command-line)，将PlantUML集成到你的[脚本和文档工具](https://plantuml.com/zh/running)上。

此时就可以绘制第一个UML图了——在工作目录中建立一个 `sequenceDiagram.txt` ，内容如下：
```bash
@startuml
Alice -> Bob: test
@enduml
```
运行PlantUML（或使用集成了PlantUML的软件来调用），将 `sequenceDiagram.txt` 作为命令输入，输出即为图像。输出图像可以在其他软件中显示，也可以写入到磁盘中的文件上。例如：
```bash
java -jar plantuml.jar sequenceDiagram.txt
```
这条命令会将顺序图输出到 `sequenceDiagram.png` 这个文件中。 

## 1.3 通过外部工具使用PlantUML:star:
事实上，PlantUML已经被集成到许多[脚本和文档工具](https://plantuml.com/zh/running)中：

- 文本编辑器和IDE，包括我想要认真学习的[Vim](https://www.vim.org/scripts/script.php?script_id=3538)/[Emacs](https://plantuml.com/zh/emacs)、VS Code。甚至还有Word（`This template allows using PlantUML directly from MS Word 2010 without need to alter document templates or edit VBA macro's. MS Word Versions 2007 and 2013 might work, but are not tested.`）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fb27ff498c1648529ffb371f50f0132b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
- 和编程语言协同，包括[Emacs Lisp](https://github.com/tj64/puml)、Markdown、Python和Java等：
![在这里插入图片描述](https://img-blog.csdnimg.cn/c59a0bb831e34c44b10f30a1168011b5.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
- 文档生成工具：
![在这里插入图片描述](https://img-blog.csdnimg.cn/118f1bc79e5e48859875663635447243.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
- 论坛和Wiki：
![在这里插入图片描述](https://img-blog.csdnimg.cn/cf6df7e33c714d418f91d502103e8f1e.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

- 线上编辑器，包括我们之前提到的在线版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e93bbe5e169d46e2a2cc57ef12c1c083.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
- 其他：
![在这里插入图片描述](https://img-blog.csdnimg.cn/7d73e302c53540f2a0718c512c688216.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


---
# 2. VS Code中安装支持插件、介绍相应功能
在VS Code的扩展商店中，搜索PlantUML，安装下面的这个插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/845921b5ff134384808c80d6b84cacf6.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
这一插件的功能相当丰富，以后有时间了再一一介绍：
![在这里插入图片描述](https://img-blog.csdnimg.cn/202fd01b0962474aa338e4a8d6799679.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

此外还可以安装这一个插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f4b5170b0d3e4d5e93b59cc8396620f9.png)

---
# 3. 在VS Code中绘制UML图
## 3.1 第一个简单UML图
创建一个 `First.plantuml` 文件，输入以下代码：
```yaml
@startuml FirstUML
Interface Interface01

Interface01 <|.. Class01 : 实现
class02 <|-- Class03     : 继承
Class04 *-- Class05      : 组合
Class06 o-- Class07      : 聚合
Class08 <-- Class9       : 关联
Class10 <.. Class11      : 依赖

@enduml
```
`Alt+D` 预览为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/ba52c8073a654178928a02dfff448c5b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
## 3.2 策略模式的UML图
策略模式结构图的代码如下：
```yaml
@startuml Strategy

abstract class Strategy
class Strategy {
+AlgorithmInterface() 
}
class Context {
-Strategy strategy
+ContextInterface()
}
class ConcreteStrategyA {
+AlgorithmInterface() 
}
class ConcreteStrategyB {
+AlgorithmInterface() 
}
class ConcreteStrategyC {
+AlgorithmInterface() 
}

Strategy <|-- ConcreteStrategyA
Strategy <|-- ConcreteStrategyB
Strategy <|-- ConcreteStrategyC

note "策略类，定义所有支持的算法的公共接口" as N0
N0 .. Strategy
note "具体策略类，封装了具体的算法或行为，继承于Strategy" as N1
N1 .. ConcreteStrategyA
N1 .. ConcreteStrategyB
N1 .. ConcreteStrategyC
note as N2
    Context上下文，用一个
    ConcreteStrategy来配置，
    维护对一个Strategy对象的引用
end note
N2 .. Context

Context o--> Strategy :Strategy
@enduml
```
策略模式预览图如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e772ce86ef0d4ff4889b72594c1cbf2a.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

