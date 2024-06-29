@[toc]

## 开始
本文在弹跳小球、反弹球、生命游戏等的基础上面使用easyx开发一个动态粒子仿真的程序。说是仿真，实际上是许多小球四处弹来弹去。只不过加入了对阻力、引力、斥力等的模拟罢了。阻力是粒子(小球)运动时遇到的，斥力是用鼠标点击(击打)时产生的，引力是鼠标移动时引起的。

我们这里的小球的坐标和速度等都是用浮点数，虽然屏幕只能是二维整形数组。使用浮点数会使得小球的运动更加...有意义。


## 实现过程
### 1. 许多静止的小球
这些小球的位置坐标和速度、半径都是double，结构体的内容都是随机的。
```cpp
#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;

#define WIDTH	  1024
#define HEIGHT  800 
#define MOVER_NUMS 800 // 移动的小球的数量

struct Mover {
	COLORREF color; // 小球颜色
	double x, y;             // 坐标
	double vx, vy;         // 速度
	double radius;        // 半径
};

Mover movers[MOVER_NUMS]; // 小球数组

void startup() {
	srand((unsigned int)time(NULL));

	for (int i = 0; i < MOVER_NUMS; ++i) { // 初始化小球数组
		movers[i].color = RGB(rand() % 256, rand() % 256, rand() % 256); // 随机颜色
		movers[i].x = rand() % WIDTH;
		movers[i].y = rand() % HEIGHT;
		movers[i].vx = cos(double(i)) * (rand() % 34); // 小球有不同的运动方向
		movers[i].vy = cos(double(i)) * (rand() % 34);
		movers[i].radius = (rand() % 34) / 15.0;  // 小球半径不可以太大
	}
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
}

void show() {
	clearrectangle(0, 0, WIDTH - 1, HEIGHT - 1); // 清空画面中的全部矩形区域
	setfillstyle(BS_SOLID);  // 设置当前填充样式
	for (int i = 0; i < MOVER_NUMS; ++i) {
		setlinecolor(movers[i].color); // 设置线形颜色
		setfillcolor(movers[i].color);  // 设置填充颜色
		fillcircle(int(movers[i].x + 0.5), int(movers[i].y + 0.5), int(movers[i].radius + 0.5)); // 四舍五入
	}
	FlushBatchDraw();
	Sleep(50);
}

void updateWithoutInput() {

}

void updateWithInput() {


}

void gameover() {
	EndBatchDraw();
	closegraph();
}
int main() {
	startup();  // 游戏初始化
	while (1) {
		show();  // 显示画面
		updateWithoutInput(); // 与用户输入无关的更新
		updateWithInput();      // 与用户输入有关的更新
	}
	gameover();
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019102615492743.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 2. 小球的运动与反弹
反弹球的思路，更新小球的坐标。遇到边界便改变方向。
```cpp
···
void updateWithoutInput() {
	for (int i = 0; i < MOVER_NUMS; ++i) {
		double x = movers[i].x, y = movers[i].y;
		double vx = movers[i].vx, vy = movers[i].vy;

		// 根据位置+速度来更新小球的坐标
		double nextX = x + vx, nextY = y + vy;
		// 如果小球将要超过上下左右边界，则设置位置为边界，反转方向
		if (nextX > WIDTH) {
			nextX = WIDTH; vx = -vx;
		}
		else if (nextX < 0) {
			nextX = 0; vx = -vx;
		}
		if (nextY > HEIGHT) {
			nextY = HEIGHT; vy = -vy;
		}
		else if (nextY < 0) {
			nextY = 0; vy = -vy;
		}
		// 更新小球的位置和速度
		movers[i].vx = vx;
		movers[i].vy = vy;
		movers[i].x = nextX;
		movers[i].y = nextY;
	}
}
···
```
效果就不放了，反正放图也是静态的。

### 3. 模拟小球运动遇到的阻力
加入全局的摩擦力系数FRICTION，每次更新速度，使之减缓。最后很快停止运动。为避免这种情况，小球运动速度过小时就使其增大，同时修改小球的半径，速度越大小球半径越大。
```cpp
...
#define FRICTION 0.96f       // 摩擦力系数
...
void updateWithoutInput() {
	for (int i = 0; i < MOVER_NUMS; ++i) { 
		...
		// 小球运动有一个摩擦力，速度逐渐减小
		vx *= FRICTION;
		vy *= FRICTION;

		double absVX = fabs(vx), absVY = fabs(vy);  // 速度的绝对值
		double avgV = (absVX + absVY) * 0.5;        // 两个方向速度的平均值
		double sc = avgV * 0.45;
		sc = max(min(sc, 3.5), 0.4); // 小球半径在0.4-3.5之间，速度越大半径越大
		movers[i].radius = sc;       // 更新小球半径

		// 速度过小时，乘以一个0-35的随机数
		if (absVX < 0.35) vx = vx * double(rand()) / RAND_MAX * 40;
		if (absVY < 0.35) vy = vy * double(rand()) / RAND_MAX * 40;
		...
	}
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191026225513729.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 4. 模拟鼠标的吸引力
一个鼠标坐标的结构体，记录鼠标的坐标。当发生鼠标移动事件时，增加一定范围内小球的向心力，距离鼠标距离越短，引力越大。
```cpp
...
#define MOVER_NUMS 1200 // 移动的小球的数量
...
struct MousePosition { // 鼠标
	double x, y;  // 坐标
};
MousePosition mouse;

void startup() {
	...
	mouse.x = WIDTH / 2, mouse.y = HEIGHT / 2;
	...
}

void updateWithoutInput() {
	double MaxDist = 0.85 * WIDTH;  // 吸引距离，小球与鼠标的距离在这个范围内就会受到吸引
	for (...) {
		...
		double dx = x - mouse.x, dy = y - mouse.y;
		double d = sqrt(dx * dx + dy * dy);  // 当前小球位置和鼠标位置的距离
		if (d != 0) { // 归一化，仅反映方向
			dx = dx / d, dy = dy / d;
		} 
		else {
			dx = dy = 0;
		}
		if (d < MaxDist) { // 在此范围内，受到吸引力
			// 吸引力引起的加速度幅度，距离越近越大
			double toAcc = (1 - d / MaxDist) * WIDTH * 0.0014;
			vx = vx - dx * toAcc; // dx、dy仅反映方向信息
			vy = vy - dy * toAcc;
		}
		.
	}
}

void updateWithInput() {
	MOUSEMSG m;  // 定义鼠标消息
	while (MouseHit()) { // 当有鼠标消息时
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE) { 
			// 如果鼠标移动，更新当前鼠标坐标变量
			mouse.x = m.x;
			mouse.y = m.y;
		}
	}
}
...
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191027113110871.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 5. 模拟鼠标的击打斥力
鼠标左键按下时会产生一个力，对一定范围内的小球产生击打斥力，距离越近影响越大。
```cpp
...
bool isMouseLeftDown; // 鼠标左键是否落下
...
void startup() {
	...
	isMouseLeftDown = false;
	...
}
...
void updateWithoutInput() {
	double toMaxDist = 0.80 * WIDTH;  // 吸引距离，小球与鼠标的距离在这个范围内就会受到吸引
	double blowMaxDist = 0.5 * WIDTH; // 排斥距离，小球与鼠标的距离在这个范围内，左键点击就会被排斥
	...
	for (int i = 0; i < MOVER_NUMS; ++i) {
		...
		// 鼠标点击并且小球在这个范围内，会有斥力
		if (isMouseLeftDown && d < blowMaxDist) {
			// 击打力带来的加速度幅度，距离越近越大
			double blowAcc = (1 - d / blowMaxDist) * HEIGHT * 0.14; // 斥力比引力大才会推开小球
			// 由上面得到的小球的方向信息，更新小球的速度
			double bias = 0.5 - (double(rand()) / RAND_MAX); // 扰动，产生一个-0.5到0.5的波动幅度
			vx = vx + dx * blowAcc + bias, vy = vy + dy * blowAcc + bias;
		}
		...
	}
}

void updateWithInput() {
	MOUSEMSG m;  // 定义鼠标消息
	while (MouseHit()) { // 当有鼠标消息时
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE) { // 如果鼠标移动，更新当前鼠标坐标变量
			mouse.x = m.x, mouse.y = m.y;
		}
		else if (m.uMsg == WM_LBUTTONDOWN) { // 鼠标左键落下
			isMouseLeftDown = true;
		}
		else if (m.uMsg == WM_LBUTTONUP) { // 鼠标左键抬起
			isMouseLeftDown = false;
		}
	}
}
...
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191027134807888.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 6. 模拟鼠标的扰动力 
当鼠标划过的时候，会对两侧的小球产生一个扰动力，将其排开。实现方法和前面的差不多。完整代码如下：

