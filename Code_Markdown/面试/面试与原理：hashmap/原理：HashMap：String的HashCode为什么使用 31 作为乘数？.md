> 源码分析、理论实践

HashMap 的实现中，HashCode 的散列占了很重要的一设计思路， 所以最好把这部分知识补全，再往下讲解。
# 一、面试题
说到 HashCode 的面试题，可能这是一个非常核心的了。其他考点；怎么实现散
列、计算逻辑等，都可以通过这道题的学习了解相关知识。
Why does Java's hashCode() in String use 31 as a multiplier?

这个问题其实☞指的就是，hashCode 的计算逻辑中，为什么是 31 作为乘数。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300011166.png)
# 二、资源下载
本文讲解的过程中涉及部分源码等资源，可以通过关注公众号：bugstack虫洞栈，
回复下载进行获取{回复下载后打开获得的链接，找到编号 ID：19}，包括；
1. HashCode 源码测试验证工程，interview-03
2. 103976 个英语单词库.txt，验证 HashCode 值
3. HashCode 散列分布.xlsx，散列和碰撞图表
# 三、源码讲解
## 1. 固定乘积 31 在这用到了
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300012630.png)
在获取 hashCode 的源码中可以看到，有一个固定值 31，在 for 循环每次执行时
进行乘积计算，循环后的公式如下； `s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]`
那么这里为什么选择 31 作为乘积值呢？
## 2. 来自 stackoverflow 的回答
在 stackoverflow 关于为什么选择 31 作为固定乘积值，有一篇讨论文章，Why
does Java's hashCode() in String use 31 as a multiplier? 这是一个时间
比较久的问题了，摘取两个回答点赞最多的；
413 个赞👍的回答
最多的这个回答是来自《Effective Java》的内容；
The value 31 was chosen because it is an odd prime. If it were even and the multipl
ication overflowed, information would be lost, as multiplication by 2 is equivalent
to shifting. The advantage of using a prime is less clear, but it is traditional.
A nice property of 31 is that the multiplication can be replaced by a shift and a s
ubtraction for better performance: 31 * i == (i << 5) - i. Modern VMs do this sort
of optimization automatically.
这段内容主要阐述的观点包括；
1. 31 是一个奇质数，如果选择偶数会导致乘积运算时数据溢出。
2. 另外在二进制中，2 个 5 次方是 32，那么也就是 31 * i == (i << 5) - i。这主要是说**乘积运算可以使用位移提升性能**，同时目前的 JVM 虚拟机也会自动支持此类的优化。

80 个赞👍的回答
As Goodrich and Tamassia point out, If you take over 50,000 English words (formed a
s the union of the word lists provided in two variants of Unix), using the constant
s 31, 33, 37, 39, and 41 will produce less than 7 collisions in each case. Knowing
this, it should come as no surprise that many Java implementations choose one of th
ese constants.
- 这个回答就很有实战意义了，告诉你用超过 5 千个单词计算 hashCode，这个 hashCode 的运算使用 31、33、37、39 和 41 作为乘积，得到的碰撞结果，31 被使用就很正常了
- **他这句话就就可以作为我们实践的指向了**。

