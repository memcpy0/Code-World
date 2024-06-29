Stack被淘汰了……改用Deque

# 1. 面试题 
谢飞机，飞机你旁边这是？ 
答：啊，谢坦克，我弟弟。还没毕业，想来看看大公司面试官的容颜。 
问：飞机，上次把 LinkedList 都看了吧，那我问你哈。LinkedList 可以当队列 用吗？ 
答：啊？可以，可以吧！ 
问：那，数组能当队列用吗？不能？对列有啥特点吗？
答：队列先进先出，嗯，嗯。 
问：还有吗？了解延时队列吗？双端队列呢？
![500](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041634338.png)

延迟队列、阻塞队列

# 2. 源码学习 
## 2.1 先说一个被抛弃 Stack 
有时候不会反而不会犯错误！怕就怕在只知道一知半解。
**抛弃的不是栈这种数据结构，而是 Stack 实现类**，如果你还不了解就用到业务开 发中，就很可能会影响系统性能。其实 **Stack 这个栈已经是不建议使用了**，但是 为什么不建议使用，我们可以通过使用和源码分析了解下根本原因。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041636867.png)
### 2.1.1 功能使用
```java
@Test  
public void test_stack() {  
    Stack<String> s = new Stack<String>();  
  
    s.push("aaa");  
    s.push("bbb");  
    s.push("ccc");  
  
    System.out.println("获取最后一个元素：" + s.peek());  
    System.out.println("获取最后一个元素：" + s.lastElement());  
    System.out.println("获取最先放置元素：" + s.firstElement());  
  
    System.out.println("弹出一个元素[LIFO]：" + s.pop());  
    System.out.println("弹出一个元素[LIFO]：" + s.pop());  
    System.out.println("弹出一个元素[LIFO]：" + s.pop());  
  
    /**  
     * 获取最后一个元素：ccc  
     * 获取最后一个元素：ccc  
     * 获取最先放置元素：aaa  
     * 弹出一个元素[LIFO]：ccc  
     * 弹出一个元素[LIFO]：bbb  
     * 弹出一个元素[LIFO]：aaa  
     */}
```
看到测试结果，与你想的答案是否一致？ 
- peek，是偷看的意思，就是看一下，不会弹出元素。满足后进先出的规则，它看的 是最后放进去的元素 ccc。 
- lastElement、firstElement，字面意思的方法，获取最后一个和获取第一个元素。 
- pop，是队列中弹出元素，弹出后也代表着要把属于这个位置都元素清空，删掉。

### 2.1.2 源码分析
我们说 Stack 栈，这个实现类已经不推荐使用了，需要从它的源码上看。
```java
/* The Stack class represents a last-in-first-out (LIFO) stack of objects. 
 * It extends class Vector with five operations that allow a vector to be treated as a stack. 
 * The usual push and pop operations are provided, as well as a method to peek at the top item on the stack, a method to test for whether the stack is empty, and a method to search the stack for an item and discover how far it is from the top.
 * When a stack is first created, it contains no items.
 * A more complete and consistent set of LIFO stack operations is provided by the Deque interface and its implementations, which should be used in preference to this class. For example:
 Deque<Integer> stack = new ArrayDeque<Integer>();
 */
public class Stack<E> extends Vector<E> {  
    /**  
     * Creates an empty Stack.     
     */    
    public Stack() {  
    }
    
```
Stack 栈是在 JDK1.0 时代时，基于继承 Vector实现的。本身 Vector 就是 一个不推荐使用的类，主要在于它的一些操作方法**锁🔒(synchronized)的力度太 粗，都是放到方法上**。

如push方法：**Stack 栈底层是使用 Vector 数组实现**，在学习 ArrayList 时候我们知道， **数组结构在元素添加和删除需要通过System.arraycopy，进行扩容操作**。而 **本身栈的特点是首尾元素的操作**，也不需要遍历，**使用数组结构其实并不太理想**。

