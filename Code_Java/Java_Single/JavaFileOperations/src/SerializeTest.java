import java.io.*;
// ��������л�: ���ڴ��е�Java����ת��Ϊ�ֽ����У�д��IO���У�����������ļ���������
// �������л�: ���ֽ����лָ���Java����
// ĳ����Ķ�����Ҫ֧�����л��ͷ����л�����Ҫʵ��(implements)Serializable�ӿ�/Externalizable�ӿڣ����������ǿ��Ա����л���
// Ȼ��ʹ��java.io.ObjectOutputStream�������/java.io.ObjectInputStream������
class Student implements Serializable {
	public String name;
	public int age;
	public Student(String name, int age) {
		this.name = name;
		this.age  = age;
	}
	public String toString() {
		return name + ", " + age;
	}
}

public class SerializeTest {
	public static void main(String[] args) {
		Student s1 = new Student("Tom", 20);
		Student s2 = new Student("Jerry", 23);
		try {
			FileOutputStream fos = new FileOutputStream("C:/Users/dell/Desktop/test.dat");
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(s1); // ��ָ���Ķ���д�뵽�����
			oos.writeObject(s2);
			oos.writeObject(new Student("neo", 44));
			oos.close();
			fos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
