> 本文属于「数据库系统」系列文章之一，这一系列着重于「数据库系统知识的学习与实践」。由于文章内容随时可能发生更新变动，欢迎关注和收藏[数据库系统系列文章汇总目录](https://memcpy0.blog.csdn.net/article/details/119996493)一文以作备忘。需要特别说明的是，为了透彻理解和全面掌握数据库系统，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - 数据库系统概念 第六版 `Database System Concepts, Sixth Edition` ，作者是 `Abraham Silberschatz, Henry F. Korth, S. Sudarshan` ，机械工业出版社
> - 数据库系统概论 第五版，王珊 萨师煊编著，高等教育出版社

@[toc]

---
# 3.3 数据定义
## 3.3.0 定义数据库
在创建数据库的时候，我并不知道db1数据库有没有创建，直接再次创建名为db1的数据库就会出现错误。

**为了避免上面的错误，在创建数据库的时候先做判断，如果不存在再创建**。运行语句效果如下：  
```sql
CREATE DATABASE IF NOT EXISTS 数据库名称;
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312048136.png)
从上面的效果可以看到，虽然db1数据库已经存在，再创建db1也没有报错，而创建db2数据库则创建成功。  

删除数据库也一样，可以先判断，如果存在则删除。运行语句效果如下：  
```sql
DROP DATABASE IF EXISTS 数据库名称;
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312050408.png)

数据库创建好了，要在数据库中创建表，得先明确在哪儿个数据库中操作，此时就需要使用数据库。  使用数据库：
```sql
USE 数据库名称;
```
查看当前使用的数据库：
```sql
SELECT DATABASE();
```
运行语句效果如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312052151.png)

---
**关系数据库系统支持三级模式结构**，其模式、外模式、内模式中的基本对象有模式、表、视图和索引等。因此SQL的数据定义功能中，包括模式定义、表定义、视图和索引的定义，如下表3.3所示：

| 操作对象 | 创建 | 删除 | 修改
|:---|:---|:---|:--
| 模式 | `create schema` | `drop schema` 
| 表 | `create table` | `drop table` | `alter table` 
| 视图 | `create view` | `drop view`
| 索引 | `create index` | `drop index` | `alter index` 

SQL标准不提供「修改模式定义」和「修改视图定义」的操作。用户如果想修改这些对象，只能**先将它们删除然后再重建**。SQL标准也没有提供「索引相关的语句」，但为了提高查询效率，商用的关系DBMS中，通常都提供了索引机制和相关的语句，如上表中创建、删除和修改索引等。

在早期的数据库系统中，所有数据库对象都属于一个数据库，也就是说只有一个命名空间。现代的关系DBMS中，提供了一个**层次化的数据库对象命名机制**，如下图所示。==一个关系DBMS的实例 `instance` 中，可以建立多个数据库，一个数据库中可以建立多个模式，一个模式下通常包括多个表、视图和索引等数据库对象==。

