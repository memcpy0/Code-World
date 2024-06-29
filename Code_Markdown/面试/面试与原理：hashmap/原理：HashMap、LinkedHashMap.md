Doug Lea老爷子的操刀，让HashMap成为使用和面试最频繁的API。
- HashMap最早出现在JDK1.2中，底层基于散列算法实现。
- **HashMap允许null键和null值**，在计算哈键的哈希值时，**null键哈希值为0**。
- **HashMap并不保证键值对的顺序**，这意味着在进行某些操作（转为红黑树？）后，键值对的顺序可能会发生变化。
- 另外，需要注意的是，**HashMap是非线程安全类，在多线程环境下可能会存在问题**。

```java
Hash table based implementation of the Map interface. This implementation provides all of the optional map operations, and permits null values and the null key. (The HashMap class is roughly equivalent to Hashtable, except that it is unsynchronized and permits nulls.) This class makes no guarantees as to the order of the map; in particular, it does not guarantee that the order will remain constant over time.
基于Map接口的哈希表实现，提供了所有可选的map操作，允许null键和null值。HashMap粗略等价于Hashtable,除了是不同步的和允许null外。HashMap不保证键值对的顺序，特别地，它不保证顺序一直相同。

This implementation provides constant-time performance for the basic operations (get and put), assuming the hash function disperses the elements properly among the buckets. Iteration over collection views requires time proportional to the "capacity" of the HashMap instance (the number of buckets) plus its size (the number of key-value mappings). Thus, it's very important not to set the initial capacity too high (or the load factor too low) if iteration performance is important.
实现提供了常量时间的基础操作(get put)，假设哈希函数将元素合适地分散在桶中。迭代整个集合需要的时间正比与HashMap实例的capacity（桶的数量）+size（键值对数量）。因此，非常重要的是，如果迭代性能很重要，不要把初始的capacity设置得太高、把load factor设置得太低。

An instance of HashMap has two parameters that affect its performance: initial capacity and load factor. The capacity is the number of buckets in the hash table, and the initial capacity is simply the capacity at the time the hash table is created. The load factor is a measure of how full the hash table is allowed to get before its capacity is automatically increased. When the number of entries in the hash table exceeds the product of the load factor and the current capacity, the hash table is rehashed (that is, internal data structures are rebuilt) so that the hash table has approximately twice the number of buckets.

As a general rule, the default load factor (.75) offers a good tradeoff between time and space costs. Higher values decrease the space overhead but increase the lookup cost (reflected in most of the operations of the HashMap class, including get and put). The expected number of entries in the map and its load factor should be taken into account when setting its initial capacity, so as to minimize the number of rehash operations. If the initial capacity is greater than the maximum number of entries divided by the load factor, no rehash operations will ever occur.

If many mappings are to be stored in a HashMap instance, creating it with a sufficiently large capacity will allow the mappings to be stored more efficiently than letting it perform automatic rehashing as needed to grow the table. Note that using many keys with the same hashCode() is a sure way to slow down performance of any hash table. To ameliorate impact, when keys are Comparable, this class may use comparison order among keys to help break ties.

Note that this implementation is not synchronized. If multiple threads access a hash map concurrently, and at least one of the threads modifies the map structurally, it must be synchronized externally. (A structural modification is any operation that adds or deletes one or more mappings; merely changing the value associated with a key that an instance already contains is not a structural modification.) This is typically accomplished by synchronizing on some object that naturally encapsulates the map. If no such object exists, the map should be "wrapped" using the Collections.synchronizedMap method. This is best done at creation time, to prevent accidental unsynchronized access to the map:
    Map m = Collections.synchronizedMap(new HashMap(...));
    
The iterators returned by all of this class's "collection view methods" are fail-fast: if the map is structurally modified at any time after the iterator is created, in any way except through the iterator's own remove method, the iterator will throw a ConcurrentModificationException. Thus, in the face of concurrent modification, the iterator fails quickly and cleanly, rather than risking arbitrary, non-deterministic behavior at an undetermined time in the future.

Note that the fail-fast behavior of an iterator cannot be guaranteed as it is, generally speaking, impossible to make any hard guarantees in the presence of unsynchronized concurrent modification. Fail-fast iterators throw ConcurrentModificationException on a best-effort basis. Therefore, it would be wrong to write a program that depended on this exception for its correctness: the fail-fast behavior of iterators should be used only to detect bugs.
```
随着几代的优化更新，到目前为止它的源码部分已经比较复杂，涉及的知识点也非常多。在 JDK 1.8 中包括；
1. 散列表实现
2. **扰动函数**
3. **初始化容量**
4. **负载因子**
5. **扩容元素拆分**
6. **链表树化**、
7. **红黑树**、
8. 插入、
9. 查找、
10. 删除、
11. 遍历、
12. 分段锁等等

# 1. 写一个最简单的 HashMap 
学习 HashMap 前，最好的方式是先了解这是一种怎么样的数据结构来存放数据。 而 HashMap 经过多个版本的迭代后，乍一看代码还是很复杂的。就像你原来只穿 个裤衩，现在还有秋裤和风衣。所以先来看看最根本的 HashMap 是什么样。

**问题**： 假设我们有一组 7 个字符串，需要存放到数组中，但要求在获取每个元 素的时候时间复杂度是 O(1)。也就是说你不能通过循环遍历的方式进行获取， 而是要定位到数组 ID 直接获取相应的元素。
**方案**： 如果说我们需要通过 ID 从数组中获取元素，那么就需要把每个字符串都 计算出一个在数组中的位置 ID。
1. ==字符串获取 ID 你能想到什么方式？ 一个字符串最直接的获取跟数字相关的信息就是 HashCode==，可 HashCode 的取值范围太大 了 $[-2147483648, 2147483647]$ ，**不可能直接使用**。
2. 那么**就需要使用 HashCode 与 数组长度做与运算，得到一个可以在数组中出现的位置**。
3. 如果说有两个元素得到 同样的 ID，那么这个数组 ID 下就存放两个字符串。 

以上呢，就是我们要把字符串散列到数组中的一个基本思路，接下来把这个思路用代码实现出来。
## 1.1 代码实现
基本思路的测试：
```java
@Test  
public void test_128hash() {  
	// 初始化一组字符串  
	List<String> list = new ArrayList<>();  
	list.add("jlkk");  
	list.add("lopi");  
	list.add("memcpy0");  
	list.add("e4we");  
	list.add("alpo");  
	list.add("yhjk");  
	list.add("plop");  

	// 定义要存放的数组  
	String[] tab = new String[8];  

	// 循环存放  
	for (String key : list) {  
		int idx = key.hashCode() & (tab.length - 1); // 计算索引位置  
		System.out.println(String.format("key值=%s Idx=%d", key, idx));  
		if (tab[idx] == null) {  
			tab[idx] = key;  
			continue;            }   
		tab[idx] = tab[idx] + "->" + key; // 仿照链表  
	}  
	// 输出测试结果  
	System.out.println("测试结果：" + JSON.toJSONString(tab));  
}  
```
这段代码整体看起来也是非常简单，并没有什么复杂度，主要包括以下内容；
1. 初始化一组字符串集合，这里初始化了 7 个。 
2. 定义一个数组用于存放字符串，注意==**这里的长度是 8，也就是 2 的倍 数**。这样的数组长度-1才会出现一个0111——除高位以外都是 1 的特征，也 是为了散列==。 
3. 接下来就是循环存放数据，计算出每个字符串在数组中的位置。 `key.hashCode() & (tab.length - 1)`。 
4. 在字符串存放到数组的过程，如果遇到相同的元素，进行连接操作**模拟 链表的过程**。
5. 最后输出存放结果。
## 1.2 测试结果
这就达到了我们一个最基本的要求，**将串元素散列存放到数组中，最后 通过字符串元素的索引 ID 进行获取对应字符串**。这样是 HashMap 的一个 **最基本原理**，有了这个基础后面就会更容易理解 HashMap 的源码实现。
```java
keyֵ=jlkk Idx=2
keyֵ=lopi Idx=4
keyֵ=memcpy0 Idx=1
keyֵ=e4we Idx=5
keyֵ=alpo Idx=2
keyֵ=yhjk Idx=0
keyֵ=plop Idx=5
测试结果：["yhjk",null,"jlkk->alpo",null,"lopi","e4we->plop",null,"小傅哥"]
```

