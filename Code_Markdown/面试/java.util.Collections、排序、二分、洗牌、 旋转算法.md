关于数据结构中涉及的算法部分，这些主要来自于 Collections 类的实现部分。

当然，算法学习不只是刷题，还需要落地与应用，否则到了写代码的时候，还是会 for 循环+ifelse。

当开发一个稍微复杂点的业务流程时，往往要用到与之契合的数据结构和算法逻
辑，在与设计模式结合，这样既能让你的写出具有高性能的代码，也能让这些代
码具备良好的扩展性。

那么，有了这些数据结构的基础，接下来我们再学习一下 Java 中提供的算法工
具类，Collections。

# 一、面试题
谢飞机，今天怎么无精打采的呢，还有黑眼圈？
答：好多知识盲区呀，最近一直在努力补短板，还有面经手册里的数据结构。
问：那数据结构看的差不多了吧，你有考虑󠀀过，数据结构里涉及的排序、二分
查找吗？
答：二分查找会一些，巴拉巴拉。
问：还不错，那你知道这个方法在 Java 中有提供对应的工具类吗？是哪个！
答：这！？好像没注意过，没用过！
问：去吧，回家在看看书，这两天也休息下。

飞机悄然的出门了，但这次面试题整体回答的还是不错的，面试官决定下次再给他一个机会。

# 二、Collections 工具类
java.util.Collections，是 java 集合框架的一个工具类，主要用于 Collection
提供的通用算法；排序、二分查找、洗牌等算法操作。

从数据结构到具体实现，再到算法，整体的结构如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042117330.png)
## 1. Collections.sort 排序
### 1.1 初始化集合
```java
List<String> list = new ArrayList<String>();
list.add("7");
list.add("4");
list.add("8");
list.add("3");
list.add("9");
```
### 1.2 默认排列[正序]
```java
Collections.sort(list);
// 测试结果：[3, 4, 7, 8, 9]
```
### 1.3 Comparator 排序
```java
Collections.sort(list, new Comparator<String>() {  
    @Override  
    public int compare(String o1, String o2) {  
        return o2.compareTo(o1);  
    }  
});  
Collections.sort(list, (a, b) -> { return b.compareTo(a); });
// [9, 8, 7, 4, 3]
```
- 我们使用 o2 与 o1 做对比，这样会出来一个倒序排序。
- 同时，Comparator 还可以对对象类按照某个字段进行排序。

### 1.4 reverseOrder 倒排
```java
Collections.sort(list, Collections.<String>reverseOrder());
// 测试结果：[9, 8, 7, 4, 3]
```
- `Collections.<String>reverseOrder()` 的源码部分就和我们上面把两个对比的类调换过来一样。c2.compareTo(c1);

### 1.5 源码简述
关于排序方面的知识点并不少，而且有点复杂。本文主要介绍 Collections 集
合工具类，后续再深入每一个排序算法进行讲解。
#### `Collections.sort`
集合排序，最终使用的方法：
```java
Arrays.sort((E[]) elementData, 0, size, c);
```
这一部分排序逻辑包括了，旧版的归并排序 legacyMergeSort 和 TimSort
排序。

但因为开关的作用，LegacyMergeSort.userRequested = false，基本
都是走到 TimSort 排序 。
```java
public static <T> void sort(T[] a, int fromIndex, int toIndex,  
                            Comparator<? super T> c) {  
    if (c == null) {  
        sort(a, fromIndex, toIndex);  
    } else {  
        rangeCheck(a.length, fromIndex, toIndex);  
        if (LegacyMergeSort.userRequested)  
            legacyMergeSort(a, fromIndex, toIndex, c);  
        else            
	        TimSort.sort(a, fromIndex, toIndex, c, null, 0, 0);  
    }  
}
```
同时在排序的过程中还会因为**元素的个数是否大于 32**，而选择分段排序和二分插入排序。这一部分内容我们后续专门在排序内容讲解

## 2. Collections.binarySearch 二分查找
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042135230.png)
看到这张图熟悉吗，这就是集合元素中通过二分查找定位指定元素 5。
- 二分查找的前提是集合有序，否则不能满足二分算法的查找过程。
- 查找集合元素 5，在这个集合中分了三部；
	- 第一步，(0 + 7) >>> 1 = 3，定位 list.get(3) = 4，则继续向右侧二分查找。
	- 第二步，(4 + 7) >>> 1 = 5，定位 list.get(5) = 6，则继续向左侧二分查找。
	- 第三步，(4 + 4) >>> 1 = 4，定位 list.get(4) = 5，找到元素，返回结果。
