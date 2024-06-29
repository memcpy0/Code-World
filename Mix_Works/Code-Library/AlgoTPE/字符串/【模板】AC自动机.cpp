#include <bits/stdc++.h>
using namespace std;
#define MAXC 26
//结点结构
struct ACNode {
public:
	ACNode *fail;		//fail指针,指向和当前结点的某个后缀匹配的最长前缀的位置
	//类似KMP中的最长前后缀的概念,即目标串在trie树上进行匹配的时候,如果在P结点上匹配失败,那么应该跳到P->fail继续匹配，
	//如果还是失败,那么跳到P->fail->fail继续匹配,对于这个fail指针的构造，下文会详细讲解
	ACNode *next[MAXC]; //子结点指针,每个结点的子结点个数就是字符串中字符集的大小
	//以下的数据针对不同的题目进行适当注释和选择,因为内存有限
	int id;  //结点编号,每个结点编号唯一,用于状态转移的时候的下标映射
	int val; //当前结点和它的fail指针指向结点的结尾单词信息的集合 
	int cnt; //有些题中模式串可能有多个单词是相同的,但是计数的时候算多个
/*
结尾标记 int cnt, val;
每个模式串在进行插入的过程中，会对模式串本身进行一次线性的遍历，当遍历完毕即表示将整个串插入完毕，在结尾结点需要一个标记，表示它是一个模式串的末尾，
有些问题会出现多个相同的模式串，所以用cnt来表示该串出现的次数，每插入一次对cnt进行一次自增；
而有的题中，相同的模式串有不同的权值，并且模式串的个数较少(<= 15)，那么可以将该结点是否是模式串的末尾--用2的幂来表示，
压缩成二进制的整数记录在val上(例如当前结点是第二个模式串和第四个模式串的结尾，则val = (1010)2)。
*/
	void reset(int _id) {
		id = _id;
		val = 0;
		cnt = 0;
		fail = nullptr;
		memset(next, 0, sizeof(next));
	}
	
	//状态机中的 接收态 概念
	//模式串为不能出现的时候才有接收态 
	bool isReceiving() {
		return val != 0;
	}
};

class ACAutoman {
public:
	//结点相关结构
	ACNode *nodes[MAXQ]; //结点缓存,避免内存重复申请和释放,节省时间
	int nodesMax;		 //缓冲区大小,永远是递增的
	ACNode *root;		 //根结点指针
	int nodeCount; 		 //结点总数
	 
	
};
 
//AC自动机2222
//const int maxn = 1000000 + 100;
//const int SIGMA_SIZE = 26;
//const int maxnode = 1000000 + 100;
//int n, ans;
//bool vis[maxn];
//map<string, int> ms;
//int ch[maxnode][SIGMA_SIZE + 5];
//int val[maxnode];
//int idx(char c) { return c - 'a'; }
//
//struct Trie {
//	int sz;
//	Trie() {
//		sz = 1;
//		memset(ch[0], 0, sizeof(ch[0]));
//		memset(vis, 0, sizeof(vis));
//	}
//	void insert(char *s) {
//		int cur = 0;
//		for (int i = 0; s[i]; ++i) {
//			int c = idx(s[i]);
//			if (ch[cur][c] == 0) {
//				memset(ch[sz], 0, sizeof(ch[sz]));
//				val[sz] = 0;
//				ch[cur][c] = sz++;
//			}
//			cur = ch[cur][c];
//		}
//		val[u]++;
//	}
//};
//
////AC自动机
//int last[maxn], f[maxn];
//void print(int j) {
//	if (j && !vis[j]) {
//		ans += val[j];
//		vis[j] = 1;
//		print(last[j]);
//	}
//}
//int getFail() {
//	queue<int> q;
//	f[0] = 0;
//	for (int c = 0; c < SIGMA_SIZE; ++c) {
//		int u = ch[0][c];
//		if (u) {
//			f[u] = 0;
//			q.push(u);
//			last[u] = 0;
//		}
//	}
//	while (!q.empty()) {
//		int r = q.front(); q.pop();
//		for (int c = 0; c < SIGMA_SIZE; ++c) {
//			int u = ch[r][c];
//			if (!u) {
//				ch[r][c] = ch[f[r]][c];
//				continue;
//			}
//			q.push(u);
//			int v = f[r];
//			while (v && !ch[v][c]) v = f[v];
//			f[u] = ch[v][c];
//			last[u] = val[f[u]] ? f[u] : last[f[u]];
//		}
//	}
//}
//void find_T(char *T) {
//	int n = strlen(T);
//	int j = 0;
//	for (int i = 0; i < n; ++i) {
//		int c = idx(T[i]);
//		j = ch[j][c];
//		if (val[j]) print(j);
//		else if (last[j]) print(last[j]);
//	}
//}
//char tmp[105];
//char text[1000000 + 1000];
//				 
//int main() { 
//	int T; cin >> T;
//	while (T--) {
//		scanf("%d", &n);
//		Trie trie;
//		ans = 0;
//		for (int i = 0; i < n; ++i) {
//			scanf("%s", tmp);
//			trie.insert(tmp);
//		}
//		getFail();
//		scanf("%s", text);
//		find_T(text);
//		cout << ans << endl;
//	}
//    return 0;
//}

