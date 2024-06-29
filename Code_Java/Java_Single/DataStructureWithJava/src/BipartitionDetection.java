import java.util.ArrayList;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//����ͼ��� 

//ͼ���ڽӾ����������ȱ���
public class BipartitionDetection {
	
	private Graph G;
	private int[] colors; //��ɫ�ж��Ƿ���ʹ�; ͬʱ��DFS����Ⱦɫ���ж��Ƿ��Ƕ���ͼ
	private boolean isBipart = true; //�Ƿ��Ƕ���ͼ
	
	public BipartitionDetection(Graph G) { //���캯����ִ��DFS, �û�ͨ��order()�����㷨�߼�
		this.G = G;
		colors = new int[G.V()]; //0����ɫ, -1����ɫ, 1����ɫ
		
		for (int i = 0; i < G.V(); ++i) { //����ÿһ����ͨ����, ���ÿ����ͨ�������Ƕ��ֵ�, ��ô����ͼ���Ƕ��ֵ�
			if (colors[i] == 0) {
				if (dfs(i, -1) == false) { // Ϊÿ����ͨ�����ĳ�ʼ��ͳһȾɫ��ɫ
					isBipart = false; //��������ͨ�������Ƕ��ֵ�, ����ͼ�Ͳ��Ƕ��ֵ�
					break;
				}
			}
		}
		
	}
  
	private boolean dfs(int v, int color) {
		colors[v] = color;
		
		for (int w : G.adj(v)) {
			if (colors[w] == 0) { //���v���ڽӵ�wû�б�����
				if (dfs(w, -color) == false) return false; //���������ɫ, �¸�������ɫ; ��������ɫ
			} else if (colors[v] == colors[w]) { //���v���ڽӵ�w���ʹ���, �Ҷ�����ͬ����ɫ
				return false; //����ì��, ֱ�ӷ���false
			}
		} 
		return true; //û�еط�ì��
	} 
	 
	public boolean isBipart() { //�Ƿ��Ƕ���ͼ
		return isBipart;
	}
	public ArrayList<Integer>[] bipart() { //�����������ֵĲ���
		ArrayList<Integer>[] res = new ArrayList[2];
		if (isBipart == false) return res;
		
		res[0] = new ArrayList<>();
		res[1] = new ArrayList<>();
		
		for (int i = 0; i < colors.length; ++i) {
			if (colors[i] == -1) res[0].add(i);
			else res[1].add(i);
		}
		return res;
	}
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		BipartitionDetection bd = new BipartitionDetection(g);
		System.out.println(bd.isBipart());
		ArrayList<Integer>[] arr = bd.bipart();
		System.out.print(arr[0] + " ");
		System.out.println(arr[1]);
		
		Graph c = new Graph("completeGraph.txt");
		BipartitionDetection bd2 = new BipartitionDetection(c);
		System.out.println(bd2.isBipart());  
		ArrayList<Integer>[] arr2 = bd2.bipart();
		System.out.print(arr2[0] + " ");
		System.out.println(arr2[1]);
	}

}
