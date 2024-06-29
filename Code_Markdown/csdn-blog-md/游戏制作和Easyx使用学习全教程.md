@[toc]

##  -1  读取图片的技巧：将图片内嵌到 exe 文件中 

注：Visual C++ Express（学习版）不支持资源编辑，无法创建资源文件，因此也就无法实现内嵌图片资源。
场景描述：张图片，通常会用相对路径方式指定一个外部文件。例如：·
```
loadimage(NULL, _T("test.jpg"));
```
这样，将编译后的 .exe 和 test.jpg 放在一起，就可以正确加载图片。许多游戏有几十个甚至上千个文件，就是有类似这样的许多外部数据。

但还有一些情况，希望图片能嵌入编译后的 .exe 里面，这样只需要拷贝一个 .exe 文件就能附带上所需图片。本文就介绍这种情况的实现方法。
- 什么是资源文件
windows 应用程序是可以包含各种“资源”的，例如：图标、对话框、菜单、快捷键等等，这些资源按照一定的格式，可以和 .exe 链接在一起。
我们所要做的，就是把图片放到资源中，然后从资源中加载图片。
- 操作步骤
以英文版的 VC6 和 VC2010 为例，嵌入资源的操作步骤如下：

    创建项目
        打开 VC6（或 VC2010 等其他版本），建立控制台应用程序，并建立相应 cpp 程序，确保可以正确编译执行。
    创建资源文件
        对于 VC6：点菜单 File -> New...，选择 Files 中的 Resource Script，并在右侧 File 中写入名称 test，点 OK 添加到项目中。VC 会默认打开 test.rc 文件，先关闭它，我们可以 FileView 找到新添加的 test.rc 文件。双击 test.rc，会在 Workspace 区中打开 Resource View 视图。
        对于 VC2010：在 Solution Explorer 中找到 Resource Files，右击，选择 Add -> New Item...，在弹出的新窗口中选择 Resource File (.rc)，在 Name 中写入名称 test，点 Add 添加。此时会自动切换到 Resource View 视图的选项卡。
        在新打开的 Resource View 视图中，会显示本项目中使用的资源，例如图标、位图、字符串等等。当然，现在还是空的。
    添加图片到资源文件中
        为了整齐，我们在项目路径下建立 res 文件夹，并将图片放入该文件夹内。举例，我们放入一个 bk.jpg 文件。
        添加 JPG 资源。
        对于 VC6：切换到 ResourceView 视图，右击 test resources，选择 Import...，导入 res\bk.jpg 文件。之后在 Custom Resource Type 中为资源取一个类型名称，例如"IMAGE"，点 OK。此时 VC 会在"IMAGE"下默认创建一个 IDR_IMAGE1 的资源，并以二进制形式打开。我们暂时关掉它。
        对于 VC2010：切换到 Resource View 视图，右击 test.rc，选择 Add Resource...，在弹出的窗口中点 Import... 按钮，选择 res\bk.jpg 文件。之后在 Custom Resource Type 中为资源取一个类型名称，例如"IMAGE"，点 OK。此时 VC 会在"IMAGE"下默认创建一个 IDR_IMAGE1 的资源，并以二进制形式打开。我们暂时关掉它。
        重命名资源。
        对于 VC6：右击 IDR_IMAGE1，选 Properties，将 ID 一栏的 IDR_IMAGE1 修改为符合其意义的名称，例如"Background"，切忌，一定要加上英文的双引号。这时，资源中可以看到 "IMAGE" / "Background" (注意都有双引号)。
        对于 VC2010：选中 IDR_IMAGE1，按 Alt + Enter 显示 Properties，将 ID 一栏的 IDR_IMAGE1 修改为符合其意义的名称，例如"Background"，切忌，一定要加上英文的双引号。这时，资源中可以看到 "IMAGE" / "Background" (注意都有双引号)。
    加载资源中的图片
    加载图片很简单，只需要指定“资源类型”和“资源名称”。例如我们前面的例子，资源类型是 "IMAGE"，资源名称是 "Background"，将这个图片资源显示在绘图窗体上可以这样做：
```c
loadimage(NULL, _T("IMAGE"), _T("Background"));
```

最后，编译程序，资源文件会自动和 exe 打包在一起。
- 特殊情况
1. 以资源 ID 的形式加载资源图片

使用图片的资源 ID 也是一种常用的加载资源的方法。默认情况下，将图片导入资源后，会自动生成一个 ID，并且会在 resource.h 里面定义这个 ID。这里说的，就是直接引用这个 ID 而不命名为字符串。

为了实现这个目的，需要首先引用资源头文件，然后用宏 MAKEINTRESOURCE 将 ID 转换为字符串。例如加载一个名称为 IDR_BACKGROUND 的资源：
```
#include "resource.h"
……
loadimage(NULL, _T("IMAGE"), MAKEINTRESOURCE(IDR_BACKGROUND));
```

2. 将 BMP 格式的图片嵌入资源

由于 BMP 格式的图片在资源中的情况特殊，导入资源后，会导致文件头丢失，从而引起加载失败。

所以，需要明确指定 BMP 图片的资源类型为其它类型，方法如下：

   在资源中导入 test.bmp 图片。默认会导入在 Bitmap 类别下，并命名为 IDB_BITMAP1。作为范例，我们修改这个资源 ID 为字符串“test_bmp”。编译项目，确保没有错误。
   以文本方式打开资源文件。
   对于 VC6，点菜单 File -> Open...，选中项目的资源文件 test.rc，底部的 Open as 选择 Text，点 Open 打开（如果此时资源视图已打开，会提示“This file is open for resource editing. Continuing will close the resource editor.”，点 OK）。
   对于 VC2010，在 Solution Explorer 里面找到资源文件 test.rc，右击，选择 View Code（如果此时资源视图已打开，会提示“The document 'xxx' is already open. Do you want to close it?”，点 Yes）。
   这样就可以以文本方式打开资源文件。
   在资源文件的文本中，找到这样的内容：
```c
/////////////////////////////////////////////////////////////////////////////
//
// Bitmap
//

test_bmp             BITMAP  DISCARDABLE     "test.bmp"
```

然后将这一行里面的 BITMAP 修改为自己定义的一个类型，例如 IMAGE：

test_bmp             IMAGE  DISCARDABLE     "test.bmp"


然后就可以按照前述方式加载这个图片资源，例如：
```
loadimage(NULL, _T("IMAGE"), _T("test_bmp"));
```
## 0：前言
针对人群

    职业不限，水平不限，男女不限。基本上，是从很基础的层次开始（但还是需要一点点基础）。

基础要求

    希望读者已经简单学过C语言。我不会从定义变量开始讲。
    要有英语基础，现阶段虽不是必须，但是学学英语总没错，以后有用。

学习目标

    能做简单的图形小游戏。
    讲解以简单实用为主，不要指望看了这几篇文章后就能过二级考试。
    目标，除了能做小游戏外，更重要的，是锻炼编程思想，以备将来做更大的程序。

学习方式

    以自学为主吧，有问题就在相关课程后面跟帖就行，一起讨论。

学习要求

    欲速则不达，不要指望一口气都看完，请务必每次看完后都写几个相关的程序。勤动手才能学好编程，
    请务必重视。

连载次数

    不确定，想到哪写到哪吧，我会把每次的文章目录发到这里。

使用平台及搭建方法

    请先正确安装 Visual C++ 2010。（注：教程最早是针对 vc6 环境的，内容正在逐步调整为针对 vc2010）
    请到 www.easyx.cn 首页下载最新版的 EasyX，并安装。
    可以参考这个视频：https://www.easyx.cn/readme/View.aspx?id=5

## 1：创建新项目
[本期目标]：学会在 VC 里面创建项目，并写简单的程序。

VC 写程序要有项目的概念，一个项目可以有多个 .cpp 文件，多个项目构成一个工作区。先记住这两个英文单词吧：

    Workspace: 工作区
    Project: 项目

现在开始创建一个新项目。

    VC6 创建新项目请参考这个视频：http://www.easyx.cn/readme/View.aspx?id=65
    VC2008 创建新项目请参考这个视频：http://www.easyx.cn/readme/View.aspx?id=85
    VC2010 与 VC2008 相似。

看明白后，自己动手建立项目试试，并输入以下代码：
```
#include <stdio.h>

int main()
{
	printf("Hello World!");
	return 0;
}
```

