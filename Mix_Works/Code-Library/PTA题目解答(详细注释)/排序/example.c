/***************************************************************************************
各种排序算法的实现 
给定N个（长整型范围内的）整数，要求输出从小到大排序后的结果。
本题旨在测试各种不同的排序算法在各种数据情况下的表现。各组测试数据特点如下：
数据1：只有1个元素；
数据2：11个不相同的整数，测试基本正确性；
数据3：10^3个随机整数；
数据4：10^4个随机整数；
数据5：10^5个随机整数；
数据6：10^5个顺序整数；
数据7：10^5个逆序整数；
数据8：10^5个基本有序的整数；
数据9：10^5个随机正整数，每个数字不超过1000。
输入格式:
输入第一行给出正整数N（≤10^5），随后一行给出N个（长整型范围内的）整数，其间以空格分隔。
输出格式:
在一行中输出从小到大排序后的结果，数字间以1个空格分隔，行末不得有多余空格。
输入样例:
11
4 981 10 -17 0 -20 29 50 8 43 -5
输出样例:
-20 -17 -5 0 4 8 10 29 43 50 981
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>

#define SortIndex 5
#define CutOff    100
#define MaxDigit  4
#define Radix     10

typedef int ElementType;
/* the Radix node */
typedef struct Node *PtrToNode;
struct Node{
	ElementType key;
	PtrToNode next;
};
struct HeadNode{
	PtrToNode head, tail;
};
typedef struct HeadNode Bucket[Radix];
/* the sort func */
void SimpleSelectSort(ElementType A[], int num);
void BubbleSort(ElementType A[], int num);
void InsertionSort(ElementType A[], int num);
void ShellSort(ElementType A[], int num);
void HeapSort(ElementType A[], int num);
void MergeSort(ElementType A[], int num);
void QuickSort(ElementType A[], int num);
void LSDRadixSort(ElementType A[], int num);
/*the other func */
ElementType *BuildData(ElementType *A, int *ptrNum);
ElementType *TestData(ElementType *A, int *ptrNum);
void Swap(ElementType *a, ElementType *b);
void printSort(ElementType *A, int num);
void AdjustMaxHeap(ElementType *A, int curIndex, int num);
void MSort(ElementType *A,ElementType *temp, int L, int R);
void Merge(ElementType *A, ElementType *temp, int L, int R, int RightEnd); 
ElementType Meadian3(ElementType *A, int L, int R);
void QSort(ElementType A[], int L, int R);
int GetDigit(ElementType X, int D);

int main()
{
	ElementType *A;
	int num;
	void(*pf[])(ElementType *, int) = {SimpleSelectSort, BubbleSort, InsertionSort, ShellSort,
	                                   HeapSort, MergeSort, QuickSort, LSDRadixSort, };
	A = BuildData(A, &num);
//	A = TestData(A, &num);
	(*pf[SortIndex])(A, num);
	printSort(A, num);
	
	return 0;
}
/* the sort func */
void SimpleSelectSort(ElementType A[], int num)
{
	int i, j, minIndex;
	
	for(i = 0; i < num; i++){
		/* find the minum index of A */
		minIndex = i;
		for(j = i + 1; j < num; j++)
			if(A[j] < A[minIndex])
				minIndex = j;
		/* exchange */
		Swap(&A[i], &A[minIndex]);
	}
}

void BubbleSort(ElementType A[], int num)
{
	int i, j, flag;
	for(j = num - 1; j > 0; j--){
		flag = 0;
		for(i = 0; i < j; i++)
			if(A[i] > A[i+1]){
				Swap(&A[i], &A[i+1]);
				flag = 1;
			}
		if(flag == 0)
			break;
	}
}

void InsertionSort(ElementType A[], int num)
{
	int i, j;
	ElementType temp;
	for(i = 1; i < num; i++){
		temp = A[i];
		for(j = i - 1; j >= 0 && A[j] > temp; j--)
				A[j+1] = A[j];
		A[j+1] = temp;
	}
}

void ShellSort(ElementType A[], int num)
{
	int maxIndex, curIndex, Dk, i, j;
	ElementType temp;
	int Sedgewick[] = {146305, 64769, 36289, 16001, 8929, 3905, 2161, 929, 505, 209, 109, 41, 19, 5, 1, 0};
	/* find the max index */
	for(maxIndex = 0; Sedgewick[maxIndex] > num; maxIndex++)
		;
	for(curIndex = maxIndex; Sedgewick[curIndex] > 0; curIndex++){
		Dk = Sedgewick[curIndex];
		for(i = Dk; i < num; i++){
			/* do the insert sort */
			temp = A[i];
			for(j = i; j >= Dk && A[j-Dk] > temp; j -= Dk)
				A[j] = A[j-Dk];
			A[j] = temp;
		}
	}
}

void HeapSort(ElementType A[], int num)
{
	int curIndex, i;
	/* make the A become maxheap */
	for(curIndex = (num - 2) / 2; curIndex >= 0; curIndex --)
		AdjustMaxHeap(A, curIndex, num);
	for(i = 1; i < num; i++){
		Swap(&A[0], &A[num-i]);
		AdjustMaxHeap(A, 0, num-i);
	}	
}

void MergeSort(ElementType A[], int num)
{
	ElementType *temp;
	temp = (ElementType *)malloc(sizeof(ElementType) * num);
	if(temp != NULL){
		MSort(A, temp, 0, num-1);
		free(temp);
	}
	else 
		printf("The memory is not enough!\n");
}

void QuickSort(ElementType A[], int num)
{
	QSort(A, 0, num-1);
}

