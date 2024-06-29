@[toc]


---
# 零、数据源
数据是信息时代的石油，做Stata分析的首要步骤是要学会下载数据。常见的经济统计数据库有Wind，同花顺，CSMAR等。

下面以[CSMAR](http://www.gtarsc.com/#/index)为例子介绍如何下载常用的财务报表数据。

<img src="https://img-blog.csdnimg.cn/20200527095359451.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

进入官网，登录账户，点击【数据中心】：
<img src="https://img-blog.csdnimg.cn/20200527095511976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

进入【公司研究系列】：
<img src="https://img-blog.csdnimg.cn/20200527095554494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

点击【财务报表】，可以下载财务报表数据：
<img src="https://img-blog.csdnimg.cn/20200527100308835.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

<img src="https://img-blog.csdnimg.cn/2020052710033073.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

<img src="https://img-blog.csdnimg.cn/20200527100345301.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

这里全部是灰色的，因为我没买，作为苦逼的学生党，建议去淘宝上，买个一个月或者两周的CSMAR数据下载账号。下面是该数据库的部分数据字段的说明：
<img src="https://img-blog.csdnimg.cn/20200527100613433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="61%">

如果下下来数据，会发现CSMAR的数据格式是，对每一个公司，都会**按照连续的年份**形成**多行**记录。而对于WIND来说，其给出的数据文件会把每个公司的**数据字段，按照连续的年份作为列**。两者之间是存在差别的，但很多时候需要两个数据库一起使用，为此我们需要**知道它们间的差异**。下面给出的要用到的两张表，分别是从CSMAR和WIND下载的。
<img src="https://img-blog.csdnimg.cn/20200528125742966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="39%">  <img src="https://img-blog.csdnimg.cn/20200528125853963.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="55%">
 
除了格式上的差异外，还有内容上的诸多不同，比如说：股票研究等的可以用CSMAR的【股票市场系列】，WIND就没有；但债券研究可以用WIND......等等。

另外，上面给出的表是经过处理的。比如说，将证券代码字段以 `code` 为属性名，证券名称以 `name` 为属性名，日期写成 `riqi` 等；对CSMAR，将中文解释的两行删去；对WIND，把文件尾的【数据来源】删除，将2008年的资产总计写成 `zc2008`，依次类推......

---
# 一、Stata简介
1.介绍
$Stata$ 的发展历史不多说，我们说一下它的优点，比起其他正经的编程语言来说，Stata在什么地方有优势：
*	短小精悍
*	运算速度极快
*	绘图功能卓越
*	更新和发展速度惊人	 
*	**适合科研的数据处理和分析**：科研人员不需要规模化的项目结构，而是需要这种脚本类型的、动态的、随写随扔的、容易出结果的软件，因此，Stata在科研领域相对比较受欢迎。

 2.网络资源
* Stata官方网站：http://www.stata.com/links/
* UCLA(加州大学洛杉矶分校)提供的网络教程：http://stats.idre.ucla.edu/stata/
* 人大经济论坛
* 百度

3.语法规则
除了矩阵运算及某些特殊符号以外，**所有的命令、变量和关键词一律用小写**；**每一行命令以回车作为结束符**，模型和选项之间用逗号隔开。

对于变量：变量名**第一个字元可以是英文字母或下划线，但不能是数字**；变量名最多只能包括英文字母、数字或下划线；变量名不能是中文；数值变量与文字变量：文本变量的取值可以是中文；**变量的遗漏值以“.”标识**。


---
# 二、Stata基本操作
## 1. 日志操作
 为什么需要日志？事实上，记录日志是一种良好的操作习惯，能记录操作过程。

* `log using` + `存储路径/日志名.log`：不写也可以，日志会存在默认路径下；
* `log using` +  `存储路径/日志名.log, append`：打开日志文件，并接着原来日志文件记录；
* `log using `+  `存储路径/日志名.log, replace`：打开日志文件，覆盖原来日志文件。
* `log off`：日志文件记录暂停；
* `log on`：日志文件记录暂停结束，继续记录；
* `log close`：日志文件记录结束，自动保存记录结果。


## 2. do文档操作
比起直接写命令运行，写在文档中，易于保存。这是另外一个强烈建议。请在do-file editor中写命令，并留下清楚的注释。
* `doedit`：打开 $do$ 文档编辑器；
* `doedit mylog.do`：打开一个已存在的 $do$ 文档，可指定完整路径。

## 3. 数据操作
导入数据后： 
* `edit`：编辑数据, 不能修改数据
* `browse`： 浏览数据,不能修改数据
* `format price wei len mpg %6.3f`：定义变量的显示格式
* `br, nolabel`：browse 的简写
* `display`：显示输出结果
* `quietly`：不需要屏幕显示
* `set more off`：关闭屏幕滚动
* `set more on`：开启屏幕滚动

## 4. 文件路径和设置
* `pwd`：显示当前工作路径
* `cd "/Users/dell"`：进入文件夹,路径内容自己确定
* `dir`：当前路径下的文件
* `mkdir`：创建新的文件夹
* `ls`：显示目录的内容，等同于`dir`
* `sysdir`：系统文件的路径
* `help`： 后面接命令，查看Stata内部命令的帮助文件，很强大
* `search`：接任何语句，查找相关信息	
* `findit`：搜寻外部命令，然后就可以手动下载
* `ssc install cmdname`：安装新命令
*  `findit dynamic cmdname`：搜索关键词,下载外部命令

例子：
```
ssc install fsum //下载fsum
```
## 5. 内存控制
- `memory`：内存查看
- `set memory #`：设置内存，#为内存值，如 $100m$
- `set matsize #`：设置最大矩阵阶数，#为数字
- `set maxvar #`：设置最大变量个数，#为数字
- `clear`：清空内存

## 6. Stata小技巧
- `set excelxlsxlargefile on`：excel文件太大，比如几十M，用这个命令可以读进去
- `preserve ... restore`：这两条命令间的命令都是后台运作，不会影响主表
---
# 三、基本的Stata命令
## 1. 三类运算命令
可以看一下运算符有哪些：
```
help operator  //运算符	
```
<img src="https://img-blog.csdnimg.cn/20200325142503867.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

**关系运算符**：`==;  >;  <;  >=;  <=;  !=;  ~=；`
```
sysuse auto, clear //使用自带的数据文件 sysuse -- Use shipped dataset(1978 Automobile Data)
describe //看看导入的数据文件的各个变量的含义
```
<img src="https://img-blog.csdnimg.cn/20200325143510774.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

```
list price //列出变量price的所有取值
```
<img src="https://img-blog.csdnimg.cn/20200325143330903.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

关系运算符常常和`if`连起来使用：
```
list price if foreign == 0 //列出记录中foreign为0的price值, 即domestic
```
```
sum  price if foreign != 1 //对记录中foreign != 1的price进行描述性统计
```
<img src="https://img-blog.csdnimg.cn/20200325151428990.png" width="70%">

**逻辑运算符**： `& -->(and) ;  | -->(or)`
```
sysuse auto, clear
sum price weight if (foreign==1 & rep78<=3)
```
<img src="https://img-blog.csdnimg.cn/20200325151918873.png" width="70%">

```
sum price wei if (rep78==1) | (rep78==5) | (foreign !=0)
```
<img src="https://img-blog.csdnimg.cn/20200325152124631.png" width="70%">

```
sum price wei if (rep78>2 & rep78<5) | (price>10000)
```	
<img src="https://img-blog.csdnimg.cn/2020032515214344.png" width="70%">

**算术运算符**：`+ - * / ^(幂)`；还有`exp`，`ln`等。
```
display 5^2    //幂
dis     1 - 3*2 + 4/5 - 9^3 
dis     2*_pi 
```
<img src="https://img-blog.csdnimg.cn/20200325152247822.png" width="40%">

**练习**
a. 请给出1978年维修次数低于三次的国产车的价格，重量和长度的描述性统计：
<img src="https://img-blog.csdnimg.cn/20200325152521286.png" width="70%">
b. 请计算出 [（70+12）乘以5除以3]的平方的值
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200325152723251.png)
## 2. 导入数据
### (1). 导入Excel资料：import excel
可以直接复制粘贴；或者菜单栏，`file-import`；或者命令：
```
import excel "路径", sheet("...") firstrow
sysuse auto, clear //调入Stata自带数据文件
```
### (2). 手工录入：input valist 和 end
还可以运用do file输入资料：
```
input id female race ses str3 schtype prog read write math science socst  //input 后面接的是变量名
		147 1 1 3 pub 1 47 62 53 53 61    //变量具体取值
		108 0 1 2 pub 2 34 33 41 36 36
		18 0 3 2 pub 3 50 33 49 44 36
		153 0 1 2 pub 3 39 31 40 39 51
		50 0 2 2 pub 2 50 59 42 53 61
		51 1 2 1 pub 2 42 36 42 31 39
		102 0 1 1 pub 1 52 41 51 53 56
		57 1 1 2 pub 1 71 65 72 66 56
		160 1 1 2 pub 1 55 65 55 50 61
		136 0 1 2 pub 1 65 59 70 63 51
end
save "练习.dta" 
```
<img src="https://img-blog.csdnimg.cn/20200325153537607.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="85%">

