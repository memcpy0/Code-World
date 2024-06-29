# 2.1 标量、向量、数组、矩阵
## 2.1.1 不能添加或删除向量元素 只能重新赋值
x <- c(88, 5, 12, 12)
x <- c(x[1:3], 168, x[4]) # `insert` 168 before 13
x
## 2.1.2 获取向量长度
x <- c(1, 2, 4)
length(x)
first <- function(x) {
  for (i in 1:length(x)) 
    if (x[i] == 1) break
  return(i)
}
x <- c()
x
length(x)
1:length(x)  # length()可能为0
## 2.1.3 矩阵和数组都是向量, 只不过它们有额外的类属性
m <- matrix(c(1, 2, 3, 4), nrow = 2, ncol = 2, byrow = TRUE)
m + 10:13

# 2.2 R无需声明变量 
y <- vector(length=2) # mode="logical" 数据类型都作为mode
y[1] <- 5
y[2] <- 12
y <- c(5, 12)  
x <- c(1, 5)
x 
x <- "abc"  # 由于变量事先没有声明, 它们的类型是不受限制的

# 2.3 循环补齐 两个向量使用运算符时, 如需具有相同长度, R会自动循环补齐
c(1, 2, 4) + c(6, 0, 9, 20, 22)  # 运算中较短的向量被循环补齐
x = matrix(c(1, 2, 3, 4, 5, 6), nrow = 3) # 矩阵在R中一列列的存储
x
x + c(1, 2)

# 2.4 常用的向量运算
## 2.4.1 向量运算和逻辑运算
2+3        # 标量实际上是一元向量, 向量可以相加
"+"(2, 3)  # R一种函数语言, 每个运算符包括+都是函数
x <- c(1, 2, 4)
x + c(5, 0, -1) # +算子按元素逐一进行运算
x * c(5, 0, -1) # *算子按元素逐一相乘
x / c(5, 4, -1)
x %% c(5, 4, -1)
## 2.4.2 向量索引 使用它选择给定向量中特定索引的元素构成子向量
y <- c(1.2, 3.9, 0.4, 0.12)
y[c(1, 3)]  # 索引向量格式: vector1[vector2] 返回向量1中索引在向量2的那些元素
y[2:3]
v <- 3:4
y[v]      
x <- c(4,2, 17, 5)
y <- x[c(1, 1, 3)]  # 可以重复选取元素
y
z <- c(5, 12, 13)
z[-1]               # 使用-索引向量代表剔除相应元素
z[-1:-2]
z[-1:-3]
z <- c(5, 12, 13)
z[-1:-length(z)]    # 使用length排除所有的元素
z[-length(z)]       # 选择除最后一个元素外的所有元素
## 2.4.3 用:运算符创建向量
5:8                 # :生成指定范围内数值构成的向量
5:1
i <- 2              
1:i-1               # :的优先级高于-
1:(i-1)             # ()优先级更高  # ?Syntax看到所有运算符优先级
## 2.4.4 使用seq()创建向量 即生成等差序列 seq(from, to, by)
seq(from=12, to=30, by=3)       # 生成间隔为3的向量
seq(from=1.1, to=2, length=10)  # 生成长度为10的向量 间隔可以不为整数
# for (i in 1:length(x)) 
# 空向量问题可以这么写 for (i in seq(x)) x为空不做任何迭代 
x <- c(5, 12, 13)
x
seq(x)              # x非空 seq(x) equals to seq_along(x)
x <- NULL
x 
seq(x)              # x空, seq(x)正确计算出空值
seq_along(x)        # 所以, seq(x)等价于seq_along(x)
## 2.4.5 使用rep()重复向量常数 rep=(x, times, each)
rep(8, 4)
rep(c(5, 12, 13), 3)
rep(1:3, 2)
rep(c(5, 12, 13), each=2)  # each指定交替重复的次数

# 2.5 使用all()和any()
x <- 1:10
any(x > 8)          # any()报告其参数是否至少有一个为TRUE
any(x > 88)
all(x > 88)         # all()报告其参数是否全部为TRUE
all(x > 0)
## 2.5.1 寻找连续出现1的序列
findruns <- function(x, k) {}

# 2.6 向量化运算符 vectorize 应用到向量上的函数实际上应用到每个元素上
## 2.6.1 向量输入, 向量输出
u <- c(5, 2, 8)
v <- c(1, 3, 9)
u > v  # 这里>函数分别运用到u[i]和v[i] 实际上是做了向量化
       # 如果一个函数使用了向量化的运算符, 它也被向量化, 使速度提升成为可能
