@[toc]

# 映射
## 1. 映射介绍
C++的 $map$，Python的字典 $dict$，Java的 $TreeMap, HashMap$，都是映射。更抽象的说，函数/方法也是映射。对于映射这种数据结构，其实就是存储 $key:value$ 数据对的结构，根据键 $key$，寻找值 $value$，就像一本字典，能够根据单词查找含义。

介绍集合的时候，我们说集合可以用来统计单词量；而映射，则可以用来统计词频。

我们在这里，可以很简单的用链表或二分搜索树实现映射，将映射作为一个接口 `Map<K, V>`，定义如下：
```java 
public interface Map<K, V> {
	void add(K key, V value);
	V remove(K key);             //用键作为索引, 进行删除和查找
	void set(K key, V newValue);
	V get(K key);
	int getSize();
	boolean isEmpty();
	boolean contains(K key);
} 
```

## 2. 基于链表实现的映射
这是链表的结点的定义：
```java
class Node {
	K key;
	V value;
	Node next;
}
```
下面是代码，由于原来的链表类只有一个参数化的类型，因此我们必须自己写一个，而不能直接重用。虽然麻烦，但是没有多难，**主要是一些设计上的习惯要知道**：

```java
public class LinkedListMap<K, V> implements Map<K, V> {
	//由于有两个参数化的类型, 不能直接重用前面的链表类
	private class Node { 
		public K key;
		public V value;
		public Node next;
		
		public Node(K key, V value, Node next) {
			this.key = key;
			this.value = value;
			this.next = next;
		}
		public Node() { this(null, null, null); }
		public Node(K key) { this(key, null, null); }
		
		@Override
		public String toString() { return key.toString() + " : " + value.toString(); }
	}
	private Node dummyHead;
	private int size;
	public LinkedListMap() {
		dummyHead = new Node();
		size = 0;
	}
	
	@Override
	public int getSize() { return size; }
	@Override
	public boolean isEmpty() { return size == 0; }
	
	//查找key对应的Node
	private Node getNode(K key) {
		Node cur = dummyHead.next;
		while (cur != null) {
			if (cur.key.equals(key))
				return cur;
			cur = cur.next;
		}
		return null;
	}
	
	@Override
	public boolean contains(K key) { //是否含有以key为键的键值对
		return getNode(key) != null;
	}
	
	@Override
	public V get(K key) { //查找键对应的值
		Node node = getNode(key);
		return node == null ? null : node.value; //key存在则返回值; 不存在则返回NULL
	}
	
	@Override
	public void add(K key, V value) { //添加一个键值对, 不存在则添加; 存在key则覆盖value
		Node node = getNode(key);
		if (node == null) { //没有这个key
			dummyHead.next = new Node(key, value, dummyHead.next);
			++size;
		} else node.value = value; //不抛出异常; 设计惯例, 更新原来的值
	}
	
	@Override
	public void set(K key, V value) {
		Node node = getNode(key);
		if (node == null) //用户指定要更新一个键的值, 因此键必须存在, 不存在则抛出异常, 以和add方法区分开来
			throw new IllegalArgumentException(key + " doesn't exist!");
		else node.value = value; //存在则更新
	}
	
	@Override
	public V remove(K key) { //删除key对应的键值对, 存在则删除返回null; 不存在则直接返回null
		Node prev = dummyHead;
		while (prev.next != null) {
			if (prev.next.key.equals(key))
				break;
			prev = prev.next;
		}
		if (prev.next != null) { //存在key则删除该结点
			Node delNode = prev.next;
			prev.next = delNode.next;
			delNode.next = null;
			return delNode.value;
		}
		return null;
	}
	
	public static void main(String[] args) {
		System.out.println("Pride and Prejudice");
		
		Array<String> words = new Array<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words)) {
			System.out.println("Total words: " + words.getSize());
			
			LinkedListMap<String, Integer> map = new LinkedListMap<>();
			for (String word : words) { //对书中的每个单词
				if (map.contains(word)) //集合中存在该单词
					map.set(word, map.get(word) + 1); //将该单词对应的键值对的值+1
				else
					map.add(word, 1); //不存在则设置该单词出现的频数为1
			}
			System.out.println("Total different words: " + map.getSize()); //不同的单词数
			System.out.println("Frequency of 'Pride': " + map.get("pride")); //傲慢出现的次数
			System.out.println("Frequency of 'Prejudice: " + map.get("prejudice")); //偏见出现的次数
		}
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326171316397.png)
## 3. 基于二分搜索树的映射
和集合一样，我们也可以用二分搜索树实现一个映射类。不过这时 $key$ 有一个限制，即 $key$ 必须是可比较的——即实现了 $Comparable$ 接口。

树的结点如下：
```java
class Node {
	K key;
	V value;
	Node left;
	Node right;
}
```
由于原来的二叉搜索树的Node只能够存储一个值，现在需要存储 $key$ 和 $value$ 了，因此自己写吧。

```java
public class BSTMap<K extends Comparable<K>, V> implements Map<K, V> {

