#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstdio>
using namespace std;
/*
生成对拍用的测试数据,写入到data.in中 
*/ 

int main() {
	freopen("data.in", "w", stdout);
	srand(time(NULL));
	int N = rand() % 5000 + 1;            //生成1~5000间的随机整数N
	cout << N << endl;				      //写入N到文件中
	
	for (int i = 0; i < N; ++i) {		  //生成N条字符串写入文件 
		int strLen = rand() % 1000 + 1;   //生成1~1000间的随机整数,作为字符串的长度
		string t; 
		char c;
		for (int j = 0; j < strLen; ++j) {
			while (true) {				  //可修改生成的字符 
				c = rand() % 127 + 1;     //随机生成字母、数字、部分标点符号或空格
				if (isalnum(c) || c == ' ' || c == '!' || c == ',' || c == '?' || c == '.') 
					break;
			}
			t.push_back(c);
		}
		cout << t << endl;				  //输出一个字符串 
	}
	int M;
	while ((M = rand() % 5000 + 1) > N);  //生成1~50000间的随机整数M<=N 
	cout << M << endl; 
	return 0;
}
 
