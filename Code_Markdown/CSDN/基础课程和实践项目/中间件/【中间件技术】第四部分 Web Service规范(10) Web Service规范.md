@[toc]

---
# 10.1 `Web Service` 体系结构
## 1. `Web Service` 的提出
`Web Service`（Web服务）是近几年兴起的一种**新的分布式计算模型**，下面从Web的发展过程，简单讨论 `Web Service` 的提出过程。

在Web早期阶段，主要是利用传统的Web服务器向Web访问者提供静态内容，此时的技术基础主要包括：Web服务器、HTTP协议、HTML语言等，由于此阶段Web上可访问的内容以静态的文档内容为主，因此该阶段也称为**文档式Web阶段**。

随着Internet的发展与普及，人们开发并利用应用服务器在Web上提供动态内容，支持访问者在Web上执行商业逻辑与网上交易，此阶段的技术基础包括：应用服务器、动态网页技术、分布处理等，由于此阶段的Web上可以完成交互式的应用功能，因此该阶段也称为**应用式Web阶段**。

从 `Web Service` 角度看，Web发展的第三阶段称为**服务式Web阶段**，该阶段的**主要变化是引入了 `Web Service`** ，其基本思想是==将Web应用、动态内容以及交易功能包装成程序可以访问的服务，即为Web提供程序可以访问的界面==，图10-1所示的、Google网站上的一幅图片，很形象的刻画了 `Web Service` 带来的变化：**Web的访问者不再局限于人，程序（软件）也可以充分利用Web上的丰富资源**。
![图10-1 Web Service为Web带来的变化](https://img-blog.csdnimg.cn/1b26b069d94949db9d0e0c0fa109e7d1.png)
`Web Service` 是一系列标准的集合，包括 `SOAP, UDDI, WSDL, WSFL/BPEL` 等，**`Web Service` 利用这些标准，提供了一个松散耦合的分布式计算环境**。在 `Web Service` 模型中，厂商将其服务封装成一个个相对独立的 `Web Service`，每个服务提供某类功能；客户（或其它厂商）可以通过 `SOAP` 协议来访问这些服务。`Web Service` 的主要特征是**将可调用的功能发布到Web上以供程序访问**。

## 2. `SOA` 架构
`Web Service` 使用**面向服务架构** `Service Oriented Architecture,  SOA` 。如图10-2所示，**`SOA` 架构中包含三个主要参与者**：服务提供者 `Service Provider` 、服务请求者 `Service Requester` 与服务代理 `Service Broker` 。**涉及发布 `Publish` 、查找 `Find` 与绑定/调用 `Bind/Invoke` 三个基本操作**。基本的工作过程为：
![图10-2 SOA架构](https://img-blog.csdnimg.cn/547a85c7e7354e5789b95252acbd7571.png)
1. 服务提供者将所提供的服务，发布到「服务代理的一个目录」上（即 `Publish` 操作）；
2. 服务请求者首先到「服务代理提供的目录」上搜索服务，得到如何调用该服务的信息（即 `Find` 操作）；
3. 根据得到的信息，服务请求中调用「服务提供者提供的服务」（即`Bind/Invoke` 操作）。

在 `Web Service` 架构中，使用的主要标准如下：
- **SOAP**：简单对象访问协议 `Simple Object Access Protocol` ，用来执行Web服务的调用。
- **WSDL**：Web服务描述语言 `Web Service Description Language` ，用来描述Web服务。
- **UDDI**：`Universal Description, Discovery and Integration` ，用来发布、查找服务。
- **WSFL/BPEL**： `Web Service Flow Language/Business Process Execution Language` ，用于将分散的、功能单一的Web服务，组织成一个复杂的有机应用。

Web服务通常具有以下特征：
- **完好的封装性**：使用者仅看到 `Web Service` 提供的功能列表，不能看到其实现细节；
- **松散耦合**：只要服务的接口保持不变，就不影响使用者，而且客户端和服务端的生命周期不相互影响，同时需要一种适合Internet环境的消息交换协议。
- **使用标准协议规范**：`Web Service` 架构使用开放的标准协议进行描述、传输和交换。
- **高度可互操作性**：可以跨越平台、语言进行调用。
- **高度可集成能力**：高度可互操作性所带来的高度可集成能力，使得 `Web Service` 可用于集成异构的软件系统。
- **动态性**：可以自动发现服务并进行调用。
## 3. `Web Service` 分类
Web服务的主要应用领域可分为以下四类：
1. **面向商业应用的 `Web Service`**（`Business-Oriented Web Service`）：将企业内部的大型系统如ERP、CRM系统等，封装成 `Web Service` 的形式在网络中（`Internet or Intranet` ）提供，基于 `Web Service` 的高度可互操作性，==这使得企业内部的应用更容易集成、企业间的众多合作伙伴的系统对接更加容易==。这是 `Web Service`目前最主要的应用方向之一。
2. **面向最终用户的 `Web Service`**（`Customer-Oriented Web Service`）：指 `Web Service` 在电子商务领域的应用，主要指针对原有B2C网站的改造，`Web Service` 技术为B2C网站增加了 `Web Service` 的应用界面，==使得桌面工具可以提供跨越多个B2C服务的桌面服务==，如将机票预定、炒股等服务集成到一个个人理财桌面系统中，这令用户更加方便地使用Internet，能够获得更加便捷的服务。
3. **面向特定设备的 `Web Service`**（`Device-Oriented Web Service`）：指面向手持设备、日用家电等特定接入设备的 `Web Service`，将原有的如天气预报、E-mail服务、股票信息等网络服务封装成 `Web Service`，支持除PC以外的各种终端。
4. **系统级 `Web Service`**（`System-Oriented Web Service`）：这类 `Web Service` 相当于之前讨论的公共服务，指将Web应用中诸如用户权限认证、系统监控等通用功能封装成 `Web Service`，发布到Internet或者企业内部的Intranet上，==其作用范围将从单个系统或局部网络、拓展到整个企业网络或整个Internet上==。比如，一个跨国企业的所有在线服务，可以使用同一个用户权限认证服务。
## 4. `Web Service` 协议栈
W3C在2001年的 `Web Service` 专题研讨会上提出了 **`Web Service` 协议栈**，该协议栈定义了 `Web Service` 使用的技术标准与发展方向，如图10-3所示。
![图10-3 Web Service 协议栈](https://img-blog.csdnimg.cn/af3166f13de749579c4ac5ddc31fc9bc.png)
**`Web Service` 追求的第一目标是简单性**：
- 首先 `Web Service` 使用的协议本身是简单的；
- 另外，一个可以使用的 `Web Service` 可以按照需要选择若干层次的功能，而无需所有的特性。例如，一个简单应用可能只要使用 `WSDL/SOAP` 就可以架构一个符合规范的 `Web Service`；
- 最后，`Web Service` 所有的机制都基于现有的技术，并没有创造一个完整的新体系，而是继承「原有的被广泛接受的技术」。

`Web Service` 目前最主要的应用方向为**集成企业应用系统**。集成企业原有系统利用 `Web Service` 的高度可集成特性，==将企业运作的各个环节有效的联系起来，组成一个协同工作的整体，从而使得企业的所有业务都真正"自动化"起来==。

`Web Service` 目前的主要问题包括：效率问题与安全性、事务控制等高级特性比较欠缺等。

---
# 10.2 `SOAP`
**`SOAP` 是在松散的、分布的环境中，使用 `XML` 交换「结构化的和类型化的信息」的一种简单协议**。`SOAP` 本身并不定义任何应用语义，如编程模型或特定语义实现，它只定义了一种「简单的、以模块化的方式」包装数据的机制。
## 1. `SOAP` 消息结构
**`SOAP` 消息是 `Web Service` 架构中不同结点间传送信息的基本数据单元**，其结构如图10-3所示。一个 `SOAP` 消息是由 `XML` 消息头和一个 `SOAP` 封装（`SOAP Envelope`）组成的 **`XML` 文档**。`SOAP` 封装描述了该消息所包含的基本信息，其中包括一个可选的 `SOAP` 消息头（`SOAP Header`）和必需的 `SOAP` 消息体（`SOAP Body`）。`SOAP` 封装定义了一个 `SOAP`错误（`SOAP Fault`）来报告出错信息。
![图10-4 SOAP消息结构](https://img-blog.csdnimg.cn/60b94fd898fc440493f4e6b12927ad79.png)
### (1) `SOAP Envelope`
**`SOAP` 封装是 `SOAP` 消息文件的顶层元素，其必须存在且元素名称为 `<env:Envelope>`** 。==该元素可以包含命名空间和声明的额外属性，其额外属性必须用命名空间修饰==。`SOAP` 封装中除了 `SOAP` 消息头和 `SOAP` 消息体外，也可以包含其它的子元素。若存在其它子元素，则必须出现在 `SOAP` 消息体之后。

**`SOAP` 消息中必须使用 `SOAP` 封装的命名空间**。其语法格式如：
```xml
xmlns:env="http://schemas.xmlsoap.org/soap/envelope/"
```
**`SOAP` 编码方式（`SOAP EncodingStyle`）是 `SOAP` 封装的一个属性，用于定义在文档中使用的数据类型**。该属性可以出现在任何 `SOAP` 元素中，并会被应用到元素的内容和元素的子元素上。其语法格式如：
```xml
env:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"
```

### (2) `SOAP Header`
如果**存在 `SOAP` 消息头时，它必须作为 `SOAP` 封装的第一个子元素，且元素名称为 `<env:Header>`** 。==消息头主要传递一些辅助性的附加消息，如身份验证、会话等==。在 `SOAP` 消息头中，==可以通过添加基于 `XML` 格式的消息条目，实现对消息进行扩展，典型的应用有认证机制、事务管理和支付等==。

`SOAP` 消息头的所有子元素被称为 `Header` 条目，每个 `Header` 条目是由一个命名空间的 `URL` 和局部名组成的，`SOAP` 规范中不允许出现「不带命名空间修饰的 `Header` 条目」。`Header` 条目包含了 `Role` 、`mustUnderstand` 和 `encodingStyle` 属性：
- `SOAP Role` 属性可用于将 `Header` 元素寻址到一个特定的端点。其语法格式如：
	```xml
	env:role="http://schemas.xmlsoap.org/soap/actor/"
	```
- `SOAP mustUnderstand` 属性可用来标识——消息头对于「要对其进行处理的接收者」来说是强制的还是可选的。其语法格式如：
	```xml
	env:mustUnderstand="true"
	```
	上例表示该消息的接收者必须认可此元素，否则消息头处理失效。
- `SOAP encodingStyle` 属性用于指定 `Header` 条目的编码方式。
### (3) `SOAP Body`
**`SOAP` 消息体为「消息的最终接受者所必须处理的信息」提供了一个容器，它包含了所交换数据的实际内容，其元素名称为 `<env:Body>`** 。`SOAP` 消息体是 `SOAP` 封装必需的元素，**如果有 `SOAP` 消息头，则消息体必须紧跟消息头之后**，否则其为 `SOAP` 封装的第一个直接子元素。

`SOAP` 消息体的所有子元素被称为 `Body` 条目，每个 `Body` 条目是由一个命名空间的 `URL` 和局部名组成的，每个 `Body` 条目被视作「 `SOAP` 消息体元素的独立子元素」进行编码。`SOAP encodingStyle` 属性用于指定 `Body` 条目的编码方式。

`SOAP` 消息头和消息体虽然是作为独立的元素定义的，但二者在实际上有一定的关联：**当 `Header` 条目的 `mustUnderstand` 属性值为 `true` ，而且该消息传递给默认的接收者时，语义上和 `Body` 条目是等价的**。

### (4) `SOAP Fault`
**`SOAP` 错误用于在 `SOAP` 消息中携带错误和/或状态信息，其作为一个 `Body` 条目出现、且不能出现多于一次，元素名称为 `<SOAP:Fault>`** 。`SOAP` 错误定义了四个子元素：
- `<faultcode>` ：标识了引发错误的故障代码；
- `<faultstring>` ：提供可供人进行阅读的故障说明；
- `<faultactor>` ：消息传送过程中引发故障的结点信息；
- `<faultdetail>` ：传输与 `SOAP` 消息体元素相关的、应用程序的错误信息。

`SOAP` 错误代码及其含义：
- `VersionMismatch` ：`SOAP` 封装包含的无效命名空间；
- `MustUnderstand` ：`SOAP` 消息头中被标识为 $1$ 的 `Header` 条目未被认可；
- `DataEncodingUnknown` ：编码方式不正确；
- `Client` ：消息中包含了不正确的结构或信息；
- `Server` ：消息接收者没有对消息进行正确处理。

```xml
<!- 程序 10-1 SOAP 消息示例 -->
<?xml version="1.0" ?>
<env:Envelope xmlns:env="http://www.w3.org/soap-envelope">
	<env:Header>
		<data:headerBlock
		xmlns:data="http://example.com/header"
		env:role="http://example.com/role"
		env:mustUnderstand="true">
		...
		</data:headerBlock>
		...
	</env:Header>
	<env:Body>
		<data:bodyBlock xmlns:data="http://example.com/header">
		...
		</data:bodyBlock>
		...
	</env:Body>
</env:Envelope>
```
## 2. `SOAP` 调用过程
![图10-5 SOAP消息调用过程](https://img-blog.csdnimg.cn/694870363d1e42fc805698de60c46618.png)
`SOAP` 的基本思想是，**将数据/对象打包成 `XML` 格式的数据在网络环境中交换、以达到服务调用的目的**。如图10-5所示，==发送者和接收者之间可以交换任何类型的消息数据：发送者利用 `XML` 编码，将消息打包为 `SOAP` 消息，通过底层的网络传输协议进行传输；接收者收到消息，并将消息进行解码，获取所需要的数据==。比如，可以用程序10-2中给出的 `XML` 文档来封装「程序10-3中定义的类的一个实例」：
```xml
<!-- 程序 10-2 封装对象的 XML 文档 -->
<?xml version="1.0" ?>
<env:Envelope xmlns:env="http://www.w3.org/soap-envelope">
	<env:Body xmlns:m="http://www.example.org/namelist">
		<m:Person>
			<m:name>zhang3</m:name>
			<m:age>20</m:age>
		</m:Person>
	</env:Body>
</env:Envelope>
```
```java
// 程序 10-3 类 Person 的定义
public class Person{
	String name;
	int age;
	// 方法的定义...
}
```
`SOAP` 没有定义任何底层的传输协议，可以使用HTTP、FTP、SMTP或者 `JMS` ，甚至是自定义协议来传输 `SOAP` 报文，在Internet环境下一般使用HTTP协议。从某种意义上讲，**`SOAP` 可以简单理解为"HTTP+XML+RPC"**，==即使用HTTP作为底层通信协议，XML作为数据传输的格式，实现Web环境下的远程服务调用（RPC）==。

下面通过一个简单实例，演示 `SOAP` 协议的基本思想。假设在Internet上有「程序10-4中给出的接口所约定的服务」存在，客户端在远程调用 `sayHelloTo` 方法时，提供一个名字，期望返回一个字符串。
```java
// 程序 10-4 服务的接口定义
public interface Hello {
	public String sayHelloTo(String name);
}
```
假设没有 `Stub/Skeleton` 机制的支持，则开发者必须负责将方法调用串行化，并把消息发给远程服务器。一种简单的方式就是使用 `XML` 打包数据，我们可以很快得到程序10-5的XML文档来表示客户端的调用请求：
```xml
<!-- 程序 10-5 XML 表示的请求信息 -->
<?xml version="1.0" ?>
<env:Envelope xmlns:env="http://www.w3.org/soap-envelope">
	<env:Body xmlns:m="http://www.example.org/namelist>
		<m:Hello>
			<m:sayHelloTo>
				<m:name>John</m:name>
			</m:sayHelloTo>
		</m:Hello>
	</env:Body>
</env:Envelope>
```
在程序10-5给出的XML文档中，我们**将接口名作为根结点，方法作为子结点**，而**方法调用的参数作为方法的子结点**。如果在其调用的过程中，服务端发生了错误，则会引发如程序10-6所示的 `SOAP` 错误消息。

**`SOAP` 消息作为 `Web Service` 规范的标准信息交换格式，解决了网络信息交换中"语言"不一致的问题，受到工业界的认可、并在实践中加以应用**。但 `SOAP` 消息的调用效率比较低，HTTP不是有效率的通信协议，并且XML需要额外的文件解析。一些格式更加简化的消息风格如**代表性状态转移** `REpresentational State Transfer, REST`  等得到了新的发展和应用。
```xml
<!-- 程序 10-6 SOAP 错误消息 -->
<?xml version="1.0" ?>
<env:Envelope xmlns:env="http://www.w3.org/soap-envelope">
	<env:Body>
		<env:Fault>
			<faultcode>SOAP-ENV:Server</faultcode>
			<faultstring>Server Error</faultstring>
			<detail>
				<myfaultdetails>
					<message>
						Sorry, my silly constraint says that I cannot say hello on Tuesday.
					</message>
					<errorcode>1001</errorcode>
				</myfaultdetails>
			</detail>
		</env:Fault>
	</env:Body>
</env:Envelope>
```
---
# 10.3 `WSDL`
**`WSDL` 是一种用XML文档来描述Web服务的标准，是Web服务的接口定义语言**。其内容包含了Web服务的基本属性信息，如Web服务所提供的操作和Web服务进行消息交换的数据格式和协议标准、Web服务的网络位置等信息。
## 1. `WSDL` 数据结构
`WSDL` 以**端口集合**的形式来描述Web服务，`WSDL` 服务描述包含==对一组操作和消息的一个抽象定义，绑定到这些操作和消息的一个具体协议，和这个绑定的一个网络端点规范==。`WSDL` 文档分为两种类型：服务接口 `Service interface` 和服务实现 `Service implementations` ，文档基本元素结构如图10-6所示。
![图10-6 WSDL元素结构图](https://img-blog.csdnimg.cn/a589221978de4886975bf227055b5be0.png)
`WSDL` 文档在Web服务的定义中，使用了以下元素：
- `Types` ：它是一个数据类型定义的容器，使用某种类型系统，**包含了所有在消息定义中需要的XML元素的类型定义**。
- `Message` ：它使用 `Types` 所定义的类型，**具体定义了「在通信中使用的消息」的数据结构**，其中包含了一组 `Part` 元素，每个 `Part` 元素都是最终消息的一个组成部分，每个 `Part` 都会引用一个 `DataType` 来表示它的结构。`Part` 元素不支持嵌套，都是并列出现。
- `Operation` ：**对服务所支持操作的抽象描述**，单个 `Operation` 描述了访问入口的一个请求/响应消息对，是访问入口支持的一种类型的调用。==`WSDL` 中支持四种访问入口调用的模式（单请求、单响应、请求/响应、响应/请求），其中请求是从客户端到Web服务端的消息调用，响应则是从Web服务端到客户端的消息传递==。
- `PortType` ：**它具体定义了一种服务访问入口的类型，即传入/传出消息的模式及其格式**。一个 `PortType` 可以包含若干个 `Operation` ，因此也是某个访问入口点类型所支持的操作的抽象集合，这个操作可以由一个或多个服务访问点来支持。
-  `Binding` ：`Binding` 结构**定义了「特定 `PortType` 的具体协议」和「数据格式规范」的绑定**，即定义了某个 `PortType` 与某一种具体的网络传输协议或消息传输协议相绑定，其描述的内容与具体的服务部署相关。可以在 `Binding` 中将 `PortType` 与 `SOAP/HTTP` 绑定，也可以将其与 `MIME/SMTP` 相绑定等。
- `Port` ：定义为「协议/数据格式绑定」与「具体Web服务地址」组合的单个访问点。**它描述的是一个服务访问入口的部署细节**，包括所要访问的 `URL` 、及应当使用的消息调用模式等。其消息调用模式使用 `Binding` 结构来表示。
- `Service` ：它描述一个「具体的被部署的Web服务」所提供的所有访问入口的部署细节，是**相关服务访问点的集合**。一个 `Service` 通常包含多个服务访问入口，而每个访问入口都会使用一个 `Port` 元素来描述。
![图10-7 WSDL元素的对象结构示意图](https://img-blog.csdnimg.cn/6a2cb34dbe45491ba83a757146949331.png)`Types, Message, PortType` 三种结构，描述了所调用Web服务的抽象定义，其相对类似于 `CORBA` 中的 `IDL` 描述。
 
`WSDL` 的设计遵循了Web技术标准的开放理念，允许通过扩展使用如 `XML Schema` 等其他的类型定义语言，允许使用如 `SOAP/HTTP` 等多种网络传输协议和消息格式。**`WSDL` 中应用了软件工程中的复用理念，分离了抽象定义层和具体的部署，使得前者的复用性大大增加**。
## 2. `WSDL` 使用示例
通过一个简单的股票报价Web服务的 `WSDL` 定义文档，说明 `WSDL` 文档的定义和使用。该服务支持名称为 `GetLastTradePrice` 的唯一操作，通过HTTP协议在 `SOAP 1.1` 上来实现。该请求接受一个类型为字符串的 `tickerSymbol` ，并返回类型为浮点数的价格。
```xml
<!-- 程序 10-7 WSDL 定义声明 -->
<?xml version="1.0" ?>
<definitions name="StockQuote"
targetNamespace="http://example.com/stockquote.wsdl"
xmlns:tns="http://example.com/stockquote.wsdl"
xmlns:xsd1="http://example.com/stockquote.xsd"
xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/"
xmlns="http://schemas.xmlsoap.org/wsdl/">
```
程序10-7是股票报价服务 `WSDL` 的**定义声明部分**。包括**XML版本头**和 **`WSDL` 定义声明**，后者通过属性分别声明了目标命名空间、服务的主命名空间、`XML Schema` 命名空间、`SOAP` 绑定采用的命名空间和XML的命名空间。

```xml
<!-- 程序 10-8 WSDL 数据类型定义 -->
<types>
	<schema targetNamespace="http://example.com/stockquote.xsd" xmlns="http://www.w3.org/1999/XMLSchema">
		<element name="TradePriceRequest">
			<complexType>
				<all>
					<element name="tickerSymbol" type="string"/>
				</all>
			</complexType>
		</element>
		<element name="TradePriceResult">
			<complexType>
				<all>
					<element name="price" type="float"/>
				</all>
			</complexType>
		</element>
	</schema>
</types>
```
程序10-8是股票报价服务 `WSDL` 的**数据类型定义部分**。定义了两种元素的结构
 `TradePriceRequest` 和 `TradePriceResult` ，分别由字符串和浮点型的复合数据类型元素构成。
```xml
<!-- 程序 10-9 WSDL 消息格式抽象定义 -->
<message name="GetLastTradePriceInput">
	<part name="body" element="xsd1:TradePriceRequest"/>
</message>
<message name="GetLastTradePriceOutput">
	<part name="body" element="xsd1:TradePriceResult"/>
</message>
```
程序10-9是**消息格式的抽象定义部分**。定义了两种消息格式 `GetLastTradePriceInput` 和 `GetLastTradePriceOutput` ，由名称为 `body` 的 `part` 组成，分别包含了程序10-8所定义的两种元素类型。
```xml
<!-- 程序 10-10 WSDL 服务访问点调用模式 -->
<portType name="StockQuotePortType">
	<operation name="GetLastTradePrice">
		<input message="tns:GetLastTradePriceInput"/>
		<output message="tns:GetLastTradePriceOutput"/>
	</operation>
</portType>
```
程序10-10是**服务访问点的调用模式定义**部分。表明了股票报价Web服务的入口类型是请求/响应模式，其输入/输出消息为程序10-9所定义的消息格式。
```xml
<!-- 程序 10-11 WSDL 绑定模式定义 -->
<binding name="StockQuoteSOAPBinding" type="tns:StockQuotePortType">
	<soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
		<operation name="GetLastTradePrice">
			<soap:operation soapAction="http://example.com/GetLastTradePrice"/>
				<input>
					<soap:body use="literal" namespace="http://example.com/stockquote.xsd"
						encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
				</input>
				<output>
					<soap:body use="literal" namespace="http://example.com/stockquote.xsd"
					encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"/>
				</output>
			</soap:operation>
		</operation>
	</soap:binding>
</binding>
```
程序10-11是**服务访问点的抽象定义和 `SOAP HTTP` 的绑定定义部分**。表明了「股票报价服务的消息传输」采用基于 `SOAP` 绑定的HTTP传输，规定了在 `SOAP` 调用时应当使用的 `soapAction` 和消息的编码规则（采用 `SOAP` 规范默认的编码风格）。
```xml
<!-- 程序 10-12 WSDL 服务定义 -->
<service name="StockQuoteService">
	<documentation>股票查询服务</documentation>
	<port name="StockQuotePort" binding="tns:StockQuoteBinding">
		<soap:address location="http://example.com/stockquote"/>
	</port>	
</service>

</definitions>
```
程序10-12是**具体的股票报价服务的服务定义部分**。定义了服务名称和提供的服务访问入口，使用的消息模式是由程序10-11的 `Binding` 所定义。

将程序10-7至10-12合并起来，组成了针对股票查询Web服务的完整的 `WSDL` 定义文档。==在Web服务的使用过程中，请求结点通过分析 `WSDL` 文档的描述，即可了解到Web服务的接口定义、部署及调用信息，利用 `SOAP` 请求消息访问Web服务==。

---
# 10.4 `UDDI`
**`UDDI` 是一套基于Web的、分布式的、并为Web服务提供注册的信息注册中心的实现规范**，同时也包含了一组「使企业能将自身提供的Web服务进行注册、以使其它企业能够发现」的访问协议实现标准。`UDDI` 标准包含一个独立于平台的框架，这一框架用于通过使用Internet来描述服务、发现企业、并对企业服务进行集成。==通过使用 `UDDI`，Web服务提供者能够注册Web服务，应用程序或服务使用者能够查找到所需要的Web服务，并了解如何与之连接和交互==。
## 1. `UDDI` 数据模型
**`UDDI` 商业注册使用一个XML文档来描述企业及其提供的Web服务**。其做法类似于电信公司发布电话号码所使用的黄页，客户可以很方便的从上面查找到需要的信息。从概 念上来说，`UDDI` 商业注册所提供的信息包括以下三部分内容：
- 白页 `White Page` ：**有关企业的基本信息**，包括了企业名称、经营范围的描述、联系信息和已知的企业标识等；
- 黄页 `Yellow Page` ：**提供基于标准分类法的行业类别**，能够使使用者在更大的范围内，查找在注册中心注册的企业或服务；
- 绿页 `Green Page` ：**关于「该企业所提供的Web服务」的技术信息和Web服务说明信息**，并提供了指向「这些服务所在实现的技术规范」的引用和一些指针，指向基于文件的URL的不同发现机制（？）。

**`UDDI` 注册中心是由 `UDDI` 规范的一种或多种实现组成，它们可以互操作以共享注册中心数据**。`UDDI` 商业注册中心是由「被称为节点的一组 `UDDI` 注册中心」构成，能够对外公开访问，通过互操作共享注册中心数据。在很多 `UDDI` 运营商（如 IBM、Microsoft、HP和SAP等）站点上，都冗余的放着 `UDDI` 商业注册中心的全部条目，只有该站点才能够对这些条目进行更改。

`UDDI` 的核心数据模型、及其属性和子元素结构如图10-8所示。主要包括：
- `businessEntity` ：**业务详细资料**，包括名称和联系信息等。`UDDI` 商业注册的商业信息发布和发现的核心XML元素都包含在该结构中。这个结构是行业试题专属信息集的最高层的数据容器，位于整个信息结构的最上层。
- `businessService` ：**业务提供的Web服务**。该元素将「一系列有关商业流程或分类目录的Web服务描述」组合到一起，这些服务都是发布服务的商业实体所需要注册的Web服务。其中包含的信息可以根据行业、产品、服务类型或地域划分进行再次分类。
- `bindingTemplate` ：**调用Web服务的技术细节描述**。其中包括应用程序连接远程Web服务、并与之通信所必需的信息，如应用Web服务的地址、应用服务宿主和调用服务前必须调用的附加服务等，另外，还包括实现一些复杂的路由选择等。
- `tModel` ：**用于访问服务描述的技术标识**。描述关于服务调用规范的元数据，包括服务名称、发布服务的组织和指向规范本身的URL指针等。
- `publisherAssertion` ：**展示业务实体之间的关联关系**。
- `operationalInfo` ：**传输操作信息的数据结构**。
![图10-8 UDDI核心数据模型关联图](https://img-blog.csdnimg.cn/3ca08761809f4fc39ed7d1036decf4db.png)

 其中，所有 `businessEntity` 元素中包含的信息支持"黄页"分类法；`businessService` 元素和 `bindingTemplate` 元素一起构成了"绿页"信息。
## 2. `UDDI` 调用过程
**`UDDI` 提供一种统一的Web服务注册环境，用于存储和发布商业组织所提供的Web服务，便于Web服务使用者查找和调用服务**。如图10-8所示：
- 软件公司、标准组织或开发人员在 `UDDI` 商业注册中心中注册「所要发布的Web服务」的类型和描述信息；
- 实施人员完成Web服务的开发和部署工作后，在 `UDDI` 中注册业务描述信息及其所发布的服务；
- `UDDI` 为每个服务和注册分配唯一的标识；
- 市场、搜索引擎或业务应用等服务使用者，利用 `UDDI` 提供的服务查询接口查询服务注册信息，发现位于其它公司的Web服务来促进自己的业务集成；
- 服务使用者可以调用其需要的服务，并应用到自己的架构和业务流程中。
![图10-9 UDDI商业注册中心运行环境](https://img-blog.csdnimg.cn/a5208a6a37b44f52bfe8e7a593185db5.png)

`UDDI` 提供了两组API供应用程序查询和发布Web服务信息。其提供的调用接口如表10-1所示。
![表 10-1 UDDI商业注册中心运行环境](https://img-blog.csdnimg.cn/d23d0d102c6f44ab926c01d27c1bff80.png)
应用程序客户端通过 `UDDI` 提供的API，并设定相关的参数信息访问 `UDDI` 注册中心，程序10-13是客户端向 `UDDI` 注册中心发送的请求消息。
```xml
<!-- 程序 10-13 UDDI authToken 请求(*) 本节后面的程序中均去除了 SOAP 协议包装信息 -->
<get_authToken generic="2.0"
	xmlns="urn:uddi-org:api_v2" 
	userID="businessA_UserId"
	cred="businessA_Password" />
```
该请求消息中包含了用户名和密码，调用 `get_authToken` 获得 `UDDI` 的使用授权。调用提供的 `authTokenUDDI` 注册中心收到该消息后进行处理，得到的响应消息如程序10-14所示。
```xml
<!-- 程序 10-14 UDDI authToken 响应 -->
<authToken generic="2.0" operator="www.ibm.com/services/uddi" xmlns="urn:uddi-org:api_v2" >
	<authInfo>businessA_AuthToken</authInfo>
</authToken>
```
该响应消息中利用一个包含单个 `authInfo` 条目的 `authToken` 结构，返回给消息使用者授权信息，完成了服务请求者与 `UDDI` 商业注册中心的一次交互过程。

---
# 10.5 `WSFL/BPEL`
**`WSFL` 是一个用来描述Web服务组合的XML语言，它是描述业务流程的一个相对成熟的标准**。在 `WSFL` 中有两种不同的组合机制，分别利用流模型和全局模型来描述。其中：
- ==流模型描述了几个服务是怎样组合成一个流，以及流是如何转换成一个新的服务的==。流模型描述了流程的数据流和控制流，并组织不同服务的执行顺序；
- ==全局模型描述了组合服务中不同服务之间的交互==。

`WSFL` 利用活动、控制链、数据链和插入链四个基本元素来描述业务过程。而 `BPEL` 是IBM的 `WSFL` 和Microsoft的 `XLANG` 相结合而产生的技术。**它是一种基于XML的、用来描述业务过程的编程语言**，「被描述的业务过程的单一步骤」则由Web服务来实现。`Business Process Execution Language for Web Services, BPEL4WS` 是 `BPEL` 的全称，是专门针对Web服务及其组合而制定的一项规范，在 $2.0$ 版本中称为 `WS-BPEL`。

在吸收和借鉴了 `PetriNet` 和 `Pi-Calculus` 优势的基础上，`BPEL` 成为一种高级的、抽象的、可执行建模语言，==它不仅实现Web服务间的交互和流程编排，也能够将流程自身暴露为Web服务==。`BPEL` 作为一种流程编排语言，==必须通过 `BPEL` 执行引擎对其进行解析、并调用相关Web服务来执行整个业务流程==。
## 1. `BPEL` 基本结构
Web服务的目标是**提高业务功能的复用和自治能力**。在利用 `Web Services` 实现的面向 `SOA` 的环境中，存在大量完成某种特定业务功能的Web服务，其中包括原子服务和组合服务。==利用 `BPEL` 的业务流程编排能力，将这些Web服务有效地组织并组合起来、完成企业所需的业务功能，并且形成更大的功能复用模块供其它企业所使用==。

**`BPEL` 的主要能力是：对Web服务进行组织和编排，并提供不同Web服务之间的进行数据交换的机制**，可以完成Web服务的调用、数据操作、故障处理和流程控制等活动。`BPEL` 业务流程的核心是一个 `<process>` 元素，其基本结构如程序10-15所示。
```xml
<!-- 程序 10-15 BPEL 流程结构 -->
<process>
	<partnerLinks> ... </partnerLinks>
	<variables> ... </variables>
	<correlationSets> ... </correlationSets>
	<faultHandlers> ... </faultHandlers>
	<compensationHandlers> ... </compensationHandlers>
	<eventHandlers> ... </eventHandlers>
	(activities)*
</process>
```
`BPEL` 基本结构中，包括以下几个元素：
- `<partnerLinks>` ：描述**两个服务之间的会话关系**，其定义了会话中每个服务所扮演的角色，并且制定了每个服务所提供给服务描述 `WSDL` 的入口类型 `portType` ，以便接收会话的上下文中的消息。
- `<variables>` ：业务流程指定了涉及伙伴之间消息交换的有状态交互。业务流程的状态不仅包括被交换的消息，还包括用于业务逻辑和构造发送给伙伴的消息的中间数据，这些消息和数据通过所定义的变量来存储。
- `<correlationSets>` ：`BPEL` 提供了声明性机制，指定服务示例中相关联的操作组。一组相关标记可定义为「相关联的组中所有消息」共享的一组特性，即相关集。
- `<faultHandlers>` ：故障处理是因发生故障而切换到撤销发生故障的作用域中的部分或不成功的工作（？）。故障处理通过定义的 `catch` 活动，拦截某种特定类型的故障。
- `<compensationHandlers>` ：通过补偿处理程序，作用域可以描述一部分通过应用程序定义的方法可撤销的行为。有补偿处理程序的作用域可以不受约束地嵌套，其需要接收当前状态的数据、并返回关于补偿结果的数据。
- `<eventHandlers>` ：整个流程以及每个作用域，都可以与一组「在相应的事件发生时的并发调用事件处理程序」相关联。其处理的事件包括 `WSDL` 的传入消息和用户设置的时间警报。

此外，`BPEL` 中还使用到了其它一些元素：
- `<source>` ：标记活动作为链接源。
- `<target>` ：标记一个活动作为链接目标。
- `<delete>` ：删除已选择的活动或元素。
- `<onMessage>` ：表明消息到达时所要执行的活动。
- `<onAlarm>` ：表明时间到达时所要执行的活动。
- `<partner>` ：定义服务交互的伙伴。
- `<container>` ：提供「组成业务流程状态的消息」的存储方法。

`BPEL` 的业务流程本身就是一个流程图，类似于表达算法的执行过程。==活动是 `BPEL` 业务流程的核心，通过 `BPEL` 规范中的一些基本活动和结构化活动，将已有的一组Web服务组合为业务流程，创建新的Web服务==。图10-10显示了一个 `BPEL` 的业务流程，通过调用相关的Web服务来实现其功能，形成一个新的Web服务。利用端口类型中的输入输出操作，完成与Web服务之间的数据交换。
![图10-10 BPEL业务流程](https://img-blog.csdnimg.cn/1f0c47f89b8a4f2f965788e2a271f82f.png)
`BPEL` 中的活动包括基本活动和结构化活动。
- **基本活动定义了针对Web服务的操作和交互**：调用 `<invoke>` 某个Web服务上的操作，等待一条消息来响应由某人从外部进行调用的服务接口的操作（？）`<receive>` ，生成输入/输出操作的响应 `<reply>` ，等待一段时间 `<wait>` ，把数据从一个地方复制到另一个地方 `<assign>` ，指明某个地方出错了 `<throw>` ，终止整个服务实例 `<terminate>` ，或者什么也不做 `<empty>` ，还包括补偿处理活动 `<compensate>` 等。
- **通过使用语言所提供的任何结构化活动，可以将这些原语活动组合成更复杂的算法**。这些结构化活动提供的能力有：定义一组步骤的有序序列 `<sequence>` ，使用现在常见的  `"case-statement"` 办法来产生分支 `<switch>` ，定义一个循环 `<while>` ，执行几条可选路径中的一条 `<pick>` ，以及指明一组步骤应该并行地执行 `<flow>` 。在并行地执行的一组活动中，可以通过使用链接 `link` 来指明执行顺序方面的约束。
## 2. `BPEL` 业务流程示例
我们通过一个贷款批准流程的例子，说明 `BPEL` 如何描述一个业务流程。该流程中首先客户发出一个贷款请求，请求得到处理，然后客户弄清楚贷款是否得到了批准。初始时，客户向启用Web服务的金融机构发送申请、机构将决定告诉客户，该流程将使用他的申请、并给他发送一个响应。
![图10-11 贷款批准流程](https://img-blog.csdnimg.cn/22569415c6be41508b1333097ef3182d.png)
`BPEL` 需要引用被交换的消息、正在被调用的操作以及这些操作所属于的 `portType` ，因此对其所涉及的 `WSDL` 有比较大的依赖性。我们假定已经创建了所需的 $3$ 个 `WSDL` 文件及定义了其对应的命名空间：
- 贷款定义 `WSDL`（`loandefinitions.wsdl`）：`http://tempuri.org/services/loandefinitions`
-  贷款批准者 `WSDL`（`loanapprover.wsdl`）：`http://tempuri.org/services/loanapprover`
-  贷款批准 `WSDL`（`loan-approval.wsdl`）：`http://tempuri.org/services/loan-approval` 

**创建流程就是定义 `<process>` 元素的过程**。`loanApprovalProcess` 流程的定义、及其命名空间的引用如程序10-16所示。
```xml
<!-- 程序 10-16 BPEL 流程定义 -->
<process name="loanApprovalProcess"
	targetNamespace="http://acme.com/simpleloanprocessing"
	xmlns="http://schemas.xmlsoap.org/ws/2002/07/business-process/"
	xmlns:lns="http://loans.org/wsdl/loan-approval"
	xmlns:loandef="http://tempuri.org/services/loandefinitions"
	xmlns:apns="http://tempuri.org/services/loanapprover">
```
本例中主要关注流程的建立，忽略了其它如补偿处理等元素的内容。下一步是声明涉及的各方 `partner` 。如程序10-17所示，定义了已命名的伙伴，每个伙伴都有一个 `WSDL serviceLinkType` 表明其特征。对于这个示例来说，伙伴是客户和金融机构。伙伴的 `myRole/partnerRole` 属性指定了给定的 `serviceLinkType` 的伙伴和流程将如何交互。`myRole` 属性引用的是流程在 `serviceLinkType` 中将要扮演的角色，而 `partnerRole` 指定了伙伴将要扮演的角色。贷款批准流程为客户提供了 `loanApprovalPT` 功能，金融机构接着又为流程提供了这个功能。
```xml
<!-- 程序 10-17 BPEL 流程伙伴定义 -->
<partners>
	<partner name="customer" serviceLinkType="lns:loanApproveLinkType" 	
		myRole="approver"/>
	<partner name="approver" serviceLinkType="lns:loanApprovalLinkType"
		partnerRole="approver"/>
</partners>
```
为了申请贷款，客户向流程发送一条消息，流程询问金融机构是否将接受贷款申请，然后用另一条消息（要么是接受申请的消息，要么是拒绝申请的消息）应答客户。`BPEL` 流程需要将传入的消息放在 `BPEL` 活动可以访问到它的地方。==在 `BPEL` 中，数据被写入到数据容器中、并且从数据容器进行访问，这些数据容器可以保存特定的 `WSDL` 消息类型的实例==。程序10-18定义了所需要使用的容器列表。
```xml
<!-- 程序 10-18 BPEL 流程容器列表 -->
<containers>
	<container name="request" messageType="loandef:CreditInformationMessage"/>
	<container name="approvalInfo" messageType="apns:approvalMessage"/>
</containers>
```
本流程中仅包含了一个活动 `sequence` 。程序10-19在 `sequence` 中定义的 `receive` 活动，可以接收客户的消息、并把它保存到适当的容器中。
```xml
<!-- 程序 10-19 BPEL 流程接收消息 -->
<sequence>
	<receive name="receive1" partner="customer"
		portType="apns:loanApprovalPT"
		operation="approve" container="request"
		createInstance="yes">
</receive>
```
询问启用Web服务的金融机构是否将接受贷款，是用一个Web服务调用来完成的，该调用是在流程中由 `invoke` 活动定义。在程序10-20中 `invoke` 活动在运行时，将使用其输入容器中的消息来调用指定的Web服务，并将得到的应答放入到输出容器中。
```xml
<!-- 程序 10-20 BPEL 流程调用服务 -->
<invoke name="invokeapprover"
	partner="approver"
	portType="apns:loanApprovalPT"
	operation="approve"
	inputContainer="request"
	outputContainer="approvalInfo">
</invoke>
```

为了使流程对客户的请求作出应答，流程使用一个 `reply` 活动。程序10-21中的 `reply` 活动需要告诉客户金融机构的决定是什么，所以在调用的输出容器中将会发现要被发送的消息：`approvalInfo` 。
```xml
<!-- 程序 10-21 BPEL 流程客户响应 -->
	<reply name="reply" partner="customer" portType="apns:loanApprovalPT"
		operation="approve" container="approvalInfo">
	</reply>
	</sequence>
</process>
```
---
# 思考与练习
10-1 `Web Service` 与CORBA对象或EJB构件实现的服务有什么区别？
10-2 在基于 `Web Service` 实现的 `SOA` 架构中，如何结合 `SOAP, WSDL, UDDI, BPEL` 完成服务的描述、查找、交互和流程的编排？
10-3 理解了 `Web Service` 架构之后，分析其在工程应用的优势和不足。
