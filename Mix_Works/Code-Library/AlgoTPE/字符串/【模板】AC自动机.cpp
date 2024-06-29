#include <bits/stdc++.h>
using namespace std;
#define MAXC 26
//���ṹ
struct ACNode {
public:
	ACNode *fail;		//failָ��,ָ��͵�ǰ����ĳ����׺ƥ����ǰ׺��λ��
	//����KMP�е��ǰ��׺�ĸ���,��Ŀ�괮��trie���Ͻ���ƥ���ʱ��,�����P�����ƥ��ʧ��,��ôӦ������P->fail����ƥ�䣬
	//�������ʧ��,��ô����P->fail->fail����ƥ��,�������failָ��Ĺ��죬���Ļ���ϸ����
	ACNode *next[MAXC]; //�ӽ��ָ��,ÿ�������ӽ����������ַ������ַ����Ĵ�С
	//���µ�������Բ�ͬ����Ŀ�����ʵ�ע�ͺ�ѡ��,��Ϊ�ڴ�����
	int id;  //�����,ÿ�������Ψһ,����״̬ת�Ƶ�ʱ����±�ӳ��
	int val; //��ǰ��������failָ��ָ����Ľ�β������Ϣ�ļ��� 
	int cnt; //��Щ����ģʽ�������ж����������ͬ��,���Ǽ�����ʱ������
/*
��β��� int cnt, val;
ÿ��ģʽ���ڽ��в���Ĺ����У����ģʽ���������һ�����Եı�������������ϼ���ʾ��������������ϣ��ڽ�β�����Ҫһ����ǣ���ʾ����һ��ģʽ����ĩβ��
��Щ�������ֶ����ͬ��ģʽ����������cnt����ʾ�ô����ֵĴ�����ÿ����һ�ζ�cnt����һ��������
���е����У���ͬ��ģʽ���в�ͬ��Ȩֵ������ģʽ���ĸ�������(<= 15)����ô���Խ��ý���Ƿ���ģʽ����ĩβ--��2��������ʾ��
ѹ���ɶ����Ƶ�������¼��val��(���統ǰ����ǵڶ���ģʽ���͵��ĸ�ģʽ���Ľ�β����val = (1010)2)��
*/
	void reset(int _id) {
		id = _id;
		val = 0;
		cnt = 0;
		fail = nullptr;
		memset(next, 0, sizeof(next));
	}
	
	//״̬���е� ����̬ ����
	//ģʽ��Ϊ���ܳ��ֵ�ʱ����н���̬ 
	bool isReceiving() {
		return val != 0;
	}
};

class ACAutoman {
public:
	//�����ؽṹ
	ACNode *nodes[MAXQ]; //��㻺��,�����ڴ��ظ�������ͷ�,��ʡʱ��
	int nodesMax;		 //��������С,��Զ�ǵ�����
	ACNode *root;		 //�����ָ��
	int nodeCount; 		 //�������
	 
	
};
 
//AC�Զ���2222
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
////AC�Զ���
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

