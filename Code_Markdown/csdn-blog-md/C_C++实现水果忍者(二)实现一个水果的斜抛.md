@[toc]

loadimage是一个函数，功能是装载图标，光标，或位图，它支持bmp，jpg，gif，png, emf，wmf，ico格式。

- 我们这里优先选择bmp格式的图片。先用苹果的图片：
apple.bmp：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019111209084570.bmp)     
 apple_mask.bmp：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112090913303.bmp)
显示语句为：
	```cpp
	putimage(apple_x, apple_y, &img_apple_mask, SRCAND);
	putimage(apple_x, apple_y, &img_apple, SRCPAINT);
	```
	`SRCAND` 绘制出的像素颜色 = 屏幕颜色 AND 图像颜色
    `SRCPAINT` 绘制出的像素颜色 = 屏幕颜色 OR 图像颜色
    效果图如下**1.显示苹果图片**所示。
    
- 另外，带有透明通道的png图片：
apple.png
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112175542517.png)
 直接使用效果是不行的，此时三元光栅操作码是SRCCOPY。
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112180937173.png)
 而用SRCPAINT的效果又不太好，有点透明：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112181125499.png)

- 如果用jpg格式的图片：
apple.jpg：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112175552477.jpg)
apple_mask.jpg：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112175609353.jpg)
显示语句为：
	```cpp
	putimage(apple_x, apple_y, &img_apple_mask, NOTSRCERASE);
	putimage(apple_x, apple_y, &img_apple, SRCINVERT);
	```
	`NOTSRCERASE` 绘制出的像素颜色 = NOT(屏幕颜色 OR 图像颜色)
  `SRCINVERT`   绘制出的像素颜色 = 屏幕颜色 XOR 图像颜色
  
  显示效果：
  ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112180226366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

因此使用bmp和jpg的原图与遮罩图是最好的。
### 1. 显示苹果图片
```cpp
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#define WIDTH 640
#define HEIGHT 480

double apple_x, apple_y; // 记录苹果坐标
IMAGE img_bk;
IMAGE img_apple, img_apple_mask; // 苹果图片和遮罩图

void startUp() {
	// 各项数据初始化
	apple_x = WIDTH / 2, apple_y = HEIGHT / 2;
	// 创建绘图界面和加载图片
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
	loadimage(&img_bk, _T("../background.jpg"), WIDTH, HEIGHT);
	loadimage(&img_apple, _T("../apple.bmp"));
	loadimage(&img_apple_mask, _T("../apple_mask.bmp"));
}

void show() { 
	putimage(0, 0, &img_bk); 
    // 使用给出的bmp图片时
	putimage(apple_x, apple_y, &img_apple_mask, SRCAND);
	putimage(apple_x, apple_y, &img_apple, SRCPAINT);
	Sleep(5);
	FlushBatchDraw();
}

void updateWithInput() {
}

void updateWithoutInput() {
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
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112090635528.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

### 2. 让苹果图片抛物线式运动
抛出苹果，苹果的运动曲线：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191112181635496.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)回想一下初中还是高中的物理，斜抛运动平抛运动之类的：
```cpp
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <stdlib.h>
#define WIDTH 640
#define HEIGHT 480

double v0 = -1;  
double apple_x, apple_y; // 记录苹果坐标
double vx, vy; // 苹果的水平和竖直运动速度
double time_tick;
double g = 0.000009;

IMAGE img_bk;
IMAGE img_apple, img_apple_mask; // 苹果图片和遮罩图

void startUp() {
	srand((unsigned int)time(NULL));
	// 各项数据初始化
	vy = -1;   // 竖直方向初速度(匀变速)
	vx = 0.1;  // 水平方向初速度(不变)
	time_tick = 1;
	apple_x = 10 + (rand() % (WIDTH - 20)), apple_y = HEIGHT + 10;
	// 创建绘图界面和加载图片
	initgraph(WIDTH, HEIGHT);
	BeginBatchDraw();
	loadimage(&img_bk, _T("../background.jpg"), WIDTH, HEIGHT);
	//loadimage(&img_apple, _T("../apple.jpg"));
	//loadimage(&img_apple_mask, _T("../apple_mask.jpg"));
	loadimage(&img_apple, _T("../apple.bmp"));
	loadimage(&img_apple_mask, _T("../apple_mask.bmp"));  
}

void show() {
	putimage(0, 0, &img_bk); // 之所以每次都绘制背景图，是为了挡住前面的水果图
	//putimage(apple_x, apple_y, &img_apple_mask, NOTSRCERASE);
    //putimage(apple_x, apple_y, &img_apple, SRCINVERT);
	putimage(apple_x, apple_y, &img_apple_mask, SRCAND);
	putimage(apple_x, apple_y, &img_apple, SRCPAINT);
	Sleep(5);
	FlushBatchDraw();
}

void updateWithInput() {

}

void updateWithoutInput() {
	apple_x += vx; // 水平方向匀速运动
	apple_y =  HEIGHT + 1.5 * vy * time_tick; 
	vy += g * time_tick; // 竖直方向匀变速运动
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



