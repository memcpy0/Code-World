/***************************************************************************************
�Ƿ�ͬһ�ö���������
����һ���������оͿ���Ψһȷ��һ�ö�����������Ȼ����һ�ø����Ķ���������ȴ�����ɶ��ֲ�ͬ�Ĳ������еõ���
����ֱ�������{2, 1, 3}��{2, 3, 1}�����ʼΪ�յĶ��������������õ�һ���Ľ�������Ƕ�������ĸ��ֲ������У�
����Ҫ�ж������Ƿ�������һ���Ķ�����������
�����ʽ:
�������������������ݡ�ÿ�����ݵĵ�1�и�������������N (��10)��L���ֱ���ÿ�����в���Ԫ�صĸ�����
��Ҫ�������и�������2�и���N���Կո�ָ�������������Ϊ��ʼ�������С����L�У�ÿ�и���N�������Ԫ�أ�
����L����Ҫ�������С�����������Ǳ�֤ÿ���������ж���1��N��һ�����С�������NΪ0ʱ����־���������
�������ݲ�Ҫ����
�����ʽ:
��ÿһ����Ҫ�������У���������ɵĶ�������������Ӧ�ĳ�ʼ�������ɵ�һ���������Yes�������������No����
��������:
4 2
3 1 4 2
3 4 1 2
3 2 4 1
2 1
2 1
1 2
0
�������:
Yes
No
No
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int ElementType;
typedef struct _tnode{
	ElementType Data;
	struct _tnode *Left;
	struct _tnode *Right;
}BTNode;
typedef BTNode *BST;
typedef BST Position;

BST Insert(BST root, ElementType item);
bool IsEqualTree(BST t1, BST t2);

int main()
{
	int N, L, i, sumL, num;
	bool result[100];
	ElementType data;
	BST templateTree, compareTree;
	
	sumL = num = 0;
	do{
		scanf("%d", &N);
		if(N){
			scanf("%d", &L);
			sumL += L;
			/*build template tree*/
			templateTree = NULL;
			for(i = 0; i < N; i++){
				scanf("%d", &data);
				templateTree = Insert(templateTree, data);
			}
			/*build compare tree and print the result*/
			while( L-- ){
				/*build compare tree*/
				compareTree = NULL;
				for(i = 0; i < N; i++){
					scanf("%d", &data);
					compareTree = Insert(compareTree, data);
				}
				/*do the compare*/
				if(IsEqualTree(templateTree, compareTree))
					result[num++] = 1;
				else 
					result[num++] = 0;
			}
		}
	}while(N);
	/*print the result*/
	for(i = 0; i < sumL; i++){
		if(result[i])
			printf("Yes\n");
		else 
			printf("No\n");
	}
	
	return 0;
}

BST Insert(BST root, ElementType item)
{
	if(root == NULL){
		root = (BTNode* )malloc(sizeof(BTNode));
		root->Data = item;
		root->Left = root->Right = NULL;
	}
	else {
		if(item > root->Data)
			root->Right = Insert(root->Right, item);
		else if(item < root->Data)
			root->Left = Insert(root->Left, item);
	}
	
	return root;
}

bool IsEqualTree(BST t1, BST t2)
{
	bool flag;
	if(t1 == NULL && t2 == NULL)
		flag = true;
	else if(t1 == NULL || t2 == NULL)
		flag = false;
	else {
		if((t1->Data == t2->Data) && IsEqualTree(t1->Left, t2->Left) && IsEqualTree(t1->Right, t2->Right))
			flag = true;
		else 
			flag = false;
	}
		
	return flag;
}


