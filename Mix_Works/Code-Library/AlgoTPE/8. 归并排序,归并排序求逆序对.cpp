#include <bits/stdc++.h>
using namespace std;

/*分治的三步法:
划分问题：把序列分成元素个数尽量相等的两半。
递归求解：把两半元素分别排序。
合并问题：把两个有序表合并成一个。

关键在于如何把两个有序表合成一个。
每次只需要把两个序列的最小元素加以比较，删除其中的较小元素并加入合并后的新表即可。
由于需要一个新表来存放结果，所以附加空间为n.

归并排序的时间复杂度和最大连续和的分治算法一样，都是O(nlogn)的
*/
void mergeSort(int *A, int x, int y, int *T) { //归并排序左闭右开区间[x,y)
	if (y - x > 1) { //多于一个元素
        int m = x + (y - x) / 2; //划分
        mergeSort(A, x, m, T);   //递归求解
        mergeSort(A, m, y, T);   //递归求解
        int p = x, q = m, i = x;
        while (p < m || q < y) { //右开区间，只要有一个序列非空，就要继续合并
            //精妙！！！
			//(q完成了&&p没有完成)||(q没有完成&&p没有完成&&A[p]<=A[q]
            if (q >= y || (p < m && A[p] <= A[q])) T[i++] = A[p++]; //从i开始 //从左半数组复制到临时空间
			//q没有完成&&(p完成了||A[p]>A[q])
			else T[i++] = A[q++]; //从右半数组复制到临时空间
		}
		for (int i = x; i < y; ++i) A[i] = T[i]; //从辅助空间复制回A数组
	}
}

/*逆序对问题。给一列数a1, a2, … , an，求它的逆序对数，即有多少个有序对(i,j)，
使得i<j但a_i>a_j。n可以高达10^6
1.n这么大，O(n^2)的枚举将超时，因此需要寻找更高效的方法
2.受到分治求最大连续和、分治归并排序的影响，试试“分治三步法”是否适用
- “划分问题”过程是把序列分成元素个数尽量相等的两半；
- “递归求解”是统计i和j均在左边或者均在右边的逆序对个数；
- “合并问题”则是统计i在左边，但j在右边的逆序对个数。

关键在于合并：如何求出i在左边，而j在右边的逆序对数目呢？

按照j的不同把这些“跨越两边”的逆序对进行分类：
只要对于右边的每个j，统计左边比它大的元素个数f(j)，则所有f(j)之和便是答案。
归并排序可以“顺便”完成f(j)的计算：
由于合并操作是从小到大进行的，当右边的A[j]复制到T中时，左边还没来得及复制到T的那些数就是左边所有比A[j]大的数。此
时在累加器中加上左边元素个数m-p即可（左边所剩的元素在区间[p,m)中，因此元素个数为m-p）
*/
int cnt = 0;
void mergeSortAndCount(int *A, int x, int y, int *T) { //[x,y)
	if (y - x > 1) {
		int m = x + (y - x) / 2;
		mergeSortAndCount(A, x, m, T);
		mergeSortAndCount(A, m, y, T);
		int p = x, q = m, i = x;
		while (p < m || q < y) {
			if (q >= y || (p < m && A[p] <= A[q])) T[i++] = A[p++];
			else {
				T[i++] = A[q++];
				cnt += m - p; //左边还没来得及复制到T的那些数就是左边所有比A[j]大的数。
				//此时在累加器中加上左边元素个数m-p即可
			}
		}
		for (int i = x; i < y; ++i) A[i] = T[i];
	}
}
//其他写法
void mergeSortAndCount2(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAndCount2(A, l, m, T);     //递归求解
		mergeSortAndCount2(A, m + 1, r, T); //递归求解
		int i = l, j = m + 1, k = i;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			//i<j && A[i]>A[j]
			else { T[k++] = A[j++]; cnt += j - k; } //mid - i + 1
			//当右边的A[j]复制到T中时，左边还没来得及复制到T的那些数就是左边所有比A[j]大的数
			//此时在累加器中加上左边元素个数m-i+1即可
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = i; x <= r; ++x) A[x] = T[x];
	}
}

void mergeSortAndCount3(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAndCount3(A, l, m, T);
		mergeSortAndCount3(A, m + 1, r, T);
		int i, j = m + 1, k = i;
		
		//按照i的不同把这些“跨越两边”的逆序对进行分类：
		//只要对于左边的每个i，统计右边比它小的元素个数f(j)，则所有f(j)之和便是答案。
		//归并排序可以“顺便”完成f(j)的计算
		for (i = l; i <= m; ++i) {
			while (j <= r && A[i] > A[j]) ++j; //直到A[i]<=A[j]
			cnt += j - mid; //A[i]>所有[mid,j)的数A[j]
		}
		//按照j的不同把这些“跨越两边”的逆序对进行分类：
		//只要对于右边的每个j，统计左边比它大的元素个数f(j)，则所有f(j)之和便是答案。
		//归并排序可以“顺便”完成f(j)的计算
		for (j = m + 1; j <= r; ++j) {
			while (i <= m && A[i] <= A[j]) ++i; //直到A[i]>A[j]
			cnt += mid - i + 1; //[mid,i]中的A[i]都>=A[j]
		}
		i = l, j = m + 1;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			else T[k++] = A[j++];
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = i; x <= r; ++x) A[x] = T[x];
	}
}

int main() {

	return 0;
}
