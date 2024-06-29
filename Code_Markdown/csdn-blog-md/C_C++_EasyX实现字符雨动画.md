@[toc]

这些代码写在VS2019。
## 实现过程
### 1. 静态字符阵列
将EasyX的绘图设备作为一个平面，划分为多个等大小的格子，每个格子输出一个绿色的字符。注意：雨是从高处往下落的，所以下面可能会有空白。
```cpp
#include <easyx.h>
#include <graphics.h>
#include <cstdlib>
#include <conio.h>
#include <ctime>
#define Height 800 // 画面大小 
#define Width 1000
#define CharSize 20  // 字符大小

int main() {
	const int heightNum = Height / CharSize;  // 40
	const int widthNum = Width / CharSize; // 50

	//CharRain存储对应字符矩阵中需要输出字符的ASCII码 40 x 50
	int CharRain[heightNum][widthNum]; // 字符矩阵, 与绘图设备对应
	int ColNum[widthNum];  // 每一列的有效字符个数 // 每一列多少行

	srand((unsigned int)time(NULL)); //
	for (int i = 0; i < widthNum; ++i) {
		ColNum[i] = (rand() % (heightNum * 9 / 10)) + heightNum / 10; // 0-44 + 5
		for (int j = 0; j < ColNum[i]; ++j) {
			CharRain[j][i] = (rand() % 26) + 65; // 产生A-Z的随机序列 // 第i列的第j行
		}
	}
	
	initgraph(Width, Height);
	setbkmode(0);
	setbkcolor(BLACK); // 背景色
	cleardevice();
	BeginBatchDraw();
	
	settextstyle(25, 10, "Courier");
	settextcolor(GREEN);
	// 形成雨
	for (int i = 0; i < widthNum; ++i) {
		int x = i * CharSize;
		for (int j = 0; j < ColNum[i]; ++j) {
			int y = j * CharSize;
			outtextxy(x, y, CharRain[j][i]); // 输出当前字符
		}
	}
	FlushBatchDraw();
	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}
```


