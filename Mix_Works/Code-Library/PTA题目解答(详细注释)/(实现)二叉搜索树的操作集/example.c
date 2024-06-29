/***************************************************************************************
�����������Ĳ�����
����Ҫ��ʵ�ָ���������������5�ֳ��ò�����
BinTree Insert( BinTree BST, ElementType X );
BinTree Delete( BinTree BST, ElementType X );
Position Find( BinTree BST, ElementType X );
Position FindMin( BinTree BST );
Position FindMax( BinTree BST );
����BinTree�ṹ�������£�
typedef struct TNode *Position;
typedef Position BinTree;
struct TNode{
    ElementType Data;
    BinTree Left;
    BinTree Right;
};
����Insert��X�������������BST�����ؽ�����ĸ����ָ�룻
����Delete��X�Ӷ���������BST��ɾ���������ؽ�����ĸ����ָ�룻���X�������У����ӡһ��Not Found������ԭ���ĸ����ָ�룻
����Find�ڶ���������BST���ҵ�X�����ظý���ָ�룻����Ҳ����򷵻ؿ�ָ�룻
����FindMin���ض���������BST����СԪ����ָ�룻
����FindMax���ض���������BST�����Ԫ����ָ�롣
����������
10
5 8 6 2 4 1 0 10 9 7
5
6 3 10 0 5
5
5 7 0 10 3
���������
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
void PreorderTraversal(BinTree BT); /* ������� */
void InorderTraversal(BinTree BT);  /* ������� */

BinTree Insert( BinTree BST, ElementType X );
BinTree Delete( BinTree BST, ElementType X );
Position Find( BinTree BST, ElementType X );
Position FindMin( BinTree BST );
Position FindMax( BinTree BST );

void PreorderTraversal(BinTree BT) { /* �ݹ�汾 */
	if (BT) {
		printf("%d ", BT->Data);
		PreorderTraversal(BT->Left);
		PreorderTraversal(BT->Right);
	} 
}
void InorderTraversal(BinTree BT) { /* �ݹ�汾 */
	if (BT) {
		InorderTraversal(BT->Left);
		printf("%d ", BT->Data);
		InorderTraversal(BT->Right);
	} 
}
void PostorderTraversal(BinTree BT) { /* �ݹ�汾 */
	if (BT) {
		PreorderTraversal(BT->Left);
		PreorderTraversal(BT->Right);
		printf("%d ", BT->Data);
	}
}
void LevelorderTraversal(BinTree BT); /* �ǵݹ�汾 */

BinTree Insert(BinTree BST, ElementType X) {
	/* Find�����ĸİ� ���Ҳ�����ʱ����в���; ����Insert��X�������������BST�����ؽ�����ĸ����ָ�� */
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
	/* Find�����ĸİ� ���ҵ�������жϺͲ���; 
	����Delete��X�Ӷ���������BST��ɾ���������ؽ�����ĸ����ָ�룻���X�������У�
	���ӡһ��Not Found������ԭ���ĸ����ָ�� */
	if (BST == NULL) {
		printf("Not Found\n");
	} else {
		if (X < BST->Data) BST->Left = Delete(BST->Left, X);
		else if (X > BST->Data) BST->Right = Delete(BST->Right, X);
		else { 
		    Position TmpCell;
            if (BST->Left && BST->Right) { /*BST has both two child */
            	TmpCell = FindMin(BST->Right);
            	BST->Data = TmpCell->Data; /* �ҵ�����������Сֵ���ڸ�����, ������������ */
            	BST->Right = Delete(BST->Right, TmpCell->Data); /* �ݹ�ɾ����������СԪ, ��û�������� */ 
			} else { /* zero childTree or one child, left or right */
            	TmpCell = BST; /* Don't forget! */
            	if (BST->Left) {         /* �������ǿ� */
            		BST = BST->Left; 
				} else if (BST->Right) { /* �������ǿ� */
					BST = BST->Right;
				} else {                 /* ΪҶ�ӽ�� */
					BST = NULL;
				} 
            	free(TmpCell);
			}
		}
	}
	return BST;
}
Position Find(BinTree BST, ElementType X) {  /* �ݹ�汾 */
	/* ����Find�ڶ���������BST���ҵ�X�����ظý���ָ�룻����Ҳ����򷵻ؿ�ָ�� */
	if (BST == NULL) return NULL;
	else {
		if (X < BST->Data) return Find(BST->Left, X);
		else if (X > BST->Data) return Find(BST->Right, X);
		else return BST;
	}
}
Position FindMin(BinTree BST) {  /* �ݹ�汾 */ 
    /* ����FindMin���ض���������BST����СԪ����ָ�� */
	if (BST == NULL) return NULL;
	else {
		if (BST->Left == NULL) return BST;
		else return FindMin(BST->Left);
	}
} 
Position FindMax(BinTree BST) {  /* �ݹ�汾 */
    /* ����FindMax���ض���������BST�����Ԫ����ָ�� */
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
