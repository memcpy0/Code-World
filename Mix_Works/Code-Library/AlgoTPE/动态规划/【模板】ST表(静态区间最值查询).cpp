1. RMQ����

RMQ(Range Minimum/Maximum Query) ������С/��ֵ���⡣����һ�� n ��Ԫ�ص����� A 1 , A 2 , A 3 , �� , A n A_1, A_2, A_3, \dots, A_n A1?,A2?,A3?,��,An? �����һ�����ݽṹ��֧�������ѯ���� QueryMin(L,R) ������ min ? { A L , A L + 1 , �� , A R } \min \{A_L, A_{L+1}, \dots, A_R\} min{AL?,AL+1?,��,AR?} ��Ҳ����֧�� QueryMax(L,R) (ע�⣺��������䶼����ɢ�����µģ�ֻ��������)��

���ÿ����ѭ��������Ȼ�����죬 m �εĲ�ѯ��Ҫ O(mn) ��ʱ�临�Ӷȡ���ǰ׺�͵�˼��Ҳ��������Ч�ʣ���Ϊ�ⲻ��һ���ɼ�����Ϣ����ô���أ�
2. Sparse-Table

ʵ������õ��� Tarjan �� Sparse-Table �㷨������˵ST��(ϡ���)��һ�ּ򵥵����ݽṹ����Ҫ���ڴ���RMQ���⡣��ʹ�ñ�����˼�룬���� O ( n log ? n ) O(n\log n) O(nlogn) Ԥ���� O ( 1 ) O(1) O(1) ��ѯ�����ҳ�����С������Ҫ���ǣ�����㷨�ǳ���д������д��

ST��ʹ��һ����ά���� dp[][] �����ڷ�Χ�ڵ����� dp[a][b] ���Ƚ���Ԥ�������㲢�洢 min ? i �� [ a ,   a + 2 b ) ( A i ) \min\limits_{i\in[a,\ a+2^b)}(A_i) i��[a, a+2b)min?(Ai?) ����ѯʱ����������Щ��������������������ֵ��
(1) Ԥ����(��̬�滮)

Ϊ�˼���ʱ�䣬�ö�̬�滮����Ԥ���������� dp[i][j] ��ʾ�� i ��ʼ�ģ�����Ϊ 2 j 2^j 2j ��һ��Ԫ���е���Сֵ����׼�����ǣ�dp[i][0] = A[i] ����ʱ���� [i, i) ����Сֵ���� A i A_i Ai? ��

Ȼ��Ϳ����õ��Ƶķ�ʽ���� dp[i][j] �� d p [ i ] [ j ] = min ? { d p [ i ] [ j ? 1 ] , d p [ i + 2 j ? 1 ] [ j ? 1 ] } dp[i][j] = \min\{dp[i][j-1], dp[i+2^{j-1}][ j - 1]\} dp[i][j]=min{dp[i][j?1],dp[i+2j?1][j?1]} ��ԭ������ͼ��

�ѳ���Ϊ 2 j 2^j 2j ������  [i, i + 2^j - 1] [i,i+2j?1] �ֳ� [ i , i + 2 j ? 1 ? 1 ] [i, i + 2^{j-1} - 1] [i,i+2j?1?1] �� [ i + 2 j ? 1 , i + 2 j ? 1 ] [i + 2^{j-1}, i + 2^j - 1] [i+2j?1,i+2j?1] �������֡����ǰ��ն��壬dp[i][j] �͵���ǰ��ε���Сֵ dp[i][j - 1] �ͺ��ε���Сֵ dp[i + (1 << (j - 1))][j - 1] �������и�С�ߡ�

ע�� 2 j �� n 2^j \le n 2j��n ������ dp[][] �����Ԫ�ظ��������� n log ? n n\log n nlogn ��ÿһ������ڳ���ʱ�������ɣ������ʱ��Ϊ O ( n log ? n ) O(n\log n) O(nlogn) ���������£�

