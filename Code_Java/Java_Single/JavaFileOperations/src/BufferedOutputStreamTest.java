import java.io.*;
// 使用文件输出字节流FileOutputStream和缓冲输出字节流BufferedOutputStream来输出
public class BufferedOutputStreamTest {  
	public static void main(String[] args) {
		File file = new File("C:/Users/dell/Desktop/test.txt");
		try {
			FileOutputStream fos = new FileOutputStream(file); // 覆盖写
			BufferedOutputStream bos = new BufferedOutputStream(fos);
			byte[] b = {65, 66, 67, 68, 69};
			bos.write(b);  // A B C的ASCII字节码
			bos.flush(); // flush用于强制清空缓存
			bos.close(); // 自动调用flush方法
		} catch (IOException e) {
			System.out.println("文件操作时出现异常！");
			e.printStackTrace();
		}
	}  
}
