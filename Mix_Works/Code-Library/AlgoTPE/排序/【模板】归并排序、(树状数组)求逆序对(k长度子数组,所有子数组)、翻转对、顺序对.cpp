/*���ε�������:
�������⣺�����зֳ�Ԫ�ظ���������ȵ����롣
�ݹ���⣺������Ԫ�طֱ�����
�ϲ����⣺�����������ϲ���һ����

�ؼ�������ΰ����������ϳ�һ����
ÿ��ֻ��Ҫ���������е���СԪ�ؼ��ԱȽϣ�ɾ�����еĽ�СԪ�ز�����ϲ�����±��ɡ�
������Ҫһ���±�����Ž�������Ը��ӿռ�Ϊn.

�鲢�����ʱ�临�ӶȺ���������͵ķ����㷨һ��������O(nlogn)��
*/
void mergeSort(int *A, int x, int y, int *T) { //�鲢��������ҿ�����[x,y)
	if (y - x > 1) { //����һ��Ԫ��
        int m = x + (y - x) / 2; //����
        mergeSort(A, x, m, T);   //�ݹ����
        mergeSort(A, m, y, T);   //�ݹ����
        int p = x, q = m, i = x;
        while (p < m || q < y) { //�ҿ����䣬ֻҪ��һ�����зǿգ���Ҫ�����ϲ�
            //����������� 
			//(q�����&&pû�����)||(qû�����&&pû�����&&A[p]<=A[q]
            if (q >= y || (p < m && A[p] <= A[q])) T[i++] = A[p++]; //��i��ʼ //��������鸴�Ƶ���ʱ�ռ�
			//qû�����&&(p�����||A[p]>A[q])
			else T[i++] = A[q++]; //���Ұ����鸴�Ƶ���ʱ�ռ�
		}
		for (int i = x; i < y; ++i) A[i] = T[i]; //�Ӹ����ռ临�ƻ�A����
	}
}
//��һ�ֹ鲢д��:
void mergeSort(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSort(A, l, m, T);     //�ݹ����
		mergeSort(A, m + 1, r, T); //�ݹ����
		int i = l, j = m + 1, k = i;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			//i<j && A[i]>A[j]
			else T[k++] = A[j++];
			//���ұߵ�A[j]���Ƶ�T��ʱ����߻�û���ü����Ƶ�T����Щ������������б�A[j]�����
			//��ʱ���ۼ����м������Ԫ�ظ���m-i+1����
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = l; x <= r; ++x) A[x] = T[x];
	}
}
//��һ�ֹ鲢д��:
void merge(int a[], int l1, int r1, int l2, int r2, int t[]) {
	int index = 0, p = l1, q = l2;
	while (p <= r1 && q <= r2) {
		while (p <= r1 && a[p] <= a[q]) { //Ϊ<=
			t[index++] = a[p++];
		}
		while (q <= r2 && a[q] <= a[p]) { //Ϊ<=
			t[index++] = a[q++];
		}
	}
	while (p <= r1) t[index++] = a[p++];
	while (q <= r2) t[index++] = a[q++];
	for (int i = 0; i < index; ++i) a[l1 + i] = t[i]; //���ϲ�������и�ֵ������A
}
void mergeSort(int a[], int s, int e) {
	if (s < e) { //���ٻ�������Ԫ��
		int mid = s + (e - s) / 2;
		mergeSort(a, s, mid);
		mergeSort(a, mid + 1, e);
		merge(a, s, mid, mid + 1, e, temp);
	}
}
//------------------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
using namespace std;
const int maxn = 500010;
long long A[maxn], T[maxn], n, cnt = 0;
/*��������⡣��һ����a1, a2, �� , an��������������������ж��ٸ������(i,j)��
ʹ��i<j��a_i>a_j��n���Ըߴ�10^6
1.n��ô��O(n^2)��ö�ٽ���ʱ�������ҪѰ�Ҹ���Ч�ķ���
2.�ܵ���������������͡����ι鲢�����Ӱ�죬���ԡ��������������Ƿ�����
- ���������⡱�����ǰ����зֳ�Ԫ�ظ���������ȵ����룻
- ���ݹ���⡱��ͳ��i��j������߻��߾����ұߵ�����Ը�����
- ���ϲ����⡱����ͳ��i����ߣ���j���ұߵ�����Ը�����

�ؼ����ںϲ���������i����ߣ���j���ұߵ��������Ŀ�أ�

����j�Ĳ�ͬ����Щ����Խ���ߡ�������Խ��з��ࣺ
ֻҪ�����ұߵ�ÿ��j��ͳ����߱������Ԫ�ظ���f(j)��������f(j)֮�ͱ��Ǵ𰸡�
�鲢������ԡ�˳�㡱���f(j)�ļ��㣺
���ںϲ������Ǵ�С������еģ����ұߵ�A[j]���Ƶ�T��ʱ����߻�û���ü����Ƶ�T����Щ������������б�A[j]���������
ʱ���ۼ����м������Ԫ�ظ���m-p���ɣ������ʣ��Ԫ��������[p,m)�У����Ԫ�ظ���Ϊm-p��
*/
void mergeSortAndCount(int *A, int x, int y, int *T) { //[x,y)
	if (y - x > 1) {
		int m = x + (y - x) / 2;
		mergeSortAndCount(A, x, m, T);
		mergeSortAndCount(A, m, y, T);
		int p = x, q = m, i = x;
		while (p < m || q < y) {
			if (q >= y || (p < m && A[p] <= A[q])) T[i++] = A[p++];
			//qû�����&&(p�����||A[p]>A[q])
			else {
				T[i++] = A[q++];
				cnt += m - p; //��߻�û���ü����Ƶ�T����Щ������������б�A[j]�������
				//��ʱ���ۼ����м������Ԫ�ظ���m-p����
			}
		}
		for (int i = x; i < y; ++i) A[i] = T[i];
	}
}
//����д��2
void mergeSortAndCount2(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAndCount2(A, l, m, T);     //�ݹ����
		mergeSortAndCount2(A, m + 1, r, T); //�ݹ����
		int i = l, j = m + 1, k = i;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			//i<j && A[i]>A[j]
			else { T[k++] = A[j++]; cnt += j - k; } //mid - i + 1: �����; i - l: ˳��� 
			//���ұߵ�A[j]���Ƶ�T��ʱ����߻�û���ü����Ƶ�T����Щ������������б�A[j]�����
			//��ʱ���ۼ����м������Ԫ�ظ���m-i+1����
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = l; x <= r; ++x) A[x] = T[x];
	}
}
//���������ڽ��������򣬶�����������ԣ����Ų����ԣ����Ų����ԣ����롢ð��
//����д��3
int cnt = 0;
void mergeSortAndCount3(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAndCount3(A, l, m, T);
		mergeSortAndCount3(A, m + 1, r, T);
		int i = l, j = m + 1, k = l;

		//����i�Ĳ�ͬ����Щ����Խ���ߡ�������Խ��з��ࣺ
		//ֻҪ������ߵ�ÿ��i��ͳ���ұ߱���С��Ԫ�ظ���f(j)��������f(j)֮�ͱ��Ǵ𰸡�
		//�鲢������ԡ�˳�㡱���f(j)�ļ���
		for (i = l; i <= m; ++i) {
			while (j <= r && A[i] > A[j]) ++j; //ֱ��A[i]<=A[j]
			cnt += j - m - 1; //A[i]>����[m+1,j)����A[j]: �����; A[i]<=����[j, r]����A[j]: ˳��� 
		}
		//���߰���j�Ĳ�ͬ����Щ����Խ���ߡ�������Խ��з��ࣺ
		//ֻҪ�����ұߵ�ÿ��j��ͳ����߱������Ԫ�ظ���f(j)��������f(j)֮�ͱ��Ǵ𰸡�������ǰѰ�ң� 
		//�鲢������ԡ�˳�㡱���f(j)�ļ���
		for (j = m + 1; j <= r; ++j) {
			while (i <= m && A[i] <= A[j]) ++i; //ֱ��A[i]>A[j]
			cnt += m - i + 1; //[i,m]�е�A[i]��>A[j]: �����; [l, i - 1]�е�A[i]��<=A[j], ˳��� 
		}
		i = l, j = m + 1;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			else T[k++] = A[j++];
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = l; x <= r; ++x) A[x] = T[x];
	}
}
 
