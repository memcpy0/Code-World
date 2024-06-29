#include <iostream>
#include <string> 
#include <cstring>
#include <vector> 
using namespace std;
//ʹ���ڽ��ơ������������vectorʵ�ִ�������, �洢�߾��ȷǸ����� 
struct BigInteger {
	static const int BASE = 100000000; //���ƣ����ڽ�һ, ȡһ���� 
	static const int WIDTH = 8;
	vector<int> d; //���ִ������ĸ�����λ 
	
	BigInteger(long long num = 0) { *this = num; } //���캯��, ����д��Ҫ����=�����
	BigInteger operator = (long long num) { //x = ���� 
		d.clear();
		do {
			d.push_back(num % BASE);
			num /= BASE;
		} while (num);
		return *this; 
	} 
	BigInteger operator = (const string& str) { //x = string
		d.clear();
		//С���ɣ�len = str.length() % WIDTH ? str.length / WIDTH + 1 : str.length() / WIDTH;
		int x, len = (str.length() - 1) / WIDTH + 1; 
		for (int i = 0; i < len; ++i) {
			int end = str.length() - i * WIDTH; //�ȴ��λ���ֻ�����string�ĸ�λ 
			int start = max(0, end - WIDTH);
			sscanf(str.substr(start, end - start).c_str(), "%d", &x);
			d.push_back(x);
		}
		return *this;
	}
	ostream& operator << (ostream& out, const BigInteger& x) { //cout << x;
		out << x.d.back(); //���λ����Ҫ��0 
		for (int i = x.d.size() - 2; i >= 0; --i) {
			char buf[20];
			sprintf(buf, "%08d", x.d[i]); //�����ÿ8λ��Ҫ��0 
			int len = strlen(buf);
			for (int j = 0; j < len; ++j) out << buf[j];
		}
		return out;
	}
	istream& operator >> (istream& in, BingInteger& x) { //cin >> x;
		string s;
		if (!(in >> s)) return in; //û�ж���, �Լ����� 
		x = s; //�������ص�= 
		return in;
	}
	BigInteger operator + (const BigInteger& b) const {
		BigInteger c;
		c.d.clear();
		for (int i = 0, g = 0; ; ++i) {
			
			
		
		
	}
	
	
	
