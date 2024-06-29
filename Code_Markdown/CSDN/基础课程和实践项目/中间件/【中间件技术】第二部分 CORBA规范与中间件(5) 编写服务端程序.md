@[toc]

从[【中间件技术】第二部分 CORBA规范与中间件(3) 基于CORBA的开发过程](https://memcpy0.blog.csdn.net/article/details/122645195)给出的服务程序的步骤来看，服务程序的流程比较固定，这个工作可以交给平台自动去完成。但是，该流程仅是一个典型简单服务程序的流程，==当服务端程序比较复杂（如需要管理大量服务端对象）时，服务程序就可能不会完全遵循该流程==。比如，可能当有客户端请求某个对象时，才动态地创建并激活它，甚至服务端程序可能会根据客户的请求，动态地创建新的对象适配器。

`CORBA` 中间件的另一优势就是**提供了灵活的服务端模型**，该模型基于可移植对象适配器体系结构，可帮助开发人员有效管理复杂的服务端程序。这部分讨论 **`CORBA` 服务程序的基本结构与机制**。

---
# 5.1 可移植对象适配器
## 5.1.1 `CORBA` 对象与伺服对象
这是两个不同抽象层次的概念，==抽象的 `CORBA` 对象与具体的伺服对象之间的彻底分离，造就了 `CORBA` 独立于任何特定程序设计语言的特性，并为服务端程序的可移植性打下基础==。**对象适配器** `object adapter` 是一个重要的 `ORB` 组件，它负责**将抽象的 `CORBA` 对象映射到具体的伺服对象**。

`CORBA` 对象是一个抽象意义上的对象，可看作一个具有对象标识、对象接口、以及对象实现的抽象实体。**它之所以被称为抽象的，是因为并没有硬性规定 `CORBA` 对象的实现机制**。由于独立于程序设计语言和特定 `ORB` 产品，一个 `CORBA` 对象的引用又称**可互操作的对象引用** `Interoperable Object Reference, IOR` 。从客户程序的角度看，`IOR` 中包含了对象的标识、接口类型以及其他信息以查找对象实现。在 `CORBA` 应用程序中简称“对象”或“对象引用”时，指的是 `CORBA` 对象或 `IOR` 。

**伺服对象 `servant` 则是指具体程序设计语言的对象或实体，通常存在于一个服务程序进程之中**。==客户程序通过对象引用发出的请求，经过 `ORB` 担当中介角色，转换为对特定的伺服对象的调用。在一个 `CORBA` 对象的生命期中，它可能与多个伺服对象相关联，因而对该对象的请求可能被发送到不同的伺服对象==。

对象标识 `Object ID` 通常是一个字符串，用于在对象适配器中标识一个 `CORBA` 对象。对象标识既可由程序员指派，也可由对象适配器自动分配，这两种方式都要求：**对象标识在创建它的对象适配器中必须具有唯一性**。

**伺服对象通过对象标识关联到 `CORBA` 对象**。建立一个伺服对象与一个 `CORBA` 对象之间关联的过程，称为“激活” `activate` 或“体现” `incarnate` ，反之，撤销这种关联的过程称为“冻结” `deactivate` 或“净化” `etherealize` 。**经过激活或体现后，`CORBA` 对象、伺服对象以及对象标识均可称为活动的 `active`** ，否则我们称 `CORBA` 对象、伺服对象以及对象标识为非活动的 `inactive` 。

所谓**持久对象** `persistent object` 或**瞬时对象** `transient object` 都是指一个 `CORBA` 对象。==持久对象可在创建它的服务程序进程之外存在，瞬时对象只能在创建它的服务程序进程中存在==。

## 5.1.2 对象适配器
服务程序的主要任务是**利用对象实现创建伺服对象**，然后**将这些服务端本地的对象实例、转换为可供远程使用的 `CORBA` 对象**。由客户程序通过对象标识解析相应的 `CORBA` 对象引用后，调用对象实现提供的各种服务。==应用程序开发人员管理服务端伺服对象、对象标识、对象引用、以及它们之间关联的主要工具是**对象适配器**==。

对象适配器负责决定，在收到客户请求时应调用哪个伺服对象，然后调用该伺服对象上的合适操作。`CORBA` 支持多种不同类型的对象适配器，但所有对象适配器的主要作用都是==创建对象引用，并将对象引用与「真正执行服务的程序设计语言伺服对象」相关联==。

`CORBA` 规范从 $2.2$ 版开始提供了**可移植对象适配器** `Portable Object Adapter, POA` ，作为各种 `ORB` 产品的标准对象适配器。`POA` 提供了更完善的机制，取代早期的**基本对象适配器** `Basic Object Adapter, BOA` ，成为我们开发服务端程序的主要工具。
## 5.1.3 可移植对象适配器体系结构
**`POA` 是对象实现与 `ORB` 其他组件之间的中介**，它将客户请求传送到伺服对象时，可能激活伺服对象，也可能按需创建子 `POA` 。**对 `CORBA` 对象的所有调用都会通过 `POA`**，即使目标对象是本地的，即伺服对象与发出调用的对象位于同一地址空间——这使得 `POA` 可以统一地应用 `POA` 策略。由于 `POA` 以相同的方式处理本地对象与远程对象，所以我们还可利用 `POA` 的优点处理本地对象。`POA` 体系结构如图5-1所示。
![图 5-1 可移植对象适配器POA](https://img-blog.csdnimg.cn/bf6ecdd73af04a00ae7fe5843f37c2fb.png)
由客户程序发出的请求中，包含了创建对象引用的 `POA` 名字、对象标识、以及目标机器与端口等信息。如果目标 `POA` 不存在，`ORB` 还允许调用**适配器激活器**创建所需的 `POA`。**一旦请求被 `ORB` 传送到正确的机器与端口，监听该端口的 `POA` 管理器负责检查对象关键码 `object key`** ，对象关键码中含有 `POA` 名字与对象标识；`POA` 管理器利用对象关键码，将请求传送给正确的 `POA`，然后由 `POA` 利用对象关键码确定对象标识，根据 `POA` 的策略集直接或间接地利用对象标识、将请求传送到正确的伺服对象。
### 1. `POA` 层次
一个服务程序进程中可使用多个 `POA`，不同 `POA` 通过名字区别开来。**这些 `POA` 的集合呈现一种层次结构，即每个 `POA` 都有一个父 `POA`，所有 `POA` 都是根 `POA` 的后代**。每个 `ORB` 在创建时都自动带有一个根 `POA`，可根据需要从根 `POA` 创建其他子 `POA`。
 
`POA` 层次为服务程序中的对象标识提供了一个层次化的名字空间。通常**每个伺服对象最多仅与一个 `POA` 相关联**，`POA` “拥有”这些对象并负责删除这些对象，**当 `POA` 被删除时，其中的所有对象以及子 `POA` 也将被删除**，因而 `POA` 层次的语义应理解为「仅仅是 `POA` 删除行为的包含层次」。
### 2. `POA` 管理器
`POA` 管理器 `POA Manager` 是一个对象，它将一个或多个 `POA` 组织在一起，为其中的 `POA` 提供共同的操作，**`POA` 管理器的状态代表了它所管理的所有 `POA` 的状态**。例如，开发人员可通过 `POA` 管理器提供的操作，决定是否接收/丢弃对 `POA` 管理器所控制的 `POA` 的请求，也可利用 `POA` 管理器终止 `POA`。
### 3. 活动对象映射表
基于 `BOA` 的实现途径，要求 `CORBA` 对象与伺服对象必须同时存在，但**基于 `POA` 的对象实现允许：`CORBA` 对象不必与伺服对象相关联即可存在**，并**支持伺服对象不必与 `CORBA` 对象相关联也可存在**。

每一个 `POA` 中都有一个活动对象映射表 `Active Object Map, AOM` ，表中保存了活动对象的对象标识、以及与之关联的伺服对象，其作用是：**将活动对象通过对象标识映射到伺服对象**。**在一个特定的 `POA` 中，对象标识唯一地标识了一个 `CORBA` 对象**。

为将伺服对象转换为一个可供远程调用的 `CORBA` 对象，必须建立 `CORBA` 对象与伺服对象之间的关联，这一关联过程甚至可以按需 `on-demand` 完成。此外，**`POA` 还允许单个伺服对象（即缺省伺服对象）同时与多个 `CORBA` 对象相关联**，这种特性对开发大规模应用意义重大。
### 4. 伺服对象管理器
伺服对象管理器 `servant manager` 是程序员自己提供的代码，用于**取代 `POA` 活动对象映射表的功能**。如果应用程序需要以一种更复杂的方案将对象标识映射到伺服对象，这时开发人员可设计专用的伺服对象管理器，它负责决定一个 `CORBA` 对象是否存在，然后查找伺服对象、并将伺服对象指派给 `CORBA` 对象。

为满足不同的应用需要，开发人员可设计多个伺服对象管理器。有两类伺服对象管理器可供选择：**伺服对象激活器**和**伺服对象定位器**，开发人员可利用 `POA` 策略决定选用哪一种类型。
## 5.1.4 模块 `PortableServer` 的 `IDL` 定义
**`POA` 及其组件均以 `IDL` 接口定义，这些定义组成了 `PortableServer` 模块**，如程序5-1所示。其中关键的接口包括 `POA`、`POAManager` 和 `AdapterActivator` ，与伺服对象管理器相关的 `ServantManager` 、`ServantActivator` 和 `ServantLocator`，以及 `ThreadPolicy`、`LifespanPolicy` 等各种策略。

```java
// 程序 5-1 模块 PortableServer 的 IDL 定义
module PortableServer {
	// 向前引用
	interface POA;
	typedef sequence<POA> PDAList;
	native Servant;
	typedef sequence<octet> ObjectId;
	exception ForwardRequest { Object forward_reference; };
	// 策略接口 
	// ---------------------------------------------------------------
	const CORBA::PolicyType THREAD_POLICY_ID = 16;
	const CORBA::PolicyType LIFESPAN_POLICY_ID = 17;
	const CORBA::PolicyType ID_UNIQUENESS_POLICY_ID = 18;
	const CORBA::PolicyType ID_ASSIGNMENT_POLICY_ID = 19;
	const CORBA::PolicyType IMPLICIT_ACTIVATION_POLICY_ID = 20;
	const CORBA::PolicyType SERVANT_RETENTION_POLICY_ID = 21;
	const CORBA::PolicyType REQUEST_PROCESSING_POLICY_ID = 22;
	
	enum ThreadPolicyValue { ORB_CTRL_MODEL, SINGLE_THREAD_MODEL };
	interface ThreadPolicy : CORBA::Policy {
		readonly attribute ThreadPolicyValue value;
	};
	
	enum LifespanPolicyValue { TRANSIENT, PERSISTENT };
	interface LifespanPolicy : CORBA::Policy {
		readonly attribute LifespanPolicyValue value;
	};
	
	enum IdUniquenessPolicyValue { UNIQUE_ID, MULTIPLE_ID };
	interface IdUniquenessPolicy : CORBA::Policy {
		readonly attribute IdUniquenessPolicyValue value;
	};
	
	enum IdAssignmentPolicyValue { USER_ID, SYSTEM_ID };
	interface IdAssignmentPolicy : CORBA::Policy {
		readonly attribute IdAssignmentPolicyValue value;
	};
	
	enum ImplicitActivationPolicyValue { IMPLICIT_ACTIVATION, NO_IMPLICIT_ACTIVATION };
	interface ImplicitActivationPolicy : CORBA::Policy {
		readonly attribute ImplicitActivationPolicyValue value;
	};
	
	enum ServantRetentionPolicyValue { RETAIN, NON_RETAIN };
	interface ServantRetentionPolicy : CORBA::Policy {
		readonly attribute ServantRetentionPolicyValue value;
	};
	
	enum RequestProcessingPolicyValue {
		USE_ACTIVE_OBJECT_MAP_ONLY,
		USE_DEFAULT_SERVANT,
		USE_SERVANT_MANAGER
	};

	interface RequestProcessingPolicy : CORBA::Policy {
		readonly attribute RequestProcessingPolicyValue value;
	};
	
	// POA 管理器接口
	// ---------------------------------------------------------------
	interface POAManager {
		exception AdapterInactive {};
		enum State { HOLDING, ACTIVE, DISCARDING, INACTIVE };
		void activate() raises(AdapterInactive);
		void hold_requests(in boolean wait_for_completion)
			raises(AdapterInactive);
		void discard_requests(in boolean wait_for_completion)
			raises(AdapterInactive);
		void deactivate(in boolean etherealize_objects, in boolean wait_for_completion)
			raises(AdapterInactive);
		State get_state(); 
	};
	
	// 适配器激活器接口
	// ---------------------------------------------------------------
	interface AdapterActivator {
		boolean unknown_adapter(in POA parent, in string name);
	};
	
	// 伺服对象管理器接口
	// ---------------------------------------------------------------
	interface ServantManager {};
	interface ServantActivator : ServantManager {
		Servant incarnate(in ObjectId id, in POA adapter) raises(ForwardRequest);
		void etherealize(in ObjectId id, in POA adapter, in Servant serv,
			in boolean cleanup_in_progress, in boolean remaining_activations);
	};
	interface ServantLocator : ServantManager {
		native Cookie;
		Servant preinvoke(in ObjectId id, in POA adapter,
			in CORBA::Identifier operation, out Cookie the_cookie) raises(ForwardRequest);
		void postinvoke(in ObjectId id, in POA adapter, in CORBA::Identifier operation,
			in Cookie the_cookie, in Servant the_servant);
	};

	// POA 接口
	// ---------------------------------------------------------------
	interface POA {
		// POA 属性
		readonly attribute string the_name;
		readonly attribute POA the_parent;
		readonly attribute PDAList the_children;
		readonly attribute POAManager the_POAManager;
		attribute AdapterActivator the_activator;
		// 异常定义
		exception AdapterAlreadyExists {};
		exception AdapterNonExistent {};
		exception InvalidPolicy { unsigned short index; };
		exception NoServant {};
		exception ObjectAlreadyActive {};
		exception ObjectNotActive {};
		exception ServantAlreadyActive {};
		exception ServantNotActive {};
		exception WrongAdapter {};
		exception WrongPolicy {};
		
		// 创建与撤销 POA
		POA create_POA(in string adapter_name, in POAManager a_POAManager,
			in CORBA::PolicyList policies) raises(AdapterAlreadyExists, InvalidPolicy);
		POA find_POA(in string adapter_name, in boolean activate_it) raises(AdapterNonExistent);
		void destroy(in boolean etherealize_objects, in boolean wait_for_completion);
		
		// 用于创建策略对象的 factory 操作
		ThreadPolicy create_thread_policy(in ThreadPolicyValue value);
		LifespanPolicy create_lifespan_policy(in LifespanPolicyValue value);
		IdUniquenessPolicy create_id_uniqueness_policy(
			in IdUniquenessPolicyValue value);
		IdAssignmentPolicy create_id_assignment_policy(
			in IdAssignmentPolicyValue value); 
		ImplicitActivationPolicy create_implicit_activation_policy(
			in ImplicitActivationPolicyValue value);
		ServantRetentionPolicy create_servant_retention_policy(
			in ServantRetentionPolicyValue value);
		RequestProcessingPolicy create_request_processing_policy(
			in RequestProcessingPolicyValue value);
			
		// 伺服对象管理器的注册
		ServantManager get_servant_manager() raises(WrongPolicy);
		void set_servant_manager(in ServantManager imgr) raises(WrongPolicy);
		
		// 为 USE_DEFAULT_SERVANT 策略提供的操作
		Servant get_servant() raises(NoServant, WrongPolicy);
		void set_servant(in Servant p_servant) raises(WrongPolicy);
		
		// 对象激活与冻结
		ObjectId activate_object(in Servant p_servant)
			raises(ServantAlreadyActive, WrongPolicy);
		void activate_object_with_id(in ObjectId id, in Servant p_servant)
			raises(ServantAlreadyActive, ObjectAlreadyActive, WrongPolicy);
		void deactivate_object(in ObjectId oid) raises(ObjectNotActive, WrongPolicy);
		
		// 创建 CORBA 对象引用的操作
		Object create_reference(in CORBA::RepositoryId intf) raises(WrongPolicy);
		Object create_reference_with_id(in ObjectId oid, in CORBA::RepositoryId intf)
			raises(WrongPolicy);
			
		// 对象标识映射操作
		ObjectId servant_to_id(in Servant p_servant) raises(ServantNotActive, WrongPolicy);
		Object servant_to_reference(in Servant p_servant) raises(ServantNotActive, WrongPolicy);
		Servant reference_to_servant(in Object reference) raises(ObjectNotActive, WrongAdapter, WrongPolicy);
		ObjectId reference_to_id(in Object reference) raises(WrongAdapter, WrongPolicy);
		Servant id_to_servant(in ObjectId oid) raises(ObjectNotActive, WrongPolicy);
		Object id_to_reference(in ObjectId oid) raises(ObjectNotActive, WrongPolicy);
	};

	// Current 接口，用于访问正被调用的 POA 与对象标识
	// ---------------------------------------------------------------
	interface Current : CORBA::Current {
		exception NoContext {};
		POA get_POA() raises(NoContext);
		ObjectId get_object_id() raises(NoContext);
	};
};
```
---
# 5.2 设计 `POA` 策略
## 5.2.1 什么是 `POA` 策略?
**`POA` 策略是一个对象，负责控制相关 `POA` 的行为、以及这些 `POA` 所管理的对象**，使用 `POA` 前应仔细考虑「应用程序所需的策略集」。例如，程序中有大量粒度细小的对象时，设计者需要一种优化方式；当程序中有大量需要长时间的操作时，设计者可能需要另一种优化方式。`POA` 策略可为「不同的应用程序设计目标」而配置不同的 `POA`。
## 5.2.2 选用 `POA` 策略
目前，`CORBA` 规范定义了 $7$ 种标准的 `POA` 策略，不同的 `ORB` 产品从可伸缩性或可靠性等因素，考虑引入一些新的 `POA` 策略。**由于 `POA` 及其策略都是以 `IDL` 定义的，不同 `ORB` 产品很容易扩充新的策略、或为特定策略添加新的特性**。

选用 `POA` 策略时，应注意某些策略值之间存在依赖关系，例如 `IMPLICIT_ACTIVATION` 策略要求同时使用 `SYSTEM_ID` 和 `RETAIN` 策略，而 `USE_ACTIVE_OBJECT_MAP_ONLY` 策略还必须结合 `RETAIN` 策略使用。
### 1. 线程策略
该策略指定 `POA` 使用的线程模型，它有两种取值：
- `ORB_CTRL_MODEL`（缺省值）：表示**由 `POA` 负责将请求指派到线程**。在多线程环境中，并发请求可以用多线程传送。
- `SINGLE_THREAD_MODEL` ：这时只有一个线程，`POA` 顺序地处理请求。
### 2. 生命期策略
该策略指定 `POA` 中对象实现的使用期限，它可有以下值：
- `TRANSIENT`（缺省值）：由 `POA` 创建的对象引用是瞬时的，这些对象在创建它的 `POA` 之外不可存在。一旦 `POA` 被冻结为非活动状态，调用该 `POA` 创建的任何对象引用将引发 `OBJECT_NOT_EXIST` 异常。**对话型对象通常设计为瞬时对象**。
- `PERSISTENT`：由 `POA` 创建的持久对象可在创建它的 `POA` 之外存在，因而使用这一策略，通常还会同时采用 `USER_ID` 策略。对持久对象的请求，可能导致隐式地激活一个进程、一个 `POA` 、以及实现该对象的伺服对象，这意味着 **`POA` 应注册伺服对象定位器或激活器**。**实体型对象通常设计为持久对象**。

### 3. 对象标识唯一性策略
该策略允许多个抽象对象共享一个伺服对象，它可取下列值：
- `UNIQUE_ID`（缺省值）：被激活的伺服对象仅支持一个对象标识，**`POA` 不允许一个伺服对象与多个 `CORBA` 对象相关联**。
- `MULTIPLE_ID`：被激活的伺服对象可以有一个或多个对象标识，在运行时刻调用伺服对象的操作时，必须由操作内部决定对象标识。应指出的是，**对象标识在特定的 `POA` 中总是唯一的**，对象标识唯一性策略指的是：**对象标识与伺服对象之间关联的唯一性或多重性**。

### 4. 对象标识指派策略
该策略指定对象标识是由程序员编写的服务程序生成、还是由 `POA` 自动生成，它可取以下值：
- `USER_ID` ：由应用程序为 `POA` 的对象引用指定对象标识。
- `SYSTEM_ID`（缺省值）：由 `POA` 为它的对象引用分配对象标识。**如果同时还采用了 `PERSISTENT` 策略，则在同一 `POA` 的所有实例中，对象标识必须是唯一的**。

典型情况将是 `USER_ID` 策略用于持久对象，将 `SYSTEM_ID` 策略用于瞬时对象。如果想将 `SYSTEM_ID` 策略用于持久对象，可从伺服对象或对象引用中提取对象标识。 
### 5. 伺服对象保持策略
该策略指定 `POA` 是否将活动伺服对象保存在活动对象映射表中，它有两种取值：
- `RETAIN`（缺省值）：`POA` 利用活动对象映射表，跟踪对象的激活情况，通常**与伺服对象激活器或 `POA` 显式激活方式结合使用**。
- `NON_RETAIN` ：`POA` 不在活动对象映射表中保存活动的伺服对象，通常**结合伺服对象定位器一起使用**。

该策略决定 `POA` 是否利用活动对象映射表，跟踪对象标识与伺服对象之间的关联。如果选择 `NON_RETAIN` 策略，则意味着不使用活动对象映射表，因而**使用该策略的应用程序必须提供一个伺服对象定位器类型的伺服对象管理器**。
### 6. 请求处理策略
该策略指定 `POA` 如何处理请求，它有三种取值：
- `USE_ACTIVE_OBJECT_MAP_ONLY`（缺省值）：`POA` 仅依赖于活动对象映射表，决定哪些对象标识可用、以及对象标识关联到哪些伺服对象。如果在活动对象映射表中找不到对象标识，则引发 `OBJECT_NOT_EXIST` 异常。**该值必须结合 `RETAIN` 策略使用**。
- `USE_DEFAULT_SERVANT`：如果 `POA` 在活动对象映射表中找不到对象标识，或已设置 `NON_RETAIN` 策略，则将请求分派给一个缺省伺服对象。缺省伺服对象必须先注册，如果未注册则引发 `OBJ_ADAPTER` 异常。**该值必须结合 `MULTIPLE_ID` 策略使用**。
- `USE_SERVANT_MANAGER` ：如果 `POA` 在活动对象映射表中找不到对象标识，或已设置了 `NON_RETAIN` 策略，则 **`POA` 使用一个伺服对象管理器激活伺服对象**。

### 7. 隐式激活策略
该策略指定 `POA` 是否支持伺服对象的隐式激活，它可取以下值：
- `IMPLICIT_ACTIVATION` ：`POA` 支持隐式激活，服务程序可调用 `servant_to_reference` 操作或 `servant_to_id` 操作，将伺服对象添加到活动对象映射表、并转换为对象引用，也可调用伺服对象的 `_this` 方法激活伺服对象。**该值要求同时使用`SYSTEM_ID` 和 `RETAIN` 策略**。
- `NO_IMPLICIT_ACTIVATION`（缺省值）：`POA` 不支持伺服对象的隐式激活，**只有通过显式的调用，才可将伺服对象与一个对象标识相关联**。

### 8. 绑定支持策略
这是一个 `VisiBroker` 特有的策略，负责控制注册到 `VisiBroker` 智能代理（`osagent`）的 `POA` 和活动对象。虽然使用该策略会影响服务程序的可移植性，但对提高服务端的可伸缩性有很大帮助。

设想服务程序中有数千个对象，将它们都注册到智能代理是不可行的，这时可改为**将 `POA` 注册到智能代理**。由于客户请求中包含 `POA` 名字与对象标识，智能代理可正确地转发请求。该策略可取以下值：
- `BY_INSTANCE` ：所有活动对象注册到智能代理。该值要求同时使用 `PERSISTENT` 和 `RETAIN` 策略。
- `BY_POA`（缺省值）：**仅将 `POA` 注册到智能代理。该值要求同时使用 `PERSISTENT`策略**。
- `NONE` ：`POA` 和活动对象都不注册到智能代理。
## 5.2.3 `POA` 处理请求小结
`ORB` 通过 `POA` 管理器，将客户请求传送到合适的 `POA` 后，**该 `POA` 对请求的处理方式取决于「它所采用的策略」以及「对象的激活状态」**。

如果 `POA` 采用 `RETAIN` 策略，则 `POA` 在活动对象映射表中查找与「请求中对象标识」相关联的伺服对象，找到伺服对象，则由 `POA` 调用该伺服对象的相应方法；如果 `POA` 采用 `NON_RETAIN` 策略，或采用 `RETAIN` 策略、但找不到关联的伺服对象，则代之以如下步骤：
1. 如果 `POA` 采用 `USE_DEFAULT_SERVANT` 策略，则由 `POA` 调用缺省伺服对象的相应操作。
2. 如果 `POA` 采用 `USE_SERVANT_MANAGER` 策略，则当 `POA` 采用 `RETAIN` 策略时、由 `POA` 调用伺服对象激活器的 `incarnate` 操作，当 `POA` 采用 `NON_REATIN` 策略时、调用伺服对象定位器的 `preinvoke` 操作；如果伺服对象管理器未能成功地将对象“体现”出来，则引发 `ForwardRequest` 异常。
3. 如果 `POA` 采用 `USE_OBJECT_MAP_ONLY` 策略，则引发一个异常。

---
# 5.3 使用 `POA`
使用 `POA` 之前，必须**首先根据应用程序的需要，设计好不同的 `POA` 策略集**，然后 `CORBA` 应用程序通常遵循以下步骤创建与激活 `POA`：
1. 调用 `ORB` 伪对象的 `resolve_initial_references` 操作，获取根 POA 的引用；
2. 调用根 `POA` 的操作，定义应用程序所需的 `POA` 策略；
3. 用自定义策略，在根 `POA` 下创建一个子 `POA`，甚至创建一个完整的 `POA` 层次；
4. 激活 `POA` 管理器，**通常整个服务程序只需使用一个 `POA` 管理器**；
5. 创建并激活伺服对象。

上述过程仅仅是一个典型的 `CORBA` 服务程序开发过程，[第三章例子程序](https://memcpy0.blog.csdn.net/article/details/122645195)演示了这些步骤。由于不同应用程序各自的需求与特点不一样，在开发具体应用程序时，上述步骤可能有所不同。例如：
- 为避免大量客户请求导致服务程序崩溃、或易于监控服务端程序，在第四步中服务程序可能不是简单地激活 `POA` 管理器，而是由系统管理员在服务器控制台、以交互方式控制 `POA` 管理器的状态；
- 又如，在第五步中服务程序未必使用以对象标识显式激活对象的方式，因为这种方式难以管理服务程序中的大量对象，**如果采用按需激活，还必须设计一个伺服对象管理器**；
- 此外，服务程序中还可能**使用适配器激活器，按需创建新的 `POA`** 。

本小节主要讨论，如何利用 `POA` 完成解决这些复杂问题的设计。
## 5.3.1 获取根 `POA` 对象引用
**所有服务程序都必须获取根 `POA` 的对象引用**，该对象引用可直接用于管理对象，也可用于创建新的子 `POA`。例如代码：
```java
org.omg.CORBA.Object obj = orb.resolve_initial_references("RootPOA");
POA rootPOA = org.omg.PortableServer.POAHelper.narrow(obj);
```
为获取了根 `POA` 的对象引用 `rootPOA` ，要通过 `ORB` 的 `resolve_initial_references` 操作，将硬编码 `"RootPOA"` 解析为一个通用对象，然后利用 `POA` 的 `Helper` 类，将通用对象窄化为 `POA` 类型。

**根 `POA` 有一个预定义的策略集**，即线程策略为 `ORB_CTRL_MODEL` 、生命期策略为 `TRANSIENT` 、对象标识唯一性策略为 `UNIQUE_ID` 、对象标识指派策略为 `SYSTEM_ID` 、伺服对象保持策略为 `RETAIN` 、请求处理策略为 `USE_ACTIVE_OBJECT_MAP_ONLY` 、隐式激活策略为 `IMPLICIT_ACTIVATION` 。

应留意到，根 `POA` 的策略值与各种策略的缺省值基本相同，**唯一例外是隐式激活策略值不同于隐式激活策略的缺省值**。==由于仅当创建 `POA` 时才可设置策略，服务程序不可改变已有 `POA` 的策略，因而根 `POA` 的策略集是不可更改的==。
## 5.3.2 创建自定义策略的 `POA`
**如果需要不同的 `POA` 行为**，例如采用持久对象策略，**则需要创建新的子 `POA`** 。`POA` 的层次结构并不意味着 `POA` 策略也具有相应的层次结构，**子 `POA` 并不继承其父 `POA` 的任何策略**。

调用 `POA` 对象引用的 `create_POA` 方法，可创建新的 `POA` 作为该 `POA` 的子 `POA` 。程序员可根据实际需要，以这种方式创建多个子 `POA` ，形成一个 `POA` 层次。例如代码：

```java
org.omg.CORBA.Policy[] policies = {
	rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
};
POA myPOA = rootPOA.create_POA("ABC", rootPOA.the_POAManager(), policies);
```
从根 `POA` 创建一个名为 `"ABC"` 的子 `POA`，该子 `POA` 具有持久的生命期策略。

**服务程序创建子 `POA` 时，只需提供子 `POA` 的名字，客户程序则必须使用完整的 `POA` 名字指定 `POA`** 。一个完整的 `POA` 名字记录了包含该子 `POA` 名字在内的整个 `POA` 层次路径，层次间用斜杠分隔。例如，`"/A/B/C"` 表示 `C` 是 `B` 的子 `POA` ，`B` 又是 `A` 的子 `POA` ，**首个斜杠表示根 `POA`** 。创建 `POA` 时可随意为 `POA` 命名，但 **`POA` 名字必须在父 `POA` 的所有子 `POA` 中是唯一的**，否则会引发 `AdapterAlreadyExists` 异常。
## 5.3.3 使用 `POA` 管理器
**创建新 `POA` 时，必须为它指定一个 `POA` 管理器，该 `POA` 管理器将用于控制新 `POA` 的状态**。在大多数应用中，服务程序的所有 `POA` 均使用同一 `POA` 管理器。例如，上例中根 `POA` 的 `POA` 管理器用于控制新创建的子 `POA` 的状态。如果将 `null` 作为第二个参数传递给 `create_POA` 操作，系统会自动创建一个新的 `POA` 管理器与新建的 `POA` 相关联。`POA` 管理器的撤销也是隐式地，当 `POA` 管理器相关联的所有 `POA` 都被 `destroy` 操作撤销时，该 `POA` 管理器也被自动撤销。

一个 `POA` 管理器创建后，可以有 $4$ 种状态：持有状态 `holding` 、活动状态 `active` 、非活动状态 `inactive` 以及丢弃状态 `discarding` ，**这些状态也就是 `POA` 管理器所控制的 `POA` 的工作状态**。调用 `POA` 管理器的 `get_state` 操作，可返回 `POA` 管理器的当前状态。

`POA` 管理器的状态、以及引起状态转换的操作，如图5-2的状态转换图所示。
![图 5-2 POA和POA管理器的状态转换图](https://img-blog.csdnimg.cn/68595159063c447abb57e8d38742e500.png)
### 1. 活动状态
**当 `POA` 管理器处于活动状态时，由它控制的所有 `POA` 将接收并开始处理请求**。调用 `POA` 管理器的 `activate` 操作，可将 `POA` 管理器从持有或丢弃状态改为活动状态。

注意：**即使在活动状态下，由于 `ORB` 实现或系统资源限制等原因，`POA` 也可能需要将来不及处理的请求排队**。每一个 `ORB` 产品都会限制「请求队列的最大长度」，超过该限制时 `POA` 会返回一个 `TRANSIENT` 异常给客户程序。
### 2. 持有状态
**`POA` 管理器被创建后即处于持有状态，在该状态下所有接入的请求被引导到一个队列中等候处理，`POA` 并没有对请求作任何处理**。为分派这些请求，必须将 `POA` 管理器转换为活动状态，即**利用 `POA` 管理器的 `activate` 操作激活该 `POA` 管理器**。例如，以下代码将根 `POA` 的 `POA` 管理器激活为活动状态：
```java
rootPOA.the_POAManager().activate();
```
调用 `POA` 管理器的 `hold_requests` 操作，可将 `POA` 管理器由活动或丢弃状态改为持有状态，如果参数为 `false` ，表示**改为持有状态后立即返回**，否则仅当在状态改变前「已开始处理的所有请求」已结束、或 `POA` 管理器被改为「除持有状态之外的其他状态」时，该操作才返回（？）。**所有排队等候且未开始处理的请求，将在持有状态期间继续排队等候**。

==当 `POA` 管理器处于持有状态时，不会调用它所控制的所有`POA` 上注册的适配器激活器，所有需要适配器激活器的请求也将排队等候处理==。
### 3. 丢弃状态
**当 `POA` 管理器处于丢弃状态时，由它控制的所有 `POA` 丢弃任何未开始处理的请求**，此外不会调用注册到这些 `POA` 的任何适配器激活器。**引入丢弃状态，为服务程序提供了一种流量控制手段**，例如，为避免服务程序被突如期来的大量请求淹没，可利用该状态拒绝接入请求，这时 `ORB` 会返回 `TRANSIENT` 异常，通知客户程序它们的请求已被丢弃，由客户程序决定是否重新发送请求。没有什么固定的方法判断是否以及何时 `POA` 收到了太多请求，通常这需要由开发人员自己负责建立线程监控程序。

调用 `POA` 管理器的 `discard_requests` 操作，可将 `POA` 管理器从持有或活动状态改为丢弃状态。实际参数为 `false` 表示**改为丢弃状态后立即返回**，否则仅当状态改变前「启动的所有请求」均已结束、或 `POA` 管理器被改为「除丢弃状态之外的其他状态」时，该操作才返回（？）。
### 4. 非活动状态
**当 `POA` 管理器处于非活动状态时，由它控制的所有 `POA` 拒绝接入请求**。与丢弃状态不同，**仅当 `POA` 管理器要关闭时才使用该状态**，因为处于该状态的 `POA` 管理器不能再转换到任何其他状态，否则会引发 `AdapterInactive` 异常。

`POA` 管理器的 `deactivate` 操作，可将 `POA` 管理器从任何其他状态改为非活动状态：
- 第一个参数为 `true` 表示状态改变后，所有具有 `RETAIN` 和 `USE_SERVANT_MANAGER` 策略的 `POA` ，立即为每个活动对象调用伺服对象激活器的 `etherealize` 操作，否则不调用 `etherealize` 操作；
- 第二个参数为 `false` 表示**改为非活动状态后立即返回**，否则仅当在状态改变前「已启动的所有请求」已结束，或「所有具有 `RETAIN` 和 `USE_SERVANT_MANAGER` 策略的 `POA` 」调用了 `etherealize` 方法后，该操作才返回。
## 5.3.4 激活与冻结对象
**激活**是指**建立 `CORBA` 对象与伺服对象之间的关联**，**冻结**是指**撤销这种关联**。
- 如果 `POA` 采用 `RETAIN` 策略，对象既可被显式激活、隐式激活或按需激活，也可使用缺省伺服对象。
- 如果 `POA` 采用 `NON_RETAIN` 策略，对象只能按需激活或使用缺省伺服对象。
### 1. 显式激活对象
显式激活对象是指**在服务程序启动时，显式地调用 `POA` 的 `activate_object_with_id` 操作 或 `activate_object` 操作激活对象**。
#### (1) `POA` 的 `activate_object_with_id` 操作
==该操作要求提供对象标识和伺服对象，并将两者的关联登记在活动对象映射表中==。**该操作常用于显式激活持久对象，当服务程序需要管理大量对象时，该操作有明显的不足之处**。第三、四章例子程序的 `Server.java` 都演示了 `activate_object_with_id` 操作的用法。
- 调用该操作的 `POA` 必须采用 `RETAIN` 策略，否则将引发 `WrongPolicy` 异常；
- 如果对象标识已在活动对象映射表中存在，则引发 `ObjectAlreadyActive` 异常；
- 如果 `POA` 采用了 `UNIQUE_ID` 策略、且伺服对象已在活动对象映射表中存在，则引发 `ServantAlreadyActive` 异常。

#### (2) `POA` 的 `activate_object` 操作
==该操作只要求提供伺服对象，由系统指派对象标识，然后将两者的关联登记在活动对象映射表中，最后返回系统分配的对象标识==。**该操作常用于显式激活瞬时对象**。
- 调用该操作的 `POA` 必须同时采用 `RETAIN` 和 `SYSTEM_ID` 策略，否则将引发 `WrongPolicy` 异常；
- 如果 `POA` 采用了 `UNIQUE_ID` 策略、且伺服对象已在活动对象映射表中存在，则引发 `ServantAlreadyActive` 异常。
### 2. 隐式激活对象
如果创建 `POA` 时采用了合适的策略，可调用 `POA` 的 `servant_to_reference` 和 `servant_to_id` 操作、或伺服对象的 `_this` 操作，隐式地激活对象。
#### (1) `POA` 的 `servant_to_reference` 操作

**该操作以一个伺服对象为参数，返回一个对象引用**。
- 调用该操作的 `POA` 必须采用 `RETAIN` 结合 `UNIQUE_ID` 或 `IMPLICIT_ACTIVATION` 策略，否则将引发 `WrongPolicy` 异常。
- 如果 `POA` 同时采用 `RETAIN` 和 `UNIQUE_ID` 策略，且指定的伺服对象是活动的，则返回「活动对象映射表中找到的对象引用」；
- 如果 `POA` 同时采用 `RETAIN` 和 `IMPLICIT_ACTIVATION` 策略，且 `POA` 还采用 `MULTIPLE_ID` 策略、或指定的伺服对象不是活动的，则以「`POA` 生成的对象标识和接口库标识」建立与伺服对象的关联，然后返回相应的对象引用；
- 如果该操作是在「执行对指定的伺服对象的请求」中被调用，则返回与当前调用相关联的对象引用；
- 否则，引发 `ServantNotActive` 异常。

第三章例子程序的 `AccountManagerImpl.java` 演示了 `servant_to_reference` 操作的用法。注意，**执行该操作的 `POA` 是根 `POA`，而根 `POA` 预定义的隐式激活策略为 `IMPLICIT_ACTIVATION`** 。
#### (2) `POA` 的 `servant_to_id` 操作
与 `servant_to_reference` 操作不同，**该操作返回的是对象标识、而不是对象引用**。
- 调用该操作的 `POA` 可采用 `USE_DEFAULT_SERVANT` 策略，也可采用 `RETAIN` 结合 `UNIQUE_ID` 或 `IMPLICIT_ACTIVATION` 策略，否则将引发 `WrongPolicy` 异常。
- 如果 `POA` 同时采用 `RETAIN` 和 `UNIQUE_ID` 策略，且指定的伺服对象是活动的，则返回与伺服对象相关联的对象标识；
- 如果`POA`同时采用 `RETAIN` 和 `IMPLICIT_ACTIVATION` 策略，且 `POA` 还采用 `MULTIPLE_ID` 策略、或指定的伺服对象不是活动的，则以「 `POA` 生成的对象标识和接口库标识」建立与伺服对象的关联，然后返回生成的对象标识；
- 如果 `POA` 具有 `USE_DEFAULT_SERVANT` 策略，指定的伺服对象是缺省伺服对象，而且是在「对缺省伺服对象的请求」中调用该操作，则返回与当前调用相关联的对象标识；
- 否则，引发 `ServantNotActive` 异常。

综上所述：
- 如果 `POA` 采用 `UNIQUE_ID` 策略，则对非活动伺服对象调用上述两个操作，会导致隐式激活；
- 如果 `POA` 采用 `MULTIPLE_ID` 策略，则上述操作总会导致隐式激活，即使伺服对象已经是活动的。

==在隐式激活过程中，伺服对象是程序员指定的，而对象标识是由 `POA` 生成的，它们之间的关联将保存在 `POA` 的活动对象映射表中==。注意，**隐式激活是由服务程序的某些操作造成的**，而不是响应某个客户程序请求。**如果伺服对象不是活动的，客户程序请求一个非活动的对象，是无法让它变为活动的**。
### 3. 按需激活对象
按需激活 `activation on demand` 指 **`POA` 利用「程序员提供的伺服对象管理器」激活对象**。
 
按需激活要求 `POA` 采用 `USE_SERVANT_MANAGER` 策略，并且首先调用 `POA` 的 `set_servant_manager` 操作，设置 `POA` 的伺服对象管理器。`set_servant_manager` 操作在每个 `POA` 创建后只能调用一次，如果 `POA` 采用 `RETAIN` 策略，则参数必须支持 `ServantActivator` 接口；如果 `POA` 采用 `NON_RETAIN` 策略，则参数必须支持 `ServantLocator` 接口。

**如果 `POA` 收到客户程序一个对 `CORBA` 对象的请求，而该 `CORBA` 对象尚未建立与任何伺服对象之间的关联，这时将发生按需激活**。`POA` 将以对象标识为参数，调用「已注册的伺服对象管理器」的 `incarnate` 或 `preinvoke` 方法。伺服对象管理器可有几种处理方式，例如：
1. 查找合适的伺服对象，由该伺服对象为请求执行相应操作。
2. 引发一个 `OBJECT_NOT_EXIST` 异常返回给客户程序。
3. 将该请求转发给其他对象。

**`POA` 的策略决定了接下来可能发生的其他步骤**，例如如果 `POA` 设置了 `RETAIN` 策略，则用「伺服对象与对象标识的关联」更新活动对象映射表。5.4节将详细介绍两类伺服对象管理器的具体用法。
### 4. 使用缺省伺服对象
**如果 `POA` 采用 `USE_DEFAULT_SERVANT` 策略，则允许 `POA` 不管什么对象标识都调用「同一个缺省的伺服对象」，由单个伺服对象处理所有的客户请求**。使用缺省伺服对象前，必须首先调用 `POA` 的 `set_servant` 操作，设置该 `POA` 的缺省伺服对象，如程序5-2所示。
```java
// 程序 5-2 使用缺省伺服对象
// 服务端的主程序
import org.omg.PortableServer.*;
public class Server {
	public static void main(String[] args) {
		try {
			// 初始化 ORB
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			// 取根 POA 的一个引用
			POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			// 创建应用程序所需的 POA 策略
			org.omg.CORBA.Policy[] policies = {
				rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
				rootPOA.create_id_uniqueness_policy(IdUniquenessPolicyValue.MULTIPLE_ID),
				rootPOA.create_request_processing_policy(RequestProcessingPolicyValue.USE_DEFAULT_SERVANT)
			};
			// 用自定义策略创建新的 POA
			POA newPOA = rootPOA.create_POA("DefaultServantBankPOA", rootPOA.the_POAManager(), policies);
			// 创建伺服对象并设置为 POA 的缺省伺服对象
			AccountManagerImpl managerServant = new AccountManagerImpl();
			newPOA.set_servant(managerServant);
			// 激活 POA 管理器
			rootPOA.the_POAManager().activate();
			// 创建第一个对象引用
			newPOA.create_reference_with_id("Zhang3".getBytes(), "IDL:Bank/AccountManager:1.0");
			// 创建第二个对象引用
			newPOA.create_reference_with_id("Li4".getBytes(), "IDL:Bank/AccountManager:1.0");
			// 等待接入请求
			System.out.println("帐户管理员 Zhang3 和 Li4 已就绪 ...");
			orb.run();
		} catch(Exception exc) {
			exc.printStackTrace();
		}
	}
}
```
程序5-2实际上重写了第三章例子程序中的 `Server.java` ，其中新创建的 `newPOA` 采用 `USE_DEFAULT_SERVANT` 策略（该策略要求同时使用 `MULTIPLE_ID` ），并调用 `set_servant` 操作、将一个缺省伺服对象注册到 `newPOA` 。然后调用 `create_reference_with_id` 操作，分别创建了对象标识为 `Zhang3` 和 `Li4` 的对象引用，该操作的第二个参数 `"IDL:Bank/AccountManager:1.0"` 指定了「代表对象接口类型的库标识」。客户程序绑定到帐户管理员对象时，既可使用对象标识 `Zhang3` ，也可使用对象标识 `Li4` ，但实际上在服务端都是同一个伺服对象负责执行真正的操作。

注意，`create_reference_with_id` 操作==虽然创建了对象引用，但并未导致有任何对象的激活。这些对象引用可传递给客户程序，客户程序对这些对象引用发出请求时，才引起按需激活对象、或使用缺省伺服对象==。类似的 `POA`操作还有`create_reference` ，该操作利用「`POA` 自动分配的对象标识」创建对象引用。
### 5. 冻结对象
**可调用 `POA` 的 `deactivate_object` 操作冻结对象，即从 `POA` 的活动对象映射表中删除「对象标识与伺服对象之间的关联」**。该操作要求 `POA` 采用 `RETAIN` 策略。==对象被冻结并不意味着该对象被永久撤销，以后它还可以被重新激活==。

此外，还可通过调用 `POA` 管理器的 `deactivate` 操作，冻结由该 `POA` 管理器所控制的所有 `POA` ，`POA` 进入非活动状态后将拒绝对它所管理对象的任何请求。

---
# 5.4 伺服对象管理器
伺服对象管理器是程序员自己编写的代码，其主要作用是**查找并返回伺服对象**。并不是所有 `CORBA` 应用程序都需要伺服对象管理器，例如，如果服务程序在启动时装入所有对象，就无需使用任何伺服对象管理器。

有两类伺服对象管理器：伺服对象激活器和伺服对象定位器。要使用伺服对象管理器，必须为 `POA` 设置 `USE_SERVANT_MANAGER` 策略，并结合伺服对象保持策略，决定使用哪一种类型的伺服对象管理器。**采用 `RETAIN` 表示使用伺服对象激活器**，常用于激活持久对象；**采用 `NON_RETAIN` 表示使用伺服对象定位器**，常用于查找瞬时对象。
## 5.4.1 伺服对象激活器
**由 `ServantActivator` 类型的伺服对象管理器激活的对象，被记录在活动对象映射表中**。利用伺服对象激活器处理请求时，`POA` 首先查找活动对象映射表：
- 如果找到对象标识，则调用伺服对象的合适操作，并将结果返回给客户程序；
- 否则以对象标识与 `POA` 作为参数，调用伺服对象激活器的 `incarnate` 操作，由伺服对象激活器查找并返回一个合适的伺服对象，然后将该伺服对象登记到活动对象映射表中；
- 最后，调用伺服对象的合适操作，并将结果返回给客户程序。
- 此后，==可根据实际应用需要，决定是将伺服对象继续保留在活动对象映射表中，还是冻结该伺服对象==。

为将第三章例子程序中的帐户管理员对象**改为按需激活，使服务程序支持更灵活的对象管理方案**，我们为它提供一个伺服对象激活器，如程序5-3所示。
```java
// 程序 5-3 伺服对象激活器示例 AccountManagerActivator.java
// 一个伺服对象激活器类型的伺服对象管理器
import org.omg.PortableServer.*;
public class AccountManagerActivator extends ServantActivatorPOA { // 
	public Servant incarnate(byte[] oid, POA adapter) throws ForwardRequest { // 
		Servant servant;
		System.out.println("incarnate with ID = " + new String(oid));
		if ((new String(oid)).equalsIgnoreCase("Zhang3"))
			servant = (Servant) new AccountManagerImpl_1();
		else
			servant = (Servant) new AccountManagerImpl_2();
		new DeactivateThread(oid, adapter).start();
		return servant;
	}
	public void etherealize(byte[] oid, POA adapter, Servant serv,
		boolean cleanup_in_progress, boolean remaining_activations) { // 
		System.out.println("etherealize with ID = " + new String(oid));
		System.gc();
	}
}

class DeactivateThread extends Thread {
	byte[] _oid;
	POA _adapter;
	
	public DeactivateThread(byte[] oid, POA adapter) {
		_oid = oid;
		_adapter = adapter;
	}
	public void run() {
		try {
			Thread.currentThread().sleep(15000);
			System.out.println("dactivate with ID = " + new String(_oid));
			_adapter.deactivate_object(_oid); // 
		} catch(Exception exc) {
			exc.printStackTrace();
		}
	}
}
```
服务端程序为帐户管理员接口 `AccountManager` 提供了两个不同的对象实现，可找出很多理由说明，为什么在某些应用场合确实需要这种设计，例如可能是由于不同算法实现的时空效率不同，也可能是由于不同管理员开设帐户的初始余额限额权限不同。总之，我们假设 `AccountManager` 接口两个不同对象实现的Java类分别命名为 `AccountManagerImpl_1` 和 `AccountManagerImpl_2` ，如果对象标识为 `Zhang3` 则以 `AccountManagerImpl_1` 的对象实例为伺服对象，否则其他对象标识均以 `AccountManagerImpl_2` 的对象实例为伺服对象。

**当 `POA` 在活动对象映射表中找不到对象标识时，会调用伺服对象激活器的 `incarnate` 方法**，该方法根据不同的对象标识，创建不同类型的伺服对象。在返回伺服对象前启动一个线程，该线程等待 $15$ 秒后试图冻结刚才激活的对象，冻结对象前 `POA` 会自动调用伺服对象激活器的 `etherealize` 方法。==如果伺服对象激活器不启动冻结对象的线程，则「每次根据对象标识返回的伺服对象」都会记录在 `POA` 的活动对象映射表中，当对象标识再次出现时 `POA` 可在活动对象映射表中找到，因而不再调用伺服对象激活器的 `incarnate` 方法==。

程序5-4所示的服务程序，首先创建一个策略为 `USE_SERVANT_MANAGER` 和 `RETAIN` 的 `POA`（注意 `RETAIN` 是缺省值，所以不必显式给出），然后创建一个伺服对象激活器的实例并注册到 `POA`，接着调用 `create_reference_with_id` 方法创建两个对象引用，这时只是创建对象引用而没有激活任何对象。
```java
// 程序 5-4 使用伺服对象激活器的服务程序 Server.java
// 服务端的主程序
import org.omg.PortableServer.*;
public class Server {
	public static void main(String[] args) {
		try {
			// 初始化 ORB
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			// 取根 POA 的一个引用
			POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			// 创建应用程序所需的 POA 策略
			org.omg.CORBA.Policy[] policies = {
				rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
				rootPOA.create_request_processing_policy(
					RequestProcessingPolicyValue.USE_SERVANT_MANAGER)
			};
			// 用自定义策略创建新的 POA
			POA newPOA = rootPOA.create_POA(
				"ServantActivator`POA`", rootPOA.the_POAManager(), policies);
			// 创建伺服对象激活器并注册到 POA
			ServantActivator sa = new AccountManagerActivator()._this(orb);
			newPOA.set_servant_manager(sa);
			// 激活 POA 管理器
			rootPOA.the_POAManager().activate();
			// 创建第一个对象引用
			newPOA.create_reference_with_id("Zhang3".getBytes(), "IDL:Bank/AccountManager:1.0");
			// 创建第二个对象引用
			newPOA.create_reference_with_id("Li4".getBytes(), "IDL:Bank/AccountManager:1.0");
			// 等待接入请求
			System.out.println("帐户管理员 Zhang3 和 Li4 已就绪 ...");
			orb.run();
		} catch(Exception exc) {
			exc.printStackTrace();
		}
	}
}
```
当客户程序第一次以 `Zhang3` 作为对象标识绑定到对象引用时，`POA` 调用伺服对象激活器的 `incarnate` 方法，将返回一个 `AccountManagerImpl_1` 类型的伺服对象；如果再次以对象标识 `Zhang3` 获取对象引用，由于 `POA` 可在它的活动对象映射表中找到，因而不会调用 `incarnate` 方法；如果对象标识是 `Li4` 或其他字符串，则会再次调用 `incarnate` 方法，返回的伺服对象是 `AccountManagerImpl_2` 类型。该例子演示了如何利用伺服对象激活器管理服务端对象。

从运行的结果来看，==由于伺服对象激活器会在将对象激活 $15$ 秒后将其冻结，因此，在服务端内存中仅会保留「在最近 $15$ 秒内刚刚被用过的对象」==（最多两个，最少零个）。将服务端对象数量扩大，当服务端对象数量很多（典型的如大型网站的后台系统）时，本节例子给出的方案会变得非常有意义：保持所有的对象同时在内存中存在，会占用大量的系统内存（这往往导致系统运行效率急剧下降）。==采用类似的机制，可仅在内存中保留「那些正在被使用、或者刚刚被用过的对象」，而暂时不用的对象不会占用系统内存，从而有效提高了系统内存的利用率，同时伺服对象激活器又可以保证所有对象的可用性==（不在内存中的对象在使用时会被重新激活）。
## 5.4.2 伺服对象定位器
在许多应用场合，`POA` 的活动对象映射表可能变得很大、并占用大量内存，这时可考虑以 `USE_SERVANT_MANAGER` 结合 `NON_RETAIN` 策略创建 `POA`，这意味着**伺服对象与`CORBA` 对象的关联不保存在活动对象映射表中**。由于没有保存任何关联，每一次请求都导致 `POA` 直接以「对象标识、`POA` 等参数」，调用已注册的伺服对象定位器的 `preinvoke` 操作。伺服对象定位器负责查找合适的伺服对象返回给 `POA`，由 `POA` 调用伺服对象的合适操作，并将结果返回给客户程序后，再调用伺服对象定位器的 `postinvoke` 操作。

我们可以将程序5-3和5-4改写为使用伺服对象定位器的对应程序，程序5-5提供了一个伺服对象定位器，它必须实现从 `ServantLocatorPOA` 继承下来的抽象方法 `preinvoke` 和 `postinvoke` 方法。
```java
// 程序 5-5 伺服对象定位器示例 AccountManagerLocator.java
// 一个伺服对象定位器类型的伺服对象管理器
import org.omg.PortableServer.*;
import org.omg.PortableServer.ServantLocatorPackage.CookieHolder;
public class AccountManagerLocator extends ServantLocatorPOA { //
	public Servant preinvoke(byte[] oid, POA adapter,
		java.lang.String operation, CookieHolder the_cookie) throws ForwardRequest { //
		System.out.println("preinvoke with ID = " + new String(oid));
		if ((new String(oid)).equalsIgnoreCase("Zhang3"))
			return new AccountManagerImpl_1();
		return new AccountManagerImpl_2();
	}
	public void postinvoke(byte[] oid, POA adapter, java.lang.String operation,
		java.lang.Object the_cookie, Servant the_servant) { //
		System.out.println("postinvoke with ID = " + new String(oid));
	}
}
```
程序5-6与程序5-4的不同之处，在于它以 `NON_RETAIN` 策略创建 `newPOA`，并且**调用 `set_servant_manager` 方法注册到 `newPOA` 的是一个伺服对象定位器**，而不是伺服对象激活器。

建立相应客户程序并运行后，会发现**伺服对象定位器与伺服对象激活器的一个重要区别**，即——如果连续两次以对象标识 `Zhang3` 绑定对象引用、并调用该对象引用的操作，伺服对象激活器只调用一次 `incarnate` 方法，而伺服对象定位器则会调用两次 `preinvoke` 方法。当伺服对象处理请求完毕后，`POA` 还将调用伺服对象定位器的 `postinvoke` 方法。
```java
// 程序 5-6 使用伺服对象定位器的服务程序 Server.java
// 服务端的主程序
import org.omg.PortableServer.*;
public class Server {
	public static void main(String[] args) {
		try {
			// 初始化 ORB
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			// 取根 POA 的一个引用
			POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			// 创建应用程序所需的 POA 策略
			org.omg.CORBA.Policy[] policies = {
				rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
				rootPOA.create_servant_retention_policy( //
					ServantRetentionPolicyValue.NON_RETAIN
				), 
				rootPOA.create_request_processing_policy( //
					RequestProcessingPolicyValue.USE_SERVANT_MANAGER
				)
			};
			// 用自定义策略创建新的 POA
			POA newPOA = rootPOA.create_POA(
				"ServantLocatorPOA", rootPOA.the_POAManager(), policies);
			// 创建伺服对象定位器并注册到 `POA`
			ServantLocator sl = new AccountManagerLocator()._this(orb); //
			newPOA.set_servant_manager(sl); //
			// 激活 POA 管理器
			rootPOA.the_POAManager().activate();
			// 创建第一个对象引用
			newPOA.create_reference_with_id(
				"Zhang3".getBytes(), "IDL:Bank/AccountManager:1.0");
			// 创建第二个对象引用
			newPOA.create_reference_with_id(
				"Li4".getBytes(), "IDL:Bank/AccountManager:1.0");
			// 等待接入请求
			System.out.println("帐户管理员 Zhang3 和 Li4 已就绪 ...");
			orb.run();
		} catch(Exception exc) {
			exc.printStackTrace();
		}
	}
}
```

---
# 5.5 适配器激活器
**适配器激活器和伺服对象管理器一样，也是由程序员自己提供的代码**。并不是所有 `CORBA` 应用程序都需要适配器激活器，如果一个服务程序在启动时就创建了它所需的全部 `POA`，那么就无需使用或提供任何适配器激活器。**仅当需要在处理请求的过程中自动创建新的 `POA` 时，才需要用到适配器激活器**。

适配器激活器注册到一个 `POA` 后，**为该 `POA` 提供按需 `on-demand` 创建子 `POA` 的能力**。如果服务端程序以 `true` 作为第二个参数，调用该 `POA` 的 `find_POA` 操作，或客户程序利用对象引用发送请求、且对象引用中包含了该 `POA` 中不存在的子 `POA` 名字时，该 `POA` 会调用已注册适配器激活器的 `unknown_adapter` 操作。**从根 `POA` 到要创建的子 `POA` 之间的层次路径中，如果有 `POA` 不存在，则会依次调用 `unknown_adapter` 操作创建这些 `POA`** 。该操作成功创建子 `POA` 后应返回 `true` ，否则返回 `false` 。

例如，程序5-7重写了第三章例子程序的服务端主程序，它创建一个适配器激活器对象实例，并调用 `_this` 方法转换为对象引用，然后注册到根 `POA`。为演示客户程序对一个不存在的 `POA` 发出请求时、适配器激活器如何工作，服务程序按照用户自定义 `POA` 策略创建了一个 `POA` 层次：`/GrandPOA/ParentPOA/ChildPOA`，然后在 `ChildPOA` 中创建一个对象标识为 `"BankManager"` 的对象引用。最后服务程序撤销自建的 `POA` 层次，三个新建的子 `POA` 都被删除。这时除非根 `POA` 有按需激活子 `POA` 的功能，否则对 `ChildPOA` 的对象引用发送的请求将无法处理。

由于所需的 `POA` 不存在，客户程序无法利用生成的 `Helper` 类提供的 `bind` 方法获取服务程序输出的对象引用。为让客户程序能利用该对象引用发送请求，服务程序调用 `ORB` 伪对象的 `object_to_string` 方法，将对象引用转换为字符串后存放在一个文件中，客户程序从该文件读出字符串后，又调用 `ORB` 伪对象的 `string_to_object` 方法将字符串转换为一个对象引用。
```java
// 程序 5-7 服务程序 Server.java
// 演示适配器激活器用法的服务端主程序
import org.omg.PortableServer.*;
public class Server {
	public static void main(String[] args) {
		try {
			// 初始化 ORB
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			// 取根 POA 的引用
			POA rootPOA = POAHelper.narrow(
				orb.resolve_initial_references("RootPOA"));
			// 创建适配器激活器伺服对象并转换为对象引用，然后设置根 POA 的适配器激活器
			AdapterActivator bankAA = new BankAdapterActivator()._this(orb); //
			rootPOA.the_activator(bankAA); //
			// 自定义应用程序所需的持久对象 POA 策略并用于创建一个 POA 层次
			org.omg.CORBA.Policy[] policies = {
				rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
			};
			POA grandPOA = rootPOA.create_POA(
				"GrandPOA", rootPOA.the_POAManager(), policies);
			POA parentPOA = grandPOA.create_POA(
				"ParentPOA", rootPOA.the_POAManager(), policies);
			POA childPOA = parentPOA.create_POA(
				"ChildPOA", rootPOA.the_POAManager(), policies);
			// 创建一个对象引用并转换为字符串
			org.omg.CORBA.Object ior = childPOA.create_reference_with_id( //
				"BankManager".getBytes(), "IDL:Bank/AccountManager:1.0");
			String iorString = orb.object_to_string(ior); //
			// 将可互操作的对象引用（IOR）保存到文件中供客户程序使用
			try {
					java.io.PrintWriter writer = new java.io.PrintWriter(
						new java.io.FileWriter("BankIOR.dat"));
					writer.println(iorString);
					writer.close();
			} catch(java.io.IOException exc) {
				System.out.println("将 IOR 写入文件时出错：" + exc.getMessage());
				return;
			}
			// 撤销 POA 层次，包括所有的子 POA
			grandPOA.destroy(true, true); //
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
程序5-8给出了程序员编写的适配器激活器源代码，**该适配器激活器类必须实现从 `AdapterActivatorPOA` 类继承下来的抽象方法 `unknown_adapter`** 。`unknown_adapter` 方法**根据应用程序所需 `POA` 策略、创建新的子 `POA`** ，在程序5-8中实际上是恢复服务程序中被撤销的原有 `POA` 层次，并将 `POA` 策略修改为`USE_DEFAULT_SERVANT` ；然后在创建名为 `ChildPOA` 的子 `POA` 时，提供一个缺省伺服对象为所有请求服务。
```java
// 程序 5-8 适配器激活器 BankAdapterActivator.java
// 用户自定义的适配器激活器
import org.omg.PortableServer.*;
public class BankAdapterActivator extends AdapterActivatorPOA { //
	public boolean unknown_adapter(POA parent, String name) {
		System.out.println("auto-generate POA " + parent.the_name() + "/" + name);
		// 定义应用程序所需的 POA 策略
		org.omg.CORBA.Policy[] policies = {
			parent.create_lifespan_policy(LifespanPolicyValue.PERSISTENT),
			parent.create_request_processing_policy( //
				RequestProcessingPolicyValue.USE_DEFAULT_SERVANT
			)
		};
		// 创建新的 POA 作为 parent 的子 POA
		POA child = null;
		try {
			child = parent.create_POA(name, parent.the_POAManager(), policies);
		} catch (Exception exc) {
			exc.printStackTrace();
			return false;
		}
		// 设置新 POA 的适配器激活器
		child.the_activator(parent.the_activator()); //
		// 创建 childPOA 时准备好缺省伺服对象以及对象标识为 BankManager 的对象引用
		if (name.equals("ChildPOA")) {
			System.out.println("正在激活 ChildPOA 的缺省伺服对象 ...");
			try {
				child.set_servant(new AccountManagerImpl());
				child.create_reference_with_id(
					"BankManager".getBytes(), "IDL:Bank/AccountManager:1.0");
			} catch (Exception exc) { 
				exc.printStackTrace();
			}
		}
		return true;
	}
}
```
程序5-9所示的客户程序，首先从文件中取出「服务程序生成的对象引用的字符串」，然后调用 `ORB` 的 `string_to_object` 方法转换为对象引用，并利用 `Helper` 类提供的 `narrow` 方法窄化为 `AccountManager` 类型。这样得到的对象引用，与利用 `Helper` 类的 `bind` 方法获取的对象引用~~用法~~相同。

在运行客户程序时，我们会发现服务端的控制台显示 `unknown_adapter` 方法被调用了三次，分别用于创建 `GrandPOA, ParentPOA, ChildPOA` 。
```java
// 程序 5-9 客户程序 Client.java
// 演示适配器激活器用法的客户端的主程序
public class Client {
	public static void main(String[] args) {
		// 初始化 ORB
		org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
		// 从文件中取出服务程序写入的对象引用字符串
		String iorString = null;
		try {
			java.io.LineNumberReader reader = new java.io.LineNumberReader(
				new java.io.FileReader("BankIOR.dat"));
			iorString = reader.readLine();
			reader.close();
		} catch(java.io.IOException exc) {
			System.out.println("从文件读出 IOR 时出错：" + exc.getMessage());
			return;
		}
		// 将对象引用的字符串转换为对象引用
		Bank.AccountManager manager = Bank.AccountManagerHelper.narrow( //
			orb.string_to_object(iorString));
		// 请求帐户管理员开设帐户并取帐户的初始余额
		Bank.Account account = manager.open("David Zeng");
		System.out.println("帐户余额为" + account.getBalance() + "元");
	}
}
```
---
# 5.6 纽带机制
如之前所述，**编写对象实现有两种途径：一是使用继承机制，一是使用纽带机制** `tie mechanism` 。由于纽带机制不占用Java语言单继承的“配额”，所以特别适合将现有的Java应用程序集成到 `CORBA` 分布式对象系统中。
## 5.6.1 纽带机制工作原理
使用继承机制编写对象实现类时，通常继承由 `IDL` 编译器自动生成的 `POA` 类，这些 `POA` 类又继承 `org.omg.PortableServer.Servant` 类，并实现由 `IDL` 编译器自动生成的 `Operations` 接口。==如果对象实现类需要利用 `Java` 语言的单继承达到其他更重要的目的，则更适合选择纽带机制==。

**使用纽带机制时，对象实现不必继承生成的 `POA` 类，但必须实现生成的 `Operations` 接口**。更要注意的是，**服务程序创建的伺服对象不再是对象实现类型**（这时对象实现类不必是 `org.omg.PortableServer.Servant` 的派生类），**而是由 `IDL` 编译器生成的 `POATie` 类型的实例**。

创建 `POATie` 的对象实例时，必须指定一个对象实现类的实例作为代表对象（`delegate`），`POATie` 类并没有提供什么新的语义，而仅仅是简单地将「它收到的每一个操作请求」委托给它的代表对象，由代表对象执行真正的操作。
## 5.6.2 改写例子程序
本小节采用纽带机制，重新实现[第三章例子程序](https://memcpy0.blog.csdn.net/article/details/122645195)的服务端。对比两种实现方式的源程序可看出，程序5-10所示的 `Account` 接口的对象实现改动很少，关键是**采用纽带机制的对象实现不再继承生成的 `POA` 类，而是直接实现生成的 `Operations` 接口**。
```java
// 程序 5-10 采用纽带机制的对象实现 AccountImpl.java
// 使用纽带机制的帐户对象实现
public class AccountImpl implements Bank.AccountOperations { //
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
程序5-11重写了 `AccountManager` 接口的对象实现，它的主要区别也是**对象实现类 `AccountManagerImpl` 不再继承 `AccountManagerPOA` 类**，因而可利用继承去重用其他更重要的类。该对象实现之所以要将一个 `POA` 作为构造方法的参数、并保存为对象实例的状态，是因为**在 `open` 方法中需利用该 `POA` 隐式地激活一个 `Account` 对象的引用**，这是由于对象实现不再继承 `POA` 类后，不可在它的方法中调用 `default_POA` 方法。应注意，**对象引用 `account` 相关联的伺服对象是 `POATie` 类型、而不是 `AccountImpl` 类型的实例，由 `POATie` 类型的实例再关联到一个 `AccountImpl` 类型的实例**。
```java
// 程序 5-11 采用纽带机制的对象实现 AccountManagerImpl.java
// 使用纽带机制的帐户管理员对象实现
import java.util.*;
import org.omg.PortableServer.*;
import Bank.*;
public class AccountManagerImpl implements AccountManagerOperations { //
	// 属性的定义
	protected POA accountPOA;
	// 用于隐式激活对象的对象适配器
	protected Hashtable accountList;
	// 该帐户管理员所负责的帐户清单
	// 构造方法，管理员开始时管理的帐户清单为空
	public AccountManagerImpl(POA POA) {
		accountPOA = POA;
		accountList = new Hashtable();
	}
	// 查找指定名字的帐户，找不到则以该名字新开一个帐户
	public synchronized Account open(String name) {
		// 在帐户清单中查找指定名字的帐户
		Account account = (Account) accountList.get(name);
		// 如果不存在则新创建一个
		if (account == null) {
			// 随机虚构帐户的初始余额，金额在 0 至 1000 元之间
			Random random = new Random();
			float balance = Math.abs(random.nextInt()) % 100000 / 100f;
			// 创建委派给一个 AccountImpl 对象实例的帐户纽带
			AccountPOATie tie = new AccountPOATie(new AccountImpl(balance));
			try {
				account = AccountHelper.narrow(accountPOA.servant_to_reference(tie));
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

// 程序 5-12 采用纽带机制的 Server.java
// 服务端的主程序
import org.omg.PortableServer.*;
public class Server {
	public static void main(String[] args) {
		try {
			// 初始化 ORB
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			// 取根 POA 的引用
			POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			// 以持久对象的 POA 策略创建子 POA
			org.omg.CORBA.Policy[] policies = {
				rootPOA.create_lifespan_policy(LifespanPolicyValue.PERSISTENT)
			};
			POA newPOA = rootPOA.create_POA("BankTiePOA",
				rootPOA.the_POAManager(), policies);
			// 创建委派给一个 AccountManagerImpl 对象实例的纽带
			Bank.AccountManagerPOATie tie =
				new Bank.AccountManagerPOATie(new AccountManagerImpl(rootPOA));
			// 用对象标识"BankManager"激活对象
			newPOA.activate_object_with_id("BankManager".getBytes(), tie);
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
程序5-12重写了服务端的主程序。与程序5-11类似，**服务程序创建对象引用时使用的伺服对象类型必须是 `POATie`** ，在创建 `POATie` 类型的实例时，**必须创建一个对象实现的实例，并指定为该纽带的代理对象**。

第三章例子程序的客户端程序无需修改，因为服务端对象实现采用继承方式、还是纽带方式，对客户程序而言是透明的，客户程序仍采用同样的方式访问「对象实现提供的服务」。

---
# 5.7 `POA` vs `BOA`
## 5.7.1 基本对象适配器
早期版本的 `CORBA` 规范定义了基本对象适配器 `BOA` 作为 `ORB` 产品提供的首选对象适配器。但是，`BOA` 直接调用操作系统的有关机制激活对象实现、并与之交互，因而 **`BOA` 依赖于特定的操作系统和编程语言**，在不同 `ORB` 产品上基于 `BOA` 开发的对象实现可移植性较差。在1998年修订的 `CORBA 2.2` 版中引入了 `POA`，`POA` 一词中的“可移植”主要针对 `BOA` 而言。

所有对象适配器的主要作用都是**创建 `CORBA` 对象，并将 `CORBA` 对象与「真正执行服务的程序设计语言对象（即伺服对象）」相关联**。`POA` 围绕这一目标提供了比 `BOA` 更完善的机制，从而取代 `BOA` 、成为 `ORB` 利用对象实现实例为请求服务的主要途径。应注意，**对象适配器只是服务端的概念，不会对客户程序产生影响**。一个 `CORBA` 兼容的客户程序，如果能够使用基于 `BOA` 的服务程序，那么它也应能够使用基于 `POA` 的服务程序。
## 5.7.2 `POA` 对 `BOA` 的改进
`POA` 对 `BOA` 的改进主要体现在以下几个方面：
- 由于 `CORBA` 关于 `BOA` 的规范不够充分，许多 `ORB` 产品供应商为提高 `BOA` 的可用性，纷纷对 `BOA` 进行了不同的扩展。而 `POA` 则通过更完善的规范改进了 `BOA`，**`POA` 本身利用 `IDL` 定义，比 `BOA` 更强调如何处理复杂问题**。
- `BOA` 不足以处理大规模系统，而 **`POA` 为实现大规模系统提供了更多的功能**。服务程序可通过以策略驱动的自动方式，处理多个客户程序的连接与请求，并且可充分利用 `POA` 管理生存期、以及 `CORBA` 对象与伺服对象之间关联的能力，管理应用程序中的大量对象。
- `BOA` 并不是完全与位置无关的，当客户程序与实现对象在同一进程或不同进程（即远程访问）时，`BOA` 的语义会发生变化，而 **`POA` 更能保持这两种情况的语义一致性**。实际上，**对本地对象的 `CORBA` 调用仍会通过 `POA` 进行**，这使得 `POA` 可统一地应用策略和服务决策。

---
# 思考与练习
5-1 简述在 `CORBA` 的 `POA` 体系结构下，客户端发出的请求如何一步步传递到最终提供服务的伺服对象。指明在上述请求传递的过程中，`CORBA` 规范为了支持灵活的服务端模型，允许程序员在哪些环节添加自己的干预，以及可以添加怎样的干预？

5-2* 本章伺服对象例子给出的方案在用于实际系统时，有一个明显的缺陷，那就是在冻结对象时并没有将对象的状态保存，因此再次激活后，账户管理员对象所开设的账户信息已经丢失，如何进一步改进此方案，使得能够保存并恢复对象的状态？

5-3 运行程序5-9所示客户程序时，服务器控制台会显示调用了三次适配器激活器的程序员自定义方法。还发现，如果不关闭服务程序再次运行客户程序时，服务器控制台却没有显示适配器激活器的方法被 `POA` 调用。为什么会这样？


