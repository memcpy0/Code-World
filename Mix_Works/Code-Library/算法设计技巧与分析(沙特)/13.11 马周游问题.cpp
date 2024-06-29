/*
13.11 дһ�������㷨������������⣺����һ��8x8�����̣�һ����������ĳ��λ���ϵ����Ƿ��
��ǡ�÷���ÿ������һ�Σ����ص���ʼλ���ϣ� 
 
���⣺������������α�·���ȼ���ѯ�����Hamiltonl��·�Ƿ���ڡ�  
��ȡdfs+�����������ø����������һ���ɴ��δ�����ʵķ�����Ŀ��Ϊ���ۡ�
���ԣ��ȶ���һ���ɴ��δ�����ʵķ�����Ŀ���ٵķ������dfs
��ѡ�������С������������̲����Ľ��͸��٣������˵�ܹ����һ�����ٶȡ� 

��ʵ���������8x8�����̴�����һ�����α�·����ô����һ��������ܹ����ظ��ط�����������λ�ò��ص�ԭ��. 
*/
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm> 
using namespace std;
const int N = 8, M = 8; 
bool visit[N][M]; //�ж��Ƿ���ʹ� 
int tempBoard[N][M]; //�������ÿ��λ���ߵĴ��� 
int fx, fy; //������� 
int direction[8][2] ={{-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}};

struct grid { //���̵�ǰ����(x,y)����һ�����ߵķ�����Ŀ 
	int x, y, cnt;
};

bool cmp(const grid &a, const grid &b) { return a.cnt < b.cnt; }

bool isLegal(int x, int y) { //�ж���һ���Ƿ���Է��� 
	return (x >= 0 && x < 8 && y >= 0 && y < 8 && visit[x][y] == false);
}

int sumCount(int x, int y) { //ͳ�Ƹ���������һ�����ߵķ�����Ŀ  
	int cnt = 0;
	for (int i = 0; i < 8; ++i) {
		int xi = x + direction[i][0], yi = y + direction[i][1];
		if (isLegal(xi, yi)) ++cnt;
	}
	return cnt;
}

int produceCosts(grid costs[], int x, int y) { //�Ե�ǰλ��(x,y)����һ��(xx,yy)��ȡ�� 
	int idx = 0;
	for (int i = 0; i < 8; ++i) { 
		int xx = x + direction[i][0], yy = y + direction[i][1];  
		if (isLegal(xx, yy)) {
			costs[idx].x = xx;
			costs[idx].y = yy;
			costs[idx].cnt = sumCount(xx, yy); //(xx,yy)��һ���ɴ�ķ������Ŀ 
			++idx;
		}
	}
 	//�����ǰλ��(x,y)����һ��(xx,yy)�����������һ��������ĿԽ�٣���Խ�ȷ��� 
	sort(costs, costs + idx, cmp);
	return idx;
}
	 
bool dfs(int x, int y, int k) {
	tempBoard[x][y] = k;
	if (k == N * M) { //���ȫ���߱� 
		for (int i = 0; i < 8; ++i) {
			int nextX = x + direction[i][0], nextY = y + direction[i][1]; 
			if (nextX == fx && nextY == fy) {  //��������һ���Ϳ��Իص����  
				for (int i = 0; i < N; ++i) {
					for (int j = 0; j < M; ++j) 
						printf("%3d ", tempBoard[i][j]);
					printf("\n");
				}
				return true;
			} 
		}
		return false;
	}
	visit[x][y] = true;
	grid costs[8];
	int len = produceCosts(costs, x, y);
	for (int i = 0; i < len; ++i) {
		if (dfs(costs[i].x, costs[i].y, k + 1)) //�ҵ��ͷ��� 
		 	return true;
	}
	visit[x][y] = false;
	return false;
}

int main() { 
	while (~scanf("%d%d", &fx, &fy)) {
		memset(visit, false, sizeof(visit)); //���ʾ�����Ϊfalse 
		dfs(fx, fy, 1);
	}
	return 0;
}
/*
��������Ϊ1 1��������£� 
 19  16  51   2  21   6  45   4
 50   1  20  17  52   3  22   7
 15  18  49  56  23  44   5  46
 64  57  24  53  48  55   8  31
 25  14  63  58  43  32  47  36
 62  59  42  33  54  37  30   9
 13  26  61  40  11  28  35  38
 60  41  12  27  34  39  10  29
����0 3��������£�
 40  37  16   1  54  23  14  21
 17   2  39  42  15  20  55  24
 38  41  36  19  64  53  22  13
  3  18  47  62  43  56  25  52
 46  35  44  57  48  63  12  29
  7   4  59  32  61  28  51  26
 34  45   6   9  58  49  30  11
  5   8  33  60  31  10  27  50 
��˶�8x8�����̴��ڹ��ܶ��ٻ�·�� һ����������ĳ��λ���ϵ����Ƿ����ǡ�÷���ÿ������һ�Σ����ص���ʼλ���ϡ� 
*/
