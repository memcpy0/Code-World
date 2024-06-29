#include <bits/stdc++.h>
using namespace std;
struct Node {
	int val;
	int son[2];	//���Ҷ��� 
} AVL[MAXN];
int CNT, ROOT;

inline void rotate(int &now, int d) { //d=0Ϊ����,d=1Ϊ���� 
	int t = AVL[now].son[d ^ 1];
	AVL[now].son[d ^ 1] = AVL[t].son[d]; 
	AVL[t].son[d] = now;
	now = r;
	update(AVL[now].son[d]); update(now); 
}
///////////////////////////////////////////////////////////////////// 
const int MAXN = 10000;
//���ƽ����ģ���� 
struct node {
	int val, l, r; 
	int height, size; //�߶Ⱥ����Ĵ�С 
} avl[MAXN];
int cnt, root;
inline void newNode(int &now, int val) {
	avl[++cnt] = val;
	avl[cnt].size = 1;
	//avl[cnt].l = avl[cnt].r = 0; 
}

inline void update(int now) {	//�߶Ⱥʹ�С���� 
	avl[now].size = avl[avl[now].l].size + avl[avl[now].r].size + 1; 
	avl[now].height = max(avl[avl[now].l].height, avl[avl[now].r].height) + 1;
}

inline int factor(int now) {   //��ƽ������(���Բ��ü�����) 
	return avl[avl[now].l].height - avl[avl[now].r].height;
}

inline void lrotate(int &now) { //���� 
	int t = avl[now].r;			//������ 
	avl[now].r = avl[t].l;      //����� 
	avl[t].l = now;				//���� 
	now = r;					//���¸���� 
	update(avl[now].l), update(now);
}
inline void rrotate(int &now) { //����
	int t = avl[now].l;			//������
	avl[now].l = avl[t].r;		//�ҹ���
	avl[t].r = now;				//���� 
	now = r;					//���¸���� 
	update(avl[now].r), update(now);
}

inline void check(int &now) {
	int nf = factor(now);
	if (nf > 1) {
		int lf = factor(avl[now].l);
		if (lf > 0) rrotate(now);			    //LL
		else lrotate(avl[now].l), rrotate(now); //LR
	} 
	else if (nf < -1) {
		int rf = factor(avl[now].r);
		if (rf < 0) lrotate(now);				//RR
		else lrotate(avl[now].r), lrotate(now); //RL
	}
	else if (now) update(now);                  //ƽ���ҷǿ�,ֱ�Ӹ��� 
}

void insert(int &now, int val) {
	if (!now) newNode(now, val); 				//�����ڵĽ��,�½����
	else if (val < avl[now].val) insert(avl[now].l, val); //С������ 
	else insert(avl[now].r, val);				//������� 
	check(now);									//�ݹ��Ե����ϻ��ݼ��ƽ��ͽ��е��� 
}
/*
ɾ���ķ����Ͷ�����������ͬ,�ݹ����ʱ���һ���Ƿ���Ҫ����
��ɾ��һ�����������ӵĽ��ʱ,ɾ������������: �Ȱѽ����ת��Ҷ�����ֱ��ɾ��;���߰��ն�����������ɾ������ɾ��
����ʹ�õ��Ƕ�����������ɾ������: �ҵ����ĺ���ú�������
Ȼ��ݹ�ɾ����̵���һ�������� 
*/ 
void remove(int &now, int val) {
	if (val == avl[now].val) { //Ҫɾ���Ľ��
		int l = avl[now].l, r = avl[now].r;  //temp
		if (!l || !r) now = l + r; //�޶��ӻ��ߵ�����,��ֱ��ָ��ջ��ߴ��ڵĵ�����
		else {
			now = findPost(r, r);  //�Һ���滻��ǰ 
			if (now != r) 		   //��ǰ�����ں��,������ԭ���������� 
				avl[now].r = r;    //�Ҷ��Ӹĳ�ԭ�����Ҷ���
 			avl[now].l = l; 	   //��������� 
		}
	} 
	else if (val < avl[now].val) remove(avl[now].l, val);
	else remove(avl[now].r, val);
	check(now);						//�ݹ���ݼ��͵��� 
	//�������ú�̵�ֵ���Ҫɾ������ֵ,Ȼ��ݹ�ɾ�������еĺ�� 
}

int findPost(int now, int fa) {		//�ҵ����
	int ret;
	if (!avl[now].l) {				//û������Ӿ��ҵ��˺��
		ret = now;					//���غ��
		avl[fa].l = avl[now].r;		//���׵�����Ӹĳɽ����Ҷ���,ɾ����������
	} else {						//���������
		ret = find(avl[now].l, now);//����ֵ=����find
		check(now);					//�Ե����ϻ��ݽ��м��͵���
	}
	return ret;						//�ҵ��Ľ���ŷ��� 
}
//��������ֵ,ֵ������,ǰ��,���
//������������ͬ,�������Ƕ���ɾ���������е�tzy[now].exist���ĳ���1,������ֿ� 
//https://github.com/StableAgOH/Code-Warehouse-for-AgOH 
int main() {

    return 0;
}

