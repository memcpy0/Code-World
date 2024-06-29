@[toc]

## 前言
除了利用控制台打印字符，我们还可以用图片、音乐等素材，开发出更加精彩的游戏。
## 点子
这里开发出图形版的flappy bird。
## 具体实现
### 1. 导入图片背景和小鸟
使用loadimage和putimage，导入背景图和小鸟图片。

其中，文件名路径都是LPCTSTR，这是什么类型？简单说，这是一个字符串指针。原因在于用路径中可能有英文字符，也可能有中文字符，中文字符使用两个字节作为一个字符(汉字机内码)。官方文档明确提及了这个问题：

> 中文版错误提示如下：error C2665: “outtextxy”: 2 个重载中没有一个可以转换所有参数类型
> 英文版错误提示如下：error C2665: ‘outtextxy’ : none of the 2 overloads could convert all the argument types
同样的，对于其他一些包含字符串调用的函数，例如 loadimage、drawtext 等，也会遇到类似问题。
【错误原因】
简单来说，这是由于字符编码问题引起的。VC6 默认使用的 MBCS 编码，而 VC2008 及高版本 VC 默认使用的 Unicode 编码。以下详细解释这个问题：
用 char 表示字符时，英文占用一个字节，中文站用两个字节。这样有一个严重的问题：两个连续字节，究竟是两个英文字符，还是一个中文字符？为了解决这个问题，Unicode 编码诞生了。Unicode 编码不管中文英文都用两个字节表示。
对于 MBCS 编码，字符变量用 char 定义。对于 Unicode 编码中，字符变量用 wchar_t 定义。为了提高代码的自适应性，微软在 tchar.h 里面定义了 TCHAR，而 TCHAR 会根据项目定义的编码，自动展开为 char 或 wchar_t。
在 Windows API 和 EasyX 里面的大多数字符串指针都用的 LPCTSTR 或 LPTSTR 类型，LPCTSTR / LPTSTR 就是“Long Point (Const) Tchar STRing”的缩写。所以可以认为，LPCTSTR 就是const TCHAR *，LPTSTR 就是TCHAR *。
于是，在 VS2008 里面，给函数传递 char 字符串时，就会提示前述错误。

>为了让代码适应char和wchar_t两种情况，可以用TCHAR宏，这个宏当项目定义为unicode时表示wchar_t，否则表示char。
对应的，还有 _T("") 宏用来表示字符串。

