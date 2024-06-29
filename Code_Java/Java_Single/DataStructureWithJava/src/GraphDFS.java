import java.util.ArrayList;
//Graph无权无向图
//O(V+E)时间复杂度
//求图的联通分量 Y
//求两点间是否可达 Y
//求两点间的一条路径 两点在同一个联通分量，意味着两点间有路径 单源路径
//检测图中是否有环
//二分图检测
//寻找图中的桥
//寻找图中的割点
//哈密尔顿路径
//拓扑排序
//深度优先遍历的非递归实现
//图的邻接矩阵的深度优先遍历
public class GraphDFS {
	
	private Graph G;
	private boolean[] visited;
	private ArrayList<Integer> pre = new ArrayList<>(); //深度优先遍历先序遍历
	private ArrayList<Integer> post = new ArrayList<>(); //深度优先遍历后序遍历
	
	public GraphDFS(Graph G) { //构造函数中执行DFS, 用户通过order()访问算法逻辑
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
			if (visited[w] == false) //结点v的邻接点没有被访问
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
