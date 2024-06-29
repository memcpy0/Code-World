@[toc]
## 下载安装
[easy_x](https://easyx.cn/)官方网站，点击【下载】可以下载不同版本的easy_x安装软件，我直接下了`EasyX库(附带graphics.h) 测试版 (2019-5-29 更新)`这一版本。

[easy_x官方文档](https://docs.easyx.cn/zh-cn/intro)，全中文。不过在启动安装软件后，会自动扫描你电脑中的不同版本VC，然后选择安装到哪个版本的VC，也可以选择是否安装chm格式的文档到电脑上。

其实很多东西官方文档中都**说得很详细**了，这里就不啰嗦了。

## EasyX基础使用知识
在Visual C++中创建空的**控制台项目**，然后**添加.cpp文件**，`#include graphics.h`就可以了。如果不是控制台项目，而是Win32 Application或者其他的项目，就无法使用控制台相关的函数。

注意：EasyX无法和Dev CPP一起使用，必须配合VC使用，代码文件扩展名要用.cpp。

基本概念：
- 设备(绘图表面)，EasyX运行在控制台上面，调用win api进行绘图。绘图的窗口只有两种，默认绘图窗口的和IMAGE对象。通过`SetWorkingImage(IMAGE* pImg = NULL)`函数设置，一般绘图设备为默认绘图窗口。选定以后，**所有的绘图函数**都会运作在这个绘图设备上。注意：EasyX不是Qt或者MFC，EasyX只是绘图工具。
- 坐标，分为物理坐标(描述设备的坐标系，左上方为原点，X轴向右为正，Y轴向下为正，单位为像素)和逻辑坐标(一般和物理坐标一一对应)。可通过`setorigin`改变坐标原点，`setaspectratio`改变坐标轴方向和缩放比例。
- 颜色。既然EasyX重点在于绘图，那颜色的表示方法就非常重要了。EasyX提供了**一些预定义大写的颜色常量**(`BLACK, BLUE, GREEN, RED......`)，这些常量实际是**16进制的颜色表示**，`0xbbggrr`；还可以用windows SDK中的**RGB宏来合成颜色**，`RGB(b, g, r)`。比较少用的是用 `HSLtoRGB`、`HSVtoRGB`转换其他色彩模型到RGB 颜色。

用EasyX来做控制台游戏之类的，和直接使用Console相关的Ｃ函数以及Windows的api**思路差别不大**，都是配合一定的程序逻辑，重复“`绘制－擦除`”这一的过程。但是方便多了。
 
---
## EasyX程序示例
### 1.1 画一个实心圆
```c
#include <graphics.h>		// 引用 EasyX 图形库
#include <conio.h>
int main()
{
	initgraph(640, 480);	// 初始化640×480的画布
	setcolor(YELLOW);       // 圆的线条为黄色
	setfillcolor(GREEN);    // 圆内部位绿色填充
	fillcircle(100, 100, 20);  // 画圆，圆心(100, 100)，半径 20
	_getch();				// 按任意键继续
	closegraph();			// 关闭图形界面
	return 0;
}
```
### 1.2 画出10条直线
```c
#include <graphics.h>
#include <conio.h>
int main()
{
	initgraph(640, 480);
	for(int y=0; y<=480; y=y+48)
		line(0, y, 640, y);
	getch();
	closegraph();
	return 0;
}
```
### 1.3 画出红蓝交替的直线
```c
#include <graphics.h>
#include <conio.h>
int main()
{
	initgraph(640, 200);
	for(int y=0; y<=200; y=y+5)
	{
		if ( y/5 % 2 == 1)    // ÅÐ¶ÏÆæÊýÐÐÅ¼ÊýÐÐ
			setcolor(RGB(255,0,0));
		else
			setcolor(RGB(0,0,255));
		line(0, y, 640, y);
	}
	getch();
	closegraph();
	return 0;
}
```
### 1.4 绘制国际象棋棋盘
```c
// 绘制国际象棋棋盘
#include <graphics.h>
#include <conio.h>
int main()
{
	int step = 50;
	// 初始化绘图窗口
	initgraph(500, 500);	
	// 设置背景色为蓝色
	setbkcolor(YELLOW);
	// 用背景色清空屏幕
	cleardevice();
	
	int i,j;
	for(i=1;i<=8;i++)  
	{
		for(j=1;j<=8;j++)  
		{
			if ((i+j)% 2 ==1)
			{
				setfillcolor(BLACK);
				solidrectangle(i*step,j*step,(i+1)*step,(j+1)*step); 	// 绘制黑色方块
			}
			else
			{
				setfillcolor(WHITE);
				solidrectangle(i*step,j*step,(i+1)*step,(j+1)*step); 	// 绘制白色方块
			}
		}
	}
	getch();
	closegraph();
	return 0;
}
```
### 1.5 绘制围棋棋盘
```c
// 绘制围棋棋盘
#include <graphics.h>
#include <conio.h>
int main()
{
	int step = 30;
	// 初始化绘图窗口
	initgraph(600, 600);	
	// 设置背景色为蓝色
	setbkcolor(YELLOW);
	// 用背景色清空屏幕
	cleardevice();
	
	setlinestyle(PS_SOLID, 2);  // 画实线，宽度为2个像素
	setcolor(RGB(0,0,0));  // 设置为黑色
	
	int i;
	for(i=1; i<=19; i++)  // 画横线和竖线
	{
		line(i*step, 1*step, i*step,  19*step);
		line(1*step, i*step, 19*step, i*step);
	}
	getch();
	closegraph();
	return 0;
}
```
### 1.6 反弹球动画
```c
#include <graphics.h>
#include <conio.h>
#define High 480  // 游戏画面尺寸
#define Width 640

int main()
{
	float ball_x,ball_y; // 小球的坐标
	float ball_vx,ball_vy; // 小球的速度
	float radius;  // 小球的半径
	
	initgraph(Width, High);
	ball_x = Width/2;
	ball_y = High/2;
	ball_vx = 1;
	ball_vy = 1;
	radius = 20;
	
	while (1)
	{
		// 绘制黑线、黑色填充的圆
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(ball_x, ball_y, radius);
		
		// 更新小圆坐标
		ball_x = ball_x + ball_vx;
		ball_y = ball_y + ball_vy;
		
		if ((ball_x<=radius)||(ball_x>=Width-radius))
			ball_vx = -ball_vx;
		if ((ball_y<=radius)||(ball_y>=High-radius))
			ball_vy = -ball_vy;	
		
		// 绘制黄线、绿色填充的圆
		setcolor(YELLOW);
		setfillcolor(GREEN);
		fillcircle(ball_x, ball_y, radius);
		// 延时
		Sleep(3);
	}
	closegraph();
	return 0;
}
```

### 1.7 无闪烁的反弹球动画
```c
#include <graphics.h>
#include <conio.h>
#define High 480  // 游戏画面尺寸
#define Width 640

int main()
{
	float ball_x,ball_y; // 小球的坐标
	float ball_vx,ball_vy; // 小球的速度
	float radius;  // 小球的半径
	
	initgraph(Width, High);
	ball_x = Width/2;
	ball_y = High/2;
	ball_vx = 1;
	ball_vy = 1;
	radius = 20;
	
	BeginBatchDraw();
	while (1)
	{
		// 绘制黑线、黑色填充的圆
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(ball_x, ball_y, radius);
		
		// 更新小圆坐标
		ball_x = ball_x + ball_vx;
		ball_y = ball_y + ball_vy;
		
		if ((ball_x<=radius)||(ball_x>=Width-radius))
			ball_vx = -ball_vx;
		if ((ball_y<=radius)||(ball_y>=High-radius))
			ball_vy = -ball_vy;	
		
		// 绘制黄线、绿色填充的圆
		setcolor(YELLOW);
		setfillcolor(GREEN);
		fillcircle(ball_x, ball_y, radius);
		
		FlushBatchDraw();
		
		// 延时
		Sleep(3);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}
```
### 1.8 小球向右移动动画
```c
#include <graphics.h>
#include <conio.h>
int main()
{
	initgraph(640, 480);
	for(int x=100; x<540; x+=20)
	{
		// 绘制黄线、绿色填充的圆
		setcolor(YELLOW);
		setfillcolor(GREEN);
		fillcircle(x, 100, 20);
		// 延时
		Sleep(200);
		// 绘制黑线、黑色填充的圆
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, 100, 20);
	}
	closegraph();
	return 0;
}
```
### 2.1 多球反弹
```c
#include <graphics.h>
#include <conio.h>
#include <math.h>
#define High 480  // 游戏画面尺寸
#define Width 640
#define BallNum 15 // 小球个数

int main()
{
	float ball_x[BallNum],ball_y[BallNum]; // 小球的坐标
	float ball_vx[BallNum],ball_vy[BallNum]; // 小球的速度
	float radius;  // 小球的半径
	int i,j;

	radius = 20;

	for (i=0;i<BallNum;i++) // 随机小球的位置与速度
	{
		ball_x[i] = rand()%int(Width-4*radius) + 2*radius;
		ball_y[i] = rand()%int(High-4*radius) + 2*radius;
		ball_vx[i] = (rand()%2)*2 - 1;
		ball_vy[i] = (rand()%2)*2 - 1;
	}

	initgraph(Width, High);	
	BeginBatchDraw();

	while (1)
	{
		// 绘制黑线、黑色填充的圆
		setcolor(BLACK);
		setfillcolor(BLACK);
		for (i=0;i<BallNum;i++)
			fillcircle(ball_x[i], ball_y[i], radius);
		
		// 更新小圆坐标
		for (i=0;i<BallNum;i++)
		{
			ball_x[i] = ball_x[i] + ball_vx[i];
			ball_y[i] = ball_y[i] + ball_vy[i];
			
			// 把超出边界的小球拉回来
			if (ball_x[i]<radius)
				ball_x[i] = radius;
			if (ball_y[i]<radius)
				ball_y[i] = radius;
			if (ball_x[i]>Width-radius)
				ball_x[i] = Width-radius;
			if (ball_y[i]>High-radius)
				ball_y[i] = High-radius;
		}
		
		// 判断是否和墙壁碰撞
		for (i=0;i<BallNum;i++)
		{	
			if ((ball_x[i]<=radius)||(ball_x[i]>=Width-radius))
				ball_vx[i] = -ball_vx[i];
			if ((ball_y[i]<=radius)||(ball_y[i]>=High-radius))
				ball_vy[i] = -ball_vy[i];	
		}
		
		float minDistances2[BallNum][2]; // 记录某个小球，距离它最近的小球的距离，这个小球的下标
		for (i=0;i<BallNum;i++)
		{
			minDistances2[i][0] = 9999999;
			minDistances2[i][1] = -1;
		}
		
		// 求解所有小球两两之间的距离平方
		for (i=0;i<BallNum;i++)
		{
			for (j=0;j<BallNum;j++)
			{
				if (i!=j) // 自己和自己不需要比
				{
					float dist2;
					dist2 = (ball_x[i] - ball_x[j])*(ball_x[i] - ball_x[j])
						+ (ball_y[i] - ball_y[j])*(ball_y[i] - ball_y[j]);
					if (dist2<minDistances2[i][0])
					{
						minDistances2[i][0] = dist2;
						minDistances2[i][1] = j;
					}
				}
			}
		}	
		
		// 判断球之间是否碰撞
		for (i=0;i<BallNum;i++)
		{
			if (minDistances2[i][0]<=4*radius*radius) // 最小距离小于阈值，发生碰撞
			{
				j = minDistances2[i][1];
				// 交换速度				
				int temp;
				temp = ball_vx[i]; ball_vx[i] = ball_vx[j]; ball_vx[j] = temp;
				temp = ball_vy[i]; ball_vy[i] = ball_vy[j]; ball_vy[j] = temp;
				
				minDistances2[j][0] = 999999999; // 避免交换两次速度，又回去了
				minDistances2[j][1] = -1;
			}
		}
		
		// 绘制黄线、绿色填充的圆
		setcolor(YELLOW);
		setfillcolor(GREEN);
		for (i=0;i<BallNum;i++)
			fillcircle(ball_x[i], ball_y[i], radius);
		
		FlushBatchDraw();
		
		// 延时
		Sleep(3);
	}
	EndBatchDraw();
	closegraph();
	return 0;
}
```
### 2.2 实时时钟
```c
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define High 480  // 游戏画面尺寸
#define Width 640
#define	PI	3.14159

int main()
{
	initgraph(Width, High);		// 初始化 640 x 480 的绘图窗口	
	int center_x,center_y;      // 中心点的坐标，也是表的中心
	center_x = Width/2;
	center_y = High/2;
	int secondLength = Width/5;           // 秒针的长度
	int minuteLength = Width/6;           // 分针的长度
	int hourLength = Width/7;             // 时针的长度
	
	int secondEnd_x,secondEnd_y;    // 秒针的终点
	int minuteEnd_x,minuteEnd_y;    // 分针的终点
	int hourEnd_x,hourEnd_y;    // 时针的终点
	float secondAngle;       // 秒钟对应的角度
	float minuteAngle;       // 分钟对应的角度
	float hourAngle;         // 时钟对应的角度
	
	SYSTEMTIME ti;				// 定义变量保存当前时间
	
	BeginBatchDraw();
	while (1)
	{
		// 绘制一个简单的表盘
		setlinestyle(PS_SOLID, 1);
		setcolor(WHITE);
		circle(center_x, center_y, Width/4);
		
		// 画刻度
		int x, y,i;
		for (i=0; i<60; i++)
		{
			x = center_x + int(Width/4.3 * sin(PI * 2 * i / 60));
			y = center_y + int(Width/4.3 * cos(PI * 2 * i / 60));
			
			if (i % 15 == 0)
				bar(x - 5, y - 5, x + 5, y + 5);
			else if (i % 5 == 0)
				circle(x, y, 3);
			else
				putpixel(x, y, WHITE);
		}
		
		outtextxy(center_x - 25, center_y + Width/6, "我的时钟");
		
		GetLocalTime(&ti);		// 获取当前时间
		// 秒钟角度变化
		secondAngle = ti.wSecond * 2*PI/60;  // 一圈一共2*PI，一圈60秒，一秒钟秒钟走过的角度为2*PI/60
		// 分钟角度变化
		minuteAngle = ti.wMinute * 2*PI/60 + secondAngle/60;  // 一圈一共2*PI，一圈60分，一分钟分钟走过的角度为2*PI/60
		// 时钟角度变化
		hourAngle = ti.wHour * 2*PI/12 + minuteAngle/12;  // 一圈一共2*PI，一圈12小时，一小时时钟走过的角度为2*PI/12		
		// 由角度决定的秒针端点坐标
		secondEnd_x = center_x + secondLength*sin(secondAngle);
		secondEnd_y = center_y - secondLength*cos(secondAngle);
		
		// 由角度决定的分针端点坐标
		minuteEnd_x = center_x + minuteLength*sin(minuteAngle);
		minuteEnd_y = center_y - minuteLength*cos(minuteAngle);
		
		// 由角度决定的时针端点坐标
		hourEnd_x = center_x + hourLength*sin(hourAngle);
		hourEnd_y = center_y - hourLength*cos(hourAngle);		
		
		setlinestyle(PS_SOLID, 2);  
		setcolor(YELLOW);
		line(center_x, center_y, secondEnd_x, secondEnd_y); // 画秒针
		
		setlinestyle(PS_SOLID, 5);  
		setcolor(BLUE);
		line(center_x, center_y, minuteEnd_x, minuteEnd_y); // 画分针
		
		setlinestyle(PS_SOLID, 10);  
		setcolor(RED);
		line(center_x, center_y, hourEnd_x, hourEnd_y); // 画时针
		
		FlushBatchDraw();		
		Sleep(10);
		
		setcolor(BLACK);
		setlinestyle(PS_SOLID, 2); 
		line(center_x, center_y, secondEnd_x, secondEnd_y);  // 隐藏前一帧的秒针
		setlinestyle(PS_SOLID, 5);  
		line(center_x, center_y, minuteEnd_x, minuteEnd_y); // 隐藏前一帧的分针
		setlinestyle(PS_SOLID, 10);  
		line(center_x, center_y, hourEnd_x, hourEnd_y); // 隐藏前一帧的时针
	}
	
	EndBatchDraw();
	getch();				// 按任意键继续	
	closegraph();			// 关闭绘图窗口
	return 0;
}
```
### 2.3 反弹球消砖块
```c
#include <conio.h>
#include <graphics.h>

#define High 480  // 游戏画面尺寸
#define Width 640
#define Brick_num 10 // 砖块个数

// 全局变量
int ball_x,ball_y; // 小球的坐标
int ball_vx,ball_vy; // 小球的速度
int radius;  // 小球的半径
int bar_x,bar_y; // 挡板中心坐标
int bar_high,bar_width;  // 挡板的高度和宽度
int bar_left,bar_right,bar_top,bar_bottom; // 挡板的上下左右位置坐标

int isBrickExisted[Brick_num]; // 每个砖块是否存在，1为存在，0为没有了
int brick_high,brick_width; // 每个砖块的高度和宽度

void startup()  // 数据初始化
{
	ball_x = Width/2;
	ball_y = High/2;
	ball_vx = 1;
	ball_vy = 1;
	radius = 20;
	
	bar_high = High/20;
	bar_width = Width/2;
	bar_x = Width/2;
	bar_y = High - bar_high/2;
	bar_left = bar_x - bar_width/2;
	bar_right = bar_x + bar_width/2;
	bar_top = bar_y - bar_high/2;
	bar_bottom = bar_y + bar_high/2;

	brick_width = Width/Brick_num;
	brick_high = High/Brick_num;

	int i;
	for (i=0;i<Brick_num;i++)
		isBrickExisted[i] = 1;
	
	initgraph(Width, High);
	BeginBatchDraw();
}

void clean()  // 消除画面
{
	setcolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(ball_x, ball_y, radius); 	// 绘制黑线、黑色填充的圆
	bar(bar_left,bar_top,bar_right,bar_bottom);	// 绘制黑线、黑色填充的挡板

	int i,brick_left,brick_right,brick_top,brick_bottom;	
	for (i=0;i<Brick_num;i++)
	{
		brick_left = i*brick_width;
		brick_right = brick_left + brick_width;
		brick_top = 0;
		brick_bottom = brick_high;
		if (!isBrickExisted[i])	 // 砖块没有了，绘制黑色
			fillrectangle(brick_left,brick_top,brick_right,brick_bottom);
	}
}	

void show()  // 显示画面
{
	setcolor(YELLOW);
	setfillcolor(GREEN);
	fillcircle(ball_x, ball_y, radius);	// 绘制黄线、绿色填充的圆
	bar(bar_left,bar_top,bar_right,bar_bottom);	// 绘制黄线、绿色填充的挡板

	int i,brick_left,brick_right,brick_top,brick_bottom;

	for (i=0;i<Brick_num;i++)
	{
		brick_left = i*brick_width;
		brick_right = brick_left + brick_width;
		brick_top = 0;
		brick_bottom = brick_high;

		if (isBrickExisted[i])	 // 砖块存在，绘制砖块
		{
			setcolor(WHITE);
			setfillcolor(RED);
			fillrectangle(brick_left,brick_top,brick_right,brick_bottom);	// 绘制砖块
		}
	}

	FlushBatchDraw();
	// 延时
	Sleep(3);	
}	

void updateWithoutInput()  // 与用户输入无关的更新
{
	// 挡板和小圆碰撞，小圆反弹
	if ( ( (ball_y+radius >= bar_top) && (ball_y+radius < bar_bottom-bar_high/3) )
		||  ( (ball_y-radius <= bar_bottom) && (ball_y-radius > bar_top-bar_high/3) ) )
		if ( (ball_x>=bar_left) && (ball_x<=bar_right) )
				ball_vy = -ball_vy;

	// 更新小圆坐标
	ball_x = ball_x + ball_vx;
	ball_y = ball_y + ball_vy;
	
	// 小圆和边界碰撞
	if ((ball_x==radius)||(ball_x==Width-radius))
		ball_vx = -ball_vx;
	if ((ball_y==radius)||(ball_y==High-radius))
		ball_vy = -ball_vy;	

	// 判断小圆是否和某个砖块碰撞
	int i,brick_left,brick_right,brick_bottom;
	for (i=0;i<Brick_num;i++)
	{
		if (isBrickExisted[i])	 // 砖块存在，才判断
		{
			brick_left = i*brick_width;
			brick_right = brick_left + brick_width;
			brick_bottom = brick_high;
			if ( (ball_y==brick_bottom+radius) && (ball_x>=brick_left) && (ball_x<=brick_right) )
			{
				isBrickExisted[i] = 0;
				ball_vy = -ball_vy;	
			}
		}
	}
}

void updateWithInput()  // 与用户输入有关的更新
{	
	char input;
	if(kbhit())  // 判断是否有输入
	{
		input = getch();  // 根据用户的不同输入来移动，不必输入回车
		if (input == 'a' && bar_left>0)   
		{
			bar_x = bar_x-15;  // 位置左移
			bar_left = bar_x - bar_width/2;
			bar_right = bar_x + bar_width/2;
		}
		if (input == 'd' && bar_right<Width)
		{
			bar_x = bar_x+15;  // 位置右移
			bar_left = bar_x - bar_width/2;
			bar_right = bar_x + bar_width/2;
		}
		if (input == 'w' && bar_top>0)   
		{
			bar_y = bar_y-15;  // 位置左移
			bar_top = bar_y - bar_high/2;
			bar_bottom = bar_y + bar_high/2;
		}
		if (input == 's' && bar_bottom<High)
		{
			bar_y = bar_y+15;  // 位置右移
			bar_top = bar_y - bar_high/2;
			bar_bottom = bar_y + bar_high/2;
		}
	}
}

void gameover()
{
	EndBatchDraw();
	closegraph();
}

int main()
{
	startup();  // 数据初始化	
	while (1)  //  游戏循环执行
	{
		clean();  // 把之前绘制的内容取消
		updateWithoutInput();  // 与用户输入无关的更新
		updateWithInput();     // 与用户输入有关的更新
		show();  // 显示新画面
	}
	gameover();     // 游戏结束、后续处理
	return 0;
}
```

### 3.1 鼠标画线
```c
#include <graphics.h>
#include <conio.h>
int main()
{	
	initgraph(640, 480); // 初始化图形窗口
	MOUSEMSG m;		// 定义鼠标消息	
	while(1)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();		
		if(m.uMsg == WM_MOUSEMOVE)
		{
			// 鼠标移动的时候在鼠标位置画白色的小点
			putpixel(m.x, m.y, WHITE);
		}			
		else if (m.uMsg == WM_LBUTTONDOWN)
		{ 
			// 鼠标左键按下时在鼠标位置画一个方块
			rectangle(m.x-5, m.y-5, m.x+5, m.y+5);
		}
		else if (m.uMsg == WM_RBUTTONUP)
		{ 
			// 鼠标右键按下又抬起后，画一个圆
			circle(m.x, m.y, 10);
		}
	}
	return 0;
}
```
### 3.2 鼠标交互反弹球
```c
#include <conio.h>
#include <graphics.h>

#define High 480  // 游戏画面尺寸
#define Width 640
#define Brick_num 10 // 砖块个数

// 全局变量
int ball_x,ball_y; // 小球的坐标
int ball_vx,ball_vy; // 小球的速度
int radius;  // 小球的半径
int bar_x,bar_y; // 挡板中心坐标
int bar_high,bar_width;  // 挡板的高度和宽度
int bar_left,bar_right,bar_top,bar_bottom; // 挡板的上下左右位置坐标

int isBrickExisted[Brick_num]; // 每个砖块是否存在，1为存在，0为没有了
int brick_high,brick_width; // 每个砖块的高度和宽度

void startup()  // 数据初始化
{
	ball_x = Width/2;
	ball_y = High/2;
	ball_vx = 1;
	ball_vy = 1;
	radius = 20;
	
	bar_high = High/20;
	bar_width = Width/2;
	bar_x = Width/2;
	bar_y = High - bar_high/2;
	bar_left = bar_x - bar_width/2;
	bar_right = bar_x + bar_width/2;
	bar_top = bar_y - bar_high/2;
	bar_bottom = bar_y + bar_high/2;

	brick_width = Width/Brick_num;
	brick_high = High/Brick_num;

	int i;
	for (i=0;i<Brick_num;i++)
		isBrickExisted[i] = 1;
	
	initgraph(Width, High);
	BeginBatchDraw();
}

void clean()  // 消除画面
{
	setcolor(BLACK);
	setfillcolor(BLACK);
	fillcircle(ball_x, ball_y, radius); 	// 绘制黑线、黑色填充的圆
	bar(bar_left,bar_top,bar_right,bar_bottom);	// 绘制黑线、黑色填充的挡板

	int i,brick_left,brick_right,brick_top,brick_bottom;	
	for (i=0;i<Brick_num;i++)
	{
		brick_left = i*brick_width;
		brick_right = brick_left + brick_width;
		brick_top = 0;
		brick_bottom = brick_high;
		if (!isBrickExisted[i])	 // 砖块没有了，绘制黑色
			fillrectangle(brick_left,brick_top,brick_right,brick_bottom);
	}
}	

void show()  // 显示画面
{
	setcolor(YELLOW);
	setfillcolor(GREEN);
	fillcircle(ball_x, ball_y, radius);	// 绘制黄线、绿色填充的圆
	bar(bar_left,bar_top,bar_right,bar_bottom);	// 绘制黄线、绿色填充的挡板

	int i,brick_left,brick_right,brick_top,brick_bottom;

	for (i=0;i<Brick_num;i++)
	{
		brick_left = i*brick_width;
		brick_right = brick_left + brick_width;
		brick_top = 0;
		brick_bottom = brick_high;

		if (isBrickExisted[i])	 // 砖块存在，绘制砖块
		{
			setcolor(WHITE);
			setfillcolor(RED);
			fillrectangle(brick_left,brick_top,brick_right,brick_bottom);	// 绘制砖块
		}
	}

	FlushBatchDraw();
	// 延时
	Sleep(3);	
}	

void updateWithoutInput()  // 与用户输入无关的更新
{
	// 挡板和小圆碰撞，小圆反弹
	if ( ( (ball_y+radius >= bar_top) && (ball_y+radius < bar_bottom-bar_high/3) )
		||  ( (ball_y-radius <= bar_bottom) && (ball_y-radius > bar_top-bar_high/3) ) )
		if ( (ball_x>=bar_left) && (ball_x<=bar_right) )
				ball_vy = -ball_vy;

	// 更新小圆坐标
	ball_x = ball_x + ball_vx;
	ball_y = ball_y + ball_vy;
	
	// 小圆和边界碰撞
	if ((ball_x==radius)||(ball_x==Width-radius))
		ball_vx = -ball_vx;
	if ((ball_y==radius)||(ball_y==High-radius))
		ball_vy = -ball_vy;	

	// 判断小圆是否和某个砖块碰撞
	int i,brick_left,brick_right,brick_bottom;
	for (i=0;i<Brick_num;i++)
	{
		if (isBrickExisted[i])	 // 砖块存在，才判断
		{
			brick_left = i*brick_width;
			brick_right = brick_left + brick_width;
			brick_bottom = brick_high;
			if ( (ball_y==brick_bottom+radius) && (ball_x>=brick_left) && (ball_x<=brick_right) )
			{
				isBrickExisted[i] = 0;
				ball_vy = -ball_vy;	
			}
		}
	}
}

void updateWithInput()  // 与用户输入有关的更新
{	
	MOUSEMSG m;		// 定义鼠标消息
	if (MouseHit())  //这个函数用于检测当前是否有鼠标消息
	{
		m = GetMouseMsg();  	// 获取一条鼠标消息
		if(m.uMsg == WM_MOUSEMOVE)
		{
			// 鼠标移动时，挡板的位置等于鼠标所在的位置
			bar_x = m.x;
			bar_y = m.y;
			bar_left = bar_x - bar_width/2;
			bar_right = bar_x + bar_width/2;
			bar_top = bar_y - bar_high/2;
			bar_bottom = bar_y + bar_high/2;
		}
		else if (m.uMsg == WM_LBUTTONDOWN)
		{
			// 按下鼠标左键，初始化小球的位置为挡板上面中心
			ball_x = bar_x;
			ball_y = bar_top - radius-3;
		}
	}
}

void gameover()
{
	EndBatchDraw();
	closegraph();
}

int main()
{
	startup();  // 数据初始化	
	while (1)  //  游戏循环执行
	{
		clean();  // 把之前绘制的内容取消
		updateWithoutInput();  // 与用户输入无关的更新
		updateWithInput();     // 与用户输入有关的更新
		show();  // 显示新画面
	}
	gameover();     // 游戏结束、后续处理
	return 0;
}
```
 