### 2.1 功能使用
```java
@Test
public void test_binarySearch() {
	List<String> list = new ArrayList<String>();
	list.add("1");
	list.add("2");
	list.add("3");
	list.add("4");
	list.add("5");
	list.add("6");
	list.add("7");
	list.add("8");
	int idx = Collections.binarySearch(list, "5");
	System.out.println("二分查找：" + idx);
}
```
- 功能就是上图中的具体实现效果，通过 Collections.binarySearch 定位元素。
- 测试结果：二分查找：4

### 2.2 源码分析

```java
/**  
 * Searches the specified list for the specified object using the binary 
 * search algorithm.  The list must be sorted into ascending order 
 * according to the {@linkplain Comparable natural ordering} of its  
 * elements (as by the {@link #sort(List)} method) prior to making this  
 * call.  If it is not sorted, the results are undefined.  If the list 
 * contains multiple elements equal to the specified object, there is no 
 * guarantee which one will be found. 
 * 
 * <p>This method runs in log(n) time for a "random access" list (which  
 * provides near-constant-time positional access).  If the specified list 
 * does not implement the {@link RandomAccess} interface and is large,  
 * this method will do an iterator-based binary search that performs
 * O(n) link traversals and O(log n) element comparisons. 
 * 
 * @param  <T> the class of the objects in the list  
 * @param  list the list to be searched.  
 * @param  key the key to be searched for.  
 * @return the index of the search key, if it is contained in the list;  
 *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The  
 *         <i>insertion point</i> is defined as the point at which the  
 *         key would be inserted into the list: the index of the first 
 *         element greater than the key, or {@code list.size()} if all  
 *         elements in the list are less than the specified key.  Note 
 *         that this guarantees that the return value will be &gt;= 0 if  
 *         and only if the key is found. 
 * @throws ClassCastException if the list contains elements that are not  
 *         <i>mutually comparable</i> (for example, strings and  
 *         integers), or the search key is not mutually comparable *         with the elements of the list. 
 */
public static <T>  
int binarySearch(List<? extends Comparable<? super T>> list, T key) {  
    if (list instanceof RandomAccess || list.size()<BINARYSEARCH_THRESHOLD)  
        return Collections.indexedBinarySearch(list, key);  
    else        
	    return Collections.iteratorBinarySearch(list, key);  
}
```
这段二分查找的代码还是蛮有意思的，list instanceof RandomAccess 这是为什
么呢？**因为 ArrayList 有实现 RandomAccess**，但是 LinkedList 并没有实现这个接口。同时**还有元素数量阈值的校验 BINARYSEARCH_THRESHOLD = 5000，小于这个范围的都采用 indexedBinarySearch 进行查找**。那么这里其实使用LinkedList 存储数据，在元素定位的时候，需要 get 循环获取元素，就会比ArrayList 更耗时。

如果是实现了RandomAccess的类，就直接二分；否则看<5000，就indexedBinarySearch，>=5000就iteratorBinarySearch

返回-(low+1)是说，返回插入点，如果插入点为0，直接返回-low=-0=0，无法区分，所以返回 -low-1 。
```java
private static <T>  
int indexedBinarySearch(List<? extends Comparable<? super T>> list, T key) {  
    int low = 0;  
    int high = list.size()-1;  
  
    while (low <= high) {  
        int mid = (low + high) >>> 1;  
        Comparable<? super T> midVal = list.get(mid);  
        int cmp = midVal.compareTo(key);  
  
        if (cmp < 0)  
            low = mid + 1;  
        else if (cmp > 0)  
            high = mid - 1;  
        else            return mid; // key found  
    }  
    return -(low + 1);  // key not found  
}  
```
以上这段代码就是通过每次折半二分定位元素，而上面所说的耗时点就是
list.get(mid)，这在我们分析数据结构时已经讲过。