int main() {
	scanf("%lld", &n);
	for (int i = 0; i < n; ++i)
		scanf("%lld", &A[i]);
	mergeSortAndCount3(A, 0, n - 1, T);
	cout << unorderPairs;
	return 0;
}



//�ϸ�˳��ԣ��ص㣩������������� LeetCode��ע�����arr[i]��Ԫ����Ŀ������ 
struct kv { int v, p; };
const int maxn = 1e5 + 10;
class Solution {
private:
    kv ins[maxn]; //��¼Ԫ��ֵ������ԭ�����е��±�
    int Left[maxn] = {0}, Right[maxn] = {0}; //��¼ԭ�����е��±��Ӧ��ֵ,���������ʱС�ڻ��ߴ��ڸ�ֵ�����ָ���
    kv tmp[maxn]; //�鲢ʱ���ݴ�����
    void mergeSort(int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        int i = l, j = m + 1, k = l, lowerBound = l, upperBound = l;
        //�ڵݹ鷵�ص�����������������ʱ,��¼������������ÿ���ڵ�����������С�ںʹ��ڸõ����Ŀ
        for (int t = m + 1; t <= r; ++t) {
            while (lowerBound <= m && ins[lowerBound].v < ins[t].v) ++lowerBound; //ins[lowerBound].v>=ins[t].v
            while (upperBound <= m && ins[upperBound].v <= ins[t].v) ++upperBound; //ins[upperBound].v>ins[t].v
            Left[ins[t].p] += (lowerBound - l); //��������С��ins[t].v�ĸ���
            Right[ins[t].p] += (m - upperBound + 1); //�������д���ins[t].v�ĸ���
        }
        while (i <= m && j <= r) {
            if (ins[i].v <= ins[j].v) tmp[k++] = ins[i++];  //��������Ԫ��ֵ<=��������Ԫ��ֵ
            else tmp[k++] = ins[j++]; //������������ֵ:�±�
        }
        while (i <= m) tmp[k++] = ins[i++];
        while (j <= r) tmp[k++] = ins[j++];
        for (int x = l; x <= r; ++x) ins[x] = tmp[x];
    } 
public:
    int createSortedArray(vector<int>& instructions) {
        int n = instructions.size(), ans = 0, mod = 1e9 + 7;
        for (int i = 0; i < n; ++i) {
            ins[i].v = instructions[i];
            ins[i].p = i; //��¼ԭ�����е�ֵ:�±�ӳ�䣨����һ�Σ����������� 
        }
        mergeSort(0, n - 1);
        for (int i = 0; i < n; ++i) {//��������ԭ������±��Ӧ��ֵ,�������ʱ�Ĵ���
            // cout << Left[i] << ", " << Right[i] << endl;
            ans = (ans + min(Left[i], Right[i])) % mod;
        }
        cout << endl;
        return ans;
    }
};


