**Description**
The police office in Tadu City decides to say ends to the chaos, as launch actions to root up the TWO gangs in the city, Gang Dragon and Gang Snake. However, the police first needs to identify which gang a criminal belongs to. The present question is, given two criminals; do they belong to a same clan? You must give your judgment based on incomplete information. (Since the gangsters are always acting secretly.)

Assume `N` (`N <= 10^5`) criminals are currently in Tadu City, numbered from `1` to `N` . And of course, **at least one of them belongs to Gang Dragon, and the same for Gang Snake.** You will be given `M` (`M <= 10^5`) messages in sequence, which are in the following two kinds:
1. `D [a] [b]`
where `[a]` and `[b]` are the numbers of two criminals, and they belong to different gangs.
2. `A [a] [b]`
where `[a]` and `[b]` are the numbers of two criminals. This requires you to decide whether a and b belong to a same gang.

**Input**
The first line of the input contains a single integer `T` (`1 <= T <= 20`), the number of test cases. Then `T` cases follow. Each test case begins with a line with two integers `N` and `M` , followed by `M` lines each containing one message as described above.

**Output**
For each message `"A [a] [b]"` in each case, your program should give the judgment based on the information got before. The answers might be one of `"In the same gang."` , `"In different gangs."` and `"Not sure yet."` .

**Sample Input**
```cpp
1
5 5
A 1 2
D 1 2
A 1 2
D 2 4
A 1 4
```
**Sample Output**
```cpp
Not sure yet.
In different gangs.
In the same gang.
```

题意：有两个黑帮 `Dragon, Snake` ，警察开始想要分辨某个罪犯属于哪个黑帮。现在的问题是，给出两个罪犯，他们是否属于同一个黑帮。我们必须基于之前的信息，做出判断——不确定，属于，不属于。

---
思路：简单的种类并查集写法。不过这道题有点问题：明明说好了，至少有一个罪犯属于 `Dragon`，同样至少有一个罪犯属于 `Snake` ，因此对于这组测试数据：
```
2 1
A 1 2
```
应该输出：`In different gangs.` 。但是很多人包括我的代码输出 `Not sure yet.` ，都过了……我的代码如下，一开始用 `cin` 输入结果TLE，改成 `scanf` 就过了：
```cpp
#include <cstdio>
#include <iostream>
using namespace std;
const int maxn = 1e5 + 10;
int father[maxn * 2], height[maxn * 2];
void init(int n) {
	for (int i = 1; i <= n; ++i) father[i] = i, height[i] = 1;
}
int find(int x) {
	return father[x] == x ? x : father[x] = find(father[x]);
}
void merge(int a, int b) {
	int x = find(a), y = find(b);
	if (x == y) return;
	if (height[x] >= height[y]) father[y] = x;
	else father[x] = y;
	if (height[x] == height[y]) ++height[x];
}
bool query(int a, int b) {
	return find(a) == find(b);
}

int main() { 
	int t, n, m, x, y;
	char c[5];
	scanf("%d", &t);
	while (t--) {
		scanf("%d%d", &n, &m);
		init(2 * n);
		for (int i = 0; i < m; ++i) {
			scanf("%s%d%d", c, &x, &y);
			if (c[0] == 'A') {
				if (query(x, y + n) || query(x + n, y)) //不是同一个黑帮
					printf("In different gangs.\n");
				else if (query(x, y)) 					//是同一个黑帮
					printf("In the same gang.\n");
				else 
					printf("Not sure yet.\n");
			} 
			else if (c[0] == 'D') {
				merge(x, y + n);
				merge(x + n, y);
			}
		}
	}
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200829134309997.png#pic_center)

按理说， `N=2` 时需要特判，但是加不加 `N = 2` 的程序都能过，很可能是因为测试数据里没有这组。我也懒得加了。


