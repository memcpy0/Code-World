import java.io.*;
import java.net.*;

public class ServerSocketMoreConnectTest {
	public static void main(String[] args) throws IOException {
		ServerSocket server = null;
		server = new ServerSocket(3333);
		int count = 0; 
		while (true) {
			System.out.println("�ȴ��ͻ�������...");
			Socket client = server.accept();
			System.out.println("�пͻ����ӣ�ͨ������ͻ��˷�������...");
			count = count + 1;
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(client.getOutputStream()));
			writer.write("���Ǳ����ĵ�" + count + "���û�!");
			writer.write("��ӭ�´ι���...");
			writer.flush();
			client.close(); 
		}
	}
}