
public class SegmentTree<E> {
	private E[] data; //原始数据
	private E[] tree;
	private Merger<E> merger; //融合器
	
	public SegmentTree(E[] arr, Merger<E> merger) {
		data = (E[])new Object[arr.length];
		for (int i = 0; i < arr.length; ++i)
			data[i] = arr[i];
		tree = (E[])new Object[arr.length * 4];
		this.merger = merger;
		buildSegmentTree(0, 0, data.length - 1); //treeIndex, l, r
	}
	
	private void buildSegmentTree(int treeIndex, int l, int r) {
		if (l == r) { //只有一个元素时，创建叶子结点
			tree[treeIndex] = data[l];
			return;
		}
		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);
		int mid = l + (r - l) / 2; 
		buildSegmentTree(leftTreeIndex, l, mid); //先构建两棵子树
		buildSegmentTree(rightTreeIndex, mid + 1, r);
		//区间和就是用+; 最大值最小值就是max,min
		//问题是E上面不一定定义了加法; 同时, 我们希望用户根据业务场景自由组合逻辑使用线段树
		tree[treeIndex] = merger.merge(tree[leftTreeIndex], tree[rightTreeIndex]);
	}
	
	//返回[queryL, queryR]区间的值
	public E query(int queryL, int queryR) {
		if (queryL < 0 || queryL >= data.length
				|| queryR < 0 || queryR >= data.length || queryL > queryR)
			throw new IllegalArgumentException("Index is illegal.");
		//treeIndex, l, r, queryL, queryR
		return query(0, 0, data.length - 1, queryL, queryR); 
	}
	//在以treeindex为根的线段树[l...r]的范围中，搜索区间[queryL...queryR]的值
	//区间范围也可以包装为一个内部类
	private E query(int treeIndex, int l, int r, int queryL, int queryR) {
		if (l == queryL && r == queryR) //是用户关注的区间
			return tree[treeIndex];
		int mid = l + (r - l) / 2;
		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);
		
		if (queryL >= mid + 1)  //与左区间无关
			return query(rightTreeIndex, mid + 1, r, queryL, queryR);
		else if (queryR <= mid) //用户关心的区间与右区间无关
			return query(leftTreeIndex, l, mid, queryL, queryR);
		
		E leftResult = query(leftTreeIndex, l, mid, queryL, mid); //把用户关心的区间也分成两半
		E rightResult = query(rightTreeIndex, mid + 1, r, mid + 1, queryR);
			return merger.merge(leftResult, rightResult); //两半区间融合用merger
	}
	
	//将index位置的元素更新为e
	public void set(int index, E e) {
		if (index < 0 || index >= data.length)
			throw new IllegalArgumentException("Index is illegal.");
		data[index] = e;
		set(0, 0, data.length - 1, index, e); //treeIndex, l,r, index, e
	}
	//在以treeIndex为根的线段树中更新index的值为e
	private void set(int treeIndex, int l, int r, int index, E e) {
		if (l == r) { //直接修改叶子结点上元素的值
			tree[treeIndex] = e;
			return;
		}
		int mid = l + (r - l) / 2;
		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);
		if (index >= mid + 1)
			set(rightTreeIndex, mid + 1, r, index, e);
		else //index <= mid
			set(rightTreeIndex, l, mid, index, e);
		//从底部往上更新线段树 
		tree[treeIndex] = merger.merge(tree[leftTreeIndex], tree[rightTreeIndex]); //两半区间融合用merger
	}

	public int getSize() {
		return data.length;
	}
	public E get(int index) {
		if (index < 0 || index >= data.length)
			throw new IllegalArgumentException("Index is illegal.");
		return data[index];
	}
	//返回完全二叉树的数组表示中，一个索引所表示的元素的左孩子结点的索引
	private int leftChild(int index) { 
		return 2 * index + 1;
	}
	//返回一个索引所表示的左孩子的索引
	private int rightChild(int index) {
		return 2 * index + 2;
	}  
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append('[');
		for (int i = 0; i < tree.length; ++i) {
			if (tree[i] != null) 
				sb.append(tree[i]);
			else
				sb.append("null");
			if (i != tree.length - 1) sb.append(' ');
		}
		return sb.toString();
	}
	
	public static void main(String[] args) {
		Integer[] nums = {-2, 0, 3, -5, 2, -1};
		SegmentTree<Integer> segTree = new SegmentTree<>(nums, (a, b) -> a + b); //lambda表达式
		
		System.out.println(segTree.query(0, 2)); //计算区间[1,2]的和-2+0+3=1
		System.out.println(segTree.query(2, 5)); //-1
		System.out.println(segTree.query(0, 5)); //-3
//		SegmentTree<Integer> segTree = new SegmentTree<>(nums, new Merger<Integer>() {
//			@Override
//			public Integer merge(Integer a, Integer b) {
//				return a + b;
//			}
//		}); //匿名类
	}
}
