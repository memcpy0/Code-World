import java.io.*;
// ʹ���ļ�����ֽ���FileOutputStream�ͻ�������ֽ���BufferedOutputStream�����
public class BufferedOutputStreamTest {  
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.txt");
		try {
			FileOutputStream fos = new FileOutputStream(file); // ����д
			BufferedOutputStream bos = new BufferedOutputStream(fos);
			byte[] b = {65, 66, 67, 68, 69};
			bos.write(b);  // A B C��ASCII�ֽ���
			bos.flush(); // flush����ǿ����ջ���
			bos.close(); // �Զ�����flush����
		} catch (IOException e) {
			System.out.println("�ļ�����ʱ�����쳣��");
			e.printStackTrace();
		}
	}  
}
