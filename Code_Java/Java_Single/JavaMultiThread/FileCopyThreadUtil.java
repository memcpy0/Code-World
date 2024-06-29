import java.io.*;

class FileCopyThread extends Thread {
    private String source;
    private String target;
    public FileCopyThread(String source, String target) {
    	this.source = source;
    	this.target = target;
    }
    
    public void run() {
    	try {
    		System.out.println("复制文件" + source + "开始！");
    		BufferedInputStream bis = new BufferedInputStream(new FileInputStream(source));
    		BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(target));
    		int i;
    		while ((i = bis.read()) != -1) bos.write(i);
    		bis.close();
    		bos.close();
    		System.out.println("复制文件" + source + "完成！");
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    }
}
public class FileCopyThreadUtil {
	public static void main(String[] args) {
		// 启动5个文件复制线程
		for (int i = 1; i <= 5; i++) {
			String source = "D:/" + i + ".rar";
			String target = "D:/" + i + "copy.rar";
			FileCopyThread thread = new FileCopyThread(source, target);
			// 启动线程, 进行复制
			thread.start(); 
		}
	}
}
