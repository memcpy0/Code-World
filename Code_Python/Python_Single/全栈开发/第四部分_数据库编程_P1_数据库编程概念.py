# -*- coding:utf-8 -*-
"""
author      : myRealization
create_time : 2018-10-11 21:40
program     : 学习数据库的基础概念
"""  #

"""
https://www.cnblogs.com/majj/p/9160383.html
MySQL 关系型数据库
1.
  - 服务端软件
    - 服务端程序
    - 解析指令
    - 对文件的操作
    
  - 客户端软件
    - 客户端程序
    - 发送指令(SQL)
    - 解析指令
    
想要使用MySQL来存储并保存数据，则需要做几件事情：
　　a. 安装MySQL服务端
　　b. 安装MySQL客户端
　　b. 【客户端】连接【服务端】
　　c. 【客户端】发送命令给【服务端MySQL】服务的接受命令并执行相应操作(增删改查等)

Model-View-Controller

2. 安装mysql服务
    * 添加系统环境变量 
    * 初始化 mysqld --initialize-insecure
    * 开启服务端 mysqld
    * 客户端连接服务端 mysql -uroot -p
    注:program data中MySQL/data存储了mysql的数据
    注:tasklist | findstr mysql 查看进程号
       ......

3. SQL语句
    1. 创建用户
        # 指定IP:127.0.0.1的mjj用户登录
        create user 'mjj'@'127.0.0.1' identified by '123';
        # 指定IP:127.0.0.开头的mjj登录
        create user 'mjj'@'127.%.%.%" identified by '123';
        # 指定任何IP的mjj用户登录
        create user  'mjj'@'%' identified by '123';
        
    2. 对当前用户授权操作
        * 只有root用户拥有授权其他用户的权限
        # 授予用户mjj对db1数据库的s1表拥有选择插入和更新的权限
        grant select, insert, update on db1.s1 to "mjj"@"%";
        # 授予mjj用户对db1的表t1拥有所有的权限
        grant all privileges on db1.t1 to 'mjj'@'%';
        # 授予mjj用户对db1的所有表拥有所有的权限
        grant all privileges on db1.*  to "mjj"@"%";
        授予mjj用户对所有数据库的所有表拥有所有的权限
        grant all privileges on *.* to "mjj"@"%";
       取消用户权限
        revoke select on db1.s1 from "mjj"@"%";
        revoke all privileges on db1.t1 from "mjj"@"%";
       刷新权限
        flush privileges;
        
    3. 其他语句
        use db1;          使用数据库db1
        
        show databases;   查看所有的数据库
        create database db1;   创建数据库db1
        show create database db1;  查看已创建的数据库db1

        show tables;       查看所有的表
        create table s1(id int, name char(10));
        show create table s1;      查看已经创建的数据表s1;
        desc s1;            查看已经创建的数据表s1的详情;   
        
        增:
          insert into s1(id, name) values(1, 'alex'), (2, 'naiz');
        删:
          drop database 数据库名;  删除数据库
          delete from s1 ...; /delete from db1.s1 ...; 如果有自增id, 新增的数据仍然是以删除前的最后一样作为起始点
          truncate tablename;  数据量大, 删除速度比上一条快, 且直接从零开始 
        改:
          update db1.s1 set k="v";
          
          alter database 数据库名 charset utf8;  修改数据库名的字符集
        查:
          select * from tablename;  从某表中选择所有数据(展示)
          单表查询:
              select 字段1, 字段2, ... from 表名 
                                      where 条件
                                      group by field
                                      having 条件
                                      order by field
                                      limit 限制条数
              - where 约束
                1. 比较运算符: > < >= <= <> != =
                2. between ... and ...
                3. in(10,20,30)  值是10/20/30
                4. like "pattern" pattern可以是%或者是_, %任意多个字符 _表示一个字符
                5. 逻辑运算符: 多个条件之间可以使用and or not
              - group by 分组查询
                select @@sql_mode;
                select post.min(salary) from employer group by post; --> 临时表
                select A.c from (select post, max(salary) as c from employer group by post) as A;
                使用分组, 先sql_mode='ONLY_PULL_GROUP_BY'
              发生在where之后, 基于where的结果分组, 分组后只能查询分组的字段, 如果想查询组内的其他字段, 必须借助
              聚合函数:
                max() sum()
                min() count()
                avg()
              - having 二次筛选 只能使用分组查询使用的字段或聚合函数查询的字段
              - order by field1 asc, field2 desc 先按照field1排序, field1相同再按照field2排序
              - limit 
          
          多表查询
              1. 多表连接查询
                 select * from employee, department;  笛卡尔积
                 select * from employee, department where employee.depid=department.id;
              2. 内连接: select * from employee inner join department on employee.depid=department.id;
              3. 外连接:
                    左连接: select * from employee left join department on employee.depid=department.id;
                    右连接: select * from employee right join department on employee.depid=department.id;
              4. 全外连接: 在内连接的基础上增加左边有而右边没有 增加右边有而左边没有的内容 
                 select * from employee left join department on employee.depid=department.id
                 union
                 select * from employee right join department on employee.depid=department.id   
          符合条件连接查询
          子查询
              是指将一个查询语句嵌套在另一个查询语句中, 内层查询的结果作为外层查询的条件
              可以包含: in, not in, any, all, exists, not exists等关键字
              还可以包含比较运算符 = != < >等
              
              
        从另一个数据库复制表:
          create table d1 select * from db1.tablename; 即复制了表结构，也有表数据
          create table d1 select * from db1.tablename where 1>5; 只复制了表结构
          ==> create table d1 like db1.tablename;      同样只复制了表结构          
          
          
    4. 存储引擎介绍
http://www.cnblogs.com/majj/p/9160831.html
        数据库中不同的表有不同的类型, 表的类型不同, 会对应mysql不同的存取机制, 表类型又称为存储引擎
    即存储和操作数据表的类型。
        MySQL支持接口: C JDBC ODBC NET PHP Python Perl Ruby VB
        MySQL Server: 连接池:  验证和授权-线程-连接限制-内存与缓存管理
                      SQL接口 解析器 优化 缓存和缓存池
                            可插式存储引擎
                        文件系统    文件和日志
        InnoDB 
        MyISAM    不产生引擎事务, 数据插入速度极快, 为方便快速插入千万条数据
        Memory   
        BLACKHOLE
        Infobright
        NTSE
        
    5. 数据类型介绍
https://www.cnblogs.com/majj/p/9164480.html
      1. 数字           有符号(默认)  无符号+unsigned
        TINYINT     1
        SMALLINT    2
        MEDIUMINT   3
        INT         4
        BIGINT      8
        
        FLOAT       4
        DOUBLE      8
        
        DECIMAL(整数数位<=65, 小数点后个数) 小数推荐 始终精准 内部是用字符串去存
      * int类型后面的(num)是指显示宽度, 而不是存储宽度!
      
      2. 字符串
        char(num)     简单粗暴 浪费空间 存取速度快  0-255(一个中文是一个字符, UTF8中的3个字节)
        varchar(num)  精准 节省空间 存取速度慢     0-65535
        sql优化  创建表时 定长的类型往前放，变长的类型往后放
      * length(): 查看字节数   char_length(): 查看字符数
      * char填充空格来满足固定长度, 但是在查询时却很不要脸的删除尾部的空格(装作自己好像没有浪费空间一样)
        SELECT sql_mode;                        查看sql_mode;
        SET sql_mode='PAD_CHAR_TO_FULL_LENGTH'; 设置sql_mode;
        varchar会存储已有的空格, 却不会填充空格来满足所谓的固定长度, 是变长的
        
      3. 时间类型
        YEAR
        DATE
        TIME
        datetime  YYYY-MM-DD HH:MM:SS  * now()
        timestamp
        
      4. 枚举类型和集合
        enum()  多选一 如enum('male', 'female'); 
        set()   多选一或多选多
        
    6. 约束条件 用于保证数据的完整性和一致性
    完整性约束
        是否可空 
        - not null 不可空
          null     可空
        默认值
        - default  创建列时可以指定默认值, 当插入时未指定则设为默认值
            nid不能为空, 为空则默认为2
                create table tb1 (
                    nid int not null default 2,
                    num int not null,
                );
        - unique
        不同的唯一的:
            单列唯一
                只有一列是唯一
                create table tb2 (
                    nid int,
                    name char(10) unique,
                );
                insert into tb2 values(1, 'it'),(2, 'it2');
            多列唯一
                每个字段都设置unique, 即每个字段插入的值都不同的时候才能插入
                create table tb2 (
                    nid int unique,
                    name char(10) unique,
                );
                insert into tb2 values(1, 'it'),(2, 'it2');
              或:
                create table tb2 (
                    nid int,
                    name char(10),
                    unique(nid),
                    unique(name),
                );
                insert into tb2 values(1, 'it'),(2, 'it2');
            组合唯一
                只要有一个字段的值不同就可以插入
                create table tb2 (
                    nid int,
                    name char(10),
                    unique(nid, name),
                );
                insert into tb2 values(1, 'it'),(2, 'it');
        - primary key (== not null + unique)
            一个表中只允许存在一个主键: (主键不能为空且独一无二)
            单列做主键 或多列做主键(复合主键)
                create table tb2 (
                    nid int primary key,
                    name char(10) not null,
                );
                
        - auto-increment 自增长 主要用在主键字段上面
            create table tb2 (
                nid int primary key auto_increment,
                name char(10) not null,
            );
            mysql> show variables like "auto_inc%";
                +--------------------------+-------+
                | Variable_name            | Value |
                +--------------------------+-------+
                | auto_increment_increment | 1     |
                | auto_increment_offset    | 1     |
                +--------------------------+-------+
            自增长的步长auto_increment_increment默认为1
            起始的偏移量auto_increment_offset默认是1
            
            设置步长 为会话设置 只在本次连接中有效
                set session auto_increment_increment=5;
            全局设置步长 都有效
                set global auto_increment_increment=5;
            设置起始偏移量
                set global auto_increment_offset=3;
        - foreign key
            
        
    7. 外键的使用和变种(让两张表发生关系)
        创建表时先创建被关联表, 再创建关联表
        插入记录时先在被关联表中插入记录, 再往关联表中插入记录
        constraint fk_dep foreign key(从表字段) reference 主表(关键字)
        on delete cascade  同步删除
        on update cascade  同步更新
        
        - 一对多 
        - 多对多 
        - 一对一
        1. 站在左表的角度去找
        2. 站在右表的角度去找
        只有1和2同时满足才能多对多
    8. 索引
        Btree树
      掌握：
        #1. 测试+链接数据库
        #2. 新建库
        #3. 新建表，新增字段+类型+约束
        #4. 设计表：外键
        #5. 新建查询
        #6. 备份库/表
        
        #注意：
        批量加注释：ctrl+？键
        批量去注释：ctrl+shift+？键
        1. 常见的几种索引
            - 普通索引 仅有一个加速查找
                create index 索引的名字 on 表名(列名);
                drop index 索引的名字 on 表名;
                show index from 表名;
            - 唯一索引
                create unique index 索引的名字 on 表名(列名);
                drop index 索引的名字 on 表名;
                show unique index from 表名; 
            - 主键索引 加速操作和唯一约束
                 创建主键+添加其他字段为唯一索引
                 drop index 索引的名字 on 表名;
            - 联合索引
                - 联合普通索引
                - 联合主键索引
                - 联合唯一索引
        2. 索引名词
            覆盖索引
            索引合并: 把多个单列索引合并使用
        3. 注意:
            组合索引代替多个索引(经常使用多个条件查询时)
            尽量使用短索引
            使用连接Join代替子查询
    http://cnblogs.com/p/.html
    9. 执行计划
    explain + SQL查询语句; 评估性能
        性能 type:
          all < index < range < index_merge < ref_or_null<ref<eq_ref < system/const
        all: 全表扫描, 对数据表从头到尾找一遍
        index: 全索引扫描, 对索引从头到尾查找一遍
        range: 对索引进行范围查找
        index_merge: 合并索引
        ref:   根据索引查找一个或多个值
        eq_ref:连接时使用primary key或unique类型
        const: 表最多有一个匹配行, 因为仅有一行, 所以很快
        system:
            系统:表仅有一行, 系统表, 这是const联接类型的一个特例
    
    10. 分页查询性能优化
        select filed1,... from tablename 
            where 
            group by
            having
            order by 
            limit
"""
# 导入模块
import pymysql

