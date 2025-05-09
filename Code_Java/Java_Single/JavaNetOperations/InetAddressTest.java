import java.net.*;
import java.util.Arrays;

public class InetAddressTest {
	public static void main(String[] args) throws UnknownHostException {
		// 获取本地主机对应的InetAddress对象, 代表IP地址
		InetAddress localhost = InetAddress.getLocalHost(); 
		// 输出主机名字
		System.out.println(localhost.getHostName());    // LETHARGY
		// 输出主机的IP地址字符串
		System.out.println(localhost.getHostAddress()); // 169.254.32.50
		// 输出主机的IP地址全限定域名
		System.out.println(localhost.getCanonicalHostName()); // LETHARGY
		// 通过域名获取主机
		InetAddress sohuHost = InetAddress.getByName("www.sohu.com");
		System.out.println(sohuHost.getHostName()); // www.sohu.com
		System.out.println(sohuHost.getHostAddress()); // 111.48.63.44
		System.out.println(sohuHost.getHostName()); // www.sohu.com
		
		// 一个域名可以绑定多个IP地址
		InetAddress[] sohuHosts = InetAddress.getAllByName("www.sohu.com");
		System.out.println(Arrays.toString(sohuHosts));
		// 直接使用IP地址获取主机
		InetAddress remoteHost = InetAddress.getByName("192.168.1.200");
		System.out.println(remoteHost.getHostName()); // 192.168.1.200
	}
}
