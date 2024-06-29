@[toc]
这是CodeUp刷题的第二部分。第一部分见：

## 《算法笔记》7.1小节——数据结构专题(1)->栈的应用
http://codeup.cn/contest.php?cid=100000605
### ★★★ 问题 A: 简单计算器 
**题目描述**：读入一个只包含 +, -, *, / 的非负整数计算表达式，计算该表达式的值。
- 输入：测试输入包含若干测试用例，每个测试用例占一行，每行不超过200个字符，整数和运算符之间用一个空格分隔。没有非法表达式。当一行中只有0时输入结束，相应的结果不要输出。
- 输出：对每个测试用例输出1行，即该表达式的值，精确到小数点后2位。
- 样例输入
	```
	30 / 90 - 26 + 97 - 5 - 6 - 13 / 88 * 6 + 51 / 29 + 79 * 87 + 57 * 92
	0
	```
- 样例输出
	```
	12178.21
	```
要注意这里采用的输入方法，以后可能会用到。
```c

```
### ★★ 问题 B: Problem E
**题目描述**：请写一个程序，判断给定表达式中的括号是否匹配，表达式中的合法括号为”(“, “)”, “[", "]“, “{“, ”}”,这三个括号可以按照任意的次序嵌套使用。
- 输入：有多个表达式，输入数据的第一行是表达式的数目，每个表达式占一行。
- 输出：对每个表达式，若其中的括号是匹配的，则输出”yes”，否则输出”no”。
- 样例输入
	```
	4
	[(d+f)*{}]
	[(2+3))
	()}
	[4(6]7)9
	```
- 样例输出
	```
	yes
	no
	no
	no
	```
