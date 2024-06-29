
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
