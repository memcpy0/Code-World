import java.util.Iterator;
import java.util.NoSuchElementException;

public class Array<E> implements Iterable<E> { //参数化类型 //实现可迭代接口
    /*
    * 将迭代器置为内部类，能够直接访问ADT的数据域，迭代效率高
    * */
	@Override
	public Iterator<E> iterator() {
		return new ArrayIterator();
	} 
	private class ArrayIterator implements Iterator<E> { //实现迭代器接口
		private int idx; //记录当前位置的游标 
		public ArrayIterator() {
			idx = 0; //将游标指向0
		}
		
		@Override
		public boolean hasNext() {
			return idx < size;
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
		
		//如果减少元素数目到等于Capacity的1/4, 不能写小于或大于
		//length可能等于1, 那么size=0,length/4等于0...不可能申请一个大小为0的数组
		if (size == data.length / 4 && data.length / 2 != 0) 
			resize(data.length / 2); //收缩到原大小的一半
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
		E[] newData = (E[])new Object[newCapacity]; //必须这样写
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
		
		Iterator<Integer> it = arr.iterator();
		while (it.hasNext())
			System.out.println(it.next() + " ");
		
		for (int i : arr)
			System.out.println(i + " ");
//		arr.addLast(9999);
//		System.out.println(arr);
//		
//		System.out.println(arr.get(arr.getSize() - 1));
//		arr.set(arr.getSize() - 1, -1111);
//		System.out.println(arr);
//		
//		if (arr.contains(0)) System.out.println("This array has element zero.");

     } 
}