```java
@Before  
public void before() {  
    // 读取文件，103976个英语单词库.txt  
    words = FileUtil.readWordList("C:\\Users\\21839\\Desktop\\interview-master\\docs\\103976个英语单词库.txt");  
}
```

```java
package com.memcpy0.interview;  
  
import java.io.BufferedReader;  
import java.io.FileInputStream;  
import java.io.InputStreamReader;  
import java.nio.charset.StandardCharsets;  
import java.util.HashSet;  
import java.util.Set;  
  
public class FileUtil {  
  
    /**  
     * 读取本地文件，单词表  
     * @param url 单词表.txt文件  
     * @return 单词集合(去重)  
     */    public static Set<String> readWordList(String url) {  
        Set<String> list = new HashSet<>();  
        try {  
            InputStreamReader isr = new InputStreamReader(new FileInputStream(url), StandardCharsets.UTF_8);  
            BufferedReader br = new BufferedReader(isr);  
            String line = "";  
            while ((line = br.readLine()) != null) {  
                String[] ss = line.split("\t");  
                list.add(ss[1]);  
            }  
            br.close();  
            isr.close();  
        } catch (Exception ignore) {  
            return null;  
        }  
        return list;  
    }  
}
```

## 1.3 Hash 散列示意图 
如果上面的测试结果不能在你的头脑中很好的建立出一个数据结构，那么可以看 以下这张散列示意图，方便理解；![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305011226496.png)
- 这张图就是上面代码实现的全过程，将每一个字符串元素通过 Hash 计算 索引位置，存放到数组中。 
- 黄色的索引 ID 是没有元素存放、绿色的索引 ID 存放了一个元素、红色 的索引 ID 存放了两个元素。

## 1.4 这个简单的 HashMap 有哪些问题
以上我们实现了一个简单的 HashMap，或者说还算不上 HashMap，只能算做一个 散列数据存放的雏形。但这样的一个数据结构放在实际使用中，会有哪些问题呢？ 
1. 这里所有的元素存放都需要获取一个索引位置，而如果**元素的位置不够**，散列碰撞严重，那么就失去了散列表存放的意义，没有达到预期的性 能。 
2. ==在获取索引 ID 的计算公式中，需要数组长度是 2 的倍数，（**这样才能用与操作代替取模，得到一个可在数组中出现的位置**），那么怎么进行 初始化这个数组大小==。 
3. 数组越小碰撞的可能越大，数组越大碰撞的可能越小，**时间与空间如何取舍**。  
4. 随着元素的不断添加，**数组长度不足扩容时，怎么把原有的元素，拆分 到新的位置上去**。 
5. 目前存放 7 个元素，已经有两个位置都存放了 2 个字符串，那么**链表越 来越长怎么优化**。

以上这些问题可以归纳为；==扰动函数、初始化容量、负载因子、扩容方法以及链表和 红黑树转换的使用==等。接下来我们会逐个问题进行分析。

---
# 2. 扰动函数 
在 HashMap 存放元素时候**有这样一段代码来处理哈希值**，这是 java 8 的**散列值 扰动函数**，用于**优化散列效果**； 
```java
/**
Computes key.hashCode() and spreads (XORs) higher bits of hash to lower. Because the table uses power-of-two masking, sets of hashes that vary only in bits above the current mask will always collide. (Among known examples are sets of Float keys holding consecutive whole numbers in small tables.) So we apply a transform that spreads the impact of higher bits downward. There is a tradeoff between speed, utility, and quality of bit-spreading. Because many common sets of hashes are already reasonably distributed (so don't benefit from spreading), and because we use trees to handle large sets of collisions in bins, we just XOR some shifted bits in the cheapest possible way to reduce systematic lossage, as well as to incorporate impact of the highest bits that would otherwise never be used in index calculations because of table bounds.
计算key.hashCode()并将hash值的16高位xor hash值的低16位(散布)
因为哈希表使用2的幂作为掩码，仅在当前掩码之上以位为单位变化的哈希集总是会发生冲突。已知的例子包括在小哈希表表中保存连续整数的浮点键集。
因此应用了一种变换，将高位的影响向下扩展。在比特扩展的速度、效用和质量之间存在权衡。
由于许多常见的哈希集已经合理分布（因此不会从扩展中受益），并且因为我们使用树来处理桶中的大型冲突集，所以只需以最便宜的方式对一些移位的比特进行异或，以减少系统损失，并合并最高比特的影响——否则由于表边界的原因，这些高位比特将永远不会用于索引计算。
*/
static final int hash(Object key) { 
	int h; 
	return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16); 
}
```
## 2.1 为什么使用扰动函数 
理论上来说**字符串的 hashCode是一个 int 类型值，那可以直接作为数组下标了， 且不会出现碰撞**？。

但这个 hashCode 的取值范围是 $[-2147483648, 2147483647]$ ， 有将近 40 亿的长度，谁也不能把数组初始化的这么大，内存也是放不下的。 我们默认初始化的 Map 大小是 16 ，长度 `DEFAULT_INITIAL_CAPACITY = 1 << 4` ， 所以**获取的 Hash 值并不能直接作为下标使用，需要与数组长度进行取模运算得 到一个下标值**，也就是我们上面做的散列列子。 
```java
/**  
 * The default initial capacity - MUST be a power of two. 
 **/
static final int DEFAULT_INITIAL_CAPACITY = 1 << 4; // aka 16

/**  
 * The maximum capacity, used if a higher value is implicitly specified * by either of the constructors with arguments. * MUST be a power of two <= 1<<30. */
static final int MAXIMUM_CAPACITY = 1 << 30;
```
那么，hashMap 源码这里不只是直接获取哈希值，还进行了一次扰动计算，`(h = key.hashCode()) ^ (h >>> 16)` 。**把哈希值右移 16 位，也就正好是自己长度的一 半，之后与原哈希值做异或运算**，这样就**混合了原哈希值中的高位和低位，增大 了随机性**。计算方式如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305011416925.png)

说白了，**使用扰动函数就是为了增加随机性，让数据元素更加均衡的散 列，减少碰撞**。
## 2.2 实验验证扰动函数 
从上面的分析可以看出，==**扰动函数使用了哈希值的高半区和低半区做异或，混合 原始哈希码的高位和低位**，以此来**加大低位区的随机性**。 但看不到实验数据的话，这终究是一段理论==，具体这段哈希值真的被增加了随机性没有，并不知道。

所以这里要做一个实验，这个实验是这样做； 
1. 选取 10 万个单词的词库 
2. 定义 **128 位长度的数组格子** 
3. 分别计算**在扰动和不扰动下**，10 万单词的下标**分配到 128 个格子的数量** 
4. **统计各个格子数量，生成波动曲线**。==如果扰动函数下的波动曲线相对更平稳，那么证明扰动函数有效果==。


