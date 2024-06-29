

According to Wikipedia: "In mathematics and in particular in combinatorics, the **Lehmer code** is a particular way to encode each possible permutation of a sequence of $n$ numbers." To be more specific, for a given permutation of items {$A_1, A_2, \dots, A_n$}, Lehmer code is a sequence of numbers {$L_1, L_2, \dots, L_n$​​} such that $L_​i$​​ is the total number of items from $A_​i$​​ to $A_​n$​​ which are less than $A​_i$​​. For example, given { 24, 35, 12, 1, 56, 23 }, the second Lehmer code $L_2$​​ is 3 since from 35 to 23 there are three items, { 12, 1, 23 }, less than the second item, 35.
## Input Specification:

Each input file contains one test case. For each case, the first line gives a positive integer $N$ ($≤10^5$​​). Then $N$ distinct numbers are given in the next line.
## Output Specification:

For each test case, output in a line the corresponding Lehmer code. The numbers must be separated by exactly one space, and there must be no extra space at the beginning or the end of the line.
## Sample Input:

```go
6
24 35 12 1 56 23
```

## Sample Output:

```go
3 3 1 0 1 0
```
---
很快有时间了更新题意和思路：
```cpp
#include <bits/stdc++.h>
#define lowbit(x) ((x) & -(x))
using namespace std;
const int maxn = 1e5 + 10; //orders[i]表示后面<arr[i]形成逆序对的数目
int n, arr[maxn], temp[maxn], tree[maxn], orders[maxn];
void insert(int i, int d) { while (i < maxn) { tree[i] += d; i += lowbit(i); } }
int sum(int i) { 
    int ans = 0;
    while (i) { ans += tree[i]; i -= lowbit(i); }
    return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d", &arr[i]); //n个不同的number
    memcpy(temp, arr, sizeof(arr));
    sort(temp, temp + n); //排序,不需要去重,因为已经无重复了; 再将arr[]映射为1-n的整数
    for (int i = 0; i < n; ++i) 
        arr[i] = lower_bound(temp, temp + n, arr[i]) - temp + 1;
    for (int i = n - 1; i >= 0; --i) {
        insert(arr[i], 1);
        orders[i] = sum(arr[i] - 1); //后面<=arr[i]-1即<arr[i],和arr[i]形成逆序对的数目 
    }
    for (int i = 0; i < n; ++i) printf(" %d" + !i, orders[i]);
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522192212488.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

