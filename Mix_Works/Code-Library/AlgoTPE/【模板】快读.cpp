//快读，读入优化，一般用于卡常
//C++读入的速度 快读>scanf>关闭同步的cin>cin
//读取字符串，获取数字
//快读只能用于读取整数
#include <iostream>
using namespace std; 
using ll = long long;
inline ll read() {
	ll num = 0; bool flag = false;
	char c = getchar();
	while (c < '0' || c > '9') {
		if (c == '-') flag = true;
		c = getchar();
	}
	while (c >= '0' && c <= '9') {
		num = num * 10 + c - '0';
		c = getchar();
	}
	return flag ? -num : num;
}