## 3. Hash 值碰撞概率统计
接下来要做的事情并不难，只是根据 stackoverflow 的回答，统计出不同的乘积数对 10 万个单词的 hash 计算结果。10 个单词表已提供，可以通过关注公众号：bugstack 虫洞栈进行下载
### 3.1 读取单词字典表
```java
1 a "n.(A)As 或 A's 安(ampere(a) art.一;n.字母 A /[军] Analog.Digital,模拟/数
字 /(=account of) 帐上"
2 aaal American Academy of Arts and Letters 美国艺术和文学学会
3 aachen 亚琛[德意志联邦共和国西部城市]
4 aacs Airways and Air Communications Service (美国)航路与航空通讯联络处
5 aah " [军]Armored Artillery Howitzer,装甲榴弹炮;[军]Advanced Attack Helicopter,先进
攻击直升机"
6 aal "ATM Adaptation Layer,ATM 适应层"
7 aapamoor "n.[生]丘泽,高低位镶嵌沼泽"
```
单词表的文件格式如上，可以自行解析。读取文件的代码比较简单，这里不展示了，可以通过资源下载进行获取
### 3.2 Hash 计算函数
```java
public static Integer hashCode(String str, Integer multiplier) {
    int hash = 0;
    for (int i = 0; i < str.length(); i++) {
        hash = multiplier * hash + str.charAt(i);
    }
    return hash;
}
```
这个过程比较简单，与原 hash 函数对比只是替换了可变参数，用于我们统计不同乘积数的计算结果。
### 3.3 Hash 碰撞概率计算
想计算碰撞很简单，也就是计算那些出现相同哈希值的数量，计算出碰撞总量即
可。这里的实现方式有很多，可以使用 set、map 也可以使用 java8 的 stream 流
统计 distinct。
```java
private static RateInfo hashCollisionRate(Integer multiplier, List<Integer> hashCod
eList) {
    int maxHash = hashCodeList.stream().max(Integer::compareTo).get();
    int minHash = hashCodeList.stream().min(Integer::compareTo).get();
    int collisionCount = (int) (hashCodeList.size() - hashCodeList.stream().distinct
    ().count());
    double collisionRate = (collisionCount * 1.0) / hashCodeList.size();
    return new RateInfo(maxHash, minHash, multiplier, collisionCount, collisionRate)
    ;
}
```
- 这里记录了最大 hash 和最小 hash 值，以及最终返回碰撞数量的统计结
果。
### 3.4 单元测试
```java
@Before
public void before() {
    "abc".hashCode();
    // 读取文件，103976 个英语单词库.txt
    words = FileUtil.readWordList("E:/itstack/git/github.com/interview/interview-01/103976 个英语单词库.txt");
}
@Test
public void test_collisionRate() {
    List<RateInfo> rateInfoList = HashCode.collisionRateList(words, 2, 3, 5, 7, 17,
    31, 32, 33, 39, 41, 199);
    for (RateInfo rate : rateInfoList) {
        System.out.println(String.format("乘数 = %4d, 最小 Hash = %11d, 最大Hash = %10d, 碰撞数量 =%6d, 碰撞概率 = %.4f%%", rate.getMultiplier(), rate.getMinHash(), rate.getMaxHash(), rate.getCollisionCount(), rate.getCollisionRate() * 100));
    }
}
```
- 以上先设定读取英文单词表中的 10 个单词，之后做 hash 计算。
- 在 hash 计算中把单词表传递进去，同时还有乘积数；2, 3, 5, 7, 17,31, 32, 33, 39, 41, 199，最终返回一个 list 结果并输出。
- 这里主要验证同一批单词，**对于不同乘积数会有怎么样的 hash 碰撞结果**。

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300020716.png)

![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300020917.png)
以上就是不同的乘数下的 hash 碰撞结果图标展示，从这里可以看出如下信息；
1. **乘数是 2 时，hash 的取值范围比较小，基本是堆积到一个范围内了**，后面内容会看到这块的展示。
2. 乘数是 3、5、7、17 等，都有较大的碰撞概率
3. **乘数是 31 的时候，碰撞的概率已经很小了，基本稳定**。
4. 顺着往下看，你会发现 199 的碰撞概率更小，这就相当于一排奇数的茅坑量多，自然会减少碰撞。但这个范围值已经远超过 int 的取值范围了，如果用此数作为乘数，又返回 int 值，就会丢失数据信息。

## 4. Hash 值散列分布
除了以上看到**哈希值在不同乘数的一个碰撞概率**后，关于散列表也就是 hash，还有一个非常重要的点，那就是要**尽可能的让数据散列分布**。只有这样才能减少 hash 碰撞次数，也就是后面章节要讲到的 hashMap 源码。