```cpp
#include <graphics.h>
#include <conio.h>
#include <cstdio>
#include <windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;

#define WIDTH	  1024
#define HEIGHT  800 
#define MOVER_NUMS 1200 // 移动的小球的数量
#define FRICTION 0.96f  // 摩擦力系数

struct Mover {
	COLORREF color; // 小球颜色
	double x, y;          // 坐标
	double vx, vy;        // 速度
	double radius;        // 半径
};

struct MousePosition { // 鼠标
	double x, y;  // 坐标
};
bool isMouseLeftDown; // 鼠标左键是否落下
double mouseVx, mouseVy; // 鼠标移动的速度
Mover movers[MOVER_NUMS]; // 小球数组
MousePosition mouse, premouse; // 现在的鼠标和上次的鼠标

void startup() {
	srand((unsigned int)time(NULL));

	for (int i = 0; i < MOVER_NUMS; ++i) { // 初始化小球数组
		movers[i].color = RGB(rand() % 256, rand() % 256, rand() % 256); // 随机颜色
		movers[i].x = rand() % WIDTH;
		movers[i].y = rand() % HEIGHT;
		movers[i].vx = cos(double(i)) * (rand() % 34); // 小球有不同的运动方向
		movers[i].vy = cos(double(i)) * (rand() % 34);
		movers[i].radius = (rand() % 34) / 12.0;  // 初始小球半径不可以太大
	}
	//初始化鼠标变量，当前鼠标和上次鼠标都在中心
	mouse.x = premouse.x = WIDTH / 2, mouse.y = premouse.y = HEIGHT / 2;
	isMouseLeftDown = false; // 初始鼠标未落下

	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
}

void show() {
	clearrectangle(0, 0, WIDTH - 1, HEIGHT - 1); // 清空画面中的全部矩形区域
	setfillstyle(BS_SOLID);  // 设置当前填充样式
	for (int i = 0; i < MOVER_NUMS; ++i) {
		setlinecolor(movers[i].color); // 设置线形颜色
		//setfillcolor(WHITE);  // 设置填充颜色
		fillcircle(int(movers[i].x + 0.5), int(movers[i].y + 0.5), int(movers[i].radius + 0.5)); // 四舍五入
	}
	FlushBatchDraw();
	Sleep(50);
}

void updateWithoutInput() {
	double toMaxDist = 0.86 * WIDTH;  // 吸引距离，小球与鼠标的距离在这个范围内就会受到吸引
	double blowMaxDist = 0.5 * WIDTH; // 排斥距离，小球与鼠标的距离在这个范围内，左键点击就会被排斥
	double stirMaxDist = 0.125 * WIDTH; // 扰动距离，小球与鼠标的距离在这个范围内，就会受到鼠标的扰动

	// 前后两次运行间鼠标移动的距离即为鼠标的速度
	mouseVx = mouse.x - premouse.x, mouseVy = mouse.y - premouse.y; 
	// 更新上次的鼠标变量
	premouse.x = mouse.x, premouse.y = mouse.y;

	for (int i = 0; i < MOVER_NUMS; ++i) {
		double x = movers[i].x, y = movers[i].y;
		double vx = movers[i].vx, vy = movers[i].vy;

		// 鼠标移动会有引力
		double dx = x - mouse.x, dy = y - mouse.y;
		double d = sqrt(dx * dx + dy * dy);  // 当前小球位置和鼠标位置的距离
		if (d != 0) { // 归一化，仅反映方向
			dx = dx / d, dy = dy / d;
		} 
		else {
			dx = dy = 0;
		}
		if (d < toMaxDist) { // 在此范围内，受到吸引力
			// 吸引力引起的加速度幅度，距离越近越大
			double toAcc = (1 - d / toMaxDist) * WIDTH * 0.0014;
			vx = vx - dx * toAcc, vy = vy - dy * toAcc; // dx、dy仅反映方向信息 
		}
		
		// 鼠标点击并且小球在这个范围内，会有斥力
		if (isMouseLeftDown && d < blowMaxDist) {
			// 击打力带来的加速度幅度，距离越近越大
			double blowAcc = (1 - d / blowMaxDist) * HEIGHT * 0.006; // 斥力比引力大才会推开小球
			// 由上面得到的小球的方向信息，更新小球的速度
			double bias = 0.5 - (double(rand()) / RAND_MAX); // 扰动，产生一个-0.5到0.5的波动幅度
			vx = vx + dx * blowAcc + bias, vy = vy + dy * blowAcc + bias;
		}
		// 距离小于扰动距离时
		if (d < stirMaxDist) {
			// 扰动力引起的加速度幅度，越近引起的加速度越大，扰动力的值越大
			double sAcc = (1 - (d / stirMaxDist)) * WIDTH * 0.00026f;
			// 鼠标速度越快，引起的扰动力越大
			vx = vx + mouseVx * sAcc, vy = vy + mouseVy * sAcc;
		}
		// 小球运动有一个摩擦力，速度逐渐减小
		vx *= FRICTION, vy *= FRICTION;

		double absVX = fabs(vx), absVY = fabs(vy);  // 速度的绝对值
		double avgV = (absVX + absVY) * 0.5;           // 两个方向速度的平均值
		double sc = avgV * 0.58;
		sc = max(min(sc, 3.8), 0.4); // 小球半径在0.4-3.8之间，速度越大半径越大
		movers[i].radius = sc;         // 更新小球半径

		// 因为有阻力的影响，速度过小时，乘以一个0-5的随机数使其变大
		if (absVX < 0.35) vx = vx * double(rand()) / RAND_MAX * 5;
		if (absVY < 0.35) vy = vy * double(rand()) / RAND_MAX * 5;

		// 根据位置+速度来更新小球的坐标
		double nextX = x + vx, nextY = y + vy;
		// 如果小球将要超过上下左右边界，则设置位置为边界，反转方向
		if (nextX > WIDTH) {
			nextX = WIDTH; vx = -vx;
		}
		else if (nextX < 0) {
			nextX = 0; vx = -vx;
		}
		if (nextY > HEIGHT) {
			nextY = HEIGHT; vy = -vy;
		}
		else if (nextY < 0) {
			nextY = 0; vy = -vy;
		}
		// 更新小球的位置和速度
		movers[i].vx = vx,     movers[i].vy = vy;
		movers[i].x = nextX, movers[i].y = nextY;
	}
}

void updateWithInput() {
	MOUSEMSG m;  // 定义鼠标消息
	while (MouseHit()) { // 当有鼠标消息时
		m = GetMouseMsg();
		if (m.uMsg == WM_MOUSEMOVE) { // 如果鼠标移动，更新当前鼠标坐标变量
			mouse.x = m.x, mouse.y = m.y;
		}
		else if (m.uMsg == WM_LBUTTONDOWN) { // 鼠标左键落下
			isMouseLeftDown = true;
		}
		else if (m.uMsg == WM_LBUTTONUP) { // 鼠标左键抬起
			isMouseLeftDown = false;
		}
	}
}

void gameover() {
	EndBatchDraw();
	closegraph();
}

int main() {
	startup();  // 游戏初始化
	while (1) {
		show();  // 显示画面
		updateWithoutInput(); // 与用户输入无关的更新
		updateWithInput();      // 与用户输入有关的更新
	}
	gameover();
	return 0;
}
```