本节介绍如何定义模式、基本表和索引，视图的概念及其定义方法见[【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL(7) 视图](https://memcpy0.blog.csdn.net/article/details/120905098)。

## 3.3.1 模式的定义与删除
### 1. 定义模式
在SQL中，模式定义语句如下所示：
```sql
create schema <模式名> authorization <用户名>;
```
模式由**模式名**或者**模式拥有者的用户名**来确定，并包含模式中的所有元素——表、视图、索引等的定义。如果没有指定 `<模式名>` ，那么 `<模式名>` 隐含为 `<用户名>` 。

==要创建模式，调用该命令的用户必须拥有数据库管理员权限，或者获得了数据库管理员授予的 `create schema` 权限==。

【例3.1】为用户 `WANG` 定义一个学生-课程模式 `ST` 。
答：下面的语句中没有指定 `<模式名>` ，因此 `<模式名>` 隐含为用户名 `WANG` ：
```sql
create schema authorization WANG;
```
或者写成：
```sql
create schema ST authorization WANG;
```
【例3.2】在数据库中创建一个教师模式 `Teachers` ，用户为 `ZHANG` 。
答：
```sql
crate schema Teachers authorization ZHANG;
```

==**定义一个模式，实际上就是定义了一个存储空间/命名空间**，在这一空间中，我们可以进一步定义该模式包含的数据库对象==，例如基本表、视图、索引等。这些数据库对象可用表3.3中相应的 `create` 语句来定义。该存储空间中的全体数据库对象，就构成该模式对应的SQL数据库。

目前，在 `create schema` 中可以接受的有：`create table` 、`create view` 、`grant` 子句。也就是说，用户可在创建模式的同时，在这个模式定义中进一步创建基本表、视图，并定义授权。即：
```sql
create schema <模式名>
authorization <用户名>
[<create domain子句> | <create table子句> | <create view子句> | <授权定义子句>];
```
当然，模式中的表、视图等，以后也可以随时创建。

【例3.3】为用户 `ZHANG` 创建一个模式 `Test` ，并在其中定义一个表 `Tab1` 。
```sql
create schema Test 
authorization ZHANG
create table Tab1 (
	col1 smallint,
	col2 int,
	col3 numeric(10, 3),
	col4 char(20),
	col5 decimal(5, 2),
	col6 varchar(30)
);
```
### 2. 删除模式
在SQL中，删除模式的语句如下：
```sql
drop schema <模式名> <cascade | restrict>;
```
其中，`cascade` 级联式和 `restrict` 约束式二者必选其一：
- 选择 `cascade` 级联，表示：==在删除模式的同时，把该模式中所有的数据库对象全部删除==；
- 选择 `restrict` 限制，表示：==如果该模式中已经定义了下属的数据库对象（如表、视图等），则拒绝该删除语句的执行。只有当该模式中没有任何下属的对象时，才能执行 `drop schema` 语句==。

【例3.4】级联删除模式 `Test` 。
答：该语句删除了模式 `Test` ，同时该模式中已经定义的表 `Tab1` 也被删除了：
```sql
drop schema Test cascade;
```

---
## 3.3.2 基本表的定义、删除与修改
### 0. 查询表
查询当前数据库下所有表名称：
```mysql
SHOW TABLES;
```
查询表结构：
```mysql
DESC 表名称;
```
查看mysql数据库中 `func` 表的结构，运行语句如下：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312115460.png)

### 1. 定义基本表
**创建了一个模式，就创建了数据库中的一个命名空间、一个框架**。在这个空间中，首先要定义的是**该模式包含的数据库基本表**。SQL语言使用 `CREATE TABLE` 语句定义基本表，其基本格式如下：
```sql
create table [模式名.]<表名> (
	<列名> <数据类型> [列级完整性约束条件]
	 [, <列名> <数据类型> [列级完整性约束条件]] ...
	 [, <表级完整性约束条件>]
);
```
其中，`<表名>` 是要定义的基本表的名字，一张表可由一个或多个属性列组成，每个列的数据类型可以是预定义类型，也可以是自定义类型。

**建表的同时，通常还可以定义与该表有关的完整性约束条件，这些完整性约束条件被存储在系统的数据字典中**。当用户操作表中的数据时，由关系DBMS自动检查该操作是否违背这些完整性约束条件。==如果完整性约束条件涉及该表的多个属性列，则必须定义在表级上，否则既可以定义在列级、也可以定义在表级==。

