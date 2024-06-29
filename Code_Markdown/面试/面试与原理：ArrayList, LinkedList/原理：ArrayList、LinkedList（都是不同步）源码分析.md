数据结构是写好代码的基础！ 

说到数据结构基本包括；数组、链表、队列、红黑树等，但当你看到这些数据结 构以及想到自己平时的开发，似乎并没有用到过。那么为什么还要学习数据结构？ 其实这些知识点你并不是没有用到的，而是 Java 中的 API 已经将各个数据结构 封装成对应的工具类，例如 ArrayList、LinkedList、HashMap 等，就像在前面 的章节中，小傅哥写了 5 篇文章将近 2 万字来分析 HashMap，从而学习它的核心 设计逻辑。

可能有人觉得这类知识就像八股文，学习只是为了应付面试。如果你真的把这些 用于支撑其整个语言的根基当八股文学习，那么硬背下来不会有多少收获。理科 学习更在乎逻辑，重在是理解基本原理，有了原理基础就复用这样的技术运用到 实际的业务开发。 那么，你什么时候会用到这样的技术呢？就是，==当你考虑体量、夯实服务、琢磨 性能时，就会逐渐的深入到数据结构以及核心的基本原理当中，那里的每一分深 入，都会让整个服务性能成指数的提升==。 

# 一、面试题 
谢飞机，听说你最近在家很努力学习 HashMap？那考你个 ArrayList 知识点 你看下面这段代码输出结果是什么？ 虽然已经构造了容量为10的数组，但add的下标超出了size：
```java
public static void main(String[] args) { 
	List list = new ArrayList(10); 
	list.add(2, "1"); 
	System.out.println(list.get(0)); 
} 
```
嗯？不知道！👀眼睛看题，看我脸干啥？好好好，告诉你吧，这样会报错！至于 为什么，回家看看书吧。 
```
Exception in thread "main" java.lang.IndexOutOfBoundsException: Index: 2, Size: 0 at java.util.ArrayList.rangeCheckForAdd(ArrayList.java:665) at java.util.ArrayList.add(ArrayList.java:477) at org.itstack.interview.test.ApiTest.main(ApiTest.java:13) Process finished with exit code 1
```

# 二、数据结构 **Array + List = 数组 + 列表 = ArrayList = 数组列表**
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305031932430.png)
ArrayList 的数据结构是基于数组实现的，只不过这个数组不像我们普通定义的 数组，它可以**在 ArrayList 的管理下插入数据时按需动态扩容、数据拷贝等操作**。 接下来，我们就逐步分析 ArrayList 的源码，也同时解答谢飞机的疑问。

# 三、源码分析
## 3.1 重要参数
```java
/**  
 * Default initial capacity. 
 */
private static final int DEFAULT_CAPACITY = 10;  
  
/**  
 * Shared empty array instance used for empty instances. 
 */
 private static final Object[] EMPTY_ELEMENTDATA = {};  
  
/**  
 * Shared empty array instance used for default sized empty instances. We 
 * distinguish this from EMPTY_ELEMENTDATA to know how much to inflate when 
 * first element is added. 
 */
 private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};  
  
/**  
 * The array buffer into which the elements of the ArrayList are stored. 
 * The capacity of the ArrayList is the length of this array buffer. Any 
 * empty ArrayList with elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA 
 * will be expanded to DEFAULT_CAPACITY when the first element is added. 
 */
 transient Object[] elementData; // non-private to simplify nested class access  
  
/**  
 * The size of the ArrayList (the number of elements it contains). * * @serial  
 */  
private int size;
```
size是元素的个数，容量是 `elementData` 的长度。默认容量是 `DEFAULT_CAPACITY = 10` ，当实例为空（调用空构造函数，很常见；或者传递参数为0），实例化一个空ArrayList。此时各个实例对象共享一个Object数组 `EMPTY_ELEMENTDATA` 。
Shared empty array instance used for empty instance

 * Shared empty array instance used for default sized empty instances. We 
 * distinguish this from EMPTY_ELEMENTDATA to know how much to inflate when 
 * first element is added. 
## 3.2 初始化
```java
/**  
 * Constructs an empty list with the specified initial capacity. 
 * @param  initialCapacity  the initial capacity of the list  
 * @throws IllegalArgumentException if the specified initial capacity  
 *         is negative 
 */
 public ArrayList(int initialCapacity) {  
    if (initialCapacity > 0) {  
        this.elementData = new Object[initialCapacity];  
    } else if (initialCapacity == 0) {  
        this.elementData = EMPTY_ELEMENTDATA;  
    } else {  
        throw new IllegalArgumentException("Illegal Capacity: "+  
	        initialCapacity);  
    }  
}  
  
/**  
 * Constructs an empty list with an initial capacity of ten. */
 public ArrayList() {  
    this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;  
}

  
/**  
 * Constructs a list containing the elements of the specified 
 * collection, in the order they are returned by the collection's iterator. 
 * @param c the collection whose elements are to be placed into this list  
 * @throws NullPointerException if the specified collection is null  
 */
public ArrayList(Collection<? extends E> c) {  
    Object[] a = c.toArray();  
    if ((size = a.length) != 0) {  
        if (c.getClass() == ArrayList.class) {  
            elementData = a;  
        } else {  
            elementData = Arrays.copyOf(a, size, Object[].class);  
        }  
    } else {  
        // replace with empty array.  
        elementData = EMPTY_ELEMENTDATA;  
    }  
}
```
- 通常情况**空构造函数**初始化 ArrayList 更常用，这种方式数组的长度会**在第一次插入数据时候进行设置、进行数组初始化**，即构造ArrayList时会将elementData指向 `DEFAULTCAPACITY_EMPTY_ELEMENTDATA` （相当于构造一个大小为10的List，只是得等到第一次add元素才能真正构造数组）——Any empty ArrayList with elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA  will be expanded to DEFAULT_CAPACITY when the first element is added. DEFAULTCAPACITY_EMPTY_ELEMENTDATA和EMPTY_ELEMENTDATA区分在于，前者用于初次扩容时分别要不要扩容到DEFAULT_CAPACITY，后者则用于其他场景
- 另外，`EMPTY_ELEMENTDATA` 是一个定义好的空对象；`private static final Object[] EMPTY_ELEMENTDATA = {}` ，在有参构造函数传递进0、传进一个空集合时指向它。
- 当我们已经知道要填充多少个元素到 ArrayList 中，比如 500 个、1000 个，那么**为 了提供性能，减少 ArrayList 中的拷贝操作，这个时候会直接初始化一个预先设定 好的长度**。 
- 还可以传递一个集合进来。为空集合时，同样共享 `EMPTY_ELEMENTDATA` 。

