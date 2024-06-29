> 本文属于「算法基础」系列文章之一。较「算法学习」系列不同的是，这一系列更着重于基础的数据结构和算法设计课程的学习，不涉及到高级数据结构和算法的运用。此外，在本系列学习文章中，为了透彻讲解算法和代码，本人参考了诸多博客、教程、文档、书籍等资料，由于精力有限，恕不能一一列出。
> <b></b>
> 为了方便在PC上运行调试、分享代码，我还建立了相关的仓库：[https://github.com/memcpy0/Data-Structure-and-Algorithm.git](https://github.com/memcpy0/Data-Structure-and-Algorithm.git)。在这一仓库中，你可以看到算法文章、模板代码、应用题目等等。由于本系列文章的内容随时可能发生更新变动，欢迎关注和收藏本文以作备忘。


@[toc]


---
# 一、目标
实现一个小型的数据结构库，从0开始用Java实现。包括：数组，栈，队列，链表，二分搜索树，堆，线段树，trie，并查集，AVL，红黑树，哈希表，邻接矩阵，邻接表等。

此外，我还想深入学习一下图论的算法。不过这是在另一个分类专栏中涉及。

# 二、数组基础
无论是C/C++，还是Java中的数组，都是连续存储的一维线性空间。因为是连续存储的，所以数组拥有`索引`的特性，这也是数组最重要的特性——快速查询，所以数组最好应用于“索引有语义”的情况（类似于哈希表）。但这一节，**我们主要处理“索引没有语义”的情况下数组的使用**。

Java中的数组，相信都使用过。我们就基于Java的数组，二次封装属于我们自己的**动态数组**类，实现**在索引没有语义的时候表示“没有元素”，以及添加元素和删除元素的方法**等。

# 三、基本Array设计与编码
- `capacity`：数组的最大容量；
- `size`：数组中元素的数量；
- `Array()`：无参数构造函数，默认数组的容量capacity=10；
- `Array(int)`：构造函数, 传入数组的容量capacity构造Array；
- `getSize()`：获取数组中的实际元素个数；
- `getCapacity()`：获取数组的容量；
- `isEmpty()`：判断数组是否为空。

上面的是最基本的方法。然后就是增删改查这些方法。

向数组中添加元素，最简单的是添加到数组末尾处(`size`指向的位置)，O(1)。
- `add(int index, int e)`：向index位置添加元素e；Array**存放元素必须连续**，因此需要检查index是否合法；
- `addLast(int)`：向所有元素后添加元素；复用add；
- `addFirst(int)`：向所有元素之前添加元素；复用add；

获得和设置Array中的元素：通过合法性检查，Array不会像被访问到非法空间；
- `get(int index)`：获得index索引位置的元素；
- `set(int index, int e)`：设置index位置的元素为e；

数组的包含、搜索、删除操作：
- `contains(int e)` ：查找数组是否有元素e；
- `find(int e)` ： 查找数组中元素e的索引, 不存在e则返回-1；
- `remove(int index)`：删除对应索引位置的元素，同时返回这个删除的元素；
- `removeFirst()`：`remove`的快捷方法，删除第一个元素；
- `removeLast()`：`remove`的快捷方法，删除最后一个元素；
- `removeElement(int e)`：利用前面的函数`find`和`remove`，设计的新接口，删除数组中的第一个元素e。

当然，接口的设计全凭个人的喜好，比如可以设计一个`removeAllElements`，删除所有的e元素；`remove`返回boolean值，确认是否真正删除了元素；...
```java

public class Array {
	private int[] data; //先只让数组存储int型
	private int size;     
	//private int capacity; //用data.length替代即可
	
	//无参数构造函数, 默认数组的容量capacity=10
	public Array() {
		this(10); //构造函数重载
	}
	
	//构造函数, 传入数组的容量capacity构造Array
	public Array(int capacity) {
		data = new int[capacity];
		size = 0;
	}
	
	//获取数组中的元素个数
	public int getSize() {
		return size;
	}
	//获取数组的容量
	public int getCapacity() {
		return data.length;
	}
	//判断数组是否为空
	public boolean isEmpty() {
		return size == 0;
	}
	
	//在index位置添加一个元素
	public void add(int index, int e) {
		if (size == data.length) 
			throw new IllegalArgumentException("Add failed. Array is full.");
		
		if (index < 0 || index > size) 
			throw new IllegalArgumentException("Add failed. Require index >= 0 and <= size.");
		
		for (int i = size - 1; i >= index; --i)
			data[i + 1] = data[i];
		data[index] = e;
		++size;
	}
	//向数组开头添加元素
	public void addFirst(int e) {
		add(0, e); //复用
	}
	//向数组末尾添加元素
	public void addLast(int e) { 
		add(size, e);
	}
	
	//获得index位置的元素
	public int get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get failed. Index is illegal.");
		
		return data[index];
	}
	//设置index位置的元素为e
	public void set(int index, int e) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set failed. Index is illegal.");
		
		data[index] = e;
	}
	
	//查找数组是否有元素e
	public boolean contains(int e) {
		for (int i = 0; i < size; ++i) {
			if (data[i] == e)
				return true;
		}
		return false;
	}
	//查找数组中元素e的索引, 不存在e则返回-1
	public int find(int e) {
		for (int i = 0; i < size; ++i) {
			if (data[i] == e)
				return i;
		}
		return -1;
	}
	//删除对应索引位置的元素, 同时返回这个删除的元素
	public int remove(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Remove failed. Index is illegal.");
		
		int ret = data[index];
		for (int i = index + 1; i < size; ++i)
			data[i - 1] = data[i];
		--size;
		return ret;
	}
	//remove的快捷方法
	public int removeFirst() {
		return remove(0);
	}
	public int removeLast() {
		return remove(size - 1);
	}
	//删除数组中的第一个元素e
	public void removeElement(int e) {
		int index = find(e);
		if (index != -1)
			remove(index);
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("Array: size = %d, capacity= %d\n", size, data.length));
		sb.append('[');
		for (int i = 0; i < size; ++i) {
			sb.append(data[i]);
			if (i != size - 1)
				sb.append(", ");
		}
		sb.append(']');
		return sb.toString();
	}
	
	public static void main(String[] args) {
		Array arr = new Array(20);
		for (int i = 0; i < 10; ++i)
			arr.addLast(i);
		System.out.println(arr);
		
		arr.add(1, 100);
		System.out.println(arr);
		
		arr.addFirst(-1);
		System.out.println(arr);
		
		arr.addLast(9999);
		System.out.println(arr);
		
		System.out.println(arr.get(arr.getSize() - 1));
		arr.set(arr.getSize() - 1, -1111);
		System.out.println(arr);
		
		if (arr.contains(0)) System.out.println("This array has element zero.");
		
		arr.remove(2);
		System.out.println(arr); //少了100
		
		arr.removeElement(4);
		System.out.println(arr);
		
		arr.removeFirst();  //少了-1
		System.out.println(arr);
	} 
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200310194654718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
 
# 四、使用泛型
在C++中对应的是模板，在C中对应的会是`void*`。如果没有意外的话，我用C、C++和Java都实现一遍常用的数据结构。

使用泛型，可以让我们的数组存放“任意类型”的数据，不仅是内部类型，也包括自定义类型。当然，Java中不能够真正放置任意的类型，如果使用了泛型，那么我们就不能存放基本的8种值类型——而需要用包装类`Boolean, Byte, Char, Short, Int, Long, Float, Double`。

参数化类型，声明泛型数组用`E[]`，申请则要使用这样的语句`(E[]) new Object[]`。
```cpp
public class Array<E> { //参数化类型
	private E[] data;
	private int size;     
	//private int capacity; //用data.length替代即可
	
	//无参数构造函数, 默认数组的容量capacity=10
	public Array() {
		this(10); //构造函数重载
	}
	
	//构造函数, 传入数组的容量capacity构造Array
	@SuppressWarnings("unchecked")
	public Array(int capacity) {
		data = (E[]) new Object[capacity]; //不能直接创建一个泛型数组
		size = 0;
	}
	
	//获取数组中的元素个数
	public int getSize() {
		return size;
	}
	//获取数组的容量
	public int getCapacity() {
		return data.length;
	}
	//判断数组是否为空
	public boolean isEmpty() {
		return size == 0;
	}
	
	//在index位置添加一个元素
	public void add(int index, E e) {
		if (size == data.length) 
			throw new IllegalArgumentException("Add failed. Array is full.");
		
		if (index < 0 || index > size) 
			throw new IllegalArgumentException("Add failed. Require index >= 0 and <= size.");
		
		for (int i = size - 1; i >= index; --i)
			data[i + 1] = data[i];
		data[index] = e;
		++size;
	}
	//向数组开头添加元素
	public void addFirst(E e) {
		add(0, e); //复用
	}
	//向数组末尾添加元素
	public void addLast(E e) { 
		add(size, e);
	}
	
	//获得index位置的元素
	public E get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get failed. Index is illegal.");
		
		return data[index];
	}
	//设置index位置的元素为e
	public void set(int index, E e) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set failed. Index is illegal.");
		
		data[index] = e;
	}
	
	//查找数组是否有元素e
	public boolean contains(E e) {
		for (int i = 0; i < size; ++i) {
			if (data[i].equals(e)) //两个类对象之间进行值比较, 应该用equals
				return true;
		}
		return false;
	}
	//查找数组中元素e的索引, 不存在e则返回-1
	public int find(E e) {
		for (int i = 0; i < size; ++i) {
			if (data[i].equals(e))
				return i;
		}
		return -1;
	}
	//删除对应索引位置的元素, 同时返回这个删除的元素
	public E remove(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Remove failed. Index is illegal.");
		/*使用泛型数组, data数组中存储的是对象的引用, 下面的删除逻辑中变化前的size
		指向的引用任然存在, 不会给GC回收, 这里我们需要注意 */
		E ret = data[index];
		for (int i = index + 1; i < size; ++i)
			data[i - 1] = data[i];
		--size;
		data[size] = null; //loitering objects //当然, 不写这句话, 逻辑也成立
		return ret;
	}
	//remove的快捷方法
	public E removeFirst() {
		return remove(0);
	}
	public E removeLast() {
		return remove(size - 1);
	}
	//删除数组中的第一个元素e
	public void removeElement(E e) {
		int index = find(e);
		if (index != -1)
			remove(index);
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("Array: size = %d, capacity= %d\n", size, data.length));
		sb.append('[');
		for (int i = 0; i < size; ++i) {
			sb.append(data[i]);
			if (i != size - 1)
				sb.append(", ");
		}
		sb.append(']');
		return sb.toString();
	}
	
	public static void main(String[] args) {
		//由于有自动拆装箱, 只要改一下下面的声明语句就可以了
		//Array arr = new Array(20);
		Array<Integer> arr = new Array<>(20);
		for (int i = 0; i < 10; ++i)
			arr.addLast(i);
		System.out.println(arr);
		
		arr.add(1, 100);
		System.out.println(arr);
		
		arr.addFirst(-1);
		System.out.println(arr);
		
		arr.addLast(9999);
		System.out.println(arr);
		
		System.out.println(arr.get(arr.getSize() - 1));
		arr.set(arr.getSize() - 1, -1111);
		System.out.println(arr);
		
		if (arr.contains(0)) System.out.println("This array has element zero.");
		
		arr.remove(2);
		System.out.println(arr); //少了100
		
		arr.removeElement(4);
		System.out.println(arr);
		
		arr.removeFirst();  //少了-1
		System.out.println(arr);
	} 
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200310194654718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
自己定义了一个Student类型，使用Array存储：
```cpp
public class Student {
	private int score;
	private String name;
	
	public Student(String _name, int _score) {
		name = _name;
		score = _score;
	}
	
	@Override 
	public String toString() {
		return String.format("Student(name: %s, socre: %d)", name, score);	
	}
	
	public static void main(String[] args) {
		Array<Student> arr = new Array<>();
		arr.addLast(new Student("Alice", 100));
		arr.addLast(new Student("Bob", 77));;
		arr.addLast(new Student("Charlie", 66));
		System.out.println(arr);

	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200310200054458.png)

不出意外，以后的数据结构都会直接使用泛型来实现。
# 五、动态数组
所谓的动态增长，实际是在类内部，自动使用新的更长的内部数组存储原数组的数据，因此每次调用是$O(n)$。当然，也可以动态减少，当size很小时，可以自动`shrink_to_fit`以节省空间，Java中的是`trimToSize`。

这里，每次扩容是$2$倍，在邓俊辉老师的课程中曾经讲过为什么选择$2$的原因。每次如果数组的元素数目减少到了仅为容量的一半时，收缩空间为$size$大小。

```cpp
//最终版本的动态数组
public class Array<E> { //参数化类型
	private E[] data;
	private int size;     
	//private int capacity; //用data.length替代即可
	
