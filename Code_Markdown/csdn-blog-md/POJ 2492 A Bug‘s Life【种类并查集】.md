**Description**
- Background
Professor Hopper is researching the sexual behavior of a rare species of bugs. He assumes that they feature two different genders and that they only interact with bugs of the opposite gender. In his experiment, individual bugs and their interactions were easy to identify, because numbers were printed on their backs.
- Problem
Given a list of bug interactions, decide whether the experiment supports his assumption of two genders with no homosexual bugs or if it contains some bug interactions that falsify it.

**Input**
The first line of the input contains the number of scenarios. Each scenario starts with one line giving the number of bugs (at least one, and up to `2000` ) and the number of interactions (up to `1000000` ) separated by a single space. In the following lines, each interaction is given in the form of two distinct bug numbers separated by a single space. Bugs are numbered consecutively starting from one.

**Output**
The output for every scenario is a line containing `"Scenario #i:"` , where i is the number of the scenario starting at `1` , followed by one line saying either `"No suspicious bugs found!"` if the experiment is consistent with his assumption about the bugs' sexual behavior, or `"Suspicious bugs found!"` if Professor Hopper's assumption is definitely wrong.

**Sample Input**
```cpp
2
3 3
1 2
2 3
1 3
4 2
1 2
3 4
```
**Sample Output**
```cpp
Scenario #1:
Suspicious bugs found!

Scenario #2:
No suspicious bugs found!

```
**Hint**
Huge input, scanf is recommended.

题意：教授研究虫子的性行为。他假定虫子的性行为只在异性之间发生。所有虫子按照 `1-N` 的顺序编号。然后给出虫子之间性行为的列表，判断教授的假设是否正确。

---
思路：种类并查集。开 `2 * N` 大小的并查集，`1~N` 属于性别 `A` ，`N+1~2N` 属于性别 `B` 。对于虫子之间的性行为，我们不知道某个虫子属于哪种特定的性别，于是都试一下，`merge(a, b + n); merge(a + n, b);` 。遇到属于同种性别的虫子进行性行为时，标记为 `true` ，表示教授的假设是错误的。具体代码如下：
```cpp
#include <iostream>
#include <cstdio> 
using namespace std;
const int maxn = 2100;
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
	int t, n, m, a, b;
	scanf("%d", &t);
	for (int i = 1; i <= t; ++i) {
		scanf("%d%d", &n, &m);	
		init(2 * n);
		bool suspicious = false; 
		for (int j = 0; j < m; ++j) {
			scanf("%d%d", &a, &b);
			if (query(a, b)) suspicious = true;
			else {
				merge(a, b + n);
				merge(a + n, b);
			}
		}
		if (suspicious) printf("Scenario #%d:\nSuspicious bugs found!\n\n", i);
		else printf("Scenario #%d:\nNo suspicious bugs found!\n\n", i);
	}
    return 0;
}
```
提交后AC：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200829152611646.png#pic_center)

