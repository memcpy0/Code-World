import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.LinkedList;
//������Ȩͼ �ڽӱ� O(V + E)�ռ临�Ӷ� ����O(E)
//��ͼ ʱ�临�Ӷ�: O(E*V), ���ж�ƽ�б�(Ҫ���ٲ���)
//�鿴�����Ƿ�����: O(degree(v)) ���O(V)
//��һ������������ڽ��: O(degree(v)) ���O(V) �˻��Ǻ��ٵ�
//TreeSet �����
//HashSet ��ϣ��

public class AdjList {
	private int V; // ������
	private int E; // ����
	private LinkedList<Integer>[] adj; // ��������
	
	public AdjList(String filename) {
		File file = new File(filename);
		
		try (Scanner scanner = new Scanner(file)){
			V = scanner.nextInt();
			if (V < 0) // ����������Ϊ��
				throw new IllegalArgumentException("V must be non-negative");
			
			adj = new LinkedList[V]; // ������������
			for (int i = 0; i < V; ++i)
				adj[i] = new LinkedList<Integer>(); // �ֱ𴴽�ÿһ������Ԫ��
			
			E = scanner.nextInt();
			if (E < 0) // ��������Ϊ��
				throw new IllegalArgumentException("E must be non-negative");
			
			for (int i = 0; i < E; ++i) {
				int a = scanner.nextInt();
				validateVertex(a);
				int b = scanner.nextInt();
				validateVertex(b);
				
				// ֻ������ͼ
				if (a == b) // �Ի�
					throw new IllegalArgumentException("Self Loop is Detected!");
				if (adj[a].contains(b)) // ƽ�б�, �Ƿ��������� O(V)
					throw new IllegalArgumentException("Parallel Edges are Detected!");
				
				adj[a].add(b); // �ڶ���a���ڽӵ�������b
				adj[b].add(a); // ͬ��
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
		return adj[v].contains(w); // v�Ƿ��ڽ�w
	}
	public LinkedList<Integer> adj(int v) { // �����붥��v���ڵĶ�������
		validateVertex(v); 
		return adj[v];
	}
	public int degree(int v) { // �����������ͼ�Ķ���v�Ķ�
		return adj(v).size();
	}
	
	@Override 
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(String.format("V = %d, E = %d\n", V, E)); // ��ӡ�������ͱ���
		
		for (int v = 0; v < V; ++v) { // ��ӡ����
			sb.append(String.format("%d : ", v));
			for (int w : adj[v]) // һ��������
				sb.append(String.format("%d ", w));
			sb.append('\n');
		}
		return sb.toString();
	}
	
	public static void main(String[] args) {
		AdjList AdjList = new AdjList("g.txt");
		System.out.print(AdjList);
	}

}