时在这个方法的注释上也明确标出来，推荐使用 `Deque stack = new ArrayDeque();` ，虽然这也是数组结构，但是**它没有粗粒度 的锁**，同时**可以申请指定空间**并且**在扩容时操作时也要优于 Stack** 。并且**它还是 一个双端队列，使用起来更灵活**。
```java
/**  
 * Pushes an item onto the top of this stack. This has exactly * the same effect as: * <blockquote><pre>  
 * addElement(item)</pre></blockquote>  
 *  
 * @param   item   the item to be pushed onto this stack.  
 * @return  the {@code item} argument.  
 * @see     java.util.Vector#addElement  
 */
 public E push(E item) {  
    addElement(item);  
  
    return item;  
}
// Vector中
/**  
 * Adds the specified component to the end of this vector, 
 * increasing its size by one. The capacity of this vector is 
 * increased if its size becomes greater than its capacity. 
 * <p>This method is identical in functionality to the  
 * {@link #add(Object) add(E)}  
 * method (which is part of the {@link List} interface).  
 * 
 * @param   obj   the component to be added  
 */
public synchronized void addElement(E obj) {  
    modCount++;  
    add(obj, elementData, elementCount);  
}

/**  
 * This helper method split out from add(E) to keep method 
 * bytecode size under 35 (the -XX:MaxInlineSize default value), 
 * which helps when add(E) is called in a C1-compiled loop. 
 */
private void add(E e, Object[] elementData, int s) {  
    if (s == elementData.length)  
        elementData = grow();  
    elementData[s] = e;  
    elementCount = s + 1;  
}
```
## 2.2 双端队列ArrayDeque（可用作Queue或Stack）
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041650907.png)

ArrayDeque 是**基于数组实现的可动态扩容的**双端队列，也就是说你可以在队列 的头和尾同时插入和弹出元素。**当元素数量超过数组初始化长度时，则需要扩容 和迁移数据**。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041652756.png)
1. **双端队列是基于数组实现，所以扩容迁移数据操作**。 
2. push=addFirst，向结尾插入、offerLast，向头部插入，这样两端都满足后进先出。 
3. 整体来看，**双端队列，就是一个环形**。所以**扩容后继续插入元素也满足后进先出**。

```java
@Test  
public void test_ArrayDeque() {  
    Deque<String> deque = new ArrayDeque<String>(1);  
  
    deque.push("a");  
    deque.push("b");  
    deque.push("c");  
    deque.push("d");  
    // Iterator<String> ie = deque.iterator();
    // while (ie.hasNext()) {  
	//     System.out.println(ie.next());  
	// }
    deque.offerLast("e");  
    deque.offerLast("f");  
    deque.offerLast("g");  
    deque.offerLast("h");  // 这时候扩容了  
  
    deque.push("i");  
    deque.offerLast("j");  
  
    System.out.println("数据出栈：");  
    while (!deque.isEmpty()) {  
        System.out.print(deque.pop() + " ");  
    }  
  
}
```
以上这部分代码就是与上图的展现是一致的，按照图中的分析我们看下输出结果， 如下； 
数据出栈：`i d c b a e f g h j `
i d c b a e f g h j，正好满足了我们的说的数据出栈顺序。可以参考上图 再进行理解。因为是一个双向环形数组，所以从i开始pop。

