@[toc]

本章首先通过三个例子讨论如何使用J2EE中间件提供的环境条目、事务控制、安全性控制等公共服务，最后简单介绍 `EJB 3.0` 。

---
# 9.1 环境条目
J2EE中的**环境条目** `Environment Entry` 由"名字-值"对组成，是**J2EE提供的源代码以外的可定制性支持之一**——==在EJB代码中可以根据环境条目的不同取值，执行不同的操作，当环境条目的值需要改变时，只需修改对应环境条目的配置、而不需要修改EJB构件的源代码==。

本节通过一个简单例子，演示如何配置与使用环境条目。我们将创建一个名为 `ConverterApp` 的J2EE应用，该应用中仅包含一个简单的无状态会话构件，提供简单的币值换算的功能。币值换算要用到汇率，**汇率是一个会经常变化的值，因此将汇率声明为环境条目**，在部署时可以修改汇率，EJB构件提供服务时，会根据EJB环境中的汇率值进行币值换算。
## 9.1.1 开发EJB构件
该EJB构件实现币值换算的功能，==由于该构件的实例（对象）不需要保存与特定客户端相关的会话状态，因此设计为无状态的会话构件==。以下简称该EJB构件为币值换算 EJB。
### 1. 定义 `Remote` 接口
`Remote` 接口包含EJB构件实现的商业方法的声明，客户端只能通过 `Remote` 接口、访问构件实现的商业方法，不能直接调用。程序9-1给出了币值换算EJB的 `Remote` 接口定义：

```java
// 程序 9-1 币值换算EJB的 Remote 接口定义
package FlexConverter;
import javax.ejb.*;
import java.rmi.*;
public interface Converter extends EJBObject {
	public double dollarToYen(double dollars) throws RemoteException;
	public double yenToEuro(double yen) throws RemoteException;
}
```
在程序9-1中，定义了一个名为 `Converter` 的 `Remote` 接口，币值换算EJB构件向客户端提供两个币值换算方法—— `dollarToYen` 完成美元到日元的换算，`yenToEuro` 完成日元到欧元的换算，`Remote` 接口中包含这两个方法的声明。可以看到，按照EJB规范的约定，**接口 `Converter` 继承了接口 `EJBObject` ，两个操作抛出 `RemoteException` 异常、以报告远程调用错误，参数与返回值均为合法的 `Java RMI` 类型**。
### 2. 定义 `Home` 接口
`Home` 接口中包含EJB构件生命周期管理的相关方法，客户程序使用 `Home Interface` 创建、查找或删除EJB的实例。程序9-2给出了币值换算EJB的 `Home` 接口定义：
```java
// 程序 9-2 币值换算EJB的 Home 接口定义
package FlexConverter;
import java.rmi.*;
import javax.ejb.*;
public interface ConverterHome extends EJBHome {
	public Converter create() throws RemoteException, CreateException;
}
```
在程序9-2中，我们定义了一个名为 `ConverterHome` 的 `Home` 接口，按照EJB规范的约定，接口 `ConverterHome` 继承了接口 `EJBHome` 。该接口中仅声明了一个 `create` 方法，其返回值必须为程序9-1中定义的 `Remote` 接口 `Converter` 。按照EJB规范的约定，**`create` 方法抛出 `RemoteException` 和 `CreateException` 异常**。
### 3. 定义 `Enterprise Bean` 类
币值换算EJB的 `Enterprise Bean` 类，首先要按照 `Remote` 接口的约定实现商业方法 `dollarToYen` 与 `yenToEuro` ，其次要实现 `Home` 接口中 `create` 方法对应的 `ejbCreate` 方法、与会话构件生命周期相关的方法。程序9-3给出了币值换算EJB的 `Enterprise Bean` 类定义：
```java
// 程序 9-3 币值换算EJB的 Enterprise Bean 类定义
package FlexConverter;
import javax.ejb.*;
import javax.naming.*;
public class ConverterBean implements SessionBean {
	SessionContext ctx;
	public double dollarToYen(double dollars) {
		Context initCtx;
		Double dollarsToYen = null;
		try {
			initCtx = new InitialContext();
			dollarsToYen = (Double)initCtx.lookup("java:comp/env/dollarsToYen");
		} catch(NamingException ne) {
			ne.printStackTrace();
		}
		return dollarsToYen.doubleValue() * dollars;
	}
	public double yenToEuro(double yen) {
		Context initCtx;
		Double yenToEuro = null;
		try {
			initCtx = new InitialContext();
			yenToEuro = (Double)initCtx.lookup("java:comp/env/ yenToEuro");
		} catch(NamingException ne) {
			ne.printStackTrace();
		}
		return yenToEuro.doubleValue() * yen;
	}
	
	public ConverterBean() {}
	public void ejbCreate() {}
	public void ejbRemove() {}
	public void ejbPassivate() {}
	public void ejbActivate() {}
	public void setSessionContext(SessionContext Context) {
		ctx = Context;
	}
}
```
在程序9-3中，我们定义了一个名为 `ConverterBean` 的 `Enterprise Bean` 类，由于该EJB构件是会话构件，因此 `Enterprise Bean` 类实现（`implements`）`SessionBean` 接口。

`Enterprise Bean` 类首先按照 `Remote` 接口 `Converter` 的约定实现了商业方法 `dollarToYen` 与 `yenToEuro` ，**在进行币值换算时，首先利用 `JDNI` 服务查找表示当前汇率的环境条目，然后根据环境条目中存储的汇率值完成币值换算**。其次，`Enterprise Bean` 类实现了 `Home` 接口 `ConverterHome` 中的 `create` 方法对应的 `ejbCreate` 方法和会话构件生命周期相关的其它方法。

源代码编写完成后，用Java语言编译器 `javac` 对EJB源代码进行编译。
### 4. 打包/布署EJB
本例中打包与布署EJB构件的过程，与8.2节中打包/布署时间EJB类似，不同的地方主要有几下几点：
- 本例将包含EJB构件的J2EE应用命名为 `ConverterApp` 。
- 在环境条目配置界面，为该EJB构件配置两个分别表示当前美元到日元汇率与当前日元到欧元汇率的环境条目，如图9-1 所示，具体的配置信息为：
	- `Coded Entry` ：该环境条目在源代码中引用时所使用的 `JNDI` 名，该名字与程序9-3中查找对应环境条目所使用的 `JNDI` 名一致，分别为 `"dollarsToYen"` 与 `"yenToEuro"` 。
	- `Type` ：环境条目的类型，本例中两个环境条目的类型均为双精度浮点类型 `Double` 。
	- `Value` ：环境条目的值，分别输入表示当前美元到日元汇率与当前日元到欧元汇率的值，如 $121$ 与 $0.0077$ 。