### (3). 以tab/空格分开的文件
- 以tab分开的文件(.csv/.txt)：`insheet   [varnames]  using  filename , clear`
- 以空格分开的文件：`infile  [varnames]  using  filename , clear`
### (4). 打开以前的dta资料
可以用菜单，或者命令：
```
use "练习.dta", clear   //use后面是数据资料存放路径
```

## 3. 数据的导出
- 数据存储：`save filename, replace`
- 导出为以`tab`分开的文件：`outsheet  [varnames]  using  filename` 
- 导出为`excel/word/tex`格式： `dataout, save (filename) word tex excel replace` 	
- `logout, save(tablename) excel replace: tabstat,s(n mean median sd max min) c(s) long f(%9.3g)`
- `logout, save(tablename) excel replace: pwcorr`


## 4. 数据的追加
### (1). 横向追加merge
在数据表的横向**追加另一张表的变量**。
```
clear
input id female race ses str3 schtype   //input 后面接的是变量名
	147 1 1 3 pub   //变量具体取值
	108 0 1 2 pub
	18 0 3 2 pub 
	153 0 1 2 pub
	50 0 2 2 pub
	51 1 2 1 pub
	102 0 1 1 pub 
	57 1 1 2 pub
	160 1 1 2 pub
	136 0 1 2 pub 
end
save "练习1.dta" 
```
<img src="https://img-blog.csdnimg.cn/20200325154021946.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

