import java.util.ArrayList;
import java.util.Collections;
//Graph无权无向图
//O(V+E)时间复杂度
//单源路径问题
//可以复用, n个SingleSourcePath做到所有点对路径问题的求解
//求传入的源点到目标点是否可达
//求两点间的一条路径 两点在同一个联通分量，意味着两点间有路径 单源路径
 
public class SingleSourcePath {
	
	private Graph G;
	private int s; //单源路径的源点 从s出发找到前往所有可达顶点的路径
	private boolean[] visited; //当然, 可以抛弃visited, 用pre就可以表达是否访问了该顶点
	private int[] pre; //记录DFS时各个顶点的前一个顶点
	
	public SingleSourcePath(Graph G, int s) { //构造函数中执行DFS 
		G.validateVertex(s);
		
		this.G = G;
		this.s = s;
		visited = new boolean[G.V()];
		pre = new int[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			pre[i] = -1; //-1指的是从源点不可达到顶点i, 当然用visited[i]更好
		dfs(s, s);
	}
 
	
	private void dfs(int v, int parent) { //顶点和顶点的前一个顶点
		pre[v] = parent; //记录顶点v的父结点
		visited[v] = true; 
		for (int w : G.adj(v)) 
			if (visited[w] == false) //没有被访问的结点v的邻接点
				dfs(w, v); 
	}  
	
	public boolean isConnectedTo(int t) { //从源点是否可达t
		G.validateVertex(t);
		return visited[t];
	}
	
	public Iterable<Integer> path(int t) { //得到从源点到t的一条路径
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
		SingleSourcePath sspath = new SingleSourcePath(g, 0); //源点为0的单源路径类
		System.out.print("0 -> 6 : ");
		System.out.println(sspath.path(6));
		System.out.print("0 -> 4 : ");
		System.out.println(sspath.path(4));
	}

}
