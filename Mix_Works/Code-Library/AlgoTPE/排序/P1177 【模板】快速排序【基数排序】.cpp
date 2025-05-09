#include <iostream>
#include <cstring>
using namespace std;
//基数排序可以说是计数排序的一个改进，对于最大数比较大，数据总数相对小的时候比较适用
//接下来说下基数排序的思路：这次我们要创建一个大小为10xN（N为待排数组的数据总个数）的二维数组，
//我们需要对待排数组里的数的每一位进行排序，然后放进相应位置，首先先根据个位进行排序，再根据十位排序，依次递增，排到最大数的位数，这时候排在我们创建的数组里的数就已经排好了，只需要输出就可以。
//这样表述可能有点看不懂，现在举个例子，现在我们要排一个数组，数据为{121,5,4564,9320,6,2,1,1234}
//首先，我们要找到数据中的最大数，这里也就是9321，然后得到它的位数，也就是4位，然后我们根据数据里的数的个位进行排序，
//首先第一个数121的个位是1，所以我们把这个数放在创建的数组的[1][0]位上，第二个数5个位是5，所以将它放在[5][1]上，以此类推，最后的结果是这样的根据个位排序结果
//现在我们将数字都根据个位排到了数组里，现在就是按顺序将他们读出来，从0开始读到9，从上读到下，这样就是按个位的从小到大排好的数组，重新覆盖到待排数组，
//现在的数组就是这样{9320,121,1,2,4564,1234,5,6}，当我们把数据读出来后记得要把我们创建的数组的数据清零，供下次使用
//这时候个位就按顺序排好了，接下来就开始排十位，这时候是要用我们刚刚读出来的新数组，第一位就是9320，它的十位是2，我们就把它放在[2][0]上，后面的跟排个位都是一样的
//，这里要注意第三个数是1，没有十位，我们就把它的十位当成0，把它排到[0][2]就可以了
//十位排完后的结果如图根据十位排序结果
//接下来再按之前的顺序读取数据，就得到了这样的数组{1,2,5,6,9320,121,1234,4564}，然后我们只要对百位和千位进行相同的排序，就可以得到最后排序好的数组了，
//这里附上百位和千位的排序结果根据百位排序结果根据千位排序结果
//可以看到，千位排完后再读取一遍就能得到排好的数组了
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
	for (int i = 0; i < n - 1; ++i)
		cout << arr[i] << " ";
	cout << arr[n - 1];
	return 0;	
} 
