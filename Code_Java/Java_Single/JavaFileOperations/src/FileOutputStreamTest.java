import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;
// ʹ���ļ�����ֽ���FileOutputStream������ļ�
public class FileOutputStreamTest {
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.txt");
		try {
			FileOutputStream fos = new FileOutputStream(file); // ����д
			byte[] b = {65, 66, 67};
			fos.write(b);  // A B C��ASCII�ֽ���
			fos.close(); 
		} catch (IOException e) {
			System.out.println("�ļ�����ʱ�����쳣��");
			e.printStackTrace();
		}
	}
}
