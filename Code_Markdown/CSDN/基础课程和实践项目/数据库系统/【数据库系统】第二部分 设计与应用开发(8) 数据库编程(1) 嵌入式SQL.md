> 本文属于「数据库系统学习实践」系列文章之一，这一系列着重于「数据库系统知识的学习与实践」。由于文章内容随时可能发生更新变动，欢迎关注和收藏[数据库系统系列文章汇总目录](https://memcpy0.blog.csdn.net/article/details/119996493)一文以作备忘。需要特别说明的是，为了透彻理解和全面掌握数据库系统，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - 数据库系统概念 第六版 `Database System Concepts, Sixth Edition` ，作者是 `Abraham Silberschatz, Henry F. Korth, S. Sudarshan` ，机械工业出版社
> - 数据库系统概论 第五版，王珊 萨师煊编著，高等教育出版社


@[toc]
 
---
# 8.1 嵌入式SQL
第3章已经讲到，SQL的特点之一是，**在交互式和嵌入式两种不同的使用方式下，SQL的语法结构基本上是一致的**。只不过在程序设计环境下，SQL语句要做某些必要的扩充。
## 8.1.1 嵌入式SQL的处理过程
**嵌入式SQL是将SQL语句嵌入程序设计语言 `Programming Language, PL` 中**，被嵌入的程序设计语言如C/C++、Java、Python等称为**宿主语言**，简称**主语言**。

对嵌入式SQL，数据库管理系统一般采用**预编译方法**处理，即由数据库管理系统的预处理程序，对源程序进行扫描，识别出嵌入式SQL语句，把它们转换成主语言调用语句，以使主语言编译程序能识别它们；然后由主语言的编译程序，将纯的主语言文件编译成目标程序。嵌入式SQL基本处理过程如下图所示：
<img src="https://img-blog.csdnimg.cn/aafb2009bc2f44509cd79e8fb704bca8.png#pic_center" width="30%">

**在嵌入式SQL中，为了能够快速区分SQL语句和主语言语句，所有SQL语句都必须加前缀**。当主语言为C语言时，语法格式为：
```sql
exec sql <SQL语句>;
```
如果主语言为Java，则嵌入式SQL称为 `SQLJ` ，语法格式为：
```java
#SQL { <SQL语句> };
```

---
## 8.1.2 嵌入式SQL语句与主语言之间的通信
将SQL嵌入到高级语言中混合编程，（**描述性的面向集合的语句**）SQL语句负责操纵数据库，（**过程性的面向记录的语句**）高级语言语句负责控制逻辑流程。这时程序中含有两种不同计算模型的语句，它们之间应该如何通信呢？

数据库工作单元与源程序工作单元之间的通信方式，主要包括：
（1）向主语言传递SQL语句的执行状态信息，使主语言能够据此信息、控制程序流程。主要用**SQL通信区 `SQL Communication Area, SQLCA)`**（一个数据结构）实现。
（2）主语言向SQL语句提供参数，主要用**主变量 `host variable`** 实现。
（3）将SQL语句查询数据库的结果交给主语言处理，主要用**主变量和游标 `cursor`** 实现。

### 1. SQL通信区
SQL语句执行后，系统要反馈给应用程序若干信息，主要包括描述系统当前工作状态和运行环境的各种数据。这些信息将送到SQL通信区中，应用程序从SQL通信区中取出这些状态信息，据此决定接来下执行的语句。

**在应用程序中，SQL通信区用 `exec sql include sqlca` 加以定义**。SQL通信区中有一个变量 `sqlcode` ，用来存放每次执行SQL语句后、返回的代码。

**应用程序每执行完一条SQL语句之后，都应该测试一下 `sqlcode` 的值**，以了解该SQL语句的执行情况，并做相应处理。如果 `sqlcode` 等于预定义的常量 `success` ，则表示SQL语句成功；否则在 `sqlcode` 中存放错误代码，程序员可根据错误代码查找问题。

### 2. 主变量
嵌入式SQL语句中，可以使用主程序的程序变量来输入或输出数据。SQL语句中使用的主语言程序变量简称为**主变量**。主变量根据其作用的不同，分为输入主变量和输出主变量。**输入主变量**由应用程序对其赋值，SQL语句引用；**输出主变量**由SQL语句对其赋值或设置状态信息，返回给应用程序。

**一个主变量可附带一个任选的指示变量** `indicator variable` 。**指示变量**是一个整型的变量，用来指示所指主变量的值或条件。指示变量可指示输入主变量是否为空值，可检测输出主变量是否为空值，值是否截断。

**所有主变量和指示变量必须在SQL语句 `begin declare section` 与 `end declare section` 之间进行说明**。说明之后，**主变量可在SQL语句中，任何一个能够使用表达式的地方出现**——为了与数据库对象名（表名、视图名、列名等）区别，SQL语句中的主变量名和指示变量前要加冒号 `:` 作为标志，指示变量还必须紧跟在所指主变量之后。**在SQL语句之外（主语言语句中）使用主变量和指示变量，可以直接引用、不用加冒号**。

### 3. 游标
**SQL是面向集合的**，一条SQL语句可产生或处理多条记录；而**主语言是面向记录的**，一组主变量一次只能存放一条记录。所以，仅使用主变量，并不能完全满足「SQL语句向应用程序输出数据」的要求。为此，嵌入式SQL引入了**游标**的概念，用游标来协调这两种不同的处理方式。

**游标是系统为用户开设的一个数据缓冲区**，存放SQL语句的执行结果，每个游标区都有一个名字。**用户可通过游标，逐一获取记录并赋给主变量，交给主语言进一步处理**。

### 4. 建立和关闭数据库连接
==嵌入式SQL程序要访问数据库，必须先连接数据库==。关系DBMS根据用户信息，对连接请求进行合法性验证，只有通过了验证，才能建立一个可用的合法连接。
（1）**建立数据库连接**：建立连接的嵌入式SQL语句如下，其中的 `target` 是要连接的数据库服务器，它可以是一个常见的服务器标识串，如 `<dbname>@<hostname>:<port>` ，可以是包含服务器标识的SQL串常量，也可以是 `default` 。
```sql
exec sql connect to target [as <连接名>] [user <用户名>];
```
`<连接名>` 则是可选的，它必须是一个有效的标识符，主要用来识别一个程序内、同时建立的多个连接。如果在整个程序内只有一个连接，也可以不指定连接名。==如果程序运行过程中建立了多个连接名，则执行的所有数据库单元的工作，都在该操作提交时所选择的当前连接上==。

程序运行过程中可修改当前连接，对应的嵌入式SQL语句为：
```sql
exec sql set connection <连接名> | default;
```
（2）**关闭数据库连接**：当某个连接上的所有数据库操作完成后，应用程序应主动释放所占用的连接资源。关系数据库连接的嵌入式SQL语句是：
```sql
exec sql disconnect [连接名];
```
其中，`连接名` 是 `exec sql connect` 所建立的数据库连接。

【例8.1】依次检查某个系的学生记录，交互式更新某些学生年龄。
答：这是一个简单的嵌入式SQL编程实例。
```sql
exec sql begin declare section;	/* 主变量说明开始 */
		char deptname[20];
		char hsno[9];
		char hsname[20];
		char hssex[2];
		int HSage;
		int NEWAGE;
exec sql end declare section;	/* 主变量说明结束 */
long SQLCODE;
exec sql include SQLCODE;		/* 定义SQL通信区 */
int main(void)	{				/* C语言主程序开始 */
	int count = 0;
	char yn;					/* 变量yn代表yes或no */
	printf("Please choose the department name(CS/MA/IS): ");
	scanf("%s", &deptname);		/* 为主变量deptname赋值 */
	exec sql connect to Test@localhost:54321 user "System"/"Manager";
	exec sql declare SX cursor for 	/* 定义游标SX */
		select Sno, Sname, Ssex, Sage	/* SX对应的语句 */
		from Student
		where Sdept = :deptname;	
	exec sql open SX;			/* 打开游标SX, 指向查询结果的第一行 */
	for (;;) {					/* 用循环结构,逐条处理结果集中的记录 */
		exec sql fetch SX into :HSno, :HSname, :HSsex, :HSage; /* 推进游标,将当前数据放入主变量 */
		if (SQLCA.sqlcode != 0)	/* sqlcode!=0,表示操作不成功 */
			break;				/* 利用SQLCA中的状态信息,决定何时退出循环 */
		if (count++ == 0)		/* 如果是第一行的话,先打出行头 */
			printf("\n%-10s%-20s%-10s%-10s%\n", "Sno", "Sname", "Ssex", "Sage");
		printf("%-10s%-20s%-10s%-10d\n", HSno, HSname, HSsex, HSage); /* 打印查询结果 */
		printf("UPDATE AGE(y/n)?");	/* 询问用户是否要更新该学生的年龄 */
		do {
			scanf("%c", &yn);
		} while (yn != 'N' && yn != 'n' && yn != 'Y' && yn != 'y');
		if (yn == 'Y' || yn == 'y') {	/* 如果选择更新操作 */
			printf("INPUT NEW AGE:");
			scanf("%d", &NEWAGE);		/* 用户输入新年龄到主变量中 */
			exec sql update Student		/* 嵌入式SQL更新语句 */
			set Sage = :NEWAGE;
			where current of SX;		/* 对当前游标指向的学生年龄进行更新 */
		}
	}
	exec SQL close SX;	/* 关闭游标SX,不再和查询结果对应 */
	exec sql commit work;	/* 提交更新 */
	exec sql disconnect Test; /* 断开数据库连接 */
}
```

---
## 8.1.3 不用游标的SQL语句
**有的嵌入式SQL语句不需要使用游标，它们是说明性语句、数据定义语句、数据控制语句、查询结果为单记录的 `select` 语句、非 `current` 形式的增删改语句。**
### 1. 查询结果为单记录的 `select` 语句
这类语句因为查询结果只有一个，**只需用 `into` 子句，指定存放查询结果的主变量**，不需要使用游标。使用查询结果为单记录的 `select` 语句时，需要注意以下几点：
（1）**`into` 子句、`where` 子句和 `having` 短语的条件表达式中，均可以使用主变量**；
（2）查询结果为空值的处理。查询返回的记录中，可能某些列为空值。为了表示空值，在 `into` 子句的主变量后可跟指示变量，**当查询得出的某个数据项为空值时，系统会自动将相应主变量后面的指示变量置为负值，而不再向该主变量赋值**。所以，**当指示变量值为负值时，不管主变量为何值，均认为主变量值为 `null`** 。
（3）如果查询结果实际上并不是单条记录、而是多条记录，则程序出错，关系DBMS会自动在SQL通信区中，返回错误信息。

【例8.2】假设已经把要查询的学生的学号赋给了主变量 `givensno` ，现在根据学号查询学生信息。
答：注意，下面的代码中，没有使用指示变量。
```sql
exec sql 
	select Sno, Sname Ssex, Sage, Sdept
	into :Hsno, :Hname, :Hsex, :Hage, :Hdept
	from Student
	where Sno = :givensno;
```
【例8.3】查询某个学生选修某门课程的成绩。假设已经把将要查询的学生的学号赋给了主变量 `givensno` ，将课程号赋给了主变量 `givencno` 。
答：如果指示变量 `Gradeid < 0` ，则不论 `Hgrade` 为何值，均认为该学生成绩为空值。
```sql
exec sql 
	select Sno, Cno, Grade
	into :Hsno, :Hcno, :Hgrade :Gradeid	/* 指示变量Gradeid */
	from SC
	where Sno = :givensno and Cno = :givencno;
```
### 2. 非 `current` 形式的增删改语句
有些非 `current` 形式的增删改语句，不需要使用游标。**在 `update` 的 `set` 子句和 `where` 子句中可以使用主变量，`set` 子句还可以使用指示变量**。

【例8.4】修改某个学生选修1号课程的成绩。
答：
```sql
exec sql
update SC
set Grade = :newgrade	/* 修改的成绩已赋给主变量newgrade */
where Sno = :givensno and Cno = 1; /* 学号已赋给主变量givensno */
```
【例8.5】某个学生新选修了某门课程，将有关记录插入 `SC` 表中。假设插入的学号已经赋给主变量 `stdno` ，课程号已赋给主变量 `couno` ，由于该学生刚选修课程，成绩应为空，所以要把指示变量赋为负值。
答：
```sql
gradeid = -1;	/* gradeid为指示变量, 赋为负值 */
exec sql
insert into SC(Sno, Cno, Grade)
values(:stdno, :couno, :gr :gradeid); /* stdno, couno, gr为主变量 */
```
---
## 8.1.4 使用游标的SQL语句
必须使用游标的SQL语句，有**查询结果为多条记录的 `select` 语句**、**`current` 形式的 `update` 和 `delete` 语句**。
### 1. 查询结果为多条记录的 `select` 语句
一般情况下，`select` 语句的查询结果是多条记录，因此需要用游标机制，将多条记录一次一条地送往主程序处理，从而把对集合的操作转换为对单个记录的处理。使用游标的步骤为：
（1）==声明游标==：用 `declare` 语句为一条 `select` 语句定义游标。**定义游标仅仅是一条说明性语句，这时关系DBMS并不执行 `select` 语句**。
```sql
exec sql declare <游标名> cursor 
	for <select语句>;
```
（2）==打开游标==：用 `open` 语句将定义的游标打开。**打开游标实际上是执行相应的 `select` 语句，把查询结果取到缓冲区中**。这时游标处于活动状态，指针指向查询结果集中的第一条记录。
```sql
exec sql open <游标名>;
```
（3）==推进游标记录并取当前记录==：用 `fetch` 语句，将缓冲区中的当前记录取出来、送至主变量供主语言进一步处理，同时把游标指针向前推进一条记录。**通过循环执行 `fetch` 语句，我们逐条取出结果集中的行进行处理**。注意，**主变量必须与 `select` 语句中的目标列表达式具有一一对应关系**。
```sql
exec sql fetch <游标名>
	into <主变量>[<指示变量>] [,<主变量>[<指示变量>]] ...;
```
（4）==关闭游标==：用 `close` 语句关闭游标，释放结果集占用的缓冲区及其他资源。**游标被关闭后，就不再和原来的查询结果集相联系**，但被关闭的游标可以再次被打开，与新的查询结果相联系。
```sql
exec sql close <游标名>;
```

### 2. `current` 形式的 `update` 和 `delete` 语句
`update` 语句和 `delete` 语句都是集合操作，如果**只想修改或删除其中某个记录**，则需要用带游标的 `select` 语句，查出所有满足条件的记录，从中进一步找出要修改或删除的记录，然后用 `current` 形式的 `update` 和 `delete` 语句修改或删除之。即 `update` 语句和 `delete` 语句中，要用子句：
```sql
where current of <游标名>
```
来表示**修改或删除的是「最近一次取出的记录」**，即游标指针指向的记录。【例8.1】中的 `update` 就是用 `current` 形式的。
> 注意：当游标定义中的 `select` 语句带有 `union` 或 `order by` 子句，或者该 `select` 语句相当于定义了一个不可更新的视图时，不能使用 `current` 形式的 `update` 语句和 `delete` 语句。

---
## 8.1.5 动态SQL
前面所讲的嵌入式SQL语句中，使用的主变量、查询目标列、条件等都是固定的，属于**静态SQL语句**。静态嵌入式SQL语句能够满足一般要求，但某些应用可能要到执行时，才能够确定要提交的SQL语句和查询的条件，此时就要使用**动态SQL语句**来解决这类问题。

动态SQL方法，允许在程序运行过程中临时组装SQL语句。动态SQL支持**动态组装SQL语句**和**动态参数**两种形式，给开发者提供设计任意SQL语句的能力。
### 1. 使用SQL语句主变量
**程序主变量包含的内容是SQL语句（的内容）**，而不是原来保存数据的输入或输出变量。这样的变量称为**SQL语句主变量**。SQL语句主变量在程序执行期间，可以设定不同的SQL语句，然后立即执行。

【例8.7】创建基本表 `Test` 。
答：
```sql
exec sql begin declare section;
	const char *stmt = "create table Test(a int);"; /* SQL语句主变量,内容是创建表的SQL语句 */
exec sql end declare section;
...
exec sql execute immediate :stmt; /* 执行动态SQL语句 */
```
### 2. 动态参数
**动态参数是SQL语句中的可变元素**，使用参数符号 `(?)` 表示「**该位置的数据在运行时设定**」。和前面使用的主变量不同，**动态参数的输入**不是编译时完成绑定，而是**通过 `prepare` 语句准备主变量、通过执行语句 `execute` 绑定数据或主变量来完成**。

使用动态参数的步骤如下：
（1）声明SQL语句主变量：SQL语句主变量的值包含动态参数 `(?)` 。
（2）准备SQL语句：`prepare` 将分析含主变量的SQL语句内容 ，建立语句中**包含的动态参数的内部描述符**，并用 `<语句名>` 标识它们的整体：
```sql
exec sql prepare <语句名> from <SQL语句主变量>;
```
（3）执行准备好的语句：`EXECUTE` 将SQL语句中分析出的**动态参数**和**主变量或数据常量**绑定，作为**语句的输入或输出变量**：
```sql
exec sql execute <语句名> [into <主变量表>] [using <主变量或常量>];
```
【例8.8】向 `Test` 中插入元组。
答：
```sql
exec sql begin declare section;
		/* 声明SQL主变量内容是insert语句 */
		const char *stmt = "insert into Test values(?);";
exec sql end declare section;
...
exec sql prepare mystmt from :stmt; /* 准备语句 */
...
exec sql execute mystmt using 100;  /* 执行语句,设定insert语句插入值100 */
exec sql execute mystmt using 200;	/* 执行语句,设定insert语句插入值200 */
```
