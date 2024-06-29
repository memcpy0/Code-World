#include <bits/stdc++.h>
using namespace std;
struct Node {
	int val;
	int son[2];	//左右儿子 
} AVL[MAXN];
int CNT, ROOT;

inline void rotate(int &now, int d) { //d=0为左旋,d=1为右旋 
	int t = AVL[now].son[d ^ 1];
	AVL[now].son[d ^ 1] = AVL[t].son[d]; 
	AVL[t].son[d] = now;
	now = r;
	update(AVL[now].son[d]); update(now); 
}
///////////////////////////////////////////////////////////////////// 
const int MAXN = 10000;
//洛谷平衡树模板题 
struct node {
	int val, l, r; 
	int height, size; //高度和树的大小 
} avl[MAXN];
int cnt, root;
inline void newNode(int &now, int val) {
	avl[++cnt] = val;
	avl[cnt].size = 1;
	//avl[cnt].l = avl[cnt].r = 0; 
}

inline void update(int now) {	//高度和大小更新 
	avl[now].size = avl[avl[now].l].size + avl[avl[now].r].size + 1; 
	avl[now].height = max(avl[avl[now].l].height, avl[avl[now].r].height) + 1;
}

inline int factor(int now) {   //求平衡因子(可以不用计算它) 
	return avl[avl[now].l].height - avl[avl[now].r].height;
}

inline void lrotate(int &now) { //左旋 
	int t = avl[now].r;			//右子树 
	avl[now].r = avl[t].l;      //左挂右 
	avl[t].l = now;				//提右 
	now = r;					//更新根结点 
	update(avl[now].l), update(now);
}
inline void rrotate(int &now) { //右旋
	int t = avl[now].l;			//左子树
	avl[now].l = avl[t].r;		//右挂左
	avl[t].r = now;				//提左 
	now = r;					//更新根结点 
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
	else if (now) update(now);                  //平衡且非空,直接更新 
}

void insert(int &now, int val) {
	if (!now) newNode(now, val); 				//不存在的结点,新建结点
	else if (val < avl[now].val) insert(avl[now].l, val); //小的往左 
	else insert(avl[now].r, val);				//大的往右 
	check(now);									//递归自底向上回溯检查平衡和进行调整 
}
/*
删除的方法和二叉搜索树相同,递归回溯时检查一下是否需要调整
在删除一个有两个儿子的结点时,删除方法有两种: 先把结点旋转到叶结点再直接删除;或者按照二叉搜索树的删除方法删除
下面使用的是二叉搜索树的删除方法: 找到它的后继用后继替代它
然后递归删除后继的那一部分子树 
*/ 
void remove(int &now, int val) {
	if (val == avl[now].val) { //要删除的结点
		int l = avl[now].l, r = avl[now].r;  //temp
		if (!l || !r) now = l + r; //无儿子或者单儿子,就直接指向空或者存在的单儿子
		else {
			now = findPost(r, r);  //找后继替换当前 
			if (now != r) 		   //当前结点存在后继,不等于原来的右子树 
				avl[now].r = r;    //右儿子改成原来的右儿子
 			avl[now].l = l; 	   //左儿子连上 
		}
	} 
	else if (val < avl[now].val) remove(avl[now].l, val);
	else remove(avl[now].r, val);
	check(now);						//递归回溯检查和调整 
	//还可以用后继的值替代要删除结点的值,然后递归删除子树中的后继 
}

int findPost(int now, int fa) {		//找到后继
	int ret;
	if (!avl[now].l) {				//没有左儿子就找到了后继
		ret = now;					//返回后继
		avl[fa].l = avl[now].r;		//父亲的左儿子改成结点的右儿子,删除了这个结点
	} else {						//还有左儿子
		ret = find(avl[now].l, now);//返回值=继续find
		check(now);					//自底向上回溯进行检查和调整
	}
	return ret;						//找到的结点编号返回 
}
//找排名的值,值的排名,前驱,后继
//和替罪羊树相同,不过不是惰性删除所以所有的tzy[now].exist都改成了1,见代码仓库 
//https://github.com/StableAgOH/Code-Warehouse-for-AgOH 
int main() {

    return 0;
}

