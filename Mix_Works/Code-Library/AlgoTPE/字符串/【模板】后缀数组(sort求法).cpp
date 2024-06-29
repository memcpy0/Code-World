#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200005;   //�ַ����ĳ���
char s[MAXN];			   //�����ַ���
int sa[MAXN], rk[MAXN], tmp[MAXN+1];
int n, k; 

bool comp_sa(int i, int j) {		//���������������:��λ��rk[i],��λ��rk[i + k]
	if (rk[i] != rk[j])				//�ȱȽϸ�λ,rk[i]��rk[j] 
		return rk[i] < rk[j];
	else {							//��λ���,�ٱȽϵ�λ��rk[i+k]��rk[j+k] 
		int ri = i + k <= n ? rk[i + k] : -1;
		int rj = j + k <= n ? rk[j + k] : -1;
		return ri < rj;
	}
}

void calc_sa() { 					//�����ַ���s�ĺ�׺����
	for (int i = 0; i <= n; ++i) {
		rk[i] = s[i];				//�ַ�����ԭʼ��ֵ 
		sa[i] = i;		 			//��׺����,ÿһ����¼��ǰ�����Ľ��
	}
	for (k = 1; k <= n; k *= 2) {   //��ʼһ��������,ÿһ�����������������
		sort(sa, sa + n, comp_sa);
		tmp[sa[0]] = 0;
		for (int i = 0; i < n; ++i) //��sa���������,��¼��tmp[]��
			tmp[sa[i + 1]] = tmp[sa[i]] + (comp_sa(sa[i], sa[i + 1] ? 1 : 0));
		for (int i = 0; i < n; ++i)	//��tmp[]���Ƹ�rk[],������һ������
			rk[i] = tmp[i];
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

���ߣ���ۿԴ
���ӣ�https://www.zhihu.com/question/390810636/answer/1328498563
��Դ��֪��
����Ȩ���������С���ҵת������ϵ���߻����Ȩ������ҵת����ע��������

void radix_sort(int x[], int y[], int sa[], int n, int m) {
    static int cnt[MAXN];
    fill(cnt, cnt + m, 0);
    for (int i = 0; i < n; i++) cnt[x[y[i]]]++;
    partial_sum(cnt, cnt + m, cnt);
    for (int i = n - 1; i >= 0; i--) sa[--cnt[x[y[i]]]] = y[i];
}
 
void suffix_array(int s[], int sa[], int rk[], int n, int m) {
    static int y[MAXN];
    copy(s, s + n, rk);
    iota(y, y + n, 0);
    radix_sort(rk, y, sa, n, m);
    for (int j = 1, p = 0; j <= n; j <<= 1, m = p, p = 0) {
        for (int i = n - j; i < n; i++) y[p++] = i;
        for (int i = 0; i < n; i++) if (sa[i] >= j) y[p++] = sa[i] - j;
        radix_sort(rk, y, sa, n, m + 1); 
        swap_ranges(rk, rk + n, y);
        rk[sa[0]] = p = 1;
        for (int i = 1; i < n; i++) 
            rk[sa[i]] = p += (y[sa[i]] == y[sa[i-1]] && y[sa[i]+j] == y[sa[i-1]+j]);
        if (p == n) break;
    }
    for (int i = 0; i < n; i++) rk[sa[i]] = i;
}
