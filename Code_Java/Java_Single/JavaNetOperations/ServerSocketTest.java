import java.io.*;
import java.net.*;

public class ServerSocketTest {
	public static void main(String[] args) throws IOException {
		// ����ServerSocket����, ������3333�˿�
		ServerSocket server = new ServerSocket(3333);
		System.out.println("�ȴ��ͻ�������...");
		// �ȴ��ͻ������ӣ����д������Ĵ�����ʱ�����У�ֱ���пͻ�������
		Socket client = server.accept();
		System.out.println("�пͻ����ӣ�ͨ������ͻ��˷�������...");
		BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(client.getOutputStream()));
		writer.write("���Ƿ���������������. ");
		writer.flush();
		client.close();
		server.close();
	}
}