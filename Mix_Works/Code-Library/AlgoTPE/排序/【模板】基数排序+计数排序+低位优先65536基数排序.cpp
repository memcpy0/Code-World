#include <iostream>
#include <cstring>
using namespace std;


//计数排序
int cnt[MAXN];
void countingSort(unsigned A[], int len) { //A[i]>=0 
	for (int i = 0; i < len; ++i) ++cnt[A[i]];
	int pos = 0;
	for (int i = 0; pos < len; ++i) 
		if (cnt[i])
			for (int j = 0; j < cnt[i]; ++j)
				A[pos++] = i;
}
int cnt[MAXN];
void keyIndexCountSort(unsigned A[], int len) {
	unsigned *B = new unsigned[len];
	for (int i = 0; i < len; ++i) ++cnt[A[i]]; 
	for (int i = 1; i < maxn; ++i) cnt[i] += cnt[i - 1];
	for (int i = 0; i < len; ++i) //对于A中的数,按照索引表给出的位置放置
		B[--cnt[A[i]]] = A[i]; 
	memcpy(A, B, sizeof(B)); 
}
void keyIndexCountSort(unsigned A[], int len) { //A[i]>=0
	unsigned *B = new unsigned[len];
	for (int i = 0; i < len; ++i) ++cnt[A[i] + 1]; 
	for (int i = 1; i < maxn; ++i) cnt[i] += cnt[i - 1];
	for (int i = 0; i < len; ++i) //对于A中的数,按照索引表给出的位置放置
		B[cnt[A[i]]++] = A[i]; //正排 
	memcpy(A, B, sizeof(B)); 
}
/*
基数排序可以说是计数排序的一个改进，对于最大数比较大，数据总数相对小的时候比较适用
接下来说下基数排序的思路：这次我们要创建一个大小为10xN（N为待排数组的数据总个数）的二维数组，
我们需要对待排数组里的数的每一位进行排序，然后放进相应位置，首先先根据个位进行排序，再根据十位排序，依次递增，排到最大数的位数，
这时候排在我们创建的数组里的数就已经排好了，只需要输出就可以。
这样表述可能有点看不懂，现在举个例子，现在我们要排一个数组，数据为{121,5,4564,9320,6,2,1,1234}

首先，我们要找到数据中的最大数，这里也就是9321，然后得到它的位数，也就是4位，然后我们根据数据里的数的个位进行排序，
首先第一个数121的个位是1，所以我们把这个数放在创建的数组的[1][0]位上，第二个数5个位是5，所以将它放在[5][1]上，以此类推，最后的结果是这样的根据个位排序结果

现在我们将数字都根据个位排到了数组里，现在就是按顺序将他们读出来，从0开始读到9，从上读到下，这样就是按个位的从小到大排好的数组，重新覆盖到待排数组，
现在的数组就是这样{9320,121,1,2,4564,1234,5,6}，当我们把数据读出来后记得要把我们创建的数组的数据清零，供下次使用

这时候个位就按顺序排好了，接下来就开始排十位，这时候是要用我们刚刚读出来的新数组，第一位就是9320，它的十位是2，我们就把它放在[2][0]上，后面的跟排个位都是一样的
，这里要注意第三个数是1，没有十位，我们就把它的十位当成0，把它排到[0][2]就可以了
十位排完后的结果如图根据十位排序结果

接下来再按之前的顺序读取数据，就得到了这样的数组{1,2,5,6,9320,121,1234,4564}，然后我们只要对百位和千位进行相同的排序，就可以得到最后排序好的数组了，
这里附上百位和千位的排序结果根据百位排序结果根据千位排序结果

可以看到，千位排完后再读取一遍就能得到排好的数组了
*/
int idx[10][100001], arr[100001], n, digits = 0, mx = -1; 
int main() {
//	freopen("C:\\Users\\dell\\Desktop\\P1177_4.in", "r", stdin);
//	freopen("C:\\Users\\dell\\Desktop\\pp.out.txt", "w", stdout); 
	cin >> n; 
	for (int i = 0; i < n; ++i) {
		cin >> arr[i];
		mx = (mx > arr[i] ? mx : arr[i]);
	}
	do {
		++digits;
		mx /= 10;
	} while (mx); //得到最大数的位数
	int ten = 1;
	for (int i = 0; i < digits; ++i) { //进行digits次排序, 从个位排到最高位
		for (int j = 0; j < n; ++j) //将每个数按照给出的j的顺序 排到创建的数组中
			idx[(arr[j] / ten) % 10][j] = arr[j];
		int x = 0; 
		for (int k = 0; k < 10; ++k) //按照顺序读取排好的数组
			for (int l = 0; l < n; ++l) 
				if (idx[k][l]) 
					arr[x++] = idx[k][l];	
		memset(idx, 0, sizeof(idx)); //将创建的数组清0 
		ten *= 10;  //ten*10表示下一次循环排更高一位
	}
	for (int i = 0; i < n; ++i) printf(" %d" + !i, arr[i]);
	return 0;	
} 
vector<int> idx(10); 
int arr[100001], n, digits = 0, mx = -1; 
int main() { 
	cin >> n; 
	for (int i = 0; i < n; ++i) {
		cin >> arr[i];
		mx = (mx > arr[i] ? mx : arr[i]);
	}
	do { ++digits; mx /= 10; } while (mx); //得到最大数的位数
	int ten = 1;
	for (int i = 0; i < digits; ++i) { //进行digits次排序, 从个位排到最高位
		for (int j = 0; j < n; ++j) //将每个数按照给出的j的顺序 排到创建的数组中
			idx[(arr[j] / ten) % 10].push_back(arr[j]);
		int x = 0; 
		for (int k = 0; k < 10; ++k) {//按照顺序读取排好的数组
			for (int l = 0; l < idx[k].size(); ++l)  
				arr[x++] = idx[k][l];
			idx[k].clear(); //将创建的数组清0 
		}
		ten *= 10;  //ten*10表示下一次循环排更高一位
	}
	for (int i = 0; i < n; ++i) printf(" %d" + !i, arr[i]);
	return 0;	
} 
//高位优先的基数排序
//低位优先的基数排序
//给一个长度为n的int数组排序 n < 10^7
//线性排序
//log_d Ai (n + d)
//取d = 2^16, 这样基数排序只用跑两次就可以了,伪线性算法,卡常很好
inline void radixSort(unsigned A[], int len) {
	unsigned *B = new unsigned[len];
	int r = 65535; //基数为65535
	int L[r + 1] = {0}, H[r + 1] = {0}; //分别对低16位和高16位计数
	for (int i = 0; i < len; ++i) 
		++L[A[i] & r], ++H[(A[i] >> 16) & r); //计数排序
	for (int i = 1; i <= r; ++i) 
		L[i] += L[i - 1], H[i] += H[i - 1]; //求前缀和
	for (int i = len - 1; i >= 0; --i) //for (int i = 0; i <= len - 1; ++i) 
		B[--L[A[i] & r]] = A[i];	//对低位进行计数排序
	for (int i = len - 1; i >= 0; --i)
		A[--H[(B[i] >> 16) & r]] = B[i];	//对高位进行计数排序
	delete [] B;
}

