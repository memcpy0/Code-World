import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Collections;
//Graph��Ȩ����ͼ
//O(V+E)ʱ�临�Ӷ�
//(��������Ȩͼ) ��Դ���·������ BFS
//���Ը���, n��SingleSourcePath�������е��·����������
//�����Դ�㵽Ŀ����Ƿ�ɴ�
//��������һ��·�� ������ͬһ����ͨ��������ζ���������·�� ��Դ·��
 
public class SingleSourcePathBFS {
	
	private Graph G;
	private int s; //��Դ·����Դ�� ��s�����ҵ�ǰ�����пɴﶥ���·��
	private boolean[] visited; //��Ȼ, ��������visited, ��pre�Ϳ��Ա����Ƿ�����˸ö���
	private int[] pre; //��¼BFSʱ���������ǰһ������
	
	public SingleSourcePathBFS(Graph G, int s) { //���캯����ִ��DFS 
		G.validateVertex(s);
		
		this.G = G;
		this.s = s;
		visited = new boolean[G.V()];
		pre = new int[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			pre[i] = -1; //-1ָ���Ǵ�Դ�㲻�ɴﵽ����i, ��Ȼ��visited[i]����
		
		bfs(s); //ֻ��Ҫ����Դ�����ڵ���һ����ͨ����
	}
 
	
	private void bfs(int s) { //���
		Queue<Integer> queue = new LinkedList<>(); 
		queue.add(s);
		visited[s] = true;
		pre[s] = s; //��¼���
		
		while (!queue.isEmpty()) {
			int v = queue.remove();
			
			for (int w : G.adj(v)) {
				if (visited[w] == false) {
					queue.add(w);
					visited[w] = true;
					pre[w] = v; //��¼����w��ǰһ������, ������·��
				}
			}
		}
	}  
	
	public boolean isConnectedTo(int t) { //��Դ���Ƿ�ɴ�t
		G.validateVertex(t); //����Ƿ�Ϸ�
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
		SingleSourcePathBFS sspathBFS = new SingleSourcePathBFS(g, 0); //Դ��Ϊ0�ĵ�Դ���·��
		System.out.print("0 -> 6 : ");
		System.out.println(sspathBFS.path(6)); 
		System.out.print("0 -> 4 : ");
		System.out.println(sspathBFS.path(4));
	}

}