```
clear
input id female prog read write math science socst  //input 后面接的是变量名
	147 1 1 47 62 53 53 61    //变量具体取值
	108 0 2 34 33 41 36 36
	18 0 3 50 33 49 44 36
	153 0 3 39 31 40 39 51
	50 0  2 50 59 42 53 61
	51 1 2 42 36 42 31 39
	102 0 1 52 41 51 53 56
	57 1 1 71 65 72 66 56
	160 1 1 55 65 55 50 61
	136 0 1 65 59 70 63 51
end
save "练习2.dta"  //路径改一下		
```
<img src="https://img-blog.csdnimg.cn/20200325154107402.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

现在我们有了两张表了，该如何合并呢？
```
use "练习1.dta" //使用的主表
merge 1:1 id using "练习2.dta" //横向追加 //使用变量id合并 //主表和副表的id是1:1
```
<img src="https://img-blog.csdnimg.cn/20200325155150664.png" width="55%">
<img src="https://img-blog.csdnimg.cn/20200325155327306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="80%">

### (2). 纵向追加
```
clear
input id female race ses str3 schtype prog read write math science socst  //input 后面接的是变量名
	147 1 1 3 pub 1 47 62 53 53 61    //变量具体取值
	108 0 1 2 pub 2 34 33 41 36 36
	18 0 3 2 pub 3 50 33 49 44 36
	153 0 1 2 pub 3 39 31 40 39 51
	50 0 2 2 pub 2 50 59 42 53 61
end
save "练习3.dta"  //路径改一下
		
clear
input id female race ses str3 schtype prog read write math science socst  //input 后面接的是变量名
	51 1 2 1 pub 2 42 36 42 31 39
	102 0 1 1 pub 1 52 41 51 53 56
	57 1 1 2 pub 1 71 65 72 66 56
	160 1 1 2 pub 1 55 65 55 50 61
	136 0 1 2 pub 1 65 59 70 63 51
end
save "练习4.dta"  //路径改一下		
```
<img src="https://img-blog.csdnimg.cn/20200325155757571.png" width="85%">
<img src="https://img-blog.csdnimg.cn/20200325155830352.png" width="85%">

