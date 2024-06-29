import java.io.*;
// 对象的序列化: 将内存中的Java对象转换为字节序列，写入IO流中，输出到磁盘文件或者网络
// 对象反序列化: 将字节序列恢复成Java对象
// 某个类的对象如要支持序列化和反序列化，需要实现(implements)Serializable接口/Externalizable接口，表明该类是可以被序列化的
// 然后使用java.io.ObjectOutputStream流来输出/java.io.ObjectInputStream流输入
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
			oos.writeObject(s1); // 将指定的对象写入到输出流
			oos.writeObject(s2);
			oos.writeObject(new Student("neo", 44));
			oos.close();
			fos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
