#include <bits/stdc++.h>
using namespace std;
const int maxn = 3e6 + 10;
int arr[maxn], ans[maxn], st[maxn], top = -1;

int main() {
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; ++i) scanf("%d", &arr[i]);
	//��ջ�׵�ջ�����±��Ӧ������ֵ(���ȼ�)���εݼ�,
	//����ȳ�,��Ӧ->��i��Ԫ��֮���һ������a_i��Ԫ�ص��±� 
	for (int i = n; i > 0; --i) {
		//���Ƚ�ջ(�����)�Ҳ�����arr[i]��ֵ�����������,
		//��һ���̾��Ǵ�i+1����Ѱ�ҵ�һ������a_i��Ԫ���±�
		while (top != -1 && arr[st[top]] <= arr[i]) --top;  
		ans[i] = (top == -1 ? 0 : st[top]);	//top=-1��ʾ֮��û�д���arr[i]��ֵ 
		st[++top] = i; //�洢arr�������±� 
	}
	for (int i = 1; i <= n; ++i) {
		if (i == 1) printf("%d", ans[i]);
		else printf(" %d", ans[i]);
	}
    return 0;
}
