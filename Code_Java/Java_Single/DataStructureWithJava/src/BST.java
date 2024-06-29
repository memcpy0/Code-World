import java.util.Random;

public class BST<E extends Comparable<E>>{

	private class Node {
		public E e;
		public Node left, right;
		
		public Node(E e) {
			this.e = e;
			left = null;
			right = null;
		}
	}
	
	private Node root;
	private int size;
	
	public BST() {
		root = null;
		size = 0;
	}
	public int size() {
		return size;
	}
	public boolean isEmpty() {
		return size == 0;
	}
	
	//����nodeΪ���Ķ�������������Ԫ��e
	public void add(E e) {  
		root = add(root, e);
	}
	//˽�а汾, �ڲ�����
	private Node add(Node root, E e) {
		if (root == null) {
			++size; //**ע��, �����++sizeд������, ����д�������add��, ��Ϊ����ÿ��e������������������
			return new Node(e);
		}
		if (e.compareTo(root.e) < 0)
			root.left = add(root.left, e);
		else if (e.compareTo(root.e) > 0)
			root.right = add(root.right, e);
		return root;
	}
	
	//����rootΪ���Ķ����������Ƿ���e
	public boolean contains(E e) {
		return contains(root, e);
	}
	private boolean contains(Node root, E e) {
		if (root == null) return false;
		
		if (e.equals(root.e)) return true;
		else if (e.compareTo(root.e) < 0) return contains(root.left, e);
		else return contains(root.right, e);
	}
	
	//��������ǰ�����
	public void preOrder() {
		preOrder(root);
	}
	private void preOrder(Node root) {
		if (root == null) return;
		System.out.println(root.e);
		preOrder(root.left);
		preOrder(root.right);
	}
	//�ǵݹ���ʽǰ�����
	public void preOrderNR() { 
		LinkedStack<Node> stack = new LinkedStack<>();
		stack.push(root);
		
		while (!stack.isEmpty()) {
			Node cur = stack.pop();
			System.out.println(cur.e);
			if (cur.right != null) stack.push(cur.right); //�������Ƚ�
			if (cur.left != null) stack.push(cur.left);
		}
	}
	
	//���������������
	public void inOrder() {
		inOrder(root);
	}
	private void inOrder(Node root) {
		if (root == null) return;
		
		inOrder(root.left);
		System.out.println(root.e);
		inOrder(root.right);
	} 
	
	//�������ĺ������
	public void postOrder() {
		postOrder(root);
	}
	private void postOrder(Node root) {
		if (root == null) return;
		
		postOrder(root.left);
		postOrder(root.right);
		System.out.println(root.e);
	} 
	
	//�������
	public void BFS() {
		LoopQueue<Node> queue = new LoopQueue<>();
		queue.enqueue(root);
		while (!queue.isEmpty()) {
			Node cur = queue.dequeue();
			System.out.println(cur.e);
			if (cur.left != null) queue.enqueue(cur.left);
			if (cur.right != null) queue.enqueue(cur.right);
		}
	}
	
