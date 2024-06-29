import java.io.*;
import java.util.Date;
import java.text.SimpleDateFormat;

public class FileUsage {
	public static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
	public static int gagibytes = 1024 * 1024 * 1024;
	public static void main(String[] args) {
		File[] disks = File.listRoots(); // ��ȡ��ǰ�ļ�ϵͳ�����д��̷���
		System.out.println("ϵͳ���̷���״����");
		for (File i : disks) {
			System.out.println(i.getAbsolutePath() + "��\t�ܿռ�: " + i.getTotalSpace() / gagibytes + "G\tʣ��ռ�" 
					+ i.getFreeSpace() / gagibytes + "G\t��ʹ�ÿռ�: " + i.getUsableSpace() / gagibytes + "G.");
		}
		
		File c = new File("D:/");
		if (!c.exists()) {
			System.out.println("δ����D�̣�");
		}
		System.out.println("D�̸�Ŀ¼�ṹ:");
		File[] files = c.listFiles();
		for (File f : files) {
			if (f.isDirectory()) {
				System.out.print("[Ŀ¼]" + f.getName());
			} else if (f.isFile()) {
				System.out.print("[�ļ�]" + f.getName());
				System.out.print("\t ��С: " + f.length() / 1024 + "k"); // ��ȡ�ļ�����
				Date date = new Date(f.lastModified());
				System.out.print("\t �޸�����: " + sdf.format(date) + "\t");
				if (f.isHidden()) System.out.print("[����]");
				if (f.canRead()) System.out.print("[�ɶ�]");
				if (f.canWrite()) System.out.print("[��д]");
				if (f.canExecute()) System.out.print("[��ִ��]");
			}
			System.out.println();
		}
	}
	
}
