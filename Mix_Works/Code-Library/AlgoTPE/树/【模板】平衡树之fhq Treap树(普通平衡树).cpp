#include <bits/stdc++.h>
using namespace std;
//先简单了解一下普通Treap是怎么回事?
/*
Treap=Tree+Heap,它让平衡树上的每个结点存放两个信息:值和一个随机优先级
其中值满足二叉搜索树的性质,优先级满足堆的性质,结合二叉搜索树和二叉堆的性质来使得树平衡
这就是Treap的性质
Treap为什么可以平衡? 我们知道如果对一棵二叉搜索树进行插入的值按次序是有序的,
那么二叉搜索树就会退化成一个链表
那么我们可以别让数值按次序插入,一个很好的方法是把插入次序随机化
Treap用二叉堆维护随机优先级,就是把插入次序随机化,插入一个值后我们必须让
随机的优先级满足二叉堆的特性,但是优先级是随机的,就会导致插入的值不知道调整到哪里去,
等同于插入次序随机了 
总之，随机大法好，吊打一切卡时间复杂度的毒瘤数据 
fhq Treap的优缺点:
- 码量小,好写,核心操作的代码都是复读机
- 好理解
- 支持的操作多
缺点: 时间常数略大

奇怪的操作?
普通Treap用来维护树平衡的操作是树旋转;
不会树旋转时可以用fhq Treap 
它的奇怪操作并且是核心操作只有两个：split分裂和合并merge
只要掌握这两个操作,就基本上已经掌握了fhq Trap
就是把树拆开拼上再拆开,像拼图一样

fhq Treap的结点信息:
左右子树编号
值
随机优先级
子树大小(排名时用到) 
*/ 
const int MAXN = 1e5 + 5;
struct Node {
	int l, r;
	int val, key;
	int size;
} fhq[MAXN];  
int cnt, root; //使用0号结点作为nullNode 
#include <random>
std::mt19937 rnd(233);
//建立新结点和赋予结点信息 
inline int newNode(int val) {
	fhq[++cnt].val = val;
	fhq[cnt].key = rnd();
	fhq[cnt].size = 1;
	return cnt;
}
/*
分裂两种:按值分裂和按大小分裂
按值分裂:把树拆成两棵树,拆出来的一棵树的值全部小于等于给定的值,另一部分的值全部大于给定的值
按大小分裂的写法: 把树拆成两棵树,其中一棵树的大小等于给定的大小,剩下部分在另外一棵树中

一般来说,使用fhq Treap作为一棵正常的平衡树用的时候,使用按值分裂 
在维护区间信息的时候,使用按大小分裂,经典的例子就是文艺平衡树
*/
void sizeUpdate(int now) {
	fhq[now].size = fhq[fhq[now].l].size + fhq[fhq[now].r].size + 1;
}

void split(int now, int val, int &x, int &y) {
	if (!now) x = y = 0; //当前要分裂的结点,它不存在子树,得到的两棵子树还是不存在
	else {
		if (fhq[now].val <= val) {
		 	x = now; //当前结点的值<=val,则根节点和其左子树都属于x
			split(fhq[now].r, val, fhq[now].r, y); 
		} else {
			y = now; //对左子树进行递归分裂 
			split(fhq[now].l, val, x, fhq[now].l);
		}
		sizeUpdate(now); //更新子树的大小 
	}
}
/*
合并:只有一种
把两棵树x,y合并成一棵树,其中x树上的所有值都小于等于y树上的所有值,新合并出的树
依然满足Treap的性质 
返回合并后的新根
*/ 
inline int merge(int x, int y) {
	if (!x || !y) return x + y;
	if (fhq[x].key > fhq[y].key) {  //> >= < <= //这里写的是大根堆
		fhq[x].r = merge(fhq[x].r, y); //y的值大于等于x的值,因此用右子树进行合并 
		sizeUpdate(x);
		return x;
	}
	else {
		fhq[y].l = merge(x, fhq[y].l); //x的值小于等于y的值,因此用y的右子树进行合并 
		sizeUpdate(y);
		return y;
	}		
}
/* 插入
插入的值为val,步骤如下:
按照值val把树分裂成x,y两棵: 分裂出来的x树上的所有值一定都小于等于val,y树上的值一定都大于等于val 
合并x,新结点,y 
*/ 
int x, y, z;
inline void insert(int val) {
	split(root, val, x, y);
	root = merge(merge(x, newNode(val)), y);	
}

/* 删除 
删除的值为val,需要4步:
按照值val把树分裂成x和z
再按值val-1把x树分裂成x,y
那么此时y树上的所有值都是等于val的,去掉它的根节点:让y等于合并y的左子树和右子树 
*/
inline void remove(int val) {
	split(root, val, x, z);
	split(x, val - 1, x, y);	
	y = merge(fhq[y].l, fhq[y].r);
	root = merge(merge(x, y), z);
}
/* 查询值的排名
要查询的值为val,那么按照val-1分裂成x,y
则x的大小+1就是val的排名 
最后再把x和y合并起来 
*/ 
inline void getRank(int val) {
	split(root, val - 1, x, y);
	printf("%d", fhq[x].size + 1);
	root = merge(x, y);
}
/* 查询排名的值
和替罪羊树的做法就可以了 
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
/*前驱/后继
操作数为val
前驱: 按值val-1分裂成x和y,则x里面最右的数就是val的前驱
后继: 按照值val分裂成x和y,则y里面最左的数就是val的后继 
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

