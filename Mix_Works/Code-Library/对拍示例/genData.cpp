#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstdio>
using namespace std;
/*
���ɶ����õĲ�������,д�뵽data.in�� 
*/ 

int main() {
	freopen("data.in", "w", stdout);
	srand(time(NULL));
	int N = rand() % 5000 + 1;            //����1~5000����������N
	cout << N << endl;				      //д��N���ļ���
	
	for (int i = 0; i < N; ++i) {		  //����N���ַ���д���ļ� 
		int strLen = rand() % 1000 + 1;   //����1~1000����������,��Ϊ�ַ����ĳ���
		string t; 
		char c;
		for (int j = 0; j < strLen; ++j) {
			while (true) {				  //���޸����ɵ��ַ� 
				c = rand() % 127 + 1;     //���������ĸ�����֡����ֱ����Ż�ո�
				if (isalnum(c) || c == ' ' || c == '!' || c == ',' || c == '?' || c == '.') 
					break;
			}
			t.push_back(c);
		}
		cout << t << endl;				  //���һ���ַ��� 
	}
	int M;
	while ((M = rand() % 5000 + 1) > N);  //����1~50000����������M<=N 
	cout << M << endl; 
	return 0;
}
 