	private class Node {
		public K key;
		public V value;
		public Node left, right;
		
		public Node(K key, V value) {
			this.key = key;
			this.value = value;
		    left = null;
		    right = null;
		}
	}
	
	private Node root;
	private int size;
	public BSTMap() {
		root = null;
		size = 0;
	}
	
	//返回键值对的数量
	@Override
	public int getSize() { return size; } 
	//是否为空
	@Override
	public boolean isEmpty() { return size == 0; }
	//向二分搜索树中添加新的元素
	@Override
	public void add(K key, V value) {
		root = add(root, key, value);
	}
	//向以root为根的二叉搜索树中插入元素(key, value), 递归算法
	//返回插入新结点的二分搜索树的根
	private Node add(Node root, K key, V value) {
		if (root == null) {
			++size;
			return new Node(key, value);
		} 
		if (key.compareTo(root.key) < 0)
			root.left = add(root.left, key, value);
		else if (key.compareTo(root.key) > 0)
			root.right = add(root.right, key, value);
		else root.value = value; //我们认为存在key的时候传入value是要更新value
		
		return root;
	}
	
	//返回以root为根的二分搜索树中, key所在的结点
	private Node getNode(Node root, K key) { 
		if (root == null)
			return null;
		if (key.compareTo(root.key) == 0) //key.equals(root.key)
			return root;
		else if (key.compareTo(root.key) < 0)
			return getNode(root.left, key);
		else return getNode(root.right, key);
	}
	
	@Override
	public boolean contains(K key) {
		return getNode(root, key) != null;
	}
	
	@Override
	public V get(K key) {
		Node node = getNode(root, key);
		return node == null ? null : node.value;
	}
	
	@Override
	public void set(K key, V newValue) {
		Node node = getNode(root, key);
		if (node == null) //和前面一样, 不存在时抛出异常
			throw new IllegalArgumentException(key + "doesn't exist!");
		node.value = newValue;	
	}
	
	@Override
	public V remove(K key) {
		Node node = getNode(root, key);
		if (node != null) { //如果使用原来的删除函数, 我们不知道key是否存在, 因此要先搜索一次
			root = remove(root, key);
			return node.value;
		}
		return null;
	}
	//删除掉以root为根的二分搜索树中键为key的结点, 递归算法
	//返回删除结点后新的二分搜索树的根
	private Node remove(Node root, K key) {
		if (root == null)
			return null;
		
		if (key.compareTo(root.key) < 0) {
			root.left = remove(root.left, key);
			return root;
		}
		else if (key.compareTo(root.key) > 0) {
			root.right = remove(root.right, key);
			return root;
		}
		else { //如果待删除结点的左子树为空的情况
			if (root.left == null) {
				Node rightNode = root.right;
				root.right = null; //方便垃圾回收
				--size;
				return rightNode;
			} // 待删除结点的右子树为空
			if (root.right == null) {
				Node leftNode = root.left;
				root.left = null; //方便垃圾回收
				--size;
				return leftNode;
			}
			//待删除结点左右子树均不为空的情况
			//找到比待删除结点大的最小结点, 即待删除结点右子树的最小结点
			//用这个结点替代待删除结点的位置
			Node successor = minimum(root.right);
			successor.right = removeMin(root.right);
			successor.left = root.left;
			root.left = root.right = null; //方便垃圾回收
			return successor;
		}
	}
	//返回以Node为根的二分搜索树的最小值所在的结点
	private Node minimum(Node node) {
		if (node.left == null) return node;
		return minimum(node.left);
	}
	//删除以node为根的二分搜索树的最小结点
	//返回删除结点后新的二分搜索子树的根
	private Node removeMin(Node node) {
		if (node.left == null) {
			Node rightNode = node.right;
			node.right = null; 
			--size;
			return rightNode;
		}
		//删除左子树中的最小值并得到新的左子树
		node.left = removeMin(node.left); 
		return node;
	}
	