	//������Сֵ
	public E minimum() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		
		return minimum(root).e;
	}
	//������NodeΪ���Ķ�������������Сֵ���ڵĽ��
	private Node minimum(Node node) {
		if (node.left == null) return node;
		return minimum(node.left);
	}
	//������Сֵ(�ǵݹ�) 
	public E minimumNR() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		Node ans = root;
		while (ans.left != null) ans = ans.left;
		return ans.e;
	}
	
	//�������ֵ
	public E maximum() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		
		return maximum(root).e;
	}
	//������NodeΪ���Ķ�������������Сֵ���ڵĽ��
	private Node maximum(Node node) {
		if (node.right == null) return node;
		return maximum(node.right);
	}
	//�������ֵ(�ǵݹ�) 
	public E maximumNR() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		Node ans = root;
		while (ans.right != null) ans = ans.right;
		return ans.e;
	}
	
	//ɾ����������������С��㲢������ֵ
	public E removeMin() {
		E ret = minimum(); //�������������
		root = removeMin(root);
		return ret;
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
	
	//ɾ������������������㲢������ֵ
	public E removeMax() {
		E ret = maximum();
		root = removeMax(root);
		return ret;
	}
	//ɾ����nodeΪ���Ķ����������������
	//����ɾ�������µĶ������������ĸ�
	private Node removeMax(Node node) {
		if (node.right == null) {
			Node leftNode = node.left; //���ֵ��������������
			node.left = null;
			--size;
			return leftNode;
		}
		//ɾ���������е����ֵ���õ��µ�������		
		node.right = removeMax(node.right);
		return node;
	}
	
	//�Ӷ�����������ɾ��Ԫ��Ϊe�Ľ��
	public void remove(E e) {
		root = remove(root, e);
	}
	//ɾ����nodeΪ���Ķ�����������ֵΪe�Ľ�㣬�ݹ��㷨
	//����ɾ�������µĶ����������ĸ�
	private Node remove(Node node, E e) {
		if (node == null) return null;
		if (e.compareTo(node.e) < 0) {
			node.left = remove(node.left, e);
			return node;
		}
		else if (e.compareTo(node.e) < 0) {
			node.right = remove(node.right, e);
			return node;
		}
		else { //e == node.e
			if (node.left == null) { //��ɾ�����������Ϊ�������
				Node rightNode = node.right;
				node.right = null;
				--size;
				return rightNode;
			} 
			if (node.right == null) {
				Node leftNode = node.left;
				node.left = null;
				--size;
				return leftNode;
			}
			
			Node successor = minimum(node.right);
			successor.right = removeMin(node.right);
			successor.left = node.left;
			node.left = node.right = null;
			//���ﲻ��д--size;��Ϊ�Ѿ���removeMin�м�����size
			return successor;
		}
	}
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		generateBSTString(root, 0, res);
		return res.toString();
	}
	//����һ�������������������ַ���, ��rootΪ���ڵ㡢���Ϊdepth���������������ַ��� 
	private void generateBSTString(Node root, int depth, StringBuilder res) {
		if (root == null) {
			res.append(generateDepthString(depth) + "null\n");
			return;
		}
		res.append(generateDepthString(depth) + root.e + "\n");
		generateBSTString(root.left, depth + 1, res);
		generateBSTString(root.right, depth + 1, res);
	}
	//������ȱ�ʾ��--
	private String generateDepthString(int depth) {
		StringBuilder res = new StringBuilder();
		for (int i = 0; i < depth; ++i)
			res.append("--"); 
		return res.toString();
	}
	
	
	public static void main(String[] args) {
//		BST<Integer> bst = new BST<>();
//		int[] nums = {5, 3, 6, 8, 4, 2};
//		for (int num : nums)
//			bst.add(num); 
//		
//		bst.BFS();
//		bst.preOrder();
//		System.out.println();
//		bst.preOrderNR(); //�ǵݹ�ǰ�����
//		System.out.println();
//		System.out.println(bst);
		//test removeMin
		BST<Integer> bst = new BST<>();
		Random random = new Random();
		int n = 1000;
		for (int i = 0; i < n; ++i)
			bst.add(random.nextInt(10000));
		Array<Integer> nums = new Array<>();
		while (!bst.isEmpty()) 
			nums.addLast(bst.removeMin());
		System.out.println(nums); //��ӡ��С�������������
		for (int i = 1; i < nums.getSize(); ++i)
			if (nums.get(i - 1) > nums.get(i))
				throw new IllegalArgumentException("Error");
		System.out.println("removeMin test completed.");
		
		//test removeMax 
		for (int i = 0; i < n; ++i)
			bst.add(random.nextInt(10000));
		nums = new Array<>();
		while (!bst.isEmpty())
			nums.addLast(bst.removeMax());
		System.out.println(nums); //��ӡ�Ӵ�С���������
		for (int i = 1; i < nums.getSize(); ++i)
			if (nums.get(i - 1) < nums.get(i))
				throw new IllegalArgumentException("Error");
		System.out.println("removeMax test completed.");
		
	}

}