//-----------��������� 
//Problem: Given an array A of n integers, find the sum of the number of inversions in all subarrays of length k. To clarify, one must determine the number of inversions in each of the n �C k + 1 subarrays of length k and add them together.
//Input: The first line contains two space separated integers n and k. The next line contains a sequence of n space separated integers where the ith integer in the sequence is A[i].
//Examples: 
//
//Input : arr[] = {1 2 3 4 5 6}
//        k = 2
//Output : 0
//
//Input : arr[] = {1 6 7 2}
//        k = 3
//Output : 2
//There are two subarrays of size 3,
//{1, 6, 7} and {6, 7, 2}. Count of
//inversions in first subarray is 0
//and count of inversions in second
//subarray is 2. So sum is 0 + 2 = 2
//
//Input :  8 4
//12 3 14 8 15 1 4 22
//Output : 14
//
//Input :  10 5
//15 51 44 44 76 50 29 88 48 50
//Output : 25
//
//Recommended: Please solve it on ��PRACTICE �� first, before moving on to the solution. 
// 
//
//[1] Naive Approach
//This problem seems fairly trivial at first, and we can easily implement a naive algorithm to brute force the solution. We simply create a window of length k and roll the window along A, adding the number of inversions in the window at each iteration. To find the number of inversions, the simplest approach is to use two for loops to consider all pairs of elements and increment a counter if the pair forms an inversion. 
//
//This approach is very easy to implement, but is it efficient? Let��s analyze the algorithm. The outermost loop runs n �C k + 1 times, once for each k-subarray of A. At each of these iterations, we find the number of inversions in the window. To do this, we consider element 1 and elements 2, ��, n, then element 2 and elements 3, ��, n until element n �C 1 and element n. Effectively, we��re performing n + (n �C 1) + (n �C 2) + �� + 1 = n(n + 1)/2 operations. Thus, our algorithm performs approximately (n �C k + 1)(n)(n + 1)/2 operations which is O(n^3 �C kn^2). Since k can range from 1 to n, the worst case performance for this algorithm is O(n^3) when k = n/2. We can do better! 

