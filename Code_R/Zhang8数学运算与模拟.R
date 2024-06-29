# 使用近似正态分布计算均值两端的置信区间
mean_ci <- function(x, conf = 0.95) {
  se <- sd(x) / sqrt(length(x))
  alpha <- 1 - conf
  mean(x) + se * qnorm(c(alpha / 2, 1 - alpha / 2))
}
# 计算加权摘要统计量
wt_mean <- function(x, w) {
  sum (x * w) / sum(x)
}
wt_var <- function(x, w) {
  mu <- wt_mean(x, w)
  sum(w * (x - mu) ^ 2) / sum(w)
}
wt_st <- function(x, w) {
  sqrt(wt_var(x, w))
}

# 连乘函数
exaltlyOne <- function(p) {
  notp <- 1 - p  # 生成一个新的概率向量, 对立事件的值1-p
  tot <- 0.0
  for (i in 1:length(p)) 
    tot <- tot + p[i] * prod(notp[-1])
  return(tot)
}
# 累积和与累积乘函数
x <- c(12, 5, 13)
cumsum(x)
cumprod(x)
# 最大值、最小值和向量最大、最小 
z <- c(1, 5, 6)
y <- c(2, 3, 2)
min(z, y)
pmin(z, y)
max(z, y)
pmax(z, y)
# 函数最大、最小
nlm(function(x) return(x^2-sin(x)), 8)$minimum