通过构造函数可以看到，只要实现 `Collection` 类的都可以作为入参。再通过 `toArray()` 转为数组，如果非空，则判断是否为传入的集合是否为ArrayList.class，是则直接将 `elementData` 指向 `a` ；否则拷贝 `Arrays.copyOf` 到 `Object[]` 再赋值给 `elementData` 。

以前的一个版本的构造函数：
```java
public ArrayList(Collection<? extends E> c) { 
	elementData = c.toArray(); 
	if ((size = elementData.length) != 0) {
		// c.toArray might (incorrectly) not return Object[] (see 6260652) 
		if (elementData.getClass() != Object[].class) 
			elementData = Arrays.copyOf(elementData, size, Object[].class); 
	} else { 
		// replace with empty array. 
		this.elementData = EMPTY_ELEMENTDATA; 
	}  
}
```
注意：`c.toArray might (incorrectly) not return Object[] (see 6260652)` see 6260652 是 JDK bug 库 的 编 号 ， 有 点 像 商 品 sku ， bug 地 址:https://bugs.java.com/bugdatabase/view_bug.do?bug_id=6260652 那这是个什么 bug 呢，我们来测试下面这段代码；
- public Object[] toArray() 返回的类型不一定就是 Object[]，其类型 取决于其返回的实际类型，毕竟 Object 是父类，它可以是其他任意类型。 
- 子类实现和父类同名的方法，仅仅返回值不一致时，默认调用的是子类的实现方法。
- 造成这个结果的原因，如下； 
	1. Arrays.asList 使用的是：`Arrays.copyOf(this.a, size,(Class) a.getClass());`  
	2. ArrayList 构造函数使用的是：`Arrays.copyOf(elementData, size, Object[].class);`
```java
@Test  
public void t() {  
    List<Integer> list1 = Arrays.asList(1, 2, 3);  
    System.out.println("通过数组转换：" + (list1.toArray().getClass() == Object[].class));  
  
    ArrayList<Integer> list2 = new ArrayList<Integer>(Arrays.asList(1, 2, 3));  
    System.out.println("通过集合转换：" + (list2.toArray().getClass() == Object[].class));  
}
```
**之前是 `false, true` ，现在都是 `true`** 。原因是：Arrays.asList内部是直接返回 `new ArrayList<>(a)` ，这个ArrayList不是那个ArrayList。**Arrays.asList 构建出来的 List 与 new ArrayList 得 到的 List，压根就不是一个 List**！而且 ==Arrasys 下的 List 是一个私有类，只能通过 asList 使用，不能单独创建==。类关系图如下；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305040248121.png)
Arrays.asList 内部调用Arrays.ArrayList的构造方法。将该集合传进util.ArrayList后调用集合的toArray方法，其实是调用Arrays.ArrayList.toArray()，而里面其实是——因此现在返回的都是 `Object[]` ：
```java
@Override  
public Object[] toArray() {  
    return Arrays.copyOf(a, a.length, Object[].class);  
}
```
如果调用 `Arrays.ArrayList.toArray(T[] a)` ，里面就是调用：
```java
if (a.length < size)  
    return Arrays.copyOf(this.a, size,  
                         (Class<? extends T[]>) a.getClass());  
System.arraycopy(this.a, 0, a, 0, size);
```
此外：
- Arrays.asList 构建的集合，不能赋值给 ArrayList 
- Arrays.asList 构建的集合，不能再添加元素 
- Arrays.asList 构建的集合，不能再删除元素
> 还有这个 ArrayList 不能添加和删除，主要是因为它的实现方式，可以参考 Arrays 类中，这部分源码；private static class ArrayList extends AbstractList implements RandomAccess, java.io.Serializable


### 3.2.1 方式 01；普通方式 
```java
ArrayList list = new ArrayList(); 
list.add("aaa"); 
list.add("bbb"); 
list.add("ccc"); 
```
这个方式很简单也是我们最常用的方式。 
### 3.2.2 方式 02；内部类方式 
```java
ArrayList list = new ArrayList() { 
	add("aaa"); add("bbb"); add("ccc"); 
}; 
```
这种方式也是比较常用的，而且省去了多余的代码量。 
### 3.2.3 方式 03；
```java
Arrays.asList ArrayList list = new ArrayList(Arrays.asList("aaa", "bbb", "ccc")) ;
``` 
以上是通过 Arrays.asList 传递给 ArrayList 构造函数的方式进行初始化。这里 有几个知识点；
### 3.2.4 方式 04；Collections.ncopies 
`Collections.nCopies` 是集合方法中用于生成多少份某个指定元素的方法，接下 来就用它来初始化 ArrayList，如下； 
```java
ArrayList list = new ArrayList(Collections.nCopies(10, 0));
```
这会初始化一个由 10 个 0 组成的集合

