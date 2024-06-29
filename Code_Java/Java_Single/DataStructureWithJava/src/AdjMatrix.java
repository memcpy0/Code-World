import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.ArrayList;
//无向无权图 邻接矩阵 空间  O(V^2)
//建图 时间复杂度: O(E), 有判断平行边(要快速查重)
//查看两点是否相邻: O(1)
//求一个点的所有相邻结点: O(V)
public class AdjMatrix {
	private int V; // 顶点数
	private int E; // 边数
	private int[][] adj;
	
	public AdjMatrix(String filename) {
		File file = new File(filename);
		
		try (Scanner scanner = new Scanner(file)){
			V = scanner.nextInt();
			if (V < 0) // 顶点数必须为正
				throw new IllegalArgumentException("V must be non-negative");
			
			adj = new int[V][V]; // 创建二维数组
			
			E = scanner.nextInt();
			if (E < 0) // 边数必须为正
				throw new IllegalArgumentException("E must be non-negative");
			
			for (int i = 0; i < E; ++i) {
				int a = scanner.nextInt();
				validateVertex(a);
				int b = scanner.nextInt();
				validateVertex(b);
				
				// 只处理简单图
				if (a == b) // 自环
					throw new IllegalArgumentException("Self Loop is Detected!");
				if (adj[a][b] == 1) // 平行边
					throw new IllegalArgumentException("Parallel Edges are Detected!");
				
				adj[a][b] = adj[b][a] = 1; 
			}
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	private void validateVertex(int v) {
		if (v < 0 || v >= V) 
			throw new IllegalArgumentException("vertex " + v + "is invalid");
	}
	
	public int V() { return V; }  // 返回邻接矩阵顶点数
	public int E() { return E; }  // 返回
	public boolean hasEdge(int v, int w) { // 是否存在一条边
		validateVertex(v); 
		validateVertex(w);
		return adj[v][w] == 1;
	}
	public ArrayList<Integer> adj(int v) { // 返回与顶点v相邻的顶点数组
		validateVertex(v);
		
		ArrayList<Integer> res = new ArrayList<>();
		for (int i = 0; i < v; ++i)  
				if (adj[v][i] == 1)
					res.add(i);
		return res;
	}
	public int degree(int v) { // 返回这个无向图的顶点v的度
		return adj(v).size();
	}
	
	@Override 
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("V = %d, E = %d\n", V, E)); // 打印顶点数和边数
		
		for (int i = 0; i < V; ++i) { // 打印数据
			for (int j = 0; j < V; ++j) 
				sb.append(String.format("%d ", adj[i][j]));
			sb.append('\n');
		}
		return sb.toString();
	}
	
	public static void main(String[] args) {
		AdjMatrix adjMatrix = new AdjMatrix("g.txt");
		System.out.print(adjMatrix);
	}

}
