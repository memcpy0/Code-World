#include <bits/stdc++.h>
using namespace std;
const int INF = 0x3fffffff;

int n, m, s, t; //sԴ��,t���
int dfs(int p = s, int flow = INF) {
	if (p == t) 
		return flow;  //�����յ�,������������·������
	vis[p] = true;    //���� 
	for (int eg = head[p]; eg; eg = edges[eg].next) {
		int to = edges[eg].to, vol = edges[eg].w, c;
		//���ص������ǲ�����������0,δ���ʸõ�,�ҽ��������Եݹ�ĵ����յ�
		//�����µ������Ǳߵ������͵�ǰ�����Ľ�Сֵ 
		if (vol > 0 && !vis[to] && (c = dfs(to, min(vol, flow)) != -1)) {
			edges[eg].w -= c;
			edges[eg ^ 1].w += c;
			//��ʽǰ����ȡ����ߵ�һ�ּ�д��
			//��ͼʱҪ��cnt��Ϊ1,��Ҫ��֤����߽���������߽��� 
			return c;
		}
	}
	return -1; //�޷������յ� 
}
	
inline int FF() {
	int ans = 0, c;
	while ((c = dfs()) != -1) {
		memset(vis, 0, sizeof(vis));
		ans += c;
	}
	return ans;
}