### 2.2.1 扰动代码测试
#### 1. 扰动函数对比方法
disturbHashIdx 扰动函数下，下标值计算
hashIdx 非扰动函数下，下标值计算
```java
package com.memcpy0.interview;  
  
/**  
 * 用于验证扰动函数的效果  
 * 如果扰动函数下的曲线更平稳，那么证明扰动函数有效果。  
 */  
public class Disturb {  
    // 没有扰动的key的下标  
    public static int hashIdx(String key, int size) {  
        return key.hashCode() & (size - 1);  
    };  
    // 有扰动的key的下标  
    public static int disturbHashIdx(String key, int size) {  
        return (key.hashCode() ^ (key.hashCode() >>> 16)) & (size - 1);  
    }  
}
```
#### 2. 单元测试
计算10万单词分别在扰动和非扰动环境下，出现多少次哈希冲突：
```java
// 10万单词已初始化到words中  
@Test  
public void test_disturb() {  
    HashMap<Integer, Integer> map = new HashMap<>(16);  
    for (String word : words) {  
        // 使用扰动函数  
        int idx = Disturb.disturbHashIdx(word, 128);  
        // 不使用扰动函数  
        // int idx = Disturb.hashIdx(word, 128);  
        if (map.containsKey(idx)) { // 已经包含这个下标  
            Integer integer = map.get(idx);  
            map.put(idx, ++integer); // 该下标对应的单词数+1,即哈希冲突,  
        } else {  
            map.put(idx, 1); // 该下标对应一个单词  
        }  
    }  
    System.out.println(map.values());  
}
```
以上分别统计两种函数下的下标值分配，最终将统计结果放到 excel 中生成图 表。使用扰动函数：
```java
[835, 825, 777, 783, 837, 850, 828, 800, 828, 825, 832, 832, 862, 792, 785, 817, 798, 809, 767, 851, 796, 760, 888, 804, 840, 815, 867, 813, 838, 803, 835, 813, 838, 797, 855, 852, 789, 810, 833, 841, 812, 881, 817, 821, 823, 873, 778, 767, 814, 741, 756, 763, 839, 832, 799, 816, 728, 824, 803, 815, 749, 815, 748, 821, 798, 845, 796, 820, 817, 823, 803, 801, 784, 834, 793, 824, 823, 792, 832, 801, 822, 871, 765, 831, 803, 808, 792, 778, 766, 847, 837, 797, 775, 812, 858, 815, 798, 784, 811, 779, 793, 796, 791, 790, 797, 851, 836, 864, 847, 811, 821, 807, 814, 794, 774, 839, 845, 778, 776, 841, 785, 857, 805, 792, 808, 849, 813, 812]
```
不使用：
```java
[819, 830, 767, 845, 822, 827, 807, 752, 843, 821, 828, 820, 836, 808, 784, 836, 815, 774, 823, 782, 820, 827, 856, 861, 796, 772, 832, 784, 783, 797, 798, 796, 818, 791, 811, 881, 801, 845, 819, 814, 813, 818, 822, 816, 805, 821, 798, 794, 792, 813, 839, 762, 827, 841, 820, 787, 839, 807, 841, 758, 757, 778, 802, 787, 778, 808, 823, 830, 832, 812, 810, 854, 803, 812, 780, 823, 818, 795, 773, 832, 785, 789, 822, 818, 765, 767, 826, 762, 793, 794, 809, 809, 842, 867, 815, 798, 776, 830, 817, 814, 817, 839, 797, 799, 843, 839, 818, 771, 840, 820, 859, 833, 803, 840, 821, 840, 825, 814, 821, 808, 860, 795, 782, 831, 840, 834, 789, 839]
```
### 2.2.2 扰动函数散列图表 
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305011805020.png)
以上的两张图，分别是没有使用扰动函数和使用扰动函数的，下标分配。
实验数 据； 
1. 10 万个不重复的单词 
2. 128 个格子，相当于 128 长度的数组 

从这两种的对比图可以看出来，**在使用了扰动函数后，数据分配的更加均匀了**。 数据分配均匀，也就是==散列的效果更好，减少了 hash 的碰撞，让数据存 放和获取的效率更佳==。

---
# 3. 初始化容量和负载因子 
接下来讨论下一个问题，从我们模仿 HashMap 的例子中以及 HashMap 默认的 初始化大小里，都可以知道，**散列数组需要一个 2 的倍数的长度，因为只有 2 的 倍数在减 1 的时候，才会出现 01111 这样的值**。 

那么这里就有一个问题，我们在初始化 HashMap 的时候，如果传一个 17 个的值 new `HashMap<>(17);` ，它会怎么处理呢？
## 3.1 寻找 2 的倍数最小值 
在 HashMap 的初始化中，有这样一段方法； 
```java
/* ---------------- Public operations -------------- */  
  
/**  
 * Constructs an empty {@code HashMap} with the specified initial  
 * capacity and load factor. * * @param  initialCapacity the initial capacity  
 * @param  loadFactor      the load factor  
 * @throws IllegalArgumentException if the initial capacity is negative  
 *         or the load factor is nonpositive 
 */
 public HashMap(int initialCapacity, float loadFactor) {  
    if (initialCapacity < 0)  
        throw new IllegalArgumentException("Illegal initial capacity: " +  
            initialCapacity);  
    if (initialCapacity > MAXIMUM_CAPACITY)  
        initialCapacity = MAXIMUM_CAPACITY;  
    if (loadFactor <= 0 || Float.isNaN(loadFactor))  
        throw new IllegalArgumentException("Illegal load factor: " +  
            loadFactor); 
    
    this.loadFactor = loadFactor;  
    this.threshold = tableSizeFor(initialCapacity);  
}
```
阀值 `threshold` ，通过方法 `tableSizeFor` 进行计算，是根据**初始化**来计 算的。 
- 这个方法也就是要**寻找比初始值大的、最小的那个2进制数值**。比如传 了 $17$，我应该找到的是 $32$ 。

计算阀值大小的方法：`MAXIMUM_CAPACITY = 1 << 30` ，这个是临界范围，也就是最大的 Map 集 合。
```java
static final int tableSizeFor(int cap) {  
    int n = -1 >>> Integer.numberOfLeadingZeros(cap - 1);  
    return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;  
}
// 以前是下面这样
static final int tableSizeFor(int cap) {  
	int n = cap - 1; 
	n |= n >>> 1; 
	n |= n >>> 2; 
	n |= n >>> 4; 
	n |= n >>> 8; 
	n |= n >>> 16; 
	return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1; 
}
```
- 怎么都在向右移位 1、2、4、8、16？这主要是==**为了 把二进制的各个位置都填上 1**，当二进制的各个位置都是 1 以后，就是 一个标准的 2 的倍数减 1 了==，**最后把结果加 1 再返回即可**。 

