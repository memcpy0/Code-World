/***************************************************************************************
哈利·波特的考试
哈利·波特要考试了，他需要你的帮助。这门课学的是用魔咒将一种动物变成另一种动物的本事。
例如将猫变成老鼠的魔咒是haha，将老鼠变成鱼的魔咒是hehe等等。反方向变化的魔咒就是简单地将原来的魔咒倒过来念，
例如ahah可以将老鼠变成猫。另外，如果想把猫变成鱼，可以通过念一个直接魔咒lalala，也可以将猫变老鼠、
老鼠变鱼的魔咒连起来念：hahahehe。
现在哈利·波特的手里有一本教材，里面列出了所有的变形魔咒和能变的动物。老师允许他自己带一只动物去考场，
要考察他把这只动物变成任意一只指定动物的本事。于是他来问你：带什么动物去可以让最难变的那种动物
（即该动物变为哈利·波特自己带去的动物所需要的魔咒最长）需要的魔咒最短？例如：如果只有猫、鼠、鱼，
则显然哈利·波特应该带鼠去，因为鼠变成另外两种动物都只需要念4个字符；而如果带猫去，则至少需要念
6个字符才能把猫变成鱼；同理，带鱼去也不是最好的选择。
输入格式:
输入说明：输入第1行给出两个正整数N (≤100)和M，其中N是考试涉及的动物总数，M是用于直接变形的魔咒条数。
为简单起见，我们将动物按1~N编号。随后M行，每行给出了3个正整数，分别是两种动物的编号、
以及它们之间变形需要的魔咒的长度(≤100)，数字之间用空格分隔。
输出格式:
输出哈利·波特应该带去考场的动物的编号、以及最长的变形魔咒的长度，中间以空格分隔。
如果只带1只动物是不可能完成所有变形要求的，则输出0。如果有若干只动物都可以备选，则输出编号最小的那只。
输入样例:
6 11
3 4 70
1 2 1
5 4 50
2 6 50
5 6 60
1 3 70
4 6 60
3 6 80
5 1 100
2 4 60
5 2 80
输出样例:
4 70
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

#define MaxNumVertex 100
#define INFINITY     65535
#define VertexOffset 1

/* define edge */
typedef int  VertexType;
typedef int  WeightType;
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
MGraph BuildMGraph(void);
void InsertEdge(MGraph Graph, Edge E);
void Floyd(MGraph Graph, WeightType Dist[][MaxNumVertex], VertexType Path[][MaxNumVertex]);
VertexType FindMaxLength(MGraph Graph, WeightType Dist[][MaxNumVertex], WeightType *maxLength);
/* test data */
MGraph TestData(void);

int main()
{
	MGraph Graph;
	WeightType Dist[MaxNumVertex][MaxNumVertex], maxLength[MaxNumVertex];
	VertexType Path[MaxNumVertex][MaxNumVertex], minVertex;

	Graph = BuildMGraph();
//	Graph = TestData();
	Floyd(Graph, Dist, Path);
	minVertex = FindMaxLength(Graph, Dist, maxLength);
	if( minVertex != 0 )
		printf("%d %d\n", minVertex, maxLength[minVertex - VertexOffset]);
	else 
		printf("0\n");
	
	return 0;
}

MGraph BuildMGraph(void)
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
	scanf("%d", &Graph->Ne);
	for(i = 0; i < Graph->Ne; i++){
		scanf("%d %d %d", &E->V1, &E->V2, &E->Weight);
		E->V1 -= VertexOffset;
		E->V2 -= VertexOffset;
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

void Floyd(MGraph Graph, WeightType Dist[][MaxNumVertex], VertexType Path[][MaxNumVertex])
{
	VertexType i, j, k;
	VertexType V, W;
	/* initial the Dist and Path*/
	for(V = 0; V < Graph->Nv; V++)
		for(W = 0; W < Graph->Nv; W++){
			Dist[V][W] = Graph->G[V][W];
			Path[V][W] = V;
		}
	/* do the Floyd algorithm */
	for(k = 0; k < Graph->Nv; k++)			
		for(i = 0; i < Graph->Nv; i++)			
			for(j = 0; j < Graph->Nv; j++)
				if(Dist[i][k] + Dist[k][j] < Dist[i][j]){
					Dist[i][j] = Dist[i][k] + Dist[k][j];
					Path[i][j] = k;
				}
}

VertexType FindMaxLength(MGraph Graph, WeightType Dist[][MaxNumVertex], WeightType *maxLength)
{
	VertexType V, W, minVertex;
	
	for(V = 0; V < Graph->Nv; V++)
		maxLength[V] = 0;
	/* find max for each V */
	for(V = 0; V < Graph->Nv; V++)
		for(W = 0; W < Graph->Nv; W++)
			if(Dist[V][W] > maxLength[V])
				maxLength[V] = Dist[V][W];
	/* find min for all V */
	minVertex = 0;
    for(V = 0; V < Graph->Nv; V++)
    	if(maxLength[V] < maxLength[minVertex])
    		minVertex = V;
    /* judge if the minVertex is right */
    for(V = 0; V < Graph->Nv; V++)
    	if(Dist[minVertex][V] == INFINITY)
    		return 0;
	
	return minVertex + VertexOffset;
}

MGraph TestData(void)
{
	int i, j;
	MGraph Graph;
	VertexType V1[11] = {3, 1, 5, 2, 5, 1, 4, 3, 5, 2, 5};
	VertexType V2[11] = {4, 2, 4, 6, 6, 3, 6, 6, 1, 4, 2};
	WeightType We[11] = {70,1, 50,50,60,70,60,80,100,60,80};
	Graph = (MGraph)malloc(sizeof(struct GNode));	
	Graph->Nv = 6;
	Graph->Ne = 11;
	for(i = 0; i < 11; i++)
		for(j = 0; j < 11; j++){
			if(i == j)
				Graph->G[i][j] = 0;
			else 
				Graph->G[i][j] = INFINITY;
		}
	for(i = 0; i < 11; i++){
		Graph->G[V1[i] - VertexOffset][V2[i] - VertexOffset] = We[i];
		Graph->G[V2[i] - VertexOffset][V1[i] - VertexOffset] = We[i];
	}
	
	return Graph;
}
