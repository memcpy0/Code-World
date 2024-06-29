#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200005;   //字符串的长度
char s[MAXN];			   //输入字符串
int sa[MAXN], rk[MAXN], tmp[MAXN+1];
int n, k; 

bool comp_sa(int i, int j) {		//组合数有两个部分:高位是rk[i],低位是rk[i + k]
	if (rk[i] != rk[j])				//先比较高位,rk[i]和rk[j] 
		return rk[i] < rk[j];
	else {							//高位相等,再比较低位的rk[i+k]和rk[j+k] 
		int ri = i + k <= n ? rk[i + k] : -1;
		int rj = j + k <= n ? rk[j + k] : -1;
		return ri < rj;
	}
}

void calc_sa() { 					//计算字符串s的后缀数组
	for (int i = 0; i <= n; ++i) {
		rk[i] = s[i];				//字符串的原始数值 
		sa[i] = i;		 			//后缀数组,每一步记录当前排序后的结果
	}
	for (k = 1; k <= n; k *= 2) {   //开始一步步操作,每一步递增两倍进行组合
		sort(sa, sa + n, comp_sa);
		tmp[sa[0]] = 0;
		for (int i = 0; i < n; ++i) //用sa倒推组合数,记录在tmp[]中
			tmp[sa[i + 1]] = tmp[sa[i]] + (comp_sa(sa[i], sa[i + 1] ? 1 : 0));
		for (int i = 0; i < n; ++i)	//把tmp[]复制给rk[],用于下一步操作
			rk[i] = tmp[i];
	}
}

int main() { 
	while (scanf("%s", s) != EOF) { //读取字符串
		n = strlen(s);
		calc_sa();					//求后缀数组sa[]
		for (int i = 0; i < n; ++i)
			cout << sa[i] << " ";
	}
    return 0;
}

作者：陈劭源
链接：https://www.zhihu.com/question/390810636/answer/1328498563
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

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
