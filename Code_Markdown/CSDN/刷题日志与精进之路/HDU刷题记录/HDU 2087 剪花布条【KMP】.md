
<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Problem Description**</font>
一块花布条，里面有些图案，另有一块直接可用的小饰条，里面也有一些图案。对于给定的花布条和小饰条，计算一下能**从花布条中尽可能剪出几块小饰条**来呢？

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Input**</font>
输入中含有一些数据，分别是**成对出现的花布条和小饰条**，其布条都是用可见ASCII字符表示的，可见的ASCII字符有多少个，布条的花纹也有多少种花样。花纹条和小饰条不会超过 `1000` 个字符长。如果遇见 `#` 字符，则不再进行工作。
 

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Output**</font>
输出能从花纹布中剪出的**最多小饰条个数**，如果一块都没有，那就老老实实输出 `0` ，每个结果之间应换行。

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Sample Input**</font>
```cpp
abcde a3
aaaaaa  aa
#
```
<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Sample Output**</font>
```cpp
0
3
```

题意：找到可能剪出的最多的小饰条数目。

---
思路：本题可以完全套用KMP。找到的匹配可能有很多个，而且可能重合，本题中需要找到能够分开的子串，即剪出不同的小饰条。这个问题很简单，直接在一次完全匹配后，让 `j = 0` 匹配永不回溯的 `i` 的新位置即可。

代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
char s[maxn], p[maxn];
int nextArr[maxn]; 
int slen, plen;

namespace kmp1 {
	void getNext() { //预先计算nextArr[],用于在失配的情况下得到回溯的位置 
		nextArr[0] = 0, nextArr[1] = 0;
		for (int i = 1; i < plen; ++i) {
			int j = nextArr[i];
			while (j && p[i] != p[j]) j = nextArr[j];
			nextArr[i + 1] = (p[i] == p[j] ? j + 1 : 0);
		}
	}
	int kmp() {											//在s中找到p 
		if (slen < plen) return 0;	
		getNext();
		int j = 0, ans = 0; //last=-1;					//last指向上次匹配的末尾位置 
		for (int i = 0; i < slen; ++i) {		 		//匹配s和p的每个字符 
			while (j && s[i] != p[j]) j = nextArr[j];	//失配后用nextArr[]找j的回溯位置 
			if (s[i] == p[j]) ++j;						//当前位置的字符匹配,继续 
			if (j >= plen) {							//完全匹配 
				++ans;
				j = 0;
			}
			/*
			if (i - last >= plen) {						//判断新的匹配和上个匹配能否分开 
				++ans;		
				last = i;
			}
			*/
		}
		return ans;
	}
}

namespace kmp2 {
	void getNext() {
		nextArr[0] = -1, nextArr[1] = 0;
		int pos = 2, cn = 0;
		while (pos < plen) {
			if (p[pos - 1] == p[cn]) nextArr[pos++] = ++cn;
			else if (cn > 0) cn = nextArr[cn];		
			else nextArr[pos++] = cn;
		}
	}
	int kmp() {
		if (slen < plen) return 0;
		getNext();
		int i = 0, j = 0, ans = 0;
		while (i < slen && j < plen) {
			if (s[i] == p[j]) ++i, ++j;
			else if (nextArr[j] != -1) j = nextArr[j];	//j != 0
			else ++i;	
			//else if (nextArr[j] == -1) ++i;
			//else j = nextArr[j];
			if (j >= plen) {	//匹配完了一次 
				++ans;
				j = 0;
			}
		}
		return ans;
	}
} 

int main() { 
	while (~scanf("%s", s)) {
		if (s[0] == '#') break;
		scanf("%s", p);
		slen = strlen(s), plen = strlen(p);
		//printf("%d\n", kmp1::kmp());
		printf("%d\n", kmp2::kmp());
	}
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200826183715496.png#pic_center)

