import java.util.List;
import java.util.Vector;
// List��Vectorʹ��
// Vectorʵ����List�ӿڣ�ʵ�ֿ������Ķ�������
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
		System.out.println("����Ϊ����? " + list.isEmpty());
		// ���һ��Ԫ��
		list.add("��һ��Ԫ��");
		System.out.println("����Ϊ����? " + list.isEmpty());
		// �������治�ܴ�Ż�������Ԫ�أ��������Զ�װ��
		list.add(10.0);
		System.out.println("������Ԫ�صĸ���Ϊ: " + list.size());
		// ɾ��Ԫ��
		list.remove(10.0);
		System.out.println("������Ԫ�صĸ���Ϊ: " + list.size());
		System.out.println("����'��һ��Ԫ��': " + list.contains("��һ��Ԫ��")); 
		List listB = new Vector();
		Student std = new Student("Tom", 10);
		for (int i = 0; i < 3; i++) {
			std.age = 10 + i;
			listB.add(std);
		}
		System.out.println(listB); // �Զ�����list�����toString������ArrayListд��Object��toString() 
	}
}
