//
//#include <iostream>
//#include <algorithm>
//#include <string>
//#include <cstring>
//#include <cstdio>
//using namespace std;
//
//struct trieNode {
//    trieNode *next[26]; // ������ܴ��ڵ�26���ַ� a-z
//    int num; // �Ը��ַ�Ϊǰ׺���ַ�������
//    int forknum; // �ý����ӽ��/��֧����
//    trieNode() {
//        for (int i = 0; i < 26; i++) next[i] = nullptr;
//        num = forknum = 0;
//    }
//
//};
//
//trieNode trie; // ����㣬�������ַ�
//
//void insert(char s[]) {
//    trieNode *p = &trie;
//    for (int i = 0; s[i]; i++) { // ���ַ������뵽�ֵ�����
//        if (p->next[s[i] - 'a'] == nullptr) { // �ӽ��û������ַ�ʱ
//            p->next[s[i] - 'a'] = new trieNode; // ����һ��
//            p->forknum++; // ��֧������1
//        }
//        p = p->next[s[i] - 'a']; // ������һ��
//        p->num++;
//    }
//}
//
//int find(char s[]) { // �������ַ���sΪǰ׺�ĵ��ʵ�����
//    trieNode *p = &trie;
//    for (int i = 0; s[i]; i++) { // ���ֵ������ҵ��õ��ʵĽ�βλ��
//        if (p->next[s[i] - 'a'] == nullptr)
//            return 0;
//        p = p->next[s[i] - 'a'];
//    }
//    return p->num;
//}
//
//string getLcp() {
//    trieNode *p = &trie;
//    string s;
//    while (p) {
//        if (p->forknum == 1) {
//            for (int i = 0; i < 26; i++) {
//                if (p->next[i] != nullptr) {
//                    s += char('a' + i);
//                    p = p->next[i];
//                    break;
//                }
//            }
//        }
//        else return s;
//    }
//    return s;
//}
//int main(){
////    char str[11];
////    while (gets(str)) {
////        if (!strlen(str)) break;
////        insert(str);
////    }
//    int t = 0;
//    (t += 20) += 4;
//    cout << t << endl;
////    while (gets(str)) {
////        if (!strlen(str)) break;
////        cout << find(str) <<endl;
////    }
////    cout << getLcp() << endl;
//    return 0;
//}

#include <iostream>
#include <cstring>
#include <map>
#include <queue>
#include <string>
#include <vector>
#define inf (-1)
using namespace std;
const int maxn = 1010;
vector<int> adjlist[maxn];
map<string, int> mp;
int visited[maxn];
int n, m, idx = 0;

int change(const string &s) {
	return mp[s];
}

int bfs(int s) {
	int len = -1; // �ʼ-1
	queue<int> q;
	q.push(s);
	visited[s] = 1;
	while (!q.empty()) {
		int v = q.front(); q.pop();
		++len; // ÿһ��+1
		for (int i = 0; i < adjList[v].size(); ++i)
			if (!visited(adjList[v][i])
				q.push(adjList[v][i]);
	}
	return len;
}
int bfsTraverse() {
	memset(visited, 0, sizeof(visited));
	return bfs(0);
}

int main() {
	string name, a, b;
	while (~scanf("%d", &n)) {
		idx = 0;
		if (!n) break;
		for (int i = 0; i < n; ++i) {
			cin >> name;
			mp[name] = idx++;
		}
		scanf("%d", &m);
		for (int i = 0; i < m; ++i) {
			cin >> a >> b;
			int ia = change(a), ib = change(b);
			adjList[ia].push_back(change(ib));
			adjList[ib].push_back(change(ia));
		}
		printf("%d\n", bfsTraverse());

		for (int i = 0; i < maxn; ++i)
			adjList.clear();
		mp.clear();
	}
	return 0;
}

