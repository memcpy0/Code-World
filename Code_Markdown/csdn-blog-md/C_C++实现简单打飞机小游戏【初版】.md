@[toc]
# C版本
## 点子
实现一个字符飞机，按asdw键后改变飞机的坐标位置(x, y)，控制飞机上下左右移动，空格键开火。注意：这里的坐标指得是飞机头的坐标。

初版是设计一个靶子，飞机按空格键后发射激光击毁靶子。然后让靶子自由移动，添加分数统计和显示等。

进阶版是将游戏模块化，设计自由下落的敌机，被击中后绘制简易爆炸效果等等。

更高级是添加关卡和多种武器等等。
### 初版1.1 静止的飞机
同小球弹跳一样，在控制台输出一些空格和换行控制飞机位置。飞机形状如下：
```
     *
    ***
   *****
  *  *  *
     *
```
```c
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
void PrintPlane(int x, int y) { //输出飞机图形
	for (int i = 0; i < y; i++)  puts(""); 
	for (int i = 0; i < x; i++)  putchar(' ');   
	puts("*");
	for (int i = 0; i < x - 1; i++) putchar(' '); 
	puts("***");
	for (int i = 0; i < x - 2; i++) putchar(' '); 
	puts("*****");
	for (int i = 0; i < x - 3; i++) putchar(' '); 
	puts("*  *  *");
	for (int i = 0; i < x; i++) putchar(' '); 
	puts("*");
}
int main() {
	int x, y;
	x = 10, y = 5;  
    PrintPlane(x, y);  
	return 0;
} 
```
效果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191003014520873.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 初版1.2 动态飞机(使用按键控制飞机移动)
可以用scanf输入字符，但是会影响游戏体验，毕竟scanf需要敲回车。替代的是使用conio.h中的getch函数。
> conio.h不是C标准库中的头文件，在C standard library，ISO C 和POSIX标准中均没有定义。
> conio是Console Input/Output（控制台输入输出）的简写，其中定义了**通过控制台进行数据输入和数据输出**的函数，主要是一些用户通过**按键盘产生的对应操作**，比如getch()函数等等。
> conio库不仅适用于 Windows 平台，在 Linux 下也可使用.网上已经有兼容包,下载后打开就可使用。

