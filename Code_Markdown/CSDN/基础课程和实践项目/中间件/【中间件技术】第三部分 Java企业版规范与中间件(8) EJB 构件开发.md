@[toc]

---
# 8.1 Java SDK
## 8.1.1 安装
**在安装Java EE平台之前，必须安装一个合适版本的JDK**，因为我们在J2EE执行模型中看到，**每种J2EE容器都需要Java标准版J2SE，即JDK的支持**。Java标准版通常包含两部分内容：一个标准版的开发包——J2SDK `Java Software Development Kit` ，和一个标准版的运行环境——J2RE `Java Runtime Environment` 。

J2SE为标准的Java应用或 `applet` 提供标准的Java服务，而Java的企业级API是由J2EE平台实现的，**要开发、运行Java企业版应用，必须安装J2EE平台**。例子相关的J2EE平台，采用Sun提供的J2EE参考实现 `Reference Implementation` 。

应注意，**J2EE平台与相应的J2SE之间通常存在版本的约束**，例子所使用的J2EE参考实现与J2SE的版本均为 $1.3.1$ 。相关软件的安装过程均比较简单，全部采用缺省设置即可完成安装。
## 8.1.2 环境变量配置
安装完Java平台后，必须正确设置相应的环境变量才能正常使用。通常需要配置的环境变量包括：
- `JAVA_HOME` ：在Java平台中，`JAVA_HOME` 环境变量用来指明**包含J2SE类和配置文件的目录**，通常是J2SE的安装目录，如 `C:\jdk1.3.1` 。J2EE平台通过该环境变量，查找需要使用的可执行文件，如Java语言编译器 `javac` 和Java语言解释器 `java` 等。
- `J2EE_HOME` ：在Java平台中，`J2EE_HOME` 环境变量用来指明**包含J2EE类和配置文件的目录**，通常是J2EE的安装目录，如 `C:\j2sdkee1.3.1` 。J2SE与J2EE平台都需要使用 `JAVA_HOME` 环境变量，而 `J2EE_HOME` 环境变量只有J2EE平台才需要。
- `PATH` ：**J2SE与J2EE包含的可执行文件**，通常放在对应安装目录下的 `bin` 子目录中，为方便使用Java平台相关的可执行文件，如 `javac, java, j2ee` 等，通常需要将J2SE与J2EE安装目录下的 `bin` 子目录，添加到系统的 `PATH` 环境变量中。
- `CLASSPATH` ：`CLASSPATH` 是**Java平台在编译或运行Java程序使用的环境变量**，该环境变量列出了**包含编译过的Java代码**（`.class` 文件）**的目录和Java目标文件包**（`.jar` 文件），缺省情况下，Java虚拟机会在 `CLASSPATH` 指明的目录和Java目标文件包中搜寻、编译、或运行Java程序所需的编译过的Java代码。通常需要在 `CLASSPATH` 环境变量中，包含J2SE和J2EE安装目录下 `lib` 子目录中的Java目标文件（如J2SE中的 `tools.jar, dt.jar` ，J2EE 中的 `j2ee.jar` 等），此外 **`CLASSPATH` 中最好包含当前路径**。
- `JAVA_FONT` ：在Java平台中，`JAVA_FONT` 环境变量用来指明包含应用所使用的字体的目录，Java平台使用该环境变量查找要使用的字体文件。

Windows中的环境变量，通常在"控制面板\系统\高级\环境变量"中进行添加与修改，并且环境变量的修改会自动在新打开的命令行窗口中生效。Linux中的环境变量通常在 `\etc\profile` 配置文件、或当前用户的 `profile` 配置文件中进行添加与修改，并且环境变量的修改会自动在用户下次登录时生效。

## 8.1.3 启动与关闭J2EE参考实现
J2EE参考实现是Sun提供的免费J2EE平台，为构建、部署、测试J2EE应用提供运行时环境。**J2EE参考实现可采用命令行方式进行启动与关闭，使用的命令为 `j2ee`**（对应可执行文件在J2EE参考实现安装目录下的 `bin` 子目录中）。命令 `j2ee` 可使用以下几个命令行参数：
- `-verbose` ：将系统日志重定向到当前命令行窗口，缺省情况下系统日志会输出到文件中。
- `-help` ：输出帮助信息。
- `-version` ：输出版本信息。
- `-singleVM` ：将所有的 `bean` 部署到同一个虚拟机（进程）中。
- `-multiVM` ：将同一个JAR文件中的 `bean` 部署到同一个虚拟机中，不同JAR中的 `bean` 部署到不同的虚拟机中。
- `-stop` ：关闭J2EE服务器。

---
# 8.2 开发与使用无状态会话构件
**会话构件是一个「基于EJB的软件系统」业务逻辑功能的主要实现者**，除了数据库的相关操作，系统中的基本功能通常均由会话构件实现，会话构件又分为无状态会话构件与有状态会话构件。

本节通过一个简单的例子，演示如何开发与使用无状态会话构件，例子程序分为客户端与服务端两部分。在服务端，我们将构建一个名为 `CurTimeApp` 的J2EE应用，该应用中仅包含一个无状态会话构件，该构件实现「返回服务端当前系统时间」的功能；客户端通过远程接口调用构件上的操作，将结果输出。

## 8.2.1 开发EJB构件
该EJB构件实现返回服务端当前系统时间的功能，==由于该构件的实例（对象）不需要保存「与特定客户端相关的会话状态」，因此设计为无状态的会话构件==。以下简称该EJB构件为时间EJB。
### 1. 定义 `Remote` 接口
`Remote` 接口包含**EJB构件实现的商业方法的声明**，客户端只能通过 `Remote` 接口访问构件实现的商业方法、不能直接调用。程序8-1给出了时间EJB的 `Remote` 接口定义：
```java
// 程序 8-1 时间EJB的 Remote 接口定义
import javax.ejb.*;
import java.rmi.*;
public interface CurTime extends EJBObject {
	String getCurTime() throws RemoteException;
}
```
在程序8-1中，我们定义了一个名为 `CurTime` 的 `Remote` 接口，时间EJB构件仅向客户端提供一个商业方法—— `getCurTime` ，`Remote` 接口中包含该方法的声明，客户端调用该方法时不需要提供任何参数，返回值为一个包含当前系统时间的字符串。可以看到，按照EJB规范的约定，**接口 `CurTime` 继承了接口 `EJBObject` ，操作 `getCurTime` 抛出 `RemoteException` 异常、以报告远程调用错误，其返回值为合法的 `Java RMI` 类型**。
### 2. 定义 `Home` 接口
`Home` 接口中包含**EJB构件生命周期管理的相关方法**，客户程序使用 `Home Interface` 创建、查找或删除EJB的实例。程序8-2给出了时间EJB的 `Home` 接口定义：
```cpp
// 程序 8-2 时间EJB的 Home 接口定义
import javax.ejb.*;
import java.rmi.*;
public interface CurTimeHome extends EJBHome {
	public CurTime create() throws RemoteException, CreateException;
}
```
在程序8-2中，我们定义了一个名为 `CurTimeHome` 的 `Home` 接口，按照EJB规范的约定，**接口 `CurTimeHome` 继承了接口 `EJBHome`** 。该接口中仅声明了一个 `create` 方法，`create` 方法在EJB规范中，用于取代传统面向对象中的构造函数来初始化一个EJB实例。**由于会话构件的 `create` 方法会为调用者准备好一个EJB构件的实例，因此 `create` 方法的返回值必须是对应EJB构件的 `Remote` 接口类型，以支持客户端调用EJB构件上的商业方法**，本例中返回的是程序8-1中定义的 `Remote` 接口 `CurTime` 。按照EJB规范的约定，**`create` 方法必须抛出`RemoteException` 和 `CreateException` 异常**，`RemoteException` 表明发生了网络错误，而 `CreateException` 异常可能由EJB构件抛出，也可能由EJB容器产生，该异常通知客户端不能创建EJB对象。
 
由于无状态会话构件的对象可能被多个客户端共享地访问，因此，==EJB规范不允许某个客户端使用特定的参数来初始化无状态会话构件的对象，进而使得无状态会话构件 `Home` 接口中，只能包含没有参数的 `create` 方法==。
### 3. 定义 `Enterprise Bean` 类
在EJB中，`Remote` 接口中所定义的商业方法由 `Enterprise Bean` 类实现，**定义好 `Remote` 接口和 `Home` 接口后，就可以定义相关的 `Enterprise Bean` 类**。`Enterprise Bean` 类首先要按照 `Remote` 接口的约定，实现商业方法 `getCurTime` ，其次要实现 `Home` 接口中 `create` 方法对应的 `ejbCreate` 方法，以及与会话构件生命周期相关的方法。程序8-3给出了时间EJB的 `Enterprise Bean` 类定义：

```java
// 程序 8-3 时间EJB的 Enterprise Bean 类定义
import javax.ejb.*;
import java.sql.*;
public class CurTimeBean implements SessionBean {
	SessionContext Context;
	public String getCurTime() {
		Timestamp ts = new Timestamp(System.currentTimeMillis());
		return ts.toString();
	}
	//无状态的 session bean 只能包含无参数的 create
	public void ejbCreate() {
		System.out.println("\n\n\n******CurTimeBean ejbCreate");
	}
	public void ejbRemove() {
		System.out.println("CurTimeBean ejbRemove");
	}
	public void ejbPassivate() {}
	public void ejbActivate() {} 
	public void setSessionContext(SessionContext Context) {
		this.Context = Context;
	}
}
```
在程序8-3中，我们定义了一个名为 `CurTimeBean` 的 `Enterprise Bean` 类，由于该 EJB构件是会话构件，因此 `Enterprise Bean` 类实现（`implements`）`SessionBean` 接口；如果要开发的EJB构件是实体构件，对应的 `Enterprise Bean` 类则需要实现 `EntityBean` 接口。**`SessionBean` 接口与 `EntityBean` 接口都是 `EnterpriseBean` 接口的子接口**。
- `Enterprise Bean` 类首先按照 `Remote` 接口 `CurTime` 的约定，实现了商业方法 `getCurTime` ，该方法将当前系统时间转换为字符串返回。
- 其次，`Enterprise Bean` 类实现了 `Home` 接口 `CurTimeHome` 中的 `create` 方法对应的 `ejbCreate` 方法，容器调用 `ejbCreate` 方法完成EJB对象的初始化。
- 此外，`Enterprise Bean` 类还实现了四个会话构件生命周期管理相关的其它方法：`ejbRemove, ejbPassivate, ejbActivate, ejbSetSessionContext` ，这些方法是接口 `SessionBean` 约定的。
	- 其中 `ejbRemove` 会在容器删除EJB实例之前被调用，因此该方法类似于传统面向对象中的析构函数；
	- `ejbPassivate` 与 `ejbActivate` 方法是有状态会话构件使用的两个方法，在下一节中讨论；
	- `setSessionContext` 方法用来初始化EJB使用的 `SessionContext` 变量，**`SessionContext` 是会话构件与容器交互的入口**，每次创建一个会话构件的对象时，容器会调用该对象的 `setSessionContext` 方法，给对象传入使用的 `SessionContext` 变量。


从上面的讨论可以看出，与普通的Java类相比，`Enterprise Bean` 类中多出了 `ejbCreate, ejbRemove, ejbPassivate, ejbActivate, setSessionContext` 等EJB生命周期管理相关的方法，这些方法会在容器管理EJB对象的过程中被调用。==只有在了解了这些方法的调用时机的基础上，才能决定在这些方法中应该完成什么样的工作==，因此，下面首先讨论「无状态会话构件的生命周期特征」。

