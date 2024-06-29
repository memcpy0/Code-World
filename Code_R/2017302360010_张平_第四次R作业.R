---
title: "R语言第四次作业_ggplot画图"
author: "张平"
date: "2019年3月31日"
output: html_document
---

```{r setup, include=FALSE}
knitr::opts_chunk$set(echo = TRUE)
```
# P10.
# (1)
因为`color = "blue"`被包含在了该图层的mapping参数中, 所以该设置被当做几何对象属性与待显示变量间的映射之一, 这样就将"blue"作为一个只有一个值“blue”的变量来对待, 所以不改变颜色。
应该在aes()的外部设置几何对象颜色, 此时颜色不会传递关于待显示变量的信息, 只是改变图层的外观。
```{r}
library(tidyverse)
ggplot(data = mpg) +
  geom_point(mapping = aes(x = displ, y = hwy), color = "blue")
```
## (2)
```{r}
?mpg
glimpse(mpg)
```
glimpse function displays the type of each column 




