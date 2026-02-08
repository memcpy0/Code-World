### HDU1062 Text Reverse
- Problem Description
Ignatius likes to write words in reverse way. Given a single line of text which is written by Ignatius, you should reverse all the words and then output them.
- Input
The input contains several test cases. The first line of the input is a single integer `T` which is the number of test cases. `T` test cases follow.
Each test case contains a single line with several words. There will be at most `1000` characters in a line.
- Output
For each test case, you should output the text which is processed.
- Sample Input
	```
	3
	olleh !dlrow
	m'I morf .udh
	I ekil .mca
	```
- Sample Output
	```
	hello world!
	I'm from hdu.
	I like acm.
	```
题意：一个人喜欢反着写单词，你需要把他写的一行字符串中的所有单词反转过来并输出。

---
思路1：读入一行字符串，然后遇到空格，翻转前面的单词即可。

代码：
```c
#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	scanf("%d\n", &n); 
	string s;
	while (n--) {
        getline(cin, s);
        int i = 0; 
        for (int j = 0; j < s.size(); j++) {
        	if (s[j] == ' ') { //把空字符前面的单词反转 
        		reverse(s.begin() + i, s.begin() + j);
        		i = j + 1;
			} else if (j == s.size() - 1) 
				reverse(s.begin() + i, s.end());
		}
		cout << s << endl;
	} 
	return 0;
}
```
---

思路2：用栈的做法，将一个单词的各个字符推入栈中，遇到空格后弹出前面的字符。
```c
#include <bits/stdc++.h>
using namespace std;
int main() {
	int n;
	scanf("%d\n", &n); 
	while (n--) {
	    stack<char> s;
	    char c;
		while ((c = getchar()) != '\n') {
			if (c == ' ') { //一个单词全部推入栈中
				while (!s.empty()) {
					putchar(s.top()); s.pop();
				}
				printf(" ");
				continue;
			}
			s.push(c);
		}
		while (!s.empty()) { //最后一个单词
			putchar(s.top()); s.pop();
		}
		putchar('\n');
	} 
	return 0;
}
```
