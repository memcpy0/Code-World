#include <bits/stdc++.h>
using namespace std;
/*
当我们处理树上点和点关系的问题（最简单的就是树上两点的距离）时，往往需要得到树上两点的最近公共祖先
LowestCommonAncestor，`LCA`。如下图： 
https://pic2.zhimg.com/80/v2-99457646a2f9b511f82dd244eb76ba2c_720w.jpg
朴素的做法是：
①进行一次dfs，求出每个点的深度
//链式前向星 
//遍历结点i的所有邻接点  
const int MAXN = 1e6;
struct Edge {
	int to, next, w;	//边:终点to,下一条边next,权值w; 起点放在head[]中
} edge[MAXN]; 
int head[MAXN];			//head[u]指向结点u的第一条边的存储位置
int cnt;				//记录edge[]的末尾位置,新加入的边放到末尾  
void init()	{ 			//初始化链式前向星
	for (int i = 0; i < MAXN; ++i) {
		head[i] = -1;	//-1结束表示没有下一条边
		edge[i].next = -1;  //表示不存在从结点i出发的边 
	}
	cnt = 0;
}
void addEdge(int u, int v, int w = 0) { //每次加入一条新边,都是直接加到整个edge[]的末尾
	edge[cnt].to = w;
	edge[cnt].w = w;
	edge[cnt].next = head[u];	//指向结点u上一次存的边的位置
	head[u] = cnt++;			//更新结点u最新边的存放位置,就是edge的末尾; cnt指向下一个空处
}

int Log[MAXN], father[MAXN][20], depth[MAXN]; //father的第二维度不应小于log2(MAXN) 
bool vis[MAXN]; 
void dfs(int cur, int father = 0) { //O(N)预处理 
	if (vis[cur]) return;
	vis[cur] = true;
	father[cur][0] = father; //一个数组fa[i][k]存储 i 号点的 2^k 级祖先
	for (int i = 1; i <= Log2[depth[cur]]; ++i) // Log2的预处理参见ST表的笔记
		father[cur][i] = father[father[cur][i - 1]][i - 1]; // 这个DP也参见ST表的笔记
	depth[cur] = depth[father] + 1; //每个点的深度等于父结点的深度+1 
	for (int eg = head[cur]; ~eg; eg = edges[eg].next)
		dfs(edges[eg].to, cur);
}
int lca(int a, int b) {
	if (depth[a] > depth[b]) 
		swap(a, b); 	//保证a的深度小于b,a比b高,让b爬升
	while (depth[a] != depth[b]) 
		b = father[b][Log2[depth[b] - depth[a]]];
	if (a == b) return a; 
	for (int k = Log2[depth[a]]; k >= 0; --k) 
		if (father[a][k] != father[b][k])
			a = father[a][k], b = father[b][k];
	return father[a][0];
}
int main()
{
    // ...
    for (int i = 2; i <= n; ++i)
        Log2[i] = Log2[i / 2] + 1;
    // ...
    dfs(s); // 无根树可以随意选一点为根
    // ...
    return 0;
}
 
现在A点的深度比B点深，所以先让B往上爬，直到与A深度相等为止。然后A点和B点再一起往上爬，直到两点相遇
，于是那一点就是LCA！ 这样下来，每次查询LCA的最坏时间复杂度是O(n)的。

但是有时候，我们需要进行很多次查询，这时朴素的O(n)算法不够了。我们需要**空间换时间**的倍增算法。
倍增的思想曾经在 
我们用一个数组fa[i][k]存储 [公式] 号点的 [公式] 级祖先。（父节点为1级祖先，祖父结点为2级祖先……以此类推）

那么这可以在dfs途中动态规划得出：
// 在dfs中...
fa[cur][0] = fath;
for (int i = 1; i <= Log2[dep[cur]]; ++i) // Log2的预处理参见ST表的笔记
    fa[cur][i] = fa[fa[cur][i - 1]][i - 1]; // 这个DP也参见ST表的笔记
 
这样，往上爬的次数可以被大大缩短（现在变成“跳”了）。

首先还是先让两点深度相等：if (dep[a] > dep[b]) // 不妨设a的深度小于等于b
    swap(a, b);
while (dep[a] != dep[b]) // 跳到深度相等为止
    b = fa[b][Log2[dep[b] - dep[a]]]; // b不断往上跳
例如，a和b本来相差22的深度，现在b不用往上爬22次，只需要依次跳16、4、2个单位，3次便能达到与a相同的距离。

两者深度相等后，如果两个点已经相遇，那么问题就得以解决。如果尚未相遇，我们再让它们一起往上跳。
问题在于，如何确定每次要跳多少？正面解决也许不太容易，我们逆向思考：
如何在a、b不相遇的情况下跳到尽可能高的位置？
如果找到了这个位置，它的父亲就是LCA了

说来也简单，从可能跳的最大步数Log2[dep[a]]（这样至多跳到0号点，不会越界）开始，不断减半步数（不用多次循环）
for (int k = Log2[dep[a]]; k >= 0; k--)
    if (fa[a][k] != fa[b][k])
        a = fa[a][k], b = fa[b][k];

以刚刚那棵树为例，先尝试Log2[4]=2，A、B点的 2^2 级祖先都是0（图中未画出），所以不跳。
然后尝试1，A、B的 2^1 祖先都是2，
也不跳。最后尝试0，A、B的2^0=1级祖先分别是4和5，跳。结束。
https://pic1.zhimg.com/80/v2-ac79bcaef6aeb0ee634c8928dbf01c11_720w.jpg

这样下来，再往上一格所得到的2号点就是所求的最近公共祖先。
全部代码如下：
 
至于树上两点 u,v 的距离dis_{u,v}，有公式
 depth(u)+depth(v)-2depth(lca(u,v)) （很好推）。 O(nlogn) 预处理， O(logn) 查询，空间复杂度为O(nlogn)。

当然，以上都是针对无权树的，如果有权值，可以额外记录一下每个点到根的距离，然后用几乎相同的公式求出。


求树中两个结点的最近公共祖先，是一系列题目：
- 二叉树
	- 普通二叉树：
	    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
	        //如果根节点为空,则返回空
	        if (!root) return nullptr; 
	        //如果根节点是p或者q,则返回root
	        if (root == p || root == q)
	            return root;
	        //根节点非空且非p非q,去左右两边的子树找
	        TreeNode *left = lowestCommonAncestor(root->left, p, q);
	        TreeNode *right = lowestCommonAncestor(root->right, p, q);
	        //如果p,q一个在左子树一个在右子树,则根节点就是最近公共祖先
	        if (left && right)
	            return root;
	        else if (left) //只在左子树中找到则返回找到的left
	            return left;
	        else if (right) //只在右子树中找到则返回找到的right
	            return right;
	        return nullptr;
	    }
	    +见普通的树 
	- 二叉搜索树：排序过的。只需要从树的根结点开始和输入的两个结点比较
				如果当前结点的值比两个结点的值都大，那么两个结点都位于左子树，它们的最低共同
				父结点也位于当前结点的左子树中，于是递归遍历当前结点的左子结点
				如果当前结点的值比两个结点的值都小，那么最低公共父结点一定在当前结点的右子树中
				于是递归遍历当前结点的右子节点
				这样在树中从上到下找到的第一个在两个输入结点的值之间的结点，就是最低公共祖先 
- 普通的树：
	- 有指向父结点的指针（除根节点外）：
		假设树结点中指向父结点的指针是pParent,那么从树的每个叶子结点开始都有一个
		由指针pParent串起来的链表，这些链表头结点是叶子结点，尾指针都是树的根结点。
		输入两个结点，那么两个结点位于两个链表上，它们的最低公共祖先刚好就是
		这两个链表的第一个公共结点。
		比如输入的两个结点分别为 `F,H` ，那么F在 `F->D->B->A` 上，`H` 在链表 `H->E->B->A` 上 
		于是这两个链表的第一个交点 `B` 刚好是它们的最低公共祖先
	![在这里插入图片描述](https://img-blog.csdnimg.cn/20200902212928694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
	- 没有：
		所谓两个结点的公共祖先，是指这两个结点都出现在某个结点的子树中
		从根结点开始遍历一棵树，每遍历一个结点时，判断两个输入结点是否都在它的子树中。
		如果在则分别递归遍历它的所有子结点，判断两个输入结点是否在它的子树中。
		这样从上到下找到的第一个结点，它自己的子树中同时包含两个输入结点而它的子结点中却没有，那么
		该结点就是最低公共祖先。 
		输入结点还是 `F,H` ：判断A的子树中是否同时包含结点 `F,H`，`true`则先后判断A的两个子结点
		B和C的子树是不是同时包含 `F,H` ，结果B的是`true`而C的是`false`。于是再判断B的两个子结点D和E
		，发现这两个结点得到的都是false，而它自己的子树中同时包含两个输入结点，于是B是`F,H`的最低公共祖先
		重复遍历很多次！
		进化一下： 
		更好的做法是：后根遍历，对树结点的每个子树，都不断递归遍历。
		如果是空返回空；是两个输入结点之一则返回它们 
		如果子树中包含两个输入结点（返回了两个有意义的结果）则返回当前结点作为最低公共祖先
		如果只在某个子树中找到，则返回某个子树返回的结果
		否则都没找到，返回空 

		还有的做法是：
		用两个链表（vector）分别保存从根结点到输入的两个结点的路径
		然后把问题转换成两个链表的最后公共结点
		首先得到一条从根结点到树中某一结点的路径，要求有一个辅助空间保存路径
		比如用先根遍历的方法来得到从根结点到H的路径的过程如下：
		（1）遍历到A，把A存放入路径中，路径中只有一个结点A
		（2）遍历到B，把B存放如路径中，此时为A->B
		（3）遍历到D，把D存放到路径中，此时路径为A->B->D
		（4）遍历到F，把F存放到路径中，此时路径为A->B->D->F
		（5）F没有子结点，于是这条路径不可能到达结点H，把F删除，变成A->B->D
		（6）遍历G，和结点F一样也不可能到H，于是遍历万之后，路径仍然是A->B->D
		（7）由于D的所有子结点都遍历过了，不可能到达结点H，因此结点D不在A到H的路径中，把
		D从路径中删除，变成A->B 
		（8）遍历E，把E加入到路径中，此时路径变为A->B->E；
		（9）遍历H，把H加入路径中，已经到达目标结点，
		A->B->E->H就是从根结点开始到达H必须经过的路径 
		
		同样得到从根结点开始到达F必须经过的路径是A->B->D->F，最后求出两个路径的最后公共结点
		就是B，也是F和H的最近公共祖先
		O(n*2)，两条路径最差情况是O(n)，通常情况下则是O(logn) 
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200902213922255.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- …… 
*/ 
namespace LCA {
	bool GetNodePath(TreeNode *pRoot, TreeNode *pNode, list<TreeNode*> &path) {
		if (pRoot == pNode) return true;
		path.push_back(pRoot);
		bool found = false;
		vector<TreeNode*>::iterator it = pRoot->m_vChildren.begin();
		while (!found && it != pRoot->m_vChildren.end()) {
			found = GetNodePath(*it, pNode, path);
			++it;
		}
		if (!found) path.pop_back();
		return found;
	}
	TreeNode *GetLastCommonNode (const list<TreeNode*> &path1, const list<TreeNode*> &path2) {
		list<TreeNode*>::const_iterator it1 = path1.begin();
		list<TreeNode*>::const_iterator it2 = path2.begin();
		TreeNode *pLast = nullptr;
		while (it1 != path1.end() && it2 != path2.end()) {
			if (*it1 == *it2) 
				pLast = *it1;
			++it1; ++it2;
		}
		return pLast;
	}
	TreeNode *GetLastCommonParent(TreeNode *pRoot, TreeNode *pNode1, TreeNode *pNode2) {
		if (pRoot == nullptr || pNode1 == nullptr || pNode2 == nullptr) 
			return nullptr;
		list<TreeNode*> path1;
		GetNodePath(pRoot, pNode1, path1);
		
		list<TreeNode*> path2;
		GetNodePath(pRoot, pNode2, path2);
		
		return GetLastCommonNode(path1, path2);
	}
}


int main() { 

    return 0;
}

