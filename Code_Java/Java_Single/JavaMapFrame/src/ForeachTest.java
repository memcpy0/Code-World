import java.util.List;
import java.util.ArrayList;

public class ForeachTest {

	public static void main(String[] args) { 
		Integer[] array = {10, 45, 78, 36, 1, 53};
		// 对数组进行迭代
		for (Integer i : array) {
			System.out.print(" " + i);
		}
		System.out.println();
		// 对集合进行迭代；使用父接口操作子类类型
		// 集合必须事先使用泛型确定集合中元素类型，否则不能这样做
		List<String> list = new ArrayList<String>();
		list.add("A");
		list.add("B");
		list.add("C");
		for (String s : list) {
			System.out.print(" " + s);
		}
	}

}
