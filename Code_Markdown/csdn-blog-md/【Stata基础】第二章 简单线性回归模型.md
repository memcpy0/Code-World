@[toc]

# 一、练习
* 列出价格大于6000的国产汽车的价格
	```
	sysuse auto, clear
	list price if price > 6000 & foreign == 0
	```
* 给出1978年维修记录少于3次或产地为国外的汽车价格和重量的描述性统计信息
    ```
    sum price weight if (rep78 < 3 | foreign == 1)
    ```
* 本数据中有多少辆国产汽车价格大于6000？
    ```
    count if price > 6000 & foreign == 0
    ```
* 列出 price wei len mpg turn foreign 变量的均值，标准差，中位数，最大值，最小值（对于这种要求，**将一系列统计量以一张表格的形式展现出来更合适**）
   ```
   tabstat price wei len mpg turn foreign, stat(mean sd median max min)
   ```
* 按国产和非国产为标准分类对price wei 进行描述性统计
	```
	by foreign : sum price wei
	```
* 将数据按照价格进行升序和降序排列
	```
	gsort price
	gsort -price
	```
* 删除1978年维修记录缺失的数据
	```
	drop if rep78 == .
	```
* 将字符型变量make转换成数值型变量cenmake
	```
	encode make, gen(cenmake)
	```
* 将price 转化成字符型变量
  ```
  tostring price, replace
  ```
* 将price 转化成数值型变量
  ```
  destring price, replace
  ```
* 生成一个虚拟变量，其中价格大于6000为1，否则表示为0
	```
	gen vari = 1 if price > 6000
	replace vari = 0 if price <= 6000
	//这样比较麻烦，可以写成如下语句
	gen dum_price = price > 6000
	```
* 给出price wei len mpg 相关系数矩阵
  ```
  pwcorr price wei len mpg
  ```
* 画出price wei len mpg 的相关系数矩阵散点图（**graph是最需要掌握的画图命令**）
   ```
   graph matrix price wei len mpg
   ```
* 画出price频率直方图
   ```
   histogram price, frequency
   ```
* 画出price和weight的散点图
    ```
    scatter price weight
    ```
* 给price 和weight散点图加上price和wei的拟合线
	```
	twoway (scatter price weight) (lfit price weight)
	```

---

# 二、线性回归模型
* 做一个简单的OLS回归，其中被解释变量为price,解释变量为weight foreign 

## 1. 初步探索
```
cd D:\Stata_Projects  //进入存储数据的工作目录
use "food.dta", clear  //使用这个数据集
```
<img src="https://img-blog.csdnimg.cn/20200327125842817.png" width="60%">

```
* 得到数据之后，一般先浏览，检查数据，并看变量的描述性统计信息
* 画出food_exp与income的散点图，并保存
twoway (scatter food_exp income)
* 保存图片, 如果有了就替代
graph save food1, replace 
* 一步到位
twoway (scatter food_exp income), saving(food1, replace) 
```
<img src="https://img-blog.csdnimg.cn/20200327130101624.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
* 更丰富的画图功能, 使用help graph就可以查看到
twoway (scatter food_exp income) ///  /* 基本语句，画什么的散点图 */
	   (lfit food_exp income), ///    /*拟合线*/
       ylabel(0(100)600)         ///  /* Y轴的刻度从0到600，间隔为100 */           
	   xlabel(0(5)35)            ///  /* X轴刻度0-35，间隔5 */
	   title(食物消费数据)   /// /* 图片名字 */
	   xtitle(每周收入)   ///
       ytitle(每周家庭食物花费)
* 还有更多的画图内容可以学习，图片的保存等也可以直接点击图片操作
```

<img src="https://img-blog.csdnimg.cn/20200327130706248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">
  
<img src="https://img-blog.csdnimg.cn/202003271309298.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

 
## 2. 简单OLS回归(最小二乘法)
```
* food_ex = coef * income + ... 对income进行回归
regress food_exp income
```
<img src="https://img-blog.csdnimg.cn/2020032713131595.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "82%">

```
* 计算y的拟合值，这里的y是food_exp'; xb是计算线性拟合值
predict yhat, xb  //calculate linear prediction
* 计算残差项的拟合值
predict ehat, residuals 
browse
```
<img src="https://img-blog.csdnimg.cn/20200327131757596.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "50%">

<img src="https://img-blog.csdnimg.cn/20200327131819558.png" width = "50%">

```
* 做预测，预测其他的收入情况在该模型中的食物支出
* 例如，预测家庭每周收入2000$的家庭，每周食物支出
set obs 41 //本数据有40个观测值，我们多加一个观测值，但是没有定义具体观测值内容，它目前是缺失
replace income = 20 in 41 //将收入第41个观测值替换成20百
predict yhat1, xb
list yhat1 in 41
```
<img src="https://img-blog.csdnimg.cn/20200327132201903.png" width = "60%">

```
* 保存dta文件
* 直接点save菜单，或者输入下面命令
save "newfood.dta", replace
```

---
# 三、二次式模型
$y=\beta _1+ \beta_2*x^2 + e$
```
* 二次式模型
use "br.dta",clear  
sum //sum, detail
```
<img src="https://img-blog.csdnimg.cn/20200327133616241.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width = "75%">

```
desc
```
<img src="https://img-blog.csdnimg.cn/20200327133746574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="85%">

```
gen sqft2 =sqft^2 //生成变量
regress price sqft2  //二次模型
predict priceq, xb
br price sqft sqft2 priceq
```
<img src="https://img-blog.csdnimg.cn/20200327134100632.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">


```
* 画出拟合曲线
twoway (scatter price sqft) 	   ///     	/* 基本的点 */
      (lfit price sqft,            ///		/* 拟合线 */
       sort lwidth(medthick))	   /*设定线的厚度 */
