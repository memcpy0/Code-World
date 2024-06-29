#include <bits/stdc++.h>
using namespace std;
//�ж���һ������ͼ���Ƿ���ڻ�
//����ͼ�ڽӱ�
namespace DFS_BFS {
	class Graph {
	private:
		int numVertexes;	//��������0-numVertexes 
		list<int> *adj; 	//ָ��һ���ڽ����������ָ��
		bool isCyclicUtilDFS(int v, bool visited[], int parent);
		bool isCyclicUtilBFS(int v, bool visited[]); 
	public:
		Graph(int numVertexes);     //���붥���� 
		~Graph();					//�������� 
		void addEdge(int v, int w); //����ߵ�ͼ�� 
		bool isCyclicDFS();			//����л�����true
		bool isCyclicBFS(); 		//����л�����true
	};
	Graph::Graph(int numVertexes) {	
		this->numVertexes = numVertexes;
		adj = new list<int>[numVertexes];
	}
	Graph::~Graph() {
		delete [] adj;
	}
	
	void Graph::addEdge(int v, int w) {
		if (v == w) return; //�������Ի� 
		adj[v].push_back(w);
		adj[w].push_back(v);
	}
	
	//�ݹ��㷨,ʹ��visited[]��parent�ж�
	//����v�ɴ����ͼ(��ͨ����)���Ƿ���ڻ�
	bool Graph::isCyclicDFS(int v, bool visited[], int parent) {
		visited[v] = true; //��ǵ�ǰ�����ʹ�
		//�Ե�ǰ����������ڽӵ���е���
		list<int>::iterator i;
		for (i = adj[v].begin(); i != adj[v].end(); ++i) {
			//���һ���ڽӵ�û�б����ʹ�,�ݹ����
			if (!visited[*i]) { 
				if (isCyclicUtilDFS(*i, visited, v)) //���ִ��ڻ�,����true 
					return true;				  //��ʱ�˳� 
			} 
			//��һ�ڽӵ�v����DFS����i�ĸ����,���Ѿ������ʹ�,����ڻ� 
			else if (*i != parent) return true;	  //����true  
		}
		return false;
	}
	//�������ͼ�к��л�,����true,���򷵻�false
	bool Graph::isCyclicDFS() {
		//������ж���Ϊδ����
		bool visited[numVertexes]; 
		for (int i = 0; i < numVertexes; ++i) visited[i] = false;
		//��ÿ��δ���ʹ��Ķ���,����isCyclicUtil�жϻ��Ƿ����
		for (int u = 0; u < numVertexes; ++u) 
			if (!visited[u] && isCyclicUtilDFS(u, visited, -1)) 
				return true;
		return false; 
	}
	
	//�����㷨 
	bool Graph::isCyclicUtilBFS(int v, bool visited[]) {
		//����ÿ�������parentΪ-1
		int parent[numVertexes];
		for (int i = 0; i < numVertexes; ++i) parent[i] = -1;
		//����һ��BFSʹ�õ�queue 
		queue<int> q;
		q.push(v);
		visited[v] = true;
		while (!q.empty()) {
			int u = q.front(); q.pop();
			//���u��ȫ���ڽӵ�v,���v����u��parent,�����Ѿ����ʹ�,����ڻ�
			for (auto i = adj[u].begin(); i != adj[u].end(); ++i) {
				int v = *i;
				if (!visited[v]) { //���û�з��� 
					visited[v] = true; //��Ƿ��� 
					q.push(v);	   //���� 
					parent[v] = u; //��¼����parent 
				} else if (v != parent[u]) //v��u���ڽӵ�,�����ʹ�,������u��parent 
					return true;   //����true
			}	
		}
		return false;
	} 
	bool Graph::isCyclicBFS() {
		//������ж���Ϊδ����
		bool visited[numVertexes];
		for (int i = 0; i < numVertexes; ++i) visited[i] = false;
		for (int u = 0; u < numVertexes; ++u)  
			if (!visited[u] && isCyclicUtilBFS(u, visited))
				return true;
		return false; 
	}
}

//------------------------------------------------------------------------ 
namespace union_find {
	//����ͼ�ı�
	class Edge {  
	public: 
		int src, dest;	
	};
	//�߼�����
	class EdgeSet {
	private:
		int V, E;
		Edge *edge; 
		int size;   //�ߵ����� 
	public:
		EdgeSet(int V, int E);
		~EdgeSet() { delete [] edge; }
		void addEdge(int v, int w); //����ߵ�ͼ�� 
		int find(int parent[], int i);
		void merge(int parent[], int x, int y);
		bool isCycle();
	};
	EdgeSet::EdgeSet(int V, int E) {
		size = 0; 
		this->V = V;
		this->E = E;
		this->edge = new Edge[this->E];
	}
	void EdgeSet::addEdge(int v, int w) {
		edge[size].src = v;
		edge[size].dest = w; 
		++size;	
	}
	
	//ʹ�ò��鼯�ж�����ͼ�еĻ�
	//�ҵ����Ԫ�����ڵ��Ӽ�
	int EdgeSet::find(int parent[], int i) {
		if (parent[i] == -1) return i;
		return find(parent, parent[i]);
	}
	//���������Ӽ��ĺϲ� 
	void EdgeSet::merge(int parent[], int xset, int yset) { 
		if (xset != yset) parent[xset] = yset;
	}
	//�������ͼ���Ƿ���ڻ� 
	bool EdgeSet::isCycle() {
		int parent[V];
		for (int i = 0; i < V; ++i) parent[i] = -1; 
		//����ͼ�����еı�,�ҵ�ÿ���ߵ������������ڵ��Ӽ�
		//�������һ����,���������������������Ӽ�����ͬ,�ʹ��ڻ� 
		//����ϲ�����������Ӽ� 
		for (int i = 0; i < E; ++i) {
			int x = find(parent, edge[i].src);
			int y = find(parent, edge[i].dest);
			if (x == y) return true;
			merge(parent, x, y);
		}
		return false;
	}
}
 
int main() {
	/*
        0  
        | \  
        |   \  
        1----2 */
//	union_find::EdgeSet es(3, 3);
//	es.addEdge(0, 1);  //(0,1)
//	es.addEdge(1, 2);  //(1,2)
//	es.addEdge(0, 2);  //(0,2)
//	es.isCycle() ? cout << "Contains Cycle!\n" : cout << "Contains No Cycle!\n"; 
	
	DFS_BFS::Graph g(5); //5������
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(0, 3);
	g.addEdge(3, 4);
	//g.isCyclicDFS() ? cout << "Contains Cycle!\n" : cout << "Doesn't Contain Cycle!\n";
	g.isCyclicBFS() ? cout << "Contains Cycle!\n" : cout << "Doesn't Contain Cycle!\n";
	
	
	DFS_BFS::Graph g2(3);
	g2.addEdge(0, 1);
	g2.addEdge(1, 2);
	//g2.isCyclicDFS() ? cout << "Contains Cycle!\n" : cout << "Doesn't Contain Cycle!\n";
	g2.isCyclicBFS() ? cout << "Contains Cycle!\n" : cout << "Doesn't Contain Cycle!\n";

    return 0;
}

