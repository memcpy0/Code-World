
@[toc]

> 这篇文章属于本人参考诸多博客、教程、文档等资料，所撰写的「Windows10上配置VS Code」系列文章之一，内容随时可能发生更新变动，欢迎关注和收藏[VS Code系列配置总目录](https://memcpy0.blog.csdn.net/article/details/117640795)一文以作备忘。


[获取了VS Code编辑器之后](https://memcpy0.blog.csdn.net/article/details/117640795)，我们将在VS Code中配置C#的语言学习环境。同样的，我会在个人代码库 `CodeWorld` 中建立 `Code_CSharp` 和 `Code_FSharp` 文件夹，里面的文件夹是 `CSharp_Single` 和 `CSharp_Multiple` 、`FSharp_Single` 和 `FSharp_Multiple` ，分别存放不同的分类代码文件夹，即程序运行目标分别是**单个源代码文件**或**少数几个源代码文件的组合**。

---
# 0. 微软.NET开发环境相关资料
访问微软的[开发者中心](https://developer.microsoft.com/zh-cn/)，从这里可以找到微软所有的开发文档，当然也包括VS Code和.NET：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4c5ce2d90cda4118be74638788f465ae.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

## 0.1 .NET的强大之处
进入[.NET页面](https://dotnet.microsoft.com/)，里面显示——.NET是一个免费的、跨平台的、开源的开发者平台，用于构建许多不同类型的应用程序：
- 为Windows/Linux/macOS/Docker开发**Web应用和服务**；
- 为iOS、Android、Windows开发**本地移动应用**；
- 为Windows/macOS创建**桌面应用**；
- 创建在Docker容器上运行的、可独立部署的**微服务**；
- 使用现有的云服务，或创建和部署自己的**云服务**；
- 创建**机器学习程序**；
- 为桌面电脑、手机和游戏机开发2D、3D**游戏**；
- 开发**物联网应用**，支持Raspberry Pi和其他 `single-board` 计算机；
![在这里插入图片描述](https://img-blog.csdnimg.cn/92f928a2eda4478baa112ead7bfe46eb.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

这么粗略一看，真是强大啊。使用.NET，**我们可以使用多种语言如C#、F#、Visual Basic，搭配多种编辑器和库，来为Web、移动设备、桌面、游戏、云和物联网开发应用，并在任何兼容的操作系统上运行**。[.NET文档](https://docs.microsoft.com/zh-cn/dotnet/?WT.mc_id=dotnet-35129-website)在此，里面包括所有你需要的.NET参考资料：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e605fc19c41b42779cb373f023397018.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
进入文档下的[https://docs.microsoft.com/zh-cn/dotnet/fundamentals/](https://docs.microsoft.com/zh-cn/dotnet/fundamentals/)页面，可以看到真正的宝藏：
![在这里插入图片描述](https://img-blog.csdnimg.cn/4e836d56ae74490680638d9728626ca2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
除了 `Docs` 外，建议关注下.NET网站上的 `Learn` ，里面是.NET的起步教程，都很简短，可以过一遍：
![在这里插入图片描述](https://img-blog.csdnimg.cn/834b3047c2ac418d93d06ec783834f53.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

其中，一个简洁的[.NET Console教程](https://dotnet.microsoft.com/learn/dotnet/hello-world-tutorial/intro)如下所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2120e37e7ec54f4fbc95ec9760e4ae64.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

当然，之前在文档中也看到了入门教程，而且更加全面具体、还有中文。建议过完 `Learn` 页面后，把文档中的入门教程也看一遍。

## 0.2 .NET、.NET Framwork、.NET Core、C#/F#/VB...之间的关系
### 0.2.1 什么是.NET Framework
Windows中的.NET平台结构图如下，虽然已经有些过时了：
<img src="https://img-blog.csdnimg.cn/418229fd29c04a9bbff2b6488f37914c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

硬件设备和操作系统无需多言，更上一层的是.NET Framework（不仅仅是类库，是**框架**）。**.NET Framework是一个仅支持Windows的.NET版本**，安装和运行在Windows操作系统上，是用于**构建在Windows上运行的、任何类型的.NET应用程序**的**核心基础**。目前最新的版本是.NET Framework 4.8。

<img src="https://img-blog.csdnimg.cn/67aaae1c8e124a728e86ed4e200e99e4.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

总的来说，**.NET Framework提供了在Windows上创建、部署、运行.NET应用的环境**，不久的曾经，开发和运行一个.NET应用程序必须安装.NETFramework。.NET Framework包含三个关键的组件：
- `CLR (Common Language Runtime)` 公共语言运行时：**.NET Framework的基础**，提供了所有.NET应用程序的运行时环境，是所有.NET应用程序都要使用的编程基础。可以将其看作一个在执行时管理代码的代理人，管理代码是CLR的基本功能，能够被托管的代码称为托管代码， 反之是非托管代码。托管代码以“CLR”为目标，非托管代码以“操作系统”为目标。
<img src="https://img-blog.csdnimg.cn/f16d1a9049cc41af8008453fa239a7b2.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">
	CLR包含三个组成部分：
	- `CLS (Common Language Specification)` 公共语言规范：是一种最低的语言标准，它制定了一种**以.NET平台为目标的语言所必须支持的最小特征**，以及**该语言与其它.NET语言之间实现互操作所需要的完整特性**。凡是遵守这个标准的语言在.NET框架下都可以实现互相调用。简单来说，就是把各种语言转换成统一的语法规范。
	-  `CTS (Common Type System)` 通用类型系统：**解决不同语言中数据类型不同的问题**，如C#中的整形是 `int` ，而VB.net中的整形是 `Integer` ，CTS可以把它们编译成通用的类型 `Int32` 。
      所有的.NET语言都共享这一类型系统，在它们之间实现对接，从而把各种语言中的不同数据类型，转换成统一通用的数据类型。
     - `JIT (Just In Time)` 即时编译器：将转换之后的中间代码编译为二进制语言，交给CPU执行。CLR编译过程如下，.NET编写的所有源代码都不被编译成本地代码，而是由各自语言的编译器编译成微软中间代码 `MSIL (Microsoft Intermediate Language)` ，再由JIT编译器转换成机器代码（操作系统专用代码）。
     <img src="https://img-blog.csdnimg.cn/39f8d9a0603e46b18e22c32cf741fd6d.png" width="55%">
     MSIL或IL通常被称为程序集语言，因为.NET程序集需要由.NET加载才能运行，所以IL代码也被叫做托管代码 `Managed Code` ，是一种介于高级语言和机器语言之间的中间语言，基于堆栈、又支持面向对象。所有能被编译为IL的语言都可以被.NET Framework托管（其它非微软的编程语言要被.NET Framework托管，则需要第三方编译器支持）。
- `FCL (.NET Framework Class Library)` .NET框架类库：一个综合性的、面向对象的、可重用类型集合，其内容被组织成一个树状的命名空间 `Namespace Tree` ，每个命名空间包含许多类型及其它命名空间，使用FCL时需要引入相应的命名空间。从功能上来看可以将FCL划分成以下几层：
   -  最底层由基础类库 `BCL (Basic Class Library)` 的大部分组成，主要作用是对.NET框架、.NET运行时及CIL语言本身进行支持，例如基元类型、集合类型、线程处理、应用程序域、运行时、安全性、互操作等。
   - 中间层包含了对操作系统功能的封装，例如文件系统、网络连接、图形图像、XML操作等。
   - 最高层包含各种类型的应用程序，例如Windows Forms、Asp.NET、WPF、WCF、WF等。	
- `DLR (Dynamic Language Runtime)` 动态语言运行时：在.NET Framework 4.0中新增了动态语言运行时 `DLR (Dynamic Language Runtime` ，向CLR中添加了一组专门用于满足动态语言的服务。借助于DLR，可以开发在.NET  Framework上运行的动态语言，还能使用C#、VB.NET等方便地与动态语言交互，目前比较出名的DLR应用有IronPython。

> FCL核心命名空间简介：
> - System：此命名空间包含所有其他的命名空用，包含了.NET中使用的公共数据类型，如 `Boolean, DateTime, lnt32` 等。此命名空间中还有一个非常重要的数据类型 `Object` ，`Object` 类是所有其他.NET对象继承的基本类。
> - `System Collections.Generic` ：支持泛型操作，是.NET 2.0新增的内容；
> - `System IO` ：支持对文件的操作，如复制、粘贴、删除及对文件的读写等；
> - `System.Net`：支持对网络协议的编程；
> - `System.Dat` ：提供对表示ADO.NET结构的类的访问；
> - `System.Windows.Forms` ：用于开发Windows应用程序，引入这个命名空间才能使用 `Win forms` 的控件和各种特性；
> - `System.Drawing` ：支持GDI+基本图形操作
> <img src="https://img-blog.csdnimg.cn/2368eacbda404c26bc7641e14deb094c.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="56%">
 
 ### 0.2.2 .NET Framework的版本迭代
.NET Framework的版本历史图如下所示：
<img src="https://img-blog.csdnimg.cn/659c2cd5e5b34116a005e4f25f7b3233.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">

**.NET Framework 3.0版本是一个分水岭**。之前.NET Framework只提供ASP.NET Web开发、WinFroms窗体应用程序开发等基本的功能，在此之后引入了许多新的特性：
- WPF：微软Vista操作系统的核心开发库之一，是一个图形引擎。用于实现界面显示与程序代码的分离；
- WCF：把Web服务、.NET Remoting等技术统一到单个面向服务的编程模型中，以实现真正的分布式计算；
- WF：一个广泛通用的工作流程框架，从下到上在每个级别都针对扩展性进行了设计。
- Windows CardSpace：是微软公司取代用户名和密码成为验证网络使用有者身份的新方法；
- LinQ：查询集成语言，简化不同数据源对象查询操作；
- ADO.NET Entity Framework：简称EF，是微软以ADO.NET为基础所发展出来的对象关系对应（O/R Mapping）解决方案（ORM框架），早期被称为 `Object Space` ，该框架之后从.NET Framework分离出来；
- Parallel Linq ：简称并行LINQ或PLINQ，是 `LINQ to Objects` 的并行实现。PLINQ将LINQ语法的简洁和可靠，与并行编程的强大功能结合在一起。 就像面向任务并行库的代码一样，PLINQ查询会根据主计算机的能力，按比例调整并发程度。
在许多情况下，PLINQ可通过更有效地使用主计算机上的所有可用内核，显著提高 `LINQ to Objects` 查询的速度。这一性能提升使桌面具备高性能计算能力。
- Task Parallel Library：简称任务并行库（TPL），是 `System.Threading` 和  `System.Threading.Tasks` 命令空间中的一组公共类型和API。TPL的目的是，通过简化将并行和并发添加到应用程序的过程，以提高开发人员的工作效率。
TPL动态缩放并发的程度，以最有效地使用所有可用的处理器。此外TPL还处理工作分区、ThreadPool上的线程调度、取消支持、状态管理以及其他低级别的细节操作。
通过使用TPL，可以在将精力集中于程序要完成的工作，同时最大程度地提高代码的性能。
- Modern UI Runtime：简称Modern UI，是一个专注于用户界面设计的框架；
- Task-Based Async Model：基于任务的异步模型

.Net Framework 4.7则主要带来了这些提升：为Win10窗体应用带来了高DPI支持；为Win10上的WPF程序带来了触控支持；增强了加密支持；提升性能和稳定性。

 
### 0.2.3 什么是.NET Core、和.NET Framework的关系
初步了解了.NET Framework，再来了解一下.NET平台发布的.NET Core。与.NET Framework不同的是，**.NET Core致力于跨平台工作，它是一个跨平台的.NET实现，适用于Windows、Linux、macOS上的网站、服务台和控制台应用程序**。此外还有Xamarin/Mono，也是一个.NET实现，用于**在所有主流移动操作系统中运行应用程序**。

它们之所以都是.NET实现，是因为遵循了.NET Standard——这是所有.NET实现通用的一组基本API，是跨操作系统的关键所在，每个实现还可以调用特定于其运行的操作系统的其他API。例如.NET框架是一个只适用于Windows的.NET实现，包括用于访问Windows注册表的特定API。为了扩展功能，微软和其他公司还在.NET Standard的基础上，维护了一个软件包生态系统，包含超过90000个包，并为.NET构建了一个包管理器NuGet。
 
不过，关键在于为什么要发布.NET Core。**跨平台是一方面，更重要的是统合.NET平台上的众多.NET Framework**——除了2002年微软发布的第一个.NET Framework外，还有.NET Compact Framework（用于小型移动设备如Windows mobile，是一个复制精简版的.NET Framework，包括Runtime、Framework、Application Model等），以及之后数年间微软推出的、多个用于在不同的设备和平台上运行的、类似.NET Framework的框架。框架越来越多，对开发者来说是一个巨大的负担，毕竟要在不同设备上维护许多套功能相同的代码，这极大增加了开发的复杂度。

为了改变这种情况，同时迎合当时微软CEO的愿景——抛弃以PC为中心、拥抱各种设备和平台，成为一家软件服务型企业，就必须构建新的微软生态链，统合Windows、Linux、OS X及其他操作系统，覆盖X86/ARM等处理器以及不同的设备（包括PC、Phone、全息眼镜及其他）。

于是.NET Core横空出世，作为一个开源的、模块化、跨平台跨设备的Framework，**它全面统合了对Web、移动设备、桌面等的开发**。更棒的是，.NET Core在发布程序时不用事先安装Framework，而是通过Nuget（[Nuget官网在此](https://www.nuget.org/)）下载，这样在初次部署时，就不用安装一个复杂而庞大的.NET Framework，而是按需动态下载当前操作系统的实现，部署起来更加简单快速、也更轻便。**这种基于Nuget的按需加载，构建起.NET Core的跨平台体系**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/bf7791a21e894955a78555dc357cf6ad.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

### 0.2.4  什么是.NET 5.0（.NET 6.0）、和.NET Core的关系

.NET 5.0是.NET Core 3.1的后继。我们完全可以把.NET 5.0看做.NET Core 5.0，只是移除了 `"Core"` 这个词。`.NET Framework 4.x` 已死，.NET Core 3.1才是 `LTS (Long-term service)` ，其后继者就是.NET 5.0。相对于.NET Core来说，.NET 5.0在开发层面没有过大的变化，依然采用.NET Core的编写模式进行，只是通过以下几个关键方式改进.NET：
- 制造一个可在任何地方使用的 .NET 运行时和框架，具有统一的运行时行为和开发人员体验；
- 通过充分利用.NET Core、.NET Framework、Xamarin和Mono来扩展.NET 的功能；
- 从单个代码库构建该产品，开发人员（Microsoft和社区）可以一起工作并一起扩展，从而改进所有方案。

下图清楚地展示了.NET 5的统治范围：
![在这里插入图片描述](https://img-blog.csdnimg.cn/35afa28349a64e08b53eed5fa9118433.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
虽然如此，.NET 5.0这个版本还不是LTS，只是大版本前的过渡版本，谈不上前景可言，但是又不可或缺，主要是为.NET 6.0做铺垫。
 
---
# 1. 下载安装.NET
由于.NET 6.0还是Preview版，这里还是下载.NET 5.0吧：
![在这里插入图片描述](https://img-blog.csdnimg.cn/225ac762ab6b46409998066205ac95b2.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)

点击.NET网站上的[Download](https://dotnet.microsoft.com/download)，进入下载页面，下载Windows下的.NET 5.0：
![在这里插入图片描述](https://img-blog.csdnimg.cn/8219ec663390456eb8b0b8a259839438.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
之后运行安装程序，此处是 `dotnet-sdk-5.0.400-win-x64.exe` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/95620ca67425401eb72cefeca61ca1c7.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
![在这里插入图片描述](https://img-blog.csdnimg.cn/83d740c1c39b44c0bedd42337bb25bc3.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
很简单就安装成功了，只能说不愧是微软：
![在这里插入图片描述](https://img-blog.csdnimg.cn/b4ed0de8b9c348caa9192e8f2eeb1872.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
安装完成后，打开一个新的命令行，输入以下命令：

```bash
C:\>dotnet

Usage: dotnet [options]
Usage: dotnet [path-to-application]

Options:
  -h|--help         Display help.
  --info            Display .NET information.
  --list-sdks       Display the installed SDKs.
  --list-runtimes   Display the installed runtimes.

path-to-application:
  The path to an application .dll file to execute.
```
运行 `dotnet new --list` 查看能够创建的项目类型：
![在这里插入图片描述](https://img-blog.csdnimg.cn/bfae91953c4947efb660650dd4852a7f.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
运行 `dotnet --info` 能够查看许多信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fd7c549a307242adb29c017e1401c458.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)


---
# 2. 命令行中创建、编辑代码、运行程序
仍在命令行中，输入以下命令来创建应用，之后进入命令新创建的文件夹：

```bash
C:\>dotnet new console -o myApp

欢迎使用 .NET 5.0!
---------------------
SDK 版本: 5.0.400

遥测
---------
.NET 工具会收集用法数据，帮助我们改善你的体验。它由 Microsoft 收集并与社区共享。你可通过使用喜欢的 shell 将 DOTNET_CLI_TELEMETRY_OPTOUT 环境变量设置为 "1" 或 "true" 来选择退出遥测。

阅读有关 .NET CLI 工具遥测的更多信息: https://aka.ms/dotnet-cli-telemetry

----------------
已安装 ASP.NET Core HTTPS 开发证书。
若要信任该证书，请运行 "dotnet dev-certs https --trust" (仅限 Windows 和 macOS)。
了解 HTTPS: https://aka.ms/dotnet-https
----------------
编写你的第一个应用: https://aka.ms/dotnet-hello-world
查找新增功能: https://aka.ms/dotnet-whats-new
浏览文档: https://aka.ms/dotnet-docs
在 GitHub 上报告问题和查找源: https://github.com/dotnet/core
使用 "dotnet --help" 查看可用命令或访问: https://aka.ms/dotnet-cli
--------------------------------------------------------------------------------------
正在准备...
已成功创建模板“Console Application”。

正在处理创建后操作...
在 myApp\myApp.csproj 上运行 “dotnet restore”...
  正在确定要还原的项目…
  已还原 C:\myApp\myApp.csproj (用时 185 ms)。
已成功还原。

C:\>cd myApp
```

`dotnet new console` 命令创建了一个新的控制台程序，`-o` 参数创建了一个名称为 `myApp` 的文件夹，里面存放了你的程序和需要的文件：
```bash
 C:\myApp 的目录

2021/09/07  14:57    <DIR>          .
2021/09/07  14:57    <DIR>          ..
2021/09/07  14:57               171 myApp.csproj
2021/09/07  14:57    <DIR>          obj
2021/09/07  14:57               187 Program.cs
               2 个文件            358 字节
               3 个目录 34,933,153,792 可用字节
```
`myApp` 文件夹中的主要文件是 `Program.cs` ，默认包含了向控制台打印 `"Hello World!"` 所需的代码：
```csharp
C:\myApp>type Program.cs
﻿using System;

namespace myApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
        }
    }
}
```
接着在命令行中输入以下命令，运行你的程序：
```bash
C:\myApp>dotnet run
Hello World!
```
然后随便使用什么文本编辑器，编辑 `Program.cs` 文件，添加一行新的代码：
```bash
C:\myApp>emacs Program.cs
using System;

namespace myApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");
            Console.WriteLine("The current time is " + DateTime.Now);
        }
    }
}
```
是时候运行了，新的程序将打印当前时间：
```bash
C:\myApp>dotnet run
Hello World!
The current time is 2021/9/7 15:10:04
```
有时间了，再把 `Learn` 页面的[其他几个简单教程](https://dotnet.microsoft.com/learn)试一下，把 `Docs` 下的入门教程看一遍。

---
# 3. VS Code中安装.NET支持插件
首先安装C#的支持插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/449d16672f5c4996b92778803f9626f3.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
接着搜索 `Ionide-fsharp` ，安装F#的支持插件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2150fc5d09024729938a4f75adca057c.png)
## 3.1 创建一个C#程序
首先像前文一样初始化一个C#项目文件夹，即运行 `dotnet new console` 创建一个C#控制台程序。再在VS Code中打开项目文件夹，它会自动安装相应依赖，有 `OmniSharp for Windows (.NET 4.6 / x64)` 、`.NET Core Debugger (Windows / x64)` 、`Razor Language Server (Windows / x64)` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/44a8b5f1709e4316946fea3e5d233f2f.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
之后，同样在终端输入 `dotnet run` 来运行程序。

## 3.2 创建一个F#程序
首先初始化一个F#项目文件夹，像前文一样，运行 `dotnet new console -lang "F#"` 创建一个控制台程序。如果遇到 `error NU1100` 这种无法解析的错误，建议看一下这个[Issue](https://github.com/dotnet/fsharp/issues/10147)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/08bca08ac4864fe580b29e9411e2b71b.png?x-oss-process=image/watermark,type_ZHJvaWRzYW5zZmFsbGJhY2s,shadow_50,text_Q1NETiBAbWVtY3B5MA==,size_20,color_FFFFFF,t_70,g_se,x_16)
之后，同样在终端输入 `dotnet run` 来运行程序，打印 `"Hello world from F#"` 这一消息。

## 3.3 C#插件功能介绍
Roslyn是微软开源的.NET编译器，支持C#和Visual Basic 代码编译，并提供丰富的代码分析API。OmniSharp是一组工具、编辑器集成和库，它们共同创建了一个生态系统，无论选择什么编辑器和操作系统，都可以得到出色的编程体验。

VS Code使用Roslyn和OmniSharp的强大功能，主要为.NET Core项目、MSBuild项目、C#脚本（CSX）提供更强的C#体验。在打开项目文件夹时，VS Code将自动加载最匹配的项目，或者进行询问：
![在这里插入图片描述](https://img-blog.csdnimg.cn/f6703d4f08ec468483e827be73f0b96c.png)
当然也可以手动选择项目。状态栏将显示已加载的项目，还允许单击状态栏项目项并选择更改项目：
![在这里插入图片描述](https://img-blog.csdnimg.cn/3c0bd12b09204cf1a5da94b75c40dc5e.png)
可供选择的方案包括：
- 选择 `project.json` 文件，将打开一个.NET Core项目，VS代码将加载该项目以及引用的项目；
- 选择 `*.sln` 文件，将打开一个MSBuild项目。它将加载引用的 `*.csproj` 项目和兄弟或后代 `project.json` 文件，但不会加载从解决方案文件引用的其他项目文件。
- 选择文件夹，将使VS Code扫描 `*.sln` 、`project.json` 和 `*.csx` 文件（C#脚本），VS Code将尝试加载所有这些文件。

加载项目后，将享受到以下舒适体验：
- Editing Evolved：更优秀的编辑体验，如 `format on type, rename-refactoring` 等，编辑特性的全描述见[Basic Editing](https://code.visualstudio.com/docs/editor/codebasics)和[Code Navigation](https://code.visualstudio.com/docs/editor/editingevolved)；
- Intellisence：`Ctrl+Space` ，比如提取基类什么的；
- Snippets for C#：除了内建的几个 `snippets` 外，也可以自己定义 `snippets for C#` 。虽然我还没有用过[User Defined Snippets](https://code.visualstudio.com/docs/editor/userdefinedsnippets)这一功能，不过说不定哪天就会用了，先记下来；
- Search for Symbols：`Ctrl+T` ，敲击要搜索的符号，就能看到匹配的C#符号列表；
- CodeLens：可以查看一个方法被引用的次数。点击引用信息，还能查看Peek视图中的引用。此引用信息在键入时持续更新；
- Find References/Peek Definition：……
- Quick Fixes / Suggestions：可以看到一个灯泡形状，点击它来查看修改建议，或者使用快捷键 `Ctrl+.` 。
- ……

---
# 4. 书写插件配置文件
VS Code对C#项目类型的支持是不全的，有些项目如 `ASP.NET MVC` 不被支持。如果只是想要一个轻量级的工具，来编辑C#、F#文件，VS Code还是可以做到的；如果想要最好的开发体验，还是要下载Visual Studio。

……

等我看完这些内容后，再更新吧：
- [Basic Editing](https://code.visualstudio.com/docs/editor/codebasics) - Learn about the powerful VS Code editor.
-  [Tasks](https://code.visualstudio.com/docs/editor/tasks) - Use tasks to build your project and more.
-  [Debugging](https://code.visualstudio.com/docs/editor/debugging)  - Find out how to use the debugger with your project.
-  [Unity development](https://code.visualstudio.com/docs/other/unity) - Learn about using VS Code with your Unity projects.

 


---
# 5. 调试运行C#、F#代码

……

以后学C#、F#的时候，慢慢写吧。