尤其是之前用 tc 的同学，请务必创建新项目试一试。成功执行后，会看到屏幕上有“Hello World!”几个字符。然后，再重新输入以下代码试试（无需理解代码含义）：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);
	line(200, 240, 440, 240);
	line(320, 120, 320, 360);
	getch();
	closegraph();

	return 0;
}
```

执行后应该可以看到屏幕正中央有一个十字。 看到该十字后，本节课结束。
## 2：简单绘图，学习单步执行
[本期目标]：学会简单绘图，并学会简单调试。

先看看上一课的代码，我加上了注释
```
#include <graphics.h>			// 绘图库头文件，绘图语句需要
#include <conio.h>				// 控制台输入输出头文件，_getch()语句需要 
int main()
{
	initgraph(640, 480);		// 初始化640x480的绘图屏幕
	line(200, 240, 440, 240);	// 画线(200,240) - (440,240)
	line(320, 120, 320, 360);	// 画线(320,120) - (320,360)

	_getch();					// 按任意键
	closegraph();				// 关闭绘图屏幕
	return 0;
}
```

解释一下：

    创建的绘图屏幕 640x480，表示横向有 640 个点，纵向有 480 个点。注意：左上角是原点 (0,0)，
    也就是说，y 轴和数学的 y 轴是相反的。
    
    _getch 实现按任意键功能，按任意键后，程序继续执行。否则，程序会立刻执行 closegraph 
    以至于看不到绘制的内容。

[作业]：用线条画出更多的图形，要求不少于 10 条直线。

[学习单步执行]：完成作业后（务必完成），开始试着单步执行刚才的程序，由于绘图和多线程等因素的限制，请务必按照以下步骤尝试（熟练了以后就不用了）：

    将 VC 取消最大化，并缩小窗口，能看到代码就行。
    按一下 F10（单步执行），会看到屏幕上出现一个黄色的小箭头，指示将要执行的代码。
    当箭头指向 initgraph 语句时，按 F10，能看到窗口发生了变化。
    将新的绘图窗口和 VC 并排放，相互不要有覆盖。这步很重要，否则绘图内容将会被 VC 窗口覆盖。
    F10 执行 getch 后，记得激活绘图窗口，并按任意键，否则程序不会继续执行。
    closegraph 后，直接按 F5 执行全部剩余程序，结束。

单步执行很重要，可以让你知道程序执行到哪里是什么效果，哪条语句执行出了问题等等。

更详细的调试资料，请看这里：http://pan.baidu.com/s/1eR6HT
该文档写的调试的东西比较多，看一下大概有个了解，以后都会用到。

[作业2]：仍然是写一个用直线绘制的图形，并熟悉调试过程。
## 3：熟悉更多的绘图语句
[学习目标]：学会更多的绘图语句
[常用的绘图语句]

    line(x1, y1, x2, y2); // 画直线 (x1,y1)-(x2,y2)，都是整形
    circle(x, y, r); // 画圆，圆心为 (x,y)，半径为 r
    putpixel(x, y, c); // 画点 (x,y)，颜色 c

还有很多，如画椭圆、圆弧、矩形、多边形，等等，请参考 EasyX 在线帮助 https://docs.easyx.cn

[设置颜色]：setlinecolor(c);    // 设置画线颜色，如 setlinecolor(RED) 设置画线颜色为红色

常用的颜色常量可以用：

    BLACK 黑　　　　DARKGRAY 深灰
    BLUE 蓝　　　　　LIGHTBLUE 亮蓝
    GREEN 绿　　　　LIGHTGREEN 亮绿
    CYAN 青　　　　　LIGHTCYAN 亮青
    RED   红　　　　　LIGHTRED   亮红
    MAGENTA 紫　　　LIGHTMAGENTA   亮紫
    BROWN 棕 　　　YELLOW 黄
    LIGHTGRAY 浅灰　WHITE 白

[配出更多的颜色]：颜色除了前面写的 16 种以外，还可以自由配色。格式：RGB(r, g, b)
r / g / b 分别表示红色、绿色、蓝色，范围都是 0~255。例如，RGB(255,0,0) 表示纯红色。

红色和绿色配成黄色，因此 RGB(255, 255, 0) 表示黄色。嫌调色麻烦可以用画笔里面的调色试试，调好了以后直接将数值抄过来就行。

例如，画两条红色浓度为 200 的直线，可以这么写：
```c
setlinecolor(RGB(200, 0, 0));
line(100, 100, 200, 100);
line(100, 120, 200, 120);
```
[用数字表示颜色]：除了用 RGB(r,g,b) 方式外，还可以用16进制表示颜色，格式：0xbbggrr
例如，setlinecolor(0x0000ff) 和 setlinecolor(RGB(255, 0, 0)) 是等效的。

[延时语句]：这个很简单，Sleep(n) 就可以表示 n 毫秒的延时。例如延时 3 秒，可以用 Sleep(3000);

[作业]

    简单看一下绘图库的帮助文件，了解更多的绘图语句。
    绘制更丰富的图形内容，不低于20行。
    将延时语句适当的插入上个作业的代码中，看看执行效果。

注：绘图语句不需要记住，用的时候翻翻手册就行。
## 4：结合流程控制语句来绘图
[学习目标]：熟练使用循环、判断语句

[熟悉for语句]：这步需要自学，看看自己手边的书，是怎样讲for语句的，简单看看就行。

[范例]：例如，画10条直线的代码：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	for(int y=100; y<200; y+=10)
		line(100, y, 300, y);

	_getch();
	closegraph();
	return 0;
}
```
换一下循环的范围和间隔，看看效果。

还可以用来画渐变色，例如：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	for(int y=0; y<256; y++)
	{
		setcolor(RGB(0,0,y));
		line(100, y, 300, y);
	}

	_getch();
	closegraph();
	return 0;
}
```

[熟悉if语句]：这步需要自学，看看自己手边的书，是怎样讲if语句的，简单看看就行。
配合 if 语句，实现红色、蓝色交替画线：
```
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	for(int y=100; y<200; y+=10)
	{
		if ( y/10 % 2 == 1)    // 判断奇数行偶数行
			setcolor(RGB(255,0,0));
		else
			setcolor(RGB(0,0,255));

		line(100, y, 300, y);
	}

	_getch();
	closegraph();
	return 0;
}
```
[作业]

    画围棋棋盘。
    画中国象棋的棋盘。
    画国际象棋的棋盘，看手册找到颜色填充语句，实现国际象棋棋盘的区块填充。
    自学 while 语句。

学到这里，已经可以画出很多东西了。把自己想象中的图案绘制一下吧。
## 5：数学知识在绘图中的运用
[学习目标]：理解数学的重要性
1. 最简单的，来个全屏的渐变色吧，是上一课的扩展。就是需要将 0~255 的颜色和 0~479 的 y 轴对应起来
	c 表示颜色，范围 0~255
	y 表示y轴，范围 0~479
	于是：
	c / 255 = y / 479
	c = y / 479 * 255 = y * 255 / 479 （先算乘法再算除法可以提高精度）
	
	看代码：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	int c;
	for(int y=0; y<480; y++)
	{
		c = y * 255 / 479;
		setlinecolor(RGB(0,0,c));
		line(0, y, 639, y);
	}

	_getch();
	closegraph();
	return 0;
}
```

试试效果吧。

2. 画一个圆形的渐变色

	首先，我们要用到圆形的基本公式：
	x * x + y * y = r * r
	
	让弧度从 0~2 * 3.14，然后需要根据弧度和半径算出 (x,y)，
	用 pi 表示圆周率
	用 r 表示半径
	用 a 表示弧度（小数）
	用 c 表示颜色
	
	于是：
	x = r * cos(a)
	y = r * sin(a)
	c = a * 255 / (2 * pi)

	看看代码：
```c
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define PI 3.14159265359

int main()
{
	initgraph(640, 480);

	int c;
	double a;
	int x, y, r = 200;
	for(a = 0; a < PI * 2; a += 0.0001)
	{
		x=(int)(r * cos(a) + 320 + 0.5);
		y=(int)(r * sin(a) + 240 + 0.5);
		c=(int)(a * 255 / (2 * PI) + 0.5);
		setlinecolor(RGB(c, 0, 0));
		line(320, 240, x, y);
	}

	_getch();
	closegraph();
	return 0;
}
```
[作业]：这次没什么作业，只是理解一下数学的重要性而已。如果读者还在念书，请重视数学。
## 6：实现简单动画
所谓动画，其实是连续显示一系列图形而已。 结合到程序上，我们需要以下几个步骤：

    绘制图像
    延时
    擦掉图像

循环以上即可实现动画。

举一个例子，我们实现一条直线从上往下移动：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	for(int y = 0; y < 480; y++)
	{
		// 绘制绿色直线
		setlinecolor(GREEN);
		line(0, y, 639, y);

		// 延时
		Sleep(10);

		// 绘制黑色直线（即擦掉之前画的绿线）
		setlinecolor(BLACK);
		line(0, y, 639, y);
	}

	closegraph();
	return 0;
}
```

再看一个例子，实现一个圆从左往右跳动：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	for(int x = 100; x < 540; x += 20)
	{
		// 绘制黄线、绿色填充的圆
		setlinecolor(YELLOW);
		setfillcolor(GREEN);
		fillcircle(x, 100, 20);

		// 延时
		Sleep(500);

		// 绘制黑线、黑色填充的圆
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, 100, 20);
	}

	closegraph();
	return 0;
}
```

也就是说，移动的间距小、延时短，动画就会越细腻。但当画面较复杂时，会带来画面的闪烁（怎样消除闪烁是以后的话题）。

[作业]：绘制一个沿 45 度移动的球，碰到窗口边界后反弹。
## 7：捕获按键，实现动画的简单控制
最常用的一个捕获按键的函数：_getch()
前几课，都把这个函数当做“按任意键继续”来用，现在我们用变量保存这个按键：
```c
char c = _getch();
```
然后再做判断即可。

不过程序执行到 _getch() 是会阻塞的，直到用户有按键才能继续执行。可游戏中总不能因为等待按键而停止游戏执行吧？所以，要有一个函数，判断是否有用户按键：_kbhit()
这个函数返回当前是否有用户按键，如果有，再用 _getch() 获取即可，这样是不会阻塞的。 即：
```c
char c;
if (_kbhit())
	c = _getch();
```

