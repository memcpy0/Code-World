@[toc]

# 一、复习
```
sysuse auto,clear
```
* 首先看一下 `auto` 中每个变量的描述性统计信息：
  ```
  sum make price mpg rep78 headroom trunk weight length turn displacement gear_ratio foreign
  ```

* 画出汽车价格和重量的散点图，加上拟合线、xlabel、ylabel、title并保存
  ```
  tabstat price weight, stat(max min median)
  twoway (scatter price weight) (lfit price weight), xlabel(1600(400)5000) ylabel(3000(2000)16000)
  graph save pwscatter, replace
  ```
  <img src="https://img-blog.csdnimg.cn/20200327170026490.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">
* 做汽车价格和重量的一元OLS回归并计算价格和残差的拟合值
   ```
   reg price weight
   predict priceHat, xb
   predict e, residuals
   br price priceHat e
   ```
   <img src="https://img-blog.csdnimg.cn/202003271703134.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="50%">
* 创建一个虚拟变量，使得价格大于6000的时候取值1，价格不大于6000时取0
	```
	gen x = price > 6000
	```
* 做出 $price=\beta_1+\beta_2*weight^2+e$ 的回归，算出**价格拟合值**并**做拟合曲线**
   ```
   gen weight2 = weight^2
   reg price weight2  //二次模型
   predict priceq, xb //价格拟合值
   graph twoway (scatter price weight) (line priceq weight, sort lwidth(medthick))
   ```
   <img src="https://img-blog.csdnimg.cn/20200327171253116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">
* 做出 $ln(price)=\beta_1+\beta_2*weight+e$ 的回归，算出**价格拟合值**并**做拟合曲线**
  ```
  gen lnprice = ln(price)
  reg lnprice weight
  predict lnpriceHat, xb
  gen priceHat = exp(lnpriceHat)
  graph twoway (scatter price weight) (line priceHat weight, sort lwidth(medthick))
  ```
  <img src="https://img-blog.csdnimg.cn/20200327171715857.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

# 二、区间估计
## 1. scalar介绍
```
help scalar 
```
<img src="https://img-blog.csdnimg.cn/20200327172554403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70" width="70%">

```
scalar a = 100   //在程序中生成一个变量a，只存储100这个数据
//生成标量变量tc975，只储存自由度N-2=38, 且满足95%估计区间的上尾t分布概率值0.025
scalar define tc975 = invttail(38, 0.025)
scalar tc975 = invttail(38, 0.025) 
//inv表明取“逆”，invttail(38，0.025) 就是满足95%估计区间上尾分布概率值0.025，取逆，得到临界值
di "t分布97.5%的概率上的临界值 = " tc975  //2.0243942
```
## 2. 创建一个区间估计
```
use food, clear
reg food_exp income
scalar ub2 = _b[income] + tc975 * _se[income]
scalar lb2 = _b[income] - tc975 * _se[income]
di "income系数估计值的95%的区间估计为：" lb2 ", "ub2
```
<img src="https://img-blog.csdnimg.cn/20200327195404376.png" width="70%">


## 3. 假设检验
用stata算出t统计量的检验统计值，临界值和p值：

* 右侧显著性检验
* 原假设 $H_0: \beta_2=0$，备择假设 $H_1：\beta_2 > 0$
```
scalar tstat0 = _b[income]/_se[income]           //t统计值和临界值
di "Ho: beta2=0 的t统计值= " tstat0               //在回归结果表的t值处  //4.8773806
di "t(38) 95th percentile = " invttail(38,0.05)   //1.6859545
//38为自由度，因为有40个数据，自由度为N-2，0.05为第（1-0.05）百分位上的临界值

//临界值来自t分布的右侧，可以使用invttail找到这个临界值,如下
scalar define tc95 = invttail(38, 0.05) 
di tc95     //1.6859545
//tstat0显著大于tc95
```
---
经济假设的右侧检验：
* 为检验原假设 $H_0: \beta_2 \le 5.5$ 和备择假设 $H_1：\beta_2 > 5.5$：
* 我们再次计算这个检验统计量和 $0.01$ 右侧临界值：
```
scalar tstat1 = (_b[income]-5.5) / _se[income]  //2.2499045
di "t-statistic for H0: beta2 = 5.5 is " tstat1 
di "t(38) 99th percentile = " invttail(38,0.01) //2.4285676
//在0.01显著性水平上（原假设为真时，拒绝原假设的概率）不能拒绝原假设
di "p-value for right-tail test Ho:beta2 = 5.5 is " ttail(38,tstat1) //.01516329
```

```
* using lincom for calculation
lincom income-5.5  
```
<img src="https://img-blog.csdnimg.cn/20200327200505182.png" width= "80%">

* 表格中 $coef.=\beta_2-5.5$
* 假设检验中 $p$ 值原则：当 $p$ 值小于或等于显著性水平 $\alpha$ 时，拒绝原假设
* 即 $p \le alpha$，拒绝 $H_0$；$p > \alpha$，不能拒绝 $H_0$
* 求 $p$ 值，$ttail$ 函数
 
---
经济假设的左侧检验：
* 原假设为 $H_0:\beta_2 \ge 15$，备择假设 $H_1<15$。
```
scalar tstat2 = (_b[income]-15) / _se[income]
di "t-statistic for H0: beta2 = 15 is " tstat2  //-2.2884634
di "t(38) 5th percentile = " invttail(38,0.95)  //拒绝原假设 -1.6859545
di "p-value for left-tail test H0:beta2 = 15 is " 1-ttail(38,tstat2)  //.01388071
lincom income-15   //双侧检验
```
<img src="https://img-blog.csdnimg.cn/20200327211439757.png" width="75%">

---
经济假设的双侧检验
* **除了临界值的计算，双侧检验与单侧检验具有相同的结构。**
* 例如对于一个显著性水平为 $0.05$ 的检验，t分布的临界值为 $2.5\%$ 和 $97.5\%$。
* 双侧检验原假设 $H_0:\beta_2 = 7.5$，备择假设 $H_1 \ne 7.5$
```
scalar tstat3 = (_b[income]-7.5)/_se[income]
di "t-statistic for Ho: beta2 = 7.5 is " tstat3        //1.2944586
di "t(38) 97.5th percentile = " invttail(38,0.025) //2.0243942
di "t(38) 2.5th percentile = " invttail(38,0.975)  //-2.0243942
di "p-value for ho:beta2 = 7.5 is " 2*ttail(38,abs(tstat3))  //.20331828
lincom income-7.5   //t统计值，比左侧，右侧的临界值绝对值都小
```

<img src="https://img-blog.csdnimg.cn/2020032721162259.png" width = "75%">

---
检验并估计参数的线性组合：
* 一个更为普遍的线性假设包括两个参数，形式可能为 $H_0: c_1\beta_1+c_2\beta_2=c_0$
* $t=[(c_1b_1+c_2b_2)-c_0] / se(c_1b_1+c_2b_2)$ 

* 可以利用lincom命令
* $H_0:\beta_1+20\beta_2$：	
	```
	lincom _cons + income*20
	```
* $H_0:\beta_1+20\beta_2-250$
  ```	
  lincom _cons + income*20 - 250
  ```
