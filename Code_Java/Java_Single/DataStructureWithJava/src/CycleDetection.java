import java.util.ArrayList;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//���ͼ���Ƿ��л� ͼ�п����ж����ͨ����, һ����ͨ�����л���ͼ�л�
//�л�: �����Ի�, ������ͨ����, һ�����ڱ�Ȼ��ͨ����һ����ͨ������һ���л�	
//�Ƿ��ǻ�, ��DFS�ĳ������Ƿ�ͽ��������, ��visited��ֹ�ظ�����, ��parent��ֹ�����Լ����Լ��Ļ�

public class CycleDetection {
	
	private Graph G;
	private boolean[] visited; 
	
	private boolean hasCycle = false;
	
	public CycleDetection(Graph G) { //���캯����ִ��DFS, �û�ͨ��order()�����㷨�߼�
		this.G = G;
		visited = new boolean[G.V()];
		
		for (int i = 0; i < G.V(); ++i) {
			if (visited[i] == false) {
				if (dfs(i, i)) { //����л�, �Ϳ���break, ��ǰ��ֹ
					hasCycle = true;
					break;
				}
			}
		} 
	}
 
	
	private boolean dfs(int v, int parent) { //�Ӷ���v��ʼ, �ж�ͼ���Ƿ��л�
		visited[v] = true; 
		
		for (int w : G.adj(v)) //����v���ڽӵ�
			if (visited[w] == false) { //���v���ڽӵ�û�б�����
				if (dfs(w, v)) return true; //�л���ֱ�ӷ���
			}
			else if (w != parent) //Ҫ���ʵ���һ������Ƿ�Ϊv��ǰһ�����, ���ǵĻ����л�, ����Ϊ�˷�ֹ����
				return true; 
		return false; //û��������
	}  
	
	public boolean hasCycle() {
		return hasCycle;
	}
	 
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		CycleDetection cd = new CycleDetection(g);
		System.out.println(cd.hasCycle()); //true�л�
		
		Graph g2 = new Graph("t.txt");
		System.out.println(g2);
		CycleDetection cd2 = new CycleDetection(g2);
		System.out.println(cd2.hasCycle); //������ͨ����, ����û�л�
	}

}