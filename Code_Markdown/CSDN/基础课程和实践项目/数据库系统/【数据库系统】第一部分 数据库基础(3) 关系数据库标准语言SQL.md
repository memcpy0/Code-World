> 本文属于「数据库系统」系列文章之一，这一系列着重于「数据库系统知识的学习与实践」。由于文章内容随时可能发生更新变动，欢迎关注和收藏[数据库系统系列文章汇总目录](https://memcpy0.blog.csdn.net/article/details/119996493)一文以作备忘。需要特别说明的是，为了透彻理解和全面掌握数据库系统，本系列文章中参考了诸多博客、教程、文档、书籍等资料，限于时间精力有限，这里无法一一列出。部分重要资料的不完全参考目录如下所示，在后续学习整理中还会逐渐补充：
> - 数据库系统概念 第六版 `Database System Concepts, Sixth Edition` ，作者是 `Abraham Silberschatz, Henry F. Korth, S. Sudarshan` ，机械工业出版社
> - 数据库系统概论 第五版，王珊 萨师煊编著，高等教育出版社


@[toc]
 
---
**结构化查询语言** `Structured Query Language, SQL` 是**关系数据库的标准语言**，也是一个**通用的、功能极强的关系数据库语言**。其功能不仅仅是查询，而是包括数据库模式创建、数据库数据的插入与修改、数据库安全性与完整性定义与控制等一系列功能（**数据定义、数据查询、数据操纵、数据控制**）。

本章详细介绍**SQL的基本功能**，并进一步讲述**关系数据库的基本概念**。
# 本章参考文献
<img src="https://img-blog.csdnimg.cn/57a5f92031e742adbc4ce6c6028a49a0.png#pic_center" width="60%">

---
# 3.1 SQL概述
自SQL成为国际标准语言以后，各个数据库厂家纷纷推出各自的SQL软件或与SQL的接口软件。这就使大多数数据库均用SQL，作为共同的数据存取语言和标准接口，使不同数据库系统之间的互操作有了共同的基础。SQL已成为数据库领域的主流语言，其意义十分重大。有人把确立SQL为关系数据库语言标准及其后的发展，称为是一场革命。
## 3.1.1 SQL的产生与发展
SQL是在1974年，由 `Boyce` 和 `Chamberlin` 提出的，最初叫 `Sequel` ，并在IBM公司研制的**关系数据库管理系统原型 `System R`** 上实现。由于SQL简单易学、功能丰富，深受用户及计算机工业界欢迎，因此被数据库厂商所采用。经过各个公司的不断修改、扩充和完善，SQL得到业界的认可。

1986年10月，[**美国国家标准局**](https://www.ansi.org/) `American National Standard Institute, ANSI` 的数据库委员会X3H2，批准了SQL作为关系数据库语言的美国标准，同年公布了SQL标准文本（简称SQL-86）。1987年，[**国际标准化组织**](https://www.iso.org/home.html) `International Organization for Standardization, ISO` 也通过了这一标准。

SQL标准从公布以来，随数据库技术的发展而不断发展、不断丰富，表3.1是SQL标准的进展过程：
<img src="https://img-blog.csdnimg.cn/cd18a45177c5498c92bd3535b388bf34.png#pic_center" width="45%">
SQL/86和SQL/89都是单个文档，SQL/92和SQL 99已经扩展为一系列开放的部分——其中SQL/92除了SQL基本部分外，还增加了SQL调用接口、SQL永久存储模块；而SQL 99则进一步扩展为框架、SQL基础部分、SQL调用接口、SQL永久存储模块、SQL宿主语言绑定、SQL外部数据的管理和SQL对象语言绑定等多个部分（SQL 99合计超过1700页！）。2008年、2011年又对SQL 2003做了一些修改和补充。不难发现，**SQL标准的内容越来越丰富，也越来越复杂**。

==目前，没有一个数据库系统能够支持SQL标准的所有概念和特性==。大部分数据库系统能支持SQL/92标准中的大部分功能，以及SQL 99、SQL 2003中的部分新概念。==同时，许多软件厂商对SQL基本命令集，还进行了不同程度的扩充和修改，又可以支持标准以外的一些功能特性==。本文不介绍完整的SQL，只介绍SQL的基本概念和基本功能，因此在使用具体系统时，要查询各产品的命令手册。
## 3.1.2 SQL的特点
SQL之所以能够为用户和业界所接受并成为国际标准，是因为它是一个综合统一的、功能极强同时又简洁易学的语言。SQL集 **数据查询 `data query`**、**数据操纵 `data manipulation`**、**数据定义 `data definition`** 和**数据控制 `data control`** 于一体，其主要特点包括以下几部分。
### 1. 综合统一
数据库系统的主要功能，是通过数据库支持的**数据语言**来实现的。非关系模型（层次模型、网状模型）的数据语言一般都分为：
（1）模式数据定义语言 `Schema Data Definition Language` ，模式DDL；
（2）外模式数据定义语言 `Subschema Data Definition Language` ，外模式DDL或子模式DDL；
（3）数据存储有关的描述语言 `Data Storage Description Language, DSDL` 
（4）数据操纵语言 `Data Manipulation Language, DML` 
它们分别用来定义模式、外模式、内模式和进行数据的存取与处置。当用户数据库投入运行后，==如果需要修改模式，必须停止现有数据库的运行，转储数据，修改模式并编译后再重装数据库，十分麻烦==。

==SQL集**数据定义语言、数据操纵语言、数据控制语言**的功能于一体，语言风格统一，可以独立完成数据库生命周期的全部活动==，包括以下一系列操作要求：
- 定义和修改、删除关系模式，定义和删除视图，插入数据，建立数据库；
- 对数据库中的数据进行查询和更新；
- 数据库重构和维护；
- 数据库安全性、完整性控制，以及事务控制；
- 嵌入式SQL和动态SQL定义。

==这就为数据库应用系统的开发提供了良好的环境==。特别是用户在数据库系统投入运行后，还可根据需要随时地、逐步地修改模式，并不影响数据库的运行，从而使系统具有良好的可扩展性。

另外，在关系模型中，实体和实体间的联系均用关系表示，这种数据结构的单一性带来了数据操作符的统一性，查找、插入、删除、更新等每一种操作都只需一种操作符，从而==克服了非关系系统由于信息表示方式的多样性带来的操作复杂性==——例如，在DBTG网状数据库系统中，需要两种插入操作符：`store` 用来把记录存入数据库，`connect` 用来把记录插入系值（系值是网状数据库中记录之间的一种联系方式）、以建立数据之间的联系。
### 2. 高度非过程化
非关系数据库的数据操纵语言是「面向过程」的语言，用「过程化语言」完成某项请求，必须指定存取路径。而用SQL进行数据操纵时，==只要提出“做什么”、而无须指明“怎么做”，因此无须了解存取路径。存取路径的选择、以及SQL的操作过程，由系统自动完成==。这不但大大减轻了用户负担，而且有利于提高数据独立性。
### 3. 面向集合的操作方式
==非关系数据模型，采用的是「面向记录」的操作方式，操作对象是一条记录==。例如查询所有平均成绩在80分以上的学生姓名，用户必须一条一条地把满足条件的学生记录找出来（通常要说明具体处理过程，即按照哪条路径、如何循环等）。

而==SQL采用的是「面向集合」的操作方式==，不仅操作对象、查找结果可以是元组的集合，而且一次插入、删除、更新操作的对象，也可以是元组的集合。
### 4. 以同一种语法结构，提供多种使用方式
==SQL既是独立的语言，又是嵌入式语言==。作为独立的语言，它能够独立地用于联机交互的使用方式，用户可以在终端键盘上直接键入SQL命令，对数据库进行操作；作为嵌入式语言，SQL语句能够嵌入到高级语言（如C、C++、Java）程序中，供程序员设计程序时使用。

而在两种不同的使用方式下，==SQL的语法结构基本上是一致的==。这种以统一的语法结构、提供多种不同使用方式的做法，提供了极大的灵活性和方便性。
### 5. 语言简洁，易学易用
SQL功能极强，但由于设计巧妙，语言十分简洁，完成核心功能只用了9个动词，如表3.2所示。SQL接近英语口语，因此易于学习和使用。
| SQL功能  |           核心动词           |
  | :------: | :--------------------------: |
  | 数据查询 |           `SELECT`           |
  | 数据定义 |  `CREATE`，`DROP`，`ALTER`   |
  | 数据操作 | `INSERT`，`UPDATE`，`DELETE` |
  | 数据控制 |      `GRANT`，`REVOKE`       |


## 3.1.3 SQL的基本概念
**支持SQL的关系DBMS，同样支持关系数据库三级模式结构**。如图3.1所示，其中外模式包括若干视图 `view` 和部分基本表 `base table` ，数据库模式包括若干基本表，内模式包括若干存储文件 `stored file` 。
<img src="https://img-blog.csdnimg.cn/d6d8907c111d4e57887725d1abcaa14b.png#pic_center" width="55%">

用户可以用SQL，对基本表和视图进行查询或其他操作：
- **基本表和视图一样，都是关系**。基本表是本身独立存在的表，在关系DBMS中一个关系就对应一个基本表，一个或多个基本表对应一个存储文件，一个表可以带若干索引，索引也存放在存储文件中。
- **存储文件的逻辑结构，组成了关系数据库的内模式**。存储文件的物理结构，对最终用户是隐蔽的。
- **视图是从一个或几个基本表导出的表**。它本身不独立存储在数据库中，即数据库中只存放视图的定义，而不存放视图对应的数据。这些数据仍存在于导出视图的基本表中，因此**视图是一个虚表**。视图在概念上与基本表等同，用户可在视图上再定义视图。
-  用户可以用SQL语言对**视图和基本表**进行查询。==在用户眼中，**视图和基本表都是关系**，而存储文件对用户是透明的==。 

下面逐一介绍各个SQL语句的功能和格式。为了突出基本概念和基本功能，略去了很多语法细节；且各关系DBMS产品在实现标准SQL时各有差别，与SQL标准的符合程度也不相同，一般在85%以上。因此，具体使用某个关系DBMS产品时，应参阅系统提供的有关手册。

## 3.1.4 SQL语言组成和语句类型
**SQL可以分为数据定义DDL、数据查询DQL、数据更新DML、数据控制DCL四大部分**，人们有时把数据更新称为数据操纵，或把数据查询与数据更新合称为数据操纵。主要介绍前面三部分的内容。
- 数据定义语言 *Data Definition Language, DDL* ，**用来创建、更新、撤销各种数据库中的对象，包括数据库模式、表、视图、列、索引、域、触发器、自定义类型**。
- 数据操作语言 *Data Manipulation Language, DML* ，用来对数据库中表的数据进行增删改。DML简单理解就对表中数据进行增删改。
- 数据查询语言 *Data Query Language, DQL* ，用来查询数据库中表的记录（数据）。DQL简单理解就是对数据进行查询操作。从数据库表中查询到我们想要的数据。  
- 数据控制语言 *Data Control Language, DCL* ，用来定义数据库的访问权限和安全级别及创建用户、事务管理、控制SQL语句集的运行。**DML简单理解就是对数据库进行权限控制**。比如我让某一个数据库表只能让某一个用户进行操作等。
 
---
# 3.2 学生-课程数据库
本文主要以学生-课程数据库为例，讲解SQL的数据定义、数据操纵、数据查询和数据控制语句。为此，首先定义一个学生-课程模式 `ST`（定义模式的操作，见3.3.1节中【例3.1】），学生-课程数据库中包括以下三个表（定义表的操作，见3.3.2节中【例3.5】、【例3.6】、【例3.7】），表中的下划线表示关系的主键。
- 学生表：$Student(\underline{Sno}, Sname, Ssex, Sage, Sdept)$
- 课程表：$Course(\underline{Cno}, Cname, Cpno, Ccredit)$ 
- 学生选课表：$SC(\underline{Sno, Cno}, Grade)$ 

各个表中的数据示例，如下图所示：
<img src="https://img-blog.csdnimg.cn/2c493492d09a40918dc7e1c836f8170e.png#pic_center" width="42%"><img src="https://img-blog.csdnimg.cn/9ac73ea678314518854bab73f5daad0f.png" width="40%"><img src="https://img-blog.csdnimg.cn/8527f857191045b0ba5ac578323dc9c3.png" width="51%">

---
在后续学习笔记中，会给出许多SQL语句的语法格式，其中一些符号的含义如下：
- `[...]`：表示其中的成分为任选项。
- `<...>`：表示其中的成分由用户具体给定。
 - `|` ： 表示其中并列的成分只能择一。

> SQL 语句可以单行或多行书写，以分号结尾才是一个完整的SQL语句。 
> MySQL 数据库的 SQL 语句不区分大小写，关键字建议使用大写。  
> 单行注释：`-- 注释内容` 或 `#注释内容`（MySQL特有） 。注意：使用 `-- ` 添加单行注释时，`--` 后面一定要加空格，而 `#` 没有要求。
> 多行注释：`/* 注释 */` 
 

# 3.3 数据定义
[【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL(3) 数据定义](https://memcpy0.blog.csdn.net/article/details/121881755)

# 3.4 数据查询
[【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL(4) 数据查询](https://memcpy0.blog.csdn.net/article/details/121856867)

# 3.5 数据更新
[【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL(5) 数据更新](https://memcpy0.blog.csdn.net/article/details/120997889)

# 3.6 空值的处理
[【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL(6) 空值的处理](https://memcpy0.blog.csdn.net/article/details/120913964)

# 3.7 视图
[【数据库系统】第一部分 数据库基础(3) 关系数据库标准语言SQL(7) 视图](https://memcpy0.blog.csdn.net/article/details/120905098) 