这里我们把 17 这样一个初始化计算阀值的过程，用代码展示出来，方便理解；
```java
@Test  
public void test_threshold() {  
    System.out.println(tableSizeFor(17));  
}  
  
static int tableSizeFor(int cap) {  
    int n = cap - 1;  
    System.out.println("n = cap - 1" + ": " + Integer.toBinaryString(n));  
    n |= n >>> 1;  
    System.out.println("n |= n >>> 1" + ": " + Integer.toBinaryString(n));  
    n |= n >>> 2;  
    System.out.println("n |= n >>> 2" + ": " + Integer.toBinaryString(n));  
    n |= n >>> 4;  
    System.out.println("n |= n >>> 4" + ": " + Integer.toBinaryString(n));  
    n |= n >>> 8;  
    System.out.println("n |= n >>> 8" + ": " + Integer.toBinaryString(n));  
    n |= n >>> 16;  
    System.out.println("n |= n >>> 16" + ": " + Integer.toBinaryString(n));  
    return (n < 0) ? 1 : (n >= (1 << 30)) ? (1 << 30) : n + 1;  
}
```
结果如下：
```java
n = cap - 1: 10000
n |= n >>> 1: 11000
n |= n >>> 2: 11110
n |= n >>> 4: 11111
n |= n >>> 8: 11111
n |= n >>> 16: 11111
32
```
为什么要 `cap - 1` ？原因在于：假设 `capacity = 4` 是2的幂，不减去1，则经过这一番位操作后得到的是 `8` 。
```java
n = cap: 100
n |= n >>> 1: 110
n |= n >>> 2: 111
n |= n >>> 4: 111
n |= n >>> 8: 111
n |= n >>> 16: 111
8

n = cap - 1: 11
n |= n >>> 1: 11
n |= n >>> 2: 11
n |= n >>> 4: 11
n |= n >>> 8: 11
n |= n >>> 16: 11
4
```
对新版的代码进行测试：`Integer.numberOfLeadingZeros(cap - 1)` 得到的是cap-1的前导0个数，`>>>` 是无符号右移，则 $-1 = (111111111\dots)_{2}\ >>>\ =\  11111$
```java
@Test  
public void test_threshold2() {  
    System.out.println(tableSizeFor2(17));  
}  
  
static int tableSizeFor2(int cap) {  
    int n = -1 >>> Integer.numberOfLeadingZeros(cap - 1);  
    System.out.println("Integer.numberOfLeadingZeros(cap - 1):" + Integer.numberOfLeadingZeros(cap - 1));  
    System.out.println("-1 >>> Integer.numberOfLeadingZeros(cap - 1):" + Integer.toBinaryString(n));   
return (n < 0) ? 1 : (n >= (1 << 30)) ? (1 << 30) : n + 1;  
}
```
结果如下：
```java
Integer.numberOfLeadingZeros(cap - 1):27
-1 >>> Integer.numberOfLeadingZeros(cap - 1):11111
32
```
numberOfLeadingZeros也有趣：
```java
@IntrinsicCandidate  
public static int numberOfLeadingZeros(int i) {  
    // HD, Count leading 0's  
    if (i <= 0)  // 负数时前导零个数为0
        return i == 0 ? 32 : 0;  
    int n = 31; // 1有31个前导零,0则是32个
    if (i >= 1 << 16) { n -= 16; i >>>= 16; }  
    if (i >= 1 <<  8) { n -=  8; i >>>=  8; }  
    if (i >= 1 <<  4) { n -=  4; i >>>=  4; }  
    if (i >= 1 <<  2) { n -=  2; i >>>=  2; }  
    return n - (i >>> 1);  
}
```
## 3.2 负载因子 
```java
/**  
 * The load factor used when none specified in constructor. 
 **/
static final float DEFAULT_LOAD_FACTOR = 0.75f;
```
### 3.2.1 负载因子是做什么的？
**负载因子，可以理解成一辆车可承重重量超过某个阀值时，把货放到新的车上**。 那么**在 HashMap 中，负载因子决定了数据量多少了以后进行扩容**，==决定了时间和空间如何平衡==。

这里要提到上面做的 HashMap 例子，准备了 7 个元素，但最后还有 3 个位置空余，2 个 位置存放了 2 个元素。 所以==可能即使你数据比数组容量大时也是不一定能正好把数组占满的，而是在某些小标位置出现了大量的碰撞，只能在同一个位置用链表存放，那么这样就失去了 Map 数组的性能==。

所以，**要选择一个合理的大小下进行扩容**，默认值 0.75 就是说**当阀值容量占了 3/4 时赶紧扩容，减少 Hash 碰撞**。 同时 0.75 是一个默认构造值，在创建 HashMap 也可以调整，比如你希望**用更多 的空间换取时间，可以把负载因子调的更小一些，减少碰撞**。 
# 4. 扩容元素拆分 
为什么扩容，因为数组长度不足了、超过负载因子了。那**扩容最直接的问题，就是需要把元素拆分 到新的数组中**。拆分元素的过程中，==原 jdk1.7 中会需要重新计算哈希值，但是 到 jdk1.8 中已经进行优化，不需要重新计算，提升了拆分的性能==，设计的还是非常巧妙的。
## 4.1 测试数据
```java
@Test  
public void test_hashMap() {  
    List<String> list = new ArrayList<>();  
    list.add("jlkk");  
    list.add("lopi");  
    list.add("jmdw");  
    list.add("e4we");  
    list.add("io98");  
    list.add("nmhg");  
    list.add("vfg6");  
    list.add("gfrt");  
    list.add("alpo");  
    list.add("vfbh");  
    list.add("bnhj");  
    list.add("zuio");  
    list.add("iu8e");  
    list.add("yhjk");  
    list.add("plop");  
    list.add("dd0p");  
  
	for (String key : list) {  
		int hash = key.hashCode() ^ (key.hashCode() >>> 16);  
		System.out.println("String: " + key + " \tIdx(16): " + ((16 - 1) & hash) // 扰动后的hash在大小为16的数组中的位置  
		        + " \tBit value: " + Integer.toBinaryString(hash) + " - " // hash的二进制表示  
		        + Integer.toBinaryString(hash & 16)
		        + " \t\tIdx(32): " + ((32 - 1) & hash)); // 扰动后的hash在大小为32的数组中的位置  
		System.out.println(Integer.toBinaryString(key.hashCode()) +" "+ // key的哈希码的二进制  
		        Integer.toBinaryString(hash) + " " + Integer.toBinaryString((32 - 1) & hash));
	}
}
```
## 4.2 测试结果
这里我们随机使用一些字符串，计算**他们分别在 16 位长度和 32 位长度数 组下的索引分配情况**，**看哪些数据被重新路由到了新的地址**。 

同时，这里还可以观察出一个非常重要的信息，**计算得到的一次/原哈希值 `key.hashCode() ^ (key.hashCode() >>> 16)` 与扩容新增 出来的长度 16，进行&运算，如果值等于 0，则下标位置不变。如果不为 0，那么新的位置则是原来位置上加 16**。｛这个地方需要好好理解下， 并看实验数据｝。
- 原因很简单：一次哈希值在扩容前后不变、且永远不变（不改变哈希算法的前提下），扩容前，在数组中的下标位置为 `hash & 1111` ，扩容后，在数组中的下标位置为 `hash & 11111` ，如果 $hash \& 16 = hash \& (10000)_2$ 为0，说明 `hash` 第五位为零，扩容后的下标位置不变；否则扩容后的下标位置+16。

