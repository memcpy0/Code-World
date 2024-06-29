
public class SegmentTree<E> {
	private E[] data; //ԭʼ����
	private E[] tree;
	private Merger<E> merger; //�ں���
	
	public SegmentTree(E[] arr, Merger<E> merger) {
		data = (E[])new Object[arr.length];
		for (int i = 0; i < arr.length; ++i)
			data[i] = arr[i];
		tree = (E[])new Object[arr.length * 4];
		this.merger = merger;
		buildSegmentTree(0, 0, data.length - 1); //treeIndex, l, r
	}
	
	private void buildSegmentTree(int treeIndex, int l, int r) {
		if (l == r) { //ֻ��һ��Ԫ��ʱ������Ҷ�ӽ��
			tree[treeIndex] = data[l];
			return;
		}
		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);
		int mid = l + (r - l) / 2; 
		buildSegmentTree(leftTreeIndex, l, mid); //�ȹ�����������
		buildSegmentTree(rightTreeIndex, mid + 1, r);
		//����;�����+; ���ֵ��Сֵ����max,min
		//������E���治һ�������˼ӷ�; ͬʱ, ����ϣ���û�����ҵ�񳡾���������߼�ʹ���߶���
		tree[treeIndex] = merger.merge(tree[leftTreeIndex], tree[rightTreeIndex]);
	}
	
	//����[queryL, queryR]�����ֵ
	public E query(int queryL, int queryR) {
		if (queryL < 0 || queryL >= data.length
				|| queryR < 0 || queryR >= data.length || queryL > queryR)
			throw new IllegalArgumentException("Index is illegal.");
		//treeIndex, l, r, queryL, queryR
		return query(0, 0, data.length - 1, queryL, queryR); 
	}
	//����treeindexΪ�����߶���[l...r]�ķ�Χ�У���������[queryL...queryR]��ֵ
	//���䷶ΧҲ���԰�װΪһ���ڲ���
	private E query(int treeIndex, int l, int r, int queryL, int queryR) {
		if (l == queryL && r == queryR) //���û���ע������
			return tree[treeIndex];
		int mid = l + (r - l) / 2;
		int leftTreeIndex = leftChild(treeIndex);
		int rightTreeIndex = rightChild(treeIndex);
		
		if (queryL >= mid + 1)  //���������޹�
			return query(rightTreeIndex, mid + 1, r, queryL, queryR);
		else if (queryR <= mid) //�û����ĵ��������������޹�
			return query(leftTreeIndex, l, mid, queryL, queryR);
		
		E leftResult = query(leftTreeIndex, l, mid, queryL, mid); //���û����ĵ�����Ҳ�ֳ�����
		E rightResult = query(rightTreeIndex, mid + 1, r, mid + 1, queryR);
			return merger.merge(leftResult, rightResult); //���������ں���merger
	}
	
	//��indexλ�õ�Ԫ�ظ���Ϊe
	public void set(int index, E e) {
		if (index < 0 || index >= data.length)
			throw new IllegalArgumentException("Index is illegal.");
		data[index] = e;
		set(0, 0, data.length - 1, index, e); //treeIndex, l,r, index, e
	}
	//����treeIndexΪ�����߶����и���index��ֵΪe
	private void set(int treeIndex, int l, int r, int index, E e) {
		if (l == r) { //ֱ���޸�Ҷ�ӽ����Ԫ�ص�ֵ
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
		//�ӵײ����ϸ����߶��� 
		tree[treeIndex] = merger.merge(tree[leftTreeIndex], tree[rightTreeIndex]); //���������ں���merger
	}

	public int getSize() {
		return data.length;
	}
	public E get(int index) {
		if (index < 0 || index >= data.length)
			throw new IllegalArgumentException("Index is illegal.");
		return data[index];
	}
	//������ȫ�������������ʾ�У�һ����������ʾ��Ԫ�ص����ӽ�������
	private int leftChild(int index) { 
		return 2 * index + 1;
	}
	//����һ����������ʾ�����ӵ�����
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
		SegmentTree<Integer> segTree = new SegmentTree<>(nums, (a, b) -> a + b); //lambda���ʽ
		
		System.out.println(segTree.query(0, 2)); //��������[1,2]�ĺ�-2+0+3=1
		System.out.println(segTree.query(2, 5)); //-1
		System.out.println(segTree.query(0, 5)); //-3
//		SegmentTree<Integer> segTree = new SegmentTree<>(nums, new Merger<Integer>() {
//			@Override
//			public Integer merge(Integer a, Integer b) {
//				return a + b;
//			}
//		}); //������
	}
}
