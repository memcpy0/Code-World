#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;
struct LeftistNode {
	int left, right, father; //左右儿子编号,father为并查集
	int val;	//结点键值,val==-1表示结点被删除 
	int dist;   //dis为结点的距离npl
} leftistTrees; //ltt 内存池写法和平衡树主席树相同
#define ls(x) leftistTrees[x].left
#define rs(x) leftistTrees[x].right //懒人专用define 
#define v(x)  leftistTrees[x].val  
#define d(x)  leftistTrees[x].dist
#define f(x)  leftistTrees[x].father

//初始化
leftistTrees[0].dist = -1; //平衡树、主席树的内存池写法都用0号结点代表空结点,距离为-1
for (int i = 1; i <= n; ++i) {
	leftistTrees[i].val = read(); //读入键值
	leftistTrees[i].father = i;   //初始化并查集
}
//合并操作
inline int merge(int x, int y) { //合并两个堆,x,y都是堆顶元素的编号
 	if (!x || !y) return x + y;  //如果有空的,就返回另一个,和fhq-treap相同
	//?或前语句为了维护小根堆性质,
	//或后语句是题目要求的,值相同则下标小的优先级高
	if (v(x) > v(y) || v(x) == v(y) && x > y)
		swap(x, y);  
	//合并右子树和y 
	rs(x) = merge(rs(x), y);  
	//维护并查集 x的新的右儿子的father会变化 
	f(rs(x)) = x;  
	//如果不满足左偏树的性质,就交换左右儿子 
	if (d(ls(x)) < d(rs(x))
		swap(ls(x), rs(x));
	d(x) = d(rs(x)) + 1; //利用结点距离等于右儿子距离+1更新dist
	return x;            //return合并好的堆顶结点编号即可
}
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;
struct LeftistNode {
	int left, right, father; //左右儿子编号,father为并查集
	int val;	//结点键值,val==-1表示结点被删除 
	int dist;   //dis为结点的距离npl
} leftistTrees; //ltt 内存池写法和平衡树主席树相同
#define ls(x) leftistTrees[x].left
#define rs(x) leftistTrees[x].right //懒人专用define 
#define v(x)  leftistTrees[x].val  
#define d(x)  leftistTrees[x].dist
#define f(x)  leftistTrees[x].father

//初始化
leftistTrees[0].dist = -1; //平衡树、主席树的内存池写法都用0号结点代表空结点,距离为-1
for (int i = 1; i <= n; ++i) {
	leftistTrees[i].val = read(); //读入键值
	leftistTrees[i].father = i;   //初始化并查集
}
//合并操作
inline int merge(int x, int y) { //合并两个堆,x,y都是堆顶元素的编号
 	if (!x || !y) return x + y;  //如果有空的,就返回另一个,和fhq-treap相同
	//?或前语句为了维护小根堆性质,
	//或后语句是题目要求的,值相同则下标小的优先级高
	if (v(x) > v(y) || v(x) == v(y) && x > y)
		swap(x, y);  
	//合并右子树和y 
	rs(x) = merge(rs(x), y);  
	//维护并查集 x的新的右儿子的father会变化 
	f(rs(x)) = x;  
	//如果不满足左偏树的性质,就交换左右儿子 
	if (d(ls(x)) < d(rs(x))
		swap(ls(x), rs(x));
	d(x) = d(rs(x)) + 1; //利用结点距离等于右儿子距离+1更新dist
	return x;            //return合并好的堆顶结点编号即可
}
//并查集和pop删除堆顶元素
int find(int x) { return f(x) == x ? x : f(x) == find(f(x)); }  //并查集和路径压缩
//pop删除
inline void pop(int x) { //删除堆顶元素  x没有别的父亲 
	v(x) = -1;   		 //值为-1表示被删除
	f(ls(x)) = ls(x);    //维护并查集,一个结点的父亲是结点本身,表示结点没有父亲了,它们都是堆顶 
	f(rs(x)) = rs(x);    //维护并查集
	//因为路径压缩,所以可能会有除了ls(x)和rs(x)以外的结点的father指针指向x(很多被压缩的结点也指向x)
	//在合并中调整,所以这样写,不让并查集断掉,考虑x的别的儿子怎么办
	//不这样写f(x)=...除了左右儿子的另外的儿子结点还是指向x,和左右儿子之间断开了 
	f(x) = merge(ls(x), rs(x));   //被删除的点的左右儿子合并后的新堆 
}
//-----------------------------------------------------------------------------
 
//合并,合并是左偏树最重要的操作，毕竟可并堆可并堆，肯定是要能够合并的。
int Merge(int x,int y)
{
    if(!x || !y)
        return x+y;
    if(v(x)>v(y) ||(v(x)==v(y) && x>y))
        swap(x,y);
    int &ls=l(x),&rs=r(x);
    rs=Merge(rs,y);
    f(rs)=x;
    if(d(ls)<d(rs))
        swap(ls,rs);
    d(x)=d(rs)+1;
    return x;
}

//deleteMin删除根节点
//只要先删除根节点，即将根节点的权值赋为-1, 
//其实有的时候不改权值也没影响），然后合并根节点的左右子树就可以了。
//删除根节点代码：
void Delroot(int x)
{
    int ls=l(x),rs=r(x);
    v(x)=-1,f(ls)=0,f(rs)=0;
    Merge(ls,rs);
}

//删除任意节点
//这里的任意节点指的是任意编号的节点而不是任意权值的节点，一般的可并堆是不支持删除给定权值节点的操作的。
//与删除根节点类似，先将要删除的节点的权值赋值为?1
//，然后合并它的左右子树，将合并后新的左偏树接到被删除节点的父节点上就可以了。
//但是与删除根节点不同的是，这个操作可能会导致整棵左偏树的左偏性质被破坏，
//因此要从该节点一直向上检查左偏性质，直到左偏性质没有被破坏或者到达了根节点。
//删除节点代码：
void Delete(int x)
{
    int fx=f(x),p=Merge(l(x),r(x));
    int &ls=l(fx),&rs=r(fx);
    f(p)=fx;
    ls==x?ls=p:rs=p;
    while(p)
    {
        if(d(ls)<d(rs))
            swap(ls,rs);
        if(d(fx)==d(rs)+1)
            return ;
        d(fx)=d(rs)+1;
        p=fx,fx=f(x);
        ls=l(fx),rs=r(fx);
    }
}

//建树
//暴力加点合并的话时间复杂度是O(nlogn),令人难以接受，因此我们需要一个比较高效的方法来实现建树。
//建树有以下几个步骤：
//1. 建立一个队列，将每个节点看作一个节点数为1的左偏树加入队列。
//2. 每次取出队头的两棵左偏树，将它们合并，并将合并后的新左偏树加入队列。
//3. 重复第2步，直到队列为空。
//建树代码
void Build()
{
    queue<int> q;
    for(int i=1;i<=n;i++)
        q.push(i);
    int x,y;
    while(q.size())
    {
        x=q.front();q.pop();
        y=q.front();q.pop();
        q.push(Merge(x,y));
    }
}


int main() {
    freopen(".in","r",stdin);
    freopen(".out","w",stdout);

    return 0;
}