完整性约束常用的**三种子句形式**如下，其中的 `CHECK` 子句规定了一个规则，即一个表的指定列只能够取这个规则定义的集合之内的值。更全的说明见[【数据库系统】第一部分 数据库基础(5) 数据库完整性](https://memcpy0.blog.csdn.net/article/details/121433913)：
```mysql
# 主键子句
primary key (<列名>) # 主键列强制满足非空和唯一性条件
# 外键子句
foreign key (<列名>) references [<表名>][<约束选项>]
# 检验子句
check (<约束条件>)
```

【例3.5】建立一个学生表 `Student` 。
答：系统执行该 `create table` 语句后，就在数据库中建立一个新的空学生表 `Student` ，并将有关学生表的定义及有关约束条件，存放在数据字典中：
```sql
create table Student (
	Sno char(9) primary key, /* 列级完整性约束条件, Sno是主键 */
	Sname char(20) not null, /* Sname非空 */
	Ssex char(2),
	Sage smallint,
	Sdept char(20),
	check (Ssex in ('男', '女')) /* 用户自定义完整性约束条件*/
);
```
【例3.6】建立一个课程表 `Courses` 。
答：==本例说明，参照表和被参照表可以是同一个表==。
```sql
create table Course (
	Cno char(4) primary key, /* 列级完整性约束条件,Cno是主键 */
	Cname char(40) not null, /* 列级完整性约束条件,Cname不能取空值 */
	Cpno char(4), 			 /* Cpno的含义是先修课 */
	Ccredit smallint,
	foreign key (Cpno) references Course(Cno) 
	/* 表级完整性约束条件,Cpno是外键,被参照表是Course,被参照列是Cno */
);
```
【例3.8】建立学生选课表 `SC` 。
答：
```sql
create table SC (
	Sno char(9),
	Cno char(4),
	Grade smallint,
	/* 主键由两个属性构成,必须作为表级完整性约束进行定义 */
	primary key (Sno, Cno), 
	/* 表级完整性约束条件,Sno是外键,被参照表是Student */
	foreign key (Sno) references Student(Sno),
	/* 表级完整性约束条件,Cno是外键,被参照表是Course */
	foreign key (Cno) references Course(Cno),
	check ((Grade is null) or
		(Grade between 0 and 100)) /* 用户自定义完整性约束条件 */  
);
```

### 2. 数据类型
==关系模型中一个很重要的概念是**域**==。**每个属性来自一个域，它的取值必须是域中的值**——==在SQL中，域的概念用数据类型来实现==。定义表的各个属性时，需要指明其数据类型及长度。SQL标准支持多种数据类型，下表3.4列出了几种常用的数据类型。要注意，不同的关系DBMS中支持的数据类型不完全相同。
<img src="https://img-blog.csdnimg.cn/20200412142635913.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center" width="64%">
<img src="https://img-blog.csdnimg.cn/f74757f62b8b484d83d1dd89c40aee41.png#pic_center" width="64%">

==一个属性选用哪种数据类型，要根据实际情况来决定，一般要从两个方面来考虑，一是取值范围，二是要做哪些运算==。例如，对于年龄 `Sage` 属性，可采用 `char(3)` 作为数据类型，但考虑到要做年龄上做算术运算（如求平均年龄），所以要采用整数作为数据类型，因为在 `char(n)` 数据类型上不能进行算术运算。整数又有长整数 `int` 、短整数 `smallint` 和大整数 `bigint` 三种，由于一个人的年龄在百岁左右，所以选择短整数作为年龄的数据类型。

MySQL 支持多种类型，可以分为三类：  
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202212312122267.png)
数值：
```sql
tinyint : 小整数型，占一个字节  
int : 大整数类型，占四个字节  
	eg : age int  
double : 浮点类型  
	使用格式: 字段名 double(总长度,小数点后保留的位数)  
	eg : score double(5,2)  
```
日期：
```sql
date : 日期值。只包含年月日  
	eg : birthday date ：  
datetime : 混合日期和时间值。包含年月日时分秒
timestamp : 只到2038年
```
字符串：
```sql
char : 定长字符串
	优点: 存储性能高一些 
	缺点: 浪费空间  
	eg : name char(10) 如果存储的数据字符个数不足10个，也会占10个的空间  
varchar : 变长字符串  
	优点: 节约空间，时间换空间
	缺点: 存储性能低 
	eg : name varchar(10) 如果存储的数据字符个数不足10个，那就数据字符个数是几就占几个
```
> 注意：没有人会把电影存到数据库，太浪费性能了。像LONGBLOB, LONGTEXT只有4个G。存电影这种大的数据，我们有专门的文件服务器，在数据库中用VARCHAR存储文件的访问路径。

### 3. 模式与表
**每个基本表都属于某个模式，一个模式包含多个基本表**。当定义基本表时，一般可以有三种方法定义它所属的模式。例如在【例3.1】中定义了一个学生-课程模式 `ST` ，现在要在 `ST` 中定义 `Student, Course, SC` 等基本表。
- 方法一，**在表名中明确地给出模式名**：
	```sql
	create table ST.Student(...); /* Student所属的模式是ST */
	create table ST.Course(...); /* Course所属的模式是ST */
	create table ST.SC(...);	/* SC所属的模式是ST */
	```
- 方法二，**在创建模式语句中，同时创建表**。如【例3.3】所示。
- 方法三，**设置所属的模式**，这样在创建表时，表名中不必给出模式名。

当用户创建基本表（其他数据库对象也一样）时若没有指定模式，系统根据搜索路径 *search path* 来确定该对象所属的模式。==搜索路径包含一组模式列表，关系DBMS会使用模式列表中第一个存在的模式，作为数据库对象的模式名。若搜索路径中的模式名都不存在，系统将给出错误==。使用下面的语句，可以显示出当前的搜索路径：
```sql
show search_path;
```
搜索路径的当前默认值是 `$user, PUBLIC` ，其含义是**首先搜索与用户名相同的模式名，如果该模式名不存在，则使用 `PUBLIC` 模式**。

