import java.io.BufferedReader;
import java.io.File; 
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
// 使用文件输入字符流FileReader和缓冲输入字符流BufferedReader读取文本
public class BufferedReaderTest {
	public static void main(String[] args) {
	File file = new File("C:\\Users\\dell\\Desktop\\未命名1.cpp");
		try {
			// 打开文件
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