这个题目还是蛮简单的，就是个平衡符号的判断题。
- 在输入整数后下一行，如果要使用gets读入字符串，**必须先用getchar()吸收整数输入后的换行符**；
- 这个算法是Online的，O(n)的复杂度。
```c
#include <cstdio>
#include <stack>
#include <map>
using namespace std;

int main() {
	int n;
	scanf("%d", &n);
	getchar(); //吸收整数输入后的换行符 
	map<char, char> mp;
	mp[']'] = '[';
	mp[')'] = '(';
	mp['}'] = '{';
	char str[10000];
	stack<char> s;
	while (n--) {
		while (!s.empty()) s.pop(); //清空循环外面定义的栈 
		gets(str);
		int flag = 1; //符号是平衡的 
		for (int i = 0; str[i]; i++) {
			if (str[i] == '[' || str[i] == '(' || str[i] == '{') s.push(str[i]);
			else if (str[i] == ']' || str[i] == ')' || str[i] == '}') {
				if (s.empty() || s.top() != mp[str[i]]) { 
				//此时栈空或栈顶开放符号不对应封闭符号
					flag = 0; break;
				} else s.pop(); 
			}
		} //输入处理结束后，栈非空，则报错
		if (!s.empty() || !flag) printf("no\n");
		else printf("yes\n");
	}
	return 0;
} 
```
## 《算法笔记》7.2小节——数据结构专题(1)->队列的应用
http://codeup.cn/contest.php?cid=100000606
### 问题 A: C语言-数字交换
相同的题目。
```c
#include <cstdio>
#include <algorithm>
using namespace std;

void input(int num[]) {
    for (int i = 0; i < 10; i++) {
        scanf("%d", &num[i]);
    }
}

void process(int num[]) {
    int max, min;
    max = min = 0;
    for (int i = 0; i < 10; i++) {
        if (num[i] < num[min]) min = i;
    }
    swap(num[0], num[min]);
    for (int i = 0; i < 10; i++) {
        if (num[i] > num[max]) max = i;
    }
    swap(num[9], num[max]);
}

void print(int num[]) {
    for (int i = 0; i < 10; i++) {
        printf("%d ", num[i]);
    }
}
int main() {
    int num[10];
    input(num);
    process(num);
    print(num);
    return 0;
}
```
## 《算法笔记》7.3小节——数据结构专题(1)->链表处理
http://codeup.cn/contest.php?cid=100000607
### ★★ 1326 Problem  A	算法2-8~2-11：链表的基本操作
这种**使用输出型引用参数**和**使用status表示函数执行状态**的想法可以用在自己写数据结构的时候，还有这里**使用头插法倒序插入元素创建链表**的写法可以学习。
```c
#include <cstdio>
#include <cstdlib>
#define ERROR (0)
#define OK (1)
typedef int status;

typedef int ElemType; 
typedef struct LNode {
	ElemType data;
	struct LNode *next; 
} LNode, *LinkList;

status GetELem_L(LinkList &L, int i, ElemType &e) {
	LinkList p = L->next;
	int j = 1;
	while (p && j < i) {
		p = p->next;
		j++;
	}
	if (!p || j > i) {
		return ERROR;
	}
    e = p->data;
	return OK;
}

status ListInsert_L(LinkList &L, int i, ElemType e) {
	LinkList p = L, s;
	int j = 0;
	while (p && j < i - 1) {
		p = p->next;
		++j;
	}
	if (!p || j > i - 1) return ERROR;
	s = (LinkList)malloc(sizeof(LNode));
	s->data = e;
	s->next = p->next;
	p->next = s;
	return OK;
}

status ListDelete_L(LinkList &L, int i, ElemType &e) {
	LinkList p = L, q;
	int j = 0;
	while (p->next && j < i - 1) {
		p = p->next;
		j++;
	}
	if (!(p->next) || j > i - 1) return ERROR;
	q = p->next;
	p->next = q->next;
	e = q->data;
	free(q);
	return OK;
}

void CreateList_L(LinkList &L, int n) {
	LinkList p;
	int i;
	L = (LinkList)malloc(sizeof(LNode));
	L->next = NULL;
	for (i = n; i > 0; --i) {
		p = (LinkList)malloc(sizeof(LNode));
		scanf("%d", &p->data);
		p->next = L->next;
		L->next = p; //插入到表头 
	}
} 

void PrintList(LinkList &L) {
	LinkList p = L->next;
	int cnt = 0;
	while (p) {
		if (cnt++ > 0) printf(" ");
		printf("%d", p->data);
		p = p->next;
	}
	printf("\n");
}

int main() {
	int n;
	scanf("%d", &n);
	LinkList L;
	CreateList_L(L, n);

	int m, a, e;
	scanf("%d", &m);
	char s[10];
	while (m--) {
		scanf("%s", s); 
		switch(s[0]) {
			case 'd':
				scanf("%d", &a);
				if (ListDelete_L(L, a, e)) printf("delete OK\n");
				else printf("delete fail\n");
				break;
			case 'g':
				scanf("%d", &a);
				if (GetELem_L(L, a, e)) printf("%d\n", e);
				else printf("get fail\n");
				break;
			case 'i':
				scanf("%d%d", &a, &e);
				if (ListInsert_L(L, a, e)) printf("insert OK\n");
				else printf("insert fail\n");
				break;
			case 's':
				if (L->next == NULL) printf("Link list is empty\n");
				else PrintList(L);
				break;
		}
	}
	return 0;
}
```
### 1870 Problem  B	C语言-链表排序
我没有用链表，用了两个数组分别排序，然后归并输出。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
struct stu {
	int id, grade;
} a[1000], b[1000];
bool cmp(struct stu a, struct stu b) {
	return a.id < b.id;
}
int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; i++) scanf("%d%d", &a[i].id, &a[i].grade);
	for (int i = 0; i < m; i++) scanf("%d%d", &b[i].id, &b[i].grade);
	sort(a, a + n, cmp); sort(b, b + m, cmp);
	
	int j = 0, k = 0;
	while (j < n && k < m) {
		if (a[j].id < b[k].id) {
			printf("%d %d\n", a[j].id, a[j].grade);
			j++;
		}
		else if (a[j].id > b[k].id) {
			printf("%d %d\n", b[k].id, b[k].grade);
			k++;
		}
	}
	for (; j < n; j++) printf("%d %d\n", a[j].id, a[j].grade);
	for (; k < m; k++) printf("%d %d\n", b[k].id, b[k].grade);

	return 0;
} 
```
这样写确实麻烦了，干脆只用一个数组，全部输入，然后排序直接输出，代码如下。
```c
#include <cstdio>
#include <algorithm>
using namespace std;
struct stu {
	int id, grade;
} a[2000];
bool cmp(struct stu a, struct stu b) {
	return a.id < b.id;
}
int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n + m; i++) scanf("%d%d", &a[i].id, &a[i].grade);
	sort(a, a + n + m, cmp);
	for (int j = 0; j < n + m; j++) printf("%d %d\n", a[j].id, a[j].grade);

	return 0;
} 
```
正统的写法如下(：
```c
#include <cstdio>
typedef struct LNode {
	int id, grade;
	struct LNode *next; 
} LNode, *LinkList;

/* 找到id小于data的最后一个结点, 即data理论上的前一个结点, 通过pre返回内容 */ 
void FindPrevious(LinkList &L, int data, LinkList &pre) {
	LinkList p = L;
	while (p->next && p->next->id < data) p = p->next;
	pre = p;
}
/* 通过输入和合适的插入创建n个元素的有序链表 */
void CreateSortedList(LinkList &L, int n) {
	L = new LNode;
	L->next = NULL;
	LinkList p, pre;
	for (int i = 0; i < n; i++) {
		p = new LNode;
		p->next = NULL;
		scanf("%d%d", &p->id, &p->grade);
		FindPrevious(L, p->id, pre);
		p->next = pre->next;
		pre->next = p; //插入到合适的位置 
	}
} 

