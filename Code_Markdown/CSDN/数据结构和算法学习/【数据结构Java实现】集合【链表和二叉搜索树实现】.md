@[toc]

集合和映射——是高层的数据结构，是一些接口，提供相应的API，用于不同的应用。底层的实现，则多种多样。

这篇文章中，基于二叉搜索树和链表，分别实现集合类。
# 集合
## 1. 集合介绍
这里实现的集合和数学里的集合一样，在实际应用时能够快速帮助我们去重。回忆上次实现的二分搜索树，**它不支持重复的元素，因此是非常好的实现集合的底层数据结构。**

`Set<E>`
- `void add(E)`：集合最重要的性质，不能添加重复的元素，将在这个方法中实现
- `void remove(E)`
- `boolean contains(E)`
- `int getSize()`
- `boolean isEmpty()`

集合的典型应用，包括——客户统计；网站访问IP统计；词汇量统计...
 
```java 
public interface Set<E> {
	void add(E e);
	void remove(E e); //用户知道删除的是什么元素, 不需要返回值
	boolean contains(E e);
	int getSize();
	boolean isEmpty();
} 
```
## 2. 基于二叉搜索树的集合实现
其实有了(**不含重复元素的**)二分搜索树的底层实现，完成集合这个接口是很简单的，我们**只需要包装一下二分搜索树，实现上面的集合接口即可。**

```java
public class BSTSet<E extends Comparable<E>> implements Set<E> { //要求元素具有可比性
	private BST<E> bst;
	//实际上我们实现的BST完全支持集合的所有操作
	public BSTSet() {
		bst = new BST<>();
	} 
	@Override
	public int getSize() {
		return bst.size();
	} 
	@Override
	public boolean isEmpty() {
		return bst.isEmpty();
	} 
	@Override
	public void add(E e) {
		bst.add(e);
	} 
	@Override
	public boolean contains(E e) {
		return bst.contains(e);
	} 
	@Override
	public void remove(E e) {
		bst.remove(e);
	}
}
```
写完了，要测试一下。测试方法是：用`双城记`和`傲慢与偏见`两本英语单词书，将它们的词汇放进集合中，看看有多少不同的词汇。
```java
//main函数写在BSTSet中
public static void main(String[] args) {
	System.out.println("A Tale of Two Cities");
	Array<String> words = new Array<>();
	BSTSet<String> set = new BSTSet<>();
	if (FileOperation.readFile("a-tale-of-two-cities.txt", words))
		System.out.println("Total words: " + words.getSize());
	for (String word : words) {
		set.add(word);
	}
	System.out.println("Total different words: " + set.getSize());
	
	System.out.println();
	
	System.out.println("Pride and Prejudice");
	Array<String> words2 = new Array<>();
	BSTSet<String> set2 = new BSTSet<>();
	if (FileOperation.readFile("pride-and-prejudice.txt", words2))
		System.out.println("Total words: " + words.getSize());
	for (String word : words2) {
		set2.add(word);
	}
	System.out.println("Total different words: " + set2.getSize());
}
```
注意，我们这里使用的是 $Array$，是以前我们自己写的，为了能够在 $Array$ 上使用`foreach`循环，我们需要 $Array$ 实现 $Iterable$ 接口，并且在 $Array$ 中实现一个 $Iterator$。相应的代码**我已经更新到了原来的那篇文章中**。同时， $FileOperation$ 是一个自定义类，实现了读入文件，分割词汇的功能。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326012841363.png)
## 3. 基于链表的集合实现
为什么还要用链表呢？因为链表和二叉搜索树一样，都属于动态数据结构。**基于这两种动态数据结构的底层来实现集合，可以比较它们的性能，从而看到二分搜索树的优势所在。**

当然，这个也很简单，简要包装一下我们以前实现的LinkedList类即可。
```java 
public class LinkedListSet<E> implements Set<E> {
	private LinkedList<E> list;
	public LinkedListSet() {
		list = new LinkedList<>();
	}
	public int getSize() {
		return list.getSize();
	}
	public boolean isEmpty() {
		return list.isEmpty();
	}
	public boolean contains(E e) {
		return list.contains(e);
	}
	public void add(E e) { 
		//以前实现的LinkedList中没有规定不能包含重复元素
		//因此稍微麻烦一点
		if (!list.contains(e))
			list.addFirst(e); //以前实现的是单链表, 没有尾指针
	}
	public void remove(E e) {
		list.removeElement(e);
	}
}
```
顺带一提，写到这里的时候，我发现了以前的单链表实现的一个小 $bug$，会更新到相应的文章中。

