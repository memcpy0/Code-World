
public class LinkedListMap<K, V> implements Map<K, V> {
	//由于有两个参数化的类型, 不能直接重用前面的链表类
	private class Node { 
		public K key;
		public V value;
		public Node next;
		
		public Node(K key, V value, Node next) {
			this.key = key;
			this.value = value;
			this.next = next;
		}
		public Node() { this(null, null, null); }
		public Node(K key) { this(key, null, null); }
		
		@Override
		public String toString() { return key.toString() + " : " + value.toString(); }
	}
	private Node dummyHead;
	private int size;
	public LinkedListMap() {
		dummyHead = new Node();
		size = 0;
	}
	
	@Override
	public int getSize() { return size; }
	@Override
	public boolean isEmpty() { return size == 0; }
	
	//查找key对应的Node
	private Node getNode(K key) {
		Node cur = dummyHead.next;
		while (cur != null) {
			if (cur.key.equals(key))
				return cur;
			cur = cur.next;
		}
		return null;
	}
	
	@Override
	public boolean contains(K key) { //是否含有以key为键的键值对
		return getNode(key) != null;
	}
	
	@Override
	public V get(K key) { //查找键对应的值
		Node node = getNode(key);
		return node == null ? null : node.value; //key存在则返回值; 不存在则返回NULL
	}
	
	@Override
	public void add(K key, V value) { //添加一个键值对, 不存在则添加; 存在key则覆盖value
		Node node = getNode(key);
		if (node == null) { //没有这个key
			dummyHead.next = new Node(key, value, dummyHead.next);
			++size;
		} else node.value = value; //不抛出异常; 设计惯例, 更新原来的值
	}
	
	@Override
	public void set(K key, V value) {
		Node node = getNode(key);
		if (node == null) //用户指定要更新一个键的值, 因此键必须存在, 不存在则抛出异常, 以和add方法区分开来
			throw new IllegalArgumentException(key + " doesn't exist!");
		else node.value = value; //存在则更新
	}
	
	@Override
	public V remove(K key) { //删除key对应的键值对, 存在则删除返回null; 不存在则直接返回null
		Node prev = dummyHead;
		while (prev.next != null) {
			if (prev.next.key.equals(key))
				break;
			prev = prev.next;
		}
		if (prev.next != null) { //存在key则删除该结点
			Node delNode = prev.next;
			prev.next = delNode.next;
			delNode.next = null;
			return delNode.value;
		}
		return null;
	}
	
	public static void main(String[] args) {
		System.out.println("Pride and Prejudice");
		
		Array<String> words = new Array<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words)) {
			System.out.println("Total words: " + words.getSize());
			
			LinkedListMap<String, Integer> map = new LinkedListMap<>();
			for (String word : words) { //对书中的每个单词
				if (map.contains(word)) //集合中存在该单词
					map.set(word, map.get(word) + 1); //将该单词对应的键值对的值+1
				else
					map.add(word, 1); //不存在则设置该单词出现的频数为1
			}
			System.out.println("Total different words: " + map.getSize()); //不同的单词数
			System.out.println("Frequency of 'Pride': " + map.get("pride")); //傲慢出现的次数
			System.out.println("Frequency of 'Prejudice: " + map.get("prejudice")); //偏见出现的次数
		}
	}
}
