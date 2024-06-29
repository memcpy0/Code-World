#include <bits/stdc++.h>
using namespace std;
/*
`atoi` ������һ���ַ���ת����һ�����������У��������ַ���**�Ƿ�**����**�� `"0"`** ʱ��`atoi` ���� `0` ������һ��ȫ�ֱ������������������
- �Ƿ�����ʱ������ `0` �������ȫ�ֱ�����Ϊһ�������ǣ�
- ������ `"0"` ʱ������ `0` ����������ȫ�ֱ�����

�� `atoi` ���� `0` ʱ�������߿���ͨ�����ȫ�ֱ�����֪�����뾿���ǷǷ����루`NULL` ָ�롢���ַ��� `""` ������ `0-9, +, -` ֮��ķǷ��ַ����������ַ��� `"0"` ��

��Ȼ��������������̫һ����ֻҪ������Ч�����ֲ��֣��Ϳ��Է�������ֵ������**��һ���ǿո��ַ�����һ����Ч�����ַ�**��**�ַ���Ϊ��**��**�ַ����������հ��ַ�**ʱ��

���⣬���ǻ���Ҫ��������������

> C++�г�Ա�����ĳ�ʼ��˳��ֻ�������������������˳���йأ������ʼ���б��е�˳���޹ء�
*/

/*
���ܲ��ԣ�������ַ�����ʾ������������0
�߽�ֵ���ԣ���������������С�ĸ�����
����������ԣ�NULLָ�롢�����ַ���Ϊ���ַ����������ַ������з������ַ��������ַ�������һ�����Ż��߸��ŵ� 
*/
namespace myAtoi { //49
//����һ��ȫ������g_nStatus����Ƿ������˷Ƿ�����,���������ַ���ָ���ǿ�ָ��
//����ȫ�ֱ���Ȼ��ֱ�ӷ���;
//�����������ַ����е������ַ�:
//������ֻ�п��ܳ������ַ����ĵ�һ���ַ�,�ȴ����һ���ַ�,�Ǹ�������Ϊ����
//��������ַ�,����0-9֮����ַ�ʱ��ֹ����;��������ʱ���ۼ���ȥ 
	enum Status { kValid = 0, kInvaild};
	int g_nStatus = kValid;
	int strToInt(const char *str) {
		g_nStatus = kInvaild;
		int num = 0;
		if (*str == '\0') return num; //�����ǿմ�"" 
		
		if (str != nullptr) { //�����ǿ�ָ��NULLʱֱ������ 
			const char *digit = str;
			bool minus = false; //�Ƿ��Ǹ��� 
			if (*digit == '+') 
				++digit;	
			else if (*digit == '-') {
				++digit;
				minus = true;
			}
			//û�п���ֻ��һ�����Ż��߸���,������g_nStatus = kValid!!!
			//if (*digit == '\0') return 0; 
			while (*digit) {
				if (*digit >= '0' && *digit <= '9') {
					num = num * 10 + (*digit - '0');	
					++digit;
				} else {
					num = 0;
					break;
				}
			}
			if (*digit == '\0') {
				g_nStatus = kValid;
				if (minus) 
					num = 0 - num;
			}
		}
		return num;
	} 
	//�ο�����
	int strToInt(const char *str) {
		g_nStatus = kInvalid;
		long long num = 0;
		if (str != nullptr && *str != '\0') { //��ָ����߿մ� 
			bool minus = false;
			if (*str == '+') 
				++str;
			else if (*str == '-') {
				++str;
				minus = true;
			}
			
			if (*str != '\0')
				num = StrToIntCore(str, minus);			
		}
		return (int)num;  
	}
	long long StrToIntCore(const char *digit, bool minus) {
		long long num = 0;
		while (*digit) {
			if (*digit >= '0' && *digit <= '9') {
				int flag = minus ? -1 : 1;
				num = num * 10 + flag * (*digit - '0');
				
				if ((!minus && num > 0x7FFFFFFF) || //����������ж����������������������� 
					(minus && num < (signed int)0x80000000)) { //���������
						num = 0;	//����Ϊ0 
						break;
				}
				++digit;
			} else { //�����Ƿ��ַ� 
				num = 0;   
				break;	
			}
		}
		if (*digit == '\0')	g_nStatus = kValid;
		return num;
	}			 
}

int main() { 

    return 0;
}
 