# user = input("请输入用户名:")
# password = input("请输入密码:")

# 创建连接
connection = pymysql.connect(host="127.0.0.1", user='root', password="wdMySQLmm123",
                database="book", port=3306, charset='utf8')
# 创建游标
cur = connection.cursor(cursor=pymysql.cursors.DictCursor)

# sql = "select * from  bookinfo where bookname=%(bookname1)s and bookpsd=%(bookpsd1)s"

# 执行mysql 防止mysql注入
# res = cur.execute(sql, ('爱丽丝梦游仙境', '123'))
# res = cur.execute(sql, ['爱丽丝梦游仙境', '123'])
# res = cur.execute(sql, {'bookname1':'爱丽丝梦游仙境', 'bookpsd1':'123'})
# sql = "insert into bookinfo(bookname, bookpsd) values(%s, %s)"

sql = "select * from bookinfo"
resCount = cur.execute(sql)

# relative
# absolute  第一个数是移动的行数, 整数为向下移动, 负数为向上移动, mode指定了是相对于当前位置移动, 还是相对于首行位置移动
cur.scroll(1, mode='absolute')
# 插入数据、删除数据、更新一定要提交
# connection.commit()

# res = cur.fetchone()
res = cur.fetchone()
print(res)

cur.close()
connection.close()

if resCount:
    print("登录成功")