inline void radix_sort(unsigned A[], int len) {
	unsigned *B = new unsigned[len];
	int r = 255;
	int L1[r + 1] = {0}, L2[r + 1] = {0}, H1[r + 1] = {0}, H2[r + 1] = {0}; //H2H1L2L1
	for (int i = 0; i < len; ++i) {
		++L1[A[i] & r];
		++L2[(A[i] >> 8) & r];
		++H1[(A[i] >> 16) & r];
		++H2[(A[i] >> 24) & r];
	}
	for (int i = 1; i <= r; ++i) {
		L1[i] += L1[i - 1];
		L2[i] += L2[i - 1];
		H1[i] += H1[i - 1];
		H2[i] += H2[i - 1];
	}
	for (int i = len - 1; i >= 0; --i) 
		B[--L1[A[i] & r]] = A[i];
	for (int i = len - 1; i >= 0; --i)
		A[--L2[(B[i] >> 8) & r]] = B[i];
	for (int i = len - 1; i >= 0; --i)
		B[--H1[(A[i] >> 16) & r]] = A[i];
	for (int i = len - 1; i >= 0; --i)
		A[--H2[(B[i] >> 24) & r]] = B[i];
	delete [] B;
}
//浮点数进行基数排序 
#include <bits/stdc++.h>
inline void sort(float *ar, int len)
{
    unsigned  *a=(unsigned *)ar;
    unsigned *BUF = (unsigned *)malloc(sizeof(unsigned ) * (len+1)),*b=BUF+1;
    *(float *)BUF=1.0; // guard
    int sum[256] = {0}, sum1[256] = {0}, sum2[256] = {0}, sum3[256] = {0};
    for (int i = 0; i < len; i++)
    {
        ++sum[a[i] & 255];
        ++sum1[(a[i] >> 8) & 255];
        ++sum2[(a[i] >> 16) & 255];
        ++sum3[a[i] >> 24];
    }
    for (int q = 1; q <= 255; ++q)
    {
        sum[q] += sum[q - 1];
        sum1[q] += sum1[q - 1];
        sum2[q] += sum2[q - 1];
        sum3[q] += sum3[q - 1];
    }
    for (int q = len - 1; q >= 0; --q)
        b[--sum[a[q] & 255]] = a[q];
    for (int q = len - 1; q >= 0; --q)
        a[--sum1[(b[q] >> 8) & 255]] = b[q];
    for (int q = len - 1; q >= 0; --q)
        b[--sum2[(a[q] >> 16) & 255]] = a[q];
    for (int q = len - 1; q >= 0; --q)
        a[--sum3[b[q] >> 24]] = b[q];
    memcpy(b,a,sizeof(unsigned)*len);
    int beg=len-1,q = beg;
    for (;b[q]& 0x80000000 ; --q)
        a[beg-q]=b[q];
    memcpy(a+beg-q,b,sizeof(unsigned)*(q+1));
    free(BUF);
    return;
}
int main(void)
{
    int len=100000000;
    std::default_random_engine e(time(NULL));//这里设置seed
    std::uniform_real_distribution<float> rnd(-1e20, 1e20);
    float *ar=(float *)malloc(sizeof(float)*len);
    for (int i=0;i<len;++i)
        ar[i]=rnd(e);
    float *ar2=(float *)malloc(sizeof(float)*len);
    memcpy(ar2,ar,sizeof(float)*len);
    clock_t beg=clock();
    std::sort(ar,ar+len);
    printf("std::sort: %f\n",(double)(clock()-beg)/CLOCKS_PER_SEC);
    beg=clock();
    sort(ar2,len);
    printf("mysort: %f\n",(double)(clock()-beg)/CLOCKS_PER_SEC);
    for (int i=0;i<len;++i)
    {
        if (ar[i]!=ar2[i])
        {
            puts("ERR");
            break;
        }
    }
    free(ar);
    free(ar2);
    return 0;
}
//后缀排序,给出一个长度为n的字符串,给它的所有后缀按照字典序排序 n <= 10^5
//假设已经按照每个后缀的前k位排好序了
//考虑将k扩展到2k,把每个后缀的前2k位用数对(a,b)代表, a是前k位的排名,
//b是第k+1到2k位的排名
//对(a,b)进行基数排序
