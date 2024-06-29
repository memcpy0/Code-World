/***************************************************************************************
Build a Huffmann Tree
Input Sample:
5
1 2 3 4 5
Output Sample:(Inorder output the tree)
3 6 1 3 2 15 4 9 5
*****************************************************************************************/ 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE      10000
#define MinData      NULL
#define MinHeapROOT  1 
/*HuffmannTree struct data and func*/
typedef struct TreeNode *HuffmannTree;
typedef int TreeElementType;
struct TreeNode{
	TreeElementType Weight;
	HuffmannTree Left;	
	HuffmannTree Right;
};
void printHuffmannTree(HuffmannTree T);
/*MinHeap struct data and func*/
typedef struct StackNode *MinHeap;
typedef struct TreeNode *MinHeapElementType; 
struct StackNode{
	MinHeapElementType* Element;
	int Size;
	int Capacity;
};
MinHeap MinHeapCreate(int MaxSize);
void MinHeapInsert(MinHeap H, MinHeapElementType T);
bool MinHeapIsFull(MinHeap H);
bool MinHeapIsEmpty(MinHeap H);
MinHeapElementType MinHeapDelete(MinHeap H);
void printMinHeap(MinHeap H);

int main()
{
	int num, i;
	TreeElementType item;
	HuffmannTree T;
	MinHeap H;
	
	scanf("%d", &num);
	/*build num of Tree and insert into MinHeap*/
	H = MinHeapCreate(MAXSIZE);
	for(i = 0; i < num; i++){
		T = (HuffmannTree )malloc(sizeof(struct TreeNode));
		scanf("%d", &T->Weight);
		T->Left = T->Right = NULL;
		MinHeapInsert(H, T);
	}
	printf("The input MinHeap is:");printMinHeap(H);printf("\n");
	/*build the Huffmann Tree*/
	for(i = 0; i < (num -1); i++){
		T = (HuffmannTree )malloc(sizeof(struct TreeNode));
		T->Left = MinHeapDelete(H);
		T->Right = MinHeapDelete(H);
		T->Weight = T->Left->Weight + T->Right->Weight;
		MinHeapInsert(H, T);
		printf("The MinHeap now is:");printMinHeap(H);printf("\n");
	}
	T = MinHeapDelete(H);
	/*print the Huffmann Tree*/
	printf("The Huffmann Tree is:");printHuffmannTree(T);printf("\n");
	
	return 0;
}

MinHeap MinHeapCreate(int MaxSize)
{
	MinHeap H;
	H = (MinHeap )malloc(sizeof(struct StackNode));
	H->Element =(MinHeapElementType *)malloc(sizeof(MinHeapElementType) * MaxSize + 1);
	H->Size = 0;
	H->Capacity = MaxSize;
	H->Element[0] = MinData;
	
	return H;
}
void MinHeapInsert(MinHeap H, MinHeapElementType T)
{
	int cur, parent;
	if( !MinHeapIsFull(H) ){
		/*add item at the end to make it full bintree*/
		H->Element[++(H->Size)] = T;
		/*adjust to make it MinHeap tree(attention: compare the weight not the point here)*/
		for(cur = H->Size; (cur / 2 > 0) && (H->Element[cur/2]->Weight > T->Weight); cur /= 2)
			H->Element[cur] = H->Element[cur / 2];
		H->Element[cur] = T;
	}
	else 
		printf("The MinHeap is Full, insert is failed!\n"); 
}

bool MinHeapIsFull(MinHeap H)
{
	return (H->Size == H->Capacity)? true: false;
}

bool MinHeapIsEmpty(MinHeap H)
{
	return (H->Size == 0)? true: false;
}

MinHeapElementType MinHeapDelete(MinHeap H)
{
	MinHeapElementType T, temp;
	int cur, minchild;
	if( !MinHeapIsEmpty(H) ){
		T = H->Element[MinHeapROOT];
		MinHeapElementType temp = H->Element[H->Size];
		for(cur = MinHeapROOT; (cur * 2) <= H->Size; cur = minchild){
			/*find the minchild of cur*/
			minchild = cur * 2;
			if((minchild + 1) <= H->Size && (H->Element[minchild + 1]->Weight < H->Element[minchild]->Weight))
				minchild ++;
			H->Element[cur] = H->Element[minchild];
		}
		H->Element[cur] = H->Element[(H->Size)--];
	}
	else {
		printf("The MinHeap is Empty, delete is failed!\n");
		T = MinData;
	}
	
	return T;	
}

void printMinHeap(MinHeap H)
{
	int i;
	for(i = 1; i <= H->Size; i++)
		printf("%d ", H->Element[i]->Weight);
}

void printHuffmannTree(HuffmannTree T)
{
	if(T){
		printHuffmannTree(T->Left);
		printf("%d ", T->Weight);
		printHuffmannTree(T->Right);
	}
}
