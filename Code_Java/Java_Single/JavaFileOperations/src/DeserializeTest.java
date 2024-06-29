import java.io.*;

public class DeserializeTest {
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.dat");
		try {
			FileInputStream fis = new FileInputStream(file);
			ObjectInputStream ois = new ObjectInputStream(fis);
			Student s1 = (Student)ois.readObject(); // 将对象从输入流中读出
			Student s2 = (Student)ois.readObject();
			Student s3 = (Student)ois.readObject();
			ois.close();
			System.out.println(s1);
			System.out.println(s2);
			System.out.println(s3);
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("操作失误!");
		}
	}
}