下面进行源码分析：
ArrayDeque 这种双端队列是基于数组实现的，所以源码上从初始化到数据入栈 扩容，都会有数组操作的痕迹。接下来我们就依次分析下。
### 2.2.1 源码分析-初始化
new ArrayDeque(1);，其实它的构造函数初始化默认也提供了几个方法， 比如你可以指定大小以及提供默认元素。
在初始化的过程中，它需要找到你当前传输值最小的 2 的倍数的一个容量。这与 HashMap 的初始化过程相似。
```java
private static int calculateSize(int numElements) { 
	int initialCapacity = MIN_INITIAL_CAPACITY; 
	// Find the best power of two to hold elements. 
	// Tests "<=" because arrays aren't kept full. 
	if (numElements >= initialCapacity) { 
		initialCapacity = numElements; 
		initialCapacity |= (initialCapacity >>> 1); 
		initialCapacity |= (initialCapacity >>> 2); 
		initialCapacity |= (initialCapacity >>> 4); 
		initialCapacity |= (initialCapacity >>> 8); 
		initialCapacity |= (initialCapacity >>> 16); 
		initialCapacity++; 
		if (initialCapacity < 0) 
		// Too many elements, must back off 
		initialCapacity >>>= 1; // Good luck allocating 2 ^ 30 element 
	} 
	return initialCapacity; 
}
```
但是我的版本里面没有这个代码，只是如下：
```java
/**  
 * Constructs an empty array deque with an initial capacity * 
 * sufficient to hold 16 elements. */
public ArrayDeque() {  
    elements = new Object[16 + 1];  
}  
  
/**  
 * Constructs an empty array deque with an initial capacity * sufficient to hold the specified number of elements. * * @param numElements lower bound on initial capacity of the deque  
 */
 public ArrayDeque(int numElements) {  
    elements =  
        new Object[(numElements < 1) ? 1 :  
                   (numElements == Integer.MAX_VALUE) ? Integer.MAX_VALUE :  
                   numElements + 1];  
}  
  
/**  
 * Constructs a deque containing the elements of the specified 
 * collection, in the order they are returned by the collection's 
 * iterator.  (The first element returned by the collection's 
 * iterator becomes the first element, or <i>front</i> of the  
 * deque.) 
 * @param c the collection whose elements are to be placed into the deque  
 * @throws NullPointerException if the specified collection is null  
 */
public ArrayDeque(Collection<? extends E> c) {  
    this(c.size());  
    copyElements(c);  
}
```
### 2.2.2 数据入栈
deque.push("a"); ， ArrayDeque ， 提 供 了 一 个 push 方法，这个方法与 deque.addFirst完全一致，没有返回值，很简单：因为push中调用了 `addFirst`
```java
/**  
 * Pushes an element onto the stack represented by this deque.  In other 
 * words, inserts the element at the front of this deque. 
 * 
 * <p>This method is equivalent to {@link #addFirst}.  
 * 
 * @param e the element to push  
 * @throws NullPointerException if the specified element is null  
 */
public void push(E e) {  
    addFirst(e);  
}
```
也和 deque.offerFirst(“a”)一致
```java
/**  
 * Inserts the specified element at the front of this deque. 
 * 
 * @param e the element to add  
 * @return {@code true} (as specified by {@link Deque#offerFirst})  
 * @throws NullPointerException if the specified element is null  
 */
public boolean offerFirst(E e) {  
    addFirst(e);  
    return true;
}
```
因为它们的底层源码是一样的（都不能添加null值），如下；
```java
/**  
 * Inserts the specified element at the front of this deque. * * @param e the element to add  
 * @throws NullPointerException if the specified element is null  
 */
public void addFirst(E e) {  
    if (e == null)  
        throw new NullPointerException();  
    final Object[] es = elements;  
    es[head = dec(head, es.length)] = e;  
    if (head == tail)  
        grow(1); // 至少增加1
}
```
此外是offerLast、addLast：
```java
/**  
 * Inserts the specified element at the end of this deque. * * @param e the element to add  
 * @return {@code true} (as specified by {@link Deque#offerLast})  
 * @throws NullPointerException if the specified element is null  
 */
public boolean offerLast(E e) {  
    addLast(e);  
	return true;
}
    
/**  
 * Inserts the specified element at the end of this deque. * * <p>This method is equivalent to {@link #add}.  
 * * @param e the element to add  
 * @throws NullPointerException if the specified element is null  
 */
 public void addLast(E e) {  
    if (e == null)  
        throw new NullPointerException();  
    final Object[] es = elements;  
    es[tail] = e;  
    if (head == (tail = inc(tail, es.length)))  
        grow(1);  
}
```
但还有之前版本的代码，其实没什么区别，
1. 在 addFirst()中，定位下标，`head = (head - 1) & (elements.length - 1)` ，因为我们的数组长度是 2^n 的倍数，所以 2^n - 1 就是一个全是 1 的二进制数，可以用于与运算得出数组下标。 
2. 同样 addLast()中，也使用了相同的方式定位下标，只不过它是从 $0$ 开始，往上 增加：
3. 最后，当头(head)与尾(tile)，数组则需要两倍扩容 doubleCapacity。
```java
addFirst： public void addFirst(E e) { 
	if (e == null) 
		throw new NullPointerException(); 
	elements[head = (head - 1) & (elements.length - 1)] = e; 
	if (head == tail) doubleCapacity(); 
} 

addLast：
public void addLast(E e) { 
	if (e == null) 
		throw new NullPointerException(); 
	elements[tail] = e; 
	if ( (tail = (tail + 1) & (elements.length - 1)) == head) 
		doubleCapacity(); 
}	
```
下标计算：`head = (head - 1) & (elements.length - 1)` ：
- (0 - 1) & (8 - 1) = 7 
- (7 - 1) & (8 - 1) = 6 
- (6 - 1) & (8 - 1) = 5  ...