```cpp
#include <easyx.h>
#include <conio.h>

int main() {
	initgraph(350, 600);
	IMAGE img_bk, img_bird;
	loadimage(&img_bk, _T("..\\source\\background.jpg"));
	putimage(0, 0, &img_bk);

	loadimage(&img_bird, _T("..\\source\\bird2.jpg"));
	putimage(100, 200, &img_bird);
	_getch();
	closegraph();
	return 0;
}
```
效果：
				 		![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020144244551.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 2. 用遮罩图解决小鸟边框
小鸟的图片不是透明的，带有明显的白色边框，因此，我们需要利用小鸟的遮罩图bird1.jpg，它与bird2.jpg像素一一对应，**bird1中白色的像素区域将bird2中对应的像素显示，bird1中黑色的区域将bird2中对应的像素隐藏**。对应的遮罩图可以通过PS抠图生成。

这里必须了解三元光栅操作码。看文档就可以了。
```cpp
#include <easyx.h>
#include <conio.h>

int main() {
	initgraph(350, 600);
	IMAGE img_bk;
	loadimage(&img_bk, _T("..\\source\\background.jpg"));
	putimage(0, 0, &img_bk);

	IMAGE img_bd1, img_bd2;
	loadimage(&img_bd1, _T("..\\source\\bird1.jpg"));
	loadimage(&img_bd2, _T("..\\source\\bird2.jpg"));
	
	//putimage(100, 200, &img_bd1, DSTINVERT); //绘制出的像素颜色 = NOT 屏幕颜色
    putimage(100, 200, &img_bd1, NOTSRCERASE); //绘制出的像素颜色 = NOT (屏幕颜色 OR 图像颜色)

	//putimage(100, 200, &img_bd2, SRCAND); //绘制出的像素颜色 = 屏幕颜色 AND 图像颜色
	//putimage(100, 200, &img_bd2, SRCCOPY); //绘制出的像素颜色 = 图像颜色
	//putimage(100, 200, &img_bd2, SRCERASE); //绘制出的像素颜色 = (NOT 屏幕颜色) AND 图像颜色
	putimage(100, 200, &img_bd2, SRCINVERT); //绘制出的像素颜色 = 屏幕颜色 XOR 图像颜色
	//putimage(100, 200, &img_bd2, SRCPAINT); //绘制出的像素颜色 = 屏幕颜色 OR 图像颜色

	_getch();
	closegraph();
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020155436274.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 3. 用游戏框架重构flappy bird游戏
将前面的字符版flappy bird的游戏框架搬过来，让小鸟自由下落和操作上升：
```cpp
#include <easyx.h>
#include <conio.h>
#include <windows.h>

IMAGE img_bk, img_bd1, img_bd2, img_bar_up1, img_bar_up2, img_bar_down1, img_bar_down2;
int bird_x, bird_y;

void initgame() {
	initgraph(350, 600);
	loadimage(&img_bk, _T("..\\source\\background.jpg")); //350 x 600
	loadimage(&img_bd1, _T("..\\source\\bird1.jpg")); //34 x 24
	loadimage(&img_bd2, _T("..\\source\\bird2.jpg"));
	loadimage(&img_bar_up1, _T("..\\source\\bar_up1.gif")); //140 x 600
	loadimage(&img_bar_up2, _T("..\\source\\bar_up2.gif"));
	loadimage(&img_bar_down1, _T("..\\source\\bar_down1.gif"));
	loadimage(&img_bar_down2, _T("..\\source\\bar_down2.gif"));

	bird_x = 50, bird_y = 200;
	BeginBatchDraw();
}

void show() {
	//显示背景
	putimage(0, 0, &img_bk);
	//显示小鸟
    putimage(bird_x, bird_y, &img_bd1, NOTSRCERASE); //绘制出的像素颜色 = NOT (屏幕颜色 OR 图像颜色)
	putimage(bird_x, bird_y, &img_bd2, SRCINVERT); //（XOR）方式 绘制出的像素颜色 = 屏幕颜色 XOR 图像颜色
	//显示上面的障碍物
	putimage(150, -300, &img_bar_up1, NOTSRCERASE);
	putimage(150, -300, &img_bar_up2, SRCINVERT); 
	//显示下面的障碍物
	putimage(150, 400, &img_bar_down1, NOTSRCERASE);
	putimage(150, 400, &img_bar_down2, SRCINVERT);

	FlushBatchDraw();
}

void updateWithoutInput() {
	if (bird_y < 570)  bird_y += 5;
	Sleep(150);  //减缓小鸟的下落速度和柱子移动速度
}

void updateWithInput() {
	char input;
	if (_kbhit()) {
		input = _getch();
		if (input == ' ' && bird_y > 20) bird_y -= 30;
	}
}

void gameover() {
	EndBatchDraw();
	_getch();
	closegraph();
}

int main() {
	initgame();
	while (1) {
		show();
		updateWithInput();
		updateWithoutInput();
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020170843829.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 4. 导入声音
播放音乐需要用mciSendString函数。需要先打开(open)音乐，再就可以循环(play ... repeat)播放、仅播放一次(play)、关闭(close)前面的音乐。
```cpp
#pragma comment(lib, "Winmm.lib")

void initgame() {
	...
	mciSendString(_T("open ..\\source\\background.mp3 alias bk_music"), NULL, 0, NULL); //打开音乐
	mciSendString(_T("play bk_music repeat"), NULL, 0, NULL);  //循环播放音乐
	...
}

void updateWithInput() {
	char input;
	if (_kbhit()) {
		input = _getch();
		if (input == ' ' && bird_y > 20) {
			bird_y -= 30;
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);  //先把前面一次的音乐关闭
			//打开音乐
			mciSendString(_T("open ..\\source\\Jump.mp3 alias jpmusic"), NULL, 0, NULL); 
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);     //仅播放一次
		}
	}
}
...
```

### 5. 实现柱子的移动和碰撞检测
这里需要范围检查，不仅仅是像字符版那样。
```cpp
#include <easyx.h>
#include <conio.h>
#include <string>
#include <windows.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

IMAGE img_bk, img_bd1, img_bd2, img_bar_up1, img_bar_up2, img_bar_down1, img_bar_down2;
int bird_x, bird_y;
int bar_yTop, bar_yDown, bar_x; //障碍物坐标
HWND hwnd;
//int score;           //分数
int exitflag = 0; //退出标记

void initgame() {
	initgraph(400, 600);
	loadimage(&img_bk, _T("..\\source\\background.jpg"), 400, 600, false); //350 x 600
	loadimage(&img_bd1, _T("..\\source\\bird1.jpg")); //34 x 24
	loadimage(&img_bd2, _T("..\\source\\bird2.jpg"));
	loadimage(&img_bar_up1, _T("..\\source\\bar_up1.gif")); //140 x 600
	loadimage(&img_bar_up2, _T("..\\source\\bar_up2.gif"));
	loadimage(&img_bar_down1, _T("..\\source\\bar_down1.gif"));
	loadimage(&img_bar_down2, _T("..\\source\\bar_down2.gif"));

	mciSendString(_T("open ..\\source\\background.mp3 alias bk_music"), NULL, 0, NULL); //打开音乐
	mciSendString(_T("play bk_music repeat"), NULL, 0, NULL);  //循环播放音乐

	hwnd = GetHWnd();
	bird_x = 50, bird_y = 250;
	bar_yTop = 300, bar_yDown = 400, bar_x = 220;
	//score = 0;
	BeginBatchDraw();
}

void show() {
	//显示背景
	putimage(0, 0, &img_bk);
	//显示小鸟
    putimage(bird_x, bird_y, &img_bd1, NOTSRCERASE); //绘制出的像素颜色 = NOT (屏幕颜色 OR 图像颜色)
	putimage(bird_x, bird_y, &img_bd2, SRCINVERT); //（XOR）方式 绘制出的像素颜色 = 屏幕颜色 XOR 图像颜色

	//outtextxy(200, 350, _T("score"));
	//显示上面的障碍物
	putimage(bar_x, -300, &img_bar_up1, NOTSRCERASE);
	putimage(bar_x, -300, &img_bar_up2, SRCINVERT);
	//显示下面的障碍物
	putimage(bar_x, bar_yDown, &img_bar_down1, NOTSRCERASE);
	putimage(bar_x, bar_yDown, &img_bar_down2, SRCINVERT);

	FlushBatchDraw();
}

void updateWithoutInput() {
	bar_x -= 5;
	if (bar_x + 140 <= 0) bar_x = 300; //重新出现柱子
	if (bird_x >= bar_x && bird_x <= bar_x + 140) { //判断是否相撞
		if (bird_y <= bar_yTop || bird_y >= bar_yDown) { 
			MessageBox(hwnd, TEXT("胜败乃兵家常事，少侠请重新再来！"), _T("结果"), MB_OK);
			system("pause");
			exitflag = 1;  
		 } //else score++;
	}
	if (bird_y < 570)  bird_y += 5; //小鸟自己下落
	Sleep(150);  //减缓小鸟的下落速度和柱子移动速度
}

void updateWithInput() {
	char input;
	if (_kbhit()) {
		input = _getch();
		if (input == ' ' && bird_y > 20) {
			bird_y -= 20;
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);  //先把前面一次的音乐关闭
			mciSendString(_T("open ..\\source\\Jump.mp3 alias jpmusic"), NULL, 0, NULL); //打开音乐
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);     //仅播放一次
		}
	}
}

void gameover() {
	EndBatchDraw();
	_getch();
	closegraph();
}

int main() {
	initgame();
	while (1) {
		show();
		updateWithoutInput();
		updateWithInput(); 
		if (exitflag) break;
	}
	gameover();
	return 0;
}
```
效果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191020204527618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