发现一个问题，如果移动到上左的边界时，飞机的输出会变形...为此添加边界检查。
```c
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
void PrintPlane(int x, int y) { //输出飞机图形
	for (int i = 0; i < y; i++)  puts(""); 
	for (int i = 0; i < x; i++)  putchar(' ');   
	puts("*");
	for (int i = 0; i < x - 1; i++) putchar(' '); 
	puts("***");
	for (int i = 0; i < x - 2; i++) putchar(' '); 
	puts("*****");
	for (int i = 0; i < x - 3; i++) putchar(' '); 
	puts("*  *  *");
	for (int i = 0; i < x; i++) putchar(' '); 
	puts("*");
}
int main() {
	int x, y;
	x = 10, y = 5; 
	char c;
	while (1) {
        PrintPlane(x, y); 
        c = getch();
		switch (c) {
			case 'a': x -= (x > 3) ? 1 : 0; break;
			case 's': y++; break;
			case 'd': x++; break;
			case 'w': y -= (y > 0) ? 1 : 0; break;
			case ' ': break;
		} 
		system("cls");
	}
	return 0;
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191003015004694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 初版1.3 动态飞机(按空格发射和关闭激光)
和按其他方向键一样，按空格键后**同样要重新绘制图形**。让飞机发射激光，即在飞机上方显示一列竖线"|"。

发射模式：按了空格键后就会不断的发射激光，如果再按一次就会关闭激光。移动时会自动发射激光。
```c
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
void PrintScreen(int x, int y, int isFired) {
	for (int i = 0; i < y; i++) { //不发射激光时会输出多余的空格
		for (int j = 0; j < x; j++) putchar(' ');
		if (isFired) putchar('|');
		puts("");
	}
	for (int i = 0; i < x; i++)  putchar(' ');   
	puts("*");
	for (int i = 0; i < x - 1; i++) putchar(' '); 
	puts("***");
	for (int i = 0; i < x - 2; i++) putchar(' '); 
	puts("*****");
	for (int i = 0; i < x - 3; i++) putchar(' '); 
	puts("*  *  *");
	for (int i = 0; i < x; i++) putchar(' '); 
	puts("*");
}
int main() {
	int x, y;
	x = 10, y = 5; 
	char c;
	int isFired = 0;
	while (1) {
        PrintScreen(x, y, isFired); 
        c = getch();
		switch (c) {
			case 'a': x -= (x > 3) ? 1 : 0; break;
			case 's': y++; break;
			case 'd': x++; break;
			case 'w': y -= (y > 0) ? 1 : 0; break;
			case ' ': isFired = isFired == 0 ? 1: 0; break;
		} 
		system("cls");
	}
	return 0;
} 
```
![!\[在这里插入图片描述\](https://img-blog.csdnimg.cn/201](https://img-blog.csdnimg.cn/20191003020103551.png) 
当然，也可以改成按空格键后**发射一次激光，然后移动时关闭激光**。一般来看，应该以这种模式为主，不然玩家只需要开启一次激光就可以了。哪有这么好的事呢？?

另外也不一定要是激光，也可以添加其他的武器，如用‘^'表示子弹。
### 初版1.4 动态飞机(按空格发射一次激光，移动时关闭)
这里空格不具有关闭激光的功能，仅仅是开启激光。因此，改为使用全局变量isFired。
```c
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
int isFired = 0;
void PrintScreen(int x, int y) {
	if (!isFired) for (int i = 0; i < y; i++) puts(""); //不发射激光时
	else {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) putchar(' ');
			if (isFired) printf("|\n");
		}
		isFired = 0; //关闭激光
	} 
	/* 打印飞机 */
	for (int i = 0; i < x; i++)  putchar(' ');   
	puts("*");
	for (int i = 0; i < x - 1; i++) putchar(' '); 
	puts("***");
	for (int i = 0; i < x - 2; i++) putchar(' '); 
	puts("*****");
	for (int i = 0; i < x - 3; i++) putchar(' '); 
	puts("*  *  *");
	for (int i = 0; i < x; i++) putchar(' '); 
	puts("*"); 
}
int main() {
	int x, y;
	x = 10, y = 5; 
	char c; 
	while (1) {
        PrintScreen(x, y); 
        c = getch();
		switch (c) {
			case 'a': x -= (x > 3) ? 1 : 0; break;
			case 's': y++; break;
			case 'd': x++; break;
			case 'w': y -= (y > 0) ? 1 : 0; break;
			case ' ': isFired = 1; break;
		} 
		system("cls");
	}
	return 0;
} 
```
### 初版1.5 飞机打靶(添加和击毁靶子)
先用‘+‘作为靶子。当飞机头的x坐标和靶子对齐时，全局变量is_killed置1，刷新屏幕取消靶子输出。
```c
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
int isFired = 0, is_killed = 0;
int target_x = 12; //靶子的x坐标, y = 0 
void PrintScreen(int x, int y) {
	if (!is_killed) { //打印靶子 
	   for (int i = 0; i < target_x; i++) printf(" ");
	   printf("+\n"); 
    }
	if (!isFired) for (int i = 0; i < y; i++) puts(""); //不发射激光时
	else { //发射激光 
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) putchar(' ');
			if (isFired) printf("|\n");
		}
		isFired = 0;
		if (x == target_x) is_killed = 1; //击中靶子 
	} 
	/* 打印飞机 */
	for (int i = 0; i < x; i++)  putchar(' ');   
	puts("*");
	for (int i = 0; i < x - 1; i++) putchar(' '); 
	puts("***");
	for (int i = 0; i < x - 2; i++) putchar(' '); 
	puts("*****");
	for (int i = 0; i < x - 3; i++) putchar(' '); 
	puts("*  *  *");
	for (int i = 0; i < x; i++) putchar(' '); 
	puts("*"); 
}
int main() {
	int x, y;
	x = 20, y = 15; 
	char c; 
	while (1) {
        PrintScreen(x, y); 
        c = getch();
		switch (c) {
			case 'a': x -= (x > 3) ? 1 : 0; break;
			case 's': y++; break;
			case 'd': x++; break;
			case 'w': y -= (y > 0) ? 1 : 0; break;
			case ' ': isFired = 1; break;
		} 
		system("cls");
	}
	return 0;
} 
```
这时的程序仍然很简陋，可以思考一下，如何让靶子自己移动？如何统计和显示得分？可以自己作为练习。我会将其作为进阶版的飞机游戏实现。