# 4. 插入
ArrayList 对元素的插入，其实就是对数组的操作，只不过需要特定时候扩容。
## 4.1 普通插入 
```java
List list = new ArrayList(); list.add("aaa"); list.add("bbb"); list.add("ccc");
```
当我们依次插入添加元素时，ArrayList.add 方法只是把元素记录到数组的各个位置上了，源码如下；
```java
/**  
 * This helper method split out from add(E) to keep method 
 * bytecode size under 35 (the -XX:MaxInlineSize default value), 
 * which helps when add(E) is called in a C1-compiled loop. 
 */
 private void add(E e, Object[] elementData, int s) {  
    if (s == elementData.length)  
        elementData = grow();  
    elementData[s] = e;  
    size = s + 1;  
}  
  
/**  
 * Appends the specified element to the end of this list. * * @param e element to be appended to this list  
 * @return {@code true} (as specified by {@link Collection#add})  
 */
 public boolean add(E e) {  
    modCount++;  
    add(e, elementData, size);  
    return true;}  
```
要么添加到末尾，如果 `size==elementData.length` 就要 `grow` 扩容。
## 4.2 插入时扩容 
在前面初始化部分讲到，**ArrayList 默认初始化时会申请 10 个长度的空间，如果 超过这个长度则需要进行扩容**，那么它是怎么扩容的呢？ 

从根本上分析来说，数组是定长的，如果超过原来定长长度，**扩容则需要申请新 的数组长度，并把原数组元素拷贝到新数组中**，如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305040316475.png)
图中介绍了当 List 结合可用空间长度不足时则需要扩容，这主要包括如下步骤； 
1. 判断长度充足；`ensureCapacityInternal(size + 1);` 或者说
	```java
	if (s == elementData.length)  
	    elementData = grow();
	...
	if ((s = size) == (elementData = this.elementData).length)  
	    elementData = grow();
	```
1. 当判断长度不足时，则通过扩大函数，进行扩容；`grow(int minCapacity)` 
2. 扩容的长度计算；`int newCapacity = oldCapacity + (oldCapacity >> 1);` ，旧容量 + 旧容量右移 1 位，这相当于扩容了原来容 量的 `(int)3/2` 。 10扩容时：`1010 + 1010 >> 1 = 1010 + 0101 = 10 + 5 = 15` 。7扩容时：0111 + 0111 >> 1 = 0111 + 0011 = 7 + 3 = 10 
3. 当扩容完以后，就**需要进行把数组中的数据拷贝到新数组**中，这个过程会用到 Arrays.copyOf(elementData, newCapacity);，但他的底层用到的 是；System.arraycopy

```java
/**  
 * Increases the capacity to ensure that it can hold at least the 
 * number of elements specified by the minimum capacity argument.
 * @param minCapacity the desired minimum capacity  
 * @throws OutOfMemoryError if minCapacity is less than zero  
 */private Object[] grow(int minCapacity) {  
    int oldCapacity = elementData.length;  
    if (oldCapacity > 0 || elementData != DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {  
        int newCapacity = ArraysSupport.newLength(oldCapacity,  
                minCapacity - oldCapacity, /* minimum growth */  
                oldCapacity >> 1           /* preferred growth */);  
        return elementData = Arrays.copyOf(elementData, newCapacity);  
    } else {  
        return elementData = new Object[Math.max(DEFAULT_CAPACITY, minCapacity)];  
    }  
}  
  
private Object[] grow() {  
    return grow(size + 1);  
}
```
### 4.2.1 System.arraycopy
```java
@Test  
public void test_arraycopy() {  
    int[] oldArr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  
    int[] newArr = new int[oldArr.length + (oldArr.length >> 1)];  
  
    System.arraycopy(oldArr, 0, newArr, 0, oldArr.length);  
  
    newArr[11] = 11;  
    newArr[12] = 12;  
    newArr[13] = 13;  
    newArr[14] = 14;  
  
    System.out.println("数组元素：" + JSON.toJSONString(newArr));  
    System.out.println("数组长度：" + newArr.length);  
  
    /**  
     * 测试结果  
     *  
     * 数组元素：[1,2,3,4,5,6,7,8,9,10,0,11,12,13,14]  
     * 数组长度：15  
     */
}
```
- 拷贝数组的过程并不复杂，主要是对 System.arraycopy 的操作。 
- 上面就是把数组 oldArr 拷贝到 newArr，同时新数组的长度，采用和 ArrayList 一样的计算逻辑；oldArr.length + (oldArr.length >> 1)
## 4.3 指定位置插入 
```java
list.add(2, "1");
``` 
到这，终于可以说说谢飞机的面试题，这段代码输出结果是什么，如下； 
```java
Exception in thread "main" java.lang.IndexOutOfBoundsException: Index: 2, Size: 0 at java.util.ArrayList.rangeCheckForAdd(ArrayList.java:665) at java.util.ArrayList.add(ArrayList.java:477) at org.itstack.interview.test.ApiTest.main(ApiTest.java:14) 
```
### 4.3.1 size验证
其实，一段报错提示，为什么呢？我们翻开下源码学习下。 
```java
/**  
 * Inserts the specified element at the specified position in this
 * list. Shifts the element currently at that position (if any) and 
 * any subsequent elements to the right (adds one to their indices). 
 * 
 * @param index index at which the specified element is to be inserted  
 * @param element element to be inserted  
 * @throws IndexOutOfBoundsException {@inheritDoc}  
 */
 public void add(int index, E element) {  
    rangeCheckForAdd(index);  
    modCount++;  
    final int s;  
    Object[] elementData;  
    // 判断是否需要扩容以及扩容操作
    if ((s = size) == (elementData = this.elementData).length)  
        elementData = grow();  
    // 数据拷贝迁移，把待插入位置空出来
    System.arraycopy(elementData, index,  
                     elementData, index + 1,  
                     s - index);  
    // 数据插入操作
    elementData[index] = element;  
    size = s + 1;  
}

private void rangeCheckForAdd(int index) { 
	if (index > size || index < 0)
		throw new IndexOutOfBoundsException(outOfBoundsMsg(index)); 
}
```
- 指定位置插入首先要判断 rangeCheckForAdd，size 的长度。 
- 通过上面的元素插入我们知道，**每插入一个元素，size 自增一次 size++**。 
- 所以即使我们申请了 10 个容量长度的 ArrayList，但是指定位置插入会依赖于 size 进行判断，所以会抛出 IndexOutOfBoundsException 异常。

