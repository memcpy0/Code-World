#include <bits/stdc++.h>
using namespace std;
/*
用STL完成的正确程序, 使用暴力方法完成程序, 用于后续的对拍工作 
*/
void printList(const list<string>& lst) {
	for (auto it = lst.cbegin(); it != lst.cend(); ++it) cout << *it << endl;
}

int main() {
    //freopen("input1.txt", "r", stdin);  //40个字符串,删除第25大字符串 
	//freopen("output1.txt", "w", stdout);
	freopen("data.in", "r", stdin);		//随机测试 
	freopen("data.out", "w", stdout);
	cout << "请输入想要排序的字符串个数: " << endl;
	 
	int N, M;
	cin >> N;
	list<string> t;
	string temp;
	//吸收多余的换行符 
	getline(cin, temp); 
	//输入N个字符串 
	cout << "输入要排序的" << N << "个字符串(回车换行分隔字符串): " << endl; 
	while (N--) {		
		getline(cin, temp);
		t.push_back(temp);
	}
	
	//按字典序递增排序 
	t.sort();			
	//按字典序输出全部字符串 
	cout << "字符串按字典序递增排序的单链表如下: " << endl; 
	printList(t);
	//将链表逆置 
	reverse(t.begin(), t.end());
	//按照字典序的倒序输出
	cout << "单链表逆序后全部字符串如下: " << endl; 
	printList(t); 
	//输入M,从链表中删除第M大的字符串 
	cout << "请输入要删除的第几大的字符串(字典序): " << endl; 
	cin >> M;
	list<string>::iterator it = t.begin();
	advance(it, M - 1);
	t.erase(it); 
	//按照倒序输出 
	cout << "单链表中删除第" << M << "大的字符串后情况如下: " << endl; 
	printList(t);
	return 0;
}

	
	
