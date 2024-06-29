/***************************************************************************************
���е�·��
��һϵ�и������ֲ���һ����ʼΪ�յ�С����H[]����������������±�i����ӡ��H[i]��������·����
�����ʽ:
ÿ����Ե�1�а���2��������N��M(��1000)���ֱ��ǲ���Ԫ�صĸ������Լ���Ҫ��ӡ��·��������
��һ�и�������[-10000, 10000]�ڵ�N��Ҫ������һ����ʼΪ�յ�С���ѵ����������һ�и���M���±ꡣ
�����ʽ:
�������и�����ÿ���±�i����һ���������H[i]��������·���ϵ����ݡ����ּ���1���ո�ָ�����ĩ�����ж���ո�
��������:
5 3
46 23 26 24 10
5 4 3
�������:
24 23 10
46 23 10
26 10
*****************************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 1000    /*the max size of minheap*/
#define MINDATA -10001  /*the guard of minheap*/
#define ROOTINDEX 1
typedef int ElementType;
typedef struct HeapStruct *MinHeap;
struct HeapStruct{
	ElementType *Element;
	int Size;
	int Capacity;
};

MinHeap Create(int maxSize);
MinHeap Insert(MinHeap H, ElementType data);
ElementType DeleteMin(MinHeap H);
bool IsFull(MinHeap H);
bool IsEmpty(MinHeap H);
void PrintRoute(MinHeap H, int index);
void printMinHeap(MinHeap H);
MinHeap Adjust(MinHeap H);

int main()
{
	int i, numData, numRoute, index;
	ElementType data;
	MinHeap H;
	
	scanf("%d", &numData);
	scanf("%d", &numRoute);
	/*use the insert func to build the minheap*/
	H = Create(MAXSIZE);
	for(i = 0; i < numData; i++){
		scanf("%d", &data);	
		H = Insert(H, data);
	}
//	/*use the adjust func to build the minheap*/
//	H = Create(MAXSIZE);
//	for(i = 0; i < numData; i++){
//		scanf("%d", &data);
//		H->Element[++(H->Size)] = data;
//	}
//	H = Adjust(H);
//	printf("the current MinHeap is:");
//	printMinHeap(H);

	/*print the route*/
	for(i = 0; i < numRoute; i++){
		scanf("%d", &index);
		PrintRoute(H, index);
	}
//	{	/*test the DeleteMin Func*/
//		int item, i, num;
//		MinHeap T;
//		
//		printf("the current MinHeap is:");
//		printMinHeap(H);
//		for(i = 0; i < 10; i++){
//			item = DeleteMin(H);
//			printf("Delete the %d, and the rest of MinHeap is:", item);
//			printMinHeap(H);
//		}
	
	return 0;
}

MinHeap Create(int maxSize)
{
	MinHeap H;
	H = (MinHeap)malloc(sizeof(struct HeapStruct));
	H->Element = (ElementType *)malloc(sizeof(ElementType) * maxSize + 1);
	H->Element[0] = MINDATA;
	H->Size = 0;
	H->Capacity = maxSize;
	
	return H; 
} 

MinHeap Insert(MinHeap H, ElementType data)
{
	int i;
	if(IsFull(H) == false){
		/*add the node at the end to make it a full bin tree*/
		H->Element[++(H->Size)] = data;
		/*adjust to make it minheap*/
		for(i = H->Size; H->Element[i/2] > data; i /= 2)
			H->Element[i] = H->Element[i/2];
		H->Element[i] = data;
	}
	
	return H;
}

bool IsFull(MinHeap H)
{
	if(H->Size == H->Capacity)
		return true;
	else 
		return false;
}

bool IsEmpty(MinHeap H)
{
	if(H->Size ==0 )
		return true;
	else 
		return false;
}

ElementType DeleteMin(MinHeap H)
{
	int cur, minchild;
	ElementType result;
	if( IsEmpty(H) ){
		result = H->Element[0];
		printf("the MinHeap is Empty, Delete is failed!\n");
		return result;
	}
	result = H->Element[ROOTINDEX];
	cur = ROOTINDEX;
	for( ; cur * 2 <= H->Size; cur = minchild){
		/*find the minchild of cur*/
		minchild = 2 * cur;
		if((minchild + 1) <= H->Size)
				minchild = (H->Element[minchild] < H->Element[minchild + 1])? (2 * cur): (2 * cur + 1);	
		/*find the location*/
		if(H->Element[H->Size] <= H->Element[minchild])
			break;
		else 
			H->Element[cur] = H->Element[minchild];	
	}
	H->Element[cur] = H->Element[H->Size--];
	
	return result;
}

void PrintRoute(MinHeap H, int index)
{
	int i;
	for(i = index; i > ROOTINDEX; i /= 2)
		printf("%d ", H->Element[i]);
	printf("%d\n", H->Element[i]);
}

void printMinHeap(MinHeap H)
{
	int i;
	if(IsEmpty(H))
		printf("the MinHeap is empty!");
	else 
		for(i = 1; i <= H->Size; i++)
			printf("%d ", H->Element[i]);	
	printf("\n");
}

MinHeap Adjust(MinHeap H)
{
	int cur, temp, minchild;
	ElementType exchange;
	for(cur = H->Size / 2; cur > 0; cur--){
		for(temp = cur; temp * 2 <= H->Size; temp = minchild){
			/*find the minchild of temp*/
			minchild = temp * 2;
			if(((minchild + 1) <= H->Size) && (H->Element[minchild + 1] < H->Element[minchild]))
				minchild ++;
			/*compare the value of temp and minchild*/
			if(H->Element[minchild] < H->Element[temp]){
				exchange = H->Element[temp];
				H->Element[temp] = H->Element[minchild];
				H->Element[minchild] = exchange;
			}
			else 
				break;
		}
	}
	
	return H;
}

