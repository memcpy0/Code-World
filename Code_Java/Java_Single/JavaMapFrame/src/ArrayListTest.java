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
		System.out.println(list); // �Զ�����list�����toString������ArrayListд��Object��toString()
		
		List listA = new ArrayList();
		List listB = new ArrayList();
		System.out.println("������A��ѧԱ����������OVER������");
		inputName(listA);
		System.out.println("������B��ѧԱ����������OVER������");	
		inputName(listB);
		
		listA.addAll(listB); // �ϲ�listB�е����ݵ�listA�У���������ѧ�������Լ��Ͻ��в��Һ�ɾ��
		
		System.out.println("������Ҫ���ҵ�ѧԱ������");
		String name = scanner.next();
		int pos = listA.indexOf(name);
		if (pos == -1) System.out.println("û�У�");
		else System.out.println("�ҵ��ˣ�λ����" + pos);
		
		System.out.println("������Ҫɾ����ѧԱ������");
		String delName = scanner.next();
		if (listA.remove(delName)) System.out.println("ɾ���ɹ�!");
		else System.out.println("ɾ��ʧ��!");
	}
}
