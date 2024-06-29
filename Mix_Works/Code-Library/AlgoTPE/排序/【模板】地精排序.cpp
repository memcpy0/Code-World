//nth_element(A + 1, A + k, A + n + 1);
//逆序对是满足i < j且A_i > A_j 的数对个数,冒泡排序的交换此次数等于逆序对个数 
//地精排序等价于插入排序 
//void gnomeSort(int arr[], int n) {
//	int i = 0;
//	while (i < n) {
//		if (i == 0 || arr[i - 1] <= arr[i]) ++i;
//		else swap(arr[i], arr[i - 1]), --i;
//	}
//}