	//无参数构造函数, 默认数组的容量capacity=10
	public Array() {
		this(10); //构造函数重载
	}
	
	//构造函数, 传入数组的容量capacity构造Array
	@SuppressWarnings("unchecked")
	public Array(int capacity) {
		data = (E[]) new Object[capacity]; //不能直接创建一个泛型数组
		size = 0;
	}
	
	//获取数组中的元素个数
	public int getSize() {
		return size;
	}
	//获取数组的容量
	public int getCapacity() {
		return data.length;
	}
	//判断数组是否为空
	public boolean isEmpty() {
		return size == 0;
	}
	
	//在index位置添加一个元素
	public void add(int index, E e) {
		if (index < 0 || index > size) 
			throw new IllegalArgumentException("Add failed. Require index >= 0 and <= size.");
		
		if (size == data.length) //不像以前那样抛出异常
			resize(2 * data.length);
		
		for (int i = size - 1; i >= index; --i)
			data[i + 1] = data[i];
		data[index] = e;
		++size;
	}
	//向数组开头添加元素
	public void addFirst(E e) {
		add(0, e); //复用
	}
	//向数组末尾添加元素
	public void addLast(E e) { 
		add(size, e);
	}
	
	//获得index位置的元素
	public E get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get failed. Index is illegal.");
		
