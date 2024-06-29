sim <- function(nreps) {
  commdata <- list() 
  commdata$countabsamecomm <- 0
  for (rep in 1:nreps) {
    commdata$whosleft <- 1:20
    commdata$numabchosen <- 0
    commdata <- choosecomm(commdata, 5)
  }
}

# quicksort() 
quicksort <- function(x) {
  if (length(x) == 1) return x
  pivot <- x[1]
  
}

运算符

描述

x + y


加法

x - y


减法

x * y


乘法

x / y


除法

x ^ y


乘幂

x %% y


模运算

x %/% y  


整数除法

x == y


判断是否相等

x <= y


判断是否小于等于

x >= y


判断是否大于等于

x && y


标量的逻辑“与”运算

x || y


标量的逻辑“或”运算

x & y


向量的逻辑“与”运算(x、y以及运算结果都是向量)                                    

X | y


向量的逻辑“或”运算(x、y以及运算结果都是向量)

!x 


逻辑非