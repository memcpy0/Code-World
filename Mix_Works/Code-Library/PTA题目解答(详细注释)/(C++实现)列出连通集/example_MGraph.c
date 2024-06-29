/***************************************************************************************
�г���ͨ���������ڽӾ���ʽ�洢�� 
����һ����N�������E���ߵ�����ͼ������DFS��BFS�ֱ��г������е���ͨ�������趥���0��N-1��š�
��������ʱ�������������Ǵӱ����С�Ķ������������ŵ�����˳������ڽӵ㡣
�����ʽ:
�����1�и���2������N(0<N��10)��E���ֱ���ͼ�Ķ������ͱ��������E�У�ÿ�и���һ���ߵ������˵㡣ÿ���е�����֮����1�ո�ָ���
�����ʽ:
����"{ v1 v2 ... vk }"�ĸ�ʽ��ÿ�����һ����ͨ���������DFS�Ľ���������BFS�Ľ����
��������:
8 6
0 7
0 1
2 0
4 1
2 4
3 5
�������:
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
 
