//����������Ż���һ�����ڿ���
//C++������ٶ� ���>scanf>�ر�ͬ����cin>cin
//��ȡ�ַ�������ȡ����
//���ֻ�����ڶ�ȡ����
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
