#include <bits/stdc++.h>
using namespace std;
const int maxn = 3e6 + 10;
int arr[maxn], ans[maxn], st[maxn], top = -1;

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%d", &arr[i]);
	//从栈底到栈顶的下标对应的数组值(优先级)依次递减,
	//后进先出,对应->第i个元素之后第一个大于a_i的元素的下标 
	for (int i = n; i > 0; --i) {
		//将先进栈(后面的)且不大于arr[i]的值的坐标清除掉,
		//这一过程就是从i+1往后寻找第一个大于a_i的元素下标
		while (top != -1 && arr[st[top]] <= arr[i]) --top;  
		ans[i] = (top == -1 ? 0 : st[top]);	//top=-1表示之后没有大于arr[i]的值 
		st[++top] = i; //存储arr的数组下标 
	}
	for (int i = 1; i <= n; ++i) {
		if (i == 1) printf("%d", ans[i]);
		else printf(" %d", ans[i]);
	}
    return 0;
}
