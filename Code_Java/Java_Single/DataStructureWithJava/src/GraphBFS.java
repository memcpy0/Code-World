import java.util.Queue;
import java.util.LinkedList;
import java.util.ArrayList;
//ͼ�Ĺ�����ȱ��� O(V+E)
//�󲿷�DFS�ܹ������BFSҲ���Խ��

public class GraphBFS {
	private Graph G;
	private boolean[] visited;
	
	private ArrayList<Integer> order = new ArrayList<>(); //������ȱ�����˳��
	
	public GraphBFS(Graph G) {
		this.G = G;
		visited = new boolean[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			if (visited[i] == false) 
				bfs(i);
	}
	
	private void bfs(int s) {
		Queue<Integer> queue = new LinkedList<>(); //Queue����һ������ǽӿ�,  ��Ҫָ�������ʵ����,  �������ܺ���
		queue.add(s);
		visited[s] = true;
		
		while (!queue.isEmpty()) {
			int v = queue.remove();
			order.add(v);
			
			for (int w : G.adj(v)) { //v�����ڶ���
				if (!visited[w]) {
					queue.add(w);
					visited[w] = true;
				}
			}
		}
	}
	
	public Iterable<Integer> order() {
		return order;
	}
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		GraphBFS graphBFS = new GraphBFS(g);
		System.out.println("BFS Order : " + graphBFS.order());

	}

}