```java
private static <T>  
int iteratorBinarySearch(List<? extends Comparable<? super T>> list, T key)  
{  
    int low = 0;  
    int high = list.size()-1;  
    ListIterator<? extends Comparable<? super T>> i = list.listIterator();  
  
    while (low <= high) {  
        int mid = (low + high) >>> 1;  
        Comparable<? super T> midVal = get(i, mid);  
        int cmp = midVal.compareTo(key);  
  
        if (cmp < 0)  
            low = mid + 1;  
        else if (cmp > 0)  
            high = mid - 1;  
        else            
	        return mid; // key found  
    }  
    return -(low + 1);  // key not found  
}
```
上面这段代码是元素数量大于 5000 个，同时是 LinkedList 时会使用迭代器
list.listIterator 的方式进行二分查找操作。也算是一个优化，但是对于链表
的数据结构，仍然没有数组数据结构二分处理的速度快，主要在获取元素的时间复杂度上 O(1) 和 O(n)。

## 3. Collections.shuffle 洗牌算法
### 3.1 功能使用
洗牌算法，其实就是将 List 集合中的元素进行打乱，一般可以用在抽奖、摇号、洗牌等各个场景中。
```java
Collections.shuffle(list);
Collections.shuffle(list, new Random(100));
```
它的使用方式非常简单，主要有这么两种方式，一种直接传入集合、另外一种还可以传入固定的随机种子这种方式可以控制洗牌范围
### 3.2 源码分析
按照洗牌的逻辑，我们来实现下具体的核心逻辑代码，如下
```java
/**  
 * 洗牌算法  
 */  
@Test  
public void test_shuffle() {  
    List<String> list = new ArrayList<String>();  
    list.add("1");  
    list.add("2");  
    list.add("3");  
    list.add("4");  
    list.add("5");  
    list.add("6");  
    list.add("7");  
    list.add("8");  
    Random random = new Random();  
    for (int i = list.size(); i > 1; --i) {  
        int ri = random.nextInt(i); // 随机位置,从0-i  
        int ji = i - 1; // 顺延  
        System.out.println("ri: " + ri + " ji: " + ji);  
        list.set(ji, list.set(ri, list.get(ji))); // 元素置换  
    }  
    System.out.println(list);  
}
```
运行结果：
```java
ri：6 ji：7
ri：4 ji：6
ri：1 ji：5
ri：2 ji：4
ri：0 ji：3
ri：0 ji：2
ri：1 ji：1
[8, 6, 4, 1, 3, 2, 5, 7]
```
这部分代码逻辑主要是通过随机数**从逐步缩小范围的集合中找到对应的元素**，与递减的下标位置进行元素替换，整体的执行过程如下
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042245214.png)
实际上的源代码也是如下，只是同样区分了RandomAccess ：
```java
/**  
 * Randomly permute the specified list using the specified source of * randomness.  All permutations occur with equal likelihood * assuming that the source of randomness is fair.<p>  
 *  
 * This implementation traverses the list backwards, from the last element * up to the second, repeatedly swapping a randomly selected element into * the "current position".  Elements are randomly selected from the * portion of the list that runs from the first element to the current * position, inclusive.<p>  
 *  
 * This method runs in linear time.  If the specified list does not * implement the {@link RandomAccess} interface and is large, this  
 * implementation dumps the specified list into an array before shuffling * it, and dumps the shuffled array back into the list.  This avoids the * quadratic behavior that would result from shuffling a "sequential * access" list in place. * * @param  list the list to be shuffled.  
 * @param  rnd the source of randomness to use to shuffle the list.  
 * @throws UnsupportedOperationException if the specified list or its  
 *         list-iterator does not support the {@code set} operation.  
 */
@SuppressWarnings({"rawtypes", "unchecked"})  
public static void shuffle(List<?> list, Random rnd) {  
    int size = list.size();  
    if (size < SHUFFLE_THRESHOLD || list instanceof RandomAccess) {  
        for (int i=size; i>1; i--)  
            swap(list, i-1, rnd.nextInt(i));  
    } else {  
        Object[] arr = list.toArray();  
  
        // Shuffle array  
        for (int i=size; i>1; i--)  
            swap(arr, i-1, rnd.nextInt(i));  
  
        // Dump array back into list  
        // instead of using a raw type here, it's possible to capture        // the wildcard but it will require a call to a supplementary        // private method        ListIterator it = list.listIterator();  
        for (Object e : arr) {  
            it.next();  
            it.set(e);  
        }  
    }  
}
```
## 4. Collections.rotate 旋转算法
旋转算法，可以把 ArrayList 或者 Linkedlist，**从指定的某个位置开始，进行
正旋或者逆旋操作**。有点像把集合理解成圆盘，把要的元素转到自己这，其他的元素顺序跟随。
### 4.1 功能应用
```java
List<String> list = new ArrayList<String>();
list.add("7");
list.add("4");
list.add("8");
list.add("3");
list.add("9");
Collections.rotate(list, 2);
System.out.println(list);
```
这里我们将集合顺序；7、4、8、3、9，顺时针旋转 2 位，测试结果如下；**逆时针旋转为负数**
测试结果
```java
[3, 9, 7, 4, 8]
```
通过测试结果我们可以看到，从元素 7 开始，正向旋转了两位，执行效果如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042248789.png)
### 4.2 源码分析
```java
/**  
 * Rotates the elements in the specified list by the specified distance. * After calling this method, the element at index {@code i} will be  
 * the element previously at index {@code (i - distance)} mod  
 * {@code list.size()}, for all values of {@code i} between {@code 0}  
 * and {@code list.size()-1}, inclusive.  (This method has no effect on  
 * the size of the list.) * * <p>For example, suppose {@code list} comprises{@code  [t, a, n, k, s]}.  
 * After invoking {@code Collections.rotate(list, 1)} (or  
 * {@code Collections.rotate(list, -4)}), {@code list} will comprise  
 * {@code [s, t, a, n, k]}.  
 * * <p>Note that this method can usefully be applied to sublists to  
 * move one or more elements within a list while preserving the * order of the remaining elements.  For example, the following idiom * moves the element at index {@code j} forward to position  
 * {@code k} (which must be greater than or equal to {@code j}):  
 * <pre>  
 *     Collections.rotate(list.subList(j, k+1), -1);  
 * </pre>  
 * To make this concrete, suppose {@code list} comprises  
 * {@code [a, b, c, d, e]}.  To move the element at index {@code 1}  
 * ({@code b}) forward two positions, perform the following invocation:  
 * <pre>  
 *     Collections.rotate(l.subList(1, 4), -1);  
 * </pre>  
 * The resulting list is {@code [a, c, d, b, e]}.  
 * * <p>To move more than one element forward, increase the absolute value  
 * of the rotation distance.  To move elements backward, use a positive * shift distance. * * <p>If the specified list is small or implements the {@link  
 * RandomAccess} interface, this implementation exchanges the first  
 * element into the location it should go, and then repeatedly exchanges * the displaced element into the location it should go until a displaced * element is swapped into the first element.  If necessary, the process * is repeated on the second and successive elements, until the rotation * is complete.  If the specified list is large and doesn't implement the * {@code RandomAccess} interface, this implementation breaks the  
 * list into two sublist views around index {@code -distance mod size}.  
 * Then the {@link #reverse(List)} method is invoked on each sublist view,  
 * and finally it is invoked on the entire list.  For a more complete * description of both algorithms, see Section 2.3 of Jon Bentley's * <i>Programming Pearls</i> (Addison-Wesley, 1986).  
 * * @param list the list to be rotated.  
 * @param distance the distance to rotate the list.  There are no  
 *        constraints on this value; it may be zero, negative, or *        greater than {@code list.size()}.  
 * @throws UnsupportedOperationException if the specified list or  
 *         its list-iterator does not support the {@code set} operation.  
 * @since 1.4  
 */
public static void rotate(List<?> list, int distance) {  
    if (list instanceof RandomAccess || list.size() < ROTATE_THRESHOLD)  
        rotate1(list, distance);  
    else        rotate2(list, distance);  
}  
```
关于旋转算法的实现类分为两部分；
1. Arraylist 或者元素数量不多时，ROTATE_THRESHOLD = 100，则通过算法rotate1 实现。
2. **如果是 LinkedList 元素数量又超过了 ROTATE_THRESHOLD**，则需要使用算法 rotate2 实现。

