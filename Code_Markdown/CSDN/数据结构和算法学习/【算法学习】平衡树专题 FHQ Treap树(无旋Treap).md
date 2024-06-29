@[toc]

---
# 1. 前言
二叉搜索树的一个致命缺点在于：如果插入值按次序是有序的，那么二叉树就会退化为一个链表，从而所有操作的时间复杂度都会退化成 $\text{O(N)}$ 。为了解决这样的情况，就到了平衡树出马的时间了。

平衡树，就是一个平衡的二叉搜索树。那什么是平衡呢？不同的平衡树的定义不太一样。

对一棵不让人满意的二叉搜索树进行一些**奇特的操作**，使得它能够一直维持"好身材"(平衡状态)，不因为插入删除而变形，这样的二叉搜索树，就是平衡树。此时，**平衡树仍然是一棵二叉搜索树**，满足二叉搜索树的所有性质。

这些奇特操作可以分为有旋转、无旋转两类：
- 有旋转：包括AVL、Splay、红黑树等；
- 无旋转：替罪羊树、fhq Treap等

我们用到的**大部分平衡树的操作都基于树旋转操作**。注意，**二叉搜索树旋转前后，中序遍历的结果不变**。要知道的是，**树旋转中基本的操作就是左旋和右旋**，将它们组合执行就是双旋转。一旦掌握了这些操作，就掌握了大部分平衡树一半的内容。

当然，还有无旋转的平衡树，它们在**理解难度**和**代码量**这两个方面都吊打有旋转平衡树。但是有旋平衡树也有很大的用处，<font color="red">**红**</font><font color="black">**黑**</font><font color="#00ff00">**树**</font>作为目前为止最强的平衡树，其地位迄今无可撼动。STL的 `set, map` 就是用它实现的。虽说如此，它们的差别并不大，各种平衡树的操作时间大多数为 $\text{O(logN)}$ 。

在这些平衡树之中，很容易用到的就是**Splay**和**fhq Treap**。由于**fhq Treap**代码量小、没有旋转、易于理解，而且之前学过了**Treap**，这里就来学习一下**fhq Treap**。


----
# 2. FHQ Treap简介
它可以完成一切二叉搜索树、Treap、Splay Tree能做的操作，可以说是Treap的变异强化版。

FHQ Treap中只有两个基本操作：
1. 分裂 `Split` ：把一棵树分成两棵树；
2. 合并 `Merge` ：把两棵树合并为一棵树。


## (1) 
```cpp
struct Node {
	int ch[3]; //0左孩子,1右孩子
	int val;  //每一个点的键值
	int pri;  //随机生成的附件权值
	int size; //以i为节点的树的节点数量
	int sz; //总结点的数量 
} fhq[MAXN];
```
## (1) 插入
```cpp
split(root, a, x, y);
root = merge(merge(x, new_node(a)), y);
```
把树先分成 `x, y` 两个子树，然后把新的结点 `a` 看成一棵子树，先和 `x` 合并，合并的整体和 `y` 合并。
## (2) 删除根
```cpp
split(root, a, x, z);
split(x, a - 1, x, y);
y = merge(ch[y][0], ch[y][1]);
root = merge(merge(x, y), z);
```
首先把树分成 `x, z` 两个部分。`x` 树中最大的权值为 `a` ，再把 `x` 分成 `x, y` 两个部分，此时 `x` 中最大权值为 `a-1` ，且 `a` 权值的结点一定是 `y` 的根结点。

然后无视 `y` 的根结点，直接把 `y` 的左右孩子合并起来，就成功删除了根结点，最后再把 `x, y, z` 合并起来就好了。

## (3) 查询a的排名
```cpp
split(root, a - 1, x, y);
printf("%d\n", siz[x] + 1);
root = merge(x, y);
```
首先按照 `a-1` 的权值把树分开；
然后 `x` 树中最大的应该就是 `a-1` ，`a` 的排名就是 `siz[x] + 1` 。
## (3) 查询排名为a的数 
```cpp
printf("%d\n",val[kth(root,a)]); 
```
直接调用查找排名的函数即可，

