<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Problem Description**</font>
The French author Georges Perec (1936–1982) once wrote a book, La disparition, without the letter 'e'. He was a member of the Oulipo group. A quote from the book:
> Tout avait Pair normal, mais tout s’affirmait faux. Tout avait Fair normal, d’abord, puis surgissait l’inhumain, l’affolant. Il aurait voulu savoir où s’articulait l’association qui l’unissait au roman : stir son tapis, assaillant à tout instant son imagination, l’intuition d’un tabou, la vision d’un mal obscur, d’un quoi vacant, d’un non-dit : la vision, l’avision d’un oubli commandant tout, où s’abolissait la raison : tout avait l’air normal mais…

Perec would probably have scored high (or rather, low) in the following contest. People are asked to write a perhaps even meaningful text on some subject with as few occurrences of a given “word” as possible. Our task is to provide the jury with a program that counts these occurrences, in order to obtain a ranking of the competitors. These competitors often write very long texts with nonsense meaning; a sequence of 500,000 consecutive 'T's is not unusual. And they never use spaces.

So we want to quickly find out how often a word, i.e., a given string, occurs in a text. More formally: given the alphabet `{'A', 'B', 'C', …, 'Z'}` and two finite strings over that alphabet, a word `W` and a text `T` , count the number of occurrences of `W` in `T` . All the consecutive characters of `W` must exactly match consecutive characters of `T` . **Occurrences may overlap**.

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Input**</font>
The first line of the input file contains a single number: the number of test cases to follow. Each test case has the following format:
- One line with the word `W` , a string over `{'A', 'B', 'C', …, 'Z'}` , with `1 ≤ |W| ≤ 10,000` (here `|W|` denotes the length of the string `W` ).
- One line with the text `T` , a string over `{'A', 'B', 'C', …, 'Z'}` , with `|W| ≤ |T| ≤ 1,000,000` .

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Output**</font>
For every test case in the input file, the output should contain a single number, on a single line: the number of occurrences of the word `W` in the text `T` .

<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Sample Input**</font>
```cpp
3
BAPC
BAPC
AZA
AZAZAZA
VERDI
AVERDXIVYERDIAN
```
<font color="#7CA9ED"	height=5px font-size="5px" font-family="Arial" font-weight="bold">**Sample Output**</font>
```cpp
1
3
0
```

题意：我们的任务是：为陪审团提供一个计算这些单词发生次数的程序。参赛者通常会写很长的文章，而且他们从不使用空格。因此，我们想快速找出一个词在文本中出现的频率。更正式地说：给定字母表 `{'A'、'B'、'C'、…、'Z'}` 和两个有限字符串，一个单词 `W` 和一个文本 `T` ，计算 `W` 在 `T` 中出现的次数。`W` 的所有连续字符必须完全匹配 `T` 的连续字符。出现可能重叠。

---
思路：本题也可以套用KMP的模板。只是需要注意，单词的出现**可能重叠**。代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e6 + 10, minn = 1e4 + 10;
char s[maxn], p[minn];
int nextArr[minn], slen, plen;

namespace kmp1 {
	void getNext() {
		nextArr[0] = nextArr[1] = 0;
		for (int i = 1; i < plen; ++i) {
			int j = nextArr[i];
			while (j && p[i] != p[j]) j = nextArr[j];
			nextArr[i + 1] = (p[i] == p[j] ? j + 1 : 0);
		}
	}
	int kmp() {
		if (slen < plen) return 0;	
		getNext();
		int j = 0, ans = 0;
		for (int i = 0; i < slen; ++i) {
			while (j && s[i] != p[j]) j = nextArr[j]; 
			if (s[i] == p[j]) ++j;
			if (j >= plen) ++ans; //其后, s[i]!=p[j], j=nextArr[j]
		}
		return ans;
	}
}
namespace kmp2 {
	void getNext() {
		nextArr[0] = -1, nextArr[1] = 0;
		int pos = 2, cn = 0;
		while (pos <= plen) {
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
			else if (nextArr[j] != -1) j = nextArr[j];
			else ++i;
			if (j >= plen) {
				++ans;
				j = nextArr[j]; //注意这一点,不然会j<plen判断时退出循环
			}
		}
		return ans;
	}
}

int main() { 
	int t;
	scanf("%d", &t);
	while (t--) {
		scanf("%s%s", p, s);
		plen = strlen(p), slen = strlen(s);
		printf("%d\n", kmp2::kmp()); //kmp1::kmp()
	}
    return 0;
}
```
提交，结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200826225328355.png#pic_center)

