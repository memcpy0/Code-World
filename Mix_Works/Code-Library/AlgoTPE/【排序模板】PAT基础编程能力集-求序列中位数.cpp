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
//// �ᳬʱ
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
//    if (lo >= hi) return lo;  // ֻ��1�� 
//    ElementType t = A[lo]; 
//    while (lo < hi) { // �г�һ��λ�� 
//        while (lo < hi && A[hi] > t) --hi; 
//        A[lo] = A[hi];
//        while (lo < hi && A[lo] <= t) ++lo; 
//        A[hi] = A[lo];
//    }
//    A[lo] = t; // ��while���˳�ʱ��lo����hi��lo��Ԫ���Ѿ�ʹ�ù��� 
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
//    // �ӵ㣺
//    // 1. (N+1)/2����ȡ������C�����к�ֱ��д��(N+1)/2Ч��һ��
//    // 2. �ڼ���ʵ�����ǰ��մӴ�С��˳�򣬵�һ�������󣬵ڶ�����Ǵδ�; ���еڼ�С
//    // 3. �����ʵ����Ŀ�����������Ե㣬�������һ�����벻���ǳ�ʱ������
//    QuickSort(A, N);
//    BubbleSort(A, N);
//	for (int i = 0; i < N; ++i)printf("%f\n", A[i]); 
//    return A[(N + 1) / 2 - 1];
//}
