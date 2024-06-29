#include <bits/stdc++.h>
using namespace std;
/*Ī���㷨��һ�ֿ��Խ���󲿷�������������������㷨
��Ҫ�Ż��Ƿֿ�
Ī�ӵ�˼��
��ʵ�ܼ򵥣�ͨ��Ų������ķ�ʽ��ĳ��˳�����ߴ��������ѯ����
�������ؼ��ʣ�Ų�����䡢��ĳ��˳�����ߴ���

ʱ�临�Ӷ�Ϊ O(n\sqrt{n}),Ҳ��һ�ֽ������ĺ÷���
Ī�Ӻܺ�д 

������������һ����:����һ������,ÿ�θ���һ��ѯ��[L,R],�����������
ǿ����Ī����
��һ������洢����:
0 1   2  3  4  5   6  7  8  9 index   
  3   8  1  2  7   4  9  0  6 val
����֪��[2,5]����ĺ���18,��[2,6]����ĺ�,������?
��[2,5]����ĺͼ��ϵ������ֵ����,��:18+4=22
��[2,4]����ĺ���ô��
��[2,5]����ĺͼ�ȥ���е�5���ֵ����,��Ϊ18-7=11
����[3,6]����ĺ;Ϳ�����[2,5]����ͼ�ȥ�ڶ����ֵ�ټ��ϵ������ֵ���.
����������������

�Ե�ǰ����[L,R],�ֱ����������������:
1. ���ϵ�ǰ�������һ��Ĺ��ף�Add(--L); //����L���һ���ֵ
2. ���ϵ�ǰ�����ұ�һ��Ĺ��ף�Add(++R); //����R�ұ�һ���ֵ
3. ��ȥ��ǰ��������һ��Ĺ��ף�Sub(L++); //��ȥL��һ���ֵ
4. ��ȥ��ǰ��������һ��Ĺ��ף�Sub(R--); //��ȥR��һ���ֵ

���䳤�ȱ仯��������ϣ����е����䶼���Դӵ�ǰ��֪����Ľ����չ������

��������ԶԶ�����ģ�����һ��n�������,��������ѯ��m�Σ�
[1,2], [n-1,n], [1,2], [n-1,n] ...
ʱ�临�Ӷ�ΪO(mn)
Ī���㷨Ҫ����ı�Ҫ�������ұ���O(1)�ƶ���������,�����ӶȺ��Ǳ�ը��

���Ƕ���ͬ������Щѯ�ʣ���������µ�˳��:
[1,2], [1,2] ... [n-1,n], [n-1,n] ... 
ʱ�临�Ӷ�˲�䵽��O(m+n)
ѯ��˳���Ī���㷨��ʱ�临�Ӷ����źܴ��Ӱ�죡Ϊ�˽����һ���⣬��Ҫʹ�÷ֿ飡

���巽�����£����ȷֿ�,���С������ͨ��\sqrt{n},Ȼ������е�ѯ�ʽ�������
����Ĺ������£�����һ��ѯ��[L,R],��L���������ڿ�����,����L��ͬһ���,����R�������� 
�����,��������ѯ��,Ȼ���øո���˵�ķ����ó�����ѯ�ʵĴ𰸼�¼����, �����ԭ˳������𰸼���! 

��[1,n]�ֳ�\sqrt{n}��

�����Ȱ���Щ���䰴����˵�l���ڵĿ������������
�ٰ�l���ڿ���ͬ�����䰴r��С��������
*/

const int maxn = 5e4 + 5;
int a[maxn];	//��¼���е��� 
int pos[maxn]; //a�����еĵڼ����ǵڼ���� 
int ans[maxn]; //��¼�� 
//ѯ��
struct Q {
	int l, r, k;	//ѯ�ʵ�����[l,r], �ڼ���ѯ�� 
} q[maxn];
//��ż������ 
const int MAXN = 1e5 + 10;
int sqr = sqrt(n);
struct Q {
	int l, r, id;
	bool operator<(const Q &b) const { //����<�����,��ż������
		//ֻ��Ҫ֪��ÿ��Ԫ�ع������ĸ���,���СΪsqrt(n),����ֱ��l/sqrt(n)����
		if (l / sqr != b.l / sqr) return l < b.l;
		if (l / sqr & 1) //������
			return r < b.r;
		return r > b.r;
	}
} Q[MAXN];