void LSDRadixSort(ElementType A[], int num)
{
	int D, Di, i;
	Bucket B;
	PtrToNode tmp, p, List = NULL;
	/* initial the Bucket */
	for(i = 0; i < Radix; i++)
		B[i].head = B[i].tail = NULL;
	for(i = 0; i < num; i++){
		tmp = (PtrToNode)malloc(sizeof(struct Node));
		tmp->key = A[i];
		tmp->next = List;
		List = tmp;
	}
	/* do the sort */
	for(D = 1; D <= MaxDigit; D++){
		/* */
		p = List;
		while(p){
			Di = GetDigit(p->key, D);
			/* remove from list */
			tmp = p; p = p->next;
			/* insert at the end of bucket */
			tmp->next = NULL;
			if(B[Di].head == NULL)
				B[Di].head = B[Di].tail = tmp;
			else {
				B[Di].tail->next = tmp;
				B[Di].tail = tmp;
			}
		}
		/* do the collect */
		List = NULL;
		for(Di = Radix-1; Di >= 0; Di--){
			if(B[Di].head){
				/*insert the head of list*/
				B[Di].tail->next = List;
				List = B[Di].head;
				B[Di].head = B[Di].tail = NULL;
			}
		}
	}
	/* copy the List to A and free List */
	for(i = 0; i < num; i++){
		tmp = List;
		List = List->next;
		A[i] = tmp->key;
		free(tmp);
	}
}

/*the other func */
ElementType *BuildData(ElementType *A, int *ptrNum)
{
	int i;
	scanf("%d", ptrNum);
	A = (ElementType *)malloc(sizeof(ElementType) * (*ptrNum));
	for(i = 0; i < *ptrNum; i++)
		scanf("%d", &A[i]);
	
	return A;
}

ElementType *TestData(ElementType *A, int *ptrNum)
{
	int i;
	ElementType data[] = {4, 981, 10, -17, 0, -20, 29, 50, 8, 43, -5};
	*ptrNum = sizeof(data) / sizeof(data[0]);
	A = (ElementType *)malloc(sizeof(ElementType) * (*ptrNum));
	for(i = 0; i < *ptrNum; i++)
		A[i] = data[i];
	
	return A;
}

void Swap(ElementType *a, ElementType *b)
{
	ElementType temp;
	temp = *b;
	*b = *a;
	*a = temp;
}

void printSort(ElementType *A, int num)
{
	int i;
	printf("%d", A[0]);
	for(i = 1; i < num; i++)
		printf(" %d", A[i]);
	printf("\n");
}

void AdjustMaxHeap(ElementType *A, int curIndex, int num)
{
	int parent, maxchild;
	ElementType temp;
	/* make the A become MaxHeap */
	temp = A[curIndex];
	for(parent = curIndex; (2 * parent + 1) < num; parent = maxchild){
		/* find the max child */ 
		maxchild = 2 * parent + 1;
		if((maxchild + 1) < (num - 1) && (A[maxchild+1] > A[maxchild]))
			maxchild++;
		/* do the compare and adjust */
		if(A[maxchild] > temp)
			A[parent] = A[maxchild];
		else 
			break;
	}
	A[parent] = temp;
}

void MSort(ElementType *A,ElementType *temp, int L, int R)
{
	int Center;
	if(L < R){
		Center = (L + R) / 2;
		MSort(A, temp, L, Center);
		MSort(A, temp, Center + 1, R);
		Merge(A, temp, L, Center + 1, R);
	}
}

void Merge(ElementType *A, ElementType *temp, int L, int R, int RightEnd)
{
	int LeftEnd, num, lIndex, rIndex, tIndex, i;
	LeftEnd = R - 1;
	num = RightEnd - L + 1;
	lIndex = L;
	rIndex = R;
	tIndex = L;
	/* do the merge */
	while(lIndex <= LeftEnd && rIndex <= RightEnd){
		if(A[lIndex] <= A[rIndex])
			temp[tIndex++] = A[lIndex++];
		else 
			temp[tIndex++] = A[rIndex++];
	}
	/* do the rest merge*/
	while(lIndex <= LeftEnd)
		temp[tIndex++] = A[lIndex++];
	while(rIndex <= RightEnd)
		temp[tIndex++] = A[rIndex++];
	/* copy the temp to A */ 		
	for(i = 0; i < num; i++, RightEnd--)
		A[RightEnd] = temp[RightEnd];
}

ElementType Meadian3(ElementType *A, int L, int R)
{
	int Center = (L + R) / 2;
	if(A[L] > A[Center])
		Swap(&A[L], &A[Center]);
	if(A[L] > A[R])
		Swap(&A[L], &A[R]);
	if(A[Center] > A[R])
		Swap(&A[Center], &A[R]);
	Swap(&A[Center], &A[R-1]);
	return A[R-1];
}

void QSort(ElementType A[], int L, int R)
{
	int Low, High;
	ElementType Pivot;
	if(CutOff <= (R - L)){
		/* exchange L Center and R */
		Pivot = Meadian3(A, L, R);
		Low = L;
		High = R-1;
		while(1){
			while(A[++Low] < Pivot);
			while(A[--High] > Pivot);
			if(Low < High)
				Swap(&A[Low], &A[High]);
			else 
				break;
		}
		Swap(&A[Low], &A[R-1]);
		QSort(A, L, Low-1);
		QSort(A, Low+1, R);
	}
	else
		InsertionSort(A+L, R-L+1);
}

int GetDigit(ElementType X, int D)
{
	int d, i;
	for(i = 1; i <= D; i++){
		d = X % Radix;
		X /= Radix;
	}
	return d;
}