如图8-1所示，**无状态会话构件的生命周期包含两个状态：方法就绪状态 `Method Ready State` 与不存在状态 `No State`** 。不存在状态表明：EJB容器中不存在对应无状态会话构件的实例，处于不存在状态的实例还未被创建；方法就绪状态表明：对应无状态会话构件对象已被创建，可以为客户端提供服务。
![图8-1 无状态会话构件生命周期](https://img-blog.csdnimg.cn/39c00be119de4576811b15b80af6b1db.png)
==只要EJB容器认为，实例池中需要更多的实例为客户端服务，就会创建新的实例，此时新创建的实例从不存在状态、进入方法就绪状态；如果EJB容器认为，当前已无需这么多的实例为客户端服务，就会根据某种策略删除池中的一些实例，此时被删除的实例又从方法就绪状态进入不存在状态==。应注意，**对于无状态的会话构件来说，实例的创建和删除都是由容器自动来控制的**，并不是客户端每调用一次 `Home` 接口中的 `create` ，都会创建一个新的实例，容器也不允许客户端调用 `Home` 接口中的 `remove` 方法来删除实例。因为无状态会话构件不保存与特定客户端相关的会话状态，其实例可以被多个客户端共享，因此**容器会尽可能使用少量的实例为多个客户端提供服务**。

从不存在状态进入方法就绪状态（创建新实例），容器会调用 `Enterprise Bean` 类对象的 `setSessionContext` 和 `ejbCreate` 方法（不是每一次客户端调用 `Home` 接口中的 `create` 都会导致容器调用 `Enterprise Bean` 类对象的 `ejbCreate` 方法）；从方法就绪状态进入不存在状态（删除实例），会调用 `ejbRemove` 方法（容器不允许客户端调用 `Home` 接口中的 `remove` 方法来删除无状态会话构件的实例）。
### 4. 编译源代码
编写完 `Remote` 接口、`Home` 接口与 `Enterprise Bean` 类之后，就可以用Java语言编译器 `javac` 对EJB的源代码进行编译，编译时仅是将接口定义和 `Enterprise Bean` 类的定义进行Java源程序到Java目标代码的编译，不会检查 `Remote` 接口、`Home` 接口、`Enterprise Bean` 类之间的对应关系，**对应关系在打包EJB时会检查**。

## 8.2.2 打包/布署EJB
### 1. 打包J2EE应用
**EJB构件不能够直接部署到J2EE服务器上**，必须==首先创建一个J2EE应用，然后将要部署的EJB添加到J2EE应用中，再把J2EE应用部署到服务器上==。本节使用J2EE参考实现提供的 组装与部署工具 `deploytool` 来创建J2EE应用。工具 `deploytool` 位于参考实现安装目录下的 `bin` 子目录中，如果仅是组装应用，可以单独启动 `deploytool` ；如果需要部署应用，则启动 `deploytool` 之前、需要首先启动J2EE服务器，可使用命令启动J2EE服务器：
```bash
prompt> j2ee -verbose
```
如图8-2所示，点击 `deploytool` 的 `File\New\Application` 菜单，可创建一个空的J2EE应用，在图8-3所示的对话框中，输入应用对应「企业目标文件的文件名」与「应用的名字」，即可完成应用的创建，这里我们将应用命名为 `CurTimeApp` 。
![图8-2 创建 J2EE 应用的菜单项](https://img-blog.csdnimg.cn/6651cad427c74bc7adf6052366181b75.png)![图8-3 输入企业目标文件名与应用名字](https://img-blog.csdnimg.cn/5ea3385d728b4186b9631cb86453eb7d.png)
选中创建的J2EE应用，如图8-4所示，可以看到其中包含J2EE规范要求的布署描述符—— `application.xml` ，除此之外，该应用中还包含一个「J2EE参考实现扩充的布署描述符」—— `sun-j2ee-ri.xml` 。
![图8-4 J2EE应用中包含的布署描述符](https://img-blog.csdnimg.cn/d9120bad35064742a14499bc055ed225.png)
创建好J2EE应用后，需要将EJB构件打包到该应用中的一个EJB模块中。按照6.4节中的讨论，**一个EJB模块对应一个Java目标文件（`JAR` 文件）**，EJB构件相关的所有类和接口、以及部署描述符都被打包到 `JAR` 文件中，**一个 `JAR` 文件中可以包含多个EJB**，在本例中只有一个EJB构件——时间EJB。一个EJB模块对应的 `JAR` 文件中，包含EJB的 `Home` 接口、`Remote` 接口、`Enterprise Bean` 类、实体构件的主键类、布署描述符、其它相关的Java类与接口等内容。

在布署工具 `deploytool` 中，可以在打包某EJB模块中的第一个EJB构件的同时，创建该模块对应的 `JAR` 文件。如图8-5所示，点击 `deploytool` 的 `File\New\Enterprise Bean` 菜单，可打包一个EJB构件，`deploytool` 提供一个图形界面的向导 `wizard` ，帮助组装者对打包的EJB构件进行设置，点击图8-6所示的向导的 `Next` 按钮，开始配置要打包的时间EJB。
![图8-5 开始打包时间EJB](https://img-blog.csdnimg.cn/15d47ae6f286480dad518122264985b8.png)
![图8-6 用于打包EJB构件的wizard的初始界面](https://img-blog.csdnimg.cn/2ba96255e2a84e0e9d034c5992816c09.png)
首先，需要在图8-7所示的界面中，选择为时间EJB创建一个新的EJB模块来包含它，本例中为该模块使用缺省的名字`"Ejb1"` 。
![图8-7 为时间EJB创建一个新的EJB模块](https://img-blog.csdnimg.cn/7994ac8a18fb4c1eadd5da5752c9c185.png)
点击图8-7所示界面中的 `Edit` 按钮，在出现的图8-8所示的界面中，将时间EJB构件的 `Home` 接口 `CurTimeHome` 、`Remote` 接口 `CurTime` 和 `Enterprise Bean` 类 `CurTimeBean` 对应的Java目标文件加入到当前模块中。添加完成后，可以看到，在当前EJB模块中包含「时间EJB对应的三个Java目标文件」和一个该模块的布署描述符—— `ejb-jar.xml` 。
![图8-8 将EJB构件相关的Java目标文件添加到模块中](https://img-blog.csdnimg.cn/e59f794dee4a4ddda27a25e2b2115191.png)
然后，在图8-9所示的界面中，设置时间EJB构件的类型为无状态会话构件 `Stateless Sesssion Bean` ，并指定其 `Enterprise Bean` 类、`Home` 接口与 `Remote` 接口的名字。
![图8-9 设置EJB构件的基本信息](https://img-blog.csdnimg.cn/d008c86845214f4bba5c42281b53551e.png)
图8-10所示的界面，用来配置当前正在打包的EJB构件的事务控制规则，由于本例中不讨论事务控制，因此可使用缺省设置。
![图8-10 设置事务控制规则](https://img-blog.csdnimg.cn/70009903f5c14f5597fe0c932c904a8c.png)
图8-11所示的界面，用来配置当前正在打包的EJB构件所使用的环境条目信息，==环境条目由"名字-值"对组成，允许我们在部署或组装EJB时进行定制，不需要修改源代码==。由于本例中不使用环境条目，因此不需进行额外的设置。
![图8-11 配置环境条目](https://img-blog.csdnimg.cn/b72341b49c944d63902c505b01365600.png)
图8-12所示的界面，用来配置当前正在打包的EJB构件所引用的其它EJB的相关信息，**对于每个被引用的EJB构件，需要指明其 `Home` 接口和 `Remote` 接口的名字**。由于本例中时间EJB不引用其它EJB构件，因此不需进行额外的设置。
![图8-12 配置EJB引用](https://img-blog.csdnimg.cn/89966ae17ba74ad6b1239ad4907ec51d.png)
图8-13所示的两个界面，分别用来配置当前正在打包的EJB构件所使用的资源工厂（如数据源等）与资源环境（如 `JMS` 的目的地等）。由于本例中，时间EJB未引用资源工厂与资源环境，因此不需进行额外的设置。
![图8-13 配置资源工厂与资源环境引用](https://img-blog.csdnimg.cn/e9c5101a5ea24ca6820033910cb6851b.png)
图8-14 所示的界面，用来配置当前正在打包的EJB构件的安全性控制规则，由于本例中不讨论安全性控制，因此可使用缺省设置。![图8-14 配置安全性控制规则](https://img-blog.csdnimg.cn/4580f190b4fe4b2a984372e8b718fe0f.png)
前面看到的步骤，实际是在该向导的帮助下，编写EJB模块的布署描述符，该向导的最后一个界面如图8-15所示，界面上显示了「根据前面的配置生成的布署描述符」的内容。
![图8-15 根据前面的配置生成的布署描述符](https://img-blog.csdnimg.cn/8219e0ad133a4e67b10502b0d805e593.png)
打包完成后，需要为EJB构件配置一个 `JNDI` 名，**配置的 `JNDI` 名允许客户端查找并使用该EJB**。在图8-16所示的 `deploytool` 界面中，选中包含时间EJB的模块，点击 `"JNDI Names"` 选项卡，可以为EJB模块中包含的所有EJB构件配置 `JDNI` 名，本例中我们为时间EJB配置名为 `"CurTimeBean"` 的 `JNDI` 名，**配置的 `JNDI` 名一定要和客户端查找EJB构件使用的 `JNDI` 名完全一致**。
![图8-16 为EJB构件配置JNDI名](https://img-blog.csdnimg.cn/82128f2106e54f4b8e2a0677d273d33a.png)
### 2. 验证并布署J2EE应用
配置完 `JNDI` 名后，就可以将J2EE应用布署到J2EE服务器上。可以看出，在打包的过程中，并没有对EJB构件的 `Home` 接口、`Remote` 接口和 `Enterprise Bean` 类之间是否符合EJB规范的约定做检查，==如果开发人员编写的程序不符合EJB规范的约定，则应用无法正常布署到服务器上，因此在布署之前，有必要对应用的合法性进行验证==。

`deploytool` 提供一个验证工具，用来检查应用是否符合 `J2EE/EJB` 规范的约定，该验证工具可以对J2EE应用、EJB模块的 `JAR` 文件、Web模块的 `WAR` 文件以及J2EE客户端的 `JAR` 文件进行验证。如图8-17所示，点击 `deploytool` 的 `Tools\Verifier` 菜单可打开验证工具，在图8-18所示的界面中，选择需要验证的模块或应用，点击 `OK` 按钮开始验证，验证工具会对模块或应用中包含的所有J2EE构件进行「是否符合 `J2EE/EJB` 规范」的逐项验证，验证结果输出到下面的列表框中；选中列表框中任一项，可以在下面的 `"Details"` 文本框中，看到该项是否验证通过以及相应的错误信息。
![图8-17 打开验证工具](https://img-blog.csdnimg.cn/d9253418c7df4ac5b4cb398a2491ee4b.png)![图8-18 验证应用合法性](https://img-blog.csdnimg.cn/15be21076ca1494ab1b5ad8362c59a6a.png)
验证通过后，即可以将应用布署到J2EE服务器上，==只有部署到J2EE服务器上的应用才可以被客户应用访问，部署后不需要再单独启动特定的应用，J2EE服务器会自动根据客户端的请求情况，管理布署的应用中的构件==。如图8-17所示，点击 `deploytool` 的 `Tools\Deploy` 菜单，可打开布署应用的向导。
![图8-19 打开布署向导](https://img-blog.csdnimg.cn/a95f40e3a85e4af28fd3e4c025d24f3f.png)
在图8-20所示的布署向导界面中，选择要布署的应用，本例中选择 `CurTimeApp` ；选择目标J2EE服务器，本例中选择本机J2EE服务器 `localhost` ；另外在本例中，==由于EJB构件的客户端是独立的Java程序，因此，需要为客户端程序生成「包含支持其访问EJB构件的接口与类文件的 `JAR` 程序包」，该程序包中包含EJB的 `Home` 接口、`Remote` 接口定义以及「客户端访问EJB时需要使用的客户端桩类等Java目标文件」==。在图8-20所示的布署向导界面中，选择 `"Return Client Jar"` ，然后指定对应 `JAR` 程序包的文件名，本例中指定对应 `JAR` 程序包的文件名为 `CurTimeAppClient.jar` ，并将其存放到客户端程序所在的目录下。
![图8-20 选择服务器并生成客户端JAR包](https://img-blog.csdnimg.cn/f11d01b4e2af4f7592c892b8524044df.png)
设置完成后，点击 `Finish` 按钮可完成布署。布署完成后，可以在图8-21所示的 `deploytool` 界面中，看到本机J2EE服务器 `localhost` 上，已经存在一个布署的应用 `CurTimeApp` ，如果希望卸载布署的应用，可用鼠标右键单击该应用，选择 `"Undeploy"` 将其卸载。
![图8-21 已经布署的应用](https://img-blog.csdnimg.cn/47acdb48cfe941ccadccaa33d1b2f1d0.png)
## 8.2.3 开发客户端程序
### 1. 创建客户端程序
本例中编写一个名为 `HClient` 的Java类作为客户端，该类包含一个入口函数 `main` 。该客户端程序调用时间EJB的 `getCurTime` 方法，将返回的时间输出到客户端。客户端程序的代码如程序8-4所示。
```java
// 程序 8-4 时间EJB的客户端程序
import java.rmi.*;
import javax.naming.*;
import java.util.*;
import javax.rmi.*;
import javax.ejb.*;
public class HClient {
	public static void main(String[] args) {
		try {
			InitialContext Init = new InitialContext();
			CurTimeHome Home = (CurTimeHome)Init.lookup("CurTimeBean");
			CurTime CurTimeObj = Home.create();
			String RetVal = CurTimeObj.getCurTime();
			System.out.println("Returned: " + RetVal);
			CurTimeObj.remove();
		} catch(java.rmi.RemoteException exception){
			System.out.println("Remote exception occurred: " + exception);
		} catch(javax.ejb.CreateException exception){
			System.out.println("Create exception occurred: " + exception);
		} catch(javax.ejb.RemoveException exception){
			System.out.println("Remove exception occurred: " + exception);
		} catch(javax.naming.NamingException exception){
			System.out.println("Naming exception occurred: " + exception);
		}
	}
｝
```
在程序8-4所示的客户端程序中，首先利用 `JNDI` 服务，查找时间EJB对应的 `Home` 接口，**`JNDI` 服务由类 `InitialContext` 提供**。查找到 `Home` 接口后，客户端调用 `Home` 接口中的 `create` 操作来获取一个可用的EJB，该操作返回一个 `Remote` 接口，客户端利用返回的 `Remote` 接口，调用EJB上的操作 `getCurTime` 。使用完EJB后，客户端程序调用 `Remote` 接口中的 `remove` 操作，通知服务端其不再使用该EJB。

应注意，**使用完EJB后，客户端并没有通过 `Home` 接口中的 `remove` 操作来删除EJB对象**，因为对于无状态会话构件来说，实例的创建和删除都是由容器自动来控制的，并不是客户端每调用一次 `Home` 接口中的 `create` 都会创建一个新的实例，容器也不允许客户端调用 `Home` 接口中的 `remove` 方法来删除实例。**`Remote` 接口中也包含一个 `remove` 方法，对于无状态会话构件来说，调用 `Remote` 接口中的方法仅仅通知容器不再使用该EJB对象了，并不一定会删除EJB的对象**，另外，`Remote` 接口中的 `remove` 方法比 `Home` 接口中的 `remove` 方法简单，不需提供任何参数。
### 2. 编译/运行客户端程序
编写完客户端程序后，即可使用Java语言编译器 `javac` 对客户端程序进行编译，**编译时必须将「布署时生成的客户端 `JAR` 程序包」包含在 `CLASSPATH` 中，因为客户端程序中使用了时间EJB构件的 `Home` 接口与 `Remote` 接口**，而布署时已经将「所需接口的定义」打包在了「客户端 `JAR` 程序包」中。本例中可使用以下命令完成对客户端程序的编译：

```bash
prompt> javac -classpath "CurTimeAppClient.jar;%CLASSPATH%" HClient.java
```
编译完客户端程序后，即可使用 `java` 命令执行客户端程序，类似地，**执行时也必须将「布署时生成的客户端 `JAR` 程序包」包含在 `CLASSPATH` 中**，因为客户端程序中使用了时间EJB构件的 `Home` 接口与 `Remote` 接口，完成调用时需要客户端桩类的支持，而布署时已经将「所需接口与客户端桩类的定义」打包在了「客户端 `JAR` 程序包」中。本例中可使用以下命令执行客户端程序：
```bash
prompt> java -classpath "CurTimeAppClient.jar;%CLASSPATH%" HClient
```
运行客户端程序时，客户端输出EJB构件返回的时间信息，例如：
```bash
Returned: 2008-01-21 17:09:04.842
```
第一次运行客户端程序，由于EJB容器中没有任何时间EJB的对象，因此**容器会自动创建一个该EJB对象供客户端使用**。创建EJB对象后，**容器会调用该对象的 `ejbCreate` 方法完成对象的初始化**，我们可以在服务端界面上（J2EE服务器的运行窗口）看到程序8-3中 `ejbCreate` 方法中输出的信息，如下所示：
```bash
******CurTimeBean ejbCreate
```

### 3. 客户端程序中的异常处理
异常是一种特殊的Java类，描述了一段代码执行过程中发生的运行时错误。本例客户端程序执行的过程中，可能会发生的异常及可能的原因如下：
- `ClassCastException` ：该异常包含在 `java.lang` 包中。如果客户程序执行时发生了 `ClassCastException` 异常，一般表明**客户程序不能访问部署时生成的客户端 `JAR` 文件**，此时应检查在部署时是否正确生成了客户端 `JAR` 文件。
- `NameNotFoundException` ：该异常包含在 `javax.naming` 包中。如果**J2EE服务器不能找到 `JNDI` 名字相关的EJB构件**时，会抛出该异常；引发该异常的可能的原因包括，EJB没有部署到服务器上、或EJB构件的 `JNDI` 名错误等。
- `NamingException` ：该异常包含在 `javax.naming` 包中。如果**客户程序不能访问J2EE服务器提供的服务（如命名服务）**，会抛出该异常。引发该异常的可能的原因包括J2EE服务器没有启动等。
- `NoClassDefFoundError` ：该异常包含在 `java.lang` 包中。如果**客户程序不能访问定义在客户端 `.class` 文件或 `j2ee.jar` 文件中的类**时，会抛出该异常。引发该异常的可能的原因包括，客户端某些Java文件未编译成功、`J2EE_HOME` 环境变量设置不正确以及 `j2ee.jar` 在未包含在虚拟机的 `ClassPath` 中等。

## 8.2.4 关于例子程序的进一步讨论
==由于无状态会话构件不保存与特定客户端相关的会话状态，其实例可以被多个客户端共享，因此容器会尽可能使用少量的实例、为多个客户端提供服务==。可能已经发现：
- 尽管客户端程序执行结束时调用了 `Remote` 接口中的 `remove` 操作，但是容器并不会删除客户端使用的EJB实例（即EJB对象的 `ejbRemove` 方法并未被调用），因为容器要将该对象保留下来供后续使用。
- 如果再次运行客户端程序，尽管再次调用了 `Home` 接口中的 `create` 方法，但是容器并不会再次创建新的EJB对象（即 `ejbCreate` 不会再次被调用），而是让客户端使用上次运行创建的对象。

连续再次运行客户端程序，对应的客户端输出与服务端输出信息如下：
```bash
# 客户端输出：
prompt> java -classpath "CurTimeAppClient.jar;%CLASSPATH%" HClient
Returned: 2008-01-21 17:09:04.842
prompt> java -classpath "CurTimeAppClient.jar;%CLASSPATH%" HClient
Returned: 2008-01-21 17:09:30.521
```
```bash
# 服务端输出：
******CurTimeBean ejbCreate
```
容器会自动管理EJB构件的生命周期，以满足多个客户端访问的需要，==对于无状态会话构件来说，当存在多个客户端同时访问某构件时，容器会创建该构件的多个对象、以并发地为多个客户端提供服务==，可以修改程序8-4给出的客户端程序，模拟出「多个客户端并发使用时间EJB构件」的情况，此时即可看到容器创建多个时间EJB对象的情形（即 `ejbCreate` 方法被多次调用）。==如果特定时刻大规模的并发访问，导致容器创建了大量的某无状态会话构件的对象，则当并发的访问量降低时，容器会自动删除一部分对象，以降低对系统资源（如系统内存）的消耗==，此时即可看到容器删除时间EJB对象的情形（即 `ejbRemove` 方法被调用）。

---
# 8.3 开发与使用有状态会话构件
本节通过一个简单的例子，演示如何开发与使用有状态会话构件，例子程序分为客户端与服务端两部分，在服务端，将构建一个名为 `ShoppingApp` 的J2EE应用，该应用中仅包含一个有状态会话构件，该构件实现网上购物系统中购物车的基本功能；客户端通过远程接口调用构件上的操作，将结果输出。
## 8.3.1 开发EJB构件
该EJB构件实现网上购物系统中购物车的基本功能，包括添加商品、去除商品、查找商品、清空购物车、提交商品等。由于该构件的实例（对象）需要保存「与特定客户端相关的会话状态」，即特定客户所选择的商品等相关信息，因此设计为有状态的会话构件。以下简称该EJB构件为购物车EJB。
### 1. 定义 `Remote` 接口
`Remote` 接口包含**EJB构件实现的商业方法的声明**，客户端只能通过 `Remote` 接口访问构件实现的商业方法，不能直接调用。程序8-5给出了购物车EJB的 `Remote` 接口定义：
```java
// 程序 8-5 购物车EJB的 Remote 接口定义
package Shopping;
import javax.ejb.*;
import java.rmi.*;
public interface ShoppingBag extends EJBObject {
	public void addCom(Commodity comm) throws RemoteException;
	public void removeComm(Commodity comm)
		throws RemoteException, NoSuchCommodityException;
	public Commodity find(String commID)
		throws RemoteException, NoSuchCommodityException;
	public void clearBag() throws RemoteException;
	public void commit() throws RemoteException, BagEmptyException;
}
```
在程序8-5中，我们定义了一个名为 `ShoppingBag` 的 `Remote` 接口，购物车EJB构件向客户端提供添加商品、去除商品、查找商品、清空购物车、提交商品等商业方法，`Remote` 接口中包含这些方法的声明：
- `addCom` ：添加商品。调用者提供一个表示商品信息的参数，没有返回值。
- `removeComm` ：去除商品。调用者提供一个表示商品信息的参数，没有返回值，如果对应商品信息在购物车中不存在，则抛出 `NoSuchCommodityException` 。
- `find` ：查找商品。调用者提供一个表示商品标识的参数，返回购物车中对应的商品信息，如果对应商品信息在购物车中不存在，则抛出 `NoSuchCommodityException` 。
- `clearBag` ：清空购物车。将购物车中所有商品清空，没有参数与返回值。
- `commit` ：提交商品。将购物车中当前的商品提交，没有参数与返回值，如果购物车中没有任何商品，则抛出 `BagEmptyException` 异常。

可以看到，按照EJB规范的约定，接口 `ShoppingBag` 继承了接口 `EJBObject`，接口中的每个操作均抛出 `RemoteException` 异常、以报告远程调用错误，所有操作的参数与返回值均为合法的 `Java RMI` 类型（`Commodity` 类具体定义详见程序8-8）。

### 2. 定义 `Home` 接口
`Home` 接口中包含**EJB构件生命周期管理的相关方法**，客户程序使用 `Home Interface` 创建、查找或删除EJB的实例。程序8-6给出了购物车EJB的 `Home` 接口定义：
```java
// 程序 8-6 购物车EJB的 Home 接口定义
package Shopping;
import javax.ejb.*;
import java.rmi.*;
public interface ShoppingBagHome extends EJBHome {
	public ShoppingBag create(String customerName)
		throws RemoteException, CreateException;
}
```
在程序8-6中，我们定义了一个名为 `ShoppingBagHome` 的 `Home` 接口，按照EJB规范的约定，接口 `ShoppingBagHome` 继承了接口 `EJBHome` 。该接口中仅声明了一个 `create` 方法，其返回值为对应EJB构件的 `Remote` 接口类型—— `ShoppingBag` ，同时 `create` 方法抛出 `RemoteException` 和 `CreateException` 异常。

和无状态的会话 `bean` 不同，**有状态会话构件所使用的 `Home` 接口中，可以包含若干个带不同参数列表的 `create` 方法**。客户端使用每一个有状态会话构件之前，可以利用一个 `create` 方法来对其初始化。本例中的 `create` 方法，支持客户端使用购物车之前指定使用者的名字。
### 3. 定义 `Enterprise Bean` 类
购物车EJB的 `Enterprise Bean` 类首先要**按照 `Remote` 接口的约定实现商业方法**，其次要**实现 `Home` 接口中 `create` 方法对应的 `ejbCreate` 方法**与**会话构件生命周期相关的方法**。程序8-7给出了购物车EJB的 `Enterprise Bean` 类定义：
```java
// 程序 8-7 购物车EJB的 Enterprise Bean 类定义
package Shopping;
import javax.ejb.*;
import java.util.*;
public class ShoppingBagBean implements SessionBean {
	SessionContext Context;
	String curCustomer;
	Hashtable curCommodities;
	public void addCom(Commodity comm) {
		Commodity nowComm = (Commodity)curCommodities.get(comm.sCommodityID);
		if(nowComm == null) {
			curCommodities.put(comm.sCommodityID, comm);
		} else {
			nowComm.iCount += comm.iCount;
		}
	}
	
	public void removeComm(Commodity comm) throws NoSuchCommodityException {
		Commodity nowComm = (Commodity)curCommodities.get(comm.sCommodityID);
		if (nowComm == null)
			throw new NoSuchCommodityException("No such commodity with id: " + 
				comm.sCommodityID);
		nowComm.iCount -= comm.iCount;
		if (nowComm.iCount <= 0)
			curCommodities.remove(comm.sCommodityID);
	}
	
	public Commodity find(String commID) throws NoSuchCommodityException {
		Commodity nowComm = (Commodity)curCommodities.get(commID);
		if (nowComm == null)
			throw new NoSuchCommodityException("No such commodity with id: " + commID); 
		return nowComm;
	}
	
	public void clearBag() {
		curCommodities.clear();
	}
	
	public void commit() throws BagEmptyException {
		if (curCommodities.isEmpty())
			throw new BagEmptyException("Bag is empty");
		System.out.println("Customer name is: " + curCustomer);
		System.out.println("\tID\t\tName\t\tSpec\t\tPrice\t\tCount");
		Enumeration enums = curCommodities.elements();
		while (enums.hasMoreElements()) {
			Commodity comm = (Commodity)enums.nextElement();
			System.out.println("\t" + comm.sCommodityID + "\t\t" + comm.sName +
			"\t\t" + comm.sSpec + "\t\t" + comm.fPrice + "\t\t" + comm.iCount);
		}
		curCommodities.clear();
	}
	
	public void ejbCreate(String customerName) {
		curCustomer = customerName;
		curCommodities = new Hashtable();
		System.out.println("\n\n******ShoppingBagBean ejbCreate with name:"
			+ customerName);
	}
	
	public void ejbRemove() {
		curCommodities.clear();
		System.out.println("ShoppingBagBean ejbRemove");
	}
	
	public void ejbPassivate() {
		System.out.println("ShoppingBagBean ejbPassivate");
	}
	
	public void ejbActivate() {
		System.out.println("ShoppingBagBean ejbActivate");
	}
	
	public void setSessionContext(SessionContext Context) {
		this.Context = Context;
	}
}
```
在程序8-7中，我们定义了一个名为 `ShoppingBagBean` 的 `Enterprise Bean` 类，**由于该EJB构件是会话构件，因此 `Enterprise Bean` 类实现（`implements`）`SessionBean` 接口**。==由于有状态会话构件要保存与特定客户端交互的中间状态，而对于有状态会话构件来说，容器保证每个EJB对象/实例被某一个客户端所专用，因此保存状态最方便的地方，就是在对象的数据成员/属性中，因为每个对象会使用一份独立的数据成员==。因此，购物车EJB的 `Enterprise Bean` 类中设置了数据成员 `curCustomer` 与 `curCommodities` ，其中数据成员 `curCustomer` 用于记录当前购物车使用者的名字，数据成员 `curCommodities` 用于记录使用者当前选择的商品的相关信息。

该 `Enterprise Bean` 类首先按照 `Remote` 接口 `ShoppingBag` 的约定实现了商业方法：
- `addCom` ：添加商品。在数据成员 `curCommodities` 查找是否存在该种商品，如果存在则修改相应的商品数量，否则添加该种商品信息。
- `removeComm` ：去除商品。在数据成员 `curCommodities` 查找该种商品信息，修改相应的商品数量，如果数量为 $0$ ，则去掉该种商品信息；如果对应商品信息在购物车中不存在，则抛出 `NoSuchCommodityException`。
- `find` ：查找商品。在数据成员 `curCommodities` 查找该种商品信息，将商品信息返回；如果对应商品信息在购物车中不存在，则抛出 `NoSuchCommodityException`。
- `clearBag` ：清空购物车。将数据成员 `curCommodities` 中所有商品清空。 
- `commit` ：提交商品。为简单起见，本例中仅将使用者选择的商品输出到屏幕，输出的信息包括数据成员 `curCustomer` 保存的使用者的名字以及数据成员 `curCommodities` 中保存的商品的详细信息。

其次，`Enterprise Bean` 类实现了 `Home` 接口 `ShoppingBagHome` 中的 `create` 方法对应的 `ejbCreate` 方法，容器调用 `ejbCreate` 方法完成EJB对象的初始化，本例中 `ejbCreate` 方法会在使用者使用一个购物车之前，为其准备好一个全新的购物车——记录使用者的名字、并初始化商品信息列表 `curCommodities` 。此外，还实现了会话构件生命周期相关的 `ejbRemove, ejbPassivate, ejbActivate, ejbSetSessionContext` 方法，其中**有状态会话构件需要使用 `ejbPassivate` 与 `ejbActivate` 方法，`ejbPassivate` 方法会在「有状态会话构件对象被转移到持久存储介质」之前被调用，`ejbActivate` 方法会在「有状态会话构件对象从持久存储介质被加载入内存中」之后被调用**。

如图8-22所示，有状态会话构件的生命周期包含三个状态：方法就绪状态 `Method Ready State` 、不存在状态 `No State` 与钝化状态 `Passivated State` 。
- 不存在状态表明，EJB容器中不存在对应有状态会话构件的实例，处于不存在状态的实例还未被创建；
- 方法就绪状态表明，对应有状态会话构件对象已被创建并处于就绪状态，可以为客户端提供服务；
- 钝化状态表明，对应有状态会话构件对象已被转移至持久存储介质，暂时不能使用。
![图8-22 有状态会话构件生命周期](https://img-blog.csdnimg.cn/534f0a89e97942b4a335cbde8dabb6dd.png)

与无状态会话构件相比，有状态会话构件的生命周期增加了一个钝化状态，这是因为==有状态会话构件需要保存与特定客户端相关的中间状态，因此每个实例/对象都是被一个客户端所专用的，这就使得每个客户端都需要一个专门的有状态会话 `bean` 来为它服务，则很有可能在服务端同时存在大量的EJB实例，从而导致服务端内存开销太大==。为了限制服务端内存使用总量：
- 当EJB实例的数量过多时，容器仅仅会在内存中保留那些正在使用、或者刚被使用的实例，会把其它的实例转移到持久存储介质上（不是删除），此时被转移到持久存储介质上的实例，会从方法就绪状态进入钝化状态。
- 当客户端再次使用处于钝化状态的EJB实例时，容器会把该实例从持久存储介质中恢复到内存中，此时该实例从钝化状态进入方法就绪状态。
- **当客户端出现超时时，容器会把持久存储介质中的实例删除掉，该实例进入不存在状态**。

对于有状态会话构件来说，只要有新的客户端请求（调用 `Home` 接口中的 `create` 方法），容器就会创建新的实例，此时新创建的实例从不存在状态进入方法就绪状态。如果客户端不再使用某个有状态会话构件对象了（调用 `Remote` 接口中的 `remove` 方法），或者客户程序出现超时现象，容器就会删除该实例，此时被删除的实例又从方法就绪状态状态进入不存在状态。

从不存在状态进入方法就绪状态（创建新实例），会调用 `Enterprise Bean` 类对象的  `setSessionContext` 和 `ejbCreate` 方法（**每一次客户端调用 `Home` 接口中的 `create` ，都会导致容器调用 `Enterprise Bean` 类对象的 `ejbCreate` 方法**）；从方法就绪状态进入不存在状态（删除实例），会调用 `ejbRemove` 方法（**每次客户端调用 `Remote` 接口中的 `remove` 方法，都会导致容器调用 `Enterprise Bean` 类对象的 `ejbRemove` 方法**）；在方法就绪状态和钝化状态之间切换时，容器会调用 `ejbPassivate` 和 `ejbActivate` 方法。
### 4. 其它辅助代码
除了购物车EJB相关的 `Remote` 接口、`Home` 接口与 `Enterprise Bean` 类之外，本例还需要三个辅助的Java类：商品类 `Commodity` 和两个辅助的异常类`NoSuchCommodityException` 与 `BagEmptyException` 。这三个类的代码如程序8-8、8-9和8-10所示。
```java
// 程序 8-8 商品类 Commodity 的定义
package Shopping;
public class Commodity implements java.io.Serializable {
	public String sCommodityID;
	public String sName;
	public String sSpec;
	public float fPrice;
	public int iCount;
}
// 程序 8-9 异常类 NoSuchCommodityException 的定义
package Shopping;
public class NoSuchCommodityException extends Exception {
	public NoSuchCommodityException(String msg) {
		super(msg);
	}
}
// 程序 8-10 异常类 BagEmptyException 的定义
package Shopping;
public class BagEmptyException extends Exception {
	public BagEmptyException(String msg){
		super(msg);
	}
}
```
### 5. 编译源代码
本例服务端编译时，需要将EJB构件相关的 `Remote` 接口、`Home` 接口、`Enterprise Bean` 类和辅助类 `Commodity` 、辅助异常 `NoSuchCommodityException` 与 `BagEmptyException` 均进行编译。
### 6. 打包/布署EJB
本例中打包与布署EJB构件的过程与上节类似，不同的地方主要有几下几点：
- 本例将包含EJB构件的J2EE应用命名为 `ShoppingApp` 。
- 本例中打包EJB构件时，需要将EJB构件相关的 `Remote` 接口、`Home` 接口、`Enterprise Bean` 类和辅助类 `Commodity` 、辅助异常 `NoSuchCommodityException` 与 `BagEmptyException` 均添加到对应的EJB模块中，如图8-23所示：
![图8-23 将EJB构件相关的Java目标文件添加到模块中](https://img-blog.csdnimg.cn/53a141999b214ed9847b81ab2b53eaac.png)
- 本例中将EJB构件类型设为有状态会话构件，如图8-24所示：
![图8-24 将EJB构件类型设置为有状态会话构件](https://img-blog.csdnimg.cn/98b12b3848ee4963a2ddef267e360d89.png)

- 为购物车EJB配置名为 `"ShoppingBag"` 的 `JNDI` 名。
- 布署时生成名为 `"StatefulAppClient.jar"` 的客户端 `JAR` 文件包，并将其存放到后面客户端程序所在的目录下。

## 8.3.2 开发客户端程序
### 1. 创建客户端程序
本例中编写一个名为 `StatefulClient` 的Java类作为客户端，该类包含一个入口函数 `main` 。该客户端程序调用购物车EJB的商业方法，完成商品的添加、去除与查找，最后将选择的商品提交。客户端程序的代码如程序8-11所示。
```java
// 程序8-11 购物车EJB的客户端程序
import java.rmi.*;
import javax.naming.*;
import java.util.*;
import javax.rmi.*;
import javax.ejb.*;
import Shopping.*;
public class StatefulClient {
	public static void main(String[] args) {
		try {
			InitialContext Init = new InitialContext();
			ShoppingBagHome Home = (ShoppingBagHome)Init.lookup("ShoppingBag");
			ShoppingBag ShoppingBagObj = Home.create("Mr. Zhang");
			Commodity comm = new Commodity();
			comm.sCommodityID = "10000001"; comm.sName = "Phone";
			comm.sSpec = "T29"; comm.fPrice = 1000; comm.iCount = 1;
			ShoppingBagObj.addCom(comm);
			comm.sCommodityID = "10000002"; comm.sName = "Camera";
			comm.sSpec = "D300"; comm.fPrice = 1500; comm.iCount = 2;
			ShoppingBagObj.addCom(comm);
			try {
				comm = ShoppingBagObj.find("10000001");
				System.out.println(
					"we have commodities with id 10000001, name is " + comm.sName);
			} catch(NoSuchCommodityException e) {
				System.out.println(e.getMessage());
			}
			try {
				ShoppingBagObj.removeComm(comm);
			} catch(NoSuchCommodityException e) {
				System.out.println(e.getMessage());
			}
			try {
				comm = ShoppingBagObj.find("10000001");
				System.out.println(
					"we have commodities with id 10000001, name is " + comm.sName);
			} catch(NoSuchCommodityException e) {
				System.out.println(e.getMessage());
			}
			try {
				ShoppingBagObj.commit();
			} catch(BagEmptyException e){
				System.out.println(e.getMessage());
			}
			ShoppingBagObj.remove();
		} catch(java.rmi.RemoteException exception){
			System.out.println("Remote exception occurred: " + exception);
		} catch(javax.ejb.CreateException exception){
			System.out.println("Create exception occurred: " + exception);
		} catch(javax.ejb.RemoveException exception){
			System.out.println("Remove exception occurred: " + exception);
		} catch(javax.naming.NamingException exception){
			System.out.println("Naming exception occurred: " + exception);
		}
	}
｝
```
在程序8-11所示的客户端程序中，首先利用 `JNDI` 服务查找购物车EJB对应的 `Home` 接口。查找到 `Home` 接口后，客户端调用 `Home` 接口中的 `create` 操作来获取一个可用的EJB，该操作返回一个 `Remote` 接口，客户端利用返回的 `Remote` 接口调用EJB上的商业方法 `addCom, RemoveComm, find, commit` 等。使用完EJB后，客户端程序调用 `Remote` 接口中的 `remove` 操作，通知服务端其不再使用该EJB。

应注意，==本例中客户端使用的是一个有状态会话构件，每个有状态会话构件的实
例/对象被一个特定客户端所专用，并为其保存交互的中间状态==。在后面的讨论中，将关注程序8-11中再次同样参数调用EJB的 `find` 操作的结果对比。
### 2. 编译/运行客户端程序
编写完客户端程序后，即可使用Java语言编译器 `javac` ，对客户端程序进行编译，编译时必须将「布署时生成的客户端 `JAR` 程序包」包含在 `CLASSPATH` 中，因为客户端程序中使用了「购物车EJB构件的 `Home` 接口与 `Remote` 接口」，而布署时已经将「所需接口的定义」打包在了「客户端 `JAR` 程序包」中。本例中可使用以下命令完成对客户端程序的编译：
```bash
prompt> javac -classpath "StatefulAppClient.jar;%CLASSPATH%" StatefulClient.java
```
编译完客户端程序后，即可使用 `java` 命令执行客户端程序，类似地，执行时也必须将「布署时生成的客户端 `JAR` 程序包」包含在 `CLASSPATH` 中，因为客户端程序中使用了「购物车EJB构件的 `Home` 接口与 `Remote` 接口」，完成调用时需要客户端桩类的支持，而布署时已经将「所需接口与客户端桩类的定义」打包在了「客户端 `JAR` 程序包」中。本例中可使用以下命令执行客户端程序：
```bash
prompt> java -classpath "StatefulAppClient.jar;%CLASSPATH%" StatefulClient
```
运行客户端程序时，客户端输出两次调用EJB的 `find` 操作的结果，例如：
```bash
we have commodities with id 10000001, name is Phone
No such commodity with id: 10000001
```
由于本例中，客户端使用的是一个有状态会话构件，因此在程序8-11所示的客户端程序中，尽管包含再次同样参数的对方法 `find` 的调用，其调用的结果却因对应购物车对象的状态不同而完全不同。

第一次运行客户端程序，容器会自动创建一个该EJB对象供客户端使用。创建EJB对象后，容器会调用该对象的 `ejbCreate` 方法完成对象的初始化，我们可以在服务端界面上（J2EE服务器的运行窗口）看到程序8-7中 `ejbCreate` 方法中输出的信息；当客户端调用EJB对象的 `commit` 操作时，我们可以在服务端界面上看到程序8-7中 `commit` 方法中输出的信息；当客户端调用 `Remote` 接口中的 `remove` 操作时，容器会将EJB构件的对应实例删除，此时会调用该对象的 `ejbRemove` 方法，我们可以在服务端界面上看到程序8-7中 `ejbRemove` 方法中输出的信息。第一次运行客户端程序时，服务端的输出信息如下所示：
```bash
******ShoppingBagBean ejbCreate with name:Mr. Zhang
Customer name is: Mr. Zhang
ID Name Spec Price Count
10000002 Camera D300 1500.0
2
ShoppingBagBean ejbRemove
```
## 8.3.3 关于例子程序的进一步讨论
由于有状态会话构件需要保存与特定客户端相关的会话状态，其实例不可以被多个客户端共享，因此**容器会在客户端不再使用某EJB实例时将其删除，而每次有新的客户端请求时，容器会为其创建一个新的实例**。

可能已经发现，与上一节无状态会话构件例子相比，客户端程序调用 `Remote` 接口中的 `remove` 操作，会导致容器删除EJB实例（对应EJB对象的 `ejbRemove` 方法会在实例被删除之前被调用），因为==容器该对象是被特定客户端所专用的，当该客户端不再使用该EJB实例时，该实例已经没有继续存在的必要==。如果再次运行客户端程序，由于再次调用了 `Home` 接口中的 `create` 方法，==容器会认为这是新的客户端请求，会再次创建新的EJB对象==（即 `ejbCreate` 再次被调用）。连续再次运行客户端程序，对应的客户端输出与服务端输出信息如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fd4131b3879f4276baa9180dd6b5a97e.png)
 
容器会自动管理EJB构件的生命周期，以满足多个客户端访问的需要，对于有状态会话构件来说，容器会为每个客户端程序创建一个该构件的对象供其专用。因此，当同时使用该构件的客户端数量很大时（尽管这些客户端一定会并发调用该EJB实例上的操作），只要客户端还需要使用（且未超时），容器必须为其保留对象实例，这就会导致服务端内存中存在大量EJB对象，从而导致服务端内存开销太大。为了限制服务端内存使用总量，==当EJB实例的数量过多时，容器仅仅会在内存中保留那些正在使用、或者刚被使用的实例，会把其它的实例转移到持久存储介质上（**不是删除**），当处于钝化状态的对象再次被使用时，容器会将其从持久存储介质上重新加载到内存中==。

有兴趣可以修改程序8-11给出的客户端程序，如多次调用 `Home` 接口中 `create` 方法、而不调用 `Remote` 接口中 `remove` 方法，模拟出大量客户端同时使用购物车EJB构件的情况，此时即可看到容器创建大量购物车EJB对象的情形。

**对于J2EE参考实现来说，当容器中对象所占用的内存容量超过预设的值时，容器会自动执行钝化操作**。该值在J2EE参考实现安装目录下的 `config` 子目录中的 `default.properties` 配置文件中设置，当前版本的缺省值 $128000000$ 字节，为了进行钝化相关的测试，可将该值改小，`default.properties` 配置文件的可能内容如程序8-12所示，其中钝化使用的内存容量临界值为 `passivation.threshold.memory` 。

```cpp
// 程序8-12 包含钝化使用的内存容量临界值的配置文件
# maximum size of message driven bean instance
# pool per mdb type
messagebean.pool.size=3
# maximum size of a "bulk" message bean delivery
messagebean.max.serversessionmsgs=1
# message-bean container resource cleanup interval
messagebean.cleanup.interval=600
passivation.threshold.memory=128000000
idle.resource.threshold=600
log.directory=logs
log.output.file=output.log
log.error.file=error.log
log.event.file=event.log
distributed.transaction.recovery=false
transaction.timeout=0
transaction.nonXA.optimization=true
sessionbean.timeout=0
# validating parser values
# validating.perser is used when archive file are loaded by
# any of the J2EE Reference Implementation tools.
# deployment.validating.parser is used when deploying an
# archive on the J2EE AppServer.
validating.parser=false
deployment.validating.parser=true
```
**执行钝化操作时，容器会在钝化某个对象前调用其 `ejbPassivate` 方法**，此时可以在服务端界面上看到程序8-7中 `ejbPassivate` 方法中输出的信息。J2EE参考实现的钝化操作，会将有状态会话构件对象以序列化/串行化 `serialize` 的方式写入数据库中，J2EE参考实现钝化时缺省使用的数据库管理系统，是其安装时自带的数据库管理系统 `cloudscape` ，因此要进行钝化测试，需要将 `cloudscape` 数据库管理系统启动，启动 `cloudscape` 数据库管理系统的命令如下：
```bash
prompt> cloudscape -start
```
J2EE参考实现会将对象写入 `clouscape` 的缺省数据库
`CloudscapeDB` 的 `SESSIONBEANTABLE` 表中。有兴趣就可以通过 `cloudscape` 的交互式SQL查看该表中的数据。进入 `cloudscape` 交互式SQL的命令如下：
```bash
prompt> cloudscape -isql
```
在 `cloudscape` 交互式SQL中执行以下命令，可以查看 `SESSIONBEANTABLE` 表中数据：
```bash
ij> select * from SESSIONBEANTABLE;
```
关闭 `clouscape` 数据库管理系统的命令如下：
```bash
prompt> cloudscape -stop
```
==如果客户端调用了某个「已经被钝化的有状态会话构件」实例上的操作，容器会将该对象重新从数据库中加载到内存中，加载完成后容器会调用该对象的 `ejbActivate` 方法==，此时我们可以看到程序8-7中 `ejbActivate` 方法输出的信息。

通过 `ejbPassivate` 与 `ejbActivate` 方法的调用时机可以看出，开发人员可以在 `ejbPassivate` 方法暂时释放占用的一些系统资源，因为对象进入钝化状态后暂时不能被使用，而此时释放的系统资源可以在 `ejbActivate` 方法中重新取回，因为此时对象又可以使用了。

---
# 8.4 开发与使用实体构件
## 8.4.1 实体构件与持久化技术
**在EJB体系结构中，实体构件主要用来封装数据库操作**。从软件体系结构的角度讲，如图8-25所示，==实体构件组成了数据层与业务逻辑层之间的一层——持久层，持久层的实体构件向业务逻辑构件（如会话构件）屏蔽数据库开发的复杂性，为业务逻辑构件方便访问数据库提供面向对象的封装==。基于实体构件的支持，业务逻辑构件以对象的方式看待与处理数据库中的数据，从而达到简化数据库开发的目的。
![图8-25 实体构件组成的持久层](https://img-blog.csdnimg.cn/f9fc9f95a7804f84b804a1a19e1cb582.png)
在Java技术的发展过程中，基于Java的数据持久化技术也得到了长足发展，目前常用的Java持久化实现方案主要包括：
- 基于 `DAO` 和 `JDBC` 实现：这种方案通过数据访问对象 `Data Access Object, DAO` 来实现数据的持久化操作，具体实现时，`DAO` 通过 `JDBC` 来完成对数据库的访问。这种方案要求开发人员对 `JDBC` 的底层信息要比较熟悉。
- 基于 `ORM` 实现：`ORM` 的全称为 `Object Relational Mapping` ，其基本思想是：**将关系型数据库中的数据利用某种机制映射为Java对象**，在业务逻辑构件看来，数据库中的数据以Java对象的形式出现，通常每个对象对应数据库中的一条记录，因此数据库操作也就转换成了对Java对象的操作。而**这种数据与Java对象之间的映射，通常可以获得自动化机制的支持，从而将开发人员从基于 `JDBC` 的复杂开发中解脱出来**。EJB 中的实体构件提供的持久化实现方案就属于这一种，其它比较常用的还有 `Hibernate` 方案等。

到目前为止，在EJB规范的三次主要版本 `1.x, 2.x, 3.x` 变化中，每次主要版本升级都对实体构件相关规范进行了较大修改。应注意，**不同版本规范下的实体构件编程模式有所不同**，下面通过两个简单的例子，演示如何进行 `EJB 1.x, EJB 2.x` 实体构件开发，`EJB 3.0` 为实体构件提供了更为强大的 `ORM` 映射机制，与 `Hibernate` 有较大类似，在[第9章]()介绍 `EJB 3.0` 时进行简单的讨论。

EJB中的实体构件又分为**容器维护的持久性** `Container Managed Persistence, CMP` **构件**与 **`Bean` 维护的持久性** `Bean Managed Persistence, BMP` **构件**，其中 `CMP` 构件的相关数据库操作由容器自动完成，`BMP` 构件的相关数据库操作由开发人员在构件实现代码中通过 `JDBC` 编程实现，本节主要关注 `CMP` 构件的开发。
## 8.4.2 开发与使用 `EJB 1.1` 实体构件
本小节通过开发一个简单的 `EJB 1.1` 实体构件，实现对数据库税率表中的数据操作的封装。**进行实体构件开发时，通常首先根据数据库表结构，抽象出实体构件的结构**，本例中涉及的税率表中包含州代码与税率两个字段，其结构如图8-26所示。以下简称该EJB构件为税率。
![图8-26 税率表的结构](https://img-blog.csdnimg.cn/deb25a6b645f4f7a95da3265ca74e1ab.png)
### 1. 定义 `Remote` 接口
`Remote` 接口包含**EJB构件实现的商业方法的声明**，==对于实体构件来说，由于实体构件的对象代表了数据库表中的记录，因此 `Remote` 接口实际上包含「数据库记录上能够执行的操作」的声明==。程序8-13给出了税率EJB的 `Remote` 接口定义：
```java
// 程序 8-13 税率EJB的 Remote 接口定义
package Data;
import java.rmi.RemoteException;
import javax.ejb.EJBObject;
public interface Tax extends EJBObject {
	public void setTaxRate(float taxRate) throws RemoteException;
	public float getTaxRate() throws RemoteException;
}
```
在程序8-13中，我们定义了一个名为 `Tax` 的 `Remote` 接口，税率EJB构件（代表税率表中的记录）向客户端提供设置税率与获取税率等商业方法，`Remote` 接口中包含这些方法的声明：
- `setTaxRate` ：设置税率。调用者提供一个表示新税率的参数，没有返回值。
- `getTaxRate`：获取税率。没有参数，返回当前记录的税率字段值。

可以看到，按照EJB规范的约定，接口 `Tax` 继承了接口 `EJBObject` ，接口中的每个操作均抛出 `RemoteException` 异常、以报告远程调用错误，所有操作的参数与返回值均为合法的 `Java RMI` 类型。
### 2. 定义 `Home` 接口
`Home` 接口中包含EJB构件生命周期管理的相关方法，客户程序使用 `Home Interface` 创建、查找或删除EJB的实例，==对于实体构件来说，由于实体构件的对象代表了数据库表中的记录，因此 `Home` 接口中的操作实际用于「数据库表中记录的创建（插入）、查找与删除」==。程序8-14给出了税率EJB的 `Home` 接口定义：
```java
// 程序 8-14 税率EJB的 Home 接口定义
package Data;
import java.util.Collection;
import java.rmi.RemoteException;
import javax.ejb.*;
public interface TaxHome extends EJBHome {
	public Tax create(String stateCode, float taxRate)
		throws RemoteException, CreateException;
	public Tax findByPrimaryKey(String primaryKey)
		throws RemoteException, FinderException;
	public Collection findInRange(float lowerLimit, float upperLimit)
		throws RemoteException, FinderException;
}
```
在程序8-14中，我们定义了一个名为 `TaxHome` 的 `Home` 接口，按照EJB规范的约定，接口 `TaxHome` 继承了接口 `EJBHome` 。该接口中声明了以下三个方法：
- `create` ：**对于 `CMP` 实体构件来说，客户端调用 `Home` 接口中的 `create` 方法，会导致在数据库中插入记录**。该操作需要两个分别表示州代码与税率的参数，其返回值为对应EJB构件的 `Remote` 接口类型—— `Tax` ，同时 `create` 方法抛出 `RemoteException` 和 `CreateException` 异常，**通常每个实体构件的 `Home` 接口中，都包含一个类似的 `create` 方法**。
- `findByPrimaryKey` ：根据给定的主键值查找对应记录。由于税率表的主键为州代码字段，因此该操作需要一个表示州代码的参数，由于主键唯一标识一条记录，因此该操作返回一条记录的引用—— `Remote` 接口 `Tax` 。该操作属于查找定位记录的相关操作，**实体构件 `Home` 接口的查找定位操作，需要抛出 `FinderException` ，该操作用于报告查找定位错误**（如未找到符合条件的记录），**通常每个实体构件的 `Home` 接口中都包含一个类似的 `findByPrimaryKey` 方法**。
- `findInRange` ：查找税率在某给定范围 `lowerLimit~upperLimit` 内的记录。该操作也属于查找定位记录的相关操作，与特定表结构和希望执行的操作有关。由于符合条件的记录可能有多条，因此该操作返回一个记录引用（`Remote` 接口 `Tax` ）的集合—— `Collection`。**实体构件的 `Home` 接口中，可根据需要包含若干个类似的查找定位操作**。

与会话构件不同，实体构件的 `Home` 接口中通常包含若干查找定位记录的相关操作（`findByPrimaryKey` 与其它自定义查找定位操作），因为数据库表中的记录经常需要进行查找定位。
### 3. 定义 `Enterprise Bean` 类
税率EJB的 `Enterprise Bean` 类，首先要按照 `Remote` 接口的约定实现商业方法，其次要实现 `Home` 接口中 `create` 方法对应的 `ejbCreate` 方法与实体构件生命周期相关的方法。程序8-15给出了税率EJB的 `Enterprise Bean` 类定义：
```java
// 程序 8-15 税率EJB的 Enterprise Bean 类定义
package Data;
import java.util.*;
import javax.ejb.*;
public class TaxBean implements EntityBean {
	public String stateCode;
	public float taxRate;
	public void setTaxRate(float taxRate){
		this.taxRate = taxRate;
	}
	
	public float getTaxRate(){
		return this.taxRate;
	}

	public String ejbCreate(String stateCode, float taxRate) throws CreateException {
		System.out.println("TaxBean ejbCreate with stateCode=" + stateCode +
			",taxRate=" + taxRate);
		if (stateCode == null) {
			throw new CreateException("The State Code is required.");
		}
		this.stateCode = stateCode;
		this.taxRate = taxRate;
		return null;
	}
	
	public void ejbPostCreate(String stateCode, float taxRate){
		System.out.println("TaxBean ejbPostCreate whith stateCode=" + stateCode
			+ ",taxRate=" + taxRate);
	}

	public void ejbLoad(){
		System.out.println("TaxBean ejbLoad whith stateCode=" + stateCode +
			",taxRate=" + taxRate);
		if (stateCode != null)
			stateCode.trim();
	}
	
	public void ejbStore() {
		System.out.println("TaxBean ejbStore whith stateCode=" + stateCode +
			",taxRate=" + taxRate);
	}

	public void ejbRemove() {
		System.out.println("TaxBean ejbRemove");
	}

	public void unsetEntityContext() {}
	public void setEntityContext(EntityContext context) {}
	public void ejbActivate() {}
	public void ejbPassivate() {}
｝
```
在程序8-15中，我们定义了一个名为 `TaxBean` 的 `Enterprise Bean` 类，**由于该EJB构件是实体构件，因此 `Enterprise Bean` 类实现（`implements`）`EntityBean` 接口**。

**由于实体构件代表数据库表中的记录，因此首先在 `Enterprise Bean` 类中定义与记录的字段一一对应的数据成员**，在本例中，我们定义了与州代码字段对应的数据成员 `stateCode` 和与税率字段对应的数据成员 `taxRate` 。

与会话构件类似，该 `Enterprise Bean` 类也按照 `Remote` 接口 `Tax` 的约定实现了商业方法。==实现商业方法时，由于 `CMP` 实体构件的数据库相关操作由容器自动完成，因此开发人员并不需要编写基于 `JDBC` 的代码来操作数据库，而是将「对应字段上的数据库操作」直接转嫁到「与字段一一对应的数据成员」上来==：
- `setTaxRate` ：设置税率。直接根据参数修改数据成员 `taxRate` 。
- `setTaxRate` ：获取税率。直接将数据成员 `taxRate` 的值返回。

其次，该 `Enterprise Bean` 类还实现了以下实体构件生命周期相关的方法（包括与 `Home` 接口中 `create` 方法对应的 `ejbCreate` 方法）：
- `ejbCreate` ：与 `Home` 接口中的 `create` 方法对应。**对于 `CMP` 实体构件来说，客户端调用 `Home` 接口中的 `create` 方法会导致在数据库中插入记录，容器在插入记录之前会调用对应对象的 `ejbCreate` 方法**。对象通常在 `ejbCreate` 方法中将与字段一一对应的数据成员的值进行设置，为容器完成记录的插入做好准备，本例中根据对数，完成数据成员 `stateCode` 与 `taxRate` 的设置。与会话构件不同，**实体构件的 `ejbCreate` 方法返回对应数据库表的主键类型**，本例中为州代码字段的类型 `String` ，**对于 `CMP` 实体构件来说，由于主键在容器记录插入完成后才生成，因此要求 `ejbCreate` 返回空 `null`** 。
- `ejbPostCreate` ：与 `Home` 接口中的 `create` 方法对应。实体构件要求**其 `Enterprise Bean` 类除了实现与 `create` 方法对应的 `ejbCreate` 之外，还要实现一个对应的 `ejbPostCreate` 方法**，容器在完成记录的插入后会调用该方法。
- `ejbLoad` ：容器将数据从数据库中**读取到实体构件对象中之后**，会调用该对象的 `ejbLoad` 方法，在此方法中可以对读出的数据进行一些预处理，本例在 `ejbLoad` 方法中去除州代码字段中末尾的空白字符。
- `ejbStore` ：容器将数据从实体构件对象中**存储到数据库中之前**，会调用该对象的 `ejbStore` 方法。在此方法中可以对要存储的的数据进行一些预处理。
- `ejbRemove` ：容器将数据从数据库中**删除之前**，会调用对应对象的 `ejbRemove` 方法。在此方法中可以进行一些整理工作， 对于 `CMP` 实体构件，客户调用 `Remote` 接口中的 `remove` 方法会导致删除数据库中的数据。
- `setEntityContext` 与 `unsetEntityContext`：与会话构件类似，`setEntityContext` 方法用来初始化EJB使用的 `EntityContext` 变量，**`EntityContext` 是实体构件与容器交互的入口**，每次创建一个实体构件的对象时，容器会调用该对象的 `setEntityContext` 方法，给对象传入使用的 `EntityContext` 变量；`unsetEntityContext` 方法在容器删除实体构件对象之前被调用，可以在该方法中释放占用的系统资源。
- `ejbActivate` 与 `ejbPassivate`：与有状态会话构件方法类似。

如图8-27所示，实体构件的生命周期包含三个状态：就绪状态 `Ready State` 、不存在状态 `No State` 与池状态 `Pooled State` 。
- 不存在状态表明，EJB容器中不存在对应实体构件的实例，处于不存在状态的实例还未被创建；
- 池状态表明，**实体构件的实例存在于实例池中，容器新创建的实例会进入这个状态**，==处于池状态的实体构件实例不与任何EJB对象关联，其中与数据库记录对应的字段均未实例化，还未和数据库中的记录对应起来，因此不能用于完成相应的数据库操作==。
- 就绪状态表明，**实体构件实例建立了与EJB对象的关联，已经和数据库记录对应起来，可以处理客户应用的请求**。客户端程序调用 `Home` 接口中的方法创建或查找到某个实体构件实例时，该实例会从池状态进入就绪状态。

实体构件生命周期相关方法的调用时机，如图8-27中的标记所示。
![图8-27 实体构件生命周期](https://img-blog.csdnimg.cn/ffc121bf5ed3445bb0bd93a46414e156.png)

### 4. 编译源代码
利用Java语言编译器 `javac` ，对税率EJB的 `Home` 接口、`Remote` 接口与 `Enterprise Bean` 类进行编译。
### 5. 打包EJB
本例中打包EJB构件的基本过程与上节类似，不同的地方主要有几下几点：
- 本例将包含EJB构件的J2EE应用命名为 `TaxBeanApp` 。
- 本例中将EJB类型设置为实体构件 `Entity` ，如图8-28所示：
![图8-28 将EJB构件类型设置为实体构件](https://img-blog.csdnimg.cn/3fe3e5fd69964b0d8c5ee6b35b58c7ea.png)
- 实体构件比会话构件多一页配置界面，用于配置实体构件的持久性相关信息，如图8-29所示，本例中关于持久性相关信息设置为：
	- 选择实体构件类型为 `EJB 1.x` 规范的 `CMP` 构件：在 `"Persistence Management"` 选择 `"Container managed persistence (1.0)"` ；
	- 选择 `stateCode` 与 `taxRate` 均为与数据库表字段对应的数据成员：在 `"Fields To Be Persisted"` 中选中 `stateCode` 与 `taxRate` 前面的复选框；
	- 设置主键类型为字符串类型：在 `"Primary Key Class"` 下面的文本框中输入 `"java.lang.String"` ；
	- 选择主键字段为 `stateCode` ：在 `"Primary Key Field Name"` 下面的列表中选择`stateCode` 。
![图8-29 设置税率EJB的持久性信息](https://img-blog.csdnimg.cn/2a17c4b23735497fabcc2c51003c01f4.png)
- 如图8-30所示，**`CMP` 实体构件的事务控制规则只能容器维护**，由于本例中不讨论事务控制，因此对应方法的事务属性可以全部使用缺省值。
![图8-30 设置税率EJB的事务控制规则](https://img-blog.csdnimg.cn/ac5c757b50934fbdb6b2023419b24fa7.png)
- 为税率EJB配置名为 `"TaxBean"` 的 `JNDI` 名。

### 6. 设置数据库信息
**对于 `CMP` 实体构件而言，需要在布署前设置相关的数据库信息**，本例中需要为税率EJB指定对应数据库表所存储的数据库，生成并补充容器执行数据库操作时需要使用的SQL语句。

首先，要设置实体构件对应表存储所用的数据库，**数据库的设置通过指定相应的数据源 `Data Source` 实现**。==J2EE中的数据源是一种代表数据库的系统资源，通常一个数据源对应一个物理的数据库，通过数据源访问数据库，可以利用前面提到的数据库连接池达到较好的数据库访问效率==。J2EE参考实现中的数据源，可在服务器配置界面中进行配置，如图8-31所示，可通过点击 `deploytool` 的 `Tools\Server Configuration` 菜单打开服务器配置界面：
![图8-31 打开服务器配置界面](https://img-blog.csdnimg.cn/0308a6ca15d84e6da33fff62493b52ee.png)
在图8-32所示的服务器配置界选中 `Data Sources` 下的 `Standard` ，可以看到J2EE服务器上已经配置的缺省数据源，每个数据源包含一个 `JNDI` 名供使用者查找，其中名为 `jdbc/Cloudscape` 的数据源为对应 `cloudscape` 缺省数据库 `CloudscapeDB` 的数据源，本例中将使用该数据源来指定「税率表存储所在的数据库」。
![图8-32 J2EE 服务器上的数据源](https://img-blog.csdnimg.cn/642ac55628764ea08591cb0c6ad58687.png)

了解了要使用的数据源后，选中刚刚打包的税率EJB，在图8-33所示的界面右侧选中 `"Entity"` 选项卡，可以看到刚刚在打包过程中设置的、税率EJB的持久性相关设置，点击 `"Deployment Setting"` 按钮，可打开数据库配置向导。
![图8-33 打开数据库配置界面](https://img-blog.csdnimg.cn/fec9a5a9968341a5a591e15149fe7c11.png)
在图8-34所示的数据库配置界面中，点击 `"Database Settings"` 按钮，并在弹出的对话框中，输入期望使用的数据库对应数据源的 `JNDI` 名，本例中为 `"jdbc/Cloudscape"` ，点击 `OK` 关闭对话框。
![图8-34 数据库配置界面](https://img-blog.csdnimg.cn/c08f97988f5846f3ad976fe0a89bf76c.png)
然后，在图8-34所示的数据库配置界面中，点击 `"Generate Default SQL"` 按钮（该按钮会在设置完数据源后变为可用状态），可以生成容器完成数据库相关操作所使用的缺省SQL语句。选中界面中的 `createRow, createTable` 等方法，可以看到对应操作使用的SQL语句，**在这些方法中，由于 `findInRange` 为自定义的查找方法，容器不可能知道开发人员真正想执行的操作，因此我们需要为该方法补充对应的SQL语句**，如图8-35所示，选中方法 `findInRange` ，并为其补充SQL语句的 `WHERE` 子句 `"WHREE "taxRate" BETWEEN ?1 AND ?2"` 。
![图8-35 为 findInRange 方法补充SQL语句](https://img-blog.csdnimg.cn/394313725af14a38bf429e992cb43d61.png)
### 7. 布署EJB
本例中布署EJB构件的基本过程与上节类似，布署时生成名为 `"StatefulAppClient.jar"` 的客户端 `JAR` 文件包，并将其存放到后面客户端程序所在的目录下。

布署时应注意必须首先启动 `cloudscape` 。布署完成后，可以在 `cloudscape` 数据库中发现一个被自动创建的表 `"TaxBeanTable"` ，这是在布署时、容器自动根据税率EJB的相关信息创建的，该表包含 `stateCode` 与 `taxRate` 两个字段。如果在布署之前已存在对应的数据库表，可以在图8-35所示的界面中，取消 `"Create table on deploy"` 复选框、来选择禁止容器自动创建表。
### 8. 开发/运行客户端程序
本例中，编写一个名为 `TaxClient` 的Java类作为客户端，该类包含一个入口函数 `main` 。该客户端程序利用税率EJB，完成对数据库中税率表中记录的操作。客户端程序的代码如程序8-16所示。

```java
// 程序 8-16 税率EJB的客户端程序
import Data.*;
import java.util.*;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.rmi.PortableRemoteObject;
public class TaxClient {
	public static void main(String[] args) {
		try {
			Context initial = new InitialContext();
			Object objRef = initial.lookup("TaxBean");
			TaxHome home = (TaxHome)PortableRemoteObject.narrow(objRef, TaxHome.class);
			Tax tax = null;
			tax = home.create("IL", 5.00f);
			tax = home.create("CA", 6.25f);
			tax = home.create("FL", 8.50f);
			tax = home.create("CO", 6.75f);
			tax = home.findByPrimaryKey("CA");
			System.out.println("CA tax rate: " + tax.getTaxRate());
			System.out.println("Changing tax rate for CA state");
			tax.setTaxRate(8.25f);
			System.out.println("New CA tax rate: " + tax.getTaxRate());
			Collection taxArray = home.findInRange(5.0f, 7.0f);
			Iterator it = taxArray.iterator();
			while(it.hasNext()) {
				Object objRef2 = it.next();
				tax = (Tax)PortableRemoteObject.narrow(objRef2, Tax.class);
				System.out.println("Tax Rate in " + tax.getPrimaryKey() + ": "
				+ tax.getTaxRate());
				tax.remove();
			}
		} catch(Exception ex) {
			System.err.println("Caught an exception.");
			ex.printStackTrace();
		}
	}
}
```
在程序8-16所示的客户端程序中，首先利用 `JNDI` 服务查找税率EJB对应的 `Home` 接口。查找到 `Home` 接口后，客户端连续调用 `Home` 接口中的 `create` 操作，向数据库中插入四条记录；然后调用 `Home` 接口的 `findByPrimaryKey` 方法定位到主键（州代码）为 `"CA"` 的记录，将其税率修改为 $8.25$ ；最后调用 `Home` 接口的 `findInRange` 方法，查找税率在 $5.0$ 与 $7.0$ 之间的记录（有 `IL` 与 `CO` 两条），将这些记录的信息输出，然后调用 `Remote` 接口的 `remove` 操作将其删除。第一次运行客户端程序，客户端的输出信息如下：
```bash
CA tax rate: 6.25
Changing tax rate for CA state
New CA tax rate: 8.25
Tax Rate in IL: 5.0
Tax Rate in CO: 6.75
```
可进入 `cloudscape` 的交互式SQL，通过执行以下SQL查看 `TaxBeanTable` 中的记录变化：
```sql
ij> select * from "TaxBeanTable";
```
第一次运行客户端之后该表中的数据为：
```bash
stateCode taxRate
CA 8.25
FL 8.5
```
## 8.4.3 开发 `EJB 2.0` 实体构件
`EJB 2.0` 的实体构件与 `EJB 1.1` 的实体构件之间的主要区别如下：
- `Enterprise Bean` 类的区别：在 `EJB 1.1` 中，`Enterprise Bean` 类由开发人员定义，而**在 ``EJB 2.0`` 中，`Enterprise Bean` 类由容器生成，开发人员仅定义一个抽象基类**。
- `Enterprise Bean` 数据成员的区别：
	- 在 `EJB 2.0` 中，与数据库字段对应的 `Bean` 属性不由用户定义，用户仅定义对应的 `set` 和 `get` 方法，**具体属性的定义由容器生成，这样容器可以对属性进行优化**。
	- 在 `EJB 2.0` 的 `CMP` 构件中，还有一种特殊的 `Container Managed Relationship, CMR` 字段，用于关联其它的表（实体构件）。**在组装/部署时，可以设置由容器自动维护表之间的关联关系**。
- 接口区别：`EJB 2.0` 引入了本地接口，**实体构件的进程内客户端，可以通过本地接口获得更好的调用效率**。

下面通过一个简单的例子来演示，如何开发 `EJB 2.0` 的实体构件。我们将开发一个名为 `OrderApp` 的J2EE应用，该应用中包含两个 `EJB 2.0` 的实体构件，分别对应数据库中的订单表与送货地址表，这两个表的结构如图8-36所示。以下简称对应订单表的实体构件为订单EJB，简称对应送货地址表的实体构件为地址EJB。

**订单表与送货地址表之间存在一个外键关联关系**，订单表中并不直接记录该订单的送货地址信息，而是仅记录一个送货地址的编号，具体的送货地址信息在送货地址中存储。

在本例中，订单EJB使用地址EJB存放订单的送货地址信息，**客户端程序仅看到订单EJB，根据订单EJB间接获取或存储送货地址信息**。由于地址EJB仅被订单EJB所使用，而==本例会把两个EJB构件打包在同一个EJB模块中，布署时会被布署到同一个容器内，因此我们可以为地址EJB提供本地接口，订单EJB可通过地址EJB的本地接口，获得较高的访问效率==。
 
![图8-36 订单表与送货地址表结构](https://img-blog.csdnimg.cn/655153a412984a7f8e5f33c904ea597d.png)
### 1. 开发地址EJB
首先定义地址EJB的 `Local` 接口，`Local` 接口完成与 `Remote` 接口类似的功能，本例中地址EJB的 `Local` 接口，约定地址表中记录上能够执行的操作。程序8-17给出了地址EJB的 `Local` 接口定义：
```java
// 程序 8-17 地址EJB的 Local 接口定义
package examples.local_objects;
import javax.ejb.EJBLocalObject;
public interface AddressLocal extends EJBLocalObject {
	public int getAddressID();
	public String getStreet();
	public void setStreet(String street);
	public String getCity();
	public void setCity(String city);
	public String getState();
	public void setState(String state);
	public String getZip();
	public void setZip(String zip);
}
```
在程序8-17中，定义了一个名为 `AddressLocal` 的 `Local` 接口，地址EJB构件（代表地址表中的记录）向客户端提供读取地址标识、读取与设置街道信息、城市信息、州信息与邮编信息等商业方法，`Local` 接口中包含这些方法的声明。

可以看到，按照EJB规范的约定，**接口 `AddressLocal` 继承了接口 `EJBLocalObject` ，由于 `Local` 接口中约定的是本地操作，因此接口中的每个操作不必抛出 `RemoteException` 异常，所有操作的参数与返回值也不必是 `Java RMI` 类型**。

`LocalHome` 接口完成与 `Home` 接口类似的功能。地址EJB的 `LocalHome` 接口用于**约定实现地址表中记录的创建（插入）、查找与删除的操作**。程序8-18给出了地址EJB的 `LocalHome` 接口定义：
```java
// 程序 8-18 地址EJB的 LocalHome 接口定义
package examples.local_objects;
import javax.ejb.*;
public interface AddressLocalHome extends EJBLocalHome{
	public AddressLocal findByPrimaryKey(Integer addressID) throws FinderException;
	public AddressLocal create(int addressID, String street, String city,
	String state, String zip) throws CreateException;
}
```
在程序8-18中，我们定义了一个名为 `AddressLocalHome` 的 `LocalHome` 接口，按照EJB规范的约定，**接口 `AddressLocalHome` 继承了接口 `EJBLocalHome`**。该接口中声明了以下两个方法：
- `create` ：对于 `CMP` 实体构件来说，客户端调用 `LocalHome` 接口中的 `create` 方法，会导致在数据库中插入记录，该操作需要表示详细地址信息的五个参数，**其返回值为对应EJB构件的 `Local` 接口类型**—— `AddressLocal`，同时 **`create` 方法抛出`CreateException` 异常**。
- `findByPrimaryKey` ：根据给定的主键值查找对应记录。由于地址表的主键为地址标识字段，因此该操作需要一个表示地址标识的参数，返回一条记录的引用—— `Local` 接口 `AddressLocal` 。

**对于 `EJB 2.0` 的实体构件而言，开发人员并不定义真正的 `Enterprise Bean` 类，而是仅定义其抽象基类**。程序8-19给出了地址EJB的 `Enterprise Bean` 类的抽象基类定义：
```java
// 程序 8-19 地址EJB的 Enterprise Bean 类的抽象基类定义
package examples.local_objects;
import javax.ejb.*;
public abstract class AddressBean implements EntityBean {
	public abstract int getAddressID();
	public abstract void setAddressID(int addressID);
	public abstract String getStreet();
	public abstract void setStreet(String street);
	public abstract String getCity();
	public abstract void setCity(String city);
	public abstract String getState();
	public abstract void setState(String state);
	public abstract String getZip();
	public abstract void setZip(String zip);
	
	public Integer ejbCreate(int addressID, String street, String city,
		String state, String zip) throws CreateException {
		System.out.println("entry AddressBean ejbCreate");
		setAddressID(addressID);
		setStreet(street);
		setCity(city);
		setState(state);
		setZip(zip);
		return null;
	}
	
	public void ejbPostCreate(int addressID, String street, String city,
		String state, String zip) throws CreateException {}
	public void ejbActivate() {}
	public void ejbLoad() {}
	public void ejbPassivate() {}
	public void ejbRemove() {}
	public void ejbStore() {}
	
	public void setEntityContext(EntityContext ctx) {}
	public void unsetEntityContext() {}
｝
```
在程序8-19中，我们定义了一个名为 `AddressBean` 的抽象类，**由于该EJB构件是实体构件，因此类 `AddressBean` 实现（`implements`）`EntityBean` 接口**。由于实体构件代表数据库表中的记录，因此首先要在类 `AddressBean` 中定义与记录的字段一一对应的数据成员，但是**与 `EJB 1.1` 的实体构件不同，在 `EJB 2.0` 的实体构件中，开发人员并不定义真正的数据成员，而是仅定义数据成员对应的 `set` 与 `get` 方法**——地址表中包含地址标识（ AddressID）、街道信息（Street）、城市信息（City）、州信息（State）与邮编信息（Zip）等五个字段，因此程序8-19中定义了五对相应的 `set` 与 `get` 方法。

由于地址EJB的 `Local` 接口中所约定的方法，均与某个数据成员对应的 `set` 或 `get` 方法相同，因此类 `AddressBean` 不需要提供对应方法的实现，**数据成员对应的 `set` 与 `get` 方法由「容器生成的真正的 `Enterprise Bean` 类」实现**。

类 `AddressBean` 还实现了与 `LocalHome` 接口的 `create` 方法对应的 `ejbCreate` 方法与 `ejbPostCreate` 方法，在 `ejbCreate` 方法中，利用数据成员对应的 `set` 方法将数据成员准备好。此外，类 `AddressBean` 还实现了实体构件相关的、其它生命周期管理相关的方法。
### 2. 开发订单EJB
由于订单EJB要被客户端进行远程调用，因此仍需使用远程接口。订单EJB的 `Remote` 接口约定订单表中记录上能够执行的操作。程序8-20给出了订单EJB 的 `Remote` 接口定义：
```java
// 程序 8-20 订单EJB的 Remote 接口定义
package examples.local_objects;
import java.rmi.RemoteException;
import javax.ejb.EJBObject;
public interface Order extends EJBObject{
	public String getCustomerName() throws RemoteException;
	public AddressValueObject getShipAddressView() throws RemoteException;
}
```
在程序8-20中，我们定义了一个名为 `Order` 的 `Remote` 接口，订单EJB构件（代表订单表中的记录）向客户端提供获取客户姓名与获取送货地址两个商业方法，`Remote` 接口中包含这些方法的声明。应注意，获取送货地址方法并没有返回对应地址EJB的引用，而是将地址信息用一个普通的可串行化对象返回，该串行化对象对应的类定义参阅程序8-23。

可以看到，按照EJB规范的约定，接口 `Order` 继承了接口 `EJBObject` ，接口中的每个操作均抛出 `RemoteException` 异常，所有操作的参数与返回值均为合法的 `Java RMI` 类型。

订单EJB的 `Home` 接口，用于约定实现订单表中记录的创建（插入）、查找与删除的操作。程序8-21给出了订单EJB的 `Home` 接口定义：
```java
// 程序 8-21 订单EJB的 Home 接口定义
package examples.local_objects;
import java.rmi.*;
import javax.ejb.*;
public interface OrderHome extends EJBHome {
	public Order create(long orderID, String customerName, AddressValueObject shipAddress) 
		throws RemoteException, CreateException;
	public Order findByPrimaryKey(Long orderID)
		throws RemoteException, FinderException;
}
```
在程序8-21中，我们定义了一个名为 `OrderHome` 的 `Home` 接口，按照EJB规范的约定，接口 `OrderHome` 继承了接口 `EJBHome` 。该接口中声明了以下两个方法：
- `create` ：**对于 `CMP` 实体构件来说，客户端调用 `Home` 接口中的 `create` 方法，会导致在数据库中插入记录**，该操作需要表示详细订单信息的三个参数（地址对数为一可串行化对象），其返回值为对应EJB构件的 `Remote` 接口类型—— `Order` ，同时 `create` 方法抛出 `RemoteException` 异常与 `CreateException` 异常。
- `findByPrimaryKey` ：根据给定的主键值查找对应记录。由于订单表的主键为订单标识字段，因此该操作需要一个表示订单标识的参数，返回一条记录的引用——  `Remote` 接口 `Order` 。

**对于 `EJB 2.0` 的实体构件而言，开发人员并不定义真正的 `Enterprise Bean` 类，而是仅定义其抽象基类**。程序8-22给出了订单EJB的 `Enterprise Bean` 类的抽象基类定义：
```java
// 程序 8-22 订单EJB的 Enterprise Bean 类的抽象基类定义
package examples.local_objects;
import javax.ejb.CreateException;
import javax.ejb.EntityBean;
import javax.ejb.EntityContext;
import javax.naming.InitialContext;
public abstract class OrderBean implements EntityBean {
	//CMP fields
	public abstract long getOrderID();
	public abstract void setOrderID(long orderID);
	public abstract String getCustomerName();
	public abstract void setCustomerName(String customerName);
	
	//cmr-fields
	public abstract AddressLocal getShipAddress();
	public abstract void setShipAddress(AddressLocal shipAddress);
	
	public AddressValueObject getShipAddressView() {
		AddressLocal shipAddress = getShipAddress();
		return new AddressValueObject(shipAddress.getAddressID(),
			shipAddress.getStreet(),
			shipAddress.getCity(),
			shipAddress.getState(),
			shipAddress.getZip()
		);
	}
	public Long ejbCreate(long orderID, String customerName,
		AddressValueObject shipAddress) throws CreateException {
		System.out.println("entry ejbCreate");
		setOrderID(orderID);
		setCustomerName(customerName); 
		setShipAddress(shipAddress);
		return null;
	}
	private AddressLocal createShipAddress(int addressId, String street,
		String city, String state, String zip) {
		try {
			InitialContext initial = new InitialContext();
			AddressLocalHome home = (AddressLocalHome)
				initial.lookup("java:comp/env/ejb/AddressEJB");
			return home.create(addressId, street, city, state, zip);
		} catch(Exception e) {
			throw new javax.ejb.EJBException(e);
		}
	}
	// public void ejbPostCreate(long orderID, String customerName)
	public void ejbPostCreate(long orderID, String customerName,
		AddressValueObject shipAddressView) throws CreateException {
		AddressLocal shipAddress = createShipAddress(
			shipAddressView.getAddressID(), shipAddressView.getStreet(),
			shipAddressView.getCity(), shipAddressView.getState(),
			shipAddressView.getZip()
		); 
		setShipAddress(shipAddress);
	}
	public void ejbActivate() {}
	public void ejbLoad() {}
	public void ejbPassivate() {}
	public void ejbRemove() {}
	public void ejbStore() {}
	public void setEntityContext(EntityContext ctx) {}
	public void unsetEntityContext() {}
｝
```
在程序8-22中，定义了一个名为 `OrderBean` 的抽象类，由于该EJB构件是实体构
件，因此类 `OrderBean` 实现（`implements`）`EntityBean` 接口。**类 `OrderBean` 中定义与记录的字段一一对应的数据成员相关的 `set` 与 `get` 方法**，订单表中包含订单标识（OrderID）、客户姓名（CustomerName）两个基本字段，因此程序8-22中定义了两对相应的 `set` 与 `get` 方法。

另外，在类 `OrderBean` 定义了一个 `Container Managed Relationship, CMR` 字段对应的一对 `set` 与 `get` 方法，**`CMR` 字段用于和其它的实体构件（其它表中的记录）关联，`CMR` 字段的类型为所关联的实体构件的 `Remote` 或 `Local` 接口**；本例中通过本地接口 `AddressLocal` 访问另一个实体构件——地址EJB。

由于订单EJB的 `Remote` 接口中所约定的方法 `getShipAddressView` 并不属于某个数据成员对应的 `set` 或 `get` 方法，因此类 `OrderBean` 中按照 `Remote` 接口的约定实现了该方法，该方法根据所关联的地址EJB、创建一个可串行化对象返回给调用者。

类 `OrderBean` 还实现了「与 `Home` 接口的 `create` 方法」对应的 `ejbCreate` 方法、与 `ejbPostCreate`方法。在 `ejbCreate` 方法中，利用数据成员对应的 `set` 方法将基本数据成员准备好；在 `ejbPostCreate` 方法中设置 `CMR` 字段的值，具体的方法是：**首先调用地址EJB的 `LocalHome` 接口中的 `create` 方法创建地址记录，然后将对应的 `Local` 接口记录在 `CMR` 字段中**。因为 `CMR` 字段代表两条记录之间的关联，所以 **`CMR` 字段通常在记录插入完成后**（如在 `ejbPostCreate` 方法中）**才设置**。此外，类 `OrderBean` 还实现了实体构件相关的其它生命周期管理相关的方法。

订单EJB中使用的可串行化对象对应的类定义，如程序8-23所示：
```java
// 程序 8-23 订单EJB使用的可串行化对象对应的类定义
package examples.local_objects;
public class AddressValueObject implements java.io.Serializable {
	private int addressID;
	private String street;
	private String city;
	private String state;
	private String zip;
	public AddressValueObject() {}
	public AddressValueObject(int addressID, String street, String city, String state, String zip) {
		this.addressID = addressID;
		this.street = street;
		this.city = city;
		this.state = state;
		this.zip = zip;
	}
	public int getAddressID() {
		return addressID;
	}
	public void setAddressID(int addressID) {
		this.addressID = addressID;
	}
	public String getStreet() {
		return street;
	}
	public void setStreet(String street) {
		this.street = street;
	}
	public String getCity() {
		return city;
	}
	public void setCity(String city) {
		this.city = city;
	}
	public String getState() {
		return state;
	}
	public void setState(String state) {
		this.state = state;
	}
	public String getZip() {
		return zip;
	}
	public void setZip(String zip) {
		this.zip = zip;
	}
}
```
### 3. 打包/布署EJB
本例中打包的基本过程与上节类似，不同的地方主要有几下几点：
- 本例将包含EJB构件的J2EE应用，命名为 `OrderApp` 。
- 首先打包地址EJB，在创建对应EJB模块时，可一次性将两个EJB构件所需的所有Java目标文件加入到EJB模块中，如图8-37所示。
![图8-37 将EJB构件相关的Java目标文件添加到模块中](https://img-blog.csdnimg.cn/d71ffba706084797bbb4fef9f9c1166d.png)
- 将设置地址EJB的类型为实体构件，注意其提供的是本地接口，如图8-38所示：
![图8-38 设置地址EJB的基本信息](https://img-blog.csdnimg.cn/261cfcd618dc40179ab2368d311f4f3d.png)
- 如图8-39所示，配置地址EJB的持久性信息：
	- 选择实体构件类型为 `EJB 2.x` 规范的 `CMP` 构件：在 `"Persistence Management"` 选择 `"Container managed persistence (2.0)"` ；
	- 选择 `state` 等五个字段均为与数据库表字段对应的数据成员：在 `"Fields To Be Persisted"` 中选中每个字段前面的复选框；
	- 设置主键类型为整型：在 `"Primary Key Class"` 下面的文本框中输入 `"java.lang.Integer"` ；
	- 选择主键字段为 `addressID` ：在 `"Primary Key Field Name"` 下面的列表中选择 `addressID` 。
![图8-39 配置地址EJB的持久性信息](https://img-blog.csdnimg.cn/24ee9a15e6f64c3ca589985c2e6c883f.png)
- 打包订单EJB时，选择将其加入已有EJB模块（打包地址EJB时创建的EJB模块）中，如图8-40所示： 
![图8-40 将订单EJB加入已有EJB模块](https://img-blog.csdnimg.cn/ec9fa3dfdbb14ce6b52b9b2d5b91d03a.png)
- 如图8-41所示，配置订单EJB的持久性信息：
	- 选择实体构件类型为 `EJB 2.x` 规范的 `CMP` 构件：在 `"Persistence Management"` 选择 `"Container managed persistence (2.0)"` ；
	- 选择 `customerName` 与 `orderID` 两个字段为与数据库表字段对应的数据成员：在 `"Fields To Be Persisted"` 中选中这两个字段前面的复选框；注意 **`shipAddress` 为 `CMR` 字段，并不与数据库表字段直接对应**。
	- 设置主键类型为长整型：在 `"Primary Key Class"` 下面的文本框中输入 `"java.lang.Long"` ；
	- 选择主键字段为 `orderID` ：在 `"Primary Key Field Name"` 下面的列表中选择 `orderID` 。
![图8-41 配置地址EJB的持久性信息](https://img-blog.csdnimg.cn/898527610dba4e7eb9c54a380f037026.png)
- **因为订单EJB引用了地址EJB，因此在打包订单EJB时，需要声明对地址EJB的引用**。如图8-42所示，通过设定以下信息，完成EJB引用的声明：
	- `Coded Name` ：代码中引用对应EJB时所使用的 `JNDI` 名，本例中为 `"ejb/AddressEJB"` ；
	- `Type` ：所引用的EJB的类型，本例中为实体构件 `Entity` ；
	- `Interfaces` ：所引用的EJB的接口类型，本例中为本地接口 `Local` ；
	- `Home Interface` ： 所引用的EJB的 `Home` 接口名，本例中为 `"examples.local_objects.AddressLocalHome"` ；
	- `Local/Remote Interface` ：所引用的EJB的 `Local` 或 `Remote` 接口名，本例中为 `"examples.local_objects.AddressLocal"` ；
	- `Enterprise Bean Name`：期望真正使用的EJB，本例中为前面打包的地址EJB。
![图8-42 配置EJB引用](https://img-blog.csdnimg.cn/8759f0eeaa6f4fea820222078ac209c4.png)
- 为地址EJB配置名为 `"AddressEJB"` 的 `JNDI` 名，为订单EJB配置名为 `"OrderBean"` 的 `JNDI` 名。
- **利用 `CMR` 字段设置EJB之间的关联**，如图8-42所示，在 `deploytool` 中选中包含地址EJB和订单EJB的EJB模块，点击右边的 `Relationships` 选项卡，点击 `Add` 按钮可打开关联关系设置界面。
![图8-43 添加关联关系](https://img-blog.csdnimg.cn/dd74f199601045b49ab5cdf95fbc99f8.png)
- 在图8-44所示的界面中，利用 `CMR` 字段 `shipAddress` 设置关联关系：
![图8-44 设置关联关系](https://img-blog.csdnimg.cn/a789b2deab674e29834a4c638e60d13b.png)
- 地址EJB与订单EJB均使用数据源 `"jdbc/Cloudscape"` ，且均生成缺省SQL语句即可。
- 布署时生成名为 `"CMP20Client.jar"` 的客户端 `JAR` 文件包，并将其存放到后面客户端程序所在的目录下。布署完成后，会在数据库 `CloudscapeDB` 中创建两张表 `AddressBeanTable` 与 `OrderBeanTable` 。

### 4. 创建客户端程序
本例使用程序8-24给出的客户端程序。
```java
// 程序 8-24 客户端程序
import examples.local_objects.*;
import javax.naming.InitialContext;
import javax.rmi.PortableRemoteObject;
public class ClientDemoDependentObject {
	public static void main(String[] args) {
		try{
			InitialContext initial = new InitialContext();
			OrderHome orderHome = (OrderHome)PortableRemoteObject.narrow(
				initial.lookup("OrderBean"), OrderHome.class);
			AddressValueObject obj1 = new AddressValueObject(25, "1132 Decimal Ave", "Evanston", "IL", "60202");
			Order order = orderHome.create(2, "Puddentane", obj1);
			System.out.println(order.getCustomerName());
			AddressValueObject shipAddress = order.getShipAddressView();
			System.out.println(shipAddress.getStreet());
			System.out.println(shipAddress.getCity());
			System.out.println(shipAddress.getState());
			System.out.println(shipAddress.getZip());
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
```
客户端程序第一次执行结束后，会在订单表与地址表中分别插入一条记录。

---
# 思考与练习
8-1 某无状态会话构件客户端程序连续两次调用该 `EJBHome` 接口中的 `create` 方法，容器会创建几个该EJB的实例？为什么？
8-2 某有状态会话构件客户端程序连续两次调用该 `EJBHome` 接口中的 `create` 方法，容器会创建几个该EJB的实例？为什么？
8-3 在8.3节的例子中，如果客户端程序在退出前没有调用 `Remote` 接口中的 `remove` 操作，那么下次运行客户端程序容器是否还会创建新的对象？为什么？
8-4 `EJB 2.x` 的实体构件与 `EJB 1.x` 的实体构件有哪些主要区别？
8-5 实现一个提供"查询股票信息"功能的无状态会话组件以及相应的客户端程序。
8-6 实现一个提供"计数器"功能的有状态会话构件，提供"计数器清 0"、"增加计数值"、"减少计数值"、"获取当前计数值"等操作。实现一个简单的客户端程序测试对该EJB进行测试。


*[组装与部署工具]:   新版本的Java EE参考不再提供单独的组装部署工具，而是将组装、布署的工作集成到了相应的开发工具，如NetBean中。

 

