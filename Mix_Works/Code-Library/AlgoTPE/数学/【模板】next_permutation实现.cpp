#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
bool myNextPermutation(T *start, T *end) { //[start,end)
	T *cur = end - 1, *pre = cur - 1; //preָ��partitionNumber 
	while (cur > start && *pre >= *cur) 
		--cur, --pre; //���ҵ������ɨ�裬���ֵ�һ��Υ���ǵݼ����Ƶ�����
	if (cur <= start) return false; //������������, �����ڸ�������� 
	//���ҵ������ɨ�裬���ֵ�һ����partitionNumber�����
	for (cur = end - 1; *cur <= *pre; --cur); //curָ��changeNumber  
	swap(*pre, *cur);
	reverse(pre + 1, end); //��β�������������� 
	return true; 
}

template<typename T>
bool myPrevPermutation(T *start, T *end) { //[start,end)
	T *cur = end - 1, *pre = cur - 1; //preָ��partitionNumber 
	while (cur > start && *pre <= *cur) 
		--cur, --pre; //���ҵ������ɨ�裬���ֵ�һ��Υ���ǵ������Ƶ�����
	if (cur <= start) return false; //������������, �����ڸ�С������ 
	//���ҵ������ɨ�裬���ֵ�һ����partitionNumberС����
	for (cur = end - 1; *cur >= *pre; --cur); //curָ��changeNumber  
	swap(*pre, *cur);
	reverse(pre + 1, end); //��β�������������� 
	return true; 
}

int main() { 
	char data[] = "cbaa";
//	int data[] = {2, 7, 8, 10}; 
	do {
		puts(data);
//		for (int i = 0; i < 4; ++i)
//			cout << data[i] << " ";
//		cout << endl;
	} while (myPrevPermutation(data, data + 4));
	return 0;	
}
