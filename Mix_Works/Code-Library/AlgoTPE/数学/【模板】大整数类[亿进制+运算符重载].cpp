#include <iostream>
#include <string> 
#include <cstring>
#include <vector> 
using namespace std;
//使用亿进制、重载运算符和vector实现大整数类, 存储高精度非负整数 
struct BigInteger {
	static const int BASE = 100000000; //进制：逢亿进一, 取一当亿 
	static const int WIDTH = 8;
	vector<int> d; //保持大整数的各个数位 
	
	BigInteger(long long num = 0) { *this = num; } //构造函数, 这样写需要重载=运算符
	BigInteger operator = (long long num) { //x = 数字 
		d.clear();
		do {
			d.push_back(num % BASE);
			num /= BASE;
		} while (num);
		return *this; 
	} 
	BigInteger operator = (const string& str) { //x = string
		d.clear();
		//小技巧：len = str.length() % WIDTH ? str.length / WIDTH + 1 : str.length() / WIDTH;
		int x, len = (str.length() - 1) / WIDTH + 1; 
		for (int i = 0; i < len; ++i) {
			int end = str.length() - i * WIDTH; //先存低位数字或者是string的高位 
			int start = max(0, end - WIDTH);
			sscanf(str.substr(start, end - start).c_str(), "%d", &x);
			d.push_back(x);
		}
		return *this;
	}
	ostream& operator << (ostream& out, const BigInteger& x) { //cout << x;
		out << x.d.back(); //最高位不需要补0 
		for (int i = x.d.size() - 2; i >= 0; --i) {
			char buf[20];
			sprintf(buf, "%08d", x.d[i]); //后面的每8位需要补0 
			int len = strlen(buf);
			for (int j = 0; j < len; ++j) out << buf[j];
		}
		return out;
	}
	istream& operator >> (istream& in, BingInteger& x) { //cin >> x;
		string s;
		if (!(in >> s)) return in; //没有读入, 自己返回 
		x = s; //利用重载的= 
		return in;
	}
	BigInteger operator + (const BigInteger& b) const {
		BigInteger c;
		c.d.clear();
		for (int i = 0, g = 0; ; ++i) {
			
			
		
		
	}
	
	
	
