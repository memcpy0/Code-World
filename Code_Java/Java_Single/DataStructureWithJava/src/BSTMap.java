public class BSTMap<K extends Comparable<K>, V> implements Map<K, V> {

	private class Node {
		public K key;
		public V value;
		public Node left, right;
		
		public Node(K key, V value) {
			this.key = key;
			this.value = value;
		    left = null;
		    right = null;
		}
	}
	
	private Node root;
	private int size;
	public BSTMap() {
		root = null;
		size = 0;
	}
	
	//���ؼ�ֵ�Ե�����
	@Override
	public int getSize() { return size; } 
	//�Ƿ�Ϊ��
	@Override
	public boolean isEmpty() { return size == 0; }
	//�����������������µ�Ԫ��
	@Override
	public void add(K key, V value) {
		root = add(root, key, value);
	}
	//����rootΪ���Ķ����������в���Ԫ��(key, value), �ݹ��㷨
	//���ز����½��Ķ����������ĸ�
	private Node add(Node root, K key, V value) {
		if (root == null) {
			++size;
			return new Node(key, value);
		} 
		if (key.compareTo(root.key) < 0)
			root.left = add(root.left, key, value);
		else if (key.compareTo(root.key) > 0)
			root.right = add(root.right, key, value);
		else root.value = value; //������Ϊ����key��ʱ����value��Ҫ����value
		
		return root;
	}
	
	//������rootΪ���Ķ�����������, key���ڵĽ��
	private Node getNode(Node root, K key) { 
		if (root == null)
			return null;
		if (key.compareTo(root.key) == 0) //key.equals(root.key)
			return root;
		else if (key.compareTo(root.key) < 0)
			return getNode(root.left, key);
		else return getNode(root.right, key);
	}
	
	@Override
	public boolean contains(K key) {
		return getNode(root, key) != null;
	}
	
	@Override
	public V get(K key) {
		Node node = getNode(root, key);
		return node == null ? null : node.value;
	}
	
	@Override
	public void set(K key, V newValue) {
		Node node = getNode(root, key);
		if (node == null) //��ǰ��һ��, ������ʱ�׳��쳣
			throw new IllegalArgumentException(key + "doesn't exist!");
		node.value = newValue;	
	}
	
	@Override
	public V remove(K key) {
		Node node = getNode(root, key);
		if (node != null) { //���ʹ��ԭ����ɾ������, ���ǲ�֪��key�Ƿ����, ���Ҫ������һ��
			root = remove(root, key);
			return node.value;
		}
		return null;
	}
	//ɾ������rootΪ���Ķ����������м�Ϊkey�Ľ��, �ݹ��㷨
	//����ɾ�������µĶ����������ĸ�
	private Node remove(Node root, K key) {
		if (root == null)
			return null;
		
		if (key.compareTo(root.key) < 0) {
			root.left = remove(root.left, key);
			return root;
		}
		else if (key.compareTo(root.key) > 0) {
			root.right = remove(root.right, key);
			return root;
		}
		else { //�����ɾ������������Ϊ�յ����
			if (root.left == null) {
				Node rightNode = root.right;
				root.right = null; //������������
				--size;
				return rightNode;
			} // ��ɾ������������Ϊ��
			if (root.right == null) {
				Node leftNode = root.left;
				root.left = null; //������������
				--size;
				return leftNode;
			}
			//��ɾ�����������������Ϊ�յ����
			//�ҵ��ȴ�ɾ���������С���, ����ɾ���������������С���
			//�������������ɾ������λ��
			Node successor = minimum(root.right);
			successor.right = removeMin(root.right);
			successor.left = root.left;
			root.left = root.right = null; //������������
			return successor;
		}
	}
	//������NodeΪ���Ķ�������������Сֵ���ڵĽ��
	private Node minimum(Node node) {
		if (node.left == null) return node;
		return minimum(node.left);
	}
	//ɾ����nodeΪ���Ķ�������������С���
	//����ɾ�������µĶ������������ĸ�
	private Node removeMin(Node node) {
		if (node.left == null) {
			Node rightNode = node.right;
			node.right = null; 
			--size;
			return rightNode;
		}
		//ɾ���������е���Сֵ���õ��µ�������
		node.left = removeMin(node.left); 
		return node;
	}
	
	public static void main(String[] args) {
		System.out.println("Pride and Prejudice");
		
		Array<String> words = new Array<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words)) {
			System.out.println("Total words: " + words.getSize());
			
			BSTMap<String, Integer> map = new BSTMap<>();
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
