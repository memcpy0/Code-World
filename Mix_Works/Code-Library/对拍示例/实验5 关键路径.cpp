#include <iostream>
#include <initializer_list>
#include <cstdio> 
#include <algorithm>
#include <vector>
using namespace std;
/*
** 编制一个算法程序, 求AOE网的关键路径，
** 可以自己构造数据或使用P266页图7.36数据测试。
P266页图7.36数据: 
A  B  6
A  C  4
A  D  5
B  E  1
C  E  1
D  H  2
E  F  9
E  G  7
H  I  4
F  I  2
G  I  4
-------------
9 11
0  1  6
0  2  4
0  3  5
1  4  1
2  4  1
3  7  2
4  5  9
4  6  7
7  8  4
5  8  2
6  8  4
*/ 
typedef int VertexType;
typedef int WeightType; 
#define INF 0x3f3f3f3f
/*
etv : earliest time of vertex
ltv : lastest  time of vertex
ete : earliest time of edge
lte : lastest  time of edge
*/
class MatrixGraph {
private:
	//VertexType *V;				 //图的顶点数组; //假设顶点序号为0~numVertexes-1  
	const int numVertexes, numEdges; //顶点数,边数 
	WeightType **G;					 //二级指针,有向图邻接矩阵,存储边<u,v>的权值 
	int *indegrees;					 //各个顶点的入度数组 
	int *etv, *ltv; 				 //事件的最早发生时间,最晚发生时间 
	int *topOrder, top;		    	 //存储拓扑排序序列的栈和栈底指针 
	int inode, enode;				 //源点和汇点 
	vector<vector<int>> adjList;	 //存储所有关键路径的邻接表 
public:  
	//含参(顶点数,边数)构造函数
    MatrixGraph(int nv, int ne) : numVertexes(nv), numEdges(ne) { //初始化列表初始化常量 
		G = new int*[numVertexes];
        for (int i = 0; i < numVertexes; ++i)
        	G[i] = new int[numVertexes];
        //初始化邻接矩阵,INF表示不存在这条边 
		for (int i = 0; i < numVertexes; ++i) 
			for (int j = 0; j < numVertexes; ++j) 
				G[i][j] = INF;	
        
		indegrees = new int[numVertexes];
        fill(indegrees, indegrees + numVertexes, 0);
        
        etv = new int[numVertexes];
        fill(etv, etv + numVertexes, 0);
        ltv = new int[numVertexes];
        
        topOrder = new int[numVertexes];
        top = -1;
        
        adjList.resize(numVertexes + 10); 
	}	 
    //列表初始化,这时不需要边数;像二维数组一样初始化 
    MatrixGraph(initializer_list<initializer_list<int>> lst) : MatrixGraph(lst.size(), 0) { 
    	int r = 0, c = 0;
		for (auto rowbeg = lst.begin(); rowbeg != lst.end(); ++rowbeg) {
			for (auto colbeg = rowbeg->begin(); colbeg != rowbeg->end(); ++colbeg) {
				G[r][c] = *colbeg;   
				if (*colbeg != INF) ++indegrees[c];
				++c; 
			}
			++r;
			c = 0;
		} 
	}
    ~MatrixGraph() {				//析构函数
    	delete [] topOrder;
    	delete [] ltv;
    	delete [] etv;
    	delete [] indegrees;
    	for (int i = 0; i < numVertexes; ++i)
    		delete [] G[i];
    	delete [] G;  
    } 
	void InputGraph() {    
		cout << "输入弧尾顶点、弧头顶点、弧权信息(u v w), 顶点序号范围为0~弧数-1:" << endl;	 
		for (int i = 0; i < numEdges; ++i) {
			int u, v, w;
			cin >> u >> v >> w;
			++indegrees[v]; 	//计算入度数组 
			G[u][v] = w; 
		}
	}
	//存储拓扑序列到topOrder; 如果不是有向无环图则返回false 
	bool TopologicalSort() {
		int stackTop = -1, count = 0;
		int *vex = new int[numVertexes];
		//将零入度的顶点入栈 
		for (int i = 0; i < numVertexes; ++i) 
			if (!indegrees[i]) vex[++stackTop] = i;
		//求出拓扑序列 
		while (~stackTop) {
			int u = vex[stackTop--];
			topOrder[++top] = u; //将顶点加入拓扑序列 
			++count;
			for (int i = 0; i < numVertexes; ++i) {
				if (G[u][i] != INF) {   
					int v = i, w = G[u][i];  //v是u的邻接点
					if (!(--indegrees[v])) vex[++stackTop] = v;
					//用etv[u]的值更新u的所有后继顶点v 
					if (etv[u] + w > etv[v]) etv[v] = etv[u] + w;
				}
			}		 
		}
		//如果拓扑序列不含有全部顶点,则非有向无环图 
		if (count < numVertexes) return false;
		cout << "拓扑序列: ";
		for (int i = 0; i <= top; ++i) cout << char('A' + topOrder[i]) << " ";
		cout << endl; 
		inode = topOrder[0];		//求出源点 
		enode = topOrder[top]; 	    //求出汇点 
		return true;
	}
	//输出G的各项关键活动; 
	//返回关键路径的长度; 如果不是有向无环图则返回-1 
	int GetCriticalPath() {
		if (TopologicalSort() == false) {
			cout << "输入的不是有向无环图!" << endl; 
			return -1;  //非DAG,返回-1
		}
		//etv数组的最大值就是关键路径的长度,也是汇点事件的最早和最迟开始时间
		int maxLength = 0;
		for (int i = 0; i < numVertexes; ++i) 
			if (etv[i] > maxLength) maxLength = etv[i]; 
		fill(ltv, ltv + numVertexes, maxLength);
		//以topOrder出栈的顺序为逆拓扑排序,求出ltv事件最迟发生时间数组
		while (~top) {	//top != -1
			int u = topOrder[top--];
			for (int i = 0; i < numVertexes; ++i) {
				if (G[u][i] != INF) {  //边存在时
					int v = i, w = G[u][i];	//u的后续顶点v
					//用u的所有后续顶点v的ltv值更新ltv[u]
					//逆拓扑排序,后续顶点的ltv已经得到 
					if (ltv[v] - w < ltv[u]) ltv[u] = ltv[v] - w;
				}
			}
		}
		printf("从源点%c到汇点%c的关键活动:\n", char(inode + 'A'),char(enode + 'A'));
		//遍历邻接矩阵的所有边,计算活动的最早开始时间和最迟开始时间
		int ete, lte;
		for (int i = 0; i < numVertexes; ++i) {
			for (int j = 0; j < numVertexes; ++j) {
				if (G[i][j] != INF) {
					int u = i, v = j, w = G[u][v];
					ete = etv[u], lte = ltv[v] - w;
					//如果活动的ete等于lte,说明没有延迟时间,是关键活动 
					if (ete == lte) {
						printf("<%c,%c>, weight=%d\n", char(u + 'A'), char(v + 'A'), w);
						//cout << u << "->" << v << ", weight=" << w << endl;
						adjList[u].push_back(v); //将这一关键活动插入邻接表 
					}
				}
			}
		}
		//返回关键路径的长度 
		return maxLength; 
	}
	void DFSPrint(vector<int> &temp, int u) {
		if (u == enode) {
			printf("%c", 'A' + temp[0]);
			for (int i = 1; i < temp.size(); ++i) //输出这条关键路径 
				printf("->%c", 'A' + temp[i]);
			cout << endl;
			return;
		}
		for (const int v : adjList[u]) {
			temp.push_back(v);
			DFSPrint(temp, v);
			temp.pop_back();
		}
	}
	//打印所有的关键路径 
	void PrintAllKeyPaths() {
		cout << "图中所有的关键路径如下:" << endl; 
		vector<int> path; 
		path.push_back(inode); //关键路径的起点 
		DFSPrint(path, inode);
	}
}; 