void PrintList(LinkList &L) {
	LinkList p = L->next;
	int cnt = 0;
	while (p) {
		printf("%d %d\n", p->id, p->grade);
		p = p->next;
	}
}

int main() {
	int n, m;
	while (scanf("%d%d", &n, &m) != EOF) {
		LinkList L;
		CreateSortedList(L, n + m);
		PrintList(L);
	}
	return 0;
} 
```
### 2421 Problem  C	最快合并链表(线性表)		
欺骗自己(完全使用数组?)的做法：
```c
#include <cstdio>

int main() {
	int n, m, a[1000];
	while (scanf("%d", &n) != EOF) {
		for (int i = 0; i < n; i++) scanf("%d", &a[i]);
		scanf("%d", &m);
		for (int i = n; i < n + m; i++) scanf("%d", &a[i]);
		
		for (int j = 0; j < n + m; j++) {
			if (j > 0) printf(" ");
			printf("%d", a[j]);
		}
		printf("\n");
	}
	return 0;
} 
```
正统的写法如下：
```c
```
### 2453 Problem  D	链表查找(线性表)	
这个题目使得我困惑， `线性表(a1,a2,a3,…,an)中元素递增有序且按顺序存储于计算机内`，这真的不是让我们用数组吗？虽然用数组插入删除效率低。 `用最少时间在表中查找数值为x的元素，若找到……若找不到将则其插入表中并使表中元素仍递增有序`，这不是用二分查找吗？
先是数组的写法：
```c
#include <cstdio>
#include <algorithm>
using namespace std;
int binarySearch(int a[], int n, int key) {
	int low = 0, high = n - 1, mid;
	while (low <= high) {
		mid = (low + high) / 2;
		if (key == a[mid]) return mid;
		else if (key < a[mid]) high = mid - 1;
		else if (key > a[mid]) low = mid + 1;
	}
	return -1;
}

int main() {
	int x, n;
	while (scanf("%d%d", &x, &n) != EOF) {
		int a[n + 1], flag = 0;
		for (int i = 0; i < n; i++) scanf("%d", &a[i]);
		/* 用最少时间在表中查找数值为x的元素 */
		int pos = binarySearch(a, n, x);
		/* 若存在则标记 */
		if (pos != -1) {
			/* 不为最后一个元素, 则将其与后继元素位置相交换 */ 
			if (pos != n - 1) swap(a[pos], a[pos + 1]);
			flag = 1;
		} else if (pos == -1) {
			/* 不存在则将其插入表中并使表中元素仍递增有序 */ 
			int p;
			for (p = n - 1; a[p] > x; p--) a[p + 1] = a[p];
			a[p + 1] = x;
		} 
		int len = flag ? n : n + 1;
		if (!flag) printf("no\n");
		for (int i = 0; i < len; i++) {
			if (i > 0) printf(" ");
			printf("%d", a[i]);
		}
		printf("\n");
	}
	return 0;
} 
```
正统的写法如下：
```c
```
### 3596 Problem  E	算法2-24 单链表反转	
有的人说`输出“printf("list is empty");”无换行，否则会输出超限60%。`不过我试了，可以通过。
第一个方法是数组反转：
```c
#include <cstdio>
int main() {
	int n;
	while (scanf("%d", &n) != EOF) {
		if (n == 0) printf("list is empty\n");
		else {
			int a[n];
			for (int i = 0; i < n; i++) scanf("%d", &a[i]);
			for (int i = 0; i < n; i++) {
			    if (i > 0) printf(" ");
			    printf("%d", a[i]);
			}
			printf("\n");
			for (int i = n - 1; i >= 0; i--) {
			    if (i < n - 1) printf(" ");
			    printf("%d", a[i]);
			}
			printf("\n");
		}
	} 
	return 0;
}
```
第二个方法是头插法建表，直接输出，连反转都不用了：
```c
```
正统的写法如下：
```c
```
### 3597 Problem  F	算法2-25 有序单链表删除重复元素
第一个方法是数组，其中删除重复元素的方法为双指针，**慢指针用来保留元素，高速指针用来跳过重复元素**。
```c
#include <cstdio>
int main() {
	int n;
	while (scanf("%d", &n) != EOF) {
		if (n == 0) printf("list is empty\n");
		else {
			int a[n];
			for (int i = 0; i < n; i++) scanf("%d", &a[i]);
			//输出删除前的元素
			for (int i = 0; i < n; i++) {
			    if (i > 0) printf(" ");
			    printf("%d", a[i]);
			}
			printf("\n");
		    int low = 0, high = 0;
			while (high < n) {
				a[low] = a[high++];	
			    while (high < n && a[high] == a[low]) high++;
 				low++;
			}
			//输出删除后的元素
			for (int i = 0; i < low; i++) {
			    if (i > 0) printf(" ");
			    printf("%d", a[i]);
			}
			printf("\n");
		}
	} 
	return 0;
}
```
正统的写法如下：
```c
```

---
## 《算法笔记》8.1小节——搜索专题->深度优先搜索（DFS）
http://codeup.cn/contest.php?cid=100000608

### 5972 Problem  A【递归入门】全排列 
题目描述：排列与组合是常用的数学方法。
- 输入: 输入一个整数n(  1<=n<=10)
- 输出: 输出所有全排列，每个全排列一行，相邻两个数用空格隔开（最后一个数后面没有空格）
- 样例输入
	```
	3
	```
- 样例输出
	```
	1 2 3
	1 3 2
	2 1 3
	2 3 1
	3 1 2
	3 2 1
	```
使用next_permutation版本：
```c
#include <cstdio>
#include <algorithm>
using namespace std;