那么怎么看散列分布呢？如果我们能把 10 万个 hash 值铺到图表上，形成的一张图，就可以看出整个散列分布。但是这样的图会比较大，当我们缩小看后，就成一个了大黑点。所以这里我们采取分段统计，**把 2 ^ 32 方分 64 个格子进行存放，每个格子都会有对应的数量的 hash 值**，最终把这些数据展示在图表上。
```java
public static Map<Integer, Integer> hashArea(List<Integer> hashCodeList) {
    Map<Integer, Integer> statistics = new LinkedHashMap<>();
    int start = 0;
    for (long i = 0x80000000; i <= 0x7fffffff; i += 67108864) {
        long min = i;
        long max = min + 67108864;
        // 筛选出每个格子里的哈希值数量，java8 流统计；https://bugstack.cn/itstack-demo-
        any/2019/12/10/%E6%9C%89%E7%82%B9%E5%B9%B2%E8%B4%A7-
        Jdk1.8%E6%96%B0%E7%89%B9%E6%80%A7%E5%AE%9E%E6%88%98%E7%AF%87(41%E4%B8%AA%E6%A1%88%E
        4%BE%8B).html
        int num = (int) hashCodeList.parallelStream().filter(x -> x >= min && x < m
        ax).count();
        statistics.put(start++, num);
    }
    return statistics;
}
```
- 这个过程主要统计 int 取值范围内，每个哈希值存放到不同格子里的数量。
- 这里也是使用了 java8 的新特性语法，统计起来还是比较方便的。

### 4.2 单元测试
```java
@Test
public void test_hashArea() {
    System.out.println(HashCode.hashArea(words, 2).values());
    System.out.println(HashCode.hashArea(words, 7).values());
    System.out.println(HashCode.hashArea(words, 31).values());
    System.out.println(HashCode.hashArea(words, 32).values());
    System.out.println(HashCode.hashArea(words, 199).values());
}
```
- 这里列出我们要统计的乘数值，每一个乘数下都会有对应的哈希值数量汇总，也就是 64 个格子里的数量。
- 最终把这些统计值放入到 excel 中进行图表化展示。

### 统计图表
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300024277.png)
- 以上是一个堆积百分比统计图，可以看到下方是不同乘数下的，每个格子里的数据统计。
- 除了 199 不能用以外，31 的散列结果相对来说比较均匀。

### 4.2.1 乘数 2 散列
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300025304.png)

乘数是 2 的时候，散列的结果基本都堆积在中间，没有很好的散列。
### 4.2.2 乘数 31 散列
乘数是 31 的时候，散列的效果就非常明显了，基本在每个范围都有数据存放。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300026981.png)

### 4.2.3 乘数 199 散列
乘数是 199 是不能用的散列结果，但是它的数据是更加分散的，从图上能看到有两个小山包。**但因为数据区间问题会有数据丢失问题，所以不能选择**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305300027512.png)

### 文中引用
- http://www.tianxiaobo.com/2018/01/18/String-hashCode-%E6%96%B9%E6%B3%95%E4%B8%BA%E4%BB%80%E4%B9%88%E9%80%89%E6%8B%A9%E6%95%B0%E5%AD%9731%E4%BD%9C%E4%B8%BA%E4%B9%98%E5%AD%90/
- https://stackoverflow.com/questions/299304/why-does-javas-hashcode-in-string-use-31-as-a-multiplier
# 四、总结
以上主要介绍了 hashCode 选择 31 作为乘数的主要原因和实验数据验证，算是一个散列的数据结构的案例讲解，在后续的类似技术中，就可以解释其他的源码设计思路了。

看过本文至少应该让你可以从根本上解释了 hashCode 的设计，关于他的所有问题也就不需要死记硬背了，学习编程内容除了最开始的模仿到深入以后就需要不断的研究数学逻辑和数据结构。

文中参考了优秀的 hashCode 资料和 stackoverflow，并亲自做实验验证结果，大家也可以下载本文中资源内容；英文字典、源码、excel 图表等内容。