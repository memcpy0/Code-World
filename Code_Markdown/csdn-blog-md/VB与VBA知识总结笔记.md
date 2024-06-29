# Visual Basic和Visual Basic Application知识总结
@[toc]

---
VB和VBA的区别在于没有区别，VBA只不过是VB用于操作Office系列办公软件的延伸。

个人认为纯粹的VB已经没有太大的价值了，可以方便地写几个小的GUI软件(大一点的还是用C#吧)，但是VBApplication还是有必要学的，**微软Office办公软件套装几乎是我们以后工作中必不可少的部分**，能够**自己写逻辑就代表着可以不必反复用鼠标操作软件界面，做到办公半自动化**，还是有一些价值的；而且学起来也很简单，当作脚步语言来学就可以了。

一个有意思的示例是从PPT提取文字到文档中：
```c
Sub Main()
    On Error Resume Next
    Dim temp As New Word.Document, tmpShape As Shape, tmpSlide As Slide
    For Each tmpSlide In ActivePresentation.Slides
        For Each tmpShape In tmpSlide.Shapes
            temp.Range().Text = temp.Range() + tmpShape.TextFrame.TextRange.Text
        Next tmpShape
    Next tmpSlide
    temp.Application.Visible = True
End Sub
```
# 第一部分 Visual Basic简介及Excel 介绍
## 1.1 Basic语言发展史
1964年BASIC语言正式发布。是由达特茅斯学院院长、匈牙利人约翰·凯梅尼与数学系教师托马斯·卡茨共同研制出来的。该语言只有26个变量名，17条语句，12个函数和3个命令，这门语言叫做“**初学者通用符号指令代码**”—Beginners All Purpose Symbolic Intruction Code。
## 1.2 VB的主要功能和特点
- **可视化的设计平台**：可视化编程环境代码编写简单，功能强大，编程效率高，易于学习掌握.
- **面向对象设计方法**：把程序和数据封装起来作为一个对象，并为每个对象赋予相应的属性.
- **事件驱动编程机制**：通过事件驱动的方式实现对对象的操作.
- **结构化设计语言**.
- **充分利用Windows资源**：支持对象的链接与嵌入（OLE）技术.
- **可以与Office结合**：VBA有利于充分利用Office套装软件，特别是与EXCEL结合，实现高效率办公。
## 1.3 EXCEL介绍
> 添加功能区的“开发工具"选项卡：自定义功能区->勾选开发工具 
### 1.3.1 Excel 中的三种基本对象
- 工作簿：Excel中用来保存并处理工作数据的文件（默认扩展名.xlsx），工作簿默认由3个工作表组成,可增减工作表。用来启动宏的工作簿扩展名为.xlsm。
- 工作表：分为普通工作表(Worksheet)，图表工作表(Chart)。
- 单元格：工作表中输入数据和公式的位置。
单元格地址：**列标+行号**或**R1C1引用样式**
单元格区域：如A1:B6、 1:1（第1行）
使用`名称`代表一个单元格区域
  > 比较：由行列交叉的单元格组成的工作表叫做**普通工作表**；还有独立的图表工作表，和嵌入式图表又有不同。
### 1.3.2 Excel基本操作
1. 数据输入
2. 使用公式与函数
（1）单元格地址及引用
（2）公式
（3）函数
Excel基本操作
Excel的数据类型
文本：自动左对齐，Alt+回车键（单元格内换行）
数字：自动右对齐，如 123、-123、日期、时间
逻辑值：True 和 False
出错值：如#DIV/0! 、#NAME?、#value! 、#N/A错误等
单元格显示“###### ”，表示列宽不够，拉宽即可
向单元格输入数据的两种类型
常量：非“=”开始，如直接输入上述几种类型的数据
公式：以“=”开始，指以公式的形式生成的结果值
1. 数据输入
往单元格中输入数据的方式
每选择一个单元格，输入一个数据
利用自动填充功能
利用公式或函数
单元格内数据的有效性设置
数据 -  数据工具 – 数据验证
例如：成绩只能是0~100之间的数值

1. 数据输入
（1）单元格地址及引用
（2）公式
（3）函数
具体函数的使用作为课外自学内容，不讲
2. 使用公式与函数
（1）单元格地址及引用
相对引用：公式中引用的单元格地址在公式复制时自行调整
绝对引用：公式中引用的单元格地址在公式复制时不改变
	相对地址：B6，A4, C5:F8
	绝对地址：$B$6, $A$4, $C$5:$F$8 
	混合地址：B$6, A$4, C$5:F$8
         	     $B6, $A4, $C5:$F8
外部引用：引用不同工作簿中的单元格
远程引用：引用其它程序中的数据，如Access数据库

[Book1]Sheet2!B6
F4    在相对地址和绝对地址之间切换
Sheet1!$A$1:$C$3
公式是指以“＝”开头，由常量值、单元格引用、名称、函数和运算符组成的序列。
若公式中引用的数值发生改变，公式的结果也随之改变。
### 附录1: EXCEL函数
| 函数名 | 函数说明 | 示例|
|:----|:-----|:--------|
| INT(num) | 将num向下舍入到最接近的整数 | INT(-8.9) = -9|
| ABS(num) | 返回num的绝对值 | ABS(-2) = 2|
| ROUND(num, x) | 将num四舍五入到x个小数位 | ROUND(2.15, 1) = 2.2|
| MAX(n1, n2...) | 返回最大值 | MAX(10, 7, 9) = 10|
|AND(...) | 检验所有参数是否为TRUE，是则返回TRUE| AND(1 < 2, 3 < 5) = TRUE|
| `SUMIF(range, criteria, [sum_range])`| **单条件求和**, 对区域sum_range中在区域range中所对应的单元格满足criteria条件的值求和 | SUMIF(B2:B5, "John", C2:C5) 只对区域 C2:C5 中在区域 B2:B5 中所对应的单元格等于“John”的值求和|
| `COUNTIF(range, criteria)` | **单条件统计**区域range中满足某个条件criteria的单元格数量| =COUNTIF(A2:A5,A4) 统计单元格 A2 到 A5 中包含“桃子”（A4 中的值）的单元格的数量|
|SUMIFS| | |
|SUMPRODUCT|
|`COUNTIFS(criteria_range1, criteria1, [criteria_range2, criteria2],…)`| 将多个条件criteriaN应用于跨多个区域criteria_rangeN的单元格，然后统计满足所有条件的次数；**条件的形式为数字、表达式、单元格引用或文本**；**每一个附加的区域都必须与参数criteria_range1具有相同的行数和列数**；如果所有的第一个单元格都满足其关联条件，则计数增加 1，类推 | |


（2）公式
Ctrl+`（重音符）（数字1键的左侧键）
在公式与结果之间切换
Excel 中的四类运算符
1.算术运算符   2.字符串连接运算符
Excel 中的四类运算符
3.比较运算符
Excel 中的四类运算符
4.引用运算符
（3）函数（预定义的内置公式）

函数名称
各参数的含义和类型
返回值
1. ABS函数、2. AND函数、16. OR函数
6. INT函数、13. MOD函数、ROUND函数
3. AVERAGE函数、19. SUM函数
10. MAX函数、12. MIN函数
17. RANK函数
14. MONTH函数、15. NOW函数
20. 单条件求和SUMIF函数、4. 单条件计数COUNTIF函数
21. 多条件求和和计数 SUMPRODUCT函数
5.  IF函数
7. LEFT函数、11. MID函数、18. RIGHT函数、8. LEN函数
22. VLOOKUP函数、9. LOOKUP函数
常用函数（该部分为课外自学内容）
作用：返回数字的绝对值
语法： ABS(number)
Number  需要计算其绝对值的实数

1. ABS函数

作用：返回逻辑值，参数全真则结果为真，否则为假
语法： AND(logical1,logical2, ...) 
Logical1,…：表示待测试的条件值或表达式

2. AND函数

16. OR函数
作用：返回逻辑值，当所有参数为假时返回假，否则返回真
语法： OR(logical1,logical2, ...)
logical1  测试条件1
logical2  测试条件2
举例：在C6单元格输入公式： =OR(A6>=60,B6>=60)
只要A6和B6单元格的数值，有一个大于等于60，结果为true
作用：向下舍入取整
语法：INT(number)
Number  需要进行向下舍入取整的实数

6. INT函数

作用：求两数相除的余数
语法： MOD(number,divisor)
Number  被除数
divisor 除数
举例：在单元格内输入“=MOD(15,6)”，显示结果是3
13. MOD函数
ROUND函数
作用：返回某个数字按指定位数取整后的数字
语法： ROUND(number,num_digits)
Number  需要进行四舍五入的数字
Num_digits  指定的位数，按此位数进行四舍五入
若num_digits >0，则四舍五入到指定的小数位 
若num_digits =0，则四舍五入到最接近的整数 
若num_digits <0，则在小数点左侧进行四舍五入 


AVERAGE 求算术平均值函数
语法： AVERAGE(number1,number2,……) 
SUM 求和函数
语法： SUM（Number1,Number2……）
Number1、Number2 需要计算的值，可以是具体的数值、引用的单元格（区域）、逻辑值等
举例：求成绩表中的总分、平均分
3. AVERAGE函数    19. SUM函数
MAX ：求出一组数中的最大值
语法：MAX(number1,number2……)
MIN ：求出一组数中的最小值
语法：MIN(number1,number2……)
Number1、Number2 需要计算的值，可以是具体的数值、引用的单元格（区域）、逻辑值等
10. MAX函数    12. MIN函数
17. RANK函数
作用：返回某一数值在一列数值中的相对于其他数值的排位
语法： RANK（Number,ref,order）
Number 	需要排序的数值
ref 	排序数值所处的单元格区域
order 	排序方式
“0”或者忽略，按降序排名
“1”，按升序排名
举例：按总分进行排位：=RANK(G3,$G$3:$G$12,0)
14. MONTH函数、15. NOW函数
MONTH：求出指定日期或引用单元格中的日期的月份
YEAR：求出指定日期或引用单元格中的日期的年份
语法： MONTH(serial_number)、YEAR(serial_number)
serial_number 指定的日期或引用的单元格
举例：输入公式： =MONTH("2003-12-18") ，显示12
举例：输入公式： =YEAR(“2003-12-18”) ，显示2003
NOW：给出当前系统日期和时间
语法： NOW( )
该函数不需要参数，显示格式可通过单元格格式进行设置
按F9功能键，可进行日期时间更新
20. 单条件求和SUMIF函数
4. 单条件计数COUNTIF函数
多条件求和SUMIFS函数
多条件计数COUNTIFS函数
21. SUMPRODUCT函数

单条件和多条件的汇总_求和与计数
案例
20. 单条件求和SUMIF函数
作用：按给定条件对指定单元格求和
语法： SUMIF(range,criteria,sum_range)
range 要判断条件是否满足的单元格区域
Criteria   确定对哪些单元格相加的条件
Sum_range  要相加的实际单元格
例如，计算每个部门的销售额
=SUMIF($B$2:$B$11,F3,$C$2:$C$11)

第1个和第3个参数注意改为绝对引用，以防止公式向下复制时引用的区域发生变化。
20.单条件求和SUMIF函数
例如：求100万元以上的销售额，下面三种写法都可以
=SUMIF(C2:C11,">100",C2:C11)
=SUMIF(C2:C11,">100")
=SUMIF(C2:C11,E7&F7)
4. 单条件计数COUNTIF函数
作用：计算指定区域中满足给定条件的单元格的个数
语法： COUNTIF(range,criteria)
range 是一个或多个要计数的单元格
Criteria 为确定哪些单元格将被计算在内的条件
例如，计算每个部门的业务笔数
=COUNTIF($B$2:$B$11,F11)

4. 单条件计数COUNTIF函数
例如，计算100万元以上的业务笔数
=COUNTIF($C$2:$C$11,">100")
=COUNTIF($C$2:$C$11,E15&F15)
多条件求和SUMIFS函数
作用：对某一区域内满足多重条件的单元格求和
语法：
SUMIFS(sum_range,criteria_range1,criteria1,criteria_range2,criteria2…)
Sum_range 求和区域（一个或多个单元格）
criteria_range1,criteria1 条件区域1，条件1
criteria_range2,criteria2 条件区域2，条件2
说明：最多写127组条件

注意：SUMIFS 和 SUMIF 的参数顺序不同
sum_range 参数在 SUMIFS 中是第一个参数
sum_range 参数在 SUMIF 中则是第三个参数
请确保按正确顺序放置参数。
多条件求和SUMIFS函数
例如，计算各部门大于100万元的销售额
SUMPRODUCT函数
作用：在给定的几组数组中，将数组间对应的元素相乘，并返回乘积之和
语法：SUMPRODUCT(array1,array2,array3, ...)
Array1, array2, array3, ...  为 2 到 255 个数组，其相应元素需要进行相乘并求和
说明：
数组参数必须具有相同的维数，否则，函数 SUMPRODUCT 将返回错误值 #VALUE!。 


21. SUMPRODUCT函数

此处不加参数 求和区域 ，就变成了对满足连个条件的单元格计数
21.  SUMPRODUCT函数
改为

多条件计数COUNTIFS函数
作用：计算某个区域中满足多重条件的单元格数目
语法：COUNTIFS(range1, criteria1,range2, criteria2…)
range1,criteria1 条件区域1，条件1
range2,criteria2 条件区域2，条件2
说明：
最多写127组条件
仅当区域中的每一单元格满足为其指定的所有相应条件时才对其进行计数 
例如：
多条件计数COUNTIFS函数
作用：根据对指定条件的逻辑判断（真假），返回相应的内容
语法：IF(Logical,Value_if_true,Value_if_false)
Logical  逻辑判断表达式
Value_if_true 当条件为“真”时的显示内容（默认返回 “TRUE” ）
Value_if_false 当条件为“假”时的显示内容（默认返回“FALSE”）
5.  IF函数
作用：左截取子串，即从字符串左侧第一个字符开始，截取指定数目的字符
语法： LEFT(text,num_chars)
text 源字符串
num_chars 截取子串的长度
举例：=LEFT("420106199806014321",6)，结果为"420106"
7. LEFT函数
作用：截取子串，从字符串的指定位置开始，截取指定数目的字符
语法： MID(text,start_num,num_chars)　
text 源字符串
start_num 指定的起始位置
num_chars 要截取的数目
举例：=MID("420106199806014321",7,4)，结果为“1998"

11. MID函数
作用：截取子串，从字符串最后一个字符开始，截取指定数目的字符
语法： RIGHT(text,num_chars)
text 源字符串
num_chars 要截取的数目
举例：=RIGHT("420106199806014321", 4)，结果为“4321"

18. RIGHT函数
作用：统计文本字符串中字符数目
语法： LEN(text)
text 要统计的字符串
举例：=LEN("420106199806014321")，结果为“18"

8. LEN函数
作用：在表格数组的首列查找指定的值，并由此返回表格数组当前行中其他列的值
语法： VLOOKUP(lookup_value,table_array,col_index_num,range_lookup)
lookup_value为需要在Table_array第一列中查找的数值 
table_array 为需要在其中查找数据的数据表 
col_index_num 为table_array中待返回的匹配值的列序号 
range_lookup指明函数VLOOKUP是精确匹配还是近似匹配(逻辑值)
第四个参数为false或0，即为精确查找 

VLOOKUP函数 课件推荐
VLOOKUP的家人们（经典再现）刘晓月ExcelHome2015-04-22
22. VLOOKUP函数
作用：在查找范围（表格数组）中查找指定的值，返回另一个查找范围中对应位置的值
语法： LOOKUP(lookup_value,table_array1, table_array2)

 示例内容见“VLOOKUP的家人们（经典再现）”
9. LOOKUP函数
函数错误 
参数错误 
函数名错误 
标点符号错误 
其他错误： 
#REF!：无效的单元格
#DIV/0!：除零错误
#N/A：“值不可用错误”（某个值对该函数或公式不可用） 
函数纠错工具 
F9功能键 
“公式”选项卡/“公式审核”中的“公式求值”功能 
函数错误及处理方法
单元格编辑
对单元格的操作
单元格、单元格区域的选定
移动和复制单元格、插入单元格、插入行、插入列、删除单元格、删除行、删除列等
对单元格内数据的操作
复制单元格数据，清除单元格内容、格式等
工作表操作
添加、删除、重命名以及移动、复制、拆分和冻结工作表
Excel单元格编辑和工作表操作
设置单元格格式
数字、字体、对齐、标题居中、表格边框、填充色
设置列宽和行高
“开始”选项卡上的“单元格”组中，单击“格式”	
设置条件格式
开始/样式/条件格式 
套用表样式
开始/样式/套用表格格式 

工作表的格式化
数据的管理
1. 数据排序
2. 数据筛选（自动筛选、高级筛选）
3. 分类汇总（先按分类字段排序，再汇总!）
4. 数据透视表和数据透视图
图表是依据选定的工作表单元格区域内的数据按照一定的数据系列而生成的，是工作表数据的图形表示方法
Excel中可方便地绘制不同的图表，如柱形图、条形图、折线图和饼图等
图表具有较好的视觉效果，可方便用户查看数据的差异、图案和预测趋势
用图表表示数据

创建图表
图表的编辑与格式化

图表分为嵌入式图表和工作表图表
图表与创建它们的工作表源数据关联，工作表数据改变，图表随之更新
用图表表示数据
图表元素
图例
数据点
坐标轴 (X)
坐标轴（Y）
数据点：图表中的条形、面积、圆点、扇面或其他符号，代表工作表某个单元格的数值
数据系列：图表中的相关数据点，源自工作表中的一行或一列。饼图只有一个数据系列
图例：显示数据系列名称
分段统计图表截图
图表取色小工具 TakeColor
 Alt+C 取色
“复制”
参考专业图表配色

保护工作簿
保护工作表
为工作簿文件设置权限密码
隐藏工作簿
隐藏工作表
隐藏行或列

Excel使用安全（该部分为课外自学内容）
保护工作簿
在“审阅”选项卡上的“更改”组中，单击“保护工作簿”


Excel使用安全
在“保护工作簿”下，请执行下列操作之一： 
要保护工作簿的结构，请选中“结构”复选框 
要使工作簿窗口在每次打开工作簿时大小和位置都相同，请选中“窗口”复选框 
要防止其他用户删除工作簿保护，请在“密码(可选)”框中，键入密码，单击“确定”，然后重新键入密码以进行确认 

结构和窗口
保护工作表
选择要保护的工作表
要对允许其他用户更改的任何单元格或区域解除锁定
* 选中区域，Ctrl+1,“保护”，取消勾选“锁定”，确定
或要隐藏不希望显示的任何公式
	* 选中区域（如总分），Ctrl+1,“保护”，勾选“隐藏”,确定
在“审阅”选项卡的“更改”组中，单击“保护工作表” 
设置允许项
设置取消工作表保护时使用的密码
Excel使用安全
为工作簿文件设置权限密码
单击“Office 按钮”/“另存为”/单击“工具”/“常规选项”
打开权限密码使用高级加密。修改权限密码不加密
两个权限密码一个用于访问文件，另一个用于为特定审阅者提供修改文件内容的权限。请确保两个密码是不同的 
记住密码很重要。如果忘记了密码，Microsoft 将无法找回
选中“建议只读”复选框。打开文件时，将询问审阅者是否要以只读方式打开文件 


Excel使用安全
隐藏工作簿
单击“视图”/“窗口”/单击“隐藏”
取消隐藏工作簿
单击“视图”/“窗口”/单击“取消隐藏”

Excel使用安全

隐藏工作表
点击相应的工作表标签（表名），右键单击“隐藏”  
取消隐藏工作表
右键单击其他工作表标签，在弹出来的菜单中选择“取消隐藏”
Excel使用安全
隐藏行或列  （比如不想打印出来的行或列）
选中要隐藏的行（列），右键单击“隐藏” 
取消隐藏行或列
选中隐藏行（列）上下相邻的行（左右相邻的列），右键单击“取消隐藏”
Excel使用安全
取消隐藏首行或首列
若隐藏的是首行（列），则不能通过选中上下相邻的行（列）的方式“取消隐藏”，解决方法如下图动画拖拉行号或列标的框线
Excel使用安全

什么是VBA？为什么要学习VBA？
宏和录制宏
开发环境VBE界面介绍
自己编写VBA程序


第一部分  Excel VBA开发平台概述
VBA ：Visual Basic for Application
VBA是 Microsoft Visual Basic 的宏语言版本，内置在宿主软件中（如Microsoft  Office） ，对Office进行二次开发。
特点：不能使用VBA创建独立运行的应用程序， VBA代码只能与宿主应用程序（如Excel）一起使用，需要有宿主应用程序的支持。
什么是VBA？
为什么要学习VBA？
VBA ：“独孤九剑”，特点是“遇强则强，遇弱则弱”
数据量有限，计算要求简单，用VBA是高射炮打蚊子
需处理大量数据，Excel无法高效完成任务，VBA是利器

第1个VBA程序例子 ：点击按钮显示“Hello World!”对话框
(1)应用性学习:"出于应用而学习, 学以致用"
(2)守株性学习:"技多不压身,越多越好,学了再说,等待某天派上大用场"
(3)娱乐性学习:"出于兴趣而学习, 纯粹为了学习而学习,学习VBA或Excel能带来巨大愉悦"
(4)逻辑性学习:"为了锻炼自己的逻辑能力而学习Excel或VBA"
(5)献身性学习:"出于帮助他人而学习"
(6)空白性学习:"出于填补空白时间而学习,实在是闲得没事干了“

	……学分、GPA ……
为什么要学习VBA？-学生篇
在Excel里，简单的宏就是使用宏录制器录下的一组操作
在宏编辑窗口，可以看到宏是一段VBA程序代码，或者一串指令集合，它定义好了一种或一组操作
通过对宏的VBA代码进行编辑修改，宏还可以执行更高级的、普通用户不能完成的任务
宏是一系列固定动作的集合，可比喻
成一段武术套路动作。
宏和录制宏
录制宏（准备工作）
录制宏（准备工作）

录制宏（准备工作）
录制宏（准备工作）
在安全警告对话框，选择启用宏，才能运行宏
若保持宏为禁用状态，则宏不可运行
确定宏来源可靠时，才可启用它
谨慎打开未知来源的包含宏的Excel文件，防范宏病毒
宏病毒是一种寄存在Office文件或模板的宏中的计算机病毒。一旦打开含有宏病毒的文件，宏病毒就会被激活，转移到计算机上，并驻留在Normal模板上。以后，所有自动保存的文档都会“感染”上这种宏病毒；如果其他用户打开了感染病毒的文档，宏病毒又会转移到他的计算机上。

宏病毒
当我们在Excel中运行宏时，如果出现上图所示的对话框，说明在打开包含宏的Excel文件（.xlsm）的时候，没有启用宏，也就是没有点击安全警告消息栏的“选项”中的“启用内容”。
录制宏（准备工作）
解决方法：关闭Excel，重新打开该文件，并点击启用宏
录制宏（准备工作）
方法一：重复复制粘贴
方法二：录制宏（录下Excel操作）
录制宏
录制宏
录制宏
点击A1，选择“录制宏”
点击行号1，选中第一行；右键选择“复制”
点击行号3，选中第3行，右键选择“插入复制的单元格”，再右键（第3行）选择“插入”（插入一行空白行）
光标定位在A4，选择“停止录制”
注意：在宏录制过程中不要有多余的操作步骤
下次宏执行开始的位置
录制宏
两种方式：
点击“开发工具”的“宏”按钮（Alt+F8），打开“宏”对话框。选中相应的宏，点击“编辑”
点击“开发工具”的“Visual Basic”按钮（Alt+F11），启动“Visual Basic编辑器（VBE）”，点击展开“模块”，双击“模块1”
录制宏
两种方式：
Alt+F11，启动VBE，点击展开“模块”，双击“模块1”。光标定位在“制作工资条”过程内，点击“运行子过程/用户窗体”按钮（F5）
Alt+F8，打开“宏”对话框。选中相应的宏，点击“执行”
执行宏，观察工资条的生成（执行一次宏生成一个工资条）
注意：执行宏进行的操作不能用Excel的撤销命令恢复
录制宏
点击Excel的“保存”按钮。弹出下面所示的对话框，一定要点击“否”（如果点击“是”，刚才的宏就白录了）


“另存为”对话框，选择保存类型“Excel启用宏的工作簿(.xlsm)”



录制宏
点击“开发工具”/ 控件 / 插入 / 表单控件 /“按钮”
在工作表空白处新建一个按钮，编辑文字为“制作工资条”，选中按钮（点击边框），右键选择“指定宏”
在“指定宏”对话框，选择刚才录制的宏“制作工资条”
点击A1（先定位），单击“制作工资条”按钮 ，查看效果
录制宏
执行效率仍然较低 （执行一次宏生成一个工资条）
用VBA改写刚才录制的宏代码
添加for循环语句，重复生成工资条
执行一次宏，生成所有的工资条
录制宏
分别添加红框内的代码段，其中10是工资条的总数。
Next i  可以省略i
返回工作表界面，选中A1，点击一次按钮，完成所有的工资条。

总结
宏是用VBA代码保存下来的程序
录制的宏是VBA里最简单的程序，存在很多缺陷：如无法进行判断和循环等，效率不高，执行不灵活
为了更自动高效地完成任务，所以需要学习VBA，掌握VBA编程方法，自主编写VBA程序
VBE（Visual Basic Editor）
编写 VBA程序的地方
进入VBE的几种方法：
“开发工具”选项卡/“代码”组，单击“Visual Basic”
快捷键 Alt+F11 
“开发工具”选项卡/“代码”组，单击“宏”，打开“宏”对话框，选中一个宏，单击“编辑”按钮
右击工作表标签 ，快捷菜单“查看代码”命令
VBE界面介绍
工程资源管理器（Ctrl+R）
代码窗口
立即窗口（“视图”/“立即窗口”）
“工具”/“选项”/“编辑器”/“要求变量声明”
VBA的编程环境： VBE

VBE界面介绍
工程资源管理器
（Ctrl+R）
代码窗口(F7)

“工具”/“选项”/“编辑器格式”
设置代码窗口的字体大小
VBE界面介绍


插入模块
选择“插入”菜单下的“模块”命令
模块名称修改
单击选中模块，如“模块1”
点击“属性”窗口，修改名称即可
快捷键F4可弹出该模块的属性窗口
或选择“视图”菜单下的“属性窗口”命令
移除模块
右击模块，快捷菜单中选择“移除模块”
VBE中插入模块、修改模块名称和移除模块



自己编写VBA程序
代码以“Sub  过程名()”开头，以“End Sub”结束
中间英文单引号开始的绿色文字（注释）是对宏的说明
其余代码是要完成的操作
语句以回车作为结束
自己编写VBA程序
MsgBox：弹出消息对话框
编程语言的三种基本结构：顺序、选择（分支）和循环
一个模块中可以写多个宏，光标定位在哪个宏的代码内，执行哪个宏
执行second宏，发现语句按自顶向下的顺序执行
vbNewLine或vbCrLf开始新行
MsgBox “Hello“ & vbNewLine & “VBA"
自己编写VBA程序
变量定义语句：Dim  变量名  [As  变量类型]
Dim  i  as  integer
i = 2016
MsgBox  i
课堂练习：试着编写一个宏程序，定义两个变量，一个赋值“你的学号”，一个赋值“你的姓名”，然后将他们分两行显示在同一个对话框中。
Dim kb 
kb=5
kb=“饿了吗？”
MsgBox  kb
未定义类型，则
变量kb是变体型
（Variant）
The End
Let’s do it better!

# 第二部分 VB基础知识
## 2.5 VBA流程控制语句
## 2. 6 VBA过程
## 2.7 常用内部函数
## 2.1 VBA 的字符集、关键字和标识符
- VBA字符集：大小写英文字母：`A~Z、a~z`，**不区分大小写**；数字：0~9；特殊符号：	`+  -  *  /  \  ^  =  <>  <  >  <=  >=  &`等运算符及一些标点符号；汉字。
- VBA关键字：关键字是VBA语言中约定的由系统使用的一些字符串，用户定义变量时不能使用关键字，以免系统误解。
**VBA关键字很多**，分为`框架类关键字、控件类关键字、声明类关键字、数据类型关键字、运算符关键字、程序结构类关键字`等几大类。  
- VBA标识符：标识符是用户定义的变量、常量、过程、函数等的名称。利用它可以完成对变量、常量、过程、函数等的引用。
**标识符的命名规则**：`首字符必须为字母或汉字`，由字母(汉字)、数字和下划线组成；不能出现`空格  .  !  @  &  $  #`等字符；不能与关键字同名；不能超过255个字符。
- 语法规则和书写规范：必须遵守否则报错(`强制语法检查的VBE设置`，默认已选）
一条语句可以多行书写，行末以空格+下划线来续行；一行可以书写多条语句，各语句之间以冒号`: `分隔。
过程名、变量名定义要有意义，尽量“见名知义”；单词之间以空格分隔；尽量使用缩进格式，以增加程序的可读性；养成使用注释的习惯：Rem语句及其简写形式；块注释与块取消注释工具按钮。
## 2.2 VBA的数据类型 
为了处理各种不同的数据，VBA定义了不同的数据类型及其存储方式。VBA的数据类型主要包括数值型、字符型、布尔型、日期型、货币型、对象型和变体型等。
| 数据类型 | 关键字 | 占用字节数 | 简写 |
|:-|:-|:-|:-|
| 布尔型 | Boolean | 2
| 字符型 | Byte | 1
| 整型 | Integer  | 2 |  %
| 长整型 | Long | 4 | &
| 单精度浮点数 | Single| 4 | !
| 双精度浮点数 | Double| 8 | #
| 货币型 | Currency| 8 | @|
| 小数型 | Decimal | 12
| 日期型 | Date | 8
| 字符串型| String |  |$ 
| 对象型 | Object| 4
| 变体型 | Variant |
## 2.3 常量和变量
- VBA中的常量：是指执行程序时值不会发生变化的数据。VBA中的常量分为直接常量和符号常量。
直接常量：数值常量：20、2.65；字符串常量：“remember me” ；逻辑常量：True、False；日期常量：#10/16/2016#；
符号常量
1.系统内部的符号常量：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190706002647783.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70 =500x400)
2.用户自定义常量：`Const   常量  As  数据类型 = 表达式`。用Const语句声明常量、设定常量的值。常量声明后，不可以对它进行再赋值。
 
- VBA中的变量：是指执行程序时值可以发生变化的量。强制显式声明(`Option Explicit`)模块中，变量要先声明（定义），后使用。
显式声明变量（Dim、Private、Public、Static）：`Dim  变量名  [as  数据类型]`，不写后面的 as  数据类型 , 则为变体型变量。
隐式声明变量：VBA 支持隐式声明变量，即可以不定义直接使用变量。未定义的变量全部是变体型。不利于编程纠错，不推荐这种方式。
- 赋值语句： 变量名= 值。赋值号“=”在这里不代表相等意思，是将“=”右边的值赋到左边的变量中。- 
- 变量的作用域：**决定可以在哪个模块或过程中使用该变量**，按作用域将变量分为本地变量、模块级变量和公共变量。

	|变量作用域 | 解释 |
	|:-|:-|
	|本地变量| 在一个过程中用Dim或Static语句声明的变量, 作用域是本过程|
	|模块级变量| 在**模块的第1个过程之前**使用Dim或Private语句声明的变量，该模块中所有过程都可以使用|
	|公共变量| 在**模块的第1个过程之前**使用**Public语句声明**的变量，所有模块中的过程都可以使用|
- VBA中的数组: 数组是具有相同数据类型的一组变量的集合，`数组名(下标)`各元素通过下标加以区分。
1.**定义固定大小的静态数组**
`Dim 数组名 (下标上界) As 数据类型`，此时默认最小下标为0，元素个数=下标上界+1。
`Dim 数组名 (第1维下标上界, 第2维下标上界,…) As 数据类型`
2.**定义动态数组**：动态数组是指**在程序运行时大小可以改变**的数组。
`Dim 数组名( ) As 数据类型
ReDim [Preserve]数组名 (下标上界)`
Dim语句中**不可以用变量定义数组的大小**，若不能确定数组的元素个数，则先定义为动态数组，再在程序中用ReDim语句**重新指定数组的大小**。(注意：ReDim语句中可以使用变量，但不可以改变原数组的类型；如果数组已经存在元素，ReDim则可加上Preserve关键字进行保护)。
3.其他几种创建数组 的方法
 使用VBA的Array函数创建数组
Sub dtsz()
    Dim arr As Variant
    arr = Array(1, 2, 3, 4, 5)
    Range("A1:E1").Value = arr
	 Range("A3:A7").Value = _       Application.WorksheetFunction.Transpose(arr)
End Sub
'运行后查看A1:E1、A3:A7单元格区域中的数据
'使用Array函数创建的数组索引号默认从0开始，除非模块最前面有“OPTION BASE 1”
其他几种创建数组 的方法
 通过单元格区域直接创建数组
Sub Rng_arr()
    Dim arr As Variant
    arr = Range("A1:B3").Value
    Range("A5:B7").Value = arr
    MsgBox arr(2, 2)
End Sub
'数组索引号从1开始
其他几种创建数组 的方法
3.使用VBA的Split函数创建数组
Sub Split_arr()
    Dim arr As Variant
    arr = Split("二丫,三傻,色后,小恶魔,雪诺,龙母", ",")
    Range("A15:F15").Value = arr
    MsgBox arr(2)
End Sub
VBA运算符与表达式
表达式的概念：由运算符连接起来的式子 

算术运算符与表达式
比较运算符与表达式
逻辑运算符与表达式
字符串连接运算符与表达式




VBA运算符与表达式
算术运算符


注意：
VBA语言中没有×号和÷号，使用* 代表乘，/代表除
\取商运算（整除） 小数部分直接丢掉，不四舍五入
以下VBA程序进行算术运算：
运算符练习程序
Sub suanshu()
    Dim x As Double
    x = 6 / 4  : Debug.Print x
    x = 6 * 3 : Debug.Print x
    x = 2 ^ 4 : Debug.Print x
    x = 6 Mod 3 : Debug.Print x
    x = 19 \ 2 : Debug.Print x
End Sub
1.5 
18 
16 
0 
9 
混合运算:指有不同的数据类型（如整型和实型）的数据共同参与的运算
混合运算的隐含数据转换规则：
从“小”数据类型自动转换成“大”的数据类型进行运算
数值范围小到数值范围大（ Integer – Long ）
精度低到精度高( Single – Double )
整数到实数（Long - Double）
如果将double 型的运算结果装入long类型变量中，会产生精度损失（应该尽量规避这种情况的发生）
溢出错误的产生原因分析，及避免该情况的发生
例子在后面的幻灯片中
混合运算的规则
例子：
Sub 买文具( )
Dim 钢笔 As Long, 书包 As Double
钢笔 = 10
书包 = 20.5
MsgBox 钢笔 + 书包
End Sub

混合运算
例子：把一个实型变量的值赋给一个整型变量
Sub 数值型变量混合运算( )
Dim number As Integer
Dim number1 As Double
number = 3
number1 = 2.5
number = number1 
'实数赋给整数，有精度损失，四舍六入五单双
MsgBox number
End Sub
混合运算
溢出错误的例子：（运行下面的程序，出错！如图示）
Sub 溢出( )
Dim num As Integer, num2 As Long
num = 5000
num2 = 200000
num = num2
MsgBox num
End Sub
怎么改？
混合运算中的溢出错误
Sub 混合运算()
Dim num As Long, num1 As Integer
num1 = 2500
num = 20000
MsgBox num * num1
End Sub
为什么不溢出？
VBA运算符与表达式
字符串连接运算符



例子：
Sub 字符串变量( )
Dim str As String '字符串类型，最多可以装2亿个字符
str = "武汉" '字符串常量必须用英文半角的双引号引起来
MsgBox str & " ，每天不一样。"
End Sub

例子：在字符串变量中追加文本内容
Sub 字符串变量2( )
Dim str1 As String
str1 = "武汉"
str1 = str1 & " ，每天不一样。" '在字符串变量中追加文本
MsgBox str1
End Sub
字符串连接运算符 &
'运算符&会强制把它后面的数据类型转换为字符串进行连接
' 
Sub 字符串变量3( )
Dim Name As String, age As Long
Name = "王小二"
age = 12
Name = Name & age & "岁了。"
MsgBox Name
End Sub
字符串连接运算符 &
复习：如果想让姓名和年龄各一行显示，该怎么改？
VBA运算符与表达式
布尔型变量与比较运算符（优先级相同）
布尔型变量（boolean）：只有两个值：真(True) 假(False)
布尔型变量经常作为关系（比较）表达式或者逻辑表达式的结果而存在
例子：
Sub 布尔型变量( )
    Dim b1 As Boolean '布尔型
    Dim x As Long
    x = 10
    b1 = True
    b1 = (x = x + 1) '最左边的等号是赋值运算符，右边是比较运算符
    MsgBox b1
End Sub
关系（比较）运算
VBA运算符与表达式
逻辑运算符


And 与运算（类比乘法）
    条件1 and 条件2
     1 * 0 = 0
     0 * 1 = 0
     0 * 0 = 0
     1 * 1 = 1
进行And逻辑运算时，只要有一个条件不成立，结果就不成立
Or 或运算
进行Or 运算时，只要有一个条件成立，结果就成立
Not 非运算（取反 ）
如果条件本来成立，Not 之后就不成立了，反之亦然
逻辑运算符
例子：
Sub booleanExc( )
    Dim b1 As Boolean, b2 As Boolean, b3 As Boolean
    '定义为布尔类型变量，但是没有赋值。VBA系统默认该变量值为False
    b1 = True
    b3 = b1 And b2 'b3为False
    b2 = b1 Or b3	 'b2为True
    b2 = b1 Or b2 Or b3	 'b2继续赋值为True
    MsgBox Not b2		 '输出False
End Sub
逻辑运算
各类运算的优先级
算术运算符>&>比较运算符>逻辑运算符>赋值运算符
圆括号
圆括号内的运算优先
不确定运算符的优先级时，可考虑借助于圆括号
另外，使用圆括号可以帮助理解程序的结构
各类运算的优先级与圆括号
Sub InputboxExc( )
    Dim x
    x =  Inputbox("请输入一个正整数")
    MsgBox  x + 10
End Sub
数据输入：Inputbox的简单使用
例子：用一个表达式判断输入的年份是否是闰年
定义一个变量 year 
(year mod 400 =0) or ((year mod 100<>0) and (year mod 4=0))  
注意：or后的圆括号把判定条件2作为一个整体，不加结果也正确，但不易读懂
闰年的判断条件 （满足其中一个条件就是闰年）
该年份可以被400 整除 
该年份可以被4 整除 并且不可以被100 整除

课堂练习1
在输入程序代码时，使用Ctrl + j 快捷键 ，具有联想功能
使用该功能前需确认“工具”/“选项”里设定自动列出成员选项
Sub 神键( )
        MsgBox  '按下Ctrl + J ，输入ms，自动联想msgbox，敲tab键确认
End Sub


Ctrl + j 快捷键
按F8功能键，将逐语句执行程序
逐句执行VBA程序时，将光标放在变量上，可查看变量当前的值
也可以F8和F5和断点设置配合使用，进行程序调试
F8键（逐句调试）
2.姚明身高226cm, 已知厘米和英寸的换算规则是：英寸=厘米/2.54 ，编程计算姚明的身高是多少英寸。
要求定义一个字符串变量表示姓名
定义一个什么变量表示身高？
定义一个什么变量表示结果英寸数？
用msgbox 语句和连接字符串的方式显示姚明的身高是多少英寸（保留两位小数）。
3. 计算自己年龄的秒数
一年大约是3.156 × 10 7 秒
你今年多少岁（age）？
请使用msgbox 语句输出你年龄是多少秒。

课堂练习
使用FormatNumber函数
格式：
	FormatNumber([数值], [小数位数], [显示模式])
第3参数=0或 vbFalse时，不显示【前导0】
第3参数=-1或 vbTrue时，显示【前导0】
举例：
MsgBox "身高是" & FormatNumber(inch,2,-1) & "英寸"
控制实数显示的小数位数（1）
使用Round函数
格式：
	Round(数值表达式 [,小数位数])
小数位数表示进行四舍五入运算时，小数点右边应保留的位数。如果忽略，则Round函数返回整数。
举例： 
MsgBox "身高是" & Round(inch,2) & "英寸"
控制实数显示的小数位数（2）
使用Format函数保留两位小数
格式：
	Format (数值表达式, "0.00")
	Format (数值表达式, "#.##") 
0：占位格式化，不足位时补足 0 
#：占位格式化，不足位时不补 0 ，不显示前导0 
举例： 
MsgBox "身高是" & Format(inch, "0.00") & "英寸"
MsgBox "身高是" & Format(inch, "#.##") & "英寸"
Format(0.2, "#.##") 显示  .2
控制实数显示的小数位数（3）
使用Format函数
用法：
	Format([数值], "Scientific")
举例：
Format(6283479, "Scientific")  显示为 6.28E+06

用科学计数法的形式显示数值
The End
Haste makes waste.
---
### 2

VBA 的字符集、关键字和标识符
VBA的数据类型 
常量和变量
运算符与表达式
VBA流程控制语句
VBA过程
常用内部函数
第二部分  VBA基础知识(2)
1
VBA流程控制
顺序结构	‘语句自顶向下执行
选择结构
If...Then...Else语句
Select Case 语句
循环结构

插入模块
选择“插入”菜单下的“模块”命令
模块名称修改
单击选中模块，如“模块1”
点击“属性”窗口，修改名称即可
快捷键F4可弹出该模块的属性窗口
或选择“视图”菜单下的“属性窗口”命令
移除模块
右击模块，快捷菜单中选择“移除模块”
复习：VBE中插入模块、修改模块名称和移除模块



例子：用一个表达式判断输入的年份是否是闰年
定义一个变量  year 
(year mod 100<>0  and  year mod 4=0) or (year mod 400 =0) 
Sub 闰年否()
    Dim year As Integer
    year = Inputbox("请输入一个年份")
    MsgBox  (year Mod 4 = 0 And year Mod 100 <> 0) Or (year Mod 400 = 0)
End Sub
‘当输入2016，显示true ;  当输入2018，显示false
‘如何输出中文显示 “是闰年”或“是平年”？

引例：复习上次课的课堂练习
IIF函数是VBA 的一个内置函数 (Build In Function，BIF)
格式： IIF (条件,条件成立返回,不成立返回)
IIF函数通常用于比较简单的条件判断
Sub 闰年_IIF()
    Dim year As Integer, 闰年否 As Boolean
    year = Val(InputBox("请输入一个年份") )
    闰年否= (year Mod 4 = 0 And year Mod 100 <> 0) Or year Mod 400 = 0
    MsgBox  year & IIf(闰年否, "是闰年", "是平年")
End Sub
解决方案一：使用IIF函数
也是一个VBA 内置函数 (Build In Function，BIF)
语法格式： Val (String)
功能：将数字字符组成的字符型数据转换成相应的数值型数据
例子：
Val(InputBox(“请输入一个年份："))
由InputBox函数返回的数据类型是字符类型
如果想得到数值类型数据，可以用Val()函数进行类型转换
在不能识别为数字的第一个字符处停止读入字符串。空格、制表符和换行符从参数中自动去掉。
如果输入是“2016”，则返回数值2016
如果输入是“春天”，则返回数值0
Val函数
选择结构：根据不同的条件，选择执行不同的语句
本次课，我们学习使用选择结构的If...Then...Else语句和Select Case语句，根据条件（表达式）的值有选择的执行语句，内容包括：
单分支if语句
双分支if语句
多分支if语句
if语句的嵌套使用
Select Case 语句

解决方案二：使用选择结构
单行形式的语法：
If condition Then [statements][Else elsestatements]
或者，可以使用块形式的语法：
If condition Then
[statements]
[ElseIf condition-n Then
[elseifstatements]] ...
[Else
[elsestatements]]
End If
判断：如果在 Then 同一行之后，还有其它非注释的内容，则此语句就是单行形式的 If 语句
选择结构的If...Then...Else 语句
单分支if语句（一条路可走）
单行形式
 If  条件表达式  Then  处理语句    '如果  条件成立  那么就  ...
块形式
'如果处理语句有多条，写成块形式，最后加一行end if
If  条件表达式  Then 
	处理语句组
End If
注意：
单行形式没有end if
块形式最后要加end if
单分支if语句
例：输入一个正整数，判断是否是偶数，若是就输出“是偶数”
Sub even( )
    Dim number As Long
    number = Val(InputBox("请输入一个正整数："))
    If number Mod 2 = 0 Then MsgBox number & " is an even number."
End Sub
该程序采用if单行形式，还可以有两种等价写法：
在Then后面敲空格加下划线（VBA 语句的续行符）后回车，在下一行写MsgBox语句。
用格式2的if块形式，最后加End If。

Sub even2( ) '单行形式续行
    Dim number As Long
    number = InputBox("请输入一个正整数：")
    If number Mod 2 = 0 Then _ 
	MsgBox number & " is an even number." '空格加下划线是续行符
End Sub
Sub even3( )   '块形式，最后加End If
    Dim number As Long
    number = InputBox("请输入一个正整数：")
    If number Mod 2 = 0 Then
             MsgBox number & " is an even number."
    End If
End Sub

两种等价写法
不敲续行符（空格+下划线）
直接回车报错
例题：输入鸡蛋的价格，如果价格过低就给出提示
Sub 鸡蛋价格()
    Dim 价格 As Double
    价格 = InputBox("请输入鸡蛋价格：", "单价录入")
    If 价格 < 2.1 Then
        MsgBox "鸡蛋单价过低，此价格无法录入系统！"
    End If
End Sub
单分支if语句
单分支if语句 课堂练习
练习1：定义一个成绩变量，输入成绩，若成绩大于等于90，输出“GPA=4.0”。
练习2：若今天是星期六，并且下雨，则输出今天不上课。
练习2试试使用续行符和if语句的块形式


双分支if语句（两条路可选，增加else子句）
单行形式：If  条件表达式  Then  语句1  Else 语句2 
		'如果  条件成立      那么就  ...  	      否则   ...
块形式
If   条件表达式   Then
   语句组1
Else 
	   语句组2
End If
 
块形式中 then 和else 后面的语句要缩进,以利于读程
初学者可先输入if语句的基本结构，以防漏掉then, else和end if, 再填入表达式和语句
双分支if语句（两条路可选）
例题：输入一个正整数，判断是否是偶数，如果是就输出“是偶数”，不是就输出“是奇数” 
Sub evenodd()
    Dim number As Long
    number = Val(InputBox("请输入一个正整数：") )
    If  number Mod 2 = 0 Then
        MsgBox number & " is an even number." 
    Else
        MsgBox number & " is an odd number."
    End If
End Sub
若else下一行语句直接写在else同一行，系统会自动在else后面添加一个冒号“：”，表示一行有多条语句
then后面的子句和else后面的子句可以不止一条
双分支if语句的单行形式
Sub 双分支if的单行形式()
    Dim a As Integer, b As Integer
    a = 10 :  b = 20
    If a > b Then Else MsgBox "a小于b"
    	Rem 省略了Then语句
    If a < b Then MsgBox "a小于b"
    	Rem 省略了ELSE子句，变成单分支if结构
    If a = b Then MsgBox "a等于b" Else MsgBox "a不等于b"
    	Rem Then 语句，Else子句都具备
End Sub
可以观察到if语句的单行形式没有end if
双分支if语句（两条路可选，只能走其中一条）
来自EH胡老师的例题
例题：判断是否是武功高手
Sub 判断高手否()
    If  Sheet2.Range("D2") >= 90 Then
         Sheet2.Range("E2").Value = "武功高手"
    Else
         Sheet2.Range("E2").Value = "武功平平"
    End If
End Sub
双分支if语句
双分支if语句 课堂练习
练习3：输入一个整数，判断该数是否大于等于0，如果成立，输出该数，否则输出该数的绝对值
练习4：输入两个数a和b，如果a大于等于b，就输出 a，否则就输出 b


*练习5：计算

*表示选做
多分支if语句（多条路可选，只能走其中一条）
格式：
If   条件表达式1  Then
	语句组1
ElseIf  条件表达式2  Then
		语句组2
……
Else              ‘以上皆否
		语句组n
End If
第1点：有多条分支，最终只能选择（匹配）一条，由顶至下，顺序进行条件匹配
第2点：语句中可以有多个elseif ，但是都必须放在 Else 子句之前
多分支if语句
例题：输入一个字符，判断它是字母、数字，还是其他字符
Sub character( )
    Dim c As String
    c = InputBox("请输入一个字符")
    If (c >= "A" And c <= "Z") Or (c >= "a" And c <= "z") Then
        MsgBox "character is letter."
    ElseIf c >= "0" And c <= "9" Then
        MsgBox "character is digit."
    Else
        MsgBox "other character."
    End If
End Sub
多分支if语句课堂练习
猜数游戏: 输入一个1~10 的整数，判断与给定的整数是否相等
若相等就输出猜对了
若大于给定的数就输出猜大了
若小于给定的数就输出猜小了
if语句的嵌套使用（在if语句中又包含有一个或多个if语句）
比较a,b,c的值，找出最大数赋给变量max，并输出最大数max
Sub max( )
    Dim a As Long, b As Long, c As Long, max As Long
    a = 5   :   b = 8   :   c = 3
    If a >= b Then
        If a >= c Then max = a  else max = c   	‘嵌套1，双分支 
    Else
        If b >= c Then max = b                     	‘嵌套2，单分支
        If b < c Then max = c			‘嵌套3，单分支
    End If
    MsgBox "最大值是" & max
End Sub
外层if语句（蓝色）嵌套了3条if语句（红色）
例子：使用输入框向工作表添加日期，输入时对输入的数据进行判断。（先选中B2单元格，再运行sub过程）
Sub 入库时间()
    Dim d As Variant
    d = InputBox("请输入日期：", "入库时间")
    If d <> "" Then
          If IsDate(d) Then
               Selection.Value = d
          Else
               MsgBox "输入格式不对，非日期！"
          End If
    Else
          MsgBox "输入为空！"
    End If
End Sub

if语句的嵌套示例
输入框提示文字
输入框标题
总结一下我们学过的几种if 语句  
If  条件 Then  单分支if语句
If 条件 Then … Else … 双分支if语句 
If 条件1 Then
     … 
ElseIf 条件2 Then 
	  … 
Else 
	  … 
End If  					    多分支if语句
If 语句的嵌套
if语句的两种写法：单行形式（无end if）和块形式
 if语句小结
练习1：输入一个年份，判断是否闰年。输出 xxxx 年是闰年, 或者输出xxxx 年是平年。
 要求使用 双分支if结构来进行判断
练习2: 学生成绩分组
 成绩>=90  优秀生
 成绩>= 80  良好生
 成绩 >=60  合格生
 成绩 <60   不及格
要求使用 多分支if结构来进行判断
以上练习自觉完成，热身训练，手写、电子版均可，不需要提交，不记入成绩。
if语句练习：
VBA流程控制
顺序结构	
选择结构:
If...Then...Else语句
Select Case 语句
根据单一表达式取值不同来执行多种可能的动作时，Select Case更为有用
语法：Select  Case 测试表达式
		[Case 表达式列表1          ‘可以有多个Case子句
	   		 [语句1] ] ...
		[Case Else
	    		[else语句 ] ]
		      End Select 		
说明：
测试表达式：必要参数，任何数值表达式或字符串表达式
如果与多个Case 表达式列表的值匹配，只执行第一个
表达式列表：如果有Case出现，则为必要参数
形式为表达式，表达式 To 表达式（较小的数值要出现在 To 之前），Is 比较运算符 表达式  的一个或多个组成(逗号隔开)的分界列表
选择结构的Select   Case 语句
Select Case 例题1
    Dim Number As Long
    Number = 8    ' 设置变量初值
    Select Case Number    '判断 Number 的值
    		Case 1 To 5    'Number 的值在 1 到 5 之间，包含1 和 5
        		Debug.Print "Between 1 and 5"
   			   '下一个 Case 子句是本示例中唯一匹配的子句
    		Case 6, 7, 8    ' Number 的值在 6 到 8 之间
        		Debug.Print "Between 6 and 8"
    		Case 9 To 10    ' Number 的值为 9 或 10
        		Debug.Print "Greater than 8"
    		Case Else     '其他数值
        		Debug.Print "Not between 1 and 10"
    End Select
    

Debug.Print 是将代码执行结果显示在“立即窗口”中
Select Case 语句例题2
    Dim 成绩 As Long
    成绩 = 95
 Select Case 成绩
    	Case 90 to 100
         MsgBox "优秀生"
    	Case Is >= 80
          MsgBox "良好生"
    	Case Is >= 60
         MsgBox "合格生"
    	Case Else
         MsgBox "不及格"
	End Select
    

注意：
在 Select Case 语句中出现的Is关键字不是比较运算符Is
使用Is关键字时，可以配合比较运算符（除Is和Like之外的）来指定一个数值范围
Is 关键字会被自动插入，所以可以直接在Case后输入“>=80”。 
附：比较运算符Is的作用则是比较两个对象的引用变量是否相同，返回True或者False。

想一想 Case Is >= 80 与 Case 成绩>= 80表达的意思相同吗？为什么不同？
Is >=80表示一个数值范围；后者判断是否是True 或 False
右边的代码本意输出“良好生”，
结果输出“合格生”
错误分析
表达式列表
表达式列表的三种形式：表达式，表达式 To 表达式，Is 比较运算符 表达式 ，可以同时出现在一个Case中，多个表达式之间用逗号分隔。


    Dim 成绩 As Long
    成绩 = 95
 Select Case 成绩
    	Case 90,91,92 to 100
         MsgBox "优秀生"
    	Case 80, 81, 82 to 84, Is >=85
          MsgBox "良好生"
    	Case Is >= 60
         MsgBox "合格生"
    	Case Else
         MsgBox "不及格"
	End Select
    

Select Case 语句课堂练习
练习1：定义一个天气变量，根据天气不同，输出不同句子。
“晴”： "天气晴朗，心情愉快。"
“雨”, “阴”："阴雨绵绵，郁闷至极。"
“雪”：“北国风光，银装素裹。"
“雾霭”： "穹顶之下，自强不吸。 "


Sub msgbox返回值演示( )
    Dim Isvip‘Isvip不能定义为boolean型，它接收msgbox的返回值（数值）
    Isvip = MsgBox("你是VIP客户吗?", vbYesNo, "用户调查")
    If  Isvip = vbYes Then
        MsgBox "VIP客户一律8折。"
    Else
        MsgBox "对不起，只有VIP客户才能优惠。"
    End If
 End Sub
msgbox返回值演示
编写电影分级制度演示程序
提示输入年龄，用InputBox
如果小于 13 岁，提示可以 看 G 级电影
如果大于等于13岁但小于16 岁，提问是否有大人陪同：
如果自己来的，可以看PG-13 级
如果有大人陪着可以看PG 级
如果大于等于16岁 但小于18岁，可以看NC-17级的电影
大于等于18岁可以看R 级电影
提示：
 可采用Select Case 语句与If 语句的嵌套
“是否有大人陪同”使用上一张幻灯片中演示的msgbox返回值进行判断
Select Case 语句课后练习
The End
Haste makes waste.

---
### 三
2
VBA 的字符集、关键字和标识符
VBA的数据类型 
常量和变量
运算符与表达式
VBA流程控制语句
VBA过程
常用内部函数
第二部分  VBA基础知识(3)
3
选择结构：根据不同的条件，选择执行不同的语句
If...Then...Else语句
单分支if语句
双分支if语句
多分支if语句
if语句的嵌套使用

复习上次课的内容
if语句的两种写法：单行形式（无end if）和块形式
单行形式
	If condition Then [statements][Else elsestatements]
块形式
If condition Then
	[statements]
[ElseIf condition-n Then
	[elseifstatements]] ...
[Else
	[elsestatements]]
End If
复习上次课的内容
Select Case 语句:根据单一表达式取值不同选择执行不同的操作
 Select Case 测试表达式
        Case 表达式列表1
            语句1
     	  …….
        Case 表达式列表n
               语句n
        Case Else
               语句
 End Select
复习上次课的内容
VBA流程控制
选择结构
If...Then...Else语句、Select Case 语句
循环结构
For...Next 语句、Do...Loop 语句
其他循环语句
For Each… in … Next
While … Wend
其他控制语句
With语句、Exit 语句、Goto 语句
Stop语句、End 语句

提出问题：写程序时，经常会遇到这种情况：需要反复执行一条或多条语句。执行一次，写一次，效率太慢！
解决问题：采用循环结构
介绍两种循环语句
For…Next 语句
Do...Loop 语句
说明：
已知循环次数，建议采用For…Next语句
未知循环次数，可采用Do...Loop语句

VBA的循环结构
作用：以指定次数来重复执行一组语句
语法：
              For 计次变量 = 起始值 To 结束值 [Step 步长]
                      [循环体语句 ] 
                     [Exit For]
                      [循环体语句 ] 
          Next [计次变量]
说明：  
计次变量：必要参数，用来做循环计数器的数值变量
起始值、结束值：必要参数，数值或表达式
Step 步长：可选参数，指定变量每次增加的值，缺省时为1 
Next 循环的最后一条语句，与For一一对应，后面可不写变量

For…Next语句
步长可以是正数，也可以是负数。但不能是0（死循环）
步长为正数（起始值小于等于结束值）
正确: For i = 1 To 5  [ Step 1 ]
错误: For i = 5 To 1
循环变量递减（起始值大于等于结束值，步长为负数）
For i = 5 To 1 Step -1
For…Next语句
Next 干了两件事  
第1步：对计次变量累加一个步长值 
第2步：判断计次变量是否大于终值（大于就退出循环语句）
Sub 计次循环遍历()
    	Dim i As Long 	'在循环中，i 通常被定义为计次变量
    	For i = 1 To 3   	'这一行只执行一次
 		      Debug.Print  i    
    	Next  i
	Debug.Print "退出循环语句后i值为：" & i
End Sub

For…Next语句示例（用F8功能键逐句运行）
例：编程求1+2+3+....+100的和（高斯累加）
Sub 高斯累加( )
    Dim i As Long, sum As Long
    sum = 0            ‘这句可不写，sum定义之后默认值为0
    For i = 1 To 100
        sum = sum + i   ‘把sum的值加上i，再放到sum中去
    Next
    Debug.Print "1+2+3+....+100=" & sum
End Sub
For…Next语句示例
例：显示当前工作簿的所有工作表
Sub ListWorksheets()
    Dim i As Integer
    Dim strMsg As String
    strMsg = "当前工作簿 " & ActiveWorkbook.Name & _
	“ 包含以下工作表:” & vbCrLf
    For  i = 1 To Worksheets.Count
        strMsg = strMsg & Worksheets(i).Name & vbCrLf
            'worksheets(i)当前活动工作簿中，第i个工作表
            '.name属性,工作表表名
    Next
    MsgBox prompt:=strMsg, Buttons:=vbInformation + vbOKOnly,  _
    Title:="提示"
End Sub
For…Next语句示例
编程求Fibonacci数列的第12项（144）
Fibonacci数列：1,1,2,3,5,8,13,21,34,55,89,144，…
又称为兔子繁殖问题：有1对新生兔子，从第3个月起它们每个月都生一对兔子，假设没有兔子死亡，按此规律，一年后共有多少对兔子？

For…Next语句示例
Sub Fibonacci数列()
    Dim i As long, fib As long, fib1 As long, fib2 As long
    fib1 = 1  :   fib2 = 1
    For i = 3 To 12
        fib = fib1 + fib2
        fib1 = fib2	‘这一行和下一行代码可以互换吗？Why?
        fib2 = fib
    Next i
    Debug.Print fib
End Sub
For…Next语句示例
用数组实现求Fibonacci数列的第12项
Sub fibonacci数列2( )
    Dim fib(1 To 12) As Integer, i As Integer
    fib(1) = 1: fib(2) = 1
    For i = 3 To 12
        fib(i) = fib(i - 1) + fib(i - 2)
    Next
        Debug.Print fib(12)
End Sub
For…Next语句示例
练习1：编程计算1到100中所有8的倍数的和  (624)
提示：判断一个数i是8的倍数可以用  i Mod 8 = 0
练习2：编程计算1+2-3+4-5+6-… +100 的结果  (52)
提示：注意这道题中，sum变量的初始值
For…Next语句课堂练习
读懂下面代码，回答问题
循环语句一共执行了几次？最后i的值是多少？
 		 Dim i As Integer
        For i = 1 To 10
                  i = i + 2
        Next
	 Debug.Print i
总结：在For循环中改变计次变量的值，会使程序代码的阅读和调试变得困难


读程序
作用 ：提前退出循环语句
可以在循环中进行判断，如果需要提前结束循环则可以使用此语句   注意：Exit和for 之间有空格，不要连在一起
举例：
Sub exit_for( )
    	Dim j As Long
    	For j = 1 To 1000
        		j = j + 50
        		If  j > 100 Then Exit For 
    	Next
    	Debug.Print "j=" & j
    End Sub
最后输出的结果是多少？
Exit for 
将一个 For...Next 循环放置在另一个 For...Next 循环中
每层的循环变量要使用不同的变量名

         For I = 1 To 10
                  For J = 1 To 10
                           For K = 1 To 10
                                     ...
                           Next K
                  Next J
         Next I

For…Next语句嵌套
编程输出九九乘法表
用双层循环
外层循环变量i控制行号1~9
第i行输出i个乘式
内层循环变量j控制列号1~i（注意不是1~9）

For…Next语句嵌套示例
Sub 九九乘法表()
    Dim i As Long, j As Long, mystr As String
    For i = 1 To 9
        For j = 1 To i
          mystr=mystr & j & "*" & i & "=" & Format(j*i, "00") & " "
        Next
        mystr = mystr & vbCrLf    
    Next
    MsgBox mystr, , "九九乘法表"
End Sub
编程输出九九乘法表
Sub 九九乘法表()
    Dim i As Long, j As Long, mystr As String
    For i = 1 To 9
        For j = 1 To i
            mystr = j & "*" & i & "=" & j * i
            Worksheets(4).Cells(i, j).Value = mystr
        Next
    Next
End Sub
在工作表“Sheet4”中输出 九九乘法表
经典案例：百钱买百鸡（用穷举法，也叫试数法）
100 文钱买100只鸡，公鸡5文1只，母鸡3文1只，小鸡1文3只，必须3种鸡最少各有一只, 1文钱不可再分割。
用双层循环实现
外循环变量用cock，从1到19 ( (100-3-1)\5 )
内循环变量用hen，从1到31（ (100-5-1)\3）
 chicken = 100 - cock - hen 
内循环体内加if条件判断，满足条件就输出（3组结果）
       

循环嵌套的经典案例
Sub 百钱买百鸡_for( )
    Dim cock As Long, hen As Long, chicken As Long
    For cock = 1 To 19	 '外循环每执行1次
        For hen = 1 To 31	 '内循环执行所有次---整个
            chicken = 100 - cock - hen
            If cock * 5 + hen * 3 + chicken / 3 = 100 Then
                MsgBox "公鸡有" & cock & "只" & vbNewLine _
                & "母鸡有" & hen & "只" & vbNewLine _
                & "小鸡有" & chicken & "只"
		 End if
        Next
    Next
End Sub

       

用For…Next循环嵌套实现该案例
For…Next语句
已知循环次数，建议采用For…Next语句
Step  步长值 指定循环变量每次增加的值
最后的Next（加步长值，判断是否大于（小于）终值）
If 判断条件 then exit for 用来提前退出所在循环语句
For…Next语句的嵌套
 For…Next语句与If 语句的嵌套
 双层For…Next语句
 注意一个For语句对应一个Next结束
For…Next语句小结
断点调试
在循环语句中单独使用F8键进行单步跟踪有时是非常痛苦的，可改为用断点调试与跟踪功能相配合。（按F5运行，执行到下一个断点处）
添加断点
在语句行左侧单击，设置断点（红点），再度点击取消断点
对当前行设置或取消断点的快捷键：F9功能键
本地窗口监视变量的值
点击“视图”下的“本地窗口”，调出本地窗口
程序调试运行时，本地窗口依次罗列出所有变量的值供参考
立即窗口与监视窗口
立即窗口：如查看debug.print 语句输出结果
监视窗口：拖动一个选取的表达式（或变量)，到监视窗口中
断点调试与变量监视
以编程求Fibonacci数列的第12项为例：
在Next行设置断点，将arr(i)选中拖至监视窗口，依次按F5键，观察变量i和数组元素arr(i)的值的变化。可在本地窗口直接展开arr查看各元素的值。 也可用F8键与断点配合调试
断点与变量监视示例

VBA流程控制
循环结构
For...Next 语句
Do...Loop 语句
其他循环语句
For Each… in … Next
While … Wend

事先无法确定循环次数，但能通过条件的变化来控制循环的开始或结束，采用Do...Loop 语句
语法：有条件前置和条件后置两种格式
Do...Loop 语句
Do 
[循环体语句]
[Exit Do]
[循环体语句]
Loop  [{While | Until} 条件]
Do [{While | Until} 条件]
[循环体语句]
[Exit Do]
[循环体语句]
Loop
while 和Until两者选一：
while：当条件为 True 时，执行循环体语句
Until：直到条件变为 True 时，结束循环
条件前置和条件后置(循环头部条件判断与循环尾部条件判断)
条件前置特点是先进行条件判断,然后依据条件决定是否进入循环体
条件后置特点是无论条件设置为何，至少先执行一次循环体。然后依据条件决定是否继续循环
While 与 Until 关键字
当while 后面的条件为true时，循环继续;条件为false 时循环结束
Until与while意思相反:条件为false 时继续循环,直到until 后面的条件为true 
另外需要特别指出的是：
Loop本身并没有自动加1的功能，需要在循环体内增加语句，修改循环变量的值，否则可能运行时死循环
Do...Loop 语句
谨防死循环
注意先保存文件，再运行代码
与For 循环相比，对初学者来说Do loop 循环稍不小心就会形成死循环，因此我们要从开始就要谨慎对待 
死循环时，用Ctrl+Break中断程序（笔记本+Fn） ，或者长按Esc键中断（死循环初期可以，若程序已停止响应，则按Esc不起作用）

Do...Loop 语句
Sub 高斯求和问题用do循环()
    Dim i As Long, sum As Long
    i = 0
    Do While i <= 100
        sum = sum + i
	   ' If sum >= 1000 Then Exit  Do
        i = i + 1
    Loop
    MsgBox "i=" & i
End Sub
如果循环体内没有i=i+1，则为死循环
按Ctrl+break退出死循环，或在任务管理器关闭Excel程序
如果修改i的初值为1，则没有i=i+1时，可通过Exit  Do退出循环体
总结：对已知循环次数的情况，如上例，用For语句更方便
Do...Loop 语句示例1：高斯求和
有时事先无法确定循环次数， 则使用Do...Loop 语句更合适
爱因斯坦长阶梯问题
有一个长阶梯
若每步上2阶，最后剩下1阶
若每步上3阶，最后剩2阶
若每步上5阶，最后剩下4阶
若每步上6阶，最后剩5阶
只有每步上7阶，最后刚好一阶也不剩
请问该阶梯至少有多少阶（正确结果：119）
Do...Loop 语句示例2：爱因斯坦长阶梯
Sub 爱因斯坦长阶梯( )  '直到条件成立退出循环
    Dim 阶梯 As Long
    阶梯 = 0
    Do
    		阶梯 = 阶梯 + 1
    Loop  Until (阶梯 Mod 2 = 1 And 阶梯 Mod 3 = 2 And _ 
		阶梯 Mod 5 = 4 And 阶梯 Mod 6 = 5 And 阶梯 Mod 7 = 0)
    MsgBox 阶梯
End Sub
注意：此处循环的结束条件是当Until后面表达式的值为True时,退出循环
Do...Loop 语句示例2
Sub 爱因斯坦长阶梯2( )   '当条件成立循环
    Dim 阶梯 As Long
    阶梯 = 0
    Do While Not (阶梯 Mod 2 = 1 And 阶梯 Mod 3 = 2 And _
	  阶梯 Mod 5 = 4 And 阶梯 Mod 6 = 5 And 阶梯 Mod 7 = 0)
        阶梯 = 阶梯 + 1
    Loop
    MsgBox 阶梯
End Sub
当While后面表达式的值为True时进行循环（Not (False)为True）
反之，退出循环。
将示例2改为While条件
练习1：将爱因斯坦长阶梯代码格式改为Until前置条件
 Do Until 前置条件
…… 
Loop
练习2：将爱因斯坦长阶梯代码格式改为While后置条件
Do
…… 
Loop While 后置条件

注意：对于一条Do...Loop循环语句，条件要么放在Do后面，要么放在Loop后面，不能前后两边放
Do...Loop语句课堂练习
用循环的方法求正整数的二进制数值
Sub Dec2Bin() '将正整数转换成2进制
        Dim i As Long, j As Long
        Dim str As String, strMsg As String
        i = InputBox(“请输入一个正整数:")
        j = i 			'  j保留i的原值 
        Do While i <> 0
            str = i Mod 2 & str    'i mod 2 取得 i 除以2的余数
            i = i \ 2               '对两个数作整除，返回一个整数
        Loop
        strMsg = j & " 的2进制数为：" & vbCrLf
        strMsg = strMsg & str & vbCrLf
        MsgBox strMsg
    End Sub

Do...Loop 语句示例3：进制转换（方法见随后2张幻灯片）
附：十进制数转换为二进制数的方法
整数部分：除2取余，且除到商为0为止，余数反序书写
小数部分：乘2取整，乘到小数部分为0为止; 乘不尽时，到满足精度为止。小数部分的整数正序书写
注意：乘2取整时，取出的乘积的整数部分不再参与乘，每次都是乘积的小数部分进行乘法运算。
0.6875=(0.1011)2
123
61
30
15
7
3
1
1
1
1
1
1
0
2
2
2
2
2
2
2
1
0

高
低
0.45
0.90
1.80
1.60
1.20

例：  (123.45)10 =(         ？                )2
1111011.0111
余数
作用 ：强制退出循环语句
可以在循环中进行判断，如果需要提前结束循环则可以使用此语句。注意：Exit和Do 之间有空格，不要连在一起
Sub exit_do()
    	Dim j As Long
    	Do while j <= 1000
        		j = j + 50
        		If  j > 100 Then Exit Do 
    	Loop
    	MsgBox j
    End Sub
最后输出的结果是多少？若条件改为while  j < 100呢？
Exit  Do 
Sub Do_Loop循环嵌套示例( ) 
    Dim Check, Counter
    Check = True: Counter = 0    ' 设置变量初始值
    Do    ' 外层循环。
        Do While Counter < 20    	' 内层循环
            Counter = Counter + 1    	' 计数器加一
            If Counter = 10 Then     	' 如果条件成立
                Check = False    	 ' 将标志值设成 False
                Exit Do    		 ' 退出内层循环
            End If
        Loop
    Loop Until Check = False    ' 退出外层循环
End Sub
内层Do...Loop语句循环到第10次时将标志值设置为False，并用Exit Do语句强制退出内层循环。外层循环则在检查到标志值为 False 时，马上退出。

Do…Loop循环的嵌套（每层的循环变量不要相同）
Do…Loop语句
Exit  Do 强制退出循环
Do…Loop语句的嵌套 
Do…Loop语句与If 语句的嵌套
双层Do…Loop嵌套
Do...Loop与For…Next组成嵌套循环
For…Next语句与Do…Loop语句比较
在For语句中，循环变量在执行完循环体语句后，Next会自动累加步长值
在Do…Loop语句中，需要自己写语句控制循环变量值的变化。如果不写，在没有Exit  Do的情况下，循环会永远无法结束，这样就陷入了死循环
Do…Loop语句小结
VBA流程控制
循环结构
For...Next 语句
Do...Loop 语句
其他循环语句
For Each… in … Next
While … Wend

For each … in … Next循环
For each in 循环通常是在Excel对象中遍历
放在后面讲对象的章节中介绍
While … Wend 循环
老的VB 语法循环，现已淘汰，可以用Do  while … loop 循环取代
       
VBA其他的循环语句
VBA流程控制
选择结构
If...Then...Else语句、Select Case 语句
循环结构
For...Next 语句、Do...Loop 语句
其他循环语句（For Each… in … Next、While … Wend）
其他控制语句
With语句、Exit 语句、Goto 语句
Stop语句、End 语句

With语句
作用：在一个单一对象上执行一系列的语句
语法
With 对象
	[语句]
End With
说明
With 语句可对某个对象执行多条语句，而不用重复引用对象的名称
例如，要改变一个对象的多个属性，只需引用对象一次,然后在 With 结构中加上各属性的赋值语句，而不必在每个属性赋值时都引用对象
不能用一个 With 语句来设置多个不同的对象
With 语句可以嵌套以实现对多个对象的操作
使用时注意事项见备注页
With语句示例
使用With语句更改选中单元格的填充色和文字样式，表示强调
Sub 设置对象属性()
       With Selection
           .Font.Bold = True         '设置文字为加粗样式
           .Font.Size = 15              '设置文字大小
           .Font.Name = "隶书"     '设置字体
           .Font.Italic = True          '设置文字为倾斜样式
           .Interior.Color = RGB(255, 255, 0)'设置单元格填充颜色为黄色
       End With
  End Sub

Exit 语句
作用: 退出 For...Next、Do...Loop、Function、Sub 或 Property 代码块, 通常和条件判断配合使用 
语法：
Exit Do 
Exit For 
Exit Function 
Exit Property 
Exit Sub 
说明：不要将 Exit 语句与 End 语句搞混
Exit 表示强制退出，并不说明一个结构的终止
End 表示结束一个过程或块，如 End With，End if，End Sub
Goto 语句
作用：无条件地转移到过程中指定的行
语法：
GoTo  line
说明：
line 参数可以是任意的行标签或行号
GoTo 只能跳到它所在过程中的行
太多的 GoTo 语句，会使程序代码不容易阅读及调试，尽可能使用结构化控制语句
Goto语句示例
Sub GotoStatementDemo() 
	Dim Number, MyString 
	Number = 1  
	If Number = 1 Then GoTo Line1 Else GoTo Line2 
Line1: 
    	MyString = "Number equals 1" 
    	GoTo LastLine    
Line2: ‘ Number=1时下行语句不会执行
   	 MyString = "Number is not equal to 1" 
LastLine: 
    	Debug.Print MyString
End Sub 


行标签
Stop 语句
作用: 暂停程序的运行，相当于在程序中设置了一个断点
语法：		Stop 
说明：Stop 语句只是暂停程序，并不会退出程序
Sub 使用暂停语句()
    Dim n As Integer      	'声明变量
    For n = 1 To 10
        n = n + 1         	'变量加1
        Debug.Print (n)    	'显示变量值
        Stop              	'暂停程序
    Next
End Sub
End 语句
作用：结束一个过程或代码块
语法：End 、    End Sub 、    End Function 
		     End If 、End Select 、End With 、……
说明：End 语句单独使用表示退出程序
Sub 退出程序()
     Dim n As Integer      '声明变量
     For n = 1 To 10
          n = n + 1              '变量加1
         Debug.Print n       '显示变量值
         End                        '退出程序
     Next
 End Sub
作业1: 报数游戏规则：大家围成一圈开始报数，每逢遇到6的倍数或者末尾含6的数，例如6,12,16,18,24,26等等，就要拍手并且不能报出，谁出错了，就要受到惩罚。
编写程序，使用For…Next语句，求出1~100之间所有这样的数，并在一个消息框内输出。

For…Next语句提交作业
作业2: 计算半径r=1到r=10时圆的面积，直到面积area大于100为止，在消息框输出此时的半径和圆面积
提示：有2个循环结束条件，满足其一即可
半径r>10  (循环条件For r=1 to 10 )
面积area大于100 (循环体内 if area>100 Then exit For )

For…Next语句提交作业
作业3：使用双层For…Next语句嵌套，编写程序，在消息框输出以下图形。


For…Next语句嵌套提交作业
作业4：用Do...Loop语句编写周末询问程序
在循环体内询问“是周末吗？”，如果回答yes, 那么输出“周末不上课，可以玩去了。”提前退出循环。否则就输出“今天上课，看看明天是周末吗？”，然后继续问“是周末吗？”
最多问3遍，如果问3遍了，仍然不是周末，就退出循环
循环结束后，再用if语句判断到底过周末了没有，过周末了就输出“周末玩的很开心：）”，没过周末就输出“一直上课不开心：（”
请编程模拟上述过程
用带有返回值的msgbox进行询问, 用变体型变量“周末否”接收返回值
周末否=msgbox("是周末吗？", vbYesNo)
Do...Loop语句提交作业
作业5：用双层Do...Loop循环嵌套改写“百钱买百鸡”案例，并将三组结果用一个MsgBox消息框输出显示
提示：
Do与Loop要成对出现，缩进格式，同一语句中两者左对齐
注意对内、外循环变量的值的修改，包括两点：
循环语句前给循环变量赋初值
循环体内每次对循环变量加1
特别要思考内循环变量赋初值的语句（hen=1）应放在何处？
外循环语句的循环体内？还是外循环语句前？
内循环语句前，还是内循环语句后？（说明：两种情况都可以，但放在前面写，执行流程更清楚。）

Do…Loop语句提交作业
循环结构共提交5个作业
注意：请将作业代码复制到“你的学号_第4次作业.txt”文件中，并提交到电子作业系统的平时小作业版块
不要提交“.xlsm”文件啊 
不要将作业发到我的邮箱啊，发邮箱不计分！ 
本次作业可以在机房上机提交，如未完成，请自行提交（请在7月2日及之前提交），迟交影响成绩。
作业要求

### 四
第二部分  VBA基础知识
2
复习VBA流程控制语句
选择结构
If...Then...Else语句、Select...Case 语句
循环结构
For...Next 语句、Do...Loop 语句
其他循环语句（For Each… in … Next、While … Wend）
其他控制语句
With语句、Exit 语句、Goto 语句
Stop语句、End 语句
VBA 的字符集、关键字和标识符
VBA的数据类型 
常量和变量
运算符与表达式
VBA流程控制语句
VBA过程
常用内部函数
第二部分  VBA基础知识(4)
VBA过程（sub 、function）
VBA程序由过程组成，过程是程序中最小的逻辑单元，通常是完成某个相对独立的功能的代码集合
任何程序代码都需要有存放之地，模块就是存放过程的容器
“插入”菜单下的“模块” 
在VBA中，过程分为事件过程、属性过程和通用过程
通用过程分为Sub和Function
 将光标置于代码窗口
点击“插入”菜单下的“过程”
 弹出“添加过程”对话框
过程前的Private 修饰符
过程的执行范围仅限于本模块

VBA过程（sub 、function）
Sub过程（子程序）
没有任何返回值的一段程序
以Sub 语句开始，以 End Sub 语句结束
Call语句用来调用过程
Function过程（函数）
用于建立用户自定义函数，可以有返回值
以 Function 语句开始，以 End Function 语句结束
调用方法与Excel内部函数调用方法一致
语法
[Private | Public | Friend] [Static] Sub name [(arglist)] 
[statements]
[Exit Sub]
[statements]
End Sub
说明：
name 过程名，不可少，同变量名命名规则
arglist  参数列表，可选的。多个参数用逗号隔开
arglist 参数的语法详见幻灯片的备注页
形参（定义时的参数）和实参（调用时给的参数值）
过程调用时，实参向形参传递数据（按地址ByRef，按值ByVal）
statements 可选的。Sub 过程中所执行的任何语句组
Sub过程语法
Sub过程示例
示例1：如何执行下面两个过程？分别按F5键可以吗？
Sub MultiBeep(numbeeps as long) 'MultiBeep是有参数的过程
    Dim counter As Long
    For counter = 1 To numbeeps
        Debug.Print counter  '在立即窗口输出counter的值
        Beep   '蜂鸣器响一声
    Next counter
End Sub
Sub Message( )
    MsgBox "运行结束!"
End Sub
答：有参过程不能作为程序的起点（不能直接执行，不会出现在“宏”对话框的列表中），需要用Call语句调用
Call语句语法
语法：
[Call] 过程名 参数列表
说明： 
将控制权转移到一个 Sub （或 Function ）过程
如果使用了Call，则参数必须加括号，如Call  MultiBeep(5)
如果省略了Call，则参数的括号可省略，过程名后自动加空格
过程名：要调用的过程名称，不可缺少
参数列表： 实参，传递给相应的形参
若使用Call语法调用函数，函数的返回值将被丢弃
Call语句调用示例
新建一个过程，用Call语句调用MultiBeep过程和Message过程
Sub Main( )
    Call MultiBeep(5)
    Call Message
End Sub
执行流程： 
Main过程调用MultiBeep过程，传递实参5给形参
numbeeps，运行MultiBeep过程后，返回Main过程
然后Main调用Message过程，运行Message后，返回Main
Main过程结束
省略Call的过程调用示例
调用MultiBeep和Message过程时，可省略Call不写，此时参数5与MultiBeep过程名之间有空格，可不加括号
Sub Main2( ) 
    MultiBeep  5
    Message
End Sub
示例2：调用过程，计算每种货品的销售总价，缺少单价或件数时，给出提示
Sub过程创建与调用示例2
点击第4行，运行程序，计算出总价为60
点击第6行，运行程序，提示“数据不全，无法计算！”

示例2：货品的销售总价
Sub main_案例2()
    Dim v As Long, c As Long		  '声明变量
    v = Cells(Selection.Row, 2).value  '获得单价
    c = Cells(Selection.Row, 3)            '获得件数
    call heji (v, c)                  '调用子过程计算总价
End Sub

Sub heji(a as Long, b as Long)   '声明子过程
    Dim p As Long                        '声明总价变量
    If a = 0 Or b = 0 Then            '是否缺少数据
        MsgBox "数据不全，无法计算！"     '缺少数据则给出提示
        Exit Sub                               '出错，退出子过程
    End If
    p = a * b                                  '计算总价
    Cells(Selection.Row, 4) = p    '写入单元格
End Sub

注意：
Cells(行号,列号)表示相应的单元格
Selection.Row表示选中行的行号
修改示例2中的两个Sub过程，增加表示行数的循环变量i，i值从3 到 7，用i 替换掉 Selection.Row
相应地增加heji过程的参数，用来接收i值
执行main_案例2 过程，一次性计算所有货品的销售总价
课堂练习
示例3：输出数字回文塔，要求定义和调用子过程Pyramid(n)，它的功能是输出一个有n行的回文塔图形
共有两个过程： main_Pyramid( )、 Pyramid(n)
要求在main_Pyramid过程中输入n值，
然后调用Pyramid(n)过程
Pyramid(n)过程中使用For循环嵌套结构实现算法，并在相应的工作表中输出对应的数字回文塔
Sub过程创建与调用示例3
Sub main_Pyramid()
    Dim n As Long
    n = InputBox("请输入回文塔层数：", "回文塔")
    Pyramid (n)
End Sub
Sub Pyramid(n As Long)
    Dim i As Long, j As Long, k As Long
    For i = 1 To n
        j = n – i 
        For k = 1 To i
            j = j + 1
            Worksheets(4).Cells(i, j) = k
        Next
        For k = i - 1 To 1 Step -1
            j = j + 1
            Worksheets(4).Cells(i, j) = k
        Next
    Next
End Sub
Sub过程创建与调用示例3
示例4：延时测试
首先创建一个名为“延时”的过程
该过程根据传递的参数来决定程序延时的长度
再创建一个“测试延时”过程，调用“延时”过程
共有两个过程 
延时( ByVal  DelayTime  As  Integer )
测试延时(  )

Sub过程创建与调用示例4
Sub 延时(ByVal DelayTime As Integer)
    Dim NewTime As Long
    NewTime = Timer() + DelayTime
    Do While Timer < NewTime
    Loop
End Sub
Sub 测试延时(  )
    Dim i As Integer
    i = Val(InputBox(“开始测试延时程序，请输入延时的秒数", _
 "延时测试", 1))  '1是缺省值
    call 延时 ( i )
    MsgBox "已延时" & i & "秒。"
End Sub
Sub过程创建与调用示例4
语法格式
[Private | Public | Friend] [Static] Function name [(arglist)] [As type] 
[statements]
	[name = expression] 
[Exit Function]
[statements]
End Function
[As type] 定义函数返回值的类型
[name = expression]  通过赋值，得到函数的返回值
也就是说：要从函数返回一个值，只需将该值赋给函数名，函数名就是系统默认的返回值变量
若没有赋值，则默认返回0(数值函数)、空字符串（字符串函数）
或者Empty（Variant函数），或Nothing（对象函数）
Function过程(函数)语法
示例5：通过函数提取单元格中夹在文字中的数字，根据这些数字计算每种货品的总价
Function函数示例
点击第3行，运行程序，计算出总价为900
内置函数：
IsNumeric(expression) 判断参数是不是数字，返回True或False
Mid(string, start[, n]) 返回字符串string从start开始向右长度为n的子字符串

Sub main_getNum()
    Dim a As Integer, b As Integer                  '声明变量
    a = getNumber(Cells(Selection.Row, 1))  '调用函数获得货品件数
    b = getNumber(Cells(Selection.Row, 2))  '调用函数获得单价值
    Cells(Selection.Row, 3) = a * b                 '相乘得到总价并写入单元格
End Sub
Function getNumber(g As String)  As Integer  '声明函数
    Dim i As Integer , s As String                         '声明变量
    For i = 1 To Len(g)                            	     '遍历文字中所有字符
        If IsNumeric(Mid(g, i, 1)) Then              '判断当前字符是否为数字
		s=s & Mid(g,i,1) 			 '是数字则拼装数字
        End If
    Next
    getNumber = Val(s) 
End Function
Function函数示例5
示例6：根据华式温度来计算摄氏温度
Sub Main_Celsius()
    Dim temp As Double
    temp = InputBox(Prompt:="请输入华氏温度：")
    MsgBox "摄氏温度是： " & Round(Celsius(temp), 2)
End Sub
Function Celsius(fDegrees As Double) As Double
    Celsius = (fDegrees - 32) * 5 / 9
End Function

Function函数示例
员工星级评定
根据打分评定星级的规则如下：
0到10分， 为“*”
11到20分，为“**”
21到30分，为“***”
31到40分，为“****”
分别定义名为“员工星级评定 ”的Sub过程，和名为“评级” 的Function函数，将评定结果填入工作表的C列
提示：使用For循环语句、Select语句进行判断
Function函数课堂练习
传值调用（Byval）与传址调用（Byref）
传值调用（Byval）
将实参变量的值传到形参
形参改变不影响实参
传址调用（Byref ）
将实参变量的地址传给形参
形参改变则实参改变（共用空间）
不写传入参数类型时，VBA 默认是传址调用
Function函数中参数的调用类型
Sub func( )
    Dim x As Long, y As Long
    x = 3
    y = 平方(x)
    MsgBox x & "的平方是" & y
End Sub
Function 平方(ByVal n As Long) As Long
    平方 = n * n
    n = n - 1
End Function
实参x的值3传给形参n，n值改变，x值不受影响
传值调用（Byval）示例
Sub func2( )
    Dim x As Long, y As Long
    x = 3
    y = 平方2(x)
    MsgBox x & "的平方是" & y
End Sub
Function 平方2(ByRef n As Long) As Long
    平方 = n * n
    n = n - 1
End Function
实参x的地址传给形参n（空间共用），n值改变则x值改变
传址调用（Byref）示例
与大多数编程语言一样，VBA包含各种内置函数，它们可以简化计算和操作
很多VBA函数与Excel的工作表函数类似（或一样）
例如：VBA函数Sqr计算一个数值的平方根，等同于Excel中的工作表函数SQRT
敲VBA代码时，键入“VBA. ”，会自动显示其所有成员的列表，其中包括VBA函数（函数名的前面都有绿色的图标）
若没有自动显示，选择“工具”|“选项”命令
然后单击“编辑器”选项卡，选中“自动列出成员”选项

VBA内置函数
使用VBA内置函数Sqr，计算平方根
Sub ShowRoot()
    Dim MyValue As Double
    Dim SquareRoot As Double
    MyValue = 25
    SquareRoot = Sqr(MyValue)   '或VBA.Sqr(MyValue)
    MsgBox SquareRoot
End Sub
VBA内置函数举例
在VBA代码中，可使用很多的工作表函数（但不是所有的）
使用工作表函数的方法是，在工作表函数名前加上前缀：
   		Application.WorksheetFunction.
例：在VBA中使用工作表函数MAX，求几个数中的最大数
Sub WorksheetF()
    Dim Mymax As Double
    Mymax = Application.WorksheetFunction.Max(10,20.5,30,28.4)
    MsgBox Mymax
End Sub
在VBA中使用工作表函数
在VBA中，不能使用具有等价的VBA函数的工作表函数
例如，VBA不能访问Excel的SQRT工作表函数，因为VBA有自己的该功能函数Sqr，下面的代码运行时会发生错误：
Sub ShowRoot2()
    Dim MyValue As Double
    Dim SquareRoot As Double
    MyValue = 25
     SquareRoot = Application.WorksheetFunction.sqrt(MyValue)
    MsgBox SquareRoot
End Sub
在VBA中使用工作表函数时的注意事项
事实上，在自动弹出的工作表函数列表中，也没有sqrt这个选项
常用内置函数
数学函数
常用内置函数
字符串函数（1）
常用内置函数
字符串函数（2）（它们的函数值类型都是String）
常用内置函数
转换函数
常用内置函数
日期函数（1）
常用内置函数
日期函数（2）
作业1：Function函数创建与调用（计算1!+2!+…+10!，要求定义并调用函数fact(n)计算n!，函数类型是double）
在过程main_fact( )中使用循环语句多次调用fact函数，并将每次的函数返回值相加
最后输出累加和
要求写清楚是传值还是传地址，要定义变量类型和函数类型
本次提交作业
本次提交作业
作业2：武大学生绩点计算
成绩区间段对应的绩点如图。分别定义名为“学生绩点 ”的Sub过程和“绩点计算 ”的Function 函数（成绩作为参数，计算出的绩点作为函数返回值），将绩点填入工作表的相应列
提示：可使用For循环语句和Select语句进行判断
注意：
本周提交的作业文档名为“你的学号_第5次作业.docx”
作业文档中包括：
复制的作业代码
运行结果截屏
请提交到电子作业系统的平时小作业板块（7月2日及之前提交），迟交影响成绩

本次提交作业
The End
Believe in yourself
Let’s do it better

---
## 第三部分 Excel VBA对象模型
推荐：VBA常用技巧代码解析（按对象整理的技巧、属性、方法）
http://club.excelhome.net/thread-395683-1-1.html
### 1. 基本概念（对象、属性、方法、事件）
1. 什么是对象？对象代表`Excel(还有Word、PPT、VISIO、OUTLOOK等Office办公软件)`中的各个元素。学会了对Excel的VBA对象进行操作就等于掌握了一把可以推而广之的钥匙。
VBA通过操作不同的对象来控制Excel。

	   Excel程序（Application）
	   工作薄（Workbook）
	   工作表（Worksheet）
	   单元格（Range）
	   图表（Chart）
	   窗体（Userform）等，都是对象。

	  每个对象可以是其他对象的组成部分，也可以由其他对象组成。即`对象存在包含与被包含的集合关系`。
 2. 对象的三要素
对象具有属性、方法和事件三个要素。对象是由属性和方法组成的一个整体，而事件是一个对象可以辨认的动作，可以编写代码针对此动作来做响应。**对对象的操作就是对对象的属性、方法和事件的操作**，VBA程序通过**读取或设置对象的属性**、**调用对象的方法**、**编写事件过程**来操作Excel等办公软件。
    > 如何分辨哪些是对象的属性，哪些是对象的方法?
可以在代码窗口，按`Ctrl+J(自动联想功能)`；或者在对象后面输入下圆点（.），自动显示“属性/方法”列表
`手指卡片的项是属性，绿色图标的项是方法`。

3. 对象的属性
   什么是对象的属性? 每个对象都有特性，称为对象的属性（找名词和形容词）
`引用方式：对象名.属性`，如：Range("A1").Value  表示单元格的值属性(默认属性)。
**设置对象的属性值**：`对象.属性=属性值`，如：`Range("A1").Value=100`，把100写入A1单元格。
如：`Range("A2:H8").Formula = "=Rand()"`为区域 A2:H8 中的每个单元格设置公式，用随机数字填充该区域。
**读取对象的属性值**：`变量=对象.属性`，如： `a=Range("A1").Value`，把A1单元格的值赋给变量a。
   > 工作表的.name属性和 .codename属性的区别：
   > 每张工作表都有两个名字：
**工作表名（name）**：工作表标签上显示的名称（如“示例2”、“成绩表”、“Sheet1”等），工作表名称是显性的。
**代码名（codename）**：代码名在工作表窗口看不到（如Sheet1、Sheet2、……、表一 ），代码名是隐性的。

   > 两个名字分别对应着工作表的属性窗口的"Name"框和" (名称) "框，`工作表名可以随意修改`(在标签处修改，或通过属性窗口或用VBA语句来修改)。`代码名不能用VBA代码修改，必须通过属性窗口修改`，因为.codename是只读属性。
    > 在立即窗口分别输入:
	```c
	Print Worksheets(1).name
	Print Worksheets(3).name
	Print Worksheets("示例3").codeName 
	Print sheet3.name
	```
	> Worksheets 表示当前工作簿中`所有普通工作表的集合`；
Worksheets(n)表示Worksheets集合里的`第n个工作表`；
Worksheets(“示例3”)表示Worksheets集合里`名称为“示例3”的工作表`；
.name属性，显示相应的工作表名；
.codename属性，显示相应工作表的代码名；
sheet3.name表示`代码名是“sheet3”`的工作表的`显式名称`；

    > 代码名在VBA中可直接引用，如：Sheet1、表一……但是Sheet1不是一个属性或方法。
    
	示例1：取得*活动工作表窗口*的*宽度和高度值*，并重设窗口大小：
	```c
	Sub 设置窗口大小()
	    Dim w As Single, h As Single            '声明变量
	    w = ActiveWindow.Width                  '获得活动窗口宽度
	    h = ActiveWindow.Height 		  		'获得活动窗口高度
	    MsgBox "当前窗口的宽度为：" & w _
	     & "高度为：" & h & "。", vbOKOnly, "提示"
	    ActiveWindow.WindowState = xlNormal     '设置窗口状态
	    ActiveWindow.Width = 300                '设置窗口宽度
	    ActiveWindow.Height = 300               '设置窗口高度
	End Sub
	```
5. 对象的方法：
 什么是对象的方法？每个对象都有方法，**方法是对象本身包含的函数或过程**，用于完成特定的功能。
`引用方式：对象名.方法`，如：Range对象有一个方法是Select，作用是选中指定的Range对象，`Range("A1").Select`表示选中A1单元格。

	示例2：将选中单元格中的英文改为首字母大写，其他字母小写的格式。
	```c
	Sub 首字母大写( )
	    Dim a As String, b As String        
	    Sheet2.Range("D3").Select      	    '使用.Select方法
	    a = Selection.Value 				'获取当前单元格的值（对象的属性）
	    b = WorksheetFunction.Proper(a)     '首字母大写（对象的方法）
	    Selection.Value = b            	    '返回处理结果
   End Sub
   ```
	> Proper是Excel工作表函数，是对象`WorksheetFunction`的方法。将文本字符串的首字母及任何非字母字符之后的首字母转换成大写。将其余的字母转换成小写。
语法：`PROPER(text)`。

6. 对象的事件
什么是对象的事件？事件是一个**对象可以辨认的动作**，如激活工作表，双击按钮等，可以编写代码针对此动作来做响应（事件过程）。
不必完全记住对象及事件的名称，也不必手工输入，系统早为你准备好了。你可以在对象的代码窗口里进行选择，左边是对象，右面是事件。
如果你想知道某个对象（例如工作薄、工作表、窗体等）有哪些事件，只需要**双击这个对象，然后在代码窗口里查看即可**。
	> 方法与事件的区别：
**事件是被外在条件激活的，是被动的；方法是主动的。**
写程序时需要关心事件在什么条件下被激活；方法要主动在代码中写出在什么地方调用，如果不主动调用，就不会发生。
**事件中要自己写代码进行事件响应；方法则由系统定义代码。**
事件中如果不写代码，那么这个事件就和没发生一样；方法由系统内部定义了代码，不需要自己修改，但是能通过传送不同的参数值，改变方法执行的效果。如：
`Range("A1:D4").BorderAround ColorIndex:=3,Weight:=xlThick`
表示给A1:D4单元格区域设置红色加粗的外边框。
	
	示例3：编写工作表“示例3”的Activate事件过程。当*从其他工作表切换到该工作表时，清空工作表，同时显示提示信息*。`先在工程资源管理器中双击工作表“示例3”，打开其代码窗口，在代码窗口顶端的左侧选择“Worksheet”对象，右侧选择“Activate”事件`。然后在自动建立的事件过程中输入代码：
	```c
	Private Sub Worksheet_Activate()
	    Cells.Clear '清空工作表
	    MsgBox "欢迎使用“示例3”工作表，内容已清空，可以重新输入数据了！"
	End Sub
	```
	当从其他工作表切换到“示例3”工作表时，发生Activate事件，执行上述事件过程代码。

### 2. Excel的对象层次模型和常用对象
Excel的对象层次模型
类比：一个文件夹里可以包含多个文件夹，而这个文件夹又可以被其他的文件夹包含
对象可以相互包含。例如：一个工作薄对象可以包含多个工作表对象，一个工作表对象又可以包含多个单元格对象，这种对象的排列模式称为Excel的对象层次模型
Excel对象模型包括了大量的对象、属性和方法，像一个字典
只需记住最常用的，其他的不清楚就查字典（帮助：对象模型参考）
小技巧： 在代码中选中一个对象如“MsgBox”，按F1键，弹出“MsgBox”的帮助窗口
Excel 对象模型参考

### 集合对象
### 对象变量的声明和赋值
### 使用Range对象
### 使用Application对象
### 使用Workbook对象
### 使用Worksheet对象
Excel VBA常用的对象
Application对象
最顶端，起点
代表Excel应用程序本身
Workbook对象
代表Excel中的工作簿
Worksheet对象
代表Excel中的普通工作表
Range对象
代表Excel中的单元格或单元格区域

对象的层次引用
【例】访问单元格A1
Application. ActiveWorkbook.Worksheets(1).Range("A1").Value    
隐含使用对象
在引用Excel对象时，可以从系统能够确定与所需对象的层次最相近的对象开始引用
即如果我们引用的是活动对象，也就是被激活的对象，引用就可以进行简化
ActiveWorkbook.Worksheets(1).Range("A1").Value
Worksheets(1).Range("A1").Value
ActiveSheet.Range("A1").Value
Range(“A1”).Value          ‘这一行与上面几行意思相同
还可以进一步简写为： Range(“A1”)或[A1]，因为.Value默认属性可省略
集合是一种特定类型的对象，代表一组相同的对象
使用集合时，可以在该集合中所有的对象上执行相同的操作。如Cells.Clear，一个集合也可能是另一个对象的属性
常用集合包括Workbooks、Worksheets等，通常使用“对象名+后缀s”的形式（即复数）来表示。如：
Worksheet表示工作表对象,Worksheets表示工作表对象集合
而Range可以代表一个单元格，也可以代表一个区域，因此，Range即可代表单个对象又可代表集合对象
程序中如需引用集合中的对象，使用如下格式
集合（“对象名” 或 对象索引号）
如： Worksheets（“成绩表”） 或Worksheets（1）
集合对象
对象变量的定义（声明）
Dim 对象变量名  as  对象类型
如：Dim  rng  as  Range
Object 类型（相当于对象类型中的变体型）
定义格式：Dim  对象变量名  as  Object
Object 类型可以代表任何对象变量类型
Object 与变体类型（Variant）的区别
Variant什么都能装，包括对象和基础数据
Object可以装任何对象，但不能装基础数据

对象变量的定义和赋值
对象变量的赋值，使用Set语句
Set  对象变量名=对象
如：Set  rng=Range("a1:c6")
观察：对象变量与基础数据类型变量赋值的区别在哪儿？
Set ！Set！！ Set！！！
重要的事情说三遍，漏掉Set就会出错啊!  错误见下图：
对象变量的定义和赋值


在程序中如果需要使用大量相同类型的对象，可以使用对象数组来指定这些对象
对象数组的定义和数组的定义格式类似
如：Dim myRange( 4, 5 )  As  Range 
对象数组的使用与对象变量的使用类似（如用Set赋值）
如：Set myRange( 1, 1 )  =range("B2")
示例：使用对象数组，求成绩表中的总分
对象数组的定义和赋值
示例：使用对象数组，求成绩表中的总分
Sub 对象数组使用( )
    Dim myRange(4, 5) As Range           '定义对象数组
    Dim n As Integer, i As Integer
    For r = 1 To 4
        For c = 1 To 5
            Set myRange(r, c) = Worksheets(1).Cells(r + 2, c + 1) '为对象数组赋值
        Next
    Next
    For r = 1 To 4
        For c = 1 To 5
            Cells(r + 2, 7) = myRange(r, c) + Cells(r + 2, 7)   '求和结果写入单元格
        Next
    Next
End Sub
对象数组的定义和赋值
Range（单元格）对象
引用单元格（区域）的几种方式
单元格对象的清除和删除（方法）
单元格对象的行列属性(Row, Column)
单元格对象的行列对象(Rows, Columns)
单元格边界的界定—End 属性

使用Range对象
Range对象是Excel应用程序中最常用的对象之一
一个Range对象代表一个单元格、一行、一列、包含一个或者更多单元格区域（可以是连续的单元格，也可以是不连续的单元格）中选定的单元格，甚至是多个工作表上的一组单元格
在操作Excel内的任何区域之前都需要将其表示为一个Range对象，然后使用该Range对象的方法和属性

Range（单元格）对象
在VBA中经常需要引用单元格或单元格区域，主要有以下几种方法
使用Range属性
使用Range属性的简写格式 [ ]
使用Cells属性

使用Range引用单元格，列在前，行在后
使用Cells引用单元格，   行在前，列在后
另外：Cells表示方式和简写格式不支持VBE自动列出成员列表的功能(自动联想，Ctrl+J)

引用单元格（区域）的几种方式
方式1：Range("地址") 
Range("a1") .Select
Range("A1:C3") .Select     ‘引用单元格区域
Range("a1, a2: c3, d2:e4").Select ‘引用不连续的单元格区域
注意不要写成 Range("a1", "a2: c3", "d2:e4").Select 
方式2：Range( 起始单元格对象，终止单元格对象）   
方式2又称为对角线表示法，引用的区域是以起始单元格为左上角，终止单元格为右下角的矩形区域，当只知起点，不知终点时，很实用
Range(Range("a1"), Range("c3")).Select
方式3： Range属性的简写方式（使用方括号[ ]）
Range( [a1],  [c3] ).Select
注意：简写形式的方括号内直接写地址，不要引号，且不能有变量


使用Range属性引用单元格（区域）对象
Sub RngSelect() 
    Sheet1.Range("A3:F6, B1:C5").Select
End Sub

使用Range属性引用单元格（区域）对象 的例子
Public Sub WbMsg()
    Range("b2") = ThisWorkbook.Name
    Range("b3") = ThisWorkbook.Path
    Range("b4") = ThisWorkbook.FullName
End Sub
Sub Fastmark()
    [A2:A5] = 2
    [Fast] = 4
End Sub

代码解析： Fastmark过程使用简写方式（快捷记号）为单元格区域赋值
第2行代码使用快捷记号将活动工作表中的A2：A5单元格赋值为2
第3行代码将工作簿中已命名为“Fast”的单元格区域赋值为4
Cells带参数，用来引用一个单元格，参数：先行号后列号
格式： Cells (行号，列号) 或    Cells (行号，"列标") 
如 ：Cells(1, 1) 、Cells(1, "a") 、Range("a1")  都表示A1单元格
若Cells后面没有参数，则表示所有单元格
 Cells.Select        ‘选中当前工作表中的所有单元格
Cells属性的参数可以使用变量，因此经常应用于在单元格区域中循环
 For i = 1 To 4
        Cells(1, i).Value =i
  Next
混用Range , Cells 来表示单元格区域
Range( cells(1,1), cells(5,3))    想一下，引用哪个单元格区域？

使用Cells属性引用单元格对象
代码解析：
Cell过程使用For...Next语句为工作表Sheet1中的A10:A100单元格区域填入行号10~100

使用Cells属性引用单元格对象的例子
Sub Cell()
   Dim icell As Integer
   For  icell = 10 To 100
           Sheet1.Cells(icell, 1).Value = icell
   Next
End Sub

其实早已不知不觉的使用这个属性很久了
单元格对象的默认属性是值属性.Value ，使用时.Value可以省略不写
 Cells(1, 1).Value =50  
表示将50填入单元格A1，可以直接写为：
Cells(1, 1) =50
注意：
读取Value属性时，只能读取单个单元格的Range对象的Value值
写入Value属性（赋值）时，则可以写入多单元格Range对象的Value值
如下面的代码，第一条是有效的，第二条无效，运行会报错
Range(“H1:H12”).Value = 99    ‘同时给区域中所有单元格赋值99
MsgBox Range(“H1:H12”).Value ‘运行报错

Range对象的默认属性  .Value
例: 编程实现九九乘法表
Sub 九九乘法表()
    Dim i As Long, j As Long
    For i = 1 To 9
        For j = 1 To i
            Cells(i, j) = j & "x" & i & "=" & i * j
        Next
    Next
End Sub
双层循环+Cells属性引用单元格对象的例子
练习1：编写程序，在单元格区域A1：D5的单元格内分别填入图示中的数据
提示：
采用双层For循环结构
采用Cells (行号，列号) 格式引用单元格
引用单元格对象课堂练习
clear 方法 
Range(“k1”).Clear   清除单元格的一切(值和格式)
clearcontents 方法
Range(“k2”).ClearContents  
清除单元格的值属性，其他属性保留，使用较多
ClearFormats方法
Range("k3").ClearFormats   清除单元格的格式属性
ClearComments方法
Range(“k3”).ClearComments   清除单元格的批注信息
Delete方法
Range("k2").Delete   删除单元格
单元格对象的清除和删除方法
.Row返回行号
.Column返回列号（返回的是数值）

Sub 单元格对象的行列属性()
    MsgBox Range("A6").Row        '返回6
    MsgBox Range("C9").Column  '返回3
    MsgBox Range("a2:e7").Row    '返回区域起始行的行号2
End Sub


单元格对象的行列属性(Row, Column)
.Rows返回行对象
.Columns返回列对象（返回的是对象）

Sub 行对象与列对象( )
  Range(“a2:c6”).Rows(1).Select   '返回区域的第1行(A2:C2)
  MsgBox Range(“a2:c6”).Rows.Count   ‘返回区域所有行的行数5
  Rows.Select                          '选中当前工作表中的全部行对象
  Rows(1).Select                     '第1行全部选中
  Columns("a").Select           '第1列全选中
End Sub
单元格对象的行列对象(Rows, Columns)
两个都返回数值
Rows.Count        返回工作表的总行数
Columns.Count  返回工作表的总列数

Sub Excel有几行几列()
    Debug.Print Rows.Count & " 行  " & Columns.Count & "  列"
End Sub

Rows.Count 和Columns.Count
End 属性直接求出的是边界单元格对象
End属性的方向( xlDown, xlUp , xlToLeft, xlToRight)
如果是求取边界单元格的行号，代码如下：
  Cells(Rows.count, "a").End(xlup).Row  
  表示A列最后一个非空单元格所在的行号
如果是求取边界单元格的列号，代码如下：
 Cells(1, Columns.Count).End(xlToleft).Column 
 表示第一行最后一个非空单元格所在的列号

通常需要使用最大行数和最大列数进行反向求取
单元格边界的界定—End 属性
Range对象的Address 属性
Range对象的Offset属性
Range对象的Resize属性
Worksheet对象的UsedRange属性
Range对象的CurrentRegion属性
Range对象的Borders 属性
Range对象的BorderAround方法
Range对象的Text 属性
Range对象的Font属性
……
Range对象的常用属性和方法
作用：返回在excel 表格中的地址（返回一个 String 值）
语法：Range.Address(RowAbsolute, ColumnAbsolute)
完整语法格式见备注页
用法：
Address(0,0)  返回区域的相对引用地址
Address等价于Address(1,1) 不加任何参数，返回绝对引用地址
例子：
Set mc = Worksheets("Sheet1").Cells(1, 1) 
MsgBox mc.Address         ' $A$1 
MsgBox mc.Address(0,0 )    ' A1 
Range.Address 属性
作用：返回一个相对于指定单元格区域按一定的行、列偏移量进行偏移后的单元格区域（返回一个Range对象）
语法：Range.Offset(RowOffset, ColumnOffset)
RowOffset，可选，区域偏移的行数（正值、负值或 0）。正值表示向下偏移，负值表示向上偏移，省略不写则默认值为 0
ColumnOffset，可选，区域偏移的列数（正值、负值或 0）。正值表示向右偏移，负值表示向左偏移，省略不写则默认值为 0
Range对象的Offset属性
示例1：选中A1：C3区域偏移三行三列后的区域，并输出区域的相对地址
Sub Offset()
    Dim myRange As Range
    Set myRange = Worksheets(1).Range("A1:C3").Offset(3, 3)
    myRange.Select
    Debug.Print myRange.Address(0, 0)
End Sub
使用Offset属性的示例
代码解析： 
运行Offset过程，使用Range对象的Offset属性，选中A1：C3单元格偏移三行三列后的区域D4：F6，并输出该区域的相对地址
注意：代码中给对象变量myRange赋值时，前面要有 Set
myRange.Address(0, 0)输出区域的相对地址

作用：调整指定区域的大小，并返回调整大小后的单元格区域
语法：Range.Resize(RowSize, ColumnSize) 
RowSize，可选的，新区域中的行数。省略则行数保持不变。
ColumnSize，可选的，新区域中的列数。省略则列数保持不变。
RowSize和ColumnSize参数，不能是0 和负数
注意：
offset 和resize 属性，返回的都是Range对象


Range对象的Resize属性
示例2：选中以A1为起点的3行3列的区域
Sub Resize()
    Worksheets(1).Range("A1").Resize(3, 3).Select
End Sub
使用Resize属性的示例
代码解析： 
以A1为起点，3行3列，选中调整大小后的A1：C3单元格区域
示例3：选中对A1：C3区域对象调整大小为3行3列的区域
Sub Resize()
    Worksheets(1).Range("A1:C3").Resize(3, 3).Select
End Sub
代码解析： 
以A1为起点，3行3列，选中A1：C3单元格区域。区域大小没有变化
使用Resize属性扩展一个单元格区域，仅以区域左上角的单元格为起点
作用：返回指定工作表上已使用单元格组成的区域
语法：Worksheet. UsedRange 
注意：
.UsedRange属性是工作表的属性，返回一个Range 对象


Worksheet对象的UsedRange属性
示例4：返回第1张工作表中所使用的单元格区域
Sub UsedRange属性()
    Worksheets(1).UsedRange.Select
End Sub


代码解析： 
使用UsedRange属性选择工作表上已使用单元格组成的区域，包括空单元格
工作表中已使用A1单元格和D8单元格，运行过程将选择A1到D8单元格区域
使用Worksheet.UsedRange属性的示例
作用：返回指定Range对象所在的当前区域。当前区域是以空行与空列的组合为边界的区域
语法：
Range. CurrentRegion 
该属性对于许多自动展开选择以包括整个当前区域的操作很有用

Range对象的CurrentRegion属性
示例5：选中以A1为起点的当前单元格区域，并输出该区域的行数和列数
Sub  Currentregion属性()
    Range("a1").CurrentRegion.Select
    MsgBox [a1].CurrentRegion.Rows.Count 
    MsgBox [a1].CurrentRegion.Columns.Count
End Sub
使用CurrentRegion属性的示例
代码解析： 
选中包含“A1”单元格的连续单元格区域。以空行和空列为界
显示该区域的行数和列数
作用：用来设置单元格边框。返回一个 Borders对象集合，它代表单元格区域的各个边框
语法： Range .Borders(Index)
参数Index可以是xlEdgeLeft(左边框), xlEdgeRight(右边框), xlEdgeTop(上边框), xlEdgeBottom(下边框) 等
不加参数，则Borders集合表示Range对象的全部边框
设置线型：Borders对象的LineStyle属性
设置线宽： Borders对象的Weight属性
设置边框颜色： Borders对象的ColorIndex属性

Range对象的Borders 属性
Borders(Index)、LineStyle属性和Weight属性取值
示例6：将第2个工作表中B2单元格的底部边框颜色设置为红色粗边框
Sub SetRangeBorder()
    With Worksheets(2).Range("B2").Borders(xlEdgeBottom)
        .LineStyle = xlContinuous  
        .Weight = xlThick
        .ColorIndex = 3
    End With
End Sub
使用Borders属性的示例
代码解析： 
ColorIndex：调色板中颜色的索引值， 3代表红色。5代表蓝色
如果想对B2单元格的全部边框进行设置，程序怎么改？
With Worksheets(2).Range("B2").Borders
作用：向单元格区域添加整个区域的外边框，并设置该边框的相关属性
语法： Range.BorderAround(LineStyle, Weight, ColorIndex, Color)
ColorIndex参数按颜色索引号设置边框颜色，Color参数以RGB值指定边框颜色，不能同时指定ColorIndex参数和Color参数
示例：
Range("A1:D4").BorderAround   ColorIndex:=3, Weight:=xlThick
表示给A1:D4单元格区域设置红色加粗的外边框


Range对象的BorderAround方法
练习2：为sheet2工作表的单元格区域B4:G10设置统一的内边框（蓝色，细边框），并添加一个加粗的蓝色外边框
提示：
内边框：用Range对象的Borders 属性
外边框：用Range对象的BorderAround方法
课堂练习
问题：上页课堂练习中代码如果写成下行形式，运行报错，为什么？ 
rng.BorderAround(LineStyle:=xlContinuous, Weight:=xlThick, ColorIndex:=5)   
解答：
1.方法后面的参数不用加括号，只需用空格隔开：
		对象.方法  参数1,参数2
2.用括号也可以，但必须
（1）用call调用：Call  对象.方法(参数1,参数2)    
（2）返回一个值给变量：如先定义变量a,    a=对象.方法(参数1,参数2) 
（3）继续引用下一级方法，如：Cells.Find(what:=“灯泡”) .Activate
3.如果没有参数，那么后面的就可全部去掉了：  
		对象.方法    或者    Call 对象.方法()
4.  := 是给参数赋值，使用:=可以不按照默认的参数顺序设定参数。如：
	msgbox title:="提示",prompt:="Please input something.",buttons:=vbOKCancel
相当于
msgbox prompt:="Please input something.",buttons:=vbOKCancel,title:="提示"
相当于
msgbox "Please input something.",vbOKCancel,"提示"
方法后面什么时候加括号？什么时候不加括号？
作用：返回或设置指定对象中显示的文本。String 型，只读
语法：
Range .Text
.Value属性与.text属性的不同
.Text返回的是单元格中显示的内容，也就是你看到的是什么就显示什么
比如，A1中输入2002.111，你使用了单元格格式，只显示一位小数，那么显示的就是2002.1，.Text返回的是你所看到的2002.1
而单元格的值并不会因此而改变，仍是2002.111，所以Value会是2002.111

Range对象的Text 属性
示例7：返回第2张工作表B14单元格中显示的文本
Sub text()
    Dim c As Range
    Set c = Worksheets("Sheet2").Range("B14")
    c.Value = 1198.3
    c.NumberFormat = "$#,##0_);($#,##0)"
    MsgBox c.text 
    MsgBox c.Value
End Sub
使用Text属性的示例
代码解析： 
注意：.Value属性与.text属性的不同。
作用：返回一个 Font 对象，它代表指定对象的字体。对Range对象的字体格式进行各种设置
语法： Range . Font
Range对象的Font属性
示例8：对单元格的字体格式进行各种设置。
Public Sub RngFont()
With  Worksheets(2).Range("A1").Font
    .Name = "微软雅黑"
    .FontStyle = "Bold"
    .Size = 18
    .ColorIndex = 3
    .Underline = 2
End With
End Sub
作业：编写程序，根据给出的购货清单，求出货品总金额、每笔平均价格和平均单价，填入相应的单元格，数据保留两位小数
一行为一笔
每笔平均价格=货品总金额/行数
平均单价=货品总金额/数量
要求：如果在购货清单末尾添加一行新的购货记录，代码仍应得出正确结果
注意：请将作业代码复制及运行结果截图复制到“你的学号_第6次作业.docx”文件中，请提交到电子作业系统的平时小作业板块（7月2日及之前提交），迟交影响成绩

本次提交作业
作业：工作表截图
The End
Believe in yourself
Let’s do it better


### 使用Range对象
#### 1. Range对象的Activate方法、Select方法
- Activate方法功能：`激活单个单元格（使其成为活动单元格`
示例：选定工作表 Sheet1 上的单元格区域 A1:C3，并激活单元格B2：
	```c
	Worksheets(“Sheet1”).Activate '激活工作表 Sheet1
	Range("A1:C3").Select 
	Range("B2").Activate
	Worksheet.Activate '使当前工作表成为活动工作表。
	```
	使用Activate方法时，**该单元格必须处于当前活动工作表内**，否则Activate方法报错。对单元格区域使用Activate方法，**结果是选中该区域，并使区域左上角的单元格成为活动单元格**。
- Select方法：选择工作表和工作表上的对象，仅用于活动工作表；而`(Application.)Selection`属性**返回代表活动工作簿中活动工作表上的当前选定区域的对象**。在成功使用Selection属性之前，必须先激活工作簿，并激活或选定工作表，然后用Select方法选定单元格区域(或其他对象)。
#### 2. Range对象的Find方法、FindNext方法
- Find方法功能：在单元格区域中查找特定信息。返回一个Range对象
语法：`Range.Find(What, After, LookIn, LookAt, SearchOrder, SearchDirection, MatchCase, MatchByte, SearchFormat)`。Find方法找到第一个满足条件的对象，就停止查找；如果未发现匹配项，则返回Nothing。
![参数说明](https://img-blog.csdnimg.cn/20190702001649407.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 使用Find方法示例
示例9：查找“灯泡”，找到相应的灯泡数量并输出。
	```c
	 Sub 查找文本()
	    Worksheets("find方法").Activate         '激活工作表
	    [A1:C9].Find(what:="灯泡").Activate     '查找灯泡, 返回Range对象B5; 激活查找到的单元格
	    MsgBox "灯泡本月销售个数为" & _
	    ActiveCell.Offset(  , 1).Value & "个"   '.Offset(0, 1)向右偏移一格, 显示灯泡数量
	End Sub
	```
	> 注意：在上面例子里，如果找不到查找的内容，则程序运行时报错。如修改参数为what:=“灯管”，因为没有Range对象进行相应的.Activate方法。
	```c
	 Sub 查找文本2()
	    Dim c As Range
	    Set  c = [A1:C9].Find(what:="灯泡")
	    If Not c Is Nothing Then
	        c.Activate        ' 查找到灯泡
	        MsgBox "灯泡本月销售个数为" & _
	        ActiveCell.Offset(0, 1).Value & "个"     '显示灯泡数量
	    Else     
	        MsgBox "没找到要查找的内容。"      '如果Find方法返回的是Nothing，则提示没有找到
	    End If
	End Sub
	```

- FindNext方法：(Find方法找到第一个满足条件的对象，就停止查找)若想**继续查找剩下的满足条件的对象**，就**接着使用FindNext方法，继续由Find方法开始的搜索。**
作用：`查找匹配相同条件的下一个单元格，并返回表示该单元格的Range对象。该操作不影响选定内容和活动单元格。`
语法： `Range.FindNext (after)` after，可选的，指定一个单元格，查找将从该单元格之后开始。
说明：使用after参数时，当**查找到指定查找区域的末尾， FindNext方法将回至区域的开始继续搜索**。
循环语句中为停止查找，可保存第一次找到的单元格地址，然后测试下一个查找到的单元格地址是否与其相同(若已修改了单元格的内容，就不能用这种方式，要用其他判断方式结束循环)。

	示例9-3：查找所有的“灯泡”，将相应的行填充绿色背景，文字改为红色加粗。
	```c
	Sub 查找文本3()
	    Dim c As Range, firstAddress As Variant
	    Set c = [A3:C9].Find(what:="灯泡") '查找“灯泡”
	    If Not c Is Nothing Then
	        firstAddress = c.Address
	        Do
	            c.Activate          '激活找到单元格
	            c.EntireRow.Interior.ColorIndex = 4     '填充单元格所在行颜色
	            c.EntireRow.Font.ColorIndex = 3         '改变文字颜色
	            c.EntireRow.Font.FontStyle = "Bold"     '文字加粗
	            Set c = [A3:C9].FindNext(c)  '查找下一个目标,也可以写成after:=c
	        Loop Until  c.Address = firstAddress
	    End If
	End Sub
	```
	![](https://img-blog.csdnimg.cn/20190702002835191.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_7)
#### 在单元格区域中循环
- *使用For...Next循环语句，常与Cells属性配合*使用：
示例：在Worksheets(5)的单元格区域C1:C20中循环，将所有绝对值小于0.01的数字都设置为0。
	```c
	Sub RoundToZero1()
	    Dim curCell As Range, counter As Integer
	    Worksheets(5).Select
	    For counter = 1 To 20
	        Set curCell = Cells(counter, 3)
	        If Abs(curCell.Value) < 0.01 Then curCell.Value = 0
	    Next counter
	End Sub
	```
- **在单元格区域中循环，使用For  Each...In…Next循环语句：**
示例：在Worksheets(5)的单元格区域 A25:D29 中循环，将所有绝对值小于0.01的数字都设置为0。
	```c
	Sub RoundToZero2()
	    Dim c As Range '遍历由Range属性指定的单元格集合
	    For Each c In Worksheets(5).Range("A25:D29").Cells
	        If Abs(c.Value) < 0.01 Then c.Value = 0
	    Next
	End Sub
	```
	**如果不知道要循环的单元格区域的边界，可用CurrentRegion属性。**
示例：在Worksheets(5)的活动单元格B25周围的区域内循环，将所有绝对值小于 0.01 的数字都设置为 0
Sub RoundToZero3()
    Dim c As Range
    Worksheets(5).Range("B25").Activate
    For Each c In ActiveCell.CurrentRegion.Cells
        If Abs(c.Value) < 0.01 Then c.Value = 0
    Next
End Sub
在单元格区域中循环
示例：遍历当前工作簿，显示所有的工作表名称
Sub ShCount2()
    Dim Sh As Worksheet
    Dim s As String
    For Each Sh In Worksheets
        s = s & Sh.Name & vbCrLf
    Next
    MsgBox "工作簿中含有以下工作表:" & vbCrLf  & s
End Sub
使用For Each...Next语句遍历工作簿中所有的工作表
Range对象的autoFit方法
Range对象的copy方法
Range对象的cut方法
Range对象的Insert方法
Range对象的Merge方法
……



Range对象的常用方法
作用：更改区域中的列宽或行高以达到最佳匹配
语法： Range.autoFit
示例：调整工作表 Sheet1 中从 A 到 I 的列，第1到3行，以获得最适当的列宽和行高
先将源数据备份到sheet2以便恢复格式，并把第2行行高拖大，再运行程序
Sub autofit()
Worksheets("Sheet1").Columns("A:I").autofit
Worksheets("Sheet1").Rows("1:3").autofit
End Sub
Range对象的autoFit方法
作用：将单元格区域复制到指定的区域或剪贴板中
语法： Range. copy( Destination )
Destination 可选,指定要复制到的新域。若省略此参数，表示将区域复制到剪贴板
示例：将工作表 Sheet1 上单元格区域 A1:D2中的内容复制到工作表 Sheet2 上的单元格区域 E5:H6 中
Sub copy()
    Worksheets("Sheet1").Range("A1:D2").copy   _
    Destination:=Worksheets("Sheet2").Range("E5")
End Sub
注意此处续行符的使用
Range对象的copy方法
如果单元格区域中有公式，也可以复制公式
复制单元格区域时，也复制了该单元格区域的格式
复制单元格区域时，如果目标区域为非空单元格区域，将显示消息框，提示是否替换单元格内容，可以设置Application .DisplayAlerts属性值为False，使复制时不出现该消息框。
（旧版，10版及高版本并不显示消息框）
Sub RangeCopy()
    Application.DisplayAlerts = False
    Sheet1.Range("A1").CurrentRegion.copy  Sheet2.Range("A1")
    Application.DisplayAlerts = True
End Sub
Range对象的copy方法
作用：将单元格区域剪切到指定的区域或剪贴板中
语法： Range. cut( Destination )
Destination 可选,指定目标区域。若省略此参数，表示将区域剪切到剪贴板。
示例：将工作表 Sheet1 上单元格区域A9:C9中的内容剪切到工作表 Sheet2 上的单元格区域 A5:C5中。
Sub cut()
    Worksheets("Sheet1").Range("A9:C9").cut _
    Destination:=Worksheets("Sheet2").Range("A5")
End Sub
Range对象的cut方法
作用：在工作表中插入行或列可以用Insert方法，它可以在工作表中插入单元格或单元格区域，其他单元格相应移位
语法： Range. Insert(Shift, CopyOrigin )
Shift 可选,指定单元格的调整方式。xlShiftToRight表示右移原来的单元格，xlShiftDown表示下移原来的单元格 
CopyOrigin复制的起点
示例：在工作表 Sheet3的单元格B2上方插入一行，左侧插入一列
Sub 插入单元格()
    Dim myRange As Range                        
    Set myRange =Range("B2") 
    With myRange
.EntireRow.Insert Shift:=xlShiftDown
.EntireColumn.Insert Shift:=xlShiftToRight
   End With
End Sub
Range对象的Insert方法
作用：由指定的 Range 对象创建合并单元格 
语法： Range. Merge (Across)
Across 可选,如果为 True，则将指定区域中每一行的单元格合并为一个单独的合并单元格。默认值是 False。 
示例：Merge方法合并单元格区域时，仅保留左上角单元格的内容
Sub 合并单元格()
    Worksheets(3).Range("A10:D10").Select
    Selection.Merge
End Sub
Range对象的Merge方法
示例：使用Merge方法合并单元格区域时，将各单元格的内容连接起来保存在合并后的单元格区域中
Sub Mergerng()
    Worksheets(3).Range("A10:D11").Select
    Dim StrMerge As String
    Dim rng As Range
    For Each rng In Selection
        StrMerge = StrMerge & rng.Value
    Next
    Selection.Merge   ‘两行四列合并为一个合并单元格
    Selection.Value = StrMerge
End Sub
Range对象的Merge方法
作业1：源数据见工作表“作业1”，有三小问。要求：
每一问写一个sub过程。
第1,2小问请将相应的区域表示出来并选中。
第3小问要求使用Do Loop循环、Find方法和FindNext方法，循环结束条件请仔细斟酌，不可照抄幻灯片的代码。
本次提交作业（1）
作业2：编写一个sub过程，将左图的第2列中内容相同的连续单元格合并。结果如右图所示。提示：
可使用For…Next循环语句，循环由最后1行到第2行，步长为-1
从最后一行开始，向上逐个单元格判断连续两个单元格的内容是否相同，如果相同则合并
本次提交作业2：Range.Merge方法
提示：
循环也可以从上往下，从第2行到最后1行，但合并单元格要小心出错
用cells(rows.count,"B").end(xlup).row属性表示最后1行
为了不多次弹出合并提示框
在代码前面可先加入 Application.DisplayAlerts = False
在代码后面恢复提示框 Application.DisplayAlerts = True



