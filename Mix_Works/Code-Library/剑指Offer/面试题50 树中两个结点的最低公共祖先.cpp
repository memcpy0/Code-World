#include <bits/stdc++.h>
using namespace std;
/*
�����Ǵ������ϵ�͵��ϵ�����⣨��򵥵ľ�����������ľ��룩ʱ��������Ҫ�õ���������������������
LowestCommonAncestor��`LCA`������ͼ�� 
https://pic2.zhimg.com/80/v2-99457646a2f9b511f82dd244eb76ba2c_720w.jpg
���ص������ǣ�
�ٽ���һ��dfs�����ÿ��������
//��ʽǰ���� 
//�������i�������ڽӵ�  
const int MAXN = 1e6;
struct Edge {
	int to, next, w;	//��:�յ�to,��һ����next,Ȩֵw; ������head[]��
} edge[MAXN]; 
int head[MAXN];			//head[u]ָ����u�ĵ�һ���ߵĴ洢λ��
int cnt;				//��¼edge[]��ĩβλ��,�¼���ı߷ŵ�ĩβ  
void init()	{ 			//��ʼ����ʽǰ����
	for (int i = 0; i < MAXN; ++i) {
		head[i] = -1;	//-1������ʾû����һ����
		edge[i].next = -1;  //��ʾ�����ڴӽ��i�����ı� 
	}
	cnt = 0;
}
void addEdge(int u, int v, int w = 0) { //ÿ�μ���һ���±�,����ֱ�Ӽӵ�����edge[]��ĩβ
	edge[cnt].to = w;
	edge[cnt].w = w;
	edge[cnt].next = head[u];	//ָ����u��һ�δ�ıߵ�λ��
	head[u] = cnt++;			//���½��u���±ߵĴ��λ��,����edge��ĩβ; cntָ����һ���մ�
}

int Log[MAXN], father[MAXN][20], depth[MAXN]; //father�ĵڶ�ά�Ȳ�ӦС��log2(MAXN) 
bool vis[MAXN]; 
void dfs(int cur, int father = 0) { //O(N)Ԥ���� 
	if (vis[cur]) return;
	vis[cur] = true;
	father[cur][0] = father; //һ������fa[i][k]�洢 i �ŵ�� 2^k ������
	for (int i = 1; i <= Log2[depth[cur]]; ++i) // Log2��Ԥ����μ�ST��ıʼ�
		father[cur][i] = father[father[cur][i - 1]][i - 1]; // ���DPҲ�μ�ST��ıʼ�
	depth[cur] = depth[father] + 1; //ÿ�������ȵ��ڸ��������+1 
	for (int eg = head[cur]; ~eg; eg = edges[eg].next)
		dfs(edges[eg].to, cur);
}
int lca(int a, int b) {
	if (depth[a] > depth[b]) 
		swap(a, b); 	//��֤a�����С��b,a��b��,��b����
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
    dfs(s); // �޸�����������ѡһ��Ϊ��
    // ...
    return 0;
}
 
����A�����ȱ�B�����������B��������ֱ����A������Ϊֹ��Ȼ��A���B����һ����������ֱ����������
��������һ�����LCA�� ����������ÿ�β�ѯLCA���ʱ�临�Ӷ���O(n)�ġ�

������ʱ��������Ҫ���кܶ�β�ѯ����ʱ���ص�O(n)�㷨�����ˡ�������Ҫ**�ռ任ʱ��**�ı����㷨��
������˼�������� 
������һ������fa[i][k]�洢 [��ʽ] �ŵ�� [��ʽ] �����ȡ������ڵ�Ϊ1�����ȣ��游���Ϊ2�����ȡ����Դ����ƣ�

��ô�������dfs;�ж�̬�滮�ó���
// ��dfs��...
fa[cur][0] = fath;
for (int i = 1; i <= Log2[dep[cur]]; ++i) // Log2��Ԥ����μ�ST��ıʼ�
    fa[cur][i] = fa[fa[cur][i - 1]][i - 1]; // ���DPҲ�μ�ST��ıʼ�
 
�������������Ĵ������Ա�������̣����ڱ�ɡ������ˣ���

���Ȼ����������������ȣ�if (dep[a] > dep[b]) // ������a�����С�ڵ���b
    swap(a, b);
while (dep[a] != dep[b]) // ����������Ϊֹ
    b = fa[b][Log2[dep[b] - dep[a]]]; // b����������
���磬a��b�������22����ȣ�����b����������22�Σ�ֻ��Ҫ������16��4��2����λ��3�α��ܴﵽ��a��ͬ�ľ��롣

���������Ⱥ�����������Ѿ���������ô����͵��Խ���������δ������������������һ����������
�������ڣ����ȷ��ÿ��Ҫ�����٣�������Ҳ��̫���ף���������˼����
�����a��b����������������������ܸߵ�λ�ã�
����ҵ������λ�ã����ĸ��׾���LCA��

˵��Ҳ�򵥣��ӿ������������Log2[dep[a]]��������������0�ŵ㣬����Խ�磩��ʼ�����ϼ��벽�������ö��ѭ����
for (int k = Log2[dep[a]]; k >= 0; k--)
    if (fa[a][k] != fa[b][k])
        a = fa[a][k], b = fa[b][k];

�Ըո��ǿ���Ϊ�����ȳ���Log2[4]=2��A��B��� 2^2 �����ȶ���0��ͼ��δ�����������Բ�����
Ȼ����1��A��B�� 2^1 ���ȶ���2��
Ҳ�����������0��A��B��2^0=1�����ȷֱ���4��5������������
https://pic1.zhimg.com/80/v2-ac79bcaef6aeb0ee634c8928dbf01c11_720w.jpg

����������������һ�����õ���2�ŵ�������������������ȡ�
ȫ���������£�
 
������������ u,v �ľ���dis_{u,v}���й�ʽ
 depth(u)+depth(v)-2depth(lca(u,v)) ���ܺ��ƣ��� O(nlogn) Ԥ���� O(logn) ��ѯ���ռ临�Ӷ�ΪO(nlogn)��

��Ȼ�����϶��������Ȩ���ģ������Ȩֵ�����Զ����¼һ��ÿ���㵽���ľ��룬Ȼ���ü�����ͬ�Ĺ�ʽ�����


������������������������ȣ���һϵ����Ŀ��
- ������
	- ��ͨ��������
	    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
	        //������ڵ�Ϊ��,�򷵻ؿ�
	        if (!root) return nullptr; 
	        //������ڵ���p����q,�򷵻�root
	        if (root == p || root == q)
	            return root;
	        //���ڵ�ǿ��ҷ�p��q,ȥ�������ߵ�������
	        TreeNode *left = lowestCommonAncestor(root->left, p, q);
	        TreeNode *right = lowestCommonAncestor(root->right, p, q);
	        //���p,qһ����������һ����������,����ڵ���������������
	        if (left && right)
	            return root;
	        else if (left) //ֻ�����������ҵ��򷵻��ҵ���left
	            return left;
	        else if (right) //ֻ�����������ҵ��򷵻��ҵ���right
	            return right;
	        return nullptr;
	    }
	    +����ͨ���� 
	- ������������������ġ�ֻ��Ҫ�����ĸ���㿪ʼ��������������Ƚ�
				�����ǰ����ֵ����������ֵ������ô������㶼λ�������������ǵ���͹�ͬ
				�����Ҳλ�ڵ�ǰ�����������У����ǵݹ������ǰ�������ӽ��
				�����ǰ����ֵ����������ֵ��С����ô��͹��������һ���ڵ�ǰ������������
				���ǵݹ������ǰ�������ӽڵ�
				���������д��ϵ����ҵ��ĵ�һ���������������ֵ֮��Ľ�㣬������͹������� 
- ��ͨ������
	- ��ָ�򸸽���ָ�루�����ڵ��⣩��
		�����������ָ�򸸽���ָ����pParent,��ô������ÿ��Ҷ�ӽ�㿪ʼ����һ��
		��ָ��pParent��������������Щ����ͷ�����Ҷ�ӽ�㣬βָ�붼�����ĸ���㡣
		����������㣬��ô�������λ�����������ϣ����ǵ���͹������ȸպþ���
		����������ĵ�һ��������㡣
		����������������ֱ�Ϊ `F,H` ����ôF�� `F->D->B->A` �ϣ�`H` ������ `H->E->B->A` �� 
		��������������ĵ�һ������ `B` �պ������ǵ���͹�������
	![���������ͼƬ����](https://img-blog.csdnimg.cn/20200902212928694.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
	- û�У�
		��ν�������Ĺ������ȣ���ָ��������㶼������ĳ������������
		�Ӹ���㿪ʼ����һ������ÿ����һ�����ʱ���ж������������Ƿ������������С�
		�������ֱ�ݹ�������������ӽ�㣬�ж������������Ƿ������������С�
		�������ϵ����ҵ��ĵ�һ����㣬���Լ���������ͬʱ��������������������ӽ����ȴû�У���ô
		�ý�������͹������ȡ� 
		�����㻹�� `F,H` ���ж�A���������Ƿ�ͬʱ������� `F,H`��`true`���Ⱥ��ж�A�������ӽ��
		B��C�������ǲ���ͬʱ���� `F,H` �����B����`true`��C����`false`���������ж�B�������ӽ��D��E
		���������������õ��Ķ���false�������Լ���������ͬʱ�������������㣬����B��`F,H`����͹�������
		�ظ������ܶ�Σ�
		����һ�£� 
		���õ������ǣ������������������ÿ�������������ϵݹ������
		����ǿշ��ؿգ�������������֮һ�򷵻����� 
		��������а������������㣨����������������Ľ�����򷵻ص�ǰ�����Ϊ��͹�������
		���ֻ��ĳ���������ҵ����򷵻�ĳ���������صĽ��
		����û�ҵ������ؿ� 

		���е������ǣ�
		����������vector���ֱ𱣴�Ӹ���㵽�������������·��
		Ȼ�������ת���������������󹫹����
		���ȵõ�һ���Ӹ���㵽����ĳһ����·����Ҫ����һ�������ռ䱣��·��
		�������ȸ������ķ������õ��Ӹ���㵽H��·���Ĺ������£�
		��1��������A����A�����·���У�·����ֻ��һ�����A
		��2��������B����B�����·���У���ʱΪA->B
		��3��������D����D��ŵ�·���У���ʱ·��ΪA->B->D
		��4��������F����F��ŵ�·���У���ʱ·��ΪA->B->D->F
		��5��Fû���ӽ�㣬��������·�������ܵ�����H����Fɾ�������A->B->D
		��6������G���ͽ��Fһ��Ҳ�����ܵ�H�����Ǳ�����֮��·����Ȼ��A->B->D
		��7������D�������ӽ�㶼�������ˣ������ܵ�����H����˽��D����A��H��·���У���
		D��·����ɾ�������A->B 
		��8������E����E���뵽·���У���ʱ·����ΪA->B->E��
		��9������H����H����·���У��Ѿ�����Ŀ���㣬
		A->B->E->H���ǴӸ���㿪ʼ����H���뾭����·�� 
		
		ͬ���õ��Ӹ���㿪ʼ����F���뾭����·����A->B->D->F������������·������󹫹����
		����B��Ҳ��F��H�������������
		O(n*2)������·����������O(n)��ͨ�����������O(logn) 
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200902213922255.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70#pic_center)
- ���� 
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

