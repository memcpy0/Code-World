@[toc]


---
# 1. Arithmetic Progression of Primes (20 分)
In mathematics, an arithmetic progression (AP，等差数列) is a sequence of numbers such that the difference between the consecutive terms is constant. In 2004, Terence Tao (陶哲轩) and Ben Green proved that for any positive $n$, there exists at least one arithmetic progression consists of $n$ consecutive prime numbers. For example, { 7，37，67，97，127，157 } is one solution for $n=6$. Now it is your job to find a maximum solution for a given $n$ within a given range.

All the numbers in a line must be separated by a space, and there must be no extra space at the beginning or the end of the line.
 
<h2>Input Specification:</h2>

Each input file contains one test case, which gives two positive integers in a line: $n\ (≤10)$, the number of consecutive prime terms in an arithmetic progression, and $MAXP\ (2≤MAXP<10^5)$, the upper bound of the largest prime in the solution.

<h2>Output Specification:</h2>

For each test case, if there exists a solution, print in ascending order the prime numbers in a line. If the solution is not unique, output the one with the maximum common difference. If there is still a tie, print the one with the maximum first number. If there is no solution bounded by $MAXP$, output the largest prime in the given range instead.

<h2>Sample Input 1:</h2>

```cpp
5 1000
```

<h2>Sample Output 1:</h2>

```cpp
23 263 503 743 983
```

<h2>Sample Input 2:</h2>

```cpp
10 200
```
<h2>Sample Output 2:</h2>

```cpp
199
```

## 题意
对于给定的正整数 $n$ ，在给定的上限 $MAXP$ 内，找出一个由 $n$ 个素数组成的等差数列。如果存在解，则按照升序，输出这列素数；解不唯一时，则输出拥有最大公差的解；如果仍不唯一，则输出数列首项最大的解。如果在 $MAXP$ 范围内无解，则输出这一给定范围内的最大素数。

