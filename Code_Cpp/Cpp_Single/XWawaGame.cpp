/*
	��C++������ơ��γ�ʵ����ҵ���ṹ��������ƺͻ��ڶ�������ϰ
	�������߼���Ϸ
	����Ϊ���߷��䲻ͬ������Ȼ��������߼񶹵�����·���㷨������д������룬ִ�г�����֤���

	�� �� �ˣ�Zhang Hua
	����ʱ�䣺2011-02-20

	�޸ļ�¼��
	2019-03-01 �޸�������C�����롣
*/
#include <iostream>
using namespace std;

#include "XWawa.h"

#define VS2017 // ������õĲ���VS2010���Ͱ�VS2010�ĳ������õ�IDE�����ƣ�����VS2017��DEVCPP

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

// ---------------------����1�����ϰ�����ɽ����ɽ---------------------------------------- 
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

// ---------------------����2��Խ���̶��ϰ�------------------------------------------------  
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

// ---------------------����3��Խ���߶Ȳ�ȷ������ȹ̶����м��������ϰ�------ 
void task3(XWawa &wawa) { 
	do { //������������, Խ���ϰ�, ������, ѭ��
		while (wawa.rightIsClear()) { //Խ����ȷ�����
			wawa.moveRight();
			if (wawa.beanIsHere()) wawa.pickupBean();
		 }
		if (wawa.taskIsAccomplished()) break; //���ҵ��ϰ����������
		while (!wawa.rightIsClear()) wawa.moveUp(); //������ȷ���߶ȵ��ϰ�, �����ƶ�
		wawa.moveRight(); //Խ���̶���ȵ��ϰ�
		wawa.moveRight();
		while (wawa.downsideIsClear()) wawa.moveDown();  //û�������ϰ�, �����ƶ�
		if (wawa.beanIsHere()) wawa.pickupBean(); 
	 } while (!wawa.taskIsAccomplished()); //���µ��ײ���������� 
}

// ---------------------����4��Խ���߶ȡ���Ⱥͼ�඼��ȷ���������ϰ�------------------
void task4(XWawa &wawa) {
   while (!wawa.taskIsAccomplished()) { //��������ÿһ��, ÿһ�гԵ�һ�Ŷ���
		while (wawa.upsideIsClear()) wawa.moveUp(); //�ƶ�������
		wawa.moveRight(); //�����һ��
		while (wawa.downsideIsClear()) wawa.moveDown(); //�ƶ����ײ�
		if (wawa.beanIsHere()) {
			wawa.pickupBean(); //�Ƕ��Ӿͼ�����
			while (wawa.rightIsClear()) { //���ұ��ǲ��ǻ��ж���
				wawa.moveRight();
				if (wawa.beanIsHere()) wawa.pickupBean();
			}
		}
	}
}
/*  ����5������4�������ϰ��Ӷ����������������Ӷ����ڶ���
*   ���������������������4�ķ���������ÿһ�� 
*/
int main()
{
    XWawa wawa;
	int taskNum;  //���Ӳ˵������û�ѡ��ۿ���һ������
	cout << "������Ҫ�ۿ��������ţ�";
	cin >> taskNum; cin.get();     //���ջ��з�
	wawa.assignTask(taskNum); // Ϊ���߷�������
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
// ��ʾ�����������ʼִ�к����ָ����������㷨ʵ�֣�
void BEGIN()
{
	cout<<"\n����Enter����ʼִ��ָ�";
	cin.get();
}
// ��������ָ��ִ�к��Ƿ���������񣬲�������Ӧ����ʾ
void END(XWawa &wawa)
{
	cout<<"\nָ����ִ�У����ڼ������������......\n";
	if (wawa.taskIsAccomplished())
	{
		cout<<"\n��ϲ����������ɡ������ƶ���"<<wawa.GetMoveCount()<<"�Ρ�\n";
	}
	else
	{
		cout<<"\n���ź�������û����ɡ��޸Ĵ��룬����һ�ΰɡ�\n";
	}
	cout<<"\n����Enter�����˳�����...\n";
	cin.get();
}
