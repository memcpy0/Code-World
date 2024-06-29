@[toc]

这里通过一个基于 `Borland VisiBroker (4.5.1)` 的简单例子，说明基于 `CORBA` 的基本开发过程，可从网上免费获得软件试用版。

---
# 3.1 设计相关的若干问题
首先，简单看一下**在系统设计阶段应注意的问题**。由于篇幅的限制，此外并不想去考察「软件系统分析设计阶段的普遍问题」，而是重点讨论几个与「基于 `CORBA` 的分布式系统」相关的问题。
## 1. 运行平台
设计者必须在设计初期决定，**待开发的分布式系统要运行在哪类硬件和软件平台之上**。由于不同平台（计算机硬件和操作系统）之间的差异，为一个平台开发的软件系统，通常不能直接运行在另一个平台之上（应注意：==尽管 `CORBA` 提供了异类环境中良好的可互操作性，但这与系统的可移植性是截然不同的两个问题==）。一般来说，**设计者总是在「软件系统的性能与通用性的矛盾」之间，作一个折衷的选择**：要使所开发的软件系统具备良好的通用性，能够方便的在不同平台上迁移，就需要尽可能的避免使用特定平台（比如某个厂商的平台或某个操作系统）相关的机制，而较好的性能在很多情况下，都要借助于特定平台的特性才能获得。

所以，设计者要根据自己特定项目或系统的特点与需求，作出一个折衷的选择。
## 2. 调用方式
在 `CORBA` 中，分布式对象提供的服务的**调用方式可有三种**：
- **同步方式**：调用时，调用者会阻塞，直到被调用的服务完成并返回。
- **异步方式**：**调用者发起调用后不会阻塞，等待服务完成期间，可以执行其它操作**。调用者通过轮询方式、或服务者发送的事件检测<u>调用完成</u>，服务完成后调用者检查并处理结果。异步方式通常依靠异步消息来实现。
- **单向方式**：调用者只是发出调用请求，并不关心调用什么时候完成（以及完成的结果）。

**不同的调用方式适合不同的应用场合**。
- 客户程序的请求所引起的服务程序操作，只需要很短时间即可完成，例如：查询某一帐户的当前余额，这时应选用同步通信方式。
- 如果客户程序请求服务程序格式化并打印几个大型文档，服务程序需要较长时间才可完成该请求，这时应选用异步通信方式，从而在服务程序格式化并打印文档期间，客户程序可以做一些其他事情。
- 如果客户程序无需获知请求已完成的确认信息，例如：向系统日志模块登记系统执行了某一操作，则应选用单向通信方式。
### 3. 资源优化
==在分布式环境下，跨网络的通信开销是相当可观的，通常是影响系统整体效率的瓶颈==。而在 `Stub/Skeleton` 机制的支撑下，开发者已经不需要自己编程处理底层通信，分布式对象在开发时，并没有表现出很强的分布特性，这更容易使设计者忽略跨网络的通信对系统的影响。

在一个集中式软件系统中，程序员可在同一进程中随意地连续多次调用一个例程，因为这些调用的系统资源开销微不足道。但==在分布式环境下，同样的调用如果发生在跨网络的进程之间，这些调用占用的系统资源是相当可观的==。因此在设计阶段，特别是在接口详细设计阶段，应考虑**尽量提高网络通信资源的利用率，避免频繁的跨网络（尤其是广域网）通信，而不应只从功能实现的方面去考虑**。
 
### 4. 其他决策问题
分布式系统通常要比集中式软件考虑更多的安全性、可靠性、事务处理、并发控制等问题。另外，还需要考虑更多的错误处理，例如客户程序发出请求、但服务程序未就绪，甚至找不到服务程序、或无权限访问服务程序时，应如何处理这种情况。

---
# 3.2 `CORBA` 应用程序开发过程
虽然OMG为 `CORBA` 制订了统一的规范，但规范中也赋予了软件供应商实现 `ORB` 产品时、自由选择各自不同的实现途径的权利，例如：`ORB` 可以是一个独立运行的守护进程，也可以嵌入到客户程序和对象实现中，有些 `ORB` 产品则选择了几种实现方式的组合。所以，不同供应商提供的 `ORB` 产品，在具体使用方法上可能存在较大差异，这里的 `CORBA` 部分以Borland公司的 `VisiBroker for Java 4.5.1` 为例，介绍一个 `CORBA` 应用程序的具体开发步骤，使用其他 `ORB` 产品时可参照类似做法。