举一个简单的例子，如果有按键，就输出相关按键。否则，输出“.”。每隔 100 毫秒输出一次。按 ESC 退出。
注：ESC 的 ASCII 码是 27。

完整代码如下：
```c
#include <graphics.h>
#include <stdio.h>
#include <conio.h>

int main()
{
	char c = 0;
	while(c != 27)
	{
		if (_kbhit())
			c = _getch();
		else
			c = '.';
		
		printf("%c", c);
		Sleep(100);
	}

	return 0;
}
```

结合上一课的简单动画，就可以做出来靠按键移动的图形了吧，看以下代码，实现 a、d 控制圆的左右移动：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);
	
	int x = 320;
	
	// 画初始图形
	setlinecolor(YELLOW);
	setfillcolor(GREEN);
	fillcircle(x, 240, 20);
	
	char c = 0;
	while(c != 27)
	{
		// 获取按键
		c = _getch();
		
		// 先擦掉上次显示的旧图形
		setlinecolor(BLACK);
		setfillcolor(BLACK);
		fillcircle(x, 240, 20);
		
		// 根据输入，计算新的坐标
		switch(c)
		{
			case 'a': x-=2; break;
			case 'd': x+=2; break;
			case 27: break;
		}
		
		// 绘制新的图形
		setlinecolor(YELLOW);
		setfillcolor(GREEN);
		fillcircle(x, 240, 20);
		
		// 延时
		Sleep(10);
	}
	
	closegraph();
	return 0;
}
```
[作业]：请继续完成这个程序，实现以下功能：

    上下的控制；
    边界检测；
    结合 kbhit 实现惯性移动（即按一下方向键，圆就会一直向这个方向移动）

注：上下左右等按键的控制，会返回 2 个字符。由于该系列教程面向初学者，因此有兴趣的请查看 MSDN。
### 使用 getch() 获得方向键和更多的功能键
通常来说，getch() 可以返回用户键入的字符。对于一些小游戏，希望用方向键控制的时候，怎么做呢？

先看看 MSDN 的解释：
When reading a function key or an arrow key, _getch and _getche must be called twice; the first call returns 0 or 0xE0, and the second call returns the actual key code.

简单来说，当 getch() 返回 0 或 0xE0 时，就表示用户按了功能键，这时候需要再调用一次 getch()。第二次 getch() 返回的值表示功能键，比如：
	
	72 表示 上
	80 表示 下
	75 表示 左
	77 表示 右

更多的功能键码，可以自己试验得出。 
具体到程序中的应用，请参考“俄罗斯方块”的源代码：http://www.easyx.cn/samples/View.aspx?id=48

## 8：用函数简化相同图案的制作
实际中有许多类似的图案，如果一一单独绘制，太麻烦。于是，我们需要一个公用的绘制过程，就是函数。

例如，我们需要画5个三角形，位于不同的位置。我们可以将绘制单个三角形的过程写成函数，函数内是一个独立的程序段，这个绘制过程很简单。
然后，在需要绘制的时候，调用这个函数即可。可以通过参数来解决细微差异（图案的坐标、颜色等），例如：
```c
#include <graphics.h>
#include <conio.h>

// 在坐标 (x,y) 处，用颜色 c 绘制三角形
void sanjiaoxing(int x, int y, int c)
{
	// 设置画线颜色
	setlinecolor(c);

	// 画三角形的三条边
	line(x, y, x+50, y);
	line(x, y, x, y+50);
	line(x+50, y, x, y+50);
}

int main()
{
	initgraph(640, 480);	// 初始化图形窗口

	sanjiaoxing(100, 100, RED);
	sanjiaoxing(120, 160, BLUE);
	sanjiaoxing(140, 220, GREEN);
	sanjiaoxing(160, 120, BLUE);
	sanjiaoxing(160, 160, GREEN);
	sanjiaoxing(220, 140, GREEN);

	_getch();				// 按任意键继续
	closegraph();			// 关闭图形窗口
	return 0;
}
```

再结合循环等控制条件，就能绘制更复杂漂亮的图案了。试试运行下面程序，理解一下函数的用处：
```c
#include <graphics.h>
#include <conio.h>

void sanjiaoxing(int x, int y, int color)
{
	// 设置画线颜色
	setlinecolor(color);

	// 画三角形的三条边
	line(x, y, x+10, y);
	line(x, y, x, y+10);
	line(x+10, y, x, y+10);
}

int main()
{
	initgraph(640, 480);	// 初始化图形窗口

	for(int x = 0; x < 640; x += 10)
		for(int y = 0; y < 480; y += 10)
			sanjiaoxing(x, y, RGB(x*255/640, y*255/480, 0));

	_getch();				// 按任意键继续
	closegraph();			// 关闭图形窗口
	return 0;
}
```

运行效果： 

本节作业：
1. 绘制 Windows 自带游戏“扫雷”的初始界面。
2. 这个作业有点独特，仔细看下面这个数学过程：

    在二维平面上随机生成 3 个点 P[0]、P[1]、P[2]；
    随机生成 1 个点 P；
    绘制点 P；
    随机生成 [0, 2] 内的整数 n；
    令 P = P 与 P[n] 的中点；
    重复执行步骤 (3)～(5) 三万次。

问题是：以上步骤执行完以后，这三万个点在屏幕上会是个什么情况？有规律吗？很难想出来吧，那就写个程序把这个过程模拟一下，看看究竟是什么。
 
## 9：绘图中的位运算
位运算和绘图有什么关系？先举个例子来个感性认识：使用XOR运算可以实现擦除图形后不破坏背景，这在时钟程序中绘制表针是很有用的。稍后我们会给出这样的例子。

一、位运算的运算法则
位运算主要分 4 种：NOT、AND、OR、XOR，位运算的运算对象是二进制数（十进制要转换为二进制，计算机会自动转换）。

运算法则如下：

1. NOT

	表示“取反”，将二进制位的 1 变 0、0 变 1。
	C 语言用符号 ~ 表示。

	如：
	二进制： ~1101 = 0010
	用十进制表示就是：~13 = 2

2. AND
表示“并且”，只有两数的对应二进制位都为 1，结果的二进制位才为 1；否则，结果的二进制位为 0。
C语言用符号 & 表示。

	如：二进制：1101 & 0110 = 0100
	用十进制表示就是：13 & 6 = 4

3. OR
 表示“或者”，两数的对应二进制位只要有一个是 1，结果的二进制位就是 1；否则，结果的二进制位为 0。
C语言用符号 | 表示。

	如：
	二进制：0101 | 0110 = 0111
	用十进制表示就是：5 | 6 = 7

4. XOR

	表示“异或”，两数的对应二进制位不同，结果的二进制位为 1；相同，结果的二进制位为 0。
	C语言用符号 ^ 表示。 
	如：
	二进制：0101 ^ 1110 = 1011

	以上只是简单介绍一下，详细的还是请大家看课本上的讲解。

二、位运算的应用
位运算的应用很多，例如 AND 和 OR 在获取和设置标志位时经常使用。更多的，以后大家会逐渐遇到，暂时先记下有这么回事。

这里着重说一下 XOR 运算，它有一个重要的特性：(a ^ b) ^ b = a。也就是说，a ^ b 之后可能是某些其它数字，但是只要再 ^b 一下，就又成了 a。一些简单的加密就用的 XOR 的这个特性。

至于绘图，假如 a 是背景图案，b 是将要绘制的图案，只要用 XOR 方式绘图，连续绘两次，那么背景是不变的。

三、演示
我们来一个简单的绘图 XOR 运算演示：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);				// 初始化 640 x 480 的绘图窗口

	setlinestyle(PS_SOLID, 10);			// 设置线宽为 10，这样效果明显
	setlinecolor(0x08BAFF);				// 设置画线颜色为黄色
	rectangle(100, 100, 200, 200);		// 画一个黄色矩形框，当做背景图案

	setwritemode(R2_XORPEN);			// 设置 XOR 绘图模式
	setlinecolor(0x2553F3);				// 设置画线颜色为红色

	line(50, 0, 200, 300);				// 画红色线
	_getch();							// 等待按任意键
	line(50, 0, 200, 300);				// 画红色线（XOR 方式重复画线会恢复背景图案）
	_getch();							// 等待按任意键

	closegraph();						// 关闭绘图窗口
	return 0;
}
```

运行一下，屏幕出现黄色(0x08BAFF)的矩形框与红色(0x2553F3)的直线相交，矩形框与直线相交的部分，颜色变成了绿色(0x2DE90C)，这是因为 黄色(0x08BAFF) XOR 红色(0x2553F3) = 绿色(0x2DE90C)。

当再次以红色(0x2553F3)画线时，绿色部分消失了，还原为完整的绿色矩形框，这是因为 绿色(0x2DE90C) XOR 红色(0x2553F3) = 黄色(0x08BAFF)。

