---
title: "第五章探索数据分析"
author: "张平"
date: "2019年4月12日"
output: html_document
---

```{r setup, include=FALSE}
knitr::opts_chunk$set(echo = TRUE)
```

```{r}
library(tidyverse)
library(nycflights13)
ggplot(data = diamonds) + 
  geom_bar(mapping = aes(x = cut))
diamonds %>%
  count(cut)
```

```{r}
ggplot(data = diamonds) +
  geom_histogram(mapping = aes(x = carat), binwidth = 0.5)
diamonds %>%
  count(cut_width(carat, 0.5))
```
```{r}
diamonds %>% 
  filter(carat < 3) %>%
  ggplot() +
  geom_histogram(mapping = aes(x = carat), binwidth = 0.01) 
diamonds %>% 
  filter(carat < 3) %>%
  ggplot(mapping = aes(x = carat, color = cut)) +
  geom_freqpoly(binwidth = 0.1)
```
```{r}
ggplot(data = faithful, mapping = aes(x = eruptions)) +
  geom_histogram(binwidth = 0.25)
```
```{r}
ggplot(diamonds) +
  geom_histogram(mapping = aes(x = y), binwidth = 0.5)

```

```{r}
ggplot(diamonds) +
  geom_histogram(mapping = aes(x = y), binwidth = 0.5) +
  coord_cartesian(ylim = c(0, 50))
```

```{r}
usual <- diamonds %>%
  filter(y < 3 | y> 20) %>%
  arrange(y)
```
```{r}
diamonds2 <- diamonds %>%
  mutate(y = ifelse(y < 3 | y > 20, NA, y))
ggplot(data = diamonds2, mapping = aes(x = x, y = y)) +
  geom_point(na.rm = TRUE)
```
```{r}
flights %>%
  mutate(
    canceled = is.na(dep_time), 
    sched_hour = sched_dep_time %/% 100,
    sched_min = sched_dep_time %% 100,
    sched_dep_time = sched_hour + sched_min / 60
  ) %>%
  ggplot(mapping = aes(sched_dep_time)) +
    geom_freqpoly(
      mapping = aes(color = canceled),
      binwidth = 1/4
    )
```
```{r}
ggplot(data = diamonds, mapping = aes(x = price)) +
  geom_freqpoly(mapping = aes(color = cut), binwidth = 500)
```
```{r}
ggplot(diamonds) +
  geom_bar(mapping = aes(x = cut))
ggplot(
  data = diamonds,
  mapping = aes(x = price, y = ..density..)
) +
  geom_freqpoly(mapping = aes(color = cut), binwidth = 500)
```

```{r}
ggplot(data = diamonds, mapping = aes(x = cut, y = price)) +
  geom_boxplot()
```
```{r}
ggplot(data = mpg, mapping = aes(x = class, y = hwy)) +
  geom_boxplot()
```

```{r}
ggplot(data = mpg) +
  geom_boxplot(
    mapping = aes(
      x = reorder(class, hwy, FUN = median), 
      y = hwy
    )
  )
```

```{r}
ggplot(data = mpg) +
  geom_boxplot(
    mapping = aes(
      x = reorder(class, hwy, FUN = median),
      y = hwy
    )
  ) +
  coord_flip()

```
```{r}
ggplot(data = diamonds) +
  geom_count(mapping = aes(x = cut, y = color))

diamonds %>%
  count(color, cut) %>%
  ggplot(mapping = aes(x = color, y = cut)) +
  geom_tile(mapping = aes(fill = n))
```
```{r}
ggplot(data = diamonds) + geom_point(mapping = aes(x = carat, y = price), alpha = 1/100)
```
```{r}
smaller <- diamonds %>% 
  filter(carat < 3) %>%
  ggplot() +
  geom_bin2d(mapping = aes(x = carat, y = price))

diamonds %>% 
  filter(carat < 3) %>%
  ggplot() +
  geom_hex(mapping = aes(x = carat, y = price))
```
```{r}
diamonds %>% 
  filter(carat < 3) %>%
  ggplot(mapping = aes(x = carat, y = price)) +
    geom_boxplot(mapping = aes(group = cut_width(carat, 0.1)))


diamonds %>% 
  filter(carat < 3) %>%
  ggplot(mapping = aes(x = carat, y = price)) +
    geom_boxplot(mapping = aes(group = cut_number(carat, 20)))
```
```{r}
ggplot(data = faithful) +
  geom_point(mapping = aes(x = eruptions, y = waiting))
```
```{r}
arx <- flights %>%
  group_by(tailnum) %>%
  arrange(tailnum)
group_split(arx)
```

```{r}

```






















