/*
13.11 写一个回溯算法求解马周游问题：给定一个8x8的棋盘，一个放在棋盘某个位置上的马是否可
以恰好访问每个方格一次，并回到起始位置上？ 
 
题意：本题求马的周游闭路，等价于询问马的Hamiltonl回路是否存在。  
采取dfs+启发函数，用给出坐标的下一步可达的未被访问的方向数目作为代价。
策略：先对下一步可达的未被访问的方向数目最少的方向进行dfs
即选择代价最小的先搜索，后继产生的结点就更少，相对来说能够提高一定的速度。 

事实上如果对于8x8的棋盘存在有一条周游闭路，那么从任一点出发都能够无重复地访问其他所有位置并回到原点. 
*/
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm> 
using namespace std;
const int N = 8, M = 8; 
bool visit[N][M]; //判断是否访问过 
int tempBoard[N][M]; //存放棋盘每个位置走的次序 
int fx, fy; //起点坐标 
int direction[8][2] ={{-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}};

struct grid { //棋盘当前坐标(x,y)和下一步可走的方向数目 
	int x, y, cnt;
};

bool cmp(const grid &a, const grid &b) { return a.cnt < b.cnt; }

bool isLegal(int x, int y) { //判断下一步是否可以访问 
	return (x >= 0 && x < 8 && y >= 0 && y < 8 && visit[x][y] == false);
}

int sumCount(int x, int y) { //统计给出坐标下一步可走的方向数目  
	int cnt = 0;
	for (int i = 0; i < 8; ++i) {
		int xi = x + direction[i][0], yi = y + direction[i][1];
		if (isLegal(xi, yi)) ++cnt;
	}
	return cnt;
}

int produceCosts(grid costs[], int x, int y) { //对当前位置(x,y)的下一步(xx,yy)做取舍 
	int idx = 0;
	for (int i = 0; i < 8; ++i) { 
		int xx = x + direction[i][0], yy = y + direction[i][1];  
		if (isLegal(xx, yy)) {
			costs[idx].x = xx;
			costs[idx].y = yy;
			costs[idx].cnt = sumCount(xx, yy); //(xx,yy)下一步可达的方向的数目 
			++idx;
		}
	}
 	//如果当前位置(x,y)的下一步(xx,yy)，其所达的下一步方向数目越少，就越先访问 
	sort(costs, costs + idx, cmp);
	return idx;
}
	 
bool dfs(int x, int y, int k) {
	tempBoard[x][y] = k;
	if (k == N * M) { //如果全部走遍 
		for (int i = 0; i < 8; ++i) {
			int nextX = x + direction[i][0], nextY = y + direction[i][1]; 
			if (nextX == fx && nextY == fy) {  //并且再走一步就可以回到起点  
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
		if (dfs(costs[i].x, costs[i].y, k + 1)) //找到就返回 
		 	return true;
	}
	visit[x][y] = false;
	return false;
}

int main() { 
	while (~scanf("%d%d", &fx, &fy)) {
		memset(visit, false, sizeof(visit)); //访问矩阵置为false 
		dfs(fx, fy, 1);
	}
	return 0;
}
/*
对于输入为1 1，结果如下： 
 19  16  51   2  21   6  45   4
 50   1  20  17  52   3  22   7
 15  18  49  56  23  44   5  46
 64  57  24  53  48  55   8  31
 25  14  63  58  43  32  47  36
 62  59  42  33  54  37  30   9
 13  26  61  40  11  28  35  38
 60  41  12  27  34  39  10  29
对于0 3，结果如下：
 40  37  16   1  54  23  14  21
 17   2  39  42  15  20  55  24
 38  41  36  19  64  53  22  13
  3  18  47  62  43  56  25  52
 46  35  44  57  48  63  12  29
  7   4  59  32  61  28  51  26
 34  45   6   9  58  49  30  11
  5   8  33  60  31  10  27  50 
因此对8x8的棋盘存在哈密尔顿回路。 一个放在棋盘某个位置上的马是否可以恰好访问每个方格一次，并回到起始位置上。 
*/
