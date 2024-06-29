/***************************************************************************************
树的同构 
给定两棵树T1和T2。如果T1可以通过若干次左右孩子互换就变成T2，则我们称两棵树是"同构"的。
输入格式： 
输入给出2棵二叉树树的信息。对于每棵树，首先在一行中给出一个非负整数N (≤10)，
即该树的结点数（此时假设结点从0到N-1编号）；随后N行，第i行对应编号第i个结点，
给出该结点中存储的1个英文大写字母、其左孩子结点的编号、右孩子结点的编号。
如果孩子结点为空，则在相应位置上给出“-”。给出的数据间用一个空格分隔。
注意：题目保证每个结点中存储的字母是不同的。
输出格式:
如果两棵树是同构的，输出“Yes”，否则输出“No”
输入样例： 
8
A 1 2
B 3 4
C 5 -
D - -
E 6 -
G 7 -
F - -
H - -
8
G - 4
B 7 6
F - -
A 5 1
H - -
C 0 -
D - -
E 2 -
输出样例：Yes 
*****************************************************************************************/ 

#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize     10
#define ElementType char
#define Null        -1

typedef int Tree;

typedef struct {
	ElementType Data;
	Tree Left;
	Tree Right;
}BTreeNode; 
BTreeNode tree1[MaxSize], tree2[MaxSize]; 

Tree CreateTree(BTreeNode *tree);
void printTree(Tree root);
bool Isisomorph(Tree t1, Tree t2);

int main()
{
	Tree root1, root2;
	
	root1 = CreateTree(tree1);
	root2 = CreateTree(tree2);
	if(Isisomorph(root1, root2))
		printf("Yes\n");
	else 
		printf("No\n");
	
	return 0;
}

Tree CreateTree(BTreeNode *tree)
{
	int num, i;
	char in1, in2;
	Tree root;
	
	scanf("%d\n", &num); 
	if(num){
		int *check;
		check = (int *)malloc(num * sizeof(int));
		for(i = 0; i < num; i++)
			check[i] = 1;
		for(i = 0; i < num; i++){
			scanf("%c %c %c\n", &tree[i].Data, &in1, &in2);
			if(in1 != '-'){
				tree[i].Left = in1 - '0';
				check[tree[i].Left] = 0;
			}
			else 
				tree[i].Left = Null;
			if(in2 != '-'){
				tree[i].Right = in2 - '0';
				check[tree[i].Right] = 0;
			}
			else 
				tree[i].Right = Null;
		}
		/*find the root of tree*/
		for(i = 0; !check[i] && (i < num); i++) ;
		root = i;
		/*free the check space*/
		free(check); 
	}
	else
		root = Null; 
	
	return root;
}

void printTree(Tree root)
{
	
} 

bool Isisomorph(Tree t1, Tree t2)
{
	bool flag;
	if(t1 == Null && t2 == Null)
		flag = true;
	else if(t1 == Null || t2 == Null)
		flag = false;
	else {
		/*compare the data of root*/ 
		if(tree1[t1].Data != tree2[t2].Data)
			flag = false;
		else {
		/*compare the left and right of root*/
		if(Isisomorph(tree1[t1].Left, tree2[t2].Left) && Isisomorph(tree1[t1].Right, tree2[t2].Right))
			flag = true;
		else if(Isisomorph(tree1[t1].Left, tree2[t2].Right) && Isisomorph(tree1[t1].Right, tree2[t2].Left))
			flag = true;
		else 
			flag = false;
		}
	}
		
	return flag;
}
