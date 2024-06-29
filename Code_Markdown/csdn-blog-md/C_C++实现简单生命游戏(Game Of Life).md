@[toc]

继main函数内部一把梭后，我们进行了模块化改造，但是这样管理游戏数据还是很不好的。加上数组，我们就有能力实现更有意思的游戏，游戏中的一些写法也会改变。

对了，虽然大体上是C实现游戏，但是有的地方写法可能是C++的，建议用.cpp格式运行。我也懒得区分了。

## 点子-Game of Life
> 生命游戏是英国数学家约翰·何顿·康威在1970年发明的细胞自动机。它最初于1970年10月在《科学美国人》杂志中Martin Gardner的“数学游戏”专栏出现。
>
> 生命游戏其实是一个零玩家游戏，它包括**一个二维矩形世界**，这个世界中的每个方格居住着一个活着的或死了的细胞。一个细胞在下一个时刻生死取决于相邻八个方格中活着的或死了的细胞的数量。如果相邻方格活着的细胞数量过多，这个细胞会因为资源匮乏而在下一个时刻死去；相反，如果周围活细胞过少，这个细胞会因太孤单而死去。
> 实际中，你可以**设定周围活细胞的数目怎样时才适宜该细胞的生存**。如果这个数目设定过低，世界中的大部分细胞会因为找不到太多的活的邻居而死去，直到整个世界都没有生命；如果这个数目设定过高，世界中又会被生命充满而没有什么变化。
> 实际中，这个数目一般选取2或者3；这样整个生命世界才不至于太过荒凉或拥挤，而是一种动态的平衡。这样的话，游戏的规则就是：**当一个方格周围有2或3个活细胞时，方格中的活细胞在下一个时刻继续存活**.......
> 还可以设定一些更加复杂的规则，例如当前方格的状况不仅由父一代决定，而且还考虑祖父一代的情况......
> 
以上所说的都是引用自百度百科。[wiki](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)上的记录更加完善。

有关的社区：[生命游戏贴吧](https://tieba.baidu.com/f?kw=%E7%94%9F%E5%91%BD%E6%B8%B8%E6%88%8F&ie=utf-8) [Golly生命游戏软件贴吧](http://tieba.baidu.com/f?ie=utf-8&kw=golly&fr=search&red_tag=d2202396210)

生命游戏的玩法多样，有的规则也很复杂，不过这里是最简单的，就是采用百度百科上面的规则：
> 每个矩阵方格可以包含一个有机体，不在边上的有机体有8个相邻方格。
1． 如果一个细胞周围有3个细胞为生，则该细胞为生（即该细胞若原先为死，则转为生，若原先为生，则保持不变）;
2． 如果一个细胞周围有2个细胞为生，则该细胞的生死状态保持不变；
3． 在其它情况下，该细胞为死（即该细胞若原先为生，则转为死，若原先为死，则保持不变）。

另外，我们不对(二维数组)边界上面的细胞生命进行处理。

## 实现
### 1. 游戏框架-静止图案
利用以前文章中的相关游戏框架，进行游戏初始化，矩阵Cells中的点随机赋值，得到静止图案。
```c
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define Height 30 //游戏画面尺寸 
#define Width 50 
int Cells[Height][Width]; //所有位置随机置1或0

void startup() {
	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width; ++j) {
			Cells[i][j] = rand() % 2;
		}
	}
}
void show() {
	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width; ++j) {
			if (Cells[i][j]) printf("*"); //活的细胞输出
			else printf(" ");             //死的细胞为空
		}
		printf("\n");
	} 
} 
void updateWithoutInput() { //游戏内部更新 

} 
void updateWithInput() { //用户输入相关更新 
	
}

int main() {
	srand(time(NULL));
	startup();
	while (1) {
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191005235600682.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
### 2. 动态图案-按规则繁衍死亡
|方位 |方位 | 方位|
|:-|:-|:-|
|(i - 1, j - 1) |(i - 1, j) |(i - 1, j + 1) |
|(i, j - 1) |(i, j) |(i, j + 1)
|(i + 1, j - 1)|(i + 1, j)|(i + 1, j + 1)

周围八个方位需要检查，该怎么做呢？我的方法是**用一个临时的二维数组tempCells，来记录Cells的下一个状态**。每次**对不在边界上的Cells中的点的周围情况进行统计，并据此更新tempCells的值**，然后**用tempCells更新Cells的值**。

每次打印一帧后，必须重新绘制图形，用前面的方法**隐藏光标**，**移动光标到原点**刷新图案。
```c
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define Height 50 //游戏画面尺寸 
#define Width 180 
int Cells[Height][Width]; //所有位置随机置1或0
int tempCells[Height][Width]; //临时矩阵 

void hideCursor() { 
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // 第二个值为0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info); 
}

void movexy(int x, int y) {
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
 
void startup() {
	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width; ++j) {
			Cells[i][j] = rand() % 2;
		}
	}
	hideCursor();
}