## 解法 暴力+剪枝
陶哲轩和 `Ben Green` 证明的定理见[Green–Tao theorem](https://en.wikipedia.org/wiki/Green%E2%80%93Tao_theorem)。这一题一看就非常麻烦，而且可以说是这四道题中最难得分的，很多网上通过的代码也有问题。可能的**坑点**在于，明白这一题的解法是**暴力+剪枝**，没有其他更有效的方法（？）。

因此，我们先**标记**和**收集**数据范围内所有的素数，再分类讨论各种情况如下：
- 当 $n = 1$ 时，结果显然是 $MAXP$ 范围内最大的素数，和无解时一致。
- 当 $n > 1$ 时，外层循环依次遍历收集到的所有素数，以当前的素数 `primes[i]` 为全素数等差数列的首元素；第二层循环从已知的最大等差 `curDiff` 开始，不断递增 `curDiff` ；最内层循环验证，是否存在以 `primes[i]` 开头、以 `curDiff` 为等差、长度为 $n$ 的全素数等差数列，存在时则获得了更有效的解——此时要么**有更大的首部元素**，要么**有更大的公差**。

复杂度比较高，但是稍微剪枝一下，就能通过了。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 10;
int n, maxp;
bitset<maxn> inp; // is not prime
vector<int> primes;
void getPrimes() {
	inp[0] = inp[1] = 1;
	for (int i = 2; i <= maxp; ++i) {
		if (inp[i] == 0)
			primes.push_back(i);
			for (int j = i + i; j <= maxp; j += i)
				inp[j] = 1;
	}
}
int main() {
	cin >> n >> maxp;
	getPrimes(); // 求出所有范围内的素数
	if (n == 1) { // 数列只有1个数时,卡的一个两分的测试点
		cout << primes.back();
		return 0;
	}
	vector<int> tmp, ans;
	int maxDiff = -1; // 依次遍历每个质数,以该质数为起点找符合条件的公差
	for (int i = 0; i < primes.size(); ++i) {
		int curDiff = (maxDiff == -1) ? 1 : maxDiff; // 剪枝
		int x = primes[i];
		while (true) {
			if (x + (n - 1) * curDiff > maxp) break; // 剪枝
			tmp.clear();
			int k = x;
			tmp.push_back(x);
			for (int j = 1; j < n; ++j) {
				k = k + curDiff;
				if (inp[k] == 1) break;
				tmp.push_back(k);
			}
			if (tmp.size() == n) {
				ans = tmp;
				maxDiff = curDiff;
			}
			++curDiff;
		}
	}
	if(maxDiff == -1) {
		printf("%d\n", primes.back()); 
		return 0;
	}
	for (int i = 0; i < ans.size(); ++i) printf(" %d" + !i, ans[i]);
	return 0;	
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/73dcc28c1ae141879c5626cecb3b5ecf.png)


---
# 2. Lab Access Scheduling (25 分)
Nowadays, we have to keep a safe social distance to stop the spread of virus due to the COVID-19 outbreak. Consequently, the access to a national lab is highly restricted. Everyone has to submit a request for lab use in advance and is only allowed to enter after the request has been approved. Now given all the personal requests for the next day, you are supposed to make a feasible plan with the maximum possible number of requests approved. It is required that at most one person can stay in the lab at any particular time.
 
<h2>Input Specification:</h2>

Each input file contains one test case. Each case starts with a positive integer $N$ $(≤2×10^3)$, the number of lab access requests. Then $N$ lines follow, each gives a request in the format:
```cpp
hh:mm:ss hh:mm:ss
```
where `hh:mm:ss` represents the time point in a day by hour:minute:second, with the earliest time being `00:00:00` and the latest `23:59:59`. For each request, the two time points are the requested entrance and exit time, respectively. It is guaranteed that the exit time is after the entrance time.

Note that all times will be within a single day. Times are recorded using a 24-hour clock.

<h2>Output Specification:</h2>
 
The output is supposed to give the total number of requests approved in your plan.

<h2>Sample Input:</h2>

```cpp
7
18:00:01 23:07:01
04:09:59 11:30:08
11:35:50 13:00:00
23:45:00 23:55:50
13:00:00 17:11:22
06:30:50 11:42:01
17:30:00 23:50:00
```

<h2>Sample Output:</h2>

```cpp
5
```
<h2>Hint:</h2>

All the requests can be approved except the last two.
## 题意
为了阻断疫情传播，需要受限进入实验室。每个人必须提前一天提交使用实验室的申请，只有请求通过后，才能在第二天进入实验室。在一个特定的时间点，最多只有一个人能待在实验室。现在给出所有的申请，你的工作是制定一个可行的计划，让最多的人使用实验室。

实验室的使用请求是一个开始时间和结束时间的元组，时间以24小时 `hh:mm:ss`  的形式给出。所有的时间都在一天之内。
## 解法 排序+区间贪心
这道题很简单，就是做过了很多次的区间贪心。先排序，让最早结束的请求排在最前面；再设置已知结束时间为 `curEnd` ，初始化为 $0$ ，接着从头遍历请求数组，如果当前请求的开始时间大于等于已知的结束时间 `curEnd` ，则通过的请求数量加一，同时更新已知的结束时间为当前请求的结束时间。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 2e3 + 10;
int n, h1, m1, s1, h2, m2, s2;
struct node {
	int bt, et; // begin time, end time;
} reqs[maxn];
int secs(int h, int m, int s) {
	return h * 60 * 60 + m * 60 + s;	
}
int main() {
	cin >> n;
	for (int i = 0; i < n; ++i) {
		scanf("%d:%d:%d %d:%d:%d", &h1, &m1, &s1, &h2, &m2, &s2);
		reqs[i].bt = secs(h1, m1, s1);
		reqs[i].et = secs(h2, m2, s2);	
	}
	sort(reqs, reqs + n, [&](const node &a, const node &b) {
		return a.et != b.et ? a.et < b.et : a.bt < b.bt;
	});
	int cnt = 0, curEnd = 0;
	for (int i = 0; i < n; ++i) {
		if (reqs[i].bt >= curEnd) {
			++cnt;
			curEnd = reqs[i].et;
		}
	}
	cout << cnt;
	return 0;	
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/3a8161232b6b421b95df4977df33c34d.png)


---
# 3. Structure of Max-Heap (25 分)

In computer science, a **max-heap** is a specialized tree-based data structure that satisfies the heap property: if $P$ is a parent node of $C$, then the key (the value) of $P$ is greater than or equal to the key of $C$. A common implementation of a heap is the binary heap, in which the tree is a complete binary tree.

Your job is to first insert a given sequence of integers into an initially empty max-heap, then to judge if a given description of the resulting heap structure is correct or not. There are 5 different kinds of description statements:
- `x is the root`
- `x and y are siblings`
- `x is the parent of y`
- `x is the left child of y`
- `x is the right child of y`

<h2>Input Specification:</h2>

Each input file contains one test case. For each case, the first line gives 2 positive integers: $N$ $(≤1,000)$, the number of keys to be inserted, and $M$ $(≤20)$, the number of statements to be judged. Then the next line contains $N$ distinct integer keys in $[−10^4,10^4]$ which are supposed to be inserted into an initially empty max-heap. Finally there are $M$ lines of statements, each occupies a line.

<h2>Output Specification:</h2>

For each statement, print `1` if it is true, or `0` if not. All the answers must be print in one line, without any space.

<h2>Sample Input:</h2>

```cpp
5 6
23 46 26 35 88
35 is the root
46 and 26 are siblings
88 is the parent of 46
35 is the left child of 26
35 is the right child of 46
-1 is the root
```

<h2>Sample Output:</h2>
 

```cpp
011010
```

## 题意
题意很简单，先插入一个给定的整数序列，到一个初始为空的最大堆中。针对这个已有的最大堆，给出一系列命题—— `x` 是根节点、`x` 和 `y` 是兄弟结点、`x` 是 `y` 的父亲/左孩子/右孩子节点，判断它们真或假。
## 解法 建堆+字符串处理+哈希表
这道题不难，只是处理起来比较繁琐。先要写一个入堆函数，由于元素都是唯一的，为了后续处理高效简单，要用哈希表，存储每个元素在堆中的位置。然后针对不同的命题处理输入的字符串，得到 `x` 和 `y` 的值，再从哈希表中取出它们在堆中的位置 `px, py` ，判断是否满足命题，是则输出 `1` ，否则输出 `0` 。
```cpp
#include <bits/stdc++.h>
#define father(i) ((i) / 2)
#define left(i) ((i) * 2)
#define right(i) (((i) * 2) + 1)
using namespace std;
const int maxn = 1010;
int n, m, val, x, y;
int h[maxn], len = 0;
unordered_map<int, int> rec;
string str;
void push(int v) {
	h[++len] = v;
	int cur = len;
	for (int i = father(cur); i && h[i] < v; cur = i, i = father(cur))
		h[cur] = h[i];
	h[cur] = v;
}
vector<string> split(const string &s, char delim = ' ') { // 按照空格分割字符串s为单词序列
	if (s.empty()) return {};
	size_t beginPos = s.find_first_not_of(delim, 0);
	size_t endPos = s.find_first_of(delim, beginPos);
	vector<string> ans;
	while (beginPos != string::npos) {
		ans.push_back(s.substr(beginPos, endPos - beginPos));
		beginPos = s.find_first_not_of(delim, endPos);
		endPos = s.find_first_of(delim, beginPos);
	}
	return ans;
}
int main() {
	cin >> n >> m;
	for (int i = 0; i < n; ++i) {
		cin >> val;
		push(val);
	}
	for (int i = 1; i <= n; ++i) rec[h[i]] = i;	
	while (m--) {
		cin >> x;
		int px = rec[x];
		getline(cin, str);
		if (!px) {
			printf("0");
			continue;
		}
		vector<string> &&tokens = split(str);
		bool flag = false;
		if (str.back() == 't') { // 是否根节点
			if (h[1] == x) flag = true;
		} 
		else if (str.back() == 's') {	// 兄弟节点
			int y = stoi(tokens[1]), py = rec[y];
			if (py && px != py && father(px) == father(py)) flag = true;
		}
		else if (tokens[2] == "parent") { // 父节点
			int y = stoi(tokens.back()), py = rec[y];
			int fa = father(py);
			if (py && fa > 0 && fa <= n && fa == px) flag = true;
		} 
		else if (tokens[2] == "left") { // 左节点
			int y = stoi(tokens.back()), py = rec[y];
			int l = left(py);
			if (py && l <= n && l == px) flag = true; 
		} 
		else if (tokens[2] == "right") { // 右节点
			int y = stoi(tokens.back()), py = rec[y];
			int r = right(py);
			if (py && r <= n && r == px) flag = true;
		}
		printf(flag ? "1" : "0");
	}
	return 0;	
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/f70e9bd49a3248f9868893436caa60ed.png)


---
# 4. Recycling of Shared Bicycles (30 分)

There are many spots for parking the shared bicycles in Hangzhou. When some of the bicycles are broken, the management center will receive a message for sending a truck to collect them. Now given the map of city, you are supposed to program the collecting route for the truck. The strategy is a simple greedy method: the truck will always move to the nearest spot to collect the broken bicycles. If there are more than one nearest spot, take the one with the smallest index.
 
<h2>Input Specification:</h2>

Each input file contains one test case. For each case, the first line contains two positive integers: $N$ $(≤ 200)$, the number of spots (hence the spots are numbered from $1$ to $N$, and the management center is always numbered $0$), and $M$, the number of streets connecting those spots. Then $M$ lines follow, describing the streets in the format:

```cpp
S1 S2 Dist
```

where `S1` and `S2` are the spots at the two ends of a street, and `Dist` is the distance between them, which is a positive integer no more than 1000. It is guaranteed that each street is given once and `S1` is never the same as `S2`.

<h2>Output Specification:</h2>

For each case, first print in a line the sequence of spots in the visiting order, starting from 0. If it is impossible to collect all the broken bicycles, output in the second line those spots that cannot be visited, in ascending order of their indices. Or if the job can be done perfectly, print in the second line the total moving distance of the truck.

All the numbers in a line must be separated by 1 space, and there must be no extra space at the beginning or the end of the line.

<h2>Sample Input 1 (shown by the figure below):</h2>

```cpp
7 10
0 2 1
0 4 5
0 7 3
0 6 4
0 5 5
1 2 2
1 7 2
2 3 4
3 4 2
6 7 9
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/ffb22c66a0914c648fb70345eeea6fac.png)
<h2>Sample Output 1:</h2>

