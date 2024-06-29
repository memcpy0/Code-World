#include <bits/stdc++.h>
using namespace std;
// Trie
void insert(string str) {
	int p = 1;
	for (int i = 0; i < str.size(); ++i) {
		int x = str[i] - 'a';
		if (trie[p][x] == 0) trie[p][x] = ++totle; // why + ����� 
		p = trie[p][x];
	}
	end[p] = true;
}
bool search(string str) {
	int p = 1;
	for (int i = 0; i < str.size(); ++i) {
		p = trie[p][str[i] - 'a'];
		if (p == 0) return false;
	}
	return end[p];
}
// ���鼯
void init(void) {
	for (int i = 0; i <= n; ++i) fa[i] = i;
}
int find(int x) {
	if (fa[x] == x) return x;
	return fa[x] = find(fa[x]);
}
void merge(int x, int y) {
	fa[x] = y;
}
// �ֿ�
// �ṹ��
// 0 | 1 2 3 | 4 5 6 | 7 8 9 Index
// N | 2 3 4 | 5 8 9 | 1 6 6 data
struct Piece { // ������ṹ 
	int l, r; // ���ұ߽� 
	int sum;  // ���� 
	int add; // ��� 
	Piece(int _l = 0, int _r = 0, int _s = 0, int _a = 0) {
		l = _l, r = _r, sum = _s, add = _a;
	}
	#define l(p) piece[p].l
	#define r(p) piece[p].r
	#define sum(p) piece[p].sum
	#define add(p) piece[p].add 
} piece[SIZE]; 
int n, m;
int len, num;
int data[SIZE], pos[SIZE];
//////// 
void init(void) {
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) cin >> data[i];
}
void buildPiece(void) { // ���� // ������ 
	len = sqrt(n); // ÿ���С 3
	num = (n % len ? n / len + 1 : n / len); // ���� 3
	for (int i = 1; i <= num; ++i) { // ȷ��ÿһ������ұ߽� 
		l(i) = (i - 1) * len + 1, r(i) = i * len;
	}
	if (r(num) > n) r(num) = n; // �ұ߽糬��ʵ�ʱ߽磬���� 
	for (int i = 1; i <= num; ++i) 
		for (int j = l(i); j <= r(i); ++j) 
			sum(i) += data[j], pos[j] = i; // ÿһ��ĺ� ÿ�����ݴ����ĸ����� 
}
void change(int l, int r, int add) {
	int pl = pos[l], pr = pos[r];
	if (p1 == pr) { // ����ͬһ���� 
		if (l == l(pl) && r == r(pr)) add(pl) += add; // ???
		else {
			for (int i = l; i <= r; ++i) data[i] += add;
			sum(pl) += add * (r - l + 1);
		}
	} else {
		for (int i = l; i <= r(pl); ++i) data[i] += add; 
		sum(pl) += add * (r(pl) - l + 1);
		for (int i = pl + 1; i <= pr - 1; ++i) add(i) += add, sum(i) += (r(i) - l(i) + 1);	
		for (int i = l(pr); i <= r; ++i) data[i] += add;
		sum(pr) += add * (r - l(pr) + 1);
	}
}			 
int ask(int l, int r) {
	int pl = pos[l], pr = pos[r];
	int val = 0;
	if (pl == pr) {
		if (l == l(pl) && r(pr) == r) val = sum(pl);
		else {
			for (int i = l; i <= r; ++i) val += add(pl) + data[i]; // ???
		}
	} else {
		for (int i = l; i <= r(pl); ++i) ans += 
		for (
		for (int i = 
	}
	return val;
}
////////
// �߶��� 
// �ڽӾ���
// �ڽӱ�

// 01����
// ��ȫ����
// ���ر���
// ���鱳��
// ��������

// ����DP
// ����DP
// �ı��β���ʽ
// ����
// ����
// ��������
// ����ʽA*
// ����ʽIDA*
// �����ж�
// ����ɸ��
// gcd
// lcm
// ��չŷ�����
// ȡ�๫ʽ
// ͬ�෽��
// ���������
// Floyd
// Dijkstra 
// Bellman-Ford
// SPFA
// kruskal
// prim
// �߶���
// ��״����
// ����LCA
// 
// ���
// ��ɢ��
// ��ϣ
// ��������
// vector
// string
// queue
// priority_queue
// map
// pair
// ����
// ͼ�ı��� 
// deque

#include <bits/stdc++.h>
using namespace std;

const int maxv = 110;
const int inf  = 123456789;
int n, m;

int graph[maxv][maxv];

void floyd() {
	int s = 1;
	for (int k = 1; k <= n; ++k) 
		for (int i = 1; i <= n; ++i) 
			for (int j = 1; j <= n; ++j) 
				if (graph[i][j] > graph[i][k] + graph[k][j]) 
					graph[i][j] = graph[i][k] + graph[k][j];
	printf("%d\n", graph[s][n]); 
}

int main() {
	while (~scanf("%d%d", &n, &m)) {
		if (!n && !m) break;
		for (int i = 1; i <= n; ++i) {
			graph[i][i] = 0;
			for (int j = 1; j <= n; ++j) {
				graph[i][j] = inf;
			}
	}
		while (m--) {
			int a, b;
			scanf("%d%d", &a, &b);
			scanf("%d", &graph[a][b]);
			graph[b][a] = graph[a][b];
		}
		floyd();
	}
}