void show() {
	movexy(0, 0);
	for (int i = 0; i < Height; ++i) {
		for (int j = 0; j < Width; ++j) {
			if (Cells[i][j]) printf("*"); //活的细胞输出
			else printf(" ");             //死的细胞为空
		}
		printf("\n");
	} 
	Sleep(150);
} 

void updateWithoutInput() { //游戏内部更新 
	int NeighbourNum;   //周边细胞的数目
	for (int i = 1; i < Height - 1; ++i) {
		for (int j = 1; j < Width - 1; ++j) {
			NeighbourNum = Cells[i - 1][j - 1] + Cells[i - 1][j] + Cells[i - 1][j + 1] +
						   Cells[i][j - 1] + Cells[i][j] + Cells[i][j + 1] +
						   Cells[i + 1][j - 1] + Cells[i + 1][j] + Cells[i + 1][j + 1]; 
			if (NeighbourNum == 3)  //通过修改这里的条件, 可以改变生命游戏中的繁衍情况 
				tempCells[i][j] = 1;
			else if (NeighbourNum == 2)  //通过修改这里的条件, 可以改变生命游戏中的繁衍情况 
				tempCells[i][j] = Cells[i][j];
			else tempCells[i][j] = 0;
		}
	}
	for (int i = 1; i < Height - 1; ++i) 
		for (int j = 1; j < Width - 1; ++j) 
			Cells[i][j] = tempCells[i][j]; 
} 

void updateWithInput() { //用户输入相关更新 
	
}

int main() {
	srand(time(NULL));
	startup();
	while (1) {
		show();
		updateWithoutInput();
		updateWithInput();
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191006002933738.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)生命游戏有时完全收敛，有时局部收敛，有时贫瘠如洗，有时生命又无比繁盛。

最后，大家可以尝试修改规则，比如：
- **初始化不同的数据**，比如初始的时候都是生命，或一半的地图是生命；
- **改变游戏规则**，如周围的细胞是3，2的时候都增加，或2，4的时候维持原状等等；
- **增加另一个物种**，食肉动物、食草动物，互相抑制；
- **增加不同地形**，比如某块区域有水源，生命更容易生存；
- 交互投食，按+加速、按-减速； 

按照Wiki上面的规则：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191006005705652.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

```cpp
if (Cells[i][j] == 1) { //Any live cell
	if (NeighbourNum < 2) 
		tempCells[i][j] = 0; //fewer than two live neighbours dies, underpopulation
	else if ((NeighbourNum == 2 || NeighbourNum == 3)) 
		tempCells[i][j] = 1; //two or three live neighbours lives on to the next generation
	else if (NeighbourNum > 3)  
		tempCells[i][j] = 0; //more than three live neighbours dies, overpopulation
} 
else if (NeighbourNum == 3 && Cells[i][j] == 0) //Any dead cell 
	tempCells[i][j] = 1; //exactly three live neighbours becomes a live cell,reproduction.
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20191006005829167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)
地图中部近乎完全收敛，生命群落稀疏，只有边缘还有活动，可能是因为没有处理边界情况，导致边界在初始化后存在太多细胞生命。





