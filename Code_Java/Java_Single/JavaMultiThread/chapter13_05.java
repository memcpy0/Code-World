// 实现Runnable接口
// 编写一个类，实现Runnable接口(中的抽象方法run)
// 将线程要运行的代码(线程体)放到run方法中
// 然后创建Runnable实现类的实例
// 以此实例作为Thread的target来创建Thread对象
// 该Thread对象才是真正的线程对象

class MyRunnable implements Runnable {
	public void run() {
		while (true) {
			System.out.println("线程: " + 
		    Thread.currentThread().getName() + "运行");
		}
	}
}

public class chapter13_05 {
	public static void main(String[] args) {
		MyRunnable target = new MyRunnable();
		Thread th1 = new Thread(target);
		th1.setName("线程1");
		Thread th2 = new Thread(target);
		th2.setName("线程2");
		// 启动线程
		th1.start();
		th2.start();
	}
}
