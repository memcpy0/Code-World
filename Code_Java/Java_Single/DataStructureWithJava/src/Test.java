import java.util.Random;

public class Test {
	private static double testStack(Stack<Integer> stack, int opCount) {
		long startTime = System.nanoTime(); //纳秒
		
		Random random = new Random();
		for (int i = 0; i < opCount; ++i)
			stack.push(random.nextInt(Integer.MAX_VALUE)); //[0, bound)的一个随机数
		for (int i = 0; i < opCount; ++i)
			stack.pop();
		
		long endTime = System.nanoTime();
		
		return (endTime - startTime) / 1000000000.0;
	}
	
	private static double testQueue(Queue<Integer> q, int opCount) {
		long startTime = System.nanoTime(); //纳秒
		
		Random random = new Random();
		for (int i = 0; i < opCount; ++i)
			q.enqueue(random.nextInt(Integer.MAX_VALUE)); //[0, bound)的一个随机数
		for (int i = 0; i < opCount; ++i)
			q.dequeue();
		
		long endTime = System.nanoTime();
		
		return (endTime - startTime) / 1000000000.0;
	}
	
	private static double testSet(Set<String> set, String filename) {
		long startTime = System.nanoTime();
		
		System.out.println(filename);
		Array<String> words = new Array<>();  
		if (FileOperation.readFile(filename, words))
			System.out.println("Total words: " + words.getSize());
		for (String word : words) {
			set.add(word);
		}
		System.out.println("Total different words: " + set.getSize());
		
		long endTime = System.nanoTime();
		return (endTime - startTime) / 1000000000.0;
	}
	
	private static double testMap(Map<String, Integer> map, String filename) { //词频统计
		long startTime = System.nanoTime();
		
		System.out.println(filename);
		Array<String> words = new Array<>();
		if (FileOperation.readFile(filename, words))
			System.out.println("Total words: " + words.getSize());
		for (String word : words) {
			if (map.contains(word))
				map.set(word, map.get(word) + 1);
			else 
				map.add(word, 1);
		}
		System.out.println("Total different words: " + map.getSize()); //不同的单词数
		System.out.println("Frequency of 'Pride': " + map.get("pride")); //傲慢出现的次数
		System.out.println("Frequency of 'Prejudice: " + map.get("prejudice")); //偏见出现的次数
		
		long endTime = System.nanoTime();
		return (endTime - startTime) / 1000000000.0;
	}
	
	public static void main(String[] args) {
		int opCount = 10_0000;
		
//		String filename = "pride-and-prejudice.txt";
//		System.out.println("BSTSet Test:");
//		BSTSet<String> bstSet = new BSTSet<>();
//		double time1 = testSet(bstSet, filename);
//		System.out.println(time1 + "s.");
//		
//		System.out.println("LinkedListSet Test:");
//		LinkedListSet<String> listSet = new LinkedListSet<>();
//		double time2 = testSet(listSet, filename);
//		System.out.println(time2 + "s.");
		
		String filename = "pride-and-prejudice.txt";
		System.out.println("BSTMap Test:");
		BSTMap<String, Integer> bstMap = new BSTMap<>();
		double time1 = testMap(bstMap, filename);
		System.out.println(time1 + "s.\n");
		
		System.out.println("LinkedListMap Test:");
		LinkedListMap<String, Integer> listMap = new LinkedListMap<>();
		double time2 = testMap(listMap, filename);
		System.out.println(time2 + "s.");
		
//		ArrayQueue<Integer> arrayQueue = new ArrayQueue<>();
//		double time1 = testQueue(arrayQueue, opCount);
//		System.out.println("ArrayQueue, time: " + time1 + "s.");
//		
//		LoopQueue<Integer> loopQueue = new LoopQueue<>();
//		double time2 = testQueue(loopQueue, opCount);
//		System.out.println("LoopQueue, time: " + time2 + "s.");
//		
//		LinkedQueue<Integer> linkedQueue = new LinkedQueue<>();
//		double time3 = testQueue(linkedQueue, opCount);
//		System.out.println("LinkedQueue, time: " + time3 + "s.");
//		ArrayStack<Integer> arrayStack = new ArrayStack<>();
//		double time1 = testStack(arrayStack, opCount);
//		System.out.println("ArrayStack, time: " + time1 + "s.");
//		
//		LinkedStack<Integer> linkedStack = new LinkedStack<>();
//		double time2 = testStack(linkedStack, opCount);
//		System.out.println("LinkedStack, time: " + time2 + "s.");
	} 
}
