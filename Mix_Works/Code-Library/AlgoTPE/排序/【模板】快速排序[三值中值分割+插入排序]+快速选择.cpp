#include <cstdio> 
#define Swap(a, b) {int t = a; a = b; b = t;}
#define cutOff 5  
const int maxn = 101001;
int n, arr[maxn];
/*快速排序是最快的通用内部排序算法,相对归并排序来说不仅速度更快，并且不需辅助空间
按照分治三步法，将快速排序算法作如下介绍：
- 划分问题：把数组的各个元素重排后分成左右两部分，使得左边的任意元素都小于或等于右边的任意元素
- 递归求解：把左右两部分分别排序。
- 合并问题：不用合并，因为此时数组已经完全有序。

快速排序本来就不是只有一种实现方法。“划分过程”有多个不同的版本，导致快速排序也有不同版本
*/
//----------------------------------------------------------------------------------
void insertionSort(int a[], int l, int r) {
	for (int i = l + 1; i <= r; ++i) {
		int t = a[i], j;
		for (j = i; j > 0 && a[j - 1] > t; --j)
		 	a[j] = a[j - 1];
		a[j] = t;
	}
} 
int Median3(int a[], int left, int right) { //找到三值中点 
	int center = (left + right) / 2;
	if (a[left] > a[center]) Swap(a[left], a[center]);
	if (a[left] > a[right]) Swap(a[left], a[right]);
	if (a[center] > a[right]) Swap(a[center], a[right]);
	//a[left] <= a[center] <= a[right]
	Swap(a[center], a[right - 1]); //隐藏的pivot, 这下有a[left]<=a[right-1]<=a[right]
	return a[right - 1];
}

void QuickSort(int a[], int l, int r) {
	int i, j, pivot;
	if (l + cutOff <= r) {
		pivot = Median3(a, l, r);
		i = l, j = r - 1; //a[i]<=a[j] 或者说 a[i]<=pivot	
		while (true) {
			while (a[++i] < pivot) {}
			while (a[--j] > pivot) {}
			if (i < j) {
				Swap(a[i], a[j]);
			}
			else break;
		}
		Swap(a[i], a[r - 1]); //将pivot交换到正确的位置 
		
		QSort(a, l, i - 1); //a[i]在的地方为枢纽元 
		QSort(a, i + 1, r);
	} else //在小数组上面做插入排序 
		insertionSort(a, l, r);
}
/*快速选择问题。输入n个整数和一个正整数k（1≤k≤n），输出这些整数从小到大排序后的第k个
（例如，k=1就是最小值）。n≤10^7
选择第k大的数，最容易想到的方法是先排序，然后直接输出下标为k-1的元素（别忘了C语言中数组下标从0开始），
但10^7的规模即使对于O(nlogn)的算法来说较大
假设在快速排序的“划分”结束后，数组A[p…r]被分成了A[p…q]和A[q+1…r]，则可以根据左边的元素个数q-p+1
和k的大小关系只在左边或者右边递归求解。可以证明，在期望意义下，程序的时间复杂度为O(n)。
*/
//----------------------------------------------------------------------------------
int main() {  
	scanf("%d", &n);
	for (int i = 0; i < n; ++i) 
		scanf("%d", &arr[i]);
	QuickSort(arr, 0, n - 1);
	for (int i = 0; i < n; ++i) {
		if (i != 0) printf(" "); 
		printf("%d", arr[i]); 
	}  
 	return 0;
}  
