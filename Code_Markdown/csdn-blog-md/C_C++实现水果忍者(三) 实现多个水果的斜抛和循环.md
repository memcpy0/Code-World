@[toc]
### 要用到的素材
从左到右依次是fruit.png、fruit.bmp、fruit_mask.bmp。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091111914.png) ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091128735.bmp)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091146990.bmp)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091249925.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091428552.bmp) ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091449258.bmp)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091458817.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111309150573.bmp)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091512856.bmp)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091520611.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091526500.bmp)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091530634.bmp)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091630614.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091637605.bmp)![在这里插入图片描述](https://img-blog.csdnimg.cn/20191113091641800.bmp)

---
### 1. 实现多个水果的斜抛
水果个数要随机，抛出的水果要随机，虽然这样说，但是先随便写一下，后面再改(下面的纯属于搬砖)：
```cpp
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <stdlib.h>
#define WIDTH 640
#define HEIGHT 480

double v0 = -1; 
double g = 0.000009;
double fruits_x[10], fruits_y[10]; // 记录水果坐标
double vx, vy;  // 水果的水平和竖直运动速度
double time_tick; 
int fruits_n1, fruits_n2; // 随机生成水果的个数

IMAGE img_bk;
IMAGE img_apple, img_apple_mask;       // 苹果图片和遮罩图
IMAGE img_banana, img_banana_mask; // 香蕉图片和遮罩图
IMAGE img_melon, img_melon_mask;     // 西瓜图片和遮罩图
IMAGE img_peach, img_peach_mask;      // 桃子图片和遮罩图
IMAGE img_berry, img_berry_mask;       // 草莓图片和遮罩图

void startUp() {
	srand((unsigned int)time(NULL));
	// 各项数据初始化
	vy = v0;   // 竖直方向初速度(匀变速)
	vx = 0.1; // 水平方向初速度(不变)
	time_tick = 1; 
	for (int i = 0; i < 10; ++i) {
		fruits_x[i] = 10 + (rand() % (WIDTH - 20));
		fruits_y[i] = HEIGHT + 10;
	}  
	fruits_n1 = 1 + rand() % 5;
	fruits_n2 = 1 + rand() % 5;
	// 创建绘图界面和加载图片
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
	loadimage(&img_bk, _T("../background.jpg"), WIDTH, HEIGHT); 

	loadimage(&img_apple, _T("../apple.bmp"));
	loadimage(&img_apple_mask, _T("../apple_mask.bmp"));

	loadimage(&img_banana, _T("../banana.bmp"));
	loadimage(&img_banana_mask, _T("../banana_mask.bmp"));

	loadimage(&img_melon, _T("../melon.bmp"));
	loadimage(&img_melon_mask, _T("../melon_mask.bmp"));

	loadimage(&img_peach, _T("../peach.bmp"));
	loadimage(&img_peach_mask, _T("../peach_mask.bmp"));

	loadimage(&img_berry, _T("../strawberry.bmp"));
	loadimage(&img_berry_mask, _T("../strawberry_mask.bmp"));
}

void show() {
	putimage(0, 0, &img_bk); // 之所以每次都绘制背景图，是为了挡住前面的水果图
 
	if (fruits_n1 == 1) {   
		putimage(fruits_x[0], fruits_y[0], &img_apple_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_apple, SRCPAINT);
	} else if (fruits_n1 == 2) {
		putimage(fruits_x[1], fruits_y[1], &img_banana_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_banana, SRCPAINT);

		putimage(fruits_x[0], fruits_y[0], &img_apple_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_apple, SRCPAINT);
	} else if (fruits_n1 == 3) {
		putimage(fruits_x[0], fruits_y[0], &img_banana_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_banana, SRCPAINT);

		putimage(fruits_x[1], fruits_y[1], &img_apple_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_apple, SRCPAINT);

		putimage(fruits_x[2], fruits_y[2], &img_melon_mask, SRCAND);
		putimage(fruits_x[2], fruits_y[2], &img_melon, SRCPAINT); 
	} else if (fruits_n1 == 4) {
		putimage(fruits_x[0], fruits_y[0], &img_banana_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_banana, SRCPAINT);

		putimage(fruits_x[1], fruits_y[1], &img_apple_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_apple, SRCPAINT);

		putimage(fruits_x[2], fruits_y[2], &img_melon_mask, SRCAND);
		putimage(fruits_x[2], fruits_y[2], &img_melon, SRCPAINT);

		putimage(fruits_x[3], fruits_y[3], &img_peach_mask, SRCAND);
		putimage(fruits_x[3], fruits_y[3], &img_peach, SRCPAINT);
	} else if (fruits_n1 == 5) {
		putimage(fruits_x[0], fruits_y[0], &img_banana_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_banana, SRCPAINT);

		putimage(fruits_x[1], fruits_y[1], &img_apple_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_apple, SRCPAINT);

		putimage(fruits_x[2], fruits_y[2], &img_melon_mask, SRCAND);
		putimage(fruits_x[2], fruits_y[2], &img_melon, SRCPAINT);

		putimage(fruits_x[3], fruits_y[3], &img_peach_mask, SRCAND);
		putimage(fruits_x[3], fruits_y[3], &img_peach, SRCPAINT);

		putimage(fruits_x[4], fruits_y[4], &img_berry_mask, SRCAND);
		putimage(fruits_x[4], fruits_y[4], &img_berry, SRCPAINT); 
	} 

	if (fruits_n2 == 1) {
		putimage(fruits_x[0], fruits_y[0], &img_apple_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_apple, SRCPAINT);
	} else if (fruits_n2 == 2) {
		putimage(fruits_x[1], fruits_y[1], &img_banana_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_banana, SRCPAINT);

		putimage(fruits_x[0], fruits_y[0], &img_apple_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_apple, SRCPAINT);
	} else if (fruits_n2 == 3) {
		putimage(fruits_x[0], fruits_y[0], &img_banana_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_banana, SRCPAINT);

		putimage(fruits_x[1], fruits_y[1], &img_apple_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_apple, SRCPAINT);

		putimage(fruits_x[2], fruits_y[2], &img_melon_mask, SRCAND);
		putimage(fruits_x[2], fruits_y[2], &img_melon, SRCPAINT);
	} else if (fruits_n2 == 4) {
		putimage(fruits_x[0], fruits_y[0], &img_banana_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_banana, SRCPAINT);

		putimage(fruits_x[1], fruits_y[1], &img_apple_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_apple, SRCPAINT);

		putimage(fruits_x[2], fruits_y[2], &img_melon_mask, SRCAND);
		putimage(fruits_x[2], fruits_y[2], &img_melon, SRCPAINT);

		putimage(fruits_x[3], fruits_y[3], &img_peach_mask, SRCAND);
		putimage(fruits_x[3], fruits_y[3], &img_peach, SRCPAINT);
	} else if (fruits_n2 == 5) {
		putimage(fruits_x[0], fruits_y[0], &img_banana_mask, SRCAND);
		putimage(fruits_x[0], fruits_y[0], &img_banana, SRCPAINT);

		putimage(fruits_x[1], fruits_y[1], &img_apple_mask, SRCAND);
		putimage(fruits_x[1], fruits_y[1], &img_apple, SRCPAINT);

		putimage(fruits_x[2], fruits_y[2], &img_melon_mask, SRCAND);
		putimage(fruits_x[2], fruits_y[2], &img_melon, SRCPAINT);

		putimage(fruits_x[3], fruits_y[3], &img_peach_mask, SRCAND);
		putimage(fruits_x[3], fruits_y[3], &img_peach, SRCPAINT);

		putimage(fruits_x[4], fruits_y[4], &img_berry_mask, SRCAND);
		putimage(fruits_x[4], fruits_y[4], &img_berry, SRCPAINT);
	}
	Sleep(4);
	FlushBatchDraw();
}

void updateWithInput() {

}

void updateWithoutInput() {
	for (int i = 0; i < 10; i++) {
		fruits_x[i] += vx;  // 水平方向匀速运动
		fruits_y[i] = HEIGHT + 1.5  * vy * time_tick;  // 竖直方向匀变速运动
	}
	vy += g * time_tick;
	time_tick++;
}

void gameOver() {
	EndBatchDraw();
	_getch();
	closegraph();
}

int main() {
	startUp();
	while (true) {
		show();
		updateWithInput();
		updateWithoutInput();
	}
	gameOver();
	return 0;
} 
```

### 2. 实现循环斜抛
循环斜抛，实际上只有两轮水果不断循环抛出，第一轮向左抛出，时间片过去后开始第二轮斜抛。如果第一轮水果没有完全消失，就仍会运动。两个时间片过去后，该轮的水果超出屏幕消失。
```cpp
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#define WIDTH 640
#define HEIGHT 480

// 记录水果坐标
double fruits_x1[10], fruits_y1[10]; // 向左发射
double fruits_x2[10], fruits_y2[10]; // 向右发射
int fruits_n1, fruits_n2; // 两批，随机生成水果的个数

int group1 = 1, group2 = 0;
time_t start1, end1, start2, end2; // __int64
double v0 = -1;
double vx1, vy1;  // 水果的水平和竖直运动速度
double vx2, vy2;  
double time_tick1 = 1, time_tick2 = 1;
double g = 0.000004;

IMAGE img_bk;
IMAGE img_apple, img_apple_mask;       // 苹果图片和遮罩图
IMAGE img_banana, img_banana_mask; // 香蕉图片和遮罩图
IMAGE img_melon, img_melon_mask;     // 西瓜图片和遮罩图
IMAGE img_peach, img_peach_mask;      // 桃子图片和遮罩图
IMAGE img_berry, img_berry_mask;       // 草莓图片和遮罩图

void startUp() {
	srand((unsigned int)time(NULL));
	// 各项数据初始化
	for (int i = 0; i < 10; ++i) {
		fruits_x1[i] = 220 + rand() % 200; // 中间部分
		fruits_y1[i] = HEIGHT + 10; // 都不出现
	}
	start1 = time(NULL); // 先从向左发射开始
 
	for (int i = 0; i < 10; ++i) {
		fruits_x2[i] = 220 + rand() % 200; // 中间部分
		fruits_y2[i] = HEIGHT + 10;  // 都不出现
	}
	fruits_n1 = 1 + rand() % 5; // 两批水果随机打印
	fruits_n2 = 1 + rand() % 5;

	vy1 = vy2 = v0;   // 竖直方向初速度(变速)
	vx1 = vx2 = -0.2; // 水平方向初速度(不变) 

	// 创建绘图界面和加载图片
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
	loadimage(&img_bk, _T("../background.jpg"), WIDTH, HEIGHT);

	loadimage(&img_apple, _T("../apple.bmp"));
	loadimage(&img_apple_mask, _T("../apple_mask.bmp"));

	loadimage(&img_banana, _T("../banana.bmp"));
	loadimage(&img_banana_mask, _T("../banana_mask.bmp"));

	loadimage(&img_melon, _T("../melon.bmp"));
	loadimage(&img_melon_mask, _T("../melon_mask.bmp"));

	loadimage(&img_peach, _T("../peach.bmp"));
	loadimage(&img_peach_mask, _T("../peach_mask.bmp"));

	loadimage(&img_berry, _T("../strawberry.bmp"));
	loadimage(&img_berry_mask, _T("../strawberry_mask.bmp"));
}

void show() {
	putimage(0, 0, &img_bk); // 之所以每次都绘制背景图，是为了挡住前面的水果图

	if (fruits_n1 == 1) {
		putimage(fruits_x1[0], fruits_y1[0], &img_apple_mask, SRCAND);
		putimage(fruits_x1[0], fruits_y1[0], &img_apple, SRCPAINT);
		end1 = time(NULL);
	}
	else if (fruits_n1 == 2) {
		putimage(fruits_x1[1], fruits_y1[1], &img_banana_mask, SRCAND);
		putimage(fruits_x1[1], fruits_y1[1], &img_banana, SRCPAINT);

		putimage(fruits_x1[0], fruits_y1[0], &img_apple_mask, SRCAND);
		putimage(fruits_x1[0], fruits_y1[0], &img_apple, SRCPAINT);
		end1 = time(NULL);
	}
	else if (fruits_n1 == 3) {
		putimage(fruits_x1[0], fruits_y1[0], &img_banana_mask, SRCAND);
		putimage(fruits_x1[0], fruits_y1[0], &img_banana, SRCPAINT);

		putimage(fruits_x1[1], fruits_y1[1], &img_apple_mask, SRCAND);
		putimage(fruits_x1[1], fruits_y1[1], &img_apple, SRCPAINT);

		putimage(fruits_x1[2], fruits_y1[2], &img_melon_mask, SRCAND);
		putimage(fruits_x1[2], fruits_y1[2], &img_melon, SRCPAINT);
		end1 = time(NULL);
	}
	else if (fruits_n1 == 4) {
		putimage(fruits_x1[0], fruits_y1[0], &img_banana_mask, SRCAND);
		putimage(fruits_x1[0], fruits_y1[0], &img_banana, SRCPAINT);

		putimage(fruits_x1[1], fruits_y1[1], &img_apple_mask, SRCAND);
		putimage(fruits_x1[1], fruits_y1[1], &img_apple, SRCPAINT);

		putimage(fruits_x1[2], fruits_y1[2], &img_melon_mask, SRCAND);
		putimage(fruits_x1[2], fruits_y1[2], &img_melon, SRCPAINT);

		putimage(fruits_x1[3], fruits_y1[3], &img_peach_mask, SRCAND);
		putimage(fruits_x1[3], fruits_y1[3], &img_peach, SRCPAINT);
		end1 = time(NULL);
	}
	else if (fruits_n1 == 5) {
		putimage(fruits_x1[0], fruits_y1[0], &img_banana_mask, SRCAND);
		putimage(fruits_x1[0], fruits_y1[0], &img_banana, SRCPAINT);

		putimage(fruits_x1[1], fruits_y1[1], &img_apple_mask, SRCAND);
		putimage(fruits_x1[1], fruits_y1[1], &img_apple, SRCPAINT);

		putimage(fruits_x1[2], fruits_y1[2], &img_melon_mask, SRCAND);
		putimage(fruits_x1[2], fruits_y1[2], &img_melon, SRCPAINT);

		putimage(fruits_x1[3], fruits_y1[3], &img_peach_mask, SRCAND);
		putimage(fruits_x1[3], fruits_y1[3], &img_peach, SRCPAINT);

		putimage(fruits_x1[4], fruits_y1[4], &img_berry_mask, SRCAND);
		putimage(fruits_x1[4], fruits_y1[4], &img_berry, SRCPAINT);
		end1 = time(NULL);
	}

	if (fruits_n2 == 1) {
		putimage(fruits_x2[0], fruits_y2[0], &img_apple_mask, SRCAND);
		putimage(fruits_x2[0], fruits_y2[0], &img_apple, SRCPAINT);
		end2 = time(NULL);
	}
	else if (fruits_n2 == 2) {
		putimage(fruits_x2[1], fruits_y2[1], &img_banana_mask, SRCAND);
		putimage(fruits_x2[1], fruits_y2[1], &img_banana, SRCPAINT);

		putimage(fruits_x2[0], fruits_y2[0], &img_apple_mask, SRCAND);
		putimage(fruits_x2[0], fruits_y2[0], &img_apple, SRCPAINT);
		end2 = time(NULL);
	}
	else if (fruits_n2 == 3) {
		putimage(fruits_x2[0], fruits_y2[0], &img_banana_mask, SRCAND);
		putimage(fruits_x2[0], fruits_y2[0], &img_banana, SRCPAINT);

		putimage(fruits_x2[1], fruits_y2[1], &img_apple_mask, SRCAND);
		putimage(fruits_x2[1], fruits_y2[1], &img_apple, SRCPAINT);

		putimage(fruits_x2[2], fruits_y2[2], &img_melon_mask, SRCAND);
		putimage(fruits_x2[2], fruits_y2[2], &img_melon, SRCPAINT);
		end2 = time(NULL);
	}
	else if (fruits_n2 == 4) {
		putimage(fruits_x2[0], fruits_y2[0], &img_banana_mask, SRCAND);
		putimage(fruits_x2[0], fruits_y2[0], &img_banana, SRCPAINT);

		putimage(fruits_x2[1], fruits_y2[1], &img_apple_mask, SRCAND);
		putimage(fruits_x2[1], fruits_y2[1], &img_apple, SRCPAINT);

		putimage(fruits_x2[2], fruits_y2[2], &img_melon_mask, SRCAND);
		putimage(fruits_x2[2], fruits_y2[2], &img_melon, SRCPAINT);

		putimage(fruits_x2[3], fruits_y2[3], &img_peach_mask, SRCAND);
		putimage(fruits_x2[3], fruits_y2[3], &img_peach, SRCPAINT);
		end2 = time(NULL);
	}
	else if (fruits_n2 == 5) {
		putimage(fruits_x2[0], fruits_y2[0], &img_banana_mask, SRCAND);
		putimage(fruits_x2[0], fruits_y2[0], &img_banana, SRCPAINT);

		putimage(fruits_x2[1], fruits_y2[1], &img_apple_mask, SRCAND);
		putimage(fruits_x2[1], fruits_y2[1], &img_apple, SRCPAINT);

		putimage(fruits_x2[2], fruits_y2[2], &img_melon_mask, SRCAND);
		putimage(fruits_x2[2], fruits_y2[2], &img_melon, SRCPAINT);

		putimage(fruits_x2[3], fruits_y2[3], &img_peach_mask, SRCAND);
		putimage(fruits_x2[3], fruits_y2[3], &img_peach, SRCPAINT);

		putimage(fruits_x2[4], fruits_y2[4], &img_berry_mask, SRCAND);
		putimage(fruits_x2[4], fruits_y2[4], &img_berry, SRCPAINT);
		end2 = time(NULL);
	}
	Sleep(4);
	FlushBatchDraw();
}

void updateWithInput() {

}

void updateWithoutInput() { 
	if (end1 - start1 == 4) // 向左发射完毕
	{
		vy2 = v0;
		time_tick2 = 1;
		for (int i = 0; i < 10; i++)
		{
			fruits_x2[i] = 100 + rand() % (WIDTH - 100);
			fruits_y2[i] = HEIGHT;
		}
		start2 = time(NULL); 
		group2 = 1;
	}
	if (group2 == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			fruits_x2[i] += -0.7 * vx2; // 向右发射
			fruits_y2[i] = HEIGHT +1.1 * vy2 * time_tick2;
		}
	}

	if (end2 - start2 == 4) // 向右发射完毕
	{
		vy1 = v0;
		time_tick1 = 1;
		for (int i = 0; i < 10; i++)
		{
			fruits_x1[i] = 100 + rand() % (WIDTH - 100);
			fruits_y1[i] = HEIGHT;
		}
		start1 = time(NULL); 
		group1 = 1; 
	}
	if (group1 == 1) {
		for (int i = 0; i < 10; i++)
		{
			fruits_x1[i] += vx1;  // 向左发射
			fruits_y1[i] = HEIGHT + 1.3 * vy1 * time_tick1;  
		}
	}

	vy1 += g * time_tick1; 
	vy2 += g * time_tick2;
	time_tick1++;
	time_tick2++;
}

void gameOver() {
	EndBatchDraw();
	_getch();
	closegraph();
}

int main() {
	startUp();
	while (true) {
		show();
		updateWithInput();
		updateWithoutInput();
	}
	gameOver();
	return 0;
}
```