int main() {
	//像平常使用二维数组一样写在代码中 
	MatrixGraph constgraph { //INF表示不存在这条边 
		{INF, 6,  4,  5 ,INF,INF,INF,INF,INF},
		{INF,INF,INF,INF, 1 ,INF,INF,INF,INF},
		{INF,INF,INF,INF, 1 ,INF,INF,INF,INF},
		{INF,INF,INF,INF,INF,INF,INF, 2 ,INF},
		{INF,INF,INF,INF,INF, 9 , 7 ,INF,INF},
		{INF,INF,INF,INF,INF,INF,INF,INF, 2 },
		{INF,INF,INF,INF,INF,INF,INF,INF, 4 },
		{INF,INF,INF,INF,INF,INF,INF,INF, 4 },
		{INF,INF,INF,INF,INF,INF,INF,INF,INF}
	};
	int keyPathLen = constgraph.GetCriticalPath();
	cout << "图的关键路径长度为: " << keyPathLen << endl; 
	constgraph.PrintAllKeyPaths(); 
	//或者输入顶点数、弧数和边信息 
//	int M, N;
//	cout << "输入顶点数和弧数:" << endl;
//	cin >> M >> N;
//	MatrixGraph graph(M, N);
//	graph.InputGraph();
//	int criticalPathLen = graph.GetCriticalPath();
//	cout << "图的关键路径长度为: " << criticalPathLen << endl; 
	return 0;
}

