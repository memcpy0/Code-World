#include <iostream>
#include <initializer_list>
#include <cstdio> 
#include <algorithm>
#include <vector>
using namespace std;
/*
** ����һ���㷨����, ��AOE���Ĺؼ�·����
** �����Լ��������ݻ�ʹ��P266ҳͼ7.36���ݲ��ԡ�
P266ҳͼ7.36����: 
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
	//VertexType *V;				 //ͼ�Ķ�������; //���趥�����Ϊ0~numVertexes-1  
	const int numVertexes, numEdges; //������,���� 
	WeightType **G;					 //����ָ��,����ͼ�ڽӾ���,�洢��<u,v>��Ȩֵ 
	int *indegrees;					 //���������������� 
	int *etv, *ltv; 				 //�¼������緢��ʱ��,������ʱ�� 
	int *topOrder, top;		    	 //�洢�����������е�ջ��ջ��ָ�� 
	int inode, enode;				 //Դ��ͻ�� 
	vector<vector<int>> adjList;	 //�洢���йؼ�·�����ڽӱ� 
public:  
	//����(������,����)���캯��
    MatrixGraph(int nv, int ne) : numVertexes(nv), numEdges(ne) { //��ʼ���б��ʼ������ 
		G = new int*[numVertexes];
        for (int i = 0; i < numVertexes; ++i)
        	G[i] = new int[numVertexes];
        //��ʼ���ڽӾ���,INF��ʾ������������ 
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
    //�б��ʼ��,��ʱ����Ҫ����;���ά����һ����ʼ�� 
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
    ~MatrixGraph() {				//��������
    	delete [] topOrder;
    	delete [] ltv;
    	delete [] etv;
    	delete [] indegrees;
    	for (int i = 0; i < numVertexes; ++i)
    		delete [] G[i];
    	delete [] G;  
    } 
	void InputGraph() {    
		cout << "���뻡β���㡢��ͷ���㡢��Ȩ��Ϣ(u v w), ������ŷ�ΧΪ0~����-1:" << endl;	 
		for (int i = 0; i < numEdges; ++i) {
			int u, v, w;
			cin >> u >> v >> w;
			++indegrees[v]; 	//����������� 
			G[u][v] = w; 
		}
	}
	//�洢�������е�topOrder; ������������޻�ͼ�򷵻�false 
	bool TopologicalSort() {
		int stackTop = -1, count = 0;
		int *vex = new int[numVertexes];
		//������ȵĶ�����ջ 
		for (int i = 0; i < numVertexes; ++i) 
			if (!indegrees[i]) vex[++stackTop] = i;
		//����������� 
		while (~stackTop) {
			int u = vex[stackTop--];
			topOrder[++top] = u; //����������������� 
			++count;
			for (int i = 0; i < numVertexes; ++i) {
				if (G[u][i] != INF) {   
					int v = i, w = G[u][i];  //v��u���ڽӵ�
					if (!(--indegrees[v])) vex[++stackTop] = v;
					//��etv[u]��ֵ����u�����к�̶���v 
					if (etv[u] + w > etv[v]) etv[v] = etv[u] + w;
				}
			}		 
		}
		//����������в�����ȫ������,��������޻�ͼ 
		if (count < numVertexes) return false;
		cout << "��������: ";
		for (int i = 0; i <= top; ++i) cout << char('A' + topOrder[i]) << " ";
		cout << endl; 
		inode = topOrder[0];		//���Դ�� 
		enode = topOrder[top]; 	    //������ 
		return true;
	}
	//���G�ĸ���ؼ��; 
	//���عؼ�·���ĳ���; ������������޻�ͼ�򷵻�-1 
	int GetCriticalPath() {
		if (TopologicalSort() == false) {
			cout << "����Ĳ��������޻�ͼ!" << endl; 
			return -1;  //��DAG,����-1
		}
		//etv��������ֵ���ǹؼ�·���ĳ���,Ҳ�ǻ���¼����������ٿ�ʼʱ��
		int maxLength = 0;
		for (int i = 0; i < numVertexes; ++i) 
			if (etv[i] > maxLength) maxLength = etv[i]; 
		fill(ltv, ltv + numVertexes, maxLength);
		//��topOrder��ջ��˳��Ϊ����������,���ltv�¼���ٷ���ʱ������
		while (~top) {	//top != -1
			int u = topOrder[top--];
			for (int i = 0; i < numVertexes; ++i) {
				if (G[u][i] != INF) {  //�ߴ���ʱ
					int v = i, w = G[u][i];	//u�ĺ�������v
					//��u�����к�������v��ltvֵ����ltv[u]
					//����������,���������ltv�Ѿ��õ� 
					if (ltv[v] - w < ltv[u]) ltv[u] = ltv[v] - w;
				}
			}
		}
		printf("��Դ��%c�����%c�Ĺؼ��:\n", char(inode + 'A'),char(enode + 'A'));
		//�����ڽӾ�������б�,���������翪ʼʱ�����ٿ�ʼʱ��
		int ete, lte;
		for (int i = 0; i < numVertexes; ++i) {
			for (int j = 0; j < numVertexes; ++j) {
				if (G[i][j] != INF) {
					int u = i, v = j, w = G[u][v];
					ete = etv[u], lte = ltv[v] - w;
					//������ete����lte,˵��û���ӳ�ʱ��,�ǹؼ�� 
					if (ete == lte) {
						printf("<%c,%c>, weight=%d\n", char(u + 'A'), char(v + 'A'), w);
						//cout << u << "->" << v << ", weight=" << w << endl;
						adjList[u].push_back(v); //����һ�ؼ�������ڽӱ� 
					}
				}
			}
		}
		//���عؼ�·���ĳ��� 
		return maxLength; 
	}
	void DFSPrint(vector<int> &temp, int u) {
		if (u == enode) {
			printf("%c", 'A' + temp[0]);
			for (int i = 1; i < temp.size(); ++i) //��������ؼ�·�� 
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
	//��ӡ���еĹؼ�·�� 
	void PrintAllKeyPaths() {
		cout << "ͼ�����еĹؼ�·������:" << endl; 
		vector<int> path; 
		path.push_back(inode); //�ؼ�·������� 
		DFSPrint(path, inode);
	}
}; 

int main() {
	//��ƽ��ʹ�ö�ά����һ��д�ڴ����� 
	MatrixGraph constgraph { //INF��ʾ������������ 
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
	cout << "ͼ�Ĺؼ�·������Ϊ: " << keyPathLen << endl; 
	constgraph.PrintAllKeyPaths(); 
	//�������붥�����������ͱ���Ϣ 
//	int M, N;
//	cout << "���붥�����ͻ���:" << endl;
//	cin >> M >> N;
//	MatrixGraph graph(M, N);
//	graph.InputGraph();
//	int criticalPathLen = graph.GetCriticalPath();
//	cout << "ͼ�Ĺؼ�·������Ϊ: " << criticalPathLen << endl; 
	return 0;
}