### 2.2.3 扩容和数据迁移
新版代码：如果 `head==tail` 则扩容，无论是从addFirst还是addLast。
ArrayDeque现在会在小规模时翻倍，大规模 $\ge 64$ 时增加50%
```java
/**  
 * Increases the capacity of this deque by at least the given amount. 
 * 
 * @param needed the required minimum extra capacity; must be positive  
 */
 private void grow(int needed) {  
    // overflow-conscious code  
    final int oldCapacity = elements.length;  
    int newCapacity;  
    // Double capacity if small; else grow by 50%  
    int jump = (oldCapacity < 64) ? (oldCapacity + 2) : (oldCapacity >> 1);  
    if (jump < needed  
        || (newCapacity = (oldCapacity + jump)) - MAX_ARRAY_SIZE > 0)  
        newCapacity = newCapacity(needed, jump);  
    final Object[] es = elements = Arrays.copyOf(elements, newCapacity); // 直接把elements整体复制过来
    // Exceptionally, here tail == head needs to be disambiguated  // 但此时head==tail的情况需要改变
    if (tail < head || (tail == head && es[head] != null)) {  // 如果tail在head前，出现wrap around
        // wrap around; slide first leg forward to end of array  
        int newSpace = newCapacity - oldCapacity;  // 
        System.arraycopy(es, head,  // 将新数组es中从head开始的、长度为oldCapacity-head的值拷贝到head+newSpace后
                         es, head + newSpace,  
                         oldCapacity - head);  
        for (int i = head, to = (head += newSpace); i < to; i++) // [head,head+newSpace)赋值为null
            es[i] = null;  
    }  
}
```

之前版本的代码——这样长度为2的幂，才能用&代替取模：
```java
private void doubleCapacity() { 
	assert head == tail; 
	int p = head; 
	int n = elements.length; 
	int r = n - p; // number of elements to the right of p 
	int newCapacity = n << 1; 
	if (newCapacity < 0) 
		throw new IllegalStateException("Sorry, deque too big");
	Object[] a = new Object[newCapacity]; 
	System.arraycopy(elements, p, a, 0, r); 
	System.arraycopy(elements, 0, a, r, p); 
	elements = a; 
	head = 0; 
	tail = n; 
}
```
其实以上这部分源码，就是进行两倍 n << 1 扩容，同时**把两端数据迁移进新的 数组**，整个操作过程也与我们上图对应。为了更好的理解，我们单独把这部分代 码做一些测试。
#### 测试代码
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041652756.png)
```java
@Test  
public void test_arraycopy() {  
    int head = 0, tail = 0;  
  
    Object[] elements = new Object[8];  
    elements[head = (head - 1) & (elements.length - 1)] = "a";  // 和现在的ArrayDeque addFirst一样，先dec再存入值
    elements[head = (head - 1) & (elements.length - 1)] = "b";  
    elements[head = (head - 1) & (elements.length - 1)] = "c";  
    elements[head = (head - 1) & (elements.length - 1)] = "d";  
  
    elements[tail] = "e";  // 和现在的ArrayDeque addLast一样
    tail = (tail + 1) & (elements.length - 1);  // 这里由于数组大小为8，可以这么做，但现在ArrayDeque不是这么做的
    elements[tail] = "f";  
    tail = (tail + 1) & (elements.length - 1);  
    elements[tail] = "g";  
    tail = (tail + 1) & (elements.length - 1);  
    elements[tail] = "h";  
    tail = (tail + 1) & (elements.length - 1);  
  
    System.out.println("head：" + head);  
    System.out.println("tail：" + tail);  
  
    int p = head;  
    int n = elements.length;  
    int r = n - p; // number of elements to the right of p  
  
    System.out.println(JSON.toJSONString(elements));  
    // head == tail 扩容  
    Object[] a = new Object[8 << 1];  
    System.arraycopy(elements, p, a, 0, r);  // 先拷贝head到末尾的元素
    System.out.println(JSON.toJSONString(a));  
    System.arraycopy(elements, 0, a, r, p);  // 再拷贝0到tail-1的元素
    System.out.println(JSON.toJSONString(a));  
    elements = a;  
    head = 0;  
    tail = n;  
  
    a[head = (head - 1) & (a.length - 1)] = "i";  
    elements[tail] = "j";  
    tail = (tail + 1) & (elements.length - 1);  
  
    System.out.println(JSON.toJSONString(a));  
}
/**
head：4 tail：4 
["e","f","g","h","d","c","b","a"] 
["d","c","b","a",null,null,null,null,null,null,null,null,null,null,null,null] ["d","c","b","a","e","f","g","h",null,null,null,null,null,null,null,null] ["d","c","b","a","e","f","g","h","j",null,null,null,null,null,null,"i"]
*/
```
从测试结果可以看到； 
1. 当 head 与 tail 相等时，进行扩容操作。 
2. 第一次数据迁移，`System.arraycopy(elements, p, a, 0, r);`，d、 c、b、a，落入新数组。
3. 第二次数据迁移，`System.arraycopy(elements, 0, a, r, p);`，e、 f、g、h，落入新数组。 
4. 最后再尝试addFirst添加新的元素i，addLast(j) 。每一次的输出结果都可以看到整个双端链路的变 化。

