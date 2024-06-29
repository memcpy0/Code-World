

@[toc]

# 一、变量类型
1.数值型(byte、int、long、float、double)
(1).   整数的存储类型
- `byte` 字节型：  $[-100, +100]$
- `int`   一般整数型： $[-32000, +32000]$
- `long` 长整数型：   $[-2.14*10^{10}, +2.14*10^{10}]$，即，正负21亿 

(2). 小数的存储类型
- `float`    浮点型：   8 位有效数字
- `double`   双精度：   16位有效数字

(3). 字符型：在Stata的数据浏览器中，字符型会以红色的形式展现
(4). 日期型（本质上是数值型）

---
# 二、变量类型处理
## 1. 字符变量与数值变量的转换
从文本文档中**读入的数值变量**可能**以字符变量的方式**存储。而且，变量中可能包含了特殊符号，如金额`$‘、逗号`,’、斜线`/‘、百分比`%’、破折号`-‘等。我们需要将其转换会数值变量。
### (1). 字符变量转换为数值变量encode, destring, real()
`describe make`
<img src="https://img-blog.csdnimg.cn/20200325193047961.png" width="70%">
发现 `make`是字符串变量，将它转换成字符型变量：
 - `encode` 语法：
   ```
   encode varname [if] [in] , generate(newvar) [label(name) noextend]
	```
   例：`encode make, gen(cenmake)`：字符串变量，变为数值变量
<img src="https://img-blog.csdnimg.cn/2020032519350527.png" width="60%">
- `destring` 语法：Convert string variables to numeric variables.
	```
	destring [varlist] , {generate(newvarlist)|replace} [destring_options] 
	```
	`destring` 的选项：
	- `gen` ：生成新的变量；
	- `replace` ： 是替换原来的变量；和上面的 `gen` 必须有一个；
	- `ignore` ：字符变量中如果有一些非数字的符号，转换过程就会出错，必须用ignore将其忽略；
	- `force` ：强制转换成数值型变量
	
	<img src="https://img-blog.csdnimg.cn/20200331171933179.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">
	
	例：`destring  year  date  size  lev,  replace  ignore(“-/,%”)`
	例：`destring   code,   gen(code2)  ignore(“-/,%“)`
	例：`destring price_1, replace` 
<img src="https://img-blog.csdnimg.cn/20200325193752175.png" width="60%">

- `real(var)`：如果有特殊符号，real可以把特殊符号变成空值
例：`gen code2=real(code)`
<img src="https://img-blog.csdnimg.cn/20200331203445746.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%"> 

###  (2). 数值变量转换为字符变量tostring, strofreal()/string() 
- `tosrting` 语法：Convert numeric variables to string  variables.
	```
	tostring varlist , {generate(newvarlist)|replace} [tostring_options]
	```
	`tostring` 选项：同上
	<img src = "https://img-blog.csdnimg.cn/20200331203301573.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">
例：`tostring   year  month  day, replace`
例：`tostring   year, gen(year2)`

- `strofreal(n)`：将数值型转换为字符型；等价于函数string(n)
  例：`gen year2=string(year)`
  <img src="https://img-blog.csdnimg.cn/20200331203846341.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">

## 2. 字符变量的处理函数
- 字符串子串：`substr(s,n1,n2)`，the substring of s, starting at n1(**下标从1开始**), for a **length** of n2
例：`gen year=substr(date,1,4)`
注意：**每个英文字母占一位，但每个中文字符占两位**。

- 字符串分割： 
	`split` 语法： 
	```
	split strvar [if] [in] [, options]
	```
	`split` 选项：(还有一些选项用于将分割的字符串变量转换成数值型，没有列出来)
	<img src="https://img-blog.csdnimg.cn/20200331205826851.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">
例：`split  date, parse("-")`
例：`split riqi, parse("-") destring` 将日期按照"-"分割并转换为数值型。最后生成了 `riqi1, riqi2, riqi3` 三个数值型变量。

- 字符串合成：`+`
例：`gen  a=  "I am" +  " a teacher"` 

- 字符匹配函数：`strmatch()`
  <img src="https://img-blog.csdnimg.cn/20200331211157566.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
例：`gen  a=strmatch(ind, "C")`
- 字符匹配函数：`regexm(s, re)`
例：`gen  a=regexm(ind,  "C")`
<img src="https://img-blog.csdnimg.cn/2020033121143551.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

## 3. 日期变量的处理
 
在Stata中，**日期变量以数值形式存储**（1960 年1月1日被认为是第0天，因此1959年12月31 为第-1天），但通过`format`命令可以按日期格式显示。

对于**存储为数值型变量**的日期，可以使用函数 `year( )、month( )、day( )` 从中提取年月日信息。

输入的日期**常常存储为字符变量**，例如“1999-03-09”或“1999/03/09”，可以使用函数 `substr( )`从中提取年月日信息。有了年月日信息，可以使用函数 `mdy( )`将其合并成日期变量（以数值变量存在，可显示为日期变量）。

---
# 三、变量的创建
## 1. _n和_N的妙用
- `_n` 表示当前观察值(Obs)的序号(从1开始到_N，**是一个变量**)，它的取值会随着`by` 和 `sort` 变化；`_N`为总的观察值的数目；当然，这两个值永远存在，但不会直接显示出来，可以建立一个变量`gen n = _n`/`gen N = _N` 来显示
- `varname[_n-j]`：表示滞后 $j$ 期的变量
- `gen lagSales=sales[_n-1]`：对于一个变量 `sales`，产生滞后一期的滞后项 `lagSales`；
- `bysort code year : gen lagVar = var[_n - 1]`：分组滞后；
- 同样利用 `gen` 和 `_n`，可以产生前导项：`gen nextVar = var[_n + 1]`；
- 产生最后一项：`gen last = var[_N]`；
- 产生一个差分变量：`gen diff = sales - sales[_n - 1]`；
- 增长率：`gen gsales = (sales - sales[_n - 1]) / sales[_n - 1]`；
- 是否是第一项/最后一项：`if _n == 1 / if _n == _N`
 
## 2. 虚拟变量的产生
- 使用 `generate` 和 `replace` 或者是关系运算产生虚拟变量：
	```
	sysuse nlsw88.dta, clear
	gen ln_wage = ln(wage)
	gen dum_black = (race==2) //虚拟变量
	gen tenure_bl = tenure*dummy_black
	```
	<img src="https://img-blog.csdnimg.cn/20200325194113316.png" width="30%">

- 用 `tab` 基于**类别变量**生成虚拟变量：
<img src="https://img-blog.csdnimg.cn/20200401005919772.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">
例：`tab  race, gen(dum_ )`

- `xi`：
xi    i.race
    xi    i.race,  prefix(dum_ ) 
    xi	   i.race,  prefix(dum_ )	noomit

### 交互项的产生
在回归的时候需要用到交互项，**产生交互项**的方法如下：
- 基本方法：`generate` 生成一个交乘项。
例：`gen   jh=state*size` 

- 因子变量的应用：`i.    c.    #     ##`  (`help   fvvarlist`，`help varlist`）
<img src="https://img-blog.csdnimg.cn/20200331225004816.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

