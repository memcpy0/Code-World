import java.io.*;
// ʹ���ļ������ֽ���FileInputStream�������ļ�
public class FileInputStreamTest {
	public static void main(String[] args) {
		File file = new File("C:\\Users\\dell\\Desktop\\δ����1.cpp");
		try {
			// ���ļ�
			FileInputStream fis = new FileInputStream(file);
			System.out.println("�ɴ��ļ��ж�ȡ���ֽ���: " + fis.available());
			int i;
			// ��ȡ���ֽڱ��浽����i�У����i=-1˵�������ļ�β�����˳�
			while ((i = fis.read()) != -1) {
				System.out.print((char)i);
			}
			fis.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
