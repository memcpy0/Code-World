@[toc]

前面的所谓动态数组，实际上是靠$resize$这种操作实现的，是对用户来说的。而链表，是真正的动态数据结构，也是最简单的动态数据结构（更难的有二叉搜索树，trie树，红黑树等）。

学习链表，可以让我们**更加深入的理解Java的引用/C++的指针**；而且链表是递归数据结构，链表的操作基本上都可以用递归实现，我们能**更深入的理解递归**。

## 一、链表LinkedList
在链表中，数据存储在结点$Node$中。一个结点的$next$如果是$null$，那它就是链表的尾结点。下面的实现中，将$Node$设计为内部类。
```java
class Node {
	E e;
	Node next;
}
```
链表的优点，在于真正的动态，不需要处理固定容量的问题，更高效的使用空间；缺点，丧失了随机访问的能力。

为了提高链表的访问能力，有$dalao$设计了$skiplist$跳跃表这种数据结构，有时间我会在后面介绍。

## 二、无头单链表实现
在LinkedList中，我们实现的是无头的单链表，有一个`Node`型的引用$head$，它指向链表的第一个数据结点。

### 1. 基本的方法
- `Node(E e, Node next)`：$Node$的构造方法；
- `Node(E e)`、`Node()`：$Node$的构造方法；
- `LinkedList()`：链表的构造方法
- `size`：私有变量，表示链表中的元素个数，即链表的长度；
- `head`：头引用；
- `getSize()`：得到链表的大小/长度；
- `isEmpty()`：链表是否为空。
### 2. 添加元素
对于单链表，在链表头添加结点是最简单的；而在数组中，在数组尾添加元素则是最简单的。

- `addFirst(E e)`：在链表头添加元素，$O(1)$；
- `add(int index)`：在链表中间，索引为$index$的地方添加元素（$index \geq 0$），**关键在于找到要添加的结点的前一个结点**，其中如果在索引为0的位置添加元素，没有前一个结点，只有头指针(引用)，**需要特判**，$O(n)$；
 
  当然，真正使用链表的时候，**我们很少使用这样的操作**，因为选择使用链表的时候，我们往往就选择了不使用索引。后续我们使用链表组建其他的数据结构时，会完全摒弃掉这个操作；
- `addLast()`：复用`add`方法，在$size$的位置添加元素就好了，$O(n)$；

目前的无头单链表的代码如下：
```java 
public class LinkedList<E> {
	
	private class Node {
		public E e;
		public Node next;
		
		public Node(E e, Node next) {
			this.e = e;
			this.next = next;
		}
		public Node(E e) {
			this(e, null);
		}
		public Node() {
			this(null, null);
		}
		
		@Override
		public String toString() {
			return e.toString();
		}
	}
	
	private Node head;
	private int size;
	
	public LinkedList() {
		head = null;
		size = 0;
	}
	
	//获取链表中的元素长度
	public int getSize() {
		return size;
	}
	
	//返回链表是否为空
	public boolean isEmpty() {
		return size == 0;
	}
	
	//在链表头添加元素
	public void addFirst(E e) {
		//Node node = new Node(e);
		//node.next = head;
		//head = node;
		head = new Node(e, head); 
		++size;
	}
	
	//在链表的index(0-based)位置添加新的元素e
	//不是常用的操作, 仅仅作为练习
	public void add(int index, E e) {
		if (index < 0 || index > size)
			throw new IllegalArgumentException("Add failed. Illegal index.");
	
		if (index == 0) //在无头链表
			addFirst(e);
		else {
			Node prev = head;
			for (int i = 0; i < index - 1; ++i)
				prev = prev.next;
			//Node node = new Node(e);
			//node.next = prev.next;
			//prev.next = node;
			prev.next = new Node(e, prev.next);
		}
		++size;
	}
	
	public void addLast(E e) {
		add(size, e);
	}
	
	public static void main(String[] args) { 
	} 
} 
```
## 三、带头单链表的实现
### 1. 带头单链表添加元素的实现
我们发现前面的无头单链表添加结点时，需要特判，不够优雅...为了统一操作，我们为链表设立一个虚拟头结点：$dummyHead$。对于一个空链表，实际上是存在一个结点的，就是$dummyHead$。这样，即使在链表头添加结点，也可以找到前一个结点，就是$dummyHead$。

操作就是前面的`add(int index, E e)`，`addFirst(E e)`，`addLast(E e)`。从$dummyHead$出发，走$index$步，走到要添加的位置的前一个结点处。

