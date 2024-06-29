//#include <stdio.h>
//
//#define MAXN 10
//typedef float ElementType;
//
//ElementType Median( ElementType A[], int N );
//
//int main ()
//{
//    ElementType A[MAXN];
//    int N, i;
//
//    scanf("%d", &N);
//    for ( i=0; i<N; i++ )
//        scanf("%f", &A[i]);
//    printf("%.2f\n", Median(A, N)); 
//    return 0;
//}
// 
//
//// 会超时
//// void InsertionSort(ElementType* A, int N) {
////     for (int i = 1; i < N; ++i) {
////         ElementType temp = A[i];
////         int j;
////         for (j = i; j > 0 && A[j - 1] < temp; --j) A[j] = A[j - 1];
////         A[j] = temp;
////     }
//// } 
//
//int Partition(ElementType* A, int lo, int hi) {
//    if (lo >= hi) return lo;  // 只有1个 
//    ElementType t = A[lo]; 
//    while (lo < hi) { // 夹出一个位置 
//        while (lo < hi && A[hi] > t) --hi; 
//        A[lo] = A[hi];
//        while (lo < hi && A[lo] <= t) ++lo; 
//        A[hi] = A[lo];
//    }
//    A[lo] = t; // 从while中退出时，lo等于hi，lo的元素已经使用过了 
//    return lo; 
//}
//
//void QSort(ElementType* A, int left, int right) {
//    if (left < right) {  
//        int mid = Partition(A, left, right);
//        QSort(A, left, mid);
//        QSort(A, mid + 1, right);
//    }
//}
//
//void QuickSort(ElementType* A, int N) { 
//    QSort(A, 0, N - 1);
//}
//
//void BubbleSort(ElementType A[], int N) {
//    for (int i = N - 1; i > 0; --i) {
//        for (int j = 0; j < i; ++j) {
//            if (A[j] < A[j + 1]) {
//                ElementType t = A[j]; 
//                A[j] = A[j + 1];
//                A[j + 1] = t;
//            }
//        }
//    }
//}
//ElementType Median(ElementType A[], int N) {
//    // InsertionSort(A, N);
//    // 坑点：
//    // 1. (N+1)/2向下取整，在C语言中和直接写成(N+1)/2效果一样
//    // 2. 第几大，实际上是按照从大到小的顺序，第一大就是最大，第二大就是次大; 还有第几小
//    // 3. 如果是实际题目不给出来测试点，可能最后一个就想不到是超时的问题
//    QuickSort(A, N);
//    BubbleSort(A, N);
//	for (int i = 0; i < N; ++i)printf("%f\n", A[i]); 
//    return A[(N + 1) / 2 - 1];
//}