### 4.3.2 元素迁移
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041219984.png)
指定位置插入的核心步骤包括； 
1. 判断 size，是否可以插入。 
2. 判断插入后是否需要扩容；~~ensureCapacityInternal(size + 1);~~（好像没有这个方法了，ensureCapacity也没在里面用过了）。 
3. **数据元素迁移，把从待插入位置后的元素，顺序往后迁移**。
4. 给数组的指定位置赋值，也就是把待插入元素插入进来。

### 4.3.3 实践
指定位置已经插入元素 1，后面的数据向后迁移完成。
```java
public static void main(String[] args) {  
	List<String> list = new ArrayList<String>(Collections.nCopies(9, "a"));  
	System.out.println("初始化：" + list);  

	list.add(2, "b");  
	System.out.println("插入后：" + list);   
}
/**
测试结果： 初始化：[a, a, a, a, a, a, a, a, a] 
插入后：[a, a, 1, a, a, a, a, a, a, a] Process finished with exit code 0
*/
```
### addAll
5. `addAll(Collection c)` 没有元素时，扩容为 `Math.max(10, 实际元素个数)` ，有元素或之前没元素但 `elementData != DEFAULTCAPACITY_EMPTY_ELEMENTDATA` 时为 `Math.max(原容量 1.5 倍, 实际元素个数=已有+新加)`（**一次性扩容到最大、能容纳传入的所有元素**，而不是多次反复扩容）
```java
public boolean addAll(Collection<? extends E> c) {  
    Object[] a = c.toArray();  
    modCount++;  
    int numNew = a.length;  
    if (numNew == 0)  
        return false;  
    Object[] elementData;  
    final int s;  
    if (numNew > (elementData = this.elementData).length - (s = size))  
        elementData = grow(s + numNew);  
    System.arraycopy(a, 0, elementData, s, numNew);  
    size = s + numNew;  
    return true;
}

/**
 * Increases the capacity to ensure that it can hold at least the
 * number of elements specified by the minimum capacity argument.
 *
 * @param minCapacity the desired minimum capacity
 * @throws OutOfMemoryError if minCapacity is less than zero
 */
private Object[] grow(int minCapacity) {
    int oldCapacity = elementData.length;
    if (oldCapacity > 0 || elementData != DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
        int newCapacity = ArraysSupport.newLength(oldCapacity,
                minCapacity - oldCapacity, /* minimum growth */
                oldCapacity >> 1           /* preferred growth */);
        return elementData = Arrays.copyOf(elementData, newCapacity);
    } else {
        return elementData = new Object[Math.max(DEFAULT_CAPACITY, minCapacity)];
    }
}

/**  
 * Computes a new array length given an array's current length, a minimum growth * amount, and a preferred growth amount. The computation is done in an overflow-safe * fashion. * * This method is used by objects that contain an array that might need to be grown * in order to fulfill some immediate need (the minimum growth amount) but would also * like to request more space (the preferred growth amount) in order to accommodate * potential future needs. The returned length is usually clamped at the soft maximum * length in order to avoid hitting the JVM implementation limit. However, the soft * maximum will be exceeded if the minimum growth amount requires it. * * If the preferred growth amount is less than the minimum growth amount, the * minimum growth amount is used as the preferred growth amount. * * The preferred length is determined by adding the preferred growth amount to the * current length. If the preferred length does not exceed the soft maximum length * (SOFT_MAX_ARRAY_LENGTH) then the preferred length is returned. * * If the preferred length exceeds the soft maximum, we use the minimum growth * amount. The minimum required length is determined by adding the minimum growth * amount to the current length. If the minimum required length exceeds Integer.MAX_VALUE, * then this method throws OutOfMemoryError. Otherwise, this method returns the greater of * the soft maximum or the minimum required length. * * Note that this method does not do any array allocation itself; it only does array * length growth computations. However, it will throw OutOfMemoryError as noted above. * * Note also that this method cannot detect the JVM's implementation limit, and it * may compute and return a length value up to and including Integer.MAX_VALUE that * might exceed the JVM's implementation limit. In that case, the caller will likely * attempt an array allocation with that length and encounter an OutOfMemoryError. * Of course, regardless of the length value returned from this method, the caller * may encounter OutOfMemoryError if there is insufficient heap to fulfill the request. * * @param oldLength   current length of the array (must be nonnegative)  
 * @param minGrowth   minimum required growth amount (must be positive)  
 * @param prefGrowth  preferred growth amount  
 * @return the new array length  
 * @throws OutOfMemoryError if the new length would exceed Integer.MAX_VALUE  
 */
public static int newLength(int oldLength, int minGrowth, int prefGrowth) {  
    // preconditions not checked because of inlining  
    // assert oldLength >= 0    // assert minGrowth > 0  
    int prefLength = oldLength + Math.max(minGrowth, prefGrowth); // might overflow  
    if (0 < prefLength && prefLength <= SOFT_MAX_ARRAY_LENGTH) {  
        return prefLength;  
    } else {  
        // put code cold in a separate method  
        return hugeLength(oldLength, minGrowth);  
    }  
}
```
# 5. 删除 
有了指定位置插入元素的经验，理解删除的过程就比较容易了，如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041228468.png)


