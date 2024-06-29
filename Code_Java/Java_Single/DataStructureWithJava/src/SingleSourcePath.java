import java.util.ArrayList;
import java.util.Collections;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//��Դ·������
//���Ը���, n��SingleSourcePath�������е��·����������
//�����Դ�㵽Ŀ����Ƿ�ɴ�
//��������һ��·�� ������ͬһ����ͨ��������ζ���������·�� ��Դ·��
 
public class SingleSourcePath {
	
	private Graph G;
	private int s; //��Դ·����Դ�� ��s�����ҵ�ǰ�����пɴﶥ���·��
	private boolean[] visited; //��Ȼ, ��������visited, ��pre�Ϳ��Ա���Ƿ�����˸ö���
	private int[] pre; //��¼DFSʱ���������ǰһ������
	
	public SingleSourcePath(Graph G, int s) { //���캯����ִ��DFS 
		G.validateVertex(s);
		
		this.G = G;
		this.s = s;
		visited = new boolean[G.V()];
		pre = new int[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			pre[i] = -1; //-1ָ���Ǵ�Դ�㲻�ɴﵽ����i, ��Ȼ��visited[i]����
		dfs(s, s);
	}
 
	
	private void dfs(int v, int parent) { //����Ͷ����ǰһ������
		pre[v] = parent; //��¼����v�ĸ����
		visited[v] = true; 
		for (int w : G.adj(v)) 
			if (visited[w] == false) //û�б����ʵĽ��v���ڽӵ�
				dfs(w, v); 
	}  
	
	public boolean isConnectedTo(int t) { //��Դ���Ƿ�ɴ�t
		G.validateVertex(t);
		return visited[t];
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
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		SingleSourcePath sspath = new SingleSourcePath(g, 0); //Դ��Ϊ0�ĵ�Դ·����
		System.out.print("0 -> 6 : ");
		System.out.println(sspath.path(6));
		System.out.print("0 -> 4 : ");
		System.out.println(sspath.path(4));
	}

}