# 3. 双端队列 LinkedList 
**Linkedlist天生就可以支持双端队列**，而且从头尾取数据也是它时间复杂度 O(1) 的。同时**数据的插入和删除也不需要像数组队列那样拷贝数据**，虽然 Linkedlist 有这些优点，但**不能说 ArrayDeque 因为有数组复制性能比它低**。 Linkedlist，数据结构：
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041904023.png)
## 3.1 测试代码
```java
@Test  
public void test_Deque_LinkedList(){  
    Deque<String> deque = new LinkedList<>();  
    deque.push("a");  
    deque.push("b");  
    deque.push("c");  
    deque.push("d");  
  
    deque.offerLast("e");  
    deque.offerLast("f");  
    deque.offerLast("g");  
    deque.offerLast("h");   
    deque.push("i");  
    deque.offerLast("j");  
  
    System.out.println("数据出栈：");  
    while (!deque.isEmpty()) {  
        System.out.print(deque.pop() + " ");  
    }  
}
```
测试结果： 数据出栈：`i d c b a e f g h j`

测试结果上看与使用 ArrayDeque 是一样的，功能上没有差异。

## 3.2 源码分析
### 压栈：deque.push("a");、deque.offerFirst("a"); 
```java
private void linkFirst(E e) { 
	final Node f = first; 
	final Node newNode = new Node<>(null, e, f); 
	first = newNode; 
	if (f == null) last = newNode; 
	else f.prev = newNode; 
	size++; modCount++; 
}
```
压栈：deque.offerLast("e");
```java
void linkLast(E e) { 
	final Node l = last; 
	final Node newNode = new Node<>(l, e, null); 
	last = newNode; 
	if (l == null) first = newNode; 
	else l.next = newNode; 
	size++; modCount++; 
}
```
linkFirst、linkLast，两个方法分别是给链表的首尾节点插入元素，因为这 是链表结构，所以也不存在扩容，只需要把双向链路链接上即可。

# 4. 延时队列DelayQueue
**你是否有时候需要把一些数据存起来，倒计时到某个时刻在使用？** 在 Java 的队列数据结构中，还有一种队列是延时队列，可以通过设定存放时间， 依次轮询获取。
```java
/**  
 * A mix-in style interface for marking objects that should be * acted upon after a given delay. * * <p>An implementation of this interface must define a  
 * {@code compareTo} method that provides an ordering consistent with  
 * its {@code getDelay} method.  
 * 
 * @since 1.5  
 * @author Doug Lea  
 */public interface Delayed extends Comparable<Delayed> {  
  
    /**  
     * Returns the remaining delay associated with this object, in the     * given time unit.     *     * @param unit the time unit  
     * @return the remaining delay; zero or negative values indicate  
     * that the delay has already elapsed     
     */    
     long getDelay(TimeUnit unit);  
}
```
## 4.1 测试
先写一个Delayed的实现类：这个相当于**延时队列的一个固定模版方法**，通过这种方式来控制延时。
```java
package com.memcpy0.interview;  
  
import java.util.concurrent.Delayed;  
import java.util.concurrent.TimeUnit;  
  
public class TestDelayed implements Delayed {  
    private String str;  
    private long time;  
    public TestDelayed(String str, long time, TimeUnit unit) {  
        this.str = str;  
        this.time = System.currentTimeMillis() + (time > 0 ? unit.toMillis(time) : 0);  
    }  
    @Override  
    public long getDelay(TimeUnit unit) { return time - System.currentTimeMillis(); }  
    @Override  
    public int compareTo(Delayed o) {  
        TestDelayed work = (TestDelayed) o;  
        long diff = this.time - work.time;  
        if (diff <= 0) return -1;  
        else return 1;  
    }  
    public String getStr() { return str; }  
}
```
案例测试：
```java
@Test  
public void test_DelayQueue() throws InterruptedException {  
    DelayQueue<TestDelayed> delayQueue =  new DelayQueue<>();  
    delayQueue.offer(new TestDelayed("aaa", 5, TimeUnit.SECONDS));  
    delayQueue.offer(new TestDelayed("ccc", 1, TimeUnit.SECONDS));  
    delayQueue.offer(new TestDelayed("bbb", 3, TimeUnit.SECONDS));  
    logger.info(((TestDelayed) delayQueue.take()).getStr());  
    logger.info(((TestDelayed) delayQueue.take()).getStr());  
    logger.info(((TestDelayed) delayQueue.take()).getStr());  
}
```
测试结果：
```java
19:18:09.436 [main] INFO  com.memcpy0.interview.test.ApiTest - ccc
19:18:09.436 [main] INFO  com.memcpy0.interview.test.ApiTest - bbb
19:18:09.436 [main] INFO  com.memcpy0.interview.test.ApiTest - aaa
```
- 在案例测试中我们分别设定不同的休眠时间，1、3、5，TimeUnit.SECONDS。 
- 测试结果分别在 21、22、24，输出了我们要的队列结果。 
- **队列中的元素不会因为存放的先后顺序而导致输出顺序变化**，它们是依赖于休眠时长决 定。
- 即使中途Thread.sleep了很长时间，依然能得到元素，且顺序不变。

