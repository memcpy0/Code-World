**Problem Description**
Every girl likes shopping, so does dandelion.Now she finds the shop is increasing the price every day because the Spring Festival is coming .She is fond of a shop which is called `"memory"` . Now she wants to know **the rank of this shop's price** after the change of everyday.
 

**Input**
One line contians a number `n(n <= 10000)` , stands for the number of shops.
Then `n` lines ,each line contains a string (the length is short than `31` and only contains lowercase letters and capital letters.) stands for the name of the shop.
Then a line contians a number `m(1<= m <= 50)` , stands for the days.
Then `m` parts , every parts contians `n` lines, each line contians a number `s` and a string `p` , stands for this day, the shop `p's` price has increased `s` .
 

**Output**
Contains `m` lines, In the `ith` line print a number of the shop `"memory"` 's rank after the `ith` day. We define the rank as: If there are `t` shops' price is higher than the `"memory"` , than its rank is `t+1` .
 
**Sample Input**
```cpp
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
**Sample Output**
```cpp
1
2
```


题意：给出 `n` 家店铺 `m` 天的价格增长，输出 `"memory"` 这家店每天的价格排名。

---
思路：用 `map` 容器操作。唯一需要注意的是，有多组测试数据。代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 40;
char str[maxn]; 

int main() { 
	int n, m, price;
	while (~scanf("%d", &n)) {
		unordered_map<string, int> shops;
		for (int i = 1; i <= n; ++i) //输入店铺名字,不用做处理 
			scanf("%s", str); 
		scanf("%d", &m);
		while (m--) {
			int rank = 1;
			for (int i = 1; i <= n; ++i) {
				scanf("%d%s", &price, str);
				shops[str] += price;   
			}
			for (const auto &it : shops)
				if (it.second > shops["memory"])
					++rank;
			printf("%d\n", rank);
		}
	}
    return 0;
}
```
另外，还会使用字符串哈希来做这一道题。