int main() {
    int n; scanf("%d", &n);
    int a[n];
    for (int i = 1; i <= n; i++) a[i - 1] = i;
    do {
        for (int i = 0; i <  n; i++) {
            if (i > 0) printf(" ");
            printf("%d", a[i]);
        }
        printf("\n");
    } while (next_permutation(a, a + n));
    return 0;
}
```
自己写DFS枚举的版本：
```c
#include <cstdio>
int n, hashTable[11] = {0}; //看看每个数选了没有 
void printPermutation(int index, int a[]) { //index: 1-n当前需要确定位置的第几个元素
	if (index > n) {
		for (int i = 0; i < n; i++) {
            if (i > 0) printf(" ");
            printf("%d", a[i]);
        }
        printf("\n");
	} else {
		for (int i = 1; i <= n; i++) {
			if (hashTable[i] == 0) {
				hashTable[i] = 1;
				a[index - 1] = i; //从0位开始填充 
				printPermutation(index + 1, a);
				hashTable[i] = 0;
			}
		}
	}
}

int main() {
    scanf("%d", &n);
    int a[n];
    printPermutation(1, a);
    return 0;
}
```
### 5973 Problem  B 【递归入门】组合的输出
题目描述：排列与组合是常用的数学方法，其中组合就是从n个元素中抽出r个元素(不分顺序且r < ＝ n)，我们可以**简单地将n个元素理解为自然数1，2，…，n**，从中任取r个数。现要求你不用递归的方法输出所有组合。
例如n ＝ 5 ，r ＝ 3 ，所有组合为：
```
1 2 3
1 2 4
1 2 5
1 3 4
1 3 5
1 4 5
2 3 4
2 3 5
2 4 5
3 4 5 
```
- 输入：一行两个自然数n、r ( 1 < n < 21，1 < ＝ r < ＝ n )。
- 输出：所有的组合，**每一个组合占一行**且**其中的元素按由小到大的顺序排列**，所有的组合也按字典顺序。

递归写法：保持组合中的元素的字典序，是为了**防止出现顺序不同但是组合元素一样的相同组合**。可以作为枚举排列剪枝为组合的条件。因此，可以知道，组合的第一个元素只能枚举到n-r+1为止。循环的方法懒得写。
```c
#include <cstdio>
int n, r;
int hash[25] = {0};
void Combine(int index, int a[]) {
	if (index > r) {
		for (int i = 0; i < r; i++) {
			if (i > 0) printf(" ");
			printf("%d", a[i]);
		} 
		printf("\n");
		return;
	}
	for (int i = 1; i <= n; i++) {
		if (hash[i] == 0) {
			hash[i] = 1; //剪枝
			//选组合开头第一个元素且其不大于…时; 后面的元素都大于前面的元素 
			if ((index == 1 && i <= n - r + 1) || i > a[index - 2]) { 
				a[index - 1] = i;
				Combine(index + 1, a);
			}  
			hash[i] = 0;
		}
	}
}

