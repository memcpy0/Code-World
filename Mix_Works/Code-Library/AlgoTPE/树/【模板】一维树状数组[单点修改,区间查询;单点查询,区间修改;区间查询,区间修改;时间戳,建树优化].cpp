#include <bits/stdc++.h>
using namespace std;

#define lowbit ((x) & -(x))
int tree[MAXN];
inline void update(int i, int x) { //�����޸�
    for (int pos = i; pos < MAXN; pos += lowbit(pos))
        tree[pos] += x;
}
inline int query(int n) //��ǰn���
{
    int ans = 0;
    for (int pos = n; pos; pos -= lowbit(pos))
        ans += tree[pos];
    return ans;
}
inline int query(int a, int b) //�����ѯ
{
    return query(b) - query(a - 1);
}
//��ʼ����ʱ������ֻ��Ҫupdateÿ����ĳ�ʼֵ����
//------------------------------------------------------- 
//�����޸�+�����ѯ(�����ǵ����ѯ)
//�����x�Ķ����Ʊ�ʾ�д��������һ��1�����ֵ
#define lowbit ((x) & -(x)) 
//��������tree[],ak=ak+d
void add(int k, int d) { //�����޸�ԭ�����ͬʱ,ά����ԭ������������Ϣ
	while (k <= n) {  //ע����µ�tree[n]Ϊֹ
		tree[k] += d; //�޸ĺ�ak�йص�tree[]
		k += lowbit(k);
	}
}
//���sum=a1+a2+...+ak
int sum(int k) {
	int sum = 0;
	while (k > 0) { //����tree[0]���в���
		sum += tree[k];
		k -= lowbit[k];
	}
	return sum;
}
int queryRange(int l, int r) { //�����ѯ(�����ѯ�������ѯ������)
	return sum(r) - sum(l - 1);
}
//------------------------------------------------------
//�����ѯ+�����޸�(�����ǵ����޸�)
#define lowbit(x) ((x) & (-x))
typedef long long ll;
const int maxn = 5e5 + 10;
ll tree[maxn], n; 
void add(int i, ll d) { //�����޸Ĳ�������ͬʱ,ά���˲��������������Ϣ
	while (i <= n) {
		tree[i] += d;
		i += lowbit(i);
	}
}
int sum(int i) {	//ǰ׺�Ͳ�ѯ(�����ѯ) 
	int ret = 0;
	while (i) {
		ret += tree[i];
		i -= lowbit(i);
	}
	return ret;
}
void addRange(int l, int r, ll v) { //�����޸�(�����޸��������޸ĵ�����)
	add(l, v);   
	add(r + 1, -v);
}

int main() { 
	//...
	memset(tree, 0, sizeof(tree));
	ll pre = 0, now; 
	for (int i = 1; i <= n; ++i) {
		scanf("%lld", &now);
		add(i, now - pre);	//ά��������� 
		pre = now;
	}
	//...
    return 0;
}  
//------------------------------------------------------
//�����ѯ(�����ǵ����ѯ)+�����޸�(�����ǵ����޸�)
#define lowbit(x) ((x) & (-x))
const int maxn = 5e5 + 10;
int tree1[maxn], tree2[maxn], n; //t1ά��\sum d_i, t2ά��\sum d_i\times i
void add(int i, int d1) { //�����޸�������������ͬʱ,ά�����������������������Ϣ
	int d2 = i * d1;
	while (i <= n) {
		tree1[i] += d1, tree2[i] += d2;
		i += lowbit(i);
	}
} 
int sum(int *tree, int i) { //ָ����״�������ǰ׺�Ͳ�ѯ(��tree1ʹ��sum��Ϊ�����ѯ)  
	int ret = 0;
	while (i) {
		ret += tree[i];
		i -= lowbit(i);
	}
	return ret;
}
void addRange(int l, int r, int v) { //�����޸�(�����޸��������޸ĵ�����)
	add(l, v);       
	add(r + 1, -v);
}
int queryRange(int l, int r) { //�����ѯ
	return (r + l) * sum(tree1, r) - sum(tree2, r) //ǰ׺��sum[r]
	- (l * sum(tree1, l - 1) - sum(tree2, l - 1)); //ǰ׺��sum[l-1] 
}
//------------------------------------------------------
//������ÿһ���ڵ��ֵ�����������Լ�ֱ�������Ķ��ӵ�ֵ��͵õ��ġ�
//��˿��Ե��ſ��ǹ��ף���ÿ��ȷ������ӵ�ֵ�����Լ���ֵ������
//����ֱ�Ӹ��ס�
// O(n)����
void init() {
  for (int i = 1; i <= n; ++i) {
    t[i] += a[i];
    int j = i + lowbit(i);
    if (j <= n) t[j] += t[i];
  }
}
//------------------------------------------------------
/*
ʱ����Ż���
�Ը��������ݺܳ����ļ��ɡ����ÿ������������ʱ�������������״���飬�Ϳ��ܻ���ɳ�ʱ��
���ʹ��tag��ǣ��洢��ǰ�ڵ��ϴ�ʹ��ʱ�䣨�����һ���Ǳ��ڼ�������ʹ�ã���
ÿ�β���ʱ�ж����λ��tag�е�ʱ��͵�ǰʱ���Ƿ���ͬ��
�Ϳ����ж����λ��Ӧ���� 0 ���������ڵ�ֵ��
*/
int tag[maxn], tree[maxn], curtag = 0;
void inc() { ++curtag; }
void add(int i, int d) {
	while (i <= n) {
		if (tag[i] != curtag) tree[i] = 0;
		tree[i] += d, tag[i] = curtag;
		i += lowbit(i);
	}
}
int sum(int i) {
	int ret = 0;
	while (i) {
		if (tag[i] == curtag) ret += t[i];
		i -= lowbit(i);
	}
	return ret;
}
//ÿ�������������ݸı�tree[],Ȼ����ն���inc 