```cpp
0 2 1 7 6 3 4 5
33
```

<h2>Sample Input 2:</h2>

```cpp
7 8
0 2 1
0 4 5
0 7 3
1 2 2
1 7 2
2 3 4
3 4 2
6 5 1
```
<h2>Sample Output 2:</h2>

```cpp
0 2 1 7 3 4
5 6
```

## 题意
杭州有许多地方用来停放共享单车，当有些单车被损坏时，管理中心将收到一个信息，并用一辆卡车收集这些单车。现在，给出城市的地图，你需要编程计算卡车收集这些单车的路线，策略是简单的贪心：卡车总是移动到最近的地点，收集其中的破单车；如果最近地点多于一个，则选取编号最小的一个地点。

首先按照地点访问顺序，打印一个地点序列。如果卡车能收集全部地点的所有破单车，则输出卡车移动的总距离，否则输出不能访问的那些地点。
## 解法 最短路+DFS
这一题其实很简单，**坑点**在于正确理解卡车移动的贪心策略——**卡车总是移动到距离当前地点最近的下一个地点**，中途可能经过其他已经访问的地点，但是不会输出这些已经访问的地点；==中途不可能经过未访问的地点，因为这样一来，下个目的地点就不是最近的地点了==。因此，我们要用全源最短路算法Floyd-Warshall，计算出所有地点到其他所有地点的最短距离。

