import java.net.*;
import java.util.Arrays;

public class InetAddressTest {
	public static void main(String[] args) throws UnknownHostException {
		// ��ȡ����������Ӧ��InetAddress����, ����IP��ַ
		InetAddress localhost = InetAddress.getLocalHost(); 
		// �����������
		System.out.println(localhost.getHostName());    // LETHARGY
		// ���������IP��ַ�ַ���
		System.out.println(localhost.getHostAddress()); // 169.254.32.50
		// ���������IP��ַȫ�޶�����
		System.out.println(localhost.getCanonicalHostName()); // LETHARGY
		// ͨ��������ȡ����
		InetAddress sohuHost = InetAddress.getByName("www.sohu.com");
		System.out.println(sohuHost.getHostName()); // www.sohu.com
		System.out.println(sohuHost.getHostAddress()); // 111.48.63.44
		System.out.println(sohuHost.getHostName()); // www.sohu.com
		
		// һ���������԰󶨶��IP��ַ
		InetAddress[] sohuHosts = InetAddress.getAllByName("www.sohu.com");
		System.out.println(Arrays.toString(sohuHosts));
		// ֱ��ʹ��IP��ַ��ȡ����
		InetAddress remoteHost = InetAddress.getByName("192.168.1.200");
		System.out.println(remoteHost.getHostName()); // 192.168.1.200
	}
}
