 
 <b><font face="Helvetica" color="blue" size="5">Description</font></b>
Severe acute respiratory syndrome (SARS), an atypical pneumonia of unknown aetiology, was recognized as a global threat in mid-March 2003. To minimize transmission to others, the best strategy is to separate the suspects from others.
In the Not-Spreading-Your-Sickness University (NSYSU), there are many student groups. Students in the same group intercommunicate with each other frequently, and a student may join several groups. To prevent the possible transmissions of SARS, the NSYSU collects the member lists of all student groups, and makes the following rule in their standard operation procedure (SOP).
Once a member in a group is a suspect, all members in the group are suspects.
However, they find that it is not easy to identify all the suspects when a student is recognized as a suspect. Your job is to write a program which finds all the suspects.

 <b><font face="Helvetica" color="blue" size="5">Input</font></b>
The input file contains several cases. Each test case begins with two integers `n` and `m` in a line, where `n` is the number of students, and `m` is the number of groups. You may assume that `0 < n <= 30000 and 0 <= m <= 500`. Every student is numbered by a unique integer between `0 and n−1` , and initially student `0` is recognized as a **suspect** in all the cases. This line is followed by `m ` member lists of the groups, one line per group. Each line begins with an integer `k` by itself representing the number of members in the group. Following the number of members, there are `k ` integers representing the students in this group. All the integers in a line are separated by at least one space.
A case with `n = 0 and m = 0` indicates the end of the input, and need not be processed.

 <b><font face="Helvetica" color="blue" size="5">Output</font></b>
For each case, output the number of suspects in one line.

 <b><font face="Helvetica" color="blue" size="5">Sample Input</font></b>

```cpp
100 4
2 1 2
5 10 13 11 12 14
2 0 1
2 99 2
200 2
1 5
5 1 2 3 4 5
1 0
0 0
```
 <b><font face="Helvetica" color="blue" size="5">Sample Output</font></b>
```cpp
4
1
1
```

题意：求出与 $0$ 号感染嫌疑人同组的人数。
 
思路：并查集简单题，不过需要注意的是，它不是一对对给出来的，而是一次性给出一组。我的写法是，每次将组内的元素 `i` 所在的集合和上次合并得到的集合进行 `union` 。由于要求出人数，因此采用了按大小合并的写法。

代码：
```cpp
#include <cstdio> 
#include <iostream>
using namespace std;
const int MAXN = 30500;
int s[MAXN];
 
void initSet(int n) {
	for (int i = 0; i < n; ++i) s[i] = -1;
}
int findSet(int x) {
	if (s[x] < 0) return x;
	else return s[x] = findSet(s[x]); 
}
int unionSet(int x, int y) { //按大小合并 //返回新集合的根节点 
	if (s[x] < s[y]) {
		s[x] += s[y];
		s[y] = x;
		return x;
	} else {
		s[y] += s[x];
		s[x] = y;
		return y;
	}
}

int main() {
	int n, m, k, fir, sec;
	while (~scanf("%d%d", &n, &m)) {
		if (!n && !m) break;
		initSet(n);
		for (int i = 0; i < m; ++i) {
			scanf("%d", &k);
			scanf("%d", &fir);
			int	x = findSet(fir); 
			for (int j = 1; j < k; ++j) {
				scanf("%d", &sec);
				int y = findSet(sec);
				if (x != y) x = unionSet(x, y);
			}
		}
		printf("%d\n", -s[findSet(0)]);
	}
	return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200609143456537.png)