int main() {
    scanf("%d%d", &n, &r);
    int a[r];
    Combine(1, a);
    return 0;
}
```
### 5974 Problem  C【递归入门】组合+判断素数
**题目描述**：已知 n 个整数b1,b2,…,bn以及一个整数 k（k＜n）。从 n 个整数中**任选 k 个整数**相加，可分别得到一系列的和。
例如当 n=4，k＝3，4 个整数分别为 3，7，12，19 时，可得**全部的组合与它们的和**为：
　　　　3＋7＋12=22　　3＋7＋19＝29　　7＋12＋19＝38　　3＋12＋19＝34。
现在，要求你计算出**和为素数共有多少种**。例如上例，只有一种的和为素数：3＋7＋19＝29。
- 输入：第一行两个整数：n , k （1<=n<=20，k＜n） 第二行n个整数：x1,x2，…,xn （1<=xi<=5000000） 
- 输出：一个整数（满足条件的方案数）。 
- 样例输入
	```
	4 3
	3 7 12 19
	```
- 样例输出
	```
	1
	```
```c
#include <cstdio>
#include <cmath>
int n, k;
int a[25], cnt = 0, sum = 0;
bool isPrime(int n) {
	if (n <= 1) return false;
	int sqr = sqrt(1.0 * n);
	for (int i = 2; i <= sqr; i++) 
		if (n % i == 0) return false;
	return true;
}
void Combine(int pos, int level) {
	if (level == k) {
		if (isPrime(sum)) cnt++; 
		return;
	}
	for (int i = pos; i < n; i++) { 	
		sum += a[i];
		Combine(i + 1, level + 1); 
		sum -= a[i];
	}
}

int main() {
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; i++)  scanf("%d", &a[i]); 
    Combine(0, 0);
    printf("%d\n", cnt);
    return 0;
}
```
### 5975 Problem  D【递归入门】n皇后 问题（原始的8皇后问题）
题目描述 会下国际象棋的人都很清楚：皇后可以在横、竖、斜线上不限步数地吃掉其他棋子。如何将8个皇后放在棋盘上（有8 * 8个方格），使它们谁也不能被吃掉！这就是著名的八皇后问题。
- 输入：一个整数n（ 1 < = n < = 10 ) 
- 输出：每行输出对应一种方案，按字典序输出所有方案。每种方案顺序输出皇后所在的列号，相邻两数之间用空格隔开。如果一组可行方案都没有，输出“no solute!”
- 样例输入
	```
	4
	```
- 样例输出
	```
	2 4 1 3
	3 1 4 2
	```
这是一个排列生成问题。如果视作子集问题或者组合问题，那么复杂度就太高了。
```c
#include <bits/stdc++.h>
int cnt = 0, n, hashTable[11] = {false}, col[10] = {0};
void dfs(int curRow){
    if (curRow > n) {
        for (int i = 1; i <= n; i++) {
            if (i > 1) printf(" ");
            printf("%d", col[i]);
        } 
        cnt++;
        printf("\n");
    } else {
        for (int x = 1; x <= n; x++) { //列数选择
            if (hashTable[x] == false) { //第x列没有皇后
                bool flag = true;
                for (int pre = 1; pre < curRow; pre++) {
                    if (abs(curRow - pre) == abs(x - col[pre])) {
                        flag = false; 
                        break;
                    }
                }
                if (flag) {
                    hashTable[x] = true;
                    col[curRow] = x;
                    dfs(curRow + 1);
                    hashTable[x] = false;
                }
            }
        }
    }
}

