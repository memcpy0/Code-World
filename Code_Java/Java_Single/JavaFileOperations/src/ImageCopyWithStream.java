import java.io.File;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
// 综合使用文件输入流和输出流实现一张图片文件的复制
public class ImageCopyWithStream {

	public static void main(String[] args) {
		File imageFile = new File("C:/Users/dell/Desktop/Snipaste1.png");
		File imageTo   = new File("C:/Users/dell/Desktop/Snipaste1副本.png");
		try {
			if (!imageFile.exists()) {
				System.out.println("源文件不存在!");
				return;
			}
			if (imageTo.exists()) {
				System.out.println("目标文件已存在!");
				return;
			}
			FileInputStream fis = new FileInputStream(imageFile);
			BufferedInputStream bis = new BufferedInputStream(fis);
			
			FileOutputStream fos = new FileOutputStream(imageTo);
			BufferedOutputStream bos = new BufferedOutputStream(fos);
			
			int i;
			while ((i = bis.read()) != -1) {
				bos.write(i);
			} 
			bis.close();  
			bos.flush();
			bos.close(); // 必须close, 不然会有字节留在缓冲区没有写入
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