```java
@Test  
public void test_copy_remove() {  
    int[] oldArr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  
  
    int index = 2;  
    int numMoved = 10 - index - 1;  
  
    System.arraycopy(oldArr, index + 1, oldArr, index, numMoved);  
  
    System.out.println("数组元素：" + JSON.toJSONString(oldArr));  
}
/**
数组元素：[1,2,4,5,6,7,8,9,10,10]
*/
```
这里结合源码：
```java
/**  
 * Removes the element at the specified position in this list. 
 * Shifts any subsequent elements to the left (subtracts one from their * indices). 
 * @param index the index of the element to be removed  
 * @return the element that was removed from the list  
 * @throws IndexOutOfBoundsException {@inheritDoc}  
 */public E remove(int index) {  
    Objects.checkIndex(index, size);  
    final Object[] es = elementData;  
  
    @SuppressWarnings("unchecked") E oldValue = (E) es[index];  
    fastRemove(es, index);  
  
    return oldValue;  
}

/**  
 * Private remove method that skips bounds checking and does not * return the value removed. */
 * private void fastRemove(Object[] es, int i) {  
    modCount++;  
    final int newSize;  
    if ((newSize = size - 1) > i)  
        System.arraycopy(es, i + 1, es, i, newSize - i);  
    es[size = newSize] = null;  
}
```
以前版本的代码如下，做法基本一致：
```java
public E remove(int index) { 
	rangeCheck(index); 
	modCount++; 
	E oldValue = elementData(index); 
	int numMoved = size - index - 1; 
	if (numMoved > 0) 
		System.arraycopy(elementData, index+1, elementData, index, numMoved); 
	elementData[--size] = null; // clear to let GC do its work return oldValue; 
}
```
1. 校验是否越界；rangeCheck(index); 
2. 计算删除元素的移动长度 numMoved，并通过 System.arraycopy 自己把元素 复制给自己。 
3. 把结尾元素清空，null（注意，ArrayList里的数组 `elementData` 是 `Object[]` ，不然无法赋值null；基本类型的数组如int[] 无法转为 Object[]）。

### 测试
设定一个拥有 10 个元素的数组，同样按照 ArrayList 的规则进行移动元素。 
- 注意，为了方便观察结果，**这里没有把结尾元素设置为 null**。
```java
@Test  
public void test_copy_remove() {  
    int[] oldArr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  
  
    int index = 2;  
    int numMoved = 10 - index - 1;  
  
    System.arraycopy(oldArr, index + 1, oldArr, index, numMoved);  
  
    System.out.println("数组元素：" + JSON.toJSONString(oldArr));  
}
/**
数组元素：[1,2,4,5,6,7,8,9,10,10]
*/
```
可以看到指定位置 index = 2，元素已经被删掉。 
- 同时数组已经移动用元素 4 占据了原来元素 3 的位置，同时结尾的 10 还等待删 除。
- ==这就是为什么 ArrayList 中有这么一句代码==；`elementData[--size] = null`

# 6. 扩展 
如果给你一组元素；a、b、c、d、e、f、g，需要你放到 ArrayList 中，但是要求 **获取一个元素的时间复杂度都是 O(1)** ，你怎么处理？ 

想解决这个问题，就**需要知道元素添加到集合中后知道它的位置**，而==这个位置呢， 其实可以通过哈希值与集合长度与运算，得出存放数据的下标==（这是什么原理？），如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041243491.png)
如图就是计算出每一个元素应该存放的位置，这样就可以 O(1)复杂度获取元素。
## 6.1 代码操作(添加元素)
```java
@Test  
public void t_hashIdx() {  
  
    List<String> list = new ArrayList<String>(Collections.<String>nCopies(8, "0"));  
  
    list.set("a".hashCode() & 8 - 1, "a");  
    list.set("b".hashCode() & 8 - 1, "b");  
    list.set("c".hashCode() & 8 - 1, "c");  
    list.set("d".hashCode() & 8 - 1, "d");  
    list.set("e".hashCode() & 8 - 1, "e");  
    list.set("f".hashCode() & 8 - 1, "f");  
    list.set("g".hashCode() & 8 - 1, "g");  
  
    System.out.println("元素集合:" + list);  
```
以上是初始化 ArrayList，并存放相应的元素。存放时候计算出每个元素的下标 值。
## 6.2 代码操作(获取元素)
```java
    System.out.println("获取元素f [\"f\".hashCode() & 8 - 1)] Idx：" + ("f".hashCode() & (8 - 1)) + " 元素：" + list.get("f".hashCode() & 8 - 1));  
    System.out.println("获取元素e [\"e\".hashCode() & 8 - 1)] Idx：" + ("e".hashCode() & (8 - 1)) + " 元素：" + list.get("e".hashCode() & 8 - 1));  
    System.out.println("获取元素d [\"d\".hashCode() & 8 - 1)] Idx：" + ("d".hashCode() & (8 - 1)) + " 元素：" + list.get("d".hashCode() & 8 - 1));  
}
```
## 6.3 测试结果
```java
元素集合:[0, a, b, c, d, e, f, g] 
获取元素 f ["f".hashCode() & 8 - 1)] Idx：6 元素：f 
获取元素 e ["e".hashCode() & 8 - 1)] Idx：5 元素：e 
获取元素 d ["d".hashCode() & 8 - 1)] Idx：4 元素：d
```
通过测试结果可以看到，下标位置 0 是初始元素，元素是按照指定的下标进行插入 的。 
那么现在获取元素的时间复杂度就是 O(1)，是不有点像 HashMap 中的桶结构。

