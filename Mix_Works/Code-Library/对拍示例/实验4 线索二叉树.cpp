#include <iostream>
#include <cstring>
using namespace std;
typedef char ElemType;
#define MAX 100
typedef enum { Link, Thread } PointerTag; //标签,Link=0表示指针;Thread=1表示线索 

/*构建链表结构二叉树结点*/
class BiTree
{
public:
	ElemType data;					//数据
	BiTree *parent;					//指向双亲结点的指针,用于后序线索化
	BiTree *leftChild;				//左子树
	BiTree *rightChild;				//右子树 
	PointerTag leftTag;				//左标签，若为0则表示没有线索化，若为1则表示为线索
	PointerTag rightTag;			//右标签，若为0则表示没有线索化，若为1则表示为线索
	BiTree() : BiTree(0, nullptr, nullptr, nullptr, Link, Link) {} 
	BiTree(ElemType d, BiTree *p = nullptr, BiTree *l = nullptr, BiTree *r = nullptr, PointerTag lt = Link, PointerTag rt = Link): 
		data(d), leftChild(l), rightChild(r), parent(p), leftTag(lt), rightTag(rt) { } 
};

BiTree* copyFrom(BiTree *root, BiTree *parent) {
	if (!root) return nullptr;
	BiTree *tRoot = new BiTree(root->data, parent);
	tRoot->leftChild = copyFrom(root->leftChild, tRoot);
	tRoot->rightChild = copyFrom(root->rightChild, tRoot);
	return tRoot;
}

/********************************************************************************************************
函数功能：搜索元素在数组中的序号
形式参数：num表示要搜索的元素，array表示要搜索的数组，len表示数组的长度
返回值：整型，i：该数字位于数组第i个位置，-1：没有找到
*********************************************************************************************************/
int Search(ElemType num, ElemType *array, int len)
{
	for(int i=0; i < len; ++i)
		if(array[i] == num)		//判断数组元素与要搜索的元素是否相等
			return i;		    //返回该元素在数组中的位置
	return -1;					//没有找到，返回-1
}

/********************************************************************************************************
函数功能：由前序和中序确定唯一二叉树
形式参数：*front 指向前序序列数组的指针，*mid指向中序序列数组的指针，len中序序列数组长度
返回值：二叉树根结点指针
*********************************************************************************************************/
BiTree *BuildBiTree(ElemType *front, ElemType *mid, int len)
{
	if(len <= 0) return nullptr;												//递归停止条件
	BiTree *temp = new BiTree(*front);											//申请一个二叉树结点,根结点的数据为前序序列的第一个数
	int index = Search(*front, mid, len);										//寻找根节点数据在中序序列中的位置
	temp->leftChild = BuildBiTree(front + 1, mid, index);   					//以上一步根节点所在位置为界限迭代左子树
	temp->rightChild = BuildBiTree(front + index + 1, mid + index + 1, len - index - 1); //迭代右子树
	return temp;                                                                //返回指向二叉树根结点的指针
}
/********************************************************************************************************
函数功能：前序线索化
形式参数:*root 指向链式结构二叉树根结点的指针
返回值:空
*********************************************************************************************************/
void preOrderThread(BiTree *root)
{
	static BiTree *pre = nullptr;												//定义pre为局部静态变量
	if (root != nullptr)													    //如果根结点不为空
	{
		if(root->leftChild == nullptr)											//如果根结点的左子树为空
		{
			root->leftTag = Thread;												//将其线索化
			root->leftChild = pre;												//该结点的左孩子结点指向前序结点
		}
		if(pre != nullptr && pre->rightChild == nullptr)						//如果前序结点不为空且前序结点的右子树为空
		{
			pre->rightTag = Thread;												//将其线索化
			pre->rightChild = root;												//让前序结点的右孩子结点指向该结点
		}
		pre = root;																//往下一个结点推移
		if(root->leftTag == Link)                                               //判断该结点是否线索化
			preOrderThread(root->leftChild);
		if(root->rightTag == Link)
			preOrderThread(root->rightChild);
	}
}
 
 
/********************************************************************************************************
函数功能：输出前序序列
形式参数:空
返回值:空		
*********************************************************************************************************/
void PreOrder(BiTree *root)
{
	while (root != nullptr)           		//一直迭代到最后
	{
		while(root->leftTag == Link)  		//如果没有线索化
		{
			cout << root->data << " ";	  	//输出数据
			root = root->leftChild;	  		//往左孩子结点迭代
		}
		cout << root->data << " ";		    //输出最左结点 
		root = root->rightChild; 			//移到后继结点 
	}
	cout << endl;
}