那么，这两个算法有什么不同呢？
```java
private static <T> void rotate1(List<T> list, int distance) {  
    int size = list.size();  
    if (size == 0)  
        return;  
    distance = distance % size;  
    if (distance < 0)  
        distance += size;  
    if (distance == 0)  
        return;  
  
    for (int cycleStart = 0, nMoved = 0; nMoved != size; cycleStart++) {  
        T displaced = list.get(cycleStart);  
        int i = cycleStart;  
        do {  
            i += distance;  
            if (i >= size)  
                i -= size;  
            displaced = list.set(i, displaced);  
            nMoved ++;  
        } while (i != cycleStart);  
    }  
}  
```
这部分代码看着稍微多一点，但是数组结构的操作起来并不复杂，基本如上面圆圈图操作，主要包括以下步骤；
1. distance = distance % size;，获取旋转的位置。如果旋转后和当前一样即distance=0，则直接返回
2. for 循环和 dowhile，配合每次的旋转操作，比如这里第一次会把 0 位置元素替换到 2 位置，接着在 dowhile 中会判断 i != cycleStart，满足条件继续把替换了 2 位置的元素继续往下替换。**直到一轮循环把所有元素都放置到正确位置**。
3. 回到 cycyleStart后，如果还有元素没有移动即 nMoved != size，就cycleStart++，再继续do-while循环
4. 最终 list 元素被循环替换完成，也就相当从某个位置开始，正序旋转 2 个位置的操作。

