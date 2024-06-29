
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
		dummyHead = new Node(null, null); //����ͷ����ʼ��
		size = 0;
	}
	
	//��ȡ�����е�Ԫ�س���
	public int getSize() {
		return size;
	}
	
	//���������Ƿ�Ϊ��
	public boolean isEmpty() {
		return size == 0;
	}
		
	//�������index(0-based)λ������µ�Ԫ��e
	//���ǳ��õĲ���, ������Ϊ��ϰ
	public void add(int index, E e) {
		if (index < 0 || index > size)
			throw new IllegalArgumentException("Add failed. Illegal index.");
	
		
		Node prev = dummyHead; //��dummyhead��ʼ����
		for (int i = 0; i < index; ++i)
			prev = prev.next;
		//Node node = new Node(e);
		//node.next = prev.next;
		//prev.next = node;
		prev.next = new Node(e, prev.next); 
		
		++size;
	}
	//������ͷ���Ԫ��
	public void addFirst(E e) {
		add(0, e);
	}
	
	public void addLast(E e) {
		add(size, e);
	}
	
	//��������indexλ��(0-based)��Ԫ��
	public E get(int index) {
		if (index < 0 || index >= size)
			throw new IllegalArgumentException("Get fail. Illegal index.");
		
		Node cur = dummyHead.next;
		for (int i = 0; i < index; ++i)
			cur = cur.next;
		return cur.e;
	}
	//�������ĵ�һ��Ԫ��
	public E getFirst() {
		return get(0);
	}
	//�����������һ��Ԫ��
	public E getLast() {
		return get(size - 1);
	}
	
	//�޸������index(0-based)λ�õ�Ԫ��Ϊe
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
			if (cur.e.equals(e)) //������һ��bug
				return true;
			cur = cur.next;
		}
		return false;
	}
	
	//��������ɾ��index(0-based)λ�õ�Ԫ��, ����ɾ����Ԫ��
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
	//��������ɾ����һ��Ԫ��, ����ɾ����Ԫ��
	public E removeFirst() {
		return remove(0);
	}
	//��������ɾ�����һ��Ԫ��, ����ɾ����Ԫ��
	public E removeLast() {
		return remove(size - 1);
	}
	
	//��������ɾ��Ԫ��e //��Ҫ�л���λ���ɾ��, ҲҪ�л���Ԫ�ص�ɾ��
	public void removeElement(E e) {
		Node prev = dummyHead;
		while (prev.next != null) { //���û��e��ʲô������
			if (prev.next.e.equals(e)) 
				break;
			prev = prev.next;
		}
		if (prev.next != null) { //��prev.next.e����e
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
