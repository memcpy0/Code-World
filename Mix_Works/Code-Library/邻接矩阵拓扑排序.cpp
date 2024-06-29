#include<stdlib.h>
#define N 7//顶点个数
 
typedef struct 
{
	char vexs[N];//顶点数组
	int arcs[N][N];//邻接矩阵
}graph;
 
void TopoSort_matrix(graph g)
{
	int row[N]={0};//按照列来设置标志，为1表示已经输出（不再考虑），为0表示未输出。
	int v=1;//标志符，1表示已经输出（不再考虑），为0表示未输出，赋给row数组
	int i,j,k,t,m;
	for(k=0;k<N;k++)
	{
		for(j=0;j<N;j++)
		{
			if(row[j]==0)//活动j还未输出
			{
				t=1;//标识符
				for(i=0;i<N;i++)
					if(g.arcs[i][j]==1)//当前活动有入度（活动i必须在活动j之前）
					{
						t=0;
						break;
					}
				if(t==1)//活动j没有入度
				{
					m=j;
					break;
				}
			}
		}
		if(j!=N)
		{
			row[m]=v;
			printf("%c",g.vexs[m]);
			for(i=0;i<N;i++)
				g.arcs[m][i]=0;//将已经输出的活动所到达的下个活动的入度置为0
			v++;
		}
		else 
			break;
		}
		if(v-1<N)//当row中不是所有的元素都被赋予新值v时，说明有环存在
			printf("\n该有向图有环存在！");
	
}
 
void main()
{
	graph g;
	int matrix[N][N]={{0,1,1,0,0,0,0},
					  {0,0,0,0,0,1,1},
			    	  {0,0,0,0,0,0,1},
					  {0,0,1,0,1,0,0},
					  {0,0,0,0,0,0,1},
					  {0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0}};
	char vertex[N]={'A','B','C','D','E','F','G'};//初始化
		for(int i=0;i<N;i++)
		{
			g.vexs[i]=vertex[i];
			for(int j=0;j<N;j++)
				g.arcs[i][j]=matrix[i][j];
		}
	TopoSort_matrix(g);
	printf("\n");
}