// C++ implementation of above approach
#include <iostream>
using namespace std;
 
// Helper function, counts number of inversions
// via bubble sort loop
int bubble_count(int arr[], int start, int end)
{
    int count = 0;
    for (int i = start; i < end; i++)
	    {
        for (int j = i + 1; j < end; j++)
        {
            if (arr[i] > arr[j])
            {
                count++;
            }
        }
    }
    return count;
}
 
// Inversion counting method, slides window of
// [start, end] across array
int inversion_count(int n, int k, int a[])
{
    int count = 0;
    for (int start = 0; start < n - k + 1; start++)
    {
        int end = start + k;
        count += bubble_count(a, start, end);
    }
    return count;
}
 
// Driver Code
int main()
{
    int n = 10;
    int arr[n] = { 15, 51, 44, 44, 76,
                   50, 29, 88, 48, 50 };
    int k = 5;
     
    int result = inversion_count(n, k, arr);
    cout << result;
    return 0;
}
 
// This code is contributed by PrinciRaj1992

Output: 

 25

[2] Mergesort-based Implementation
One optimization we can make is improving our inefficient, quadratic-time inversion counting method. One approach could involve using a mergesort-based method as outlined in this article. Since this runs in O(nlogn), our overall runtime is reduced to O(n^2logn), which is better, but still won��t be able to handle cases for which n = 10^6 for instance.  

import java.util.*;
 
public class Subarray_Inversions {
 
    // Inversion counting method, slides window of [start,
    // end] across array
    static int inversion_count(int n, int k, int[] a)
    {
        int count = 0;
        for (int start = 0; start < n - k + 1; start++) {
            int[] sub_array = new int[k];
            for (int i = start; i < start + k; i++) {
                sub_array[i - start] = a[i];
            }
            count += subarray_inversion_count(sub_array);
        }
        return count;
    }
 
    // Counts number of inversions when merging
    public static long merge_inversion_count(int[] arr,
                                int[] left, int[] right)
    {
        int i = 0, j = 0, count = 0;
        while (i < left.length || j < right.length) {
            if (i == left.length) {
                arr[i + j] = right[j];
                j++;
            } else if (j == right.length) {
                arr[i + j] = left[i];
                i++;
            } else if (left[i] <= right[j]) {
                arr[i + j] = left[i];
                i++;
            } else {
                arr[i + j] = right[j];
                count += left.length - i;
                j++;
            }
        }
        return count;
    }
 
    // Divide and conquer approach -- splits array and counts
    // inversions via merge method
    public static long subarray_inversion_count(int[] arr)
    {
        if (arr.length < 2)
            return 0;
 
        int m = (arr.length + 1) / 2;
        int left[] = Arrays.copyOfRange(arr, 0, m);
        int right[] = Arrays.copyOfRange(arr, m, arr.length);
 
        return subarray_inversion_count(left) +
               subarray_inversion_count(right) +
               merge_inversion_count(arr, left, right);
    }
 
    public static void main(String[] args)
    {
        int n = 10;
        int[] arr = { 15, 51, 44, 44, 76, 50, 29, 88, 48, 50 };
        int k = 5;
 
        long result = inversion_count(n, k, arr);
        System.out.println(result);
    }
}

Output: 

 25
//-------------------------------------------------- 
//[3] Overlapping Subarrays Implementation
//Let��s revisit our overall approach. We��re looking at the window [0, k) and finding the number of inversions, then we look at [1, k+1). There��s a significant overlap in this range: we��ve already counted the number of inversions in [1, k) during the first iteration, and now we��re counting them again! Instead of counting inversions, let��s count the change in inversions from one window to the next. Effectively, shifting the window is just adding one element to the head of the window and removing an element from its tail �� the body of the window remains the same. Checking for inversions among internal elements would be redundant; all we need to do is add the number of inversions induced by the new element and subtract the number of inversions induced by the removed element. We now only need to count the number of inversions in the first window, which we can do in O(klogk) time, and for each of the n �C k additional windows, we simply perform one iteration through the k elements in the array to find the change in the number of inversions. Our overall runtime is now O(k(n �C k) + klogk) = O(nk �C k) which is still worst case O(n^2). 

