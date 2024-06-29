#include <iostream>
#include <cstdio>
using namespace std;
const int maxn = 100010;
int arr[maxn], temp[maxn], n;

void Merge(int a[], int l1, int r1, int l2, int r2, int t[]) {
	int index = 0, p = l1, q = l2;
	while (p <= r1 && q <= r2) {
		while (p <= r1 && a[p] <= a[q]) { //为<= 
			t[index++] = a[p++];	
		}
		while (q <= r2 && a[q] <= a[p]) { //为<= 
			t[index++] = a[q++];	
		}
	}
	while (p <= r1) t[index++] = a[p++];
	while (q <= r2) t[index++] = a[q++];
	for (int i = 0; i < index; ++i)
		a[l1 + i] = t[i]; //将合并后的序列赋值回数组A 
}

void MergeSort(int a[], int s, int e) {
	if (s < e) { //至少还有两个元素 
		int mid = s + (e - s) / 2; 
		MergeSort(a, s, mid);
		MergeSort(a, mid + 1, e);
		Merge(a, s, mid, mid + 1, e, temp);
	}
}

int main() {
	cin >> n;
	for (int i = 0; i < n; ++i) 
		scanf("%d", &arr[i]); 
	MergeSort(arr, 0, n - 1);
	for (int i = 0; i < n - 1; ++i) 
		printf("%d ", arr[i]);
	printf("%d", arr[n - 1]); 
	return 0;
}
