#include <bits/stdc++.h>
using namespace std;

int find(const string &s, const string &t, int *sa) { //在s中查找t,sa是s的后缀数组 
	int i = 0, j = s.size();
	while (j - i > 1) {
		int k = (i + j) / 2;						  //二分法 
		if (s.compare(sa[k], t.size(), t) < 0) i = k; //匹配一次复杂度O(n)	
		else j = k;
	} 
	if (s.compare(sa[j], t.size(), t) == 0) 		  //找到了,返回t在s中的位置
		return sa[j]; 
	if (s.compare(sa[i], t.size(), t) == 0)
		return sa[i];
	return -1;										  //没找到 
}

int main() { 
	string s = "vamamadn", t = "ad"; 				  //母串和子串
	int sa[] = {5, 3, 1, 6, 4, 2, 7, 0};
	int location = find(s, t, sa);
	cout << location << ":" << &s[location] << endl;  //打印t在s中的位置 
    return 0;
}

