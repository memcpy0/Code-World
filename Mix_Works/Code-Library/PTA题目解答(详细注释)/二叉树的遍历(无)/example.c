/***************************************************************************************
二叉树的遍历 
本题要求给定二叉树的4种遍历。
函数接口定义：
void InorderTraversal( BinTree BT );
void PreorderTraversal( BinTree BT );
void PostorderTraversal( BinTree BT );
void LevelorderTraversal( BinTree BT );
其中BinTree结构定义如下：
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};
要求4个函数分别按照访问顺序打印出结点的内容，格式为一个空格跟着一个字符。
输出样例（对于图中给出的树）：

Inorder: D B E F A G H C I
Preorder: A B D F E C G H I
Postorder: D E F B H G I C A
Levelorder: A B C D F G I E H
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

typedef char ElementType;
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};

BinTree CreatBinTree(); /* 实现细节忽略 */
void InorderTraversal( BinTree BT );
void PreorderTraversal( BinTree BT );
void PostorderTraversal( BinTree BT );
void LevelorderTraversal( BinTree BT );

int main()
{
    BinTree BT = CreatBinTree();
    printf("Inorder:");    InorderTraversal(BT);    printf("\n");
    printf("Preorder:");   PreorderTraversal(BT);   printf("\n");
    printf("Postorder:");  PostorderTraversal(BT);  printf("\n");
    printf("Levelorder:"); LevelorderTraversal(BT); printf("\n");
    return 0;
}
/* 你的代码将被嵌在这里 */
BinTree CreatBinTree()
{
	
}

void InorderTraversal( BinTree BT )
{
	if(BT){
		/*visit left*/
		InorderTraversal(BT->Left);	 
		/*visit data*/ 
		printf("%d ", BT->Data);
		/*visit right*/ 
		InorderTraversal(BT->Right);
	}
} 

void PreorderTraversal( BinTree BT )
{
	if(BT){
		/*visit data*/
		printf("%d ", BT->Data);
		/*visit left*/
		PreorderTraversal(BT->Left);
		/*visit right*/
		PreorderTraversal(BT->Right);
	}
}

void PostorderTraversal( BinTree BT )
{
	if(BT){
		/*visit left*/
		PreorderTraversal(BT->Left);
		/*visit right*/
		PreorderTraversal(BT->Right);
		/*visit data*/
		printf("%d ", BT->Data);
	}
}
//#define ERROR NULL
//typedef BinTree QElementType;
//typedef struct _queue{
//	QElementType Data;
//	struct _queue *Next;
//}Queue;
//
//Queue *CreateQ(void)
//{
//	Queue *que;
//	que = (Queue *)malloc(sizeof(Queue));
//	que->Next = NULL;
//	
//	return que;	
//}
//void AddQ(Queue* que, QElementType item)
//{
//	Queue *temp;
//	temp = (Queue *)malloc(sizeof(Queue));
//	temp->Data = item;
//	temp->Next = NULL;
//	while(que->Next != NULL){
//		que = que->Next;
//	}
//	que->Next = temp;	
//}
//QElementType DeleteQ(Queue* que)
//{
//	QElementType data;
//	if(que->Next != NULL){
//		Queue *temp;
//		temp = que->Next;
//		data = temp->Data;
//		que->Next = temp->Next;
//		free(temp);
//	}
//	else {
//		printf("the queue is empty, delete is failed!\n");
//		data = ERROR;
//	} 
//		
//	return data;
//}
#define MaxSize 100 
void LevelorderTraversal( BinTree BT )
{
	Queue que;
	BinTree qTree;
	
	que = CreateQueue(MaxSize);
	if(BT){
		AddQ(que, BT);
		qTree = DeleteQ(que);
		while(qTree != NULL){
			printf("%d ", qTree->Data);
			if(qTree->Left != NULL)
				AddQ(que, qTree->Left);
			if(qTree->Right != NULL)
				AddQ(que, qTree->Right);
			qTree = DeleteQ(que);
		}
	}	
}