这样一来，就不需要在重新计算每一个数组中元素的哈希值了。
```html
String: jlkk 	Idx(16): 3 	Bit value: 1100011101001000010011 - 10000 	Idx(32): 19
1100011101001000100010 1100011101001000010011 10011
String: lopi 	Idx(16): 14 	Bit value: 1100101100011010001110 - 0 	Idx(32): 14
1100101100011010111100 1100101100011010001110 1110
String: jmdw 	Idx(16): 7 	Bit value: 1100011101010100100111 - 0 	Idx(32): 7
1100011101010100010110 1100011101010100100111 111
String: e4we 	Idx(16): 3 	Bit value: 1011101011101101010011 - 10000 	Idx(32): 19
1011101011101101111101 1011101011101101010011 10011
String: io98 	Idx(16): 4 	Bit value: 1100010110001011110100 - 10000 	Idx(32): 20
1100010110001011000101 1100010110001011110100 10100
String: nmhg 	Idx(16): 13 	Bit value: 1100111010011011001101 - 0 	Idx(32): 13
1100111010011011111110 1100111010011011001101 1101
String: vfg6 	Idx(16): 8 	Bit value: 1101110010111101101000 - 0 	Idx(32): 8
1101110010111101011111 1101110010111101101000 1000
String: gfrt 	Idx(16): 1 	Bit value: 1100000101111101010001 - 10000 	Idx(32): 17
1100000101111101100001 1100000101111101010001 10001
String: alpo 	Idx(16): 7 	Bit value: 1011011011101101000111 - 0 	Idx(32): 7
1011011011101101101010 1011011011101101000111 111
String: vfbh 	Idx(16): 1 	Bit value: 1101110010111011000001 - 0 	Idx(32): 1
1101110010111011110110 1101110010111011000001 1
String: bnhj 	Idx(16): 0 	Bit value: 1011100011011001100000 - 0 	Idx(32): 0
1011100011011001001110 1011100011011001100000 0
String: zuio 	Idx(16): 8 	Bit value: 1110010011100110011000 - 10000 	Idx(32): 24
1110010011100110100001 1110010011100110011000 11000
String: iu8e 	Idx(16): 8 	Bit value: 1100010111100101101000 - 0 	Idx(32): 8
1100010111100101011001 1100010111100101101000 1000
String: yhjk 	Idx(16): 8 	Bit value: 1110001001010010101000 - 0 	Idx(32): 8
1110001001010010010000 1110001001010010101000 1000
String: plop 	Idx(16): 9 	Bit value: 1101001000110011101001 - 0 	Idx(32): 9
1101001000110011011101 1101001000110011101001 1001
String: dd0p 	Idx(16): 14 	Bit value: 1011101111001011101110 - 0 	Idx(32): 14
1011101111001011000000 1011101111001011101110 1110
```
### 4.3 数据迁移
这张图就是原 16 位长度数组元素，向新数组中转移的过程。 
- 其中黄色区域元素 zuio 因计算结果 hash & oldCap 不为 1，则被迁移到 下标位置 24。 
- 同时还是用重新计算哈希值的方式验证了，确实分配到 24 的位置，**因为 这是在二进制计算中补 1 的过程**，所以**可以通过上面简化的方式确定哈 希值的位置**。
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305012016475.png)

# 总结
如果能坚持看完这部分内容，并按照文中的例子进行相应的实验验证，那么一定可以学会涉及这五项知识点；==1、散列表实现、2、扰动函数、3、初始化容量、4、负载因子、5、扩容元素拆分==。 

对我个人来说以前也知道这部分知识，但是没有验证过，只知道概念如此，正好借着写面试手册专栏，加深学习，用数据验证理论，让知识点 可以更加深入的理解。


---
在上一章讲解并用数据验证了，HashMap 中的，散列表的实现、扰动函数、 负载因子以及扩容拆分等核心知识点以及作用。 除了以上这些知识点外，HashMap 还有基本的数据功能；**存储、删除、获取、遍历**， 在这些功能中经常会听到链表、红黑树、之间转换等功能。

而**红黑树是在 jdk1.8 引入到 HashMap 中解决链表过长问题的**，简单说当链表长度 $>=8$  时，将链表转换 位红黑树 ( 当 然 **这 里 还 有 一 个 扩 容 的 知 识 点** ， 不 一 定 都 会 树 化 `[MIN_TREEIFY_CAPACITY]` )。 

以下知识点； 
1. **数据插入**流程和源码分析 
2. **链表树化**以及**树转链表** 
3. 遍历过程中的无序 Set 的核心知识

# 1. 数据功能-插入
## 1.1 疑问点&考题 
通过上一章学习，对于一个散列表数据结构的 HashMap 往里面插入数据 时，基本已经有了一个印象。简单来说就是==通过你的 Key 值取得哈希再计算下标， 之后把相应的数据存放到里面==。 

但这个过程中会遇到一些问题，比如； 
1. 如果出现哈希值计算的下标碰撞了怎么办？ 
2. 如果**碰撞了是扩容数组**还是**把值存成链表结构**，让一个节点有多个值存放呢？ 
3. 如果**存放的数据的链表过长，就失去了散列表的性能了**，怎么办呢？ 
4. 如果想解决链表过长，什么时候使用树结构呢，**使用哪种树呢**？ 

这些疑问点都会在后面的内容中逐步讲解，也可以自己思考一下，如果是你来设 计，你会怎么做。
## 1.2 插入流程和源码分析
```java
/**  
 * Associates the specified value with the specified key in this map. 
 * If the map previously contained a mapping for the key, the old 
 * value is replaced. 
 * @param key key with which the specified value is to be associated  
 * @param value value to be associated with the specified key  
 * @return the previous value associated with {@code key}, or  
 *         {@code null} if there was no mapping for {@code key}.  
 *         (A {@code null} return can also indicate that the map  
 *         previously associated {@code null} with {@code key}.)  
 * 如果map中已经有了key对应的映射，则旧value会替换为新value，返回key关联的旧value，如果key之前没有映射，则返回null（也可能是key之前关联的是null）
 */
 public V put(K key, V value) {  
    return putVal(hash(key), key, value, false, true);  
}
```
### 1.2.1 HashMap插入数据流程图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305012027083.png)

以上就是 HashMap 中一个数据插入 `putVal` 的整体流程，包括了；计算下标、何时扩容、 何时链表转红黑树等，具体如下；
1. 首先进行哈希hashCode的扰动，获得一个新的哈希值 `hash` ：`(key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);`
2. 判断HashMap中的tab（表数组）是否为空、或长度为 $0$ ，**如果是则进行扩容操作**
	```java
	if ((tab = table) == null || (n = tab.length) == 0)  
		n = (tab = resize()).length;
	```
3. 根据哈希值计算下标 `(n - 1) & hash` ，**如果对应下标正好没有存放数据为 `null` ，就直接插入结点即可**：
	```java
	if ((p = tab[i = (n - 1) & hash]) == null)  
	    tab[i] = newNode(hash, key, value, null);
	```
	不然，**如果对应下标的结点 `p` 有数据、`hash` 相等、键相同（为 `null` 或同一个键，用 `==` 比较）或键相等，则覆盖**：
	```java
	Node<K, V> e; K k;
	if (p.hash == hash &&
		((k = p.key) == key || (!key != null && key.equals(k))))
			e = p;
	...
	```
	往非空地**覆盖值**的代码是**共用**的，根据要找到的结点位置（即已经存在过key的映射），查询旧值，插入新值——==**但后面还有内容**==：
	```java
	if (e != null) { // existing mapping for key  
	    V oldValue = e.value;  
	    if (!onlyIfAbsent || oldValue == null)  
	        e.value = value;  
	    afterNodeAccess(e);  // Callbacks to allow LinkedHashMap post-actions
	    return oldValue;  
	}
	```
4. 有存放数据、但不是要插入的位置时，判断 `tab[i]` 是否为树节点，**是则调用红黑树的插入方法、向树中插入结点**：
	```java
	else if (p instanceof TreeNode)  
	    e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value);
	```
	否则向链表中插入数据——遍历链表找到要插入的位置，并计算链表长度。如果对应键在链表中已存在，则**覆盖**，和上面一样，**此处不会触发链表长度变化**；否则**新增一个链表结点**，判断 `binCount >= TREEIFY_THRESHOLD-1 = 8-1` 即链表长度 $\ge 8$ ，是则将链表转换为红黑树 `treeifyBin(tab, hash)` ：
	```java
	for (int binCount = 0; ; ++binCount) {  
	    if ((e = p.next) == null) {  
	        p.next = newNode(hash, key, value, null);  
	        if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st  
	            treeifyBin(tab, hash);  
	        break;    
	    }  
	    if (e.hash == hash &&  
	        ((k = e.key) == key || (key != null && key.equals(k))))  
	        break;  
	    p = e;  
	}
	```
5. **新增元素可能会执行树化**，往tab的空处插入元素、覆盖原有元素都不会树化，无论是否树化，**最后所有元素处理完成后，判断总元素个数 `++size` 是否超过 `threshold` 扩容阈值 $> capacity \times loadFactor$** ，超过则扩容 `resize` 。
	```java
	if (++size > threshold)  
	    resize();
	```
