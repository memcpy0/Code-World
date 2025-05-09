@[toc]

---
# 6.1 概述
## 6.1.1 Java 2平台
Java 2为「从嵌入式环境、小型桌面环境到大型企业级平台的各层次软件系统开发」，提供了针对性的开发与运行环境支持。图6-1给出了Java 2平台的整体示意图：
![图6-1 Java 2平台示意图](https://img-blog.csdnimg.cn/739b094bd0be4ea6a2c0463438f36ee0.png)
由于Java是一种解释性语言，因此==Java 2平台的最底层是面向不同环境的Java虚拟机 `Virtual Machine` ，为相应环境下的Java应用程序，提供基本的解释执行Java字节码的运行支撑==。
- `Card VM` 是面向智能卡 `java card` 的虚拟机，它为智能卡定义了一组Java语言子集和虚拟机；典型的 `Java Card` 设备拥有 $8$ 位或 $16$ 位处理器，$1\textrm{M}\sim 5\textrm{MHz}$ 的运行频率，$1.2\textrm{KB}$ 的RAM和 $32\textrm{KB}$ 的ROM。`Card VM` 仅支持最小的Java语言子集。
- 比 `Card VM` 高一个层次的是 `KVM` ，是面向手机的虚拟机（一般运行环境在 $512\textrm{K}$ 以内）；现在全世界有上亿部手机中的应用程序，是用Java开发的，这些应用程序的运行通常基于 `KVM` 。
- `Classic VM` 是面向PDA、机顶盒等设备的虚拟机。
- `Hotspot` 是面向企业级应用的虚拟机。

从 `Card VM, KVM, Classic VM` 到 `HotSpot VM` ，它们所基于的硬件环境拥有越来越多的资源（更高速度的处理器、更多的内存），因此也支持越来越复杂、规模越来越大的应用软件。应注意，**新版本的Java虚拟机结构有所变化，去掉了 `Classic VM`** ；另外，**早期版本中还存在一个 `Exact VM`** 。
 
 
基于底层的虚拟机支持，Java 2平台为开发人员提供了三种不同版本的开发/运行环境，帮助开发、管理基于不同环境的Java应用。
- `Java 2 Micro Edition, J2ME` ：Java最初是为消费电子设备编程而设计的嵌入式语言，`J2ME` 就是Java 2平台针对「**基于消费电子设备**（如手机、PDA、智能卡等）**的应用开发**」所推出的平台。
- `Java 2 Standard Edition, J2SE` ：用来支持开发**小型的桌面应用**的Java平台，也就是通常所说的JDK。
- `Java 2 Enterprise Edition, J2EE` ：即Java企业版，是用来支持**大型企业级应用**开发与运行的平台，以下简称J2EE。

应注意，**上述三个版本是同时存在的，用来支持不同领域或规模的应用**。

J2EE的目标是用来==支持以构件化的方法开发、部署、管理多层结构的分布式企业应用，尤其是基于Web的多层应用==。J2EE围绕这一目标，提供了一种**统一的、开放标准的多层平台**。这一点和CORBA类似，==J2EE为开发人员提供的是一种工业标准、而不是某个厂商自己的产品，尽管J2EE规范是由 Sun 负责制定和发布的，但它确实是一套业界普遍支持和采纳的开放标准==，规范本身也是由很多个大型厂商，比如BEA、IBM 等来共同推动的。基于一种工业标准所开发的应用，一般不会局限于某个厂商特定的应用服务器产品，开发人员无须额外的开销，即可获得较高的可移植性。同时，遵循统一规范的不同产品之间的竞争环境，也有利于提高产品的质量。

**在J2EE规范中，容器的概念非常突出，J2EE构件要依赖于容器所提供的系统级支持**。与基于J2ME、J2SE的应用不同，==基于J2EE的应用系统不能直接基于Java虚拟机运行，还需要容器构成的 Java企业版平台的支持才能运行==。容器为运行于其中的构件，提供生命周期管理等大量的服务，支持部署人员将构件部署到其中，同时为构件的运行指派线程。

**在J2EE中，许多构件的行为都可以在部署时，以声明的方式进行定制**，这是J2EE的一个很大的特色。就是说系统开发完后，部署时还可以在源代码以外的地方进行定制来配置构件，使得它能够满足特定的应用需要，即**开发人员一次开发的构件，可以通过定制去装配到不同需求的应用环境中**。另一方面，当应用背景或需求发生变化的时候，很多情况下，可以不需要去修改源程序，当然更不需要重新编译，而是**在源代码以外的地方进行定制，就可以使得原有的构件适应新的环境或需求**。
## 6.1.2 J2EE平台技术
J2EE平台为开发企业级应用，提供三个方面的技术支持，如图6-2所示，包括**应用构件、公共服务与通信**三个方面。
![图6-2 J2EE 平台提供的支持](https://img-blog.csdnimg.cn/ed521484841743ff83cd1d96cc1e2cfa.png)

第一个侧面就是**对应用构件或者组件的支持**，在J2EE中，应用构件是由开发人员实现、用来构成应用系统的软件模块，这和软件系统的一般意义上的构件含义相同；另外，==J2EE构件都是运行在J2EE平台之上的，更确切地讲，这些构件都是包含在某一类容器中的==。

J2EE中的应用构件可分为两大类：**客户端构件和服务端构件**。客户端构件又包含 `Applets` 和 `Application Clients` ；服务端构件又包含Web构件 `JSP, Servlets` 和EJB `Enterprise Java Bean` 构件，第2节将对每种J2EE构件进行简要的解释。

第二个侧面是**对服务的支持**，即[【中间件技术】第一部分 概述(1) 软件构件与中间件基本概念](https://memcpy0.blog.csdn.net/article/details/122645156)中提到的公共服务。具体到J2EE平台，服务是指J2EE应用构件所使用的功能，这些功能由J2EE平台提供商实现。这和CORBA中类似，在CORBA中，开发人员也可以使用大量的服务，比如对象服务中包含的命名服务、安全性控制等，公共设施中包含的打印、电子邮件等，以及一些领域接口；这些服务不需要程序员自己实现。但在J2EE平台上，服务有一个重要的区别，就是==J2EE中除了像CORBA中那样、以API的方式提供的服务外，还提供了丰富的运行时服务==。J2EE平台提供的服务，一部分我们可以像在CORBA中那样，通过「J2EE规范约定的标准接口」在程序中使用，还有一部分可以在源代码以外的地方，通过声明的方式来使用，这些就是运行时服务。

J2EE平台技术的第三个侧面就是**对通信的支持**，主要用来支持构件之间的交互，**通信机制也是由容器来提供的**。
## 6.1.3 J2EE平台的执行模型
J2EE典型地支持**三层/多层分布式应用**的开发与运行，图6-3所示的J2EE 平台的执行模型，描述了「基于J2EE的应用系统」在运行时的基本结构：
![图6-3 J2EE 平台的执行模型](https://img-blog.csdnimg.cn/38f8c9a2b0824b04a230b98f54c45142.png)
首先，从构成系统的构件来讲：
- 在客户层，可以包含 `Applet` 和 `Application Client` 两种构件；类似使用 `Applet` 的、基于浏览器的客户端，通常称为**瘦客户端** `thin client` ，因为这种客户端不需要在客户机器上安装任何应用代码，只需一个浏览器；而使用独立的客户端程序的客户端，称之为**胖客户端** `thick client` ，因为其需要在每一个客户端机器上安装客户端程序。J2EE中的胖客户端通常是一个独立的Java程序。
- 在中间层，应用中可以包含像 `servlet, JSP` 这样的Web构件，也可以包含EJB构件。一般又会把J2EE应用的核心中间层，划分成「包含Web构件的 Web子层」和「包含EJB构件的EJB子层」。
- 在数据层，包含应用使用的各种企业数据，这些数据有些存储在数据库中，有些可能包含在企业的原有系统中，比如：企业中可能已经有一套基于SAP的ERP系统，其中的数据要被新开发的这套J2EE系统使用。

其次，在J2EE平台中广泛使用**构件/容器体系结构**，即**构成J2EE应用系统的构件都运行在某种J2EE容器中**。还要注意的是，**J2EE平台中每一个容器底层都需要J2SE的支持**。比如，大家经常使用的浏览器就是一种常用的 `Applet` 容器。另外，==在中间层，有两类非常重要的容器——Web 容器和EJB容器，分别为Web构件和EJB构件提供运行环境==。

在构件/容器体系结构下，==将底层常用的、且通常是比较复杂的服务打包在容器中，由容器向其中的构件提供运行环境与公共服务的支持==。容器由J2EE平台提供商提供，为构件提供特定的**开发用服务**和**运行时服务**（如生命周期管理、安全性管理、事务处理等），不同类型的J2EE容器提供不同的服务。容器除了为构件提供公共服务之外，还为构件之间的交互、或者是构件访问后台数据提供了基本的支持，包括客户端与中间层Web构件或EJB构件的交互、Web构件与EJB构件的交互、以及构件访问数据层数据等。

---
# 6.2 J2EE应用构件
本节简要解释「构成J2EE应用系统的构件」，重点关注其中的核心——EJB构件，其它构件的详细内容可参阅其它资料。
## 6.2.1 客户端构件
### 1. `Applet`
**`Applet` 是具有图形用户界面的特殊的Java类，一般运行在Web浏览器中，也可以运行在支持 `Applet` 编程模型的容器中**（如J2SE中的 `appletviewer` ）。在J2EE应用中，`Applets` 一般用来提供用户界面，图6-4给出了一个 `applet` 运行时的界面，当用户在 `applet` 提供的图形界面上操作时，可能会导致服务端的构件（比如EJB）被调用。
![图6-4 Applet 示例](https://img-blog.csdnimg.cn/4b298d8707ba40d0865e3a526dbc914d.png)
与具有图形用户界面的独立Java程序不同，`applet` 类自身不包含 `main` 入口函数，它被容器调度执行。`applet` 代码不需要提前在运行的客户端机器上安装，而是**由 `applet` 容器在运行时从服务器下载到本地执行**。
### 2. `Application Client`
**`Application Client` 指有图形用户界面的独立Java程序，在J2EE应用中所起的作用与 `Applet` 类似——用来提供用户界面**。当用户在 `Application clients` 提供的图形界面上操作时，也可能会导致服务端的构件（比如EJB）被调用。

与 `applet` 不同，**`Application Client` 通常包含 `main` 入口函数，且需要在每个使用的客户端机器上安装**。图6-5给出了一个 `application client` 运行时的界面。
![图6-5 Application Client 示例](https://img-blog.csdnimg.cn/27a71e7845fa4617bd9b59b879525bd0.png)
应注意，很多J2EE应用中均不使用 `application client` 或 `applet` 作为客户端程序，**客户端的用户界面一般由Web页面来提供，要么是静态的HTML页面，要么是服务端的Web构件动态生成的页面**。这其中除了==胖客户端需要在每个客户端机器上布署客户端程序==的原因外，==页面设计的快捷与美观==也成为越来越重要的原因，同时，==「基于浏览器的客户端」只能访问中间层的Web构件，这使J2EE应用被划分成更为清晰的「由客户层、Web层、EJB层和数据层构成的四层结构」==。

## 6.2.2 服务端构件
### 1. Web构件 `Servlet`
`Servlet` 也是一种特殊的Java类，**和 `applet` 不同，`servlet` 是运行在服务端的，因此它不需要图形用户界面**。`Servlet` 在J2EE应用中的主要作用是，**接收客户端的HTTP请求**（如通过浏览器发出的请求），**动态生成HTTP响应**（如一个页面）。程序6-1给出了一个处理HTTP请求的 `servlet` 的例子，该 `servlet` 的主要功能就是==根据客户端的请求，为其动态生成一个HTTP响应——HTML页面==。
```java
// 程序 6-1 Servket 代码示例
// 演示如何处理 HTTP GET 请求
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class GetDemo extends HttpServlet {
	public void doGet(HttpServletRequest req, HttpServletResponse resp) 
		throws ServletException, IOException {
		resp.setContentType("text/html");
		PrintWriter out = resp.getWriter();
		String username = req.getParameter("uname");
		String password = req.getParameter("userpw");
		out.println("<HTML>");
		out.println("<HEAD><TITLE>GetDemo</TITLE></HEAD>");
		out.println("<BODY>");
		out.println("Hello " + username + "<br>");
		out.println("Your password was : " + password +"<br>");
		out.println("</BODY>");
		out.println("</HTML>");
		out.close();
	}
}
```
上面的 `servlet` 经过布署后，可以被客户端通过浏览器访问，该 `servlet` 根据用户访问时提供的用户名与密码信息，为其动态生成一个页面，该页面上包含用户提供的信息，该 `servlet` 的某次被访问时呈现的页面如图6-6 所示。
![图6-6 servlet 被访问时产生的动态页面](https://img-blog.csdnimg.cn/e7774b8f24684d2db2ccfc786958a369.png)

可以看出，`servlet` 为J2EE应用提供了基本的动态页面支持。
### 2. Web构件 `JSP`
**`Java Server Page, JSP` 是一类特殊的HTML文档，它通过在HTML文档中嵌入 `JSP` 特定的标签，允许程序员在页面中加入Java代码，通过Java代码的执行，动态生成页面的内容**。程序6-2给出了一个 `JSP` 的例子，该JSP在客户端访问时，会在页面上显示服务器端的当前时间。
```java
// 程序 6-2 JSP 代码示例
// 演示 JSP
<html>
	<head>
		<title>Ch6 Simple jsp</title>
	</head>
	<body>
		<%@ page import="java.util.*" %>
		<h2>Date and Time: <%= new Date().toString() %></h2>
		<hr>
		<% for(int i=0; i<10; i++) %>
		hello<br>
		<% %>
		how are u?<br>
	</body>
</html>
```
在服务端，`JSP` 页面被编译成 `servlet` 执行。显然，使用 `JSP` 开发动态页面要比使用 `servlet` 方便，因为==在JSP中，页面上静态的内容可以直接用HTML语言编写，从而可以利用现有的页面设计工具、以可视化的方式完成，只有动态的部分才需要嵌入Java代码来实现==。而**有了 `JSP` ，`servlet`  本身就通常不再完成具体的动态页面生成工作**，在第4节会看到，**`servlet` 通常会在J2EE应用中，充当分发请求的控制器**。

### 3. EJB构件
**EJB是 `Enterprise Java Bean` 的缩写，通常是构成J2EE应用系统的核心构件，是用来支持大型的企业级应用开发的**。在J2EE规范中，开发人员可以编写 $3$ 种类型的EJB：实体构件 `Entity Bean` 、会话构件 `Session Bean` 和消息驱动构件 `Message Driven Bean` 。

在[【中间件技术】第二部分 CORBA规范与中间件(4) 编写对象接口](https://memcpy0.blog.csdn.net/article/details/122645257)第6节曾经提到过，在系统中经常会区分实体型接口和会话型接口，**在J2EE中直接把这种思想纳入了规范本身**，开发人员所开发的EJB就包含代表**实际业务系统中的共享实体的实体构件**和**支持客户端交互的会话构件**。其中实体构件又分为 `Container Managed Persistence, CMP` 构件和 `Bean Managed Persistence, BMP` 构件，会话构件又分为无状态会话构件 `Stateless Session Bean` 和有状态会话构件 `Stateful Session Bean` 。

在 `EJB 2.0` 之后引入了消息驱动构件，消息驱动构件是在EJB中用来支持异步模式编程的，这种构件通常不被客户端直接调用，而是当有事件发生，即有消息收到时才会执行相应动作。[第8章]()将详细介绍EJB构件的开发与使用。

---
# 6.3 J2EE中的公共服务
J2EE平台为应用构件提供公共服务与通信的基本支持，其中公共服务又分为**通过API调用方式使用的 `Service API`** 和**通过配置方式使用的运行时服务 `Run-time Service`**，本节简要解释J2EE平台提供的常见 `Service API` 和运行时服务。

## 6.3.1 `Service API`
### 1. `JNDI`
`Java Naming and Directory Interface, JNDI`  是J2EE平台提供的命名服务接口，它为开发人员提供的主要功能是**在程序中查找/定位构件或系统资源**。比如，如果需要访问某个EJB，可以利用 `JNDI` 服务找到要使用的EJB。如果需要访问数据库，可以利用 `JNDI` 服务找到要访问的数据库资源。**`JNDI` 服务使用的名字，是一个带树型结构目录的名字**，类似于CORBA中 `POA` 名字。

在J2EE规范推出之前，就已经存在多种可以使用的目录服务，支持我们完成类似的功能，比如基于轻量级目录访问协议 `Lightweight Directory Access Protocol, LDAP` 的目录服务。==如果开发应用时在程序中使用了某种特定的目录服务，那么一般情况下就很难直接迁移到另一种目录服务上，因为不同的目录服务所提供的接口是不一样的==。

而 **`JNDI` 所提供的服务，非常类似于一种通用的目录服务**。如图6-7所示，基于 `JNDI` 的支持，在J2EE应用中可通过这种统一的API来使用目录服务的功能，它为应用屏蔽了不同目录服务之间的差异，这使得利用 `JNDI` 开发的应用，可以不用修改代码、而应用到不同的目录服务上。
![图6-7 JNDI 服务](https://img-blog.csdnimg.cn/7238432ba031464da1000ad19c960785.png)

在J2EE中，大多数的 `Service API` 都具备这样的特征——这些API为应用使用这些服务提供了一个标准的接口，开发人员不用关心「这些接口之下的实现」会存在什么差异，只要按照接口的约定来使用这些服务；厂商实现这些API时可以任意选择，比如厂商可以选择将自己原有的目录服务器，利用 `JNDI` 进行封装，为J2EE开发者提供目录服务。可以看到，这些 `service API` 和CORBA规范中标准化的接口起的是同样的作用：**开发人员按照标准的接口来使用，厂商按照自己的方式来实现接口中的功能**。

### 2. `JDBC`
**`JDBC` 为应用提供与厂商无关的数据库连接**，它通常==提供一种通用的方法，用来查询、更新关系型数据库表，并且把数据库操作的结果转化成Java的数据类型==。一个 `JDBC` 驱动支持应用跨数据库平台、完成以下三件事情：
1. 建立与数据库的连接；
2. 向数据源发送查询和更新语句；
3. 处理结果。

`JDBC 2.0` 包含了一个内嵌的数据库连接池，**数据库连接池是J2EE构件访问数据库的常用方式**。在传统的Java程序中，==应用需要访问数据库时，通常首先加载一个驱动程序，然后建立数据库连接，进行数据库操作，操作完成后再释放数据库连接==。在这个过程中，建立和释放数据库连接的过程是非常耗时的，有时甚至比真正的数据库操作所占用的时间还多。

而数据库连接池则为应用提供了一种**更为快捷的数据库访问方式**，在这种模式下，==平台把数据库连接当作是一种系统资源来存放在数据库连接池中，平台初始化时会自动建好一些数据库连接，当应用需要操作数据库时，直接从池中获取一个建好的连接，用完后再把连接放回连接池供后续使用==，这就为应用节省了建立数据库连接和释放数据库连接的过程，从而达到一个较高的访问数据库的效率。
### 3. `JTA`
`Java Transaction API, JTA` 用来支持应用中的事务控制。事务通常包含一组操作的集合，**事务管理支持可靠的服务端计算**，这在很多关键系统中都是必需的。事务具有如下特性：
- **原子性**：所有相关的操作必须全部成功；如果任何一个操作失败，则所有操作全部撤销。
- **一致性**：一致性保证事务所作的任何改变不会使系统处于无效状态。
- **隔离性**：并发的事务不会互相影响。事务访问的任何数据不会受其它事务所作的修改的影响，直到第一个事务完成。
- **持久性**：事务提交时，对数据所作的任何改变都要记录到持久存储器中。通常由事务日志实现。

而==分布式事务可能会跨越多个组件、多台机器，并可能涉及多个数据源，因此需要更复杂的控制机制==。

`JTA` 是在 `J2EE` 应用中使用的一种支持分布式事务处理的标准API。厂商可以选择自己合适的实现方式来提供事务控制的功能，比如在 `websphere` 中，利用一种支持两阶段提交的数据库驱动来实现 `JTA` 。**在J2EE应用中，除了在代码中利用 `JTA` 来控制事务外，还可以通过配置方式来实现事务控制**，参考本节后续的运行时服务。

### 4. `JCA`
在基于J2EE开发一套新的应用系统时，经常需要在新的系统中去访问企业原有的旧系统，我们把这种系统称为**外部信息系统** `External Information System, EIS` 。企业原有系统各种各样，比如有基于 `SAP` 实现的信息系统，有基于 `CICS` 实现的信息系统。

一般来说，要访问不同的系统，所用的手段和编写的程序是不一样的，因为不同的系统所提供的接口不一样。 `J2EE Connector Architecture, JCA` 是在J2EE中支持访问不同信息系统的一种统一API。如图6-8所示，**基于 `JCA` ，J2EE应用可以使用统一的API访问不同类型的信息系统**。
![图6-8 JCA原理](https://img-blog.csdnimg.cn/cbd11ddb77fb44f7892aceff75a2d1d3.png)
基于 `JCA` 访问不同系统时，通常通过**连接不同类型系统的适配 `Adapter`** 来屏蔽不同系统的差异，这些适配器一般由开发 `EIS` 的厂商、或者第三方厂商提供。理想情况下，外部信息系统某种标准中间件开发，则可以期望由中间件厂商来实现该适配器。如果外部信息系统不是基于这些中间件开发的，更糟糕的是可能没有办法找到系统的开发者，相应的适配器就很难实现了。
### 5. `Java Mail API`
**`Java Mail API` 定义了一组在程序中用来调用邮件功能的API**。这组API屏蔽了真正的邮件服务所使用的协议。Sun提供了一种 `Java Mail API` 的实现，支持我们在应用中使用Email功能，可以从Sun网站上免费获得该 `Java Mail API` 的实现。

与其它 `Service API` 类似，`Java Mail API` 为J2EE应用提供了一种统一的API，这种API使得Email可以在Internet中跨越平台和邮件协议传输。
### 6. `Java IDL`
**`Java IDL Service API` 是用Java实现的CORBA规范**，该API提供异类环境构件的互操作，允许在J2EE应用中访问CORBA构件。

## 6.3.2 运行时服务
### 1. 生命周期管理
**生命周期管理主要指的是EJB构件的生命周期管理**。该服务的主要含义是**由容器来管理运行于其中的构件的生命周期**。EJB规范统一规定了EJB生命周期管理策略，==厂商实现J2EE平台时，要按照规范实现生命周期管理功能，开发人员编写EJB构件时，要注意构件的生存期特性==。和CORBA相比，有了容器提供的生命周期管理服务，**开发人员就不需要自己写服务程序了**，容器会自动在需要的时候将EJB构件准备好，由窗口来决定什么时候实例化分布式对象、以及实例化多少个对象。


当然，这是两方面的，不编写服务程序就意味着==开发人员放弃了对服务端程序的更多的控制力，如果容器提供的生命周期管理服务不能满足应用的某些特定需要，可能能添加的干预也很有限==。
### 2. 事务控制
在J2EE中，**除了利用前面提到的 `JTA` 来实现事务控制外，还可以将事务交给容器自动控制、而开发人员不用编写任何Java代码**。容器会根据部署时指定的事务属性，自动完成控制的事务。当然，容器控制的事务是有局限性的，只能是方法级的，即其维护的事务的粒度是方法。

容器维护的事务称为 `Container Managed Transaction, CMT` ，对应地，开发人员在代码中通过 `JTA` 来控制事务的方式称为 `Bean Managed Transaction, BMT` 。
### 3. 安全服务
J2EE中的安全性控制分为两个级别：第一级是认证 `authentication` ，就是首先要验证访问者的身份。经过身份认证后，第二级就是授权`authorization` 控制，所谓==授权就是判断特定的访问者是否具有在特定资源上执行某种操作的权限==，比如，是否有权限访问系统中的某个 `JSP` 页面，是否有权限调用系统中某个EJB提供的特定方法等。

**在J2EE中，实现安全性控制的常用方式是声明的方式**。声明方式的安全性控制完全由容器自动控制，不需要程序员编写任何Java代码。开发人员主要通过以下配置工作来定制安全性控制的规则：
- **声明需要的安全性角色** `security-role` ，并且把角色映射到实际的安全域中。这里的==安全性角色是指为应用系统所指定的逻辑用户组==，比如经理角色、职员角色等。
在实际的安全域中，一般会有多个用户映射到同一个角色。比如，我们可以为一个公司使用的应用系统建立一套用户管理系统，其中张经理使用的用户 `managerzhang` 映射到经理角色，李经理使用的用户 `managerli` 也映射到经理角色。同时还会有多个员工使用的用户映射到职员这一角色。
- **声明安全控制规则**，通过这些规则告诉容器，如何为应用实现安全性控制。比如，我们可以通过规则告知容器：“经理”角色可以执行“员工管理” `Bean` 的“辞退员工”方法，还告知容器：“所有人（未经过身份认证）”可以访问商品信息页面等。

除了声明的方式外，我们还可以自己写代码来进行安全性控制。这种方式要结合声明的方式使用，还需要声明需要的安全性角色，然后在程序中根据读到的角色来控制它是否有权限执行请求的操作。
### 4. 持久性服务
**持久性服务**主要指**与实体构件 `Entity Bean` 相关的数据库操作**。==J2EE中的实体构件主要用来实现数据库的访问。基于容器提供的持久性服务，开发人员可以通过实体构件，将相关的数据库操作交给容器来自动完成==，这种方式称为 `Container Managed Persistence, CMP` ；当然，实体构件相关的数据库操作也可以由开发人员编写Java代码来完成，这种方式称为 `Bean Managed Persistence, BMP` 。
 
### 5. 资源管理
**资源管理**是指==由J2EE运行平台负责维护/管理「应用所用到的系统资源」==，如数据库连接资源、线程资源等。比如，前面提到的数据库连接池就是用来管理数据库连接这种系统资源的，类似的还有系统线程池，用来维护一定数量的已创建好的线程。

资源管理为应用带来的好处主要有两个：
- **提高系统资源的利用率和效率**：有限的资源通过平台管理起来，在众多的构件之间共享，可以提高资源的利用率；另外，资源管理为资源的使用作了一些预处理操作（如预先建好数据库连接等），可以有效提高资源的使用效率。
- **使得应用程序与具体的资源相对无关**：比如，应用使用不同的数据库管理系统时，程序员所写的代码是一样的，当使用的数据库管理系统发生变化时（比如由DB2变为Oracle），不需要修改源程序，只要改一下资源的配置信息就可以实现。

## 6.3.3 通信支持
### 1. `RMI/IIOP`
`RMI/IIOP` 是基于「CORBA之中提出的 `IIOP` 协议」实现的远程方法调用 `Remote Method Invocation` ，主要是用于**支持EJB构件客户端对EJB的远程调用**。

应注意，`RMI/IIOP` 与[【中间件技术】第一部分 概述(1) 软件构件与中间件基本概念](https://memcpy0.blog.csdn.net/article/details/122645156)例子中使用的 `Java RMI` 不同，**`Java RMI` 是Java SE提供的、用来支持远程访问 `Java RMI` 对象的标准**，该标准基于 `Java Remote Messaging Protocol, JRMP` 实现。`Java RMI` 对象与EJB都是用Java语言实现的分布式对象，它们之间最大的区别就是：**EJB可以使用J2EE平台提供的各种公共服务，而 `Java RMI` 对象不可以**。
### 2. `JMS`
`JMS` 是 `Java Messaging Service` 的缩写，代表**J2EE平台为应用构件提供的异步消息服务**。`JMS` 定义了 `Service` ，定义了一组统一的API，支持在程序中发送和接收异步消息。和其它的 `Service API` 类似，不同的厂商根据API来实现异步消息处理机制，一般的做法是提供一个消息服务器。

---
# 6.4 J2EE应用开发
本节简要介绍**J2EE应用程序的基本结构**，以及**J2EE应用的基本开发过程**。
## 6.4.1 J2EE应用程序
在J2EE规范下，**开发者所开发的各种J2EE构件，需要被组装成完整的J2EE应用，才能部署到J2EE服务器上**。如图6-9所示，==应用构件通过组装被打包到一个J2EE应用文件（`.ear` 文件）中，然后布署到J2EE中间件平台上运行==。在打包过程中，需要应用组装者编写一种特殊的组装配置文件——**布署描述符** `Deployment Descriptor, DD` 。 
![图6-9 J2EE 应用打包与布署的基本过程](https://img-blog.csdnimg.cn/3aeee0ce3c644acf84177a35425172af.png)
在J2EE中，**开发者开发的不同构件会按照类型与层次，被打包到不同的目标文件中**，J2EE平台上的目标文件主要有以下三种类型：
- **Java目标文件** `Java Archive` ：Java语言的目标程序（`.class` 文件）包，对应磁盘上后缀名为 `.jar` 的文件，在J2EE应用中用来打包EJB构件、`Application Client` 以及它们需要的辅助Java目标文件。
- **Web目标文件** `Web Archive` ：Web构件的目标程序包，对应磁盘上后缀名为 `.war` 的文件，在J2EE应用中用来打包Web构件（`Servlet, JSP`）以及静态页面相关的文件（如HTML文档、图片等）。
- **企业目标文件** `Enterprise Archive` ：J2EE应用目标程序包，对应磁盘上后缀名为 `.ear` 的文件，在J2EE应用中用来打包完整的J2EE应用。一个完整的J2EE应用中，可以包含若干个Java目标文件和若干个Web目标文件。

在打包Java目标文件、Web目标文件与企业目标文件时，都需要提供相应的布署描述符。

基于上述三种程序包，J2EE应用程序的基本结构如图6-10所示，==一个J2EE应用对应一个企业目标文件，其中可以包含若干个打包了EJB的EJB模块（一个EJB模块对应一个Java目标文件）、若干个打包了Web构件的Web模块（一个Web模块对应一个Web目标文件）和若干个打包了客户端程序的客户端模块（一个客户端模块对应一个Java目标文件）==。
![图6-10 J2EE 应用程序的基本结构](https://img-blog.csdnimg.cn/329545023f2b481a965d0b60b9e28f70.png)
将每个模块中包含的构件进一步细化，我们可以得到图6-11所示的J2EE应用程序的完整结构：
![图6-11 J2EE 应用程序的完整结构](https://img-blog.csdnimg.cn/e3fd13a59c68455085784d854355a81c.png)
==每个EJB模块包含若干个EJB构件、与一个当前模块的布署描述符；每个 Web模块包含若干个 `Servlet, JSP` 构件、若干静态页面与相关图片等资源，以及一个当前模块的布署描述符；每个客户端模块包含若干个客户端程序使用的Java类、与一个当前模块的布署描述符==。除了每个模块包含一个布署描述符外，整个J2EE应用还包含一个布署描述符。

**一个布署描述符是一个 `XML` 格式的文件**，该文件中描述了「当前模块中所包含的内容（构件或模块）」和「模块所需要的环境」，J2EE平台的丰富源代码以外的可定制性，大部分都体现在部署描述符中。**每个模块或 `.ear` 文件都有一个部署描述符**。J2EE中，布署描述符的名称是硬编码的，即每种类型的目标文件都包含一个固定名字的布署描述符。

一般来说，J2EE平台提供商会提供相应的工具来自动生成部署描述符，并且支持以可视化的方式进行编辑。因为部署描述符是一个 `XML` 格式的文件，所以可以手工创建和编辑，只要你熟悉部署描述符的格式。下面简要介绍常用布署描述符中的内容：
- EJB模块的布署描述符的名称是 `ejb-jar.xml` 。**同一EJB模块中的所有EJB构件，共享该模块的布署描述符**。在该布署描述符中，对于每一个EJB构件描述了该EJB的 `Home` 接口、`Remote` 接口、以及真正提供服务的类的名字；如果是 `Session Beans` ，说明是哪种类型的 `Session Bean` ；如果是 `Entity Bean` ，说明需要容器提供的持久性管理服务及相关的信息；说明该EJB是否需要由容器来控制事务，如果需要，怎样控制；说明该EJB的安全控制策略等等。
可以看出，EJB构件的很多特性都是在部署描述符中确定的，比如如何管理持久性（也就是 `Entity Bean` 的数据库操作）、怎样控制事务、怎样控制安全性等信息，都在部署描述符中有所体现。==当这些相关的特性发生变化时，可以不修改EJB的源程序，而仅通过修改部署描述符，就可以使得EJB去适应新的环境==。
- Web模块的布署描述符的名称是 `web.xml` ，**同一Web模块中的所有构件，共享该模块的布署描述符**。Web模块的布署描述符首先描述了当前模块包含的构件（包括 `Servlet, JSP` 、表态页面等）；可以为当前模块中的构件说明安全控制规则。此外，J2EE应用的Web模块中，经常需要配置应用使用的用户认证方式，因为在典型的J2EE应用中，客户端通常通过浏览器直接访问Web模块中的构件，Web模块中的构件首先与访问者接触，因此通常在Web模块中完成对用户身份的认证。
- J2EE应用的布署描述符的名称是 `application.xml` 。J2EE应用的布署描述符描述了「当前应用中包含的所有模块」，此外还可能定义「应用使用的安全性角色」。
## 6.4.2 基于角色的开发过程
J2EE规范将应用系统「从开发到布署维护的生命周期」映射到了 $6$ 个角色：
- J2EE平台提供者 `J2EE Product Provider` ，其**工作就是按照J2EE规范实现J2EE平台**，如IBM、BEA、Sun等厂商，他们提供的主流J2EE平台包括IBM的 `Webshpere Application Server` 、BEA的 `WebLogic Application Server` 、Sun的 `iPlanet Application Server` 等，除非特别声明，第三部分的例子均基于Sun提供的免费J2EE平台——J2EE参考实现 `J2EE Reference Implementation` 。
- 工具提供者 `Tool Provider` ，其**任务是为其他角色提供各种工具来帮助其完成J2EE平台上的任务**，如 `Websphere Application Server, WSAD` 、`Weblogic Builder` 、`JBuilder` 、`NetBean` 等开发工具，用于组装J2EE应用的组装工具，用于管理、监控J2EE平台与应用的管理/监视工具等。
- 应用构件提供者 `Application Component Provider` ，其**工作是按照应用需求、实现构成应用的各种构件，另外还可能参与编写布署描述符**。
- 应用组装者 `Application Assembler` ，其**工作是将构件提供者提供的构件组装成应用**。
- 布署者 `Deployer` ，其**工作是将应用安装配置到运行环境中**。
- 系统管理员 `System Administrator` ，其工作比较繁杂，包括==备份配置数据、配置企业级应用、进行应用统计分析和性能分析、创建服务器组和服务器克隆以优化系统性能、管理并控制应用的安全性==等。

**在这些角色中，一个角色的输出经常是另一个角色的输入**，如 `Application Assembler` 将 `Application Component Provider` 开发的构件组装成应用，而 `Deployer` 则将 `Application Assembler` 组装的应用部署到运行平台上。同一个（组）人可能会执行两个或多个角色的任务，一个角色的任务也可能由几个（组）人完成。
  

## 6.4.3 J2EE应用中的 `MVC` 设计模式
模型-视图-控制器 `Modeling-View-Controller, MVC` 设计模式明确划分了不同的构件在应用系统中的作用。如图6-12所示：
- 在J2EE应用中，模型 `Modeling` 即**系统的模型或系统基本的业务功能**，通常由EJB构件实现；
- 视图 `View` ，即**系统的人机交互界面**，通常由JSP构件实现；
- 控制器 `Controller` ，即**分发客户请求，决定每次客户端请求调用哪个EJB构件完成、结果由哪个JSP构件呈现**的控制器，通常由Servlet构件实现。
![图6-12 J2EE应用中的MVC模式](https://img-blog.csdnimg.cn/ec8747258f84402faebd1aaca5204b1d.png)

在 `MVC` 模式下，**J2EE应用呈现更为清晰的四层结构：客户层、Web层、EJB层与数据层**，==客户通过浏览器发出的请求，通常被Servlet构件接收，Servlet调用合适的EJB构件完成客户请求，然后再将处理结果利用JSP呈现==。此时，客户端只能访问Web构件，Web构件不会直接访问数据库，J2EE应用呈现出典型的四层结构。 

---
# 思考与练习
6-1 J2EE应用中构件/容器能够为应用构件提供哪些好处？
6-2 组成J2EE应用的应用构件主要有哪几种？每种应用构件在J2EE应用中的基本作用是什么？`MVC` 模式如何在J2EE中实现？
6-3 请简单描述J2EE应用程序的基本结构，说明布署描述符的主要作用。