		return data[index];
	}
	//得到第一个元素
	public E getFirst() {
		return get(0);
	}
	//得到最后一个元素
	public E getLast() {
		return get(size - 1);
	}
	//设置index位置的元素为e
	public void set(int index, E e) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set failed. Index is illegal.");
		
		data[index] = e;
	}
	
	//查找数组是否有元素e
	public boolean contains(E e) {
		for (int i = 0; i < size; ++i) {
			if (data[i].equals(e)) //两个类对象之间进行值比较, 应该用equals
				return true;
		}
		return false;
	}
	//查找数组中元素e的索引, 不存在e则返回-1
	public int find(E e) {
		for (int i = 0; i < size; ++i) {
			if (data[i].equals(e))
				return i;
		}
		return -1;
	}
	
	//删除对应索引位置的元素, 同时返回这个删除的元素
	public E remove(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Remove failed. Index is illegal.");
		/*使用泛型数组, data数组中存储的是对象的引用, 下面的删除逻辑中变化前的size
		指向的引用任然存在, 不会给GC回收, 这里我们需要注意 */
		E ret = data[index];
		for (int i = index + 1; i < size; ++i)
			data[i - 1] = data[i];
		--size;
		data[size] = null; //loitering objects //当然, 不写这句话, 逻辑也成立
		
		if (size == data.length / 2) //如果减少元素数目到等于Capacity的一半, 不能写小于或大于
			resize(data.length / 2);
		return ret;
	}
	//remove的快捷方法
	public E removeFirst() {
		return remove(0);
	}
	public E removeLast() {
		return remove(size - 1);
	}
	//删除数组中的第一个元素e
	public void removeElement(E e) {
		int index = find(e);
		if (index != -1)
			remove(index);
	}
	
	//动态数组的能力
	private void resize(int newCapacity) {
		E[] newData = (E[])new Object[newCapacity];
		for (int i = 0; i < size; ++i)
			newData[i] = data[i];
		data = newData;
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("Array: size = %d, capacity= %d\n", size, data.length));
		sb.append('[');
		for (int i = 0; i < size; ++i) {
			sb.append(data[i]);
			if (i != size - 1)
				sb.append(", ");
		}
		sb.append(']');
		return sb.toString();
	}
	
	public static void main(String[] args) { 
		Array<Integer> arr = new Array<>(); //默认的容量10个
		for (int i = 0; i < 10; ++i)
			arr.addLast(i);
		System.out.println(arr);
		
		arr.add(1, 100);
		System.out.println(arr);
		
		arr.addFirst(-1);
		System.out.println(arr);
 
		arr.remove(2);
		System.out.println(arr);
		
		arr.removeElement(4);
		System.out.println(arr);
		
		arr.removeFirst();
		System.out.println(arr); 
	} 
}
```




![在这里插入图片描述](https://img-blog.csdnimg.cn/20200310202053176.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
# 六、简单的时间复杂度分析
$O(n)$描述的是一个渐进时间复杂度，相当于$n$趋近于无穷时候的情况。其中，$O(1)$表示操作消耗的时间与数据的规模无关。

- `getSize()`：获取数组中的实际元素个数，$O(1)$；
- `getCapacity()`：获取数组的容量，$O(1)$；
- `isEmpty()`：判断数组是否为空，$O(1)$。 

- `add(int index, int e)`：向index位置添加元素e，$O(n)$；
- `addLast(int)`：在最后一个位置添加元素，$O(1)$；
- `addFirst(int)`：在第一个位置添加元素，$O(n)$；


- `get(int index)`：获得index索引位置的元素，$O(1)$；
- `getFirst()`：$O(1)$；
- `getLast()`：$O(1)$；
- `set(int index, int e)`：设置index位置的元素为e，$O(1)$；
 
- `contains(int e)` ：查找数组是否有元素e，$O(n)$；
- `find(int e)` ： 查找数组中元素e的索引, 不存在e则返回-1，$O(n)$；
- `remove(int index)`：删除对应索引位置的元素，同时返回这个删除的元素，$O(n)$；
- `removeFirst()`：`remove`的快捷方法，删除第一个元素，$O(n)$；
- `removeLast()`：`remove`的快捷方法，删除最后一个元素，$O(1)$；
- `removeElement(int e)`：删除数组中的第一个元素e，$O(n)$；
- `resize(int newCapacity)`：$O(n)$。

# 七、迭代接口的实现【最终修改】
为了让 $Array$ 更加接近 $ArrayList$，在 $Array$ 上面使用 `foreach` 结构，我们需要实现 $Iterable$ 接口。代码如下，对前面的代码有一小点的修改：
```java
public class Array<E> implements Iterable<E> { //参数化类型 //实现可迭代接口
    /*
    * 将迭代器置为内部类，能够直接访问ADT的数据域，迭代效率高
    * */
	@Override
	public Iterator<E> iterator() { //实现Iterable接口需要实现一个public Iterator<E> iterator方法
		return new ArrayIterator();
	} 
	private class ArrayIterator implements Iterator<E> { //实现迭代器接口
		private int idx; //记录当前位置的游标 
		public ArrayIterator() {
			idx = 0; //将游标指向0
		}
		
