/******************************************************************************************************************************
迷宫问题 
*********************************************************************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAXSIZE 100
#define EndDir   8
 
typedef struct{
	int RowOffset;
	int ColOffset;
}Offset;

typedef struct{
	int Row;
	int Col;
	int Dir;
}ElementType;

typedef struct{
	ElementType Data[MAXSIZE];
	int Top;
}Stack;

Stack* CreateStack(void);
bool IsFull(Stack* ptrS);
bool IsEmpty(Stack* ptrS);
void Push(Stack* ptrS, ElementType item);
ElementType Pop(Stack* ptrS);

static int Maze[][17] = {   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
					        {1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1},
							{1,1,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1},
							{1,0,1,0,0,0,0,0,1,1,1,1,0,0,1,1,1},
							{1,1,1,0,1,1,1,1,0,0,1,0,1,1,0,0,1},
							{1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,1},
							{1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,1,1},
							{1,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1},
							{1,0,0,1,1,0,1,1,0,1,1,1,1,1,0,1,1},
							{1,1,1,0,0,0,1,1,0,0,1,0,0,0,0,0,1},
							{1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,0,1},
							{1,0,1,0,0,0,0,1,1,1,0,1,1,1,1,0,1},
							{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}  };

static Offset Mov[] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};

int main()
{
	int ExitRow, ExitCol;
	bool isFound;
	Stack* ptrS;
	ElementType curPosition, nextPosition;
	int route[MAXSIZE][MAXSIZE] =  {0};
	
	ExitRow = sizeof(Maze) / sizeof(Maze[0]) - 2;
	ExitCol = sizeof(Maze[0]) / sizeof(Maze[0][0]) - 2;
	ptrS = CreateStack();
	isFound = false;
	curPosition.Row =  curPosition.Col = 1;
	curPosition.Dir = 2;
	Push(ptrS, curPosition);
	while((!isFound) && (!IsEmpty(ptrS))){
		curPosition = Pop(ptrS);
		while(curPosition.Dir != EndDir){
			/*judge if exit*/
			if(curPosition.Row == ExitRow && curPosition.Col == ExitCol){
				isFound = true;
				break;
			}
			/*get nextPosition*/
			nextPosition.Row = curPosition.Row + Mov[curPosition.Dir].RowOffset;
			nextPosition.Col = curPosition.Col + Mov[curPosition.Dir].ColOffset;
			nextPosition.Dir = 0;		
			/*judge nextPosition is through or not in route*/
			if((Maze[nextPosition.Row][nextPosition.Col] == 0) && 
				(route[nextPosition.Row][nextPosition.Col] == 0)){
				curPosition.Dir++;
				Push(ptrS, curPosition);
				curPosition = nextPosition;	
				route[curPosition.Row][curPosition.Col] = 1;
			}
			else 
				curPosition.Dir++;
		}
	}
	if(isFound){
		int i;
		printf("找到了一条迷宫出去的路径：\n");
		for(i = 0; i <= ptrS->Top; i++){
			printf("(%d %d)-->", ptrS->Data[i].Row, ptrS->Data[i].Col);
			if(((i+1)%10) == 0)
				printf("\n");
		}
		printf("(%d %d)\n", ExitRow, ExitCol);
	}
	else 
		printf("此迷宫无出去的路线！\n");
	
	return 0; 
}

Stack* CreateStack(void)
{
	Stack* s;
	s = (Stack* )malloc(sizeof(Stack));
	s->Top = -1;
	
	return s;
}

bool IsFull(Stack* ptrS)
{
	return (ptrS->Top == MAXSIZE-1)? true: false;
} 
bool IsEmpty(Stack* ptrS)
{
	return (ptrS->Top == -1)? true: false;
}
void Push(Stack* ptrS, ElementType item)
{
	/*判断堆栈是否已满*/
	if(IsFull(ptrS))
		printf("堆栈已满，压栈操作失败！\n");
	else 
		ptrS->Data[++(ptrS->Top)] = item;
}

ElementType Pop(Stack* ptrS)
{
	ElementType data;
	/*判断堆栈是否空*/
	if(IsEmpty(ptrS)){
		printf("堆栈已空，弹出失败！\n");
		data.Row = data.Col = data.Dir = 0;
	}
	else 
		data = ptrS->Data[(ptrS->Top)--];	
	
	return data;
}