6. `treeifyBin` 是一个链表转树的方法，但不是所有的链表长度为8后都会转为树，还需判断**存放 `key` 值的数组桶长度**是否小于 $64$ `MIN_TREEIFY_CAPACITY` 。**如果小于则需要扩容**，扩容后链表上的数据会被 拆分散列的相应的桶节点上，**也就把链表长度缩短了**。
7. 扩容时都执行 `resize` 方法。不用重新计算元素的hash（因为存入其中），发现树结点个数<6时会执行链化逻辑。

```java
/**  
 * The default initial capacity - MUST be a power of two. */static final int DEFAULT_INITIAL_CAPACITY = 1 << 4; // aka 16  
  
/**  
 * The maximum capacity, used if a higher value is implicitly specified * by either of the constructors with arguments. * MUST be a power of two <= 1<<30. */
static final int MAXIMUM_CAPACITY = 1 << 30;  
  
/**  
 * The load factor used when none specified in constructor. */
static final float DEFAULT_LOAD_FACTOR = 0.75f;  
  
/**  
 * The bin count threshold for using a tree rather than list for a * bin.  Bins are converted to trees when adding an element to a * bin with at least this many nodes. The value must be greater * than 2 and should be at least 8 to mesh with assumptions in * tree removal about conversion back to plain bins upon * shrinkage. */
static final int TREEIFY_THRESHOLD = 8;  
  
/**  
 * The bin count threshold for untreeifying a (split) bin during a * resize operation. Should be less than TREEIFY_THRESHOLD, and at * most 6 to mesh with shrinkage detection under removal. */
 static final int UNTREEIFY_THRESHOLD = 6;  
  
/**  
 * The smallest table capacity for which bins may be treeified. 
 * (Otherwise the table is resized if too many nodes in a bin.) 
 * Should be at least 4 * TREEIFY_THRESHOLD to avoid conflicts 
 * between resizing and treeification thresholds. 
 */
 static final int MIN_TREEIFY_CAPACITY = 64;
```
### 1.2.2 JDK1.8 HashMap的put方法源码如下
```java
public V put(K key, V value) {
	return ptVal(hash(key), key, value, false, true);
}
final V putVal(int hash, K key, V value, boolean onlyIfAbsent, boolean evict) {
	Node<K, V>[] tab; Node<K, V> p; int n, i;
	// 初始化桶数组tab，table被延迟到插入新数据时再进行初始化
	if ((tab == table) == null || (n = tab.length) == 0)
		n = (tab = resize()).length;
	// 如果桶中不包含键值对节点引用，则将新键值对节点的引用存入桶中即可
	if ((p = tab[i = (n - 1) & hash]) == null)
		tab[i] = newNode(hash, key, value, null);
	else {
		Node<K, V> e; K k;
		// 如果键的值以及节点hash等于链表(或树)中第一个键值对节点时，则将e指向该键值对
		if (p.hash == hash && 
			((k = p.key) == key || (key != null && key.equals(k))))
			e = p;
		// 如果桶中的引用类型为TreeNode, 则调用红黑树的插入方法
		else if (p instanceof TreeNode) 
			e = ((TreeNode<K, V>)p).putTreeVal(this, tab, hash, key, value);
		else {
			// 对链表进行遍历，并统计链表长度
			for (int binCount = 0; ; ++binCount) {
				// 链表中不包含要插入的键值对节点时，则将该节点接在链表的最后
				if ((e = p.next) == null) {
					p.next = newNode(hash, key, value, null);
					// 如果链表长度大于等于树化阈值，则进行树化操作
					if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
						treeifyBin(tab, hash);
					break;
				}
				// 条件为true,表示当前链表包含要插入的键值对，终止遍历
				if (e.hash == hash &&
					((k = e.key) == key || (key != null && key.equals(k))))
					break;
				p = e;
			}
		}
		// 判断需要插入的键值对是否存在于HashMap中
		if (e != null) { // existing mapping for key
			V oldValue = e.value;
			// onlyIfAbsent表示是否仅在oldValue为null的情况下，更新键值对的值
			if (!onlyIfAbsent || oldValue == null) 
				e.value = value;
			afterNodeAccess(e); // Callbacks to allow LinkedHashMap post-actions
			return oldValue;
		}
	}
	++modCount;
	// 键值对数量超过阈值时，则进行扩容
	if (++size > threshold) 
		resize();
	afterNodeInsertion(evict); // Callbacks to allow LinkedHashMap post-actions
	return null;
}
```
# 2. 扩容机制
> **新增元素可能会执行树化**，往tab的空处插入元素、覆盖原有元素都不会树化，无论是否树化，**最后所有元素处理完成后，判断总元素个数 `++size` 是否超过 `threshold` 扩容阈值 $> capacity \times loadFactor$** ，超过则扩容 `resize` 。

HashMap是基于数组+链表/红黑树实现的，但用于存放key值的数组桶的长度是固定的，由初始化（找到大于等于的2的幂）决定。那么随着数据的插入数量增加、以及负载因子确定的时空间平衡的作用下，就需要扩容来存放更多的数据。==扩容中有一个非常重要的点，就是jdk1.8中的优化操作，可以不需要再重新计算每个元素的哈希值，这已经说过==。机制如下图：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305021744629.png)
这里主要看**扩容代码**（要么初始化table，要么大小翻倍。
- 如果table为空或长度为0，则**初始化容量目标为threshold**（在数组未分配空间时，**threshold持有initial array capacity——在HashMap构造函数中为tableSizeFor(initialCapacity)即最初的桶数组大小（2的幂），或者是0、暗示DEFAULT_INITIAL_CAPACITY**，后来分配空间后应为 `loadFactory * capacity` 。
- 否则执行2的幂次扩展，每个桶中的元素要么在原地，要么加上一个2的幂的偏移：
```java
/*
 Initializes or doubles table size. If null, allocates in accord with initial capacity target held in field threshold. Otherwise, because we are using power-of-two expansion, the elements from each bin must either stay at same index, or move with a power of two offset in the new table.
 */
final Node<K, V>[] resize() {
	Node<K, V>[] oldTab = table;
	int oldCap = (oldTab == null) ? 0 : oldTab.length; // null:0,0:0,...
	int oldThr = threshold; // 旧的阈值
	int newCap, newThr = 0;
	// Cap是capacity的缩写，容量如果不为0，则说明已经初始化
	if (oldCap > 0) {
		// 如果旧容量达到最大1<<30则不再扩容
		if (oldCap >= MAXIMUM_CAPACITY) {
			threshold = Integer.MAX_VALUE; // 不会再超过负载因子*容量了
			return oldTab;
		}
		// 按旧容量(2的幂)和阈值的2倍计算新容量和阈值
		else if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY &&
			oldCap >= DEFAULT_INITIAL_CAPACITY)
			newThr = oldThr << 1; // double threshold
	}
	// initial capacity was placed in threshold 翻译：
	// HashMap使用threshold变量暂时保存initialCapacity参数的值(2的幂)
	// 初始化时，将threshold的值赋给newCap
	else if (oldThr > 0) // HashMap第一个构造传入initial_capacity,threshold=tableSizeFor... 
		newCap = oldThr; // 桶数组的初始化容量
	else { // zero initial threshold signifies using defaults
		// 这一部分也是，源码中也有相应英文注释
		// 调用无参构造(和传入一个loadFactor的构造)方法时，桶数组容量为默认容量 1 << 4; aka 16
		// threshold为0表示使用默认容量
		newCap = DEFAULT_INITIAL_CAPACITY;
		newThr = (int)(DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
		// 阈值：是默认容量与负载因子的乘积
	}
	// newThr为0，则使用阈值公式计算阈值
	if (newThr == 0) {
		float ft = (float)newCap * loadFactor;
		newThr = (newCap < MAXIMUM_CAPACITY && ft < (float)MAXIMUM_CAPACITY ?
			(int)ft : Integer.MAX_VALUE);
	}
	threshold = newThr;
	@SuppressWarnings({"rawtypes","unchecked"})  
	Node<K,V>[] newTab = (Node<K,V>[])new Node[newCap]; // 初始化新桶数组，用于存放key
	table = newTab; 
	if (oldTab != null) { // 如果旧数组桶非空
		for (int j = 0; j < oldCap; ++j) { // oldCap有值，则遍历将键值映射到新数组桶中 
			Node<K, V> e;
			if ((e = oldTab[j]) != null) {
				oldTab[j] = null; // 断开引用
				if (e.next == null) // 有一个值
					newTab[e.hash & (newCap - 1)] = e;
				else if (e instanceof TreeNode) 
					// 这里split是红黑树拆分操作，在重新映射时操作的
					((TreeNode<K, V>e).split(this, newTab, j, oldCap);
				else { // preserve order
					Node<K, V> loHead = null, loTail = null;
					Node<K, V> hiHead = null, hiTail = null;
					Node<K, V> next;
					// 这里是链表，如果当前是按照链表存放的，则将链表节点按照原顺序进行分组
					// 如何不需要重新计算哈希值进行拆分
					do {
						next = e.next;
						if ((e.hash & oldCap) == 0) { // 保留在同样的位置
							if (loTail == null) // 没有头结点
								loHead = e;
							else loTail.next = e; // 尾插
							loTail = e;
						}
						else { // 需要加oldCap
							if (hiTail == null)
								hiHead = e;
							else
								hiTail.next = e;
							hiTail = e;
						}
					} while ((e = next) != null);
					if (loTail != null) {
						loTail.next = null;
						newTab[j] = loHead; // 保留在原位置
					}
					if (hiTail != null) {
						hiTail.next = null;
						newTab[j + oldCap] = hiHead;
					}
				}
			} 
		}
	}
	return newTab;
}
```
代码整体的逻辑蛮清晰的，主要包括：
1. 扩容时计算出新的 `newCap, newThr` 
2. `newCap` 用于创建新的桶 `new Node[newCap];`
3. 随着扩容后，原先那些因为哈希碰撞，存放成链表和红黑树的元素，都需要进行拆分放到新的位置中。

