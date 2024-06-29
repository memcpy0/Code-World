import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.ArrayList;
//������Ȩͼ �ڽӾ��� �ռ�  O(V^2)
//��ͼ ʱ�临�Ӷ�: O(E), ���ж�ƽ�б�(Ҫ���ٲ���)
//�鿴�����Ƿ�����: O(1)
//��һ������������ڽ��: O(V)
public class AdjMatrix {
	private int V; // ������
	private int E; // ����
	private int[][] adj;
	
	public AdjMatrix(String filename) {
		File file = new File(filename);
		
		try (Scanner scanner = new Scanner(file)){
			V = scanner.nextInt();
			if (V < 0) // ����������Ϊ��
				throw new IllegalArgumentException("V must be non-negative");
			
			adj = new int[V][V]; // ������ά����
			
			E = scanner.nextInt();
			if (E < 0) // ��������Ϊ��
				throw new IllegalArgumentException("E must be non-negative");
			
			for (int i = 0; i < E; ++i) {
				int a = scanner.nextInt();
				validateVertex(a);
				int b = scanner.nextInt();
				validateVertex(b);
				
				// ֻ�����ͼ
				if (a == b) // �Ի�
					throw new IllegalArgumentException("Self Loop is Detected!");
				if (adj[a][b] == 1) // ƽ�б�
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
	
	public int V() { return V; }  // �����ڽӾ��󶥵���
	public int E() { return E; }  // ����
	public boolean hasEdge(int v, int w) { // �Ƿ����һ����
		validateVertex(v); 
		validateVertex(w);
		return adj[v][w] == 1;
	}
	public ArrayList<Integer> adj(int v) { // �����붥��v���ڵĶ�������
		validateVertex(v);
		
		ArrayList<Integer> res = new ArrayList<>();
		for (int i = 0; i < v; ++i)  
				if (adj[v][i] == 1)
					res.add(i);
		return res;
	}
	public int degree(int v) { // �����������ͼ�Ķ���v�Ķ�
		return adj(v).size();
	}
	
	@Override 
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("V = %d, E = %d\n", V, E)); // ��ӡ�������ͱ���
		
		for (int i = 0; i < V; ++i) { // ��ӡ����
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
