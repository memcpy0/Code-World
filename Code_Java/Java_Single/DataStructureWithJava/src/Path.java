import java.util.ArrayList;
import java.util.Collections;
//Graph无权无向图
//O(V+E)时间复杂度
//固定点对问题
//可以复用SingleSourcePath的一些逻辑
//求两点间是否联通
//求两点间的一条路径 两点在同一个联通分量，意味着两点间有路径 固定点对
 
public class Path {
	
	private Graph G;
	private int s; //固定点对的源点 从s出发找到前往所有可达顶点的路径
	private int t; //固定点对的终点
	private boolean[] visited; //当然, 可以抛弃visited, 用pre就可以表达是否访问了该顶点
	private int[] pre; //记录DFS时各个顶点的前一个顶点
	
	public Path(Graph G, int s, int t) { //构造函数中执行DFS, 用户通过order()访问算法逻辑
		G.validateVertex(s);
		G.validateVertex(t);
		
		this.G = G; 
		this.s = s;
		this.t = t;
		
		visited = new boolean[G.V()];
		pre = new int[G.V()];
		
		for (int i = 0; i < G.V(); ++i) 
			pre[i] = -1; //-1指的是从源点不可达到顶点i, 当然用visited[i]更好
		dfs(s, s);
	}
 
	
	private boolean dfs(int v, int parent) { //参数是顶点和顶点的前一个顶点
		pre[v] = parent; //记录顶点v的父结点
		visited[v] = true; 
		
		if (v == t) return true; //如果得到了从s到t的一条路径
		for (int w : G.adj(v)) 
			if (visited[w] == false) //没有被访问的结点v的邻接点
				if (dfs(w, v)) 
					return true;
		return false;
	}  
	
	public boolean isConnected() { //从源点是否可达t
		return visited[t];
	}
	
	public Iterable<Integer> path() { //得到从源点到t的一条路径
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
		Path path = new Path(g, 0, 6); //源点为0,终点为6的固定点对类
		System.out.print("0 -> 6 : ");
		System.out.println(path.path()); 
		
		Path path2 = new Path(g, 0, 1); //源点为0,终点为1的固定点对类
		System.out.print("0 -> 1 : ");
		System.out.println(path2.path()); 
		
		Path path3 = new Path(g, 0, 4); //源点为0,终点为4的固定点对类
		System.out.print("0 -> 4 : ");
		System.out.println(path3.path()); //0到5不联通
	}

} 
//检测无向图的环