![在这里插入图片描述](https://img-blog.csdnimg.cn/20191026103757567.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 2. 一场下落的字符雨
实现每一列字符的下落效果，直到该列字符被填满了为止。
```cpp
#include <easyx.h>
#include <graphics.h>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>
#define Height 800 // 画面大小 
#define Width 1000
#define CharSize 20  // 字符大小

int main() {
	const int heightNum = Height / CharSize;  // 40
	const int widthNum = Width / CharSize; // 50

	//CharRain存储对应字符矩阵中需要输出字符的ASCII码 40 x 50
	int CharRain[heightNum][widthNum]; // 字符矩阵, 与绘图设备对应
	int ColNum[widthNum];  // 每一列的有效字符个数 // 每一列多少行

	srand((unsigned int)time(NULL)); // 初始化字符矩阵
	for (int i = 0; i < widthNum; ++i) {
		ColNum[i] = (rand() % (heightNum * 9 / 10)) + heightNum / 10; // 0-44 + 5
		for (int j = 0; j < ColNum[i]; ++j) {
			CharRain[j][i] = (rand() % 26) + 65; // 产生A-Z的随机序列 // 第i列的第j行
		}
	}

	initgraph(Width, Height);
	setbkmode(0);
	setbkcolor(BLACK); // 背景色
	cleardevice();
	BeginBatchDraw();

	settextstyle(25, 10, "Courier");
	settextcolor(GREEN);
	// 形成雨 下面每一帧让字符向下移动，然后最上面产生新的字符
	while (1) {
		for (int i = 0; i < widthNum; ++i) { 
			if (ColNum[i] < heightNum - 1) { // 当这一列没有填满时
				for (int j = ColNum[i] - 1; j >= 0; --j)
					CharRain[j + 1][i] = CharRain[j][i];  // 每个字符下移一格
				CharRain[0][i] = (rand() % 26) + 65;  //这一列 最上面一个产生随机ASCII码
				ColNum[i]++;  // 这一列的有效字符个数+1
			}
		}
		// 输出整个字符矩阵
		for (int i = 0; i < widthNum; ++i) {
			int x = i * CharSize; 
			for (int j = 0; j < ColNum[i]; ++j) {
				int y = j * CharSize;
				outtextxy(x, y, CharRain[j][i]); // 输出当前字符
			} 
		}
		FlushBatchDraw();
		Sleep(100);
		clearrectangle(0, 0, Width - 1, Height - 1);  // 清楚画面中的全部矩形区域
	}
	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}
```
### 3. 字符雨动画
第三步，当某列字符填满后**使其颜色变暗**，然后**重新生成该列的随机字符**，实现无限循环的动画效果。

变暗？我们知道easyx中可以使用RGB来形成颜色，而绿色为`#define	GREEN			0x00AA00`，即RGB(0, 255, 0)，因此，我们逐渐减少G的值就行了。
```cpp
#include <easyx.h>
#include <graphics.h>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>
#define Height 800 // 画面大小 
#define Width 1000
#define CharSize 18  // 字符大小

int main() {
	const int heightNum = Height / CharSize;  // 40
	const int widthNum = Width / CharSize; // 50

	//CharRain存储对应字符矩阵中需要输出字符的ASCII码 40 x 50
	int CharRain[heightNum][widthNum]; // 字符矩阵, 与绘图设备对应
	int ColNum[widthNum];  // 每一列的有效字符个数 // 每一列多少行
	int ColorG[widthNum];  // 每一列的字符颜色

	srand((unsigned int)time(NULL)); // 初始化字符矩阵
	for (int i = 0; i < widthNum; ++i) {
		ColNum[i] = (rand() % (heightNum * 9 / 10)) + heightNum / 10; // 0-44 + 5
		ColorG[i] = 255;
		for (int j = 0; j < ColNum[i]; ++j) {
			CharRain[j][i] = (rand() % 26) + 65; // 产生A-Z的随机序列 // 第i列的第j行
		}
	}

	initgraph(Width, Height);
	setbkmode(0);
	setbkcolor(BLACK); // 背景色
	cleardevice();
	BeginBatchDraw();

	settextstyle(25, 10, "Courier");
	settextcolor(GREEN);
	// 形成雨 下面每一帧让字符向下移动，然后最上面产生新的字符
	while (1) {
		for (int i = 0; i < widthNum; ++i) { 
			if (ColNum[i] < heightNum - 1) { // 当这一列没有填满时
				for (int j = ColNum[i] - 1; j >= 0; --j)
					CharRain[j + 1][i] = CharRain[j][i];  // 每个字符下移一格
				CharRain[0][i] = (rand() % 26) + 65;  //这一列 最上面一个产生随机ASCII码
				ColNum[i]++;  // 这一列的有效字符个数+1
			}
			else {  // 这一列填满后
				if (ColorG[i] > 60) ColorG[i] -= 20; // 让满的这一列逐渐变暗
				else {
					ColNum[i] = (rand() % (heightNum * 9 / 10)) + heightNum / 10; // 这一列的字符个数
					ColorG[i] = (rand() % 75) + 180; // 这一列字符的颜色
					for (int j = 0; j < ColNum[i]; ++j)
						CharRain[j][i] = (rand() % 26) + 65; // 重新初始化这一列字符
				}
			}
		}
		// 输出整个字符矩阵
		for (int i = 0; i < widthNum; ++i) {
			int x = i * CharSize; 
			for (int j = 0; j < ColNum[i]; ++j) {
				int y = j * CharSize;
				setcolor(RGB(0, ColorG[i], 0));
				outtextxy(x, y, CharRain[j][i]); // 输出当前字符
			} 
		}
		FlushBatchDraw();
		Sleep(120);
		clearrectangle(0, 0, Width - 1, Height - 1);  // 清楚画面中的全部矩形区域
	}
	EndBatchDraw();
	_getch();
	closegraph();
	return 0;
}
```
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20191026110941970.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
