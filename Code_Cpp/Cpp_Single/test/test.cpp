#include <iostream>
#include <queue>
using namespace std;
const int maxn = 25;
char room[maxn][maxn];
int move[][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // 左上右下
#define check(x, y) ((x < w && x >= 0) && (y < h && y > 0))
int dx, dy;
struct point {int x, y; };
int w, h;

int bfs() {
    queue<point> q;
    int num = 1; // 记录可走的黑色瓷砖数量
    q.push(point{dx, dy}); // 从起点开始
    room[dy][dx] = '#'; // 把起点堵住防止重复计算
    while (!q.empty()) {
        point t = q.top(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int tx = t.x + move[i][0], ty = t.y + move[i][1];
            if (check(tx, ty) && room[tx][ty] == '.') {
                ++num; // 可走的路多了一条
                q.push(point{tx, ty});
                room[dy][dx] = '#';
            }
        }
    }
    return num;
}

int main() {
    char c;
    while (~scanf("%d %d", &w, &h)) {
        if (w == 0 && h == 0) break;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                scanf("%c", &c);
                if (c == '@') dx = j, dy = i;
                room[i][j] = c;
            }
        }
        printf("%d\n", bfs());
    }
    return 0;
}
