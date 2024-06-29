 public class LinkedQueue<E> implements Queue<E> {
	
	private class Node { //�����㲻��
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
	
	private Node head, tail; //��������
	private int size;
	
	public LinkedQueue() {
		head = null;
		tail = null;
		size = 0;
	}
	
	@Override //��ȡ���е�Ԫ����Ŀ
	public int getSize() {
		return size;
	} 
	
	@Override //���ض����Ƿ�Ϊ��
	public boolean isEmpty() {
		return size == 0;
	}
	@Override	
	public void enqueue(E e) {
		if (tail == null) { //�������Ԫ��, ����
			tail = new Node(e);
			head = tail;
		} else {
			tail.next = new Node(e);
			tail = tail.next;
		}
		++size;
	}
	@Override
	public E dequeue() {
		if (isEmpty())
			throw new IllegalArgumentException("Cannot dequeue from an empty queue.");
		
		Node retNode = head;
		head = head.next;
		if (head == null) //������Ӻ�Ϊ������ 
			tail = null; 
		retNode.next = null; //�������жϿ�ԭ����ͷ���
		--size;
		return retNode.e;
	}
	@Override
	public E getFront() {
		if (isEmpty())
			throw new IllegalArgumentException("Cannot dequeue from an empty queue.");
		
		return head.e;
	} 
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		res.append("Queue: front ");
		
		Node cur = head;
		while (cur != null) {
			res.append(cur.e + "->");
			cur = cur.next;
		}
		res.append("NULL tail");
		return res.toString();
	}
	
	public static void main(String[] args) {
		 LinkedQueue<Integer> queue = new LinkedQueue<>();
		 for (int i = 0; i < 10; ++i) {
			 queue.enqueue(i); 
			 System.out.println(queue);
		 } 
		 while (queue.getSize() > 2) queue.dequeue();
		 System.out.println(queue);
	}

}