## 4.2 源码分析
DelayedQueue：元素是Delayed接口的子类，继承了AbstractQueue，实现了BlockingQueue接口
- BlockingQueue是一个Queue的子接口，它支持一些额外的操作：等待队列非空时获取元素take()和poll(time,unit)、等待空间去存放一个元素到队列中put(e)和offer(e, time, unit)。后两个有时间的方法留给延时队列这种有时限的队列实现。
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305041927489.png)

```java
An unbounded blocking queue of Delayed elements, in which an element can only be taken when its delay has expired. 
无界的延时Delayed元素的阻塞队列，每个元素只在它的delay过期时被取出
The head of the queue is that Delayed element whose delay expired furthest in the past. 
延时队列的头部是Delayed元素（已经过期了）
If no delay has expired there is no head and poll will return null. 
如果没有过时元素则head和poll返回null
Expiration occurs when an element's getDelay(TimeUnit.NANOSECONDS) method returns a value less than or equal to zero. 
一个Delayed元素的getDelay方法返回<=0的结果时过期
Even though unexpired elements cannot be removed using take or poll, they are otherwise treated as normal elements. 
未过期的元素不能使用take或poll移除
For example, the size method returns the count of both expired and unexpired elements. 
size方法返回过期和未过期的元素个数
This queue does not permit null elements.
队列不允许null元素
This class and its iterator implement all of the optional methods of the Collection and Iterator interfaces.
The Iterator provided in method iterator() is not guaranteed to traverse the elements of the DelayQueue in any
particular order.
```
~~DelayQueue 是基于数组实现的，所以可以动态扩容~~。~~ 下面看到是基于PriorityQueue实现的。它插入元素的顺序并不影响最终的输出顺序。但PriorityQueue底层也是依赖于Object[]实现的。

而**元素的排序依赖于 compareTo 方法进行排序**，也就是休眠的时间长短决定的。**同时只有实现了 Delayed 接口，才能存放元素**。
### 4.2.1 参数
一个可重入锁，一个优先队列，然后是线程和Condition  Leader-Follower模式
```java
import java.util.PriorityQueue;  
import java.util.concurrent.locks.Condition;  
import java.util.concurrent.locks.ReentrantLock;

...
private final transient ReentrantLock lock = new ReentrantLock();  
private final PriorityQueue<E> q = new PriorityQueue<E>();
/**  
 * Thread designated to wait for the element at the head of 
 * the queue.  This variant of the Leader-Follower pattern 
 * (http://www.cs.wustl.edu/~schmidt/POSA/POSA2/) serves to 
 * minimize unnecessary timed waiting.  When a thread becomes 
 * the leader, it waits only for the next delay to elapse, but 
 * other threads await indefinitely.  The leader thread must 
 * signal some other thread before returning from take() or 
 * poll(...), unless some other thread becomes leader in the 
 * interim.  Whenever the head of the queue is replaced with 
 * an element with an earlier expiration time, the leader 
 * field is invalidated by being reset to null, and some 
 * waiting thread, but not necessarily the current leader, is 
 * signalled.  So waiting threads must be prepared to acquire 
 * and lose leadership while waiting. 
 */
private Thread leader;  
  
/**  
 * Condition signalled when a newer element becomes available 
 * at the head of the queue or a new thread may need to 
 * become leader. 
 */
private final Condition available = lock.newCondition();
```
### 4.2.2 元素入队源码分析-delayQueue.offer(new TestDelayed("aaa", 5, TimeUnit.SECONDS));
BlockingQueue有四类操作：`with different ways of handling operations that cannot be satisfied immediately`
- add(e)、remove()、element() ：无法加入/移除并返回首部元素/返回首部元素时，抛出IllegalStateException / NoSuchElementException / NoSuchElementException 
- offer(e)、poll()、peek() ：无法加入/移除并返回首部元素/返回首部元素时，返回特定值 false / null / null
- put(e)、take() ：一直等待、直到条件满足
- offer(e, time, unit)、poll(time, unit) ：等有限时间
```java
BlockingQueue methods come in four forms, with different ways of handling operations that cannot be satisfied immediately, 
but may be satisfied at some point in the future: one throws an exception, 
the second returns a special value (either null or false, depending on the operation), 
the third blocks the current thread indefinitely until the operation can succeed, 
and the fourth blocks for only a given maximum time limit before giving up. These methods are summarized in the following table:
```
==但在DelayQueue中add(e)、offer(e)、put(e)和offer(e, time, unit)都是相同的，因为DelayQueue无界，不会被阻塞！！！==