int dp[MAXN[21];	//�ڶ�ά�ȵĴ�С�������ݷ�Χȷ��,��С��log(MAXN)
//Ԫ�ر�Ŵ�1~n,��Ϊ�����1~n
int RMQ_init() {
	for (int i = 1; i <= n; ++i) dp[i][0] = A[i]; //��������,��׼
	for (int j = 1; (1 << j) <= n; ++j) 
		for (int i = 1; i + (1 << j) - 1 <= n; ++i)
			dp[i][j] = min(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
}


(2) �����ѯ

��ѯ�����ܼ򵥣��� k Ϊ���� 2 k �� R ? L + 1 2^k \le R-L+1 2k��R?L+1 ��������������� L L L ��ͷ���� R R R ��β����������Ϊ 2 k 2^k 2k ������������򸲸��˲�ѯ���� [ L , R ] [L, R] [L,R] ������ȡ��ֵ����ЩԪ���ظ�����Ҳû��ϵ������������ۼӣ��ظ�Ԫ���ǲ�����ģ�������ͼ��
���������ͼƬ����
��������ֻҪ�ҵ����� [L,R]�������䣬���ǵĲ���ǡ���� [ L , R ] [L, R] [L,R] �������ཻ���ǲ����Գ������䷶Χ������������ĳ��ȶ�Ϊ k k k ���ֱ��� [ L , L + 2 k ? 1 ] [L, L + 2^k - 1] [L,L+2k?1] �� [ R ? 2 k + 1 , R ] [R - 2^k + 1, R] [R?2k+1,R] ��ǰһ����������Ҷ˵㾡���ܽӽ� R R R ����һ�����������˵㾡���ܽӽ� L L L ����֪�� L + 2 k ? 1 = R L + 2^k - 1 = R L+2k?1=R ʱ���� k = log ? 2 ( R ? L + 1 ) k = \log_2(R-L+1) k=log2?(R?L+1) ����ʱ R ? 2 k + 1 = L R - 2^k + 1 = L R?2k+1=L Ҳ������

�����ȡ�������� k = ? log ? 2 ( R ? L + 1 ) ? k = \lfloor \log_2(R - L+1) \rfloor k=?log2?(R?L+1)? �����⣬ÿ�μ���log�е���£������logҲ����һ�ε���Ԥ����

for (int i = 2; i <= n; ++i)
	Log2[i] = Log2[i / 2] + 1;

�����������Ӧ�� dp[][] �ֱ��� dp[L][k] �� dp[R - (1 << k) + 1][k] �����Դ���ܺ�д��

int RMQ(int L, int R) {
	//int k = 0;
	//while ((1 << (k + 1)) <= R - L + 1) ++k;
	int k = Log2[R - L + 1];
	return min(dp[L][k], dp[R - (1 << k) + 1][k]);
}

3. ��չ

ST�����ܹ��������ֵ������Сֵ�����Ƿ��Ͻ�����ҿ��ظ����׵���Ϣ��ѯ��������ʹ��ST���Ч����

���ظ�������Ϊ��һ����Ԫ���� f ( x , y ) f(x, y) f(x,y) ������ f ( a , a ) = a f(a, a) = a f(a,a)=a ���� f f f �ǿ��ظ����׵ġ���Ȼ�����ֵ����Сֵ�������������С����������λ�򡢰�λ�붼������������������������ڣ��ܹ�������������Ϊ�յ����������Ϣ�ϲ���
4. ��Ŀ

��� P2880 [USACO07JAN]ƽ�������Balanced Lineup��
ԭ�����ӣ�https://blog.csdn.net/myRealization/article/details/108374025
#include <bits/stdc++.h>
using namespace std;
inline int read() {
	int x = 0, sign = 1;
	char ch = getchar();
	while (ch > '9' || ch < '0') {
	    if (ch == '-') sign = -1;
	    ch = getchar();
	}
	while (ch >= '0' && ch <= '9') {
	    x = x * 10 + ch - '0';
		ch = getchar();
	}
	return x * sign;
}
inline void write(int x) {
	if (x == 0) {
	    putchar(48);
		return;
	}
	int len = 0, dg[20];
	while (x > 0) {
		dg[++len] = x % 10;
		x /= 10;
	}
	for (register int i = len; i >= 1; --i)
		putchar(dg[i] + 48);
}

int f[MAXN][21]; // �ڶ�ά�Ĵ�С�������ݷ�Χ��������С��log(MAXN)
for (int i = 1; i <= n; ++i)
    f[i][0] = read(); // ��������
for (int i = 1; i <= 20; ++i)
    for (int j = 1; j + (1 << i) - 1 <= n; ++j)
        f[j][i] = max(f[j][i - 1], f[j + (1 << (i - 1))][i - 1]);
 
for (int i = 2; i <= n; ++i)
    Log2[i] = Log2[i / 2] + 1;


for (int i = 0; i < m; ++i)
{
    int l = read(), r = read();
    int s = Log2[r - l + 1];
    printf("%d\n", max(f[l][s], f[r - (1 << s) + 1][s]));
}



int dp[MAXN[21];	//�ڶ�ά�ȵĴ�С�������ݷ�Χȷ��,��С��log(MAXN)
//Ԫ�ر�Ŵ�1~n,��Ϊ�����1~n
int RMQ_init() {
	for (int i = 1; i <= n; ++i) dp[i][0] = A[i]; //��������,��׼
	for (int j = 1; (1 << j) <= n; ++j) 
		for (int i = 1; i + (1 << j) - 1 <= n; ++i) //�����ȳ��ȶ���i 
			dp[i][j] = min(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
}
for (int i = 2; i <= n; ++i)
	Log2[i] = Log2[i / 2] + 1;
int RMQ(int L, int R) {
	//int k = 0;
	//while ((1 << (k + 1)) <= R - L + 1) ++k;
	int k = Log2[R - L + 1];
	return min(dp[L][k], dp[R - (1 << k) + 1][k]);
}

int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}
