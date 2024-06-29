- Problem Description
Every girl likes shopping,so does dandelion.Now she finds **the shop is increasing the price every day** because the Spring Festival is coming .She is fond of a shop which is called "memory". Now she wants to know the rank of this shop's price after the change of everyday.
- Input
One line contians a number n ( n<=10000),stands for the number of shops.
Then n lines ,each line contains a string (the length is short than 31 and only contains lowercase letters and capital letters.)stands for the name of the shop.
Then a line contians a number m (1<=m<=50),stands for the days .
Then m parts , every parts contians n lines , each line contians a number s and a string p ,stands for this day ,the shop p 's price has increased s.
- Output
Contains m lines ,In the ith line print a number of the shop "memory" 's rank after the ith day. We define the rank as :If there are t shops' price is higher than the "memory" , than its rank is t+1.
- Sample Input
	```
	3
	memory
	kfc
	wind
	2
	49 memory
	49 kfc
	48 wind
	80 kfc
	85 wind
	83 memory
	```
- Sample Output
	```
	1
	2
	```
题意：许多商店。这些商店每天的价格都会上涨，dandelion天天关注其中的一家商店“memory”的价格排名。

思路：模拟题意即可。关键在于留意到`the shop is increasing the price every day`。然后并不需要排序，因为题目只要求我们输出“memory”的价格排名。
```c
#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m, p; 
	string s;
	map<string, int> mp;
	while (cin >> n) {
		for (int i = 0; i < n; i++) cin >> s; //用不到 
		cin >> m;
		while (m--) {
			for (int i = 0; i < n; i++) {
				cin >> p >> s;
				mp[s] += p; //价格每天都在上涨 
			}
			int rank = 1;
			for (auto it : mp) 
				if (it.second > mp["memory"]) rank++;
			cout << rank << endl;
		}
		mp.clear();
	}
	return 0;
} 
```
