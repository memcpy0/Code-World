/*
13.19 对于13.5节讨论的旅行商问题，应用分支限界算法到如下实例：
	inf    5	2	10 
	2	  inf   5   12
	3      7   inf   5
	8      2    4    inf
上面的实例给出了4个城市和相互之间访问的耗费, 找出耗费最小的旅行 
*/  
#include <iostream> 
#include <queue>
#include <vector> 
using namespace std;  
struct Node {
	int lcost, cc, rcost;  //lcost = cc + rcost子树费用下界, cc当前费用, x[s:n-1]中顶点最小出边费用   
	int s, *x;	//根节点到当前节点的路径为x[0:s], 需要进一步搜索的顶点是x[s+1:n-1]  
};
 
class cmp{
public:
	bool operator()(const Node &a1,const Node &a2)  
	{
		return a1.lcost>a2.lcost;
	}
};
 
class TSP {
private:		
	int n;		//图的顶点数量  
 	int *a;		//图的邻接矩阵  
 	int NoEdge;	//无边标记 
 	 
 	int *x ;	//当前解 
 	int cc;		//当前耗费 
 	int *bestx;	//最优解、 
 	int bestc;	//最短路长 
 
	
public:
    int Solve(int n_, int *a_, int NoEdge_, int v[])
    {
    	n = n_;
    	a = a_;
    	NoEdge = NoEdge_;
    	priority_queue<Node,vector<Node>,cmp> q;
    	int  MinOut[n + 1]; //i节点与其他节点的路径中最小的耗费 
    	int MinSum = 0;
    	for(int i = 1; i <= n; ++i) {
    		int Min = NoEdge;
    		for(int j = 1; j <= n; ++j) {
    			if(a[i * (n + 1) + j] != NoEdge && (a[i * (n+1) + j] < Min || Min == NoEdge))
    				Min = a[i*(n+1)+j];
    		}
    		if(Min == NoEdge) return NoEdge;			//无回路 
    		MinOut[i] = Min;
			MinSum += Min; 
		}
		//初始化
		Node E;
		E.x = new int[n];
		for (int i = 0; i < n; ++i) E.x[i] = i + 1;	//初始顺序1-n 
		E.s = 0;						  	//初始走到0 
		E.cc = 0;						  	//初始费用0 
		E.rcost = MinSum;				 
		int bestc = NoEdge;
		//搜索解空间		
		while(E.s < n-1) {	//非叶结点
			if(E.s == n-2) { //当前扩展结点是叶结点的父亲 
				//所构成回路是否是最优解 
				if(a[E.x[n-2]*(n+1)+E.x[n-1]]!=NoEdge && a[E.x[n-1]*(n+1)+1]!=NoEdge &&
					(E.cc+a[E.x[n-1]*(n+1)+1]+a[E.x[n-1]*(n+1)+1] <bestc || bestc==NoEdge)) {
						//费用更小的路 
						bestc = E.cc +a[E.x[n-2]*(n+1)+E.x[n-1]] + a[E.x[n-1]*(n+1)+1];
						E.cc = bestc;
						E.lcost = bestc;
						E.s++;
						q.push(E);
				} 
				else { // 舍弃扩展节点 
					delete [] E.x; 
				}
			}
			else { // 产生当前扩展节点的孩子节点 
				for(int i=E.s+1; i<n;i++) {
					if(a[E.x[E.s]*(n+1)+E.x[i]]!=NoEdge) {
						//可行结点 
						int cc = E.cc + a[E.x[E.s]*(n+1)+E.x[i]];
						int rcost = E.rcost - MinOut[E.x[E.s]];
						int b = cc+rcost;	//下界
						if(b<bestc || bestc == NoEdge) {
							//子树可能含有最优解
							//结点插入最小堆
							Node N;
							N.x = new int[n];
							for(int j=0;j<n;++j) N.x[j]=E.x[j];
							N.x[E.s+1] =E.x[i];		
							N.x[i]  = E.x[E.s+1];	//节点交换
							N.cc =cc;				//费用更新 
							N.s = E.s+1;			//层数+1 
							N.lcost = b;
							N.rcost =rcost;
							q.push(N); 
						}
					}
 				}
 				delete [] E.x;	
			}
			if(q.empty()) break;
			E = q.top();
			q.pop(); 
		}
		if(bestc == NoEdge) return NoEdge;	//无回路 
		
    	//复制最优解到v[1：n] 
    	for(int i = 0; i < n; ++i) v[i + 1] = E.x[i];
    	
    	//释放队列中的剩余元素 
		while(!q.empty()) {
			E = q.top();
			if(E.x) delete [] E.x;
			q.pop();
		}
		return bestc; 
	}
};
/*
最短旅行商回路路长11
路线: 1 3 4 2 1
*/
int main() {
 	int n = 4; 
 	int NoEdge = 0x3fffffff; 
 	int a[] = { //耗费矩阵从下标为1开始，值为0表示不可达 
 		0, 0, 0, 0, 0, 
 		0, 0, 5, 2, 10, 
 		0, 2, 0, 5, 12, 
 		0, 3, 7, 0, 5, 
 		0, 8, 2, 4, 0 
 	};
 	int v[5]; 
 	TSP tsp; 
 	int c = tsp.Solve(n, a, NoEdge, v); 
 	cout << "最短路长" << c <<endl; 
 	cout << "路线:"; 
 	for(int i = 1; i <= n; ++i) 
 		cout<< ' ' << v[i];
 	cout << " 1" << endl;
 	return 0;
} 
