/*
	《C++程序设计》课程实验作业：结构化程序设计和基于对象编程练习
	程序：瓦瓦捡豆游戏
	任务：为瓦瓦分配不同的任务，然后设计瓦瓦捡豆的最优路径算法，并编写程序代码，执行程序验证结果

	创 建 人：Zhang Hua
	创建时间：2011-02-20

	修改记录：
	2019-03-01 修改遗留的C风格代码。
*/
#include <iostream>
using namespace std;

#include "XWawa.h"

#define VS2017 // 如果你用的不是VS2010，就把VS2010改成你所用的IDE的名称，例如VS2017，DEVCPP

#ifdef VS2010
	#pragma comment(lib, "XWawa.lib")
#else
	#ifdef VS2017
		#pragma comment(lib, "XWawa_VS2017.lib")
	#else
		#pragma comment(lib, "XWawa.a")
	#endif
#endif

void BEGIN();
void END(XWawa&);

// ---------------------任务1：无障碍的爬山和下山---------------------------------------- 
void task1(XWawa &wawa) {
	int flag = 1;
	do {
		wawa.moveRight();  
		if (wawa.beanIsHere()) {
			wawa.pickupBean();
			if (flag && wawa.upsideIsClear()) wawa.moveUp(); 
			else flag = 0; //this time move down and move right.
		    if (!flag && wawa.downsideIsClear()) wawa.moveDown();
		} 
	} while (!wawa.taskIsAccomplished());
}

// ---------------------任务2：越过固定障碍------------------------------------------------  
void task2(XWawa &wawa) {
	do {
		wawa.moveRight();
		if (wawa.beanIsHere()) wawa.pickupBean();
		if (wawa.taskIsAccomplished()) break;
		wawa.moveUp();
		wawa.moveUp();
		wawa.moveRight();
		wawa.moveRight();
		wawa.moveDown();
		wawa.moveDown();
		if (wawa.beanIsHere()) wawa.pickupBean();
	} while (!wawa.taskIsAccomplished());
}

// ---------------------任务3：越过高度不确定、宽度固定、有间距的若干障碍------ 
void task3(XWawa &wawa) { 
	do { //先向右再向上, 越过障碍, 再向下, 循环
		while (wawa.rightIsClear()) { //越过不确定间距
			wawa.moveRight();
			if (wawa.beanIsHere()) wawa.pickupBean();
		 }
		if (wawa.taskIsAccomplished()) break; //向右到障碍并完成任务
		while (!wawa.rightIsClear()) wawa.moveUp(); //遇到不确定高度的障碍, 往上移动
		wawa.moveRight(); //越过固定宽度的障碍
		wawa.moveRight();
		while (wawa.downsideIsClear()) wawa.moveDown();  //没有遇到障碍, 往下移动
		if (wawa.beanIsHere()) wawa.pickupBean(); 
	 } while (!wawa.taskIsAccomplished()); //向下到底部并完成任务 
}

// ---------------------任务4：越过高度、宽度和间距都不确定的若干障碍------------------
void task4(XWawa &wawa) {
   while (!wawa.taskIsAccomplished()) { //暴力搜索每一列, 每一列吃掉一颗豆子
		while (wawa.upsideIsClear()) wawa.moveUp(); //移动到顶部
		wawa.moveRight(); //检查下一列
		while (wawa.downsideIsClear()) wawa.moveDown(); //移动到底部
		if (wawa.beanIsHere()) {
			wawa.pickupBean(); //是豆子就捡起来
			while (wawa.rightIsClear()) { //看右边是不是还有豆子
				wawa.moveRight();
				if (wawa.beanIsHere()) wawa.pickupBean();
			}
		}
	}
}
/*  任务5：任务4的若干障碍从顶部延伸下来，豆子都放在顶部
*   解决方法：可以沿用任务4的方法，搜索每一列 
*/
int main()
{
    XWawa wawa;
	int taskNum;  //增加菜单，让用户选择观看任一个任务
	cout << "请输入要观看的任务编号：";
	cin >> taskNum; cin.get();     //吸收换行符
	wawa.assignTask(taskNum); // 为瓦瓦分派任务
	BEGIN();
	switch (taskNum) {
		case 1: task1(wawa);  break;
		case 2: task2(wawa);  break;
		case 3: task3(wawa);  break;
		case 4: task4(wawa);  break;
	} 
	END(wawa); 
	return 0;
}
// 提示按下任意键后开始执行后面的指令（完成任务的算法实现）
void BEGIN()
{
	cout<<"\n按下Enter键后开始执行指令。";
	cin.get();
}
// 检查上面的指令执行后是否完成了任务，并给出相应的提示
void END(XWawa &wawa)
{
	cout<<"\n指令已执行，正在检查任务完成情况......\n";
	if (wawa.taskIsAccomplished())
	{
		cout<<"\n恭喜！任务已完成。瓦瓦移动了"<<wawa.GetMoveCount()<<"次。\n";
	}
	else
	{
		cout<<"\n很遗憾，任务没有完成。修改代码，再试一次吧。\n";
	}
	cout<<"\n按下Enter键后退出程序...\n";
	cin.get();
}