import java.util.*;
 
public class Subarray_Inversions {
 
    // Inversion counting method, slides window of [start,
    // end] across array
    static long inversion_count(int n, int m, int[] arr)
    {
        long count = 0;
        count += subarray_inversion_count_initial(Arrays.copyOfRange(arr, 0, m));
        long subarray_count = subarray_inversion_count_initial(Arrays.copyOfRange(arr, 1, m));
        for (int start = 1; start <= n - m; start++) {
            int end = start + m - 1;
            long[] ans = subarray_inversion_count(arr, start, end, subarray_count);
            count += ans[0];
            subarray_count = ans[1];
        }
        return count;
    }
 
    // start >=1; find inversion in interval [start, end)
    public static long[] subarray_inversion_count(int[] arr, int start,
                                          int end, long subarray_count)
    {
        int new_element = arr[end];
        long count = subarray_count;
        for (int i = start; i < end; i++) {
            if (new_element < arr[i])
                count++;
        }
        long totalSum = count;
        int last_element = arr[start];
        for (int i = start + 1; i <= end; i++) {
            if (last_element > arr[i])
                count--;
        }
        long[] ans = { totalSum, count };
        return ans;
    }
 
    // Counts number of inversions when merging
    public static long merge_inversion_count(int[] arr, int[] left,
                                                       int[] right)
    {
        int i = 0, j = 0, count = 0;
        while (i < left.length || j < right.length) {
            if (i == left.length) {
                arr[i + j] = right[j];
                j++;
            } else if (j == right.length) {
                arr[i + j] = left[i];
                i++;
            } else if (left[i] <= right[j]) {
                arr[i + j] = left[i];
                i++;
            } else {
                arr[i + j] = right[j];
                count += left.length - i;
                j++;
            }
        }
        return count;
    }
 
    // Divide and conquer approach -- splits array and counts
    // inversions via merge method
    public static long subarray_inversion_count_initial(int[] arr)
    {
        if (arr.length < 2)
            return 0;
 
        int m = (arr.length + 1) / 2;
        int left[] = Arrays.copyOfRange(arr, 0, m);
        int right[] = Arrays.copyOfRange(arr, m, arr.length);
 
        return subarray_inversion_count_initial(left) +
               subarray_inversion_count_initial(right) +
               merge_inversion_count(arr, left, right);
    }
 
    public static void main(String[] args) throws Exception
    {
        int n = 10;
        int[] arr = { 15, 51, 44, 44, 76, 50, 29, 88, 48, 50 };
        int k = 5;
 
        long result = inversion_count(n, k, arr);
        System.out.println(result);
    }
}
//
//Output: 
//
// 25
//
//[4] Binary Indexed Tree Implementation(ÿ�μ����µ�Ԫ��,�����µ������,��ȥ�ɵ������)
//Iterating over each window seems inevitable, so the bottleneck here appears to be the way we 
//handle the windows. We know that consecutive windows overlap significantly, so all we need 
//to know is the number of elements greater than the newly added element and number of elements 
//less than the newly removed element. Many times, algorithms that perform the same or similar 
//operation(s) repeatedly can be improved by the use of a more robust data structure. In our case, 
//we��re looking for a dynamic data structure that can efficiently answer queries about an element��s r
//elative position when sorted. We can use a self-balancing binary tree to actually maintain a 
//sorted list, but insertion/removal takes logarithmic time. We can do this in constant time using a Binary Indexed Tree, or Fenwick Tree. 
//
//A binary indexed tree is a tree represented in the form of an array. It uses clever bit manipulation to compute the 
//cumulative sum of elements very efficiently. We can call the function update(index, val) function to add 
//val to BIT[index] and all of the ancestors of index. 
//The function read(index) returns the sum of the values stored at BIT[index] and all of the ancestors of index 
//in the tree. Thus, calling read(index) returns the cumulative sum of all elements in BIT less than or equal 
//to index. Instead of storing values, if we simply store 1, we can use read(index + 1) to determine the number 
//of elements less than index. Now, we can construct a binary indexed tree by inserting the elements 
//(updating) of the first window. For subsequent windows, we can remove the trailing element by calling update(tail_element, -1) 
//and add the new element with update(head_element, 1). Since this is a tree, the longest possible root-node path 
//is O(logk), Thus, we achieve an optimal runtime of O(nlogk + klogk) = O(nlogk)!
//
//Or do we��? Remember, binary indexed trees allocate memory for every possible value in the range [0, max_element], so this 
//requires O(max_element) time and space. For very sparse arrays, this can be quite costly. Instead, we can define a hash function to . We can do this because we��re only concerned about inversions �� as long as we keep the relative magnitude the same (i.e. A[i] <= A ==> A[hash(i)] <= A[hash(j)]), our solution will still be correct. Thus, we can map all the elements in A to the set {0, 1, 2, ��, n}, yielding a guaranteed runtime of O(nlogk).

