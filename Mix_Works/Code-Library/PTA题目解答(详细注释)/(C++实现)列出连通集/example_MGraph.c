/***************************************************************************************
列出连通集（采用邻接矩阵方式存储） 
给定一个有N个顶点和E条边的无向图，请用DFS和BFS分别列出其所有的连通集。假设顶点从0到N-1编号。
进行搜索时，假设我们总是从编号最小的顶点出发，按编号递增的顺序访问邻接点。
输入格式:
输入第1行给出2个整数N(0<N≤10)和E，分别是图的顶点数和边数。随后E行，每行给出一条边的两个端点。每行中的数字之间用1空格分隔。
输出格式:
按照"{ v1 v2 ... vk }"的格式，每行输出一个连通集。先输出DFS的结果，再输出BFS的结果。
输入样例:
8 6
0 7
0 1
2 0
4 1
2 4
3 5
输出样例:
{ 0 1 4 2 7 }
{ 3 5 }
{ 6 }
{ 0 1 2 7 4 }
{ 3 5 }
{ 6 }
*****************************************************************************************/ 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define MaxNumVertex 10
#define QueERROR     0

typedef int        VertexType;
typedef int        WeightType;
typedef char       DataType;
typedef VertexType QueDataType;
/* define the Edge */
typedef struct ENode *PtrToENode;
struct ENode{
	VertexType V1, V2;
	WeightType Weight;
};
typedef PtrToENode Edge;
/* define the Graph */
typedef struct GNode *PtrToGNode;
struct GNode{
	int Nv;
	int Ne;
	WeightType G[MaxNumVertex][MaxNumVertex];
	DataType Data[MaxNumVertex];
};
typedef PtrToGNode MGraph;

MGraph CreateMGraph(int vertexNum);
MGraph BuildMGraph(void);
void InsertEdge(MGraph Graph, Edge E);
void DFS(MGraph Graph, VertexType V, void (*ptrf)(VertexType));
void BFS(MGraph Graph, VertexType V, void (*ptrf)(VertexType));
void Visited(VertexType V);
bool IsAdj(MGraph Graph, VertexType V, VertexType W);
/* define the Queue */
typedef struct QNode *PtrToQNode;
struct QNode{
	int Front;
	int Rear;
	int MaxSize;
	QueDataType Element[MaxNumVertex + 1];
};
typedef PtrToQNode Queue;

Queue CreateQue(void);
void AddQue(Queue Q, QueDataType item);
QueDataType DeleteQue(Queue Q);
bool IsEmpty(Queue Q);
bool IsFull(Queue Q);
/* Globle variable */ 
bool visited[MaxNumVertex];

int main()
{
	MGraph Graph;
	VertexType V;
	
	Graph = BuildMGraph();
	/* do the DFS */
	/* initial the visited[] */
	for(V = 0; V < Graph->Nv; V++)
		visited[V] = false;
	for(V = 0; V < Graph->Nv; V++){
		if( !visited[V] ){
			printf("{ ");
			DFS(Graph, V, Visited);
			printf("}\n");	
		}
	}
	/* do the BFS */
	/* initial the visited[] */
	for(V = 0; V < Graph->Nv; V++)
		visited[V] = false;
	for(V = 0; V < Graph->Nv; V++){
		if( !visited[V] ){
			printf("{ ");
			BFS(Graph, V, Visited);
			printf("}\n");	
		}
	}
	
	return 0;
}

MGraph CreateMGraph(int vertexNum)
{
	VertexType V, W;
	MGraph Graph;
	Graph = (MGraph )malloc(sizeof(struct GNode));
	Graph->Nv = vertexNum;
	Graph->Ne = 0;
	for(V = 0; V < vertexNum; V++)
		for(W = 0; W < vertexNum; W++)
			Graph->G[V][W] = 0;
	
	return Graph;
}

MGraph BuildMGraph(void)
{
	int vertexNum, i; 
	MGraph Graph;
	Edge E;
	
	scanf("%d", &vertexNum);
	Graph = CreateMGraph(vertexNum);
	scanf("%d", &Graph->Ne);
	E = (Edge )malloc(sizeof(struct ENode));
	for(i = 0; i < Graph->Ne; i++){
		scanf("%d %d", &E->V1, &E->V2);
		InsertEdge(Graph, E);
	}
	free(E);
	
	return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
	Graph->G[E->V1][E->V2] = 1;
	Graph->G[E->V2][E->V1] = 1;
}

void DFS(MGraph Graph, VertexType V, void (*ptrf)(VertexType))
{
	VertexType W;
	
	visited[V] = true;
	ptrf(V);
	for(W = 0; W < Graph->Nv; W++) /*all the adjNode based in V*/
		if( IsAdj(Graph, V, W) && !visited[W] )
			DFS(Graph, W, ptrf);
}

void Visited(VertexType V)
{
	printf("%d ", V);
}

bool IsAdj(MGraph Graph, VertexType V, VertexType W)
{/*judge the W is adjNode of V or not in Graph*/
	return Graph->G[V][W];
}

void BFS(MGraph Graph, VertexType V, void (*ptrf)(VertexType))
{
	Queue Q;
	VertexType W, X;
	
	Q = CreateQue();
	/* visited the V */
	visited[V] = true;
	ptrf(V);
	AddQue(Q, V);
	while( !IsEmpty(Q) ){
		W = DeleteQue(Q);
		for(X = 0; X < Graph->Nv; X++)
			if( !visited[X] && IsAdj(Graph, W, X)){
				visited[X] = true;
				ptrf(X);
				AddQue(Q, X);
			}
	}
	free(Q);
}

Queue CreateQue(void)
{
	Queue Q;
	Q = (Queue )malloc(sizeof(struct QNode));
	Q->Front = Q->Rear = 0;
	Q->MaxSize = MaxNumVertex + 1;
	
	return Q;
}
void AddQue(Queue Q, QueDataType item)
{
	if( !IsFull(Q) ){
		Q->Rear = (Q->Rear + 1) % Q->MaxSize;
		Q->Element[Q->Rear] = item;
	}
	else 
		printf("The Queue is full, Add failed!\n");
}
QueDataType DeleteQue(Queue Q)
{
	QueDataType item;
	if( !IsEmpty(Q) ){
		Q->Front = (Q->Front + 1) % Q->MaxSize;
		item = Q->Element[Q->Front];
	}
	else {
		printf("The Queue is empty, Delete failed!\n");
		item = QueERROR;
	}
		
	return item;
}
bool IsEmpty(Queue Q)
{
	return ( Q->Front == Q->Rear );
}
bool IsFull(Queue Q)
{
	return ( (Q->Rear + 1) % Q->MaxSize == Q->Front);
}
 
