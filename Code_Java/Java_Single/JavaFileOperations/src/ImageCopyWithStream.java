import java.io.File;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
// �ۺ�ʹ���ļ��������������ʵ��һ��ͼƬ�ļ��ĸ���
public class ImageCopyWithStream {

	public static void main(String[] args) {
		File imageFile = new File("C:/Users/dell/Desktop/Snipaste1.png");
		File imageTo   = new File("C:/Users/dell/Desktop/Snipaste1����.png");
		try {
			if (!imageFile.exists()) {
				System.out.println("Դ�ļ�������!");
				return;
			}
			if (imageTo.exists()) {
				System.out.println("Ŀ���ļ��Ѵ���!");
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
			bos.close(); // ����close, ��Ȼ�����ֽ����ڻ�����û��д��
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
