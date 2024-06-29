import java.io.*;
import java.net.*;

public class ClientImformationInputTest {
	public static void main(String[] args) throws IOException {
		ServerSocket server = null;
		server = new ServerSocket(3333);
		Socket client = null;
		client = server.accept();
		BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
		System.out.println("等待客户端连接...");
		String s = null;
		while ((s = reader.readLine()) != null) System.out.println(s);
		reader.close();
		client.close();
		server.close();
	}
}
