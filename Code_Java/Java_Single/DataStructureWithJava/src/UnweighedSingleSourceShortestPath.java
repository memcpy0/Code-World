import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Collections;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//(��������Ȩͼ) ��Դ���·������ BFS
//���Ը���, n��UnweighedSingleSourceShortestPath������Ȩͼ���е�����·����������
//�����Դ�㵽Ŀ����Ƿ�ɴ�
//�����������·�� ������ͬһ����ͨ��������ζ���������·��
//www.javaxxz.com_*YG%D67J(4FU
public class UnweighedSingleSourceShortestPath {
	
	private Graph G;
	private int s; //��Դ·����Դ�� ��s�����ҵ�ǰ�����пɴﶥ���·��
	private boolean[] visited; //��Ȼ, ��������visited, ��pre/dis�Ϳ��Ա����Ƿ�����˸ö���
	private int[] pre; //��¼BFSʱ���������ǰһ������
	private int[] dis; //��¼BFSʱ��Դ�㵽�ɴ����������ľ���
	
	public UnweighedSingleSourceShortestPath(Graph G, int s) { //���캯����ִ��DFS 
		G.validateVertex(s);
		
		this.G = G;
		this.s = s;
		visited = new boolean[G.V()];
		pre = new int[G.V()];
		dis = new int[G.V()];
		
		for (int i = 0; i < G.V(); ++i) {
			pre[i] = -1; //-1ָ���Ǵ�Դ�㲻�ɴﵽ����i, ��Ȼ��visited[i]����
			dis[i] = -1;
		}
		bfs(s); //ֻ��Ҫ����Դ�����ڵ���һ����ͨ����
	}
 
	
	private void bfs(int s) { //���
		Queue<Integer> queue = new LinkedList<>(); 
		queue.add(s);
		visited[s] = true;
		pre[s] = s; //��¼���
		dis[s] = 0; //��㵽�Լ��ľ���Ϊ0
		
		while (!queue.isEmpty()) {
			int v = queue.remove();
			
			for (int w : G.adj(v)) {
				if (visited[w] == false) {
					queue.add(w);
					visited[w] = true;
					pre[w] = v; //��¼����w��ǰһ������, ������·��
					dis[w] = dis[v] + 1;
				}
			}
		}
	}  
	
	public boolean isConnectedTo(int t) { //��Դ���Ƿ�ɴ�t
		G.validateVertex(t); //����Ƿ�Ϸ�
		return visited[t]; //pre[t] == -1 //dis[t] == -1
	}
	
	public Iterable<Integer> path(int t) { //�õ���Դ�㵽t��һ��·��
		ArrayList<Integer> res = new ArrayList<>();
		if (!isConnectedTo(t)) return res;
		
		int cur = t;
		while (cur != s) {
			res.add(cur);
			cur = pre[cur];
		}
		res.add(s);
		Collections.reverse(res);
		return res;
	}
	
	public int dis(int t) { //����Դ�㵽����t�����·���ľ���
		G.validateVertex(t);
		return dis[t];
	}
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		UnweighedSingleSourceShortestPath usspath = new UnweighedSingleSourceShortestPath(g, 0); //Դ��Ϊ0�ĵ�Դ���·��
		System.out.print("0 -> 6 : ");
		System.out.println(usspath.path(6));  
		System.out.println(usspath.dis(6));
	}

}