/********************************************************************************************************
函数功能：中序线索化
形式参数:*root 指向链式结构二叉树根结点的指针的引用
返回值:空
*********************************************************************************************************/
void inOrderThread(BiTree *root)
{
	static BiTree *pre = nullptr;
	if (root != nullptr)							//如果根节点不为空
	{	
		//递归左子树线索化 
		if (root->leftTag == Link)			  		//先迭代左子树
			inOrderThread(root->leftChild);
			
		if (root->leftChild == nullptr)
		{
			root->leftTag = Thread;					//前驱线索	
			root->leftChild = pre;					//左孩子指针指向前驱 
		}
		//前驱没有右孩子 
		if (pre != nullptr && pre->rightChild == nullptr)
		{
			pre->rightTag = Thread;					//后继线索 
			pre->rightChild = root;					//前驱右孩子指针指向后继(当前节点p) 
		}
		pre = root;									//往后迭代,保证pre指向前驱 
		
		if(root->rightTag == Link)					//再迭代右子树
			inOrderThread(root->rightChild);
	}
}

/********************************************************************************************************
函数功能：输出中序序列
形式参数:空
返回值:空		
*********************************************************************************************************/
void InOrder(BiTree *root)
{
	while (root != nullptr)
	{
		while (root->leftTag == Link)
		{
			root = root->leftChild; 			//找到最左边的结点
		}
		cout << root->data << " ";
		while (root->rightTag == Thread)		//利用线索找到后继节点
		{
			root = root->rightChild;
			cout << root->data << " ";
		}
		root = root->rightChild;				//往右子树迭代
	}
	cout << endl;
}

/********************************************************************************************************
函数功能：后序线索化
形式参数:*root 指向链式结构二叉树根结点的指针的引用
返回值:空
*********************************************************************************************************/
void postOrderThread(BiTree *root)
{
	static BiTree *pre = nullptr;
	if (root != nullptr)
	{
		postOrderThread(root->leftChild);	//左子树递归
		postOrderThread(root->rightChild);	//右子树递归
		if(root->leftChild == nullptr)		//线索化指向前驱结点
		{
			root->leftTag = Thread;
			root->leftChild = pre;
		}
		if(pre != nullptr && pre->rightChild == nullptr)    //为pre找到后继节点
		{
			pre->rightTag = Thread;
			pre->rightChild = root;
		}
		pre = root;			//向后迭代
	}
}

/********************************************************************************************************
函数功能：输出后序序列
形式参数:空
返回值:空	
		    A
	   B		C
	D    E     F G
  H	 I
*********************************************************************************************************/
void PostOrder(BiTree *root) {
	BiTree *cur = root, *pre = nullptr;
	//1.找后序遍历方式开始的节点
	while (cur != nullptr && cur->leftTag == Link) cur = cur->leftChild; //到最左结点 
	
	while (cur != nullptr) { //空树进不来 
		//cur右指针指向后继,就遍历左子树结点或者加上右子树结点 
		if (cur != nullptr && cur->rightTag == Thread) { 
			//根+左结点=>先左结点然后根,最后直到遇到一个结点,有右子树,见else 
			//根+左+右结点=>先左结点然后右结点再回到子树的根,此时右指针不是线索,见else,向上 
			cout << cur->data << " ";
			pre = cur;
			cur = cur->rightChild;	//通过右结点(线索)进入后继:父结点或者右结点 
		} else { 
			//从根+左+右结点到这个if,此时要上到上一层的父结点; 
			//如果上次处理的结点是当前结点的右结点 
			if (cur != nullptr && cur->rightChild == pre) { //处理完子树和其根,上升到父结点 
				cout << cur->data << " ";
				//如果是root就直接返回 
				if (cur == root) { cout << endl; return; }
				pre = cur;
				cur = cur->parent; 
				//通过parent进入上层的父结点,如果cur是parent的右结点,就会继续这个if向上;
				//否则cur是parent的左结点,如果parent有右子树的话,后继就在右子树上,见下面的else 
			}
			//回想二叉树的五种基本形态,这里是根+右子树 
			else { //if (cur != nullptr && cur->leftChild == pre)
				//如果从左结点向上进入parent则找到parent右子树的最左结点
				cur = cur->rightChild;
				while (cur != nullptr && cur->leftTag == Link) cur = cur->leftChild;
			}
		}
	}
	cout << endl;
}

