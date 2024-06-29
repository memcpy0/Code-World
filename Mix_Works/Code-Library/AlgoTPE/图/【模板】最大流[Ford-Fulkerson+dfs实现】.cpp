#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3fffffff;

int n, m, s, t; //s源点,t汇点
int dfs(int p = s, int flow = INF) {
	if (p == t) 
		return flow;  //到达终点,返回这条增广路的流量
	vis[p] = true;    //访问 
	for (int eg = head[p]; eg; eg = edges[eg].next) {
		int to = edges[eg].to, vol = edges[eg].w, c;
		//返回的条件是残余容量大于0,未访问该点,且接下来可以递归的到达终点
		//传递下的流量是边的容量和当前流量的较小值 
		if (vol > 0 && !vis[to] && (c = dfs(to, min(vol, flow)) != -1)) {
			edges[eg].w -= c;
			edges[eg ^ 1].w += c;
			//链式前向星取反向边的一种简单写法
			//建图时要把cnt置为1,且要保证反向边紧接着正向边建立 
			return c;
		}
	}
	return -1; //无法到达终点 
}
	
inline int FF() {
	int ans = 0, c;
	while ((c = dfs()) != -1) {
		memset(vis, 0, sizeof(vis));
		ans += c;
	}
	return ans;
}