```java
/**  
 * Inserts the specified element into this delay queue. * * @param e the element to add  
 * @return {@code true} (as specified by {@link Collection#add})  
 * @throws NullPointerException if the specified element is null  
 */
 public boolean add(E e) {  
    return offer(e);  
}  
  
/**  
 * Inserts the specified element into this delay queue. * * @param e the element to add  
 * @return {@code true}  
 * @throws NullPointerException if the specified element is null  
 */
 public boolean offer(E e) {  
    final ReentrantLock lock = this.lock;  
    lock.lock();  
    try {  
        q.offer(e);  
        if (q.peek() == e) {  
            leader = null;  
            available.signal();  
        }  
        return true;  
    } finally {  
        lock.unlock();  
    }  
}  
  
/**  
 * Inserts the specified element into this delay queue. As the queue is 
 * unbounded this method will never block. 
 * 
 * @param e the element to add  
 * @throws NullPointerException {@inheritDoc}  
 */
public void put(E e) {  
    offer(e);  
}

/**  
 * Inserts the specified element into this delay queue. As the queue is 
 * unbounded this method will never block. 
 * 
 * @param e the element to add  
 * @param timeout This parameter is ignored as the method never blocks  
 * @param unit This parameter is ignored as the method never blocks  
 * @return {@code true}  
 * @throws NullPointerException {@inheritDoc}  
 */
public boolean offer(E e, long timeout, TimeUnit unit) {  
    return offer(e);  
}
```
### 4.2.3 元素出队-delayQueue.take()
特别说明的是，remove(Object o)方法很少使用：不论是否过期都出去一个元素
```java
/**  
 * Removes a single instance of the specified element from this 
 * queue, if it is present, whether or not it has expired. 
 **/
 public boolean remove(Object o) {  
    final ReentrantLock lock = this.lock;  
    lock.lock();  
    try {  
        return q.remove(o);  
    } finally {  
        lock.unlock();  
    }  
}
```
下面是其他三个出队方法poll()：出队过期的队首元素、没有则返回null，不会因为等待元素而阻塞
```java
/**  
 * Retrieves and removes the head of this queue, or returns {@code null}  
 * if this queue has no elements with an expired delay. 
 * 
 * @return the head of this queue, or {@code null} if this  
 *         queue has no elements with an expired delay 
 */
 public E poll() {  
    final ReentrantLock lock = this.lock;  
    lock.lock();  
    try {  
        E first = q.peek();  
        return (first == null || first.getDelay(NANOSECONDS) > 0)  
            ? null  
            : q.poll();  
    } finally {  
        lock.unlock();  
    }  
}  
```
take()等待过期元素的出现并返回，
- 这部分的代码有点长，主要是元素的获取。
- DelayQueue 是 Leader-Followr 模式的变种，消费者线程处于等待 await 时，总是等待最先休眠完成的元素。
- 这里会最小化的空等时间，提高线程利用率。数据结构讲完后，后面会有专门章节介绍