int main() {
    scanf("%d", &n);
    dfs(1);
    if (cnt == 0) printf("no solute!\n");
    return 0;
}
```
### 5976 Problem  E【递归入门】出栈序列统计 
### 5977 Problem  F
### 5972 Problem  A
## 《算法笔记》8.2小节——搜索专题->广度优先搜索（BFS）
### 2078 Problem  A Jugs
### 4054 Problem  B	DFS or BFS?
### 5997 Problem  C	【宽搜入门】8数码难题
### 5998 Problem  D	【宽搜入门】魔板
### 5999 Problem  E	【宽搜入门】巧妙取量 
---
## 《算法笔记》9.1小节——数据结构专题(2)->树与二叉树
http://codeup.cn/contest.php?cid=100000610
## 《算法笔记》9.2小节——数据结构专题(2)->二叉树的遍历
## 《算法笔记》9.3小节——数据结构专题(2)->树的遍历
## 《算法笔记》9.4小节——数据结构专题(2)->二叉查找树（BST）
## 《算法笔记》9.6小节——数据结构专题(2)->并查集
http://codeup.cn/contest.php?cid=100000615
### 问题 A: 通信系统
**题目描述**：某市计划建设一个通信系统。按照规划，这个系统包含若干端点，这些端点由通信线缆链接。**消息可以在任何一个端点产生，并且只能通过线缆传送**。每个端点接收消息后会将消息**传送到与其相连的端点**，除了那个消息发送过来的端点。如果某个端点是产生消息的端点，那么消息将被传送到与其相连的每一个端点。
为了提高传送效率和节约资源，要求**当消息在某个端点生成**后，**其余各个端点均能接收到消息**，并且**每个端点均不会重复收到消息**。
现给你通信系统的描述，你能**判断此系统是否符合以上要求**吗？
- 输入
输入包含多组测试数据。每两组输入数据之间由空行分隔。
每组输入首先包含2个整数N和M，N（1<=N<=1000）表示端点个数，M（0<=M<=N*(N-1)/2）表示通信线路个数。
接下来M行每行输入2个整数A和B（1<=A，B<=N），表示端**点A和B由一条通信线缆相连**。两个端点之间**至多由一条线缆直接相连**，并且**没有将某个端点与其自己相连的线缆**。
当N和M都为0时，输入结束。
- 输出
对于每组输入，如果所给的系统描述符合题目要求，则输出Yes，否则输出No。
- 样例输入
	```
	4 3
	1 2
	2 3
	3 4
	
	3 1
	2 3
	
	0 0
	```
- 样例输出
	```
	Yes
	No
	```
这题还是有点坑的，虽然这里说了`没有将某个端点与其自己相连的线缆`，这就排除了环(自回路)。`两个端点之间至多由一条线缆直接相连`，这就排除了多重边。但是它要求`每个端点均不会重复收到消息`，而且`0<=M<=N*(N-1)/2`，说明最大可以是无向完全图，每个端点都与其他端点有连线。而我们**只想要树**。根据树的边只有N-1条，可以判断会不会形成回路。
```c
#include <cstdio>
const int maxn = 1010;
typedef int SetName;
typedef int ELemType;
typedef ELemType DisjSet[maxn];
DisjSet syst; //通信系统

void Initialize(int N) {
    for (int i = 0; i < N; i++) syst[i] = -1;    
}

SetName Find(ELemType x) {
    if (syst[x] < 0) return x;
    else return syst[x] = Find(syst[x]);
}

void SetUnion(SetName root1, SetName root2) {
    if (syst[root1] < syst[root2]) {
        syst[root1] += syst[root2];
        syst[root2] = root1;
    } else {
        syst[root2] += syst[root1];
        syst[root1] = root2;
    }
}
int main() {
    int n, m;
    while (scanf("%d%d", &n, &m), n || m) {
        Initialize(n);
        ELemType a, b;
        for (int i = 0; i < m; i++) {
        	scanf("%d%d", &a, &b);
            SetName r1 = Find(a - 1), r2 = Find(b - 1);
            if (r1 != r2) SetUnion(r1, r2); //两个端点间有连线
        }
        int ans = 0; //记录连通块个数
        for (int i = 0; i < n; i++) {
            if (syst[i] < 0) ans++;
        }
        if (ans == 1 && m == n - 1) printf("Yes\n");
        else printf("No\n");
    }
    return 0;
}
```
### 问题 B: 畅通工程
**题目描述**：某省调查城镇交通状况，得到现有城镇道路统计表，表中列出了每条道路直接连通的城镇。省政府“畅通工程”的目标是使全省任何两个城镇间都可以实现交通（但**不一定有直接的道路相连**，只要互相间接通过道路可达即可）。问**最少还需要建设多少条道路**？
- 输入
测试输入包含若干测试用例。每个测试用例的第1行给出两个正整数，分别是城镇数目N ( < 1000 )和道路数目M；随后的M行对应M条道路，每行给出一对正整数，分别是该条道路直接连通的两个城镇的编号。为简单起见，**城镇从1到N编号**。 
    注意: **两个城市之间可以有多条道路相通**,也就是说
    3 3
    1 2
    1 2
    2 1
    这种输入也是合法的。当N为0时，输入结束，该用例不被处理。
- 输出：对每个测试用例，在1行里输出最少还需要建设的道路数目。
- 样例输入
	```
	5 3
	1 2
	3 2
	4 5
	0
	```
- 样例输出
	```
	1
	```
与算法笔记上的代码有点相似。这题的**输入允许多重边，甚至是回路**，不过我们的代码自动剔除了这种情况。**只需要统计出有多少个集合**(划分)就可以了，最少修建的道路就是这些集合间的生成树，数目就是集合数-1。
```c
#include <cstdio>
#include <cstring>
const int maxn = 1010;
int father[maxn];

int Find(int x) {
    if (father[x] < 0) return x;
    else return father[x] = Find(father[x]);
}

void Union(int root1, int root2) {
    if (father[root1] < father[root2]) {
        father[root1] += father[root2];
        father[root2] = root1;
    } else {
        father[root2] += father[root1];
        father[root1] = root2;
    }
}

