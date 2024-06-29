import java.io.*;
import java.net.*;

public class ClientSocketTest {
	public static void main(String[] args) {
		try {
			Socket socket = new Socket("127.0.0.1", 3333);
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
			writer.write("服务器你好啊！");
			writer.newLine();
			writer.write("我是Java程序编写的客户端！");
			writer.newLine();
			writer.write("这里是ClientSocket!");
			writer.flush();
			socket.close(); 
		} catch (UnknownHostException e) {
			System.out.println("无法连接到服务器！");
		} catch (IOException e) {
			System.out.println("遇到IO错误！");
		}
	}
}
