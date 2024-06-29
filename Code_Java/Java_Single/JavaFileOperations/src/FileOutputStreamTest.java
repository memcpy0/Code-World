import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;
// 使用文件输出字节流FileOutputStream来输出文件
public class FileOutputStreamTest {
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.txt");
		try {
			FileOutputStream fos = new FileOutputStream(file); // 覆盖写
			byte[] b = {65, 66, 67};
			fos.write(b);  // A B C的ASCII字节码
			fos.close(); 
		} catch (IOException e) {
			System.out.println("文件操作时出现异常！");
			e.printStackTrace();
		}
	}
}
