import java.util.ArrayList;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//��ͼ����ͨ��������Connected Component
//�����������Ƿ���ͨ
//��ͼ�еĲ�ͬ����ͨ����������
public class CC {
	
	private Graph G;
	private int[] visited; 
	private int cccnt = 0; //��ͨ��������, ͬʱ��Ϊ���ֲ�ͬ��ͨ������id
	
	private void dfs(int v) {
		visited[v] = cccnt; 
		for (int w : G.adj(v)) 
			if (visited[w] == -1) //���v���ڽӵ�û�б�����
				dfs(w); 
	} 
	public CC(Graph G) { //���캯����ִ��DFS, �û�ͨ��order()�����㷨�߼�
		this.G = G;
		visited = new int[G.V()];
		for (int i = 0; i < visited.length; ++i) 
			visited[i] = -1;
		
		for (int i = 0; i < G.V(); ++i) {
			if (visited[i] == -1) { //visited[i]Ϊ-1, û�з���
				dfs(i);
				++cccnt;
			}
		}
	}
 
	public boolean isConnected(int v, int w) { //�ж����������Ƿ���ͨ
		G.validateVertex(v);
		G.validateVertex(w);
		return visited[v] == visited[w]; //�����������ڵ���ͨ������ID��ͬ
	}

	 
	public int count() { //��ͨ�����ĸ���
//		for (int e : visited) 
//			System.out.println(e + " ");
//		System.out.println();
		return cccnt;
	}
	
	public ArrayList<Integer>[] components() { //�������е���ͨ����
		ArrayList<Integer>[] res = new ArrayList[cccnt];
		
		for (int i = 0; i < cccnt; ++i) 
			res[i] = new ArrayList<>();  //�ֱ𴴽�
		
		for (int v = 0; v < G.V(); ++v) 
			res[visited[v]].add(v);      //������ӵ�visited�����Ķ�Ӧ����ͨ������
		
		return res;
	}
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		System.out.println(g.toString());
		CC cc = new CC(g);
		System.out.println(cc.count());  //��ӡ��ͨ�����ĸ���
		System.out.println(cc.isConnected(0, 4)); //������������Ƿ���ͨ
		
		ArrayList<Integer>[] comp = cc.components();
		
		for (int ccid = 0; ccid < comp.length; ++ccid) {
			System.out.print(ccid + " : "); 
			for (int w : comp[ccid])  
				System.out.print(w + " ");
			System.out.println();
		}
	}

}