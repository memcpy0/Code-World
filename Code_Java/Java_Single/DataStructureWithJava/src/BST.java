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
	
	//向以node为根的二叉搜索树插入元素e
	public void add(E e) {  
		root = add(root, e);
	}
	//私有版本, 内部调用
	private Node add(Node root, E e) {
		if (root == null) {
			++size; //**注意, 必须把++size写在这里, 不能写在上面的add中, 因为不是每个e都会真正被插入树中
			return new Node(e);
		}
		if (e.compareTo(root.e) < 0)
			root.left = add(root.left, e);
		else if (e.compareTo(root.e) > 0)
			root.right = add(root.right, e);
		return root;
	}
	
	//看以root为根的二叉搜索树是否含有e
	public boolean contains(E e) {
		return contains(root, e);
	}
	private boolean contains(Node root, E e) {
		if (root == null) return false;
		
		if (e.equals(root.e)) return true;
		else if (e.compareTo(root.e) < 0) return contains(root.left, e);
		else return contains(root.right, e);
	}
	
	//二叉树的前序遍历
	public void preOrder() {
		preOrder(root);
	}
	private void preOrder(Node root) {
		if (root == null) return;
		System.out.println(root.e);
		preOrder(root.left);
		preOrder(root.right);
	}
	//非递归形式前序遍历
	public void preOrderNR() { 
		LinkedStack<Node> stack = new LinkedStack<>();
		stack.push(root);
		
		while (!stack.isEmpty()) {
			Node cur = stack.pop();
			System.out.println(cur.e);
			if (cur.right != null) stack.push(cur.right); //右子树先进
			if (cur.left != null) stack.push(cur.left);
		}
	}
	
	//二叉树的中序遍历
	public void inOrder() {
		inOrder(root);
	}
	private void inOrder(Node root) {
		if (root == null) return;
		
		inOrder(root.left);
		System.out.println(root.e);
		inOrder(root.right);
	} 
	
	//二叉树的后序遍历
	public void postOrder() {
		postOrder(root);
	}
	private void postOrder(Node root) {
		if (root == null) return;
		
		postOrder(root.left);
		postOrder(root.right);
		System.out.println(root.e);
	} 
	
	//层序遍历
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
	
	//返回最小值
	public E minimum() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		
		return minimum(root).e;
	}
	//返回以Node为根的二分搜索树的最小值所在的结点
	private Node minimum(Node node) {
		if (node.left == null) return node;
		return minimum(node.left);
	}
	//返回最小值(非递归) 
	public E minimumNR() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		Node ans = root;
		while (ans.left != null) ans = ans.left;
		return ans.e;
	}
	
	//返回最大值
	public E maximum() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		
		return maximum(root).e;
	}
	//返回以Node为根的二分搜索树的最小值所在的结点
	private Node maximum(Node node) {
		if (node.right == null) return node;
		return maximum(node.right);
	}
	//返回最大值(非递归) 
	public E maximumNR() {
		if (size == 0)
			throw new IllegalArgumentException("BST is empty!");
		Node ans = root;
		while (ans.right != null) ans = ans.right;
		return ans.e;
	}
	
	//删除二分搜索树的最小结点并返回其值
	public E removeMin() {
		E ret = minimum(); //在这里面检查参数
		root = removeMin(root);
		return ret;
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
	
	//删除二分搜索树的最大结点并返回其值
	public E removeMax() {
		E ret = maximum();
		root = removeMax(root);
		return ret;
	}
	//删除以node为根的二分搜索树的最大结点
	//返回删除结点后新的二分搜索子树的根
	private Node removeMax(Node node) {
		if (node.right == null) {
			Node leftNode = node.left; //最大值结点可能有左子树
			node.left = null;
			--size;
			return leftNode;
		}
		//删除右子树中的最大值并得到新的右子树		
		node.right = removeMax(node.right);
		return node;
	}
	
	//从二分搜索树中删除元素为e的结点
	public void remove(E e) {
		root = remove(root, e);
	}
	//删除以node为根的二分搜索树中值为e的结点，递归算法
	//返回删除结点后新的二分搜索树的根
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
			if (node.left == null) { //待删除结点左子树为根的情况
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
			//这里不用写--size;因为已经在removeMin中减少了size
			return successor;
		}
	}
	
	@Override
	public String toString() {
		StringBuilder res = new StringBuilder();
		generateBSTString(root, 0, res);
		return res.toString();
	}
	//生成一个描述二叉搜索树的字符串, 以root为根节点、深度为depth的描述二叉树的字符串 
	private void generateBSTString(Node root, int depth, StringBuilder res) {
		if (root == null) {
			res.append(generateDepthString(depth) + "null\n");
			return;
		}
		res.append(generateDepthString(depth) + root.e + "\n");
		generateBSTString(root.left, depth + 1, res);
		generateBSTString(root.right, depth + 1, res);
	}
	//生成深度表示符--
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
//		bst.preOrderNR(); //非递归前序遍历
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
		System.out.println(nums); //打印从小到大排序的数组
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
		System.out.println(nums); //打印从大到小排序的数组
		for (int i = 1; i < nums.getSize(); ++i)
			if (nums.get(i - 1) < nums.get(i))
				throw new IllegalArgumentException("Error");
		System.out.println("removeMax test completed.");
		
	}

}
