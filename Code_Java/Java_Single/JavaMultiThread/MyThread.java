// ͨ���̳�Thread���������߳���
// ����Thread�����࣬��д�����run������run���������߳���Ҫִ�е�����
// ����Thread�����ʵ�������������̶߳���
// ʹ���̶߳����start�����������߳�

class ThreadSon extends Thread {
	public void run() {
		while (true) { // currentThread��̬������ȡ��ǰ�������е��̶߳���
			System.out.println("�߳����У�" + Thread.currentThread().getName()); 
		}
	}
}

public class MyThread {
	public static void main(String[] args) {
		System.out.println("main�������У�");
		ThreadSon myThread1 = new ThreadSon();
		myThread1.setName("�߳�1");
		ThreadSon myThread2 = new ThreadSon();
		myThread2.setName("�߳�2");
		myThread1.start();
		myThread2.start();
		// ������ѭ��
		while (true) {
			System.out.println("���̣߳�");
		}
	}
}
 