```
use "练习3.dta"	
append using "练习4.dta" //数据合并到了下方
```
<img src="https://img-blog.csdnimg.cn/20200325160218119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="84%">
 
 
 ### (3). 资料合并与追加的总结

**横向合并**：`merge`
例：`merge  1:1  code  year  using   filename`
例：`merge  1:m  code   using  filename`
例：`merge  m:1  code   using  filename`
例：`drop if _m == 2`  删除那些仅仅出现在副表的观测值

<img src="https://img-blog.csdnimg.cn/20200331235823907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70"  width = "65%"> 

<img src="https://img-blog.csdnimg.cn/20200401000404792.png" width = "65%">
<img src="https://img-blog.csdnimg.cn/20200401000611514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

纵向追加：`append` 
<img src="https://img-blog.csdnimg.cn/20200401000957650.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "65%">

---
# 四、数据分析
## 1. 基本命令

- `sysuse auto, clear`：调入Stata自带数据文件
- `describe`：描述数据资料来源与资料大小
- `list`：依次列出观察变量的各个值
  `list price foreign in 1/20, sepby(foreign)`：列出前20个price、foreign的变量值
 <img src="https://img-blog.csdnimg.cn/20200325185451253.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "40%">
- `lookfor "Repair"`：搜索包含特定关键词的变量
 <img src="https://img-blog.csdnimg.cn/20200325185645454.png" width = "65%">
- `sum //summarize`：基本统计量描述；其调用后的结果可以用`return list`来查看
- `codebook`：描述资料的详细内容；`codebook var
` 不重复值统计
- `compress`：自动压缩资料的存储格式
 <img src="https://img-blog.csdnimg.cn/20200325161005925.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "45%">
- `count if price > 10000`：计数
- `ceil(wage)`：向上取整
- `sort price`：按照price进行升序排列
  <img src="https://img-blog.csdnimg.cn/20200325161606546.png" width="30%">
 - `order var1 var2...`：变量顺序的重新排列，可以将这些变量按照order后面的顺序排列，便于查看
- `gsort price`：按照price进行升序排列
  `gsort -price`：按照price进行降序排列（gsort可以用"+"按照某个变量的升序，用"-"按照某个变量的降序排列）
<img src="https://img-blog.csdnimg.cn/2020032516170599.png" width="30%">
- `replace price==15000 if price>15000`：替换，和关系运算合用，替换那些大于15000的price值为price

下面介绍其他的一些更加重要的命令。
## 2. 分组统计by, bysort


<img src="https://img-blog.csdnimg.cn/20200325161917186.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width ="70%"> 

- `bysort foreign: sum price wei len`：分组统计

<img src="https://img-blog.csdnimg.cn/20200325162316653.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">



- `bysort code : gen n = _n`

 **盈余管理分行业回归，可以用by**。