# 总结 
就像我们开头说的一样，数据结构是你写出代码的基础，更是写出高级代码的核 心。只有了解好数据结构，才能更透彻的理解程序设计。并不是所有的逻辑都是 for 循环 。
面试题只是引导你学习的点，但不能为了面试题而忽略更重要的核心知识学习，背 一两道题是不可能抗住深度问的。因为任何一个考点，都不只是一种问法，往往可 以从很多方面进行提问和考查。就像你看完整篇文章，是否理解了没有说到的知 识，当你固定位置插入数据时会进行数据迁移，那么在拥有大量数据的 ArrayList 中是不适合这么做的，非常影响性能。 
也参考到一些优秀的资料，尤其发现这份外文文档； https://beginnersbook.com/ 大家可以参考学习。

---
# LinkedList、ArrayList，插入分析
很多时候，你额外的能力才是自身价值的体现，不要以为你的能力就只是做个业 务开发每天 CRUD，并不是产品让你写 CRUD，而是因为你的能力只能产品功能设 计成 CRUD。 就像数据结构、算法逻辑、源码技能，它都是可以为你的业务开发赋能的，也是 写出更好、更易扩展程序的根基，所以学好这份知识非常有必要。
# 一、面试题 
谢飞机，ArrayList 资料看了吧？嗯，那行问问你哈 
问：ArrayList 和 LinkedList，都用在什么场景呢？ 
答：啊，这我知道了。ArrayList 是基于数组实现、LinkedList 是基于双向链表 实现，所以基于数据结构的不同，遍历和查找多的情况下用 ArrayList、插入和 删除频繁的情况下用 LinkedList。
问：嗯，那 LinkedList 的插入效率一定比 ArrayList 好吗？ 
答：对，好！ 送你个飞机✈，回去等消息吧！ 

其实，飞机回答的也不是不对，只是不全面。出门后不甘心买瓶肥宅水又回来， 跟面试官聊了 2 个点，要到了两张图，如下；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041254063.png)
如图，分别是；10 万、100 万、1000 万，数据在两种集合下不同位置的插入效果， 所以：不能说 LinkedList 插入就快，ArrayList 插入就慢，还需要看具体的操 作情况。

# 2. 数据结构 
Linked + List = 链表 + 列表 = LinkedList = 链表列表
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041257196.png)
LinkedList，是基于链表实现，由双向链条 next、prev，把数据节点穿插起来。 所以，在插入数据时，是不需要像我们上一章节介绍的 ArrayList 那样，扩容数 组。 但，又不能说所有的插入都是高效，比如**中间区域插入，他还需要遍历元素找到 插入位置**。

具体的细节，我们在下文的源码分析中进行讲解，也帮谢飞机扫除疑 惑。
# 3. 源码分析 
## 3.1 初始化 
与 ArrayList 不同，LinkedList 初始化不需要创建数组，因为它是一个链表结 构。而且也没有传给构造函数初始化多少个空间的入参

```java
@Test  
public void test_init() {  
    // 初始化方式；普通方式  
    LinkedList<String> list01 = new LinkedList<String>();  
    list01.add("a");  
    list01.add("b");  
    list01.add("c");  
    System.out.println(list01);  
  
    // 初始化方式；Arrays.asList  
    LinkedList<String> list02 = new LinkedList<String>(Arrays.asList("a", "b", "c"));  
    System.out.println(list02);  
  
    // 初始化方式；内部类  
    LinkedList<String> list03 = new LinkedList<String>() {  
        {  
            add("a");  
            add("b");  
            add("c");  
        }  
    };  
    System.out.println(list03);  
  
    // 初始化方式；Collections.nCopies  
    LinkedList<Integer> list04 = new LinkedList<Integer>(Collections.nCopies(10, 0));  
    System.out.println(list04);  
}
```
这些方式都可以初始化操作，按需选择即可。
## 3.2 插入 
LinkedList 的插入方法比较多，**List 中接口中默认提供的是 add，也可以指定 位置插入**。但在 LinkedList 中还提供了**头插 addFirst 和尾插 addLast**。 关于插入这部分就会讲到为什么；有的时候 LinkedList 插入更耗时、有的时候 ArrayList 插入更好。 
### 3.2.1 头插 
先来看一张数据结构对比图，回顾下 ArrayList 的插入也和 LinkedList 插入做 下对比，如下；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041516678.png)
```java
/**  
 * Inserts the specified element at the beginning of this list. * * @param e the element to add  
 */
public void addFirst(E e) {  
    linkFirst(e);  
}
/**  
 * Links e as first element. 
 */
 private void linkFirst(E e) {  
    final Node<E> f = first;  
    final Node<E> newNode = new Node<>(null, e, f);  
    first = newNode;  
    if (f == null)  
        last = newNode;  
    else        
	    f.prev = newNode;  
    size++;  
    modCount++;  
}
```
- first，首节点会一直被记录，这样就非常方便头插。
- 插入时候会创建新的节点元素，new Node<>(null, e, f)，紧接着把新的头 元素赋值给 first。 
- 之后判断 f 节点是否存在，**不存在则把头插节点作为最后一个节点**、存在则用 f 节 点的上一个链条 prev 链接。 
- 最后记录 size 大小、和元素数量 modCount。**modCount 用在遍历时做校验， modCount != expectedModCount**

