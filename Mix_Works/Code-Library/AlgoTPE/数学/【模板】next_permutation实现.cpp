#include <iostream>
#include <algorithm>
using namespace std;

template<typename T>
bool myNextPermutation(T *start, T *end) { //[start,end)
	T *cur = end - 1, *pre = cur - 1; //pre指向partitionNumber 
	while (cur > start && *pre >= *cur) 
		--cur, --pre; //从右到左进行扫描，发现第一个违背非递减趋势的数字
	if (cur <= start) return false; //整个排列逆序, 不存在更大的排列 
	//从右到左进行扫描，发现第一个比partitionNumber大的数
	for (cur = end - 1; *cur <= *pre; --cur); //cur指向changeNumber  
	swap(*pre, *cur);
	reverse(pre + 1, end); //将尾部的逆序变成正序 
	return true; 
}

template<typename T>
bool myPrevPermutation(T *start, T *end) { //[start,end)
	T *cur = end - 1, *pre = cur - 1; //pre指向partitionNumber 
	while (cur > start && *pre <= *cur) 
		--cur, --pre; //从右到左进行扫描，发现第一个违背非递增趋势的数字
	if (cur <= start) return false; //整个排列逆序, 不存在更小的排列 
	//从右到左进行扫描，发现第一个比partitionNumber小的数
	for (cur = end - 1; *cur >= *pre; --cur); //cur指向changeNumber  
	swap(*pre, *cur);
	reverse(pre + 1, end); //将尾部的逆序变成正序 
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
