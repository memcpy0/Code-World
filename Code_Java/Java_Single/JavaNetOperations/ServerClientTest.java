import java.io.*;
import java.net.*;
import java.util.*;
// ��֤�ͻ���ע���������
public class ServerClientTest {
	public final static String STATE_OK = "1";
	public final static String STATE_USER_NOT_EXIST = "1";
	public final static String STATE_CODE_WRONG = "1";
	public static Map<String, String> users;
	static { // ��̬������ʼ����
		users = new HashMap<String, String>();
		users.put("Tom", "400-0000");
		users.put("Jerry", "500-0000");
		users.put("ZShan", "600-0000");
		users.put("Lishi", "700-0000");
		users.put("wangw", "800-0000");
	}
	public static void main(String[] args) {
		ServerSocket server;
		try {
			server = new ServerSocket(3333);
		} catch (IOException e) {
			System.out.println("�˿ڼ�������");
			return;
		}
		while (true) {
			try {
				System.out.println("�ȴ��ͻ�������!");
				Socket client = server.accept();
				BufferedReader br = new BufferedReader(new InputStreamReader(client.getInputStream()));
			}
		}
	}
}
