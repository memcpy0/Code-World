@[toc]

---
# 4.1 概述
## 4.1.1 接口与实现的分离
==将一个大型系统划分为若干模块，将一个模块的接口与实现相分离，并进一步能为一个接口提供多个实现，这是大型程序设计追求的目标之一==。这一设计目标直接带来的好处是，**有可能在「不同时间/空间效率的算法和数据结构」之间作出折衷选择**。接口与实现的分离，要求==将模块的私有部分归属到实现一方，接口只提供模块对外公开的信息，并且接口与实现之间不能再简单地通过相同的名字进行匹配==。

将这种思想应用在面向对象的分布式软件系统中，会带来更大的好处。**客户程序仅仅依赖于对象的接口，而不是对象实现**。客户程序甚至可以采用「与对象实现完全不同风格的程序设计语言」来编写。

**将对象接口与对象实现分离后，对象接口成为向外公布对象行为的唯一途径**。为保证对象接口的可用性，对象接口的定义应允许包括对象的名字、对象上可进行的操作（操作名字、参数类型、返回结果、可能产生的异常等）、对象上可访问的属性（实际上可归结为一种 $0$ 元操作）、相关的数据类型定义、常量定义、异常定义等信息。
## 4.1.2 接口定义语言
在 `CORBA` 模型中，对象实现对外提供信息和服务，客户程序利用这些信息与服务完成某些功能。这些信息与服务是对象实现与客户程序之间的一种合约 `contract` ，只有严格遵循合约的规定，双方才有可能协调工作。

OMG的接口定义语言 `Interface Definition Language, IDL` ，就是书写这种合约的标准语言，客户方与服务方必须使用这种统一的语言，才能正确地理解合约的内容。由于在分布式计算领域还存在DCE的 `IDL`、Microsoft 的 `IDL` 等其他缩写为 `IDL` 的接口定义语言，所以由OMG发布的接口定义语言，通常简写为 `OMG IDL`，在无二义性的上下文中也可简写为 `IDL` 。

`OMG IDL` 能够给出完整的接口定义，包括每一个操作需要的参数、返回结果、上下文信息、以及可能引发的异常等，完全能胜任对「复杂对象的规格说明」的无二义性描述。`OMG IDL` 具有如下特点：
- 具有面向对象的设计风格；
- 可用于定义分布式服务的规格说明；
- 可用于定义复杂的数据类型；
- 独立于具体的程序设计语言；
- 独立于特定的硬件系统。

应注意的是，**`OMG IDL` 仅定义了规格说明中的语法部分，未包括任何语义信息，因而一个 `IDL` 接口还不能理解为一个完整的抽象数据类型**。这对于实现软件自动化或软件重用的目标，是远远不够的。

`OMG IDL` 是一种说明性 `declarative` 语言，而不是一种程序设计语言，程序员的编程工作仍然采用传统的面向对象/过程式程序设计语言，编写客户端代码和服务端代码。**`OMG IDL` 用于定义由远程对象所提供服务的接口，包括分布式对象的服务能力、以及「由客户程序和服务程序共享的复杂数据类型**」。`IDL` 支持整数、浮点数、字符、宽字符、布尔值、八进制位组、`any` 等基本数据类型，并可利用 `typedef, struct, union, enum` 等方式合成更复杂的数据类型。

由于用 `IDL` 编写的对象接口担当了中间角色，==客户程序无法（也无需）知道服务端代码采用什么程序设计语言编写，程序员编写服务端程序也不必预测「访问服务对象的客户程序」由什么程序设计语言编写==。这时，**同一接口、多种实现的目标可轻而易举地达到**。

---
# 4.2 `OMG IDL` 的语法与语义
**`OMG IDL` 的语法规则基本上是 `ANSI C++` 的一个子集，再加上一些 `IDL` 特有的调用机制**。由于 `IDL` 被设计为一种说明性语言，所以它支持C++语言的常量、数据类型和操作的声明，但不允许出现任何具体的数据表示（如变量或对象实例的声明）和操作实现（如算法、控制结构、构造与析构函数等）。

在 `CORBA` 规范中，`OMG IDL` 的语义是以非形式化的自然语言表述的，理解 `IDL` 语义的最佳途径是**掌握 `IDL` 到某一门程序设计语言的映射规范**。`IDL` 到Java语言的映射规范，规定了如何将各种 `IDL` 构造映射到相应的Java构造，**在现有程序设计语言中，Java语言拥有最接近于 `OMG IDL` 的构造，因而 `IDL` 到Java语言的映射规则也是最简单的**。

`IDL` 的规格说明可分为 $6$ 类，包括模块、类型、常量、异常、接口以及值。
## 4.2.1 词法规则
`OMG IDL` 采用类似 `ANSI C++` 的词法规则和预处理特性（如编译指令 `#include` ）。`IDL` 文件本身采用 `ASCII` 字符集，但字符与字符串文字常量则采用 `Unicode` ，即 `ISO Latin-1 (8859.1)` 字符集。

==在 `OMG IDL` 中，关键字是大小写敏感的，但标识符却是大小写无关的，如果两个标识符仅仅大小写有区别，就可能在某些情况下产生编译错误==。例如，标识符 `account` 与 `ACCOUNT` 会被认为是重复定义；由于 `boolean` 是 `IDL` 的关键字，程序员将 `BOOLEAN` 用作标识符是非法的。==一旦声明了一个标识符后，所有对该标识符的引用必须与声明时的大小写完全相同，这样才可以自然地映射到那些像C++或Java一样区分大小写的程序设计语言==。出现这类大小写匹配问题时，`IDL` 编译器通常会给出一些警告信息。
## 4.2.2 模块的声明
模块用于限制标识符的作用域。尽管OMG没有硬性规定 `IDL` 文件必须使用模块，但是将相关的接口、类型、异常、常量等放在同一模块之中，无疑是一种良好的设计风格，这相当于为标识符划分了不同的名字空间。

一个 `IDL` 模块被映射为一个同名的Java程序包，该模块中的所有 `IDL` 类型被映射到相应程序包中的Java类或接口；不包含在任何模块之中的 `IDL` 声明，被映射到一个无名的Java全局作用域程序包。注意：`IDL` 模块不同于C/C++语言所采用的文件模块。**一个 `IDL` 文件可能定义多个模块，也可能不包含任何模块**。
## 4.2.3 类型的声明
### 1. 基本数据类型
`OMG IDL` 提供了 $9$ 种预定义的基本数据类型，包括浮点类型 `float, double, long double` 、整数类型（包括有符号整数类型 `short, long, long long` 和无符号整数类型 `unsigned short, unsigned long, unsinged long long` 、字符类型 `char` 、宽字符类型 `wchar` 、布尔类型 `boolean` 、八进制类型 `octet` 、任意类型 `any` 、对象类型 `Object` 和值基类型 `ValueBase` 。注意：**`IDL` 没有C/C++语言中那种 `int` 类型，这避免了数据类型对具体机器的依赖性**。