```cpp 
public class LinkedList<E> { 
	private class Node {
		public E e;
		public Node next; 
		
		public Node(E e, Node next) {
			this.e = e;
			this.next = next;
		}
		public Node(E e) {
			this(e, null);
		}
		public Node() {
			this(null, null);
		}
		
		@Override
		public String toString() {
			return e.toString();
		}
	}
	
	private Node dummyHead;
	private int size;
	
	public LinkedList() {
		dummyHead = new Node(null, null); //虚拟头结点初始化
		size = 0;
	}
	
	//获取链表中的元素长度
	public int getSize() {
		return size;
	}
	
	//返回链表是否为空
	public boolean isEmpty() {
		return size == 0;
	}
		
	//在链表的index(0-based)位置添加新的元素e
	//不是常用的操作, 仅仅作为练习
	public void add(int index, E e) {
		if (index < 0 || index > size)
			throw new IllegalArgumentException("Add failed. Illegal index.");
	
		
		Node prev = dummyHead; //从dummyhead开始遍历
		for (int i = 0; i < index; ++i)
			prev = prev.next;
		//Node node = new Node(e);
		//node.next = prev.next;
		//prev.next = node;
		prev.next = new Node(e, prev.next); 
		
		++size;
	}
	//在链表头添加元素
	public void addFirst(E e) {
		add(0, e);
	} 
	public void addLast(E e) {
		add(size, e);
	} 
	public static void main(String[] args) { 
	} 
} 
```
### 2. 带头单链表的遍历、查询、修改
- `get(int index)`：获得链表的$index$位置(0-based)的元素，$O(n)$；
- `getFirst()`：获得链表的第一个元素，$O(1)$；
- `getLast()`：获得链表的最后一个元素，$O(n)$；
- `set(int index, E e)`：修改链表的index(0-based)位置的元素为e，$O(n)$；
- `contains(E e)`：是否含有元素e，$O(n)$；

```java
public class LinkedList<E> {
	//.................
	//获得链表的index位置(0-based)的元素
	public E get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get fail. Illegal index.");
		
		Node cur = dummyHead.next;
		for (int i = 0; i < index; ++i)
			cur = cur.next;
		return cur.e;
	}
	//获得链表的第一个元素
	public E getFirst() {
		return get(0);
	}
	//获得链表的最后一个元素
	public E getLast() {
		return get(size - 1);
	}
	
	//修改链表的index(0-based)位置的元素为e
	public void set(int index, E e) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set fail. Illegal index.");
		
		Node cur = dummyHead.next;
		for (int i = 0; i < index; ++i)
			cur = cur.next;
		cur.e = e;
	}
	
	public boolean contains(E e) {
		Node cur = dummyHead.next;
		
		while (cur != null) {
			if (cur.e.equals(e)) //发现了一个bug
				return true;
			cur = cur.next;
		}
		return false;
	}
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		
		Node cur = dummyHead.next;
		while (cur != null) {
			res.append(cur.e + "->");
			cur = cur.next;
		}
		res.append("NULL");
		return res.toString();
	}
	
	public static void main(String[] args) {
		 LinkedList<Integer> linkedList = new LinkedList<>();
		 for (int i = 0; i < 5; ++i) {
			 linkedList.addFirst(i);
			 System.out.println(linkedList);
		 }
		 linkedList.add(2,  666);
		 System.out.println(linkedList); 
	} 
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200312003833133.png)
### 3. 带头单链表元素的删除【最终版本】
从$dummyHead$出发，走$index$步，走到要删除元素的位置的前一个结点处，得到$prev$，让$prev.next = delNode.next$，跳过$delNode$，然后将$delNode.next = null$，方便Java的垃圾回收。
 - `remove(int index, E e)`：从链表中删除index(0-based)位置的元素, 返回删除的元素，$O(n)$；
- `removeFirst()`：从链表中删除第一个元素, 返回删除的元素，$O(1)$；
- `removeLast()`：从链表中删除最后一个元素, 返回删除的元素，$O(n)$；
- `removeElement(E e)`：删除链表中的第一个元素e。

```java 
public class LinkedList<E> {
	
	private class Node {
		public E e;
		public Node next;
		
		public Node(E e, Node next) {
			this.e = e;
			this.next = next;
		}
		public Node(E e) {
			this(e, null);
		}
		public Node() {
			this(null, null);
		}
		
		@Override
		public String toString() {
			return e.toString();
		}
	}
	