####  ArrayList、LinkeList，头插源码验证
```java
@Test  
public void test_ArrayList_addFirst() {  
    ArrayList<Integer> list = new ArrayList<Integer>();  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < 10000000; i++) {  
        list.add(0, i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
    // 10万=406、100万=46288、1000万=  
}  
  
@Test  
public void test_LinkedList_addFirst() {  
    LinkedList<Integer> list = new LinkedList<Integer>();  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < 10000000; i++) {  
        list.addFirst(i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
    // 10万=7、100万=37、10000万=5714  
}
```
这里我们分别验证，10 万、100 万、1000 万的数据量，在头插时的一个耗时情 况。 
如我们数据结构对比图中一样，**ArrayList 需要做大量的位移和复制操作，而 LinkedList 的优势就体现出来了**，耗时只是实例化一个对象。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041607496.png)
### 3.2.2 尾插 
先来看一张数据结构对比图，回顾下 ArrayList 的插入也和 LinkedList 插入做 下对比，如下；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041608140.png)
1. ArrayList 尾插时，是不需要数据位移的，**比较耗时的是数据的扩容时，需要拷贝 迁移**。 
2. LinkedList 尾插时，与头插相比耗时点会在对象的实例化上。

```java
/**  
 * Links e as last element. 
 */
void linkLast(E e) {  
    final Node<E> l = last;  
    final Node<E> newNode = new Node<>(l, e, null);  
    last = newNode;  
    if (l == null)  
        first = newNode;  
    else        
	    l.next = newNode;  
    size++;  
    modCount++;  
}
```
与头插代码相比几乎没有什么区别，只是 first 换成 last 
耗时点只是在创建节点上，Node
#### 验证
```java
@Test  
public void test_ArrayList_addLast() {  
    ArrayList<Integer> list = new ArrayList<Integer>();  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < 10000000; i++) {  
        list.add(i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
    // 10万=12、100万=44、1000万=2648  
}  
  
@Test  
public void test_LinkedList_addLast() {  
    LinkedList<Integer> list = new LinkedList<Integer>();  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < 1000000; i++) {  
        list.addLast(i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
    // 10万=11、100万=45、1000万=5899  
}
```
这里我们分别验证，10 万、100 万、1000 万的数据量，在尾插时的一个耗时情 况。 
- 如我们数据结构对比图中一样，ArrayList 不需要做位移拷贝也就不那么耗时了， 而 LinkedList 则需要创建大量的对象。所以这里 ArrayList 尾插的效果更好一些。
- 但在本人电脑上，头插和尾插都是LinkedList快。

### 3.3 中间插
先来看一张数据结构对比图，回顾下 ArrayList 的插入也和 LinkedList 插入做 下对比，如下；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041614526.png)
看上图我们可以分析出几点； 
1. ArrayList 中间插入，首先我们知道**他的定位时间复杂度是 O(1)**，比较耗时的点在 于**数据迁移和容量不足的时候扩容**。
2. LinkedList 中间插入，链表的数据实际插入时候并不会怎么耗时，但**它定位的元 素的时间复杂度是 O(n)**，所以这部分以及元素的实例化比较耗时。

这里看下 LinkedList 指定位置插入的源码； 使用 add(位置、元素)方法插入：
```java
/**  
 * Inserts the specified element at the specified position in this list. 
 * Shifts the element currently at that position (if any) and any 
 * subsequent elements to the right (adds one to their indices). 
 * @param index index at which the specified element is to be inserted  
 * @param element element to be inserted  
 * @throws IndexOutOfBoundsException {@inheritDoc}  
 */
 public void add(int index, E element) {  
    checkPositionIndex(index);  
  
    if (index == size)  
        linkLast(element);  
    else        
	    linkBefore(element, node(index));  
}

/**  
 * Returns the (non-null) Node at the specified element index. 
 */
Node<E> node(int index) {  
    // assert isElementIndex(index);  
    if (index < (size >> 1)) {  
        Node<E> x = first;  
        for (int i = 0; i < index; i++)  
            x = x.next;  
        return x;  
    } else {  
        Node<E> x = last;  
        for (int i = size - 1; i > index; i--)  
            x = x.prev;  
        return x;  
    }  
}
```
size >> 1，**这部分的代码判断元素位置在左半区间，还是右半区间，在进行循 环查找**。

执行插入：找到指定位置插入的过程就比较简单了，与头插、尾插，相差不大。 
整个过程可以看到，插入中比较耗时的点会在遍历寻找插入位置上。
```java
/**  
 * Inserts element e before non-null Node succ. */
 void linkBefore(E e, Node<E> succ) {  
    // assert succ != null;  
    final Node<E> pred = succ.prev;  
    final Node<E> newNode = new Node<>(pred, e, succ);  
    succ.prev = newNode;  
    if (pred == null)  
        first = newNode;  
    else        
	    pred.next = newNode;  
    size++;  
    modCount++;  
}
```
#### ArrayList、LinkeList，中间插入源码验证
```java
@Test  
public void test_ArrayList_addCenter() {  
    ArrayList<Integer> list = new ArrayList<Integer>();  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < 10000000; i++) {  
        list.add(list.size() >> 1, i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
    // 10万=251、100万=22460、1000万=  
}  
  
@Test  
public void test_LinkedList_addCenter() {  
    LinkedList<Integer> list = new LinkedList<Integer>();  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < 1000000; i++) {  
        list.add(list.size() >> 1, i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
    // 10万=11112、100万=45、1000万=？  
}
```
这里我们分别验证，10 万、100 万、1000 万的数据量，在中间插时的一个耗时情 况。  可以看到 Linkedlist 在中间插入时，遍历寻找位置还是非常耗时了。所以不同的情 况下，需要选择不同的 List 集合做业务。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041619283.png)

