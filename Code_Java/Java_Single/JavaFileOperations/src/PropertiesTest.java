import java.io.*;
import java.util.Properties;

public class PropertiesTest {
	public static void main(String[] args) {
		File f = new File("C:/Users/dell/Desktop/test.properties");
		Properties prop = new Properties();
		prop.put("font-size", "14px"); // 存入键值对
		prop.put("color", "红色");
		try { // FileOutputStream/BufferedOutputStream, FileWriter/BufferedWriter 
			// prop.store(new FileOutputStream(f), "配置文件注释"); // "The comments of Properties
			// FileOutputStream fos = new FileOutputStream(f); 
			// prop.store(new BufferedOutputStream(fos), "配置文件注释");
			
			// prop.store(new FileWriter(f), "配置文件注释");
			 FileWriter fw = new FileWriter(f);
			 prop.store(new BufferedWriter(fw), "配置文件注释");
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("配置文件出错!");
		}
		
		Properties prop2 = new Properties();
		try {
			prop2.load(new FileInputStream(f)); 
			String fontSize = prop.getProperty("font-size");  // 读取配置文件
			String color = prop.getProperty("color");
			System.out.println(fontSize);
			System.out.println(color);
		} catch (FileNotFoundException e) {
			System.out.println("配置文件未找到!");
		} catch (IOException e) {
		    System.out.println("配置文件读取失败!");
		}
	}
}
