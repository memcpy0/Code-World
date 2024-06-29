import java.util.ArrayList;
//Graph无权无向图
//O(V+E)时间复杂度
//二分图检测 

//图的邻接矩阵的深度优先遍历
public class BipartitionDetection {
	
	private Graph G;
	private int[] colors; //颜色判断是否访问过; 同时在DFS中用染色来判断是否是二分图
	private boolean isBipart = true; //是否是二分图
	
	public BipartitionDetection(Graph G) { //构造函数中执行DFS, 用户通过order()访问算法逻辑
		this.G = G;
		colors = new int[G.V()]; //0是无色, -1是蓝色, 1是绿色
		
		for (int i = 0; i < G.V(); ++i) { //访问每一个联通分量, 如果每个联通分量都是二分的, 那么整张图就是二分的
			if (colors[i] == 0) {
				if (dfs(i, -1) == false) { // 为每个联通分量的初始点统一染色蓝色
					isBipart = false; //如果这个连通分量不是二分的, 整张图就不是二分的
					break;
				}
			}
		}
		
	}
  
	private boolean dfs(int v, int color) {
		colors[v] = color;
		
		for (int w : G.adj(v)) {
			if (colors[w] == 0) { //结点v的邻接点w没有被访问
				if (dfs(w, -color) == false) return false; //这个点是蓝色, 下个点是绿色; 否则是蓝色
			} else if (colors[v] == colors[w]) { //结点v的邻接点w访问过了, 且都是相同的颜色
				return false; //发现矛盾, 直接返回false
			}
		} 
		return true; //没有地方矛盾
	} 
	 
	public boolean isBipart() { //是否是二分图
		return isBipart;
	}
	public ArrayList<Integer>[] bipart() { //返回两个二分的部分
		ArrayList<Integer>[] res = new ArrayList[2];
		if (isBipart == false) return res;
		
		res[0] = new ArrayList<>();
		res[1] = new ArrayList<>();
		
		for (int i = 0; i < colors.length; ++i) {
			if (colors[i] == -1) res[0].add(i);
			else res[1].add(i);
		}
		return res;
	}
	
	public static void main(String[] args) {
		Graph g = new Graph("g.txt");
		BipartitionDetection bd = new BipartitionDetection(g);
		System.out.println(bd.isBipart());
		ArrayList<Integer>[] arr = bd.bipart();
		System.out.print(arr[0] + " ");
		System.out.println(arr[1]);
		
		Graph c = new Graph("completeGraph.txt");
		BipartitionDetection bd2 = new BipartitionDetection(c);
		System.out.println(bd2.isBipart());  
		ArrayList<Integer>[] arr2 = bd2.bipart();
		System.out.print(arr2[0] + " ");
		System.out.println(arr2[1]);
	}

}