![图9-1 配置表示汇率的环境条目](https://img-blog.csdnimg.cn/a470288a2e73473796edb49279b2b70c.png)
- 为币值换算EJB配置名为 `"FlexibleConverterBean"` 的 `JNDI` 名。
- 布署时生成名为 `"ConverterAppClient.jar"` 的客户端 `JAR` 文件包，并将其存放到后面客户端程序所在的目录下。
## 9.1.2 开发客户端程序
### 1. 创建客户端程序
本例中，编写一个名为 `ConverterClient` 的Java类作为客户端，该类包含一个入口函数 `main` 。该客户端程序调用币值换算EJB的 `dollarToYen` 与 `yenToEuro` 方法，提供的参数均为 $100$ ，并将返回的换算结果输出到客户端。客户端程序的代码如程序9-4所示。
```java
// 程序 9-4 币值换算EJB的客户端程序
import FlexConverter.*;
import javax.naming.*;
import javax.rmi.PortableRemoteObject;
public class ConverterClient {
	public static void main(String[] args) {
		try {
			Context initial = new InitialContext();  
			ConverterHome converterHome = (ConverterHome)initial.lookup("FlexibleConverterBean");
			Converter converter = converterHome.create();
			double amount = converter.dollarToYen(100.00);
			System.out.println("$100 equals Yen " + String.valueOf(amount));
			amount = converter.yenToEuro(100.00);
			System.out.println("Yen 100 equals Euro " + String.valueOf(amount));
			converter.remove();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
｝
```
在程序9-4所示的客户端程序中，首先利用 `JNDI` 服务查找币值换算EJB对应的 `Home` 接口，查找到 `Home` 接口后，客户端调用 `Home` 接口中的 `create` 操作来获取一个可用的EJB，该操作返回一个 `Remote` 接口，客户端利用返回的 `Remote` 接口调用EJB上的操作 `dollarToYen` 与 `yenToEuro` 。使用完EJB后，客户端程序调用 `Remote` 接口中的 `remove` 操作，通知服务端其不再使用该EJB。
### 2. 编译/运行客户端程序
本例中可使用以下命令，完成对客户端程序的编译：
```bash
prompt> javac -classpath "ConverterAppClient.jar;%CLASSPATH%" ConverterClient.java
```
本例中可使用以下命令，执行客户端程序：
```bash
prompt> java -classpath " ConverterAppClient.jar;%CLASSPATH%" ConverterClient
```

运行客户端程序时，客户端输出返回的币值换算结果，例如：
```java
$100 equals Yen 12100.0
Yen 100 equals Euro 0.77
```
客户端程序调用 `dollarToYen` 与 `yenToEuro` 方法时传入的参数均为 $100$ ，而打包时环境条目中配置的相应汇率分别为 $121$ 与 $0.0077$ ，因此我们得到了上面的输出结果。

==当相应的汇率发生变化时，我们可以仅修改J2EE应用中对应的环境条目值，而不需修改EJB构件的源代码，就可以使得EJB构件按照新的汇率完成币值换算==。如图9-2所示，在 `deploytool` 中，选中打包好的EJB构件 `ConverterBean` ，点击右侧的 `"Env. Entries"` 选项卡，可以修改汇率对应的环境条目值，如可以将汇率分别修改为 $200$ 与 $0.006$ 。
![图9-2 配置表示汇率的环境条目](https://img-blog.csdnimg.cn/444f814982444aa0a507aefac93bec74.png)
修改完环境条目后，将本例的J2EE应用重新发布，再次执行客户端程序，可以得到下面的输出，看出得到的币值换算结果使用了新的汇率值。
```java
$100 equals Yen 20000.0
Yen 100 equals Euro 0.6
```


---
# 9.2 事务控制
J2EE平台为EJB构件提供 `Container Managed Transaction` ，容器维护的事务）与 `BMT` 两种事务控制方式。`CMT` 方式下，**事务由容器自动控制，开发人员只需在打包EJB构件时，为EJB上的操作选择合适的事务属性即可**。`BMT` 方式下，**开发人员在EJB的源代码中，利用 `JTA` 来编程控制事务**。

本节通过两个例子，分别演示 `CMT` 方式与 `BMT` 方式的事务控制。
## 9.2.1 基于 `CMT` 的事务控制
本小节通过一个简单的例子，演示如何基于 `CMT` 方式实现事务控制。`CMT` 方式下，程序员==在EJB构件的源程序中，没有事务边界控制的代码（如事务开始、回滚、提交等），而是在部署描述符中指定事务属性，由容器按照属性对应的规则来控制事务的边界==。可以选择的事务属性、及对应的事务控制规则，如表9-1所示：
![表9-1 事务属性与事务控制规则](https://img-blog.csdnimg.cn/b17d255ca4c14ab486b3a27d0e0bda6b.png)
在表9-1中，第一列表示**EJB方法的事务属性取值**，打包EJB构件时，可以为EJB的商业方法选择 `Required, RequiresNew` 等六种不同的事务属性；第二列表示**调用该方法的客户端的事务控制情况**，其中 `None` 表示客户端没有事务控制，`T1` 表示客户端有事务控制；第三列表示**该方法的事务控制规则**，其中 `T2` 表示**容器为该方法的执行维护一个对应的事务**，`T1` 表示**该方法在客户端维护的事务中执行**，`None` 表示**该方法在无事务控制的环境下执行**，`ERROR` 表示**出错情况**。

以表9-1中的前两行为例：
- 第一行表示，如果某方法的事务属性设为 `Required`（第一列为 `Required` ），并且客户端没有事务控制（第二列为 `None`），则容器会为该方法的执行维护一个对应的事务（第三列为 `T2` ），==容器维护的事务可保证：该方法中涉及的所有操作构成一个完整事务（执行过程满足事务特性，如事务的原子性可保证所有操作要么全部执行成功，要么一个都不会执行）==。
- 第二行表示，如果某方法的事务属性设为 `Required`（第一列为 `Required` ），并且客户端有事务控制（第二列为 `T1` ），则该方法在客户端维护的事务中执行（第三列为 `T1` ）。

应注意，**容器维护的事务是方法级的**，即容器默认将一个方法当作一个事务执行；并且，**只有当方法执行的过程中发生了系统级异常，容器才会自动将事务回滚**，即将方法前面执行的结果恢复；==如果发生的异常是用户自定义异常，则容器会认为事务的执行是成功的，不会将事务回滚==。

下面将开发一个名为 `CMT` 的J2EE应用，该应用中包含一个无状态会话构件，该EJB构件实现「银行账户上的存款、取款与余额查询」等操作。由于该构件的每次操作均需调用者提供账户名参数，因此其实例（对象）不需要保存与特定客户端相关的会话状态，可设计为无状态的会话构件。以下简称该EJB构件为账户EJB。
### 1. 定义 `Remote` 接口
`Remote` 接口包含EJB构件实现的商业方法的声明，客户端只能通过 `Remote` 接口访问构件实现的商业方法，不能直接调用。程序9-5给出了账户EJB的 `Remote` 接口定义：
```java
// 程序 9-5 账户EJB的 Remote 接口定义
package bank;
import javax.ejb.*;
import java.rmi.*;
public interface Banker extends EJBObject {
	public void deposit(String accountName, int amount)
		throws RemoteException, BankerFailureException;
	public void withdraw(String accountName, int amount)
		throws RemoteException, BankerFailureException;
	public int getBalance(String accountName)
		throws RemoteException, BankerFailureException;
}
```
在程序9-5中，我们定义了一个名为 `Banker` 的 `Remote` 接口，账户EJB构件向客户端提供三个账户操作—— `deposit` 完成存款功能，`withdraw` 完成取款功能，`getBalance` 完成余额查询功能，`Remote` 接口中包含这三个方法的声明。可以看到，按照EJB规范的约定，接口 `Banker` 继承了接口 `EJBObject` ，三个操作均抛出 `RemoteException` 异常、以报告远程调用错误，参数与返回值均为合法的 `Java RMI` 类型；另外这三个操作还抛出 `BankerFailureExpception` 、以报告操作失败。
### 2. 定义 `Home` 接口
`Home` 接口中包含EJB构件生命周期管理的相关方法，客户程序使用 `Home Interface` 创建、查找或删除EJB的实例。程序9-6给出了账户EJB的 `Home` 接口定义：
```java
// 程序 9-6 账户EJB的 Home 接口定义
package bank;
import java.rmi.RemoteException;
import javax.ejb.*;
public interface BankerHome extends EJBHome {
	public Banker create() throws RemoteException, CreateException;
}
```
在程序9-6中，定义了一个名为 `BankerHome` 的 `Home` 接口，按照EJB规范的约定，接口 `BankerHome` 继承了接口 `EJBHome` 。该接口中仅声明了一个 `create` 方法，其返回值为程序9-5中定义的 `Remote` 接口 `Banker` 。按照EJB规范的约定，`create` 方法抛出 `RemoteException` 和 `CreateException` 异常。
### 3. 定义 `Enterprise Bean` 类
账户EJB的 `Enterprise Bean` 类，首先要按照 `Remote` 接口的约定实现商业方法 `deposit, withdraw, getBalance` ，其次要实现 `Home` 接口中 `create` 方法对应的 `ejbCreate` 方法与会话构件生命周期相关的方法。程序9-7给出了账户EJB的 `Enterprise Bean` 类定义：
```java
// 程序 9-7 账户EJB的 Enterprise Bean 类定义
package bank;
import javax.ejb.*;
import javax.naming.*;
import java.sql.*;
import javax.sql.*;
import java.util.Random;
public class BankerBean implements SessionBean {
	DataSource ds;
	Connection conn;
	public void deposit(String accountName, int amount) throws BankerFailureException {
		try {
			conn = ds.getConnection();
			Statement stmt = conn.createStatement();
			ResultSet res = stmt.executeQuery("SELECT * FROM accounts WHERE accountname = '" + 
				accountName + "'");
			if (res.next()) {
				int newBalance;
				newBalance = res.getInt("balance") + amount;
				stmt.execute("UPDATE accounts SET balance = " + newBalance +
					" WHERE accountname = '" + accountName + "'");
			} else {
				throw new BankerFailureException("invalid accountName");
			}
			conn.close();
		} catch(Exception e) {
			throw new BankerFailureException("invalid accountName");
		}
	}
	
	public void withdraw(String accountName, int amount) throws BankerFailureException {
		try {
			System.out.println("\nEntry withdraw");
			conn = ds.getConnection();
			Statement stmt = conn.createStatement();
			ResultSet res = stmt.executeQuery("SELECT * FROM accounts WHERE accountname = '" + 
				accountName + "'");
			//从账户上减去相应的金额
			int newBalance;
			if (res.next()) {
				if (amount > res.getInt("balance")) {
					throw new BankerFailureException("no enough balance");
				}
				newBalance = res.getInt("balance") - amount;
				stmt.execute("UPDATE accounts SET balance = " + newBalance +
					" WHERE accountname = '" + accountName + "'");
			} else {
				throw new BankerFailureException("invalid accountName");
			}
			conn.close();
			System.out.println(accountName + "'s balance changed to " + newBalance);
			System.out.println("pushing cash...");
			//操纵取款机为用户吐出现金
			pushCash(amount);
			System.out.println("withdraw finished successfully");
		} catch(SQLException e) {
			throw new BankerFailureException("operation failed");
		}
	} 
	
	private void pushCash(int amount) {
		Random rand = new Random();
		int i = Math.abs(rand.nextInt());
		if (i > 1000000000) {
			System.out.println("pushCash failed(" + i +")");
			throw new RuntimeException();
		}
	}
	
	public int getBalance(String accountName) throws BankerFailureException {
		try {
			conn = ds.getConnection();
			Statement stmt = conn.createStatement();
			ResultSet res = stmt.executeQuery("SELECT * FROM accounts WHERE accountname = '" + accountName + "'");
			int curBalance;
			if (res.next()) {
				curBalance = res.getInt("balance");
				conn.close();
				return curBalance;
			} else {
				throw new BankerFailureException("invalid accountName");
			}
		} catch(Exception e) {
			throw new BankerFailureException("operation failed");
		}
	}
	
	public void ejbCreate() throws CreateException {
		try {
			InitialContext initialCtx = new InitialContext();
			ds = (DataSource)initialCtx.lookup("java:comp/env/jdbc/Cloudscape");
		} catch(NamingException ex) {
			throw new CreateException("lookup datasource failed");
		} catch(Exception e) {
			throw new CreateException("operation failed");
		}
	}
	public void ejbRemove() {}
	public void ejbPassivate() {}
	public void ejbActivate() {}
	public void setSessionContext(SessionContext Context) {}
}
```
在程序9-7中，定义了一个名为 `BankerBean` 的 `Enterprise Bean` 类，由于该EJB构件是会话构件，因此 `Enterprise Bean` 类实现（`implements`）`SessionBean` 接口。

`Enterprise Bean` 类首先按照 `Remote` 接口 `Banker` 的约定，实现了商业方法 `deposit, withdraw, getBalance` 。其次，`Enterprise Bean` 类实现了 `Home` 接口 `BankerHome` 中的 `create` 方法对应的 `ejbCreate` 方法和会话构件生命周期相关的其它方法。

为简化应用的结构，在实现这些商业方法时，**账户EJB并没有利用实体构件访问数据库，而是利用数据源，通过编写代码直接完成数据库的访问**。在类 `BankerBean` 的 `ejbCreate` 方法中，我们利用 `JNDI` 服务找到需要使用的数据源，并将其引用保存在数据成员 `ds` 中，由于 `ejbCreate` 方法只会在容器创建无状态会话构件时调用一次，因此数据源的查找操作对于每个账户EJB的对象来说只会执行一次，与「在每次需要使用数据源时均首先查找的方法」相比，本例采用的方法是一种具有更好执行效率的常用方法。

实现 `deposit` 方法时，首先利用保存的数据源引用，获取一个可用的数据库连接；然后利用该连接，执行 `SELECT` 语句定位到参数 `accountName` 所标识的记录。对于 `deposit` 与 `withdraw` 方法，要将记录的余额进行相应的修改。`getBalance` 方法的实现过程与 `deposit` 方法类似，只是不需修改对应的余额。

`withdraw` 方法是本例中重点关注的方法。为演示事务控制，我们为 `withdraw` 方法增加一步吐出现金的操作，可以把本例中的取款操作，想像成在自动取款机上执行取款操作，该操作逻辑上分为修改账户余额与吐出现金两个主要步骤。==从业务逻辑上讲，这两个步骤是一个原子的整体，即这两个步骤要么应全部执行成功，要么一步都不能执行，这种特性可以通过事务控制实现==。实现余额修改的流程，与实现 `deposit` 方法基本类似，首先利用保存的数据源引用，获取一个可用的数据库连接，然后利用该连接，执行 `SELECT` 语句定位到参数 `accountName` 所标识的记录，将记录的余额进行相应的修改。

我们还定义了一个辅助方法 `pushCash` 来模拟吐出现金的过程，在该方法实现中，仅通过产生一个随机数来模拟吐出现金操作，且会以某个概率发生错误：如果产生的随机数大于 $1000000000$ ，方法 `pushCash` 就会产生一个系统级异常—— `RuntimeException` 。请注意，`pushCash` 执行失败时我们并没有编写代码去处理 `pushCash` 之前的余额修改，**我们将通过事务控制来保证系统的正确性，还在 `withdraw` 方法中添加了四条输出语句**（程序中9-7加粗的 `System.out.println` 语句）**以跟踪 `withdraw` 方法的执行情况**：
- 刚进入 `withdraw` 方法，会输出一条信息 `"Entry withdraw"` ；
- 余额修改完成，会输出一条信息 `"xxx's balance changed to nnn"` ；
- 执行 `pushCash` 之前，会输出一条信息 `"pushing cash..."` ；
- `pushCash` 执行成功，会输出一条信息 `"withdraw finished successfully"` 。
- 如果 `pushCash` 执行失败，则不会输出信息 `"withdraw finished successfully"` ，而此时 `pushCash` 方法中会输出一条信息 `"pushCash failed(nnnnnn)"` 。

此外，账户EJB还使用了一个辅助的异常类 `BankerFailureException` ，程序9-8给出了 `BankerFailureException` 类的定义：
```java
// 程序 9-8 BankerFailureException 的类定义
package bank;
public class BankerFailureException extends Exception {
	public BankerFailureException() {}
	public BankerFailureException(String msg) {
		super(msg);
	}
}
```

源代码编写完成后，用Java语言编译器 `javac` 对EJB源代码进行编译。
### 4. 打包/布署EJB
本例中打包与布署EJB构件的过程，与8.2节中打包/布署时间EJB类似，不同的地方主要有几下几点：
- 本例将包含EJB构件的J2EE应用命名为 `CMT`。
- 本例选择EJB的事务控制方式为 `CMT` ，如图9-3所示，**选择事务控制方式为 `"Container Managed"`** ，并**为EJB的每个商业方法选择一个事务属性值**，本例中可以均使用缺省的事务属性值 `"Required"` 。
我们重点关注EJB的 `withdraw` 方法，由于本例的客户端没有事务控制，因此客户端调用 `withdraw` 方法时，容器会为 `withdraw` 方法的执行维护一个事务，该事务可保证 `withdraw` 方法中的所有操作（基本分为修改余额与吐出现金两个步骤）要么全部执行成功，要么一个都不会执行。 
![图9-3 配置事务控制方式为CMT](https://img-blog.csdnimg.cn/bbe71708bf4a4625b67f4540a97c02d9.png)
- 本例需要在图9-4所示的资源工厂配置界面中，配置程序9-7中引用的数据源 `BankDB` ，具体的配置信息为：
	- `Coded Name` ：代码中引用资源工厂时使用的名字，本例中为 `"jdbc/BankDB"` ；
	- `Type` ：资源工厂的类型，本例中为数据源 `javax.sql.DataSource` ；
	- `JDNI Name` ：引用的资源工厂在J2EE平台上对应的实际资源的 `JNDI` 名，本例中仍将账户信息表存储在 `cloudscape` 缺省数据库 `CloudscapeDB` 中，因此此处填写该数据库对应的数据源的 `JNDI` 名 `"jdbc/Cloudscape"` 。
![图9-4 配置数据源](https://img-blog.csdnimg.cn/157dc95940dc42019f462d862b27877b.png)
- 为币值换算EJB配置名为 `"Banker"` 的 `JNDI` 名。
-  布署时，生成名为 `"BankAppClient.jar"` 的客户端 `JAR` 文件包，并将其存放到后面客户端程序所在的目录下。

另外在本例中，我们需要在数据库中创建账户EJB使用的数据库表，并在其中插入一些测试数据。我们可以利用 `"cloudscape –isql"` 命令进入 `cloudscape` 的交互式SQL，通过执行以下SQL语句，在数据库中创建账户EJB使用的账户信息表：
```sql
ij> CREATE TABLE accounts (accountName CHAR(20), balance INTEGER);
```
然后执行以下SQL语句，在刚刚创建的表中插入一条记录，账户名为 `name1` ，余额为 $1000$ ： 
```sql
ij> INSERT INTO accounts VALUES('name1', 1000);
```
### 5. 开发/运行客户端程序
本例中，编写一个名为 `BankerClient` 的Java类作为客户端，该类包含一个入口函数 `main` 。该客户端程序调用账户EJB的 `withdraw` 操作，取款 $100$ 元。客户端程序的代码如程序 9-9 所示。
```java
// 程序 9-9 账户EJB的客户端程序
import bank.*;
import java.rmi.RemoteException;
import javax.ejb.*;
import javax.naming.*;
import javax.rmi.PortableRemoteObject;
public class BankerClient {
	public static void main(String[] args) {
		try {
			Context initial = new InitialContext();
			BankerHome bankerHome = (BankerHome)PortableRemoteObject.narrow(
				initial.lookup("Banker"), BankerHome.class);
			Banker banker = bankerHome.create();
			System.out.println("name1's balance is: " + // 
				banker.getBalance("name1"));
			banker.withdraw("name1", 100); // 取款操作
			System.out.println("name1's balance is: " + //
				banker.getBalance("name1"));
		} catch(RemoteException re) {
			System.out.println("RemoteException : " + re.getMessage());
		} catch(BankerFailureException be) {
			System.out.println("BankerFailureException : " + be.getMessage());
		} catch(CreateException ce) {
			System.out.println("CreateException : " + ce.getMessage());
		} catch(Exception e) {
			System.out.println("Get Exception: " + e.getMessage());
		}
		
		try {
			Context initial = new InitialContext();
			BankerHome bankerHome = (BankerHome)PortableRemoteObject.narrow(
				initial.lookup("Banker"), BankerHome.class);
			Banker banker = bankerHome.create();
			System.out.println("name1's balance is: " + //
				banker.getBalance("name1"));
		} catch(RemoteException re) {
			System.out.println("RemoteException : " + re.getMessage());
		} catch(BankerFailureException be) {
			System.out.println("BankerFailureException : " + be.getMessage());
		} catch(CreateException ce) {
			System.out.println("CreateException : " + ce.getMessage());
		} catch(Exception e) {
			System.out.println("Get Exception: " + e.getMessage());
		}
	}
}
```
在程序9-9所示的客户端程序中，首先利用 `JNDI` 服务查找账户EJB对应的 `Home` 接口，查找到 `Home` 接口后，客户端调用 `Home` 接口中的 `create` 操作来获取一个可用的EJB，**该操作返回一个 `Remote` 接口，客户端利用返回的 `Remote` 接口调用EJB上的操作**。

在客户端程序中，三次调用 `getBalance` 操作并将返回的余额信息输出（程序9-9的 `System.out.println` 语句）以跟踪账户的变化情况，第一次是在调用 `withdraw` 方法取款 $100$ 元之前，第二次是在调用 `withdraw` 成功完成后，第三次是在客户端程序退出之前。由于 `withdraw` 方法的执行可能出错（ `pushCash` 方法可能抛出 `RuntimeException` ），因此第二次的输出语句可能不执行。

运行客户端程序时，如果服务端 `withdraw` 方法的执行没有错误，我们可以看到账户 `name1` 中余额减少了 $100$ 元，对应的客户端与服务端输出信息如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/35f76e1315ce4af1807412040441383a.png)
如果运行客户端程序时，服务端 `withdraw` 方法的执行出现错误，可以看到账户 `name1` 中余额并没有减少，==尽管在服务端输出的信息中我们看到余额已经修改，但是容器维护的事务保证了吐出现金操作失败时余额的修改是无效的==，此时对应的客户端与服务端输出信息如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/e5ba50c8ce7b49da879a20fde1623c56.png)
可从 `cloudscape` 的交互式SQL中，通过执行以下SQL语句确认数据库中的数据。

应注意，`withdraw` 方法执行错误时，抛出的异常是 `RuntimeException` ，属于系统级异常，因此容器会自动将 `withdraw` 方法对应的事务回滚，**如果执行过程中抛出的异常不是系统级异常**（如将程序9-7中 `RuntimeException` 改为 `BankFailureException` ），**则容器会认为事务的执行是成功的，从而将事务提交**。有兴趣可以修改程序9-7，并进行相关测试。
## 9.2.2 基于 `BMT` 的事务控制
**`BMT` 方式下，程序员在EJB的源程序中控制事务边界（如事务开始、回滚、提交等），并在部署描述符中指定由 `Bean` 控制事务的边界**。与 `CMT` 方式相比，`Bean` 维护的事务可以跨越方法的边界，因此通常具有更好的灵活性。

本小节通过修改上一小节的例子，演示如何基于 `BMT` 方式实现事务控制。

我们通过修改程序9-7中的 `withdraw` 方法，在其中加入事务边界控制的代码，同时修改 `pushCash` 方法使其抛出用户自定义异常 `BankerFailureException` ，其它的实现保持不变。程序9-10给出了修改后的 `withdraw` 方法与 `pushCash` 方法。
```java
// 程序 9-10 修改后的 withdraw 方法与 pushCash 方法
//其它实现代码
public void withdraw(String accountName, int amount) throws BankerFailureException {
	System.out.println("Entry withdraw");
	try {
		System.out.println("Getting transaction object...");
		UserTransaction userTrx = sessionCtx.getUserTransaction();
		userTrx.begin(); // 开始事务
		System.out.println("user transaction begin...");
	} catch(Exception exc) {
		throw new BankerFailureException("transaction not begin");
	}
	try {
		conn = ds.getConnection();
		Statement stmt = conn.createStatement();
		ResultSet res = stmt.executeQuery("SELECT * FROM accounts WHERE accountname = '" 
			+ accountName + "'");
		//从账户上减去相应的金额
		int newBalance;
		if (res.next()) {
			if (amount > res.getInt("balance")) {
				throw new BankerFailureException("no enough balance");
			}
			newBalance = res.getInt("balance") - amount;
			stmt.execute("UPDATE accounts SET balance = " + newBalance);
		}
		else {
			throw new BankerFailureException("invalid accountName");
		}
		conn.close();
		System.out.println(accountName + "'s balance changed to " + newBalance);
		System.out.println("pushing cash...");
		//操纵取款机为用户吐出现金
		pushCash(amount);
		System.out.println("withdraw finished successfully");
		try {
			UserTransaction userTrx = sessionCtx.getUserTransaction();
			userTrx.commit(); // 提交事务
			System.out.println("user transaction committed");
		} catch(Exception exc) {
			throw new BankerFailureException("transaction commit failed");
		}
	} catch(Exception e) {
		System.out.println("Exception caught, try rollback");
		try {
			UserTransaction userTrx = sessionCtx.getUserTransaction();
			userTrx.rollback(); // 回滚事务
			System.out.println("user transaction rollbacked");
		} catch(Exception exc) {
			throw new BankerFailureException("transaction rollback failed");
		}
		throw new BankerFailureException("operation failed");
	}
}
private void pushCash(int amount) throws BankerFailureException {
	Random rand = new Random();
	int i = Math.abs(rand.nextInt());
	if (i > 1000000000) {
		System.out.println("pushCash failed(" + i +")");
		throw new BankerFailureException();
	}
}
//其它实现代码
```
在程序9-10给出的代码中，`withdraw` 方法共增加了三处**事务边界控制代码**：
- 刚刚进入 `withdraw` 方法，利用 `JTA` 中的 `begin` 方法控制事务开始。
- 吐出现金操作成功完成后，利用 `JTA` 中的 `commit` 方法控制事务提交。
- 捕获到异常后，利用 `JTA` 中的 `rollback` 方法控制事务回滚。

修改完毕后，将EJB重新编译，我们可以重新创建一个J2EE应用，并将修改后的EJB构件重新打包，打包过程与上一小节的过程类似，主要区别就是**在图9-5所示的事务控制界面，选择由 `Bean` 而不是容器来控制事务**。
![图9-5 设置事务控制方式为BMT](https://img-blog.csdnimg.cn/d4aa56b642a641aab09012fc974b80a7.png)
运行上一小节同样的客户端程序，我们可以看到当 `pushCash` 操作执行发生异常时，由于引发的异常被 `withdraw` 方法中的 `catch` 语句捕获到，进而调用了 `JTA` 的 `rollback` 操作控制事务回滚，从而使数据库中的余额恢复到了 `withdraw` 操作执行前的状态。一种可能的客户端与服务端输出如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2c55c8a4322d4288aa240f170a971c0c.png)
应注意，**使用 `BMT` 控制事务时，应保证「程序中的所有控制流出口」均将事务结束（提交或回滚）**，因为如果事务未成功结束，往往导致数据库中的数据被加锁，从而使得应用无法继续访问被锁定的数据。

---
# 9.3 安全性控制
==J2EE中的安全性控制，分为认证与授权两个级别，实现安全性控制的常用方式是声明==。声明方式的安全性控制完全由容器自动控制，开发人员主要通过**配置安全性角色**与**授权规则**来定制安全性控制的规则。

典型的声明性安全性控制，需要在部署描述符中描述安全性控制：
- **在J2EE应用的部署描述符中，定义应用所使用的安全性角色**，角色为应用系统所指定的逻辑用户组，如经理角色、职员角色等；
- **在Web模块中描述认证方式**：选择如何进行用户身份验证；
- **在Web模块中描述安全性规则**（授权规则）：设置Web模块中的构件（如 `JSP` 、`Servlet` 、静态页面等）的访问权限；
- **在EJB模块中描述安全性规则**（授权规则）：设置EJB模块中EJB构件的访问权限；
- **部署时，将定义的安全性角色映射到实际的安全域中**：将实际用户管理系统中的用户映射到角色。

本节通过一个相对完整的J2EE应用，演示声明性安全性控制的配置过程与运行结果，将给出该应用中所有构件的源代码，但重点是**打包过程中安全性配置相关的内容**。本例中我们将开发一个简单的银行应用，==客户通过Web访问Web模块中的 `servlet` 构件，该 `servlet` 构件调用「后台的EJB构件提供的服务」完成客户请求==。**我们将在Web端进行认证和授权控制，在EJB端进行授权控制**。以下简称本例中开发的应用为银行应用。
## 9.3.1 构建银行应用
### 1. 应用结构与关键代码
我们将开发一个名为 `NewBankApp` 的J2EE应用，该应用中包含以下模块：
- EJB模块 `BankBeanJAR` ，该模块中包含三个EJB构件：
	- `AccountManager` 构件：一个无状态会话构件，完成银行账户管理员的任务，提供开户、存款、取款、查询余额等操作；
	- `AccountBean` 构件：一个 `EJB 2.0` 的 `CMP` 实体构件，对应数据库中的账户记录表，该表中记录了系统中已有的账户信息；
	- `LogBean` 构件：一个 `EJB 2.0` 的 `CMP` 实体构件，对应数据库中的日志记录表，该表用于记录日志信息；
- Web模块 `BankWeb` ，该模块中包含一个 `Servlet` 构件：
	- `BankServlet` 构件：响应客户的HTTP请求，调用 `AccountManage` 的操作。

该应用中构件之间的交互关系如图9-6所示：
![图9-6 银行应用中构件之间的交互关系](https://img-blog.csdnimg.cn/8080839e39ae4a0e96f64a3fe196a5af.png)
运行时，`Servlet` 构件响应客户端的HTTP请求，并调用会话构件 `AccountManager` 完成客户端请求，会话构件 `AccountManager` 响应 `Servlet` 请求时，会调用实体构件 `AccountBean` 完成对账户信息的操作，并调用实体构件 `LogBean` 记录系统日志。

在将应用打包后，我们将**重点控制 `BankServlet` 构件与 `AccountManager` 构件的相关授权规则**。该应用中各构件的源代码见下。

`Servlet` 构件 `BankServlet` 通过会话构件 `AccountManager` 的 `Remote` 接口调用账户管理员的相关操作，程序9-11给出了会话构件 `AccountManager` 的 `Remote` 接口、`Home` 接口、`Enterprise Bean` 类定义，其中声明和实现了存款 `deposit` 、取款 `withdraw` 、查询余额 `getBalance` 、计算利息 `calculateInterest` 、开户 `createAccount` 与销户 `removeAccount` 六个操作。
```java
// 程序 9-11 会话构件 AccountManager 的 Remote 接口定义
package banking;
import javax.ejb.*;
import java.rmi.*;
public interface AccountManager extends EJBObject {
	public void deposit(String customerName, double amount) throws RemoteException;
	public double withdraw(String customerName, double amount)
		throws RemoteException, InsufficientFundsException;
	public double getBalance(String customerName) throws RemoteException;
	public double calculateInterest(String customerName) throws RemoteException;
	public Account createAccount(String customerName, double initialBalance)
		throws NoAccountCreatedException, RemoteException;
	public void removeAccount(String customerName)
		throws NoAccountCreatedException, RemoteException;
}

// 会话构件 AccountManager 的 Home 接口定义
package banking;
import java.rmi.RemoteException;
import javax.ejb.*;
public interface AccountManagerHome extends EJBHome {
	public AccountManager create() throws RemoteException, CreateException;
}

// 会话构件 AccountManager 的 Enterprise Bean 类定义
package banking;
import javax.ejb.*;
import javax.naming.*;
import java.util.*;
import javax.rmi.*;
import java.rmi.RemoteException; 
public class AccountManagerBean implements SessionBean {
	private SessionContext context;
	public static final String DEPOSIT = "deposit funds";
	public static final String WITHDRAW = "withdraw funds";
	public static final String GETBALANCE = "get an account balance";
	public static final String CALCULATEINTEREST = "calculate interest on an account";
	public static final String CREATEACCOUNT = "create an account";
	public static final String CLIENT = "Client";
	public static final String BANK = "Bank";
	
	public AccountManagerBean() {}
	public void ejbRemove() {}
	public void ejbPassivate() {}
	public void ejbActivate() {}
	public void setSessionContext(SessionContext ctx) {
		context = ctx;
	}
	public void ejbCreate() {}
	
	//business method
	public void deposit(String customerName, double amount) {
		logActivity(DEPOSIT, customerName);
		try {
			Account account = getAccount(customerName);
			account.deposit(amount);
		} catch(NoSuchAccountException e) {
			throw new EJBException();
		} catch(RemoteException ex) {
			throw new EJBException();
		}
	}
	public double withdraw(String customerName, double amount)
		throws InsufficientFundsException {
		double newBal;
		logActivity(WITHDRAW, customerName);
		try {
			Account account = getAccount(customerName);
			double bal = account.getBalance();
			if (amount > bal) {
				throw new InsufficientFundsException("Account does not have " + amount);
			}
			newBal = account.withdraw(amount);
		} catch(NoSuchAccountException e) {
			throw new EJBException();
		} catch(RemoteException ex) {
			throw new EJBException();
		}
		return newBal;
	}
	public double getBalance(String customerName) {
		System.out.println("Entry getBalance");
		logActivity(GETBALANCE, customerName);
		System.out.println("after logActivity");
		double bal = 0;
		try {
			Account account = getAccount(customerName);
			System.out.println("after getAccount"); 
			bal = account.getBalance();
			System.out.println("after getBalance");
		// } catch(AccountAccessDeniedException AE) {
		// 	throw new AccountAccessDeniedException("Access Denied");
		} catch(NoSuchAccountException e) {
			throw new EJBException();
		} catch(RemoteException ex) {
			throw new EJBException();
		}
		return bal;
	}
	public double calculateInterest(String customerName) {
		logActivity(CALCULATEINTEREST, customerName);
		try {
			Account account = getAccount(customerName);
			return account.calculateInterest();
		} catch(NoSuchAccountException e) {
			throw new EJBException();
		} catch(RemoteException ex) {
			throw new EJBException();
		}
	}
	
	public Account createAccount(String customerName, double initialBalance)
		throws NoAccountCreatedException {
		logActivity(CREATEACCOUNT, customerName);
		try {
			AccountHome accountHome = getAccountHome();
			return accountHome.create(customerName, initialBalance);
		} catch(CreateException ce) {
			throw new NoAccountCreatedException(ce.getMessage());
		} catch(RemoteException ex) {
			throw new EJBException();
		}
	}
	public void removeAccount(String customerName) throws NoAccountCreatedException {
		try {
			Account account = getAccount(customerName);
			account.remove();
		} catch(NoSuchAccountException e) {
			throw new EJBException();
		} catch(RemoveException re) {
			throw new EJBException();
		} catch(RemoteException ex) {
			throw new EJBException();
		}
	}
	
	private void logActivity(String method, String customer) {
		this.log(this.context.getCallerPrincipal().getName() + "tried to " +
			method + " for " + customer);
	}
	
	private Account getAccount(String customerName) throws NoSuchAccountException {
		try {
			System.out.println("Entry getAccount");
			AccountHome accountHome = getAccountHome();
			System.out.println("after getAccountHome");
			return accountHome.findByPrimaryKey(new String(customerName));
		} catch(RemoteException ex) { 
			throw new EJBException();
		} catch(FinderException fe) {
			throw new NoSuchAccountException("No such account");
		}
	}
	
	private AccountHome getAccountHome() {
		try {
			InitialContext initial = new InitialContext();
			Object objref = initial.lookup("java:comp/env/ejb/account");
			AccountHome home = (AccountHome)javax.rmi.PortableRemoteObject.narrow
				(objref, AccountHome.class);
			return home;
		} catch(NamingException ne) {
			throw new EJBException();
		}
	}
	
	private void log(String msg) {
		try {
			InitialContext ctx = new InitialContext();
			Object ref = ctx.lookup("java:comp/env/ejb/log");
			LogHome h = (LogHome)javax.rmi.PortableRemoteObject.narrow(ref, LogHome.class);
			h.create(msg);
		} catch(Exception e) {}
	}
}
```
程序9-12给出了实体构件 `AccountBean` 的 `Remote` 接口、`Home` 接口、`Enterprise Bean` 类定义：
```java
// 程序 9-12 实体构件 AccountBean 的 Remote 接口定义
package banking;
import javax.ejb.*;
import java.rmi.*;
public interface Account extends EJBObject {
	public double getBalance() throws RemoteException;
	public void deposit(double amount) throws RemoteException;
	public double withdraw(double amount) throws RemoteException, InsufficientFundsException;
	public double calculateInterest() throws RemoteException;
}

// 实体构件 AccountBean 的 Home 接口定义
package banking;
import java.rmi.RemoteException;
import javax.ejb.*;
public interface AccountHome extends EJBHome {
	public Account create(String customerName, double currentBalance)
		throws RemoteException, CreateException;
	public Account findByPrimaryKey(String key) throws RemoteException, FinderException;
}

// 实体构件 AccountBean 的 Enterprise Bean 类定义
package banking;
import javax.ejb.*;
import javax.naming.*; 
public abstract class AccountBean implements EntityBean {
	private EntityContext context;
	private static final double INTEREST = 0.01;
	public AccountBean() {}
	// CMP fields
	public abstract double getCurrentBalance();
	public abstract void setCurrentBalance(double orderID);
	public abstract String getCustomerName();
	public abstract void setCustomerName(String customerName);
	
	// business method
	public double getBalance() {
		return getCurrentBalance();
	}
	public void deposit(double amount) {
		setCurrentBalance(getCurrentBalance() + amount);
	}
	public double withdraw(double amount) throws InsufficientFundsException {
		if (getCurrentBalance() < amount) {
			throw new InsufficientFundsException("Account does not have " + amount);
		} else {
			setCurrentBalance(getCurrentBalance() - amount);
			return getCurrentBalance();
		}
	}
	public double calculateInterest() {
		setCurrentBalance(getCurrentBalance() + (getCurrentBalance() * INTEREST));
		return getCurrentBalance();
	}
	
	public String ejbCreate(String customerName, double currentBalance)
		throws CreateException {
		setCustomerName(customerName);
		setCurrentBalance(currentBalance);
		return null;
	}
	
	public void ejbPostCreate(String customerName, double currentBalance) {}
	public void ejbActivate() {}
	public void ejbLoad() {}
	public void ejbPassivate() {}
	public void ejbRemove() {}
	public void ejbStore() {}
	public void setEntityContext(EntityContext ctx) {
		context = ctx;
	} 
	public void unsetEntityContext() {
		context = null;
	}
}
```
程序9-13给出了实体构件 `LogBean` 的 `Remote` 接口、`Home` 接口、`Enterprise Bean` 类定义：
```java 
// 程序 9-13 实体构件 LogBean 的 Remote 接口定义
package banking;
import javax.ejb.*;
import java.rmi.*;
public interface Log extends EJBObject { 	
	public void log(String msg) throws RemoteException;
}

// 实体构件 LogBean 的 Home 接口定义
package banking;
import java.rmi.RemoteException;
import javax.ejb.*;
public interface LogHome extends EJBHome {
	public Log create(String msg) throws RemoteException, CreateException;
	public Log findByPrimaryKey(String key) throws RemoteException, FinderException;
}

// 实体构件 LogBean 的 Enterprise Bean 类定义
package banking;
import javax.ejb.*;
import javax.naming.*;
public abstract class LogBean implements EntityBean {
	private EntityContext context;
	public LogBean() {}
	// CMP fields
	public abstract String getLogMessage();
	public abstract void setLogMessage(String logMessage);
	
	// business method
	public String ejbCreate(String msg) throws CreateException {
		setLogMessage(msg);
		return null;
	}
	public void log(String msg){
		setLogMessage(msg);
	}
	
	public void ejbPostCreate(String msg) {}
	public void ejbActivate() {}
	public void ejbLoad() {}
	public void ejbPassivate() {}
	public void ejbRemove() {}
	public void ejbStore() {}
	public void setEntityContext(EntityContext ctx) {
		context = ctx;
	}
	public void unsetEntityContext() {
		context = null;
	}
}
```
程序9-14给出了一些辅助异常的定义：
```java 
// 程序 9-14 辅助异常 AccountAccessDeniedException 的类定义
package banking;
public class AccountAccessDeniedException extends Exception {
	public AccountAccessDeniedException(String msg) {
		super(msg);
	}
}

// 辅助异常 InsufficientFundsException 的类定义
package banking;
public class InsufficientFundsException extends Exception {
	public InsufficientFundsException(String msg) {
		super(msg);
	}
}

// 辅助异常 NoAccountCreatedException 的类定义
package banking;
public class NoAccountCreatedException extends Exception {
	public NoAccountCreatedException(String msg) {
		super(msg); 
	}
}

// 辅助异常 NoSuchAccountException 的类定义
package banking;
public class NoSuchAccountException extends Exception {
	public NoSuchAccountException(String msg) {
		super(msg);
	}
}
```
`Servlet` 构件 `BankServlet` 响应客户端的HTTP请求，程序9-15给出了 `Servlet` 构件的类定义：
```java
// 程序 9-15 Servlet 构件 BankServlet 的类定义
package bankWeb;
import banking.*;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*; 
import java.rmi.RemoteException;
import javax.ejb.*;
import javax.naming.*;
import javax.rmi.PortableRemoteObject;
public class BankServlet extends HttpServlet {
	public void doGet(HttpServletRequest req, HttpServletResponse resp)
		throws ServletException, IOException {
		resp.setContentType("text/html");
		PrintWriter out = resp.getWriter();
		out.println("<HTML>");
		out.println("<HEAD><TITLE>BankServlet</TITLE></HEAD>");
		out.println("<BODY>");
		try {
			Context initial = new InitialContext();
			AccountManagerHome accountManagerHome = (AccountManagerHome)
				initial.lookup("java:comp/env/ejb/AccountManager");
			AccountManager accountManager = accountManagerHome.create();
			out.println("get name1's balance...<br>");
			out.println("name1's balance is: " + accountManager.getBalance("name1") + "<br>");
			Account account = accountManager.createAccount("newAccount", 1000.00f);
			out.println("createAccount newAccount successfully<br>");
			accountManager.removeAccount("newAccount");
			out.println("removeAccount successfully<br>");
		} catch(RemoteException re) {
			out.println("RemoteException : " + re.getMessage());
		} catch(NoAccountCreatedException nace) {
			out.println("NoAccountCreatedException : " + nace.getMessage());
		} catch(CreateException ce) {
			out.println("CreateException : " + ce.getMessage());
		} catch(Exception e) {
			out.println("Get Exception: " + e.getMessage());
		}
		out.println("</BODY>");
		out.println("</HTML>");
		out.close();
	}
}
```
程序9-15定义的 `BankServlet` 实现了一个 `doGet` 方法，**当客户端通过 `HTTP GET` 方法访问 `BankServlet` 时，其 `doGet` 方法会被调用，`doGet` 方法为客户端动态生成一个 `HTML` 页面**。

首先利用 `JNDI` 服务，查找会话构件 `AccountManager` 的 `Home` 接口，然后调用 `Home` 接口中的 `create` 方法获取一个可用的EJB对象—— `Remote` 接口的引用。最终动态生成的页面中，显示了调用 `Remote` 接口中操作的调用结果：
- 调用 `getBalance` 之前，在页面上打印信息 `"get name1's balance…"`
- 调用 `getBalance` 并在页面上打印信息 `"name1's balance is nnn"`
- 调用 `createAccount` 新开账户 `newAccount` ，并在调用完成后在页面上打印信息 `"createAccount newAccount successfully"`
- 调用 `removeAccount` 删除账户 `newAccount` ，并在调用完成后在页面上打印信息 `"removeAccount successfully"`
### 2. 打包测试应用
打包银行应用的过程分为以下几个主要步骤：
#### (1) 创建应用
创建名为 `NewBankApp` 的J2EE应用。
#### (2) 打包EJB构件
将会话构件 `AccountManager` 、实体构件 `AccountBean` 与 `LogBean` 打包在同一个EJB模块中，主要过程如下：
- 打包实体构件 `AccountBean` ：
	- 创建一个名为 `BankBeanJAR` 的EJB模块，将应用中三个EJB构件对应的 `Remote` 接口、`Home` 接口与 `Enterprise Bean` 类以及相关的辅助异常类全部加入到该EJB模块中；
	- 设置该EJB为实体构件 `Entity` ；
	- 设置该EJB的持久性特性：类型为 `EJB 2.0` 的 `CMP` 构件，两个数据成员 `customerName` 与 `currentBalance` 均与数据库中字段对应，主键类型为 `java.lang.String` ，主键字段为  `customerName` ；
	- 数据库设置：使用数据源 `"jdbc/Cloudscape"` ，生成缺省的SQL语句即可；
	- 配置名为 `"account"` 的 `JNDI` 名。
- 打包实体构件 `LogBean` ：
	- 将该EJB添加到EJB模块 `BankBeanJAR` 中；
	- 设置该EJB为实体构件 `Entity` ；
	- 设置该EJB的持久性特性：类型为 `EJB 2.0` 的 `CMP` 构件，数据成员 `logMessage` 与数据库中字段对应，主键类型为 `java.lang.String` ，主键字段为 `logMessage` ；
	- 数据库设置：使用数据源 `"jdbc/Cloudscape"` ，生成缺省的SQL语句即可；
	-  配置名为 `"log"` 的 `JNDI` 名。
- 打包会话构件 `AccountManager` ：
	- 将该EJB添加到EJB模块 `BankBeanJAR` 中；
	- 设置该EJB为无状态会话构件；
	- **添加对实体构件 `AccountBean` 与 `LogBean` 的引用**，正确填写对应的信息（填写信息的规则参考8.4.2中订单EJB的打包）；
	- 配置名为 `"AccountManager"` 的 `JNDI` 名。

#### (3) 打包 `Servlet` 构件
打包 `Servlet` 构件 `BankServlet` 在名为 `BankWeb` 的Web模块中，主要过程如下： 
- 如图9-7所示，点击 `deploytool` 的 `"File/New/Web Component"` 打开打包Web构件的向导。
![图9-7 打开打包Web构件向导](https://img-blog.csdnimg.cn/ec0c8533149b42418ada826ecf563f28.png)
- 在图9-8所示的界面中，选择创建一个名为 `BankWeb` 的Web模块，并将 `BankServer` 对应的类文件 `BankServlet.class` 加入到该模块中。
![图9-8 创建Web模块](https://img-blog.csdnimg.cn/6154918d0b9d444080f5e9b1c9cb15a1.png)
- 在图9-9所示的界面中，选择打包的构件为 `Servlet` 构件：
![图9-9 选择构件类型](https://img-blog.csdnimg.cn/6497c4f47fd34e27b04d7082bb7bf5a4.png)
- 在图9-10所示的界面中，选择 `Servlet` 构件的实现类为 `bankWeb.BankServlet` ：
![图9-10 选择Servlet构件的实现类](https://img-blog.csdnimg.cn/986731bc63814b28bb5b2226b0445695.png)
- 在图9-11所示的界面中，为该 `Servlet` 配置一个名为 `"BankServlet"` 的别名，**别名是客户端以HTTP方式访问 `Servlet` 时、在URL中使用的名字，打包时应至少为 `Servlet` 配置一个别名**。
![图9-11 为Servlet配置别名](https://img-blog.csdnimg.cn/aa2d92bc807e408fbb176474014221fe.png)
- 在图9-12所示的界面中，添加对会话构件 `AccountManager` 的引用（EJB引用的填写规则参考8.4.2中订单EJB的打包）： ![图9-12 添加对会话构件AccountManager的引用](https://img-blog.csdnimg.cn/00de794b49c34c75bb47b1251d41a3b1.png)
- 向导中其它界面均采用缺省设置即可。
- 打包Web构件的向导完成后，如图9-13所示，在 `deploytool` 中选中正在打包的J2EE应用，点击右侧的 `"Web Context"` 选项卡，为Web模块 `BankWeb` 配置一个名为 `"bankweb"` 的上下文根，**上下文根为当前Web模块中所有构件提供一个上下文的根，在URL中上下文根通常放在构件别名的前面、用来共同标识对应构件**。![图9-13 为Web模块配置上下文根](https://img-blog.csdnimg.cn/0e8632df53da4064969dcb3dcf1d4400.png)

在上述的打包过程中，我们并没有对应用的安全性进行任何的配置，因此**布署后该应用中的构件可以被无限制的访问**。==可将此状态下的应用进行布署，因为此应用的客户端通过HTTP访问应用，并不需要编写任何客户端程序，因此布署时不需生成客户端使用的 `JAR` 程序包==。

布署完成后，我们需要在实体构件 `AccountBean` 对应的数据库表中，插入一条记录来支持后续的测试，进入 `cloudscape` 的交互式SQL，执行以下SQL语句，可在实体构件 `AccountBean` 对应数据库表 `AccountBeanTable` 中插入测试需要的数据：
```sql
ij> INSERT INTO "AccountBeanTable" VALUES(1000,'name1');
```
打开浏览器，在地址栏中输入URL来访问银行应用中的 `BankServlet` ，该URL为 `http://localhost:8000/bankweb/BankServlet` ，由三部分组成：
- **主机名＋端口号信息**：`"localhost:8000"` ，用于标识J2EE平台的Web服务器；
- **上下文根**：`"bankweb"` ，需要访问的构件所在Web模块的上下文根；
- **构件别名**：`"BankServlet"` ，构件的别名。

客户端访问 `BankServlet` 时看到的页面中，显示了该 `Servlet` 对会话构件 `AccountManager` 提供的商业方法的调用结果，所显示的页面如图9-14所示：
![图9-14 客户端输出](https://img-blog.csdnimg.cn/01f855a9f87949b0b30b32c4bc13b34c.png)
### 3. 为应用增加安全性配置
下面为银行应用添加安全性配置，==我们将该应用的访问者均分为普通用户与超级用户两个角色==：
- 配置这两个角色均可以访问应用的 `BankServlet` 构件；
- 同时，配置这两个角色均可以访问会话构件 `AccountManager` 的 `getBalance` 方法；
- 只有超级用户角色才能访问会话构件 `AccountManager` 的 `createAccount` 与 `removeAccount` 方法。

配置的主要过程如下。
#### (1) 配置应用使用的安全性角色
如图9-15所示，在 `deploytool` 中选中刚刚打包的J2EE应用，点击右侧的 `"Security"` 选项卡，点击右下角的 `"Edit Roles"` 按钮：
![图9-15 配置应用使用的安全性角色](https://img-blog.csdnimg.cn/9e62c1bd51944346a555d618a3197b5d.png)
在图9-16所示的界面中，为银行应用添加 `"NormalUser"` 与 `"Supervisor"` 两个安全性角色。
![图9-16 添加安全性角色](https://img-blog.csdnimg.cn/ff082c9d33764a67aeb90d063071c2bf.png)
#### (2) 配置认证方式
如图9-17所示，在 `deploytool` 中选中银行应用中的Web模块 `BankWeb` ，点击右侧的 `"Security"` 选项卡，在 `"User Authentication Method"` 下面的下拉列表框中选择基本的认证方式 `Basic` 。可以选择的许可证方式包括：
- `Basic` ：**基本认证方式**。该方式下，用户第一次访问系统时，系统会弹出一个对话框，提示访问者输入用户名和密码。本例中采用该方式。
- `Client Certificate` ：**电子证书方式**。该方式下，用户访问时向服务器提供一个表示身份的电子证书、以进行身份验证。
- `Form Based` ：**表单方式**。该方式下，由开发人员提供登录页面来验证用户身份。
![图9-17 配置认证方式](https://img-blog.csdnimg.cn/0bcae6953de147b9855bad98a100f5f0.png)
#### (3) 配置Web模块的授权规则
如图9-18所示，在 `deploytool` 中选中银行应用中的Web模块 `BankWeb` ，点击右侧的 `"Security"` 选项卡，点击 `"Security Constraints"` 右侧的 `"Add"` 按钮，添加一条安全性规则。
![图9-18 添加一条安全性规则](https://img-blog.csdnimg.cn/eea430188a164b63aa502d769fe8863b.png)
如图9-19 所示，点击 `"Web Resource Collections"` 右侧的 `"Add"` 按钮，添加安全性规则所施加的一个Web资源集合。**一条安全性规则可施加于多个Web资源集合**。然后点击界面下侧中部的 `"Edit"` 按钮，在弹出的图9-20所示的界面中，点击 `"Add URL Pattern"` 按钮添加 `BankServlet` 的别名对应的URL `"/BankServlet"` 。**由于Web模块中的安全性规则仅施加于 `Servlet` 构件 `BankServlet` ，因此只需在当前Web资源集合中添加该 `Servlet` 对应的 `URL` 即可**。
![图9-19 添加一个Web资源集合](https://img-blog.csdnimg.cn/da8cfb2bd2b84d3e89beb39662a84b2f.png)![图9-20 在Web资源集合添加BankServlet的URL](https://img-blog.csdnimg.cn/58e3d825a99744d0bbe2b5fc06e9eb24.png)
如图9-21所示，选中 `"Get"` 复选框以控制 `BankServlet` 构件的 `HTTP GET` 方式访问，点击界面右下角的 `"Edit"` 按钮，在弹出的图9-22所示的界面中，**将 `NormalUser` 与 `Supervisor` 两个角色均加入到授权角色列表中，以使得这两个角色都可以访问 `BankServlet`** 。
![图9-21 选择控制HTTP GET方式访问](https://img-blog.csdnimg.cn/ea2aa7c690de407ba9bc4d6f51fb1e70.png)![图9-22 添加可以访问的角色](https://img-blog.csdnimg.cn/04139d796e3a4c3c89978e57e24872bf.png)
如图9-23所示，在 `deploytool` 中选中 `BankServlet` 构件，点击右侧的 `"Security"` 选项卡，**设置 `BankServlet` 的安全身份（`Security Identity`）为使用调用者的身份（`Use Caller ID`）**。
![图9-23 设置BankServlet的安全身份](https://img-blog.csdnimg.cn/9ff86f7037914b0dbced7aebb3ac5396.png)
**安全身份**指J2EE应用中**构件在访问其它构件时**，如本例中的 `BankServlet` 构件访问会话构件 `AccountManager` 的情形，**该构件（`BankServlet`）的访问身份**。安全身份有两种选择：
- `Use Caller ID` ：**使用访问者的身份**，使用此种设置时，构件（`BankServlet`）将使用「访问自己的访问者的身份」来访问其它构件（`AccountManager`）。
- `Run As Role` ：**使用固定的身份访问其它构件**，使用此种设置时，不论构件自己（`BankServlet`）的访问者是否认证，构件都将使用「指定的固定的身份」访问其它构件（`AccountManager`）。

#### (4) 配置EJB模块的授权规则
如图9-24所示，在 `deploytool` 中选中会话构件 `AccountManager`，点击右侧的 `"Security"` 选项卡来**配置该构件的安全性规则**。构件 `AccountManager` 的安全身份使用缺省值。选中构件的 `Remote` 接口，可以看到所有方法的缺省授权均为所有人可以访问 `All Users` ，将 `getBalance` 方法的授权规则设为可以由 `NormalUser` 与 `Supervisor` 访问，而 `createAccount` 与 `removeAccount` 方法的授权规则设为只能由 `Supervisor` 访问。
![图9-24 配置会话构件的安全性规则](https://img-blog.csdnimg.cn/6e2ae65b7b624173afea9903dc4316cd.png)
#### (5) 将安全性角色映射到实际的安全域
最后一步的配置是，**将安全性角色映射到实际的安全域**，不同的J2EE平台支持不同的安全域，如很多平台支持使用操作系统的用户管理系统，J2EE参考实现允许使用「平台自带的用户管理系统」。点击 `deploytool` 的 `"Tools\Server Configuration"` 菜单，打开服务器配置界面，如图9-25所示，选中 `"J2EE Server"` 下的 `Users` ，可以配置应用可使用的用户信息。本例中为银行应用增加 `managerwang` 、`managerli` 与 `user1` 三个用户（增加用户后，需要重新启动J2EE服务器，以使得修改的信息生效）。
![图9-25 增加银行应用使用的用户](https://img-blog.csdnimg.cn/bfe356f6ed6d4ef58ff4c7f70aa9be2c.png)
创建好应用使用的用户之后，我们就可以**将定义的安全性角色映射到实际的用户了**。如图9-26所示，在 `deploytool` 中选中银行应用 `NewBankApp` ，点击右侧的 `"Security"` 选项卡，选中要映射的安全性角色，并点击下面的 `"Add"` 按钮，在图9-27所示的弹出界面中添加要映射的用户。本例中我们将用户 `managerwang` 与 `managerli` 映射到角色 `Supervisor` ，将用户  `user1` 映射到角色 `NormalUser` 。
![图9-26 为角色映射用户](https://img-blog.csdnimg.cn/abb9987bf0ca4d198ef1adb4e9e1854a.png)![图9-27 选择要映射的用户](https://img-blog.csdnimg.cn/1c85d3094f2845cbaef226b4cc6dbb79.png)
### 4. 重新测试应用
完成上述安全性配置之后，将应用重新布署（重新布署后，可能需要重新在数据库表 `AccountBeanTable` 中插入测试用记录），我们可以对银行应用进行重新测试。

打开浏览器，在地址栏中输入前面给出的URL来访问银行应用中的 `BankServlet` ，如图9-28所示，可以看到浏览器弹出一个登录对话框，请求输入用户名与密码信息。
![图9-28 输入用户名与密码](https://img-blog.csdnimg.cn/0a0a30f8f7124263a1d57ab3c8a87483.png)
如果三次验证不通过，则无法访问 `BankServlet` ，客户端会显示图9-29所示的错误信息页面，提示用户身份验证失败。
![图9-29 输入用户名与密码](https://img-blog.csdnimg.cn/c285496a231f4bedb00d2a023108e8e3.png)
重新访问 `BankServlet` ，如图9-30所示输入用户 `managerli` 及相应密码，可以得到图9-31所示的访问结果。由于用户 `managerli` 映射到 `Supervisor` 角色，而前面配置的Web模块授权规则允许 `Supervisor` 访问 `BankServlet`，因此可以访问到 `BankServlet`；`BankServlet` 的安全身份配置为"使用调用者身份"，因此 `BankServlet` 调用会话构件 `AccountManager` 时使用的身份为访问者的身份，即 `Supervisor`，因此 `AccountManager` 的 `getBalance`、`createAccount`、`removeAccount` 方法均可以访问。
![图9-30 输入用户名managerli与密码](https://img-blog.csdnimg.cn/d61d038c6a8c488282192ff0ef42caec.png)
![图9-31 用户managerli访问结果](https://img-blog.csdnimg.cn/109264fa40294922804911a828f9c818.png)
关闭浏览器，重新访问 `BankServlet`，如图9-32所示输入用户 `user1` 及相应密码，可以得到图9-33 所示的访问结果。由于用户 `user1` 映射到 `NormalUser` 角色，而前面配置的Web模块授权规则允许 `NormalUser` 访问 `BankServlet` ，因此可以访问到 `BankServlet` ；`BankServlet` 的安全身份配置为"使用调用者身份"，因此 `BankServlet` 调用会话构件 `AccountManager` 时使用的身份为访问者的身份，即 `NormalUser` ，因此 `AccountManager` 的 `getBalance` 可以被访问，而 `createAccount`、`removeAccount` 方法只能由 `Supervisor` 访问，因此这两个方法不可访问。 
![图9-32 输入用户名user1与密码](https://img-blog.csdnimg.cn/ff04969a632940209caa9e10d1b961bb.png)![图9-33 用户user1访问结果](https://img-blog.csdnimg.cn/2a089219e7df41c99c8862fb78e19a2e.png)

---
# 9.4 `EJB 3.0` 概述
从前面的讨论可以看出，==EJB构件的开发，需要开发人员遵循很多EJB规范的细节，这给开发人员带来了不小的负担==，有一种观点认为"EJB大概是J2EE架构中唯一一个没有兑现其「能够简单开发并提高生产力」的承诺的组件"，`EJB 3.0` 规范正尝试在这方面作出努力，以减轻其开发的复杂性。

Sun于2004年发布了 `EJB 3.0` 的草稿，2005年10月，开源J2EE平台 `jboss` 宣布开始支持 `EJB 3.0`，Sun于2006年6月正式发布了`EJB 3.0`规范，并同期发布了 `Java Enterprise Edition 5, Java EE5` 。`EJB 3.0` 的主要目标是简化开发，同时这也是 `Java EE5` 平台的主要目标之一。

**新规范利用Java5中的程序注释工具，取消或最小化了很多**（以前这些是必须实现的）**回调方法的实现**，同时借鉴了 `Hibernate` 的成功之处，**降低了实体 `Bean` 及 `O/R` 映射模型的复杂性**。 与旧规范相比，`EJB 3.0` 的主要变化包括以下部分。
## 1. 基于程序注释的新EJB模型
**在 `EJB 3.0` 中，EJB构件是一个加了适当注释的简单Java对象**—— `plain-old-Java-object or plain-original-Java-object, POJO`  。其中：
- 部署描述符不再是必须的了（配置通过注释直接写到 `Bean` 代码中）；
- `Home` 接口没有了；
- `Bean` 类可以实现 `Remote` 接口，也可以不实现它。

也就是说，**简单情况下，EJB构件可以只包含一个 `Java` 类**，程序9-16给出了一个符合 `EJB 3.0` 规范的、简单无状态会话构件的源代码：
```java
// 程序 9-16 简单的无状态会话构件代码
import javax.ejb.*;
/**
* A stateless session bean requesting that a remote business
* interface be generated for it.
*/
@Stateless
@Remote
public class HelloWorldBean {
	public String sayHello() {
		return "Hello World!!!";
	}
}
```
程序9-17给出了一个符合 `EJB 3.0` 规范的、简单有状态会话构件的源代码：
```java
// 程序 9-17 简单的有状态会话构件代码
import javax.ejb.*;
@Stateful
@Remote
public class MyAccountBean implements Serializable {
	private int total = 0;
	private int addresult = 0;
	public int Add(int a, int b) {
		addresult = a + b;
		return addresult;
	}
	public int getResult() {
		total += addresult;
		return total;
	}
	@Remove
	public void stopSession () {
		System.out.println("stopSession()方法被调用");
	}
}
```
可以看出，**开发人员可以不用再定义 `Home` 接口和 `Remote` 接口**（平台可以自动生成它），而且**打包时的基本配置信息可以以注释的方式**（`@Stateless` 与`@Remote` 等）**直接写在EJB的源代码中**，需要使用的生命周期管理相关的方法，可以以注释的方式在程序中进行标记（如程序9-17中标记了 `@Remove` 的 `stopSession` 方法）。
## 2. 改进的实体构件及 `O/R` 映射模型
在 `EJB 3.0` 中，借鉴了 `Hibernate` 持久化支持的成功之处，**改进了实体构件与 `O/R` 映射模型**，程序9-18给出了一个符合 `EJB 3.0` 的简单实体构件的源代码：
```java
// 程序 9-18 简单的有状态会话构件代码
import javax.ejb.*;
@Entity
@Table(name = "Person") 
public class Person {
	private Integer personid;
	private String name;
	private boolean sex;
	@Id
	@GeneratedValue
	public Integer getPersonid() {
		return personid;
	}
	public void setPersonid(Integer personid) {
		this.personid = personid;
	}
	@Column(name = "PersonName", nullable=false, length=32)
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	@Column(nullable=false)
	public boolean getSex() {
		return sex;
	}
	public void setSex(boolean sex) {
		this.sex = sex;
	}
}
```

需要注意地是，从开发角度看，新的EJB规范看起来发生了很大变化（尤其是对于初学者来说），但是**支撑EJB构件的核心机制并没有发生变化**，==在开发实际复杂应用时，第三部分所讨论的EJB构件开发与J2EE公共服务相关内容，仍然是开发人员应该掌握的必要内容==。

---
# 9.5 `J2EE/EJB` 与 `CORBA` 对比
作为主流中间件标准，`J2EE/EJB` 与 `CORBA` 均为软件系统开发提供了「强大的构件支持」与「丰富的公共服务支持」。==在实际项目开发时，选择基于哪种规范进行开发，通常取决于待开发项目的需求特点与规范优势的匹配程度==。现就以下几个方面，对 `J2EE/EJB` 规范与 `CORBA` 规范进行简要对比。
## 1. 工业标准特性
可以说，**`J2EE/EJB` 与 `CORBA` 提供的都是一种工业标准，而非一个软件产品**，不同供应商的竞争导致市场上存在大量高质量的、完全遵循 `J2EE/EJB` 或 `CORBA` 的产品。共同遵循的标准也为应用系统提供了较好的可移植性。
## 2. 对分布式对象的支持
`J2EE/EJB` 与 `CORBA` 均采用 `Stub/Skeleton` 结构，对分布式对象提供了良好的支持，**基于 `Stub/Skeleton` 结构，开发人员不需编写底层通信代码，即可实现分布式对象与客户端之间跨越网络的交互**。
- 但在 `J2EE/EJB` 规范中，每个EJB构件与客户端通过两对 `Stub/Skeleton` 交互，并且直接通过 `Skeleton` 访问的并不是真正的EJB构件，而是容器自动生成的对象，这样有助于容器更好地为EJB构件提供公共服务。
- 另外，EJB支持本地接口，同进程内的EJB构件之间，可以通过本地接口获得较高的访问效率。
- 而 `CORBA` 则对客户端的动态调用接口与服务端的动态框架接口提供了较好的支持，可以适应一些特殊场合应用的开发。
## 3. 对公共服务的支持
`J2EE/EJB` 与 `CORBA` 均提供了丰富的公共服务支持，==基于这些公共服务的支持，应用构件可以直接将命名服务、事务服务、安全服务、消息服务、资源管理服务等功能直接集成到应用系统中，而不需要开发人员自行实现==。
- 相对而言，**`CORBA` 规范中提供的公共服务涵盖的范围更广**，OMG甚至对特定领域内（如电信领域、医疗领域等）通用的功能进行了标准化，这使得从最基本的如命名服务、事务服务等公共服务、一直到领域内通用的功能，都具备了「直接将平台提供商实现的功能集成入应用系统」的可能。
- 但是，`CORBA` 中提供的公共服务多局限于基于API调用的使用方式，这些服务的使用方式可对应到 `J2EE/EJB` 规范中的 `Service API` ，而 **`J2EE/EJB` 规范除了以API方式提供的公共服务之外，还提供丰富的运行时服务**，这些运行时服务可通过配置的方式直接使用，从而为应用提供了较好的、源代码以外的可定制性。
- 此外，**`J2EE/EJB` 规范提供了强大的构件生命周期管理服务**，平台/容器会根据运行与客户端请求状况自动管理构件的生命周期，而 `CORBA` 中需要开发人员编写服务程序来管理服务端的应用构件，但是 **`CORBA` 也提供了相应的机制**（如伺服对象管理器机制）来帮助开发人员管理服务端构件（开发人员应注意，"软件系统中的功能交给平台自动完成"永远不一定是好事情）。
## 4. 开发过程
把3.2节描述的 `CORBA` 应用的开发过程一般化，我们可以用类似的过程描述一般分布式应用的开发过程，如图9-34所示，其==基本过程为首先定义分布式对象的接口，然后分别完成服务端与客户端的开发，最后布署运行应用程序==。下面简要分析开发过程中 `J2EE/EJB` 与 `CORBA` 的不同之处。
- 编写对象接口：在 `CORBA` 中，对象接口是用 `OMG IDL` 定义的，而 `IDL` 可以映射到不同的程序设计语言，因此 **`CORBA` 应用的服务端与客户端可以使用不同程序设计语言实现**；在 `J2EE/EJB` 中，EJB构件的接口是用Java语言的 `interface` 定义的，因此**EJB构件及其客户端均必须用Java语言实现**。
- 编写服务端程序：在 `CORBA` 中，服务端除了需要按照接口的约定实现分布式对象外，**开发人员还需编写一个服务程序**来将分布式对象准备好（创建并激活）；而在 `J2EE/EJB` 中，开发人员不需编写类似的服务程序，**容器会根据运行与客户端请求状况自动创建或删除构件**。但是将构件生命周期管理交给容器自动管理的同时，也意味着开发人员丧失了对服务端程序的更多控制力。
- 编写客户端程序：在 `J2EE/EJB` 和 `CORBA` 中，编写客户端的工作非常相似，只是在`EJB 3.0` 之前，EJB的客户端需要首先利用 `Home` 的 `create` 方法，获取一个可用的EJB引用，过程比 `CORBA` 客户端要复杂。
![图9-34 分布式系统的一般开发过程](https://img-blog.csdnimg.cn/13050a3e9acc4155b8ec16d108d70f2f.png)
- 布署应用程序：与 `CORBA` 相比，**`J2EE/EJB` 提供了丰富的源代码以外的可定制性**，其应用的布署过程通常比 `CORBA` 应用的布署要复杂，但是随着复杂的布署过程，应用也获得了丰富的可定制性支持。

总地来说，`J2EE/EJB` 规范比较适合**基于Web的分布式应用的开发**，而 `CORBA` 则在**可互操作要求高的分布式系统开发**方面有更大的优势。

---
# 思考与练习
9-1 在9.2节的 `CMT` 例子中，如果 `withdraw` 方法的执行过程中发生了 `SQLException`，那么对应事务最终会提交还是回滚？为什么？
9-2 在9.2节的 `BMT` 例子中，某次执行如果既没有执行 `commit` ，也没有执行 `rollback` ，那么事务最终的执行状态如何？这样对于应用系统来说有什么害处？
9-3 试为9.3节中的例子增加更详细的安全性控制规则，如可为实体构件也增加授权控制，并重新布署并测试你的配置效果。

