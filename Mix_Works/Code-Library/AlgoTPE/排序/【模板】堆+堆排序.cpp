#include <bits/stdc++.h>
using namespace std;
const int maxn = 1010;
#define leftSon(x) ((x) * 2)
#define rightSon(x) ((x) * 2 + 1)
#define father(x) ((x) / 2) 
int heap[maxn]; //大根堆 
void downAdjust(int x) {
	int i = x, j = leftSon(i);
	while (j <= heap[0]) {
		if (j + 1 <= heap[0] && heap[j + 1] > heap[j]) ++j;
		if (heap[i] >= heap[j]) break; //当父节点的值>=子节点中最大的值时,下滤停止 
		swap(heap[i], heap[j]); //调整堆序 
		i = j, j = leftSon(i);
	}
}
void upAdjust(int x) {
	int temp = heap[x], i;
	for (i = x; father(i) >= 1 && heap[father(i)] < temp; i = father(i)) heap[i] = heap[father(i)]; 
	heap[i] = temp;
}
//将堆顶元素同堆最后位置的那个元素交换位置,然后对堆顶位置进行下滤 
void pop() {
	swap(heap[1], heap[heap[0]]);
	--heap[0]; //堆大小减少
	downAdjust(1); 
}
//将新元素加入到堆最后位置,然后执行上滤 
void push(int val) {
	++heap[0]; //堆大小+1
	heap[heap[0]] = val;
	upAdjust(heap[0]); 
}
//判断堆是否为空 
bool empty() { return !heap[0]; }
//返回堆的大小
int size() { return heap[0]; } 
//返回堆顶元素 
int top() { return heap[1]; }
//线性建堆 
void makeHeap(int *arr, int n) {
	//从最后一个有叶子节点的位置开始调整 
	memcpy(heap + 1, arr, sizeof(int) * n); //复制到堆中
	heap[0] = n; //存储堆的大小 
	for (int i = n / 2; i >= 1; --i) downAdjust(i); 
}

int main() {
	int arr[] = {3, 1, 2, 8, 7, 5, 9, 4, 6, 0};
	makeHeap(arr, sizeof(arr) / sizeof(int)); 
	push(11);
	push(99); 
//	while (!empty()) {
//		printf("%d ", top());
//		pop();
//	} 
	int len = size();
	while (!empty()) pop();
	for (int i = 1; i <= len; ++i) printf("%d ", heap[i]); 
	return 0;
}
