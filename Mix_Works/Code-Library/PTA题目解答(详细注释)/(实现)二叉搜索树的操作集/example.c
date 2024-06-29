/***************************************************************************************
二叉搜索树的操作集
本题要求实现给定二叉搜索树的5种常用操作。
BinTree Insert( BinTree BST, ElementType X );
BinTree Delete( BinTree BST, ElementType X );
Position Find( BinTree BST, ElementType X );
Position FindMin( BinTree BST );
Position FindMax( BinTree BST );
其中BinTree结构定义如下：
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};
函数Insert将X插入二叉搜索树BST并返回结果树的根结点指针；
函数Delete将X从二叉搜索树BST中删除，并返回结果树的根结点指针；如果X不在树中，则打印一行Not Found并返回原树的根结点指针；
函数Find在二叉搜索树BST中找到X，返回该结点的指针；如果找不到则返回空指针；
函数FindMin返回二叉搜索树BST中最小元结点的指针；
函数FindMax返回二叉搜索树BST中最大元结点的指针。
输入样例：
10
5 8 6 2 4 1 0 10 9 7
5
6 3 10 0 5
5
5 7 0 10 3
输出样例：
Preorder: 5 2 1 0 4 8 6 7 10 9
6 is found
3 is not found
10 is found
10 is the largest key
0 is found
0 is the smallest key
5 is found
Not Found
Inorder: 1 2 4 6 8 9
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};
void PreorderTraversal(BinTree BT); /* 先序遍历 */
void InorderTraversal(BinTree BT);  /* 中序遍历 */

BinTree Insert( BinTree BST, ElementType X );
BinTree Delete( BinTree BST, ElementType X );
Position Find( BinTree BST, ElementType X );
Position FindMin( BinTree BST );
Position FindMax( BinTree BST );

void PreorderTraversal(BinTree BT) { /* 递归版本 */
	if (BT) {
		printf("%d ", BT->Data);
		PreorderTraversal(BT->Left);
		PreorderTraversal(BT->Right);
	} 
}
void InorderTraversal(BinTree BT) { /* 递归版本 */
	if (BT) {
		InorderTraversal(BT->Left);
		printf("%d ", BT->Data);
		InorderTraversal(BT->Right);
	} 
}
void PostorderTraversal(BinTree BT) { /* 递归版本 */
	if (BT) {
		PreorderTraversal(BT->Left);
		PreorderTraversal(BT->Right);
		printf("%d ", BT->Data);
	}
}
void LevelorderTraversal(BinTree BT); /* 非递归版本 */

BinTree Insert(BinTree BST, ElementType X) {
	/* Find操作的改版 在找不到的时候进行操作; 函数Insert将X插入二叉搜索树BST并返回结果树的根结点指针 */
	if (BST == NULL) {
        BST = (BinTree)malloc(sizeof(struct TNode));
		BST->Data = X;
		BST->Left = BST->Right = NULL;
	} else {
		if (X < BST->Data) BST->Left = Insert(BST->Left, X);
		else if (X > BST->Data) BST->Right = Insert(BST->Right, X);
		/* else do nothing */
	}
	return BST; /* Don't forget this line! */
}
BinTree Delete(BinTree BST, ElementType X) {
	/* Find操作的改版 在找到后进行判断和操作; 
	函数Delete将X从二叉搜索树BST中删除，并返回结果树的根结点指针；如果X不在树中，
	则打印一行Not Found并返回原树的根结点指针 */
	if (BST == NULL) {
		printf("Not Found\n");
	} else {
		if (X < BST->Data) BST->Left = Delete(BST->Left, X);
		else if (X > BST->Data) BST->Right = Delete(BST->Right, X);
		else { 
		    Position TmpCell;
            if (BST->Left && BST->Right) { /*BST has both two child */
            	TmpCell = FindMin(BST->Right);
            	BST->Data = TmpCell->Data; /* 找到右子树的最小值放在根上面, 它大于左子树 */
            	BST->Right = Delete(BST->Right, TmpCell->Data); /* 递归删除右子树最小元, 它没有左子树 */ 
			} else { /* zero childTree or one child, left or right */
            	TmpCell = BST; /* Don't forget! */
            	if (BST->Left) {         /* 左子树非空 */
            		BST = BST->Left; 
				} else if (BST->Right) { /* 右子树非空 */
					BST = BST->Right;
				} else {                 /* 为叶子结点 */
					BST = NULL;
				} 
            	free(TmpCell);
			}
		}
	}
	return BST;
}
Position Find(BinTree BST, ElementType X) {  /* 递归版本 */
	/* 函数Find在二叉搜索树BST中找到X，返回该结点的指针；如果找不到则返回空指针 */
	if (BST == NULL) return NULL;
	else {
		if (X < BST->Data) return Find(BST->Left, X);
		else if (X > BST->Data) return Find(BST->Right, X);
		else return BST;
	}
}
Position FindMin(BinTree BST) {  /* 递归版本 */ 
    /* 函数FindMin返回二叉搜索树BST中最小元结点的指针 */
	if (BST == NULL) return NULL;
	else {
		if (BST->Left == NULL) return BST;
		else return FindMin(BST->Left);
	}
} 
Position FindMax(BinTree BST) {  /* 递归版本 */
    /* 函数FindMax返回二叉搜索树BST中最大元结点的指针 */
	if (BST == NULL) return NULL;
	else {
		if (BST->Right == NULL) return BST;
		else return FindMax(BST->Right);
	}
} 

int main()
{
    BinTree BST, MinP, MaxP, Tmp;
    ElementType X;
    int N, i;

    BST = NULL;
    scanf("%d", &N);
    for ( i=0; i<N; i++ ) {
        scanf("%d", &X);
        BST = Insert(BST, X);
    }
    printf("Preorder:"); 
	PreorderTraversal(BST); 
	printf("\n");
    MinP = FindMin(BST);
    MaxP = FindMax(BST);
    scanf("%d", &N);
    for( i=0; i<N; i++ ) {
        scanf("%d", &X);
        Tmp = Find(BST, X);
        if (Tmp == NULL) printf("%d is not found\n", X);
        else {
            printf("%d is found\n", Tmp->Data);
            if (Tmp==MinP) printf("%d is the smallest key\n", Tmp->Data);
            if (Tmp==MaxP) printf("%d is the largest key\n", Tmp->Data);
        }
    }
    scanf("%d", &N);
    for( i=0; i<N; i++ ) {
        scanf("%d", &X);
        BST = Delete(BST, X);
    }
    printf("Inorder:"); InorderTraversal(BST); printf("\n");

    return 0;
}
