// ʵ��Runnable�ӿ�
// ��дһ���࣬ʵ��Runnable�ӿ�(�еĳ��󷽷�run)
// ���߳�Ҫ���еĴ���(�߳���)�ŵ�run������
// Ȼ�󴴽�Runnableʵ�����ʵ��
// �Դ�ʵ����ΪThread��target������Thread����
// ��Thread��������������̶߳���

class MyRunnable implements Runnable {
	public void run() {
		while (true) {
			System.out.println("�߳�: " + 
		    Thread.currentThread().getName() + "����");
		}
	}
}

public class chapter13_05 {
	public static void main(String[] args) {
		MyRunnable target = new MyRunnable();
		Thread th1 = new Thread(target);
		th1.setName("�߳�1");
		Thread th2 = new Thread(target);
		th2.setName("�߳�2");
		// �����߳�
		th1.start();
		th2.start();
	}
}
