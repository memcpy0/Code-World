import java.io.*;
import java.net.*;

public class UdpSocketReceiverTest {
    public static void main(String[] args) { 
    	try {
    		DatagramSocket udp = new DatagramSocket(7777);
    		// ��װ���ܵ����ݣ�byte���顢���ܳ��ȡ����ͷ���ַ�����ͷ��˿ں�
			DatagramPacket data = new DatagramPacket(new byte[100], 100, InetAddress.getLocalHost(), 8888);
			// receive�������������У�һֱ�ȴ����Ͷ˷�������
			udp.receive(data);
			// ����String�Ĺ��캯����byte����ת��Ϊ�ַ���
			String s = new String(data.getData(), 0, data.getLength());
			System.out.println("�������ݣ�" + s);
			udp.close();
		} catch (UnknownHostException e) { 
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
    	
    }
}
