**Problem  Description**
Today is Ignatius' birthday. He invites a lot of friends. Now it's dinner time. Ignatius wants to know **how many tables he needs at least**. You have to notice that not all the friends know each other, and all the friends do not want to stay with strangers.

One important rule for this problem is that if I tell you  `A`  knows  `B` , and  `B`  knows  `C` , that means  `A` ,  `B` ,  `C`  know each other, so they can stay in one table.

For example: If I tell you  `A`  knows  `B` ,  `B`  knows  `C` , and  `D`  knows  `E` , so  `A` ,  `B` ,  `C`  can stay in one table, and  `D` ,  `E`  have to stay in the other one. So Ignatius needs $2$ tables at least.
 
**Input**
The input starts with an integer `T(1<=T<=25) ` which indicate the number of test cases. Then `T` test cases follow.  Each test case starts with two integers `N` and `M(1<=N,M<=1000)`. `N` indicates the number of friends, the friends are marked from `1 ` to `N`. Then `M` lines follow.  Each line consists of two integers  `A`  and  `B` ( `A` != `B` ), that means friend  `A`  and friend  `B`  know each other. There will be a blank line between two cases.
 

**Output**
For each test case, just output how many tables Ignatius needs at least.  Do NOT print any blanks.
 

**Sample Input**
```cpp
2
5 3
1 2
2 3
4 5

5 1
2 5
```
**Sample Output**
```cpp
2
4
```
题意：需要多少张桌子，容纳不同的朋友群体。

思路：并查集的板子题，朴素的写法就可以过。

代码：
```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> father;
int findSet(int x) {
	if (father[x] == x) return x;
	else return findSet(father[x]);
}
void unionSet(int r1, int r2) {
	father[r1] = r2;
}

int main() {
	int T;
	scanf("%d", &T);
	while (T--) {
		int n, m, a, b, tableNum = 0;
		scanf("%d%d", &n, &m);
		father.resize(n + 1);
		for (int i = 1; i <= n; ++i) father[i] = i; 
		while (m--) {
			scanf("%d%d", &a, &b);
			int r1 = findSet(a), r2 = findSet(b);
			if (r1 != r2) unionSet(r1, r2);
		}
		for (int i = 1; i <= n; ++i) 
			if (father[i] == i)
				++tableNum;
		printf("%d\n", tableNum);
		father.clear();
	}
	return 0;
}
```



