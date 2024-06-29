@[toc]

实现水果被划掉就消失的功能，只需要添加显示与否的flag，判断鼠标的坐标是否碰到水果就可以了。感觉代码像面条一样拉长中......
```cpp
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#define WIDTH 640
#define HEIGHT 480
// 开始弹出第一波时很慢；水果出现粘连的情况

// 记录水果坐标
double fruits_x1[10], fruits_y1[10]; // 向左发射
double fruits_x2[10], fruits_y2[10]; // 向右发射
// 左边一半与右边一半
double fruits_left_x1[10], fruits_left_y1[10];
double fruits_right_x1[10], fruits_right_y1[10];

double fruits_left_x2[10], fruits_left_y2[10];
double fruits_right_x2[10], fruits_right_y2[10];

int fruits_n1, fruits_n2; // 两批，随机生成水果的个数

int group1 = 1, group2 = 0;
time_t start1, end1, start2, end2; // 定义时间片的起始与终止
double v0 = -1; // 一个初始的竖直速度
double vx1 = -0.2, vy1 = 0;  // 水果的水平和竖直运动速度
double vx2 = -0.2, vy2 = 0; 
double time_tick1 = 1, time_tick2 = 1; // 水果移动的时间量
double g = 0.000004;

//0是香蕉
//1是苹果
//2是芒果
//4是西瓜
//5是草莓
// 向左的水果出现
int Fruit_1flag0 = 1; //香蕉banana的出现
int Fruit_1flag1 = 1; //苹果apple的出现
int Fruit_1flag2 = 1; //芒果peach的出现
int Fruit_1flag3 = 1; //西瓜sandia的出现
int Fruit_1flag4 = 1; //草莓basaha的出现
// 向右的水果出现
int Fruit_2flag0 = 1; //香蕉banana的出现
int Fruit_2flag1 = 1; //苹果apple的出现
int Fruit_2flag2 = 1; //芒果peach的出现
int Fruit_2flag3 = 1; //西瓜sandia的出现
int Fruit_2flag4 = 1; //草莓basaha的出现

IMAGE img_bk;
IMAGE img_apple, img_apple_mask;       // 苹果图片和遮罩图
IMAGE img_banana, img_banana_mask; // 香蕉图片和遮罩图
IMAGE img_melon, img_melon_mask;     // 西瓜图片和遮罩图
IMAGE img_peach, img_peach_mask;      // 桃子图片和遮罩图
IMAGE img_berry, img_berry_mask;       // 草莓图片和遮罩图

IMAGE img_apple_left, img_apple_right, img_apple_left_mask, img_apple_right_mask; // 苹果左右边图片和遮罩图
IMAGE img_banana_left, img_banana_right, img_banana_left_mask, img_banana_right_mask; // 香蕉左右边图片和遮罩图
IMAGE img_melon_left, img_melon_right, img_melon_left_mask, img_melon_right_mask; // 西瓜左右边图片和遮罩图
IMAGE img_peach_left, img_peach_right, img_peach_left_mask, img_peach_right_mask; // 桃子左右边图片和遮罩图
IMAGE img_berry_left, img_berry_right, img_berry_left_mask, img_berry_right_mask; // 草莓左右边图片和遮罩图

void startUp() {
	srand((unsigned int)time(NULL));
	// 各项数据初始化
	// 给所有向左的图片定义一个初始位置
	for (int i = 0; i < 10; ++i) {
		fruits_x1[i] = 220 + rand() % 200; // 中间部分
		fruits_y1[i] = HEIGHT + 10; // 都不出现
	}
	start1 = time(NULL); // 先从向左发射开始
	// 给所有向右的图片定义一个初始位置
	for (int i = 0; i < 10; ++i) {
		fruits_x2[i] = 220 + rand() % 200; // 中间部分
		fruits_y2[i] = HEIGHT + 10;  // 都不出现
	}
	// 两批水果随机打印
	fruits_n1 = 1 + rand() % 5; 
	fruits_n2 = 1 + rand() % 5;

	// 创建绘图界面和加载图片
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
	loadimage(&img_bk, _T("../source/images/background.jpg"), WIDTH, HEIGHT);
	// 所有水果的图片与掩码图
	loadimage(&img_apple, _T("../source/images/apple.bmp"));
	loadimage(&img_apple_mask, _T("../source/images/apple_mask.bmp"));
	loadimage(&img_banana, _T("../source/images/banana.bmp"));
	loadimage(&img_banana_mask, _T("../source/images/banana_mask.bmp"));
	loadimage(&img_melon, _T("../source/images/melon.bmp"));
	loadimage(&img_melon_mask, _T("../source/images/melon_mask.bmp"));
	loadimage(&img_peach, _T("../source/images/peach.bmp"));
	loadimage(&img_peach_mask, _T("../source/images/peach_mask.bmp"));
	loadimage(&img_berry, _T("../source/images/strawberry.bmp"));
	loadimage(&img_berry_mask, _T("../source/images/strawberry_mask.bmp"));

	// 所有切开水果左右部分的图片与掩码图
	loadimage(&img_apple_left, _T("../source/images/apple_left.bmp"));
	loadimage(&img_apple_left_mask, _T("../source/images/apple_left_mask.bmp"));
	loadimage(&img_apple_right, _T("../source/images/apple_right.bmp"));
	loadimage(&img_apple_right_mask, _T("../source/images/apple_right_mask.bmp"));

	loadimage(&img_banana_left, _T("../source/images/banana_left.bmp"));
	loadimage(&img_banana_left_mask, _T("../source/images/banana_left_mask.bmp"));
	loadimage(&img_banana_right, _T("../source/images/banana_right.bmp"));
	loadimage(&img_banana_right_mask, _T("../source/images/banana_right_mask.bmp"));

	loadimage(&img_melon_left, _T("../source/images/melon_left.bmp"));
	loadimage(&img_melon_left_mask, _T("../source/images/melon_left_mask.bmp"));
	loadimage(&img_melon_right, _T("../source/images/melon_right.bmp"));
	loadimage(&img_melon_right_mask, _T("../source/images/melon_right_mask.bmp"));

	loadimage(&img_peach_left, _T("../source/images/peach_left.bmp"));
	loadimage(&img_peach_left_mask, _T("../source/images/peach_left_mask.bmp"));
	loadimage(&img_peach_right, _T("../source/images/peach_right.bmp"));
	loadimage(&img_peach_right_mask, _T("../source/images/peach_right_mask.bmp"));

	loadimage(&img_berry_left, _T("../source/images/berry_left.bmp"));
	loadimage(&img_berry_left_mask, _T("../source/images/berry_left_mask.bmp"));
	loadimage(&img_berry_right, _T("../source/images/berry_right.bmp"));
	loadimage(&img_berry_right_mask, _T("../source/images/berry_right_mask.bmp"));
}
//0是香蕉banana 126 50
//1是苹果apple  66 66
//2是芒果peach 62 59
//3是西瓜sandia 98 85
//4是草莓basaha  68 72 
inline void drawLeftImage(int index, IMAGE* pmask, IMAGE* psource) {
	putimage(fruits_x1[index], fruits_y1[index], pmask, SRCAND);
	putimage(fruits_x1[index], fruits_y1[index], psource, SRCPAINT);
}

inline void drawRightImage(int index, IMAGE* pmask, IMAGE* psource) {
	putimage(fruits_x2[index], fruits_y2[index], pmask, SRCAND);
	putimage(fruits_x2[index], fruits_y2[index], psource, SRCPAINT);
}

void show() {
	putimage(0, 0, &img_bk); // 之所以每次都绘制背景图，是为了挡住前面的水果图
	// 以下显示水果，如果可以显示
	switch (fruits_n1) {
	case 1:
	{
		if (Fruit_1flag2 == 1)
		{
			drawLeftImage(2, &img_peach_mask, &img_peach);
		}
		end1 = time(NULL);
		break;
	}
	case 2:
	{
		if (Fruit_1flag1 == 1)
		{
			drawLeftImage(1, &img_apple_mask, &img_apple);
		}
		if (Fruit_1flag0 == 1)
		{
			drawLeftImage(0, &img_banana_mask, &img_banana);
		}
		end1 = time(NULL);
		break;
	}
	case 3:
	{
		if (Fruit_1flag2 == 1)
		{
			drawLeftImage(2, &img_peach_mask, &img_peach);
		}
		if (Fruit_1flag3 == 1)
		{
			drawLeftImage(3, &img_melon_mask, &img_melon);
		}
		end1 = time(NULL);
		break;
	}
	case 4: 
	{
		if (Fruit_1flag1 == 1)
		{
			drawLeftImage(1, &img_apple_mask, &img_apple);
		}
		if (Fruit_1flag3 == 1)
		{
			drawLeftImage(3, &img_melon_mask, &img_melon);
		}
		end1 = time(NULL); 
		break;
	}
	case 5:
	{
		if (Fruit_1flag0 == 1)
		{
			drawLeftImage(0, &img_banana_mask, &img_banana);
		}
		if (Fruit_1flag2 == 1)
		{
			drawLeftImage(2, &img_peach_mask, &img_peach);
		}
		if (Fruit_1flag4 == 1)
		{
			drawLeftImage(4, &img_berry_mask, &img_berry);
		}
		end1 = time(NULL);
		break;
	}
	case 6:
	{
		if (Fruit_1flag4 == 1)
		{
			drawLeftImage(4, &img_berry_mask, &img_berry);
		}
		end1 = time(NULL);
		break;
	}
	case 7:
	{
		if (Fruit_1flag1 == 1)
		{
			drawLeftImage(1, &img_apple_mask, &img_apple);
		}
		if (Fruit_1flag4 == 1)
		{
			drawLeftImage(4, &img_berry_mask, &img_berry);
		}
		end1 = time(NULL);
		break;
	} 
	}

	switch (fruits_n2) {
	case 1:
	{
		if (Fruit_2flag0 == 1)
		{
			drawRightImage(0, &img_banana_mask, &img_banana);
		}

		if (Fruit_2flag2 == 1)
		{
			drawRightImage(2, &img_peach_mask, &img_peach);
		}
		end2 = time(NULL);
		break;
	}
	case 2:
	{
		if (Fruit_2flag2 == 1)
		{
			drawRightImage(2, &img_peach_mask, &img_peach);
		}
		if (Fruit_2flag3 == 1)
		{
			drawRightImage(3, &img_melon_mask, &img_melon);
		}
		if (Fruit_2flag4 == 1)
		{
			drawRightImage(4, &img_berry_mask, &img_berry);
		}
		end2 = time(NULL);
		break;
	}
	case 3:
	{
		if (Fruit_2flag3 == 1)
		{
			drawRightImage(3, &img_melon_mask, &img_melon);
		}
		end2 = time(NULL);
		break;
	}
	case 4:
	{
		if (Fruit_2flag0 == 1)
		{
			drawRightImage(0, &img_banana_mask, &img_banana);
		}
		if (Fruit_2flag2 == 1)
		{
			drawRightImage(2, &img_peach_mask, &img_peach);
		}
		end2 = time(NULL);
		break;
	}
	case 5:
	{
		if (Fruit_2flag0 == 1)
		{
			drawRightImage(0, &img_banana_mask, &img_banana);
		}
		if (Fruit_2flag1 == 1)
		{
			drawRightImage(1, &img_apple_mask, &img_apple);
		}
		end2 = time(NULL);
		break;
	}
	case 6:
	{
		if (Fruit_2flag4 == 1)
		{
			drawRightImage(4, &img_berry_mask, &img_berry);
		}
		end2 = time(NULL);
		break;
	}
	case 7:
	{
		if (Fruit_2flag2 == 1)
		{
			drawRightImage(2, &img_peach_mask, &img_peach);
		}
		if (Fruit_2flag4 == 1)
		{
			drawRightImage(4, &img_melon_mask, &img_melon);
		}
		end2 = time(NULL);
		break;
	}
	}
	Sleep(2);
	FlushBatchDraw();
}

void updateWithInput() {
	MOUSEMSG m;
	if (MouseHit()) {
		m = GetMouseMsg();
		// 鼠标移动 // 鼠标左键按下太麻烦了 m.mkLButton
		if (m.uMsg == WM_MOUSEMOVE) {
			//0是香蕉 126 50
			//1是苹果 66 66
			//2是芒果 62 59
			//3是西瓜 98 85
			//4是草莓 68 72 
			//实现鼠标划倒就消失 
			//向左消失的水果
			if (((m.x - fruits_x1[0] - 25) * (m.x - fruits_x1[0] - 25) + (m.y - fruits_y1[0] - 25) * (m.y - fruits_y1[0] - 25)) <= 900)
			{
				Fruit_1flag0 = 0;
			}
			if (((m.x - fruits_x1[1] - 33) * (m.x - fruits_x1[1] - 33) + (m.y - fruits_y1[1] - 33) * (m.y - fruits_y1[1] - 33)) <= 1000)
			{
				Fruit_1flag1 = 0;
			}
			if (((m.x - fruits_x1[2] - 35) * (m.x - fruits_x1[2] - 35) + (m.y - fruits_y1[2] - 35) * (m.y - fruits_y1[2] - 35)) <= 1000)
			{
				Fruit_1flag2 = 0;
			}
			if (((m.x - fruits_x1[3] - 45) * (m.x - fruits_x1[3] - 45) + (m.y - fruits_y1[3] - 45) * (m.y - fruits_y1[3] - 45)) <= 1600)
			{
				Fruit_1flag3 = 0;
			}
			if (((m.x - fruits_x1[4] - 35) * (m.x - fruits_x1[4] - 35) + (m.y - fruits_y1[4] - 35) * (m.y - fruits_y1[4] - 35)) <= 900)
			{
				Fruit_1flag4 = 0;
			}

			//向右消失的水果
			if (((m.x - fruits_x2[0] - 25) * (m.x - fruits_x2[0] - 25) + (m.y - fruits_y2[0] - 25) * (m.y - fruits_y2[0] - 25)) <= 800)
			{
				Fruit_2flag0 = 0;
			}
			if (((m.x - fruits_x2[1] - 33) * (m.x - fruits_x2[1] - 33) + (m.y - fruits_y2[1] - 33) * (m.y - fruits_y2[1] - 33)) <= 1000)
			{
				Fruit_2flag1 = 0;
			}
			if (((m.x - fruits_x2[2] - 35) * (m.x - fruits_x2[2] - 35) + (m.y - fruits_y2[2] - 35) * (m.y - fruits_y2[2] - 35)) <= 1000)
			{
				Fruit_2flag2 = 0;
			}
			if (((m.x - fruits_x2[3] - 45) * (m.x - fruits_x2[3] - 45) + (m.y - fruits_y2[3] - 45) * (m.y - fruits_y2[3] - 45)) <= 1600)
			{
				Fruit_2flag3 = 0;
			}
			if (((m.x - fruits_x2[4] - 35) * (m.x - fruits_x2[4] - 35) + (m.y - fruits_y2[4] - 35) * (m.y - fruits_y2[4] - 35)) <= 900)
			{
				Fruit_2flag4 = 0;
			}
		}
	}
} 

void updateWithoutInput() { 
	if (end1 - start1 == 2) // 向左发射完毕
	{
		//向右
		Fruit_2flag0 = 1; //香蕉的出现
		Fruit_2flag1 = 1; //苹果的出现
		Fruit_2flag2 = 1; //芒果的出现
		Fruit_2flag3 = 1; //西瓜的出现
		Fruit_2flag4 = 1; //草莓的出现

		vy2 = v0;
		time_tick2 = 1;
		for (int i = 0; i < 10; i++)
		{
			fruits_x2[i] = 220 + rand() % 200;
			fruits_y2[i] = HEIGHT + 10;
		}
		start2 = time(NULL);
		group2 = 1;
	}
	if (group2 == 1)
	{
		for (int i = 0; i < 10; i++)
		{
			fruits_x2[i] += -0.7 * vx2; // 向右发射
			fruits_y2[i] = HEIGHT + 1.1 * vy2 * time_tick2;
		}
	}

	if (end2 - start2 == 2)
	{
		//向左
		Fruit_1flag0 = 1; //香蕉的出现
		Fruit_1flag1 = 1; //苹果的出现
		Fruit_1flag2 = 1; //芒果的出现
		Fruit_1flag3 = 1; //西瓜的出现
		Fruit_1flag4 = 1; //草莓的出现

		vy1 = v0;
		time_tick1 = 1;
		for (int i = 0; i < 10; i++)
		{
			fruits_x1[i] = 100 + rand() % (WIDTH - 200);
			fruits_y1[i] = HEIGHT + 10;
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

	vy1 += 1.1 * g * time_tick1;
	vy2 += 1.2 * g * time_tick2;
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
