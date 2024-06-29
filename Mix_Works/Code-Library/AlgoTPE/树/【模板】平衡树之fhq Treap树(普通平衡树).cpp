#include <bits/stdc++.h>
using namespace std;
//�ȼ��˽�һ����ͨTreap����ô����?
/*
Treap=Tree+Heap,����ƽ�����ϵ�ÿ�������������Ϣ:ֵ��һ��������ȼ�
����ֵ�������������������,���ȼ�����ѵ�����,��϶����������Ͷ���ѵ�������ʹ����ƽ��
�����Treap������
TreapΪʲô����ƽ��? ����֪�������һ�ö������������в����ֵ�������������,
��ô�����������ͻ��˻���һ������
��ô���ǿ��Ա�����ֵ���������,һ���ܺõķ����ǰѲ�����������
Treap�ö����ά��������ȼ�,���ǰѲ�����������,����һ��ֵ�����Ǳ�����
��������ȼ��������ѵ�����,�������ȼ��������,�ͻᵼ�²����ֵ��֪������������ȥ,
��ͬ�ڲ����������� 
��֮������󷨺ã�����һ�п�ʱ�临�ӶȵĶ������� 
fhq Treap����ȱ��:
- ����С,��д,���Ĳ����Ĵ��붼�Ǹ�����
- �����
- ֧�ֵĲ�����
ȱ��: ʱ�䳣���Դ�

��ֵĲ���?
��ͨTreap����ά����ƽ��Ĳ���������ת;
��������תʱ������fhq Treap 
������ֲ��������Ǻ��Ĳ���ֻ��������split���Ѻͺϲ�merge
ֻҪ��������������,�ͻ������Ѿ�������fhq Trap
���ǰ�����ƴ���ٲ�,��ƴͼһ��

fhq Treap�Ľ����Ϣ:
�����������
ֵ
������ȼ�
������С(����ʱ�õ�) 
*/ 
const int MAXN = 1e5 + 5;
struct Node {
	int l, r;
	int val, key;
	int size;
} fhq[MAXN];  
int cnt, root; //ʹ��0�Ž����ΪnullNode 
#include <random>
std::mt19937 rnd(233);
//�����½��͸�������Ϣ 
inline int newNode(int val) {
	fhq[++cnt].val = val;
	fhq[cnt].key = rnd();
	fhq[cnt].size = 1;
	return cnt;
}
/*
��������:��ֵ���ѺͰ���С����
��ֵ����:�������������,�������һ������ֵȫ��С�ڵ��ڸ�����ֵ,��һ���ֵ�ֵȫ�����ڸ�����ֵ
����С���ѵ�д��: �������������,����һ�����Ĵ�С���ڸ����Ĵ�С,ʣ�²���������һ������

һ����˵,ʹ��fhq Treap��Ϊһ��������ƽ�����õ�ʱ��,ʹ�ð�ֵ���� 
��ά��������Ϣ��ʱ��,ʹ�ð���С����,��������Ӿ�������ƽ����
*/
void sizeUpdate(int now) {
	fhq[now].size = fhq[fhq[now].l].size + fhq[fhq[now].r].size + 1;
}

void split(int now, int val, int &x, int &y) {
	if (!now) x = y = 0; //��ǰҪ���ѵĽ��,������������,�õ��������������ǲ�����
	else {
		if (fhq[now].val <= val) {
		 	x = now; //��ǰ����ֵ<=val,����ڵ����������������x
			split(fhq[now].r, val, fhq[now].r, y); 
		} else {
			y = now; //�����������еݹ���� 
			split(fhq[now].l, val, x, fhq[now].l);
		}
		sizeUpdate(now); //���������Ĵ�С 
	}
}
/*
�ϲ�:ֻ��һ��
��������x,y�ϲ���һ����,����x���ϵ�����ֵ��С�ڵ���y���ϵ�����ֵ,�ºϲ�������
��Ȼ����Treap������ 
���غϲ�����¸�
*/ 
inline int merge(int x, int y) {
	if (!x || !y) return x + y;
	if (fhq[x].key > fhq[y].key) {  //> >= < <= //����д���Ǵ����
		fhq[x].r = merge(fhq[x].r, y); //y��ֵ���ڵ���x��ֵ,��������������кϲ� 
		sizeUpdate(x);
		return x;
	}
	else {
		fhq[y].l = merge(x, fhq[y].l); //x��ֵС�ڵ���y��ֵ,�����y�����������кϲ� 
		sizeUpdate(y);
		return y;
	}		
}
/* ����
�����ֵΪval,��������:
����ֵval�������ѳ�x,y����: ���ѳ�����x���ϵ�����ֵһ����С�ڵ���val,y���ϵ�ֵһ�������ڵ���val 
�ϲ�x,�½��,y 
*/ 
int x, y, z;
inline void insert(int val) {
	split(root, val, x, y);
	root = merge(merge(x, newNode(val)), y);	
}

/* ɾ�� 
ɾ����ֵΪval,��Ҫ4��:
����ֵval�������ѳ�x��z
�ٰ�ֵval-1��x�����ѳ�x,y
��ô��ʱy���ϵ�����ֵ���ǵ���val��,ȥ�����ĸ��ڵ�:��y���ںϲ�y���������������� 
*/
inline void remove(int val) {
	split(root, val, x, z);
	split(x, val - 1, x, y);	
	y = merge(fhq[y].l, fhq[y].r);
	root = merge(merge(x, y), z);
}
/* ��ѯֵ������
Ҫ��ѯ��ֵΪval,��ô����val-1���ѳ�x,y
��x�Ĵ�С+1����val������ 
����ٰ�x��y�ϲ����� 
*/ 
inline void getRank(int val) {
	split(root, val - 1, x, y);
	printf("%d", fhq[x].size + 1);
	root = merge(x, y);
}
/* ��ѯ������ֵ
�����������������Ϳ����� 
*/
inline void getNum(int rank) {
	int now = root;
	while (now) {
		if (fhq[fhq[now].l].size + 1 == rank)
			break;
		else if (fhq[fhq[now].l].size >= rank)
			now = fhq[now].l;
		else {
			rank -= fhq[fhq[now].l].size + 1;
			now = fhq[now].r;
		}
	}
	printf("%d", fhq[rank].val);
}
/*ǰ��/���
������Ϊval
ǰ��: ��ֵval-1���ѳ�x��y,��x�������ҵ�������val��ǰ��
���: ����ֵval���ѳ�x��y,��y���������������val�ĺ�� 
*/ 
inline void pre(int val) {
	split(root, val - 1, x, y);
	int now = x;
	while (fhq[now].r) 
		now = fhq[now].r;
	printf("%d", fhq[now].val);
	root = merge(x, y);
}
inline void next(int val) {
	split(root, val, x, y);
	int now = x;
	while (fhq[now].l) 
		now = fhq[now].l;
	printf("%d", fhq[now].val);
	root = merge(x, y);
}

int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

