 public class LinkedQueue<E> implements Queue<E> {
	
	private class Node { //链表结点不变
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
	
	private Node head, tail; //都是引用
	private int size;
	
	public LinkedQueue() {
		head = null;
		tail = null;
		size = 0;
	}
	
	@Override //获取队列的元素数目
	public int getSize() {
		return size;
	} 
	
	@Override //返回队列是否为空
	public boolean isEmpty() {
		return size == 0;
	}
	@Override	
	public void enqueue(E e) {
		if (tail == null) { //空链表进元素, 特判
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
		if (head == null) //如果出队后为空链表 
			tail = null; 
		retNode.next = null; //从链表中断开原来的头结点
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
