@[toc]

## 1、开始
为了熟悉EasyX的API，我花了一天多一点的时间写了一个简单的双人对战五子棋游戏，有开始界面、下棋界面和暂停界面，随机选择音乐并循环播放，使用鼠标点击下棋。

之所以会花一天的时间，主要原因是对面向对象的编程方式不熟悉，在类设计上面权衡了蛮久，然后将就着写出来了......

这些功能不多，和EasyX能够玩出的花样比起来还远远不够。比如动作游戏中实现的人物的移动、复杂动画效果的实现等，都需要一些时间，尤其是准备合适的美术素材和进行处理。

## 2、类结构
下棋游戏可以抽象出屏幕、棋盘、棋手、棋子等对象。我实现了screen、board、chess这三个对象，并用game对象进行统一管理。

chess中包含棋子半径radius和绘制棋子的方法，是最基础的类。screen管理各种界面的绘制，board管理棋盘的绘制和棋局的整体绘制。game是上层对象，调度这三个对象。

之所以没有实现player，是因为这里没有打算统计player的姓名、分数、下棋的棋数等信息......

![在这里插入图片描述](https://img-blog.csdnimg.cn/20191017230134889.png)
游戏的主文件代码：
```c
#include "game.h" 
using namespace std; 
int main() { 
	game my_game; 
	my_game.Load(); 
	return 0;
}
```
几个界面：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191017225852486.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191017230533303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191017230046287.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
## 3、具体实现
就不说明具体细节了，注释都是按照EasyX文档上面写的。
```cpp
//screen.h
#pragma once
#include <graphics.h>
#include <string>
#include <conio.h>
#include <windows.h> 
#include <cstdlib>
#include <ctime>
#include <string>
//#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std; 

class screen {
private:  
	int unit = 30;   //一个单元格大小
	int width = 20 * unit + 150, height = 21 * unit + 40;
	string startPath = "开始界面.jpg";
	string path = "五子棋背景.jpg"; 
	string tpath = "停止界面.jpg";

	IMAGE startBk; // 开始界面背景图
	IMAGE boardBk;  // 棋盘背景图对象
	IMAGE pauseBk; // 暂停界面背景图

	int bkcolor = YELLOW;  // 屏幕背景颜色
public:  
	screen(); // 初始化绘图设备
	~screen(); // 关闭绘画设备 
	int getWidth() { return width; }
	int getHeight() { return height; }

	void printScreen();  // 绘制屏幕（使用背景图路径并绘制背景图(使用部分图片)）
	void repeatPlayMusic();  // 循环播放背景音乐

	void startMenu();
	void pauseMenu();
};
```
```cpp
//screen.cpp
#include "screen.h"

screen::screen()
{
	initgraph(width, height);
	// 用于开始批量绘图，执行后，任何绘图操作都将暂时不输出到屏幕上，
	// 直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出
	HWND hwnd = GetHWnd(); // 获取窗口句柄 
	SetWindowText(hwnd, "双人五子棋 初版"); // 设置窗口标题

	BeginBatchDraw();

	loadimage(&startBk, startPath.c_str(), width, height);  // 加载开始界面的背景图，进行图片拉伸
	loadimage(&boardBk, path.c_str(), 1400, 1400); // 加载背景图，图片进行拉伸 
	loadimage(&pauseBk, tpath.c_str(), width, height); // 加载背景图，拉伸
	srand((unsigned int)time(NULL)); // 随机播放音乐
}

screen::~screen()
{
	EndBatchDraw();
	closegraph();  // 关闭绘画设备
}

void screen::printScreen()
{
	setbkcolor(bkcolor);  // 设置屏幕背景颜色
	cleardevice(); //用当前背景色清空屏幕，并将当前点移至 (0, 0)
	
	putimage(0, 0, width, height, &boardBk, 0, 0); // 绘制图像(指定宽高和起始位置)

	setbkmode(0);
	settextcolor(BLACK);
	int x = width - 4 * unit, y = height - 8 * unit;
	settextstyle(20, 0, _T("黑体"));
	outtextxy(x, y, "五子棋规则：");

	settextstyle(16, 0, _T("宋体"));
	outtextxy(x, y + unit, "1. 五子一线赢");
	outtextxy(x, y + 2 * unit, "2. 黑子先行");
	outtextxy(x, y + 3 * unit, "3. 按esc暂停");  
	outtextxy(x, y + 4 * unit, "4. 按s存档");
	outtextxy(x, y + 5 * unit, "5. 按r悔棋");

	FlushBatchDraw();  
}

void screen::repeatPlayMusic()
{
	/* 
	0 千年幻想乡
	1 竹取飞翔
	2 碎月
	3 幽雅に咲かせ 墨染の桜
	4 東方妖々夢～広有射怪鳥事
	*/
	string t1 = "open music", t2 = ".mp3 alias bkmusic";
	char kth = '0' + rand() % 5;  // 随机播放音乐
	string sound = t1 + kth + t2; 
	/*
	1. 打开音乐 open后面的music.mp3是音乐文件的相对路径，使用绝对路径也可以
	2. alias bgm 是给音乐取个别名  下面就可以直接open bgm了,别名可以自取
	3. 播放音乐中 repeat表示重复播放，如果只想播放一次，可以去掉repeat
	4. mciSendString这个函数可以播放mp3,wav格式的音乐，如果代码无误但是没法播放音乐,尝试换一首
	*/
	mciSendString(sound.c_str(), NULL, 0, NULL); // 打开背景音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放音乐 	
}

void screen::startMenu()
{ 
	putimage(0, 0, width, height, &startBk, 0, 0);
	setbkmode(0);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书")); 
	outtextxy(width * 0.35, height * 0.7, "a 进入游戏");
	outtextxy(width * 0.35, height * 0.76, "b 退出游戏");
	FlushBatchDraw();

	settextcolor(LIGHTGRAY);
	settextstyle(30, 0, _T("黑体")); 
	outtextxy(15, height * 0.83, "双人五子棋，进入游戏后点击鼠标落子，按esc键暂停");
	FlushBatchDraw();
}

void screen::pauseMenu()
{
	putimage(0, 0, width, height, &pauseBk, 0, 0);
	setbkmode(0);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("隶书"));
	outtextxy(width * 0.35, height * 0.4, "1 继续游戏");
	outtextxy(width * 0.35, height * 0.46, "2 退出游戏");
	FlushBatchDraw();
}
```

```cpp
//board.h
#pragma once 
#include <graphics.h>
#include <windows.h>
#include <cstdio>

const int black = 1, white = 2;
// 提供棋子的半径和打印棋子的静态方法
class chess {
public:
	const static int radius = 8;  // 棋子的半径
	friend class board;
	static void printChess(int x, int y, int color) { // 画棋子 	// 画面坐标，非棋盘坐标
		setlinestyle(PS_SOLID, 2); // 这里设置当前边框样式
		setlinecolor(color);

		setfillstyle(BS_SOLID); // 设置当前填充样式
		setfillcolor(color);  // 设置填充颜色
		fillcircle(x, y, radius); // 画有边框的填充圆
	}
};

// 实现二维数组运算的重载
// 实现双下标的方式对元素进行赋值和读取的操作
class row {
private:
	const static int size = 20; 
	int *p; // 一维数组
public:
	row() { p = new int[size]; } // 申请行内存
	~row() { delete[] p; }           // 释放内存
	int &operator[](int idx) { 
		if (idx >= size) exit(-1); // 若下标值大于元素个数，则终止程序

		return p[idx];  // 返回该元素本身
	}
};

class board
{
private:
	int unit = 30; // 单元格大小
	const static int width = 20; // 棋盘大小
	row *used_board;  // 棋盘状态 
public:     
	board() {
		used_board = new row[width];

		for (int i = 0; i < width; ++i) // 初始全部为空
			for (int j = 0; j < width; ++j)
				used_board[i][j] = 0; // 0为空; 1为黑棋; 2为白棋(black = 1, white = 2) 
	}
	~board() { delete [] used_board; }

	int getWidth() { return width;  }
	int getUnit() { return unit; }

	void printBoard();  // 打印棋盘
	void printChesses(chess *temp); // 根据used_board情况打印所有棋子, 用作暂停后重绘整体棋局

	bool whoWin();  // 判断输赢 
	row &operator[](int idx) {
		if (idx >= width) exit(-1);  // 若下标值大于元素个数，则终止程序
		 //返回一个row类型的对象，用row类的对象调用第二个下标值，再返回row类中的元素
		return used_board[idx]; 
	} 
};
```

```cpp
//board.cpp
#include "board.h" 

char buffer[5];
char* change(int x) { // 转换数字为字符串
	sprintf_s(buffer, "%d", x);
	return buffer;
}

void board::printBoard()
{
	setcolor(BLACK); // 设置画线颜色和文本颜色，相当于同时使用setlinecolor和settextcolor
	setlinestyle(PS_SOLID, 1); // 设置画线样式，实线，宽2
	for (int i = 2; i < width; ++i) { // 绘制棋盘
		line(unit, i * unit, width * unit, i * unit); // 横线
		line(i * unit, unit, i * unit, width * unit); // 竖线
	} 

	setlinestyle(PS_SOLID, 2); // 设置画线样式，实线，宽2
	line(1 * unit, 1 * unit, width * unit, 1 * unit);  // 绘制外面的四条线
	line(width * unit, 1 * unit, width * unit, width * unit);
	line(1 * unit, 1 * unit, 1 * unit, width * unit);
	line(1 * unit, width * unit, width * unit, width * unit);
	
	setbkmode(0);  // 设置图案填充和文字输出时的背景模式，默认为当前背景色
	//settextcolor(BLACK);  // 设置文字颜色 //前面已经设置过了
	settextstyle(16, 0, _T("宋体")); // 设置文字高度和字体
	// 对棋盘线标上下标
	for (int i = 1; i <= width; ++i)  // 纵坐标
		outtextxy(unit - 25, i * unit - 8, change(i));
	for (int j = 1; j <= width; ++j)
		outtextxy(j * unit, width * unit + 10, 'A' + j - 1);

	// 五子棋/围棋的5个黑点
	setfillcolor(BLACK); // 设置当前的填充颜色
	fillcircle(4 * unit, 4 * unit, 3); // 左上 // 使用当前线形和当前填充样式绘制有外框的填充圆
	fillcircle(17 * unit, 4 * unit, 3); // 右上
	fillcircle(4 * unit, 17 * unit, 3); // 左下
	fillcircle(17 * unit, 17 * unit, 3); // 右下

	//fillcircle(width / 2 * unit, width / 2 * unit, 3); // 中间
	//setbkmode(0);
	//settextstyle(16, 0, _T("宋体"));
	//settextcolor(BLACK);
	//outtextxy(510, 40, "棋子坐标为:");
	//outtextxy(520, 60, "  行  列");
	//outtextxy(510, 100, "该:");
	//outtextxy(510, 130, "白棋步数：");
	//outtextxy(510, 160, "黑棋步数：");
}

void board::printChesses(chess *temp)  // 根据used_board的情况打印所有出现的棋子
{
	for (int i = 1; i <= width; ++i) {
		for (int j = 1; j <= width; ++j) {
			if (used_board[i - 1][j - 1] == 1) temp->printChess(i * unit, j * unit, BLACK);
			else if (used_board[i - 1][j - 1] == 2) temp->printChess(i * unit, j * unit, WHITE);
		}
	}
	FlushBatchDraw();
}
 
bool board::whoWin()
{		
	HWND hwnd = GetHWnd(); // windows窗口句柄
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < width; ++j) { //黑棋判断赢了
			if (
				(used_board[i][j] == 1 && used_board[i + 1][j] == 1 && used_board[i + 2][j] == 1 && used_board[i + 3][j] == 1 && used_board[i + 4][j] == 1) || // 横向
				(used_board[i][j] == 1 && used_board[i][j + 1] == 1 && used_board[i][j + 2] == 1 && used_board[i][j + 3] == 1 && used_board[i][j + 4] == 1) || // 纵向
				(used_board[i][j] == 1 && used_board[i + 1][j + 1] == 1 && used_board[i + 2][j + 2] == 1 && used_board[i + 3][j + 3] == 1 && used_board[i + 4][j + 4] == 1) || // 左上到右下
				(used_board[i][j] == 1 && used_board[i - 1][j + 1] == 1 && used_board[i - 2][j + 2] == 1 && used_board[i - 3][j + 3] == 1 && used_board[i - 4][j + 4] == 1)  // 右上到左下
			) {
				MessageBox(hwnd, TEXT("黑棋赢！"), "胜利：", MB_OK);
				system("pause");
				return true;
			} else if ( 	//白棋判断赢了
				(used_board[i][j] == 2 && used_board[i + 1][j] == 2 && used_board[i + 2][j] == 2 && used_board[i + 3][j] == 2 && used_board[i + 4][j] == 2) || // 横向
				(used_board[i][j] == 2 && used_board[i][j + 1] == 2 && used_board[i][j + 2] == 2 && used_board[i][j + 3] == 2 && used_board[i][j + 4] == 2) || // 纵向
				(used_board[i][j] == 2 && used_board[i + 1][j + 1] == 2 && used_board[i + 2][j + 2] == 2 && used_board[i + 3][j + 3] == 2 && used_board[i + 4][j + 4] == 2) || // 左上到右下
				(used_board[i][j] == 2 && used_board[i - 1][j + 1] == 2 && used_board[i - 2][j + 2] == 2 && used_board[i - 3][j + 3] == 2 && used_board[i - 4][j + 4] == 2)  // 右上到左下
			) {
				MessageBox(hwnd, TEXT("白棋赢！"), "胜利：", MB_OK);
				system("pause");
				return true;
			}
		}
	}
	return false;
}
```
下面的玩家信息输入界面和退出界面没有实现...
```cpp
//game.h
#pragma once
#include "screen.h"
#include "board.h" 

#include <cmath>
#include <graphics.h>
// 开始界面；玩家信息输入界面；下棋界面；暂停界面；退出 
enum status {START, RAWINPUT, PLAY, PAUSE, EXIT}; 

class game {
private:
	screen *my_screen;
	board  my_board;
	chess  *my_chess;
	status game_status, old_game_status;  // 游戏状态	
	int player_exchange;

	void StartOrNot();	 // 开始或退出
	void PauseOrNot(); // 暂停或退出
	void End() {
		delete my_screen;
		delete my_chess;
	};

	void BasicShow();  // 不会变的(棋盘和部分屏幕)
	void UpdateWithInput(); // 用户点击棋盘更新used_board状态
	void Circle();  // 游戏主循环

	void WriteBoardRecord();  // 存档，棋盘数据
	void WritePlayRecord();  // 存档，玩家数据
	void ReadBoardRecord(); // 读档，棋盘数据
	void ReadPlayRecord(); // 读档，玩家数据

public:
	game() {
		my_screen = new screen; // 先有屏幕
		my_board = *new board; // 棋盘在屏幕上面
		my_chess = new chess; 
		old_game_status = game_status = START;  // 先是开始界面
		player_exchange = 1;  // 开始是黑棋先行
	} 
	void Load() { // 唯一的公共接口，加载游戏
		StartOrNot();
		Circle();
		End();
	}
}; 
```
没有玩家信息，读档和存档函数都懒得实现。
```cpp
//game.cpp
#include "game.h"

void game::StartOrNot()
{
	my_screen->repeatPlayMusic();  // 循环播放音乐 
	if (game_status == START) {
		my_screen->startMenu(); // 开始界面 

		char input;
		do {
			input = _getch(); // 非功能键、控制键、数字键盘的码值
			if (input == 'a') {
				game_status = PLAY; // 开始新游戏
				break;
			}
			else if (input == 'b') {
				game_status = EXIT; // 直接退出
				exit(0);
			}
		} while (input != 'a' || input != 'b'); // 其他键继续
	}
}

void game::BasicShow()  // 打印游戏全部基本不变的要素
{
	my_screen->printScreen();
	my_board.printBoard();
	my_board.printChesses(my_chess);
	FlushBatchDraw();
}

void game::PauseOrNot()
{
	if (game_status == PAUSE) {
		my_screen->pauseMenu(); // 暂停界面
		old_game_status = game_status; // 记录之前的状态
		 
		char input;
		do {
			input = _getch(); // 阻塞进程
			if (input == '1') { // 非功能键、控制键、数字键盘的码值
				game_status = PLAY; 
				return;
			} else if (input == '2') {
				game_status = EXIT;
				exit(0);
			}
		} while (input != '1' || input != '2');
	} 
	old_game_status = PLAY; 
}

void game::UpdateWithInput()
{
	if (game_status == PLAY) {
		MOUSEMSG m;   // 定义鼠标信息
		while (MouseHit()) { // 鼠标发生消息
			m = GetMouseMsg();  // 得到鼠标消息
			int a = 0, b = 0, x = 0, y = 0;  // 交叉点的棋盘坐标和屏幕坐标
			int bw = my_board.getWidth(), u = my_board.getUnit();
			
			int flag = 0;
			if (m.uMsg == WM_LBUTTONDOWN) {  // 单击鼠标左键
				// 得到棋子的位置
				for (int i = 1; i <= bw; ++i) {
					for (int j = 1; j <= bw; ++j) {
						if (abs(m.x - i * u) <= 12 && abs(m.y - j * u <= 12)) {
							a = i - 1, b = j - 1;
							x = i * u, y = j * u;
							flag = 1;  break;
						}
					}
					if (flag) break;
				}
				if ((x >= 30 && y >= 30) && (x <= bw * u && y <= bw * u) && my_board[a][b] == 0) {
					if (player_exchange % 2 == 1) my_board[a][b] = 1;  // 下黑棋(奇数)
					else my_board[a][b] = 2; // 下白棋(偶数)
					
					my_chess->printChess(x, y, my_board[a][b] == 1 ? BLACK : WHITE); // 即时更新，绘制棋子
					FlushBatchDraw();
					player_exchange++; // 换棋手
					if (my_board.whoWin()) exit(0);
				}
			}
		}

		char input;
		if (_kbhit()) {
			input = _getch();
			if (input == 27) game_status = PAUSE;  // 用户输入esc，暂停
			else if (input == 's') { }  // 存档
			else if (input == 'r') { }  // 悔棋
		}
	}
}

void game::Circle() // 实现游戏主循环
{
	if (game_status == PLAY) BasicShow(); // 如果进入游戏，显示屏幕和棋盘
	while (true) {
		if (old_game_status == PAUSE) BasicShow(); // 从暂停界面恢复到棋盘
		UpdateWithInput(); // 等待输入
		PauseOrNot();  // 判断是否进入暂停状态
	}
}
```
400多行，不大。就是这样了。

不过五子棋还是可以继续做下去，把上面的功能都实现。然后，还可以做：比如使用不同的按键asdw和上下左右，支持双人输入，而不是共用一个鼠标?；实现人机对战；实现联网对战等等。

