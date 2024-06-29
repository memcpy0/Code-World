#include <iostream>
#include <cstring>
using namespace std;
typedef char ElemType;
#define MAX 100
typedef enum { Link, Thread } PointerTag; //��ǩ,Link=0��ʾָ��;Thread=1��ʾ���� 

/*��������ṹ���������*/
class BiTree
{
public:
	ElemType data;					//����
	BiTree *parent;					//ָ��˫�׽���ָ��,���ں���������
	BiTree *leftChild;				//������
	BiTree *rightChild;				//������ 
	PointerTag leftTag;				//���ǩ����Ϊ0���ʾû������������Ϊ1���ʾΪ����
	PointerTag rightTag;			//�ұ�ǩ����Ϊ0���ʾû������������Ϊ1���ʾΪ����
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
�������ܣ�����Ԫ���������е����
��ʽ������num��ʾҪ������Ԫ�أ�array��ʾҪ���������飬len��ʾ����ĳ���
����ֵ�����ͣ�i��������λ�������i��λ�ã�-1��û���ҵ�
*********************************************************************************************************/
int Search(ElemType num, ElemType *array, int len)
{
	for(int i=0; i < len; ++i)
		if(array[i] == num)		//�ж�����Ԫ����Ҫ������Ԫ���Ƿ����
			return i;		    //���ظ�Ԫ���������е�λ��
	return -1;					//û���ҵ�������-1
}

/********************************************************************************************************
�������ܣ���ǰ�������ȷ��Ψһ������
��ʽ������*front ָ��ǰ�����������ָ�룬*midָ���������������ָ�룬len�����������鳤��
����ֵ�������������ָ��
*********************************************************************************************************/
BiTree *BuildBiTree(ElemType *front, ElemType *mid, int len)
{
	if(len <= 0) return nullptr;												//�ݹ�ֹͣ����
	BiTree *temp = new BiTree(*front);											//����һ�����������,����������Ϊǰ�����еĵ�һ����
	int index = Search(*front, mid, len);										//Ѱ�Ҹ��ڵ����������������е�λ��
	temp->leftChild = BuildBiTree(front + 1, mid, index);   					//����һ�����ڵ�����λ��Ϊ���޵���������
	temp->rightChild = BuildBiTree(front + index + 1, mid + index + 1, len - index - 1); //����������
	return temp;                                                                //����ָ�������������ָ��
}
/********************************************************************************************************
�������ܣ�ǰ��������
��ʽ����:*root ָ����ʽ�ṹ������������ָ��
����ֵ:��
*********************************************************************************************************/
void preOrderThread(BiTree *root)
{
	static BiTree *pre = nullptr;												//����preΪ�ֲ���̬����
	if (root != nullptr)													    //�������㲻Ϊ��
	{
		if(root->leftChild == nullptr)											//���������������Ϊ��
		{
			root->leftTag = Thread;												//����������
			root->leftChild = pre;												//�ý������ӽ��ָ��ǰ����
		}
		if(pre != nullptr && pre->rightChild == nullptr)						//���ǰ���㲻Ϊ����ǰ�����������Ϊ��
		{
			pre->rightTag = Thread;												//����������
			pre->rightChild = root;												//��ǰ������Һ��ӽ��ָ��ý��
		}
		pre = root;																//����һ���������
		if(root->leftTag == Link)                                               //�жϸý���Ƿ�������
			preOrderThread(root->leftChild);
		if(root->rightTag == Link)
			preOrderThread(root->rightChild);
	}
}
 
 
/********************************************************************************************************
�������ܣ����ǰ������
��ʽ����:��
����ֵ:��		
*********************************************************************************************************/
void PreOrder(BiTree *root)
{
	while (root != nullptr)           		//һֱ���������
	{
		while(root->leftTag == Link)  		//���û��������
		{
			cout << root->data << " ";	  	//�������
			root = root->leftChild;	  		//�����ӽ�����
		}
		cout << root->data << " ";		    //��������� 
		root = root->rightChild; 			//�Ƶ���̽�� 
	}
	cout << endl;
}

/********************************************************************************************************
�������ܣ�����������
��ʽ����:*root ָ����ʽ�ṹ������������ָ�������
����ֵ:��
*********************************************************************************************************/
void inOrderThread(BiTree *root)
{
	static BiTree *pre = nullptr;
	if (root != nullptr)							//������ڵ㲻Ϊ��
	{	
		//�ݹ������������� 
		if (root->leftTag == Link)			  		//�ȵ���������
			inOrderThread(root->leftChild);
			
		if (root->leftChild == nullptr)
		{
			root->leftTag = Thread;					//ǰ������	
			root->leftChild = pre;					//����ָ��ָ��ǰ�� 
		}
		//ǰ��û���Һ��� 
		if (pre != nullptr && pre->rightChild == nullptr)
		{
			pre->rightTag = Thread;					//������� 
			pre->rightChild = root;					//ǰ���Һ���ָ��ָ����(��ǰ�ڵ�p) 
		}
		pre = root;									//�������,��֤preָ��ǰ�� 
		
		if(root->rightTag == Link)					//�ٵ���������
			inOrderThread(root->rightChild);
	}
}

/********************************************************************************************************
�������ܣ������������
��ʽ����:��
����ֵ:��		
*********************************************************************************************************/
void InOrder(BiTree *root)
{
	while (root != nullptr)
	{
		while (root->leftTag == Link)
		{
			root = root->leftChild; 			//�ҵ�����ߵĽ��
		}
		cout << root->data << " ";
		while (root->rightTag == Thread)		//���������ҵ���̽ڵ�
		{
			root = root->rightChild;
			cout << root->data << " ";
		}
		root = root->rightChild;				//������������
	}
	cout << endl;
}

/********************************************************************************************************
�������ܣ�����������
��ʽ����:*root ָ����ʽ�ṹ������������ָ�������
����ֵ:��
*********************************************************************************************************/
void postOrderThread(BiTree *root)
{
	static BiTree *pre = nullptr;
	if (root != nullptr)
	{
		postOrderThread(root->leftChild);	//�������ݹ�
		postOrderThread(root->rightChild);	//�������ݹ�
		if(root->leftChild == nullptr)		//������ָ��ǰ�����
		{
			root->leftTag = Thread;
			root->leftChild = pre;
		}
		if(pre != nullptr && pre->rightChild == nullptr)    //Ϊpre�ҵ���̽ڵ�
		{
			pre->rightTag = Thread;
			pre->rightChild = root;
		}
		pre = root;			//������
	}
}

/********************************************************************************************************
�������ܣ������������
��ʽ����:��
����ֵ:��		
*********************************************************************************************************/
void PostOrder(BiTree *root)
{
	BiTree *pCur = root;
	BiTree *pre = nullptr;
	while (pCur != nullptr) 
	{
		while (pCur->leftChild != pre && pCur->leftTag == Link) //�ҵ�����ߵĽ�� 
		{
			pCur = pCur->leftChild;
		}
		while (pCur != nullptr && pCur->rightTag == Thread)		//�������ĺ�� 
		{
			cout << pCur->data << " ";
			pre = pCur;											//ͬʱ��סÿ�α�����ǰ�� 
			pCur = pCur->rightChild;
		}
		while (pCur != nullptr && pCur->rightChild == pre)	    //��ʱpCur�������ĸ����,pre�����Һ��� 
		{
			cout << pCur->data << " ";							//�����������������,��������ĸ�������� 
			pre = pCur;
			pCur = pCur->parent;								//��������pCur��˫�׽�� 
		}
		if (pCur != nullptr && pCur->rightTag == Link) {		//���������� 
			pCur = pCur->rightChild;							//���������� 
		}
	} 
	cout << endl; 
}

int main()
{   
/*
ABDEFGC DBFEGAC DFGEBCA
ABDHIECFG HDIBEAFCG HIDEBFGCA
ABC ABC CBA
*/
	ElemType *preorder = new ElemType[MAX]; //ǰ��
	ElemType *inorder = new ElemType[MAX];  //����
	cout << "������ǰ�����У�" << endl;
	cin >> preorder;
	cout << "�������������У�" << endl;
	cin >> inorder;
	BiTree *root = BuildBiTree(preorder, inorder, strlen(inorder));
	
	BiTree *preRoot = copyFrom(root, nullptr);
	cout << "�ǵݹ��㷨�õ���ǰ�����У�" << endl;
	preOrderThread(preRoot);
	PreOrder(preRoot);
	
	BiTree *inRoot = copyFrom(root, nullptr);
	cout<<"�ǵݹ��㷨�õ����������У�"<<endl;
	inOrderThread(inRoot);
	InOrder(inRoot);
	
	BiTree *postRoot = copyFrom(root, nullptr);
	cout << "�ǵݹ��㷨�õ��ĺ������У�" << endl;
	postOrderThread(postRoot);
	PostOrder(postRoot);
	//system("pause");
	return 0;
}

