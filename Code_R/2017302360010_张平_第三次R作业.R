# 1
rescale01 <- function(x) {
  rng <- range(x, na.rm = FALSE)
  (x - rng[1]) / (rng[2] - rng[1])
}
##### 因为NA是存在但不确定的数 
##### 不跳过会使得范围内最大值和最小值不确定
rescale01(c(NA, 1, 2, 4))

# 2
my_rescale01 <- function(x) {
  x[x == -Inf] <- 0
  x[x == Inf] <- 1
  rng <- range(x, na.rm = TRUE, finite = TRUE)
  (x - rng[1]) / (rng[2] - rng[1])
}
my_rescale01(c(-Inf, 2, Inf))

# 5
both_na <- function(x, y) 
  return(ifelse(length(x) != length(y), 0, sum(pmin(is.na(x), is.na(y)))))
x <- c(NA, 1, 0, NA, 3, NA)
y <- c(NA, NA, 5, 6, NA, NA)
both_na(x, y)

both_na2 <- function(x, y) 
  return(ifelse(length(x) != length(y), 0, sum(is.na(x) & is.na(y))))
both_na2(x, y)

both_na3 <- function(x, y) {
  if (length(x) != length(y)) return(0)
  sum1 <- 0
  for (i in seq_along(x))
    if (is.na(x[i]) && is.na(y[i])) sum1 <- sum1 + 1
  return(sum1)
}
both_na3(x, y)

# (3).a. Alice the Camel
para1 <- "Alice the camel has " 
para2 <- " humps,"
repeat_str <- c("five", "four", "three", "two", "one", "no")
next_sequence <- "So ride, Alice, ride\nboom, boom, boom\n"
last_sequence <- "So Alice the camel is a horse, of course!"
ballad <- NULL
for (i in repeat_str) {
  for (j in 1:3) 
    ballad <- c(ballad, paste(para1, para2, sep=i))
  if (i == "no") ballad <- c(ballad, last_sequence)
  else ballad <- c(ballad, next_sequence)
}
cat(paste(ballad, collapse = "\n"))

# (3).b. Ten in the Bed
ballad_in_somewhere <- function(x, bedding="bed") {
  if (length(x) == 1)  x <- x:1
  repeat_sentence1 <- "There were NUM in the bed and the little one said"
  repeat_sentence2 <- "\"Roll over, roll over\"\nSo they all rolled over and one fell out\n"
  ballad <- NULL
  for (i in x) {
    new_1 <- gsub("bed", bedding, gsub("NUM", as.character(i), repeat_sentence1))
    if (i == 1) ballad <- c(ballad, new_1, "\"Good night\"")
    else ballad <- c(ballad, new_1, repeat_sentence2)
  } 
  cat(paste(ballad, collapse = "\n"))
}
ballad_in_somewhere(9, "mat")

# (3).c. Ten in the Bed
bottles_on_the_wall <- function(x, container="bottles", liquid="beer", surface="wall") {
  if (length(x) == 1)  x <- x:1
  sentence1 <- paste("NUM", container, "of", liquid, "on the", surface)
  sentence2 <- paste("NUM", container, "of", liquid)
  sen <- paste(sentence1, sentence2, sep = ", ")
  sentence3 <- paste("\n \bTake one down and pass it around,", "NUM", 
                     container, "of", liquid, "on the", surface,  "\n\n")
  for (quant in x) {
    if (quant > 1)
      cat(paste(gsub("NUM", as.character(quant), sen), 
                gsub("NUM", as.character(quant-1), sentence3)))
    else {
      suffix = paste("no more", liquid, "on the", surface)
      cat(paste(gsub("NUM", as.character(quant), sen), 
                "\n \bTake one down and pass it around,",
                suffix))
    }
  }
}
bottles_on_the_wall(5, container = "cans", liquid = "water", surface = "ground")

# (4) 
# 循环运算前，记得预先设置好数据结构和输出变量的长度和类型，不宜在循环过程中渐进性地增加数据长度
# 因为这样做，在连接已有的向量和新向量, 并赋值给输出变量时会耗费大量的时间
# 好的解决办法是将其存放在一个指出实际所需长度长度的数据结构中:
test_func1 <- function() {
  out <- vector("double", length=100000)
  for (i in 1:100000) 
    out[i] <- rnorm(1, i)
}
system.time(test_func1())

test_func2 <- function() {
  output <- double()
  for (i in 1:100000) 
    output <- c(output, rnorm(1, i))
}
system.time(test_func2())
# 第二种方法用时更长, 效率更低, 
# 当然，如果不清楚要输出向量的长度，还是需要这种方法表示
