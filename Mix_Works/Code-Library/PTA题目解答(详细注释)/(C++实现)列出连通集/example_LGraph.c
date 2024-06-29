/***************************************************************************************
列出连通集（采用链表方式存储） 
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
#define ERROR        0
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
/* define the AdjNode */
typedef struct AdjNode *PtrToAdjNode;
struct AdjNode{
	VertexType V;
	WeightType Weight;
	PtrToAdjNode Next;
};
/* define the HeadNode */
typedef struct HeadNode *PtrToHeadNode;
typedef struct HeadNode{
	DataType Data;
	PtrToAdjNode FirstAdjNode;
}ListHeadNode[MaxNumVertex];
/* define the Graph and func */
typedef struct GNode *PtrToGNode;
struct GNode{
	int Nv;
	int Ne;
	ListHeadNode G;
};
typedef PtrToGNode LGraph;

LGraph BuildLGraph(void);
LGraph CreateLGraph(int numVertex);
void InsertEdge(LGraph Graph, Edge E);
void DFS(LGraph Graph, VertexType V, void(*ptrf)(VertexType));
void BFS(LGraph Graph, VertexType V, void(*ptrf)(VertexType));
void Visited(VertexType V);
/* define the Queue and func*/
typedef struct QNode *PtrToQNode;
struct QNode{
	int Front;
	int Rear;
	int MaxSize;
	QueDataType Element[MaxNumVertex + 1];
};
typedef PtrToQNode Queue;

Queue CreateQue(void);
void AddQue(Queue Q, QueDataType V);
bool IsEmpty(Queue Q);
bool IsFull(Queue Q);
QueDataType DeleteQue(Queue Q);
/* define the global variable */
bool visited[MaxNumVertex];
/////////////////////////////////////////////
int main()
{
	LGraph Graph;
	VertexType V;
	Graph = BuildLGraph();
	/* DFS */
	/* initial the visited[] */
	for(V = 0; V < Graph->Nv; V++)
		visited[V] = false;
	for(V = 0; V < Graph->Nv; V++)
		if( !visited[V] ){
			printf("{ ");
			DFS(Graph, V, Visited);
			printf("}\n");
		}
	/* BFS */
	/* initial the visited[] */
	for(V = 0; V < Graph->Nv; V++)
		visited[V] = false;
	for(V = 0; V < Graph->Nv; V++)
		if( !visited[V] ){
			printf("{ ");
			BFS(Graph, V, Visited);
			printf("}\n");
		}	

	return 0;
}

LGraph BuildLGraph(void)
{
	int numVertex, i;
	LGraph Graph;
	Edge E;
	
	scanf("%d", &numVertex);
	Graph = CreateLGraph(numVertex);
	scanf("%d", &Graph->Ne);
	E = (Edge)malloc(sizeof(struct ENode));
	for(i = 0; i < Graph->Ne; i++){
		scanf("%d %d", &E->V1, &E->V2);
		InsertEdge(Graph, E);
	}
	free(E);
	
	return Graph;
}

LGraph CreateLGraph(int numVertex)
{
	LGraph Graph;
	VertexType V;
	Graph = (LGraph)malloc(sizeof(struct GNode));
	Graph->Nv = numVertex;
	Graph->Ne = 0;
	for(V = 0; V < Graph->Nv; V++)
		Graph->G[V].FirstAdjNode = NULL;
	
	return Graph;
}

void InsertEdge(LGraph Graph, Edge E)
{
	PtrToAdjNode NewNode, NewNode2;
	/*insert from v1 to v2*/
	NewNode = (PtrToAdjNode)malloc(sizeof(struct AdjNode));
	NewNode->V = E->V2;
	NewNode->Next = Graph->G[E->V1].FirstAdjNode;
	Graph->G[E->V1].FirstAdjNode = NewNode;
	
	/*insert from v2 to v1*/
	NewNode2 = (PtrToAdjNode)malloc(sizeof(struct AdjNode));
	NewNode2->V = E->V1;
	NewNode2->Next = Graph->G[E->V2].FirstAdjNode;
	Graph->G[E->V2].FirstAdjNode = NewNode2;
}

void DFS(LGraph Graph, VertexType V, void(*ptrf)(VertexType))
{
	PtrToAdjNode temp;
	ptrf(V);
	visited[V] = true;
	for(temp = Graph->G[V].FirstAdjNode; temp; temp = temp->Next)
		if( !visited[temp->V] )
			DFS(Graph, temp->V, ptrf);
}

void Visited(VertexType V)
{
	printf("%d ", V);
}

void BFS(LGraph Graph, VertexType V, void(*ptrf)(VertexType))
{
	Queue Q;
	VertexType W;
	PtrToAdjNode temp;
	
	Q = CreateQue();
	ptrf(V);
	visited[V] = true;
	AddQue(Q, V);
	while( !IsEmpty(Q) ){
		W = DeleteQue(Q);
		for(temp = Graph->G[W].FirstAdjNode; temp; temp = temp->Next)
			if( !visited[temp->V] ){
				ptrf(temp->V);
				visited[temp->V] = true;
				AddQue(Q, temp->V);
			}
	}
	free(Q);
}
Queue CreateQue(void)
{
	Queue Q;
	Q = (Queue)malloc(sizeof(struct QNode));
	Q->Front = Q->Rear = 0;
	Q->MaxSize = MaxNumVertex + 1;
	
	return Q;
}
void AddQue(Queue Q, QueDataType V)
{
	if( !IsFull(Q) ){
		Q->Rear = (Q->Rear + 1) % Q->MaxSize;
		Q->Element[Q->Rear] = V;
	}
	else 
		printf("The queue is full, Add failed!\n");
}
bool IsEmpty(Queue Q)
{
	return (Q->Front == Q->Rear);
}
bool IsFull(Queue Q)
{
	return ((Q->Rear + 1) % Q->MaxSize == Q->Front);
}
QueDataType DeleteQue(Queue Q)
{
	QueDataType item;
	if( !IsEmpty(Q) ){
		Q->Front = (Q->Front + 1) % Q->MaxSize;
		item = Q->Element[Q->Front];
	}
	else{
		printf("The queue is empty, Delete failed!\n");
		item = ERROR;
	}
	return item;
}

