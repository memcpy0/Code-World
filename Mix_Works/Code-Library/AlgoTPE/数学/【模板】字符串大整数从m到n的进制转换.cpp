#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;

string conversion(int m, string s, int n) {
	string b;
    int len = s.size(); 
    for (int i = 0; i < len; ) { //��ʮ�����ַ���δ����ʱ����ѭ��
    	int rest = 0; //���� 
        for (int j = i; j < len; j++) {
            int digit = s[j] - '0';
			int temp = (rest * m + digit) % n;//��һλ������=(��һλ������Ȩ+��һλ��)%ȡ�� 
            s[j] = (rest * m + digit) / n + '0'; //һλ�ַ��ı仯
            rest = temp;
        }
        b += char(rest + '0'); //�ӵ�λ���λ����ȡ����ַ�
        while (s[i] == '0') i++;//������һ�β����ĸ�λ��0
        //12345 / 2 ���06172 ��ô�´�����������Ϊ0���±�, ��6��ʼ ,�����±�������n����
    }
    return b; 
}
int main()
{
    string a,b,c;
    while (cin >> a){
        b = conversion(10, a, 2);
        a = conversion(2, b, 10);
        reverse(a.begin(), a.end());
        cout << a << endl;
    }
}
