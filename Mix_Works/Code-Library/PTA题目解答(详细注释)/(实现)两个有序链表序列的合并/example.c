/******************************************************************************************************************************
��Ŀ�����������������еĺϲ� 
��֪�����ǽ�����������S1��S2����ƺ��������S1��S2�Ĳ����·ǽ�������S3��
�����ʽ:
��������У��ֱ���ÿ�и��������ɸ����������ɵķǽ������У���-1��ʾ���еĽ�β��-1������������У��������ÿո�����
�����ʽ:
��һ��������ϲ����µķǽ����������ּ��ÿո�ֿ�����β�����ж���ո���������Ϊ�գ����NULL��
��������:
1 3 5 -1
2 4 6 8 10 -1
�������:
1 2 3 4 5 6 8 10
*********************************************************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

#define END -1

typedef int ElementType;
typedef struct LNode* PtrToLNode;
typedef PtrToLNode List;
typedef PtrToLNode Position;
struct LNode{
	ElementType Data;
	PtrToLNode Next;
};

List ScanList(void);
List ComList(List L1, List L2);
void PrintList(List L);
List Insert(List L, ElementType Data, List *Ltail);

int main()
{
	List S1, S2, S3;
	S1 = ScanList();
	S2 = ScanList();
	
	S3 = ComList(S1, S2);
	
	PrintList(S3);
	
    return 0;
}

List ScanList(void)
{
	List L, cur;
	ElementType data;
	L = NULL;
	do{
		scanf("%d", &data);	
		if(data != END){
			List ptr = (List)(malloc(sizeof(struct LNode)));
			ptr->Data = data;
			ptr->Next = NULL;
			if(L == NULL)
				L = ptr;
			else{
				for(cur = L; cur->Next; cur = cur->Next);
				cur->Next = ptr;
			}
		}
	}while(data != END);
	
	return L;
}

List Insert(List L, ElementType Data, List *Ltail)
{
	List ptr = (List)(malloc(sizeof(struct LNode)));
	ptr->Data = Data;
	ptr->Next = NULL;
	if(L == NULL){
		L = ptr;
		*Ltail = ptr;
	}
	else {
		(*Ltail)->Next = ptr;
		*Ltail = ptr;
	}
	
	return L;
}
List ComList(List L1, List L2)
{
	List L3, L3tail;
	L3 = L3tail = NULL;
	while(L1 && L2){
		if(L1->Data < L2->Data){
			L3 = Insert(L3, L1->Data, &L3tail);
			L1 = L1->Next;
		}
		else if(L1->Data == L2->Data){
			L3 = Insert(L3, L1->Data, &L3tail);
			L3 = Insert(L3, L2->Data, &L3tail);
			L1 = L1->Next;
			L2 = L2->Next;
		}
		else {
			L3 = Insert(L3, L2->Data, &L3tail);
			L2 = L2->Next;
		}
	}
	while(L1){
		L3 = Insert(L3, L1->Data, &L3tail);
		L1 = L1->Next;
	}
	while(L2){
		L3 = Insert(L3, L2->Data, &L3tail);
		L2 = L2->Next;
	}
	
	return L3;
}
void PrintList(List L)
{
	if(L == NULL)
		printf("NULL\n");
	else {
		for(; L; L = L->Next){
			printf("%d", L->Data);
			if(L->Next != NULL)
				printf(" ");
		}
	}
}