接下来这部分源码主要是针对大于 100 个元素的 LinkedList 进行操作，所以整
个算法也更加有意思，它的主要操作包括；
1. **定位拆链位置**，-distance % size + size，也就是我们要旋转后找到的元素位置
2. **第一次翻转**，把位置 0 到拆链位置旋转
3. 第二次翻转，把拆链位置到结尾旋转
4. 第三次翻转，翻转整个链表

整体执行过程，可以参考下图，更方便理解；
```java
private static void rotate2(List<?> list, int distance) {  
    int size = list.size();  
    if (size == 0)  
        return;  
    int mid =  -distance % size;  
    if (mid < 0)  
        mid += size;  
    if (mid == 0)  
        return;  
  
    reverse(list.subList(0, mid));  
    reverse(list.subList(mid, size));  
    reverse(list);  
}
```
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042258929.png)
# 5. 其他 API 介绍
这部分 API 内容，使用和设计上相对比较简单，平时可能用的时候不多，但有的
小伙伴还没用过，就当为你扫盲了。
## 5.1 最大最小值
```java
String min = Collections.min(Arrays.asList("1", "2", "3"));
String max = Collections.max(Arrays.asList("1", "2", "3"));
```
`Collections` 工具包可以进行最值的获取。
## 5.2 元素替换
```java
List<String> list = new ArrayList<String>();
list.add("7");
list.add("4");
list.add("8");
list.add("8");
Collections.replaceAll(list, "8", "9");
// 测试结果： [7, 4, 9, 9]
```
## 5.3 连续集合位置判断
```java
@Test
public void test_indexOfSubList() {
	List<String> list = new ArrayList<String>();
	list.add("7");
	list.add("4");
	list.add("8");
	list.add("3");
	list.add("9");
	int idx = Collections.indexOfSubList(list, Arrays.asList("8", "3"));
	System.out.println(idx);
	// 测试结果：2
}
```
在使用 String 操作中，我们知道 `"74839".indexOf("8");` ，可以获取某个元素在什么位置。而在 ArrayList 集合操作中，可以获取连续的元素，在集合中的位置。
### 5.4 synchronized
```java
List<String> list = Collections.synchronizedList(new ArrayList<String>());
Map<String, String> map = Collections.synchronizedMap(new HashMap<String, String>()
);
```
这个很熟悉吧，甚至经常使用，但可能会忽略**这些线程安全的方法来自于
Collections 集合工具包**。
# 总结
本章节基本将 java.util.Collections 工具包中的常用方法介绍完了，以及
一些算法的讲解。这样在后续需要使用到这些算法逻辑时，就可以直接使用并不需要重复造轮子。

学习数据结构与算法、设计模式，这三方面的知识，重点还是能落地到日常的业务开发中，否则空、假、虚，只能适合吹吹牛，并不会给项目研发带来实际的价值。

**懂了就是真的懂，别让自己太难受**。死记硬背谁也受不了，耗费了大量的时间，知识也没有吸收，学习一个知识点最好就从根本学习，不要心浮气躁。