## 5.求x的前驱(前驱定义为小于a，且最大的数)
```cpp
split(root,a-1,x,y);
printf("%d\n",val[kth(x,siz[x])]);
root=merge(x,y);
```
因为要小于a，那么我们按照a-1的权值划分，

x中最大的一定是<=a-1的，

所以我们直接输出x中最大的数就好，

（这里有一个小技巧，因为siz储存的是节点的数目，然后根据二叉查找树的性质，编号最大的就是值最大的）

6.求x的后继(后继定义为大于x，且最小的数)
```cpp
split(root,a,x,y);
printf("%d\n",val[kth(y,1)]);
root=merge(x,y);
```

#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<cstdlib>
#include<ctime>
using namespace std;
const int MAXN=100001;
static void read(int &n)
{
    char c='+';int x=0;bool flag=0;
    while(c<'0'||c>'9'){c=getchar();if(c=='-')flag=1;}
    while(c>='0'&&c<='9'){x=(x<<1)+(x<<3)+(c-48);c=getchar();}
    flag==1?n=-x:n=x;
}
int ch[MAXN][3];// 0左孩子 1右孩子
int val[MAXN];// 每一个点的权值
int pri[MAXN];// 随机生成的附件权值
int siz[MAXN];// 以i为节点的树的节点数量
int sz;// 总结点的数量
void update(int x)
{
    siz[x]=1+siz[ch[x][0]]+siz[ch[x][1]];
}
int new_node(int v)
{
    siz[++sz]=1;// 新开辟一个节点
    val[sz]=v;
    pri[sz]=rand();
    return sz;
}
int merge(int x,int y)// 合并
{
    if(!x||!y)    return x+y;// x和y中必定有一个是0
    if(pri[x]<pri[y])// 把x加到左边的树上
    {
        ch[x][1]=merge(ch[x][1],y);// 不懂的看GIF图
        update(x);
        return x;
    }
    else
    {
        ch[y][0]=merge(x,ch[y][0]);
        update(y);
        return y;
    }
}
void split(int now,int k,int &x,int &y)
{
    if(!now) x=y=0;// 到达叶子节点
    else
    {
        if(val[now]<=k)// 分离右子树
            x=now,split(ch[now][1],k,ch[now][1],y);
        else
            y=now,split(ch[now][0],k,x,ch[now][0]);
        update(now);
    }
}
int kth(int now,int k)// 查询排名
{
    while(1)
    {
        if(k<=siz[ch[now][0]])
            now=ch[now][0];// 在左子树中，且数量小于左子树的大小，迭代寻找
        else if(k==siz[ch[now][0]]+1)
            return now;// 找到了
        else
            k-=siz[ch[now][0]]+1,now=ch[now][1];// 去右子树找
    }
}
int main()
{
    srand((unsigned)time(NULL));
    int n;
    read(n);
    int root=0,x,y,z;
    for(int i=1;i<=n;i++)
    {
        int how,a;
        read(how);read(a);
        if(how==1)// 插入
        {
            split(root,a,x,y);
            root=merge(merge(x,new_node(a)),y);
        }
        else if(how==2)//删除x
        {
            split(root,a,x,z);
            split(x,a-1,x,y);
            y=merge(ch[y][0],ch[y][1]);
            root=merge(merge(x,y),z);
        }
        else if(how==3)//查询x的排名
        {
            split(root,a-1,x,y);
            printf("%d\n",siz[x]+1);
            root=merge(x,y);
        }
        else if(how==4)// 查询排名为x的数
        {
            printf("%d\n",val[kth(root,a)]);
        }
        else if(how==5)// 求x的前驱
        {
            split(root,a-1,x,y);
            printf("%d\n",val[kth(x,siz[x])]);
            root=merge(x,y);
        }
        else if(how==6)// 求x的后继
        {
            split(root,a,x,y);
            printf("%d\n",val[kth(y,1)]);
            root=merge(x,y);
        }
    }
    return 0;
}
