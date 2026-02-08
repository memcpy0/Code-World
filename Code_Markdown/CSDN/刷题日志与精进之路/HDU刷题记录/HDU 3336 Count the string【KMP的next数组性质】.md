题目链接：[HDU 3336 Count the string](http://acm.hdu.edu.cn/showproblem.php?pid=3336)

---
**Problem Description**
It is well known that AekdyCoin is good at string problems as well as number theory problems. When given a string `s` , we can write down all the **non-empty prefixes** of this string. 

For example: `s: "abab"`. The prefixes are: `"a", "ab", "aba", "abab"`.

For each prefix, we can count the times it matches in `s` . So we can see that prefix `"a"` matches twice, `"ab"` matches twice too, `"aba"` matches once, and `"abab"` matches once. Now you are asked to calculate the sum of the match times for all the prefixes. For `"abab"` , it is `2 + 2 + 1 + 1 = 6` .

The answer may be very large, so output the answer mod `10007` .
 
**Input**
The first line is a single integer `T` , indicating the number of test cases.
For each case, the first line is an integer `n` (`1 <= n <= 200000`), which is the length of string `s` . A line follows giving the string `s` . The characters in the strings are all lower-case letters.
 

**Output**
For each case, output only one number: the sum of the match times for all the prefixes of `s` mod `10007` .
 

**Sample Input**
```cpp
1
4
abab
```
**Sample Output**
```cpp
6
```

题意：求出字符串中每个非空前缀在串中的出现次数的总和。

---
思路：利用KMP算法的 `next[]` 数组性质。在 `【算法学习】字符串 KMP算法、next数组性质` 这篇文章中，已经将整个过程讲得很清楚了。代码如下：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 200010;
char s[maxn];
int nextArr[maxn], len;

/*
void getNext() {
	nextArr[0] = nextArr[1] = 0;
	int pos = 2, cn = 0;
	while (pos <= len) {
		if (s[pos - 1] == s[cn]) nextArr[pos++] = ++cn;
		else if (cn > 0) cn = nextArr[cn];
		else nextArr[pos++] = cn;
	}
}
*/
void getNext() {
	nextArr[0] = 0, nextArr[1] = 0;
	for (int i = 1; i < len; ++i) {
		int j = nextArr[i];
		while (j && s[i] != s[j]) j = nextArr[j];
		nextArr[i + 1] = (s[i] == s[j] ? j + 1 : 0);
	}
}

int main() { 
	int t;
	scanf("%d", &t);
	while (t--) {
		scanf("%d%s", &len, s);	
		long long prefixNum = len; //最开始可以确定的非空前缀个数(串长度) 
		getNext();
		prefixNum += nextArr[len]; //先包含:最后的nextArr值 
		for (int i = 0; i < len; ++i) 
			if (nextArr[i] > 0 && nextArr[i] + 1 != nextArr[i + 1])
				prefixNum += nextArr[i]; //当不满足递推规律时+next值
			printf("%lld\n", prefixNum % 10007); 	 
	}
	return 0;
}
```
提交后：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200831002518922.png#pic_center)

