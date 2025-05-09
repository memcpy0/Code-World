import java.util.ArrayList;
//Graph无权无向图
//O(V+E)时间复杂度
//求图的联通分量个数Connected Component
//求两个顶点是否联通
//求图中的不同的联通分量的序列
public class CC {
	
	private Graph G;
	private int[] visited; 
	private int cccnt = 0; //联通分量个数, 同时作为区分不同联通分量的id
	
	private void dfs(int v) {
		visited[v] = cccnt; 
		for (int w : G.adj(v)) 
			if (visited[w] == -1) //结点v的邻接点没有被访问
				dfs(w); 
	} 
	public CC(Graph G) { //构造函数中执行DFS, 用户通过order()访问算法逻辑
		this.G = G;
		visited = new int[G.V()];
		for (int i = 0; i < visited.length; ++i) 
			visited[i] = -1;
		
		for (int i = 0; i < G.V(); ++i) {
			if (visited[i] == -1) { //visited[i]为-1, 没有访问
				dfs(i);
				++cccnt;
			}
		}
	}
 
	public boolean isConnected(int v, int w) { //判断两个顶点是否联通
		G.validateVertex(v);
		G.validateVertex(w);
		return visited[v] == visited[w]; //两个顶点属于的联通分量的ID相同
	}

	 
	public int count() { //联通分量的个数
//		for (int e : visited) 
//			System.out.println(e + " ");
//		System.out.println();
		return cccnt;
	}
	
	public ArrayList<Integer>[] components() { //返回所有的联通分量
		ArrayList<Integer>[] res = new ArrayList[cccnt];
		
		for (int i = 0; i < cccnt; ++i) 
			res[i] = new ArrayList<>();  //分别创建
		
		for (int v = 0; v < G.V(); ++v) 
			res[visited[v]].add(v);      //将顶点加到visited标明的对应的联通分量中
		
		return res;
	}
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		System.out.println(g.toString());
		CC cc = new CC(g);
		System.out.println(cc.count());  //打印联通分量的个数
		System.out.println(cc.isConnected(0, 4)); //检测两个顶点是否联通
		
		ArrayList<Integer>[] comp = cc.components();
		
		for (int ccid = 0; ccid < comp.length; ++ccid) {
			System.out.print(ccid + " : "); 
			for (int w : comp[ccid])  
				System.out.print(w + " ");
			System.out.println();
		}
	}

}
