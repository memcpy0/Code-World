import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.BufferedInputStream;
//ʹ���ļ������ֽ���FileInputStream�ͻ��������ֽ���BufferedInputStream�������ļ�
public class BufferedInputStreamTest { 
	public static void main(String[] args) {
		File file = new File("C:\\Users\\dell\\Desktop\\δ����1.cpp");
		try {
			// ���ļ�
			FileInputStream fis = new FileInputStream(file);
			BufferedInputStream bis = new BufferedInputStream(fis);
			System.out.println("�ɴ��ļ��ж�ȡ���ֽ���: " + bis.available());
			int i;
			// ��ȡ���ֽڱ��浽����i�У����i=-1˵�������ļ�β�����˳�
			while ((i = fis.read()) != -1) {
				System.out.print((char)i);
			}
			fis.close();
			bis.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
