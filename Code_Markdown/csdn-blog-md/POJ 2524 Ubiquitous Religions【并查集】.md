 <b><font face="Helvetica" color="blue" size="5">Description</font></b>
There are so many different religions in the world today that it is difficult to keep track of them all. You are interested in finding out how many different religions students in your university believe in.

You know that there are n students in your university (`0 < n <= 50000`). It is infeasible for you to ask every student their religious beliefs. Furthermore, many students are not comfortable expressing their beliefs. One way to avoid these problems is to ask m (`0 <= m <= n(n-1)/2`) pairs of students and ask them whether they believe in the same religion (e.g. they may know if they both attend the same church). From this data, you may not know what each person believes in, but you can get an idea of the upper bound of how many different religions can be possibly represented on campus. You may assume that each student subscribes to at most one religion.

 <b><font face="Helvetica" color="blue" size="5">Input</font></b>
The input consists of a number of cases. Each case starts with a line specifying the integers `n` and `m` . The next `m` lines each consists of two integers `i` and `j` , specifying that students `i` and `j` believe in the same religion. The students are numbered `1 to n` . The end of input is specified by a line in which `n = m = 0`.

 <b><font face="Helvetica" color="blue" size="5">Output</font></b>
For each test case, print on a single line the case number (starting with $1$ ) followed by the maximum number of different religions that the students in the university believe in.

 <b><font face="Helvetica" color="blue" size="5">Sample Input</font></b>

```cpp
10 9
1 2
1 3
1 4
1 5
1 6
1 7
1 8
1 9
1 10
10 4
2 3
4 5
4 8
5 8
0 0
```

 <b><font face="Helvetica" color="blue" size="5">Sample Output</font></b>

```cpp
Case 1: 1
Case 2: 7
```

 <b><font face="Helvetica" color="blue" size="5">Hint</font></b>
Huge input, scanf is recommended.

题意：求出全部的不同宗教的数量。
 
思路：并查集简单题。

代码：


```cpp
#include <cstdio> 
#include <iostream>
using namespace std;
const int MAXN = 50500;
int s[MAXN];
 
void initSet(int n) {
	for (int i = 1; i <= n; ++i) s[i] = -1;
}
int findSet(int x) {
	if (s[x] < 0) return x;
	else return s[x] = findSet(s[x]); 
}
void unionSet(int x, int y) { //按大小合并 
	if (s[x] < s[y]) {
		s[x] += s[y];
		s[y] = x;
	} else {
		s[y] += s[x];
		s[x] = y;
	}
}

int main() {
	int n, m, a, b, caseNo = 0;
	while (~scanf("%d%d", &n, &m)) {
		if (!n && !m) break;
		initSet(n);
		for (int i = 0; i < m; ++i) {
			cin >> a >> b;
			int x = findSet(a), y = findSet(b);
			if (x != y) unionSet(x, y);
		}
		int diffReligions = 0;
		for (int i = 1; i <= n; ++i) 
			if (s[i] < 0) ++diffReligions;
		printf("Case %d: %d\n", ++caseNo, diffReligions);
	}
	return 0;
}
```
<img src="https://img-blog.csdnimg.cn/20200609152855380.png" width="130%">
这里完全可以用朴素的写法，因为不需要每个集合的大小；还可以将 `find` 写成迭代形式，也可以迭代中压缩路径；另外，**直接在合并的时候减少可能存在的宗教数量**，能够提升一些程序速度。

emmm，测试的时候发现，上面的程序之所以慢，最大的问题在于输入时使用了 `cin` ……

```cpp
#include <cstdio> 
#include <iostream>
using namespace std;
const int MAXN = 50500;
int s[MAXN], count;
 
void initSet(int n) {
	for (int i = 1; i <= n; ++i) s[i] = i;
}
int findSet(int x) {
	while (s[x] != x) x = s[x];
	return x; 
}
void unionSet(int x, int y) { 
	--count; //每次合并, 减少一个宗教的数量 
	s[x] = y;
}

int main() {
	int n, m, a, b, caseNo = 0;
	while (~scanf("%d%d", &n, &m)) {
		if (!n && !m) break;
		initSet(n);
		count = n;
		for (int i = 0; i < m; ++i) {
			scanf("%d%d", &a, &b);
			int x = findSet(a), y = findSet(b);
			if (x != y) unionSet(x, y);
		} 
		printf("Case %d: %d\n", ++caseNo, count);
	}
	return 0;
}
```
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20200609153900904.png)

