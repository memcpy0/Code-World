import java.util.ArrayList;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//��ͼ����ͨ���� Y
//��������Ƿ�ɴ� Y
//��������һ��·�� ������ͬһ����ͨ��������ζ���������·�� ��Դ·��
//���ͼ���Ƿ��л�
//����ͼ���
//Ѱ��ͼ�е���
//Ѱ��ͼ�еĸ��
//���ܶ���·��
//��������
//������ȱ����ķǵݹ�ʵ��
//ͼ���ڽӾ����������ȱ���
public class GraphDFS {
	
	private Graph G;
	private boolean[] visited;
	private ArrayList<Integer> pre = new ArrayList<>(); //������ȱ����������
	private ArrayList<Integer> post = new ArrayList<>(); //������ȱ����������
	
	public GraphDFS(Graph G) { //���캯����ִ��DFS, �û�ͨ��order()�����㷨�߼�
		this.G = G;
		visited = new boolean[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			if (visited[i] == false)
				dfs(i);
	}
 
	
	private void dfs(int v) {
		visited[v] = true;
		pre.add(v); 
		for (int w : G.adj(v)) 
			if (visited[w] == false) //���v���ڽӵ�û�б�����
				dfs(w);
		post.add(v);
	} 
	
	public Iterable<Integer> pre() {
		return pre;
	}
	public Iterable<Integer> post() {
		return post;
	}
	 
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		GraphDFS graphDFS = new GraphDFS(g);
		System.out.println(graphDFS.pre());
		System.out.println(graphDFS.post());
	}

}
