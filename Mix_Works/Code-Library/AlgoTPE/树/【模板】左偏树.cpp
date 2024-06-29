#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;
struct LeftistNode {
	int left, right, father; //���Ҷ��ӱ��,fatherΪ���鼯
	int val;	//����ֵ,val==-1��ʾ��㱻ɾ�� 
	int dist;   //disΪ���ľ���npl
} leftistTrees; //ltt �ڴ��д����ƽ������ϯ����ͬ
#define ls(x) leftistTrees[x].left
#define rs(x) leftistTrees[x].right //����ר��define 
#define v(x)  leftistTrees[x].val  
#define d(x)  leftistTrees[x].dist
#define f(x)  leftistTrees[x].father

//��ʼ��
leftistTrees[0].dist = -1; //ƽ��������ϯ�����ڴ��д������0�Ž�����ս��,����Ϊ-1
for (int i = 1; i <= n; ++i) {
	leftistTrees[i].val = read(); //�����ֵ
	leftistTrees[i].father = i;   //��ʼ�����鼯
}
//�ϲ�����
inline int merge(int x, int y) { //�ϲ�������,x,y���ǶѶ�Ԫ�صı��
 	if (!x || !y) return x + y;  //����пյ�,�ͷ�����һ��,��fhq-treap��ͬ
	//?��ǰ���Ϊ��ά��С��������,
	//����������ĿҪ���,ֵ��ͬ���±�С�����ȼ���
	if (v(x) > v(y) || v(x) == v(y) && x > y)
		swap(x, y);  
	//�ϲ���������y 
	rs(x) = merge(rs(x), y);  
	//ά�����鼯 x���µ��Ҷ��ӵ�father��仯 
	f(rs(x)) = x;  
	//�����������ƫ��������,�ͽ������Ҷ��� 
	if (d(ls(x)) < d(rs(x))
		swap(ls(x), rs(x));
	d(x) = d(rs(x)) + 1; //���ý���������Ҷ��Ӿ���+1����dist
	return x;            //return�ϲ��õĶѶ�����ż���
}
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5 + 5;
struct LeftistNode {
	int left, right, father; //���Ҷ��ӱ��,fatherΪ���鼯
	int val;	//����ֵ,val==-1��ʾ��㱻ɾ�� 
	int dist;   //disΪ���ľ���npl
} leftistTrees; //ltt �ڴ��д����ƽ������ϯ����ͬ
#define ls(x) leftistTrees[x].left
#define rs(x) leftistTrees[x].right //����ר��define 
#define v(x)  leftistTrees[x].val  
#define d(x)  leftistTrees[x].dist
#define f(x)  leftistTrees[x].father

//��ʼ��
leftistTrees[0].dist = -1; //ƽ��������ϯ�����ڴ��д������0�Ž�����ս��,����Ϊ-1
for (int i = 1; i <= n; ++i) {
	leftistTrees[i].val = read(); //�����ֵ
	leftistTrees[i].father = i;   //��ʼ�����鼯
}
//�ϲ�����
inline int merge(int x, int y) { //�ϲ�������,x,y���ǶѶ�Ԫ�صı��
 	if (!x || !y) return x + y;  //����пյ�,�ͷ�����һ��,��fhq-treap��ͬ
	//?��ǰ���Ϊ��ά��С��������,
	//����������ĿҪ���,ֵ��ͬ���±�С�����ȼ���
	if (v(x) > v(y) || v(x) == v(y) && x > y)
		swap(x, y);  
	//�ϲ���������y 
	rs(x) = merge(rs(x), y);  
	//ά�����鼯 x���µ��Ҷ��ӵ�father��仯 
	f(rs(x)) = x;  
	//�����������ƫ��������,�ͽ������Ҷ��� 
	if (d(ls(x)) < d(rs(x))
		swap(ls(x), rs(x));
	d(x) = d(rs(x)) + 1; //���ý���������Ҷ��Ӿ���+1����dist
	return x;            //return�ϲ��õĶѶ�����ż���
}
//���鼯��popɾ���Ѷ�Ԫ��
int find(int x) { return f(x) == x ? x : f(x) == find(f(x)); }  //���鼯��·��ѹ��
//popɾ��
inline void pop(int x) { //ɾ���Ѷ�Ԫ��  xû�б�ĸ��� 
	v(x) = -1;   		 //ֵΪ-1��ʾ��ɾ��
	f(ls(x)) = ls(x);    //ά�����鼯,һ�����ĸ����ǽ�㱾��,��ʾ���û�и�����,���Ƕ��ǶѶ� 
	f(rs(x)) = rs(x);    //ά�����鼯
	//��Ϊ·��ѹ��,���Կ��ܻ��г���ls(x)��rs(x)����Ľ���fatherָ��ָ��x(�ܶ౻ѹ���Ľ��Ҳָ��x)
	//�ںϲ��е���,��������д,���ò��鼯�ϵ�,����x�ı�Ķ�����ô��
	//������дf(x)=...�������Ҷ��ӵ�����Ķ��ӽ�㻹��ָ��x,�����Ҷ���֮��Ͽ��� 
	f(x) = merge(ls(x), rs(x));   //��ɾ���ĵ�����Ҷ��Ӻϲ�����¶� 
}
//-----------------------------------------------------------------------------
 
//�ϲ�,�ϲ�����ƫ������Ҫ�Ĳ������Ͼ��ɲ��ѿɲ��ѣ��϶���Ҫ�ܹ��ϲ��ġ�
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

//deleteMinɾ�����ڵ�
//ֻҪ��ɾ�����ڵ㣬�������ڵ��Ȩֵ��Ϊ-1, 
//��ʵ�е�ʱ�򲻸�ȨֵҲûӰ�죩��Ȼ��ϲ����ڵ�����������Ϳ����ˡ�
//ɾ�����ڵ���룺
void Delroot(int x)
{
    int ls=l(x),rs=r(x);
    v(x)=-1,f(ls)=0,f(rs)=0;
    Merge(ls,rs);
}

//ɾ������ڵ�
//���������ڵ�ָ���������ŵĽڵ����������Ȩֵ�Ľڵ㣬һ��Ŀɲ����ǲ�֧��ɾ������Ȩֵ�ڵ�Ĳ����ġ�
//��ɾ�����ڵ����ƣ��Ƚ�Ҫɾ���Ľڵ��Ȩֵ��ֵΪ?1
//��Ȼ��ϲ������������������ϲ����µ���ƫ���ӵ���ɾ���ڵ�ĸ��ڵ��ϾͿ����ˡ�
//������ɾ�����ڵ㲻ͬ���ǣ�����������ܻᵼ��������ƫ������ƫ���ʱ��ƻ���
//���Ҫ�Ӹýڵ�һֱ���ϼ����ƫ���ʣ�ֱ����ƫ����û�б��ƻ����ߵ����˸��ڵ㡣
//ɾ���ڵ���룺
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

//����
//�����ӵ�ϲ��Ļ�ʱ�临�Ӷ���O(nlogn),�������Խ��ܣ����������Ҫһ���Ƚϸ�Ч�ķ�����ʵ�ֽ�����
//���������¼������裺
//1. ����һ�����У���ÿ���ڵ㿴��һ���ڵ���Ϊ1����ƫ��������С�
//2. ÿ��ȡ����ͷ��������ƫ���������Ǻϲ��������ϲ��������ƫ��������С�
//3. �ظ���2����ֱ������Ϊ�ա�
//��������
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

