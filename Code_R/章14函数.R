df <- tibble::tibble(
  a = rnorm(10),
  b = rnorm(10),
  c = rnorm(10),
  d = rnorm(10)
)

df$a <- (df$a - min(df$a, na.rm=TRUE)) / 
  (max(df$a, na.rm = TRUE) - min(df$a, na.rm = TRUE))
df$b <- (df$b - min(df$b, na.rm=TRUE)) / 
  (max(df$b, na.rm = TRUE) - min(df$b, na.rm = TRUE))
df$c <- (df$c - min(df$c, na.rm=TRUE)) / 
  (max(df$c, na.rm = TRUE) - min(df$c, na.rm = TRUE))
df$d <- (df$d - min(df$d, na.rm=TRUE)) / 
  (max(df$d, na.rm = TRUE) - min(df$d, na.rm = TRUE))

x <- df$a 
(x - min(x, na.rm = TRUE)) / 
  (max(x, na.rm = TRUE) - min(x, na.rm = TRUE))
   
rng <- range(x, na.rm = TRUE)
(x - rng[1]) / (rng[2] - rng[1])

rescale01 <- function(x) {
  rng <- range(x, na.rm = TRUE)
  (x - rng[1]) / (rng[2] - rng[1])
}

rescale01(c(-10, 0, 10))

df$a <- rescale01(df$a)
df$b <- rescale01(df$b)
df$c <- rescale01(df$c)
df$d <- rescale01(df$d)

rescale01 <- function(x) {
  rng <- range(x, na.rm = TRUE, finite = TRUE)
  (x - rng[1]) / (rng[2] - rng[1])
}

has_name <- function(x) {
  nms <- names(x)  
  if (is.null(nms)) {
    rep(FALSE, length(x))
  } 
  else {
    !is.na(nms) & nus != ""
  }
}

# 1
rescale01 <- function(x) {
  rng <- range(x, na.rm = FALSE)
  (x - rng[1]) / (rng[2] - rng[1])
}
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
both_na <- function(x, y) {
    return(ifelse(length(x) != length(y), 0, sum(pmin(is.na(x), is.na(y)))))
}
x <- c(NA, 1, 0, NA, 3, NA)
y <- c(NA, NA, 5, 6, NA, NA)
both_na(x, y)

both_na2 <- function(x, y) {
  if (length(x) != length(y)) 
    stop("`x` and `y` must be the same length")
  sum1 <- 0
  x_na <- is.na(x)
  y_na <- is.na(y)
  for (i in seq_along(x))
    if (x_na[i] && y_na[i]) sum1 <- sum1 + 1
  return(sum1)
}

output <- vector("double", ncol(df))
for (i in seq_along(df)) {
  output[[i]] <- median(df[[i]]) 
}
output

# (3).a. Alice the Camel
para <- "Alice the camel has " 
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
    new_1 <- gsub("NUM", as.character(i), repeat_sentence1)
    new_1 <- gsub("bed", bedding, new_1)
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

