#include <bits/stdc++.h>
using namespace std;

int find(const string &s, const string &t, int *sa) { //��s�в���t,sa��s�ĺ�׺���� 
	int i = 0, j = s.size();
	while (j - i > 1) {
		int k = (i + j) / 2;						  //���ַ� 
		if (s.compare(sa[k], t.size(), t) < 0) i = k; //ƥ��һ�θ��Ӷ�O(n)	
		else j = k;
	} 
	if (s.compare(sa[j], t.size(), t) == 0) 		  //�ҵ���,����t��s�е�λ��
		return sa[j]; 
	if (s.compare(sa[i], t.size(), t) == 0)
		return sa[i];
	return -1;										  //û�ҵ� 
}

int main() { 
	string s = "vamamadn", t = "ad"; 				  //ĸ�����Ӵ�
	int sa[] = {5, 3, 1, 6, 4, 2, 7, 0};
	int location = find(s, t, sa);
	cout << location << ":" << &s[location] << endl;  //��ӡt��s�е�λ�� 
    return 0;
}

