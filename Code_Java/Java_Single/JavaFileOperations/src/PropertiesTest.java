import java.io.*;
import java.util.Properties;

public class PropertiesTest {
	public static void main(String[] args) {
		File f = new File("C:/Users/dell/Desktop/test.properties");
		Properties prop = new Properties();
		prop.put("font-size", "14px"); // �����ֵ��
		prop.put("color", "��ɫ");
		try { // FileOutputStream/BufferedOutputStream, FileWriter/BufferedWriter 
			// prop.store(new FileOutputStream(f), "�����ļ�ע��"); // "The comments of Properties
			// FileOutputStream fos = new FileOutputStream(f); 
			// prop.store(new BufferedOutputStream(fos), "�����ļ�ע��");
			
			// prop.store(new FileWriter(f), "�����ļ�ע��");
			 FileWriter fw = new FileWriter(f);
			 prop.store(new BufferedWriter(fw), "�����ļ�ע��");
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("�����ļ�����!");
		}
		
		Properties prop2 = new Properties();
		try {
			prop2.load(new FileInputStream(f)); 
			String fontSize = prop.getProperty("font-size");  // ��ȡ�����ļ�
			String color = prop.getProperty("color");
			System.out.println(fontSize);
			System.out.println(color);
		} catch (FileNotFoundException e) {
			System.out.println("�����ļ�δ�ҵ�!");
		} catch (IOException e) {
		    System.out.println("�����ļ���ȡʧ��!");
		}
	}
}
