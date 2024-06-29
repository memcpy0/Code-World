import java.io.*;
import java.net.*;
// UDP：一种不可靠的网络协议，通信两端各建立一个Socket，之间不需要连接，只是发送和接受数据
public class UdpSocketTest {
	public static void main(String[] args) {
		try {
			DatagramSocket udp = new DatagramSocket(8888); // 发送端, 占用8888端口
			String s = "你好, 朋友！";
			// 构造函数中给出发送给对象的数据、数据长度、对方的地址和信息接受端口7777
			DatagramPacket data = new DatagramPacket(s.getBytes(), s.getBytes().length, InetAddress.getLocalHost(), 7777);
			udp.send(data); // 发送数据
			udp.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