w <- function(x) return(x+1)  # w()使用向量化的+(), 从而w()也是向量化的
w(u)
sqrt(1:9) # 平方根 对数 三角函数等也是向量化的
y <- c(1.2, 2.1, 3.9, 0.4)
round(y)
round(1.2) # 标量实际上是一元向量, 这里只是一种特殊情形
y <- c(12, 5, 13)
y + 4      # 循环补齐也发挥了作用
"+"(y, 4)  # +等运算符实际上也是函数
f <- function(x, c) return((x+c)^2)
f
f(1:3, 0)
f(1:3, 1)
f(1:3, 1:3)
f <- function(x, c) {
  if (length(c) != 1) stop("vector c not allowed")  # 确实想把c限制为标量,要判断
  return((x+c)^2)
}
## 2.6.2 向量输入, 矩阵输出
z12 <- function(z) return(c(z, z^2))
x <- 1:8
z12(x)
matrix(z12(x), ncol=2)
sapply(1:8, z12)   # simplify apply sapply(x, f)对每一个元素使用f(), 结果转换为矩阵

# 2.7 NA与NULL值 缺失值在R中表示为NA, NULL代表不存在的值, 而不是存在但未知的值
## 2.7.1 NA的使用
x <- c(88, NA, 12, 168, 13)
x 
mean(x)               # 存在NA缺失值, 导致mean()无法计算均值
mean(x, na.rm = T)    # 跳过NA, 计算其余元素的均值
x <- c(88, NULL, 12, 168, 13)
mean(x)               # R会自动跳过空值NULL, 因为其不存在
x <- c(5, NA, 12)
mode(x[1])
mode(x[2])
y <- c("abc", NA, "def")
mode(y[2])
mode(y[3])
## 2.7.2 NULL的使用
z <- NULL             # 其一是在循环中创建向量 不然会被刷新
for (i in 1:10) if (i %% 2 == 0) z <- c(z, i)
z
seq(2, 10, 2)
2*1:5 
z <- NA               # 会得到多余的NA, 这反应了NA和NULL的区别
for (i in 1:10) if (i %% 2 == 0) z <- c(z, i) 
z
length(NULL)          # NULL作为不存在而计数, 是R的一种特殊对象, 没有mode
length(NA)            # NA作为存在计算

# 2.8 筛选 提取向量中满足一定条件的元素
## 2.8.1 生成筛选索引
z <- c(5, 2, -3, 8) 
w <- z[z*z > 8]
w
z
z*z > 8   # 向量 向量运算符 循环补齐 is equal to ">"(z*z, 8) 对向量使用函数, 是向量化的另一个例子
">"(2, 1) # >实际上是一个函数
">"(2, 5)
z[c(TRUE, FALSE, TRUE, TRUE)]
j <- z*z > 8
j
y <- c(1, 2, 30, 5)
y[j]
z <- c(5, 2, -3, 8)
y <- c(1, 2, 30, 5)
y[z*z > 8]
x <- c(1, 3, 8, 2, 20)
x[x > 3] <- 0
x
## 2.8.2 使用subset()函数筛选
x <- c(6, 1:3, NA, 12)
x
x[x > 5]  # NA是未知但存在的
subset(x, x>5) # 会自动剔除NA
## 2.8.3 选择函数which() 找到满足条件的元素的位置而不是提取该元素
z <- c(5, 2, -3, 8)
which(z*z > 8)
z[which(z*z > 8)]
first1 <- function(x) return(which(x == 1)[1])

# 2.9 向量化的ifelse()函数 ifelse(b, u, v) b[i]为TRUE时, 返回值的第i个元素为u[i], 不然为v[i]
x <- 1:10
y <- ifelse(x %% 2 == 0, 5, 12)  # 5 12都进行了循环补齐
y
x <- c(5, 2, 9, 12)
ifelse(x > 6, 2*x, 3*x)

# 2.10 测试向量相等 
x <- 1:3
y <- c(1, 3, 4)
x == y   # ==是一个函数 注:R语言中向量化是核心的内容, 明白了向量化就掌握了R的基础
"=="(3, 2)
i <- 2
"=="(i, 2)
x <- 1:3
y <- c(1, 3, 4)
all(x == y)
identical(x, y) # 比较两个对象是否完全一样, 包括mode
x <- 1:2        # 产生的是integer
y <- c(1, 2)    # 产生的默认是double
x
y
identical(x, y)
typeof(x)
typeof(y)

# 2.11 向量元素的名称 可以随便指定向量元素的名称
x <- c(1, 2, 4) 
names(x)                      # 用names()询问向量元素的名称
names(x) <- c("a", "b", "c")  # 用names()给向量元素命名
names(x)
x
names(x) <- NULL              # 把向量元素名称赋值为NULL, 将其移除
x
x <- c(1, 2, 4)
names(x) <- c("a", "b", "ab") 
x["b"]                        # 可以用名称引用向量里的元素

# 2.12 关于连接函数c()的更多内容
c(5, 2, "abc")                # 传递到c()中的参数类型不同, 它们将被降级为同一可最大保留它们共同特性的类型
c(5, 2, list(a=1, b=4))       
c(5, 2, c(1, 5, 6))           # c()对向量有扁平化的效果