四、完整的范例
来一个相对完整的范例吧，就是钟表程序，三个表针用的都是 XOR 方式绘制，请大家运行体会一下 XOR 的作用：
```c
#include <graphics.h>
#include <conio.h>
#include <math.h>

#define PI 3.14159265359

void Draw(int hour, int minute, int second)
{
	double a_hour, a_min, a_sec;					// 时、分、秒针的弧度值
	int x_hour, y_hour, x_min, y_min, x_sec, y_sec;	// 时、分、秒针的末端位置
	
	// 计算时、分、秒针的弧度值
	a_sec = second * 2 * PI / 60;
	a_min = minute * 2 * PI / 60 + a_sec / 60;
	a_hour= hour * 2 * PI / 12 + a_min / 12;
	
	// 计算时、分、秒针的末端位置
	x_sec = 320 + (int)(120 * sin(a_sec));
	y_sec = 240 - (int)(120 * cos(a_sec));
	x_min = 320 + (int)(100 * sin(a_min));
	y_min = 240 - (int)(100 * cos(a_min));
	x_hour= 320 + (int)(70 * sin(a_hour));
	y_hour= 240 - (int)(70 * cos(a_hour));

	// 画时针
	setlinestyle(PS_SOLID, 10, NULL);
	setlinecolor(WHITE);
	line(320, 240, x_hour, y_hour);

	// 画分针
	setlinestyle(PS_SOLID, 6, NULL);
	setlinecolor(LIGHTGRAY);
	line(320, 240, x_min, y_min);

	// 画秒针
	setlinestyle(PS_SOLID, 2, NULL);
	setlinecolor(RED);
	line(320, 240, x_sec, y_sec);
}

int main()
{
	initgraph(640, 480);		// 初始化 640 x 480 的绘图窗口

	// 绘制一个简单的表盘
	circle(320, 240, 2);
	circle(320, 240, 60);
	circle(320, 240, 160);
	outtextxy(296, 310, _T("BestAns"));

	// 设置 XOR 绘图模式
	setwritemode(R2_XORPEN);	// 设置 XOR 绘图模式

	// 绘制表针
	SYSTEMTIME ti;				// 定义变量保存当前时间
	while(!_kbhit())			// 按任意键退出钟表程序
	{
		GetLocalTime(&ti);		// 获取当前时间
		Draw(ti.wHour, ti.wMinute, ti.wSecond);	// 画表针
		Sleep(1000);							// 延时 1 秒
		Draw(ti.wHour, ti.wMinute, ti.wSecond);	// 擦表针（擦表针和画表针的过程是一样的）
	}

	closegraph();				// 关闭绘图窗口
	return 0;
}
```
五、作业
最后给出的绘制时钟的例子，很不完善，有不少问题。请完善该程序。例如样式上，表盘上没有刻度，没有数字，指针靠中心的一端应该长出来一点点，表盘太简单。还有就是尝试发现并改进功能实现上的问题。

## 10：用鼠标控制绘图/游戏程序
捕获鼠标消息就像捕获按键消息一样简单。
对于按键，通常我们会先检查是否有按键，然后定义一个变量保存按键，再然后根据该按键的值，执行相应的程序。对于鼠标，道理是一样的。

先写个代码对比一下：
获取按键：               获取鼠标：
```c
　　char c;                  　　MOUSEMSG m;
　　if (_kbhit())            　　if (MouseHit())
　　    c = _getch()         　　    m = GetMouseMsg();
```
很简单吧。由于鼠标消息的内容太多，不像按键那么简单，因此需要用一个结构体来保存。通过该结构体，我们可以获取鼠标的如下信息：
```c
struct MOUSEMSG
{
	UINT uMsg;      // 当前鼠标消息
	bool mkCtrl;    // Ctrl 键是否按下
	bool mkShift;   // Shift 键是否按下
	bool mkLButton; // 鼠标左键是否按下
	bool mkMButton; // 鼠标中键是否按下
	bool mkRButton; // 鼠标右键是否按下
	int x;          // 当前鼠标 x 坐标
	int y;          // 当前鼠标 y 坐标
	int wheel;      // 鼠标滚轮滚动值
};
```
其中，“当前鼠标消息”可以是以下值：
```
    WM_MOUSEMOVE     鼠标移动消息
    WM_MOUSEWHEEL    鼠标滚轮拨动消息
    WM_LBUTTONDOWN   左键按下消息
    WM_LBUTTONUP     左键弹起消息
    WM_LBUTTONDBLCLK 左键双击消息
    WM_MBUTTONDOWN   中键按下消息
    WM_MBUTTONUP     中键弹起消息
    WM_MBUTTONDBLCLK 中键双击消息
    WM_RBUTTONDOWN   右键按下消息
    WM_RBUTTONUP     右键弹起消息
    WM_RBUTTONDBLCLK 右键双击消息
```
例如，判断获取的消息是否是鼠标左键按下，可以用：
```
if (m.uMsg == WM_LBUTTONDOWN)
	...
```
下面举一个综合的例子（我偷点懒，直接粘贴的绘图库帮助里面的鼠标范例），该程序会用红色的点标出鼠标移动的轨迹，按左键画一个小方块，按Ctrl+左键画一个大方块，按右键退出：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	// 初始化图形窗口
	initgraph(640, 480);
	
	MOUSEMSG m;						// 定义鼠标消息
	while(true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();

		switch(m.uMsg)
		{
			case WM_MOUSEMOVE:		// 鼠标移动的时候画红色的小点
				putpixel(m.x, m.y, RED);
				break;

			case WM_LBUTTONDOWN:	// 如果点左键的同时按下了 Ctrl 键
				if (m.mkCtrl)		// 画一个大方块
					rectangle(m.x-10, m.y-10, m.x+10, m.y+10);
				else				// 画一个小方块
					rectangle(m.x-5, m.y-5, m.x+5, m.y+5);
				break;

			case WM_RBUTTONUP:
				return;				// 按鼠标右键退出程序
		}
	}

	// 关闭图形窗口
	closegraph();
	return 0;
}
```
[本节作业]

1. 画一个填充的三角形，要用鼠标点选三角形的三个顶点。提示：可以用 fillpoly 函数画多边形。

2. 写一个“格子涂色”的游戏，要求：屏幕上有16x8的格子，屏幕底部有类似画笔中的选色区（随便放上一些常用的颜色），鼠标点击选择区的颜色后，就作为当前颜色，然后再点屏幕上的格子，就可以用刚才的颜色填涂相应格子。
## 11：随机函数
[随机函数简介]
游戏中，许多情况都是随即发生的。还有一些图案程序，例如屏保，也是随即运动的。这就需要用随机函数。

随机函数很简单，只有一个： rand()
该函数返回 0～32767 之间的一个整数。(不需要记住 32767 这个数字，大概知道这个范围就行了)。该函数在头文件 <stdlib.h> 中，使用前记得引用。

[简单测试]
```c
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int r;
	for(int i=0; i<10; i++)
	{
		r = rand();
		printf("%d\n", r);
	}

	return 0;
}
```

执行后，可以看到输出了 10 个随机数字。

[指定范围的随机函数]
实际中，我们经常要产生指定范围的随机函数，通常我们用求余数的办法。例如，产生 0～9 之间的随机数，只需要将任意产生的随机数除以 10 求余数即可。求余数的运算符号是 %，我们可以这样做：
```c
r = rand() % 10;
```
修改前面的测试程序执行后可以看到，产生的数字都是小于 10 的。

如果是 1～6 之间的怎样求呢？
r = rand() % 6 + 1;

无论产生什么样范围的随机函数，都是通过各种运算将随机数的范围 [0, 32767] 修改为自己需要的范围。

[随机种子]

做了多次试验，我们会发现一个问题：虽然产生的数字是随机的，但每次产生的数字序列都一样。为了解决这个问题，我们需要用“随机种子”。
随机函数的产生原理简单来说，就是：前一个随机函数的值，决定下一个随机函数的值。

根据这个原理我们可以知道：只要第一个随机函数的值确定了，那么后面数字序列就是确定的。如果我们想的得到不同的数字序列，我们需要确定第一个随机函数的值，对于设置第一个随机函数的值，叫做设置“随机种子”。易知，随机种子设置一次即可。

设置随机种子的函数如下： srand(种子);

通常，我们用当前时间来做随机种子： srand( (unsigned)time( NULL ) );

因为使用 time 函数，所以记得引用 <time.h>。

[绘图中的应用]：来一个简单的程序，在屏幕上任意位置画任意颜色的点(按任意键退出)：
```c
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int main()
{
	srand( (unsigned)time( NULL ) );

	initgraph(640, 480);

	int x, y, c;
	while(!kbhit())
	{
		x = rand() % 640;
		y = rand() % 480;
		c = RGB(rand() % 256, rand() % 256, rand() % 256);
		putpixel(x, y, c);
	}

	closegraph();
	return 0;
}
```
[作业]

1. 回顾一下第 6 课“实现简单动画”的作业：绘制一个沿 45 度移动的球，碰到窗口边界后反弹。 将这个球改为沿任意角度移动的球，碰到边界后以任意角度反弹。
## 12：数组
课程要求：先复习下课本上对数组的讲解（随便一本教材都行）
一维数组
数组可以实现批量操作。比如，我们产生 10 个随机数，产生后先保存起来，然后输出最大的：
```c
	int n[10];
	int i;
	for (i = 0; i < 10; i++)
		n[i] = rand() % 1000;

	// 按生成的顺序，逆序输出
	for (i = 9; i >= 0; i--)
		printf("%d\n", n[i]);

	// 找出最大的
	int max = -1;
	for (i = 0; i < 10; i++)
	{
		if (n[i] > max)
			max = n[i];
	}

	printf("最大的数字是：%d\n", max);
