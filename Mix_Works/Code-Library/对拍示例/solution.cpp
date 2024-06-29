#include "ListNode.h"
#include "LinkedList.h"
#include "String.h"
using namespace std; 
/*
指针,串,链表:
从键盘上输入N个字符串，创建一个按字典递增序的单链表，按字典序输出。
并将单链表逆置，按字典序的倒序输出。再输入一个M，其中（N>=M）,从链表中删除第M大的字符串，再倒序输出。
(注: 链表结点的字符串采用动态指针分配空间，输入最大字符长度不超过1000) 
*/	
int main() {
    //freopen("input1.txt", "r", stdin);  //40个字符串,删除第25大字符串 
	//freopen("output2.txt", "w", stdout);
	freopen("data.in", "r", stdin);		  //随机测试 
	freopen("solution_data.out", "w", stdout);
	cout << "请输入想要排序的字符串个数: " << endl;
	 
	int N, M;
	cin >> N;
	LinkedList lst;
	String temp;
	//吸收多余的换行符 
	getchar();
	//输入N个字符串 
	cout << "输入要排序的" << N << "个字符串(回车换行分隔字符串): " << endl; 
	while (N--) {		
		temp.getline();
		lst.orderlyInsert(temp);  //按字典序递增顺序插入,确保单链表有序 
	}		
	//按字典序输出全部字符串 
	cout << "字符串按字典序递增排序的单链表如下: " << endl; 
	lst.printList();
	//将链表逆置 
	lst.reverse();
	//按照字典序的倒序输出
	cout << "单链表逆序后全部字符串如下: " << endl; 
	lst.printList(); 
	//输入M,从链表中删除第M大的字符串 
	cout << "请输入要删除的第几大的字符串(字典序): " << endl; 
	cin >> M;
	lst.remove(M);
	//按照倒序输出 
	cout << "单链表中删除第" << M << "大的字符串后情况如下: " << endl; 
	lst.printList();
	return 0;
} 