import java.util.*;
 
public class Subarray_Inversions {
 
    // Declare binary indexed tree with global scope
    static BIT bit;
 
    // first window, counts first k elements and creates
    // BIT
    static long inversionCountBIT1(int[] arr, int start,
                                               int end)
    {
        bit = new BIT(arr.length);
        long count = 0;
        for (int index = start; index >= end; index--) {
            count += bit.read(arr[index]);
            bit.update(arr[index], 1);
        }
        return count;
    }
 
    // subsequent windows, removes previous element, adds
    // new element, and increments change in inversions
    static long inversionCountBIT2(int[] arr, int start,
                                       int end, long val)
    {
        bit.update(arr[start + 1], -1); // remove trailing element
 
        // find number of elements in range [start, end-1]
        // greater than first
        int numGreaterThanFirst = start - end - bit.read(arr[start + 1] + 1);
        long count = val + bit.read(arr[end]) - numGreaterThanFirst;
        bit.update(arr[end], 1); // adds leading element
 
        return count;
    }
 
    // Main method to count inversions in size k subarrays
    public static long inversionCount(int n, int k, int[] arr)
    {
        bit = new BIT(n);
        HashMap<Integer, Integer> freq = new HashMap<Integer, Integer>();
        int[] asort = arr.clone();
 
        // Map elements from [A[0]...A[n-1]] to [1...n]
        Arrays.sort(asort);
        int index = 0;
        int current = 1;
        for (int i = 0; i < n; i++) {
            if (!freq.containsKey(asort[i])) {
                freq.put(asort[i], current);
                current++;
            }
        }
        for (int i = 0; i < n; i++) {
            arr[i] = freq.get(arr[i]);
        }
 
        long count = 0;
        long val = 0;
 
        //[start - end] ==> start - end = k+1
        for (int start = n - 1; start >= k - 1; start--) {
            int end = start - k + 1;
            if (start == n - 1) { // First pass
                val = inversionCountBIT1(arr, n - 1, n - k);
            } else { // subsequent passes
                val = inversionCountBIT2(arr, start, end, val);
            }
            count += val;
        }
        return count;
    }
 
    public static void main(String[] args) throws Exception
    {
        int n = 10;
        int[] arr = { 15, 51, 44, 44, 76, 50, 29, 88, 48, 50 };
        int k = 5;
 
        long result = inversionCount(n, k, arr);
        System.out.println(result);
    }
 
    // Implementation of Binary Indexed Tree
    static class BIT {
        int[] tree;
        int maxVal;
    public BIT(int N)
        {
            tree = new int[N + 1];
            maxVal = N;
        }
 
        // Updates BIT, starting with element at index
        // and all ancestors
        void update(int index, int val)
        {
            while (index <= maxVal) {
                tree[index] += val;
                index += (index & -index);
            }
        }
 
        // Returns the cumulative frequency of index
        // starting with element at index and all ancestors
        int read(int index)
        {
            --index;
            int cumulative_sum = 0;
            while (index > 0) {
                cumulative_sum += tree[index];
                index -= (index & -index);
            }
            return cumulative_sum;
        }
    };
}

//Output: 
//
// 25

//----------------------------------------------------
//