```

看明白这个程序后，我们继续。
下面，我们绘制一个从屏幕上边任意位置往下落的白色点：
```c
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int main()
{
	srand( (unsigned)time(NULL) );

	initgraph(640, 480);

	int x = rand() % 640;	// 点的 x 坐标
	int y = 0;		// 点的 y 坐标

	while(!_kbhit())
	{
		// 擦掉前一个点
		putpixel(x, y, BLACK);
		// 计算新坐标
		y+=3;
		if (y >= 480) break;
		// 绘制新点
		putpixel(x, y, WHITE);

		Sleep(10);
	}

	closegraph();
	return 0;
}
```

现在利用数组，来产生 100 个随机下落的点。并且每个点落到底部后，就回到顶部重新往下落：
```c
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int main()
{
	srand( (unsigned)time(NULL) );

	initgraph(640, 480);

	// 定义点的坐标数组
	int x[100];			// 点的 x 坐标
	int y[100];			// 点的 y 坐标
	int i;

	// 初始化点的初始坐标
	for (i = 0; i < 100; i++)
	{
		x[i] = rand() % 640;
		y[i] = rand() % 480;
	}

	while(!_kbhit())
	{
		for(i = 0; i < 100; i++)
		{
			// 擦掉前一个点
			putpixel(x[i], y[i], BLACK);
			// 计算新坐标
			y[i] += 3;
			if (y[i] >= 480) y[i] = 0;
			// 绘制新点
			putpixel(x[i], y[i], WHITE);
		}

		Sleep(10);
	}

	closegraph();
	return 0;
}
```
二维数组

理解了一维数组，再看二维数组甚至多维数组，就简单多了，看下面程序理解一下二维数组：

程序要求：屏幕上有 16x8 的方格，按随机顺序在将 1～128 的数字写到每个格子上。
考虑：我们需要记录这些格子，哪些写过数字，哪些没写数字。
我们用一个二维数组来记录：
bool cell[16][8];

写过数字后，我们将相应数组的值设置为 true，看程序：
```c
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

int main()
{
	int x, y;
	wchar_t num[4];

	srand( (unsigned)time(NULL) );

	initgraph(640, 480);

	// 画格子
	for (x = 0; x < 480; x += 30)
		for (y = 0; y < 240; y += 30)
			rectangle(x, y, x + 28, y + 28);

	// 定义二维数组
	bool cell[16][8];
	// 初始化二维数组
	for (x = 0; x < 16; x++)
		for (y = 0; y < 8; y++)
			cell[x][y] = false;

	// 在每个格子上写数字
	for (int i = 1; i <= 128; i++)
	{
		// 找到一个没有写数字的随机格子
		do
		{
			x = rand() % 16;
			y = rand() % 8;
		}while(cell[x][y] == true);

		// 标记该格子已用
		cell[x][y] = true;

		// 在格子上写数字
		swprintf_s(num, 4, L"%d", i);
		outtextxy(x * 30 + 2, y * 30 + 6, num);
	}

	_getch();
	closegraph();
	return 0;
}
```
以上几个范例，无论从实用上还是美观上都很差，我只是希望大家能举一反三，写出更多漂亮的程序。
作业 
1. 回顾一下上一节课的作业，绘制一个任意反弹的球。这次，将程序修改成屏幕上有 10 个任意反弹的球。
2. 如果反弹的不是球，而是点呢？再将某些点之间用线连起来，就可以做一个屏保“变幻线”的程序了。试试做一个。
3. 写“涂格子（也叫点灯）”的游戏。详细规则可以试玩网上的各种版本。

以下作业，有时间就写。因为**讲完这 12 节课，可以写出很多游戏了**，所以可能会感觉作业一下子多了许多。
4. 写个俄罗斯方块。
5. 写贪吃蛇、扫雷。这两个稍微复杂一些，如果遇到问题，贴吧里贴出来，大家一起讨论。

后面还会有更精彩的课程，敬请期待。

## 13：getimage / putimage / loadimag / saveimage / IMAGE 的用法
 getimage / putimage / loadimage / saveimage 这一组命令和 IMAGE 对象可以实现图像处理的相关功能，下面逐个介绍。
（有点类似 tc 中的 imagesize）

[加载图片]：实现加载图片主要分三步：
1. 定义 IMAGE 对象
2. 读取图片至 IMAGE 对象
3. 显示 IMAGE 对象到需要的位置

很简单，我们看一下完整的代码：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	IMAGE img;	// 定义 IMAGE 对象
	loadimage(&img, _T("D:\\test.jpg"));	// 读取图片到 img 对象中
	putimage(0, 0, &img);	// 在坐标 (0, 0) 位置显示 IMAGE 对象

	_getch();
	closegraph();
	return 0;
}
```

注意要显示的图片是 D:\test.jpg，你可以修改为自己的图片路径。

如果只需要加载图片到绘图窗体上， 那么请将 loadimage 的第一个参数设置为 NULL 即可，这样就不需要定义 IMAGE 对象了。

[保存屏幕区域]：和加载图片类似，我们可以从屏幕的某个区域加载图像至 IMAGE 对象，然后再 putimage 到需要的地方。

获取屏幕区域的代码格式：
getimage(IMAGE& img, int x, int y, int w, int h);
参数说明:
    img: 保存该屏幕区域的 IMAGE 对象
    x, y: 区域的左上角坐标
    w, h: 区域的宽和高（注意：不是右下角坐标）

看代码：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	// 定义 IMAGE 对象
	IMAGE img;

	// 绘制内容
	circle(100, 100, 20);
	line(70, 100, 130, 100);
	line(100, 70, 100, 130);

	// 保存区域至 img 对象
	getimage(&img, 70, 70, 60, 60);

	// 将 img 对对象显示在屏幕的某个位置
	putimage(200, 200, &img);

	_getch();
	closegraph();
	return 0;
}
```

[移动复杂的图案]

复杂的图案如果要移动，每次都重新绘制显然效率很低，移动的时候会出现严重的屏幕闪烁。

而 getimage / putimage 的效率十分高，我们可以将复杂的图案用 getimage 保存下来，然后再逐步 putimage 实现复杂图案的移动。
这个代码就不举例了，作为作业大家练习吧。

[更多的功能]：getimage / putimage 有许多重载，这里就不多介绍了，详细看看帮助中的描述吧。
读取图片的技巧：将图片内嵌到 exe 文件中，请参见：http://www.easyx.cn/skills/View.aspx?id=6

[作业]

1. 用线条、圆等各种基础绘图语句画一个“汽车”，然后用 getimage / putimage 实现该“汽车”的平滑移动。
2. 自己学一下帮助中 BeginBatchDraw / FlushBatchDraw / EndBatchDraw 三个函数，可以进一步优化“平滑移动”的效果。这三个命令挺简单的，一看就懂。

## 14：通过位运算实现颜色的分离与处理 
[颜色基础]：在 EasyX 库中，颜色是一个 int 类型的数据，转换为 16 进制后的颜色格式是 0xbbggrr，其中，bb/gg/rr 分别表示两位十六进制的蓝/绿/红颜色值，每种颜色的范围是 0x0～0xff，转换为十进制就是 0～255。

举几个颜色标示的例子：
```
    颜色　　直接表示　　RGB 宏标示
    纯绿色　0x00ff00　　RGB(0, 255, 0)
    青色　　0xffff00　　RGB(0, 255, 255)　　注：青=蓝+绿
    中灰色　0x7f7f7f　　RGB(127, 127, 127)
    黄色　　0x00ffff　　RGB(255, 255, 0)　　注：黄=红+绿
```
例如设置绘图颜色为黄色，可以多种方法，例如：
```
    setcolor(YELLOW);
    setcolor( RGB(255, 255, 0) );
    setcolor(0x00ffff);
```
[获取颜色]
```
getpixel 是用来获取屏幕颜色的函数，其返回值为 int 类型的颜色。例如：
int c = getpixel(100, 100, c);    // 该语句将返回坐标 (100, 100) 位置的颜色。
```
[颜色分离与处理]：有时候我们需要修改颜色某一位的值，这时，可以通过位运算来实现。比如，我们想把某一个点的颜色的红色部分去掉，可以这么做：
```c
int c = getpixel(100, 100);
c &= 0xffff00;
putpixel(100, 100);
```

我们来看一个完整的程序，这个程序，将图片左半部中的红色“去掉”了，就像是显示器“缺色”的效果：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	// 读取图片
	loadimage(NULL, _T("D:\\test.jpg"));
	
	int c;
	for(int x = 0; x < 320; x++)
		for(int y = 0; y < 480; y++)
		{
			c = getpixel(x, y);
			c = (0xff0000 - (c & 0xff0000)) | (0x00ff00 - (c & 0x00ff00)) | (0x0000ff - (c & 0x0000ff));
			putpixel(x, y, c);
		}

	_getch();
	closegraph();
	return 0;
}
```
继续实践，找到这行：
c &= 0xffff00;

我们修改为：
c = (0xff0000 - (c & 0xff0000)) | (0x00ff00 - (c & 0x00ff00)) | (0x0000ff - (c & 0x0000ff));

在执行看看效果，就成了照片的底片效果。
注：通过宏 GetRValue / GetGValue / GetBValue 可以直接获取 COLORREF 中的颜色分量，详见帮助。

