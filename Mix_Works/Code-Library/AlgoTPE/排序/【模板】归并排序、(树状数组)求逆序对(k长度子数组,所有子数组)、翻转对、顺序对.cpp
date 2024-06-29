/*分治的三步法:
划分问题：把序列分成元素个数尽量相等的两半。
递归求解：把两半元素分别排序。
合并问题：把两个有序表合并成一个。

关键在于如何把两个有序表合成一个。
每次只需要把两个序列的最小元素加以比较，删除其中的较小元素并加入合并后的新表即可。
由于需要一个新表来存放结果，所以附加空间为n.

归并排序的时间复杂度和最大连续和的分治算法一样，都是O(nlogn)的
*/
void mergeSort(int *A, int x, int y, int *T) { //归并排序左闭右开区间[x,y)
	if (y - x > 1) { //多于一个元素
        int m = x + (y - x) / 2; //划分
        mergeSort(A, x, m, T);   //递归求解
        mergeSort(A, m, y, T);   //递归求解
        int p = x, q = m, i = x;
        while (p < m || q < y) { //右开区间，只要有一个序列非空，就要继续合并
            //精妙！！！！！ 
			//(q完成了&&p没有完成)||(q没有完成&&p没有完成&&A[p]<=A[q]
            if (q >= y || (p < m && A[p] <= A[q])) T[i++] = A[p++]; //从i开始 //从左半数组复制到临时空间
			//q没有完成&&(p完成了||A[p]>A[q])
			else T[i++] = A[q++]; //从右半数组复制到临时空间
		}
		for (int i = x; i < y; ++i) A[i] = T[i]; //从辅助空间复制回A数组
	}
}
//另一种归并写法:
void mergeSort(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSort(A, l, m, T);     //递归求解
		mergeSort(A, m + 1, r, T); //递归求解
		int i = l, j = m + 1, k = i;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			//i<j && A[i]>A[j]
			else T[k++] = A[j++];
			//当右边的A[j]复制到T中时，左边还没来得及复制到T的那些数就是左边所有比A[j]大的数
			//此时在累加器中加上左边元素个数m-i+1即可
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = l; x <= r; ++x) A[x] = T[x];
	}
}
//另一种归并写法:
void merge(int a[], int l1, int r1, int l2, int r2, int t[]) {
	int index = 0, p = l1, q = l2;
	while (p <= r1 && q <= r2) {
		while (p <= r1 && a[p] <= a[q]) { //为<=
			t[index++] = a[p++];
		}
		while (q <= r2 && a[q] <= a[p]) { //为<=
			t[index++] = a[q++];
		}
	}
	while (p <= r1) t[index++] = a[p++];
	while (q <= r2) t[index++] = a[q++];
	for (int i = 0; i < index; ++i) a[l1 + i] = t[i]; //将合并后的序列赋值回数组A
}
void mergeSort(int a[], int s, int e) {
	if (s < e) { //至少还有两个元素
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
/*逆序对问题。给一列数a1, a2, … , an，求它的逆序对数，即有多少个有序对(i,j)，
使得i<j但a_i>a_j。n可以高达10^6
1.n这么大，O(n^2)的枚举将超时，因此需要寻找更高效的方法
2.受到分治求最大连续和、分治归并排序的影响，试试“分治三步法”是否适用
- “划分问题”过程是把序列分成元素个数尽量相等的两半；
- “递归求解”是统计i和j均在左边或者均在右边的逆序对个数；
- “合并问题”则是统计i在左边，但j在右边的逆序对个数。

关键在于合并：如何求出i在左边，而j在右边的逆序对数目呢？

按照j的不同把这些“跨越两边”的逆序对进行分类：
只要对于右边的每个j，统计左边比它大的元素个数f(j)，则所有f(j)之和便是答案。
归并排序可以“顺便”完成f(j)的计算：
由于合并操作是从小到大进行的，当右边的A[j]复制到T中时，左边还没来得及复制到T的那些数就是左边所有比A[j]大的数。此
时在累加器中加上左边元素个数m-p即可（左边所剩的元素在区间[p,m)中，因此元素个数为m-p）
*/
void mergeSortAndCount(int *A, int x, int y, int *T) { //[x,y)
	if (y - x > 1) {
		int m = x + (y - x) / 2;
		mergeSortAndCount(A, x, m, T);
		mergeSortAndCount(A, m, y, T);
		int p = x, q = m, i = x;
		while (p < m || q < y) {
			if (q >= y || (p < m && A[p] <= A[q])) T[i++] = A[p++];
			//q没有完成&&(p完成了||A[p]>A[q])
			else {
				T[i++] = A[q++];
				cnt += m - p; //左边还没来得及复制到T的那些数就是左边所有比A[j]大的数。
				//此时在累加器中加上左边元素个数m-p即可
			}
		}
		for (int i = x; i < y; ++i) A[i] = T[i];
	}
}
//其他写法2
void mergeSortAndCount2(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAndCount2(A, l, m, T);     //递归求解
		mergeSortAndCount2(A, m + 1, r, T); //递归求解
		int i = l, j = m + 1, k = i;
		while (i <= m && j <= r) {
			if (A[i] <= A[j]) T[k++] = A[i++];
			//i<j && A[i]>A[j]
			else { T[k++] = A[j++]; cnt += j - k; } //mid - i + 1: 逆序对; i - l: 顺序对 
			//当右边的A[j]复制到T中时，左边还没来得及复制到T的那些数就是左边所有比A[j]大的数
			//此时在累加器中加上左边元素个数m-i+1即可
		}
		while (i <= m) T[k++] = A[i++];
		while (j <= r) T[k++] = A[j++];
		for (int x = l; x <= r; ++x) A[x] = T[x];
	}
}
//但凡是用于交换的排序，都可以求逆序对？快排不可以，堆排不可以？插入、冒泡
//其他写法3
int cnt = 0;
void mergeSortAndCount3(int *A, int l, int r, int *T) { //[l,r]
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAndCount3(A, l, m, T);
		mergeSortAndCount3(A, m + 1, r, T);
		int i = l, j = m + 1, k = l;

		//按照i的不同把这些“跨越两边”的逆序对进行分类：
		//只要对于左边的每个i，统计右边比它小的元素个数f(j)，则所有f(j)之和便是答案。
		//归并排序可以“顺便”完成f(j)的计算
		for (i = l; i <= m; ++i) {
			while (j <= r && A[i] > A[j]) ++j; //直到A[i]<=A[j]
			cnt += j - m - 1; //A[i]>所有[m+1,j)的数A[j]: 逆序对; A[i]<=所有[j, r]的数A[j]: 顺序对 
		}
		//或者按照j的不同把这些“跨越两边”的逆序对进行分类：
		//只要对于右边的每个j，统计左边比它大的元素个数f(j)，则所有f(j)之和便是答案。（都向前寻找） 
		//归并排序可以“顺便”完成f(j)的计算
		for (j = m + 1; j <= r; ++j) {
			while (i <= m && A[i] <= A[j]) ++i; //直到A[i]>A[j]
			cnt += m - i + 1; //[i,m]中的A[i]都>A[j]: 逆序对; [l, i - 1]中的A[i]都<=A[j], 顺序对 
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



//严格顺序对（重点）则必须这样做！ LeetCode，注意等于arr[i]的元素数目！！！ 
struct kv { int v, p; };
const int maxn = 1e5 + 10;
class Solution {
private:
    kv ins[maxn]; //记录元素值和其在原数组中的下标
    int Left[maxn] = {0}, Right[maxn] = {0}; //记录原数组中的下标对应的值,在有序插入时小于或者大于该值的数字个数
    kv tmp[maxn]; //归并时的暂存数组
    void mergeSort(int l, int r) {
        if (l >= r) return;
        int m = l + (r - l) / 2;
        mergeSort(l, m);
        mergeSort(m + 1, r);
        int i = l, j = m + 1, k = l, lowerBound = l, upperBound = l;
        //在递归返回的左右子树数组有序时,记录右子树数组中每个节点在左子树中小于和大于该点的数目
        for (int t = m + 1; t <= r; ++t) {
            while (lowerBound <= m && ins[lowerBound].v < ins[t].v) ++lowerBound; //ins[lowerBound].v>=ins[t].v
            while (upperBound <= m && ins[upperBound].v <= ins[t].v) ++upperBound; //ins[upperBound].v>ins[t].v
            Left[ins[t].p] += (lowerBound - l); //左子树中小于ins[t].v的个数
            Right[ins[t].p] += (m - upperBound + 1); //左子树中大于ins[t].v的个数
        }
        while (i <= m && j <= r) {
            if (ins[i].v <= ins[j].v) tmp[k++] = ins[i++];  //左子树的元素值<=右子树的元素值
            else tmp[k++] = ins[j++]; //复制右子树的值:下标
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
            ins[i].p = i; //记录原数组中的值:下标映射（离线一次！！！！！） 
        }
        mergeSort(0, n - 1);
        for (int i = 0; i < n; ++i) {//遍历所有原数组的下标对应的值,有序插入时的代价
            // cout << Left[i] << ", " << Right[i] << endl;
            ans = (ans + min(Left[i], Right[i])) % mod;
        }
        cout << endl;
        return ans;
    }
};


//-----------逆序对问题 
//Problem: Given an array A of n integers, find the sum of the number of inversions in all subarrays of length k. To clarify, one must determine the number of inversions in each of the n – k + 1 subarrays of length k and add them together.
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
//Recommended: Please solve it on “PRACTICE ” first, before moving on to the solution. 
// 
//
//[1] Naive Approach
//This problem seems fairly trivial at first, and we can easily implement a naive algorithm to brute force the solution. We simply create a window of length k and roll the window along A, adding the number of inversions in the window at each iteration. To find the number of inversions, the simplest approach is to use two for loops to consider all pairs of elements and increment a counter if the pair forms an inversion. 
//
//This approach is very easy to implement, but is it efficient? Let’s analyze the algorithm. The outermost loop runs n – k + 1 times, once for each k-subarray of A. At each of these iterations, we find the number of inversions in the window. To do this, we consider element 1 and elements 2, …, n, then element 2 and elements 3, …, n until element n – 1 and element n. Effectively, we’re performing n + (n – 1) + (n – 2) + … + 1 = n(n + 1)/2 operations. Thus, our algorithm performs approximately (n – k + 1)(n)(n + 1)/2 operations which is O(n^3 – kn^2). Since k can range from 1 to n, the worst case performance for this algorithm is O(n^3) when k = n/2. We can do better! 

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
One optimization we can make is improving our inefficient, quadratic-time inversion counting method. One approach could involve using a mergesort-based method as outlined in this article. Since this runs in O(nlogn), our overall runtime is reduced to O(n^2logn), which is better, but still won’t be able to handle cases for which n = 10^6 for instance.  

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
//Let’s revisit our overall approach. We’re looking at the window [0, k) and finding the number of inversions, then we look at [1, k+1). There’s a significant overlap in this range: we’ve already counted the number of inversions in [1, k) during the first iteration, and now we’re counting them again! Instead of counting inversions, let’s count the change in inversions from one window to the next. Effectively, shifting the window is just adding one element to the head of the window and removing an element from its tail — the body of the window remains the same. Checking for inversions among internal elements would be redundant; all we need to do is add the number of inversions induced by the new element and subtract the number of inversions induced by the removed element. We now only need to count the number of inversions in the first window, which we can do in O(klogk) time, and for each of the n – k additional windows, we simply perform one iteration through the k elements in the array to find the change in the number of inversions. Our overall runtime is now O(k(n – k) + klogk) = O(nk – k) which is still worst case O(n^2). 

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
//[4] Binary Indexed Tree Implementation(每次加入新的元素,计算新的逆序对,减去旧的逆序对)
//Iterating over each window seems inevitable, so the bottleneck here appears to be the way we 
//handle the windows. We know that consecutive windows overlap significantly, so all we need 
//to know is the number of elements greater than the newly added element and number of elements 
//less than the newly removed element. Many times, algorithms that perform the same or similar 
//operation(s) repeatedly can be improved by the use of a more robust data structure. In our case, 
//we’re looking for a dynamic data structure that can efficiently answer queries about an element’s r
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
//Or do we…? Remember, binary indexed trees allocate memory for every possible value in the range [0, max_element], so this 
//requires O(max_element) time and space. For very sparse arrays, this can be quite costly. Instead, we can define a hash function to . We can do this because we’re only concerned about inversions — as long as we keep the relative magnitude the same (i.e. A[i] <= A ==> A[hash(i)] <= A[hash(j)]), our solution will still be correct. Thus, we can map all the elements in A to the set {0, 1, 2, …, n}, yielding a guaranteed runtime of O(nlogk).

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