int main() {
    int n, m, a, b;
    while (scanf("%d", &n) && n) {
        memset(father, -1, sizeof(father));
        scanf("%d", &m);
        for (int i = 0; i < m; i++) {
            scanf("%d%d", &a, &b);
            int r1 = Find(a - 1), r2 = Find(b - 1);
            if (r1 != r2) Union(r1, r2);
        }
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (father[i] < 0) ans++;
        }
        printf("%d\n", ans - 1); //集合数-1
    }
    return 0;
}
```
## 《算法笔记》9.7小节——数据结构专题(2)->堆
http://codeup.cn/contest.php?cid=100000616
### 问题 C: 合并果子（堆）
**题目描述**：在一个果园里，多多已经将所有的果子打了下来，而且按果子的不同种类分成了不同的堆。多多决定把所有的果子合成一堆。
    每一次合并，多多可以**把两堆果子合并到一起**，**消耗的体力等于两堆果子的重量之和**。可以看出，所有的果子经过n-1次合并之后，就只剩下一堆了。多多在合并果子时总共消耗的体力等于每次合并所耗体力之和。
    因为还要花大力气把这些果子搬回家，所以多多在合并果子时**要尽可能地节省体力**。假定每个果子重量都为1，并且已知果子的种类数和每种果子的数目，你的任务是设计出合并的次序方案，使多多耗费的体力最少，并**输出这个最小的体力耗费值**。
    例如有3种果子，数目依次为1，2，9。可以先将 1、2堆合并，新堆数目为3，耗费体力为3。接着，将新堆与原先的第三堆合并，又得到新的堆，数目为12，耗费体力为 12。所以多多总共耗费体力=3+12=15。可以证明15为最小的体力耗费值。
- 输入
输入文件fruit.in包括两行，第一行是一个整数n（1 <= n <= 30000），表示果子的种类数。第二行包含n个整数，用空格分隔，第i个整数ai（1 <= ai <= 20000）是第i种果子的数目。
- 输出
输出文件fruit.out包括一行，这一行只包含一个整数，也就是最小的体力耗费值。输入数据保证这个值小于2^31^。
- 样例输入
	```
	10
	3 5 1 7 6 4 2 5 4 1
	```
- 样例输出
	```
	120
	```
通过每次将最小的两堆搬到一起，N堆要搬运log N次，可以得到最小的体力消耗。其实就是哈夫曼树的构建思想。
```c
#include <bits/stdc++.h>
using namespace std;
priority_queue<long long, vector<long long>, greater<long long> > q;

int main() {    
    int n;
	long long t; 
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &t);
        q.push(t); //将初始重量压入优先队列 
    }
    long long ans = 0; //体力最小消耗量
    while (q.size() > 1) {
        int t1 = q.top(); q.pop();
        int t2 = q.top(); q.pop();
        q.push(t1 + t2); //取出堆顶的两个元素, 求和后压入优先队列 
        ans += (t1 + t2); //累计求和的结果 
        
    }
    printf("%lld\n", ans);
    return 0;
}
```
---
## 《算法笔记》10.1小节——图算法专题->图的定义和相关术语
本小节暂无练习题。
## 《算法笔记》10.2小节——图算法专题->图的存储
本小节暂无练习题。
但是**图的存储和遍历与二叉树的存储与遍历都是基础中的基础**。
## 《算法笔记》10.3小节——图算法专题->图的遍历
### 问题 A: 第一题
该题的目的是要你统计图的连通分支数。
- 输入：每个输入文件包含若干行，每行两个整数i,j，表示节点i和j之间存在一条边。
- 输出：输出每个图的联通分支数。
- 样例输入
	```
	1 4
	4 3
	5 5
	```
- 样例输出
	```
	2
	```
这一题确实有点坑，开始一看觉得很简单，也确实简单，不过它没有给出结点编号的范围。因此很容易`运行错误`。下面的总结写的很完善了，引用过来。
> 事实上测试数据结点编号在1~1e6之间，标记数组稍微小一个数量级都会出现段错误
> 然后，就是要注意会出现自己指向自己的边，在使用邻接表进行记录时要区分一下，不然会有重复元素，但实际上不区分，也是可以通过的，就是耗时多一些。
> 访问顶点，直接在输入数据的时候就保存一个最大编号的变量，这样直接for循环就可以，省事而且高效。
 ———————————————— 
版权声明：本文为CSDN博主「漫浸天空的雨色」的原创文章。原文链接：https://blog.csdn.net/a845717607/article/details/81708837

```c
#include <cstdio>
#include <vector>
using namespace std;
const int maxv = 500010;
vector<int> Adj[maxv];
bool vis[maxv] = {false};

