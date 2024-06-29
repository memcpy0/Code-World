#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
/*
���������˼��������������µ����丳ֵ�����ܿ����ô���Ԫ�ر�Ϊͬһ������
������������Ԫ��<l,r,v>����ʽ�������ݣ����� [l,r] �е�Ԫ�ص�ֵ����v����
*/ 
struct node {
	ll l, r;
	mutable ll v; //mutableҪд��Ȼ���ܻ�CE 
	node(ll l, ll r, ll v) : l(l), r(r), v(v) { } //���캯��
	bool operator<(const node &o) const { return l < o.l; } //����С�������
};
set<node> kdl; //�������
/*
Ҫ�ѽṹ��Ž�set����Ҫ����С���������set�ᱣ֤�ڲ�Ԫ�����򣨲��롢ɾ���Ͳ�ѯ��ʱ�临�Ӷȶ��� 
O(logn)��
��mutableʹ�õ������ṹ��Ϊconstʱ����Ϊmutable�ĳ�Ա�Կɱ䣨��Ϊ����������ӵȲ�������

Ȼ�������ǽ����������ʱ����������ô���ˣ����ܻ��ԭ������������Ͽ���
������Ҫһ��������ʵ�֡��Ͽ����Ĳ�������<l,r,v>�ϳ�<l,pos-1,v>��<pos,r,v>��
*/
auto split(ll pos) { //set<node>::iterator //֧��C++14����
	auto it = kdl.lower_bound(node(pos, 0, 0)); //Ѱ����˵���ڵ���pos�ĵ�һ�����
	//�����֧��C++11��auto��Ҫ��Ϊset<node>::iterator 
	if (it != kdl.end() && it->l == pos) //����Ѿ�������posΪ��˵�Ľ�㣬ֱ�ӷ���
		return it; 
	it--; //������ǰһ�����
	ll l = it->l, r = it->r, v = it->v;
	kdl.erase(it); //ɾ���ý��
	kdl.insert(node(l, pos - 1, v)); //����<l, pos - 1, v>��<pos, r, v>
	return kdl.insert(node(pos, r, v)).first; //������pos��ͷ���Ǹ����ĵ�����
	//insertĬ�Ϸ���ֵ��һ��pair,��һ����Ա��������Ҫ�� 
}
//��������ľ����������丳ֵ�������丳ֵ������д��Ҳ�����
void assign(ll l, ll r, ll v) {
	auto end = split(r + 1), begin = split(l); //˳���ܵߵ�,�������RE
	kdl.erase(begin, end); //���һϵ�н��
	kdl.insert(node(l, r, v)); //�����µĽ��	
/*
�޷Ǿ��ǰѷ�Χ�ڵĽڵ�ȫ��ɾ����Ȼ�����µģ���Χ�ϴ�ģ��ڵ���ѡ�ֻ����Ҫע����end��begin��˳���ܵߵ���
��Ϊsplit(end)���ܰ�beginԭ�����ڵĽڵ�Ͽ���
*/
}
//����ӣ������ӣ�
void add(ll l, ll r, ll v) {
	auto end = split(r + 1);
	for (auto it = split(l); it != end; ++it)
		it->v += v;
}
//������k��ֵ��ֱ�ӵ�vector��������
ll kth(ll l, ll r, ll k) {
	auto end = split(r + 1);
	vector<pair<ll, ll>> v; //���pair�д洢����ֵ�����䳤��
	for (auto it = split(l); it != end; ++it) 
		v.push_back(make_pair(it->v, it->r - it->l + 1));
	sort(v.begin(), v.end()); //ֱ�Ӱ��ս���ֵ�Ĵ�С������
	for (int i = 0; i < v.size(); ++i) { //Ȼ�󰤸���������ֱ������k��Ԫ��Ϊֹ
		k -= v[i].second;
		if (k <= 0)
			return v[i].first;
	}
}
//������n�η��ͣ��ÿ�����ֱ����
ll sum_of_pow(ll l, ll r, ll x, ll y) {
	ll tot = 0;
	auto end = split(r + 1);
	for (auto it = split(l); it != end; ++it) 
		tot = (tot + fastPow(it->v, x, y) * (it->r - it->l + 1)) % y; //������
	return tot; 
}

int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

