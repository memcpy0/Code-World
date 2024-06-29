import java.io.*;
//使用文件输出字符流FileWriter和缓冲输出字符流BufferedWriter写入文本
public class BufferedWriterTest {
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.txt");
		try {
			FileWriter fw = new FileWriter(file); // 覆盖写
			BufferedWriter bw = new BufferedWriter(fw);
			bw.write("中文输出也可以！");
			bw.newLine();
			bw.write("换行建议用newLine方法！");
			bw.close(); 
		} catch (IOException e) {
			System.out.println("文件操作时出现异常！");
			e.printStackTrace();
		}
	}
}