##  3. 制表tabulate, tabstat
- `tabulate foreign`：列成表格，呈现频数 
<img src="https://img-blog.csdnimg.cn/2020032516263077.png" width = "70%">  
- `tabulate foreign rep78`：二维频数列表（注意到Total为69，原因是有一些缺失值）
 <img src="https://img-blog.csdnimg.cn/20200325162844374.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70"  width = "70%">
 <img src="https://img-blog.csdnimg.cn/20200325163014588.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 
-   `tabulate foreign rep78, sum(price) mean`
<img src="https://img-blog.csdnimg.cn/20200325163330414.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "75%">

-  `tabstat price wei len mpg turn foreign, s(mean sd p50 min max)`：Compact table of summary statistics
<img src="https://img-blog.csdnimg.cn/20200325180712247.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

当然，后面我们会看到`tabulate`在创建指示变量中，也有着重要的作用。

---
## 4. 样本等分命令
找到样本的 ${1\over 3}, {2\over 3}$ 分位点很复杂，因为你不知道它在哪里。取而代之的是用 `group` 命令。当然，需要先指定用哪个变量分组，分组前将该变量排序：
```
* 两个命令一定要连着用
sort in_num
gen aa = group(5) //5是分组的组数，可以自己改变
```
<img src="https://img-blog.csdnimg.cn/2020033022181761.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

## 5. 分位点和分位数
与分位点相关的有几个命令：
<img src="https://img-blog.csdnimg.cn/20200401002327257.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="65%">

命令`pctile`：产生分位数，不能与by连用
例：`pctile    x=size,  nq(10)  `
例：`pctile    x=size,  nq(10![在这里插入图片描述](https://img-blog.csdnimg.cn/20200401002757133.png))   gen(y)`


命令`xtile`：产生所属组别，不能与by连用
例：`xtile  x=size, nq(10)`

函数 `pctile( )`与`xtile( )`，可以与 `by` 或 `bysort` 连用
例：`bysort   year: egen   x=pctile(size), nq(10)`
例：`bysort   year: egen   x=xtile(size), nq(10)`

极端值的处理

极端值的查找：`adjacent` (外部命令)
p25, p75, iqr (=p75-p25)
the highest = p75 + iqr*3/2
the lowest = p25 - iqr*3/2
缩尾处理(winsorizing)：winsor (外部命令)
截尾处理(truncating)
例：  _pctile roa,p(1 99)
            drop  if   roa<r(r1)  |  roa>r(r2)
那么我们不能够找出分位点吗？可以，用`_pctile`找分位点：
```
_pctile in_num, percentile(33 66) //找出变量in_num的33%和66%分位点
//运行完后要用return list看数值
return list
```
<img src="https://img-blog.csdnimg.cn/20200330222614673.png" width="50%">

有了分位点，可以手动将连续变量映射为类别变量。下面是 `_pctile` 的命令语法和定义：
<img src="https://img-blog.csdnimg.cn/20200330223058414.png" width="70%">
<img src="https://img-blog.csdnimg.cn/20200330223317556.png" width="70%">

---
## . 变量的创建gen, egen
- `gen weight2=weight^2`：创建一个新变量`weight2`，值是`weight`的平方
<img src="https://img-blog.csdnimg.cn/20200325191249761.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="86%">
- `egen zprice = std(price)`
  `egen rpice = mean(price), by(foreign)`：按照foreign分组并生成新的变量
  `egen mprice = median(price), by(foreign)`：按照foreign分组并生成新的变量
  `egen standard = std(in_num)`：把变量in_num标准化
  `egen a = rowmean(var1 var2)`：行变量求平均
<img src="https://img-blog.csdnimg.cn/20200325192455499.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
egen是gen的扩展，**不用纠结具体有啥区别**。总之，平时生成变量用gen，如果**想要使用某些特定的函数生成一些变量**，就用egen。

其实也简单，`gen newvar = exp`表示gen生成的新变量应该是由表达式运算得到的，`egen newvar = fcn(arguments)`，表示egen生成的新变量应该是由函数运算得到的。

其中，需要注意的一点是：==gen使用sum是累计求和，egen使用sum是全部求和==。