同时，测试如下：
```java
public static void main(String[] args) {
	System.out.println("A Tale of Two Cities");
	Array<String> words = new Array<>();
	LinkedListSet<String> set = new LinkedListSet<>();
	if (FileOperation.readFile("a-tale-of-two-cities.txt", words))
		System.out.println("Total words: " + words.getSize());
	for (String word : words) {
		set.add(word);
	}
	System.out.println("Total different words: " + set.getSize());
	
	System.out.println();
	
	System.out.println("Pride and Prejudice");
	Array<String> words2 = new Array<>();
	LinkedListSet<String> set2 = new LinkedListSet<>();
	if (FileOperation.readFile("pride-and-prejudice.txt", words2))
		System.out.println("Total words: " + words.getSize());
	for (String word : words2) {
		set2.add(word);
	}
	System.out.println("Total different words: " + set2.getSize());
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326013437544.png)
## 4. 性能比较
在之前使用这两个集合的时候，我们很容易就会发现，`BSTSet`比`LinkedListSet`快许多。但究竟快多少呢？我们可以进行测试，和以前一样，在`Test`类中进行测试。
```java 
public class Test {  
	private static double testSet(Set<String> set, String filename) {
		long startTime = System.nanoTime();
		
		System.out.println(filename);
		Array<String> words = new Array<>();  
		if (FileOperation.readFile(filename, words))
			System.out.println("Total words: " + words.getSize()); //单词数
		for (String word : words) {
			set.add(word);
		}
		System.out.println("Total different words: " + set.getSize()); //不同的单词数
		
		long endTime = System.nanoTime();
		return (endTime - startTime) / 1000000000.0;
	}
	
	public static void main(String[] args) { 
		String filename = "pride-and-prejudice.txt";
		System.out.println("BSTSet Test:");
		BSTSet<String> bstSet = new BSTSet<>();
		double time1 = testSet(bstSet, filename);
		System.out.println(time1 + "s.");
		
		System.out.println("LinkedListSet Test:");
		LinkedListSet<String> listSet = new LinkedListSet<>();
		double time2 = testSet(listSet, filename);
		System.out.println(time2 + "s."); 
	} 
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200326143944730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 5. 复杂度分析
我们实现的集合就只有五种操作，主要的就只有下面的三种操作，逐一分析：
|  |`add` | `contains` | `remove` | 
|:--|:--|:--|:--
|LinkedListSet | $O(n)$，需要判断是否存在 | $O(n)$ | $O(n)$|
| BSTSet | $O(h)$ | $O(h)$ | $O(h)$ |

我们二叉搜索树上添加、删除、查找元素，从树的根节点向叶子节点除法，就像在走一个链表，最多只会有 $O(h)$ 的复杂度，$h$ 是树的高度，树有多高，那么操作的复杂度就是多少。但是我们不知道 $h$ 是多少，如何比较呢？

简单，假设二叉树是满二叉树：
 
- 第 $0$ 层：$1$
- 第 $1$ 层：$2$
- 第 $2$ 层：$4$
- 第 $3$ 层：$8$
- ...
- 第 $h-1$ 层：$2^{(h-1)}$

总共有多少节点呢？$\sum_{i=0}^{h-1} 2^i = 2^h - 1 = n$，所以有 $h = log_2(n+1)$，则 $O(h) = O(log_2n)$。
|  |`add` | `contains` | `remove` | 
|:--|:--|:--|:--
|LinkedListSet | $O(n)$，需要判断是否存在 | $O(n)$ | $O(n)$|
| BSTSet | $O(log_2n)$ | $O(log_2n)$ | $O(log_2n)$ |

事实上，$O(log_2n)$ **是一个非常非常快的速度**，比如说 $n = 2^{32}$时，$log_2n = 32$，两者的差距太大了，而且随着 $n$ 的逐渐增大，两者的差距会越来越大。

当然，这里准确来讲，应该说 $BSTSet$ 的平均操作复杂度是 $O(logn)$。原因在于，二叉搜索树很容易退化成一条单链表，这时最坏就是 $O(n)$ 的复杂度。而且，这种最坏的情况很容易复现，比如用一个有序的序列插入，最后得到的就是一个单链表。

因此，$BSTSet$ 的操作复杂度为 $O(h)$ 是最确切的，即使有可能出现最坏情况，但是大多数时候，都会大大快于 $LinkedListSet$。
## 6. 有序集合与无序集合
C++有 $set, unordered\_set, multiset$，Java也有$TreeSet, HashSet$。

我们上面实现的 $BSTSet$ 比较接近 $TreeSet$，因为是用二叉搜索树作为底层结构，具有有序性。为了维持这种有序性，$BSTSet$ 的插入、删除、查找都需要 $O(logn)$ 的复杂度。

但是有时候，我们只需要集合的去重功能，不需要有序性，因此**可以用哈希来实现集合，提高增删改查这几种操作的效率**。这个也会在后面实现。




