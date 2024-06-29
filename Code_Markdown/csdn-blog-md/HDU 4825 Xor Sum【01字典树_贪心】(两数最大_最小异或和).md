**Problem Description**
`Zeus` 和 `Prometheus` 做了一个游戏，`Prometheus` 给 `Zeus` 一个集合，集合中包含了 `N` 个正整数，随后 `Prometheus` 将向 `Zeus` 发起`M` 次询问，每次询问中包含一个正整数 `S` ，之后 `Zeus` 需要在集合当中找出一个正整数 `K` ，使得 `K` 与 `S` 的异或结果最大。`Prometheus` 为了让 `Zeus` 看到人类的伟大，随即同意 `Zeus` 可以向人类求助。你能证明人类的智慧么？
 

**Input**
输入包含若干组测试数据，每组测试数据包含若干行。
输入的第一行是一个整数 `T`（`T < 10`），表示共有 `T` 组数据。
每组数据的第一行输入两个正整数 `N, M`（`1<=N,M<=100000`），接下来一行，包含 `N` 个正整数，代表 `Zeus` 的获得的集合，之后 `M` 行，每行一个正整数 `S` ，代表 `Prometheus` 询问的正整数。所有正整数均不超过 `2^32` 。
 

**Output**
对于每组数据，首先需要输出单独一行 `Case #?:` ，其中问号处应填入当前的数据组数，组数从 `1` 开始计算。对于每个询问，输出一个正整数 `K` ，使得 `K` 与 `S` 异或值最大。
 

**Sample Input**
```cpp
2
3 2
3 4 5
1
5
4 1
4 6 5 6
3
```
**Sample Output**
```cpp
Case #1:
4
3
Case #2:
4
```

题意：有最多 `9` 组数据，每组数据包含最多 `100000` 个正整数，以及最多 `100000` 个询问。对于每个询问 `S` ，在集合当中找出一个正整数 `K` ，使得 `K` 与 `S` 的异或结果最大。

----
思路：暴力算法毫无希望。本题最优的解法是使用01字典树。做法是：将数字转换为二进制01串，补成一样的长度，然后**从高位到低位**像普通字典树一样存储。之后，就可以**贪心**地解决这个问题——要找与给定的数 `S` **异或最大**的数 `K` ，就应该**从高位到低位**、尽可能走与数 `S` **当前位不同**的路径。反之则尽可能走与**当前位相同**的路径。

如果从低位到高位存储数字，然后从低位到高位走与 `S` 当前位不同的路径，不能得到最大的异或值，(⊙o⊙)…比如：
```css
			  #
		   /     \
		  0       1 
		   \	   \ 
		 	1       1
			 \	   / \ 
	  		  1   0   1
	         (4) (3) (5)
S: 5(寻找和5异或值最大的数字)
从低位到高位走不同的路径, 最终得到的是4, 而不是正解3
5 ^ 4 = 1
5 ^ 3 = 4
```
知道了原理后，就可以很容易地解决这道题目了。代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int MAXNODE = 3300010, MAXBITS = 32; //每个数都补成33位 
int trie[MAXNODE][2], pos;
ll num[MAXNODE];	//记录每个插入字典树的数 

void init() {
	memset(trie, 0, sizeof(trie));
	pos = 1;	
}

void insert(ll a) { //正整数<=2^32 
	int cur = 0;
	for (int i = MAXBITS; i >= 0; --i) {
		int bit = (a >> i) & 1; //求出当前位并插入
		if (trie[cur][bit] == 0) 
			trie[cur][bit] = pos++;
		cur = trie[cur][bit];
	}
	num[cur] = a;
}
ll findXorMax(ll a) { //找到与a异或最大的那个数
	int cur = 0;
	for (int i = MAXBITS; i >= 0; --i) {
		int bit = (a >> i) & 1;
		if (trie[cur][bit ^ 1] != 0) //优先走与当前位不同的路径
			cur = trie[cur][bit ^ 1];
		else  
			cur = trie[cur][bit];
	}
	return num[cur];
}

int main() {
	int t, n, m;
	ll a;
	scanf("%d", &t);
	for (int cases = 1; cases <= t; ++cases) {
		init();
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; ++i) {
			scanf("%lld", &a);
			insert(a);
		}
		printf("Case #%d:\n", cases);
		for (int i = 1; i <= m; ++i) {
			scanf("%lld", &a);
			printf("%lld\n", findXorMax(a)); 
		}
	}
    return 0;
}
```
虽然说起来简单，但是这道题我还是提交了好几次的。一是数据范围 `<= 2^32` ，会爆 `int` ；二是要补成 `33` 位的二进制数；三是有多组测试用例，要记得每次重新初始化字典树。提交后AC：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200831133020357.png#pic_center)

