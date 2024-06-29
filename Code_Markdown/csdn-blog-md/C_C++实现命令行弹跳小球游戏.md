@[toc]
# C版本
### 1.1 显示静止的小球
使用printf函数在屏幕坐标(x, y)处显示一个静止的小球字符'O'，注意屏幕坐标系的原点在左上角，向右为X轴，向下为Y轴。

做法是先输出许多换行符，再输出一些空格，然后是小球字符。
```c
#include <stdio.h>
int main() {
	int x, y;
	x = 5, y = 10;
	for (int i = 0; i < y; i++) {
		printf("\n");
	}
	for (int j = 0; j < x; j++) {
		printf(" ");
	}
	printf("o\n");
	return 0;
} 
```
效果如图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191002011437709.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70) 
 ### 1.2 动态的小球(下落)
 如果要实现小球的变动，就要**每次输出一个小球后就清一下屏幕，然后改变小球的坐标**。先从改变纵坐标y开始，定义一个速度变量velocity。
```c
#include <stdio.h>
#include <stdlib.h>
int main() {
	int x, y, velocity = 1;
	x = 5, y = 0;
	while (1) {
		for (int i = 0; i < y; i++) {
			printf("\n");
		}
		for (int j = 0; j < x; j++) {
			printf(" ");
		}
		printf("O\n");
		system("cls");
		y += velocity; 
	}	
	return 0;
}  
 ```
难以截图，不过发现了问题。小球会一直往下落。可以**添加范围检查，当y大于或小于某范围值的时候，改变其方向**。
```c
#include <stdio.h>
#include <stdlib.h>
int main() {
	int x, y, velocity = 1;
	x = 10, y = 0;
	while (1) {
		for (int i = 0; i < y; i++) {
			printf("\n");
		}
		for (int j = 0; j < x; j++) {
			printf(" ");
		}
		printf("O\n");
		
		y += velocity; 
		if (y >= 15 || y <= 0) velocity = -velocity;
		
		system("cls");
	}	
	return 0;
} 
```
### 1.3 动态的小球(斜着弹跳)
让小球还能够斜着弹跳，主要思路是同时改变x、y两个坐标的值。定义两个方向的速度velocity_x、velocity_y，初值为1，当碰到边界时改变两个方向的速度的正负号。
```c
#include <stdio.h>
#include <stdlib.h>
int main() {
	int x, y, velocity_x, velocity_y;
	x = 5, y = 10;
	velocity_x = velocity_y = 1;
	
	int left, down, top, right;
	left = top = 0, down = 20, right = 40;
	
	while (1) {
		for (int i = 0; i < y; i++) {
			printf("\n");
		}
		for (int j = 0; j < x; j++) {
			printf(" ");
		}
		printf("O\n");
		
		y += velocity_y; 
		x += velocity_x;
		if (y >= down || y <= top) velocity_y = -velocity_y;
		if (x >= right || x <= left) velocity_x = -velocity_x;	
		
		system("cls");
	}	
	return 0;
} 
```
### 1.4 动态的小球(Sleep慢速)
发现上面的小球弹跳太快了，减慢一下速度。使用windows.h中的Sleep。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191002024237645.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)当需要用到Windows API进行开发的时候，比如说进行Windows窗口程序开发时，就会用到这个。如果是普通开发，或者有跨平台要求的时候，不要用这个头文件，因为它只面向Windows操作系统。
```c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main() {
	int x, y, velocity_x, velocity_y;
	x = 5, y = 10;
	velocity_x = velocity_y = 1;
	
	int left, down, top, right;
	left = top = 0, down = 20, right = 40;
	
	while (1) {
		for (int i = 0; i < y; i++) {
			printf("\n");
		}
		for (int j = 0; j < x; j++) {
			printf(" ");
		}
		printf("O\n");
		
		y += velocity_y; 
		x += velocity_x;
		if (y >= down || y <= top) velocity_y = -velocity_y;
		if (x >= right || x <= left) velocity_x = -velocity_x;	
		
		Sleep(50); //50ms
		system("cls");
	}	
	return 0;
} 
```
### 1.5 动态的小球(使用循环减速)
用空循环就可以了，尝试了一下，发现5千万太慢，改成15000000，感觉还行。
```c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main() {
	int x, y, velocity_x, velocity_y;
	x = 5, y = 10;
	velocity_x = velocity_y = 1;
	
	int left, down, top, right;
	left = top = 0, down = 20, right = 40;
	
	while (1) {
		for (int i = 0; i < y; i++) {
			printf("\n");
		}
		for (int j = 0; j < x; j++) {
			printf(" ");
		}
		printf("o\n");
		
		y += velocity_y; 
		x += velocity_x;
		if (y >= down || y <= top)  velocity_y = -velocity_y; 
		if (x >= right || x <= left) velocity_x = -velocity_x; 
		int k = 0;
		while (k++ < 18000000) ;
		system("cls");
	}	
	return 0;
} 
```
### 1.6 动态的小球(边界响铃)
可以使用转义字符“\a"来响铃。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191002024948180.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
```c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
int main() {
	int x, y, velocity_x, velocity_y;
	x = 5, y = 10;
	velocity_x = velocity_y = 1;
	
	int left, down, top, right;
	left = top = 0, down = 20, right = 40;
	
	while (1) {
		for (int i = 0; i < y; i++) {
			printf("\n");
		}
		for (int j = 0; j < x; j++) {
			printf(" ");
		}
		printf("O\n");
		
		y += velocity_y; 
		x += velocity_x;
		if (y >= down || y <= top) {
		   velocity_y = -velocity_y;
		   printf("\a"); 
        } 
		if (x >= right || x <= left) {
		   velocity_x = -velocity_x;
		   printf("\a");	
        } 
		
		Sleep(50); //50ms
		system("cls");
	}	
	return 0;
} 
```
### 1.7 动态的小球(绘制边框)
绘制边框，也是输出字符作为边框。要和小球先输出上面的一部分，再输出下面的一部分边框。可以用‘H’作为边框。当然，还是存在一些问题，比如说屏幕不断闪烁...
```c
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//小球坐标(x, y); 边框宽度和高度 
void PrintAll(int x, int y, int width, int height) { 
    for (int k = 0; k < width; k++) printf("█"); //障碍可以用H 
    printf("\n");
	for (int i = 1; i < height - 1; i++) {  
    	printf("█");
    	for (int j = 1; j < width - 1; j++) {
    		if (i == y && j == x) printf("Ο"); //希腊字母Omicron 
			else printf("  ");	
		}
		printf("█\n"); 
	}
	for (int k = 0; k < width; k++) printf("█");
	printf("\n\n");
}

int main() {
	int x, y, velocity_x, velocity_y;
	x = 1, y = 1;
	velocity_x = velocity_y = 1;
	
	int left, down, top, right;
	left = top = 0, down = 20, right = 40;
	
	while (1) {	
		PrintAll(x, y, right, down); 
		y += velocity_y; 
		x += velocity_x;
		if (y + 2 == down || y - 1 == top) { //碰到障碍
		   velocity_y = -velocity_y;
		   printf("\a"); 
        } 
		if (x + 2 == right || x - 1 == left) { //碰到障碍
		   velocity_x = -velocity_x;
		   printf("\a");	
        }  
        
		Sleep(100);
		system("cls");
	}	
	return 0;
} 
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191002160203848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

 