尽管使用不同 `ORB` 产品的具体操作差异较大，但程序员开发一个 `CORBA` 应用程序通常会遵循一定的框架，即==首先通过面向对象分析与设计过程，认定应用程序所需的对象，包括对象的属性、行为与约束等特性，然后遵循本节所述的几个开发步骤，完成应用程序的开发、部署与运行==，如图3-1所示，图中的箭头表示了任务之间的先后次序。
![图3-1 CORBA 应用的典型开发过程](https://img-blog.csdnimg.cn/74f894d6d48a4a15aba5cbbe32cb3704.png)

下面简要解释开发的每一步骤的主要工作。
## 1. 编写对象接口
==**对象接口**是分布式对象对外提供的服务的规格说明==。`CORBA` 中分布式对象的接口定义，要包括以下内容：
- **提供或使用的服务的名字**，即客户端可以调用的方法名；
- **每个方法的参数列表与返回值**；
- **方法可能会引发的异常**；
 - **可选的上下文环境**，即调用相关的上下文环境，上下文环境起和参数类似的作用，包含调用相关的若干信息，只不过它并不写死在参数列表中，有更强的灵活性。

可以看出，`CORBA` 中对象接口中定义的核心内容，和 `RMI` 例子中用 `Java interface` 定义的接口类似，但它们有一点明显区别，那就是 **`CORBA` 中对象接口是由 `OMG IDL` 定义的，而这种 `IDL` 是独立于程序设计语言的**。正是有了这种中性的接口约定，才使得分布式对象和客户端的跨语言得以实现。[第四章]()详细介绍了 `IDL` 的语法与语义。
## 2. 编译 `IDL` 文件
`IDL` 是一种独立于具体程序设计语言的说明性语言，`IDL` 编译器的作用是**将 `IDL` 映射到具体程序设计语言，产生客户程序使用的桩代码、以及编写对象实现所需的框架代码**。由OMG制订的语言映射规范，允许将 `IDL` 语言映射到Java、C++、Ada、C、COBOL等多种程序设计语言，这通常是由软件供应商提供的不同编译器分别完成的。

一般厂商实现 `CORBA` 平台时，都会提供专门的 `IDL` 编译器来完成 `IDL` 接口的编译工作。厂商实现 `IDL` 编译器时，应参照OMG制订的语言的规范，编程人员只要选择使用合适的编译器就可以了，`IDL` 编译器的工作原理如图3-2所示。
![图 3-2 IDL 编译器的工作原理](https://img-blog.csdnimg.cn/87408be39fb04b98acb19fa803a8659a.png)
`VisiBroker for Java` 提供的 `idl2java` 编译器，将 `IDL` 映射到Java语言，生成Java语言的客户端桩代码、以及服务端框架代码，桩和框架可分别看做是「服务对象在客户端和服务端的代理」。**`IDL` 文件严格地定义了客户程序与对象实现之间的接口，因而客户端的桩可以与服务端的框架协调工作**，即使这两端是用不同的程序设计语言编译，或运行在不同供应商的 `ORB` 产品之上。
## 3. 编写客户程序
在 `CORBA` 中，客户程序的流程较为简单，如图3-3所示，**首先初始化 `ORB` ，然后绑定到要使用的服务对象，然后调用服务对象提供的服务**。和 `Java RMI` 相比，`CORBA` 客户端程序多了一步初始化 `ORB` 的操作。
![图3-3 客户程序的工作流程](https://img-blog.csdnimg.cn/7522ab922af14ca093c99195e8c77f24.png)

在 `CORBA` 中，无论是客户程序还是服务程序，都必须在利用 `ORB` 进行通信之前初始化 `ORB` 。初始化 `ORB` 的作用有两个，一个**让 `ORB` 了解有新的成员加入，以便后继为其提供服务**；另一个作用就是**获取 `ORB` 伪对象的引用**，以备将来调用 `ORB` 内核提供的操作。
> 所谓伪对象，专指在 `CORBA` 基础设施中的一个对象，比如 `ORB` 本身可以看作一个伪对象，伪对象的这个“伪”字主要是针对「在程序中远程访问的 `CORBA` 分布式对象」而言的，因为**伪对象是本地的**，我们通过伪对象调用 `CORBA` 基础设施提供的操作。

`ORB` 内核提供了一些「不依赖于任何对象适配器的操作」，这些操作可由客户程序或对象实现调用，包括获取初始引用的操作、动态调用相关的操作、生成类型码的操作、线程和策略相关的操作等，初始化 `ORB` 即是其中的一个操作。**程序员在程序中通过 `ORB` 伪对象来调用这些操作**，也就是说，在程序中通过使用 `<orb 伪对象名>.<方法名>` 的方式来调用ORB 内核提供的操作。
## 4. 编写对象实现和服务程序代码
在 `CORBA` 服务端，开发的主要工作包括**编写对象实现与服务程序**。`IDL` 文件只定义了**服务对象的规格说明**，程序员必须另外编写**服务对象的具体实现**。

`CORBA` 规定，所有对象接口定义必须统一用 `IDL` 书写，但对象实现则有很多选择的余地，例如可使用Java、C++、C、Smalltalk等程序设计语言，并且**选择这些语言与客户程序所选用的语言无关**，只要 `ORB` 产品供应商支持 `IDL` 到这些语言的映射即可。

选用任何一门程序设计语言的程序员，应该熟悉 `IDL` 到该语言的映射规则，因为通常情况下，**`IDL` 编译器除了生成 `Stub` 与 `Skeleton` 之外，还会生成一些开发时需要使用辅助代码**。例如 `VisiBroker for Java` 的 `IDL` 编译器，将自动生成一些对象适配器的Java类和各种辅助性的Java类，编写对象实现的代码时，必须继承其中的一些类、或使用某些类提供的方法。

与 `Java RMI` 例子中类似，**在 `CORBA` 服务端，除了编写对象实现代码后，还要编写一个服务程序来将分布式对象准备好**。==服务程序利用可移植对象适配器 `POA` 激活伺服对象供客户程序使用。服务程序通常是一个循环执行的进程，不断监听客户程序请求并为之服务==。
## 5. 创建并部署应用程序
编写完代码以后，就可以编译生成目标应用程序了。==创建客户程序时，应将「程序员编写的客户程序代码」与「`IDL` 编译器自动生成的客户程序桩代码」一起编译；创建服务程序时，应将「程序员编写的对象实现代码」与 「`IDL` 编译器自动生成的服务程序框架代码」一起编译==。一些 `ORB` 产品提供了专门的编译器以简化这一过程，例如 `VisiBroker for Java` 提供的编译器 `vbjc` 会自动调用JDK中的Java编译器 `javac` ，指示 `javac` 在编译客户程序的同时、编译相关的客户程序桩文件，在编译服务程序的、同时编译相关的服务程序框架文件。

程序员创建的客户程序和服务应用程序，在通过测试并准备投入运行后，进入应用程序的部署 `deployment` 阶段。**分布式系统的布署工作通常远比集中式软件的安装复杂**，在该阶段由系统管理员规划，如何在终端用户的桌面系统安装客户程序，或在服务器一类的机器上安装服务程序。由于其复杂性，布署工作经常由单独的角色来承担。
## 6. 运行应用程序
**运行 `CORBA` 应用程序时，必须首先启动服务程序，然后才可运行客户程序**。其他步骤可能与具体 `ORB` 产品有关，例如 `VisiBroker for Java` 的 `ORB` 内核是一个名为 `osagent` 的独立运行进程（又称智能代理 `smart agent` ），可以在启动服务程序之后才启动 `osagent` ，但必须在运行客户程序之前让 `osagent` 启动完毕。

应注意，上述过程是一个典型的 `CORBA` 应用程序开发过程，具体实施时，各个步骤会由于不同项目的各自特点而有所区别。例如，利用 `CORBA` 集成企业原有系统时，就是一种先有对象实现，而后有对象接口规格说明的过程。

---
# 3.3 `CORBA` 开发实例
本节以一个**银行帐户管理**的简单例子，演示 `CORBA` 应用程序的典型开发过程，使对「**`CORBA` 应用程序的开发、部署与运行**」有一个初步的感性认识。

这是一个简单的银行帐户管理程序，服务端管理大量银行顾客的账户，向远程客户端提供基本的开户、存款、取款、查询余额的功能。
## 3.3.1 认定分布式对象
按照面向对象的设计理念，我们很容易认定出，系统中应包含图3-4所示的两类对象：
- `Account` 是一个银行帐户的实体模型，它有一个属性 `balance` 表示当前的余额，另有三个行为分别为存款 `deposit` 、取款 `withdraw` 和查询余额 `getBalance` ，每一银行帐户在生存期的任何时刻，都满足帐户余额不小于 $0$ 这一约束。
- 由于例子程序不是仅仅管理某一位顾客的帐户，而是涉及到大量的帐户需要处理，所以还建立了“帐户管理员”这一实体模型，它负责对每一个帐户的开设、撤销和访问等，在实现世界中该实体对应着银行中的储蓄员。帐户管理员 `AccountManager` 有一个属性 `accountList` ，记录当前已开设的所有帐户，并且有一个行为表示根据帐户标识查找某一帐户，如果该标识的帐户不存在，则创建一个新帐户，我们将该行为命名为 `open` 。这其实只是帐户管理员的最简化模型，在一个实际应用系统中会赋予帐户管理员这一实体更多的职能。
![图3-4 系统对象](https://img-blog.csdnimg.cn/52ceebb91f994188a31ab93d882d3c48.png)

应注意，上述两类对象都是分布式对象，因为对象上的 `open, deposit, withdraw` 与 `getBalance` 操作，都是要被客户端远程调用的。

 
## 3.3.2 编写分布式对象的接口
按照图3-1所示的开发过程，**对于每一个分布式对象，首先要做得就是定义其接口**。我们利用OMG的接口定义语言 `IDL` ，编写对象 `Account` 和 `AccountManager` 的规格说明，规格说明存放在一个文本文件中，称之为 `IDL` 文件。从程序3-1所示 `IDL` 文件中的对象接口定义可看出，**`IDL` 与Java中的 `interface` 具有类似的语法**。第四章将详细介绍如何使用 IDL 定义模块、接口、数据结构等。

```java
// 程序 3-1 Bank.idl 文件中定义的对象接口
// 银行帐户管理系统的对象接口定义
module Bank {
	// 帐户
	interface Account {
		// 存款
		void deposit(in float amount);
		// 取款
		boolean withdraw(in float amount);
		// 查询余额
		float getBalance();
	};
	
	// 帐户管理员
	interface AccountManager {
		// 查询指定名字的帐户，查无则新开帐户
		Account open(in string name);
	};
};
```

## 3.3.3 编译 `IDL` 文件生成桩与框架
完成对象接口的规格说明后，下一步工作是利用 `VisiBroker for Java` 提供的 `idl2java` 编译器，根据 `IDL` 文件生成客户程序的桩代码、以及对象实现的框架代码。客户程序用这些 `Java` 桩代码调用所有的远程方法，框架代码则与程序员编写的代码一起创建对象实现。

上述 `Bank.idl` 文件无需作特殊处理，它可用以下命令编译：
```bash
prompt> idl2java Bank.idl
```

由于Java语言规定，每一个文件只能定义一个公有的接口或类，因此 `IDL` 编译器的输出会生成多个 `.java` 文件。这些文件存储在一个新建的子目录 `Bank` 中，`Bank` 是 `IDL` 文件中指定的模块名字，也是**根据该模块中所有 `IDL` 接口、自动生成的所有Java类所属的程序包**。

`IDL` 编译器为 `IDL` 文件中定义的每一个接口自动生成 $7$ 个 `.java` 文件，故上述 `IDL` 文件的编译结果会生成 $14$ 个 `.java` 文件，下面以 `Account` 接口对应的 $7$ 个文件为例，介绍 `IDL` 编译器生成的代码，`IDL` 编译器为接口 `Account` 生成的文件包括：
```bash
AccountOperations.java
Account.java
_AccountStub.java
AccountPOA.java
AccountPOATie.java
AccountHelper.java
AccountHolder.java
```
在这些文件中，`Account.java` 和 `AccountOperations.java` 定义了 `IDL` 接口 `Account` 的完整基调（`signature` ，包括操作的名字和参数表，可用于唯一地表示一个操作的类型）。`AccountOperations.java` 是 `IDL` 文件中由 `Account` 接口定义的所有方法和常量的基调声明，如程序3-2所示。**由 `IDL` 编译器自动生成的对象实现框架代码，将实现该接口**（参见程序3-5），该接口还与 `AccountPOATie` 类一起提供**纽带机制**（参见程序3-6）。

```java
// 程序 3-2 IDL 编译器生成的 AccountOperations.java 文件内容
package Bank;
public interface AccountOperations {
	public void deposit(float amount);
	public boolean withdraw(float amount);
	public float getBalance();
}
```
**`IDL` 编译器为每一个 `IDL` 接口，生成一个最基本的 `Java` 接口**，例如，`Account.java` 包含了 `Account` 接口的声明，如程序3-3所示。**该接口继承了 `AccountOperations` 接口**。在程序员编写的客户程序中，会使用该接口来调用远程账户对象上的操作。
```java
// 程序 3-3 IDL 编译器生成的 Account.java 文件内容
package Bank;
public interface Account 
	extends com.inprise.vbroker.CORBA.Object,
	Bank.AccountOperations, org.omg.CORBA.portable.IDLEntity {}
```
**`IDL` 编译器还为每一个接口创建一个桩类**，`_AccountStub.java` 是 `Account` 对象在客户端的桩代码，它实现了 `Account` 接口，如程序3-4所示。应注意，此类中 `deposit, withdraw, 
getBanlance` 等操作**并没有真正实现账户对象的业务逻辑，只是替客户端完成（对）服务端真正业务逻辑实现的调用**。该类负责客户端调用账户对象时的底层通信工作，从客户程序的代码上（程序 3-9）看，客户端通过 `Account` 接口来调用账户对象上的操作，但实际上客户端调用的是该类上的操作，由该类替客户端完成远程调用。
```bash
// 程序 3-4 IDL 编译器生成的_AccountStub.java 文件内容
package Bank;
public class _AccountStub 
	extends com.inprise.vbroker.CORBA.portable.ObjectImpl implements Account {
	final public static java.lang.Class _opsClass = Bank.AccountOperations.class;
	private static java.lang.String[] __ids = {"IDL:Bank/Account:1.0"};
	
	public java.lang.String[] _ids() {
		return __ids;
	}	
	
	public void deposit(float amount) {
		while (true) {
			if (!_is_local()) {
				org.omg.CORBA.portable.OutputStream _output = null;
				org.omg.CORBA.portable.InputStream _input = null;
				try {
					_output = this._request("deposit", true);
					_output.write_float((float) amount);
					_input = this._invoke(_output);
				} catch(org.omg.CORBA.portable.ApplicationException _exception) {
					final org.omg.CORBA.portable.InputStream in =
					_exception.getInputStream();
					java.lang.String _exception_id = _exception.getId();
					throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _exception_id);
				} catch(org.omg.CORBA.portable.RemarshalException _exception) {
					continue;
				} finally {
					this._releaseReply(_input);
				}
			} else {
				final org.omg.CORBA.portable.ServantObject _so =
				_servant_preinvoke("deposit", _opsClass);
				if (_so == null) {
					continue;
				}
				final Bank.AccountOperations _self = (Bank.AccountOperations)_so.servant;
				try {
					_self.deposit(amount);
				} finally {
					_servant_postinvoke(_so);
				}
			}
			break;
		}
	}
	
	public boolean withdraw(float amount) {
		while (true) {
			if (!_is_local()) {
				org.omg.CORBA.portable.OutputStream _output = null;
				org.omg.CORBA.portable.InputStream _input = null;
				boolean _result;
				try {
					_output = this._request("withdraw", true);
					_output.write_float((float)amount);
					_input = this._invoke(_output);
					_result = _input.read_boolean();
					return _result;
				} catch(org.omg.CORBA.portable.ApplicationException _exception) {
					final org.omg.CORBA.portable.InputStream in = _exception.getInputStream();
					java.lang.String _exception_id = _exception.getId();
					throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _exception_id);
				} catch(org.omg.CORBA.portable.RemarshalException _exception) {
					continue;
				} finally {
					this._releaseReply(_input);
				}
			} else {
				final org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("withdraw", _opsClass);
				if (_so == null) {
					continue;
				}
				final Bank.AccountOperations _self = (Bank.AccountOperations)_so.servant;
				try {
					return _self.withdraw(amount);
				} finally {
					_servant_postinvoke(_so);
				}
			}
		}
	}
	
	public float getBalance() {
		while (true) {
			if (!_is_local()) {
				org.omg.CORBA.portable.OutputStream _output = null;
				org.omg.CORBA.portable.InputStream _input = null;
				float _result;
				try {
					_output = this._request("getBalance", true);
					_input = this._invoke(_output);
					_result = _input.read_float();
					return _result;
				} catch(org.omg.CORBA.portable.ApplicationException _exception) {
					final org.omg.CORBA.portable.InputStream in = _exception.getInputStream();
					java.lang.String _exception_id = _exception.getId();
					throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _exception_id);
				} catch(org.omg.CORBA.portable.RemarshalException _exception) {
					continue;
				} finally {
					this._releaseReply(_input);
				}
			} else {
				final org.omg.CORBA.portable.ServantObject _so = _servant_preinvoke("getBalance", _opsClass);
				if (_so == null) {
					continue;
				}
				final Bank.AccountOperations _self = (Bank.AccountOperations)_so.servant;
				try {
					return _self.getBalance();
				} finally {
					_servant_postinvoke(_so);
				}
			}
		}
	}
}
```
**`AccountPOA.java` 是 `Account` 对象的服务端框架代码**，如程序3-5所示。该类一方面负责解包 `in` 类型的参数，并将参数传递给对象实现，另一方面，负责打包返回值与所有 `out` 类型的参数。所谓**打包** `marshal` 是指将「特定程序设计语言描述的数据类型」转换为「 `CORBA` 的 `IIOP` 流格式」，经过网络传输到目标地点后，再通过解包 `unmarshal` ，从「 `IIOP` 流格式」转换为「依赖于具体程序设计语言的数据结构」。

**编写对象实现的最简单途径就是继承这些 `POA` 类，即把它们作为对象实现的基类**（参见程序3-12）。
```bash
// 程序 3-5 IDL 编译器生成的 AccountPOA.java 文件内容
package Bank;
public abstract class AccountPOA extends org.omg.PortableServer.Servant implements org.omg.CORBA.portable.InvokeHandler, Bank.AccountOperations {
	public Bank.Account _this() {
		return Bank.AccountHelper.narrow(super._this_object());
	}
	
	public Bank.Account _this(org.omg.CORBA.ORB orb) {
		return Bank.AccountHelper.narrow(super._this_object(orb));
	}
	
	public java.lang.String[] _all_interfaces (
		final org.omg.PortableServer.POA poa, final byte[] objectId) {
		return __ids;
	}
	
	private static java.lang.String[] __ids = {"IDL:Bank/Account:1.0"};
	private static java.util.Dictionary _methods = new java.util.Hashtable();
	static {
		_methods.put("deposit",
		new com.inprise.vbroker.CORBA.portable.MethodPointer(0, 0));
		_methods.put("withdraw",
		new com.inprise.vbroker.CORBA.portable.MethodPointer(0, 1));
		_methods.put("getBalance",
		new com.inprise.vbroker.CORBA.portable.MethodPointer(0, 2));
	}
	
	public org.omg.CORBA.portable.OutputStream _invoke(java.lang.String opName, 
		org.omg.CORBA.portable.InputStream _input, org.omg.CORBA.portable.ResponseHandler handler) {
		com.inprise.vbroker.CORBA.portable.MethodPointer method =
		(com.inprise.vbroker.CORBA.portable.MethodPointer) _methods.get(opName);
		if (method == null) {
			throw new org.omg.CORBA.BAD_OPERATION();
		}
		switch (method.interface_id) {
			case 0: {
				return Bank.AccountPOA._invoke(this, method.method_id, _input, handler);
			}
		}
		throw new org.omg.CORBA.BAD_OPERATION();
	}
	
	public static org.omg.CORBA.portable.OutputStream _invoke(
		Bank.AccountOperations _self, int _method_id,
		org.omg.CORBA.portable.InputStream _input,
		org.omg.CORBA.portable.ResponseHandler _handler) {
		org.omg.CORBA.portable.OutputStream _output = null; 
		{
			switch (_method_id) {
				case 0: {
					float amount;
					amount = _input.read_float(); 
					_self.deposit(amount);
					_output = _handler.createReply();
					return _output;
				}
				case 1: {
					float amount;
					amount = _input.read_float();
					boolean _result = _self.withdraw(amount);
					_output = _handler.createReply();
					_output.write_boolean((boolean)_result);
					return _output;
				}
				case 2: {
					float _result = _self.getBalance();
					_output = _handler.createReply();
					_output.write_float((float)_result);
					return _output;
				}
			}
			throw new org.omg.CORBA.BAD_OPERATION();
		}
	}
}
```
**`AccountPOATie.java` 用于采用纽带机制、实现服务端的 `Account` 对象**，其内容如程序3-6所示。==由于Java语言中类只支持单重继承，因此有时，对象实现可能由于需要继承其它类、而导致其无法继承 `POA` 类，这里需要利用该纽带机制类来共同支持客户端的远程调用==。[第五章]()将详细介绍纽带机制。
```java
// 程序 3-6 IDL 编译器生成的 AccountPOATie.java 文件内容
package Bank;
public class AccountPOATie extends AccountPOA {
	private Bank.AccountOperations _delegate;
	private org.omg.PortableServer.POA _poa;
	
	public AccountPOATie(final Bank.AccountOperations _delegate) {
		this._delegate = _delegate;
	}

	public AccountPOATie(final Bank.AccountOperations _delegate,
		final org.omg.PortableServer.POA _poa) {
		this._delegate = _delegate;
		this._poa = _poa;
	}
	
	public Bank.AccountOperations _delegate() {
		return this._delegate;
	}
	
	public void _delegate(final Bank.AccountOperations delegate) {
		this._delegate = delegate;
	}
	
	public org.omg.PortableServer.POA _default_POA() {
		if (_poa != null) {
			return _poa;
		} else {
			return super._default_POA();
		}
	}
	
	public void deposit(float amount) {
		this._delegate.deposit(amount);
	}
	
	public boolean withdraw(float amount) {
		return this._delegate.withdraw(amount);
	}
	
	public float getBalance() {
		return this._delegate.getBalance();
	}
}
```
**`IDL` 编译器还为每一个用户自定义类型生成一个辅助工具类**。`AccountHelper.java` 定义了 `AccountHelper` 类，该类为 `Account` 接口定义了许多实用功能、以及支持功能的静态方法（又称类方法），如程序3-7所示。

这些工具性的方法，为用户自定义类型的对象提供了以下主要操作：从 `Any` 对象提取或向 `Any` 对象插入对象（`extract` 和 `insert` 方法）；从输入/输出流读写对象（`read` 和 `write` 方法）；获取对象的库标识和类型码（`id` 和 `type` 方法）；绑定对象与类型转换操作（ `bind` 和 `narrow` 方法）等等。

由 `IDL` 编译器自动生成的 `Java` 类，以及程序员编写的客户程序与对象实现代码，都直接使用了 `AccountHelper` 类提供的类方法，例如：几种重载形式的 `bind` 方法，用于将客户程序绑定到「指定对象标识与主机的 `Account` 类型对象」，`narrow` 方法则将通用类型对象窄化为 `Account` 类型。
```java
// 程序 3-7 IDL 编译器生成的 AccountHelper.java 文件内容
package Bank;
public final class AccountHelper {	
	public static Bank.Account narrow(final org.omg.CORBA.Object obj) {
		return narrow(obj, false);
	}

	public static Bank.Account unchecked_narrow(org.omg.CORBA.Object obj) {
		return narrow(obj, true);
	}

	private static Bank.Account narrow(final org.omg.CORBA.Object obj, final boolean is_a) {
		if (obj == null) {
			return null;
		}
		if (obj instanceof Bank.Account) {
			return (Bank.Account)obj;
		}
		if (is_a || obj._is_a(id())) {
			final org.omg.CORBA.portable.ObjectImpl _obj = (org.omg.CORBA.portable.ObjectImpl)obj;
			Bank._AccountStub result = new Bank._AccountStub();
			final org.omg.CORBA.portable.Delegate _delegate = _obj._get_delegate();
			result._set_delegate(_delegate);
			return result;
		}
		throw new org.omg.CORBA.BAD_PARAM();
	}
	
	public static Bank.Account bind(org.omg.CORBA.ORB orb) {
		return bind(orb, null, null, null);
	}
	
	public static Bank.Account bind(org.omg.CORBA.ORB orb, java.lang.String name) {
		return bind(orb, name, null, null);
	}
	
	public static Bank.Account bind(org.omg.CORBA.ORB orb, java.lang.String name,
		java.lang.String host, com.inprise.vbroker.CORBA.BindOptions _options) {
		if (!(orb instanceof com.inprise.vbroker.CORBA.ORB)) {
			throw new org.omg.CORBA.BAD_PARAM();
		}
		return narrow(((com.inprise.vbroker.CORBA.ORB) orb).bind(id(), name, host, _options), true);
	}
	
	public static Bank.Account bind(org.omg.CORBA.ORB orb, java.lang.String fullPoaName, byte[] oid) {
		return bind(orb, fullPoaName, oid, null, null);
	}

	public static Bank.Account bind(org.omg.CORBA.ORB orb, java.lang.String fullPoaName, byte[] oid,
		java.lang.String host, com.inprise.vbroker.CORBA.BindOptions _options) {
		if (!(orb instanceof com.inprise.vbroker.CORBA.ORB)) {
			throw new org.omg.CORBA.BAD_PARAM();
		}
		return narrow(((com.inprise.vbroker.CORBA.ORB) orb).bind(fullPoaName, oid, 
			host, _options), true);
	}
	
	public java.lang.Object read_Object(final org.omg.CORBA.portable.InputStream istream) {
		return read(istream);
	}
	
	public void write_Object(final org.omg.CORBA.portable.OutputStream ostream,
		final java.lang.Object obj) {
		if (!(obj instanceof Bank.Account)) {
			throw new org.omg.CORBA.BAD_PARAM();
		}
		write(ostream, (Bank.Account)obj);
	}

	public java.lang.String get_id() {
		return id();	
	}
	
	public org.omg.CORBA.TypeCode get_type() {
		return type();
	}

	private static org.omg.CORBA.TypeCode _type;
	private static boolean _initializing;
	private static org.omg.CORBA.ORB _orb() {
		return org.omg.CORBA.ORB.init();	
	}
	
	public static Bank.Account read(final org.omg.CORBA.portable.InputStream _input) {
		return narrow(_input.read_Object(_AccountStub.class), true);
	}

	public static void write(final org.omg.CORBA.portable.OutputStream _output,
		final Bank.Account _vis_value) {
		if (!(_output instanceof org.omg.CORBA_2_3.portable.OutputStream)) {
			throw new org.omg.CORBA.BAD_PARAM();
		}
		if (_vis_value != null && !(_vis_value instanceof org.omg.CORBA.portable.ObjectImpl))	{
			throw new org.omg.CORBA.BAD_PARAM();
		}
		_output.write_Object((org.omg.CORBA.Object)_vis_value);
	}

	public static void insert(final org.omg.CORBA.Any any, final Bank.Account _vis_value) {
		any.insert_Object((org.omg.CORBA.Object)_vis_value,
		Bank.AccountHelper.type());
	}

	public static Bank.Account extract(final org.omg.CORBA.Any any) {
		Bank.Account _vis_value;
		final org.omg.CORBA.Object _obj = any.extract_Object();
		_vis_value = Bank.AccountHelper.narrow(_obj);
		return _vis_value;
	}

	public static org.omg.CORBA.TypeCode type() {
		if (_type == null) {
			synchronized (org.omg.CORBA.TypeCode.class) {
				_type = _orb().create_interface_tc(id(), "Account");
			}
		}
		return _type;
	}

	public static java.lang.String id() {
		return "IDL:Bank/Account:1.0";
	}
}
```
**`AccountHolder.java` 声明的 `AccountHolder` 类，为传递 `Account` 对象提供支持**，其内容如程序3-8所示。在下一章将了解到，**`IDL` 有三种参数传递方式：`in, out, inout`** ，==调用方法时 `in` 类型的参数以及返回结果，与Java的参数传递方式与结果返回方式完全相同，但 `out` 和 `inout` 两种类型的参数允许参数具有返回结果的能力，无法直接映射到Java语言的参数传递机制==，这时 `AccountHolder` 类为传递 `out` 和 `inout` 参数提供了一个托架 `holder` 。

```java
// 程序 3-8 IDL 编译器生成的 AccountHolder.java 文件内容
package Bank;
public final class AccountHolder implements org.omg.CORBA.portable.Streamable {
	public Bank.Account value;
	public AccountHolder() {}
	public AccountHolder(final Bank.Account _vis_value) {
		this.value = _vis_value;
	}
	public void _read(final org.omg.CORBA.portable.InputStream input) {
		value = Bank.AccountHelper.read(input);
	}
	public void _write(final org.omg.CORBA.portable.OutputStream output) {
		Bank.AccountHelper.write(output, value);
	}
	public org.omg.CORBA.TypeCode _type() {
		return Bank.AccountHelper.type();
	}
}
```

## 3.3.4 编写对象实现
**对象实现代码所在的类名字可由程序员自由掌握，只要不与 `IDL` 编译器自动产生的Java类产生名字冲突即可**。例如，我们为例子程序中的两个对象实现分别命名为 `AccountImpl` 和 `AccountManagerImpl` 。客户程序也无须了解对象实现是由哪一个Java类完成的。

**`CORBA` 应用程序的对象实现最常用、最简单的实现方式是使用继承**，即「由程序员编写的对象实现 `AccountImpl` 类」直接继承「由 `IDL` 编译器生成的 `AccountPOA` 类」。但是，Java语言仅支持类的单继承，如果对象实现 `AccountImpl` 继承了 `AccountPOA` 类就无法再继承其他类，**当对象实现需要利用继承机制达到其他目的时，就必须改用上面提到的 `CORBA` 对象实现的另一种实现方式——纽带机制 `tie mechanism`** 。

例程采用简单的继承方式编写对象实现。帐户的对象实现如程序3-9所示，`AccountImpl` 类继承抽象类`AccountPOA` ，并实现了 `AccountPOA` 从 `AccountOperations` 接口遗留的 $3$ 个方法，成为一个**可创建对象实例的具体类**。
```java
// 程序 3-9 对象实现 AccountImpl.java
// 帐户的具体实现
public class AccountImpl extends Bank.AccountPOA {
	// 属性定义
	protected float balance;
	// 构造方法，按指定余额创建新的帐户
	public AccountImpl(float bal) {
		balance = bal;
	}
	// 往帐户中存款
	public void deposit(float amount) {
		balance += amount;
	}
	// 从帐户中取款，不足余额则返回 false
	public boolean withdraw(float amount) {
		if (balance < amount) return false;
		else {
			balance -= amount;
			return true;
		}
	}
	// 查询帐户余额
	public float getBalance() {
		return balance;
	}
}
```

帐户管理员的对象实现如程序3-10所示，`AccountManagerImpl` 类继承了抽象类 `AccountManagerPOA` ，并实现了 `AccountManagerPOA` 的所有遗留方法。由于在本例中账户管理员用来管理另一类分布式对象——账户，账户管理员的核心功能是：将新创建的账户分布式对象准备好（代码中 `open` 方法），因此其功能与后面的服务程序非常类似（服务程序的主要作用是：把账户管理员分布式对象准备好），可以先学习程序3-11中的服务端程序，然后再对比学习 `AccountManagerImpl` 类的实现代码。
```java
// 程序 3-10 对象实现 AccountManagerImpl.java
// 帐户管理员的具体实现
import java.util.*;
import org.omg.PortableServer.*;
public class AccountManagerImpl extends Bank.AccountManagerPOA {
	// 属性的定义
	protected Hashtable accountList; // 该帐户管理员所负责的帐户清单
	// 构造方法，管理员开始时管理的帐户清单为空
	public AccountManagerImpl() {
		accountList = new Hashtable();
	}
	
	// 查找指定名字的帐户，找不到则以该名字新开一个帐户
	public synchronized Bank.Account open(String name) {
		// 在帐户清单中查找指定名字的帐户
		Bank.Account account = (Bank.Account) accountList.get(name);
		// 如果不存在则新创建一个
		if (account == null) {
			// 随机虚构帐户的初始余额，金额在 0 至 1000 之间
			Random random = new Random();
			float balance = Math.abs(random.nextInt()) % 100000 / 100f;
			// 按指定余额创建帐户的伺服对象
			AccountImpl accountServant = new AccountImpl(balance);
			try {
				// 用缺省的 POA 激活伺服对象，这里缺省的 POA 就是根 POA
				org.omg.CORBA.Object obj = _default_POA().servant_to_reference(accountServant);
				// 将对象引用收窄为帐户类型
				account = Bank.AccountHelper.narrow(obj);
			} catch(Exception exc) {
				exc.printStackTrace();
			}
			// 将帐户保存到帐户清单中
			accountList.put(name, account);
			// 在服务端控制台打印已创建新帐户的提示信息
			System.out.println("新开帐户：" + name);
		}
		// 返回找到的帐户或新开设的帐户
		return account;
	}
}
```
## 3.3.5 编写服务程序
通常，程序员都会编写一个名为 `Server.java` 的服务程序，如程序3-11所示。典型的服务程序通常执行以下步骤：
1. **初始化对象请求代理** `ORB` 。
2. **用所需策略创建一个可移植对象适配器** `POA` 。程序中的 `BankPOA` 是我们为 `POA` 起的名字，客户程序必须使用相同的 `POA` 名字。
3. **创建一个提供服务的伺服对象**。伺服对象是服务端的本地对象，当伺服对象通过对象适配器注册到 `ORB` 后，就成为一个可供远程调用的 `CORBA` 对象。
4. **激活新创建的伺服对象**，即利用 `POA` 将伺服对象以一个字符串表示的标识注册到 `ORB` 上。
5. **激活 `POA` 管理器**。 
6. **等待客户程序发来请求**。

关于 `POA` 、`POA` 策略以及 `POA` 管理器，将在[第五章]()作详细介绍。
```java
// 程序 3-11 服务程序 Server.java
// 服务端的主程序
import org.omg.PortableServer.*;
public class Server {
	public static void main(String[] args) {
		try {
			// 初始化 ORB
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			// 取根 POA 的引用
			POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			// 创建持久 POA 的策略
			org.omg.CORBA.Policy[] policies = {
				rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
			};
			// 用新定义的策略创建 myPOA
			POA myPOA = rootPOA.create_POA("BankPOA", rootPOA.the_POAManager(), policies);
			// 创建伺服对象
			AccountManagerImpl managerServant = new AccountManagerImpl();
			// 在 myPOA 上用标识"BankManager"激活伺服对象
			myPOA.activate_object_with_id("BankManager".getBytes(), managerServant);
			// 激活 POA 管理器
			rootPOA.the_POAManager().activate();
			// 等待处理客户程序的请求
			System.out.println("帐户管理员 BankManager 已就绪 ...\n");
			orb.run();
		} catch(Exception exc) {
			exc.printStackTrace();
		}
	}
}
```

## 3.3.6 编写客户程序
客户程序 `Client.java` 演示，如何查询一个银行帐户的当前余额，然后进行存款和取款操作后，查看帐户的最新余额，如程序3-9所示，
```java
// 程序 3-9 客户程序 Client.java
// 客户端的主程序
public class Client {
	public static void main(String[] args) {
		// 初始化 ORB
		org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
		// 利用 POA 全称与对象标识"BankManager"查找帐户管理员
		Bank.AccountManager manager = Bank.AccountManagerHelper.bind(orb, "/BankPOA", "BankManager".getBytes());
		// 如果命令行参数未指帐户名字则使用缺省名字
		String name = args.length > 0 ? args[0] : "David Zeng";
		// 请求帐户管理员找出一个指定名字的帐户，无此帐户则新开一个
		Bank.Account account = manager.open(name);
		// 取该帐户的余额
		System.out.println(name + "的帐户余额为" + account.getBalance() + "元");
		// 往帐户中存款 200 元后，重新查看余额
		account.deposit(200);
		System.out.println("存款 200 元后，余额为" + account.getBalance() + "元");
		// 从帐户中取款 600 元后，重新查看余额
		if (account.withdraw(600)) {
			System.out.println("取款 600 元后，余额为" + account.getBalance() + "元");
		} else {
			System.out.println("余额不足 600 元，取款失败，余额保持不变");
		}
	}
}
```
如前所述，`IDL` 编译器根据 `Bank.idl` 文件自动生成的许多Java类存放在子目录 `Bank` 中，客户程序会直接利用其中的某些类或接口。例子程序中的客户程序主要执行了以下几个步骤：
1. **初始化 `ORB`** 。利用 `org.omg.CORBA.ORB` 提供的 `init` 方法，可获取 `ORB` 伪对象的一个对象引用。
2. **绑定到一个 `AccountManager` 对象**。从这一例子程序，看到了两种典型的对象引用获取方式。第一种是**在应用程序刚启动时，没有任何服务对象的引用，这时通常利用 `IDL` 编译器生成的 `Helper` 类中提供的类方法 `bind` ，将对象标识解析为对象引用**。`bind` 方法请求 `ORB` 查找对象实现，并与之建立连接，如果成功找到对象实现、并建立了连接，则创建一个 `AccountManagerPOA` 对象、作为对象实现的代理，然后将 `AccountManager` 对象的对象引用返回给客户程序。关于 `Helper` 类的 `bind` 方法可参阅程序3-7。
3. **通过调用 `AccountManager` 对象上的 `open` 方法获取一个 `Account` 对象**。这是获取对象引用更常见的方式，如果已存在一个远程 `CORBA` 对象的引用，则可以利用该对象引用调用某一方法、返回新的对象引用。例如，`AccountManager` 的 `open` 方法，可根据指定的顾客名字获取一个 `Account` 对象的引用。
4. **通过调用 `Account` 对象上的方法，完成指定帐户的存款、取款或查询余额操作**。只要客户程序获取了一个 `Account` 对象的引用，就可调用 `deposit, withdraw, getBalance` 方法对该帐户对象进行存款、取款和查询余额操作。注意：==客户端的这三个方法，实际上是由 `idl2java` 编译器生成的桩代码，它们收集一个请求所需要的所有数据，并将请求通过 `ORB` 发送到真正执行操作的对象实现==。
## 3.3.7 编译应用程序
综上所述，我们共开发了 $1$ 个 `IDL` 文件和 $4$ 个Java类，如表3-1所示。经过 `IDL` 编译器 `idl2java` 处理 `Bank.idl` 文件后，会创建一个名为 `Bank` 的子目录，其中含有 $14$ 个 `.java` 文件。
![表 3-1 例子程序的源文件清单](https://img-blog.csdnimg.cn/3c249e5adf794b7882c9bf7d7f6f9466.png)
下一步是调用Java语言的编译器，生成可运行的客户程序与服务程序的字节码，我们利用 `VisiBroker for Java` 提供的编译器 `vbjc` 完成这一工作：
```bash
prompt> vbjc Server.java
prompt> vbjc Client.java
```
`vbjc` 实际上封装了JDK提供的Java编译器，它调用 `javac` 编译源文件、并将与源文件有关的其他文件也编译成字节码，从而避免了我们手工逐个地编译。注意，**由 `IDL` 编译器生成的 `Holder` 类和 `POATie` 类，需要指定额外的参数才会生成**，因而上述编译过程完成后，`Bank` 子目录中只会新增 $10$ 个 `.class` 文件。
## 3.3.8 运行应用程序
完成上述编译过程后，就准备好了运行我们的第一个 `CORBA` 应用程序。
### 1. 启动智能代理
**运行 `CORBA` 应用程序之前，网络中必须至少有一台主机上启动了智能代理 `osagent`** 。这是 `VisiBroker` 特有的分布式位置服务 `location service` 守护进程，网络中多个智能代理可协作、以查找合适的对象实现。启动智能代理的基本命令如下：
```bash
prompt> osagent
```
如果运行平台是Windows NT，还可将智能代理作为 `NT` 服务来启动。这需要在安装 `VisiBroker for Java` 时将 `ORB` 服务注册为 `NT` 服务，注册服务后就可通过服务控制面板，将智能代理作为一个 `NT` 服务启动。
### 2. 启动服务程序
打开一个MS DOS方式的命令行窗口，使用以下命令启动服务程序：
```bash
prompt> start vbj Server
```
`vbj` 实际上封装了Java虚拟机 `java` ，它设置一些属性与类路径后，启动 `java` 运行目标程序。为接着利用当前命令行窗口启动客户程序，我们用 `start` 命令衍生一个新进程运行服务程序。
### 3. 运行客户程序
在命令行状态使用以下命令启动客户程序：
```bash
prompt> vbj Client
```
如果我们连续两次启动客户程序、且不带命令行参数，则访问的是同一个缺省顾客名字的帐户。第一次运行时将开设一个新的帐户，并且帐户的初始余额是随机设置的。每开设一个新帐户时，服务程序的控制台会给出一行提示信息，例如：
```bash
帐户管理员 BankManager 已就绪 ...
新开帐户：David Zeng
```
第二次运行客户程序时，由于该顾客的帐户已存在，所以查询的初始余额是上次运行时的最终余额。连接两次不带命令行参数、运行客户程序的提示信息，类似如下画面：

```bash
// 第一次运行时输出 ...
David Zeng 的帐户余额为 685.38 元。
存款 200 元后，余额变为 885.38 元。
取款 600 元后，余额变为 285.38 元。

// 第二次运行时输出 ...
David Zeng 的帐户余额为 285.38 元。
存款 200 元后，余额变为 485.38 元。
余额不足 600 元，取款失败，余额保持不变。
```
如果运行客户程序之前，智能代理 `osagent` 未就绪，或者服务程序进程未启动完毕，客户程序执行 `bind` 方法时，将引发一个 `CORBA` 异常，屏幕上会出现类似如下的出错信息：
![在这里插入图片描述](https://img-blog.csdnimg.cn/fe657ab2fc494c1a86aa42ac25e4ec76.png)

 
## 3.3.9 互操作尝试
### 1. 跨越机器与操作系统
与1.4.2中 `RMI` 例子类似，本例也可以将服务端和客户端分布到不同的机器或操作系统之上。如需在Linux等操作系统上编译运行客户端或服务端程序，需要在对应操作系统上安装 `VisiBroker for Java` ，由于本例客户端与服务端均没有使用特定操作系统相关的机制，因此都可以方便移植到Linux等操作系统。

==跨越机器运行时，需要修改客户端程序中绑定到账户管理员对象的代码，使其绑定到远端机器上的账户管理员对象==，如程序3-10所示：
```java
// 程序 3-10 绑定到远端机器对象的客户程序 Client.java
// 客户端的主程序
public class Client {
	public static void main(String[] args) {
		// 初始化 ORB
		org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
		// 利用 POA 全称与对象标识"BankManager"查找帐户管理员
		Bank.AccountManager manager = Bank.AccountManagerHelper.bind(orb, 
			"/BankPOA", "BankManager".getBytes(),
			" serverhost", new com.inprise.vbroker.CORBA.BindOptions());
		);
		// 如果命令行参数未指帐户名字则使用缺省名字
		String name = args.length > 0 ? args[0] : "David Zeng";
		// 请求帐户管理员找出一个指定名字的帐户，无此帐户则新开一个
		Bank.Account account = manager.open(name);
		// 取该帐户的余额
		//……
	}
}
```

### 2. 跨语言
由于 `CORBA` 提供了跨语言的基本支持，因此本例可以将服务端或客户端，用C++等Java语言之外的其它程序设计语言实现。下面以「在Linux平台上用C++语言实现本例的客户端程序」为例进行说明。

用C++语言实现本例的客户端程序，首先需要在客户端机器上安装 `VisiBroker for C++` ，可在安装 `VisiBroker for Java` 的机器上同时安装 `VisiBroker for C++` 。这两者对于开发的不同支持主要包括：
- `IDL` 编译器：与 `VisiBroker for Java` 提供的 `idl2java` 类似，`VisiBroker for C++`提供了一个 `IDL` 编译器 `idl2cpp` ，将开发人员用 `IDL` 定义的接口、映射到C++语言，生成 C++语言实现的客户端 `Stub` 、服务端 `Skeleton` 、以及相关的辅助文件。**由于C++语言支持类的多重继承与引用型参数，因此不需要 `xxxPOATie` 与 `xxxHolder` 等对应的类，生成的总的文件要比 `idl2java` 少**。
- 编译与运行：`VisiBroker for C++ (4.5.1 for Linux)` 并不提供类似 `vbjc` 与 `vbj` 的辅助编译与运行工具，而是直接利用Linux平台上的GCC编译器和 `make` 指令完成编译。而C++语言不是解释性语言，因此编译后得到的可执行程序能直接运行。利用 `make` 指令编译程序时，开发人员通常需要编写一个 `makefile` 来指示编译器具体的编译链接工作，可直接修改 `VisiBroker for C++` 自带例子程序使用的 `makefile` 来使用，或参考GCC相关手册自行编写。

程序3-11与程序3-12，给出了一个用C++实现的客户端程序（完成与程序3-9类似的功能）和该程序编译使用的 `makefile` 。

```cpp
// 程序 3-11 C++实现的客户程序 Client.C
// C++客户端的主程序
#include "Bank_c.hh"
// USE_STD_NS is a define setup by VisiBroker to use the std namespace
USE_STD_NS

int main(int argc, char* const* argv) {
	try {
		// 初始化 ORB
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
		// Get the manager Id
		PortableServer::ObjectId_var managerId = PortableServer::string_to_ObjectId("BankManager");
		// 绑定到远端账户管理员
		Bank::AccountManager_var manager = Bank::AccountManager::_bind("/BankPOA", managerId, "serverhost");
		//如果命令行参数未指帐户名字则使用缺省名字
		const char* name = argc > 1 ? argv[1] : "Jack B. Quick";
		//请求帐户管理员找出一个指定名字的帐户，无此帐户则新开一个
		Bank::Account_var account = manager->open(name);
		// 取该帐户的余额
		CORBA::Float balance;
		balance = account->getBalance();
		// Print out the balance.
		cout << "The balance in " << name << "'s account is $"
			 << balance << endl;
	} catch(const CORBA::Exception& e) {
	}
	return 0;
}
```

```cpp
// 程序 3-12 C++客户程序使用的 makefile
# C++客户程序的使用的 makefile
include $(VBROKERDIR)/examples/stdmk

EXE = Client
all: $(EXE)
clean:
	-rm -f core *.o *.hh *.cc $(EXE)
	-rm -rf SunWS_cache

#
# "Bank" specific make rules
#

Bank_c.cc: Bank.idl
	$(ORBCC) Bank.idl

Client: Bank_c.o Client.o
	$(CC) -o Client Client.o Bank_c.o \
	$(LIBPATH) $(LIBORB) $(STDCC_LIBS)
```

在命令行状态使用以下命令编译客户程序：
```bash
prompt> make
```
在命令行状态使用以下命令编译客户程序：
```bash
prompt> ./Client
```
**该客户端程序可以访问前面用Java实现的服务端程序，完成对服务端账户的访问**。至此，本节例子程序可以实现跨越机器、操作系统与程序设计语言的互操作，如用Java 语言实现的服务端程序，可以运行在一台安装Windows操作系统的机器上，而用C++语言实现的客户端程序，可以运行在另一台安装Linux操作系统的机器上。
### 3. 进一步尝试
可在前面讨论的基础上，进一步尝试 `CORBA` 对于可互操作的良好支持：
- **跨越 `ORB` 平台**：由于 `CORBA 2.0` 引入了 `GIOP` 与 `IIOP` ，因此基于不同厂商 `ORB` 平台的客户端程序与服务端程序，也可以方便地交互，可再安装另一支持 `IIOP` 的 `CORBA` 平台（如 `Orbix` 等），将本例的客户端程序或服务端程序移植到该平台上，进行跨越 `ORB` 平台的互操作。
- **跨越体系结构**：`CORBA` 中 `ESIOP` 可以解决一部分跨越体系结构的互操作问题，如一个 `CORBA` 对象可通过协议桥接操作一个 `DCOM` 对象，也可以参照OMG相关手册进行尝试。

---
# 思考与练习
3-1 将练习1-4用 `CORBA` 实现。
3-2 试参考 `VisiBroker for C++` 的联机参考手册，将3-9的客户程序改写为与3-11类似的C++语言实现，尝试这两种不同的程序设计语言能很好地进行互操作。
3-3** 从本章的例子来看，程序3-11所示的服务程序完成的工作是比较固定的，那就是创建并激活分布式对象，而这一工作完全可以通过平台自动完成、而不需要开发人员编写代码实现，你认为有没有必要由开发人员编写该服务程序？为什么？
