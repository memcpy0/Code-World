@[toc]

###  ITP1_1_D	Watch
将秒转换成时分秒的格式。
```c
#include <cstdio>

int main() {
    int second;
    scanf("%d", &second);
    printf("%d:%d:%d\n",  second / 3600 , second % 3600 / 60, second % 60);
    return 0;
}

```
# Algorithms and Data Structures I 
## 1. Getting Started
### ALDS1_1_A	Insertion Sort
我写的插入排序：
```c
#include <cstdio>
//按顺序输出数组元素
void printArray(int a[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d", a[i]);
		if (i < n - 1) printf(" ");
		else printf("\n");
	}
}
template<typename T>
void insertionSort(T a[], int n) {
	T temp; 
	for (int i = 1; i < n; i++) {
		temp = a[i];
		int j;
		for (j = i; j > 0 && a[j - 1] > temp; j--) {
			a[j] = a[j - 1];
		}
		a[j] = temp;
		printArray(a, n);
	}
}

int main() {
    int N;
    scanf("%d", &N);
    int a[N];
    for (int i = 0; i < N; i++) {
    	scanf("%d", &a[i]);
	}
	printArray(a, n);
    insertionSort(a, N);
    return 0;
}
```
书上的插入排序：(同一种算法的不同实现)
```c
#include <cstdio>
//trace按顺序输出数组元素
template<typename T>
void trace(T a[], int n) {
	for (int i = 0; i < n; i++) {
		if (i > 0) printf(" ");
		printf("%c%c", a[i], a[i]);
	}
	printf("\n");
}
void insertionSort(int a[], int n) {
	int temp, j;
	for (int i = 1; i < n; i++) {
		temp = a[i]; 
		j = i - 1;
		while (j >= 0 && a[j] > temp) {
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = temp;
		trace(a, n);
	}
}

int main() {
    int N;
    scanf("%d", &N);
    int a[N];
    for (int i = 0; i < N; i++) {
    	scanf("%d", &a[i]);
	}
	trace(a, N);
    insertionSort(a, N);
    return 0;
}
```
## 2. Sort I
###  ALDS1_2_A	Bubble Sort
我的实现见下面， **从数组开头冒大泡到结尾**，书上的则是**从数组末尾冒小泡到开头**：
```c
#include <cstdio>
//trace函数见前面, 按顺序输出数组元素
template<typename T> 
void MySwap(T &a, T &b) {
	T t = a;
	a = b;
	b = t;
}
template<typename T>
void bubbleSort(T a[], int n) {
	int swapCount = 0;
	for (int i = n - 1; i > 0; i--) {
		int flag = 1; 
		for (int j = 0; j < i; j++) {
			if (a[j] > a[j + 1]) {
				MySwap(a[j], a[j + 1]);
				flag = 0; swapCount++;
			}
		}
		//trace(a, n); 
		if (flag) break; //当数组不存在顺序相反的相邻元素提前退出 
	}
	trace(a, n);
	printf("%d\n", swapCount);
}

int main() {
    int N;
    scanf("%d", &N);
    int a[N];
    for (int i = 0; i < N; i++) {
    	scanf("%d", &a[i]);
	}
    bubbleSort(a, N);
    return 0;
}
```
### ALDS1_2_B	Selection Sort
```c
#include <cstdio>
//trace函数见前面, 按顺序输出数组元素
//MySwap函数见前面, 使用引用交换两个模板变量的值
template<typename T>
void selectionSort(T a[], int n)  {
	int minj, swapCount = 0;
	for (int i = 0; i < n; i++) {
		minj = i;
		for (int j = i; j < n; j++) {
			if (a[j] < a[minj]) minj = j;
		}
		if (i != minj) {
			MySwap(a[i], a[minj]);
			swapCount++;
		}
	}
	trace(a, n);
	printf("%d\n", swapCount);
} 

int main() {
    int N;
    scanf("%d", &N);
    int a[N];
    for (int i = 0; i < N; i++) {
    	scanf("%d", &a[i]);
	}
    selectionSort(a, N);
    return 0;
}
```
### ★ ALDS1_2_C	Stable Sort
- 这里我使用了pair表示一张扑克牌的花色，pair**完全可以**当作两个元素的结构体使用。当然，多写一个两个元素的结构体也不难，可以使用如下的结构体。
- 其他函数与前面差别不大，只是去掉了一些不用的变量，改了一点地方而已。
- **判断排序算法的某一次输出是否稳定**的时候有一点小技巧，已知冒泡排序是稳定排序算法，其输出总是稳定的，而选择排序不是稳定排序，其输出有时稳定，有时不稳定。所以只需**将它们处理过的数组比较一下**就可以知道选择排序的某一次输出是否稳定。
```c
struct Card {char suit, value;}
```
```c
#include <cstdio>
#include <map>
#include <iostream>
using namespace std;

template<typename T>
void trace(T a[], int n) {
	...
		printf("%c%c", a[i].first, a[i].second);
	...
	printf("\n");
}
//MySwap函数见前面, 使用引用交换两个模板变量的值
template<typename T>
void bubbleSort(T a[], int n) {
	...
			if (a[j].second > a[j + 1].second) {
	...
	trace(a, n);
}
template<typename T>
void selectionSort(T a[], int n) {
	...
			if (a[j].second < a[minj].second) minj = j;
	...
 	trace(a, n);
} 

int main() {
    int N;
    scanf("%d", &N);
    pair<char, char> a[N], b[N];
    for (int i = 0; i < N; i++) {
    	cin >> a[i].first >> a[i].second;
    	b[i] = a[i];
    }
    bubbleSort(a, N);
    printf("Stable\n");
   
    selectionSort(b, N);
    int flag = 1;
    for (int i = 0; i < N; i++) {
    	if (a[i].first != b[i].first) {
    		printf("Not stable\n");
    		flag = 0; break;
		}
	}
	if (flag) printf("Stable\n");
    return 0;
}
```
### ★★ ALDS1_2_D	Shell Sort
现在我也是可以自己写希尔排序的人了?。
```c
#include <cstdio>
#include <vector>
using namespace std;
long long cnt;
int A[1000000];
vector<int> G;

template<typename T>
void shellSort(T a[], int n) {
	//生成希尔增量序列
	for (int h = 1; ;) {
		if (h > n) break;
		G.push_back(h);
		h = 3 * h + 1;
	}
	for (int i = G.size() - 1; i >= 0; i--) { //按逆序指定increment = G[i] 
		int increment = G[i];
		for (int j = increment; j < n; j++) {
			T temp = a[j];
			int k;
			for (k = j; k >= increment && a[k - increment] > temp; k -= increment) {
				a[k] = a[k - increment]; cnt++;
			}
			a[k] = temp;
		}
	}
}

int main() {
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%d", &A[i]);
	
	shellSort(A, N);
    printf("%d\n", G.size());
    for (int i = G.size() - 1; i >= 0; i--) {
    	printf("%d", G[i]);
    	if (i) printf(" ");
	}
	printf("\n%d\n", cnt);
	for (int i = 0; i < N; i++) printf("%d\n", A[i]);
    	
    return 0;
}
```
## 3. Elementary data structures
### ★★ ALDS1_3_A	Stack
这里通过stack计算逆波兰(后缀)表达式的结果。
- **用全局数组模拟了一个简易的栈**，AizuOJ是单点测试的，所以可以使用全局数组，不然还要再写一个makeEmpty/clear()函数来置数组为空；
- 这里全局数组开得够大，不用担心栈满上溢，而且也不会出现栈空时访问栈的情况，因此isEmpty()和isFull()函数这里是不需要的。
- 输入的数值可能是多位，2位以上，因此要用字符串(逆位)存储，**使用cstdlib的函数atoi将字符串形式的数值转换位整型数值**。
- 注意减法的顺序。
```c
#include <cstdio>
#include <cstdlib>
const int maxn = 1000;
int Stack[maxn], top = -1; //简易栈 Stack从0开始
bool isEmpty() {
	return top == -1;
} 
bool isFull() {
	return top + 1 == maxn;
}
void push(int x) {
	if (isFull()) 
		printf("Full Stack Error");
	else Stack[++top] = x;
}
int pop() {
	if (isEmpty()) 
		printf("Empty Stack Error");
	else return Stack[top--];
}

int main() {
	int a, b;
	char s[100]; //存储一个字符串, 数字或者是算符
	while (scanf("%s", s) != EOF) {
		if (s[0] == '+') {
			a = pop();
			b = pop();
			push(a + b);
		} else if (s[0] == '-') { //减法顺序要搞对 
			a = pop();
			b = pop();
			push(b - a);
		} else if (s[0] == '*') {
			a = pop();
			b = pop();
			push(a * b);
		} else { //数值字符串, 可能一位, 也可能多位数字 
			push(atoi(s)); 
		}
	} 
	printf("%d\n", pop());
	return 0;
}
```
- 如果我们**使用stl中的stack**的话，还能更简单一点；
- 这里**我们自己从逆位存储的字符串中还原回数字**。
```c
#include <cstdio>
#include <stack>
using namespace std;

int main() {
	int a, b;
	stack<int> st;
	char s[100]; //存储一个字符串, 数字或者是算符
	while (scanf("%s", s) != EOF) {
		if (s[0] == '+') {
			a = st.top(); st.pop();
			b = st.top(); st.pop();
			st.push(a + b);
		} else if (s[0] == '-') { //减法顺序要搞对 
			a = st.top(); st.pop();
			b = st.top(); st.pop();
			st.push(b - a);
		} else if (s[0] == '*') {
			a = st.top(); st.pop();
			b = st.top(); st.pop();
			st.push(a * b);
		} else { //数值字符串, 可能一位, 也可能多位数字 
            int sum = 0;
            for (int i = 0; s[i]; i++) 
            	sum = sum * 10 + (s[i] - '0'); 
			st.push(sum); 
		}
	} 
	printf("%d\n", st.top());
	return 0;
}
```
### ALDS1_3_B	Queue
这题**用队列和时间片模拟CPU通过循环调度法**处理名称为name~i~且处理时间为time~i~的n个任务，这n个任务按顺序排成一列。
- **用全局结构体数组模拟了一个简易的循环队列**，同上题一样的理由，不然还要再写一个makeEmpty/clear()函数来置数组为空；
- 这里**全局数组开得够大**，不用担心队列上溢，但是**可能会出现队列空时访问队列的情况**，因此isEmpty()函数这里是需要的，用于判断处理任务的循环什么时候结束。
```c
#include <cstdio>

typedef struct process {
	char name[12];
	int time;
} p;
const int maxn = 100100;
p queue[maxn];
int front = 0, rear = 0; //[front, rear)  
bool isEmpty() { //队空
	return front == rear; 
} 
bool isFull() { //队满 
	return front == (rear + 1) % maxn; 
}
void push(p x) {
	if (isFull()) {
		printf("Full Queue Error");
	} else {
		queue[rear] = x;
		rear = (rear + 1) % maxn;
	} 
}
p pop() {
	if (isEmpty()) {
		printf("Empty Queue Error");
	} else {
		p ans = queue[front];
		front = (front + 1) % maxn;
		return ans;
	}
}

int main() {
	int n, q, endTime = 0;
	scanf("%d%d", &n, &q);
	for (int i = 0; i < n; i++) { //按顺序将所有任务添加到队列
		scanf("%s%d", temp.name, &temp.time);
		push(temp);
	}
	p temp;
	while (!isEmpty()) { //队列不空的时候 
		temp = pop();
		if (temp.time <= q) {
			endTime += temp.time;
			printf("%s %d\n", temp.name, endTime);
		} else {
			temp.time -= q;
			endTime += q;
			push(temp);
		}
	}
	return 0;
}
```
- 如果我们**使用stl中的queue**的话，还能简洁一点：
```c
#include <utility>
#include <queue>
#include <string>
#include <iostream>
using namespace std;

int main() {
	int n, q, endTime = 0;
	cin >> n >> q;
	queue<pair<string, int> > Q;
	string name; int time;
	for (int i = 0; i < n; i++) {
		cin >> name >> time;
		Q.push(make_pair(name, time));
	}
	pair<string, int> t;
	while (!Q.empty()) { //队列不空的时候 
		t = Q.front(); Q.pop();
		if (t.second <= q) {
			endTime += t.second;
			cout << t.first << " " << endTime << endl;
		} else {
			t.second -= q;
			endTime += q;
			Q.push(t);
		}
	}
	return 0;
}
```
###  ★★ ALDS1_3_C	Doubly Linked List
写这个**双向链表**还挺费劲的。
```c
#include <cstdio>
/* 双向链表结点 */
typedef struct dbNode {
	int key;
	struct dbNode *prev;
	struct dbNode *next;
} dbNode;
dbNode *head; //全局的双向链表头结点

void init() { //初始化空表, 让prev和next都指向头结点head 
	head = new dbNode;
	head->next = head;
	head->prev = head; 
} 
 
void insert(int key) { //在头结点后面插入元素 
    dbNode *x = new dbNode;
    x->key = key; 
    x->next = head->next;
	head->next->prev = x;
	head->next = x;
	x->prev = head; 
}

dbNode *listSearch(int key) {
	dbNode *cur = head->next; //从头结点后面的元素开始访问
	while (cur != head && cur->key != key)  cur = cur->next;
	return cur;
}
void deleteNode(dbNode *t) { //删除指针指向的结点
	if (t == head) return;
	t->prev->next = t->next;
	t->next->prev = t->prev;
	delete t;
}

void deleteKey(int key) { //删除链表中第一个出现的元素为key的结点
	deleteNode(listSearch(key));
}
void deleteFirst() { //删除第一个数据结点 
	deleteNode(head->next);
}
void deleteLast() { //删除最后一个数据结点 
	deleteNode(head->prev);
}

void printList() { //打印整表
	dbNode *cur = head->next;
	int isf = 0; 
	while (cur != head)  {
		if (isf++ > 0) printf(" ");
		printf("%d", cur->key);
		cur = cur->next;
	}
	printf("\n");
}
void disposeDbList() { //销毁双端链表 
	dbNode *cur = head->next, *temp;
	while (cur != head)  {
		temp = cur->next;
	    delete cur;	
	    cur = temp;
	}
	delete head;
}

int main() {
	int n, x; 
	char order[15];
	init();
	scanf("%d", &n);
	while (n--) {
		scanf("%s", order);
		if (order[0] == 'i') {
			scanf("%d", &x); 
			insert(x);
		} else if (order[0] == 'd' && order[6] == 'F') { 
            deleteFirst();
		} else if (order[0] == 'd' && order[6] == 'L') {
			deleteLast();
		} else {
			scanf("%d", &x);
			deleteKey(x);
		}
	}
	printList();
	disposeDbList();
	return 0;
}
```
- 可以使用stl中的list，双端队列，支持`size()、begin()、end()、push_front(x)、pop_front()、push_back(x)、pop_front()、insert(p, x)、erase(p)、clear()`等操作。
- list还可以像vector一样使用`[]`访问，且元素插入和删除只需O(1)即可完成，效率很高。
```c
#include <cstdio>
#include <list>
using namespace std;

int main() {
	int n, x; 
	char order[15];
	list<int> v;
	scanf("%d", &n);
	while (n--) {
		scanf("%s", order);
		if (order[0] == 'i') {
			scanf("%d", &x); 
			v.push_front(x);
		} else if (order[0] == 'd' && order[6] == 'F') { 
            v.pop_front();
		} else if (order[0] == 'd' && order[6] == 'L') {
			v.pop_back();
		} else {
			scanf("%d", &x);
			for (list<int>::iterator it = v.begin(); it != v.end(); it++) {
				if (*it == x) {
					v.erase(it); break;
				}
			}
		}
	}
	int i = 0;
	for (list<int>::iterator it = v.begin(); it != v.end(); it++) {
	    if (i++) printf(" ");
	    printf("%d", *it);
	}
	printf("\n");
	return 0;
}
```
### ★★★ ALDS1_3_D	Areas on the Cross-Section Diagram	
```c
```
##  4. Search
### ALDS1_4_A	Linear Search
```c
#include <cstdio>
int main() {
	int n;
	scanf("%d", &n);
	int s[n + 1];
	for (int i = 0; i < n; i++) scanf("%d", &s[i]);
	int q;
	scanf("%d", &q);
	int t[q];
	for (int i = 0; i < q; i++) scanf("%d", &t[i]);
	/* 在集合t中搜索同时出现在集合s中的元素数量 */
	int cnt = 0;
	for (int i = 0; i < q; i++) {
		s[n] = t[i]; //哨兵 
		int j = 0;
		while (s[j] != t[i]) j++;
		if (j != n) cnt++;
	} 
	printf("%d\n", cnt);
	return 0;
} 
```
将线性搜索的部分独立出来形成函数如下，运用了哨兵的技巧(**数组要在末尾或开头多开一个额外的空间**，用于存放目标关键字的数据)，**可以简化循环控制，简化比较运算的次数**(因为标记可以确保while不成为死循环)，省去循环结束条件，将算法效率提高常数倍，处理大规模数据时效果明显。
```c
int linear_search(int A[], int n, int key) {
	int i = 0; A[n] = key;
	while (A[i] != key) i++;
	return i != n;
}
```
### ALDS1_4_B	Binary Search
```c
#include <cstdio>
int binary_search(int s[], int n, int key) {
	int low = 0, high = n - 1;
	while (low <= high) {
		int mid = (low + high) / 2;
		if (key == s[mid]) return mid;
		else if (key < s[mid]) high = mid - 1;
		else if (key > s[mid]) low = mid + 1;
	} 
	return -1;
} 
int main() {
	int n;
	scanf("%d", &n);
	int s[n];
	for (int i = 0; i < n; i++) scanf("%d", &s[i]);
	
	int q, temp, cnt = 0;
	scanf("%d", &q);
	for (int i = 0; i < q; i++) {
		scanf("%d", &temp);
		if (binary_search(s, n, temp) != -1) cnt++;
	}
	printf("%d\n", cnt);
	return 0;
} 
```
二分搜索中的low和high代表的下标范围可能会让人疑惑，换成别的下标可以吗？当然可以，二分搜索将数据整体作为搜索范围，因此**要求low和high能表达除-1外所有可能返回的下标**，即表达数组[0, n-1]的范围，而**循环条件则限制我们必须有能力搜索全部数据**。事实上我们还有一种写法(`注意`：如果**初始范围和循环条件**变化了的话，low和high的移动方法也要改变)。
- `[low, high]`，左闭右闭区间，开始为`[0, n-1]`，`while (low <= high)`表示范围不为Ø(`low > high`)时继续搜索；
- `[low, high)`，左闭右开区间，开始为`[0, n)`，`while (low < high)`表示范围不为Ø(`low == high)`时继续搜索；
	```c
	int binary_search(int s[], int n, int key) {
		int low = 0, high = n;
		while (low < high) {
			int mid = (low + high) / 2;
			if (key == s[mid]) return mid;
			else if (key < s[mid]) high = mid;
			else if (key > s[mid]) low = mid + 1;
		} 
		return -1;
	} 
	```
另外，如果是**找到数组中第一个满足某一性质的元素的位置**，这个位置必然是存在的，循环条件可以改为**只要搜索范围还大于1个元素**就继续搜索。
### ★★ ALDS1_4_C	Dictionary 
这道题里面使用了**双散列结构**。
###  ALDS1_4_D	Allocation
##  5. Recursion / Divide and Conquer
