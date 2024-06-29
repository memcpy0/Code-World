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
	
	//返回键值对的数量
	@Override
	public int getSize() { return size; } 
	//是否为空
	@Override
	public boolean isEmpty() { return size == 0; }
	//向二分搜索树中添加新的元素
	@Override
	public void add(K key, V value) {
		root = add(root, key, value);
	}
	//向以root为根的二叉搜索树中插入元素(key, value), 递归算法
	//返回插入新结点的二分搜索树的根
	private Node add(Node root, K key, V value) {
		if (root == null) {
			++size;
			return new Node(key, value);
		} 
		if (key.compareTo(root.key) < 0)
			root.left = add(root.left, key, value);
		else if (key.compareTo(root.key) > 0)
			root.right = add(root.right, key, value);
		else root.value = value; //我们认为存在key的时候传入value是要更新value
		
		return root;
	}
	
	//返回以root为根的二分搜索树中, key所在的结点
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
		if (node == null) //和前面一样, 不存在时抛出异常
			throw new IllegalArgumentException(key + "doesn't exist!");
		node.value = newValue;	
	}
	
	@Override
	public V remove(K key) {
		Node node = getNode(root, key);
		if (node != null) { //如果使用原来的删除函数, 我们不知道key是否存在, 因此要先搜索一次
			root = remove(root, key);
			return node.value;
		}
		return null;
	}
	//删除掉以root为根的二分搜索树中键为key的结点, 递归算法
	//返回删除结点后新的二分搜索树的根
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
		else { //如果待删除结点的左子树为空的情况
			if (root.left == null) {
				Node rightNode = root.right;
				root.right = null; //方便垃圾回收
				--size;
				return rightNode;
			} // 待删除结点的右子树为空
			if (root.right == null) {
				Node leftNode = root.left;
				root.left = null; //方便垃圾回收
				--size;
				return leftNode;
			}
			//待删除结点左右子树均不为空的情况
			//找到比待删除结点大的最小结点, 即待删除结点右子树的最小结点
			//用这个结点替代待删除结点的位置
			Node successor = minimum(root.right);
			successor.right = removeMin(root.right);
			successor.left = root.left;
			root.left = root.right = null; //方便垃圾回收
			return successor;
		}
	}
	//返回以Node为根的二分搜索树的最小值所在的结点
	private Node minimum(Node node) {
		if (node.left == null) return node;
		return minimum(node.left);
	}
	//删除以node为根的二分搜索树的最小结点
	//返回删除结点后新的二分搜索子树的根
	private Node removeMin(Node node) {
		if (node.left == null) {
			Node rightNode = node.right;
			node.right = null; 
			--size;
			return rightNode;
		}
		//删除左子树中的最小值并得到新的左子树
		node.left = removeMin(node.left); 
		return node;
	}
	
	public static void main(String[] args) {
		System.out.println("Pride and Prejudice");
		
		Array<String> words = new Array<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words)) {
			System.out.println("Total words: " + words.getSize());
			
			BSTMap<String, Integer> map = new BSTMap<>();
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
