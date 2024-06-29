#include <iostream>
using namespace std;
int n, m;
void remove(int a[], int t) { //删除数组a中下标为t的数 
	if (t < 0 || t >= n) return;
	for (int i = t; i < n - 1; ++i)
		a[i] = a[i + 1]; 
}
//打印数组, 用做调试和实验 
void print(int a[]) { for (int i = 0; i < n; ++i) printf("%d ", a[i]); }


int main() {
	cin >> n >> m;
	int arr[n];
	//初始化为1-n
	for (int i = 0; i < n; ++i) arr[i] = i + 1;
	//可随机访问, 不需要像链表一样一个个数过去 
 	int pos = 0; //初始化为0是因为数组从0开始; 取余操作会得到0-(n-1)的数的范围 
	while (n > 1) {
		pos = (pos + m - 1) % n; //得到pos后面的第m-1个数的下标, 取余不断变化的n 
		print(arr); printf(" | a[%d] = %d\n", pos, arr[pos]); 
		remove(arr, pos); //删除这个数; pos指向的位置现在放的是下一个数 
		--n; //减少一个人 
	}
	cout << arr[0]; 
	return 0;	
}
