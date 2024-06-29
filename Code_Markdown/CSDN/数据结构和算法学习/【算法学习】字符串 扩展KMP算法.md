# 7. 扩展KMP算法
扩展KMP算法模板题：[HDU 2594 Simpsons Hidden Talents](http://acm.hdu.edu.cn/showproblem.php?pid=2594) 

扩展KMP是对KMP算法的拓展，它解决的问题是：文本串 `S` 和模式串 `T` ，`S` 长度为 `n` ，`T` 的长度为 `m` ，求出文本串 `S` 的每个后缀子串与模式串 `T` 的最长公共前缀。

为此设置一个 `extend[]` 数组，`extend[i]` 表示 `T` 与 `S[i, n - 1]` 的最长公共前缀，要求出所有的 `extend[i], 0 <= i < n` 。

我们注意到：如果有一个位置 `extend[i] = m` ，则表示 `T` 在 `S` 中出现，而且是在 `i` 位置出现，这就是标准的KMP问题，只是做了一些拓展罢了。

下面举一个例子 `S = "aaaabaa", T = "aaaaa"` ：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200830194511816.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)


扩展KMP算法的一般步骤：
- 首先从左到右依次计算 `extend[]` 数组，某一时刻设 `extend[0...k]` 已经计算完毕，并且之前匹配过程中达到的最远位置为 `P` ——所谓最远位置，就是 `i + extend[i] - 1` 的最大值 `0 <= i <= k` ；
- 
