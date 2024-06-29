import java.io.*;
// 使用文件输入字节流FileInputStream来输入文件
public class FileInputStreamTest {
	public static void main(String[] args) {
		File file = new File("C:\\Users\\dell\\Desktop\\未命名1.cpp");
		try {
			// 打开文件
			FileInputStream fis = new FileInputStream(file);
			System.out.println("可从文件中读取的字节数: " + fis.available());
			int i;
			// 读取的字节保存到变量i中，如果i=-1说明到达文件尾部，退出
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
