// 单例模式
// 创建类模式之一
class Singleton { 
	private static Singleton instance;
	private Singleton() { }
	public static Singleton getSingletonInstance() {
		if (instance == null) 
			instance = new Singleton();
		return instance;
	}
}

public class Single {
	public static void main(String[] args) {
		Singleton s1, s2;
		s1 = Singleton.getSingletonInstance();
		s2 = Singleton.getSingletonInstance();
		if (s1 == s2) System.out.println("相同！");
		else System.out.println("不同！");
	}
}
