<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Problem Description**</font>
Given two sequences of numbers : a[1], a[2], ...... , a[N], and b[1], b[2], ...... , b[M] (1 <= M <= 10000, 1 <= N <= 1000000). Your task is to find a number K which make a[K] = b[1], a[K + 1] = b[2], ...... , a[K + M - 1] = b[M]. If there are more than one K exist, output the smallest one.
 

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Input**</font>
The first line of input is a number T which indicate the number of cases. Each case contains three lines. The first line is two numbers N and M (1 <= M <= 10000, 1 <= N <= 1000000). The second line contains N integers which indicate a[1], a[2], ...... , a[N]. The third line contains M integers which indicate b[1], b[2], ...... , b[M]. All integers are in the range of [-1000000, 1000000].
 

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Output**</font>
For each test case, you should output one line which only contain K described above. If no such K exists, output -1 instead.

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Sample Input**</font>
```cpp
2
13 5
1 2 1 2 3 1 2 3 1 3 2 1 2
1 2 3 1 3
13 5
1 2 1 2 3 1 2 3 1 3 2 1 2
1 2 3 2 1
```
<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Sample Output**</font>
```cpp
6
-1
```
题意：找出 `b` 数组在 `a` 数组中出现的第一个位置。

---
思路：KMP板子题。代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int smaxn = 1000000 + 10, pmaxn = 10000 + 10;
int s[smaxn], p[pmaxn];
int nextArr[pmaxn];
int cases, n, m;

namespace kmp1 {
	void getNext() {
		nextArr[0] = nextArr[1] = 0;
		for (int i = 1; i < m; ++i) {
			int j = nextArr[i];
			while (j && p[i] != p[j]) j = nextArr[j];
			nextArr[i + 1] = (p[i] == p[j] ? j + 1 : 0);
		}
	}
	int kmp() {
		if (n < m) return -1;
		getNext();
		int j = 0;
		for (int i = 0; i < n; ++i) {
			while (j && s[i] != p[j]) j = nextArr[j];
			if (s[i] == p[j]) ++j;
			if (j >= m) return i + 2 - j;
		}
		return -1;
	}
}
namespace kmp2 {
	void getNext() {
		nextArr[0] = -1, nextArr[1] = 0;
		int pos = 2, cn = 0;
		while (pos <= m) {
			if (p[pos - 1] == p[cn]) nextArr[pos++] = ++cn;
			else if (cn > 0) cn = nextArr[cn];
			else nextArr[pos++] = cn;
		}
	}
	int kmp() {
		if (n < m) return -1;
		getNext();
		int i = 0, j = 0;
		while (i < n && j < m) {
			if (s[i] == p[j]) ++i, ++j;
			else if (nextArr[j] != -1) j = nextArr[j];
			else ++i;
			if (j >= m) return i + 1 - j;
		}
		return -1;
	}
}

int main() {
	scanf("%d", &cases);
	while (cases--) {
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; ++i) scanf("%d", &s[i]);
		for (int i = 0; i < m; ++i) scanf("%d", &p[i]);
		printf("%d\n", kmp1::kmp());
	}
    return 0;
}
```
提交结果如下：
 
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020082700574251.png#pic_center)

