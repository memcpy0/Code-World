import java.io.*;
//ʹ���ļ�����ַ���FileWriter�ͻ�������ַ���BufferedWriterд���ı�
public class BufferedWriterTest {
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.txt");
		try {
			FileWriter fw = new FileWriter(file); // ����д
			BufferedWriter bw = new BufferedWriter(fw);
			bw.write("�������Ҳ���ԣ�");
			bw.newLine();
			bw.write("���н�����newLine������");
			bw.close(); 
		} catch (IOException e) {
			System.out.println("�ļ�����ʱ�����쳣��");
			e.printStackTrace();
		}
	}
}
