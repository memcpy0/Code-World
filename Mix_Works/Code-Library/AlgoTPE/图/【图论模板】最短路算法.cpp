#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXN = 500100;
const ll inf = 0x7fffffff;
ll n, m, s, u, v, w;
//���·���ľ���; ���·����; ��Ȩ/��Ȩ֮��; ���·+DFS 
struct edge {
	ll from, to, w;
	edge(ll f, ll _t, ll _w) { from = f; to = _t; w = _w; }
};
vector<edge> adj[MAXN];

struct s_node {
	ll id, n_dis;
	s_node(int i, int d) { id = i; n_dis = d; }
	bool operator < (const s_node &a) const { return n_dis > a.n_dis; }
};

bool vis[MAXN];
ll d[MAXN];
void dijkstra(ll s) {
	for (int i = 1; i <= n; ++i) { vis[i] = false; d[i] = inf; }
	d[s] = 0;
	priority_queue<s_node> Q;
	Q.push(s_node(s, d[s]));
	
	while (!Q.empty()) {
		s_node v = Q.top(); Q.pop();
		if (vis[v.id]) continue; //�����Ѿ����ʹ��ҵ����·���Ľ�� 
		vis[v.id] = true;
		for (int i = 0; i < adj[v.id].size(); ++i) {
			edge y = adj[v.id][i];
			if (vis[y.to]) continue;
			if (y.w + v.n_dis < d[y.to]) {
				d[y.to] = y.w + v.n_dis;
				Q.push(s_node(y.to, d[y.to]));
			}
		}
	}
	for (int i = 1; i <= n; ++i) {
		if (i == 1) printf("%lld", d[i]);
		else printf(" %lld", d[i]); 
	}
} 
int reachableNodes(vector<vector<int>>& edges, int m, int n) {
    typedef pair<int, int> PII;
    vector<vector<PII>> g(n);
    for(auto& e : edges) {
        int a = e[0], b = e[1], c = e[2];
        g[a].push_back({b, c});
        g[b].push_back({a, c});
    }
    vector<int> dist(n, 0x3f3f3f3f);
    vector<bool> st(n);
    dist[0] = 0;
    priority_queue<PII, vector<PII>, greater<PII>> q;
    q.push({0, 0});
    while(q.size()) {
        auto t = q.top(); q.pop();
        int i = t.second, d = t.first;
        if(st[i]) continue;
        st[i] = true;
        for(auto& [j, w]: g[i])
            if(dist[j] > d + 1 + w) {
                dist[j] = d + 1 + w;
                q.push({dist[j], j});
            }
    } 
} 
//---------------------------------------------------------
int main() {
	scanf("%lld%lld%lld", &n, &m, &s);
	for (int i = 0; i < m; ++i) {
		scanf("%lld%lld%lld", &u, &v, &w);
		adj[u].push_back(edge(u, v, w));
	}
	dijkstra(s);
	return 0;
}
//-------------�������·��-------------------
//����+·����Ȩ�����
#include <bits/stdc++.h>
using namespace std;
const int maxn = 510, inf = 0x3f3f3f3f;
int n, m, s, e;        //n������0~n-1,m����·,���s,�յ�e
int c1, c2, len;       //����1,����2,����
int rescueTeams[maxn]; //0~n-1ÿ�����еļ��ȶ�����
int dis[maxn];         //ÿ�����е����ľ���
bool vis[maxn];        //�Ƿ񱻷��ʹ�
struct edge { int to, w; }; //w�Ǿ���
vector<edge> lst[maxn]; 
struct s_node {
    int to, w;
    s_node() {}
    s_node(int _t, int _w) : to(_t), w(_w) {}
    bool operator<(const s_node& a) const {
        return w > a.w;      
    }
};
int num[maxn], weight[maxn]; //��s���ﶥ��i�����·������Ŀ,��s���ﶥ��i����༱�ȶ�����
void dijkstra(int s, int e) { 
    fill(dis, dis + maxn, inf);
    fill(vis, vis + maxn, false);
    fill(num, num + maxn, 0);
    fill(weight, weight + maxn, 0);
    dis[s] = 0; weight[s] = rescueTeams[s]; num[s] = 1;
    priority_queue<s_node> pq;
    pq.push(s_node(s, 0)); //���
    while (!pq.empty()) {
        s_node t = pq.top(); pq.pop(); //�ҵ����뿪ʼ������̵ĳ���
        int u = t.to;
        if (vis[u]) continue;
        vis[u] = true; 
        for (int j = 0; j < lst[u].size(); ++j) {
            int v = lst[u][j].to, w = lst[u][j].w; 
            if (vis[v]) continue;      //�����ʹ��õ�
            if (dis[v] > dis[u] + w) { //��uΪ�н�����ʹv��s�ľ������
                dis[v] = dis[u] + w;   //���¾���
                num[v] = num[u];       //�̳�s����u�����·��Ŀ
                weight[v] = weight[u] + rescueTeams[v]; //��·����̵������,ʹ�þ�Ԯ���������,���µ�Ȩ
                pq.push(s_node(v, dis[v]));
            } else if (dis[v] == dis[u] + w) {//���ø��¾��� 
                num[v] += num[u];      //�����µ����·��Ŀ 
                if (weight[u] + rescueTeams[v] > weight[v]) 
                    weight[v] = weight[u] + rescueTeams[v]; //�ж��Ƿ���Եõ�����ĵ�Ȩ
                pq.push(s_node(v, dis[v]));
            } 
        }
    }
}

int main() {
    scanf("%d%d%d%d", &n, &m, &s, &e);
    for (int i = 0; i < n; ++i) scanf("%d", &rescueTeams[i]);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &c1, &c2, &len);
        lst[c1].push_back(edge{c2, len});
        lst[c2].push_back(edge{c1, len});
    } 
    dijkstra(s, e);
    printf("%d %d", num[e], weight[e]);
    return 0;
} 
