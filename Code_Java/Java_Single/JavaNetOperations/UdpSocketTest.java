import java.io.*;
import java.net.*;
// UDP��һ�ֲ��ɿ�������Э�飬ͨ�����˸�����һ��Socket��֮�䲻��Ҫ���ӣ�ֻ�Ƿ��ͺͽ�������
public class UdpSocketTest {
	public static void main(String[] args) {
		try {
			DatagramSocket udp = new DatagramSocket(8888); // ���Ͷ�, ռ��8888�˿�
			String s = "���, ���ѣ�";
			// ���캯���и������͸���������ݡ����ݳ��ȡ��Է��ĵ�ַ����Ϣ���ܶ˿�7777
			DatagramPacket data = new DatagramPacket(s.getBytes(), s.getBytes().length, InetAddress.getLocalHost(), 7777);
			udp.send(data); // ��������
			udp.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
