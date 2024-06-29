#include <iostream>
#include <string>
using namespace std;
//用char数组存储每一位=>直接使用string 
//高精度比较
bool cmp(const string& x, const string& y) { //<
	if (x.size() == y.size()) //长度相等 
		for (int i = 0; i < x.size(); ++i)
			if (x[i] != y[i])
				return x[i] < y[i]; //逐位对比 
	return x.size() < y.size();
}
//两个数, 需要x一定要大于y，可以
/* if (cmp(x, y)) swap(x, y); */ 

//高精度加法
string add(string x, string y) {
	if (cmp(x, y)) swap(x, y);
	int len = x.size() - y.size(); //差的长度 1234和4->0004 
	for (int i = 1; i <= len; ++i) y = '0' + y; //让两者对齐 
	string z;
	int a = 0, b = 0; //a为两位+进位之和, b为进位 
	for (int i = x.size() - 1; i >= 0; --i) {
		a = x[i] - '0' + y[i] - '0' + b;
		b = a / 10, a %= 10;
		z = char(a + '0') + z;
	}
	if (b) z = char(b + '0') + z; //往前进位 
	return z;
}
//高精度减法 
string sub(string x, string y) {
	if (cmp(x, y)) { cout << '-'; swap(x, y); } //如果x<y, x-y<0
	string z;
	int len = x.size() - y.size(), b = 0;  //len为差的位数, b为进位 
	for (int i = y.size() - 1; i >= 0; --i) { //从低位往高位 
		if (x[len + i] < y[i] + b) //1234 34 从x的最后一位开始减 
			z = char(x[len + i] - y[i] - b + '0' + 10) + z, b = 1;
		else 
			z = char(x[len + i] - y[i] - b + '0') + z, b = 0;
	for (int i = len - 1; i >= 0; --i) {
		if (x[i] - b >= '0')
			z = char(x[i] - b) + z, b = 0;
		else 
			z = char(x[i] - b + 10) + z, b = 1;
	z.erase(0, z.find_first_not_of('0')); //删除前导的0 
	if (!z.size()) z = '0'; //如果没有了,z还要补个0 
	return z;
}
//高精度乘法
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
	z.erase(0, z.find_first_not_of('0')); //删除前导的0 
	if (!z.size()) z = '0';  //如果没有了,z还要补个0 
	return z;
}
/*
luogu:
P1601 A+B Problem(高精) P2142 高精度减法 P1303 A*B Problem P1255 数楼梯 P1604 B进制星球 
*/
int main() {
 	return 0;
}