	public static void main(String[] args) {
		System.out.println("Pride and Prejudice");
		
		Array<String> words = new Array<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words)) {
			System.out.println("Total words: " + words.getSize());
			
			BSTMap<String, Integer> map = new BSTMap<>();
			for (String word : words) { //对书中的每个单词
				if (map.contains(word)) //集合中存在该单词
					map.set(word, map.get(word) + 1); //将该单词对应的键值对的值+1
				else
					map.add(word, 1); //不存在则设置该单词出现的频数为1
			}
			System.out.println("Total different words: " + map.getSize()); //不同的单词数
			System.out.println("Frequency of 'Pride': " + map.get("pride")); //傲慢出现的次数
			System.out.println("Frequency of 'Prejudice: " + map.get("prejudice")); //偏见出现的次数
		}
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020032621205518.png)
## 4. 性能比较和复杂度分析
性能比较很简单，和集合的测试几乎一样，写在 $Test$ 类中：
```java
import java.util.Random;

public class Test { 
	private static double testMap(Map<String, Integer> map, String filename) { //词频统计
		long startTime = System.nanoTime();
		
		System.out.println(filename);
		Array<String> words = new Array<>();
		if (FileOperation.readFile(filename, words))
			System.out.println("Total words: " + words.getSize());
		for (String word : words) {
			if (map.contains(word))
				map.set(word, map.get(word) + 1);
			else 
				map.add(word, 1);
		}
		System.out.println("Total different words: " + map.getSize()); //不同的单词数
		System.out.println("Frequency of 'Pride': " + map.get("pride")); //傲慢出现的次数
		System.out.println("Frequency of 'Prejudice: " + map.get("prejudice")); //偏见出现的次数
		
		long endTime = System.nanoTime();
		return (endTime - startTime) / 1000000000.0;
	}
	
	public static void main(String[] args) { 
		String filename = "pride-and-prejudice.txt";
		System.out.println("BSTMap Test:");
		BSTMap<String, Integer> bstMap = new BSTMap<>();
		double time1 = testMap(bstMap, filename);
		System.out.println(time1 + "s.\n");
		
		System.out.println("LinkedListMap Test:");
		LinkedListMap<String, Integer> listMap = new LinkedListMap<>();
		double time2 = testMap(listMap, filename);
		System.out.println(time2 + "s."); 
	} 
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326212018350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
和 `LinkedListSet` 与 `BSTSet` 的比较一样，`BSTMap` 的速度远快于 `LinkedListMap`。
|  |  LinkedListMap | BSTMap |BSTMap平均 | BSTMap最差
|:--:|:--:|:--:|:--:|:--:
| `add`  | $O(n)$ | $O(h)$ | $O(logn)$ | $O(n)$ |
| `remove`| $O(n)$ | $O(h)$ | $O(logn)$ | $O(n)$ |
| `set` | $O(n)$ | $O(h)$ | $O(logn)$ | $O(n)$ |
| `get` |$O(n)$ | $O(h)$ | $O(logn)$ | $O(n)$ |
| `contains`| $O(n)$ | $O(h)$ | $O(logn)$ | $O(n)$ |

---
## 5. 有序映射和无序映射
同集合一样，我们用二叉搜索树实现的Map，就是一个有序映射；链表实现的Map，其实是一个无序映射。不过 `LinkedListMap` 太慢了，通常，我们会**用哈希实现更快的无序映射**。

Java中的 `TreeMap` 和 `HashMap` 就是有序和无序映射的典型代表；C++中的则是 `map` 和 `unordered_map`。
## 6. 多重映射
多重映射中的键可以重复，比如：C++的`multimap`。有时候可能会需要用到。
## 7. 集合和映射的关系
在实现的时候，我们发现集合与映射有很多相同的地方。实际上，映射就是把 `<key, value>` 作为元素的集合；而集合，就是把 `<key, null>` 作为元素的映射。因此，**有了集合，我们可以实现映射；有了映射，我们也可以反过来实现集合**。

比如说，**基于集合，我们可以重新定义其中的元素为一种键值对，并且要强调， 这种键值对在比较的时候，(如果集合基于二叉搜索树)是以key的值进行比较的，而不管value的值**。但是，用集合实现映射，还需要添加新的操作。

所以，**更常见的做法是基于映射实现集合**。怎样做到的呢？很简单，我们就把映射所有的 $value$ 作为空，而且只考虑键，这时整个Map就是一个集合，当然，这时 `get` 和 `set` 方法就没有意义了。我们实现一个映射类，再对映射进行包装，就可以实现集合。

甚至于，我们在实现二叉搜索树的时候，就可以直接以 `<key, value>` 作为结点值，这样会更加简单，**就省去了很多的Map和Set的重复的底层实现**。以后实现AVL树，红黑树的时候就是这种做法。