graph save br_quad1, replace       //保存图片 

twoway (scatter price sqft) 	  ///     	/* 基本的点 */
       (line price sqft,          ///		/* 折线 */
       sort lwidth(medthick))	  /*设定线的厚度 */
graph save br_quad2, replace      //保存图片	
	
twoway (scatter price sqft) 	  ///     	/* 基本的点 */
       (line priceq sqft,         ///		/* 折线 */
       sort lwidth(medthick))     /*设定线的厚度 */
graph save br_quad3, replace      //保存图片	
graph combine "br_quad1" "br_quad2" "br_quad3"
*	比较三个图片区别	
```
 <img src="https://img-blog.csdnimg.cn/20200327140355171.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="79%">

--- 
# 四、对数线性模型
$lny=\beta_1 + \beta_2x+e$
```
histogram price
```
<img src="https://img-blog.csdnimg.cn/20200327141811769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
graph save price, replace 
	
gen lnprice=ln(price)        //price 存在一些值特别大,尾巴很长,为了把数据平滑一点,取对数
histogram lnprice,percent    //更接近正态分布
```

<img src="https://img-blog.csdnimg.cn/2020032714200512.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
reg lnprice sqft    //对数线性模型回归
predict lpricef, xb //得到对数形式的拟合值
```
<img src="https://img-blog.csdnimg.cn/20200327142210540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
* 画出拟合曲线
generate pricef = exp(lpricef)
twoway (scatter price sqft)  
       (line pricef sqft, sort lwidth(medthick))
graph save br_loglin, replace
```
<img src="https://img-blog.csdnimg.cn/20200327142510799.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">


---
# 五、指示变量回归
首先进一步学习创建**虚拟变量/factor variables/指示变量**：
* 方法1. 设定**是否满足某个条件来生成**，条件为真，取值 $1$，否则取 $0$：
  ```
  gen bh = (bedrooms>4) // 卧室数量大于四为大房子，取值为1 
  ```
  <img src="https://img-blog.csdnimg.cn/2020032714411193.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="40%">
  
* 方法2. $tabulate$ 创建指示变量。让**每个可能的 $baths$ 取值都设定一个独立的虚拟变量**：
  ```
  tabulate region, gen(reg)  // Create indicator variables for region, called reg1, reg2, ...
  tabulate baths, gen(bath)  // 为每个baths取值计数，gen(bath)就会产生5个指示变量bath1-5
  ```
  <img src="https://img-blog.csdnimg.cn/20200327144227960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="60%">
  <img src="https://img-blog.csdnimg.cn/20200327144735473.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
 

---
# 六、练习
```
use "utown.dta", clear
describe  //拿到数据的规定动作，了解数据内容和描述性统计信息
sum  
```
<img src="https://img-blog.csdnimg.cn/20200327144955648.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">

```
* 按照房子是否位于大学附近进行分类画图	
histogram price if utown==0, width(12) start(130) percent  ///
          xtitle(House prices ($1000) in Golden Oaks)      ///
	      xlabel(130(24)350) legend(off)
graph save utown_0, replace
```
<img src="https://img-blog.csdnimg.cn/20200327145354222.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
histogram price if utown==1, width(12) start(130) percent  ///
          xtitle(House prices ($1000) in University Town)  ///
	      xlabel(130(24)350) legend(off)
graph save utown_1, replace
graph combine "utown_0" "utown_1", col(1) iscale(1) 
*把两张图放在一张图上，col(#)分成几列显示，iscale()文本和标记的大小
graph save combined ,replace
```
<img src="https://img-blog.csdnimg.cn/20200327154607166.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

<img src="https://img-blog.csdnimg.cn/20200327154749268.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

`histogram`每一个语句的意思需要好好理解，例如：
- `histogram price if utown==0`：意思是做房屋**不位于**大学附近的房子的价格的直方图；
 - `width(12)`是柱子宽度为 $12$ 个刻度；
 - `start(130)`价格从 $130$ 开始画（price最小值为 $134$，这样x轴不会从 $0$ 开始）
- `percent`表示**纵轴为占比**；
- `xlabel`表示 $x$ 轴刻度从 $130$ 到 $350$，单位刻度(即间隔刻度)为 $25$；
- `legend(off)`表示不显示图例

```
label define utownlabel 0 "Golden Oaks" 1 "University Town"  //定义utown值的label
label value utown utownlabel //将utown的label也按照值的label定义
```
* `label variable varname` 给变量定标签
* `label define labelname` 给变量的值定义标签
* `label value varlist(一列变量名) labelname`  将取值的标签定义给变量
 
 
 ```
 * by(utown, cols(1))  按照utown分组画图	
histogram price, by(utown, cols(2))  			 
           start(130) percent                	 
           xtitle(House prices ($1000))     		 
           xlabel(130(24)350) legend(off)
graph save combined2, replace
```

<img src="https://img-blog.csdnimg.cn/20200327155521485.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

* 检验独立社区房价的描述性统计信息
	`sum price if utown == 0`
	`sum price if utown == 1	`
  <img src="https://img-blog.csdnimg.cn/20200327155808751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">
* 复习，用 `bysort` 分组统计
	`bysort utown:sum price 	`
	<img src="https://img-blog.csdnimg.cn/20200327160120777.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">
* 另一种写法
	`by utown,sort:sum price	`
 <img src="https://img-blog.csdnimg.cn/20200327160131708.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="75%">
 
> $by$ and $bysort$ are **really the same command**; $bysort$ is just $by$ with the $sort$ option.