int CreateGraph() {
    int i, j;
    int max = -1; //记录最大顶点的编号, 以便逐个访问连通块 
    while (scanf("%d%d", &i, &j) != EOF) {
    	if (i != j) {
    		Adj[i].push_back(j);
        	Adj[j].push_back(i);
		} else Adj[i].push_back(j);
        //i可能等于j
        max = max > i ? max : i;
        max = max > j ? max : j;
    }
    return max;
}

void DFS(int u) {
    vis[u] = true; //点u已经访问
    for (int i = 0; i < Adj[u].size(); i++) {
        int v = Adj[u][i]; 
        if (vis[v] == false) {
            DFS(v);
        }
    }
}

void DFSTraverse(int &comp, int max) {
    for (int i = 1; i <= max; i++) {
        if (Adj[i].size() > 0 && vis[i] == false) { //该点存在
            DFS(i); comp++; //访问结束了一个连通块 
        }
    }
}

int main() {
    int max = CreateGraph();
    int components = 0;
    DFSTraverse(components, max); 
    printf("%d\n", components); //打印连通分支个数
    return 0;
}
```
### 问题 B: 连通图 
题目描述：给定一个无向图和其中的所有边，判断这个图是否所有顶点都是连通的。
输入

每组数据的第一行是两个整数 n 和 m（0<=n<=1000）。**n 表示图的顶点数目，m 表示图中边的数目**。如果 n 为 0 表示输入结束。随后有 m 行数据，每行有两个值 x 和 y（**0<x, y <=n**），表示顶点 x 和 y 相连，**顶点的编号从 1 开始计算**。**输入不保证这些边是否重复**。
输出

对于每组输入数据，如果所有顶点都是连通的，输出"YES"，否则输出"NO"。
- 样例输入
	```
	4 3
	4 3
	1 2
	1 3
	
	5 7
	3 5
	2 3
	1 3
	3 2
	2 5
	3 4
	4 1
	
	7 3
	6 2
	3 1
	5 6
	
	0 0
	```
- 样例输出
	```
	YES
	YES
	NO
	```
很多这种无向图连通的问题，可以**转化为并查集解决**。
```c
#include <cstdio>

const int maxv = 1010;
int father[maxv];

void Initialize(int n) {
	for (int i = 0; i < n; i++) father[i] = -1;
}
int Find(int x) {
    if (father[x] < 0) return x;
    else return father[x] = Find(father[x]);
}

void SetUnion(int r1, int r2) {
    if (father[r1] < father[r2]) {
        father[r1] += father[r2];
        father[r2] = r1;
    } else {
        father[r2] += father[r1];
        father[r1] = r2;
    }
}

int main() {
    int n, m;
    while (scanf("%d", &n) != EOF && n != 0) {
        int comp = 0, u, v;
        Initialize(n);
        scanf("%d", &m);
        for (int i = 0; i < m; i++) {
            scanf("%d%d", &u, &v);
            int r1 = Find(u - 1), r2 = Find(v - 1);
            if (r1 != r2) SetUnion(r1, r2);
        }
        for (int i = 0; i < n; i++) {
            if (father[i] < 0) comp++;
        }
        if (comp == 1) printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}
```
**DFS的邻接表**代码。事实上，这题的结点个数<=1000，完全可以用邻接矩阵。
```c
#include <cstdio>
#include <vector>
using namespace std;

const int maxv = 1010;
vector<int> adj[maxv];
bool vis[maxv] = {0};
void CreateGraph(int m) {
	for (int i = 1; i <= m; i++) adj[i].clear(); 
    int u, v;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d", &u, &v);
        if (u != v) {
            adj[u].push_back(v);
            adj[v].push_back(u);     
        } else adj[u].push_back(v);
    }
}

void DFS(int u) {
    vis[u] = true;
    for (int i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i];
        if (vis[v] == false) {
            DFS(v);
        }
    }
}

void DFSTrave(int n, int &comp) {
	for (int i = 1; i <= n; i++) vis[i] = false; 
    for (int i = 1; i <= n; i++) {
        if (vis[i] == false) { //孤立点也是一个连通块
            DFS(i); comp++;
        }
    }    
}

int main() {
    int n, m;
    while (scanf("%d", &n) != EOF && n != 0) {
        int comp = 0;
        scanf("%d", &m);
        CreateGraph(m); //需要初始化邻接表 
        DFSTrave(n, comp); //每次遍历要重新设置vis数组
        if (comp == 1) printf("YES\n");
        else printf("NO\n");
    }
    return 0;
}
```
## ☆☆☆《算法笔记》10.4小节——图算法专题->最短路径
### 
### 
### 
### 
### 