[作业]

    实现提高/降低图像亮度的程序。
    自己搜索“灰度算法”，实现彩色图像转换为灰度图像。
## 15：窗体句柄（Windows 编程入门）
EasyX 库有一个获取窗口句柄的功能，很是强大，这里介绍一下。
【窗体句柄】
窗体句柄是 Windows 下窗口的标识，可以理解为窗口的 ID。Windows SDK 中的许多窗口操作函数都需要指明窗体句柄，也就是说，有了句柄，我们可以通过 Windows SDK 中的 API 实现许多高级的窗体控制。
【函数原型】窗体句柄为 HWND 类型，通过 GetHWnd() 函数可以返回绘图窗体的句柄。其函数原型是：
```c
HWND GetHWnd();
```
【使用句柄】举个例子，设置窗体标题文字的 Windows API 为：
```c
BOOL SetWindowText(HWND hWnd, LPCTSTR lpString);
```
参数：
-  hWnd: 要设置标题文字的窗口句柄
-  lpString: 窗体的标题文字，是一个指向字符串的指针。
- 返回值：
    设置成功与否。

以下是设置窗体标题文字的完整范例：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);

	// 获取窗口句柄
	HWND hwnd = GetHWnd();
	// 设置窗口标题文字
	SetWindowText(hwnd, "Hello World!");

	_getch();
	closegraph();
	return 0;
}
```
更多的窗体控制函数，请参考 MSDN。

## 窗口技巧：利用窗体句柄实现圆形窗口

该篇文章讲述如何做一个圆形窗口。根据该思路，可以实现各种形状的窗口。

【基础程序】

先写一个基础程序，实现按鼠标右键退出，完整代码如下：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(640, 480);	// 初始化图形窗口
	MOUSEMSG m;				// 定义鼠标消息

	while(true)
	{
		m = GetMouseMsg();	// 获取一条鼠标消息
		switch(m.uMsg)
		{
			// 按鼠标右键退出程序
			case WM_RBUTTONUP:
				closegraph();
				return 0;
		}
	}
}
```
【实现圆形窗体】

通过 EasyX 库函数 GetHWnd() 获取句柄，然后通过 Windows API 中的 CreateEllipticRgn 创建圆形区域，再通过 SetWindowRgn 将创建的圆形区域应用到窗体上，实现圆型窗体。局部代码如下：
```c
	……

	// 初始化图形窗口
	initgraph(200,200);	

	// 获取窗口句柄
	HWND hWnd = GetHWnd();

	// 设置圆形区域
	HRGN rgn = CreateEllipticRgn(0, 0, 200, 200);
	SetWindowRgn(hWnd, rgn, true);

	……
```
效果：

【修正圆形窗体的位置】

根据上图可以看到，圆形窗体包括了标题栏和边框，这是我们不希望的，我们需要指定多边形的时候加上这个边框区域，但是不同的 windows 皮肤，边框的大小是可变的，需要用相关的 API 函数计算出边框的尺寸，例如：
```c
    GetClientRect();   // 获取客户区的尺寸（主要是判断宽高用）
    GetWindowRect();   // 获取窗口在屏幕上占用的矩形区域
    GetSystemMetrics(SM_CYCAPTION);          // 获取标题栏的高度
```
只需要在源代码的区域中增加这些尺寸即可，设置区域的局部代码修改如下：
```c
	……

	// 获取窗口边框宽高
	RECT rcClient, rcWind;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	int cx = ((rcWind.right - rcWind.left) - rcClient.right) / 2;
	int cy = ((rcWind.bottom - rcWind.top + GetSystemMetrics(SM_CYCAPTION)) - rcClient.bottom) / 2;

	// 设置圆形区域
	HRGN rgn = CreateEllipticRgn(0 + cx, 0 + cy, 200 + cx, 200 + cy);
	SetWindowRgn(hWnd, rgn, true);

	……
```
效果：

【拖动窗体】

现在的问题是：窗体没有标题栏，无法拖动。

实现拖动异形窗体的方法很多，这里采用的是：当鼠标点击时，通过 PostMessage 发消息给 Windows 告诉他点在了标题栏上，这个实现很简单，只需在鼠标左键事件中增加如下语句：
```c
PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(m.x, m.y));
```
【最终程序】

再增加一点绘图效果（我通过 HSL 颜色模型绘制了一个渐变色的圆），最终的圆形窗口程序如下：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	initgraph(200,200);			// 初始化图形窗口
	HWND hWnd = GetHWnd();		// 获取窗口句柄
	
	// 获取窗口边框宽高
	RECT rcClient, rcWind;
	GetClientRect(hWnd, &rcClient);
	GetWindowRect(hWnd, &rcWind);
	int cx = ((rcWind.right - rcWind.left) - rcClient.right) / 2;
	int cy = ((rcWind.bottom - rcWind.top + GetSystemMetrics(SM_CYCAPTION)) - rcClient.bottom) / 2;

	// 设置圆形区域
	HRGN rgn = CreateEllipticRgn(0 + cx, 0 + cy, 200 + cx, 200 + cy);
	SetWindowRgn(hWnd, rgn, true);
	
	// 画彩虹球
	setlinestyle(PS_SOLID, 2);
	for(int r = 100; r > 0; r--)
	{
		setlinecolor( HSLtoRGB(360 - r * 3.6f, 1, 0.5) );
		circle(100, 100, r);
	}
	
	MOUSEMSG m;						// 定义鼠标消息
	
	while(true)
	{
		m = GetMouseMsg();			// 获取一条鼠标消息
		
		switch(m.uMsg)
		{
			case WM_LBUTTONDOWN:
				// 如果左键按下，欺骗 windows 点在了标题栏上
				PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(m.x, m.y));
				break;
			
			case WM_RBUTTONUP:			// 按鼠标右键退出程序
				closegraph();
				return 0;
		}
	}
}
```
效果：

【拓展】

Windows SDK 有许多关于窗体控制的 API，比如，还可以将窗体设置变为半透明的：
```c
SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
SetLayeredWindowAttributes(hWnd, 0, 192, LWA_ALPHA);    
// 设置窗体透明度为 192（0 为全透明，255 为不透明）
```

SetLayeredWindowAttributes 函数还可以设置某个颜色为透明区域，有兴趣的可以参考一下 MSDN，里面有函数的详细用法，以及更多有趣的窗体控制函数。


## 什么是 LPCTSTR？和 char* 有什么关系？
简单来说，LPCTSTR 就表示字符串指针。 那么和 char* 又有什么区别呢？

大家用 char 表示字符时，英文占用一个字节，中文站用两个字节。这样有一个严重的问题：两个字节，究竟是两个英文字符，还是一个中文字符？为了解决这个问题，unicode 编码诞生了。

常见的 unicode 编码不管中文英文都用两个字节表示，用 wchar_t 来定义。 为了让代码适应 char 和 wchar_t 两种情况，可以用 TCHAR 宏，这个宏当项目定义为 unicode 时表示 wchar_t，否则表示 char。 对应的，还有 _T("") 宏用来表示字符串，例如 _T("abc") 在项目为 unicode 时表示 L"abc"，否则表示 "abc"。

然后再看 LPCTSTR，它是以下几个概念的缩写：Long Point Const Tchar STRing，这样意思就明确了吧。

相关的问题
## 解决错误：error C2665: “outtextxy”: 2 个重载中没有一个可以转换所有参数类型 
【错误描述】

一些程序在 VC6 下运行好好地，但是放到 VC2008 及更高版本 VC 下编译却报错误（以下仅以 VC2008 举例，高版本 VC 类似），例如使用如下语句：
```c
outtextxy(10, 20, "Hello World");
```
在 VC6 下可以成功编译，但在 VC2008 下编译后会有错误。

中文版错误提示如下：

error C2665: “outtextxy”: 2 个重载中没有一个可以转换所有参数类型

英文版错误提示如下：

error C2665: 'outtextxy' : none of the 2 overloads could convert all the argument types

同样的，对于其他一些包含字符串调用的函数，例如 loadimage、drawtext 等，也会遇到类似问题。

【错误原因】

简单来说，这是由于字符编码问题引起的。VC6 默认使用的 MBCS 编码，而 VC2008 及高版本 VC 默认使用的 Unicode 编码。以下详细解释这个问题：

用 char 表示字符时，英文占用一个字节，中文站用两个字节。这样有一个严重的问题：两个连续字节，究竟是两个英文字符，还是一个中文字符？为了解决这个问题，Unicode 编码诞生了。Unicode 编码不管中文英文都用两个字节表示。

对于 MBCS 编码，字符变量用 char 定义。对于 Unicode 编码中，字符变量用 wchar_t 定义。为了提高代码的自适应性，微软在 tchar.h 里面定义了 TCHAR，而 TCHAR 会根据项目定义的编码，自动展开为 char 或 wchar_t。

在 **Windows API 和 EasyX 里面的大多数字符串指针都用的 LPCTSTR 或 LPTSTR 类型**，LPCTSTR / LPTSTR 就是“Long Point (Const) Tchar STRing”的缩写。所以可以认为，LPCTSTR 就是`const TCHAR *`，LPTSTR 就是`TCHAR *`。

于是，在 VS2008 里面，给函数传递 char 字符串时，就会提示前述错误。

【解决方案】解决方法有多个，目的一样，都是**让字符编码相匹配**。

方法一：**将所有字符串都修改为 TCHAR 版本**。

简单来说需要注意以下几点：

1. 在程序中使用 #include <tchar.h> 添加对 TCHAR 的支持。

2. 对于字符串，例如 "abc" 用 _T("abc") 表示。就是加上 _T("")。

3. 定义字符变量时，将 char 换成 TCHAR。

4. 操作字符串的函数也要换成相应的 TCHAR 版本，例如 strcpy 要换成 _tcscpy。（详见 MSDN）

方法二：在代码中取消 Unicode 编码的宏定义，让后续编译都以 MBCS 编码进行。 

方法很简单，只需要在代码顶部增加以下代码：
```c
#undef UNICODE
#undef _UNICODE
```
这样就可以取消 Unicode 编码的宏定义，让整个项目以 MBCS 编码编译。

方法三：在 VC2008 里面，将项目属性中的字符编码修改为 MBCS。

以下分别列举中英文两种版本的 VC2008 的操作步骤：

在中文版 VC2008 中的操作方法如下：点菜单“项目-> xxx 属性...”（或右击项目名称，选择“属性”，或按 Alt + F7 也可以打开项目属性），点左侧的“配置属性”，在右侧的设置中找到“字符集”，修改默认的“使用 Unicode 字符集”为“使用多字节字符集”。

在英文版 VC2008 中的操作方法如下：点菜单“Project -> xxx Properties...”（或右击项目名称，选择 Properties，或按 Alt + F7 也可以打开项目属性），点左侧的“Configuration Properties”，在右侧的设置中找到“Character Set”，修改默认的“Use Unicode Character Set”为“Use Multi-Byte Character Set”。

设置完毕后，再次编译就可以看到问题已经解决。

## 16：设备上下文句柄（Windows 编程入门2）
注：学习本节前，请自备 MSDN，以便查阅 Windows GDI 函数。
EasyX的绘图函数最初是模仿的BGI的函数命名。为了让大家借此学习 Windows GDI 绘图，EasyX 增加了获取 HDC 句柄的功能。

对于 Windows GDI 中的绘图函数，很多都需要一个 HDC 句柄。我们用 GetImageHDC() 函数获取该句柄，然后就可以使用 Windows GDI 了。先看看例子吧：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	// 初始化绘图窗口，并获取 HDC 句柄
	initgraph(640, 480);
	HDC hdc = GetImageHDC();

	// 以下是标准 Windows GDI 操作画一条线（相关语句，请查阅 MSDN）
	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 200, 200);
	// 标准 Windows GDI 操作结束

	// 按任意键返回
	_getch();
	closegraph();
	return 0;
}
```