//void PostOrder(BiTree *root)
//{
//	BiTree *pCur = root;
//	BiTree *pre = nullptr;
//	while (pCur != nullptr) 
//	{
//		while (pCur->leftChild != pre && pCur->leftTag == Link) //找到最左边的结点 
//		{
//			pCur = pCur->leftChild;
//		}
//		while (pCur != nullptr && pCur->rightTag == Thread)		//遍历结点的后继 
//		{
//			cout << pCur->data << " ";
//			pre = pCur;											//同时记住每次遍历的前驱 
//			pCur = pCur->rightChild;
//		}
//		while (pCur != nullptr && pCur->rightChild == pre)	    //此时pCur是子树的根结点,pre是其右孩子 
//		{
//			cout << pCur->data << " ";							//左右子树都处理完成,输出子树的根结点数据 
//			pre = pCur;
//			pCur = pCur->parent;								//继续处理pCur的双亲结点 
//		}
//		if (pCur != nullptr && pCur->rightTag == Link) {		//存在右子树 
//			pCur = pCur->rightChild;							//向右子树走 
//		}
//	} 
//	cout << endl; 
//}

int main()
{   
/*
ABDEFGC DBFEGAC DFGEBCA
ABDHIECFG HDIBEAFCG HIDEBFGCA
ABC ABC CBA
*/
	ElemType *preorder = new ElemType[MAX]; //前序
	ElemType *inorder = new ElemType[MAX];  //中序
	cout << "请输入前序序列：" << endl;
	cin >> preorder;
	cout << "请输入中序序列：" << endl;
	cin >> inorder;
	BiTree *root = BuildBiTree(preorder, inorder, strlen(inorder));
	
	BiTree *preRoot = copyFrom(root, nullptr);
	cout << "非递归算法得到的前序序列：" << endl;
	preOrderThread(preRoot);
	PreOrder(preRoot);
	
	BiTree *inRoot = copyFrom(root, nullptr);
	cout<<"非递归算法得到的中序序列："<<endl;
	inOrderThread(inRoot);
	InOrder(inRoot);
	
	BiTree *postRoot = copyFrom(root, nullptr);
	cout << "非递归算法得到的后序序列：" << endl;
	postOrderThread(postRoot);
	PostOrder(postRoot);
	//system("pause");
	return 0;
}
1.后序后继线索二叉树中找结点p的后继（并不知道二叉树的根）。算法思想，对存储结构的要求，算法实现。

要点：

①第一位同学算法考虑了四种情况：

    若结点p为根，则无后继，即结点P的后继为空；
    若结点p为其双亲的右孩子，则其后继为其双亲结点；
    若结点p为其双亲的左孩子，且双亲无右子女，则其后继为其双亲结点；
    若结点p为其双亲的左孩子，且双亲有右子女，则结点p的后继是其双亲的右子树中按后序遍历的第一个结点，即该右子树中最左下的叶节点。

这种考虑有缺陷，没有考虑到右子树。

②第二位同学对其算法进行了补充完善，并讲解了相应的代码实现。考虑情况如下：

    有右线索，直接得到
    若结点*p为根，则无后继；
    若结点*p为其双亲的右孩子，则其后继为其双亲；
    若结点*p为其双亲的左孩子，且双亲无右子女，则其后继为其双亲；
    若结点*p为其双亲的左孩子，且双亲有右子女，则结点*p的后继是其双亲的右子树中按后序遍历的第一个结点。

这样的考虑就很完善了，代码实现也很简洁。
