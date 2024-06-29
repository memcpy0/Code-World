
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

