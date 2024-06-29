#include <bits/stdc++.h>
using namespace std;

//fhq Treap���ʵ���������
/*
Ҫ������������[l,r]
��ô������fhq Treap�а���һ��ֱ��split��������в���Ȼ��ϲ���ȥ
����С���ѵ�д��: �������������,����һ�����Ĵ�С���ڸ����Ĵ�С,ʣ�²���������һ������
���岽��:
��fhq Treap���մ�Сl-1��ֳ�x��y
�ٰ�y����Сr-l+1��ֳ�y��z
��ʱy����������Ҫ�����������ƽ����,�������в����ͺ���
����xyz�ϲ���ȥ
 
��ν������ƽ�������⣿
ʹ���߶����г����ĸ�������(lazy Tag)
ÿ��������Ҫ��һ��������з�תʱ,���ǾͶ��������ά�������:���ԭ��û�������
�ʹ���;���ԭ���������,��ȥ��,��Ϊ��ת���ε���û�з�ת.
������д����ǵ��´�����,�����ǰ�������������ں����������б����ĵķ��յĻ�,���´�,�Է���һ 
*/ 
const int MAXN = 1e5 + 10;
struct node {
	int l, r;
	int val, key;
	int size;
	bool reverse; //�����,֤����������Ƿ񱻷�ת 
} fhq[MAXN];
int cnt, root;
#include <random>
std::mt19937 rnd(233);

inline int newNode(int val) {
	fhq[++cnt].val = val;
	fhq[cnt].key = rnd();
	fhq[cnt].size = 1;
	return cnt;
}
inline void sizeUpdate(int now) {
	fhq[now].size = fhq[fhq[now].l].size + fhq[fhq[now].r].size + 1;
}
//����ǵ��´�
inline void pushdown(int now) {
	//��������������ת
	swap(fhq[now].l, fhq[now].r);
	fhq[fhq[now].l].reverse ^= 1; //false=true;true=false
	fhq[fhq[now].r].reverse ^= 1;
	fhq[now].reverse = false;     //�ѵ�ǰ���������ȥ������false 
}

inline void split(int now, int siz, int &x, int &y) {
	if (!now) x = y = 0;
	else {
		if (fhq[now].reverse) pushdown(now);
		if (fhq[fhq[now].l].size < siz) {
			x = now; //�����һ����x 
			split(fhq[now].r, siz - fhq[fhq[now].l].size - 1, fhq[now].r, y); //��ǰ���+������ 
		} else {
			y = now;
			split(fhq[now].l, siz, x, fhq[now].l);
		}
	}
}

inline int merge(int x, int y) {
	if (!x || !y) return x + y;
	if (fhq[x].key < fhq[y].key) {
		if (fhq[x].reverse) pushdown(x);
		fhq[x].r = merge(fhq[x].r, y); //y��ֵ���ڵ���x��ֵ,��������������кϲ� 
		sizeUpdate(x);
		return x;
	}
	else {
		if (fhq[y].reverse) pushdown(y);
		fhq[y].l = merge(x, fhq[y].l); //x��ֵС�ڵ���y��ֵ,�����y�����������кϲ� 
		sizeUpdate(y);
		return y;
	}	
}

void reverse(int l, int r) {
	int x, y, z;
	split(root, l - 1, x, y);
	split(y, r - l + 1, y, z);
	//��ʱy����������Ҫ��ת������
	fhq[y].reverse ^= 1;
	root = merge(merge(x, y), z);
}

void inorder(int now) {
	if (!now) return;
	if (fhq[now].reverse) pushdown(now);
	inorder(fhq[now].l);
	printf("%d ", fhq[now].val);
	inorder(fhq[now].r);
}

int main() {
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) { 
		root = merge(root, newNode(i));
	}
	while (m--) {
		int l, r;
		cin >> l >> r;
		reverse(l, r);
	}
	ldr(root);
    return 0;
}
