
public class Student {
	private int score;
	private String name;
	
	public Student(String _name, int _score) {
		name = _name;
		score = _score;
	}
	
	@Override 
	public String toString() {
		return String.format("Student(name: %s, socre: %d)", name, score);	
	}
	
	public static void main(String[] args) {
		Array<Student> arr = new Array<>();
		arr.addLast(new Student("Alice", 100));
		arr.addLast(new Student("Bob", 77));;
		arr.addLast(new Student("Charlie", 66));
		System.out.println(arr);

	}

}
