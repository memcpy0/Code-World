import java.util.*;
// 集合工具类的使用
public class ConnectionsUtilityTest {
	public static void main(String[] args) {
		List<String> list = Arrays.asList("张三", "李四", "王五"); // Arrays.asList
//		for (String str : list) {
//			System.out.println(str);
//		}
		int[] arrays = {234, 43, 5454, 6, 56, 88}; 
		System.out.println(Arrays.toString(arrays)); // Arrays.toString
		Arrays.sort(arrays);                         // Arrays.sort
		System.out.println(Arrays.toString(arrays));
		System.out.println(Arrays.binarySearch(arrays, 234)); // Arrays.binarySearch
	}
}
