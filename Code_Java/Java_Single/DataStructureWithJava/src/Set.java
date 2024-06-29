
public interface Set<E> {
	void add(E e);
	void remove(E e); //用户知道删除的是什么元素, 不需要返回值
	boolean contains(E e);
	int getSize();
	boolean isEmpty();
}
