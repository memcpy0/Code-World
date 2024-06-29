import java.util.List;
import java.util.ArrayList; 

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

public class ArrayListTest {
	private static java.util.Scanner scanner = new java.util.Scanner(System.in);
	public static void inputName(List list) {
		do {
			String name = scanner.next();
			if (name.equalsIgnoreCase("OVER")) break;
			list.add(name);
		} while (true);
	}
	
	public static void main(String[] args) {
		List list = new ArrayList();
		Student std = new Student("Tom", 10);
		for (int i = 0; i < 3; i++) {
			std.age = 10 + i;
			list.add(std);
		}
		System.out.println(list); // 自动调用list对象的toString方法，ArrayList写了Object的toString()
		
		List listA = new ArrayList();
		List listB = new ArrayList();
		System.out.println("请输入A班学员姓名，输入OVER结束！");
		inputName(listA);
		System.out.println("请输入B班学员姓名，输入OVER结束！");	
		inputName(listB);
		
		listA.addAll(listB); // 合并listB中的数据到listA中，接着输入学生姓名对集合进行查找和删除
		
		System.out.println("请输入要查找的学员姓名：");
		String name = scanner.next();
		int pos = listA.indexOf(name);
		if (pos == -1) System.out.println("没有！");
		else System.out.println("找到了，位置是" + pos);
		
		System.out.println("请输入要删除的学员姓名：");
		String delName = scanner.next();
		if (listA.remove(delName)) System.out.println("删除成功!");
		else System.out.println("删除失败!");
	}
}
