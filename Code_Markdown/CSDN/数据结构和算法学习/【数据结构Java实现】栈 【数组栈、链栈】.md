@[toc]
 
## 一、栈与应用
栈也是一种线性数据结构，相比数组，涉及的操作更少，相当与栈的子集。只能从一段添加元素，另一端取出元素，$LIFO$。通过这样的限制，栈在计算机世界里，拥有了不可思议的作用。

下面的就是一个栈：
```
|    | <--栈顶
|    |
|    |
|    |
|    |
|    |
|____| <--栈底
```
应用：如`Undo`撤销，是通过栈实现的；程序调用，也是借由系统栈来运作的。

## 二、栈的基本操作和实现介绍
常见的栈的操作有：
- `void push(E)`
- `E pop()`
- `E peek()`
- `int getSize()`
- `boolean isEmpty()`

从用户角度来看，支持这些操作就好，具体底层的实现，则有多种实现方式。这里用两种实现方式：先定义一个接口`Interface Stack<E>`，再利用前面实现的$Array$来实现一个顺序栈`ArrayStack`。

## 三、动态数组实现顺序栈ArrayStack
下面的每个操作都是$O(1)$的，对于`push`和`pop`，触发了`resize`，但是经过均摊，时间复杂度仍然为$O(1)$。
```cpp 
public class ArrayStack<E> implements Stack<E> {
	Array<E> array;
	
	public ArrayStack(int capacity) {
		array = new Array<>(capacity);
	}
	
	public ArrayStack() {
		array = new Array<>();
	}
	
	@Override //表示实现接口中的方法
	public int getSize() {
		return array.getSize();
	}
	
	@Override
	public boolean isEmpty() {
		return array.isEmpty();
	}
	
	//顺序栈特有的方法, 而非接口的公共方法
	public int getCapacity() {
		return array.getCapacity();
	}
	
	@Override
	public void push(E e) {
		array.addLast(e);
	}
	
	@Override
	public E pop() {
		return array.removeLast();
	}
	
	@Override
	public E peek() {
		return array.getLast();
	}
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		res.append("Stack: ");
		res.append('[');
		for (int i = 0; i < array.getSize(); ++i) {
			res.append(array.get(i));
			if (i != array.getSize() - 1)
				res.append(", ");
		}
		res.append("] top");
		return res.toString();
	}
	
	public static void main(String[] args) {
		ArrayStack<Integer> stack = new ArrayStack<>();
		for (int i = 0; i < 5; ++i) {
			stack.push(i);
			System.out.println(stack);
		}
		stack.pop();
		System.out.println(stack);
	}
}
```
## 四、栈的应用一：括号匹配
见[LeetCode 20.Valid Parentheses](https://leetcode-cn.com/problems/valid-parentheses/)。很简单的题目，题解在LeetCode专栏里。

## 五、用单链表实现链栈
这里的链表基于[【数据结构Java实现】单链表【最基本的动态数据结构】](https://blog.csdn.net/myRealization/article/details/104794021)这篇文章。

链栈的各个操作的时间复杂度都是$O(1)$。
```java 
public class LinkedStack<E> implements Stack<E> {
	private LinkedList<E> list;
	
	public LinkedStack() {
		list = new LinkedList<>();
	}
	
	@Override
	public int getSize() {
		return list.getSize();
	}
	@Override
	public boolean isEmpty() {
		return list.isEmpty();
	}
	@Override
	public void push(E e) {
		list.addFirst(e);
	}
	@Override
	public E pop() {
		return list.removeFirst();
	}
	@Override
	public E peek() {
		return list.getFirst();
	}
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		res.append("Stack: top ");
		res.append(list);
		return res.toString();
	}
	
	public static void main(String[] args) {
		LinkedStack<Integer> stack = new LinkedStack<>();
		for (int i = 0; i < 5; ++i) {
			stack.push(i);
			System.out.println(stack);
		}
		stack.pop();
		System.out.println(stack);
	} 
} 
```
## 六、顺序栈和链栈的实际测试
用的是前面测试顺序队列和循环队列的类$Test$：
```java
import java.util.Random;

public class Test {
	private static double testStack(Stack<Integer> stack, int opCount) {
		long startTime = System.nanoTime(); //纳秒
		
		Random random = new Random();
		for (int i = 0; i < opCount; ++i)
			stack.push(random.nextInt(Integer.MAX_VALUE)); //[0, bound)的一个随机数
		for (int i = 0; i < opCount; ++i)
			stack.pop();
		
		long endTime = System.nanoTime();
		
		return (endTime - startTime) / 1000000000.0;
	} 
	
	public static void main(String[] args) {
		int opCount = 100_0000;
		ArrayStack<Integer> arrayStack = new ArrayStack<>();
		double time1 = testStack(arrayStack, opCount);
		System.out.println("ArrayStack, time: " + time1 + "s.");
		
		LinkedStack<Integer> linkedStack = new LinkedStack<>();
		double time2 = testStack(linkedStack, opCount);
		System.out.println("LinkedStack, time: " + time2 + "s.");
	}

}
```
实际上，我们可以看出，这两者的性能差距不大。链栈之所以会慢一些，主要是链栈在添加大量的元素时有许多的`new Node`操作。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200312010251517.png)
