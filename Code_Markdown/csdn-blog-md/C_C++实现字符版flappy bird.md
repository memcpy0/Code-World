@[toc]
# C版本
## 点子
用游戏函数框架和控制台，写一个flappy bird的程序，每次出现一个障碍物，玩家控制的小鸟(@)越过障碍物的缺口。

## 实现过程
### 1. 小鸟下落和操作上升
第一步实现一个简单下落的小鸟@，小鸟自己有重力，会下落，每次y坐标+1，按空格键上升。换句话说，这里只给小鸟一种操作，上升。

```cpp
#include <cstdio>
#include <windows.h>
#include <conio.h>

HANDLE hout;                     //定义标准输出设备句柄
CONSOLE_SCREEN_BUFFER_INFO csbi; //定义窗口缓冲区信息结构体
CONSOLE_CURSOR_INFO curInfo;     //定义光标信息结构体
int bird_x, bird_y;
int width, height;
 
void gotoxy(int x, int y) { //移动光标
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}
void hideCursor() {  //隐藏光标
	curInfo.bVisible = 0;
	curInfo.dwSize = 1;
	SetConsoleCursorInfo(hout, &curInfo);
} 
	
int initGame() {
	hout = GetStdHandle(STD_OUTPUT_HANDLE);          //获得标准输出设备句柄 
	GetConsoleScreenBufferInfo(hout, &csbi);   		 //获得窗口缓冲区信息
	GetConsoleCursorInfo(hout, &curInfo);            //获得控制台光标信息 
	
	width = 30, height = 20;
	bird_x = width / 10, bird_y = 5;
	
	hideCursor();  //隐藏光标 
}

void endGame() {
	CloseHandle(hout);  //关闭标准输出设备句柄
}

void updateWithInput() {
	char input;
	if (kbhit()) {
		input = getch();  
		if (input == ' ') bird_y -= 2;
	}
}

void updateWithoutInput() {	
	bird_y++;   //小鸟逐渐下落  
	Sleep(150); //减缓下落  
}

void show() {
	gotoxy(0, 0);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (i == bird_y && j == bird_x) printf("@"); //输出小鸟
			else printf(" ");
		}
		printf("\n");
	}
} 

int main() {
	initGame();
	while (1) {
		show();
		updateWithInput();
		updateWithoutInput();
	}
	endGame();
	return 0;
}
```
效果如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019102009304715.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 2. 输出障碍物并让其移动
同时显示小鸟和静止的障碍物，并让障碍物自己从右向左移动。障碍物是从上面向下一截，从下面向上一截，中间有一个空缺。用bar_yTop，bar_yDown，bar1_x来描述障碍物。修改如下：
```cpp
...
int bar_yTop, bar_yDown, bar_x; //障碍物 

int initGame() {
	...
	bar_yTop = height / 4;
	bar_yDown = height / 2;
	bar_x = width - 5;
}


void updateWithoutInput() {	 
	...
	bar_x--;    //障碍物逐渐右移 
	Sleep(200); //减缓下落  
}

void show() {
	...
			else if ((i <= bar_yTop || i >= bar_yDown) && j == bar_x) printf("#");
	...
} 

...
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020094446766.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 3. 判断障碍物是否与小鸟相撞
看小鸟是否从障碍物的缝隙中通过，还是发生碰撞。碰撞后退出游戏循环。越过障碍显示得分。
```cpp
#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <time.h>

HANDLE hout;                     //定义标准输出设备句柄
CONSOLE_SCREEN_BUFFER_INFO csbi; //定义窗口缓冲区信息结构体
CONSOLE_CURSOR_INFO curInfo;     //定义光标信息结构体
int bird_x, bird_y;
int width, height;
int bar_yTop, bar_yDown, bar_x;  //障碍物  
int score;
int exitflag = 0;                //退出游戏循环标志 
 
void setSize(short len, short wid) { //设置窗口大小 
	COORD pos = {len, wid};
	SMALL_RECT rc = {0, 0, len - 1, wid - 1}; //坐标从0开始 
	// -- 设置窗口信息
	// @param	HANDLE	[in] 窗口句柄
	// @param	bool	[in] 意思不明，但在true时才起作用
	// @param	RECT *	[in] 分别指定窗口左上角坐标和右下角坐标
	// #return	bool		 成功返回非0值 
	SetConsoleWindowInfo(hout, true, &rc); 
	// -- 设置缓冲区大小
	// -- 长和宽不得小于控制台大小；不得小于系统最小限制。否则设置无效
	// @param	HANDLE	[in] 窗口句柄
	// @param	COORD	[in] 坐标结构，包含长和宽
	// #return	bool		 成功返回非0值
	SetConsoleScreenBufferSize(hout, pos); 
}
void gotoxy(int x, int y) { //移动光标
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); 
}
void hideCursor() {  //隐藏光标
	curInfo.bVisible = 0;
	curInfo.dwSize = 1;
	SetConsoleCursorInfo(hout, &curInfo);
} 
	
int initGame() {
	hout = GetStdHandle(STD_OUTPUT_HANDLE);          //获得标准输出设备句柄 
	GetConsoleScreenBufferInfo(hout, &csbi);   		 //获得窗口缓冲区信息
	GetConsoleCursorInfo(hout, &curInfo);            //获得控制台光标信息 
	hideCursor();  									 //隐藏光标
	
	setSize(width * 2, height * 2);                  //设置窗口大小 
	
	width = 30, height = 20;
	bird_x = width / 10, bird_y = 5; 
	
	bar_yTop = height / 4;
	bar_yDown = height / 2;
	bar_x = width - 5;
}

void endGame() {
	CloseHandle(hout); //关闭标准输出设备句柄
}

void updateWithInput() {
	char input;
	if (kbhit()) {
		input = getch();  
		if (input == ' ') bird_y -= 2;
	}
}

void updateWithoutInput() {	
	bird_y++;   //小鸟逐渐下落 
	bar_x--;    //障碍物逐渐右移 
	if (bar_x == bird_x) {
		if (bird_y > bar_yTop && bird_y < bar_yDown) score++;
		else {
			printf("游戏失败！");
			system("pause");
			exitflag = 1; 
		}
	}
	Sleep(200); //减缓下落  
}

void show() {
	gotoxy(0, 0);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (i == bird_y && j == bird_x) printf("@");
			else if ((i <= bar_yTop || i >= bar_yDown) && j == bar_x) printf("#");
			else printf(" ");
		}
		printf("\n");
	}
	printf("得分为：%d\n", score); 
} 

int main() {
	initGame();
	while (1) {
		show();
		updateWithInput();
		updateWithoutInput();
		if (exitflag) break;  //游戏失败, 满足退出条件 
	}
	endGame();
	return 0;
}
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020101014605.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 4. 障碍物循环出现
类似飞机游戏中敌机被击落或者超出屏幕范围后重新出现敌机，这里让障碍物在左边消失后在最右边循环出现。用rand()**随机产生障碍物的缝隙的**位置和**大小**，当然，要让小鸟通过。
```cpp
void updateWithoutInput() {	
	bird_y++;   //小鸟逐渐下落 
	bar_x--;    //障碍物逐渐右移 
	if (bar_x < 0) {  //重新生成一个障碍物 
		bar_x = width;
		int temp = rand() % (int)(height * 0.8); 
		bar_yTop = temp - height / 10 - 1;  
		bar_yDown = temp + height / 10 + 1;
	}
	...
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020102627454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
思考：
- 如何同时显示多根柱子？提示：用x坐标数组合y坐标数组；或者用结构体。
- 模拟重力效果，小鸟加速下落。
