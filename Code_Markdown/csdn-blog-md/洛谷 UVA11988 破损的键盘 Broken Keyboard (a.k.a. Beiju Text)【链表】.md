**题目描述**
 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210521200045499.png)

**输入格式**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210521200051928.png)

**输出格式**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210521200058349.png)

**题意翻译** 
你在输入文章的时候，键盘上的Home键和End键出了问题，会不定时的按下。你却不知道此问题，而是专心致志地打稿子，甚至显示器都没开。当你打开显示器之后，展现你面前的数一段悲剧文本。你的任务是在显示器打开前计算出这段悲剧的文本。 给你一段按键的文本，其中'['表示Home键，']'表示End键，输入结束标志是文件结束符（EOF）。

输出一行，即这段悲剧文本。 翻译贡献者UID：71371

**输入输出样例**
输入 #1 
```clike
This_is_a_[Beiju]_text
[[]][][]Happy_Birthday_to_Tsinghua_University
```

输出 #1

```clike
BeijuThis_is_a__text
Happy_Birthday_to_Tsinghua_University
```

---
### 解法 链表
由于本题中需要快速的插入，因此不能使用数组，于是使用链表。下面的代码使用了STL中的 `list`（双向链表）：
```cpp
#include <bits/stdc++.h>
using namespace std;
string s;
list<char> ls; 
int main() {
	while (getline(cin, s)) { 
		list<char>::iterator it = ls.begin();
		for (const char &c : s) {
			if (c == '[') it = ls.begin(); //光标回到首位
			else if (c == ']') it = ls.end(); //光标回到末位 
			else { it = ls.insert(it, c); ++it; } //在光标位置插入字符,光标指向原本的位置 
		}
		for (auto it = ls.begin(); it != ls.end(); ++it) printf("%c", *it);
		printf("\n"); 
		s.clear();
		ls.clear();
	}
	return 0;
} 
```
如果STL的 `list` 使用体验不是很好，也可以手写单链表：
```cpp
#include <bits/stdc++.h>
using namespace std;
string s;
struct node {
	node *next;
	char c;
	node(char ch, node *ne = nullptr) : c(ch), next(ne) {}
};
int main() {
	while (getline(cin, s)) {
		node head(0), *rear = &head, *temp = rear;
		for (const char &c : s) {
			if (c == '[') rear = &head;
			else if (c == ']') rear = temp;
			else {
				node *t = new node(c, rear->next);
				rear->next = t;
				rear = t; 
				if (rear->next == nullptr) temp = rear; //更新末尾指针
			}
		}
		for (node *cur = head.next; cur; cur = cur->next) printf("%c", cur->c);
		printf("\n");
	}
	return 0;
}
```
或者使用数组模拟链表：
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 100;
string s;
struct node {
	char c;
	int next = 0;
} ls[maxn];
int tot = 0;
int newNode(char c, int ne = 0) {
	ls[++tot].c = c;
	ls[tot].next = ne;
	return tot;
}
int main() {
	while (getline(cin, s)) {
		tot = 0;
		for (int i = 0; i < maxn; ++i) ls[i].next = 0; //注意初始化 
		int head = newNode(0), rear = head, temp = head;
		for (const char &c : s) {
			if (c == '[') rear = head; 
			else if (c == ']') rear = temp;
			else {
				int t = newNode(c, ls[rear].next);
				ls[rear].next = t;
				rear = t; 
				if (ls[rear].next == 0) temp = rear; //更新末尾指针
			}
		}
		for (int cur = ls[head].next; cur; cur = ls[cur].next) printf("%c", ls[cur].c);
		printf("\n"); 
	}
	return 0;
} 
```
提交后结果如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210521200440461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

