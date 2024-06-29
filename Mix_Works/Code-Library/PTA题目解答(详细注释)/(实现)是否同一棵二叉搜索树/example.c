/***************************************************************************************
是否同一棵二叉搜索树
给定一个插入序列就可以唯一确定一棵二叉搜索树。然而，一棵给定的二叉搜索树却可以由多种不同的插入序列得到。
例如分别按照序列{2, 1, 3}和{2, 3, 1}插入初始为空的二叉搜索树，都得到一样的结果。于是对于输入的各种插入序列，
你需要判断它们是否能生成一样的二叉搜索树。
输入格式:
输入包含若干组测试数据。每组数据的第1行给出两个正整数N (≤10)和L，分别是每个序列插入元素的个数和
需要检查的序列个数。第2行给出N个以空格分隔的正整数，作为初始插入序列。最后L行，每行给出N个插入的元素，
属于L个需要检查的序列。简单起见，我们保证每个插入序列都是1到N的一个排列。当读到N为0时，标志输入结束，
这组数据不要处理。
输出格式:
对每一组需要检查的序列，如果其生成的二叉搜索树跟对应的初始序列生成的一样，输出“Yes”，否则输出“No”。
输入样例:
4 2
3 1 4 2
3 4 1 2
3 2 4 1
2 1
2 1
1 2
0
输出样例:
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


