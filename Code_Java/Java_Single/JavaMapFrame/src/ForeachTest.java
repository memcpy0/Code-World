import java.util.List;
import java.util.ArrayList;

public class ForeachTest {

	public static void main(String[] args) { 
		Integer[] array = {10, 45, 78, 36, 1, 53};
		// ��������е���
		for (Integer i : array) {
			System.out.print(" " + i);
		}
		System.out.println();
		// �Լ��Ͻ��е�����ʹ�ø��ӿڲ�����������
		// ���ϱ�������ʹ�÷���ȷ��������Ԫ�����ͣ�������������
		List<String> list = new ArrayList<String>();
		list.add("A");
		list.add("B");
		list.add("C");
		for (String s : list) {
			System.out.print(" " + s);
		}
	}

}
