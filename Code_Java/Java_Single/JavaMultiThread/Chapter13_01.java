
public class Chapter13_01 {

	public static void main(String[] args) {
		for (int i = 0; i < 10; i++) {
			System.out.println("*");
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				System.out.println("ÐÝÃß±»´ò¶Ï£¡");
			}
		}

	}

}
