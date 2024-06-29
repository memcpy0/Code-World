
public class LinkedListSet<E> implements Set<E> {
	private LinkedList<E> list;
	public LinkedListSet() {
		list = new LinkedList<>();
	}
	public int getSize() {
		return list.getSize();
	}
	public boolean isEmpty() {
		return list.isEmpty();
	}
	public boolean contains(E e) {
		return list.contains(e);
	}
	public void add(E e) { 
		//以前实现的LinkedList中没有规定不能包含重复元素
		//因此稍微麻烦一点
		if (list.contains(e) == false)
			list.addFirst(e); //以前实现的是单链表, 没有尾指针
	}
	public void remove(E e) {
		list.removeElement(e);
	}
	
	public static void main(String[] args) {
		System.out.println("A Tale of Two Cities");
		Array<String> words = new Array<>();
		LinkedListSet<String> set = new LinkedListSet<>();
		if (FileOperation.readFile("a-tale-of-two-cities.txt", words))
			System.out.println("Total words: " + words.getSize());
		for (String word : words) {
			set.add(word);
		}
		System.out.println("Total different words: " + set.getSize());
		
		System.out.println();
		
		System.out.println("Pride and Prejudice");
		Array<String> words2 = new Array<>();
		LinkedListSet<String> set2 = new LinkedListSet<>();
		if (FileOperation.readFile("pride-and-prejudice.txt", words2))
			System.out.println("Total words: " + words.getSize());
		for (String word : words2) {
			set2.add(word);
		}
		System.out.println("Total different words: " + set2.getSize());
	}
}