另外，egen的功能很强大，对于很多只用于列的函数也提供了用于行变量的版本，前面提到的`rmean`，...
<img src="https://img-blog.csdnimg.cn/20200331000102575.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

---
##  . 重命名变量和删除变量
- `rename weight wei`：单个变量更名
- `rename (length gear_ratio) (len gr)`：批量重命名
<img src="https://img-blog.csdnimg.cn/20200325190330103.png" width = "30%">
- `drop turn gr make`：删除变量，可以删除多个
- `drop if rep78==.`：和关系运算一起用，删除那些缺失值
<img src="https://img-blog.csdnimg.cn/20200325190657686.png" width="30%">
<img src="https://img-blog.csdnimg.cn/20200325190812837.png" width="35%">
 
##  . 标签操作
### 1. 标签重命名
- `label variable ln_wage "In(wage)"` 
- `label variable dum_black "1=blak;0=otherwise"`
- `label variable tenure_bl "tenure*dum_black"`
<img src="https://img-blog.csdnimg.cn/20200325194514586.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">

改了标签后，用`describe`时，就可以看到新增的变量的描述了。

### 2. 增加数字-文字对应表
```
label define hard_work 1 "hours>行业中位数" 0 "otherwise"
label list hard_work  //显示特定变量的数字-文字对应关系
labelbook             //显示所有变量的数字-文字对应关系	
```
<img src="https://img-blog.csdnimg.cn/20200325201351264.png" width="70%">


---
##  . 相关系数和相关系数矩阵
这是皮尔森相关系数：
<img src="https://img-blog.csdnimg.cn/20200331011159599.png" width="70%">

