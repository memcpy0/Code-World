

Larry just studied the algorithm to count number of inversions. He's very interested in it. He's considering another problem: Given a permutation of integers from 1 to n, how many inversions it has if we reverse one of its subarray?

Formally speaking, given an integer array $a$ (indices are from 0 to $n−1$) which contains a permutation of integers from 1 to $n$, two elements $a[i]$ and $a[j]$ form an **inversion** if $a[i]>a[j]$ and $i<j$. Your job is to count, for each pair of $0≤i≤j<n$, the number of inversions if we reverse the subarray from $a[i]$ to $a[j]$.
## Input Specification:

Each input file contains one test case. Each case consists of a positive integer $n$ ($≤1,000$) in the first line, and a permutation of integers from 1 to $n$ in the second line. The numbers in a line are separated by a single space.
## Output Specification:

For each test case, output $n(n+1)/2$ integers in a single line. The results are for reversing subarray indicating by all possible pairs of indices $0≤i≤j<n$ in i-major order -- that is, the first $n$ results are for the reverse of subarrary [0..0], [0..1], ...[0..$n−1$]; the next $n−1$ results are for the reverse of subarry [1..1], [1..2],..., [1..$n−1$] and so on.

All the numbers in a line must be separated by a single space, with no extra space at the beginning or the end of the line.
## Sample Input:

```go
3
2 1 3
```

## Sample Output:

```go
1 0 2 1 2 1
```

## Hint: 
The original array is { 2, 1, 3 }.
-  Reversing subarray [0..0] makes { 2, 1, 3 } which has 1 inversion.
 -  Reversing subarray [0..1] makes { 1, 2, 3 } which has 0 inversion.
 -  Reversing subarray [0..2] makes { 3, 1, 2 } which has 2 inversions.
 -  Reversing subarray [1..1] makes { 2, 1, 3 } which has 1 inversion.
 -  Reversing subarray [1..2] makes { 2, 3, 1 } which has 2 inversions.
 -  Reversing subarrays [2..2] makes { 2, 1, 3 } which has 1 inversion.



---
很快有时间了就更新题意和解题思路：

```cpp
#include <bits/stdc++.h>
using namespace std;
#define lowbit(x) ((x) & -(x))
const int maxn = 1010;
int tree[maxn], n, arr[maxn], tot = 0; //inversions[i]是arr[i]相对于arr[1:i)形成的逆序对
vector<int> ans;
void insert(int i, int d) { while (i < maxn) tree[i] += d, i += lowbit(i); }
int sum(int i) { //得到<=i的数目
    int ans = 0;
    while (i) ans += tree[i], i -= lowbit(i);
    return ans;
}
//原来的逆序对数目-翻转前子数组中的逆序对数目+翻转后子数组中的逆序对数目=新数组的逆序对数目
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) { 
        scanf("%d", &arr[i]);
        insert(arr[i], 1); //arr[i]的数目+1 //找到i之前>arr[i]的数字的数目
        tot += i - sum(arr[i]); //目前的总数减去<=arr[i]的数的数目
    }
    for (int i = 1; i <= n; ++i) { //O(n^2 logk)
    	ans.push_back(tot); //只翻转1个数字,相当于原数组不变,使用原来的逆序对数目
    	memset(tree, 0, sizeof(tree));
    	insert(arr[i], 1);
    	int subarrTot = 0;
        for (int j = i + 1; j <= n; ++j) {
        	insert(arr[j], 1); 
			subarrTot += j - i + 1 - sum(arr[j]); 
			ans.push_back(tot - subarrTot + (j - i + 1) * (j - i) / 2 - subarrTot);
        }
    }
    for (int i = 0; i < ans.size(); ++i) printf(" %d" + !i, ans[i]);
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522193145582.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)


