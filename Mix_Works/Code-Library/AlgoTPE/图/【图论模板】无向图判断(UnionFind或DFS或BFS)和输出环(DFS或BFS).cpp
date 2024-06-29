#include <bits/stdc++.h>
using namespace std;
//判断在一张无向图中是否存在环
//无向图邻接表
namespace DFS_BFS {
	class Graph {
	private:
		int numVertexes;	//顶点编号数0-numVertexes 
		list<int> *adj; 	//指向一个邻接链表数组的指针
		bool isCyclicUtilDFS(int v, bool visited[], int parent);
		bool isCyclicUtilBFS(int v, bool visited[]); 
	public:
		Graph(int numVertexes);     //传入顶点数 
		~Graph();					//析构函数 
		void addEdge(int v, int w); //加入边到图中 
		bool isCyclicDFS();			//如果有环返回true
		bool isCyclicBFS(); 		//如果有环返回true
	};
	Graph::Graph(int numVertexes) {	
		this->numVertexes = numVertexes;
		adj = new list<int>[numVertexes];
	}
	Graph::~Graph() {
		delete [] adj;
	}
	
	void Graph::addEdge(int v, int w) {
		if (v == w) return; //不允许自环 
		adj[v].push_back(w);
		adj[w].push_back(v);
	}
	
	//递归算法,使用visited[]和parent判断
	//顶点v可达的子图(连通分量)中是否存在环
	bool Graph::isCyclicDFS(int v, bool visited[], int parent) {
		visited[v] = true; //标记当前结点访问过
		//对当前顶点的所有邻接点进行迭代
		list<int>::iterator i;
		for (i = adj[v].begin(); i != adj[v].end(); ++i) {
			//如果一个邻接点没有被访问过,递归访问
			if (!visited[*i]) { 
				if (isCyclicUtilDFS(*i, visited, v)) //发现存在环,返回true 
					return true;				  //及时退出 
			} 
			//这一邻接点v不是DFS树上i的父结点,且已经被访问过,则存在环 
			else if (*i != parent) return true;	  //返回true  
		}
		return false;
	}
	//如果无向图中含有环,返回true,否则返回false
	bool Graph::isCyclicDFS() {
		//标记所有顶点为未访问
		bool visited[numVertexes]; 
		for (int i = 0; i < numVertexes; ++i) visited[i] = false;
		//对每个未访问过的顶点,调用isCyclicUtil判断环是否存在
		for (int u = 0; u < numVertexes; ++u) 
			if (!visited[u] && isCyclicUtilDFS(u, visited, -1)) 
				return true;
		return false; 
	}
	
	//迭代算法 
	bool Graph::isCyclicUtilBFS(int v, bool visited[]) {
		//定义每个顶点的parent为-1
		int parent[numVertexes];
		for (int i = 0; i < numVertexes; ++i) parent[i] = -1;
		//定义一个BFS使用的queue 
		queue<int> q;
		q.push(v);
		visited[v] = true;
		while (!q.empty()) {
			int u = q.front(); q.pop();
			//检测u的全部邻接点v,如果v不是u的parent,但是已经访问过,则存在环
			for (auto i = adj[u].begin(); i != adj[u].end(); ++i) {
				int v = *i;
				if (!visited[v]) { //如果没有访问 
					visited[v] = true; //标记访问 
					q.push(v);	   //进队 
					parent[v] = u; //记录它的parent 
				} else if (v != parent[u]) //v是u的邻接点,被访问过,但不是u的parent 
					return true;   //返回true
			}	
		}
		return false;
	} 
	bool Graph::isCyclicBFS() {
		//标记所有顶点为未访问
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
	//无向图的边
	class Edge {  
	public: 
		int src, dest;	
	};
	//边集数组
	class EdgeSet {
	private:
		int V, E;
		Edge *edge; 
		int size;   //边的数量 
	public:
		EdgeSet(int V, int E);
		~EdgeSet() { delete [] edge; }
		void addEdge(int v, int w); //加入边到图中 
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
	
	//使用并查集判断无向图中的环
	//找到这个元素所在的子集
	int EdgeSet::find(int parent[], int i) {
		if (parent[i] == -1) return i;
		return find(parent, parent[i]);
	}
	//进行两个子集的合并 
	void EdgeSet::merge(int parent[], int xset, int yset) { 
		if (xset != yset) parent[xset] = yset;
	}
	//检查无向图中是否存在环 
	bool EdgeSet::isCycle() {
		int parent[V];
		for (int i = 0; i < V; ++i) parent[i] = -1; 
		//迭代图中所有的边,找到每条边的两个顶点所在的子集
		//如果发现一条边,它的两个顶点属于两个子集都相同,就存在环 
		//否则合并两个顶点的子集 
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
	
	DFS_BFS::Graph g(5); //5个顶点
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