# 3. 链表树化 
HashMap 这种散列表的数据结构，**最大的性能在于可以 O(1)时间复杂度定位到元 素**，但因为哈希碰撞不得已在一个下标里存放多组数据，==那么 jdk1.8 之前的设 计只是采用链表的方式进行存放，如果需要从链表中定位到数据时间复杂度就是 O(n)，链表越长性能越差==。
- 因为**在 jdk1.8 中把过长的链表也就是 8 个，优化为 自平衡的红黑树结构**，以此让定位元素的时间复杂度优化近似于 O(logn)，这样 来提升元素查找的效率。
- **但也不是完全抛弃链表**，因为在元素相对不多的情况下， **链表的插入速度更快**，所以综合考虑下**设定阈值为 8** （且容量>=MIN_TREEIFY_CAPACITY）才进行红黑树转换操作。 链表转红黑树，如下图
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305021835790.png)
以上就是一组链表转换为红黑树的情况，元素包括；40、51、62、73、84、95、 150、161 。这些是经过实际验证可分配到 Idx：12 的节点。通过这张图，基本可以有一个链表换行到红黑树的印象，接下来阅读下对应的源 码。
## 链表树化源码
```java
final void treeifyBin(Node<K,V>[] tab, int hash){
	int n, index; Node<K,V> e;
	// 这就是上面提到的，不一定树化，还可能只是扩容，主要桶数组容量是否小于64 MIN_TREEIFY_CAPACITY
	if (tab == null || (n = tab.length) < MIN_TREEIFY_CAPACITY)
		resize(); // 扩容
	else if ((e = tab[index = (n - 1) & hash]) != null) {
		// hd=head,tl=tile
		TreeNode<K,V> hd = null, tl = null;
		do {
			// 将普通结点转换为树结点，但此时还不是红黑树，即还不一定平衡
			TreeNode<K,V> p = replacementTreeNode(e, null);
			if (tl == null) {
				hd = p;
			} else {
				p.prev = tl;
				tl.next = p;
			}
			tl = p;
		} while ((e = e.next) != null); // 转成一条由TreeNode构成的双向链表
		if ((tab[index] = hd) != null) {
			// 转红黑树操作，需要循环比较、染色、旋转
			hd.treeify(tab);
		}
	}
}
```
这一部分链表树化的操作并不复杂，复杂点在于下一层的红黑树转换上，这部分 知识点会在后续章节中专门介绍； 以上源码主要包括的知识点如下；
1. 链表树化的条件有两点；**链表长度大于等于 8、桶容量大于等于 64**，否则只是扩容，不会树化。 
2. 链表树化的过程中是==先由链表转换为树节点，此时的树可能不是一颗平衡树==。同时 **在树转换过程中会记录原有链表的顺序**，tl.next = p，这主要**方便后续树转链表**和 **拆分** 更方便。 
3. **链表转换成树完成后，在进行红黑树的转换**。先简单介绍下，红黑树的转换需要染 色和旋转，以及**比对大小**。在比较元素的大小中，有一个比较有意思的方法， tieBreakOrder 加时赛，这主要是因为 ==HashMap 没有像 TreeMap 那样本身就 有 Comparator 的实现==。

Node节点里面记录了key的Hash值，单向指针 Map.Entry -> Node(next) -> LinkedHashMap.Entry(before, after) -> TreeNode(parent, left, right, prev) **才能记录原有链表的顺序**
```java
static class Node<K,V> implements Map.Entry<K,V> {  
    final int hash;  
    final K key;  
    V value;  
    Node<K,V> next;  
  
    Node(int hash, K key, V value, Node<K,V> next) {  
        this.hash = hash;  
        this.key = key;  
        this.value = value;  
        this.next = next;  
    }  
  
    public final K getKey()        { return key; }  
    public final V getValue()      { return value; }  
    public final String toString() { return key + "=" + value; }  
  
    public final int hashCode() {  
        return Objects.hashCode(key) ^ Objects.hashCode(value);  
    }  
  
    public final V setValue(V newValue) {  
        V oldValue = value;  
        value = newValue;  
        return oldValue;  
    }  
  
    public final boolean equals(Object o) {  
        if (o == this)  
            return true;  
  
        return o instanceof Map.Entry<?, ?> e  
                && Objects.equals(key, e.getKey())  
                && Objects.equals(value, e.getValue());  
    }  
}
```
# 4. 红黑树转链
在链表转红黑树中我们重点介绍了一句，**在转换树的过程中，记录了原有链表的顺序**。 那么，这就简单了，**红黑树转链表时候，直接把 TreeNode 转换为 Node 即可**，源 码如下；
```java
final Node<K,V> untreeify(HashMap<K,v> map) {
	Node<K,V> hd = null, tl = null;
	// 遍历TreeNode
	for (Node<K,V> q = this; q != null; q = q.next) {
		// TreeNode替换Node
		Node<K,V> p = map.replacement(q, null);
		if (tl == null) hd = p;
		else t1.next = p;
		tl = p;
	}
	return hd;
}
// 替换方法
Node<K,V> replacementNode(Node<K,V> p, Node<K,V> next) {
	return new Node<>(p.hash, p.key, p.value, next);
}
```
**因为记录了链表关系，所以替换过程很容易**。所以**好的数据结构可以让操作变得 更加容易**。