数据库管理员也可以设置搜索路径，例如：
```sql
set search_path to "ST", PUBLIC;
```
然后，定义基本表：
```sql
create table Student (...);
```
实际结果是建立了 `ST.Student` 这个基本表。因为关系DBMS发现搜索路径中第一个模式名 `ST` 存在，就把该模式作为基本表 `Student` 所属的模式。

### 4. 修改基本表
随着应用环境和应用需求的变化，有时需要修改已建立好的基本表，包括增加新属性列、修改原来的列定义、增加新的/删除已经有的完整性约束等。SQL语言中用 `alter table` 语句修改基本表，其一般格式为：
```mysql
alter table <表名> [rename to <新表名>];
alter table <表名>
	[add <column> <新列名> <数据类型> [完整性约束]];
	[add <表级完整性约束>];
	[drop [column] <列名> [cascade | restrict]] ...
	[drop constraint <完整性约束名> [cascade restrict]] ...
	[alter column <列名> <数据类型>] ...; 
	[modify <列名> <新数据类型> <新约束>];
	[change <列名> <新列名> <新数据类型>];
```
其中，`<表名>` 指定**需要修改的基本表**。各子句的用法是：
- `add` 子句用于**增加新列和新的列级/表级完整性约束条件**。如补充定义主键 `alter table <表名> add primary key (<列名>...)` 。
- `drop column` 子句用于**删除表中指定的列**，如果指定了 `cascade` 短语，则自动删除引用了该列的其他对象，比如视图；如果指定了 `restrict` 短语，则该列若被其他对象引用，关系DBMS将拒绝删除该列。
- `drop constraint` 子句用于**删除指定的完整性约束条件**。
- `alter column` 子句用于**修改原有的列定义**，包括修改列名和数据类型（在Oracle数据库中是 `modify` 子句）。

【例3.8】向 `Student` 表增加“入学时间”列，其数据类型为日期型，增加“专业”和“地址”列，数据类型为字符串型。然后将学生表中的“专业”和“地址”列删除。
答：==不论基本表中原来是否已有数据，新增加的列一律为空值==。
```sql
alter table Student
add (
	Sentrance date,
	Subject varchar(20), 
    Saddr varchar(20)
);
alter table Student drop Subject;
alter table Student drop Saddr;
```

【例3.9】将年龄的数据类型由字符型（假设原来的数据类型是字符型）改为整数型。
答：注意，==修改原来的列定义可能会破坏已有的数据==。
```sql
alter table Student
alter column Sage int;
```
【例3.10】增加课程名称必须取唯一值的约束条件。
答：
```sql
alter table Course
add unique(Cname);
```