可以用xi:reg来对交乘项进行回归。
```
reg   roa   state    lev    size   
reg   roa   state    lev    size    c.lev#c.size  ( reg  roa  state  c.lev##c.size)
reg   roa   state    lev    i.state#c.lev  size  (reg  roa  i.state##c.lev  size)
reg   roa   state    lev    nsize  c.lev#i.nsize
分类变量加i. 连续变量加c.  #交乘
```
### egen命令 



因子变量
**默认对照组**：`reg   wage   i.race`
**设置对照组**：`reg   wage   ib3.race`

 
条件函数：`cond(s, a, b, c)`
例：`gen  x=cond(age>50, 1, 0, .)`
`S`是条件	`a`是满足条件的     `b`是不满足条件  `c`是备注项										

 

egen与gen的差异  可以调用一些函数 
gen  s1=sum(sales)   累加
egen s2=sum(sales)  总体加总

egen命令

gen     a1=(b+c)/2
egen    a2=rmean(b  c)
产生各种统计参数
egen  m=median(sales)   ///  sd( )  mean( )  min( )  max( )  count( )
bysort  ind: egen  m=median(sales) 
egen  m=median(sales), by(ind)

---
# 四、程序的结构与运行
capture program drop myprog
program define myprog
version 11
... ...
end
程序的运行