还可以针对 IMAGE 对象使用 GDI 绘图函数，看下面这个例子：
```c
#include <graphics.h>
#include <conio.h>

int main()
{
	// 初始化绘图窗口
	initgraph(640, 480);

	// 创建 300x300 的 IMAGE 对象，并获取其 HDC 句柄
	IMAGE img(300, 300);
	HDC hdc = GetImageHDC(&img);

	// 以下是标准 Windows GDI 操作画一条线（相关语句，请查阅 MSDN）
	MoveToEx(hdc, 100, 100, NULL);
	LineTo(hdc, 200, 200);
	// 标准 Windows GDI 操作结束
	
	// 将 img 贴到绘图窗口上：
	putimage(0, 0, &img);

	// 按任意键返回
	_getch();
	closegraph();
	return 0;
}
```
注意：

    这次的内容虽然少，但是 Windows GDI 的内容相当多，所以，完成本节的学习还是很不容易的。
    Windows GDI 并没有设置颜色这样的函数，需要创建画笔(画刷)并选入画笔(画刷)，并且在不用的时候记得删除。Windows GDI 相当的丰富，这里就不多做介绍了，感兴趣的请参考相关书籍。
    至于作业，其实从前几讲开始就没必要弄什么作业了，能坚持看下来的，相信都会自觉的写一些东西。

## 17: 在游戏中播放音乐
游戏没有声音多单调。这里做一个简单的范例，用 mciSendString 函数播放 MP3 格式的音乐，先看看代码吧：
```c
// 编译该范例前，请把 music.mp3 放在项目文件夹中
// 发布时，请把 music.mp3 和编译的 exe 放在一起
// 编译环境：VC6~VC2017 + EasyX_20190314(beta)
//
#include <graphics.h>
#include <conio.h>
// 引用 Windows Multimedia API
#pragma comment(lib,"Winmm.lib")

void main()
{
	initgraph(640, 480);

	// 打开音乐
	mciSendString(_T("open music.mp3 alias mymusic"), NULL, 0, NULL);

	outtextxy(0, 0, _T("按任意键开始播放"));
	getch();

	// 播放音乐
	mciSendString(_T("play mymusic"), NULL, 0, NULL);

	outtextxy(0, 0, _T("按任意键停止播放"));
	getch();

	// 停止播放并关闭音乐
	mciSendString(_T("stop mymusic"), NULL, 0, NULL);
	mciSendString(_T("close mymusic"), NULL, 0, NULL);

	outtextxy(0, 0, _T("按任意键退出程序"));
	getch();
	closegraph();
}
```
简单讲解一下： 一定要引用 Winmm.lib 库文件。这个范例中是通过 #pragma comment 命令引用的，也可以在项目属性中设置，这里不作介绍。

mciSendString 函数的功能很强大，甚至可以播放视频，不过这里就不多作介绍了，详细请参见 MSDN。我们平时只需要用到第一个参数，将另外三个参数置为 NULL, 0, NULL 即可。

第一个参数是多媒体命令字符串，不区分大小写。程序中，先要通过 open 命令打开 background.mp3，并用 alias 指定了别名为“mymusic”，这样在之后的代码中就可以方便的通过“mymusic”这个别名访问该音乐了。当然，并不是必须要指定别名，每次通过文件名访问也是可以的。

open 后面的 mp3 用绝对路径或相对路径都可以。

然后就是：play mymusic 开始播放，stop mymusic 停止播放，close mymusic 关闭文件。

如果需要同时播放多个音乐，请为不同的音乐指定不同的别名，然后分别操作即可。
不再需要播放的音乐，请记得用 close 命令关闭。

另外还有一个 PlaySound 函数也可以用来播放声音，不过可惜不支持 mp3 / wma，这里就不多做介绍了。

一些有用的多媒体命令：
- 从头播放 xxx：
"play xxx from 0"
- 循环播放 .mp3 文件(.wma 同样可以)：
"open xxx.mp3" 然后 "play xxx.mp3 repeat"
- 循环播放 .wav 文件：
"open xxx.wav type MPEGVideo" 然后 "play xxx.wav repeat"
- 关闭当前程序打开的所有多媒体文件：
"close all" 
- 如果文件名中有空格，需要用双引号引起来文件名（注意转义）：
mciSendString("open \"D:\\My Music\\俞丽拿 梁祝 化蝶.mp3\" alias mymusic", NULL, 0, NULL);

如果希望将 mp3 嵌入到 exe 中，请参考：http://www.easyx.cn/skills/View.aspx?id=87

这些暂时应该够用了吧，更多的多媒体命令请参见 MSDN。

- 解决某些 MP3 无法播放的问题
有些 mp3 无法用 mciSendString 播放，是因为用了比较大的封面，我测试过一个 mp3 的封面尺寸是 1824x1824，直接无法播放。将封面尺寸修改为 500x500，顺利播放。当然，更简单的办法是直接移除封面。这里给大家推荐个工具：Mp3tag，官网：https://www.mp3tag.de，Mp3tag 的操作方法非常简单，点点鼠标就能搞定，这里不再详述。

## 18: 实现简单的询问对话框

游戏结束的时候，通常会简单的弹出一个对话框询问用户是否要重新来一句，如下图：
这篇文章就简单讲解一下这个功能如何实现。首先，我们需要使用一个 Windows API 函数：MessageBox。该函数原型如下：
```c
int MessageBox(
	HWND hWnd,          // handle to owner window
	LPCTSTR lpText,     // text in message box
	LPCTSTR lpCaption,  // message box title
	UINT uType          // message box style
);
```

第一个参数 hWnd 是指向父窗口的句柄，可以通过 EasyX 的函数 GetHWnd() 得到绘图窗口的句柄；
第二个参数 lpText 是要显示的字符串；
第三个参数 lpCaption 是提醒对话框的标题显示字符串；
第四个参数 uType 是对话框的按钮类型及属性，相当丰富，详细请看 MSDN，稍后的范例仅仅列出来一些常用的值。

返回值表示用户点选的按钮。

不多说了，直接写两个例子吧，相信大家一看就懂：

例 1，实现等待用户按“确定”的功能：
```c
HWND wnd = GetHWnd();
MessageBox(wnd, "您的系统版本太低，请升级系统。", "警告", MB_OK | MB_ICONWARNING);
```
例 2，实现文章开始的图片那样的对话框，当用户选择“是”，执行 NewGame()，否则执行 Quit()：
```c
HWND wnd = GetHWnd();
if (MessageBox(wnd, "游戏结束。\n重来一局吗？", "询问", MB_YESNO | MB_ICONQUESTION) == IDYES)
	NewGame();
else
	Quit();
```