`OMG IDL` 基本数据类型映射到Java语言基本数据类型的规则如表 4-1 所示。
![表4-1 OMG IDL基本数据类型到Java语言的映射](https://img-blog.csdnimg.cn/4bee51250fd94880ac1dd8d6fb057755.png)
### 2. 复合数据类型
==`OMG IDL` 声明用户自定义类型的方式，与C++语言非常类似，可利用枚举、结构、联合体、序列和数组等方式定义新的类型名字==。
- 定义新类型的最简单方式，是利用 `typedef` 定义类型别名，但这时并没有创建任何新的类型，只是为原有类型起了一个新名字。
- 在 `enum` 声明的枚举类型中，标识符出现的次序定义了它们之间的相对次序，最大枚举长度为 $232$ 个标识符。**一个 `IDL` 枚举类型被映射为Java语言的一个同名的最终类**，其中定义了一个取值方法，并为每一个枚举常量定义了两个静态数据成员，以及一个将整数转换为该类型值的方法和一个私有的构造方法（**私有构造方法常用于阻止创建对象实例**）。
- `struct` 用于定义一种结构类型，名字作用域规则要求：在一个特定结构中成员的声明必须是唯一的。**`OMG IDL` 的结构类型被映射到Java语言的一个同名的最终类**，该类按 `IDL` 定义中的次序，为结构中的每一个域提供一个实例变量，并提供一个带参数的构造方法初始化所有的值，同时提供一个缺省构造方法，使得结构中的域可以到以后再初始化。
- **`IDL` 的 `union` 实际上兼蓄了C/C++语言的 `union` 和 `switch` ，要求联合体中的每一个成员都必须有一个标签**，如程序4-1所示。

```cpp
// 程序 4-1 OMG IDL 的枚举类型、结构类型与联合类型示例
enum Status {Mass, PartyMember, LeagueMember};	// 政治面貌
struct Person {									// 个人资料
	string name;								// 姓名
	boolean sex; 								// 性别
	short age; 									// 年龄
	union Class switch(Status) {
		case PartyMember: 
			struct PartyStatus { 				// 党员需要填写三样东西
				string partyName; 				// 党派名称
				string joinDate; 				// 入党时间
				string job; 					// 担任党内职务
			} ptMember;
		case LeagueMember: 
			struct LeagueStatus { 				// 团员需要填写两样东西
				string joinDate; 				// 入团时间
				string job; 					// 担任团内职务
			} lgMember;
		// 群众不必填写任何东西
	} politicsStatus;
};
```
**由 `sequence` 定义的序列实际上是一个数组**，但==它具有两个特性：在编译时确定的最大长度和在运行时确定的实际长度——在声明序列时可指定序列的最大长度，也可不指明最大长度（即序列长度不受限制），这些都会影响 `IDL` 到具体语言的映射==。`IDL` 的数组声明方式同C++语言，支持多维数组。序列和数组都映射为Java语言中的数组。

**字符串 `string` 也可定义为有限字符串和无限字符串**。宽字符串 `wstring` 与 `string` 的不同，只在于元素是 `wchar` 而不是 `char` 。定点类型 `fixed` 表示最大有效位为 $31$ 位的定点数，**声明定点数时，可指定一个非负整数作为比例因子**。

**本地类型 `native` 仅用于本地调用**，如果用于远程调用会引发一个 `MARSHAL` 异常。引入本地类型的主要目的是**专供对象适配器接口使用**，在应用程序接口或服务中通常不使用本地类型。

复合数据类型的定义有时是递归的，`IDL` 规定**只有 `sequence` 类型才允许递归**，例如程序4-2声明了一个单向链表中的结点类型。注意：`IDL` 并没有类似C++语言的指针类型。
```cpp
// 程序 4-2 用 IDL 递归定义数据类型
struct Node {
	ElementType element;
	sequence<Node> next;
};
typedef sequence<Node> List;
```
## 4.2.4 常量的声明
与C++语言相似，**`IDL` 的文字常量通过语法书写形式来决定常量的类型**，通过常量表达式可以定义符号常量。但是各种基本数据类型的内部表示方法、取值范围以及可运算的操作，与C++语言略有区别。

**在 `IDL` 接口中定义的符号常量映射为Java语言时，对应着该接口的 `Operations` 文件中的常量声明**。例如，在接口 `A` 中声明的如下 `IDL` 常量：
```cpp
const double PI = 3.1415926;
```
将映射为 `AOperations` 接口中的如下定义：
```cpp
public final static double PI = (double) 3.1415926;
```
**如果常量 `PI` 的声明不是出现在一个 `IDL` 接口中，则由 `IDL` 编译器创建一个与常量同名的公有接口**，生成的 `PI.java` 内容如下：
```cpp
public interface PI {
	public final static double value = (double) 3.1415926;
}
```
## 4.2.5 异常的声明
在Java或C++语言中，异常被定义为一个类，**`IDL` 则引入关键字 `exception` 专门用于定义异常**。异常由**异常标识符与相关的成员**组成，==如果调用某一操作时引发了一个异常，可通过异常标识符确定引发的是哪个异常；如果该异常还声明了成员，还可访问这些成员的值；如果该异常未声明任何成员，意味着该异常无任何附加信息==。

在 `CORBA` 中，异常被组织为图4-1所示的层次结构。 
![图4-1 CORBA异常的组织](https://img-blog.csdnimg.cn/7f249c42bfbf40e1ae2598b15f3626ec.png)
## 4.2.6 接口的声明
**接口是 `IDL` 定义中最重要的构造**，它可分为**抽象接口**和**具体接口**，抽象接口不可用于创建对象实例，但有其特殊的语义性质。**利用向前声明，可实现接口定义间的相互引用**。

由于 `IDL` 必须映射到一些不允许重载的程序设计语言，所以，**当前版本的 `IDL` 禁止使用C++和Java语言那种重载机制**。
### 1. 属性声明
**在 `IDL` 接口中包含属性的声明**，这些属性不应看作对象的状态数据，最好将它们理解为一种特殊的操作（零元操作）。==接口中的**可读写属性**将映射为Java语言中「一对互为重载的访问方法和设置方法」，**只读属性**只映射到单个访问方法，这些方法与属性同名==。例如，由如下声明的属性 `count` 和 `name` ：
```cpp
attribute long count;
readonly attribute string name;
```
将映射为 `Operations` 类中的三个操作：
```cpp
public int count();
public void count(int count);
public java.lang.String name();
```

注意：**`IDL` 的属性映射为Java语言的方法而不是变量**，故而 `IDL` 接口可直接映射到Java语言的接口而不是类（？）。

### 2. 参数传递方向
`OMG IDL` 声明形式参数表时，不同于大多数程序设计语言，它要求**每一个形式参数都必须包含一个标明参数传递方向的关键字 `in, out, inout`** 。
- `in` ：表明参数从客户程序传向对象实现；
- `out` ：表明数据从对象实现返回给客户程序；
- `inout` ：表明数据从客户程序传给对象实现，然后返回给客户程序。

**传递方向为 `in` 的参数，可直接映射到Java方法的参数**，并且 `IDL` 操作的返回方式也与Java方法相同。但是，`out` 和 `inout` 参数不能直接映射到Java语言的参数传递机制，为所有 `IDL` 基本类型和用户自定义类型生成的 `Holder` 类，正是为了解决这一问题。

**客户程序提供一个 `Holder` 类的对象实例，并以按值调用的方式传递给 `out` 或 `inout` 参数**，如程序4-3的第二部分所示。**调用前，`inout` 参数的传入值必须保存到「作为实际参数的 `Holder` 实例」中（既可通过带参数的构造方法，也可从另一 `Holder` 实例赋值，甚至可直接设置公有成员 `value` ）**。服务端的对象实现执行操作时，会修改该 `Holder` 实例的状态。**调用后，客户程序可通过实际参数的公有成员 `value` 访问传出的值**，如程序4-3的第三部分所示。
```cpp
// 程序 4-3 IDL 的参数传递方向
// 第一部分： IDL 定义
interface Modes {
	long op(in long inArg, out long outArg, inout long inoutArg);
};
// 第二部分：由 IDL 编译器生成的Java代码
public interface ModesOperations {
	public int op(int inArg, org.omg.CORBA.IntHolder outArg,
		org.omg.CORBA.IntHolder inoutArg);
}
public interface Modes extends com.inprise.vbroker.CORBA.Object,
	ModesOperations, org.omg.CORBA.portable.IDLEntity
{}

// 第三部分：程序员自己编写的Java代码（如客户程序）
Modes obj = ... ;
// 获取一个目标对象
int p1 = 57;
// 准备 in 参数的实际参数
IntHolder p2 = new IntHolder();
// 准备 out 参数的实际参数
IntHolder p3 = new IntHolder(123);
// 建立 inout 参数的实际参数
int result = obj.op(p1, p2, p3);
// 调用方法
... p2.value ...
// 使用 out 参数的值
... p3.value ...
// 使用 inout 参数的值
```
### 3. 单向操作
在 `IDL` 中没有返回值的操作，还可设计为单向 `oneway` 操作。**调用单向操作时，请求被发送到服务端，但对象实现不确认请求是否真的收到，因而单向操作不允许引发任何异常、或返回任何值**。例如，客户程序向服务端的系统监控程序，登记用户完成了某一操作的动作 `log` ，可设计为如下的单向操作：
```cpp
oneway void log(in string userName, in string description);
```
### 4. 上下文表达式
接口中的操作可附带一个上下文表达式，用于指明客户端那些「影响到对象请求执行情况的上下文元素」，如果没有上下文表达式，则表明不存在与该操作有关的请求上下文。

上下文表达式是由 `context` 引导的、一系列字符串常量清单，对象实现在执行请求指定的操作时，可利用客户端与这些字符串常量关联的值，这些信息存放在请求上下文 `request context` 之中。[第六章]()介绍动态调用接口时，将详细解释请求上下文的用法。
### 5. 继承机制
**`IDL` 接口的继承机制没有像C++或Java语言那样的访问控制，相当于只有C++语言的公有继承**，父接口中定义的所有操作、属性、数据类型、常量和异常等在派生接口中都是可见的。在派生 `IDL` 接口中重定义或重载父接口中的操作，都会被认为是重复定义，`IDL` 编译器会提示有语法错误。

**`IDL` 接口支持多继承和重复继承**：
- 如果两个 `IDL` 接口含有同名的操作或属性，则不可同时继承这两个接口；
- 如果同名的是常量、类型或异常，则允许同时继承这两个接口，但使用这些名字时必须必须采用前缀接口名和 `::` 的受限名字；
- 如果接口是一个抽象接口，那么它只能继承其他的抽象接口。

即使在 `IDL` 文件的接口定义中使用了继承机制，对象实现也未必一定要有继承。==`IDL` 支持多继承，可直接映射到C++这类支持多继承的程序设计语言，也可映射到Java这类仅支持单继承的语言，甚至可映射到C这类非面向对象程序设计语言==。虽然 `IDL` 中的方法都理解为动态绑定的，类似C++语言中的虚函数，但它们的实现技术未必采用C++语言常用的虚指针 `virtual pointers` 或虚表格 `virtual tables` 。
### 6. 抽象接口
**抽象接口同时是 `CORBA` 对象引用和值类型的抽象**，由于值类型无须支持 `CORBA` 对象引用的语义，因而抽象接口并不是隐式地继承 `CORBA::Object`，而是隐式地继承本地类型 `CORBA::AbstractBase` 。如果抽象接口能够被成功地转换为一个对象引用类型（即普通接口），则可在该接口上调用 `CORBA::Object` 中的操作。

抽象接口的主要用途是**用于操作的形式参数声明，从而可在运行时根据实际参数的类型，动态地决定参数传递方式是按值调用、还是按引用调用**。详细用法参见4.2.7小节。
## 4.2.7 值类型的声明
**值类型主要用于在网络中传递对象的状态信息**，例如，根据服务端一个对象实例的状态，在客户端创建同类型、同状态的另一个对象实例。==值类型有两种典型用法：一是，在按值调用的参数传递方式中创建对象副本；二是，在远程操作返回一个对象时创建对象副本==。当一个对象的主要目的是为了封装数据，或者一个应用程序需要显式地对某个对象进行复制时，对象应使用值类型 `valuetype` ，这时的对象实例通常简称为值 `value` 。

==由于作为实际参数的值是由客户端创建的，服务端接收后需要创建该值的副本，所以双方都必须了解对象的内部状态、以及与实现有关的信息==。这些状态信息都必须在 `IDL` 文件中显式地表达出来，作为客户程序与对象实现之间合约的一部分，因而**值类型同时兼有 `IDL` 的 `struct` 与 `interface` 两者的特性**。我们可将值类型理解为带有继承与操作的 `struct` ，与普通接口不同之处在于**它拥有描述内部状态的属性**，并且**包含比普通接口更多的实现细节**。

值类型有 $4$ 种声明形式，包括普通值声明、抽象值声明、封装值声明以及向前声明。
### 1. 抽象值类型
**抽象值类型**中只含有操作的基调，而没有状态数据和初始化操作，因而它们**不能用于创建对象实例**。一个抽象值类型可继承多个抽象值类型，但不允许继承普通值类型。
### 2. 普通值类型
普通值类型类似 `OMG IDL` 的 `struct` ，但其中允许包含状态数据和初始化操作，并且**一个普通值类型可以继承单个普通值类型和多个抽象值类型，还可支持 `support` 单个普通接口和多个抽象接口**。普通值类型可用关键字 `factory` 声明一些初始化操作，用于以可移植方式创建值类型的实例。

`ORB` 收到一个值时，会寻找相应值类型的 `Factory` 类，用于创建该值类型的一个本地对象实例，然后将值的状态数据解包到该实例中。如果找不到合适的 `Factory` 类，则引发一个 `MARSHAL` 异常。值类型与其工厂对象之间的映射与语言有关，例如，在 `VisiBroker for Java` 中，值类型 `V` 的 `Factory` 类的缺省名字为 `VDefaultFactory` ，如果使用其他名字，则必须显式地调用 `ORB` 提供的`register_value_factory` 方法在 `ORB` 注册工厂对象。使用缺省名字意味着隐
式地注册了工厂对象。工厂对象还可利用 `unregister_value_factory` 方法注销，也可调用 `lookup_value_factory` 查找已注册的工厂对象。

==如果形式参数类型为接口类型或值类型，它们分别静态地确定了参数传递方式为按引用调用和按值调用。由于抽象接口可看作是接口类型和值类型的抽象，利用抽象接口可支持运行时选择参数传递方式==。如果形式参数的类型是抽象接口 `A` ，则由实际参数 `p` 决定是按值、还是按引用传递，其规则如下：
- 如果 `p` 属于普通接口或其子类型，而该类型又是 `A` 的子类型，且 `p` 已注册到 `ORB` ，则 `p` 被当作一个对象引用；
- 否则，如果 `p` 属于值类型，且该值类型支持 `A`（一个值类型可支持多个抽象接口），则 `p` 被当作一个值；
- 否则，引发一个 `BAD_PARAM` 异常。

考察程序4-4，如果调用操作 `op` 时，实际参数 `para` 类型为 `itype` ，则 `para` 作为一个对象引用传递；如果 `para` 类型为 `vtype` ，则 `para` 作为值传递。
程序 4-4 由实际参数类型决定参数传递方式

```java
abstract interface atype {};
interface itype: atype {};
valuetype vtype supports atype {};

interface x {
	void op(in atype para);
};
```
**所有值类型都继承 `CORBA::ValueBase`，正如所有对象引用都是 `CORBA::Object` 的后代**，但缺省情况下值类型不继承 `CORBA::Object`，因而这些值不支持通常的对象引用语义。除非**显式地声明值类型支持某一接口类型，并将该值类型的实例经过对象适配器注册到 `ORB` ，这些值才支持对象引用语义**。
### 3. 封装值类型
**封装值类型 `boxed valuetype` 允许将非值类型的 `IDL` 数据类型包装为值类型**。例如，下述封装值类型的 `IDL` 声明：
```cpp
valuetype Label string;
```
等价于以下 `IDL` 值类型声明：

```cpp
valuetype Label {
	public string name; 
};
```
在 `CORBA` 模块中定义了一些标准的封装值类型，如 `StringValue, WstringValue` 等。**一旦 `IDL` 数据类型被包装为值类型，这些数据类型就可当作值使用**。使用封装值类型的最大理由是简单性：**封装值类型完全由 `IDL` 编译器自动生成的代码实现，无需我们自己编写代码**。
### 4. 定制值类型
如果声明值类型时加有前缀 `custom` ，说明这是一种定制的值类型，表示**不采用缺省的对象打包和解包操作**，而是由程序员手工实现由 `CustomMarshal` 接口继承的 `marshal` 和 `unmarshal` 方法。

编译一个定制值类型时，该值类型继承 `org.omg.CORBA.portable.CustomValue` ，而不像普通值类型那样继承 `org.omg.CORBA.portable.StreamableValue` 。编译器也不为定制值类型生成 `read` 和 `write` 方法，程序员必须自己实现这两个方法。
### 5. 可截断值类型
声明可截断值类型时，必须在被继承的值类型之前加上关键字 `trunctatble` ，表示**允许将「被继承的值类型」看作该值类型的父类型**。传递可截断值类型的对象时，所有在派生值类型中不属于父类型的状态数据都将丢失，因而==可截断值类型常用于接收对象时，不需要在派生值类型中定义的新数据成员或方法、或者无须知道值类型的确切派生类型的情况==。

---
# 4.3 使用值类型
本小节通过在[【中间件技术】第二部分 CORBA规范与中间件(3) 基于CORBA的开发过程](https://memcpy0.blog.csdn.net/article/details/122645195)3.3节例子基础上、修改得到的例子程序，演示 `IDL` 值类型的实现方式与使用方法，建议认真比较这两个例子中“对象引用”与“值”的使用效果。

下面以「新例对3.3节示例的改动」为线索来介绍。
## 4.3.1 `IDL` 定义
程序4-5所示的 `IDL` 文件，展示了修改后的的接口定义。与程序3-1中定义的接口不同，账户由接口 `interface` 变为了值类型 `valuetype` ，因此==客户端使用的“账户”已经不再是远端的分布式对象，而是远端账户对象在本地的副本（即本地对象）==。尽管从代码上看，账户管理员的 `open` 操作返回的仍然是 `Account` ，但是==返回值的性质发生了根本变化，原来返回的是远端对象的引用，现在返回的是一个对象副本==。
```cpp
// 程序 4-5 Bank.IDL
// 银行帐户管理系统的对象接口定义
module Bank {
	// 帐户
	valuetype Account {
		private float balance; //帐户的当前余额
		// 存款
		void deposit(in float amount);
		// 取款
		boolean withdraw(in float amount);
		// 查询余额
		float getBalance();
		// 初始化
		factory open(in float init);
	}; 
	
	// 帐户管理员
	interface AccountManager {
		// 查询指定名字的帐户，查无则新开帐户
		Account open(in string name);
	};
};
```
由于**值类型约定的是本地对象的规格说明**，因此认定账户对象包含一个表示当前账户余额的数据成员 `balance` ，此外，上面的程序中还为账户对象定义了一个初始化操作 `open` 用于创建账户对象，**值类型的初始化操作用关键字 `factory` 声明**。
## 4.3.2 编译 `IDL` 文件
使用 `VisiBroker forJava` 提供的 `IDL` 编译器 `IDL2java` 编译该 `IDL` 文件后，会在 `Bank` 子目录中生成若干 `.java` 文件。

由于 `AccountManager` 仍为接口 `interface` ，所以仍会生成与原来类似的 $7$ 个文件。而 `Account` 由接口变为了值类型，因此生成的文件会有所变化。`IDL2java` 会为上面定义的值类型 `Account` 生成如下 $5$ 个Java文件：
- `Account.java` ：包含值类型定义的所有变量的声明与方法基调的声明。该类约束了**提供的值类型实现必须实现哪些方法**，通常作为值类型实现的基类。
- `AccountValueFactory.java` ：仅当值类型的 `IDL` 定义中含有 `factory` 操作时，才生成该接口。它继承 `org.omg.CORBA.portable.ValueFactory` ，包含「值类型中所有通过 `factory` 关键字声明的初始化操作」的基调。**值类型的任何工厂 `Factory` 类都必须实现该接口**，即任何工厂类都必须实现值类型中声明的初始化操作。
- `AccountDefaultFactory.java` ：程序员可为一种值类型提供多个候选的工厂 `Factory` 类实现，`AccountDefaultFactory` 是由 `IDL` 编译器自动生成的缺省 `Factory` 类。如果存在 `AccountValueFactory` 接口，则由缺省 `Factory` 类实现该接口，否则缺省 `Factory` 类直接实现 `org.omg.CORBA.portable.ValueFactory` 接口。**程序员通常直接修改该类的方法来实现 `Factory` 类**。
- `AccountHelper.java` ：**提供常用辅助功能的 `Helper` 类**。如果值类型含有 `factory` 操作，`Helper` 类还提供这些初始化操作（例如 `open` 方法）的定义。
- `AccountHolder.java` ：类似接口对应的 `Holder` 类，是**支持 `out` 和 `inout` 参数的 `Holder` 类**。
## 4.3.3 实现账户管理员
普通接口 `AccountManager` 的实现方式与[【中间件技术】第二部分 CORBA规范与中间件(3) 基于CORBA的开发过程](https://memcpy0.blog.csdn.net/article/details/122645195)的例子程序相似，仍然采用继承 `AccountManagerPOA` 类的方式实现，如程序4-6所示。注意：程序中实例 `account` 的类型虽然是用 `IDL` 声明的 `Account` ，但却是一个地地道道的本地对象，**该实例被创建后、无须像原例子中一样注册到 `ORB`**（被注释的部分）。
```cpp
// 程序 4-6 AccountManagerImpl.java
public class AccountManagerImpl extends Bank.AccountManagerPOA {
	protected Hashtable accountList; // 该帐户管理员所负责的帐户清单
	public AccountManagerImpl(){
		accountList = new Hashtable();
	}
	
	public synchronized Bank.Account open(String name) {
		Bank.AccountImpl account = (Bank.AccountImpl)accountList.get(name);
		if (account == null) {
			Random random = new Random();
			float balance = Math.abs(random.nextInt()) % 100000 / 100f;
			account = new AccountImpl(balance);
			// try {
			//     org.omg.CORBA.Object obj =
			//     _default_POA().servant_to_reference(accountServant);
			//     account = Bank.AccountHelper.narrow(obj);
			// } catch(Exception exc) {
			//     exc.printStackTrace();
			// }
			accountList.put(name, account);
			System.out.println("新开帐户：" + name);
		}
		return account;
	}
}
```
**对象实现中的 `open` 方法，是使用值类型的一种典型用法，即返回一个值类型 `Account` 的实例**。服务端执行 `open` 方法时，调用 `Account` 的构造方法创建了一个名为 `account` 的实例，客户程序调用 `open` 方法获取的不是 `account` 的对象引用，而是在客户端生成的一个 `account` 的副本。**实例 `account` 及其副本分别在服务端和客户端、且都是本地的**。
## 4.3.4 实现值类型
类似普通接口类型，我们必须为 `Account` 提供真正的实现，实现代码如程序4-7所示，**值类型的实现 `AccountImpl` 继承了这一值类型的基类 `Account`** 。
```cpp
// 程序 4-7 AccountImpl.java
// 派生值类型 Account 的实现
public class AccountImpl extends Bank.Account {
	// 属性定义
	// protected float balance;
	// 构造方法，按指定余额创建新的帐户
	public AccountImpl(float bal){
		balance = bal;
	}
	
	// 往帐户中存款
	public void deposit(float amount){
		balance += amount;
	}
	
	// 从帐户中取款，不足余额则返回 false
	public boolean withdraw(float amount){
		if (balance < amount) return false;
		else {
			balance -= amount;
			return true;
		}
	}
	
	// 查询帐户余额
	public float getBalance(){
		return balance;
	}
}
```
与之前例子的实现不同，**由于在 `IDL` 文件中已经定义了属性 `balance` ，因此 `IDL` 编译器生成的基类 `Account` 已经包含了对应的属性定义**，所以在 `AccountImpl` 类实现中不需要再定义对应的数据成员。

与普通接口的实现方式不同的是，**普通值类型还必须实现一个 `Factory` 类，`Factory` 类提供了 `IDL` 中定义的所有 `factory` 初始化操作的实现**。由于值类型 `Account` 含有 `factory` 操作 `open` ， 所 以 `IDL` 编译器生成一个`AccountValueFactory` 接口，生成的另一个类 `AccountDefaultFactory` 实现了 `AccountValueFactory` 接口，我们直接对该类的方法进行修改，结果如程序4-8所示，修改的代码为创建 `AccountImpl` 对象的两行代码。

```cpp
// 程序 4-8 Bank\AccountDefaultFactory.java
// 值类型 Account 的缺省 Factory 类
package Bank;
public class AccountDefaultFactory implements AccountValueFactory {
	public java.io.Serializable read_value(org.omg.CORBA.portable.InputStream is) {
		// 创建并初始化值类型的对象
		java.io.Serializable val = new AccountImpl(0);
		// 通过 read_value 从 InputStream 中读取对象状态
		val = ((org.omg.CORBA_2_3.portable.InputStream) is).read_value(val);
		return val;
	}
	public Bank.Account open(float init) {
		return new AccountImpl(init);
	}
}
```
如果 `Factory` 类的名字不是使用缺省名字（例如命名为 `AccountSpecialFactory` ），则在客户程序和服务程序中都必须使用程序4-9所示语句，将工厂对象显式地注册到 `ORB` 。程序4-9显式地注册非缺省名字的工厂对象：
```cpp
...
// 确定工厂对象的标识
String id = AccountHelper.id();
// 创建一个工厂对象
AccountSpecialFactory factory = new AccountSpecialFactory();
// 将工厂对象注册到 ORB
((org.omg.CORBA_2_3.ORB) orb).register_value_factory(id, factory);
...
```
注意：`vbjc` 编译客户端或服务端的主程序时，不会自动编译 `AccountDefaultFactory` 类，我们必须自己动手完成编译。为避免涉及众多生成的Java类之间的依赖性问题，最简单的方式是执行如下命令：
```bash
prompt> vbjc Bank\*.java
```
## 4.3.5 服务程序与客户程序
与之前的例子相同，服务程序的主要任务是**创建、并在 `ORB` 注册账户管理员伺服对象** `managerServant` ，由于在新的例子中账户管理员仍然是一个分布式对象，因此可以使用与以前完全相同的服务程序。

为了演示值类型的使用效果，我们对之前使用的客户程序进行修改，修改后的代码如程序4-10所示。
```cpp
// 程序 4-10 Client.java
// 客户端的主程序
import Bank.*;
public class Client {
	public static void main(String[] args) {
		org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
		// 利用 POA 全称与对象标识"BankManager"查找帐户管理员
		Bank.AccountManager manager = Bank.AccountManagerHelper.bind(
			orb, "/BankPOA", "BankManager".getBytes());
		String name = args.length > 0 ? args[0] : "David Zeng";
		// 请求帐户管理员找出一个指定名字的帐户，无此帐户则新开一个
		Bank.Account account = manager.open(name);
		
		System.out.println(name + "的帐户余额为" + account.getBalance() + "元");
		account.deposit(200);
		System.out.println("存款 200 元后，本地余额为" + account.getBalance() + "元");
		account = manager.open(name);
		System.out.println("服务端" + name + "的帐户余额为" + account.getBalance() + "元");
	}
}
```

客户程序通过 `ORB` 获取账户管理员的远程对象引用 `manager` 后，调用该对象引用的 `open` 方法开设一个账户。注意：**由于 `Account` 是一种 `IDL` 值类型，调用 `open` 方法返回的实例 `account` 是客户端本地的局部对象，而不是通常的 `CORBA` 对象引用**，客户端的实例从服务端的实例复制了状态后，它们两者之间就是相互独立的。客户程序对返回的对象存款 $200$ 元后，再次调用账户管理员的 `open` 操作，重新打开同样的账户，查询其余额。

将本小节的例子程序拿去编译并运行会发现，客户程序控制台输出的存款操作已经完成，余额已经变化，但是再次打开同样的账户，却显示帐户的余额未发生变化，仍为存款之前的余额。我们有意设计成这种效果，以反映==值与对象引用的区别：由于 `open` 方法的返回值是值类型，因而客户端操纵的只是客户端对象的副本，当然不会对服务端的帐户余额产生影响==。

---
# 4.4 接口库
## 4.4.1 什么是接口库
`OMG IDL` 描述了一系列的**模块、接口、类型、常量、异常**等，这些定义存放在一个平坦的 `IDL` 文本文件中。这些对象接口定义也可由一个库服务来存储或管理，我们可以**将 `IDL` 文件编译成可运行的接口库 `Interface Repository, IR` 服务，由该服务提供对象接口的相关信息**。
 
接口库中的信息与 `IDL` 文件相同，只不过这些信息被组织为「更适合客户程序在运行时访问的形式」，因而可将接口库看作**关于 `CORBA` 对象的元信息的联机数据库**。例如，可开发一个客户程序浏览一个接口库的内容，作为项目开发小组的联机参考工具；接口库更常见的用法是，==查找对象引用的所有接口定义，利用接口库获取一个编译时未知类型的对象引用的接口信息，为利用动态调用接口 `DII` 或动态框架接口 `DSI` 调用对象作准备==。
## 4.4.2 接口库的结构
==接口是关于对象的描述，接口库则将这些接口描述又看作对象，为这些对象定义新的接口，从而无须引入数据库这一类的新机制==。接口库将「接口定义」存储为「一组具有层次结构的对象」，然后提供以 `OMG IDL` 形式定义的接口来访问这些对象。**接口库中保存的是接口的相关信息，而不是关于对象引用的信息**。

**在 `IDL` 文件中定义的各种构造具有明显的层次结构**，例如：一个 `IDL` 文件中包含模块的定义，模块中又包含接口的定义，接口中含有操作的定义。相应地，**接口库也组织为层次结构**，例如：一个接口库中包含 `ModuleDef` 对象，`ModuleDef` 对象又包含 `InterfaceDef` 对象，`InterfaceDef` 对象又可包含 `OperationDef` 对象。我们根据接口库的 `ModuleDef` 就可了解它所包含的 `InterfaceDef, OperationDef` 等；反之亦然，给定 `InterfaceDef` 即可知道包含它的 `ModuleDef` 。所有其他的 `IDL` 构造，如类型、常量、异常、属性、值类型等，都可如此表示在接口库中。

**接口库中的模块定义、接口定义等对象，都有一个字符串作为名字，有一个枚举常量表示对应的类型**。但是，用于唯一标识一个接口库对象的字符串是**接口库标识**。一个库标识由三部分组成，不同部分之间由冒号 `:` 分隔。开头部分固定为 `"IDL"` ，结尾部分表示版本号（例如 `"1.0"` ），中间部分是一系列由斜杠 `/` 分隔的标识符，表示接口库对象在 `IDL` 接口定义中的层次结构。例如，对应程序4-5所示 `Bank.IDL` 的接口库中，值类型 `Account` 的库标识为 `IDL:Bank/Account:1.0` 。

接口库中还包含了**类型码** `type code` ，==类型码是表示参数类型或属性类型的值，只要有类型码，就可确定一个类型的完整结构==。类型码无须由程序员显式地定义在 `IDL` 文件中，它们会自动从 `IDL` 文件中出现的各种类型生成。==类型码可用于打包和解包 `any` 类型的对象实例，`any` 类型是一个通用类型，可表示任意类型、并用于动态调用接口==。从接口库或 `IDL` 编译器都获取类型码，在接下来两章中还将看到类型码的作法。

使用接口库时，应注意**接口库是可读写的，并且无任何访问控制保护**。一个错误的或恶意的客户程序，可能破坏接口库或获取接口库的敏感信息。
## 4.4.3 接口库管理工具
**开发人员可创建多个接口库，并自己决定如何调配和命名接口库**。例如，一个项目小组可能约定，用一个中心接口库包含所有最终软件产品的对象接口，而不同的开发人员则可创建自己的接口库进行测试。

**接口库采用典型的客户机/服务器模型**。`VisiBroker for Java` 提供的接口库服务程序称为 `irep` ，该程序作为一个监控进程 `daemon` 运行，可用于创建接口库、并装入其内容，它必须结合智能代理 `osagent` 一起使用。开发人员还可利用另外两个工具 `ir2IDL` 和 `IDL2ir` ，浏览、更新和设置接口库，或者自己编写专用的接口库客户程序来监控或新接口库。
### 1. 装入接口库
例如，可在命令行状态输入下述命令，指示从 `IDL` 文件 `Bank.IDL` 创建一个名为 `BankIR` 的接口库：
```bash
prompt> start irep BankIR Bank.IDL
```
进程 `irep` 启动后，将 `Bank.IDL` 中的内容装入到接口库 `BankIR` 中，然后等待响应客户程序送来的请求。如果只是想创建一个空的接口库，可使用如下命令：
```bash
prompt> start irep BankIR
```
`irep` 提供最简单的事务功能——如果指定的 `IDL` 文件不能成功装入，接口库会将其内容回卷 `rollback` 到原状态；如果成功装入，则提交新的状态供后续事务使用。
### 2. 浏览接口库内容
利用客户程序 `ir2IDL` 可查看接口库的内容。例如，以下命令列出接口库 `BankIR` 中的所有定义：
```bash
prompt> ir2IDL -irep BankIR
```
运行该命令，要求在网络中至少已启动一个智能代理，并且程序员已启动一个 `irep` 服务程序装入接口库 `BankIR` 。下述命令还可将接口库中的 `IDL` 定义存储到一个名为 `NewBank.IDL` 的 `IDL` 文件中：
```bash
prompt> ir2IDL -irep BankIR -o NewBank.IDL
```
多次启动 `irep` 进程，可装入多个不同名字的接口库，如果在 `ir2IDL` 后不接任何参数，如：
```bash
prompt> ir2IDL
```

则表示随机地列出由智能代理能找到的某一个接口库的内容。
### 3. 更新接口库内容
利用客户程序 `IDL2ir` 可更新接口库的内容。例如，以下命令指示根据 `IDL` 文件 `Bank.IDL` 中的定义更新接口库 `BankIR` ：
```bash
prompt> IDL2ir -irep BankIR -replace Bank.IDL
```
**接口库中的内容不能直接使用 `IDL2ir` 或 `irep` 工具修改或删除**。为更新接口库中的某一个项目，必须退出 `irep` 程序，编辑曾在 `irep` 命令行指定的 `IDL` 文件，然后使用更新过的 `IDL` 文件重新启动 `irep` 。

如果想删除接口库中所有的项，可使用一个全空的 `IDL` 文件替换其内容。例如使用名为 `Empty.IDL` 的全空 `IDL` 文件，可运行以下命令：

```bash
prompt> IDL2ir –irep BankIR –replace Empty.IDL
```

## 4.4.4 接口库客户程序
**存储在接口库中的对象类型，基本上对应着 `IDL` 的语法单位**，如表4-2所示。例如，一个 `StructDef` 类型的对象包含与一个 `IDL` 结构声明相同的信息，一个 `InterfaceDef` 类型的对象包含与一个 `IDL` 接口声明相同的信息，一个 `PrimitveDef` 对象包含与一个 `IDL` 基本类型声明（如 `boolean, long` 等）相同的信息。
![表4-2 存储在接口库中的对象](https://img-blog.csdnimg.cn/18da77221ce6442ebb0feb60064cdcde.png)
`CORBA` 为表4-2中的每一种对象类型，都提供了丰富的操作，用于创建或访问其中包含的各种定义，并将「接口库中所有对象的共同特性」定义为 $3$ 个 `IDL` 抽象接口，如表4-3所示。这 $3$ 个接口都提供了 `Helper` 类和 `Holder` 类，**这些辅助类的作用与普通接口生成的 `Helper` 类和 `Holder` 类相同**。
![表4-3 接口库对象的父接口](https://img-blog.csdnimg.cn/19cfdd8f5bb848258887ad1fe8833a39.png)
本节给出一个简单的例子程序，演示如何获取接口库中的接口定义信息。由于接口库以 `irep` 作为服务程序，所以无须另外开发接口库的服务程序，只要编写客户程序查询或操纵接口库中的内容即可，客户程序的源代码如程序4-11所示。
```java
// 程序 4-11 自编客户程序访问接口库中的内容
// 利用接口库检查对象接口中定义的操作
import org.omg.CORBA.*;
public class Client {
	static final String MODULE_NAME = "Bank";
	static final String INTERFACE_NAME = "Account";
	static final String OPERATION_NAME = "transfer";
 
	public static void main(String[] args) {
		try {
			// 初始化 ORB 并获取接口库的根容器对象
			org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init(args, null);
			Repository ir = RepositoryHelper.narrow(
				orb.resolve_initial_references("InterfaceRepository"));
				
			// 列出接口库中所有的模块定义
			Contained modules[] = ir.contents(DefinitionKind.dk_Module, true);
			System.out.println("接口库有" + modules.length + "个模块");
			for (int ptr = 0; ptr <= modules.length - 1; ptr++)
				System.out.println("模块" + (ptr + 1) + "库 Id 为" + modules[ptr].id());
				
			// 如果接口库中有模块 Bank 的定义，则列出其中所有的接口定义
			Container bankDef = ContainerHelper.narrow(ir.lookup(MODULE_NAME));
			if (bankDef != null) {
				System.out.println("接口库中包含模块" + MODULE_NAME);
				Contained interfaces[] = bankDef.contents(DefinitionKind.dk_Interface, true);
				System.out.println("模块" + MODULE_NAME + "有" + interfaces.length + "个接口");
				for (int ptr = 0; ptr <= interfaces.length - 1; ptr++)
					System.out.println("接口" + (ptr + 1) + "库 Id 为" + interfaces[ptr].id());
			}
			
			// 如果模块 Bank 中有接口 Account 的定义，则列出其中所有的操作定义
			Container accountDef = ContainerHelper.narrow(bankDef.lookup(INTERFACE_NAME));
			if (accountDef != null) {
				System.out.println("模块" + MODULE_NAME + "包含接口" + INTERFACE_NAME);
				Contained operations[] = accountDef.contents(DefinitionKind.dk_Operation, true);
				System.out.println("接口" + INTERFACE_NAME + "有" + operations.length + "个操作");
				for (int ptr = 0; ptr <= operations.length - 1; ptr++)
					System.out.println("操作" + (ptr + 1) + "库 Id 为" + operations[ptr].id());
			}
		} catch(Exception exc) {
			exc.printStackTrace();
		}
	}
}
```
客户程序首先初始化 `ORB` ，然后利用 `ORB` 对象引用的`resolve_initial_references` 方法，获取接口库最上层容器的一个对象引用 `ir` ，这是一个类型为 `org.omg.CORBA.Object` 的通用对象引用，必须利用接口库容器类 `Repository` 的 `Helper` 类将它窄化为 `Repository` 类型。注意，这里名字 `"InterfaceRepository"` 是 `resolve_initial_references` 方法专用于接口库的硬编码 `hard code` ，类似的硬编码还有 `RootPOA, POACurrent, PolicyCurrent, NameService, DynAnyFactory` 等等。

**客户程序访问接口库对象的内容时，主要利用 `contents` 和 `lookup` 方法**。方法 `contents` 列出接口库对象所包含的、指定类型的 `IDL` 定义内容，可指定的类型作为枚举常量定义在 `DefinitionKind` 中，其中 `dk_all` 表示所有类型。方法 `lookup` 将一个名字解析为一个类型为 `Contained` 的对象引用，由于还要利用该对象引用作为 `Container` 继续列出其中的内容，我们必须使用 `Container` 的 `Helper` 类，将该对象引用的类型窄化为 `Container` 。注意，**在 `CORBA` 应用程序中，许多类型转换都必须利用 `Helper` 类的 `narrow` 方法完成，不能使用Java语言提供的显式或隐式类型转换**。

运行上述例子程序之前，必须首先启动 `osagent` ，并启动 `irep` 、装入一个 `IDL` 文件作为接口库的服务程序。如果运行前 `osagent` 未就绪、或 `irep` 未启动，`resolve_initial_references` 方法将引发一个 `OBJECT_NOT_EXIST` 异常。假设 `irep` 将[CORBA规范与中间件(3)](https://memcpy0.blog.csdn.net/article/details/122645195)例子程序 `Bank.IDL` 装入作为接口库的内容，运行本小节的例子程序，可得到如下所示的输出结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/44146a7b4bac47ad96a128935a9aaf67.png)
在本书接下来的两章中，还将看到接口库的另一种典型用法，即==应用在「客户端的动态调用接口」和「服务端的动态框架接口」中，为动态调用提供准确的接口信息==。

---
# 4.5 编写对象接口的准则
## 4.5.1 如何编写好的对象接口
这是一个十分有价值却又很难回答的问题。**高质量的 `IDL` 接口设计对 `CORBA` 应用系统的成功具有重大影响**。在编写对象接口时，有大量与应用有关的问题需要考虑，因而不存在什么统一标准来度量对象接口的优劣。尽管如此，人们在长期的分布式软件应用中仍总结出了几条启发性的规则可供参考：
1. 由面向对象分析与设计理论可知，一个 `IDL` 接口应该与「现实世界中的业务系统中的某个工作实体」相对应。==面向对象分析设计的知识与经验，有助于开发良好风格的 `IDL` 对象接口设计==。面向对象的软件开发过程可看作一个建模的过程，而 `IDL` 接口定义只是这个模型的一部分形式化描述。
2. 开发人员在设计分布式对象接口时，必须始终贯彻这样的指导思想，即==随时注意到「同一进程中对象的接口」与「分布式对象的接口」通常存在很大的区别==，忽视这种区别，将会给分布式对象系统的可靠性、可伸缩性、可重用性等质量因素带来危害。
3. 分布式对象设计必须考虑的重要问题之一是**性能问题**。注意，==使用远程调用时必须考虑网络的开销==。本地调用通常以微秒级度量，但远程调用则以毫秒级计算。并且，在设计 `IDL` 接口的操作时，应一次返回尽可能多的信息，从而避免客户程序又要发出远程调用、以获取一些相关的数据。在稍后的高级课题中，还将讨论与性能有关的若干问题。
4. ==区别会话型接口与实体型接口，有利于设计人员组织众多的分布式对象==。客户程序利用会话型接口，表达与特定客户程序有关的上下文信息，而**实体型接口则用于表示共享的业务对象**。例如，在一个典型的网上购物系统中，购物车属于会话型接口，而所购商品则属于实体型接口。
5. ==在许多分布式数据处理系统中，事务处理也是必须考虑的重要问题之一==。设计人员应仔细考虑系统中有哪些事务，以及如何将这些事务映射为接口上的操作。此外，在使用异步事务时还须考虑，如何确定事务的完成状态、或当事务提交失败时状态的恢复。

## 4.5.2 典型的对象接口
在不同的分布式应用系统中，存在许多不同类型的接口和设计模式，参考这些典型的对象接口设计，有助于我们开发设计良好风格的 `IDL` 接口。常见的对象接口包括：
1. **操纵型接口**，这类接口常用于**操纵单个对象实体**。例如，例子程序中的帐户接口 `Account` 属于这一类型。
2. **工厂型接口**，这类接口常用于**创建或撤销对象**。例如，例子程序中的帐户管理员接口 `AccountManager` 属于这一类型，它负责帐户对象实例的开设与撤销。这种类型的接口名字通常以 `Manager, Factory` 等作为后缀。在 `CORBA` 模型中，工厂对象专指那些「用于创建其他分布式对象的分布式对象」。
3. **查找与选择型接口**，这类接口用于**确定对哪个对象进行操纵**。例如，在例子程序中，我们可能设计一个名为 `AccountFinder` 的接口，用于查找满足某些条件的数据，并返回一个或多个 `Account` 的对象引用供客户程序使用。
4. **管理型接口**，这通常是一个 `RPC` 风格的接口，我们将「要操纵的对象」设计为操作的参数，而不是调用这些对象的方法。这一类型的接口常用于**在一次调用中操纵多个对象实例**。例如，在一个分布式的电算化会计系统中，`GeneralLedgerManager` 有一个“试算平衡”操作 `checkBalance` ，该操作输入一系列帐户后，检查所有帐户的借方与贷方是否相等。

---
# 思考与练习
4-1 `OMG IDL` 的值类型有什么作用？它与 `IDL` 接口有什么主要区别？
4-2 如何的保持 `Account` 为值类型不变的前提下，修改4.3中例子，使得客户程序中对账户的修改真正在服务端生效？
4-3 4-2中基于值类型的方案与第三章基于接口的方案相比，采用值类型方案有什么优势，适用于什么应用场合？
