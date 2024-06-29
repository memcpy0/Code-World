#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
using namespace std;

string conversion(int m, string s, int n) {
	string b;
    int len = s.size(); 
    for (int i = 0; i < len; ) { //当十进制字符还未除完时继续循环
    	int rest = 0; //余数 
        for (int j = i; j < len; j++) {
            int digit = s[j] - '0';
			int temp = (rest * m + digit) % n;//这一位的余数=(上一位余数乘权+这一位数)%取余 
            s[j] = (rest * m + digit) / n + '0'; //一位字符的变化
            rest = temp;
        }
        b += char(rest + '0'); //从低位向高位存入取余的字符
        while (s[i] == '0') i++;//跳过这一次产生的高位的0
        //12345 / 2 变成06172 那么下次跳过了数字为0的下标, 从6开始 ,但是下标总是以n结束
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
