import java.util.Iterator;
import java.util.NoSuchElementException;

public class Array<E> implements Iterable<E> { //���������� //ʵ�ֿɵ����ӿ�
    /*
    * ����������Ϊ�ڲ��࣬�ܹ�ֱ�ӷ���ADT�������򣬵���Ч�ʸ�
    * */
	@Override
	public Iterator<E> iterator() {
		return new ArrayIterator();
	} 
	private class ArrayIterator implements Iterator<E> { //ʵ�ֵ������ӿ�
		private int idx; //��¼��ǰλ�õ��α� 
		public ArrayIterator() {
			idx = 0; //���α�ָ��0
		}
		
		@Override
		public boolean hasNext() {
			return idx < size;
		}
		
		@Override 
		public E next() {
			if (!hasNext())
				throw new NoSuchElementException("No elements.");
			return data[idx++]; //�������ڵ�Ԫ��; idx+1ָ����һ��λ��
		}
		//�˵�����û��ʵ��ɾ������, ֻ���׳�UnsupportedOperationException
		@Override
		public void remove() {
			throw new UnsupportedOperationException("This iterator doesn't implement the remove operation!");
		}
	}
	
	private E[] data;
	private int size;     
	//private int capacity; //��data.length�������
	
	//�޲������캯��, Ĭ�����������capacity=10
	public Array() {
		this(10); //���캯������
	}
	
	//���캯��, �������������capacity����Array
	@SuppressWarnings("unchecked")
	public Array(int capacity) {
		data = (E[]) new Object[capacity]; //����ֱ�Ӵ���һ����������
		size = 0;
	}
	
	//��ȡ�����е�Ԫ�ظ���
	public int getSize() {
		return size;
	}
	//��ȡ���������
	public int getCapacity() {
		return data.length;
	}
	//�ж������Ƿ�Ϊ��
	public boolean isEmpty() {
		return size == 0;
	}
	
	//��indexλ�����һ��Ԫ��
	public void add(int index, E e) {
		if (index < 0 || index > size) 
			throw new IllegalArgumentException("Add failed. Require index >= 0 and <= size.");
		
		if (size == data.length) //������ǰ�����׳��쳣
			resize(2 * data.length);
		
		for (int i = size - 1; i >= index; --i)
			data[i + 1] = data[i];
		data[index] = e;
		++size;
	}
	//�����鿪ͷ���Ԫ��
	public void addFirst(E e) {
		add(0, e); //����
	}
	//������ĩβ���Ԫ��
	public void addLast(E e) { 
		add(size, e);
	}
	
	//���indexλ�õ�Ԫ��
	public E get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get failed. Index is illegal.");
		
		return data[index];
	}
	//�õ���һ��Ԫ��
	public E getFirst() {
		return get(0);
	}
	//�õ����һ��Ԫ��
	public E getLast() {
		return get(size - 1);
	}
	//����indexλ�õ�Ԫ��Ϊe
	public void set(int index, E e) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Set failed. Index is illegal.");
		
		data[index] = e;
	}
	
	//���������Ƿ���Ԫ��e
	public boolean contains(E e) {
		for (int i = 0; i < size; ++i) {
			if (data[i].equals(e)) //���������֮�����ֵ�Ƚ�, Ӧ����equals
				return true;
		}
		return false;
	}
	//����������Ԫ��e������, ������e�򷵻�-1
	public int find(E e) {
		for (int i = 0; i < size; ++i) {
			if (data[i].equals(e))
				return i;
		}
		return -1;
	}
	
	//ɾ����Ӧ����λ�õ�Ԫ��, ͬʱ�������ɾ����Ԫ��
	public E remove(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Remove failed. Index is illegal.");
		/*ʹ�÷�������, data�����д洢���Ƕ��������, �����ɾ���߼��б仯ǰ��size
		ָ���������Ȼ����, �����GC����, ����������Ҫע�� */
		E ret = data[index];
		for (int i = index + 1; i < size; ++i)
			data[i - 1] = data[i];
		--size;
		data[size] = null; //loitering objects //��Ȼ, ��д��仰, �߼�Ҳ����
		
		//�������Ԫ����Ŀ������Capacity��1/4, ����дС�ڻ����
		//length���ܵ���1, ��ôsize=0,length/4����0...����������һ����СΪ0������
		if (size == data.length / 4 && data.length / 2 != 0) 
			resize(data.length / 2); //������ԭ��С��һ��
		return ret;
	}
	//remove�Ŀ�ݷ���
	public E removeFirst() {
		return remove(0);
	}
	public E removeLast() {
		return remove(size - 1);
	}
	//ɾ�������еĵ�һ��Ԫ��e
	public void removeElement(E e) {
		int index = find(e);
		if (index != -1)
			remove(index);
	}
	
	//��̬���������
	private void resize(int newCapacity) {
		E[] newData = (E[])new Object[newCapacity]; //��������д
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
		Array<Integer> arr = new Array<>(); //Ĭ�ϵ�����10��
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
