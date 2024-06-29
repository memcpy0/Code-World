import java.io.*;
import java.net.*;

public class UdpSocketReceiverTest {
    public static void main(String[] args) { 
    	try {
    		DatagramSocket udp = new DatagramSocket(7777);
    		// 包装接受的数据，byte数组、接受长度、发送方地址、发送方端口号
			DatagramPacket data = new DatagramPacket(new byte[100], 100, InetAddress.getLocalHost(), 8888);
			// receive会阻塞程序运行，一直等待发送端发送数据
			udp.receive(data);
			// 利用String的构造函数将byte数组转换为字符串
			String s = new String(data.getData(), 0, data.getLength());
			System.out.println("接收内容：" + s);
			udp.close();
		} catch (UnknownHostException e) { 
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
    	
    }
}