然后从管理中心的位置（顶点 `0` ），递归运行贪心策略，移动到下个最近地点，直到所有地点已经被访问或无法访问其他地点，同时计算移动的总距离。
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 210;
const int inf = 0x3f3f3f3f;
int n, m, s1, s2, d;
int g[maxn][maxn];
bool vis[maxn];
void floyd() {
	for (int k = 0; k <= n; ++k) {
		for (int i = 0; i <= n; ++i) {
			if (g[i][k] != inf) {
				for (int j = 0; j <= n; ++j) {
					if (g[k][j] != inf)
						g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
				}
			}
		}
	}
}
int pathDist = 0;
vector<int> path;
void dfs(int u) {
	vis[u] = true;
	path.push_back(u);
	int v = -1, minDist = inf;
	for (int i = 0; i <= n; ++i) {
		if (!vis[i] && g[u][i] < minDist) {
			v = i;
			minDist = g[u][i];
		}
	}
	if (v == -1 || minDist == inf) return;
	pathDist += minDist;
	dfs(v);
}
int main() {
	cin >> n >> m;
	memset(g, 0x3f, sizeof(g));
// 	for (int i = 0; i <= n; ++i) for (int j = 0; j <= n; ++j) if (i == j) g[i][j] = 0;
	for (int i = 0; i < m; ++i) {
		cin >> s1 >> s2 >> d;
		g[s1][s2] = g[s2][s1] = d;
	}
	floyd();
	dfs(0);
	for (int i = 0; i < path.size(); ++i) printf(" %d" + !i, path[i]);
	cout << endl;
	if (path.size() == n + 1) { // 收集完所有破单车
		cout << pathDist;
	} else {
		vector<int> ans;
		for (int i = 0; i <= n; ++i)
			if (!vis[i]) ans.push_back(i);
		for (int i = 0; i < ans.size(); ++i) printf(" %d" + !i, ans[i]);
	}
	return 0;	
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/e61eac682fdc4d6983f69527d0062f5d.png)

