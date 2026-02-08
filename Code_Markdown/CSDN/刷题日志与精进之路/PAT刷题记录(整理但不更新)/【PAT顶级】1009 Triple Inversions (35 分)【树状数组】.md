Given a list of $N$ integers $A_1​​, A_2, A_3, \dots, A_N$ there's a famous problem to count the number of inversions in it. An inversion is defined as a piar of indices $i<j$ such that $A_i​​>A​_j$ ​​.

Now we have a new challenging problem. You are supposed to count the number of triple inversions in it. As you may guess, a triple inversion is defined as a triple of indices $i<j<k$ such that $A_i​>A_j​​>A_k$ ​​. For example, in the list { 5, 1, 4, 3, 2 } there are 4 triple inversions, namely (5,4,3), (5,4,2), (5,3,2) and (4,3,2). To simplify the problem, the list $A$ is given as a permutation of integers from $1$ to $N$.
## Input Specification:

Each input file contains one test case. For each case, the first line gives a positive integer $N$ in $[3,10​^5​]$ . The second line contains a permutation of integers from $1$ to $N$ and each of the integer is separated by a single space.
## Output Specification: 
For each case, print in a line the number of triple inversions in the list.
## Sample Input:

```clike
22
1 2 3 4 5 16 6 7 8 9 10 19 11 12 14 15 17 18 21 22 20 13
```

## Sample Output:

```clike
8
```

---
很快有时间了就更新题意和解题思路：
```cpp
#include <bits/stdc++.h>
#define lowbit(x) ((x) & -(x))
using namespace std;
const int maxn = 1e5 + 10; //invers[i]表示数组前面>arr[i]形成逆序对的数目,orders[i]表示后面<arr[i]形成逆序对的数目
int n, arr[maxn], tree[maxn], invers[maxn], orders[maxn];
long long ans = 0; 
void insert(int i, int d) { while (i < maxn) { tree[i] += d; i += lowbit(i); } }
int sum(int i) { 
    int ans = 0;
    while (i) { ans += tree[i]; i -= lowbit(i); }
    return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) { 
        scanf("%d", &arr[i]); //所有值都不同
        insert(arr[i], 1);
        invers[i] = i + 1 - sum(arr[i]); //前面和arr[i]形成逆序对的数目
    }
    memset(tree, 0, sizeof(tree));
    for (int i = n - 1; i >= 0; --i) {
        insert(arr[i], 1);
        orders[i] = sum(arr[i] - 1); //后面<=arr[i]-1即<arr[i],和arr[i]形成逆序对的数目 
    }
    for (int i = 1; i < n - 1; ++i) if (invers[i] && orders[i]) ans += (long long)invers[i] * orders[i];
    printf("%lld\n", ans);
    return 0;
}
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210522191454165.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L215UmVhbGl6YXRpb24=,size_16,color_FFFFFF,t_70)

