#include <bits/stdc++.h>
using namespace std;

//邻接矩阵
{
//无向有权图的写法,其他类型的图写法类似
inline void add(int u, int v, int w) {
	mat[u][v] = mat[v][u] = w;
}
}

//邻接表
{
//如果没有边权可以不用结构体,用int只存储终点即可
struct edge {
	int from, to, w;										//边:起点,终点,权值
	edge(int a, int b, int c) { from = a; to = b; w = c; }  //对边赋值
};

vector<edge> e[NUM];						//e[i]存第i个结点连接的所有边
//初始化邻接表
for (int i = 1; i <= n; ++i) e[i].clear();

//存有向边
e[a].push_back(edge(a, b, c));				//把边(a,b)存到结点a的邻接表中
//或者写成函数
inline void add(int f, int t, int w) {
	edge e = {f, t, w};
	e[f].push_back(e);
}
//对无向图调用两次add即可
inline void add2(int f, int t, int w) {
	add(f, t, w);
	add(t, f, w);
}

//检索结点u的所有邻接点
for (int i = 0; i < e[u].size(); ++i) {		//结点u的邻接点有e[u].size()个
	...
}
}
//链式前向星
{ 
const int NUM = 1000000;		//一百万个点,一百万条边
struct Edge {
	int to, next, w;			//边:终点to,下一条边next,权值w; 起点放在head[]中
} edge[NUM];
int head[NUM];					//head[u]指向结点u的第一条边的存储位置
int cnt;						//记录edge[]的末尾位置,新加入的边放在末尾

void init() {
	for (int i = 0; i < NUM; ++i) {
		edge[i].next = -1;		//-1:结束,没有下一条边
		head[i] = -1;			//-1:不存在从结点i出发的边
	}
	cnt = 0;
}
//每次加入一条新边,都是直接加到整个edge[]的末尾
void addEdge(int u, int v, int w) {
	edge[cnt].to = v;
	edge[cnt].w = w;
	edge[cnt].next = head[u];	//指向结点u上一次存的边的位置
	head[u] = cnt++;			//更新结点u最新边的存放位置,就是edge的末尾; cnt指向下一个空处
}

//遍历结点i的所有邻接点
for (int i = head[u]; ~i; i = edge[i].next) //~i也可写成i != -1
	...
}
}

int main() {


	return 0;
}
