#include <bits/stdc++.h>
using namespace std;
#define cutOff 6  
const int maxn = 101001;
int n, a[maxn]; 

/* ------------------ 三数中值分割算法 ---------------------- */ 
void InsertionSort(int a[], int l, int r) {
	for (int i = l + 1; i <= r; ++i) {
		int t = a[i], j;
		for (j = i; j > 0 && a[j - 1] > t; --j)
		 	a[j] = a[j - 1];
		a[j] = t;
	}
} 
int PartitionMedian3(int a[], int left, int right) { //找到三值中点 
	int center = (left + right) / 2;
	if (a[left] > a[center]) swap(a[left], a[center]);
	if (a[left] > a[right]) swap(a[left], a[right]);
	if (a[center] > a[right]) swap(a[center], a[right]); //a[left] <= a[center] <= a[right]
	swap(a[center], a[right - 1]); //隐藏的pivot,这下有a[left]<=a[right-1]<=a[right]
	return a[right - 1];
}
void QuickSortMedian3(int a[], int l, int r) {
	int i, j, pivot;
	if (l + cutOff <= r) {
		pivot = PartitionMedian3(a, l, r);
		i = l, j = r - 1; //a[i]<=a[j] 或者说 a[i]<=pivot	
		while (true) {
			while (a[++i] < pivot) {}
			while (a[--j] > pivot) {}
			if (i < j) {
				swap(a[i], a[j]);
			} else break;
		}
		swap(a[i], a[r - 1]);		
		QuickSortMedian3(a, l, i - 1);
		QuickSortMedian3(a, i + 1, r);
	} else //在小数组上面做插入排序 
		InsertionSort(a, l, r);
}

int main() {  
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) 
		scanf("%d", &a[i]);
	QuickSortMedian3(a, 0, n - 1);
	for (int i = 0; i < n; ++i) {
		if (i != 0) printf(" "); 
		printf("%d", a[i]); 
	}  
 	return 0;
}
