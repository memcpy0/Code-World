#include <bits/stdc++.h>
using namespace std;

//�ڽӾ���
{
//������Ȩͼ��д��,�������͵�ͼд������
inline void add(int u, int v, int w) {
	mat[u][v] = mat[v][u] = w;
}
}

//�ڽӱ�
{
//���û�б�Ȩ���Բ��ýṹ��,��intֻ�洢�յ㼴��
struct edge {
	int from, to, w;										//��:���,�յ�,Ȩֵ
	edge(int a, int b, int c) { from = a; to = b; w = c; }  //�Ա߸�ֵ
};

vector<edge> e[NUM];						//e[i]���i��������ӵ����б�
//��ʼ���ڽӱ�
for (int i = 1; i <= n; ++i) e[i].clear();

//�������
e[a].push_back(edge(a, b, c));				//�ѱ�(a,b)�浽���a���ڽӱ���
//����д�ɺ���
inline void add(int f, int t, int w) {
	edge e = {f, t, w};
	e[f].push_back(e);
}
//������ͼ��������add����
inline void add2(int f, int t, int w) {
	add(f, t, w);
	add(t, f, w);
}

//�������u�������ڽӵ�
for (int i = 0; i < e[u].size(); ++i) {		//���u���ڽӵ���e[u].size()��
	...
}
}
//��ʽǰ����
{ 
const int NUM = 1000000;		//һ�������,һ��������
struct Edge {
	int to, next, w;			//��:�յ�to,��һ����next,Ȩֵw; ������head[]��
} edge[NUM];
int head[NUM];					//head[u]ָ����u�ĵ�һ���ߵĴ洢λ��
int cnt;						//��¼edge[]��ĩβλ��,�¼���ı߷���ĩβ

void init() {
	for (int i = 0; i < NUM; ++i) {
		edge[i].next = -1;		//-1:����,û����һ����
		head[i] = -1;			//-1:�����ڴӽ��i�����ı�
	}
	cnt = 0;
}
//ÿ�μ���һ���±�,����ֱ�Ӽӵ�����edge[]��ĩβ
void addEdge(int u, int v, int w) {
	edge[cnt].to = v;
	edge[cnt].w = w;
	edge[cnt].next = head[u];	//ָ����u��һ�δ�ıߵ�λ��
	head[u] = cnt++;			//���½��u���±ߵĴ��λ��,����edge��ĩβ; cntָ����һ���մ�
}

//�������i�������ڽӵ�
for (int i = head[u]; ~i; i = edge[i].next) //~iҲ��д��i != -1
	...
}
}

int main() {


	return 0;
}
