
public class LinkedListMap<K, V> implements Map<K, V> {
	//����������������������, ����ֱ������ǰ���������
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
	
	//����key��Ӧ��Node
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
	public boolean contains(K key) { //�Ƿ�����keyΪ���ļ�ֵ��
		return getNode(key) != null;
	}
	
	@Override
	public V get(K key) { //���Ҽ���Ӧ��ֵ
		Node node = getNode(key);
		return node == null ? null : node.value; //key�����򷵻�ֵ; �������򷵻�NULL
	}
	
	@Override
	public void add(K key, V value) { //���һ����ֵ��, �����������; ����key�򸲸�value
		Node node = getNode(key);
		if (node == null) { //û�����key
			dummyHead.next = new Node(key, value, dummyHead.next);
			++size;
		} else node.value = value; //���׳��쳣; ��ƹ���, ����ԭ����ֵ
	}
	
	@Override
	public void set(K key, V value) {
		Node node = getNode(key);
		if (node == null) //�û�ָ��Ҫ����һ������ֵ, ��˼��������, ���������׳��쳣, �Ժ�add�������ֿ���
			throw new IllegalArgumentException(key + " doesn't exist!");
		else node.value = value; //���������
	}
	
	@Override
	public V remove(K key) { //ɾ��key��Ӧ�ļ�ֵ��, ������ɾ������null; ��������ֱ�ӷ���null
		Node prev = dummyHead;
		while (prev.next != null) {
			if (prev.next.key.equals(key))
				break;
			prev = prev.next;
		}
		if (prev.next != null) { //����key��ɾ���ý��
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
			for (String word : words) { //�����е�ÿ������
				if (map.contains(word)) //�����д��ڸõ���
					map.set(word, map.get(word) + 1); //���õ��ʶ�Ӧ�ļ�ֵ�Ե�ֵ+1
				else
					map.add(word, 1); //�����������øõ��ʳ��ֵ�Ƶ��Ϊ1
			}
			System.out.println("Total different words: " + map.getSize()); //��ͬ�ĵ�����
			System.out.println("Frequency of 'Pride': " + map.get("pride")); //�������ֵĴ���
			System.out.println("Frequency of 'Prejudice: " + map.get("prejudice")); //ƫ�����ֵĴ���
		}
	}
}
