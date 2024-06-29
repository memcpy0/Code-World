import java.io.*;
import java.util.Date;
import java.text.SimpleDateFormat;

public class FileUsage {
	public static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
	public static int gagibytes = 1024 * 1024 * 1024;
	public static void main(String[] args) {
		File[] disks = File.listRoots(); // 获取当前文件系统的所有磁盘分区
		System.out.println("系统磁盘分区状况：");
		for (File i : disks) {
			System.out.println(i.getAbsolutePath() + "盘\t总空间: " + i.getTotalSpace() / gagibytes + "G\t剩余空间" 
					+ i.getFreeSpace() / gagibytes + "G\t可使用空间: " + i.getUsableSpace() / gagibytes + "G.");
		}
		
		File c = new File("D:/");
		if (!c.exists()) {
			System.out.println("未发现D盘！");
		}
		System.out.println("D盘根目录结构:");
		File[] files = c.listFiles();
		for (File f : files) {
			if (f.isDirectory()) {
				System.out.print("[目录]" + f.getName());
			} else if (f.isFile()) {
				System.out.print("[文件]" + f.getName());
				System.out.print("\t 大小: " + f.length() / 1024 + "k"); // 获取文件长度
				Date date = new Date(f.lastModified());
				System.out.print("\t 修改日期: " + sdf.format(date) + "\t");
				if (f.isHidden()) System.out.print("[隐藏]");
				if (f.canRead()) System.out.print("[可读]");
				if (f.canWrite()) System.out.print("[可写]");
				if (f.canExecute()) System.out.print("[可执行]");
			}
			System.out.println();
		}
	}
	
}
