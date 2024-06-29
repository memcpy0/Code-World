#include <cstdio> 
#define Swap(a, b) {int t = a; a = b; b = t;}
#define cutOff 5  
const int maxn = 101001;
int n, arr[maxn];
/*��������������ͨ���ڲ������㷨,��Թ鲢������˵�����ٶȸ��죬���Ҳ��踨���ռ�
���շ����������������������㷨�����½��ܣ�
- �������⣺������ĸ���Ԫ�����ź�ֳ����������֣�ʹ����ߵ�����Ԫ�ض�С�ڻ�����ұߵ�����Ԫ��
- �ݹ���⣺�����������ֱַ�����
- �ϲ����⣺���úϲ�����Ϊ��ʱ�����Ѿ���ȫ����

�����������Ͳ���ֻ��һ��ʵ�ַ����������ֹ��̡��ж����ͬ�İ汾�����¿�������Ҳ�в�ͬ�汾
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
int Median3(int a[], int left, int right) { //�ҵ���ֵ�е� 
	int center = (left + right) / 2;
	if (a[left] > a[center]) Swap(a[left], a[center]);
	if (a[left] > a[right]) Swap(a[left], a[right]);
	if (a[center] > a[right]) Swap(a[center], a[right]);
	//a[left] <= a[center] <= a[right]
	Swap(a[center], a[right - 1]); //���ص�pivot, ������a[left]<=a[right-1]<=a[right]
	return a[right - 1];
}

void QuickSort(int a[], int l, int r) {
	int i, j, pivot;
	if (l + cutOff <= r) {
		pivot = Median3(a, l, r);
		i = l, j = r - 1; //a[i]<=a[j] ����˵ a[i]<=pivot	
		while (true) {
			while (a[++i] < pivot) {}
			while (a[--j] > pivot) {}
			if (i < j) {
				Swap(a[i], a[j]);
			}
			else break;
		}
		Swap(a[i], a[r - 1]); //��pivot��������ȷ��λ�� 
		
		QSort(a, l, i - 1); //a[i]�ڵĵط�Ϊ��ŦԪ 
		QSort(a, i + 1, r);
	} else //��С������������������ 
		insertionSort(a, l, r);
}
/*����ѡ�����⡣����n��������һ��������k��1��k��n���������Щ������С���������ĵ�k��
�����磬k=1������Сֵ����n��10^7
ѡ���k��������������뵽�ķ�����������Ȼ��ֱ������±�Ϊk-1��Ԫ�أ�������C�����������±��0��ʼ����
��10^7�Ĺ�ģ��ʹ����O(nlogn)���㷨��˵�ϴ�
�����ڿ�������ġ����֡�����������A[p��r]���ֳ���A[p��q]��A[q+1��r]������Ը�����ߵ�Ԫ�ظ���q-p+1
��k�Ĵ�С��ϵֻ����߻����ұߵݹ���⡣����֤���������������£������ʱ�临�Ӷ�ΪO(n)��
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