poll(time, unit)
```java
/**  
 * Retrieves and removes the head of this queue, waiting if necessary   
 * until an element with an expired delay is available on this queue.  
 * 
 * @return the head of this queue  
 * @throws InterruptedException {@inheritDoc}  
 */
public E take() throws InterruptedException {  
    final ReentrantLock lock = this.lock;  
    lock.lockInterruptibly();  // 不太明白
    try {  
        for (;;) {  
            E first = q.peek();  
            if (first == null)  
                available.await();  // 释放锁
            else {  
                long delay = first.getDelay(NANOSECONDS);  
                if (delay <= 0L)  // 直到过期元素出现
                    return q.poll();  
                first = null; // don't retain ref while waiting  
                if (leader != null)  
                    available.await();  
                else {  
                    Thread thisThread = Thread.currentThread();  
                    leader = thisThread;  
                    try {  
                        available.awaitNanos(delay);  
                    } finally {  
                        if (leader == thisThread)  
                            leader = null;  
                    }  
                }  
            }  
        }  
    } finally {  
        if (leader == null && q.peek() != null)  
            available.signal();  
        lock.unlock();  
    }  
}  
  
/**  
 * Retrieves and removes the head of this queue, waiting if necessary 
 * until an element with an expired delay is available on this queue, 
 * or the specified wait time expires. 
 * 
 * @return the head of this queue, or {@code null} if the  
 *         specified waiting time elapses before an element with 
 *         an expired delay becomes available 
 * @throws InterruptedException {@inheritDoc}  
 */public E poll(long timeout, TimeUnit unit) throws InterruptedException {  
    long nanos = unit.toNanos(timeout);  
    final ReentrantLock lock = this.lock;  
    lock.lockInterruptibly();  
    try {  
        for (;;) {  
            E first = q.peek();  
            if (first == null) {  
                if (nanos <= 0L)  
                    return null;  
                else                    nanos = available.awaitNanos(nanos);  
            } else {  
                long delay = first.getDelay(NANOSECONDS);  
                if (delay <= 0L)  
                    return q.poll();  
                if (nanos <= 0L)  
                    return null;  
                first = null; // don't retain ref while waiting  
                if (nanos < delay || leader != null)  
                    nanos = available.awaitNanos(nanos);  
                else {  
                    Thread thisThread = Thread.currentThread();  
                    leader = thisThread;  
                    try {  
                        long timeLeft = available.awaitNanos(delay);  
                        nanos -= delay - timeLeft;  
                    } finally {  
                        if (leader == thisThread)  
                            leader = null;  
                    }  
                }  
            }  
        }  
    } finally {  
        if (leader == null && q.peek() != null)  
            available.signal();  
        lock.unlock();  
    }  
}
```
# 5. 还有哪些队列
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042104322.png)
![](https://image-1307616428.cos.ap-beijing.myqcloud.com/Obsidian/202305042106598.png)
除了我们已经讲过的队列以外，剩余的基本都是阻塞队列，也就是上面这些。

在数据结构方面基本没有差异，只不过添加了相应的阻塞功能和锁的机制。

## 5.2 使用案例
这是一个 LinkedBlockingQueue 队列使用案例，**一方面存储数据，一方面从
队列中获取进行消费**。

因为这是一个阻塞队列，所以在获取元素的时候，如果队列为空，会进行阻塞。

LinkedBlockingQueue 是一个阻塞队列，**内部由两个 ReentrantLock 来实现出入队列的线程安全**，==由各自的 Condition 对象的 await 和 signal 来实现等待和唤醒功能==。
```java
public class DataQueueStack {
	private BlockingQueue<DataBean> dataQueue = null;
	public DataQueueStack() {
		//实例化队列
		dataQueue = new LinkedBlockingQueue<DataBean>(100);
	}
   /**
	* 添加数据到队列
	* @param dataBean
	* @return
	*/
	public boolean doOfferData(DataBean dataBean) {
		try {
			return dataQueue.offer(dataBean, 2, TimeUnit.SECONDS);
		} catch (InterruptedException e) {
			e.printStackTrace();
			return false;
		}
	}
   /**
	* 弹出队列数据
	* @return
	*/
	public DataBean doPollData() {
		try {
			return dataQueue.poll(2, TimeUnit.SECONDS);
		} catch (InterruptedException e) {
			e.printStackTrace();
			return null;
		}
	}
   /**
	* 获得队列数据个数
	* @return
	*/
	public int doGetQueueCount() {
		return dataQueue.size();
	}
}
```

这里还有一些关于**阻塞队列锁🔒的应用过程**，到我们后面讲锁相关知识点，再重点介绍。

队列结构的设计非常适合某些需要 LIFO 或者 FIFO 的应用场景，同时在队列的数据结构中也有双端、延时和组合的功能类，使用起来也非常方便。