具体的选项：(`sig` 显示显著性水平，`star` 标星号）
<img src="https://img-blog.csdnimg.cn/20200331011353411.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
sysuse auto, clear
pwcorr price weight length mpg //得到这几个变量的相关系数矩阵, 官方命令
```
<img src="https://img-blog.csdnimg.cn/20200325213757712.png" width="70%">

```
graph matrix price weight length mpg  //相关系数矩阵图(散点图)
```
<img src="https://img-blog.csdnimg.cn/20200325214007875.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

##  . 直方图和密度函数图
- `histogram price`：直方图。
<img src="https://img-blog.csdnimg.cn/2020032521405048.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">
- `histogram price, frequency`：频数图

<img src="https://img-blog.csdnimg.cn/20200325214216677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

- `kdensity price`：核密度图
<img src="https://img-blog.csdnimg.cn/2020032521431428.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
- `scatter price weight`：画散点图
<img src="https://img-blog.csdnimg.cn/20200325214404125.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
- `twoway (scatter price weight) (lfit price weight), title("散点图和线性拟合图")`
<img src="https://img-blog.csdnimg.cn/20200325214752748.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

---
# 五、简单的回归
## 1. reg函数解读
**regress 语法**：
<img src="https://img-blog.csdnimg.cn/20200330215448494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

**regress 命令运行结果的存储与调用**：
<img src="https://img-blog.csdnimg.cn/20200330215124568.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="80%">

## 2. 回归例子
```
* 线性回归
sysuse auto, clear
generate weight2 = weight^2
regress mpg weight weight2 foreign //简单的多元OLS回归,回归结果解读
```
<img src="https://img-blog.csdnimg.cn/20200327161831463.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">

```
* 不带常数项回归		  
regress mpg weight weight2 foreign,nocon //简单的多元OLS回归,回归结果解读
predict mpg   //计算被解释变量拟合值
predict e1, residual  //计算残差拟合值	
```
<img src="https://img-blog.csdnimg.cn/20200327161925361.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">


```
* 子样本回归
reg mpg weight weight2 foreign if price >=5000
```
<img src="https://img-blog.csdnimg.cn/20200327161954180.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "75%">


```
* 逐个剔除(Backward selection) 
stepwise, pr(0.2): reg price mpg weight displ
* 逐个加入(Forward selection) 
stepwise, pe(.2):  reg price mpg weight displ
```
<img src="https://img-blog.csdnimg.cn/20200330221122225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

---
# 六、面板数据和时间数据
- `tsset code year //时间序列声明(填充)`：Declare data to be time-series data
- `xtset code year //面板数据声明(填充)`：Declare data to be panel data

生成时间序列，自动按照后面的变量列表分组。

时间序列和面板数据的滞后项、前导项、差分、增长率：
```
xtset code year //行业代码 年度code year都要死数值型
gen lagsales = l.sales //滞后1期的滞后项 销售额
gen firsales = f.sales //前导1期的前导项
gen dsales = d.sales //差分
gen gsales = d.sales / l.sales //增长率
```
还有`l2.sales`，`f2.sales`……后面的数值可以改变，表示滞后/前导几期。`d2.sales`是二次差分。

<img src="https://img-blog.csdnimg.cn/20200331233119576.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">

## 时间序列数据
声明时间序列：`tsset`
检查是否有断点：`tsreport,   report`（要先声明才能再检验）
填充缺漏的日期：`tsfill`
追加样本：`tsappend,  add(#)`（#是追加的数目） 
## 面板数据
面板数据的声明：`xtset`
重复值的查验与删除
查验：`duplicates report	 stkcd year`
删除：`duplicates	drop	code	year,	force`（不要轻易使用，因为也是有规则的删除，一般是按照某个规律排序之后再删除）
面板资料的基本描述：`xtdes    xtpattern`
统计公司数目： `panels   code`
产生连续的公司代码：-egen- group( code )  （gen调用是分组，egen调用是产生连续代码）
处理为平行面板： xtbalance  平衡面板数据是说每个代码的公司有相同的年度，
例：xtbalance, range(2000  2008)（可以做面板数据）
例：xtbalance, range(2000  2008)  miss(size  lev  roa  ......)







 


行列对调
xpose ,clear


 




# 数据处理的结果输出数据浏览

# 数据显示与浏览：list / browse
查看数据结构
`describe  varlist`
`xtdescribe(面板数据）`
定义变量的显示格式：`format  varlist  str18 ( %18s  %-18s  %8.0g  %6.2f) ` 
变量标签：`label var`

# 基本统计量

查看基本的描述性统计：summarize  varlist, detail
查看变量格式、取值、缺失等信息：codebook  varlist
查看数值变量的分布：inspect  varlist
列表统计：table/tabulate
分类变量的频数分布：table  csrc_w
分类变量的频数分布：table  csrc_w  year 
连续变量的列表统计：table  csrc_w year , c(mean roa) 
格式设置：table  csrc_w year , c(mean roa)  f(%3.2f)  center row col

# 基本统计量：论文格式输出

命令：tabstat
tabstat  varlist, stats(n mean median sd min max)
tabstat  varlist, stats(n mean median sd min max)  by(var)
tabstat  varlist, stats(n mean sd min p25 p50 p75 max)
tabstat  varlist, stats(n mean median sd min max) c(s) f(%10.3f)
logout, save(tablename) excel replace: tabstat  varlist, stats(n mean sd min p25 p50 p75 max)  c(s) f(%10.3f)
# 相关系数表

corr   varlist
pwcorr   varlist, sig star(0.05)
论文格式输出
pwcorr_a   varlist
logout, save(tablename) excel replace: pwcorr_a   varlist
# 单变量分析1
变量的正态分布检验：swilk  varlist  p值小，拒绝正太分布
两变量的均值差异检验(t检验) ：ttest   var1=var2  配对样本（2个变量）
两独立样本的均值差异检验(t检验)：ttest   var, by(group)（一个变量）
两变量的秩和检验(wilcoxon检验)：signrank  var1=var2配对样本
两独立样本的秩和检验(wilcoxon检验) ：ranksum  var, by(group)
方差分析（F检验）：anova/oneway    var   group   （多样本常用）第一个变量是连续变量，第二个是分类变量
分类变量的差异检验(chi-sq检验)：tab  var_d  group, chi（对于分类变量的差异检验）

# 线性回归
reg y x1 x2 …
xi: reg y x1 x2 … i.year i.industry
xi: reg y x1 x2 c.x1#c.x2 … i.year i.industry
xi: reg y x1 x2 c.x1#c.x2 … i.year i.industry, cluster(code)（对于面板数据，会有聚集问题，会是t值虚增，处理异方差和自相关问题）调整有交融性
# 论文格式输出

outreg2 using file, bdec(3) sdec(2) excel replace  //默认输出系数与标准误
outreg2 using file, bdec(3) tdec(2) excel tstat replace  //输出系数与t值
outreg2 using file, bdec(3) pdec(2) excel pvalue replace  //输出系数与p值
outreg2 using file, bdec(3) excel beta replace  //输出系数与标准化系数
outreg2 using file, bdec(3) tdec(2) excel tstat append 
outreg2 using file, bdec(3) tdec(2) excel tstat append  addstat(F test, e(p), Adj R-squared, e(r2_a), F test, e(F))
outreg2 using $dz\result_1.xls,stats(coef tstat) aster(coef) bdec(3)tdec(2) adds(Pseudo R2,e(r2_a)) append


# 分组操作并搜集结果
statsby coef=_b[x1]   obs=e(N)  fstat=e(F), by(year ind) clear: reg y x1 x2 x3
statsby _b _se obs=e(N)  fstat=e(F), by(year ind) clear: reg y x1 x2 x3
statsby _b _se obs=e(N)  fstat=e(F), saving(file, replace) by(year ind): reg y x1 x2 x3
注：ereturn list、return list输出的标量都可以导出

# 检验与预测
test x1=1
test x1=x2
test x1=x2=x3
predict var, xb   因变量的估计值
predict var, residual
predict var if e(sample), residual  只有在回归的样本里才会预测残差，否则就是缺失值
margins, at((mean) _all (p25) age)  // age取25%分位数、其他解释变量取均值时的因变量预测值
margins, at( (p25) age)   atmeans  // age取25%分位数、其他解释变量取均值时的因变量预测值


# Probit/Logit回归
xi: probit y x1 x2 … i.year i.industry, cluster(code)
xi: logit y x1 x2 … i.year i.industry, cluster(code)
输出：outreg2 using file, bdec(3) tdec(2) excel tstat append  addstat(Log pseudolikelihood, e(ll), Pseudo R-squared, e(r2_p), chi2 test, e(p), chi2 test, e(chi2))
第一次是replace,第二次以后是append

# 边际效应

dprobit2 / dlogit2  y  x1  x2  x3  //解释变量取平均值时的边际效应
dprobit2 / dlogit2  y  x1  x2  x3, at(x1==1)  //满足x1==1的子样本中，解释变量取平均值时的边际效应（注：此时依然为全部样本回归；样本中必须有x1=1的观测）
mfx
mfx, at(median)
mfx, at(mean x1=1)  // x1=1、其他变量取均值时的全部样本边际效应（注：样本中无须存在x1=1的观测）x1取1.其他变量取均值的时候的边际效应，跟上面那个不一样
margins, dydx(*) at((mean) _all) //所有解释变量取均值时的边际效应
margins, dydx(*) at((mean)  _all age=30) //所有解释变量取均值时的边际效应（注：等同于mfx, at(age=30)）
margins, dydx(age income) at((mean) _all) //所有解释变量取均值时变量age、income的边际效应

# 虚拟变量的边际效应

虚拟变量的边际效应比较特殊
margins求得的边际效应不是虚拟变量从0到1变动导致的目标概率变化  0.5   取均值时的边际效应
mfx、dprobit自动报告虚拟变量各取值的概率预测之差，是从0到1导致的因变量概率的变化，而dprobit2、margins依然自动报告均值时的边际效应（除非专门设定）

# 第五讲：其他回归模型的命令

（1）排序模型
ologit ,oprobit
（2）泊松回归
Possion
（3）tobit回归
Tobit,ll() ul()