将其保存为文件myprog.ado（保存地址可以自行设置） 
方法一：
adopath + D:\stata10\ （ado程序保存的地址）
myprog

方法二：
首先点击“run current file”运行刚才的程序
myprog

程序的管理

program dir         /*查找内存中的程序*/
program list myprog
program list _all
program drop mynike    /*删除内存中调入的程序，但不影响硬盘中存储的文件*/
program drop _all

两个技巧

避免列印过多的结果： 
quietly {
}
避免数据在程序执行过后有所变动：
preserve
… …
restore 

单值 

存放数值
clear
set obs 10
scalar a = ln(10) + (2^5)/exp(2)
display a

存放字符串 
scalar s1 = "hello, Arlion"
scalar s2 = substr(s1,1,5)
di s1
di s2

暂元
存放数字
 local a = 5
  dis `a'
 local b = `a' + 7
  dis `b'

存放文字
 local name1  lgm: 
  dis "`name1’"
 local name2  武汉大学经济与管理学院
  dis "`name2'"
 local name3  `name1'`name2'
  dis "`name3'"

存放变量名称
 local varlist price weight rep78 length
  sum `varlist’

数学运算符的处理
 local a 2+2
  dis `a'
  dis "`a'"
 local b = 2+2
  dis `b'
  dis "`b'"

 数学运算式的简化
 gen price2 = price^2 
 local i = 1
 local j = (`i'+7)/4
sum price`j'
全局暂元

global aa "This is my first program!"      
dis "$aa"
 
global x1 = 5
global x2 = 2^$x1
dis $x2
其他暂时性物件 
暂时性变量
sysuse nlsw88, clear
tempvar x1 x2
gen `x1' = hours^2
gen `x2' = ln(wage)
sum `x1'  `x2'

暂时性文件
tempfile  filex
use `filex’, clear


## 1. 控制语句
## 2. 顺序语句
## 3. 条件语句
scalar gender = 1 
if gender ==1 {
   dis “男孩”
}
else if gender ==0 {
   dis “女孩”
}


## 4. 循环语句
### （1）条件循环：while 语句
```
local j = 0
while j' < 5 {
	scalar a = `j' ^ 2
	dis a
	local j = `j' + 1
}
```


local j = 0
while `j'<5 {
   scalar  a=`j’^2
   di a
  local j = `j'+1
} 

### （2）步长的循环：forvalues 语句

格式一：
```
forvalues i = 0(-1)-14 { //从0到-14, 每次减一
	dis `i'
}
```
格式二：
```
forvalues i = 0/4 {
	dis `i'
}
```
格式三：
```
forvalues i = 10(-2)1 {
	dis `i'
}
```
 
变量、暂元、文件等的循环：foreach 语句

任意格式：foreach v in ... 
追加样本：
use d1, clear
foreach v in  d2  d3{
append using `v'
}

变量名循环：
local vars "price weight length"
foreach v of varlist `vars‘ {
sum `v' 
}

暂元循环：

local vars price weight length
foreach v of  local  vars {
  gen `v'_2 = `v'^2 
}

数字循环：

foreach v of numlist 1 4/8 13(2)21 103 {
    display  `v'
}




引用Stata命令的结果
return list
ereturn list
sreturn list
如何显示留存在内存中的结果？
与Stata命令的类型有关
Stata 命令分为三种类型：
*  (1) r-class 与模型估计无关的命令； 如，summary
*  (2) e-class 与模型估计有关的命令； 如，regress
*  (3) s-class 其它命令；如，list

显示留存值的方法：
*  **r-class**: `return  list`  与模型估计无关，如`summary`
*  **e-class**: `ereturn list`  与模型估计有关
*  **s-class**: `sreturn list`   其他   如：`list` 

留存值分为四种类型：
* 单值：  如，r(mean), r(max), r(N), e(r2), e(F)
* 矩阵：  如，e(b), e(V)
* 暂元：  如，e(cmd), e(depvar)
* 函变量：如，e(sample)



