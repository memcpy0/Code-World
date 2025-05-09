# 3.1 创建矩阵
### 矩阵的行和列下标都从1开始, 如a[1, 1], 在R中按列存储, 默认按列展示
y <- matrix(c(1, 2, 3, 4), nrow = 2, ncol = 2) 
y
y <- matrix(c(1, 2, 3, 4), ncol = 2)
y
y[,2]  # 第几行 第几列
y <- matrix(nrow = 2, ncol = 2)
y[1, 1] <- 1
y[2, 1] <- 2
y[1, 2] <- 3
y[2, 2] <- 4
y
m <- matrix(c(1, 2, 3, 4, 5, 6), nrow = 2, byrow = T) # 使元素按行排列, 但矩阵本身是按照列存储的
m

# 3.2 一般矩阵操作
## 3.2.1 线性代数操作
y %*% y   # 矩阵相乘
y * 3     # 矩阵数量乘法
y + y     # 矩阵加法
## 3.2.2 矩阵元素索引  矩阵[向量1, 向量2], 两个向量可以选择性省略
z = matrix(c(1:4, rep(c(1, 0), each=2), rep(c(1, 0), 2)), nrow = 4)
z
z[, 2:3]  # 2到3列组成一个子矩阵
z[2:3,]   # 2到3行组成一个子矩阵
z[2:3, 2] # 2到3行的第二列组成一个子矩阵
y <- matrix(1:6, ncol = 2)
y
y[c(1, 3),] <- matrix(c(1, 1, 8, 12), nrow = 2)
y
x <- matrix(nrow=3, ncol=3)
y <- matrix(c(4, 5, 2, 3), nrow = 2)
x[2:3, 2:3] <- y
x
y <- matrix(1:6, ncol = 2)
y
y[-2,]   # 取出矩阵y中除第二行外的所有行

## 3.2.4 矩阵元素筛选




























