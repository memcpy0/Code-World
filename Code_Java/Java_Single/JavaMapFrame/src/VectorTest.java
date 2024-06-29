import java.util.List;
import java.util.Vector;
// List和Vector使用
// Vector实现了List接口，实现可增长的对象数组
class Student {
	String name;
	int age;
	public Student(String name, int age) {
		this.name = name;
		this.age = age;
	}
	public String toString() {
		return name + "/" + age;
	}
}

public class VectorTest {
	public static void main(String[] args) {
		List list = new Vector();
		System.out.println("集合为空吗? " + list.isEmpty());
		// 添加一个元素
		list.add("第一个元素");
		System.out.println("集合为空吗? " + list.isEmpty());
		// 集合里面不能存放基本类型元素，但可以自动装箱
		list.add(10.0);
		System.out.println("集合中元素的个数为: " + list.size());
		// 删除元素
		list.remove(10.0);
		System.out.println("集合中元素的个数为: " + list.size());
		System.out.println("包含'第一个元素': " + list.contains("第一个元素")); 
		List listB = new Vector();
		Student std = new Student("Tom", 10);
		for (int i = 0; i < 3; i++) {
			std.age = 10 + i;
			listB.add(std);
		}
		System.out.println(listB); // 自动调用list对象的toString方法，ArrayList写了Object的toString() 
	}
}
