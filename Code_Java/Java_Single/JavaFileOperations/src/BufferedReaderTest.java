import java.io.BufferedReader;
import java.io.File; 
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
// ʹ���ļ������ַ���FileReader�ͻ��������ַ���BufferedReader��ȡ�ı�
public class BufferedReaderTest {
	public static void main(String[] args) {
	File file = new File("C:\\Users\\dell\\Desktop\\δ����1.cpp");
		try {
			// ���ļ�
			FileReader fr = new FileReader(file);
			BufferedReader br = new BufferedReader(fr);  
            // int i;
            // while ((i = br.read()) != -1) {
			// System.out.print((char)i);
            // }
			String s; 
			while ((s = br.readLine()) != null) {
				System.out.println(s);
			}
			fr.close();
			br.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
