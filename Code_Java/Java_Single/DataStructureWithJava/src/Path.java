import java.util.ArrayList;
import java.util.Collections;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//�̶��������
//���Ը���SingleSourcePath��һЩ�߼�
//��������Ƿ���ͨ
//��������һ��·�� ������ͬһ����ͨ��������ζ���������·�� �̶����
 
public class Path {
	
	private Graph G;
	private int s; //�̶���Ե�Դ�� ��s�����ҵ�ǰ�����пɴﶥ���·��
	private int t; //�̶���Ե��յ�
	private boolean[] visited; //��Ȼ, ��������visited, ��pre�Ϳ��Ա���Ƿ�����˸ö���
	private int[] pre; //��¼DFSʱ���������ǰһ������
	
	public Path(Graph G, int s, int t) { //���캯����ִ��DFS, �û�ͨ��order()�����㷨�߼�
		G.validateVertex(s);
		G.validateVertex(t);
		
		this.G = G; 
		this.s = s;
		this.t = t;
		
		visited = new boolean[G.V()];
		pre = new int[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			pre[i] = -1; //-1ָ���Ǵ�Դ�㲻�ɴﵽ����i, ��Ȼ��visited[i]����
		dfs(s, s);
	}
 
	
	private boolean dfs(int v, int parent) { //�����Ƕ���Ͷ����ǰһ������
		pre[v] = parent; //��¼����v�ĸ����
		visited[v] = true; 
		
		if (v == t) return true; //����õ��˴�s��t��һ��·��
		for (int w : G.adj(v)) 
			if (visited[w] == false) //û�б����ʵĽ��v���ڽӵ�
				if (dfs(w, v)) 
					return true;
		return false;
	}  
	
	public boolean isConnected() { //��Դ���Ƿ�ɴ�t
		return visited[t];
	}
	
	public Iterable<Integer> path() { //�õ���Դ�㵽t��һ��·��
		ArrayList<Integer> res = new ArrayList<>();
		if (!isConnected()) return res;
		
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
		Path path = new Path(g, 0, 6); //Դ��Ϊ0,�յ�Ϊ6�Ĺ̶������
		System.out.print("0 -> 6 : ");
		System.out.println(path.path()); 
		
		Path path2 = new Path(g, 0, 1); //Դ��Ϊ0,�յ�Ϊ1�Ĺ̶������
		System.out.print("0 -> 1 : ");
		System.out.println(path2.path()); 
		
		Path path3 = new Path(g, 0, 4); //Դ��Ϊ0,�յ�Ϊ4�Ĺ̶������
		System.out.print("0 -> 4 : ");
		System.out.println(path3.path()); //0��5����ͨ
	}

} 
//�������ͼ�Ļ