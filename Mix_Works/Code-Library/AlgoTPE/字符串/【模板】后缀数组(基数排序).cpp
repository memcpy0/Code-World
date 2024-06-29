#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200005;   //�ַ����ĳ���
char s[MAXN];			   //�����ַ���
int sa[MAXN], rk[MAXN], cnt[MAXN], height[MAXN];
int t1[MAXN], t2[MAXN];
int n; 

void calc_sa() { //void build_sa(int n, int m)  //n���ַ�������
	int m = 127;	//m��Сд��ĸ��ASCII�뷶Χ,�����ַ���s�ĺ�׺����,ÿ���ַ���ֵ����Ϊ0~m-1
	int i, *x = t1, *y = t2;
	for (i = 0; i < m; ++i) cnt[i] = 0;
	for (i = 0; i < n; ++i) cnt[x[i] = s[i]]++;
	for (i = 1; i < m; ++i) cnt[i] += cnt[i - 1];	//ǰ׺��
	for (i = n - 1; i >= 0; --i) sa[--cnt[x[i]]] = i; //����
	//sa[]: ��0��n-1
	for (int k = 1; k <= n; k *= 2)	{ //���öԳ���Ϊk������Ľ���Գ���Ϊ2k����
		int p = 0;
		//2nd
		for (i = n - k; i < n; ++i) y[p++] = i;
		for (i = 0; i < n; ++i) if (sa[i] >= k) y[p++] = sa[i] - k;
		//1st
		for (i = 0; i < m; ++i) cnt[i] = 0;
		for (i = 0; i < n; ++i) cnt[x[y[i]]]++;
		for (i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
		for (i = n - 1; i >= 0; --i) sa[--cnt[x[y[i]]]] = y[i];
		swap(x, y);
		p = 1; 
		x[sa[0]] = 0;
		for (i = 1; i < n; ++i)
			x[sa[i]] = 
				y[sa[i - 1]] == y[sa[i]] && y[sa[i - 1] + k] == y[sa[i] + k] ? p - 1 : p++;
		if (p >= n) break;
		m = p;
	}
}
	 

int main() { 
	while (scanf("%s", s) != EOF) { //��ȡ�ַ���
		n = strlen(s);
		calc_sa();					//���׺����sa[]
		for (int i = 0; i < n; ++i)
			cout << sa[i] << " ";
	}
    return 0;
}
