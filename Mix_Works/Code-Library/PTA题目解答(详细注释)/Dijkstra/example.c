/***************************************************************************************
DijkstraÀ„∑® µœ÷ 
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxNumVertex 500
#define INFINITY     65535
#define ERROR        -1
/* define edge */
typedef int  VertexType;
typedef int  WeightType;
typedef int  CostType;
typedef char DataType;
typedef struct ENode *PtrToENode;
struct ENode{
	VertexType V1, V2;
	WeightType Weight;
};
typedef PtrToENode Edge;
/* define graph and func*/
typedef struct GNode *PtrToGNode;
struct GNode{
	int Nv;
	int Ne;
	WeightType G[MaxNumVertex][MaxNumVertex];
	DataType Data[MaxNumVertex];
};
typedef PtrToGNode MGraph;

MGraph BuildMGraph(VertexType *S);
void InsertEdge(MGraph Graph, Edge E);
void Dijkstra(MGraph Graph, WeightType *Dist, VertexType *Path, VertexType S);
VertexType FindMinValue(WeightType *Dist, int numV, bool *collectS);
/* test data */
MGraph TestData(VertexType *S);

int main()
{
	MGraph Graph;
	WeightType Dist[MaxNumVertex];
	VertexType Path[MaxNumVertex], S, minVertex, V;
	
//	Graph = BuildMGraph(&S);
	Graph = TestData(&S);
	Dijkstra(Graph, Dist, Path, S);
	for(V = 0; V < Graph->Nv; V++)
		printf("%d ", Dist[V]);
	
	return 0;
}

MGraph BuildMGraph(VertexType *S)
{
	int i, j;
	MGraph Graph;
	Edge E;
	
	Graph = (MGraph)malloc(sizeof(struct GNode));
	E = (Edge)malloc(sizeof(struct ENode));
	scanf("%d", &Graph->Nv);
	Graph->Ne = 0;
	for(i = 0; i < Graph->Nv; i++)
		for(j = 0; j < Graph->Nv; j++){
			if(i == j)
				Graph->G[i][j] = 0;
			else 
				Graph->G[i][j] = INFINITY;
		}
	scanf("%d %d", &Graph->Ne, S);
	for(i = 0; i < Graph->Ne; i++){
		scanf("%d %d %d", &E->V1, &E->V2, &E->Weight);
		InsertEdge(Graph, E);
	}
	free(E);
			
	return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
	Graph->G[E->V1][E->V2] = E->Weight;
	Graph->G[E->V2][E->V1] = E->Weight;
}

void Dijkstra(MGraph Graph, WeightType *Dist, VertexType *Path, VertexType S)
{
	VertexType V, minV;
	bool collectS[MaxNumVertex];
	/* initial the Dist and Path */
	for(V = 0; V < Graph->Nv; V++){
		Dist[V] = Graph->G[S][V];
		Path[V] = S;
		collectS[V] = false; 
	}
	/* do the Dijkstra */
	while(1){
		minV = FindMinValue(Dist, Graph->Nv, collectS);
		if(minV == ERROR)	
			break;
		collectS[minV] = true;
		for(V = 0; V < Graph->Nv; V++)
			if(collectS[V] == false && Graph->G[minV][V] < INFINITY)
				if(Dist[minV] + Graph->G[minV][V] < Dist[V]){
					Dist[V] = Dist[minV] + Graph->G[minV][V];
					Path[V] = minV;
				}
	}
}

VertexType FindMinValue(WeightType *Dist, int numV, bool *collectS)
{
	VertexType minV, V;
	WeightType minDist;
	
	minDist = INFINITY;
	minV = ERROR;
	for(V = 0; V < numV; V++)
		if(collectS[V] == false && Dist[V] < minDist){
			minDist = Dist[V];
			minV = V;
		}
			
	return minV;
}

MGraph TestData(VertexType *S)
{
	int i, j;
	MGraph Graph;
	VertexType V1[] = {0,0,1,1,2,3,3,6,6,5,2,5,4,5,7,8,7};
	VertexType V2[] = {1,3,3,2,5,5,6,5,8,8,4,4,7,7,8,9,9};
	WeightType We[] = {2,5,2,5,4,4,2,3,7,6,8,2,5,9,3,8,4};
	Graph = (MGraph)malloc(sizeof(struct GNode));	
	Graph->Nv = 10;
	Graph->Ne = 17;
	for(i = 0; i < Graph->Ne; i++)
		for(j = 0; j < Graph->Ne; j++){
			if(i == j)
				Graph->G[i][j] = 0;
			else 
				Graph->G[i][j] = INFINITY;
		}
	for(i = 0; i < Graph->Ne; i++){
		Graph->G[V1[i]][V2[i]] = We[i];
		Graph->G[V2[i]][V1[i]] = We[i];
	}
	*S = 0;
	
	return Graph;
}
