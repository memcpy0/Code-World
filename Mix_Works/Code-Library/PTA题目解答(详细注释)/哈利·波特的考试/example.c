/***************************************************************************************
���������صĿ���
����������Ҫ�����ˣ�����Ҫ��İ��������ſ�ѧ������ħ�佫һ�ֶ�������һ�ֶ���ı��¡�
���罫è��������ħ����haha�������������ħ����hehe�ȵȡ�������仯��ħ����Ǽ򵥵ؽ�ԭ����ħ�䵹�����
����ahah���Խ�������è�����⣬������è����㣬����ͨ����һ��ֱ��ħ��lalala��Ҳ���Խ�è������
��������ħ���������hahahehe��
���ڹ��������ص�������һ���̲ģ������г������еı���ħ����ܱ�Ķ����ʦ�������Լ���һֻ����ȥ������
Ҫ����������ֻ����������һָֻ������ı��¡������������㣺��ʲô����ȥ���������ѱ�����ֶ���
�����ö����Ϊ�����������Լ���ȥ�Ķ�������Ҫ��ħ�������Ҫ��ħ����̣����磺���ֻ��è�����㣬
����Ȼ����������Ӧ�ô���ȥ����Ϊ�����������ֶ��ﶼֻ��Ҫ��4���ַ����������èȥ����������Ҫ��
6���ַ����ܰ�è����㣻ͬ������ȥҲ������õ�ѡ��
�����ʽ:
����˵���������1�и�������������N (��100)��M������N�ǿ����漰�Ķ���������M������ֱ�ӱ��ε�ħ��������
Ϊ����������ǽ����ﰴ1~N��š����M�У�ÿ�и�����3�����������ֱ������ֶ���ı�š�
�Լ�����֮�������Ҫ��ħ��ĳ���(��100)������֮���ÿո�ָ���
�����ʽ:
�������������Ӧ�ô�ȥ�����Ķ���ı�š��Լ���ı���ħ��ĳ��ȣ��м��Կո�ָ���
���ֻ��1ֻ�����ǲ�����������б���Ҫ��ģ������0�����������ֻ���ﶼ���Ա�ѡ������������С����ֻ��
��������:
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
�������:
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