# 5. 查找
HashMap的查找很简单：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305022337576.png)
上图就是HashMap查找的一个流程图，比较简单，也是高效的：
```java
public V get(Object key) {
	Node<K,V> e;
	// 同样需要经过扰动函数计算哈希值
	return (e = getNode(hash(key), key)) == null ? null : e.value;
}
final Node<K,V> getNode(int hash, Object key) {
	Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
	// 判断桶数组是否为空、长度是否为0
	if ((tab = table) != null && (n = tab.length) > 0 && 
		// 计算下标，哈希值与数组长度-1
		(first = tab[(n - 1) & hash]) != null) {
		if (first.hash == hash && // always check first node
			((k = first.key) == key || (key != null && key.equals(k))))
			return first;
		if ((e = first.next) != null) {
			// TreeNode节点就直接调用红黑树的查找方法，时间复杂度O(logn)
			if (first instanceof TreeNode)
				return ((TreeNode<K,V>)first).getTreeNode(hash, key);
			// 如果是链表就依次遍历查找
			do {
				if (e.hash == hash &&
					((k = e.key) == key || (key != null && key.equals(k))))
					return e;
			} while ((e = e.next) != null);
		}
	}
	return null;
}
```
以上查找的代码还是比较简单的，主要包括以下知识点； 
1. 扰动函数的使用，获取新的哈希值，这在上一章节已经讲过 
2. 下标的计算，同样也介绍过 `tab[(n - 1) & hash])` 
3. 确定了桶数组下标位置，接下来就是对红黑树和链表进行查找和遍历操作了

# 6. 删除
删除的操作比较简单，里面都没有太多复杂的逻辑。因为红黑树的操作被包装了，只看使用上也很容易。
```java
public V remove(Object key) {
	Node<K,V> e;
	return (e = removeNode(hash(key), key, null, false, true)) == null ?
		null : e.value;
} // matchValue=false，就是不用匹配值是否相等
// matchValue – if true only remove if value is equal 
// movable – if false do not move other nodes while removing
final Node<K,V> removeNode(int hash, Object key, Object value, boolean matchValue, boolean movable) {
	Node<K,V>[] tab; Node<K,V> p; int n, index;
	// 定位桶数组中的下标位置，index=(n-1)&hash
	if ((tab = table) != null && (n = tab.length) > 0 &&
		(p = tab[index = (n - 1) & hash]) != null) {
		Node<K,V> node = null, e; K k; V v;
		// 如果键的值与链表第一个结点相等,则将node指向该结点
		if (p.hash == hash &&
			((k = p.key) == key || (key != null && key.equals(k)))) 
			node = p;
		else if ((e = p.next) != null) {
			// 树节点，调用红黑树的查找方法，定位节点
			if (p instanceof TreeNode) 
				node = ((TreeNode<K,V>)p).getTreeNode(hash, key);
			else {
				// 遍历链表，找到待删除结点
				do {
					if (e.hash == hash &&
						((k = e.key) == key ||
							(key != null && key.equals(k)))) {
						node = e;
						break;		
					}
					p = e;
				} while ((e = e.next) != null);
			}
		}
		// 删除节点，以及红黑树需要修复，因为删除后会破坏平衡性，链表的删除更加简单
		if (node != null && (!matchValue || (v = node.value) == value || 
			(value != null && value.equals(v)))) {
			if (node instanceof TreeNode)
				((TreeNode<K,V>)node).removeTreeNode(this, tab, movable);
			else if (node == p) // 是链表首
				tab[index] = node.next;
			else // p是node前驱
				p.next = node.next;
			++modCount;
			--size;
			afterNodeRemoval(node); // 这里是单向链表，所以是空方法，但可以被子类重写，比如LinkedHashMap就是用双向链表，要修改前后向节点的指针
			return node;
		}
	}
	return null;
}
```
# 7. 遍历
HashMap 中的遍历也是非常常用的 API 方法，包括； 
```java
// KeySet
for (String key : map.keySet()) { System.out.print(key + " "); } 
// EntrySet 
for (HashMap.Entry entry : map.entrySet()) { System.out.print(entry + " "); }
```
从方法上以及日常使用都知道，KeySet 遍历是无序的，但每次使用不同方式遍历包括 `keys.iterator()` ，**它们遍历的结果是固定的**。

那么从实现的角度来看，这些种遍历都是从散列表中的链表和红黑树获取集合值， 那么他们有一个什么固定的规律吗？
## 7.1 代码测试
测试的场景和前提； 
1. 这里我们要设定一个既有红黑树又有链表结构的数据场景 
2. 为了可以有这样的数据结构，**我们最好把 HashMap 的初始长度设定为 64，避免在链表 $\ge 8$ 位后扩容，而是直接让其转换为红黑树**。 
3. 找到 18 个元素，分别放在不同节点(这些数据通过程序计算得来)； 
	1. 桶数组 02 节点：24、46、68 
	2. 桶数组 07 节点：29 
	3. 桶数组 12 节点：150、172、194、271、293、370、392、491、590

```java
@Test 
public void test_Iterator() {  
    Map<String, String> map = new HashMap<String, String>(64);  
    map.put("24", "Idx：2");  
    map.put("46", "Idx：2");  
    map.put("68", "Idx：2");  
    map.put("29", "Idx：7");  
    map.put("150", "Idx：12");  
    map.put("172", "Idx：12");  
    map.put("194", "Idx：12");  
    map.put("271", "Idx：12"); // 没有>=8
  
    System.out.println("排序01：");  
    for (String key : map.keySet()) {  
        System.out.print(key + " ");  
    }  
  
    map.put("293", "Idx：12");  
    map.put("370", "Idx：12");  
    map.put("392", "Idx：12");  
    map.put("491", "Idx：12");  // >=8且桶数组容量>=MIN_TREEIFY_CAPACITY=64
    map.put("590", "Idx：12");  
  
    System.out.println("\n\n排序02：");  
    for (String key : map.keySet()) {  
        System.out.print(key + " ");  
    }                        
  
    map.remove("293");  
    map.remove("370");  
    map.remove("392");  
    map.remove("491");  
    map.remove("590");  
  
    System.out.println("\n\n排序03：");  
    for (String key : map.keySet()) {  
        System.out.print(key + " ");  
    }
}
```
这段代码分别测试了三种场景，如下；
1. 添加元素，在 HashMap **还是链表结构**时，输出测试结果 01 
2. 添加元素，在 HashMap **转换为红黑树**时，输出测试结果 02 
3. 删除元素，在 HashMap **转换为链表结构**时，输出测试结果 03

```java
排序01：
24 46 68 29 150 172 194 271 

排序02：
24 46 68 29 271 150 172 194 293 370 392 491 590 

排序03：
24 46 68 29 172 271 150 194 
```
从 map.keySet()测试结果可以看到，如下信息； 
1. 01 情况下，排序定位哈希值下标和链表信息
	![600](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305030055512.png)
2. 02 情况下，因为链表转换为红黑树，树根会移动到数组头部。 moveRootToFront()方法
	![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305030057975.png)
3. 03 情况下，因为删除了部分元素，红黑树退化成链表
	![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305030059391.png)
# 8. 总结
这一篇 API 源码**put、链表转树、扩容、树转链表、get、delete**以及逻辑与上一篇数据结构中**散列表实现、扰动函数、取数组下标、初始化、负载因子** 等，内容的结合，算是把 HashMap **基本常用技术点**，梳理完成了。但知识绝不止 于此，这里还有红黑树的相关技术内容，后续会进行详细。  除了 HashMap 以外还有 TreeMap、ConcurrentHashMap 等，每一个核心类都有一 些相关的核心知识点，每一个都非常值得深入研究。这个烧脑的过程，是学习获得 知识的最佳方式。