### 5. 删除基本表
当某个基本表不再需要时，可以使用 `drop table` 语句删除它。其一般格式为：
```sql
drop table <表名> [cascade | restrict] [IF EXISTS 表名];
```
（**默认情况是 `restrict`** ）若选择了 `restrict` ，则该表的删除是有限制条件的——欲删除的基本表不能被其他表的约束所引用（如 `check, foreign key` 等约束）、不能有[视图](https://memcpy0.blog.csdn.net/article/details/120905098)、不能有[触发器](https://memcpy0.blog.csdn.net/article/details/121433913)（`trigger`）、不能有[存储过程或函数](https://memcpy0.blog.csdn.net/article/details/121434216)等。如果存在这些依赖该表的对象，则此表不能被删除。只有先清除表中的全部记录行数据，以及在该表上建立的索引、视图等数据库对象，解除和其他表之间的引用关系后，才能够删除这个空表。

若选择了 `cascade` ，则该表的删除没有限制条件。在删除基本表的同时，相关的依赖对象如视图等，都将被一起删除。

【例3.11】删除 `Student` 表。
答：`Student` 表一旦被删除，表中的数据、建立的索引、视图都会自动被删除。
```sql
drop table Student cascade;
```
**基本表定义一旦被删除，不仅表中的数据和此表的定义将被删除，而且此表上建立的索引、触发器等对象一般也都将被删除**。有的关系DBMS还会同时删除在此表上建立的视图。如果欲删除的基本表被其他基本表所引用，则这些表也可能被删除。例如 `SC` 表通过外键 `Sno` 引用 `Student` 表，则执行【例3.11】后 `Student` 表被删除，`SC` 也被级联删除。因此，==执行删除基本表的操作时一定要格外小心==。

【例3.12】若表上建有视图，选择 `restrict` 时表不能删除；选择 `cascade` 时可以删除表，视图也自动被删除。
答：
```sql
create view IS_Student /* Student表上建立视图 */
as
select Sno, Sname, Sage
from Student
where Sdept = 'IS';

drop table Student restrict; /* 删除Student表 */
-- ERROR: cannot drop table Student because other objects depend on it
/* 系统返回错误信息,存在依赖该表的对象,此表不能被删除 */

drop table Student cascade; /* 删除Student表 */
-- NOTICE: drop cascades to view IS_Student 
/* 系统返回提示:此表上的视图也被删除 */

select * from IS_Student;
-- ERROR: relation "IS_Student" does not exist
```

> 注意：不同的数据库产品，在遵循SQL标准的基础上，具体实现细节和处理策略会与标准有差别。
> <b></b>
> 下面就SQL 2011标准对 `drop table` 的规定，对比分析Kingbase ES、Oracle 12c Release1(12.1)、MS SQL Server 2012这三种数据库产品对 `drop table` 的不同处理策略。表3.5中的 `R` 表示 `restrict` ，即 `drop table <基本表名> restrict;` ；`C` 表示 `cascade` ，即 `drop table <基本表名> cascade;`  。其中Oracle 12c没有 `restrict` 选项；SQL Server没有 `restrict` 和 `cascade` 选项：
> <img src="https://img-blog.csdnimg.cn/dda2df7e70094fe7925e8b3136181b9e.png#pic_center" width="64%">
> `X` 表示不能删除基本表，`√` 表示能删除基本表，“保留”表示删除基本表后，还保留依赖对象。从比较表中可以知道：
> （1）对于索引，删除基本表后，这三个关系DBMS都自动删除该基本表上已建立的所有索引；
> （2）对于视图，Oracle 12c与SQL Server 2012是删除基本表后，还保留此基本表上的视图定义，但是已经失效；Kingbase ES分两种情况：若删除基本表时带 `restrict` 选项，则不可以删除基本表；若删除基本表时带 `cascade` 选项，则可以删除基本表，同时也删除视图。Kingbase ES的这种策略符合SQL 2011标准。
> （3）对于存储过程和函数，删除基本表后，这三个数据库产品都不自动删除建立在此基本表上的存储过程和函数，但是已经失效。
> （4）如果欲删除的基本表上有触发器，或者被其他基本表的约束所引用（`check, foreign key` 等），可从比较表中得到这三个系统的处理策略，这里就不再赘述了。
> <b></b>
> 同样，**对于其他的SQL语句，不同数据库产品在处理策略上会与标准有所差异**。因此，如果后续的示例在某个数据库产品上不能运行时，要参见对应的用户手册，并做适当修改。

---
## 3.3.3 索引的建立与删除
当表的数据量比较大时，查询操作会比较耗时。**建立索引是加快查询速度的有效手段**。数据库索引类似于图书后面的索引，能快速定位到需要查询的内容。用户可以根据应用环境的需要，在基本表上建立一个或多个索引，以提供多种存取路径，加快查找速度。
> SQL新标准不主张使用索引，而是在创建表时定义主键，系统会自动在主键上建立索引。

数据库索引有多种类型，常见索引包括**顺序文件上的索引、B+树索引、散列 `hash` 索引、位图索引**等：
- ==顺序文件上的索引==是针对指定属性值升序或降序存储的关系，在该属性上建立一个顺序索引文件，索引文件由属性值和相应的元组指针组成；
- ==B+树索引==是将索引属性组织成B+树形式，B+树的叶子节点为属性值和相应的元组指针。B+树索引具有动态平衡的优点。
- ==散列索引==是建立若干个桶，将索引属性按照其散列函数值映射到相应桶中，桶中存放索引属性值和相应的元组指针。散列查找具有查找速度快的特点。
- ==位图索引==是用位向量记录索引属性中可能出现的值，每个位向量对应一个可能值。

索引虽然能够加速数据库查询，但需要占用一定的存储空间。当基本表更新时，索引也要进行相应的维护。这些都会增加数据库的负担，因此要根据实际应用的需要，有选择地创建索引。

目前，SQL标准中没有涉及索引，但是商用关系DBMS一般都支持索引机制，只是不同的关系DBMS支持的索引类型不尽相同。

一般来说，建立与删除索引由数据库管理员或表的属主 `owner`（即建立表的人），负责完成。==关系DBMS在执行查询时，会自动选择合适的索引作为存取路径，**用户不必也不能显式地选择索引**==。索引是关系DBMS的内部实现技术，属于内模式的范畴。
### 1. 建立索引
在SQL语言中，建立索引使用 `create index` 语句，其一般格式为：
```sql
create [unique] [cluster] index <索引名> 
on <表名>
(<列名> [asc | desc][, <列名> [asc | desc]] ...);
```
其中，`<表名>` 是要建立索引的基本表的名字。索引可以建立在该表的一列或多列上，各列之间用逗号分隔。每个 `<列名>` 后面还可用 `<次序>` 指定索引值的排列次序，可选 `asc`（升序）或 `desc`（降序），默认值为 `asc` 。

`unique` 表明**此索引的每个索引值只对应唯一的数据记录**；`cluster` 表示**要建立的索引是聚簇索引**，即**索引项的顺序与表中记录的物理顺序一致**——注意，在一个基本表上**最多只能建立一个聚簇索引**；更新聚簇索引列数据会导致表中记录物理顺序的变更，代价较大，**经常更新的列不宜建立聚簇索引**。关于更多聚簇索引的信息，见[【数据库系统】第二部分 设计与应用开发(7) 数据库设计](https://memcpy0.blog.csdn.net/article/details/121718620)7.5.2小节关系模式存取方法。

【例3.13】为学生-课程数据库中的 `Student` 、`Course` 和 `SC` 三个表建立索引。其中 `Student` 表按学号升序建立唯一索引，`Course` 表按课程号升序建立唯一索引，`SC` 表按学号升序和课程号降序建立唯一索引。
答：
```mysql
create unique index Stusno on Student(Sno);
create unique index Coucno on Course(Cno);
create unique index SCno on SC(Sno asc, Cno desc);
```
### 2. 修改索引
对于已经建立的索引，如果需要对其重新命名，可以使用 `alter index` 语句。其中一般格式为：
```sql
alter index <旧索引名> rename to <新索引名>;
```
【例3.14】将 `SC` 表的 `SCno` 索引名改为 `SCSno` 。
答：
```sql
alter index SCno rename to SCSno;
```
### 3. 删除索引
索引一经建立就由系统使用和维护，不需要用户干预。建立索引是为了减少查询操作的时间，但如果数据增、删、改频繁，系统会花费许多时间来维护索引，从而降低了查询效率。这时，就可以删除一些不必要的索引。

在SQL中，删除索引使用 `drop index` 语句，其一般格式为：
```sql
drop index <索引名>;
```
【例3.15】删除 `Student` 表的 `Stusname` 索引。
答：
```sql
drop index Stusname;
```
==删除索引时，系统会同时从数据字典中删去有关该索引的描述==。

---
## 3.3.4 域类型的定义
域类型是用于**建立用户自定义数据类型**的一个特定的数据类型，由带有**约束**的**数据类型**和**默认值**一起构成。域类型的定义语法如下：
```sql
create domain <域名> [as] <数据类型>
[DEFAULT <缺省值>] [<域约束>];
```
例如，建立一个建筑公司的、由3个表组成的数据库模式。其中创建了一个域类型，为 `item_id`，允许值为 $4$ 位数字，默认值为零，并限制其值非空。**可以在多个表中使用该域类型，无需重复定义；以后改变了域类型，该变化会自动施加到使用该域类型的所有表中**。
```sql
create domain item_id 
	numeric(4) default 0
	check (value is not null); 
```
**域的完整性约束**见[【数据库系统】第一部分 数据库基础(5) 数据库完整性](https://memcpy0.blog.csdn.net/article/details/121433913)5.5节。
 
---
## 3.3.5 数据字典
**数据字典是关系DBMS内部的一组系统表**（见[【数据库系统】第三部分 数据库系统(12) 数据库管理系统](https://memcpy0.blog.csdn.net/article/details/121729382)中**数据字典的组织**一小节），==它记录了数据库中所有的定义信息，包括关系模式定义、视图定义、索引定义、完整性约束定义、各类用户对数据库的操作权限、统计信息等==。关系DBMS在执行SQL的数据定义语句时，实际上就是在更新数据字典表中的相应信息。在进行查询优化和查询处理时，数据字典中的信息是其重要依据。


