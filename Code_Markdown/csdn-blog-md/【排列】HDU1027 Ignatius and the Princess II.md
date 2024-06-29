- Problem Description
Now our hero finds the door to the BEelzebub feng5166. He opens the door and finds feng5166 is about to kill our pretty Princess. But now the BEelzebub has to beat our hero first. feng5166 says, "I have three question for you, if you can work them out, I will release the Princess, or you will be my dinner, too." Ignatius says confidently, "OK, at last, I will save the Princess."

	"Now I will show you the first problem." feng5166 says, "Given a sequence of number 1 to N, we define that 1,2,3...N-1,N is the smallest sequence among all the sequence which can be composed with number 1 to N(each number can be and should be use only once in this problem). So it's easy to see the second smallest sequence is 1,2,3...N,N-1. Now I will give you two numbers, N and M. You should tell me the Mth smallest sequence which is composed with number 1 to N. It's easy, isn't is? Hahahahaha......"
	Can you help Ignatius to solve this problem?
- Input
The input contains several test cases. Each test case consists of two numbers, N and M(1<=N<=1000, 1<=M<=10000). You may assume that there is always a sequence satisfied the BEelzebub's demand. The input is terminated by the end of file.
- Output
For each test case, you only have to output the sequence satisfied the BEelzebub's demand. When output a sequence, you should print a space between two numbers, but do not output any spaces after the last number.
- Sample Input
	```
	6 4
	11 8
	```
 - Sample Output
	```
	1 2 3 5 6 4
	1 2 3 4 5 6 7 9 8 11 10
	```
题意：别西卜抓走了公主，伊格那修要拯救公主，别西卜就要求伊格纳修对给定的n个数字，从1到n，输出第m小的序列。
思路：首先生成一个123...n的最小字典序列，然后用next_permutation即可。
```c
#include <bits/stdc++.h>
using namespace std;
int main() {
	int n, m;
	while (cin >> n >> m) {
		int a[n + 1], num = 1;;
		for (int i = 1; i <= n; i++) a[i] = i;
		do {
			if (num == m) break;
			num++;
		} while (next_permutation(a + 1, a + n + 1));
		for (int i = 1; i < n; i++) 
			cout << a[i] << " ";
		cout << a[n] << endl;
	}
	return 0;
}
```