int main() {
	int n, m;
	cin >> n >> m;
	//�ֿ�
	int size = sqrt(n); //��Ĵ�С 
	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
		pos[i] = i / size; //�ֿ� 
	}
	//ѯ��
	for (int i = 0; i < m; ++i) {
		cin >> q[i].l >> q[i].r;
		q[i].k = i;
	}
	//���е�ѯ�ʵ�˳���������
	sort(q, q + m, [](Q x, Q y) { return pos[x.l] == pos[y.l] ? x.r < y.r : pos[x.l] < pos[y.l] }); //ͬһ���鰴��r����,�����տ�˳����
	int l = 1, r = 0; //��ǰ��֪�ı�����[l,r]
	//�������е�ѯ�� 
	for (int i = 0; i < m; ++i) {
		while (q[i].l < l) Add(--l);
		while (q[i].r > r) Add(++r);
		while (q[i].l > l) Sub(l++);
		while (q[i].r < r) Sub(r--);
		//��¼�� 
		ans[q[i].k] = res;
	}
    return 0;
}

//----------------------Ī��+��ż������----------------------------------------------
const int MAXN = 30005, MAXQ = 200005, MAXM = 1000005;
int sq;
struct query // ��ѯ���Խṹ�巽ʽ����
{
    int l, r, id;
    bool operator<(const query &o) const // ����<���������ż������
    {
        // ����ֻ��Ҫ֪��ÿ��Ԫ�ع����ĸ��飬����Ĵ�С����sqrt(n)�����Կ���ֱ����l/sq
        if (l / sq != o.l / sq) 
            return l < o.l;
        if (l / sq & 1)
            return r < o.r;
        return r > o.r;
    }
} Q[MAXQ];
int A[MAXN], ans[MAXQ], Cnt[MAXM], cur, l = 1, r = 0;
inline void add(int p)
{
    if (Cnt[A[p]] == 0)
        cur++;
    Cnt[A[p]]++;
}
inline void del(int p)
{
    Cnt[A[p]]--;
    if (Cnt[A[p]] == 0)
        cur--;
}
int main()
{
    int n = read();
    sq = sqrt(n);
    for (int i = 1; i <= n; ++i)
        A[i] = read();
    int q = read();
    for (int i = 0; i < q; ++i)
        Q[i].l = read(), Q[i].r = read(), Q[i].id = i; // ��ѯ����������
    sort(Q, Q + q); // ����
    for (int i = 0; i < q; ++i)
    {
        while (l < Q[i].l)
            del(l++);
        while (l > Q[i].l)
            add(--l);
        while (r < Q[i].r)
            add(++r);
        while (r > Q[i].r)
            del(r--);
        ans[Q[i].id] = cur; // �����
    }
    for (int i = 0; i < q; ++i)
        printf("%d\n", ans[i]); // �����˳�����
    return 0;
}
//------------------------------------------------------------------
//��Ŀ���ӣ�https://www.spoj.com/problems/DQUERY/
//��ʵ����ȡ��pos�����¼��ŵ� 
#include <bits/stdc++.h>
using namespace std;
const int maxn = 3e4 + 10, maxq = 2e5 + 10, maxm = 1000005;
int a[maxn], cnt[maxm], n, q, sqr;
struct Q {
	int l, r, id;
	bool operator<(const Q &b) const {
		if (l / sqr != b.l / sqr) //�Ȱ�����߽�Ŀ�Ŵ�С�������� 
			return l < b.l;
		else if (l / sqr & 1) //������ʱ,�����������,�����ұ߽�r��С�������� 
			return r < b.r;
		return r > b.r;	//ż���� 
	}
} query[maxq];
int ans[maxq];
int res = 0;
void Add(int n) {
	if (cnt[a[n]] == 0) ++res;
	++cnt[a[n]];	
}
void Sub(int n) {
	--cnt[a[n]];
	if (cnt[a[n]] == 0) --res;
}

int main() { 
	scanf("%d", &n);
	sqr = sqrt(n);
	for (int i = 1; i <= n; ++i) 
		scanf("%d", &a[i]); 
	scanf("%d", &q);
	for (int i = 0; i < q; ++i) {
		scanf("%d%d", &query[i].l, &query[i].r);
		query[i].id = i;
	}
	sort(query, query + q);
	int l = 1, r = 0;
	for (int i = 0; i < q; ++i) {
		while (l > query[i].l) Add(--l);
		while (r < query[i].r) Add(++r);
		while (l < query[i].l) Sub(l++);
		while (r > query[i].r) Sub(r--);
		ans[query[i].id] = res;
	}
    for (int i = 0; i < q; ++i)
        printf("%d\n", ans[i]); // �����˳�����
    return 0;
}