## 3.3 删除
讲了这么多插入的操作后，删除的知识点就很好理解了。与 ArrayList 不同，删 除不需要拷贝元素，LinkedList 是找到元素位置，把元素前后链连接上。基本如下图；
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041620235.png)
确定出要删除的元素 x，把前后的链接进行替换。 
**如果是删除首尾元素，操作起来会更加容易**，这也就是为什么说插入和删除快。但 中间位置删除，需要遍历找到对应位置。
### 3.3.1 删除操作方法
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041621237.png)
测试代码：
```java
@Test  
public void test_remove() {  
    LinkedList<String> list = new LinkedList<String>();  
    list.add("a");  
    list.add("b");  
    list.add("c");  
  
    list.remove();  
    list.remove(1);  
    list.remove("a");  
    list.removeFirst();  
    list.removeLast();  
    list.removeAll(Arrays.asList("a", "b"));  
}
```
### 3.3.2 源码分析
删除操作的源码都差不多，分为删除首尾节点与其他节点时候，对节点的解链操 作。这里我们举例一个删除其他位置的源码进行学习，如下；
```java
/**  
 * Removes the first occurrence of the specified element from this list, * if it is present.  If this list does not contain the element, it is 
 * unchanged.  More formally, removes the element with the lowest index * {@code i} such that  
 * {@code Objects.equals(o, get(i))}  
 * (if such an element exists).  Returns {@code true} if this list  
 * contained the specified element (or equivalently, if this list 
 * changed as a result of the call).  
 * @param o element to be removed from this list, if present  
 * @return {@code true} if this list contained the specified element  
 */public boolean remove(Object o) {  
    if (o == null) {  
        for (Node<E> x = first; x != null; x = x.next) {  
            if (x.item == null) {  
                unlink(x);  
                return true;            
            }  
        }  
    } else {  
        for (Node<E> x = first; x != null; x = x.next) {  
            if (o.equals(x.item)) {  
                unlink(x);  
                return true;           
            }  
        }  
    }  
    return false;  
}
```
这一部分是元素定位，和 unlink(x)解链。循环查找对应的元素，这部分没有什 么难点。

这部分源码主要有以下几个知识点； 
1. 获取待删除节点的信息；元素 item、元素下一个节点 next、元素上一个节点 prev。 
2. 如果上个节点为空则把待删除元素的下一个节点赋值给首节点，否则把待删除节点 的下一个节点，赋值给待删除节点的上一个节点的子节点。 
3. 同样待删除节点的下一个节点 next，也执行 2 步骤同样操作。 
4. 最后是把删除节点设置为 null，并扣减 size 和 modCount 数量。
```java
/**  
 * Unlinks non-null node x. 
 */
 E unlink(Node<E> x) {  
    // assert x != null;  
    final E element = x.item;  
    final Node<E> next = x.next;  
    final Node<E> prev = x.prev;  
  
    if (prev == null) {  
        first = next;  
    } else {  
        prev.next = next;  
        x.prev = null;  
    }  
  
    if (next == null) {  
        last = prev;  
    } else {  
        next.prev = prev;  
        x.next = null;  
    }  
  
    x.item = null;  
    size--;  
    modCount++;  
    return element;  
}
```

## 3.4 遍历

接下来说下遍历，ArrayList 与 LinkedList 的遍历都是通用的，基本包括 5 种 方式。 这里我们先初始化出待遍历的集合 1 千万数据；
下面是普通for循环和增强for循环：
```java
int xx = 0;  
@Before  
public void init() {  
    for (int i = 0; i < 10000000; i++) {  
        list.add(i);  
    }  
}  
  
  
@Test  
public void test_LinkedList_for0() {  
    long startTime = System.currentTimeMillis();  
    for (int i = 0; i < list.size(); i++) {  
        xx += list.get(i);  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
}  
  
@Test  
public void test_LinkedList_for1() {  
    long startTime = System.currentTimeMillis();  
    for (Integer itr : list) {  
        xx += itr;  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
}
```
然后是iterator遍历和forEach循环、stream流：
```java
@Test  
public void test_LinkedList_Iterator() {  
    long startTime = System.currentTimeMillis();  
    Iterator<Integer> iterator = list.iterator();  
    while (iterator.hasNext()) {  
        Integer next = iterator.next();  
        xx += next;  
    }  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
}  
  
@Test  
public void test_LinkedList_forEach() {  
    long startTime = System.currentTimeMillis();  
    list.forEach(integer -> {  
        xx += integer;  
    });  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
}  
  
  
@Test  
public void test_LinkedList_stream() {  
    long startTime = System.currentTimeMillis();  
    list.stream().forEach(integer -> {  
        // do something  
        xx += integer;  
    });  
    System.out.println("耗时：" + (System.currentTimeMillis() - startTime));  
}
```
那么，以上这 5 种遍历方式谁最慢呢？按照我们的源码学习分析下吧
# 总结

ArrayList 与 LinkedList 都有自己的使用场景，如果你不能很好的确定，那么就使用 ArrayList。
- 但如果你能确定你会**在集合的首位有大量的插入、删除以及获取操作**， 那么可以使用 LinkedList，因为它都有相应的方法；addFirst、addLast、 removeFirst、removeLast、getFirst、getLast，这些操作的时间复杂 度都是 O(1)，非常高效。 
- **LinkedList 的链表结构不一定会比 ArrayList 节省空间**，首先**它所占用的内存不是连 续的**，其次他还需要**大量的实例化对象创造节点**。虽然不一定节省空间，但链表结 构也是非常优秀的数据结构，它能在你的程序设计中起着非常优秀的作用，例如可 视化的链路追踪图，就是需要链表结构，并需要每个节点自旋一次，用于串联业 务。 

程序的精髓往往就是数据结构的设计，这能为你的程序开发提供出非常高的效率改 变。可能目前你还不能用到，但万一有一天你需要去造🚀火箭了呢？