更多的使用方法，请参考 MSDN。
## 19： 同时检测多个按键和平滑按键处理
getch() 函数，用于返回用户输入的字符。当连续按键时，该函数返回第一个字符和第二个字符之间，默认有 0.5 秒的延时，并且之后的连续字符，默认是每秒钟 15 次输入。这两个数值可以在控制面板中设置。

如果需要平滑的按键输入，或者同时按下多个按键，就不能用 getch() 了，需要使用另一个 Windows API 函数：GetAsyncKeyState()。该函数原型如下：
```c
SHORT GetAsyncKeyState(
	int vKey		// virtual-key code
);
```

vKey 是要检测的按键的虚拟键码，常用的如 VK_UP、VK_DOWN 等，分别表示方向键的上、下等。需要注意：对于 26 个字母的键码，可以直接写 'A'、'B'……，而不要写 VK_A、VK_B。数字键也是，请直接写 '0'、'1'……。全部的 256 种虚拟键码，请参考 MSDN 中的 Virtual-Key Codes。

返回的 SHORT 值，如果最高位为 1，表示该键被按下；否则表示该键弹起。该函数的最低位还可以用来检测开关键（比如大小写锁定键）的状态。作为按键处理，还可以使用 GetKeyState、GetKeyboardState 等函数，详细请参考 MSDN 手册中的 Keyboard Input Functions 部分。

下面给一个简单的例子，该范例是用键盘的上下左右键移动一个圆，并且可以通过左 Shift 放大、左 Ctrl 缩小，几个按键可以同时灵活地控制圆。代码如下：
```c
// 程序名称：同时检测多个按键及平滑按键输入的范例
// 编译环境：Visual C++ 6.0 / 2010，EasyX 惊蛰版
//
#include <graphics.h>

/////////////////////////////////////////////
// 定义常量、枚举量、结构体、全局变量
/////////////////////////////////////////////

#define	CMD_UP			1
#define	CMD_DOWN		2
#define	CMD_LEFT		4
#define	CMD_RIGHT		8
#define	CMD_ZOOMIN		16
#define	CMD_ZOOMOUT		32
#define	CMD_QUIT		64

// 声明圆的坐标和半径
int g_x, g_y, g_r;



/////////////////////////////////////////////
// 函数声明
/////////////////////////////////////////////

void Init();						// 初始化
void Quit();						// 退出
int  GetCommand();					// 获取控制命令
void DispatchCommand(int _cmd);		// 分发控制命令
void OnUp();						// 上移
void OnDown();						// 下移
void OnLeft();						// 左移
void OnRight();						// 右移
void OnZoomIn();					// 放大
void OnZoomOut();					// 缩小



/////////////////////////////////////////////
// 函数定义
/////////////////////////////////////////////

// 主函数
void main()
{
	Init();

	int c;
	do
	{
		c = GetCommand();
		DispatchCommand(c);
		Sleep(10);
	}while(!(c & CMD_QUIT));

	Quit();
}

// 初始化
void Init()
{
	// 设置绘图屏幕和绘图模式
	initgraph(640, 480);
	setwritemode(R2_XORPEN);

	// 设置圆的初始位置和大小
	g_x = 320;
	g_y = 240;
	g_r = 20;

	// 显示操作说明
	setfont(14, 0, _T("宋体"));
	outtextxy(20, 270, _T("操作说明"));
	outtextxy(20, 290, _T("上：上移"));
	outtextxy(20, 310, _T("下：下移"));
	outtextxy(20, 330, _T("左：左移"));
	outtextxy(20, 350, _T("右：右移"));
	outtextxy(20, 370, _T("左 Shift：放大"));
	outtextxy(20, 390, _T("左 Ctrl：缩小"));
	outtextxy(20, 410, _T("ESC：退出"));
	outtextxy(20, 450, _T("注：可以同时按多个键，但能同时按下的键的数量，受键盘硬件限制"));

	// 画圆
	circle(g_x, g_y, g_r);
}

// 退出
void Quit()
{
	closegraph();
}

// 获取控制命令
int GetCommand()
{
	int c = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)		c |= CMD_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)	c |= CMD_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)		c |= CMD_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)		c |= CMD_DOWN;
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)	c |= CMD_ZOOMIN;
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)	c |= CMD_ZOOMOUT;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)	c |= CMD_QUIT;

	return c;
}

// 分发控制命令
void DispatchCommand(int _cmd)
{
	if (_cmd & CMD_UP)			OnUp();
	if (_cmd & CMD_DOWN)		OnDown();
	if (_cmd & CMD_LEFT)		OnLeft();
	if (_cmd & CMD_RIGHT)		OnRight();
	if (_cmd & CMD_ZOOMIN)		OnZoomIn();
	if (_cmd & CMD_ZOOMOUT)		OnZoomOut();
}

// 上移
void OnUp()
{
	circle(g_x, g_y, g_r);
	if (g_y <= 0) g_y = 480; else g_y-=2;
	circle(g_x, g_y, g_r);
}

// 下移
void OnDown()
{
	circle(g_x, g_y, g_r);
	if (g_y >= 480) g_y = 0; else g_y+=2;
	circle(g_x, g_y, g_r);
}

// 左移
void OnLeft()
{
	circle(g_x, g_y, g_r);
	if (g_x <= 0) g_x = 640; else g_x-=2;
	circle(g_x, g_y, g_r);
}

// 右移
void OnRight()
{
	circle(g_x, g_y, g_r);
	if (g_x >= 640) g_x = 0; else g_x+=2;
	circle(g_x, g_y, g_r);
}

// 放大
void OnZoomIn()
{
	circle(g_x, g_y, g_r);
	if (g_r < 100) g_r++;
	circle(g_x, g_y, g_r);
}

// 缩小
void OnZoomOut()
{
	circle(g_x, g_y, g_r);
	if (g_r > 10) g_r--;
	circle(g_x, g_y, g_r);
}
```

再额外说一个小问题：由于 GetAsyncKeyState() 函数获取的按键状态是直接取自硬件，并非取自消息队列。所以，即便程序处非活动状态，GetAsyncKeyState() 仍然可以正确获取按键状态。所以会有这样一个问题：比如你写了一个打字练习的小游戏，在游戏中途切换到另一个应用去发邮件，你会看到发邮件录入文字时，你的打字练习小游戏仍然会接受键盘输入。很明显，这时候需要判当前应用是否处于活动状态。解决方法有多种，例如，通过 Windows API 函数 GetForegroundWindow() 获取到当前前景窗口的句柄，再和 EasyX 窗口的句柄对比，如果相同，就表示 EasyX 的窗口处于活动状态，从而解决非活动状态的按键处理问题。

##  学习编写一个完整的 Windows 应用程序 

从 C 语言，到常见的 setup.exe 安装程序，究竟有多远？怎样实现双击 .xls 文件会自动启动 excel 并加载？这篇文章就解释这个问题。

请跟随以下步骤：

1. 编写应用程序。为了叙述方便，暂且叫 test.exe 吧。通常 test.exe 中会用若干 scanf 或 cin 来读取用户输入的参数，并根据参数来执行程序，这是大家在学习 C 语言的过程中见到的。

2. 要规定程序所需数据的格式。例如，word 需要使用 .doc 格式的数据，photoshop 需要使用 .psd 格式的数据。不管这些数据文件是文本的还是二进制的，只有格式确定下来，程序才能按照原定意图解释数据。数据格式就是将用户输入的全部数据都放进一个文件中，然后将 scanf 语句修改为读取该文件（读文件的程序部分请参考相关书籍）。

3. 设置文件关联。在上一步我们规定了一个我们需要的数据格式，现在给这个格式的文件自定一个扩展名，比如叫 .abc，然后双击该文件，Windows 会提示用户选择打开该类型的程序，我们选择步骤 1 编写的应用程序。不过这时候，程序并没有什么响应。

4. 修改应用程序以处理用户双击 .abc 类型的文件。例如，当用户双击文件 my.abc 时，Windows 会调用 test.exe my.abc，也就是说，会把用户要打开的文件当做参数传递给 test.exe。所以，需要通过 main 函数的 argc / argv 参数，来获取用户双击的文件名。然后将该文件名放在第二步中读取。

5. 制作安装程序。为什么要制作安装程序呢？通过以上四个步骤可以看到，使用户双击 .abc 类型的文件能生效，必须要设置文件关联，将 .abc 类型关联到 test.exe 上。但是总不能让用户来手工做这个操作吧？Windows 所有文件关联的设置都保存在注册表里面，我们需要通过安装程序来修改注册表，自动注册文件关联。安装程序的执行解压文件、拷贝文件、注册文件关联等都是常规任务，常见的制作安装程序的软件都可以完成，不需要我们自己写程序的。用安装程序软件可以很简单的生成 setup.exe 安装程序，并且在安装后自动出现在“添加/删除程序”中，再或者添加桌面图标等等。

至此，经过以上几个步骤就可以做出来一个完整的 Windows 应用程序。

顺便，这里给出一个包含以上全部步骤的 Windows 应用程序：EasyIFS 1.0，一个开源的分形学 IFS 演示程序，可以依据 IFS 数据文件中定义的参数生成图像。下载地址：http://www.easyx.cn/downloads/View.aspx?id=3。该程序包含全部的 VC6 源代码。



