import java.io.*;
import java.net.*;

public class ClientSocketTest {
	public static void main(String[] args) {
		try {
			Socket socket = new Socket("127.0.0.1", 3333);
			BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
			writer.write("��������ð���");
			writer.newLine();
			writer.write("����Java�����д�Ŀͻ��ˣ�");
			writer.newLine();
			writer.write("������ClientSocket!");
			writer.flush();
			socket.close(); 
		} catch (UnknownHostException e) {
			System.out.println("�޷����ӵ���������");
		} catch (IOException e) {
			System.out.println("����IO����");
		}
	}
}