		@Override
		public boolean hasNext() {
			return idx < size; //idx没有超出数组范围时，就还有下一个
		} 
		@Override 
		public E next() {
			if (!hasNext())
				throw new NoSuchElementException("No elements.");
			return data[idx++]; //返回现在的元素; idx+1指向下一个位置
		}
		//此迭代器没有实现删除功能, 只是抛出UnsupportedOperationException
		@Override
		public void remove() {
			throw new UnsupportedOperationException("This iterator doesn't implement the remove operation!");
		}
	}
	...
	public static void main(String[] args) { 
		Array<Integer> arr = new Array<>(); //默认的容量10个
        ...
		Iterator<Integer> it = arr.iterator();
		while (it.hasNext())
			System.out.println(it.next() + " ");
		
		for (int i : arr)
			System.out.println(i + " ");
		...
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326014617809.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
# 八、resize的均摊时间复杂度分析[Amortized-Time-Complexity]
对于$resize$函数，它不会每次都被触发，因此我们在分析相关的$add$、$remove$等涉及到了$resize$的操作时，如果都从最坏的时间复杂度来分析，就有点不妥。

对于`addLast`，假设`Capacity = n`，`n + 1`次`addLast`，触发$resize$，总共进行了$2n+1$次操作，平均每次`addLast`操作，进行了$2$次基本操作，是$O(1)$级别的。比计算最坏情况更有意义。这被称为“均摊复杂度”。

对于一个比较耗时的操作，如果我们可以保证它不会每次都触发，那么它的复杂度可以均摊到其他的操作中。

如果分别考虑`addLast`和`removeLast`，都是$O(1)$。但是我们同时考虑这两者的时候，**复杂度震荡**的情况也会出现。当`size=10， Capacity=10`时，`addLast`扩容，$O(n)$，再`removeLast`，收缩，$O(n)$，...，如此反复，每一个操作都是$O(n)$....

出现问题的原因在于，我们`removeLast`时`resize`过于着急，要使用$Lazy$的方案：当`addLast`时空间满了，一定要扩容，这个不可能等待；但是`removeLast`，不一定立刻收缩，我们可以等一等，等到$size = Capacity / 4$，才将`Capacity`减半。

改写的实际代码如下：
```cpp
//删除对应索引位置的元素, 同时返回这个删除的元素
public E remove(int index) {
	if (index < 0 || index >= size)
		throw new IllegalArgumentException("Remove failed. Index is illegal.");
	/*使用泛型数组, data数组中存储的是对象的引用, 下面的删除逻辑中变化前的size
	指向的引用任然存在, 不会给GC回收, 这里我们需要注意 */
	E ret = data[index];
	for (int i = index + 1; i < size; ++i)
		data[i - 1] = data[i];
	--size;
	data[size] = null; //loitering objects //当然, 不写这句话, 逻辑也成立
	
	//如果减少元素数目到等于Capacity的1/4, 不能写小于或大于
	//length可能等于1, 那么size=0,length/4等于0...不可能申请一个大小为0的数组
	if (size == data.length / 4 && data.length / 2 != 0) 
		resize(data.length / 2);
	return ret;
}
```

