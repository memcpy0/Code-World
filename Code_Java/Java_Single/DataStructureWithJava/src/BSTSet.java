
public class BSTSet<E extends Comparable<E>> implements Set<E> {
	private BST<E> bst;
	//实际上我们实现的BST完全支持集合的所有操作
	public BSTSet() {
		bst = new BST<>();
	}
	
	@Override
	public int getSize() {
		return bst.size();
	}
	
	@Override
	public boolean isEmpty() {
		return bst.isEmpty();
	}
	
	@Override
	public void add(E e) {
		bst.add(e);
	}
	
	@Override
	public boolean contains(E e) {
		return bst.contains(e);
	}
	
	@Override
	public void remove(E e) {
		bst.remove(e);
	}
	 
	public static void main(String[] args) {
		System.out.println("A Tale of Two Cities");
		Array<String> words = new Array<>();
		BSTSet<String> set = new BSTSet<>();
		if (FileOperation.readFile("a-tale-of-two-cities.txt", words))
			System.out.println("Total words: " + words.getSize());
		for (String word : words) {
			set.add(word);
		}
		System.out.println("Total different words: " + set.getSize());
		
		System.out.println();
		
		System.out.println("Pride and Prejudice");
		Array<String> words2 = new Array<>();
		BSTSet<String> set2 = new BSTSet<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words2))
			System.out.println("Total words: " + words.getSize());
		for (String word : words2) {
			set2.add(word);
		}
		System.out.println("Total different words: " + set2.getSize());
	}
}