	private Node dummyHead;
	private int size;
	
	public LinkedList() {
		dummyHead = new Node(null, null); //虚拟头结点初始化
		size = 0;
	}
	
	//获取链表中的元素长度
	public int getSize() {
		return size;
	}
	
	//返回链表是否为空
	public boolean isEmpty() {
		return size == 0;
	}
		
	//在链表的index(0-based)位置添加新的元素e
	//不是常用的操作, 仅仅作为练习
	public void add(int index, E e) {
		if (index < 0 || index > size)
			throw new IllegalArgumentException("Add failed. Illegal index.");
	
		
		Node prev = dummyHead; //从dummyhead开始遍历
		for (int i = 0; i < index; ++i)
			prev = prev.next;
		//Node node = new Node(e);
		//node.next = prev.next;
		//prev.next = node;
		prev.next = new Node(e, prev.next); 
		
		++size;
	}
	//在链表头添加元素
	public void addFirst(E e) {
		add(0, e);
	}
	
	public void addLast(E e) {
		add(size, e);
	}
	
	//获得链表的index位置(0-based)的元素
	public E get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get fail. Illegal index.");
		
		Node cur = dummyHead.next;
		for (int i = 0; i < index; ++i)
			cur = cur.next;
		return cur.e;
	}
	//获得链表的第一个元素
	public E getFirst() {
		return get(0);
	}
	//获得链表的最后一个元素
	public E getLast() {
		return get(size - 1);
	}
	
	//修改链表的index(0-based)位置的元素为e
	public void set(int index, E e) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set fail. Illegal index.");
		
		Node cur = dummyHead.next;
		for (int i = 0; i < index; ++i)
			cur = cur.next;
		cur.e = e;
	}
	
	public boolean contains(E e) {
		Node cur = dummyHead.next;
		
		while (cur != null) {
			if (cur.e.equals(e)) //发现了一个bug
				return true;
			cur = cur.next;
		}
		return false;
	}
	
	//从链表中删除index(0-based)位置的元素, 返回删除的元素
	public E remove(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set fail. Illegal index.");
		
		Node prev = dummyHead;
		for (int i = 0; i < index; ++i)
			prev = prev.next;
		
		Node retNode = prev.next;
		prev.next = retNode.next;
		retNode.next = null;
		--size;
		return retNode.e;
	}
	//从链表中删除第一个元素, 返回删除的元素
	public E removeFirst() {
		return remove(0);
	}
	//从链表中删除最后一个元素, 返回删除的元素
	public E removeLast() {
		return remove(size - 1);
	}
	//从链表中删除元素e //既要有基于位序的删除, 也要有基于元素的删除
	public void removeElement(E e) {
		Node prev = dummyHead;
		while (prev.next != null) { //如果没有e就什么都不做
			if (prev.next.e.equals(e)) 
				break;
			prev = prev.next;
		}
		if (prev.next != null) { //则prev.next.e等于e
			Node delNode = prev.next;
			prev.next = delNode.next;
			delNode.next = null;
		}
	}
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		
		Node cur = dummyHead.next;
		while (cur != null) {
			res.append(cur.e + "->");
			cur = cur.next;
		}
		res.append("NULL");
		return res.toString();
	}
	
	public static void main(String[] args) {
		 LinkedList<Integer> linkedList = new LinkedList<>();
		 for (int i = 0; i < 5; ++i) {
			 linkedList.addFirst(i);
			 System.out.println(linkedList);
		 }
		 linkedList.add(2,  666);
		 System.out.println(linkedList);
		 
		 linkedList.remove(2);
		 System.out.println(linkedList);
		 
		 linkedList.removeFirst();
		 System.out.println(linkedList);
		 
		 linkedList.removeLast();
		 System.out.println(linkedList);
	} 
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200312003811851.png)

## 四、简单链表的总结
链表的增删改查几乎都是$O(n)$的，总体来说，性能弱于数组；但是我们发现，如果只对链表头进行操作，如查、删、添加，就是$O(1)$的时间。

通过其他的一些技巧，如同时保存指向头结点和尾结点的指针，循环链表等，可以提高对链表尾的操作效率到$O(1)$。**但是，对链表中间进行操作，几乎都是线性时间。**

这也提示我们，**使用链表最好是对链表头尾进行操作**，满足这样的，就是栈和队列。因此，后面将会实现链栈和链队。

最后，链表是一个天然的递归数据结构，**可以通过它很好的学习递归操作**，下面会写一篇文章对此进行总结。
