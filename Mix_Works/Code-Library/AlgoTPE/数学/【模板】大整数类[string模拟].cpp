#include <iostream>
#include <string>
using namespace std;
//��char����洢ÿһλ=>ֱ��ʹ��string 
//�߾��ȱȽ�
bool cmp(const string& x, const string& y) { //<
	if (x.size() == y.size()) //������� 
		for (int i = 0; i < x.size(); ++i)
			if (x[i] != y[i])
				return x[i] < y[i]; //��λ�Ա� 
	return x.size() < y.size();
}
//������, ��Ҫxһ��Ҫ����y������
/* if (cmp(x, y)) swap(x, y); */ 

//�߾��ȼӷ�
string add(string x, string y) {
	if (cmp(x, y)) swap(x, y);
	int len = x.size() - y.size(); //��ĳ��� 1234��4->0004 
	for (int i = 1; i <= len; ++i) y = '0' + y; //�����߶��� 
	string z;
	int a = 0, b = 0; //aΪ��λ+��λ֮��, bΪ��λ 
	for (int i = x.size() - 1; i >= 0; --i) {
		a = x[i] - '0' + y[i] - '0' + b;
		b = a / 10, a %= 10;
		z = char(a + '0') + z;
	}
	if (b) z = char(b + '0') + z; //��ǰ��λ 
	return z;
}
//�߾��ȼ��� 
string sub(string x, string y) {
	if (cmp(x, y)) { cout << '-'; swap(x, y); } //���x<y, x-y<0
	string z;
	int len = x.size() - y.size(), b = 0;  //lenΪ���λ��, bΪ��λ 
	for (int i = y.size() - 1; i >= 0; --i) { //�ӵ�λ����λ 
		if (x[len + i] < y[i] + b) //1234 34 ��x�����һλ��ʼ�� 
			z = char(x[len + i] - y[i] - b + '0' + 10) + z, b = 1;
		else 
			z = char(x[len + i] - y[i] - b + '0') + z, b = 0;
	for (int i = len - 1; i >= 0; --i) {
		if (x[i] - b >= '0')
			z = char(x[i] - b) + z, b = 0;
		else 
			z = char(x[i] - b + 10) + z, b = 1;
	z.erase(0, z.find_first_not_of('0')); //ɾ��ǰ����0 
	if (!z.size()) z = '0'; //���û����,z��Ҫ����0 
	return z;
}
//�߾��ȳ˷�
string mul(string x, string y) {
	string z, tempz;
	int len1 = x.size(), len2 = y.size();
	for (int i = len2 - 1; i >= 0; --i) {
		tempz = "";
		int temp = y[i] - '0';
		int t = 0, cf = 0;
		if (temp != 0) {
			for (int j = 1; j <= len2 - 1; ++j) temp += '0';
			for (int j = len1 - 1; j >= 0; --j) {
				t = (temp * (x[j] - '0') + cf) % 10;
				cf = (temp * (x[j] - '0') + cf) / 10;
				tempz = char(t + '0') + tempz;
			}
			if (cf != 0) tempz = char(cf + '0') + tempz;
		}
		z = add(z, tempz);
	}
	z.erase(0, z.find_first_not_of('0')); //ɾ��ǰ����0 
	if (!z.size()) z = '0';  //���û����,z��Ҫ����0 
	return z;
}
/*
luogu:
P1601 A+B Problem(�߾�) P2142 �߾��ȼ��� P1303 A*B Problem P1255 ��¥�� P1604 B�������� 
*/
int main() {
 	return 0;
}
